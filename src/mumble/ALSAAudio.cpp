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

#include "ALSAAudio.h"
#include "Player.h"
#include "Global.h"
#include "MainWindow.h"
#include <sys/poll.h>

#define NBLOCKS 8

#define MAX(a,b)        ( (a) > (b) ? (a) : (b) )
#define MIN(a,b)        ( (a) < (b) ? (a) : (b) )

class ALSAEnumerator {
	public:
		QHash<QString,QString> qhInput;
		QHash<QString,QString> qhOutput;
		ALSAEnumerator();
};

static ALSAEnumerator *cards = NULL;

class ALSAAudioInit : public DeferInit {
		void initialize() {
			cards = new ALSAEnumerator();
		};
		void destroy() {
			delete cards;
			cards = NULL;
		};
};

static ALSAAudioInit aai;

class ALSAAudioInputRegistrar : public AudioInputRegistrar {
	public:
		ALSAAudioInputRegistrar();
		virtual AudioInput *create();
		virtual const QList<audioDevice> getDeviceChoices();
		virtual void setDeviceChoice(const QVariant &, Settings &);
		virtual bool canEcho(const QString &);
};


class ALSAAudioOutputRegistrar : public AudioOutputRegistrar {
	public:
		ALSAAudioOutputRegistrar();
		virtual AudioOutput *create();
		virtual const QList<audioDevice> getDeviceChoices();
		virtual void setDeviceChoice(const QVariant &, Settings &);
};

static ALSAAudioInputRegistrar airALSA;
static ALSAAudioOutputRegistrar aorALSA;

ALSAAudioInputRegistrar::ALSAAudioInputRegistrar() : AudioInputRegistrar(QLatin1String("ALSA")) {
}

AudioInput *ALSAAudioInputRegistrar::create() {
	return new ALSAAudioInput();
}

const QList<audioDevice> ALSAAudioInputRegistrar::getDeviceChoices() {
	QList<audioDevice> qlReturn;

	QStringList qlInputDevs = cards->qhInput.keys();
	qSort(qlInputDevs);

	if (qlInputDevs.contains(g.s.qsALSAInput)) {
		qlInputDevs.removeAll(g.s.qsALSAInput);
		qlInputDevs.prepend(g.s.qsALSAInput);
	}

	foreach(const QString &dev, qlInputDevs) {
		QString t=QString::fromLatin1("[%1] %2").arg(dev).arg(cards->qhInput.value(dev));
		qlReturn << audioDevice(t, dev);
	}

	return qlReturn;
}

void ALSAAudioInputRegistrar::setDeviceChoice(const QVariant &choice, Settings &s) {
	s.qsALSAInput = choice.toString();
}

bool ALSAAudioInputRegistrar::canEcho(const QString &) {
	return false;
}

ALSAAudioOutputRegistrar::ALSAAudioOutputRegistrar() : AudioOutputRegistrar(QLatin1String("ALSA")) {
}

AudioOutput *ALSAAudioOutputRegistrar::create() {
	return new ALSAAudioOutput();
}

const QList<audioDevice> ALSAAudioOutputRegistrar::getDeviceChoices() {
	QList<audioDevice> qlReturn;

	QStringList qlOutputDevs = cards->qhOutput.keys();
	qSort(qlOutputDevs);

	if (qlOutputDevs.contains(g.s.qsALSAOutput)) {
		qlOutputDevs.removeAll(g.s.qsALSAOutput);
		qlOutputDevs.prepend(g.s.qsALSAOutput);
	}

	foreach(const QString &dev, qlOutputDevs) {
		QString t=QString::fromLatin1("[%1] %2").arg(dev).arg(cards->qhOutput.value(dev));
		qlReturn << audioDevice(t, dev);
	}

	return qlReturn;
}

void ALSAAudioOutputRegistrar::setDeviceChoice(const QVariant &choice, Settings &s) {
	s.qsALSAOutput = choice.toString();
}

ALSAEnumerator::ALSAEnumerator() {
	qhInput.insert(QLatin1String("default"), ALSAAudioInput::tr("Default ALSA Card"));
	qhOutput.insert(QLatin1String("default"), ALSAAudioOutput::tr("Default ALSA Card"));

	int card=-1;
	snd_card_next(&card);
	while (card != -1) {
		char *name;
		snd_ctl_t *ctl=NULL;
		snd_card_get_longname(card, &name);
		QByteArray dev=QString::fromLatin1("hw:%1").arg(card).toUtf8();
		if (snd_ctl_open(&ctl, dev.data(), SND_CTL_READONLY) >= 0) {
			snd_pcm_info_t *info = NULL;
			snd_pcm_info_malloc(&info);

			char *cname = NULL;
			snd_card_get_name(card, &cname);

			int device = -1;
			snd_ctl_pcm_next_device(ctl, &device);

			bool play = false;
			bool cap = false;

			while (device != -1) {
				QString devname=QString::fromLatin1("hw:%1,%2").arg(card).arg(device);
				snd_pcm_info_set_device(info, device);
				snd_pcm_info_set_stream(info, SND_PCM_STREAM_CAPTURE);
				if (snd_ctl_pcm_info(ctl,info) == 0) {
					QString fname=QString::fromLatin1(snd_pcm_info_get_name(info));
					qhInput.insert(devname,fname);
					qhInput.insert(QString::fromLatin1("plug:\"")+devname+QString::fromLatin1("\""), fname);
					cap = true;
				}

				snd_pcm_info_set_stream(info, SND_PCM_STREAM_PLAYBACK);
				if (snd_ctl_pcm_info(ctl,info) == 0) {
					QString fname=QString::fromLatin1(snd_pcm_info_get_name(info));
					qhOutput.insert(devname,fname);
					qhOutput.insert(QString::fromLatin1("plug:\"")+devname+QString::fromLatin1("\""), fname);
					play = true;
				}

				snd_ctl_pcm_next_device(ctl, &device);
			}
			if (play) {
				qhOutput.insert(QString::fromLatin1("plug:\"dmix:CARD=%1\"").arg(card),QLatin1String(cname));
			}
			if (cap) {
				qhInput.insert(QString::fromLatin1("plug:\"dsnoop:CARD=%1\"").arg(card),QLatin1String(cname));
			}
			snd_pcm_info_free(info);
			snd_ctl_close(ctl);
		}
		snd_card_next(&card);
	}
}


ALSAAudioInput::ALSAAudioInput() {
	bRunning = true;
}

ALSAAudioInput::~ALSAAudioInput() {
	// Signal input thread to end
	bRunning = false;
	wait();
}

#define ALSA_ERRBAIL(x) if (!bOk) {} else if ((err=(x)) != 0) bOk = false

void ALSAAudioInput::run() {
	int readblapp;
	
	QByteArray device_name = g.s.qsALSAInput.toLatin1();
	snd_pcm_hw_params_t *hw_params = NULL;
	snd_pcm_t *capture_handle = NULL;


	unsigned int rrate = SAMPLE_RATE;
	bool bOk = true;

	int err = 0;
	
	unsigned int iChannels;

	qWarning("ALSAAudioInput: Initing audiocapture %s.",device_name.data());

	ALSA_ERRBAIL(snd_pcm_open(&capture_handle, device_name.data(), SND_PCM_STREAM_CAPTURE, 0));
	ALSA_ERRBAIL(snd_pcm_hw_params_malloc(&hw_params));
	ALSA_ERRBAIL(snd_pcm_hw_params_any(capture_handle, hw_params));
	ALSA_ERRBAIL(snd_pcm_hw_params_set_access(capture_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED));
	ALSA_ERRBAIL(snd_pcm_hw_params_set_format(capture_handle, hw_params, SND_PCM_FORMAT_S16_LE));
	ALSA_ERRBAIL(snd_pcm_hw_params_set_rate_min(capture_handle, hw_params, &rrate, NULL));
	ALSA_ERRBAIL(snd_pcm_hw_params_set_channels_near(capture_handle, hw_params, &iChannels));

	SpeexResamplerState *srs = NULL;
	if (rrate != SAMPLE_RATE) {
		srs = speex_resampler_init(1, rrate, SAMPLE_RATE, 3, &err);
	}

	unsigned int iInSize = (iFrameSize * rrate) / SAMPLE_RATE;

	snd_pcm_uframes_t wantPeriod = iInSize;
	snd_pcm_uframes_t wantBuff = wantPeriod * 8;

	ALSA_ERRBAIL(snd_pcm_hw_params_set_period_size_near(capture_handle, hw_params, &wantPeriod, NULL));
	ALSA_ERRBAIL(snd_pcm_hw_params_set_buffer_size_min(capture_handle, hw_params, &wantBuff));
	ALSA_ERRBAIL(snd_pcm_hw_params(capture_handle, hw_params));

	qWarning("ALSAAudioInput: Actual buffer %d hz, %d channel %ld samples [%ld(%d) per period]",rrate,iChannels,wantBuff,wantPeriod,iInSize);

#ifdef ALSA_VERBOSE
	snd_output_t *log;
	snd_output_stdio_attach(&log, stderr,0);
	if (capture_handle)
		snd_pcm_dump(capture_handle, log);
#endif

	ALSA_ERRBAIL(snd_pcm_prepare(capture_handle));
	ALSA_ERRBAIL(snd_pcm_start(capture_handle));

	if (hw_params)
		snd_pcm_hw_params_free(hw_params);

	if (! bOk) {
		if (capture_handle) {
			snd_pcm_drain(capture_handle);
			snd_pcm_close(capture_handle);
			capture_handle = NULL;
		}
		g.mw->msgBox(tr("Opening chosen ALSA Input failed: %1").arg(QLatin1String(snd_strerror(err))));
		return;
	}
	
	short inbuff[iInSize * iChannels];

	while (bRunning) {
#ifdef ALSA_VERBOSE
		snd_pcm_status_malloc(&status);
		snd_pcm_status(capture_handle, status);
		snd_pcm_status_dump(status, log);
		snd_pcm_status_free(status);
#endif
		readblapp = snd_pcm_readi(capture_handle, (srs || (iChannels>1)) ? inbuff : psMic, iInSize);
		if (readblapp == -ESTRPIPE) {
			// suspend event - what to do?
			qWarning("ALSAAudioInput: %s", snd_strerror(readblapp));
		} else if (readblapp == -EPIPE) {
			err = snd_pcm_prepare(capture_handle);
			qWarning("ALSAAudioInput: %s: %s", snd_strerror(readblapp), snd_strerror(err));
		} else if (readblapp < 0) {
			err = snd_pcm_prepare(capture_handle);
			qWarning("ALSAAudioInput: %s: %s", snd_strerror(readblapp), snd_strerror(err));
		} else if (iInSize == readblapp) {
			if (iChannels > 1) {
				for(int i=0;i<iInSize;++i) {
					int sum = 0;
					for(int j=0;j<iChannels;++j)
						sum += inbuff[i*iChannels+j];
					inbuff[i] = sum / iChannels;
				}
			}
			if (srs) {
				spx_uint32_t inlen = iInSize;
				spx_uint32_t outlen = iFrameSize;
				speex_resampler_process_int(srs, 0, inbuff, &inlen, psMic, &outlen);
			}
			encodeAudioFrame();
		}
	}

	snd_pcm_drain(capture_handle);
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
	snd_pcm_t *pcm_handle = NULL;
	struct pollfd fds[16];
	int count;
	bool stillRun = true;
	int err = 0;
	bool bOk = true;

	iMixerFreq = SAMPLE_RATE;
	iChannels = g.s.doPositionalAudio() ? 2 : 1;

	snd_pcm_hw_params_t *hw_params = NULL;
	snd_pcm_sw_params_t *sw_params = NULL;
	QByteArray device_name = g.s.qsALSAOutput.toLatin1();

	ALSA_ERRBAIL(snd_pcm_open(&pcm_handle, device_name.data(), SND_PCM_STREAM_PLAYBACK, 0));
	ALSA_ERRBAIL(snd_pcm_hw_params_malloc(&hw_params));
	ALSA_ERRBAIL(snd_pcm_sw_params_malloc(&sw_params));

	ALSA_ERRBAIL(snd_pcm_hw_params_any(pcm_handle, hw_params));
	ALSA_ERRBAIL(snd_pcm_hw_params_set_access(pcm_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED));
	ALSA_ERRBAIL(snd_pcm_hw_params_set_format(pcm_handle, hw_params, SND_PCM_FORMAT_S16_LE));
	ALSA_ERRBAIL(snd_pcm_hw_params_set_rate_min(pcm_handle, hw_params, &iMixerFreq, NULL));
	ALSA_ERRBAIL(snd_pcm_hw_params_set_channels_min(pcm_handle, hw_params, &iChannels));

	unsigned int iOutputSize = (iFrameSize * iMixerFreq) / SAMPLE_RATE;

	snd_pcm_uframes_t period_size = iOutputSize;
	snd_pcm_uframes_t buffer_size = iOutputSize * (g.s.iOutputDelay + 1);

	ALSA_ERRBAIL(snd_pcm_hw_params_set_period_size_near(pcm_handle, hw_params, &period_size, NULL));
	ALSA_ERRBAIL(snd_pcm_hw_params_set_buffer_size_min(pcm_handle, hw_params, &buffer_size));

	ALSA_ERRBAIL(snd_pcm_hw_params(pcm_handle, hw_params));
	ALSA_ERRBAIL(snd_pcm_hw_params_current(pcm_handle, hw_params));
	
	ALSA_ERRBAIL(snd_pcm_sw_params_current(pcm_handle, sw_params));
	ALSA_ERRBAIL(snd_pcm_sw_params_set_start_threshold(pcm_handle, sw_params, 0));
	ALSA_ERRBAIL(snd_pcm_sw_params_set_avail_min(pcm_handle, sw_params, period_size));

	ALSA_ERRBAIL(snd_pcm_sw_params(pcm_handle, sw_params));

	ALSA_ERRBAIL(snd_pcm_prepare(pcm_handle));

	const unsigned int buffsize = period_size * iChannels;

	short zerobuff[buffsize];
	short outbuff[buffsize];
	float mixbuff[buffsize];

	for (unsigned int i=0;i<buffsize;i++)
		zerobuff[i]=0;

	// Fill one frame
	if (bOk && pcm_handle)
		for (int i = 0; i < g.s.iOutputDelay + 1; i++)
			snd_pcm_writei(pcm_handle, zerobuff, period_size);

	if (sw_params)
		snd_pcm_sw_params_free(sw_params);
	if (hw_params)
		snd_pcm_hw_params_free(hw_params);

	if (! bOk) {
		g.mw->msgBox(tr("Opening chosen ALSA Output failed: %1").arg(QLatin1String(snd_strerror(err))));
		if (pcm_handle) {
			snd_pcm_close(pcm_handle);
			pcm_handle = NULL;
		}
		return;
	}
	
	const unsigned int chanmasks[] = {
		SPEAKER_FRONT_LEFT,
		SPEAKER_FRONT_RIGHT,
		SPEAKER_FRONT_CENTER,
		SPEAKER_LOW_FREQUENCY,
		SPEAKER_BACK_LEFT,
		SPEAKER_BACK_RIGHT,
		SPEAKER_SIDE_LEFT,
		SPEAKER_SIDE_RIGHT,
		SPEAKER_BACK_CENTER
	};
	
	// FIXME: This will crash with >7 channels!
	
	qWarning("ALSAAudioOutput: Initializing %d channel, %d hz mixer", iChannels, iMixerFreq);
	initializeMixer(chanmasks);

	count = snd_pcm_poll_descriptors_count(pcm_handle);
	snd_pcm_poll_descriptors(pcm_handle, fds, count);

	while (bRunning) {
		poll(fds, count, 20);
		unsigned short revents;

		snd_pcm_poll_descriptors_revents(pcm_handle, fds, count, &revents);
		if (revents & POLLERR) {
			snd_pcm_prepare(pcm_handle);
		} else if (revents & POLLOUT) {
			unsigned int avail = snd_pcm_avail_update(pcm_handle);
			while (avail >= iFrameSize) {
				stillRun = mix(mixbuff, period_size);
				for(int j=0;j<period_size*iChannels;++j)
					outbuff[j] = static_cast<short>(mixbuff[j] * 32767.f);
				int w=snd_pcm_writei(pcm_handle, outbuff, period_size);
				if (w == -EPIPE) {
					qWarning("ALSAAudioOutput: %s", snd_strerror(w));
					snd_pcm_prepare(pcm_handle);
					for (int i=0;i<g.s.iOutputDelay;i++)
						snd_pcm_writei(pcm_handle, zerobuff, period_size);
				}
				avail = snd_pcm_avail_update(pcm_handle);
			}

			if (! stillRun) {
				snd_pcm_drain(pcm_handle);
				
				while (bRunning && !mix(mixbuff, period_size))
					this->msleep(20);

				if (! bRunning)
					break;
					
				for(int j=0;j<period_size;++j)
					outbuff[j] = static_cast<short>(mixbuff[j] * 32767.f);

				snd_pcm_prepare(pcm_handle);

				// Fill one frame
				for (int i = 0; i < g.s.iOutputDelay-1; i++)
					snd_pcm_writei(pcm_handle, zerobuff, period_size);

				snd_pcm_writei(pcm_handle, outbuff, period_size);
			}
		}
	}
	snd_pcm_close(pcm_handle);
}
