// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_GLOBALSHORTCUT_MACX_H_
#define MUMBLE_MUMBLE_GLOBALSHORTCUT_MACX_H_

#include <QtCore/QObject>
#include <stdlib.h>

#include <ApplicationServices/ApplicationServices.h>

#include "Global.h"
#include "GlobalShortcut.h"

class GlobalShortcutMac : public GlobalShortcutEngine {
private:
	Q_OBJECT
	Q_DISABLE_COPY(GlobalShortcutMac)
public:
	GlobalShortcutMac();
	~GlobalShortcutMac() Q_DECL_OVERRIDE;
	ButtonInfo buttonInfo(const QVariant &) Q_DECL_OVERRIDE;
	void dumpEventTaps();
	void needRemap() Q_DECL_OVERRIDE;
	bool handleModButton(CGEventFlags newmask);
	bool canSuppress() Q_DECL_OVERRIDE;

	void setEnabled(bool) Q_DECL_OVERRIDE;
	bool enabled() Q_DECL_OVERRIDE;
	bool canDisable() Q_DECL_OVERRIDE;

public slots:
	void forwardEvent(void *evt);

protected:
	CFRunLoopRef loop;
	CFMachPortRef port;
	CGEventFlags modmask;
	UCKeyboardLayout *kbdLayout;

	void run() Q_DECL_OVERRIDE;

	static CGEventRef callback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *udata);
	QString translateMouseButton(const unsigned int keycode) const;
	QString translateModifierKey(const unsigned int keycode) const;
	QString translateKeyName(const unsigned int keycode) const;
};

#endif
