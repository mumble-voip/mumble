// Copyright 2005-2020 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_CIPHERTYPE_H
#define MUMBLE_CIPHERTYPE_H

#include <QObject>
#include <string>

enum class CipherType { INVALID = 0, AES_128_OCB2 = 1, AES_256_GCM = 2 };

struct CipherTypeDescription {
	CipherType type;
	std::string name;
	QString description;

	/// initialize the description of the given cipher type.
	CipherTypeDescription(CipherType type) {
		switch (type) {
			case CipherType::AES_128_OCB2:
				type        = CipherType::AES_128_OCB2;
				name        = "AES-128-OCB2";
				description = QObject::tr("Mumble's builtin 128-bit AES-OCB2 algorithm");
				break;
			case CipherType::AES_256_GCM:
				type        = CipherType::AES_256_GCM;
				name        = "AES-256-GCM";
				description = QObject::tr("OpenSSL's 256-bit AES-GCM algorithm");
				break;
			default:
				type        = CipherType::INVALID;
				name        = "";
				description = "";
		}
	}
};

#endif // MUMBLE_CIPHERTYPE_H
