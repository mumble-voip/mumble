// Copyright 2005-2017 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include <windows.h>
#include <shlwapi.h>
#include <stdio.h>
#include <sddl.h>

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

// GetExecutablePath returns the path to mumble.exe.
static const std::wstring GetExecutablePath() {
	wchar_t path[MAX_PATH];

	if (GetModuleFileNameW(NULL, path, MAX_PATH) == 0)
		return std::wstring();

	std::wstring exe_path(path);
	return exe_path;
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

// UIAccessDisabledViaConfig queries the configuration store, and returns
// whether UIAccess has been disabled by the user via the
// "shortcut/windows/uiaccess/enable" config option.
static bool UIAccessDisabledViaConfig() {
	// RegQueryValueEx only zero-terminates if the registry value's type is
	// one of the Windows registry's string types. Because of this, we have
	// to use a buffer of 7 elements in order to query for the string "false".
	//
	// If the value's type is string, we'll end up with "false\0\0", because we
	// always zero-pad.
	//
	// But that's better than ending up in an infinite loop because we failed
	// to zero-terminate.
	wchar_t buf[7];
	memset(&buf, 0, sizeof(buf));
	DWORD sz = sizeof(buf) - 1*sizeof(wchar_t);

	HKEY key = NULL;
	bool success = (RegOpenKeyExW(HKEY_CURRENT_USER, L"Software\\Mumble\\Mumble\\shortcuts\\windows\\uiaccess", NULL, KEY_READ, &key) == ERROR_SUCCESS) &&
	               (RegQueryValueExW(key, L"enable" , NULL, NULL, (LPBYTE)&buf, &sz) == ERROR_SUCCESS);
	if (success && _wcsicmp(buf, L"false") == 0) {
		return true;
	}

	return false;
}

// SystemSupportsUIAccess returns true if the
// operating system supports UIAccess.
static bool SystemSupportsUIAccess() {
	OSVERSIONINFOEXW ovi;
	memset(&ovi, 0, sizeof(ovi));

	ovi.dwOSVersionInfoSize = sizeof(ovi);
	GetVersionEx(reinterpret_cast<OSVERSIONINFOW *>(&ovi));

	// Windows Vista and later support UIAccess.
	return ovi.dwMajorVersion > 5;
}

// ProcessHasUIAccess returns true if the current process has UIAccess enabled.
static bool ProcessHasUIAccess() {
	HANDLE token = NULL;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &token)) {
		return false;
	}

	DWORD ui_access = 0;
	DWORD ui_access_size = sizeof(ui_access);
	if (!GetTokenInformation(token, TokenUIAccess, &ui_access, sizeof(ui_access), &ui_access_size)) {
		CloseHandle(token);
		return false;
	}

	CloseHandle(token);
	return ui_access != 0;
}

// RelaunchWithoutUIAccessIfNecessary relaunches the process
// without UIAccess, if neccs.
static bool RelaunchWithoutUIAccessIfNecessary() {
	if (!SystemSupportsUIAccess()) {
		return true;
	}
	if (!ProcessHasUIAccess()) {
		return true;
	}
	if (!UIAccessDisabledViaConfig()) {
		return true;
	}

	STARTUPINFO startup_info;
	memset(&startup_info, 0, sizeof(startup_info));

	GetStartupInfo(&startup_info);

	PROCESS_INFORMATION process_info;
	memset(&process_info, 0, sizeof(process_info));

	// Store the original value of __COMPAT_LAYER so we can
	// restore it (which may be equivalent to not setting it all)
	// once we've been relaunched.
	wchar_t *compat_layer_env = _wgetenv(L"__COMPAT_LAYER");
	if (compat_layer_env == NULL) {
		compat_layer_env = L"mumble_exe_none";
	}
	_wputenv_s(L"mumble_exe_prev_compat_layer", compat_layer_env);

	// The only way we have found thus far to ignore the executable's
	// manifest when calling CreateProcess, is to use the __COMPAT_LAYER
	// environment variable.
	//
	// Setting this to RunAsInvoker seemingly ignores the application's
	// manifest. RunAsInvoker is the default execution level for Windows
	// programs.
	_wputenv_s(L"__COMPAT_LAYER", L"RunAsInvoker");

	std::wstring exe_path = GetExecutablePath();
	if (exe_path.empty()) {
		return false;
	}

	if (!CreateProcessW(exe_path.c_str(),
	              GetCommandLineW(),
	              NULL,
	              NULL,
	              FALSE,
	              0,
	              NULL,
	              NULL,
	              &startup_info,
	              &process_info)) {
		return false;
	}

	exit(0);
}

// CleanupEnvironmentVariables cleans up any
// unnecessary environment variables.
//
// For now, this function cleans up any environment
// variables set by RelaunchWithoutUIAccessIfNecessary.
static void CleanupEnvironmentVariables() {
	wchar_t *prev_compat_layer_env = _wgetenv(L"mumble_exe_prev_compat_layer");
	if (prev_compat_layer_env != NULL) {
		if (wcscmp(prev_compat_layer_env, L"mumble_exe_none") == 0) {
			_wputenv_s(L"__COMPAT_LAYER", L"");
		} else {
			_wputenv_s(L"__COMPAT_LAYER", prev_compat_layer_env);
		}
	}
	return true;
}

#ifdef DEBUG
int main(int argc, char **argv) {
	if (!CleanupEnvironmentVariables()) {
		Alert(L"Mumble Launcher Error -5", L"Unable to clean environment variables");
		return -5;
	}

	if (!RelaunchWithoutUIAccessIfNecessary()) {
		Alert(L"Mumble Launcher Error -6", L"Unable to complete RelaunchWithoutUIAccessIfNecessary");
		return -6;
	}

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
	if (!CleanupEnvironmentVariables()) {
		Alert(L"Mumble Launcher Error -5", L"Unable to clean environment variables");
		return -5;
	}

	if (!RelaunchWithoutUIAccessIfNecessary()) {
		Alert(L"Mumble Launcher Error -6", L"Unable to complete RelaunchWithoutUIAccessIfNecessary");
		return -6;
	}

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
