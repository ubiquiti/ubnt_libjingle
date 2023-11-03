#include "objc_audio_processing.h"

#include "api/make_ref_counted.h"
#include "rtc_base/logging.h"

#include "sdk/objc/native/src/unifi_capture_post_processing.h"

namespace webrtc {

rtc::scoped_refptr<AudioProcessing> CreateAudioProcessing(
    id<RTC_OBJC_TYPE(RTCAudioProcessing)> audio_processing) {
  RTC_DLOG(LS_INFO) << __FUNCTION__;
  
  return webrtc::AudioProcessingBuilder().SetCapturePostProcessing(std::make_unique<UnifiCapturePostProcessing>(audio_processing)).Create();
}

}  // namespace webrtc