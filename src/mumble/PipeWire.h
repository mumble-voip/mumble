// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_PIPEWIRE_H
#define MUMBLE_MUMBLE_PIPEWIRE_H

#include "AudioInput.h"
#include "AudioOutput.h"

#include <QLibrary>
#include <QMutex>
#include <QWaitCondition>

class PipeWireInit;

struct pw_buffer;
struct pw_loop;
struct pw_properties;
struct pw_stream;
struct pw_stream_events;
struct pw_thread_loop;

struct spa_dict;
struct spa_pod;

class PipeWireEngine {
public:
	bool isOk() { return m_ok; };

	bool connect(const uint8_t direction, const uint32_t *channels, const uint8_t nChannels);

	void start();
	void stop();

	pw_buffer *dequeueBuffer();
	void queueBuffer(pw_buffer *buffer);
	void setActive(bool active);

	PipeWireEngine(const char *category, void *param, const std::function< void(void *param) > callback);
	~PipeWireEngine();

protected:
	bool m_ok;
	pw_loop *m_loop;
	pw_stream *m_stream;
	pw_thread_loop *m_thread;
	std::unique_ptr< pw_stream_events > m_events;

private:
	Q_DISABLE_COPY(PipeWireEngine)
};

class PipeWireSystem : public QObject {
	friend PipeWireEngine;
	friend PipeWireInit;

public:
	bool isOk() { return m_ok; };

	PipeWireSystem();
	~PipeWireSystem();

protected:
	bool m_ok;
	uint8_t m_users;
	QLibrary m_lib;

	const char *(*pw_get_library_version)();

	void (*pw_init)(int *argc, char **argv[]);
	void (*pw_deinit)();

	pw_loop *(*pw_loop_new)(const spa_dict *props);
	void (*pw_loop_destroy)(pw_loop *loop);

	pw_thread_loop *(*pw_thread_loop_new_full)(pw_loop *loop, const char *name, const spa_dict *props);
	void (*pw_thread_loop_destroy)(pw_thread_loop *loop);
	int (*pw_thread_loop_start)(pw_thread_loop *loop);
	int (*pw_thread_loop_stop)(pw_thread_loop *loop);
	void (*pw_thread_loop_lock)(pw_thread_loop *loop);
	void (*pw_thread_loop_unlock)(pw_thread_loop *loop);

	pw_properties *(*pw_properties_new)(const char *key, ...);

	pw_stream *(*pw_stream_new_simple)(pw_loop *loop, const char *name, pw_properties *props,
									   const pw_stream_events *events, void *data);
	int (*pw_stream_set_active)(pw_stream *stream, bool active);
	void (*pw_stream_destroy)(pw_stream *stream);
	int (*pw_stream_connect)(pw_stream *stream, uint32_t direction, uint32_t target_id, uint32_t flags,
							 const spa_pod **params, uint32_t n_params);
	pw_buffer *(*pw_stream_dequeue_buffer)(pw_stream *stream);
	int (*pw_stream_queue_buffer)(pw_stream *stream, pw_buffer *buffer);

private:
	Q_OBJECT
	Q_DISABLE_COPY(PipeWireSystem)
};

class PipeWireInput : public AudioInput {
public:
	void run() override;

	PipeWireInput();
	~PipeWireInput() override;

protected:
	std::unique_ptr< PipeWireEngine > m_engine;

	static void processCallback(void *param);

	void onUserMutedChanged() override;

private:
	Q_OBJECT
	Q_DISABLE_COPY(PipeWireInput)
};

class PipeWireOutput : public AudioOutput {
public:
	void run() override;

	PipeWireOutput();
	~PipeWireOutput() override;

protected:
	std::unique_ptr< PipeWireEngine > m_engine;

	static void processCallback(void *param);

private:
	Q_OBJECT
	Q_DISABLE_COPY(PipeWireOutput)
};

#endif
