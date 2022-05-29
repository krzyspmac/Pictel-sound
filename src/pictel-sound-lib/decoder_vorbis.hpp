//
//  decoder_vorbis.hpp
//  Pictel-sound-macos
//
//  Created by krzysp on 28/05/2022.
//

#ifndef decoder_vorbis_hpp
#define decoder_vorbis_hpp

#include <stdio.h>
#include "interfaces.h"
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
        bool ReadBuffer(void *buffer, unsigned int capacity, unsigned int *outTotalBytesRead);
    };
};

#endif /* decoder_vorbis_hpp */
