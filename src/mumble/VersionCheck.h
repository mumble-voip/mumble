// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_VERSIONCHECK_H_
#define MUMBLE_MUMBLE_VERSIONCHECK_H_

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QUrl>

class VersionCheck : public QObject {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(VersionCheck)
	public slots:
		void fetched(QByteArray data, QUrl url);
	public:
		VersionCheck(bool autocheck, QObject *parent = NULL, bool focus = false);
};

#endif
