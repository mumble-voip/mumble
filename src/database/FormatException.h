// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_DATABASE_FORMATEXCEPTION_H_
#define MUMBLE_DATABASE_FORMATEXCEPTION_H_

#include "Exception.h"

namespace mumble {
namespace db {

	/**
	 * Thrown whenever an invalid format is encountered in the context of a database operation
	 */
	struct FormatException : public Exception {
		using Exception::Exception;
	};

} // namespace db
} // namespace mumble

#endif // MUMBLE_DATABASE_FORMATEXCEPTION_H_
