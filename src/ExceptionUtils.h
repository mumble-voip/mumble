// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_EXCEPTIONUTILS_H_
#define MUMBLE_EXCEPTIONUTILS_H_

#include <exception>
#include <iosfwd>
#include <string>

namespace mumble {

/**
 * Helper function to print exception messages that can also fully unfold nested exceptions
 */
void printExceptionMessage(std::ostream &stream, const std::exception &e, int indent = 0, bool trailingNL = true);

/**
 * Helper function to get exception messages that can also fully unfold nested exceptions
 */
std::string getExceptionMessage(const std::exception &e, int indent = 0);

} // namespace mumble

#endif
