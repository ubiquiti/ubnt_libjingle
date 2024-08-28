#include "unifi_capture_post_processing.h"
#include "modules//audio_processing/audio_buffer.h"
#include "rtc_base/logging.h"
#include "rtc_base/strings/string_builder.h"

#import "components/audio/RTCAudioProcessing.h"

namespace webrtc {

UnifiCapturePostProcessing::UnifiCapturePostProcessing(id<RTC_OBJC_TYPE(RTCAudioProcessing)> audio_processing)
    : audio_processing_(audio_processing) {
  RTC_LOG(LS_INFO) << __FUNCTION__;
}

void UnifiCapturePostProcessing::Initialize(int sample_rate_hz, int num_channels) {
  RTC_LOG(LS_INFO) << __FUNCTION__ << " sample_rate_hz=" << sample_rate_hz << " num_channels=" << num_channels;
  sample_rate_hz_ = sample_rate_hz;
  num_channels_ = num_channels;
  [audio_processing_ initialize:sample_rate_hz channels:num_channels];
}

void UnifiCapturePostProcessing::Process(AudioBuffer* audio) {
}

std::string UnifiCapturePostProcessing::ToString() const {
  rtc::StringBuilder ost;
  ost << "UnifiCapturePostProcessing[sample_rate_hz:" << sample_rate_hz_
      << "num_channels_:" << num_channels_ << "]";
  return ost.Release();
}

void UnifiCapturePostProcessing::UpdateAudioIndicator(int average, int peak) {
  [audio_processing_ updateAudioIndicator:average peak:peak];
}

void UnifiCapturePostProcessing::SetRuntimeSetting(AudioProcessing::RuntimeSetting setting) {
}

}  // namespace webrtc
