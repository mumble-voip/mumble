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

#include "DXAudioInput.h"
#include "Global.h"

#undef FAILED
#define FAILED(Status) (static_cast<HRESULT>(Status)<0)

class DXAudioInputRegistrar : public AudioInputRegistrar {
	public:
		DXAudioInputRegistrar();
		virtual AudioInput *create();
		virtual const QList<audioDevice> getDeviceChoices();
		virtual void setDeviceChoice(const QVariant &, Settings &);
		virtual bool canEcho(const QString &);

};

// Static singleton
static DXAudioInputRegistrar airDX;

DXAudioInputRegistrar::DXAudioInputRegistrar() : AudioInputRegistrar(QLatin1String("DirectSound")) {
}

AudioInput *DXAudioInputRegistrar::create() {
	return new DXAudioInput();
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

bool DXAudioInputRegistrar::canEcho(const QString &) {
	return false;
}

#define NBUFFBLOCKS 50

DXAudioInput::DXAudioInput() {
	HRESULT       hr;
	WAVEFORMATEX  wfx;
	DSCBUFFERDESC dscbd;
	DSBPOSITIONNOTIFY    aPosNotify[ NBUFFBLOCKS ];

	hNotificationEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	pDSCapture = NULL;
	pDSCaptureBuffer = NULL;
	pDSNotify = NULL;

	bOk = false;

	bool failed = false;

	ZeroMemory(&wfx, sizeof(wfx));
	wfx.wFormatTag = WAVE_FORMAT_PCM;

	ZeroMemory(&dscbd, sizeof(dscbd));
	dscbd.dwSize = sizeof(dscbd);

	dscbd.dwBufferBytes = dwBufferSize = iFrameSize * sizeof(short) * NBUFFBLOCKS;
	dscbd.lpwfxFormat = &wfx;

	wfx.nChannels = 1;
	wfx.nSamplesPerSec = SAMPLE_RATE;
	wfx.nBlockAlign = 2;
	wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;
	wfx.wBitsPerSample = 16;

	for (int i = 0; i < NBUFFBLOCKS; i++) {
		aPosNotify[i].dwOffset = (iFrameSize * sizeof(short) * (i+1)) -1;
		aPosNotify[i].hEventNotify = hNotificationEvent;
	}

	// Create IDirectSoundCapture using the preferred capture device
	if (! g.s.qbaDXInput.isEmpty()) {
		LPGUID lpguid = reinterpret_cast<LPGUID>(g.s.qbaDXInput.data());
		if (FAILED(hr = DirectSoundCaptureCreate8(lpguid, &pDSCapture, NULL))) {
			failed = true;
		}
	}

	if (! pDSCapture && FAILED(hr = DirectSoundCaptureCreate8(&DSDEVID_DefaultVoiceCapture, &pDSCapture, NULL)))
		qWarning("DXAudioInput: DirectSoundCaptureCreate");
	else if (FAILED(hr = pDSCapture->CreateCaptureBuffer(&dscbd, &pDSCaptureBuffer, NULL)))
		qWarning("DXAudioInput: CreateCaptureBuffer");
	else if (FAILED(hr = pDSCaptureBuffer->QueryInterface(IID_IDirectSoundNotify, reinterpret_cast<void **>(&pDSNotify))))
		qWarning("DXAudioInput: QueryInterface (Notify)");
	else if (FAILED(hr = pDSNotify->SetNotificationPositions(NBUFFBLOCKS, aPosNotify)))
		qWarning("DXAudioInput: SetNotificationPositions");
	else
		bOk = true;


	if (! bOk) {
		QMessageBox::warning(NULL, tr("Mumble"), tr("Opening chosen DirectSound Input failed. No microphone capture will be done."), QMessageBox::Ok, QMessageBox::NoButton);
		return;
	}

	if (failed)
		QMessageBox::warning(NULL, tr("Mumble"), tr("Opening chosen DirectSound Input failed. Default device will be used."), QMessageBox::Ok, QMessageBox::NoButton);

	qWarning("DXAudioInput: Initialized");

	bRunning = true;
}

DXAudioInput::~DXAudioInput() {
	bRunning = false;
	wait();

	if (pDSNotify)
		pDSNotify->Release();
	if (pDSCaptureBuffer)
		pDSCaptureBuffer->Release();
	if (pDSCapture)
		pDSCapture->Release();

	CloseHandle(hNotificationEvent);
}

void DXAudioInput::run() {
	HRESULT       hr;
	DWORD dwReadyBytes;
	DWORD dwLastReadPos = 0;
	DWORD dwReadPosition;
	DWORD dwCapturePosition;

	LPVOID aptr1, aptr2;
	DWORD nbytes1, nbytes2;

	if (! bOk)
		return;

	if (FAILED(hr = pDSCaptureBuffer->Start(DSCBSTART_LOOPING)))
		qFatal("DXAudioInput: Start");

	while (bRunning) {

		do {
			if (FAILED(hr = pDSCaptureBuffer->GetCurrentPosition(&dwCapturePosition, &dwReadPosition)))
				qFatal("DXAudioInput: GetCurrentPosition");
			if (dwReadPosition < dwLastReadPos)
				dwReadyBytes = (dwBufferSize - dwLastReadPos) + dwReadPosition;
			else
				dwReadyBytes = dwReadPosition - dwLastReadPos;

			if (static_cast<int>(dwReadyBytes) < sizeof(short) * iFrameSize) {
				WaitForSingleObject(hNotificationEvent, INFINITE);
			}
		} while (static_cast<int>(dwReadyBytes) < sizeof(short) * iFrameSize);

		if (FAILED(hr = pDSCaptureBuffer->Lock(dwLastReadPos, sizeof(short) * iFrameSize, &aptr1, &nbytes1, &aptr2, &nbytes2, 0)))
			qFatal("DXAudioInput: Lock from %ld (%d bytes)",dwLastReadPos, sizeof(short) * iFrameSize);

		if (aptr1 && nbytes1)
			CopyMemory(psMic, aptr1, nbytes1);

		if (aptr2 && nbytes2)
			CopyMemory(psMic+nbytes1/2, aptr2, nbytes2);

		if (FAILED(hr = pDSCaptureBuffer->Unlock(aptr1, nbytes1, aptr2, nbytes2)))
			qFatal("DXAudioInput: Unlock");

		dwLastReadPos = (dwLastReadPos + sizeof(short) * iFrameSize) % dwBufferSize;

		encodeAudioFrame();
	}

	pDSCaptureBuffer->Stop();
}
