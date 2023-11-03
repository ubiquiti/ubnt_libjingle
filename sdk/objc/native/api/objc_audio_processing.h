#ifndef SDK_OBJC_NATIVE_API_OBJC_AUDIO_PROCESSING_H_
#define SDK_OBJC_NATIVE_API_OBJC_AUDIO_PROCESSING_H_

#import "components/audio/RTCAudioProcessing.h"
#include "modules/audio_processing/include/audio_processing.h"

namespace webrtc {

rtc::scoped_refptr<AudioProcessing> CreateAudioProcessing(
    id<RTC_OBJC_TYPE(RTCAudioProcessing)> audio_processing);

}  // namespace webrtc

#endif  // SDK_OBJC_NATIVE_API_OBJC_AUDIO_PROCESSING_H_
