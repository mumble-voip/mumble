/* Copyright (C) 2005-2006, Thorvald Natvig <thorvald@natvig.com>

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

static FILE *fConsole;
static void mumbleMessageOutput(QtMsgType type, const char *msg)
{
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
	fprintf(fConsole, "<%c>%s %s\n", c, qPrintable(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz")), msg);
	fflush(fConsole);
}

#define ER ExceptionInfo->ExceptionRecord
#define CT ExceptionInfo->ContextRecord
static LONG WINAPI MumbleUnhandledExceptionFilter(struct _EXCEPTION_POINTERS* ExceptionInfo) {
	EXCEPTION_RECORD *er = ExceptionInfo->ExceptionRecord;
	CONTEXT *ctx = ExceptionInfo->ContextRecord;

	qWarning("=============================================================================");
	qWarning("Mumble crash report. Cut and paste this entire report as well as the previos");
	qWarning("10 seconds (see the timestamps). Build " __DATE__ " " __TIME__);
	qWarning("=============================================================================");

	while (er) {
		qWarning("Unhandled exception. Code 0x%08lx, Flags 0x%08lx, Address %p.",er->ExceptionCode,er->ExceptionFlags,er->ExceptionAddress);

		MODULEENTRY32 me;
		me.dwSize = sizeof(me);
		HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());
		if (hSnap != INVALID_HANDLE_VALUE) {
			bool ok = Module32First(hSnap, &me);

			while (ok) {
				BYTE *start = me.modBaseAddr;
				BYTE *stop = me.modBaseAddr + me.modBaseSize;
				if ((er->ExceptionAddress >= start) && (er->ExceptionAddress <= stop)) {
					qWarning("Modulematch: %ls (%p %08x)", me.szModule, start, static_cast<BYTE *>(er->ExceptionAddress) - start);
				}
				ok = Module32Next(hSnap, &me);
			}
			CloseHandle(hSnap);
		}

		for(unsigned int i=0;i<er->NumberParameters;i++) {
			qWarning("Parameter %4d: %08lx", i, er->ExceptionInformation[i]);
		}
		if (er->ExceptionRecord)
			qWarning("Chaining record...");
		er = er->ExceptionRecord;
	}

	qWarning("EAX %08lx EBX %08lx ECX %08lx EDX %08lx",ctx->Eax,ctx->Ebx,ctx->Ecx,ctx->Edx);
	qWarning("ESI %08lx EDI %08lx EBP %08lx ESP %08lx",ctx->Esi,ctx->Edi,ctx->Ebp,ctx->Esp);

	DWORD Sp=ctx->Esp;
	DWORD Bp=ctx->Ebp;

	DWORD start,stop;
	start = Sp;
	stop = Bp + 0x40;
	if ((stop < start) || (stop-start >512))
		stop = start + 512;
	for(Sp=stop;Sp>=start;Sp-=sizeof(DWORD)) {
		DWORD val[4] = {0,0,0,0};
		DWORD nbytes = sizeof(DWORD)*4;
		if (ReadProcessMemory(GetCurrentProcess(), reinterpret_cast<const void *>(Sp), &val[0],sizeof(DWORD)*4, &nbytes))
			qWarning("%08lx[%04lx][%08lx] %08lx %08lx %08lx %08lx", Sp, Sp-ctx->Esp,Sp-Bp, val[0],val[1],val[2],val[3]);
		else
			break;
	}

	qWarning("=============================================================================");
	qWarning("Crash report ends");
	qWarning("=============================================================================");


	return EXCEPTION_CONTINUE_SEARCH;
}

void os_init() {
#define cpuid(func,ax,bx,cx,dx) __asm__ __volatile__ ("cpuid": "=a" (ax), "=b" (bx), "=c" (cx), "=d" (dx) : "a" (func));
#define MMXSSE 0x02800000
	unsigned int ax, bx, cx, dx;
	cpuid(1,ax,bx,cx,dx);
	if ((dx & MMXSSE) != MMXSSE) {
		::MessageBoxA(NULL, "Mumble requires a SSE capable processor (Pentium 3 / Ahtlon-XP)", "Mumble", MB_OK | MB_ICONERROR);
		exit(0);
	}


#ifdef QT_NO_DEBUG
	fConsole=fopen("Console.txt", "a+");
	if (fConsole)
		qInstallMsgHandler(mumbleMessageOutput);
	SetUnhandledExceptionFilter(MumbleUnhandledExceptionFilter);

	// Increase our priority class to live alongside games.
	if (!SetPriorityClass(GetCurrentProcess(),HIGH_PRIORITY_CLASS))
		qWarning("Application: Failed to set priority!");
#endif
}
