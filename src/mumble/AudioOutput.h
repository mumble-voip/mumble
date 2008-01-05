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

#ifndef _AUDIOOUTPUT_H
#define _AUDIOOUTPUT_H

// AudioOutput depends on Player being valid. This means it's important
// to removeBuffer from here BEFORE MainWindow gets any PlayerLeft
// messages. Any decendant player should feel free to remove unused
// AudioOutputPlayer objects; it's better to recreate them than
// having them use resources while unused.

#include "Audio.h"

class AudioOutput;
class ClientPlayer;

typedef boost::shared_ptr<AudioOutput> AudioOutputPtr;

class AudioOutputRegistrar {
	public:
		static QMap<QString, AudioOutputRegistrar *> *qmNew;
		static QString current;
		static AudioOutputPtr newFromChoice(QString choice = QString());

		const QString name;

		AudioOutputRegistrar(const QString &n);
		virtual ~AudioOutputRegistrar();
		virtual AudioOutput *create() = 0;
		virtual const QList<audioDevice> getDeviceChoices() = 0;
		virtual void setDeviceChoice(const QVariant &) = 0;
};

class AudioOutputPlayer : public QObject {
		friend class AudioOutput;
		Q_OBJECT
	public:
		AudioOutputPlayer(const QString name);
		const QString qsName;
		unsigned int iFrameSize;
		short *psBuffer;
		float fPos[3];
		virtual bool decodeNextFrame() = 0;
};

class AudioOutputSpeech : public AudioOutputPlayer {
		friend class AudioOutput;
		Q_OBJECT
	protected:

		SpeexBits sbBits;
		int iFrameCounter;
		QMutex qmJitter;
		JitterBuffer *jbJitter;
		void *dsDecState;

		bool bSpeech;

		int iMissCount;

		unsigned char ucFlags;

		static int speexCallback(SpeexBits *bits, void *state, void *data);
	public:
		int iMissedFrames;
		ClientPlayer *p;

		virtual bool decodeNextFrame();

		void addFrameToBuffer(const QByteArray &, int iBaseSeq);
		AudioOutputSpeech(ClientPlayer * = NULL);
		~AudioOutputSpeech();
};


class AudioSine : public AudioOutputPlayer {
		Q_OBJECT
	protected:
		float v;
		float inc;
		float dinc;
		float volume;
		unsigned int frames;
		unsigned int cntr;
		unsigned int tbin;
		bool bSearch;
		void *fftTable;
	public:
		bool decodeNextFrame();
		AudioSine(float hz, float i, unsigned int frm, float v);
		~AudioSine();
};

class AudioOutput : public QThread {
		Q_OBJECT
	protected:
		volatile bool bRunning;
		int iFrameSize;
		QReadWriteLock qrwlOutputs;
		QMultiHash<const ClientPlayer *, AudioOutputPlayer *> qmOutputs;
		virtual void newPlayer(AudioOutputPlayer *);
		virtual void removeBuffer(AudioOutputPlayer *);
		bool mixAudio(short *output);
	public:
		void wipe();

		AudioOutput();
		~AudioOutput();
		void addFrameToBuffer(ClientPlayer *, const QByteArray &, int iSeq);
		void removeBuffer(const ClientPlayer *);
		void playSine(float hz, float i = 0.0, unsigned int frames = 0xffffff, float volume = 0.3);
		void run() = 0;
};

#else
class AudioInput;
#endif
