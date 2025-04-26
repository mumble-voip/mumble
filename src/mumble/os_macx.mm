// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "MainWindow.h"
#ifdef USE_OVERLAY
#	include "Overlay.h"
#endif
#include "Utils.h"
#include "Global.h"

#include <CoreFoundation/CoreFoundation.h>

char *os_lang = nullptr;

#define PATH_MAX 1024
static char crashhandler_fn[PATH_MAX];

static void crashhandler_signals_restore();
static void crashhandler_handle_crash();

void query_language() {
	CFPropertyListRef cfaLangs;
	CFStringRef cfsLang;
	static char lang[16];

	cfaLangs = CFPreferencesCopyAppValue(CFSTR("AppleLanguages"), kCFPreferencesCurrentApplication);
	cfsLang = (CFStringRef) CFArrayGetValueAtIndex((CFArrayRef)cfaLangs, 0);

	if (! CFStringGetCString(cfsLang, lang, 16, kCFStringEncodingUTF8))
		return;

	os_lang = lang;
}

static void crashhandler_signal_handler(int signal) {
	switch (signal) {
		case SIGQUIT:
		case SIGILL:
		case SIGTRAP:
		case SIGABRT:
		case SIGEMT:
		case SIGFPE:
		case SIGBUS:
		case SIGSEGV:
		case SIGSYS:
			crashhandler_signals_restore();
			crashhandler_handle_crash();
			break;
		default:
			break;
	}
}

/* These are the signals that according to signal(3) produce a coredump by default. */
int sigs[] = { SIGQUIT, SIGILL, SIGTRAP, SIGABRT, SIGEMT, SIGFPE, SIGBUS, SIGSEGV, SIGSYS };
#define NSIGS sizeof(sigs)/sizeof(sigs[0])

static void crashhandler_signals_setup() {
	for (size_t i = 0; i < NSIGS; i++) {
		signal(sigs[i], crashhandler_signal_handler);
	}
}

static void crashhandler_signals_restore() {
	for (size_t i = 0; i < NSIGS; i++) {
		signal(sigs[i], nullptr);
	}
}

static void crashhandler_init() {
	QString dump = Global::get().qdBasePath.filePath(QLatin1String("mumble.dmp"));
	if (strncpy(crashhandler_fn, dump.toUtf8().data(), PATH_MAX)) {
		crashhandler_signals_setup();
		/* todo: Change the behavior of the Apple crash dialog? Maybe? */
	}
}

static void crashhandler_handle_crash() {
	/* Abuse mtime for figuring out which crashdump we should send. */
	FILE *f = fopen(crashhandler_fn, "w");
	fflush(f);
	fclose(f);
}

void os_init() {
	crashhandler_init();

	/* Query for language setting. OS X's LANG environment variable is determined from the region selected
	 * in SystemPrefs -> International -> Formats -> Region instead of the system language. We override this
	 * by always using the system language, to get rid of all sorts of nasty language inconsistencies. */
	query_language();
}

#undef PATH_MAX
#undef NSIGS
