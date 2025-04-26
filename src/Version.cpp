// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Version.h"

#include <QObject>
#include <QRegularExpression>

namespace Version {

QString getRelease() {
	return MUMBLE_RELEASE;
}

Version::full_t fromString(const QString &version) {
	Version::component_t major, minor, patch;

	if (Version::getComponents(major, minor, patch, version)) {
		return Version::fromComponents(major, minor, patch);
	}

	return Version::UNKNOWN;
}

Version::full_t fromConfig(const QVariant &config) {
	Version::full_t version;

	bool ok                    = false;
	std::uint64_t integerValue = config.toULongLong(&ok);
	if (ok) {
		if ((integerValue >> Version::OFFSET_MAJOR) != 0) {
			// We assume this must be the new version format (v2), as a bit
			// after the 32nd is set.
			version = static_cast< Version::full_t >(integerValue);
		} else {
			version = Version::fromLegacyVersion(static_cast< std::uint32_t >(integerValue));
		}
	} else {
		// The config contains non-numeric characters -> We assume it contains a version string such as "1.5.0".
		// If this call fails, UNKNOWN is returned.
		version = Version::fromString(config.toString());
	}

	if (version == 0) {
		// 0 is not a valid value for a suggested version
		version = Version::UNKNOWN;
	}

	return version;
}

QString toString(Version::full_t v) {
	if (v == Version::UNKNOWN) {
		return QObject::tr("Unknown Version");
	}
	Version::component_t major, minor, patch;
	Version::getComponents(major, minor, patch, v);
	return QString::fromLatin1("%1.%2.%3").arg(major).arg(minor).arg(patch);
}

QString toConfigString(Version::full_t v) {
	if (v == Version::UNKNOWN) {
		return QString();
	}
	return Version::toString(v);
}

bool getComponents(Version::component_t &major, Version::component_t &minor, Version::component_t &patch,
				   const QString &version) {
	const QRegularExpression regex(
		QRegularExpression::anchoredPattern(QLatin1String("(\\d+)\\.(\\d+)\\.(\\d+)(?:\\.(\\d+))?")));
	const QRegularExpressionMatch match = regex.match(version);

	if (match.hasMatch()) {
		major = static_cast< Version::component_t >(match.captured(1).toInt());
		minor = static_cast< Version::component_t >(match.captured(2).toInt());
		patch = static_cast< Version::component_t >(match.captured(3).toInt());

		return true;
	}
	return false;
}

} // namespace Version
