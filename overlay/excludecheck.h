// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_EXCLUDECHECK_H_
#define MUMBLE_EXCLUDECHECK_H_

/// Check whether the overlay should be enabled for the given absolute and relative exe names.
bool ExcludeCheckIsOverlayEnabled(std::string absExeName_, std::string exeName_);

#endif
