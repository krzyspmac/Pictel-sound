//
//  pictel_sound_file.cpp
//  Pictel-sound
//
//  Created by krzysp on 28/05/2022.
//

#include "pictel_sound_file.hpp"

using namespace PictelSound;

PictelSoundFile::PictelSoundFile(DecoderI *decoder)
:   m_decoder(decoder)
{
}

PictelSoundFile::~PictelSoundFile()
{
    Close();
}

bool PictelSoundFile::Open()
{
    return m_decoder->Open();
}

void PictelSoundFile::Close()
{
    if (m_decoder != nullptr)
    {   m_decoder->Close();
        delete m_decoder;
        m_decoder = NULL;
    }
}
