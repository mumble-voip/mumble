// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "User.h"
#include "Channel.h"

User::User() {
	uiSession = 0;
	iId       = -1;
	bMute = bDeaf = false;
	bSelfMute = bSelfDeaf = false;
	bPrioritySpeaker      = false;
	bRecording            = false;
	bSuppress             = false;
	cChannel              = 0;
}

bool User::lessThan(const User *first, const User *second) {
	// We explicitly don't use localeAwareCompare as this would result in a different
	// ordering of users on clients with different locales. This is not what one would
	// expect and thus we don't take the locale into account for comparing users.
	// First we compare case-insensitively, in order to sort users with different names
	// in a case-insensitive (intuitive) way. However, if some users have the same name
	// that only differs in casing (theoretically possible), a case-insensitive comparison
	// would not yield a (guaranteed) stable order of such users, which is why we compare
	// such cases in a case-sensitive way.
	int result = QString::compare(first->qsName, second->qsName, Qt::CaseInsensitive);

	if (result == 0) {
		// Names are equal (except for casing)
		result = QString::compare(first->qsName, second->qsName, Qt::CaseSensitive);
	}

	return result < 0;
}
