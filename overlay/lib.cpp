// Copyright 2005-2017 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "lib.h"

#include "overlay_exe/overlay_exe.h"

#include "exclude.h"

#include <algorithm>
#include <string>
#include <vector>

#undef max // for std::numeric_limits<T>::max()

static HANDLE hMapObject = NULL;
static HANDLE hHookMutex = NULL;
static HHOOK hhookWnd = 0;

BOOL bIsWin8 = FALSE;

static BOOL bMumble = FALSE;
static BOOL bDebug = FALSE;
static BOOL bEnableOverlay = TRUE;

static HardHook hhLoad;
static HardHook hhLoadW;

static SharedData *sd = NULL;

static int iExcludeMode = 0;
static std::vector<std::string> vLaunchers;
static std::vector<std::string> vWhitelist;
static std::vector<std::string> vPaths;
static std::vector<std::string> vBlacklist;

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
	if (bEnableOverlay && (GetModuleHandleW(L"wpfgfx_v0300.dll") || GetModuleHandleW(L"wpfgfx_v0400.dll"))) {
		ods("Lib: Blacklisted for loading WPF library");
		bEnableOverlay = false;
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
					RECT r = {
						static_cast<LONG>(omMsg.omb.x),
						static_cast<LONG>(omMsg.omb.y),
						static_cast<LONG>(omMsg.omb.x + omMsg.omb.w),
						static_cast<LONG>(omMsg.omb.y + omMsg.omb.h)
					};

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
					if (a_ucTexture) {
						setRect();
						blit(0, 0, uiWidth, uiHeight);
					}
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

	if (bEnableOverlay) {
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
				ods("Lib: Installed overlay hook");
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
	HANDLE parent = reinterpret_cast<HANDLE>(udata);
	DWORD status = WaitForSingleObject(parent, INFINITE);
	if (status != WAIT_OBJECT_0) {
		ExitProcess(OVERLAY_HELPER_ERROR_DLL_PDEATH_WAIT_FAIL);
	}

	ExitProcess(0);
}

extern "C" __declspec(dllexport) int __cdecl OverlayHelperProcessMain(unsigned int magic, HANDLE parent) {
	int retval = 0;

	OutputDebugStringA("in main");

	if (GetOverlayMagicVersion() != magic) {
		return OVERLAY_HELPER_ERROR_DLL_MAGIC_MISMATCH;
	}

	HANDLE pcheckHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) OverlayHelperProcessParentDeathThread,
	                                   reinterpret_cast<void *>(parent), 0, NULL);
	if (pcheckHandle == 0) {
		return OVERLAY_HELPER_ERROR_DLL_PDEATH_THREAD_ERROR;
	}

	PrepareD3D9();
	PrepareDXGI();

	InstallHooks();

	while (1) {
		MSG msg;
		BOOL ret;

		ret = GetMessage(&msg, NULL, 0, 0);

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

static bool isParentProcessAllowed();
static void performEarlyExcludeChecks(const std::string &dir);
static void performExcludeChecks(const std::string &absExeName, const std::string &exeName);
static bool createSharedDataMap();

static bool dllmainProcAttach(char *procname) {
	Mutex::init();

	int mode = ExcludeGetMode();
	if (mode == -1) {
		ods("Lib: No setting for overlay exclusion mode. Using 'launcher' mode (0)");
		mode = 0;
	}
	iExcludeMode = mode;
	vLaunchers = ExcludeGetLaunchers();
	vWhitelist = ExcludeGetWhitelist();
	vPaths = ExcludeGetPaths();
	vBlacklist = ExcludeGetBlacklist();

	// Absolute path of the executable.
	std::string absExeName;
	// The directory the executable resides in.
	std::string dir;
	// The basename of the executable.
	std::string exeName;

	{
		char *p = strrchr(procname, '\\');

		absExeName = std::string(procname);
		dir = std::string(procname, p - procname);
		exeName = std::string(p);
	}

	if (GetProcAddress(NULL, "mumbleSelfDetection") != NULL) {
		ods("Lib: Attached to overlay helper or Mumble process. Blacklisted - no overlay injection.");
		bEnableOverlay = FALSE;
		bMumble = TRUE;
	} else {
		performEarlyExcludeChecks(dir);
		performExcludeChecks(absExeName, exeName);

		if (!bEnableOverlay) {
			return false;
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
		return false;
	}

	if(!createSharedDataMap())
		return false;

	if (! bMumble) {
		// Hook our own LoadLibrary functions so we notice when a new library (like the d3d ones) is loaded.
		hhLoad.setup(reinterpret_cast<voidFunc>(LoadLibraryA), reinterpret_cast<voidFunc>(MyLoadLibrary));
		hhLoadW.setup(reinterpret_cast<voidFunc>(LoadLibraryW), reinterpret_cast<voidFunc>(MyLoadLibraryW));

		checkHooks(true);
		ods("Lib: Injected into %s", procname);
	}

	return true;
}

static bool findParentProcessForChild(DWORD child, PROCESSENTRY32 *parent) {
	DWORD parentpid = 0;
	HANDLE hSnap = NULL;
	bool done = false;

	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(pe);

	// Find our parent's process ID.
	{
		BOOL ok = FALSE;

		hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (hSnap == INVALID_HANDLE_VALUE) {
			return false;
		}
		ok = Process32First(hSnap, &pe);
		while (ok) {
			if (pe.th32ProcessID == child) {
				parentpid = pe.th32ParentProcessID;
				break;
			}
			ok = Process32Next(hSnap, &pe);
		}
		CloseHandle(hSnap);
	}

	if (parentpid == 0) {
		return false;
	}

	// Find our parent's name.
	{
		BOOL ok = FALSE;

		hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (hSnap == INVALID_HANDLE_VALUE) {
			return false;
		}
		ok = Process32First(hSnap, &pe);
		while (ok) {
			if (pe.th32ProcessID == parentpid) {
				memcpy(parent, &pe, sizeof(pe));
				ok = FALSE;
				done = true;
				break;
			}
			ok = Process32Next(hSnap, &pe);
		}
		CloseHandle(hSnap);
	}

	return done;
}

static bool findParentProcess(PROCESSENTRY32 *parent) {
	DWORD ourpid = GetCurrentProcessId();
	return findParentProcessForChild(ourpid, parent);
}

static void performEarlyExcludeChecks(const std::string &dir) {
	// Check for "nooverlay" file.
	std::string nooverlay = dir + "\\nooverlay";
	HANDLE h = CreateFile(nooverlay.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (h != INVALID_HANDLE_VALUE) {
		CloseHandle(h);
		ods("Lib: Overlay disable %s found", nooverlay.c_str());
		bEnableOverlay = FALSE;
		return;
	}

	// check for "debugoverlay" file, which would enable overlay debugging
	std::string debugoverlay = dir + "\\debugoverlay";
	h = CreateFile(debugoverlay.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (h != INVALID_HANDLE_VALUE) {
		CloseHandle(h);
		ods("Lib: Overlay debug %s found", debugoverlay.c_str());
		bDebug = TRUE;
	}

	// check for blacklisting for loading WPF library
	checkForWPF();
}

template <typename T> static inline bool vecContains(const std::vector<T> &v, T val) {
	return std::find(v.begin(), v.end(), val) != v.end();
}

static bool isBlacklistedExe(const std::string &exeName) {
	return vecContains(vBlacklist, exeName);
}

static bool isWhitelistedExe(const std::string &exeName) {
	return vecContains(vWhitelist, exeName);
}

static bool isWhitelistedPath(const std::string &absExeName) {
	for (size_t i = 0; i < vPaths.size(); i++) {
		const std::string &path = vPaths.at(i);
		if (absExeName.find(path) == 0) {
			return true;
		}
	}
	return false;
}

static bool isWhitelistedParent(const std::string &parentExeName) {
	return vecContains(vLaunchers, parentExeName);
}

static void performExcludeChecks(const std::string &absExeName, const std::string &exeName) {
	if (iExcludeMode == 0) { // Launcher filter

		PROCESSENTRY32 parent;
		if (!findParentProcess(&parent)) {
			// Unable to find parent. Process is allowed.
			ods("Lib: Unable to find parent. Process allowed.");
			bEnableOverlay = true;
		}

		std::string parentExeName(parent.szExeFile);
		std::transform(parentExeName.begin(), parentExeName.end(), parentExeName.begin(), tolower);

		ods("Lib: Parent is '%s'", parentExeName.c_str());

		// The blacklist always wins.
		// If an exe is in the blacklist, never show the overlay, ever.
		if (isBlacklistedExe(exeName)) {
			ods("Lib: '%s' is blacklisted. Overlay disabled.", exeName.c_str());
			bEnableOverlay = false;
		   return;
		// If the process's exe name is whitelisted, allow the overlay to be shown.
		} else if (isWhitelistedExe(exeName)) {
			ods("Lib: '%s' is whitelisted. Overlay enabled.", exeName.c_str());
			bEnableOverlay = true;
		// If the exe is within a whitelisted path, allow the overlay to be shown.
		// An example is:
		// Whitelisted path: d:\games
		// absExeName: d:\games\World of Warcraft\Wow-64.exe
		// The overlay would be shown in WoW (and any game that lives under d:\games)
		 } else if (isWhitelistedPath(absExeName)) {
			ods("Lib: '%s' is within a whitelisted path. Overlay enabled.", absExeName.c_str());
		 	bEnableOverlay = true;
		// If the direct parent is whitelisted, allow the process through.
		// This allows us to whitelist Steam.exe, etc. -- and have the overlay
		// show up in all Steam titles.
		} else if (isWhitelistedParent(parentExeName)) {
			ods("Lib: Parent '%s' of '%s' is whitelisted. Overlay enabled.", parentExeName.c_str(), exeName.c_str());
			bEnableOverlay = true;
		// If nothing matched, do not show the overlay.
		} else {
			ods("Lib: No matching overlay exclusion rule found. Overlay disabled.");
			bEnableOverlay = false;
		}
	} else if (iExcludeMode == 1) {
		if (isWhitelistedExe(exeName)) {
			bEnableOverlay = true;
		} else {
			bEnableOverlay = false;
		}
	} else if (iExcludeMode == 2) { // Blacklist
		if (isBlacklistedExe(exeName)) {
			bEnableOverlay = false;
		} else {
			bEnableOverlay = true;
		}
	}
}

static bool createSharedDataMap() {
	DWORD dwSharedSize = sizeof(SharedData) + sizeof(Direct3D9Data) + sizeof(DXGIData) + sizeof(D3D10Data) + sizeof(D3D11Data);

#if defined(_M_IX86)
	const char *name = "MumbleOverlayPrivate-x86";
#elif defined(_M_X64)
	const char *name = "MumbleOverlayPrivate-x64";
#else
# error Unsupported architecture
#endif

	hMapObject = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, dwSharedSize, name);
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

	if (sd)
		UnmapViewOfFile(sd);
	if (hMapObject)
		CloseHandle(hMapObject);
	if (hHookMutex)
		CloseHandle(hHookMutex);
}

static void dllmainThreadAttach() {
	static bool bTriedHook = false;
	if (bEnableOverlay && sd && ! bTriedHook) {
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
	GetModuleFileNameA(NULL, procname, ARRAY_NUM_ELEMENTS(procname));
	// Fix for windows XP; on length nSize does not include null-termination
	// @see http://msdn.microsoft.com/en-us/library/windows/desktop/ms683197%28v=vs.85%29.aspx
	procname[ARRAY_NUM_ELEMENTS(procname) - 1] = '\0';

	switch (fdwReason) {
		case DLL_PROCESS_ATTACH: {
			ods("Lib: ProcAttach: %s", procname);
			bool shouldAttach = dllmainProcAttach(procname);
			if (!shouldAttach) {
				return FALSE;
			}
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
	unsigned long off = static_cast<unsigned long>(fn - base);

	// XXX: convert this function to use something other than int.
	// Issue mumble-voip/mumble#1924.
	if (off > static_cast<unsigned long>(std::numeric_limits<int>::max())) {
		ods("Internal overlay error: GetFnOffsetInModule() offset greater than return type can hold.");
		return -1;
	}

	return static_cast<int>(off);
}
