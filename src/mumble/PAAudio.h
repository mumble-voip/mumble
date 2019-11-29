// Copyright 2005-2020 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_PAAUDIO_H_
#define MUMBLE_MUMBLE_PAAUDIO_H_

#include "AudioInput.h"
#include "AudioOutput.h"

#include <QtCore/QLibrary>
#include <QtCore/QWaitCondition>

#include <portaudio.h>

class PortAudioInit;

class PortAudioSystem : public QObject {
		friend PortAudioInit;
	private:
		Q_OBJECT
		Q_DISABLE_COPY(PortAudioSystem)
	protected:
		bool bOk;
		QMutex qmWait;
		QLibrary qlPortAudio;
		QWaitCondition qwcWait;

		static int streamCallback(const void *input, void *output, unsigned long frames, const PaStreamCallbackTimeInfo *, PaStreamCallbackFlags statusFlags, void *isInput);

		const char *(*Pa_GetVersionText)();
		const char *(*Pa_GetErrorText)(PaError errorCode);
		PaError (*Pa_Initialize)();
		PaError (*Pa_Terminate)();
		PaError (*Pa_OpenStream)(PaStream **stream, const PaStreamParameters *inputParameters, const PaStreamParameters *outputParameters, double sampleRate, unsigned long framesPerBuffer, PaStreamFlags streamFlags, PaStreamCallback *streamCallback, void *userData);
		PaError (*Pa_CloseStream)(PaStream *stream);
		PaError (*Pa_StartStream)(PaStream *stream);
		PaError (*Pa_StopStream)(PaStream *stream);
		PaError (*Pa_IsStreamActive)(PaStream *stream);
		PaDeviceIndex (*Pa_GetDefaultInputDevice)();
		PaDeviceIndex (*Pa_GetDefaultOutputDevice)();
		PaDeviceIndex (*Pa_HostApiDeviceIndexToDeviceIndex)(PaHostApiIndex hostApi, int hostApiDeviceIndex);
		PaHostApiIndex (*Pa_GetHostApiCount)();
		const PaHostApiInfo *(*Pa_GetHostApiInfo)(PaHostApiIndex hostApi);
		const PaDeviceInfo *(*Pa_GetDeviceInfo)(PaDeviceIndex device);
	public:
		const QList<audioDevice> enumerateDevices(const bool input, const PaDeviceIndex current);

		bool isStreamRunning(PaStream *stream);

		int openStream(PaStream **stream, PaDeviceIndex device, const uint32_t frameSize, const bool isInput);
		bool closeStream(PaStream *stream);

		bool startStream(PaStream *stream);
		bool stopStream(PaStream *stream);

		PortAudioSystem();
		~PortAudioSystem();
};


class PortAudioInput : public AudioInput {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(PortAudioInput)
	protected:
		QMutex qmWait;
		QWaitCondition qwcSleep;
		PaStream *stream;
	public:
		void process(const uint32_t frames, const void *buffer);
		void run() Q_DECL_OVERRIDE;
		PortAudioInput();
		~PortAudioInput() Q_DECL_OVERRIDE;
};


class PortAudioOutput : public AudioOutput {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(PortAudioOutput)
	protected:
		QMutex qmWait;
		QWaitCondition qwcSleep;
		PaStream *stream;
	public:
		void process(const uint32_t frames, void *buffer);
		void run() Q_DECL_OVERRIDE;
		PortAudioOutput();
		~PortAudioOutput() Q_DECL_OVERRIDE;
};

#endif
