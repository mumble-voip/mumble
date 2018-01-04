// Copyright 2005-2018 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "murmur_pch.h"

#include <QtCore/QLibrary>

#include "PlatformCheck.h"

bool PlatformCheck::IsWine() {
#ifdef Q_OS_WIN
	if (QLibrary::resolve(QLatin1String("ntdll.dll"), "wine_get_version") != NULL) {
		return true;
	}
#endif
	return false;
}
