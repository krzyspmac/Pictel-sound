#include <iostream>
#include "pictel_sound.h"
#include <ogg/config_types.h>
#include <ogg/ogg.h>
#include <vorbis/codec.h>

int main()
{
    std::cout << "Hello, World!" << std::endl;

    PictelSoundRef ref = PictelSoundOpenSound("myFilename");
    printf("ref = %ld\n", (long)ref);
    printf("path = %s\n", PictelSoundGetPath(ref));
    PictelSoundRelease(ref);

//    std::cout << "My Pictel Function " << MyPictelFunction() << std::endl;
    return 0;
}
