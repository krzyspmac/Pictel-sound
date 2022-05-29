//
//  system_audio_macos.hpp
//  Pictel-sound-macos
//
//  Created by krzysp on 28/05/2022.
//

#ifndef system_audio_macos_hpp
#define system_audio_macos_hpp

#include <stdio.h>
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
