// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "win.h"

#include <string>

#include <sddl.h>
#include <shlwapi.h>
#include <stdio.h>

// Alert shows a fatal error dialog and waits for the user to click OK.
static void Alert(LPCWSTR title, LPCWSTR msg) {
	MessageBox(nullptr, msg, title, MB_OK | MB_ICONERROR);
}

// GetExecutablePath returns the path to mumble.exe.
static const std::wstring GetExecutablePath() {
	wchar_t path[MAX_PATH];

	if (GetModuleFileNameW(nullptr, path, MAX_PATH) == 0)
		return std::wstring();

	std::wstring exe_path(path);
	return exe_path;
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
	DWORD sz = sizeof(buf) - 1 * sizeof(wchar_t);

	HKEY key = nullptr;
	bool success =
		(RegOpenKeyExW(HKEY_CURRENT_USER, L"Software\\Mumble\\Mumble\\shortcut\\windows\\uiaccess", 0, KEY_READ, &key)
		 == ERROR_SUCCESS)
		&& (RegQueryValueExW(key, L"enable", nullptr, nullptr, (LPBYTE) &buf, &sz) == ERROR_SUCCESS);
	if (success && _wcsicmp(buf, L"false") == 0) {
		return true;
	}

	return false;
}

// ProcessHasUIAccess returns true if the current process has UIAccess enabled.
static bool ProcessHasUIAccess() {
	HANDLE token = nullptr;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &token)) {
		return false;
	}

	DWORD ui_access      = 0;
	DWORD ui_access_size = sizeof(ui_access);
	if (!GetTokenInformation(token, TokenUIAccess, &ui_access, sizeof(ui_access), &ui_access_size)) {
		CloseHandle(token);
		return false;
	}

	CloseHandle(token);
	return ui_access != 0;
}

// RelaunchWithoutUIAccessIfNecessary relaunches the process
// without UIAccess, if necessary.
//
// If a relaunch of Mumble is necessary, the new process is
// spawned and the existing process is terminated. In that
// situation, this function will not return to the caller.
static bool RelaunchWithoutUIAccessIfNecessary() {
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
	const wchar_t *compat_layer_env = _wgetenv(L"__COMPAT_LAYER");
	if (!compat_layer_env) {
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

	if (!CreateProcessW(exe_path.c_str(), GetCommandLineW(), nullptr, nullptr, FALSE, 0, nullptr, nullptr,
						&startup_info, &process_info)) {
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
	if (prev_compat_layer_env) {
		if (wcscmp(prev_compat_layer_env, L"mumble_exe_none") == 0) {
			_wputenv_s(L"__COMPAT_LAYER", L"");
		} else {
			_wputenv_s(L"__COMPAT_LAYER", prev_compat_layer_env);
		}
	}
}

int os_early_init() {
	CleanupEnvironmentVariables();

	if (!RelaunchWithoutUIAccessIfNecessary()) {
		Alert(L"Mumble Early Init Error 50", L"Unable to complete RelaunchWithoutUIAccessIfNecessary");
		return 50;
	}

	return 0;
}
