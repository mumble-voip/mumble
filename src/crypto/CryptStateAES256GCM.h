// Copyright 2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_CRYPTSTATEAES256GCM_H
#define MUMBLE_CRYPTSTATEAES256GCM_H

#include "crypto/CryptStateEVP.h"


class CryptStateAES256GCM : public CryptStateEVP< CryptStateAES256GCM > {
public:
	// Regarding GCM, see NIST Special Publication 800-38D
	// https://nvlpubs.nist.gov/nistpubs/Legacy/SP/nistspecialpublication800-38d.pdf

	// IV generation procedure is detailed in NIST 800-38D Sec. 8.2
	// The construction we adopt is Deterministic Construction in 8.2.1
	// 32-bit of IV is fixed field and the remaining 64-bit is
	// "invocation field", which is an integer counter.

	static constexpr unsigned int IV_LENGTH  = 96 / 8;
	static constexpr unsigned int KEY_LENGTH = 256 / 8;

	// See NIST SP 800-38D (https://nvlpubs.nist.gov/nistpubs/Legacy/SP/nistspecialpublication800-38d.pdf)
	// for required tag length
	static constexpr unsigned int TAG_LENGTH = 64 / 8;

	CryptStateAES256GCM() : CryptStateEVP< CryptStateAES256GCM >(EVP_aes_256_gcm()) { headLength = TAG_LENGTH + 1; };
	~CryptStateAES256GCM() override = default;
};


#endif // MUMBLE_CRYPTSTATEAES256GCM_H
