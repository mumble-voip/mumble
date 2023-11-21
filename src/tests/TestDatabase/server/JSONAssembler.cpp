// Copyright 2023 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "JSONAssembler.h"
#include "JSONLocator.h"

#include "murmur/database/ServerDatabase.h"

#include "database/Backend.h"
#include "database/Column.h"
#include "database/DataType.h"
#include "database/Table.h"

#include <cassert>
#include <cstring>
#include <fstream>
#include <utility>

#include <nlohmann/json.hpp>

namespace msdb = ::mumble::server::db;
namespace mdb  = ::mumble::db;

namespace mumble {
namespace db {
	namespace test {

		JSONAssembler::JSONAssembler() {
			// Load data from disk to memory
			for (const std::pair< const char *, const char * > &currentPaths : locateJSONTables()) {
				const char *inputPath    = currentPaths.first;
				const char *migratedPath = currentPaths.second;

				std::ifstream inputDataStream(inputPath);
				std::ifstream migratedDataStream(migratedPath);

				DataPair pair;
				pair.inputData    = nlohmann::json::parse(inputDataStream);
				pair.migratedData = nlohmann::json::parse(migratedDataStream);

				m_rawData.push_back(std::move(pair));
			}
		}

		unsigned int getMaxVersion(const nlohmann::json json) {
			unsigned int maxVersion = 0;

			for (const auto &currentNode : json.items()) {
				if (currentNode.value().is_object() && currentNode.key().find('v') == 0) {
					unsigned int version = static_cast< unsigned int >(std::stoi(currentNode.key().substr(1)));

					maxVersion = std::max(maxVersion, version);
				}
			}

			return maxVersion;
		}

		const nlohmann::json &selectSchemeVersion(const nlohmann::json &json, unsigned int version) {
			unsigned int selectedVersion = 0;
			for (unsigned int currentVersion = version; currentVersion > 0; --currentVersion) {
				if (json.contains("v" + std::to_string(currentVersion))) {
					selectedVersion = currentVersion;
					break;
				}
			}

			if (selectedVersion == 0) {
				throw std::runtime_error("Did not find a data definition for scheme version <= "
										 + std::to_string(version));
			}

			return json["v" + std::to_string(selectedVersion)];
		}

		void writeTableMetadata(nlohmann::json &targetTableRep, const nlohmann::json &sourceRep,
								const ::mdb::Table &table, ::mdb::Backend backend) {
			// The column names can simply be copied as-is
			targetTableRep["column_names"] = sourceRep.at("column_names");
			targetTableRep["column_types"] = nlohmann::json::array();

			if (sourceRep.contains("column_types")) {
				// Import column types from given data (and apply conversions to match representation of current
				// backend)
				for (const nlohmann::json &currentType : sourceRep["column_types"]) {
					DataType type = ::mdb::DataType::fromSQLRepresentation(currentType.get< std::string >());
					if (backend != Backend::MySQL) {
						// We have to hack in order to allow size-specification for MySQL without
						// breaking Postgres
						type.setSize(DataType::Unsized);
					}
					targetTableRep["column_types"].push_back(type.sqlRepresentation(backend));
				}
			} else {
				// Read the column types from the actual table definition
				for (const nlohmann::json &currentColName : targetTableRep["column_names"]) {
					const ::mdb::Column *column = table.findColumn(currentColName.get< std::string >());

					if (!column) {
						throw std::runtime_error("Unable to locate column \"" + currentColName.get< std::string >()
												 + "\" in table data for \"" + table.getName() + "\"");
					}

					targetTableRep["column_types"].push_back(column->getType().sqlRepresentation(backend));
				}
			}
		}


		void writeTableRows(nlohmann::json &tableRep, const nlohmann::json tableData, unsigned int schemeVersion,
							std::string &tableName) {
			const nlohmann::json &versionedData = selectSchemeVersion(tableData, schemeVersion);

			if (versionedData.contains("inherit")) {
				const nlohmann::json &inheritBlock = versionedData.at("inherit");
				std::string base                   = inheritBlock.at("from").get< std::string >();
				assert(base.size() > 1);
				// Remove leading "v"
				base.erase(base.begin());
				unsigned int baseVersion = std::stoi(base);

				// Import inherited data
				writeTableRows(tableRep, tableData, baseVersion, tableName);

				if (inheritBlock.contains("substitute")) {
					// Perform substitutions
					const nlohmann::json &substituteBlock = inheritBlock.at("substitute");

					if (substituteBlock.contains("table_name")) {
						// Replace table name
						tableName = substituteBlock.at("table_name").get< std::string >();
					}

					if (substituteBlock.contains("column_names")) {
						// Replace all given column names by their replacement
						for (auto iter = substituteBlock["column_names"].begin();
							 iter != substituteBlock["column_names"].end(); ++iter) {
							std::string replace = iter.key();
							std::string with    = iter.value().get< std::string >();

							std::replace(tableRep["column_names"].begin(), tableRep["column_names"].end(), replace,
										 with);
						}
					}
				}

				if (inheritBlock.contains("append")) {
					const nlohmann::json &appendBlock = inheritBlock.at("append");

					if (appendBlock.contains("rows")) {
						for (const nlohmann::json &currentRow : appendBlock["rows"]) {
							tableRep["rows"].push_back(currentRow);
						}
					}

					if (appendBlock.contains("columns")) {
						// Append new column and add default value for all existing rows
						for (auto iter = appendBlock["columns"].begin(); iter != appendBlock["columns"].end(); ++iter) {
							std::string columnName = iter.key();
							std::string columnType = iter->at("type").get< std::string >();
							nlohmann::json columnDefault =
								iter->contains("default") ? iter->at("default") : nlohmann::json();

							tableRep["column_names"].push_back(columnName);
							tableRep["column_types"].push_back(columnType);

							for (nlohmann::json &currentRow : tableRep["rows"]) {
								currentRow.push_back(columnDefault);
							}
						}
					}
				}
			}

			if (versionedData.contains("rows")) {
				// Overwrite rows
				tableRep["rows"] = versionedData["rows"];
			}

			if (versionedData.contains("column_names")) {
				// Overwrite column names
				tableRep["column_names"] = versionedData["column_names"];
			}

			if (versionedData.contains("column_types")) {
				// Overwrite column types
				// TODO: Should we use fromSQLRepresentation here to ensure we arrive at valid types?
				// Maybe this should also be a preprocessing step
				tableRep["column_types"] = versionedData["column_types"];
			}
		}


		JSONAssembler::DataPair JSONAssembler::buildTestData(unsigned int fromSchemeVersion,
															 const ::msdb::ServerDatabase &serverDB) {
			DataPair pair;

			pair.inputData["meta_data"]    = { { "scheme_version", fromSchemeVersion } };
			pair.migratedData["meta_data"] = { { "scheme_version", msdb::ServerDatabase::DB_SCHEME_VERSION } };

			pair.inputData["tables"]    = nlohmann::json::object();
			pair.migratedData["tables"] = nlohmann::json::object();

			nlohmann::json &inputTables    = pair.inputData["tables"];
			nlohmann::json &migratedTables = pair.migratedData["tables"];

			for (const DataPair &currentTable : m_rawData) {
				const nlohmann::json &inputData    = selectSchemeVersion(currentTable.inputData, fromSchemeVersion);
				const nlohmann::json &migratedData = selectSchemeVersion(currentTable.migratedData, fromSchemeVersion);

				assert(!migratedData.empty());

				std::string migratedTableName;
				nlohmann::json currentMigratedTable;

				writeTableRows(currentMigratedTable, currentTable.migratedData, fromSchemeVersion, migratedTableName);

				if (migratedTableName.empty()) {
					migratedTableName = migratedData.at("table_name").get< std::string >();
				}

				const ::mdb::Table *table = serverDB.getTable(migratedTableName);

				if (!table) {
					throw std::runtime_error("Table \"" + migratedTableName
											 + "\" does not exist in the most up-to-date scheme definition");
				}

				writeTableMetadata(currentMigratedTable, migratedData, *table, serverDB.getBackend());

				migratedTables[migratedTableName] = std::move(currentMigratedTable);


				// The table might not have existed at all in the given scheme version. If that's the case,
				// we don't actually add it to the list of old tables
				if (!inputData.empty()) {
					nlohmann::json currentInputTable;
					std::string inputTableName;

					writeTableRows(currentInputTable, currentTable.inputData, fromSchemeVersion, inputTableName);

					if (inputTableName.empty()) {
						inputTableName = inputData.at("table_name").get< std::string >();
					}

					writeTableMetadata(currentInputTable, inputData, *table, serverDB.getBackend());

					inputTables[inputTableName] = std::move(currentInputTable);
				}
			}

			return pair;
		}

	} // namespace test
} // namespace db
} // namespace mumble
