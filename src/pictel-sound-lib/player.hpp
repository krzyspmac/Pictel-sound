//
//  pictel_sound_file.hpp
//  Pictel-sound
//
//  Created by krzysp on 28/05/2022.
//

#ifndef pictel_sound_file_hpp
#define pictel_sound_file_hpp

#include <iostream>
#include "interfaces.h"

namespace PictelSound
{
    class Player: public PlayerI, public __PlayerI
    {
        DecoderI *m_decoder;
        SystemAudioI *m_systemAudio;
    public:
        /** The default initializer. Takes ownership of the DecoderI provided */
        Player(DecoderI*, SystemAudioI*);
        ~Player();

    public:
        bool Open();
        void Play();
        void Pause();
        void Stop();
        void Close();
        void SetVolume(double);
        void Seek(double);
        void SetLoops(bool);
        PlayerState GetState();
    };
};

#endif /* pictel_sound_file_hpp */
