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

#ifndef _AUDIOINPUT_H
#define _AUDIOINPUT_H

#include <QThread>
#include <QGroupBox>
#include <speex/speex.h>
#include <speex/speex_preprocess.h>
#include <speex/speex_echo.h>

#include "Audio.h"

class AudioInput;

typedef AudioInput *(*AudioInputRegistrarNew)();

class AudioInputRegistrar {
	public:
		static QMap<QString, AudioInputRegistrarNew> *qmNew;
		static QMap<QString, AudioRegistrarConfig> *qmConfig;

		static QString current;
		AudioInputRegistrar(QString name, AudioInputRegistrarNew n, AudioRegistrarConfig c);
		static AudioInput *newFromChoice(QString choice = QString());
};

class AudioInput : public QThread {
	Q_OBJECT
	protected:
		int	iFrameSize;
		int iByteSize;

		SpeexBits sbBits;
		SpeexPreprocessState *sppPreprocess;
		SpeexEchoState *sesEcho;
		void *esEncState;

		short *psMic;
		short *psSpeaker;
		short *psClean;
		float *pfY;
		void encodeAudioFrame();

		bool bRunning;
		bool bPreviousVoice;
		bool bHasSpeaker;

		static int c_iFrameCounter;
	public:
		bool bResetProcessor;

		int iBitrate;
		double dSnr;
		double dLoudness;
		double dPeakMic, dPeakSpeaker;
		double dSpeechProb;

		AudioInput();
		~AudioInput();
		void run() = 0;
};

#else
class AudioInput;
#endif
