// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_LOCKFILE_H_
#define MUMBLE_MUMBLE_LOCKFILE_H_

#include <QtCore/QtGlobal>

#ifdef Q_OS_WIN
#	include "win.h"
#endif

#include <QtCore/QString>

/// UserLockFile implements an atomic lock file
/// that can be used as a mutex between different
/// processes run by the same user.
class UserLockFile {
#if defined(Q_OS_WIN)
	HANDLE m_handle;
#endif
	QString m_path;

public:
	/// Constructs a LockFile at path.
	/// The path should be somewhere
	/// owned by the current user, such
	/// as inside the home directory of
	/// the user. This is to avoid clashing
	/// with other lock files.
	UserLockFile(const QString &path);

	/// Destroys the LockFile, and ensures
	/// that it is released.
	~UserLockFile();

	/// Returns the path that the lock file
	/// exists at.
	QString path() const;

	/// Acquires the lock file.
	bool acquire();

	/// Releases the lock file.
	void release();
};

#endif
