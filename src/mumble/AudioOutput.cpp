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

#include "AudioOutput.h"
#include "Player.h"
#include "Global.h"

// Remember that we cannot use static member classes that are not pointers, as the constructor
// for AudioOutputRegistrar() might be called before they are initialized, as the constructor
// is called from global initialization.
// Hence, we allocate upon first call.

QMap<QString, AudioOutputRegistrarNew> *AudioOutputRegistrar::qmNew;
QString AudioOutputRegistrar::current = QString();

AudioOutputRegistrar::AudioOutputRegistrar(QString name, AudioOutputRegistrarNew n) {
	if (! qmNew)
		qmNew = new QMap<QString, AudioOutputRegistrarNew>();
	qmNew->insert(name,n);
}

AudioOutputPtr AudioOutputRegistrar::newFromChoice(QString choice) {
	QSettings qs;
	if (!choice.isEmpty() && qmNew->contains(choice)) {
		qs.setValue(QLatin1String("AudioOutputDevice"), choice);
		current = choice;
		return AudioOutputPtr(qmNew->value(choice)());
	}
	choice = qs.value(QLatin1String("AudioOutputDevice")).toString();
	if (qmNew->contains(choice)) {
		current = choice;
		return AudioOutputPtr(qmNew->value(choice)());
	}

	// Try a sensible default. For example, ASIO is NOT a sensible default, but it's
	// pretty early in the sorted map.

	if (qmNew->contains(QLatin1String("DirectSound"))) {
		current = QLatin1String("DirectSound");
		return AudioOutputPtr(qmNew->value(current)());
	}


	QMapIterator<QString, AudioOutputRegistrarNew> i(*qmNew);
	if (i.hasNext()) {
		i.next();
		current = i.key();
		return AudioOutputPtr(i.value()());
	}
	return AudioOutputPtr();
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

	for(int i=0;i<3;i++)
		fPos[i]=0.0;
	for(unsigned int i=0;i<iFrameSize;i++)
		psBuffer[i]=0;
	iMissCount = 0;

	speex_jitter_init(&sjJitter, dsDecState, SAMPLE_RATE);
}

AudioOutputPlayer::~AudioOutputPlayer() {
	speex_decoder_destroy(dsDecState);
	speex_jitter_destroy(&sjJitter);
	delete [] psBuffer;
}

void AudioOutputPlayer::addFrameToBuffer(QByteArray &qbaPacket, int iSeq) {
	QMutexLocker lock(&qmJitter);

/*	if (! bSpeech)
		sjJitter.buffer_size=g.s.iJitterBufferSize;
*/
	speex_jitter_put(&sjJitter, qbaPacket.data(), qbaPacket.size(), iSeq * iFrameSize);
}

static QTime tt;

bool AudioOutputPlayer::decodeNextFrame() {
	int iTimestamp;
	int iSpeech = 0;
	int iAltSpeak = 0;
	int left;
	int i;
	unsigned int v;
	bool alive = true;

//	qWarning("Asking for packet at %08ld", tt.elapsed() / 20);

	{
		QMutexLocker lock(&qmJitter);
		speex_jitter_get(&sjJitter, psBuffer, &iTimestamp);
		if (sjJitter.valid_bits) {
			iSpeech = speex_bits_unpack_unsigned(&sjJitter.current_packet, 1);
			iAltSpeak = speex_bits_unpack_unsigned(&sjJitter.current_packet, 1);
			if (! iSpeech) {
#ifdef SPEEX_ANCIENT
				sjJitter.reset_state = 1;
#else
				jitter_buffer_reset(sjJitter.packets);
#endif
				sjJitter.valid_bits = 0;
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
				iMissCount = 0;
			} else {
				iMissCount++;
				if (iMissCount >= 4)
					fPos[0] = fPos[1] = fPos[2] = 0.0;
			}
		}
		if (! sjJitter.valid_bits) {
			alive = false;
		}
	}

	bSpeech = iSpeech;

	p->setTalking(bSpeech, (iAltSpeak ? true : false));

	if (g.s.bDeaf || ! bSpeech)
		memset(psBuffer, 0, iByteSize);

	return alive;
}

AudioOutput::AudioOutput() {
	bRunning = false;
}

AudioOutput::~AudioOutput() {
	bRunning = false;
	wait();
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
	qrwlOutputs.lockForRead();
	AudioOutputPlayer *aop = qmOutputs.value(player);
	if (! aop) {
		qrwlOutputs.unlock();
		qrwlOutputs.lockForWrite();
		aop = getPlayer(player);
		qmOutputs[player]=aop;
	}

	aop->addFrameToBuffer(qbaPacket, iSeq);

	if (! isRunning())
		start(QThread::HighPriority);
	qrwlOutputs.unlock();
}

void AudioOutput::removeBuffer(Player *player) {
	QWriteLocker locker(&qrwlOutputs);
	AudioOutputPlayer *aopOutput = qmOutputs.take(player);
	if (aopOutput)
		delete aopOutput;
}
