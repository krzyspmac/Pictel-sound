//
//  pictel_sound_file.hpp
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
        bool QueryIsPlaging();
        double QueryDuration();
        double QueryPosition();
        PlayerState GetState();
    };
};

#endif /* pictel_sound_file_hpp */
