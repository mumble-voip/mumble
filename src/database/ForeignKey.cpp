// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ForeignKey.h"
#include "Column.h"
#include "Table.h"

#include <cassert>

namespace mumble {
namespace db {

	ForeignKey::ForeignKey() {}

	const std::vector< std::string > &ForeignKey::getForeignColumnNames() const { return m_foreignColumnNames; }

	const std::vector< std::string > &ForeignKey::getColumnNames() const { return m_columnNames; }

	const std::string &ForeignKey::getForeignTableName() const { return m_foreignTableName; }

	void ForeignKey::setColumns(const Table &foreignTable, const std::vector< const Column * > &columns) {
		assert(foreignTable.hasPrimaryKey());
		assert(foreignTable.getPrimaryKey().getColumnNames().size() == columns.size());

#ifndef NDEBUG
		for (std::size_t i = 0; i < columns.size(); ++i) {
			const Column *foreignColumn = foreignTable.findColumn(foreignTable.getPrimaryKey().getColumnNames()[i]);

			// Assert that column exists
			assert(foreignColumn);

			// Assert that the column's type matches that of the foreign column
			assert(foreignColumn->getType() == columns[i]->getType());
		}
#endif

		m_foreignTableName = foreignTable.getName();

		m_columnNames.clear();
		m_columnNames.reserve(columns.size());

		for (const Column *currentCol : columns) {
			m_columnNames.push_back(currentCol->getName());
		}
	}

} // namespace db
} // namespace mumble
