//
//  PictelSoundPlayer.m
//  Pictem-iOS-sample
//
//  Created by krzysp on 31/05/2022.
//

#import "PictelSoundPlayer.h"
#import "pictel_sound.hpp"

@implementation PictelSoundPlayer
{
    PictelSoundRef _soundRef;
}

- (id)initWithPath:(NSString*)path
{
    self = [super init];
    if (self)
    {
        _soundRef = PictelSoundCreate([path cStringUsingEncoding:NSUTF8StringEncoding]);
        if (_soundRef)
        {   PictelSoundOpen(_soundRef);
        }
        else
        {   PictelSoundRelease(_soundRef);
        }
    }
    return self;
}

- (void)dealloc
{
    if (_soundRef)
    {   PictelSoundRelease(_soundRef);
    }
}

- (void)play
{
    if (_soundRef)
    {   PictelSoundPlay(_soundRef);
    }
}

@end
