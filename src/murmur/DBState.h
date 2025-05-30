// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MURMUR_DBSTATE_H_
#define MUMBLE_MURMUR_DBSTATE_H_

/**
 * Possible states the database can be in
 */
enum class DBState {
	Normal,
	ReadOnly,
};

#endif // MUMBLE_MURMUR_DBSTATE_H_
