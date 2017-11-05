// Copyright 2005-2017 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_PLUGIN_STRING_H_
#define MUMBLE_MUMBLE_PLUGIN_STRING_H_

#include <string>
#include <algorithm>
#include <assert.h>
#include <cstring> // For memset

/// String allowing std::string like semantics (minus auto-growth) over a C interface
/// std::string like means this is not a zero terminated C string. The len fi
typedef struct _MumbleString {
	unsigned char *data; /// Pointer to the string characters (not guaranteed to be zero terminated)
	size_t len; /// Number of characters actually stored in the string
	size_t writableLenght; /// Size of the linear writable space pointed to by data. 0 for read-only.
} MumbleString;

typedef struct _MumbleWideString {
	wchar_t *data; /// Pointer to the string characters (not guaranteed to be zero terminated)
	size_t len; /// Number of characters actually stored in the string
	size_t writableLenght; /// Size of the linear writable space pointed to by data. 0 for read-only.
} MumbleWideString;

static inline void MumbleStringAssign(MumbleString *str, const std::string &stdstr) {
	const bool writableString = str != NULL && str->data != NULL && str->writableLenght > 0;
	assert(writableString);
	if (writableString) {
		const size_t nc = std::min(str->writableLenght, stdstr.size());
		memcpy(str->data, stdstr.data(), nc);
		str->len = nc;
	}
}

static inline void MumbleStringAssign(MumbleWideString *wstr, const std::wstring &stdwstr) {
	const bool writableString = wstr != NULL && wstr->data != NULL && wstr->writableLenght > 0;
	assert(writableString);
	if (writableString) {
		const size_t nc = std::min(wstr->writableLenght, stdwstr.size());
		memcpy(wstr->data, stdwstr.data(), nc * sizeof(wchar_t));
		wstr->len = nc;
	}
}

static inline void MumbleStringClear(MumbleString *str) {
	assert(str != NULL);
	if (str != NULL) {
		str->len = 0;
	}
}

static inline void MumbleStringClear(MumbleWideString *wstr) {
	assert(wstr != NULL);
	if (wstr != NULL) {
		wstr->len = 0;
	}
}

static inline std::string MumbleStringToStdString(const MumbleString &str) {
	assert(str.data != NULL);
	if (str.data != NULL) {
		return std::string(reinterpret_cast<const char*>(str.data), str.len);
	}
	return std::string();
}

static inline std::wstring MumbleStringToStdWString(const MumbleWideString &wstr) {
	assert(wstr.data != NULL);
	if (wstr.data != NULL) {
		return std::wstring(wstr.data, wstr.len);
	}
	return std::wstring();
}

#endif
