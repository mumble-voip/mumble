// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_PROTOUTILS_H_
#define MUMBLE_PROTOUTILS_H_

#include "Mumble.pb.h"
#include "Version.h"

namespace MumbleProto {

::Version::full_t getVersion(const MumbleProto::Version &msg);
void setVersion(MumbleProto::Version &msg, const ::Version::full_t version);

::Version::full_t getSuggestedVersion(const MumbleProto::SuggestConfig &msg);
void setSuggestedVersion(MumbleProto::SuggestConfig &msg, const ::Version::full_t version);

} // namespace MumbleProto

#endif // MUMBLE_PROTOUTILS_H_
