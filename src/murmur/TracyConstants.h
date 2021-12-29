// Copyright 2021 The Mumble Developers. All rights reserved.
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
}; // namespace TracyConstants

#endif // MUMBLE_MURMUR_TRACYCONSTANTS_H_
