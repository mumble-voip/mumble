// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_VERSION_H_
#define MUMBLE_VERSION_H_

#include <cstdint>
#include <limits>

#include <QString>
#include <QVariant>

#define MUMXTEXT(X) #X
#define MUMTEXT(X) MUMXTEXT(X)

#define MUMBLE_RELEASE MUMTEXT(MUMBLE_VERSION)

namespace Version {

enum class CompareMode { AtLeast, LessThan };

//
// The mumble version format (v2) is a uint64:
// major   minor   patch   reserved/unused
// 0xFFFF  0xFFFF  0xFFFF  0xFFFF
// (big-endian)
//

using full_t      = std::uint64_t;
using component_t = std::uint16_t;

constexpr full_t OFFSET_MAJOR  = 48;
constexpr full_t OFFSET_MINOR  = 32;
constexpr full_t OFFSET_PATCH  = 16;
constexpr full_t OFFSET_UNUSED = 0;
constexpr full_t FIELD_MASK    = 0xFFFF;
constexpr full_t FIELD_MAJOR   = (FIELD_MASK << Version::OFFSET_MAJOR);
constexpr full_t FIELD_MINOR   = (FIELD_MASK << Version::OFFSET_MINOR);
constexpr full_t FIELD_PATCH   = (FIELD_MASK << Version::OFFSET_PATCH);
constexpr full_t FIELD_UNUSED  = (FIELD_MASK << Version::OFFSET_UNUSED);

constexpr full_t UNKNOWN = 0;

QString getRelease();
full_t fromString(const QString &version);
full_t fromConfig(const QVariant &config);
QString toString(full_t version);
QString toConfigString(full_t version);
bool getComponents(Version::component_t &major, Version::component_t &minor, Version::component_t &patch,
				   const QString &version);

constexpr Version::component_t getMajor(Version::full_t version) {
	return static_cast< Version::component_t >((version & Version::FIELD_MAJOR) >> Version::OFFSET_MAJOR);
}

constexpr Version::component_t getMinor(Version::full_t version) {
	return static_cast< Version::component_t >((version & Version::FIELD_MINOR) >> Version::OFFSET_MINOR);
}

constexpr Version::component_t getPatch(Version::full_t version) {
	return static_cast< Version::component_t >((version & Version::FIELD_PATCH) >> Version::OFFSET_PATCH);
}

constexpr Version::full_t fromComponents(Version::component_t major, Version::component_t minor,
										 Version::component_t patch) {
	return ((static_cast< full_t >(major) << Version::OFFSET_MAJOR)
			| (static_cast< full_t >(minor) << Version::OFFSET_MINOR)
			| (static_cast< full_t >(patch) << Version::OFFSET_PATCH));
}

constexpr full_t get() {
	return fromComponents(MUMBLE_VERSION_MAJOR, MUMBLE_VERSION_MINOR, MUMBLE_VERSION_PATCH);
}

constexpr void getComponents(Version::component_t &major, Version::component_t &minor, Version::component_t &patch,
							 Version::full_t version = get()) {
	major = Version::getMajor(version);
	minor = Version::getMinor(version);
	patch = Version::getPatch(version);
}

//
// Legacy versions: These versions are kept around for backward compatibility, but
// have since been replaced by other version formats.
//
// Mumble legacy version format (v1) is a uint32:
// major   minor  patch
// 0xFFFF  0xFF   0xFF
// (big-endian)
//

constexpr Version::full_t fromLegacyVersion(std::uint32_t version) {
	return fromComponents(static_cast< component_t >((version & 0xFFFF0000) >> 16),
						  static_cast< component_t >((version & 0xFF00) >> 8),
						  static_cast< component_t >(version & 0xFF));
}

constexpr std::uint32_t toLegacyVersion(Version::full_t version) {
	// If any of the version components exceeds their allowed value range, they will
	// be truncated to the highest representable value
	return ((std::min(static_cast< std::uint32_t >(Version::getMajor(version)),
					  static_cast< std::uint32_t >(std::numeric_limits< std::uint16_t >::max()))
			 << 16)
			| (std::min(static_cast< std::uint32_t >(Version::getMinor(version)),
						static_cast< std::uint32_t >(std::numeric_limits< std::uint8_t >::max()))
			   << 8)
			| std::min(static_cast< std::uint32_t >(Version::getPatch(version)),
					   static_cast< std::uint32_t >(std::numeric_limits< std::uint8_t >::max())));
}

} // namespace Version

#endif
