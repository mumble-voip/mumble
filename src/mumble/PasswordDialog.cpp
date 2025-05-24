#include "PasswordDialog.h"
#include <QObject>
#include "Global.h"

PasswordDialog::PasswordDialog(QWidget *parent) : QDialog(parent) {
	setupUi(this);
}

CanSavePassword canSavePassword() {
	if (Global::get().s.bSuppressIdentity) {
		return CanSavePassword::No(CantSavePasswordReason::SuppressIdentity);
	};
	return CanSavePassword::Yes();
}


QString toString(CantSavePasswordReason err) {
	switch (err) {
		case CantSavePasswordReason::SuppressIdentity:
			return QObject::tr("Suppress certificate and password storage setting enabled");
		default:
			return QObject::tr("Unknown error");
	}
}

QString PasswordDialog::getEnteredPassword() const {
	return qlePassword->text();
}

bool PasswordDialog::getSavePassword() const {
	return qcbSavePassword->isChecked();
}

QString PasswordDialog::getPassword(QWidget *parent, const QString &title, const QString &label,
									QString defaultPassword, bool *ok, bool *save, Qt::WindowFlags flags) {
	PasswordDialog dialog(parent);
	Qt::WindowFlags neededflags = flags;
	// Ensure the window type is set to Dialog
	if (!(neededflags & Qt::WindowType_Mask)) {
		neededflags |= Qt::Dialog;
	}
	dialog.setWindowFlags(neededflags);
	dialog.setWindowTitle(title);
	dialog.qlMessage->setText(label);
	dialog.qlePassword->setText(defaultPassword);
	CanSavePassword checkResult = canSavePassword();
	if (checkResult.canSave) {
		dialog.qcbSavePassword->setChecked(save ? *save : false);
	} else {
		dialog.qcbSavePassword->setEnabled(false);
		QString new_tooltip = QObject::tr("Password can't be saved: ") + toString(checkResult.reason);
		dialog.qcbSavePassword->setToolTip(new_tooltip);
	}

	int result = dialog.exec();
	if (ok) {
		*ok = (result == QDialog::Accepted);
	}

	if (save) {
		*save = dialog.getSavePassword();
	}

	if (result == QDialog::Accepted) {
		return dialog.getEnteredPassword();
	}

	return QString();
};
