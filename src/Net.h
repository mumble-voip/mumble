// Copyright 2005-2017 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_NET_H_
#define MUMBLE_NET_H_

#include <QtCore/QDateTime>
#include <QtCore/QString>
#include <QtNetwork/Q_IPV6ADDR>

#include "HostAddress.h"

#ifndef DEFAULT_MUMBLE_PORT
#define DEFAULT_MUMBLE_PORT 64738
#endif

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
	bool operator < (const Ban &) const;
	bool operator == (const Ban &) const;
	QString toString() const;
};

quint32 qHash(const Ban &);

#endif
