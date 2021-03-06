Pictel-sound
============

What is it?
-----------

An easy to use, simple, C/C++ crossplatform __ogg/vorbis__ audio player.
__It can play ogg files only.__

Why does it exist?
------------------

The library is written to support my custom, 2d engine
[https://github.com/krzyspmac/Pictel](https://github.com/krzyspmac/Pictel) but can be
used in any other project.

I need a library that's:

- simple to use
- plays an open-source format (ogg/vorbis was chosen for that)
- has limited amount of functionality
- works well with different environments/systems

OGG/VORBIS
----------

The library includes a statically compiled OGG and VORBIS libraries for all
supported platforms. 

Build scripts are provided for Xcode (MacOS/iOS Device/iOS Simulator) and Windows (VS C++).
Linux & Android support coming at a later time.

Dependencies
------------

- [libogg/libvorbis](https://xiph.org/ogg)
- [miniaudio](https://github.com/mackron/miniaudio) (windows & linux)

Completion
----------

Not all platform specific code has been completed.

- [x] MacOS (native code)
- [x] iOS (native code)
- [x] Windows (miniaudio)
- [ ] Linux (miniaudio)
- [ ] Android (don't know yet)

Usage
=======

C
-
```c
#include "pictel_sound.h"

static void SoundCallback(PlayerState state);

int main(int argc, char *argv[])
{
    // create a handle to file
    PictelSoundRef file = PictelSoundCreate("path_to_file.ogg");
   
    // add a C-function state callback
    PictelSoundAddObserver(shortFile, &SoundCallback);
   
    // open the sound file
    PictelSoundOpen(file);				
   
    // play the file
    PictelSoundPlay(file);

    // wait a while
    PictelSoundRelease(file);
}

void SoundCallback(PlayerState state)
{
    printf("Callback from function with state %d.\n", state);
}
```

C++
-

```c_cpp
#include "pictel_sound.hpp"

int main(int argc, char *argv[])
{

    // create a handle to file
    auto *player = PictelSound::PlayerI::CreateFromFile(path);
    
    // set it to loop
    player->SetLoops(true);
    
    // add a state change callback
    auto callbackRef = player->AddCallbackLambda([&](auto state){
        printf("Callback with state %d.\n", state);
    });
    
    // open the sound file
    player->Open();
    
    // play the file
    player->Play();

    // wait a bit and release when you're ready   
    delete player;
}

```

License
=======

Copyright (c) 2022 Krzysztof Paw??owski

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
