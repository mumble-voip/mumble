// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "License.h"

#include "licenses.h"

QString License::license() {
	return QString::fromUtf8(licenseMumble);
}

QList< LicenseInfo > License::thirdPartyLicenses() {
	QList< LicenseInfo > licenses;
	for (int i = 0; !licenses3rdParties[i].isEmpty(); i++) {
		const ThirdPartyLicense *tpl = &licenses3rdParties[i];
		LicenseInfo li;
		li.name    = QString::fromUtf8(tpl->name);
		li.url     = QString::fromUtf8(tpl->url);
		li.license = QString::fromUtf8(tpl->license);
		licenses << li;
	}
	return licenses;
}

QString License::printableThirdPartyLicenseInfo() {
	QString output;

	QList< LicenseInfo > thirdPartyLicenses = License::thirdPartyLicenses();
	foreach (LicenseInfo li, thirdPartyLicenses) {
		QString header          = QString::fromLatin1("%1 (%2)\n").arg(li.name).arg(li.url);
		QString headerHorizLine = QString::fromLatin1("-").repeated(header.size()) + QLatin1String("\n");

		output.append(headerHorizLine);
		output.append(header);
		output.append(headerHorizLine);
		output.append(QLatin1String("\n"));
		output.append(li.license);
		output.append(QLatin1String("\n\n"));
	}

	return output;
}
