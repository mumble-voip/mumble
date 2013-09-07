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

#ifndef MUMBLE_MUMBLE_AUDIOOUTPUT_H_
#define MUMBLE_MUMBLE_AUDIOOUTPUT_H_

#include <boost/shared_ptr.hpp>
#include <QtCore/QObject>
#include <QtCore/QThread>

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
#include "Message.h"

class AudioOutput;
class ClientUser;
class AudioOutputUser;
class AudioOutputSample;

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

#endif
