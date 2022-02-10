// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "SQLiteConnectionParameter.h"

#include <boost/algorithm/string.hpp>

namespace mumble {
namespace db {

	SQLiteConnectionParameter::SQLiteConnectionParameter(const std::string &dbPath, bool useWAL) : useWAL(useWAL) {
		// Make sure the proper file extension is used
		if (!boost::iends_with(dbPath, ".sqlite")) {
			this->dbPath = dbPath + ".sqlite";
		} else {
			this->dbPath = dbPath;
		}
	}

	Backend SQLiteConnectionParameter::applicability() const { return Backend::SQLite; }

} // namespace db
} // namespace mumble
