// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "UserLockFile.h"

UserLockFile::UserLockFile(const QString &lockFilePath) : m_handle(0), m_path(lockFilePath) {
}

UserLockFile::~UserLockFile() {
	release();
}

QString UserLockFile::path() const {
	return m_path;
}

bool UserLockFile::acquire() {
	if (m_handle != 0) {
		return false;
	}

	m_handle = CreateFile(reinterpret_cast< const wchar_t * >(m_path.utf16()), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS,
						  FILE_ATTRIBUTE_HIDDEN, nullptr);
	if (m_handle == INVALID_HANDLE_VALUE && GetLastError() == ERROR_SHARING_VIOLATION) {
		return false;
	}
	return true;
}

void UserLockFile::release() {
	if (m_handle != 0) {
		CloseHandle(m_handle);
		m_handle = 0;
	}
}
