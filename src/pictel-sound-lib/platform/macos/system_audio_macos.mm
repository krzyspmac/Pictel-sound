//
//  system_audio_macos.cpp
//  Pictel-sound-macos
//
// Copyright (c) 2022 Krzysztof Pawłowski
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "system_audio_macos.h"
#import <Foundation/Foundation.h>
#import <AudioToolbox/AudioToolbox.h>
#import <MacTypes.h>

using namespace PictelSound;

#define PICTEL_BITS_PER_BYTE 8
#define PICTEL_BYTES_TO_BITS(bytes) ((bytes) * PICTEL_BITS_PER_BYTE)
#define PICTEL_OGG_VORBIS_WORDSIZE 2
#define PICTEL_AUDIO_BUFFER_SIZE 128  * 1024

/** Static callbacks */
static void PictelOutputCallback(void *                  inUserData,
                                 AudioQueueRef           inAQ,
                                 AudioQueueBufferRef     inCompleteAQBuffer);

static void PictelPropertyListener(void*                 inUserData,
                                   AudioQueueRef         inAQ,
                                   AudioQueuePropertyID  inID);

SystemAudio::SystemAudio()
:   SystemAudioI()
,   m_decoder(NULL)
,   m_playerState(PLAYER_STOPPED)
,   m_loops(false)
{
    bzero(&m_audioDescription, PICTEL_DEFAULT_BUFFER_SIZE);
    bzero(&m_buffers, sizeof(m_buffers));
}

SystemAudio::~SystemAudio()
{
    Stop();
    Free();
}

void SystemAudio::SetState(PlayerState state)
{
    m_playerState = state;

    m_callbackStack.clear();
    for (auto it = m_callbacks.begin(); it != m_callbacks.end(); ++it)
    {
        m_callbackStack.emplace_back(it->get());
    }

    for (auto it = m_callbackStack.begin(); it != m_callbackStack.end(); ++it)
    {
        (*it)->PerformStateCallback(state);
    }
}

void SystemAudio::SetDecoder(DecoderI *decoder)
{
    do
    {
        // decoder store
        m_decoder = decoder;

        // stream description fillout
        auto description = (AudioStreamBasicDescription*)m_audioDescription;
        int bytesPerChannel = PICTEL_OGG_VORBIS_WORDSIZE;
        FillOutASBDForLPCM(*description,
                           (Float64)decoder->GetRate(),
                           (UInt32)decoder->GetChannels(), // channels per frame
                           (UInt32)PICTEL_BYTES_TO_BITS(bytesPerChannel), // valid bits per channel
                           (UInt32)PICTEL_BYTES_TO_BITS(bytesPerChannel), // total bits per channel
                           false, // isFloat
                           false); // isBigEndian

        // audio-queue setup
        AudioQueueRef queue = NULL;
        OSStatus status = AudioQueueNewOutput(description,
                                              PictelOutputCallback,
                                              this,
                                              CFRunLoopGetCurrent(),
                                              kCFRunLoopCommonModes,
                                              0,
                                              &queue);
        if (status != noErr)
        {   printf("Could not initialize the stream!\n");
            break;;
        }

        // audio-queue store
        m_queue = queue;

        // parameters setup
        AudioQueueSetParameter(queue, kAudioQueueParam_Volume, 1.0);

        // property listener
        status = AudioQueueAddPropertyListener(queue,
                                               kAudioQueueProperty_IsRunning,
                                               PictelPropertyListener,
                                               this);
        if (status != noErr)
        {   printf("Could not initialize property listener!\n");
            break;
        }

        // buffer setup
        for(int i = 0; i < PICTEL_BUFFER_COUNT; ++i)
        {
            UInt32 bufferSize = PICTEL_AUDIO_BUFFER_SIZE;
            AudioQueueBufferRef buffer;
            OSStatus status = AudioQueueAllocateBuffer(queue, bufferSize, &buffer);
            if(status != noErr)
            {   printf("Could not alocate buffer!\n");
                AudioQueueDispose(queue, true);
                m_queue = 0;
                return;
            }

            m_buffers[i] = buffer;
        }
    } while(false);
}

void SystemAudio::PrepareToPlay()
{
    for(int i = 0; i < PICTEL_BUFFER_COUNT; ++i)
    {
        AudioQueueBufferRef buffer = (AudioQueueBufferRef)m_buffers[i];
        ReadBufferInto(buffer);
    }

    SetState(PLAYER_PREPARED);
}

void SystemAudio::ReadBufferInto(void *ptr)
{
    if (m_decoder == nullptr)
    {   return;
    }

    if (m_playerState == PLAYER_STOPPING)
    {   return;
    }

    AudioQueueBufferRef pBuffer = (AudioQueueBufferRef)ptr;
    if (pBuffer == nullptr)
    {   return;
    }

    unsigned int nTotalBytesRead = 0;
    m_decoder->ReadBuffer((char*)pBuffer->mAudioData,
                          (unsigned int)pBuffer->mAudioDataBytesCapacity,
                          &nTotalBytesRead);
    if (nTotalBytesRead < 1)
    {   printf("Did not read any bytes!\n");
        AudioQueueStop((AudioQueueRef)m_queue, false);
        SetState(PLAYER_STOPPING);
        return;
    }

    pBuffer->mAudioDataByteSize = nTotalBytesRead;
    pBuffer->mPacketDescriptionCount = 0;
    printf("Read %d bytes\n", nTotalBytesRead);

    AudioQueueRef queue = (AudioQueueRef)m_queue;
    OSStatus status = AudioQueueEnqueueBuffer(queue, pBuffer, 0, 0);
    if(status != noErr)
    {
        printf("Error: %s status=%d\n", __PRETTY_FUNCTION__, (int)status);
    }
}

void SystemAudio::Play()
{
    switch (m_playerState)
    {
        case PLAYER_STOPPED:    break;
        case PLAYER_PREPARED:   break;
        case PLAYER_PLAYING:    return;
        case PLAYER_PAUSED:     break;
        case PLAYER_STOPPING:   return;
        case PLAYER_DISCARDED:  return;
    }

    AudioQueueRef queue = (AudioQueueRef)m_queue;
    OSStatus osStatus = AudioQueueStart(queue, NULL);
    if (osStatus != noErr)
    {   printf("Could not start audio queue.\n");
        return;
    }

    SetState(PLAYER_PLAYING);
}

void SystemAudio::Pause()
{
    if (m_playerState != PLAYER_PLAYING)
    {   return;
    }

    OSStatus osStatus = AudioQueuePause((AudioQueueRef)m_queue);
    if (osStatus != noErr)
    {   printf("Could not pause audio queue.\n");
        return;
    }

    SetState(PLAYER_PAUSED);
}

void SystemAudio::Stop()
{
    if (m_playerState != PLAYER_PLAYING)
    {   return;
    }

    SetState(PLAYER_STOPPING);

    OSStatus osStatus = AudioQueueStop((AudioQueueRef)m_queue, true);
    if (osStatus != noErr)
    {   printf("Could not stop audio queue, OSStatus=%d.\n", osStatus);
        return;
    }

    SignalDidFinish(false);
}

void SystemAudio::Free()
{
    AudioQueueRef queue = (AudioQueueRef)m_queue;
    if (queue == NULL)
    {   return;
    }

    for(int i = 0; i < PICTEL_BUFFER_COUNT; ++i)
    {
        AudioQueueBufferRef buffer = (AudioQueueBufferRef)m_buffers[i];
        if (buffer != nullptr)
        {   AudioQueueFreeBuffer(queue, buffer);
            m_buffers[i] = NULL;
        }
    }

    AudioQueueDispose(queue, true);
    m_queue = NULL;

    SetState(PLAYER_DISCARDED);
}

bool SystemAudio::QueryIsRunning()
{
    UInt32 oRunning = 0;
    UInt32 ioSize = sizeof(oRunning);
    AudioQueueRef queue = (AudioQueueRef)m_queue;
    AudioQueueGetProperty(queue, kAudioQueueProperty_IsRunning, &oRunning, &ioSize);
    return oRunning;
}

double SystemAudio::QueryPosition()
{
    AudioQueueRef queue = (AudioQueueRef)m_queue;
    if (queue == NULL)
    {   return -1;
    }

    double timeInterval = 0;
    AudioQueueTimelineRef timeLine;

    OSStatus status = AudioQueueCreateTimeline(queue, &timeLine);
    if( status != noErr) {
        printf("Could not get current time!\n");
        return -1;
    }

    AudioTimeStamp timeStamp;
    AudioQueueGetCurrentTime(queue, timeLine, &timeStamp, NULL);
    timeInterval = timeStamp.mSampleTime / (Float64)m_decoder->GetRate(); // modified
    return timeInterval;
}

double SystemAudio::GetDuration()
{
    return m_decoder->GetDuration();
}

PlayerState SystemAudio::GetState()
{
    return m_playerState;
}

void SystemAudio::SetVolume(double value)
{
    AudioQueueSetParameter((AudioQueueRef)m_queue, kAudioQueueParam_Volume, value);
}

void SystemAudio::SetLoops(bool loops)
{
    m_loops = loops;
}

void SystemAudio::AddCallback(PlayerCallbackI* callback)
{
    m_callbacks.emplace_back(std::move(callback));
}

PlayerCallbackI* SystemAudio::AddCallbackLambda(std::function<void(PlayerState)> lambda)
{
    auto callback = std::unique_ptr<PlayerCallbackI>(new PlayerCallbackLambda(lambda));
    m_callbacks.emplace_back(std::move(callback));
    return m_callbacks.at(m_callbacks.size()-1).get();
}

PlayerCallbackI *SystemAudio::AddCallbackFunction(void (*f)(PlayerState))
{
    auto callback = std::unique_ptr<PlayerCallbackI>(new PlayerCallbackFunction(f));
    m_callbacks.emplace_back(std::move(callback));
    return m_callbacks.at(m_callbacks.size()-1).get();
}

void SystemAudio::RemoveCallback(PlayerCallbackI* callback)
{
    for (auto it = m_callbacks.begin(); it != m_callbacks.end(); ++it)
    {
        if (it->get() == callback)
        {   m_callbacks.erase(it);
            return;
        }
    }
}

void SystemAudio::SignalDidFinish(bool canRestart)
{
    printf("Did finish signal received\n");
    SetState(PLAYER_STOPPED);
    m_decoder->Seek(0);
    PrepareToPlay();
    SetState(PLAYER_PREPARED);

    if (canRestart && m_loops)
    {   Play();
    }
}

/** Static callbacks */

void PictelOutputCallback(void *                  inUserData,
                          AudioQueueRef           inAQ,
                          AudioQueueBufferRef     inCompleteAQBuffer)
{
    SystemAudio* ptr = static_cast<SystemAudio*>(inUserData);
    ptr->ReadBufferInto(inCompleteAQBuffer);
}

static void PictelPropertyListener(void*                 inUserData,
                                   AudioQueueRef         inAQ,
                                   AudioQueuePropertyID  inID)
{
    SystemAudio* pPlayer = static_cast<SystemAudio*>(inUserData);
    printf("Property id = %d\n", inID);
    if(inID == kAudioQueueProperty_IsRunning)
    {
        bool isRunning = pPlayer->QueryIsRunning();//[pPlayer queryIsRunning];
        printf("IsRunning = %d\n", isRunning);
        printf("Time to end = %f\n", pPlayer->GetDuration());

        bool didFinish = pPlayer->GetState() == PLAYER_PLAYING && !isRunning;
        if (didFinish)
        {
        }

        if (!isRunning)
        {
            pPlayer->SignalDidFinish(true);
        }
    }
}
