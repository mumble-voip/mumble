// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

// This is a collection of fixes for MinGW's <qos2.h> header.
// MinGW's <qos2.h> header does not provide everything we need,
// so define QOS_FLOWID (and PQOS_FLOWID) as well as QOS_NON_ADAPTIVE_FLOW ourselves
// to allow us to build with QoS support on MinGW.

#ifndef MUMBLE_QOS2_MINGW_H_
#define MUMBLE_QOS2_MINGW_H_

#ifdef __MINGW32__
	typedef UINT32 QOS_FLOWID, *PQOS_FLOWID;
	#define QOS_NON_ADAPTIVE_FLOW 0x00000002 // MinGW's qos2.h header doesn't define QOS_NON_ADAPTIVE_FLOW, so we define it ourselves for MinGW to use.
#endif

#endif
