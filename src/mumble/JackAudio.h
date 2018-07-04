// Copyright 2005-2018 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_JACKAUDIO_H_
#define MUMBLE_MUMBLE_JACKAUDIO_H_

#include "AudioInput.h"
#include "AudioOutput.h"

#include <QtCore/QWaitCondition>

#include <jack/jack.h>

#define JACK_MAX_OUTPUT_PORTS 2

class JackAudioOutput;
class JackAudioInput;

class JackAudioSystem : public QObject {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(JackAudioSystem)
	protected:
		bool bActive;
		jack_client_t *client;
		jack_port_t *in_port;
		jack_port_t *out_ports[JACK_MAX_OUTPUT_PORTS];
		jack_default_audio_sample_t *output_buffer;
		jack_nframes_t iBufferSize;

		void allocOutputBuffer(jack_nframes_t frames);

		void auto_connect_ports();
		void disconnect_ports();

		static int process_callback(jack_nframes_t nframes, void *arg);
		static int srate_callback(jack_nframes_t frames, void *arg);
		static int buffer_size_callback(jack_nframes_t frames, void *arg);
		static void shutdown_callback(void *arg);
	public:
		QHash<QString, QString> qhInput;
		QHash<QString, QString> qhOutput;
		bool bJackIsGood;
		bool bInputIsGood;
		bool bOutputIsGood;
		int iSampleRate;
		unsigned int iOutPorts;
		QMutex qmWait;

		void init_jack();
		void close_jack();

		void activate();

		void initializeInput();
		void destroyInput();

		void initializeOutput();
		void destroyOutput();

		JackAudioSystem();
};

class JackAudioInput : public AudioInput {
	friend class JackAudioSystem;
	private:
		Q_OBJECT
		Q_DISABLE_COPY(JackAudioInput)
	protected:
		QMutex qmMutex;
		QWaitCondition qwcWait;
	public:
		JackAudioInput();
		~JackAudioInput() Q_DECL_OVERRIDE;
		void run() Q_DECL_OVERRIDE;
};

class JackAudioOutput : public AudioOutput {
	friend class JackAudioSystem;
	private:
		Q_OBJECT
		Q_DISABLE_COPY(JackAudioOutput)
	protected:
		QMutex qmMutex;
		QWaitCondition qwcWait;
	public:
		JackAudioOutput();
		~JackAudioOutput() Q_DECL_OVERRIDE;
		void run() Q_DECL_OVERRIDE;
};

#endif
