// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_COREAUDIO_H_
#	define MUMBLE_MUMBLE_COREAUDIO_H_

#	include "AudioInput.h"
#	include "AudioOutput.h"
#	include <AudioToolbox/AudioToolbox.h>

enum AUDirection { OUTPUT = 0, INPUT = 1 };

class CoreAudioSystem : public QObject {
private:
	Q_OBJECT
	Q_DISABLE_COPY(CoreAudioSystem)
public:
	static const QHash< QString, QString > getDevices(bool input, bool echo);
	static const QList< audioDevice > getDeviceChoices(bool input);
};

class CoreAudioInput : public AudioInput {
private:
	Q_OBJECT
	Q_DISABLE_COPY(CoreAudioInput)
	/// Open HAL AU as input and pass back the output stream description.
	bool openAUHAL(AudioStreamBasicDescription &streamDescription);

	/// Open VoiceProcessingIO AU as input, utilizing macOS's builtin echo cancellation,
	/// and pass back the output stream description.
	bool openAUVoip(AudioStreamBasicDescription &streamDescription);

	/// Initialize input AU with preferred parameters of Mumble
	bool initializeInputAU(AudioUnit au, AudioStreamBasicDescription &streamDescription, int &actualBufferLength);

protected:
	/// Hardware Abstraction Layer's AudioUnit, directly interacts with the hardware
	AudioUnit auHAL{};
	/// VoiceProcessingIO AU, provides audio input and echo cancellation
	AudioUnit auVoip{};
	AudioDeviceID inputDevId{};
	AudioDeviceID echoOutputDevId{};
	AudioBufferList buflist{};
	/// The AudioUnit (auHAL or auVoip) that CoreAudioInput::propertyChange was registered on;
	/// needed to remove that exact listener in stop().
	AudioUnit auPropertyListener{};
	/// Whether CoreAudioInput::deviceChange is currently registered as a system property listener.
	bool bDeviceListenerRegistered = false;
	/// UID configured for input when run() started; empty for "Default Device". Cached here
	/// because devicesChanged() runs off the Qt thread and can't read Settings.
	QString qsConfiguredInputDevice;
	/// Whether inputDevId is running on the default device because qsConfiguredInputDevice
	/// could not be found.
	bool bInputOnFallbackDevice = false;
	/// Same as qsConfiguredInputDevice/bInputOnFallbackDevice, but for echoOutputDevId.
	QString qsConfiguredEchoDevice;
	bool bEchoOnFallbackDevice = false;
	/// Whether CoreAudioInput::devicesChanged is currently registered as a listener.
	bool bDeviceListListenerRegistered = false;
	static void propertyChange(void *udata, AudioUnit au, AudioUnitPropertyID prop, AudioUnitScope scope,
							   AudioUnitElement element);
	static OSStatus deviceChange(AudioObjectID inObjectID, UInt32 inNumberAddresses,
								 const AudioObjectPropertyAddress inAddresses[], void *udata);
	/// Fires when a device is added to or removed from the system, so a configured device
	/// coming back (or disappearing) can be noticed even though it isn't the default device.
	static OSStatus devicesChanged(AudioObjectID inObjectID, UInt32 inNumberAddresses,
								   const AudioObjectPropertyAddress inAddresses[], void *udata);
	static OSStatus inputCallback(void *udata, AudioUnitRenderActionFlags *flags, const AudioTimeStamp *ts,
								  UInt32 busnum, UInt32 npackets, AudioBufferList *buflist);

public:
	CoreAudioInput();
	~CoreAudioInput() Q_DECL_OVERRIDE;
	void run() Q_DECL_OVERRIDE;
	void stop();
};

class CoreAudioOutput : public AudioOutput {
private:
	Q_OBJECT
	Q_DISABLE_COPY(CoreAudioOutput)
protected:
	/// Hardware Abstraction Layer's AudioOutputUnit, directly interacts with the hardware
	AudioUnit auHAL{};
	/// Whether CoreAudioOutput::propertyChange is currently registered as a listener on auHAL.
	bool bPropertyListenerRegistered = false;
	/// Whether CoreAudioOutput::deviceChange is currently registered as a system property listener.
	bool bDeviceListenerRegistered = false;
	/// UID configured for output when run() started; empty for "Default Device". Cached here
	/// because devicesChanged() runs off the Qt thread and can't read Settings.
	QString qsConfiguredOutputDevice;
	/// Whether devId is running on the default device because qsConfiguredOutputDevice could
	/// not be found.
	bool bOutputOnFallbackDevice = false;
	/// Whether CoreAudioOutput::devicesChanged is currently registered as a listener.
	bool bDeviceListListenerRegistered = false;
	static void propertyChange(void *udata, AudioUnit au, AudioUnitPropertyID prop, AudioUnitScope scope,
							   AudioUnitElement element);
	static OSStatus deviceChange(AudioObjectID inObjectID, UInt32 inNumberAddresses,
								 const AudioObjectPropertyAddress inAddresses[], void *udata);
	/// Fires when a device is added to or removed from the system, so a configured device
	/// coming back (or disappearing) can be noticed even though it isn't the default device.
	static OSStatus devicesChanged(AudioObjectID inObjectID, UInt32 inNumberAddresses,
								   const AudioObjectPropertyAddress inAddresses[], void *udata);
	static OSStatus outputCallback(void *udata, AudioUnitRenderActionFlags *flags, const AudioTimeStamp *ts,
								   UInt32 busnum, UInt32 npackets, AudioBufferList *buflist);

public:
	CoreAudioOutput();
	~CoreAudioOutput() Q_DECL_OVERRIDE;
	void run() Q_DECL_OVERRIDE;
	void stop();
};

class CoreAudioInputRegistrar : public AudioInputRegistrar {
public:
	CoreAudioInputRegistrar();
	virtual AudioInput *create();
	virtual const QVariant getDeviceChoice();
	virtual const QList< audioDevice > getDeviceChoices();
	virtual void setDeviceChoice(const QVariant &, Settings &);
	virtual bool canEcho(EchoCancelOptionID echoCancelID, const QString &outputSystem) const;
	virtual bool isMicrophoneAccessDeniedByOS();
};

class CoreAudioOutputRegistrar : public AudioOutputRegistrar {
public:
	CoreAudioOutputRegistrar() : AudioOutputRegistrar(QLatin1String("CoreAudio"), 10) {}
	virtual AudioOutput *create();
	virtual const QVariant getDeviceChoice();
	virtual const QList< audioDevice > getDeviceChoices();
	virtual void setDeviceChoice(const QVariant &, Settings &);
	bool canMuteOthers() const;
};

#else
class CoreAudioSystem;
class CoreAudioInput;
class CoreAudioOutput;
#endif
