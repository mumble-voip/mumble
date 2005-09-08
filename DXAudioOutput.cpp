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

#include "DXAudioOutput.h"
#include "MainWindow.h"

#define NBLOCKS 8
#define MAX(a,b)        ( (a) > (b) ? (a) : (b) )
#define MIN(a,b)        ( (a) < (b) ? (a) : (b) )

static AudioOutput *DXAudioOutputNew() {
	return new DXAudioOutput();
}

static QWidget *DXAudioOutputConfig(QWidget *parent) {
	return new QWidget(parent);
}

AudioOutputRegistrar aorDX("DirectSound", DXAudioOutputNew, DXAudioOutputConfig);

DXAudioOutput::DXAudioOutput() {
	HRESULT hr;
    DSBUFFERDESC        dsbdesc;
    WAVEFORMATEX wfx;
    WAVEFORMATEX wfxSet;

	    // Create IDirectSound using the preferred sound device
	if( FAILED( hr = DirectSoundCreate8( NULL, &pDS, NULL ) ) )
		qFatal("DXAudioOutput: DirectSoundCreate");

	    // Set coop level to DSSCL_PRIORITY
	if( FAILED( hr = pDS->SetCooperativeLevel( g.mw->winId(), DSSCL_PRIORITY ) ) )
		qFatal("DXAudioOutput: SetCooperativeLevel");

    // Obtain primary buffer
    ZeroMemory( &dsbdesc, sizeof(DSBUFFERDESC) );
    dsbdesc.dwSize  = sizeof(DSBUFFERDESC);
    dsbdesc.dwFlags = DSBCAPS_PRIMARYBUFFER;

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

	qWarning("DXAudioOutput: Primary buffer of %ld Hz, %d channels, %d bits",wfxSet.nSamplesPerSec,wfxSet.nChannels,wfxSet.wBitsPerSample);

	bRunning = true;
}

DXAudioOutput::~DXAudioOutput() {
	if (pDSBPrimary)
		pDSBPrimary->Release();
	if (pDS)
		pDS->Release();
}

AudioOutputPlayer *DXAudioOutput::getPlayer(short sId) {
	DXAudioOutputPlayer *daopPlayer = new DXAudioOutputPlayer(this, sId);
	daopPlayer->start();
	return daopPlayer;
}

DXAudioOutputPlayer::DXAudioOutputPlayer(AudioOutput *aoOutput, short sId) : AudioOutputPlayer(aoOutput, sId) {
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
   	dsbd.dwBufferBytes = iFrameSize * 2 * NBLOCKS;
    dsbd.lpwfxFormat     = &wfx;

    // Create the DirectSound buffer
    if( FAILED( hr = dxAudio->pDS->CreateSoundBuffer( &dsbd, &pDSBOutput, NULL ) ) )
    	qFatal("DXAudioOutputPlayer: CreateSoundBuffer (Secondary): 0x%08lx", hr);

	DSBPOSITIONNOTIFY    aPosNotify[NBLOCKS];

	hNotificationEvent = CreateEvent( NULL, FALSE, FALSE, NULL );

	for(int i=0;i<NBLOCKS;i++) {
		aPosNotify[i].dwOffset = iFrameSize * 2 * i;
		aPosNotify[i].hEventNotify = hNotificationEvent;
	}

	if( FAILED( hr = pDSBOutput->QueryInterface( IID_IDirectSoundNotify, (VOID**)&pDSNotify ) ) )
		qFatal("DXAudioOutputPlayer: QueryInterface (Notify)");

    if( FAILED( hr = pDSNotify->SetNotificationPositions( NBLOCKS, aPosNotify ) ) )
    	qFatal("DXAudioOutputPlayer: SetNotificationPositions");

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
	wait();
	if (pDSNotify)
		pDSNotify->Release();
	if (pDSBOutput)
		pDSBOutput->Release();
}

void DXAudioOutputPlayer::run() {
	int playblock;
	int nowriteblock;
	int lastwriteblock = NBLOCKS-1;
	int iAudioDecodeLag = 4;
	DWORD dwPlayPosition, dwWritePosition;
    HRESULT             hr;

    if (FAILED( hr = pDSBOutput->Play(0, 0, DSBPLAY_LOOPING)))
    	qFatal("DXAUdioOutputPlayer: Play");

	bRunning = true;

	while (bRunning && dxAudio->bRunning) {
		if( FAILED( hr = pDSBOutput->GetCurrentPosition(&dwPlayPosition, &dwWritePosition ) ) )
				qFatal("DXAudioOutputPlayer: GetCurrentPosition");

		playblock = dwWritePosition / iByteSize;
		nowriteblock = (playblock + iAudioDecodeLag) % NBLOCKS;

		for(int block=(lastwriteblock + 1) % NBLOCKS;block!=nowriteblock;block=(block + 1) % NBLOCKS) {
			lastwriteblock = block;

			decodeNextFrame();

		    LPVOID aptr1, aptr2;
		    DWORD nbytes1, nbytes2;

		    if (FAILED( hr = pDSBOutput->Lock(block * iByteSize, iByteSize, &aptr1, &nbytes1, &aptr2, &nbytes2, 0)))
		    	qFatal("DXAudioOutput: Lock block %d (%d bytes)",block, iByteSize);
			if (aptr1 && nbytes1)
				CopyMemory(aptr1, psBuffer, MIN(iByteSize, (int) nbytes1));
			if (aptr2 && nbytes2)
				CopyMemory(aptr2, psBuffer+(nbytes1/2), MIN(iByteSize-nbytes1, nbytes2));
		    if (FAILED( hr = pDSBOutput->Unlock(aptr1, nbytes1, aptr2, nbytes2)))
		    	qFatal("DXAudioOutput: Unlock");

			if( FAILED( hr = pDSBOutput->GetCurrentPosition(&dwPlayPosition, &dwWritePosition ) ) )
				qFatal("DXAudioOutputPlayer: GetCurrentPosition");

			playblock = dwWritePosition / iByteSize;
			nowriteblock = (playblock + iAudioDecodeLag) % NBLOCKS;
		}
		WaitForSingleObject(hNotificationEvent, INFINITE);
	}
}
