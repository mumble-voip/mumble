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

#ifndef _FMODAUDIO_H
#define _FMODAUDIO_H

#include "AudioInput.h"
#include "AudioOutput.h"
#include <fmod.h>
#include <fmod_errors.h>

class FMODSystem;
class FMODAudioOutput;
class FMODAudioInput;

typedef boost::shared_ptr<FMODSystem> FMODSystemPtr;
typedef boost::weak_ptr<FMODSystem> WeakFMODSystemPtr;

class FMODSystem : public QThread {
		friend class FMODAudioOutput;
		friend class FMODAudioInput;
		Q_OBJECT
	protected:
		static QMutex qmSystem;
		static WeakFMODSystemPtr fsSystem;

		FMOD_SYSTEM *system;
		FMODAudioOutput *aoOutput;
		FMODAudioInput *aiInput;
		bool bRunning;
	public:
		QWaitCondition qwWait;

		FMODSystem();
		~FMODSystem();

		static FMODSystemPtr getSystem();
		void run();
};

class FMODAudioInput : public AudioInput {
		friend class FMODSystem;
		Q_OBJECT
	protected:
		FMOD_SOUND *sound;
		unsigned int uiLastRead;
		unsigned int uiBufferSize;
		void frame(FMOD_SYSTEM *system);
		void release();
	public:
		FMODAudioInput();
		~FMODAudioInput();
		void run();
};

class FMODAudioOutput;
class FMODOutputPlayer : public AudioOutputPlayer {
		friend class FMODAudioOutput;
		Q_OBJECT
	protected:
		FMOD_SOUND *sound;
		FMOD_CHANNEL *channel;
		FMODAudioOutput *fao;
		bool bAlive;
		bool bSetup;
		int iAliveHold;
		void setupSound(FMOD_SYSTEM *system);
		void ReadNextFrames(void *data, unsigned int datalen);
		static FMOD_RESULT F_CALLBACK PCMReadCallback(FMOD_SOUND *sound, void *data, unsigned int datalen);
	public:
		FMODOutputPlayer(FMODAudioOutput *, Player *);
		~FMODOutputPlayer();
};


class FMODAudioOutput : public AudioOutput {
		friend class FMODSystem;
		friend class FMODOutputPlayer;
		Q_OBJECT
	protected:
		virtual AudioOutputPlayer *getPlayer(Player *);
		void frame(FMOD_SYSTEM *system);
	public:
		FMODAudioOutput();
		~FMODAudioOutput();
		void run();
};

#else
class DXAudioOutput;
#endif
