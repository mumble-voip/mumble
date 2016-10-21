// Copyright 2005-2016 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.


#include "URLHandler.h"

#include "Log.h"

const QLatin1String ChannelUrlPathHandler::SEP = QLatin1String("/");

QString ChannelUrlPathHandler::getEncodedChannelname(Channel *c) {
	return QString::fromUtf8(QUrl::toPercentEncoding(c->qsName));
}

Channel *ChannelUrlPathHandler::searchChannelV1(Channel *parent, QStringList &subPath, const QString &namePrefix, bool &retExactMatch) {
	// We are out of stuff/path to look for
	if (subPath.isEmpty()) {
		// If we have an unidentified channel, we failed to find an exact match
		retExactMatch = namePrefix.isNull();
		return parent;
	}

	// Next channel name to look for
	QString name = subPath.takeFirst();
	// If we still have an unidentified channel, construct name with slash
	if (!namePrefix.isNull()) {
		name.prepend(SEP);
		name.prepend(namePrefix);
	}

	Channel *child = NULL;
	foreach(Channel *c, parent->qlChannels) {
		// Compare in lower case for backwards compatibility (can lead to
		// mistakes with channels where only the case is different)
		if (c->qsName.toLower() == name.toLower()) {
			child = c;
			break;
		}
	}

	return searchChannelV1(child ? child : parent, subPath, child ? QString() : name, retExactMatch);
}

// Depth first search (not searching for alternative exact matches that are
// possible through ambiguous slashes or case)
Channel *ChannelUrlPathHandler::searchChannelV1(Channel *root, const QString &path, bool &retExactMatch) {
	// The hostname-path separator slash is missing for URLs copied from:
	// * The server (connect dialog)
	// * The root channel
	// * Channels with a leading slash that are a child of the root channel
	//
	// However, as path was passed through QUrl it always has a leading slash.
	// It is thus ambiguous whether it is a channel name slash or separator.
	//
	// Most/All special characters are escaped, except slashes. Thus, a path
	// may not be unique (c1/c2 could be a channel or parent and child).
	//
	// Channels with a leading slash deeper into the tree have their own
	// slash (path separator + channel name slash: root/chan1//ChanwithOneLeadingSlash).

	QStringList qlChans = path.split(SEP);
	Channel *chan = searchChannelV1(root, qlChans, QString(), retExactMatch);

	// We expect a path of "/" to have been an URL without a path slash
	// in most cases (an URL to the server/root channel). In that case, the
	// root channel is an exact match. As "/" is a valid channel as well, we
	// had to look for it, but did not find such a channel.
	if (!retExactMatch && path == SEP) {

		retExactMatch = true;
	}
	return chan;
}

Channel *ChannelUrlPathHandler::searchChannelV2(Channel *parent, QStringList &chans, bool &retExactMatch) {
	if (chans.isEmpty()) {
		retExactMatch = true;
		return parent;
	}

	QString name = chans.takeFirst();
	foreach(Channel *c, parent->qlChannels) {
		QString cn = getEncodedChannelname(c);
		if (cn == name) {
			return searchChannelV2(c, chans, retExactMatch);
		}
	}
	retExactMatch = false;
	return parent;
}

Channel *ChannelUrlPathHandler::searchChannelV2(Channel *root, const QString &path, bool &retExactMatch) {
	QStringList qlChans = path.split(SEP);
	// The path always begins with a slash - remove the first element
	qlChans.removeFirst();
	return searchChannelV2(root, qlChans, retExactMatch);
}

QString ChannelUrlPathHandler::getUrlPathForChannel(Channel *c) {
	if (c->cParent == NULL) {
		return "/";
	}
	QString path;
	while (c->cParent != NULL) {
		path.prepend(getEncodedChannelname(c));
		path.prepend(SEP);
		c = c->cParent;
	}
	return path;
}

Channel *ChannelUrlPathHandler::searchChannelByPath(QString &path, bool &retExactMatch) {
	Channel *root = Channel::get(0);
	if (!root) {
		qFatal("Failed to identify Root channel");
		return NULL;
	}

	bool bExactMatchV2 = false;
	Channel *matchV2 = searchChannelV2(root, path, bExactMatchV2);
	if (bExactMatchV2) {
		retExactMatch = true;
		return matchV2;
	}

	// Try to parse path in old URL encoding
	bool bExactMatchV1 = false;
	Channel *matchV1 = searchChannelV1(root, path, bExactMatchV1);
	if (bExactMatchV1) {
		retExactMatch = true;
		return matchV1;
	}

	retExactMatch = false;
	return matchV1;
}
