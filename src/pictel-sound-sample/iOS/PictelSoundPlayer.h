//
//  PictelSoundPlayer.h
//  Pictem-iOS-sample
//
//  Created by krzysp on 31/05/2022.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface PictelSoundPlayer : NSObject

- (id)initWithPath:(NSString*)path;
- (void)play;

@end

NS_ASSUME_NONNULL_END
