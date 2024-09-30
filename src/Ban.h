// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_BAN_H_
#define MUMBLE_BAN_H_

#include "HostAddress.h"

#include <QtCore/QDateTime>
#include <QtCore/QString>
#include <QtCore/QtGlobal>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/Q_IPV6ADDR>

struct Ban {
	HostAddress haAddress;
	int iMask;
	QString qsUsername;
	QString qsHash;
	QString qsReason;
	QDateTime qdtStart;
	unsigned int iDuration;
	bool isExpired() const;
	bool isValid() const;
	bool operator<(const Ban &) const;
	bool operator==(const Ban &) const;
	QString toString() const;
};

std::size_t qHash(const Ban &);

#endif
