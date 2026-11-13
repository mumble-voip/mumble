// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_CRYPTSTATEOCB2_H
#define MUMBLE_CRYPTSTATEOCB2_H

#include "CryptState.h"

#include <openssl/evp.h>

#define AES_BLOCK_SIZE 16
#define AES_KEY_SIZE_BITS 128
#define AES_KEY_SIZE_BYTES (AES_KEY_SIZE_BITS / 8)


class CryptStateOCB2 : public CryptState {
public:
	/// The largest number of consecutive lost packets that decrypt() can recover from on its own. Beyond this, the
	/// decrypt IV stays out of sync until the IVs are explicitly resynchronized (which only happens after 5 seconds
	/// without a successfully decrypted packet).
	static constexpr unsigned int MAX_RECOVERABLE_PACKET_LOSS = 1024;

	CryptStateOCB2();
	~CryptStateOCB2() noexcept override;

	virtual bool isValid() const Q_DECL_OVERRIDE;
	virtual void genKey() Q_DECL_OVERRIDE;
	virtual bool setKey(const std::string &rkey, const std::string &eiv, const std::string &div) Q_DECL_OVERRIDE;
	virtual bool setRawKey(const std::string &rkey) Q_DECL_OVERRIDE;
	virtual bool setEncryptIV(const std::string &iv) Q_DECL_OVERRIDE;
	virtual bool setDecryptIV(const std::string &iv) Q_DECL_OVERRIDE;
	virtual std::string getRawKey() Q_DECL_OVERRIDE;
	virtual std::string getEncryptIV() Q_DECL_OVERRIDE;
	virtual std::string getDecryptIV() Q_DECL_OVERRIDE;

	virtual bool decrypt(const unsigned char *source, unsigned char *dst, unsigned int crypted_length) Q_DECL_OVERRIDE;
	virtual bool encrypt(const unsigned char *source, unsigned char *dst, unsigned int plain_length) Q_DECL_OVERRIDE;

	bool ocb_encrypt(const unsigned char *plain, unsigned char *encrypted, unsigned int len, const unsigned char *nonce,
					 unsigned char *tag, bool modifyPlainOnXEXStarAttack = true);
	bool ocb_decrypt(const unsigned char *encrypted, unsigned char *plain, unsigned int len, const unsigned char *nonce,
					 unsigned char *tag);

private:
	/// Called when decrypt() rejects a packet. Assumes that more than 128 packets have been lost since the last one
	/// that was received and tries all IVs up to MAX_RECOVERABLE_PACKET_LOSS packets ahead whose least significant
	/// byte matches the one transmitted in the packet. On success, the packet is decrypted into dst and the decrypt IV
	/// is moved forward to the IV that worked.
	///
	/// @param source The encrypted packet (including the 4-byte header)
	/// @param dst The buffer to write the decrypted data to
	/// @param plain_length The length of the decrypted data
	/// @returns Whether the packet could be decrypted
	bool recoverFromPacketLoss(const unsigned char *source, unsigned char *dst, unsigned int plain_length);

	unsigned char raw_key[AES_KEY_SIZE_BYTES];
	unsigned char encrypt_iv[AES_BLOCK_SIZE];
	unsigned char decrypt_iv[AES_BLOCK_SIZE];
	unsigned char decrypt_history[0x100];

	EVP_CIPHER_CTX *enc_ctx_ocb_enc;
	EVP_CIPHER_CTX *dec_ctx_ocb_enc;
	EVP_CIPHER_CTX *enc_ctx_ocb_dec;
	EVP_CIPHER_CTX *dec_ctx_ocb_dec;
};


#endif // MUMBLE_CRYPTSTATEOCB2_H
