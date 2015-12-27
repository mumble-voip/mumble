/* Copyright (C) 2015, Mikkel Krautz <mikkel@krautz.dk>

   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.
   - Neither the name of the Mumble Developers nor the names of its
     contributors may be used to endorse or promote products derived from this
     software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef MUMBLE_MUMBLE_LOCKFILE_H_
#define MUMBLE_MUMBLE_LOCKFILE_H_

#if defined(Q_OS_WIN)
# include <windows.h>
#endif

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
