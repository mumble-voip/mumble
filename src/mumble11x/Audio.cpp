/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>

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

#include <math.h>

#include "Audio.h"
#include "AudioInput.h"
#include "AudioOutput.h"
#include "Global.h"

#define DOUBLE_RAND (rand()/static_cast<double>(RAND_MAX))

LoopPlayer LoopPlayer::lpLoopy;

LoopPlayer::LoopPlayer() {
	qsName = QLatin1String("Loopy");
	uiSession = 0;
	iId = 0;
	sState = Player::Authenticated;
	bMute = bDeaf = bSuppressed = false;
	bLocalMute = bSelfDeaf = false;
	bTalking = false;
	bAltSpeak = false;
	cChannel = NULL;
	qtTicker.start();
}

void LoopPlayer::addFrame(const QByteArray &packet, int seq) {
	if (DOUBLE_RAND < g.s.dPacketLoss) {
		qWarning("Drop");
		return;
	}

	bool restart = (qtLastFetch.elapsed() > 100);

	{
		QMutexLocker l(&qmLock);

		double time = qtTicker.elapsed();

		double r;
		if (restart)
			r = 0.0;
		else
			r = DOUBLE_RAND * g.s.dMaxPacketDelay;

		qmPackets.insert(static_cast<float>(time + r), Packet(seq, packet));
	}

	// Restart check
	if (qtLastFetch.elapsed() > 100) {
		AudioOutputPtr ao = g.ao;
		if (ao) {
			ao->addFrameToBuffer(this, QByteArray(), 0);
		}
	}

}

void LoopPlayer::fetchFrames() {
	QMutexLocker l(&qmLock);

	AudioOutputPtr ao = g.ao;
	if (!ao || qmPackets.isEmpty())
		return;

	double cmp = qtTicker.elapsed();

	QMultiMap<float, Packet>::iterator i = qmPackets.begin();

	while (i != qmPackets.end()) {
		if (i.key() > cmp)
			break;
		ao->addFrameToBuffer(this, i.value().second, i.value().first);
		i = qmPackets.erase(i);
	}

	qtLastFetch.restart();
}

void Audio::startOutput(const QString &output) {
	g.ao = AudioOutputRegistrar::newFromChoice(output);
	if (g.ao)
		g.ao->start(QThread::HighPriority);
}

void Audio::stopOutput() {
	AudioOutputPtr ao = g.ao;

	g.ao.reset();

	while (ao.get() && ! ao.unique()) {
#if QT_VERSION >= 0x040500
		QThread::yieldCurrentThread();
#endif
	}

	ao.reset();
}

void Audio::startInput(const QString &input) {
	g.ai = AudioInputRegistrar::newFromChoice(input);
	if (g.ai)
		g.ai->start(QThread::HighestPriority);
}

void Audio::stopInput() {
	AudioInputPtr ai = g.ai;

	g.ai.reset();

	while (ai.get() && ! ai.unique()) {
#if QT_VERSION >= 0x040500
		QThread::yieldCurrentThread();
#endif
	}

	ai.reset();
}

void Audio::start(const QString &input, const QString &output) {
	startInput(input);
	startOutput(output);
}

void Audio::stop() {
	AudioInputPtr ai = g.ai;
	AudioOutputPtr ao = g.ao;

	g.ao.reset();
	g.ai.reset();

	while ((ai.get() && ! ai.unique()) || (ao.get() && ! ao.unique())) {
#if QT_VERSION >= 0x040500
		QThread::yieldCurrentThread();
#endif
	}

	ai.reset();
	ao.reset();
}
