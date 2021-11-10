// Copyright 2005-2020 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "CryptStateFactory.h"
#include "CryptStateEVP.h"
#include "CryptStateOCB2.h"

CryptStateFactory::CryptStateFactory() {
	m_availableCiphers.push_back(CipherType::AES_128_OCB2);
	m_availableCiphers.push_back(CipherType::AES_256_GCM);
	m_availableCiphers.push_back(CipherType::CHACHA20_POLY1305);

	for (CipherType cipher : m_availableCiphers) {
		m_availableCipherLookup[CipherTypeDescription(cipher).name] = cipher;
	}
}

CryptStateFactory &CryptStateFactory::getFactory() {
	static CryptStateFactory factory;
	return factory;
}

std::unique_ptr< CryptState > CryptStateFactory::getCryptState(CipherType cipherType) {
	switch (cipherType) {
		case CipherType::AES_128_OCB2:
			return std::make_unique< CryptStateOCB2 >();
		case CipherType::AES_256_GCM:
			return std::make_unique< CryptStateAES256GCM >();
		case CipherType::CHACHA20_POLY1305:
			return std::make_unique< CryptStateChaCha20Poly1305 >();
		default:
			throw CipherUnavailableException();
	}
}

const std::vector< CipherType > CryptStateFactory::getAvailableCiphers() {
	return m_availableCiphers;
}

CipherType CryptStateFactory::getCipherType(std::string name) {
	auto search = m_availableCipherLookup.find(name);
	if (search != m_availableCipherLookup.end())
		return search->second;
	else
		return CipherType::INVALID;
}
