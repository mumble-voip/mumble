// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#import <Cocoa/Cocoa.h>
#include <dlfcn.h>

__attribute__ ((visibility("default")))
OSErr MumbleOverlayEventHandler(const AppleEvent *ae, AppleEvent *reply, long refcon) {
	(void) ae;
	(void) reply;
	(void) refcon;

	/* Is the overlay already loaded into the process? */
	if (dlsym(RTLD_DEFAULT, "MumbleOverlayEntryPoint")) {
		fprintf(stderr, "MumbleOverlayLoader: Overlay already loaded.\n");
		return noErr;
	}

	/*
	 * Load the overlay lib - hard coded because we're the only consumer, and because we
	 * can only live in /Library/ScriptingAdditions/
	 */
	dlopen("/Library/ScriptingAdditions/MumbleOverlay.osax/Contents/MacOS/libmumbleoverlay.dylib", RTLD_LAZY);

	return noErr;
}
