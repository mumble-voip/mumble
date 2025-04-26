// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_GLOBALSHORTCUTKEYS_H
#define MUMBLE_MUMBLE_GLOBALSHORTCUTKEYS_H

#include <memory>

#include <QtWidgets/QDialog>

namespace Ui {
class GlobalShortcutButtons;
}

class GlobalShortcutButtons : public QDialog {
private:
	Q_OBJECT
	Q_DISABLE_COPY(GlobalShortcutButtons)
	Q_PROPERTY(QList< QVariant > buttons READ buttons WRITE setButtons USER true)

protected:
	QList< QVariant > m_previousButtons;
	std::unique_ptr< Ui::GlobalShortcutButtons > m_ui;

	void addItem(const QVariant &button);
	void removeItem();
	void toggleCapture(const bool enabled);
	void updateButtons(const bool last);

	virtual bool eventFilter(QObject *, QEvent *event);

public:
	QList< QVariant > buttons();
	void setButtons(const QList< QVariant > &buttons);

	GlobalShortcutButtons(QWidget *p = nullptr);
	~GlobalShortcutButtons();
};

#endif
