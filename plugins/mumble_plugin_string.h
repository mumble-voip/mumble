// Copyright 2005-2017 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_PLUGIN_STRING_H_
#define MUMBLE_MUMBLE_PLUGIN_STRING_H_

#include <string>
#include <cstring> // For memset

typedef struct _MumbleString {
        unsigned char *data;
        size_t len;
} MumbleString;

typedef struct _MumbleWideString {
        wchar_t *data;
        size_t len;
} MumbleWideString;

#define MumbleInitConstWideString(cs) \
        { cs, sizeof(cs)/sizeof(cs[0]) }

static inline void MumbleStringAssign(MumbleString *str, const std::string &stdstr) {
        if (str != NULL && str->data != NULL) {
                memset(str->data, 0, str->len);

                size_t nc = stdstr.size();
                if (nc > str->len - 1) {
                        nc = str->len - 1;
                }

                memcpy(str->data, stdstr.c_str(), nc);
        }
}

static inline void MumbleStringAssign(MumbleWideString *wstr, const std::wstring &stdwstr) {
        if (wstr != NULL && wstr->data != NULL) {
                memset(wstr->data, 0, wstr->len * sizeof(wchar_t));

                size_t nc = stdwstr.size() * sizeof(wchar_t);
                if (nc > (wstr->len * sizeof(wchar_t)) - sizeof(wchar_t)) {
                        nc = (wstr->len * sizeof(wchar_t)) - sizeof(wchar_t);
                }

                memcpy(wstr->data, stdwstr.c_str(), nc);
        }
}

static inline void MumbleStringClear(MumbleString *str) {
        if (str != NULL && str->data != NULL) {
                memset(str->data, 0, str->len);
        }
}

static inline void MumbleStringClear(MumbleWideString *wstr) {
        if (wstr != NULL && wstr->data != NULL) {
                memset(wstr->data, 0, wstr->len * sizeof(wchar_t));
        }
} 

#endif
