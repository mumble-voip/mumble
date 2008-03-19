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
			for(unsigned int idx=0;idx<ndev;++idx) {
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
}

void WASAPIInput::run() {
	HRESULT hr;
	IMMDeviceEnumerator *pEnumerator = NULL;
	IMMDevice *pDevice = NULL;
	IAudioClient *pAudioClient = NULL;
	IAudioCaptureClient *pCaptureClient = NULL;
	WAVEFORMATEX *pwfx = NULL;
	WAVEFORMATEXTENSIBLE *pwfxe = NULL;
	UINT32 bufferFrameCount;
	REFERENCE_TIME hnsRequestedDuration = 20 * 10000;
	SpeexResamplerState *srs = NULL;
	UINT32 numFramesAvailable;
	UINT32 numFramesLeft;
	UINT32 packetLength;
	UINT32 wantLength;
	HANDLE hEvent;
	BYTE *pData;
 	DWORD flags;
 	int err = 0;
	DWORD gotLength = 0;
	DWORD dwTaskIndex = 0;
	HANDLE hMmThread;

	CoInitialize(NULL);

	hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), reinterpret_cast<void **>(&pEnumerator));

	if (! pEnumerator || FAILED(hr)) {
		qWarning("WASAPIInput: Failed to instatiate enumerator");
		return;
	}

	if (! g.s.qsWASAPIInput.isEmpty()) {
		STACKVAR(wchar_t, devname, g.s.qsWASAPIInput.length());
		g.s.qsWASAPIInput.toWCharArray(devname);
		hr = pEnumerator->GetDevice(devname, &pDevice);
		if (FAILED(hr)) {
			qWarning("WASAPIInput: Failed to open selected device, falling back to default");
		}
	}

	if (! pDevice) {
		hr = pEnumerator->GetDefaultAudioEndpoint(eCapture, eCommunications, &pDevice);
		if (FAILED(hr)) {
			qWarning("WASAPIInput: Failed to open input device");
			pEnumerator->Release();
			return;
		}
	}

	pEnumerator->Release();
	pEnumerator = NULL;

	hr = pDevice->Activate(__uuidof(IAudioClient), CLSCTX_ALL, NULL, (void **) &pAudioClient);
	pDevice->Release();
	pDevice = NULL;

	if (FAILED(hr)) {
		qWarning("WASAPIInput: Activate AudioClient failed");
		return;
	}

	hr = pAudioClient->GetMixFormat(&pwfx);
	pwfxe = reinterpret_cast<WAVEFORMATEXTENSIBLE *>(pwfx);

	if ((pwfx->wBitsPerSample != (sizeof(float) * 8)) || (pwfxe->SubFormat != KSDATAFORMAT_SUBTYPE_IEEE_FLOAT)) {
		qWarning("WASAPIInput: Subformat is not IEEE Float");
		CoTaskMemFree(pwfx);
		pAudioClient->Release();
		return;
	}

	hr = pAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, AUDCLNT_STREAMFLAGS_EVENTCALLBACK, hnsRequestedDuration, 0, pwfx, NULL);
	if (FAILED(hr)) {
		qWarning("WASAPIInput: Initialize failed");
		CoTaskMemFree(pwfx);
		pAudioClient->Release();
		return;
	}

	hr = pAudioClient->GetBufferSize(&bufferFrameCount);
	hr = pAudioClient->GetService(__uuidof(IAudioCaptureClient), (void**)&pCaptureClient);
	if (FAILED(hr)) {
		qWarning("WASAPIInput: GetService failed");
		CoTaskMemFree(pwfx);
		pAudioClient->Release();
		return;
	}

	srs = speex_resampler_init(1, pwfx->nSamplesPerSec, 16000, 8, &err);

	wantLength = (iFrameSize * pwfx->nSamplesPerSec) / 16000;

	qWarning("WASAPIInput: %d %d %d", bufferFrameCount, pwfx->nSamplesPerSec, wantLength);

	hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	pAudioClient->SetEventHandle(hEvent);
	if ((hEvent == NULL) || (FAILED(hr))) {
		qWarning("WASAPI: Input: Failed to set event");
	}


	hMmThread = AvSetMmThreadCharacteristics(L"Pro Audio", &dwTaskIndex);
	if (hMmThread == NULL) {
		qWarning("WASAPI: Input: Failed to set Pro Audio thread priority");
	}

	hr = pAudioClient->Start();

	float mul = 32768.0f / pwfx->nChannels;

	STACKVAR(float, inbuff, wantLength);
	STACKVAR(float, outbuff, iFrameSize);

	while (bRunning && ! FAILED(hr)) {
		hr = pCaptureClient->GetNextPacketSize(&packetLength);
//		qWarning("Avail: %d", packetLength);
		while (! FAILED(hr) && (packetLength > 0)) {
			hr = pCaptureClient->GetBuffer(&pData, &numFramesAvailable, &flags, NULL, NULL);
			numFramesLeft = numFramesAvailable;
			if (! FAILED(hr)) {
				float *inData = reinterpret_cast<float *>(pData);
				while (numFramesLeft) {
					float v = 0.0f;
					for(int i=0;i<pwfx->nChannels;i++) {
						v+= *inData;
						++inData;
					}
					inbuff[gotLength] = v;
					++gotLength;

					if (gotLength == wantLength) {
						spx_uint32_t inlen = wantLength;
						spx_uint32_t outlen = iFrameSize;
						speex_resampler_process_float(srs, 0, inbuff, &inlen, outbuff, &outlen);
						for(int i=0;i<iFrameSize;i++)
							psMic[i] = static_cast<short>(outbuff[i] * mul);

						encodeAudioFrame();
						gotLength = 0;
					}
					--numFramesLeft;
				}
				hr = pCaptureClient->ReleaseBuffer(numFramesAvailable);
				if (! FAILED(hr)) {
					hr = pCaptureClient->GetNextPacketSize(&packetLength);
				}
			}
		}
		if (! FAILED(hr))
			WaitForSingleObject(hEvent, 2000);
	}

	pAudioClient->Stop();

	if (hMmThread != NULL)
		AvRevertMmThreadCharacteristics(hMmThread);

	speex_resampler_destroy(srs);
	CoTaskMemFree(pwfx);
	pCaptureClient->Release();
	pAudioClient->Release();

	CloseHandle(hEvent);
}

WASAPIOutput::WASAPIOutput() {
	bRunning = true;
}

WASAPIOutput::~WASAPIOutput() {
	bRunning = false;
}

void WASAPIOutput::run() {
}
