// Copyright 2007-2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Audio.h"

#include "AudioInput.h"
#include "AudioOutput.h"
#include "CELTCodec.h"
#ifdef USE_OPUS
#	include "OpusCodec.h"
#endif
#include "Log.h"
#include "PacketDataStream.h"
#include "PluginManager.h"
#include "Global.h"

#include <QtCore/QObject>


class CodecInit : public DeferInit {
public:
	void initialize();
	void destroy();
};

#define DOUBLE_RAND (rand() / static_cast< double >(RAND_MAX))

LoopUser LoopUser::lpLoopy;
CodecInit ciInit;

void CodecInit::initialize() {
#ifdef USE_OPUS
	OpusCodec *oCodec = new OpusCodec();
	if (oCodec->isValid()) {
		oCodec->report();
		Global::get().oCodec = oCodec;
	} else {
		Log::logOrDefer(
			Log::CriticalError,
			QObject::tr("CodecInit: Failed to load Opus, it will not be available for encoding/decoding audio."));
		delete oCodec;
	}
#endif

	if (Global::get().s.bDisableCELT) {
		// Kill switch for CELT activated. Do not initialize it.
		return;
	}

	CELTCodec *codec = nullptr;

#ifdef USE_SBCELT
	codec = new CELTCodecSBCELT();
	if (codec->isValid()) {
		codec->report();
		Global::get().qmCodecs.insert(codec->bitstreamVersion(), codec);
	} else {
		delete codec;
	}
#else
	codec = new CELTCodec070(QLatin1String("0.7.0"));
	if (codec->isValid()) {
		codec->report();
		Global::get().qmCodecs.insert(codec->bitstreamVersion(), codec);
	} else {
		delete codec;
		codec = new CELTCodec070(QLatin1String("0.0.0"));
		if (codec->isValid()) {
			codec->report();
			Global::get().qmCodecs.insert(codec->bitstreamVersion(), codec);
		} else {
			delete codec;
		}
	}
#endif
}

void CodecInit::destroy() {
#ifdef USE_OPUS
	delete Global::get().oCodec;
#endif

	foreach (CELTCodec *codec, Global::get().qmCodecs)
		delete codec;
	Global::get().qmCodecs.clear();
}

LoopUser::LoopUser() {
	qsName    = QLatin1String("Loopy");
	uiSession = 0;
	iId       = 0;
	bMute = bDeaf = bSuppress = false;
	bLocalIgnore = bLocalMute = bSelfDeaf = false;
	tsState                               = Settings::Passive;
	cChannel                              = nullptr;
	qetTicker.start();
	qetLastFetch.start();
}

void LoopUser::addFrame(const QByteArray &packet) {
	if (DOUBLE_RAND < Global::get().s.dPacketLoss) {
		qWarning("Drop");
		return;
	}

	{
		QMutexLocker l(&qmLock);
		bool restart = (qetLastFetch.elapsed() > 100);

		double time = qetTicker.elapsed();

		double r;
		if (restart)
			r = 0.0;
		else
			r = DOUBLE_RAND * Global::get().s.dMaxPacketDelay;

		qmPackets.insert(static_cast< float >(time + r), packet);
	}

	// Restart check
	if (qetLastFetch.elapsed() > 100) {
		AudioOutputPtr ao = Global::get().ao;
		if (ao) {
			MessageHandler::UDPMessageType msgType =
				static_cast< MessageHandler::UDPMessageType >((packet.at(0) >> 5) & 0x7);
			ao->addFrameToBuffer(this, QByteArray(), 0, msgType);
		}
	}
}

void LoopUser::fetchFrames() {
	QMutexLocker l(&qmLock);

	AudioOutputPtr ao(Global::get().ao);
	if (!ao || qmPackets.isEmpty()) {
		return;
	}

	double cmp = qetTicker.elapsed();

	QMultiMap< float, QByteArray >::iterator i = qmPackets.begin();

	while (i != qmPackets.end()) {
		if (i.key() > cmp)
			break;

		int iSeq;
		const QByteArray &data = i.value();
		PacketDataStream pds(data.constData(), data.size());

		unsigned int msgFlags = static_cast< unsigned int >(pds.next());

		pds >> iSeq;

		QByteArray qba;
		qba.reserve(pds.left() + 1);
		qba.append(static_cast< char >(msgFlags));
		qba.append(pds.dataBlock(pds.left()));

		MessageHandler::UDPMessageType msgType = static_cast< MessageHandler::UDPMessageType >((msgFlags >> 5) & 0x7);

		ao->addFrameToBuffer(this, qba, iSeq, msgType);
		i = qmPackets.erase(i);
	}

	qetLastFetch.restart();
}

RecordUser::RecordUser() : LoopUser() {
	qsName = QLatin1String("Recorder");
}

RecordUser::~RecordUser() {
	AudioOutputPtr ao = Global::get().ao;
	if (ao)
		ao->removeBuffer(this);
}

void RecordUser::addFrame(const QByteArray &packet) {
	AudioOutputPtr ao(Global::get().ao);
	if (!ao)
		return;

	int iSeq;
	PacketDataStream pds(packet.constData(), packet.size());

	unsigned int msgFlags = static_cast< unsigned int >(pds.next());

	pds >> iSeq;

	QByteArray qba;
	qba.reserve(pds.left() + 1);
	qba.append(static_cast< char >(msgFlags));
	qba.append(pds.dataBlock(pds.left()));

	MessageHandler::UDPMessageType msgType = static_cast< MessageHandler::UDPMessageType >((msgFlags >> 5) & 0x7);

	ao->addFrameToBuffer(this, qba, iSeq, msgType);
}

void Audio::startOutput(const QString &output) {
	Global::get().ao = AudioOutputRegistrar::newFromChoice(output);
	if (Global::get().ao)
		Global::get().ao->start(QThread::HighPriority);
}

void Audio::stopOutput() {
	// Take a copy of the global AudioOutput shared pointer
	// to keep a reference around.
	AudioOutputPtr ao = Global::get().ao;

	// Reset the global AudioOutput shared pointer to the null pointer.
	Global::get().ao.reset();

	// Wait until our copy of the AudioOutput shared pointer (ao)
	// is the only one left.
	while (ao.get() && !ao.unique()) {
		QThread::yieldCurrentThread();
	}

	// Reset our copy of the AudioOutput shared pointer.
	// This causes the AudioOutput destructor to be called
	// right here in this function, on the main thread.
	// Our audio backends expect this to happen.
	//
	// One such example is PulseAudioInput, whose destructor
	// takes the PulseAudio mainloop lock. If the destructor
	// is called inside one of the PulseAudio callbacks that
	// take copies of Global::get().ai, the destructor will try to also
	// take the mainloop lock, causing an abort().
	ao.reset();
}

void Audio::startInput(const QString &input) {
	Global::get().ai = AudioInputRegistrar::newFromChoice(input);
	if (Global::get().ai)
		Global::get().ai->start(QThread::HighestPriority);
}

void Audio::stopInput() {
	// Take a copy of the global AudioInput shared pointer
	// to keep a reference around.
	AudioInputPtr ai = Global::get().ai;

	// Reset the global AudioInput shared pointer to the null pointer.
	Global::get().ai.reset();

	// Wait until our copy of the AudioInput shared pointer (ai)
	// is the only one left.
	while (ai.get() && !ai.unique()) {
		QThread::yieldCurrentThread();
	}

	// Reset our copy of the AudioInput shared pointer.
	// This causes the AudioInput destructor to be called
	// right here in this function, on the main thread.
	// Our audio backends expect this to happen.
	//
	// One such example is PulseAudioInput, whose destructor
	// takes the PulseAudio mainloop lock. If the destructor
	// is called inside one of the PulseAudio callbacks that
	// take copies of Global::get().ai, the destructor will try to also
	// take the mainloop lock, causing an abort().
	ai.reset();
}

void Audio::start(const QString &input, const QString &output) {
	startInput(input);
	startOutput(output);

	// Now that the audio input and output is created, we connect them to the PluginManager
	// As these callbacks might want to change the audio before it gets further processed, all these connections have to
	// be direct
	QObject::connect(Global::get().ai.get(), &AudioInput::audioInputEncountered, Global::get().pluginManager,
					 &PluginManager::on_audioInput, Qt::DirectConnection);
	QObject::connect(Global::get().ao.get(), &AudioOutput::audioSourceFetched, Global::get().pluginManager,
					 &PluginManager::on_audioSourceFetched, Qt::DirectConnection);
	QObject::connect(Global::get().ao.get(), &AudioOutput::audioOutputAboutToPlay, Global::get().pluginManager,
					 &PluginManager::on_audioOutputAboutToPlay, Qt::DirectConnection);
}

void Audio::stop() {
	// Take copies of the global AudioInput and AudioOutput
	// shared pointers to keep a reference to each of them
	// around.
	AudioInputPtr ai  = Global::get().ai;
	AudioOutputPtr ao = Global::get().ao;

	// Reset the global AudioInput and AudioOutput shared pointers
	// to the null pointer.
	Global::get().ao.reset();
	Global::get().ai.reset();

	// Wait until our copies of the AudioInput and AudioOutput shared pointers
	// (ai and ao) are the only ones left.
	while ((ai.get() && !ai.unique()) || (ao.get() && !ao.unique())) {
		QThread::yieldCurrentThread();
	}

	// Reset our copies of the AudioInput and AudioOutput
	// shared pointers.
	// This causes the AudioInput and AudioOutput destructors
	// to be called right here in this function, on the main
	// thread. Our audio backends expect this to happen.
	//
	// One such example is PulseAudioInput, whose destructor
	// takes the PulseAudio mainloop lock. If the destructor
	// is called inside one of the PulseAudio callbacks that
	// take copies of Global::get().ai, the destructor will try to also
	// take the mainloop lock, causing an abort().
	ai.reset();
	ao.reset();
}

#undef DOUBLE_RAND
