//
//  pictel_sound_file.hpp
//  Pictel-sound
//
//  Created by krzysp on 28/05/2022.
//

#ifndef pictel_sound_file_hpp
#define pictel_sound_file_hpp

#include <iostream>
#include "interfaces.h"

namespace PictelSound
{
    class PictelSoundFile: public PlayerI
    {
        DecoderI *m_decoder;
        SystemAudioI *m_systemAudio;
    public:
        /** The default initializer. Takes ownership of the DecoderI provided */
        PictelSoundFile(DecoderI*, SystemAudioI*);
        ~PictelSoundFile();

    public:
        bool Open();
        void Play();
        void Pause();
        void Stop();
        void Close();
        PlayerState GetState();

    public:
//        std::string& GetPath() { return m_path; };
    };
};

#endif /* pictel_sound_file_hpp */
