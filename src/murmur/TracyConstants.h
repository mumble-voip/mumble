// Copyright 2021-2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MURMUR_TRACYCONSTANTS_H_
#define MUMBLE_MURMUR_TRACYCONSTANTS_H_

namespace TracyConstants {
static constexpr const char *udp_packet_processing_zone = "udp_packet_processing";
static constexpr const char *tcp_packet_processing_zone = "tcp_packet_processing";
static constexpr const char *ping_processing_zone       = "tcp_ping";
static constexpr const char *udp_ping_processing_zone   = "udp_ping";
static constexpr const char *decrypt_unknown_peer_zone  = "decrypt_unknown_peer";

static constexpr const char *udp_frame = "udp_frame";

static constexpr const char *audio_send_out_zone         = "audio_send_out";
static constexpr const char *audio_encode                = "audio_encode";
static constexpr const char *audio_update                = "audio_update";
static constexpr const char *audio_whisper_cache_restore = "audio_whisper_cache_restore";
static constexpr const char *audio_whisper_cache_create  = "audio_whisper_cache_create";
}; // namespace TracyConstants

#endif // MUMBLE_MURMUR_TRACYCONSTANTS_H_
