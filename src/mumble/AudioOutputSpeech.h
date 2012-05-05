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

#ifndef AUDIOOUTPUTSPEECH_H_
#define AUDIOOUTPUTSPEECH_H_

#include <stdint.h>
#include <speex/speex.h>
#include <speex/speex_resampler.h>
#include <speex/speex_jitter.h>
#include <celt.h>

#include <QtCore/QMutex>

#include "AudioOutputUser.h"
#include "Message.h"

class CELTCodec;
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

		OpusDecoder *opusState;

		SpeexBits sbBits;
		void *dsSpeex;

		QList<QByteArray> qlFrames;

		unsigned char ucFlags;
	public:
		MessageHandler::UDPMessageType umtType;
		int iMissedFrames;
		ClientUser *p;

		virtual bool needSamples(unsigned int snum);

		void addFrameToBuffer(const QByteArray &, unsigned int iBaseSeq);
		AudioOutputSpeech(ClientUser *, unsigned int freq, MessageHandler::UDPMessageType type);
		~AudioOutputSpeech();
};

#endif  // AUDIOOUTPUTSPEECH_H_
