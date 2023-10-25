#ifndef SDK_OBJC_NATIVE_SRC_AUDIO_UNIFI_CAPTURE_POST_PROCESSING_H_
#define SDK_OBJC_NATIVE_SRC_AUDIO_UNIFI_CAPTURE_POST_PROCESSING_H_

#import "base/RTCMacros.h"

#include "modules/audio_processing/include/audio_processing.h"

@protocol RTC_OBJC_TYPE
(RTCAudioProcessing);

namespace webrtc {

class UnifiCapturePostProcessing : public CustomProcessing {

 public:
  explicit UnifiCapturePostProcessing(id<RTC_OBJC_TYPE(RTCAudioProcessing)>);
  virtual ~UnifiCapturePostProcessing() override {}

  void Initialize(int sample_rate_hz, int num_channels) override;
  void Process(AudioBuffer* audio) override;
  std::string ToString() const override;
  void SetRuntimeSetting(AudioProcessing::RuntimeSetting setting) override;
  void UpdateAudioIndicator(int average, int peak) override;

 private:
  id<RTC_OBJC_TYPE(RTCAudioProcessing)> audio_processing_;
  int sample_rate_hz_ = 0;
  int num_channels_ = 0;
};

}  // namespace webrtc

#endif