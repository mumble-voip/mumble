// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_CRYPTOGRAPHICRANDOM_H_
#define MUMBLE_CRYPTOGRAPHICRANDOM_H_

#include <stdint.h>

/// CryptographicRandom provides useful utilities for acquiring
/// pseudo-random data for use in cryptography.
class CryptographicRandom {
public:
	/// Fill the buffer at |buf| with |numBytes| bytes of pseudo-random data.
	/// The value of |numBytes| must be >= 0. Otherwise, program execution is aborted.
	///
	/// This function is equivalent to the arc4random_buf() function from OpenBSD.
	///
	/// This function should not ever fail. It is considered a catastrophic failure if
	/// it does. The program is aborted if the function fails, because it is generally
	/// impossible to recover from such a scenario.
	static void fillBuffer(void *buf, int numBytes);

	/// Return a pseudo-random number in the range of 0 to (2**32)-1.
	///
	/// This function is equivalent to the arc4random() function from OpenBSD.
	///
	/// This function should not ever fail. It is considered a catastrophic failure if
	/// it does. The program is aborted if the function fails, because it is generally
	/// impossible to recover from such a scenario.
	static uint32_t uint32();

	/// Calculate a uniformly distributed random number less than |upperBound|
	/// avoiding "modulo bias".
	///
	/// This function is equivalent to the arc4random_uniform() function from OpenBSD.
	///
	/// This function should not ever fail. It is considered a catastrophic failure if
	/// it does. The program is aborted if the function fails, because it is generally
	/// impossible to recover from such a scenario.
	static uint32_t uniform(uint32_t upperBound);
};

#endif
