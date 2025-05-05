// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_ENVUTILS_H_
#define MUMBLE_MUMBLE_ENVUTILS_H_

#include <QString>

namespace EnvUtils {

// getenv is a wrapper around _wgetenv_s (on Windows)
// and getenv (on everything else).
//
// On Windows, it expects a Unicode environment -- so variables
// are expected to be UTF16.
//
// On everything else, it expects environment variables to use the
// locale-defined encoding. (From a Qt-perspective, we use toLocal8Bit/fromLocal8Bit.)
QString getenv(QString name);

bool setenv(QString name, QString value);

bool waylandIsUsed();

} // namespace EnvUtils

#endif
