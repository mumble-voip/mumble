/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>

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

#include "TextToSpeech.h"
#include "Global.h"
#ifdef USE_SPEECHD
#include <libspeechd.h>
#endif

class TextToSpeechPrivate {
#ifdef USE_SPEECHD
	protected:
		SPDConnection *spd;
#endif
	public:
		TextToSpeechPrivate();
		~TextToSpeechPrivate();
		void say(const QString &text);
		void setVolume(int v);
};

#ifdef USE_SPEECHD
TextToSpeechPrivate::TextToSpeechPrivate() {
	spd = spd_open("Mumble", NULL, NULL, SPD_MODE_THREADED);
	if (! spd) {
		qWarning("TextToSpeech: Failed to contact speech dispatcher.");
	} else {
		if (spd_set_punctuation(spd, SPD_PUNCT_NONE) != 0)
			qWarning("TextToSpech: Failed to set punctuation mode.");
		if (spd_set_spelling(spd, SPD_SPELL_ON) != 0)
			qWarning("TextToSpeech: Failed to set spelling mode.");
	}
}

TextToSpeechPrivate::~TextToSpeechPrivate() {
	if (spd) {
		spd_close(spd);
		spd = NULL;
	}
}

void TextToSpeechPrivate::say(const QString &txt) {
	if (spd)
		spd_say(spd, SPD_MESSAGE, txt.toUtf8());
}

void TextToSpeechPrivate::setVolume(int vol) {
	if (spd)
		spd_set_volume(spd, vol * 2 - 100);
}
#else
TextToSpeechPrivate::TextToSpeechPrivate() {
	qWarning("TextToSpeech: Compiled without support for speech-dispatcher");
}

TextToSpeechPrivate::~TextToSpeechPrivate() {
}

void TextToSpeechPrivate::say(const QString &) {
}

void TextToSpeechPrivate::setVolume(int) {
}
#endif


TextToSpeech::TextToSpeech(QObject *p) : QObject(p) {
	enabled = true;
	d = new TextToSpeechPrivate();
}

TextToSpeech::~TextToSpeech() {
	delete d;
}

void TextToSpeech::say(const QString &text) {
	if (enabled)
		d->say(text);
}

void TextToSpeech::setEnabled(bool e) {
	enabled = e;
}

void TextToSpeech::setVolume(int volume) {
	d->setVolume(volume);
}


bool TextToSpeech::isEnabled() const {
	return enabled;
}
