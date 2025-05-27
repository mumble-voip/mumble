// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include <QString>
#include <QtTest>

#include "ExceptionUtils.h"

#include "database/AccessException.h"
#include "database/ConnectionParameter.h"
#include "database/Database.h"
#include "database/FormatException.h"
#include "database/Index.h"
#include "database/MetaTable.h"
#include "database/Savepoint.h"
#include "database/Trigger.h"
#include "database/UnsupportedOperationException.h"
#include "database/Utils.h"

#include "AutoIncrementTable.h"
#include "CompositeForeignKeyTable.h"
#include "CompositePrimaryKeyTable.h"
#include "ConstraintTable.h"
#include "DefaultTable.h"
#include "ForeignKeyTable.h"
#include "IntegerTable.h"
#include "KeyValueTable.h"
#include "PrimaryKeyTable.h"
#include "TestUtils.h"

#include <nlohmann/json.hpp>

#include <algorithm>
#include <cctype>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace mumble::db;

#if QT_VERSION < QT_VERSION_CHECK(6, 3, 0)
#	define QVERIFY_THROWS_EXCEPTION(kind, expression) QVERIFY_EXCEPTION_THROWN(expression, kind)
#endif

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

template<> char *toString(const std::vector< std::uint8_t > &vec) {
	std::stringstream sstream;
	sstream << std::setfill('0') << std::setw(2) << std::hex;

	sstream << "{ ";
	for (std::size_t i = 0; i < vec.size(); ++i) {
		sstream << "0x" << static_cast< unsigned int >(vec[i]);

		if (i + 1 < vec.size()) {
			sstream << ", ";
		}
	}

	sstream << "}";

	std::string str = sstream.str();

	char *buffer = new char[str.size() + 1];
	std::strcpy(buffer, str.data());

	return buffer;
}

template<> char *toString(const std::unordered_set< std::string > &set) {
	std::stringstream stream;

	stream << "{ ";
	for (auto it = set.begin(); it != set.end(); ++it) {
		if (it != set.begin()) {
			stream << ", ";
		}

		stream << "\"" << *it << "\"";
	}

	stream << " }";

	std::string str = stream.str();

	char *buffer = new char[str.size() + 1];
	std::strcpy(buffer, str.data());

	return buffer;
}

} // namespace QTest


class TestDB : public Database {
public:
	static constexpr const char *EXAMPLE_TABLE_NAME = "sample_table";
	using Database::Database;
	~TestDB() override {
		if (m_connected) {
			// Clear up everything that we have created in our test case
			try {
				this->destroyTables();
			} catch (const Exception &e) {
				std::cerr << "Exception encountered while destroying tables:" << std::endl;
				mumble::printExceptionMessage(std::cerr, e);
			}
		}
	}

	unsigned int getSchemeVersion() const override { return 42; }

	void connect(const ConnectionParameter &param) {
		Database::connectToDB(param);
		m_connected = true;
	}

	void init(const ConnectionParameter &param) override {
		Database::init(param);
		m_connected = true;
	}

	bool tableExistsInDB(const std::string &name) { return Database::tableExistsInDB(name); }

	std::unordered_set< std::string > getExistingTables() { return Database::getExistingTables(); }

	soci::session &getSQLHandle() { return m_sql; }

private:
	bool m_connected = false;
};

class DatabaseTest : public QObject {
	Q_OBJECT
private slots:
	void hexConversions();
	void connect();
	void getBackendVersion();
	void tableExistsInDB();
	void getExistingTables();
	void simpleExport();
	void simpleImport();
	void defaults();
	void autoIncrement();
	void constraints();
	void constraintFunctionality();
	void keys();
	void indices();
	void triggers();
	void dataTypes();
	void keyValueTable();
	void unicode();
	void fetchMinimumFreeID();
	void dateToEpoch();
	void savepoints();
};

void DatabaseTest::hexConversions() {
	qInfo() << "Hex to binary";

	std::string hex                           = "abcd";
	std::vector< std::uint8_t > expectedBytes = { 0xab, 0xcd };
	QCOMPARE(expectedBytes, utils::hexToBinary< decltype(expectedBytes) >(hex));

	// Using a prefix shouldn't change the result
	hex = "0x" + hex;
	QCOMPARE(expectedBytes, utils::hexToBinary< decltype(expectedBytes) >(hex));

	// Case shouldn't matter
	std::transform(hex.begin(), hex.end(), hex.begin(), [](char c) { return std::toupper(c); });
	QCOMPARE(expectedBytes, utils::hexToBinary< decltype(expectedBytes) >(hex));

	hex                                         = "0xFf12640003";
	std::array< std::uint8_t, 5 > expectedArray = { 0xff, 0x12, 0x64, 0x00, 0x03 };
	QCOMPARE(expectedArray, utils::hexToBinary< decltype(expectedArray) >(hex));

	hex                                      = "0x";
	std::array< std::uint8_t, 0 > emptyArray = {};
	QCOMPARE(emptyArray, utils::hexToBinary< decltype(emptyArray) >(hex));

	bool success = true;
	// Too little input
	utils::hexToBinary< 1 >("", &success);
	QVERIFY(!success);
	success = true;
	// invalid character
	utils::hexToBinary< 1 >("ak", &success);
	QVERIFY(!success);
	success = true;
	// Too much input
	utils::hexToBinary< 1 >("ab4f", &success);
	QVERIFY(!success);
	success = true;
	// Invalid number of characters
	utils::hexToBinary< decltype(expectedArray) >("ab4", &success);
	QVERIFY(!success);
	success = true;


	qInfo() << "Binary to hex";

	std::string expectedHex = "0xabcd";
	QCOMPARE(expectedHex, utils::binaryToHex(std::vector< std::uint8_t >{ 0xab, 0xcd }));
	expectedHex = "0x";
	QCOMPARE(expectedHex, utils::binaryToHex(std::vector< std::uint8_t >{}));
	expectedHex = "0x000001";
	QCOMPARE(expectedHex, utils::binaryToHex(std::vector< std::uint8_t >{ 0x00, 0x00, 0x01 }));


	qInfo() << "Consistency checks";

	for (const std::vector< std::uint8_t > &current :
		 std::vector< std::vector< std::uint8_t > >{ { 0xaf, 0x12 }, {}, { 0x26, 0xff, 0x78, 0xf9 } }) {
		success = false;
		std::vector< std::uint8_t > produced =
			utils::hexToBinary< decltype(produced) >(utils::binaryToHex(current), &success);

		QVERIFY(success);
		QCOMPARE(current, produced);
	}
}

void DatabaseTest::connect() {
	MUMBLE_BEGIN_TEST_CASE_NO_INIT

	db.connect(test::utils::getConnectionParamter(currentBackend));

	MUMBLE_END_TEST_CASE
}

void DatabaseTest::getBackendVersion() {
	MUMBLE_BEGIN_TEST_CASE

	Version defaultVersion = {};
	Version queriedVersion = db.getBackendVersion();

	qInfo() << "Queried version:" << QString::fromStdString(queriedVersion.toString());

	QVERIFY(queriedVersion > defaultVersion);

	MUMBLE_END_TEST_CASE
}

void DatabaseTest::tableExistsInDB() {
	MUMBLE_BEGIN_TEST_CASE

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

	MUMBLE_END_TEST_CASE
}

void DatabaseTest::getExistingTables() {
	MUMBLE_BEGIN_TEST_CASE

	std::unordered_set< std::string > tableNames = db.getExistingTables();

	// The meta table is always created
	QCOMPARE(tableNames, std::unordered_set< std::string >{ MetaTable::NAME });

	MUMBLE_END_TEST_CASE
}

void DatabaseTest::simpleExport() {
	MUMBLE_BEGIN_TEST_CASE

	MetaTable *metaTable = static_cast< MetaTable * >(db.getTable(MetaTable::NAME));

	metaTable->setSchemeVersion(5);
	QCOMPARE(metaTable->getSchemeVersion(), static_cast< unsigned int >(5));

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
								"column_types", { metaTable->findColumn("meta_key")->getType().sqlRepresentation(currentBackend),
									metaTable->findColumn("meta_value")->getType().sqlRepresentation(currentBackend) }
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
	test::utils::alignRowOrder(actualJSON, expectedJson);

	QCOMPARE(db.exportToJSON(), expectedJson);

	MUMBLE_END_TEST_CASE
}

void DatabaseTest::simpleImport() {
	MUMBLE_BEGIN_TEST_CASE

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
								"column_types", { metaTable->findColumn("meta_key")->getType().sqlRepresentation(currentBackend),
									metaTable->findColumn("meta_value")->getType().sqlRepresentation(currentBackend) }
							},
							{
								"rows", nlohmann::json::array({ { "scheme_version", "12" } })
							}
						}
					},
					{ "test_table",
						{
							{
								"column_names", { "col1", "col2", "col3", "col4" }
							},
							{
								"column_types", { "INTEGER", "DOUBLE PRECISION", "VARCHAR(100)", DataType(DataType::Blob).sqlRepresentation(currentBackend) }
							},
							{
								"rows", nlohmann::json::array({
									{ 1, 42.42, "I am a test", "0x234504252ca1000000000567567323b5" },
									{ 2, 0.5, "Other test", nlohmann::json{} }
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
	QVERIFY_THROWS_EXCEPTION(FormatException, db.importFromJSON(serializedDB, true));
	QVERIFY(!db.tableExistsInDB("test_table"));

	serializedDB["meta_data"]["scheme_version"] = db.getSchemeVersion();

	// Importing into a non-existent table without telling the DB to create the missing tables on-the-fly should
	// result in an error
	QVERIFY(db.getTable("test_table") == nullptr);
	QVERIFY_THROWS_EXCEPTION(FormatException, db.importFromJSON(serializedDB, false));
	QVERIFY(!db.tableExistsInDB("test_table"));

	// Now with the version_scheme fixed and the necessary flag set, the import should succeed
	db.importFromJSON(serializedDB, true);
	QVERIFY(db.tableExistsInDB("test_table"));
	QVERIFY(db.getTable("test_table") != nullptr);


	// Exporting should re-yield the exact same JSON that we fed in to populate the DB in the first place
	// (potentially minus column order)
	nlohmann::json exported = db.exportToJSON();

	test::utils::alignColumnOrder(exported, serializedDB);
	test::utils::alignRowOrder(exported, serializedDB);
	QCOMPARE(exported, serializedDB);

	MUMBLE_END_TEST_CASE
}

void DatabaseTest::defaults() {
	MUMBLE_BEGIN_TEST_CASE_NO_INIT

	Database::table_id id = db.addTable(std::make_unique< test::DefaultTable >(db.getSQLHandle(), currentBackend));

	db.init(test::utils::getConnectionParamter(currentBackend));

	test::DefaultTable *table = static_cast< test::DefaultTable * >(db.getTable(id));

	QVERIFY(table != nullptr);

	table->insert("randomKey");
	QCOMPARE(table->select("randomKey"), std::string(test::DefaultTable::DEFAULT));

	MUMBLE_END_TEST_CASE
}

void DatabaseTest::autoIncrement() {
	MUMBLE_BEGIN_TEST_CASE_NO_INIT

	Database::table_id id =
		db.addTable(std::make_unique< test::AutoIncrementTable >(db.getSQLHandle(), currentBackend));

	db.init(test::utils::getConnectionParamter(currentBackend));

	test::AutoIncrementTable *table = static_cast< test::AutoIncrementTable * >(db.getTable(id));

	QVERIFY(table != nullptr);

	table->insert("randomKey");
	table->insert("otherKey");
	// We don't know whether the col will start at e.g. 0 or one, but we expect
	// the subsequent entry to be bigger by exactly one.
	QCOMPARE(table->select("randomKey") + 1, table->select("otherKey"));

	MUMBLE_END_TEST_CASE
}

void DatabaseTest::constraints() {
	MUMBLE_BEGIN_TEST_CASE

	MetaTable *meta          = static_cast< MetaTable * >(db.getTable(MetaTable::NAME));
	const Column *metaKeyCol = meta->findColumn("meta_key");
	QVERIFY(metaKeyCol != nullptr);

	std::vector< Constraint > constraints;
	constraints.push_back(Constraint(Constraint::NotNull));
	constraints.push_back(Constraint(Constraint::Unique));

	for (const Constraint &currentConstraint : constraints) {
		qInfo() << "  Current constraint: " << currentConstraint.getType();

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

		// Remove the table again and immediately destroy it
		db.takeTable(id)->destroy();
	}

	MUMBLE_END_TEST_CASE
}

void DatabaseTest::constraintFunctionality() {
	MUMBLE_BEGIN_TEST_CASE_NO_INIT

	Database::table_id id = db.addTable(std::make_unique< test::ConstraintTable >(db.getSQLHandle(), currentBackend));

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
	QVERIFY_THROWS_EXCEPTION(AccessException, table->insertNullInNotNullCol());


	// Unique
	qInfo() << "Testing UNIQUE constraint";
	TEST_INSERT_SELECT_ROUNDTRIP(Unique, "keyA", "valueA");
	TEST_INSERT_SELECT_ROUNDTRIP(Unique, "keyB", "valueB");
	TEST_INSERT_SELECT_ROUNDTRIP(Unique, "", "valueEmpty");

	// Inserting any of the above keys again should error
	QVERIFY_THROWS_EXCEPTION(AccessException, table->insertUnique("keyA", "otherA"));
	QVERIFY_THROWS_EXCEPTION(AccessException, table->insertUnique("keyB", "otherB"));
	QVERIFY_THROWS_EXCEPTION(AccessException, table->insertUnique("", "otherEmpty"));

#undef TEST_INSERT_SELECT_ROUNDTRIP

	MUMBLE_END_TEST_CASE
}

void DatabaseTest::keys() {
	MUMBLE_BEGIN_TEST_CASE_NO_INIT

	Database::table_id primKeyID =
		db.addTable(std::make_unique< test::PrimaryKeyTable >(db.getSQLHandle(), currentBackend));
	Database::table_id compPrimKeyID =
		db.addTable(std::make_unique< test::CompositePrimaryKeyTable >(db.getSQLHandle(), currentBackend));

	test::PrimaryKeyTable *primaryKeyTable = static_cast< test::PrimaryKeyTable * >(db.getTable(primKeyID));
	test::CompositePrimaryKeyTable *compositePKTable =
		static_cast< test::CompositePrimaryKeyTable * >(db.getTable(compPrimKeyID));
	QVERIFY(primaryKeyTable != nullptr);
	QVERIFY(compositePKTable != nullptr);


	Database::table_id foreignKeyID =
		db.addTable(std::make_unique< test::ForeignKeyTable >(db.getSQLHandle(), currentBackend, *primaryKeyTable));
	Database::table_id compForeignKeyID = db.addTable(
		std::make_unique< test::CompositeForeignKeyTable >(db.getSQLHandle(), currentBackend, *compositePKTable));

	test::ForeignKeyTable *foreignKeyTable = static_cast< test::ForeignKeyTable * >(db.getTable(foreignKeyID));
	test::CompositeForeignKeyTable *compositeFKTable =
		static_cast< test::CompositeForeignKeyTable * >(db.getTable(compForeignKeyID));
	QVERIFY(foreignKeyTable != nullptr);
	QVERIFY(compositeFKTable != nullptr);

	db.init(test::utils::getConnectionParamter(currentBackend));


#define TEST_INSERT_SELECT_ROUNDTRIP(table, key, value) \
	table->insert(key, value);                          \
	QCOMPARE(table->select(key), std::string(value));

	qInfo() << "Primary key";

	TEST_INSERT_SELECT_ROUNDTRIP(primaryKeyTable, "keyA", "valueA");
	TEST_INSERT_SELECT_ROUNDTRIP(primaryKeyTable, "keyB", "valueA");

	// Re-inserting an existing key again should result in an error
	QVERIFY_THROWS_EXCEPTION(AccessException, primaryKeyTable->insert("keyA", "bla"));

	primaryKeyTable->dropKey("keyA");

	// Now re-inserting it should work
	TEST_INSERT_SELECT_ROUNDTRIP(primaryKeyTable, "keyA", "otherValueA");

	// Since primary keys imply NOT NULL, inserting NULL into it should error
	QVERIFY_THROWS_EXCEPTION(AccessException, primaryKeyTable->insertNull());

#undef TEST_INSERT_SELECT_ROUNDTRIP

	qInfo() << "Composite primary key";

#define TEST_INSERT_SELECT_ROUNDTRIP(table, keyA, keyB, value) \
	table->insert(keyA, keyB, value);                          \
	QCOMPARE(table->select(keyA, keyB), std::string(value));

	// Duplicating the value of either column on its own is fine
	TEST_INSERT_SELECT_ROUNDTRIP(compositePKTable, "A1", "B1", "value11");
	TEST_INSERT_SELECT_ROUNDTRIP(compositePKTable, "A1", "B2", "value12");
	TEST_INSERT_SELECT_ROUNDTRIP(compositePKTable, "A2", "B1", "value21");
	TEST_INSERT_SELECT_ROUNDTRIP(compositePKTable, "A2", "B2", "value22");

	// Only duplicating a pair of values errors
	QVERIFY_THROWS_EXCEPTION(AccessException, compositePKTable->insert("A1", "B1", "other11"));

	compositePKTable->dropKey("A1", "B1");

	// Re-inserting should be fine
	TEST_INSERT_SELECT_ROUNDTRIP(compositePKTable, "A1", "B1", "other11");

#undef TEST_INSERT_SELECT_ROUNDTRIP


	qInfo() << "Foreign key";
#define TEST_INSERT_SELECT_ROUNDTRIP(table, key, value) \
	table->insert(key, value);                          \
	QCOMPARE(table->select(key), std::string(value));

	// Insertion with any of the keys also present in primaryKeyTable should be fine
	TEST_INSERT_SELECT_ROUNDTRIP(foreignKeyTable, "keyA", "firstValue");
	TEST_INSERT_SELECT_ROUNDTRIP(foreignKeyTable, "keyB", "secondValue");

	// Attempting to insert any key not used in primaryKeyTable should error
	QVERIFY_THROWS_EXCEPTION(AccessException, foreignKeyTable->insert("Non-existing primary key", "Bla"));

	// Deleting a primary key from primaryKeyTable should also remove it from foreignKeyTable
	primaryKeyTable->dropKey("keyA");
	QVERIFY_THROWS_EXCEPTION(AccessException, foreignKeyTable->select("keyA"));

	// Inserting duplicates into a FK table should be fine
	foreignKeyTable->insert("keyB", "anotherValue");

#undef TEST_INSERT_SELECT_ROUNDTRIP

	qInfo() << "Composite foreign key";
#define TEST_INSERT_SELECT_ROUNDTRIP(table, keyA, keyB, value) \
	table->insert(keyA, keyB, value);                          \
	QCOMPARE(table->select(keyA, keyB), std::string(value));

	// Inserting using any of the primary key pairs in compositePKTable should work
	TEST_INSERT_SELECT_ROUNDTRIP(compositeFKTable, "A1", "B1", "fvalue11");
	TEST_INSERT_SELECT_ROUNDTRIP(compositeFKTable, "A1", "B2", "fvalue12");
	TEST_INSERT_SELECT_ROUNDTRIP(compositeFKTable, "A2", "B1", "fvalue21");
	TEST_INSERT_SELECT_ROUNDTRIP(compositeFKTable, "A2", "B2", "fvalue22");

	// Inserting anything else should fail
	QVERIFY_THROWS_EXCEPTION(AccessException, compositeFKTable->insert("A1", "C1", "Bla"));
	QVERIFY_THROWS_EXCEPTION(AccessException, compositeFKTable->insert("C1", "A1", "Bla"));

	// Deleting a primary key from compositePKTable should also remove the respective rows from compositeFKTable
	compositePKTable->dropKey("A1", "B1");
	QVERIFY_THROWS_EXCEPTION(AccessException, compositeFKTable->select("A1", "B1"));

	// Inserting duplicates into a FK table should be fine
	compositeFKTable->insert("A2", "B2", "anotherValue");

#undef TEST_INSERT_SELECT_ROUNDTRIP

	MUMBLE_END_TEST_CASE
}

void DatabaseTest::indices() {
	MUMBLE_BEGIN_TEST_CASE_NO_INIT

	Database::table_id id =
		db.addTable(std::make_unique< Table >(db.getSQLHandle(), currentBackend, "dummyTable",
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
	QVERIFY_THROWS_EXCEPTION(AccessException, table->addIndex(singleIndex, true));
	QVERIFY_THROWS_EXCEPTION(AccessException, table->addIndex(multiIndex, true));

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
	QVERIFY_THROWS_EXCEPTION(AccessException, table->removeIndex(singleIndex, true));
	table->removeIndex(singleIndex, false);


	// Unique indices
	Index uniqueIndex("unique_idx", { colA->getName() });
	table->addIndex(uniqueIndex, true);
	table->removeIndex(uniqueIndex, true);

	Index multiUniqueIndex("multi_unique_idx", { colA->getName(), colB->getName() });
	table->addIndex(multiUniqueIndex, true);
	table->removeIndex(multiUniqueIndex, true);

	MUMBLE_END_TEST_CASE
}

void DatabaseTest::triggers() {
	MUMBLE_BEGIN_TEST_CASE_NO_INIT

	Database::table_id id =
		db.addTable(std::make_unique< Table >(db.getSQLHandle(), currentBackend, "dummyTable",
											  std::vector< Column >{ Column("colA", DataType(DataType::SmallInteger)),
																	 Column("colB", DataType(DataType::Double)) }));

	db.init(test::utils::getConnectionParamter(currentBackend));

	Table *table       = db.getTable(id);
	const Column *colA = table->findColumn("colA");
	QVERIFY(colA != nullptr);
	const Column *colB = table->findColumn("colB");
	QVERIFY(colB != nullptr);

	for (Trigger::Timing timing : { Trigger::Timing::Before, Trigger::Timing::After }) {
		qInfo() << "Current timing:" << static_cast< int >(timing);
		for (Trigger::Event event : { Trigger::Event::Insert, Trigger::Event::Update, Trigger::Event::Delete }) {
			qInfo() << "Current event:" << static_cast< int >(event);

			Trigger trigger("myTestTrigger", timing, event,
							"UPDATE \"" + table->getName() + "\" SET " + colA->getName() + "=1;");

			table->addTrigger(trigger, true);

			// Adding again should error
			QVERIFY_THROWS_EXCEPTION(AccessException, table->addTrigger(trigger, true));

			QVERIFY(table->removeTrigger(trigger, true));

			// Removing again should fail (without error though)
			QVERIFY(!table->removeTrigger(trigger, true));

			// Adding without applying and then removing with applying should error
			table->addTrigger(trigger, false);
			QVERIFY_THROWS_EXCEPTION(AccessException, table->removeTrigger(trigger, true));
			table->removeTrigger(trigger, false);

			// Now create a trigger with a condition
			Trigger conditionalTrigger("myConditionalTestTrigger", timing, event,
									   "UPDATE \"" + table->getName() + "\" SET " + colA->getName() + "=1;",
									   "EXISTS( SELECT 1 from \"" + table->getName() + "\" WHERE " + colB->getName()
										   + " = 0.5 )");

			table->addTrigger(conditionalTrigger, true);
			table->removeTrigger(conditionalTrigger, true);
		}
	}

	MUMBLE_END_TEST_CASE
}

void DatabaseTest::dataTypes() {
	MUMBLE_BEGIN_TEST_CASE

	for (DataType::Type currentType : { DataType::Integer, DataType::SmallInteger, DataType::Float, DataType::Double,
										DataType::FixedSizeString, DataType::VarChar, DataType::Text,
										DataType::EpochTime, DataType::Blob, DataType::Binary, DataType::Timestamp }) {
		qInfo() << "Current data type:" << currentType;

		for (bool sized : { true, false }) {
			if ((!DataType::canBeSized(currentType) && sized) || (!DataType::canBeUnsized(currentType) && !sized)) {
				continue;
			}
			// Special case for binary type
			// The type claims that it can be sized, but this is only true for MySQL.
			// However, there we need the sizedness during data migration (tests) and
			// thus the type advertises itself as being able to be sized, when it
			// isn't really.
			if (currentType == DataType::Binary && sized) {
				continue;
			}

			qInfo() << "Sized:" << sized;

			DataType type(currentType, sized ? 42 : DataType::Unsized);

			// Ensure that a datatype object can be compared against a plain type and that this
			// disregards the size of the object
			QVERIFY(type == currentType);
			QVERIFY(currentType == type);

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

	MUMBLE_END_TEST_CASE
}

// This test ensures that our KeyValueTable helper class actually works as expected
void DatabaseTest::keyValueTable() {
	MUMBLE_BEGIN_TEST_CASE_NO_INIT

	Database::table_id id = db.addTable(std::make_unique< test::KeyValueTable >(db.getSQLHandle(), currentBackend));

	db.init(test::utils::getConnectionParamter(currentBackend));

	test::KeyValueTable *table = static_cast< test::KeyValueTable * >(db.getTable(id));
	QVERIFY(table != nullptr);

	table->insert("test", "value");
	QCOMPARE(table->query("test"), std::string("value"));

	MUMBLE_END_TEST_CASE
}

void DatabaseTest::unicode() {
	MUMBLE_BEGIN_TEST_CASE_NO_INIT

	Database::table_id id = db.addTable(std::make_unique< test::KeyValueTable >(db.getSQLHandle(), currentBackend));

	db.init(test::utils::getConnectionParamter(currentBackend));

	test::KeyValueTable *table = static_cast< test::KeyValueTable * >(db.getTable(id));
	QVERIFY(table != nullptr);

	for (const std::string symbol :
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

	MUMBLE_END_TEST_CASE
}

void DatabaseTest::fetchMinimumFreeID() {
	MUMBLE_BEGIN_TEST_CASE_NO_INIT

	Database::table_id id = db.addTable(std::make_unique< test::IntegerTable >(db.getSQLHandle(), currentBackend));

	db.init(test::utils::getConnectionParamter(currentBackend));

	test::IntegerTable *table = static_cast< test::IntegerTable * >(db.getTable(id));

	// Default-populate table
	table->init();

	int fetchedID =
		table->selectMinFreeID(utils::getLowestUnoccupiedIDStatement(currentBackend, test::IntegerTable::NAME,
																	 test::IntegerTable::column::int_col),
							   "second", nullptr);
	QCOMPARE(fetchedID, test::IntegerTable::lowestID_unconstrained);

	std::string secondColReq = "";
	fetchedID =
		table->selectMinFreeID(utils::getLowestUnoccupiedIDStatement(
								   currentBackend, test::IntegerTable::NAME, test::IntegerTable::column::int_col,
								   { utils::ColAlias(::test::IntegerTable::column::second_col, "second") }),
							   "second", &secondColReq);
	QCOMPARE(fetchedID, test::IntegerTable::lowestID_second_empty);

	secondColReq = "bla";
	fetchedID =
		table->selectMinFreeID(utils::getLowestUnoccupiedIDStatement(
								   currentBackend, test::IntegerTable::NAME, test::IntegerTable::column::int_col,
								   { utils::ColAlias(::test::IntegerTable::column::second_col, "second") }),
							   "second", &secondColReq);
	QCOMPARE(fetchedID, test::IntegerTable::lowestID_second_bla);

	MUMBLE_END_TEST_CASE
}

void DatabaseTest::dateToEpoch() {
	MUMBLE_BEGIN_TEST_CASE

	const std::vector< std::pair< std::string, unsigned int > > tests = {
		{ "'2016-08-11 17:15'", 1470935700 },
		{ "'2007-01-26'", 1169769600 },
		{ "'2007-01-26'", 1169769600 },
	};

	for (const std::pair< std::string, unsigned int > &current : tests) {
		qInfo() << "  " << QString::fromStdString(current.first);

		const std::string query = "SELECT " + utils::dateToEpoch(current.first, currentBackend, true);

		unsigned int timestamp = 0;
		db.getSQLHandle() << query, soci::into(timestamp);
		QCOMPARE(timestamp, current.second);

		soci::row row;
		db.getSQLHandle() << query, soci::into(row);
		QCOMPARE(row.get_indicator(0), soci::i_ok);

		if (currentBackend != Backend::SQLite) {
			unsigned int rowStamp = row.get< unsigned int >(0);
			QCOMPARE(rowStamp, current.second);
		} else {
			// Due to SQLite's lack of static typing, we can only get the stamp as a string value
			std::string rowStamp = row.get< std::string >(0);
			QCOMPARE(rowStamp, std::to_string(current.second));
		}
	}

	MUMBLE_END_TEST_CASE
}

void DatabaseTest::savepoints() {
	MUMBLE_BEGIN_TEST_CASE_NO_INIT

	Database::table_id id = db.addTable(std::make_unique< test::KeyValueTable >(db.getSQLHandle(), currentBackend));

	db.init(test::utils::getConnectionParamter(currentBackend));

	TransactionHolder transaction = db.ensureTransaction();

	test::KeyValueTable *table = static_cast< test::KeyValueTable * >(db.getTable(id));

	const std::string fallback = "dummy";
	const std::string key      = "my_val";
	QCOMPARE(table->query(key, fallback), fallback);

	{
		Savepoint save(db.getSQLHandle(), "this_is_a_test_savepoint");

		table->insert(key, "test");

		QCOMPARE(table->query(key, fallback), "test");

		save.rollback();
	}

	// We explicitly rolled the savepoint back, so the table should no longer exist
	QCOMPARE(table->query(key, fallback), fallback);

	{
		Savepoint save(db.getSQLHandle(), "this_is_a_test_savepoint");

		table->insert(key, "test");

		QCOMPARE(table->query(key, fallback), "test");
	}

	// Savepoint went out of scope without being released, so it should have rolled back
	QCOMPARE(table->query(key, fallback), fallback);

	{
		Savepoint save(db.getSQLHandle(), "this_is_a_test_savepoint");

		table->insert(key, "test");

		QCOMPARE(table->query(key, fallback), "test");

		save.release();
	}

	QCOMPARE(table->query(key, fallback), "test");


	MUMBLE_END_TEST_CASE
}

QTEST_MAIN(DatabaseTest)
#include "DatabaseTest.moc"
