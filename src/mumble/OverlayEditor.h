// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_OVERLAYEDITOR_H_
#define MUMBLE_MUMBLE_OVERLAYEDITOR_H_

#include "OverlayEditorScene.h"
#include "ui_OverlayEditor.h"

struct OverlaySettings;

class OverlayEditor : public QDialog, public Ui::OverlayEditor {
private:
	Q_OBJECT
	Q_DISABLE_COPY(OverlayEditor)
protected:
	QGraphicsItem *qgiPromote;
	OverlayEditorScene oes;
	OverlaySettings *os;

	void enterEvent(QEnterEvent *e) Q_DECL_OVERRIDE;
	void leaveEvent(QEvent *e) Q_DECL_OVERRIDE;

public:
	OverlayEditor(QWidget *p = nullptr, QGraphicsItem *qgi = nullptr, OverlaySettings *osptr = nullptr);
	~OverlayEditor() Q_DECL_OVERRIDE;
signals:
	void applySettings();
public slots:
	void reset();
	void apply();
	void accept() Q_DECL_OVERRIDE;

	void on_qrbPassive_clicked();
	void on_qrbTalking_clicked();
	void on_qrbWhisper_clicked();
	void on_qrbShout_clicked();

	void on_qcbAvatar_clicked();
	void on_qcbUser_clicked();
	void on_qcbChannel_clicked();
	void on_qcbMutedDeafened_clicked();
	void on_qcbBox_clicked();

	void on_qsZoom_valueChanged(int);
};

#endif
