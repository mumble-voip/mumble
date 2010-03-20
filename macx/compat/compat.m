/* Copyright (C) 2010, Mikkel Krautz <mikkel@krautz.dk>

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

@interface CompatApp : NSObject
@end

@implementation CompatApp

- (void) applicationDidFinishLaunching:(NSNotification *)notification {
	NSAlert *alert = [NSAlert alertWithMessageText:@"Mumble" defaultButton:@"OK" alternateButton:nil otherButton:@"Visit Website"
	                          informativeTextWithFormat:@"This version of Mumble only runs on 64-bit Intel Macs.\n\n"
	                                                     "Please download the Universal version from the Mumble website instead.\n"];
	[[alert window] makeKeyAndOrderFront:self];
	[[alert window] center];
	NSInteger button = [alert runModal];
	if (button == NSAlertOtherReturn) {
		[[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:@"http://mumble.info/"]];
	}

	[NSApp terminate:self];
	[alert release];
}

@end

int main(int argc, char *argv[]) {
	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	CompatApp *app = [[CompatApp alloc] init];	
	[NSApplication sharedApplication];
	[NSApp setDelegate:app];
	[NSApp run];
        [pool release];
	return 0;
}
