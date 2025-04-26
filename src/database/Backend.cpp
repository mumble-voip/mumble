// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Backend.h"

#include <boost/algorithm/string.hpp>

#include <cassert>

namespace mumble {
namespace db {

	Backend stringToBackend(const std::string &str) {
		if (boost::iequals(str, "sqlite")) {
			return Backend::SQLite;
		} else if (boost::iequals(str, "mysql")) {
			return Backend::MySQL;
		} else if (boost::iequals(str, "postgresql")) {
			return Backend::PostgreSQL;
		}

		throw BadBackendException(std::string("Unknown backend \"") + str + "\"");
	}

	std::string backendToString(Backend backend) {
		switch (backend) {
			case Backend::SQLite:
				return "SQLite";
			case Backend::MySQL:
				return "MySQL";
			case Backend::PostgreSQL:
				return "PostgreSQL";
		}

		// This code shouldn't be reached under normal circumstances
		assert(false && "Reached unreachable code path");
		throw BadBackendException(std::string("Invalid value for Backend enum: ")
								  + std::to_string(static_cast< std::underlying_type_t< Backend > >(backend)));
	}

} // namespace db
} // namespace mumble
