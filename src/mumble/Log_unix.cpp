/* Copyright (C) 2005-2011, Thorvald Natvig <thorvald@natvig.com>
   Copyright (C) 2009-2011, Stefan Hacker <dd0t@users.sourceforge.net> 

   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.
   - Neither the name of the Mumble Developers nor the names of its
     contributors may be used to endorse or promote products derived from this
     software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "mumble_pch.hpp"

#include "Log.h"
#include "Global.h"
#include "MainWindow.h"
#include "Settings.h"

void Log::postNotification(MsgType mt, const QString &console, const QString &plain) {
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
			args.append(console);
			args.append(QStringList());
			args.append(hints);
			args.append(5000);

			response = kde.callWithArgumentList(QDBus::AutoDetect, QLatin1String("Notify"), args);
		}
	}

	if (response.type()!=QDBusMessage::ReplyMessage || response.arguments().at(0).toUInt()==0) {
		QDBusInterface gnome(QLatin1String("org.freedesktop.Notifications"), QLatin1String("/org/freedesktop/Notifications"), QLatin1String("org.freedesktop.Notifications"));
		if (gnome.isValid())
			response = gnome.call(QLatin1String("Notify"), QLatin1String("Mumble"), uiLastId, qsIcon, msgName(mt), console, QStringList(), hints, -1);
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