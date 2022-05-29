#include <iostream>
#include "pictel_sound.h"
#include <ogg/config_types.h>
#include <ogg/ogg.h>
#include <vorbis/codec.h>
#import <Foundation/Foundation.h>
#include <stdio.h>

static void RunQueueForTimeInterval(NSTimeInterval);

int main()
{
    std::cout << "Hello, World!" << std::endl;

    const char * path = "/Users/krzysp/Documents/Projekty/Programistyczne/Pictel-sound/resources/Rondo_Alla_Turka.ogg";
    const char * path1 = "/Users/krzysp/Desktop/ogg-sample.ogg";

//    PictelSoundRef ref = PictelSoundOpenSound(path);
//    printf("ref = %ld\n", (long)ref);
//    printf("path = %s\n", PictelSoundGetPath(ref));
//    PictelSoundOpen(ref);
//    PictelSoundPlay(ref);


    PictelSoundRef ref = PictelSoundOpenSound(path1);
    printf("ref = %ld\n", (long)ref);
    printf("path = %s\n", PictelSoundGetPath(ref));

    PictelSoundOpen(ref);
    PictelSoundPlay(ref);


    RunQueueForTimeInterval(2);

    PictelSoundPause(ref);
    RunQueueForTimeInterval(1.5);

    PictelSoundPlay(ref);
    RunQueueForTimeInterval(3.5);

    PictelSoundStop(ref);
//    RunQueueForTimeInterval(5);

    PictelSoundRelease(ref);
    return 0;
}

void RunQueueForTimeInterval(NSTimeInterval seconds)
{
    NSDate *dateStarted = [NSDate date];
    while ( fabs([dateStarted timeIntervalSinceNow]) < seconds )
    {
        printf("Runloop running...\n");
        [[NSRunLoop currentRunLoop] runUntilDate:[NSDate dateWithTimeIntervalSinceNow:1]];
    }
}
