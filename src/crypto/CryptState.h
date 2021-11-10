// Copyright 2020-2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_CRYPTSTATE_H_
#define MUMBLE_CRYPTSTATE_H_

#include "Timer.h"
#include <string>

class CryptState {
private:
	Q_DISABLE_COPY(CryptState)
public:
	/// The plain input and encrypted output of the block cipher always preserve the same length.
	/// However, in order to transmit the authenticate tag and the packet sequence number, an extra
	/// "head" is needed to be attached to the packet.
	/// This length varies from cipher to cipher and is stored in this `headLength`.
	unsigned int headLength = 0;

	unsigned int uiGood   = 0;
	unsigned int uiLate   = 0;
	unsigned int uiLost   = 0;
	unsigned int uiResync = 0;

	unsigned int uiRemoteGood   = 0;
	unsigned int uiRemoteLate   = 0;
	unsigned int uiRemoteLost   = 0;
	unsigned int uiRemoteResync = 0;

	Timer tLastGood;
	Timer tLastRequest;
	bool bInit = false;
	CryptState(){};
	virtual ~CryptState(){};

	virtual bool isValid() const                                                                 = 0;
	virtual void genKey()                                                                        = 0;
	virtual bool setKey(const std::string &rkey, const std::string &eiv, const std::string &div) = 0;
	virtual bool setRawKey(const std::string &rkey)                                              = 0;
	virtual bool setEncryptIV(const std::string &iv)                                             = 0;
	virtual bool setDecryptIV(const std::string &iv)                                             = 0;
	virtual std::string getRawKey()                                                              = 0;
	virtual std::string getEncryptIV()                                                           = 0;
	virtual std::string getDecryptIV()                                                           = 0;

	/// Decrypt one packet. Return true if decryption was successful.
	virtual bool decrypt(const unsigned char *source, unsigned char *dst, unsigned int encrypted_length,
						 unsigned int &plain_length) = 0;

	/// Encrypt one packet. Return false if the current key-iv combination is not safe and a new set of key-
	/// iv is necessary, or the encryption result is not cryptographically safe.
	virtual bool encrypt(const unsigned char *source, unsigned char *dst, unsigned int plain_length,
						 unsigned int &encrypted_length) = 0;
};


#endif
