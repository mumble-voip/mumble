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

#define NBLOCKS 8

#define MAX(a,b)        ( (a) > (b) ? (a) : (b) )
#define MIN(a,b)        ( (a) < (b) ? (a) : (b) )

static AudioOutput *FMODAudioOutputNew() {
	return new FMODAudioOutput();
}

static AudioOutputRegistrar aorFMOD("FMOD", FMODAudioOutputNew);

FMODOutputPlayer::FMODOutputPlayer(FMODAudioOutput *ao, Player *player) : AudioOutputPlayer(ao, player) {
	fao = static_cast<FMODAudioOutput *>(aoOutput);

	sound = NULL;
	channel = NULL;
}

void FMODOutputPlayer::setupSound() {
	FMOD_CREATESOUNDEXINFO  createsoundexinfo;
	FMOD_RESULT result;
	FMOD_MODE mode = FMOD_2D | FMOD_OPENUSER | FMOD_LOOP_NORMAL | FMOD_HARDWARE | FMOD_CREATESTREAM;
	memset(&createsoundexinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
    createsoundexinfo.cbsize            = sizeof(FMOD_CREATESOUNDEXINFO);              /* required. */
    createsoundexinfo.decodebuffersize  = SAMPLE_RATE * NBLOCKS;                             /* Chunk size of stream update in samples.  This will be the amount of data passed to the user callback. */
    createsoundexinfo.length            = SAMPLE_RATE * NBLOCKS * sizeof(signed short) * 5; /* Length of PCM data in bytes of whole song (for Sound::getLength) */
    createsoundexinfo.numchannels       = 1;                                    /* Number of channels in the sound. */
    createsoundexinfo.defaultfrequency  = SAMPLE_RATE;                                       /* Default playback rate of sound. */
    createsoundexinfo.format            = FMOD_SOUND_FORMAT_PCM16;                     /* Data format of sound. */
    createsoundexinfo.pcmreadcallback   = PCMReadCallback;                             /* User callback for reading. */
    createsoundexinfo.userdata = this;


    result = FMOD_System_CreateSound(fao->system, 0, mode, &createsoundexinfo, &sound);
    result = FMOD_System_PlaySound(fao->system, FMOD_CHANNEL_FREE, sound, 0, &channel);

	qWarning("FMODOutputPlayer: %s initialized", qPrintable(p->qsName));
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
	short *buffer_orig = psBuffer;
	psBuffer = static_cast<short *>(data);

	bool alive;

	while(datalen) {
		alive = alive && decodeNextFrame();
		psBuffer += iFrameSize;
		datalen -= iByteSize;
	}

	psBuffer = buffer_orig;
}


FMODAudioOutput::FMODAudioOutput() {
    FMOD_RESULT             result;

    result = FMOD_System_Create(&system);
    qWarning("SysCre Res %d", result);
	result = FMOD_System_Init(system, 4, FMOD_INIT_NORMAL, NULL);
    qWarning("SysInit Res %d", result);

	qWarning("FMODAudioOutput: System %p initialized", system);

	bRunning = true;
}

FMODAudioOutput::~FMODAudioOutput() {
    FMOD_RESULT             result;

	qWarning("Going away");

	bRunning = false;
	wipe();

    result = FMOD_System_Close(system);
    result = FMOD_System_Release(system);
}

AudioOutputPlayer *FMODAudioOutput::getPlayer(Player *player) {
	qWarning("Ashhkking for playah");
	return new FMODOutputPlayer(this, player);
}

void FMODAudioOutput::run() {
	while (bRunning) {
		qWarning("Checking lost sheep");
		qrwlOutputs.lockForRead();
		foreach(AudioOutputPlayer *aop, qmOutputs) {
			FMODOutputPlayer *fop=static_cast<FMODOutputPlayer *>(aop);
			if (! fop->sound)
				fop->setupSound();
		}
		qrwlOutputs.unlock();

		qWarning("Running system update on %p", system);
		FMOD_System_Update(system);
		msleep(10);
	}
}
