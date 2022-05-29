#include <iostream>
#include "pictel_sound.h"
#include <ogg/config_types.h>
#include <ogg/ogg.h>
#include <vorbis/codec.h>
#import <Foundation/Foundation.h>
#include <stdio.h>
#include "pictel_sound.hpp"

static void RunQueueForTimeInterval(NSTimeInterval);

int main()
{
    std::cout << "Hello, World!" << std::endl;

    const char * path = "/Users/krzysp/Documents/Projekty/Programistyczne/Pictel-sound/resources/Rondo_Alla_Turka.ogg";
    const char * path1 = "/Users/krzysp/Desktop/ogg-sample.ogg";

    PictelSoundRef shortFile = PictelSoundOpenSound(path);
    printf("ref = %ld\n", (long)shortFile);
    printf("path = %s\n", PictelSoundGetPath(shortFile));
//    PictelSoundOpen(shortFile);
//    PictelSoundPlay(shortFile);


//    PictelSoundRef longMusic = PictelSoundOpenSound(path1);
//    PictelSoundOpen(longMusic);
//    PictelSoundPlay(longMusic);
//    RunQueueForTimeInterval(2);
//    PictelSoundSetVolume(longMusic, 0.5);
//    RunQueueForTimeInterval(5);
//    PictelSoundSetVolume(longMusic, 1);
//    RunQueueForTimeInterval(5);
//
//
//
//    PictelSoundRelease(longMusic);

    auto *longFile = PictelSound::PlayerI::CreateFromFile(path1);
    longFile->Open();
    longFile->Play();

    RunQueueForTimeInterval(5);

    PictelSoundRelease(shortFile);
    delete longFile;

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
