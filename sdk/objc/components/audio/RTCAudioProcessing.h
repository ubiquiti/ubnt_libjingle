#import <Foundation/Foundation.h>

#import "RTCMacros.h"

NS_ASSUME_NONNULL_BEGIN

RTC_OBJC_EXPORT @protocol RTC_OBJC_TYPE
(RTCAudioProcessing)<NSObject>

- (void)initialize:(int)sample_rate_hz channels:(int)num_channels;

- (void)process:(const float*)data;

- (void)updateAudioIndicator:(int)average peak:(int)peak;

@end

NS_ASSUME_NONNULL_END