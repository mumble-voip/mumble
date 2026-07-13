// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_VERSIONCHECK_H_
#define MUMBLE_MUMBLE_VERSIONCHECK_H_

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QUrl>

class QNetworkReply;

/// MUMBLE-TFAR: the update check queries the fork's GitHub releases instead of
/// the upstream mumble.info update server — upstream would advertise vanilla
/// Mumble builds without TFAR. When a newer release is found it offers to
/// download the Windows installer asset and run it.
class VersionCheck : public QObject {
private:
	Q_OBJECT
	Q_DISABLE_COPY(VersionCheck)

public:
	/// verbose: report "already up to date" and errors to the chat log
	/// (manual check). The startup check passes false and only speaks up
	/// when a new version is found.
	VersionCheck(bool verbose, QObject *parent = nullptr);

	/// Whether a found update may be downloaded and installed right away
	/// (after user confirmation). When disabled, only a chat log message
	/// with a download link is shown.
	static bool autoInstallEnabled();
	static void setAutoInstallEnabled(bool enabled);

private slots:
	void onMetadataReceived();
	void onInstallerDownloaded();

private:
	void logInfo(const QString &message);
	void logWarning(const QString &message);

	bool m_verbose;
	QNetworkReply *m_reply = nullptr;
	QString m_newVersion;
	QString m_assetName;
	QUrl m_assetUrl;
};

#endif
