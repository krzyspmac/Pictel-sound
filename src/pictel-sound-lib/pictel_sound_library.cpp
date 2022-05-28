//
// Created by krzysp on 28/05/2022.
//

#include "include/pictel_sound.h"
#include "pictel_sound_file.hpp"

using namespace PictelSound;

static PictelSoundFile *PictelSoundFileConv(PictelSoundRef);

PictelSoundRef PictelSoundOpenSound(const char *filename)
{
    PictelSoundFile *reference = new PictelSoundFile(filename);
    return reference;
}

void PictelSoundRelease(PictelSoundRef ref)
{
    if ( auto *reference = PictelSoundFileConv(ref) )
    {   delete reference;
    }
}

const char *PictelSoundGetPath(PictelSoundRef ref)
{
    if ( auto *reference = PictelSoundFileConv(ref) )
    {   return reference->GetPath().c_str();
    }
    else
    {   return nullptr;
    }
}

PictelSoundFile *PictelSoundFileConv(PictelSoundRef ref)
{
    return static_cast<PictelSoundFile*>(ref);
}
