// Copyright 2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

// This code aims to provide a unified interface to operate the EVP interface of OpenSSL.
// See https://wiki.openssl.org/index.php/EVP_Authenticated_Encryption_and_Decryption

#ifndef MUMBLE_CRYPTOSTATEEVP_H
#define MUMBLE_CRYPTOSTATEEVP_H

#include "CryptState.h"
#include "EVPCiphers.h"

#include <openssl/evp.h>
#include <openssl/rand.h>


template< class T > class CryptStateEVP : public CryptState {
public:
	explicit CryptStateEVP();

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

	unsigned char raw_key[T::KEY_LENGTH];
	unsigned char encrypt_iv[T::IV_LENGTH];
	unsigned char decrypt_iv[T::IV_LENGTH];
	unsigned char decrypt_history[0x100];
};


template< typename T > CryptStateEVP< T >::CryptStateEVP() : CryptState() {
	cipher = T::cipher;

	// headLength = tag length + one byte of IV
	headLength = T::TAG_LENGTH + 1;

	for (int i = 0; i < 0x100; i++)
		decrypt_history[i] = 0;

	memset(raw_key, 0, T::KEY_LENGTH);
	memset(encrypt_iv, 0, T::IV_LENGTH);
	memset(decrypt_iv, 0, T::IV_LENGTH);
}
template< typename T > bool CryptStateEVP< T >::isValid() const {
	return bInit;
}
template< typename T > void CryptStateEVP< T >::genKey() {
	RAND_bytes(raw_key, T::KEY_LENGTH);
	RAND_bytes(encrypt_iv, T::IV_LENGTH);
	RAND_bytes(decrypt_iv, T::IV_LENGTH);
	bInit = true;
}

template< typename T >
bool CryptStateEVP< T >::setKey(const std::string &rkey, const std::string &eiv, const std::string &div) {
	if (rkey.length() == T::KEY_LENGTH && eiv.length() == T::IV_LENGTH && div.length() == T::IV_LENGTH) {
		memcpy(raw_key, rkey.data(), T::KEY_LENGTH);
		memcpy(encrypt_iv, eiv.data(), T::IV_LENGTH);
		memcpy(decrypt_iv, div.data(), T::IV_LENGTH);
		bInit = true;
		return true;
	}
	return false;
}

template< typename T > bool CryptStateEVP< T >::setRawKey(const std::string &rkey) {
	if (rkey.length() == T::KEY_LENGTH) {
		memcpy(raw_key, rkey.data(), T::KEY_LENGTH);
		return true;
	}
	return false;
}

template< typename T > bool CryptStateEVP< T >::setEncryptIV(const std::string &iv) {
	if (iv.length() == T::IV_LENGTH) {
		memcpy(encrypt_iv, iv.data(), T::IV_LENGTH);
		return true;
	}
	return false;
}

template< typename T > bool CryptStateEVP< T >::setDecryptIV(const std::string &iv) {
	if (iv.length() == T::IV_LENGTH) {
		memcpy(decrypt_iv, iv.data(), T::IV_LENGTH);
		return true;
	}
	return false;
}

template< typename T > std::string CryptStateEVP< T >::getRawKey() {
	return std::string(reinterpret_cast< const char * >(raw_key), T::KEY_LENGTH);
}

template< typename T > std::string CryptStateEVP< T >::getEncryptIV() {
	return std::string(reinterpret_cast< const char * >(encrypt_iv), T::IV_LENGTH);
}

template< typename T > std::string CryptStateEVP< T >::getDecryptIV() {
	return std::string(reinterpret_cast< const char * >(decrypt_iv), T::IV_LENGTH);
}

template< typename T >
bool CryptStateEVP< T >::encrypt(const unsigned char *source, unsigned char *dst, unsigned int plain_length,
								 unsigned int &encrypted_length) {
	EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
	unsigned char tag[T::TAG_LENGTH];

	// First, increase our IV.
	unsigned int increase_bit;
	for (increase_bit = 0; increase_bit < T::IV_LENGTH - 8; increase_bit++)
		if (++encrypt_iv[increase_bit])
			break;

	if (increase_bit == T::IV_LENGTH - 8) {
		// the counter go backs to 0 and start to repeat itself
		// regenerate iv, return false to trigger a cipher resync
		// that generates a new key-iv combination
		return false;
	}

	if (1 != EVP_EncryptInit(ctx, cipher, raw_key, encrypt_iv))
		return false;

	encrypted_length = 0;
	int outlen       = 0;

	unsigned char *ciphertext = dst + T::TAG_LENGTH + 1;

	if (1 != EVP_EncryptUpdate(ctx, ciphertext + encrypted_length, &outlen, source + encrypted_length, plain_length))
		return false;
	encrypted_length += outlen;

	if (1 != EVP_EncryptFinal(ctx, ciphertext + encrypted_length, &outlen))
		return false;
	encrypted_length += outlen;

	if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_AEAD_GET_TAG, T::TAG_LENGTH, tag))
		return false;
	EVP_CIPHER_CTX_free(ctx);

	// Filling the packet "header" with part of the counter of the iv and the authentication tag
	dst[0] = encrypt_iv[0];
	memcpy(dst + 1, tag, T::TAG_LENGTH);

	encrypted_length += T::TAG_LENGTH + 1;

	return true;
}

template< typename T >
bool CryptStateEVP< T >::decrypt(const unsigned char *source, unsigned char *dst, unsigned int encrypted_length,
								 unsigned int &plain_length) {
	if (encrypted_length < T::TAG_LENGTH + 1)
		return false;

	unsigned char saveiv[T::IV_LENGTH];
	unsigned char ivbyte = source[0];
	bool restore         = false;

	int lost = 0;
	int late = 0;

	memcpy(saveiv, decrypt_iv, T::IV_LENGTH);

	if (((decrypt_iv[0] + 1) & 0xFF) == ivbyte) {
		// In order as expected.
		if (ivbyte > decrypt_iv[0]) {
			decrypt_iv[0] = ivbyte;
		} else if (ivbyte < decrypt_iv[0]) {
			decrypt_iv[0] = ivbyte;
			for (unsigned int i = 1; i < T::IV_LENGTH - 8; i++)
				if (++decrypt_iv[i])
					break;
			// no need to handle the case of iv getting back to 0
			// the server will trigger a cipher resync anyway
		} else {
			return false;
		}
	} else {
		// This is either out of order or a repeat.

		int diff = ivbyte - decrypt_iv[0];
		if (diff > 128)
			diff = diff - 256;
		else if (diff < -128)
			diff = diff + 256;

		if ((ivbyte < decrypt_iv[0]) && (diff > -30) && (diff < 0)) {
			// Late packet, but no wraparound.
			late          = 1;
			lost          = -1;
			decrypt_iv[0] = ivbyte;
			restore       = true;
		} else if ((ivbyte > decrypt_iv[0]) && (diff > -30) && (diff < 0)) {
			// Last was 0x02, here comes 0xff from last round
			late          = 1;
			lost          = -1;
			decrypt_iv[0] = ivbyte;
			for (unsigned int i = 1; i < T::IV_LENGTH - 8; i++)
				if (decrypt_iv[i]--)
					break;
			restore = true;
		} else if ((ivbyte > decrypt_iv[0]) && (diff > 0)) {
			// Lost a few packets, but beyond that we're good.
			lost          = ivbyte - decrypt_iv[0] - 1;
			decrypt_iv[0] = ivbyte;
		} else if ((ivbyte < decrypt_iv[0]) && (diff > 0)) {
			// Lost a few packets, and wrapped around
			lost          = 256 - decrypt_iv[0] + ivbyte - 1;
			decrypt_iv[0] = ivbyte;
			for (unsigned int i = 1; i < T::IV_LENGTH - 8; i++)
				if (++decrypt_iv[i])
					break;
		} else {
			return false;
		}

		if (decrypt_history[decrypt_iv[0]] == decrypt_iv[1]) {
			memcpy(decrypt_iv, saveiv, T::IV_LENGTH);
			return false;
		}
	}

	EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();

	EVP_DecryptInit(ctx, cipher, raw_key, decrypt_iv);

	EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, T::TAG_LENGTH, (void *) (source + 1));

	const unsigned char *ciphertext = source + T::TAG_LENGTH + 1;

	plain_length               = 0;
	unsigned int cipher_length = encrypted_length - T::TAG_LENGTH - 1;
	int outlen                 = 0;

	EVP_DecryptUpdate(ctx, dst + plain_length, &outlen, ciphertext + plain_length, cipher_length);
	plain_length += outlen;

	bool dec_success = EVP_DecryptFinal(ctx, dst + plain_length, &outlen);
	EVP_CIPHER_CTX_free(ctx);


	if (!dec_success) {
		memcpy(decrypt_iv, saveiv, T::IV_LENGTH);
		return false;
	}
	decrypt_history[decrypt_iv[0]] = decrypt_iv[1];

	if (restore)
		memcpy(decrypt_iv, saveiv, T::IV_LENGTH);

	uiGood++;
	uiLate += late;
	uiLost += lost;

	tLastGood.restart();
	return true;
}

using CryptStateAES256GCM        = CryptStateEVP< EVPCipher::AES_256_GCM >;
using CryptStateChaCha20Poly1305 = CryptStateEVP< EVPCipher::CHACHA20_POLY1305 >;

#endif // MUMBLE_CRYPTOSTATEEVP_H
