// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "CoreAudio.h"

#include "User.h"

#include <Carbon/Carbon.h>

// We define a global macro called 'g'. This can lead to issues when included code uses 'g' as a type or parameter name (like protobuf 3.7 does). As such, for now, we have to make this our last include.
#include "Global.h"

// Ignore deprecation warnings for the whole file, for now.
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

class CoreAudioInputRegistrar : public AudioInputRegistrar {
	public:
		CoreAudioInputRegistrar();
		virtual AudioInput *create();
		virtual const QList<audioDevice> getDeviceChoices();
		virtual void setDeviceChoice(const QVariant &, Settings &);
		virtual bool canEcho(const QString &) const;
};

class CoreAudioOutputRegistrar : public AudioOutputRegistrar {
	public:
		CoreAudioOutputRegistrar();
		virtual AudioOutput *create();
		virtual const QList<audioDevice> getDeviceChoices();
		virtual void setDeviceChoice(const QVariant &, Settings &);
		bool canMuteOthers() const;
};

class CoreAudioInit : public DeferInit {
		CoreAudioInputRegistrar *cairReg;
		CoreAudioOutputRegistrar *caorReg;
	public:
		CoreAudioInit() : cairReg(NULL), caorReg(NULL) {}
		void initialize();
		void destroy();
};

static CoreAudioInit cainit;

void CoreAudioInit::initialize() {
	cairReg = new CoreAudioInputRegistrar();
	caorReg = new CoreAudioOutputRegistrar();
}

void CoreAudioInit::destroy() {
	delete cairReg;
	delete caorReg;
}

CFStringRef CoreAudioSystem::QStringToCFString(const QString &str) {
	return CFStringCreateWithCharacters(kCFAllocatorDefault, reinterpret_cast<const UniChar *>(str.unicode()), str.length());
}

const QList<audioDevice> CoreAudioSystem::getDeviceChoices(bool input) {
	QHash<QString, QString> qhDevices = CoreAudioSystem::getDevices(input);
	QList<audioDevice> qlReturn;
	QStringList qlDevices;

	qhDevices.insert(QString(), tr("Default Device"));
	qlDevices = qhDevices.keys();

	const QString &qsDev = input ? g.s.qsCoreAudioInput : g.s.qsCoreAudioOutput;
	if (qlDevices.contains(qsDev)) {
		qlDevices.removeAll(qsDev);
		qlDevices.prepend(qsDev);
	}

	foreach(const QString &qsIdentifier, qlDevices) {
		qlReturn << audioDevice(qhDevices.value(qsIdentifier), qsIdentifier);
	}

	return qlReturn;
}

const QHash<QString, QString> CoreAudioSystem::getDevices(bool input) {
	QHash<QString, QString> qhReturn;
	UInt32 len, ndevs;
	OSStatus err;

	err = AudioHardwareGetPropertyInfo(kAudioHardwarePropertyDevices, &len, NULL);
	if (err != noErr)
		return qhReturn;

	ndevs = len / sizeof(AudioDeviceID);
	AudioDeviceID devs[ndevs];

	err = AudioHardwareGetProperty(kAudioHardwarePropertyDevices, &len, devs);
	if (err != noErr)
		return qhReturn;

	for (UInt32 i = 0; i < ndevs; i++) {
		QString qsDeviceName;
		QString qsDeviceIdentifier;
		char buf[4096];

		/* Get number of channels, to determine whether we're an input or an output... */
		AudioBufferList *bufs = NULL;

		err = AudioDeviceGetPropertyInfo(devs[i], 0, input, kAudioDevicePropertyStreamConfiguration, &len, NULL);
		if (err != noErr) {
			qWarning("CoreAudioSystem: Failed to get length of AudioStreamConfiguration. Unable to allocate.");
			continue;
		}

		bufs = reinterpret_cast<AudioBufferList *>(malloc(len));
		err = AudioDeviceGetProperty(devs[i], 0, input, kAudioDevicePropertyStreamConfiguration, &len, bufs);
		if (! bufs || err != noErr) {
			qWarning("CoreAudioSystem: Failed to get AudioStreamConfiguration from device.");
			free(bufs);
			continue;
		}

		UInt32 channels = 0;
		for (UInt32 j = 0; j < bufs->mNumberBuffers; j++) {
			channels += bufs->mBuffers[j].mNumberChannels;
		}

		free(bufs);

		if (! channels)
			continue;

		/* Get device name. */
		len = sizeof(CFStringRef);
		CFStringRef devName = NULL;
		err = AudioDeviceGetProperty(devs[i], 0, 0, kAudioDevicePropertyDeviceNameCFString, &len, &devName);
		if (! devName || err != noErr) {
			qWarning("CoreAudioSystem: Failed to get device name. Skipping device.");
		}
		CFStringGetCString(devName, buf, 4096, kCFStringEncodingUTF8);
		qsDeviceName = QString::fromUtf8(buf);
		CFRelease(devName);

		/* Device UID. */
		CFStringRef devUid = NULL;
		err = AudioDeviceGetProperty(devs[i], 0, 0, kAudioDevicePropertyDeviceUID, &len, &devUid);
		if (! devUid || err != noErr) {
			qWarning("CoreAudioSystem: Failed to get device UID. Skipping device.");
		}
		CFStringGetCString(devUid, buf, 4096, kCFStringEncodingUTF8);
		qsDeviceIdentifier = QString::fromUtf8(buf);
		CFRelease(devUid);

		qhReturn.insert(qsDeviceIdentifier, qsDeviceName);
	}

	return qhReturn;
}

CoreAudioInputRegistrar::CoreAudioInputRegistrar() : AudioInputRegistrar(QLatin1String("CoreAudio"), 10) {
}

AudioInput *CoreAudioInputRegistrar::create() {
	return new CoreAudioInput();
}

const QList<audioDevice> CoreAudioInputRegistrar::getDeviceChoices() {
	return CoreAudioSystem::getDeviceChoices(true);
}

void CoreAudioInputRegistrar::setDeviceChoice(const QVariant &choice, Settings &s) {
	s.qsCoreAudioInput = choice.toString();
}

bool CoreAudioInputRegistrar::canEcho(const QString &outputsys) const {
	Q_UNUSED(outputsys);

	return false;
}

CoreAudioOutputRegistrar::CoreAudioOutputRegistrar() : AudioOutputRegistrar(QLatin1String("CoreAudio"), 10) {
}

AudioOutput *CoreAudioOutputRegistrar::create() {
	return new CoreAudioOutput();
}

const QList<audioDevice> CoreAudioOutputRegistrar::getDeviceChoices() {
	return CoreAudioSystem::getDeviceChoices(false);
}

void CoreAudioOutputRegistrar::setDeviceChoice(const QVariant &choice, Settings &s) {
	s.qsCoreAudioOutput = choice.toString();
}

bool CoreAudioOutputRegistrar::canMuteOthers() const {
	return false;
}

CoreAudioInput::CoreAudioInput() {
	OSStatus err;
	AudioStreamBasicDescription fmt;
	AudioDeviceID devId = 0;
	CFStringRef devUid = NULL;
	UInt32 val, len;

	memset(&buflist, 0, sizeof(AudioBufferList));

	if (! g.s.qsCoreAudioInput.isEmpty()) {
		qWarning("CoreAudioInput: Set device to '%s'.", qPrintable(g.s.qsCoreAudioInput));
		devUid = CoreAudioSystem::QStringToCFString(g.s.qsCoreAudioInput);

		AudioValueTranslation avt;
		avt.mInputData = const_cast<struct __CFString **>(&devUid);
		avt.mInputDataSize = sizeof(CFStringRef *);
		avt.mOutputData = &devId;
		avt.mOutputDataSize = sizeof(AudioDeviceID);

		len = sizeof(AudioValueTranslation);
		err = AudioHardwareGetProperty(kAudioHardwarePropertyDeviceForUID, &len, &avt);
		if (err != noErr) {
			qWarning("CoreAudioInput: Unable to query for AudioDeviceID.");
			return;
		}
	} else {
		qWarning("CoreAudioInput: Set device to 'Default Device'.");

		len = sizeof(AudioDeviceID);
		err = AudioHardwareGetProperty(kAudioHardwarePropertyDefaultInputDevice, &len, &devId);
		if (err != noErr) {
			qWarning("CoreAudioInput: Unable to query for default input AudioDeviceID.");
			return;
		}

		len = sizeof(CFStringRef);
		err = AudioDeviceGetProperty(devId, 0, true, kAudioDevicePropertyDeviceUID, &len, &devUid);
		if (err != noErr) {
			qWarning("CoreAudioInput: Unable to get default device UID.");
			return;
		}
	}

	Component comp;
	ComponentDescription desc;

	desc.componentType = kAudioUnitType_Output;
	desc.componentSubType = kAudioUnitSubType_HALOutput;
	desc.componentManufacturer = kAudioUnitManufacturer_Apple;
	desc.componentFlags = 0;
	desc.componentFlagsMask = 0;

	comp = FindNextComponent(NULL, &desc);
	if (comp == NULL) {
		qWarning("CoreAudioInput: Unable to find AudioUnit.");
		return;
	}

	err = OpenAComponent(comp, &au);
	if (err != noErr) {
		qWarning("CoreAudioInput: Unable to open AudioUnit component.");
		return;
	}

	err = AudioUnitInitialize(au);
	if (err != noErr) {
		qWarning("CoreAudioInput: Unable to initialize AudioUnit.");
		return;
	}

	val = 1;
	err = AudioUnitSetProperty(au, kAudioOutputUnitProperty_EnableIO, kAudioUnitScope_Input, 1, &val, sizeof(UInt32));
	if (err != noErr) {
		qWarning("CoreAudioInput: Unable to configure input scope on AudioUnit.");
		return;
	}

	val = 0;
	err = AudioUnitSetProperty(au, kAudioOutputUnitProperty_EnableIO, kAudioUnitScope_Output, 0, &val, sizeof(UInt32));
	if (err != noErr) {
		qWarning("CoreAudioInput: Unable to configure output scope on AudioUnit.");
		return;
	}

	len = sizeof(AudioDeviceID);
	err = AudioUnitSetProperty(au, kAudioOutputUnitProperty_CurrentDevice, kAudioUnitScope_Global, 0, &devId, len);
	if (err != noErr) {
		qWarning("CoreAudioInput: Unable to set device of AudioUnit.");
		return;
	}

	len = sizeof(AudioStreamBasicDescription);
	err = AudioUnitGetProperty(au, kAudioUnitProperty_StreamFormat, kAudioUnitScope_Input, 1, &fmt, &len);
	if (err != noErr) {
		qWarning("CoreAudioInput: Unable to query device for stream info.");
		return;
	}

	if (fmt.mFormatFlags & kAudioFormatFlagIsFloat) {
		eMicFormat = SampleFloat;
	} else if (fmt.mFormatFlags & kAudioFormatFlagIsSignedInteger) {
		eMicFormat = SampleShort;
	}

	if (fmt.mChannelsPerFrame > 1) {
		qWarning("CoreAudioInput: Input device with more than one channel detected. Defaulting to 1.");
	}

	iMicFreq = static_cast<int>(fmt.mSampleRate);
	iMicChannels = 1;
	initializeMixer();

	if (eMicFormat == SampleFloat) {
		fmt.mFormatFlags = kAudioFormatFlagIsFloat | kAudioFormatFlagIsPacked;
		fmt.mBitsPerChannel = sizeof(float) * 8;
	} else if (eMicFormat == SampleShort) {
		fmt.mFormatFlags = kAudioFormatFlagIsSignedInteger | kAudioFormatFlagIsPacked;
		fmt.mBitsPerChannel = sizeof(short) * 8;
	}

	fmt.mFormatID = kAudioFormatLinearPCM;
	fmt.mSampleRate = iMicFreq;
	fmt.mChannelsPerFrame = iMicChannels;
	fmt.mBytesPerFrame = iMicSampleSize;
	fmt.mBytesPerPacket = iMicSampleSize;
	fmt.mFramesPerPacket = 1;

	len = sizeof(AudioStreamBasicDescription);
	err = AudioUnitSetProperty(au, kAudioUnitProperty_StreamFormat, kAudioUnitScope_Output, 1, &fmt, len);
	if (err != noErr) {
		qWarning("CoreAudioInput: Unable to set stream format for output device.");
		return;
	}

	err = AudioUnitAddPropertyListener(au, kAudioUnitProperty_StreamFormat, CoreAudioInput::propertyChange, this);
	if (err != noErr) {
		qWarning("CoreAudioInput: Unable to create input property change listener. Unable to listen to property change events.");
	}

	AURenderCallbackStruct cb;
	cb.inputProc = CoreAudioInput::inputCallback;
	cb.inputProcRefCon = this;
	len = sizeof(AURenderCallbackStruct);
	err = AudioUnitSetProperty(au, kAudioOutputUnitProperty_SetInputCallback, kAudioUnitScope_Global, 0, &cb, len);
	if (err != noErr) {
		qWarning("CoreAudioInput: Unable to setup callback.");
		return;
	}

	AudioValueRange range;
	len = sizeof(AudioValueRange);
	err = AudioDeviceGetProperty(devId, 0, true, kAudioDevicePropertyBufferFrameSizeRange, &len, &range);
	if (err != noErr) {
		qWarning("CoreAudioInput: Unable to query for allowed buffer size ranges.");
	} else {
		qWarning("CoreAudioInput: BufferFrameSizeRange = (%.2f, %.2f)", range.mMinimum, range.mMaximum);
	}

	int iActualBufferLength = iMicLength;
	val = iMicLength;
	err = AudioDeviceSetProperty(devId, NULL, 0, true, kAudioDevicePropertyBufferFrameSize, sizeof(UInt32), &val);
	if (err != noErr) {
		qWarning("CoreAudioInput: Unable to set preferred buffer size on device. Querying for device default.");
		len = sizeof(UInt32);
		err = AudioDeviceGetProperty(devId, 0, true, kAudioDevicePropertyBufferFrameSize, &len, &val);
		if (err != noErr) {
			qWarning("CoreAudioInput: Unable to query device for buffer size.");
			return;
		}

		iActualBufferLength = (int) val;
	}

	buflist.mNumberBuffers = 1;
	AudioBuffer *b = buflist.mBuffers;
	b->mNumberChannels = iMicChannels;
	b->mDataByteSize = iMicSampleSize * iActualBufferLength;
	b->mData = calloc(1, b->mDataByteSize);

	err = AudioOutputUnitStart(au);
	if (err != noErr) {
		qWarning("CoreAudioInput: Unable to start AudioUnit.");
		return;
	}

	bRunning = true;
};

CoreAudioInput::~CoreAudioInput() {
	OSStatus err;

	bRunning = false;
	wait();

	if (au) {
		err = AudioOutputUnitStop(au);
		if (err != noErr) {
			qWarning("CoreAudioInput: Unable to stop AudioUnit.");
		}
	}

	AudioBuffer *b = buflist.mBuffers;
	if (b && b->mData)
		free(b->mData);

	qWarning("CoreAudioInput: Shutting down.");
}

OSStatus CoreAudioInput::inputCallback(void *udata, AudioUnitRenderActionFlags *flags, const AudioTimeStamp *ts,
                                       UInt32 busnum, UInt32 nframes, AudioBufferList *buflist) {
	Q_UNUSED(udata);
	Q_UNUSED(buflist);

	CoreAudioInput *i = reinterpret_cast<CoreAudioInput *>(udata);
	OSStatus err;

	err = AudioUnitRender(i->au, flags, ts, busnum, nframes, &i->buflist);
	if (err != noErr) {
		qWarning("CoreAudioInput: AudioUnitRender failed.");
		return err;
	}

	i->addMic(i->buflist.mBuffers->mData, nframes);

	return noErr;
}

void CoreAudioInput::propertyChange(void *udata, AudioUnit au, AudioUnitPropertyID prop, AudioUnitScope scope, AudioUnitElement element) {
	Q_UNUSED(udata);
	Q_UNUSED(au);
	Q_UNUSED(scope);
	Q_UNUSED(element);

	if (prop == kAudioUnitProperty_StreamFormat) {
		qWarning("CoreAudioInput: Stream format change detected. Restarting AudioInput.");
		Audio::stopInput();
		Audio::startInput();
	} else {
		qWarning("CoreAudioInput: Unexpected property changed event received.");
	}
}

void CoreAudioInput::run() {
}

CoreAudioOutput::CoreAudioOutput() {
	OSStatus err;
	AudioStreamBasicDescription fmt;
	unsigned int chanmasks[32];
	AudioDeviceID devId = 0;
	CFStringRef devUid = NULL;
	UInt32 len;

	if (! g.s.qsCoreAudioOutput.isEmpty()) {
		devUid = CoreAudioSystem::QStringToCFString(g.s.qsCoreAudioOutput);
		qWarning("CoreAudioOutput: Set device to '%s'.", qPrintable(g.s.qsCoreAudioOutput));

		AudioValueTranslation avt;
		avt.mInputData = const_cast<struct __CFString **>(&devUid);
		avt.mInputDataSize = sizeof(CFStringRef *);
		avt.mOutputData = &devId;
		avt.mOutputDataSize = sizeof(AudioDeviceID);

		len = sizeof(AudioValueTranslation);
		err = AudioHardwareGetProperty(kAudioHardwarePropertyDeviceForUID, &len, &avt);
		if (err != noErr) {
			qWarning("CoreAudioOutput: Unable to query for AudioDeviceID.");
			return;
		}
	} else {
		qWarning("CoreAudioOutput: Set device to 'Default Device'.");

		len = sizeof(AudioDeviceID);
		err = AudioHardwareGetProperty(kAudioHardwarePropertyDefaultOutputDevice, &len, &devId);
		if (err != noErr) {
			qWarning("CoreAudioOutput: Unable to query for default output AudioDeviceID");
			return;
		}

		len = sizeof(CFStringRef);
		err = AudioDeviceGetProperty(devId, 0, false, kAudioDevicePropertyDeviceUID, &len, &devUid);
		if (err != noErr) {
			qWarning("CoreAudioOutput: Unable to get default device UID.");
			return;
		}

	}

	Component comp;
	ComponentDescription desc;

	desc.componentType = kAudioUnitType_Output;
	desc.componentSubType = kAudioUnitSubType_HALOutput;
	desc.componentManufacturer = kAudioUnitManufacturer_Apple;
	desc.componentFlags = 0;
	desc.componentFlagsMask = 0;

	comp = FindNextComponent(NULL, &desc);
	if (comp == NULL) {
		qWarning("CoreAudioOuput: Unable to find AudioUnit.");
		return;
	}

	err = OpenAComponent(comp, &au);
	if (err != noErr) {
		qWarning("CoreAudioOutput: Unable to open AudioUnit component.");
		return;
	}

	err = AudioUnitInitialize(au);
	if (err != noErr) {
		qWarning("CoreAudioOutput: Unable to initialize output AudioUnit");
		return;
	}

	len = sizeof(AudioDeviceID);
	err = AudioUnitSetProperty(au, kAudioOutputUnitProperty_CurrentDevice, kAudioUnitScope_Global, 0, &devId, len);
	if (err != noErr) {
		qWarning("CoreAudioOutput: Unable to set CurrentDevice property on AudioUnit.");
		return;
	}

	len = sizeof(AudioStreamBasicDescription);
	err = AudioUnitGetProperty(au, kAudioUnitProperty_StreamFormat, kAudioUnitScope_Output, 0, &fmt, &len);
	if (err != noErr) {
		qWarning("CoreAudioOuptut: Unable to query device for stream info.");
		return;
	}

	iMixerFreq = static_cast<int>(fmt.mSampleRate);
	iChannels = static_cast<int>(fmt.mChannelsPerFrame);

	if (fmt.mFormatFlags & kAudioFormatFlagIsFloat) {
		eSampleFormat = SampleFloat;
	} else if (fmt.mFormatFlags & kAudioFormatFlagIsSignedInteger) {
		eSampleFormat = SampleShort;
	}

	chanmasks[0] = SPEAKER_FRONT_LEFT;
	chanmasks[1] = SPEAKER_FRONT_RIGHT;
	initializeMixer(chanmasks);

	if (eSampleFormat == SampleFloat) {
		fmt.mFormatFlags = kAudioFormatFlagIsFloat | kAudioFormatFlagIsPacked;
		fmt.mBitsPerChannel = sizeof(float) * 8;
	} else if (eSampleFormat == SampleShort) {
		fmt.mFormatFlags = kAudioFormatFlagIsSignedInteger | kAudioFormatFlagIsPacked;
		fmt.mBitsPerChannel = sizeof(short) * 8;
	}

	fmt.mFormatID = kAudioFormatLinearPCM;
	fmt.mSampleRate = iMixerFreq;
	fmt.mChannelsPerFrame = iChannels;
	fmt.mBytesPerFrame = iSampleSize;
	fmt.mBytesPerPacket = iSampleSize;
	fmt.mFramesPerPacket = 1;

	len = sizeof(AudioStreamBasicDescription);
	err = AudioUnitSetProperty(au, kAudioUnitProperty_StreamFormat, kAudioUnitScope_Input, 0, &fmt, len);
	if (err != noErr) {
		qWarning("CoreAudioOutput: Unable to set stream format for output device.");
		return;
	}

	err = AudioUnitAddPropertyListener(au, kAudioUnitProperty_StreamFormat, CoreAudioOutput::propertyChange, this);
	if (err != noErr) {
		qWarning("CoreAudioOutput: Unable to create output property change listener. Unable to listen to property change events.");
	}

	AURenderCallbackStruct cb;
	cb.inputProc = CoreAudioOutput::outputCallback;
	cb.inputProcRefCon = this;
	len = sizeof(AURenderCallbackStruct);
	err = AudioUnitSetProperty(au, kAudioUnitProperty_SetRenderCallback, kAudioUnitScope_Global, 0, &cb, len);
	if (err != noErr) {
		qWarning("CoreAudioOutput: Unable to setup callback.");
		return;
	}

	AudioValueRange range;
	len = sizeof(AudioValueRange);
	err = AudioDeviceGetProperty(devId, 0, false, kAudioDevicePropertyBufferFrameSizeRange, &len, &range);
	if (err != noErr) {
		qWarning("CoreAudioOutput: Unable to query for allowed buffer size ranges.");
	} else {
		qWarning("CoreAudioOutput: BufferFrameSizeRange = (%.2f, %.2f)", range.mMinimum, range.mMaximum);
	}

	UInt32 val = (iFrameSize * iMixerFreq) / SAMPLE_RATE;
	err = AudioDeviceSetProperty(devId, NULL, 0, false, kAudioDevicePropertyBufferFrameSize, sizeof(UInt32), &val);
	if (err != noErr) {
		qWarning("CoreAudioOutput: Could not set requested buffer size for device. Continuing with default.");
	}

	err = AudioOutputUnitStart(au);
	if (err != noErr) {
		qWarning("CoreAudioOutput: Unable to start AudioUnit");
		return;
	}

	bRunning = true;
}

CoreAudioOutput::~CoreAudioOutput() {
	OSStatus err;

	bRunning = false;
	wait();

	if (au) {
		err = AudioOutputUnitStop(au);
		if (err != noErr) {
			qWarning("CoreAudioOutput: Unable to stop AudioUnit.");
		}
	}

	qWarning("CoreAudioOutput: Shutting down.");
}

OSStatus CoreAudioOutput::outputCallback(void *udata, AudioUnitRenderActionFlags *flags, const AudioTimeStamp *ts,
        UInt32 busnum, UInt32 nframes, AudioBufferList *buflist) {
	Q_UNUSED(flags);
	Q_UNUSED(ts);
	Q_UNUSED(busnum);

	CoreAudioOutput *o = reinterpret_cast<CoreAudioOutput *>(udata);
	AudioBuffer *buf = buflist->mBuffers;

	bool done = o->mix(buf->mData, nframes);
	if (! done) {
		buf->mDataByteSize = 0;
		return -1;
	}

	return noErr;
}

void CoreAudioOutput::propertyChange(void *udata, AudioUnit au, AudioUnitPropertyID prop, AudioUnitScope scope, AudioUnitElement element) {
	Q_UNUSED(udata);
	Q_UNUSED(au);
	Q_UNUSED(scope);
	Q_UNUSED(element);

	if (prop == kAudioUnitProperty_StreamFormat) {
		qWarning("CoreAudioOuptut: Stream format change detected. Restarting AudioOutput.");
		Audio::stopOutput();
		Audio::startOutput();
	} else {
		qWarning("CoreAudioOutput: Unexpected property changed event received.");
	}
}

void CoreAudioOutput::run() {
}
