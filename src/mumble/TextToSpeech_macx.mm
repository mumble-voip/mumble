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

#include "mumble_pch.hpp"

#include "Global.h"
#include "TextToSpeech.h"

@interface MUSpeechSynthesizerPrivateHelper : NSObject <NSSpeechSynthesizerDelegate>
{
	NSSpeechSynthesizer *m_synthesizer;
	QList<QByteArray> m_messages;
}

 @property (nonatomic, readonly) NSSpeechSynthesizer *synthesizer;

- (void)appendMessage:(QByteArray)message;

- (void)processSpeech;
- (void)speechSynthesizer:(NSSpeechSynthesizer *)sender didFinishSpeaking:(BOOL)success;

@end

@implementation MUSpeechSynthesizerPrivateHelper

@synthesize synthesizer = m_synthesizer;

- (id)init {
	self = [super init];
	if (self != nil) {
		m_synthesizer = [[NSSpeechSynthesizer alloc] initWithVoice:nil];
		m_synthesizer.delegate = self;
	}
	return self;
}

- (void)dealloc {
	[m_synthesizer release];
	[super dealloc];
}

- (void)appendMessage:(QByteArray)message {
	m_messages.append(message);
}

- (void)processSpeech {
	Q_ASSERT(!m_messages.isEmpty());
	
	QByteArray firstMessage = m_messages.takeFirst();

	// Use NSMacOSRomanStringEncoding because "Apple Roman" is used in TextToSpeechPrivate::say
	NSString *text = [[NSString alloc] initWithBytes:firstMessage.constData() length:firstMessage.size() encoding:NSMacOSRomanStringEncoding];
	Q_ASSERT(text != nil);
	if (text != nil) {
		[m_synthesizer startSpeakingString:text];
	}
	[text release];
}

- (void)speechSynthesizer:(NSSpeechSynthesizer *)synthesizer didFinishSpeaking:(BOOL)success {
	if (!m_messages.isEmpty()) {
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
	QTextCodec *codec = QTextCodec::codecForName("Apple Roman");
	if (codec == NULL) {
		qWarning("CODEC WAS NULL");
	}
	Q_ASSERT(codec != NULL);

	if (codec != NULL) {
		[m_synthesizerHelper appendMessage:codec->fromUnicode(text)];

		if (![m_synthesizerHelper.synthesizer isSpeaking]) {
			[m_synthesizerHelper processSpeech];
		}
	}
}

void TextToSpeechPrivate::setVolume(int volume) {
	[m_synthesizerHelper.synthesizer setVolume:volume / 100.0];
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
