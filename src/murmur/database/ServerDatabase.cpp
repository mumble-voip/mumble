// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ServerDatabase.h"

namespace mumble {
namespace server {
	namespace db {

		constexpr unsigned int ServerDatabase::DB_SCHEME_VERSION;

		ServerDatabase::ServerDatabase(::mumble::db::Backend backend) : ::mumble::db::Database(backend) {}

		unsigned int ServerDatabase::getSchemeVersion() const { return DB_SCHEME_VERSION; }

		void ServerDatabase::setupStandardTables() {
			// TODO: Add all necessary tables
		}

	} // namespace db
} // namespace server
} // namespace mumble
