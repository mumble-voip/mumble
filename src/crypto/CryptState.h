// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_CRYPTSTATE_H_
#define MUMBLE_CRYPTSTATE_H_

#include "Timer.h"
#include <chrono>
#include <queue>
#include <string>

struct PacketStats {
	unsigned int good   = 0;
	unsigned int late   = 0;
	unsigned int lost   = 0;
	unsigned int resync = 0;
};

struct PacketStatsSnapshot {
	PacketStats stats;
	std::chrono::time_point< std::chrono::steady_clock > timestamp;
};

class CryptState {
private:
	Q_DISABLE_COPY(CryptState)

	const std::chrono::seconds m_rollingScanInterval                             = std::chrono::seconds(5);
	std::chrono::time_point< std::chrono::steady_clock > m_rollingLastSampleTime = {};

	std::queue< PacketStatsSnapshot > m_statsLocalReference;
	std::queue< PacketStatsSnapshot > m_statsRemoteReference;

protected:
	void updateRollingStats();

public:
	PacketStats m_statsLocal         = {};
	PacketStats m_statsRemote        = {};
	PacketStats m_statsLocalRolling  = {};
	PacketStats m_statsRemoteRolling = {};

	bool m_rollingStatsEnabled = false;
	/// This is the packet statistics sliding time window size in seconds
	std::chrono::duration< unsigned int, std::ratio< 1 > > m_rollingWindow = std::chrono::minutes(5);

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
