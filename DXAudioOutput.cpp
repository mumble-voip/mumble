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

#include <QMessageBox>
#include <math.h>
#include "DXAudioOutput.h"
#include "MainWindow.h"
#include "Plugins.h"
#include "Global.h"

#undef FAILED
#define FAILED(Status) (static_cast<HRESULT>(Status)<0)

#define NBLOCKS 50
#define MAX(a,b)        ( (a) > (b) ? (a) : (b) )
#define MIN(a,b)        ( (a) < (b) ? (a) : (b) )

static AudioOutput *DXAudioOutputNew() {
	return new DXAudioOutput();
}

static ConfigWidget *DXAudioOutputConfig(QWidget *parent) {
	return NULL;
}

AudioOutputRegistrar aorDX("DirectSound", DXAudioOutputNew, DXAudioOutputConfig);

DXAudioOutput::DXAudioOutput() {
	HRESULT hr;
    DSBUFFERDESC        dsbdesc;
    WAVEFORMATEX wfx;
    WAVEFORMATEX wfxSet;

	pDS = NULL;

	bool failed = false;

	if (! g.s.qbaDXOutput.isEmpty()) {
		LPGUID lpguid = reinterpret_cast<LPGUID>(g.s.qbaDXOutput.data());
	    if( FAILED( hr = DirectSoundCreate8(lpguid, &pDS, NULL))) {
			failed = true;
		}
	}

    // Create IDirectSound using the preferred sound device
	if (! pDS)
		if( FAILED( hr = DirectSoundCreate8( &DSDEVID_DefaultVoicePlayback, &pDS, NULL ) ) )
			qFatal("DXAudioOutput: DirectSoundCreate");

	if (failed)
		QMessageBox::warning(NULL, tr("Mumble"), tr("Opening chosen DirectSound Output failed. Using defaults."), QMessageBox::Ok, QMessageBox::NoButton);


   // Set coop level to DSSCL_PRIORITY
	if( FAILED( hr = pDS->SetCooperativeLevel( g.mw->winId(), DSSCL_PRIORITY ) ) )
		qFatal("DXAudioOutput: SetCooperativeLevel");

    // Obtain primary buffer
    ZeroMemory( &dsbdesc, sizeof(DSBUFFERDESC) );
    dsbdesc.dwSize  = sizeof(DSBUFFERDESC);
    dsbdesc.dwFlags = DSBCAPS_PRIMARYBUFFER;
    if (g.s.a3dModel != Settings::None)
    	dsbdesc.dwFlags |= DSBCAPS_CTRL3D;

    if( FAILED( hr = pDS->CreateSoundBuffer( &dsbdesc, &pDSBPrimary, NULL ) ) )
    	qFatal("DXAudioOutput: CreateSoundBuffer (Primary) : 0x%08lx", hr);


   	ZeroMemory( &wfxSet, sizeof(wfxSet) );
    wfxSet.wFormatTag = WAVE_FORMAT_PCM;

   	ZeroMemory( &wfx, sizeof(wfx) );
    wfx.wFormatTag = WAVE_FORMAT_PCM;

    wfx.nChannels = 1;
    wfx.nSamplesPerSec = SAMPLE_RATE;
    wfx.nBlockAlign = 2;
    wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;
    wfx.wBitsPerSample = 16;

	if( FAILED( hr = pDSBPrimary->SetFormat( &wfx ) ) )
    	qFatal("DXAudioOutput: SetFormat");

    if( FAILED( hr = pDSBPrimary->GetFormat( &wfxSet, sizeof(wfxSet), NULL ) ) )
       	qFatal("DXAudioOutput: GetFormat");

	p3DListener = NULL;
    if (g.s.a3dModel != Settings::None) {
		if (FAILED(hr = pDSBPrimary->QueryInterface(IID_IDirectSound3DListener8, reinterpret_cast<void **>(&p3DListener)))) {
			qWarning("DXAudioOutput: QueryInterface (DirectSound3DListener8): 0x%08lx",hr);
		} else {
			p3DListener->SetRolloffFactor(g.s.fDXRollOff, DS3D_DEFERRED);
			p3DListener->SetDopplerFactor(g.s.fDXDoppler, DS3D_DEFERRED);
			p3DListener->CommitDeferredSettings();
		}
	}

	qWarning("DXAudioOutput: Primary buffer of %ld Hz, %d channels, %d bits",wfxSet.nSamplesPerSec,wfxSet.nChannels,wfxSet.wBitsPerSample);

	bRunning = true;
}

DXAudioOutput::~DXAudioOutput() {
	bRunning = false;
	wipe();

	if (pDSBPrimary)
		pDSBPrimary->Release();
	if (pDS)
		pDS->Release();
}

AudioOutputPlayer *DXAudioOutput::getPlayer(short sId) {
	DXAudioOutputPlayer *daopPlayer = new DXAudioOutputPlayer(this, sId);
	daopPlayer->start(QThread::HighPriority);
	return daopPlayer;
}

void DXAudioOutput::updateListener() {
	if (! p3DListener)
		return;

	HRESULT hr;
	DS3DLISTENER li;
	Plugins *p = g.p;
	li.dwSize=sizeof(li);
	if (p->bValid && ! g.bCenterPosition && p3DListener) {
		// Only set this if we need to. If centerposition is on, or we don't have valid data,
		// the 3d mode for the buffers will be disabled, so don't bother with updates.
		p3DListener->SetPosition(p->fPosition[0], p->fPosition[1], p->fPosition[2], DS3D_DEFERRED);
		p3DListener->SetVelocity(p->fVelocity[0], p->fVelocity[1], p->fVelocity[2], DS3D_DEFERRED);
		p3DListener->SetOrientation(p->fFront[0], p->fFront[1], p->fFront[2],
									p->fTop[0], p->fTop[1], p->fTop[2], DS3D_DEFERRED);
	}
	if (FAILED(hr =p3DListener->CommitDeferredSettings()))
		qWarning("DXAudioOutputPlayer: CommitDeferrredSettings failed 0x%08lx", hr);
}

DXAudioOutputPlayer::DXAudioOutputPlayer(AudioOutput *ao, short id) : AudioOutputPlayer(ao, id) {
	dxAudio = static_cast<DXAudioOutput *>(aoOutput);

    DSBUFFERDESC dsbd;
    WAVEFORMATEX wfx;
    HRESULT hr;

    ZeroMemory( &dsbd, sizeof(DSBUFFERDESC) );

   	ZeroMemory( &wfx, sizeof(wfx) );
    wfx.wFormatTag = WAVE_FORMAT_PCM;
    wfx.nChannels = 1;
    wfx.nBlockAlign = 2;
    wfx.nSamplesPerSec = SAMPLE_RATE;
    wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;
    wfx.wBitsPerSample = 16;

    dsbd.dwSize          = sizeof(DSBUFFERDESC);
    dsbd.dwFlags         = DSBCAPS_GLOBALFOCUS|DSBCAPS_GETCURRENTPOSITION2;
	dsbd.dwFlags	 |= DSBCAPS_CTRLPOSITIONNOTIFY;
	if (dxAudio->p3DListener)
		dsbd.dwFlags	 |= DSBCAPS_CTRL3D;
   	dsbd.dwBufferBytes = iFrameSize * 2 * NBLOCKS;
    dsbd.lpwfxFormat     = &wfx;
	if (dxAudio->p3DListener) {
		switch (g.s.a3dModel) {
			case Settings::None:
			case Settings::Panning:
			    dsbd.guid3DAlgorithm = DS3DALG_NO_VIRTUALIZATION;
			    break;
			case Settings::Light:
			    dsbd.guid3DAlgorithm = DS3DALG_HRTF_LIGHT;
			    break;
			case Settings::Full:
			    dsbd.guid3DAlgorithm = DS3DALG_HRTF_FULL;
			    break;
		}
	}

	hNotificationEvent = CreateEvent( NULL, FALSE, FALSE, NULL );

    // Create the DirectSound buffer
    if( FAILED( hr = dxAudio->pDS->CreateSoundBuffer( &dsbd, &pDSBOutput, NULL ) ) )
    	qFatal("DXAudioOutputPlayer: CreateSoundBuffer (Secondary): 0x%08lx", hr);

	DSBPOSITIONNOTIFY    aPosNotify[NBLOCKS];

	for(int i=0;i<NBLOCKS;i++) {
		aPosNotify[i].dwOffset = iFrameSize * 2 * i;
		aPosNotify[i].hEventNotify = hNotificationEvent;
	}

	if( FAILED( hr = pDSBOutput->QueryInterface( IID_IDirectSoundNotify, reinterpret_cast<VOID**>(&pDSNotify) ) ) )
		qFatal("DXAudioOutputPlayer: QueryInterface (Notify)");

    if( FAILED( hr = pDSNotify->SetNotificationPositions( NBLOCKS, aPosNotify ) ) )
    	qFatal("DXAudioOutputPlayer: SetNotificationPositions");

	pDS3dBuffer = NULL;
	if (dxAudio->p3DListener) {
		if (FAILED(pDSBOutput->QueryInterface(IID_IDirectSound3DBuffer8, reinterpret_cast<void **>(&pDS3dBuffer))))
			qFatal("DXAudioOutputPlayer: QueryInterface (DirectSound3DBuffer)");

		pDS3dBuffer->SetMinDistance(g.s.fDXMinDistance, DS3D_DEFERRED);
		pDS3dBuffer->SetMaxDistance(g.s.fDXMaxDistance, DS3D_DEFERRED);
	}

    LPVOID aptr1, aptr2;
    DWORD nbytes1, nbytes2;

    if (FAILED( hr = pDSBOutput->Lock(0, 0, &aptr1, &nbytes1, &aptr2, &nbytes2, DSBLOCK_ENTIREBUFFER)))
    	qFatal("DXAudioOutputPlayer: Initial Lock");

    dwBufferSize = nbytes1 + nbytes2;
    if (aptr1)
   		ZeroMemory(aptr1, nbytes1);
   	if (aptr2)
   		ZeroMemory(aptr2, nbytes2);

    if (FAILED( hr = pDSBOutput->Unlock(aptr1, nbytes1, aptr2, nbytes2)))
    	qFatal("DXAudioOutputPlayer: Initial Unlock");

	dwLastWritePos = 0;
	dwLastPlayPos = 0;
	dwTotalPlayPos = 0;

	qWarning("DXAudioOutputPlayer: New %dHz output buffer of %ld bytes", SAMPLE_RATE, dwBufferSize);
}

DXAudioOutputPlayer::~DXAudioOutputPlayer() {
	bRunning = false;
	SetEvent(hNotificationEvent);
	wait();
	if (pDSNotify)
		pDSNotify->Release();
	if (pDSBOutput)
		pDSBOutput->Release();
	CloseHandle(hNotificationEvent);
}

void DXAudioOutputPlayer::run() {
	int playblock;
	int nowriteblock;
	int lastwriteblock = NBLOCKS-1;
	DWORD dwPlayPosition, dwWritePosition;
    HRESULT             hr;

    if (FAILED( hr = pDSBOutput->Play(0, 0, DSBPLAY_LOOPING)))
    	qFatal("DXAUdioOutputPlayer: Play");

	bRunning = true;

	while (bRunning && dxAudio->bRunning) {
		if( FAILED( hr = pDSBOutput->GetCurrentPosition(&dwPlayPosition, &dwWritePosition ) ) )
				qFatal("DXAudioOutputPlayer: GetCurrentPosition");

		playblock = dwWritePosition / iByteSize;
		nowriteblock = (playblock + g.s.iDXOutputDelay + 1) % NBLOCKS;

		for(int block=(lastwriteblock + 1) % NBLOCKS;block!=nowriteblock;block=(block + 1) % NBLOCKS) {
			lastwriteblock = block;

			decodeNextFrame();
			dxAudio->updateListener();

			if (pDS3dBuffer) {
				bool center = g.bCenterPosition;
				DWORD mode;

				pDS3dBuffer->GetMode(&mode);
				if (! center) {
					if ((fabs(fPos[0]) < 0.1) && (fabs(fPos[1]) < 0.1) && (fabs(fPos[2]) < 0.1))
						center = true;
					else if (! g.p->bValid)
						center = true;
				}
				if (center) {
					if (mode != DS3DMODE_DISABLE)
						pDS3dBuffer->SetMode(DS3DMODE_DISABLE, DS3D_DEFERRED);
				} else {
					if (mode != DS3DMODE_NORMAL)
						pDS3dBuffer->SetMode(DS3DMODE_NORMAL, DS3D_DEFERRED);
					pDS3dBuffer->SetPosition(fPos[0], fPos[1], fPos[2], DS3D_DEFERRED);
					pDS3dBuffer->SetVelocity(fVel[0], fVel[1], fVel[2], DS3D_DEFERRED);
				}
			}

		    LPVOID aptr1, aptr2;
		    DWORD nbytes1, nbytes2;

		    if (FAILED( hr = pDSBOutput->Lock(block * iByteSize, iByteSize, &aptr1, &nbytes1, &aptr2, &nbytes2, 0)))
		    	qFatal("DXAudioOutput: Lock block %d (%d bytes)",block, iByteSize);
			if (aptr1 && nbytes1)
				CopyMemory(aptr1, psBuffer, MIN(iByteSize, nbytes1));
			if (aptr2 && nbytes2)
				CopyMemory(aptr2, psBuffer+(nbytes1/2), MIN(iByteSize-nbytes1, nbytes2));
		    if (FAILED( hr = pDSBOutput->Unlock(aptr1, nbytes1, aptr2, nbytes2)))
		    	qFatal("DXAudioOutput: Unlock");

			if( FAILED( hr = pDSBOutput->GetCurrentPosition(&dwPlayPosition, &dwWritePosition ) ) )
				qFatal("DXAudioOutputPlayer: GetCurrentPosition");

			playblock = dwWritePosition / iByteSize;
			nowriteblock = (playblock + g.s.iDXOutputDelay + 1) % NBLOCKS;
		}
		WaitForSingleObject(hNotificationEvent, INFINITE);
	}
}
