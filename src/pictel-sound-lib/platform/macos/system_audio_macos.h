//
//  system_audio_macos.hpp
//  Pictel-sound-macos
//
//  Created by krzysp on 28/05/2022.
//

#ifndef system_audio_macos_hpp
#define system_audio_macos_hpp

#include <stdio.h>
#include "interfaces.h"

namespace PictelSound
{
    class SystemAudio: public SystemAudioI
    {
        char m_audioDescription[1024];
    public:
        SystemAudio();
        ~SystemAudio();
        void SetDecoder(DecoderI *decoder);
    };
};

#endif /* system_audio_macos_hpp */
