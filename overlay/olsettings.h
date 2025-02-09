// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_OVERLAY_OLSETTINGS_H_
#define MUMBLE_OVERLAY_OLSETTINGS_H_

#include <string>
#include <vector>

/// OverlayExclusionMode defines the overlay exclusion
/// rules for Mumble.
enum OverlayExclusionMode {
	/// Use the launcher filter-based exclusion mode.
	LauncherFilterExclusionMode,
	/// Use only the program whitelist.
	WhitelistExclusionMode,
	/// Use only the program blacklist.
	BlacklistExclusionMode
};

/// Get the configured overlay exclusion mode for Mumble.
OverlayExclusionMode SettingsGetExclusionMode();

/// Get the list of launcher programs to consider when
/// checking the overlay exclusion rules.
std::vector< std::string > SettingsGetLaunchers();

/// Get the list of whitelisted programs to consider when
/// checking the overlay exclusion rules.
std::vector< std::string > SettingsGetWhitelist();

/// Get the list of whitelisted paths to consider when
/// checking the overlay exclusion rules.
std::vector< std::string > SettingsGetPaths();

/// Get the list of blacklisted programs to consider when
/// checking the overlay exclusion rules.
std::vector< std::string > SettingsGetBlacklist();

#endif
