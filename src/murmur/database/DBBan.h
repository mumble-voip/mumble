// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_SERVER_DATABASE_DBBAN_H_
#define MUMBLE_SERVER_DATABASE_DBBAN_H_

#include <array>
#include <chrono>
#include <cstdint>
#include <string>

#include <boost/optional.hpp>

namespace mumble {
namespace server {
	namespace db {

		/**
		 * A small struct representing what the database needs to know / knows about a ban
		 */
		struct DBBan {
			unsigned int serverID = {};
			/**
			 * The base address encoded as an IPv6 address (16 bytes in network order)
			 */
			std::array< std::uint8_t, 16 > baseAddress                     = {};
			unsigned int prefixLength                                      = {};
			boost::optional< std::string > bannedUserName                  = {};
			boost::optional< std::string > bannedUserCertHash              = {};
			boost::optional< std::string > reason                          = {};
			std::chrono::time_point< std::chrono::steady_clock > startDate = {};
			std::chrono::seconds duration                                  = {};

			DBBan() = default;
			DBBan(unsigned int serverID, std::array< std::uint8_t, 16 > baseAddress, unsigned int prefixLength = 128);


			static std::string ipv6ToString(const std::array< std::uint8_t, 16 > &address);
			static std::array< std::uint8_t, 16 > ipv6FromString(const std::string &str);
			static std::array< std::uint8_t, 16 > ipv4ToIpv6(const std::array< std::uint8_t, 4 > &address,
															 bool convertHostToNetworkByteOrder = false);
			static unsigned int subNetMaskToPrefixLength(unsigned int mask);


			friend bool operator==(const DBBan &lhs, const DBBan &rhs);
			friend bool operator!=(const DBBan &lhs, const DBBan &rhs);
		};

	} // namespace db
} // namespace server
} // namespace mumble

#endif // MUMBLE_SERVER_DATABASE_DBBAN_H_
