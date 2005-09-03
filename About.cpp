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

#include "About.h"

#include <QTabWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>

#include "licenses.h"

static const char *about = "Mumble - A group voice chat utility \n\n\
http://mumble.sourceforge.net\
\n\n\
See 'BSD License' for license information on the Mumble sourcecode.\n\
For binary releases, all of licenses of libraries Mumble depends \
on applies as well, see the individual license tabs.";

AboutDialog::AboutDialog(QWidget *parent) : QDialog(parent) {
	QTabWidget *qtwTab = new QTabWidget(this);

	QTextEdit *qteMain=new QTextEdit();
	qteMain->setReadOnly(true);
	qteMain->setPlainText(about);

	QTextEdit *qteBSD=new QTextEdit();
	qteBSD->setReadOnly(true);
	qteBSD->setPlainText(licenseMumble);

	QTextEdit *qteQT=new QTextEdit();
	qteQT->setReadOnly(true);
	qteQT->setPlainText(licenseGPL);

	QTextEdit *qteSpeex=new QTextEdit();
	qteSpeex->setReadOnly(true);
	qteSpeex->setPlainText(licenseSpeex);

	QTextEdit *qteUPX=new QTextEdit();
	qteUPX->setReadOnly(true);
	qteUPX->setPlainText(licenseUPX);

	qtwTab->addTab(qteMain, "About Mumble");
	qtwTab->addTab(qteBSD, "Source License");
	qtwTab->addTab(qteQT, "QT");
	qtwTab->addTab(qteSpeex, "Speex");
	qtwTab->addTab(qteUPX, "UPX");

    QPushButton *okButton = new QPushButton("OK");
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));

    QVBoxLayout *vblMain = new QVBoxLayout;
    vblMain->addWidget(qtwTab);
    vblMain->addWidget(okButton);

    setLayout(vblMain);

    resize(550,300);
}
