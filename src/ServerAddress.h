// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_SERVERADDRESS_H_
#define MUMBLE_SERVERADDRESS_H_

#include <QtCore/QString>

#include "HostAddress.h"

/// ServerAddress represents a server
/// address consisting of a HostAddress
/// and a port.
struct ServerAddress {
	HostAddress host    = {};
	unsigned short port = 0;

	/// Construct a default ServerAddress.
	/// The default ServerAddress value is considered
	/// invalid per the |isValid| method.
	ServerAddress() = default;

	/// Construct a ServerAddress pointing to |host_| and |port_|.
	ServerAddress(HostAddress host_, unsigned short port_);

	/// Check whether the ServerAddress is valid.
	/// A ServerAddress is valid if it has a valid |host|
	/// and if its |port| > 0.
	bool isValid() const;
};

/// Check whether the ServerAddresses |lhs| and |rhs| are equal.
bool operator==(const ServerAddress &lhs, const ServerAddress &rhs);

/// Check whether the ServerAddresses |lhs| and |rhs| are not equal.
bool operator!=(const ServerAddress &lhs, const ServerAddress &rhs);

/// Check whether ServerAddress |lhs| should be sorted before |rhs|.
/// This is implemented such that ServerAddress can be used in QMap.
bool operator<(const ServerAddress &lhs, const ServerAddress &rhs);

/// Implementation of qHash for ServerAddress, such that ServerAddress
/// can be used as a key in QHash, QMap, etc.
std::size_t qHash(const ServerAddress &key);

#endif
