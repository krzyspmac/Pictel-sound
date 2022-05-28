//
// Created by krzysp on 28/05/2022.
//

#include "include/pictel_sound.h"
#include "pictel_sound_file.hpp"

using namespace PictelSound;

static PictelSoundFile *PictelSoundFileConv(PictelSoundRef);
#define REF2OBJ(x) PictelSoundFileConv(x)

PictelSoundRef PictelSoundOpenSound(const char *filename)
{
    PictelSoundFile *reference = new PictelSoundFile(filename);
    return reference;
}

void PictelSoundRelease(PictelSoundRef ref)
{
    delete REF2OBJ(ref);
}

const char *PictelSoundGetPath(PictelSoundRef ref)
{
    return REF2OBJ(ref)->GetPath().c_str();
}

bool PictelSoundOpen(PictelSoundRef ref)
{
    return REF2OBJ(ref)->Open();
}

PictelSoundFile *PictelSoundFileConv(PictelSoundRef ref)
{
    return static_cast<PictelSoundFile*>(ref);
}
