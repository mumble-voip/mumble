/* Copyright (C) 2005-2007, Thorvald Natvig <thorvald@natvig.com>

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

#ifndef _AUDIOOUTPUT_H
#define _AUDIOOUTPUT_H

// AudioOutput depends on Player being valid. This means it's important
// to removeBuffer from here BEFORE MainWindow gets any PlayerLeft
// messages. Any decendant player should feel free to remove unused
// AudioOutputPlayer objects; it's better to recreate them than
// having them use resources while unused.

#include "Audio.h"

class AudioOutput;
class Player;

typedef AudioOutput *(*AudioOutputRegistrarNew)();
typedef boost::shared_ptr<AudioOutput> AudioOutputPtr;

class AudioOutputRegistrar {
	public:
		static QMap<QString, AudioOutputRegistrarNew> *qmNew;
		static QString current;
		AudioOutputRegistrar(QString name, AudioOutputRegistrarNew n);
		static AudioOutputPtr newFromChoice(QString choice = QString());
};

class AudioOutputPlayer : public QObject {
	friend class AudioOutput;
	Q_OBJECT
	protected:
		Player *p;

		SpeexBits sbBits;
		unsigned int iFrameSize;
		unsigned int iByteSize;
		int iFrameCounter;
		QMutex qmJitter;
		SpeexJitter sjJitter;
		void *dsDecState;
		AudioOutput *aoOutput;

		short *psBuffer;
		bool bSpeech;

		int iMissCount;
		float fPos[3];

		bool decodeNextFrame();
	public:
		void addFrameToBuffer(QByteArray &, int iSeq);
		AudioOutputPlayer(AudioOutput *, Player *);
		~AudioOutputPlayer();
};

class AudioOutput : public QThread {
	friend class FMODSystem;
	Q_OBJECT
	protected:
		bool bRunning;
		QReadWriteLock qrwlOutputs;
		QHash<Player *, AudioOutputPlayer *> qmOutputs;
		virtual AudioOutputPlayer *getPlayer(Player *) = 0;
	public:
		void wipe();

		AudioOutput();
		~AudioOutput();
		void addFrameToBuffer(Player *, QByteArray &, int iSeq);
		void removeBuffer(Player *);
		void run() = 0;
};

#else
class AudioInput;
#endif
