/* Copyright (C) 2005-2009, Thorvald Natvig <thorvald@natvig.com>

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
#include "AudioOutput.h"
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

	codec = new CELTCodec061(QLatin1String("0.6.1"));
	if (codec->isValid()) {
		codec->report();
		g.qmCodecs.insert(codec->bitstreamVersion(), codec);
	} else {
		delete codec;
	}

	codec = new CELTCodec061(QLatin1String("0.6.2"));
	if (codec->isValid()) {
		codec->report();
		g.qmCodecs.insert(codec->bitstreamVersion(), codec);
	} else {
		delete codec;
	}

	codec = new CELTCodec070(QLatin1String("0.6.3"));
	if (codec->isValid()) {
		codec->report();
		g.qmCodecs.insert(codec->bitstreamVersion(), codec);
	} else {
		delete codec;
	}
}

void CodecInit::destroy() {
	foreach(CELTCodec *codec, g.qmCodecs)
		delete codec;
	g.qmCodecs.clear();
}


#define RESOLVE(var) { * reinterpret_cast<void **>(&var) = static_cast<void *>(qlCELT.resolve(#var)); bValid = bValid && (var != NULL); }

CELTCodec::CELTCodec(const QString &version) {
	bValid = false;
	cmMode = NULL;
	qlCELT.setLoadHints(QLibrary::ResolveAllSymbolsHint);

	QStringList alternatives;
#if defined(Q_OS_MAC)
	alternatives << QString::fromLatin1("celt.%1.dylib").arg(version);
	alternatives << QString::fromLatin1("libcelt.%1.dylib").arg(version);
#elif defined(Q_OS_UNIX)
	alternatives << QString::fromLatin1("celt.so.%1").arg(version);
	alternatives << QString::fromLatin1("libcelt.so.%1").arg(version);
#else
	alternatives << QString::fromLatin1("celt.%1.dll").arg(version);
	alternatives << QString::fromLatin1("libcelt.%1.dll").arg(version);
#endif
	foreach(const QString &lib, alternatives) {
		qlCELT.setFileName(QApplication::instance()->applicationDirPath() + QLatin1String("/") + lib);
		if (qlCELT.load()) {
			bValid = true;
			break;
		}

#ifdef Q_OS_MAC
		qlCELT.setFileName(QApplication::instance()->applicationDirPath() + QLatin1String("/../Codecs/") + lib);
		if (qlCELT.load()) {
			bValid = true;
			break;
		}
#endif

#ifdef PLUGIN_PATH
		qlCELT.setFileName(QLatin1String(MUMTEXT(PLUGIN_PATH) "/") + lib);
		if (qlCELT.load()) {
			bValid = true;
			break;
		}
#endif

		qlCELT.setFileName(lib);
		if (qlCELT.load()) {
			bValid = true;
			break;
		}
	}

	RESOLVE(celt_mode_destroy);
	RESOLVE(celt_mode_info);

	RESOLVE(celt_encoder_destroy);
	RESOLVE(celt_encode_float);
	RESOLVE(celt_encode);
	RESOLVE(celt_encoder_ctl);

	RESOLVE(celt_decoder_destroy);
	RESOLVE(celt_decode_float);
	RESOLVE(celt_decode);
	RESOLVE(celt_decoder_ctl);
}

CELTCodec::~CELTCodec() {
	if (cmMode)
		celt_mode_destroy(const_cast<CELTMode *>(cmMode));
}

bool CELTCodec::isValid() const {
	return bValid;
}

int CELTCodec::bitstreamVersion() const {
	int v = INT_MIN;

	if (cmMode)
		celt_mode_info(cmMode, CELT_GET_BITSTREAM_VERSION, reinterpret_cast<celt_int32 *>(&v));
	return v;
}

void CELTCodec::report() const {
	qWarning("CELT bitstream %08x from %s", bitstreamVersion(), qPrintable(qlCELT.fileName()));
}

CELTCodec061::CELTCodec061(const QString &version) : CELTCodec(version) {
	RESOLVE(celt_mode_create);

	RESOLVE(celt_encoder_create);

	RESOLVE(celt_decoder_create);

	if (bValid) {
		cmMode = celt_mode_create(SAMPLE_RATE, 1, SAMPLE_RATE / 100, NULL);
	}
}

CELTEncoder *CELTCodec061::encoderCreate() {
	return celt_encoder_create(cmMode);
}

CELTDecoder *CELTCodec061::decoderCreate() {
	return celt_decoder_create(cmMode);
}

CELTCodec070::CELTCodec070(const QString &version) : CELTCodec(version) {
	RESOLVE(celt_mode_create);
	RESOLVE(celt_encoder_create);
	RESOLVE(celt_decoder_create);

	if (bValid) {
		cmMode = celt_mode_create(SAMPLE_RATE, SAMPLE_RATE / 100, NULL);
	}
}

CELTEncoder *CELTCodec070::encoderCreate() {
	return celt_encoder_create(cmMode, 1, NULL);
}

CELTDecoder *CELTCodec070::decoderCreate() {
	return celt_decoder_create(cmMode, 1, NULL);
}

LoopUser::LoopUser() {
	qsName = QLatin1String("Loopy");
	uiSession = 0;
	iId = 0;
	bMute = bDeaf = bSuppress = false;
	bLocalMute = bSelfDeaf = false;
	tsState = ClientUser::TalkingOff;
	cChannel = NULL;
	qtTicker.start();
}

void LoopUser::addFrame(const QByteArray &packet) {
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

	AudioOutputPtr ao = g.ao;
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
