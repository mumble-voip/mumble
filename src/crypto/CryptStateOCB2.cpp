// Copyright 2020-2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

/*
 * This code implements OCB-AES128.
 * In the US, OCB is covered by patents. The inventor has given a license
 * to all programs distributed under the GPL.
 * Mumble is BSD (revised) licensed, meaning you can use the code in a
 * closed-source program. If you do, you'll have to either replace
 * OCB with something else or get yourself a license.
 */

#include <QtCore/QtGlobal>

#ifndef __LP64__
#	ifdef Q_OS_WIN
#		include "win.h"
#		include <winsock2.h>
#	else
#		include <arpa/inet.h>
#	endif
#endif

#include "ByteSwap.h"
#include "CryptStateOCB2.h"
#include "CryptographicRandom.h"

#include <cstring>
#include <openssl/rand.h>

CryptStateOCB2::CryptStateOCB2() : CryptState() {
	for (int i = 0; i < 0x100; i++)
		decrypt_history[i] = 0;
	memset(raw_key, 0, AES_KEY_SIZE_BYTES);
	memset(encrypt_iv, 0, AES_BLOCK_SIZE);
	memset(decrypt_iv, 0, AES_BLOCK_SIZE);
}

bool CryptStateOCB2::isValid() const {
	return bInit;
}

void CryptStateOCB2::genKey() {
	CryptographicRandom::fillBuffer(raw_key, AES_KEY_SIZE_BYTES);
	CryptographicRandom::fillBuffer(encrypt_iv, AES_BLOCK_SIZE);
	CryptographicRandom::fillBuffer(decrypt_iv, AES_BLOCK_SIZE);
	AES_set_encrypt_key(raw_key, AES_KEY_SIZE_BITS, &encrypt_key);
	AES_set_decrypt_key(raw_key, AES_KEY_SIZE_BITS, &decrypt_key);
	bInit = true;
}

bool CryptStateOCB2::setKey(const std::string &rkey, const std::string &eiv, const std::string &div) {
	if (rkey.length() == AES_KEY_SIZE_BYTES && eiv.length() == AES_BLOCK_SIZE && div.length() == AES_BLOCK_SIZE) {
		memcpy(raw_key, rkey.data(), AES_KEY_SIZE_BYTES);
		memcpy(encrypt_iv, eiv.data(), AES_BLOCK_SIZE);
		memcpy(decrypt_iv, div.data(), AES_BLOCK_SIZE);
		AES_set_encrypt_key(raw_key, AES_KEY_SIZE_BITS, &encrypt_key);
		AES_set_decrypt_key(raw_key, AES_KEY_SIZE_BITS, &decrypt_key);
		bInit = true;
		return true;
	}
	return false;
}

bool CryptStateOCB2::setRawKey(const std::string &rkey) {
	if (rkey.length() == AES_KEY_SIZE_BYTES) {
		memcpy(raw_key, rkey.data(), AES_KEY_SIZE_BYTES);
		return true;
	}
	return false;
}

bool CryptStateOCB2::setEncryptIV(const std::string &iv) {
	if (iv.length() == AES_BLOCK_SIZE) {
		memcpy(encrypt_iv, iv.data(), AES_BLOCK_SIZE);
		return true;
	}
	return false;
}

bool CryptStateOCB2::setDecryptIV(const std::string &iv) {
	if (iv.length() == AES_BLOCK_SIZE) {
		memcpy(decrypt_iv, iv.data(), AES_BLOCK_SIZE);
		return true;
	}
	return false;
}

std::string CryptStateOCB2::getRawKey() {
	return std::string(reinterpret_cast< const char * >(raw_key), AES_KEY_SIZE_BYTES);
}

std::string CryptStateOCB2::getEncryptIV() {
	return std::string(reinterpret_cast< const char * >(encrypt_iv), AES_BLOCK_SIZE);
}

std::string CryptStateOCB2::getDecryptIV() {
	return std::string(reinterpret_cast< const char * >(decrypt_iv), AES_BLOCK_SIZE);
}

bool CryptStateOCB2::encrypt(const unsigned char *source, unsigned char *dst, unsigned int plain_length) {
	unsigned char tag[AES_BLOCK_SIZE];

	// First, increase our IV.
	for (int i = 0; i < AES_BLOCK_SIZE; i++)
		if (++encrypt_iv[i])
			break;

	if (!ocb_encrypt(source, dst + 4, plain_length, encrypt_iv, tag)) {
		return false;
	}

	dst[0] = encrypt_iv[0];
	dst[1] = tag[0];
	dst[2] = tag[1];
	dst[3] = tag[2];
	return true;
}

bool CryptStateOCB2::decrypt(const unsigned char *source, unsigned char *dst, unsigned int crypted_length) {
	if (crypted_length < 4)
		return false;

	unsigned int plain_length = crypted_length - 4;

	unsigned char saveiv[AES_BLOCK_SIZE];
	unsigned char ivbyte = source[0];
	bool restore         = false;
	unsigned char tag[AES_BLOCK_SIZE];

	int lost = 0;
	int late = 0;

	memcpy(saveiv, decrypt_iv, AES_BLOCK_SIZE);

	if (((decrypt_iv[0] + 1) & 0xFF) == ivbyte) {
		// In order as expected.
		if (ivbyte > decrypt_iv[0]) {
			decrypt_iv[0] = ivbyte;
		} else if (ivbyte < decrypt_iv[0]) {
			decrypt_iv[0] = ivbyte;
			for (int i = 1; i < AES_BLOCK_SIZE; i++)
				if (++decrypt_iv[i])
					break;
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
			for (int i = 1; i < AES_BLOCK_SIZE; i++)
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
			for (int i = 1; i < AES_BLOCK_SIZE; i++)
				if (++decrypt_iv[i])
					break;
		} else {
			return false;
		}

		if (decrypt_history[decrypt_iv[0]] == decrypt_iv[1]) {
			memcpy(decrypt_iv, saveiv, AES_BLOCK_SIZE);
			return false;
		}
	}

	bool ocb_success = ocb_decrypt(source + 4, dst, plain_length, decrypt_iv, tag);

	if (!ocb_success || memcmp(tag, source + 1, 3) != 0) {
		memcpy(decrypt_iv, saveiv, AES_BLOCK_SIZE);
		return false;
	}
	decrypt_history[decrypt_iv[0]] = decrypt_iv[1];

	if (restore)
		memcpy(decrypt_iv, saveiv, AES_BLOCK_SIZE);

	uiGood++;
	uiLate += late;
	uiLost += lost;

	tLastGood.restart();
	return true;
}

#if defined(__LP64__)

#	define BLOCKSIZE 2
#	define SHIFTBITS 63
typedef quint64 subblock;

#	define SWAPPED(x) SWAP64(x)

#else
#	define BLOCKSIZE 4
#	define SHIFTBITS 31
typedef quint32 subblock;
#	define SWAPPED(x) htonl(x)
#endif

typedef subblock keyblock[BLOCKSIZE];

#define HIGHBIT (1 << SHIFTBITS);


static void inline XOR(subblock *dst, const subblock *a, const subblock *b) {
	for (int i = 0; i < BLOCKSIZE; i++) {
		dst[i] = a[i] ^ b[i];
	}
}

static void inline S2(subblock *block) {
	subblock carry = SWAPPED(block[0]) >> SHIFTBITS;
	for (int i = 0; i < BLOCKSIZE - 1; i++)
		block[i] = SWAPPED((SWAPPED(block[i]) << 1) | (SWAPPED(block[i + 1]) >> SHIFTBITS));
	block[BLOCKSIZE - 1] = SWAPPED((SWAPPED(block[BLOCKSIZE - 1]) << 1) ^ (carry * 0x87));
}

static void inline S3(subblock *block) {
	subblock carry = SWAPPED(block[0]) >> SHIFTBITS;
	for (int i = 0; i < BLOCKSIZE - 1; i++)
		block[i] ^= SWAPPED((SWAPPED(block[i]) << 1) | (SWAPPED(block[i + 1]) >> SHIFTBITS));
	block[BLOCKSIZE - 1] ^= SWAPPED((SWAPPED(block[BLOCKSIZE - 1]) << 1) ^ (carry * 0x87));
}

static void inline ZERO(keyblock &block) {
	for (int i = 0; i < BLOCKSIZE; i++)
		block[i] = 0;
}

#define AESencrypt(src, dst, key) \
	AES_encrypt(reinterpret_cast< const unsigned char * >(src), reinterpret_cast< unsigned char * >(dst), key);
#define AESdecrypt(src, dst, key) \
	AES_decrypt(reinterpret_cast< const unsigned char * >(src), reinterpret_cast< unsigned char * >(dst), key);

bool CryptStateOCB2::ocb_encrypt(const unsigned char *plain, unsigned char *encrypted, unsigned int len,
								 const unsigned char *nonce, unsigned char *tag, bool modifyPlainOnXEXStarAttack) {
	keyblock checksum, delta, tmp, pad;
	bool success = true;

	// Initialize
	AESencrypt(nonce, delta, &encrypt_key);
	ZERO(checksum);

	while (len > AES_BLOCK_SIZE) {
		// Counter-cryptanalysis described in section 9 of https://eprint.iacr.org/2019/311
		// For an attack, the second to last block (i.e. the last iteration of this loop)
		// must be all 0 except for the last byte (which may be 0 - 128).
		bool flipABit = false; // *plain is const, so we can't directly modify it
		if (len - AES_BLOCK_SIZE <= AES_BLOCK_SIZE) {
			unsigned char sum = 0;
			for (int i = 0; i < AES_BLOCK_SIZE - 1; ++i) {
				sum |= plain[i];
			}
			if (sum == 0) {
				if (modifyPlainOnXEXStarAttack) {
					// The assumption that critical packets do not turn up by pure chance turned out to be incorrect
					// since digital silence appears to produce them in mass.
					// So instead we now modify the packet in a way which should not affect the audio but will
					// prevent the attack.
					flipABit = true;
				} else {
					// This option still exists but only to allow us to test ocb_decrypt's detection.
					success = false;
				}
			}
		}

		S2(delta);
		XOR(tmp, delta, reinterpret_cast< const subblock * >(plain));
		if (flipABit) {
			*reinterpret_cast< unsigned char * >(tmp) ^= 1;
		}
		AESencrypt(tmp, tmp, &encrypt_key);
		XOR(reinterpret_cast< subblock * >(encrypted), delta, tmp);
		XOR(checksum, checksum, reinterpret_cast< const subblock * >(plain));
		if (flipABit) {
			*reinterpret_cast< unsigned char * >(checksum) ^= 1;
		}

		len -= AES_BLOCK_SIZE;
		plain += AES_BLOCK_SIZE;
		encrypted += AES_BLOCK_SIZE;
	}

	S2(delta);
	ZERO(tmp);
	tmp[BLOCKSIZE - 1] = SWAPPED(len * 8);
	XOR(tmp, tmp, delta);
	AESencrypt(tmp, pad, &encrypt_key);
	memcpy(tmp, plain, len);
	memcpy(reinterpret_cast< unsigned char * >(tmp) + len, reinterpret_cast< const unsigned char * >(pad) + len,
		   AES_BLOCK_SIZE - len);
	XOR(checksum, checksum, tmp);
	XOR(tmp, pad, tmp);
	memcpy(encrypted, tmp, len);

	S3(delta);
	XOR(tmp, delta, checksum);
	AESencrypt(tmp, tag, &encrypt_key);

	return success;
}

bool CryptStateOCB2::ocb_decrypt(const unsigned char *encrypted, unsigned char *plain, unsigned int len,
								 const unsigned char *nonce, unsigned char *tag) {
	keyblock checksum, delta, tmp, pad;
	bool success = true;

	// Initialize
	AESencrypt(nonce, delta, &encrypt_key);
	ZERO(checksum);

	while (len > AES_BLOCK_SIZE) {
		S2(delta);
		XOR(tmp, delta, reinterpret_cast< const subblock * >(encrypted));
		AESdecrypt(tmp, tmp, &decrypt_key);
		XOR(reinterpret_cast< subblock * >(plain), delta, tmp);
		XOR(checksum, checksum, reinterpret_cast< const subblock * >(plain));
		len -= AES_BLOCK_SIZE;
		plain += AES_BLOCK_SIZE;
		encrypted += AES_BLOCK_SIZE;
	}

	S2(delta);
	ZERO(tmp);
	tmp[BLOCKSIZE - 1] = SWAPPED(len * 8);
	XOR(tmp, tmp, delta);
	AESencrypt(tmp, pad, &encrypt_key);
	memset(tmp, 0, AES_BLOCK_SIZE);
	memcpy(tmp, encrypted, len);
	XOR(tmp, tmp, pad);
	XOR(checksum, checksum, tmp);
	memcpy(plain, tmp, len);

	// Counter-cryptanalysis described in section 9 of https://eprint.iacr.org/2019/311
	// In an attack, the decrypted last block would need to equal `delta ^ len(128)`.
	// With a bit of luck (or many packets), smaller values than 128 (i.e. non-full blocks) are also
	// feasible, so we check `tmp` instead of `plain`.
	// Since our `len` only ever modifies the last byte, we simply check all remaining ones.
	if (memcmp(tmp, delta, AES_BLOCK_SIZE - 1) == 0) {
		success = false;
	}

	S3(delta);
	XOR(tmp, delta, checksum);
	AESencrypt(tmp, tag, &encrypt_key);

	return success;
}

#undef BLOCKSIZE
#undef SHIFTBITS
#undef SWAPPED
#undef HIGHBIT
#undef AES_encrypt
#undef AES_decrypt
