// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include <windows.h>
#include <shellapi.h>
#include <shlwapi.h>
#include <stdio.h>

#include <string>
#include <vector>

#include "../overlay.h"
#include "overlay_exe.h"

#define UNUSED(x) ((void) x)

typedef int (*OverlayHelperProcessMain)(unsigned int magic, HANDLE parent);

// Signal to the overlay DLL that it should not inject
// into this process.
extern "C" __declspec(dllexport) void mumbleSelfDetection(){};

// Alert shows a fatal error dialog and waits for the user to click OK.
static void Alert(LPCWSTR title, LPCWSTR msg) {
	MessageBox(nullptr, msg, title, MB_OK | MB_ICONERROR);
}

// GetExecutableDirPath returns the directory that
// mumble_ol_helper.exe resides in.
static std::wstring GetExecutableDirPath() {
	wchar_t path[MAX_PATH];

	if (GetModuleFileNameW(nullptr, path, MAX_PATH) == 0)
		return std::wstring();

	if (!PathRemoveFileSpecW(path))
		return std::wstring();

	std::wstring exePath(path);
	return exePath.append(L"\\");
}

// ConfigureEnvironment prepares mumble_ol_helper.exe's environment to
// run mumble_ol.dll's OverlayHelperProcessMain() function.
static bool ConfigureEnvironment() {
	std::wstring exePath = GetExecutableDirPath();

	// Remove the current directory from the DLL search path.
	if (!SetDllDirectoryW(L""))
		return false;

	// Set mumble_ol_helper.exe's directory as the current working directory.
	if (!SetCurrentDirectoryW(exePath.c_str()))
		return false;

	return true;
}

// GetAbsoluteMumbleOverlayDllPath returns the absolute path to
// mumble_ol.dll - the DLL containing the Mumble overlay code.
static std::wstring GetAbsoluteMumbleOverlayDllPath() {
	std::wstring exePath = GetExecutableDirPath();
	if (exePath.empty())
		return std::wstring();

	std::wstring absDLLPath(exePath);
	absDLLPath.append(L"\\");
	absDLLPath.append(MUMBLE_OVERLAY_DLL_NAME);
	return absDLLPath;
}

// GetCommandLineArgs returns the command line arguments
// passed to the process.
// If the returned vector has a length of 0, an unknown
// error occurred.
static std::vector< std::wstring > GetCommandLineArgs() {
	std::vector< std::wstring > args;

	LPWSTR cmdLine = GetCommandLine();
	if (!cmdLine) {
		return args;
	}

	int argc     = 0;
	LPWSTR *argv = CommandLineToArgvW(cmdLine, &argc);
	if (!argv) {
		return args;
	}

	for (int i = 0; i < argc; i++) {
		args.push_back(std::wstring(argv[i]));
	}

	return args;
}

int main(int argc, char **argv) {
	UNUSED(argc);
	UNUSED(argv);

	// Mumble passes the OVERLAY_MAGIC_NUMBER it was built
	// with the first argument to this helper process.
	//
	// This is done for two reasons.
	//
	// The first reason is that we need to ensure
	// compatibility between Mumble, the overlay helper,
	// and the overlay itself.
	//
	// The second reason is that it is an easy way to catch
	// users accidentally double-clicking on the overlay
	// helper's EXE file. We can detect these non-Mumble
	// initiated launches by checking if we were passed any
	// arguments at all. If no parameters are passed, we
	// display a nice alert dialog directing users to
	// 'mumble.exe' instead.
	unsigned int magic = 0;
	HANDLE parent      = 0;
	{
		std::vector< std::wstring > args = GetCommandLineArgs();

		// If there is only a single argument, it's the program name.
		// That probably means that a user has double-clicked
		// the executable. Tell them to run 'mumble.exe' instead.
		//
		// This also handles the case where GetCommandLineArgs returns
		// an empty vector (the error case).
		if (args.size() <= 1) {
			Alert(L"Mumble Overlay", L"This program is not meant to be run by itself. Run 'mumble.exe' instead.");
			return OVERLAY_HELPER_ERROR_EXE_NO_ARGUMENTS;
		}

		// The Mumble process passes the overlay magic number,
		// and a HANDLE in numeric form as its only two arguments.
		// We must have both of them to continue.
		if (args.size() < 3) {
			return OVERLAY_HELPER_ERROR_TOO_FEW_ARGUMENTS;
		}

		std::wstring magicNumberStr = args[1];
		std::wstring handleStr      = args[2];

		try {
			unsigned long passedInMagic = std::stoul(magicNumberStr);
			magic                       = static_cast< unsigned int >(passedInMagic);
		} catch (std::exception &) {
			return OVERLAY_HELPER_ERROR_EXE_INVALID_MAGIC_ARGUMENT;
		}

		try {
			unsigned long long passedInHandle = std::stoull(handleStr);
			parent                            = reinterpret_cast< HANDLE >(passedInHandle & 0xFFFFFFFFULL);
		} catch (std::exception &) {
			return OVERLAY_HELPER_ERROR_EXE_INVALID_HANDLE_ARGUMENT;
		}
	}

	if (magic != OVERLAY_MAGIC_NUMBER) {
		return OVERLAY_HELPER_ERROR_EXE_MAGIC_MISMATCH;
	}

	if (!ConfigureEnvironment()) {
		return OVERLAY_HELPER_ERROR_EXE_CONFIGURE_ENVIRONMENT;
	}

	std::wstring absDLLPath = GetAbsoluteMumbleOverlayDllPath();
	if (absDLLPath.empty()) {
		return OVERLAY_HELPER_ERROR_EXE_GET_DLL_PATH;
	}

	HMODULE dll = LoadLibraryExW(absDLLPath.c_str(), nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);
	if (!dll) {
		return OVERLAY_HELPER_ERROR_EXE_LOAD_DLL;
	}

	OverlayHelperProcessMain entryPoint =
		reinterpret_cast< OverlayHelperProcessMain >(GetProcAddress(dll, "OverlayHelperProcessMain"));
	if (!entryPoint) {
		return OVERLAY_HELPER_ERROR_EXE_LOOKUP_ENTRY_POINT;
	}

	return entryPoint(magic, parent);
}

int APIENTRY wWinMain(HINSTANCE instance, HINSTANCE prevInstance, wchar_t *cmdArg, int cmdShow) {
	UNUSED(instance);
	UNUSED(prevInstance);
	UNUSED(cmdArg);
	UNUSED(cmdShow);

	return main(0, nullptr);
}
