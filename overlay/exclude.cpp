// Copyright 2005-2017 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "lib.h" // include lib.h for Windows headers...

#include <algorithm>
#include <string> 
#include <vector>
#include <iterator>

#include "overlay_blacklist.h"
#include "overlay_whitelist.h"
#include "overlay_launchers.h"

static std::vector<std::string> defaultBlacklistVector() {
	std::vector<std::string> out;
	size_t i = 0;
	while (overlayBlacklist[i] != NULL) {
		out.push_back(std::string(overlayBlacklist[i]));
		i++;
	}
	return out;
}

static std::vector<std::string> defaultWhitelistVector() {
	std::vector<std::string> out;
	size_t i = 0;
	while (overlayWhitelist[i] != NULL) {
		out.push_back(std::string(overlayWhitelist[i]));
		i++;
	}
	return out;
}

static std::vector<std::string> defaultLaunchersVector() {
	std::vector<std::string> out;
	size_t i = 0;
	while (overlayLaunchers[i] != NULL) {
		out.push_back(std::string(overlayLaunchers[i]));
		i++;
	}
	return out;
}

static std::vector<std::string> regReadMultiString(HKEY key,
                                                   const std::string &subKey,
                                                   const std::string &valueName)
{
	LONG err = 0;
	std::vector<std::string> out;
	char *buf = NULL;

	err = RegOpenKeyExA(key, subKey.c_str(), NULL, KEY_READ, &key);
	if (err != ERROR_SUCCESS) {
		goto err;
	}

	DWORD sz = 0;
	DWORD type = 0;
	err = RegQueryValueExA(key, valueName.c_str(), NULL, &type, NULL, &sz);
	if (err != ERROR_SUCCESS) {
		goto err;
	}

	if (type != REG_MULTI_SZ) {
		goto err;
	}

	// If the multisz is longer than 4MB, treat it as an error.
	if (sz > 4*1024*1024) {
		goto err;
	}

	buf = reinterpret_cast<char *>(malloc(sz));
	if (buf == NULL) {
		goto err;
	}

	size_t begin = 0;
	for (size_t i = 0; i < sz; i++) {
		if (buf[i] == 0) {
			size_t len = i - begin;
			if (len == 0) {
				break;
			}
			std::string s(&buf[begin], len);
			out.push_back(s);
			begin = i + 1;
			continue;
		}
	}

err:
	free(buf);
	return out;
}

int ExcludeGetMode() {
	LONG err = 0;
	HKEY key = NULL;
	DWORD mode = -1;

	err = RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Mumble\\Mumble\\overlay", NULL, KEY_READ, &key);
	if (err != ERROR_SUCCESS) {
		return -1;
	}

	DWORD sz = sizeof(mode);
	err = RegQueryValueExA(key, "mode", NULL, NULL, reinterpret_cast<BYTE *>(&mode), &sz);
	if (err != ERROR_SUCCESS) {
		return -1;
	}
	if (sz != sizeof(mode)) {
		return -1;
	}

	return static_cast<int>(mode);
}	

static std::string slowercase(std::string s) {
	std::transform(s.begin(), s.end(), s.begin(), tolower);
	return s;
}

static std::vector<std::string> vlowercase(std::vector<std::string> vec) {
	std::transform(vec.begin(), vec.end(), vec.begin(), slowercase);
	return vec;

}

static std::vector<std::string> vmerge(std::vector<std::string> v1, const std::vector<std::string> &v2) {
	v1.insert(v1.end(), v2.begin(), v2.end());
	return v1;
}

static std::vector<std::string> vexclude(std::vector<std::string> v, std::vector<std::string> vremove) {
	std::vector<std::string> out;
	std::set_difference(v.begin(), v.end(), vremove.begin(), vremove.end(), std::inserter(out, out.begin()));	
	return out;
}

std::vector<std::string> ExcludeGetLaunchers() {
	std::vector<std::string> defaultLaunchers = vlowercase(defaultLaunchersVector());
	std::vector<std::string> userLaunchers = vlowercase(regReadMultiString(HKEY_CURRENT_USER, "Software\\Mumble\\Mumble\\overlay", "launchers"));
	std::vector<std::string> userExcludedLaunchers = vlowercase(regReadMultiString(HKEY_CURRENT_USER, "Software\\Mumble\\Mumble\\overlay", "launchersexclude"));
	return vexclude(vmerge(defaultLaunchers, userLaunchers), userExcludedLaunchers);
}

std::vector<std::string> ExcludeGetWhitelist() {
	std::vector<std::string> defaultWhitelist = vlowercase(defaultWhitelistVector());
	std::vector<std::string> userWhitelist = vlowercase(regReadMultiString(HKEY_CURRENT_USER, "Software\\Mumble\\Mumble\\overlay", "whitelist"));
	std::vector<std::string> userExcludedWhitelistEntries = vlowercase(regReadMultiString(HKEY_CURRENT_USER, "Software\\Mumble\\Mumble\\overlay", "whitelistexclude"));
	return vexclude(vmerge(defaultWhitelist, userWhitelist), userExcludedWhitelistEntries);
}

std::vector<std::string> ExcludeGetPaths() {
	std::vector<std::string> defaultPaths;
	std::vector<std::string> userPaths = vlowercase(regReadMultiString(HKEY_CURRENT_USER, "Software\\Mumble\\Mumble\\overlay", "paths"));
	std::vector<std::string> userExcludedPaths = vlowercase(regReadMultiString(HKEY_CURRENT_USER, "Software\\Mumble\\Mumble\\overlay", "pathsexclude"));
	return vexclude(vmerge(defaultPaths, userPaths), userExcludedPaths);
}

std::vector<std::string> ExcludeGetBlacklist() {
	std::vector<std::string> defaultBlacklist = vlowercase(defaultBlacklistVector());
	std::vector<std::string> userBlacklist = vlowercase(regReadMultiString(HKEY_CURRENT_USER, "Software\\Mumble\\Mumble\\overlay", "paths"));
	std::vector<std::string> userExcludedBlacklistEntries = vlowercase(regReadMultiString(HKEY_CURRENT_USER, "Software\\Mumble\\Mumble\\overlay", "pathsexclude"));
	return vexclude(vmerge(defaultBlacklist, userBlacklist), userExcludedBlacklistEntries);
}
