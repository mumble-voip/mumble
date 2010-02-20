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

#import <Cocoa/Cocoa.h>
#include <dlfcn.h>

__attribute__ ((visibility("default")))
OSErr MumbleOverlayEventHandler(const AppleEvent *ae, AppleEvent *reply, long refcon) {

	/* Is the overlay already loaded into the process? */
	if (dlsym(RTLD_DEFAULT, "MumbleOverlayEntryPoint")) {
		fprintf(stderr, "MumbleOverlayLoader: Overlay already loaded.\n");
		return;
	}

	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

	/* Get a hold of our Mumble process, to find our overlay library. */
	NSArray *apps = [[NSWorkspace sharedWorkspace] launchedApplications];
	NSString *mumblePath = nil;

	for (NSDictionary *dict in apps) {
		if ([[dict objectForKey:@"NSApplicationBundleIdentifier"] isEqualToString:@"net.sourceforge.mumble.Mumble"]) {
			mumblePath = [dict objectForKey:@"NSApplicationPath"];
			break;
		}
		if ([[dict objectForKey:@"NSApplicationBundleIdentifier"] isEqualToString:@"net.sourceforge.mumble.Mumble11x"]) {
			mumblePath = [dict objectForKey:@"NSApplicationPath"];
			break;
		}
	}

	if (! mumblePath) {
		fprintf(stderr, "MumbleOverlayLoader: Cannot find open Mumble instance. Bailing.\n");
		goto out;
	}

	NSBundle *appBundle = [NSBundle bundleWithPath: mumblePath];
	if (! appBundle) {
		fprintf(stderr, "MumbleOverlayLoader: Cannot open Mumble app bundle.\n");
		goto out;
	}

	NSString *overlayInBundle = [appBundle objectForInfoDictionaryKey:@"MumbleOverlayLibrary"];
	if (! overlayInBundle) {
		fprintf(stderr, "MumbleOverlayLoader: No key 'MumbleOverlayLibrary' specified in Mumble's property list. Cannot find overlay library.\n");
		goto out;
	}

	NSString *overlayPath = [NSString stringWithFormat:@"%@/%@", mumblePath, overlayInBundle];

	if (! [[NSFileManager defaultManager] fileExistsAtPath:overlayPath]) {
		fprintf(stderr, "MumbleOverlayLoader: Overlay library non-existant at MumbleOverlayLibrary-specified path.\n");
		goto out;
	}

	/* Load the overlay library. */
	dlopen([overlayPath UTF8String ], RTLD_LAZY);

out:
	[pool release];
	return noErr;
}
