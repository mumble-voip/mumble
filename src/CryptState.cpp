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
	AES_set_encrypt_key(raw_key, 128, &crypt_key);
	bInit = true;
}

void CryptState::setKey(const unsigned char *rkey, const unsigned char *eiv, const unsigned char *div) {
	memcpy(raw_key, rkey, AES_BLOCK_SIZE);
	memcpy(encrypt_iv, eiv, AES_BLOCK_SIZE);
	memcpy(decrypt_iv, div, AES_BLOCK_SIZE);
	AES_set_encrypt_key(raw_key, 128, &crypt_key);
	bInit = true;
}

void CryptState::setDecryptIV(const unsigned char *iv) {
	memcpy(decrypt_iv, iv, AES_BLOCK_SIZE);
}

void CryptState::encrypt(const unsigned char *source, unsigned char *dst, unsigned int plain_length) {
	unsigned int crypted_length = plain_length + 4;
	
	// First, increase our IV.
	for(int i=0;i<AES_BLOCK_SIZE;i++)
		if (++encrypt_iv[i])
			break;
	
	dst[0] = encrypt_iv[0];
	
	// Checksum (cheeze it)
	dst[1] = encrypt_iv[1];
	dst[2] = encrypt_iv[2];
	dst[3] = encrypt_iv[3];
	memcpy(dst + 4, source, plain_length);
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
		for(int i=1;i<AES_BLOCK_SIZE;i++)
			if (++decrypt_iv[i])
				break;
		decrypt_iv[0] = ivbyte;
	} else {
		return false;
	}
	if (memcmp(source+1, & decrypt_iv[1], 3) != 0) {
		memcpy(decrypt_iv, saveiv, AES_BLOCK_SIZE);
		return false;
	}
	memcpy(dst, source + 4, plain_length);
	tLastGood.restart();
	return true;
}

