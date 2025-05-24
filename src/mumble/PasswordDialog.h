#ifndef PASSWORDDIALOG_H
#define PASSWORDDIALOG_H

#include "ui_PasswordDialog.h"

class PasswordDialog : public QDialog, private Ui::PasswordDialog {
	Q_OBJECT
	Q_DISABLE_COPY(PasswordDialog)
public:
	static QString getPassword(QWidget *parent, const QString &title, const QString &label,
							   QString defaultPassword = QString(), bool *ok = nullptr, bool *save = nullptr,
							   Qt::WindowFlags flags = Qt::WindowFlags());

private:
	PasswordDialog(QWidget *parent = nullptr);
	QString getEnteredPassword() const;
	bool getSavePassword() const;
};

#endif // PASSWORDDIALOG_H
