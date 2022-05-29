//
//  decoder_vorbis.cpp
//  Pictel-sound-macos
//
//  Created by krzysp on 28/05/2022.
//

#include "decoder_vorbis.hpp"

#define IDZ_OGG_VORBIS_WORDSIZE 2

using namespace PictelSound;

DecoderVorbis::DecoderVorbis(std::string path)
:   DecoderI(path)
,   m_path(path)
,   m_file(NULL)
,   m_OggVorbisInfo(NULL)
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

    m_OggVorbisInfo = ov_info(&m_OggVorbisFile, -1);

    return true;
}

double DecoderVorbis::GetRate()
{
    return m_OggVorbisInfo->rate;
}

uint32_t DecoderVorbis::GetChannels()
{
    return m_OggVorbisInfo->channels;
}

void DecoderVorbis::Seek(double position)
{
    ov_time_seek(&m_OggVorbisFile, position);
}

double DecoderVorbis::GetDuration()
{
    double duration = ov_time_total(&m_OggVorbisFile, -1);
    return duration;
}

void DecoderVorbis::Close()
{
    if (m_file != NULL)
    {   fclose(m_file);
        m_file = NULL;

        ov_clear(&m_OggVorbisFile);
    }
}

bool DecoderVorbis::ReadBuffer(void *buffer, unsigned int capacity, unsigned int *outTotalBytesRead)
{
    int bigEndian = 0;
    int wordSize = IDZ_OGG_VORBIS_WORDSIZE;
    int signedSamples = 1;
    int currentSection = -1;

    *outTotalBytesRead = 0;

    unsigned int nTotalBytesRead = 0;
    long nBytesRead = 0;
    do
    {
        nBytesRead = ov_read(&m_OggVorbisFile,
                             (char*)buffer + nTotalBytesRead,
                             (int)(capacity - nTotalBytesRead),
                             bigEndian, wordSize,
                             signedSamples, &currentSection);
        if(nBytesRead  <= 0)
            break;
        nTotalBytesRead += nBytesRead;
    } while(nTotalBytesRead < capacity);

    if (nTotalBytesRead == 0)
    {   return false;
    }

    if (nBytesRead < 0)
    {   return false;
    }

    *outTotalBytesRead = nTotalBytesRead;

    return true;
}
