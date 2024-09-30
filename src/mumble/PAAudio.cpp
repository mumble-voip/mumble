// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "PAAudio.h"
#include "Global.h"

#ifdef Q_CC_GNU
#	define RESOLVE(var)                                                          \
		{                                                                         \
			var = reinterpret_cast< __typeof__(var) >(qlPortAudio.resolve(#var)); \
			if (!var)                                                             \
				return;                                                           \
		}
#else
#	define RESOLVE(var)                                                                           \
		{                                                                                          \
			*reinterpret_cast< void ** >(&var) = static_cast< void * >(qlPortAudio.resolve(#var)); \
			if (!var)                                                                              \
				return;                                                                            \
		}
#endif

static std::unique_ptr< PortAudioSystem > pas;

class PortAudioInputRegistrar : public AudioInputRegistrar {
private:
	AudioInput *create() Q_DECL_OVERRIDE;
	const QVariant getDeviceChoice() Q_DECL_OVERRIDE;
	const QList< audioDevice > getDeviceChoices() Q_DECL_OVERRIDE;
	void setDeviceChoice(const QVariant &, Settings &) Q_DECL_OVERRIDE;
	bool canEcho(EchoCancelOptionID echoCancelID, const QString &outputSystem) const Q_DECL_OVERRIDE;
	bool isMicrophoneAccessDeniedByOS() Q_DECL_OVERRIDE { return false; };

public:
	PortAudioInputRegistrar();
};

class PortAudioOutputRegistrar : public AudioOutputRegistrar {
private:
	AudioOutput *create() Q_DECL_OVERRIDE;
	const QVariant getDeviceChoice() Q_DECL_OVERRIDE;
	const QList< audioDevice > getDeviceChoices() Q_DECL_OVERRIDE;
	void setDeviceChoice(const QVariant &, Settings &) Q_DECL_OVERRIDE;

public:
	PortAudioOutputRegistrar();
};

class PortAudioInit : public DeferInit {
private:
	std::unique_ptr< PortAudioInputRegistrar > airPortAudio;
	std::unique_ptr< PortAudioOutputRegistrar > aorPortAudio;
	void initialize();
	void destroy();
};

PortAudioInputRegistrar::PortAudioInputRegistrar() : AudioInputRegistrar(QLatin1String("PortAudio")) {
}

AudioInput *PortAudioInputRegistrar::create() {
	return new PortAudioInput();
}

const QVariant PortAudioInputRegistrar::getDeviceChoice() {
	return Global::get().s.iPortAudioInput;
}

const QList< audioDevice > PortAudioInputRegistrar::getDeviceChoices() {
	return pas->enumerateDevices(true);
}

void PortAudioInputRegistrar::setDeviceChoice(const QVariant &choice, Settings &s) {
	s.iPortAudioInput = choice.toInt();
}

bool PortAudioInputRegistrar::canEcho(EchoCancelOptionID, const QString &) const {
	return false;
}

PortAudioOutputRegistrar::PortAudioOutputRegistrar() : AudioOutputRegistrar(QLatin1String("PortAudio")) {
}

AudioOutput *PortAudioOutputRegistrar::create() {
	return new PortAudioOutput();
}

const QVariant PortAudioOutputRegistrar::getDeviceChoice() {
	return Global::get().s.iPortAudioOutput;
}

const QList< audioDevice > PortAudioOutputRegistrar::getDeviceChoices() {
	return pas->enumerateDevices(false);
}

void PortAudioOutputRegistrar::setDeviceChoice(const QVariant &choice, Settings &s) {
	s.iPortAudioOutput = choice.toInt();
}

void PortAudioInit::initialize() {
	pas.reset(new PortAudioSystem());

	if (pas->bOk) {
		airPortAudio.reset(new PortAudioInputRegistrar());
		aorPortAudio.reset(new PortAudioOutputRegistrar());
	} else {
		pas.reset();
	}
}

void PortAudioInit::destroy() {
	airPortAudio.reset();
	aorPortAudio.reset();
	pas.reset();
}

// Instantiate PortAudioSystem, PortAudioInputRegistrar and PortAudioOutputRegistrar
static PortAudioInit pai;

PortAudioSystem::PortAudioSystem() : bOk(false) {
	QStringList alternatives;
#ifdef Q_OS_WIN
	alternatives << QLatin1String("portaudio_x64.dll");
	alternatives << QLatin1String("portaudio_x86.dll");
#elif defined(Q_OS_MAC)
	alternatives << QLatin1String("libportaudio.dylib");
	alternatives << QLatin1String("libportaudio.2.dylib");
#else
	alternatives << QLatin1String("libportaudio.so");
	alternatives << QLatin1String("libportaudio.so.2");
#endif
	for (const auto &lib : alternatives) {
		qlPortAudio.setFileName(lib);
		if (qlPortAudio.load()) {
			break;
		}
	}

	if (!qlPortAudio.isLoaded()) {
		return;
	}

	RESOLVE(Pa_GetVersionText)
	RESOLVE(Pa_GetErrorText)
	RESOLVE(Pa_Initialize)
	RESOLVE(Pa_Terminate)
	RESOLVE(Pa_OpenStream)
	RESOLVE(Pa_CloseStream)
	RESOLVE(Pa_StartStream)
	RESOLVE(Pa_StopStream)
	RESOLVE(Pa_IsStreamActive)
	RESOLVE(Pa_GetDefaultInputDevice)
	RESOLVE(Pa_GetDefaultOutputDevice)
	RESOLVE(Pa_HostApiDeviceIndexToDeviceIndex)
	RESOLVE(Pa_GetHostApiCount)
	RESOLVE(Pa_GetHostApiInfo)
	RESOLVE(Pa_GetDeviceInfo)

	const auto ret = Pa_Initialize();
	if (ret != paNoError) {
		qWarning("PortAudioSystem: failed to initialize library - Pa_Initialize() returned: %s", Pa_GetErrorText(ret));
		return;
	}

	bOk = true;

	qDebug("%s from %s", Pa_GetVersionText(), qPrintable(qlPortAudio.fileName()));
}

PortAudioSystem::~PortAudioSystem() {
	if (bOk) {
		const auto ret = Pa_Terminate();
		if (ret != paNoError) {
			qWarning("PortAudioSystem: failed to terminate library - Pa_Terminate() returned: %s",
					 Pa_GetErrorText(ret));
		}
	}
}

const QList< audioDevice > PortAudioSystem::enumerateDevices(const bool input) {
	QList< audioDevice > audioDevices;

	if (!bOk) {
		return audioDevices;
	}

	audioDevices << audioDevice(tr("Default device"), -1);

	for (PaHostApiIndex apiIndex = 0; apiIndex < Pa_GetHostApiCount(); ++apiIndex) {
		const auto *apiInfo = Pa_GetHostApiInfo(apiIndex);
		if (!apiInfo) {
			continue;
		}

		for (PaDeviceIndex apiDeviceIndex = 0; apiDeviceIndex < apiInfo->deviceCount; ++apiDeviceIndex) {
			const auto deviceIndex = Pa_HostApiDeviceIndexToDeviceIndex(apiIndex, apiDeviceIndex);
			const auto *deviceInfo = Pa_GetDeviceInfo(deviceIndex);
			if (!deviceInfo) {
				continue;
			}

			if ((input && (deviceInfo->maxInputChannels > 0)) || (!input && (deviceInfo->maxOutputChannels > 0))) {
				audioDevices << audioDevice(
					QLatin1String(apiInfo->name) + QLatin1String(": ") + QLatin1String(deviceInfo->name), deviceIndex);
			}
		}
	}

	return audioDevices;
}

bool PortAudioSystem::isStreamRunning(PaStream *stream) {
	if (!bOk || !stream) {
		return false;
	}

	const auto ret = Pa_IsStreamActive(stream);
	if (ret == 1) {
		return true;
	} else if (ret != 0) {
		qWarning("PortAudioSystem: failed to determine stream status - Pa_IsStreamActive() returned: %s",
				 Pa_GetErrorText(ret));
	}

	return false;
}

int PortAudioSystem::openStream(PaStream **stream, PaDeviceIndex device, const uint32_t frameSize, const bool isInput) {
	if (!bOk || !stream) {
		return 0;
	}

	QMutexLocker lock(&qmWait);

	// -1 is the default device
	if (device == -1) {
		device = (isInput ? Pa_GetDefaultInputDevice() : Pa_GetDefaultOutputDevice());
	}

	const auto *devInfo = Pa_GetDeviceInfo(device);
	if (!devInfo) {
		qWarning("PortAudioSystem: failed to retrieve info about device %d - Pa_GetDeviceInfo() returned: nullptr",
				 device);
		return 0;
	}

	const auto *apiInfo = Pa_GetHostApiInfo(devInfo->hostApi);
	if (!apiInfo) {
		qWarning(
			"PortAudioSystem: failed to retrieve info about API %d (device %d) - Pa_GetHostApiInfo() returned: nullptr",
			devInfo->hostApi, device);
		return 0;
	}

	qDebug("PortAudioSystem: using %s %s", apiInfo->name, devInfo->name);

	PaStreamParameters streamPar;
	streamPar.channelCount = 1;

	if (!isInput && devInfo->maxOutputChannels > 1) {
		// TODO: add support for more than 2 channels
		streamPar.channelCount = 2;
	}

	streamPar.device           = device;
	streamPar.sampleFormat     = paFloat32;
	streamPar.suggestedLatency = (isInput ? devInfo->defaultLowInputLatency : devInfo->defaultLowOutputLatency);
	streamPar.hostApiSpecificStreamInfo = nullptr;

	const auto ret =
		Pa_OpenStream(stream, isInput ? &streamPar : nullptr, isInput ? nullptr : &streamPar, SAMPLE_RATE, frameSize,
					  paClipOff | paDitherOff, &streamCallback, reinterpret_cast< void * >(isInput));
	if (ret != paNoError) {
		qWarning("PortAudioSystem: failed to open stream - Pa_OpenStream() returned: %s", Pa_GetErrorText(ret));
		*stream = nullptr;
		return 0;
	}

	return streamPar.channelCount;
}

bool PortAudioSystem::closeStream(PaStream *stream) {
	if (!bOk || !stream) {
		return false;
	}

	QMutexLocker lock(&qmWait);

	const auto ret = Pa_CloseStream(stream);
	if (ret != paNoError) {
		qWarning("PortAudioSystem: failed to close stream - Pa_CloseStream() returned: %s", Pa_GetErrorText(ret));
		return false;
	}

	return true;
}

bool PortAudioSystem::startStream(PaStream *stream) {
	if (!bOk) {
		return false;
	}

	if (isStreamRunning(stream)) {
		return true;
	}

	const auto ret = Pa_StartStream(stream);
	if (ret != paNoError) {
		qWarning("PortAudioSystem: failed to start stream - Pa_StartStream() returned: %s", Pa_GetErrorText(ret));
		return false;
	}

	return true;
}

bool PortAudioSystem::stopStream(PaStream *stream) {
	if (!bOk) {
		return false;
	}

	if (!isStreamRunning(stream)) {
		return true;
	}

	const auto ret = Pa_StopStream(stream);
	if (ret != paNoError) {
		qWarning("PortAudioSystem: failed to stop stream - Pa_StopStream() returned: %s", Pa_GetErrorText(ret));
		return false;
	}

	return true;
}

int PortAudioSystem::streamCallback(const void *inBuffer, void *outBuffer, unsigned long frames,
									const PaStreamCallbackTimeInfo *, PaStreamCallbackFlags, void *isInput) {
	if (isInput) {
		auto const input = dynamic_cast< PortAudioInput * >(Global::get().ai.get());
		if (!input) {
			return paAbort;
		}

		input->process(static_cast< unsigned int >(frames), inBuffer);
	} else {
		auto const output = dynamic_cast< PortAudioOutput * >(Global::get().ao.get());
		if (!output) {
			return paAbort;
		}

		output->process(static_cast< unsigned int >(frames), outBuffer);
	}

	return paContinue;
}

PortAudioInput::PortAudioInput() : stream(nullptr) {
	iMicChannels =
		static_cast< unsigned int >(pas->openStream(&stream, Global::get().s.iPortAudioInput, iFrameSize, true));
	if (!iMicChannels) {
		qWarning("PortAudioInput: failed to open stream");
		return;
	}

	iMicFreq   = SAMPLE_RATE;
	eMicFormat = SampleFloat;
	initializeMixer();

	if (!pas->startStream(stream)) {
		qWarning("PortAudioInput: failed to start stream");
	}
}

PortAudioInput::~PortAudioInput() {
	// Request interruption
	qmWait.lock();

	if (!pas->stopStream(stream)) {
		qWarning("PortAudioInput: failed to stop stream");
	}

	qwcSleep.wakeAll();
	qmWait.unlock();

	// Wait for thread to exit
	wait();

	// Cleanup
	if (!pas->closeStream(stream)) {
		qWarning("PortAudioInput: failed to close stream");
	}
}

void PortAudioInput::process(const uint32_t frames, const void *buffer) {
	addMic(buffer, frames);
}

void PortAudioInput::run() {
	if (!pas->isStreamRunning(stream)) {
		return;
	}

	// Pause thread until interruption is requested by the destructor
	qmWait.lock();
	qwcSleep.wait(&qmWait);
	qmWait.unlock();
}

PortAudioOutput::PortAudioOutput() : stream(nullptr) {
	iChannels =
		static_cast< unsigned int >(pas->openStream(&stream, Global::get().s.iPortAudioOutput, iFrameSize, false));
	if (!iChannels) {
		qWarning("PortAudioOutput: failed to open stream");
		return;
	}

	iMixerFreq    = SAMPLE_RATE;
	eSampleFormat = SampleFloat;

	const uint32_t channelsMask[]{ SPEAKER_FRONT_LEFT, SPEAKER_FRONT_RIGHT };

	initializeMixer(channelsMask);

	if (!pas->startStream(stream)) {
		qWarning("PortAudioOutput: failed to start stream");
	}
}

PortAudioOutput::~PortAudioOutput() {
	// Request interruption
	qmWait.lock();

	if (!pas->stopStream(stream)) {
		qWarning("PortAudioOutput: failed to stop stream");
	}

	qwcSleep.wakeAll();
	qmWait.unlock();

	// Wait for thread to exit
	wait();

	// Cleanup
	if (!pas->closeStream(stream)) {
		qWarning("PortAudioOutput: failed to close stream");
	}
}

void PortAudioOutput::process(const uint32_t frames, void *buffer) {
	if (!mix(buffer, frames)) {
		memset(buffer, 0, sizeof(float) * frames * iChannels);
	}
}

void PortAudioOutput::run() {
	if (!pas->isStreamRunning(stream)) {
		return;
	}

	// Pause thread until interruption is requested by the destructor
	qmWait.lock();
	qwcSleep.wait(&qmWait);
	qmWait.unlock();
}

#undef RESOLVE
