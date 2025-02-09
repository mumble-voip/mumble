// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_PASSWORDGENERATOR_H_
#define MUMBLE_PASSWORDGENERATOR_H_

#include <QString>

class PasswordGenerator {
public:
	static QString generatePassword(int length);
};

#endif
