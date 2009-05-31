/* Copyright (C) 2005-2009, Thorvald Natvig <thorvald@natvig.com>

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
#include "User.h"
#include "Global.h"
#include "Message.h"
#include "Plugins.h"
#include "PacketDataStream.h"

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

AudioOutputUser::AudioOutputUser(const QString name) : qsName(name) {
	iBufferSize = 0;
	pfBuffer = NULL;
	pfVolume = NULL;
	fPos[0]=fPos[1]=fPos[2]=0.0;
}

AudioOutputUser::~AudioOutputUser() {
	if (pfBuffer)
		delete [] pfBuffer;
	if (pfVolume)
		delete [] pfVolume;
}

void AudioOutputUser::resizeBuffer(unsigned int newsize) {
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

SoundFile::SoundFile(const QString &fname) {
	siInfo.frames = 0 ;
	siInfo.channels = 1 ;
	siInfo.samplerate = SAMPLE_RATE ;
	siInfo.samplerate = 0 ;
	siInfo.sections = 0 ;
	siInfo.seekable = 0 ;

	sfFile = NULL;

	qfFile.setFileName(fname);

	if (qfFile.open(QIODevice::ReadOnly)) {
		static SF_VIRTUAL_IO svi = {&SoundFile::vio_get_filelen, &SoundFile::vio_seek, &SoundFile::vio_read, &SoundFile::vio_write, &SoundFile::vio_tell};

		sfFile = sf_open_virtual(&svi, SFM_READ, &siInfo, this) ;
	}
}

SoundFile::~SoundFile() {
	if (sfFile)
		sf_close(sfFile);
}

bool SoundFile::isOpen() const {
	return (sfFile != NULL) && qfFile.isOpen();
}

int SoundFile::channels() const {
	return siInfo.channels;
}

int SoundFile::samplerate() const {
	return siInfo.samplerate;
}

int SoundFile::error() const {
	return sf_error(sfFile);
}

QString SoundFile::strError() const {
	return QLatin1String(sf_strerror(sfFile));
}

sf_count_t SoundFile::seek(sf_count_t frames, int whence) {
	return sf_seek(sfFile, frames, whence);
}

sf_count_t SoundFile::read(float *ptr, sf_count_t items) {
	return sf_read_float(sfFile, ptr, items);
}

sf_count_t SoundFile::vio_get_filelen(void *user_data) {
	SoundFile *sf = reinterpret_cast<SoundFile *>(user_data);

	if (! sf->qfFile.isOpen())
		return -1;

	return (sf->qfFile.size());
}

sf_count_t SoundFile::vio_seek(sf_count_t offset, int whence, void *user_data) {
	SoundFile *sf = reinterpret_cast<SoundFile *>(user_data);

	if (! sf->qfFile.isOpen())
		return -1;

	if (whence == SEEK_SET) {
		sf->qfFile.seek(offset);
	} else if (whence == SEEK_END) {
		sf->qfFile.seek(sf->qfFile.size() - offset);
	} else {
		sf->qfFile.seek(sf->qfFile.pos() + offset);
	}
	return sf->qfFile.pos();
}

sf_count_t SoundFile::vio_read(void *ptr, sf_count_t count, void *user_data) {
	SoundFile *sf = reinterpret_cast<SoundFile *>(user_data);

	if (! sf->qfFile.isOpen())
		return -1;

	return sf->qfFile.read(reinterpret_cast<char *>(ptr), count);
}

sf_count_t SoundFile::vio_write(const void *ptr, sf_count_t count, void *user_data) {
	SoundFile *sf = reinterpret_cast<SoundFile *>(user_data);

	if (! sf->qfFile.isOpen())
		return -1;

	return sf->qfFile.write(reinterpret_cast<const char *>(ptr), count);
}

sf_count_t SoundFile::vio_tell(void *user_data) {
	SoundFile *sf = reinterpret_cast<SoundFile *>(user_data);

	if (! sf->qfFile.isOpen())
		return -1;

	return sf->qfFile.pos();
}

AudioOutputSample::AudioOutputSample(const QString &name, SoundFile *psndfile, bool loop, unsigned int freq) : AudioOutputUser(name) {
	int err;

	sfHandle = psndfile;
	iOutSampleRate = freq;

	// Check if the file is good
	if (sfHandle->channels() <= 0 || sfHandle->channels() > 2) {
		sfHandle = NULL;
		return;
	}

	/* qWarning() << "Channels: " << sfHandle->channels();
	qWarning() << "Samplerate: " << sfHandle->samplerate();
	qWarning() << "Target Sr.: " << freq;
	qWarning() << "Format: " << sfHandle->format() << endl; */

	// If the frequencies don't match initialize the resampler
	if (sfHandle->samplerate() != freq) {
		srs = speex_resampler_init(1, sfHandle->samplerate(), iOutSampleRate, 3, &err);
		if (err != RESAMPLER_ERR_SUCCESS) {
			qWarning() << "Initialize " << sfHandle->samplerate() << " to " << iOutSampleRate << " resampler failed!";
			srs = NULL;
			sfHandle = NULL;
			return;
		}
	} else {
		srs = NULL;
	}

	iLastConsume = iBufferFilled = 0;
	bLoop = loop;
}

AudioOutputSample::~AudioOutputSample() {
	if (sfHandle) {
		delete sfHandle;
		sfHandle = NULL;
	}
}

SoundFile* AudioOutputSample::loadSndfile(const QString &filename) {
	SoundFile *sf;

	// Create the filehandle and do a quick check if everything is ok
	sf = new SoundFile(filename);

	if (! sf->isOpen()) {
		qWarning() << "File " << filename << " failed to open";
		delete sf;
		return NULL;
	}

	if (sf->error() != SF_ERR_NO_ERROR) {
		qWarning() << "File " << filename << " couldn't be loaded: " << sf->strError();
		delete sf;
		return NULL;
	}

	if (sf->channels() <= 0 || sf->channels() > 2) {
		qWarning() << "File " << filename << " contains " << sf->channels() << " Channels, only 1 or 2 are supported.";
		delete sf;
		return NULL;
	}
	return sf;
}

bool AudioOutputSample::needSamples(unsigned int snum) {
	// Forward the buffer
	for (unsigned int i=iLastConsume;i<iBufferFilled;++i)
		pfBuffer[i-iLastConsume]=pfBuffer[i];
	iBufferFilled -= iLastConsume;
	iLastConsume = snum;


	// Check if we can satisfy request with current buffer
	if (iBufferFilled >= snum)
		return true;

	// Calculate the required buffersize to hold the results
	unsigned int iInputFrames = iroundf(ceilf(static_cast<float>(snum * sfHandle->samplerate()) / static_cast<float>(iOutSampleRate)));
	unsigned int iInputSamples = iInputFrames * sfHandle->channels();

	float *pOut;
	bool mix = sfHandle->channels() > 1;
	STACKVAR(float, fOut, iInputSamples);
	STACKVAR(float, fMix, iInputFrames);

	bool eof = false;
	sf_count_t read;
	do {
		resizeBuffer(iBufferFilled + snum);

		// If we need to resample or mix write to the buffer on stack
		pOut = (srs || mix) ? fOut : pfBuffer + iBufferFilled;

		// Try to read all samples needed to satifsy this request
		if ((read = sfHandle->read(pOut, iInputSamples)) < iInputSamples) {
			if (sfHandle->error() != SF_ERR_NO_ERROR || !bLoop) {
				// We reached the eof or encountered an error, stuff with zeroes
				memset(pOut, 0, sizeof(float) * (iInputSamples - read));
				read = iInputSamples;
				eof = true;
			} else {
				sfHandle->seek(SEEK_SET, 0);
			}
		}

		if (mix) { // Mix the channels (only two channels)
			read /= 2;
			// If we need to resample after this write to extra buffer
			pOut = srs ? fMix : pfBuffer + iBufferFilled;
			for (unsigned int i = 0; i < read; i++)
				pOut[i] = (fOut[i*2] + fOut[i*2 + 1]) * 0.5f;

		}

		spx_uint32_t inlen = read;
		spx_uint32_t outlen = snum;
		if (srs) // If necessary resample
			speex_resampler_process_float(srs, 0, pOut, &inlen, pfBuffer + iBufferFilled, &outlen);

		iBufferFilled += outlen;
	} while (iBufferFilled < snum);
	return !eof;
}

AudioOutputSpeech::AudioOutputSpeech(ClientUser *user, unsigned int freq) : AudioOutputUser(user->qsName) {
	int err;
	p = user;

	iFrameSize = SAMPLE_RATE / 100;
	cmMode = celt_mode_create(SAMPLE_RATE, 1, iFrameSize, NULL);
	cdDecoder = celt_decoder_create(cmMode);

	if (freq != SAMPLE_RATE)
		srs = speex_resampler_init(1, SAMPLE_RATE, freq, 3, &err);
	else
		srs = NULL;

	iOutputSize = iroundf(ceilf(static_cast<float>(iFrameSize * freq) / static_cast<float>(SAMPLE_RATE)));

	iBufferOffset = iBufferFilled = iLastConsume = 0;
	bLastAlive = true;

	iMissCount = 0;
	iMissedFrames = 0;

	jbJitter = jitter_buffer_init(iFrameSize);
	int margin = g.s.iJitterBufferSize * iFrameSize;
	jitter_buffer_ctl(jbJitter, JITTER_BUFFER_SET_MARGIN, &margin);
}

AudioOutputSpeech::~AudioOutputSpeech() {
	celt_decoder_destroy(cdDecoder);
	celt_mode_destroy(cmMode);

	jitter_buffer_destroy(jbJitter);
}

void AudioOutputSpeech::addFrameToBuffer(const QByteArray &qbaPacket, unsigned int iSeq) {
	QMutexLocker lock(&qmJitter);

	if (qbaPacket.size() < 2)
		return;

	PacketDataStream pds(qbaPacket);

	pds.next();

	int frames = 0;
	unsigned int header = 0;
	do {
		header = pds.next();
		frames++;
		pds.skip(header & 0x7f);
	} while ((header & 0x80) && pds.isValid());

	if (pds.isValid()) {
		JitterBufferPacket jbp;
		jbp.data = const_cast<char *>(qbaPacket.constData());
		jbp.len = qbaPacket.size();
		jbp.span = iFrameSize * frames;
		jbp.timestamp = iFrameSize * iSeq;

		jitter_buffer_put(jbJitter, &jbp);
	}
}

bool AudioOutputSpeech::needSamples(unsigned int snum) {
	for (unsigned int i=iLastConsume;i<iBufferFilled;++i)
		pfBuffer[i-iLastConsume]=pfBuffer[i];
	iBufferFilled -= iLastConsume;

	iLastConsume = snum;

	if (iBufferFilled >= snum)
		return bLastAlive;

	float *pOut;
	STACKVAR(float, fOut, iFrameSize + 4096);

	while (iBufferFilled < snum) {
		resizeBuffer(iBufferFilled + iOutputSize);

		pOut = (srs) ? fOut : (pfBuffer + iBufferFilled);

		if (p == &LoopUser::lpLoopy)
			LoopUser::lpLoopy.fetchFrames();

		if (qlFrames.isEmpty()) {
			QMutexLocker lock(&qmJitter);

			char data[4096];
			JitterBufferPacket jbp;
			jbp.data = data;
			jbp.len = 4096;

			spx_int32_t startofs = 0;

			if (jitter_buffer_get(jbJitter, &jbp, iFrameSize, &startofs) == JITTER_BUFFER_OK) {
				PacketDataStream pds(jbp.data, jbp.len);

				ucFlags = pds.next();

				unsigned int header = 0;
				do {
					header = pds.next();
					qlFrames << pds.dataBlock(header & 0x7f);
				} while ((header & 0x80) && pds.isValid());

				if (pds.left()) {
					pds >> fPos[0];
					pds >> fPos[1];
					pds >> fPos[2];
				} else {
					fPos[0] = fPos[1] = fPos[2] = 0.0f;
				}
				if (pds.isValid()) {
					iMissCount = 0;
					bLastAlive = true;
				}
			} else {
				iMissCount++;
				if (iMissCount > 10) {
					memset(pOut, 0, sizeof(float) * iFrameSize);
					bLastAlive = false;
				}
			}

			jitter_buffer_update_delay(jbJitter, &jbp, NULL);
		}

		jitter_buffer_tick(jbJitter);

		if (! qlFrames.isEmpty()) {
			QByteArray qba = qlFrames.takeFirst();
			if (! qba.isEmpty()) {
				celt_decode_float(cdDecoder, reinterpret_cast<unsigned char *>(qba.data()), qba.size(), pOut);
				bLastAlive = true;
			} else {
				bLastAlive = false;
			}
		} else {
			celt_decode_float(cdDecoder, NULL, 0, pOut);
		}
		spx_uint32_t inlen = iFrameSize;
		spx_uint32_t outlen = iOutputSize;
		if (srs && bLastAlive)
			speex_resampler_process_float(srs, 0, fOut, &inlen, pfBuffer + iBufferFilled, &outlen);
		iBufferFilled += outlen;
	}

	if (p)
		p->setTalking(bLastAlive, ((ucFlags != 0) ? true : false));
	return bLastAlive;
}


AudioOutput::AudioOutput() {
	iFrameSize = SAMPLE_RATE / 100;
	bRunning = false;

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
	foreach(AudioOutputUser *aop, qmOutputs)
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
	while ((iMixerFreq == 0) && isRunning()) {}

	qrwlOutputs.lockForWrite();
	AudioSine *as = new AudioSine(hz,i,frames, volume, iMixerFreq);
	qmOutputs.insert(NULL, as);
	qrwlOutputs.unlock();
	return as;
}

AudioOutputSample *AudioOutput::playSample(const QString &filename, bool loop) {
	SoundFile *handle;
	handle = AudioOutputSample::loadSndfile(filename);
	if (handle == NULL)
		return NULL;

	while ((iMixerFreq == 0) && isRunning()) {}

	qrwlOutputs.lockForWrite();
	AudioOutputSample *aos = new AudioOutputSample(filename, handle, loop, iMixerFreq);
	qmOutputs.insert(NULL, aos);
	qrwlOutputs.unlock();

	return aos;

}

void AudioOutput::addFrameToBuffer(ClientUser *user, const QByteArray &qbaPacket, unsigned int iSeq) {
	if (iChannels == 0)
		return;
	qrwlOutputs.lockForRead();
	AudioOutputSpeech *aop = dynamic_cast<AudioOutputSpeech *>(qmOutputs.value(user));

	if (! aop) {
		qrwlOutputs.unlock();

		while ((iMixerFreq == 0) && isRunning()) {}

		qrwlOutputs.lockForWrite();
		aop = new AudioOutputSpeech(user, iMixerFreq);
		qmOutputs.replace(user,aop);
	}

	aop->addFrameToBuffer(qbaPacket, iSeq);

	qrwlOutputs.unlock();
}

void AudioOutput::removeBuffer(const ClientUser *user) {
	removeBuffer(qmOutputs.value(user));
}

void AudioOutput::removeBuffer(AudioOutputUser *aop) {
	QWriteLocker locker(&qrwlOutputs);
	QMultiHash<const ClientUser *, AudioOutputUser *>::iterator i;
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
	iSampleSize = static_cast<int>(iChannels * ((eSampleFormat == SampleFloat) ? sizeof(float) : sizeof(short)));
	qWarning("AudioOutput: Initialized %d channel %d hz mixer", iChannels, iMixerFreq);
}

bool AudioOutput::mix(void *outbuff, unsigned int nsamp) {
	AudioOutputUser *aop;
	QList<AudioOutputUser *> qlMix;
	QList<AudioOutputUser *> qlDel;

	if (g.s.fVolume < 0.01)
		return false;

	const float mul = g.s.fVolume;
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

AudioSine::AudioSine(float hz, float i, unsigned int frm, float vol, unsigned int freq) : AudioOutputUser(QLatin1String("Sine")) {
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
				cntr = 0;
				tbin *= 2;
				if (tbin >= 64)
					tbin = 4;
			}

			AudioInputPtr ai = g.ai;

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
