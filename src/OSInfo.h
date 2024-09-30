// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_OSINFO_H_
#define MUMBLE_OSINFO_H_

#include <QtCore/QList>
#include <QtCore/QString>

class QDomDocument;
class QDomElement;
class QHostAddress;

class OSInfo {
public:
	/// If \p build is false, returns the operating system architecture.
	/// If \p build is true, returns the build architecture.
	static QString getArchitecture(const bool build);

	/// Returns an hash generated from the primary network interface's MAC address.
	static QString getMacHash(const QList< QHostAddress > & = QList< QHostAddress >());

	/// Returns the operating system's basename.
	/// Examples: "Windows", "Linux", "macOS", "FreeBSD", "Unknown", etc.
	static QString getOS();

	/// Returns the complete operating system version.
	///
	/// Examples:
	/// "Windows 10 Enterprise 2009 19042.804 [x64]"
	/// "Debian GNU/Linux bullseye/sid [x64]"
	///
	/// The architecture is not explicitly added when \p appendArch is false.
	static QString getOSDisplayableVersion(const bool appendArch = true);

	/// Returns the operating system's version.
	/// Examples: "10.0.19042.1", "10.16.0 20C69"
	static QString getOSVersion();

	static void fillXml(QDomDocument &doc, QDomElement &root, const QList< QHostAddress > & = {});
};

#endif
