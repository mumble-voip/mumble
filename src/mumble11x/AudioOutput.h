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

#ifndef _AUDIOOUTPUT_H
#define _AUDIOOUTPUT_H

// AudioOutput depends on Player being valid. This means it's important
// to removeBuffer from here BEFORE MainWindow gets any PlayerLeft
// messages. Any decendant player should feel free to remove unused
// AudioOutputPlayer objects; it's better to recreate them than
// having them use resources while unused.

#ifndef SPEAKER_FRONT_LEFT
#define SPEAKER_FRONT_LEFT              0x1
#define SPEAKER_FRONT_RIGHT             0x2
#define SPEAKER_FRONT_CENTER            0x4
#define SPEAKER_LOW_FREQUENCY           0x8
#define SPEAKER_BACK_LEFT               0x10
#define SPEAKER_BACK_RIGHT              0x20
#define SPEAKER_FRONT_LEFT_OF_CENTER    0x40
#define SPEAKER_FRONT_RIGHT_OF_CENTER   0x80
#define SPEAKER_BACK_CENTER             0x100
#define SPEAKER_SIDE_LEFT               0x200
#define SPEAKER_SIDE_RIGHT              0x400
#define SPEAKER_TOP_CENTER              0x800
#define SPEAKER_TOP_FRONT_LEFT          0x1000
#define SPEAKER_TOP_FRONT_CENTER        0x2000
#define SPEAKER_TOP_FRONT_RIGHT         0x4000
#define SPEAKER_TOP_BACK_LEFT           0x8000
#define SPEAKER_TOP_BACK_CENTER         0x10000
#define SPEAKER_TOP_BACK_RIGHT          0x20000
#endif

#include "Audio.h"
#include "Settings.h"
#include "smallft.h"

class AudioOutput;
class ClientPlayer;

typedef boost::shared_ptr<AudioOutput> AudioOutputPtr;

class AudioOutputRegistrar {
	private:
		Q_DISABLE_COPY(AudioOutputRegistrar)
	public:
		static QMap<QString, AudioOutputRegistrar *> *qmNew;
		static QString current;
		static AudioOutputPtr newFromChoice(QString choice = QString());

		const QString name;
		int priority;

		AudioOutputRegistrar(const QString &n, int priority = 0);
		virtual ~AudioOutputRegistrar();
		virtual AudioOutput *create() = 0;
		virtual const QList<audioDevice> getDeviceChoices() = 0;
		virtual void setDeviceChoice(const QVariant &, Settings &) = 0;
		virtual bool canMuteOthers() const;
		virtual bool usesOutputDelay() const;
};

class AudioOutputPlayer : public QObject {
		friend class AudioOutput;
	private:
		Q_OBJECT
		Q_DISABLE_COPY(AudioOutputPlayer)
	protected:
		unsigned int iBufferSize;
		void resizeBuffer(unsigned int newsize);
	public:
		AudioOutputPlayer(const QString name);
		~AudioOutputPlayer();
		const QString qsName;
		float *pfBuffer;
		float *pfVolume;
		float fPos[3];
		virtual bool needSamples(unsigned int snum) = 0;
};

class AudioOutputSpeech : public AudioOutputPlayer {
		friend class AudioOutput;
	private:
		Q_OBJECT
		Q_DISABLE_COPY(AudioOutputSpeech)
	protected:
		unsigned int iBufferOffset;
		unsigned int iBufferFilled;
		unsigned int iOutputSize;
		unsigned int iLastConsume;
		unsigned int iFrameSize;
		bool bLastAlive;

		SpeexResamplerState *srs;

		SpeexBits sbBits;
		QMutex qmJitter;
		JitterBuffer *jbJitter;
		void *dsDecState;
		int iMissCount;

		unsigned char ucFlags;

		static int speexCallback(SpeexBits *bits, void *state, void *data);
	public:
		int iMissedFrames;
		ClientPlayer *p;

		virtual bool needSamples(unsigned int snum);

		void addFrameToBuffer(const QByteArray &, unsigned int iBaseSeq);
		AudioOutputSpeech(ClientPlayer *, unsigned int freq);
		~AudioOutputSpeech();
};

class AudioOutputSample : public AudioOutputPlayer {
		friend class AudioOutput;
	private:
		Q_OBJECT
		Q_DISABLE_COPY(AudioOutputSample)
	protected:
		unsigned int iBufferOffset;
		unsigned int iBufferFilled;
		unsigned int iOutputSize;
		unsigned int iLastConsume;
		unsigned int iFrameSize;
		int iPacketIndex;
		QList<QByteArray> qlPackets;
		SpeexBits sbBits;
		void *dsDecState;
		SpeexResamplerState *srs;
		bool bLastAlive;
		bool bLoop;
	public:
		static QList<QByteArray> getPacketsFromFile(const QString &filename);
		virtual bool needSamples(unsigned int snum);
		AudioOutputSample(const QString &filename, const QList<QByteArray> &packets, bool repeat, unsigned int freq);
		~AudioOutputSample();
};

class AudioSine : public AudioOutputPlayer {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(AudioSine)
	protected:
		float v;
		float inc;
		float dinc;
		float volume;
		unsigned int frames;
		unsigned int cntr;
		unsigned int tbin;
		bool bSearch;
	public:
		virtual bool needSamples(unsigned int snum);
		AudioSine(float hz, float i, unsigned int frm, float v, unsigned int freq);
		~AudioSine();
};

class AudioOutput : public QThread {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(AudioOutput)
	private:
		float *fSpeakers;
		float *fSpeakerVolume;
		bool *bSpeakerPositional;
	protected:
		enum { SampleShort, SampleFloat } eSampleFormat;
		volatile bool bRunning;
		unsigned int iFrameSize;
		volatile unsigned int iMixerFreq;
		unsigned int iChannels;
		unsigned int iSampleSize;
		QReadWriteLock qrwlOutputs;
		QMultiHash<const ClientPlayer *, AudioOutputPlayer *> qmOutputs;

		virtual void removeBuffer(AudioOutputPlayer *);
		void initializeMixer(const unsigned int *chanmasks, bool forceheadphone = false);
		bool mix(void *output, unsigned int nsamp);
	public:
		void wipe();

		AudioOutput();
		~AudioOutput();
		void addFrameToBuffer(ClientPlayer *, const QByteArray &, unsigned int iSeq);
		void removeBuffer(const ClientPlayer *);
		AudioSine *playSine(float hz, float i = 0.0, unsigned int frames = 0xffffff, float volume = 0.3f);
		AudioOutputSample *playSample(const QString &filename, bool loop);
		void run() = 0;
		virtual bool isAlive() const;
		const float *getSpeakerPos(unsigned int &nspeakers);
		static float calcGain(float dotproduct, float distance);
};

#else
class AudioInput;
#endif
