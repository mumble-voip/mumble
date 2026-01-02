// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "widgets/BanDialog.h"
#include "ServerHandler.h"
#include "Version.h"
#include "Global.h"

#include <QObject>
#include <QPushButton>

BanDialog::BanDialog(ClientUser *user, QWidget *parent) : QDialog(parent), m_user(user->uiSession) {
	setupUi(this);

	setWindowTitle(tr("Banning user %1").arg(user->qsName));

	if (Global::get().sh->m_version < Version::fromComponents(1, 6, 0)) {
		banCertificate->setVisible(false);
		banIP->setVisible(false);
		banCertificate->setEnabled(false);
		banIP->setEnabled(false);
		banCertificate->setChecked(true);
		banIP->setChecked(true);
	} else {
		bool userHasCertificate = !user->qsHash.isEmpty();

		banCertificate->setEnabled(userHasCertificate);
		banIP->setEnabled(userHasCertificate);

		banCertificate->setChecked(userHasCertificate);
		banIP->setChecked(!userHasCertificate);

		QObject::connect(banCertificate, &QCheckBox::toggled, this, &BanDialog::validateInput);
		QObject::connect(banIP, &QCheckBox::toggled, this, &BanDialog::validateInput);
	}

	QObject::connect(cancelButton, &QPushButton::clicked, this, &QDialog::close);
	QObject::connect(okButton, &QPushButton::clicked, this, &BanDialog::execute);
}

void BanDialog::validateInput() {
	okButton->setEnabled(banCertificate->isChecked() || banIP->isChecked());
}

void BanDialog::execute() {
	if (!ClientUser::get(m_user)) {
		// Session no longer valid. Aborting...
		return;
	}

	Global::get().sh->banUser(m_user, banReason->text(), banCertificate->isChecked(), banIP->isChecked());
	close();
}
