//
//  pictel_sound_file.hpp
//  Pictel-sound
//
//  Created by krzysp on 28/05/2022.
//

#ifndef pictel_sound_file_hpp
#define pictel_sound_file_hpp

#include <iostream>
#include "decoder_interface.h"

namespace PictelSound
{
    class PictelSoundFile
    {
        DecoderI *m_decoder;
    public:
        /** The default initializer. Takes ownership of the DecoderI provided */
        PictelSoundFile(DecoderI*);
        ~PictelSoundFile();

    public:
        bool Open();
        void Close();

    public:
//        std::string& GetPath() { return m_path; };
    };
};

#endif /* pictel_sound_file_hpp */
