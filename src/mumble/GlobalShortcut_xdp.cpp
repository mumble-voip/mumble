// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "GlobalShortcut_xdp.h"

#include "EnvUtils.h"
#include "Global.h"
#include "GlobalShortcutTypes.h"
#include "Settings.h"

#include <QWindow>

#include "globalshortcuts_portal_interface.h"
#include "portalsrequest_interface.h"

Q_GLOBAL_STATIC_WITH_ARGS(OrgFreedesktopPortalGlobalShortcutsInterface, s_shortcutsInterface, ("org.freedesktop.portal.Desktop",
                                                                        "/org/freedesktop/portal/desktop",
                                                                        QDBusConnection::sessionBus()));

bool GlobalShortcutXdp::isAvailable()
{
	return s_shortcutsInterface->isValid();
}

GlobalShortcutXdp::GlobalShortcutXdp() {
	qDBusRegisterMetaType<XdpShortcuts>();
	qDBusRegisterMetaType<XdpShortcut>();

	connect(s_shortcutsInterface, &OrgFreedesktopPortalGlobalShortcutsInterface::Activated, this, &GlobalShortcutXdp::shortcutActivated);
	connect(s_shortcutsInterface, &OrgFreedesktopPortalGlobalShortcutsInterface::Deactivated, this, &GlobalShortcutXdp::shortcutDeactivated);
	connect(s_shortcutsInterface, &OrgFreedesktopPortalGlobalShortcutsInterface::ShortcutsChanged, this, &GlobalShortcutXdp::shortcutsConfigured);

    QTimer::singleShot(0, this, &GlobalShortcutXdp::createSession);
}

GlobalShortcutXdp::~GlobalShortcutXdp() {

}

void GlobalShortcutXdp::createSession()
{
    XdpShortcuts initialShortcuts;
    initialShortcuts.reserve(qmShortcuts.count());
    Global::get().s.qlShortcuts.clear();

    int i = 0;
    m_ids.resize(qmShortcuts.count());
    // Populate the shortcuts list to provide the xdg-desktop-portal
    for (GlobalShortcut *shortcut : qmShortcuts) {
        initialShortcuts.append({shortcut->objectName(), {
            { QStringLiteral("description"), shortcut->name }
        }});

        Shortcut ourShortcut = { i, {uint(i)}, shortcut->qvDefault, false};
        m_ids[i] = shortcut->objectName();
        Global::get().s.qlShortcuts << ourShortcut;
        ++i;
    }

    QDBusArgument arg;
    arg << initialShortcuts;
    QDBusPendingReply<QDBusObjectPath> reply = s_shortcutsInterface->CreateSession({
        { QLatin1String("session_handle_token"), "Mumble" },
        { QLatin1String("handle_token"), QLatin1String("mumble") },
        { QLatin1String("shortcuts"), QVariant::fromValue(arg) },
    });
    reply.waitForFinished();
    if (reply.isError()) {
        qWarning() << "Couldn't get reply";
        qWarning() << "Error:" << reply.error().message();
    } else {
        QDBusConnection::sessionBus().connect(QString(),
                                            reply.value().path(),
                                            QLatin1String("org.freedesktop.portal.Request"),
                                            QLatin1String("Response"),
                                            this,
                                            SLOT(gotGlobalShortcutsCreateSessionResponse(uint,QVariantMap)));
    }
}

void GlobalShortcutXdp::shortcutActivated(const QDBusObjectPath &session_handle, const QString &shortcut_id, qulonglong timestamp, const QVariantMap &options)
{
    Q_UNUSED(session_handle);
    Q_UNUSED(timestamp);
    Q_UNUSED(options);

    for (GlobalShortcut *shortcut : qmShortcuts) {
        if (shortcut_id == shortcut->objectName()) {
            shortcut->triggered(true, shortcut_id);
            shortcut->down(shortcut_id);
        }
    }
}

void GlobalShortcutXdp::shortcutDeactivated(const QDBusObjectPath &session_handle, const QString &shortcut_id, qulonglong timestamp, const QVariantMap &options)
{
    Q_UNUSED(session_handle);
    Q_UNUSED(timestamp);
    Q_UNUSED(options);

    for (GlobalShortcut *shortcut : qmShortcuts) {
        if (shortcut_id == shortcut->objectName()) {
            shortcut->triggered(false, shortcut_id);
        }
    }
}

void GlobalShortcutXdp::shortcutsConfigured(const QDBusObjectPath &session_handle, const XdpShortcuts &shortcuts)
{
    Q_UNUSED(session_handle);
    if (m_shortcuts != shortcuts) {
        m_shortcuts = shortcuts;
        emit shortcutsChanged();
    }
}

void GlobalShortcutXdp::run() {
	// 🤘🤪
}

bool GlobalShortcutXdp::canDisable() {
	return false;
}

GlobalShortcutXdp::ButtonInfo GlobalShortcutXdp::buttonInfo(const QVariant &v) {
	ButtonInfo info;
	bool ok;
	unsigned int key = v.toUInt(&ok);
	if (!ok) {
		return info;
	}

    info.device       = tr("Desktop");
    info.devicePrefix = QString();
    const QString id = m_ids.value(key);
    for (const XdpShortcut &x : qAsConst(m_shortcuts)) {
        if (x.first == id) {
            info.name = x.second["trigger_description"].toString();
        }
    }
	return info;
}

static QString parentWindowId()
{
    if (EnvUtils::waylandIsUsed()) {
        // TODO
        return {};
    }
    return QLatin1String("x11:") + QString::number(qApp->focusWindow()->winId());
}

void GlobalShortcutXdp::gotGlobalShortcutsCreateSessionResponse(uint code, const QVariantMap &results)
{
    if (code != 0) {
        qWarning() << "failed to create a global shortcuts session" << code << results;
        return;
    }

    m_globalShortcutsSession = QDBusObjectPath(results["session_handle"].toString());

    QDBusPendingReply<QDBusObjectPath> reply = s_shortcutsInterface->ListShortcuts(m_globalShortcutsSession, {});
    reply.waitForFinished();
    if (reply.isError()) {
        qWarning() << "failed to call ListShortcuts" << reply.error();
        return;
    }

    OrgFreedesktopPortalRequestInterface *req = new OrgFreedesktopPortalRequestInterface(QLatin1String("org.freedesktop.portal.Desktop"),
                                                        reply.value().path(), QDBusConnection::sessionBus(), this);

    // BindShortcuts and ListShortcuts answer the same
    connect(req, &OrgFreedesktopPortalRequestInterface::Response, this, &GlobalShortcutXdp::gotListShortcutsResponse);
    connect(req, &OrgFreedesktopPortalRequestInterface::Response, req, &QObject::deleteLater);
}

void GlobalShortcutXdp::gotListShortcutsResponse(uint, const QVariantMap &results)
{
    const QDBusArgument arg = results["shortcuts"].value<QDBusArgument>();
    arg >> m_shortcuts;
}

void GlobalShortcutXdp::configure()
{
    QDBusPendingReply<QDBusObjectPath> reply = s_shortcutsInterface->BindShortcuts(m_globalShortcutsSession, m_shortcuts, parentWindowId(), {});
    reply.waitForFinished();
    if (reply.isError()) {
        qWarning() << "failed to call ListShortcuts" << reply.error();
        return;
    }

    OrgFreedesktopPortalRequestInterface *req = new OrgFreedesktopPortalRequestInterface(QLatin1String("org.freedesktop.portal.Desktop"),
                                                        reply.value().path(), QDBusConnection::sessionBus(), this);

    // BindShortcuts and ListShortcuts answer the same
    connect(req, &OrgFreedesktopPortalRequestInterface::Response, this, &GlobalShortcutXdp::gotListShortcutsResponse);
    connect(req, &OrgFreedesktopPortalRequestInterface::Response, req, &QObject::deleteLater);
}
