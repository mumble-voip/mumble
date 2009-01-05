/* Copyright (C) 2005-2009, Thorvald Natvig <thorvald@natvig.com>

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
#include "Plugins.h"
#include "Player.h"
#include "Global.h"

#undef FAILED
#define FAILED(Status) (static_cast<HRESULT>(Status)<0)

// #define MY_DEFERRED DS3D_DEFERRED
#define MY_DEFERRED DS3D_IMMEDIATE


#define NBLOCKS 50
#define MAX(a,b)        ( (a) > (b) ? (a) : (b) )
#define MIN(a,b)        ( (a) < (b) ? (a) : (b) )

class DXAudioOutputRegistrar : public AudioOutputRegistrar {
	public:
		DXAudioOutputRegistrar();
		virtual AudioOutput *create();
		virtual const QList<audioDevice> getDeviceChoices();
		virtual void setDeviceChoice(const QVariant &, Settings &);

};

// Static singleton
static DXAudioOutputRegistrar airDX;

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

DXAudioOutput::DXAudioOutput() {
	bRunning = true;
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
	LPDIRECTSOUNDNOTIFY8       pDSNotify = NULL;

	DWORD	dwBufferSize;
	DWORD	dwLastWritePos;
	DWORD	dwLastPlayPos;
	DWORD	dwTotalPlayPos;
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
		qWarning("DXAudioOutput: DirectSoundCreate");
		goto cleanup;
	} else if (FAILED(hr = pDS->SetCooperativeLevel(g.mw->winId(), DSSCL_PRIORITY))) {
		qWarning("DXAudioOutput: SetCooperativeLevel");
		goto cleanup;
	} else if (FAILED(hr = pDS->CreateSoundBuffer(&dsbdesc, &pDSBPrimary, NULL))) {
		qWarning("DXAudioOutput: CreateSoundBuffer (Primary) : 0x%08lx", hr);
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
		dwMask = KSAUDIO_SPEAKER_MONO;

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
	wfx.Format.nChannels = qMax(ns, 1);
	wfx.Format.nSamplesPerSec = SAMPLE_RATE;
	wfx.Format.nBlockAlign = sizeof(short) * wfx.Format.nChannels;
	wfx.Format.nAvgBytesPerSec = wfx.Format.nSamplesPerSec * wfx.Format.nBlockAlign;
	wfx.Format.wBitsPerSample = 16;

	if (FAILED(hr = pDSBPrimary->SetFormat(reinterpret_cast<WAVEFORMATEX *>(&wfx)))) {
		qWarning("DXAudioOutput: SetFormat");
		goto cleanup;
	}
	if (FAILED(hr = pDSBPrimary->GetFormat(reinterpret_cast<WAVEFORMATEX *>(&wfxSet), sizeof(wfxSet), NULL))) {
		qWarning("DXAudioOutput: GetFormat");
		goto cleanup;
	}

	qWarning("DXAudioOutput: Primary buffer of %ld Hz, %d channels, %d bits",wfxSet.Format.nSamplesPerSec,wfxSet.Format.nChannels,wfxSet.Format.wBitsPerSample);

	ZeroMemory(&wfx, sizeof(wfx));
	wfx.Format.wFormatTag = WAVE_FORMAT_EXTENSIBLE;
	wfx.Format.nChannels = ns;
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
		qWarning("DXAudioOutputPlayer: CreateSoundBuffer (Secondary): 0x%08lx", hr);
		goto cleanup;
	}


	if (FAILED(hr = pDSBOutput->QueryInterface(IID_IDirectSoundNotify, reinterpret_cast<void **>(&pDSNotify)))) {
		qWarning("DXAudioOutputPlayer: QueryInterface (Notify)");
		goto cleanup;
	}

	qWarning("DXAudioOutputPlayer: New %dHz output buffer of %ld bytes", SAMPLE_RATE, dsbdesc.dwBufferBytes);

	if (failed)
		g.mw->msgBox(tr("Opening chosen DirectSound Output failed. Default device will be used."));

	initializeMixer(chanmasks, bHead);

	if (FAILED(hr = pDSBOutput->Lock(0, 0, &aptr1, &nbytes1, &aptr2, &nbytes2, DSBLOCK_ENTIREBUFFER))) {
		qWarning("DXAudioOutputPlayer: Initial Lock");
		goto cleanup;
	}

	dwBufferSize = nbytes1 + nbytes2;
	if (aptr1)
		ZeroMemory(aptr1, nbytes1);
	if (aptr2)
		ZeroMemory(aptr2, nbytes2);

	if (FAILED(hr = pDSBOutput->Unlock(aptr1, nbytes1, aptr2, nbytes2))) {
		qWarning("DXAudioOutputPlayer: Initial Unlock");
		goto cleanup;
	}

	if (FAILED(hr = pDSBOutput->Play(0, 0, DSBPLAY_LOOPING))) {
		qWarning("DXAudioOutputPlayer: Play");
		goto cleanup;
	}

	dwLastWritePos = 0;
	dwLastPlayPos = 0;
	dwTotalPlayPos = 0;

	iLastwriteblock = (NBLOCKS - 1 + g.s.iOutputDelay) % NBLOCKS;

	bOk = true;

	float safety = 2.0f;
	bool didsleep = false;
	bool firstsleep = false;

	while (bRunning && ! FAILED(hr)) {
		if (FAILED(hr = pDSBOutput->GetCurrentPosition(&dwPlayPosition, &dwWritePosition))) {
			qWarning("DXAudioOutputPlayer: GetCurrentPosition");
			break;
		}

		playblock = dwWritePosition / iByteSize;
		nowriteblock = (playblock + g.s.iOutputDelay + 1) % NBLOCKS;

		for (int block=(iLastwriteblock + 1) % NBLOCKS;(!FAILED(hr)) && (block!=nowriteblock);block=(block + 1) % NBLOCKS) {
			iLastwriteblock = block;

			if (FAILED(hr = pDSBOutput->Lock(block * iByteSize, iByteSize, &aptr1, &nbytes1, &aptr2, &nbytes2, 0))) {
				qWarning("DXAudioOutput: Lock block %u (%d bytes)",block, iByteSize);
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
				qWarning("DXAudioOutput: Unlock %p(%lu) %p(%lu)",aptr1,nbytes1,aptr2,nbytes2);
				break;
			}

			if (FAILED(hr = pDSBOutput->GetCurrentPosition(&dwPlayPosition, &dwWritePosition))) {
				qWarning("DXAudioOutputPlayer: GetCurrentPosition");
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

