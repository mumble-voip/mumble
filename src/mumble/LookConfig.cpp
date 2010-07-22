/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>
   Copyright (C) 2009, Stefan Hacker <dd0t@users.sourceforge.net>

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

#ifndef Q_OS_MAC
	if (! QSystemTrayIcon::isSystemTrayAvailable())
#endif
		qgbTray->hide();

	qcbLanguage->addItem(tr("System default"));
	QDir d(QLatin1String(":"),QLatin1String("mumble_*.qm"),QDir::Name,QDir::Files);
	QStringList langs;
	foreach(const QString &key, d.entryList()) {
		QString cc = key.mid(7,key.indexOf(QLatin1Char('.'))-7);
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
	qcbExpand->addItem(tr("Only with users"), Settings::ChannelsWithUsers);
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

	// Load Layout checkbox state
	switch (r.wlWindowLayout) {
		case Settings::LayoutClassic:
			qrbLClassic->setChecked(true);
			break;
		case Settings::LayoutStacked:
			qrbLStacked->setChecked(true);
			break;
		case Settings::LayoutHybrid:
			qrbLHybrid->setChecked(true);
			break;
		case Settings::LayoutCustom:
		default:
			s.wlWindowLayout = Settings::LayoutCustom;
			qrbLCustom->setChecked(true);
			break;
	}


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

	loadComboBox(qcbAlwaysOnTop, r.aotbAlwaysOnTop);

	qleCSS->setText(r.qsSkin);
	loadComboBox(qcbExpand, r.ceExpand);
	loadComboBox(qcbChannelDrag, r.ceChannelDrag);
	loadCheckBox(qcbUsersTop, r.bUserTop);
	loadCheckBox(qcbAskOnQuit, r.bAskOnQuit);
	loadCheckBox(qcbHideTray, r.bHideInTray);
	loadCheckBox(qcbStateInTray, r.bStateInTray);
	loadCheckBox(qcbShowUserCount, r.bShowUserCount);
	loadCheckBox(qcbShowContextMenuInMenuBar, r.bShowContextMenuInMenuBar);
	loadCheckBox(qcbHighContrast, r.bHighContrast);
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

	// Save Layout radioboxes state
	if (qrbLClassic->isChecked()) {
		s.wlWindowLayout = Settings::LayoutClassic;
	} else if (qrbLStacked->isChecked()) {
		s.wlWindowLayout = Settings::LayoutStacked;
	} else if (qrbLHybrid->isChecked()) {
		s.wlWindowLayout = Settings::LayoutHybrid;
	} else {
		s.wlWindowLayout = Settings::LayoutCustom;
	}

	s.ceExpand=static_cast<Settings::ChannelExpand>(qcbExpand->currentIndex());
	s.ceChannelDrag=static_cast<Settings::ChannelDrag>(qcbChannelDrag->currentIndex());
	s.bUserTop=qcbUsersTop->isChecked();
	s.aotbAlwaysOnTop = static_cast<Settings::AlwaysOnTopBehaviour>(qcbAlwaysOnTop->currentIndex());
	s.bAskOnQuit = qcbAskOnQuit->isChecked();
	s.bHideInTray = qcbHideTray->isChecked();
	s.bStateInTray = qcbStateInTray->isChecked();
	s.bShowUserCount = qcbShowUserCount->isChecked();
	s.bShowContextMenuInMenuBar = qcbShowContextMenuInMenuBar->isChecked();
	s.bHighContrast = qcbHighContrast->isChecked();
}

void LookConfig::accept() const {
	if (! s.qsStyle.isEmpty() && g.qsCurrentStyle != s.qsStyle) {
		qApp->setStyle(s.qsStyle);
		g.qsCurrentStyle = s.qsStyle;
	}
	if (s.qsSkin.isEmpty()) {
		if (qApp->styleSheet() != MainWindow::defaultStyleSheet) {
			qApp->setStyleSheet(MainWindow::defaultStyleSheet);
			g.mw->qteLog->document()->setDefaultStyleSheet(qApp->styleSheet());
		}
	} else {
		QFile file(s.qsSkin);
		file.open(QFile::ReadOnly);
		QString sheet = QLatin1String(file.readAll());
		if (! sheet.isEmpty() && (sheet != qApp->styleSheet())) {
			qApp->setStyleSheet(sheet);
			g.mw->qteLog->document()->setDefaultStyleSheet(sheet);
		}
	}
}

bool LookConfig::expert(bool b) {
	qcbExpand->setVisible(b);
	qliExpand->setVisible(b);
	qcbUsersTop->setVisible(b);
	qcbStyle->setVisible(b);
	qliStyle->setVisible(b);
	qcbStateInTray->setVisible(b);
	qcbShowContextMenuInMenuBar->setVisible(b);
	return true;
}

void LookConfig::on_qpbSkinFile_clicked(bool) {
	QString file = QFileDialog::getOpenFileName(this, tr("Choose skin file"), QString(), QLatin1String("*.qss"));
	if (! file.isEmpty()) {
		qleCSS->setText(file);
	}
}
