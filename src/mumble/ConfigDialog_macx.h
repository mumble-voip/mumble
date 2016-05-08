// Copyright 2005-2016 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_CONFIGDIALOG_MACX_H_
#define MUMBLE_MUMBLE_CONFIGDIALOG_MACX_H_

#include "mumble_pch.hpp"
#include "ui_ConfigDialog.h"
#include "ConfigWidget.h"
#include "Settings.h"

class ConfigDialogMac : public QDialog, public Ui::ConfigDialog {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(ConfigDialogMac)
	protected:
		QHash<ConfigWidget *, QWidget *> qhPages;
		QMap<unsigned int, ConfigWidget *> qmWidgets;
		QMap<QListWidgetItem *, ConfigWidget *> qmIconWidgets;
		void addPage(ConfigWidget *aw, unsigned int idx);
		Settings s;

	public:
		ConfigDialogMac(QWidget *p = NULL);
		~ConfigDialogMac() Q_DECL_OVERRIDE;
	protected:
		void setupMacToolbar(bool expert);
		void removeMacToolbar();
	public:
		void updateExpert(bool expert);
		void on_widgetSelected(ConfigWidget *);
	public slots:
		void delayedInit();
		void on_pageButtonBox_clicked(QAbstractButton *);
		void on_dialogButtonBox_clicked(QAbstractButton *);
		void on_qlwIcons_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
		void on_qcbExpert_clicked(bool);
		void apply();
		void accept() Q_DECL_OVERRIDE;
};

#else
class ConfigDialogMac;
#endif
