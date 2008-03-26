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

#include "AudioOutput.h"
#include "AudioInput.h"
#include "Player.h"
#include "Global.h"
#include "Message.h"
#include "Plugins.h"

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
	qmNew->remove(name);
}

AudioOutputPtr AudioOutputRegistrar::newFromChoice(QString choice) {
	if (! qmNew)
		return AudioOutputPtr();

	if (!choice.isEmpty() && qmNew->contains(choice)) {
		g.s.qsAudioOutput = choice;
		current = choice;
		return AudioOutputPtr(qmNew->value(choice)->create());
	}
	choice = g.s.qsAudioOutput;
	if (qmNew->contains(choice)) {
		current = choice;
		return AudioOutputPtr(qmNew->value(choice)->create());
	}

	// Try a sensible default. For example, ASIO is NOT a sensible default, but it's
	// pretty early in the sorted map.

	if (qmNew->contains(QLatin1String(DEFAULT_SOUNDSYS))) {
		current = QLatin1String(DEFAULT_SOUNDSYS);
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

	jbJitter = jitter_buffer_init(iFrameSize);
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
		jbp.len = 4096;

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
					memset(psBuffer, 0, iFrameSize*sizeof(short));
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

	if (g.s.fVolume < 0.01)
		memset(psBuffer, 0, iFrameSize*sizeof(short));

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

// Here's the theory.
// We support sound "bloom"ing. That is, if sound comes directly from the left, if it is sufficiently
// close, we'll hear it full intensity from the left side, and "bloom" intensity from the right side.
//
// FIXME: Add a barebones minimum volume, and scale all of this according to that.

float AudioOutput::calcGain(float dotproduct, float distance) {

	float dotfactor = (dotproduct + 1.0f) / 2.0f;
	float scaling = (1.0 - g.s.fAudioMinVolume);
	float att;


	// No distance attenuation
	if (g.s.fAudioRollOff < 0.01f) {
		att = qMin(1.0f, dotfactor + g.s.fAudioBloom);
	} else if (distance < g.s.fAudioMinDistance) {
		float bloomfac = g.s.fAudioBloom * (1.0f - distance/g.s.fAudioMinDistance);

		att = qMin(1.0f, bloomfac + dotfactor);
	} else {
		if (distance > g.s.fAudioMaxDistance)
			distance = g.s.fAudioMaxDistance;
		float datt = g.s.fAudioMinDistance / (g.s.fAudioMinDistance + (distance-g.s.fAudioMinDistance)*g.s.fAudioRollOff);

		att = datt * dotfactor;
	}
	return g.s.fAudioMinVolume + scaling * att;
}

void AudioOutput::newPlayer(AudioOutputPlayer *) {
}

void AudioOutput::wipe() {
	foreach(const ClientPlayer *p, qmOutputs.keys())
	removeBuffer(p);
}

void AudioOutput::playSine(float hz, float i, unsigned int frames, float volume) {
	qrwlOutputs.lockForWrite();
	AudioSine *as = new AudioSine(hz,i,frames, volume);
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

#if defined(__MMX__) || defined(Q_OS_WIN)
	_mm_empty();
	__m64 *out=reinterpret_cast<__m64 *>(buffer);
	__m64 zero=_mm_cvtsi32_si64(0);

	int sz = iFrameSize/4;

	for (int i=0;i<sz;i++)
		out[i]=zero;

	foreach(aop, qlMix) {
		__m64 *in=reinterpret_cast<__m64 *>(aop->psBuffer);

		for (int i=0;i<sz;i++)
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

void AudioOutput::normalizeSpeakers(float * speakerpos, int nspeakers) {
	for (int i=0;i<nspeakers;++i) {
		float d = sqrtf(speakerpos[3*i+0]*speakerpos[3*i+0] + speakerpos[3*i+1]*speakerpos[3*i+1] + speakerpos[3*i+2]*speakerpos[3*i+2]);
		if (d > 0.0f) {
			speakerpos[3*i+0] /= d;
			speakerpos[3*i+1] /= d;
			speakerpos[3*i+2] /= d;
		}
	}
}

bool AudioOutput::mixSurround(float * output, float * speakerpos, int nspeakers) {
	AudioOutputPlayer *aop;
	QList<AudioOutputPlayer *> qlMix;
	QList<AudioOutputPlayer *> qlDel;

	const float mul = 1.0f / 32768.0f;

	memset(output, 0, sizeof(float) * iFrameSize * nspeakers);

	STACKVAR(float, inbuff, iFrameSize);

	qrwlOutputs.lockForRead();
	foreach(aop, qmOutputs) {
		if (! aop->decodeNextFrame()) {
			qlDel.append(aop);
		} else {
			qlMix.append(aop);
		}
	}


	if (! qlMix.isEmpty()) {
		STACKVAR(float, speaker, nspeakers*3);
		STACKVAR(bool, dirspeaker, nspeakers);
		bool validListener = false;

		g.p->fetch();

#ifndef AUDIO_TEST
		if (g.p->bValid) {
			float front[3] = { g.p->fFront[0], g.p->fFront[1], g.p->fFront[2]};
			float top[3] = { g.p->fTop[0], g.p->fTop[1], g.p->fTop[2]};
#else
		if (true) {
			float front[3] = { 0.0f, -1.0f, 0.0f };
			float top[3] = {0.0f, 0.0f, 1.0f };
			g.p->fPosition[0] = 0.0f;
			g.p->fPosition[1] = 0.0f;
			g.p->fPosition[2] = 0.0f;
#endif
			if (fabs(front[0] * top[0] + front[1] * top[1] + front[2] * top[2]) > 0.01f) {
				// Not perpendicular. Ditch Y and point top up.
				front[1] = 0;
				top[0] = 0;
				top[1] = 1;
				top[2] = 0;
			}

			// Normalize
			float flen = sqrtf(front[0]*front[0]+front[1]*front[1]+front[2]*front[2]);
			float tlen = sqrtf(top[0]*top[0]+top[1]*top[1]+top[2]*top[2]);

			if (flen > 0.0f) {
				front[0] /= flen;
				front[1] /= flen;
				front[2] /= flen;
			}

			if (tlen > 0.0f) {
				top[0] /= tlen;
				top[1] /= tlen;
				top[2] /= tlen;
			}

			// Calculate right vector as front X top
			float right[3] = {top[1]*front[2] - top[2]*front[1], top[2]*front[0] - top[0]*front[2], top[0]*front[1] - top[1] * front[0] };

			/*
						qWarning("Front: %f %f %f", front[0], front[1], front[2]);
						qWarning("Top: %f %f %f", top[0], top[1], top[2]);
						qWarning("Right: %f %f %f", right[0], right[1], right[2]);
			*/
			// Rotate speakers to match orientation
			for (int i=0;i<nspeakers;++i) {
				speaker[3*i+0] = speakerpos[3*i+0] * right[0] + speakerpos[3*i+1] * top[0] + speakerpos[3*i+2] * front[0];
				speaker[3*i+1] = speakerpos[3*i+0] * right[1] + speakerpos[3*i+1] * top[1] + speakerpos[3*i+2] * front[1];
				speaker[3*i+2] = speakerpos[3*i+0] * right[2] + speakerpos[3*i+1] * top[2] + speakerpos[3*i+2] * front[2];

				if ((speaker[3*i+0] != 0.0f) || (speaker[3*i+1] != 0.0f) || (speaker[3*i+2] != 0.0f)) {
					dirspeaker[i] = true;
				} else {
					dirspeaker[i] = false;
				}
			}
			validListener = true;
		}
		foreach(aop, qlMix) {
			for (int i=0;i<iFrameSize;i++)
				inbuff[i] = aop->psBuffer[i] * mul;

#ifdef AUDIO_TEST
			aop->fPos[0] = 4.0f;
			aop->fPos[1] = 0.0f;
			aop->fPos[2] = 0.0f;
#endif

			if (validListener && ((aop->fPos[0] != 0.0f) || (aop->fPos[1] != 0.0f) || (aop->fPos[2] != 0.0f))) {
				float dir[3] = { aop->fPos[0] - g.p->fPosition[0], aop->fPos[1] - g.p->fPosition[1], aop->fPos[2] - g.p->fPosition[2] };
				float len = sqrtf(dir[0] * dir[0] + dir[1] * dir[1] + dir[2] * dir[2]);
				dir[0] /= len;
				dir[1] /= len;
				dir[2] /= len;
				/*
								qWarning("Voice pos: %f %f %f", aop->fPos[0], aop->fPos[1], aop->fPos[2]);
								qWarning("Voice dir: %f %f %f", dir[0], dir[1], dir[2]);
				*/
				for (int s=0;s<nspeakers;++s) {
					float dot = dirspeaker[s] ? dir[0] * speaker[s*3+0] + dir[1] * speaker[s*3+1] + dir[2] * speaker[s*3+2] : 1.0f;
					float str = calcGain(dot, len);
					/*
										qWarning("%d: Pos %f %f %f : Dot %f Len %f Str %f", s, speaker[s*3+0], speaker[s*3+1], speaker[s*3+2], dot, len, str);
					*/
					for (int i=0;i<iFrameSize;++i)
						output[i*nspeakers+s] += inbuff[i] * str;
				}
			} else {
				for (int i=0;i<iFrameSize;++i)
					for (int j=0;j<nspeakers;++j)
						output[i*nspeakers+j] += inbuff[i];
			}
		}
		// Clip
		for (int i=0;i<iFrameSize*nspeakers;i++)
			output[i] = output[i] < -1.0f ? -1.0f : (output[i] > 1.0f ? 1.0f : output[i]);
	}

	qrwlOutputs.unlock();

	foreach(aop, qlDel)
	removeBuffer(aop);

	return (! qlMix.isEmpty());
}

bool AudioOutput::mixStereoAudio(short *buffer) {
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

	STACKVAR(int, t, iFrameSize*2);

	for (int i=0;i< (iFrameSize*2);i++)
		t[i]=0;

	Plugins *p = g.p;

	p->fetch(); // Make sure we use the actual position

	if (p->bValid) { // Use stereo if plugin data is valid
		const float left[3] = { // We have an LHS coordinate system
			- p->fTop[1]*p->fFront[2] + p->fTop[2]*p->fFront[1],
			- p->fTop[2]*p->fFront[0] + p->fTop[0]*p->fFront[2],
			- p->fTop[0]*p->fFront[1] + p->fTop[1]*p->fFront[0]
		};
		// This cross-product defines the left/right split plane

		const float left_norm = sqrtf(left[0]*left[0] + left[1]*left[1]+left[2]*left[2]);
		foreach(aop, qlMix) {
			float vol_scal = 1.0;
			float vol_stereo = 0;

			// Test if source has valid position data
			if ((aop->fPos[0] != 0.0) || (aop->fPos[1] != 0.0) || (aop->fPos[2] != 0.0)) {
				const float source_direction[3] = {
					aop->fPos[0] - p->fPosition[0],
					aop->fPos[1] - p->fPosition[1],
					aop->fPos[1] - p->fPosition[1]
				};

				const float source_direction_norm = sqrtf(
				                                        source_direction[0]*source_direction[0]
				                                        + source_direction[1]*source_direction[1]
				                                        + source_direction[2]*source_direction[2]);

				vol_stereo = (left[0]*source_direction[0] +
				              left[1]*source_direction[1] +
				              left[2]*source_direction[2])
				             / left_norm / source_direction_norm;
				// asin(scalar_product) = angle from center, where
				// angle < 0 is right and angle > 0 is left

				vol_scal= calcGain(vol_stereo, source_direction_norm);
			}
			const float vol_left=  vol_scal * (1.0 + vol_stereo);
			const float vol_right= vol_scal * (1.0 - vol_stereo);
			for (int i=0;i<iFrameSize;i++) {
				t[2*i] += (int)(vol_left * aop->psBuffer[i]);
				t[2*i+1] += (int)(vol_right *aop->psBuffer[i]);
			}
		}
	} else { // no stereo
		foreach(aop, qlMix) {
			for (int i=0;i<iFrameSize;i++) {
				t[2*i]   += aop->psBuffer[i];
				t[2*i+1] += aop->psBuffer[i];
			}
		}
	}

	for (int i=0;i<(iFrameSize*2);i++)
		buffer[i]=qMax(-32727,qMin(32767,t[i]));

	qrwlOutputs.unlock();

	foreach(aop, qlDel)
	removeBuffer(aop);

	return (! qlMix.isEmpty());
}

AudioSine::AudioSine(float hz, float i, unsigned int frm, float vol) : AudioOutputPlayer(QLatin1String("Sine")) {
	v = 0.0;
	inc = M_PI * hz / 8000.0;
	dinc = M_PI * i / (8000.0 * 8000.0);
	volume = vol;
	frames = frm;
	iFrameSize = 320;
	cntr = 0;
	psBuffer = new short[iFrameSize];
	tbin = 4;

	if (inc == 0.0)
		g.iAudioPathTime = 0;
}

AudioSine::~AudioSine() {
	delete [] psBuffer;
}

bool AudioSine::decodeNextFrame() {
	if (frames > 0) {
		frames--;

		if (inc == 0.0) {
			if (++cntr == 50) {
				bSearch = true;
				cntr = 0;
				tbin *= 2;
				if (tbin >= 64)
					tbin = 4;
			}

			AudioInputPtr ai = g.ai;
			if (ai && bSearch && ai->iBestBin == tbin) {
				bSearch = false;
				g.iAudioPathTime = cntr;
			}

			for (unsigned int i=0;i<iFrameSize;i++)
				psBuffer[i] = lroundf(32768.0f * volume * sinf(M_PI * i * (tbin * 1.0f) / (1.0f * iFrameSize)));

			return true;
		}

		float t = v;

		for (unsigned int i=0;i<iFrameSize;i++) {
			psBuffer[i]=static_cast<short>(sinf(t) * volume * 32768.0f);
			inc+=dinc;
			t+=inc;
		}
		v = t;
		return true;
	} else {
		memset(psBuffer, 0, iFrameSize*2);
		return false;
	}
}
