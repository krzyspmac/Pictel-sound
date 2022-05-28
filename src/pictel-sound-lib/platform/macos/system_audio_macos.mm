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

SystemAudio::SystemAudio()
:   SystemAudioI()
{
    bzero(&m_audioDescription, 1024);
}

SystemAudio::~SystemAudio()
{
}

void SystemAudio::SetDecoder(DecoderI *decoder)
{
    AudioStreamBasicDescription *description = (AudioStreamBasicDescription*)m_audioDescription;
    int bytesPerChannel = PICTEL_OGG_VORBIS_WORDSIZE;

    FillOutASBDForLPCM(*description,
                       (Float64)decoder->GetRate(),
                       (UInt32)decoder->GetChannels(), // channels per frame
                       (UInt32)PICTEL_BYTES_TO_BITS(bytesPerChannel), // valid bits per channel
                       (UInt32)PICTEL_BYTES_TO_BITS(bytesPerChannel), // total bits per channel
                       false, // isFloat
                       false); // isBigEndian
}

