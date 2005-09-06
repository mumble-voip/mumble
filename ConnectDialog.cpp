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

#include "ConnectDialog.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QGridLayout>
#include <QIntValidator>
#include <QSettings>

ConnectDialog::ConnectDialog(QWidget *parent) : QDialog(parent) {
	QGridLayout *l=new QGridLayout;
    QVBoxLayout *vbl = new QVBoxLayout;
	QHBoxLayout *vbh = new QHBoxLayout();
    vbl->addLayout(l);
    vbl->addLayout(vbh);

    setLayout(vbl);
	QLabel *lab;

	QSettings qs;

	qleServer=new QLineEdit(qs.value("ServerAddress", "").toString(), this);
	lab=new QLabel("&Address", this);
	lab->setBuddy(qleServer);

	l->addWidget(lab, 0, 0);
	l->addWidget(qleServer, 0, 1);

	qlePort=new QLineEdit(qs.value("ServerPort", "64738").toString(), this);
	qlePort->setValidator(new QIntValidator(1, 65535, qlePort));
	lab=new QLabel("&Port", this);
	lab->setBuddy(qlePort);

	l->addWidget(lab, 1, 0);
	l->addWidget(qlePort, 1, 1);

	qlePassword=new QLineEdit(qs.value("ServerPassword", "").toString(), this);
	qlePassword->setEchoMode(QLineEdit::Password);
	lab=new QLabel("&Password", this);
	lab->setBuddy(qlePassword);

	l->addWidget(lab, 2, 0);
	l->addWidget(qlePassword, 2,1);

	qleUsername=new QLineEdit(qs.value("ServerUsername", "").toString(), this);
	lab=new QLabel("&Username", this);
	lab->setBuddy(qleUsername);

	l->addWidget(lab, 3, 0);
	l->addWidget(qleUsername, 3, 1);

    QPushButton *okButton = new QPushButton("&OK");
    okButton->setDefault(true);
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));

    QPushButton *cancelButton = new QPushButton("&Cancel");
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

	vbh->addWidget(okButton);
	vbh->addWidget(cancelButton);
}

void ConnectDialog::accept() {
	qsServer = qleServer->text();
	qsUsername = qleUsername->text();
	qsPassword = qlePassword->text();
	iPort = qlePort->text().toInt();

	QSettings qs;
	qs.setValue("ServerAddress", qsServer);
	qs.setValue("ServerUsername", qsUsername);
	qs.setValue("ServerPassword", qsPassword);
	qs.setValue("ServerPort", iPort);

	QDialog::accept();
}
