// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "CompositePrimaryKeyTable.h"
#include "database/AccessException.h"
#include "database/Column.h"
#include "database/DataType.h"
#include "database/PrimaryKey.h"
#include "database/Utils.h"

#include <vector>

#include <soci/soci.h>

namespace mumble {
namespace db {
	namespace test {

		constexpr const char *CompositePrimaryKeyTable::NAME;

		CompositePrimaryKeyTable::CompositePrimaryKeyTable(soci::session &sql, Backend backend)
			: Table(sql, backend, NAME) {
			std::vector< Column > columns;

			columns.push_back(Column("pk_keyA", DataType(DataType::String, 100), { Constraint(Constraint::NotNull) }));
			columns.push_back(Column("pk_keyB", DataType(DataType::String, 100), { Constraint(Constraint::NotNull) }));
			columns.push_back(Column("pk_value", DataType(DataType::String, 100), { Constraint(Constraint::NotNull) }));

			setColumns(columns);

			PrimaryKey pk(std::vector< const Column * >{ &columns[0], &columns[1] });
			setPrimaryKey(pk);
		}

		void CompositePrimaryKeyTable::insert(const std::string &keyA, const std::string &keyB,
											  const std::string &value) {
			try {
				m_sql << "INSERT INTO \"" + getName() + "\" (pk_keyA, pk_keyB, pk_value) VALUES (:keyA, :keyB, :value)",
					soci::use(keyA), soci::use(keyB), soci::use(value);
			} catch (const soci::soci_error &e) {
				throw AccessException(std::string("Failed at inserting: ") + e.what());
			}
		}

		void CompositePrimaryKeyTable::insertNullA() {
			try {
				m_sql << "INSERT INTO \"" + getName() + "\" (pk_keyA, pk_keyB, pk_value) VALUES (NULL, '42', 'dummy')";
			} catch (const soci::soci_error &e) {
				throw AccessException(std::string("Failed at inserting NULL value: ") + e.what());
			}
		}

		void CompositePrimaryKeyTable::insertNullB() {
			try {
				m_sql << "INSERT INTO \"" + getName() + "\" (pk_keyA, pk_keyB, pk_value) VALUES ('42', NULL, 'dummy')";
			} catch (const soci::soci_error &e) {
				throw AccessException(std::string("Failed at inserting NULL value: ") + e.what());
			}
		}

		std::string CompositePrimaryKeyTable::select(const std::string &keyA, const std::string &keyB) {
			std::string value;
			try {
				m_sql << "SELECT pk_value FROM \"" + getName() + "\" WHERE pk_keyA = :keyA AND pk_keyB = :keyB",
					soci::use(keyA), soci::use(keyB), soci::into(value);
			} catch (const soci::soci_error &e) {
				throw AccessException(std::string("Failed at querying value: ") + e.what());
			}

			db::utils::verifyQueryResultedInData(m_sql);

			return value;
		}

		void CompositePrimaryKeyTable::dropKey(const std::string &keyA, const std::string &keyB) {
			try {
				m_sql << "DELETE FROM \"" + getName() + "\" WHERE pk_keyA = :keyA AND pk_keyB = :keyB", soci::use(keyA),
					soci::use(keyB);
			} catch (const soci::soci_error &e) {
				throw AccessException(std::string("Failed at dropping key: ") + e.what());
			}
		}

	} // namespace test
} // namespace db
} // namespace mumble
