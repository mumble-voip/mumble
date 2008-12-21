/* Copyright (C) 2008, Mikkel Krautz <mikkel@krautz.dk>

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

/*
 * G15 Helper Daemon for Win32.
 */

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
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
	int i;
	HANDLE hStdin, hStdout;
	DWORD dwLen;
	lgLcdConnectContextEx conn;
	lgLcdDeviceDescEx dev[G15_MAX_DEV];
	lgLcdOpenContext ctx[G15_MAX_DEV];
	lgLcdBitmap160x43x1 bitmap;
	int ndev = 0;

	warn("Args: %s", lpCmdLine);

	if (lpCmdLine && (strcmp(lpCmdLine, "/detect") == 0)) {
		warn("Detect mode!");
		bDetect = TRUE;
	} else if (! lpCmdLine || (strcmp(lpCmdLine, "/mumble") != 0)) {
		MessageBox(NULL, L"This program is run by Mumble, and should not be started separately.", L"Nothing to see here, move along", MB_OK | MB_ICONERROR);
		return 0;
	}

	hStdin = GetStdHandle(STD_INPUT_HANDLE);
    if (hStdin == INVALID_HANDLE_VALUE)
        die(G15_ERR_PIPESTATE, "Failed to get standard input");

	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdout == INVALID_HANDLE_VALUE)
        die(G15_ERR_PIPESTATE, "Failed to get standard output");

	/*
	 * Clear and set up initial structures.
	 */
	memset(&conn, 0, sizeof(lgLcdConnectContext));
	memset(&dev, 0, G15_MAX_DEV * sizeof(lgLcdDeviceDesc));
	memset(&ctx, 0, G15_MAX_DEV * sizeof(lgLcdOpenContext));
	memset(&bitmap, 0, sizeof(lgLcdBitmap160x43x1));

	conn.appFriendlyName = G15_WIDGET_NAME;
	conn.isAutostartable = FALSE;
	conn.isPersistent = FALSE;
	conn.dwAppletCapabilitiesSupported =LGLCD_APPLET_CAP_BASIC | LGLCD_APPLET_CAP_CAN_RUN_ON_MULTIPLE_DEVICES;
	conn.connection = LGLCD_INVALID_CONNECTION;

	/*
	 * Initialize and connect.
	 */
	dwErr = lgLcdInit();
	if (dwErr != ERROR_SUCCESS)
		die(G15_ERR_INIT, "Unable to initialize Logitech LCD library. (Error: %i)", dwErr);

	dwErr = lgLcdConnectEx(&conn);
	if (dwErr != ERROR_SUCCESS)
		die(G15_ERR_CONNECT, "Unable to connect to Logitech LCD manager. (Error: %i)", dwErr);

	/*
	 * Enumerate devices.
	 */
	for (i = 0; i < G15_MAX_DEV; i++) {
		dwErr = lgLcdEnumerateEx(conn.connection, i, &dev[i]);
		if (dwErr != ERROR_SUCCESS)
			break;
		warn("Found device #%i %ls (%02x). (%ix%i, %ibpp, %i buttons)", i, dev[i].deviceDisplayName, dev[i].deviceFamilyId, dev[i].Width, dev[i].Height, dev[i].Bpp, dev[i].NumSoftButtons);

		ctx[ndev].connection = conn.connection;
		ctx[ndev].index = i;
		ctx[ndev].onSoftbuttonsChanged.softbuttonsChangedCallback = NULL;
		ctx[ndev].onSoftbuttonsChanged.softbuttonsChangedContext = NULL;
		ctx[ndev].device = LGLCD_INVALID_DEVICE;

		if (bDetect) {
			wprintf(L"%ls\n", dev[i].deviceDisplayName);
			++ndev;
		} else {
			dwErr = lgLcdOpen(&ctx[ndev]);
			if (dwErr != ERROR_SUCCESS)
				warn("Unable to open device %d. (Error: %i)", i, dwErr);
			else {
				warn("Opened device %d",i);
				++ndev;
			}
		}
	}

	if (bDetect)
		return (ndev == 0);

	if (ndev == 0)
			die(G15_ERR_OPEN, "Unable to open devices");

	warn("Total devices in this session: %i.", ndev);

	/*
	 * Diplay buffer format.
	 */
	bitmap.hdr.Format = LGLCD_BMP_FORMAT_160x43x1;

	/*
	 * Submit an empty frame (to make our device show up in the G15
	 * LCD menu).
	 */
	for (i = 0; i < ndev; i++) {
		dwErr = lgLcdUpdateBitmap(ctx[i].device, (const lgLcdBitmapHeader *) &bitmap, LGLCD_SYNC_UPDATE(LGLCD_PRIORITY_NORMAL));
		if (dwErr != ERROR_SUCCESS)
			warn("Unable to submit empty frame to device #%i. (Error: %i)", i, dwErr);
	}

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
			warn("Read %d", dwRead);
			if (bErr == FALSE || dwRead == 0)
				die(G15_ERR_READFILE, "Error while reading framebuffer. %d %x",dwRead,GetLastError());
			dwTotRead += dwRead;
		} while (dwTotRead < G15_MAX_FBMEM);

		for (i = 0; i < ndev; i++) {
			dwErr = lgLcdUpdateBitmap(ctx[i].device, (const lgLcdBitmapHeader *) &bitmap, bPriority ? LGLCD_SYNC_UPDATE(LGLCD_PRIORITY_ALERT) : LGLCD_SYNC_UPDATE(LGLCD_PRIORITY_NORMAL));
			if (dwErr != ERROR_SUCCESS)
				warn("Unable to update bitmap for device #%i successfully. (Error: %i)", i, dwErr);
		}
	}

	/*
	 * Close device connections.
	 */
	for (i = 0; i < G15_MAX_DEV; i++) {
		dwErr = lgLcdClose(ctx[i].device);
		if (dwErr != ERROR_SUCCESS)
			die(G15_ERR_CLOSE, "Unable to close LCD device. (Error: %i)", dwErr);
	}

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

	/*
	 * Close pipe handle.
	 */

	warn("Terminated successfully.");

	return 0;
}
