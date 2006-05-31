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

#include "FMODAudio.h"
#include "Player.h"
#include "Global.h"

#include <fmod_errors.h>

#define NBLOCKS 8

#define MAX(a,b)        ( (a) > (b) ? (a) : (b) )
#define MIN(a,b)        ( (a) < (b) ? (a) : (b) )

static AudioOutput *FMODAudioOutputNew() {
	return new FMODAudioOutput();
}
static AudioInput *FMODAudioInputNew() {
	return new FMODAudioInput();
}

static AudioOutputRegistrar aorFMOD("FMOD", FMODAudioOutputNew);
static AudioInputRegistrar airFMOD("FMOD", FMODAudioInputNew);

WeakFMODSystemPtr FMODSystem::fsSystem;
QMutex FMODSystem::qmSystem;

FMODSystem::FMODSystem() {
	aiInput = NULL;
	aoOutput = NULL;
	bRunning = true;
}

FMODSystem::~FMODSystem() {
	bRunning = false;
	wait();
}

FMODSystemPtr FMODSystem::getSystem() {
	qmSystem.lock();

	FMODSystemPtr ptr = fsSystem.lock();
	if (! ptr) {
		ptr = FMODSystemPtr(new FMODSystem());
		fsSystem = ptr;
		ptr->start(QThread::HighPriority);
	}

	qmSystem.unlock();
	return ptr;
}

void FMODSystem::run() {
    FMOD_RESULT             result;

       result = FMOD_System_Create(&system);
       if (result != FMOD_OK)
       	qWarning("FMODSystem: FMOD_System_Create %d", result);

       	FMOD_System_SetOutput(system, FMOD_OUTPUTTYPE_ALSA);
       if (result != FMOD_OK)
       	qWarning("FMODSystem: FMOD_System_SetOutput %d", result);
       	
	result = FMOD_System_Init(system, 4, FMOD_INIT_NORMAL, NULL);
       if (result != FMOD_OK)
       	qWarning("FMODSystem: FMOD_System_Init %d: %s", result,FMOD_ErrorString(result));

	qWarning("FMODSystem: System initialized");

	while (bRunning) {
		if (aoOutput) {
				if (! aoOutput->bRunning) {
					aoOutput->wipe();
					qwWait.wakeAll();
				} else
					aoOutput->frame(system);
		}
		if (aiInput) {
			if (! aiInput->bRunning) {
				aiInput->release();
				qwWait.wakeAll();
			} else
				aiInput->frame(system);
		}
		FMOD_System_Update(system);
		msleep(10);
	};

    result = FMOD_System_Close(system);
    result = FMOD_System_Release(system);

    qWarning("FMODSystem: Exiting");
}

FMODAudioInput::FMODAudioInput() {
	sound = NULL;
}

FMODAudioInput::~FMODAudioInput() {
	bRunning = false;
	wait();
}

void FMODAudioInput::release() {
	if (sound) {
		qWarning("Releasing FMOD AI");
		FMOD_Sound_Release(sound);
		sound = NULL;
	}
}

void FMODAudioInput::run() {
	FMODSystemPtr sys = FMODSystem::getSystem();
	QMutex m;

	bRunning = true;

	sys->aiInput = this;
	while (bRunning) {
		m.lock();
		sys->qwWait.wait(&m);
		m.unlock();
	}
	sys->aiInput = NULL;
}

void FMODAudioInput::frame(FMOD_SYSTEM *system) {
	if (! sound) {
    	FMOD_CREATESOUNDEXINFO exinfo;
    	FMOD_RESULT result;

    	memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));

    	uiBufferSize = iFrameSize * sizeof(short) * 32;

	    exinfo.cbsize           = sizeof(FMOD_CREATESOUNDEXINFO);
	    exinfo.numchannels      = 1;
	    exinfo.format           = FMOD_SOUND_FORMAT_PCM16;
	    exinfo.defaultfrequency = SAMPLE_RATE;
	    exinfo.length           = uiBufferSize;

		result = FMOD_System_CreateSound(system, 0, FMOD_2D | FMOD_SOFTWARE | FMOD_OPENUSER, &exinfo, &sound);
		result = FMOD_System_RecordStart(system, sound, true);

		uiLastRead = 0;
	}

	unsigned int recordpos;

	unsigned int ready;

	do {
		FMOD_System_GetRecordPosition(system, &recordpos);
		recordpos *= 2;
		if (recordpos <= uiLastRead)
			ready = (uiBufferSize - uiLastRead) + recordpos;
		else
			ready = recordpos - uiLastRead;
			
			
		if (ready > iByteSize) {
		    void *ptr1, *ptr2;
		    unsigned int len1, len2;

			FMOD_Sound_Lock(sound, uiLastRead, iByteSize, &ptr1, &ptr2, &len1, &len2);

	    	if (ptr1 && len1)
	    		CopyMemory(psMic, ptr1, len1);

	    	if (ptr2 && len2)
	    		CopyMemory(psMic+len1/2, ptr2, len2);

			FMOD_Sound_Unlock(sound, ptr1, ptr2, len1, len2);

			encodeAudioFrame();

			uiLastRead = (uiLastRead + iByteSize) % uiBufferSize;
		}

	} while(ready >= iByteSize * 2);
}

FMODOutputPlayer::FMODOutputPlayer(FMODAudioOutput *ao, Player *player) : AudioOutputPlayer(ao, player) {
	fao = static_cast<FMODAudioOutput *>(aoOutput);

	sound = NULL;
	channel = NULL;
}

void FMODOutputPlayer::setupSound(FMOD_SYSTEM *system) {
	FMOD_CREATESOUNDEXINFO  createsoundexinfo;
	FMOD_RESULT result;
	FMOD_MODE mode = FMOD_2D | FMOD_OPENUSER | FMOD_LOOP_NORMAL | FMOD_SOFTWARE | FMOD_CREATESTREAM;
	memset(&createsoundexinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
    createsoundexinfo.cbsize            = sizeof(FMOD_CREATESOUNDEXINFO);              /* required. */
    createsoundexinfo.decodebuffersize  = iFrameSize * 5;                             /* Chunk size of stream update in samples.  This will be the amount of data passed to the user callback. */
    createsoundexinfo.length            = iFrameSize * sizeof(signed short) * 10; /* Length of PCM data in bytes of whole song (for Sound::getLength) */
    createsoundexinfo.numchannels       = 1;                                    /* Number of channels in the sound. */
    createsoundexinfo.defaultfrequency  = SAMPLE_RATE;                                       /* Default playback rate of sound. */
    createsoundexinfo.format            = FMOD_SOUND_FORMAT_PCM16;                     /* Data format of sound. */
    createsoundexinfo.pcmreadcallback   = PCMReadCallback;                             /* User callback for reading. */
    createsoundexinfo.userdata = this;

	bSetup = true;

    result = FMOD_System_CreateSound(system, 0, mode, &createsoundexinfo, &sound);
       if (result != FMOD_OK)
       	qWarning("FMODOutputPlayer: FMOD_System_CreateSound %d", result);
    result = FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, sound, 0, &channel);
       if (result != FMOD_OK)
       	qWarning("FMODOutputPlayer: FMOD_System_PlaySound %d", result);

    bSetup = false;

	qWarning("FMODOutputPlayer: %s initialized", qPrintable(p->qsName));

	bAlive = true;
	iAliveHold = 0;
}

FMODOutputPlayer::~FMODOutputPlayer() {
	if (sound) {
		FMOD_Sound_Release(sound);
	}

	qWarning("FMODOutputPlayer: %s: Removed", qPrintable(p->qsName));
}

FMOD_RESULT F_CALLBACK FMODOutputPlayer::PCMReadCallback(FMOD_SOUND *sound, void *data, unsigned int datalen) {
	void *udata;
	FMOD_Sound_GetUserData(sound, &udata);
	FMODOutputPlayer *aop = static_cast<FMODOutputPlayer *>(udata);
	aop->ReadNextFrames(data,datalen);
	return FMOD_OK;
}

void FMODOutputPlayer::ReadNextFrames(void *data, unsigned int datalen) {
	if (bSetup) {
		ZeroMemory(data, datalen);
		return;
	}

	short *buffer_orig = psBuffer;
	psBuffer = static_cast<short *>(data);

	bool alive;

	while(datalen) {
		alive = decodeNextFrame();
		psBuffer += iFrameSize;
		datalen -= iByteSize;

		if (alive)
			iAliveHold = 0;
		else {
			iAliveHold++;
			if (iAliveHold > 50) {
				bAlive = false;
			}
		}

	}

	psBuffer = buffer_orig;
}


FMODAudioOutput::FMODAudioOutput() {
	qWarning("FMODAudioOutput: Initialized");
	bRunning = true;
}

FMODAudioOutput::~FMODAudioOutput() {
	bRunning = false;
	wait();
}

AudioOutputPlayer *FMODAudioOutput::getPlayer(Player *player) {
	return new FMODOutputPlayer(this, player);
}

void FMODAudioOutput::frame(FMOD_SYSTEM *system) {
	qrwlOutputs.lockForRead();
	foreach(AudioOutputPlayer *aop, qmOutputs) {
		FMODOutputPlayer *fop=static_cast<FMODOutputPlayer *>(aop);
		if (! fop->sound)
			fop->setupSound(system);
		else if (! fop->bAlive) {
			qrwlOutputs.unlock();
			removeBuffer(fop->p);
			qrwlOutputs.lockForRead();
			break;
		}
	}
	qrwlOutputs.unlock();
}

void FMODAudioOutput::run() {
	FMODSystemPtr sys = FMODSystem::getSystem();
	QMutex m;

	sys->aoOutput = this;

	while (bRunning) {
		m.lock();
		sys->qwWait.wait(&m);
		m.unlock();
	}
	sys->aoOutput = NULL;
}
