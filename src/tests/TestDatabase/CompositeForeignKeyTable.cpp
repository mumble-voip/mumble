// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "CompositeForeignKeyTable.h"
#include "database/AccessException.h"
#include "database/Column.h"
#include "database/DataType.h"
#include "database/ForeignKey.h"
#include "database/Utils.h"

#include <vector>

#include <soci/soci.h>

namespace mumble {
namespace db {
	namespace test {

		constexpr const char *CompositeForeignKeyTable::NAME;

		CompositeForeignKeyTable::CompositeForeignKeyTable(soci::session &sql, Backend backend,
														   const Table &foreignTable)
			: Table(sql, backend, NAME) {
			std::vector< Column > columns;

			columns.push_back(Column("fk_keyA", DataType(DataType::VarChar, 100), { Constraint(Constraint::NotNull) }));
			columns.push_back(Column("fk_keyB", DataType(DataType::VarChar, 100), { Constraint(Constraint::NotNull) }));
			columns.push_back(
				Column("fk_value", DataType(DataType::VarChar, 100), { Constraint(Constraint::NotNull) }));

			setColumns(columns);

			ForeignKey key(foreignTable, std::vector< const Column * >{ &columns[0], &columns[1] });
			addForeignKey(key);
		}

		void CompositeForeignKeyTable::insert(const std::string &keyA, const std::string &keyB,
											  const std::string &value) {
			try {
				m_sql << "INSERT INTO \"" + getName() + "\" (fk_keyA, fk_keyB, fk_value) VALUES (:keyA, :keyB, :value)",
					soci::use(keyA), soci::use(keyB), soci::use(value);
			} catch (const soci::soci_error &e) {
				throw AccessException(std::string("Failed at inserting: ") + e.what());
			}
		}

		void CompositeForeignKeyTable::insertNullA() {
			try {
				m_sql << "INSERT INTO \"" + getName() + "\" (fk_keyA, fk_keyB, fk_value) VALUES (NULL, '42', 'dummy')";
			} catch (const soci::soci_error &e) {
				throw AccessException(std::string("Failed at inserting NULL value: ") + e.what());
			}
		}

		void CompositeForeignKeyTable::insertNullB() {
			try {
				m_sql << "INSERT INTO \"" + getName() + "\" (fk_keyA, fk_keyB, fk_value) VALUES ('42', NULL, 'dummy')";
			} catch (const soci::soci_error &e) {
				throw AccessException(std::string("Failed at inserting NULL value: ") + e.what());
			}
		}

		std::string CompositeForeignKeyTable::select(const std::string &keyA, const std::string &keyB) {
			std::string value;
			try {
				m_sql << "SELECT fk_value FROM \"" + getName() + "\" WHERE fk_keyA = :keyA AND fk_keyB = :keyB",
					soci::use(keyA), soci::use(keyB), soci::into(value);
			} catch (const soci::soci_error &e) {
				throw AccessException(std::string("Failed at querying value: ") + e.what());
			}

			db::utils::verifyQueryResultedInData(m_sql);

			return value;
		}

		void CompositeForeignKeyTable::dropKey(const std::string &keyA, const std::string &keyB) {
			try {
				m_sql << "DELETE FROM \"" + getName() + "\" WHERE fk_keyA = :keyA AND fk_keyB = :keyB", soci::use(keyA),
					soci::use(keyB);
			} catch (const soci::soci_error &e) {
				throw AccessException(std::string("Failed at dropping key: ") + e.what());
			}
		}

	} // namespace test
} // namespace db
} // namespace mumble
