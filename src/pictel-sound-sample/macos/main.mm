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

    // Open the sound file
    PictelSoundRef shortFile = PictelSoundOpenSound(path);
    PictelSoundOpen(shortFile);
    PictelSoundPlay(shortFile);

    // Run the main queue for a couple of seconds
    RunQueueForTimeInterval(5);

    // Release memory, play nice.
    PictelSoundRelease(shortFile);

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
