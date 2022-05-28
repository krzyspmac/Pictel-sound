//
//  pictel_sound_file.hpp
//  Pictel-sound
//
//  Created by krzysp on 28/05/2022.
//

#ifndef pictel_sound_file_hpp
#define pictel_sound_file_hpp

#include <iostream>

namespace PictelSound
{
    class PictelSoundFile
    {
        std::string m_path;
    public:
        PictelSoundFile(std::string);
        ~PictelSoundFile();

    public:
        std::string& GetPath() { return m_path; };
    };
};

#endif /* pictel_sound_file_hpp */
