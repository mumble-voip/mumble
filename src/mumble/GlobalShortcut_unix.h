// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_GLOBALSHORTCUT_UNIX_H_
#define MUMBLE_MUMBLE_GLOBALSHORTCUT_UNIX_H_

#include "ConfigDialog.h"
#include "Global.h"
#include "GlobalShortcut.h"


#include <X11/X.h>

#ifdef USE_WAYLAND
extern "C" {
#	include <libudev.h>
#	include <libinput.h>
#	include <xkbcommon/xkbcommon.h>
}
#endif

#define NUM_BUTTONS 0x2ff

struct _XDisplay;
typedef _XDisplay Display;

class QFileSystemWatcher;
class QSocketNotifier;

#ifdef USE_WAYLAND
class GlobalShortcutWayland : public GlobalShortcutEngine {
private:
	Q_OBJECT
public:
	udev *_udev;
	libinput *_libinput;
	xkb_context *xkb_ctx;
	xkb_keymap *xkb_km;
	xkb_state *xkb;
	GlobalShortcutWayland();
	~GlobalShortcutWayland() Q_DECL_OVERRIDE;
	ButtonInfo buttonInfo(const QVariant &) Q_DECL_OVERRIDE;
public slots:
	void libinputEvent(int);
protected:
	bool init();
};
#endif

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

	bool canDisable() override;
	bool enabled() override;
	void setEnabled(bool enabled) override;
public slots:
	void displayReadyRead(int);
	void inputReadyRead(int);
	void directoryChanged(const QString &);

protected:
	bool m_enabled                = true;
	QFileSystemWatcher *m_watcher = nullptr;
	QSocketNotifier *m_notifier   = nullptr;

	bool init();
	void stop();
};

// These are macros that X11/X.h defines and that are causing problems in unity builds
// if left defined
#undef None
#undef KeyPress
#undef KeyRelease
#undef FontChange

#endif
