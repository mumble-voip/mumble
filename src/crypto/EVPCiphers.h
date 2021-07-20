// Copyright 2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_EVPCIPHERS_H
#define MUMBLE_EVPCIPHERS_H

#include <openssl/evp.h>

namespace EVPCipher {

struct AES_256_GCM {
	// Regarding GCM, see NIST Special Publication 800-38D
	// https://nvlpubs.nist.gov/nistpubs/Legacy/SP/nistspecialpublication800-38d.pdf

	static const EVP_CIPHER *cipher;

	// IV generation procedure is detailed in NIST 800-38D Sec. 8.2
	// The construction we adopt is Deterministic Construction in 8.2.1
	// 32-bit of IV is fixed field and the remaining 64-bit is
	// "invocation field", which is an integer counter.

	static constexpr unsigned int IV_LENGTH  = 96 / 8;
	static constexpr unsigned int KEY_LENGTH = 256 / 8;

	// See NIST SP 800-38D (https://nvlpubs.nist.gov/nistpubs/Legacy/SP/nistspecialpublication800-38d.pdf)
	// for required tag length
	static constexpr unsigned int TAG_LENGTH = 64 / 8;
};

struct CHACHA20_POLY1305 {
	static const EVP_CIPHER *cipher;

	// For IV generation, see https://datatracker.ietf.org/doc/html/rfc7539#section-4
	static constexpr unsigned int IV_LENGTH  = 96 / 8;
	static constexpr unsigned int KEY_LENGTH = 256 / 8;
	static constexpr unsigned int TAG_LENGTH = 128 / 8;
};

}; // namespace EVPCipher

#endif // MUMBLE_EVPCIPHERS_H
