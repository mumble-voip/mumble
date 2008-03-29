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

#include "AudioInput.h"
#include "AudioOutput.h"
#include "ServerHandler.h"
#include "MainWindow.h"
#include "Player.h"
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

AudioInputRegistrar::AudioInputRegistrar(const QString &n) : name(n) {
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

	// Try a sensible default. For example, ASIO is NOT a sensible default, but it's
	// pretty early in the sorted map.

	if (qmNew->contains(QLatin1String(DEFAULT_SOUNDSYS))) {
		current = QLatin1String(DEFAULT_SOUNDSYS);
		return AudioInputPtr(qmNew->value(current)->create());
	}

	QMapIterator<QString, AudioInputRegistrar *> i(*qmNew);
	if (i.hasNext()) {
		i.next();
		current = i.key();
		return AudioInputPtr(i.value()->create());
	}
	return AudioInputPtr();
}


AudioInput::AudioInput() {
	speex_bits_init(&sbBits);
	speex_bits_reset(&sbBits);
	iFrames = 0;

	esEncState=speex_encoder_init(&speex_wb_mode);
	speex_encoder_ctl(esEncState,SPEEX_GET_FRAME_SIZE,&iFrameSize);

	mumble_drft_init(&fftTable, iFrameSize);

	iByteSize=iFrameSize * 2;

	iFrameCounter = 0;
	iSilentFrames = 0;
	iHoldFrames = 0;
	iBestBin = 0;

	int iArg=1;
	float fArg=0.0;
	speex_encoder_ctl(esEncState,SPEEX_SET_VBR, &iArg);

	iArg = 0;

	speex_encoder_ctl(esEncState,SPEEX_SET_VAD, &iArg);
	speex_encoder_ctl(esEncState,SPEEX_SET_DTX, &iArg);

	fArg = g.s.iQuality;
	speex_encoder_ctl(esEncState,SPEEX_SET_VBR_QUALITY, &fArg);
	speex_encoder_ctl(esEncState,SPEEX_GET_BITRATE,&iArg);
	speex_encoder_ctl(esEncState, SPEEX_SET_VBR_MAX_BITRATE, &iArg);


	iArg = g.s.iComplexity;
	speex_encoder_ctl(esEncState,SPEEX_SET_COMPLEXITY, &iArg);

	bResetProcessor = true;

	sppPreprocess = NULL;
	sesEcho = NULL;

	psMic = new short[iFrameSize];
	psSpeaker = new short[iFrameSize];
	psClean = new short[iFrameSize];

	bHasSpeaker = false;

	bPreviousVoice = false;

	iBitrate = 0;
	dPeakMic = dPeakSignal = dPeakSpeaker = 0.0;

	if (g.uiSession) {
		setMaxBandwidth(g.iMaxBandwidth);
	}

	bRunning = false;
}

AudioInput::~AudioInput() {
	bRunning = false;
	wait();
	speex_bits_destroy(&sbBits);
	speex_encoder_destroy(esEncState);
	mumble_drft_clear(&fftTable);

	if (sppPreprocess)
		speex_preprocess_state_destroy(sppPreprocess);
	if (sesEcho)
		speex_echo_state_destroy(sesEcho);

	delete [] psMic;
	delete [] psSpeaker;
	delete [] psClean;
}

int AudioInput::getMaxBandwidth() {
	int audiorate;

	void *es;
	float f = g.s.iQuality;
	es = speex_encoder_init(&speex_wb_mode);
	speex_encoder_ctl(es,SPEEX_SET_VBR_QUALITY, &f);
	speex_encoder_ctl(es,SPEEX_GET_BITRATE,&audiorate);
	speex_encoder_destroy(es);

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

	void *es;
	es = speex_encoder_init(&speex_wb_mode);

	float f = g.s.iQuality;

	do {
		speex_encoder_ctl(es, SPEEX_SET_VBR_QUALITY, &f);
		speex_encoder_ctl(es, SPEEX_GET_BITRATE, &baserate);
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
			f -= 1.0;
		}
	} while (audiorate > bytespersec);

	speex_encoder_destroy(es);

	speex_encoder_ctl(esEncState, SPEEX_SET_VBR_QUALITY, &f);
	speex_encoder_ctl(esEncState, SPEEX_SET_VBR_MAX_BITRATE, &baserate);

	g.iAudioBandwidth = audiorate;
	g.iAudioQuality = lroundf(f);
}

void AudioInput::encodeAudioFrame() {
	int iArg;
	ClientPlayer *p=ClientPlayer::get(g.uiSession);
	short max;
	float micMax;
	int i;

	short *psSource;

	iFrameCounter++;

	if (! bRunning) {
		return;
	}

	max=1;
	for (i=0;i<iFrameSize;i++)
		if (abs(psMic[i]) > max)
			max=abs(psMic[i]);
	dPeakMic=20.0f*log10f((max  * 1.0f) / 32768.0f);
	micMax = max;

	if (g.bEchoTest) {
		STACKVAR(float, fft, iFrameSize);
		STACKVAR(float, power, iFrameSize);
		float scale = 1.f / iFrameSize;
		for (i=0;i<iFrameSize;i++)
			fft[i] = psMic[i] * scale;
		mumble_drft_forward(&fftTable, fft);
		float mp = 0.0;
		int bin = 0;
		power[0]=power[1]=0.0;
		for (i=2;i < iFrameSize / 2;i++) {
			power[i] = sqrtf(fft[2*i]*fft[2*i]+fft[2*i-1]*fft[2*i-1]);
			if (power[i] > mp) {
				bin = i;
				mp = power[i];
			}
		}
		for (i=2;i< iFrameSize / 2;i++) {
			if (power[i] * 2 > mp) {
				if (i != bin)
					bin = 0;
			}
		}
		iBestBin = bin * 2;
	}

	if (bHasSpeaker) {
		max=1;
		for (i=0;i<iFrameSize;i++)
			if (abs(psSpeaker[i]) > max)
				max=abs(psSpeaker[i]);
		dPeakSpeaker=20.0f*log10f((max  * 1.0f) / 32768.0f);
	} else {
		dPeakSpeaker = 0.0;
	}

	QMutexLocker l(&qmSpeex);

	if (bResetProcessor) {
		if (sppPreprocess)
			speex_preprocess_state_destroy(sppPreprocess);

		sppPreprocess = speex_preprocess_state_init(iFrameSize, SAMPLE_RATE);

		iArg = 1;
		speex_preprocess_ctl(sppPreprocess, SPEEX_PREPROCESS_SET_VAD, &iArg);
		speex_preprocess_ctl(sppPreprocess, SPEEX_PREPROCESS_SET_DENOISE, &iArg);
		speex_preprocess_ctl(sppPreprocess, SPEEX_PREPROCESS_SET_AGC, &iArg);
		speex_preprocess_ctl(sppPreprocess, SPEEX_PREPROCESS_SET_DEREVERB, &iArg);

		iArg = 30000;
		speex_preprocess_ctl(sppPreprocess, SPEEX_PREPROCESS_SET_AGC_LEVEL, &iArg);

		float v = 30000.0f / g.s.iMinLoudness;
		iArg = lroundf(floorf(20.0f * log10f(v)));
		speex_preprocess_ctl(sppPreprocess, SPEEX_PREPROCESS_SET_AGC_MAX_GAIN, &iArg);

		iArg = g.s.iNoiseSuppress;
		speex_preprocess_ctl(sppPreprocess, SPEEX_PREPROCESS_SET_NOISE_SUPPRESS, &iArg);

		if (bHasSpeaker) {
			if (sesEcho)
				speex_echo_state_destroy(sesEcho);
			sesEcho = speex_echo_state_init(iFrameSize, iFrameSize*10);
			iArg = SAMPLE_RATE;
			speex_echo_ctl(sesEcho, SPEEX_SET_SAMPLING_RATE, &iArg);
			speex_preprocess_ctl(sppPreprocess, SPEEX_PREPROCESS_SET_ECHO_STATE, sesEcho);
			qWarning("AudioInput: ECHO CANCELLER ACTIVE");
		}

		iFrames = 0;
		speex_bits_reset(&sbBits);

		bResetProcessor = false;
	}

	int iIsSpeech;

	if (bHasSpeaker) {
		speex_echo_cancellation(sesEcho, psMic, psSpeaker, psClean);
		iIsSpeech=speex_preprocess_run(sppPreprocess, psClean);
		psSource = psClean;
	} else {
		iIsSpeech=speex_preprocess_run(sppPreprocess, psMic);
		psSource = psMic;
	}

	max=1;
	for (i=0;i<iFrameSize;i++)
		if (abs(psSource[i]) > max)
			max=abs(psSource[i]);
	dPeakSignal=20.0*log10f((max  * 1.0f) / 32768.0f);

	spx_int32_t prob = 0;
	speex_preprocess_ctl(sppPreprocess, SPEEX_PREPROCESS_GET_PROB, &prob);
	fSpeechProb = prob / 100.0;

	float level = (g.s.vsVAD == Settings::SignalToNoise) ? fSpeechProb : (micMax / 32767.0f);

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

	iIsSpeech = iIsSpeech || (g.iPushToTalk > 0) || (g.iAltSpeak > 0);

	if (g.s.bMute || (p && p->bMute) || g.bPushToMute) {
		iIsSpeech = 0;
	}

	if (iIsSpeech) {
		iSilentFrames = 0;
	} else {
		iSilentFrames++;
		if (iSilentFrames > 200)
			iFrameCounter = 0;
	}

	if (p)
		p->setTalking(iIsSpeech, (g.iAltSpeak > 0));

	if (g.s.bPushClick && (g.s.atTransmit == Settings::PushToTalk)) {
		AudioOutputPtr ao = g.ao;
		if (iIsSpeech && ! bPreviousVoice && ao)
			ao->playSine(400,1200,5);
		else if (ao && !iIsSpeech && bPreviousVoice && ao)
			ao->playSine(620,-1200,5);
	}
	if (! iIsSpeech && ! bPreviousVoice) {
		iBitrate = 0;
		return;
	}

	bPreviousVoice = iIsSpeech;

	if (! iIsSpeech) {
		memset(psMic, 0, iByteSize);
	}

	if (g.s.bTransmitPosition && g.p && ! g.bCenterPosition && (iFrames == 0) && g.p->fetch()) {
		QByteArray q;
		QDataStream ds(&q, QIODevice::WriteOnly);
		ds << g.p->fPosition[0];
		ds << g.p->fPosition[1];
		ds << g.p->fPosition[2];

		speex_bits_pack(&sbBits, 13, 5);
		speex_bits_pack(&sbBits, q.size(), 4);

		const unsigned char *d=reinterpret_cast<const unsigned char*>(q.data());
		for (i=0;i<q.size();i++) {
			speex_bits_pack(&sbBits, d[i], 8);
		}
	}

	speex_encode_int(esEncState, psSource, &sbBits);
	iFrames++;

	speex_encoder_ctl(esEncState, SPEEX_GET_BITRATE, &iBitrate);

	flushCheck();
}

void AudioInput::flushCheck() {
	if (bPreviousVoice && iFrames < g.s.iFramesPerPacket)
		return;

	unsigned char flags = 0;
	if (g.iAltSpeak > 0)
		flags += MessageSpeex::AltSpeak;
	if (g.s.lmLoopMode == Settings::Server)
		flags += MessageSpeex::LoopBack;

	if (! bPreviousVoice)
		flags += MessageSpeex::EndSpeech;

	flags += (iFrames - 1) << 4;

	int len = speex_bits_nbytes(&sbBits);
	QByteArray qba(len + 1, 0);
	qba[0] = flags;

	speex_bits_write(&sbBits, qba.data() + 1, len);

	MessageSpeex msPacket;
	msPacket.qbaSpeexPacket = qba;
	msPacket.iSeq = iFrameCounter;

	if (g.s.lmLoopMode == Settings::Local) {
		LoopPlayer::lpLoopy.addFrame(qba, msPacket.iSeq);
	} else if (g.sh) {
		g.sh->sendMessage(&msPacket);
	}

	iFrames = 0;
	speex_bits_reset(&sbBits);
}
