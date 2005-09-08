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

#include <QMap>
#include <windows.h>
#include <servprov.h>
#include <sapi.h>

#include "TextToSpeech.h"

#undef FAILED
#define FAILED(Status) (static_cast<HRESULT>(Status)<0)

class TextToSpeechPrivate {
	public:
		ISpVoice * pVoice;
		TextToSpeechPrivate();
		~TextToSpeechPrivate();
		void say(QString text);
};

TextToSpeechPrivate::TextToSpeechPrivate() {
	pVoice = NULL;

   	HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&pVoice);
   	if( FAILED(hr))
   		qWarning("TextToSpeechPrivate: Failed to allocate TTS Voice");
}

TextToSpeechPrivate::~TextToSpeechPrivate() {
	if (pVoice)
		pVoice->Release();
}

void TextToSpeechPrivate::say(QString text) {
	if (pVoice)
		pVoice->Speak((const wchar_t *) text.utf16(), SPF_ASYNC, NULL);
}


TextToSpeech::TextToSpeech(QObject *p) : QObject(p) {
	enabled = true;
	d = new TextToSpeechPrivate();
}

TextToSpeech::~TextToSpeech() {
	delete d;
}

void TextToSpeech::say(QString text) {
	if (enabled)
		d->say(text);
}

void TextToSpeech::setEnabled(bool e) {
	enabled = e;
}

bool TextToSpeech::isEnabled() const {
	return enabled;
}
