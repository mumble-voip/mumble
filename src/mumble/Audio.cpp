// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Audio.h"

#include "AudioInput.h"
#include "AudioOutput.h"
#include "Log.h"
#include "PacketDataStream.h"
#include "PluginManager.h"
#include "Global.h"

#include <QtCore/QObject>

#include <cstring>


#define DOUBLE_RAND (rand() / static_cast< double >(RAND_MAX))

LoopUser LoopUser::lpLoopy;

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

void LoopUser::addFrame(const Mumble::Protocol::AudioData &audioData) {
	if (DOUBLE_RAND < Global::get().s.dPacketLoss) {
		qWarning("Drop");
		return;
	}

	{
		QMutexLocker l(&qmLock);
		bool restart = (qetLastFetch.elapsed() > 100);

		qint64 time = qetTicker.elapsed();

		float r;
		if (restart)
			r = 0;
		else
			r = static_cast< float >(DOUBLE_RAND * Global::get().s.dMaxPacketDelay);


		float virtualArrivalTime = static_cast< float >(time) + r;
		// Insert default-constructed AudioPacket object and only then fill its data in-place. This is necessary to
		// avoid any moving around of the payload vector which would mess up our pointers in the AudioData object.
		m_packets[virtualArrivalTime] = AudioPacket{};
		AudioPacket &packet           = m_packets[virtualArrivalTime];

		// copy audio data to packet
		packet.payload.resize(audioData.payload.size());
		std::memcpy(packet.payload.data(), audioData.payload.data(), audioData.payload.size());

		packet.audioData = audioData;
		// The audio data is now stored in the payload vector and thus this is where we should point the used view (we
		// don't own the original buffer and can thus not guarantee what happens with it once this function returns).
		packet.audioData.payload = { packet.payload.data(), packet.payload.size() };
	}

	// Restart check
	if (qetLastFetch.elapsed() > 100) {
		AudioOutputPtr ao = Global::get().ao;
		if (ao) {
			Mumble::Protocol::AudioData empty;
			empty.usedCodec = audioData.usedCodec;
			ao->addFrameToBuffer(this, empty);
		}
	}
}

void LoopUser::fetchFrames() {
	QMutexLocker l(&qmLock);

	AudioOutputPtr ao(Global::get().ao);
	if (!ao || m_packets.empty()) {
		return;
	}

	float cmp = static_cast< float >(qetTicker.elapsed());

	auto it = m_packets.begin();
	while (it != m_packets.end()) {
		if (it->first > cmp) {
			break;
		}

		ao->addFrameToBuffer(this, it->second.audioData);

		it = m_packets.erase(it);
	}

	qetLastFetch.restart();
}

RecordUser::RecordUser() {
	qsName = QLatin1String("Recorder");
}

RecordUser::~RecordUser() {
	AudioOutputPtr ao = Global::get().ao;
	if (ao)
		ao->removeUser(this);
}

void RecordUser::addFrame(const Mumble::Protocol::AudioData &audioData) {
	AudioOutputPtr ao(Global::get().ao);
	if (!ao)
		return;

	ao->addFrameToBuffer(this, audioData);
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
