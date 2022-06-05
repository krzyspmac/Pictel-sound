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

#include "system_audio_windows.h"
#include "system_audio_manager.h"

#define MA_NO_VORBIS    /* Disable the built-in Vorbis decoder to ensure the libvorbis decoder is picked. */
#define MA_NO_OPUS      /* Disable the (not yet implemented) built-in Opus decoder to ensure the libopus decoder is picked. */
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include "extras/miniaudio_libvorbis.h"

using namespace PictelSound;

typedef struct {
    ma_decoder_config m_decoderConfig;
    ma_decoder m_ma_decoder;
    ma_device_config m_deviceConfig;
    ma_device m_device;
    ma_format m_format;
    ma_uint32 m_channels;
    ma_uint32 m_sampleRate;
    ma_uint32 cursor;
} MiniAudioConfig;

static ma_result ma_decoding_backend_init__libvorbis(void* pUserData, ma_read_proc onRead, ma_seek_proc onSeek, ma_tell_proc onTell, void* pReadSeekTellUserData, const ma_decoding_backend_config* pConfig, const ma_allocation_callbacks* pAllocationCallbacks, ma_data_source** ppBackend)
{
    ma_result result;
    ma_libvorbis* pVorbis;

    (void)pUserData;

    pVorbis = (ma_libvorbis*)ma_malloc(sizeof(*pVorbis), pAllocationCallbacks);
    if (pVorbis == NULL) {
        return MA_OUT_OF_MEMORY;
    }

    result = ma_libvorbis_init(onRead, onSeek, onTell, pReadSeekTellUserData, pConfig, pAllocationCallbacks, pVorbis);
    if (result != MA_SUCCESS) {
        ma_free(pVorbis, pAllocationCallbacks);
        return result;
    }

    *ppBackend = pVorbis;

    return MA_SUCCESS;
}

static ma_result ma_decoding_backend_init_file__libvorbis(void* pUserData, const char* pFilePath, const ma_decoding_backend_config* pConfig, const ma_allocation_callbacks* pAllocationCallbacks, ma_data_source** ppBackend)
{
    ma_result result;
    ma_libvorbis* pVorbis;

    (void)pUserData;

    pVorbis = (ma_libvorbis*)ma_malloc(sizeof(*pVorbis), pAllocationCallbacks);
    if (pVorbis == NULL) {
        return MA_OUT_OF_MEMORY;
    }

    result = ma_libvorbis_init_file(pFilePath, pConfig, pAllocationCallbacks, pVorbis);
    if (result != MA_SUCCESS) {
        ma_free(pVorbis, pAllocationCallbacks);
        return result;
    }

    *ppBackend = pVorbis;

    return MA_SUCCESS;
}

static void ma_decoding_backend_uninit__libvorbis(void* pUserData, ma_data_source* pBackend, const ma_allocation_callbacks* pAllocationCallbacks)
{
    ma_libvorbis* pVorbis = (ma_libvorbis*)pBackend;

    (void)pUserData;

    ma_libvorbis_uninit(pVorbis, pAllocationCallbacks);
    ma_free(pVorbis, pAllocationCallbacks);
}

static ma_result ma_decoding_backend_get_channel_map__libvorbis(void* pUserData, ma_data_source* pBackend, ma_channel* pChannelMap, size_t channelMapCap)
{
    ma_libvorbis* pVorbis = (ma_libvorbis*)pBackend;

    (void)pUserData;

    return ma_libvorbis_get_data_format(pVorbis, NULL, NULL, NULL, pChannelMap, channelMapCap);
}

static ma_decoding_backend_vtable g_ma_decoding_backend_vtable_libvorbis =
{
    ma_decoding_backend_init__libvorbis,
    ma_decoding_backend_init_file__libvorbis,
    NULL, /* onInitFileW() */
    NULL, /* onInitMemory() */
    ma_decoding_backend_uninit__libvorbis
};


void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    MiniAudioConfig* config = (MiniAudioConfig*)pDevice->pUserData;
    ma_data_source* pDataSource = (ma_data_source*)&config->m_ma_decoder;//(ma_data_source*)pDevice->pUserData;
    if (pDataSource == NULL) {
        return;
    }

    ma_data_source_read_pcm_frames(pDataSource, pOutput, frameCount, NULL);

    (void)pInput;
}

SystemAudio::SystemAudio()
:   SystemAudioI()
,   m_playerState(PLAYER_STOPPED)
,   m_loops(false)
,   m_mediaPlayerConfig(nullptr)
{
}

SystemAudio::~SystemAudio()
{
    Stop();
    Free();
}

void SystemAudio::SetState(PlayerState state)
{
    m_playerState = state;
}

void SystemAudio::SetDecoder(DecoderI *decoder)
{
    ma_result result;

    MiniAudioConfig* config = (MiniAudioConfig*)malloc(sizeof(MiniAudioConfig));
    if (config == nullptr)
    {   return;
    }

    config->cursor = 0;
    m_mediaPlayerConfig = config;

    /*
    Add your custom backend vtables here. The order in the array defines the order of priority. The
    vtables will be passed in via the decoder config.
    */
    ma_decoding_backend_vtable* pCustomBackendVTables[] =
    {
        &g_ma_decoding_backend_vtable_libvorbis
    };

    /* Initialize the decoder. */
    config->m_decoderConfig = ma_decoder_config_init_default();
    config->m_decoderConfig.pCustomBackendUserData = NULL;  /* In this example our backend objects are contained within a ma_decoder_ex object to avoid a malloc. Our vtables need to know about this. */
    config->m_decoderConfig.ppCustomBackendVTables = pCustomBackendVTables;
    config->m_decoderConfig.customBackendCount = sizeof(pCustomBackendVTables) / sizeof(pCustomBackendVTables[0]);

    result = ma_decoder_init_file(decoder->GetPath().c_str(), &config->m_decoderConfig, &config->m_ma_decoder);
    if (result != MA_SUCCESS) {
        printf("Failed to initialize decoder.");
        return;
    }

    ma_data_source_set_looping(&config->m_ma_decoder, 1);

    /* Initialize the device. */
    result = ma_data_source_get_data_format(&config->m_ma_decoder, &config->m_format, &config->m_channels, &config->m_sampleRate, NULL, 0);
    if (result != MA_SUCCESS) {
        printf("Failed to retrieve decoder data format.");
        ma_decoder_uninit(&config->m_ma_decoder);
        return;
    }

    config->m_deviceConfig = ma_device_config_init(ma_device_type_playback);

    config->m_deviceConfig.playback.format = config->m_format;
    config->m_deviceConfig.playback.channels = config->m_channels;
    config->m_deviceConfig.sampleRate = config->m_sampleRate;
    config->m_deviceConfig.dataCallback = data_callback;
    config->m_deviceConfig.pUserData = m_mediaPlayerConfig;
}

void SystemAudio::PrepareToPlay()
{
    if (m_mediaPlayerConfig == nullptr)
    {   return;
    }

    MiniAudioConfig* config = (MiniAudioConfig*)m_mediaPlayerConfig;

    if (ma_device_init(NULL, &config->m_deviceConfig, &config->m_device) != MA_SUCCESS) {
        printf("Failed to open playback device.\n");
        ma_decoder_uninit(&config->m_ma_decoder);
        return;
    }

    SetState(PLAYER_PREPARED);
}

void SystemAudio::ReadBufferInto(void *ptr)
{
    // Not used in mini audio
}

void SystemAudio::Play()
{
    if (m_mediaPlayerConfig == nullptr)
    {   return;
    }

    MiniAudioConfig* config = (MiniAudioConfig*)m_mediaPlayerConfig;

    if (ma_device_start(&config->m_device) != MA_SUCCESS)
    {   printf("Failed to start playback device.\n");
        Free();
        return;
    }

    SetState(PLAYER_PLAYING);
}

void SystemAudio::Pause()
{
    if (m_mediaPlayerConfig == nullptr)
    {   return;
    }

    MiniAudioConfig* config = (MiniAudioConfig*)m_mediaPlayerConfig;
    if (ma_device_stop(&config->m_device) != MA_SUCCESS)
    {   printf("Failed to start playback device.\n");
        Free();
        return;
    }

    SetState(PLAYER_PAUSED);
}

void SystemAudio::Stop()
{
    if (m_mediaPlayerConfig == nullptr)
    {   return;
    }

    MiniAudioConfig* config = (MiniAudioConfig*)m_mediaPlayerConfig;
    if (ma_device_stop(&config->m_device) != MA_SUCCESS)
    {   printf("Failed to start playback device.\n");
        Free();
        return;
    }

    if (ma_data_source_seek_to_pcm_frame((ma_data_source*)&config->m_ma_decoder, 0) != MA_SUCCESS)
    {   printf("Could not seek to start!.\n");
        return;
    }
    
    SetState(PLAYER_STOPPED);
}

void SystemAudio::Free()
{
    if (m_mediaPlayerConfig == nullptr)
    {   return;
    }

    MiniAudioConfig* config = (MiniAudioConfig*)m_mediaPlayerConfig;

    ma_device_uninit(&config->m_device);
    ma_decoder_uninit(&config->m_ma_decoder);

    free(m_mediaPlayerConfig);
    m_mediaPlayerConfig = nullptr;

    SetState(PLAYER_DISCARDED);
}

bool SystemAudio::QueryIsRunning()
{
    if (m_mediaPlayerConfig == nullptr)
    {   return false;
    }

    MiniAudioConfig* config = (MiniAudioConfig*)m_mediaPlayerConfig;
    return ma_device_is_started(&config->m_device);
}

double SystemAudio::GetDuration()
{
    MiniAudioConfig* config = (MiniAudioConfig*)m_mediaPlayerConfig;
    if (config == nullptr)
    {   return false;
    }

    float duration = 0.0;

    if (ma_data_source_get_length_in_seconds((ma_data_source*)&config->m_ma_decoder, &duration) != MA_SUCCESS)
    {   printf("Could not get the length!\n");
        return -1;
    }
    
    return duration;
}

PlayerState SystemAudio::GetState()
{
    return m_playerState;
}

void SystemAudio::SetVolume(double value)
{
    if (m_mediaPlayerConfig == nullptr)
    {   return;
    }

    MiniAudioConfig* config = (MiniAudioConfig*)m_mediaPlayerConfig;

    if (ma_device_set_master_volume(&config->m_device, value) != MA_SUCCESS)
    {   printf("Could not set the volume!\n");
    }
}

void SystemAudio::SetLoops(bool loops)
{
    m_loops = loops;
}

void SystemAudio::SignalDidFinish()
{
}
