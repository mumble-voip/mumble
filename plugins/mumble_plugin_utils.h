// Copyright 2005-2020 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_PLUGIN_UTILS_H_
#define MUMBLE_MUMBLE_PLUGIN_UTILS_H_

#include <codecvt>
#include <locale>

static inline std::string utf16ToUtf8(const std::wstring &wstr) {
	std::wstring_convert< std::codecvt_utf8_utf16< wchar_t > > conv;
	return conv.to_bytes(wstr);
}

// escape lossily converts the given
// string to ASCII, replacing any
// character not within the printable
// ASCII region (32-126) with an ASCII
// space character.
//
// escape also replaces any double quote
// characters with an ASCII space. This
// allows the string to be safely used
// when constructing JSON documents via
// string concatenation.
//
// Finally, escape ensures that the given
// string is NUL-terminated by always
// setting the last byte of the input
// string to the value 0.
static inline void escape(char *str, const size_t &size) {
	// Ensure the input string is properly NUL-terminated.
	str[size - 1] = 0;
	char *c       = str;

	while (*c != '\0') {
		// For JSON compatibility, the string
		// can't contain double quotes.
		// If a double quote is found, replace
		// it with an ASCII space.
		if (*c == '"') {
			*c = ' ';
		}

		// Ensure the string is within printable
		// ASCII. If not, replace the offending
		// byte with an ASCII space.
		if (*c < 32 || *c > 126) {
			*c = ' ';
		}

		c += 1;
	}
}

#endif
