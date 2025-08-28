#include "PasswordDialog.h"
#include <QObject>
#include "Global.h"

PasswordDialog::PasswordDialog(QWidget *parent) : QDialog(parent) {
	setupUi(this);
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
	dialog.qcbSavePassword->setChecked(save ? *save : false);

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
