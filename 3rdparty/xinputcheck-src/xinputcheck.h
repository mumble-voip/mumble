/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2013 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

#ifndef XINPUTCHECK_H
#define XINPUTCHECK_H

#include <windows.h>

/// XInputCheck_IsGuidProductXInputDevice checks whether the DirectInput
/// guidProduct given in GuidProductFromDirectInput is a valid XInput device.
///
/// This function holds an internal cache of the system's device tree. This
/// cache can be cleared by calling the XInputCheck_ClearDeviceCache function.
/// The function does not ever clear this cache itself, so it is important to
/// to call XInputCheck_ClearDeviceCache when new hardware has been added to
/// the system.
///
/// This function is not thread safe.
///
/// @param  pGuidProductFromDirectInput  The DirectInput guidProduct to check.
///                                      This parameter must not be NULL.
/// @return  Returns true if the given device is an XInput device. Returns
///          false if it is not.
///
/// @see  XInputCheck_ClearDeviceCache
bool XInputCheck_IsGuidProductXInputDevice(const GUID *pGuidProductFromDirectInput);

/// XInputCheck_ClearDeviceCache clears XInputCheck's internal device cache.
/// Calling this function causes the next call to
/// XInputCheck_IsGuidProductXInputDevice to query the system's device tree.
///
/// This function is not thread safe.
void XInputCheck_ClearDeviceCache();

#endif
