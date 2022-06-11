//
//  pictel_sound.hpp
//  Pictel-sound
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

#ifndef pictel_sound_h
#define pictel_sound_h

#include <iostream>
#include "pictel_sound.h"

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
        virtual PlayerState GetState() = 0;
        virtual bool QueryIsPlaging() = 0;
        virtual double QueryDuration() = 0;
        virtual double QueryPosition() = 0;
        virtual void SetLoops(bool) = 0;
    };
};

#endif /* pictel_sound_h */
