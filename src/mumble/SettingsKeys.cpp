// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "SettingsKeys.h"

SettingsKey::SettingsKey(std::initializer_list< std::string > keyIDs) : m_ids(keyIDs.begin(), keyIDs.end()) {
	if (keyIDs.size() == 0) {
		// Don't use std::exception here in order to increase the chance of this "exception" remaining uncaught and
		// therefore crashing the application (if this happens, this is a non-recoverable programmer error).
		throw "SettingsKey instances must at least contain one ID!";
	}
}

SettingsKey::operator const char *() const {
	// The first entry is considered to be the most recent
	return m_ids[0].c_str();
}

SettingsKey::operator nlohmann::json::object_t::key_type() const {
	// The first entry is considered to be the most recent
	return m_ids[0].c_str();
}

nlohmann::json SettingsKey::selectFrom(const nlohmann::json &json) const {
	// Start with the most recent ID and go through to the oldest one. The first entry with a given ID, that exists,
	// will be selected.
	for (std::size_t i = 0; i < m_ids.size(); ++i) {
		if (json.contains(m_ids[i])) {
			return json[m_ids[i]];
		}
	}

	// Not found
	return {};
}
