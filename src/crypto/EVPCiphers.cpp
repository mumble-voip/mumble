// Copyright 2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "EVPCiphers.h"

namespace EVPCipher {

const EVP_CIPHER *AES_256_GCM::cipher       = EVP_aes_256_gcm();
const EVP_CIPHER *CHACHA20_POLY1305::cipher = EVP_chacha20_poly1305();

} // namespace EVPCipher
