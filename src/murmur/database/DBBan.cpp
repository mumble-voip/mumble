// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "DBBan.h"

#include <boost/algorithm/hex.hpp>

#include <cassert>

namespace mumble {
namespace server {
	namespace db {

		DBBan::DBBan(unsigned int serverID, std::array< std::uint8_t, 16 > baseAddress, unsigned int prefixLength)
			: serverID(serverID), baseAddress(std::move(baseAddress)), prefixLength(prefixLength) {}

		std::string DBBan::ipv6ToString(const std::array< std::uint8_t, 16 > &address) {
			std::string str;
			// Each byte will be represented by 2 hexadecimal digits and every two bytes, there will be a separator
			// (":")
			str.resize(2 * address.size() + (address.size() / 2 - 1));
			auto inIt  = address.begin();
			auto outIt = str.begin();

			while (inIt != address.end()) {
				outIt = boost::algorithm::hex_lower(inIt, inIt + 1, outIt);
				inIt++;
				outIt = boost::algorithm::hex_lower(inIt, inIt + 1, outIt);
				inIt++;
				if (inIt != address.end()) {
					*outIt = ':';
					outIt++;
				}
			}

			return str;
		}

		std::array< std::uint8_t, 16 > DBBan::ipv6FromString(const std::string &str) {
			// Each of the 16 bytes is represented by 2 hexadecimal digits and after every 2 bytes (4 digits) there is a
			// colon acting as a separator
			if (str.size() != 16 * 2 + 7) {
				throw boost::algorithm::hex_decode_error();
			}

			std::array< std::uint8_t, 16 > address;
			auto outIt = address.begin();
			auto inIt  = str.begin();

			while (outIt != address.end()) {
				outIt = boost::algorithm::unhex(inIt, inIt + 2, outIt);
				inIt += 2;
				outIt = boost::algorithm::unhex(inIt, inIt + 2, outIt);
				inIt += 2;
				assert(outIt == address.end() || *inIt == ':');
				inIt++;
			}

			return address;
		}

		bool hostIsUsingBigEndian() {
			const std::uint16_t value{ 0x01 };
			const std::uint8_t *first_byte = reinterpret_cast< const std::uint8_t * >(&value);

			return *first_byte == 0x00;
		}

		std::array< std::uint8_t, 16 > DBBan::ipv4ToIpv6(const std::array< std::uint8_t, 4 > &address,
														 bool convertHostToNetworkByteOrder) {
			// see https://www.rfc-editor.org/rfc/rfc4291#section-2.5.5.2
			std::array< std::uint8_t, 16 > ipv6Address;
			ipv6Address.fill(0);

			ipv6Address[10] = 0xff;
			ipv6Address[11] = 0xff;

			if (!hostIsUsingBigEndian() && convertHostToNetworkByteOrder) {
				// The host is using little-endian encoding, but we want to write the address out in network byte order
				// (big-endian).
				ipv6Address[12] = address[3];
				ipv6Address[13] = address[2];
				ipv6Address[14] = address[1];
				ipv6Address[15] = address[0];
			} else {
				ipv6Address[12] = address[0];
				ipv6Address[13] = address[1];
				ipv6Address[14] = address[2];
				ipv6Address[15] = address[3];
			}

			return ipv6Address;
		}

		unsigned int DBBan::subNetMaskToPrefixLength(unsigned int mask) {
			// When mapping an IPv4 address to IPv6, the actual address is written only to the last 4 bytes of the 16
			// byte address. Thus, the subnet mask that has to be applied to these last 4 bytes also needs to ignore the
			// leading 12 bytes = 96 bits.
			return mask + 96;
		}

		template< typename TimePoint > std::size_t toSeconds(const TimePoint &tp) {
			return std::chrono::duration_cast< std::chrono::seconds >(tp.time_since_epoch()).count();
		}

		bool operator==(const DBBan &lhs, const DBBan &rhs) {
			return lhs.serverID == rhs.serverID && lhs.baseAddress == rhs.baseAddress
				   && lhs.prefixLength == rhs.prefixLength && lhs.bannedUserName == rhs.bannedUserName
				   && lhs.bannedUserCertHash == rhs.bannedUserCertHash && lhs.reason == rhs.reason
				   && toSeconds(lhs.startDate) == toSeconds(rhs.startDate) && lhs.duration == rhs.duration;
		}

		bool operator!=(const DBBan &lhs, const DBBan &rhs) { return !(lhs == rhs); }

	} // namespace db
} // namespace server
} // namespace mumble
