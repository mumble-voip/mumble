// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include <QString>
#include <QtTest>

#include "database/AccessException.h"
#include "database/Backend.h"
#include "database/FormatException.h"
#include "database/MetaTable.h"
#include "database/NoDataException.h"

#include "database/ACLTable.h"
#include "database/BanTable.h"
#include "database/ChannelLinkTable.h"
#include "database/ChannelListenerTable.h"
#include "database/ChannelPropertyTable.h"
#include "database/ChannelTable.h"
#include "database/ConfigTable.h"
#include "database/DBAcl.h"
#include "database/DBBan.h"
#include "database/DBChannel.h"
#include "database/DBChannelLink.h"
#include "database/DBChannelListener.h"
#include "database/DBGroup.h"
#include "database/DBGroupMember.h"
#include "database/DBLogEntry.h"
#include "database/GroupMemberTable.h"
#include "database/GroupTable.h"
#include "database/LogTable.h"
#include "database/ServerDatabase.h"
#include "database/ServerTable.h"
#include "database/UserProperty.h"
#include "database/UserPropertyTable.h"
#include "database/UserTable.h"

#include "MumbleConstants.h"

#include "JSONAssembler.h"
#include "TestUtils.h"

#include <nlohmann/json.hpp>

#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <unordered_map>
#include <vector>


#if QT_VERSION < QT_VERSION_CHECK(6, 3, 0)
#	define QVERIFY_THROWS_EXCEPTION(kind, expression) QVERIFY_EXCEPTION_THROWN(expression, kind)
#endif

namespace mdb  = ::mumble::db;
namespace msdb = ::mumble::server::db;

namespace QTest {
// Provide an overload of toString for JSON objects so that Qt's macros can properly display
// these objects to report failing tests
template<> char *toString(const nlohmann::json &json) {
	std::string str = json.dump(2);
	char *buffer    = new char[str.size() + 1];
	std::strcpy(buffer, str.data());

	return buffer;
}

template<> char *toString(const nlohmann::json::array_t &array) {
	return toString< nlohmann::json >(array);
}

template<> char *toString(const std::string &str) {
	char *buffer = new char[str.size() + 1];
	std::strcpy(buffer, str.data());

	return buffer;
}

template<> char *toString(const ::msdb::DBUserData &data) {
	std::stringstream sstream;
	sstream << "{ name: \"" << data.name << "\", last_active: "
			<< std::chrono::duration_cast< std::chrono::seconds >(data.lastActive.time_since_epoch()).count()
			<< ", last_disconnect: "
			<< std::chrono::duration_cast< std::chrono::seconds >(data.lastDisconnect.time_since_epoch()).count()
			<< ", last_channel_id: " << data.lastChannelID << ", texture: " << data.texture.size()
			<< " bytes, password: { \"" << data.password.passwordHash << "\", \"" << data.password.salt << "\", "
			<< data.password.kdfIterations << " } }";

	std::string str = sstream.str();

	char *buffer = new char[str.size() + 1];
	std::strcpy(buffer, str.data());

	return buffer;
}

} // namespace QTest

namespace std {
template< std::size_t size > char *toString(const std::array< std::uint8_t, size > array) {
	std::stringstream sstream;
	sstream << "{ ";
	for (std::size_t i = 0; i < size; ++i) {
		sstream << "0x" << std::setfill('0') << std::setw(2) << std::hex << static_cast< int >(array[i]);

		if (i + 1 < size) {
			sstream << ", ";
		}
	}
	sstream << " }";

	std::string str = sstream.str();

	char *buffer = new char[str.size() + 1];
	std::strcpy(buffer, str.data());

	return buffer;
}

// The dummy template parameter serves as a SFINAE filter to rule out all T that don't have operator<<
template< typename T, typename = decltype(std::declval< std::stringstream >() << std::declval< T >()) >
char *toString(const std::vector< T > vector) {
	std::stringstream sstream;
	sstream << "{ ";
	for (std::size_t i = 0; i < vector.size(); ++i) {
		sstream << vector[i];

		if (i + 1 < vector.size()) {
			sstream << ", ";
		}
	}
	sstream << " }";

	std::string str = sstream.str();

	char *buffer = new char[str.size() + 1];
	std::strcpy(buffer, str.data());

	return buffer;
}


} // namespace std


/**
 * Helper function to convert a std::chrono::timepoint to seconds since epoch
 */
template< typename TimePoint > std::size_t toSeconds(const TimePoint &tp) {
	return static_cast< std::size_t >(
		std::chrono::duration_cast< std::chrono::seconds >(tp.time_since_epoch()).count());
}


class TestDB : public msdb::ServerDatabase {
public:
	using msdb::ServerDatabase::ServerDatabase;

	void configureStandardTablesWithoutCreation() {
		addTable(std::make_unique< mdb::MetaTable >(m_sql, m_backend));

		::msdb::ServerDatabase::setupStandardTables();
	}

	void resetTables() { m_tables.clear(); }

	~TestDB() override {
		// Clear up everything that we have created in our test case
		try {
			this->destroyTables();
		} catch (const ::mdb::Exception &e) {
			std::cerr << "Exception encountered while destroying tables:" << std::endl;
			mumble::db::test::utils::print_exception_message(e);
		}
	}

	soci::session &getSQLHandle() { return m_sql; }
};

// Note: PlainDB and TestDB are to be seen as two different ways to access the same underlying table.
// Therefore, we don't try to blindly delete the tables in TestDB in its destructor as this would interfere
// with the setup (e.g. Triggers) of TestDB that PlainDB knows nothing about.
class PlainDB : public mdb::Database {
public:
	PlainDB(::mdb::Backend backend, unsigned int schemeVersion)
		: ::mdb::Database(backend), schemeVersion(schemeVersion) {}

	unsigned int getSchemeVersion() const override { return schemeVersion; }

	~PlainDB() override {
		// Clear up everything that we have created in our test case
		while (!m_tables.empty()) {
			try {
				this->destroyTables();

				// If no errors are encountered during destroyTables, we can exit the loop
				break;
			} catch (const ::mdb::Exception &) {
				// destroyTables iterates through the tables in order. If destroying one of them
				// raises an exception, we assume that this is due to the respective table being
				// managed by TestDB and therefore, we simply make PlainDB forget about it.
				m_tables.erase(m_tables.begin());
			}
		}
	}

	soci::session &getSQLHandle() { return m_sql; }

private:
	unsigned int schemeVersion;
};

class ServerDatabaseTest : public QObject {
	Q_OBJECT
private slots:
	void serverTable_server_management();
	void logTable_general();
	void configTable_general();
	void channelTable_general();
	void channelPropertyTable_general();
	void userTable_general();
	void userPropertyTable_general();
	void groupTable_general();
	void groupMemberTable_general();
	void aclTable_general();
	void channelLinkTable_general();
	void ipAddress_conversions();
	void banTable_general();
	void channelListenerTable_general();

	void database_scheme_migration();
};



void ServerDatabaseTest::serverTable_server_management() {
	MUMBLE_BEGIN_TEST_CASE

	::msdb::ServerTable &table = db.getServerTable();

	QVERIFY(table.getAllServerIDs().empty());

	QVERIFY(!table.serverExists(0));
	QCOMPARE(table.getFreeServerID(), static_cast< unsigned int >(0));
	table.addServer(0);
	QVERIFY(table.serverExists(0));
	// Some RMDBs (looking at you MySQL!) will by default treat an explicit value of zero as "please auto-generate the
	// next number in this auto_increment column" which usually happens to be 1.
	QVERIFY(!table.serverExists(1));
	QCOMPARE(table.getAllServerIDs(), std::vector< unsigned int >{ 0 });
	QCOMPARE(table.getFreeServerID(), static_cast< unsigned int >(1));
	table.addServer(1);
	QVERIFY(table.serverExists(1));
	QVERIFY(!table.serverExists(2));
	table.addServer(2);
	QVERIFY(table.serverExists(2));
	table.removeServer(2);
	QVERIFY(!table.serverExists(2));
	table.addServer(3);
	QCOMPARE(table.getFreeServerID(), static_cast< unsigned int >(2));

	std::vector< unsigned int > expectedIDs = { 0, 1, 3 };
	std::vector< unsigned int > fetchedIDs  = table.getAllServerIDs();
	QCOMPARE(fetchedIDs.size(), expectedIDs.size());
	QVERIFY(std::is_permutation(fetchedIDs.begin(), fetchedIDs.end(), expectedIDs.begin()));

	// Server IDs have to be unique, so we expect an error when attempting to add a duplicate ID
	QVERIFY_THROWS_EXCEPTION(::mdb::AccessException, table.addServer(1));

	MUMBLE_END_TEST_CASE
}

void ServerDatabaseTest::logTable_general() {
	MUMBLE_BEGIN_TEST_CASE

	unsigned int existingServerID    = 1;
	unsigned int nonExistingServerID = 5;

	db.getServerTable().addServer(existingServerID);

	QVERIFY(db.getServerTable().serverExists(existingServerID));
	QVERIFY(!db.getServerTable().serverExists(nonExistingServerID));

	QVERIFY_THROWS_EXCEPTION(::mdb::AccessException,
							 db.getLogTable().logMessage(nonExistingServerID, ::msdb::DBLogEntry("Dummy msg")));

	QCOMPARE(db.getLogTable().getLogs(existingServerID).size(), static_cast< std::size_t >(0));
	QCOMPARE(db.getLogTable().getLogSize(existingServerID), static_cast< std::size_t >(0));

	db.getLogTable().logMessage(existingServerID, ::msdb::DBLogEntry("I am a test message"));

	QCOMPARE(db.getLogTable().getLogs(existingServerID).size(), static_cast< std::size_t >(1));
	QCOMPARE(db.getLogTable().getLogSize(existingServerID), static_cast< std::size_t >(1));
	QCOMPARE(db.getLogTable().getLogSize(nonExistingServerID), static_cast< std::size_t >(0));

	db.getLogTable().logMessage(existingServerID,
								::msdb::DBLogEntry("I am a test message containing some unicode characters: ✅ 👀"));

	QCOMPARE(db.getLogTable().getLogs(existingServerID).size(), static_cast< std::size_t >(2));

	db.getLogTable().clearLog(nonExistingServerID);

	QCOMPARE(db.getLogTable().getLogs(existingServerID).size(), static_cast< std::size_t >(2));
	QCOMPARE(db.getLogTable().getLogs(nonExistingServerID).size(), static_cast< std::size_t >(0));

	db.getLogTable().clearLog(existingServerID);

	QCOMPARE(db.getLogTable().getLogs(existingServerID).size(), static_cast< std::size_t >(0));

	// Populate table with several default entries
	std::vector<::msdb::DBLogEntry > entries;
	for (std::size_t i = 0; i < 10; ++i) {
		entries.emplace_back(std::string("Message ") + std::to_string(i),
							 std::chrono::system_clock::time_point(std::chrono::seconds(i)));

		db.getLogTable().logMessage(existingServerID, entries[i]);
	}
	// Sort values such that most recent entry comes first
	std::reverse(entries.begin(), entries.end());

	QCOMPARE(db.getLogTable().getLogSize(existingServerID), static_cast< std::size_t >(10));

	for (std::size_t maxEntries = 0; maxEntries < entries.size() + 2; ++maxEntries) {
		for (std::size_t offset = 0; offset < entries.size() + 2; ++offset) {
			std::vector<::msdb::DBLogEntry > fetchedEntries = db.getLogTable().getLogs(
				existingServerID, static_cast< unsigned int >(maxEntries), static_cast< unsigned int >(offset));

			const int totalEntryCount = static_cast< int >(entries.size());
			int maxPossibleEntries    = totalEntryCount - std::min(static_cast< int >(offset), totalEntryCount);
			int requestableEntryCount = std::min(static_cast< int >(maxEntries), totalEntryCount);
			int expectedEntryCount    = requestableEntryCount;
			expectedEntryCount += std::min(maxPossibleEntries - requestableEntryCount, 0);
			expectedEntryCount = std::max(expectedEntryCount, 0);

			QCOMPARE(static_cast< int >(fetchedEntries.size()), expectedEntryCount);

			// This check is more to ensure that we don't run into UB below
			QVERIFY(fetchedEntries.empty() || fetchedEntries.size() + offset <= entries.size());

			for (std::size_t i = 0; i < fetchedEntries.size(); ++i) {
				QCOMPARE(fetchedEntries[i].message, entries[i + offset].message);
				QCOMPARE(fetchedEntries[i].timestamp, entries[i + offset].timestamp);
			}
		}
	}

	MUMBLE_END_TEST_CASE
}

void ServerDatabaseTest::configTable_general() {
	MUMBLE_BEGIN_TEST_CASE

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

	MUMBLE_END_TEST_CASE
}

void ServerDatabaseTest::channelTable_general() {
	MUMBLE_BEGIN_TEST_CASE

	unsigned int existingServerID    = 1;
	unsigned int otherServerID       = 2;
	unsigned int nonExistingServerID = 5;

	db.getServerTable().addServer(existingServerID);
	db.getServerTable().addServer(otherServerID);

	QVERIFY(db.getServerTable().serverExists(existingServerID));
	QVERIFY(db.getServerTable().serverExists(otherServerID));
	QVERIFY(!db.getServerTable().serverExists(nonExistingServerID));

	::msdb::ChannelTable &channelTable = db.getChannelTable();

	QVERIFY(channelTable.getChildrenOf(existingServerID, Mumble::ROOT_CHANNEL_ID).empty());
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
	QVERIFY(channelTable.getChildrenOf(existingServerID, Mumble::ROOT_CHANNEL_ID).empty());

	::msdb::DBChannel fetchedData = channelTable.getChannelData(existingServerID, Mumble::ROOT_CHANNEL_ID);
	QCOMPARE(fetchedData, rootChannel);

	// Referencing a non-existing server ID should throw an exception (as should using an invalid channel ID)
	QVERIFY_THROWS_EXCEPTION(::mdb::NoDataException,
							 channelTable.getChannelData(nonExistingServerID, Mumble::ROOT_CHANNEL_ID));
	QVERIFY_THROWS_EXCEPTION(::mdb::NoDataException, channelTable.getChannelData(existingServerID, 5));

	QCOMPARE(channelTable.getFreeChannelID(existingServerID), static_cast< unsigned int >(1));

	::msdb::DBChannel other;
	other.serverID   = nonExistingServerID;
	other.channelID  = channelTable.getFreeChannelID(existingServerID);
	other.parentID   = rootChannel.channelID;
	other.name       = "Other channel with ❤";
	other.inheritACL = true;

	// other references a non-existing server, which should result in an exception being thrown
	QVERIFY_THROWS_EXCEPTION(::mdb::AccessException, channelTable.addChannel(other));

	other.serverID = existingServerID;

	QVERIFY(rootChannel != other);

	channelTable.addChannel(other);

	QCOMPARE(channelTable.getChildrenOf(existingServerID, rootChannel.channelID).size(), static_cast< std::size_t >(1));

	::msdb::DBChannel updatedOther = other;
	updatedOther.name              = "New name❗";

	QVERIFY(other != updatedOther);

	channelTable.updateChannel(updatedOther);

	fetchedData = channelTable.getChannelData(updatedOther.serverID, updatedOther.channelID);
	QCOMPARE(fetchedData, updatedOther);

	QCOMPARE(channelTable.getFreeChannelID(existingServerID), static_cast< unsigned int >(2));

	::msdb::DBChannel third;
	third.serverID  = existingServerID;
	third.channelID = channelTable.getFreeChannelID(existingServerID);
	third.parentID  = rootChannel.channelID;
	third.name      = "Third channel";

	channelTable.addChannel(third);

	QCOMPARE(channelTable.getChildrenOf(existingServerID, rootChannel.channelID).size(), static_cast< std::size_t >(2));

	channelTable.removeChannel(other);

	// We expect to first re-use the ID 1 before moving on to ID 3
	QCOMPARE(channelTable.getFreeChannelID(existingServerID), static_cast< unsigned int >(1));

	// On this other server, we expect to start at 0 (there are no channels yet)
	QCOMPARE(channelTable.getFreeChannelID(otherServerID), static_cast< unsigned int >(0));

	MUMBLE_END_TEST_CASE
}

void ServerDatabaseTest::channelPropertyTable_general() {
	MUMBLE_BEGIN_TEST_CASE

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

	QVERIFY(!table.isPropertySet(existingServerID, existingChannelID, ::msdb::ChannelProperty::Description));

	table.setProperty(existingServerID, existingChannelID, ::msdb::ChannelProperty::Description, "Random description");
	table.setProperty(existingServerID, existingChannelID, ::msdb::ChannelProperty::MaxUsers, std::to_string(42));
	// Immediately update the property
	table.setProperty(existingServerID, existingChannelID, ::msdb::ChannelProperty::MaxUsers, std::to_string(5));

	QCOMPARE(
		table.getProperty< std::string >(existingServerID, existingChannelID, ::msdb::ChannelProperty::Description),
		std::string("Random description"));
	QCOMPARE(table.getProperty< int >(existingServerID, existingChannelID, ::msdb::ChannelProperty::MaxUsers), 5);
	QCOMPARE(table.getProperty< unsigned int >(existingServerID, existingChannelID, ::msdb::ChannelProperty::MaxUsers),
			 static_cast< unsigned int >(5));
	// By default, querying non-existing values or using a wrong type will result in an exception
	QVERIFY_THROWS_EXCEPTION(::mdb::AccessException, table.getProperty< int >(nonExistingServerID, existingChannelID,
																			  ::msdb::ChannelProperty::Description));
	QVERIFY_THROWS_EXCEPTION(::mdb::AccessException, table.getProperty< int >(existingServerID, nonExistingChannelID,
																			  ::msdb::ChannelProperty::Description));
	QVERIFY_THROWS_EXCEPTION(::mdb::AccessException, table.getProperty< int >(existingServerID, existingChannelID,
																			  ::msdb::ChannelProperty::Position));
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

	// Get a default string value
	std::string fetchedStr = table.getProperty< std::string, false >(
		existingServerID, existingChannelID, ::msdb::ChannelProperty::Description, "My default");
	QCOMPARE(fetchedStr, std::string("My default"));

	MUMBLE_END_TEST_CASE
}

void ServerDatabaseTest::userTable_general() {
	MUMBLE_BEGIN_TEST_CASE

	const unsigned int existingServerID     = 0;
	const unsigned int nonExistingServerID  = 5;
	const unsigned int existingChannelID    = 1;
	const unsigned int nonExistingChannelID = 5;
	::msdb::DBChannel rootChannel;
	rootChannel.serverID  = existingServerID;
	rootChannel.channelID = Mumble::ROOT_CHANNEL_ID;
	rootChannel.parentID  = rootChannel.channelID;
	rootChannel.name      = "Root";

	::msdb::DBChannel channel;
	channel.serverID  = existingServerID;
	channel.channelID = existingChannelID;
	channel.parentID  = rootChannel.channelID;
	channel.name      = "Test channel";

	db.getServerTable().addServer(existingServerID);
	db.getChannelTable().addChannel(rootChannel);
	db.getChannelTable().addChannel(channel);

	QVERIFY(db.getServerTable().serverExists(existingServerID));
	QVERIFY(!db.getServerTable().serverExists(nonExistingServerID));
	QVERIFY(db.getChannelTable().channelExists(rootChannel));
	QVERIFY(db.getChannelTable().channelExists(channel));
	QVERIFY(!db.getChannelTable().channelExists(existingServerID, nonExistingChannelID));

	::msdb::UserTable &table = db.getUserTable();

	::msdb::DBUser testUser;
	testUser.serverID         = existingServerID;
	testUser.registeredUserID = 3;

	::msdb::DBUserData testUserData;
	testUserData.name = "Test user";

	QVERIFY(!table.userExists(testUser));
	QCOMPARE(table.getFreeUserID(existingServerID), static_cast< unsigned int >(0));

	table.addUser(testUser, testUserData.name);

	QVERIFY(table.userExists(testUser));
	// As the testUser uses an ID > 0, getFreeUserID should still return 0
	QCOMPARE(table.getFreeUserID(existingServerID), static_cast< unsigned int >(0));

	// Adding a user with an invalid (empty) name should throw
	QVERIFY_THROWS_EXCEPTION(::mdb::FormatException, table.addUser(::msdb::DBUser(existingServerID, 13), {}));
	// Adding a user to a non-existing server should throw
	QVERIFY_THROWS_EXCEPTION(::mdb::AccessException, table.addUser(::msdb::DBUser(nonExistingServerID, 13), "bob"));
	// Adding a user with an already in-use ID should throw
	QVERIFY_THROWS_EXCEPTION(::mdb::AccessException,
							 table.addUser(::msdb::DBUser(existingServerID, testUser.registeredUserID), "bob"));
	// Adding a user with an already in-use name should throw
	QVERIFY_THROWS_EXCEPTION(::mdb::AccessException,
							 table.addUser(::msdb::DBUser(existingServerID, 14), testUserData.name));

#define CHECK_USER_DATA(user, data)                                                     \
	QCOMPARE(table.getData(user), data);                                                \
	QCOMPARE(toSeconds(table.getLastDisconnect(user)), toSeconds(data.lastDisconnect)); \
	QCOMPARE(toSeconds(table.getLastActive(user)), toSeconds(data.lastActive));         \
	QCOMPARE(table.getLastChannelID(user), data.lastChannelID);                         \
	QCOMPARE(table.getTexture(user), data.texture);                                     \
	QCOMPARE(table.getPassword(user), data.password);                                   \
	QCOMPARE(table.getName(user), data.name);

	CHECK_USER_DATA(testUser, testUserData);

	testUserData.password.passwordHash = "Imagine this was a password hash";
	const std::uint8_t textureBuffer[] = "< A beautiful avatar >";
	testUserData.texture = std::vector< std::uint8_t >(textureBuffer, textureBuffer + sizeof(textureBuffer));

	table.updateData(testUser, testUserData);
	CHECK_USER_DATA(testUser, testUserData);

	testUserData.name                     = "Alice";
	const std::uint8_t newTextureBuffer[] = "Alice's very special texture 💓";
	testUserData.texture = std::vector< std::uint8_t >(newTextureBuffer, newTextureBuffer + sizeof(newTextureBuffer));
	testUserData.password.passwordHash  = "lksdhfsekldjf";
	testUserData.password.salt          = "salt'n'pepper";
	testUserData.password.kdfIterations = 424242;
	testUserData.lastChannelID          = channel.channelID;
	testUserData.lastActive             = std::chrono::system_clock::now();
	testUserData.lastDisconnect         = std::chrono::system_clock::now();

	table.updateData(testUser, testUserData);
	CHECK_USER_DATA(testUser, testUserData);
#undef CHECK_USER_DATA

	// Test clear* functions
	table.clearLastDisconnect(testUser);
	QCOMPARE(table.getLastDisconnect(testUser), std::chrono::time_point< std::chrono::system_clock >());

	table.clearLastActive(testUser);
	QCOMPARE(table.getLastActive(testUser), std::chrono::time_point< std::chrono::system_clock >());

	// "Clearing" the last channel really means resetting it to the root channel ID
	table.clearLastChannelID(testUser);
	QCOMPARE(table.getLastChannelID(testUser), static_cast< unsigned int >(Mumble::ROOT_CHANNEL_ID));

	table.clearTexture(testUser);
	QCOMPARE(table.getTexture(testUser), std::vector< std::uint8_t >());

	table.clearPassword(testUser);
	QCOMPARE(table.getPassword(testUser), ::msdb::DBUserData::PasswordData());


	// Test set* functions
	std::chrono::time_point< std::chrono::system_clock > now = std::chrono::system_clock::now();
	table.setLastDisconnect(testUser, now);
	QCOMPARE(toSeconds(table.getLastDisconnect(testUser)), toSeconds(now));

	table.setLastActive(testUser, now);
	QCOMPARE(toSeconds(table.getLastActive(testUser)), toSeconds(now));

	table.setLastChannelID(testUser, existingChannelID);
	QCOMPARE(table.getLastChannelID(testUser), existingChannelID);
	QVERIFY_THROWS_EXCEPTION(::mdb::AccessException, table.setLastChannelID(testUser, nonExistingChannelID));

	table.setTexture(testUser, testUserData.texture);
	QCOMPARE(table.getTexture(testUser), testUserData.texture);

	table.setPassword(testUser, testUserData.password);
	QCOMPARE(table.getPassword(testUser), testUserData.password);

	testUserData.name = "Pia";
	table.setName(testUser, testUserData.name);
	QCOMPARE(table.getName(testUser), testUserData.name);
	QVERIFY_THROWS_EXCEPTION(::mdb::FormatException, table.setName(testUser, ""));


	::msdb::DBUser nonExistingUser(existingServerID, 42);
	QVERIFY(!table.userExists(nonExistingUser));

	// All get* functions should throw, if given a non-existing user
	QVERIFY_THROWS_EXCEPTION(::mdb::AccessException, table.getData(nonExistingUser));
	QVERIFY_THROWS_EXCEPTION(::mdb::AccessException, table.getLastDisconnect(nonExistingUser));
	QVERIFY_THROWS_EXCEPTION(::mdb::AccessException, table.getLastActive(nonExistingUser));
	QVERIFY_THROWS_EXCEPTION(::mdb::AccessException, table.getLastChannelID(nonExistingUser));
	QVERIFY_THROWS_EXCEPTION(::mdb::AccessException, table.getTexture(nonExistingUser));
	QVERIFY_THROWS_EXCEPTION(::mdb::AccessException, table.getPassword(nonExistingUser));
	QVERIFY_THROWS_EXCEPTION(::mdb::AccessException, table.getName(nonExistingUser));


	// Test findUser
	boost::optional< unsigned int > userID = table.findUser(existingServerID, testUserData.name);
	QVERIFY(userID);
	QCOMPARE(userID.get(), testUser.registeredUserID);

	userID = table.findUser(existingServerID, "I don't exist");
	QVERIFY(!userID);

	userID = table.findUser(nonExistingServerID, testUserData.name);
	QVERIFY(!userID);


	// Test getRegisteredUsers
	::msdb::DBUser additionalUser(existingServerID, 0);
	table.addUser(additionalUser, "Dummy name");
	std::vector<::msdb::DBUser > expectedUsers = { testUser, additionalUser };
	std::vector<::msdb::DBUser > actualUsers   = table.getRegisteredUsers(existingServerID);
	QVERIFY(expectedUsers.size() == actualUsers.size()
			&& std::is_permutation(expectedUsers.begin(), expectedUsers.end(), actualUsers.begin()));

	actualUsers = table.getRegisteredUsers(nonExistingServerID);
	QVERIFY(actualUsers.empty());

	QCOMPARE(table.getFreeUserID(existingServerID), static_cast< unsigned int >(1));


	// Test that a user's last channel is reset to the root channel, if the referenced channel gets deleted
	QVERIFY(channel.channelID != Mumble::ROOT_CHANNEL_ID);
	table.setLastChannelID(testUser, channel.channelID);
	QCOMPARE(table.getLastChannelID(testUser), channel.channelID);
	db.getChannelTable().removeChannel(channel);
	QCOMPARE(table.getLastChannelID(testUser), static_cast< unsigned int >(Mumble::ROOT_CHANNEL_ID));


	// Test that changing a user's last channel automatically updates the last_active stats as well
	std::chrono::time_point< std::chrono::system_clock > lastActive(std::chrono::seconds(12));
	QVERIFY(toSeconds(lastActive) < toSeconds(std::chrono::system_clock::now()));
	table.setLastActive(testUser, lastActive);
	QCOMPARE(toSeconds(table.getLastActive(testUser)), toSeconds(lastActive));
	table.setLastChannelID(testUser, Mumble::ROOT_CHANNEL_ID);
	QVERIFY(toSeconds(table.getLastActive(testUser)) > toSeconds(lastActive));


	table.removeUser(testUser);
	QVERIFY(!table.userExists(testUser));

	MUMBLE_END_TEST_CASE
}

void ServerDatabaseTest::userPropertyTable_general() {
	MUMBLE_BEGIN_TEST_CASE

	unsigned int existingServerID    = 0;
	unsigned int nonExistingServerID = 5;
	unsigned int existingUserID      = 0;
	unsigned int nonExistingUserID   = 5;
	::msdb::DBUser user(existingServerID, existingUserID);

	::msdb::DBChannel rootChannel;
	rootChannel.channelID = Mumble::ROOT_CHANNEL_ID;
	rootChannel.parentID  = rootChannel.channelID;
	rootChannel.serverID  = existingServerID;
	rootChannel.name      = "Root";

	db.getServerTable().addServer(existingServerID);
	db.getChannelTable().addChannel(rootChannel);
	db.getUserTable().addUser(user, "Pete");

	QVERIFY(db.getServerTable().serverExists(existingServerID));
	QVERIFY(!db.getServerTable().serverExists(nonExistingServerID));
	QVERIFY(db.getUserTable().userExists(user));
	QVERIFY(!db.getUserTable().userExists(::msdb::DBUser(existingServerID, nonExistingUserID)));

	::msdb::UserPropertyTable &table = db.getUserPropertyTable();

	QVERIFY(!table.isPropertySet(user, ::msdb::UserProperty::Email));

	QVERIFY(table.findUsersWithProperty(existingServerID, ::msdb::UserProperty::Email, "pete@random.com").empty());

	table.setProperty(user, ::msdb::UserProperty::Email, "pete@random.com");
	table.setProperty(user, ::msdb::UserProperty::kdfIterations, std::to_string(42));
	// Immediately update value
	table.setProperty(user, ::msdb::UserProperty::kdfIterations, std::to_string(5));

	QVERIFY(table.findUsersWithProperty(existingServerID, ::msdb::UserProperty::Email, "pete@random.com").size() == 1);
	QVERIFY(table.findUsersWithProperty(nonExistingServerID, ::msdb::UserProperty::Email, "pete@random.com").empty());
	QCOMPARE(table.findUsersWithProperty(existingServerID, ::msdb::UserProperty::Email, "pete@random.com")[0],
			 user.registeredUserID);

	QCOMPARE(table.getProperty< std::string >(user, ::msdb::UserProperty::Email), std::string("pete@random.com"));
	QCOMPARE(table.getProperty< int >(user, ::msdb::UserProperty::kdfIterations), 5);
	QCOMPARE(table.getProperty< unsigned int >(user, ::msdb::UserProperty::kdfIterations),
			 static_cast< unsigned int >(5));
	// By default, querying non-existing values or using a wrong type will result in an exception
	QVERIFY_THROWS_EXCEPTION(
		::mdb::AccessException,
		table.getProperty< int >(::msdb::DBUser(nonExistingServerID, existingUserID), ::msdb::UserProperty::Email));
	QVERIFY_THROWS_EXCEPTION(
		::mdb::AccessException,
		table.getProperty< int >(::msdb::DBUser(existingServerID, nonExistingUserID), ::msdb::UserProperty::Email));
	QVERIFY_THROWS_EXCEPTION(::mdb::AccessException, table.getProperty< int >(user, ::msdb::UserProperty::Comment));
	// However, we can instead request for a default value to be returned instead
	std::string fetchedValue = table.getProperty< std::string, false >(
		::msdb::DBUser(existingServerID, nonExistingUserID), ::msdb::UserProperty::Comment, "A random comment");
	QCOMPARE(fetchedValue, std::string("A random comment"));
	fetchedValue = table.getProperty< std::string, false >(user, ::msdb::UserProperty::Comment, "A random comment");
	QCOMPARE(fetchedValue, std::string("A random comment"));

	fetchedValue = table.getProperty< std::string, false >(user, ::msdb::UserProperty::Comment, "A random comment");
	QCOMPARE(fetchedValue, std::string("A random comment"));

	QVERIFY(table.isPropertySet(user, ::msdb::UserProperty::Email));
	QVERIFY(table.isPropertySet(user, ::msdb::UserProperty::kdfIterations));

	table.clearProperty(user, ::msdb::UserProperty::Email);

	QVERIFY(!table.isPropertySet(user, ::msdb::UserProperty::Email));
	QVERIFY(table.isPropertySet(user, ::msdb::UserProperty::kdfIterations));

	table.setProperty(user, ::msdb::UserProperty::CertificateHash, "CertHash");

	QVERIFY(table.isPropertySet(user, ::msdb::UserProperty::CertificateHash));
	QVERIFY(table.isPropertySet(user, ::msdb::UserProperty::kdfIterations));

	table.clearAllProperties(::msdb::DBUser(nonExistingServerID, nonExistingUserID));

	QVERIFY(table.isPropertySet(user, ::msdb::UserProperty::CertificateHash));
	QVERIFY(table.isPropertySet(user, ::msdb::UserProperty::kdfIterations));

	table.clearAllProperties(user);

	QVERIFY(!table.isPropertySet(user, ::msdb::UserProperty::CertificateHash));
	QVERIFY(!table.isPropertySet(user, ::msdb::UserProperty::kdfIterations));

	MUMBLE_END_TEST_CASE
}

void ServerDatabaseTest::groupTable_general() {
	MUMBLE_BEGIN_TEST_CASE

	unsigned int existingServerID     = 0;
	unsigned int nonExistingServerID  = 5;
	unsigned int existingChannelID    = 3;
	unsigned int nonExistingChannelID = 5;

	::msdb::DBChannel rootChannel;
	rootChannel.channelID = Mumble::ROOT_CHANNEL_ID;
	rootChannel.parentID  = rootChannel.channelID;
	rootChannel.serverID  = existingServerID;
	rootChannel.name      = "Root";

	::msdb::DBChannel otherChannel;
	otherChannel.channelID = existingChannelID;
	otherChannel.parentID  = rootChannel.channelID;
	otherChannel.serverID  = existingServerID;
	otherChannel.name      = "Other channel";

	db.getServerTable().addServer(existingServerID);
	db.getChannelTable().addChannel(rootChannel);
	db.getChannelTable().addChannel(otherChannel);

	QVERIFY(db.getServerTable().serverExists(existingServerID));
	QVERIFY(!db.getServerTable().serverExists(nonExistingServerID));
	QVERIFY(db.getChannelTable().channelExists(rootChannel));
	QVERIFY(db.getChannelTable().channelExists(otherChannel));
	QVERIFY(!db.getChannelTable().channelExists(existingServerID, nonExistingChannelID));

	::msdb::GroupTable &table = db.getGroupTable();

	::msdb::DBGroup group;
	group.serverID  = existingServerID;
	group.name      = "Example group";
	group.groupID   = 0;
	group.channelID = otherChannel.channelID;

	QVERIFY(!table.groupExists(group));
	QCOMPARE(table.getFreeGroupID(existingServerID), static_cast< unsigned int >(0));

	QCOMPARE(table.countGroups(existingServerID, otherChannel.channelID), static_cast< std::size_t >(0));
	QCOMPARE(table.countGroups(existingServerID, rootChannel.channelID), static_cast< std::size_t >(0));

	QCOMPARE(table.findGroupID(existingServerID, group.name), boost::optional< unsigned int >());

	table.addGroup(group);

	QCOMPARE(table.findGroupID(existingServerID, group.name), boost::optional< unsigned int >(group.groupID));
	QCOMPARE(table.findGroupID(nonExistingServerID, group.name), boost::optional< unsigned int >());

	QVERIFY(table.groupExists(group));
	QCOMPARE(table.getFreeGroupID(existingServerID), static_cast< unsigned int >(1));

	QCOMPARE(table.countGroups(existingServerID, otherChannel.channelID), static_cast< std::size_t >(1));
	QCOMPARE(table.countGroups(existingServerID, rootChannel.channelID), static_cast< std::size_t >(0));

	// This should be a no-op as there currently are no groups connected with the root channel
	table.clearGroups(existingServerID, rootChannel.channelID);
	QCOMPARE(table.countGroups(existingServerID, otherChannel.channelID), static_cast< std::size_t >(1));
	QCOMPARE(table.countGroups(existingServerID, rootChannel.channelID), static_cast< std::size_t >(0));

	QCOMPARE(table.getGroup(group.serverID, group.groupID), group);

	group.inherit        = false;
	group.is_inheritable = false;
	group.name           = "Another group name";

	table.updateGroup(group);

	QCOMPARE(table.getGroup(group.serverID, group.groupID), group);

	// Empty group names are invalid
	group.name = "";
	QVERIFY_THROWS_EXCEPTION(::mdb::FormatException, table.updateGroup(group));
	unsigned int oldID = group.groupID;
	group.groupID      = table.getFreeGroupID(group.serverID);
	QVERIFY_THROWS_EXCEPTION(::mdb::FormatException, table.addGroup(group));

	group.groupID = oldID;

	QVERIFY(table.groupExists(group));
	table.removeGroup(group);
	QVERIFY(!table.groupExists(group));

	QCOMPARE(table.countGroups(existingServerID, otherChannel.channelID), static_cast< std::size_t >(0));

	group.name = "My group";

	table.addGroup(group);

	::msdb::DBGroup otherGroup = group;
	otherGroup.groupID         = table.getFreeGroupID(group.serverID);
	otherGroup.channelID       = nonExistingChannelID;
	otherGroup.name            = "Other group";

	// Non-existing channel should throw
	QVERIFY_THROWS_EXCEPTION(::mdb::AccessException, table.addGroup(otherGroup));

	otherGroup.channelID = group.channelID;
	otherGroup.name      = group.name;
	// Duplicate group name should throw
	QVERIFY_THROWS_EXCEPTION(::mdb::AccessException, table.addGroup(otherGroup));

	otherGroup.name = "Other group";
	table.addGroup(otherGroup);

	std::vector<::msdb::DBGroup > expectedGroups = { group, otherGroup };

	QVERIFY(table.getAllGroups(existingServerID, rootChannel.channelID).empty());
	std::vector<::msdb::DBGroup > fetchedGroups = table.getAllGroups(group.serverID, group.channelID);
	QCOMPARE(fetchedGroups.size(), static_cast< std::size_t >(table.countGroups(group.serverID, group.channelID)));
	QVERIFY(fetchedGroups.size() == expectedGroups.size()
			&& std::is_permutation(expectedGroups.begin(), expectedGroups.end(), fetchedGroups.begin()));

	QCOMPARE(table.countGroups(existingServerID, otherChannel.channelID), static_cast< std::size_t >(2));
	table.clearGroups(existingServerID, otherChannel.channelID);
	QCOMPARE(table.countGroups(existingServerID, otherChannel.channelID), static_cast< std::size_t >(0));

	MUMBLE_END_TEST_CASE
}

void ServerDatabaseTest::groupMemberTable_general() {
	MUMBLE_BEGIN_TEST_CASE

	unsigned int existingServerID     = 0;
	unsigned int nonExistingServerID  = 5;
	unsigned int nonExistingChannelID = 5;
	unsigned int nonExistingUserID    = 42;
	unsigned int nonExistingGroupID   = 45;

	::msdb::DBChannel rootChannel;
	rootChannel.channelID = Mumble::ROOT_CHANNEL_ID;
	rootChannel.parentID  = rootChannel.channelID;
	rootChannel.serverID  = existingServerID;
	rootChannel.name      = "Root";

	::msdb::DBChannel otherChannel;
	otherChannel.channelID = 3;
	otherChannel.parentID  = rootChannel.channelID;
	otherChannel.serverID  = existingServerID;
	otherChannel.name      = "Other channel";

	::msdb::DBUser userA(existingServerID, 0);
	::msdb::DBUser userB(existingServerID, 1);

	::msdb::DBGroup groupA;
	groupA.serverID  = existingServerID;
	groupA.groupID   = 0;
	groupA.channelID = rootChannel.channelID;
	groupA.name      = "Group A";

	::msdb::DBGroup groupB;
	groupB.serverID  = existingServerID;
	groupB.groupID   = 1;
	groupB.channelID = otherChannel.channelID;
	groupB.name      = "Group B";

	db.getServerTable().addServer(existingServerID);
	db.getChannelTable().addChannel(rootChannel);
	db.getChannelTable().addChannel(otherChannel);
	db.getUserTable().addUser(userA, "userA");
	db.getUserTable().addUser(userB, "userB");
	db.getGroupTable().addGroup(groupA);
	db.getGroupTable().addGroup(groupB);

	QVERIFY(db.getServerTable().serverExists(existingServerID));
	QVERIFY(!db.getServerTable().serverExists(nonExistingServerID));
	QVERIFY(db.getChannelTable().channelExists(rootChannel));
	QVERIFY(db.getChannelTable().channelExists(otherChannel));
	QVERIFY(!db.getChannelTable().channelExists(existingServerID, nonExistingChannelID));
	QVERIFY(!db.getUserTable().userExists(::msdb::DBUser(existingServerID, nonExistingUserID)));
	QVERIFY(db.getUserTable().userExists(userA));
	QVERIFY(db.getUserTable().userExists(userB));
	QVERIFY(!db.getGroupTable().groupExists(existingServerID, nonExistingGroupID));
	QVERIFY(db.getGroupTable().groupExists(groupA));
	QVERIFY(db.getGroupTable().groupExists(groupB));


	::msdb::GroupMemberTable &table = db.getGroupMemberTable();

	::msdb::DBGroupMember memberA(existingServerID, groupA.groupID, userA.registeredUserID);
	::msdb::DBGroupMember memberB(existingServerID, groupB.groupID, userB.registeredUserID);

	QVERIFY(!table.entryExists(memberA));
	QVERIFY(!table.entryExists(memberB));

	std::vector<::msdb::DBGroupMember > fetchedMembers = table.getEntries(existingServerID, groupA.groupID);
	QVERIFY(fetchedMembers.empty());

	table.addEntry(memberA);

	QVERIFY(table.entryExists(memberA));
	QVERIFY(!table.entryExists(memberB));

	std::vector<::msdb::DBGroupMember > expectedMembers = { memberA };

	fetchedMembers = table.getEntries(existingServerID, groupB.groupID);
	QVERIFY(fetchedMembers.empty());

	fetchedMembers = table.getEntries(existingServerID, groupA.groupID);
	QCOMPARE(fetchedMembers, expectedMembers);

	table.addEntry(memberB);

	QVERIFY(table.entryExists(memberB));

	expectedMembers = { memberA };
	fetchedMembers  = table.getEntries(existingServerID, groupA.groupID);
	QCOMPARE(fetchedMembers, expectedMembers);

	expectedMembers = { memberB };
	fetchedMembers  = table.getEntries(existingServerID, groupB.groupID);
	QCOMPARE(fetchedMembers, expectedMembers);

	table.removeEntry(memberA);
	QVERIFY(!table.entryExists(memberA));

	// Re-adding the same entry should error
	QVERIFY_THROWS_EXCEPTION(::mdb::AccessException, table.addEntry(memberB));

	// Referencing a non-existent group ID should error
	memberA.groupID = nonExistingGroupID;
	QVERIFY_THROWS_EXCEPTION(::mdb::AccessException, table.addEntry(memberA));

	// Using a non-existent server ID should error
	memberA.groupID  = groupA.groupID;
	memberA.serverID = nonExistingServerID;
	QVERIFY_THROWS_EXCEPTION(::mdb::AccessException, table.addEntry(memberA));

	// Using a non-existent user ID should error
	memberA.serverID = existingServerID;
	memberA.userID   = nonExistingUserID;
	QVERIFY_THROWS_EXCEPTION(::mdb::AccessException, table.addEntry(memberA));

	MUMBLE_END_TEST_CASE
}

void ServerDatabaseTest::aclTable_general() {
	MUMBLE_BEGIN_TEST_CASE

	unsigned int existingServerID     = 0;
	unsigned int nonExistingServerID  = 5;
	unsigned int nonExistingChannelID = 5;
	unsigned int nonExistingUserID    = 42;
	unsigned int nonExistingGroupID   = 45;

	::msdb::DBChannel rootChannel;
	rootChannel.channelID = Mumble::ROOT_CHANNEL_ID;
	rootChannel.parentID  = rootChannel.channelID;
	rootChannel.serverID  = existingServerID;
	rootChannel.name      = "Root";

	::msdb::DBChannel otherChannel;
	otherChannel.channelID = 3;
	otherChannel.parentID  = rootChannel.channelID;
	otherChannel.serverID  = existingServerID;
	otherChannel.name      = "Other channel";

	::msdb::DBGroup groupA;
	groupA.serverID  = existingServerID;
	groupA.groupID   = 0;
	groupA.channelID = rootChannel.channelID;
	groupA.name      = "Group A";

	::msdb::DBGroup groupB;
	groupB.serverID  = existingServerID;
	groupB.groupID   = 1;
	groupB.channelID = otherChannel.channelID;
	groupB.name      = "Group B";

	db.getServerTable().addServer(existingServerID);
	db.getChannelTable().addChannel(rootChannel);
	db.getChannelTable().addChannel(otherChannel);
	db.getGroupTable().addGroup(groupA);
	db.getGroupTable().addGroup(groupB);

	QVERIFY(db.getServerTable().serverExists(existingServerID));
	QVERIFY(!db.getServerTable().serverExists(nonExistingServerID));
	QVERIFY(db.getChannelTable().channelExists(rootChannel));
	QVERIFY(db.getChannelTable().channelExists(otherChannel));
	QVERIFY(!db.getChannelTable().channelExists(existingServerID, nonExistingChannelID));
	QVERIFY(!db.getUserTable().userExists(::msdb::DBUser(existingServerID, nonExistingUserID)));
	QVERIFY(!db.getGroupTable().groupExists(existingServerID, nonExistingGroupID));
	QVERIFY(db.getGroupTable().groupExists(groupA));
	QVERIFY(db.getGroupTable().groupExists(groupB));


	::msdb::ACLTable &table = db.getACLTable();

	::msdb::DBAcl acl;
	acl.serverID        = existingServerID;
	acl.channelID       = groupA.channelID;
	acl.affectedGroupID = groupA.groupID;
	acl.priority        = 5;

	QVERIFY(!table.aclExists(acl));
	table.addACL(acl);
	QVERIFY(table.aclExists(acl));

	// Duplicating the same ACL should error
	QVERIFY_THROWS_EXCEPTION(::mdb::AccessException, table.addACL(acl));
	// Changing the priority should be enough to make the insert successful though
	::msdb::DBAcl acl2 = acl;
	acl2.priority += 1;
	acl2.affectedGroupID.reset();
	acl2.affectedMetaGroup = ::msdb::DBAcl::MetaGroup::Sub;
	acl2.groupModifiers    = { "!", "~", ",0,1" };
	table.addACL(acl2);

	QVERIFY(table.aclExists(acl2));
	table.removeACL(acl2);
	QVERIFY(!table.aclExists(acl2));

	// Invalid server should throw
	acl2.serverID = nonExistingServerID;
	QVERIFY_THROWS_EXCEPTION(::mdb::AccessException, table.addACL(acl2));

	// Invalid channel should throw
	acl2.serverID  = existingServerID;
	acl2.channelID = nonExistingChannelID;
	QVERIFY_THROWS_EXCEPTION(::mdb::AccessException, table.addACL(acl2));

	// Invalid group should throw
	acl2.channelID = rootChannel.channelID;
	acl2.affectedUserID.reset();
	acl2.affectedGroupID = nonExistingGroupID;
	QVERIFY_THROWS_EXCEPTION(::mdb::AccessException, table.addACL(acl2));

	// Invalid user should throw
	acl2.channelID      = rootChannel.channelID;
	acl2.affectedUserID = nonExistingUserID;
	acl2.affectedGroupID.reset();
	QVERIFY_THROWS_EXCEPTION(::mdb::AccessException, table.addACL(acl2));

	// An ACL that doesn't affect anyone should throw
	acl2.affectedUserID.reset();
	acl2.affectedGroupID.reset();
	acl2.affectedMetaGroup.reset();
	acl2.accessToken.reset();
	QVERIFY_THROWS_EXCEPTION(::mdb::FormatException, table.addACL(acl2));

	acl2.affectedMetaGroup = ::msdb::DBAcl::MetaGroup::Sub;
	acl2.channelID         = groupB.channelID;

	table.addACL(acl2);

	QCOMPARE(table.countOverallACLs(existingServerID), static_cast< std::size_t >(2));
	QCOMPARE(table.countOverallACLs(nonExistingServerID), static_cast< std::size_t >(0));

	std::vector<::msdb::DBAcl > expectedACLs = { acl };
	QCOMPARE(table.getAllACLs(acl.serverID, acl.channelID), expectedACLs);

	expectedACLs = { acl2 };
	QCOMPARE(table.getAllACLs(acl2.serverID, acl2.channelID), expectedACLs);

	table.clearACLs(acl2.serverID, acl2.channelID);

	QCOMPARE(table.countOverallACLs(existingServerID), static_cast< std::size_t >(1));

	QVERIFY(table.getAllACLs(acl2.serverID, acl2.channelID).empty());

	acl2.affectedMetaGroup.reset();
	acl2.accessToken = "LetMeIn";

	QVERIFY(!table.aclExists(acl2));
	table.addACL(acl2);
	QVERIFY(table.aclExists(acl2));

	expectedACLs = { acl2 };
	QCOMPARE(table.getAllACLs(acl2.serverID, acl2.channelID), expectedACLs);

	MUMBLE_END_TEST_CASE
}

void ServerDatabaseTest::channelLinkTable_general() {
	MUMBLE_BEGIN_TEST_CASE

	unsigned int existingServerID     = 0;
	unsigned int nonExistingServerID  = 5;
	unsigned int nonExistingChannelID = 5;

	::msdb::DBChannel rootChannel;
	rootChannel.channelID = Mumble::ROOT_CHANNEL_ID;
	rootChannel.parentID  = rootChannel.channelID;
	rootChannel.serverID  = existingServerID;
	rootChannel.name      = "Root";

	::msdb::DBChannel firstChannel;
	firstChannel.channelID = 3;
	firstChannel.parentID  = rootChannel.channelID;
	firstChannel.serverID  = existingServerID;
	firstChannel.name      = "First channel";

	::msdb::DBChannel secondChannel;
	secondChannel.channelID = 2;
	secondChannel.parentID  = rootChannel.channelID;
	secondChannel.serverID  = existingServerID;
	secondChannel.name      = "Second channel";

	db.getServerTable().addServer(existingServerID);
	db.getChannelTable().addChannel(rootChannel);
	db.getChannelTable().addChannel(firstChannel);
	db.getChannelTable().addChannel(secondChannel);

	QVERIFY(db.getServerTable().serverExists(existingServerID));
	QVERIFY(!db.getServerTable().serverExists(nonExistingServerID));
	QVERIFY(db.getChannelTable().channelExists(rootChannel));
	QVERIFY(db.getChannelTable().channelExists(firstChannel));
	QVERIFY(db.getChannelTable().channelExists(secondChannel));
	QVERIFY(!db.getChannelTable().channelExists(existingServerID, nonExistingChannelID));


	::msdb::ChannelLinkTable &table = db.getChannelLinkTable();

	QVERIFY(table.getAllLinks(existingServerID).empty());

	::msdb::DBChannelLink firstLink(existingServerID, rootChannel.channelID, firstChannel.channelID);
	::msdb::DBChannelLink secondLink(existingServerID, firstChannel.channelID, secondChannel.channelID);

	QVERIFY(!table.linkExists(firstLink));

	// Adding with an invalid server ID should error
	firstLink.serverID = nonExistingServerID;
	QVERIFY_THROWS_EXCEPTION(::mdb::AccessException, table.addLink(firstLink));

	// Adding with an invalid channel ID should error
	firstLink.serverID       = existingServerID;
	firstLink.firstChannelID = nonExistingChannelID;
	QVERIFY_THROWS_EXCEPTION(::mdb::AccessException, table.addLink(firstLink));
	firstLink.firstChannelID  = rootChannel.channelID;
	firstLink.secondChannelID = nonExistingChannelID;
	QVERIFY_THROWS_EXCEPTION(::mdb::AccessException, table.addLink(firstLink));

	// Adding a link from a channel to itself should throw
	firstLink.secondChannelID = rootChannel.channelID;
	QVERIFY_THROWS_EXCEPTION(::mdb::FormatException, table.addLink(firstLink));

	firstLink.secondChannelID = firstChannel.channelID;

	table.addLink(firstLink);

	QVERIFY(table.linkExists(firstLink));

	// Re-adding the same link should throw
	QVERIFY_THROWS_EXCEPTION(::mdb::AccessException, table.addLink(firstLink));

	// Swapping the two channel IDs shouldn't make a difference
	std::swap(firstLink.firstChannelID, firstLink.secondChannelID);
	QVERIFY(table.linkExists(firstLink));
	QVERIFY_THROWS_EXCEPTION(::mdb::AccessException, table.addLink(firstLink));


	QVERIFY(table.getAllLinks(nonExistingServerID).empty());

	std::vector<::msdb::DBChannelLink > expectedLinks = { firstLink };
	QCOMPARE(table.getAllLinks(existingServerID), expectedLinks);

	table.addLink(secondLink);

	expectedLinks                                    = { firstLink, secondLink };
	std::vector<::msdb::DBChannelLink > fetchedLinks = table.getAllLinks(existingServerID);
	QVERIFY(expectedLinks.size() == fetchedLinks.size());
	QVERIFY(std::is_permutation(expectedLinks.begin(), expectedLinks.end(), fetchedLinks.begin()));

	QVERIFY(table.linkExists(secondLink));
	table.removeLink(secondLink);
	QVERIFY(!table.linkExists(secondLink));

	// Inserting such that secondID <= firstID should error
	try {
		unsigned int firstID  = firstChannel.channelID;
		unsigned int secondID = secondChannel.channelID;

		if (secondID > firstID) {
			std::swap(firstID, secondID);
		}

		db.getSQLHandle() << "INSERT INTO \"" << ::msdb::ChannelLinkTable::NAME << "\" ("
						  << ::msdb::ChannelLinkTable::column::server_id << ", "
						  << ::msdb::ChannelLinkTable::column::first_id << ", "
						  << ::msdb::ChannelLinkTable::column::second_id << ") VALUES (:serverID, :firstID, :secondID)",
			soci::use(existingServerID), soci::use(firstID), soci::use(secondID);

		QFAIL("Was able to insert secondID <= firstID - violates internal constraint");
	} catch (const soci::soci_error &) {
		// Above statement throwing, is exactly what we want
	}

	MUMBLE_END_TEST_CASE
}

void ServerDatabaseTest::ipAddress_conversions() {
	std::array< std::uint8_t, 16 > ipv6Address;
	ipv6Address.fill(0);

	std::string stringRep                      = ::msdb::DBBan::ipv6ToString(ipv6Address);
	std::string expectedRep                    = "0000:0000:0000:0000:0000:0000:0000:0000";
	std::array< std::uint8_t, 16 > fetchedAddr = ::msdb::DBBan::ipv6FromString(expectedRep);

	QCOMPARE(stringRep, expectedRep);
	QCOMPARE(fetchedAddr, ipv6Address);

	expectedRep = "2345:0425:2ca1:0000:0000:0567:5673:23b5";
	ipv6Address = { 0x23, 0x45, 0x04, 0x25, 0x2c, 0xa1, 0x00, 0x00, 0x00, 0x00, 0x05, 0x67, 0x56, 0x73, 0x23, 0xb5 };
	fetchedAddr = ::msdb::DBBan::ipv6FromString(expectedRep);
	stringRep   = ::msdb::DBBan::ipv6ToString(ipv6Address);

	QCOMPARE(stringRep, expectedRep);
	QCOMPARE(fetchedAddr, ipv6Address);

	std::array< std::uint8_t, 4 > ipv4Address = { 127, 0, 0, 1 };
	ipv6Address = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x7f, 0x00, 0x00, 0x01 };
	fetchedAddr = ::msdb::DBBan::ipv4ToIpv6(ipv4Address, false);

	std::uint32_t ipv4Numeric = (static_cast< std::uint32_t >(127) << (8 + 8 + 8)) + 1;
	std::uint8_t *ipv4Ptr     = reinterpret_cast< std::uint8_t * >(&ipv4Numeric);
	ipv4Address[0]            = ipv4Ptr[0];
	ipv4Address[1]            = ipv4Ptr[1];
	ipv4Address[2]            = ipv4Ptr[2];
	ipv4Address[3]            = ipv4Ptr[3];
	fetchedAddr               = ::msdb::DBBan::ipv4ToIpv6(ipv4Address, true);

	QCOMPARE(fetchedAddr, ipv6Address);
}

void ServerDatabaseTest::banTable_general() {
	MUMBLE_BEGIN_TEST_CASE

	unsigned int existingServerID    = 0;
	unsigned int nonExistingServerID = 5;

	db.getServerTable().addServer(existingServerID);

	QVERIFY(db.getServerTable().serverExists(existingServerID));
	QVERIFY(!db.getServerTable().serverExists(nonExistingServerID));


	::msdb::BanTable &table = db.getBanTable();

	::msdb::DBBan ban(
		existingServerID,
		{ 0x12, 0x23, 0x34, 0x45, 0x56, 0x67, 0x78, 0x89, 0x9a, 0xab, 0xbc, 0xcd, 0xde, 0xef, 0x1f, 0x2f }, 64);

	QVERIFY(!table.banExists(ban));
	QVERIFY(table.getAllBans(existingServerID).empty());

	table.addBan(ban);

	QVERIFY(table.banExists(ban));

	QCOMPARE(table.getBanDetails(ban.serverID, ban.baseAddress, ban.prefixLength), ban);

	std::vector<::msdb::DBBan > expectedBans = { ban };

	QCOMPARE(table.getAllBans(existingServerID), expectedBans);

	::msdb::DBBan ban2(existingServerID, ban.baseAddress, ban.prefixLength);
	ban2.bannedUserName     = "Hugo";
	ban2.bannedUserCertHash = "xyz";
	ban2.reason             = "Hugo didn't behave properly";
	ban2.startDate          = std::chrono::system_clock::now();
	ban2.duration           = std::chrono::seconds(42);

	// Since we re-used the server ID, base address and prefix length, adding this should error
	QVERIFY_THROWS_EXCEPTION(::mdb::AccessException, table.addBan(ban2));

	ban2.prefixLength++;
	QVERIFY(!table.banExists(ban2));
	table.addBan(ban2);
	QVERIFY(table.banExists(ban2));

	QCOMPARE(table.getBanDetails(ban2.serverID, ban2.baseAddress, ban2.prefixLength), ban2);

	expectedBans                            = { ban, ban2 };
	std::vector<::msdb::DBBan > fetchedBans = table.getAllBans(existingServerID);
	QCOMPARE(fetchedBans.size(), expectedBans.size());
	QVERIFY(std::is_permutation(expectedBans.begin(), expectedBans.end(), fetchedBans.begin()));

	table.clearBans(nonExistingServerID);
	QVERIFY(table.banExists(ban));

	table.clearBans(existingServerID);
	QVERIFY(!table.banExists(ban));
	QVERIFY(!table.banExists(ban2));

	table.addBan(ban);
	QVERIFY(table.banExists(ban));

	table.setBans(existingServerID, { ban2 });

	QVERIFY(!table.banExists(ban));
	QVERIFY(table.banExists(ban2));

	// Adding to a non-existing server should error
	QVERIFY_THROWS_EXCEPTION(::mdb::AccessException, table.addBan(::msdb::DBBan(nonExistingServerID, {}, 0)));

	QVERIFY(table.getAllBans(nonExistingServerID).empty());

	MUMBLE_END_TEST_CASE
}

void ServerDatabaseTest::channelListenerTable_general() {
	MUMBLE_BEGIN_TEST_CASE

	unsigned int existingServerID     = 0;
	unsigned int nonExistingServerID  = 5;
	unsigned int nonExistingChannelID = 7;
	unsigned int nonExistingUserID    = 5;

	::msdb::DBUser user1(existingServerID, 2);
	::msdb::DBUser user2(existingServerID, 3);

	::msdb::DBChannel rootChannel;
	rootChannel.channelID = Mumble::ROOT_CHANNEL_ID;
	rootChannel.parentID  = rootChannel.channelID;
	rootChannel.serverID  = existingServerID;
	rootChannel.name      = "Root";

	::msdb::DBChannel firstChannel;
	firstChannel.channelID = 3;
	firstChannel.parentID  = rootChannel.channelID;
	firstChannel.serverID  = existingServerID;
	firstChannel.name      = "First channel";

	db.getServerTable().addServer(existingServerID);
	db.getChannelTable().addChannel(rootChannel);
	db.getChannelTable().addChannel(firstChannel);
	db.getUserTable().addUser(user1, "User 1");
	db.getUserTable().addUser(user2, "User 2");

	QVERIFY(db.getServerTable().serverExists(existingServerID));
	QVERIFY(!db.getServerTable().serverExists(nonExistingServerID));
	QVERIFY(db.getChannelTable().channelExists(rootChannel));
	QVERIFY(db.getChannelTable().channelExists(firstChannel));
	QVERIFY(!db.getChannelTable().channelExists(existingServerID, nonExistingChannelID));
	QVERIFY(db.getUserTable().userExists(user1));
	QVERIFY(db.getUserTable().userExists(user2));
	QVERIFY(!db.getUserTable().userExists(::msdb::DBUser(existingServerID, nonExistingUserID)));


	::msdb::ChannelListenerTable &table = db.getChannelListenerTable();

	::msdb::DBChannelListener listener1(existingServerID, firstChannel.channelID, user1.registeredUserID);
	::msdb::DBChannelListener listener2(existingServerID, rootChannel.channelID, user2.registeredUserID);

	listener2.volumeAdjustment = 3.5;
	listener2.enabled          = false;

	QVERIFY(table.getListenersForChannel(existingServerID, rootChannel.channelID).empty());
	QVERIFY(table.getListenersForChannel(nonExistingServerID, rootChannel.channelID).empty());
	QVERIFY(table.getListenersForChannel(existingServerID, nonExistingChannelID).empty());
	QVERIFY(table.getListenersForUser(existingServerID, user1.registeredUserID).empty());
	QVERIFY(table.getListenersForUser(nonExistingServerID, user1.registeredUserID).empty());
	QVERIFY(table.getListenersForUser(existingServerID, nonExistingUserID).empty());
	QVERIFY_THROWS_EXCEPTION(::mdb::NoDataException, table.getListenerDetails(listener1));

	QVERIFY(!table.listenerExists(listener1));
	QVERIFY(!table.listenerExists(listener2));

	table.addListener(listener1);

	QVERIFY(table.listenerExists(listener1));
	QVERIFY(!table.listenerExists(listener2));

	table.addListener(listener2);

	QVERIFY(table.listenerExists(listener2));
	QCOMPARE(table.getListenerDetails(listener2), listener2);

	std::vector<::msdb::DBChannelListener > expectedListeners = { listener1 };
	std::vector<::msdb::DBChannelListener > fetchedListeners =
		table.getListenersForUser(user1.serverID, user1.registeredUserID);

	QCOMPARE(fetchedListeners, expectedListeners);

	expectedListeners = { listener2 };
	fetchedListeners  = table.getListenersForChannel(rootChannel.serverID, rootChannel.channelID);

	QCOMPARE(fetchedListeners, expectedListeners);

	table.removeListener(listener2);

	QVERIFY(!table.listenerExists(listener2));

	listener2.channelID = listener1.channelID;

	table.addListener(listener2);

	expectedListeners = { listener1, listener2 };
	fetchedListeners  = table.getListenersForChannel(firstChannel.serverID, firstChannel.channelID);
	QVERIFY(expectedListeners.size() == fetchedListeners.size());
	QVERIFY(std::is_permutation(expectedListeners.begin(), expectedListeners.end(), fetchedListeners.begin()));

	listener1.volumeAdjustment = 0.876f;
	table.updateListener(listener1);

	expectedListeners = { listener1 };
	fetchedListeners  = table.getListenersForUser(user1.serverID, user1.registeredUserID);

	QCOMPARE(fetchedListeners, expectedListeners);

	// Double-adding should error
	QVERIFY_THROWS_EXCEPTION(::mdb::AccessException, table.addListener(listener1));

	// Adding to invalid server should error
	::msdb::DBChannelListener invalid = listener1;
	invalid.serverID                  = nonExistingServerID;
	QVERIFY_THROWS_EXCEPTION(::mdb::AccessException, table.addListener(invalid));

	// Adding with invalid channel should error
	invalid.serverID  = existingServerID;
	invalid.channelID = nonExistingChannelID;
	QVERIFY_THROWS_EXCEPTION(::mdb::AccessException, table.addListener(invalid));

	// Adding with invalid user should error
	invalid.channelID = rootChannel.channelID;
	invalid.userID    = nonExistingUserID;
	QVERIFY_THROWS_EXCEPTION(::mdb::AccessException, table.addListener(invalid));


	MUMBLE_END_TEST_CASE
}

void ServerDatabaseTest::database_scheme_migration() {
	::mumble::db::test::JSONAssembler dataAssembler;

	for (unsigned int schemeVersion = 6; schemeVersion <= ::msdb::ServerDatabase::DB_SCHEME_VERSION; ++schemeVersion) {
		qInfo() << "Current scheme version:" << schemeVersion;

		MUMBLE_BEGIN_TEST_CASE_NO_INIT

		db.configureStandardTablesWithoutCreation();

		const ::mumble::db::test::JSONAssembler::DataPair tableData = dataAssembler.buildTestData(schemeVersion, db);

		db.resetTables();

		PlainDB plainDB(currentBackend, schemeVersion);
		plainDB.init(::mumble::db::test::utils::getConnectionParamter(currentBackend));

		// Populate the DB with the old data
		plainDB.importFromJSON(tableData.inputData, true);


		// Start migration (happens automatically on init, if the scheme version is lower)
		db.init(::mumble::db::test::utils::getConnectionParamter(currentBackend));

		qInfo() << "Migration successful";


		// Now check whether the migrated data looks the way we would expect
		nlohmann::json migratedData = db.exportToJSON();

		mdb::test::utils::alignColumnOrder(tableData.migratedData, migratedData);
		mdb::test::utils::alignRowOrder(tableData.migratedData, migratedData);

		// First check that we obtained the expected set of tables
		std::vector< std::string > expectedTables;
		std::vector< std::string > encounteredTables;
		for (auto iter : tableData.migratedData.at("tables").items()) {
			if (iter.key() == ::mdb::MetaTable::NAME) {
				continue;
			}
			expectedTables.push_back(iter.key());
		}
		for (auto iter : migratedData.at("tables").items()) {
			if (iter.key() == ::mdb::MetaTable::NAME) {
				continue;
			}
			encounteredTables.push_back(iter.key());
		}
		std::sort(expectedTables.begin(), expectedTables.end());
		std::sort(encounteredTables.begin(), encounteredTables.end());

		QCOMPARE(encounteredTables, expectedTables);

		// Now ensure that each table contains the expected content
		for (const std::string &currentTable : encounteredTables) {
			if (currentTable == mdb::MetaTable::NAME) {
				// For the time being, we don't compare the contents of the meta table as there shouldn't be
				// anything in it that needs migration.
				continue;
			}

			nlohmann::json diff = nlohmann::json::diff(tableData.migratedData["tables"].at(currentTable),
													   migratedData["tables"].at(currentTable));

			qInfo() << "Checking contents of table " << QString::fromStdString(currentTable) << "...";

			// If the diff is empty, then the two JSON objects are equal. However, the diff
			// can be read much more easily than the string representation of the JSON objects.
			// The diff describes how to get from the expected data to the one that has been observed.
			QCOMPARE(diff, nlohmann::json::array_t{});
		}

		db.destroyTables();


		MUMBLE_END_TEST_CASE
	}
}

QTEST_MAIN(ServerDatabaseTest)
#include "ServerDatabaseTest.moc"
