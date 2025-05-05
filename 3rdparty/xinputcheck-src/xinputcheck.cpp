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

#include "xinputcheck.h"

#include <string.h>
#include <stdlib.h>

// XInputCheck is an XInput check abstracted away from SDL.
//
// If you need to update this file, please arrange the code such that
// it is possible to copy/paste directly from SDL. This makes code
// sharing easier! Right now, the SDL_IsXInputDevice is directly
// copied from SDL.
//
// Below, we have a few preprocessor defines, that allow the SDL functions
// we use, to run in our environment.
//
// After that, there is an implementation of the SDL_XINPUT_Enabled
// that always returns true.
//
// Then comes the implementation of SDL_IsXInputDevice, which is copied
// directly from SDL.
//
// At the bottom of the file, you'll find our wrapper interfaces --
// the public API for the XInputCheck library.
//
// Again, if you ever need to change anything in the SDL functions, well.
// Think again! If you find a bug, feel free to fix it, but report it
// upstream as well.
// If at all possible, try to use wrapper functions to implement "new"
// functionality (such as XInputCheck_ClearDeviceCache, for clearing
// the internal cache).

// Preprocessor helpers for running SDL code as-is.
#define SDL_bool                bool
#define SDL_TRUE                true
#define SDL_FALSE               false
#define SDL_free                free
#define SDL_malloc              malloc
#define SDL_OutOfMemory         abort
#define SDL_strstr              strstr
#define SDL_memcmp              memcmp
#define SDL_arraysize(array)    (sizeof(array)/sizeof(array[0]))


static SDL_bool
SDL_XINPUT_Enabled() {
    return SDL_TRUE;
}

static PRAWINPUTDEVICELIST SDL_RawDevList = NULL;
static UINT SDL_RawDevListCount = 0;

static SDL_bool
SDL_IsXInputDevice(const GUID* pGuidProductFromDirectInput)
{
    static GUID IID_ValveStreamingGamepad = { MAKELONG(0x28DE, 0x11FF), 0x0000, 0x0000, { 0x00, 0x00, 0x50, 0x49, 0x44, 0x56, 0x49, 0x44 } };
    static GUID IID_X360WiredGamepad = { MAKELONG(0x045E, 0x02A1), 0x0000, 0x0000, { 0x00, 0x00, 0x50, 0x49, 0x44, 0x56, 0x49, 0x44 } };
    static GUID IID_X360WirelessGamepad = { MAKELONG(0x045E, 0x028E), 0x0000, 0x0000, { 0x00, 0x00, 0x50, 0x49, 0x44, 0x56, 0x49, 0x44 } };
    static GUID IID_XOneWiredGamepad = { MAKELONG(0x045E, 0x02FF), 0x0000, 0x0000, { 0x00, 0x00, 0x50, 0x49, 0x44, 0x56, 0x49, 0x44 } };
    static GUID IID_XOneWirelessGamepad = { MAKELONG(0x045E, 0x02DD), 0x0000, 0x0000, { 0x00, 0x00, 0x50, 0x49, 0x44, 0x56, 0x49, 0x44 } };
    static GUID IID_XOneNewWirelessGamepad = { MAKELONG(0x045E, 0x02D1), 0x0000, 0x0000, { 0x00, 0x00, 0x50, 0x49, 0x44, 0x56, 0x49, 0x44 } };
    static GUID IID_XOneSWirelessGamepad = { MAKELONG(0x045E, 0x02EA), 0x0000, 0x0000, { 0x00, 0x00, 0x50, 0x49, 0x44, 0x56, 0x49, 0x44 } };
    static GUID IID_XOneSBluetoothGamepad = { MAKELONG(0x045E, 0x02E0), 0x0000, 0x0000, { 0x00, 0x00, 0x50, 0x49, 0x44, 0x56, 0x49, 0x44 } };
    static GUID IID_XOneEliteWirelessGamepad = { MAKELONG(0x045E, 0x02E3), 0x0000, 0x0000, { 0x00, 0x00, 0x50, 0x49, 0x44, 0x56, 0x49, 0x44 } };

    static const GUID *s_XInputProductGUID[] = {
        &IID_ValveStreamingGamepad,
        &IID_X360WiredGamepad,         /* Microsoft's wired X360 controller for Windows. */
        &IID_X360WirelessGamepad,      /* Microsoft's wireless X360 controller for Windows. */
        &IID_XOneWiredGamepad,         /* Microsoft's wired Xbox One controller for Windows. */
        &IID_XOneWirelessGamepad,      /* Microsoft's wireless Xbox One controller for Windows. */
        &IID_XOneNewWirelessGamepad,   /* Microsoft's updated wireless Xbox One controller (w/ 3.5 mm jack) for Windows. */
        &IID_XOneSWirelessGamepad,     /* Microsoft's wireless Xbox One S controller for Windows. */
        &IID_XOneSBluetoothGamepad,    /* Microsoft's Bluetooth Xbox One S controller for Windows. */
        &IID_XOneEliteWirelessGamepad  /* Microsoft's wireless Xbox One Elite controller for Windows. */
    };

    size_t iDevice;
    UINT i;

    if (!SDL_XINPUT_Enabled()) {
        return SDL_FALSE;
    }

    /* Check for well known XInput device GUIDs */
    /* This lets us skip RAWINPUT for popular devices. Also, we need to do this for the Valve Streaming Gamepad because it's virtualized and doesn't show up in the device list. */
    for (iDevice = 0; iDevice < SDL_arraysize(s_XInputProductGUID); ++iDevice) {
        if (SDL_memcmp(pGuidProductFromDirectInput, s_XInputProductGUID[iDevice], sizeof(GUID)) == 0) {
            return SDL_TRUE;
        }
    }

    /* Go through RAWINPUT (WinXP and later) to find HID devices. */
    /* Cache this if we end up using it. */
    if (SDL_RawDevList == NULL) {
        if ((GetRawInputDeviceList(NULL, &SDL_RawDevListCount, sizeof(RAWINPUTDEVICELIST)) == -1) || (!SDL_RawDevListCount)) {
            return SDL_FALSE;  /* oh well. */
        }

        SDL_RawDevList = (PRAWINPUTDEVICELIST)SDL_malloc(sizeof(RAWINPUTDEVICELIST) * SDL_RawDevListCount);
        if (SDL_RawDevList == NULL) {
            SDL_OutOfMemory();
            return SDL_FALSE;
        }

        if (GetRawInputDeviceList(SDL_RawDevList, &SDL_RawDevListCount, sizeof(RAWINPUTDEVICELIST)) == -1) {
            SDL_free(SDL_RawDevList);
            SDL_RawDevList = NULL;
            return SDL_FALSE;  /* oh well. */
        }
    }

    for (i = 0; i < SDL_RawDevListCount; i++) {
        RID_DEVICE_INFO rdi;
        char devName[128];
        UINT rdiSize = sizeof(rdi);
        UINT nameSize = SDL_arraysize(devName);

        rdi.cbSize = sizeof(rdi);
        if ((SDL_RawDevList[i].dwType == RIM_TYPEHID) &&
            (GetRawInputDeviceInfoA(SDL_RawDevList[i].hDevice, RIDI_DEVICEINFO, &rdi, &rdiSize) != ((UINT)-1)) &&
            (MAKELONG(rdi.hid.dwVendorId, rdi.hid.dwProductId) == ((LONG)pGuidProductFromDirectInput->Data1)) &&
            (GetRawInputDeviceInfoA(SDL_RawDevList[i].hDevice, RIDI_DEVICENAME, devName, &nameSize) != ((UINT)-1)) &&
            (SDL_strstr(devName, "IG_") != NULL)) {
            return SDL_TRUE;
        }
    }

    return SDL_FALSE;
}

bool XInputCheck_IsGuidProductXInputDevice(const GUID *pGuidProductFromDirectInput) {
    return SDL_IsXInputDevice(pGuidProductFromDirectInput);
}

void XInputCheck_ClearDeviceCache() {
    SDL_free(SDL_RawDevList);
    SDL_RawDevList = NULL;
    SDL_RawDevListCount = 0;
}
