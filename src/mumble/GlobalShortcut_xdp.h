// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_GLOBALSHORTCUT_XDP_H_
#define MUMBLE_MUMBLE_GLOBALSHORTCUT_XDP_H_

#include "ConfigDialog.h"
#include "Global.h"
#include "GlobalShortcut.h"

#include <QDBusObjectPath>

class OrgFreedesktopPortalGlobalShortcutsInterface;

using XdpShortcut = QPair<QString, QVariantMap>;
using XdpShortcuts = QList<XdpShortcut>;

class GlobalShortcutXdp : public GlobalShortcutEngine {
private:
	Q_OBJECT
	Q_DISABLE_COPY(GlobalShortcutXdp)
public:
	static bool isAvailable();

	void createSession();

	GlobalShortcutXdp();
	~GlobalShortcutXdp() Q_DECL_OVERRIDE;
	void run() Q_DECL_OVERRIDE;
	ButtonInfo buttonInfo(const QVariant &) Q_DECL_OVERRIDE;

	bool canDisable() override;
	bool canConfigure() override { return true; }
	void configure() override;

public slots:
    void gotGlobalShortcutsCreateSessionResponse(uint, const QVariantMap &results);
    void gotListShortcutsResponse(uint, const QVariantMap &results);

	void shortcutActivated(const QDBusObjectPath &session_handle, const QString &shortcut_id, qulonglong timestamp, const QVariantMap &options);
	void shortcutDeactivated(const QDBusObjectPath &session_handle, const QString &shortcut_id, qulonglong timestamp, const QVariantMap &options);
	void shortcutsConfigured(const QDBusObjectPath &session_handle, const QList<QPair<QString,QVariantMap>> &shortcuts);

private:
	XdpShortcuts m_shortcuts;
	QVector<QString> m_ids;
	QDBusObjectPath m_globalShortcutsSession;
};

#endif
