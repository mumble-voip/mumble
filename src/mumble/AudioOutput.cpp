// Copyright 2005-2020 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

// <cmath> includes <math.h>, but only if it isn't already included.
// We include <cmath> as first header to make sure that we include <math.h> with _USE_MATH_DEFINES.
#ifdef _MSC_VER
#	define _USE_MATH_DEFINES
#endif

#include <cmath>

#include "AudioOutput.h"

#include "AudioInput.h"
#include "AudioOutputSample.h"
#include "AudioOutputSpeech.h"
#include "Channel.h"
#include "ChannelListener.h"
#include "Message.h"
#include "PacketDataStream.h"
#include "Plugins.h"
#include "ServerHandler.h"
#include "SpeechFlags.h"
#include "Timer.h"
#include "User.h"
#include "Utils.h"
#include "VoiceRecorder.h"

// We define a global macro called 'g'. This can lead to issues when included code uses 'g' as a type or parameter name
// (like protobuf 3.7 does). As such, for now, we have to make this our last include.
#include "Global.h"

// Remember that we cannot use static member classes that are not pointers, as the constructor
// for AudioOutputRegistrar() might be called before they are initialized, as the constructor
// is called from global initialization.
// Hence, we allocate upon first call.

QMap< QString, AudioOutputRegistrar * > *AudioOutputRegistrar::qmNew;
QString AudioOutputRegistrar::current = QString();

AudioOutputRegistrar::AudioOutputRegistrar(const QString &n, int p) : name(n), priority(p) {
	if (!qmNew)
		qmNew = new QMap< QString, AudioOutputRegistrar * >();
	qmNew->insert(name, this);
}

AudioOutputRegistrar::~AudioOutputRegistrar() {
	qmNew->remove(name);
}

AudioOutputPtr AudioOutputRegistrar::newFromChoice(QString choice) {
	if (!qmNew)
		return AudioOutputPtr();

	if (!choice.isEmpty() && qmNew->contains(choice)) {
		g.s.qsAudioOutput = choice;
		current           = choice;
		return AudioOutputPtr(qmNew->value(choice)->create());
	}
	choice = g.s.qsAudioOutput;
	if (qmNew->contains(choice)) {
		current = choice;
		return AudioOutputPtr(qmNew->value(choice)->create());
	}

	AudioOutputRegistrar *r = nullptr;
	foreach (AudioOutputRegistrar *aor, *qmNew)
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

AudioOutput::~AudioOutput() {
	bRunning = false;
	wait();
	wipe();

	delete[] fSpeakers;
	delete[] fSpeakerVolume;
	delete[] bSpeakerPositional;
	delete[] fStereoPanningFactor;
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
		float bloomfac = g.s.fAudioBloom * (1.0f - distance / g.s.fAudioMinDistance);

		att = qMin(1.0f, bloomfac + dotfactor);
	} else {
		float datt;

		if (distance >= g.s.fAudioMaxDistance) {
			datt = g.s.fAudioMaxDistVolume;
		} else {
			float mvol = g.s.fAudioMaxDistVolume;
			if (mvol < 0.01f)
				mvol = 0.01f;

			float drel = (distance - g.s.fAudioMinDistance) / (g.s.fAudioMaxDistance - g.s.fAudioMinDistance);
			datt       = powf(10.0f, log10f(mvol) * drel);
		}

		att = datt * dotfactor;
	}
	return att;
}

void AudioOutput::wipe() {
	foreach (AudioOutputUser *aop, qmOutputs)
		removeBuffer(aop);
}

const float *AudioOutput::getSpeakerPos(unsigned int &speakers) {
	if ((iChannels > 0) && fSpeakers) {
		speakers = iChannels;
		return fSpeakers;
	}
	return nullptr;
}

void AudioOutput::addFrameToBuffer(ClientUser *user, const QByteArray &qbaPacket, unsigned int iSeq,
								   MessageHandler::UDPMessageType type) {
	if (iChannels == 0)
		return;
	qrwlOutputs.lockForRead();
	// qmOutputs is a map of users and their AudioOutputUser objects, which will be create when audio from that user
	// is received. This map will be iterated in mix(). After one's audio is finished, his AudioOutputUser will be
	// removed from this map.
	AudioOutputSpeech *aop = qobject_cast< AudioOutputSpeech * >(qmOutputs.value(user));

	if (!UDPMessageTypeIsValidVoicePacket(type)) {
		qWarning("AudioOutput: ignored frame with invalid message type 0x%x in addFrameToBuffer().",
				 static_cast< unsigned char >(type));
		return;
	}

	if (!aop || (aop->umtType != type)) {
		qrwlOutputs.unlock();

		if (aop)
			removeBuffer(aop);

		while ((iMixerFreq == 0) && isAlive()) {
			QThread::yieldCurrentThread();
		}

		if (!iMixerFreq)
			return;

		qrwlOutputs.lockForWrite();
		aop = new AudioOutputSpeech(user, iMixerFreq, type, iBufferSize);
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
	QMultiHash< const ClientUser *, AudioOutputUser * >::iterator i;
	for (i = qmOutputs.begin(); i != qmOutputs.end(); ++i) {
		if (i.value() == aop) {
			qmOutputs.erase(i);
			delete aop;
			break;
		}
	}
}

AudioOutputSample *AudioOutput::playSample(const QString &filename, bool loop) {
	SoundFile *handle = AudioOutputSample::loadSndfile(filename);
	if (!handle)
		return nullptr;

	Timer t;
	const quint64 oneSecond = 1000000;

	while (!t.isElapsed(oneSecond) && (iMixerFreq == 0) && isAlive()) {
		QThread::yieldCurrentThread();
	}

	// If we've waited for more than one second, we declare timeout.
	if (t.isElapsed(oneSecond)) {
		qWarning("AudioOutput: playSample() timed out after 1 second: device not ready");
		return nullptr;
	}

	if (!iMixerFreq)
		return nullptr;

	QWriteLocker locker(&qrwlOutputs);
	AudioOutputSample *aos = new AudioOutputSample(filename, handle, loop, iMixerFreq, iBufferSize);
	qmOutputs.insert(nullptr, aos);

	return aos;
}

void AudioOutput::initializeMixer(const unsigned int *chanmasks, bool forceheadphone) {
	delete[] fSpeakers;
	delete[] bSpeakerPositional;
	delete[] fSpeakerVolume;
	delete[] fStereoPanningFactor;

	fSpeakers            = new float[iChannels * 3];
	bSpeakerPositional   = new bool[iChannels];
	fSpeakerVolume       = new float[iChannels];
	fStereoPanningFactor = new float[iChannels * 2];

	memset(fSpeakers, 0, sizeof(float) * iChannels * 3);
	memset(bSpeakerPositional, 0, sizeof(bool) * iChannels);

	for (unsigned int i = 0; i < iChannels; ++i)
		fSpeakerVolume[i] = 1.0f;

	if (iChannels > 1) {
		for (unsigned int i = 0; i < iChannels; i++) {
			float *s              = &fSpeakers[3 * i];
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
					fSpeakerVolume[i]     = 0.0f;
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
		for (unsigned int i = 0; i < iChannels; i++) {
			float d = sqrtf(fSpeakers[3 * i + 0] * fSpeakers[3 * i + 0] + fSpeakers[3 * i + 1] * fSpeakers[3 * i + 1]
							+ fSpeakers[3 * i + 2] * fSpeakers[3 * i + 2]);
			if (d > 0.0f) {
				fSpeakers[3 * i + 0] /= d;
				fSpeakers[3 * i + 1] /= d;
				fSpeakers[3 * i + 2] /= d;
			}
			float *spf = &fStereoPanningFactor[2 * i];
			spf[0]     = (1.0 - fSpeakers[i * 3 + 0]) / 2.0;
			spf[1]     = (1.0 + fSpeakers[i * 3 + 0]) / 2.0;
		}
	} else if (iChannels == 1) {
		fStereoPanningFactor[0] = 0.5;
		fStereoPanningFactor[1] = 0.5;
	}
	iSampleSize = static_cast< int >(iChannels * ((eSampleFormat == SampleFloat) ? sizeof(float) : sizeof(short)));
	qWarning("AudioOutput: Initialized %d channel %d hz mixer", iChannels, iMixerFreq);
}

bool AudioOutput::mix(void *outbuff, unsigned int frameCount) {
#ifdef USE_MANUAL_PLUGIN
	positions.clear();
#endif

	// A list of users that have audio to contribute
	QList< AudioOutputUser * > qlMix;
	// A list of users that no longer have any audio to play and can thus be deleted
	QList< AudioOutputUser * > qlDel;

	if (g.s.fVolume < 0.01f) {
		return false;
	}

	const float adjustFactor = std::pow(10.f, -18.f / 20);
	const float mul          = g.s.fVolume;
	const unsigned int nchan = iChannels;
	ServerHandlerPtr sh      = g.sh;
	VoiceRecorderPtr recorder;
	if (sh) {
		recorder = g.sh->recorder;
	}

	qrwlOutputs.lockForRead();

	bool prioritySpeakerActive = false;

	// Get the users that are currently talking (and are thus serving as an audio source)
	QMultiHash< const ClientUser *, AudioOutputUser * >::const_iterator it = qmOutputs.constBegin();
	while (it != qmOutputs.constEnd()) {
		AudioOutputUser *aop = it.value();
		if (!aop->prepareSampleBuffer(frameCount)) {
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

	if (!qlMix.isEmpty()) {
		// There are audio sources available -> mix those sources together and feed them into the audio backend
		STACKVAR(float, speaker, iChannels * 3);
		STACKVAR(float, svol, iChannels);

		STACKVAR(float, fOutput, iChannels *frameCount);

		// If the audio backend uses a float-array we can sample and mix the audio sources directly into the output.
		// Otherwise we'll have to use an intermediate buffer which we will convert to an array of shorts later
		float *output      = (eSampleFormat == SampleFloat) ? reinterpret_cast< float * >(outbuff) : fOutput;
		bool validListener = false;

		memset(output, 0, sizeof(float) * frameCount * iChannels);

		// Initialize recorder if recording is enabled
		boost::shared_array< float > recbuff;
		if (recorder) {
			recbuff = boost::shared_array< float >(new float[frameCount]);
			memset(recbuff.get(), 0, sizeof(float) * frameCount);
			recorder->prepareBufferAdds();
		}

		for (unsigned int i = 0; i < iChannels; ++i)
			svol[i] = mul * fSpeakerVolume[i];

		if (g.s.bPositionalAudio && (iChannels > 1) && g.p->fetch()
			&& (g.bPosTest || g.p->fCameraPosition[0] != 0 || g.p->fCameraPosition[1] != 0
				|| g.p->fCameraPosition[2] != 0)) {
			// Calculate the positional audio effects if it is enabled

			float front[3] = { g.p->fCameraFront[0], g.p->fCameraFront[1], g.p->fCameraFront[2] };
			float top[3]   = { g.p->fCameraTop[0], g.p->fCameraTop[1], g.p->fCameraTop[2] };

			// Front vector is dominant; if it's zero we presume all is zero.

			float flen = sqrtf(front[0] * front[0] + front[1] * front[1] + front[2] * front[2]);

			if (flen > 0.0f) {
				front[0] *= (1.0f / flen);
				front[1] *= (1.0f / flen);
				front[2] *= (1.0f / flen);

				float tlen = sqrtf(top[0] * top[0] + top[1] * top[1] + top[2] * top[2]);

				if (tlen > 0.0f) {
					top[0] *= (1.0f / tlen);
					top[1] *= (1.0f / tlen);
					top[2] *= (1.0f / tlen);
				} else {
					top[0] = 0.0f;
					top[1] = 1.0f;
					top[2] = 0.0f;
				}

				const float dotproduct = front[0] * top[0] + front[1] * top[1] + front[2] * top[2];
				const float error      = std::abs(dotproduct);
				if (error > 0.5f) {
					// Not perpendicular by a large margin. Assume Y up and rotate 90 degrees.

					float azimuth = 0.0f;
					if ((front[0] != 0.0f) || (front[2] != 0.0f))
						azimuth = atan2f(front[2], front[0]);
					float inclination = acosf(front[1]) - static_cast< float >(M_PI) / 2.0f;

					top[0] = sinf(inclination) * cosf(azimuth);
					top[1] = cosf(inclination);
					top[2] = sinf(inclination) * sinf(azimuth);
				} else if (error > 0.01f) {
					// Not perpendicular by a small margin. Find the nearest perpendicular vector.

					top[0] -= front[0] * dotproduct;
					top[1] -= front[1] * dotproduct;
					top[2] -= front[2] * dotproduct;

					// normalize top again
					tlen = sqrtf(top[0] * top[0] + top[1] * top[1] + top[2] * top[2]);
					// tlen is guaranteed to be non-zero, otherwise error would have been larger than 0.5
					top[0] *= (1.0f / tlen);
					top[1] *= (1.0f / tlen);
					top[2] *= (1.0f / tlen);
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
			float right[3] = { top[1] * front[2] - top[2] * front[1], top[2] * front[0] - top[0] * front[2],
							   top[0] * front[1] - top[1] * front[0] };

			/*
						qWarning("Front: %f %f %f", front[0], front[1], front[2]);
						qWarning("Top: %f %f %f", top[0], top[1], top[2]);
						qWarning("Right: %f %f %f", right[0], right[1], right[2]);
			*/
			// Rotate speakers to match orientation
			for (unsigned int i = 0; i < iChannels; ++i) {
				speaker[3 * i + 0] =
					fSpeakers[3 * i + 0] * right[0] + fSpeakers[3 * i + 1] * top[0] + fSpeakers[3 * i + 2] * front[0];
				speaker[3 * i + 1] =
					fSpeakers[3 * i + 0] * right[1] + fSpeakers[3 * i + 1] * top[1] + fSpeakers[3 * i + 2] * front[1];
				speaker[3 * i + 2] =
					fSpeakers[3 * i + 0] * right[2] + fSpeakers[3 * i + 1] * top[2] + fSpeakers[3 * i + 2] * front[2];
			}
			validListener = true;
		}

		foreach (AudioOutputUser *aop, qlMix) {
			// Iterate through all audio sources and mix them together into the output (or the intermediate array)
			const float *RESTRICT pfBuffer = aop->pfBuffer;
			float volumeAdjustment         = 1;

			// Check if the audio source is a user speaking (instead of a sample playback) and apply potential volume
			// adjustments
			AudioOutputSpeech *speech = qobject_cast< AudioOutputSpeech * >(aop);
			if (speech) {
				const ClientUser *user = speech->p;
				volumeAdjustment *= user->getLocalVolumeAdjustments();

				if (user->cChannel && ChannelListener::isListening(g.uiSession, user->cChannel->iId)
					&& (speech->ucFlags & SpeechFlags::Listen)) {
					// We are receiving this audio packet only because we are listening to the channel
					// the speaking user is in. Thus we receive the audio via our "listener proxy".
					// Thus we'll apply the volume adjustment for our listener proxy as well
					volumeAdjustment *= ChannelListener::getListenerLocalVolumeAdjustment(user->cChannel);
				}

				if (prioritySpeakerActive) {
					if (user->tsState != Settings::Whispering && !user->bPrioritySpeaker) {
						volumeAdjustment *= adjustFactor;
					}
				}
			}

			// If recording is enabled add the current audio source to the recording buffer
			if (recorder) {
				if (speech) {
					if (speech->bStereo) {
						// Mix down stereo to mono. TODO: stereo record support
						// frame: for a stereo stream, the [LR] pair inside ...[LR]LRLRLR.... is a frame
						for (unsigned int i = 0; i < frameCount; ++i) {
							recbuff[i] += (pfBuffer[2 * i] / 2.0 + pfBuffer[2 * i + 1] / 2.0) * volumeAdjustment;
						}
					} else {
						for (unsigned int i = 0; i < frameCount; ++i) {
							recbuff[i] += pfBuffer[i] * volumeAdjustment;
						}
					}

					if (!recorder->isInMixDownMode()) {
						recorder->addBuffer(speech->p, recbuff, frameCount);
						recbuff = boost::shared_array< float >(new float[frameCount]);
						memset(recbuff.get(), 0, sizeof(float) * frameCount);
					}

					// Don't add the local audio to the real output
					if (qobject_cast< RecordUser * >(speech->p)) {
						continue;
					}
				}
			}

			if (validListener && ((aop->fPos[0] != 0.0f) || (aop->fPos[1] != 0.0f) || (aop->fPos[2] != 0.0f))) {
				// Add position to position map
				AudioOutputSpeech *speech = qobject_cast< AudioOutputSpeech * >(aop);
#ifdef USE_MANUAL_PLUGIN
				if (speech) {
					const ClientUser *user = speech->p;
					// The coordinates in the plane are actually given by x and z instead of x and y (y is up)
					positions.insert(user->uiSession, { aop->fPos[0], aop->fPos[2] });
				}
#endif

				// If positional audio is enabled, calculate the respective audio effect here
				float dir[3] = { aop->fPos[0] - g.p->fCameraPosition[0], aop->fPos[1] - g.p->fCameraPosition[1],
								 aop->fPos[2] - g.p->fCameraPosition[2] };
				float len    = sqrtf(dir[0] * dir[0] + dir[1] * dir[1] + dir[2] * dir[2]);
				if (len > 0.0f) {
					dir[0] /= len;
					dir[1] /= len;
					dir[2] /= len;
				}
				/*
								qWarning("Voice pos: %f %f %f", aop->fPos[0], aop->fPos[1], aop->fPos[2]);
								qWarning("Voice dir: %f %f %f", dir[0], dir[1], dir[2]);
				*/
				if (!aop->pfVolume) {
					aop->pfVolume = new float[nchan];
					for (unsigned int s = 0; s < nchan; ++s)
						aop->pfVolume[s] = -1.0;
				}
				for (unsigned int s = 0; s < nchan; ++s) {
					const float dot = bSpeakerPositional[s] ? dir[0] * speaker[s * 3 + 0] + dir[1] * speaker[s * 3 + 1]
																  + dir[2] * speaker[s * 3 + 2]
															: 1.0f;
					const float str   = svol[s] * calcGain(dot, len) * volumeAdjustment;
					float *RESTRICT o = output + s;
					const float old   = (aop->pfVolume[s] >= 0.0f) ? aop->pfVolume[s] : str;
					const float inc   = (str - old) / static_cast< float >(frameCount);
					aop->pfVolume[s]  = str;
					/*
										qWarning("%d: Pos %f %f %f : Dot %f Len %f Str %f", s, speaker[s*3+0],
					   speaker[s*3+1], speaker[s*3+2], dot, len, str);
					*/
					if ((old >= 0.00000001f) || (str >= 0.00000001f)) {
						for (unsigned int i = 0; i < frameCount; ++i) {
							if (speech && speech->bStereo) {
								// Mix stereo user's stream into mono
								// frame: for a stereo stream, the [LR] pair inside ...[LR]LRLRLR.... is a frame
								o[i * nchan] += (pfBuffer[2 * i] / 2.0 + pfBuffer[2 * i + 1] / 2.0)
												* (old + inc * static_cast< float >(i));
							} else {
								o[i * nchan] += pfBuffer[i] * (old + inc * static_cast< float >(i));
							}
						}
					}
				}
			} else {
				// Mix the current audio source into the output by adding it to the elements of the output buffer after
				// having applied a volume adjustment
				for (unsigned int s = 0; s < nchan; ++s) {
					const float str   = svol[s] * volumeAdjustment;
					float *RESTRICT o = output + s;
					if (aop->bStereo) {
						// Linear-panning stereo stream according to the projection of fSpeaker vector on left-right
						// direction.
						// frame: for a stereo stream, the [LR] pair inside ...[LR]LRLRLR.... is a frame
						for (unsigned int i = 0; i < frameCount; ++i)
							o[i * nchan] += (pfBuffer[2 * i] * fStereoPanningFactor[2 * s + 0]
											 + pfBuffer[2 * i + 1] * fStereoPanningFactor[2 * s + 1])
											* str;
					} else {
						for (unsigned int i = 0; i < frameCount; ++i)
							o[i * nchan] += pfBuffer[i] * str;
					}
				}
			}
		}

		if (recorder && recorder->isInMixDownMode()) {
			recorder->addBuffer(nullptr, recbuff, frameCount);
		}

		// Clip the output audio
		if (eSampleFormat == SampleFloat)
			for (unsigned int i = 0; i < frameCount * iChannels; i++)
				output[i] = qBound(-1.0f, output[i], 1.0f);
		else
			// Also convert the intermediate float array into an array of shorts before writing it to the outbuff
			for (unsigned int i = 0; i < frameCount * iChannels; i++)
				reinterpret_cast< short * >(outbuff)[i] =
					static_cast< short >(qBound(-32768.f, (output[i] * 32768.f), 32767.f));
	}

	qrwlOutputs.unlock();

	// Delete all AudioOutputUsers that no longer provide any new audio
	foreach (AudioOutputUser *aop, qlDel)
		removeBuffer(aop);

#ifdef USE_MANUAL_PLUGIN
	Manual::setSpeakerPositions(positions);
#endif

	// Return whether data has been written to the outbuff
	return (!qlMix.isEmpty());
}

bool AudioOutput::isAlive() const {
	return isRunning();
}

unsigned int AudioOutput::getMixerFreq() const {
	return iMixerFreq;
}

void AudioOutput::setBufferSize(unsigned int bufferSize) {
	iBufferSize = bufferSize;
}
