// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

/*
 * G15 Helper Daemon for Win32.
 */

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <wchar.h>

#include "g15helper.h"
#include "lglcd.h"

static void __cdecl ods(const char *fmt, va_list args) {
	char buffer[2048];
	_vsnprintf_s(buffer, 2048, _TRUNCATE, fmt, args);
	OutputDebugStringA(buffer);
}

static void __cdecl warn(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	ods(fmt, args);
	va_end(args);
}

static void __cdecl die(int err, const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	ods(fmt, args);
	va_end(args);
	exit(err);
}

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	DWORD dwErr;
	BOOL bErr;
	BOOL bDetect = FALSE;
	HANDLE hStdin, hStdout;
	lgLcdConnectContextEx conn;
	lgLcdOpenByTypeContext ctx;
	lgLcdBitmap160x43x1 bitmap;

	warn("Args: %s", lpCmdLine);

	if (lpCmdLine && (strcmp(lpCmdLine, "/detect") == 0)) {
		warn("Detect mode!");
		bDetect = TRUE;
	} else if (!lpCmdLine || (strcmp(lpCmdLine, "/mumble") != 0)) {
		MessageBox(NULL, L"This program is run by Mumble, and should not be started separately.",
				   L"Nothing to see here, move along", MB_OK | MB_ICONERROR);
		return 0;
	}

	hStdin = GetStdHandle(STD_INPUT_HANDLE);
	if (hStdin == INVALID_HANDLE_VALUE)
		die(G15_ERR_INIT, "Failed to get standard input");

	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdout == INVALID_HANDLE_VALUE)
		die(G15_ERR_INIT, "Failed to get standard output");

	/*
	 * Clear and set up initial structures.
	 */
	ZeroMemory(&conn, sizeof(conn));
	ZeroMemory(&ctx, sizeof(ctx));
	ZeroMemory(&bitmap, sizeof(bitmap));

	conn.appFriendlyName               = G15_WIDGET_NAME;
	conn.isAutostartable               = FALSE;
	conn.isPersistent                  = FALSE;
	conn.dwAppletCapabilitiesSupported = LGLCD_APPLET_CAP_BASIC | LGLCD_APPLET_CAP_BW;
	conn.connection                    = LGLCD_INVALID_CONNECTION;

	/*
	 * Initialize and connect.
	 */
	dwErr = lgLcdInit();
	if (dwErr != ERROR_SUCCESS)
		die(G15_ERR_INIT, "Unable to initialize Logitech LCD library. (Error: %i)", dwErr);

	dwErr = lgLcdConnectEx(&conn);
	if (dwErr != ERROR_SUCCESS)
		die(G15_ERR_CONNECT, "Unable to connect to Logitech LCD manager. (Error: %i)", dwErr);

	ctx.connection = conn.connection;
	ctx.device     = LGLCD_INVALID_DEVICE;
	ctx.deviceType = LGLCD_DEVICE_BW;

	dwErr = lgLcdOpenByType(&ctx);

	warn("That returned %d %d", dwErr, ERROR_SUCCESS);

	if (bDetect)
		return (dwErr != ERROR_SUCCESS);
	else if (dwErr != ERROR_SUCCESS)
		die(G15_ERR_OPEN, "Unable to open device. (Error: %i)", dwErr);

	/*
	 * Diplay buffer format.
	 */
	bitmap.hdr.Format = LGLCD_BMP_FORMAT_160x43x1;

	/*
	 * Main drawing loop.
	 */
	while (1) {
		DWORD dwRead;
		DWORD dwTotRead = 0;
		BYTE bPriority;

		dwErr = WaitForSingleObject(hStdin, 2000);
		if (dwErr != WAIT_OBJECT_0) {
			die(0, "Unable to wait for object.");
		}

		bErr = ReadFile(hStdin, &bPriority, 1, &dwRead, NULL);
		if ((bErr == FALSE) || (dwRead != 1))
			die(G15_ERR_READFILE, "Error while reading priority.");

		do {
			dwErr = WaitForSingleObject(hStdin, 2000);
			if (dwErr != WAIT_OBJECT_0) {
				die(0, "Unable to wait for object.");
			}
			bErr = ReadFile(hStdin, bitmap.pixels + dwTotRead, G15_MAX_FBMEM - dwTotRead, &dwRead, NULL);
			if (bErr == FALSE || dwRead == 0)
				die(G15_ERR_READFILE, "Error while reading framebuffer. %d %x", dwRead, GetLastError());
			dwTotRead += dwRead;
		} while (dwTotRead < G15_MAX_FBMEM);

		dwErr = lgLcdUpdateBitmap(ctx.device, (const lgLcdBitmapHeader *) &bitmap,
								  bPriority ? LGLCD_SYNC_UPDATE(LGLCD_PRIORITY_ALERT)
											: LGLCD_SYNC_UPDATE(LGLCD_PRIORITY_NORMAL));
		if (dwErr != ERROR_SUCCESS)
			warn("Unable to update bitmap for device. (Error: %i)", dwErr);
	}

	/*
	 * Close device connections.
	 */
	dwErr = lgLcdClose(ctx.device);
	if (dwErr != ERROR_SUCCESS)
		die(G15_ERR_CLOSE, "Unable to close LCD device. (Error: %i)", dwErr);

	/*
	 * Disconnect from LCD monitor.
	 */
	dwErr = lgLcdDisconnect(conn.connection);
	if (dwErr != ERROR_SUCCESS)
		die(G15_ERR_DISCONNECT, "Unable to disconnect from LCD manager. (Error: %i)", dwErr);

	/*
	 * Deinitialize G15 library.
	 */
	dwErr = lgLcdDeInit();
	if (dwErr != ERROR_SUCCESS)
		die(G15_ERR_DEINIT, "Unable to deinitialize LCD library. (Error: %i)", dwErr);

	warn("Terminated successfully.");

	return 0;
}
