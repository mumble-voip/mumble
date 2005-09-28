/* Copyright (C) 2005, Thorvald Natvig <thorvald@natvig.com>

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

#include <QApplication>
#include <QIcon>
#include <QPushButton>
#include <QHBoxLayout>
#include "Audio.h"

#include "AudioInput.h"
#include "AudioOutput.h"
#include "Global.h"

QMap<int, ConfigWidgetNew> *ConfigRegistrar::c_qmNew;

ConfigRegistrar::ConfigRegistrar(int priority, ConfigWidgetNew n) {
	if (! c_qmNew)
		c_qmNew = new QMap<int, ConfigWidgetNew>();
	c_qmNew->insert(priority,n);
}

ConfigWidget::ConfigWidget(QWidget *p) : QWidget(p) {
}

QString ConfigWidget::title() const {
	return QString("Missing Title");
}

QIcon ConfigWidget::icon() const {
	return qApp->windowIcon();
}

ConfigDialog::ConfigDialog(QWidget *p) : QDialog(p) {
    setWindowTitle(tr("Mumble Configuration"));

    qlwIcons = new QListWidget();
    qswPages = new QStackedWidget();

    qlwIcons->setViewMode(QListView::IconMode);
    qlwIcons->setIconSize(QSize(96, 84));
    qlwIcons->setMovement(QListView::Static);
    qlwIcons->setMaximumWidth(128);
    qlwIcons->setSpacing(12);
    qlwIcons->setObjectName("Icons");

    QPushButton *okButton = new QPushButton(tr("&OK"));
    okButton->setDefault(true);
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
    okButton->setToolTip(tr("Accept changes"));
    okButton->setWhatsThis(tr("This button will accept current settings and return to the application.<br />"
    						"The settings will be stored to disk when you leave the application."));
    QPushButton *cancelButton = new QPushButton(tr("&Cancel"));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
	cancelButton->setToolTip(tr("Reject changes"));
	cancelButton->setWhatsThis(tr("This button will reject all changes and return to the application.<br />"
								  "The settings will be reset to the previous positions."));
    QPushButton *applyButton = new QPushButton(tr("&Apply"));
    connect(applyButton, SIGNAL(clicked()), this, SLOT(apply()));
	applyButton->setToolTip(tr("Apply changes"));
	applyButton->setWhatsThis(tr("This button will immediately apply all changes."));

	ConfigWidgetNew cwn;
	foreach(cwn, *ConfigRegistrar::c_qmNew) {
		addPage(cwn());
	}

    QHBoxLayout *buttons = new QHBoxLayout;
    buttons->addStretch(1);
    buttons->addWidget(applyButton);
    buttons->addWidget(okButton);
    buttons->addWidget(cancelButton);

    QVBoxLayout *l = new QVBoxLayout;
    l->addWidget(qswPages, 1);
    l->addSpacing(12);
    l->addLayout(buttons);


    QHBoxLayout *top = new QHBoxLayout;
    top->addWidget(qlwIcons);
    top->addLayout(l);

    setLayout(top);

	qlwIcons->scrollTo(qlwIcons->currentIndex(), QAbstractItemView::PositionAtTop);

    QMetaObject::connectSlotsByName(this);

    qlwIcons->setCurrentRow(0);
}

void ConfigDialog::addPage(ConfigWidget *cw) {
	qswPages->addWidget(cw);
	QListWidgetItem *i = new QListWidgetItem(qlwIcons);
	i->setIcon(cw->icon());
	i->setText(cw->title());
	i->setTextAlignment(Qt::AlignHCenter);
	i->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

	widgets << cw;
}

void ConfigDialog::on_Icons_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current)
        current = previous;

    qswPages->setCurrentIndex(qlwIcons->row(current));
}

void ConfigDialog::apply() {
	foreach(ConfigWidget *cw, widgets)
		cw->accept();

	QWriteLocker lock(&g.qrwlAudio);

	delete g.ai;
	delete g.ao;

	g.ai = AudioInputRegistrar::newFromChoice(g.s.qsAudioInput);
	g.ai->start(QThread::HighestPriority);
	g.ao = AudioOutputRegistrar::newFromChoice(g.s.qsAudioOutput);
}

void ConfigDialog::accept() {
	apply();
	QDialog::accept();
}
