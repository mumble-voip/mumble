// Copyright 2005-2018 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_PULSEAUDIO_H_
#define MUMBLE_MUMBLE_PULSEAUDIO_H_

#include <pulse/pulseaudio.h>
#include <pulse/ext-stream-restore.h>
#include <QtCore/QWaitCondition>

#include "AudioInput.h"
#include "AudioOutput.h"

struct PulseAttenuation {
	uint32_t index;
	QString name;
	QString stream_restore_id;
	pa_cvolume normal_volume;
	pa_cvolume attenuated_volume;
};

class PulseAudioInput;
class PulseAudioOutput;

class PulseAudioSystem : public QObject {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(PulseAudioSystem)
	protected:
		void wakeup();
		pa_context *pacContext;
		pa_stream *pasInput, *pasOutput, *pasSpeaker;
		pa_threaded_mainloop *pam;
		pa_defer_event *pade;

		bool bSourceDone, bSinkDone, bServerDone, bRunning;

		QString qsDefaultInput, qsDefaultOutput;

		int iDelayCache;
		QString qsOutputCache, qsInputCache, qsEchoCache;
		bool bPositionalCache;
		bool bEchoMultiCache;
		QHash<QString, QString> qhEchoMap;
		QHash<QString, pa_sample_spec> qhSpecMap;
		QHash<QString, pa_channel_map> qhChanMap;

		bool bAttenuating;
		int iRemainingOperations;
		int iSinkId;
		QHash<uint32_t, PulseAttenuation> qhVolumes;
		QList<uint32_t> qlMatchedSinks;
		QHash<QString, PulseAttenuation> qhUnmatchedSinks;
		QHash<QString, PulseAttenuation> qhMissingSinks;

		static void defer_event_callback(pa_mainloop_api *a, pa_defer_event *e, void *userdata);
		static void context_state_callback(pa_context *c, void *userdata);
		static void subscribe_callback(pa_context *c, pa_subscription_event_type_t t, uint32_t idx, void *userdata);
		static void sink_callback(pa_context *c, const pa_sink_info *i, int eol, void *userdata);
		static void source_callback(pa_context *c, const pa_source_info *i, int eol, void *userdata);
		static void server_callback(pa_context *c, const pa_server_info *i, void *userdata);
		static void sink_info_callback(pa_context *c, const pa_sink_info *i, int eol, void *userdata);
		static void stream_callback(pa_stream *s, void *userdata);
		static void read_callback(pa_stream *s, size_t bytes, void *userdata);
		static void write_callback(pa_stream *s, size_t bytes, void *userdata);
		static void volume_sink_input_list_callback(pa_context *c, const pa_sink_input_info *i, int eol, void *userdata);
		static void restore_sink_input_list_callback(pa_context *c, const pa_sink_input_info *i, int eol, void *userdata);
		static void stream_restore_read_callback(pa_context *c, const pa_ext_stream_restore_info *i, int eol, void *userdata);
		static void restore_volume_success_callback(pa_context *c, int success, void *userdata);
		void contextCallback(pa_context *c);
		void eventCallback(pa_mainloop_api *a, pa_defer_event *e);

		void query();

		QString outputDevice() const;
		QString inputDevice() const;

		void setVolumes();
		PulseAttenuation* getAttenuation(QString stream_restore_id);

	public:
		QHash<QString, QString> qhInput;
		QHash<QString, QString> qhOutput;
		bool bPulseIsGood;
		QMutex qmWait;
		QWaitCondition qwcWait;

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
