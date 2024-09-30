// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_CHANNELFILTERMODE_H_
#define MUMBLE_CHANNELFILTERMODE_H_

/// Visibility modifiers used when applying the channel filter
/// The channel the user is in will be always visible
/// This enum is used in the client DB, only append new entries
enum class ChannelFilterMode {
	/// The default channel filtering behavior
	NORMAL,
	/// Channel is filtered, if the channel filter is active
	HIDE,
	/// Channel is not filtered, even if it is empty
	PIN
};

#endif
