/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>

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

#include "WASAPI.h"
#include "User.h"
#include "Global.h"
#include "MainWindow.h"
#include "Timer.h"

// Now that Win7 is published, which includes public versions of these
// interfaces, we simply inherit from those but use the "old" IIDs.

MIDL_INTERFACE("33969B1D-D06F-4281-B837-7EAAFD21A9C0")
IVistaAudioSessionControl2 :
public IAudioSessionControl2 {
};

MIDL_INTERFACE("94BE9D30-53AC-4802-829C-F13E5AD34775")
IAudioSessionQuery :
public IUnknown {
	virtual HRESULT STDMETHODCALLTYPE GetQueryInterface(IAudioSessionEnumerator **) = 0;
};

class WASAPIInputRegistrar : public AudioInputRegistrar {
	public:
		WASAPIInputRegistrar();
		virtual AudioInput *create();
		virtual const QList<audioDevice> getDeviceChoices();
		virtual void setDeviceChoice(const QVariant &, Settings &);
		virtual bool canEcho(const QString &) const;
		virtual bool canExclusive() const;
};

class WASAPIOutputRegistrar : public AudioOutputRegistrar {
	public:
		WASAPIOutputRegistrar();
		virtual AudioOutput *create();
		virtual const QList<audioDevice> getDeviceChoices();
		virtual void setDeviceChoice(const QVariant &, Settings &);
		bool canMuteOthers() const;
		virtual bool canExclusive() const;
};

class WASAPIInit : public DeferInit {
		WASAPIInputRegistrar *wirReg;
		WASAPIOutputRegistrar *worReg;
	public:
		void initialize();
		void destroy();
};

static WASAPIInit wasapiinit;

extern bool bIsWin7, bIsVistaSP1;

void WASAPIInit::initialize() {
	wirReg = NULL;
	worReg = NULL;

	if (! bIsVistaSP1) {
		qWarning("WASAPIInit: Requires Vista SP1");
		return;
	}

	HMODULE hLib = LoadLibrary(L"AVRT.DLL");
	if (hLib == NULL) {
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
}

AudioInput *WASAPIInputRegistrar::create() {
	return new WASAPIInput();
}

const QList<audioDevice> WASAPIInputRegistrar::getDeviceChoices() {
	return WASAPISystem::mapToDevice(WASAPISystem::getInputDevices(), g.s.qsWASAPIInput);
}

void WASAPIInputRegistrar::setDeviceChoice(const QVariant &choice, Settings &s) {
	s.qsWASAPIInput = choice.toString();
}

bool WASAPIInputRegistrar::canEcho(const QString &outputsys) const {
	return (outputsys == name);
}

bool WASAPIInputRegistrar::canExclusive() const {
	return true;
}

WASAPIOutputRegistrar::WASAPIOutputRegistrar() : AudioOutputRegistrar(QLatin1String("WASAPI"), 10) {
}

AudioOutput *WASAPIOutputRegistrar::create() {
	return new WASAPIOutput();
}

const QList<audioDevice> WASAPIOutputRegistrar::getDeviceChoices() {
	return WASAPISystem::mapToDevice(WASAPISystem::getOutputDevices(), g.s.qsWASAPIOutput);
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

const QHash<QString, QString> WASAPISystem::getInputDevices() {
	return getDevices(eCapture);
}

const QHash<QString, QString> WASAPISystem::getOutputDevices() {
	return getDevices(eRender);
}

const QHash<QString, QString> WASAPISystem::getDevices(EDataFlow dataflow) {
	QHash<QString, QString> devices;

	HRESULT hr;

	IMMDeviceEnumerator *pEnumerator = NULL;
	IMMDeviceCollection *pCollection = NULL;

	hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), reinterpret_cast<void **>(&pEnumerator));

	if (! pEnumerator || FAILED(hr)) {
		qWarning("WASAPI: Failed to instatiate enumerator: hr=0x%08lx", hr);
	} else {
		hr = pEnumerator->EnumAudioEndpoints(dataflow, DEVICE_STATE_ACTIVE, &pCollection);
		if (! pCollection || FAILED(hr)) {
			qWarning("WASAPI: Failed to enumerate: hr=0x%08lx", hr);
		} else {
			devices.insert(QString(), tr("Default Device"));

			UINT ndev = 0;
			pCollection->GetCount(&ndev);
			for (unsigned int idx=0;idx<ndev;++idx) {
				IMMDevice *pDevice = NULL;
				IPropertyStore *pStore = NULL;

				pCollection->Item(idx, &pDevice);
				pDevice->OpenPropertyStore(STGM_READ, &pStore);

				LPWSTR strid = NULL;
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

const QList<audioDevice> WASAPISystem::mapToDevice(const QHash<QString, QString> devs, const QString &match) {
	QList<audioDevice> qlReturn;

	QStringList qlDevices = devs.keys();
	qSort(qlDevices);

	if (qlDevices.contains(match)) {
		qlDevices.removeAll(match);
		qlDevices.prepend(match);
	}

	foreach(const QString &dev, qlDevices) {
		qlReturn << audioDevice(devs.value(dev), dev);
	}
	return qlReturn;
}

WASAPIInput::WASAPIInput() {
};

WASAPIInput::~WASAPIInput() {
	bRunning = false;
	wait();
}

void WASAPIInput::run() {
	HRESULT hr;
	IMMDeviceEnumerator *pEnumerator = NULL;
	IMMDevice *pMicDevice = NULL;
	IAudioClient *pMicAudioClient = NULL;
	IAudioCaptureClient *pMicCaptureClient = NULL;
	IMMDevice *pEchoDevice = NULL;
	IAudioClient *pEchoAudioClient = NULL;
	IAudioCaptureClient *pEchoCaptureClient = NULL;
	WAVEFORMATEX *micpwfx = NULL, *echopwfx = NULL;
	WAVEFORMATEXTENSIBLE *micpwfxe = NULL, *echopwfxe = NULL;
	WAVEFORMATEXTENSIBLE wfe;
	UINT32 bufferFrameCount;
	UINT32 numFramesAvailable;
	UINT32 numFramesLeft;
	UINT32 micPacketLength = 0, echoPacketLength = 0;
	UINT32 allocLength;
	UINT64 devicePosition;
	UINT64 qpcPosition;
	HANDLE hEvent;
	BYTE *pData;
	DWORD flags;
	DWORD dwTaskIndex = 0;
	HANDLE hMmThread;
	float *tbuff = NULL;
	short *sbuff = NULL;
	bool doecho = g.s.doEcho();
	REFERENCE_TIME def, min, latency, want;
	bool exclusive = false;

	CoInitialize(NULL);

	hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	hMmThread = AvSetMmThreadCharacteristics(L"Pro Audio", &dwTaskIndex);
	if (hMmThread == NULL) {
		qWarning("WASAPIInput: Failed to set Pro Audio thread priority");
	}

	hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), reinterpret_cast<void **>(&pEnumerator));

	if (! pEnumerator || FAILED(hr)) {
		qWarning("WASAPIInput: Failed to instatiate enumerator: hr=0x%08lx", hr);
		return;
	}

	if (! g.s.qsWASAPIInput.isEmpty()) {
		STACKVAR(wchar_t, devname, g.s.qsWASAPIInput.length() + 1);
		int len = g.s.qsWASAPIInput.toWCharArray(devname);
		devname[len] = 0;
		hr = pEnumerator->GetDevice(devname, &pMicDevice);
		if (FAILED(hr)) {
			qWarning("WASAPIInput: Failed to open selected input device %s %ls (hr=0x%08lx), falling back to default", qPrintable(g.s.qsWASAPIInput), devname, hr);
		}
	}

	if (! pMicDevice) {
		hr = pEnumerator->GetDefaultAudioEndpoint(eCapture, eCommunications, &pMicDevice);
		if (FAILED(hr)) {
			qWarning("WASAPIInput: Failed to open input device: hr=0x%08lx", hr);
			goto cleanup;
		}
		wchar_t *devname = NULL;
		hr = pMicDevice->GetId(&devname);
		if (FAILED(hr)) {
			qWarning("WASAPIInput: Failed to query input device: hr=0x%08lx", hr);
			goto cleanup;
		}
		pMicDevice->Release();
		hr = pEnumerator->GetDevice(devname, &pMicDevice);
		if (FAILED(hr)) {
			qWarning("WASAPIInput: Failed to reopen default input device: hr=0x%08lx", hr);
			goto cleanup;
		}
		CoTaskMemFree(devname);
	}

	if (doecho) {
		if (! g.s.qsWASAPIOutput.isEmpty()) {
			STACKVAR(wchar_t, devname, g.s.qsWASAPIOutput.length());
			g.s.qsWASAPIOutput.toWCharArray(devname);
			hr = pEnumerator->GetDevice(devname, &pEchoDevice);
			if (FAILED(hr)) {
				qWarning("WASAPIInput: Failed to open selected echo device (hr=0x%08lx), falling back to default", hr);
			}
		}

		if (! pEchoDevice) {
			hr = pEnumerator->GetDefaultAudioEndpoint(eRender, eCommunications, &pEchoDevice);
			if (FAILED(hr)) {
				qWarning("WASAPIInput: Failed to open echo device: hr=0x%08lx", hr);
				goto cleanup;
			}
			wchar_t *devname = NULL;
			hr = pEchoDevice->GetId(&devname);
			if (FAILED(hr)) {
				qWarning("WASAPIInput: Failed to query echo device: hr=0x%08lx", hr);
				goto cleanup;
			}
			pEchoDevice->Release();
			hr = pEnumerator->GetDevice(devname, &pEchoDevice);
			if (FAILED(hr)) {
				qWarning("WASAPIInput: Failed to reopen default echo device: hr=0x%08lx", hr);
				goto cleanup;
			}
			CoTaskMemFree(devname);
		}
	}

	pEnumerator->Release();
	pEnumerator = NULL;

	hr = pMicDevice->Activate(__uuidof(IAudioClient), CLSCTX_ALL, NULL, (void **) &pMicAudioClient);
	if (FAILED(hr)) {
		qWarning("WASAPIInput: Activate Mic AudioClient failed: hr=0x%08lx", hr);
		goto cleanup;
	}

	def = min = latency = 0;

	pMicAudioClient->GetDevicePeriod(&def, &min);

	want = qMax<REFERENCE_TIME>(min, 100000);
	qWarning("WASAPIInput: Latencies %lld %lld => %lld", def, min, want);

	if (g.s.bExclusiveInput && ! doecho) {
		for (int channels = 1; channels<=2; ++channels) {
			ZeroMemory(&wfe, sizeof(wfe));
			wfe.Format.cbSize = 0;
			wfe.Format.wFormatTag = WAVE_FORMAT_PCM;
			wfe.Format.nChannels = channels;
			wfe.Format.nSamplesPerSec = 48000;
			wfe.Format.wBitsPerSample = 16;
			wfe.Format.nBlockAlign = wfe.Format.nChannels * wfe.Format.wBitsPerSample / 8;
			wfe.Format.nAvgBytesPerSec = wfe.Format.nBlockAlign * wfe.Format.nSamplesPerSec;

			micpwfxe = &wfe;
			micpwfx = reinterpret_cast<WAVEFORMATEX *>(&wfe);

			hr = pMicAudioClient->Initialize(AUDCLNT_SHAREMODE_EXCLUSIVE, AUDCLNT_STREAMFLAGS_EVENTCALLBACK, want, want, micpwfx, NULL);
			if (SUCCEEDED(hr)) {
				eMicFormat = SampleShort;
				exclusive = true;
				qWarning("WASAPIInput: Successfully opened exclusive mode");
				break;
			}

			micpwfxe = NULL;
			micpwfx = NULL;
		}
	}

	if (!  micpwfxe) {
		if (g.s.bExclusiveInput)
			qWarning("WASAPIInput: Failed to open exclusive mode.");

		hr = pMicAudioClient->GetMixFormat(&micpwfx);
		micpwfxe = reinterpret_cast<WAVEFORMATEXTENSIBLE *>(micpwfx);

		if (FAILED(hr) || (micpwfx->wBitsPerSample != (sizeof(float) * 8)) || (micpwfxe->SubFormat != KSDATAFORMAT_SUBTYPE_IEEE_FLOAT)) {
			qWarning("WASAPIInput: Mic Subformat is not IEEE Float: hr=0x%08lx", hr);
			goto cleanup;
		}

		hr = pMicAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, AUDCLNT_STREAMFLAGS_EVENTCALLBACK, 0, 0, micpwfx, NULL);
		if (FAILED(hr)) {
			qWarning("WASAPIInput: Mic Initialize failed: hr=0x%08lx", hr);
			goto cleanup;
		}
	}

	pMicAudioClient->GetStreamLatency(&latency);
	hr = pMicAudioClient->GetBufferSize(&bufferFrameCount);
	qWarning("WASAPIInput: Stream Latency %lld (%d)", latency, bufferFrameCount);

	hr = pMicAudioClient->GetService(__uuidof(IAudioCaptureClient), (void**)&pMicCaptureClient);
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
	iMicFreq = micpwfx->nSamplesPerSec;

	if (doecho) {
		hr = pEchoDevice->Activate(__uuidof(IAudioClient), CLSCTX_ALL, NULL, (void **) &pEchoAudioClient);
		if (FAILED(hr)) {
			qWarning("WASAPIInput: Activate Echo AudioClient failed: hr=0x%08lx", hr);
			goto cleanup;
		}

		hr = pEchoAudioClient->GetMixFormat(&echopwfx);
		echopwfxe = reinterpret_cast<WAVEFORMATEXTENSIBLE *>(echopwfx);

		if (FAILED(hr) || (echopwfx->wBitsPerSample != (sizeof(float) * 8)) || (echopwfxe->SubFormat != KSDATAFORMAT_SUBTYPE_IEEE_FLOAT)) {
			qWarning("WASAPIInput: Echo Subformat is not IEEE Float: hr=0x%08lx", hr);
			goto cleanup;
		}

		hr = pEchoAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, AUDCLNT_STREAMFLAGS_EVENTCALLBACK | AUDCLNT_STREAMFLAGS_LOOPBACK, 0, 0, echopwfx, NULL);
		if (FAILED(hr)) {
			qWarning("WASAPIInput: Echo Initialize failed: hr=0x%08lx", hr);
			goto cleanup;
		}

		hr = pEchoAudioClient->GetBufferSize(&bufferFrameCount);
		hr = pEchoAudioClient->GetService(__uuidof(IAudioCaptureClient), (void**)&pEchoCaptureClient);
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

		iEchoChannels = echopwfx->nChannels;
		iEchoFreq = echopwfx->nSamplesPerSec;
	}

	initializeMixer();

	allocLength = (iMicLength / 2) * micpwfx->nChannels;

	if (exclusive) {
		sbuff = new short[allocLength];
		while (bRunning && ! FAILED(hr)) {
			hr = pMicCaptureClient->GetBuffer(&pData, &numFramesAvailable, &flags, &devicePosition, &qpcPosition);
			if (hr != AUDCLNT_S_BUFFER_EMPTY) {
				if (FAILED(hr))
					goto cleanup;

				numFramesLeft = numFramesAvailable;

				UINT32 nFrames = numFramesAvailable * micpwfx->nChannels;
				if (nFrames > allocLength) {
					delete [] sbuff;
					allocLength = nFrames;
					sbuff = new short[allocLength];
				}

				memcpy(sbuff, pData, nFrames * sizeof(short));
				hr = pMicCaptureClient->ReleaseBuffer(numFramesAvailable);
				if (FAILED(hr))
					goto cleanup;
				addMic(sbuff, numFramesAvailable);
			}
			if (! FAILED(hr))
				WaitForSingleObject(hEvent, 100);
		}
	} else {
		tbuff = new float[allocLength];
		while (bRunning && ! FAILED(hr)) {
			hr = pMicCaptureClient->GetNextPacketSize(&micPacketLength);
			if (! FAILED(hr) && iEchoChannels)
				hr = pEchoCaptureClient->GetNextPacketSize(&echoPacketLength);
			if (FAILED(hr))
				goto cleanup;

			while ((micPacketLength > 0) || (echoPacketLength > 0)) {
				if (echoPacketLength > 0) {
					hr = pEchoCaptureClient->GetBuffer(&pData, &numFramesAvailable, &flags, &devicePosition, &qpcPosition);
					numFramesLeft = numFramesAvailable;
					if (FAILED(hr))
						goto cleanup;

					UINT32 nFrames = numFramesAvailable * echopwfx->nChannels;
					if (nFrames > allocLength) {
						delete [] tbuff;
						allocLength = nFrames;
						tbuff = new float[allocLength];
					}
					memcpy(tbuff, pData, nFrames * sizeof(float));
					hr = pEchoCaptureClient->ReleaseBuffer(numFramesAvailable);
					if (FAILED(hr))
						goto cleanup;
					addEcho(tbuff, numFramesAvailable);
				} else if (micPacketLength > 0) {
					hr = pMicCaptureClient->GetBuffer(&pData, &numFramesAvailable, &flags, &devicePosition, &qpcPosition);
					numFramesLeft = numFramesAvailable;
					if (FAILED(hr))
						goto cleanup;

					UINT32 nFrames = numFramesAvailable * micpwfx->nChannels;
					if (nFrames > allocLength) {
						delete [] tbuff;
						allocLength = nFrames;
						tbuff = new float[allocLength];
					}
					memcpy(tbuff, pData, nFrames * sizeof(float));
					hr = pMicCaptureClient->ReleaseBuffer(numFramesAvailable);
					if (FAILED(hr))
						goto cleanup;
					addMic(tbuff, numFramesAvailable);
				}
				hr = pMicCaptureClient->GetNextPacketSize(&micPacketLength);
				if (! FAILED(hr) && iEchoChannels)
					hr = pEchoCaptureClient->GetNextPacketSize(&echoPacketLength);
			}
			if (! FAILED(hr))
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

	if (pEnumerator)
		pEnumerator->Release();

	if (hMmThread != NULL)
		AvRevertMmThreadCharacteristics(hMmThread);

	if (hEvent != NULL)
		CloseHandle(hEvent);

	delete [] tbuff;
	delete [] sbuff;
}

WASAPIOutput::WASAPIOutput() {
}

WASAPIOutput::~WASAPIOutput() {
	bRunning = false;
	wait();
}

void WASAPIOutput::setVolumes(IMMDevice *pDevice, bool talking) {
	HRESULT hr;

	if (! talking) {
		QMap<ISimpleAudioVolume *, VolumePair>::const_iterator i;
		for (i=qmVolumes.constBegin(); i != qmVolumes.constEnd(); ++i) {
			float fVolume = 1.0f;
			hr = i.key()->GetMasterVolume(&fVolume);
			if (qFuzzyCompare(i.value().second, fVolume))
				hr = i.key()->SetMasterVolume(i.value().first, NULL);
			i.key()->Release();
		}
		qmVolumes.clear();
		return;
	}

	IAudioSessionManager2 *pAudioSessionManager = NULL;
	int max = 0;
	DWORD dwMumble = GetCurrentProcessId();

	qmVolumes.clear();
	if (qFuzzyCompare(g.s.fOtherVolume, 1.0f))
		return;

	// FIXME: Try to keep the session object around when returning volume.

	if (SUCCEEDED(hr = pDevice->Activate(bIsWin7 ? __uuidof(IAudioSessionManager2) : __uuidof(IAudioSessionManager), CLSCTX_ALL, NULL, (void **) &pAudioSessionManager))) {
		IAudioSessionEnumerator *pEnumerator = NULL;
		IAudioSessionQuery *pMysticQuery = NULL;
		if (! bIsWin7) {
			if (SUCCEEDED(hr = pAudioSessionManager->QueryInterface(__uuidof(IAudioSessionQuery), (void **) &pMysticQuery))) {
				hr = pMysticQuery->GetQueryInterface(&pEnumerator);
			}
		} else {
			hr = pAudioSessionManager->GetSessionEnumerator(&pEnumerator);
		}

		QSet<QUuid> seen;

		if (SUCCEEDED(hr)) {
			if (SUCCEEDED(hr = pEnumerator->GetCount(&max))) {
				for (int i=0;i<max;++i) {
					IAudioSessionControl *pControl = NULL;
					if (SUCCEEDED(hr = pEnumerator->GetSession(i, &pControl))) {
						IAudioSessionControl2 *pControl2 = NULL;
						if (SUCCEEDED(hr = pControl->QueryInterface(bIsWin7 ? __uuidof(IAudioSessionControl2) : __uuidof(IVistaAudioSessionControl2), (void **) &pControl2)))  {
							DWORD pid;
							if (SUCCEEDED(hr = pControl2->GetProcessId(&pid)) && (pid != dwMumble)) {
								AudioSessionState ass;
								if (SUCCEEDED(hr = pControl2->GetState(&ass)) && (ass != AudioSessionStateExpired)) {
									GUID group;
									if (SUCCEEDED(hr = pControl2->GetGroupingParam(&group))) {
										QUuid quuid(group);
										if (! seen.contains(quuid)) {
											seen.insert(quuid);
											ISimpleAudioVolume *pVolume = NULL;
											if (SUCCEEDED(hr = pControl2->QueryInterface(__uuidof(ISimpleAudioVolume), (void **) &pVolume))) {
												BOOL bMute = TRUE;
												bool keep = false;
												if (SUCCEEDED(hr = pVolume->GetMute(&bMute)) && ! bMute) {
													float fVolume = 1.0f;
													if (SUCCEEDED(hr = pVolume->GetMasterVolume(&fVolume)) && ! qFuzzyCompare(fVolume,0.0f)) {
														float fSetVolume = fVolume * g.s.fOtherVolume;
														if (SUCCEEDED(hr = pVolume->SetMasterVolume(fSetVolume, NULL))) {
															hr = pVolume->GetMasterVolume(&fSetVolume);
															qmVolumes.insert(pVolume, VolumePair(fVolume,fSetVolume));
															keep = true;
														}
													}
												}
												if (! keep)
													pVolume->Release();
											}
										}
									}
								}
							}
							pControl2->Release();
						}
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

void WASAPIOutput::run() {
	HRESULT hr;
	IMMDeviceEnumerator *pEnumerator = NULL;
	IMMDevice *pDevice = NULL;
	IAudioClient *pAudioClient = NULL;
	IAudioRenderClient *pRenderClient = NULL;
	WAVEFORMATEX *pwfx = NULL;
	WAVEFORMATEXTENSIBLE *pwfxe = NULL;
	UINT32 bufferFrameCount;
	REFERENCE_TIME def, min, latency, want;
	UINT32 numFramesAvailable;
	UINT32 wantLength;
	HANDLE hEvent;
	BYTE *pData;
	DWORD dwTaskIndex = 0;
	HANDLE hMmThread;
	int ns = 0;
	unsigned int chanmasks[32];
	QMap<DWORD, float> qmVolumes;
	bool lastspoke = false;
	REFERENCE_TIME bufferDuration = (g.s.iOutputDelay > 1) ? (g.s.iOutputDelay + 1) * 100000 : 0;
	bool exclusive = false;

	CoInitialize(NULL);

	hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	hMmThread = AvSetMmThreadCharacteristics(L"Pro Audio", &dwTaskIndex);
	if (hMmThread == NULL) {
		qWarning("WASAPIOutput: Failed to set Pro Audio thread priority");
	}

	hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), reinterpret_cast<void **>(&pEnumerator));

	if (! pEnumerator || FAILED(hr)) {
		qWarning("WASAPIOutput: Failed to instatiate enumerator: hr=0x%08lx", hr);
		return;
	}

	if (! g.s.qsWASAPIOutput.isEmpty()) {
		STACKVAR(wchar_t, devname, g.s.qsWASAPIOutput.length() + 1);
		int len = g.s.qsWASAPIOutput.toWCharArray(devname);
		devname[len] = 0;
		hr = pEnumerator->GetDevice(devname, &pDevice);
		if (FAILED(hr)) {
			qWarning("WASAPIOutput: Failed to open selected input device (hr=0x%08lx), falling back to default", hr);
		}
	}

	if (! pDevice) {
		hr = pEnumerator->GetDefaultAudioEndpoint(eRender, eCommunications, &pDevice);
		if (FAILED(hr)) {
			qWarning("WASAPIOutput: Failed to open output device: hr=0x%08lx", hr);
			goto cleanup;
		}
		wchar_t *devname = NULL;
		hr = pDevice->GetId(&devname);
		if (FAILED(hr)) {
			qWarning("WASAPIOutput: Failed to query output device: hr=0x%08lx", hr);
			goto cleanup;
		}
		pDevice->Release();
		hr = pEnumerator->GetDevice(devname, &pDevice);
		if (FAILED(hr)) {
			qWarning("WASAPIOutput: Failed to reopen default output device: hr=0x%08lx", hr);
			goto cleanup;
		}
		CoTaskMemFree(devname);
	}

	pEnumerator->Release();
	pEnumerator = NULL;

	hr = pDevice->Activate(__uuidof(IAudioClient), CLSCTX_ALL, NULL, (void **) &pAudioClient);
	if (FAILED(hr)) {
		qWarning("WASAPIOutput: Activate AudioClient failed: hr=0x%08lx", hr);
		goto cleanup;
	}

	pAudioClient->GetDevicePeriod(&def, &min);
	want = qMax<REFERENCE_TIME>(min, 100000);
	qWarning("WASAPIOutput: Latencies %lld %lld => %lld", def, min, want);

	if (g.s.bExclusiveOutput) {
		hr = pAudioClient->GetMixFormat(&pwfx);
		if (FAILED(hr)) {
			qWarning("WASAPIOutput: GetMixFormat failed: hr=0x%08lx", hr);
			goto cleanup;
		}

		pwfx->cbSize = 0;
		pwfx->wFormatTag = WAVE_FORMAT_PCM;
		pwfx->nSamplesPerSec = 48000;
		pwfx->wBitsPerSample = 16;
		pwfx->nBlockAlign = pwfx->nChannels * pwfx->wBitsPerSample / 8;
		pwfx->nAvgBytesPerSec = pwfx->nBlockAlign * pwfx->nSamplesPerSec;

		pwfxe = reinterpret_cast<WAVEFORMATEXTENSIBLE *>(pwfx);

		hr = pAudioClient->Initialize(AUDCLNT_SHAREMODE_EXCLUSIVE, AUDCLNT_STREAMFLAGS_EVENTCALLBACK, want, want, pwfx, NULL);
		if (SUCCEEDED(hr)) {
			eSampleFormat = SampleShort;
			exclusive = true;
			qWarning("WASAPIOutput: Successfully opened exclusive mode");
		} else {
			CoTaskMemFree(pwfx);

			pwfxe = NULL;
			pwfx = NULL;
		}
	}

	if (!  pwfxe) {
		if (g.s.bExclusiveOutput)
			qWarning("WASAPIOutput: Failed to open exclusive mode.");

		hr = pAudioClient->GetMixFormat(&pwfx);
		if (FAILED(hr)) {
			qWarning("WASAPIOutput: GetMixFormat failed: hr=0x%08lx", hr);
			goto cleanup;
		}
		pwfxe = reinterpret_cast<WAVEFORMATEXTENSIBLE *>(pwfx);

		if (FAILED(hr) || (pwfx->wBitsPerSample != (sizeof(float) * 8)) || (pwfxe->SubFormat != KSDATAFORMAT_SUBTYPE_IEEE_FLOAT)) {
			qWarning("WASAPIOutput: Subformat is not IEEE Float: hr=0x%08lx", hr);
			goto cleanup;
		}

		hr = pAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, AUDCLNT_STREAMFLAGS_EVENTCALLBACK, bufferDuration, 0, pwfx, NULL);
		if (FAILED(hr)) {
			qWarning("WASAPIOutput: Initialize failed: hr=0x%08lx", hr);
			goto cleanup;
		}
	}

	pAudioClient->GetStreamLatency(&latency);
	pAudioClient->GetBufferSize(&bufferFrameCount);
	qWarning("WASAPIOutput: Stream Latency %lld (%d)", latency, bufferFrameCount);

	iMixerFreq = pwfx->nSamplesPerSec;

	qWarning("WASAPIOutput: Periods %lldus %lldus (latency %lldus)", def / 10LL, min / 10LL, latency / 10LL);
	qWarning("WASAPIOutput: Buffer is %dus (%d)", (bufferFrameCount * 1000000) / iMixerFreq, g.s.iOutputDelay);

	hr = pAudioClient->GetService(__uuidof(IAudioRenderClient), (void**)&pRenderClient);
	if (FAILED(hr)) {
		qWarning("WASAPIOutput: GetService failed: hr=0x%08lx", hr);
		goto cleanup;
	}

	wantLength = lroundf(ceilf((iFrameSize * pwfx->nSamplesPerSec) / (SAMPLE_RATE * 1.0f)));

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

	for (int i=0;i<32;i++) {
		if (pwfxe->dwChannelMask & (1 << i)) {
			chanmasks[ns++] = 1 << i;
		}
	}
	if (ns != pwfx->nChannels) {
		qWarning("WASAPIOutput: Chanmask bits doesn't match number of channels.");
	}

	iChannels = pwfx->nChannels;
	initializeMixer(chanmasks);

	bool mixed = false;
	if (! exclusive) {
		while (bRunning && ! FAILED(hr)) {
			hr = pAudioClient->GetCurrentPadding(&numFramesAvailable);
			if (FAILED(hr))
				goto cleanup;

			UINT32 packetLength = bufferFrameCount - numFramesAvailable;

			if (lastspoke != (g.bAttenuateOthers || mixed)) {
				lastspoke = g.bAttenuateOthers || mixed;
				setVolumes(pDevice, lastspoke);
			}

			while (packetLength > 0) {
				hr = pRenderClient->GetBuffer(packetLength, &pData);
				if (FAILED(hr))
					goto cleanup;

				mixed = mix(reinterpret_cast<float *>(pData), packetLength);
				if (mixed)
					hr = pRenderClient->ReleaseBuffer(packetLength, 0);
				else
					hr = pRenderClient->ReleaseBuffer(packetLength, AUDCLNT_BUFFERFLAGS_SILENT);
				if (FAILED(hr))
					goto cleanup;

				if (!g.s.bAttenuateOthers && !g.bAttenuateOthers) {
					mixed = false;
				}

				if (lastspoke != (g.bAttenuateOthers || mixed)) {
					lastspoke = g.bAttenuateOthers || mixed;
					setVolumes(pDevice, lastspoke);
				}

				hr = pAudioClient->GetCurrentPadding(&numFramesAvailable);
				if (FAILED(hr))
					goto cleanup;

				packetLength = bufferFrameCount - numFramesAvailable;
			}
			if (! FAILED(hr))
				WaitForSingleObject(hEvent, 2000);
		}
	} else {
		wantLength = bufferFrameCount;

		while (bRunning && ! FAILED(hr)) {
			hr = pRenderClient->GetBuffer(wantLength, &pData);
			if (FAILED(hr))
				goto cleanup;

			mixed = mix(pData, wantLength);

			if (mixed)
				hr = pRenderClient->ReleaseBuffer(wantLength, 0);
			else
				hr = pRenderClient->ReleaseBuffer(wantLength, AUDCLNT_BUFFERFLAGS_SILENT);

			if (FAILED(hr))
				goto cleanup;

			if (! FAILED(hr))
				WaitForSingleObject(hEvent, 100);
		}
	}

cleanup:
	if (pwfx)
		CoTaskMemFree(pwfx);

	setVolumes(pDevice, false);

	if (pAudioClient) {
		pAudioClient->Stop();
		pAudioClient->Release();
	}
	if (pRenderClient)
		pRenderClient->Release();
	if (pDevice)
		pDevice->Release();

	if (pEnumerator)
		pEnumerator->Release();

	if (hMmThread != NULL)
		AvRevertMmThreadCharacteristics(hMmThread);

	if (hEvent != NULL)
		CloseHandle(hEvent);
}
