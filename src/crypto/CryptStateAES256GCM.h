// Copyright 2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

// This code invoke the EVP interface of OpenSSL.
// See https://wiki.openssl.org/index.php/EVP_Authenticated_Encryption_and_Decryption
// Regarding GCM, see NIST Special Publication 800-38D
// https://nvlpubs.nist.gov/nistpubs/Legacy/SP/nistspecialpublication800-38d.pdf

#ifndef MUMBLE_CRYPTSTATEAES256GCM_H
#define MUMBLE_CRYPTSTATEAES256GCM_H

#include "crypto/CryptState.h"

#include <openssl/evp.h>


class CryptStateAES256GCM : public CryptState {
public:
	static constexpr unsigned int IV_LENGTH  = 96 / 8;
	static constexpr unsigned int KEY_LENGTH = 256 / 8;

	// See NIST SP 800-38D (https://nvlpubs.nist.gov/nistpubs/Legacy/SP/nistspecialpublication800-38d.pdf)
	// for required tag length
	static constexpr unsigned int TAG_LENGTH = 64 / 8;

	/// The head of the packet is one iv byte plus tag.
	static constexpr unsigned int HEAD_LENGTH = TAG_LENGTH + 1;


	CryptStateAES256GCM();
	~CryptStateAES256GCM(){};

	bool isValid() const override;
	void genKey() override;
	bool setKey(const std::string &rkey, const std::string &eiv, const std::string &div) override;
	bool setRawKey(const std::string &rkey) override;
	bool setEncryptIV(const std::string &iv) override;
	bool setDecryptIV(const std::string &iv) override;
	std::string getRawKey() override;
	std::string getEncryptIV() override;
	std::string getDecryptIV() override;

	bool decrypt(const unsigned char *source, unsigned char *dst, unsigned int encrypted_length,
				 unsigned int &plain_length) override;
	bool encrypt(const unsigned char *source, unsigned char *dst, unsigned int plain_length,
				 unsigned int &encrypted_length) override;

private:
	const EVP_CIPHER *cipher;

	unsigned char raw_key[KEY_LENGTH];
	unsigned char encrypt_iv[IV_LENGTH];
	unsigned char decrypt_iv[IV_LENGTH];
	unsigned char decrypt_history[0x100];
};


#endif // MUMBLE_CRYPTSTATEAES256GCM_H
