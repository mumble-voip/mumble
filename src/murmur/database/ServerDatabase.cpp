// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ServerDatabase.h"
#include "ServerTable.h"

#include <cassert>

namespace mdb = ::mumble::db;

namespace mumble {
namespace server {
	namespace db {

		namespace TableIndex {
			enum Tables {
				MetaTable, // This table is always present and is created by the base class
				ServerTable,
			};
		}

		constexpr unsigned int ServerDatabase::DB_SCHEME_VERSION;

		ServerDatabase::ServerDatabase(::mdb::Backend backend) : ::mdb::Database(backend) {}

		unsigned int ServerDatabase::getSchemeVersion() const { return DB_SCHEME_VERSION; }

		void ServerDatabase::setupStandardTables() {
			::mdb::Database::table_id id = addTable(std::make_unique< ServerTable >(m_sql, m_backend));
			assert(id == TableIndex::ServerTable);

			// Mark id as unused in case the asserts are disabled (e.g. in release builds)
			(void) id;
		}

		ServerTable &ServerDatabase::getServerTable() {
			::mdb::Table *table = m_tables[TableIndex::ServerTable].get();
			assert(table);
			return *static_cast< ServerTable * >(table);
		}

	} // namespace db
} // namespace server
} // namespace mumble
