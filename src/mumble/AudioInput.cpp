/* Copyright (C) 2005-2009, Thorvald Natvig <thorvald@natvig.com>

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

#include "AudioInput.h"
#include "AudioOutput.h"
#include "ServerHandler.h"
#include "MainWindow.h"
#include "User.h"
#include "Plugins.h"
#include "Message.h"
#include "Global.h"
#include "NetworkConfig.h"

// Remember that we cannot use static member classes that are not pointers, as the constructor
// for AudioInputRegistrar() might be called before they are initialized, as the constructor
// is called from global initialization.
// Hence, we allocate upon first call.

QMap<QString, AudioInputRegistrar *> *AudioInputRegistrar::qmNew;
QString AudioInputRegistrar::current = QString();

AudioInputRegistrar::AudioInputRegistrar(const QString &n, int p) : name(n), priority(p) {
	if (! qmNew)
		qmNew = new QMap<QString, AudioInputRegistrar *>();
	qmNew->insert(name,this);
}

AudioInputRegistrar::~AudioInputRegistrar() {
	qmNew->remove(name);
}

AudioInputPtr AudioInputRegistrar::newFromChoice(QString choice) {
	if (! qmNew)
		return AudioInputPtr();

	if (!choice.isEmpty() && qmNew->contains(choice)) {
		g.s.qsAudioInput = choice;
		current = choice;
		return AudioInputPtr(qmNew->value(current)->create());
	}
	choice = g.s.qsAudioInput;
	if (qmNew->contains(choice)) {
		current = choice;
		return AudioInputPtr(qmNew->value(choice)->create());
	}

	AudioInputRegistrar *r = NULL;
	foreach(AudioInputRegistrar *air, *qmNew)
		if (!r || (air->priority > r->priority))
			r = air;
	if (r) {
		current = r->name;
		return AudioInputPtr(r->create());
	}
	return AudioInputPtr();
}


AudioInput::AudioInput() {
	iFrameSize = SAMPLE_RATE / 100;
	cmMode = celt_mode_create(SAMPLE_RATE, 1, iFrameSize, NULL);
	ceEncoder = celt_encoder_create(cmMode);

	mumble_drft_init(&fftTable, iFrameSize);

	iFrameCounter = 0;
	iSilentFrames = 0;
	iHoldFrames = 0;

	bResetProcessor = true;

	bEchoMulti = false;

	sppPreprocess = NULL;
	sesEcho = NULL;
	srsMic = srsEcho = NULL;
	jb = jitter_buffer_init(10);
	iJitterSeq = 1;

	psMic = new short[iFrameSize];
	psClean = new short[iFrameSize];

	psSpeaker = NULL;

	iEchoChannels = iMicChannels = 0;
	iEchoFreq = iMicFreq = SAMPLE_RATE;
	iEchoFilled = iMicFilled = 0;
	eMicFormat = eEchoFormat = SampleFloat;
	iMicSampleSize = iEchoSampleSize = 0;

	bPreviousVoice = false;

	pfMicInput = pfEchoInput = pfOutput = NULL;

	iBitrate = 0;
	dPeakMic = dPeakSignal = dPeakSpeaker = 0.0;

	if (g.uiSession) {
		setMaxBandwidth(g.iMaxBandwidth);
	}

	bRunning = false;

	connect(this, SIGNAL(doMute()), g.mw->qaAudioMute, SLOT(trigger()), Qt::QueuedConnection);
}

AudioInput::~AudioInput() {
	bRunning = false;
	wait();

	celt_encoder_destroy(ceEncoder);
	celt_mode_destroy(cmMode);

	mumble_drft_clear(&fftTable);
	jitter_buffer_destroy(jb);

	if (sppPreprocess)
		speex_preprocess_state_destroy(sppPreprocess);
	if (sesEcho)
		speex_echo_state_destroy(sesEcho);

	if (srsMic)
		speex_resampler_destroy(srsMic);
	if (srsEcho)
		speex_resampler_destroy(srsEcho);

	delete [] psMic;
	delete [] psSpeaker;
	delete [] psClean;

	if (pfMicInput)
		delete [] pfMicInput;
	if (pfEchoInput)
		delete [] pfEchoInput;
	if (pfOutput)
		delete [] pfOutput;
}


#define IN_MIXER_FLOAT(channels) \
static void inMixerFloat##channels ( float * RESTRICT buffer, const void * RESTRICT ipt, unsigned int nsamp, unsigned int N) { \
  const float * RESTRICT input = reinterpret_cast<const float *>(ipt); \
  register const float m = 1.0f / static_cast<float>(channels); \
  Q_UNUSED(N); \
  for(unsigned int i=0;i<nsamp;++i) {\
	  register float v= 0.0f; \
	  for(unsigned int j=0;j<channels;++j) \
	  	v += input[i*channels+j]; \
	  buffer[i] = v * m; \
  } \
}

#define IN_MIXER_SHORT(channels) \
static void inMixerShort##channels ( float * RESTRICT buffer, const void * RESTRICT ipt, unsigned int nsamp, unsigned int N) { \
  const short * RESTRICT input = reinterpret_cast<const short *>(ipt); \
  register const float m = 1.0f / (32768.f * static_cast<float>(channels)); \
  Q_UNUSED(N); \
  for(unsigned int i=0;i<nsamp;++i) {\
	  register float v= 0.0f; \
	  for(unsigned int j=0;j<channels;++j) \
	  	v += static_cast<float>(input[i*channels+j]); \
	  buffer[i] = v * m; \
  } \
}

IN_MIXER_FLOAT(1)
IN_MIXER_FLOAT(2)
IN_MIXER_FLOAT(3)
IN_MIXER_FLOAT(4)
IN_MIXER_FLOAT(5)
IN_MIXER_FLOAT(6)
IN_MIXER_FLOAT(7)
IN_MIXER_FLOAT(8)
IN_MIXER_FLOAT(N)

IN_MIXER_SHORT(1)
IN_MIXER_SHORT(2)
IN_MIXER_SHORT(3)
IN_MIXER_SHORT(4)
IN_MIXER_SHORT(5)
IN_MIXER_SHORT(6)
IN_MIXER_SHORT(7)
IN_MIXER_SHORT(8)
IN_MIXER_SHORT(N)

AudioInput::inMixerFunc AudioInput::chooseMixer(const unsigned int nchan, SampleFormat sf) {
	inMixerFunc r = NULL;
	if (sf == SampleFloat) {
		switch (nchan) {
			case 1:
				r = inMixerFloat1;
				break;
			case 2:
				r = inMixerFloat2;
				break;
			case 3:
				r = inMixerFloat3;
				break;
			case 4:
				r = inMixerFloat4;
				break;
			case 5:
				r = inMixerFloat5;
				break;
			case 6:
				r = inMixerFloat6;
				break;
			case 7:
				r = inMixerFloat7;
				break;
			case 8:
				r = inMixerFloat8;
				break;
			default:
				r = inMixerFloatN;
				break;
		}
	} else {
		switch (nchan) {
			case 1:
				r = inMixerShort1;
				break;
			case 2:
				r = inMixerShort2;
				break;
			case 3:
				r = inMixerShort3;
				break;
			case 4:
				r = inMixerShort4;
				break;
			case 5:
				r = inMixerShort5;
				break;
			case 6:
				r = inMixerShort6;
				break;
			case 7:
				r = inMixerShort7;
				break;
			case 8:
				r = inMixerShort8;
				break;
			default:
				r = inMixerShortN;
				break;
		}
	}
	return r;
}

void AudioInput::initializeMixer() {
	int err;

	if (srsMic)
		speex_resampler_destroy(srsMic);
	if (srsEcho)
		speex_resampler_destroy(srsEcho);
	if (pfMicInput)
		delete [] pfMicInput;
	if (pfEchoInput)
		delete [] pfEchoInput;
	if (pfOutput)
		delete [] pfOutput;

	if (iMicFreq != SAMPLE_RATE)
		srsMic = speex_resampler_init(1, iMicFreq, SAMPLE_RATE, 3, &err);

	iMicLength = (iFrameSize * iMicFreq) / SAMPLE_RATE;

	pfMicInput = new float[iMicLength];
	pfOutput = new float[iFrameSize * qMax(1U,iEchoChannels)];

	if (iEchoChannels > 0) {
		psSpeaker = new short[iFrameSize * iEchoChannels];
		bEchoMulti = g.s.bEchoMulti;
		if (iEchoFreq != SAMPLE_RATE)
			srsEcho = speex_resampler_init(bEchoMulti ? iEchoChannels : 1, iEchoFreq, SAMPLE_RATE, 3, &err);
		iEchoLength = (iFrameSize * iEchoFreq) / SAMPLE_RATE;
		iEchoMCLength = bEchoMulti ? iEchoLength * iEchoChannels : iEchoLength;
		iEchoFrameSize = bEchoMulti ? iFrameSize * iEchoChannels : iFrameSize;
		pfEchoInput = new float[iEchoMCLength];
	} else {
		srsEcho = NULL;
		pfEchoInput = NULL;
	}

	imfMic = chooseMixer(iMicChannels, eMicFormat);
	imfEcho = chooseMixer(iEchoChannels, eEchoFormat);

	iMicSampleSize = static_cast<int>(iMicChannels * ((eMicFormat == SampleFloat) ? sizeof(float) : sizeof(short)));
	iEchoSampleSize = static_cast<int>(iEchoChannels * ((eEchoFormat == SampleFloat) ? sizeof(float) : sizeof(short)));

	bResetProcessor = true;

	qWarning("AudioInput: Initialized mixer for %d channel %d hz mic and %d channel %d hz echo", iMicChannels, iMicFreq, iEchoChannels, iEchoFreq);
}

void AudioInput::addMic(const void *data, unsigned int nsamp) {
	while (nsamp > 0) {
		unsigned int left = qMin(nsamp, iMicLength - iMicFilled);

		imfMic(pfMicInput + iMicFilled, data, left, iMicChannels);

		iMicFilled += left;
		nsamp -= left;

		if (nsamp > 0) {
			if (eMicFormat == SampleFloat)
				data = reinterpret_cast<const float *>(data) + left * iMicChannels;
			else
				data = reinterpret_cast<const short *>(data) + left * iMicChannels;
		}

		if (iMicFilled == iMicLength) {
			iMicFilled = 0;

			float *ptr = srsMic ? pfOutput : pfMicInput;
			if (srsMic) {
				spx_uint32_t inlen = iMicLength;
				spx_uint32_t outlen = iFrameSize;
				speex_resampler_process_float(srsMic, 0, pfMicInput, &inlen, pfOutput, &outlen);
			}
			const float mul = 32768.f;
			for (int j=0;j<iFrameSize;++j)
				psMic[j] = static_cast<short>(ptr[j] * mul);

			if (iEchoChannels > 0) {
				JitterBufferPacket jbp;
				jbp.data = reinterpret_cast<char *>(psSpeaker);
				jbp.len = static_cast<int>(iEchoFrameSize * sizeof(short));
				jbp.timestamp = 0;
				jbp.span = 0;
				jbp.sequence = 0;
				jbp.user_data = 0;

				spx_int32_t offs;

				jitter_buffer_get(jb, &jbp, 10, &offs);
				jitter_buffer_tick(jb);
			}
			encodeAudioFrame();
		}
	}
}

void AudioInput::addEcho(const void *data, unsigned int nsamp) {
	while (nsamp > 0) {
		unsigned int left = qMin(nsamp, iEchoLength - iEchoFilled);

		if (bEchoMulti) {
			if (eEchoFormat == SampleFloat)
				for (unsigned int i=0;i<iEchoMCLength;++i)
					pfEchoInput[i] = reinterpret_cast<const float *>(data)[i];
			else
				for (unsigned int i=0;i<iEchoMCLength;++i)
					pfEchoInput[i] = static_cast<float>(reinterpret_cast<const short *>(data)[i]) * (1.0f / 32768.f);
		} else {
			imfEcho(pfEchoInput + iEchoFilled, data, left, iEchoChannels);
		}

		iEchoFilled += left;
		nsamp -= left;

		if (nsamp > 0) {
			if (eEchoFormat == SampleFloat)
				data = reinterpret_cast<const float *>(data) + left * iEchoChannels;
			else
				data = reinterpret_cast<const short *>(data) + left * iEchoChannels;
		}

		if (iEchoFilled == iEchoLength) {
			iEchoFilled = 0;

			STACKVAR(short, outbuff, iEchoFrameSize);
			float *ptr = srsEcho ? pfOutput : pfEchoInput;
			if (srsEcho) {
				spx_uint32_t inlen = iEchoLength;
				spx_uint32_t outlen = iFrameSize;
				speex_resampler_process_interleaved_float(srsEcho, pfEchoInput, &inlen, pfOutput, &outlen);
			}
			const float mul = 32768.f;
			for (unsigned int j=0;j<iEchoFrameSize;++j)
				outbuff[j] = static_cast<short>(ptr[j] * mul);

			JitterBufferPacket jbp;
			jbp.data = reinterpret_cast<char *>(outbuff);
			jbp.len = static_cast<int>(iEchoFrameSize * sizeof(short));
			jbp.timestamp = ++iJitterSeq * 10;
			jbp.span = 10;
			jbp.sequence = static_cast<unsigned short>(iJitterSeq);
			jbp.user_data = 0;

			jitter_buffer_put(jb, &jbp);
		}
	}
}

int AudioInput::getMaxBandwidth() {
	int audiorate;

	audiorate = 1000;

	audiorate /= 400/g.s.iFramesPerPacket;

	// Overhead
	audiorate += 20 + 8 + 4 + 3 + 1 + 2;

	if (g.s.bTransmitPosition)
		audiorate += 12;

	if (NetworkConfig::TcpModeEnabled())
		audiorate += 12;

	audiorate = (audiorate * 50) / g.s.iFramesPerPacket;

	return audiorate;
}

void AudioInput::setMaxBandwidth(int bytespersec) {
	int audiorate;
	int baserate;

	float f = static_cast<float>(g.s.iQuality);

	do {
		// FIXME: Autoreduce quality
		baserate = 100;
		audiorate = baserate;

		audiorate /= 400/g.s.iFramesPerPacket;

		// Overhead
		audiorate += 20 + 8 + 4 + 3 + 1 + 2;

		if (g.s.bTransmitPosition)
			audiorate += 12;

		if (NetworkConfig::TcpModeEnabled())
			audiorate += 12;

		audiorate = (audiorate * 50) / g.s.iFramesPerPacket;

		if (f <= 1.9)
			break;

		if (audiorate > bytespersec) {
			f -= 1.0f;
		}
	} while (audiorate > bytespersec);

	g.iAudioBandwidth = audiorate;
	g.iAudioQuality = iroundf(f);
}


/*
 * FIXME: Experimental zero-latency callback-based preprocessing.

extern "C" {
	int speex_preprocess_run_cb(SpeexPreprocessState *st, float *ft);
	SpeexPreprocessState *speex_preprocess_callback_init(int frame_size, int sampling_rate);
};

celt_int32_t celtBack(CELTEncoder *enc, void *rawdata, celt_int32_t format, celt_int32_t bits, celt_int32_t num, void *data) {
	qWarning() << "CB" << enc << rawdata << format << bits << num << data;

	static SpeexPreprocessState *st = NULL;
	if (! st) {
		int samp = 480;
		int freq = 48000;

		celt_encoder_ctl(enc, CELT_GET_SAMPLE_RATE, &freq);
		celt_encoder_ctl(enc, CELT_GET_FRAME_SIZE, &samp);

		qWarning() << "Init" << samp << freq;
		st= speex_preprocess_callback_init(samp, freq);

		int iArg;

		iArg = 1;
		speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_VAD, &iArg);
		speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_AGC, &iArg);
		speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_DENOISE, &iArg);
		speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_DEREVERB, &iArg);

		iArg = 30000;
		speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_AGC_TARGET, &iArg);

		iArg = 30000;
		speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_AGC_TARGET, &iArg);

		float v = 30000.0f / static_cast<float>(g.s.iMinLoudness);
		iArg = iroundf(floorf(20.0f * log10f(v)));
		speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_AGC_MAX_GAIN, &iArg);

		iArg = g.s.iNoiseSuppress;
		speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_NOISE_SUPPRESS, &iArg);
	}
	qWarning() << "Call";
//	int res = 1;
	int res = speex_preprocess_run_cb(st, (float *) rawdata);
	qWarning() << "Result" << res;
	return 1;
}
 */

void AudioInput::encodeAudioFrame() {
	int iArg;
	ClientUser *p=ClientUser::get(g.uiSession);
	int i;
	float sum;
	short max;

	short *psSource;

	iFrameCounter++;

	if (! bRunning) {
		return;
	}

	sum=1.0f;
	for (i=0;i<iFrameSize;i++)
		sum += static_cast<float>(psMic[i] * psMic[i]);
	dPeakMic=20.0f*log10f(sqrtf(sum / static_cast<float>(iFrameSize)) / 32768.0f);
	if (dPeakMic < -96.0f)
		dPeakMic = -96.0f;

	max = 1;
	for (i=0;i<iFrameSize;i++)
		max = static_cast<short>(abs(psMic[i]) > max ? abs(psMic[i]) : max);
	dMaxMic = max;

	if (iEchoChannels > 0) {
		sum=1.0f;
		for (i=0;i<iFrameSize;i++)
			sum += static_cast<float>(psSpeaker[i] * psSpeaker[i]);
		dPeakSpeaker=20.0f*log10f(sqrtf(sum / static_cast<float>(iFrameSize)) / 32768.0f);
		if (dPeakSpeaker < -96.0f)
			dPeakSpeaker = -96.0f;
	} else {
		dPeakSpeaker = 0.0;
	}

	QMutexLocker l(&qmSpeex);

	if (bResetProcessor) {
		if (sppPreprocess)
			speex_preprocess_state_destroy(sppPreprocess);
		if (sesEcho)
			speex_echo_state_destroy(sesEcho);

		sppPreprocess = speex_preprocess_state_init(iFrameSize, SAMPLE_RATE);

		iArg = 1;
		speex_preprocess_ctl(sppPreprocess, SPEEX_PREPROCESS_SET_VAD, &iArg);
		speex_preprocess_ctl(sppPreprocess, SPEEX_PREPROCESS_SET_AGC, &iArg);
		speex_preprocess_ctl(sppPreprocess, SPEEX_PREPROCESS_SET_DENOISE, &iArg);
		speex_preprocess_ctl(sppPreprocess, SPEEX_PREPROCESS_SET_DEREVERB, &iArg);

		iArg = 30000;
		speex_preprocess_ctl(sppPreprocess, SPEEX_PREPROCESS_SET_AGC_TARGET, &iArg);

		float v = 30000.0f / static_cast<float>(g.s.iMinLoudness);
		iArg = iroundf(floorf(20.0f * log10f(v)));
		speex_preprocess_ctl(sppPreprocess, SPEEX_PREPROCESS_SET_AGC_MAX_GAIN, &iArg);

		iArg = g.s.iNoiseSuppress;
		speex_preprocess_ctl(sppPreprocess, SPEEX_PREPROCESS_SET_NOISE_SUPPRESS, &iArg);

		if (iEchoChannels > 0) {
			sesEcho = speex_echo_state_init_mc(iFrameSize, iFrameSize*10, 1, bEchoMulti ? iEchoChannels : 1);
			iArg = SAMPLE_RATE;
			speex_echo_ctl(sesEcho, SPEEX_ECHO_SET_SAMPLING_RATE, &iArg);
			speex_preprocess_ctl(sppPreprocess, SPEEX_PREPROCESS_SET_ECHO_STATE, sesEcho);

			jitter_buffer_reset(jb);
			qWarning("AudioInput: ECHO CANCELLER ACTIVE");
		} else {
			sesEcho = NULL;
		}

		bResetProcessor = false;
	}

	int iIsSpeech;

	if (sesEcho) {
		speex_echo_cancellation(sesEcho, psMic, psSpeaker, psClean);
		iIsSpeech=speex_preprocess_run(sppPreprocess, psClean);
		psSource = psClean;
	} else {
		iIsSpeech=speex_preprocess_run(sppPreprocess, psMic);
		psSource = psMic;
	}

	sum=1.0f;
	for (i=0;i<iFrameSize;i++)
		sum += static_cast<float>(psSource[i] * psSource[i]);
	float micLevel = sqrtf(sum / static_cast<float>(iFrameSize));
	dPeakSignal=20.0f*log10f(micLevel / 32768.0f);
	if (dPeakSignal < -96.0f)
		dPeakSignal = -96.0f;

	spx_int32_t prob = 0;
	speex_preprocess_ctl(sppPreprocess, SPEEX_PREPROCESS_GET_PROB, &prob);
	fSpeechProb = static_cast<float>(prob) / 100.0f;

	float level = (g.s.vsVAD == Settings::SignalToNoise) ? fSpeechProb : (1.0f + dPeakMic / 96.0f);

	if (level > g.s.fVADmax)
		iIsSpeech = 1;
	else if (level > g.s.fVADmin && bPreviousVoice)
		iIsSpeech = 1;
	else
		iIsSpeech = 0;

	if (! iIsSpeech) {
		iHoldFrames++;
		if (iHoldFrames < g.s.iVoiceHold)
			iIsSpeech=1;
	} else {
		iHoldFrames = 0;
	}

	if (g.s.atTransmit == Settings::Continous)
		iIsSpeech = 1;
	else if (g.s.atTransmit == Settings::PushToTalk)
		iIsSpeech = g.s.uiDoublePush && ((g.uiDoublePush < g.s.uiDoublePush) || (g.tDoublePush.elapsed() < g.s.uiDoublePush));

	iIsSpeech = iIsSpeech || (g.iPushToTalk > 0);

	if (g.s.bMute || ((g.s.lmLoopMode != Settings::Local) && p && p->bMute) || g.bPushToMute || (g.iTarget < 0)) {
		iIsSpeech = 0;
	}

	if (iIsSpeech) {
		iSilentFrames = 0;
	} else {
		iSilentFrames++;
		if (iSilentFrames > 500)
			iFrameCounter = 0;
	}

	if (p)
		p->setTalking(iIsSpeech, (g.iTarget != 0));

	if (g.s.bPushClick && (g.s.atTransmit == Settings::PushToTalk)) {
		AudioOutputPtr ao = g.ao;
		if (iIsSpeech && ! bPreviousVoice && ao)
			ao->playSine(400.0f,1200.0f,5);
		else if (ao && !iIsSpeech && bPreviousVoice && ao)
			ao->playSine(620.0f,-1200.0f,5);
	}
	if (! iIsSpeech && ! bPreviousVoice) {
		iBitrate = 0;
		if (g.s.iIdleTime && ! g.s.bMute && ((tIdle.elapsed() / 1000000ULL) > g.s.iIdleTime)) {
			emit doMute();
			tIdle.restart();
		}
		return;
	}

	tIdle.restart();
/*
	int r = celt_encoder_ctl(ceEncoder, CELT_SET_POST_MDCT_CALLBACK(celtBack, NULL));
	qWarning() << "Set Callback" << r;
*/
	if (! iIsSpeech) {
		flushCheck(QByteArray());
		iBitrate = 0;
	} else {
		unsigned char buffer[512];
		int len = celt_encode(ceEncoder, psSource, NULL, buffer, 50);
		iBitrate = len * 100 * 8;

		flushCheck(QByteArray(reinterpret_cast<const char *>(buffer), len));
	}

	bPreviousVoice = iIsSpeech;
}

void AudioInput::flushCheck(const QByteArray &qba) {
	qlFrames << qba;
	if (! qba.isEmpty() && qlFrames.count() < g.s.iFramesPerPacket)
		return;

	int flags = g.iTarget;
	if (g.s.lmLoopMode == Settings::Server)
		flags = 0x1f;

	char data[1024];
	data[0] = static_cast<unsigned char>(flags);

	PacketDataStream pds(data + 1, 1023);
	pds << iFrameCounter;

	for (int i=0;i<qlFrames.count(); ++i) {
		const QByteArray &qba = qlFrames.at(i);
		unsigned char head = qba.size();
		if (i < qlFrames.count() - 1)
			head |= 0x80;
		pds.append(head);
		pds.append(qba.constData(), qba.size());
	}

	if (! bPreviousVoice)
		pds.append(0x00);

	if (g.s.bTransmitPosition && g.p && ! g.bCenterPosition && g.p->fetch()) {
		pds << g.p->fPosition[0];
		pds << g.p->fPosition[1];
		pds << g.p->fPosition[2];
	}

	if (g.s.lmLoopMode == Settings::Local)
		LoopUser::lpLoopy.addFrame(QByteArray(data, pds.size() + 1));
	else if (g.sh)
		g.sh->sendMessage(data, pds.size() + 1);

	qlFrames.clear();
}
