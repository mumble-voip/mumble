// Copyright 2005-2017 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "mumble_pch.hpp"

#include "Log.h"
#include "Global.h"
#include "MainWindow.h"
#include "Settings.h"

void Log::postNotification(MsgType mt, const QString &plain) {
	// Message notification with balloon tooltips
	QString qsIcon;
	switch (mt) {
		case DebugInfo:
		case CriticalError:
			qsIcon=QLatin1String("gtk-dialog-error");
			break;
		case Warning:
			qsIcon=QLatin1String("gtk-dialog-warning");
			break;
		case TextMessage:
			qsIcon=QLatin1String("gtk-edit");
			break;
		default:
			qsIcon=QLatin1String("gtk-dialog-info");
			break;
	}

#ifdef USE_DBUS
	QDBusMessage response;
	QVariantMap hints;
	hints.insert(QLatin1String("desktop-entry"), QLatin1String("mumble"));

	{
		QDBusInterface kde(QLatin1String("org.kde.VisualNotifications"), QLatin1String("/VisualNotifications"), QLatin1String("org.kde.VisualNotifications"));
		if (kde.isValid()) {
			QList<QVariant> args;
			args.append(QLatin1String("mumble"));
			args.append(uiLastId);
			args.append(QString());
			args.append(QLatin1String("mumble"));
			args.append(msgName(mt));
			args.append(plain);
			args.append(QStringList());
			args.append(hints);
			args.append(5000);

			response = kde.callWithArgumentList(QDBus::AutoDetect, QLatin1String("Notify"), args);
		}
	}

	if (response.type()!=QDBusMessage::ReplyMessage || response.arguments().at(0).toUInt()==0) {
		QDBusInterface gnome(QLatin1String("org.freedesktop.Notifications"), QLatin1String("/org/freedesktop/Notifications"), QLatin1String("org.freedesktop.Notifications"));
		if (gnome.isValid())
			response = gnome.call(QLatin1String("Notify"), QLatin1String("Mumble"), uiLastId, qsIcon, msgName(mt), plain, QStringList(), hints, -1);
	}

	if (response.type()==QDBusMessage::ReplyMessage && response.arguments().count() == 1) {
		uiLastId = response.arguments().at(0).toUInt();
	} else {
#else
	if (true) {
#endif
		postQtNotification(mt, plain);
	}
}
