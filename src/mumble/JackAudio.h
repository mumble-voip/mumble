// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_JACKAUDIO_H_
#define MUMBLE_MUMBLE_JACKAUDIO_H_

#include "AudioInput.h"
#include "AudioOutput.h"

#include <QtCore/QVector>
#include <QtCore/QWaitCondition>

#include <jack/jack.h>

#define JACK_MAX_OUTPUT_PORTS 2

typedef QVector<jack_port_t *> JackPorts;

class JackAudioInit;

class JackAudioSystem : public QObject {
		friend JackAudioInit;

	private:
		Q_OBJECT
		Q_DISABLE_COPY(JackAudioSystem)

	protected:
		uint8_t users;
		QMutex qmWait;
		QWaitCondition qwcWait;
		jack_client_t *client;

		static int processCallback(jack_nframes_t frames, void *);
		static int sampleRateCallback(jack_nframes_t, void *);
		static int bufferSizeCallback(jack_nframes_t frames, void *);
		static void shutdownCallback(void *);

	public:
		QHash<QString, QString> qhInput;
		QHash<QString, QString> qhOutput;

		bool isOk();
		uint8_t outPorts();
		jack_nframes_t sampleRate();
		jack_nframes_t bufferSize();
		JackPorts getPhysicalPorts(const uint8_t &flags);
		jack_port_t *registerPort(const char *name, const uint8_t &flags);
		bool unregisterPort(jack_port_t *port);
		bool connectPort(jack_port_t *sourcePort, jack_port_t *destinationPort);
		bool disconnectPort(jack_port_t *port);

		bool initialize();
		void deinitialize();
		bool activate();
		void deactivate();

		JackAudioSystem();
		~JackAudioSystem();
};

class JackAudioInput : public AudioInput {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(JackAudioInput)

	protected:
		bool bReady;
		QMutex qmWait;
		QWaitCondition qwcSleep;
		jack_port_t *port;

	public:
		bool isReady();
		bool process(const jack_nframes_t &frames);
		bool activate();
		void deactivate();
		bool registerPorts();
		bool unregisterPorts();
		void connectPorts();
		bool disconnectPorts();

		void run() Q_DECL_OVERRIDE;
		JackAudioInput();
		~JackAudioInput() Q_DECL_OVERRIDE;
};

class JackAudioOutput : public AudioOutput {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(JackAudioOutput)

	protected:
		bool bReady;
		QMutex qmWait;
		QWaitCondition qwcSleep;
		JackPorts ports;
		std::unique_ptr<jack_default_audio_sample_t[]> buffer;

	public:
		bool isReady();
		bool process(const jack_nframes_t &frames);
		void allocBuffer(const jack_nframes_t &frames);
		bool activate();
		void deactivate();
		bool registerPorts();
		bool unregisterPorts();
		void connectPorts();
		bool disconnectPorts();

		void run() Q_DECL_OVERRIDE;
		JackAudioOutput();
		~JackAudioOutput() Q_DECL_OVERRIDE;
};

#endif
