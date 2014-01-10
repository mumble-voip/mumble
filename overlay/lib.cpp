/* Copyright (C) 2005-2011, Thorvald Natvig <thorvald@natvig.com>

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

#include "overlay_blacklist.h"

static HANDLE hMapObject = NULL;
static HANDLE hHookMutex = NULL;
static HHOOK hhookWnd = 0;

BOOL bIsWin8 = FALSE;

static BOOL bMumble = FALSE;
static BOOL bDebug = FALSE;
static BOOL bBlackListed = FALSE;

static HardHook hhLoad;
static HardHook hhLoadW;
static HardHook hhFree;

static SharedData *sd = NULL;

FakeInterface::FakeInterface(IUnknown *orig, int entries) {
	this->pOriginal = orig;
	pAssembly = VirtualAlloc(NULL, entries * 256, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	unsigned char *f = (unsigned char *) pAssembly;
	vtbl = new void *[entries];

	pNew = (IUnknown *) &vtbl;

	ods("Lib: FakeInterface: Allocated %p for %d number of entries", f, entries);
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
	ods("Lib: FakeInterface: replace: That gave %p", p);
	vtbl[offset] = p;
}

CRITICAL_SECTION Mutex::cs;

void Mutex::init() {
	InitializeCriticalSection(&cs);
}

Mutex::Mutex() {
	if (! TryEnterCriticalSection(&cs)) {
		ods("Lib: Mutex: CritFail - blocking until able to enter critical section");
		EnterCriticalSection(&cs);
	}
}

Mutex::~Mutex() {
	LeaveCriticalSection(&cs);
}

void __cdecl ods(const char *format, ...) {
#ifndef DEBUG
	if (!bDebug)
		return;
#endif
	va_list args;

	va_start(args, format);
	_ods_out(format, &args);
	va_end(args);
}

void __cdecl checkForWPF() {
	if (!bBlackListed && (GetModuleHandleW(L"wpfgfx_v0300.dll") || GetModuleHandleW(L"wpfgfx_v0400.dll"))) {
		ods("Lib: Blacklisted for loading WPF library");
		bBlackListed = TRUE;
	}
}

Pipe::Pipe() {
	hSocket = INVALID_HANDLE_VALUE;
	hMemory = NULL;
	a_ucTexture = NULL;

	omMsg.omh.iLength = -1;

	dwAlreadyRead = 0;

	uiWidth = uiHeight = 0;
	uiLeft = uiRight = uiTop = uiBottom = 0;
}

Pipe::~Pipe() {
	disconnect();
}

void Pipe::release() {
	if (hMemory) {
		CloseHandle(hMemory);
		hMemory = NULL;
		if (a_ucTexture) {
			UnmapViewOfFile(a_ucTexture);
			a_ucTexture = NULL;
		}

		uiLeft = uiRight = uiTop = uiBottom = 0;
	}
}

void Pipe::disconnect() {
	release();
	if (hSocket != INVALID_HANDLE_VALUE) {
		ods("Pipe: Disconnect");
		CloseHandle(hSocket);
		hSocket = INVALID_HANDLE_VALUE;
	}
	uiWidth = 0;
	uiHeight = 0;
	omMsg.omh.iLength = -1;
}

bool Pipe::sendMessage(const OverlayMsg &om) {
	DWORD dwBytesToWrite = sizeof(OverlayMsgHeader) + om.omh.iLength;
	DWORD dwBytesWritten = dwBytesToWrite;

	if (WriteFile(hSocket, om.headerbuffer, sizeof(OverlayMsgHeader) + om.omh.iLength, &dwBytesWritten, NULL))
		if (dwBytesToWrite == dwBytesWritten)
			return true;

	ods("Pipe: Short write");
	disconnect();
	return false;
}

void Pipe::checkMessage(unsigned int width, unsigned int height) {
	if (!width || ! height)
		return;

	if (hSocket == INVALID_HANDLE_VALUE) {
		hSocket = CreateFileW(L"\\\\.\\pipe\\MumbleOverlayPipe", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
		if (hSocket == INVALID_HANDLE_VALUE) {
			ods("Pipe: Connection failed");
			return;
		}
		ods("Pipe: Connected");

		uiWidth = 0;
		uiHeight = 0;

		// initially, instantiate and send an OverlayMessage with the current process id
		OverlayMsg om;
		om.omh.uiMagic = OVERLAY_MAGIC_NUMBER;
		om.omh.uiType = OVERLAY_MSGTYPE_PID;
		om.omh.iLength = sizeof(OverlayMsgPid);
		om.omp.pid = GetCurrentProcessId();

		if (!sendMessage(om))
			return;

		ods("Pipe: Process ID sent");
	}

	// if the passed width and height do not match the current overlays uiWidth and uiHeight, re-initialize
	if ((uiWidth != width) || (uiHeight != height)) {
		release();

		uiWidth = width;
		uiHeight = height;

		// instantiate and send an initialization-OverlayMessage
		OverlayMsg om;
		om.omh.uiMagic = OVERLAY_MAGIC_NUMBER;
		om.omh.uiType = OVERLAY_MSGTYPE_INIT;
		om.omh.iLength = sizeof(OverlayMsgInit);
		om.omi.uiWidth = uiWidth;
		om.omi.uiHeight = uiHeight;

		if (!sendMessage(om))
			return;

		ods("Pipe: SentInitMsg with w h %d %d", uiWidth, uiHeight);
	}

	std::vector<RECT> blits;

	while (1) {
		DWORD dwBytesLeft;
		DWORD dwBytesRead;

		if (! PeekNamedPipe(hSocket, NULL, 0, NULL, &dwBytesLeft, NULL)) {
			ods("Pipe: Could not peek");
			disconnect();
			return;
		}

		if (! dwBytesLeft)
			break;

		if (omMsg.omh.iLength == -1) {
			if (! ReadFile(hSocket, reinterpret_cast<unsigned char *>(omMsg.headerbuffer) + dwAlreadyRead, sizeof(OverlayMsgHeader) - dwAlreadyRead, &dwBytesRead, NULL)) {
				ods("Pipe: Read header fail");
				disconnect();
				return;
			}

			dwBytesLeft -= dwBytesRead;
			dwAlreadyRead += dwBytesRead;

			if (dwAlreadyRead != sizeof(OverlayMsgHeader)) {
				break;
			}

			dwAlreadyRead = 0;

			if (omMsg.omh.uiMagic != OVERLAY_MAGIC_NUMBER) {
				ods("Pipe: Invalid magic number %x", omMsg.omh.uiMagic);
				disconnect();
				return;
			}

			if (static_cast<int>(dwBytesLeft) < omMsg.omh.iLength)
				continue;
		}

		if (! ReadFile(hSocket, reinterpret_cast<unsigned char *>(omMsg.msgbuffer) + dwAlreadyRead, omMsg.omh.iLength - dwAlreadyRead, &dwBytesRead, NULL)) {
			ods("Pipe: Read data fail");
			disconnect();
			return;
		}

		dwAlreadyRead += dwBytesRead;

		if (static_cast<int>(dwBytesLeft) < omMsg.omh.iLength)
			continue;

		dwAlreadyRead = 0;

		switch (omMsg.omh.uiType) {
			case OVERLAY_MSGTYPE_SHMEM: {
					wchar_t memname[2048];
					memname[0] = 0;

					MultiByteToWideChar(CP_UTF8, 0, omMsg.oms.a_cName, omMsg.omh.iLength, memname, 2048);

					release();

					hMemory = CreateFileMappingW(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, uiWidth * uiHeight * 4, memname);

					if (GetLastError() != ERROR_ALREADY_EXISTS) {
						ods("Pipe: Memory %s(%d) => %ls doesn't exist", omMsg.oms.a_cName, omMsg.omh.iLength, memname);
						if (hMemory) {
							CloseHandle(hMemory);
							hMemory = NULL;
							break;
						}
					}

					if (! hMemory) {
						ods("Pipe: CreateFileMapping failed");
						break;
					}

					a_ucTexture = reinterpret_cast<unsigned char *>(MapViewOfFile(hMemory, FILE_MAP_ALL_ACCESS, 0, 0, 0));

					if (a_ucTexture == NULL) {
						ods("Pipe: Failed to map memory");
						CloseHandle(hMemory);
						hMemory = NULL;
						break;
					}

					MEMORY_BASIC_INFORMATION mbi;
					memset(&mbi, 0, sizeof(mbi));
					if ((VirtualQuery(a_ucTexture, &mbi, sizeof(mbi)) == 0) || (mbi.RegionSize < (uiHeight * uiWidth * 4))) {
						ods("Pipe: Memory too small");
						UnmapViewOfFile(a_ucTexture);
						CloseHandle(hMemory);
						a_ucTexture = NULL;
						hMemory = NULL;
						break;
					}

					OverlayMsg om;
					om.omh.uiMagic = OVERLAY_MAGIC_NUMBER;
					om.omh.uiType = OVERLAY_MSGTYPE_SHMEM;
					om.omh.iLength = 0;

					if (!sendMessage(om))
						return;

					newTexture(uiWidth, uiHeight);
				}
				break;
			case OVERLAY_MSGTYPE_BLIT: {
					RECT r = {omMsg.omb.x, omMsg.omb.y, omMsg.omb.x + omMsg.omb.w, omMsg.omb.y + omMsg.omb.h};

					std::vector<RECT>::iterator i = blits.begin();
					while (i != blits.end()) {
						RECT is;
						if (::IntersectRect(&is, &r, & *i)) {
							::UnionRect(&is, &r, & *i);
							r = is;
							blits.erase(i);
							i = blits.begin();
						} else {
							++i;
						}
					}
					blits.push_back(r);
				}
				break;
			case OVERLAY_MSGTYPE_ACTIVE: {
					uiLeft = omMsg.oma.x;
					uiTop = omMsg.oma.y;
					uiRight = omMsg.oma.x + omMsg.oma.w;
					uiBottom = omMsg.oma.y + omMsg.oma.h;
					if (a_ucTexture)
						setRect();
				}
				break;
			default:
				break;
		}
		omMsg.omh.iLength = -1;
	}

	if (!a_ucTexture)
		return;

	for (std::vector<RECT>::iterator i = blits.begin(); i != blits.end(); ++i)
		blit((*i).left, (*i).top, (*i).right - (*i).left, (*i).bottom - (*i).top);
}

static void checkHooks(bool preonly) {
	checkD3D9Hook(preonly);
	checkDXGIHook(preonly);
	checkDXGI10Hook(preonly);
	checkDXGI11Hook(preonly);
	checkOpenGLHook();
}

typedef HMODULE(__stdcall *LoadLibraryAType)(const char *);
static HMODULE WINAPI MyLoadLibrary(const char *lpFileName) {
	//TODO: Move logic to HardHook.
	// Call base without active hook in case of no trampoline.
	LoadLibraryAType oLoadLibrary = (LoadLibraryAType) hhLoad.call;
	hhLoad.restore();
	HMODULE h = oLoadLibrary(lpFileName);
	hhLoad.inject();

	ods("Lib: Library %s loaded to %p", lpFileName, h);

	if (! bBlackListed) {
		checkHooks();
	}

	return h;
}

typedef HMODULE(__stdcall *LoadLibraryWType)(const wchar_t *);
static HMODULE WINAPI MyLoadLibraryW(const wchar_t *lpFileName) {
	//TODO: Move logic to HardHook.
	// Call base without active hook in case of no trampoline.
	LoadLibraryWType oLoadLibrary = (LoadLibraryWType) hhLoadW.call;
	hhLoadW.restore();
	HMODULE h = oLoadLibrary(lpFileName);
	hhLoadW.inject();

	ods("Lib: Library %ls wloaded to %p", lpFileName, h);

	checkForWPF();

	if (! bBlackListed) {
		checkHooks();
	}

	return h;
}

typedef BOOL(__stdcall *FreeLibraryType)(HMODULE hModule);
static BOOL WINAPI MyFreeLibrary(HMODULE hModule) {
	ods("Lib: MyFreeLibrary %p", hModule);

	//TODO: Move logic to HardHook.
	// Call base without active hook in case of no trampoline.
	FreeLibraryType oFreeLibrary = (FreeLibraryType) hhFree.call;
	hhFree.restore();
	BOOL r = oFreeLibrary(hModule);
	hhFree.inject();

	freeD3D9Hook(hModule);

	return r;
}

static LRESULT CALLBACK CallWndProc(int nCode, WPARAM wParam, LPARAM lParam) {
	return CallNextHookEx(hhookWnd, nCode, wParam, lParam);
}

extern "C" __declspec(dllexport) void __cdecl RemoveHooks() {
	DWORD dwWaitResult = WaitForSingleObject(hHookMutex, 1000L);
	if (dwWaitResult == WAIT_OBJECT_0) {
		if (sd != NULL && sd->bHooked) {
			if (hhookWnd) {
				UnhookWindowsHookEx(hhookWnd);
				hhookWnd = NULL;
			}
			sd->bHooked = false;
		}
		ReleaseMutex(hHookMutex);
	}
}

extern "C" __declspec(dllexport) void __cdecl InstallHooks() {
	DWORD dwWaitResult = WaitForSingleObject(hHookMutex, 1000L);
	if (dwWaitResult == WAIT_OBJECT_0) {
		if (sd != NULL && ! sd->bHooked) {
			HMODULE hSelf = NULL;
			GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (char *) &InstallHooks, &hSelf);
			if (hSelf == NULL) {
				ods("Lib: Failed to find myself");
			} else {
				hhookWnd = SetWindowsHookEx(WH_CBT, CallWndProc, hSelf, 0);
				if (hhookWnd == NULL)
					ods("Lib: Failed to insert WNDProc hook");
			}

			sd->bHooked = true;
		}
		ReleaseMutex(hHookMutex);
	}
}

extern "C" __declspec(dllexport) unsigned int __cdecl GetOverlayMagicVersion() {
	return OVERLAY_MAGIC_NUMBER;
}

static bool dllmainProcAttachCheckProcessIsBlacklisted(char procname[], char *p);
static bool createSharedDataMap();

static void dllmainProcAttach(char *procname) {
	Mutex::init();

	char *p = strrchr(procname, '\\');
	if (!p) {
		// No blacklisting if the file has no path
	} else if (GetProcAddress(NULL, "mumbleSelfDetection") != NULL) {
		ods("Lib: Attached to self (own process). Blacklisted - no overlay injection.");
		bBlackListed = TRUE;
		bMumble = TRUE;
	} else {
		if (dllmainProcAttachCheckProcessIsBlacklisted(procname, p)) {
			ods("Lib: Process %s is blacklisted - no overlay injection.", procname);
			return;
		}
	}


	OSVERSIONINFOEX ovi;
	memset(&ovi, 0, sizeof(ovi));
	ovi.dwOSVersionInfoSize = sizeof(ovi);
	GetVersionEx(reinterpret_cast<OSVERSIONINFO *>(&ovi));
	bIsWin8 = (ovi.dwMajorVersion >= 7) || ((ovi.dwMajorVersion == 6) && (ovi.dwBuildNumber >= 9200));

	ods("Lib: bIsWin8: %i", bIsWin8);


	hHookMutex = CreateMutex(NULL, false, "MumbleHookMutex");
	if (hHookMutex == NULL) {
		ods("Lib: CreateMutex failed");
		return;
	}

	if(!createSharedDataMap())
		return;

	if (! bMumble) {
		// Hook our own LoadLibrary functions so we notice when a new library (like the d3d ones) is loaded.
		hhLoad.setup(reinterpret_cast<voidFunc>(LoadLibraryA), reinterpret_cast<voidFunc>(MyLoadLibrary));
		hhLoadW.setup(reinterpret_cast<voidFunc>(LoadLibraryW), reinterpret_cast<voidFunc>(MyLoadLibraryW));
		hhFree.setup(reinterpret_cast<voidFunc>(FreeLibrary), reinterpret_cast<voidFunc>(MyFreeLibrary));

		checkHooks(true);
		ods("Lib: Injected into %s", procname);
	}
}

// Is the process black(listed)?
static bool dllmainProcAttachCheckProcessIsBlacklisted(char procname[], char *p) {
	DWORD buffsize = MAX_PATH * 20; // Initial buffer size for registry operation

	bool usewhitelist = false;
	HKEY key = NULL;

	char *buffer = new char[buffsize];

	// check if we're using a whitelist or a blacklist
	DWORD tmpsize = buffsize - 1;
	bool success = (RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Mumble\\Mumble\\overlay", NULL, KEY_READ, &key) == ERROR_SUCCESS) &&
	          (RegQueryValueExA(key, "usewhitelist", NULL, NULL, (LPBYTE)buffer, &tmpsize) == ERROR_SUCCESS);

	if (success) {
		buffer[tmpsize] = '\0';
		usewhitelist = (_stricmp(buffer, "true") == 0);
		// reset tmpsize to the buffers size (minus 1 char for str-termination), as it was changed by RegQuery
		tmpsize = buffsize - 1;

		// read the whitelist or blacklist (depending on which one we use)
		DWORD ret;
		while ((ret = RegQueryValueExA(key, usewhitelist ? "whitelist" : "blacklist", NULL, NULL, (LPBYTE)buffer, &tmpsize)) == ERROR_MORE_DATA) {
			// Increase the buffsize according to the required size RegQuery wrote into tmpsize, so we can read the whole value
			delete []buffer;
			buffsize = tmpsize + 1;
			buffer = new char[buffsize];
		}

		success = (ret == ERROR_SUCCESS);
	}

	if (key)
		RegCloseKey(key);

	if (success) {
		buffer[tmpsize] = '\0';
		unsigned int pos = 0;

		if (usewhitelist) {
			// check if process is whitelisted
			bool onwhitelist = false;
			while (pos < buffsize && buffer[pos] != 0) {
				if (_stricmp(procname, buffer + pos) == 0 || _stricmp(p+1, buffer + pos) == 0) {
					ods("Lib: Overlay enabled for whitelisted '%s'", buffer + pos);
					onwhitelist = true;
					break;
				}
				pos += strlen(buffer + pos) + 1;
			}

			if (!onwhitelist) {
				ods("Lib: No whitelist entry found for '%s', auto-blacklisted", procname);
				bBlackListed = TRUE;
				return true;
			}
		} else {
			// check if process is blacklisted
			while (pos < buffsize && buffer[pos] != 0) {
				if (_stricmp(procname, buffer + pos) == 0 || _stricmp(p+1, buffer + pos) == 0) {
					ods("Lib: Overlay blacklist entry found for '%s'", buffer + pos);
					bBlackListed = TRUE;
					return true;
				}
				pos += strlen(buffer + pos) + 1;
			}
		}
	} else {
		// If there is no list in the registry, fallback to using the default blacklist
		ods("Lib: Overlay fallback to default blacklist");
		int i = 0;
		while (overlayBlacklist[i]) {
			if (_stricmp(procname, overlayBlacklist[i]) == 0 || _stricmp(p+1, overlayBlacklist[i])==0) {
				ods("Lib: Overlay default blacklist entry found for '%s'", overlayBlacklist[i]);
				bBlackListed = TRUE;
				return true;
			}
			i++;
		}
	}

	// Make sure to always free/destroy buffer & heap
	delete []buffer;

	// if the processname is already found to be blacklisted, we can stop here
	if (bBlackListed)
		return true;

	// check if there is a "nooverlay" file in the executables folder, which would disable/blacklist the overlay
	// Same buffersize as procname; which we copy from.
	char fname[PROCNAMEFILEPATH_EXTENDED_BUFFER_BUFLEN];

	int pathlength = p - procname;
	p = fname + pathlength;
	strncpy_s(fname, sizeof(fname), procname, pathlength + 1);


	strcpy_s(p+1, PROCNAMEFILEPATH_EXTENDED_EXTLEN, "nooverlay");
	HANDLE h = CreateFile(fname, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (h != INVALID_HANDLE_VALUE) {
		CloseHandle(h);
		ods("Lib: Overlay disable %s found", fname);
		bBlackListed = TRUE;
		return true;
	}

	// check for "debugoverlay" file, which would enable overlay debugging
	strcpy_s(p+1, PROCNAMEFILEPATH_EXTENDED_EXTLEN, "debugoverlay");
	h = CreateFile(fname, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (h != INVALID_HANDLE_VALUE) {
		CloseHandle(h);
		ods("Lib: Overlay debug %s found", fname);
		bDebug = TRUE;
	}

	// check for blacklisting for loading WPF library
	checkForWPF();

	if (bBlackListed)
		return true;

	return false;
}

static bool createSharedDataMap() {
	DWORD dwSharedSize = sizeof(SharedData) + sizeof(Direct3D9Data) + sizeof(DXGIData) + sizeof(D3D10Data) + sizeof(D3D11Data);

	hMapObject = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, dwSharedSize, "MumbleOverlayPrivate");
	if (hMapObject == NULL) {
		ods("Lib: CreateFileMapping failed");
		return false;
	}

	//Note: If the mapping exists dwSharedSize value will be ignored and existing handle returned
	bool bInit = (GetLastError() != ERROR_ALREADY_EXISTS);

	unsigned char *rawSharedPointer = static_cast<unsigned char *>(
			MapViewOfFile(hMapObject, FILE_MAP_ALL_ACCESS, 0, 0, dwSharedSize));

	if (rawSharedPointer == NULL) {
		ods("Lib: MapViewOfFile failed");
		return false;
	}

	if (bInit)
		memset(rawSharedPointer, 0, dwSharedSize);

	sd = reinterpret_cast<SharedData *>(rawSharedPointer);
	rawSharedPointer += sizeof(SharedData);

	d3dd = reinterpret_cast<Direct3D9Data *>(rawSharedPointer);
	rawSharedPointer += sizeof(Direct3D9Data);

	dxgi = reinterpret_cast<DXGIData *>(rawSharedPointer);
	rawSharedPointer += sizeof(DXGIData);

	d3d10 = reinterpret_cast<D3D10Data *>(rawSharedPointer);
	rawSharedPointer += sizeof(D3D10Data);

	d3d11 = reinterpret_cast<D3D11Data *>(rawSharedPointer);
	rawSharedPointer += sizeof(D3D11Data);

	return true;
}

static void dllmainProcDetach() {

	hhLoad.restore(true);
	hhLoad.reset();
	hhLoadW.restore(true);
	hhLoadW.reset();
	hhFree.restore(true);
	hhFree.reset();

	if (sd)
		UnmapViewOfFile(sd);
	if (hMapObject)
		CloseHandle(hMapObject);
	if (hHookMutex)
		CloseHandle(hHookMutex);
}

static void dllmainThreadAttach() {
	static bool bTriedHook = false;
	if (!bBlackListed && sd && ! bTriedHook) {
		bTriedHook = true;
		checkForWPF();

		if (!bBlackListed) {
			ods("Lib: Checking for hooks, potentially injecting");
			checkHooks();
		}
	}
}

extern "C" BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID) {

	char procname[PROCNAMEFILEPATH_EXTENDED_BUFFER_BUFLEN];
	GetModuleFileNameA(NULL, procname, ARRAY_NUM_ELEMENTS(procname));
	// Fix for windows XP; on length nSize does not include null-termination
	// @see http://msdn.microsoft.com/en-us/library/windows/desktop/ms683197%28v=vs.85%29.aspx
	procname[ARRAY_NUM_ELEMENTS(procname) - 1] = '\0';

	switch (fdwReason) {
		case DLL_PROCESS_ATTACH:
			ods("Lib: ProcAttach: %s", procname);
			dllmainProcAttach(procname);
			break;
		case DLL_PROCESS_DETACH:
			ods("Lib: ProcDetach: %s", procname);
			dllmainProcDetach();
			break;
		case DLL_THREAD_ATTACH:
			ods("Lib: ThreadAttach: %s", procname);
			dllmainThreadAttach();
			break;
		default:
			break;
	}
	return TRUE;
}

bool IsFnInModule(voidFunc fnptr, wchar_t *refmodulepath, const std::string &logPrefix, const std::string &fnName) {

	HMODULE hModule = NULL;

	BOOL success = GetModuleHandleEx(
			GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
			reinterpret_cast<LPCSTR>(fnptr), &hModule);
	if (!success) {
		ods((logPrefix + ": Failed to get module for " + fnName).c_str());
	} else {
		wchar_t modulename[MODULEFILEPATH_BUFLEN];
		GetModuleFileNameW(hModule, modulename, ARRAY_NUM_ELEMENTS(modulename));
		return _wcsicmp(modulename, refmodulepath) == 0;
	}
	return false;
}

int GetFnOffsetInModule(voidFunc fnptr, wchar_t *refmodulepath, unsigned int refmodulepathLen, const std::string &logPrefix, const std::string &fnName) {

	HMODULE hModule = NULL;

	if (! GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (char *) fnptr, &hModule)) {
		ods((logPrefix + ": Failed to get module for " + fnName).c_str());
		return -1;
	}

	const bool bInit = refmodulepath[0] == '\0';
	if (bInit) {
		GetModuleFileNameW(hModule, refmodulepath, refmodulepathLen);
	} else {
		wchar_t modulename[MODULEFILEPATH_BUFLEN];
		GetModuleFileNameW(hModule, modulename, ARRAY_NUM_ELEMENTS(modulename));
		if (_wcsicmp(modulename, refmodulepath) != 0) {
			ods((logPrefix + ": " + fnName + " functions module path does not match previously found. Now: '%ls', Previously: '%ls'").c_str(), modulename, refmodulepath);
			return -2;
		}
	}

	unsigned char *fn = reinterpret_cast<unsigned char *>(fnptr);
	unsigned char *base = reinterpret_cast<unsigned char *>(hModule);
	return fn - base;
}
