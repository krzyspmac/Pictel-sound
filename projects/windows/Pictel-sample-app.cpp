// Pictel-sample-app.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <pictel_sound.h>
#include <stdio.h>

int main()
{
    auto ref1 = PictelSoundCreate("C:\\Users\\Krzysiek\\Documents\\GitHub\\Pictel-sound\\resources\\Rondo_Alla_Turka.ogg");
    if (ref1 != nullptr)
    {
        PictelSoundOpen(ref1);
        printf("ref1.duration = %f\n", PictelSoundQueryDuration(ref1));

        PictelSoundPlay(ref1);
    }
    
    auto ref2 = PictelSoundCreate("C:\\Users\\Krzysiek\\Downloads\\file_example_OOG_2MG.ogg");
    if (ref2 != nullptr)
    {
        PictelSoundOpen(ref2);
        PictelSoundPlay(ref2);
    }

    printf("ref1 is playing = %d\n", PictelSoundIsPlaying(ref1));
    printf("ref2.duration = %f\n", PictelSoundQueryDuration(ref2));

    printf("Press any key to pause ref2\\n");
    getchar();

    PictelSoundPause(ref2);

    printf("Press any key to unpause ref2 and lower the volume on ref2\\n");
    getchar();

    printf("ref1.position = %f\n", PictelSoundQueryPosition(ref1));
    printf("ref2.position = %f\n", PictelSoundQueryPosition(ref2));

    PictelSoundPlay(ref2);
    PictelSoundSetVolume(ref2, 0.5);

    printf("Press any key to quit\\n");
    getchar();

    PictelSoundRelease(ref1);
}
