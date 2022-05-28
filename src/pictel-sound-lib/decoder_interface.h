//
//  pictel_sound_decoder_interface.h
//  Pictel-sound
//
//  Created by krzysp on 28/05/2022.
//

#ifndef pictel_sound_decoder_interface_h
#define pictel_sound_decoder_interface_h

#include <iostream>

namespace PictelSound
{
    class DecoderI
    {
    public:
        DecoderI(std::string path) { };
        virtual ~DecoderI() { };
        
        virtual bool Open() = 0;
        virtual void Close() = 0;
    };
};

#endif /* pictel_sound_decoder_interface_h */
