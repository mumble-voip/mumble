/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>
   Copyright (C) 2007, Sebastian Schlingmann <mit_service@users.sourceforge.net>
   Copyright (C) 2008-2009, Mikkel Krautz <mikkel@krautz.dk>

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

#include "Global.h"
#include "TextToSpeech.h"

class TextToSpeechPrivate {
	public:
		SpeechChannel scChannel;
		QMutex qmLock;
		Fixed fVolume;
		QList<QByteArray> qlMessages;
		bool bRunning;

		TextToSpeechPrivate();
		void ProcessSpeech();
		void say(const QString &text);
		void setVolume(int v);
};

static void speech_done_cb(SpeechChannel scChannel, void *udata) {
	TextToSpeechPrivate *tts = reinterpret_cast<TextToSpeechPrivate *>(udata);

	Q_ASSERT(scChannel == tts->scChannel);

	DisposeSpeechChannel(tts->scChannel);

	if (tts->qlMessages.isEmpty())
		tts->bRunning = false;
	else
		tts->ProcessSpeech();
}

TextToSpeechPrivate::TextToSpeechPrivate() {
	bRunning = false;
}

void TextToSpeechPrivate::ProcessSpeech() {
	QByteArray ba;

	qmLock.lock();
	ba = qlMessages.takeFirst();
	qmLock.unlock();

	NewSpeechChannel(NULL, &scChannel);
	SetSpeechInfo(scChannel, soVolume, &fVolume);
	SetSpeechInfo(scChannel, soRefCon, this);
	SetSpeechInfo(scChannel, soSpeechDoneCallBack, reinterpret_cast<void *>(speech_done_cb));
	SpeakText(scChannel, ba.constData(), ba.size());
}

void TextToSpeechPrivate::say(const QString &text) {
	QTextCodec *codec = QTextCodec::codecForName("Apple Roman");
	Q_ASSERT(codec != NULL);

	qmLock.lock();
	qlMessages.append(codec->fromUnicode(text));
	qmLock.unlock();

	if (!bRunning) {
		ProcessSpeech();
		bRunning = true;
	}
}

void TextToSpeechPrivate::setVolume(int volume) {
	fVolume = FixRatio(volume, 100);
}

TextToSpeech::TextToSpeech(QObject *) {
	enabled = true;
	d = NULL;

	/* Determine which release of OS X we're running on. Tiger has a buggy implementation, and
	 * therefore we'll just disable ourselves when we're running on that.
	 *
	 * What it comes down to, is that calling DisposeSpeechChannel() on Tiger will crash in certain
	 * situations.
	 *
	 * For more information, see this thread on Apple's speech mailing list:
	 * http://lists.apple.com/archives/speech-dev/2005/Aug/msg00000.html
	 */

	int version = qMacVersion();
	if (version != QSysInfo::MV_Unknown && version < QSysInfo::MV_LEOPARD) {
		qWarning("Mac OS X 10.4 (Tiger) detected. Disabling Text-to-Speech because of a buggy implementation in 10.4.");
		return;
	}

	d = new TextToSpeechPrivate();
}

TextToSpeech::~TextToSpeech() {
	delete d;
}

void TextToSpeech::say(const QString &text) {
	if (d && enabled)
		d->say(text);
}

void TextToSpeech::setEnabled(bool e) {
	enabled = e;
}

void TextToSpeech::setVolume(int volume) {
	if (d && enabled)
		d->setVolume(volume);
}

bool TextToSpeech::isEnabled() const {
	return enabled;
}
