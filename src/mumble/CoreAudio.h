// Copyright 2005-2020 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_COREAUDIO_H_
#	define MUMBLE_MUMBLE_COREAUDIO_H_

#	include <AudioToolbox/AudioToolbox.h>

#	include "AudioInput.h"
#	include "AudioOutput.h"

class CoreAudioSystem : public QObject {
private:
	Q_OBJECT
	Q_DISABLE_COPY(CoreAudioSystem)
public:
	static CFStringRef QStringToCFString(const QString &str);
	static const QHash< QString, QString > getDevices(bool input);
	static const QList< audioDevice > getDeviceChoices(bool input);
};

class CoreAudioInput : public AudioInput {
private:
	Q_OBJECT
	Q_DISABLE_COPY(CoreAudioInput)
	static bool getInputDeviceId(CFStringRef devUid, AudioDeviceID &devId);
	static bool getDefaultInputDeviceId(CFStringRef devUid, AudioDeviceID &devId);
	bool openAUHAL(AudioStreamBasicDescription &streamDescription);
	bool initializeAUHAL(AudioStreamBasicDescription &streamDescription, int &actualBufferLength);

protected:
	// Hardware Abstraction Layer's AudioOutputUnit, directly interacts with the hardware
	AudioUnit auHAL;
	AudioDeviceID devId;
	AUEventListenerRef el;
	AudioBufferList buflist;
	static void propertyChange(void *udata, AudioUnit au, AudioUnitPropertyID prop, AudioUnitScope scope,
							   AudioUnitElement element);
	static OSStatus inputCallback(void *udata, AudioUnitRenderActionFlags *flags, const AudioTimeStamp *ts,
								  UInt32 busnum, UInt32 npackets, AudioBufferList *buflist);

public:
	CoreAudioInput();
	~CoreAudioInput() Q_DECL_OVERRIDE;
	void run() Q_DECL_OVERRIDE;
};

class CoreAudioOutput : public AudioOutput {
private:
	Q_OBJECT
	Q_DISABLE_COPY(CoreAudioOutput)
protected:
	// Hardware Abstraction Layer's AudioOutputUnit, directly interacts with the hardware
	AudioUnit auHAL;
	AUEventListenerRef el;
	static void propertyChange(void *udata, AudioUnit au, AudioUnitPropertyID prop, AudioUnitScope scope,
							   AudioUnitElement element);
	static OSStatus outputCallback(void *udata, AudioUnitRenderActionFlags *flags, const AudioTimeStamp *ts,
								   UInt32 busnum, UInt32 npackets, AudioBufferList *buflist);

public:
	CoreAudioOutput();
	~CoreAudioOutput() Q_DECL_OVERRIDE;
	void run() Q_DECL_OVERRIDE;
};

class CoreAudioInputRegistrar : public AudioInputRegistrar, public QObject {
public:
	CoreAudioInputRegistrar() : AudioInputRegistrar(QLatin1String("CoreAudio"), 10) {}
	virtual AudioInput *create();
	virtual const QList< audioDevice > getDeviceChoices();
	virtual void setDeviceChoice(const QVariant &, Settings &);
	virtual bool canEcho(const QString &) const;
	virtual bool isMicrophoneAccessDeniedByOS();
};

class CoreAudioOutputRegistrar : public AudioOutputRegistrar {
public:
	CoreAudioOutputRegistrar() : AudioOutputRegistrar(QLatin1String("CoreAudio"), 10) {}
	virtual AudioOutput *create();
	virtual const QList< audioDevice > getDeviceChoices();
	virtual void setDeviceChoice(const QVariant &, Settings &);
	bool canMuteOthers() const;
};

#else
class CoreAudioSystem;
class CoreAudioInput;
class CoreAudioOutput;
#endif
