// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "WASAPI.h"
#include "WASAPINotificationClient.h"

#ifdef __MINGW32__
// Fix a redefinition issue in protobuf's "strutil.h" include file:
// "redefinition of 'std::__cxx11::string google::protobuf::StrCat_instead_use_StringCbCat_or_StringCchCat'"
#	ifdef StrCat
#		undef StrCat
#	endif
#endif

#include "MainWindow.h"
#include "Utils.h"
#include "Global.h"

// Now that Win7 is published, which includes public versions of these
// interfaces, we simply inherit from those but use the "old" IIDs.

// Note that the DEFINE_GUID macro here only declares the existence of the respective variables
// as extern variables. The actual initialization of these variables happens in WinGUIDs.cpp
DEFINE_GUID(IID_IVistaAudioSessionControl2, 0x33969B1DL, 0xD06F, 0x4281, 0xB8, 0x37, 0x7E, 0xAA, 0xFD, 0x21, 0xA9,
			0xC0);
MIDL_INTERFACE("33969B1D-D06F-4281-B837-7EAAFD21A9C0")
IVistaAudioSessionControl2 : public IAudioSessionControl2{};

DEFINE_GUID(IID_IAudioSessionQuery, 0x94BE9D30L, 0x53AC, 0x4802, 0x82, 0x9C, 0xF1, 0x3E, 0x5A, 0xD3, 0x47, 0x75);
MIDL_INTERFACE("94BE9D30-53AC-4802-829C-F13E5AD34775")
IAudioSessionQuery : public IUnknown {
	virtual HRESULT STDMETHODCALLTYPE GetQueryInterface(IAudioSessionEnumerator **) = 0;
};

/// Convert the configured 'wasapi/role' to an ERole.
static ERole WASAPIRoleFromSettings() {
	QString role = Global::get().s.qsWASAPIRole.toLower().trimmed();

	if (role == QLatin1String("console")) {
		return eConsole;
	} else if (role == QLatin1String("multimedia")) {
		return eMultimedia;
	}

	return eCommunications;
}

class WASAPIInputRegistrar : public AudioInputRegistrar {
public:
	WASAPIInputRegistrar();
	virtual AudioInput *create();
	virtual const QVariant getDeviceChoice();
	virtual const QList< audioDevice > getDeviceChoices();
	virtual void setDeviceChoice(const QVariant &, Settings &);
	virtual bool canEcho(EchoCancelOptionID echoCancelID, const QString &outputSystem) const;
	virtual bool canExclusive() const;
	virtual bool isMicrophoneAccessDeniedByOS();

	// Windows doesn't provide an interface to query the permission of microphone access.
	// We only know the answer after trying to initialize the WASAPIAudioInput.
	// This static attribute will be set by WASAPIAudioInput to indicate if its access attempt
	// failed.
	static bool hasOSPermissionDenied;
};

bool WASAPIInputRegistrar::hasOSPermissionDenied = false;

class WASAPIOutputRegistrar : public AudioOutputRegistrar {
public:
	WASAPIOutputRegistrar();
	virtual AudioOutput *create();
	virtual const QVariant getDeviceChoice();
	virtual const QList< audioDevice > getDeviceChoices();
	virtual void setDeviceChoice(const QVariant &, Settings &);
	bool canMuteOthers() const;
	virtual bool canExclusive() const;
};

class WASAPIInit : public DeferInit {
	WASAPIInputRegistrar *wirReg;
	WASAPIOutputRegistrar *worReg;

public:
	WASAPIInit() : wirReg(nullptr), worReg(nullptr) {}
	void initialize();
	void destroy();
};

static WASAPIInit wasapiinit;

extern bool bIsWin7, bIsVistaSP1;

void WASAPIInit::initialize() {
	wirReg = nullptr;
	worReg = nullptr;

	if (!bIsVistaSP1) {
		qWarning("WASAPIInit: Requires Vista SP1");
		return;
	}

	HMODULE hLib = LoadLibrary(L"AVRT.DLL");
	if (!hLib) {
		qWarning("WASAPIInit: Failed to load avrt.dll");
		return;
	}
	FreeLibrary(hLib);

	wirReg = new WASAPIInputRegistrar();
	worReg = new WASAPIOutputRegistrar();
}

void WASAPIInit::destroy() {
	delete wirReg;
	delete worReg;
}


WASAPIInputRegistrar::WASAPIInputRegistrar() : AudioInputRegistrar(QLatin1String("WASAPI"), 10) {
	echoOptions.push_back(EchoCancelOptionID::SPEEX_MIXED);
	echoOptions.push_back(EchoCancelOptionID::SPEEX_MULTICHANNEL);
}

bool WASAPIInputRegistrar::isMicrophoneAccessDeniedByOS() {
	return hasOSPermissionDenied;
};


/// Calls getMixFormat on given IAudioClient and checks whether it is compatible.
/// At the moment this means the format is either 32bit float or 16bit PCM.
///
/// @param sourceName Name to prepend to log in case of error
/// @param deviceName Device name to refer to in case of error
/// @param audioClient IAudioClient to get and check mix format for
/// @param waveFormatEx WAVEFORMATEX structure to store getMixFormat result in
/// @param waveFormatExtensible If waveFormatEx is of type WAVEFORMATEXTENSIBLE receives a cast pointer.
/// @param sampleFormat Receives either SampleFloat or SampleShort as valid format
/// @return True if mix format is ok. False if incompatible or another error occurred.

template< typename SAMPLEFORMAT > // Template on SampleFormat enum as AudioOutput and AudioInput each define their own
bool getAndCheckMixFormat(const char *sourceName, const char *deviceName, IAudioClient *audioClient,
						  WAVEFORMATEX **waveFormatEx, WAVEFORMATEXTENSIBLE **waveFormatExtensible,
						  SAMPLEFORMAT *sampleFormat) {
	*waveFormatEx         = nullptr;
	*waveFormatExtensible = nullptr;

	HRESULT hr = audioClient->GetMixFormat(waveFormatEx);
	if (FAILED(hr)) {
		qWarning("%s: %s GetMixFormat failed: hr=0x%08lx", sourceName, deviceName, hr);
		return false;
	}

	if ((*waveFormatEx)->wFormatTag == WAVE_FORMAT_EXTENSIBLE) {
		(*waveFormatExtensible) = reinterpret_cast< WAVEFORMATEXTENSIBLE * >((*waveFormatEx));
		if ((*waveFormatExtensible)->SubFormat == KSDATAFORMAT_SUBTYPE_IEEE_FLOAT) {
			*sampleFormat = SAMPLEFORMAT::SampleFloat;
		} else if ((*waveFormatExtensible)->SubFormat == KSDATAFORMAT_SUBTYPE_PCM) {
			*sampleFormat = SAMPLEFORMAT::SampleShort;
		} else {
			qWarning() << sourceName << ":" << deviceName
					   << "Subformat is not IEEE Float or PCM but:" << (*waveFormatExtensible)->SubFormat;
			return false;
		}
	} else {
		if ((*waveFormatEx)->wFormatTag == WAVE_FORMAT_IEEE_FLOAT) {
			*sampleFormat = SAMPLEFORMAT::SampleFloat;
		} else if ((*waveFormatEx)->wFormatTag != WAVE_FORMAT_PCM) {
			*sampleFormat = SAMPLEFORMAT::SampleShort;
		} else {
			qWarning() << sourceName << ":" << deviceName
					   << "format tag is not IEEE Float or PCM but:" << (*waveFormatEx)->wFormatTag;
			return false;
		}
	}

	if (*sampleFormat == SAMPLEFORMAT::SampleFloat) {
		if ((*waveFormatEx)->wBitsPerSample != (sizeof(float) * 8)) {
			qWarning() << sourceName << ":" << deviceName
					   << "unexpected number of bits per sample for IEEE Float:" << (*waveFormatEx)->wBitsPerSample;
			return false;
		}
	} else if (*sampleFormat == SAMPLEFORMAT::SampleShort) {
		if ((*waveFormatEx)->wBitsPerSample != (sizeof(short) * 8)) {
			qWarning() << sourceName << ":" << deviceName
					   << "unexpected number of bits per sample for PCM:" << (*waveFormatEx)->wBitsPerSample;
			return false;
		}
	} else {
		qFatal("%s: %s unexpected sample format %lu", sourceName, deviceName,
			   static_cast< unsigned long >(*sampleFormat));
		return false;
	}

	return true;
}


AudioInput *WASAPIInputRegistrar::create() {
	return new WASAPIInput();
}

const QVariant WASAPIInputRegistrar::getDeviceChoice() {
	return Global::get().s.qsWASAPIInput;
}

const QList< audioDevice > WASAPIInputRegistrar::getDeviceChoices() {
	QList< audioDevice > choices;

	const QHash< QString, QString > devs = WASAPISystem::getInputDevices();

	auto keys = devs.keys();
	std::sort(keys.begin(), keys.end());

	for (const auto &key : keys) {
		choices << audioDevice(devs.value(key), key);
	}

	return choices;
}

void WASAPIInputRegistrar::setDeviceChoice(const QVariant &choice, Settings &s) {
	s.qsWASAPIInput = choice.toString();
}

bool WASAPIInputRegistrar::canEcho(EchoCancelOptionID echoOptionIDs, const QString &outputSystem) const {
	return (echoOptionIDs == EchoCancelOptionID::SPEEX_MIXED || echoOptionIDs == EchoCancelOptionID::SPEEX_MULTICHANNEL)
		   && (outputSystem == name);
}

bool WASAPIInputRegistrar::canExclusive() const {
	return true;
}

WASAPIOutputRegistrar::WASAPIOutputRegistrar() : AudioOutputRegistrar(QLatin1String("WASAPI"), 10) {
}

AudioOutput *WASAPIOutputRegistrar::create() {
	return new WASAPIOutput();
}

const QVariant WASAPIOutputRegistrar::getDeviceChoice() {
	return Global::get().s.qsWASAPIOutput;
}

const QList< audioDevice > WASAPIOutputRegistrar::getDeviceChoices() {
	QList< audioDevice > choices;

	const QHash< QString, QString > devs = WASAPISystem::getOutputDevices();
	auto keys                            = devs.keys();
	std::sort(keys.begin(), keys.end());

	for (const auto &key : keys) {
		choices << audioDevice(devs.value(key), key);
	}

	return choices;
}

void WASAPIOutputRegistrar::setDeviceChoice(const QVariant &choice, Settings &s) {
	s.qsWASAPIOutput = choice.toString();
}

bool WASAPIOutputRegistrar::canMuteOthers() const {
	return true;
}

bool WASAPIOutputRegistrar::canExclusive() const {
	return true;
}

const QHash< QString, QString > WASAPISystem::getInputDevices() {
	return getDevices(eCapture);
}

const QHash< QString, QString > WASAPISystem::getOutputDevices() {
	return getDevices(eRender);
}

const QHash< QString, QString > WASAPISystem::getDevices(EDataFlow dataflow) {
	QHash< QString, QString > devices;

	HRESULT hr;

	IMMDeviceEnumerator *pEnumerator = nullptr;
	IMMDeviceCollection *pCollection = nullptr;

	hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator),
						  reinterpret_cast< void ** >(&pEnumerator));

	if (!pEnumerator || FAILED(hr)) {
		qWarning("WASAPI: Failed to instantiate enumerator: hr=0x%08lx", hr);
	} else {
		hr = pEnumerator->EnumAudioEndpoints(dataflow, DEVICE_STATE_ACTIVE, &pCollection);
		if (!pCollection || FAILED(hr)) {
			qWarning("WASAPI: Failed to enumerate: hr=0x%08lx", hr);
		} else {
			devices.insert(QString(), tr("Default Device"));

			UINT ndev = 0;
			pCollection->GetCount(&ndev);
			for (unsigned int idx = 0; idx < ndev; ++idx) {
				IMMDevice *pDevice     = nullptr;
				IPropertyStore *pStore = nullptr;

				pCollection->Item(idx, &pDevice);
				pDevice->OpenPropertyStore(STGM_READ, &pStore);

				LPWSTR strid = nullptr;
				pDevice->GetId(&strid);

				PROPVARIANT varName;
				PropVariantInit(&varName);

				pStore->GetValue(PKEY_Device_FriendlyName, &varName);

				devices.insert(QString::fromWCharArray(strid), QString::fromWCharArray(varName.pwszVal));

				PropVariantClear(&varName);
				CoTaskMemFree(strid);

				pStore->Release();
				pDevice->Release();
			}
			pCollection->Release();
		}
		pEnumerator->Release();
	}

	return devices;
}

WASAPIInput::WASAPIInput(){};

WASAPIInput::~WASAPIInput() {
	bRunning = false;
	wait();
}

static IMMDevice *openNamedOrDefaultDevice(const QString &name, EDataFlow dataFlow, ERole role) {
	HRESULT hr;
	IMMDeviceEnumerator *pEnumerator = nullptr;

	hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator),
						  reinterpret_cast< void ** >(&pEnumerator));
	if (!pEnumerator || FAILED(hr)) {
		qWarning("WASAPI: Failed to instantiate enumerator: hr=0x%08lx", hr);
		return nullptr;
	}

	IMMDevice *pDevice = nullptr;
	// Try to find a device pointer for |name|.
	if (!name.isEmpty()) {
		std::vector< wchar_t > devname;
		devname.resize(name.length() + 1);
		int len      = name.toWCharArray(devname.data());
		devname[len] = 0;
		hr           = pEnumerator->GetDevice(devname.data(), &pDevice);
		if (FAILED(hr)) {
			qWarning("WASAPI: Failed to open selected device %s %ls (df=%d, e=%d, hr=0x%08lx), falling back to default",
					 qPrintable(name), devname.data(), dataFlow, role, hr);
		} else {
			WASAPINotificationClient::get().enlistDeviceAsUsed(devname.data());
		}
	}

	// Use the default device if |pDevice| is still nullptr.
	// We retrieve the actual device name for the currently selected default device and
	// open the device by it's real name to work around triggering the automatic
	// ducking behavior.
	if (!pDevice) {
		hr = pEnumerator->GetDefaultAudioEndpoint(dataFlow, role, &pDevice);
		if (FAILED(hr)) {
			qWarning("WASAPI: Failed to open device: df=%d, e=%d, hr=0x%08lx", dataFlow, role, hr);
			goto cleanup;
		}
		wchar_t *devname = nullptr;
		hr               = pDevice->GetId(&devname);
		if (FAILED(hr)) {
			qWarning("WASAPI: Failed to query device: df=%d, e=%d, hr=0x%08lx", dataFlow, role, hr);
			goto cleanup;
		}
		pDevice->Release();
		hr = pEnumerator->GetDevice(devname, &pDevice);
		if (FAILED(hr)) {
			qWarning("WASAPI: Failed to reopen default device: df=%d, e=%d, hr=0x%08lx", dataFlow, role, hr);
			goto cleanup;
		}
		WASAPINotificationClient::get().enlistDefaultDeviceAsUsed(devname);
		CoTaskMemFree(devname);
	}

cleanup:
	if (pEnumerator)
		pEnumerator->Release();

	return pDevice;
}

void WASAPIInput::run() {
	HRESULT hr;
	IMMDevice *pMicDevice                   = nullptr;
	IAudioClient *pMicAudioClient           = nullptr;
	IAudioCaptureClient *pMicCaptureClient  = nullptr;
	IMMDevice *pEchoDevice                  = nullptr;
	IAudioClient *pEchoAudioClient          = nullptr;
	IAudioCaptureClient *pEchoCaptureClient = nullptr;
	WAVEFORMATEX *micpwfx = nullptr, *echopwfx = nullptr;
	WAVEFORMATEXTENSIBLE *micpwfxe = nullptr, *echopwfxe = nullptr;
	WAVEFORMATEXTENSIBLE wfe;
	UINT32 bufferFrameCount;
	UINT32 numFramesAvailable;
	UINT32 micPacketLength = 0, echoPacketLength = 0;
	UINT32 allocLength;
	UINT64 devicePosition;
	UINT64 qpcPosition;
	HANDLE hEvent;
	BYTE *pData;
	DWORD flags;
	DWORD dwTaskIndex = 0;
	HANDLE hMmThread;
	float *tbuff = nullptr;
	short *sbuff = nullptr;
	bool doecho  = Global::get().s.doEcho();
	REFERENCE_TIME def, min, latency, want;
	bool exclusive = false;

	CoInitialize(nullptr);

	hEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

	hMmThread = AvSetMmThreadCharacteristics(L"Pro Audio", &dwTaskIndex);
	if (!hMmThread) {
		qWarning("WASAPIInput: Failed to set Pro Audio thread priority");
	}

	// Open mic device.
	pMicDevice = openNamedOrDefaultDevice(Global::get().s.qsWASAPIInput, eCapture, WASAPIRoleFromSettings());
	if (!pMicDevice)
		goto cleanup;

	// Open echo capture device.
	if (doecho) {
		pEchoDevice = openNamedOrDefaultDevice(Global::get().s.qsWASAPIOutput, eRender, WASAPIRoleFromSettings());
		if (!pEchoDevice)
			doecho = false;
	}

	hr = pMicDevice->Activate(__uuidof(IAudioClient), CLSCTX_ALL, nullptr, (void **) &pMicAudioClient);
	if (FAILED(hr)) {
		qWarning("WASAPIInput: Activate Mic AudioClient failed: hr=0x%08lx", hr);
		goto cleanup;
	}

	def = min = latency = 0;

	pMicAudioClient->GetDevicePeriod(&def, &min);

	want = qMax< REFERENCE_TIME >(min, 100000);
	qWarning("WASAPIInput: Latencies %lld %lld => %lld", def, min, want);

	if (Global::get().s.bExclusiveInput && !doecho) {
		for (int channels = 1; channels <= 2; ++channels) {
			ZeroMemory(&wfe, sizeof(wfe));
			wfe.Format.cbSize          = 0;
			wfe.Format.wFormatTag      = WAVE_FORMAT_PCM;
			wfe.Format.nChannels       = channels;
			wfe.Format.nSamplesPerSec  = 48000;
			wfe.Format.wBitsPerSample  = 16;
			wfe.Format.nBlockAlign     = wfe.Format.nChannels * wfe.Format.wBitsPerSample / 8;
			wfe.Format.nAvgBytesPerSec = wfe.Format.nBlockAlign * wfe.Format.nSamplesPerSec;

			micpwfxe = &wfe;
			micpwfx  = reinterpret_cast< WAVEFORMATEX * >(&wfe);

			hr = pMicAudioClient->Initialize(AUDCLNT_SHAREMODE_EXCLUSIVE, AUDCLNT_STREAMFLAGS_EVENTCALLBACK, want, want,
											 micpwfx, nullptr);
			if (SUCCEEDED(hr)) {
				eMicFormat = SampleShort;
				exclusive  = true;
				qWarning("WASAPIInput: Successfully opened exclusive mode");
				break;
			}

			micpwfxe = nullptr;
			micpwfx  = nullptr;
		}
	}

	if (!micpwfxe) {
		if (Global::get().s.bExclusiveInput)
			qWarning("WASAPIInput: Failed to open exclusive mode.");

		if (!getAndCheckMixFormat("WASAPIInput", "Mic", pMicAudioClient, &micpwfx, &micpwfxe, &eMicFormat)) {
			goto cleanup;
		}

		hr = pMicAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, AUDCLNT_STREAMFLAGS_EVENTCALLBACK, 0, 0, micpwfx,
										 nullptr);
		if (FAILED(hr)) {
			qWarning("WASAPIInput: Mic Initialize failed: hr=0x%08lx", hr);
			if (hr == E_ACCESSDENIED) {
				WASAPIInputRegistrar::hasOSPermissionDenied = true;
				Global::get().mw->msgBox(
					tr("Access to the microphone was denied. Please check that your operating system's "
					   "microphone settings allow Mumble to use the microphone."));
			}
			goto cleanup;
		}
	}

	qWarning() << "WASAPIInput: Mic Stream format" << eMicFormat;

	pMicAudioClient->GetStreamLatency(&latency);
	hr = pMicAudioClient->GetBufferSize(&bufferFrameCount);
	qWarning("WASAPIInput: Stream Latency %lld (%d)", latency, bufferFrameCount);

	hr = pMicAudioClient->GetService(__uuidof(IAudioCaptureClient), (void **) &pMicCaptureClient);
	if (FAILED(hr)) {
		qWarning("WASAPIInput: Mic GetService failed: hr=0x%08lx", hr);
		goto cleanup;
	}

	pMicAudioClient->SetEventHandle(hEvent);
	if (FAILED(hr)) {
		qWarning("WASAPIInput: Failed to set mic event: hr=0x%08lx", hr);
		goto cleanup;
	}

	hr = pMicAudioClient->Start();
	if (FAILED(hr)) {
		qWarning("WASAPIInput: Failed to start mic: hr=0x%08lx", hr);
		goto cleanup;
	}

	iMicChannels = micpwfx->nChannels;
	iMicFreq     = micpwfx->nSamplesPerSec;

	if (doecho) {
		hr = pEchoDevice->Activate(__uuidof(IAudioClient), CLSCTX_ALL, nullptr, (void **) &pEchoAudioClient);
		if (FAILED(hr)) {
			qWarning("WASAPIInput: Activate Echo AudioClient failed: hr=0x%08lx", hr);
			goto cleanup;
		}

		if (!getAndCheckMixFormat("WASAPIInput", "Echo", pEchoAudioClient, &echopwfx, &echopwfxe, &eEchoFormat)) {
			goto cleanup;
		}

		hr = pEchoAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED,
										  AUDCLNT_STREAMFLAGS_EVENTCALLBACK | AUDCLNT_STREAMFLAGS_LOOPBACK, 0, 0,
										  echopwfx, nullptr);
		if (FAILED(hr)) {
			qWarning("WASAPIInput: Echo Initialize failed: hr=0x%08lx", hr);
			goto cleanup;
		}

		hr = pEchoAudioClient->GetBufferSize(&bufferFrameCount);
		hr = pEchoAudioClient->GetService(__uuidof(IAudioCaptureClient), (void **) &pEchoCaptureClient);
		if (FAILED(hr)) {
			qWarning("WASAPIInput: Echo GetService failed: hr=0x%08lx", hr);
			goto cleanup;
		}

		pEchoAudioClient->SetEventHandle(hEvent);
		if (FAILED(hr)) {
			qWarning("WASAPIInput: Failed to set echo event: hr=0x%08lx", hr);
			goto cleanup;
		}

		hr = pEchoAudioClient->Start();
		if (FAILED(hr)) {
			qWarning("WASAPIInput: Failed to start Echo: hr=0x%08lx", hr);
			goto cleanup;
		}

		qWarning() << "WASAPIInput: Echo Stream format" << eEchoFormat;

		iEchoChannels = echopwfx->nChannels;
		iEchoFreq     = echopwfx->nSamplesPerSec;
	}

	initializeMixer();

	allocLength = (iMicLength / 2) * micpwfx->nChannels;

	if (exclusive) {
		sbuff = new short[allocLength];
		while (bRunning && !FAILED(hr)) {
			hr = pMicCaptureClient->GetBuffer(&pData, &numFramesAvailable, &flags, &devicePosition, &qpcPosition);
			if (hr != AUDCLNT_S_BUFFER_EMPTY) {
				if (FAILED(hr))
					goto cleanup;

				UINT32 nFrames = numFramesAvailable * micpwfx->nChannels;
				if (nFrames > allocLength) {
					delete[] sbuff;
					allocLength = nFrames;
					sbuff       = new short[allocLength];
				}

				memcpy(sbuff, pData, nFrames * sizeof(short));
				hr = pMicCaptureClient->ReleaseBuffer(numFramesAvailable);
				if (FAILED(hr))
					goto cleanup;
				addMic(sbuff, numFramesAvailable);
			}
			if (!FAILED(hr))
				WaitForSingleObject(hEvent, 100);
		}
	} else {
		tbuff = new float[allocLength];
		while (bRunning && !FAILED(hr)) {
			hr = pMicCaptureClient->GetNextPacketSize(&micPacketLength);
			if (!FAILED(hr) && iEchoChannels)
				hr = pEchoCaptureClient->GetNextPacketSize(&echoPacketLength);
			if (FAILED(hr)) {
				qWarning("WASAPIInput: GetNextPacketSize failed: hr=0x%08lx", hr);
				goto cleanup;
			}

			while ((micPacketLength > 0) || (echoPacketLength > 0)) {
				if (echoPacketLength > 0) {
					hr = pEchoCaptureClient->GetBuffer(&pData, &numFramesAvailable, &flags, &devicePosition,
													   &qpcPosition);
					if (FAILED(hr)) {
						qWarning("WASAPIInput: GetBuffer failed: hr=0x%08lx", hr);
						goto cleanup;
					}

					UINT32 nFrames = numFramesAvailable * echopwfx->nChannels;
					if (nFrames > allocLength) {
						delete[] tbuff;
						allocLength = nFrames;
						tbuff       = new float[allocLength];
					}
					memcpy(tbuff, pData, nFrames * sizeof(float));
					hr = pEchoCaptureClient->ReleaseBuffer(numFramesAvailable);
					if (FAILED(hr)) {
						qWarning("WASAPIInput: ReleaseBuffer failed: hr=0x%08lx", hr);
						goto cleanup;
					}
					addEcho(tbuff, numFramesAvailable);
				} else if (micPacketLength > 0) {
					hr = pMicCaptureClient->GetBuffer(&pData, &numFramesAvailable, &flags, &devicePosition,
													  &qpcPosition);
					if (FAILED(hr)) {
						qWarning("WASAPIInput: GetBuffer failed: hr=0x%08lx", hr);
						goto cleanup;
					}

					UINT32 nFrames = numFramesAvailable * micpwfx->nChannels;
					if (nFrames > allocLength) {
						delete[] tbuff;
						allocLength = nFrames;
						tbuff       = new float[allocLength];
					}
					memcpy(tbuff, pData, nFrames * sizeof(float));
					hr = pMicCaptureClient->ReleaseBuffer(numFramesAvailable);
					if (FAILED(hr)) {
						qWarning("WASAPIInput: ReleaseBuffer failed: hr=0x%08lx", hr);
						goto cleanup;
					}
					addMic(tbuff, numFramesAvailable);
				}
				hr = pMicCaptureClient->GetNextPacketSize(&micPacketLength);
				if (!FAILED(hr) && iEchoChannels)
					hr = pEchoCaptureClient->GetNextPacketSize(&echoPacketLength);
			}
			if (!FAILED(hr))
				WaitForSingleObject(hEvent, 2000);
		}
	}

cleanup:
	if (micpwfx && !exclusive)
		CoTaskMemFree(micpwfx);
	if (echopwfx)
		CoTaskMemFree(echopwfx);

	if (pMicAudioClient) {
		pMicAudioClient->Stop();
		pMicAudioClient->Release();
	}
	if (pMicCaptureClient)
		pMicCaptureClient->Release();
	if (pMicDevice)
		pMicDevice->Release();

	if (pEchoAudioClient) {
		pEchoAudioClient->Stop();
		pEchoAudioClient->Release();
	}
	if (pEchoCaptureClient)
		pEchoCaptureClient->Release();
	if (pEchoDevice)
		pEchoDevice->Release();

	if (hMmThread)
		AvRevertMmThreadCharacteristics(hMmThread);

	if (hEvent)
		CloseHandle(hEvent);

	delete[] tbuff;
	delete[] sbuff;
}

WASAPIOutput::WASAPIOutput() {
}

WASAPIOutput::~WASAPIOutput() {
	bRunning = false;
	wait();
}

void WASAPIOutput::setVolumes(IMMDevice *pDevice, bool talking) {
	HRESULT hr;

	if (!talking) {
		QMap< ISimpleAudioVolume *, VolumePair >::const_iterator i;
		for (i = qmVolumes.constBegin(); i != qmVolumes.constEnd(); ++i) {
			float fVolume = 1.0f;
			hr            = i.key()->GetMasterVolume(&fVolume);
			if (qFuzzyCompare(i.value().second, fVolume))
				hr = i.key()->SetMasterVolume(i.value().first, nullptr);
			i.key()->Release();
		}
		qmVolumes.clear();
		return;
	}

	IAudioSessionManager2 *pAudioSessionManager = nullptr;
	int max                                     = 0;
	DWORD dwMumble                              = GetCurrentProcessId();

	qmVolumes.clear();
	if (qFuzzyCompare(Global::get().s.fOtherVolume, 1.0f))
		return;

	// FIXME: Try to keep the session object around when returning volume.

	if (SUCCEEDED(hr = pDevice->Activate(bIsWin7 ? __uuidof(IAudioSessionManager2) : __uuidof(IAudioSessionManager),
										 CLSCTX_ALL, nullptr, (void **) &pAudioSessionManager))) {
		IAudioSessionEnumerator *pEnumerator = nullptr;
		IAudioSessionQuery *pMysticQuery     = nullptr;
		if (!bIsWin7) {
			if (SUCCEEDED(hr = pAudioSessionManager->QueryInterface(IID_IAudioSessionQuery, (void **) &pMysticQuery))) {
				hr = pMysticQuery->GetQueryInterface(&pEnumerator);
			}
		} else {
			hr = pAudioSessionManager->GetSessionEnumerator(&pEnumerator);
		}

		QSet< QUuid > seen;

		if (SUCCEEDED(hr)) {
			if (SUCCEEDED(hr = pEnumerator->GetCount(&max))) {
				for (int i = 0; i < max; ++i) {
					IAudioSessionControl *pControl = nullptr;
					if (SUCCEEDED(hr = pEnumerator->GetSession(i, &pControl))) {
						setVolumeForSessionControl(pControl, dwMumble, seen);
						pControl->Release();
					}
				}
			}
			pEnumerator->Release();
		}
		if (pMysticQuery)
			pMysticQuery->Release();
		pAudioSessionManager->Release();
	}
}

bool WASAPIOutput::setVolumeForSessionControl2(IAudioSessionControl2 *control2, const DWORD mumblePID,
											   QSet< QUuid > &seen) {
	HRESULT hr;
	DWORD pid;

	// Don't set the volume for our own control
	if (FAILED(hr = control2->GetProcessId(&pid)) || (pid == mumblePID))
		return true;

	// Don't work on expired audio sessions
	AudioSessionState ass;
	if (FAILED(hr = control2->GetState(&ass)) || (ass == AudioSessionStateExpired))
		return false;

	// Don't act twice on the same session
	GUID group;
	if (FAILED(hr = control2->GetGroupingParam(&group)))
		return false;

	QUuid quuid(group);
	if (seen.contains(quuid))
		return true;

	seen.insert(quuid);

	// Adjust volume
	ISimpleAudioVolume *pVolume = nullptr;
	if (FAILED(hr = control2->QueryInterface(__uuidof(ISimpleAudioVolume), (void **) &pVolume)))
		return false;

	BOOL bMute = TRUE;
	bool keep  = false;
	if (SUCCEEDED(hr = pVolume->GetMute(&bMute)) && !bMute) {
		float fVolume = 1.0f;
		if (SUCCEEDED(hr = pVolume->GetMasterVolume(&fVolume)) && !qFuzzyCompare(fVolume, 0.0f)) {
			float fSetVolume = fVolume * Global::get().s.fOtherVolume;
			if (SUCCEEDED(hr = pVolume->SetMasterVolume(fSetVolume, nullptr))) {
				hr = pVolume->GetMasterVolume(&fSetVolume);
				qmVolumes.insert(pVolume, VolumePair(fVolume, fSetVolume));
				keep = true;
			}
		}
	}

	if (!keep)
		pVolume->Release();

	return true;
}

bool WASAPIOutput::setVolumeForSessionControl(IAudioSessionControl *control, const DWORD mumblePID,
											  QSet< QUuid > &seen) {
	HRESULT hr;
	IAudioSessionControl2 *pControl2 = nullptr;

	if (!SUCCEEDED(
			hr = control->QueryInterface(bIsWin7 ? __uuidof(IAudioSessionControl2) : IID_IVistaAudioSessionControl2,
										 (void **) &pControl2)))
		return false;

	bool result = setVolumeForSessionControl2(pControl2, mumblePID, seen);

	pControl2->Release();
	return result;
}

static void SetDuckingOptOut(IMMDevice *pDevice) {
	if (!bIsWin7)
		return;

	HRESULT hr;
	IAudioSessionManager2 *pSessionManager2 = nullptr;
	IAudioSessionControl *pSessionControl   = nullptr;
	IAudioSessionControl2 *pSessionControl2 = nullptr;

	// Get session manager & control1+2 to disable ducking
	hr = pDevice->Activate(__uuidof(IAudioSessionManager2), CLSCTX_ALL, nullptr,
						   reinterpret_cast< void ** >(&pSessionManager2));
	if (FAILED(hr)) {
		qWarning("WASAPIOutput: Activate AudioSessionManager2 failed: hr=0x%08lx", hr);
		goto cleanup;
	}

	hr = pSessionManager2->GetAudioSessionControl(nullptr, 0, &pSessionControl);
	if (FAILED(hr)) {
		qWarning("WASAPIOutput: GetAudioSessionControl failed: hr=0x%08lx", hr);
		goto cleanup;
	}

	hr = pSessionControl->QueryInterface(__uuidof(IAudioSessionControl2),
										 reinterpret_cast< void ** >(&pSessionControl2));
	if (FAILED(hr)) {
		qWarning("WASAPIOutput: Querying SessionControl2 failed: hr=0x%08lx", hr);
		goto cleanup;
	}

	hr = pSessionControl2->SetDuckingPreference(TRUE);
	if (FAILED(hr)) {
		qWarning("WASAPIOutput: Failed to set ducking preference: hr=0x%08lx", hr);
		goto cleanup;
	}

cleanup:
	if (pSessionControl2)
		pSessionControl2->Release();

	if (pSessionControl)
		pSessionControl->Release();

	if (pSessionManager2)
		pSessionManager2->Release();
}

void WASAPIOutput::run() {
	HRESULT hr;
	IMMDevice *pDevice                = nullptr;
	IAudioClient *pAudioClient        = nullptr;
	IAudioRenderClient *pRenderClient = nullptr;
	WAVEFORMATEX *pwfx                = nullptr;
	WAVEFORMATEXTENSIBLE *pwfxe       = nullptr;
	UINT32 bufferFrameCount;
	REFERENCE_TIME def, min, latency, want;
	UINT32 numFramesAvailable;
	HANDLE hEvent;
	BYTE *pData;
	DWORD dwTaskIndex = 0;
	HANDLE hMmThread;
	int ns = 0;
	unsigned int chanmasks[32];
	bool lastspoke = false;
	REFERENCE_TIME bufferDuration =
		(Global::get().s.iOutputDelay > 1) ? (Global::get().s.iOutputDelay + 1) * 100000 : 0;
	bool exclusive = false;
	bool mixed     = false;

	CoInitialize(nullptr);

	hEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

	hMmThread = AvSetMmThreadCharacteristics(L"Pro Audio", &dwTaskIndex);
	if (!hMmThread) {
		qWarning("WASAPIOutput: Failed to set Pro Audio thread priority");
	}

	// Open the output device.
	pDevice = openNamedOrDefaultDevice(Global::get().s.qsWASAPIOutput, eRender, WASAPIRoleFromSettings());
	if (!pDevice)
		goto cleanup;

	// Opt-out of the Windows 7 ducking behavior
	SetDuckingOptOut(pDevice);

	hr = pDevice->Activate(__uuidof(IAudioClient), CLSCTX_ALL, nullptr, (void **) &pAudioClient);
	if (FAILED(hr)) {
		qWarning("WASAPIOutput: Activate AudioClient failed: hr=0x%08lx", hr);
		goto cleanup;
	}

	pAudioClient->GetDevicePeriod(&def, &min);
	want = qMax< REFERENCE_TIME >(min, 100000);
	qWarning("WASAPIOutput: Latencies %lld %lld => %lld", def, min, want);

	if (Global::get().s.bExclusiveOutput) {
		hr = pAudioClient->GetMixFormat(&pwfx);
		if (FAILED(hr)) {
			qWarning("WASAPIOutput: GetMixFormat failed: hr=0x%08lx", hr);
			goto cleanup;
		}

		if (pwfx->wFormatTag == WAVE_FORMAT_EXTENSIBLE) {
			pwfxe = reinterpret_cast< WAVEFORMATEXTENSIBLE * >(pwfx);
		}

		if (!Global::get().s.bPositionalAudio) {
			// Override mix format and request stereo
			pwfx->nChannels = 2;
			if (pwfxe) {
				pwfxe->dwChannelMask = KSAUDIO_SPEAKER_STEREO;
			}
		}

		pwfx->cbSize = 0;
		if (pwfxe) {
			pwfxe->SubFormat = KSDATAFORMAT_SUBTYPE_PCM;
		} else {
			pwfx->wFormatTag = WAVE_FORMAT_PCM;
		}
		pwfx->nSamplesPerSec  = 48000;
		pwfx->wBitsPerSample  = 16;
		pwfx->nBlockAlign     = pwfx->nChannels * pwfx->wBitsPerSample / 8;
		pwfx->nAvgBytesPerSec = pwfx->nBlockAlign * pwfx->nSamplesPerSec;

		hr = pAudioClient->Initialize(AUDCLNT_SHAREMODE_EXCLUSIVE, AUDCLNT_STREAMFLAGS_EVENTCALLBACK, want, want, pwfx,
									  nullptr);
		if (SUCCEEDED(hr)) {
			eSampleFormat = SampleShort;
			exclusive     = true;
			qWarning("WASAPIOutput: Successfully opened exclusive mode");
		} else {
			CoTaskMemFree(pwfx);

			pwfxe = nullptr;
			pwfx  = nullptr;
		}
	}

	if (!pwfx) {
		if (Global::get().s.bExclusiveOutput)
			qWarning("WASAPIOutput: Failed to open exclusive mode.");

		if (!getAndCheckMixFormat("WASAPIOutput", "Output", pAudioClient, &pwfx, &pwfxe, &eSampleFormat)) {
			goto cleanup;
		}

		if (!Global::get().s.bPositionalAudio) {
			pwfx->nChannels       = 2;
			pwfx->nBlockAlign     = pwfx->nChannels * pwfx->wBitsPerSample / 8;
			pwfx->nAvgBytesPerSec = pwfx->nBlockAlign * pwfx->nSamplesPerSec;

			if (pwfxe) {
				pwfxe->dwChannelMask = KSAUDIO_SPEAKER_STEREO;
			}

			WAVEFORMATEX *closestFormat = nullptr;
			hr = pAudioClient->IsFormatSupported(AUDCLNT_SHAREMODE_SHARED, pwfx, &closestFormat);
			if (hr == S_FALSE) {
				qWarning("WASAPIOutput: Driver says no to 2 channel output. Closest format: %d channels @ %lu kHz",
						 closestFormat->nChannels, static_cast< unsigned long >(closestFormat->nSamplesPerSec));
				CoTaskMemFree(pwfx);

				// Fall back to whatever the device offers.

				if (!getAndCheckMixFormat("WASAPIOutput", "Output", pAudioClient, &pwfx, &pwfxe, &eSampleFormat)) {
					CoTaskMemFree(closestFormat);
					goto cleanup;
				}
			} else if (FAILED(hr)) {
				qWarning("WASAPIOutput: IsFormatSupported failed: hr=0x%08lx", hr);
			}

			CoTaskMemFree(closestFormat);
		}

		hr = pAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, AUDCLNT_STREAMFLAGS_EVENTCALLBACK, bufferDuration, 0,
									  pwfx, nullptr);
		if (FAILED(hr)) {
			qWarning("WASAPIOutput: Initialize failed: hr=0x%08lx", hr);
			goto cleanup;
		}
	}

	qWarning() << "WASAPIOutput: Output stream format" << eSampleFormat;

	pAudioClient->GetStreamLatency(&latency);
	pAudioClient->GetBufferSize(&bufferFrameCount);
	qWarning("WASAPIOutput: Stream Latency %lld (%d)", latency, bufferFrameCount);

	iMixerFreq = pwfx->nSamplesPerSec;

	qWarning("WASAPIOutput: Periods %lldus %lldus (latency %lldus)", def / 10LL, min / 10LL, latency / 10LL);
	qWarning("WASAPIOutput: Buffer is %dus (%d)", (bufferFrameCount * 1000000) / iMixerFreq,
			 Global::get().s.iOutputDelay);

	hr = pAudioClient->GetService(__uuidof(IAudioRenderClient), (void **) &pRenderClient);
	if (FAILED(hr)) {
		qWarning("WASAPIOutput: GetService failed: hr=0x%08lx", hr);
		goto cleanup;
	}

	pAudioClient->SetEventHandle(hEvent);
	if (FAILED(hr)) {
		qWarning("WASAPIOutput: Failed to set event: hr=0x%08lx", hr);
		goto cleanup;
	}

	hr = pAudioClient->Start();
	if (FAILED(hr)) {
		qWarning("WASAPIOutput: Failed to start: hr=0x%08lx", hr);
		goto cleanup;
	}

	if (pwfxe) {
		for (int i = 0; i < 32; i++) {
			if (pwfxe->dwChannelMask & (1 << i)) {
				chanmasks[ns++] = 1 << i;
			}
		}
	} else {
		qWarning("WASAPIOutput: No chanmask available. Assigning in order.");

		for (int i = 0; i < pwfx->nChannels && i < 32; ++i) {
			chanmasks[ns++] = 1 << i;
		}
	}

	if (ns != pwfx->nChannels) {
		qWarning("WASAPIOutput: Chanmask bits doesn't match number of channels.");
	}

	iChannels = pwfx->nChannels;
	initializeMixer(chanmasks);

	numFramesAvailable = 0;

	while (bRunning && !FAILED(hr)) {
		if (!exclusive) {
			// Attenuate stream volumes.
			if (lastspoke != (Global::get().bAttenuateOthers || mixed)) {
				lastspoke = Global::get().bAttenuateOthers || mixed;
				setVolumes(pDevice, lastspoke);
			}

			hr = pAudioClient->GetCurrentPadding(&numFramesAvailable);
			if (FAILED(hr)) {
				qWarning("WASAPIOutput: GetCurrentPadding failed: hr=0x%08lx", hr);
				goto cleanup;
			}
		}

		UINT32 packetLength = bufferFrameCount - numFramesAvailable;

		while (packetLength > 0) {
			hr = pRenderClient->GetBuffer(packetLength, &pData);
			if (FAILED(hr)) {
				qWarning("WASAPIOutput: GetBuffer failed: hr=0x%08lx", hr);
				goto cleanup;
			}

			mixed = mix(reinterpret_cast< float * >(pData), packetLength);
			if (mixed)
				hr = pRenderClient->ReleaseBuffer(packetLength, 0);
			else
				hr = pRenderClient->ReleaseBuffer(packetLength, AUDCLNT_BUFFERFLAGS_SILENT);
			if (FAILED(hr)) {
				qWarning("WASAPIOutput: ReleaseBuffer failed: hr=0x%08lx", hr);
				goto cleanup;
			}

			// Exclusive mode rendering ends here.
			if (exclusive)
				break;

			if (!Global::get().s.bAttenuateOthers && !Global::get().bAttenuateOthers) {
				mixed = false;
			}

			if (lastspoke != (Global::get().bAttenuateOthers || mixed)) {
				lastspoke = Global::get().bAttenuateOthers || mixed;
				setVolumes(pDevice, lastspoke);
			}

			hr = pAudioClient->GetCurrentPadding(&numFramesAvailable);
			if (FAILED(hr)) {
				qWarning("WASAPIOutput: GetCurrentPadding failed: hr=0x%08lx", hr);
				goto cleanup;
			}

			packetLength = bufferFrameCount - numFramesAvailable;
		}
		if (!FAILED(hr))
			WaitForSingleObject(hEvent, exclusive ? 100 : 2000);
	}

cleanup:
	if (pwfx)
		CoTaskMemFree(pwfx);

	if (pDevice) {
		setVolumes(pDevice, false);
	}

	if (pAudioClient) {
		pAudioClient->Stop();
		pAudioClient->Release();
	}
	if (pRenderClient)
		pRenderClient->Release();
	if (pDevice)
		pDevice->Release();

	if (hMmThread)
		AvRevertMmThreadCharacteristics(hMmThread);

	if (hEvent)
		CloseHandle(hEvent);
}
