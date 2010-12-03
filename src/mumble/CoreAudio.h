/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>
   Copyright (C) 2009, Mikkel Krautz <mikkel@krautz.dk>

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

#ifndef _COREAUDIO_H
#define _COREAUDIO_H

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
		~CoreAudioInput();
		void run();
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
		~CoreAudioOutput();
		void run();
};

#else
class CoreAudioSystem;
class CoreAudioInput;
class CoreAudioOutput;
#endif
