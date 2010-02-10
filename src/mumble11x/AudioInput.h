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

#ifndef _AUDIOINPUT_H
#define _AUDIOINPUT_H

#include "Audio.h"
#include "Settings.h"
#include "Timer.h"
#include "smallft.h"

class AudioInput;
typedef boost::shared_ptr<AudioInput> AudioInputPtr;

class AudioInputRegistrar {
	private:
		Q_DISABLE_COPY(AudioInputRegistrar)
	public:
		static QMap<QString, AudioInputRegistrar *> *qmNew;
		static QString current;
		static AudioInputPtr newFromChoice(QString choice = QString());

		const QString name;
		int priority;

		AudioInputRegistrar(const QString &n, int priority = 0);
		virtual ~AudioInputRegistrar();
		virtual AudioInput *create() = 0;
		virtual const QList<audioDevice> getDeviceChoices() = 0;
		virtual void setDeviceChoice(const QVariant &, Settings &) = 0;
		virtual bool canEcho(const QString &outputsys) const = 0;
};

class AudioInput : public QThread {
		friend class AudioNoiseWidget;
		friend class AudioEchoWidget;
		friend class AudioStats;
		friend class AudioInputDialog;
		friend class FMODSystem;
	private:
		Q_OBJECT
		Q_DISABLE_COPY(AudioInput)
	protected:
		typedef enum { SampleShort, SampleFloat } SampleFormat;
		typedef void (*inMixerFunc)(float * RESTRICT, const void * RESTRICT, unsigned int, unsigned int);
	private:
		SpeexResamplerState *srsMic, *srsEcho;
		JitterBuffer *jb;
		unsigned int iJitterSeq;
		unsigned int iMicFilled, iEchoFilled;
		inMixerFunc imfMic, imfEcho;
		inMixerFunc chooseMixer(const unsigned int nchan, SampleFormat sf);
	protected:
		SampleFormat eMicFormat, eEchoFormat;

		unsigned int iSampleRate;
		unsigned int iMicChannels, iEchoChannels;
		unsigned int iMicFreq, iEchoFreq;
		unsigned int iMicLength, iEchoLength;
		unsigned int iMicSampleSize, iEchoSampleSize;

		int	iFrameSize;

		QMutex qmSpeex;
		SpeexBits sbBits;
		SpeexPreprocessState *sppPreprocess;
		SpeexEchoState *sesEcho;

		void *esEncState;
		drft_lookup fftTable;

		short *psMic;
		short *psSpeaker;
		short *psClean;

		float *pfMicInput;
		float *pfEchoInput;
		float *pfOutput;

		void encodeAudioFrame();
		void addMic(const void *data, unsigned int nsamp);
		void addEcho(const void *data, unsigned int nsamp);

		volatile bool bRunning;
		bool bPreviousVoice;

		int iFrameCounter;
		int iSilentFrames;
		int iHoldFrames;

		int iFrames;

		void flushCheck();

		void initializeMixer();
	signals:
		void doMute();
	public:
		bool bResetProcessor;

		Timer tIdle;

		int iBitrate;
		float dPeakMic, dPeakSpeaker, dPeakSignal, dMaxMic;
		float fSpeechProb;

		unsigned int iBestBin;

		virtual bool isAlive() const;

		bool isTransmitting() {
			return bPreviousVoice;
		};

		static int getMaxBandwidth();
		void setMaxBandwidth(int bytespersec);

		AudioInput();
		~AudioInput();
		void run() = 0;
};

#else
class AudioInput;
#endif
