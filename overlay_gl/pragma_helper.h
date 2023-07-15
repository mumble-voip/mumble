// Copyright 2023 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_PRAGMA_HELPER_H
#define MUMBLE_PRAGMA_HELPER_H

#define DO_PRAGMA(x) _Pragma (#x)
#define PUSH_PRAGMA(type, additional) \
		DO_PRAGMA(GCC diagnostic push) \
		DO_PRAGMA(GCC diagnostic type additional)

#define PUSH_PRAGMA_WARNING(x) \
        PUSH_PRAGMA(warning, x)
#define POP_PRAGMA() \
	DO_PRAGMA(GCC diagnostic pop)

#endif // MUMBLE_PRAGMA_HELPER_H
