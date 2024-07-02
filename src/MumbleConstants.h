// Copyright 2021-2023 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLECONSTANTS_H_
#define MUMBLE_MUMBLECONSTANTS_H_

namespace Mumble {

constexpr unsigned int ROOT_CHANNEL_ID = 0;
constexpr unsigned int SUPERUSER_ID    = 0;

namespace Plugins {
	namespace PluginMessage {

		constexpr int MAX_DATA_LENGTH    = 1000;
		constexpr int MAX_DATA_ID_LENGTH = 100;

	} // namespace PluginMessage
} // namespace Plugins
} // namespace Mumble

#endif // MUMBLE_MUMBLECONSTANTS_H_
