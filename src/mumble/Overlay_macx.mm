/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>
   Copyright (C) 2010, Mikkel Krautz <mikkel@krautz.dk>

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

#import <ScriptingBridge/ScriptingBridge.h>
#include "Overlay.h"

@interface OverlayInjectorMac : NSObject
 - (void) appLaunched:(NSNotification *)notification;
 - (void) setActive:(BOOL)flag;
@end

@implementation OverlayInjectorMac
- (void) appLaunched:(NSNotification *)notification {
	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	NSDictionary *userInfo = [notification userInfo];
	
	pid_t pid = [[userInfo objectForKey:@"NSApplicationProcessIdentifier"] intValue];
	SBApplication *app = [SBApplication applicationWithProcessIdentifier:pid];

	[app setSendMode:kAENoReply];
	[app sendEvent:kASAppleScriptSuite id:kGetAEUT parameters:0];
	[app sendEvent:'MUOL' id:'load' parameters:0];
	[pool release];
}

- (void) setActive:(BOOL)flag {
	NSWorkspace *workspace = [NSWorkspace sharedWorkspace];

	if (flag == YES) {
		[[workspace notificationCenter] addObserver:self
		                                selector:@selector(appLaunched:)
		                                name:NSWorkspaceDidLaunchApplicationNotification
		                                object:workspace];
	} else {
		[[workspace notificationCenter] removeObserver:self
		                                name:NSWorkspaceDidLaunchApplicationNotification
		                                object:workspace];
	}
}
@end

class OverlayPrivateMac : public OverlayPrivate {
	protected:
		OverlayInjectorMac *olm;
	public:
		void setActive(bool);
		OverlayPrivateMac(QObject *);
		~OverlayPrivateMac();
};

OverlayPrivateMac::OverlayPrivateMac(QObject *p) : OverlayPrivate(p) {
	olm = [[OverlayInjectorMac alloc] init];
}

OverlayPrivateMac::~OverlayPrivateMac() {
	[olm release];
}

void OverlayPrivateMac::setActive(bool act) {
	[olm setActive:act];
}

void Overlay::platformInit() {
	d = new OverlayPrivateMac(this);
}

void Overlay::setActive(bool act) {
	static_cast<OverlayPrivateMac *>(d)->setActive(act);
}
