//
//  system_audio_macos.cpp
//  Pictel-sound-macos
//
//  Created by krzysp on 28/05/2022.
//

#include "system_audio_macos.h"
#import <Foundation/Foundation.h>
#import <AudioToolbox/AudioToolbox.h>
#import <MacTypes.h>

using namespace PictelSound;

#define PICTEL_BITS_PER_BYTE 8
#define PICTEL_BYTES_TO_BITS(bytes) ((bytes) * PICTEL_BITS_PER_BYTE)
#define PICTEL_OGG_VORBIS_WORDSIZE 2

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
{
    bzero(&m_audioDescription, PICTEL_DEFAULT_BUFFER_SIZE);
    bzero(&m_buffers, sizeof(m_buffers));
}

SystemAudio::~SystemAudio()
{
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
            UInt32 bufferSize = 128 * 1024;
            AudioQueueBufferRef buffer;
            OSStatus status = AudioQueueAllocateBuffer(queue, bufferSize, &buffer);
            if(status != noErr)
            { printf("Could not alocate buffer!\n");
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
//    self.state = IDZAudioPlayerStatePrepared;
//    return YES;
}

void SystemAudio::ReadBufferInto(void *ptr)
{
    if (m_decoder == nullptr)
    {   return;
    }

    AudioQueueBufferRef pBuffer = (AudioQueueBufferRef)ptr;
    unsigned int nTotalBytesRead = 0;
    m_decoder->ReadBuffer((char*)pBuffer->mAudioData,
                          (unsigned int)pBuffer->mAudioDataBytesCapacity,
                          &nTotalBytesRead);
    if (nTotalBytesRead < 1)
    {   printf("Did not read any bytes!\n");

        /*
         * Signal to the audio queue that we have run out of data,
         * but set the immediate flag to false so that playback of
         * currently enqueued buffers completes.
         */
//        self.state = IDZAudioPlayerStateStopping;
//        Boolean immediate = false;
//        AudioQueueStop(mQueue, immediate);

        return;
    }

    pBuffer->mAudioDataByteSize = nTotalBytesRead;
    pBuffer->mPacketDescriptionCount = 0;

    AudioQueueRef queue = (AudioQueueRef)m_queue;
    OSStatus status = AudioQueueEnqueueBuffer(queue, pBuffer, 0, 0);
    if(status != noErr)
    {
        NSLog(@"Error: %s status=%d", __PRETTY_FUNCTION__, (int)status);
    }

}

void SystemAudio::Play()
{
    AudioQueueRef queue = (AudioQueueRef)m_queue;
    OSStatus osStatus = AudioQueueStart(queue, NULL);
    printf("Started\n");
//    NSAssert(osStatus == noErr, @"AudioQueueStart failed");
//    self.state = IDZAudioPlayerStatePlaying;
//    self.playing = YES;
//    return (osStatus == noErr);
}

bool SystemAudio::QueryIsRunning()
{
    UInt32 oRunning = 0;
    UInt32 ioSize = sizeof(oRunning);
    AudioQueueRef queue = (AudioQueueRef)m_queue;
    OSStatus result = AudioQueueGetProperty(queue, kAudioQueueProperty_IsRunning, &oRunning, &ioSize);
    return oRunning;
}

double SystemAudio::GetDuration()
{
    return m_decoder->GetDuration();
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
//        NSLog(@"isRunning = %u", (unsigned int)isRunning);
//        BOOL bDidFinish = (pPlayer.playing && !isRunning);
//        pPlayer.playing = isRunning ? YES : NO;
//        if(bDidFinish)
//        {
//            [pPlayer.delegate audioPlayerDidFinishPlaying:pPlayer
//                                              successfully:YES];
//            /*
//             * To match AVPlayer's behavior we need to reset the file.
//             */
//            pPlayer.currentTime = 0;
//        }
//        if(!isRunning)
//            pPlayer.state = IDZAudioPlayerStateStopped;
    }
}
