// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ServerUser.h"

#include "ClientType.h"
#include "Meta.h"
#include "Server.h"

#ifdef Q_OS_UNIX
#	include "Utils.h"
#endif

ServerUser::ServerUser(Server *p, QSslSocket *socket)
	: Connection(p, socket), User(), s(nullptr), leakyBucket(p->iMessageLimit, p->iMessageBurst),
	  m_pluginMessageBucket(p->iPluginMessageLimit, p->iPluginMessageBurst) {
	sState       = ServerUser::Connected;
	m_clientType = ClientType::REGULAR;
	sUdpSocket   = INVALID_SOCKET;

	memset(&saiUdpAddress, 0, sizeof(saiUdpAddress));
	memset(&saiTcpLocalAddress, 0, sizeof(saiTcpLocalAddress));

	dUDPPingAvg = dUDPPingVar = 0.0f;
	dTCPPingAvg = dTCPPingVar = 0.0f;
	uiUDPPackets = uiTCPPackets = 0;

	aiUdpFlag            = 1;
	m_version            = Version::UNKNOWN;
	bVerified            = true;
	iLastPermissionCheck = -1;

	bOpus = false;
}


ServerUser::operator QString() const {
	return QString::fromLatin1("%1:%2(%3)").arg(qsName).arg(uiSession).arg(iId);
}
BandwidthRecord::BandwidthRecord() {
	iRecNum = 0;
	iSum    = 0;
	for (int i = 0; i < N_BANDWIDTH_SLOTS; i++)
		a_iBW[i] = 0;
}

bool BandwidthRecord::addFrame(int size, int maxpersec) {
	QMutexLocker ml(&qmMutex);

	quint64 elapsed = a_qtWhen[iRecNum].elapsed();

	if (elapsed == 0)
		return false;

	int nsum = iSum - a_iBW[iRecNum] + size;
	int bw   = static_cast< int >((static_cast< quint64 >(nsum) * 1000000ULL) / elapsed);

	if (bw > maxpersec)
		return false;

	a_iBW[iRecNum] = static_cast< unsigned short >(size);
	a_qtWhen[iRecNum].restart();

	iSum = nsum;

	iRecNum++;
	if (iRecNum == N_BANDWIDTH_SLOTS)
		iRecNum = 0;

	return true;
}

int BandwidthRecord::onlineSeconds() const {
	QMutexLocker ml(&qmMutex);

	return static_cast< int >(tFirst.elapsed() / 1000000LL);
}

int BandwidthRecord::idleSeconds() const {
	QMutexLocker ml(&qmMutex);

	quint64 iIdle = a_qtWhen[(iRecNum + N_BANDWIDTH_SLOTS - 1) % N_BANDWIDTH_SLOTS].elapsed();
	if (tIdleControl.elapsed() < iIdle)
		iIdle = tIdleControl.elapsed();

	return static_cast< int >(iIdle / 1000000LL);
}

void BandwidthRecord::resetIdleSeconds() {
	QMutexLocker ml(&qmMutex);

	tIdleControl.restart();
}

int BandwidthRecord::bandwidth() const {
	QMutexLocker ml(&qmMutex);

	int sum         = 0;
	quint64 elapsed = 0ULL;

	for (int i = 1; i < N_BANDWIDTH_SLOTS; ++i) {
		int idx   = (iRecNum + N_BANDWIDTH_SLOTS - i) % N_BANDWIDTH_SLOTS;
		quint64 e = a_qtWhen[idx].elapsed();
		if (e > 1000000ULL) {
			break;
		} else {
			sum += a_iBW[idx];
			elapsed = e;
		}
	}

	if (elapsed < 250000ULL)
		return 0;

	return static_cast< int >((static_cast< quint64 >(sum) * 1000000ULL) / elapsed);
}

LeakyBucket::LeakyBucket(unsigned int tokensPerSec, unsigned int maxTokens)
	: m_tokensPerSec(tokensPerSec), m_maxTokens(maxTokens), m_currentTokens(0), m_timer() {
	m_timer.start();

	if (!QElapsedTimer::isMonotonic()) {
		qFatal("Non-monotonic clocks are not reliable enough and lead to issues as "
			   "https://github.com/mumble-voip/mumble/issues/3985. "
			   "This is a serious issue and should be reported!");
	}
}

bool LeakyBucket::ratelimit(int tokens) {
	// First remove tokens we leaked over time
	const qint64 elapsedMillis = m_timer.elapsed();

	if (elapsedMillis < 0) {
		if (m_timer.isValid()) {
			// By definition of a monotinic clock, this shouldn't be possible to happen.
			// Thus if it does happen there's something going very wrong which is why we
			// emit the qFatal which at the state this line here was added, will crash
			// the server. But at least that guarantees to give us a report on the crash
			// instead of obscure reports about the rate limiter causing all sorts of
			// weird issues.
			qFatal("ServerUser.cpp: Monotonic timer returned negative elapsed time!");

			// In case the implementation changes and qFatal no longer crashes the server,
			// we'll restart the timer but limit the message.
			m_timer.restart();
			return true;
		} else {
			// For some reason the timer is in an invalid state. This shouldn't happen
			// as we start it in the constructor, but in case it does, we log the error
			// and reset the timer and the tokens as a fail-safe. We also won't limit
			// in this case as a potential limit is based on an invalid timer. That's
			// not what we want.
			qCritical("ServerUser.cpp: Monotonic timer is invalid!");
			m_timer.restart();
			m_currentTokens = 0;
			return false;
		}
	}

	const qint64 drainTokens = (elapsedMillis * m_tokensPerSec) / 1000;

	// Only restart the timer if enough time has elapsed so whe drain at least
	// a single token. If we were to restart the timer every time, even if the
	// interval between the calls to this function is so small that we don't
	// drain a token, we can end up in a situation in which we will never drain
	// a token even if let's say by the time this function is called the second
	// time we would have to drain a single token (over the course of both function
	// calls, but each function call individually wouldn't drain a token).
	if (drainTokens > 0) {
		m_timer.restart();
	}

	// Make sure that m_currentTokens never gets less than 0 by draining
	if (static_cast< qint64 >(m_currentTokens) < drainTokens) {
		m_currentTokens = 0;
	} else {
		m_currentTokens -= static_cast< decltype(m_currentTokens) >(drainTokens);
	}

	// Now that the tokens have been updated to reflect the constant drain caused by
	// the imaginary leaking bucket, we can check whether the given amount of tokens
	// still fit in this imaginary bucket (and thus the corresponding message may pass)
	// or if it doesn't (and thus the message will be limited (rejected))
	bool limit = m_currentTokens > ((static_cast< long >(m_maxTokens)) - tokens);

	// If the bucket is not overflowed, allow message and add tokens
	if (!limit) {
		m_currentTokens += static_cast< decltype(m_currentTokens) >(tokens);
	}

	return limit;
}
