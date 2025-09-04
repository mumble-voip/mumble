// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_WIDGETS_FAILED_CONNECTION_DIALOG_H_
#define MUMBLE_MUMBLE_WIDGETS_FAILED_CONNECTION_DIALOG_H_

#include <QDialog>
#include <QString>
#include <QWidget>

#include "ui_FailedConnectionDialog.h"

enum class ConnectionFailType {
	InvalidUsername,
	UsernameAlreadyInUse,
	AuthenticationFailure,
	InvalidServerPassword,
};

struct ConnectDetails {
	QString host;
	unsigned short port = 0;
	QString username;
	QString password;
};

class FailedConnectionDialog : public QDialog, protected Ui::FailedConnectionDialog {
public:
	FailedConnectionDialog(ConnectDetails details, ConnectionFailType type, QWidget *parent = nullptr);

private:
	ConnectDetails m_details;

	void connectSignals();

private slots:
	void initiateReconnect();
};

#endif // MUMBLE_MUMBLE_WIDGETS_FAILED_CONNECTION_DIALOG_H_
