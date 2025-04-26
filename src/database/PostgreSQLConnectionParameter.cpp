// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "database/PostgreSQLConnectionParameter.h"
#include "database/Backend.h"

#include <string>

namespace mumble {
namespace db {

	PostgreSQLConnectionParameter::PostgreSQLConnectionParameter(const std::string &dbName) : dbName(dbName) {}

	Backend PostgreSQLConnectionParameter::applicability() const { return Backend::PostgreSQL; }


} // namespace db
} // namespace mumble
