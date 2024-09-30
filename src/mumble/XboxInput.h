// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_XBOXINPUT_H_
#define MUMBLE_MUMBLE_XBOXINPUT_H_

#include "win.h"

#include <QUuid>

#include <stdint.h>

/// XBOXINPUT_MAX_DEVICES defines the maximum
/// number of devices that can be connected
/// to the system at once.
///
/// In typical operation, one calls GetState()
/// in a loop bounded by XBOXINPUT_MAX_DEVICES.
#define XBOXINPUT_MAX_DEVICES 4

/// XBOXINPUT_TRIGGER_THRESHOLD defines the threshold
/// that an analog trigger (leftTrigger and
/// rightTrigger of XboxInputState) must have exceeded
/// in order to count as a button press.
#define XBOXINPUT_TRIGGER_THRESHOLD 30

/// XboxInputState represents the state of an
/// Xbox controller as returned by GetState().
struct XboxInputState {
	uint32_t packetNumber;
	uint16_t buttons;
	uint8_t leftTrigger;
	uint8_t rightTrigger;
	uint16_t leftThumbX;
	uint16_t leftThumbY;
	uint16_t rightThumbY;
	uint16_t rightThumbX;
	uint32_t paddingReserved; // Required for XInputGetStateEx. Not required for XInputGetState.
};

typedef uint32_t(WINAPI *XboxInputGetStateFunc)(uint32_t deviceIndex, XboxInputState *state);

/// XboxInput is an XInput wrapper that dynamically loads an appropriate
/// xinput*.dll on construction and provides access to its GetState(Ex)
/// function.
class XboxInput {
public:
	XboxInput();
	virtual ~XboxInput();

	/// s_XboxInputGuid is the GUID used by GlobalShortcut_win
	/// to distinguish XboxInputLibrary's events from other event
	/// sources.
	static const QUuid s_XboxInputGuid;

	/// isValid determines whether the XboxInputLibrary
	/// is usable.
	bool isValid() const;

	/// Query the state of the Xbox controller at deviceIndex.
	/// If the function succeeds, it returns 0 (Windows's ERROR_SUCCESS).
	/// If no device is connected, it returns 0x48F (Windows's ERROR_DEVICE_NOT_CONNECTED).
	XboxInputGetStateFunc GetState;

protected:
	/// m_getStateFunc represents XInputGetState from the XInput DLL.
	XboxInputGetStateFunc m_getStateFunc;

	/// m_getStateFuncEx represents XInputGetStateEx, which is optionally
	/// available in the XInput DLL.
	XboxInputGetStateFunc m_getStateExFunc;

	/// m_xinputlib is the handle to the XInput DLL as returned by
	/// LoadLibrary.
	HMODULE m_xinputlib;

	/// m_valid determines whether or not the XboxInputLibrary
	/// is valid for use.
	bool m_valid;
};

#endif
