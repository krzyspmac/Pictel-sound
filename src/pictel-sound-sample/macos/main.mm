#import <Foundation/Foundation.h>
#include <iostream>
#include <stdio.h>
#include "pictel_sound.h"
#include "pictel_sound.hpp"

static void RunQueueForTimeInterval(NSTimeInterval);

int main(int argc, char *argv[])
{
    const char * path = argv[1];
    NSLog(@"Opening file %s", path);

    // C library usage
    PictelSoundRef shortFile = PictelSoundOpenSound(path);
    PictelSoundOpen(shortFile);
    PictelSoundPlay(shortFile);
    PictelSoundSetLoops(shortFile, true);
    RunQueueForTimeInterval(15);
    PictelSoundRelease(shortFile);

    // C++ libray usage
//    auto *player = PictelSound::PlayerI::CreateFromFile(path);
//    player->SetLoops(true);
//    player->Open();
//    player->Play();
//    RunQueueForTimeInterval(15);
//    delete player;

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
