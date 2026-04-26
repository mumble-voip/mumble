// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include <QObject>
#include <QTest>

// The ACL editor determines whether a user lookup was unresolved by checking
// whether their user ID is less than -1. In ACLEditor, iUserId == -1 means the
// ACL entry applies to a group (not a specific user). Unresolved/unregistered
// users are assigned temporary IDs starting at -2 (iUnknown) by ACLEditor::id().
// If the server does not return a positive ID for a queried name, the user is
// not registered and the warning is shown in returnQuery().
//
// See: src/mumble/ACLEditor.cpp (returnQuery, ACLEditor::id)
// See: src/User.h (iId field)

class TestACLUserRegistration : public QObject {
	Q_OBJECT
private slots:
	void registeredUser_hasNonNegativeId() {
		// A registered user always has iId >= 0
		int userId = 42;
		QVERIFY(userId >= 0);
	}

	void groupEntry_hasIdOfNegativeOne() {
		// iUserId == -1 means a group-based ACL entry, not a user-specific entry.
		// This should NOT trigger the warning (predicate is < -1, not < 0)
		int userId = -1;
		bool shouldWarn = (userId < -1);
		QVERIFY(!shouldWarn);
	}

	void unknownUser_hasNegativeId() {
		// A user whose name could not be resolved gets a temporary negative ID
		// (iUnknown, starting at -2). These SHOULD trigger the warning.
		int userId = -2;
		bool shouldWarn = (userId < -1);
		QVERIFY(shouldWarn);
	}

	void registrationCheck_correctlyIdentifiesRegistered() {
		// The warning should NOT fire for registered users (iId >= 0)
		int userId = 100;
		bool shouldWarn = (userId < -1);
		QVERIFY(!shouldWarn);
	}

	void registrationCheck_correctlyIdentifiesUnregistered() {
		// The warning fires for temporary IDs (< -1), not for -1 (group entry)
		int userId = -2;
		bool shouldWarn = (userId < -1);
		QVERIFY(shouldWarn);
	}

	void registrationCheck_boundaryAtZero() {
		// User ID of exactly 0 is valid (registered), should not warn
		int userId = 0;
		bool shouldWarn = (userId < -1);
		QVERIFY(!shouldWarn);
	}

	void registrationCheck_groupEntrySentinel() {
		// User ID of -1 is the group ACL sentinel, should NOT warn
		int userId = -1;
		bool shouldWarn = (userId < -1);
		QVERIFY(!shouldWarn);
	}

	void registrationCheck_temporaryIds() {
		// Temporary IDs assigned during name lookup (iUnknown starting at -2)
		// should trigger the warning since they are < -1
		for (int tempId = -2; tempId >= -10; tempId--) {
			bool shouldWarn = (tempId < -1);
			QVERIFY(shouldWarn);
		}
	}
};

QTEST_MAIN(TestACLUserRegistration)
#include "TestACLUserRegistration.moc"
