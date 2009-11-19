/* Copyright (C) 2008-2009, Mikkel Krautz <mikkel@krautz.dk>

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
 * G15 Helper Daemon for Mac OS X.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <CoreFoundation/CoreFoundation.h>

#include "g15helper.h"
#include "lglcd.h"

#define ERROR_SUCCESS 0
#define BOOL unsigned char
#define BYTE unsigned char
#define TRUE 1
#define FALSE 0

static void ods(const char *fmt, va_list args) {
	vfprintf(stderr, fmt, args);
}

static void warn(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	ods(fmt, args);
	va_end(args);
}

static void die(int err, const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	ods(fmt, args);
	va_end(args);
	exit(err);
}

int main(int argc, char *argv[]) {
	int dwErr;
	BOOL bDetect = FALSE;
	int i;
	lgLcdConnectContextEx conn;
	lgLcdOpenByTypeContext ctx;
	lgLcdBitmap160x43x1 bitmap;

	if (argc > 1 && (strcmp(argv[1], "/detect") == 0)) {
		warn("Detect mode!");
		bDetect = TRUE;
	} else if (!(argc > 1) || (strcmp(argv[1], "/mumble") != 0)) {
		CFUserNotificationDisplayAlert(0, 0, NULL,  NULL, NULL, CFSTR("Nothing to see here"), CFSTR("This program is run by Mumble, and should not be started separately."), CFSTR("OK"), NULL, NULL, NULL);
		return 0;
	}

	/*
	 * Clear and set up initial structures.
	 */
	memset(&conn, 0, sizeof(conn));
	memset(&ctx, 0, sizeof(ctx));
	memset(&bitmap, 0, sizeof(bitmap));

	conn.appFriendlyName = G15_WIDGET_NAME;
	conn.isAutostartable = FALSE;
	conn.isPersistent = FALSE;
	conn.dwAppletCapabilitiesSupported =LGLCD_APPLET_CAP_BASIC | LGLCD_APPLET_CAP_BW;
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

	ctx.connection = conn.connection;
	ctx.device = LGLCD_INVALID_DEVICE;
	ctx.deviceType =LGLCD_DEVICE_BW;

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
		int ret;
		int remain = 0;
		BYTE bPriority;

		ret = read(0, &bPriority, 1);
		if (ret == -1 || ret != 1)
			die(G15_ERR_READFILE, "Error while reading priority.");

		do {
			ret = read(0, bitmap.pixels + remain, G15_MAX_FBMEM - remain);
			if (ret < 1)
				die(G15_ERR_READFILE, "Error while reading framebuffer. %d (%s)", ret, strerror(errno));
			remain += ret;
		} while (remain < G15_MAX_FBMEM);

		dwErr = lgLcdUpdateBitmap(ctx.device, (const lgLcdBitmapHeader *) &bitmap, bPriority ? LGLCD_SYNC_UPDATE(LGLCD_PRIORITY_ALERT) : LGLCD_SYNC_UPDATE(LGLCD_PRIORITY_NORMAL));
		if (dwErr != ERROR_SUCCESS)
			warn("Unable to update bitmap for device #%i successfully. (Error: %i)", i, dwErr);
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
