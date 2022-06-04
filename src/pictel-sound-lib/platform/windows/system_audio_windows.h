//
// Created by Krzysiek on 6/2/2022.
//

#ifndef PICTEL_SOUND_SYSTEM_AUDIO_WINDOWS_H
#define PICTEL_SOUND_SYSTEM_AUDIO_WINDOWS_H

#include <stdio.h>
#include <iostream>
#include <vector>
#include "../../interfaces.h"

#include <windows.h>
#include <wrl\implements.h>
#include <mmreg.h>
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <mferror.h>

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x) { SafeRelease(&x); }
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(x) { delete x; x = nullptr; }
#endif

#ifndef SAFE_ARRAYDELETE
#define SAFE_ARRAYDELETE(x) { delete[] x; x = nullptr; }
#endif

enum RenderSampleType
{
    SampleTypeUnknown,
    SampleTypeFloat,
    SampleType16BitPCM,
    SampleType24in32BitPCM,
};

struct RenderBuffer
{
    UINT32          BufferSize;
    UINT32          BytesFilled;
    BYTE*           Buffer;
    RenderBuffer*   Next;

    RenderBuffer() :
        BufferSize(0),
        BytesFilled(0),
        Buffer(nullptr),
        Next(nullptr)
    {
    }

    ~RenderBuffer()
    {
        SAFE_ARRAYDELETE(Buffer);
    }
};

namespace PictelSound
{
    class SystemAudio: public SystemAudioI
    {
        PlayerState m_playerState;
        bool m_loops;
        RenderBuffer *m_SampleQueue;

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
