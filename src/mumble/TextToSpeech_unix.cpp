// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "TextToSpeech.h"

#ifdef USE_SPEECHD
# ifdef USE_SPEECHD_PKGCONFIG
#  include <speech-dispatcher/libspeechd.h>
# else
#  include <libspeechd.h>
# endif
#endif

#include <QtCore/QLocale>

// We define a global macro called 'g'. This can lead to issues when included code uses 'g' as a type or parameter name (like protobuf 3.7 does). As such, for now, we have to make this our last include.
#include "Global.h"

class TextToSpeechPrivate {
#ifdef USE_SPEECHD
	protected:
		SPDConnection *spd;
		/// Used to store the requested volume of the TextToSpeech object
		/// before speech-dispatcher has been initialized.
		int volume;
		bool initialized;
		void ensureInitialized();
#endif
	public:
		TextToSpeechPrivate();
		~TextToSpeechPrivate();
		void say(const QString &text);
		void setVolume(int v);
};

#ifdef USE_SPEECHD
TextToSpeechPrivate::TextToSpeechPrivate() {
	initialized = false;
	volume = -1;
	spd = NULL;
}

TextToSpeechPrivate::~TextToSpeechPrivate() {
	if (spd) {
		spd_close(spd);
		spd = NULL;
	}
}

void TextToSpeechPrivate::ensureInitialized() {
	if (initialized) {
		return;
	}

	spd = spd_open("Mumble", NULL, NULL, SPD_MODE_THREADED);
	if (! spd) {
		qWarning("TextToSpeech: Failed to contact speech dispatcher.");
	} else {
		QString lang;
		if (!g.s.qsTTSLanguage.isEmpty()) {
			lang = g.s.qsTTSLanguage;
		} else if (!g.s.qsLanguage.isEmpty()) {
			QLocale locale(g.s.qsLanguage);
			lang = locale.bcp47Name();
		} else {
			QLocale systemLocale;
			lang = systemLocale.bcp47Name();
		}
		if (!lang.isEmpty()) {
			if (spd_set_language(spd, lang.toLocal8Bit().constData()) != 0) {
				qWarning("TextToSpeech: Failed to set language.");
			}
		}

		if (spd_set_punctuation(spd, SPD_PUNCT_NONE) != 0)
			qWarning("TextToSpech: Failed to set punctuation mode.");
		if (spd_set_spelling(spd, SPD_SPELL_ON) != 0)
			qWarning("TextToSpeech: Failed to set spelling mode.");
	}

	initialized = true;

	if (volume != -1) {
		setVolume(volume);
	}
}

void TextToSpeechPrivate::say(const QString &txt) {
	ensureInitialized();

	if (spd)
		spd_say(spd, SPD_MESSAGE, txt.toUtf8());
}

void TextToSpeechPrivate::setVolume(int vol) {
	if (!initialized) {
		volume = vol;
		return;
	}

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
