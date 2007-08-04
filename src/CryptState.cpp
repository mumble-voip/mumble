/* Copyright (C) 2005-2007, Thorvald Natvig <thorvald@natvig.com>

   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.
   - Neither the name of the Mumble Developers nor the names of its
     contributors may be used to endorse or promote products derived from this
     software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*
 * This code implements OCB-AES128.
 * In the US, OCB is covered by patents. The inventor has given a license
 * to all programs distributed under the GPL.
 * Mumble is BSD (revised) licensed, meaning you can use the code in a
 * closed-source program. If you do, you'll have to either replace
 * OCB with something else or get yourself a license.
 */

#include "CryptState.h"

CryptState::CryptState() {
	bInit = false;
}

bool CryptState::isValid() const {
	return bInit;
}

void CryptState::genKey() {
	RAND_bytes(raw_key, AES_BLOCK_SIZE);
	RAND_bytes(encrypt_iv, AES_BLOCK_SIZE);
	RAND_bytes(decrypt_iv, AES_BLOCK_SIZE);
	AES_set_encrypt_key(raw_key, 128, &encrypt_key);
	AES_set_decrypt_key(raw_key, 128, &decrypt_key);
	bInit = true;
}

void CryptState::setKey(const unsigned char *rkey, const unsigned char *eiv, const unsigned char *div) {
	memcpy(raw_key, rkey, AES_BLOCK_SIZE);
	memcpy(encrypt_iv, eiv, AES_BLOCK_SIZE);
	memcpy(decrypt_iv, div, AES_BLOCK_SIZE);
	AES_set_encrypt_key(raw_key, 128, &encrypt_key);
	AES_set_decrypt_key(raw_key, 128, &decrypt_key);
	bInit = true;
}

void CryptState::setDecryptIV(const unsigned char *iv) {
	memcpy(decrypt_iv, iv, AES_BLOCK_SIZE);
}

void CryptState::encrypt(const unsigned char *source, unsigned char *dst, unsigned int plain_length) {
	unsigned char tag[AES_BLOCK_SIZE];

	// First, increase our IV.
	for (int i=0;i<AES_BLOCK_SIZE;i++)
		if (++encrypt_iv[i])
			break;

	ocb_encrypt(source, dst+4, plain_length, encrypt_iv, tag);

	dst[0] = encrypt_iv[0];
	dst[1] = tag[0];
	dst[2] = tag[1];
	dst[3] = tag[2];
}

bool CryptState::decrypt(const unsigned char *source, unsigned char *dst, unsigned int crypted_length) {
	if (crypted_length < 4)
		return false;

	unsigned int plain_length = crypted_length - 4;

	unsigned char saveiv[AES_BLOCK_SIZE];
	unsigned char ivbyte = source[0];

	memcpy(saveiv, decrypt_iv, AES_BLOCK_SIZE);

	if (ivbyte > decrypt_iv[0]) {
		decrypt_iv[0] = ivbyte;
	} else if (ivbyte < decrypt_iv[0]) {
		for (int i=1;i<AES_BLOCK_SIZE;i++)
			if (++decrypt_iv[i])
				break;
		decrypt_iv[0] = ivbyte;
	} else {
		return false;
	}

	unsigned char tag[AES_BLOCK_SIZE];
	ocb_decrypt(source+4, dst, plain_length, decrypt_iv, tag);

	if (memcmp(tag, source+1, 3) != 0) {
		memcpy(decrypt_iv, saveiv, AES_BLOCK_SIZE);
		return false;
	}
	tLastGood.restart();
	return true;
}

static void inline XOR(unsigned char *dst, const unsigned char *a, const unsigned char *b) {
	for (int i=0;i<AES_BLOCK_SIZE;i++)
		dst[i] = a[i] ^ b[i];
}

static void inline S2(unsigned char *block) {
	unsigned char carry = block[0] >> 7;
	for (int i=0;i<AES_BLOCK_SIZE-1;i++)
		block[i] = (block[i] << 1) | (block[i+1] >> 7);
	block[AES_BLOCK_SIZE-1] = (block[AES_BLOCK_SIZE-1] << 1) ^(carry * 0x87);
}

static void inline S3(unsigned char *block) {
	unsigned char carry = block[0] >> 7;
	for (int i=0;i<AES_BLOCK_SIZE-1;i++)
		block[i] ^= (block[i] << 1) | (block[i+1] >> 7);
	block[AES_BLOCK_SIZE-1] ^= (block[AES_BLOCK_SIZE-1] << 1) ^(carry * 0x87);
}

void CryptState::ocb_encrypt(const unsigned char *plain, unsigned char *encrypted, unsigned int len, const unsigned char *nonce, unsigned char *tag) {
	unsigned char checksum[AES_BLOCK_SIZE];
	unsigned char delta[AES_BLOCK_SIZE];
	unsigned char tmp[AES_BLOCK_SIZE];
	unsigned char pad[AES_BLOCK_SIZE];

	// Initialize
	AES_encrypt(nonce, delta, &encrypt_key);
	memset(checksum, 0, AES_BLOCK_SIZE);

	while (len > AES_BLOCK_SIZE) {
		S2(delta);
		XOR(tmp, delta, plain);
		AES_encrypt(tmp, tmp, &encrypt_key);
		XOR(encrypted, delta, tmp);
		XOR(checksum, checksum, plain);
		len -= AES_BLOCK_SIZE;
		plain += AES_BLOCK_SIZE;
		encrypted += AES_BLOCK_SIZE;
	}

	S2(delta);
	memset(tmp, 0, AES_BLOCK_SIZE);
	tmp[AES_BLOCK_SIZE - 1] = len * 8;
	XOR(tmp, tmp, delta);
	AES_encrypt(tmp, pad, &encrypt_key);
	memcpy(tmp, plain, len);
	memcpy(tmp+len, pad+len, AES_BLOCK_SIZE - len);
	XOR(checksum, checksum, tmp);
	XOR(tmp, pad, tmp);
	memcpy(encrypted, tmp, len);

	S3(delta);
	XOR(tmp, delta, checksum);
	AES_encrypt(tmp, tag, &encrypt_key);
}

void CryptState::ocb_decrypt(const unsigned char *encrypted, unsigned char *plain, unsigned int len, const unsigned char *nonce, unsigned char *tag) {
	unsigned char checksum[AES_BLOCK_SIZE];
	unsigned char delta[AES_BLOCK_SIZE];
	unsigned char tmp[AES_BLOCK_SIZE];
	unsigned char pad[AES_BLOCK_SIZE];

	// Initialize
	AES_encrypt(nonce, delta, &encrypt_key);
	memset(checksum, 0, AES_BLOCK_SIZE);

	while (len > AES_BLOCK_SIZE) {
		S2(delta);
		XOR(tmp, delta, encrypted);
		AES_decrypt(tmp, tmp, &decrypt_key);
		XOR(plain, delta, tmp);
		XOR(checksum, checksum, plain);
		len -= AES_BLOCK_SIZE;
		plain += AES_BLOCK_SIZE;
		encrypted += AES_BLOCK_SIZE;
	}

	S2(delta);
	memset(tmp, 0, AES_BLOCK_SIZE);
	tmp[AES_BLOCK_SIZE - 1] = len * 8;
	XOR(tmp, tmp, delta);
	AES_encrypt(tmp, pad, &encrypt_key);
	memset(tmp, 0, AES_BLOCK_SIZE);
	memcpy(tmp, encrypted, len);
	XOR(tmp, tmp, pad);
	XOR(checksum, checksum, tmp);
	memcpy(plain, tmp, len);

	S3(delta);
	XOR(tmp, delta, checksum);
	AES_encrypt(tmp, tag, &encrypt_key);
}
