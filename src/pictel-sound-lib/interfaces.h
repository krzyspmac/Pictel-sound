//
//  pictel_sound_decoder_interface.h
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

#ifndef pictel_sound_decoder_interface_h
#define pictel_sound_decoder_interface_h

#include <iostream>
#include "pictel_sound.h"
#include "pictel_sound.hpp"

namespace PictelSound
{
    class DecoderI
    {
    public:
        DecoderI(std::string path) { };
        virtual ~DecoderI() { };

        virtual bool Open() = 0;
        virtual void Close() = 0;

        virtual double GetRate() = 0;
        virtual uint32_t GetChannels() = 0;
        virtual double GetDuration() = 0;
        virtual void Seek(double) = 0;

        virtual bool ReadBuffer(void *buffer, unsigned int capacity, unsigned int *outTotalBytesRead) = 0;
    };

    class SystemAudioI
    {
        DecoderI *m_decoder;
    public:
        SystemAudioI() { };
        virtual ~SystemAudioI() { };

        virtual void SetDecoder(DecoderI*) = 0;
        virtual void PrepareToPlay() = 0;
        virtual void Play() = 0;
        virtual void Pause() = 0;
        virtual void Stop() = 0;
        virtual void Free() = 0;
        virtual bool QueryIsRunning() = 0;
        virtual double GetDuration() = 0;
        virtual PlayerState GetState() = 0;
        virtual void SetVolume(double) = 0;
        virtual void SetLoops(bool) = 0;
    };

    class __PlayerI
    {
    public:
        __PlayerI(DecoderI*, SystemAudioI*) { };

        virtual bool Open() = 0;
        virtual void Play() = 0;
        virtual void Pause() = 0;
        virtual void Stop() = 0;
        virtual void Close() = 0;
        virtual void SetVolume(double) = 0;
        virtual void SetLoops(bool) = 0;

        virtual PlayerState GetState() = 0;
    };
};

#endif /* pictel_sound_decoder_interface_h */
