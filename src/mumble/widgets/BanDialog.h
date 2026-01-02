// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_WIDGETS_BAN_DIALOG_H_
#define MUMBLE_MUMBLE_WIDGETS_BAN_DIALOG_H_

#include <QDialog>

#include "ClientUser.h"

#include "ui_BanDialog.h"

class BanDialog : public QDialog, protected Ui::BanDialog {
public:
	BanDialog(ClientUser *user, QWidget *parent = nullptr);

private:
	const unsigned int m_user;

	void validateInput();
	void execute();
};

#endif // MUMBLE_MUMBLE_WIDGETS_BAN_DIALOG_H_
