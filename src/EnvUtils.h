// Copyright 2005-2017 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_ENVUTILS_H_
#define MUMBLE_MUMBLE_ENVUTILS_H_

#include <QString>

class EnvUtils {
public:
	// getEnvVariable is a wrapper around _wgetenv_s (on Windows)
	// and getenv (on everything else).
	//
	// On Windows, it expects a Unicode environment -- so variables
	// are expected to be UTF16.
	// On everything else, it expects the environment variables to be
	// UTF-8 encoded.
	static QString getenv(QString name);
};

#endif
