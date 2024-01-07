// Copyright 2021-2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "IPCUtils.h"
extern "C" {
#include "../../overlay/ipc_utils.h"
}

#include <QDir>
#include <QProcessEnvironment>
#include <QString>

namespace Mumble {
const std::string getRuntimePath(void) {
	char *c_mumbleRuntimePath = getRuntimePath__();
	if (c_mumbleRuntimePath == NULL)
		return "";
	std::string mumbleRuntimePath = c_mumbleRuntimePath;
	std::free(c_mumbleRuntimePath);
	return mumbleRuntimePath;
}

const std::string getAndCreateOverlayPipePath(void) {
	char *c_pipepath = getAndCreateOverlayPipePath__();
	if (c_pipepath == NULL)
		return "";
	std::string pipepath = c_pipepath;
	std::free(c_pipepath);
	return pipepath;
}

const std::string getAndCreateSocketPath(const std::string &basename) {
	const std::string mumbleRuntimePath = getRuntimePath();
#ifdef Q_OS_WIN
	return basename;
#endif
	if (mumbleRuntimePath.empty())
		return "";
	QString qMumbleRuntimePath = QString::fromUtf8(mumbleRuntimePath.data(), int(mumbleRuntimePath.size()));
	QDir(qMumbleRuntimePath).mkpath(".");
#ifdef Q_OS_LINUX
	std::string pipepath = mumbleRuntimePath + basename + "Socket";
#else // MAC or BSD
	const std::string pipepath = mumbleRuntimePath + "." + basename + "Socket";
#endif
	return pipepath;
}

}; // namespace Mumble
