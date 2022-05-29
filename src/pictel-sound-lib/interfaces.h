//
//  pictel_sound_decoder_interface.h
//  Pictel-sound
//
//  Created by krzysp on 28/05/2022.
//

#ifndef pictel_sound_decoder_interface_h
#define pictel_sound_decoder_interface_h

#include <iostream>

namespace PictelSound
{
    typedef enum
    {
        PLAYER_STOPPED      = 10,
        PLAYER_PREPARED     = 11,
        PLAYER_PLAYING      = 12,
        PLAYER_PAUSED       = 13,
        PLAYER_STOPPING     = 14,
        PLAYER_DISCARDED    = 100
    } PlayerState;

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
    };

    class PlayerI
    {
    public:
        PlayerI(DecoderI*, SystemAudioI*) { };

        virtual bool Open() = 0;
        virtual void Play() = 0;
        virtual void Pause() = 0;
        virtual void Stop() = 0;
        virtual void Close() = 0;

        virtual PlayerState GetState() = 0;
    };
};

#endif /* pictel_sound_decoder_interface_h */
