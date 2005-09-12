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
#include "AudioConfigDialog.h"

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
    QPushButton *cancelButton = new QPushButton(tr("&Cancel"));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

	addPage(new AudioConfigDialog());

    QHBoxLayout *top = new QHBoxLayout;
    top->addWidget(qlwIcons);
    top->addWidget(qswPages, 1);

    QHBoxLayout *buttons = new QHBoxLayout;
    buttons->addStretch(1);
    buttons->addWidget(okButton);
    buttons->addWidget(cancelButton);

    QVBoxLayout *l = new QVBoxLayout;
    l->addLayout(top);
    l->addStretch(1);
    l->addSpacing(12);
    l->addLayout(buttons);
    setLayout(l);

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

void ConfigDialog::accept() {
	foreach(ConfigWidget *cw, widgets)
		cw->accept();
	QDialog::accept();
}
