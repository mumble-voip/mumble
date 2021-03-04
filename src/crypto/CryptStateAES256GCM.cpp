// Copyright 2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

// This code invoke the EVP interface of OpenSSL.
// See https://wiki.openssl.org/index.php/EVP_Authenticated_Encryption_and_Decryption
// Regarding GCM, see NIST Special Publication 800-38D
// https://nvlpubs.nist.gov/nistpubs/Legacy/SP/nistspecialpublication800-38d.pdf

#include <QtCore/QtGlobal>

#ifndef __LP64__
#	ifdef Q_OS_WIN
#		include "win.h"
#		include <winsock2.h>
#	else
#		include <arpa/inet.h>
#	endif
#endif

#include "CryptStateAES256GCM.h"

#include <cstring>
#include <openssl/err.h>
#include <openssl/rand.h>

CryptStateAES256GCM::CryptStateAES256GCM() : CryptState() {
	for (int i = 0; i < 0x100; i++)
		decrypt_history[i] = 0;
	memset(raw_key, 0, KEY_LENGTH);
	memset(encrypt_iv, 0, IV_LENGTH);
	memset(decrypt_iv, 0, IV_LENGTH);
	cipher = EVP_aes_256_gcm();
}

bool CryptStateAES256GCM::isValid() const {
	return bInit;
}

void CryptStateAES256GCM::genKey() {
	RAND_bytes(raw_key, KEY_LENGTH);
	RAND_bytes(encrypt_iv, IV_LENGTH);
	RAND_bytes(decrypt_iv, IV_LENGTH);
	bInit = true;
}

bool CryptStateAES256GCM::setKey(const std::string &rkey, const std::string &eiv, const std::string &div) {
	if (rkey.length() == KEY_LENGTH && eiv.length() == IV_LENGTH && div.length() == IV_LENGTH) {
		memcpy(raw_key, rkey.data(), KEY_LENGTH);
		memcpy(encrypt_iv, eiv.data(), IV_LENGTH);
		memcpy(decrypt_iv, div.data(), IV_LENGTH);
		bInit = true;
		return true;
	}
	return false;
}

bool CryptStateAES256GCM::setRawKey(const std::string &rkey) {
	if (rkey.length() == KEY_LENGTH) {
		memcpy(raw_key, rkey.data(), KEY_LENGTH);
		return true;
	}
	return false;
}

bool CryptStateAES256GCM::setEncryptIV(const std::string &iv) {
	if (iv.length() == IV_LENGTH) {
		memcpy(encrypt_iv, iv.data(), IV_LENGTH);
		return true;
	}
	return false;
}

bool CryptStateAES256GCM::setDecryptIV(const std::string &iv) {
	if (iv.length() == IV_LENGTH) {
		memcpy(decrypt_iv, iv.data(), IV_LENGTH);
		return true;
	}
	return false;
}

std::string CryptStateAES256GCM::getRawKey() {
	return std::string(reinterpret_cast< const char * >(raw_key), KEY_LENGTH);
}

std::string CryptStateAES256GCM::getEncryptIV() {
	return std::string(reinterpret_cast< const char * >(encrypt_iv), IV_LENGTH);
}

std::string CryptStateAES256GCM::getDecryptIV() {
	return std::string(reinterpret_cast< const char * >(decrypt_iv), IV_LENGTH);
}

bool CryptStateAES256GCM::encrypt(const unsigned char *source, unsigned char *dst, unsigned int plain_length,
								  unsigned int &encrypted_length) {
	EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
	unsigned char tag[TAG_LENGTH];

	// First, increase our IV.
	// IV generation procedure is detailed in NIST 800-38D Sec. 8.2
	// The construction we adopt here is Deterministic Construction in 8.2.1
	// 32-bit of IV is fixed field and the remaining 64-bit is
	// "invocation field", which is an integer counter.
	unsigned int increase_bit;
	for (increase_bit = 0; increase_bit < IV_LENGTH - 8; increase_bit++)
		if (++encrypt_iv[increase_bit])
			break;

	if (increase_bit == IV_LENGTH - 8) {
		// the counter go backs to 0 and start to repeat itself
		// regenerate iv, return false to trigger a cipher resync
		// that generates a new key-iv combination
		return false;
	}

	if (1 != EVP_EncryptInit(ctx, cipher, raw_key, encrypt_iv))
		return false;

	encrypted_length = 0;
	int outlen       = 0;

	unsigned char *ciphertext = dst + TAG_LENGTH + 1;

	if (1 != EVP_EncryptUpdate(ctx, ciphertext + encrypted_length, &outlen, source + encrypted_length, plain_length))
		return false;
	encrypted_length += outlen;

	if (1 != EVP_EncryptFinal(ctx, ciphertext + encrypted_length, &outlen))
		return false;
	encrypted_length += outlen;

	if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, TAG_LENGTH, tag))
		return false;
	EVP_CIPHER_CTX_free(ctx);

	// Filling the packet "header" with part of the counter of the iv and the authentication tag
	dst[0] = encrypt_iv[0];
	memcpy(dst + 1, tag, TAG_LENGTH);

	encrypted_length += TAG_LENGTH + 1;

	return true;
}

bool CryptStateAES256GCM::decrypt(const unsigned char *source, unsigned char *dst, unsigned int encrypted_length,
								  unsigned int &plain_length) {
	if (encrypted_length < TAG_LENGTH + 1)
		return false;

	unsigned char saveiv[IV_LENGTH];
	unsigned char ivbyte = source[0];
	bool restore         = false;

	int lost = 0;
	int late = 0;

	memcpy(saveiv, decrypt_iv, IV_LENGTH);

	if (((decrypt_iv[0] + 1) & 0xFF) == ivbyte) {
		// In order as expected.
		if (ivbyte > decrypt_iv[0]) {
			decrypt_iv[0] = ivbyte;
		} else if (ivbyte < decrypt_iv[0]) {
			decrypt_iv[0] = ivbyte;
			for (unsigned int i = 1; i < IV_LENGTH - 8; i++)
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
			for (unsigned int i = 1; i < IV_LENGTH - 8; i++)
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
			for (unsigned int i = 1; i < IV_LENGTH - 8; i++)
				if (++decrypt_iv[i])
					break;
		} else {
			return false;
		}

		if (decrypt_history[decrypt_iv[0]] == decrypt_iv[1]) {
			memcpy(decrypt_iv, saveiv, IV_LENGTH);
			return false;
		}
	}

	EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();

	EVP_DecryptInit(ctx, cipher, raw_key, decrypt_iv);

	EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, TAG_LENGTH, (void *) (source + 1));

	const unsigned char *ciphertext = source + TAG_LENGTH + 1;

	plain_length               = 0;
	unsigned int cipher_length = encrypted_length - TAG_LENGTH - 1;
	int outlen                 = 0;

	EVP_DecryptUpdate(ctx, dst + plain_length, &outlen, ciphertext + plain_length, cipher_length);
	plain_length += outlen;

	bool dec_success = EVP_DecryptFinal(ctx, dst + plain_length, &outlen);
	EVP_CIPHER_CTX_free(ctx);


	if (!dec_success) {
		memcpy(decrypt_iv, saveiv, IV_LENGTH);
		return false;
	}
	decrypt_history[decrypt_iv[0]] = decrypt_iv[1];

	if (restore)
		memcpy(decrypt_iv, saveiv, IV_LENGTH);

	uiGood++;
	uiLate += late;
	uiLost += lost;

	tLastGood.restart();
	return true;
}
