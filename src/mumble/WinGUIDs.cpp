// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include <windows.h>
#include <audioclient.h>
#include <avrt.h>
#include <functiondiscoverykeys.h>
// initguid.h defines the macro INITGUID before including guiddef.h
// which causes DEFINE_GUID to actually set the value of that GUID
// (instead of only declaring its name as an extern variable).
#include <initguid.h>
#include <ksmedia.h>
#include <mmdeviceapi.h>
#include <mmreg.h>
#include <propkey.h>
#include <strsafe.h>
#include <wtypes.h>
#ifdef _INC_FUNCTIONDISCOVERYKEYS
#	undef _INC_FUNCTIONDISCOVERYKEYS
#endif
#include <audiopolicy.h>
#include <functiondiscoverykeys_devpkey.h>
#include <propidl.h>

DEFINE_GUID(IID_IVistaAudioSessionControl2, 0x33969B1DL, 0xD06F, 0x4281, 0xB8, 0x37, 0x7E, 0xAA, 0xFD, 0x21, 0xA9,
			0xC0);
DEFINE_GUID(IID_IAudioSessionQuery, 0x94BE9D30L, 0x53AC, 0x4802, 0x82, 0x9C, 0xF1, 0x3E, 0x5A, 0xD3, 0x47, 0x75);
