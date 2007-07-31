/* Copyright (C) 2005-2007, Thorvald Natvig <thorvald@natvig.com>

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
}

AudioInputPtr AudioInputRegistrar::newFromChoice(QString choice) {
	QSettings qs;
	if (!choice.isEmpty() && qmNew->contains(choice)) {
		qs.setValue(QLatin1String("AudioInputDevice"), choice);
		current = choice;
		return AudioInputPtr(qmNew->value(current)->create());
	}
	choice = qs.value(QLatin1String("AudioInputDevice")).toString();
	if (qmNew->contains(choice)) {
		current = choice;
		return AudioInputPtr(qmNew->value(choice)->create());
	}

	// Try a sensible default. For example, ASIO is NOT a sensible default, but it's
	// pretty early in the sorted map.

	if (qmNew->contains(QLatin1String("DirectSound"))) {
		current = QLatin1String("DirectSound");
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

	iByteSize=iFrameSize * 2;

	iFrameCounter = 0;
	iSilentFrames = 0;
	iHoldFrames = 0;

	int iarg=1;
	speex_encoder_ctl(esEncState,SPEEX_SET_VBR, &iarg);

	iarg = 0;

	speex_encoder_ctl(esEncState,SPEEX_SET_VAD, &iarg);
	speex_encoder_ctl(esEncState,SPEEX_SET_DTX, &iarg);

	bResetProcessor = true;

	sppPreprocess = NULL;
	sesEcho = NULL;

	psMic = new short[iFrameSize];
	psSpeaker = new short[iFrameSize];
	psClean = new short[iFrameSize];
	pfY = new int[iFrameSize+1];

	bHasSpeaker = false;

	bPreviousVoice = false;

	iBitrate = 0;
	dPeakMic = dPeakSignal = dPeakSpeaker = 0.0;

	bRunning = false;
}

AudioInput::~AudioInput() {
	bRunning = false;
	wait();
	speex_bits_destroy(&sbBits);
	speex_encoder_destroy(esEncState);

	if (sppPreprocess)
		speex_preprocess_state_destroy(sppPreprocess);
	if (sesEcho)
		speex_echo_state_destroy(sesEcho);

	delete [] psMic;
	delete [] psSpeaker;
	delete [] psClean;
	delete [] pfY;
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
	audiorate += 20 + 8 + 3 + 2;

	// Subframe lengths
	if (g.s.iFramesPerPacket > 1)
		audiorate += g.s.iFramesPerPacket;

	if (g.s.bTransmitPosition)
		audiorate += 12;

	audiorate = (audiorate * 50) / g.s.iFramesPerPacket;

	return audiorate;
}

void AudioInput::encodeAudioFrame() {
	int iArg;
	float fArg;
	ClientPlayer *p=ClientPlayer::get(g.uiSession);
	short max;
	double micMax;
	int i;

	short *psSource;

	iFrameCounter++;

	if (! bRunning) {
		return;
	}

// #define ECHOTEST
#ifdef ECHOTEST

	static double framhist[10];

	for (int i=0;i<9;i++)
		framhist[i]=framhist[i+1];
	framhist[9]=1+(int)(310.0*rand()/(RAND_MAX+1.0));

	// Sine wave test
	for (i=0;i<iFrameSize;i++) {
		psMic[i]    += (sin((framhist[0] * M_PI * i) / (iFrameSize * 1.0)) * 4096.0);
		psSpeaker[i] = (sin((framhist[4] * M_PI * i) / (iFrameSize * 1.0)) * 8192.0);
//		psSpeaker[i] = psMic[i] * 2;
//		qWarning("%d %d", i, psMic[i]);
	}
#endif


	max=1;
	for (i=0;i<iFrameSize;i++)
		if (abs(psMic[i]) > max)
			max=abs(psMic[i]);
	dPeakMic=20.0*log10((max  * 1.0L) / 32768.0L);
	micMax = max;

	if (bHasSpeaker) {
		max=1;
		for (i=0;i<iFrameSize;i++)
			if (abs(psSpeaker[i]) > max)
				max=abs(psSpeaker[i]);
		dPeakSpeaker=20.0*log10((max  * 1.0L) / 32768.0L);
	} else {
		dPeakSpeaker = 0.0;
	}

	if (bResetProcessor) {
		if (sppPreprocess)
			speex_preprocess_state_destroy(sppPreprocess);

		sppPreprocess = speex_preprocess_state_init(iFrameSize, SAMPLE_RATE);

		iArg = 1;
		speex_preprocess_ctl(sppPreprocess, SPEEX_PREPROCESS_SET_VAD, &iArg);
		speex_preprocess_ctl(sppPreprocess, SPEEX_PREPROCESS_SET_DENOISE, &iArg);
		speex_preprocess_ctl(sppPreprocess, SPEEX_PREPROCESS_SET_AGC, &iArg);
		speex_preprocess_ctl(sppPreprocess, SPEEX_PREPROCESS_SET_DEREVERB, &iArg);

		fArg = 20000;
		speex_preprocess_ctl(sppPreprocess, SPEEX_PREPROCESS_SET_AGC_LEVEL, &fArg);

		double v = 20000.0 / g.s.iMinLoudness;
		iArg = lround(floor(20.0 * log10(v)));
		speex_preprocess_ctl(sppPreprocess, SPEEX_PREPROCESS_SET_AGC_MAX_GAIN, &iArg);

		iArg = -45;
		speex_preprocess_ctl(sppPreprocess, SPEEX_PREPROCESS_SET_NOISE_SUPPRESS, &iArg);

		if (bHasSpeaker) {
			if (sesEcho)
				speex_echo_state_destroy(sesEcho);
			sesEcho = speex_echo_state_init(iFrameSize, iFrameSize*20);
			iArg = SAMPLE_RATE;
			speex_echo_ctl(sesEcho, SPEEX_SET_SAMPLING_RATE, &iArg);
			qWarning("AudioInput: ECHO CANCELLER ACTIVE");
		}

		iFrames = 0;
		speex_bits_reset(&sbBits);

		bResetProcessor = false;
	}


	fArg = g.s.iQuality;
	speex_encoder_ctl(esEncState,SPEEX_SET_VBR_QUALITY, &fArg);

	iArg = g.s.iComplexity;
	speex_encoder_ctl(esEncState,SPEEX_SET_COMPLEXITY, &iArg);

	int iIsSpeech;

	if (bHasSpeaker) {
		speex_echo_cancel(sesEcho, psMic, psSpeaker, psClean, pfY);
		iIsSpeech=speex_preprocess(sppPreprocess, psClean, pfY);
		psSource = psClean;
	} else {
		iIsSpeech=speex_preprocess(sppPreprocess, psMic, NULL);
		psSource = psMic;
	}

	max=1;
	for (i=0;i<iFrameSize;i++)
		if (abs(psSource[i]) > max)
			max=abs(psSource[i]);
	dPeakSignal=20.0*log10((max  * 1.0L) / 32768.0L);


	CloneSpeexPreprocessState *st=reinterpret_cast<CloneSpeexPreprocessState *>(sppPreprocess);
	float Zframe = 0;
	int freq_start = static_cast<int>(300.0f*2.f*st->ps_size/st->sampling_rate);
	int freq_end   = static_cast<int>(2000.0f*2.f*st->ps_size/st->sampling_rate);
	for (int ii=freq_start;ii<freq_end;ii++)
		Zframe += st->zeta[ii];
	Zframe /= (freq_end-freq_start);
	dSNR = Zframe;

	double level = (g.s.vsVAD == Settings::SignalToNoise) ? dSNR / 32.767 : micMax / 32767.0;

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
		iIsSpeech = 0;

	iIsSpeech = iIsSpeech || (g.iPushToTalk > 0) || g.bAltSpeak;

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
		p->setTalking(iIsSpeech, g.bAltSpeak);

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

	if (g.s.bTransmitPosition && g.p && ! g.bCenterPosition && (iFrames == 0)) {
		g.p->fetch();
		if (g.p->bValid) {
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
	if (g.bAltSpeak)
		flags += MessageSpeex::AltSpeak;
	if (g.lmLoopMode == Global::Server)
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

	if (g.lmLoopMode == Global::Local) {
		LoopPlayer::lpLoopy.addFrame(qba, msPacket.iSeq);
	} else if (g.sh) {
		g.sh->sendMessage(&msPacket);
	}

	iFrames = 0;
	speex_bits_reset(&sbBits);
}
