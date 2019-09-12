// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "User.h"
#include "Channel.h"

User::User() {
	uiSession = 0;
	iId = -1;
	bMute = bDeaf = false;
	bSelfMute = bSelfDeaf = false;
	bPrioritySpeaker = false;
	bRecording = false;
	bSuppress = false;
	cChannel = 0;
}

bool User::lessThan(const User *first, const User *second) {
	return (QString::localeAwareCompare(first->qsName, second->qsName) < 0);
}
