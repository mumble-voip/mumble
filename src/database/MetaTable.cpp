// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "MetaTable.h"
#include "AccessException.h"
#include "Column.h"
#include "DataType.h"
#include "Database.h"
#include "MigrationException.h"
#include "PrimaryKey.h"
#include "Table.h"

#include <soci/soci.h>

#include <cassert>
#include <optional>
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
		std::optional< std::string > strVersion = queryKey("scheme_version");

		return strVersion ? static_cast< unsigned int >(std::stoi(strVersion.value())) : 0;
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

	std::optional< std::string > MetaTable::queryKey(const std::string &key) {
		try {
			std::string value;
			// Note: We are deliberately not wrapping the column names in double quotes as this prevents errors to be
			// emitted if these columns don't exist when using SQLite. The reason being that SQLite accepts strings
			// wrapped in double quotes and hence reads the column names as strings.
			// See https://sqlite.org/quirks.html#double_quoted_string_literals_are_accepted
			// However, we rely on this throwing an error when querying the scheme version from Database::init.
			m_sql << "SELECT " << column::value << " FROM " << m_name << " WHERE " << column::key << " = :key",
				soci::use(key), soci::into(value);

			return m_sql.got_data() ? std::optional< std::string >(value) : std::nullopt;
		} catch (const soci::soci_error &e) {
			throw AccessException(e.what());
		}
	}

	void MetaTable::migrate(unsigned int fromSchemeVersion, unsigned int toSchemeVersion) {
		// Note: Always hard-code table and column names in this function in order to ensure that this
		// migration path always stays the same regardless of whether the respective named constants change.
		assert(fromSchemeVersion <= toSchemeVersion);

		try {
			if (fromSchemeVersion < 10) {
				// In v10 we renamed the columns "keystring" -> "meta_key" and "value" -> "meta_value"
				// -> Import all data from the old table into the new one
				m_sql << "INSERT INTO \"" << getName() << "\" (\"" << column::key << "\", \"" << column::value
					  << "\") SELECT \"keystring\", \"value\" FROM \"meta" << Database::OLD_TABLE_SUFFIX << "\"";

				// Furthermore, the schema version is no longer stored under "version" but under "scheme_version"
				m_sql << "UPDATE \"" << getName() << "\" SET \"" << column::key << "\" = 'scheme_version' WHERE \""
					  << column::key << "\" = 'version'";
			} else {
				// Use default implementation to handle migration without change of format
				Table::migrate(fromSchemeVersion, toSchemeVersion);
			}
		} catch (const soci::soci_error &) {
			std::throw_with_nested(MigrationException(std::string("Failed at migrating table \"") + NAME
													  + "\" from scheme version " + std::to_string(fromSchemeVersion)
													  + " to " + std::to_string(toSchemeVersion)));
		}
	}

	unsigned int MetaTable::getSchemeVersionLegacy() {
		try {
			std::string value;
			// Note: We are deliberately not wrapping the column names in double quotes as this prevents errors to be
			// emitted if these columns don't exist when using SQLite. The reason being that SQLite accepts strings
			// wrapped in double quotes and hence reads the column names as strings.
			// See https://sqlite.org/quirks.html#double_quoted_string_literals_are_accepted
			// However, we rely on this throwing an error when querying the scheme version from Database::init.
			m_sql << "SELECT value FROM meta WHERE keystring = 'version'", soci::into(value);

			return m_sql.got_data() ? static_cast< unsigned int >(std::stoi(value)) : 0;
		} catch (const soci::soci_error &e) {
			throw AccessException(std::string("During legacy scheme version fetching: ") + e.what());
		}
	}

} // namespace db
} // namespace mumble
