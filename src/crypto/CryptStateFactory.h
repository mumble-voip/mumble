// Copyright 2005-2020 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_CRYPTSTATEFACTORY_H
#define MUMBLE_CRYPTSTATEFACTORY_H

#include "CipherType.h"
#include "CryptState.h"

#include <exception>
#include <map>
#include <memory>
#include <string>
#include <vector>


class CipherUnavailableException : std::exception {};


class CryptStateFactory {
private:
	std::vector< CipherType > m_availableCiphers;
	std::map< std::string, CipherType > m_availableCipherLookup;
	CryptStateFactory();

public:
	std::unique_ptr< CryptState > getCryptState(CipherType cipherType);
	const std::vector< CipherType > getAvailableCiphers();
	CipherType getCipherType(std::string);

	static CryptStateFactory &getFactory();
};


#endif // MUMBLE_CRYPTSTATEFACTORY_H
