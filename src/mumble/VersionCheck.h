// Copyright 2005-2020 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_VERSIONCHECK_H_
#define MUMBLE_MUMBLE_VERSIONCHECK_H_
#include <QtCore/QObject>
#ifndef USE_APPIMAGE_UPDATER_BRIDGE
# include <QtCore/QByteArray>
# include <QtCore/QUrl>
#else
# include <QJsonObject>
# include <QString>
# include <AppImageUpdaterBridge>
# include <AppImageUpdaterDialog>
#endif

class VersionCheck : public QObject {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(VersionCheck)
#ifdef USE_APPIMAGE_UPDATER_BRIDGE
		AppImageUpdaterBridge::AppImageDeltaRevisioner *m_Revisioner;
		AppImageUpdaterBridge::AppImageUpdaterDialog *m_UpdaterDialog;
#endif
#ifndef USE_APPIMAGE_UPDATER_BRIDGE
	public slots:
		void fetched(QByteArray data, QUrl url);
#endif
#if defined(USE_APPIMAGE_UPDATER_BRIDGE) && defined(Q_OS_LINUX)
	public slots:
		void handleUpdateCheck(bool,const QJsonObject&);
		void handleUpdateCheckError(short);

		void handleUpdateFinish(const QJsonObject&);
		void handleUpdateCancel();
		void handleUpdateError(const QString&,short);
#endif
	public:
		VersionCheck(bool autocheck, QObject *parent = NULL, bool focus = false);
};

#endif // MUMBLE_MUMBLE_VERSIONCHECK_H_
