//
// Created by krzysp on 28/05/2022.
//

#include "include/pictel_sound.h"
#include "pictel_sound_file.hpp"
#include "decoder_vorbis.hpp"
#include "system_audio_macos.h"

using namespace PictelSound;

static PictelSoundFile *PictelSoundFileConv(PictelSoundRef);
#define REF2OBJ(x) PictelSoundFileConv(x)

PictelSoundRef PictelSoundOpenSound(const char *filename)
{
    DecoderI *decoder = new DecoderVorbis(filename);
    SystemAudioI *systemAudio = new SystemAudio();
    PictelSoundFile *reference = new PictelSoundFile(decoder, systemAudio);
    return reference;
}

void PictelSoundRelease(PictelSoundRef ref)
{
    delete REF2OBJ(ref);
}

const char *PictelSoundGetPath(PictelSoundRef ref)
{
    return "";
//    return REF2OBJ(ref)->GetPath().c_str();
}

bool PictelSoundOpen(PictelSoundRef ref)
{
    return REF2OBJ(ref)->Open();
}

void PictelSoundPrepare(PictelSoundRef ref)
{
    REF2OBJ(ref)->PrepareToPlay();
}

void PictelSoundPlay(PictelSoundRef ref)
{
    REF2OBJ(ref)->Play();
}

PictelSoundFile *PictelSoundFileConv(PictelSoundRef ref)
{
    return static_cast<PictelSoundFile*>(ref);
}
