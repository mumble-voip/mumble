/* Copyright (C) 2011-2013, Benjamin Jemlich <pcgod@users.sourceforge.net>
   Copyright (C) 2013-2015 Mikkel Krautz <mikkel@krautz.dk>

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
#include <shlwapi.h>
#include <stdio.h>

#include <string>

#include "../overlay.h"

#define UNUSED(x) ((void)x)

typedef int (*OverlayHelperProcessMain)(unsigned int magic);

// Signal to the overlay DLL that it should not inject
// into this process.
extern "C" __declspec(dllexport) void mumbleSelfDetection() {};

// Alert shows a fatal error dialog and waits for the user to click OK.
static void Alert(LPCWSTR title, LPCWSTR msg) {
	MessageBox(NULL, msg, title, MB_OK|MB_ICONERROR);
}

// GetExecutableDirPath returns the directory that
// mumble.exe resides in.
static std::wstring GetExecutableDirPath() {
	wchar_t path[MAX_PATH];

	if (GetModuleFileNameW(NULL, path, MAX_PATH) == 0)
		return std::wstring();

	if (!PathRemoveFileSpecW(path))
		return std::wstring();

	std::wstring exe_path(path);
	return exe_path.append(L"\\");
}

// ConfigureEnvironment prepares mumble.exe's environment to
// run mumble_app.dll.
static bool ConfigureEnvironment() {
	std::wstring exe_path = GetExecutableDirPath();

	// Remove the current directory from the DLL search path.
	if (!SetDllDirectoryW(L""))
		return false;

	// Set mumble.exe's directory as the current working directory.
	if (!SetCurrentDirectoryW(exe_path.c_str()))
		return false;

	return true;
}

// GetAbsoluteMumbleOverlayDllPath returns the absolute path to
// mumble_ol.dll - the DLL containing the Mumble overlay code.
static std::wstring GetAbsoluteMumbleOverlayDllPath() {
	std::wstring exe_path = GetExecutableDirPath();
	if (exe_path.empty())
		return std::wstring();

	std::wstring abs_dll_path(exe_path);
	abs_dll_path.append(L"\\");
	abs_dll_path.append(L"mumble_ol.dll");
	return abs_dll_path;
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
	{
		std::wstring command_line(GetCommandLine());

		// The command line will contain two consecutive spaces
		// if the program was passed any arguments. If we don't
		// find them, it probably means that a user has double-clicked
		// the executable. Tell them to run 'mumble.exe' instead.
		size_t sep = command_line.find(std::wstring(L"  "));
		if (sep == std::string::npos) {
			Alert(L"Mumble Overlay", L"This program is not meant to be run by itself. Run 'mumble.exe' instead.");
			return -1;
		}

		// We expect that the Mumble process passes the overlay
		// magic number that it is built with to us.
		std::wstring magic_number_str = command_line.substr(sep);
		try {
			unsigned long passed_in_magic = std::stoul(magic_number_str);
			magic = static_cast<unsigned int>(passed_in_magic);
		} catch (std::exception &) {
			return -2;
		}
	}

	if (magic != OVERLAY_MAGIC_NUMBER) {
		return -3;
	}

	if (!ConfigureEnvironment()) {
		return -4;
	}

	std::wstring abs_dll_path = GetAbsoluteMumbleOverlayDllPath();
	if (abs_dll_path.empty()) {
		return -5;
	}

	HMODULE dll = LoadLibraryExW(abs_dll_path.c_str(), NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
	if (!dll) {
		return -6;
	}

	OverlayHelperProcessMain entry_point = reinterpret_cast<OverlayHelperProcessMain>(GetProcAddress(dll, "OverlayHelperProcessMain"));
	if (!entry_point) {
		return -7;
	}

	return entry_point(magic);
}

int APIENTRY wWinMain(HINSTANCE instance, HINSTANCE prevInstance, wchar_t *cmdArg, int cmdShow) {
	UNUSED(instance);
	UNUSED(prevInstance);
	UNUSED(cmdArg);
	UNUSED(cmdShow);

	return main(0, NULL);
}
