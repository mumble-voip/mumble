// Copyright 2013-2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

// Used by the overlay to detect whether we injected into ourselves.
//
// This code also lives in Overlay_win.cpp, where it serves the same purpose.
//
// When running in "mumble_app.dll"-mode, it's convenient for this symbol to be
// defined here in mumble.exe (alongside the symbol in mumble_app.dll) because that
// allows older versions of mumble_ol.dll that are not "mumble_app.dll"-aware at all
// to continue to work in that world.
extern "C" __declspec(dllexport) void mumbleSelfDetection(){};
