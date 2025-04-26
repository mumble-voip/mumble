// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MURMUR_TRACYCONSTANTS_H_
#define MUMBLE_MURMUR_TRACYCONSTANTS_H_

namespace TracyConstants {
static constexpr const char *UDP_PACKET_PROCESSING_ZONE = "udp_packet_processing";
static constexpr const char *TCP_PACKET_PROCESSING_ZONE = "tcp_packet_processing";
static constexpr const char *PING_PROCESSING_ZONE       = "tcp_ping";
static constexpr const char *UDP_PING_PROCESSING_ZONE   = "udp_ping";
static constexpr const char *DECRYPT_UNKNOWN_PEER_ZONE  = "decrypt_unknown_peer";

static constexpr const char *UDP_FRAME = "udp_frame";

static constexpr const char *AUDIO_SENDOUT_ZONE         = "audio_send_out";
static constexpr const char *AUDIO_ENCODE               = "audio_encode";
static constexpr const char *AUDIO_UPDATE               = "audio_update";
static constexpr const char *AUDIO_WHISPER_CACHE_STORE  = "audio_whisper_cache_restore";
static constexpr const char *AUDIO_WHISPER_CACHE_CREATE = "audio_whisper_cache_create";
} // namespace TracyConstants

#endif // MUMBLE_MURMUR_TRACYCONSTANTS_H_
