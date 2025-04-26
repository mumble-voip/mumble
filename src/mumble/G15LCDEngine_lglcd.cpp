// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "G15LCDEngine_lglcd.h"

#define G15_MAX_DEV 5
#define G15_MAX_WIDTH 160
#define G15_MAX_HEIGHT 43
#define G15_MAX_BPP 1
#define G15_MAX_FBMEM (G15_MAX_WIDTH * G15_MAX_HEIGHT * G15_MAX_BPP)
#define G15_MAX_FBMEM_BITS (G15_MAX_FBMEM / 8)
#if defined(WIN32)
#	define G15_WIDGET_NAME L"Mumble G15 Display"
#elif defined(APPLE)
#	define G15_WIDGET_NAME CFSTR("Mumble G15 Display")
#endif

static LCDEngine *G15LCDEngineNew() {
	return new G15LCDEngineLGLCD();
}

static LCDEngineRegistrar registrarG15LCD_lglcd(G15LCDEngineNew);

G15LCDEngineLGLCD::G15LCDEngineLGLCD() : LCDEngine() {
	DWORD dwErr;

	ZeroMemory(&llcceConnect, sizeof(llcceConnect));
	ZeroMemory(&llcContext, sizeof(llcContext));

	llcceConnect.appFriendlyName               = G15_WIDGET_NAME;
	llcceConnect.isAutostartable               = FALSE;
	llcceConnect.isPersistent                  = FALSE;
	llcceConnect.dwAppletCapabilitiesSupported = LGLCD_APPLET_CAP_BASIC | LGLCD_APPLET_CAP_BW;
	llcceConnect.connection                    = LGLCD_INVALID_CONNECTION;

	llcContext.device = LGLCD_INVALID_DEVICE;

	dwErr = lgLcdInit();
	if (dwErr != ERROR_SUCCESS) {
		qWarning() << "LGLCD: Unable to initialize Logitech LCD library" << dwErr;
		return;
	}

	dwErr = lgLcdConnectEx(&llcceConnect);
	if (dwErr != ERROR_SUCCESS) {
		qWarning() << "LGLCD: Unable to connect to Logitech LCD manager" << dwErr;
		return;
	}


	qlDevices << new G15LCDDeviceLGLCD(this);

	QMetaObject::connectSlotsByName(this);
}

G15LCDEngineLGLCD::~G15LCDEngineLGLCD() {
	if (llcContext.device != LGLCD_INVALID_DEVICE) {
		lgLcdClose(llcContext.device);
		llcContext.device = LGLCD_INVALID_DEVICE;
	}
	if (llcceConnect.connection != LGLCD_INVALID_CONNECTION) {
		lgLcdDisconnect(llcceConnect.connection);
		llcceConnect.connection = LGLCD_INVALID_CONNECTION;
	}
	lgLcdDeInit();
}

QList< LCDDevice * > G15LCDEngineLGLCD::devices() const {
	return qlDevices;
}

/* -- */

G15LCDDeviceLGLCD::G15LCDDeviceLGLCD(G15LCDEngineLGLCD *e) : LCDDevice(), bEnabled(false) {
	engine = e;
}

G15LCDDeviceLGLCD::~G15LCDDeviceLGLCD() {
}

bool G15LCDDeviceLGLCD::enabled() {
	return bEnabled;
}

void G15LCDDeviceLGLCD::setEnabled(bool b) {
	bEnabled = b;

	if (bEnabled && (engine->llcContext.device == LGLCD_INVALID_DEVICE)) {
		ZeroMemory(&engine->llcContext, sizeof(engine->llcContext));
		engine->llcContext.connection = engine->llcceConnect.connection;
		engine->llcContext.device     = LGLCD_INVALID_DEVICE;
		engine->llcContext.deviceType = LGLCD_DEVICE_BW;

		DWORD dwErr = lgLcdOpenByType(&engine->llcContext);

	} else if (!bEnabled && (engine->llcContext.device != LGLCD_INVALID_DEVICE)) {
		lgLcdClose(engine->llcContext.device);
		engine->llcContext.device = LGLCD_INVALID_DEVICE;
	}
}

void G15LCDDeviceLGLCD::blitImage(QImage *img, bool alert) {
	Q_ASSERT(img);
	int len    = G15_MAX_FBMEM_BITS;
	uchar *tmp = img->bits();

	lgLcdBitmap160x43x1 bitmap;
	unsigned char *buf = bitmap.pixels;

	if (engine->llcContext.device == LGLCD_INVALID_DEVICE)
		return;

	if (!bEnabled)
		return;

	/*
	 * The amount of copying/conversion we're doing is hideous.
	 *
	 * To draw to the LCD display using Logitech's SDK, we need to pass
	 * it a byte array (in which each byte represents a single pixel on
	 * the LCD.)
	 *
	 * Unfortunately, there's no way out, really.  We *could* perhaps draw
	 * directly to a monochrome "bytemap" (via Format_Indexed8, and a mono-
	 * chrome colormap), but QPainter simply doesn't want to draw to a
	 * QImage of Format_Indexed8.
	 *
	 * (What's even worse is that the byte array passed to the Logitech SDK
	 * isn't even the native format of the LCD. It has to convert it once
	 * more, when it receives a frame.)
	 */
	for (int i = 0; i < len; i++) {
		int idx      = i * 8;
		buf[idx + 7] = tmp[i] & 0x80 ? 0xff : 0x00;
		buf[idx + 6] = tmp[i] & 0x40 ? 0xff : 0x00;
		buf[idx + 5] = tmp[i] & 0x20 ? 0xff : 0x00;
		buf[idx + 4] = tmp[i] & 0x10 ? 0xff : 0x00;
		buf[idx + 3] = tmp[i] & 0x08 ? 0xff : 0x00;
		buf[idx + 2] = tmp[i] & 0x04 ? 0xff : 0x00;
		buf[idx + 1] = tmp[i] & 0x02 ? 0xff : 0x00;
		buf[idx + 0] = tmp[i] & 0x01 ? 0xff : 0x00;
	}

	bitmap.hdr.Format = LGLCD_BMP_FORMAT_160x43x1;

	DWORD dwErr =
		lgLcdUpdateBitmap(engine->llcContext.device, &bitmap.hdr,
						  alert ? LGLCD_SYNC_UPDATE(LGLCD_PRIORITY_ALERT) : LGLCD_SYNC_UPDATE(LGLCD_PRIORITY_NORMAL));
}

QString G15LCDDeviceLGLCD::name() const {
	return QString::fromLatin1("Logitech Gamepanel");
}

QSize G15LCDDeviceLGLCD::size() const {
	return QSize(G15_MAX_WIDTH, G15_MAX_HEIGHT);
}

#undef G15_MAX_DEV
#undef G15_MAX_WIDTH
#undef G15_MAX_HEIGHT
#undef G15_MAX_BPP
#undef G15_MAX_FBMEM
#undef G15_MAX_FBMEM_BITS
#undef G15_WIDGET_NAME
