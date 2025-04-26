// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ServerDatabase.h"
#include "ACLTable.h"
#include "BanTable.h"
#include "ChannelLinkTable.h"
#include "ChannelListenerTable.h"
#include "ChannelPropertyTable.h"
#include "ChannelTable.h"
#include "ConfigTable.h"
#include "GroupMemberTable.h"
#include "GroupTable.h"
#include "LogTable.h"
#include "ServerTable.h"
#include "UserProperty.h"
#include "UserPropertyTable.h"
#include "UserTable.h"

#include "database/MetaTable.h"
#include "database/MigrationException.h"

#include <cassert>

namespace mdb = ::mumble::db;

namespace mumble {
namespace server {
	namespace db {

		namespace TableIndex {
			enum Tables {
				MetaTable, // This table is always present and is created by the base class
				ServerTable,
				LogTable,
				ConfigTable,
				ChannelTable,
				ChannelPropertyTable,
				UserTable,
				UserPropertyTable,
				GroupTable,
				GroupMemberTable,
				ACLTable,
				ChannelLinkTable,
				BanTable,
				ChannelListenerTable,
			};
		}

		constexpr unsigned int ServerDatabase::DB_SCHEME_VERSION;

		ServerDatabase::ServerDatabase(::mdb::Backend backend) : ::mdb::Database(backend) {}

		unsigned int ServerDatabase::getSchemeVersion() const { return DB_SCHEME_VERSION; }

		void ServerDatabase::migrateTables(unsigned int fromSchemeVersion, unsigned int toSchemeVersion) {
			// Ensure that the migration is in a range that we actually support
			if (toSchemeVersion != DB_SCHEME_VERSION) {
				throw ::mdb::MigrationException(
					"Requested to migrate to scheme version " + std::to_string(toSchemeVersion)
					+ " but can only migrate to latest scheme version (" + std::to_string(DB_SCHEME_VERSION) + ")");
			}
			if (fromSchemeVersion < 6) {
				throw ::mdb::MigrationException("Requested to migrate from scheme version "
												+ std::to_string(fromSchemeVersion)
												+ " but support for versions < 6 has been dropped");
			}

			::mdb::Database::migrateTables(fromSchemeVersion, toSchemeVersion);
		}

		void ServerDatabase::setupStandardTables() {
			::mdb::Database::table_id id = addTable(std::make_unique< ServerTable >(m_sql, m_backend));
			assert(id == TableIndex::ServerTable);

			id = addTable(std::make_unique< LogTable >(m_sql, m_backend, getServerTable()));
			assert(id == TableIndex::LogTable);

			id = addTable(std::make_unique< ConfigTable >(m_sql, m_backend, getServerTable()));
			assert(id == TableIndex::ConfigTable);

			id = addTable(std::make_unique< ChannelTable >(m_sql, m_backend, getServerTable()));
			assert(id == TableIndex::ChannelTable);

			id = addTable(std::make_unique< ChannelPropertyTable >(m_sql, m_backend, getChannelTable()));
			assert(id == TableIndex::ChannelPropertyTable);

			id = addTable(std::make_unique< UserTable >(m_sql, m_backend, getServerTable(), getChannelTable()));
			assert(id == TableIndex::UserTable);

			id = addTable(std::make_unique< UserPropertyTable >(m_sql, m_backend, getUserTable()));
			assert(id == TableIndex::UserPropertyTable);

			id = addTable(std::make_unique< GroupTable >(m_sql, m_backend, getChannelTable()));
			assert(id == TableIndex::GroupTable);

			id = addTable(std::make_unique< GroupMemberTable >(m_sql, m_backend, getGroupTable(), getUserTable()));
			assert(id == TableIndex::GroupMemberTable);

			id = addTable(
				std::make_unique< ACLTable >(m_sql, m_backend, getChannelTable(), getUserTable(), getGroupTable()));
			assert(id == TableIndex::ACLTable);

			id = addTable(std::make_unique< ChannelLinkTable >(m_sql, m_backend, getChannelTable()));
			assert(id == TableIndex::ChannelLinkTable);

			id = addTable(std::make_unique< BanTable >(m_sql, m_backend, getServerTable()));
			assert(id == TableIndex::BanTable);

			id =
				addTable(std::make_unique< ChannelListenerTable >(m_sql, m_backend, getUserTable(), getChannelTable()));
			assert(id == TableIndex::ChannelListenerTable);

			// Mark id as unused in case the asserts are disabled (e.g. in release builds)
			(void) id;
		}

		::mdb::MetaTable &ServerDatabase::getMetaTable() {
			assert(m_tables.size() > TableIndex::MetaTable);
			::mdb::Table *table = m_tables[TableIndex::MetaTable].get();
			assert(table);
			return *static_cast<::mdb::MetaTable * >(table);
		}

#define GET_TABLE_IMPL(tableName)                                    \
	tableName &ServerDatabase::get##tableName() {                    \
		assert(m_tables.size() > TableIndex::tableName);             \
		::mdb::Table *table = m_tables[TableIndex::tableName].get(); \
		assert(table);                                               \
		return *static_cast< tableName * >(table);                   \
	}

		GET_TABLE_IMPL(ServerTable)
		GET_TABLE_IMPL(LogTable)
		GET_TABLE_IMPL(ConfigTable)
		GET_TABLE_IMPL(ChannelTable)
		GET_TABLE_IMPL(ChannelPropertyTable)
		GET_TABLE_IMPL(UserTable)
		GET_TABLE_IMPL(UserPropertyTable)
		GET_TABLE_IMPL(GroupTable)
		GET_TABLE_IMPL(GroupMemberTable)
		GET_TABLE_IMPL(ACLTable)
		GET_TABLE_IMPL(ChannelLinkTable)
		GET_TABLE_IMPL(BanTable)
		GET_TABLE_IMPL(ChannelListenerTable)

#undef GET_TABLE_IMPL

	} // namespace db
} // namespace server
} // namespace mumble
