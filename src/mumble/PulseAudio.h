/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>

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

#ifndef _PULSEAUDIO_H
#define _PULSEAUDIO_H

#include "AudioInput.h"
#include "AudioOutput.h"
#include <pulse/pulseaudio.h>

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

		bool bSourceDone, bSinkDone, bServerDone;

		QString qsDefaultInput, qsDefaultOutput;

		int iDelayCache;
		QString qsOutputCache, qsInputCache, qsEchoCache;
		bool bPositionalCache;
		QHash<QString, QString> qhEchoMap;
		QHash<QString, pa_sample_spec> qhSpecMap;
		QHash<QString, int> qhIndexMap;

		static void defer_event_callback(pa_mainloop_api *a, pa_defer_event *e, void *userdata);
		static void context_state_callback(pa_context *c, void *userdata);
		static void subscribe_callback(pa_context *c, pa_subscription_event_type_t t, uint32_t idx, void *userdata);
		static void sink_callback(pa_context *c, const pa_sink_info *i, int eol, void *userdata);
		static void source_callback(pa_context *c, const pa_source_info *i, int eol, void *userdata);
		static void server_callback(pa_context *c, const pa_server_info *i, void *userdata);
		static void stream_callback(pa_stream *s, void *userdata);
		static void read_callback(pa_stream *s, size_t bytes, void *userdata);
		static void write_callback(pa_stream *s, size_t bytes, void *userdata);
		void contextCallback(pa_context *c);
		void eventCallback(pa_mainloop_api *a, pa_defer_event *e);

		void query();

	public:
		QHash<QString, QString> qhInput;
		QHash<QString, QString> qhOutput;
		bool bPulseIsGood;
		QMutex qmWait;
		QWaitCondition qwcWait;

		void wakeup_lock();

		PulseAudioSystem();
		~PulseAudioSystem();
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
		~PulseAudioInput();
		void run();
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
		~PulseAudioOutput();
		void run();
};

#endif
