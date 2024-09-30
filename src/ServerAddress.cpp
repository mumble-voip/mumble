// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ServerAddress.h"

ServerAddress::ServerAddress(HostAddress host_, unsigned short port_) : host(host_), port(port_) {
}

bool ServerAddress::isValid() const {
	return host.isValid() && port != 0;
}

bool operator==(const ServerAddress &lhs, const ServerAddress &rhs) {
	return lhs.host == rhs.host && lhs.port == rhs.port;
}

bool operator!=(const ServerAddress &lhs, const ServerAddress &rhs) {
	return !operator==(lhs, rhs);
}

bool operator<(const ServerAddress &lhs, const ServerAddress &rhs) {
	if (lhs.host < rhs.host) {
		return true;
	} else if (lhs.host == rhs.host) {
		if (lhs.port < rhs.port) {
			return true;
		}
	}
	return false;
}

std::size_t qHash(const ServerAddress &key) {
	return qHash(key.host) ^ static_cast< std::size_t >(key.port);
}
