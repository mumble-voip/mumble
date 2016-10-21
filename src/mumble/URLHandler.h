// Copyright 2005-2016 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_URLHANDLER_H_
#define MUMBLE_MUMBLE_URLHANDLER_H_

#include <QtCore/QtGlobal>
#include "Channel.h"

class Log;

class ChannelUrlPathHandler {
private:
	static const QLatin1String SEP;

	static QString getEncodedChannelname(Channel *c);

	// Depth first search (not searching for alternative exact matches that are
	// possible through ambiguous slashes or case)
	static Channel *searchChannelV1(Channel *root, const QString &path, bool &retExactMatch);
	static Channel *searchChannelV1(Channel *parent, QStringList &subPath, const QString &namePrefix, bool &retExactMatch);

	static Channel *searchChannelV2(Channel *root, const QString &path, bool &retExactMatch);
	static Channel *searchChannelV2(Channel *parent, QStringList &chans, bool &retExactMatch);

public:
	static QString getUrlPathForChannel(Channel *c);

	static Channel *searchChannelByPath(QString &path, bool &retExactMatch);
};

#endif
