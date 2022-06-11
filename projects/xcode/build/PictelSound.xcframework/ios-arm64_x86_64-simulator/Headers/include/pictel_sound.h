//
// pictel_sound.h
// Pictel-sound project
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

#ifndef PICTEL_SOUND_H
#define PICTEL_SOUND_H

#include <stdio.h>

typedef enum
{
    PLAYER_STOPPED      = 10,
    PLAYER_PREPARED     = 11,
    PLAYER_PLAYING      = 12,
    PLAYER_PAUSED       = 13,
    PLAYER_STOPPING     = 14,
    PLAYER_DISCARDED    = 100
} PlayerState;

/** Defines a handle for the sound. All subsequent calls require it.
    When you're done with it, please release via PictelSoundRelease(ref).
 */
typedef void* PictelSoundRef;

/** Defines a handle for the audio state observer */
typedef void* PictelSoundStateObseverRef;

/** Open the file handle.
    Returns NULL upon error. */
PictelSoundRef PictelSoundCreate(const char *);

/** Release the file handle.
    This stops the audio and releases the memory. */
void PictelSoundRelease(PictelSoundRef);

/** Get file attributes */
const char *PictelSoundGetPath(PictelSoundRef);

/** Open the file */
bool PictelSoundOpen(PictelSoundRef);

/** Play the sound file */
void PictelSoundPlay(PictelSoundRef);

/** Pause the sound file */
void PictelSoundPause(PictelSoundRef);

/** Stop the sound file */
void PictelSoundStop(PictelSoundRef);

/** Set volume. Values from 0 to 1. */
void PictelSoundSetVolume(PictelSoundRef, double);

/** Loop audio */
void PictelSoundSetLoops(PictelSoundRef, bool);

/** Get the player state */
PlayerState PictelSoundGetPlayerState(PictelSoundRef);

/** Query is playing */
bool PictelSoundIsPlaying(PictelSoundRef);

/** Query duration */
double PictelSoundQueryDuration(PictelSoundRef);

/** Query position */
double PictelSoundQueryPosition(PictelSoundRef);

/** Add a state observer */
PictelSoundStateObseverRef PictelSoundAddObserver(PictelSoundRef, void (*SoundCallback)(PlayerState));

/** Remove a state observer */
void PictelSoundRemoveObserver(PictelSoundRef, PictelSoundStateObseverRef);

#endif //PICTEL_SOUND_H
