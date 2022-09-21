// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include <QString>
#include <QtTest>

#include "database/AccessException.h"
#include "database/Backend.h"
#include "database/FormatException.h"
#include "database/NoDataException.h"

#include "database/ChannelPropertyTable.h"
#include "database/ChannelTable.h"
#include "database/ConfigTable.h"
#include "database/DBChannel.h"
#include "database/LogTable.h"
#include "database/ServerDatabase.h"
#include "database/ServerTable.h"
#include "database/UserProperty.h"
#include "database/UserPropertyTable.h"
#include "database/UserTable.h"

#include "MumbleConstants.h"

#include "TestUtils.h"

#include <nlohmann/json.hpp>

#include <algorithm>
#include <chrono>
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

/**
 * Helper function to convert a std::chrono::timepoint to seconds since epoch
 */
template< typename TimePoint > std::size_t toSeconds(const TimePoint &tp) {
	return std::chrono::duration_cast< std::chrono::seconds >(tp.time_since_epoch()).count();
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
	void userTable_general();
	void userPropertyTable_general();
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
	QCOMPARE(table.getFreeServerID(), static_cast< unsigned int >(0));
	table.addServer(0);
	QVERIFY(table.serverExists(0));
	// Some RMDBs (looking at you MySQL!) will by default treat an explicit value of zero as "please auto-generate the
	// next number in this auto_increment column" which usually happens to be 1.
	QVERIFY(!table.serverExists(1));
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
	unsigned int otherServerID       = 2;
	unsigned int nonExistingServerID = 5;

	db.getServerTable().addServer(existingServerID);
	db.getServerTable().addServer(otherServerID);

	QVERIFY(db.getServerTable().serverExists(existingServerID));
	QVERIFY(db.getServerTable().serverExists(otherServerID));
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

	QCOMPARE(channelTable.getFreeChannelID(existingServerID), static_cast< unsigned int >(1));

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

	QCOMPARE(channelTable.getFreeChannelID(existingServerID), static_cast< unsigned int >(2));

	::msdb::DBChannel third;
	third.serverID  = existingServerID;
	third.channelID = channelTable.getFreeChannelID(existingServerID);
	third.parentID  = rootChannel.channelID;
	third.name      = "Third channel";

	channelTable.addChannel(third);
	channelTable.removeChannel(other);

	// We expect to first re-use the ID 1 before moving on to ID 3
	QCOMPARE(channelTable.getFreeChannelID(existingServerID), static_cast< unsigned int >(1));

	// On this other server, we expect to start at 0 (there are no channels yet)
	QCOMPARE(channelTable.getFreeChannelID(otherServerID), static_cast< unsigned int >(0));

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

	QVERIFY(!table.isPropertySet(existingServerID, existingChannelID, ::msdb::ChannelProperty::Description));

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

	// Get a default string value
	std::string fetchedStr = table.getProperty< std::string, false >(
		existingServerID, existingChannelID, ::msdb::ChannelProperty::Description, "My default");
	QCOMPARE(fetchedStr, std::string("My default"));

	END_TEST_CASE
}

void ServerDatabaseTest::userTable_general() {
	BEGIN_TEST_CASE

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

	table.addUser(testUser, testUserData.name);

	QVERIFY(table.userExists(testUser));

	// Adding a user with an invalid (empty) name should throw
	QVERIFY_EXCEPTION_THROWN(table.addUser(::msdb::DBUser(existingServerID, 13), {}), ::mdb::FormatException);
	// Adding a user to a non-existing server should throw
	QVERIFY_EXCEPTION_THROWN(table.addUser(::msdb::DBUser(nonExistingServerID, 13), "bob"), ::mdb::AccessException);
	// Adding a user with an already in-use ID should throw
	QVERIFY_EXCEPTION_THROWN(table.addUser(::msdb::DBUser(existingServerID, testUser.registeredUserID), "bob"),
							 ::mdb::AccessException);
	// Adding a user with an already in-use name should throw
	QVERIFY_EXCEPTION_THROWN(table.addUser(::msdb::DBUser(existingServerID, 14), testUserData.name),
							 ::mdb::AccessException);

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
	testUserData.lastActive             = std::chrono::steady_clock::now();
	testUserData.lastDisconnect         = std::chrono::steady_clock::now();

	table.updateData(testUser, testUserData);
	CHECK_USER_DATA(testUser, testUserData);
#undef CHECK_USER_DATA

	// Test clear* functions
	table.clearLastDisconnect(testUser);
	QCOMPARE(table.getLastDisconnect(testUser), std::chrono::time_point< std::chrono::steady_clock >());

	table.clearLastActive(testUser);
	QCOMPARE(table.getLastActive(testUser), std::chrono::time_point< std::chrono::steady_clock >());

	// "Clearing" the last channel really means resetting it to the root channel ID
	table.clearLastChannelID(testUser);
	QCOMPARE(table.getLastChannelID(testUser), static_cast< unsigned int >(Mumble::ROOT_CHANNEL_ID));

	table.clearTexture(testUser);
	QCOMPARE(table.getTexture(testUser), std::vector< std::uint8_t >());

	table.clearPassword(testUser);
	QCOMPARE(table.getPassword(testUser), ::msdb::DBUserData::PasswordData());


	// Test set* functions
	std::chrono::time_point< std::chrono::steady_clock > now = std::chrono::steady_clock::now();
	table.setLastDisconnect(testUser, now);
	QCOMPARE(toSeconds(table.getLastDisconnect(testUser)), toSeconds(now));

	table.setLastActive(testUser, now);
	QCOMPARE(toSeconds(table.getLastActive(testUser)), toSeconds(now));

	table.setLastChannelID(testUser, existingChannelID);
	QCOMPARE(table.getLastChannelID(testUser), existingChannelID);
	QVERIFY_EXCEPTION_THROWN(table.setLastChannelID(testUser, nonExistingChannelID), ::mdb::AccessException);

	table.setTexture(testUser, testUserData.texture);
	QCOMPARE(table.getTexture(testUser), testUserData.texture);

	table.setPassword(testUser, testUserData.password);
	QCOMPARE(table.getPassword(testUser), testUserData.password);

	testUserData.name = "Pia";
	table.setName(testUser, testUserData.name);
	QCOMPARE(table.getName(testUser), testUserData.name);
	QVERIFY_EXCEPTION_THROWN(table.setName(testUser, ""), ::mdb::FormatException);


	::msdb::DBUser nonExistingUser(existingServerID, 42);
	QVERIFY(!table.userExists(nonExistingUser));

	// All get* functions should throw, if given a non-existing user
	QVERIFY_EXCEPTION_THROWN(table.getData(nonExistingUser), ::mdb::AccessException);
	QVERIFY_EXCEPTION_THROWN(table.getLastDisconnect(nonExistingUser), ::mdb::AccessException);
	QVERIFY_EXCEPTION_THROWN(table.getLastActive(nonExistingUser), ::mdb::AccessException);
	QVERIFY_EXCEPTION_THROWN(table.getLastChannelID(nonExistingUser), ::mdb::AccessException);
	QVERIFY_EXCEPTION_THROWN(table.getTexture(nonExistingUser), ::mdb::AccessException);
	QVERIFY_EXCEPTION_THROWN(table.getPassword(nonExistingUser), ::mdb::AccessException);
	QVERIFY_EXCEPTION_THROWN(table.getName(nonExistingUser), ::mdb::AccessException);


	// Test findUser
	boost::optional< unsigned int > userID = table.findUser(existingServerID, testUserData.name);
	QVERIFY(userID);
	QCOMPARE(userID.get(), testUser.registeredUserID);

	userID = table.findUser(existingServerID, "I don't exist");
	QVERIFY(!userID);

	userID = table.findUser(nonExistingServerID, testUserData.name);
	QVERIFY(!userID);


	// Test getRegisteredUsers
	::msdb::DBUser additionalUser(existingServerID, 128);
	table.addUser(additionalUser, "Dummy name");
	std::vector<::msdb::DBUser > expectedUsers = { testUser, additionalUser };
	std::vector<::msdb::DBUser > actualUsers   = table.getRegisteredUsers(existingServerID);
	QVERIFY(expectedUsers.size() == actualUsers.size()
			&& std::is_permutation(expectedUsers.begin(), expectedUsers.end(), actualUsers.begin()));

	actualUsers = table.getRegisteredUsers(nonExistingServerID);
	QVERIFY(actualUsers.empty());


	// Test that a user's last channel is reset to the root channel, if the referenced channel gets deleted
	QVERIFY(channel.channelID != Mumble::ROOT_CHANNEL_ID);
	table.setLastChannelID(testUser, channel.channelID);
	QCOMPARE(table.getLastChannelID(testUser), channel.channelID);
	db.getChannelTable().removeChannel(channel);
	QCOMPARE(table.getLastChannelID(testUser), static_cast< unsigned int >(Mumble::ROOT_CHANNEL_ID));


	// Test that changing a user's last channel automatically updates the last_active stats as well
	std::chrono::time_point< std::chrono::steady_clock > lastActive(std::chrono::seconds(12));
	QVERIFY(toSeconds(lastActive) < toSeconds(std::chrono::steady_clock::now()));
	table.setLastActive(testUser, lastActive);
	QCOMPARE(toSeconds(table.getLastActive(testUser)), toSeconds(lastActive));
	table.setLastChannelID(testUser, Mumble::ROOT_CHANNEL_ID);
	QVERIFY(toSeconds(table.getLastActive(testUser)) > toSeconds(lastActive));


	table.removeUser(testUser);
	QVERIFY(!table.userExists(testUser));

	END_TEST_CASE
}

void ServerDatabaseTest::userPropertyTable_general() {
	BEGIN_TEST_CASE

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

	table.setProperty(user, ::msdb::UserProperty::Email, "pete@random.com");
	table.setProperty(user, ::msdb::UserProperty::kdfIterations, std::to_string(5));

	QCOMPARE(table.getProperty< std::string >(user, ::msdb::UserProperty::Email), std::string("pete@random.com"));
	QCOMPARE(table.getProperty< int >(user, ::msdb::UserProperty::kdfIterations), 5);
	QCOMPARE(table.getProperty< unsigned int >(user, ::msdb::UserProperty::kdfIterations),
			 static_cast< unsigned int >(5));
	// By default, querying non-existing values or using a wrong type will result in an exception
	QVERIFY_EXCEPTION_THROWN(
		table.getProperty< int >(::msdb::DBUser(nonExistingServerID, existingUserID), ::msdb::UserProperty::Email),
		::mdb::AccessException);
	QVERIFY_EXCEPTION_THROWN(
		table.getProperty< int >(::msdb::DBUser(existingServerID, nonExistingUserID), ::msdb::UserProperty::Email),
		::mdb::AccessException);
	QVERIFY_EXCEPTION_THROWN(table.getProperty< int >(user, ::msdb::UserProperty::Comment), ::mdb::AccessException);
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

	END_TEST_CASE
}


QTEST_MAIN(ServerDatabaseTest)
#include "ServerDatabaseTest.moc"
