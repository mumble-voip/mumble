// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "IntegerTable.h"
#include "database/AccessException.h"
#include "database/Column.h"
#include "database/Constraint.h"
#include "database/DataType.h"
#include "database/PrimaryKey.h"
#include "database/Utils.h"

#include <soci/soci.h>

namespace mumble {
namespace db {
	namespace test {

		constexpr const char *IntegerTable::NAME;
		constexpr const char *IntegerTable::column::int_col;
		constexpr const char *IntegerTable::column::second_col;
		constexpr int IntegerTable::lowestID_unconstrained;
		constexpr int IntegerTable::lowestID_second_empty;
		constexpr int IntegerTable::lowestID_second_bla;

		IntegerTable::IntegerTable(soci::session &sql, Backend backend) : Table(sql, backend, NAME) {
			std::vector< Column > columns;

			columns.push_back(
				Column(column::int_col, DataType(DataType::Integer), { Constraint(Constraint::NotNull) }));
			columns.push_back(Column(column::second_col, DataType(DataType::VarChar, 255)));

			setColumns(columns);

			PrimaryKey pk(columns[0]);
			setPrimaryKey(pk);
		}

		void IntegerTable::init() {
			insert(3);
			insert(4);
			insert(5);
			insert(20);

			insert(0, "bla");
			insert(1, "bla");
			insert(2, "bla");
			insert(6, "bla");
			insert(12, "bla");
		}

		void IntegerTable::insert(int value, const std::string &secondVal) {
			try {
				m_sql << "INSERT INTO \"" << getName() << "\" (" << column::int_col << ", " << column::second_col
					  << ") VALUES (:i, :second)",
					soci::use(value), soci::use(secondVal);
			} catch (const soci::soci_error &e) {
				throw AccessException(std::string("Failed at inserting into IntegerTable") + e.what());
			}
		}

		int IntegerTable::selectMinFreeID(const std::string &sqlStatement, const std::string &secondColAlias,
										  const std::string *secondVal) {
			try {
				int value;

				if (secondVal) {
					m_sql << sqlStatement, soci::use(*secondVal, secondColAlias), soci::into(value);
				} else {
					m_sql << sqlStatement, soci::into(value);
				}

				::mumble::db::utils::verifyQueryResultedInData(m_sql);

				return value;
			} catch (const soci::soci_error &e) {
				throw AccessException(std::string("Failed at getting minimal free ID: ") + e.what());
			}
		}
	} // namespace test
} // namespace db
} // namespace mumble
