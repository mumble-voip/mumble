// Copyright 2015-2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_SSLCIPHERINFO_H_
#define MUMBLE_SSLCIPHERINFO_H_

struct SSLCipherInfo {
	const char *openssl_name;
	const char *rfc_name;
	const char *encryption;
	const char *key_exchange_verbose;
	const bool forward_secret;
	const char *message_authentication;
};

const SSLCipherInfo *SSLCipherInfoLookupByOpenSSLName(const char *openSslCipherName);

#endif
