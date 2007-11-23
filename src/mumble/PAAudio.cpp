/* Copyright (C) 2005-2007, Thorvald Natvig <thorvald@natvig.com>
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


// ------------------------------------------------------------------------------------------------


class PortAudioInputRegistrar : public AudioInputRegistrar {
	public:
		PortAudioInputRegistrar();
		virtual AudioInput *create();
		virtual const QList<audioDevice> getDeviceChoices();
		virtual void setDeviceChoice(const QVariant &);
};

class PortAudioOutputRegistrar : public AudioOutputRegistrar {
	public:
		PortAudioOutputRegistrar();
		virtual AudioOutput *create();
		virtual const QList<audioDevice> getDeviceChoices();
		virtual void setDeviceChoice(const QVariant &);
};


static PortAudioInputRegistrar  sPAInputRegistrar;
static PortAudioOutputRegistrar sPAOutputRegistrar;


// ------------------------------------------------------------------------------------------------


PortAudioInputRegistrar::PortAudioInputRegistrar() : AudioInputRegistrar(QLatin1String("PortAudio")) {
	qWarning() << "PortAudioInputRegistrar::PortAudioInputRegistrar()";
}

AudioInput *PortAudioInputRegistrar::create() {
	qWarning() << "PortAudioInputRegistrar::create(), creating new audio input";
	return new PortAudioInput();
}

const QList<audioDevice> PortAudioInputRegistrar::getDeviceChoices() {
	qWarning() << "PortAudioInputRegistrar::getDeviceChoices()";
	QList<audioDevice> qlReturn;

	PortAudioSystemPtr pSys = PortAudioSystem::self();

	QHash<PaHostApiIndex, PortAudioSystem::HostApiDevices>::const_iterator apiIt;
	QHash<PaHostApiIndex, PortAudioSystem::HostApiDevices>::const_iterator apiItEnd = pSys->qhHostApis.constEnd();
	for (apiIt = pSys->qhHostApis.constBegin(); apiIt != apiItEnd; ++apiIt) {
		const PortAudioSystem::HostApiDevices devs = apiIt.value();

		QHash<PaDeviceIndex, QString>::const_iterator devIt;
		QHash<PaDeviceIndex, QString>::const_iterator devItEnd;

		devItEnd = devs.qhInputs.constEnd();
		for (devIt = devs.qhInputs.constBegin(); devIt != devItEnd; ++devIt) {
			QString devName;
			devName = QString("%1 (%2)").arg(devIt.value(), devs.qsHostApiName);
			qlReturn << audioDevice(devName, devIt.key());
		}
	}

	return qlReturn;
}

void PortAudioInputRegistrar::setDeviceChoice(const QVariant &choice) {
	g.s.iPortAudioInput = choice.toInt();
}


// ------------------------------------------------------------------------------------------------


PortAudioOutputRegistrar::PortAudioOutputRegistrar() : AudioOutputRegistrar(QLatin1String("PortAudio")) {
	qWarning() << "PortAudioOutputRegistrar::PortAudioOutputRegistrar()";
}

AudioOutput *PortAudioOutputRegistrar::create() {
	qWarning() << "PortAudioOutputRegistrar::create(), creating new audio output";
	return new PortAudioOutput();
}

const QList<audioDevice> PortAudioOutputRegistrar::getDeviceChoices() {
	qWarning() << "PortAudioOutputRegistrar::getDeviceChoices()";
	QList<audioDevice> qlReturn;
	PortAudioSystemPtr pSys = PortAudioSystem::self();

	QHash<PaHostApiIndex, PortAudioSystem::HostApiDevices>::const_iterator apiIt;
	QHash<PaHostApiIndex, PortAudioSystem::HostApiDevices>::const_iterator apiItEnd = pSys->qhHostApis.constEnd();
	for (apiIt = pSys->qhHostApis.constBegin(); apiIt != apiItEnd; ++apiIt) {
		const PortAudioSystem::HostApiDevices devs = apiIt.value();

		QHash<PaDeviceIndex, QString>::const_iterator devIt;
		QHash<PaDeviceIndex, QString>::const_iterator devItEnd;

		devItEnd = devs.qhOutputs.constEnd();
		for (devIt = devs.qhOutputs.constBegin(); devIt != devItEnd; ++devIt) {
			QString devName;
			devName = QString("%1 (%2)").arg(devIt.value(), devs.qsHostApiName);
			qlReturn << audioDevice(devName, devIt.key());
		}
	}

	return qlReturn;
}

void PortAudioOutputRegistrar::setDeviceChoice(const QVariant &choice) {
	g.s.iPortAudioOutput = choice.toInt();
}


// ------------------------------------------------------------------------------------------------


// init static members
WeakPortAudioSystemPtr PortAudioSystem::wpaSystem;
QMutex                 PortAudioSystem::qmSystem;

PortAudioSystem::PortAudioSystem() {
	qWarning() << "PortAudioSystem::PortAudioSystem()";

	PaError err = Pa_Initialize();
	if (err != paNoError) {
		qCritical() << "Could not initialize PortAudio library, error:" << Pa_GetErrorText(err);
		return; // will probably make mumble crash but we're lost anyway
	}
	qWarning() << "Initialized PortAudio library";

	enumerateDevices(); // fill qhInput and qhOutput
}

PortAudioSystem::~PortAudioSystem() {
	qWarning() << "PortAudioSystem::~PortAudioSystem()";

	PaError err = Pa_Terminate();
	if (err != paNoError) {
		qWarning() << "Could not terminate PortAudio library, error:" << Pa_GetErrorText(err);
		// returning here makes little sense
	} else
		qWarning() << "Terminated PortAudio library";
}

PortAudioSystemPtr PortAudioSystem::self() {
	qmSystem.lock();
	PortAudioSystemPtr ptr = wpaSystem.lock();
	if (!ptr) {
		ptr = PortAudioSystemPtr(new PortAudioSystem());
		wpaSystem = ptr;
	}
	qmSystem.unlock();
	return ptr;
}

bool PortAudioSystem::initStream(PaStream **stream, PaDeviceIndex devIndex, int frameSize, bool isInput) {
	QMutexLocker lock(&muStream);
	PaError            err;
	PaStreamParameters streamPar;
	//qWarning() << "PortAudioSystem::initStream()";

	if (!stream) // null pointer passed
		return false;

	if (*stream) // pointer valid but already initialized, exit gracefully
		return true;

	// -1 is default device, otherwise pass value straight to PA
	PaDeviceIndex devId = devIndex;
	qWarning() << "Will use device" << devId;
	if (devId == -1)
		devId = (isInput ? Pa_GetDefaultInputDevice() : Pa_GetDefaultOutputDevice());

	const PaDeviceInfo *devInfo = Pa_GetDeviceInfo(devId);
	if (!devInfo)
		return false;

	streamPar.device                    = devId;
	streamPar.channelCount              = 1;
	streamPar.sampleFormat              = paInt16;
	//TODO: Can I determine this latency from mumble settings?
	streamPar.suggestedLatency          = (isInput ? devInfo->defaultHighInputLatency : devInfo->defaultHighOutputLatency);
	streamPar.hostApiSpecificStreamInfo = NULL;

	qWarning() << "suggestedLatency (ms):" << int(streamPar.suggestedLatency * 1000);
	qWarning() << "frameSize (bytes)    :" << frameSize;
	qWarning() << "sample rate (bits/s) :" << SAMPLE_RATE;

	err = Pa_OpenStream(stream,
	                    (isInput ? &streamPar : NULL),  // input parameters
	                    (isInput ? NULL : &streamPar),  // output parameters
	                    SAMPLE_RATE,                    // sample rate (from Audio.h)
	                    frameSize,                      // frames per buffer
	                    paClipOff|paDitherOff,          // stream flags, do not clip or dither data
	                    NULL,                           // callback funtion, UNUSED
	                    NULL);                          // callback data, UNUSED
	if (err != paNoError) {
		qCritical() << "Could not open PortAudio stream, error:" << Pa_GetErrorText(err);
		stream = 0;
		return false;
	}

	qWarning() << "Opened PortAudio stream" << *stream;
	return true;
}

bool PortAudioSystem::terminateStream(PaStream *stream) {
	QMutexLocker lock(&muStream);
	PaError err;
	//qWarning() << "PortAudioSystem::terminateStream()";

	if (!stream) {
		qWarning() << "NULL PaStream passed to PortAudioSystem::terminateStream()";
		return false;
	}

	err = Pa_CloseStream(stream);
	if (err != paNoError) {
		qCritical() << "Could not close PortAudio stream, error:" << Pa_GetErrorText(err);
		return false;
	}

	qWarning() << "Closed PortAudio stream" << stream;
	return true;
}

bool PortAudioSystem::startStream(PaStream *stream) {
	//TODO: check if we need to lock muStream in here, I'd like to avoid this
	PaError err;

	if (!stream) {
		qWarning() << "NULL PaStream passed to PortAudioSystem::startStream()";
		return false;
	}

	err = Pa_IsStreamStopped(stream);
	if (err == 0) { // stream running
		return true;
	} else if (err != 1) { // some error, 1 means "stopped"
		qCritical() << "Could not determine status of PortAudio stream, error:" << Pa_GetErrorText(err);
		return false;
	}

	err = Pa_StartStream(stream);
	if (err != paNoError) {
		qCritical() << "Could not start PortAudio stream, error:" << Pa_GetErrorText(err);
		return false;
	}

	//qWarning() << "PortAudioSystem::startStream(); started stream" << stream;
	return true;
}

bool PortAudioSystem::stopStream(PaStream *stream) {
	//TODO: check if we need to lock muStream in here, I'd like to avoid this
	PaError err;

	if (!stream) {
		qWarning() << "NULL PaStream passed to PortAudioSystem::stopStream()";
		return false;
	}

	err = Pa_IsStreamStopped(stream);
	if (err == 1) { // stream already stopped
		return true;
	} else if (err != 0) { // some error, 0 means "running"
		qCritical() << "Could not determine status of PortAudio stream, error:" << Pa_GetErrorText(err);
		return false;
	}

	err = Pa_StopStream(stream);
	if (err != paNoError) {
		qWarning() << "Could not stop PortAudio stream, error:" << Pa_GetErrorText(err);
		return false;
	}

	//qWarning() << "PortAudioSystem::stopStream(); stopped stream" << stream ;
	return true;
}

void PortAudioSystem::enumerateDevices() {
	PaHostApiIndex iApiCnt;      // number of Host-APIs
	PaHostApiIndex iApiIndex;    // current index of Host-API
	int            iApiDevIndex; // current Host-API device index (needs mapping to PaDeviceIndex)
	PaDeviceIndex  iDevIndex;    // current device index (the one we can use later on)
	const PaDeviceInfo *pDevInfo;

	HostApiDevices &devs = qhHostApis[-1];
	// Technically this mapping is wrong
	// Default input and output could refer to two different devices but users won't care anyway
	devs.qsHostApiName  = QLatin1String("Default Device");
	devs.qhInputs[-1]   = QLatin1String("Default Input");
	devs.qhOutputs[-1]  = QLatin1String("Default Output");

	iApiCnt = Pa_GetHostApiCount();
	for (iApiIndex = 0; iApiIndex < iApiCnt; iApiIndex++) {
		const PaHostApiInfo *apiInfo = Pa_GetHostApiInfo(iApiIndex);
		if (!apiInfo)
			continue;

		QString qsApiName = QString::fromLocal8Bit(apiInfo->name);

		for (iApiDevIndex = 0; iApiDevIndex < apiInfo->deviceCount; iApiDevIndex++) {
			iDevIndex = Pa_HostApiDeviceIndexToDeviceIndex(iApiIndex, iApiDevIndex);
			pDevInfo = Pa_GetDeviceInfo(iDevIndex);
			if (!pDevInfo)
				continue;

			QString qsDevName = QString::fromLocal8Bit(pDevInfo->name);

			//TODO: more extensive test on device capabilities
			if (pDevInfo->maxInputChannels > 0) {
				HostApiDevices &devs = qhHostApis[iApiIndex];

				devs.qsHostApiName = qsApiName; // ensure host api has a name
				devs.qhInputs[iDevIndex] = qsDevName;
			}

			if (pDevInfo->maxOutputChannels > 0) {
				HostApiDevices &devs = qhHostApis[iApiIndex];

				devs.qsHostApiName = qsApiName; // ensure host api has a name
				devs.qhOutputs[iDevIndex] = qsDevName;
			}
		}
	}
}


// ------------------------------------------------------------------------------------------------


PortAudioInput::PortAudioInput() {
	qWarning() << "PortAudioInput::PortAudioInput()";
}

PortAudioInput::~PortAudioInput() {
	qWarning() << "PortAudioInput::~PortAudioInput()";
	// thread should leave event loop soonish
	bRunning = false;
	// wait for end of thread
	wait();
}

void PortAudioInput::run() {
	PortAudioSystemPtr  pSys = PortAudioSystem::self();
	PaStream           *inputStream = 0;
	PaError             err;

	//qWarning() << "PortAudioInput::run() BEGIN ===";
	if (!pSys->initStream(&inputStream, g.s.iPortAudioInput, iFrameSize, true))
		return; // PA init or stream opening failed, we will give up

	// depend on the stream having started
	bRunning = pSys->startStream(inputStream);
	while (bRunning) {
		err = Pa_ReadStream(inputStream, psMic, iFrameSize);
		if (err == paNoError) {
			encodeAudioFrame();
		} else if (err == paInputOverflow) {
			qWarning() << "Overflow on PortAudio input-stream, we lost incoming data!";
		} else { // other error, aborg
			qWarning() << "Could not read from PortAudio stream, error:" << Pa_GetErrorText(err);
			bRunning = false;
		}
	}
	// ignoring return value on purpose, we cannot do anything about it anyway
	pSys->stopStream(inputStream);

	// ignoring return value on purpose, we cannot do anything about it anyway
	pSys->terminateStream(inputStream);
	inputStream = 0; // just for gdb sessions ;)
}



// ------------------------------------------------------------------------------------------------


PortAudioOutput::PortAudioOutput() {
	qWarning() << "PortAudioOutput::PortAudioOutput()";
	bRunning = true;
}

PortAudioOutput::~PortAudioOutput() {
	qWarning() << "PortAudioOutput::~PortAudioOutput()";
	bRunning = false; // inform loop in run() about exit
	wipe(); // Call destructor of all children
	wait(); // wait for thread termination
}

//TODO: redo this without busy waiting if possible
void PortAudioOutput::run() {
	PortAudioSystemPtr  pSys = PortAudioSystem::self();
	PaStream           *outputStream    = 0;
	short               outBuffer[iFrameSize];
	bool                hasMoreToMix    = true;
	PaError             err             = paNoError;

	if (!pSys->initStream(&outputStream, g.s.iPortAudioOutput, iFrameSize, false))
		return; // PA initialization or stream opening failed, we will give up

	while (bRunning) {
		bool nextHasMoreToMix = mixAudio(outBuffer);
		if (hasMoreToMix) {
			if (pSys->startStream(outputStream)) {
				err = Pa_WriteStream(outputStream, outBuffer, iFrameSize);
				if (err != paNoError) {
					qWarning() << "Could not write to PortAudio stream, error:" << Pa_GetErrorText(err);
					bRunning = false;
				}
			} else
				bRunning = false;
		} else {
			if (!pSys->stopStream(outputStream))
				bRunning = false;
			this->usleep(50 * 1000); // 50ms wait to avoid hogging the cpu too much
		}
		hasMoreToMix = nextHasMoreToMix;
	} // while

	// ignoring return value of terminateStream, we cannot do anything about it anyway
	pSys->terminateStream(outputStream);
	outputStream = 0; // just for gdb sessions
}
