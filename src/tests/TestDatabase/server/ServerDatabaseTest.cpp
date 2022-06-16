// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include <QString>
#include <QtTest>

#include "database/Backend.h"

#include "database/ServerDatabase.h"
#include "database/ServerTable.h"

#include "TestUtils.h"

#include <nlohmann/json.hpp>

#include <cstring>
#include <iostream>
#include <vector>

namespace mdb  = ::mumble::db;
namespace msdb = ::mumble::server::db;

std::vector< mdb::Backend > backends = {
	mdb::Backend::SQLite,
#ifdef MUMBLE_TEST_MYSQL
	mdb::Backend::MySQL,
#endif
#ifdef MUMBLE_TEST_POSTGRESQL
	mdb::Backend::PostgreSQL,
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


class TestDB : public msdb::ServerDatabase {
public:
	using msdb::ServerDatabase::ServerDatabase;

	~TestDB() override {
		// Clear up everything that we have created in our test case
		this->destroyTables();
	}
};

class ServerDatabaseTest : public QObject {
	Q_OBJECT;
private slots:
	void serverTable_server_management();
};

/**
 * Helper function to print exception messages that can also fully unfold nested exceptions
 */
void print_exception_message(const std::exception &e) {
	std::cerr << "  " << e.what() << "\n";
	try {
		std::rethrow_if_nested(e);
	} catch (const std::exception &nested) {
		print_exception_message(nested);
	}
}



#define BEGIN_TEST_CASE                                                                                      \
	try {                                                                                                    \
		for (::mdb::Backend currentBackend : backends) {                                                     \
			qInfo() << "Current backend:" << QString::fromStdString(::mdb::backendToString(currentBackend)); \
			TestDB db(currentBackend);                                                                       \
			db.init(::mumble::db::test::utils::getConnectionParamter(currentBackend));

#define END_TEST_CASE                                  \
	}                                                  \
	}                                                  \
	catch (const std::exception &e) {                  \
		std::cerr << "Caught unexpected exception:\n"; \
		print_exception_message(e);                    \
		QFAIL("Aborting due to thrown exception");     \
	}



void ServerDatabaseTest::serverTable_server_management() {
	BEGIN_TEST_CASE

	::msdb::ServerTable &table = db.getServerTable();

	QVERIFY(table.serverExists(1));
	QVERIFY(!table.serverExists(2));
	table.addServer(2);
	QVERIFY(table.serverExists(2));
	table.removeServer(2);
	QVERIFY(!table.serverExists(2));

	END_TEST_CASE
}


QTEST_MAIN(ServerDatabaseTest)
#include "ServerDatabaseTest.moc"
