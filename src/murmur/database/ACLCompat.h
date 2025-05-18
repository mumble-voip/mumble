// Copyright 2023 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_SERVER_DATABASE_ACLCOMPAT_H_
#define MUMBLE_SERVER_DATABASE_ACLCOMPAT_H_

#include "DBAcl.h"

#include <optional>
#include <string>
#include <vector>

namespace mumble {
namespace server {
	namespace db {

		class GroupTable;

		struct LegacyGroupData {
			std::string name;
			bool isAccessToken = false;
			std::vector< std::string > modifiers;
		};

		LegacyGroupData parseLegacyGroupSpecification(std::string groupSpec);

		std::optional< DBAcl::MetaGroup > parseMetaGroup(const std::string &str);

		std::string metaGroupName(DBAcl::MetaGroup group);

		std::string getLegacyGroupData(const DBAcl &acl, GroupTable &groupTable);

	} // namespace db
} // namespace server
} // namespace mumble

#endif // MUMBLE_SERVER_DATABASE_ACLCOMPAT_H_
