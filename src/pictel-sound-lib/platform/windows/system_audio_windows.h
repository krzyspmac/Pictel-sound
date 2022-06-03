//
// Created by Krzysiek on 6/2/2022.
//

#ifndef PICTEL_SOUND_SYSTEM_AUDIO_WINDOWS_H
#define PICTEL_SOUND_SYSTEM_AUDIO_WINDOWS_H

#include <stdio.h>
#include <iostream>
#include <vector>
#include "../../interfaces.h"

namespace PictelSound
{
    class SystemAudio: public SystemAudioI
    {
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

#endif //PICTEL_SOUND_SYSTEM_AUDIO_WINDOWS_H
