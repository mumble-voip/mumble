// Copyright 2005-2020 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_VOICEPROTOCOLTYPE_H
#define MUMBLE_VOICEPROTOCOLTYPE_H

#include "crypto/CipherType.h"
#include "crypto/CryptStateFactory.h"

#include <exception>
#include <memory>
#include <string>

enum class VoiceTransportType { INVALID = 0, UDP = 1 };

class VoiceProtocolException : public std::exception {};

class VoiceProtocol {
protected:
	bool m_valid;
	std::string m_protocolName;
	explicit VoiceProtocol(const std::string &s);

public:
	VoiceTransportType m_transport;
	virtual bool operator==(const VoiceProtocol &rhs) const;
	virtual std::string toString() const;
	static std::shared_ptr< VoiceProtocol > fromString(const std::string &s);
	bool isValid() const;
	VoiceProtocol();
	virtual ~VoiceProtocol(){};
};


class UDPVoiceProtocol : public VoiceProtocol {
private:
	explicit UDPVoiceProtocol(const std::string &s);
	friend VoiceProtocol;

public:
	explicit UDPVoiceProtocol(CipherType cipher);
	CipherType m_udpCipher;
};

#endif // MUMBLE_VOICEPROTOCOLTYPE_H
