//
//  pictel_sound_file.cpp
//  Pictel-sound
//
//  Created by krzysp on 28/05/2022.
//

#include "pictel_sound_file.hpp"

using namespace PictelSound;

PictelSoundFile::PictelSoundFile(DecoderI *decoder, SystemAudioI *systemAudio)
:   m_decoder(decoder)
,   m_systemAudio(systemAudio)
{
}

PictelSoundFile::~PictelSoundFile()
{
    Close();
}

bool PictelSoundFile::Open()
{
    if (!m_decoder->Open())
    {   return false;
    }

    m_systemAudio->SetDecoder(m_decoder);

    return true;
}

void PictelSoundFile::PrepareToPlay()
{
    m_systemAudio->PrepareToPlay();
}

void PictelSoundFile::Play()
{
    m_systemAudio->Play();
}

void PictelSoundFile::Close()
{
    if (m_decoder != nullptr)
    {   m_decoder->Close();
        delete m_decoder;
        m_decoder = NULL;
    }
}
