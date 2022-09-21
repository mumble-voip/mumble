// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include <QString>
#include <QtTest>

#include "database/AccessException.h"
#include "database/Backend.h"
#include "database/NoDataException.h"

#include "database/ChannelPropertyTable.h"
#include "database/ChannelTable.h"
#include "database/ConfigTable.h"
#include "database/DBChannel.h"
#include "database/LogTable.h"
#include "database/ServerDatabase.h"
#include "database/ServerTable.h"

#include "MumbleConstants.h"

#include "TestUtils.h"

#include <nlohmann/json.hpp>

#include <cstring>
#include <iostream>
#include <unordered_map>
#include <vector>

namespace mdb  = ::mumble::db;
namespace msdb = ::mumble::server::db;

std::vector< mdb::Backend > backends = {
#ifdef MUMBLE_TEST_SQLITE
	mdb::Backend::SQLite,
#endif
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

class TestDB : public msdb::ServerDatabase {
public:
	using msdb::ServerDatabase::ServerDatabase;

	~TestDB() override {
		// Clear up everything that we have created in our test case
		try {
			this->destroyTables();
		} catch (const ::mdb::Exception &e) {
			std::cerr << "Exception encountered while destroying tables:" << std::endl;
			print_exception_message(e);
		}
	}
};

class ServerDatabaseTest : public QObject {
	Q_OBJECT;
private slots:
	void serverTable_server_management();
	void logTable_logMessage();
	void configTable_general();
	void channelTable_general();
	void channelPropertyTable_general();
};



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

	QVERIFY(!table.serverExists(0));
	table.addServer(0);
	QVERIFY(table.serverExists(0));
	// Some RMDBs (looking at you MySQL!) will by default treat an explicit value of zero as "please auto-generate the
	// next number in this auto_increment column" which usually happens to be 1.
	QVERIFY(!table.serverExists(1));
	table.addServer(1);
	QVERIFY(table.serverExists(1));
	QVERIFY(!table.serverExists(2));
	table.addServer(2);
	QVERIFY(table.serverExists(2));
	table.removeServer(2);
	QVERIFY(!table.serverExists(2));

	// Server IDs have to be unique, so we expect an error when attempting to add a duplicate ID
	QVERIFY_EXCEPTION_THROWN(table.addServer(1), ::mdb::AccessException);

	END_TEST_CASE
}

void ServerDatabaseTest::logTable_logMessage() {
	BEGIN_TEST_CASE

	unsigned int existingServerID    = 1;
	unsigned int nonExistingServerID = 5;

	db.getServerTable().addServer(existingServerID);

	QVERIFY(db.getServerTable().serverExists(existingServerID));
	QVERIFY(!db.getServerTable().serverExists(nonExistingServerID));

	QVERIFY_EXCEPTION_THROWN(db.getLogTable().logMessage(nonExistingServerID, "Dummy msg"), ::mdb::AccessException);

	db.getLogTable().logMessage(existingServerID, "I am a test message");

	db.getLogTable().logMessage(existingServerID, "I am a test message containing some unicode characters: ✅ 👀");

	END_TEST_CASE
}

void ServerDatabaseTest::configTable_general() {
	BEGIN_TEST_CASE

	unsigned int existingServerID    = 1;
	unsigned int nonExistingServerID = 5;

	db.getServerTable().addServer(existingServerID);

	QVERIFY(db.getServerTable().serverExists(existingServerID));
	QVERIFY(!db.getServerTable().serverExists(nonExistingServerID));

	msdb::ConfigTable &configTable = db.getConfigTable();

	std::string obtainedValue = configTable.getConfig(existingServerID, "IDontExist", "DefaultValue");
	std::string expectedValue = "DefaultValue";
	QCOMPARE(obtainedValue, expectedValue);

	QVERIFY(configTable.getAllConfigs(existingServerID).empty());

	configTable.setConfig(existingServerID, "miau", "test");

	obtainedValue = configTable.getConfig(existingServerID, "miau");
	expectedValue = std::string("test");
	QCOMPARE(obtainedValue, expectedValue);

	obtainedValue = configTable.getConfig(nonExistingServerID, "miau", "default");
	expectedValue = std::string("default");
	QCOMPARE(obtainedValue, expectedValue);

	configTable.setConfig(existingServerID, "miau", "different");
	obtainedValue = configTable.getConfig(existingServerID, "miau");
	expectedValue = std::string("different");
	QCOMPARE(obtainedValue, expectedValue);


	configTable.setConfig(existingServerID, "theKey", "theValue");

	obtainedValue = configTable.getConfig(existingServerID, "theKey");
	expectedValue = "theValue";
	QCOMPARE(obtainedValue, expectedValue);

	configTable.clearConfig(existingServerID, "theKey");
	obtainedValue = configTable.getConfig(existingServerID, "theKey", "theDefault");
	expectedValue = "theDefault";
	QCOMPARE(obtainedValue, expectedValue);


	std::unordered_map< std::string, std::string > allConfigs;
	allConfigs["miau"] = "different";

	QCOMPARE(configTable.getAllConfigs(existingServerID), allConfigs);

	configTable.clearAllConfigs(existingServerID);
	QVERIFY(configTable.getAllConfigs(existingServerID).empty());

	END_TEST_CASE
}

void ServerDatabaseTest::channelTable_general() {
	BEGIN_TEST_CASE

	unsigned int existingServerID    = 1;
	unsigned int nonExistingServerID = 5;

	db.getServerTable().addServer(existingServerID);

	QVERIFY(db.getServerTable().serverExists(existingServerID));
	QVERIFY(!db.getServerTable().serverExists(nonExistingServerID));

	::msdb::ChannelTable &channelTable = db.getChannelTable();

	QVERIFY(!channelTable.channelExists(existingServerID, 5));

	::msdb::DBChannel rootChannel;
	rootChannel.serverID   = existingServerID;
	rootChannel.channelID  = Mumble::ROOT_CHANNEL_ID;
	rootChannel.parentID   = rootChannel.channelID;
	rootChannel.name       = "Root";
	rootChannel.inheritACL = false;

	QVERIFY(!channelTable.channelExists(rootChannel));

	channelTable.addChannel(rootChannel);

	QVERIFY(channelTable.channelExists(rootChannel));

	::msdb::DBChannel fetchedData = channelTable.getChannelData(existingServerID, Mumble::ROOT_CHANNEL_ID);
	QCOMPARE(fetchedData, rootChannel);

	// Referencing a non-existing server ID should throw an exception (as should using an invalid channel ID)
	QVERIFY_EXCEPTION_THROWN(channelTable.getChannelData(nonExistingServerID, Mumble::ROOT_CHANNEL_ID),
							 ::mdb::NoDataException);
	QVERIFY_EXCEPTION_THROWN(channelTable.getChannelData(existingServerID, 5), ::mdb::NoDataException);

	::msdb::DBChannel other;
	other.serverID   = nonExistingServerID;
	other.channelID  = channelTable.getFreeChannelID(existingServerID);
	other.parentID   = rootChannel.channelID;
	other.name       = "Other channel with ❤";
	other.inheritACL = true;

	// other references a non-existing server, which should result in an exception being thrown
	QVERIFY_EXCEPTION_THROWN(channelTable.addChannel(other), ::mdb::AccessException);

	other.serverID = existingServerID;

	QVERIFY(rootChannel != other);

	channelTable.addChannel(other);

	::msdb::DBChannel updatedOther = other;
	updatedOther.name              = "New name❗";

	QVERIFY(other != updatedOther);

	channelTable.updateChannel(updatedOther);

	fetchedData = channelTable.getChannelData(updatedOther.serverID, updatedOther.channelID);
	QCOMPARE(fetchedData, updatedOther);


	END_TEST_CASE
}

void ServerDatabaseTest::channelPropertyTable_general() {
	BEGIN_TEST_CASE

	unsigned int existingServerID     = 0;
	unsigned int nonExistingServerID  = 5;
	unsigned int existingChannelID    = 0;
	unsigned int nonExistingChannelID = 5;
	::msdb::DBChannel channel;
	channel.serverID   = existingServerID;
	channel.channelID  = existingChannelID;
	channel.parentID   = channel.channelID;
	channel.name       = "Test channel";
	channel.inheritACL = true;

	db.getServerTable().addServer(existingServerID);
	db.getChannelTable().addChannel(channel);

	QVERIFY(db.getServerTable().serverExists(existingServerID));
	QVERIFY(!db.getServerTable().serverExists(nonExistingServerID));
	QVERIFY(db.getChannelTable().channelExists(channel));
	QVERIFY(!db.getChannelTable().channelExists(existingServerID, nonExistingChannelID));

	::msdb::ChannelPropertyTable &table = db.getChannelPropertyTable();

	QVERIFY(!table.isPropertySet(existingChannelID, existingChannelID, ::msdb::ChannelProperty::Description));

	table.setProperty(existingServerID, existingChannelID, ::msdb::ChannelProperty::Description, "Random description");
	table.setProperty(existingServerID, existingChannelID, ::msdb::ChannelProperty::MaxUsers, std::to_string(5));

	QCOMPARE(
		table.getProperty< std::string >(existingServerID, existingChannelID, ::msdb::ChannelProperty::Description),
		std::string("Random description"));
	QCOMPARE(table.getProperty< int >(existingServerID, existingChannelID, ::msdb::ChannelProperty::MaxUsers), 5);
	QCOMPARE(table.getProperty< unsigned int >(existingServerID, existingChannelID, ::msdb::ChannelProperty::MaxUsers),
			 static_cast< unsigned int >(5));
	// By default, querying non-existing values or using a wrong type will result in an exception
	QVERIFY_EXCEPTION_THROWN(
		table.getProperty< int >(nonExistingServerID, existingChannelID, ::msdb::ChannelProperty::Description),
		::mdb::AccessException);
	QVERIFY_EXCEPTION_THROWN(
		table.getProperty< int >(existingServerID, nonExistingChannelID, ::msdb::ChannelProperty::Description),
		::mdb::AccessException);
	QVERIFY_EXCEPTION_THROWN(
		table.getProperty< int >(existingServerID, existingChannelID, ::msdb::ChannelProperty::Position),
		::mdb::AccessException);
	// However, we can instead request for a default value to be returned instead
	int fetchedValue =
		table.getProperty< int, false >(existingServerID, nonExistingChannelID, ::msdb::ChannelProperty::Position, 42);
	QCOMPARE(fetchedValue, 42);
	fetchedValue =
		table.getProperty< int, false >(existingServerID, existingChannelID, ::msdb::ChannelProperty::Description, 42);
	QCOMPARE(fetchedValue, 42);

	QVERIFY(table.isPropertySet(existingServerID, existingChannelID, ::msdb::ChannelProperty::Description));
	QVERIFY(table.isPropertySet(existingServerID, existingChannelID, ::msdb::ChannelProperty::MaxUsers));

	table.clearProperty(existingServerID, existingChannelID, ::msdb::ChannelProperty::Description);

	QVERIFY(!table.isPropertySet(existingServerID, existingChannelID, ::msdb::ChannelProperty::Description));
	QVERIFY(table.isPropertySet(existingServerID, existingChannelID, ::msdb::ChannelProperty::MaxUsers));

	table.setProperty(existingServerID, existingChannelID, ::msdb::ChannelProperty::Position, std::to_string(12));

	QVERIFY(table.isPropertySet(existingServerID, existingChannelID, ::msdb::ChannelProperty::MaxUsers));
	QVERIFY(table.isPropertySet(existingServerID, existingChannelID, ::msdb::ChannelProperty::Position));

	table.clearAllProperties(nonExistingServerID, nonExistingChannelID);

	QVERIFY(table.isPropertySet(existingServerID, existingChannelID, ::msdb::ChannelProperty::MaxUsers));
	QVERIFY(table.isPropertySet(existingServerID, existingChannelID, ::msdb::ChannelProperty::Position));

	table.clearAllProperties(existingServerID, existingChannelID);

	QVERIFY(!table.isPropertySet(existingServerID, existingChannelID, ::msdb::ChannelProperty::MaxUsers));
	QVERIFY(!table.isPropertySet(existingServerID, existingChannelID, ::msdb::ChannelProperty::Position));

	END_TEST_CASE
}


QTEST_MAIN(ServerDatabaseTest)
#include "ServerDatabaseTest.moc"
