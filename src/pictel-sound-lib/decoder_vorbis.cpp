//
//  decoder_vorbis.cpp
//  Pictel-sound-macos
//
//  Created by krzysp on 28/05/2022.
//

#include "decoder_vorbis.hpp"

#define PICTEL_BITS_PER_BYTE 8
#define PICTEL_BYTES_TO_BITS(bytes) ((bytes) * IDZ_BITS_PER_BYTE)
#define PICTEL_OGG_VORBIS_WORDSIZE 2

using namespace PictelSound;

DecoderVorbis::DecoderVorbis(std::string path)
:   DecoderI(path)
,   m_path(path)
,   m_file(NULL)
{
}

DecoderVorbis::~DecoderVorbis()
{
    Close();
}

bool DecoderVorbis::Open()
{
    m_file = fopen(m_path.c_str(), "r");
    if (m_file == NULL)
    {   return false;
    }

    int result = ov_open_callbacks(m_file, &m_OggVorbisFile, NULL, 0, OV_CALLBACKS_NOCLOSE);
    if (result < 0)
    {   printf("Callback creation did not succeed!\n");
        return false;
    }

    vorbis_info *pInfo = ov_info(&m_OggVorbisFile, -1);
    int bytesPerChannel = PICTEL_OGG_VORBIS_WORDSIZE;

    return true;
}

void DecoderVorbis::Close()
{
    if (m_file != NULL)
    {   fclose(m_file);
        m_file = NULL;

        ov_clear(&m_OggVorbisFile);
    }
}
