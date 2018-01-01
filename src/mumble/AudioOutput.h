// Copyright 2005-2018 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

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

                /// Construct an AudioOutput.
                ///
                /// This constructor is only ever called by Audio::startOutput(), and is guaranteed
                /// to be called on the application's main thread.
		AudioOutput();

                /// Destroy an AudioOutput.
                ///
                /// This destructor is only ever called by Audio::stopOutput() and Audio::stop(),
                /// and is guaranteed to be called on the application's main thread.
		~AudioOutput() Q_DECL_OVERRIDE;

		void addFrameToBuffer(ClientUser *, const QByteArray &, unsigned int iSeq, MessageHandler::UDPMessageType type);
		void removeBuffer(const ClientUser *);
		AudioOutputSample *playSample(const QString &filename, bool loop = false);
		void run() Q_DECL_OVERRIDE = 0;
		virtual bool isAlive() const;
		const float *getSpeakerPos(unsigned int &nspeakers);
		static float calcGain(float dotproduct, float distance);
		unsigned int getMixerFreq() const;
};

#endif
