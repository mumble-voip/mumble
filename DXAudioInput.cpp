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

#define NBUFFBLOCKS 64

DXAudioInput::DXAudioInput() {
    HRESULT       hr;
    WAVEFORMATEX  wfx;
    DSCBUFFERDESC dscbd;
    DSBPOSITIONNOTIFY    aPosNotify[ NBUFFBLOCKS ];

	m_hNotificationEvent = CreateEvent( NULL, FALSE, FALSE, NULL );

    // Create IDirectSoundCapture using the preferred capture device
    if( FAILED( hr = DirectSoundCaptureCreate8(&DSDEVID_DefaultVoiceCapture, &m_pDSCapture, NULL ) ) )
    	qFatal("DXAudioInput: DirectSoundCaptureCreate");

	ZeroMemory( &wfx, sizeof(wfx) );
	wfx.wFormatTag = WAVE_FORMAT_PCM;

	ZeroMemory( &dscbd, sizeof(dscbd) );
    dscbd.dwSize = sizeof(dscbd);

    dscbd.dwBufferBytes = m_dwBufferSize = m_iByteSize * NBUFFBLOCKS;
    dscbd.lpwfxFormat = &wfx;

    wfx.nChannels = 1;
    wfx.nSamplesPerSec = SAMPLE_RATE;
    wfx.nBlockAlign = 2;
    wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;
    wfx.wBitsPerSample = 16;

	if( FAILED( hr = m_pDSCapture->CreateCaptureBuffer( &dscbd, &m_pDSCaptureBuffer, NULL ) ) )
		qFatal("DXAudioInput: CreateCaptureBuffer");

	if( FAILED( hr = m_pDSCaptureBuffer->QueryInterface( IID_IDirectSoundNotify, (VOID**)&m_pDSNotify ) ) )
		qFatal("DXAudioInput: QueryInterface (Notify)");


    // Setup the notification positions
    for( int i = 0; i < NBUFFBLOCKS; i++ )
    {
        aPosNotify[i].dwOffset = (m_iByteSize * (i+1)) -1;
        aPosNotify[i].hEventNotify = m_hNotificationEvent;
    }

    // Tell DirectSound when to notify us. the notification will come in the from
    // of signaled events that are handled in WinMain()
    if( FAILED( hr = m_pDSNotify->SetNotificationPositions( NBUFFBLOCKS, aPosNotify ) ) )
    	qFatal("DXAudioInput: SetNotificationPositions");

	qWarning("DXAudioInput: Initialized at %d Hz, %d buffers of %d bytes (%d)",wfx.nSamplesPerSec, NBUFFBLOCKS, m_iByteSize, m_dwBufferSize);
}

DXAudioInput::~DXAudioInput() {
	m_bRunning = false;
	wait();

	if (m_pDSNotify)
		m_pDSNotify->Release();
	if (m_pDSCaptureBuffer)
		m_pDSCaptureBuffer->Release();
	if (m_pDSCapture)
		m_pDSCapture->Release();

	CloseHandle(m_hNotificationEvent);
}

void DXAudioInput::run() {
    HRESULT       hr;
	DWORD dwReadyBytes;
	DWORD dwLastReadPos = 0;
	DWORD dwReadPosition;
	DWORD dwCapturePosition;

    LPVOID aptr1, aptr2;
  	DWORD nbytes1, nbytes2;

	int iBytesUncaptured;
	int iOffset;
   	short *psAudio;

	if( FAILED( hr = m_pDSCaptureBuffer->Start( DSCBSTART_LOOPING ) ) )
		qFatal("DXAudioInput: Start");

	m_bRunning = true;

	while (m_bRunning) {

		do {
			if( FAILED( hr = m_pDSCaptureBuffer->GetCurrentPosition(&dwCapturePosition, &dwReadPosition ) ) )
				qFatal("DXAudioInput: GetCurrentPosition");
			if (dwReadPosition < dwLastReadPos)
				dwReadyBytes = (m_dwBufferSize - dwLastReadPos) + dwReadPosition;
			else
				dwReadyBytes = dwReadPosition - dwLastReadPos;

			if (dwReadyBytes < m_iByteSize) {
				WaitForSingleObject(m_hNotificationEvent, INFINITE);
			}
		} while (dwReadyBytes < m_iByteSize);

    	if (FAILED( hr = m_pDSCaptureBuffer->Lock(dwLastReadPos, m_iByteSize, &aptr1, &nbytes1, &aptr2, &nbytes2, 0)))
    		qFatal("DXAudioInput: Lock from %d (%d bytes)",dwLastReadPos, m_iByteSize);

    	if (aptr1 && nbytes1)
    		CopyMemory(m_psMic, aptr1, nbytes1);

    	if (aptr2 && nbytes2)
    		CopyMemory(m_psMic+nbytes1/2, aptr2, nbytes2);

    	if (FAILED( hr = m_pDSCaptureBuffer->Unlock(aptr1, nbytes1, aptr2, nbytes2)))
    		qFatal("DXAudioInput: Unlock");

		dwLastReadPos = (dwLastReadPos + m_iByteSize) % m_dwBufferSize;

		encodeAudioFrame();
	}

	m_pDSCaptureBuffer->Stop();
}
