// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "win.h"

#include <shlwapi.h>
#include <stdio.h>

#include <string>

typedef int (*DLL_MAIN)(HINSTANCE, HINSTANCE, LPSTR, int);
#ifdef DEBUG
typedef int (*DLL_DEBUG_MAIN)(int, char **);
#endif

// Alert shows a fatal error dialog and waits for the user to click OK.
static void Alert(LPCWSTR title, LPCWSTR msg) {
	MessageBox(NULL, msg, title, MB_OK|MB_ICONERROR);
}

// Get the current Mumble version built into this executable.
// If no version is available, this function returns an empty
// string.
static const std::wstring GetMumbleVersion() {
#ifdef MUMBLE_VERSION
# define MUMXTEXT(X) L#X
# define MUMTEXT(X) MUMXTEXT(X)
	const std::wstring version(MUMTEXT(MUMBLE_VERSION));
	return version;
#else
	return std::wstring();
#endif
}

// GetExecutableDirPath returns the directory that
// mumble.exe resides in.
static const std::wstring GetExecutableDirPath() {
	wchar_t path[MAX_PATH];

	if (GetModuleFileNameW(NULL, path, MAX_PATH) == 0)
		return std::wstring();

	if (!PathRemoveFileSpecW(path))
		return std::wstring();

	std::wstring exe_path(path);
	return exe_path.append(L"\\");
}

// GetVersionedRootPath returns the versioned root path if
// Mumble is configured to work with versioned paths.
// If Mumble is not configured for versioned paths, this
// function returns an empty string.
static const std::wstring GetVersionedRootPath() {
	const std::wstring versionedRootPath = GetExecutableDirPath();
	if (versionedRootPath.empty()) {
		return std::wstring();
	}

	const std::wstring version = GetMumbleVersion();
	if (version.length() > 0) {
		return versionedRootPath + L"Versions\\" + version;
	}

	return std::wstring();
}

// ConfigureEnvironment prepares mumble.exe's environment to
// run mumble_app.dll.
static bool ConfigureEnvironment() {
	// Remove the current directory from the DLL search path.
	if (!SetDllDirectoryW(L""))
		return false;

	// Set the versioned root as the working directory if one is available.
	// If not, use the directory containing mumble.exe as the working directory.
	//
	// We use the versioned root as the working directory because of an odd
	// interaction between the UCRT's forward exports and LoadLibraryEx.
	// Most likely a bug in older Windows versions (Windows 10 is unaffected).
	//
	// In Mumble, mumble_app.dll is loaded via
	//
	//    LoadLibraryEx(..., ..., LOAD_WITH_ALTERED_SEARCH_PATH).
	//
	// This works on Windows 10, but is broken on Windows 7. On Windows 7, it
	// seems like the forward exports from api-win-ms*.dll to ucrtbase.dll cause
	// ucrtbase.dll to be loaded WITHOUT LOAD_WITH_ALTERED_SEARCH_PATH, but instead
	// using Standard Search Order For Desktop Applications.
	//
	// It looks for ucrtbase.dll in the following locations:
	//
	//  1. Next to the .exe
	//  2. 32-bit system directory
	//  3. 16-bit system directory
	//  4. Windows folder
	//  5. CWD
	//  6. %PATH% (seemingly)...
	//
	// But the application doesn't run, since it doesn't even try to load
	// ucrtbase.dll in the directory containing mumble_app.dll as it should,
	// because we've loaded mumble_app.dll with LOAD_WITH_ALTERED_SEARCH_PATH.
	//
	// Our workaround is to use the mumble_app.dll's directory as the working
	// directory. This causes the program to successfully load, even when
	// ucrtbase.dll is loaded using the Standard Search Order For Desktop.
	//
	// See https://github.com/mumble-voip/mumble/issues/2837 for more information.
	std::wstring cwd = GetVersionedRootPath();
	if (cwd.empty()) {
		cwd = GetExecutableDirPath();
	}
	if (!SetCurrentDirectoryW(cwd.c_str()))
		return false;

	return true;
}

// GetAbsoluteMumbleAppDllPath returns the absolute path to
// mumble_app.dll - the DLL containing the Mumble client
// application code.
static const std::wstring GetAbsoluteMumbleAppDllPath(std::wstring suggested_base_dir) {
	std::wstring base_dir = suggested_base_dir;

	if (base_dir.empty()) {
		base_dir = GetExecutableDirPath();
	}

	if (base_dir.empty()) {
		return std::wstring();
	}

	return base_dir + L"\\mumble_app.dll";
}

#ifdef DEBUG
int main(int argc, char **argv) {
	if (!ConfigureEnvironment()) {
		Alert(L"Mumble Launcher Error -1", L"Unable to configure environment.");
		return -1;
	}

	std::wstring versioned_root_path = GetVersionedRootPath();

	bool ok = false;
	if (!versioned_root_path.empty()) {
		if (PathFileExists(versioned_root_path.c_str())) {
			_wputenv_s(L"MUMBLE_VERSION_ROOT", versioned_root_path.c_str());
			ok = true;
		}
	}
	if (!ok) {
		_wputenv_s(L"MUMBLE_VERSION_ROOT", L"");
	}

	std::wstring abs_dll_path = GetAbsoluteMumbleAppDllPath(ok ? versioned_root_path : std::wstring());
	if (abs_dll_path.empty()) {
		Alert(L"Mumble Launcher Error -2", L"Unable to find the absolute path of mumble_app.dll.");
		return -2;
	}

	HMODULE dll = LoadLibraryExW(abs_dll_path.c_str(), NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
	if (!dll) {
		Alert(L"Mumble Launcher Error -3", L"Failed to load mumble_app.dll.");
		return -3;
	}

	DLL_DEBUG_MAIN entry_point = reinterpret_cast<DLL_DEBUG_MAIN>(GetProcAddress(dll, "main"));
	if (!entry_point) {
		Alert(L"Mumble Launcher Error -4", L"Unable to find expected entry point ('main') in mumble_app.dll.");
		return -4;
	}

	int rc = entry_point(argc, argv);

	return rc;
}
#endif  // DEBUG

int APIENTRY wWinMain(HINSTANCE instance, HINSTANCE prevInstance, wchar_t *cmdArg, int cmdShow) {
	if (!ConfigureEnvironment()) {
		Alert(L"Mumble Launcher Error -1", L"Unable to configure environment.");
		return -1;
	}

	std::wstring versioned_root_path = GetVersionedRootPath();

	bool ok = false;
	if (!versioned_root_path.empty()) {
		if (PathFileExists(versioned_root_path.c_str())) {
			_wputenv_s(L"MUMBLE_VERSION_ROOT", versioned_root_path.c_str());
			ok = true;
		}
	}
	if (!ok) {
		_wputenv_s(L"MUMBLE_VERSION_ROOT", L"");
	}

	std::wstring abs_dll_path = GetAbsoluteMumbleAppDllPath(ok ? versioned_root_path : std::wstring());
	if (abs_dll_path.empty()) {
		Alert(L"Mumble Launcher Error -2", L"Unable to find the absolute path of mumble_app.dll.");
		return -2;
	}

	HMODULE dll = LoadLibraryExW(abs_dll_path.c_str(), NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
	if (!dll) {
		Alert(L"Mumble Launcher Error -3", L"Failed to load mumble_app.dll.");
		return -3;
	}

	DLL_MAIN entry_point = reinterpret_cast<DLL_MAIN>(GetProcAddress(dll, "MumbleMain"));
	if (!entry_point) {
		Alert(L"Mumble Launcher Error -4", L"Unable to find expected entry point ('MumbleMain') in mumble_app.dll.");
		return -4;
	}

	(void) cmdArg;
	int rc = entry_point(instance, prevInstance, NULL, cmdShow);

	return rc;
}
