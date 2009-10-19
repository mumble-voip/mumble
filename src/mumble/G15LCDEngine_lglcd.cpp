/* Copyright (C) 2005-2009, Thorvald Natvig <thorvald@natvig.com>
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

#include "G15LCDEngine_lglcd.h"

static LCDEngine *G15LCDEngineNew() {
	return new G15LCDEngineLGLCD();
}

#ifdef Q_OS_WIN32
#define G15_LGLCD_EXEC_SUFFIX ".exe"
#else
#define G15_LGLCD_EXEC_SUFFIX ""
#endif

static LCDEngineRegistrar registrar(G15LCDEngineNew);

G15LCDEngineLGLCD::G15LCDEngineLGLCD() : LCDEngine() {
	bRunning = false;
	bUnavailable = false;

	qsHelperExecutable = QString::fromLatin1("\"%1/mumble-g15-helper%2\"").arg(qApp->applicationDirPath()).arg(QLatin1String(G15_LGLCD_EXEC_SUFFIX));

	qpHelper = new QProcess(this);
	qpHelper->setObjectName(QLatin1String("Helper"));
	qpHelper->setWorkingDirectory(qApp->applicationDirPath());

	/*
	 * Call our helper to detect whether any Logitech Gamepanel devices
	 * are available on the system.
	 */
	qpHelper->start(qsHelperExecutable, QStringList(QLatin1String("/detect")));
	qpHelper->waitForFinished();
	if (qpHelper->exitCode() != 0) {
		qWarning("G15LCDEngine_lglcd: Logitech LCD Manager not detected.");
		return;
	}

	qlDevices << new G15LCDDeviceLGLCD(this);

	QMetaObject::connectSlotsByName(this);
}

G15LCDEngineLGLCD::~G15LCDEngineLGLCD() {
	setProcessStatus(false);
}

QList<LCDDevice *> G15LCDEngineLGLCD::devices() const {
	return qlDevices;
}

void G15LCDEngineLGLCD::setProcessStatus(bool run) {
	if (bUnavailable)
		return;

	if (run && !bRunning) {
		bRunning = true;
		qpHelper->start(qsHelperExecutable, QStringList(QLatin1String("/mumble")));
		if (! qpHelper->waitForStarted(2000)) {
			qWarning("G15LCDEngine_lglcd: Unable to launch G15 helper.");
			bRunning = false;
			return;
		}
	} else if (!run && bRunning) {
		bRunning = false;
		qpHelper->kill();
		qpHelper->waitForFinished();
	}
}

void G15LCDEngineLGLCD::on_Helper_finished(int exitCode, QProcess::ExitStatus status) {
	/* Skip the signal if we killed ourselves. */
	if (! bRunning)
		return;

	if (status == QProcess::CrashExit) {
		qWarning("G15LCDEngine_lglcd: Helper process crashed. Restarting.");
		qpHelper->start(qsHelperExecutable, QStringList(QLatin1String("/mumble")));
	} else if (status == QProcess::NormalExit && exitCode != 0) {
		qWarning("G15LCDEngine_lglcd: Helper process exited. Exit code was: `%i'. Not attempting recovery.", exitCode);
		bUnavailable = true;
	}
}

bool G15LCDEngineLGLCD::framebufferReady() const {
	return !bUnavailable;
}

void G15LCDEngineLGLCD::submitFrame(bool alert, unsigned char *buf, size_t len) {
	char pri = alert ? 1 : 0;
	if ((qpHelper->write(&pri, 1) != 1) || (qpHelper->write(reinterpret_cast<char *>(buf), len) != len))
		qWarning("G15LCDEngine_lglcd: failed to write");
}

/* -- */

G15LCDDeviceLGLCD::G15LCDDeviceLGLCD(G15LCDEngineLGLCD *e) : LCDDevice() {
	engine = e;
}

G15LCDDeviceLGLCD::~G15LCDDeviceLGLCD() {
}

bool G15LCDDeviceLGLCD::enabled() {
	return bEnabled;
}

void G15LCDDeviceLGLCD::setEnabled(bool b) {
	engine->setProcessStatus(b);
	bEnabled = b;
}

void G15LCDDeviceLGLCD::blitImage(QImage *img, bool alert) {
	Q_ASSERT(img);
	int len = G15_MAX_FBMEM_BITS;
	uchar *tmp = img->bits();
	uchar buf[G15_MAX_FBMEM];

	if (! engine->framebufferReady())
		return;

	if (! bEnabled)
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
		int idx = i*8;
		buf[idx+7] = tmp[i] & 0x80 ? 0xff : 0x00;
		buf[idx+6] = tmp[i] & 0x40 ? 0xff : 0x00;
		buf[idx+5] = tmp[i] & 0x20 ? 0xff : 0x00;
		buf[idx+4] = tmp[i] & 0x10 ? 0xff : 0x00;
		buf[idx+3] = tmp[i] & 0x08 ? 0xff : 0x00;
		buf[idx+2] = tmp[i] & 0x04 ? 0xff : 0x00;
		buf[idx+1] = tmp[i] & 0x02 ? 0xff : 0x00;
		buf[idx+0] = tmp[i] & 0x01 ? 0xff : 0x00;
	}

	engine->submitFrame(alert, buf, G15_MAX_FBMEM);
}

QString G15LCDDeviceLGLCD::name() const {
	return QString::fromLatin1("Logitech Gamepanel");
}

QSize G15LCDDeviceLGLCD::size() const {
	return QSize(G15_MAX_WIDTH, G15_MAX_HEIGHT);
}
