/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>
   Copyright (C) 2009, Stefan Hacker <dd0t@users.sourceforge.net>

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

// AudioOutput depends on User being valid. This means it's important
// to removeBuffer from here BEFORE MainWindow gets any UserLeft
// messages. Any decendant user should feel free to remove unused
// AudioOutputUser objects; it's better to recreate them than
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
#include "Message.h"
#include "smallft.h"

class AudioOutput;
class ClientUser;

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
		virtual bool canExclusive() const;
};

class AudioOutputUser : public QObject {
		friend class AudioOutput;
	private:
		Q_OBJECT
		Q_DISABLE_COPY(AudioOutputUser)
	protected:
		unsigned int iBufferSize;
		void resizeBuffer(unsigned int newsize);
	public:
		AudioOutputUser(const QString name);
		~AudioOutputUser();
		const QString qsName;
		float *pfBuffer;
		float *pfVolume;
		float fPos[3];
		virtual bool needSamples(unsigned int snum) = 0;
};

class AudioOutputSpeech : public AudioOutputUser {
		friend class AudioOutput;
	private:
		Q_OBJECT
		Q_DISABLE_COPY(AudioOutputSpeech)
	protected:
		MessageHandler::UDPMessageType umtType;
		unsigned int iBufferOffset;
		unsigned int iBufferFilled;
		unsigned int iOutputSize;
		unsigned int iLastConsume;
		unsigned int iFrameSize;
		bool bLastAlive;
		bool bHasTerminator;

		float *fFadeIn;
		float *fFadeOut;

		SpeexResamplerState *srs;

		QMutex qmJitter;
		JitterBuffer *jbJitter;
		int iMissCount;

		CELTCodec *cCodec;
		CELTDecoder *cdDecoder;

		SpeexBits sbBits;
		void *dsSpeex;

		QList<QByteArray> qlFrames;

		unsigned char ucFlags;
	public:
		int iMissedFrames;
		ClientUser *p;

		virtual bool needSamples(unsigned int snum);

		void addFrameToBuffer(const QByteArray &, unsigned int iBaseSeq);
		AudioOutputSpeech(ClientUser *, unsigned int freq, MessageHandler::UDPMessageType type);
		~AudioOutputSpeech();
};

class SoundFile : public QObject {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(SoundFile)
	protected:
		SNDFILE *sfFile;
		SF_INFO siInfo;
		QFile qfFile;
		static sf_count_t vio_get_filelen(void *user_data);
		static sf_count_t vio_seek(sf_count_t offset, int whence, void *user_data);
		static sf_count_t vio_read(void *ptr, sf_count_t count, void *user_data);
		static sf_count_t vio_write(const void *ptr, sf_count_t count, void *user_data);
		static sf_count_t vio_tell(void *user_data);
	public:
		SoundFile(const QString &fname);
		virtual ~SoundFile();

		int channels() const;
		int samplerate() const;
		int error() const ;
		QString strError() const;
		bool isOpen() const;

		sf_count_t seek(sf_count_t frames, int whence);
		sf_count_t read(float *ptr, sf_count_t items);
};

class AudioOutputSample : public AudioOutputUser {
		friend class AudioOutput;
	private:
		Q_OBJECT
		Q_DISABLE_COPY(AudioOutputSample)
	protected:
		unsigned int iLastConsume;
		unsigned int iBufferFilled;
		unsigned int iOutSampleRate;
		SpeexResamplerState *srs;

		SoundFile *sfHandle;

		bool bLastAlive;
		bool bLoop;
		bool bEof;
	signals:
		void playbackFinished();
	public:
		static SoundFile* loadSndfile(const QString &filename);
		static QString browseForSndfile();
		virtual bool needSamples(unsigned int snum);
		AudioOutputSample(const QString &name, SoundFile *psndfile, bool repeat, unsigned int freq);
		~AudioOutputSample();
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
		QMultiHash<const ClientUser *, AudioOutputUser *> qmOutputs;

		virtual void removeBuffer(AudioOutputUser *);
		void initializeMixer(const unsigned int *chanmasks, bool forceheadphone = false);
		bool mix(void *output, unsigned int nsamp);
	public:
		void wipe();

		AudioOutput();
		~AudioOutput();

		void addFrameToBuffer(ClientUser *, const QByteArray &, unsigned int iSeq, MessageHandler::UDPMessageType type);
		void removeBuffer(const ClientUser *);
		AudioOutputSample *playSample(const QString &filename, bool loop = false);
		void run() = 0;
		virtual bool isAlive() const;
		const float *getSpeakerPos(unsigned int &nspeakers);
		static float calcGain(float dotproduct, float distance);
		unsigned int getMixerFreq() const;
};

#else
class AudioInput;
#endif
