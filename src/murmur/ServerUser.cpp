/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>
   Copyright (C) 2009, Stefan Hacker <dd0t@users.sourceforge.net>

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

#include "murmur_pch.h"

#include "ServerUser.h"
#include "Meta.h"

ServerUser::ServerUser(Server *p, QSslSocket *socket) : Connection(p, socket), User() {
	sState = ServerUser::Connected;
	sUdpSocket = INVALID_SOCKET;

	memset(&saiUdpAddress, 0, sizeof(saiUdpAddress));

	dUDPPingAvg = dUDPPingVar = 0.0f;
	dTCPPingAvg = dTCPPingVar = 0.0f;
	uiUDPPackets = uiTCPPackets = 0;

	bUdp = true;
	uiVersion = 0;
	bVerified = true;
	iLastPermissionCheck = -1;
}


ServerUser::operator const QString() const {
	return QString::fromLatin1("%1:%2(%3)").arg(qsName).arg(uiSession).arg(iId);
}
BandwidthRecord::BandwidthRecord() {
	iRecNum = 0;
	iSum = 0;
	for (int i=0;i<N_BANDWIDTH_SLOTS;i++)
		a_iBW[i] = 0;
}

bool BandwidthRecord::addFrame(int size, int maxpersec) {
	quint64 elapsed = a_qtWhen[iRecNum].elapsed();

	if (elapsed == 0)
		return false;

	int nsum = iSum-a_iBW[iRecNum]+size;
	int bw = static_cast<int>((nsum * 1000000LL) / elapsed);

	if (bw > maxpersec)
		return false;

	a_iBW[iRecNum] = static_cast<unsigned short>(size);
	a_qtWhen[iRecNum].restart();

	iSum = nsum;

	iRecNum++;
	if (iRecNum == N_BANDWIDTH_SLOTS)
		iRecNum = 0;

	return true;
}

int BandwidthRecord::onlineSeconds() const {
	return static_cast<int>(tFirst.elapsed() / 1000000LL);
}

int BandwidthRecord::idleSeconds() const {
	quint64 iIdle = a_qtWhen[(iRecNum + N_BANDWIDTH_SLOTS - 1) % N_BANDWIDTH_SLOTS].elapsed();
	if (tIdleControl.elapsed() < iIdle)
		iIdle = tIdleControl.elapsed();

	return static_cast<int>(iIdle / 1000000LL);
}

void BandwidthRecord::resetIdleSeconds() {
	tIdleControl.restart();
}

int BandwidthRecord::bandwidth() const {
	int sum = 0;
	int records = 0;
	quint64 elapsed = 0ULL;

	for (int i=1;i<N_BANDWIDTH_SLOTS;++i) {
		int idx = (iRecNum + N_BANDWIDTH_SLOTS - i) % N_BANDWIDTH_SLOTS;
		quint64 e = a_qtWhen[idx].elapsed();
		if (e > 1000000ULL) {
			break;
		} else {
			++records;
			sum += a_iBW[idx];
			elapsed = e;
		}
	}

	if (elapsed < 250000ULL)
		return 0;

	return static_cast<int>((sum * 1000000ULL) / elapsed);
}

