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

/*
 * Mumble overlay app-launcher for OS X.
 */

#include <unistd.h>
#include <ApplicationServices/ApplicationServices.h>

#import <Cocoa/Cocoa.h>

static void usage(void) {
	printf("Usage: mumble-overlay [opts] <app> [app-args ...]\n");
	printf("\n");
	printf(" Launch an application (given by the `app' argument) with\n");
	printf(" the Mumble overlay enabled.\n");
	printf("\n");
	printf(" -h -?  Show usage information.\n");
	printf(" -d     Run overlay in debug mode.\n");
	printf(" -v     Show version information.\n");
	printf("\n");
	exit(0);
}

static void version(void) {
	printf("%s\n", VERSION);
	exit(0);
}

int main(int argc, char *argv[]) {
	NSAutoreleasePool *pool;
	char *appPath = NULL;
	int c, i, debug = 0;

	pool = [[NSAutoreleasePool alloc] init];

	while ((c = getopt(argc, argv, "h?vd")) != -1) {
		switch (c) {
			case 'h':
			case '?':
				usage();
				break;
			case 'v':
				version();
				break;
			case 'd':
				debug = 1;
				break;
		}
	}
	argc -= optind;
	argv += optind;

	if (argc) {
		appPath = argv[0];
		--argc;
		++argv;
	} else
		usage();

	FSRef ref;
	OSStatus err;

	/* Get FSRef of our app path. */
	err = FSPathMakeRef((const UInt8 *) appPath, &ref, NULL);
	if (err != noErr) {
		fprintf(stderr, "mumble-overlay: Unable to get reference to application path. Make sure you specify a valid path.\n");
		return -1;
	}

	/* Get a hold of our Mumble process, to find our overlay library. */
	NSArray *apps = [[NSWorkspace sharedWorkspace] launchedApplications];
	NSString *mumblePath = nil;

	for (NSDictionary *dict in apps) {
		if ([[dict objectForKey:@"NSApplicationBundleIdentifier"] isEqualToString:@"net.sourceforge.mumble"]) {
			mumblePath = [dict objectForKey:@"NSApplicationPath"];
		}
	}

	if (! mumblePath) {
		fprintf(stderr, "mumble-overlay: Cannot find open Mumble instance. Bailing.\n");
		return -1;
	}

	NSBundle *appBundle = [NSBundle bundleWithPath: mumblePath];
	if (! appBundle) {
		fprintf(stderr, "mumble-overlay: Cannot open Mumble app bundle.\n");
		return -1;
	}

	NSString *overlayInBundle = [appBundle objectForInfoDictionaryKey:@"MumbleOverlayLibrary"];
	if (! overlayInBundle) {
		fprintf(stderr, "mumble-overlay: No key 'MumbleOverlayLibrary' specified in Mumble's property list. Cannot find overlay library.\n");
		return -1;
	}

	NSString *overlayPath = [NSString stringWithFormat:@"%@/%@", mumblePath, overlayInBundle];

	if (! [[NSFileManager defaultManager] fileExistsAtPath:overlayPath]) {
		fprintf(stderr, "mumble-overlay: Overlay library non-existant at MumbleOverlayLibrary-specified path.\n");
		return -1;
	}

	/* Create environment dict. */
	NSArray *keys = [NSArray arrayWithObjects: @"DYLD_INSERT_LIBRARIES", @"MUMBLE_OVERLAY_DEBUG", nil];
	NSArray *vals = [NSArray arrayWithObjects: overlayPath, @"1", nil];
	NSDictionary *environment = [NSDictionary dictionaryWithObjects:vals forKeys:keys];

	if (! environment) {
		/* We were unable create an environment dict, and thus we are unable to inject the overlay into the process. */
		fprintf(stderr, "mumble-overlay: Unable to create environment dictionary. Will launch process without overlay.\n"); 
	}

	/* Create argv array. */
	NSMutableArray *arguments = [NSMutableArray arrayWithCapacity:argc];
	for (i = 0; i < argc; i++) {
		[arguments insertObject: [NSString stringWithCString:argv[i] encoding:NSUTF8StringEncoding] atIndex:0];
	}

	LSApplicationParameters parm = {
		.version             = 0,
		.flags               = kLSLaunchDefaults,
		.application         = &ref,
		.asyncLaunchRefCon   = NULL,
		.environment         = environment,
		.argv                = arguments,
		.initialEvent        = NULL
	};

	err = LSOpenApplication(&parm, NULL);

	if (err != noErr) {
		fprintf(stderr, "mumble-overlay: LSOpenApplication() failed: %i\n", (int)err);
		return -1;
	}

	return 0;
}
