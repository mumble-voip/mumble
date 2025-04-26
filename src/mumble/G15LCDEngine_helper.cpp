// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "G15LCDEngine_helper.h"

#include "MumbleApplication.h"

static LCDEngine *G15LCDEngineNew() {
	return new G15LCDEngineHelper();
}

static LCDEngineRegistrar registrarLCDEngine(G15LCDEngineNew);

G15LCDEngineHelper::G15LCDEngineHelper() : LCDEngine() {
	bRunning     = false;
	bUnavailable = true;

#if defined(Q_OS_WIN)
	qsHelperExecutable = QString::fromLatin1("\"%1/mumble-g15-helper.exe\"")
							 .arg(MumbleApplication::instance()->applicationVersionRootPath());
#elif defined(Q_OS_MAC)
	qsHelperExecutable = QString::fromLatin1("\"%1/mumble-g15-helper\"")
							 .arg(MumbleApplication::instance()->applicationVersionRootPath());
#endif

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

	qlDevices << new G15LCDDeviceHelper(this);
	bUnavailable = false;

	QMetaObject::connectSlotsByName(this);
}

G15LCDEngineHelper::~G15LCDEngineHelper() {
	setProcessStatus(false);
}

QList< LCDDevice * > G15LCDEngineHelper::devices() const {
	return qlDevices;
}

void G15LCDEngineHelper::setProcessStatus(bool run) {
	if (bUnavailable)
		return;

	if (run && !bRunning) {
		bRunning = true;
		qpHelper->start(qsHelperExecutable, QStringList(QLatin1String("/mumble")));
		if (!qpHelper->waitForStarted(2000)) {
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

void G15LCDEngineHelper::on_Helper_finished(int exitCode, QProcess::ExitStatus status) {
	/* Skip the signal if we killed ourselves. */
	if (!bRunning)
		return;

	if (status == QProcess::CrashExit) {
		qWarning("G15LCDEngine_lglcd: Helper process crashed. Restarting.");
		qpHelper->start(qsHelperExecutable, QStringList(QLatin1String("/mumble")));
	} else if (status == QProcess::NormalExit && exitCode != 0) {
		qWarning("G15LCDEngine_lglcd: Helper process exited. Exit code was: `%i'. Not attempting recovery.", exitCode);
		bUnavailable = true;
	}
}

bool G15LCDEngineHelper::framebufferReady() const {
	return !bUnavailable && (qpHelper->state() == QProcess::Running);
}

void G15LCDEngineHelper::submitFrame(bool alert, unsigned char *buf, qint64 len) {
	char pri = alert ? 1 : 0;
	if ((qpHelper->write(&pri, 1) != 1) || (qpHelper->write(reinterpret_cast< char * >(buf), len) != len))
		qWarning("G15LCDEngine_lglcd: failed to write");
}

/* -- */

G15LCDDeviceHelper::G15LCDDeviceHelper(G15LCDEngineHelper *e) : LCDDevice(), bEnabled(false) {
	engine = e;
}

G15LCDDeviceHelper::~G15LCDDeviceHelper() {
}

bool G15LCDDeviceHelper::enabled() {
	return bEnabled;
}

void G15LCDDeviceHelper::setEnabled(bool b) {
	engine->setProcessStatus(b);
	bEnabled = b;
}

void G15LCDDeviceHelper::blitImage(QImage *img, bool alert) {
	Q_ASSERT(img);
	int len    = G15_MAX_FBMEM_BITS;
	uchar *tmp = img->bits();
	uchar buf[G15_MAX_FBMEM];

	if (!engine->framebufferReady())
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

	engine->submitFrame(alert, buf, G15_MAX_FBMEM);
}

QString G15LCDDeviceHelper::name() const {
	return QString::fromLatin1("Logitech Gamepanel");
}

QSize G15LCDDeviceHelper::size() const {
	return QSize(G15_MAX_WIDTH, G15_MAX_HEIGHT);
}
