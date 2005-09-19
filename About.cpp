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

#include <QTabWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QWidget>
#include <QApplication>
#include <QMessageBox>
#include <QIcon>
#include <QFileDialog>
#include <speex/speex.h>
#include "About.h"
#include "licenses.h"

AboutDialog::AboutDialog(QWidget *p) : QDialog(p) {
	QTabWidget *qtwTab = new QTabWidget(this);
    QVBoxLayout *vblMain = new QVBoxLayout(this);

	QTextEdit *qteLicense=new QTextEdit();
	qteLicense->setReadOnly(true);
	qteLicense->setPlainText(licenseMumble);

	QWidget *about=new QWidget();

	QLabel *icon=new QLabel(about);
	icon->setPixmap(qApp->windowIcon().pixmap(qApp->windowIcon().actualSize(QSize(64,64))));

	QLabel *text=new QLabel(about);
	text->setText(tr(
		"<h3>Mumble v0.3.2</h3>"
		"<p>Copyright 2005 Thorvald Natvig<br />slicer@users.sourceforge.net</p>"
		"<p><b>A voicechat utility for gamers</b></p>"
		"<p><tt>http://mumble.sourceforge.net/</tt></p>"
	));
	QHBoxLayout *qhbl=new QHBoxLayout();
	qhbl->addWidget(icon);
	qhbl->addWidget(text);
	about->setLayout(qhbl);

	qtwTab->addTab(about, tr("&About Mumble"));
	qtwTab->addTab(qteLicense, tr("&License"));

    QPushButton *okButton = new QPushButton(tr("OK"));
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));

    vblMain->addWidget(qtwTab);
    vblMain->addWidget(okButton);

    setLayout(vblMain);
}

AboutSpeexDialog::AboutSpeexDialog(QWidget *p) : QDialog(p) {
	char *verptr;
	speex_lib_ctl(SPEEX_LIB_GET_VERSION_STRING, &verptr);

	QLabel *text=new QLabel();
	text->setText(tr(
		"<h3>About Speex</h3>"
		"<p><tt>http://www.speex.org/</tt></p>"
		"<p>This program uses Speex version %1</p>"
		"<p>Speex is used for echo cancellation, noise<br />"
		"filtering, voice activity detection and speech<br />"
		"compression.</p>"
	).arg(verptr));

    QPushButton *okButton = new QPushButton(tr("OK"));
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));

    QVBoxLayout *vblMain = new QVBoxLayout();
	QHBoxLayout *qhbl=new QHBoxLayout();
	qhbl->addWidget(text);

	vblMain->addLayout(qhbl);
	vblMain->addWidget(okButton);
    setLayout(vblMain);
}
