//
//  pictel_sound_decoder_interface.h
//  Pictel-sound
//
//  Created by krzysp on 28/05/2022.
//

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
