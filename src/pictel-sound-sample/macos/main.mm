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

#import <Foundation/Foundation.h>
#include <iostream>
#include <stdio.h>
#include "pictel_sound.h"
#include "pictel_sound.hpp"

static void RunQueueForTimeInterval(NSTimeInterval);
static void SoundCallback(PlayerState state);

int main(int argc, char *argv[])
{
    const char * path = argv[1];
    NSLog(@"Opening file %s", path);

//    // C library usage
//    PictelSoundRef shortFile = PictelSoundCreate(path);
//    PictelSoundAddObserver(shortFile, &SoundCallback);
//    PictelSoundOpen(shortFile);
//
//    for (int i = 0; i < 3; i++)
//    {
//        PictelSoundPlay(shortFile);
//        RunQueueForTimeInterval(1.0);
//        PictelSoundStop(shortFile);
//    }
//
//    PictelSoundPlay(shortFile);
//    PictelSoundSetLoops(shortFile, true);
//    RunQueueForTimeInterval(2.5);
//    RunQueueForTimeInterval(15);
//    PictelSoundRelease(shortFile);

    // C++ libray usage
    auto *player = PictelSound::PlayerI::CreateFromFile(path);
    player->SetLoops(false);
    PictelSound::PlayerCallbackI *callbackRef = nullptr;
    callbackRef = player->AddCallbackLambda([&](auto state){
        printf("Callback with state %d.\n", state);
        if (state == PLAYER_PAUSED) {
            player->RemoveCallback(callbackRef);
        }
    });
    player->Open();
    player->Play();
    RunQueueForTimeInterval(3);
    player->Pause();
//    player->RemoveCallback(callbackRef);
    RunQueueForTimeInterval(15);
    delete player;

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

void SoundCallback(PlayerState state)
{
    printf("Callback from function with state %d.\n", state);
}
