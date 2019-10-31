// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_JACKAUDIO_H_
#define MUMBLE_MUMBLE_JACKAUDIO_H_

#include "AudioInput.h"
#include "AudioOutput.h"

#include <QtCore/QLibrary>
#include <QtCore/QVector>
#include <QtCore/QWaitCondition>

#include <jack/types.h>

#define JACK_MAX_OUTPUT_PORTS 2

typedef QVector<jack_port_t *> JackPorts;

class JackAudioInit;

class JackAudioSystem : public QObject {
		friend JackAudioInit;

	private:
		Q_OBJECT
		Q_DISABLE_COPY(JackAudioSystem)

	protected:
		bool bAvailable;
		uint8_t users;
		QMutex qmWait;
		QLibrary qlJack;
		QWaitCondition qwcWait;
		jack_client_t *client;

		static int processCallback(jack_nframes_t frames, void *);
		static int sampleRateCallback(jack_nframes_t, void *);
		static int bufferSizeCallback(jack_nframes_t frames, void *);
		static void shutdownCallback(void *);

		const char *(*jack_get_version_string)();
		const char **(*jack_get_ports)(jack_client_t *client, const char *port_name_pattern, const char *type_name_pattern, unsigned long flags);
		char *(*jack_get_client_name)(jack_client_t *client);
		char *(*jack_port_name)(jack_port_t *port);
		int (*jack_client_close)(jack_client_t *client);
		int (*jack_activate)(jack_client_t *client);
		int (*jack_deactivate)(jack_client_t *client);
		int (*jack_set_process_callback)(jack_client_t *client, JackProcessCallback process_callback, void *arg);
		int (*jack_set_sample_rate_callback)(jack_client_t *client, JackSampleRateCallback process_callback, void *arg);
		int (*jack_set_buffer_size_callback)(jack_client_t *client, JackBufferSizeCallback process_callback, void *arg);
		int (*jack_on_shutdown)(jack_client_t *client, JackShutdownCallback process_callback, void *arg);
		int (*jack_connect)(jack_client_t *client, const char *source_port, const char *destination_port);
		int (*jack_port_disconnect)(jack_client_t *client, jack_port_t *port);
		int (*jack_port_unregister)(jack_client_t *client, jack_port_t *port);
		int (*jack_port_flags)(const jack_port_t *port);
		void *(*jack_port_get_buffer)(jack_port_t *port, jack_nframes_t frames);
		void (*jack_free)(void *ptr);
		jack_client_t *(*jack_client_open)(const char *client_name, jack_options_t options, jack_status_t *status, ...);
		jack_nframes_t(*jack_get_sample_rate)(jack_client_t *client);
		jack_nframes_t(*jack_get_buffer_size)(jack_client_t *client);
		jack_port_t *(*jack_port_by_name)(jack_client_t *client, const char *port_name);
		jack_port_t *(*jack_port_register)(jack_client_t *client, const char *port_name, const char *port_type, unsigned long flags, unsigned long buffer_size);

	public:
		QHash<QString, QString> qhInput;
		QHash<QString, QString> qhOutput;

		bool isOk();
		uint8_t outPorts();
		jack_nframes_t sampleRate();
		jack_nframes_t bufferSize();
		JackPorts getPhysicalPorts(const uint8_t &flags);
		void *getPortBuffer(jack_port_t *port, const jack_nframes_t &frames);
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
