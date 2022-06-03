//
//  decoder_vorbis.cpp
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

#include "decoder_vorbis.hpp"

#define IDZ_OGG_VORBIS_WORDSIZE 2

using namespace PictelSound;

DecoderVorbis::DecoderVorbis(std::string path)
:   DecoderI(path)
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
#ifdef _WIN32
    m_file = fopen(m_path.c_str(), "r");
#else
    m_file = fopen(m_path.c_str(), "r");
#endif
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
    int result = ov_time_seek(&m_OggVorbisFile, position);
    if (result < 0)
    {   printf("Could not seek to %f\n", position);
        return;
    }
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
