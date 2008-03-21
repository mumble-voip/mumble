/* Copyright (C) 2005-2008, Thorvald Natvig <thorvald@natvig.com>

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
#include "Player.h"
#include "Global.h"
#include "MainWindow.h"
#include "Timer.h"

#define MAX(a,b)        ( (a) > (b) ? (a) : (b) )
#define MIN(a,b)        ( (a) < (b) ? (a) : (b) )

class WASAPIInputRegistrar : public AudioInputRegistrar {
	public:
		WASAPIInputRegistrar();
		virtual AudioInput *create();
		virtual const QList<audioDevice> getDeviceChoices();
		virtual void setDeviceChoice(const QVariant &);
};

class WASAPIOutputRegistrar : public AudioOutputRegistrar {
	public:
		WASAPIOutputRegistrar();
		virtual AudioOutput *create();
		virtual const QList<audioDevice> getDeviceChoices();
		virtual void setDeviceChoice(const QVariant &);
};

static ConfigWidget *WASAPIConfigDialogNew(Settings &st) {
	return new WASAPIConfig(st);
}

class WASAPIInit : public DeferInit {
		ConfigRegistrar *confReg;
		WASAPIInputRegistrar *wirReg;
		WASAPIOutputRegistrar *worReg;
	public:
		void initialize();
		void destroy();
};

static WASAPIInit wasapiinit;

void WASAPIInit::initialize() {
	confReg = NULL;
	wirReg = NULL;
	worReg = NULL;

	OSVERSIONINFOEXW ovi;
	memset(&ovi, 0, sizeof(ovi));

	ovi.dwOSVersionInfoSize=sizeof(ovi);
	GetVersionEx(reinterpret_cast<OSVERSIONINFOW *>(&ovi));

	if ((ovi.dwMajorVersion < 6) || (ovi.dwBuildNumber < 6001)) {
		qWarning("WASAPIInit: Requires Vista SP1");
		return;
	}

	HMODULE hLib = LoadLibrary(L"AVRT.DLL");
	if (hLib == NULL) {
		qWarning("WASAPIInit: Failed to load avrt.dll");
		return;
	}
	FreeLibrary(hLib);

	confReg = new ConfigRegistrar(23, WASAPIConfigDialogNew);
	wirReg = new WASAPIInputRegistrar();
	worReg = new WASAPIOutputRegistrar();
}

void WASAPIInit::destroy() {
	if (confReg)
		delete confReg;
	if (wirReg)
		delete wirReg;
	if (worReg)
		delete worReg;
}


WASAPIInputRegistrar::WASAPIInputRegistrar() : AudioInputRegistrar(QLatin1String("WASAPI")) {
}

AudioInput *WASAPIInputRegistrar::create() {
	return new WASAPIInput();
}

const QList<audioDevice> WASAPIInputRegistrar::getDeviceChoices() {
	return WASAPISystem::mapToDevice(WASAPISystem::getInputDevices(), g.s.qsWASAPIOutput);
}

void WASAPIInputRegistrar::setDeviceChoice(const QVariant &choice) {
	g.s.qsWASAPIInput = choice.toString();
}

WASAPIOutputRegistrar::WASAPIOutputRegistrar() : AudioOutputRegistrar(QLatin1String("WASAPI")) {
}

AudioOutput *WASAPIOutputRegistrar::create() {
	return new WASAPIOutput();
}

const QList<audioDevice> WASAPIOutputRegistrar::getDeviceChoices() {
	return WASAPISystem::mapToDevice(WASAPISystem::getOutputDevices(), g.s.qsWASAPIOutput);
}

void WASAPIOutputRegistrar::setDeviceChoice(const QVariant &choice) {
	g.s.qsWASAPIOutput = choice.toString();
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
		qWarning("WASAPI: Failed to instatiate enumerator");
	} else {
		hr = pEnumerator->EnumAudioEndpoints(dataflow, DEVICE_STATE_ACTIVE, &pCollection);
		if (! pCollection || FAILED(hr)) {
			qWarning("WASAPI: Failed to enumerate");
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


WASAPIConfig::WASAPIConfig(Settings &st) : ConfigWidget(st) {
	setupUi(this);

	const QHash<QString, QString> qhInput = WASAPISystem::getInputDevices();
	const QHash<QString, QString> qhOutput = WASAPISystem::getOutputDevices();

	QStringList qlOutputDevs = qhOutput.keys();
	qSort(qlOutputDevs);
	QStringList qlInputDevs = qhInput.keys();
	qSort(qlInputDevs);

	foreach(QString dev, qlInputDevs) {
		qcbInputDevice->addItem(qhInput.value(dev), dev);
	}

	foreach(QString dev, qlOutputDevs) {
		qcbOutputDevice->addItem(qhOutput.value(dev), dev);
	}
}

QString WASAPIConfig::title() const {
	return tr("WASAPI");
}

QIcon WASAPIConfig::icon() const {
	return QIcon(QLatin1String("skin:config_dsound.png"));
}

void WASAPIConfig::save() const {
	s.qsWASAPIInput =  qcbInputDevice->itemData(qcbInputDevice->currentIndex()).toString();
	s.qsWASAPIOutput =  qcbOutputDevice->itemData(qcbOutputDevice->currentIndex()).toString();
	s.bWASAPIEcho = qcbEcho->isChecked();
}

void WASAPIConfig::load(const Settings &r) {
	for (int i=0;i<qcbInputDevice->count();i++) {
		if (qcbInputDevice->itemData(i).toString() == r.qsWASAPIInput) {
			loadComboBox(qcbInputDevice, i);
			break;
		}
	}

	for (int i=0;i<qcbOutputDevice->count();i++) {
		if (qcbOutputDevice->itemData(i).toString() == r.qsWASAPIOutput) {
			loadComboBox(qcbOutputDevice, i);
			break;
		}
	}
	loadCheckBox(qcbEcho, r.bWASAPIEcho);
}

bool WASAPIConfig::expert(bool b) {
	return true;
}

WASAPIInput::WASAPIInput() {
	bRunning = true;
	bHasSpeaker = g.s.bWASAPIEcho;
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
	UINT32 bufferFrameCount;
	REFERENCE_TIME hnsRequestedDuration = 200 * 10000;
	SpeexResamplerState *srsMic = NULL, *srsEcho = NULL;
	UINT32 numFramesAvailable;
	UINT32 numFramesLeft;
	UINT32 micPacketLength = 0, echoPacketLength = 0;
	UINT32 micWantLength, echoWantLength;
	UINT32 allocLength;
	UINT64 devicePosition;
	UINT64 qpcPosition;
	HANDLE hEvent;
	BYTE *pData;
	DWORD flags;
	int err = 0;
	DWORD micGotLength = 0, echoGotLength = 0;
	DWORD dwTaskIndex = 0;
	HANDLE hMmThread;
	float *micInputBuffer = NULL, *echoInputBuffer = NULL, *outputBuffer = NULL;
	float micmul = 0.0f;
	float echomul = 0.0f;
	float *tbuff = NULL;
	JitterBuffer *jbJitter = NULL;
	int iEchoSeq = 0;
	spx_int32_t startofs;
	JitterBufferPacket jbp;

	CoInitialize(NULL);

	hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	hMmThread = AvSetMmThreadCharacteristics(L"Pro Audio", &dwTaskIndex);
	if (hMmThread == NULL) {
		qWarning("WASAPIInput: Failed to set Pro Audio thread priority");
	}

	hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), reinterpret_cast<void **>(&pEnumerator));

	if (! pEnumerator || FAILED(hr)) {
		qWarning("WASAPIInput: Failed to instatiate enumerator");
		return;
	}

	if (! g.s.qsWASAPIInput.isEmpty()) {
		STACKVAR(wchar_t, devname, g.s.qsWASAPIInput.length());
		g.s.qsWASAPIInput.toWCharArray(devname);
		hr = pEnumerator->GetDevice(devname, &pMicDevice);
		if (FAILED(hr)) {
			qWarning("WASAPIInput: Failed to open selected input device, falling back to default");
		}
	}

	if (! pMicDevice) {
		hr = pEnumerator->GetDefaultAudioEndpoint(eCapture, eCommunications, &pMicDevice);
		if (FAILED(hr)) {
			qWarning("WASAPIInput: Failed to open input device");
			goto cleanup;
		}
	}

	if (bHasSpeaker) {
		if (! g.s.qsWASAPIOutput.isEmpty()) {
			STACKVAR(wchar_t, devname, g.s.qsWASAPIOutput.length());
			g.s.qsWASAPIOutput.toWCharArray(devname);
			hr = pEnumerator->GetDevice(devname, &pEchoDevice);
			if (FAILED(hr)) {
				qWarning("WASAPIInput: Failed to open selected echo device, falling back to default");
			}
		}

		if (! pEchoDevice) {
			hr = pEnumerator->GetDefaultAudioEndpoint(eRender, eCommunications, &pEchoDevice);
			if (FAILED(hr)) {
				qWarning("WASAPIInput: Failed to open echo device");
				goto cleanup;
			}
		}
	}

	pEnumerator->Release();
	pEnumerator = NULL;

	hr = pMicDevice->Activate(__uuidof(IAudioClient), CLSCTX_ALL, NULL, (void **) &pMicAudioClient);
	if (FAILED(hr)) {
		qWarning("WASAPIInput: Activate Mic AudioClient failed");
		goto cleanup;
	}

	hr = pMicAudioClient->GetMixFormat(&micpwfx);
	micpwfxe = reinterpret_cast<WAVEFORMATEXTENSIBLE *>(micpwfx);

	if (FAILED(hr) || (micpwfx->wBitsPerSample != (sizeof(float) * 8)) || (micpwfxe->SubFormat != KSDATAFORMAT_SUBTYPE_IEEE_FLOAT)) {
		qWarning("WASAPIInput: Mic Subformat is not IEEE Float");
		goto cleanup;
	}

	hr = pMicAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, AUDCLNT_STREAMFLAGS_EVENTCALLBACK, hnsRequestedDuration, 0, micpwfx, NULL);
	if (FAILED(hr)) {
		qWarning("WASAPIInput: Mic Initialize failed");
		goto cleanup;
	}

	hr = pMicAudioClient->GetBufferSize(&bufferFrameCount);
	hr = pMicAudioClient->GetService(__uuidof(IAudioCaptureClient), (void**)&pMicCaptureClient);
	if (FAILED(hr)) {
		qWarning("WASAPIInput: Mic GetService failed");
		goto cleanup;
	}

	srsMic = speex_resampler_init(1, micpwfx->nSamplesPerSec, 16000, 3, &err);
	micWantLength = (iFrameSize * micpwfx->nSamplesPerSec) / 16000;

	pMicAudioClient->SetEventHandle(hEvent);
	if (FAILED(hr)) {
		qWarning("WASAPIInput: Failed to set mic event");
		goto cleanup;
	}

	hr = pMicAudioClient->Start();
	if (FAILED(hr)) {
		qWarning("WASAPIInput: Failed to start mic");
		goto cleanup;
	}

	micmul = 32768.0f / micpwfx->nChannels;

	micInputBuffer = new float[micWantLength];

	if (bHasSpeaker) {
		hr = pEchoDevice->Activate(__uuidof(IAudioClient), CLSCTX_ALL, NULL, (void **) &pEchoAudioClient);
		if (FAILED(hr)) {
			qWarning("WASAPIInput: Activate Echo AudioClient failed");
			goto cleanup;
		}

		hr = pEchoAudioClient->GetMixFormat(&echopwfx);
		echopwfxe = reinterpret_cast<WAVEFORMATEXTENSIBLE *>(echopwfx);

		if (FAILED(hr) || (echopwfx->wBitsPerSample != (sizeof(float) * 8)) || (echopwfxe->SubFormat != KSDATAFORMAT_SUBTYPE_IEEE_FLOAT)) {
			qWarning("WASAPIInput: Echo Subformat is not IEEE Float");
			goto cleanup;
		}

		hr = pEchoAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, AUDCLNT_STREAMFLAGS_EVENTCALLBACK | AUDCLNT_STREAMFLAGS_LOOPBACK, hnsRequestedDuration, 0, echopwfx, NULL);
		if (FAILED(hr)) {
			qWarning("WASAPIInput: Echo Initialize failed");
			goto cleanup;
		}

		hr = pEchoAudioClient->GetBufferSize(&bufferFrameCount);
		hr = pEchoAudioClient->GetService(__uuidof(IAudioCaptureClient), (void**)&pEchoCaptureClient);
		if (FAILED(hr)) {
			qWarning("WASAPIInput: Echo GetService failed");
			goto cleanup;
		}

		srsEcho = speex_resampler_init(1, echopwfx->nSamplesPerSec, 16000, 3, &err);
		echoWantLength = (iFrameSize * echopwfx->nSamplesPerSec) / 16000;

		pEchoAudioClient->SetEventHandle(hEvent);
		if (FAILED(hr)) {
			qWarning("WASAPIInput: Failed to set echo event");
			goto cleanup;
		}

		hr = pEchoAudioClient->Start();
		if (FAILED(hr)) {
			qWarning("WASAPIInput: Failed to start Echo");
			goto cleanup;
		}

		echomul = 32768.0f / echopwfx->nChannels;
		echoInputBuffer = new float[echoWantLength];

		jbJitter = jitter_buffer_init(iFrameSize);
	}

	outputBuffer = new float[iFrameSize];

	allocLength = (micWantLength / 2) * micpwfx->nChannels;
	tbuff = new float[allocLength];

	while (bRunning && ! FAILED(hr)) {
		hr = pMicCaptureClient->GetNextPacketSize(&micPacketLength);
		if (! FAILED(hr) && bHasSpeaker)
			hr = pEchoCaptureClient->GetNextPacketSize(&echoPacketLength);
		if (FAILED(hr))
			goto cleanup;

		while((micPacketLength > 0) || (echoPacketLength > 0)) {
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
				float *inData = tbuff;
				memcpy(inData, pData, nFrames * sizeof(float));
				hr = pEchoCaptureClient->ReleaseBuffer(numFramesAvailable);
				if (FAILED(hr))
					goto cleanup;
				while (numFramesLeft) {
					float v = 0.0f;
					for (int i=0;i<echopwfx->nChannels;i++) {
						v+= *inData;
						++inData;
					}
					echoInputBuffer[echoGotLength] = v;
					++echoGotLength;

					if (echoGotLength == echoWantLength) {
						spx_uint32_t inlen = echoWantLength;
						spx_uint32_t outlen = iFrameSize;
						speex_resampler_process_float(srsEcho, 0, echoInputBuffer, &inlen, outputBuffer, &outlen);

						jbp.data = reinterpret_cast<char *>(outputBuffer);
						jbp.len = iFrameSize * sizeof(float);
						jbp.span = iFrameSize;
						jbp.timestamp = iFrameSize * (++iEchoSeq);
						jbp.sequence = iEchoSeq;
						jbp.user_data = NULL;
						jitter_buffer_put(jbJitter, &jbp);
						echoGotLength = 0;
					}
					--numFramesLeft;
				}
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
				float *inData = tbuff;
				memcpy(inData, pData, nFrames * sizeof(float));
				hr = pMicCaptureClient->ReleaseBuffer(numFramesAvailable);
				if (FAILED(hr))
					goto cleanup;
				while (numFramesLeft) {
					float v = 0.0f;
					for (int i=0;i<micpwfx->nChannels;i++) {
						v+= *inData;
						++inData;
					}
					micInputBuffer[micGotLength] = v;
					++micGotLength;

					if (micGotLength == micWantLength) {
						spx_uint32_t inlen = micWantLength;
						spx_uint32_t outlen = iFrameSize;
						speex_resampler_process_float(srsMic, 0, micInputBuffer, &inlen, outputBuffer, &outlen);
						for (int i=0;i<iFrameSize;i++)
							psMic[i] = static_cast<short>(outputBuffer[i] * micmul);

						if (bHasSpeaker) {
							jbp.data = reinterpret_cast<char *>(outputBuffer);
							jbp.len = iFrameSize * sizeof(float);
							jbp.span = iFrameSize;
							jbp.timestamp = 0;
							jbp.sequence = 0;
							jitter_buffer_get(jbJitter, &jbp, iFrameSize, &startofs);
							jitter_buffer_update_delay(jbJitter, &jbp, NULL);
							jitter_buffer_tick(jbJitter);
							for(int i=0;i<iFrameSize;i++)
								psSpeaker[i] = static_cast<short>(outputBuffer[i] * echomul);
						}

						encodeAudioFrame();
						micGotLength = 0;
					}
					--numFramesLeft;
				}
			}
			hr = pMicCaptureClient->GetNextPacketSize(&micPacketLength);
			if (! FAILED(hr) && bHasSpeaker)
				hr = pEchoCaptureClient->GetNextPacketSize(&echoPacketLength);
		}
		if (! FAILED(hr))
			WaitForSingleObject(hEvent, 2000);
	}

cleanup:
	if (micpwfx)
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
	if (srsMic)
		speex_resampler_destroy(srsMic);

	if (pEchoAudioClient) {
		pEchoAudioClient->Stop();
		pEchoAudioClient->Release();
	}
	if (pEchoCaptureClient)
		pEchoCaptureClient->Release();
	if (pEchoDevice)
		pEchoDevice->Release();
	if (srsEcho)
		speex_resampler_destroy(srsEcho);

	if (pEnumerator)
		pEnumerator->Release();

	if (hMmThread != NULL)
		AvRevertMmThreadCharacteristics(hMmThread);

	if (hEvent != NULL)
		CloseHandle(hEvent);

	if (jbJitter)
		jitter_buffer_destroy(jbJitter);

	if (micInputBuffer)
		delete [] micInputBuffer;
	if (echoInputBuffer)
		delete [] echoInputBuffer;
	if (outputBuffer)
		delete [] outputBuffer;
	if (tbuff)
		delete [] tbuff;
}

WASAPIOutput::WASAPIOutput() {
	bRunning = true;
}

WASAPIOutput::~WASAPIOutput() {
	bRunning = false;
	wait();
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
	REFERENCE_TIME hnsRequestedDuration = g.s.iDXOutputDelay * 20 * 10000;
	SpeexResamplerState *srs;
	UINT32 numFramesAvailable;
	UINT32 numFramesLeft;
	UINT32 packetLength = 0;
	UINT32 wantLength;
	UINT32 allocLength;
	UINT64 devicePosition;
	UINT64 qpcPosition;
	HANDLE hEvent;
	BYTE *pData;
	DWORD flags;
	int err = 0;
	DWORD gotLength = 0;
	DWORD dwTaskIndex = 0;
	HANDLE hMmThread;
	float *inputBuffer = NULL, *outputBuffer = NULL;
	float mul = 0.0f;
	short *mixBuffer = NULL;

	CoInitialize(NULL);

	hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	hMmThread = AvSetMmThreadCharacteristics(L"Pro Audio", &dwTaskIndex);
	if (hMmThread == NULL) {
		qWarning("WASAPIOutput: Failed to set Pro Audio thread priority");
	}

	hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), reinterpret_cast<void **>(&pEnumerator));

	if (! pEnumerator || FAILED(hr)) {
		qWarning("WASAPIOutput: Failed to instatiate enumerator");
		return;
	}

	if (! g.s.qsWASAPIOutput.isEmpty()) {
		STACKVAR(wchar_t, devname, g.s.qsWASAPIOutput.length());
		g.s.qsWASAPIOutput.toWCharArray(devname);
		hr = pEnumerator->GetDevice(devname, &pDevice);
		if (FAILED(hr)) {
			qWarning("WASAPIOutput: Failed to open selected input device, falling back to default");
		}
	}

	if (! pDevice) {
		hr = pEnumerator->GetDefaultAudioEndpoint(eRender, eCommunications, &pDevice);
		if (FAILED(hr)) {
			qWarning("WASAPIOutput: Failed to open output device");
			goto cleanup;
		}
	}

	pEnumerator->Release();
	pEnumerator = NULL;

	hr = pDevice->Activate(__uuidof(IAudioClient), CLSCTX_ALL, NULL, (void **) &pAudioClient);
	if (FAILED(hr)) {
		qWarning("WASAPIOutput: Activate AudioClient failed");
		goto cleanup;
	}

	hr = pAudioClient->GetMixFormat(&pwfx);
	pwfxe = reinterpret_cast<WAVEFORMATEXTENSIBLE *>(pwfx);

	if (FAILED(hr) || (pwfx->wBitsPerSample != (sizeof(float) * 8)) || (pwfxe->SubFormat != KSDATAFORMAT_SUBTYPE_IEEE_FLOAT)) {
		qWarning("WASAPIOutput: Subformat is not IEEE Float");
		goto cleanup;
	}

	hr = pAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, AUDCLNT_STREAMFLAGS_EVENTCALLBACK, hnsRequestedDuration, 0, pwfx, NULL);
	if (FAILED(hr)) {
		qWarning("WASAPIOutput: Initialize failed");
		goto cleanup;
	}

	hr = pAudioClient->GetBufferSize(&bufferFrameCount);
	hr = pAudioClient->GetService(__uuidof(IAudioRenderClient), (void**)&pRenderClient);
	if (FAILED(hr)) {
		qWarning("WASAPIOutput: GetService failed");
		goto cleanup;
	}

	srs = speex_resampler_init(1, 16000, pwfx->nSamplesPerSec, 3, &err);
	wantLength = (iFrameSize * pwfx->nSamplesPerSec) / 16000;

	pAudioClient->SetEventHandle(hEvent);
	if (FAILED(hr)) {
		qWarning("WASAPIOutput: Failed to set event");
		goto cleanup;
	}

	hr = pAudioClient->Start();
	if (FAILED(hr)) {
		qWarning("WASAPIOutput: Failed to start");
		goto cleanup;
	}

	mul = 1.0f / 32768.0f;

	inputBuffer = new float[iFrameSize];
	mixBuffer = new short[iFrameSize];
	outputBuffer = new float[wantLength];

	while (bRunning && ! FAILED(hr)) {
		hr = pAudioClient->GetCurrentPadding(&numFramesAvailable);
		if (FAILED(hr))
			goto cleanup;

		packetLength = bufferFrameCount - numFramesAvailable;

		while(packetLength >= wantLength) {
			hr = pRenderClient->GetBuffer(wantLength, &pData);
			if (FAILED(hr))
				goto cleanup;

			bool mixed = mixAudio(mixBuffer);
			if (mixed) {
				for(int i=0;i<iFrameSize;i++)
					inputBuffer[i] = mixBuffer[i] * mul;

				spx_uint32_t inlen = iFrameSize;
				spx_uint32_t outlen = wantLength;
				speex_resampler_process_float(srs, 0, inputBuffer, &inlen, outputBuffer, &outlen);

				float *outData = reinterpret_cast<float *>(pData);
				for(int i=0;i<wantLength;i++)
					for(int j=0;j<pwfx->nChannels;j++)
						outData[i * pwfx->nChannels + j] = outputBuffer[i];

				hr = pRenderClient->ReleaseBuffer(wantLength, 0);
				if (FAILED(hr))
					goto cleanup;
			} else {
				hr = pRenderClient->ReleaseBuffer(wantLength, AUDCLNT_BUFFERFLAGS_SILENT);
				if (FAILED(hr))
					goto cleanup;
			}
			hr = pAudioClient->GetCurrentPadding(&numFramesAvailable);
			if (FAILED(hr))
				goto cleanup;

			packetLength = bufferFrameCount - numFramesAvailable;
		}
		if (! FAILED(hr))
			WaitForSingleObject(hEvent, 2000);
	}

cleanup:
	if (pwfx)
		CoTaskMemFree(pwfx);

	if (pAudioClient) {
		pAudioClient->Stop();
		pAudioClient->Release();
	}
	if (pRenderClient)
		pRenderClient->Release();
	if (pDevice)
		pDevice->Release();
	if (srs)
		speex_resampler_destroy(srs);

	if (pEnumerator)
		pEnumerator->Release();

	if (hMmThread != NULL)
		AvRevertMmThreadCharacteristics(hMmThread);

	if (hEvent != NULL)
		CloseHandle(hEvent);

	if (inputBuffer)
		delete [] inputBuffer;
	if (mixBuffer)
		delete [] mixBuffer;
	if (outputBuffer)
		delete [] outputBuffer;
}
