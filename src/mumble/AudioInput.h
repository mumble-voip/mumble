/* Copyright (C) 2005-2011, Thorvald Natvig <thorvald@natvig.com>

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

#ifndef MUMBLE_MUMBLE_AUDIOINPUT_H_
#define MUMBLE_MUMBLE_AUDIOINPUT_H_

#include <boost/shared_ptr.hpp>
#include <boost/array.hpp>
#include <speex/speex.h>
#include <speex/speex_echo.h>
#include <speex/speex_preprocess.h>
#include <speex/speex_resampler.h>
#include <QtCore/QObject>
#include <QtCore/QThread>
#include <vector>

#include "Audio.h"
#include "Settings.h"
#include "Timer.h"
#include "Message.h"

class AudioInput;
class CELTCodec;
struct CELTEncoder;
struct OpusEncoder;
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
		virtual bool canExclusive() const;
};

class AudioInput : public QThread {
		friend class AudioNoiseWidget;
		friend class AudioEchoWidget;
		friend class AudioStats;
		friend class AudioInputDialog;
	private:
		Q_OBJECT
		Q_DISABLE_COPY(AudioInput)
	protected:
		typedef enum { CodecCELT, CodecSpeex } CodecFormat;
		typedef enum { SampleShort, SampleFloat } SampleFormat;
		typedef void (*inMixerFunc)(float * RESTRICT, const void * RESTRICT, unsigned int, unsigned int);
	private:
		SpeexResamplerState *srsMic, *srsEcho;

		QMutex qmEcho;
		QList<short *> qlEchoFrames;
		unsigned int iJitterSeq;
		int iMinBuffered;

		unsigned int iMicFilled, iEchoFilled;
		inMixerFunc imfMic, imfEcho;
		inMixerFunc chooseMixer(const unsigned int nchan, SampleFormat sf);
		void resetAudioProcessor();

		OpusEncoder *opusState;
		bool selectCodec();
		
		typedef boost::array<unsigned char, 960> EncodingOutputBuffer;
		
		int encodeOpusFrame(short *source, int size, EncodingOutputBuffer& buffer);
		int encodeCELTFrame(short *pSource, EncodingOutputBuffer& buffer);
	protected:
		MessageHandler::UDPMessageType umtType;
		SampleFormat eMicFormat, eEchoFormat;

		unsigned int iSampleRate;
		unsigned int iMicChannels, iEchoChannels;
		unsigned int iMicFreq, iEchoFreq;
		unsigned int iMicLength, iEchoLength;
		unsigned int iMicSampleSize, iEchoSampleSize;
		unsigned int iEchoMCLength, iEchoFrameSize;

		bool bEchoMulti;
		int	iFrameSize;

		QMutex qmSpeex;
		SpeexPreprocessState *sppPreprocess;
		SpeexEchoState *sesEcho;

		CELTCodec *cCodec;
		CELTEncoder *ceEncoder;

		/// Encoded audio rate in bit/s
		int iAudioQuality;
		/// Number of 10ms audio "frames" per packet (!= frames in packet)
		int iAudioFrames;

		short *psMic;
		short *psSpeaker;
		short *psClean;

		float *pfMicInput;
		float *pfEchoInput;
		float *pfOutput;

		std::vector<short> opusBuffer;

		void encodeAudioFrame();
		void addMic(const void *data, unsigned int nsamp);
		void addEcho(const void *data, unsigned int nsamp);

		volatile bool bRunning;
		volatile bool bPreviousVoice;

		int iFrameCounter;
		int iSilentFrames;
		int iHoldFrames;
		int iBufferedFrames;

		QList<QByteArray> qlFrames;
		void flushCheck(const QByteArray &, bool terminator);

		void initializeMixer();

		static void adjustBandwidth(int bitspersec, int &bitrate, int &frames);
	signals:
		void doDeaf();
		void doMute();
	public:
		bool bResetProcessor;

		Timer tIdle;

		int iBitrate;
		float dPeakSpeaker, dPeakSignal, dMaxMic, dPeakMic, dPeakCleanMic;
		float fSpeechProb;

		static int getNetworkBandwidth(int bitrate, int frames);
		static void setMaxBandwidth(int bitspersec);

		AudioInput();
		~AudioInput();
		void run() = 0;
		virtual bool isAlive() const;
		bool isTransmitting() const;
};

#endif
