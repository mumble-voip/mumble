// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

/* Copyright (C) 2013, Morris Moraes <mauricioarozi@gmail.com>
   Copyright (C) 2014, Stefan Hacker <dd0t@users.sourceforge.net>

   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright notice,
	 this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright notice,
	 this list of conditions and the following disclaimer in the documentation
	 and/or other materials provided with the distribution.
   - Neither the name of the Mumble Developers nor the names of its
	 contributors may be used to endorse or promote products derived from this
	 software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#ifndef MUMBLE_MURMUR_PBKDF2_H_
#define MUMBLE_MURMUR_PBKDF2_H_

#include <stddef.h>

class QString;

///
/// Fully static wrapper class for PBKF2 password hashing functionality used in Murmur.
///
/// @note Using int all over the place because OpenSSL uses them in its C interface
///       and we want to make sure not to parameterize it in unexpected ways.
/// @warning Operations in this class that experience internal failure will abort
///          program execution using qFatal.
///
class PBKDF2 {
public:
	///
	/// @return Upper bound on iterations possible in
	///         BENCHMARK_DURATION_TARGET_IN_MS on this machine.
	///
	static int benchmark();

	/// Performs a PBKDF2 hash operation using EVP_sha384.
	///
	/// @param hexSalt Hex encoded salt to use in operation.
	/// @param password Password to hash.
	/// @param iterationCount Number of PBKDF2 iterations to apply.
	/// @return Hex encoded password hash of DERIVED_KEY_LENGTH octets.
	///
	static QString getHash(const QString &hexSalt, const QString &password, int iterationCount);

	///
	/// @return SALT_LENGTH octets of hex encoded random salt.
	///
	static QString getSalt();

	/// Length of hash in octets
	static const int DERIVED_KEY_LENGTH = 48;
	/// Length salt in octests
	static const int SALT_LENGTH = 8;

	/// Duration for hash operation the benchmark function should target
	static const int BENCHMARK_DURATION_TARGET_IN_MS = 10;
	/// Benchmark returns highest iteration number of N benchmark attempts
	static const size_t BENCHMARK_N = 40;
	/// Lower bound of iteration count returned by benchmark
	/// regardless of duration (should be divisible by 2)
	static const int BENCHMARK_MINIMUM_ITERATION_COUNT = 1000;
};

#endif // MUMBLE_MURMUR_PBKDF2_H_
