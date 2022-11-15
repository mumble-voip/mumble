// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Utils.h"

#include <cassert>

namespace mumble {
namespace db {
	namespace utils {

		std::string triggerErrorStatement(Backend backend, const std::string &errorMsg) {
			switch (backend) {
				case Backend::SQLite:
					return "SELECT RAISE(ABORT, '" + errorMsg + "')";
				case Backend::MySQL:
					return "SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = '" + errorMsg + "'";
				case Backend::PostgreSQL:
					return "RAISE EXCEPTION '" + errorMsg + "'";
			}

			// Should be unreachable
			assert(false);
			return {};
		}

		ColAlias::ColAlias(std::string colName) : colName(colName), colAlias(std::move(colName)) {}
		ColAlias::ColAlias(std::string colName, std::string colAlias)
			: colName(std::move(colName)), colAlias(std::move(colAlias)) {}

		std::string getLowestUnoccupiedIDStatement(Backend backend, const std::string &tableName,
												   const std::string &idColName,
												   const std::vector< ColAlias > &additionalCols) {
			(void) backend;

			std::string restrictions;
			for (const ColAlias &currentCol : additionalCols) {
				restrictions += " AND " + currentCol.colName + " = :" + currentCol.colAlias;
			}

			// Taken and adapted from https://stackoverflow.com/a/907300
			// clang-format off
			return
				// Part I: Use an ID of zero, if this ID is not yet taken
				  "SELECT " + idColName + " FROM "
				+ "("
				+	"SELECT 0 AS " + idColName
				+ ") AS \"helperTable1\" "
				+ "WHERE NOT EXISTS "
				+ "("
				+	"SELECT 1 FROM \"" + tableName + "\" "
				+		"WHERE " + idColName + " = 0" + restrictions
				+ ")"
				// Combine Part I + II
				+ "UNION "
				// Part II: Find the ID that is not immediately followed by an entry for ID + 1 and then use that ID + 1
				+ "SELECT * FROM "
				+	"("
				+		"SELECT " + idColName + " + 1 FROM \"" + tableName + "\" AS t1 "
				+		"WHERE NOT EXISTS "
				+			"("
				+				"SELECT 1 FROM \"" + tableName + "\" AS t2 WHERE t2." + idColName + " = t1." + idColName + " + 1" + restrictions
				+			")"
				+		"ORDER BY " + idColName + " "
				+		"LIMIT 1"
				+	") AS \"helperTable2\" "
				+ "ORDER BY " + idColName + " "
				+ "LIMIT 1";
			// clang-format on
		}

		std::string limitOffset(Backend backend, const std::string &limit, const std::string &offset) {
			std::string statement = "LIMIT " + limit;

			switch (backend) {
				case Backend::MySQL:
				case Backend::PostgreSQL:
				case Backend::SQLite:
					statement += " OFFSET ";
					break;
			}

			statement += offset;

			return statement;
		}

	} // namespace utils
} // namespace db
} // namespace mumble
