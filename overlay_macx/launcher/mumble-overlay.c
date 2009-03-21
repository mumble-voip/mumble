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

static void usage(void) {
	printf("Usage: mumble-overlay [opts] <app>\n");
	printf("\n");
	printf(" Launch an application (given by the `app' argument) with\n");
	printf(" the Mumble overlay enabled.\n");
	printf("\n");
	printf(" -h -?  Show usage information.\n");
	printf(" -v     Show version information.\n");
	printf("\n");
	exit(0);
}

static void version(void) {
	printf("%s\n", VERSION);
	exit(0);
}

int main(int argc, char *argv[]) {
	char *appPath = NULL;
	int c;

	while ((c = getopt(argc, argv, "h?v")) != -1) {
		switch (c) {
			case 'h':
			case '?':
				usage();
				break;
			case 'v':
				version();
				break;
		}
	}
	argc -= optind;
	argv += optind;
	if (argc) {
		appPath = *argv;
	} else
		usage();

	FSRef ref;
	OSStatus err;

	err = FSPathMakeRef((const UInt8 *) appPath, &ref, NULL);
	if (err != noErr)
		return -1;

	LSApplicationParameters parm = {
		.version             = 0,
		.flags               = kLSLaunchDefaults,
		.application         = &ref,
		.asyncLaunchRefCon   = NULL,
		.environment         = NULL,
		.argv                = NULL,
		.initialEvent        = NULL
	};
	ProcessSerialNumber psn;

	err = LSOpenApplication(&parm, &psn);
	if (err != noErr)
		return -1;

	pid_t pid;
	err = GetProcessPID(&psn, &pid);
	if (err != noErr)
		return -1;

	sleep(1);

	char *envPath = getenv("PATH");
	if (envPath == NULL)
		setenv("PATH", MUMBLE_OVERLAY_MACX_PATH, 1);
	else {
		char buf[4096] = { 0 };
		snprintf(buf, 4096, "%s:%s", MUMBLE_OVERLAY_MACX_PATH, envPath);
		setenv("PATH", buf, 1);
	}

	char pidStr[25];
	snprintf(pidStr, 25, "%lu", (unsigned long) pid);

	char *appName = "mumble-overlay-injector";
	char *args[] = { appName, pidStr, NULL };
	if (execvp(appName, args) == -1)
		return -1;

	return 0;
}
