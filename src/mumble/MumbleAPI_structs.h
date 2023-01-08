// Copyright 2022-2023 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

/// This header file includes the latest definition of Mumble's API
/// and also all older API structs for backward compatibility.

#ifndef EXTERNAL_MUMBLE_PLUGIN_API_STRUCTS_H_
#define EXTERNAL_MUMBLE_PLUGIN_API_STRUCTS_H_

// First, include the latest plugin API header file completely
#include "MumbleAPI_v_1_2_x.h"

// Include all older API headers for their API structs,
// but prevent utility definitions (to prevent name clashes).
#define EXTERNAL_MUMBLE_PLUGIN_API_NO_AUXILIARY_DEFINITIONS
#include "MumbleAPI_v_1_0_x.h"
#undef EXTERNAL_MUMBLE_PLUGIN_API_NO_AUXILIARY_DEFINITIONS

#endif // EXTERNAL_MUMBLE_PLUGIN_API_STRUCTS_H_
