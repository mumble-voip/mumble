/* Copyright (C) 2005-2008, Thorvald Natvig <thorvald@natvig.com>

   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.
   - Neither the name of the Mumble Developers nor the names of its
     contributors may be used to endorse or promote products derived from this
     software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _CONFIGDIALOG_H
#define _CONFIGDIALOG_H

#include "mumble_pch.h"
#include "ui_ConfigDialog.h"
#include "Settings.h"

class ConfigWidget : public QWidget {
		Q_OBJECT
	protected:
		static void loadSlider(QSlider *, int);
		static void loadCheckBox(QAbstractButton *, bool);
		static void loadComboBox(QComboBox *, int);
	public:
		Settings &s;
		ConfigWidget(Settings &st);
		virtual QString title() const = 0;
		virtual QIcon icon() const;
	public slots:
		virtual void accept() const;
		virtual void save() const = 0;
		virtual void load(const Settings &r) = 0;
		virtual bool expert(bool) = 0;
};

typedef ConfigWidget *(*ConfigWidgetNew)(Settings &st);

class ConfigRegistrar {
		friend class ConfigDialog;
	protected:
		static QMap<int, ConfigWidgetNew> *c_qmNew;
	public:
		ConfigRegistrar(int priority, ConfigWidgetNew n);
};

class ConfigDialog : public QDialog, public Ui::ConfigDialog {
		Q_OBJECT;
	protected:
		QList<ConfigWidget *> widgets;
		void addPage(ConfigWidget *aw);
		Settings s;
	public:
		ConfigDialog(QWidget *p = NULL);
	public slots:
		void on_qlwIcons_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
		void on_pageButtonBox_clicked(QAbstractButton *);
		void on_dialogButtonBox_clicked(QAbstractButton *);
		void on_qcbExpert_clicked(bool);
		void apply();
		void accept();
};

#else
class ConfigWidget;
class ConfigDialog;
#endif
