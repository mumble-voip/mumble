// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_USAGE_H_
#define MUMBLE_MUMBLE_USAGE_H_

#include <QtCore/QBuffer>
#include <QtCore/QDataStream>
#include <QtCore/QObject>

class ClientUser;

/// This class is used to send usage information to the mumble.info website.
///
/// During instantiation of this class a ten minute timer will be started. After that
/// the object's registerUsage function will be called that sends the respective
/// information to the server.
class Usage : public QObject {
	Q_OBJECT
protected:
	QBuffer qbReport;
	QDataStream qdsReport;

public:
	Usage(QObject *p = nullptr);
public slots:
	/// Performs the actual registration.
	void registerUsage();
};

#endif
