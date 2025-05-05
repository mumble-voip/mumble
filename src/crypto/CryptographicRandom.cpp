// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "CryptographicRandom.h"

#include "arc4random_uniform.h"

#include <QtCore/QDebug>

#include <openssl/rand.h>

void CryptographicRandom::fillBuffer(void *buf, int numBytes) {
	// We treat negative and zero values of numBytes to be
	// fatal errors in the program. Abort the program if such
	// a value is passed to fillBuffer().
	if (numBytes <= 0) {
		qFatal("CryptographicRandom::fillBuffer(): numBytes is <= 0");
	}

	// RAND_bytes only returns an error if the entropy pool has not yet been sufficiently filled,
	// or in the case of a catastrophic, unrecoverable error in the RAND_bytes implementation happens.
	// OpenSSL needs at least 32-bytes of high-entropy random data to seed its CSPRNG.
	// If OpenSSL cannot acquire enough random data to seed its CSPRNG at the time Mumble and Murmur
	// are running, there is not much we can do about it other than aborting the program.
	if (RAND_bytes(reinterpret_cast< unsigned char * >(buf), numBytes) != 1) {
		qFatal("CryptographicRandom::fillBuffer(): internal error in OpenSSL's RAND_bytes or entropy pool not yet "
			   "filled.");
	}
}

uint32_t CryptographicRandom::uint32() {
	uint32_t ret = 0;

	unsigned char buf[4];
	CryptographicRandom::fillBuffer(buf, sizeof(buf));

	ret = buf[0];
	ret |= static_cast< decltype(ret) >(buf[1] << 8);
	ret |= static_cast< decltype(ret) >(buf[2] << 16);
	ret |= static_cast< decltype(ret) >(buf[3] << 24);
	return ret;
}

uint32_t CryptographicRandom::uniform(uint32_t upperBound) {
	// Implemented in 3rdparty/arc4random/arc4random_uniform.c
	return mumble_arc4random_uniform(upperBound);
}
