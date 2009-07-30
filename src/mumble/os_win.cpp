/* Copyright (C) 2005-2009, Thorvald Natvig <thorvald@natvig.com>

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

#include <windows.h>
#include <tlhelp32.h>
#include <dbghelp.h>

extern "C" {
void __cpuid(int a[4], int b);
};

#define PATH_MAX 1024

static FILE *fConsole = NULL;
static wchar_t wcCrashDumpPath[PATH_MAX] = { 0, };

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
	fprintf(fConsole, "<%c>%s %s\n", c, qPrintable(QDateTime::currentDateTime().toString(QLatin1String("yyyy-MM-dd hh:mm:ss.zzz"))), msg);
	fflush(fConsole);
	OutputDebugStringA(msg);
	if (type == QtFatalMsg) {
		::MessageBoxA(NULL, msg, "Mumble", MB_OK | MB_ICONERROR);
		exit(0);
	}
}

static LONG WINAPI MumbleUnhandledExceptionFilter(struct _EXCEPTION_POINTERS* ExceptionInfo) {
	MINIDUMP_EXCEPTION_INFORMATION i;
	i.ThreadId = GetCurrentThreadId();
	i.ExceptionPointers = ExceptionInfo;

	HANDLE hMinidump = CreateFile(wcCrashDumpPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hMinidump == INVALID_HANDLE_VALUE) {
		goto out;
	}

	if (! MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hMinidump, MiniDumpWithThreadInfo, &i, NULL, NULL)) {
		goto out;
	}

	FlushFileBuffers(hMinidump);
	CloseHandle(hMinidump);

out:
	return EXCEPTION_CONTINUE_SEARCH;
}

void os_init() {
	int cpuinfo[4];
	__cpuid(cpuinfo, 1);

#define MMXSSE 0x02800000
	if ((cpuinfo[3] & MMXSSE) != MMXSSE) {
		::MessageBoxA(NULL, "Mumble requires a SSE capable processor (Pentium 3 / Ahtlon-XP)", "Mumble", MB_OK | MB_ICONERROR);
		exit(0);
	}

#ifdef QT_NO_DEBUG
	// FIXME: QDesktopServices::storageLocation
	errno_t res = 0;
	size_t reqSize, bSize;
	_wgetenv_s(&reqSize, NULL, 0, L"APPDATA");
	if (reqSize > 0) {
		reqSize += strlen("/Mumble/Console.txt");
		bSize = reqSize;

		STACKVAR(wchar_t, buff, reqSize+1);

		_wgetenv_s(&reqSize, buff, bSize, L"APPDATA");
		wcscat_s(buff, bSize, L"/Mumble/Console.txt");
		res = _wfopen_s(&fConsole, buff, L"a+");
	}
	if ((res != 0) || (! fConsole)) {
		res=_wfopen_s(&fConsole, L"Console.txt", L"a+");
	}
	if ((res == 0) && fConsole)
		qInstallMsgHandler(mumbleMessageOutput);

	_wgetenv_s(&reqSize, wcCrashDumpPath, PATH_MAX, L"APPDATA");
	if (reqSize > 0) {
		wcscat_s(wcCrashDumpPath, PATH_MAX, L"/Mumble/mumble.dmp");
		SetUnhandledExceptionFilter(MumbleUnhandledExceptionFilter);
	}

	// Increase our priority class to live alongside games.
	if (!SetPriorityClass(GetCurrentProcess(),HIGH_PRIORITY_CLASS))
		qWarning("Application: Failed to set priority!");
#endif
}
