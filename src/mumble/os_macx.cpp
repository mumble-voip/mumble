/* Copyright (C) 2005-2008, Thorvald Natvig <thorvald@natvig.com>
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

#undef qDebug
#include <Carbon/Carbon.h>
#include <sys/stat.h>
#include <stdio.h>

#undef check
#include "MainWindow.h"
#include "Global.h"

char *os_url = NULL;
char *os_lang = NULL;
static FILE *fConsole = NULL;

static OSErr urlCallback(const AppleEvent *ae, AppleEvent *, void *) {
	OSErr err;
	DescType type;
	Size dataSize, size;
	static char url[1024];

	err = AESizeOfParam(ae, keyDirectObject, &type, &dataSize);
	if (err != noErr)
		return err;

	memset(url, 0, 1024);
	if (dataSize > 1023)
		dataSize = 1023;

	err = AEGetParamPtr(ae, keyDirectObject, typeChar, &type, url, dataSize, &size);
	if (err != noErr)
		return err;

	/*
	 * Determine whether a mumble:// URL invoked a Mumble launch
	 * of if we were already running beforehand.
	 */
	if (g.mw)
		g.mw->openUrl(QUrl(QString(url)));
	else
		os_url = url;

	return noErr;
}

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
		CFStringRef csMsg = CFStringCreateWithCStringNoCopy(NULL, msg, kCFStringEncodingUTF8, NULL);
		CFUserNotificationDisplayAlert(0, 0, NULL,  NULL, NULL, CFSTR("Mumble"), csMsg, CFSTR("OK"), NULL, NULL, NULL);
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

void os_init() {
	const char *home = getenv("HOME");
	const char *path = "/Library/Logs/Mumble.log";

	/* Console.txt logging. */
	if (home) {
		size_t len = strlen(home) + strlen(path) + 1;
		STACKVAR(char, buff, len);
		memset(buff, 0, len);
		strcat(buff, home);
		strcat(buff, path);
		fConsole = fopen(buff, "a+");
		if (fConsole)
			qInstallMsgHandler(mumbleMessageOutput);
	}

	/* Query for language setting. OS X's LANG environment variable is determined from the region selected
	 * in SystemPrefs -> International -> Formats -> Region instead of the system language. We override this
	 * by always using the system langauge, to get rid of all sorts of nasty langauge inconsistencies. */
	query_language();

	/* Install Apple Event handler for GURL events. This intercepts any URLs set in Mumble's Info.plist. */
	AEInstallEventHandler(kInternetEventClass, kAEGetURL, NewAEEventHandlerUPP(urlCallback), 0, false);
}
