/*
   Copyright (C) 2009, Mikkel Krautz <mikkel@krautz.dk>

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

#import "MumbleOverlayEnabler.h"

/*
 * This is an input-manager based overlay loader for Mumble on the Mac.
 *
 * Input Managers are sort-of deprecated, but they are one of the most
 * painless ways to implement automatic overlay injection like Mumble
 * currently does on Windows.
 *
 * In Mumble 1.1.8 we did not support automatic injection. Instead we
 * provided a contextual menu plugin for the Finder, allowing people
 * to 'Launch with Mumble Overlay...'.
 *
 * Notes on 64-bit compatibility for this method of injection:
 *
 * Currently, Input Managers are restricted to work only for 32-bit code.
 * 64-bit processes will not load input managers. However, as the Mac gaming
 * market is quite small, and many new games are Cider-ports, I don't think
 * this is going to be an issue in the near future. Maybe it never will.
 *
 * A workable implementation of a 64-bit automatic overlay injector would
 * be use a helper process and mach_inject (as already used for the manual
 * injector commandline utility in overlay_macx/injector.)
 * This helper process would listen for application launches via NSWork-
 * space notifications, and inject the overlay, exactly like the current
 * command-line tool does it.
 */

@implementation MumbleOverlayEnabler

- (id)init {

	/* fixme(mkrautz): Do some permission checks, etc. before loading the bundle. Perhaps validate code signature? */
	NSBundle *bundle = [[NSBundle alloc] initWithPath:@"/Library/MumbleOverlay/Bundles/Overlay.bundle"];

	if (bundle == nil) {
		NSLog(@"MumbleOverlayEnabler: Unable to initialize NSBundle object.");
		return nil;
	}

	if ([bundle load] == NO) {
		NSLog(@"MumbleOverlayEnabler: Unable to load Overlay.bundle.");
		return nil;
	}

	return self;
}

@end
