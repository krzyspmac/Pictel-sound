//
//  decoder_vorbis.hpp
//  Pictel-sound-macos
//
//  Created by krzysp on 28/05/2022.
//

#ifndef decoder_vorbis_hpp
#define decoder_vorbis_hpp

#include <stdio.h>
#include "decoder_interface.h"
#include <vorbis/vorbisfile.h>

namespace PictelSound
{
    class DecoderVorbis: public DecoderI
    {
        std::string m_path;
        FILE *m_file;
        OggVorbis_File m_OggVorbisFile;
    public:
        DecoderVorbis(std::string path);
        ~DecoderVorbis();
        bool Open();
        void Close();
    };
};

#endif /* decoder_vorbis_hpp */
