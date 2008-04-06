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
	iBufferSize = 0;
	pfBuffer = NULL;
	fPos[0]=fPos[1]=fPos[2]=0.0;
}

AudioOutputPlayer::~AudioOutputPlayer() {
	if (pfBuffer)
		delete [] pfBuffer;
}

void AudioOutputPlayer::resizeBuffer(unsigned int newsize) {
	if (newsize > iBufferSize) {
		float *n = new float[newsize];
		if (pfBuffer) {
			memcpy(n, pfBuffer, sizeof(float) * iBufferSize);
			delete [] pfBuffer;
		}
		pfBuffer = n;
		iBufferSize = newsize;
	}
}

AudioOutputSpeech::AudioOutputSpeech(ClientPlayer *player, unsigned int freq) : AudioOutputPlayer(player->qsName) {
	p = player;

	dsDecState=speex_decoder_init(&speex_wb_mode);

	int iArg=1;
	int err;

	speex_decoder_ctl(dsDecState, SPEEX_SET_ENH, &iArg);
	speex_decoder_ctl(dsDecState, SPEEX_GET_FRAME_SIZE, &iFrameSize);

	qWarning("Resampling from %d to %d", 16000, freq);
	srs = speex_resampler_init(1, 16000, freq, 3, &err);

	iOutputSize = lroundf(ceilf((iFrameSize * freq) / (16000 * 1.0f)));

	iBufferOffset = iBufferFilled = iLastConsume = 0;
	bLastAlive = true;

	SpeexCallback sc;
	sc.callback_id = 1;
	sc.func = speexCallback;
	sc.data = this;

	speex_decoder_ctl(dsDecState, SPEEX_SET_USER_HANDLER, &sc);

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

void AudioOutputSpeech::addFrameToBuffer(const QByteArray &qbaPacket, unsigned int iSeq) {
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

bool AudioOutputSpeech::needSamples(unsigned int snum) {
	for(unsigned int i=iLastConsume;i<iBufferFilled;++i)
		pfBuffer[i-iLastConsume]=pfBuffer[i];
	iBufferFilled -= iLastConsume;

	iLastConsume = snum;

	if (iBufferFilled >= snum)
		return bLastAlive;

	STACKVAR(float, fOut, iFrameSize);

	while (iBufferFilled < snum) {
		resizeBuffer(iBufferFilled + iOutputSize);

		if (p == &LoopPlayer::lpLoopy)
			LoopPlayer::lpLoopy.fetchFrames();


		if (speex_decode(dsDecState, &sbBits, fOut) == 0) {
			jitter_buffer_tick(jbJitter);
			bLastAlive = true;
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
				speex_decode(dsDecState, &sbBits, fOut);
				bLastAlive = true;
			} else {
				if (ucFlags & MessageSpeex::EndSpeech) {
					memset(fOut, 0, sizeof(float) * iFrameSize);
					bLastAlive = false;
				} else {
					iMissCount++;
					if (iMissCount < 5) {
						speex_decode(dsDecState, NULL, fOut);
					} else {
						memset(fOut, 0, sizeof(float) * iFrameSize);
						bLastAlive = false;
					}
				}
			}

			int activity;
			speex_decoder_ctl(dsDecState, SPEEX_GET_ACTIVITY, &activity);
			if (activity < 30)
				jitter_buffer_update_delay(jbJitter, &jbp, NULL);

			jitter_buffer_tick(jbJitter);
		}
		spx_uint32_t inlen = iFrameSize;
		spx_uint32_t outlen = iOutputSize;
		speex_resampler_process_float(srs, 0, fOut, &inlen, pfBuffer + iBufferFilled, &outlen);
		iBufferFilled += outlen;
	}

	if (p)
		p->setTalking(bLastAlive, ((ucFlags & MessageSpeex::AltSpeak) ? true : false));
	return bLastAlive;
}

AudioOutput::AudioOutput() {
	void *ds=speex_decoder_init(&speex_wb_mode);
	speex_decoder_ctl(ds, SPEEX_GET_FRAME_SIZE, &iFrameSize);
	speex_decoder_destroy(ds);
	bRunning = false;

	iChannels = 0;
	fSpeakers = NULL;
	fSpeakerVolume = NULL;
	bSpeakerPositional = NULL;

	iMixerFreq = 16000;
}

AudioOutput::~AudioOutput() {
	bRunning = false;
	wait();
	wipe();

	if (fSpeakers)
		delete [] fSpeakers;
	if (fSpeakerVolume)
		delete [] fSpeakerVolume;
	if (bSpeakerPositional)
		delete [] bSpeakerPositional;
}

// Here's the theory.
// We support sound "bloom"ing. That is, if sound comes directly from the left, if it is sufficiently
// close, we'll hear it full intensity from the left side, and "bloom" intensity from the right side.

float AudioOutput::calcGain(float dotproduct, float distance) {

	float dotfactor = (dotproduct + 1.0f) / 2.0f;
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
	return att;
}

void AudioOutput::newPlayer(AudioOutputPlayer *) {
}

void AudioOutput::wipe() {
	foreach(const ClientPlayer *p, qmOutputs.keys())
	removeBuffer(p);
}

void AudioOutput::playSine(float hz, float i, unsigned int frames, float volume) {
	qrwlOutputs.lockForWrite();
	AudioSine *as = new AudioSine(hz,i,frames, volume, iMixerFreq);
	qmOutputs.insert(NULL, as);
	newPlayer(as);
	qrwlOutputs.unlock();
}

void AudioOutput::addFrameToBuffer(ClientPlayer *player, const QByteArray &qbaPacket, unsigned int iSeq) {
	qrwlOutputs.lockForRead();
	AudioOutputSpeech *aop = dynamic_cast<AudioOutputSpeech *>(qmOutputs.value(player));
	if (! aop) {
		qrwlOutputs.unlock();
		qrwlOutputs.lockForWrite();
		aop = new AudioOutputSpeech(player, iMixerFreq);
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

void AudioOutput::initializeMixer(unsigned int *chanmasks, unsigned int nchannels) {
	iChannels = nchannels;

	fSpeakers = new float[iChannels * 3];
	bSpeakerPositional = new bool[iChannels];
	fSpeakerVolume = new float[iChannels];

	memset(fSpeakers, 0, sizeof(float) * iChannels * 3);
	memset(bSpeakerPositional, 0, sizeof(bool) * iChannels);

	for(unsigned int i=0;i<iChannels;++i)
		fSpeakerVolume[i] = 1.0f;

	if (g.s.bPositionalAudio && (iChannels > 1)) {
		for(unsigned int i=0;i<iChannels;i++) {
			float *s = &fSpeakers[3*i];
			bSpeakerPositional[i] = true;

			switch (chanmasks[i]) {
				case SPEAKER_FRONT_LEFT:
					s[0] = -0.5f;
					s[2] = 1.0f;
					break;
				case SPEAKER_FRONT_RIGHT:
					s[0] = 0.5f;
					s[2] = 1.0f;
					break;
				case SPEAKER_FRONT_CENTER:
					s[2] = 1.0f;
					break;
				case SPEAKER_LOW_FREQUENCY:
					break;
				case SPEAKER_BACK_LEFT:
					s[0] = -0.5f;
					s[2] = -1.0f;
					break;
				case SPEAKER_BACK_RIGHT:
					s[0] = 0.5f;
					s[2] = -1.0f;
					break;
				case SPEAKER_FRONT_LEFT_OF_CENTER:
					s[0] = -0.25;
					s[2] = 1.0f;
					break;
				case SPEAKER_FRONT_RIGHT_OF_CENTER:
					s[0] = 0.25;
					s[2] = 1.0f;
					break;
				case SPEAKER_BACK_CENTER:
					s[2] = -1.0f;
					break;
				case SPEAKER_SIDE_LEFT:
					s[0] = -1.0f;
					break;
				case SPEAKER_SIDE_RIGHT:
					s[0] = 1.0f;
					break;
				case SPEAKER_TOP_CENTER:
					s[1] = 1.0f;
					s[2] = 1.0f;
					break;
				case SPEAKER_TOP_FRONT_LEFT:
					s[0] = -0.5f;
					s[1] = 1.0f;
					s[2] = 1.0f;
					break;
				case SPEAKER_TOP_FRONT_CENTER:
					s[1] = 1.0f;
					s[2] = 1.0f;
					break;
				case SPEAKER_TOP_FRONT_RIGHT:
					s[0] = 0.5f;
					s[1] = 1.0f;
					s[2] = 1.0f;
					break;
				case SPEAKER_TOP_BACK_LEFT:
					s[0] = -0.5f;
					s[1] = 1.0f;
					s[2] = -1.0f;
					break;
				case SPEAKER_TOP_BACK_CENTER:
					s[1] = 1.0f;
					s[2] = -1.0f;
					break;
				case SPEAKER_TOP_BACK_RIGHT:
					s[0] = 0.5f;
					s[1] = 1.0f;
					s[2] = -1.0f;
					break;
				default:
					bSpeakerPositional[i] = false;
					fSpeakerVolume[i] = 0.0f;
					qWarning("AudioOutput: Unknown speaker %d: %08x", i, chanmasks[i]);
					break;
			}
			if (g.s.bPositionalHeadphone) {
				s[1] = 0.0f;
				s[2] = 0.0f;
				if (s[0] == 0.0f)
					fSpeakerVolume[i] = 0.0f;
			}
		}
		for(unsigned int i=0;i<iChannels;i++) {
			float d = sqrtf(fSpeakers[3*i+0]*fSpeakers[3*i+0] + fSpeakers[3*i+1]*fSpeakers[3*i+1] + fSpeakers[3*i+2]*fSpeakers[3*i+2]);
			if (d > 0.0f) {
				fSpeakers[3*i+0] /= d;
				fSpeakers[3*i+1] /= d;
				fSpeakers[3*i+2] /= d;
			}
		}
	}
}

bool AudioOutput::mix(float *output, unsigned int nsamp) {
	AudioOutputPlayer *aop;
	QList<AudioOutputPlayer *> qlMix;
	QList<AudioOutputPlayer *> qlDel;

	memset(output, 0, sizeof(float) * nsamp * iChannels);

	if (g.s.fVolume < 0.01)
		return false;

	const float mul = g.s.fVolume / 32768.0f;

	qrwlOutputs.lockForRead();
	foreach(aop, qmOutputs) {
		if (! aop->needSamples(nsamp)) {
			qlDel.append(aop);
		} else {
			qlMix.append(aop);
		}
	}

	if (! qlMix.isEmpty()) {
		STACKVAR(float, speaker, iChannels*3);
		bool validListener = false;

		if (g.s.bPositionalAudio && (iChannels > 1) && g.p->fetch()) {
			float front[3] = { g.p->fFront[0], g.p->fFront[1], g.p->fFront[2]};
			float top[3] = { g.p->fTop[0], g.p->fTop[1], g.p->fTop[2]};

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
			for (unsigned int i=0;i<iChannels;++i) {
				speaker[3*i+0] = fSpeakers[3*i+0] * right[0] + fSpeakers[3*i+1] * top[0] + fSpeakers[3*i+2] * front[0];
				speaker[3*i+1] = fSpeakers[3*i+0] * right[1] + fSpeakers[3*i+1] * top[1] + fSpeakers[3*i+2] * front[1];
				speaker[3*i+2] = fSpeakers[3*i+0] * right[2] + fSpeakers[3*i+1] * top[2] + fSpeakers[3*i+2] * front[2];
			}
			validListener = true;
		}
		foreach(aop, qlMix) {

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
				for (unsigned int s=0;s<iChannels;++s) {
					float dot = bSpeakerPositional[s] ? dir[0] * speaker[s*3+0] + dir[1] * speaker[s*3+1] + dir[2] * speaker[s*3+2] : 1.0f;
					float str = mul * calcGain(dot, len) * fSpeakerVolume[s];
					/*
										qWarning("%d: Pos %f %f %f : Dot %f Len %f Str %f", s, speaker[s*3+0], speaker[s*3+1], speaker[s*3+2], dot, len, str);
					*/
					for (unsigned int i=0;i<nsamp;++i)
						output[i*iChannels+s] += aop->pfBuffer[i] * str;
				}
			} else {
				for (unsigned int i=0;i<nsamp;++i)
					for (unsigned int j=0;j<iChannels;++j)
						output[i*iChannels+j] += aop->pfBuffer[i] * fSpeakerVolume[j] * mul;
			}
		}
		// Clip
		for (unsigned int i=0;i<iFrameSize*iChannels;i++)
			output[i] = output[i] < -1.0f ? -1.0f : (output[i] > 1.0f ? 1.0f : output[i]);
	}

	qrwlOutputs.unlock();

	foreach(aop, qlDel)
		removeBuffer(aop);

	return (! qlMix.isEmpty());
}

AudioSine::AudioSine(float hz, float i, unsigned int frm, float vol, unsigned int freq) : AudioOutputPlayer(QLatin1String("Sine")) {
	float hfreq = freq / 2.0f;
	v = 0.0;
	inc = M_PI * hz / hfreq;
	dinc = M_PI * i / (hfreq * hfreq);
	volume = vol * 32768.f;
	frames = frm;
	cntr = 0;
	tbin = 4;

	if (inc == 0.0)
		g.iAudioPathTime = 0;
}

AudioSine::~AudioSine() {
}

bool AudioSine::needSamples(unsigned int snum) {
	if (frames > 0) {
		frames--;

		resizeBuffer(snum);

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

			// FIXME: Not * snum, * something else
			for (unsigned int i=0;i<snum;i++)
				pfBuffer[i] = volume * sinf(M_PI * i * (tbin * 1.0f) / (1.0f * snum));

			return true;
		}

		float t = v;

		for (unsigned int i=0;i<snum;i++) {
			pfBuffer[i]=sinf(t) * volume;
			inc+=dinc;
			t+=inc;
		}
		v = t;
		return true;
	} else {
		memset(pfBuffer, 0, sizeof(float) * snum);
		return false;
	}
}
