/* Copyright (C) 2005-2011, Thorvald Natvig <thorvald@natvig.com>
   Copyright (C) 2009-2011, Stefan Hacker <dd0t@users.sourceforge.net>

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

#include "mumble_pch.hpp"

#include "AudioOutput.h"

#include "AudioInput.h"
#include "AudioOutputSample.h"
#include "AudioOutputSpeech.h"
#include "User.h"
#include "Global.h"
#include "Message.h"
#include "Plugins.h"
#include "PacketDataStream.h"
#include "ServerHandler.h"
#include "VoiceRecorder.h"

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

bool AudioOutputRegistrar::canExclusive() const {
	return false;
}

AudioOutput::AudioOutput()
    : fSpeakers(NULL)
    , fSpeakerVolume(NULL)
    , bSpeakerPositional(NULL)
    
    , eSampleFormat(SampleFloat)
    
    , bRunning(true)
    
    , iFrameSize(SAMPLE_RATE / 100)
    , iMixerFreq(0)
    , iChannels(0)
    , iSampleSize(0)
    
    , qrwlOutputs()
    , qmOutputs() {
	
	// Nothing
}

AudioOutput::~AudioOutput() {
	bRunning = false;
	wait();
	wipe();

	delete [] fSpeakers;
	delete [] fSpeakerVolume;
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

void AudioOutput::addFrameToBuffer(ClientUser *user, const QByteArray &qbaPacket, unsigned int iSeq, MessageHandler::UDPMessageType type) {
	if (iChannels == 0)
		return;
	qrwlOutputs.lockForRead();
	AudioOutputSpeech *aop = qobject_cast<AudioOutputSpeech *>(qmOutputs.value(user));

	if (! aop || (aop->umtType != type)) {
		qrwlOutputs.unlock();

		if (aop)
			removeBuffer(aop);

		while ((iMixerFreq == 0) && isAlive()) {
			QThread::yieldCurrentThread();
		}

		if (! iMixerFreq)
			return;

		qrwlOutputs.lockForWrite();
		aop = new AudioOutputSpeech(user, iMixerFreq, type);
		qmOutputs.replace(user, aop);
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

AudioOutputSample *AudioOutput::playSample(const QString &filename, bool loop) {
	SoundFile *handle = AudioOutputSample::loadSndfile(filename);
	if (handle == NULL)
		return NULL;

	while ((iMixerFreq == 0) && isAlive()) {
		QThread::yieldCurrentThread();
	}

	if (! iMixerFreq)
		return NULL;

	QWriteLocker locker(&qrwlOutputs);
	AudioOutputSample *aos = new AudioOutputSample(filename, handle, loop, iMixerFreq);
	qmOutputs.insert(NULL, aos);

	return aos;

}

void AudioOutput::initializeMixer(const unsigned int *chanmasks, bool forceheadphone) {
	delete[] fSpeakers;
	delete[] bSpeakerPositional;
	delete[] fSpeakerVolume;

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
	QList<AudioOutputUser *> qlMix;
	QList<AudioOutputUser *> qlDel;
	
	if (g.s.fVolume < 0.01f) {
		return false;
	}

	const float adjustFactor = std::pow(10.f , -18.f / 20);
	const float mul = g.s.fVolume;
	const unsigned int nchan = iChannels;
	ServerHandlerPtr sh = g.sh;
	VoiceRecorderPtr recorder;
	if (sh) {
		recorder = g.sh->recorder;
	}

	qrwlOutputs.lockForRead();
	
	bool prioritySpeakerActive = false;
	
	QMultiHash<const ClientUser *, AudioOutputUser *>::const_iterator it = qmOutputs.constBegin();
	while (it != qmOutputs.constEnd()) {
		AudioOutputUser *aop = it.value();
		if (! aop->needSamples(nsamp)) {
			qlDel.append(aop);
		} else {
			qlMix.append(aop);
			
			const ClientUser *user = it.key();
			if (user && user->bPrioritySpeaker) {
				prioritySpeakerActive = true;
			}
		}
		++it;
	}

	if (g.prioritySpeakerActiveOverride) {
		prioritySpeakerActive = true;
	}

	if (! qlMix.isEmpty()) {
		STACKVAR(float, speaker, iChannels*3);
		STACKVAR(float, svol, iChannels);

		STACKVAR(float, fOutput, iChannels * nsamp);
		float *output = (eSampleFormat == SampleFloat) ? reinterpret_cast<float *>(outbuff) : fOutput;
		bool validListener = false;

		memset(output, 0, sizeof(float) * nsamp * iChannels);

		boost::shared_array<float> recbuff;
		if (recorder) {
			recbuff = boost::shared_array<float>(new float[nsamp]);
			memset(recbuff.get(), 0, sizeof(float) * nsamp);
			recorder->prepareBufferAdds();
		}

		for (unsigned int i=0;i<iChannels;++i)
			svol[i] = mul * fSpeakerVolume[i];

		if (g.s.bPositionalAudio && (iChannels > 1) && g.p->fetch() && (g.bPosTest || g.p->fCameraPosition[0] != 0 || g.p->fCameraPosition[1] != 0 || g.p->fCameraPosition[2] != 0)) {

			float front[3] = { g.p->fCameraFront[0], g.p->fCameraFront[1], g.p->fCameraFront[2] };
			float top[3] = { g.p->fCameraTop[0], g.p->fCameraTop[1], g.p->fCameraTop[2] };

			// Front vector is dominant; if it's zero we presume all is zero.

			float flen = sqrtf(front[0]*front[0]+front[1]*front[1]+front[2]*front[2]);

			if (flen > 0.0f) {
				front[0] *= (1.0f / flen);
				front[1] *= (1.0f / flen);
				front[2] *= (1.0f / flen);

				float tlen = sqrtf(top[0]*top[0]+top[1]*top[1]+top[2]*top[2]);

				if (tlen > 0.0f) {
					top[0] *= (1.0f / tlen);
					top[1] *= (1.0f / tlen);
					top[2] *= (1.0f / tlen);
				} else {
					top[0] = 0.0f;
					top[1] = 1.0f;
					top[2] = 0.0f;
				}

				if (std::abs<float>(front[0] * top[0] + front[1] * top[1] + front[2] * top[2]) > 0.01f) {
					// Not perpendicular. Assume Y up and rotate 90 degrees.

					float azimuth = 0.0f;
					if ((front[0] != 0.0f) || (front[2] != 0.0f))
						azimuth = atan2f(front[2], front[0]);
					float inclination = acosf(front[1]) - static_cast<float>(M_PI) / 2.0f;

					top[0] = sinf(inclination)*cosf(azimuth);
					top[1] = cosf(inclination);
					top[2] = sinf(inclination)*sinf(azimuth);
				}
			} else {
				front[0] = 0.0f;
				front[1] = 0.0f;
				front[2] = 1.0f;

				top[0] = 0.0f;
				top[1] = 1.0f;
				top[2] = 0.0f;
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

		foreach(AudioOutputUser *aop, qlMix) {
			const float * RESTRICT pfBuffer = aop->pfBuffer;
			float volumeAdjustment = 1;

			if (prioritySpeakerActive) {
				AudioOutputSpeech *speech = qobject_cast<AudioOutputSpeech *>(aop);
				if (speech) {
					const ClientUser* user = speech->p;
					
					if (user->tsState != Settings::Whispering
					    && !user->bPrioritySpeaker) {
						
						volumeAdjustment = adjustFactor;
					}
				}
			}

			if (recorder) {
				AudioOutputSpeech *aos = qobject_cast<AudioOutputSpeech *>(aop);

				if (aos) {
					for (unsigned int i = 0; i < nsamp; ++i) {
						recbuff[i] += pfBuffer[i] * volumeAdjustment;
					}

					if (!recorder->isInMixDownMode()) {
						if (aos) {
							recorder->addBuffer(aos->p, recbuff, nsamp);
						} else {
							// this should be unreachable
							Q_ASSERT(false);
						}
						recbuff = boost::shared_array<float>(new float[nsamp]);
						memset(recbuff.get(), 0, sizeof(float) * nsamp);
					}

					// Don't add the local audio to the real output
					if (qobject_cast<RecordUser *>(aos->p)) {
						continue;
					}
				}
			}

			if (validListener && ((aop->fPos[0] != 0.0f) || (aop->fPos[1] != 0.0f) || (aop->fPos[2] != 0.0f))) {
				float dir[3] = { aop->fPos[0] - g.p->fCameraPosition[0], aop->fPos[1] - g.p->fCameraPosition[1], aop->fPos[2] - g.p->fCameraPosition[2] };
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
					const float str = svol[s] * calcGain(dot, len) * volumeAdjustment;
					float * RESTRICT o = output + s;
					const float old = (aop->pfVolume[s] >= 0.0f) ? aop->pfVolume[s] : str;
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
					const float str = svol[s] * volumeAdjustment;
					float * RESTRICT o = output + s;
					for (unsigned int i=0;i<nsamp;++i)
						o[i*nchan] += pfBuffer[i] * str;
				}
			}
		}

		if (recorder && recorder->isInMixDownMode()) {
			recorder->addBuffer(NULL, recbuff, nsamp);
		}

		// Clip
		if (eSampleFormat == SampleFloat)
			for (unsigned int i=0;i<nsamp*iChannels;i++)
				output[i] = qBound(-1.0f, output[i], 1.0f);
		else
			for (unsigned int i=0;i<nsamp*iChannels;i++)
				reinterpret_cast<short *>(outbuff)[i] = static_cast<short>(qBound(-32768.f, (output[i] * 32768.f), 32767.f));
	}

	qrwlOutputs.unlock();

	foreach(AudioOutputUser *aop, qlDel)
		removeBuffer(aop);
	
	return (! qlMix.isEmpty());
}

bool AudioOutput::isAlive() const {
	return isRunning();
}

unsigned int AudioOutput::getMixerFreq() const {
	return iMixerFreq;
}

