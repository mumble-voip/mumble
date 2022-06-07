// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "MetaTable.h"
#include "AccessException.h"
#include "Column.h"
#include "DataType.h"
#include "InitException.h"
#include "PrimaryKey.h"

#include <soci/soci.h>

#include "FormatException.h"
#include <nlohmann/json.hpp>

namespace mumble {
namespace db {

	constexpr const char *MetaTable::NAME;

	MetaTable::MetaTable(soci::session &sql, Backend backend) : Table(sql, backend, MetaTable::NAME) {
		std::vector< Column > columns;
		columns.push_back(Column("meta_key", DataType(DataType::VarChar, 500), { Constraint(Constraint::NotNull) }));
		columns.push_back(Column("meta_value", DataType(DataType::VarChar, 5000), { Constraint(Constraint::NotNull) }));

		setColumns(columns);

		PrimaryKey pk(columns[0]);
		setPrimaryKey(pk);
	}

	unsigned int MetaTable::getSchemeVersion() {
		try {
			unsigned int version = 0;
			m_sql << "SELECT meta_value FROM " << m_name << " WHERE meta_key='version'", soci::into(version);

			return version;
		} catch (const soci::soci_error &e) {
			throw AccessException(e.what());
		}
	}

	void MetaTable::setSchemeVersion(unsigned int version) {
		try {
			m_sql << "INSERT INTO \"" << m_name << "\" (meta_key, meta_value) VALUES ('scheme_version', :version)",
				soci::use(version);
		} catch (const soci::soci_error &e) {
			throw AccessException(e.what());
		}
	}

} // namespace db
} // namespace mumble
