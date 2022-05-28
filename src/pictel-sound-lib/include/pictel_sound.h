//
// Created by krzysp on 28/05/2022.
//

#ifndef PICTEL_SOUND_H
#define PICTEL_SOUND_H

#include <stdio.h>

/** Defines a handler for the sound. All subsequent calls require it.*/
typedef void* PictelSoundRef;

/** Open the file handle.
    Return NULL upon error. */
PictelSoundRef PictelSoundOpenSound(const char *);

/** Release the file handle.
    This stops the audio and releases the memory. */
void PictelSoundRelease(PictelSoundRef);

/** Get file attributes */
const char *PictelSoundGetPath(PictelSoundRef);

#endif //PICTEL_SOUND_H