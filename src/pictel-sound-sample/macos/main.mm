#include <iostream>
#include "pictel_sound.h"
#include <ogg/config_types.h>
#include <ogg/ogg.h>
#include <vorbis/codec.h>
#import <Foundation/Foundation.h>
#include <stdio.h>

int main()
{
    std::cout << "Hello, World!" << std::endl;

    const char * path = "/Users/krzysp/Documents/Projekty/Programistyczne/Pictel-sound/resources/Rondo_Alla_Turka.ogg";
    PictelSoundRef ref = PictelSoundOpenSound(path);
    printf("ref = %ld\n", (long)ref);
    printf("path = %s\n", PictelSoundGetPath(ref));
    PictelSoundOpen(ref);
    PictelSoundPrepare(ref);
    PictelSoundPlay(ref);

    NSDate *dateStarted = [NSDate date];
    while ( fabs([dateStarted timeIntervalSinceNow]) < 50 )
    {
        printf("Runloop running...\n");
        [[NSRunLoop currentRunLoop] runUntilDate:[NSDate dateWithTimeIntervalSinceNow:1]];
    }

    return 0;
}
