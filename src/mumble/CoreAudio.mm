// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include <AVFoundation/AVFoundation.h>
#include <IOKit/audio/IOAudioTypes.h>
#include <CoreAudio/AudioHardware.h>
#include "MainWindow.h"
#include "Global.h"

#include <exception>
#include <sstream>
#include "CoreAudio.h"

// Ignore deprecation warnings for the whole file, for now.
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"


namespace {
extern "C" {
// The dirty hack used to disable the "compulsory" ducking offered by VoiceProcessingAU.
// A popular variant of this hack is `printf "p *(char*)(void(*)())AudioDeviceDuck=0xc3\nq" | lldb -n FaceTime`
// See https://github.com/chromium/chromium/blob/6acb61fb1f335a720c51ed20acec5b3a4a19f308/media/audio/mac/audio_low_latency_input_mac.cc#L38
// for reference.
OSStatus AudioDeviceDuck(AudioDeviceID inDevice,
		     Float32 inDuckedLevel,
		     const AudioTimeStamp* __nullable inStartTime,
		     Float32 inRampDuration) __attribute__((weak_import));
}

void UndoDucking(AudioDeviceID output_device_id) {
    if (AudioDeviceDuck != nullptr) {
	    // Ramp the volume back up over half a second.
	    qDebug("CoreAudioInput: Undo Ducking caused by VoIP AU.");
	    AudioDeviceDuck(output_device_id, 1.0, nullptr, 0.5);
    }
}
}


namespace core_audio_utils {

class CoreAudioException : public std::exception {
private:
	QString msg_;
public:
	explicit CoreAudioException(const QString& msg) : msg_(msg) {}
	~CoreAudioException() = default;

	const char* what() const noexcept { return msg_.toUtf8().constData(); }
	QString getMessage() const noexcept { return msg_; }
};



CFStringRef QStringToCFString(const QString &str) {
	return CFStringCreateWithCharacters(kCFAllocatorDefault, reinterpret_cast< const UniChar * >(str.unicode()),
	                                    str.length());
}

QString GetDeviceStringProperty(AudioObjectID device_id, AudioObjectPropertySelector property_selector) {
	CFStringRef property_value = nullptr;
	UInt32 size = sizeof(property_value);
	AudioObjectPropertyAddress property_address = {
		property_selector,
		kAudioObjectPropertyScopeGlobal,
		kAudioObjectPropertyElementMaster
	};

	OSStatus result = AudioObjectGetPropertyData(
		device_id, &property_address, 0,
		nullptr, &size, &property_value);
	if (result != noErr) {
		throw CoreAudioException(QString("Unable to get string property %1 of %2.").arg(property_selector).arg(static_cast<int>(device_id)));
	}

	char buf[4096];
	CFStringGetCString(property_value, buf, 4096, kCFStringEncodingUTF8);
	return QString::fromUtf8(buf);
}

UInt32 GetDeviceUint32Property(AudioObjectID device_id, AudioObjectPropertySelector property_selector, AudioObjectPropertyScope property_scope) {
    AudioObjectPropertyAddress property_address = {
	    property_selector,
	    property_scope,
	    kAudioObjectPropertyElementMaster};
    UInt32 property_value;
    UInt32 size = sizeof(property_value);
    OSStatus result = AudioObjectGetPropertyData(device_id, &property_address, 0, nullptr, &size, &property_value);
    if (result != noErr) {
	    throw CoreAudioException(QString("Unable to get uint32 property %1 of %2.").arg(property_selector).arg(static_cast<int>(device_id)));
    }

    return property_value;
}

UInt32 GetDevicePropertySize(AudioObjectID device_id, AudioObjectPropertySelector property_selector, AudioObjectPropertyScope property_scope) {
    AudioObjectPropertyAddress property_address = {
	    property_selector,
	    property_scope,
	    kAudioObjectPropertyElementMaster};
    UInt32 size = 0;
    OSStatus result = AudioObjectGetPropertyDataSize(device_id, &property_address, 0, nullptr, &size);
    if (result != noErr) {
	    throw CoreAudioException(QString("Unable to get property size of %1 of %2.").arg(property_selector).arg(static_cast<int>(device_id)));
    }
    return size;
}

QVector<AudioObjectID> GetAudioObjectIDs(AudioObjectID audio_object_id, AudioObjectPropertySelector property_selector) {
	AudioObjectPropertyAddress property_address = {
		property_selector,
		kAudioObjectPropertyScopeGlobal,
		kAudioObjectPropertyElementMaster
	};

	UInt32 size = GetDevicePropertySize(audio_object_id, property_selector, kAudioObjectPropertyScopeGlobal);

	if (size == 0)
		return {};

	int device_count = size / sizeof(AudioObjectID);

	QVector<AudioObjectID> device_ids(device_count);
	OSStatus result = AudioObjectGetPropertyData(audio_object_id, &property_address, 0,
	                                             nullptr , &size, device_ids.data());
	if (result != noErr) {
		throw CoreAudioException(QString("Unable to get object ids from %1.").arg(audio_object_id));
		return {};
	}

	return device_ids;
}

AudioBufferList* GetDeviceStreamConfiguration(AudioObjectID device_id, AUDirection type) {
	AudioBufferList *bufs = nullptr;
	OSStatus err;
	auto scope = (type == AUDirection::INPUT) ? kAudioDevicePropertyScopeInput : kAudioDevicePropertyScopeOutput;
	unsigned int len = GetDevicePropertySize(
		device_id,
		kAudioDevicePropertyStreamConfiguration,
		scope);

	AudioObjectPropertyAddress propertyAddress = { kAudioHardwarePropertyDevices, scope,
	                                               kAudioObjectPropertyElementMaster };

	bufs                      = reinterpret_cast< AudioBufferList * >(malloc(len));
	propertyAddress.mSelector = kAudioDevicePropertyStreamConfiguration;
	err = AudioObjectGetPropertyData(device_id, &propertyAddress, 0, nullptr, &len, bufs);
	if (!bufs || err != noErr) {
		free(bufs);
		throw CoreAudioException(QString("Failed to get AudioStreamConfiguration from device %1.").arg(device_id));
	}

	return bufs;
}

AudioDeviceID GetDeviceID(const QString& devUid, AUDirection type) {
	OSStatus err;
	UInt32 len;
	CFStringRef csDevUid = QStringToCFString(devUid);
	AudioDeviceID devId;

	// Struct used to query information of the system audio setup
	AudioObjectPropertyAddress propertyAddress = {
		.mSelector = 0, // this attribute will be specified later
		.mScope = (type == AUDirection::INPUT) ? kAudioDevicePropertyScopeInput : kAudioDevicePropertyScopeOutput,
		.mElement = kAudioObjectPropertyElementMaster
	};

	AudioValueTranslation avt;
	avt.mInputData      = const_cast< struct __CFString ** >(&csDevUid);
	avt.mInputDataSize  = sizeof(CFStringRef *);
	avt.mOutputData     = &devId;
	avt.mOutputDataSize = sizeof(AudioDeviceID);

	len                       = sizeof(AudioValueTranslation);
	propertyAddress.mSelector = kAudioHardwarePropertyDeviceForUID;
	err = AudioObjectGetPropertyData(kAudioObjectSystemObject, &propertyAddress, 0, nullptr, &len, &avt);
	if (err != noErr) {
		throw CoreAudioException(QString("Unable to query AudioDeviceID of %1.").arg(devUid));
	}

	return devId;
}

AudioDeviceID GetDefaultDeviceID(AUDirection type) {
	OSStatus err;
	UInt32 len;
	AudioDeviceID devId;

	// Struct used to query information of the system audio setup
	AudioObjectPropertyAddress propertyAddress = {
		.mSelector = 0, // this attribute will be specified later
		.mScope = (type == AUDirection::INPUT) ? kAudioDevicePropertyScopeInput : kAudioDevicePropertyScopeOutput,
		.mElement = kAudioObjectPropertyElementMaster
	};

	len                       = sizeof(AudioDeviceID);
	propertyAddress.mSelector = (type == AUDirection::INPUT) ? kAudioHardwarePropertyDefaultInputDevice : kAudioHardwarePropertyDefaultOutputDevice;
	err = AudioObjectGetPropertyData(kAudioObjectSystemObject, &propertyAddress, 0, nullptr, &len, &devId);
	if (err != noErr) {
		throw CoreAudioException("Unable to query for default AudioDeviceID.");
	}

    return devId;
}

UInt32 GetDeviceTransportType(AudioObjectID devId){
	return core_audio_utils::GetDeviceUint32Property(devId,
	                                                 kAudioDevicePropertyTransportType,
	                                                 kAudioObjectPropertyScopeGlobal);
}

bool IsInputDevice(AudioObjectID device_id) {
	// This part of the code is modified from Chromium. Original comments are kept.
	// See https://github.com/chromium/chromium/blob/6acb61fb1f335a720c51ed20acec5b3a4a19f308/media/audio/mac/core_audio_util_mac.cc
	// for reference.
	QVector<AudioObjectID> streams = GetAudioObjectIDs(device_id, kAudioDevicePropertyStreams);

	int num_undefined_input_streams = 0;
	int num_defined_input_streams = 0;
	int num_output_streams = 0;

	for (auto stream_id : streams) {
		auto direction = GetDeviceUint32Property(stream_id, kAudioStreamPropertyDirection,
		                                         kAudioObjectPropertyScopeGlobal);
		const UInt32 kDirectionOutput = 0;
		const UInt32 kDirectionInput = 1;
		if (direction == kDirectionOutput) {
			++num_output_streams;
		} else if (direction == kDirectionInput) {
			// Filter input streams based on what terminal it claims to be attached
			// to. Note that INPUT_UNDEFINED comes from a set of terminals declared
			// in IOKit. CoreAudio defines a number of terminals in
			// AudioHardwareBase.h but none of them match any of the values I've
			// seen used in practice, though I've only tested a few devices.
			auto terminal = GetDeviceUint32Property(stream_id, kAudioStreamPropertyTerminalType,
			                                        kAudioObjectPropertyScopeGlobal);
			if (terminal == INPUT_UNDEFINED) {
				++num_undefined_input_streams;
			} else {
				++num_defined_input_streams;
			}
		}
	}

	// I've only seen INPUT_UNDEFINED introduced by the VoiceProcessing AudioUnit,
	// but to err on the side of caution, let's allow a device with only undefined
	// input streams and no output streams as well.
	return num_defined_input_streams > 0 ||
	       (num_undefined_input_streams > 0 && num_output_streams == 0);
}

bool IsOutputDevice(AudioObjectID device_id) {
    QVector<AudioObjectID> streams = GetAudioObjectIDs(device_id, kAudioDevicePropertyStreams);

    for (auto stream_id : streams) {
	    auto direction = GetDeviceUint32Property(stream_id, kAudioStreamPropertyDirection,
						     kAudioObjectPropertyScopeGlobal);
	    const UInt32 kDirectionOutput = 0;
	    if (direction == kDirectionOutput) {
	    	return true;
	    }
    }
    return false;
}


#ifdef DEBUG

static void LogStreamDescription(AudioStreamBasicDescription format) {
    std::stringstream ss;
    unsigned char formatID[5];
    *(UInt32 *) formatID = OSSwapHostToBigInt32(format.mFormatID);
    formatID[4] = '\0';

    // General description
    ss << format.mChannelsPerFrame << " ch, " << format.mSampleRate << " Hz, '" << formatID << "' (0x"
       << std::hex << std::setw(8) << std::setfill('0') << format.mFormatFlags << std::dec << ") ";

    if (kAudioFormatLinearPCM == format.mFormatID) {
	    // Bit depth
	    UInt32 fractionalBits =
		    ((0x3f << 7)/*kLinearPCMFormatFlagsSampleFractionMask*/ & format.mFormatFlags)
			    >> 7/*kLinearPCMFormatFlagsSampleFractionShift*/;
	    if (0 < fractionalBits)
		    ss << (format.mBitsPerChannel - fractionalBits) << "." << fractionalBits;
	    else
		    ss << format.mBitsPerChannel;

	    ss << "-bit";

	    // Endianness
	    bool isInterleaved = !(kAudioFormatFlagIsNonInterleaved & format.mFormatFlags);
	    UInt32 interleavedChannelCount = (isInterleaved ? format.mChannelsPerFrame : 1);
	    UInt32 sampleSize = (0 < format.mBytesPerFrame && 0 < interleavedChannelCount ?
				 format.mBytesPerFrame / interleavedChannelCount : 0);
	    if (1 < sampleSize)
		    ss << ((kLinearPCMFormatFlagIsBigEndian & format.mFormatFlags) ? " big-endian"
										   : " little-endian");

	    // Sign
	    bool isInteger = !(kLinearPCMFormatFlagIsFloat & format.mFormatFlags);
	    if (isInteger)
		    ss << ((kLinearPCMFormatFlagIsSignedInteger & format.mFormatFlags) ? " signed"
										       : " unsigned");

	    // Integer or floating
	    ss << (isInteger ? " integer" : " float");

	    // Packedness
	    if (0 < sampleSize && ((sampleSize << 3) != format.mBitsPerChannel))
		    ss << ((kLinearPCMFormatFlagIsPacked & format.mFormatFlags) ? ", packed in "
										: ", unpacked in ")
		       << sampleSize << " bytes";

	    // Alignment
	    if ((0 < sampleSize && ((sampleSize << 3) != format.mBitsPerChannel)) ||
		(0 != (format.mBitsPerChannel & 7)))
		    ss << ((kLinearPCMFormatFlagIsAlignedHigh & format.mFormatFlags) ? " high-aligned"
										     : " low-aligned");

	    if (!isInterleaved)
		    ss << ", deinterleaved";
    } else if (kAudioFormatAppleLossless == format.mFormatID) {
	    UInt32 sourceBitDepth = 0;
	    switch (format.mFormatFlags) {
		    case kAppleLosslessFormatFlag_16BitSourceData:
			    sourceBitDepth = 16;
			    break;
		    case kAppleLosslessFormatFlag_20BitSourceData:
			    sourceBitDepth = 20;
			    break;
		    case kAppleLosslessFormatFlag_24BitSourceData:
			    sourceBitDepth = 24;
			    break;
		    case kAppleLosslessFormatFlag_32BitSourceData:
			    sourceBitDepth = 32;
			    break;
	    }

	    if (0 != sourceBitDepth)
		    ss << "from " << sourceBitDepth << "-bit source, ";
	    else
		    ss << "from UNKNOWN source bit depth, ";

	    ss << format.mFramesPerPacket << " frames/packet";
    } else
	    ss << format.mBitsPerChannel << " bits/channel, " << format.mBytesPerPacket << " bytes/packet, "
	       << format.mFramesPerPacket << " frames/packet, " << format.mBytesPerFrame << " bytes/frame";

    qDebug("CoreAudioStreamDescription: %s", ss.str().c_str());
}

static void LogAUStreamDescription(AudioUnit au) {
    AudioStreamBasicDescription description;
    UInt32 len = sizeof(AudioStreamBasicDescription);
    OSStatus err;
    err = AudioUnitGetProperty(au, kAudioUnitProperty_StreamFormat, kAudioUnitScope_Input, 1, &description,
			       &len);
    if (err == noErr) {
	    LogStreamDescription(description);
    }
}

#endif
};

#define CHECK_RETURN_FALSE(statement, warning_msg) \
{ \
	OSStatus _err = statement; \
	if (_err != noErr) { \
		qWarning(warning_msg); \
		return false; \
	} \
} \

#define CHECK_RETURN(statement, warning_msg) \
{ \
	OSStatus _err = statement; \
	if (_err != noErr) { \
		qWarning(warning_msg); \
		return; \
	} \
} \

#define CHECK_WARN(statement, warning_msg) \
{ \
	OSStatus _err = statement; \
	if (_err != noErr) { \
		qWarning(warning_msg); \
	} \
} \

class CoreAudioInit : public DeferInit {
	CoreAudioInputRegistrar *cairReg;
	CoreAudioOutputRegistrar *caorReg;

public:
	CoreAudioInit() : cairReg(nullptr), caorReg(nullptr) {}
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

const QList< audioDevice > CoreAudioSystem::getDeviceChoices(bool input) {
	const bool doEcho = (Global::get().s.echoOption == EchoCancelOptionID::APPLE_AEC);
	const QHash< QString, QString > devices = CoreAudioSystem::getDevices(input, doEcho);

	QList< audioDevice > choices = {
		audioDevice(QObject::tr("Default Device"), QString())
	};

	for (auto &key : devices.keys()) {
		choices << audioDevice(devices.value(key), key);
	}

	return choices;
}

const QHash< QString, QString > CoreAudioSystem::getDevices(bool input, bool echo) {
	QHash< QString, QString > qhReturn;
	try {
		QVector<AudioObjectID> devs = core_audio_utils::GetAudioObjectIDs(kAudioObjectSystemObject,
		                                                                  kAudioHardwarePropertyDevices);

		for (AudioObjectID devid : devs) {

			// Mac's native echo cancellation doesn't support aggregate device
			// since it will create aggregate devices itself.
			if (echo && core_audio_utils::GetDeviceTransportType(devid) == kAudioDeviceTransportTypeAggregate) {
				continue;
			}

			if ((input && !core_audio_utils::IsInputDevice(devid))
			    || (!input && !core_audio_utils::IsOutputDevice(devid))) continue;

			QString qsDeviceName = core_audio_utils::GetDeviceStringProperty(devid, kAudioDevicePropertyDeviceNameCFString);
			QString qsDeviceIdentifier = core_audio_utils::GetDeviceStringProperty(devid, kAudioDevicePropertyDeviceUID);

			qhReturn.insert(qsDeviceIdentifier, qsDeviceName);
		}
	} catch (core_audio_utils::CoreAudioException& e) {
		qWarning() << "CoreAudioSystem: " << e.what();
	}

	return qhReturn;
}

CoreAudioInputRegistrar::CoreAudioInputRegistrar() : AudioInputRegistrar(QLatin1String("CoreAudio"), 10) {
	echoOptions.push_back(EchoCancelOptionID::APPLE_AEC);
}

AudioInput *CoreAudioInputRegistrar::create() {
	if (!isMicrophoneAccessDeniedByOS()) {
		return new CoreAudioInput();
	} else {
		return nullptr;
	}
}

const QVariant CoreAudioInputRegistrar::getDeviceChoice() {
	return Global::get().s.qsCoreAudioInput;
}

const QList< audioDevice > CoreAudioInputRegistrar::getDeviceChoices() {
	return CoreAudioSystem::getDeviceChoices(true);
}

void CoreAudioInputRegistrar::setDeviceChoice(const QVariant &choice, Settings &s) {
	s.qsCoreAudioInput = choice.toString();
}

bool CoreAudioInputRegistrar::canEcho(EchoCancelOptionID echoCancelID, const QString &outputSystem) const {
	Q_UNUSED(outputSystem)

	if (@available(macOS 10.14, *)) {
		if (echoCancelID == EchoCancelOptionID::APPLE_AEC) return true;
	}
	return false;
}

bool CoreAudioInputRegistrar::isMicrophoneAccessDeniedByOS() {
	// Only available after macOS 10.14
	// See https://developer.apple.com/documentation/avfoundation/cameras_and_media_capture/
	// requesting_authorization_for_media_capture_on_macos?language=objc
	if (@available(macOS 10.14, *)){
		qDebug("CoreAudioInput: Checking microphone permission....");
		// Request permission to access the camera and microphone.
		switch ([AVCaptureDevice authorizationStatusForMediaType:AVMediaTypeAudio])
		{
			case AVAuthorizationStatusAuthorized: {
				// The user has previously granted access to the camera.
				qDebug("CoreAudioInput: Checking microphone permission passed.");
				return false;
			}
			case AVAuthorizationStatusNotDetermined: {
				// The app hasn't yet asked the user for microphone access.
				qWarning("CoreAudioInput: Mumble hasn't asked the user for microphone access. Asking for it now.");
				[AVCaptureDevice requestAccessForMediaType:AVMediaTypeAudio completionHandler: ^(BOOL _granted) {
					if (_granted) {
						Audio::stopInput();
						Audio::startInput();
					} else {
						qWarning("CoreAudioInput: Microphone access denied by user.");
					}
				}];
				return true;
			}
			case AVAuthorizationStatusDenied: {
				// The user has previously denied access.
				qWarning("CoreAudioInput: Microphone access has been previously denied by user.");
				Global::get().mw->msgBox(QObject::tr("Access to the microphone was denied. Please allow Mumble to use the microphone "
				                         "by changing the settings in System Preferences -> Security & Privacy -> Privacy -> "
				                         "Microphone."));
				return true;
			}
			case AVAuthorizationStatusRestricted: {
				// The user can't grant access due to restrictions.
				qWarning("CoreAudioInput: Microphone access denied due to system restrictions.");
				Global::get().mw->msgBox(QObject::tr("Access to the microphone was denied due to system restrictions. You will not be able "
				                         "to use the microphone in this session."));
				return true;
			}
			default: {
				return true;
			}
		}
	} else {
		return false;
	}
}

AudioOutput *CoreAudioOutputRegistrar::create() {
	return new CoreAudioOutput();
}

const QVariant CoreAudioOutputRegistrar::getDeviceChoice() {
	return Global::get().s.qsCoreAudioOutput;
}

const QList< audioDevice > CoreAudioOutputRegistrar::getDeviceChoices() {
	return CoreAudioSystem::getDeviceChoices(false);
}

void CoreAudioOutputRegistrar::setDeviceChoice(const QVariant &choice, Settings &s) {
	s.qsCoreAudioOutput = choice.toString();
}

bool CoreAudioOutputRegistrar::canMuteOthers() const {
	return false;
}

CoreAudioInput::CoreAudioInput() {
}

bool CoreAudioInput::openAUHAL(AudioStreamBasicDescription &streamDescription){
	AudioComponent comp;
	AudioComponentDescription desc;
	UInt32 val, len;

	desc.componentType         = kAudioUnitType_Output;
	desc.componentSubType      = kAudioUnitSubType_HALOutput;
	desc.componentManufacturer = kAudioUnitManufacturer_Apple;
	desc.componentFlags        = 0;
	desc.componentFlagsMask    = 0;

	qDebug("CoreAudioInput: Use AUHAL as IO AudioUnit.");

	comp = AudioComponentFindNext(nullptr, &desc);
	if (!comp) {
		qWarning("CoreAudioInput: Unable to find AUHAL.");
		return false;
	}

	CHECK_RETURN_FALSE(AudioComponentInstanceNew(comp, &auHAL), "CoreAudioInput: Unable to open AUHAL component.");

	val = 1;
	CHECK_RETURN_FALSE(AudioUnitSetProperty(auHAL, kAudioOutputUnitProperty_EnableIO, kAudioUnitScope_Input,
	                                        AUDirection::INPUT, &val, sizeof(UInt32)),
	                   "CoreAudioInput: Unable to configure input scope on AUHAL.");

	val = 0;
	CHECK_RETURN_FALSE(AudioUnitSetProperty(auHAL, kAudioOutputUnitProperty_EnableIO, kAudioUnitScope_Output,
	                                        AUDirection::OUTPUT, &val, sizeof(UInt32)),
	                   "CoreAudioInput: Unable to configure output scope on AUHAL.");

	len = sizeof(AudioDeviceID);
	CHECK_RETURN_FALSE(AudioUnitSetProperty(auHAL, kAudioOutputUnitProperty_CurrentDevice, kAudioUnitScope_Global,
	                                        AUDirection::OUTPUT, &inputDevId, len),
	                   "CoreAudioInput: Unable to set device of AUHAL.");

	len = sizeof(AudioStreamBasicDescription);
	CHECK_RETURN_FALSE(AudioUnitGetProperty(auHAL, kAudioUnitProperty_StreamFormat, kAudioUnitScope_Input,
	                                        AUDirection::INPUT, &streamDescription, &len),
	                   "CoreAudioInput: Unable to query device for stream info.");

	return true;
}

bool CoreAudioInput::openAUVoip(AudioStreamBasicDescription &streamDescription) {
	// Initialize VoiceProcessingIO AU, utilizing macOS's builtin echo cancellation.
	// This AU is poorly documented by Apple. See Chromium's code for more information:
	// https://github.com/chromium/chromium/blob/master/media/audio/mac/audio_low_latency_input_mac.cc
	if(@available(macOS 10.12, *)) {
		UInt32 len, val;
		AudioComponentDescription desc = {
				.componentType = kAudioUnitType_Output,
				.componentSubType = kAudioUnitSubType_VoiceProcessingIO,
				.componentManufacturer = kAudioUnitManufacturer_Apple,
				.componentFlags = 0,
				.componentFlagsMask = 0
		};

		qDebug("CoreAudioInput: Use VoiceProcessingIO as IO AudioUnit.");

		AudioComponent inputComponent = AudioComponentFindNext(nullptr, &desc);

		CHECK_RETURN_FALSE(AudioComponentInstanceNew(inputComponent, &auVoip),
		                   "CoreAudioInput: Unable to create VoiceProcessingIO AudioUnit.");

		try {
			if (core_audio_utils::GetDeviceTransportType(inputDevId) == kAudioDeviceTransportTypeAggregate) {
				qWarning("CoreAudioInput: Aggregated devices are not supported by VoiceProcessingIO AudioUnit.");
				return false;
			}
		} catch (core_audio_utils::CoreAudioException& e) {
			qWarning() << "CoreAudioInput: " << e.what();
			return false;
		}

		len = sizeof(AudioDeviceID);
		CHECK_RETURN_FALSE(AudioUnitSetProperty(auVoip, kAudioOutputUnitProperty_CurrentDevice, kAudioUnitScope_Global,
		                                        AUDirection::INPUT, &inputDevId, len),
		                   "CoreAudioInput: Unable to set device of VoiceProcessingIO AudioUnit.");

		// It is reported that the echo source need to be specified as the output device.
		// If no output device is specified, MacOS would take the default output device as echo source.
		CHECK_RETURN_FALSE(AudioUnitSetProperty(auVoip, kAudioOutputUnitProperty_CurrentDevice, kAudioUnitScope_Global,
		                                        AUDirection::OUTPUT, &echoOutputDevId, len),
		                   "CoreAudioInput: Unable to set device of VoiceProcessingIO AudioUnit.");


		len = sizeof(AudioStreamBasicDescription);
		CHECK_RETURN_FALSE(AudioUnitGetProperty(auVoip, kAudioUnitProperty_StreamFormat, kAudioUnitScope_Input,
		                                        AUDirection::INPUT, &streamDescription, &len),
		                   "CoreAudioInput: Unable to query device for stream info from VoiceProcessingIO AudioUnit.");

#ifdef DEBUG
		qDebug("CoreAudioInput: VOIP Input stream description:");
		core_audio_utils::LogAUStreamDescription(auVoip);
#endif

		// Mute the VoiceProcessing AU (Value 0 stands for "mute")
		// VoiceProcessing AU is a output node and has the ability of playing things out. We simply don't want that.
		val = 0;
		len = sizeof(val);
		AudioUnitSetProperty(auVoip, kAUVoiceIOProperty_MuteOutput, kAudioUnitScope_Global, 1, &val, len);

		return true;
	} else {
		return false;
	}
}



bool CoreAudioInput::initializeInputAU(AudioUnit au, AudioStreamBasicDescription &streamDescription, int &actualBufferLength) {
	OSStatus err;
	UInt32 len, val;

	len = sizeof(AudioStreamBasicDescription);
	CHECK_RETURN_FALSE(AudioUnitSetProperty(au, kAudioUnitProperty_StreamFormat, kAudioUnitScope_Output,
	                                        AUDirection::INPUT, &streamDescription, len),
	                   "CoreAudioInput: Unable to set stream format for Input AU - 1.")

	CHECK_RETURN_FALSE(AudioUnitSetProperty(au, kAudioUnitProperty_StreamFormat, kAudioUnitScope_Input,
	                                        AUDirection::OUTPUT, &streamDescription, len),
	                   "CoreAudioInput: Unable to set stream format for Input AU - 2.");
#ifdef DEBUG
	qDebug("CoreAudioInput: Input stream description:");
	core_audio_utils::LogAUStreamDescription(au);
#endif

	// Struct used to query information of the system audio setup
	AudioObjectPropertyAddress propertyAddress = {
		.mSelector = 0, // this attribute will be specified later
		.mScope = kAudioDevicePropertyScopeInput,
		.mElement = kAudioObjectPropertyElementMaster
	};

	AudioValueRange range;
	len                       = sizeof(AudioValueRange);
	propertyAddress.mSelector = kAudioDevicePropertyBufferFrameSizeRange;
	CHECK_WARN(AudioObjectGetPropertyData(inputDevId, &propertyAddress, 0, nullptr, &len, &range),
	           "CoreAudioInput: Unable to query for allowed buffer size ranges of input device.");

	qWarning("CoreAudioInput: BufferFrameSizeRange = (%.2f, %.2f)", range.mMinimum, range.mMaximum);

	actualBufferLength        = static_cast<int>(iMicLength);
	val                       = iMicLength;
	propertyAddress.mSelector = kAudioDevicePropertyBufferFrameSize;
	err                       = AudioObjectSetPropertyData(inputDevId, &propertyAddress, 0, nullptr, sizeof(UInt32), &val);
	if (err != noErr) {
		qWarning("CoreAudioInput: Unable to set preferred buffer size on device. Querying for device default.");
		len = sizeof(UInt32);
		CHECK_RETURN_FALSE(AudioDeviceGetProperty(inputDevId, 0, true, kAudioDevicePropertyBufferFrameSize, &len, &val),
		                   "CoreAudioInput: Unable to query for device's buffer size.");

		actualBufferLength = (int) val;
	}

	CHECK_RETURN_FALSE(AudioUnitInitialize(au), "CoreAudioInput: Unable to initialize VoiceProcessingIO AudioUnit.");

	return true;
}

void CoreAudioInput::run() {
	OSStatus err;
	UInt32 len;
	AudioStreamBasicDescription fmt;
	inputDevId = 0;
	echoOutputDevId = 0;
	bool doEcho = (Global::get().s.echoOption == EchoCancelOptionID::APPLE_AEC);

	auHAL = nullptr;
	auVoip = nullptr;

	memset(&buflist, 0, sizeof(AudioBufferList));

	try {
		if (!Global::get().s.qsCoreAudioInput.isEmpty()) {
			qWarning("CoreAudioInput: Set device to '%s'.", qPrintable(Global::get().s.qsCoreAudioInput));
			inputDevId = core_audio_utils::GetDeviceID(Global::get().s.qsCoreAudioInput, AUDirection::INPUT);
		} else {
			qWarning("CoreAudioInput: Set device to 'Default Device'.");
			inputDevId = core_audio_utils::GetDefaultDeviceID(AUDirection::INPUT);
		}

		if (doEcho) {
			echoOutputDevId = core_audio_utils::GetDeviceID(Global::get().s.qsCoreAudioOutput, AUDirection::OUTPUT);
			if (!openAUVoip(fmt)) { return; };
		} else {
			if (!openAUHAL(fmt)) { return; };
		}
	} catch (core_audio_utils::CoreAudioException& e) {
		qWarning() << "CoreAudioInput: " << e.getMessage();
	}

#ifdef DEBUG
	qDebug("CoreAudioInput: Original input stream description:");
	core_audio_utils::LogStreamDescription(fmt);
#endif

	if (fmt.mFormatFlags & kAudioFormatFlagIsFloat) {
		eMicFormat = SampleFloat;
	} else if (fmt.mFormatFlags & kAudioFormatFlagIsSignedInteger) {
		eMicFormat = SampleShort;
	}

	if (fmt.mChannelsPerFrame > 1) {
		qWarning("CoreAudioInput: Input device with more than one channel detected. Defaulting to 1.");
	}

	iMicFreq     = static_cast<unsigned int>(fmt.mSampleRate);
	iMicChannels = 1;
	initializeMixer();

	if (eMicFormat == SampleFloat) {
		fmt.mFormatFlags    = kAudioFormatFlagIsFloat | kAudioFormatFlagIsPacked;
		fmt.mBitsPerChannel = sizeof(float) * 8;
	} else if (eMicFormat == SampleShort) {
		fmt.mFormatFlags    = kAudioFormatFlagIsSignedInteger | kAudioFormatFlagIsPacked;
		fmt.mBitsPerChannel = sizeof(short) * 8;
	}

	AudioUnit auFinal = auHAL;

	fmt.mFormatID         = kAudioFormatLinearPCM;
	fmt.mSampleRate       = iMicFreq;
	fmt.mChannelsPerFrame = iMicChannels;
	fmt.mBytesPerFrame    = iMicSampleSize;
	fmt.mBytesPerPacket   = iMicSampleSize;
	fmt.mFramesPerPacket  = 1;

	int actualBufferLength;

	if (doEcho) {
		// Initialize macOS's builtin echo cancellation AU.
		if(initializeInputAU(auVoip, fmt, actualBufferLength)) {
			auFinal = auVoip;
		} else {
			qWarning("CoreAudioInput: Unable to initialize VoiceProcessing AU for echo cancellation.");
			return;
		}
	} else {
		if(!initializeInputAU(auHAL, fmt, actualBufferLength)) {
			return;
		};
	}

	AURenderCallbackStruct cb;
	cb.inputProc       = CoreAudioInput::inputCallback;
	cb.inputProcRefCon = this;
	len                = sizeof(AURenderCallbackStruct);
	CHECK_RETURN(AudioUnitSetProperty(auFinal, kAudioOutputUnitProperty_SetInputCallback, kAudioUnitScope_Global, 0, &cb, len),
	             "CoreAudioInput: Unable to setup input callback.");

	err = AudioUnitAddPropertyListener(auFinal, kAudioUnitProperty_StreamFormat, CoreAudioInput::propertyChange, this);
	if (err != noErr) {
		qWarning("CoreAudioInput: Unable to create input property change listener for AUHAL. Unable to listen to property change "
				 "events.");
	}

	AudioObjectPropertyAddress inputDeviceAddress = {
		kAudioHardwarePropertyDefaultInputDevice,
		kAudioObjectPropertyScopeGlobal,
		kAudioObjectPropertyElementMaster
	};
	CHECK_WARN(AudioObjectAddPropertyListener(kAudioObjectSystemObject, &inputDeviceAddress, CoreAudioInput::deviceChange, this),
			   "CoreAudioInput: Unable to create input device change listener. Unable to listen to device changes.");

	buflist.mNumberBuffers = 1;
	AudioBuffer *b         = buflist.mBuffers;
	b->mNumberChannels     = iMicChannels;
	b->mDataByteSize       = iMicSampleSize * static_cast<unsigned int>(actualBufferLength);
	b->mData               = calloc(1, b->mDataByteSize);

	// Start!
	CHECK_RETURN(AudioOutputUnitStart(auFinal), "CoreAudioInput: Unable to start AudioUnit.");

	if (doEcho) {
		UndoDucking(echoOutputDevId);
	}

	bRunning = true;
}

void CoreAudioInput::stop() {
	bRunning = false;

	if (auHAL) {
		CHECK_WARN(AudioOutputUnitStop(auHAL),
		           "CoreAudioInput: Unable to stop AudioUnit.");
		CHECK_WARN(AudioUnitUninitialize(auHAL),
		           "CoreAudioInput: Unable to uninitialize AudioUnit.");
		auHAL = nullptr;
	}

	if (auVoip) {
		CHECK_WARN(AudioOutputUnitStop(auVoip),
		           "CoreAudioInput: Unable to stop AudioUnit.");
		CHECK_WARN(AudioUnitUninitialize(auVoip),
		           "CoreAudioInput: Unable to uninitialize AudioUnit.");
		CHECK_WARN(AudioComponentInstanceDispose(auVoip),
		           "CoreAudioInput: Unable to dispose AudioUnit.");
		auVoip = nullptr;
	}

	AudioBuffer *b = buflist.mBuffers;
	if (b && b->mData)
		free(b->mData);

	qWarning("CoreAudioInput: Shutting down.");
}

CoreAudioInput::~CoreAudioInput() {
	bRunning = false;
	wait();
	stop();
}

OSStatus CoreAudioInput::inputCallback(void *udata, AudioUnitRenderActionFlags *flags, const AudioTimeStamp *ts,
                                       UInt32 busnum, UInt32 nframes, AudioBufferList *buflist) {
	Q_UNUSED(udata);
	Q_UNUSED(buflist);

	CoreAudioInput *i = reinterpret_cast< CoreAudioInput * >(udata);
	OSStatus err;

	if (i->auVoip) {
		err = AudioUnitRender(i->auVoip, flags, ts, busnum, nframes, &i->buflist);
	} else {
		err = AudioUnitRender(i->auHAL, flags, ts, busnum, nframes, &i->buflist);
	}
	if (err != noErr) {
		qWarning("CoreAudioInput: AudioUnitRender failed.");
		return err;
	}

	i->addMic(i->buflist.mBuffers->mData, nframes);

	return noErr;
}

void CoreAudioInput::propertyChange(void *udata, AudioUnit auHAL, AudioUnitPropertyID prop, AudioUnitScope scope,
									AudioUnitElement element) {
	Q_UNUSED(udata);
	Q_UNUSED(auHAL);
	Q_UNUSED(scope);
	Q_UNUSED(element);

	CoreAudioInput *o = reinterpret_cast< CoreAudioInput * >(udata);
	if (!o->bRunning) { return; }
	if (prop == kAudioUnitProperty_StreamFormat) {
		qWarning("CoreAudioInput: Stream format change detected. Restarting AudioInput.");
		Audio::stopInput();
		Audio::startInput();
	} else {
		qWarning("CoreAudioInput: Unexpected property changed event received.");
	}
}

OSStatus CoreAudioInput::deviceChange(AudioObjectID inObjectID, UInt32 inNumberAddresses,
									  const AudioObjectPropertyAddress inAddresses[], void *udata) {
	Q_UNUSED(inObjectID);
	Q_UNUSED(inNumberAddresses);
	Q_UNUSED(inAddresses);

	CoreAudioInput *o = reinterpret_cast< CoreAudioInput * >(udata);
	if (!o->bRunning) return noErr;

	qWarning("CoreAudioInput: Input device change detected. Restarting AudioInput.");
	Audio::stopInput();
	Audio::startInput();

	return noErr;
}


CoreAudioOutput::CoreAudioOutput() {
}


void CoreAudioOutput::run() {
	OSStatus err;
	AudioStreamBasicDescription fmt;
	unsigned int chanmasks[32];
	AudioDeviceID devId = 0;
	UInt32 len;
	AudioObjectPropertyAddress propertyAddress = { 0, kAudioDevicePropertyScopeOutput,
	                                               kAudioObjectPropertyElementMaster };

	try {
		if (!Global::get().s.qsCoreAudioOutput.isEmpty()) {
			qWarning("CoreAudioOutput: Set device to '%s'.", qPrintable(Global::get().s.qsCoreAudioOutput));

			devId = core_audio_utils::GetDeviceID(Global::get().s.qsCoreAudioOutput, AUDirection::OUTPUT);
		} else {
			qWarning("CoreAudioOutput: Set device to 'Default Device'.");

			devId = core_audio_utils::GetDefaultDeviceID(AUDirection::OUTPUT);
		}
	} catch (core_audio_utils::CoreAudioException& e) {
		qWarning() << "CoreAudioOutput: " << e.what();
	}

	AudioComponent comp;
	AudioComponentDescription desc;

	desc.componentType         = kAudioUnitType_Output;
	desc.componentSubType      = kAudioUnitSubType_HALOutput;
	desc.componentManufacturer = kAudioUnitManufacturer_Apple;
	desc.componentFlags        = 0;
	desc.componentFlagsMask    = 0;

	comp = AudioComponentFindNext(nullptr, &desc);
	if (!comp) {
		qWarning("CoreAudioOuput: Unable to find AudioUnit.");
		return;
	}

	CHECK_RETURN(AudioComponentInstanceNew(comp, &auHAL),
	             "CoreAudioOutput: Unable to open AudioUnit component.");

	CHECK_RETURN(AudioUnitInitialize(auHAL),
	             "CoreAudioOutput: Unable to initialize output AudioUnit");

	len = sizeof(AudioDeviceID);
	CHECK_RETURN(AudioUnitSetProperty(auHAL, kAudioOutputUnitProperty_CurrentDevice, kAudioUnitScope_Global, 0, &devId, len),
	             "CoreAudioOutput: Unable to set CurrentDevice property on AudioUnit.");

	len = sizeof(AudioStreamBasicDescription);
	CHECK_RETURN(AudioUnitGetProperty(auHAL, kAudioUnitProperty_StreamFormat, kAudioUnitScope_Output, 0, &fmt, &len),
	             "CoreAudioOuptut: Unable to query device for stream info.");

#ifdef DEBUG
	qDebug("CoreAudioOutput: Original output stream description:");
	core_audio_utils::LogAUStreamDescription(auHAL);
#endif

	iMixerFreq = static_cast< unsigned int >(fmt.mSampleRate);
	iChannels  = static_cast< unsigned int >(fmt.mChannelsPerFrame);

	if (fmt.mFormatFlags & kAudioFormatFlagIsFloat) {
		eSampleFormat = SampleFloat;
	} else if (fmt.mFormatFlags & kAudioFormatFlagIsSignedInteger) {
		eSampleFormat = SampleShort;
	}

	chanmasks[0] = SPEAKER_FRONT_LEFT;
	chanmasks[1] = SPEAKER_FRONT_RIGHT;
	initializeMixer(chanmasks);

	if (eSampleFormat == SampleFloat) {
		fmt.mFormatFlags    = kAudioFormatFlagIsFloat | kAudioFormatFlagIsPacked;
		fmt.mBitsPerChannel = sizeof(float) * 8;
	} else if (eSampleFormat == SampleShort) {
		fmt.mFormatFlags    = kAudioFormatFlagIsSignedInteger | kAudioFormatFlagIsPacked;
		fmt.mBitsPerChannel = sizeof(short) * 8;
	}

	fmt.mFormatID         = kAudioFormatLinearPCM;
	fmt.mSampleRate       = iMixerFreq;
	fmt.mChannelsPerFrame = iChannels;
	fmt.mBytesPerFrame    = iSampleSize;
	fmt.mBytesPerPacket   = iSampleSize;
	fmt.mFramesPerPacket  = 1;

	len = sizeof(AudioStreamBasicDescription);
	CHECK_RETURN(AudioUnitSetProperty(auHAL, kAudioUnitProperty_StreamFormat, kAudioUnitScope_Input, 0, &fmt, len),
	             "CoreAudioOutput: Unable to set stream format for output device.");

#ifdef DEBUG
	qDebug("CoreAudioOutput: Finalized output stream description:");
	core_audio_utils::LogAUStreamDescription(auHAL);
#endif

	CHECK_WARN(AudioUnitAddPropertyListener(auHAL, kAudioUnitProperty_StreamFormat, CoreAudioOutput::propertyChange, this),
	           "CoreAudioOutput: Unable to create output property change listener. Unable to listen to property changes.");

	AudioObjectPropertyAddress outputDeviceAddress = {
		kAudioHardwarePropertyDefaultOutputDevice,
		kAudioObjectPropertyScopeGlobal,
		kAudioObjectPropertyElementMaster
	};
	CHECK_WARN(AudioObjectAddPropertyListener(kAudioObjectSystemObject, &outputDeviceAddress, CoreAudioOutput::deviceChange, this),
			   "CoreAudioOutput: Unable to create output device change listener. Unable to listen to device changes.");

	AURenderCallbackStruct cb;
	cb.inputProc       = CoreAudioOutput::outputCallback;
	cb.inputProcRefCon = this;
	len                = sizeof(AURenderCallbackStruct);
	CHECK_RETURN(AudioUnitSetProperty(auHAL, kAudioUnitProperty_SetRenderCallback, kAudioUnitScope_Global, 0, &cb, len),
	             "CoreAudioOutput: Unable to setup callback.");

	AudioValueRange range;
	len                       = sizeof(AudioValueRange);
	propertyAddress.mSelector = kAudioDevicePropertyBufferFrameSizeRange;
	err                       = AudioObjectGetPropertyData(devId, &propertyAddress, 0, nullptr, &len, &range);
	if (err != noErr) {
		qWarning("CoreAudioOutput: Unable to query for allowed buffer size ranges.");
	} else {
		setBufferSize(static_cast<unsigned int>(range.mMaximum));
		qWarning("CoreAudioOutput: BufferFrameSizeRange = (%.2f, %.2f)", range.mMinimum, range.mMaximum);
	}

	UInt32 val                = (iFrameSize * iMixerFreq) / SAMPLE_RATE;
	propertyAddress.mSelector = kAudioDevicePropertyBufferFrameSize;
	CHECK_WARN(AudioObjectSetPropertyData(devId, &propertyAddress, 0, nullptr, sizeof(UInt32), &val),
	           "CoreAudioOutput: Could not set requested buffer size for device. Continuing with default.");

	CHECK_RETURN(AudioOutputUnitStart(auHAL),
	             "CoreAudioOutput: Unable to start AudioUnit");

	bRunning = true;
}

void CoreAudioOutput::stop() {
	bRunning = false;

	if (auHAL) {
		CHECK_WARN(AudioOutputUnitStop(auHAL),
		           "CoreAudioOutput: Unable to stop AudioUnit.");
		CHECK_WARN(AudioUnitUninitialize(auHAL),
		           "CoreAudioOutput: Unable to uninitialize AudioUnit.");
		auHAL = nullptr;
	}

	qWarning("CoreAudioOutput: Shutting down.");
}

CoreAudioOutput::~CoreAudioOutput() {
	bRunning = false;

	wait();
	stop();
}

OSStatus CoreAudioOutput::outputCallback(void *udata, AudioUnitRenderActionFlags *flags, const AudioTimeStamp *ts,
                                         UInt32 busnum, UInt32 nframes, AudioBufferList *buflist) {
	Q_UNUSED(flags);
	Q_UNUSED(ts);
	Q_UNUSED(busnum);

	CoreAudioOutput *o = reinterpret_cast< CoreAudioOutput * >(udata);
	AudioBuffer *buf   = buflist->mBuffers;

	if (o->bRunning) {
		bool done = o->mix(buf->mData, nframes);
		if (!done) {
			buf->mDataByteSize = 0;
			return -1;
		}
	} else {
		buf->mDataByteSize = 0;
	}

	return noErr;
}

void CoreAudioOutput::propertyChange(void *udata, AudioUnit auHAL, AudioUnitPropertyID prop, AudioUnitScope scope,
                                     AudioUnitElement element) {
	Q_UNUSED(udata);
	Q_UNUSED(auHAL);
	Q_UNUSED(scope);
	Q_UNUSED(element);

	CoreAudioOutput *o = reinterpret_cast< CoreAudioOutput * >(udata);
	if (!o->bRunning) return;

	if (prop == kAudioUnitProperty_StreamFormat) {
		qWarning("CoreAudioOutput: Stream format change detected. Restarting AudioOutput.");
		o->stop();
		Audio::stopOutput();
		Audio::startOutput();
	} else {
		qWarning("CoreAudioOutput: Unexpected property changed event received.");
	}
}

OSStatus CoreAudioOutput::deviceChange(AudioObjectID inObjectID, UInt32 inNumberAddresses,
									   const AudioObjectPropertyAddress inAddresses[], void *udata) {
	Q_UNUSED(inObjectID);
	Q_UNUSED(inNumberAddresses);
	Q_UNUSED(inAddresses);

	CoreAudioOutput *o = reinterpret_cast< CoreAudioOutput * >(udata);
	if (!o->bRunning) return noErr;

	qWarning("CoreAudioOutput: Output device change detected. Restarting AudioOutput.");
	Audio::stopOutput();
	Audio::startOutput();

	return noErr;
}
