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

PictelSoundRef file = PictelSoundCreate("path_to_file.ogg");
PictelSoundOpen(file);
PictelSoundPlay(file);

PictelSoundRelease(file);
```

C++
-

```c_cpp
#include "pictel_sound.hpp"

auto *player = PlayerI::CreateFromFile("path_to_file.ogg");
player->Open();
player->Play();
delete player;
```

License
=======

Copyright (c) 2022 Krzysztof Pawłowski

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
