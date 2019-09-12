// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "DirectSound.h"

#include "MainWindow.h"
#include "Plugins.h"
#include "User.h"

#include "win.h"

#define DIRECTSOUND_VERSION 0x1000

#include <mmsystem.h>
#include <dsound.h>
#include <ks.h>
#include <ksmedia.h>

// We define a global macro called 'g'. This can lead to issues when included code uses 'g' as a type or parameter name (like protobuf 3.7 does). As such, for now, we have to make this our last include.
#include "Global.h"

// from os_win.cpp
extern HWND mumble_mw_hwnd;

#undef FAILED
#define FAILED(Status) (static_cast<HRESULT>(Status)<0)

// #define MY_DEFERRED DS3D_DEFERRED
#define MY_DEFERRED DS3D_IMMEDIATE

#define NBLOCKS 50

class DXAudioOutputRegistrar : public AudioOutputRegistrar {
	public:
		DXAudioOutputRegistrar();
		virtual AudioOutput *create();
		virtual const QList<audioDevice> getDeviceChoices();
		virtual void setDeviceChoice(const QVariant &, Settings &);

};

class DXAudioInputRegistrar : public AudioInputRegistrar {
	public:
		DXAudioInputRegistrar();
		virtual AudioInput *create();
		virtual const QList<audioDevice> getDeviceChoices();
		virtual void setDeviceChoice(const QVariant &, Settings &);
		virtual bool canEcho(const QString &) const;

};

class DirectSoundInit : public DeferInit {
		DXAudioInputRegistrar *airReg;
		DXAudioOutputRegistrar *aorReg;
	public:
		DirectSoundInit() : airReg(NULL), aorReg(NULL) {}
		void initialize();
		void destroy();
};

static DirectSoundInit dsinit;

void DirectSoundInit::initialize() {
	airReg = NULL;
	aorReg = NULL;

#ifdef USE_WASAPI
	OSVERSIONINFOEXW ovi;
	memset(&ovi, 0, sizeof(ovi));

	ovi.dwOSVersionInfoSize=sizeof(ovi);
	GetVersionEx(reinterpret_cast<OSVERSIONINFOW *>(&ovi));

#ifdef QT_NO_DEBUG
	if ((ovi.dwMajorVersion > 6) || ((ovi.dwMajorVersion == 6) && (ovi.dwBuildNumber >= 6001))) {
		HMODULE hLib = LoadLibrary(L"AVRT.DLL");
		if (hLib != NULL) {
			FreeLibrary(hLib);
			qWarning("DirectSound: Disabled as WASAPI is available");
			return;
		}
	}
#endif
#endif

	airReg = new DXAudioInputRegistrar();
	aorReg = new DXAudioOutputRegistrar();
}

void DirectSoundInit::destroy() {
	delete airReg;
	delete aorReg;
}


DXAudioOutputRegistrar::DXAudioOutputRegistrar() : AudioOutputRegistrar(QLatin1String("DirectSound")) {
}

AudioOutput *DXAudioOutputRegistrar::create() {
	return new DXAudioOutput();
}

typedef QPair<QString, GUID> dsDevice;

static BOOL CALLBACK DSEnumProc(LPGUID lpGUID, const WCHAR* lpszDesc,
                                const WCHAR*, void *ctx) {
	if (lpGUID) {
		QList<dsDevice> *l =reinterpret_cast<QList<dsDevice> *>(ctx);
		*l << dsDevice(QString::fromUtf16(reinterpret_cast<const ushort*>(lpszDesc)), *lpGUID);
	}

	return(true);
}

const QList<audioDevice> DXAudioOutputRegistrar::getDeviceChoices() {
	QList<dsDevice> qlOutput;

	qlOutput << dsDevice(DXAudioOutput::tr("Default DirectSound Voice Output"), DSDEVID_DefaultVoicePlayback);
	DirectSoundEnumerate(DSEnumProc, reinterpret_cast<void *>(&qlOutput));

	QList<audioDevice> qlReturn;

	const GUID *lpguid = NULL;

	if (! g.s.qbaDXOutput.isEmpty()) {
		lpguid = reinterpret_cast<LPGUID>(g.s.qbaDXOutput.data());
	} else {
		lpguid = &DSDEVID_DefaultVoicePlayback;
	}

	foreach(dsDevice d, qlOutput) {
		if (d.second == *lpguid) {
			qlReturn << audioDevice(d.first, QByteArray(reinterpret_cast<const char *>(&d.second), sizeof(GUID)));
		}
	}
	foreach(dsDevice d, qlOutput) {
		if (d.second != *lpguid) {
			qlReturn << audioDevice(d.first, QByteArray(reinterpret_cast<const char *>(&d.second), sizeof(GUID)));
		}
	}
	return qlReturn;
}

void DXAudioOutputRegistrar::setDeviceChoice(const QVariant &choice, Settings &s) {
	s.qbaDXOutput = choice.toByteArray();
}

DXAudioInputRegistrar::DXAudioInputRegistrar() : AudioInputRegistrar(QLatin1String("DirectSound")) {
}

AudioInput *DXAudioInputRegistrar::create() {
	return new DXAudioInput();
}

const QList<audioDevice> DXAudioInputRegistrar::getDeviceChoices() {
	QList<dsDevice> qlInput;

	qlInput << dsDevice(DXAudioInput::tr("Default DirectSound Voice Input"), DSDEVID_DefaultVoiceCapture);
	DirectSoundCaptureEnumerate(DSEnumProc, reinterpret_cast<void *>(&qlInput));

	QList<audioDevice> qlReturn;

	const GUID *lpguid = NULL;

	if (! g.s.qbaDXInput.isEmpty()) {
		lpguid = reinterpret_cast<LPGUID>(g.s.qbaDXInput.data());
	} else {
		lpguid = &DSDEVID_DefaultVoiceCapture;
	}

	foreach(dsDevice d, qlInput) {
		if (d.second == *lpguid) {
			qlReturn << audioDevice(d.first, QByteArray(reinterpret_cast<const char *>(&d.second), sizeof(GUID)));
		}
	}
	foreach(dsDevice d, qlInput) {
		if (d.second != *lpguid) {
			qlReturn << audioDevice(d.first, QByteArray(reinterpret_cast<const char *>(&d.second), sizeof(GUID)));
		}
	}
	return qlReturn;
}

void DXAudioInputRegistrar::setDeviceChoice(const QVariant &choice, Settings &s) {
	s.qbaDXInput = choice.toByteArray();
}

bool DXAudioInputRegistrar::canEcho(const QString &) const {
	return false;
}

DXAudioOutput::DXAudioOutput() {
}

DXAudioOutput::~DXAudioOutput() {
	bRunning = false;
	wait();
}

void DXAudioOutput::run() {
	HRESULT hr;
	DSBUFFERDESC        dsbdesc;
	WAVEFORMATEXTENSIBLE wfx;
	WAVEFORMATEXTENSIBLE wfxSet;
	int ns = 0;
	unsigned int chanmasks[32];

	LPDIRECTSOUND8             pDS = NULL;
	LPDIRECTSOUNDBUFFER       pDSBPrimary = NULL;
	LPDIRECTSOUNDBUFFER       pDSBOutput = NULL;
	LPDIRECTSOUNDNOTIFY       pDSNotify = NULL;

	int iLastwriteblock;
	LPVOID aptr1, aptr2;
	DWORD nbytes1, nbytes2;

	int playblock;
	int nowriteblock;
	DWORD dwPlayPosition, dwWritePosition;

	unsigned int iByteSize;

	bool bOk;
	DWORD dwSpeakerConfig;

	bool failed = false;

	bOk = false;
	DWORD dwMask = 0;
	bool bHead = false;

	ZeroMemory(&dsbdesc, sizeof(DSBUFFERDESC));
	dsbdesc.dwSize  = sizeof(DSBUFFERDESC);
	dsbdesc.dwFlags = DSBCAPS_PRIMARYBUFFER;

	if (! g.s.qbaDXOutput.isEmpty()) {
		LPGUID lpguid = reinterpret_cast<LPGUID>(g.s.qbaDXOutput.data());
		if (FAILED(hr = DirectSoundCreate8(lpguid, &pDS, NULL))) {
			failed = true;
		}
	}

	if (! pDS && FAILED(hr = DirectSoundCreate8(&DSDEVID_DefaultVoicePlayback, &pDS, NULL))) {
		qWarning("DXAudioOutput: DirectSoundCreate failed: hr=0x%08lx", hr);
		goto cleanup;
	} else if (FAILED(hr = pDS->SetCooperativeLevel(mumble_mw_hwnd, DSSCL_PRIORITY))) {
		qWarning("DXAudioOutput: SetCooperativeLevel failed: hr=0x%08lx", hr);
		goto cleanup;
	} else if (FAILED(hr = pDS->CreateSoundBuffer(&dsbdesc, &pDSBPrimary, NULL))) {
		qWarning("DXAudioOutput: CreateSoundBuffer (Primary) failed: hr=0x%08lx", hr);
		goto cleanup;
	}

	pDS->GetSpeakerConfig(&dwSpeakerConfig);


	switch (DSSPEAKER_CONFIG(dwSpeakerConfig)) {
		case DSSPEAKER_HEADPHONE:
			dwMask = KSAUDIO_SPEAKER_STEREO;
			bHead = true;
			break;
		case DSSPEAKER_MONO:
			dwMask = KSAUDIO_SPEAKER_MONO;
			break;
		case DSSPEAKER_QUAD:
			dwMask = KSAUDIO_SPEAKER_QUAD;
			break;
		case DSSPEAKER_STEREO:
			dwMask = KSAUDIO_SPEAKER_STEREO;
			break;
		case DSSPEAKER_SURROUND:
			dwMask = KSAUDIO_SPEAKER_SURROUND;
			break;
		case DSSPEAKER_5POINT1:
			dwMask = KSAUDIO_SPEAKER_5POINT1;
			break;
		case DSSPEAKER_7POINT1:
			dwMask = KSAUDIO_SPEAKER_7POINT1;
			break;
		case DSSPEAKER_7POINT1_SURROUND:
			dwMask = KSAUDIO_SPEAKER_7POINT1_SURROUND;
			break;
		case DSSPEAKER_5POINT1_SURROUND:
			dwMask = KSAUDIO_SPEAKER_5POINT1_SURROUND;
			break;
		default:
			dwMask = 0;
			break;
	}

	if (! g.s.doPositionalAudio())
		dwMask = KSAUDIO_SPEAKER_STEREO;

	for (int i=0;i<32;i++) {
		if (dwMask & (1 << i)) {
			chanmasks[ns++] = 1 << i;
		}
	}

	iMixerFreq = SAMPLE_RATE;
	iChannels = ns;
	eSampleFormat = SampleShort;

	iByteSize = iFrameSize * sizeof(short) * ns;

	ZeroMemory(&wfxSet, sizeof(wfxSet));
	wfxSet.Format.wFormatTag = WAVE_FORMAT_PCM;

	ZeroMemory(&wfx, sizeof(wfx));
	wfx.Format.wFormatTag = WAVE_FORMAT_PCM;
	wfx.Format.nChannels = qMax(ns, 2);
	wfx.Format.nSamplesPerSec = SAMPLE_RATE;
	wfx.Format.nBlockAlign = sizeof(short) * wfx.Format.nChannels;
	wfx.Format.nAvgBytesPerSec = wfx.Format.nSamplesPerSec * wfx.Format.nBlockAlign;
	wfx.Format.wBitsPerSample = 16;

	if (FAILED(hr = pDSBPrimary->SetFormat(reinterpret_cast<WAVEFORMATEX *>(&wfx)))) {
		qWarning("DXAudioOutput: SetFormat failed: hr=0x%08lx", hr);
		goto cleanup;
	}
	if (FAILED(hr = pDSBPrimary->GetFormat(reinterpret_cast<WAVEFORMATEX *>(&wfxSet), sizeof(wfxSet), NULL))) {
		qWarning("DXAudioOutput: GetFormat failed: hr=0x%08lx", hr);
		goto cleanup;
	}

	qWarning("DXAudioOutput: Primary buffer of %ld Hz, %d channels, %d bits",wfxSet.Format.nSamplesPerSec,wfxSet.Format.nChannels,wfxSet.Format.wBitsPerSample);

	ZeroMemory(&wfx, sizeof(wfx));
	wfx.Format.wFormatTag = WAVE_FORMAT_EXTENSIBLE;
	wfx.Format.nChannels = qMax(ns, 2);
	wfx.Format.nSamplesPerSec = SAMPLE_RATE;
	wfx.Format.nBlockAlign = sizeof(short) * wfx.Format.nChannels;
	wfx.Format.nAvgBytesPerSec = wfx.Format.nSamplesPerSec * wfx.Format.nBlockAlign;
	wfx.Format.wBitsPerSample = 16;
	wfx.Format.cbSize = 32;
	wfx.Samples.wValidBitsPerSample = wfx.Format.wBitsPerSample;
	wfx.dwChannelMask = dwMask;
	wfx.SubFormat = KSDATAFORMAT_SUBTYPE_PCM;

	ZeroMemory(&dsbdesc, sizeof(DSBUFFERDESC));
	dsbdesc.dwSize          = sizeof(DSBUFFERDESC);
	dsbdesc.dwFlags         = DSBCAPS_GLOBALFOCUS|DSBCAPS_GETCURRENTPOSITION2;
	dsbdesc.dwFlags	 |= DSBCAPS_CTRLPOSITIONNOTIFY;
	dsbdesc.dwBufferBytes = wfx.Format.nChannels * iFrameSize * sizeof(short) * NBLOCKS;
	dsbdesc.lpwfxFormat     = reinterpret_cast<WAVEFORMATEX *>(&wfx);

	if (FAILED(hr = pDS->CreateSoundBuffer(&dsbdesc, &pDSBOutput, NULL))) {
		qWarning("DXAudioOutputUser: CreateSoundBuffer (Secondary) failed: hr=0x%08lx", hr);
		goto cleanup;
	}


	if (FAILED(hr = pDSBOutput->QueryInterface(IID_IDirectSoundNotify, reinterpret_cast<void **>(&pDSNotify)))) {
		qWarning("DXAudioOutputUser: QueryInterface (Notify) failed: hr=0x%08lx", hr);
		goto cleanup;
	}

	qWarning("DXAudioOutputUser: New %dHz output buffer of %ld bytes", SAMPLE_RATE, dsbdesc.dwBufferBytes);

	if (failed)
		g.mw->msgBox(tr("Opening chosen DirectSound Output failed. Default device will be used."));

	initializeMixer(chanmasks, bHead);

	if (FAILED(hr = pDSBOutput->Lock(0, 0, &aptr1, &nbytes1, &aptr2, &nbytes2, DSBLOCK_ENTIREBUFFER))) {
		qWarning("DXAudioOutputUser: Initial Lock failed: hr=0x%08lx", hr);
		goto cleanup;
	}

	if (aptr1)
		ZeroMemory(aptr1, nbytes1);
	if (aptr2)
		ZeroMemory(aptr2, nbytes2);

	if (FAILED(hr = pDSBOutput->Unlock(aptr1, nbytes1, aptr2, nbytes2))) {
		qWarning("DXAudioOutputUser: Initial Unlock failed: hr=0x%08lx", hr);
		goto cleanup;
	}

	if (FAILED(hr = pDSBOutput->Play(0, 0, DSBPLAY_LOOPING))) {
		qWarning("DXAudioOutputUser: Play failed: hr=0x%08lx", hr);
		goto cleanup;
	}

	iLastwriteblock = (NBLOCKS - 1 + g.s.iOutputDelay) % NBLOCKS;

	bOk = true;

	while (bRunning && ! FAILED(hr)) {
		if (FAILED(hr = pDSBOutput->GetCurrentPosition(&dwPlayPosition, &dwWritePosition))) {
			qWarning("DXAudioOutputUser: GetCurrentPosition failed: hr=0x%08lx", hr);
			break;
		}

		playblock = dwWritePosition / iByteSize;
		nowriteblock = (playblock + g.s.iOutputDelay + 1) % NBLOCKS;

		for (int block=(iLastwriteblock + 1) % NBLOCKS;(!FAILED(hr)) && (block!=nowriteblock);block=(block + 1) % NBLOCKS) {
			iLastwriteblock = block;

			if (FAILED(hr = pDSBOutput->Lock(block * iByteSize, iByteSize, &aptr1, &nbytes1, &aptr2, &nbytes2, 0))) {
				qWarning("DXAudioOutput: Lock block %u (%d bytes) failed: hr=0x%08lx",block, iByteSize, hr);
				break;
			}
			if (aptr2 || nbytes2) {
				qWarning("DXAudioOutput: Split buffer");
				break;
			}
			if (!aptr1 || ! nbytes1) {
				qWarning("DXAudioOutput: Zerolock");
				break;
			}
			if (! mix(reinterpret_cast<short *>(aptr1), iFrameSize))
				ZeroMemory(aptr1, iByteSize);

			if (FAILED(hr = pDSBOutput->Unlock(aptr1, nbytes1, aptr2, nbytes2))) {
				qWarning("DXAudioOutput: Unlock %p(%lu) %p(%lu) failed: hr=0x%08lx",aptr1,nbytes1,aptr2,nbytes2,hr);
				break;
			}

			if (FAILED(hr = pDSBOutput->GetCurrentPosition(&dwPlayPosition, &dwWritePosition))) {
				qWarning("DXAudioOutputUser: GetCurrentPosition failed: hr=0x%08lx", hr);
				break;
			}

			playblock = dwWritePosition / iByteSize;
			nowriteblock = (playblock + g.s.iOutputDelay + 1) % NBLOCKS;
		}
		if (! FAILED(hr))
			msleep(19);
	}

	if (FAILED(hr)) {
		g.mw->msgBox(tr("Lost DirectSound output device."));
	}
cleanup:
	if (! bOk) {
		g.mw->msgBox(tr("Opening chosen DirectSound Output failed. No audio will be heard."));
		return;
	}

	if (pDSNotify)
		pDSNotify->Release();
	if (pDSBOutput) {
		pDSBOutput->Stop();
		pDSBOutput->Release();
	}
	if (pDSBPrimary)
		pDSBPrimary->Release();
	if (pDS)
		pDS->Release();
}

#define NBUFFBLOCKS 50

DXAudioInput::DXAudioInput() {
}

DXAudioInput::~DXAudioInput() {
	bRunning = false;
	wait();
}

void DXAudioInput::run() {
	LPDIRECTSOUNDCAPTURE8      pDSCapture;
	LPDIRECTSOUNDCAPTUREBUFFER pDSCaptureBuffer;
	LPDIRECTSOUNDNOTIFY        pDSNotify;

	DWORD dwBufferSize;
	bool bOk;
	DWORD dwReadPosition;
	DWORD dwCapturePosition;

	LPVOID aptr1, aptr2;
	DWORD nbytes1, nbytes2;

	HRESULT       hr;
	WAVEFORMATEX  wfx;
	DSCBUFFERDESC dscbd;

	pDSCapture = NULL;
	pDSCaptureBuffer = NULL;
	pDSNotify = NULL;

	bOk = false;

	bool failed = false;

	Timer t;

	ZeroMemory(&wfx, sizeof(wfx));
	wfx.wFormatTag = WAVE_FORMAT_PCM;

	ZeroMemory(&dscbd, sizeof(dscbd));
	dscbd.dwSize = sizeof(dscbd);

	dscbd.dwBufferBytes = dwBufferSize = iFrameSize * sizeof(short) * NBUFFBLOCKS;
	dscbd.lpwfxFormat = &wfx;

	wfx.nChannels = 1;
	wfx.nSamplesPerSec = iSampleRate;
	wfx.nBlockAlign = 2;
	wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;
	wfx.wBitsPerSample = 16;

	// Create IDirectSoundCapture using the preferred capture device
	if (! g.s.qbaDXInput.isEmpty()) {
		LPGUID lpguid = reinterpret_cast<LPGUID>(g.s.qbaDXInput.data());
		if (FAILED(hr = DirectSoundCaptureCreate8(lpguid, &pDSCapture, NULL))) {
			failed = true;
		}
	}

	if (! pDSCapture && FAILED(hr = DirectSoundCaptureCreate8(&DSDEVID_DefaultVoiceCapture, &pDSCapture, NULL)))
		qWarning("DXAudioInput: DirectSoundCaptureCreate failed: hr=0x%08lx", hr);
	else if (FAILED(hr = pDSCapture->CreateCaptureBuffer(&dscbd, &pDSCaptureBuffer, NULL)))
		qWarning("DXAudioInput: CreateCaptureBuffer failed: hr=0x%08lx", hr);
	else if (FAILED(hr = pDSCaptureBuffer->QueryInterface(IID_IDirectSoundNotify, reinterpret_cast<void **>(&pDSNotify))))
		qWarning("DXAudioInput: QueryInterface (Notify) failed: hr=0x%08lx", hr);
	else
		bOk = true;



	if (failed)
		g.mw->msgBox(tr("Opening chosen DirectSound Input failed. Default device will be used."));

	qWarning("DXAudioInput: Initialized");

	if (! bOk)
		goto cleanup;

	if (FAILED(hr = pDSCaptureBuffer->Start(DSCBSTART_LOOPING))) {
		qWarning("DXAudioInput: Start failed: hr=0x%08lx", hr);
	} else {
		DWORD dwReadyBytes = 0;
		DWORD dwLastReadPos = 0;
		float safety = 2.0f;

		while (bRunning) {
			bool firstsleep = true;
			bool didsleep = false;

			do {
				if (FAILED(hr = pDSCaptureBuffer->GetCurrentPosition(&dwCapturePosition, &dwReadPosition))) {
					qWarning("DXAudioInput: GetCurrentPosition failed: hr=0x%08lx", hr);
					bRunning = false;
					break;
				}
				if (dwReadPosition < dwLastReadPos)
					dwReadyBytes = (dwBufferSize - dwLastReadPos) + dwReadPosition;
				else
					dwReadyBytes = dwReadPosition - dwLastReadPos;

				if (static_cast<size_t>(dwReadyBytes) < sizeof(short) * iFrameSize) {
					double msecleft = 20.0 - (dwReadyBytes * 20.0) / (sizeof(short) * iFrameSize);

					if (didsleep)
						safety *= 1.1f;
					else if (firstsleep)
						safety *= 0.998f;

					int msec = static_cast<int>(msecleft + (firstsleep ? safety : 0.0));

					msleep(msec);

					didsleep = true;
					firstsleep = false;
				}
			} while (static_cast<size_t>(dwReadyBytes) < sizeof(short) * iFrameSize);

			// Desynchonized?
			if (dwReadyBytes > (dwBufferSize / 2)) {
				qWarning("DXAudioInput: Lost synchronization");
				dwLastReadPos = dwReadPosition;
			} else if (bRunning) {
				if (FAILED(hr = pDSCaptureBuffer->Lock(dwLastReadPos, sizeof(short) * iFrameSize, &aptr1, &nbytes1, &aptr2, &nbytes2, 0))) {
					qWarning("DXAudioInput: Lock from %lu (%zu bytes) failed: hr=0x%08lx", static_cast<unsigned long>(dwLastReadPos), sizeof(short) * iFrameSize, hr);
					bRunning = false;
					break;
				}

				if (aptr1 && nbytes1)
					CopyMemory(psMic, aptr1, nbytes1);

				if (aptr2 && nbytes2)
					CopyMemory(psMic+nbytes1/2, aptr2, nbytes2);

				if (FAILED(hr = pDSCaptureBuffer->Unlock(aptr1, nbytes1, aptr2, nbytes2))) {
					qWarning("DXAudioInput: Unlock failed: hr=0x%08lx", hr);
					bRunning = false;
					break;
				}

				dwLastReadPos = (dwLastReadPos + sizeof(short) * iFrameSize) % dwBufferSize;

				encodeAudioFrame();
			}
		}
		if (! FAILED(hr))
			pDSCaptureBuffer->Stop();
	}
	if (FAILED(hr)) {
		g.mw->msgBox(tr("Lost DirectSound input device."));
	}

cleanup:
	if (! bOk) {
		g.mw->msgBox(tr("Opening chosen DirectSound Input device failed. No microphone capture will be done."));
	}
	if (pDSNotify)
		pDSNotify->Release();
	if (pDSCaptureBuffer)
		pDSCaptureBuffer->Release();
	if (pDSCapture)
		pDSCapture->Release();
}
