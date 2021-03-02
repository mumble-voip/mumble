// Copyright 2020-2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "UserLocalNicknameDialog.h"
#include "ClientUser.h"
#include "Database.h"
#include "MainWindow.h"

#include <QtGui/QCloseEvent>
#include <QtWidgets/QPushButton>

// We define a global macro called 'g'. This can lead to issues when included code uses 'g' as a type or parameter name
// (like protobuf 3.7 does). As such, for now, we have to make this our last include.
#include "Global.h"

UserLocalNicknameDialog::UserLocalNicknameDialog(
	unsigned int sessionId,
	std::unordered_map< unsigned int, NicknameDialogPtr > &qmUserNicknameTracker)
	: QDialog(nullptr), m_clientSession(sessionId), m_qmUserNicknameTracker(qmUserNicknameTracker) {
	setupUi(this);

	qleUserLocalNickname->setFocus();

	qleUserLocalNickname->setAccessibleName(tr("User nickname"));

	ClientUser *user = ClientUser::get(sessionId);
	if (!user) {
		UserLocalNicknameDialog::close();
	} else {
		QString title = tr("Adjusting local nickname for %1").arg(user->qsName);
		setWindowTitle(title);
		qleUserLocalNickname->setText(user->getLocalNickname());
		m_originalNickname = qleUserLocalNickname->text();
	}

	if (g.mw && g.mw->windowFlags() & Qt::WindowStaysOnTopHint) {
		// If the main window is set to always be on top of other windows, we should make the
		// nickname dialog behave the same in order for it to not get hidden behind the main window.
		setWindowFlags(Qt::WindowStaysOnTopHint);
	}
}

void UserLocalNicknameDialog::closeEvent(QCloseEvent *event) {
	m_qmUserNicknameTracker.erase(m_clientSession);
	event->accept();
}

void UserLocalNicknameDialog::present(
	unsigned int sessionId,
	std::unordered_map< unsigned int, NicknameDialogPtr > &qmUserNicknameTracker) {
	if (qmUserNicknameTracker.find(sessionId) != qmUserNicknameTracker.end()) {
		qmUserNicknameTracker.at(sessionId)->show();
		qmUserNicknameTracker.at(sessionId)->raise();
	} else {
		// Make sure to use the custom deleter for QObjects that calls deleteLater() on them instead of using
		// delete directly as the latter can lead to segmentation faults.
		NicknameDialogPtr userNickname(new UserLocalNicknameDialog(sessionId, qmUserNicknameTracker), deleteQObject);
		userNickname->show();
		qmUserNicknameTracker.insert(std::make_pair(sessionId, std::move(userNickname)));
	}
}

void UserLocalNicknameDialog::on_qleUserLocalNickname_textChanged(const QString &text) {
	ClientUser *user = ClientUser::get(m_clientSession);
	if (user) {
		user->setLocalNickname(text);
	}
}

void UserLocalNicknameDialog::on_qbbUserLocalNickname_clicked(QAbstractButton *button) {
	if (button == qbbUserLocalNickname->button(QDialogButtonBox::Reset)) {
		qleUserLocalNickname->setText(m_originalNickname);
	} else if (button == qbbUserLocalNickname->button(QDialogButtonBox::Ok)) {
		ClientUser *user = ClientUser::get(m_clientSession);
		if (user) {
			if (!user->qsHash.isEmpty()) {
				g.db->setUserLocalNickname(user->qsHash, user->getLocalNickname());
			} else {
				g.mw->logChangeNotPermanent(QObject::tr("Local Nickname Adjustment..."), user);
			}
		}
		UserLocalNicknameDialog::close();
	} else if (button == qbbUserLocalNickname->button(QDialogButtonBox::Cancel)) {
		qleUserLocalNickname->setText(m_originalNickname);
		UserLocalNicknameDialog::close();
	}
}

void UserLocalNicknameDialog::reject() {
	m_qmUserNicknameTracker.erase(m_clientSession);
	UserLocalNicknameDialog::close();
}
