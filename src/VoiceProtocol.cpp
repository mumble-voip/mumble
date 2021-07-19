// Copyright 2005-2020 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "VoiceProtocol.h"

#include <string>

VoiceProtocol::VoiceProtocol(const std::string &s)
	: m_valid(false), m_protocolName(s), m_transport(VoiceTransportType::INVALID) {
}

VoiceProtocol::VoiceProtocol() : m_valid(false), m_transport(VoiceTransportType::INVALID) {
}

std::string VoiceProtocol::toString() const {
	return m_protocolName;
}

std::shared_ptr< VoiceProtocol > VoiceProtocol::fromString(const std::string &s) {
	if (s.rfind("MUMBLE_UDP_", 0) != std::string::npos)
		return std::make_shared< UDPVoiceProtocol >(UDPVoiceProtocol(s));
	else {
		return std::make_shared< VoiceProtocol >(VoiceProtocol(s));
	}
}

bool VoiceProtocol::operator==(const VoiceProtocol &rhs) const {
	return (rhs.m_protocolName == m_protocolName);
}

bool VoiceProtocol::isValid() const {
	return m_valid;
}

UDPVoiceProtocol::UDPVoiceProtocol(const std::string &s) : VoiceProtocol(s) {
	if (s.rfind("MUMBLE_UDP_", 0) != 0)
		throw VoiceProtocolException();

	m_transport = VoiceTransportType::UDP;

	std::string cipher_name = s.substr(strlen("MUMBLE_UDP_"));
	CipherType cipher       = CryptStateFactory::getFactory().getCipherType(cipher_name);

	if (cipher != CipherType::INVALID) {
		m_valid     = true;
		m_udpCipher = cipher;
	}
}

UDPVoiceProtocol::UDPVoiceProtocol(CipherType cipher) : VoiceProtocol("") {
	m_valid        = true;
	m_transport    = VoiceTransportType::UDP;
	m_udpCipher    = cipher;
	m_protocolName = "MUMBLE_UDP_" + std::string(CipherTypeDescription(cipher).name);
}
