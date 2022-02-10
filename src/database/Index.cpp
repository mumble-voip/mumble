// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Index.h"
#include "Table.h"

#include <cassert>

namespace mumble {
namespace db {

	Index::Index(const std::string &name, const std::vector< std::string > &columns)
		: m_name(name), m_columns(columns) {}
	Index::Index(const std::string &name, std::vector< std::string > &&columns)
		: m_name(name), m_columns(std::move(columns)) {}

	const std::string &Index::getName() const { return m_name; }

	const std::vector< std::string > &Index::getColumnNames() const { return m_columns; }

	std::string Index::creationQuery(const Table &table, Backend) const {
		assert(!m_name.empty());
		assert(!m_columns.empty());

		std::string query = "CREATE INDEX \"" + m_name + "\" ON \"" + table.getName() + "\" (";

		for (std::size_t i = 0; i < m_columns.size(); ++i) {
			query += m_columns[i];

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

	bool operator==(const Index &lhs, const Index &rhs) {
		return lhs.m_name == rhs.m_name && lhs.m_columns == rhs.m_columns;
	}

	bool operator!=(const Index &lhs, const Index &rhs) { return !(lhs == rhs); }

} // namespace db
} // namespace mumble
