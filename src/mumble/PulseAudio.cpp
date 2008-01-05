/* Copyright (C) 2005-2008, Thorvald Natvig <thorvald@natvig.com>

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

#include "PulseAudio.h"
#include "Player.h"
#include "Global.h"
#include "MainWindow.h"
#include "Timer.h"
#include <sys/soundcard.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>

static PulseAudioSystem *pasys = NULL;

static const pa_sample_spec ss = { PA_SAMPLE_S16LE, SAMPLE_RATE, 1 };

#define NBLOCKS 8

#define MAX(a,b)        ( (a) > (b) ? (a) : (b) )
#define MIN(a,b)        ( (a) < (b) ? (a) : (b) )

class PulseAudioInputRegistrar : public AudioInputRegistrar {
	public:
		PulseAudioInputRegistrar();
		virtual AudioInput *create();
		virtual const QList<audioDevice> getDeviceChoices();
		virtual void setDeviceChoice(const QVariant &);
};


class PulseAudioOutputRegistrar : public AudioOutputRegistrar {
	public:
		PulseAudioOutputRegistrar();
		virtual AudioOutput *create();
		virtual const QList<audioDevice> getDeviceChoices();
		virtual void setDeviceChoice(const QVariant &);
};

static PulseAudioInputRegistrar airPulseAudio;
static PulseAudioOutputRegistrar aorPulseAudio;

class PulseAudioInit : public DeferInit {
	public:
		void initialize() { pasys = new PulseAudioSystem(); };
		void destroy() { delete pasys; pasys = NULL; };
};

static PulseAudioInit pulseinit;

PulseAudioSystem::PulseAudioSystem() {
	pasInput = pasOutput = pasSpeaker = NULL;
	bSourceDone=bSinkDone=bServerDone = false;
	iDelayCache = 0;

	psInput = psEcho = NULL;
	iInputIdx = iEchoIdx = 0;
	iEchoSeq = 0;

	pam = pa_mainloop_new();
	pa_mainloop_api *api = pa_mainloop_get_api(pam);

	pacContext = pa_context_new(api, "Mumble");
	pa_context_set_state_callback(pacContext, context_state_callback, this);
	pa_context_connect(pacContext, NULL, PA_CONTEXT_NOAUTOSPAWN, NULL);

	pade = api->defer_new(api, defer_event_callback, this);
	api->defer_enable(pade, false);

	int margin = 320;
	jbJitter = jitter_buffer_init(margin);
	jitter_buffer_ctl(jbJitter, JITTER_BUFFER_SET_MARGIN, &margin);
	start(QThread::TimeCriticalPriority);
}

PulseAudioSystem::~PulseAudioSystem() {
	pa_mainloop_quit(pam, 0);
	wait();
	jitter_buffer_destroy(jbJitter);
	pa_context_disconnect(pacContext);
	pa_context_unref(pacContext);
	pa_mainloop_free(pam);
}

void PulseAudioSystem::wakeup() {
	pa_mainloop_api *api = pa_mainloop_get_api(pam);
	api->defer_enable(pade, true);
	pa_mainloop_wakeup(pam);
}

void PulseAudioSystem::run() {
	int rv;
	pa_mainloop_run(pam, &rv);
	if (rv != 0)
		qWarning("PulseAudio: Mainloop quit with retval %d", rv);
}

void PulseAudioSystem::defer_event_callback(pa_mainloop_api *a, pa_defer_event *e, void *userdata) {
	PulseAudioSystem *pas = reinterpret_cast<PulseAudioSystem *>(userdata);
	pas->eventCallback(a, e);
}

void PulseAudioSystem::eventCallback(pa_mainloop_api *api, pa_defer_event *evt) {
	Q_ASSERT(pade == evt);
	api->defer_enable(pade, false);

	if (! bSourceDone || ! bSinkDone || ! bServerDone)
		return;

	AudioInputPtr ai = g.ai;
	AudioOutputPtr ao = g.ao;
	AudioInput *raw_ai = ai.get();
	AudioOutput *raw_ao = ao.get();
	PulseAudioInput *pai = dynamic_cast<PulseAudioInput *>(raw_ai);
	PulseAudioOutput *pao = dynamic_cast<PulseAudioOutput *>(raw_ao);

	if (raw_ao) {
		QString odev = g.s.qsPulseAudioOutput.isEmpty() ? qsDefaultOutput : g.s.qsPulseAudioOutput;
		pa_stream_state ost = pasOutput ? pa_stream_get_state(pasOutput) : PA_STREAM_TERMINATED;
		bool do_stop = false;
		bool do_start = false;

		if (! pao && (ost == PA_STREAM_READY)) {
			do_stop = true;
		} else if (pao) {
			switch (ost) {
				case PA_STREAM_TERMINATED: {
						if (pasOutput)
							pa_stream_unref(pasOutput);
						pasOutput = pa_stream_new(pacContext, "Mumble Speakers", &ss, NULL);
						pa_stream_set_state_callback(pasOutput, stream_callback, this);
						pa_stream_set_write_callback(pasOutput, write_callback, this);
					}
				case PA_STREAM_UNCONNECTED:
					do_start = true;
					break;
				case PA_STREAM_READY: {
						if (g.s.iDXOutputDelay != iDelayCache) {
							do_stop = true;
						} else if (odev != qsOutputCache) {
							do_stop = true;
						}
						break;
					}
				default:
					break;
			}
		}
		if (do_stop) {
			qWarning("PulseAudio: Stopping output");
			pa_stream_disconnect(pasOutput);
		} else if (do_start) {
			qWarning("PulseAudio: Starting output");
			pa_buffer_attr buff;
			buff.maxlength = pao->iFrameSize * NBLOCKS;
			buff.tlength = g.s.iDXOutputDelay * pao->iFrameSize;
			buff.prebuf = buff.tlength;
			buff.minreq = pao->iFrameSize;

			iDelayCache = g.s.iDXOutputDelay;
			qsOutputCache = odev;

			pa_stream_connect_playback(pasOutput, qPrintable(odev), &buff, PA_STREAM_INTERPOLATE_TIMING, NULL, NULL);
		}
	}

	if (raw_ai) {
		QString idev = g.s.qsPulseAudioInput.isEmpty() ? qsDefaultInput : g.s.qsPulseAudioInput;
		pa_stream_state ist = pasInput ? pa_stream_get_state(pasInput) : PA_STREAM_TERMINATED;
		bool do_stop = false;
		bool do_start = false;

		if (! pai && (ist == PA_STREAM_READY)) {
			do_stop = true;
		} else if (pai) {
			switch (ist) {
				case PA_STREAM_TERMINATED: {
						if (pasInput)
							pa_stream_unref(pasInput);

						pasInput = pa_stream_new(pacContext, "Mumble Microphone", &ss, NULL);
						pa_stream_set_state_callback(pasInput, stream_callback, this);
						pa_stream_set_read_callback(pasInput, read_callback, this);

					}
				case PA_STREAM_UNCONNECTED:
					do_start = true;
					break;
				case PA_STREAM_READY: {
						if (idev != qsInputCache) {
							do_stop = true;
						}
						break;
					}
				default:
					break;
			}
		}
		if (do_stop) {
			qWarning("PulseAudio: Stopping input");
			pa_stream_disconnect(pasInput);
		} else if (do_start) {
			qWarning("PulseAudio: Starting input %s",qPrintable(idev));
			pa_buffer_attr buff;
			buff.maxlength = pai->iFrameSize * NBLOCKS;
			buff.fragsize = pai->iFrameSize;

			qsInputCache = idev;

			pa_stream_connect_record(pasInput, qPrintable(idev), &buff, PA_STREAM_INTERPOLATE_TIMING);
		}
	}

	if (raw_ai) {
		QString odev = g.s.qsPulseAudioOutput.isEmpty() ? qsDefaultOutput : g.s.qsPulseAudioOutput;
		QString edev = qhEchoMap.value(odev);
		pa_stream_state est = pasSpeaker ? pa_stream_get_state(pasSpeaker) : PA_STREAM_TERMINATED;
		bool do_stop = false;
		bool do_start = false;

		if ((! pai || ! g.s.bPulseAudioEcho) && (est == PA_STREAM_READY)) {
			do_stop = true;
		} else if (pai && g.s.bPulseAudioEcho) {
			switch (est) {
				case PA_STREAM_TERMINATED: {
						if (pasSpeaker)
							pa_stream_unref(pasSpeaker);

						pasSpeaker = pa_stream_new(pacContext, "Mumble Speakers (Echo)", &ss, NULL);
						pa_stream_set_state_callback(pasSpeaker, stream_callback, this);
						pa_stream_set_read_callback(pasSpeaker, read_callback, this);

					}
				case PA_STREAM_UNCONNECTED:
					do_start = true;
					break;
				case PA_STREAM_READY: {
						if (edev != qsEchoCache) {
							do_stop = true;
						}
						break;
					}
				default:
					break;
			}
		}
		if (do_stop) {
			qWarning("PulseAudio: Stopping echo");
			pa_stream_disconnect(pasSpeaker);
		} else if (do_start) {
			qWarning("PulseAudio: Starting echo");
			pa_buffer_attr buff;
			buff.maxlength = pai->iFrameSize * NBLOCKS;
			buff.fragsize = pai->iFrameSize;

			qsEchoCache = edev;

			pa_stream_connect_record(pasSpeaker, qPrintable(edev), &buff, PA_STREAM_INTERPOLATE_TIMING);
		}
	}
}

void PulseAudioSystem::context_state_callback(pa_context *c, void *userdata) {
	PulseAudioSystem *pas = reinterpret_cast<PulseAudioSystem *>(userdata);
	pas->contextCallback(c);
}

void PulseAudioSystem::sink_callback(pa_context *, const pa_sink_info *i, int eol, void *userdata) {
	PulseAudioSystem *pas = reinterpret_cast<PulseAudioSystem *>(userdata);
	if (!i || eol) {
		pas->bSinkDone = true;
		pas->wakeup();
		return;
	}

	pas->qhIndexMap.insert(QLatin1String(i->name), i->index);

	pas->qhOutput.insert(QLatin1String(i->name), QLatin1String(i->description));
	pas->qhEchoMap.insert(QLatin1String(i->name), QLatin1String(i->monitor_source_name));
}

void PulseAudioSystem::source_callback(pa_context *, const pa_source_info *i, int eol, void *userdata) {
	PulseAudioSystem *pas = reinterpret_cast<PulseAudioSystem *>(userdata);
	if (!i || eol) {
		pas->bSourceDone = true;
		pas->wakeup();
		return;
	}

	pas->qhIndexMap.insert(QLatin1String(i->name), i->index);

	if (i->monitor_of_sink == PA_INVALID_INDEX)
		pas->qhInput.insert(QLatin1String(i->name), QLatin1String(i->description));
}

void PulseAudioSystem::server_callback(pa_context *, const pa_server_info *i, void *userdata) {
	PulseAudioSystem *pas = reinterpret_cast<PulseAudioSystem *>(userdata);

	pas->qsDefaultInput = QLatin1String(i->default_source_name);
	pas->qsDefaultOutput = QLatin1String(i->default_sink_name);

	pas->bServerDone = true;
	pas->wakeup();
}

void PulseAudioSystem::stream_callback(pa_stream *s, void *userdata) {
	PulseAudioSystem *pas = reinterpret_cast<PulseAudioSystem *>(userdata);
	switch (pa_stream_get_state(s)) {
		case PA_STREAM_FAILED:
			qWarning("PulseAudio: Stream error: %s", pa_strerror(pa_context_errno(pa_stream_get_context(s))));
			break;
		default:
			break;
	}
	pas->wakeup();
}

void PulseAudioSystem::read_callback(pa_stream *s, size_t bytes, void *userdata) {
	PulseAudioSystem *pas = reinterpret_cast<PulseAudioSystem *>(userdata);

	size_t length = bytes;
	const void *data;
	pa_stream_peek(s, &data, &length);
	const short *buffer = reinterpret_cast<const short *>(data);
	int samples = length / 2;
	
	AudioInputPtr ai = g.ai;
	PulseAudioInput *pai = dynamic_cast<PulseAudioInput *>(ai.get());
	if (! pai) {
		pa_stream_drop(s);
		pas->wakeup();
		return;
	}

	// PulseAudio supports setting fragments. But they're only avisory,
	// so we have to copy data like mad.
	if (! pas->psInput)
		pas->psInput = new short[pai->iFrameSize];
	if (! pas->psEcho)
		pas->psEcho = new short[pai->iFrameSize];

	if (s == pas->pasInput) {
		while (samples > 0) {
			int ncopy = MIN(pai->iFrameSize - pas->iInputIdx, samples);
			memcpy(pas->psInput + pas->iInputIdx, buffer, ncopy * sizeof(short));
			pas->iInputIdx += ncopy;
			buffer += ncopy;
			samples -= ncopy;
			if (pas->iInputIdx == pai->iFrameSize) {
				pas->iInputIdx = 0;
				memcpy(pai->psMic, pas->psInput, pai->iFrameSize * sizeof(short));

				if (g.s.bPulseAudioEcho) {
					JitterBufferPacket jbp;
					jbp.data = reinterpret_cast<char *>(pai->psSpeaker);
					jbp.len = pai->iFrameSize * sizeof(short);
					spx_int32_t startofs = 0;
					jitter_buffer_get(pas->jbJitter, &jbp, pai->iFrameSize, &startofs);
					jitter_buffer_update_delay(pas->jbJitter, &jbp, NULL);
					jitter_buffer_tick(pas->jbJitter);
				}
				pai->encodeAudioFrame();
			}
		}
	} else if (s == pas->pasSpeaker) {
		while (samples > 0) {
			int ncopy = MIN(pai->iFrameSize - pas->iEchoIdx, samples);
			memcpy(pas->psEcho + pas->iEchoIdx, buffer, ncopy * sizeof(short));
			pas->iEchoIdx += ncopy;
			buffer += ncopy;
			samples -= ncopy;
			if (pas->iEchoIdx == pai->iFrameSize) {
				pas->iEchoIdx = 0;

				JitterBufferPacket jbp;
				jbp.data = reinterpret_cast<char *>(pas->psEcho);
				jbp.len = pai->iFrameSize * sizeof(short);
				jbp.span = pai->iFrameSize;
				jbp.timestamp = pai->iFrameSize * (++(pas->iEchoSeq));

				jitter_buffer_put(pas->jbJitter, &jbp);
			}
		}
	}

	pa_stream_drop(s);
}

void PulseAudioSystem::write_callback(pa_stream *s, size_t bytes, void *userdata) {
	PulseAudioSystem *pas = reinterpret_cast<PulseAudioSystem *>(userdata);
	Q_ASSERT(s == pas->pasOutput);

	AudioOutputPtr ao = g.ao;
	PulseAudioOutput *pao = dynamic_cast<PulseAudioOutput *>(ao.get());

	int samples = bytes / 2;

	if (! pao) {
		// Transitioning, but most likely transitions back, so just zero.
		short zero[samples];
		memset(zero, 0, samples*sizeof(short));
		pa_stream_write(s, zero, samples * sizeof(short), NULL, 0, PA_SEEK_RELATIVE);
		pas->wakeup();
		return;
	}

	short buffer[pao->iFrameSize] __attribute__((aligned(16)));

	while (samples >= 0) {
		samples -= pao->iFrameSize;
		pao->mixAudio(buffer);
		pa_stream_write(s, buffer, pao->iFrameSize * sizeof(short), NULL, 0, PA_SEEK_RELATIVE);
	}
}

void PulseAudioSystem::query() {
	bSourceDone=bSinkDone=bServerDone = false;
	qhInput.clear();
	qhOutput.clear();
	qhEchoMap.clear();
	qhInput.insert(QString(), tr("Default Input"));
	qhOutput.insert(QString(), tr("Default Output"));
	pa_operation_unref(pa_context_get_server_info(pacContext, server_callback, this));
	pa_operation_unref(pa_context_get_sink_info_list(pacContext, sink_callback, this));
	pa_operation_unref(pa_context_get_source_info_list(pacContext, source_callback, this));
	wakeup();
}

void PulseAudioSystem::contextCallback(pa_context *c) {
	Q_ASSERT(c == pacContext);
	switch (pa_context_get_state(c)) {
		case PA_CONTEXT_READY:
			query();
			break;
		case PA_CONTEXT_TERMINATED:
			qWarning("PulseAudio: Forcibly disconnected from PulseAudio");
			break;
		case PA_CONTEXT_FAILED:
			qWarning("PulseAudio: Connection failure: %s", pa_strerror(pa_context_errno(c)));
			break;
		default:
			break;
	}
}

PulseAudioInputRegistrar::PulseAudioInputRegistrar() : AudioInputRegistrar(QLatin1String("PulseAudio")) {
}

AudioInput *PulseAudioInputRegistrar::create() {
	return new PulseAudioInput();
}

const QList<audioDevice> PulseAudioInputRegistrar::getDeviceChoices() {
	QList<audioDevice> qlReturn;

	QStringList qlInputDevs = pasys->qhInput.keys();
	qSort(qlInputDevs);

	if (qlInputDevs.contains(g.s.qsPulseAudioInput)) {
		qlInputDevs.removeAll(g.s.qsPulseAudioInput);
		qlInputDevs.prepend(g.s.qsPulseAudioInput);
	}

	foreach(const QString &dev, qlInputDevs) {
		qlReturn << audioDevice(pasys->qhInput.value(dev), dev);
	}

	return qlReturn;
}

void PulseAudioInputRegistrar::setDeviceChoice(const QVariant &choice) {
	g.s.qsPulseAudioInput = choice.toString();
}

PulseAudioOutputRegistrar::PulseAudioOutputRegistrar() : AudioOutputRegistrar(QLatin1String("PulseAudio")) {
}

AudioOutput *PulseAudioOutputRegistrar::create() {
	return new PulseAudioOutput();
}

const QList<audioDevice> PulseAudioOutputRegistrar::getDeviceChoices() {
	QList<audioDevice> qlReturn;

	QStringList qlOutputDevs = pasys->qhOutput.keys();
	qSort(qlOutputDevs);

	if (qlOutputDevs.contains(g.s.qsPulseAudioOutput)) {
		qlOutputDevs.removeAll(g.s.qsPulseAudioOutput);
		qlOutputDevs.prepend(g.s.qsPulseAudioOutput);
	}

	foreach(const QString &dev, qlOutputDevs) {
		qlReturn << audioDevice(pasys->qhOutput.value(dev), dev);
	}

	return qlReturn;
}

void PulseAudioOutputRegistrar::setDeviceChoice(const QVariant &choice) {
	g.s.qsPulseAudioOutput = choice.toString();
}

static ConfigWidget *PulseAudioConfigDialogNew(Settings &st) {
	return new PulseAudioConfig(st);
}

static ConfigRegistrar registrar(22, PulseAudioConfigDialogNew);

PulseAudioConfig::PulseAudioConfig(Settings &st) : ConfigWidget(st) {
	setupUi(this);

	QStringList qlOutputDevs = pasys->qhOutput.keys();
	qSort(qlOutputDevs);
	QStringList qlInputDevs = pasys->qhInput.keys();
	qSort(qlInputDevs);

	foreach(QString dev, qlInputDevs) {
		qcbInputDevice->addItem(pasys->qhInput.value(dev), dev);
	}

	foreach(QString dev, qlOutputDevs) {
		qcbOutputDevice->addItem(pasys->qhOutput.value(dev), dev);
	}
}

QString PulseAudioConfig::title() const {
	return tr("PulseAudio");
}

QIcon PulseAudioConfig::icon() const {
	return QIcon(QLatin1String("skin:config_dsound.png"));
}

void PulseAudioConfig::save() const {
	s.iDXOutputDelay = qsOutputDelay->value();
	s.qsPulseAudioInput =  qcbInputDevice->itemData(qcbInputDevice->currentIndex()).toString();
	s.qsPulseAudioOutput =  qcbOutputDevice->itemData(qcbOutputDevice->currentIndex()).toString();
	s.bPulseAudioEcho = qcbEcho->isChecked();
}

void PulseAudioConfig::load(const Settings &r) {
	for (int i=0;i<qcbInputDevice->count();i++) {
		if (qcbInputDevice->itemData(i).toString() == r.qsPulseAudioInput) {
			loadComboBox(qcbInputDevice, i);
			break;
		}
	}

	for (int i=0;i<qcbOutputDevice->count();i++) {
		if (qcbOutputDevice->itemData(i).toString() == r.qsPulseAudioOutput) {
			loadComboBox(qcbOutputDevice, i);
			break;
		}
	}
	loadSlider(qsOutputDelay, r.iDXOutputDelay);
	loadCheckBox(qcbEcho, r.bPulseAudioEcho);
}

bool PulseAudioConfig::expert(bool b) {
	qcbOutput->setVisible(b);
	return true;
}

void PulseAudioConfig::on_qsOutputDelay_valueChanged(int v) {
	qlOutputDelay->setText(tr("%1ms").arg(v*20));
}

PulseAudioInput::PulseAudioInput() {
	bRunning = true;
	bHasSpeaker = g.s.bPulseAudioEcho;
	if (pasys)
		pasys->wakeup();
};

PulseAudioInput::~PulseAudioInput() {
	bRunning = false;
	if (pasys)
		pasys->wakeup();
}

PulseAudioOutput::PulseAudioOutput() {
	bRunning = true;
	if (pasys)
		pasys->wakeup();
}

PulseAudioOutput::~PulseAudioOutput() {
	bRunning = false;
	if (pasys)
		pasys->wakeup();
}
