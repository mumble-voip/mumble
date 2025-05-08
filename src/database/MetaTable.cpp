// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "MetaTable.h"
#include "AccessException.h"
#include "Column.h"
#include "DataType.h"
#include "FormatException.h"
#include "InitException.h"
#include "PrimaryKey.h"

#include <boost/optional.hpp>

#include <soci/soci.h>

#include <string>

namespace mumble {
namespace db {

	constexpr const char *MetaTable::NAME;

	MetaTable::MetaTable(soci::session &sql, Backend backend) : Table(sql, backend, MetaTable::NAME) {
		std::vector< Column > columns;
		columns.push_back(Column(column::key, DataType(DataType::VarChar, 500), { Constraint(Constraint::NotNull) }));
		columns.push_back(
			Column(column::value, DataType(DataType::VarChar, 5000), { Constraint(Constraint::NotNull) }));

		setColumns(columns);

		PrimaryKey pk(columns[0]);
		setPrimaryKey(pk);
	}

	unsigned int MetaTable::getSchemeVersion() {
		boost::optional< std::string > strVersion = queryKey("scheme_version");

		return strVersion ? static_cast< unsigned int >(std::stoi(strVersion.get())) : 0;
	}

	void MetaTable::setSchemeVersion(unsigned int version) { setKey("scheme_version", std::to_string(version)); }

	void MetaTable::setKey(const std::string &key, const std::string &value) {
		try {
			int containsKey = false;
			m_sql << "SELECT 1 FROM \"" << m_name << "\" WHERE \"" << column::key << "\" = :key", soci::use(key),
				soci::into(containsKey);

			if (containsKey) {
				m_sql << "UPDATE \"" << m_name << "\" SET \"" << column::value << "\" = :value WHERE \"" << column::key
					  << "\" = :key",
					soci::use(value), soci::use(key);
			} else {
				m_sql << "INSERT INTO \"" << m_name << "\" (\"" << column::key << "\", \"" << column::value
					  << "\") VALUES (:key, :value)",
					soci::use(key), soci::use(value);
			}
		} catch (const soci::soci_error &e) {
			throw AccessException(e.what());
		}
	}

	boost::optional< std::string > MetaTable::queryKey(const std::string &key) {
		try {
			std::string value;
			m_sql << "SELECT \"" << column::value << "\" FROM " << m_name << " WHERE \"" << column::key << "\" = :key",
				soci::use(key), soci::into(value);

			return m_sql.got_data() ? boost::optional< std::string >(value) : boost::none;
		} catch (const soci::soci_error &e) {
			throw AccessException(e.what());
		}
	}

} // namespace db
} // namespace mumble
