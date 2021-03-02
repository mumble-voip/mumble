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

	virtual bool decrypt(const unsigned char *source, unsigned char *dst, unsigned int crypted_length) = 0;
	virtual bool encrypt(const unsigned char *source, unsigned char *dst, unsigned int plain_length)   = 0;
};


#endif
