// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_G15_HELPER_EMU_H_
#define MUMBLE_G15_HELPER_EMU_H_

#include <QFile>
#include <QLabel>
#include <QMainWindow>
#include <QThread>

class G15Reader : public QThread {
	Q_OBJECT
public:
	G15Reader();
	~G15Reader();
	void run() Q_DECL_OVERRIDE;

	bool m_isRunning;
signals:
	void readFrame(QPixmap p);
};

class G15Emulator : public QMainWindow {
	Q_OBJECT
public:
	QLabel *m_displayLabel;
	G15Emulator();
public slots:
	void drawFrame(QPixmap p);
};

#endif
