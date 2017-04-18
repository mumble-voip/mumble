// Copyright 2005-2017 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_LOGEMITTER_H_
#define MUMBLE_LOGEMITTER_H_

#include <QtCore/QObject>
#include <QtCore/QString>

class LogEmitter : public QObject {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(LogEmitter)
	signals:
		void newLogEntry(const QString &msg);
	public:
		LogEmitter(QObject *parent = NULL);
		void addLogEntry(const QString &msg);
};

#endif
