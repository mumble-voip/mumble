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

static HANDLE hPipe = INVALID_HANDLE_VALUE;
static lgLcdConnectContext conn;
static lgLcdDeviceDesc dev[G15_MAX_DEV];
static lgLcdOpenContext ctx[G15_MAX_DEV];
static lgLcdBitmap160x43x1 bitmap;
static FILE *fLogFile = NULL;
static int ndev = 0;

#define FMT "[%d/%m/%Y %X] "
#define PRINTF_ERR                                    \
	{                                                 \
		errno_t err;                                  \
		struct tm now;                                \
		__time64_t ltime;                             \
		char timestamp[32];                           \
		va_list args;                                 \
		_time64(&ltime);                              \
		err = _localtime64_s(&now, &ltime);           \
		ZeroMemory(timestamp, 32);                    \
		if (err == 0)                                 \
			strftime(timestamp, 31, FMT, &now);       \
		va_start(args, fmt);                          \
		fprintf_s(stderr, timestamp);                 \
		vfprintf(stderr, fmt, args);                  \
		fprintf_s(stderr, "\n");                      \
		fflush(stderr);                               \
		if (fLogFile) {                               \
			fprintf_s(fLogFile, timestamp);           \
			vfprintf(fLogFile, fmt, args);            \
			fprintf_s(fLogFile, "\n");                \
			fflush(fLogFile);                         \
		}                                             \
		va_end(args);                                 \
	}

void __cdecl warn(const char *fmt, ...) {
	PRINTF_ERR;
}

void __cdecl die(int err, const char *fmt, ...) {
	PRINTF_ERR;
	exit(err);
}

void setupLogFile(void) {
	wchar_t *buff = NULL;
	errno_t res;
	res = _wfopen_s(&fLogFile, L"G15HelperLog.txt", L"a+");
	if ((res != 0) || (! fLogFile)) {
		size_t reqSize, bSize;
		_wgetenv_s(&reqSize, NULL, 0, L"APPDATA");
		if (reqSize > 0) {
			reqSize += strlen(L"/Mumble/G15HelperLog.txt");
			bSize = reqSize;
			buff = _alloca(reqSize+1);
			wcscat_s(buff, bSize, L"/Mumble/G15HelperLog.txt");
			res = _wfopen_s(&fLogFile, buff, L"a+");
		}
	}
	warn("Opened log file.");
}

BOOL detectLCDManager(void) {
	DWORD dwErr = 0;
	BOOL bRet = TRUE;

	memset(&conn, 0, sizeof(lgLcdConnectContext));
	conn.appFriendlyName = G15_WIDGET_NAME;

	if (lgLcdInit() != ERROR_SUCCESS)
		bRet = FALSE;
	if (lgLcdConnect(&conn) != ERROR_SUCCESS)
		bRet = FALSE;
	if (lgLcdDeInit() != ERROR_SUCCESS)
		bRet = FALSE;

	return bRet;
}

int main(int argc, char *argv[]) {
	DWORD dwErr, dwMode;
	LPWSTR *cmdLine = NULL;
	BOOL bErr;
	int i;

	/*
	 * If passed the '/detect' parameter, figure out whether the Logitech
	 * LCD manager is running on the system.
	 */
	cmdLine = CommandLineToArgvW(GetCommandLineW(), &argc);
	if (argc > 1 && cmdLine != NULL) {
		if (!wcscmp(cmdLine[1], L"/detect")) {
			return (detectLCDManager() != TRUE);
		}
	}

#ifdef USE_LOGFILE
	setupLogFile();
#endif

	/*
	 * Set up IPC pipe.
	 */
	bErr = WaitNamedPipe(G15_PIPE_NAME, 2000);
	if (bErr == FALSE)
		die(G15_ERR_WAITPIPE, "WaitNamedPipe() failed.");

	hPipe = CreateFile(G15_PIPE_NAME,
	                   GENERIC_READ | FILE_WRITE_ATTRIBUTES,
	                   0, NULL, OPEN_EXISTING, 0, NULL);
	if (hPipe == INVALID_HANDLE_VALUE)
		die(G15_ERR_CREATEFILE, "CreateFile failed.");

	dwMode = PIPE_READMODE_MESSAGE | PIPE_WAIT;
	bErr = SetNamedPipeHandleState(hPipe, &dwMode, NULL, NULL);
	if (bErr == FALSE)
		die(G15_ERR_PIPESTATE, "SetNamedPipeHandleState failed.");

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
	conn.onConfigure.configCallback = NULL;
	conn.onConfigure.configContext = NULL;
	conn.connection = LGLCD_INVALID_CONNECTION;

	/*
	 * Initialize and connect.
	 */
	dwErr = lgLcdInit();
	if (dwErr != ERROR_SUCCESS)
		die(G15_ERR_INIT, "Unable to initialize Logitech LCD library. (Error: %i)", dwErr);

	dwErr = lgLcdConnect(&conn);
	if (dwErr != ERROR_SUCCESS)
		die(G15_ERR_CONNECT, "Unable to connect to Logitech LCD manager. (Error: %i)", dwErr);

	/*
	 * Enumerate devices.
	 */
	for (i = 0; i < G15_MAX_DEV; i++) {
		dwErr = lgLcdEnumerate(conn.connection, i, &dev[i]);
		if (dwErr != ERROR_SUCCESS)
			break;
		warn("Found device #%i. (%ix%i, %ibpp, %i buttons)", i, dev[i].Width, dev[i].Height, dev[i].Bpp, dev[i].NumSoftButtons);
		++ndev;
	}

	warn("Total devices in this session: %i.", ndev);

	/*
	 * Open devices.
	 */
	for (i = 0; i < ndev; i++) {
		ctx[i].connection = conn.connection;
		ctx[i].index = i;
		ctx[i].onSoftbuttonsChanged.softbuttonsChangedCallback = NULL;
		ctx[i].onSoftbuttonsChanged.softbuttonsChangedContext = NULL;
		ctx[i].device = LGLCD_INVALID_DEVICE;

		dwErr = lgLcdOpen(&ctx[0]);
		if (dwErr != ERROR_SUCCESS)
			die(G15_ERR_OPEN, "Unable to open device. (Error: %i)", dwErr);
	}

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

		dwErr = WaitForSingleObject(hPipe, 2000);
		if (dwErr != WAIT_OBJECT_0) {
			die(0, "Unable to wait for object.");
		}

		bErr = ReadFile(hPipe, bitmap.pixels, G15_MAX_FBMEM, &dwRead, NULL);
		if (bErr == FALSE || dwRead != G15_MAX_FBMEM)
			die(G15_ERR_READFILE, "Error while reading framebuffer.");

		for (i = 0; i < ndev; i++) {
			dwErr = lgLcdUpdateBitmap(ctx[i].device, (const lgLcdBitmapHeader *) &bitmap, LGLCD_SYNC_UPDATE(LGLCD_PRIORITY_NORMAL));
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
	CloseHandle(hPipe);

	warn("Terminated successfully.");

	return 0;
}

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	return main(0, NULL);
}
