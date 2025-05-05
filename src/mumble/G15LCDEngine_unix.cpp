// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include <G15LCDEngine_unix.h>

static LCDEngine *G15LCDEngineNew() {
	return new G15LCDEngineUnix();
}

static LCDEngineRegistrar registrarLCDEngine_unix(G15LCDEngineNew);

G15LCDEngineUnix::G15LCDEngineUnix() {
	sock = new_g15_screen(G15_PIXELBUF);
	if (sock < 0) {
		qWarning("G15LCDEngineUnix: Unable to connect to G15Daemon.");
		return;
	}

	qlDevices << new G15LCDDeviceUnix(this);
}

G15LCDEngineUnix::~G15LCDEngineUnix() {
}

QList< LCDDevice * > G15LCDEngineUnix::devices() const {
	return qlDevices;
}

/* -- */

G15LCDDeviceUnix::G15LCDDeviceUnix(G15LCDEngineUnix *e) : LCDDevice() {
	bEnabled = false;
	engine   = e;
}

G15LCDDeviceUnix::~G15LCDDeviceUnix() {
}

bool G15LCDDeviceUnix::enabled() {
	return bEnabled;
}

void G15LCDDeviceUnix::setEnabled(bool b) {
	bEnabled = b;
}

void G15LCDDeviceUnix::blitImage(QImage *img, bool) {
	Q_ASSERT(img);

	const unsigned int len = 6880;
	uchar buf[len];
	uchar *tmp = img->bits();

	for (unsigned int i = 0; i < len / 8; ++i) {
		unsigned int idx = i * 8;
		buf[idx + 7]     = tmp[i] & 0x80 ? 1 : 0;
		buf[idx + 6]     = tmp[i] & 0x40 ? 1 : 0;
		buf[idx + 5]     = tmp[i] & 0x20 ? 1 : 0;
		buf[idx + 4]     = tmp[i] & 0x10 ? 1 : 0;
		buf[idx + 3]     = tmp[i] & 0x08 ? 1 : 0;
		buf[idx + 2]     = tmp[i] & 0x04 ? 1 : 0;
		buf[idx + 1]     = tmp[i] & 0x02 ? 1 : 0;
		buf[idx + 0]     = tmp[i] & 0x01 ? 1 : 0;
	}

	int ret = g15_send(engine->sock, reinterpret_cast< char * >(buf), len);
	if (ret < 0)
		qWarning("G15LCDDeviceUnix: Unable to g15_send().");
}

QString G15LCDDeviceUnix::name() const {
	return QString::fromLatin1("Logitech Gamepanel (G15Daemon)");
}

QSize G15LCDDeviceUnix::size() const {
	return QSize(G15_WIDTH, G15_HEIGHT);
}
