// Copyright 2005-2017 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_AUDIOINPUT_H_
#define MUMBLE_MUMBLE_AUDIOINPUT_H_

#include <boost/array.hpp>
#include <speex/speex.h>
#include <speex/speex_echo.h>
#include <speex/speex_preprocess.h>
#include <speex/speex_resampler.h>
#include <QtCore/QObject>
#include <QtCore/QThread>
#include <QtCore/QSharedPointer>
#include <vector>

#include "Audio.h"
#include "Settings.h"
#include "Timer.h"
#include "Message.h"

class AudioInput;
class CELTCodec;
struct CELTEncoder;
struct OpusEncoder;
typedef QSharedPointer<AudioInput> AudioInputPtr;

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

		/// bResetEncoder is a flag that notifies
		/// our encoder functions that the encoder
		/// needs to be reset.
		bool bResetEncoder;

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
		~AudioInput() Q_DECL_OVERRIDE;
		void run() Q_DECL_OVERRIDE = 0;
		virtual bool isAlive() const;
		bool isTransmitting() const;
};

#endif
