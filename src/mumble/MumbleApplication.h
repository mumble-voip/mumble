// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_MUMBLEAPPLICATION_H
#define MUMBLE_MUMBLE_MUMBLEAPPLICATION_H

#include <QApplication>
#include <QUrl>

#ifdef Q_OS_WIN
#	include <QAbstractNativeEventFilter>
#endif

/**
 * @brief Implements custom system shutdown behavior as well as event filtering.
 */
#ifdef Q_OS_WIN
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
	bool nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result) Q_DECL_OVERRIDE;
#endif

	QUrl quLaunchURL;

public slots:
	/// Saves state and suppresses ask on quit before system shutdown.
	void onCommitDataRequest(QSessionManager &);
};

#endif // MUMBLE_MUMBLE_MUMBLEAPPLICATION_H
