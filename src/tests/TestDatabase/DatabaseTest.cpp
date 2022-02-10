// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include <QString>
#include <QtTest>

#include "database/AccessException.h"
#include "database/ConnectionParameter.h"
#include "database/Database.h"
#include "database/FormatException.h"
#include "database/Index.h"
#include "database/MetaTable.h"
#include "database/Trigger.h"
#include "database/UnsupportedOperationException.h"

#include "ConstraintTable.h"
#include "DefaultTable.h"
#include "KeyValueTable.h"
#include "TestUtils.h"

#include <nlohmann/json.hpp>

#include <algorithm>
#include <cstring>
#include <vector>

using namespace mumble::db;

std::vector< Backend > backends = {
	Backend::SQLite,
#ifdef MUMBLE_TEST_MYSQL
	Backend::MySQL,
#endif
#ifdef MUMBLE_TEST_POSTGRESQL
	Backend::PostgreSQL,
#endif
};


namespace QTest {
// Provide an overload of toString for JSON objects so that Qt's macros can properly display
// these objects to report failing tests
template<> char *toString(const nlohmann::json &json) {
	std::string str = json.dump(2);
	char *buffer    = new char[str.size() + 1];
	std::strcpy(buffer, str.data());

	return buffer;
}

template<> char *toString(const std::string &str) {
	char *buffer = new char[str.size() + 1];
	std::strcpy(buffer, str.data());

	return buffer;
}

}; // namespace QTest


class TestDatabase : public Database {
public:
	static constexpr const char *EXAMPLE_TABLE_NAME = "sample_table";
	using Database::Database;
	~TestDatabase() override {
		// Clear up everything that we have created in our test case
		this->destroyTables();
	}

	unsigned int getSchemeVersion() const override { return 42; }

	void connect(const ConnectionParameter &param) { Database::connectToDB(param); }

	bool tableExistsInDB(const std::string &name) { return Database::tableExistsInDB(name); }

	soci::session &getSQLHandle() { return m_sql; }
};

class DatabaseTest : public QObject {
	Q_OBJECT;
private slots:
	void connect();
	void getBackendVersion();
	void tableExistsInDB();
	void simpleExport();
	void simpleImport();
	void defaults();
	void constraints();
	void constraintFunctionality();
	void indices();
	void triggers();
	void dataTypes();
	void keyValueTable();
	void unicode();
};

void DatabaseTest::connect() {
	for (Backend currentBackend : backends) {
		qInfo() << "Current backend:" << QString::fromStdString(backendToString(currentBackend));

		TestDatabase db(currentBackend);

		db.connect(test::utils::getConnectionParamter(currentBackend));
	}
}

void DatabaseTest::getBackendVersion() {
	for (Backend currentBackend : backends) {
		qInfo() << "Current backend:" << QString::fromStdString(backendToString(currentBackend));

		TestDatabase db(currentBackend);

		db.connect(test::utils::getConnectionParamter(currentBackend));

		Version defaultVersion = {};
		Version queriedVersion = db.getBackendVersion();

		qInfo() << "Queried version:" << QString::fromStdString(queriedVersion.toString());

		QVERIFY(queriedVersion > defaultVersion);
	}
}

void DatabaseTest::tableExistsInDB() {
	for (Backend currentBackend : backends) {
		qInfo() << "Current backend:" << QString::fromStdString(backendToString(currentBackend));

		TestDatabase db(currentBackend);
		db.init(test::utils::getConnectionParamter(currentBackend));

		QVERIFY(!db.tableExistsInDB("IDontExist"));

		// The meta-table is always created in the DB init code
		QVERIFY(db.tableExistsInDB(MetaTable::NAME));

		// Create a new table (with capital letters in its name) and ensure that we properly recognize it as well
		std::string newTable = "newTable";
		QVERIFY(!db.tableExistsInDB(newTable));
		Database::table_id id = db.addTable(
			std::make_unique< Table >(db.getSQLHandle(), currentBackend, newTable,
									  std::vector< Column >{ Column("dummyCol", DataType(DataType::Integer)) }));
		db.getTable(id)->create();
		QVERIFY(db.tableExistsInDB(newTable));
	}
}

void DatabaseTest::simpleExport() {
	for (Backend currentBackend : backends) {
		qInfo() << "Current backend:" << QString::fromStdString(backendToString(currentBackend));

		TestDatabase db(currentBackend);
		db.init(test::utils::getConnectionParamter(currentBackend));

		MetaTable *metaTable = static_cast< MetaTable * >(db.getTable(MetaTable::NAME));

		metaTable->setSchemeVersion(5);

		// clang-format off
		nlohmann::json expectedJson = {
			{ "tables",
				{
					{ MetaTable::NAME,
						{
							{
								"column_names", { "meta_key", "meta_value" }
							},
							{
								"column_types", { metaTable->findColumn("meta_key")->getType().sqlRepresentation(),
									metaTable->findColumn("meta_value")->getType().sqlRepresentation() }
							},
							{
								"rows", nlohmann::json::array({ { "scheme_version", "5" } })
							}
						}
					}
				}
			},
			{ "meta_data",
				{
				   { "scheme_version", 42 }
				}
			}
		};
		// clang-format on

		nlohmann::json actualJSON = db.exportToJSON();

		test::utils::alignColumnOrder(actualJSON, expectedJson);

		QCOMPARE(db.exportToJSON(), expectedJson);
	}
}

void DatabaseTest::simpleImport() {
	for (Backend currentBackend : backends) {
		qInfo() << "Current backend:" << QString::fromStdString(backendToString(currentBackend));

		TestDatabase db(currentBackend);
		db.init(test::utils::getConnectionParamter(currentBackend));

		QVERIFY(!db.tableExistsInDB("test_table"));

		MetaTable *metaTable = static_cast< MetaTable * >(db.getTable(MetaTable::NAME));

		// clang-format off
		nlohmann::json serializedDB = {
			{ "tables",
				{
					{ MetaTable::NAME,
						{
							{
								"column_names", { "meta_key", "meta_value" }
							},
							{
								"column_types", { metaTable->findColumn("meta_key")->getType().sqlRepresentation(),
									metaTable->findColumn("meta_value")->getType().sqlRepresentation() }
							},
							{
								"rows", nlohmann::json::array({ { "scheme_version", "12" } })
							}
						}
					},
					{ "test_table",
						{
							{
								"column_names", { "col1", "col2", "col3" }
							},
							{
								"column_types", { "INTEGER", "DOUBLE PRECISION", "VARCHAR(100)" }
							},
							{
								"rows", nlohmann::json::array({
										{ 1, 42.42, "I am a test" },
										{ 2, 0.5, "Other test" }
										})
							}
						}
					}
				}
			}, { "meta_data",
				{
					{ "scheme_version", 1 }
				}
			}
		};
		// clang-format on

		QVERIFY(db.getSchemeVersion() != serializedDB["meta_data"]["scheme_version"].get< unsigned int >());

		// The scheme_version specified in meta-data doesn't match, so we expect the import to fail (before creating any
		// tables)
		QVERIFY(db.getTable("test_table") == nullptr);
		QVERIFY_EXCEPTION_THROWN(db.importFromJSON(serializedDB, true), FormatException);
		QVERIFY(!db.tableExistsInDB("test_table"));

		serializedDB["meta_data"]["scheme_version"] = db.getSchemeVersion();

		// Importing into a non-existent table without telling the DB to create the missing tables on-the-fly should
		// result in an error
		QVERIFY(db.getTable("test_table") == nullptr);
		QVERIFY_EXCEPTION_THROWN(db.importFromJSON(serializedDB, false), FormatException);
		QVERIFY(!db.tableExistsInDB("test_table"));

		// Now with the version_scheme fixed and the necessary flag set, the import should succeed
		db.importFromJSON(serializedDB, true);
		QVERIFY(db.tableExistsInDB("test_table"));
		QVERIFY(db.getTable("test_table") != nullptr);


		// Exporting should re-yield the exact same JSON that we fed in to populate the DB in the first place
		// (potentially minus column order)
		nlohmann::json exported = db.exportToJSON();

		test::utils::alignColumnOrder(exported, serializedDB);
		QCOMPARE(exported, serializedDB);
	}
}

void DatabaseTest::defaults() {
	for (Backend currentBackend : backends) {
		qInfo() << "Current backend:" << QString::fromStdString(backendToString(currentBackend));

		TestDatabase db(currentBackend);

		Database::table_id id = db.addTable(std::make_unique< test::DefaultTable >(db.getSQLHandle(), currentBackend));

		db.init(test::utils::getConnectionParamter(currentBackend));

		test::DefaultTable *table = static_cast< test::DefaultTable * >(db.getTable(id));

		QVERIFY(table != nullptr);

		table->insert("randomKey");
		QCOMPARE(table->select("randomKey"), std::string(test::DefaultTable::DEFAULT));
	}
}

void DatabaseTest::constraints() {
	for (Backend currentBackend : backends) {
		qInfo() << "Current backend:" << QString::fromStdString(backendToString(currentBackend));

		TestDatabase db(currentBackend);
		db.init(test::utils::getConnectionParamter(currentBackend));

		MetaTable *meta          = static_cast< MetaTable * >(db.getTable(MetaTable::NAME));
		const Column *metaKeyCol = meta->findColumn("meta_key");
		QVERIFY(metaKeyCol != nullptr);

		std::vector< Constraint > constraints;
		constraints.push_back(Constraint(Constraint::NotNull));                           // unnamed
		constraints.push_back(Constraint(Constraint::NotNull, "my_not_null_constraint")); // named
		constraints.push_back(Constraint(Constraint::Unique));                            // unnamed
		constraints.push_back(Constraint(Constraint::Unique, "my_unique_constraint"));    // named
		constraints.push_back(Constraint(Constraint::PrimaryKey));                        // unnamed
		constraints.push_back(Constraint(Constraint::PrimaryKey, "my_primary_key"));      // named
		constraints.push_back(Constraint(Constraint::ForeignKey, "my_foreign_key", meta,
										 metaKeyCol)); // named (unnamed doesn't work for foreign keys)

		for (const Constraint &currentConstraint : constraints) {
			qInfo() << "  Current constraint: " << currentConstraint.getType()
					<< " name:" << currentConstraint.getName().c_str();

			std::vector< Column > columns;

			Column col("test_col", metaKeyCol->getType(), { currentConstraint });
			columns.push_back(std::move(col));

			std::string tableName = "dummyTable";

			Database::table_id id =
				db.addTable(std::make_unique< Table >(db.getSQLHandle(), currentBackend, tableName, columns));

			Table *table = db.getTable(id);
			QVERIFY(table != nullptr);

			// We always want to start from a clean slate
			QVERIFY(!db.tableExistsInDB(tableName));

			table->create();

			QVERIFY(db.tableExistsInDB(tableName));

			if (currentConstraint.canBeDropped(currentBackend)) {
				// Also attempt removing the constraint again
				// If this doesn't error (throw an exception), we'll assume that it has worked)
				db.getSQLHandle() << table->getColumns()[0].getConstraints()[0].dropQuery(
					*table, table->getColumns()[0], currentBackend);
			}

			// Remove the table again and immediately destroy it
			db.takeTable(id)->destroy();
		}
	}
}

void DatabaseTest::constraintFunctionality() {
	for (Backend currentBackend : backends) {
		qInfo() << "Current backend:" << QString::fromStdString(backendToString(currentBackend));

		TestDatabase db(currentBackend);

		Database::table_id id =
			db.addTable(std::make_unique< test::ConstraintTable >(db.getSQLHandle(), currentBackend));

		db.init(test::utils::getConnectionParamter(currentBackend));

		test::ConstraintTable *table = static_cast< test::ConstraintTable * >(db.getTable(id));
		QVERIFY(table != nullptr);

#define TEST_INSERT_SELECT_ROUNDTRIP(variant, key, value) \
	table->insert##variant(key, value);                   \
	QCOMPARE(table->select##variant(key), std::string(value));

		// Not-Null
		qInfo() << "Testing NOT NULL constraint";

		TEST_INSERT_SELECT_ROUNDTRIP(NotNull, "bla", "blubb");
		// Empty and NULL are different things
		TEST_INSERT_SELECT_ROUNDTRIP(NotNull, "", "blubb");

		// Inserting NULL should error
		QVERIFY_EXCEPTION_THROWN(table->insertNullInNotNullCol(), AccessException);


		// Unique
		qInfo() << "Testing UNIQUE constraint";
		TEST_INSERT_SELECT_ROUNDTRIP(Unique, "keyA", "valueA");
		TEST_INSERT_SELECT_ROUNDTRIP(Unique, "keyB", "valueB");
		TEST_INSERT_SELECT_ROUNDTRIP(Unique, "", "valueEmpty");

		// Inserting any of the above keys again should error
		QVERIFY_EXCEPTION_THROWN(table->insertUnique("keyA", "otherA"), AccessException);
		QVERIFY_EXCEPTION_THROWN(table->insertUnique("keyB", "otherB"), AccessException);
		QVERIFY_EXCEPTION_THROWN(table->insertUnique("", "otherEmpty"), AccessException);


		// PrimaryKey
		qInfo() << "Testing PRIMARY KEY constraint";

		TEST_INSERT_SELECT_ROUNDTRIP(PrimaryKey, "keyA", "valueA");
		TEST_INSERT_SELECT_ROUNDTRIP(PrimaryKey, "keyB", "valueB");
		TEST_INSERT_SELECT_ROUNDTRIP(PrimaryKey, "", "valueEmpty");
		TEST_INSERT_SELECT_ROUNDTRIP(PrimaryKey, "primaryA", "valueA");
		TEST_INSERT_SELECT_ROUNDTRIP(PrimaryKey, "primaryB", "valueB");

		// Inserting any of the above keys again should error
		QVERIFY_EXCEPTION_THROWN(table->insertPrimaryKey("keyA", "otherA"), AccessException);
		QVERIFY_EXCEPTION_THROWN(table->insertPrimaryKey("keyB", "otherB"), AccessException);
		QVERIFY_EXCEPTION_THROWN(table->insertPrimaryKey("", "otherEmpty"), AccessException);
		// Inserting NULL should also error
		QVERIFY_EXCEPTION_THROWN(table->insertNullInPrimaryKeyCol(), AccessException);

		// Deleting an existing key should be fine
		table->deletePrimaryKey("keyA");
		table->deletePrimaryKey("keyB");
		table->deletePrimaryKey("");

		// Fetching one of those keys again should error (because that's what we coded up in ConstraintTable
		// in order to easily verify that above deletes were successful)
		QVERIFY_EXCEPTION_THROWN(table->selectPrimaryKey("keyA"), AccessException);
		QVERIFY_EXCEPTION_THROWN(table->selectPrimaryKey("keyB"), AccessException);
		QVERIFY_EXCEPTION_THROWN(table->selectPrimaryKey(""), AccessException);


		// ForeignKey
		qInfo() << "Testing FOREIGN KEY constraint";

		// Using any key other than the ones that have been added as primary key above should fail.
		// And using one of the deleted key from above should also fail
		QVERIFY_EXCEPTION_THROWN(table->insertForeignKey("RandomKey", "RandomValue"), AccessException);
		QVERIFY_EXCEPTION_THROWN(table->insertForeignKey("keyA", "RandomValue"), AccessException);
		QVERIFY_EXCEPTION_THROWN(table->insertForeignKey("", "RandomValue"), AccessException);

		// Inserting using one of the remaining primary keys should succeed
		TEST_INSERT_SELECT_ROUNDTRIP(ForeignKey, "primaryA", "Some value");
		TEST_INSERT_SELECT_ROUNDTRIP(ForeignKey, "primaryB", "Other value");

		// Duplicates should be fine as well (though we currently have no way of fetching all matches)
		table->insertForeignKey("primaryB", "Duplicate value");

		// Deleting a primary key should also delete all rows referencing that key (as foreign key)
		table->deletePrimaryKey("primaryB");
		QVERIFY_EXCEPTION_THROWN(table->selectForeignKey("primaryB"), AccessException);

		// The other entry should still be there, though
		QCOMPARE(table->selectForeignKey("primaryA"), std::string("Some value"));

#undef TEST_INSERT_SELECT_ROUNDTRIP
	}
}

void DatabaseTest::indices() {
	for (Backend currentBackend : backends) {
		qInfo() << "Current backend:" << QString::fromStdString(backendToString(currentBackend));

		TestDatabase db(currentBackend);

		Database::table_id id = db.addTable(
			std::make_unique< Table >(db.getSQLHandle(), currentBackend, "dummyTable",
									  std::vector< Column >{ Column("colA", DataType(DataType::SmallInteger)),
															 Column("colB", DataType(DataType::Double)) }));

		db.init(test::utils::getConnectionParamter(currentBackend));

		Table *table       = db.getTable(id);
		const Column *colA = table->findColumn("colA");
		const Column *colB = table->findColumn("colB");
		QVERIFY(colA != nullptr);
		QVERIFY(colB != nullptr);

		// We assume that if the addition of the index doesn't throw, it has been added successfully

		Index singleIndex("test_index", { colA->getName() });
		table->addIndex(singleIndex, true);

		Index multiIndex("other_index", { colA->getName(), colB->getName() });
		table->addIndex(multiIndex, true);

		// Adding the same index again should error
		QVERIFY_EXCEPTION_THROWN(table->addIndex(singleIndex, true), AccessException);
		QVERIFY_EXCEPTION_THROWN(table->addIndex(multiIndex, true), AccessException);

		// The failed additions shouldn't have changed anything
		QCOMPARE(table->getIndices().size(), static_cast< std::size_t >(2));

		// Now the removal should work just fine
		QVERIFY(table->removeIndex(singleIndex, true));
		QVERIFY(table->removeIndex(multiIndex, true));

		// A second removal should fail (the error should be caught before accessing the DB though - thus no exception
		// should be thrown)
		QVERIFY(!table->removeIndex(singleIndex, true));
		QVERIFY(!table->removeIndex(multiIndex, true));

		// Re-adding without applying and the removing with applying should now error with an exception
		table->addIndex(singleIndex, false);
		QVERIFY_EXCEPTION_THROWN(table->removeIndex(singleIndex, true), AccessException);
	}
}

void DatabaseTest::triggers() {
	for (Backend currentBackend : backends) {
		qInfo() << "Current backend:" << QString::fromStdString(backendToString(currentBackend));

		TestDatabase db(currentBackend);

		Database::table_id id = db.addTable(
			std::make_unique< Table >(db.getSQLHandle(), currentBackend, "dummyTable",
									  std::vector< Column >{ Column("colA", DataType(DataType::SmallInteger)),
															 Column("colB", DataType(DataType::Double)) }));

		db.init(test::utils::getConnectionParamter(currentBackend));

		Table *table       = db.getTable(id);
		const Column *colA = table->findColumn("colA");
		QVERIFY(colA != nullptr);

		for (Trigger::Timing timing : { Trigger::Timing::Before, Trigger::Timing::After }) {
			qInfo() << "Current timing:" << static_cast< int >(timing);
			for (Trigger::Event event : { Trigger::Event::Insert, Trigger::Event::Update, Trigger::Event::Delete }) {
				qInfo() << "Current event:" << static_cast< int >(event);

				Trigger trigger("myTestTrigger", timing, event,
								"UPDATE \"" + table->getName() + "\" SET " + colA->getName() + "=1;");

				table->addTrigger(trigger, true);

				// Adding again should error
				QVERIFY_EXCEPTION_THROWN(table->addTrigger(trigger, true), AccessException);

				QVERIFY(table->removeTrigger(trigger, true));

				// Removing again should fail (without error though)
				QVERIFY(!table->removeTrigger(trigger, true));

				// Adding without applying and then removing with applying should error
				table->addTrigger(trigger, false);
				QVERIFY_EXCEPTION_THROWN(table->removeTrigger(trigger, true), AccessException);
			}
		}
	}
}

void DatabaseTest::dataTypes() {
	for (Backend currentBackend : backends) {
		qInfo() << "Current backend:" << QString::fromStdString(backendToString(currentBackend));

		TestDatabase db(currentBackend);

		db.init(test::utils::getConnectionParamter(currentBackend));

		for (DataType::Type currentType : { DataType::Integer, DataType::SmallInteger, DataType::Double,
											DataType::FixedSizeString, DataType::String }) {
			qInfo() << "Current data type:" << currentType;

			for (bool sized : { true, false }) {
				if ((!DataType::canBeSized(currentType) && sized) || (!DataType::canBeUnsized(currentType) && !sized)) {
					continue;
				}

				qInfo() << "Sized:" << sized;

				DataType type(currentType, sized ? 42 : DataType::Unsized);

				Column col1("dummyCol1", type);
				Column col2("dummyCol2", type);

				Database::table_id id = db.addTable(std::make_unique< Table >(
					db.getSQLHandle(), currentBackend, "dummyTable", std::vector< Column >{ col1, col2 }));

				Table *table = db.getTable(id);
				QVERIFY(table != nullptr);

				// If we can create the table without errors, that means that the used datatypes seem to be fine
				table->create();

				table->destroy();

				db.takeTable(id);
			}
		}
	}
}

void DatabaseTest::keyValueTable() {
	// This test ensures that our KeyValueTable helper class actually works as expected
	for (Backend currentBackend : backends) {
		qInfo() << "Current backend:" << QString::fromStdString(backendToString(currentBackend));

		TestDatabase db(currentBackend);

		Database::table_id id = db.addTable(std::make_unique< test::KeyValueTable >(db.getSQLHandle(), currentBackend));

		db.init(test::utils::getConnectionParamter(currentBackend));

		test::KeyValueTable *table = static_cast< test::KeyValueTable * >(db.getTable(id));
		QVERIFY(table != nullptr);

		table->insert("test", "value");
		QCOMPARE(table->query("test"), std::string("value"));
	}
}

void DatabaseTest::unicode() {
	for (Backend currentBackend : backends) {
		qInfo() << "Current backend:" << QString::fromStdString(backendToString(currentBackend));

		TestDatabase db(currentBackend);

		Database::table_id id = db.addTable(std::make_unique< test::KeyValueTable >(db.getSQLHandle(), currentBackend));

		db.init(test::utils::getConnectionParamter(currentBackend));

		test::KeyValueTable *table = static_cast< test::KeyValueTable * >(db.getTable(id));
		QVERIFY(table != nullptr);

		for (const std::string &symbol :
			 { "Ä", "è", "ß", "¼", "💚", "🚒", "ձ", "Φ", "ヲ", "𐌒", "Ю", "¥", "ℏ", "ℜ", "¯_(ツ)_/¯" }) {
			qInfo() << "Current symbol:" << QString::fromStdString(symbol);

			std::string unicodeValue = "Some string with " + symbol;
			std::string unicodeKey   = "Key with " + symbol;

			table->insert("testKey", unicodeValue);
			QCOMPARE(table->query("testKey"), unicodeValue);
			table->clear();

			table->insert(unicodeKey, "TestValue");
			QCOMPARE(table->query(unicodeKey), std::string("TestValue"));
			table->clear();

			table->insert(unicodeKey, unicodeValue);
			QCOMPARE(table->query(unicodeKey), unicodeValue);
			table->clear();

			table->insert(symbol, symbol);
			QCOMPARE(table->query(symbol), symbol);
			table->clear();
		}
	}
}

QTEST_MAIN(DatabaseTest)
#include "DatabaseTest.moc"
