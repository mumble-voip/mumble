// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_SERVER_DATABASE_DBACL_H_
#define MUMBLE_SERVER_DATABASE_DBACL_H_

#include <optional>
#include <string>
#include <vector>

namespace mumble {
namespace server {
	namespace db {

		struct DBAcl {
			enum class MetaGroup {
				None,
				All,
				Auth,
				Strong,
				In,
				Out,
				Sub,
			};

			unsigned int serverID                         = {};
			unsigned int channelID                        = {};
			unsigned int priority                         = {};
			std::optional< unsigned int > affectedUserID  = {};
			std::optional< unsigned int > affectedGroupID = {};
			std::optional< MetaGroup > affectedMetaGroup  = {};
			std::optional< std::string > accessToken      = {};
			std::vector< std::string > groupModifiers     = {};
			bool applyInCurrentChannel                    = true;
			bool applyInSubChannels                       = true;
			unsigned int grantedPrivilegeFlags            = {};
			unsigned int revokedPrivilegeFlags            = {};

			DBAcl() = default;

			friend bool operator==(const DBAcl &lhs, const DBAcl &rhs);
			friend bool operator!=(const DBAcl &lhs, const DBAcl &rhs);
		};

	} // namespace db
} // namespace server
} // namespace mumble

#endif // MUMBLE_SERVER_DATABASE_DBACL_H_
