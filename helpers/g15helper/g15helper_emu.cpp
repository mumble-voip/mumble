// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

/*
 * G15 Helper Emulator
 */

#include <windows.h>
#include <cstdio>

#include <QApplication>

#include "g15helper.h"
#include "g15helper_emu.h"

#if defined(Q_OS_WIN)
#	include <fcntl.h>
#	include <io.h>
#endif

G15Reader::G15Reader() : QThread() {
	moveToThread(this);

	m_isRunning = true;
}

G15Reader::~G15Reader() {
	m_isRunning = false;
}

void G15Reader::run() {
#if defined(Q_OS_WIN)
	_setmode(_fileno(stdin), O_BINARY);
#endif

	quint8 buf[G15_MAX_FBMEM];

	QImage img(QSize(G15_MAX_WIDTH, G15_MAX_HEIGHT), QImage::Format_RGB32);

	while (m_isRunning) {
		quint8 priority = 0;
		size_t nread    = 0;
		size_t ntotal   = 0;

		memset(&buf, 0, G15_MAX_FBMEM);

		nread = fread(&priority, 1, sizeof(priority), stdin);
		if (nread <= 0) {
			qFatal("g15helper_emu: unable to read stdin, retval %lli", static_cast< long long >(nread));
		}

		// The priority flag is not used by the emulator.
		Q_UNUSED(priority);

		do {
			nread = fread(&buf[0] + ntotal, 1, G15_MAX_FBMEM - ntotal, stdin);
			if (nread <= 0) {
				qFatal("g15helper_emu: unable to read stdin, retval %lli", static_cast< long long >(nread));
			}
			ntotal += nread;
		} while (ntotal < G15_MAX_FBMEM);

		for (int w = 0; w < G15_MAX_WIDTH; w++) {
			for (int h = 0; h < G15_MAX_HEIGHT; h++) {
				quint8 color = buf[G15_MAX_WIDTH * h + w];
				uint val     = 0xff000000;
				if (color == 0xff) {
					val = 0xffffffff;
				}
				img.setPixel(w, h, val);
			}
		}

		QPixmap p = QPixmap::fromImage(img);

		emit readFrame(p);
	}
}

G15Emulator::G15Emulator() : QMainWindow() {
	setWindowTitle(QLatin1String("Mumble G15 Emulator"));
	setStyleSheet("QMainWindow {background: #cacaca;}");

	m_displayLabel = new QLabel(this);
	m_displayLabel->setMinimumSize(QSize(G15_MAX_WIDTH, G15_MAX_HEIGHT));
	setCentralWidget(m_displayLabel);
}

void G15Emulator::drawFrame(QPixmap p) {
	m_displayLabel->setPixmap(p);
}

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	Q_UNUSED(hInstance);
	Q_UNUSED(hPrevInstance);
	Q_UNUSED(nCmdShow);

	if (lpCmdLine && (strcmp(lpCmdLine, "/detect") == 0)) {
		return 0;
	} else if (!lpCmdLine || (strcmp(lpCmdLine, "/mumble") != 0)) {
		MessageBox(nullptr, L"This program is run by Mumble, and should not be started separately.",
				   L"Nothing to see here, move along", MB_OK | MB_ICONERROR);
		return 0;
	}

	char *argvec[1];
	argvec[0] = nullptr;

	int argc    = 0;
	char **argv = &argvec[0];

	QApplication app(argc, argv);

	G15Reader reader;
	G15Emulator emu;

	QObject::connect(&reader, SIGNAL(readFrame(QPixmap)), &emu, SLOT(drawFrame(QPixmap)));

	reader.start();
	emu.show();

	app.exec();

	return 0;
}
