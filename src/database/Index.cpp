// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Index.h"
#include "Database.h"
#include "Table.h"

#include <cassert>

namespace mumble {
namespace db {

	Index::Index(const std::string &name, const std::vector< std::string > &columns, int flags)
		: m_name(name), m_columns(columns), m_flags(flags) {}
	Index::Index(const std::string &name, std::vector< std::string > &&columns, int flags)
		: m_name(name), m_columns(std::move(columns)), m_flags(flags) {}

	const std::string &Index::getName() const { return m_name; }

	const std::vector< std::string > &Index::getColumnNames() const { return m_columns; }

	std::string Index::creationQuery(const Table &table, Backend) const {
		assert(!m_name.empty());
		assert(!m_columns.empty());

		std::string query = "CREATE ";
		if (m_flags & Flag::UNIQUE) {
			query += "UNIQUE ";
		}
		query += "INDEX \"" + m_name + "\" ON \"" + table.getName() + "\" (";

		for (std::size_t i = 0; i < m_columns.size(); ++i) {
			query += "\"" + m_columns[i] + "\"";

			if (i + 1 < m_columns.size()) {
				query += ", ";
			}
		}

		query += ")";

		return query;
	}

	std::string Index::dropQuery(const Table &table, Backend backend) const {
		switch (backend) {
			case Backend::SQLite:
				// Fallthrough
			case Backend::PostgreSQL:
				return "DROP INDEX \"" + m_name + "\"";
			case Backend::MySQL:
				return "DROP INDEX \"" + m_name + "\" ON \"" + table.getName() + "\"";
		}

		// This code should be unreachable
		assert(false);
		return "";
	}

	std::string Index::existsQuery(const Table &table, Backend backend) const {
		switch (backend) {
			case Backend::SQLite:
				// Note: we explicitly don't include the associated table's name in the query as index
				// names in SQLite are global (instead of scoped per table)
				return "SELECT 1 FROM sqlite_master WHERE type = 'index' AND name = '" + m_name + "'";
			case Backend::PostgreSQL:
				// Note: index names are global (for a given schema), hence the table's name
				// does not appear in the query
				return "SELECT 1 FROM pg_indexes WHERE indexname = '" + m_name + "'";
			case Backend::MySQL:
				// Note: the statistics table contains information about table indices
				return "SELECT 1 FROM information_schema.statistics WHERE table_schema=(SELECT DATABASE()) AND "
					   "table_name = '"
					   + table.getName() + "' AND index_name = '" + m_name + "'";
		}

		// This code should be unreachable
		assert(false);
		return "";
	}

	bool operator==(const Index &lhs, const Index &rhs) {
		return lhs.m_name == rhs.m_name && lhs.m_columns == rhs.m_columns;
	}

	bool operator!=(const Index &lhs, const Index &rhs) { return !(lhs == rhs); }

} // namespace db
} // namespace mumble
