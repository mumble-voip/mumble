/*
   Copyright (C) 2008, Mikkel Krautz <mikkel@krautz.dk>

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

#include <ApplicationServices/ApplicationServices.h>
#include <CoreServices/CoreServices.h>
#include <CoreFoundation/CoreFoundation.h>
#include <sys/syslimits.h>
#include <mach-o/dyld.h>
#include <dlfcn.h>
#include <stdio.h>
#include <unistd.h>

#include "mach_inject.h"
#include "../stub/stub.h"

#define STUB_BUNDLE     "/System/Library/MumbleOverlay/Bundles/Stub.framework"
#define OVERLAY_BUNDLE  "/System/Library/MumbleOverlay/Bundles/Overlay.framework"

static void
usage(void)
{
	printf("Usage: mumble-overlay-injector [options] <pid>\n");
	printf("\n");
	printf("  -h -?   Show usage information.\n");
	printf("\n");
	exit(0);
}

int
main(int argc, char *argv[])
{
	StubParameters *p = NULL;
	mach_error_t err;
	CFURLRef url;
	void *entry;
	size_t len;
	FSRef fsr;

	int c;
	pid_t injectpid = 0;
	char *appbundle = NULL;

	while ((c = getopt(argc, argv, "h?")) != -1) {
		switch (c) {
			case 'h':
			case '?':
				usage();
				break;
			case 'a':
				appbundle = optarg;
				break;
		}
	}
	argc -= optind;
	argv += optind;
	if (argc) {
		injectpid = atoi(*argv);
	} else
		usage();

	/*
	 * Ready up the strings of our bundles.
	 */
	CFURLRef stubBundle = CFURLCreateWithFileSystemPath(kCFAllocatorDefault, CFSTR(STUB_BUNDLE), kCFURLPOSIXPathStyle, true);
	if (! stubBundle) {
		fprintf(stderr, "CFURLCreateCopyAppendingPathComponent failed for STUB_BUNDLE.\n");
		return 1;
	}

	CFURLRef overlayBundle = CFURLCreateWithFileSystemPath(kCFAllocatorDefault, CFSTR(STUB_BUNDLE), kCFURLPOSIXPathStyle, true);
	if (! overlayBundle) {
		fprintf(stderr, "CFURLCreateCopyAppendingPathComponent failed for OVERLAY_BUNDLE.\n");
		return 1;
	}

	/*
	 * Make sure that they exist in our particular bundle.
	 */
	CFBooleanRef stubExists = CFURLCreatePropertyFromResource(kCFAllocatorDefault, stubBundle, kCFURLFileExists, NULL);
	if (!stubExists || !CFBooleanGetValue(stubExists)) {
		fprintf(stderr, "Unable to locate Stub bundle.\n");
		return 1;
	}

	CFBooleanRef overlayExists = CFURLCreatePropertyFromResource(kCFAllocatorDefault, overlayBundle, kCFURLFileExists, NULL);
	if (!overlayExists || !CFBooleanGetValue(overlayExists)) {
		fprintf(stderr, "Unable to locate Overlay bundle.\n");
		return 1;
	}

	/*
	 * Load in stub bundle.
 	 */
	CFBundleRef bundle = CFBundleCreate(kCFAllocatorDefault, stubBundle);
	if (! bundle) {
		fprintf(stderr, "Unable to CFBundleCreate() for stub bundle.\n");
		return 1;
	}

	entry = CFBundleGetFunctionPointerForName(bundle, CFSTR(INJECT_ENTRY_SYMBOL));
	if (! entry) {
		fprintf(stderr, "Unable to find symbol '%s' in bundle.\n", INJECT_ENTRY_SYMBOL);
		return 1;
	}

	/*
	 * Set up stub parameters.
	 */
	CFBundleRef overlay = CFBundleCreate(kCFAllocatorDefault, overlayBundle);
	if (! overlay) {
		fprintf(stderr, "Unable to CFBundleCreate() for overlay bundle.\n");
		return 1;
	}

	CFURLRef overlayUrl = CFBundleCopyExecutableURL(overlay);
	if (! overlayUrl) {
		fprintf(stderr, "Unable to fetch bundle executable URL\n");
		return 1;
	}
	CFURLRef overlayAbsolute = CFURLCopyAbsoluteURL(overlayUrl);
	CFStringRef overlayStr = CFURLCopyFileSystemPath(overlayAbsolute, kCFURLPOSIXPathStyle);
	CFIndex overlayLen = CFStringGetMaximumSizeForEncoding(CFStringGetLength(overlayStr), kCFStringEncodingUTF8);

	len = sizeof(ptrdiff_t) + overlayLen + 1;
	p = calloc(len, 1);
	CFStringGetCString(overlayStr, (char *) p + sizeof(ptrdiff_t), overlayLen, kCFStringEncodingUTF8);

	err = mach_inject(entry, p, len, injectpid, 0);
	if (err != 0) {
		fprintf(stderr, "Error. Injection failed. (err=%i)\n", err);
		return 1;
	}
}
