/* Copyright (C) 2005-2011, Thorvald Natvig <thorvald@natvig.com>

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

#include "mumble_pch.hpp"

#include "Audio.h"

#include "AudioInput.h"
#include "AudioOutput.h"
#include "CELTCodec.h"
#include "Global.h"
#include "PacketDataStream.h"

class CodecInit : public DeferInit {
	public:
		void initialize();
		void destroy();
};

#define DOUBLE_RAND (rand()/static_cast<double>(RAND_MAX))

LoopUser LoopUser::lpLoopy;
CodecInit ciInit;

void CodecInit::initialize() {
	CELTCodec *codec = NULL;

	if (g.s.bDisableCELT) {
		// Kill switch for CELT activated. Do not initialize it.
		return;
	}

#ifdef USE_SBCELT
	codec = new CELTCodecSBCELT();
	if (codec->isValid()) {
		codec->report();
		g.qmCodecs.insert(codec->bitstreamVersion(), codec);
	} else {
		delete codec;
	}
#else
	codec = new CELTCodec070(QLatin1String("0.7.0"));
	if (codec->isValid()) {
		codec->report();
		g.qmCodecs.insert(codec->bitstreamVersion(), codec);
	} else {
		delete codec;
		codec = new CELTCodec070(QLatin1String("0.0.0"));
		if (codec->isValid()) {
			codec->report();
			g.qmCodecs.insert(codec->bitstreamVersion(), codec);
		} else {
			delete codec;
		}
	}

	codec = new CELTCodec011(QLatin1String("0.11.0"));
	if (codec->isValid()) {
		codec->report();
		g.qmCodecs.insert(codec->bitstreamVersion(), codec);
	} else {
		delete codec;
		codec = new CELTCodec011(QLatin1String("2.0.0"));
		if (codec->isValid()) {
			codec->report();
			g.qmCodecs.insert(codec->bitstreamVersion(), codec);
		} else {
			delete codec;
		}
	}
#endif
}

void CodecInit::destroy() {
	foreach(CELTCodec *codec, g.qmCodecs)
		delete codec;
	g.qmCodecs.clear();
}

LoopUser::LoopUser() {
	qsName = QLatin1String("Loopy");
	uiSession = 0;
	iId = 0;
	bMute = bDeaf = bSuppress = false;
	bLocalIgnore = bLocalMute = bSelfDeaf = false;
	tsState = Settings::Passive;
	cChannel = NULL;
	qtTicker.start();
}

void LoopUser::addFrame(const QByteArray &packet) {
	if (DOUBLE_RAND < g.s.dPacketLoss) {
		qWarning("Drop");
		return;
	}

	{
		QMutexLocker l(&qmLock);
		bool restart = (qtLastFetch.elapsed() > 100);

		double time = qtTicker.elapsed();

		double r;
		if (restart)
			r = 0.0;
		else
			r = DOUBLE_RAND * g.s.dMaxPacketDelay;

		qmPackets.insert(static_cast<float>(time + r), packet);
	}

	// Restart check
	if (qtLastFetch.elapsed() > 100) {
		AudioOutputPtr ao = g.ao;
		if (ao) {
			MessageHandler::UDPMessageType msgType = static_cast<MessageHandler::UDPMessageType>((packet.at(0) >> 5) & 0x7);
			ao->addFrameToBuffer(this, QByteArray(), 0, msgType);
		}
	}

}

void LoopUser::fetchFrames() {
	QMutexLocker l(&qmLock);

	AudioOutputPtr ao(g.ao);
	if (!ao || qmPackets.isEmpty()) {
		return;
	}

	double cmp = qtTicker.elapsed();

	QMultiMap<float, QByteArray>::iterator i = qmPackets.begin();

	while (i != qmPackets.end()) {
		if (i.key() > cmp)
			break;

		int iSeq;
		const QByteArray &data = i.value();
		PacketDataStream pds(data.constData(), data.size());

		unsigned int msgFlags = static_cast<unsigned int>(pds.next());

		pds >> iSeq;

		QByteArray qba;
		qba.reserve(pds.left() + 1);
		qba.append(static_cast<char>(msgFlags));
		qba.append(pds.dataBlock(pds.left()));

		MessageHandler::UDPMessageType msgType = static_cast<MessageHandler::UDPMessageType>((msgFlags >> 5) & 0x7);

		ao->addFrameToBuffer(this, qba, iSeq, msgType);
		i = qmPackets.erase(i);
	}

	qtLastFetch.restart();
}

RecordUser::RecordUser() : LoopUser() {
	qsName = QLatin1String("Recorder");
}

RecordUser::~RecordUser() {
	AudioOutputPtr ao = g.ao;
	if (ao)
		ao->removeBuffer(this);
}

void RecordUser::addFrame(const QByteArray &packet) {
	AudioOutputPtr ao(g.ao);
	if (!ao)
		return;

	int iSeq;
	PacketDataStream pds(packet.constData(), packet.size());

	unsigned int msgFlags = static_cast<unsigned int>(pds.next());

	pds >> iSeq;

	QByteArray qba;
	qba.reserve(pds.left() + 1);
	qba.append(static_cast<char>(msgFlags));
	qba.append(pds.dataBlock(pds.left()));

	MessageHandler::UDPMessageType msgType = static_cast<MessageHandler::UDPMessageType>((msgFlags >> 5) & 0x7);

	ao->addFrameToBuffer(this, qba, iSeq, msgType);
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
		QThread::yieldCurrentThread();
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
		QThread::yieldCurrentThread();
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
		QThread::yieldCurrentThread();
	}

	ai.reset();
	ao.reset();
}
