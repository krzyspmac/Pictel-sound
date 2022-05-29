//
// Created by krzysp on 28/05/2022.
//

#ifndef PICTEL_SOUND_H
#define PICTEL_SOUND_H

#include <stdio.h>

/** Defines a handler for the sound. All subsequent calls require it.
    When you're done with it, please release via PictelSoundRelease(ref).
 */
typedef void* PictelSoundRef;

/** Open the file handle.
    Returns NULL upon error. */
PictelSoundRef PictelSoundOpenSound(const char *);

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

#endif //PICTEL_SOUND_H
