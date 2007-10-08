	/* Copyright (C) 2005-2007, Thorvald Natvig <thorvald@natvig.com>

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

static ConfigRegistrar registrar(11, LookConfigNew);

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
}

QString LookConfig::title() const {
	return tr("User Interface");
}

QIcon LookConfig::icon() const {
	return QIcon(QLatin1String("skin:config_ui.png"));
}

void LookConfig::load(const Settings &r) {
	qcbLanguage->setCurrentIndex(0);
	qcbStyle->setCurrentIndex(0);

	for(int i=0;i<qcbLanguage->count();i++) {
		if (qcbLanguage->itemText(i) == r.qsLanguage) {
			qcbLanguage->setCurrentIndex(i);
			break;
		}
	}
	for(int i=0;i<qcbStyle->count();i++) {
		if (qcbStyle->itemText(i) == r.qsStyle) {
			qcbStyle->setCurrentIndex(i);
			break;
		}
	}
	qleCSS->setText(r.qsSkin);
	qcbHorizontal->setChecked(r.bHorizontal);
        qcbExpand->setChecked(r.bExpandAll);
        qcbPlayersTop->setChecked(r.bPlayerTop);
        qcbAutoUpdate->setChecked(r.bUpdateCheck);
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
	s.bHorizontal = qcbHorizontal->isChecked();
	s.bExpandAll=qcbExpand->isChecked();
	s.bPlayerTop=qcbPlayersTop->isChecked();
	s.bUpdateCheck=qcbAutoUpdate->isChecked();
}

void LookConfig::accept() const {
	if (! s.qsStyle.isEmpty()) {
		qApp->setStyle(s.qsStyle);
	}
	if (s.qsSkin.isEmpty()) {
		qApp->setStyleSheet(QString());
	} else {
		QFile file(s.qsSkin);
		file.open(QFile::ReadOnly);
		QString sheet = QLatin1String(file.readAll());
		if (! sheet.isEmpty())
			qApp->setStyleSheet(sheet);
	}

	if (s.bHorizontal) {
		g.mw->qsSplit->setOrientation(Qt::Horizontal);
		g.mw->qsSplit->addWidget(g.mw->qteLog);
		g.mw->qsSplit->addWidget(g.mw->qtvPlayers);
	} else {
		g.mw->qsSplit->setOrientation(Qt::Vertical);
		g.mw->qsSplit->addWidget(g.mw->qtvPlayers);
		g.mw->qsSplit->addWidget(g.mw->qteLog);
	}
}

bool LookConfig::expert(bool b) {
	qcbExpand->setVisible(b);
	qcbHorizontal->setVisible(b);
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
