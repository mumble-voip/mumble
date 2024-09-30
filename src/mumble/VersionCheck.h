// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_VERSIONCHECK_H_
#define MUMBLE_MUMBLE_VERSIONCHECK_H_

#include <QFutureWatcher>
#include <QtCore/QByteArray>
#include <QtCore/QObject>
#include <QtCore/QUrl>

class VersionCheck : public QObject {
private:
	Q_OBJECT
	Q_DISABLE_COPY(VersionCheck)

	QFutureWatcher< void > m_preparationWatcher;
	QUrl m_requestURL;
protected slots:
	void performRequest();
public slots:
	void fetched(QByteArray data, QUrl url);

public:
	VersionCheck(bool autocheck, QObject *parent = nullptr, bool focus = false);
};

#endif
