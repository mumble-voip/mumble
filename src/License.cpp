// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "License.h"

#include "licenses.h"

#include <QString>

#include <vector>

QString License::license() {
	return QString::fromUtf8(mumbleLicense);
}

std::vector< LicenseInfo > License::thirdPartyLicenses() {
	std::vector< LicenseInfo > licenses;
	for (const ThirdPartyLicense &current : licenses3rdParties) {
		LicenseInfo li;
		li.name      = QString::fromUtf8(current.name);
		li.url       = QString::fromUtf8(current.url);
		li.licenseId = QString::fromUtf8(current.spdxLicenseId);

		licenses.emplace_back(std::move(li));
	}
	return licenses;
}

QString License::printableThirdPartyLicenseInfo() {
	QString output;

	for (const LicenseInfo &li : License::thirdPartyLicenses()) {
		output += "- ";
		output.append(li.name);
		output += " (";
		output.append(li.licenseId);
		output += ") - ";
		output.append(li.url);
		output += "\n";
	}

	return output;
}
