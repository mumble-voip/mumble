/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>
   Copyright (C) 2007, Stefan Gehn <mETz AT gehn DOT net>

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

#ifndef _MUMBLE_PAAUDIO_H
#define _MUMBLE_PAAUDIO_H

#include "AudioInput.h"
#include "AudioOutput.h"

#include <portaudio.h>

class PortAudioSystem;
typedef boost::shared_ptr<PortAudioSystem> PortAudioSystemPtr;
typedef boost::weak_ptr<PortAudioSystem> WeakPortAudioSystemPtr;

/**
 * Small wrapper around some critical PortAudio functions.
 *
 * Basically this ensures that the PA lib is initialized and terminated properly and that
 * several threads can open/close streams as they like.
 */
class PortAudioSystem : public QObject {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(PortAudioSystem)
	protected:
		//! Mutex around PA stream creation/deletion
		static QMutex qmStream;
	public:
		static bool initStream(PaStream **stream, PaDeviceIndex devIndex, int frameSize, int *chans, bool isInput);
		static bool terminateStream(PaStream *stream);

		static bool startStream(PaStream *stream);
		static bool stopStream(PaStream *stream);

		static const QList<audioDevice> enumerateDevices(bool input, PaDeviceIndex match = -1);
};


class PortAudioInput : public AudioInput {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(PortAudioInput)
	public:
		PortAudioInput();
		~PortAudioInput();
		void run();
};


class PortAudioOutput : public AudioOutput {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(PortAudioOutput)
	public:
		PortAudioOutput();
		~PortAudioOutput();
		void run();
};

#else
class PortAudioSystem;
class PortAudioInput;
class PortAudioOutput;
#endif
