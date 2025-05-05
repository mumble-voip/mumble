// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_SERVER_SERVERUSERINFO_H_
#define MUMBLE_SERVER_SERVERUSERINFO_H_

#include "User.h"
#include "Version.h"

#include <QString>
#include <QStringList>

#include <string>

/**
 * A collection of properties of a ServerUser object for which it must be possible to have access to it without
 * creating a ServerUser object. Therefore, this can be seen as a data collection used for external processing.
 */
class ServerUserInfo : public User {
public:
	Version::full_t m_version = Version::UNKNOWN;
	QString qsRelease;
	QString qsOS;
	QString qsOSVersion;

	std::string ssContext;
	QString qsIdentity;

	bool bVerified;
	QStringList qslEmail;
};

#endif // MUMBLE_SERVER_SERVERUSERINFO_H_
