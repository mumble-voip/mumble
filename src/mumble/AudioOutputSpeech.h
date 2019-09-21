// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_AUDIOOUTPUTSPEECH_H_
#define MUMBLE_MUMBLE_AUDIOOUTPUTSPEECH_H_

#include <stdint.h>
#include <speex/speex.h>
#include <speex/speex_resampler.h>
#include <speex/speex_jitter.h>
#include <celt.h>

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
		unsigned int iSampleRate;
		unsigned int iMixerFreq;
		bool bLastAlive;
		bool bHasTerminator;
		bool bStereo;

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

		QList<QByteArray> qlFrames;

		unsigned char ucFlags;
	public:
		MessageHandler::UDPMessageType umtType;
		int iMissedFrames;
		ClientUser *p;

		virtual bool prepareSampleBuffer(unsigned int snum) Q_DECL_OVERRIDE;

		void addFrameToBuffer(const QByteArray &, unsigned int iBaseSeq);
		AudioOutputSpeech(ClientUser *, unsigned int freq, MessageHandler::UDPMessageType type);
		~AudioOutputSpeech() Q_DECL_OVERRIDE;
};

#endif  // AUDIOOUTPUTSPEECH_H_
