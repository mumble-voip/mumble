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

#ifndef _AUDIOOUTPUT_H
#define _AUDIOOUTPUT_H

#define SAMPLE_RATE 16000

#include <QThread>
#include <QMutex>
#include <QMap>
#include <speex/speex.h>
#include <speex/speex_jitter.h>

class AudioOutput;

class AudioOutputPlayer : public QObject {
	Q_OBJECT
	protected:
		short m_sId;

		SpeexBits m_sbBits;
		int	m_iFrameSize;
		int m_iByteSize;
		int m_iFrameCounter;
		QMutex m_qmJitter;
		SpeexJitter m_sjJitter;
		void *m_dsDecState;
		AudioOutput *m_aoOutput;

		short *m_psBuffer;

		void decodeNextFrame();
	public:
		void addFrameToBuffer(QByteArray &, int iSeq);
		AudioOutputPlayer(AudioOutput *, short);
		~AudioOutputPlayer();
};

class AudioOutput : public QObject {
	Q_OBJECT
	protected:
		bool m_bRunning;
		QMutex m_qmOutputMutex;
		QMap<short, AudioOutputPlayer *> m_qmOutputs;
		virtual AudioOutputPlayer *getPlayer(short) = 0;
	public:
		void wipe();

		AudioOutput();
		~AudioOutput();
		void addFrameToBuffer(short, QByteArray &, int iSeq);
		void removeBuffer(short);
		static QWidget *configWidget(QWidget *parent = NULL);
};

extern AudioOutput *g_aoOutput;

#else
class AudioInput;
#endif
