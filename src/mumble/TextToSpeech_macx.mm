// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

/* Copyright (C) 2005-2011, Thorvald Natvig <thorvald@natvig.com>
   Copyright (C) 2007, Sebastian Schlingmann <mit_service@users.sourceforge.net>
   Copyright (C) 2008-2011, Mikkel Krautz <mikkel@krautz.dk>
   Copyright (C) 2014, Mayur Pawashe <zorgiepoo@gmail.com>

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

#import <AVFoundation/AVFoundation.h>

#include "TextToSpeech.h"

@interface MUSpeechHelper : NSObject {
	AVSpeechSynthesizer *m_synthesizer;
	float m_volume;
}
- (void)say:(NSString *)message;
- (void)setVolume:(float)volume;
@end

@implementation MUSpeechHelper

- (id)init {
	if ((self = [super init])) {
		m_synthesizer = [[AVSpeechSynthesizer alloc] init];
		m_volume = 1.0f;
	}
	return self;
}

- (void)dealloc {
	[m_synthesizer release];
	[super dealloc];
}

- (void)say:(NSString *)message {
	AVSpeechUtterance *utterance = [AVSpeechUtterance speechUtteranceWithString:message];
	utterance.volume = m_volume;
	[m_synthesizer speakUtterance:utterance];
}

- (void)setVolume:(float)volume {
	m_volume = volume;
}

@end

class TextToSpeechPrivate {
	public:
		MUSpeechHelper *m_helper;

		TextToSpeechPrivate();
		~TextToSpeechPrivate();
		void say(const QString &text);
		void setVolume(int v);
};

TextToSpeechPrivate::TextToSpeechPrivate() {
	m_helper = [[MUSpeechHelper alloc] init];
}

TextToSpeechPrivate::~TextToSpeechPrivate() {
	[m_helper release];
}

void TextToSpeechPrivate::say(const QString &text) {
	QByteArray byteArray = text.toUtf8();
	NSString *message   = [[NSString alloc] initWithBytes:byteArray.constData()
                                                   length:((NSUInteger) byteArray.size())
                                                 encoding:NSUTF8StringEncoding];

	if (message == nil) {
		return;
	}

	[m_helper say:message];
	[message release];
}

void TextToSpeechPrivate::setVolume(int volume) {
	[m_helper setVolume:(float) volume / 100.0f];
}

TextToSpeech::TextToSpeech(QObject *) {
	enabled = true;
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
