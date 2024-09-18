// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_DATABASE_NODATAEXCEPTION_H_
#define MUMBLE_DATABASE_NODATAEXCEPTION_H_

#include "AccessException.h"

namespace mumble {
namespace db {

	/**
	 * Thrown whenever a query does not result in any (usable) data
	 */
	struct NoDataException : public AccessException {
		using AccessException::AccessException;
	};
} // namespace db
} // namespace mumble

#endif // MUMBLE_DATABASE_NODATAEXCEPTION_H_
