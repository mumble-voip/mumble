// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_SERVER_LEGACYPASSWORDHASH_HPP_
#define MUMBLE_SERVER_LEGACYPASSWORDHASH_HPP_

#include <string>

class QString;

/**
 * @returns The legacy hash of the provided password.
 */
QString getLegacyPasswordHash(const QString &password);

QString getLegacyPasswordHash(const std::string &password);

#endif // MUMBLE_SERVER_LEGACYPASSWORDHASH_HPP_
