//
// Copyright (c) 2022 Krzysztof Pawłowski
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "include/pictel_sound.h"
#include "player.hpp"
#include "decoder_vorbis.hpp"
#include "system_audio_macos.h"

using namespace PictelSound;

static Player *PictelSoundFileConv(PictelSoundRef);
#define REF2OBJ(x) PictelSoundFileConv(x)

PictelSoundRef PictelSoundCreate(const char *filename)
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
