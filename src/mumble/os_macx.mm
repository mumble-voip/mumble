// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "LogEmitter.h"
#include "MainWindow.h"
#include "Overlay.h"
#include "Utils.h"

#include <CoreFoundation/CoreFoundation.h>

// We define a global macro called 'g'. This can lead to issues when included code uses 'g' as a type or parameter name (like protobuf 3.7 does). As such, for now, we have to make this our last include.
#include "Global.h"

char *os_lang = NULL;
static FILE *fConsole = NULL;

static QSharedPointer<LogEmitter> le;

#define PATH_MAX 1024
static char crashhandler_fn[PATH_MAX];

static void crashhandler_signals_restore();
static void crashhandler_handle_crash();

static void mumbleMessageOutputQString(QtMsgType type, const QString &msg) {
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
		qPrintable(QDateTime::currentDateTime().toString(QLatin1String("yyyy-MM-dd hh:mm:ss.zzz"))), qPrintable(msg))

	QString date = QDateTime::currentDateTime().toString(QLatin1String("yyyy-MM-dd hh:mm:ss.zzz"));
	QString fmsg = QString::fromLatin1("<%1>%2 %3").arg(c).arg(date).arg(msg);
	fprintf(stderr, "%s\n", qPrintable(fmsg));
	fprintf(fConsole, "%s\n", qPrintable(fmsg));
	fflush(fConsole);

	le->addLogEntry(fmsg);

	if (type == QtFatalMsg) {
		CFStringRef csMsg = CFStringCreateWithFormat(kCFAllocatorDefault, NULL, CFSTR("%s\n\nThe error has been logged. Please submit your log file to the Mumble project if the problem persists."), qPrintable(msg));
		CFUserNotificationDisplayAlert(0, 0, NULL,  NULL, NULL, CFSTR("Mumble has encountered a fatal error"), csMsg, CFSTR("OK"), NULL, NULL, NULL);
		CFRelease(csMsg);
		exit(0);
	}
}

#if QT_VERSION < 0x050000
static void mumbleMessageOutput(QtMsgType type, const char *msg) {
	mumbleMessageOutputQString(type, QString::fromUtf8(msg));
}
#elif QT_VERSION >= 0x050000
static void mumbleMessageOutputWithContext(QtMsgType type, const QMessageLogContext &ctx, const QString &msg) {
	Q_UNUSED(ctx);
	mumbleMessageOutputQString(type, msg);
}
#endif

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
		signal(sigs[i], NULL);
	}
}

static void crashhandler_init() {
	QString dump = g.qdBasePath.filePath(QLatin1String("mumble.dmp"));
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
	const char *logpath = "/Library/Logs/Mumble.log";

	// Make a copy of the global LogEmitter, such that
	// os_macx.mm doesn't have to consider the deletion
	// of the Global object and its LogEmitter object.
	le = g.le;

	if (home) {
		size_t len = strlen(home) + strlen(logpath) + 1;
		STACKVAR(char, buff, len);
		memset(buff, 0, len);
		strcat(buff, home);
		strcat(buff, logpath);
		fConsole = fopen(buff, "a+");
		if (fConsole) {
#if QT_VERSION >= 0x050000
		qInstallMessageHandler(mumbleMessageOutputWithContext);
#else
		qInstallMsgHandler(mumbleMessageOutput);
#endif
		}
	}

	/* Query for language setting. OS X's LANG environment variable is determined from the region selected
	 * in SystemPrefs -> International -> Formats -> Region instead of the system language. We override this
	 * by always using the system langauge, to get rid of all sorts of nasty langauge inconsistencies. */
	query_language();
}
