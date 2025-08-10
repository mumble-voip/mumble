// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "widgets/FailedConnectionDialog.h"
#include "Database.h"
#include "MainWindow.h"
#include "ServerHandler.h"
#include "Global.h"

#include <QAction>
#include <QLineEdit>
#include <QObject>
#include <QPushButton>

FailedConnectionDialog::FailedConnectionDialog(ConnectDetails details, ConnectionFailType type, QWidget *parent)
	: QDialog(parent), m_details(std::move(details)) {
	setupUi(this);

	invalidUserNameInput->setText(m_details.username);
	usedUsernameInput->setText(m_details.username);

	userPasswordInput->setText(m_details.password);
	serverPasswordInput->setText(m_details.password);

	connectSignals();

	switch (type) {
		case ConnectionFailType::AuthenticationFailure:
			pageStack->setCurrentWidget(authenticationFailed);
			wrongPasswordMsg->setVisible(!m_details.password.isEmpty());
			wrongCertificateMsg->setVisible(m_details.password.isEmpty());
			break;
		case ConnectionFailType::InvalidServerPassword:
			pageStack->setCurrentWidget(wrongServerPassword);
			break;
		case ConnectionFailType::InvalidUsername:
			pageStack->setCurrentWidget(invalidUsername);
			usernameAlreadyUsedMsg->setVisible(false);
			break;
		case ConnectionFailType::UsernameAlreadyInUse:
			pageStack->setCurrentWidget(invalidUsername);
			invalidUsernameMsg->setVisible(false);
			break;
	}
}

void FailedConnectionDialog::connectSignals() {
	QObject::connect(reconnectButton, &QPushButton::clicked, this, &FailedConnectionDialog::initiateReconnect);

	QObject::connect(reconnectButton, &QPushButton::clicked, this, &QDialog::close);
	QObject::connect(cancelButton, &QPushButton::clicked, this, &QDialog::close);

	// auth failure page
	QObject::connect(usedUsernameInput, &QLineEdit::textChanged,
					 [this](QString name) { m_details.username = std::move(name); });
	QObject::connect(userPasswordInput, &QLineEdit::textChanged,
					 [this](QString pw) { m_details.password = std::move(pw); });
	QObject::connect(changeCertButton, &QPushButton::clicked, Global::get().mw->qaConfigCert, &QAction::triggered);

	// invalid server password page
	QObject::connect(serverPasswordInput, &QLineEdit::textChanged,
					 [this](QString pw) { m_details.password = std::move(pw); });

	// invalid username page
	QObject::connect(invalidUserNameInput, &QLineEdit::textChanged,
					 [this](QString name) { m_details.username = std::move(name); });
}

void FailedConnectionDialog::initiateReconnect() {
	if (Global::get().db->isFavorite(m_details.host, m_details.port)) {
		Global::get().db->setPassword(m_details.host, m_details.port, m_details.username, m_details.password);
	}

	Global::get().sh->setConnectionInfo(m_details.host, m_details.port, m_details.username, m_details.password);

	// Reuse logic implemented for automatic reconnect attempts to actually perform the connection
	Global::get().mw->on_Reconnect_timeout();
}
