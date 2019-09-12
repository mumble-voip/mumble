// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

/* Copyright (C) 2005-2011, Thorvald Natvig <thorvald@natvig.com>
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

#include "PAAudio.h"
#include "Global.h"

QMutex                 PortAudioSystem::qmStream;

class PortAudioInputRegistrar : public AudioInputRegistrar {
	public:
		PortAudioInputRegistrar();
		virtual AudioInput *create();
		virtual const QList<audioDevice> getDeviceChoices();
		virtual void setDeviceChoice(const QVariant &, Settings &);
		virtual bool canEcho(const QString &) const;
};

class PortAudioOutputRegistrar : public AudioOutputRegistrar {
	public:
		PortAudioOutputRegistrar();
		virtual AudioOutput *create();
		virtual const QList<audioDevice> getDeviceChoices();
		virtual void setDeviceChoice(const QVariant &, Settings &);
};

class PAInit : public DeferInit {
	protected:
		bool bInit;
		PortAudioInputRegistrar *pairReg;
		PortAudioOutputRegistrar *paorReg;
	public:
		void initialize();
		void destroy();
};

static PAInit spaiInit;

void PAInit::initialize() {
	pairReg = NULL;
	paorReg = NULL;
	bInit = false;

	PaError err = Pa_Initialize();
	if (err != paNoError) {
		qWarning("PortAudio: Failed library initialization: %s",Pa_GetErrorText(err));
		return;
	}
	bInit = true;
	pairReg = new PortAudioInputRegistrar();
	paorReg = new PortAudioOutputRegistrar();
}

void PAInit::destroy() {
	delete pairReg;
	delete paorReg;
	if (bInit) {
		PaError err = Pa_Terminate();
		if (err != paNoError) {
			qWarning("PortAudio: Failed termination: %s", Pa_GetErrorText(err));
		}
	}
}

PortAudioInputRegistrar::PortAudioInputRegistrar() : AudioInputRegistrar(QLatin1String("PortAudio")) {
}

AudioInput *PortAudioInputRegistrar::create() {
	return new PortAudioInput();
}

const QList<audioDevice> PortAudioInputRegistrar::getDeviceChoices() {
	return PortAudioSystem::enumerateDevices(true, g.s.iPortAudioInput);
}

void PortAudioInputRegistrar::setDeviceChoice(const QVariant &choice, Settings &s) {
	s.iPortAudioInput = choice.toInt();
}

bool PortAudioInputRegistrar::canEcho(const QString &) const {
	return false;
}

PortAudioOutputRegistrar::PortAudioOutputRegistrar() : AudioOutputRegistrar(QLatin1String("PortAudio")) {
}

AudioOutput *PortAudioOutputRegistrar::create() {
	return new PortAudioOutput();
}

const QList<audioDevice> PortAudioOutputRegistrar::getDeviceChoices() {
	return PortAudioSystem::enumerateDevices(false, g.s.iPortAudioOutput);
}

void PortAudioOutputRegistrar::setDeviceChoice(const QVariant &choice, Settings &s) {
	s.iPortAudioOutput = choice.toInt();
}


bool PortAudioSystem::initStream(PaStream **stream, PaDeviceIndex devIndex, int frameSize, int *chans, bool isInput) {
	QMutexLocker lock(&qmStream);
	PaError            err;
	PaStreamParameters streamPar;
	int nchans = 1;

	if (!stream) // null pointer passed
		return false;

	if (*stream) // pointer valid but already initialized, exit gracefully
		return true;

	// -1 is default device, otherwise pass value straight to PA
	if (devIndex == -1)
		devIndex = (isInput ? Pa_GetDefaultInputDevice() : Pa_GetDefaultOutputDevice());

	const PaDeviceInfo *devInfo = Pa_GetDeviceInfo(devIndex);
	if (!devInfo) {
		qWarning("PortAudioSystem: Failed to find information about device %d", devIndex);
		return false;
	}

	const PaHostApiInfo *apiInfo = Pa_GetHostApiInfo(devInfo->hostApi);
	if (!apiInfo) {
		qWarning("PortAudioSystem: Failed to find information about API %d (dev %d)", devInfo->hostApi, devIndex);
		return false;
	}

	qWarning("PortAudioSystem: Will use device %d: %s %s",devIndex, apiInfo->name, devInfo->name);

	if (!isInput) {
		if (g.s.doPositionalAudio() && devInfo->maxOutputChannels > 1)
			nchans = 2;
	}

	if (chans)
		*chans = nchans;

	streamPar.device                    = devIndex;
	streamPar.channelCount              = nchans;
	streamPar.sampleFormat              = paInt16;
	//TODO: Can I determine this latency from mumble settings?
	streamPar.suggestedLatency          = (isInput ? devInfo->defaultLowInputLatency : devInfo->defaultLowOutputLatency);
	streamPar.hostApiSpecificStreamInfo = NULL;

	qWarning("suggestedLatency : %6d ms",int(streamPar.suggestedLatency * 1000));
	qWarning("channels:        : %6d", nchans);
	qWarning("frameSize        : %6d bytes", frameSize);
	qWarning("sample rate      : %6d samples/sec", SAMPLE_RATE);

	err = Pa_OpenStream(stream,
	                    (isInput ? &streamPar : NULL),  // input parameters
	                    (isInput ? NULL : &streamPar),  // output parameters
	                    SAMPLE_RATE,                    // sample rate (from Audio.h)
	                    frameSize,                      // frames per buffer
	                    paClipOff|paDitherOff,          // stream flags, do not clip or dither data
	                    NULL,                           // callback funtion, UNUSED
	                    NULL);                          // callback data, UNUSED
	if (err != paNoError) {
		qCritical("PortAudioSystem: Could not open PortAudio stream, error %s", Pa_GetErrorText(err));
		stream = 0;
		return false;
	}

	qWarning("PortAudioSystem Opened PortAudio stream %p", *stream);
	return true;
}

bool PortAudioSystem::terminateStream(PaStream *stream) {
	QMutexLocker lock(&qmStream);
	PaError err;

	if (!stream) {
		return false;
	}

	err = Pa_CloseStream(stream);
	if (err != paNoError) {
		qCritical("PortAudioSystem: Could not close PortAudio stream, error %s", Pa_GetErrorText(err));
		return false;
	}

	qWarning("PortAudioSystem: Closed PortAudio stream %p", stream);
	return true;
}

bool PortAudioSystem::startStream(PaStream *stream) {
	PaError err;

	if (!stream) {
		qWarning("PortAudioSystem: NULL PaStream passed to startStream()");
		return false;
	}

	err = Pa_IsStreamStopped(stream);
	if (err == 0) { // stream running
		return true;
	} else if (err != 1) { // some error, 1 means "stopped"
		qCritical("PortAudioSystem: Could not determine status of PortAudio stream, error %s", Pa_GetErrorText(err));
		return false;
	}

	err = Pa_StartStream(stream);
	if (err != paNoError) {
		qCritical("PortAudioSystem: Could not start PortAudio stream, error %s",Pa_GetErrorText(err));
		return false;
	}

	return true;
}

bool PortAudioSystem::stopStream(PaStream *stream) {
	PaError err;

	if (!stream) {
		qWarning("PortAudioSystem: NULL PaStream passed to stopStream()");
		return false;
	}

	err = Pa_IsStreamStopped(stream);
	if (err == 1) { // stream already stopped
		return true;
	} else if (err != 0) { // some error, 0 means "running"
		qCritical("PortAudioSystem: Could not determine status of PortAudio stream, error %s", Pa_GetErrorText(err));
		return false;
	}

	err = Pa_StopStream(stream);
	if (err != paNoError) {
		qWarning("PortAudioSystem: Could not stop PortAudio stream, error %s",Pa_GetErrorText(err));
		return false;
	}

	//qWarning() << "PortAudioSystem::stopStream(); stopped stream" << stream ;
	return true;
}

const QList<audioDevice> PortAudioSystem::enumerateDevices(bool input, PaDeviceIndex match) {
	PaHostApiIndex iApiCnt;
	PaHostApiIndex iApiIndex;
	int            iApiDevIndex;
	PaDeviceIndex  iDevIndex;
	const PaDeviceInfo *pDevInfo;

	QList<audioDevice> adl;

	adl << audioDevice(tr("Default Device"), -1);

	iApiCnt = Pa_GetHostApiCount();
	for (iApiIndex = 0; iApiIndex < iApiCnt; iApiIndex++) {
		const PaHostApiInfo *apiInfo = Pa_GetHostApiInfo(iApiIndex);
		if (!apiInfo)
			continue;

		QString qsApiName = QLatin1String(apiInfo->name);

		for (iApiDevIndex = 0; iApiDevIndex < apiInfo->deviceCount; iApiDevIndex++) {
			iDevIndex = Pa_HostApiDeviceIndexToDeviceIndex(iApiIndex, iApiDevIndex);
			pDevInfo = Pa_GetDeviceInfo(iDevIndex);
			if (!pDevInfo)
				continue;

			QString qsDevName = QLatin1String(pDevInfo->name);

			if ((input && (pDevInfo->maxInputChannels > 0)) ||
			        (!input && (pDevInfo->maxOutputChannels > 0)))
				adl << audioDevice(qsApiName + QLatin1String(": ") + qsDevName, iDevIndex);
		}
	}
	for (int i=0;i<adl.count();++i) {
		if (adl.at(i).second == match) {
			audioDevice ad = adl.takeAt(i);
			adl.prepend(ad);
			break;
		}
	}
	return adl;
}


// ------------------------------------------------------------------------------------------------


PortAudioInput::PortAudioInput() {
}

PortAudioInput::~PortAudioInput() {
	// thread should leave event loop soonish
	bRunning = false;
	// wait for end of thread
	wait();
}

void PortAudioInput::run() {
	PaStream           *inputStream = 0;
	PaError             err;

	//qWarning() << "PortAudioInput::run() BEGIN ===";
	if (!PortAudioSystem::initStream(&inputStream, g.s.iPortAudioInput, iFrameSize, reinterpret_cast<int *>(&iMicChannels), true))
		return; // PA init or stream opening failed, we will give up

	iMicFreq = SAMPLE_RATE;
	eMicFormat = SampleShort;
	initializeMixer();

	short inBuffer[iMicLength * iMicChannels];

	// depend on the stream having started
	bRunning = PortAudioSystem::startStream(inputStream);
	while (bRunning) {
		err = Pa_ReadStream(inputStream, inBuffer, iMicLength);
		if (err == paNoError) {
			addMic(inBuffer, iMicLength);
		} else if (err == paInputOverflowed) {
			qWarning("PortAudioInput: Overflow on PortAudio input-stream, we lost incoming data!");
		} else { // other error, aborg
			qWarning("PortAudioInput: Could not read from PortAudio stream, error %s", Pa_GetErrorText(err));
			bRunning = false;
		}
	}
	// ignoring return value on purpose, we cannot do anything about it anyway
	PortAudioSystem::stopStream(inputStream);

	// ignoring return value on purpose, we cannot do anything about it anyway
	PortAudioSystem::terminateStream(inputStream);
	inputStream = 0; // just for gdb sessions ;)
}



// ------------------------------------------------------------------------------------------------


PortAudioOutput::PortAudioOutput() {
	bRunning = true;
}

PortAudioOutput::~PortAudioOutput() {
	bRunning = false; // inform loop in run() about exit
	wipe(); // Call destructor of all children
	wait(); // wait for thread termination
}

//TODO: redo this without busy waiting if possible
void PortAudioOutput::run() {
	PaStream            *outputStream   = 0;
	PaError             err             = paNoError;
	int                 chans           = 0;
	bool                zero            = true;

	if (!PortAudioSystem::initStream(&outputStream, g.s.iPortAudioOutput, iFrameSize, &chans, false))
		return; // PA initialization or stream opening failed, we will give up

	const unsigned int cmask[2] = {
		SPEAKER_FRONT_LEFT,
		SPEAKER_FRONT_RIGHT
	};

	iChannels = chans;
	iMixerFreq = SAMPLE_RATE;
	eSampleFormat = SampleShort;
	initializeMixer(cmask);

	short outBuffer[iFrameSize * iChannels];

	while (bRunning) {
		bool avail = true;

		if (zero)
			memset(outBuffer, 0, sizeof(short) * iFrameSize * iChannels);
		else
			avail = mix(outBuffer, iFrameSize);

		if (avail) {
			if (! PortAudioSystem::startStream(outputStream)) {
				bRunning = false;
				break;
			}
			err = Pa_WriteStream(outputStream, outBuffer, iFrameSize);
			if (err == paOutputUnderflowed) {
				qWarning("PortAudioOutput: Output underflowed. Attempting graceful recovery.");
				zero = true;
				continue;
			} else if (err != paNoError) {
				qWarning("PortAudioOutput: Could not write to PortAudio stream, error %s", Pa_GetErrorText(err));
				bRunning = false;
			}
		} else {
			if (! PortAudioSystem::stopStream(outputStream))
				bRunning = false;
			this->msleep(20);
		}

		zero = false;
	}

	PortAudioSystem::terminateStream(outputStream);
	outputStream = NULL; // just for gdb sessions
}
