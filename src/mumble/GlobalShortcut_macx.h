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

// Helper functions for runtime macOS version checking.
// These allow C++ code to check and request the appropriate permission
// (Input Monitoring on macOS 10.15+, Accessibility on older versions).
#ifdef __cplusplus
extern "C" {
#endif

/// Check if the app has the necessary permission for global shortcuts.
/// On macOS 10.15+, checks Input Monitoring; on older versions, checks Accessibility.
bool macOS_hasGlobalShortcutPermission();

/// Request the necessary permission for global shortcuts.
/// On macOS 10.15+, requests Input Monitoring; on older versions, this is a no-op
/// (Accessibility permission is requested automatically when the event tap fails).
void macOS_requestGlobalShortcutPermission();

/// Open the appropriate Privacy settings pane in System Preferences.
/// On macOS 10.15+, opens Input Monitoring; on older versions, opens Accessibility.
void macOS_openPrivacySettings();

/// Returns true if the current macOS version uses Input Monitoring (10.15+),
/// false if it uses Accessibility (10.14 and earlier).
bool macOS_usesInputMonitoring();

#ifdef __cplusplus
}
#endif

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
