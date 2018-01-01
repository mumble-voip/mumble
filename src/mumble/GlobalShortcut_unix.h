// Copyright 2005-2018 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_GLOBALSHORTCUT_UNIX_H_
#define MUMBLE_MUMBLE_GLOBALSHORTCUT_UNIX_H_

#include "GlobalShortcut.h"
#include "ConfigDialog.h"
#include "Global.h"

#include <X11/X.h>
#include <X11/Xlib.h>
#ifndef NO_XINPUT2
#include <X11/extensions/XI2.h>
#include <X11/extensions/XInput2.h>
#endif
#include <X11/Xutil.h>
#ifdef Q_OS_LINUX
#include <linux/input.h>
#include <fcntl.h>
#endif

#define NUM_BUTTONS 0x2ff

class GlobalShortcutX : public GlobalShortcutEngine {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(GlobalShortcutX)
	public:
		Display *display;
		QSet<Window> qsRootWindows;
		int iXIopcode;
		QSet<int> qsMasterDevices;

		volatile bool bRunning;
		QSet<QString> qsKeyboards;
		QMap<QString, QFile *> qmInputDevices;

		GlobalShortcutX();
		~GlobalShortcutX() Q_DECL_OVERRIDE;
		void run() Q_DECL_OVERRIDE;
		QString buttonName(const QVariant &) Q_DECL_OVERRIDE;

		void queryXIMasterList();
	public slots:
		void displayReadyRead(int);
		void inputReadyRead(int);
		void directoryChanged(const QString &);
};

#endif
