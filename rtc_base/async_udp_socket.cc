/*
 *  Copyright 2004 The WebRTC Project Authors. All rights reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "rtc_base/async_udp_socket.h"


#include "rtc_base/checks.h"
#include "rtc_base/logging.h"
#include "rtc_base/network/sent_packet.h"
#include "rtc_base/time_utils.h"
#include "system_wrappers/include/field_trial.h"

namespace rtc {

// Returns true if the experiement "WebRTC-SCM-Timestamp" is explicitly
// disabled.
static bool IsScmTimeStampExperimentDisabled() {
  return webrtc::field_trial::IsDisabled("WebRTC-SCM-Timestamp");
}

AsyncUDPSocket* AsyncUDPSocket::Create(Socket* socket,
                                       const SocketAddress& bind_address
// UI Customization Begin
                                       ,int interfaceIndex) {
// UI Customization End
  std::unique_ptr<Socket> owned_socket(socket);
// UI Customization Begin
  if (socket->Bind(bind_address, interfaceIndex) < 0) {
// UI Customization End
    RTC_LOG(LS_ERROR) << "Bind() failed with error " << socket->GetError();
    return nullptr;
  }
  return new AsyncUDPSocket(owned_socket.release());
}

AsyncUDPSocket* AsyncUDPSocket::Create(SocketFactory* factory,
                                       const SocketAddress& bind_address
// UI Customization Begin
                                       ,int interfaceIndex) {
// UI Customization End
  Socket* socket = factory->CreateSocket(bind_address.family(), SOCK_DGRAM);
  if (!socket)
    return nullptr;
// UI Customization Begin
  return Create(socket, bind_address, interfaceIndex);
// UI Customization End
}

AsyncUDPSocket::AsyncUDPSocket(Socket* socket) : socket_(socket) {
  sequence_checker_.Detach();
  // The socket should start out readable but not writable.
  socket_->SignalReadEvent.connect(this, &AsyncUDPSocket::OnReadEvent);
  socket_->SignalWriteEvent.connect(this, &AsyncUDPSocket::OnWriteEvent);
}

SocketAddress AsyncUDPSocket::GetLocalAddress() const {
  return socket_->GetLocalAddress();
}

SocketAddress AsyncUDPSocket::GetRemoteAddress() const {
  return socket_->GetRemoteAddress();
}

int AsyncUDPSocket::Send(const void* pv,
                         size_t cb,
                         const rtc::PacketOptions& options) {
  rtc::SentPacket sent_packet(options.packet_id, rtc::TimeMillis(),
                              options.info_signaled_after_sent);
  CopySocketInformationToPacketInfo(cb, *this, false, &sent_packet.info);
  int ret = socket_->Send(pv, cb);
  SignalSentPacket(this, sent_packet);
  return ret;
}

int AsyncUDPSocket::SendTo(const void* pv,
                           size_t cb,
                           const SocketAddress& addr,
                           const rtc::PacketOptions& options) {
  rtc::SentPacket sent_packet(options.packet_id, rtc::TimeMillis(),
                              options.info_signaled_after_sent);
  CopySocketInformationToPacketInfo(cb, *this, true, &sent_packet.info);
  int ret = socket_->SendTo(pv, cb, addr);
  SignalSentPacket(this, sent_packet);
  return ret;
}

int AsyncUDPSocket::Close() {
  return socket_->Close();
}

AsyncUDPSocket::State AsyncUDPSocket::GetState() const {
  return STATE_BOUND;
}

int AsyncUDPSocket::GetOption(Socket::Option opt, int* value) {
  return socket_->GetOption(opt, value);
}

int AsyncUDPSocket::SetOption(Socket::Option opt, int value) {
  return socket_->SetOption(opt, value);
}

int AsyncUDPSocket::GetError() const {
  return socket_->GetError();
}

void AsyncUDPSocket::SetError(int error) {
  return socket_->SetError(error);
}

void AsyncUDPSocket::OnReadEvent(Socket* socket) {
  RTC_DCHECK(socket_.get() == socket);
  RTC_DCHECK_RUN_ON(&sequence_checker_);

  SocketAddress remote_addr;
  int64_t timestamp = -1;
  int len = socket_->RecvFrom(buf_, BUF_SIZE, &remote_addr, &timestamp);

  if (len < 0) {
    // An error here typically means we got an ICMP error in response to our
    // send datagram, indicating the remote address was unreachable.
    // When doing ICE, this kind of thing will often happen.
    // TODO: Do something better like forwarding the error to the user.
    SocketAddress local_addr = socket_->GetLocalAddress();
    RTC_LOG(LS_INFO) << "AsyncUDPSocket[" << local_addr.ToSensitiveString()
                     << "] receive failed with error " << socket_->GetError();
    return;
  }
  if (timestamp == -1) {
    // Timestamp from socket is not available.
    timestamp = TimeMicros();
  } else {
    if (!socket_time_offset_) {
      socket_time_offset_ =
          !IsScmTimeStampExperimentDisabled() ? TimeMicros() - timestamp : 0;
    }
    timestamp += *socket_time_offset_;
  }

  // TODO: Make sure that we got all of the packet.
  // If we did not, then we should resize our buffer to be large enough.
  SignalReadPacket(this, buf_, static_cast<size_t>(len), remote_addr,
                   timestamp);
}

void AsyncUDPSocket::OnWriteEvent(Socket* socket) {
  SignalReadyToSend(this);
}

}  // namespace rtc
