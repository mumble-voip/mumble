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

#include "AudioOutput.h"
#include "Player.h"
#include "Settings.h"
#include <QSettings>

AudioOutput *g_aoOutput;

// Remember that we cannot use static member classes that are not pointers, as the constructor
// for AudioOutputRegistrar() might be called before they are initialized, as the constructor
// is called from global initialization.
// Hence, we allocate upon first call.

QMap<QString, AudioOutputRegistrarNew> *AudioOutputRegistrar::qmNew;
QMap<QString, AudioOutputRegistrarConfig> *AudioOutputRegistrar::qmConfig;
QString AudioOutputRegistrar::current = QString();

AudioOutputRegistrar::AudioOutputRegistrar(QString name, AudioOutputRegistrarNew n, AudioOutputRegistrarConfig c) {
	if (! qmNew)
		qmNew = new QMap<QString, AudioOutputRegistrarNew>();
	if (! qmConfig)
		qmConfig = new QMap<QString, AudioOutputRegistrarConfig>();
	qmNew->insert(name,n);
	qmConfig->insert(name,c);
}

AudioOutput *AudioOutputRegistrar::newFromChoice(QString choice) {
	QSettings qs;
	if (!choice.isEmpty() && qmNew->contains(choice)) {
		qs.setValue("AudioOutputDevice", choice);
		current = choice;
		return qmNew->value(choice)();
	}
	choice = qs.value("AudioOutputDevice").toString();
	if (qmNew->contains(choice)) {
		current = choice;
		return qmNew->value(choice)();
	}
	QMapIterator<QString, AudioOutputRegistrarNew> i(*qmNew);
	if (i.hasNext()) {
		i.next();
		current = i.key();
		return i.value()();
	}
	return NULL;
}

AudioOutputPlayer::AudioOutputPlayer(AudioOutput *aoOutput, short sId) {
	m_aoOutput = aoOutput;
	m_sId = sId;

	speex_bits_init(&m_sbBits);
	m_dsDecState=speex_decoder_init(&speex_wb_mode);

	int iArg=1;

	speex_decoder_ctl(m_dsDecState, SPEEX_SET_ENH, &iArg);
	speex_decoder_ctl(m_dsDecState, SPEEX_GET_FRAME_SIZE, &m_iFrameSize);

	m_iByteSize = m_iFrameSize * 2;

	m_iFrameCounter = 0;

	m_psBuffer = new short[m_iFrameSize];

	speex_jitter_init(&m_sjJitter, m_dsDecState, SAMPLE_RATE);
}

AudioOutputPlayer::~AudioOutputPlayer() {
		speex_decoder_destroy(m_dsDecState);
		speex_jitter_destroy(&m_sjJitter);
}

void AudioOutputPlayer::addFrameToBuffer(QByteArray &qbaPacket, int iSeq) {
	m_qmJitter.lock();
	speex_jitter_put(&m_sjJitter, qbaPacket.data(), qbaPacket.size(), iSeq * 20);
	m_qmJitter.unlock();
}

void AudioOutputPlayer::decodeNextFrame() {
	int iTimestamp;
	bool bSpeech;
	int iSpeech = 0;

	m_qmJitter.lock();
	speex_jitter_get(&m_sjJitter, m_psBuffer, &iTimestamp);
	if (m_sjJitter.valid_bits)
		iSpeech = speex_bits_unpack_unsigned(&m_sjJitter.current_packet, 1);
	m_qmJitter.unlock();
	bSpeech = (iSpeech != 0);

	Player *p=Player::get(m_sId);
	if (p)
		p->setTalking(bSpeech);

	if (g_s.bDeaf)
		memset(m_psBuffer, 0, m_iByteSize);
}

AudioOutput::AudioOutput() {
	m_bRunning = false;
}

AudioOutput::~AudioOutput() {
	if (m_bRunning)
		qFatal("AudioOutput: Destructor while running");
}

void AudioOutput::wipe() {
	m_qmOutputMutex.lock();

	QMapIterator<short, AudioOutputPlayer *> iOutputs(m_qmOutputs);
    while (iOutputs.hasNext()) {
		iOutputs.next();
		delete iOutputs.value();
	}

	m_qmOutputs.clear();

	m_qmOutputMutex.unlock();
}

void AudioOutput::addFrameToBuffer(short sId, QByteArray &qbaPacket, int iSeq) {
	m_qmOutputMutex.lock();
	if (! m_qmOutputs.contains(sId)) {
		m_qmOutputs[sId] = getPlayer(sId);
	}
	m_qmOutputs[sId]->addFrameToBuffer(qbaPacket, iSeq);
	m_qmOutputMutex.unlock();
}

void AudioOutput::removeBuffer(short sId) {
	m_qmOutputMutex.lock();
	if (m_qmOutputs.contains(sId)) {
		AudioOutputPlayer *aopOutput = m_qmOutputs.take(sId);
		delete aopOutput;
	}
	m_qmOutputMutex.unlock();
}
