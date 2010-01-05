/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>

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
static HHOOK hhookWnd = 0;

SharedMem *sm = NULL;
HANDLE hSharedMutex = NULL;
HMODULE hSelf = NULL;
static BOOL bMumble = FALSE;
static BOOL bDebug = FALSE;

static HardHook hhLoad;
static HardHook hhLoadW;
void *HardHook::pCode = NULL;
unsigned int HardHook::uiCode = 0;

HardHook::HardHook() {
	int i;
	baseptr = NULL;
	for (i=0;i<6;i++)
		orig[i]=replace[i]=0;
}

static unsigned int modrmbytes(unsigned char a, unsigned char b) {
	unsigned char lower = (a & 0x0f);
	if (a >= 0xc0) {
		return 0;
	} else if (a >= 0x80) {
		if ((lower == 4) || (lower == 12))
			return 5;
		else
			return 4;
	} else if (a >= 0x40) {
		if ((lower == 4) || (lower == 12))
			return 2;
		else
			return 1;

	} else {
		if ((lower == 4) || (lower == 12)) {
			if ((b & 0x07) == 0x05)
				return 5;
			else
				return 1;
		} else if ((lower == 5) || (lower == 13))
			return 4;
		return 0;
	}
}

void *HardHook::cloneCode(void **porig) {
	if (! pCode || uiCode > 4000) {
		uiCode = 0;
		pCode = VirtualAlloc(NULL, 4096, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	}

	unsigned char *o = (unsigned char *) *porig;
	unsigned char *n = (unsigned char *) pCode;
	n += uiCode;
	unsigned int idx = 0;

	while (*o == 0xe9) {
		int *iptr = reinterpret_cast<int *>(o+1);
		o += *iptr + 5;

		ods("Chaining from %p to %p", *porig, o);
		*porig = o;
	}

	do {
		unsigned char opcode = o[idx];
		unsigned char a = o[idx+1];
		unsigned char b = o[idx+2];
		unsigned int extra = 0;

		n[idx] = opcode;
		idx++;

		switch (opcode) {
			case 0x50: // PUSH
			case 0x51:
			case 0x52:
			case 0x53:
			case 0x54:
			case 0x55:
			case 0x56:
			case 0x57:
			case 0x58: // POP
			case 0x59:
			case 0x5a:
			case 0x5b:
			case 0x5c:
			case 0x5d:
			case 0x5e:
			case 0x5f:
				break;
			case 0x68:
				extra = 4;
				break;
			case 0x81: // CMP immediate
				extra = modrmbytes(a,b) + 5;
				break;
			case 0x83:	// CMP
				extra = modrmbytes(a,b) + 2;
				break;
			case 0x8b:	// MOV
				extra = modrmbytes(a,b) + 1;
				break;
			default:
				fods("Unknown opcode at %d: %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x", idx-1, o[0], o[1], o[2], o[3], o[4], o[5], o[6], o[7], o[8], o[9], o[10], o[11]);
				return NULL;
				break;
		}
		for (unsigned int i=0;i<extra;++i)
			n[idx+i] = o[idx+i];
		idx += extra;

	} while (idx < 6);

	n[idx++] = 0xe9;
	int offs = o - n - 5;

	int *iptr = reinterpret_cast<int *>(&n[idx]);
	*iptr = offs;
	idx += 4;

	uiCode += idx;
	FlushInstructionCache(GetCurrentProcess(), n, idx);

	return n;
}

void HardHook::setup(voidFunc func, voidFunc replacement) {
	int i;
	DWORD oldProtect, restoreProtect;

	if (baseptr)
		return;

	unsigned char *fptr = reinterpret_cast<unsigned char *>(func);
	unsigned char *nptr = reinterpret_cast<unsigned char *>(replacement);

	ods("HH: Asked to replace %p with %p", func, replacement);

	if (VirtualProtect(fptr, 16, PAGE_EXECUTE_READ, &oldProtect)) {
		call = (voidFunc) cloneCode((void **) &fptr);

		if (call) {
			bTrampoline = true;
		} else {
			bTrampoline = false;
			call = func;
		}

		unsigned char **iptr = reinterpret_cast<unsigned char **>(&replace[1]);
		*iptr = nptr;
		replace[0] = 0x68;
		replace[5] = 0xc3;

		for (i=0;i<6;i++)
			orig[i]=fptr[i];

		baseptr = fptr;
		inject(true);

		VirtualProtect(fptr, 16, oldProtect, &restoreProtect);
	} else {
		ods("Failed initial vprotect");
	}
}

void HardHook::setupInterface(IUnknown *unkn, LONG funcoffset, voidFunc replacement) {
	ods("HH: Replacing %p function #%ld", unkn, funcoffset);
	void **ptr = reinterpret_cast<void **>(unkn);
	ptr = reinterpret_cast<void **>(ptr[0]);
	setup(reinterpret_cast<voidFunc>(ptr[funcoffset]), replacement);
}

void HardHook::inject(bool force) {
	DWORD oldProtect, restoreProtect;
	int i;

	if (! baseptr)
		return;
	if (! force && bTrampoline)
		return;

	if (VirtualProtect(baseptr, 6, PAGE_EXECUTE_READWRITE, &oldProtect)) {
		for (i=0;i<6;i++)
			baseptr[i] = replace[i];
		VirtualProtect(baseptr, 6, oldProtect, &restoreProtect);
		FlushInstructionCache(GetCurrentProcess(),baseptr, 6);
	}
	for (i=0;i<6;i++)
		if (baseptr[i] != replace[i])
			ods("HH: Injection failure at byte %d", i);
}

void HardHook::restore(bool force) {
	DWORD oldProtect, restoreProtect;
	int i;

	if (! baseptr)
		return;
	if (! force && bTrampoline)
		return;

	if (VirtualProtect(baseptr, 6, PAGE_EXECUTE_READWRITE, &oldProtect)) {
		for (i=0;i<6;i++)
			baseptr[i] = orig[i];
		VirtualProtect(baseptr, 6, oldProtect, &restoreProtect);
		FlushInstructionCache(GetCurrentProcess(),baseptr, 6);
	}
}

void HardHook::print() {
	ods("HH: %02x %02x %02x %02x %02x => %02x %02x %02x %02x %02x (%02x %02x %02x %02x %02x)",
	    orig[0], orig[1], orig[2], orig[3], orig[4],
	    replace[0], replace[1], replace[2], replace[3], replace[4],
	    baseptr[0], baseptr[1], baseptr[2], baseptr[3], baseptr[4]);
}

void HardHook::check() {
	if (memcmp(baseptr, replace, 6) != 0) {
		if (memcmp(baseptr, orig, 6) == 0) {
			ods("HH: Restoring function %p", baseptr);
			inject(true);
		} else {
			ods("HH: Function %p replaced by third party. Lost.");
		}
	}
}

FakeInterface::FakeInterface(IUnknown *orig, int entries) {
	this->pOriginal = orig;
	pAssembly = VirtualAlloc(NULL, entries * 256, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	unsigned char *f = (unsigned char *) pAssembly;
	vtbl = new void *[entries];

	pNew = (IUnknown *) &vtbl;

	ods("Allocated %p for %d", f, entries);
	for (int i=0;i<entries;i++) {
		DWORD offset = i * 4;
		vtbl[i] = f;

		f[0] = 0x8b; // mov eax, [esp+4]
		f[1] = 0x44;
		f[2] = 0xe4;
		f[3] = 0x04;
		f+=4;

		f[0] = 0x83; // sub eax, 4
		f[1] = 0xc0;
		f[2] = 0x04;
		f+=3;

		f[0] = 0x8b; // mov eax, [eax]
		f[1] = 0x00;
		f+=2;

		f[0] = 0x89; // mov [esp+4], eax
		f[1] = 0x44;
		f[2] = 0xe4;
		f[3] = 0x04;
		f+=4;

		f[0] = 0x8b; // mov eax, [eax]
		f[1] = 0x00;
		f+=2;

		f[0] = 0x8b; // mov eax, [eax + offset]
		f[1] = 0x80;
		f[2] = ((offset >> 0) & 0xFF);
		f[3] = ((offset >> 8) & 0xFF);
		f[4] = ((offset >> 16) & 0xFF);
		f[5] = ((offset >> 24) & 0xFF);
		f+=6;

		f[0] = 0xff; // jmp eax
		f[1] = 0xe0;
		f+=2;
	}
}

FakeInterface::~FakeInterface() {
	VirtualFree((void *) pAssembly, 0, MEM_RELEASE);
	delete [] vtbl;
}

void FakeInterface::replace(LONG offset, voidMemberFunc replacement) {
	void *p = NULL;
	_asm {
		mov eax, replacement
		mov p, eax
	}
	ods("That gave %p", p);
	vtbl[offset] = p;
}

CRITICAL_SECTION Mutex::cs;

void Mutex::init() {
	InitializeCriticalSection(&cs);
}

Mutex::Mutex() {
	if (! TryEnterCriticalSection(&cs)) {
		ods("CritFail");
		EnterCriticalSection(&cs);
	}
}

Mutex::~Mutex() {
	LeaveCriticalSection(&cs);
}

void __cdecl fods(const char *format, ...) {
	char    buf[4096], *p = buf;
	va_list args;

	va_start(args, format);
	int len = _vsnprintf_s(p, sizeof(buf) - 1, _TRUNCATE, format, args);
	va_end(args);

	if (len <= 0)
		return;

	p += len;

	while (p > buf  &&  isspace(p[-1]))
		*--p = '\0';

	*p++ = '\r';
	*p++ = '\n';
	*p   = '\0';

	OutputDebugStringA(buf);
}


void __cdecl ods(const char *format, ...) {
#ifndef DEBUG
	if (!bDebug && (!sm || ! sm->bDebug))
		return;
#endif
	char    buf[4096], *p = buf;
	va_list args;

	va_start(args, format);
	int len = _vsnprintf_s(p, sizeof(buf) - 1, _TRUNCATE, format, args);
	va_end(args);

	if (len <= 0)
		return;

	p += len;

	while (p > buf  &&  isspace(p[-1]))
		*--p = '\0';

	*p++ = '\r';
	*p++ = '\n';
	*p   = '\0';

	OutputDebugStringA(buf);
}

static const char *blacklist[] = {
	"iexplore.exe",
	"ieuser.exe",
	"vlc.exe",
	"dbgview.exe",
	"opera.exe",
	"explorer.exe",
	NULL
};

typedef HMODULE(__stdcall *LoadLibraryAType)(const char *);
static HMODULE WINAPI MyLoadLibrary(const char *lpFileName) {
	LoadLibraryAType oLoadLibrary = (LoadLibraryAType) hhLoad.call;
	hhLoad.restore();

	HMODULE h = oLoadLibrary(lpFileName);
	ods("Library %s loaded to %p", lpFileName, h);

	if (! bMumble) {
		checkD3D9Hook();
		checkDXGIHook();
		checkOpenGLHook();
	}

	hhLoad.inject();
	return h;
}

typedef HMODULE(__stdcall *LoadLibraryWType)(const wchar_t *);
static HMODULE WINAPI MyLoadLibraryW(const wchar_t *lpFileName) {
	LoadLibraryWType oLoadLibrary = (LoadLibraryWType) hhLoadW.call;
	hhLoadW.restore();

	HMODULE h = oLoadLibrary(lpFileName);
	ods("Library %ls wloaded to %p", lpFileName, h);

	if (! bMumble) {
		checkD3D9Hook();
		checkDXGIHook();
		checkOpenGLHook();
	}

	hhLoadW.inject();
	return h;
}

static LRESULT CALLBACK CallWndProc(int nCode, WPARAM wParam, LPARAM lParam) {
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
			GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (char *) &InstallHooks, &hSelf);
			if (hSelf == NULL) {
				ods("Lib: Failed to find myself");
			} else {
				hhookWnd = SetWindowsHookEx(WH_CBT, CallWndProc, hSelf, 0);
				if (hhookWnd == NULL)
					ods("Lib: Failed to insert WNDProc hook");
			}

			sm->bHooked = true;
		}
		ReleaseMutex(hHookMutex);
	}
}

extern "C" __declspec(dllexport) unsigned int __cdecl GetOverlayMagicVersion() {
	return OVERLAY_MAGIC_NUMBER;
}

extern "C" __declspec(dllexport) SharedMem * __cdecl GetSharedMemory() {
	return sm;
}

extern "C" BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID) {

	char procname[1024+64];
	GetModuleFileName(NULL, procname, 1024);

	switch (fdwReason) {
		case DLL_PROCESS_ATTACH: {
				Mutex::init();
				ods("Lib: ProcAttach: %s", procname);
				char *p = strrchr(procname, '\\');

				if (p) {
					if ((_stricmp(p+1, "mumble.exe")==0) || (_stricmp(p+1, "mumble11x.exe")==0))
						bMumble = TRUE;
					int i =0;
					while (blacklist[i]) {
						if (_stricmp(p+1,blacklist[i])==0) {
							ods("Process %s is blacklisted", procname);
							return TRUE;
						}
						i++;
					}
					strcpy_s(p+1, 64, "nooverlay");

					HANDLE h = CreateFile(procname, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
					if (h != INVALID_HANDLE_VALUE) {
						CloseHandle(h);
						fods("Overlay disable %s found", procname);
						return TRUE;
					}

					strcpy_s(p+1, 64, "debugoverlay");

					h = CreateFile(procname, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
					if (h != INVALID_HANDLE_VALUE) {
						CloseHandle(h);
						fods("Overlay debug %s found", procname);
						bDebug = TRUE;
					}
				}

				hSharedMutex = CreateMutex(NULL, false, "MumbleSharedMutex");
				hHookMutex = CreateMutex(NULL, false, "MumbleHookMutex");
				if ((hSharedMutex == NULL) || (hHookMutex == NULL)) {
					ods("Lib: CreateMutex failed");
					return TRUE;
				}

				DWORD dwWaitResult = WaitForSingleObject(hSharedMutex, 1000L);
				if (dwWaitResult != WAIT_OBJECT_0) {
					ods("Lib: WaitForMutex failed");
					return TRUE;
				}

				DWORD dwSharedSize = sizeof(SharedMem) + sizeof(Direct3D9Data) + sizeof(DXGIData);

				hMapObject = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, dwSharedSize, "MumbleSharedMemory");
				if (hMapObject == NULL) {
					ods("Lib: CreateFileMapping failed");
					ReleaseMutex(hSharedMutex);
					return TRUE;
				}

				bool bInit = (GetLastError() != ERROR_ALREADY_EXISTS);

				sm = (SharedMem *) MapViewOfFile(hMapObject, FILE_MAP_ALL_ACCESS, 0, 0, dwSharedSize);

				if (sm == NULL) {
					ods("MapViewOfFile Failed");
					ReleaseMutex(hSharedMutex);
					return TRUE;
				}

				unsigned char *raw = (unsigned char *) sm;
				d3dd = (Direct3D9Data *)(raw + sizeof(SharedMem));
				dxgi = (DXGIData *)(raw + sizeof(SharedMem) + sizeof(Direct3D9Data));

				if (bInit) {
					memset(sm, 0, sizeof(SharedMem) + sizeof(Direct3D9Data) + sizeof(DXGIData));
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

				if (! bMumble) {
					hhLoad.setup(reinterpret_cast<voidFunc>(LoadLibraryA), reinterpret_cast<voidFunc>(MyLoadLibrary));
					hhLoadW.setup(reinterpret_cast<voidFunc>(LoadLibraryW), reinterpret_cast<voidFunc>(MyLoadLibraryW));

					// Hm. Don't check D3D9 as apparantly it's creation causes problems in some applications.
					checkD3D9Hook(true);
					checkDXGIHook(true);
					checkOpenGLHook();
					ods("Injected");
				}
			}
			break;
		case DLL_PROCESS_DETACH: {
				ods("Lib: ProcDetach: %s", procname);
				hhLoad.restore(true);
				hhLoadW.restore(true);
				if (sm)
					UnmapViewOfFile(sm);
				if (hMapObject)
					CloseHandle(hMapObject);
				if (hSharedMutex)
					CloseHandle(hSharedMutex);
				if (hHookMutex)
					CloseHandle(hHookMutex);
			}
			break;
		case DLL_THREAD_ATTACH: {
				static bool bTriedHook = false;
				if (sm && ! bTriedHook && ! bMumble) {
					bTriedHook = true;
					checkD3D9Hook();
					checkDXGIHook();
					checkOpenGLHook();
				}
			}
			break;
		default:
			break;
	}
	return TRUE;
}
