// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "olsettings.h"

#include "lib.h" // include lib.h for Windows headers...
#include "util.h"

#include <algorithm>
#include <iterator>
#include <string>
#include <vector>

#include "overlay_blacklist.h"
#include "overlay_launchers.h"
#include "overlay_whitelist.h"

// Get the default blacklist (from overlay_blacklist.h) as a string vector.
static std::vector< std::string > defaultBlacklistVector() {
	std::vector< std::string > out;
	size_t i = 0;
	while (overlayBlacklist[i]) {
		out.push_back(std::string(overlayBlacklist[i]));
		i++;
	}
	return out;
}

// Get the default whitelist (from overlay_whitelist.h) as a string vector.
static std::vector< std::string > defaultWhitelistVector() {
	std::vector< std::string > out;
	size_t i = 0;
	while (overlayWhitelist[i]) {
		out.push_back(std::string(overlayWhitelist[i]));
		i++;
	}
	return out;
}

// Get the default launcher list (from overlay_launchers.h) as a string vector.
static std::vector< std::string > defaultLaunchersVector() {
	std::vector< std::string > out;
	size_t i = 0;
	while (overlayLaunchers[i]) {
		out.push_back(std::string(overlayLaunchers[i]));
		i++;
	}
	return out;
}

// Read a REG_MULTI_SZ value from the Windows registry and return it as a string vector.
// Returns an empty vector on failure.
static std::vector< std::string > regReadMultiString(HKEY key, const std::string &subKey,
													 const std::string &valueName) {
	LONG err = 0;
	std::vector< std::string > out;
	char *buf         = nullptr;
	HKEY subKeyHandle = 0;

	err = RegOpenKeyExA(key, subKey.c_str(), 0, KEY_READ, &subKeyHandle);
	if (err != ERROR_SUCCESS) {
		goto err;
	}

	DWORD sz   = 0;
	DWORD type = 0;
	err        = RegQueryValueExA(subKeyHandle, valueName.c_str(), nullptr, &type, nullptr, &sz);
	if (err != ERROR_SUCCESS) {
		goto err;
	}

	if (type != REG_MULTI_SZ) {
		goto err;
	}

	// If the size is longer than 4MB, treat it as an error.
	if (sz > 4 * 1024 * 1024) {
		goto err;
	}

	buf = reinterpret_cast< char * >(malloc(sz));
	if (!buf) {
		goto err;
	}

	err = RegQueryValueExA(subKeyHandle, valueName.c_str(), nullptr, &type, reinterpret_cast< BYTE * >(buf), &sz);
	if (err != ERROR_SUCCESS) {
		goto err;
	}

	size_t begin = 0;
	for (size_t i = 0; i < sz; i++) {
		if (buf[i] == 0) {
			size_t len = i - begin;
			if (len > 0) {
				std::string s(&buf[begin], len);
				out.push_back(s);
			}
			begin = i + 1;
		}
	}

err:
	free(buf);
	return out;
}

// Get the Mumble client's configured overlay exclusion mode as an integer.
// Returns -1 if the function could not read the value from the Windows registry.
static int getModeInternal() {
	LONG err   = 0;
	HKEY key   = nullptr;
	DWORD mode = -1;

	err = RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Mumble\\Mumble\\overlay", 0, KEY_READ, &key);
	if (err != ERROR_SUCCESS) {
		return -1;
	}

	DWORD sz = sizeof(mode);
	err      = RegQueryValueExA(key, "mode", nullptr, nullptr, reinterpret_cast< BYTE * >(&mode), &sz);
	if (err != ERROR_SUCCESS) {
		return -1;
	}
	if (sz != sizeof(mode)) {
		return -1;
	}

	return static_cast< int >(mode);
}

OverlayExclusionMode SettingsGetExclusionMode() {
	int mode = getModeInternal();
	if (mode == -1) {
		// If no exclusion mode is set in the registry,
		// use the launcher filter.
		return LauncherFilterExclusionMode;
	}
	return static_cast< OverlayExclusionMode >(mode);
}

std::vector< std::string > SettingsGetLaunchers() {
	std::vector< std::string > defaultLaunchers = vlowercase(defaultLaunchersVector());
	std::vector< std::string > userLaunchers =
		vlowercase(regReadMultiString(HKEY_CURRENT_USER, "Software\\Mumble\\Mumble\\overlay", "launchers"));
	std::vector< std::string > userExcludedLaunchers =
		vlowercase(regReadMultiString(HKEY_CURRENT_USER, "Software\\Mumble\\Mumble\\overlay", "launchersexclude"));
	std::vector< std::string > actualExcludedLaunchers = vintersect(defaultLaunchers, userExcludedLaunchers);
	return vexclude(vmerge(defaultLaunchers, userLaunchers), actualExcludedLaunchers);
}

std::vector< std::string > SettingsGetWhitelist() {
	std::vector< std::string > defaultWhitelist = vlowercase(defaultWhitelistVector());
	// We don't consider Mumble's built-in whitelist when in WhitelistExclusionMode.
	// The built-in whitelist is only used in LauncherFilterExclusionMode.
	if (SettingsGetExclusionMode() == WhitelistExclusionMode) {
		defaultWhitelist = std::vector< std::string >();
	}
	std::vector< std::string > userWhitelist =
		vlowercase(regReadMultiString(HKEY_CURRENT_USER, "Software\\Mumble\\Mumble\\overlay", "whitelist"));
	std::vector< std::string > userExcludedWhitelistEntries =
		vlowercase(regReadMultiString(HKEY_CURRENT_USER, "Software\\Mumble\\Mumble\\overlay", "whitelistexclude"));
	std::vector< std::string > actualExcludedWhitelistEntries =
		vintersect(defaultWhitelist, userExcludedWhitelistEntries);
	return vexclude(vmerge(defaultWhitelist, userWhitelist), actualExcludedWhitelistEntries);
}

std::vector< std::string > SettingsGetPaths() {
	std::vector< std::string > defaultPaths;
	std::vector< std::string > userPaths =
		vlowercase(regReadMultiString(HKEY_CURRENT_USER, "Software\\Mumble\\Mumble\\overlay", "paths"));
	std::vector< std::string > userExcludedPaths =
		vlowercase(regReadMultiString(HKEY_CURRENT_USER, "Software\\Mumble\\Mumble\\overlay", "pathsexclude"));
	std::vector< std::string > actualExcludedPaths = vintersect(defaultPaths, userExcludedPaths);
	return vexclude(vmerge(defaultPaths, userPaths), actualExcludedPaths);
}

std::vector< std::string > SettingsGetBlacklist() {
	std::vector< std::string > defaultBlacklist = vlowercase(defaultBlacklistVector());
	std::vector< std::string > userBlacklist =
		vlowercase(regReadMultiString(HKEY_CURRENT_USER, "Software\\Mumble\\Mumble\\overlay", "blacklist"));
	std::vector< std::string > userExcludedBlacklistEntries =
		vlowercase(regReadMultiString(HKEY_CURRENT_USER, "Software\\Mumble\\Mumble\\overlay", "blacklistexclude"));
	std::vector< std::string > actualExcludedPaths = vintersect(defaultBlacklist, userExcludedBlacklistEntries);
	return vexclude(vmerge(defaultBlacklist, userBlacklist), actualExcludedPaths);
}
