// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_BANEDITOR_H_
#define MUMBLE_MUMBLE_BANEDITOR_H_

#include "Ban.h"

#include "ui_BanEditor.h"

namespace MumbleProto {
class BanList;
}

class BanEditor : public QDialog, public Ui::BanEditor {
private:
	Q_OBJECT
	Q_DISABLE_COPY(BanEditor)
protected:
	QList< Ban > qlBans;

	int maskDefaultValue;

	Ban toBan(bool &);

public:
	BanEditor(const MumbleProto::BanList &msbl, QWidget *p = nullptr);
public slots:
	void accept();
	void on_qlwBans_currentRowChanged();
	void on_qpbAdd_clicked();
	void on_qpbUpdate_clicked();
	void on_qpbRemove_clicked();
	void refreshBanList();
	void on_qdteEnd_editingFinished();
	void on_qpbClear_clicked();
private slots:
	void on_qleHash_textChanged(QString);
	void on_qleSearch_textChanged(const QString &match);
	void on_qleReason_textChanged(QString);
	void on_qleIP_textChanged(QString);
	void on_qleUser_textChanged(QString);
};

#endif
