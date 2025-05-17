// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ExceptionUtils.h"

#include <exception>
#include <iostream>
#include <sstream>
#include <string>

namespace mumble {

void printExceptionMessage(std::ostream &stream, const std::exception &e, int indent, bool trailingNL) {
	for (int i = 0; i < indent; ++i) {
		stream << " ";
	}

	stream << e.what();

	try {
		std::rethrow_if_nested(e);
	} catch (const std::exception &nestedExc) {
		stream << "\n";
		indent += 2;

		printExceptionMessage(stream, nestedExc, indent, false);
	}

	if (trailingNL) {
		stream << "\n";
	}

	stream.flush();
}

std::string getExceptionMessage(const std::exception &e, int indent) {
	std::stringstream sstream;

	printExceptionMessage(sstream, e, indent, false);

	return sstream.str();
}

} // namespace mumble
