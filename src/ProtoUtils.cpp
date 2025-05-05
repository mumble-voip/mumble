// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ProtoUtils.h"

namespace MumbleProto {

::Version::full_t getVersion(const MumbleProto::Version &msg) {
	if (msg.has_version_v2()) {
		return static_cast<::Version::full_t >(msg.version_v2());
	} else if (msg.has_version_v1()) {
		return ::Version::fromLegacyVersion(msg.version_v1());
	}

	return ::Version::UNKNOWN;
}

void setVersion(MumbleProto::Version &msg, const ::Version::full_t version) {
	msg.set_version_v2(version);
	msg.set_version_v1(::Version::toLegacyVersion(version));
}

::Version::full_t getSuggestedVersion(const MumbleProto::SuggestConfig &msg) {
	if (msg.has_version_v2()) {
		return static_cast<::Version::full_t >(msg.version_v2());
	} else if (msg.has_version_v1()) {
		return ::Version::fromLegacyVersion(msg.version_v1());
	}

	return ::Version::UNKNOWN;
}

void setSuggestedVersion(MumbleProto::SuggestConfig &msg, const ::Version::full_t version) {
	msg.set_version_v2(version);
	msg.set_version_v1(::Version::toLegacyVersion(version));
}

} // namespace MumbleProto
