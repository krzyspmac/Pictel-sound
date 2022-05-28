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

    char test[5555];
    bzero(test, 5555);
    unsigned int read = 0;
    m_decoder->ReadBuffer(test, 5000, &read);

    return true;
}

void PictelSoundFile::Close()
{
    if (m_decoder != nullptr)
    {   m_decoder->Close();
        delete m_decoder;
        m_decoder = NULL;
    }
}
