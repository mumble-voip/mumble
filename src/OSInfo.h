// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_OSINFO_H_
#define MUMBLE_OSINFO_H_

#include <QtCore/QList>
#include <QtCore/QString>

class QDomDocument;
class QDomElement;
class QHostAddress;

class OSInfo {
	public:
		static QString getMacHash(const QList<QHostAddress> & = QList<QHostAddress>());
		static QString getOS();
		static QString getOSVersion();
		static QString getOSDisplayableVersion();
		static void fillXml(QDomDocument &doc, QDomElement &root, const QString &os = OSInfo::getOS(), const QString &osver = OSInfo::getOSVersion(), const QList<QHostAddress> & = QList<QHostAddress>());
};

#endif
