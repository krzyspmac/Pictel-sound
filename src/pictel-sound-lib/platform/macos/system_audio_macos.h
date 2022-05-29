//
//  system_audio_macos.hpp
//  Pictel-sound-macos
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

#ifndef system_audio_macos_hpp
#define system_audio_macos_hpp

#include <stdio.h>
#include <iostream>
#include <vector>
#include "interfaces.h"

/** Apple uses 3 buffers in the AQPlayer example. We'll do the same.
 * See: http://developer.apple.com/library/ios/#samplecode/SpeakHere/Listings/Classes_AQPlayer_mm.html
 */
#define PICTEL_BUFFER_COUNT 3
#define PICTEL_BUFFER_SIZE 128 * 1024
#define PICTEL_DEFAULT_BUFFER_SIZE 2048

namespace PictelSound
{
    class SystemAudio: public SystemAudioI
    {
        char m_audioDescription[PICTEL_DEFAULT_BUFFER_SIZE];
        void *m_queue;
        void *m_buffers[PICTEL_BUFFER_COUNT];
        DecoderI *m_decoder;
        PlayerState m_playerState;
        bool m_loops;
    public: /** SystemAudioI */
        SystemAudio();
        ~SystemAudio();
        void SetDecoder(DecoderI *decoder);
        void PrepareToPlay();
        void Play();
        void Pause();
        void Stop();
        void Free();
        PlayerState GetState();
        bool QueryIsRunning();
        double GetDuration();
        void SetVolume(double);
        void SetLoops(bool);
    public: /** PlayerObserverI */
    public:
        /** Read bytes into the system-prepared buffer. The pointer is system-specific. */
        void ReadBufferInto(void*);

        void SignalDidFinish();
    private:
        /** Sets the state. Will invoke the listener callback. */
        void SetState(PlayerState);
    };
};

#endif /* system_audio_macos_hpp */
