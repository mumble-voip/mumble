/* Copyright (C) 2005, Thorvald Natvig <thorvald@natvig.com>

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

#include "lib.h"

static HANDLE hMapObject = NULL;
static HANDLE hHookMutex = NULL;

SharedMem *sm;
HANDLE hSharedMutex = NULL;
HMODULE hSelf = NULL;

bool bVideoHooked = false;
unsigned int uiAudioCount = 0;

static HHOOK hhookWnd = 0;

HardHook::HardHook() {
	int i;
	baseptr = NULL;
	for(i=0;i<5;i++)
		orig[i]=replace[i]=0;
}

void HardHook::setup(voidFunc func, voidFunc replacement) {
	int i;
	DWORD oldProtect, restoreProtect;

	if (baseptr)
		return;

	unsigned char *fptr = reinterpret_cast<unsigned char *>(func);
	unsigned char *nptr = reinterpret_cast<unsigned char *>(replacement);

	ods("HH: Asked to replace %p with %p", func, replacement);

	int offs = nptr - fptr - 5;
	int *iptr = reinterpret_cast<int *>(&replace[1]);
	*iptr = offs;
	replace[0] = 0xe9;

	if (VirtualProtect(fptr, 5, PAGE_EXECUTE_READ, &oldProtect)) {
		for(i=0;i<5;i++)
			orig[i]=fptr[i];
		VirtualProtect(fptr, 5, oldProtect, &restoreProtect);
		baseptr = fptr;
	}
}

void HardHook::setupInterface(IUnknown *unkn, LONG funcoffset, voidFunc replacement) {
	ods("HH: Replacing %p function #%ld", unkn, funcoffset);
	void **ptr = reinterpret_cast<void **>(unkn);
	ptr = reinterpret_cast<void **>(ptr[0]);
	setup(reinterpret_cast<voidFunc>(ptr[funcoffset]), replacement);
}

void HardHook::inject() {
	DWORD oldProtect, restoreProtect;
	int i;

	if (! baseptr)
		return;
	if (VirtualProtect(baseptr, 5, PAGE_EXECUTE_READWRITE, &oldProtect)) {
		for(i=0;i<5;i++)
			baseptr[i] = replace[i];
		VirtualProtect(baseptr, 5, oldProtect, &restoreProtect);
		FlushInstructionCache(GetCurrentProcess(),baseptr, 5);
	}
}

void HardHook::restore() {
	DWORD oldProtect, restoreProtect;
	int i;

	if (! baseptr)
		return;
	if (VirtualProtect(baseptr, 5, PAGE_EXECUTE_READWRITE, &oldProtect)) {
		for(i=0;i<5;i++)
			baseptr[i] = orig[i];
		VirtualProtect(baseptr, 5, oldProtect, &restoreProtect);
		FlushInstructionCache(GetCurrentProcess(),baseptr, 5);
	}
}

void __cdecl ods(const char *format, ...) {
        if (!sm || ! sm->bDebug)
                return;

        char    buf[4096], *p = buf;
        va_list args;

        va_start(args, format);
        p += _vsnprintf(p, sizeof buf - 1, format, args);
        va_end(args);

        while ( p > buf  &&  isspace(p[-1]) )
                *--p = '\0';

        *p++ = '\r';
        *p++ = '\n';
        *p   = '\0';

        OutputDebugStringA(buf);
        FILE *f = fopen("c:\\overlay.log", "a");
        fprintf(f, "%d %s", GetTickCount(), buf);
        fclose(f);
}

static LRESULT CALLBACK CallWndProc(int nCode, WPARAM wParam, LPARAM lParam) {
	CWPSTRUCT *s = reinterpret_cast<CWPSTRUCT *>(lParam);
	if (s) {
		switch (s->message) {
			case WM_CREATE:
			case WM_DESTROY:
			case WM_SETFOCUS:
			case WM_GETMINMAXINFO:	// For things that link directly
			case WM_GETICON:		// Worked for BF2
			case WM_NCCREATE:		// Lots of games
				checkD3D9Hook();
				checkOpenGLHook();
				checkOpenALHook();
				// checkDSHook(s->hwnd);
				break;
			default:
				break;
		}
	}
	return CallNextHookEx(hhookWnd, nCode, wParam, lParam);
}

extern "C" __declspec(dllexport) void __cdecl RemoveHooks() {
	DWORD dwWaitResult = WaitForSingleObject(hHookMutex, 1000L);
	if (dwWaitResult == WAIT_OBJECT_0) {
		if (sm->bHooked) {
			if (hhookWnd) {
				UnhookWindowsHookEx(hhookWnd);
				hhookWnd = NULL;
			}
			sm->bHooked = false;
		}
		ReleaseMutex(hHookMutex);
	}
}

extern "C" __declspec(dllexport) void __cdecl InstallHooks() {
	sm->lastAppAlive = GetTickCount();

	DWORD dwWaitResult = WaitForSingleObject(hHookMutex, 1000L);
	if (dwWaitResult == WAIT_OBJECT_0) {
		if (! sm->bHooked) {
			GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (wchar_t *) &InstallHooks, &hSelf);
			if (hSelf == NULL) {
				ods("Lib: Failed to find myself");
			} else {
				hhookWnd = SetWindowsHookEx(WH_CALLWNDPROC,CallWndProc,hSelf,0);
				if (hhookWnd == NULL)
					ods("Lib: Failed to insert WNDProc hook");
			}
			sm->bHooked = true;
		}
		ReleaseMutex(hHookMutex);
	}
}

extern "C" __declspec(dllexport) SharedMem * __cdecl GetSharedMemory() {
	return sm;
}

extern "C" BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	switch (fdwReason) {
		case DLL_PROCESS_ATTACH:
			{
				hSharedMutex = CreateMutex(NULL, false, L"MumbleSharedMutex");
				hHookMutex = CreateMutex(NULL, false, L"MumbleHookMutex");
				if ((hSharedMutex == NULL) || (hHookMutex == NULL)) {
					ods("Lib: CreateMutex failed");
					return FALSE;
				}

				DWORD dwWaitResult = WaitForSingleObject(hSharedMutex, 1000L);
				if (dwWaitResult != WAIT_OBJECT_0) {
					ods("Lib: WaitForMutex failed");
					return FALSE;
				}

				hMapObject = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(SharedMem), L"MumbleSharedMemory");
				if (hMapObject == NULL) {
					ods("Lib: CreateFileMapping failed");
					ReleaseMutex(hSharedMutex);
					return FALSE;
				}

				bool bInit = (GetLastError() != ERROR_ALREADY_EXISTS);

				sm = (SharedMem *) MapViewOfFile(hMapObject, FILE_MAP_WRITE, 0, 0, 0);
				if (sm == NULL) {
					ods("MapViewOfFile Failed");
					ReleaseMutex(hSharedMutex);
					return FALSE;
				}
				if (bInit) {
					memset(sm, 0, sizeof(SharedMem));
					sm->lastAppAlive = 0;
					sm->bHooked = false;
					sm->bDebug = false;
					sm->bShow = true;
					sm->bReset = false;
					sm->fX = sm->fY = 1.0;
					sm->bTop = false;
					sm->bBottom = true;
					sm->bLeft = true;
					sm->bRight = false;
					sm->fFontSize = 72;
				}
				ReleaseMutex(hSharedMutex);
			}
			break;
		case DLL_PROCESS_DETACH:
			{
				UnmapViewOfFile(sm);
				CloseHandle(hMapObject);
				CloseHandle(hSharedMutex);
				CloseHandle(hHookMutex);
			}
			break;
		default:
			break;
	}
	return TRUE;
}
