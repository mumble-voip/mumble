// Copyright 2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_CRYPTSTATECHACHA20POLY1305_H
#define MUMBLE_CRYPTSTATECHACHA20POLY1305_H

#include "CryptStateEVP.h"


// This class calls the EVP interface of ChaCha20-Poly1305 of OpenSSL.
// See https://www.openssl.org/docs/man1.1.1/man3/EVP_chacha20_poly1305.html.

class CryptStateChaCha20Poly1305 : public CryptStateEVP< CryptStateChaCha20Poly1305 > {
public:
	// For IV generation, see https://datatracker.ietf.org/doc/html/rfc7539#section-4
	static constexpr unsigned int IV_LENGTH  = 96 / 8;
	static constexpr unsigned int KEY_LENGTH = 256 / 8;

	static constexpr unsigned int TAG_LENGTH = 128 / 8;

	CryptStateChaCha20Poly1305() : CryptStateEVP< CryptStateChaCha20Poly1305 >(EVP_chacha20_poly1305()) {
		headLength = TAG_LENGTH + 1;
	}
	~CryptStateChaCha20Poly1305() override = default;
};

#endif // MUMBLE_CRYPTSTATECHACHA20POLY1305_H
