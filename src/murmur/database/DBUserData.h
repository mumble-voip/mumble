// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_SERVER_DATABASE_DBUSERDATA_H_
#define MUMBLE_SERVER_DATABASE_DBUSERDATA_H_

#include <chrono>
#include <cstdint>
#include <string>
#include <vector>

namespace mumble {
namespace server {
	namespace db {

		struct DBUserData {
			struct PasswordData {
				std::string passwordHash = {};
				std::string salt         = {};
				int kdfIterations        = 0;

				PasswordData() = default;
				PasswordData(std::string hash, std::string salt = {}, int kdfIterations = 0)
					: passwordHash(std::move(hash)), salt(std::move(salt)), kdfIterations(kdfIterations) {}

				friend bool operator==(const PasswordData &lhs, const PasswordData &rhs);
				friend bool operator!=(const PasswordData &lhs, const PasswordData &rhs);
			};

			std::string name                                                    = {};
			PasswordData password                                               = {};
			unsigned int lastChannelID                                          = {};
			std::vector< std::uint8_t > texture                                 = {};
			std::chrono::time_point< std::chrono::steady_clock > lastActive     = {};
			std::chrono::time_point< std::chrono::steady_clock > lastDisconnect = {};

			DBUserData() = default;

			friend bool operator==(const DBUserData &lhs, const DBUserData &rhs);
			friend bool operator!=(const DBUserData &lhs, const DBUserData &rhs);
		};

	} // namespace db
} // namespace server
} // namespace mumble

#endif // MUMBLE_SERVER_DATABASE_DBUSERDATA_H_
