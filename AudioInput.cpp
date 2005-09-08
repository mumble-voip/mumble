/* Copyright (C) 2005, Thorvald Natvig <thorvald@natvig.com>

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
#include "ServerHandler.h"
#include "Global.h"
#include "MainWindow.h"
#include "Player.h"

// Remember that we cannot use static member classes that are not pointers, as the constructor
// for AudioInputRegistrar() might be called before they are initialized, as the constructor
// is called from global initialization.
// Hence, we allocate upon first call.

QMap<QString, AudioInputRegistrarNew> *AudioInputRegistrar::qmNew;
QMap<QString, AudioInputRegistrarConfig> *AudioInputRegistrar::qmConfig;
QString AudioInputRegistrar::current = QString();

AudioInputRegistrar::AudioInputRegistrar(QString name, AudioInputRegistrarNew n, AudioInputRegistrarConfig c) {
	if (! qmNew)
		qmNew = new QMap<QString, AudioInputRegistrarNew>();
	if (! qmConfig)
		qmConfig = new QMap<QString, AudioInputRegistrarConfig>();
	qmNew->insert(name,n);
	qmConfig->insert(name,c);
}

AudioInput *AudioInputRegistrar::newFromChoice(QString choice) {
	QSettings qs;
	if (!choice.isEmpty() && qmNew->contains(choice)) {
		qs.setValue("AudioInputDevice", choice);
		current = choice;
		return qmNew->value(choice)();
	}
	choice = qs.value("AudioInputDevice").toString();
	if (qmNew->contains(choice)) {
		current = choice;
		return qmNew->value(choice)();
	}
	QMapIterator<QString, AudioInputRegistrarNew> i(*qmNew);
	if (i.hasNext()) {
		i.next();
		current = i.key();
		return i.value()();
	}
	return NULL;
}


int AudioInput::c_iFrameCounter = 0;

AudioInput::AudioInput()
{
	speex_bits_init(&sbBits);
	esEncState=speex_encoder_init(&speex_wb_mode);
	speex_encoder_ctl(esEncState,SPEEX_GET_FRAME_SIZE,&iFrameSize);

	iByteSize=iFrameSize * 2;

	int iarg=1;
	speex_encoder_ctl(esEncState,SPEEX_SET_VBR, &iarg);

	iarg = 0;

	speex_encoder_ctl(esEncState,SPEEX_SET_VAD, &iarg);
	speex_encoder_ctl(esEncState,SPEEX_SET_DTX, &iarg);

	bResetProcessor = true;

	sppPreprocess = NULL;
	psMic = new short[iFrameSize];

	bRunning = false;
}

AudioInput::~AudioInput()
{
	bRunning = false;
	wait();
	speex_bits_destroy(&sbBits);
	speex_encoder_destroy(esEncState);

	if (sppPreprocess)
		speex_preprocess_state_destroy(sppPreprocess);

	delete [] psMic;
}

void AudioInput::encodeAudioFrame() {
	int iArg;
	float fArg;
	int iLen;
	Player *p=Player::get(g.mw->sMyId);

	c_iFrameCounter++;

	if (! bRunning) {
		return;
	}

	if (bResetProcessor) {
		if (sppPreprocess)
			speex_preprocess_state_destroy(sppPreprocess);

		sppPreprocess = speex_preprocess_state_init(iFrameSize, SAMPLE_RATE);

		bResetProcessor = false;
	}

	fArg = 8;
	speex_encoder_ctl(esEncState,SPEEX_SET_VBR_QUALITY, &fArg);

	iArg = 5;
	speex_encoder_ctl(esEncState,SPEEX_SET_COMPLEXITY, &iArg);

	iArg = 1;
	speex_preprocess_ctl(sppPreprocess, SPEEX_PREPROCESS_SET_VAD, &iArg);
	speex_preprocess_ctl(sppPreprocess, SPEEX_PREPROCESS_SET_DENOISE, &iArg);
	speex_preprocess_ctl(sppPreprocess, SPEEX_PREPROCESS_SET_AGC, &iArg);
	speex_preprocess_ctl(sppPreprocess, SPEEX_PREPROCESS_SET_DEREVERB, &iArg);

	fArg = 20000;
	speex_preprocess_ctl(sppPreprocess, SPEEX_PREPROCESS_SET_AGC_LEVEL, &fArg);

	int iIsSpeech;
	iIsSpeech=speex_preprocess(sppPreprocess, psMic, NULL);

	if (sppPreprocess->loudness2 < 4000)
		sppPreprocess->loudness2 = 4000;

	if (g.s.atTransmit == Settings::PushToTalk)
		iIsSpeech = g.s.bPushToTalk;
	else if (g.s.atTransmit == Settings::Continous)
		iIsSpeech = 1;

	// Ideally, we'd like to go DTX (discontinous transmission)
	// if we didn't detect speech. Unfortunately, the jitter
	// buffer on the receiving end doesn't cope with that
	// very well.

	if (g.s.bMute || (p && p->bMute)) {
		if (p)
			p->setTalking(false);
		return;
	}

	if (! iIsSpeech) {
		// Zero frame -- we don't want comfort noise
		memset(psMic, 0, iByteSize);
	}

	if (p)
		p->setTalking(iIsSpeech);


	speex_bits_reset(&sbBits);
	speex_encode_int(esEncState, psMic, &sbBits);
	speex_bits_pack(&sbBits, (iIsSpeech) ? 1 : 0, 1);

	iLen=speex_bits_nbytes(&sbBits);
	QByteArray qbaPacket(iLen, 0);
	speex_bits_write(&sbBits, qbaPacket.data(), iLen);

	MessageSpeex msPacket;
	msPacket.qbaSpeexPacket = qbaPacket;
	msPacket.iSeq = c_iFrameCounter;
	if (g.sh)
		g.sh->sendMessage(&msPacket);
}
