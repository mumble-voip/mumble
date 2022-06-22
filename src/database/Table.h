// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_DATABASE_TABLE_H_
#define MUMBLE_DATABASE_TABLE_H_

#include "Backend.h"
#include "Column.h"
#include "ForeignKey.h"
#include "Index.h"
#include "PrimaryKey.h"
#include "Trigger.h"

#include <string>
#include <unordered_set>
#include <vector>

#include <nlohmann/json_fwd.hpp>

namespace soci {
class session;
};

namespace mumble {
namespace db {

	class Table {
	public:
		static constexpr const char *BACKUP_SUFFIX = "_backup";

		Table(soci::session &sql, Backend backend);
		Table(soci::session &sql, Backend backend, const std::string &name);
		Table(soci::session &sql, Backend backend, const std::string &name, const std::vector< Column > &columns);
		virtual ~Table() = default;

		const std::string &getName() const;
		void setName(const std::string &name);

		const std::vector< Column > &getColumns() const;
		void setColumns(const std::vector< Column > &columns);
		const Column *findColumn(const std::string &name) const;
		bool containsColumn(const std::string &name) const;

		virtual void create();
		/**
		 * Migrate this table to the new scheme version. Note that this function assumes that a transaction has already
		 * been started when this function is called. Note also that when this function is called, the original table
		 * containing the legacy data has been renamed to have the suffix Database::OLD_TABLE_SUFFIX.
		 *
		 * @param fromSchemeVersion The scheme version of the existing DB
		 * @param toSchemeVersion The scheme version to migrate to
		 */
		virtual void migrate(unsigned int fromSchemeVersion, unsigned int toSchemeVersion);
		virtual void destroy();
		virtual void clear();

		const std::vector< Index > &getIndices() const;
		void addIndex(const Index &index, bool applyToDB);
		bool removeIndex(const Index &index, bool applyToDB);

		const std::vector< Trigger > &getTrigger() const;
		void addTrigger(const Trigger &trigger, bool applyToDB);
		bool removeTrigger(const Trigger &trigger, bool applyToDB);

		bool hasPrimaryKey() const;
		const PrimaryKey &getPrimaryKey() const;
		void setPrimaryKey(const PrimaryKey &key);

		const std::vector< ForeignKey > &getForeignKeys() const;
		void addForeignKey(const ForeignKey &key);
		void removeForeignKey(const ForeignKey &key);
		void clearForeignKeys();

		/**
		 * Imports the data from the given JSON into the table represented by this object. Note
		 * that the caller of this function is expected to already have initiated a database
		 * transaction to prevent a partial import.
		 *
		 * @param json The data to import
		 * @param create Whether this function shall create the represented table in the associated
		 * database before starting to import data.
		 */
		virtual void importFromJSON(const nlohmann::json &json, bool create = false);
		virtual nlohmann::json exportToJSON();

	protected:
		std::string m_name;
		std::vector< Column > m_columns;
		soci::session &m_sql;
		Backend m_backend;
		std::vector< Index > m_indices;
		std::vector< Trigger > m_trigger;
		PrimaryKey m_primaryKey;
		std::vector< ForeignKey > m_foreignKeys;

		void performCtorAssertions();
	};

} // namespace db
} // namespace mumble

#endif // MUMBLE_DATABASE_TABLE_H_
