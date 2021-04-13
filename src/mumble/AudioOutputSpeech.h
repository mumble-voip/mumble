// Copyright 2011-2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_AUDIOOUTPUTSPEECH_H_
#define MUMBLE_MUMBLE_AUDIOOUTPUTSPEECH_H_

#include <celt.h>
#include <speex/speex.h>
#include <speex/speex_jitter.h>
#include <speex/speex_resampler.h>

#include <QtCore/QMutex>

#include "AudioOutputUser.h"
#include "Message.h"

class CELTCodec;
class OpusCodec;
class ClientUser;
struct OpusDecoder;

class AudioOutputSpeech : public AudioOutputUser {
private:
	Q_OBJECT
	Q_DISABLE_COPY(AudioOutputSpeech)
protected:
	unsigned int iAudioBufferSize;
	unsigned int iBufferOffset;
	unsigned int iBufferFilled;
	unsigned int iOutputSize;
	unsigned int iLastConsume;
	unsigned int iFrameSize;
	unsigned int iFrameSizePerChannel;
	unsigned int iSampleRate;
	unsigned int iMixerFreq;
	bool bLastAlive;
	bool bHasTerminator;

	float *fFadeIn;
	float *fFadeOut;
	float *fResamplerBuffer;

	SpeexResamplerState *srs;

	QMutex qmJitter;
	JitterBuffer *jbJitter;
	int iMissCount;

	CELTCodec *cCodec;
	CELTDecoder *cdDecoder;

	OpusCodec *oCodec;
	OpusDecoder *opusState;

	SpeexBits sbBits;
	void *dsSpeex;

	QList< QByteArray > qlFrames;

public:
	unsigned char ucFlags;
	MessageHandler::UDPMessageType umtType;
	int iMissedFrames;
	ClientUser *p;

	/// Fetch and decode frames from the jitter buffer. Called in mix().
	///
	/// @param frameCount Number of frames to decode. frame means a bundle of one sample from each channel.
	virtual bool prepareSampleBuffer(unsigned int frameCount) Q_DECL_OVERRIDE;

	void addFrameToBuffer(const QByteArray &, unsigned int iBaseSeq);

	/// @param systemMaxBufferSize maximum number of samples the system audio play back may request each time
	AudioOutputSpeech(ClientUser *, unsigned int freq, MessageHandler::UDPMessageType type,
					  unsigned int systemMaxBufferSize);
	~AudioOutputSpeech() Q_DECL_OVERRIDE;
};

#endif // AUDIOOUTPUTSPEECH_H_
