#include <iostream>
#include "pictel_sound.h"
#include <ogg/config_types.h>
#include <ogg/ogg.h>
#include <vorbis/codec.h>

int main()
{
    std::cout << "Hello, World!" << std::endl;

    const char * path = "/Users/krzysp/Documents/Projekty/Programistyczne/Pictel-sound/resources/Rondo_Alla_Turka.ogg";
    PictelSoundRef ref = PictelSoundOpenSound(path);
    printf("ref = %ld\n", (long)ref);
    printf("path = %s\n", PictelSoundGetPath(ref));
    PictelSoundOpen(ref);
    PictelSoundRelease(ref);

//    std::cout << "My Pictel Function " << MyPictelFunction() << std::endl;
    return 0;
}
