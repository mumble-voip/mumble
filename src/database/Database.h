// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_DATABASE_DATABASE_H_
#define MUMBLE_DATABASE_DATABASE_H_

#include "Backend.h"
#include "ConnectionParameter.h"
#include "NonCopyable.h"
#include "Table.h"
#include "TransactionHolder.h"
#include "Version.h"

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include <soci/soci.h>

#include <nlohmann/json_fwd.hpp>

namespace mumble {
namespace db {

	/**
	 * A general class representing a database which in turn consists of tables. This is an abstract class
	 * that is intended to be subclassed for actual implementations.
	 */
	class Database : NonCopyable {
	public:
		using table_id = unsigned int;

		static constexpr const char *OLD_TABLE_SUFFIX = "_old";

		Database(Backend backend);
		virtual ~Database();

		virtual void init(const ConnectionParameter &parameter);

		virtual unsigned int getSchemeVersion() const = 0;

		table_id addTable(std::unique_ptr< Table > table);

		const Table *getTable(const std::string &name) const;
		const Table *getTable(table_id tableID) const;
		Table *getTable(const std::string &name);
		Table *getTable(table_id tableID);

		std::unique_ptr< Table > takeTable(const std::string &name);
		std::unique_ptr< Table > takeTable(table_id tableID);

		void removeTable(const std::string &name);
		void removeTable(table_id tableID);

		void importFromJSON(const nlohmann::json &json, bool createMissingTables);
		nlohmann::json exportToJSON() const;

		/**
		 * Deletes all tables from the database. Note that this will leave the actual Table objects contained
		 * in this object in tact.
		 */
		virtual void destroyTables();
		/**
		 * Clears all tables contained in this database
		 */
		virtual void clearTables();

		/**
		 * @returns The version of the used DB backend. If the version query fails, an AccessException is thrown. If
		 * the query succeeds but the result can't be parsed properly, a default constructed Version object (0.0.0)
		 * is returned.
		 */
		Version getBackendVersion();

		/**
		 * If the database is not already in a transaction, a new transaction will be started.
		 *
		 * @return A TransactionHolder managing the (potentially) started transaction.
		 */
		TransactionHolder ensureTransaction();

	protected:
		Backend m_backend;
		std::vector< std::unique_ptr< Table > > m_tables;
		soci::session m_sql;
		bool m_activeTransaction = false;

		void connectToDB(const ConnectionParameter &parameter);

		bool tableExistsInDB(const std::string &name);

		std::unordered_set< std::string > getExistingTables();

		void applyBackendSpecificSetup(const ConnectionParameter &parameter);

		virtual void setupStandardTables();
		void createTables();
		/**
		 * Initiates the migration of all tables currently contained in this database. Note that this function
		 * assumes that a transaction has already been started before calling this function.
		 *
		 * @param fromSchemeVersion The scheme version to migrate from
		 * @param toSchemeVersion The scheme version to migrate to
		 */
		void migrateTables(unsigned int fromSchemeVersion, unsigned int toSchemeVersion);

		virtual void importMetaData(const nlohmann::json &json);
		virtual nlohmann::json exportMetaData() const;
	};

} // namespace db
} // namespace mumble

#endif // MUMBLE_DATABASE_DATABASE_H_
