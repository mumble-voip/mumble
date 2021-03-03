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
	static const unsigned int ivLength  = AES_BLOCK_SIZE;
	static const unsigned int keyLength = AES_KEY_SIZE_BYTES;

	/// The plain input and encrypted output of block cipher always preserve the same length.
	/// However, in order to transmit authenticate tag and the packet sequence number,
	/// An extra part will need to be attached to the packet.
	/// This length varies from cipher to cipher and is stored in this `headLength`.
	static const unsigned int headLength = 4;

	CryptStateOCB2();
	~CryptStateOCB2(){};

	bool isValid() const Q_DECL_OVERRIDE;
	void genKey() Q_DECL_OVERRIDE;
	bool setKey(const std::string &rkey, const std::string &eiv, const std::string &div) Q_DECL_OVERRIDE;
	bool setRawKey(const std::string &rkey) Q_DECL_OVERRIDE;
	bool setEncryptIV(const std::string &iv) Q_DECL_OVERRIDE;
	bool setDecryptIV(const std::string &iv) Q_DECL_OVERRIDE;
	std::string getRawKey() Q_DECL_OVERRIDE;
	std::string getEncryptIV() Q_DECL_OVERRIDE;
	std::string getDecryptIV() Q_DECL_OVERRIDE;

	bool decrypt(const unsigned char *source, unsigned char *dst, unsigned int encrypted_length,
				 unsigned int &plain_length) Q_DECL_OVERRIDE;
	bool encrypt(const unsigned char *source, unsigned char *dst, unsigned int plain_length,
				 unsigned int &encrypted_length) Q_DECL_OVERRIDE;

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
