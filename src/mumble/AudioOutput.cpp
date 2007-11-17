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

#ifdef __MMX__
#include <mmintrin.h>
#endif
#include <math.h>

#include "AudioOutput.h"
#include "Player.h"
#include "Global.h"
#include "Message.h"

// Remember that we cannot use static member classes that are not pointers, as the constructor
// for AudioOutputRegistrar() might be called before they are initialized, as the constructor
// is called from global initialization.
// Hence, we allocate upon first call.

QMap<QString, AudioOutputRegistrar *> *AudioOutputRegistrar::qmNew;
QString AudioOutputRegistrar::current = QString();

AudioOutputRegistrar::AudioOutputRegistrar(const QString &n) : name(n) {
	if (! qmNew)
		qmNew = new QMap<QString, AudioOutputRegistrar *>();
	qmNew->insert(name,this);
}

AudioOutputRegistrar::~AudioOutputRegistrar() {
}

AudioOutputPtr AudioOutputRegistrar::newFromChoice(QString choice) {
	QSettings qs;
	if (!choice.isEmpty() && qmNew->contains(choice)) {
		qs.setValue(QLatin1String("AudioOutputDevice"), choice);
		current = choice;
		return AudioOutputPtr(qmNew->value(choice)->create());
	}
	choice = qs.value(QLatin1String("AudioOutputDevice")).toString();
	if (qmNew->contains(choice)) {
		current = choice;
		return AudioOutputPtr(qmNew->value(choice)->create());
	}

	// Try a sensible default. For example, ASIO is NOT a sensible default, but it's
	// pretty early in the sorted map.

	if (qmNew->contains(QLatin1String("DirectSound"))) {
		current = QLatin1String("DirectSound");
		return AudioOutputPtr(qmNew->value(current)->create());
	}


	QMapIterator<QString, AudioOutputRegistrar *> i(*qmNew);
	if (i.hasNext()) {
		i.next();
		current = i.key();
		return AudioOutputPtr(i.value()->create());
	}
	return AudioOutputPtr();
}

AudioOutputPlayer::AudioOutputPlayer(const QString name) : qsName(name) {
	iFrameSize = 0;
	psBuffer = NULL;
	fPos[0]=fPos[1]=fPos[2]=0.0;
}

AudioOutputSpeech::AudioOutputSpeech(ClientPlayer *player) : AudioOutputPlayer(player->qsName) {
	p = player;

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

	for (unsigned int i=0;i<iFrameSize;i++)
		psBuffer[i]=0;
	iMissCount = 0;
	iMissedFrames = 0;

	jbJitter = jitter_buffer_init();
	int margin = g.s.iJitterBufferSize * iFrameSize;
	jitter_buffer_ctl(jbJitter, JITTER_BUFFER_SET_MARGIN, &margin);

	speex_bits_init(&sbBits);
}

AudioOutputSpeech::~AudioOutputSpeech() {
	speex_decoder_destroy(dsDecState);
	jitter_buffer_destroy(jbJitter);
	speex_bits_destroy(&sbBits);

	delete [] psBuffer;
}

int AudioOutputSpeech::speexCallback(SpeexBits *bits, void *, void *data) {
	AudioOutputSpeech *aos=reinterpret_cast<AudioOutputSpeech *>(data);

	int len=speex_bits_unpack_unsigned(bits, 4);

	QByteArray qba(len, 0);
	unsigned char *ptr = reinterpret_cast<unsigned char *>(qba.data());

	for (int i=0;i<len;i++)
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

	if (qbaPacket.size() < 1)
		return;

	unsigned int flags = qbaPacket.at(0);
	unsigned int frames = (flags >> 4) + 1;

	JitterBufferPacket jbp;
	jbp.data = const_cast<char *>(qbaPacket.constData());
	jbp.len = qbaPacket.size();
	jbp.span = iFrameSize * frames;
	jbp.timestamp = iFrameSize * iSeq;

	jitter_buffer_put(jbJitter, &jbp);
}

bool AudioOutputSpeech::decodeNextFrame() {
	bool alive = true;

	if (p == &LoopPlayer::lpLoopy)
		LoopPlayer::lpLoopy.fetchFrames();

	if (speex_decode_int(dsDecState, &sbBits, psBuffer) == 0) {
		jitter_buffer_tick(jbJitter);
	} else {
		QMutexLocker lock(&qmJitter);

		char data[4096];
		JitterBufferPacket jbp;
		jbp.data = data;
		
		spx_int32_t startofs = 0;

		if (jitter_buffer_get(jbJitter, &jbp, iFrameSize, &startofs) == JITTER_BUFFER_OK) {
			ucFlags = jbp.data[0];
			fPos[0] = fPos[1] = fPos[2] = 0.0;
			speex_bits_read_from(&sbBits, jbp.data + 1, jbp.len - 1);
			speex_decode_int(dsDecState, &sbBits, psBuffer);
		} else {
			if (ucFlags & MessageSpeex::EndSpeech) {
				memset(psBuffer, 0, iFrameSize*2);
				alive = false;
			} else {
				iMissCount++;
				if (iMissCount < 5) {
					speex_decode_int(dsDecState, NULL, psBuffer);
				} else {
					memset(psBuffer, 0, iFrameSize*2);
					alive = false;
				}
			}
		}

		int activity;
		speex_decoder_ctl(dsDecState, SPEEX_GET_ACTIVITY, &activity);
		if (activity < 30)
			jitter_buffer_update_delay(jbJitter, &jbp, NULL);

		jitter_buffer_tick(jbJitter);
	}

	if (g.s.bLocalDeafen)
		memset(psBuffer, 0, iFrameSize*2);

	if (p)
		p->setTalking(alive, ((ucFlags & MessageSpeex::AltSpeak) ? true : false));
	return alive;
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
	foreach(const ClientPlayer *p, qmOutputs.keys())
	removeBuffer(p);
}

void AudioOutput::playSine(float hz, float i, unsigned int frames) {
	qrwlOutputs.lockForWrite();
	AudioSine *as = new AudioSine(hz,i,frames);
	qmOutputs.insert(NULL, as);
	newPlayer(as);
	qrwlOutputs.unlock();
}

void AudioOutput::addFrameToBuffer(ClientPlayer *player, const QByteArray &qbaPacket, int iSeq) {
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

void AudioOutput::removeBuffer(const ClientPlayer *player) {
	removeBuffer(qmOutputs.value(player));
}

void AudioOutput::removeBuffer(AudioOutputPlayer *aop) {
	QWriteLocker locker(&qrwlOutputs);
	QMultiHash<const ClientPlayer *, AudioOutputPlayer *>::iterator i=qmOutputs.begin();
	while (i != qmOutputs.end()) {
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

#ifdef __MMX__
	_mm_empty();
	__m64 *out=reinterpret_cast<__m64 *>(buffer);
	__m64 zero=_mm_cvtsi32_si64(0);

	for (int i=0;i<iFrameSize/4;i++)
		out[i]=zero;

	foreach(aop, qlMix) {
		__m64 *in=reinterpret_cast<__m64 *>(aop->psBuffer);

		for (int i=0;i<iFrameSize/4;i++)
			out[i]=_mm_adds_pi16(in[i],out[i]);
	}

	_mm_empty();
#else
	int t[iFrameSize];
	for (int i=0;i<iFrameSize;i++)
		t[i]=0;

	foreach(aop, qlMix)
	for (int i=0;i<iFrameSize;i++)
		t[i] += aop->psBuffer[i];

	for (int i=0;i<iFrameSize;i++)
		buffer[i]=qMax(-32727,qMin(32767,t[i]));
#endif

	qrwlOutputs.unlock();


	foreach(aop, qlDel)
	removeBuffer(aop);

	return (! qlMix.isEmpty());
}

AudioSine::AudioSine(float hz, float i, unsigned int frm) : AudioOutputPlayer(QLatin1String("Sine")) {
	v = 0.0;
	inc = M_PI * hz / 8000.0;
	dinc = M_PI * i / (8000.0 * 8000.0);
	frames = frm;
	iFrameSize = 320;
	psBuffer = new short[iFrameSize];
}

AudioSine::~AudioSine() {
	delete [] psBuffer;
}

bool AudioSine::decodeNextFrame() {
	if (frames > 0) {
		frames--;

		float t = v;
		v += iFrameSize * inc + 0.5 * dinc * iFrameSize * iFrameSize;

		for (unsigned int i=0;i<iFrameSize;i++) {
			psBuffer[i]=static_cast<short>(sin(t) * 10000.0);
			inc+=dinc;
			t+=inc;
		}
		return true;
	} else {
		memset(psBuffer, 0, iFrameSize*2);
		return false;
	}
}
