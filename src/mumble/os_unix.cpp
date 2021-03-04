// Copyright 2016-2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.
#include "LogEmitter.h"
#include "Global.h"

static QSharedPointer< LogEmitter > le;

static void mumbleMessageOutputQString(QtMsgType type, const QString &msg) {
	char c;

	switch (type) {
		case QtDebugMsg:
			c = 'D';
			break;
		case QtWarningMsg:
			c = 'W';
			break;
		case QtFatalMsg:
			c = 'F';
			break;
		default:
			c = 'X';
	}

	QString date = QDateTime::currentDateTime().toString(QLatin1String("yyyy-MM-dd hh:mm:ss.zzz"));
	QString fmsg = QString::fromLatin1("<%1>%2 %3").arg(c).arg(date).arg(msg);
	fprintf(stderr, "%s\n", qPrintable(fmsg));

	le->addLogEntry(fmsg);

	if (type == QtFatalMsg) {
		exit(1);
	}
}

static void mumbleMessageOutputWithContext(QtMsgType type, const QMessageLogContext &ctx, const QString &msg) {
	Q_UNUSED(ctx);
	mumbleMessageOutputQString(type, msg);
}

void os_init() {
	// Make a copy of the global LogEmitter, such that
	// os_unix.cpp doesn't have to consider the deletion
	// of the Global object and its LogEmitter object.
	le = Global::get().le;

	qInstallMessageHandler(mumbleMessageOutputWithContext);
}
