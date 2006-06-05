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

#include "ALSAAudio.h"
#include "Player.h"
#include "Global.h"

#define NBLOCKS 8

#define MAX(a,b)        ( (a) > (b) ? (a) : (b) )
#define MIN(a,b)        ( (a) < (b) ? (a) : (b) )

static AudioOutput *ALSAAudioOutputNew() {
	return new ALSAAudioOutput();
}
static AudioInput *ALSAAudioInputNew() {
	return new ALSAAudioInput();
}

static AudioOutputRegistrar aorALSA("ALSA", ALSAAudioOutputNew);
static AudioInputRegistrar airALSA("ALSA", ALSAAudioInputNew);

ALSAAudioInput::ALSAAudioInput() {
}

ALSAAudioInput::~ALSAAudioInput() {
	// Signal input thread to end
	bRunning = false;
	wait();
}

void ALSAAudioInput::run() {
	bRunning = true;

	// Open device and stuff

	while (bRunning) {
		// Grab input and process.
		
		// Input put in 'psMic', size iByteSize (iFrameSize samples)

			encodeAudioFrame();

	}
	
	// Close device and stuff
}

ALSAOutputPlayer::ALSAOutputPlayer(ALSAAudioOutput *ao, Player *player) : AudioOutputPlayer(ao, player) {
	aao = static_cast<ALSAAudioOutput *>(aoOutput);
}

ALSAOutputPlayer::~ALSAOutputPlayer() {
	// If playing, stop
	qWarning("ALSAOutputPlayer: %s: Removed", qPrintable(p->qsName));
}

ALSAAudioOutput::ALSAAudioOutput() {
	qWarning("ALSAAudioOutput: Initialized");
}

ALSAAudioOutput::~ALSAAudioOutput() {
	bRunning = false;
	// Call destructor of all children
	wipe();
	// Wait for terminate
	wait();
}

AudioOutputPlayer *ALSAAudioOutput::getPlayer(Player *player) {
	return new ALSAOutputPlayer(this, player);
}

void ALSAAudioOutput::run() {
	bRunning = true;

	while (bRunning) {
		// Check all children for events and process.
	}
	
	
}
