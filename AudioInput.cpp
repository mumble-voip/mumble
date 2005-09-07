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
#include "Settings.h"
#include "MainWindow.h"
#include "Player.h"

AudioInput *g_aiInput;

// Remember that we cannot use static member classes that are not pointers, as the constructor
// for AudioOutputRegistrar() might be called before they are initialized, as the constructor
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
	speex_bits_init(&m_sbBits);
	m_esEncState=speex_encoder_init(&speex_wb_mode);
	speex_encoder_ctl(m_esEncState,SPEEX_GET_FRAME_SIZE,&m_iFrameSize);

	m_iByteSize=m_iFrameSize * 2;

	int iarg=1;
	speex_encoder_ctl(m_esEncState,SPEEX_SET_VBR, &iarg);

	iarg = 0;

	speex_encoder_ctl(m_esEncState,SPEEX_SET_VAD, &iarg);
	speex_encoder_ctl(m_esEncState,SPEEX_SET_DTX, &iarg);

	m_bResetProcessor = true;

	m_sppPreprocess = NULL;
	m_psMic = new short[m_iFrameSize];

	m_bRunning = false;
}

AudioInput::~AudioInput()
{
	m_bRunning = false;
	wait();
	speex_bits_destroy(&m_sbBits);
	speex_encoder_destroy(m_esEncState);

	if (m_sppPreprocess)
		speex_preprocess_state_destroy(m_sppPreprocess);

	delete [] m_psMic;
}

void AudioInput::encodeAudioFrame() {
	int iArg;
	float fArg;
	int iLen;
	Player *p=Player::get(g_mwMainWindow->m_sMyId);

	c_iFrameCounter++;

	if (! m_bRunning) {
		return;
	}

	if (m_bResetProcessor) {
		if (m_sppPreprocess)
			speex_preprocess_state_destroy(m_sppPreprocess);

		m_sppPreprocess = speex_preprocess_state_init(m_iFrameSize, SAMPLE_RATE);

		m_bResetProcessor = false;
	}

	fArg = 8;
	speex_encoder_ctl(m_esEncState,SPEEX_SET_VBR_QUALITY, &fArg);

	iArg = 5;
	speex_encoder_ctl(m_esEncState,SPEEX_SET_COMPLEXITY, &iArg);

	iArg = 1;
	speex_preprocess_ctl(m_sppPreprocess, SPEEX_PREPROCESS_SET_VAD, &iArg);
	speex_preprocess_ctl(m_sppPreprocess, SPEEX_PREPROCESS_SET_DENOISE, &iArg);
	speex_preprocess_ctl(m_sppPreprocess, SPEEX_PREPROCESS_SET_AGC, &iArg);
	speex_preprocess_ctl(m_sppPreprocess, SPEEX_PREPROCESS_SET_DEREVERB, &iArg);

	fArg = 20000;
	speex_preprocess_ctl(m_sppPreprocess, SPEEX_PREPROCESS_SET_AGC_LEVEL, &fArg);

	int iIsSpeech;
	iIsSpeech=speex_preprocess(m_sppPreprocess, m_psMic, NULL);

	if (m_sppPreprocess->loudness2 < 4000)
		m_sppPreprocess->loudness2 = 4000;

	if (g_s.atTransmit == Settings::PushToTalk)
		iIsSpeech = g_s.bPushToTalk;
	else if (g_s.atTransmit == Settings::Continous)
		iIsSpeech = 1;

	// Ideally, we'd like to go DTX (discontinous transmission)
	// if we didn't detect speech. Unfortunately, the jitter
	// buffer on the receiving end doesn't cope with that
	// very well.

	if (g_s.bMute || (p && p->m_bMute)) {
		if (p)
			p->setTalking(false);
		return;
	}

	if (! iIsSpeech) {
		// Zero frame -- we don't want comfort noise
		memset(m_psMic, 0, m_iByteSize);
	}

	if (p)
		p->setTalking(iIsSpeech);


	speex_bits_reset(&m_sbBits);
	speex_encode_int(m_esEncState, m_psMic, &m_sbBits);
	speex_bits_pack(&m_sbBits, (iIsSpeech) ? 1 : 0, 1);

	iLen=speex_bits_nbytes(&m_sbBits);
	QByteArray qbaPacket(iLen, 0);
	speex_bits_write(&m_sbBits, qbaPacket.data(), iLen);

	MessageSpeex msPacket;
	msPacket.m_qbaSpeexPacket = qbaPacket;
	msPacket.m_iSeq = c_iFrameCounter;
	if (g_shServer)
		g_shServer->sendMessage(&msPacket);
}
