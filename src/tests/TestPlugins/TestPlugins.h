// Copyright 2005-2018 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef TESTS_TESTPLUGINS_H
#define TESTS_TESTPLUGINS_H

#include "mumble_plugin.h"

#include <QLibrary>

struct PluginInfo {
	bool tested;
	QLibrary lib;
	QString shortname;
	QString description;
	MumblePlugin *p;
	MumblePlugin2 *p2;
};

#endif // TESTS_TESTPLUGINS_H
