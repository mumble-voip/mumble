// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_LICENSE_H_
#define MUMBLE_LICENSE_H_

#include <QList>
#include <QString>

/// LicenseInfo represents license information
/// for a third-party library used by Mumble.
struct LicenseInfo {
	/// The name of the third-party library.
	QString name;
	/// A URL for the website of the third-party library.
	QString url;
	/// The license text for the third-party library.
	QString license;
};

/// The License class contains static helpers for
/// getting Mumble license/author information.
class License {
public:
	/// Get the Mumble license text (the LICENSE file)
	static QString license();
	/// Get a list of license information for Mumble's
	/// third-party libraries.
	static QList< LicenseInfo > thirdPartyLicenses();
	/// Get a human-readable, ready-to-print combination
	/// of all of Mumble's third-party license information.
	static QString printableThirdPartyLicenseInfo();
};

#endif
