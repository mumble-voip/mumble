// Copyright 2020-2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_CRYPTSTATEOCB2_H
#define MUMBLE_CRYPTSTATEOCB2_H

#include "CryptState.h"

#include <openssl/aes.h>

#define AES_KEY_SIZE_BITS 128
#define AES_KEY_SIZE_BYTES (AES_KEY_SIZE_BITS / 8)


class CryptStateOCB2 : public CryptState {
public:
	static constexpr unsigned int IV_LENGTH  = AES_BLOCK_SIZE;
	static constexpr unsigned int KEY_LENGTH = AES_KEY_SIZE_BYTES;

	/// The head is one iv byte plus 3 bytes of tag
	static const unsigned int HEAD_LENGTH = 4;

	CryptStateOCB2();
	~CryptStateOCB2(){};

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

	bool ocb_encrypt(const unsigned char *plain, unsigned char *encrypted, unsigned int len, const unsigned char *nonce,
					 unsigned char *tag, bool modifyPlainOnXEXStarAttack = true);
	bool ocb_decrypt(const unsigned char *encrypted, unsigned char *plain, unsigned int len, const unsigned char *nonce,
					 unsigned char *tag);

private:
	unsigned char raw_key[AES_KEY_SIZE_BYTES];
	unsigned char encrypt_iv[AES_BLOCK_SIZE];
	unsigned char decrypt_iv[AES_BLOCK_SIZE];
	unsigned char decrypt_history[0x100];

	AES_KEY encrypt_key;
	AES_KEY decrypt_key;
};


#endif // MUMBLE_CRYPTSTATEOCB2_H
