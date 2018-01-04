// Copyright 2005-2018 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_COREAUDIO_H_
#define MUMBLE_MUMBLE_COREAUDIO_H_

#include "AudioInput.h"
#include "AudioOutput.h"

#include <AudioToolbox/AudioToolbox.h>

class CoreAudioSystem : public QObject {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(CoreAudioSystem)
	public:
		static CFStringRef QStringToCFString(const QString &str);
		static const QHash<QString, QString> getDevices(bool input);
		static const QList<audioDevice> getDeviceChoices(bool input);
};

class CoreAudioInput : public AudioInput {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(CoreAudioInput)
	protected:
		AudioUnit au;
		AUEventListenerRef el;
		AudioBufferList buflist;
		static void propertyChange(void *udata, AudioUnit au, AudioUnitPropertyID prop, AudioUnitScope scope, AudioUnitElement element);
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
		AudioUnit au;
		AUEventListenerRef el;
		static void propertyChange(void *udata, AudioUnit au, AudioUnitPropertyID prop, AudioUnitScope scope, AudioUnitElement element);
		static OSStatus outputCallback(void *udata, AudioUnitRenderActionFlags *flags, const AudioTimeStamp *ts,
		                               UInt32 busnum, UInt32 npackets, AudioBufferList *buflist);
	public:
		CoreAudioOutput();
		~CoreAudioOutput() Q_DECL_OVERRIDE;
		void run() Q_DECL_OVERRIDE;
};

#else
class CoreAudioSystem;
class CoreAudioInput;
class CoreAudioOutput;
#endif
