// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_ANCESTOR_H_
#define MUMBLE_ANCESTOR_H_

#include <string>
#include <vector>

/// Return the absolute and relative exe names of this process's ancestors in
/// |absAncestorExeNames| and |ancestorExeNames|.
///
/// Returns true on success and fills out |absAncestorExeNames| and |ancestorExeNames|.
/// Returns false on failure, and does not change |absAncestorExeNames| and |ancestorExeNames|.
bool GetProcessAncestorChain(std::vector< std::string > &absAncestorExeNames,
							 std::vector< std::string > &ancestorExeNames);

#endif
