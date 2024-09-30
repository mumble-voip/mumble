// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Version.h"

#include <QObject>
#include <QTest>

#include <limits>

class TestVersion : public QObject {
	Q_OBJECT
private slots:

	void legacy_encode() {
		QCOMPARE(Version::toLegacyVersion(Version::fromComponents(1, 3, 5)), static_cast< std::uint32_t >(0x010305));
	}

	void legacy_decode() { QCOMPARE(Version::fromLegacyVersion(0x010305), Version::fromComponents(1, 3, 5)); }

	void legacy_consistency() {
		QCOMPARE(Version::fromLegacyVersion(Version::toLegacyVersion(Version::fromComponents(1, 3, 5))),
				 Version::fromComponents(1, 3, 5));
	}

	void general_getMajor() {
		QCOMPARE(Version::getMajor(Version::fromComponents(1, 3, 5)), static_cast< Version::component_t >(1));
	}

	void general_getMinor() {
		QCOMPARE(Version::getMinor(Version::fromComponents(1, 3, 5)), static_cast< Version::component_t >(3));
	}

	void general_getPatch() {
		QCOMPARE(Version::getPatch(Version::fromComponents(1, 3, 5)), static_cast< Version::component_t >(5));
	}

	void v2_encode() { QCOMPARE(Version::fromComponents(1, 3, 5), static_cast< Version::full_t >(0x0001000300050000)); }

	void v2_decode() {
		Version::component_t major = 0, minor = 0, patch = 0;

		Version::getComponents(major, minor, patch);
		QCOMPARE(major, static_cast< Version::component_t >(MUMBLE_VERSION_MAJOR));
		QCOMPARE(minor, static_cast< Version::component_t >(MUMBLE_VERSION_MINOR));
		QCOMPARE(patch, static_cast< Version::component_t >(MUMBLE_VERSION_PATCH));

		Version::getComponents(major, minor, patch, "2.8.42");
		QCOMPARE(major, static_cast< Version::component_t >(2));
		QCOMPARE(minor, static_cast< Version::component_t >(8));
		QCOMPARE(patch, static_cast< Version::component_t >(42));
	}

	void v2_limits() {
		Version::component_t major, minor, patch;
		major = minor = patch = std::numeric_limits< Version::component_t >::max();

		Version::full_t version = Version::fromComponents(major, minor, patch);

		QCOMPARE(Version::getMajor(version), major);
		QCOMPARE(Version::getMinor(version), minor);
		QCOMPARE(Version::getPatch(version), patch);
	}

	void config_decoding() {
		// Version may be given as a string (probably most common)
		Version::full_t version = Version::fromConfig("1.8.266");

		QCOMPARE(version, Version::fromComponents(1, 8, 266));

		// Or as an integer (v2-encoded)
		version = Version::fromConfig(static_cast< qulonglong >(Version::fromComponents(2, 18, 42)));

		QCOMPARE(version, Version::fromComponents(2, 18, 42));

		// Or as an integer (v1-encoded)
		version = Version::fromConfig(Version::toLegacyVersion(Version::fromComponents(2, 18, 42)));

		QCOMPARE(version, Version::fromComponents(2, 18, 42));
	}
};

QTEST_MAIN(TestVersion)
#include "TestVersion.moc"
