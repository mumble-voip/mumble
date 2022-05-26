// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "PrimaryKey.h"
#include "Column.h"

#include <cassert>

namespace mumble {
namespace db {

	PrimaryKey::PrimaryKey() : PrimaryKey(std::vector< std::string >{}) {}

	PrimaryKey::PrimaryKey(const Column &column) { m_columnNames.push_back(column.getName()); }

	PrimaryKey::PrimaryKey(const std::vector< const Column * > &columns) {
		m_columnNames.reserve(columns.size());

		for (const Column *current : columns) {
			m_columnNames.push_back(current->getName());
		}
	}

	PrimaryKey::PrimaryKey(const std::vector< std::string > &columnNames) : m_columnNames(columnNames) {}

	const std::vector< std::string > &PrimaryKey::getColumnNames() const { return m_columnNames; }

	void PrimaryKey::setColumnNames(const std::vector< std::string > &columnNames) { m_columnNames = columnNames; }

	bool PrimaryKey::isCompositeKey() const { return m_columnNames.size() > 1; }

	bool PrimaryKey::isValid() const { return !m_columnNames.empty(); }

	std::string PrimaryKey::sql() const {
		assert(isValid());

		std::string sql = "PRIMARY KEY (";

		for (std::size_t i = 0; i < m_columnNames.size(); ++i) {
			sql += m_columnNames[i];

			if (i + 1 < m_columnNames.size()) {
				sql += ", ";
			}
		}

		sql += ")";

		return sql;
	}

} // namespace db
} // namespace mumble
