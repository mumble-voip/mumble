// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Database.h"
#include "AccessException.h"
#include "FormatException.h"
#include "InitException.h"
#include "MetaTable.h"
#include "MigrationException.h"
#include "MySQLConnectionParameter.h"
#include "PostgreSQLConnectionParameter.h"
#include "SQLiteConnectionParameter.h"
#include "Savepoint.h"

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <limits>
#include <memory>
#include <unordered_set>

#include <nlohmann/json.hpp>

#include <boost/algorithm/string.hpp>

// These functions are defined by the respective SOCI backend libraries
#ifndef MUMBLE_DISABLE_SQLITE
extern "C" void register_factory_sqlite3();
#endif
#ifndef MUMBLE_DISABLE_MYSQL
extern "C" void register_factory_mysql();
#endif
#ifndef MUMBLE_DISABLE_POSTGRESQL
extern "C" void register_factory_postgresql();
#endif

namespace mumble {
namespace db {

	constexpr const char *Database::OLD_TABLE_SUFFIX;

	struct find_by_name {
		const std::string &name;

		bool operator()(const std::unique_ptr< Table > &ptr) { return ptr && ptr->getName() == name; }
	};

	Database::Database(Backend backend) : m_backend(backend) {}

	void Database::init(const ConnectionParameter &parameter) { init(parameter, true, 0); }

	Backend Database::getBackend() const { return m_backend; }

	Database::table_id Database::addTable(std::unique_ptr< Table > table) {
		table->setDatabase(this);
		m_tables.push_back(std::move(table));

		// The "table ID" is just the index of the table in the used vector
		return static_cast< table_id >(m_tables.size() - 1);
	}

	const Table *Database::getTable(const std::string &name) const {
		auto it = std::find_if(m_tables.begin(), m_tables.end(), find_by_name{ name });

		if (it != m_tables.end()) {
			return it->get();
		} else {
			return nullptr;
		}
	}

	const Table *Database::getTable(Database::table_id tableID) const {
		assert(tableID < m_tables.size());

		if (tableID < m_tables.size()) {
			return m_tables[tableID].get();
		} else {
			return nullptr;
		}
	}

	Table *Database::getTable(const std::string &name) {
		// Reuse const implementation
		return const_cast< Table * >(const_cast< const Database * >(this)->getTable(name));
	}

	Table *Database::getTable(Database::table_id tableID) {
		// Reuse const implementation
		return const_cast< Table * >(const_cast< const Database * >(this)->getTable(tableID));
	}

	std::unique_ptr< Table > Database::takeTable(const std::string &name) {
		auto it = std::find_if(m_tables.begin(), m_tables.end(), find_by_name{ name });

		if (it != m_tables.end()) {
			// In order to not invalidate an previously assigned table IDs, we can't actually remove the table from our
			// vector (that would mess up all IDs (indices) of the tables after it. Instead, we move the pointer out of
			// the vector, but keep a nullptr at its original place in the list.
			std::unique_ptr< Table > ptr = std::move(*it);

			it->reset();

			ptr->setDatabase(nullptr);

			return ptr;
		} else {
			return nullptr;
		}
	}

	std::unique_ptr< Table > Database::takeTable(Database::table_id tableID) {
		assert(tableID < m_tables.size());

		if (tableID < m_tables.size()) {
			// Same argument here, as in the overload taking a string (see above)
			std::unique_ptr< Table > ptr = std::move(m_tables[tableID]);

			m_tables[tableID].reset();

			ptr->setDatabase(nullptr);

			return ptr;
		} else {
			return nullptr;
		}
	}

	void Database::removeTable(const std::string &name) { takeTable(name); }

	void Database::removeTable(Database::table_id tableID) { takeTable(tableID); }

	void Database::importFromJSON(const nlohmann::json &json, bool createMissingTables) {
		// We wrap the entire import into a transaction to make sure, that we don't end up with partial imports if an
		// error is encountered during the import.
		TransactionHolder transaction = ensureTransaction();

		if (!json.contains("meta_data")) {
			throw FormatException("JSON-import: JSON is missing top-level \"meta_data\" object");
		}

		importMetaData(json["meta_data"]);

		if (!json.contains("tables")) {
			throw FormatException("JSON-import: JSON is missing top-level \"tables\" object");
		}

		const nlohmann::json tables = json["tables"];

		if (!tables.is_object()) {
			throw FormatException("JSON-import: Top-level \"tables\" entry is not of type object");
		}

		for (auto it = tables.begin(); it != tables.end(); ++it) {
			std::string tableName = it.key();

			auto tableIt = std::find_if(m_tables.begin(), m_tables.end(), find_by_name{ tableName });

			Table *table;
			bool tableIsNew = false;
			if (tableIt == m_tables.end()) {
				if (createMissingTables) {
					// Create table on-the-fly
					table_id id = addTable(std::make_unique< Table >(m_sql, m_backend, tableName));

					table      = m_tables[id].get();
					tableIsNew = true;
				} else {
					throw FormatException("JSON-import: Unknown table \"" + tableName + "\"");
				}
			} else {
				table = tableIt->get();
			}

			const nlohmann::json &body = it.value();

			if (!body.is_object()) {
				throw FormatException(std::string("JSON-import: Specification for table \"") + tableName
									  + "\" is not an object");
			}

			// First ensure the table is empty
			if (!tableIsNew) {
				table->clear();
			}

			// Then import new data into the table
			table->importFromJSON(body, tableIsNew);
		}

		transaction.commit();
	}

	nlohmann::json Database::exportToJSON() const {
		TransactionHolder transaction = ensureTransaction();
		nlohmann::json json;
		json["meta_data"] = exportMetaData();

		nlohmann::json &tables = json["tables"];

		for (const std::unique_ptr< Table > &currentTable : m_tables) {
			if (currentTable) {
				tables[currentTable->getName()] = currentTable->exportToJSON();
			}
		}

		transaction.commit();
		return json;
	}

	std::size_t countTables(const std::vector< std::unique_ptr< Table > > &tables) {
		std::size_t size = 0;
		for (const std::unique_ptr< Table > &current : tables) {
			if (!current) {
				continue;
			}

			size++;
		}

		return size;
	}

	void Database::destroyTables() {
		TransactionHolder transaction = ensureTransaction();

		// When dropping tables, we have to make sure that we do this in an order that does not violate any potentially
		// existing foreign key constraints between the tables. In order to do so, we have to start dropping the child
		// (referencing) tables before dropping the parent (referenced) tables
		std::size_t prevNTables = 0;
		std::size_t nTables     = countTables(m_tables);

		// First drop all triggers that are supposed to be dropped before the tables are deleted
		try {
			for (const std::unique_ptr< Table > &currentTable : m_tables) {
				if (!currentTable || !tableExistsInDB(currentTable->getName())) {
					continue;
				}

				for (const Trigger &currentTrigger : currentTable->getTrigger()) {
					if (currentTrigger.created() && currentTrigger.dropBeforeDeleteTable()) {
						m_sql << currentTrigger.dropQuery(*currentTable, m_backend);
					}
				}
			}
		} catch (const soci::soci_error &e) {
			throw AccessException(std::string("Failed at dropping trigger: ") + e.what());
		}

		while (prevNTables != nTables) {
			std::unordered_set< std::string > referencedTables;

			for (const std::unique_ptr< Table > &current : m_tables) {
				if (!current) {
					continue;
				}

				for (const ForeignKey &key : current->getForeignKeys()) {
					// Ignore self-references (the RDMS should be able to take care of those)
					if (key.getForeignTableName() != current->getName()) {
						referencedTables.insert(key.getForeignTableName());
					}
				}
			}

			for (std::unique_ptr< Table > &current : m_tables) {
				if (!current || referencedTables.find(current->getName()) != referencedTables.end()) {
					continue;
				}

				// The current table is not referenced anywhere -> drop it
				if (tableExistsInDB(current->getName())) {
					current->destroy();
				}
				current.reset();
			}

			prevNTables = nTables;
			nTables     = countTables(m_tables);
		}

		assert(nTables == 0);

		transaction.commit();
	}

	void Database::clearTables() {
		TransactionHolder transaction = ensureTransaction();

		for (const std::unique_ptr< Table > &currentTable : m_tables) {
			if (currentTable && tableExistsInDB(currentTable->getName())) {
				currentTable->clear();
			}
		}

		transaction.commit();
	}

	Version Database::getBackendVersion() {
		Version version;
		try {
			switch (m_backend) {
				case Backend::SQLite: {
					std::string versionString;
					m_sql << "SELECT sqlite_version()", soci::into(versionString);

					if (std::sscanf(versionString.c_str(), "%u.%u.%u", &version.m_major, &version.m_minor,
									&version.m_patch)
						!= 3) {
						// Parsing failed
						return {};
					}

					break;
				}
				case Backend::MySQL: {
					std::string versionString;
					m_sql << "SELECT VERSION()", soci::into(versionString);

					if (std::sscanf(versionString.c_str(), "%u.%u.%u", &version.m_major, &version.m_minor,
									&version.m_patch)
						!= 3) {
						// Parsing failed
						return {};
					}

					break;
				}
				case Backend::PostgreSQL: {
					std::string versionString;
					m_sql << "SHOW server_version", soci::into(versionString);

					// Postgres seems to (sometimes) only supply major and minor version, so we're also content with
					// only parsing 2 version segments
					int parsedFields = std::sscanf(versionString.c_str(), "%u.%u.%u", &version.m_major,
												   &version.m_minor, &version.m_patch);
					if (parsedFields != 2 && parsedFields != 3) {
						// Parsing failed
						return {};
					}

					break;
				}
			}
		} catch (const soci::soci_error &e) {
			throw AccessException(std::string("Failed to query DB backend version: ") + e.what());
		}

		return version;
	}

	TransactionHolder Database::ensureTransaction() const {
		return TransactionHolder(m_sql, !m_activeTransaction, &m_activeTransaction);
	}

	void Database::connectToDB(const ConnectionParameter &parameter) {
		assert(parameter.applicability() == m_backend);

		std::string connectionString;
		switch (m_backend) {
			case Backend::SQLite: {
#ifdef MUMBLE_DISABLE_SQLITE
				throw InitException("Tried to connect to SQLite database, but SQLite support is disabled");
#else
				static bool registered = false;
				if (!registered) {
					register_factory_sqlite3();
					registered = true;
				}

				const auto &sqliteParameter = static_cast< const SQLiteConnectionParameter & >(parameter);
				connectionString            = "sqlite3://dbname=" + sqliteParameter.dbPath;
#endif
				break;
			}
			case Backend::MySQL: {
#ifdef MUMBLE_DISABLE_MYSQL
				throw InitException("Tried to connect to MySQL database, but MySQL support is disabled");
#else
				static bool registered      = false;
				if (!registered) {
					register_factory_mysql();
					registered = true;
				}

				const auto &mysqlParameter = static_cast< const MySQLConnectionParameter & >(parameter);

				connectionString = "mysql://dbname='" + mysqlParameter.dbName + "'";

				if (!mysqlParameter.userName.empty()) {
					connectionString += " user='" + mysqlParameter.userName + "'";
				}
				if (!mysqlParameter.password.empty()) {
					connectionString += " password='" + mysqlParameter.password + "'";
				}
				if (!mysqlParameter.host.empty()) {
					connectionString += " host='" + mysqlParameter.host + "'";
				}
				if (!mysqlParameter.port.empty()) {
					connectionString += " port='" + mysqlParameter.port + "'";
				}
#endif
				break;
			}
			case Backend::PostgreSQL: {
#ifdef MUMBLE_DISABLE_POSTGRESQL
				throw InitException("Tried to connect to PostgreSQL database, but PostgreSQL support is disabled");
#else
				static bool registered = false;
				if (!registered) {
					register_factory_postgresql();
					registered = true;
				}

				const auto &postgresqlParameter = static_cast< const PostgreSQLConnectionParameter & >(parameter);

				connectionString = "postgresql://dbname='" + postgresqlParameter.dbName + "'";

				if (!postgresqlParameter.userName.empty()) {
					connectionString += " user='" + postgresqlParameter.userName + "'";
				}
				if (!postgresqlParameter.password.empty()) {
					connectionString += " password='" + postgresqlParameter.password + "'";
				}
				if (!postgresqlParameter.host.empty()) {
					connectionString += " host='" + postgresqlParameter.host + "'";
				}
				if (!postgresqlParameter.port.empty()) {
					connectionString += " port='" + postgresqlParameter.port + "'";
				}
#endif
				break;
			}
		}

		try {
			m_sql.open(connectionString);
		} catch (const soci::soci_error &e) {
			throw InitException(e.what());
		}
	}

	bool Database::tableExistsInDB(const std::string &name) {
		std::string query;
		switch (m_backend) {
			case Backend::SQLite:
				query = "SELECT EXISTS(SELECT 1 FROM sqlite_master WHERE type='table' AND name=:tableName)";
				break;
			case Backend::MySQL:
				// Make sure to restrict our search to the currently selected database (we always assume that we have a
				// specific DB selected)
				query = "SELECT EXISTS(SELECT 1 FROM information_schema.tables WHERE table_schema=(SELECT DATABASE()) "
						"AND table_name=:tableName)";
				break;
			case Backend::PostgreSQL:
				query = "SELECT EXISTS(SELECT 1 FROM information_schema.tables WHERE table_name=:tableName AND "
						"table_catalog=(SELECT CURRENT_DATABASE()))";
				break;
		}

		try {
			int exists = 0;
			m_sql << query, soci::use(name), soci::into(exists);

			return exists;
		} catch (const soci::soci_error &e) {
			// Rethrow
			throw AccessException(e.what());
		}
	}

	std::unordered_set< std::string > Database::getExistingTables() {
		std::string query;
		switch (m_backend) {
			case Backend::SQLite:
				query = "SELECT name FROM sqlite_master WHERE type='table'";
				break;
			case Backend::MySQL:
				query = "SELECT table_name FROM information_schema.tables WHERE table_schema=(SELECT DATABASE())";
				break;
			case Backend::PostgreSQL:
				query = "SELECT table_name FROM information_schema.tables WHERE table_catalog=(SELECT "
						"CURRENT_DATABASE()) AND table_schema = 'public'";
				break;
		}

		try {
			constexpr unsigned int BATCH_SIZE = 20;

			std::unordered_set< std::string > tableNames;
			std::vector< std::string > batch(BATCH_SIZE);
			soci::statement stmt = (m_sql.prepare << query, soci::into(batch));

			stmt.execute(false);

			while (stmt.fetch()) {
				tableNames.reserve(tableNames.size() + batch.size());
				tableNames.insert(batch.begin(), batch.end());

				batch.resize(BATCH_SIZE);
			}

			return tableNames;
		} catch (const soci::soci_error &e) {
			// Rethrow
			throw AccessException(e.what());
		}
	}

	void Database::applyBackendSpecificSetup(const ConnectionParameter &parameter) {
		assert(parameter.applicability() == m_backend);


		try {
			switch (m_backend) {
				case Backend::SQLite: {
					// We don't have to (and can't) set the transaction isolation level for SQLite as it will
					// always use the SERIALIZABLE level (via global read-write locks).
					// See also https://stackoverflow.com/a/34008401

					// Check the DB encoding to make sure that it is Unicode-compatible
					std::string encoding;
					m_sql << "PRAGMA ENCODING", soci::into(encoding);

					if (!boost::iequals(encoding, "UTF-8") && !boost::iequals(encoding, "UTF-16")
						&& !boost::iequals(encoding, "UTF-16le") && !boost::iequals(encoding, "UTF-16be")) {
						throw InitException("Database has invalid encoding \"" + encoding + "\"");
					}

					// Make sure that foreign key constraints are actually enforced
					m_sql << "PRAGMA foreign_keys = ON";
					int enabled;
					m_sql << "PRAGMA foreign_keys", soci::into(enabled);

					if (!enabled) {
						throw InitException("Failed at enabling foreign key enforcement");
					}

					const SQLiteConnectionParameter sqliteParam =
						static_cast< const SQLiteConnectionParameter & >(parameter);
					if (sqliteParam.useWAL) {
						// Below pragma statement returns the new journal mode (or the old one on failure)
						std::string newJournalMode;
						m_sql << "PRAGMA journal_mode=WAL", soci::into(newJournalMode);

						if (!boost::iequals(newJournalMode, "WAL")) {
							throw InitException("Failed at changing journal mode from \"" + newJournalMode
												+ "\" to \"WAL\"");
						}
					}

					break;
				}
				case Backend::MySQL: {
					// Make MySQL as conforming to ANSI standard SQL as possible and disable the "feature" of producing
					// the next auto-incremented value in an auto_increment column when explicitly inserting zero.
					// Also enable strict SQL mode to prevent any Vodoo on invalid values and instead emit an error.
					m_sql << "SET sql_mode = 'ANSI,NO_AUTO_VALUE_ON_ZERO,STRICT_ALL_TABLES '";

					// Set transaction isolation level
					m_sql << "SET TRANSACTION ISOLATION LEVEL READ COMMITTED";


					// Ensure that MySQL uses proper Unicode character encoding (note that "utf8" isn't actual proper
					// UTF8, so we have to use the fixed version "utf8mb4" instead
					m_sql << "ALTER DATABASE CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci";

					// Ensure that log_bin_trust_function_creators is enabled. This is required in order for us to be
					// able to e.g. create triggers, without requiring the SUPER privilege for our user account.
					int enabled = 0;
					m_sql << "SELECT @@GLOBAL.log_bin_trust_function_creators", soci::into(enabled);

					if (!enabled) {
						throw InitException("'log_bin_trust_function_creators' is NOT enabled. Mumble can't operate "
											"properly without it.");
					}

					// Ensure the binlog_format is ROW to make sure, we don't run into issues due to
					// log-bin-trust-function-creators being enabled
					std::string binlogFormat;
					m_sql << "SELECT @@SESSION.binlog_format", soci::into(binlogFormat);

					if (!boost::iequals(binlogFormat, "ROW")) {
						throw InitException(
							"Mumble requires MySQL's binlog_format to be set to \"ROW\" but it is set to \""
							+ binlogFormat + "\"");
					}

					// Set default timezone to UTC
					m_sql << "SET @@SESSION.time_zone = '+00:00'";

					break;
				}
				case Backend::PostgreSQL: {
					Version backendVersion = getBackendVersion();
					if (backendVersion < Version{ 10, 0, 0 }) {
						// Support for identity columns (which we use for auto-incrementing columns) was only added in
						// PostgreSQL 10
						throw InitException(
							"We require at least PostgreSQL v10.0.0 as earlier version don't implement all "
							"necessary features");
					}

					// Set transaction isolation level
					m_sql << "SET DEFAULT_TRANSACTION_ISOLATION TO 'READ COMMITTED'";

					// Check the DB encoding to make sure that it is Unicode-compatible
					std::string encoding;
					m_sql << "SHOW SERVER_ENCODING", soci::into(encoding);

					if (!boost::iequals(encoding, "utf8")) {
						throw InitException("Invalid (non-UTF-8) database encoding encountered: " + encoding);
					}

					// Install the "lo" extension, which is used for managing (B)LOBs - it provides the lo_manage
					// function that we require.
					int lo_installed = false;
					m_sql << "SELECT 1 FROM pg_extension WHERE extname='lo'", soci::into(lo_installed);
					if (!lo_installed) {
						// The "lo" extension was marked "trusted" in Postgresql v13, which means that from then on, we
						// can install this extension without requiring superuser privileges. For older versions, the
						// user will have to do this manually.
						if (backendVersion < Version(13, 0, 0)) {
							throw InitException(
								"The extension \"lo\" is not installed, but is required. For your version of "
								"Postgresql, you'll have to manually install this extension for the Mumble database.");
						} else {
							m_sql << "CREATE EXTENSION \"lo\";";
						}
					}

					// Set default timezone to UTC
					m_sql << "SET timezone TO 'UTC'";

					break;
				}
			}
		} catch (const soci::soci_error &e) {
			throw AccessException(std::string("Applying DB-specific settings failed: ") + e.what());
		}
	}

	void Database::setupStandardTables() {
		// The default implementation does nothing
	}

	void Database::createTables() {
		TransactionHolder transaction = ensureTransaction();

		for (std::unique_ptr< Table > &currentTable : m_tables) {
			if (!currentTable) {
				continue;
			}
			if (currentTable->getName() == MetaTable::NAME) {
				// Meta table is special. We assume it is created separately
				continue;
			}

			currentTable->create();
		}

		transaction.commit();
	}

	void Database::migrateTables(unsigned int fromSchemeVersion, unsigned int toSchemeVersion) {
		std::unordered_set< std::string > tableNames = getExistingTables();
		std::unordered_set< std::string > tablesToBeRemoved;
		tablesToBeRemoved.reserve(tableNames.size());

		TransactionHolder transaction = ensureTransaction();

		// Rename all existing tables
		try {
			for (const std::string &currentTableName : tableNames) {
				if (currentTableName.find("sqlite_") == 0) {
					// SQLite creates some internal tables that we don't want to mess with
					continue;
				}

				m_sql << "ALTER TABLE \"" << currentTableName << "\" RENAME TO \"" << currentTableName
					  << OLD_TABLE_SUFFIX << "\"";

				tablesToBeRemoved.insert(currentTableName + OLD_TABLE_SUFFIX);
			}
		} catch (const soci::soci_error &e) {
			throw AccessException(std::string("Failed at renaming table: ") + e.what());
		}

		for (std::unique_ptr< Table > &currentTable : m_tables) {
			if (currentTable) {
				assert(tablesToBeRemoved.find(currentTable->getName()) == tablesToBeRemoved.end());

				// First make sure the new table is created
				currentTable->create();

				// Then issue a migration step in which the old data is imported into the new one (potentially
				// transforming its format)
				currentTable->migrate(fromSchemeVersion, toSchemeVersion);
			}
		}

		// For dropping these tables we have to consider that potentially there exist foreign keys (and/or other
		// constraints) on these tables. So if we were to simply start dropping tables, we could easily run into
		// constraint violations.
		// The strategy is to keep looping over the tables and attempting to delete them. If there is an error,
		// skip that table and retry in the next iteration. That way we should eventually have deleted all tables
		// in the proper order required to satisfy all constraints.
		std::size_t prevSize = 0;
		do {
			prevSize = tablesToBeRemoved.size();

			auto iter = tablesToBeRemoved.begin();

			while (iter != tablesToBeRemoved.end()) {
				const std::string &currentTable = *iter;

				try {
					m_sql << "DROP TABLE \"" << currentTable << "\"";

					iter = tablesToBeRemoved.erase(iter);
				} catch (const soci::soci_error &) {
					iter++;
				}
			}
		} while (prevSize > tablesToBeRemoved.size());

		if (!tablesToBeRemoved.empty()) {
			throw MigrationException("Failed to delete " + std::to_string(tablesToBeRemoved.size())
									 + " old tables after migration");
		}

		for (std::unique_ptr< Table > &currentTable : m_tables) {
			if (!currentTable) {
				continue;
			}

			currentTable->postMigrationAction(fromSchemeVersion, toSchemeVersion);
		}

		transaction.commit();
	}

	void Database::importMetaData(const nlohmann::json &metaData) {
		if (!metaData.contains("scheme_version")) {
			throw FormatException("JSON-import: meta_data is missing \"scheme_version\" field");
		}
		if (!metaData["scheme_version"].is_number_integer()) {
			throw FormatException("JSON-import: Expected to be \"scheme_version\" field to be of type integer");
		}
		if (metaData["scheme_version"].get< unsigned int >() != getSchemeVersion()) {
			// For the time being, we will only verify that the specified version scheme will match our current scheme
			// version
			throw FormatException(std::string("JSON-import: Can't import data for version scheme ")
								  + std::to_string(metaData["scheme_version"].get< unsigned int >())
								  + " when current scheme version is " + std::to_string(getSchemeVersion()));
		}
	}

	nlohmann::json Database::exportMetaData() const {
		nlohmann::json metaData;
		metaData["scheme_version"] = getSchemeVersion();

		return metaData;
	}

	void Database::init(const ConnectionParameter &parameter, bool createMeta, unsigned int assumedSchemaVersion) {
		assert(parameter.applicability() == m_backend);
		if (parameter.applicability() != m_backend) {
			throw InitException("Supplied connection parameter does not apply to chosen database backend");
		}

		connectToDB(parameter);

		applyBackendSpecificSetup(parameter);

		// Start a transaction to ensure that we don't mess up the DB should anything fail during initialization
		TransactionHolder transaction = ensureTransaction();

		unsigned int schemeVersion = assumedSchemaVersion;
		table_id meta_id           = std::numeric_limits< table_id >::max();
		if (createMeta) {
			// Create a meta-table
			auto metaTable = std::make_unique< MetaTable >(m_sql, m_backend);
			metaTable->setDatabase(this);

			if (!tableExistsInDB(metaTable->getName())) {
				metaTable->create();
			} else {
				// Get scheme version of already existing DB scheme and then set the version
				// of the scheme we are going to apply to the DB
				try {
					// Savepoints are required as PostgreSQL doesn't like it when there is any kind of error during a
					// transaction. Afterwards, any further action other than rolling back the transaction yields
					// "current transaction is aborted" errors. Hence, we need to create a sub-transaction in form of a
					// savepoint that we can indeed roll back in case of error
					Savepoint save(m_sql, "scheme_version_fetch");

					schemeVersion = metaTable->getSchemeVersion();

					save.release();
				} catch (const AccessException &) {
					// If the above has errored, we assume it's because of a mismatch in column names due to the
					// existing Meta table being from before scheme version 10 where it used different column names (and
					// table migration will only happen below).
					Savepoint save(m_sql, "scheme_version_fetch_legacy");

					schemeVersion = metaTable->getSchemeVersionLegacy();

					save.release();
				}
			}

			meta_id = addTable(std::move(metaTable));
		}

		// Make sure all standard tables are added
		setupStandardTables();

		if (schemeVersion == 0) {
			// The scheme version entry does not exist. We assume that this means that this is a freshly created DB
			createTables();
		} else if (schemeVersion < getSchemeVersion()) {
			// The DB is still using an older scheme than we want to use -> perform an upgrade
			migrateTables(schemeVersion, getSchemeVersion());
		} else if (schemeVersion > getSchemeVersion()) {
			// The DB is using a more recent scheme than we want to use -> abort immediately as we don't know how this
			// could possibly look like and we don't want to risk data loss.
			throw InitException(std::string("The existing database is using a more recent scheme version (")
								+ std::to_string(schemeVersion) + ") than the current Mumble instance intends to ("
								+ std::to_string(getSchemeVersion()) + ")");
		} else {
			// The DB's scheme version matches the one we want to use already -> just use as is
		}

		if (createMeta) {
			// Store most up-to-date scheme version in table
			static_cast< MetaTable * >(getTable(meta_id))->setSchemeVersion(getSchemeVersion());
		}

		transaction.commit();
	}


} // namespace db
} // namespace mumble
