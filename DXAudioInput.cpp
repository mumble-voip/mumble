/* Copyright (C) 2005, Thorvald Natvig <thorvald@natvig.com>

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

static AudioInput *DXAudioInputNew() {
	return new DXAudioInput();
}

static AudioInputRegistrar airDX("DirectSound", DXAudioInputNew);

#define NBUFFBLOCKS 50

DXAudioInput::DXAudioInput() {
    HRESULT       hr;
    WAVEFORMATEX  wfx;
    DSCBUFFERDESC dscbd;
    DSBPOSITIONNOTIFY    aPosNotify[ NBUFFBLOCKS ];

	hNotificationEvent = CreateEvent( NULL, FALSE, FALSE, NULL );

	pDSCapture = NULL;
	pDSCaptureBuffer = NULL;
	pDSNotify = NULL;

	bOk = false;

	bool failed = false;

	ZeroMemory( &wfx, sizeof(wfx) );
	wfx.wFormatTag = WAVE_FORMAT_PCM;

	ZeroMemory( &dscbd, sizeof(dscbd) );
    dscbd.dwSize = sizeof(dscbd);

    dscbd.dwBufferBytes = dwBufferSize = iByteSize * NBUFFBLOCKS;
    dscbd.lpwfxFormat = &wfx;

    wfx.nChannels = 1;
    wfx.nSamplesPerSec = SAMPLE_RATE;
    wfx.nBlockAlign = 2;
    wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;
    wfx.wBitsPerSample = 16;

    for( int i = 0; i < NBUFFBLOCKS; i++ )
    {
        aPosNotify[i].dwOffset = (iByteSize * (i+1)) -1;
        aPosNotify[i].hEventNotify = hNotificationEvent;
    }

    // Create IDirectSoundCapture using the preferred capture device
	if (! g.s.qbaDXInput.isEmpty()) {
		LPGUID lpguid = reinterpret_cast<LPGUID>(g.s.qbaDXInput.data());
	    if( FAILED( hr = DirectSoundCaptureCreate8(lpguid, &pDSCapture, NULL))) {
			failed = true;
		}
	}

    if (! pDSCapture && FAILED( hr = DirectSoundCaptureCreate8(&DSDEVID_DefaultVoiceCapture, &pDSCapture, NULL ) ) )
	    qWarning("DXAudioInput: DirectSoundCaptureCreate");
	else if( FAILED( hr = pDSCapture->CreateCaptureBuffer( &dscbd, &pDSCaptureBuffer, NULL)))
		qWarning("DXAudioInput: CreateCaptureBuffer");
	else if( FAILED( hr = pDSCaptureBuffer->QueryInterface( IID_IDirectSoundNotify, reinterpret_cast<VOID**>(&pDSNotify) ) ) )
		qWarning("DXAudioInput: QueryInterface (Notify)");
	else if( FAILED( hr = pDSNotify->SetNotificationPositions( NBUFFBLOCKS, aPosNotify ) ) )
    	qWarning("DXAudioInput: SetNotificationPositions");
    else
    	bOk = true;


	if (! bOk) {
		QMessageBox::warning(NULL, tr("Mumble"), tr("Opening chosen DirectSound Input failed. No microphone capture will be done."), QMessageBox::Ok, QMessageBox::NoButton);
		return;
	}

	if (failed)
		QMessageBox::warning(NULL, tr("Mumble"), tr("Opening chosen DirectSound Input failed. Using defaults."), QMessageBox::Ok, QMessageBox::NoButton);

	qWarning("DXAudioInput: Initialized");
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

	if( FAILED( hr = pDSCaptureBuffer->Start( DSCBSTART_LOOPING ) ) )
		qFatal("DXAudioInput: Start");

	bRunning = true;

	while (bRunning) {

		do {
			if( FAILED( hr = pDSCaptureBuffer->GetCurrentPosition(&dwCapturePosition, &dwReadPosition ) ) )
				qFatal("DXAudioInput: GetCurrentPosition");
			if (dwReadPosition < dwLastReadPos)
				dwReadyBytes = (dwBufferSize - dwLastReadPos) + dwReadPosition;
			else
				dwReadyBytes = dwReadPosition - dwLastReadPos;

			if (static_cast<int>(dwReadyBytes) < iByteSize) {
				WaitForSingleObject(hNotificationEvent, INFINITE);
			}
		} while (static_cast<int>(dwReadyBytes) < iByteSize);

    	if (FAILED( hr = pDSCaptureBuffer->Lock(dwLastReadPos, iByteSize, &aptr1, &nbytes1, &aptr2, &nbytes2, 0)))
    		qFatal("DXAudioInput: Lock from %ld (%d bytes)",dwLastReadPos, iByteSize);

    	if (aptr1 && nbytes1)
    		CopyMemory(psMic, aptr1, nbytes1);

    	if (aptr2 && nbytes2)
    		CopyMemory(psMic+nbytes1/2, aptr2, nbytes2);

    	if (FAILED( hr = pDSCaptureBuffer->Unlock(aptr1, nbytes1, aptr2, nbytes2)))
    		qFatal("DXAudioInput: Unlock");

		dwLastReadPos = (dwLastReadPos + iByteSize) % dwBufferSize;

		encodeAudioFrame();
	}

	pDSCaptureBuffer->Stop();
}
