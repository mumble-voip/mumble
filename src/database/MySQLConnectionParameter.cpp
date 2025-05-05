// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "database/MySQLConnectionParameter.h"
#include "database/Backend.h"

#include <string>

namespace mumble {
namespace db {

	MySQLConnectionParameter::MySQLConnectionParameter(const std::string &dbName) : dbName(dbName) {}

	Backend MySQLConnectionParameter::applicability() const { return Backend::MySQL; }


} // namespace db
} // namespace mumble
