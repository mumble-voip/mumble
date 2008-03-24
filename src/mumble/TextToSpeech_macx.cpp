/* Copyright (C) 2005-2008, Thorvald Natvig <thorvald@natvig.com>
   Copyright (C) 2007, Sebastian Schlingmann <mit_service@users.sourceforge.net>

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

/* need the undef or else the include would not work
 it would help to include the header before QT, but that might mess with QT */
#undef qDebug
#include "ApplicationServices/ApplicationServices.h"

#include "Global.h"
#include "TextToSpeech.h"

static QMutex qmLock;
static Fixed fVolume;
static QList<QByteArray> qlMessages;
static bool bRunning;
static bool bEnabled;

static void process_speech();

static void speech_done_cb(SpeechChannel scChannel, long, void **, unsigned long *, long *) {
	DisposeSpeechChannel(scChannel);

	if (qlMessages.isEmpty())
		bRunning = false;
	else
		process_speech();
}

static void process_speech() {
	SpeechChannel scChannel;
	QByteArray ba;

	qmLock.lock();
	ba = qlMessages.takeFirst();
	qmLock.unlock();

	NewSpeechChannel(NULL, &scChannel);
	SetSpeechInfo(scChannel, soVolume, &fVolume);
	SetSpeechInfo(scChannel, soSpeechDoneCallBack, reinterpret_cast<void *>(speech_done_cb));
	SpeakText(scChannel, ba.constData(), ba.size());
}

TextToSpeech::TextToSpeech(QObject *) {
	bEnabled = true;
}

TextToSpeech::~TextToSpeech() {
}

void TextToSpeech::say(const QString &text) {
	if (!bEnabled)
		return;

	QTextCodec *codec = QTextCodec::codecForName("Apple Roman");
	Q_ASSERT(codec != NULL);

	qmLock.lock();
	qlMessages.append(codec->fromUnicode(text));
	qmLock.unlock();

	if (!bRunning) {
		process_speech();
		bRunning = true;
	}
}

void TextToSpeech::setEnabled(bool e) {
	bEnabled = e;
}

void TextToSpeech::setVolume(int volume) {
	fVolume = FixRatio(volume, 100);
}

bool TextToSpeech::isEnabled() const {
	return bEnabled;
}
