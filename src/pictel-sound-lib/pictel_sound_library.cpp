//
// Created by krzysp on 28/05/2022.
//

#include "include/pictel_sound.h"
#include "player.hpp"
#include "decoder_vorbis.hpp"
#include "system_audio_macos.h"

using namespace PictelSound;

static Player *PictelSoundFileConv(PictelSoundRef);
#define REF2OBJ(x) PictelSoundFileConv(x)

PictelSoundRef PictelSoundOpenSound(const char *filename)
{
    DecoderI *decoder = new DecoderVorbis(filename);
    SystemAudioI *systemAudio = new SystemAudio();
    Player *reference = new Player(decoder, systemAudio);
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

void PictelSoundPlay(PictelSoundRef ref)
{
    REF2OBJ(ref)->Play();
}

void PictelSoundPause(PictelSoundRef ref)
{
    REF2OBJ(ref)->Pause();
}

void PictelSoundStop(PictelSoundRef ref)
{
    REF2OBJ(ref)->Stop();
}

void PictelSoundSetVolume(PictelSoundRef ref, double value)
{
    REF2OBJ(ref)->SetVolume(value);
}

void PictelSoundSetLoops(PictelSoundRef ref, bool value)
{
    REF2OBJ(ref)->SetLoops(value);
}

PlayerState PictelSoundGetPlayerState(PictelSoundRef ref)
{
    return REF2OBJ(ref)->GetState();
}

PlayerI *PlayerI::CreateFromFile(std::string path)
{
    DecoderI *decoder = new DecoderVorbis(path);
    SystemAudioI *systemAudio = new SystemAudio();
    Player *reference = new Player(decoder, systemAudio);
    return reference;
}

Player *PictelSoundFileConv(PictelSoundRef ref)
{
    return static_cast<Player*>(ref);
}
