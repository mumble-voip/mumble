// Copyright 2005-2016 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_PLUGIN_UTILS_H_
#define MUMBLE_MUMBLE_PLUGIN_UTILS_H_

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
static void escape(char *str) {
    char *c = str;

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
        if (*c < 32) {
            *c = ' ';
        } else if (*c > 126) {
            *c = ' ';
        }

        c += 1;
    }
}

#endif
