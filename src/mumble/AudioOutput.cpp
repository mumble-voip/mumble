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

#include <mmintrin.h>
#include <math.h>

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

AudioOutputPlayer::AudioOutputPlayer(const QString name) : qsName(name) {
    	iFrameSize = 0;
    	psBuffer = NULL;
    	fPos[0]=fPos[1]=fPos[2]=0.0;
}

AudioOutputSpeech::AudioOutputSpeech(Player *player) : AudioOutputPlayer(player->qsName) {
	p = player;

	speex_bits_init(&sbBits);
	dsDecState=speex_decoder_init(&speex_wb_mode);

	int iArg=1;

	speex_decoder_ctl(dsDecState, SPEEX_SET_ENH, &iArg);
	speex_decoder_ctl(dsDecState, SPEEX_GET_FRAME_SIZE, &iFrameSize);

	SpeexCallback sc;
	sc.callback_id = 1;
	sc.func = speexCallback;
	sc.data = this;

	speex_decoder_ctl(dsDecState, SPEEX_SET_USER_HANDLER, &sc);

	iFrameCounter = 0;

	psBuffer = new short[iFrameSize];
	bSpeech = false;

	for(unsigned int i=0;i<iFrameSize;i++)
		psBuffer[i]=0;
	iMissCount = 0;
	iMissedFrames = 0;

	speex_jitter_init(&sjJitter, dsDecState, SAMPLE_RATE);
}

AudioOutputSpeech::~AudioOutputSpeech() {
	speex_decoder_destroy(dsDecState);
	speex_jitter_destroy(&sjJitter);
	delete [] psBuffer;
}

int AudioOutputSpeech::speexCallback(SpeexBits *bits, void *state, void *data) {
    AudioOutputSpeech *aos=reinterpret_cast<AudioOutputSpeech *>(data);

    int len=speex_bits_unpack_unsigned(bits, 4);

    qWarning("GOT INBANDY! %d bytes",len);

    QByteArray qba(len, 0);
    unsigned char *ptr = reinterpret_cast<unsigned char *>(qba.data());

    for(int i=0;i<len;i++)
    	ptr[i]=speex_bits_unpack_unsigned(bits, 8);
    QDataStream ds(qba);
    ds >> aos->fPos[0];
    ds >> aos->fPos[1];
    ds >> aos->fPos[2];
    aos->iMissCount = 0;

    return 0;
}

void AudioOutputSpeech::addFrameToBuffer(const QByteArray &qbaPacket, int iSeq) {
	QMutexLocker lock(&qmJitter);

/*	if (! bSpeech)
		sjJitter.buffer_size=g.s.iJitterBufferSize;
*/
	speex_jitter_put(&sjJitter, const_cast<char *>(qbaPacket.constData()), qbaPacket.size(), iSeq * iFrameSize);
}

bool AudioOutputSpeech::decodeNextFrame() {
	int iTimestamp;
	int iSpeech = 0;
	int iAltSpeak = 0;
	int left;
	int i;
	unsigned int v;
	bool alive = true;

	{
		QMutexLocker lock(&qmJitter);
		speex_jitter_get(&sjJitter, psBuffer, &iTimestamp);
		if (sjJitter.valid_bits) {
			iMissedFrames = 0;
			iSpeech = speex_bits_unpack_unsigned(&sjJitter.current_packet, 1);
			iAltSpeak = speex_bits_unpack_unsigned(&sjJitter.current_packet, 1);
			if (! iSpeech) {
				jitter_buffer_reset(sjJitter.packets);
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
		} else {
			alive = false;
			iMissedFrames++;
		}
	}

	if (alive) {
		bSpeech = iSpeech;
		p->setTalking(bSpeech, (iAltSpeak ? true : false));
		if (!bSpeech)
			return false;
    	} else if (iMissedFrames > 10) {
	    	memset(psBuffer, 0, iFrameSize*2);
	    	bSpeech = false;
	    	p->setTalking(false, false);
	    	return false;
	}
	return true;
}

AudioOutput::AudioOutput() {
	void *ds=speex_decoder_init(&speex_wb_mode);
	speex_decoder_ctl(ds, SPEEX_GET_FRAME_SIZE, &iFrameSize);
	speex_decoder_destroy(ds);
	bRunning = false;
}

AudioOutput::~AudioOutput() {
	bRunning = false;
	wait();
	wipe();
}

void AudioOutput::newPlayer(AudioOutputPlayer *) {
}

void AudioOutput::wipe() {
    	foreach(const Player *p, qmOutputs.keys())
    		removeBuffer(p);
}

void AudioOutput::playSine(float hz, float i, int frames) {
    	qrwlOutputs.lockForWrite();
    	AudioSine *as = new AudioSine(hz,i,frames);
    	qmOutputs.insert(NULL, as);
    	newPlayer(as);
    	qrwlOutputs.unlock();
}

void AudioOutput::addFrameToBuffer(Player *player, const QByteArray &qbaPacket, int iSeq) {
	qrwlOutputs.lockForRead();
	AudioOutputSpeech *aop = dynamic_cast<AudioOutputSpeech *>(qmOutputs.value(player));
	if (! aop) {
		qrwlOutputs.unlock();
		qrwlOutputs.lockForWrite();
		aop = new AudioOutputSpeech(player);
		qmOutputs.replace(player,aop);
		newPlayer(aop);
	}

	aop->addFrameToBuffer(qbaPacket, iSeq);

	qrwlOutputs.unlock();
}

void AudioOutput::removeBuffer(const Player *player) {
    	removeBuffer(qmOutputs.value(player));
}

void AudioOutput::removeBuffer(AudioOutputPlayer *aop) {
	QWriteLocker locker(&qrwlOutputs);
	QMultiHash<const Player *, AudioOutputPlayer *>::iterator i=qmOutputs.begin();
	while(i != qmOutputs.end()) {
	    if (i.value() == aop) {
	    	qmOutputs.erase(i);
	    	delete aop;
	    	break;
	    }
	    ++i;
	}
}

bool AudioOutput::mixAudio(short *buffer) {
	AudioOutputPlayer *aop;
	QList<AudioOutputPlayer *> qlMix;
	QList<AudioOutputPlayer *> qlDel;

	qrwlOutputs.lockForRead();
	foreach(aop, qmOutputs) {
		if (! aop->decodeNextFrame()) {
			qlDel.append(aop);
		} else {
			qlMix.append(aop);
		}
	}

	_mm_empty();
	__m64 *out=reinterpret_cast<__m64 *>(buffer);
	__m64 zero=_mm_cvtsi32_si64(0);

	for(int i=0;i<iFrameSize/4;i++)
		out[i]=zero;

	foreach(aop, qlMix) {
		__m64 *in=reinterpret_cast<__m64 *>(aop->psBuffer);

		for(int i=0;i<iFrameSize/4;i++)
			out[i]=_mm_adds_pi16(in[i],out[i]);
	}
	qrwlOutputs.unlock();

	_mm_empty();

	foreach(aop, qlDel)
		removeBuffer(aop);

	return (! qlMix.isEmpty());
}

AudioSine::AudioSine(float hz, float i, int frm) : AudioOutputPlayer(QLatin1String("Sine")) {
    v = 0.0;
    inc = M_PI * hz / 8000.0;
    dinc = M_PI * i / (8000.0 * 8000.0);
    frames = frm;
    iFrameSize = 160;
    psBuffer = new short[iFrameSize];
}

AudioSine::~AudioSine() {
    delete [] psBuffer;
}

bool AudioSine::decodeNextFrame() {
    	if (frames-- > 0) {
	    for(int i=0;i<iFrameSize;i++) {
		    psBuffer[i]=sin(v) * 20000.0;
		    inc+=dinc;
		    v+=inc;
	    }
	    return true;
        } else {
	    memset(psBuffer, 0, iFrameSize*2);
	    return false;
	}
}
