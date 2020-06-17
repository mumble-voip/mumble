// Copyright 2005-2020 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_VOICEPROTOCOLTYPE_H
#define MUMBLE_VOICEPROTOCOLTYPE_H

#include <string>

enum class VoiceProtocolType { UNDEFINED = 0, UNSUPPORTED = 1, UDP_AES_128_OCB2 = 2, UDP_AES_256_GCM = 3 };

struct VoiceProtocol {
	VoiceProtocolType protocolType;
	std::string unsupportedProtocolName;
	explicit VoiceProtocol(VoiceProtocolType t) : protocolType(t), unsupportedProtocolName() {}
	explicit VoiceProtocol(const std::string &s) {
		if (s == "UDP_AES_128_OCB2")
			protocolType = VoiceProtocolType::UDP_AES_128_OCB2;
		else if (s == "UDP_AES_256_GCM")
			protocolType = VoiceProtocolType::UDP_AES_256_GCM;
		else {
			if (!s.empty()) {
				protocolType            = VoiceProtocolType::UNSUPPORTED;
				unsupportedProtocolName = s;
			} else {
				protocolType = VoiceProtocolType::UNDEFINED;
			}
		}
	}

	std::string toString() const {
		switch (protocolType) {
			case VoiceProtocolType::UDP_AES_128_OCB2:
				return "UDP_AES_128_OCB2";
			case VoiceProtocolType::UDP_AES_256_GCM:
				return "UDP_AES_256_GCM";
			case VoiceProtocolType::UNSUPPORTED:
				return unsupportedProtocolName;
			case VoiceProtocolType::UNDEFINED:
			default:
				return "";
		}
		return "";
	}
};

#endif // MUMBLE_VOICEPROTOCOLTYPE_H
