// Copyright 2007-2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_AUDIOOUTPUT_H_
#define MUMBLE_MUMBLE_AUDIOOUTPUT_H_

#include <QtCore/QObject>
#include <QtCore/QThread>
#include <boost/shared_ptr.hpp>

#ifdef USE_MANUAL_PLUGIN
#	include "ManualPlugin.h"
#endif

// AudioOutput depends on User being valid. This means it's important
// to removeBuffer from here BEFORE MainWindow gets any UserLeft
// messages. Any decendant user should feel free to remove unused
// AudioOutputUser objects; it's better to recreate them than
// having them use resources while unused.

#ifndef SPEAKER_FRONT_LEFT
#	define SPEAKER_FRONT_LEFT 0x1
#	define SPEAKER_FRONT_RIGHT 0x2
#	define SPEAKER_FRONT_CENTER 0x4
#	define SPEAKER_LOW_FREQUENCY 0x8
#	define SPEAKER_BACK_LEFT 0x10
#	define SPEAKER_BACK_RIGHT 0x20
#	define SPEAKER_FRONT_LEFT_OF_CENTER 0x40
#	define SPEAKER_FRONT_RIGHT_OF_CENTER 0x80
#	define SPEAKER_BACK_CENTER 0x100
#	define SPEAKER_SIDE_LEFT 0x200
#	define SPEAKER_SIDE_RIGHT 0x400
#	define SPEAKER_TOP_CENTER 0x800
#	define SPEAKER_TOP_FRONT_LEFT 0x1000
#	define SPEAKER_TOP_FRONT_CENTER 0x2000
#	define SPEAKER_TOP_FRONT_RIGHT 0x4000
#	define SPEAKER_TOP_BACK_LEFT 0x8000
#	define SPEAKER_TOP_BACK_CENTER 0x10000
#	define SPEAKER_TOP_BACK_RIGHT 0x20000
#endif

#include "Audio.h"
#include "Message.h"

class AudioOutput;
class ClientUser;
class AudioOutputUser;
class AudioOutputSample;

typedef boost::shared_ptr< AudioOutput > AudioOutputPtr;

class AudioOutputRegistrar {
private:
	Q_DISABLE_COPY(AudioOutputRegistrar)
public:
	static QMap< QString, AudioOutputRegistrar * > *qmNew;
	static QString current;
	static AudioOutputPtr newFromChoice(QString choice = QString());

	const QString name;
	int priority;

	AudioOutputRegistrar(const QString &n, int priority = 0);
	virtual ~AudioOutputRegistrar();
	virtual AudioOutput *create()                              = 0;
	virtual const QVariant getDeviceChoice()                   = 0;
	virtual const QList< audioDevice > getDeviceChoices()      = 0;
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
	/// Speaker positional vector
	float *fSpeakers         = nullptr;
	float *fSpeakerVolume    = nullptr;
	bool *bSpeakerPositional = nullptr;
	/// Used when panning stereo stream w.r.t. each speaker.
	float *fStereoPanningFactor = nullptr;

protected:
	enum { SampleShort, SampleFloat } eSampleFormat = SampleFloat;
	volatile bool bRunning                          = true;
	unsigned int iFrameSize                         = SAMPLE_RATE / 100;
	volatile unsigned int iMixerFreq                = 0;
	unsigned int iChannels                          = 0;
	unsigned int iSampleSize                        = 0;
	unsigned int iBufferSize                        = 0;
	QReadWriteLock qrwlOutputs;
	QMultiHash< const ClientUser *, AudioOutputUser * > qmOutputs;

#ifdef USE_MANUAL_PLUGIN
	QHash< unsigned int, Position2D > positions;
#endif

	virtual void removeBuffer(AudioOutputUser *);
	void initializeMixer(const unsigned int *chanmasks, bool forceheadphone = false);
	bool mix(void *output, unsigned int frameCount);

public:
	void wipe();

	/// Construct an AudioOutput.
	///
	/// This constructor is only ever called by Audio::startOutput(), and is guaranteed
	/// to be called on the application's main thread.
	AudioOutput(){};

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
	void setBufferSize(unsigned int bufferSize);

signals:
	/// Signal emitted whenever an audio source has been fetched
	///
	/// @param outputPCM The fetched output PCM
	/// @param sampleCount The amount of samples in the output
	/// @param channelCount The amount of channels in the output
	/// @param sampleRate The used sample rate in Hz
	/// @param isSpeech Whether the fetched output is considered to be speech
	/// @param A pointer to the user that this speech belongs to or nullptr if this isn't speech
	void audioSourceFetched(float *outputPCM, unsigned int sampleCount, unsigned int channelCount,
							unsigned int sampleRate, bool isSpeech, const ClientUser *user);
	/// Signal emitted whenever an audio is about to be played to the user
	///
	/// @param outputPCM The output PCM that is to be played
	/// @param sampleCount The amount of samples in the output
	/// @param channelCount The amount of channels in the output
	/// @param sampleRate The used sample rate in Hz
	/// @param modifiedAudio Pointer to bool if audio has been modified or not and should be played
	void audioOutputAboutToPlay(float *outputPCM, unsigned int sampleCount, unsigned int channelCount,
								unsigned int sampleRate, bool *modifiedAudio);
};

#endif
