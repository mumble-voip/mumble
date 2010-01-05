/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>

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

#include "AudioInput.h"
#include "AudioOutput.h"
#include "LookConfig.h"
#include "Global.h"
#include "MainWindow.h"

static ConfigWidget *LookConfigNew(Settings &st) {
	return new LookConfig(st);
}

static ConfigRegistrar registrar(1100, LookConfigNew);

LookConfig::LookConfig(Settings &st) : ConfigWidget(st) {
	setupUi(this);

	qcbLanguage->addItem(tr("System default"));
	QDir d(QLatin1String(":"),QLatin1String("mumble_*.qm"),QDir::Name,QDir::Files);
	QStringList langs;
	foreach(QString key, d.entryList()) {
		QString cc = key.mid(7,2);
		langs << cc;
		qcbLanguage->addItem(cc);
	}

	QStringList styles = QStyleFactory::keys();
	styles.sort();
	qcbStyle->addItem(tr("System default"));
	foreach(QString key, styles) {
		qcbStyle->addItem(key);
	}
	qcbExpand->addItem(tr("None"), Settings::NoChannels);
	qcbExpand->addItem(tr("Only with players"), Settings::ChannelsWithPlayers);
	qcbExpand->addItem(tr("All"), Settings::AllChannels);

	qcbChannelDrag->insertItem(Settings::Ask, tr("Ask"), Settings::Ask);
	qcbChannelDrag->insertItem(Settings::DoNothing, tr("Do Nothing"), Settings::DoNothing);
	qcbChannelDrag->insertItem(Settings::Move, tr("Move"), Settings::Move);
}

QString LookConfig::title() const {
	return tr("User Interface");
}

QIcon LookConfig::icon() const {
	return QIcon(QLatin1String("skin:config_ui.png"));
}

void LookConfig::load(const Settings &r) {
	loadComboBox(qcbLanguage, 0);
	loadComboBox(qcbStyle, 0);
	loadComboBox(qcbChannelDrag, 0);

	for (int i=0;i<qcbLanguage->count();i++) {
		if (qcbLanguage->itemText(i) == r.qsLanguage) {
			loadComboBox(qcbLanguage, i);
			break;
		}
	}
	for (int i=0;i<qcbStyle->count();i++) {
		if (qcbStyle->itemText(i) == r.qsStyle) {
			loadComboBox(qcbStyle, i);
			break;
		}
	}

	qleCSS->setText(r.qsSkin);
	loadComboBox(qcbExpand, r.ceExpand);
	loadComboBox(qcbChannelDrag, r.ceChannelDrag);
	loadCheckBox(qcbPlayersTop, r.bPlayerTop);
	loadCheckBox(qcbAlwaysOnTop, r.bAlwaysOnTop);
	loadCheckBox(qcbAskOnQuit, r.bAskOnQuit);
	loadCheckBox(qcbHideTray, r.bHideTray);
}

void LookConfig::save() const {
	if (qcbLanguage->currentIndex() == 0)
		s.qsLanguage = QString();
	else
		s.qsLanguage = qcbLanguage->currentText();

	if (qcbStyle->currentIndex() == 0)
		s.qsStyle = QString();
	else
		s.qsStyle = qcbStyle->currentText();

	if (qleCSS->text().isEmpty())
		s.qsSkin = QString();
	else
		s.qsSkin = qleCSS->text();
	s.ceExpand=static_cast<Settings::ChannelExpand>(qcbExpand->currentIndex());
	s.ceChannelDrag=static_cast<Settings::ChannelDrag>(qcbChannelDrag->currentIndex());
	s.bPlayerTop=qcbPlayersTop->isChecked();
	s.bAlwaysOnTop = qcbAlwaysOnTop->isChecked();
	s.bAskOnQuit = qcbAskOnQuit->isChecked();
	s.bHideTray = qcbHideTray->isChecked();
}

void LookConfig::accept() const {
	if (! s.qsStyle.isEmpty()) {
		qApp->setStyle(s.qsStyle);
	}
	if (s.qsSkin.isEmpty()) {
		qApp->setStyleSheet(QString());
		g.mw->qteLog->document()->setDefaultStyleSheet(QString());
	} else {
		QFile file(s.qsSkin);
		file.open(QFile::ReadOnly);
		QString sheet = QLatin1String(file.readAll());
		if (! sheet.isEmpty()) {
			qApp->setStyleSheet(sheet);
			g.mw->qteLog->document()->setDefaultStyleSheet(sheet);
		}
	}
}

bool LookConfig::expert(bool b) {
	qcbExpand->setVisible(b);
	qliExpand->setVisible(b);
	qcbPlayersTop->setVisible(b);
	qcbStyle->setVisible(b);
	qliStyle->setVisible(b);
	return true;
}

void LookConfig::on_qpbSkinFile_clicked(bool) {
	QString file = QFileDialog::getOpenFileName(this, tr("Choose skin file"), QString(), QLatin1String("*.qss"));
	if (! file.isEmpty()) {
		qleCSS->setText(file);
	}
}
