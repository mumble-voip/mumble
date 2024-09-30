// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_PULSEAUDIO_H_
#define MUMBLE_MUMBLE_PULSEAUDIO_H_

#include "AudioInput.h"
#include "AudioOutput.h"

#include <QtCore/QLibrary>
#include <QtCore/QWaitCondition>

#include <pulse/channelmap.h>
#include <pulse/context.h>
#include <pulse/def.h>
#include <pulse/ext-stream-restore.h>
#include <pulse/introspect.h>
#include <pulse/mainloop-api.h>
#include <pulse/sample.h>
#include <pulse/stream.h>
#include <pulse/subscribe.h>
#include <pulse/thread-mainloop.h>
#include <pulse/volume.h>

#include <condition_variable>
#include <mutex>

struct PulseAttenuation {
	uint32_t index;
	QString name;
	QString stream_restore_id;
	pa_cvolume normal_volume;
	pa_cvolume attenuated_volume;
};

class PulseAudio : public QObject {
private:
	Q_OBJECT
	Q_DISABLE_COPY(PulseAudio)
protected:
	QLibrary m_lib;

public:
	bool m_ok;

	const char *(*get_library_version)();
	const char *(*strerror)(int error);

	void (*operation_unref)(pa_operation *o);

	int (*cvolume_equal)(const pa_cvolume *a, const pa_cvolume *b);
	pa_cvolume *(*sw_cvolume_multiply_scalar)(pa_cvolume *dest, const pa_cvolume *a, pa_volume_t b);

	int (*sample_spec_equal)(const pa_sample_spec *a, const pa_sample_spec *b);
	int (*channel_map_equal)(const pa_channel_map *a, const pa_channel_map *b);

	pa_proplist *(*proplist_new)();
	void (*proplist_free)(pa_proplist *p);
	const char *(*proplist_gets)(const pa_proplist *p, const char *key);
	int (*proplist_sets)(pa_proplist *p, const char *key, const char *value);

	pa_threaded_mainloop *(*threaded_mainloop_new)();
	void (*threaded_mainloop_free)(pa_threaded_mainloop *m);
	int (*threaded_mainloop_start)(pa_threaded_mainloop *m);
	void (*threaded_mainloop_stop)(pa_threaded_mainloop *m);
	void (*threaded_mainloop_lock)(pa_threaded_mainloop *m);
	void (*threaded_mainloop_unlock)(pa_threaded_mainloop *m);
	pa_mainloop_api *(*threaded_mainloop_get_api)(pa_threaded_mainloop *m);

	int (*context_errno)(const pa_context *c);
	pa_context *(*context_new_with_proplist)(pa_mainloop_api *mainloop, const char *name, const pa_proplist *proplist);
	void (*context_unref)(pa_context *c);
	int (*context_connect)(pa_context *c, const char *server, pa_context_flags_t flags, const pa_spawn_api *api);
	void (*context_disconnect)(pa_context *c);
	pa_operation *(*context_subscribe)(pa_context *c, pa_subscription_mask_t m, pa_context_success_cb_t cb,
									   void *userdata);
	pa_context_state_t (*context_get_state)(const pa_context *c);
	pa_operation *(*context_get_server_info)(pa_context *c, pa_server_info_cb_t cb, void *userdata);
	pa_operation *(*context_get_sink_info_by_name)(pa_context *c, const char *name, pa_sink_info_cb_t cb,
												   void *userdata);
	pa_operation *(*context_get_sink_info_list)(pa_context *c, pa_sink_info_cb_t cb, void *userdata);
	pa_operation *(*context_get_sink_input_info_list)(pa_context *c, pa_sink_input_info_cb_t cb, void *userdata);
	pa_operation *(*context_get_source_info_list)(pa_context *c, pa_source_info_cb_t cb, void *userdata);
	pa_operation *(*context_set_sink_input_volume)(pa_context *c, uint32_t idx, const pa_cvolume *volume,
												   pa_context_success_cb_t cb, void *userdata);
	void (*context_set_state_callback)(pa_context *c, pa_context_notify_cb_t cb, void *userdata);
	void (*context_set_subscribe_callback)(pa_context *c, pa_context_subscribe_cb_t cb, void *userdata);

	pa_stream *(*stream_new)(pa_context *c, const char *name, const pa_sample_spec *ss, const pa_channel_map *map);
	void (*stream_unref)(pa_stream *s);
	int (*stream_connect_playback)(pa_stream *s, const char *dev, const pa_buffer_attr *attr, pa_stream_flags_t flags,
								   const pa_cvolume *volume, pa_stream *sync_stream);
	int (*stream_connect_record)(pa_stream *s, const char *dev, const pa_buffer_attr *attr, pa_stream_flags_t flags);
	int (*stream_disconnect)(pa_stream *s);
	int (*stream_peek)(pa_stream *p, const void **data, size_t *nbytes);
	int (*stream_write)(pa_stream *p, const void *data, size_t nbytes, pa_free_cb_t free_cb, int64_t offset,
						pa_seek_mode_t seek);
	int (*stream_drop)(pa_stream *p);
	pa_operation *(*stream_cork)(pa_stream *s, int b, pa_stream_success_cb_t cb, void *userdata);
	pa_stream_state_t (*stream_get_state)(const pa_stream *p);
	pa_context *(*stream_get_context)(const pa_stream *p);
	const pa_sample_spec *(*stream_get_sample_spec)(pa_stream *s);
	const pa_channel_map *(*stream_get_channel_map)(pa_stream *s);
	const pa_buffer_attr *(*stream_get_buffer_attr)(pa_stream *s);
	void (*stream_set_state_callback)(pa_stream *s, pa_stream_notify_cb_t cb, void *userdata);
	void (*stream_set_read_callback)(pa_stream *p, pa_stream_request_cb_t cb, void *userdata);
	void (*stream_set_write_callback)(pa_stream *p, pa_stream_request_cb_t cb, void *userdata);

	pa_operation *(*ext_stream_restore_read)(pa_context *c, pa_ext_stream_restore_read_cb_t cb, void *userdata);
	pa_operation *(*ext_stream_restore_write)(pa_context *c, pa_update_mode_t mode,
											  const pa_ext_stream_restore_info data[], unsigned n,
											  int apply_immediately, pa_context_success_cb_t cb, void *userdata);

public:
	PulseAudio();
};

class PulseAudioSystem : public QObject {
private:
	Q_OBJECT
	Q_DISABLE_COPY(PulseAudioSystem)
protected:
	bool m_initialized = false;
	std::mutex m_initLock;
	std::condition_variable m_initWaiter;

	void wakeup();

	PulseAudio m_pulseAudio;
	pa_context *pacContext;
	pa_stream *pasInput, *pasOutput, *pasSpeaker;
	pa_threaded_mainloop *pam;
	pa_defer_event *pade;

	bool bSourceDone, bSinkDone, bServerDone, bRunning;

	QString qsDefaultInput, qsDefaultOutput;

	int iDelayCache;
	QString qsOutputCache, qsInputCache, qsEchoCache;
	bool bEchoMultiCache;
	QHash< QString, QString > qhEchoMap;
	QHash< QString, pa_sample_spec > qhSpecMap;
	QHash< QString, pa_channel_map > qhChanMap;

	bool bAttenuating;
	int iRemainingOperations;
	int iSinkId;
	QHash< uint32_t, PulseAttenuation > qhVolumes;
	QList< uint32_t > qlMatchedSinks;
	QHash< QString, PulseAttenuation > qhUnmatchedSinks;
	QHash< QString, PulseAttenuation > qhMissingSinks;

	static void defer_event_callback(pa_mainloop_api *a, pa_defer_event *e, void *userdata);
	static void context_state_callback(pa_context *c, void *userdata);
	static void subscribe_callback(pa_context *c, pa_subscription_event_type_t t, uint32_t idx, void *userdata);
	static void sink_callback(pa_context *c, const pa_sink_info *i, int eol, void *userdata);
	static void source_callback(pa_context *c, const pa_source_info *i, int eol, void *userdata);
	static void server_callback(pa_context *c, const pa_server_info *i, void *userdata);
	static void sink_info_callback(pa_context *c, const pa_sink_info *i, int eol, void *userdata);
	static void write_stream_callback(pa_stream *s, void *userdata);
	static void read_stream_callback(pa_stream *s, void *userdata);
	static void read_callback(pa_stream *s, size_t bytes, void *userdata);
	static void write_callback(pa_stream *s, size_t bytes, void *userdata);
	static void volume_sink_input_list_callback(pa_context *c, const pa_sink_input_info *i, int eol, void *userdata);
	static void restore_sink_input_list_callback(pa_context *c, const pa_sink_input_info *i, int eol, void *userdata);
	static void stream_restore_read_callback(pa_context *c, const pa_ext_stream_restore_info *i, int eol,
											 void *userdata);
	static void restore_volume_success_callback(pa_context *c, int success, void *userdata);
	void contextCallback(pa_context *c);
	void eventCallback(pa_mainloop_api *a, pa_defer_event *e);

	void query();

	QString outputDevice() const;
	QString inputDevice() const;

	void setVolumes();
	PulseAttenuation *getAttenuation(QString stream_restore_id);

public:
	QHash< QString, QString > qhInput;
	QHash< QString, QString > qhOutput;
	bool bPulseIsGood;

	void wakeup_lock();

	PulseAudioSystem();
	~PulseAudioSystem() Q_DECL_OVERRIDE;
};

class PulseAudioInput : public AudioInput {
	friend class PulseAudioSystem;

private:
	Q_OBJECT
	Q_DISABLE_COPY(PulseAudioInput)
protected:
	QMutex qmMutex;
	QWaitCondition qwcWait;
	pa_sample_spec pssMic, pssEcho;

public:
	PulseAudioInput();
	~PulseAudioInput() Q_DECL_OVERRIDE;
	void run() Q_DECL_OVERRIDE;
};

class PulseAudioOutput : public AudioOutput {
	friend class PulseAudioSystem;

private:
	Q_OBJECT
	Q_DISABLE_COPY(PulseAudioOutput)
protected:
	QMutex qmMutex;
	QWaitCondition qwcWait;
	pa_sample_spec pss;
	pa_channel_map pcm;

public:
	PulseAudioOutput();
	~PulseAudioOutput() Q_DECL_OVERRIDE;
	void run() Q_DECL_OVERRIDE;
};

#endif
