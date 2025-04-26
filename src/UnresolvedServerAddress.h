// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_UNRESOLVEDSERVERADDRESS_H_
#define MUMBLE_UNRESOLVEDSERVERADDRESS_H_

#include <QtCore/QString>

/// UnresolvedServerAddress represents a
/// server address consisting of a hostname
/// and a port.
struct UnresolvedServerAddress {
	QString hostname;
	unsigned short port;

	/// Construct a default UnresolvedServerAddress.
	/// The default UnresolvedServerAddress value is considered
	/// invalid per the |isValid| method.
	UnresolvedServerAddress();

	/// Construct a UnresolvedServerAddress pointing to |hostname| and |port|.
	/// The passed-in hostname is normalized to lowercase.
	UnresolvedServerAddress(QString hostname, unsigned short port);

	/// Check whether the UnresolvedServerAddress is valid.
	/// An UnresolvedServerAddress is valid if it has a non-empty
	/// |hostname| and if its |port| > 0.
	bool isValid() const;
};

/// Check whether |lhs| and |rhs| are equal.
bool operator==(const UnresolvedServerAddress &lhs, const UnresolvedServerAddress &rhs);

/// Check whether |lhs| and |rhs| are not equal.
bool operator!=(const UnresolvedServerAddress &lhs, const UnresolvedServerAddress &rhs);

/// Check whether |lhs| is less than |rhs|.
/// This is implemented such that UnresolvedServerAddress can be used in QMap.
bool operator<(const UnresolvedServerAddress &lhs, const UnresolvedServerAddress &rhs);

/// Implementation of qHash for UnresolvedServerAddress, such that
/// UnresolvedServerAddress can be used as a key in QHash, QMap, etc.
std::size_t qHash(const UnresolvedServerAddress &key);

#endif
