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

enum class CantSavePasswordReason { None, SuppressIdentity };

// This structure is used as a Result type.
// Preferably replaced with Expected type in the future.
// As of c++17, expected isn't available
struct CanSavePassword {
	bool canSave;
	CantSavePasswordReason reason; // Only useful when cansave = false

	static CanSavePassword Yes() { return { true, CantSavePasswordReason::None }; };
	static CanSavePassword No(CantSavePasswordReason r) { return { false, r }; };
};

#endif // PASSWORDDIALOG_H
