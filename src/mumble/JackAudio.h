// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_JACKAUDIO_H_
#define MUMBLE_MUMBLE_JACKAUDIO_H_

#include "AudioInput.h"
#include "AudioOutput.h"

#include <QtCore/QLibrary>
#include <QtCore/QSemaphore>
#include <QtCore/QVector>
#include <QtCore/QWaitCondition>

#include <jack/types.h>

#define JACK_MAX_OUTPUT_PORTS 2
#define JACK_BUFFER_PERIODS 3

// Definitions from <jack/ringbuffer.h>
typedef void *jack_ringbuffer_t;

struct jack_ringbuffer_data_t {
	char *buf;
	size_t len;
};

typedef QVector< jack_port_t * > JackPorts;
typedef QVector< jack_default_audio_sample_t * > JackBuffers;

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
	jack_client_t *client;

	static int processCallback(jack_nframes_t frames, void *);
	static int sampleRateCallback(jack_nframes_t, void *);
	static int bufferSizeCallback(jack_nframes_t frames, void *);
	static void shutdownCallback(void *);

	const char *(*jack_get_version_string)();
	const char **(*jack_get_ports)(jack_client_t *client, const char *port_name_pattern, const char *type_name_pattern,
								   unsigned long flags);
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
	jack_nframes_t (*jack_get_sample_rate)(jack_client_t *client);
	jack_nframes_t (*jack_get_buffer_size)(jack_client_t *client);
	jack_port_t *(*jack_port_by_name)(jack_client_t *client, const char *port_name);
	jack_port_t *(*jack_port_register)(jack_client_t *client, const char *port_name, const char *port_type,
									   unsigned long flags, unsigned long buffer_size);

	jack_ringbuffer_t *(*jack_ringbuffer_create)(size_t sz);
	void (*jack_ringbuffer_free)(jack_ringbuffer_t *rb);
	int (*jack_ringbuffer_mlock)(jack_ringbuffer_t *rb);
	size_t (*jack_ringbuffer_read)(jack_ringbuffer_t *rb, char *dest, size_t cnt);
	size_t (*jack_ringbuffer_read_space)(const jack_ringbuffer_t *rb);
	size_t (*jack_ringbuffer_write)(jack_ringbuffer_t *rb, const char *src, size_t cnt);
	void (*jack_ringbuffer_get_write_vector)(const jack_ringbuffer_t *rb, jack_ringbuffer_data_t *vec);
	size_t (*jack_ringbuffer_write_space)(const jack_ringbuffer_t *rb);
	void (*jack_ringbuffer_write_advance)(jack_ringbuffer_t *rb, size_t cnt);

public:
	QHash< QString, QString > qhInput;
	QHash< QString, QString > qhOutput;

	bool isOk();
	uint8_t outPorts();
	jack_nframes_t sampleRate();
	jack_nframes_t bufferSize();
	JackPorts getPhysicalPorts(const uint8_t flags);
	void *getPortBuffer(jack_port_t *port, const jack_nframes_t frames);
	jack_port_t *registerPort(const char *name, const uint8_t flags);
	bool unregisterPort(jack_port_t *port);
	bool connectPort(jack_port_t *sourcePort, jack_port_t *destinationPort);
	bool disconnectPort(jack_port_t *port);

	jack_ringbuffer_t *ringbufferCreate(const size_t size);
	void ringbufferFree(jack_ringbuffer_t *buffer);
	int ringbufferMlock(jack_ringbuffer_t *buffer);
	size_t ringbufferRead(jack_ringbuffer_t *buffer, const size_t size, void *destination);
	size_t ringbufferReadSpace(const jack_ringbuffer_t *buffer);
	size_t ringbufferWrite(jack_ringbuffer_t *buffer, const size_t size, const void *source);
	void ringbufferGetWriteVector(const jack_ringbuffer_t *buffer, jack_ringbuffer_data_t *vector);
	size_t ringbufferWriteSpace(const jack_ringbuffer_t *buffer);
	void ringbufferWriteAdvance(jack_ringbuffer_t *buffer, const size_t size);

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
	volatile bool bReady;
	QMutex qmWait;
	QSemaphore qsSleep;
	jack_port_t *port;
	jack_ringbuffer_t *buffer;
	size_t bufferSize;

public:
	bool isReady();
	bool process(const jack_nframes_t frames);
	bool allocBuffer(const jack_nframes_t frames);
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
	volatile bool bReady;
	QMutex qmWait;
	QSemaphore qsSleep;
	JackPorts ports;
	JackBuffers outputBuffers;
	jack_ringbuffer_t *buffer;

public:
	bool isReady();
	bool process(const jack_nframes_t frames);
	bool allocBuffer(const jack_nframes_t frames);
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
