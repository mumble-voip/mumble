// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_DATABASE_INITEXCEPTION_H_
#define MUMBLE_DATABASE_INITEXCEPTION_H_

#include "Exception.h"

namespace mumble {
namespace db {

	/**
	 * Thrown whenever an error occurs during database initialization
	 */
	struct InitException : public Exception {
		using Exception::Exception;
	};

} // namespace db
} // namespace mumble

#endif // MUMBLE_DATABASE_INITEXCEPTION_H_
