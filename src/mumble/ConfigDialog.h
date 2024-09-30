// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_CONFIGDIALOG_H_
#define MUMBLE_MUMBLE_CONFIGDIALOG_H_

#include "ConfigWidget.h"
#include "Settings.h"

#include "ui_ConfigDialog.h"

#include <QtCore/QMutex>

class ConfigDialog : public QDialog, public Ui::ConfigDialog {
private:
	Q_OBJECT
	Q_DISABLE_COPY(ConfigDialog)

	void updateTabOrder();

protected:
	static QMutex s_existingWidgetsMutex;
	static QHash< QString, ConfigWidget * > s_existingWidgets;
	QHash< ConfigWidget *, QWidget * > qhPages;
	QMap< unsigned int, ConfigWidget * > qmWidgets;
	QMap< QListWidgetItem *, ConfigWidget * > qmIconWidgets;
	void updateListView();
	void addPage(ConfigWidget *aw, unsigned int idx);
	Settings s;

public:
	ConfigDialog(QWidget *p = nullptr);
	~ConfigDialog() Q_DECL_OVERRIDE;

	/// @returns The pointer to the existing ConfigWidget with the given name or nullptr,
	/// 	if no such widget exists.
	static ConfigWidget *getConfigWidget(const QString &name);

signals:
	/// Emitted whenever the settings dialog has been accepted. For potential slots this
	/// means that the settings potentially have changed.
	void settingsAccepted();
public slots:
	void on_pageButtonBox_clicked(QAbstractButton *);
	void on_dialogButtonBox_clicked(QAbstractButton *);
	void on_qlwIcons_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
	void apply();
	void accept() Q_DECL_OVERRIDE;
};

#endif
