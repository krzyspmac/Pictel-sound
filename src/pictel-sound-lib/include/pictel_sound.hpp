//
//  pictel_sound.hpp
//  Pictel-sound
//
//  Created by krzysp on 29/05/2022.
//

#ifndef pictel_sound_h
#define pictel_sound_h

#include <iostream>

namespace PictelSound
{
    class PlayerI
    {
    public:
        static PlayerI* CreateFromFile(std::string);
        virtual ~PlayerI() { };

        virtual bool Open() = 0;
        virtual void Play() = 0;
        virtual void Pause() = 0;
        virtual void Stop() = 0;
        virtual void SetVolume(double) = 0;
    };
};

#endif /* pictel_sound_h */
