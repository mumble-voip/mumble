// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "LegacyPasswordHash.h"

#include <QByteArray>
#include <QCryptographicHash>
#include <QString>

QString getLegacyPasswordHash(const QString &password) {
	QByteArray hash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha1);

	return QString::fromLatin1(hash.toHex());
}

QString getLegacyPasswordHash(const std::string &password) {
	return getLegacyPasswordHash(QString::fromStdString(password));
}
