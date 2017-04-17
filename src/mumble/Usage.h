// Copyright 2005-2017 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_USAGE_H_
#define MUMBLE_MUMBLE_USAGE_H_

#include <QtCore/QBuffer>
#include <QtCore/QDataStream>
#include <QtCore/QObject>

class ClientUser;

class Usage : public QObject {
		Q_OBJECT
	protected:
		QBuffer qbReport;
		QDataStream qdsReport;
	public:
		Usage(QObject *p = NULL);
	public slots:
		void registerUsage();
};

#endif
