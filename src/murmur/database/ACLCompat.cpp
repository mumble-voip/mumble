// Copyright 2023 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ACLCompat.h"
#include "GroupTable.h"

#include <algorithm>
#include <cassert>
#include <optional>

namespace mumble {
namespace server {
	namespace db {

		LegacyGroupData parseLegacyGroupSpecification(std::string groupSpec) {
			static const std::vector< char > existingGroupModifiers = { '!', '~', '#', '$' };

			LegacyGroupData data;

			while (!groupSpec.empty()
				   && std::find(existingGroupModifiers.begin(), existingGroupModifiers.end(), groupSpec[0])
						  != existingGroupModifiers.end()) {
				if (groupSpec[0] == '#') {
					data.isAccessToken = true;
				} else {
					data.modifiers.push_back(std::string(1, groupSpec[0]));
				}

				// Remove modifier from group name
				groupSpec.erase(groupSpec.begin());
			}

			// Reverse modifier order as they will also be applied in reverse order and therefore reversing here retains
			// the original order
			std::reverse(data.modifiers.begin(), data.modifiers.end());

			if (!groupSpec.empty() && groupSpec.size() > 4 && groupSpec.substr(0, 4) == "sub,") {
				// This is the "sub" metagroup and it contains the special modifier representing a parameter list
				std::string parameters = groupSpec.substr(3, std::string::npos);
				groupSpec.erase(3, std::string::npos);

				data.modifiers.push_back(std::move(parameters));
			}

			data.name = std::move(groupSpec);

			return data;
		}

		std::optional< DBAcl::MetaGroup > parseMetaGroup(const std::string &str) {
			if (str == "all") {
				return DBAcl::MetaGroup::All;
			} else if (str == "auth") {
				return DBAcl::MetaGroup::Auth;
			} else if (str == "in") {
				return DBAcl::MetaGroup::In;
			} else if (str == "none") {
				return DBAcl::MetaGroup::None;
			} else if (str == "out") {
				return DBAcl::MetaGroup::Out;
			} else if (str == "strong") {
				return DBAcl::MetaGroup::Strong;
			} else if (str == "sub") {
				return DBAcl::MetaGroup::Sub;
			}

			// Not a meta group
			return std::nullopt;
		}

		std::string metaGroupName(DBAcl::MetaGroup group) {
			switch (group) {
				case DBAcl::MetaGroup::All:
					return "all";
				case DBAcl::MetaGroup::Auth:
					return "auth";
				case DBAcl::MetaGroup::In:
					return "in";
				case DBAcl::MetaGroup::None:
					return "none";
				case DBAcl::MetaGroup::Out:
					return "out";
				case DBAcl::MetaGroup::Strong:
					return "strong";
				case DBAcl::MetaGroup::Sub:
					return "sub";
			}

			assert(false);
			throw std::runtime_error("Reached supposedly unreachable code");
		}

		std::string getLegacyGroupData(const DBAcl &acl, GroupTable &groupTable) {
			std::string groupData;
			if (acl.affectedGroupID) {
				groupData = groupTable.getGroup(acl.serverID, acl.affectedGroupID.value()).name;
			} else if (acl.affectedMetaGroup) {
				// The main ChanACL class doesn't distinguish real groups from meta groups
				groupData = metaGroupName(acl.affectedMetaGroup.value());
			} else if (acl.accessToken) {
				// "Group names" containing the # modifier are understood to represent access tokens
				groupData = '#' + acl.accessToken.value();
			} else {
				return {};
			}

			// Potentially add group modifiers to group data
			for (const std::string &currentModifier : acl.groupModifiers) {
				assert(!currentModifier.empty());

				// All modifiers are prefix modifiers and should therefore simply be prepended to the group's name
				// The only exception is the syntax for the "sub" metagroup which takes optional trailing arguments
				// separated by commas.
				if (currentModifier[0] == ',') {
					groupData += currentModifier;
				} else {
					groupData = currentModifier + groupData;
				}
			}

			return groupData;
		}

	} // namespace db
} // namespace server
} // namespace mumble
