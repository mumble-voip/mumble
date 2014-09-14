/* Copyright (C) 2005-2011, Thorvald Natvig <thorvald@natvig.com>

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

#include "mumble_pch.hpp"

#include "OverlayEditor.h"

#include "OverlayClient.h"
#include "OverlayText.h"
#include "User.h"
#include "Channel.h"
#include "Global.h"
#include "Message.h"
#include "Database.h"
#include "NetworkConfig.h"
#include "ServerHandler.h"
#include "MainWindow.h"
#include "GlobalShortcut.h"

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
			qgpw->setPos(iroundf(g.ocIntercept->uiWidth / 16.0f + 0.5f), iroundf(g.ocIntercept->uiHeight / 16.0f + 0.5f));
			qgpw->resize(iroundf(g.ocIntercept->uiWidth * 14.0f / 16.0f + 0.5f), iroundf(g.ocIntercept->uiHeight * 14.0f / 16.0f + 0.5f));
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

void OverlayEditor::enterEvent(QEvent *event) {
	QGraphicsProxyWidget *qgpw = g.mw->graphicsProxyWidget();
	if (qgpw)
		qgpw->setOpacity(0.9f);

	qgpw = graphicsProxyWidget();
	if (qgpw)
		qgpw->setOpacity(1.0f);

	if (qgiPromote)
		qgiPromote->setZValue(-1.0f);

	QDialog::enterEvent(event);
}

void OverlayEditor::leaveEvent(QEvent *event) {
	QGraphicsProxyWidget *qgpw = g.mw->graphicsProxyWidget();
	if (qgpw)
		qgpw->setOpacity(0.3f);

	qgpw = graphicsProxyWidget();
	if (qgpw)
		qgpw->setOpacity(0.3f);

	if (qgiPromote)
		qgiPromote->setZValue(1.0f);

	QDialog::leaveEvent(event);
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
