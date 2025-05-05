// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ALSAAudio.h"

#include "MainWindow.h"
#include "User.h"
#include "Utils.h"

#include <alsa/asoundlib.h>
#include <poll.h>

#include "Global.h"

#define NBLOCKS 8

class ALSAEnumerator {
public:
	QHash< QString, QString > qhInput;
	QHash< QString, QString > qhOutput;
	static QString getHint(void *hint, const char *id);
	ALSAEnumerator();
};

static ALSAEnumerator *cards = nullptr;

class ALSAAudioInputRegistrar : public AudioInputRegistrar {
public:
	ALSAAudioInputRegistrar();
	virtual AudioInput *create();
	virtual const QVariant getDeviceChoice();
	virtual const QList< audioDevice > getDeviceChoices();
	virtual void setDeviceChoice(const QVariant &, Settings &);
	virtual bool canEcho(EchoCancelOptionID echoCancelID, const QString &outputSystem) const;
	virtual bool isMicrophoneAccessDeniedByOS() { return false; };
};


class ALSAAudioOutputRegistrar : public AudioOutputRegistrar {
public:
	ALSAAudioOutputRegistrar();
	virtual AudioOutput *create();
	virtual const QVariant getDeviceChoice();
	virtual const QList< audioDevice > getDeviceChoices();
	virtual void setDeviceChoice(const QVariant &, Settings &);
};

class ALSAInit : public DeferInit {
protected:
	ALSAAudioInputRegistrar *pairALSA;
	ALSAAudioOutputRegistrar *paorALSA;

public:
	void initialize();
	void destroy();
};

static ALSAInit aiInit;
QMutex qmALSA;

void ALSAInit::initialize() {
	pairALSA = nullptr;
	paorALSA = nullptr;
	cards    = nullptr;

	int card = -1;
	snd_card_next(&card);
	if (card != -1) {
		pairALSA = new ALSAAudioInputRegistrar();
		paorALSA = new ALSAAudioOutputRegistrar();
		cards    = new ALSAEnumerator();
	} else {
		qWarning("ALSAInit: No cards found, not initializing");
	}
}

void ALSAInit::destroy() {
	QMutexLocker qml(&qmALSA);
	delete pairALSA;
	delete paorALSA;
	delete cards;
}

ALSAAudioInputRegistrar::ALSAAudioInputRegistrar() : AudioInputRegistrar(QLatin1String("ALSA"), 5) {
}

AudioInput *ALSAAudioInputRegistrar::create() {
	return new ALSAAudioInput();
}

const QVariant ALSAAudioInputRegistrar::getDeviceChoice() {
	return Global::get().s.qsALSAInput;
}

const QList< audioDevice > ALSAAudioInputRegistrar::getDeviceChoices() {
	QList< audioDevice > choices;

	QStringList keys = cards->qhInput.keys();
	std::sort(keys.begin(), keys.end());

	for (const auto &key : keys) {
		const auto name = QString::fromLatin1("[%1] %2").arg(key, cards->qhInput.value(key));
		choices << audioDevice(name, key);
	}

	return choices;
}

void ALSAAudioInputRegistrar::setDeviceChoice(const QVariant &choice, Settings &s) {
	s.qsALSAInput = choice.toString();
}

bool ALSAAudioInputRegistrar::canEcho(EchoCancelOptionID, const QString &) const {
	return false;
}

ALSAAudioOutputRegistrar::ALSAAudioOutputRegistrar() : AudioOutputRegistrar(QLatin1String("ALSA"), 5) {
}

AudioOutput *ALSAAudioOutputRegistrar::create() {
	return new ALSAAudioOutput();
}

const QVariant ALSAAudioOutputRegistrar::getDeviceChoice() {
	return Global::get().s.qsALSAOutput;
}

const QList< audioDevice > ALSAAudioOutputRegistrar::getDeviceChoices() {
	QList< audioDevice > choices;

	QStringList keys = cards->qhOutput.keys();
	std::sort(keys.begin(), keys.end());

	for (const auto &key : keys) {
		const auto name = QString::fromLatin1("[%1] %2").arg(key, cards->qhOutput.value(key));
		choices << audioDevice(name, key);
	}

	return choices;
}

void ALSAAudioOutputRegistrar::setDeviceChoice(const QVariant &choice, Settings &s) {
	s.qsALSAOutput = choice.toString();
}

ALSAEnumerator::ALSAEnumerator() {
	QMutexLocker qml(&qmALSA);

	qhInput.insert(QLatin1String("default"), ALSAAudioInput::tr("Default ALSA Card"));
	qhOutput.insert(QLatin1String("default"), ALSAAudioOutput::tr("Default ALSA Card"));

#if SND_LIB_VERSION >= 0x01000e
	void **hints = nullptr;
	void **hint;
	snd_config_t *basic = nullptr;
	int r;

	snd_config_update();
	r = snd_config_search(snd_config, "defaults.namehint.extended", &basic);
	if ((r == 0) && basic) {
		if (snd_config_set_ascii(basic, "on"))
			qWarning("ALSAEnumerator: Failed to set namehint");
	} else {
		qWarning("ALSAEnumerator: Namehint not found");
	}

	r = snd_device_name_hint(-1, "pcm", &hints);

	if (r || !hints) {
		qWarning("ALSAEnumerator: snd_device_name_hint: %d", r);
	} else {
		hint = hints;
		while (*hint) {
			const QString name = getHint(*hint, "NAME");
			const QString ioid = getHint(*hint, "IOID");
			QString desc       = getHint(*hint, "DESC");

			desc.replace(QLatin1Char('\n'), QLatin1Char(' '));


			// ALSA, in it's infinite wisdom, claims "dmix" is an input/output device.
			// Since there seems to be no way to fetch the ctl interface for a matching device string
			// without actually opening it, we'll simply have to start guessing.

			bool caninput  = (ioid.isNull() || (ioid.compare(QLatin1String("Input"), Qt::CaseInsensitive) == 0));
			bool canoutput = (ioid.isNull() || (ioid.compare(QLatin1String("Output"), Qt::CaseInsensitive) == 0));

			if (name.startsWith(QLatin1String("dmix:")))
				caninput = false;
			else if (name.startsWith(QLatin1String("dsnoop:")))
				canoutput = false;

			if (caninput)
				qhInput.insert(name, desc);
			if (canoutput)
				qhOutput.insert(name, desc);

			++hint;
		}
		snd_device_name_free_hint(hints);
	}

	snd_config_update_free_global();
	snd_config_update();
#else
	int card = -1;
	snd_card_next(&card);
	while (card != -1) {
		char *name;
		int err;
		snd_ctl_t *ctl = nullptr;
		if ((err = snd_card_get_longname(card, &name)) != 0) {
			Global::get().mw->msgBox(tr("Getting name (longname) of the sound card failed: %1")
										 .arg(QString::fromUtf8(snd_strerror(err)).toHtmlEscaped()));
			return;
		}
		QByteArray dev = QString::fromLatin1("hw:%1").arg(card).toUtf8();
		if (snd_ctl_open(&ctl, dev.data(), SND_CTL_READONLY) >= 0) {
			snd_pcm_info_t *info = nullptr;
			snd_pcm_info_malloc(&info);

			char *cname = nullptr;
			if ((err = snd_card_get_name(card, &cname)) != 0) {
				Global::get().mw->msgBox(tr("Getting name of the sound card failed: %1")
											 .arg(QString::fromUtf8(snd_strerror(err)).toHtmlEscaped()));
				return;
			}
			int device = -1;
			snd_ctl_pcm_next_device(ctl, &device);

			bool play = false;
			bool cap  = false;

			while (device != -1) {
				QString devname = QString::fromLatin1("hw:%1,%2").arg(card).arg(device);
				snd_pcm_info_set_device(info, device);
				snd_pcm_info_set_stream(info, SND_PCM_STREAM_CAPTURE);
				if (snd_ctl_pcm_info(ctl, info) == 0) {
					QString fname = QString::fromLatin1(snd_pcm_info_get_name(info));
					qhInput.insert(devname, fname);
					cap = true;
				}

				snd_pcm_info_set_stream(info, SND_PCM_STREAM_PLAYBACK);
				if (snd_ctl_pcm_info(ctl, info) == 0) {
					QString fname = QString::fromLatin1(snd_pcm_info_get_name(info));
					qhOutput.insert(devname, fname);
					play = true;
				}

				snd_ctl_pcm_next_device(ctl, &device);
			}
			if (play) {
				qhOutput.insert(QString::fromLatin1("dmix:CARD=%1").arg(card), QLatin1String(cname));
			}
			if (cap) {
				qhInput.insert(QString::fromLatin1("dsnoop:CARD=%1").arg(card), QLatin1String(cname));
			}
			snd_pcm_info_free(info);
			snd_ctl_close(ctl);
			free(cname);
		}
		snd_card_next(&card);
		free(name);
	}
#endif
}

QString ALSAEnumerator::getHint(void *hint, const char *id) {
	QString s;
#if SND_LIB_VERSION >= 0x01000e
	char *value = snd_device_name_get_hint(hint, id);
	if (value) {
		s = QLatin1String(value);
		free(value);
	}
#endif
	return s;
}


ALSAAudioInput::ALSAAudioInput() {
	bRunning = true;
}

ALSAAudioInput::~ALSAAudioInput() {
	// Signal input thread to end
	bRunning = false;
	wait();
}

#define ALSA_ERRBAIL(x)                                       \
	if (!bOk) {                                               \
	} else if ((err = static_cast< int >(x)) < 0) {           \
		bOk = false;                                          \
		qWarning("ALSAAudio: %s: %s", #x, snd_strerror(err)); \
	}
#define ALSA_ERRCHECK(x)                                                    \
	if (!bOk) {                                                             \
	} else if ((err = static_cast< int >(x)) < 0) {                         \
		qWarning("ALSAAudio: Non-critical: %s: %s", #x, snd_strerror(err)); \
	}

void ALSAAudioInput::run() {
	QMutexLocker qml(&qmALSA);

	QByteArray device_name         = Global::get().s.qsALSAInput.toLatin1();
	snd_pcm_hw_params_t *hw_params = nullptr;
	snd_pcm_t *capture_handle      = nullptr;

	unsigned int rrate = SAMPLE_RATE;
	bool bOk           = true;

	int err = 0;

	unsigned int iChannels = 1;

	qWarning("ALSAAudioInput: Initing audiocapture %s.", device_name.data());

	snd_pcm_hw_params_alloca(&hw_params);

	ALSA_ERRBAIL(snd_pcm_open(&capture_handle, device_name.data(), SND_PCM_STREAM_CAPTURE, 0));
	ALSA_ERRCHECK(snd_pcm_hw_params_any(capture_handle, hw_params));
	ALSA_ERRBAIL(snd_pcm_hw_params_set_access(capture_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED));
	ALSA_ERRBAIL(snd_pcm_hw_params_set_format(capture_handle, hw_params, SND_PCM_FORMAT_S16));
	ALSA_ERRBAIL(snd_pcm_hw_params_set_rate_near(capture_handle, hw_params, &rrate, nullptr));
	ALSA_ERRBAIL(snd_pcm_hw_params_set_channels_near(capture_handle, hw_params, &iChannels));

	snd_pcm_uframes_t wantPeriod = (rrate * iFrameSize) / SAMPLE_RATE;
	snd_pcm_uframes_t wantBuff   = wantPeriod * 8;

	ALSA_ERRBAIL(snd_pcm_hw_params_set_period_size_near(capture_handle, hw_params, &wantPeriod, nullptr));
	ALSA_ERRBAIL(snd_pcm_hw_params_set_buffer_size_near(capture_handle, hw_params, &wantBuff));
	ALSA_ERRBAIL(snd_pcm_hw_params(capture_handle, hw_params));

	qWarning("ALSAAudioInput: Actual buffer %d hz, %d channel %ld samples [%ld per period]", rrate, iChannels, wantBuff,
			 wantPeriod);

	ALSA_ERRBAIL(snd_pcm_hw_params_current(capture_handle, hw_params));
	ALSA_ERRBAIL(snd_pcm_hw_params_get_channels(hw_params, &iMicChannels));
	ALSA_ERRBAIL(snd_pcm_hw_params_get_rate(hw_params, &iMicFreq, nullptr));

#ifdef ALSA_VERBOSE
	snd_output_t *log;
	snd_output_stdio_attach(&log, stderr, 0);
	if (capture_handle)
		snd_pcm_dump(capture_handle, log);
#endif

	ALSA_ERRBAIL(snd_pcm_prepare(capture_handle));
	ALSA_ERRBAIL(snd_pcm_start(capture_handle));

	if (!bOk) {
		if (capture_handle) {
			snd_pcm_drain(capture_handle);
			snd_pcm_close(capture_handle);
			capture_handle = nullptr;
		}
		Global::get().mw->msgBox(
			tr("Opening chosen ALSA Input failed: %1").arg(QString::fromLatin1(snd_strerror(err)).toHtmlEscaped()));
		return;
	}

	eMicFormat = SampleShort;
	initializeMixer();

	static std::vector< char > inbuff;
	inbuff.resize(wantPeriod * iChannels * sizeof(short));

	qml.unlock();

	while (bRunning) {
#ifdef ALSA_VERBOSE
		snd_pcm_status_malloc(&status);
		snd_pcm_status(capture_handle, status);
		snd_pcm_status_dump(status, log);
		snd_pcm_status_free(status);
#endif
		const snd_pcm_sframes_t ret = snd_pcm_readi(capture_handle, inbuff.data(), wantPeriod);
		if (ret >= 0) {
			if (static_cast< snd_pcm_uframes_t >(ret) == wantPeriod) {
				addMic(inbuff.data(), static_cast< unsigned int >(ret));
			}
		} else {
			err = static_cast< decltype(err) >(ret);
			switch (err) {
				case -EINTR:
				case -EPIPE:
				case -ESTRPIPE:
					qWarning("ALSAAudioInput encountered unrecoverable error: %s -> exiting...", snd_strerror(err));
					while (bRunning && snd_pcm_recover(capture_handle, err, 1) == -EAGAIN) {
						msleep(1000);
					}

					break;
				default:
					qWarning("ALSAAudioInput: %s, breaking the loop...", snd_strerror(err));
					bRunning = false;
			}
		}
	}

	snd_pcm_drop(capture_handle);
	snd_pcm_close(capture_handle);

	qWarning("ALSAAudioInput: Releasing ALSA Mic.");
}

ALSAAudioOutput::ALSAAudioOutput() {
	qWarning("ALSAAudioOutput: Initialized");
	bRunning = true;
}

ALSAAudioOutput::~ALSAAudioOutput() {
	bRunning = false;
	// Call destructor of all children
	wipe();
	// Wait for terminate
	wait();
	qWarning("ALSAAudioOutput: Destroyed");
}

void ALSAAudioOutput::run() {
	QMutexLocker qml(&qmALSA);
	snd_pcm_t *pcm_handle = nullptr;
	struct pollfd fds[16];
	int count;
	bool stillRun = true;
	int err       = 0;
	bool bOk      = true;


	snd_pcm_hw_params_t *hw_params = nullptr;
	snd_pcm_sw_params_t *sw_params = nullptr;
	QByteArray device_name         = Global::get().s.qsALSAOutput.toLatin1();

	snd_pcm_hw_params_alloca(&hw_params);
	snd_pcm_sw_params_alloca(&sw_params);

	ALSA_ERRBAIL(snd_pcm_open(&pcm_handle, device_name.data(), SND_PCM_STREAM_PLAYBACK, 0));
	ALSA_ERRCHECK(snd_pcm_hw_params_any(pcm_handle, hw_params));

	iChannels = 1;
	ALSA_ERRBAIL(snd_pcm_hw_params_get_channels_max(hw_params, &iChannels));
	if (iChannels > 9) {
		qWarning("ALSAAudioOutput: ALSA reports %d output channels. Clamping to 2.", iChannels);
		iChannels = 2;
	}

	ALSA_ERRBAIL(snd_pcm_hw_params_set_access(pcm_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED));
	ALSA_ERRBAIL(snd_pcm_hw_params_set_format(pcm_handle, hw_params, SND_PCM_FORMAT_S16));
	ALSA_ERRBAIL(snd_pcm_hw_params_set_channels_near(pcm_handle, hw_params, &iChannels));
	unsigned int rrate = SAMPLE_RATE;
	ALSA_ERRBAIL(snd_pcm_hw_params_set_rate_near(pcm_handle, hw_params, &rrate, nullptr));

	unsigned int iOutputSize = (iFrameSize * rrate) / SAMPLE_RATE;

	snd_pcm_uframes_t period_size = iOutputSize;
	snd_pcm_uframes_t buffer_size = iOutputSize * static_cast< unsigned int >(Global::get().s.iOutputDelay + 1);

	int dir = 1;
	ALSA_ERRBAIL(snd_pcm_hw_params_set_period_size_near(pcm_handle, hw_params, &period_size, &dir));
	ALSA_ERRBAIL(snd_pcm_hw_params_set_buffer_size_near(pcm_handle, hw_params, &buffer_size));

	ALSA_ERRBAIL(snd_pcm_hw_params(pcm_handle, hw_params));
	ALSA_ERRBAIL(snd_pcm_hw_params_current(pcm_handle, hw_params));
	ALSA_ERRBAIL(snd_pcm_hw_params_get_period_size(hw_params, &period_size, &dir));
	ALSA_ERRBAIL(snd_pcm_hw_params_get_buffer_size(hw_params, &buffer_size));

	qWarning("ALSAAudioOutput: Actual buffer %d hz, %d channel %ld samples [%ld per period]", rrate, iChannels,
			 buffer_size, period_size);

	ALSA_ERRBAIL(snd_pcm_sw_params_current(pcm_handle, sw_params));
	ALSA_ERRBAIL(snd_pcm_sw_params_set_avail_min(pcm_handle, sw_params, period_size));
	ALSA_ERRBAIL(snd_pcm_sw_params_set_start_threshold(pcm_handle, sw_params, buffer_size - period_size));
	ALSA_ERRBAIL(snd_pcm_sw_params_set_stop_threshold(pcm_handle, sw_params, buffer_size));
	ALSA_ERRBAIL(snd_pcm_sw_params(pcm_handle, sw_params));

#ifdef ALSA_VERBOSE
	snd_output_t *log;
	snd_output_stdio_attach(&log, stderr, 0);
	if (pcm_handle)
		snd_pcm_dump(pcm_handle, log);
#endif

	ALSA_ERRBAIL(snd_pcm_prepare(pcm_handle));

	const unsigned int buffsize = static_cast< unsigned int >(period_size * iChannels);

	static std::vector< float > zerobuff;
	zerobuff.resize(buffsize);
	static std::vector< float > outbuff;
	outbuff.resize(buffsize);

	for (unsigned int i = 0; i < buffsize; i++)
		zerobuff[i] = 0;

	// Fill buffer
	if (bOk && pcm_handle)
		for (unsigned int i = 0; i < buffer_size / period_size; i++)
			snd_pcm_writei(pcm_handle, zerobuff.data(), period_size);

	if (!bOk) {
		Global::get().mw->msgBox(
			tr("Opening chosen ALSA Output failed: %1").arg(QString::fromLatin1(snd_strerror(err)).toHtmlEscaped()));
		if (pcm_handle) {
			snd_pcm_close(pcm_handle);
			pcm_handle = nullptr;
		}
		return;
	}

	const unsigned int chanmasks[32] = { SPEAKER_FRONT_LEFT, SPEAKER_FRONT_RIGHT,  SPEAKER_BACK_LEFT,
										 SPEAKER_BACK_RIGHT, SPEAKER_FRONT_CENTER, SPEAKER_LOW_FREQUENCY,
										 SPEAKER_SIDE_LEFT,  SPEAKER_SIDE_RIGHT,   SPEAKER_BACK_CENTER };

	ALSA_ERRBAIL(snd_pcm_hw_params_current(pcm_handle, hw_params));
	ALSA_ERRBAIL(snd_pcm_hw_params_get_channels(hw_params, &iChannels));
	ALSA_ERRBAIL(snd_pcm_hw_params_get_rate(hw_params, &rrate, nullptr));
	iMixerFreq    = rrate;
	eSampleFormat = SampleShort;

	qWarning("ALSAAudioOutput: Initializing %d channel, %d hz mixer", iChannels, iMixerFreq);
	initializeMixer(chanmasks);

	count = snd_pcm_poll_descriptors_count(pcm_handle);
	snd_pcm_poll_descriptors(pcm_handle, fds, static_cast< unsigned int >(count));

	qml.unlock();

	while (bRunning && bOk) {
		poll(fds, static_cast< nfds_t >(count), 20);
		unsigned short revents;

		snd_pcm_poll_descriptors_revents(pcm_handle, fds, static_cast< unsigned int >(count), &revents);
		if (revents & POLLERR) {
			snd_pcm_prepare(pcm_handle);
		} else if (revents & POLLOUT) {
			snd_pcm_sframes_t avail{};
			ALSA_ERRCHECK(avail = snd_pcm_avail_update(pcm_handle));
			while (avail >= static_cast< int >(period_size)) {
				stillRun = mix(outbuff.data(), static_cast< unsigned int >(period_size));
				if (stillRun) {
					snd_pcm_sframes_t w = 0;
					ALSA_ERRCHECK(w = snd_pcm_writei(pcm_handle, outbuff.data(), period_size));
					if (w < 0) {
						avail = w;
						break;
					}
				} else
					break;
				ALSA_ERRCHECK(avail = snd_pcm_avail_update(pcm_handle));
			}

			if (avail == -EPIPE) {
				snd_pcm_drain(pcm_handle);
				ALSA_ERRCHECK(snd_pcm_prepare(pcm_handle));
				for (unsigned int i = 0; i < buffer_size / period_size; ++i)
					ALSA_ERRCHECK(snd_pcm_writei(pcm_handle, zerobuff.data(), period_size));
			}

			if (!stillRun) {
				snd_pcm_drain(pcm_handle);

				while (bRunning && !mix(outbuff.data(), static_cast< unsigned int >(period_size))) {
					this->msleep(10);
				}

				if (!bRunning)
					break;

				snd_pcm_prepare(pcm_handle);

				// Fill one frame
				for (unsigned int i = 0; i < (buffer_size / period_size) - 1; i++)
					snd_pcm_writei(pcm_handle, zerobuff.data(), period_size);

				snd_pcm_writei(pcm_handle, outbuff.data(), period_size);
			}
		}
	}
	snd_pcm_close(pcm_handle);
}

#undef NBLOCKS
#undef ALSA_ERRBAIL
#undef ALSA_ERRCHECK
