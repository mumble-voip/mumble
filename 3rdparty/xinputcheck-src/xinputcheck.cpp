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

#define SDL_bool      bool
#define SDL_TRUE      true
#define SDL_FALSE     false

#define SDL_free         free
#define SDL_malloc       malloc
#define SDL_OutOfMemory  abort
#define SDL_strstr       strstr
#define SDL_memcmp       memcmp

#define SDL_arraysize(array)    (sizeof(array)/sizeof(array[0]))

static PRAWINPUTDEVICELIST SDL_RawDevList = NULL;
static UINT SDL_RawDevListCount = 0;

static SDL_bool
SDL_XINPUT_Enabled() {
    return SDL_TRUE;
}

static SDL_bool
SDL_IsXInputDevice(const GUID* pGuidProductFromDirectInput)
{
    static GUID IID_ValveStreamingGamepad = { MAKELONG(0x28DE, 0x11FF), 0x0000, 0x0000, { 0x00, 0x00, 0x50, 0x49, 0x44, 0x56, 0x49, 0x44 } };
    static GUID IID_X360WiredGamepad = { MAKELONG(0x045E, 0x02A1), 0x0000, 0x0000, { 0x00, 0x00, 0x50, 0x49, 0x44, 0x56, 0x49, 0x44 } };
    static GUID IID_X360WirelessGamepad = { MAKELONG(0x045E, 0x028E), 0x0000, 0x0000, { 0x00, 0x00, 0x50, 0x49, 0x44, 0x56, 0x49, 0x44 } };

    static const GUID *s_XInputProductGUID[] = {
        &IID_ValveStreamingGamepad,
        &IID_X360WiredGamepad,   /* Microsoft's wired X360 controller for Windows. */
        &IID_X360WirelessGamepad /* Microsoft's wireless X360 controller for Windows. */
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
