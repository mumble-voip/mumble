#ifndef PASSWORDDIALOG_H
#define PASSWORDDIALOG_H

#include <QDialog>

namespace Ui {
class PasswordDialog;
}

class PasswordDialog : public QDialog {
	Q_OBJECT

public:
	explicit PasswordDialog(QWidget *parent = nullptr);
	~PasswordDialog();
	QString getEnteredPassword() const;
	bool getSavePassword() const;
	static QString getPassword(QWidget *parent, const QString &title, const QString &label,
							   QString defaultPassword = QString(), bool *ok = nullptr, bool *save = nullptr,
							   Qt::WindowFlags flags = Qt::WindowFlags());

private:
	Ui::PasswordDialog *ui;
};

#endif // PASSWORDDIALOG_H
