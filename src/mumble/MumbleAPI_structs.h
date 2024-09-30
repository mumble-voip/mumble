// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

/// This header file includes the latest definition of Mumble's API
/// and also all older API structs for backward compatibility.

#ifndef EXTERNAL_MUMBLE_PLUGIN_API_STRUCTS_H_
#define EXTERNAL_MUMBLE_PLUGIN_API_STRUCTS_H_

#define MUMBLE_PLUGIN_NO_DEFAULT_FUNCTION_DEFINITIONS
#define MUMBLE_PLUGIN_NO_API_TYPEDEF

// First, include the latest plugin API header file completely
#include "MumblePlugin.h"


// Re-include the API definition
#undef EXTERNAL_MUMBLE_PLUGIN_MUMBLE_API_
// But this time, overwrite the version
#undef MUMBLE_PLUGIN_API_MAJOR_MACRO
#define MUMBLE_PLUGIN_API_MAJOR_MACRO 1
#undef MUMBLE_PLUGIN_API_MINOR_MACRO
#define MUMBLE_PLUGIN_API_MINOR_MACRO 0

#include "MumblePlugin.h"

#undef MUMBLE_PLUGIN_NO_DEFAULT_FUNCTION_DEFINITIONS

#endif // EXTERNAL_MUMBLE_PLUGIN_API_STRUCTS_H_
