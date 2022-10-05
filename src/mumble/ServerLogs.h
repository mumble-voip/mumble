// Copyright 2021-2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_SERVERLOGS_H_
#define MUMBLE_MUMBLE_SERVERLOGS_H_

#include <QDialog>

#include "ui_ServerLogs.h"

class ServerLogs : public QDialog, private Ui::ServerLogs {
	Q_OBJECT;
	Q_DISABLE_COPY(ServerLogs);

public:
	ServerLogs(QWidget *parent = nullptr);

	void updateFields();

public slots:
	void on_okButton_clicked();

private:
	static QString m_unknownStr;

	void updateTimeSpan();
	void updateLogs();
};

#endif // MUMBLE_MUMBLE_SERVERLOGS_H_
