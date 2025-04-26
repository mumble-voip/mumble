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

#import <AppKit/AppKit.h>

#include "TextToSpeech.h"

@interface MUSpeechSynthesizerPrivateHelper : NSObject {
	NSMutableArray *m_messages;
	NSSpeechSynthesizer *m_synthesizer;
}
- (NSSpeechSynthesizer *)synthesizer;
- (void)appendMessage:(NSString *)message;
- (void)processSpeech;
@end

#if !defined(USE_MAC_UNIVERSAL) && MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_6
@interface MUSpeechSynthesizerPrivateHelper () <NSSpeechSynthesizerDelegate>
@end
#endif

@implementation MUSpeechSynthesizerPrivateHelper

- (id)init {
	if ((self = [super init])) {
		m_synthesizer = [[NSSpeechSynthesizer alloc] initWithVoice:nil];
		m_messages = [[NSMutableArray alloc] init];
		[m_synthesizer setDelegate:self];
	}
	return self;
}

- (void)dealloc {
	[m_synthesizer release];
	[m_messages release];
	[super dealloc];
}

- (NSSpeechSynthesizer *)synthesizer {
	return m_synthesizer;
}

- (void)appendMessage:(NSString *)message {
	[m_messages insertObject:message atIndex:0];
}

- (void)processSpeech {
	Q_ASSERT([m_messages count] == 0);
	
	NSString *poppedMessage = [m_messages lastObject];
	[m_synthesizer startSpeakingString:poppedMessage];
	[m_messages removeLastObject];
}

- (void)speechSynthesizer:(NSSpeechSynthesizer *)synthesizer didFinishSpeaking:(BOOL)success {
	Q_UNUSED(synthesizer);
	Q_UNUSED(success);

	if ([m_messages count] != 0) {
		[self processSpeech];
	}
}

@end

class TextToSpeechPrivate {
	public:
		MUSpeechSynthesizerPrivateHelper *m_synthesizerHelper;

		TextToSpeechPrivate();
		~TextToSpeechPrivate();
		void say(const QString &text);
		void setVolume(int v);
};

TextToSpeechPrivate::TextToSpeechPrivate() {
	m_synthesizerHelper = [[MUSpeechSynthesizerPrivateHelper alloc] init];
}

TextToSpeechPrivate::~TextToSpeechPrivate() {
	[m_synthesizerHelper release];
}

void TextToSpeechPrivate::say(const QString &text) {
	QByteArray byteArray = text.toUtf8();
	NSString *message = [[NSString alloc] initWithBytes:byteArray.constData() length:((NSUInteger) byteArray.size()) encoding:NSUTF8StringEncoding];

	if (message == nil) {
		return;
	}

	[m_synthesizerHelper appendMessage:message];
	[message release];

	if (![[m_synthesizerHelper synthesizer] isSpeaking]) {
		[m_synthesizerHelper processSpeech];
	}
}

void TextToSpeechPrivate::setVolume(int volume) {
	// Check for setVolume: availability. It's only available on 10.5+.
	if ([[m_synthesizerHelper synthesizer] respondsToSelector:@selector(setVolume:)]) {
		[[m_synthesizerHelper synthesizer] setVolume:volume / 100.0f];
	}
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
