// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "lib.h"
#include "excludecheck.h"

#include "overlay_exe/overlay_exe.h"

static HANDLE hMapObject = nullptr;
static HANDLE hHookMutex = nullptr;
static HHOOK hhookWnd    = 0;

BOOL bIsWin8 = FALSE;

static BOOL bMumble        = FALSE;
static BOOL bDebug         = FALSE;
static BOOL bEnableOverlay = TRUE;

static HardHook hhLoad;
static HardHook hhLoadW;

static SharedData *sd = nullptr;

CRITICAL_SECTION Mutex::cs;

void Mutex::init() {
	InitializeCriticalSection(&cs);
}

Mutex::Mutex() {
	if (!TryEnterCriticalSection(&cs)) {
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
	if (bEnableOverlay && (GetModuleHandleW(L"wpfgfx_v0300.dll") || GetModuleHandleW(L"wpfgfx_v0400.dll"))) {
		ods("Lib: Blacklisted for loading WPF library");
		bEnableOverlay = FALSE;
	}
}

Pipe::Pipe() {
	hSocket     = INVALID_HANDLE_VALUE;
	hMemory     = nullptr;
	a_ucTexture = nullptr;

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
		hMemory = nullptr;
		if (a_ucTexture) {
			UnmapViewOfFile(a_ucTexture);
			a_ucTexture = nullptr;
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
	uiWidth           = 0;
	uiHeight          = 0;
	omMsg.omh.iLength = -1;
}

bool Pipe::sendMessage(const OverlayMsg &om) {
	DWORD dwBytesToWrite = sizeof(OverlayMsgHeader) + om.omh.iLength;
	DWORD dwBytesWritten = dwBytesToWrite;

	if (WriteFile(hSocket, om.headerbuffer, sizeof(OverlayMsgHeader) + om.omh.iLength, &dwBytesWritten, nullptr))
		if (dwBytesToWrite == dwBytesWritten)
			return true;

	ods("Pipe: Short write");
	disconnect();
	return false;
}

void Pipe::checkMessage(unsigned int width, unsigned int height) {
	if (!width || !height)
		return;

	if (hSocket == INVALID_HANDLE_VALUE) {
		hSocket = CreateFileW(L"\\\\.\\pipe\\MumbleOverlayPipe", GENERIC_READ | GENERIC_WRITE, 0, nullptr,
							  OPEN_EXISTING, 0, nullptr);
		if (hSocket == INVALID_HANDLE_VALUE) {
			ods("Pipe: Connection failed");
			return;
		}
		ods("Pipe: Connected");

		uiWidth  = 0;
		uiHeight = 0;

		// initially, instantiate and send an OverlayMessage with the current process id
		OverlayMsg om;
		om.omh.uiMagic = OVERLAY_MAGIC_NUMBER;
		om.omh.uiType  = OVERLAY_MSGTYPE_PID;
		om.omh.iLength = sizeof(OverlayMsgPid);
		om.omp.pid     = GetCurrentProcessId();

		if (!sendMessage(om))
			return;

		ods("Pipe: Process ID sent");
	}

	// if the passed width and height do not match the current overlays uiWidth and uiHeight, re-initialize
	if ((uiWidth != width) || (uiHeight != height)) {
		release();

		uiWidth  = width;
		uiHeight = height;

		// instantiate and send an initialization-OverlayMessage
		OverlayMsg om;
		om.omh.uiMagic  = OVERLAY_MAGIC_NUMBER;
		om.omh.uiType   = OVERLAY_MSGTYPE_INIT;
		om.omh.iLength  = sizeof(OverlayMsgInit);
		om.omi.uiWidth  = uiWidth;
		om.omi.uiHeight = uiHeight;

		if (!sendMessage(om))
			return;

		ods("Pipe: SentInitMsg with w h %d %d", uiWidth, uiHeight);
	}

	std::vector< RECT > blits;

	while (1) {
		DWORD dwBytesLeft;
		DWORD dwBytesRead;

		if (!PeekNamedPipe(hSocket, nullptr, 0, nullptr, &dwBytesLeft, nullptr)) {
			ods("Pipe: Could not peek");
			disconnect();
			return;
		}

		if (!dwBytesLeft)
			break;

		if (omMsg.omh.iLength == -1) {
			if (!ReadFile(hSocket, reinterpret_cast< unsigned char * >(omMsg.headerbuffer) + dwAlreadyRead,
						  sizeof(OverlayMsgHeader) - dwAlreadyRead, &dwBytesRead, nullptr)) {
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

			if (static_cast< int >(dwBytesLeft) < omMsg.omh.iLength)
				continue;
		}

		if (!ReadFile(hSocket, reinterpret_cast< unsigned char * >(omMsg.msgbuffer) + dwAlreadyRead,
					  omMsg.omh.iLength - dwAlreadyRead, &dwBytesRead, nullptr)) {
			ods("Pipe: Read data fail");
			disconnect();
			return;
		}

		dwAlreadyRead += dwBytesRead;

		if (static_cast< int >(dwBytesLeft) < omMsg.omh.iLength)
			continue;

		dwAlreadyRead = 0;

		switch (omMsg.omh.uiType) {
			case OVERLAY_MSGTYPE_SHMEM: {
				wchar_t memname[2048];
				memname[0] = 0;

				MultiByteToWideChar(CP_UTF8, 0, omMsg.oms.a_cName, omMsg.omh.iLength, memname, 2048);

				release();

				hMemory = CreateFileMappingW(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, 0, uiWidth * uiHeight * 4,
											 memname);

				if (GetLastError() != ERROR_ALREADY_EXISTS) {
					ods("Pipe: Memory %s(%d) => %ls doesn't exist", omMsg.oms.a_cName, omMsg.omh.iLength, memname);
					if (hMemory) {
						CloseHandle(hMemory);
						hMemory = nullptr;
						break;
					}
				}

				if (!hMemory) {
					ods("Pipe: CreateFileMapping failed");
					break;
				}

				a_ucTexture = reinterpret_cast< unsigned char * >(MapViewOfFile(hMemory, FILE_MAP_ALL_ACCESS, 0, 0, 0));

				if (!a_ucTexture) {
					ods("Pipe: Failed to map memory");
					CloseHandle(hMemory);
					hMemory = nullptr;
					break;
				}

				MEMORY_BASIC_INFORMATION mbi;
				memset(&mbi, 0, sizeof(mbi));
				if ((VirtualQuery(a_ucTexture, &mbi, sizeof(mbi)) == 0)
					|| (mbi.RegionSize < (uiHeight * uiWidth * 4))) {
					ods("Pipe: Memory too small");
					UnmapViewOfFile(a_ucTexture);
					CloseHandle(hMemory);
					a_ucTexture = nullptr;
					hMemory     = nullptr;
					break;
				}

				OverlayMsg om;
				om.omh.uiMagic = OVERLAY_MAGIC_NUMBER;
				om.omh.uiType  = OVERLAY_MSGTYPE_SHMEM;
				om.omh.iLength = 0;

				if (!sendMessage(om))
					return;

				newTexture(uiWidth, uiHeight);
			} break;
			case OVERLAY_MSGTYPE_BLIT: {
				RECT r = { static_cast< LONG >(omMsg.omb.x), static_cast< LONG >(omMsg.omb.y),
						   static_cast< LONG >(omMsg.omb.x + omMsg.omb.w),
						   static_cast< LONG >(omMsg.omb.y + omMsg.omb.h) };

				std::vector< RECT >::iterator i = blits.begin();
				while (i != blits.end()) {
					RECT is;
					if (::IntersectRect(&is, &r, &*i)) {
						::UnionRect(&is, &r, &*i);
						r = is;
						blits.erase(i);
						i = blits.begin();
					} else {
						++i;
					}
				}
				blits.push_back(r);
			} break;
			case OVERLAY_MSGTYPE_ACTIVE: {
				uiLeft   = omMsg.oma.x;
				uiTop    = omMsg.oma.y;
				uiRight  = omMsg.oma.x + omMsg.oma.w;
				uiBottom = omMsg.oma.y + omMsg.oma.h;
				if (a_ucTexture) {
					setRect();
					blit(0, 0, uiWidth, uiHeight);
				}
			} break;
			default:
				break;
		}
		omMsg.omh.iLength = -1;
	}

	if (!a_ucTexture)
		return;

	for (std::vector< RECT >::iterator i = blits.begin(); i != blits.end(); ++i)
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
	// TODO: Move logic to HardHook.
	// Call base without active hook in case of no trampoline.
	LoadLibraryAType oLoadLibrary = (LoadLibraryAType) hhLoad.call;
	hhLoad.restore();
	HMODULE h = oLoadLibrary(lpFileName);
	hhLoad.inject();

	ods("Lib: Library %s loaded to %p", lpFileName, h);

	if (bEnableOverlay) {
		checkHooks();
	}

	return h;
}

typedef HMODULE(__stdcall *LoadLibraryWType)(const wchar_t *);
static HMODULE WINAPI MyLoadLibraryW(const wchar_t *lpFileName) {
	// TODO: Move logic to HardHook.
	// Call base without active hook in case of no trampoline.
	LoadLibraryWType oLoadLibrary = (LoadLibraryWType) hhLoadW.call;
	hhLoadW.restore();
	HMODULE h = oLoadLibrary(lpFileName);
	hhLoadW.inject();

	ods("Lib: Library %ls wloaded to %p", lpFileName, h);

	checkForWPF();

	if (bEnableOverlay) {
		checkHooks();
	}

	return h;
}

static LRESULT CALLBACK CallWndProc(int nCode, WPARAM wParam, LPARAM lParam) {
	return CallNextHookEx(hhookWnd, nCode, wParam, lParam);
}

extern "C" __declspec(dllexport) void __cdecl RemoveHooks() {
	DWORD dwWaitResult = WaitForSingleObject(hHookMutex, 1000L);
	if (dwWaitResult == WAIT_OBJECT_0) {
		if (sd && sd->bHooked) {
			if (hhookWnd) {
				UnhookWindowsHookEx(hhookWnd);
				hhookWnd = nullptr;
			}
			sd->bHooked = false;
		}
		ReleaseMutex(hHookMutex);
	}
}

extern "C" __declspec(dllexport) void __cdecl InstallHooks() {
	DWORD dwWaitResult = WaitForSingleObject(hHookMutex, 1000L);
	if (dwWaitResult == WAIT_OBJECT_0) {
		if (sd && !sd->bHooked) {
			HMODULE hSelf = nullptr;
			GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
							  reinterpret_cast< LPCTSTR >(&InstallHooks), &hSelf);
			if (!hSelf) {
				ods("Lib: Failed to find myself");
			} else {
				hhookWnd = SetWindowsHookEx(WH_CBT, CallWndProc, hSelf, 0);
				if (!hhookWnd)
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

// Via d3d9.cpp
extern "C" __declspec(dllexport) void __cdecl PrepareD3D9();
// Via dxgi.cpp
extern "C" __declspec(dllexport) void __cdecl PrepareDXGI();

void __stdcall OverlayHelperProcessParentDeathThread(void *udata) {
	HANDLE parent = reinterpret_cast< HANDLE >(udata);
	DWORD status  = WaitForSingleObject(parent, INFINITE);
	if (status != WAIT_OBJECT_0) {
		ExitProcess(OVERLAY_HELPER_ERROR_DLL_PDEATH_WAIT_FAIL);
	}

	ExitProcess(0);
}

extern "C" __declspec(dllexport) int __cdecl OverlayHelperProcessMain(unsigned int magic, HANDLE parent) {
	int retval = 0;

	if (GetOverlayMagicVersion() != magic) {
		return OVERLAY_HELPER_ERROR_DLL_MAGIC_MISMATCH;
	}

	HANDLE pcheckHandle = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE) OverlayHelperProcessParentDeathThread,
									   reinterpret_cast< void * >(parent), 0, nullptr);
	if (pcheckHandle == 0) {
		return OVERLAY_HELPER_ERROR_DLL_PDEATH_THREAD_ERROR;
	}

	PrepareD3D9();
	PrepareDXGI();

	InstallHooks();

	while (1) {
		MSG msg;
		BOOL ret;

		ret = GetMessage(&msg, nullptr, 0, 0);

		// The ret variable is set to 0 on WM_QUIT,
		// and -1 on error.
		if (ret == 0) {
			retval = 0;
			break;
		} else if (ret == -1) {
			retval = -1001;
			break;
		}

		if (msg.message == WM_CLOSE) {
			retval = 0;
			break;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	RemoveHooks();

	return retval;
}

static bool createSharedDataMap();

// Check for the presence of a "nooverlay" file in the directory of the
// program we're injected into. If such a file exists, it's a hint that
// we shouldn't inject into it.
static void checkNoOverlayFile(const std::string &dir) {
	std::string nooverlay = dir + "\\nooverlay";
	HANDLE h              = CreateFile(nooverlay.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING,
                          FILE_ATTRIBUTE_NORMAL, nullptr);
	if (h != INVALID_HANDLE_VALUE) {
		CloseHandle(h);
		ods("Lib: Overlay disable %s found", dir.c_str());
		bEnableOverlay = FALSE;
		return;
	}
}

// Check for the presence of a "debugoverlay" file in the directory of
// the program we're injected into. If such a file exists, it's a hint
// from the user that they want verbose debugging output from the overlay.
static void checkDebugOverlayFile(const std::string &dir) {
	// check for "debugoverlay" file, which would enable overlay debugging
	std::string debugoverlay = dir + "\\debugoverlay";
	HANDLE h                 = CreateFile(debugoverlay.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING,
                          FILE_ATTRIBUTE_NORMAL, nullptr);
	if (h != INVALID_HANDLE_VALUE) {
		CloseHandle(h);
		ods("Lib: Overlay debug %s found", debugoverlay.c_str());
		bDebug = TRUE;
	}
}

// Given the absolute path to the current process's executable via |procname|,
// return the absolute path to the executable in |absExeName|, the directory
// that the executable lives in in |dir| and the basename of the executable in
// |exeName|.
//
// Returns true on success and fills out |absExeName|, |dir| and |exeName|.
// Returns false on failure, and does not touch |absExeName|, |dir| and |exeName|.
static bool parseProcName(char *procname, std::string &absExeName, std::string &dir, std::string &exeName) {
	if (!procname) {
		return false;
	}

	char *p = strrchr(procname, '\\');
	if (!p) {
		return false;
	}

	absExeName = std::string(procname);
	dir        = std::string(procname, p - procname);
	exeName    = std::string(p + 1);

	return true;
}

static bool dllmainProcAttach(char *procname) {
	Mutex::init();

	std::string absExeName;
	std::string dir;
	std::string exeName;
	bool ok = parseProcName(procname, absExeName, dir, exeName);

	if (!ok) {
		// No blacklisting if the file has no path
	} else if (GetProcAddress(nullptr, "mumbleSelfDetection")) {
		ods("Lib: Attached to overlay helper or Mumble process. Blacklisted - no overlay injection.");
		bEnableOverlay = FALSE;
		bMumble        = TRUE;
	} else {
		checkNoOverlayFile(dir);
		checkDebugOverlayFile(dir);

		if (bEnableOverlay) {
			bEnableOverlay = ExcludeCheckIsOverlayEnabled(absExeName, exeName);
		}

		if (!bEnableOverlay) {
			return false;
		}
	}

	OSVERSIONINFOEX ovi;
	memset(&ovi, 0, sizeof(ovi));
	ovi.dwOSVersionInfoSize = sizeof(ovi);
	GetVersionEx(reinterpret_cast< OSVERSIONINFO * >(&ovi));
	bIsWin8 = (ovi.dwMajorVersion >= 7) || ((ovi.dwMajorVersion == 6) && (ovi.dwBuildNumber >= 9200));

	ods("Lib: bIsWin8: %i", bIsWin8);


	hHookMutex = CreateMutex(nullptr, false, "MumbleHookMutex");
	if (!hHookMutex) {
		ods("Lib: CreateMutex failed");
		return false;
	}

	if (!createSharedDataMap())
		return false;

	if (!bMumble) {
		// Hook our own LoadLibrary functions so we notice when a new library (like the d3d ones) is loaded.
		hhLoad.setup(reinterpret_cast< voidFunc >(LoadLibraryA), reinterpret_cast< voidFunc >(MyLoadLibrary));
		hhLoadW.setup(reinterpret_cast< voidFunc >(LoadLibraryW), reinterpret_cast< voidFunc >(MyLoadLibraryW));

		checkHooks(true);
		ods("Lib: Injected into %s", procname);
	}

	return true;
}

static bool createSharedDataMap() {
	DWORD dwSharedSize =
		sizeof(SharedData) + sizeof(Direct3D9Data) + sizeof(DXGIData) + sizeof(D3D10Data) + sizeof(D3D11Data);

#if defined(_M_IX86)
	const char *name = "MumbleOverlayPrivate-x86";
#elif defined(_M_X64)
	const char *name = "MumbleOverlayPrivate-x64";
#else
#	error Unsupported architecture
#endif

	hMapObject = CreateFileMapping(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, 0, dwSharedSize, name);
	if (!hMapObject) {
		ods("Lib: CreateFileMapping failed");
		return false;
	}

	// Note: If the mapping exists dwSharedSize value will be ignored and existing handle returned
	bool bInit = (GetLastError() != ERROR_ALREADY_EXISTS);

	unsigned char *rawSharedPointer =
		static_cast< unsigned char * >(MapViewOfFile(hMapObject, FILE_MAP_ALL_ACCESS, 0, 0, dwSharedSize));

	if (!rawSharedPointer) {
		ods("Lib: MapViewOfFile failed");
		return false;
	}

	if (bInit)
		memset(rawSharedPointer, 0, dwSharedSize);

	sd = reinterpret_cast< SharedData * >(rawSharedPointer);
	rawSharedPointer += sizeof(SharedData);

	d3dd = reinterpret_cast< Direct3D9Data * >(rawSharedPointer);
	rawSharedPointer += sizeof(Direct3D9Data);

	dxgi = reinterpret_cast< DXGIData * >(rawSharedPointer);
	rawSharedPointer += sizeof(DXGIData);

	d3d10 = reinterpret_cast< D3D10Data * >(rawSharedPointer);
	rawSharedPointer += sizeof(D3D10Data);

	d3d11 = reinterpret_cast< D3D11Data * >(rawSharedPointer);
	rawSharedPointer += sizeof(D3D11Data);

	return true;
}

static void dllmainProcDetach() {
	hhLoad.restore(true);
	hhLoad.reset();
	hhLoadW.restore(true);
	hhLoadW.reset();

	if (sd)
		UnmapViewOfFile(sd);
	if (hMapObject)
		CloseHandle(hMapObject);
	if (hHookMutex)
		CloseHandle(hHookMutex);
}

static void dllmainThreadAttach() {
	static bool bTriedHook = false;
	if (bEnableOverlay && sd && !bTriedHook) {
		bTriedHook = true;
		checkForWPF();

		if (bEnableOverlay) {
			ods("Lib: Checking for hooks, potentially injecting");
			checkHooks();
		}
	}
}

extern "C" BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID) {
	char procname[PROCNAMEFILEPATH_EXTENDED_BUFFER_BUFLEN];
	GetModuleFileNameA(nullptr, procname, ARRAY_NUM_ELEMENTS(procname));
	// Fix for windows XP; on length nSize does not include null-termination
	// @see http://msdn.microsoft.com/en-us/library/windows/desktop/ms683197%28v=vs.85%29.aspx
	procname[ARRAY_NUM_ELEMENTS(procname) - 1] = '\0';

	switch (fdwReason) {
		case DLL_PROCESS_ATTACH: {
			ods("Lib: ProcAttach: %s", procname);
			dllmainProcAttach(procname);
			break;
		}
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
	HMODULE hModule = nullptr;

	BOOL success =
		GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
						  reinterpret_cast< LPCTSTR >(fnptr), &hModule);
	if (!success) {
		ods((logPrefix + ": Failed to get module for " + fnName).c_str());
	} else {
		wchar_t modulename[MODULEFILEPATH_BUFLEN];
		GetModuleFileNameW(hModule, modulename, ARRAY_NUM_ELEMENTS(modulename));
		return _wcsicmp(modulename, refmodulepath) == 0;
	}
	return false;
}

boost::optional< size_t > GetFnOffsetInModule(voidFunc fnptr, wchar_t *refmodulepath, unsigned int refmodulepathLen,
											  const std::string &logPrefix, const std::string &fnName) {
	HMODULE hModule = nullptr;

	if (!GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
						   reinterpret_cast< LPCTSTR >(fnptr), &hModule)) {
		ods((logPrefix + ": Failed to get module for " + fnName).c_str());
		return boost::none;
	}

	const bool bInit = refmodulepath[0] == '\0';
	if (bInit) {
		GetModuleFileNameW(hModule, refmodulepath, refmodulepathLen);
	} else {
		wchar_t modulename[MODULEFILEPATH_BUFLEN];
		GetModuleFileNameW(hModule, modulename, ARRAY_NUM_ELEMENTS(modulename));
		if (_wcsicmp(modulename, refmodulepath) != 0) {
			ods((logPrefix + ": " + fnName
				 + " functions module path does not match previously found. Now: '%ls', Previously: '%ls'")
					.c_str(),
				modulename, refmodulepath);
			return boost::none;
		}
	}

	size_t fn   = reinterpret_cast< size_t >(fnptr);
	size_t base = reinterpret_cast< size_t >(hModule);

	return fn - base;
}
