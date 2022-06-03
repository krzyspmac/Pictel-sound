//
//  decoder_vorbis.hpp
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

#ifndef decoder_vorbis_hpp
#define decoder_vorbis_hpp

#include <stdio.h>
#include "interfaces.h"
//#include "vorbis/vorbisfile.h"
#include <vorbis/vorbisfile.h>

namespace PictelSound
{
    class DecoderVorbis: public DecoderI
    {
        std::string m_path;
        FILE *m_file;
        OggVorbis_File m_OggVorbisFile;
        vorbis_info *m_OggVorbisInfo;
    public:
        DecoderVorbis(std::string path);
        ~DecoderVorbis();
        bool Open();
        void Close();
        double GetRate();
        double GetDuration();
        uint32_t GetChannels();
        void Seek(double position);
        bool ReadBuffer(void *buffer, unsigned int capacity, unsigned int *outTotalBytesRead);
    };
};

#endif /* decoder_vorbis_hpp */
