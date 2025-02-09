// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "PulseAudio.h"

#include "MainWindow.h"
#include "Timer.h"
#include "User.h"
#include "Global.h"

#include <cstdint>
#include <vector>

#ifdef Q_CC_GNU
#	define RESOLVE(var)                                                          \
		{                                                                         \
			var = reinterpret_cast< __typeof__(var) >(m_lib.resolve("pa_" #var)); \
			if (!var)                                                             \
				return;                                                           \
		}
#else
#	define RESOLVE(var)                                                                           \
		{                                                                                          \
			*reinterpret_cast< void ** >(&var) = static_cast< void * >(m_lib.resolve("pa_" #var)); \
			if (!var)                                                                              \
				return;                                                                            \
		}
#endif

static const char *mumble_sink_input = "Mumble Speakers";
static const char *mumble_echo       = "Mumble Speakers (Echo)";

static PulseAudioSystem *pasys = nullptr;

#define NBLOCKS 8

class PulseAudioInputRegistrar : public AudioInputRegistrar {
public:
	PulseAudioInputRegistrar();
	virtual AudioInput *create();
	virtual const QVariant getDeviceChoice();
	virtual const QList< audioDevice > getDeviceChoices();
	virtual void setDeviceChoice(const QVariant &, Settings &);
	virtual bool canEcho(EchoCancelOptionID echoCancelID, const QString &outputSystem) const;
	virtual bool isMicrophoneAccessDeniedByOS() { return false; };
};


class PulseAudioOutputRegistrar : public AudioOutputRegistrar {
public:
	PulseAudioOutputRegistrar();
	virtual AudioOutput *create();
	virtual const QVariant getDeviceChoice();
	virtual const QList< audioDevice > getDeviceChoices();
	virtual void setDeviceChoice(const QVariant &, Settings &);
	bool canMuteOthers() const;
};

class PulseAudioInit : public DeferInit {
public:
	PulseAudioInputRegistrar *airPulseAudio;
	PulseAudioOutputRegistrar *aorPulseAudio;
	void initialize() {
		pasys = new PulseAudioSystem();
		if (pasys->bPulseIsGood) {
			airPulseAudio = new PulseAudioInputRegistrar();
			aorPulseAudio = new PulseAudioOutputRegistrar();
		} else {
			airPulseAudio = nullptr;
			aorPulseAudio = nullptr;
			delete pasys;
			pasys = nullptr;
		}
	};
	void destroy() {
		delete airPulseAudio;
		delete aorPulseAudio;
		delete pasys;
		pasys = nullptr;
	};
};

static PulseAudioInit pulseinit;

PulseAudioSystem::PulseAudioSystem() {
	pasInput = pasOutput = pasSpeaker = nullptr;
	bSourceDone = bSinkDone = bServerDone = false;
	iDelayCache                           = 0;
	bAttenuating                          = false;
	iRemainingOperations                  = 0;
	bPulseIsGood                          = false;
	iSinkId                               = -1;
	bRunning                              = false;

	if (!m_pulseAudio.m_ok) {
		return;
	}

	pam                  = m_pulseAudio.threaded_mainloop_new();
	pa_mainloop_api *api = m_pulseAudio.threaded_mainloop_get_api(pam);

	pa_proplist *proplist = m_pulseAudio.proplist_new();
	m_pulseAudio.proplist_sets(proplist, PA_PROP_APPLICATION_NAME, "Mumble");
	m_pulseAudio.proplist_sets(proplist, PA_PROP_APPLICATION_ID, "net.sourceforge.mumble.mumble");
	m_pulseAudio.proplist_sets(proplist, PA_PROP_APPLICATION_ICON_NAME, "mumble");
	m_pulseAudio.proplist_sets(proplist, PA_PROP_MEDIA_ROLE, "game");

	pacContext = m_pulseAudio.context_new_with_proplist(api, nullptr, proplist);
	m_pulseAudio.proplist_free(proplist);

	m_pulseAudio.context_set_subscribe_callback(pacContext, subscribe_callback, this);

	m_pulseAudio.context_set_state_callback(pacContext, context_state_callback, this);
	m_pulseAudio.context_connect(pacContext, nullptr, PA_CONTEXT_NOAUTOSPAWN, nullptr);

	pade = api->defer_new(api, defer_event_callback, this);
	api->defer_enable(pade, false);

	m_pulseAudio.threaded_mainloop_start(pam);

	bRunning = true;

	std::unique_lock< std::mutex > guard(m_initLock);
	if (!m_initialized) {
		// The mutex is atomically released as soon as this thread starts waiting and will be
		// re-acquired when waking up.
		// Spurious wake-ups are avoided by checking m_initialized in the given predicate
		m_initWaiter.wait(guard, [this]() { return m_initialized; });
	}
}

PulseAudioSystem::~PulseAudioSystem() {
	if (!m_pulseAudio.m_ok) {
		return;
	}

	bRunning = false;

	if (bAttenuating) {
		bAttenuating = false;
		setVolumes();
	}
	m_pulseAudio.threaded_mainloop_stop(pam);
	m_pulseAudio.context_disconnect(pacContext);
	m_pulseAudio.context_unref(pacContext);
	m_pulseAudio.threaded_mainloop_free(pam);
}

QString PulseAudioSystem::outputDevice() const {
	QString odev = Global::get().s.qsPulseAudioOutput;
	if (odev.isEmpty()) {
		odev = qsDefaultOutput;
	}
	if (!qhOutput.contains(odev)) {
		odev = qsDefaultOutput;
	}
	return odev;
}

QString PulseAudioSystem::inputDevice() const {
	QString idev = Global::get().s.qsPulseAudioInput;
	if (idev.isEmpty()) {
		idev = qsDefaultInput;
	}
	if (!qhInput.contains(idev)) {
		idev = qsDefaultInput;
	}
	return idev;
}

void PulseAudioSystem::wakeup() {
	pa_mainloop_api *api = m_pulseAudio.threaded_mainloop_get_api(pam);
	api->defer_enable(pade, true);
}

void PulseAudioSystem::wakeup_lock() {
	m_pulseAudio.threaded_mainloop_lock(pam);
	pa_mainloop_api *api = m_pulseAudio.threaded_mainloop_get_api(pam);
	api->defer_enable(pade, true);
	m_pulseAudio.threaded_mainloop_unlock(pam);
}

void PulseAudioSystem::defer_event_callback(pa_mainloop_api *a, pa_defer_event *e, void *userdata) {
	PulseAudioSystem *pas = reinterpret_cast< PulseAudioSystem * >(userdata);
	pas->eventCallback(a, e);
}

void PulseAudioSystem::eventCallback(pa_mainloop_api *api, pa_defer_event *) {
	api->defer_enable(pade, false);

	if (!bSourceDone || !bSinkDone || !bServerDone)
		return;

	AudioInputPtr ai      = Global::get().ai;
	AudioOutputPtr ao     = Global::get().ao;
	AudioInput *raw_ai    = ai.get();
	AudioOutput *raw_ao   = ao.get();
	PulseAudioInput *pai  = dynamic_cast< PulseAudioInput * >(raw_ai);
	PulseAudioOutput *pao = dynamic_cast< PulseAudioOutput * >(raw_ao);

	if (raw_ao) {
		QString odev        = outputDevice();
		pa_stream_state ost = pasOutput ? m_pulseAudio.stream_get_state(pasOutput) : PA_STREAM_TERMINATED;
		bool do_stop        = false;
		bool do_start       = false;

		if (!pao && (ost == PA_STREAM_READY)) {
			do_stop = true;
		} else if (pao) {
			switch (ost) {
				case PA_STREAM_TERMINATED: {
					if (pasOutput)
						m_pulseAudio.stream_unref(pasOutput);

					pa_sample_spec pss = qhSpecMap.value(odev);
					pa_channel_map pcm = qhChanMap.value(odev);
					if ((pss.format != PA_SAMPLE_FLOAT32NE) && (pss.format != PA_SAMPLE_S16NE))
						pss.format = PA_SAMPLE_FLOAT32NE;
					pss.rate = SAMPLE_RATE;
					if (pss.channels == 0)
						pss.channels = 1;

					pasOutput = m_pulseAudio.stream_new(pacContext, mumble_sink_input, &pss,
														(pss.channels == 1) ? nullptr : &pcm);
					m_pulseAudio.stream_set_state_callback(pasOutput, write_stream_callback, this);
					m_pulseAudio.stream_set_write_callback(pasOutput, write_callback, this);
				}
					// Fallthrough
				case PA_STREAM_UNCONNECTED:
					do_start = true;
					break;
				case PA_STREAM_READY: {
					if (Global::get().s.iOutputDelay != iDelayCache) {
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
			m_pulseAudio.stream_disconnect(pasOutput);
			iSinkId = -1;
		} else if (do_start) {
			qWarning("PulseAudio: Starting output: %s", qPrintable(odev));
			pa_buffer_attr buff;
			const pa_sample_spec *pss    = m_pulseAudio.stream_get_sample_spec(pasOutput);
			const size_t sampleSize      = (pss->format == PA_SAMPLE_FLOAT32NE) ? sizeof(float) : sizeof(short);
			const unsigned int iBlockLen = pao->iFrameSize * pss->channels * static_cast< unsigned int >(sampleSize);
			buff.tlength                 = iBlockLen * static_cast< unsigned int >(Global::get().s.iOutputDelay + 1);
			buff.minreq                  = iBlockLen;
			buff.maxlength               = static_cast< decltype(buff.maxlength) >(-1);
			buff.prebuf                  = static_cast< decltype(buff.prebuf) >(-1);
			buff.fragsize                = iBlockLen;

			iDelayCache   = Global::get().s.iOutputDelay;
			qsOutputCache = odev;

			m_pulseAudio.stream_connect_playback(pasOutput, qPrintable(odev), &buff, PA_STREAM_ADJUST_LATENCY, nullptr,
												 nullptr);
			m_pulseAudio.context_get_sink_info_by_name(pacContext, qPrintable(odev), sink_info_callback, this);
		}
	}

	if (raw_ai) {
		QString idev        = inputDevice();
		pa_stream_state ist = pasInput ? m_pulseAudio.stream_get_state(pasInput) : PA_STREAM_TERMINATED;
		bool do_stop        = false;
		bool do_start       = false;

		if (!pai && (ist == PA_STREAM_READY)) {
			do_stop = true;
		} else if (pai) {
			switch (ist) {
				case PA_STREAM_TERMINATED: {
					if (pasInput)
						m_pulseAudio.stream_unref(pasInput);

					pa_sample_spec pss = qhSpecMap.value(idev);
					if ((pss.format != PA_SAMPLE_FLOAT32NE) && (pss.format != PA_SAMPLE_S16NE))
						pss.format = PA_SAMPLE_FLOAT32NE;
					pss.rate     = SAMPLE_RATE;
					pss.channels = 1;

					pasInput = m_pulseAudio.stream_new(pacContext, "Microphone", &pss, nullptr);
					m_pulseAudio.stream_set_state_callback(pasInput, read_stream_callback, this);
					m_pulseAudio.stream_set_read_callback(pasInput, read_callback, this);
				}
					// Fallthrough
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
			m_pulseAudio.stream_disconnect(pasInput);
		} else if (do_start) {
			qWarning("PulseAudio: Starting input %s", qPrintable(idev));
			pa_buffer_attr buff;
			const pa_sample_spec *pss = m_pulseAudio.stream_get_sample_spec(pasInput);
			const size_t sampleSize   = (pss->format == PA_SAMPLE_FLOAT32NE) ? sizeof(float) : sizeof(short);
			const unsigned int iBlockLen =
				static_cast< unsigned int >(static_cast< unsigned int >(pai->iFrameSize) * pss->channels * sampleSize);
			buff.tlength   = iBlockLen;
			buff.minreq    = iBlockLen;
			buff.maxlength = static_cast< decltype(buff.maxlength) >(-1);
			buff.prebuf    = static_cast< decltype(buff.prebuf) >(-1);
			buff.fragsize  = iBlockLen;

			qsInputCache = idev;

			m_pulseAudio.stream_connect_record(pasInput, qPrintable(idev), &buff, PA_STREAM_ADJUST_LATENCY);
		}
	}

	if (raw_ai) {
		QString odev        = outputDevice();
		QString edev        = qhEchoMap.value(odev);
		pa_stream_state est = pasSpeaker ? m_pulseAudio.stream_get_state(pasSpeaker) : PA_STREAM_TERMINATED;
		bool do_stop        = false;
		bool do_start       = false;

		if ((!pai || Global::get().s.echoOption == EchoCancelOptionID::DISABLED) && (est == PA_STREAM_READY)) {
			do_stop = true;
		} else if (pai && Global::get().s.echoOption != EchoCancelOptionID::DISABLED) {
			switch (est) {
				case PA_STREAM_TERMINATED: {
					if (pasSpeaker)
						m_pulseAudio.stream_unref(pasSpeaker);

					pa_sample_spec pss = qhSpecMap.value(edev);
					pa_channel_map pcm = qhChanMap.value(edev);
					if ((pss.format != PA_SAMPLE_FLOAT32NE) && (pss.format != PA_SAMPLE_S16NE))
						pss.format = PA_SAMPLE_FLOAT32NE;
					pss.rate = SAMPLE_RATE;
					if ((pss.channels == 0) || (Global::get().s.echoOption != EchoCancelOptionID::SPEEX_MULTICHANNEL))
						pss.channels = 1;

					pasSpeaker =
						m_pulseAudio.stream_new(pacContext, mumble_echo, &pss, (pss.channels == 1) ? nullptr : &pcm);
					m_pulseAudio.stream_set_state_callback(pasSpeaker, read_stream_callback, this);
					m_pulseAudio.stream_set_read_callback(pasSpeaker, read_callback, this);
				}
					// Fallthrough
				case PA_STREAM_UNCONNECTED:
					do_start = true;
					break;
				case PA_STREAM_READY: {
					if ((Global::get().s.echoOption == EchoCancelOptionID::SPEEX_MULTICHANNEL) != bEchoMultiCache) {
						do_stop = true;
					} else if (edev != qsEchoCache) {
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
			m_pulseAudio.stream_disconnect(pasSpeaker);
		} else if (do_start) {
			qWarning("PulseAudio: Starting echo: %s", qPrintable(edev));
			pa_buffer_attr buff;
			const pa_sample_spec *pss = m_pulseAudio.stream_get_sample_spec(pasSpeaker);
			const size_t sampleSize   = (pss->format == PA_SAMPLE_FLOAT32NE) ? sizeof(float) : sizeof(short);
			const unsigned int iBlockLen =
				static_cast< unsigned int >(static_cast< unsigned int >(pai->iFrameSize) * pss->channels * sampleSize);
			buff.tlength   = iBlockLen;
			buff.minreq    = iBlockLen;
			buff.maxlength = static_cast< decltype(buff.maxlength) >(-1);
			buff.prebuf    = static_cast< decltype(buff.prebuf) >(-1);
			buff.fragsize  = iBlockLen;

			bEchoMultiCache = (Global::get().s.echoOption == EchoCancelOptionID::SPEEX_MULTICHANNEL);
			qsEchoCache     = edev;

			m_pulseAudio.stream_connect_record(pasSpeaker, qPrintable(edev), &buff, PA_STREAM_ADJUST_LATENCY);
		}
	}
}

void PulseAudioSystem::context_state_callback(pa_context *c, void *userdata) {
	PulseAudioSystem *pas = reinterpret_cast< PulseAudioSystem * >(userdata);
	pas->contextCallback(c);
}

void PulseAudioSystem::subscribe_callback(pa_context *, pa_subscription_event_type evt, unsigned int, void *userdata) {
	switch (evt & PA_SUBSCRIPTION_EVENT_TYPE_MASK) {
		case PA_SUBSCRIPTION_EVENT_NEW:
		case PA_SUBSCRIPTION_EVENT_REMOVE:
			break;
		default:
			return;
	}
	switch (evt & PA_SUBSCRIPTION_EVENT_FACILITY_MASK) {
		case PA_SUBSCRIPTION_EVENT_SINK:
		case PA_SUBSCRIPTION_EVENT_SOURCE:
			break;
		default:
			return;
	}
	PulseAudioSystem *pas = reinterpret_cast< PulseAudioSystem * >(userdata);
	qWarning("PulseAudio: Sinks or inputs changed (inserted or removed sound card)");
	pas->query();
}

void PulseAudioSystem::sink_callback(pa_context *, const pa_sink_info *i, int eol, void *userdata) {
	PulseAudioSystem *pas = reinterpret_cast< PulseAudioSystem * >(userdata);
	if (!i || eol) {
		pas->bSinkDone = true;
		pas->wakeup();
		return;
	}

	const QString name = QString::fromUtf8(i->name);

	pas->qhSpecMap.insert(name, i->sample_spec);
	pas->qhChanMap.insert(name, i->channel_map);
	pas->qhOutput.insert(name, QString::fromUtf8(i->description));
	pas->qhEchoMap.insert(name, QString::fromUtf8(i->monitor_source_name));
}

void PulseAudioSystem::source_callback(pa_context *, const pa_source_info *i, int eol, void *userdata) {
	PulseAudioSystem *pas = reinterpret_cast< PulseAudioSystem * >(userdata);
	if (!i || eol) {
		pas->bSourceDone = true;
		pas->wakeup();
		return;
	}

	const QString name = QString::fromUtf8(i->name);

	pas->qhSpecMap.insert(name, i->sample_spec);
	pas->qhChanMap.insert(name, i->channel_map);

	pas->qhInput.insert(QString::fromUtf8(i->name), QString::fromUtf8(i->description));
}

void PulseAudioSystem::server_callback(pa_context *, const pa_server_info *i, void *userdata) {
	PulseAudioSystem *pas = reinterpret_cast< PulseAudioSystem * >(userdata);

	pas->qsDefaultInput  = QString::fromUtf8(i->default_source_name);
	pas->qsDefaultOutput = QString::fromUtf8(i->default_sink_name);

	pas->bServerDone = true;
	pas->wakeup();
}

void PulseAudioSystem::sink_info_callback(pa_context *, const pa_sink_info *i, int eol, void *userdata) {
	PulseAudioSystem *pas = reinterpret_cast< PulseAudioSystem * >(userdata);
	if (!i || eol) {
		return;
	}

	pas->iSinkId = static_cast< int >(i->index);
}

void PulseAudioSystem::write_stream_callback(pa_stream *s, void *userdata) {
	PulseAudioSystem *pas = reinterpret_cast< PulseAudioSystem * >(userdata);
	const auto &pa        = pas->m_pulseAudio;

	switch (pa.stream_get_state(s)) {
		case PA_STREAM_FAILED:
			qWarning("PulseAudio: Stream error: %s", pa.strerror(pa.context_errno(pa.stream_get_context(s))));
			break;
		default:
			break;
	}
	const pa_buffer_attr *bufferAttr;
	if ((bufferAttr = pa.stream_get_buffer_attr(s))) {
		Global::get().ao->setBufferSize(bufferAttr->maxlength);
	}
	pas->wakeup();
}

void PulseAudioSystem::read_stream_callback(pa_stream *s, void *userdata) {
	PulseAudioSystem *pas = reinterpret_cast< PulseAudioSystem * >(userdata);
	const auto &pa        = pas->m_pulseAudio;

	switch (pa.stream_get_state(s)) {
		case PA_STREAM_FAILED:
			qWarning("PulseAudio: Stream error: %s", pa.strerror(pa.context_errno(pa.stream_get_context(s))));
			break;
		default:
			break;
	}
	pas->wakeup();
}

void PulseAudioSystem::read_callback(pa_stream *s, size_t bytes, void *userdata) {
	PulseAudioSystem *pas = reinterpret_cast< PulseAudioSystem * >(userdata);
	const auto &pa        = pas->m_pulseAudio;

	size_t length    = bytes;
	const void *data = nullptr;
	pa.stream_peek(s, &data, &length);
	if (!data && length > 0) {
		qWarning("PulseAudio: pa_stream_peek reports no data at current read index.");
	} else if (!data && length == 0) {
		qWarning("PulseAudio: pa_stream_peek reports empty memblockq.");
	} else if (!data || length == 0) {
		qWarning("PulseAudio: invalid pa_stream_peek state encountered.");
		return;
	}

	AudioInputPtr ai     = Global::get().ai;
	PulseAudioInput *pai = dynamic_cast< PulseAudioInput * >(ai.get());
	if (!pai) {
		if (length > 0) {
			pa.stream_drop(s);
		}
		pas->wakeup();
		return;
	}

	const pa_sample_spec *pss = pa.stream_get_sample_spec(s);

	if (s == pas->pasInput) {
		if (!pa.sample_spec_equal(pss, &pai->pssMic)) {
			pai->pssMic       = *pss;
			pai->iMicFreq     = pss->rate;
			pai->iMicChannels = pss->channels;
			if (pss->format == PA_SAMPLE_FLOAT32NE)
				pai->eMicFormat = PulseAudioInput::SampleFloat;
			else
				pai->eMicFormat = PulseAudioInput::SampleShort;
			pai->initializeMixer();
		}
		if (data) {
			pai->addMic(data, static_cast< unsigned int >(length) / pai->iMicSampleSize);
		}
	} else if (s == pas->pasSpeaker) {
		if (!pa.sample_spec_equal(pss, &pai->pssEcho)) {
			pai->pssEcho       = *pss;
			pai->iEchoFreq     = pss->rate;
			pai->iEchoChannels = pss->channels;
			if (pss->format == PA_SAMPLE_FLOAT32NE)
				pai->eEchoFormat = PulseAudioInput::SampleFloat;
			else
				pai->eEchoFormat = PulseAudioInput::SampleShort;
			pai->initializeMixer();
		}
		if (data) {
			pai->addEcho(data, static_cast< unsigned int >(length) / pai->iEchoSampleSize);
		}
	}

	if (length > 0) {
		pa.stream_drop(s);
	}
}

void PulseAudioSystem::write_callback(pa_stream *s, size_t bytes, void *userdata) {
	PulseAudioSystem *pas = reinterpret_cast< PulseAudioSystem * >(userdata);
	Q_ASSERT(s == pas->pasOutput);

	const PulseAudio &pa = pas->m_pulseAudio;

	AudioOutputPtr ao     = Global::get().ao;
	PulseAudioOutput *pao = dynamic_cast< PulseAudioOutput * >(ao.get());

	if (!pao) {
		// The AudioSystem pointer is invalid, probably because of an AudioSystem restart.
		// However, PA expects this callback to return exactly the provided number of
		// bytes. Failing to do so will cause PA to never call this again,
		// effectively removing the audio output completely until Mumble is restarted.
		// See: https://github.com/mumble-voip/mumble/issues/4883
		// See: https://gitlab.freedesktop.org/pulseaudio/pulseaudio/-/issues/1132
		static std::vector< std::uint8_t > m_silenceBuffer;
		m_silenceBuffer.resize(bytes);

		pa.stream_write(s, m_silenceBuffer.data(), bytes, nullptr, 0, PA_SEEK_RELATIVE);
		return;
	}

	const pa_sample_spec *pss = pa.stream_get_sample_spec(s);
	const pa_channel_map *pcm = pa.stream_get_channel_map(pas->pasOutput);
	if (!pa.sample_spec_equal(pss, &pao->pss) || !pa.channel_map_equal(pcm, &pao->pcm)) {
		pao->pss = *pss;
		pao->pcm = *pcm;
		if (pss->format == PA_SAMPLE_FLOAT32NE)
			pao->eSampleFormat = PulseAudioOutput::SampleFloat;
		else
			pao->eSampleFormat = PulseAudioOutput::SampleShort;
		pao->iMixerFreq = pss->rate;
		pao->iChannels  = pss->channels;
		static std::vector< unsigned int > chanmasks;
		chanmasks.resize(pss->channels);
		for (int i = 0; i < pss->channels; ++i) {
			unsigned int cm = 0;
			switch (pcm->map[i]) {
				case PA_CHANNEL_POSITION_LEFT:
					cm = SPEAKER_FRONT_LEFT;
					break;
				case PA_CHANNEL_POSITION_RIGHT:
					cm = SPEAKER_FRONT_RIGHT;
					break;
				case PA_CHANNEL_POSITION_CENTER:
					cm = SPEAKER_FRONT_CENTER;
					break;
				case PA_CHANNEL_POSITION_REAR_LEFT:
					cm = SPEAKER_BACK_LEFT;
					break;
				case PA_CHANNEL_POSITION_REAR_RIGHT:
					cm = SPEAKER_BACK_RIGHT;
					break;
				case PA_CHANNEL_POSITION_REAR_CENTER:
					cm = SPEAKER_BACK_CENTER;
					break;
				case PA_CHANNEL_POSITION_LFE:
					cm = SPEAKER_LOW_FREQUENCY;
					break;
				case PA_CHANNEL_POSITION_SIDE_LEFT:
					cm = SPEAKER_SIDE_LEFT;
					break;
				case PA_CHANNEL_POSITION_SIDE_RIGHT:
					cm = SPEAKER_SIDE_RIGHT;
					break;
				case PA_CHANNEL_POSITION_FRONT_LEFT_OF_CENTER:
					cm = SPEAKER_FRONT_LEFT_OF_CENTER;
					break;
				case PA_CHANNEL_POSITION_FRONT_RIGHT_OF_CENTER:
					cm = SPEAKER_FRONT_RIGHT_OF_CENTER;
					break;
				default:
					cm = 0;
					break;
			}
			chanmasks[static_cast< std::size_t >(i)] = cm;
		}
		pao->initializeMixer(chanmasks.data());
	}

	const unsigned int iSampleSize = pao->iSampleSize;
	const unsigned int samples     = static_cast< unsigned int >(bytes) / iSampleSize;
	bool oldAttenuation            = pas->bAttenuating;

	static std::vector< unsigned char > buffer;
	buffer.resize(bytes);
	// do we have some mixed output?
	if (pao->mix(buffer.data(), samples)) {
		// attenuate if instructed to or it's in settings
		pas->bAttenuating = (Global::get().bAttenuateOthers || Global::get().s.bAttenuateOthers);

	} else {
		std::fill(buffer.begin(), buffer.end(), 0);

		// attenuate if intructed to (self-activated)
		pas->bAttenuating = Global::get().bAttenuateOthers;
	}

	// if the attenuation state has changed
	if (oldAttenuation != pas->bAttenuating) {
		pas->setVolumes();
	}

	pa.stream_write(s, buffer.data(), iSampleSize * samples, nullptr, 0, PA_SEEK_RELATIVE);
}

void PulseAudioSystem::volume_sink_input_list_callback(pa_context *c, const pa_sink_input_info *i, int eol,
													   void *userdata) {
	PulseAudioSystem *pas = reinterpret_cast< PulseAudioSystem * >(userdata);
	const auto &pa        = pas->m_pulseAudio;

	if (eol == 0) {
		// If we're using the default of "enable attenuation on all outputs" and output from an application is
		// loopbacked, both the loopback and the application will be attenuated leading to double attenuation.
		if (!Global::get().s.bOnlyAttenuateSameOutput && pas->iSinkId > -1 && !strcmp(i->driver, "module-loopback.c")) {
			return;
		}
		// If we're not attenuating different sinks and the input is not on this sink, don't attenuate. Or,
		// if the input is a loopback module and connected to Mumble's sink, also ignore it (loopbacks are used to
		// connect sinks). An attenuated loopback means an indirect application attenuation.
		if (Global::get().s.bOnlyAttenuateSameOutput && pas->iSinkId > -1) {
			if (int(i->sink) != pas->iSinkId
				|| (int(i->sink) == pas->iSinkId && !strcmp(i->driver, "module-loopback.c")
					&& !Global::get().s.bAttenuateLoopbacks)) {
				return;
			}
		}
		// ensure we're not attenuating ourselves!
		if (strcmp(i->name, mumble_sink_input) != 0) {
			// create a new entry
			PulseAttenuation patt;
			patt.index             = i->index;
			patt.name              = QString::fromUtf8(i->name);
			patt.stream_restore_id = QString::fromUtf8(pa.proplist_gets(i->proplist, "module-stream-restore.id"));
			patt.normal_volume     = i->volume;

			// calculate the attenuated volume
			pa_volume_t adj = static_cast< pa_volume_t >(PA_VOLUME_NORM * Global::get().s.fOtherVolume);
			pa.sw_cvolume_multiply_scalar(&patt.attenuated_volume, &i->volume, adj);

			// set it on the sink input
			pa.operation_unref(
				pa.context_set_sink_input_volume(c, i->index, &patt.attenuated_volume, nullptr, nullptr));

			// store it
			pas->qhVolumes[i->index] = patt;
		}

	} else if (eol < 0) {
		qWarning("PulseAudio: Sink input introspection error.");
	}
}

void PulseAudioSystem::restore_sink_input_list_callback(pa_context *c, const pa_sink_input_info *i, int eol,
														void *userdata) {
	PulseAudioSystem *pas = reinterpret_cast< PulseAudioSystem * >(userdata);
	const auto &pa        = pas->m_pulseAudio;

	if (eol == 0) {
		// if we were tracking this specific sink previously
		if (pas->qhVolumes.contains(i->index)) {
			// and if it has the attenuated volume we applied to it
			if (pa.cvolume_equal(&i->volume, &pas->qhVolumes[i->index].attenuated_volume) != 0) {
				// mark it as matched
				pas->qlMatchedSinks.append(i->index);

				// reset the volume to normal
				pas->iRemainingOperations++;
				pa.operation_unref(pa.context_set_sink_input_volume(
					c, i->index, &pas->qhVolumes[i->index].normal_volume, restore_volume_success_callback, pas));
			}

			// otherwise, save for matching at the end of iteration
		} else {
			QString restore_id = QString::fromUtf8(pa.proplist_gets(i->proplist, "module-stream-restore.id"));
			PulseAttenuation patt;
			patt.index                        = i->index;
			patt.normal_volume                = i->volume;
			pas->qhUnmatchedSinks[restore_id] = patt;
		}

	} else if (eol < 0) {
		qWarning("PulseAudio: Sink input introspection error.");

	} else {
		// build a list of missing streams by iterating our active list
		QHash< uint32_t, PulseAttenuation >::const_iterator it;
		for (it = pas->qhVolumes.constBegin(); it != pas->qhVolumes.constEnd(); ++it) {
			// skip if previously matched
			if (pas->qlMatchedSinks.contains(it.key())) {
				continue;
			}

			// check if the restore id matches. the only case where this would
			// happen is if the application was reopened during attenuation.
			if (pas->qhUnmatchedSinks.contains(it.value().stream_restore_id)) {
				PulseAttenuation active_sink = pas->qhUnmatchedSinks[it.value().stream_restore_id];
				// if the volume wasn't changed from our attenuation
				if (pa.cvolume_equal(&active_sink.normal_volume, &it.value().attenuated_volume) != 0) {
					// reset the volume to normal
					pas->iRemainingOperations++;
					pa.operation_unref(pa.context_set_sink_input_volume(c, active_sink.index, &it.value().normal_volume,
																		restore_volume_success_callback, pas));
				}
				continue;
			}

			// at this point, we don't know what happened to the sink. add
			// it to a list to check the stream restore database for.
			pas->qhMissingSinks[it.value().stream_restore_id] = it.value();
		}

		// clean up
		pas->qlMatchedSinks.clear();
		pas->qhUnmatchedSinks.clear();
		pas->qhVolumes.clear();

		// if we had missing sinks, check the stream restore database
		// to see if we can find and update them.
		if (pas->qhMissingSinks.count() > 0) {
			pas->iRemainingOperations++;
			pa.operation_unref(pa.ext_stream_restore_read(c, stream_restore_read_callback, pas));
		}

		// trigger the volume completion callback;
		// necessary so that shutdown actions are called
		restore_volume_success_callback(c, 1, pas);
	}
}

void PulseAudioSystem::stream_restore_read_callback(pa_context *c, const pa_ext_stream_restore_info *i, int eol,
													void *userdata) {
	PulseAudioSystem *pas = reinterpret_cast< PulseAudioSystem * >(userdata);
	const auto &pa        = pas->m_pulseAudio;

	if (eol == 0) {
		QString name = QString::fromUtf8(i->name);

		// were we looking for this restoration?
		if (pas->qhMissingSinks.contains(name)) {
			// make sure it still has the volume we gave it
			if (pa.cvolume_equal(&pas->qhMissingSinks[name].attenuated_volume, &i->volume) != 0) {
				// update the stream restore record
				pa_ext_stream_restore_info restore = *i;
				restore.volume                     = pas->qhMissingSinks[name].normal_volume;
				pas->iRemainingOperations++;
				pa.operation_unref(pa.ext_stream_restore_write(c, PA_UPDATE_REPLACE, &restore, 1, 1,
															   restore_volume_success_callback, pas));
			}

			pas->qhMissingSinks.remove(name);
		}

	} else if (eol < 0) {
		qWarning("PulseAudio: Couldn't read stream restore database.");
		pas->qhMissingSinks.clear();

	} else {
		// verify missing list is empty
		if (pas->qhMissingSinks.count() > 0) {
			qWarning("PulseAudio: Failed to match %lld stream(s).",
					 static_cast< qsizetype >(pas->qhMissingSinks.count()));
			pas->qhMissingSinks.clear();
		}

		// trigger the volume completion callback;
		// necessary so that shutdown actions are called
		restore_volume_success_callback(c, 1, pas);
	}
}

void PulseAudioSystem::restore_volume_success_callback(pa_context *, int, void *userdata) {
	PulseAudioSystem *pas = reinterpret_cast< PulseAudioSystem * >(userdata);

	pas->iRemainingOperations--;
}

void PulseAudioSystem::query() {
	bSourceDone = bSinkDone = bServerDone = false;
	qhInput.clear();
	qhOutput.clear();
	qhEchoMap.clear();
	qhSpecMap.clear();
	qhChanMap.clear();
	qhInput.insert(QString(), tr("Default Input"));
	qhOutput.insert(QString(), tr("Default Output"));
	m_pulseAudio.operation_unref(m_pulseAudio.context_get_server_info(pacContext, server_callback, this));
	m_pulseAudio.operation_unref(m_pulseAudio.context_get_sink_info_list(pacContext, sink_callback, this));
	m_pulseAudio.operation_unref(m_pulseAudio.context_get_source_info_list(pacContext, source_callback, this));
	wakeup();
}

void PulseAudioSystem::setVolumes() {
	// set attenuation state and volumes
	if (bAttenuating) {
		// ensure the volume map is empty, otherwise it may be dangerous to change
		if (qhVolumes.empty()) {
			// set the new per-application volumes and store the old ones
			m_pulseAudio.operation_unref(
				m_pulseAudio.context_get_sink_input_info_list(pacContext, volume_sink_input_list_callback, this));
		}
		// clear attenuation state and restore normal volumes
	} else {
		iRemainingOperations++;
		m_pulseAudio.operation_unref(
			m_pulseAudio.context_get_sink_input_info_list(pacContext, restore_sink_input_list_callback, this));
	}
}

void PulseAudioSystem::contextCallback(pa_context *c) {
	Q_ASSERT(c == pacContext);
	switch (m_pulseAudio.context_get_state(c)) {
		case PA_CONTEXT_READY: {
			bPulseIsGood = true;
			const auto mask =
				static_cast< pa_subscription_mask_t >(PA_SUBSCRIPTION_MASK_SINK | PA_SUBSCRIPTION_MASK_SOURCE);
			m_pulseAudio.operation_unref(m_pulseAudio.context_subscribe(pacContext, mask, nullptr, this));
			query();
			break;
		}
		case PA_CONTEXT_TERMINATED:
			qWarning("PulseAudio: Forcibly disconnected from PulseAudio");
			break;
		case PA_CONTEXT_FAILED:
			qWarning("PulseAudio: Connection failure: %s", m_pulseAudio.strerror(m_pulseAudio.context_errno(c)));
			break;
		default:
			// These are other status callbacks we don't care about. However we explicitly want to wait until
			// one of the status flags listed above are emitted before claiming we are initialized (this callback
			// will be called multiple times).
			return;
	}

	{
		std::unique_lock< std::mutex > guard(m_initLock);
		m_initialized = true;
	}
	m_initWaiter.notify_all();
}

PulseAudioInputRegistrar::PulseAudioInputRegistrar() : AudioInputRegistrar(QLatin1String("PulseAudio"), 10) {
	echoOptions.push_back(EchoCancelOptionID::SPEEX_MIXED);
	echoOptions.push_back(EchoCancelOptionID::SPEEX_MULTICHANNEL);
}

AudioInput *PulseAudioInputRegistrar::create() {
	return new PulseAudioInput();
}

const QVariant PulseAudioInputRegistrar::getDeviceChoice() {
	return Global::get().s.qsPulseAudioInput;
}

const QList< audioDevice > PulseAudioInputRegistrar::getDeviceChoices() {
	QList< audioDevice > choices;

	QStringList keys = pasys->qhInput.keys();
	std::sort(keys.begin(), keys.end());

	for (const auto &key : keys) {
		choices << audioDevice(pasys->qhInput.value(key), key);
	}

	return choices;
}

void PulseAudioInputRegistrar::setDeviceChoice(const QVariant &choice, Settings &s) {
	s.qsPulseAudioInput = choice.toString();
}

bool PulseAudioInputRegistrar::canEcho(EchoCancelOptionID echoOption, const QString &osys) const {
	return (echoOption == EchoCancelOptionID::SPEEX_MIXED || echoOption == EchoCancelOptionID::SPEEX_MULTICHANNEL)
		   && (osys == name);
}

PulseAudioOutputRegistrar::PulseAudioOutputRegistrar() : AudioOutputRegistrar(QLatin1String("PulseAudio"), 10) {
}

AudioOutput *PulseAudioOutputRegistrar::create() {
	return new PulseAudioOutput();
}

const QVariant PulseAudioOutputRegistrar::getDeviceChoice() {
	return Global::get().s.qsPulseAudioOutput;
}

const QList< audioDevice > PulseAudioOutputRegistrar::getDeviceChoices() {
	QList< audioDevice > choices;

	QStringList keys = pasys->qhOutput.keys();
	std::sort(keys.begin(), keys.end());

	for (const auto &key : keys) {
		choices << audioDevice(pasys->qhOutput.value(key), key);
	}

	return choices;
}

void PulseAudioOutputRegistrar::setDeviceChoice(const QVariant &choice, Settings &s) {
	s.qsPulseAudioOutput = choice.toString();
}

bool PulseAudioOutputRegistrar::canMuteOthers() const {
	return true;
}

PulseAudioInput::PulseAudioInput() {
	memset(&pssMic, 0, sizeof(pssMic));
	memset(&pssEcho, 0, sizeof(pssEcho));
	bRunning = true;
	if (pasys)
		pasys->wakeup_lock();
}

PulseAudioInput::~PulseAudioInput() {
	bRunning = false;
	qmMutex.lock();
	qwcWait.wakeAll();
	qmMutex.unlock();
	wait();
	if (pasys)
		pasys->wakeup_lock();
}

void PulseAudioInput::run() {
	qmMutex.lock();
	while (bRunning)
		qwcWait.wait(&qmMutex);
	qmMutex.unlock();
}

PulseAudioOutput::PulseAudioOutput() {
	memset(&pss, 0, sizeof(pss));
	memset(&pcm, 0, sizeof(pcm));
	bRunning = true;
	if (pasys)
		pasys->wakeup_lock();
}

PulseAudioOutput::~PulseAudioOutput() {
	bRunning = false;
	qmMutex.lock();
	qwcWait.wakeAll();
	qmMutex.unlock();
	wait();
	if (pasys)
		pasys->wakeup_lock();
}

void PulseAudioOutput::run() {
	qmMutex.lock();
	while (bRunning)
		qwcWait.wait(&qmMutex);
	qmMutex.unlock();
}

PulseAudio::PulseAudio() : m_ok(false) {
	const QStringList alternatives{ QLatin1String("libpulse.so"), QLatin1String("libpulse.so.0") };

	for (const QString &alternative : alternatives) {
		m_lib.setFileName(alternative);
		if (m_lib.load()) {
			break;
		}
	}

	if (!m_lib.isLoaded()) {
		return;
	}

	RESOLVE(get_library_version);
	RESOLVE(strerror);
	RESOLVE(operation_unref);
	RESOLVE(cvolume_equal);
	RESOLVE(sw_cvolume_multiply_scalar);
	RESOLVE(sample_spec_equal);
	RESOLVE(channel_map_equal);
	RESOLVE(proplist_new);
	RESOLVE(proplist_free);
	RESOLVE(proplist_gets);
	RESOLVE(proplist_sets);
	RESOLVE(threaded_mainloop_new);
	RESOLVE(threaded_mainloop_free);
	RESOLVE(threaded_mainloop_start);
	RESOLVE(threaded_mainloop_stop);
	RESOLVE(threaded_mainloop_lock);
	RESOLVE(threaded_mainloop_unlock);
	RESOLVE(threaded_mainloop_get_api);
	RESOLVE(context_errno);
	RESOLVE(context_new_with_proplist);
	RESOLVE(context_unref);
	RESOLVE(context_connect);
	RESOLVE(context_disconnect);
	RESOLVE(context_subscribe);
	RESOLVE(context_get_state);
	RESOLVE(context_get_server_info);
	RESOLVE(context_get_sink_info_by_name);
	RESOLVE(context_get_sink_info_list);
	RESOLVE(context_get_sink_input_info_list);
	RESOLVE(context_get_source_info_list);
	RESOLVE(context_set_sink_input_volume);
	RESOLVE(context_set_subscribe_callback);
	RESOLVE(context_set_state_callback);
	RESOLVE(stream_new);
	RESOLVE(stream_unref);
	RESOLVE(stream_connect_playback);
	RESOLVE(stream_connect_record);
	RESOLVE(stream_disconnect);
	RESOLVE(stream_peek);
	RESOLVE(stream_write);
	RESOLVE(stream_drop);
	RESOLVE(stream_cork);
	RESOLVE(stream_get_state);
	RESOLVE(stream_get_context);
	RESOLVE(stream_get_sample_spec);
	RESOLVE(stream_get_channel_map);
	RESOLVE(stream_get_buffer_attr);
	RESOLVE(stream_set_state_callback);
	RESOLVE(stream_set_read_callback);
	RESOLVE(stream_set_write_callback);
	RESOLVE(ext_stream_restore_read);
	RESOLVE(ext_stream_restore_write);

	qDebug("PulseAudio %s from %s", get_library_version(), qPrintable(m_lib.fileName()));

	m_ok = true;
}

#undef RESOLVE
#undef NBLOCKS
