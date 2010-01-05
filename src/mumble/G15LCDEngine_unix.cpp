/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>
   Copyright (C) 2008-2009, Mikkel Krautz <mikkel@krautz.dk>

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

#include <G15LCDEngine_unix.h>

static LCDEngine *G15LCDEngineNew() {
	return new G15LCDEngineUnix();
}

static LCDEngineRegistrar registrar(G15LCDEngineNew);

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

QList<LCDDevice *> G15LCDEngineUnix::devices() const {
	return qlDevices;
}

/* -- */

G15LCDDeviceUnix::G15LCDDeviceUnix(G15LCDEngineUnix *e) : LCDDevice() {
	bEnabled = false;
	engine = e;
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
	Q_ASSERT(img != NULL);

	const unsigned int len = 6880;
	uchar buf[len];
	uchar *tmp = img->bits();

	for (unsigned int i = 0; i < len / 8; ++i) {
		unsigned int idx = i*8;
		buf[idx+7] = tmp[i] & 0x80 ? 1 : 0;
		buf[idx+6] = tmp[i] & 0x40 ? 1 : 0;
		buf[idx+5] = tmp[i] & 0x20 ? 1 : 0;
		buf[idx+4] = tmp[i] & 0x10 ? 1 : 0;
		buf[idx+3] = tmp[i] & 0x08 ? 1 : 0;
		buf[idx+2] = tmp[i] & 0x04 ? 1 : 0;
		buf[idx+1] = tmp[i] & 0x02 ? 1 : 0;
		buf[idx+0] = tmp[i] & 0x01 ? 1 : 0;
	}

	int ret = g15_send(engine->sock, reinterpret_cast<char *>(buf), len);
	if (ret < 0)
		qWarning("G15LCDDeviceUnix: Unable to g15_send().");
}

QString G15LCDDeviceUnix::name() const {
	return QString::fromLatin1("Logitech Gamepanel (G15Daemon)");
}

QSize G15LCDDeviceUnix::size() const {
	return QSize(G15_WIDTH, G15_HEIGHT);
}
