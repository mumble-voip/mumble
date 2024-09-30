// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "UserLocalNicknameDialog.h"
#include "ClientUser.h"
#include "Database.h"
#include "MainWindow.h"
#include "Global.h"

#include <QtGui/QCloseEvent>
#include <QtWidgets/QPushButton>

UserLocalNicknameDialog::UserLocalNicknameDialog(
	unsigned int sessionId,
	std::unordered_map< unsigned int, qt_unique_ptr< UserLocalNicknameDialog > > &qmUserNicknameTracker)
	: QDialog(nullptr), m_clientSession(sessionId), m_qmUserNicknameTracker(qmUserNicknameTracker) {
	setupUi(this);

	qleUserLocalNickname->setFocus();

	ClientUser *user = ClientUser::get(sessionId);
	if (!user) {
		UserLocalNicknameDialog::close();
	} else {
		QString title = tr("Adjusting local nickname for %1").arg(user->qsName);
		setWindowTitle(title);
		qleUserLocalNickname->setText(user->getLocalNickname());
		m_originalNickname = qleUserLocalNickname->text();
	}

	if (Global::get().mw && Global::get().mw->windowFlags() & Qt::WindowStaysOnTopHint) {
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
	std::unordered_map< unsigned int, qt_unique_ptr< UserLocalNicknameDialog > > &qmUserNicknameTracker) {
	if (qmUserNicknameTracker.find(sessionId) != qmUserNicknameTracker.end()) {
		qmUserNicknameTracker.at(sessionId)->show();
		qmUserNicknameTracker.at(sessionId)->raise();
	} else {
		qt_unique_ptr< UserLocalNicknameDialog > userNickname =
			make_qt_unique< UserLocalNicknameDialog >(sessionId, qmUserNicknameTracker);
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
				Global::get().db->setUserLocalNickname(user->qsHash, user->getLocalNickname());
			} else {
				Global::get().mw->logChangeNotPermanent(QObject::tr("Local Nickname Adjustment..."), user);
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
