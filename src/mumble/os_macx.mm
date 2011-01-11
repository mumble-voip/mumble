/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>
   Copyright (C) 2008-2009, Mikkel Krautz <mikkel@krautz.dk>

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
#ifndef COMPAT_CLIENT
#include "Overlay.h"
#endif
#include "MainWindow.h"

char *os_url = NULL; // fixme(mkrautz): Remove this when mumble11x is gone.
char *os_lang = NULL;
static FILE *fConsole = NULL;

#define PATH_MAX 1024
static char crashhandler_fn[PATH_MAX];

static void crashhandler_signals_restore();
static void crashhandler_handle_crash();

static void mumbleMessageOutput(QtMsgType type, const char *msg) {
	char c;

	switch (type) {
		case QtDebugMsg:
			c='D';
			break;
		case QtWarningMsg:
			c='W';
			break;
		case QtFatalMsg:
			c='F';
			break;
		default:
			c='X';
	}

#define LOG(f, msg) fprintf(f, "<%c>%s %s\n", c, \
		qPrintable(QDateTime::currentDateTime().toString(QLatin1String("yyyy-MM-dd hh:mm:ss.zzz"))), msg)

	LOG(stderr, msg);
	LOG(fConsole, msg);
	fflush(fConsole);

	if (type == QtFatalMsg) {
		CFStringRef csMsg = CFStringCreateWithFormat(kCFAllocatorDefault, NULL, CFSTR("%s\n\nThe error has been logged. Please submit your log file to the Mumble project if the problem persists."), msg);
		CFUserNotificationDisplayAlert(0, 0, NULL,  NULL, NULL, CFSTR("Mumble has encountered a fatal error"), csMsg, CFSTR("OK"), NULL, NULL, NULL);
		CFRelease(csMsg);
		exit(0);
	}
}

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
	for (int i = 0; i < NSIGS; i++) {
		signal(sigs[i], crashhandler_signal_handler);
	}
}

static void crashhandler_signals_restore() {
	for (int i = 0; i < NSIGS; i++) {
		signal(sigs[i], NULL);
	}
}

static void crashhandler_init() {
#ifdef COMPAT_CLIENT
	QString dump = QDir::homePath() + QLatin1String("/Library/Preferences/Mumble/mumble11x.dmp");
#else
	QString dump = g.qdBasePath.filePath(QLatin1String("mumble.dmp"));
#endif
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

	const char *home = getenv("HOME");
#ifdef COMPAT_CLIENT
	const char *logpath = "/Library/Logs/Mumble11x.log";
#else
	const char *logpath = "/Library/Logs/Mumble.log";
#endif

	if (home) {
		size_t len = strlen(home) + strlen(logpath) + 1;
		STACKVAR(char, buff, len);
		memset(buff, 0, len);
		strcat(buff, home);
		strcat(buff, logpath);
		fConsole = fopen(buff, "a+");
		if (fConsole)
			qInstallMsgHandler(mumbleMessageOutput);
	}

	/* Query for language setting. OS X's LANG environment variable is determined from the region selected
	 * in SystemPrefs -> International -> Formats -> Region instead of the system language. We override this
	 * by always using the system langauge, to get rid of all sorts of nasty langauge inconsistencies. */
	query_language();
}
