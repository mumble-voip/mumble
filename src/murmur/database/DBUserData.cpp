// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "DBUserData.h"
#include "ChronoUtils.h"

#include <chrono>

namespace mumble {
namespace server {
	namespace db {

		bool operator==(const DBUserData::PasswordData &lhs, const DBUserData::PasswordData &rhs) {
			return lhs.passwordHash == rhs.passwordHash && lhs.salt == rhs.salt
				   && lhs.kdfIterations == rhs.kdfIterations;
		}

		bool operator!=(const DBUserData::PasswordData &lhs, const DBUserData::PasswordData &rhs) {
			return !(lhs == rhs);
		}

		bool operator==(const DBUserData &lhs, const DBUserData &rhs) {
			return lhs.name == rhs.name && lhs.password == rhs.password && lhs.lastChannelID == rhs.lastChannelID
				   && lhs.texture == rhs.texture && toEpochSeconds(lhs.lastActive) == toEpochSeconds(rhs.lastActive)
				   && toEpochSeconds(lhs.lastDisconnect) == toEpochSeconds(rhs.lastDisconnect);
		}

		bool operator!=(const DBUserData &lhs, const DBUserData &rhs) { return !(lhs == rhs); }

	} // namespace db
} // namespace server
} // namespace mumble
