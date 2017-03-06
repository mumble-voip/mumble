// Copyright 2005-2017 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include <initguid.h>
#include <windows.h>
#include <mmreg.h>
#include <strsafe.h>
#include <mmdeviceapi.h>
#include <avrt.h>
#include <audioclient.h>
#include <ksmedia.h>
#include <functiondiscoverykeys.h>
#ifdef _INC_FUNCTIONDISCOVERYKEYS
# undef _INC_FUNCTIONDISCOVERYKEYS
#endif
#include <functiondiscoverykeys_devpkey.h>
#include <propidl.h>
#include <audiopolicy.h>

// Temporarily define these for MinGW to fix the build...
#if defined(__MINGW32__)
DEFINE_PROPERTYKEY(PKEY_AudioEndpoint_PhysicalSpeakers,0x1da5d803,0xd492,0x4edd,0x8c,0x23,0xe0,0xc0,0xff,0xee,0x7f,0x0e,3);
DEFINE_PROPERTYKEY(PKEY_AudioEngine_DeviceFormat,0xf19f064d,0x082c,0x4e27,0xbc,0x73,0x68,0x82,0xa1,0xbb,0x8e,0x4c,0);
#endif

DEFINE_GUID(IID_IVistaAudioSessionControl2, 0x33969B1DL, 0xD06F, 0x4281, 0xB8, 0x37, 0x7E, 0xAA, 0xFD, 0x21, 0xA9, 0xC0);
DEFINE_GUID(IID_IAudioSessionQuery, 0x94BE9D30L, 0x53AC, 0x4802, 0x82, 0x9C, 0xF1, 0x3E, 0x5A, 0xD3, 0x47, 0x75);
