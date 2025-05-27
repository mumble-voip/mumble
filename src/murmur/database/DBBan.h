// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_SERVER_DATABASE_DBBAN_H_
#define MUMBLE_SERVER_DATABASE_DBBAN_H_

#include <array>
#include <chrono>
#include <cstdint>
#include <optional>
#include <string>

#include <boost/algorithm/string.hpp>

namespace mumble {
namespace server {
	namespace db {

		/**
		 * A small struct representing what the database needs to know / knows about a ban
		 */
		struct DBBan {
			using ipv6_type = std::array< std::uint8_t, 16 >;

			unsigned int serverID = {};
			/**
			 * The base address encoded as an IPv6 address (16 bytes in network order)
			 */
			ipv6_type baseAddress                                          = {};
			std::uint8_t prefixLength                                      = {};
			std::optional< std::string > bannedUserName                    = {};
			std::optional< std::string > bannedUserCertHash                = {};
			std::optional< std::string > reason                            = {};
			std::chrono::time_point< std::chrono::system_clock > startDate = {};
			std::chrono::seconds duration                                  = {};

			DBBan() = default;
			DBBan(unsigned int serverID, ipv6_type baseAddress, std::uint8_t prefixLength);


			static std::string ipv6ToString(const ipv6_type &address);
			static ipv6_type ipv6FromString(const std::string &str);
			static ipv6_type ipv4ToIpv6(const std::array< std::uint8_t, 4 > &address,
										bool convertHostToNetworkByteOrder = false);
			static std::uint8_t subNetMaskToPrefixLength(std::uint8_t mask);


			friend bool operator==(const DBBan &lhs, const DBBan &rhs);
			friend bool operator!=(const DBBan &lhs, const DBBan &rhs);
		};

	} // namespace db
} // namespace server
} // namespace mumble

#endif // MUMBLE_SERVER_DATABASE_DBBAN_H_
