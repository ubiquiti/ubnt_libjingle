#include "unifi_capture_post_processing.h"

#import "base/RTCLogging.h"
#import "components/audio/RTCAudioProcessing.h"

namespace webrtc {

UnifiCapturePostProcessing::UnifiCapturePostProcessing(id<RTC_OBJC_TYPE(RTCAudioProcessing)> audio_processing)
    : audio_processing_(audio_processing) {}

void UnifiCapturePostProcessing::Initialize(int sample_rate_hz, int num_channels) {
  BOOL ret = [audio_processing_ initialize:sample_rate_hz channels:num_channels];
  if (!ret)
    RTCLogError(@"Failed to initialize capture post processing");
}

void UnifiCapturePostProcessing::Process(AudioBuffer* audio) {
}

std::string UnifiCapturePostProcessing::ToString() const {
  return "UnifiCapturePostProcessing";
}

void UnifiCapturePostProcessing::UpdateAudioIndicator(int average, int peak) {
  [audio_processing_ updateAudioIndicator:average peak:peak];
}

void UnifiCapturePostProcessing::SetRuntimeSetting(AudioProcessing::RuntimeSetting setting) {
}

}  // namespace webrtc
