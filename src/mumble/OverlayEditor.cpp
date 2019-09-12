// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "OverlayEditor.h"

#include "OverlayClient.h"
#include "OverlayText.h"
#include "User.h"
#include "Channel.h"
#include "Message.h"
#include "Database.h"
#include "NetworkConfig.h"
#include "ServerHandler.h"
#include "MainWindow.h"
#include "GlobalShortcut.h"
#include "Utils.h"

#include <QtWidgets/QGraphicsProxyWidget>

// We define a global macro called 'g'. This can lead to issues when included code uses 'g' as a type or parameter name (like protobuf 3.7 does). As such, for now, we have to make this our last include.
#include "Global.h"

OverlayEditor::OverlayEditor(QWidget *p, QGraphicsItem *qgi, OverlaySettings *osptr) :
		QDialog(p),
		qgiPromote(qgi),
		oes(g.s.os) {
	setupUi(this);

	os = osptr ? osptr : &g.s.os;

	connect(qdbbBox->button(QDialogButtonBox::Apply), SIGNAL(clicked()), this, SLOT(apply()));
	connect(qdbbBox->button(QDialogButtonBox::Reset), SIGNAL(clicked()), this, SLOT(reset()));

	QGraphicsProxyWidget *qgpw = graphicsProxyWidget();
	if (qgpw) {
		qgpw->setFlag(QGraphicsItem::ItemIgnoresParentOpacity);
		if (g.ocIntercept) {
			qgpw->setPos(iroundf(static_cast<float>(g.ocIntercept->uiWidth) / 16.0f + 0.5f),
			             iroundf(static_cast<float>(g.ocIntercept->uiHeight) / 16.0f + 0.5f));
			qgpw->resize(iroundf(static_cast<float>(g.ocIntercept->uiWidth) * 14.0f / 16.0f + 0.5f),
			             iroundf(static_cast<float>(g.ocIntercept->uiHeight) * 14.0f / 16.0f + 0.5f));
		}
	}

	qgvView->setScene(&oes);

	reset();
}

OverlayEditor::~OverlayEditor() {
	QGraphicsProxyWidget *qgpw = g.mw->graphicsProxyWidget();
	if (qgpw)
		qgpw->setOpacity(0.9f);
	if (qgiPromote)
		qgiPromote->setZValue(-1.0f);
}

void OverlayEditor::enterEvent(QEvent *e) {
	QGraphicsProxyWidget *qgpw = g.mw->graphicsProxyWidget();
	if (qgpw)
		qgpw->setOpacity(0.9f);

	qgpw = graphicsProxyWidget();
	if (qgpw)
		qgpw->setOpacity(1.0f);

	if (qgiPromote)
		qgiPromote->setZValue(-1.0f);

	QDialog::enterEvent(e);
}

void OverlayEditor::leaveEvent(QEvent *e) {
	QGraphicsProxyWidget *qgpw = g.mw->graphicsProxyWidget();
	if (qgpw)
		qgpw->setOpacity(0.3f);

	qgpw = graphicsProxyWidget();
	if (qgpw)
		qgpw->setOpacity(0.3f);

	if (qgiPromote)
		qgiPromote->setZValue(1.0f);

	QDialog::leaveEvent(e);
}

void OverlayEditor::reset() {
	oes.os = *os;
	oes.resync();

	qcbAvatar->setChecked(oes.os.bAvatar);
	qcbUser->setChecked(oes.os.bUserName);
	qcbChannel->setChecked(oes.os.bChannel);
	qcbMutedDeafened->setChecked(oes.os.bMutedDeafened);
	qcbBox->setChecked(oes.os.bBox);
}

void OverlayEditor::apply() {
	*os = oes.os;
	emit applySettings();
}

void OverlayEditor::accept() {
	apply();
	QDialog::accept();
}

void OverlayEditor::on_qrbPassive_clicked() {
	oes.tsColor = Settings::Passive;
	oes.resync();
}

void OverlayEditor::on_qrbTalking_clicked() {
	oes.tsColor = Settings::Talking;
	oes.resync();
}

void OverlayEditor::on_qrbWhisper_clicked() {
	oes.tsColor = Settings::Whispering;
	oes.resync();
}

void OverlayEditor::on_qrbShout_clicked() {
	oes.tsColor = Settings::Shouting;
	oes.resync();
}

void OverlayEditor::on_qcbAvatar_clicked() {
	oes.os.bAvatar = qcbAvatar->isChecked();
	if (! oes.os.bAvatar && ! oes.os.bUserName) {
		qcbUser->setChecked(true);
		oes.os.bUserName = true;
		oes.updateUserName();
	}
	oes.updateAvatar();
}

void OverlayEditor::on_qcbUser_clicked() {
	oes.os.bUserName = qcbUser->isChecked();
	if (! oes.os.bAvatar && ! oes.os.bUserName) {
		qcbAvatar->setChecked(true);
		oes.os.bAvatar = true;
		oes.updateAvatar();
	}
	oes.updateUserName();
}

void OverlayEditor::on_qcbChannel_clicked() {
	oes.os.bChannel = qcbChannel->isChecked();
	oes.updateChannel();
}

void OverlayEditor::on_qcbMutedDeafened_clicked() {
	oes.os.bMutedDeafened = qcbMutedDeafened->isChecked();
	oes.updateMuted();
}

void OverlayEditor::on_qcbBox_clicked() {
	oes.os.bBox = qcbBox->isChecked();
	oes.moveBox();
}

void OverlayEditor::on_qsZoom_valueChanged(int zoom) {
	oes.uiZoom = zoom;
	oes.resync();
}
