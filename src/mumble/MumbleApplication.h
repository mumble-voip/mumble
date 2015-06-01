/* Copyright (C) 2005-2011, Thorvald Natvig <thorvald@natvig.com>

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

#ifndef MUMBLE_MUMBLE_MUMBLEAPPLICATION_H
#define MUMBLE_MUMBLE_MUMBLEAPPLICATION_H

#include <QApplication>
#include <QUrl>

/**
 * @brief Implements custom system shutdown behavior as well as event filtering.
 */
#if QT_VERSION >= 0x050000 && defined(Q_OS_WIN)
class MumbleApplication : public QApplication, public QAbstractNativeEventFilter {
#else
class MumbleApplication : public QApplication {
#endif
		Q_OBJECT
	public:

		/// The instance function returns an instance
		/// of the MumbleApplication singleton.
		static MumbleApplication *instance();

		MumbleApplication(int &pargc, char **pargv);

		/// applicationVersionRootPath returns
		/// Mumble's "versioned root"-path.
		///
		/// This is a version-specific path that contains
		/// supplementary binaries and other products
		/// that Mumble needs to function.
		///
		/// In the current implementation, the versioned
		/// root path is set by the MUMBLE_VERSION_ROOT
		/// environment variable. This environment variable
		/// is set in the mumble.exe launcher.
		///
		/// If a versioned root path has not been
		/// configured in the environment, the function
		/// returns the same path as Qt's own
		/// QApplication::applicationDirPath().
		QString applicationVersionRootPath();

		bool event(QEvent *e) Q_DECL_OVERRIDE;
#ifdef Q_OS_WIN
# if QT_VERSION >= 0x050000
		bool MumbleApplication::nativeEventFilter(const QByteArray &eventType, void *message, long *result) Q_DECL_OVERRIDE;
# else
		bool winEventFilter(MSG *msg, long *result) Q_DECL_OVERRIDE;
# endif
#endif
		
		QUrl quLaunchURL;
		
	public slots:
		/// Saves state and suppresses ask on quit before system shutdown. 
		void onCommitDataRequest(QSessionManager&);
};

#endif // MUMBLE_MUMBLE_MUMBLEAPPLICATION_H
