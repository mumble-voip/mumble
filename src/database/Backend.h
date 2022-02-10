// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_DATABASE_BACKEND_H_
#define MUMBLE_DATABASE_BACKEND_H_

#include "Exception.h"

#include <string>

namespace mumble {
namespace db {

	/**
	 * An enum holding the different supported backend types
	 */
	enum class Backend {
		SQLite,
		MySQL,
		PostgreSQL,
	};

	/**
	 * An exception thrown if something goes wrong during Backend <-> string conversions
	 */
	class BadBackendException : public Exception {
	public:
		// Re-use all constructors of std::runtime_error
		using Exception::Exception;
	};

	/**
	 * Converts the given string into a Backend enum value. The string matching is performed case-insensitively.
	 *
	 * @param str The string to convert
	 * @returns The Backend corresponding to the given string
	 */
	Backend stringToBackend(const std::string &str);

	/**
	 * Converts the given Backend value into a string representation.
	 *
	 * @param backend The Backend to convert
	 * @returns The corresponding string representation
	 */
	std::string backendToString(Backend backend);

} // namespace db
} // namespace mumble

#endif // MUMBLE_DATABASE_BACKEND_H_
