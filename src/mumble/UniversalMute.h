// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_UNIVERSAL_MUTE_H_
#define MUMBLE_MUMBLE_UNIVERSAL_MUTE_H_

#include <functional>
#include <memory>
#include <string>

// A wrapper around the Windows 10 VoIP Call API, specifically enough to
// interact with the Windows 11 Universal Mute feature. This ensures physical mute buttons
// in recent laptops can mute Mumble.
//
// Universal Mute is only supported on Windows 11 22H2 and later, but this class
// gracefully no-ops on unsupported platforms.
//
// https://stackoverflow.com/questions/74683703/how-do-i-support-call-mute-universal-mute-in-my-app-for-windows-11-22h2
class UniversalMuter {
public:
	// The onMuted/onUnmuted callbacks are called when the user mutes/unmutes themselves using
	// the Universal Mute button. They must call setMuted()/setUnmuted() here when they have
	// processed the event, or the button won't actually change state.
	UniversalMuter(std::function< void() > onMuted, std::function< void() > onUnmuted);
	~UniversalMuter();

	UniversalMuter(const UniversalMuter &)            = delete;
	UniversalMuter &operator=(const UniversalMuter &) = delete;
	UniversalMuter(UniversalMuter &&)                 = delete;
	UniversalMuter &operator=(UniversalMuter &&)      = delete;

	void setMuted();
	void setUnmuted();

	void startCall(const std::wstring &contactName, const std::wstring &serviceName);
	void tryEndCall();

	void trySetCallName(const std::wstring &callName);

private:
	// Use the PIMPL pattern to avoid including WRL/WinRT headers in the public header.
	struct Impl;
	std::shared_ptr< Impl > m_impl;
};

#endif // MUMBLE_MUMBLE_UNIVERSAL_MUTE_H_
