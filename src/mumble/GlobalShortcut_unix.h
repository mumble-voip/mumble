// Copyright 2007-2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_GLOBALSHORTCUT_UNIX_H_
#define MUMBLE_MUMBLE_GLOBALSHORTCUT_UNIX_H_

#include "ConfigDialog.h"
#include "Global.h"
#include "GlobalShortcut.h"

#include <X11/X.h>

#define NUM_BUTTONS 0x2ff

struct _XDisplay;
typedef _XDisplay Display;

class GlobalShortcutX : public GlobalShortcutEngine {
private:
	Q_OBJECT
	Q_DISABLE_COPY(GlobalShortcutX)
public:
	Display *display;
	QSet< Window > qsRootWindows;
	int iXIopcode;
	QSet< int > qsMasterDevices;

	volatile bool bRunning;
	QSet< QString > qsKeyboards;
	QMap< QString, QFile * > qmInputDevices;

	GlobalShortcutX();
	~GlobalShortcutX() Q_DECL_OVERRIDE;
	void run() Q_DECL_OVERRIDE;
	ButtonInfo buttonInfo(const QVariant &) Q_DECL_OVERRIDE;

	void queryXIMasterList();
public slots:
	void displayReadyRead(int);
	void inputReadyRead(int);
	void directoryChanged(const QString &);
};

// These are macros that X11/X.h defines and that are causing problems in unity builds
// if left defined
#undef None
#undef KeyPress
#undef FontChange

#endif
