//
//  pictel_sound_file.cpp
//  Pictel-sound
//
//  Created by krzysp on 28/05/2022.
//

#include "pictel_sound_file.hpp"

using namespace PictelSound;

PictelSoundFile::PictelSoundFile(std::string path)
:   m_path(path)
{
}

PictelSoundFile::~PictelSoundFile()
{
    std::cout << "asd" << std::endl;
}
