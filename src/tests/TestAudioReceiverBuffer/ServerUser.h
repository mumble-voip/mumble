// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.


// NOTE: This is merely a mock of the ServerUser class

#include "Version.h"

#include <string>

struct ServerUser {
	ServerUser(unsigned int uiSession, Version::full_t version, bool deaf = false, bool selfDeaf = false,
			   const std::string context = "")
		: uiSession(uiSession), m_version(version), bDeaf(deaf), bSelfDeaf(selfDeaf), ssContext(context) {}

	unsigned int uiSession;
	Version::full_t m_version;
	bool bDeaf;
	bool bSelfDeaf;
	std::string ssContext;
};
