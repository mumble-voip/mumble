/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>

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

AudioOutputRegistrar::AudioOutputRegistrar(const QString &n, int p) : name(n), priority(p) {
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

	AudioOutputRegistrar *r = NULL;
	foreach(AudioOutputRegistrar *aor, *qmNew)
		if (!r || (aor->priority > r->priority))
			r = aor;
	if (r) {
		current = r->name;
		return AudioOutputPtr(r->create());
	}
	return AudioOutputPtr();
}

bool AudioOutputRegistrar::canMuteOthers() const {
	return false;
}

bool AudioOutputRegistrar::usesOutputDelay() const {
	return true;
}

AudioOutputPlayer::AudioOutputPlayer(const QString name) : qsName(name) {
	iBufferSize = 0;
	pfBuffer = NULL;
	pfVolume = NULL;
	fPos[0]=fPos[1]=fPos[2]=0.0;
}

AudioOutputPlayer::~AudioOutputPlayer() {
	if (pfBuffer)
		delete [] pfBuffer;
	if (pfVolume)
		delete [] pfVolume;
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

AudioOutputSample::AudioOutputSample(const QString &filename, const QList<QByteArray> &packets, bool loop, unsigned int freq) : AudioOutputPlayer(filename) {
	dsDecState=speex_decoder_init(speex_lib_get_mode(SPEEX_MODEID_WB));

	int iArg=1;
	int err;

	speex_decoder_ctl(dsDecState, SPEEX_SET_ENH, &iArg);
	speex_decoder_ctl(dsDecState, SPEEX_GET_FRAME_SIZE, &iFrameSize);

	if (freq != SAMPLE_RATE)
		srs = speex_resampler_init(1, SAMPLE_RATE, freq, 3, &err);
	else
		srs = NULL;

	iOutputSize = lroundf(ceilf(static_cast<float>(iFrameSize * freq) / static_cast<float>(SAMPLE_RATE)));

	iBufferOffset = iBufferFilled = iLastConsume = 0;

	bLoop = false;

	iPacketIndex = -1;

	qlPackets = packets;

	speex_bits_init(&sbBits);

	bLoop = loop;
}

AudioOutputSample::~AudioOutputSample() {
	speex_decoder_destroy(dsDecState);
	speex_bits_destroy(&sbBits);
}

QList<QByteArray> AudioOutputSample::getPacketsFromFile(const QString &filename) {
	QList<QByteArray> packets;

	QFile f(filename);
	if (! f.open(QIODevice::ReadOnly)) {
		qWarning("AudioOutputSample: Failed to open %s for reading", qPrintable(filename));
		return QList<QByteArray>();
	}

	QByteArray qbaSample = f.readAll();
	f.close();

	if (qbaSample.startsWith(QString::fromLatin1("OggS").toUtf8())) {
		ogg_sync_state sync;
		ogg_page page;
		ogg_packet packet;
		ogg_stream_state stream;
		bool stream_init = false;
		bool eos = false;
		int speex_serialno = -1;
		int packetno = -1;
		bool header_ok = false;
		int nframes = -1;
		int extra_headers = 0;

		ogg_sync_init(&sync);
		char *data = ogg_sync_buffer(&sync, qbaSample.length());
		memcpy(data, qbaSample.constData(), qbaSample.length());
		ogg_sync_wrote(&sync, qbaSample.length());

		while (ogg_sync_pageout(&sync, &page)==1) {
			if (! stream_init)  {
				ogg_stream_init(&stream, ogg_page_serialno(&page));
				stream_init = true;
			} else if (ogg_page_serialno(&page) != stream.serialno) {
				ogg_stream_reset_serialno(&stream, ogg_page_serialno(&page));
			}
			ogg_stream_pagein(&stream, &page);
			while (! eos && ogg_stream_packetout(&stream, &packet) == 1) {
				if (packet.bytes >= 5 && memcmp(packet.packet, "Speex", 5)==0) {
					speex_serialno = stream.serialno;
				}
				if (speex_serialno == -1 || stream.serialno != speex_serialno)
					break;

				++packetno;

				if (packetno == 0) {
					SpeexHeader *header = speex_packet_to_header(reinterpret_cast<char *>(packet.packet), packet.bytes);

					if (header && header->speex_version_id == 1 && header->rate == 16000 && header->nb_channels == 1) {
						header_ok = true;
						extra_headers = header->extra_headers;
						nframes = header->frames_per_packet;
					} else {
						break;
					}
				} else if (packetno <= 1 + extra_headers) {
				} else {
					if (packet.e_o_s)
						eos = true;
					packets << QByteArray(reinterpret_cast<const char *>(packet.packet), packet.bytes);
				}
			}
		}
		ogg_sync_clear(&sync);
		if (stream_init)
			ogg_stream_clear(&stream);

		if (! header_ok) {
			qWarning("AudioOutputSample: %s contained invalid data", qPrintable(filename));
		}
	} else {
		qWarning("AudioOutputSample: %s is not an ogg file", qPrintable(filename));
	}
	return packets;
}

bool AudioOutputSample::needSamples(unsigned int snum) {
	for (unsigned int i=iLastConsume;i<iBufferFilled;++i)
		pfBuffer[i-iLastConsume]=pfBuffer[i];
	iBufferFilled -= iLastConsume;

	iLastConsume = snum;

	if (iBufferFilled >= snum)
		return bLastAlive;

	float *pOut;
	STACKVAR(float, fOut, iFrameSize);


	while (iBufferFilled < snum) {
		resizeBuffer(iBufferFilled + iOutputSize);

		pOut = (srs) ? fOut : pfBuffer + iBufferFilled;

		if (speex_bits_remaining(&sbBits) < 43) {
			++iPacketIndex;
			if (iPacketIndex >= qlPackets.count()) {
				if (qlPackets.isEmpty() || !bLoop) {
					memset(pfBuffer + iBufferFilled, 0, sizeof(float) * (snum-iBufferFilled));
					return false;
				} else {
					iPacketIndex = 0;
				}
			}

			const QByteArray &qba = qlPackets[iPacketIndex];

			speex_bits_set_bit_buffer(&sbBits, reinterpret_cast<void *>(const_cast<char *>(qba.constData())), qba.length());
			speex_bits_rewind(&sbBits);
		}

		if (speex_decode(dsDecState, &sbBits, pOut) != 0) {
			memset(pfBuffer + iBufferFilled, 0, sizeof(float) * (snum-iBufferFilled));
			return false;
		}

		spx_uint32_t inlen = iFrameSize;
		spx_uint32_t outlen = iOutputSize;
		if (srs)
			speex_resampler_process_float(srs, 0, fOut, &inlen, pfBuffer + iBufferFilled, &outlen);
		iBufferFilled += outlen;
	}

	return true;
}

AudioOutputSpeech::AudioOutputSpeech(ClientPlayer *player, unsigned int freq) : AudioOutputPlayer(player->qsName) {
	p = player;

	dsDecState=speex_decoder_init(speex_lib_get_mode(SPEEX_MODEID_WB));

	int iArg=1;
	int err;

	speex_decoder_ctl(dsDecState, SPEEX_SET_ENH, &iArg);
	speex_decoder_ctl(dsDecState, SPEEX_GET_FRAME_SIZE, &iFrameSize);

	if (freq != SAMPLE_RATE)
		srs = speex_resampler_init(1, SAMPLE_RATE, freq, 3, &err);
	else
		srs = NULL;

	iOutputSize = lroundf(ceilf(static_cast<float>(iFrameSize * freq) / static_cast<float>(SAMPLE_RATE)));

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
		ptr[i]=static_cast<unsigned char>(speex_bits_unpack_unsigned(bits, 8));
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
	for (unsigned int i=iLastConsume;i<iBufferFilled;++i)
		pfBuffer[i-iLastConsume]=pfBuffer[i];
	iBufferFilled -= iLastConsume;

	iLastConsume = snum;

	if (iBufferFilled >= snum)
		return bLastAlive;

	float *pOut;
	STACKVAR(float, fOut, iFrameSize);


	while (iBufferFilled < snum) {
		resizeBuffer(iBufferFilled + iOutputSize);

		pOut = (srs) ? fOut : pfBuffer + iBufferFilled;

		if (p == &LoopPlayer::lpLoopy)
			LoopPlayer::lpLoopy.fetchFrames();


		if (speex_decode(dsDecState, &sbBits, pOut) == 0) {
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
				speex_decode(dsDecState, &sbBits, pOut);
				bLastAlive = true;
			} else {
				if (ucFlags & MessageSpeex::EndSpeech) {
					memset(pOut, 0, sizeof(float) * iFrameSize);
					bLastAlive = false;
				} else {
					iMissCount++;
					if (iMissCount < 5) {
						speex_decode(dsDecState, NULL, pOut);
					} else {
						memset(pOut, 0, sizeof(float) * iFrameSize);
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
		if (srs)
			speex_resampler_process_float(srs, 0, fOut, &inlen, pfBuffer + iBufferFilled, &outlen);
		iBufferFilled += outlen;
	}

	if (p)
		p->setTalking(bLastAlive, ((ucFlags & MessageSpeex::AltSpeak) ? true : false));
	return bLastAlive;
}


AudioOutput::AudioOutput() {
	void *ds=speex_decoder_init(speex_lib_get_mode(SPEEX_MODEID_WB));
	speex_decoder_ctl(ds, SPEEX_GET_FRAME_SIZE, &iFrameSize);
	speex_decoder_destroy(ds);
	bRunning = true;

	iChannels = 0;
	fSpeakers = NULL;
	fSpeakerVolume = NULL;
	bSpeakerPositional = NULL;

	iMixerFreq = 0;
	eSampleFormat = SampleFloat;
	iSampleSize = 0;
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
	if (g.s.fAudioMaxDistVolume > 0.99f) {
		att = qMin(1.0f, dotfactor + g.s.fAudioBloom);
	} else if (distance < g.s.fAudioMinDistance) {
		float bloomfac = g.s.fAudioBloom * (1.0f - distance/g.s.fAudioMinDistance);

		att = qMin(1.0f, bloomfac + dotfactor);
	} else {
		float datt;

		if (distance >= g.s.fAudioMaxDistance) {
			datt = g.s.fAudioMaxDistVolume;
		} else {
			float mvol = g.s.fAudioMaxDistVolume;
			if (mvol < 0.01f)
				mvol = 0.01f;

			float drel = (distance-g.s.fAudioMinDistance) / (g.s.fAudioMaxDistance - g.s.fAudioMinDistance);
			datt = powf(10.0f, log10f(mvol) * drel);
		}

		att = datt * dotfactor;
	}
	return att;
}

void AudioOutput::wipe() {
	foreach(AudioOutputPlayer *aop, qmOutputs)
		removeBuffer(aop);
}

const float *AudioOutput::getSpeakerPos(unsigned int &speakers) {
	if ((iChannels > 0) && fSpeakers) {
		speakers = iChannels;
		return fSpeakers;
	}
	return NULL;
}

AudioSine *AudioOutput::playSine(float hz, float i, unsigned int frames, float volume) {
	while ((iMixerFreq == 0) && isAlive()) {}
	if (! iMixerFreq)
		return NULL;

	qrwlOutputs.lockForWrite();
	AudioSine *as = new AudioSine(hz,i,frames, volume, iMixerFreq);
	qmOutputs.insert(NULL, as);
	qrwlOutputs.unlock();
	return as;
}

AudioOutputSample *AudioOutput::playSample(const QString &filename, bool loop) {
	QList<QByteArray> packets = AudioOutputSample::getPacketsFromFile(filename);
	if (packets.isEmpty())
		return NULL;

	while ((iMixerFreq == 0) && isAlive()) {}
	if (! iMixerFreq)
		return NULL;

	qrwlOutputs.lockForWrite();
	AudioOutputSample *aos = new AudioOutputSample(filename, packets, loop, iMixerFreq);
	qmOutputs.insert(NULL, aos);
	qrwlOutputs.unlock();
	return aos;

}

void AudioOutput::addFrameToBuffer(ClientPlayer *player, const QByteArray &qbaPacket, unsigned int iSeq) {
	if (iChannels == 0)
		return;
	qrwlOutputs.lockForRead();
	AudioOutputSpeech *aop = dynamic_cast<AudioOutputSpeech *>(qmOutputs.value(player));
	if (! aop) {
		qrwlOutputs.unlock();

		while ((iMixerFreq == 0) && isAlive()) {}
		if (! iMixerFreq)
			return;

		qrwlOutputs.lockForWrite();
		aop = new AudioOutputSpeech(player, iMixerFreq);
		qmOutputs.replace(player,aop);
	}

	aop->addFrameToBuffer(qbaPacket, iSeq);

	qrwlOutputs.unlock();
}

void AudioOutput::removeBuffer(const ClientPlayer *player) {
	removeBuffer(qmOutputs.value(player));
}

void AudioOutput::removeBuffer(AudioOutputPlayer *aop) {
	QWriteLocker locker(&qrwlOutputs);
	QMultiHash<const ClientPlayer *, AudioOutputPlayer *>::iterator i;
	for (i=qmOutputs.begin(); i != qmOutputs.end(); ++i) {
		if (i.value() == aop) {
			qmOutputs.erase(i);
			delete aop;
			break;
		}
	}
}

void AudioOutput::initializeMixer(const unsigned int *chanmasks, bool forceheadphone) {
	if (fSpeakers)
		delete fSpeakers;
	if (bSpeakerPositional)
		delete bSpeakerPositional;
	if (fSpeakerVolume)
		delete fSpeakerVolume;

	fSpeakers = new float[iChannels * 3];
	bSpeakerPositional = new bool[iChannels];
	fSpeakerVolume = new float[iChannels];

	memset(fSpeakers, 0, sizeof(float) * iChannels * 3);
	memset(bSpeakerPositional, 0, sizeof(bool) * iChannels);

	for (unsigned int i=0;i<iChannels;++i)
		fSpeakerVolume[i] = 1.0f;

	if (g.s.bPositionalAudio && (iChannels > 1)) {
		for (unsigned int i=0;i<iChannels;i++) {
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
			if (g.s.bPositionalHeadphone || forceheadphone) {
				s[1] = 0.0f;
				s[2] = 0.0f;
				if (s[0] == 0.0f)
					fSpeakerVolume[i] = 0.0f;
			}
		}
		for (unsigned int i=0;i<iChannels;i++) {
			float d = sqrtf(fSpeakers[3*i+0]*fSpeakers[3*i+0] + fSpeakers[3*i+1]*fSpeakers[3*i+1] + fSpeakers[3*i+2]*fSpeakers[3*i+2]);
			if (d > 0.0f) {
				fSpeakers[3*i+0] /= d;
				fSpeakers[3*i+1] /= d;
				fSpeakers[3*i+2] /= d;
			}
		}
	}
	iSampleSize = iChannels * ((eSampleFormat == SampleFloat) ? sizeof(float) : sizeof(short));
	qWarning("AudioOutput: Initialized %d channel %d hz mixer", iChannels, iMixerFreq);
}

bool AudioOutput::mix(void *outbuff, unsigned int nsamp) {
	AudioOutputPlayer *aop;
	QList<AudioOutputPlayer *> qlMix;
	QList<AudioOutputPlayer *> qlDel;


	if (g.s.fVolume < 0.01)
		return false;

	const float mul = g.s.fVolume / 32768.0f;
	const unsigned int nchan = iChannels;

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
		STACKVAR(float, svol, iChannels);

		STACKVAR(float, fOutput, iChannels * nsamp);
		float *output = (eSampleFormat == SampleFloat) ? reinterpret_cast<float *>(outbuff) : fOutput;
		bool validListener = false;

		memset(output, 0, sizeof(float) * nsamp * iChannels);

		for (unsigned int i=0;i<iChannels;++i)
			svol[i] = mul * fSpeakerVolume[i];

		if (g.s.bPositionalAudio && (iChannels > 1) && g.p->fetch() && (g.bPosTest || g.p->fPosition[0] != 0 || g.p->fPosition[1] != 0 || g.p->fPosition[2] != 0)) {
			float front[3] = { g.p->fFront[0], g.p->fFront[1], g.p->fFront[2] };
			float top[3] = { g.p->fTop[0], g.p->fTop[1], g.p->fTop[2] };

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
			const float * RESTRICT pfBuffer = aop->pfBuffer;

			if (validListener && ((aop->fPos[0] != 0.0f) || (aop->fPos[1] != 0.0f) || (aop->fPos[2] != 0.0f))) {
				float dir[3] = { aop->fPos[0] - g.p->fPosition[0], aop->fPos[1] - g.p->fPosition[1], aop->fPos[2] - g.p->fPosition[2] };
				float len = sqrtf(dir[0] * dir[0] + dir[1] * dir[1] + dir[2] * dir[2]);
				if (len > 0.0f) {
					dir[0] /= len;
					dir[1] /= len;
					dir[2] /= len;
				}
				/*
								qWarning("Voice pos: %f %f %f", aop->fPos[0], aop->fPos[1], aop->fPos[2]);
								qWarning("Voice dir: %f %f %f", dir[0], dir[1], dir[2]);
				*/
				if (! aop->pfVolume) {
					aop->pfVolume = new float[nchan];
					for (unsigned int s=0;s<nchan;++s)
						aop->pfVolume[s] = -1.0;
				}
				for (unsigned int s=0;s<nchan;++s) {
					const float dot = bSpeakerPositional[s] ? dir[0] * speaker[s*3+0] + dir[1] * speaker[s*3+1] + dir[2] * speaker[s*3+2] : 1.0f;
					const float str = svol[s] * calcGain(dot, len);
					float * RESTRICT o = output + s;
					const float old = (aop->pfVolume[s] >= 0.0) ? aop->pfVolume[s] : str;
					const float inc = (str - old) / static_cast<float>(nsamp);
					aop->pfVolume[s] = str;
					/*
										qWarning("%d: Pos %f %f %f : Dot %f Len %f Str %f", s, speaker[s*3+0], speaker[s*3+1], speaker[s*3+2], dot, len, str);
					*/
					if ((old >= 0.00000001f) || (str >= 0.00000001f))
						for (unsigned int i=0;i<nsamp;++i)
							o[i*nchan] += pfBuffer[i] * (old + inc*static_cast<float>(i));
				}
			} else {
				for (unsigned int s=0;s<nchan;++s) {
					const float str = svol[s];
					float * RESTRICT o = output + s;
					for (unsigned int i=0;i<nsamp;++i)
						o[i*nchan] += pfBuffer[i] * str;
				}
			}
		}
		// Clip
		if (eSampleFormat == SampleFloat)
			for (unsigned int i=0;i<nsamp*iChannels;i++)
				output[i] = output[i] < -1.0f ? -1.0f : (output[i] > 1.0f ? 1.0f : output[i]);
		else
			for (unsigned int i=0;i<nsamp*iChannels;i++)
				reinterpret_cast<short *>(outbuff)[i] = static_cast<short>(32768.f * (output[i] < -1.0f ? -1.0f : (output[i] > 1.0f ? 1.0f : output[i])));
	}

	qrwlOutputs.unlock();

	foreach(aop, qlDel)
		removeBuffer(aop);

	return (! qlMix.isEmpty());
}

AudioSine::AudioSine(float hz, float i, unsigned int frm, float vol, unsigned int freq) : AudioOutputPlayer(QLatin1String("Sine")) {
	float hfreq = static_cast<float>(freq) / 2.0f;
	v = 0.0;
	inc = static_cast<float>(M_PI * hz / hfreq);
	dinc = static_cast<float>(M_PI * i / (hfreq * hfreq));
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
	resizeBuffer(snum);
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

			// FIXME: Not * snum, * something else
			const float m = static_cast<float>(M_PI * static_cast<float>(tbin) / static_cast<float>(snum));
			for (unsigned int i=0;i<snum;i++)
				pfBuffer[i] = volume * sinf(m * static_cast<float>(i));

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

bool AudioOutput::isAlive() const {
	return isRunning();
}
