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
#include "Global.h"
#include <QSettings>

// Remember that we cannot use static member classes that are not pointers, as the constructor
// for AudioOutputRegistrar() might be called before they are initialized, as the constructor
// is called from global initialization.
// Hence, we allocate upon first call.

QMap<QString, AudioOutputRegistrarNew> *AudioOutputRegistrar::qmNew;
QMap<QString, AudioRegistrarConfig> *AudioOutputRegistrar::qmConfig;
QString AudioOutputRegistrar::current = QString();

AudioOutputRegistrar::AudioOutputRegistrar(QString name, AudioOutputRegistrarNew n, AudioRegistrarConfig c) {
	if (! qmNew)
		qmNew = new QMap<QString, AudioOutputRegistrarNew>();
	if (! qmConfig)
		qmConfig = new QMap<QString, AudioRegistrarConfig>();
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

AudioOutputPlayer::AudioOutputPlayer(AudioOutput *ao, Player *player) {
	aoOutput = ao;
	p = player;

	speex_bits_init(&sbBits);
	dsDecState=speex_decoder_init(&speex_wb_mode);

	int iArg=1;

	speex_decoder_ctl(dsDecState, SPEEX_SET_ENH, &iArg);
	speex_decoder_ctl(dsDecState, SPEEX_GET_FRAME_SIZE, &iFrameSize);

	iByteSize = iFrameSize * 2;

	iFrameCounter = 0;

	psBuffer = new short[iFrameSize];
	bSpeech = false;

	speex_jitter_init(&sjJitter, dsDecState, SAMPLE_RATE);
}

AudioOutputPlayer::~AudioOutputPlayer() {
		speex_decoder_destroy(dsDecState);
		speex_jitter_destroy(&sjJitter);
		delete [] psBuffer;
}

void AudioOutputPlayer::addFrameToBuffer(QByteArray &qbaPacket, int iSeq) {
	QMutexLocker lock(&qmJitter);

	if (! bSpeech)
		sjJitter.buffer_size=g.s.iJitterBufferSize;
	speex_jitter_put(&sjJitter, qbaPacket.data(), qbaPacket.size(), iSeq * 20);
}

bool AudioOutputPlayer::decodeNextFrame() {
	int iTimestamp;
	int iSpeech = 0;
	int left;
	int i;
	unsigned int v;
	bool alive = true;

	{
		QMutexLocker lock(&qmJitter);
		speex_jitter_get(&sjJitter, psBuffer, &iTimestamp);
		if (sjJitter.valid_bits) {
			iSpeech = speex_bits_unpack_unsigned(&sjJitter.current_packet, 1);
			if (! iSpeech) {
				sjJitter.reset_state = 1;
				speex_decoder_ctl(dsDecState, SPEEX_RESET_STATE, NULL);
			}
			left = speex_bits_remaining(&sjJitter.current_packet) / 8;
			if (left >= 12) {
				QByteArray qba(left, 0);
				unsigned char *ptr = reinterpret_cast<unsigned char *>(qba.data());
				for(i=0;i<left;i++) {
					v = speex_bits_unpack_unsigned(&sjJitter.current_packet, 8);
					ptr[i]=v;
				}
				QDataStream ds(qba);
				ds >> fPos[0];
				ds >> fPos[1];
				ds >> fPos[2];
				if (left >= 24) {
					ds >> fVel[0];
					ds >> fVel[1];
					ds >> fVel[2];
				} else {
					fVel[0] = fVel[1] = fVel[2] = 0.0;
				}
			} else {
				fPos[0] = fPos[1] = fPos[2] = 0.0;
			}
		}
		if (sjJitter.reset_state)
			alive = false;
	}

	bSpeech = iSpeech;

	p->setTalking(bSpeech);

	if (g.s.bDeaf || ! bSpeech)
		memset(psBuffer, 0, iByteSize);

	return alive;
}

AudioOutput::AudioOutput() {
	bRunning = false;
}

AudioOutput::~AudioOutput() {
	bRunning = false;
	wipe();
}

void AudioOutput::wipe() {
	QWriteLocker locker(&qrwlOutputs);

	foreach(AudioOutputPlayer *aop, qmOutputs) {
		delete aop;
	}
	qmOutputs.clear();
}

void AudioOutput::addFrameToBuffer(Player *player, QByteArray &qbaPacket, int iSeq) {
	QReadLocker locker(&qrwlOutputs);
	AudioOutputPlayer *aop = qmOutputs.value(player);
	if (! aop) {
		aop = getPlayer(player);
		qmOutputs[player]=aop;
	}
	aop->addFrameToBuffer(qbaPacket, iSeq);
}

void AudioOutput::removeBuffer(Player *player) {
	QWriteLocker locker(&qrwlOutputs);
	AudioOutputPlayer *aopOutput = qmOutputs.take(player);
	if (aopOutput)
		delete aopOutput;
}
