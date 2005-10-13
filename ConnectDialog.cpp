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

#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QGridLayout>
#include <QIntValidator>
#include <QSettings>
#include <QSqlQuery>
#include <QSqlError>
#include <QItemSelectionModel>
#include <QSqlRecord>
#include <QMessageBox>

#include "ConnectDialog.h"
#include "Global.h"

ConnectDialog::ConnectDialog(QWidget *p) : QDialog(p) {
	QGridLayout *l=new QGridLayout;
    QVBoxLayout *vbl = new QVBoxLayout;
	QHBoxLayout *vbh = new QHBoxLayout();
    vbl->addLayout(l);
    vbl->addLayout(vbh);

    setLayout(vbl);
	QLabel *lab;

	bDirty = false;

	qstmServers = new QSqlTableModel(this);
	qstmServers->setTable("servers");
	qstmServers->setSort(1, Qt::AscendingOrder);
	if (! qstmServers->select()) {
		qWarning("ConnectDialog: Failed to reselect table");
	}
	qstmServers->setEditStrategy(QSqlTableModel::OnManualSubmit);

	qlwServers=new QListView();
	l->addWidget(qlwServers,0,0,5,1);
	qlwServers->setModel(qstmServers);
	qlwServers->setModelColumn(1);
	qlwServers->setEditTriggers(QAbstractItemView::NoEditTriggers);
	qlwServers->setObjectName("List");

	QItemSelectionModel *selectionModel = qlwServers->selectionModel();
	connect(selectionModel, SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(onSelection_Changed(const QModelIndex &, const QModelIndex &)));

	qleName=new QLineEdit(g.qs->value("ServerName", "").toString());
	lab=new QLabel("&Name");
	lab->setBuddy(qleName);

	l->addWidget(lab, 0, 1);
	l->addWidget(qleName, 0, 2);

	qleServer=new QLineEdit(g.qs->value("ServerAddress", "").toString());
	lab=new QLabel("A&ddress");
	lab->setBuddy(qleServer);

	l->addWidget(lab, 1, 1);
	l->addWidget(qleServer, 1, 2);

	qlePort=new QLineEdit(g.qs->value("ServerPort", "64738").toString());
	qlePort->setValidator(new QIntValidator(1, 65535, qlePort));
	lab=new QLabel("&Port");
	lab->setBuddy(qlePort);

	l->addWidget(lab, 2, 1);
	l->addWidget(qlePort, 2, 2);

	qleUsername=new QLineEdit(g.qs->value("ServerUsername", "").toString());
	lab=new QLabel("&Username");
	lab->setBuddy(qleUsername);

	l->addWidget(lab, 3, 1);
	l->addWidget(qleUsername, 3, 2);

	qlePassword=new QLineEdit(g.qs->value("ServerPassword", "").toString());
	qlePassword->setEchoMode(QLineEdit::Password);
	lab=new QLabel("&Password");
	lab->setBuddy(qlePassword);

	l->addWidget(lab, 4, 1);
	l->addWidget(qlePassword, 4,2);

    QPushButton *okButton = new QPushButton("&Connect");
    okButton->setDefault(true);
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));

    QPushButton *cancelButton = new QPushButton("Cancel");
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

    QPushButton *addButton = new QPushButton("&Add");
    addButton->setObjectName("Add");

    QPushButton *removeButton = new QPushButton("&Remove");
    removeButton->setObjectName("Remove");

	vbh->addWidget(okButton);
	vbh->addWidget(cancelButton);
	vbh->addWidget(addButton);
	vbh->addWidget(removeButton);

	connect(qleName, SIGNAL(textEdited(const QString &)), this, SLOT(onDirty(const QString &)));
	connect(qleServer, SIGNAL(textEdited(const QString &)), this, SLOT(onDirty(const QString &)));
	connect(qleUsername, SIGNAL(textEdited(const QString &)), this, SLOT(onDirty(const QString &)));
	connect(qlePassword, SIGNAL(textEdited(const QString &)), this, SLOT(onDirty(const QString &)));
	connect(qlePort, SIGNAL(textEdited(const QString &)), this, SLOT(onDirty(const QString &)));

	connect(qlwServers, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(accept()));

    QMetaObject::connectSlotsByName(this);
}

void ConnectDialog::accept() {
	if (bDirty && qlwServers->currentIndex().isValid()) {
		QSqlRecord r;
		r = toRecord();
		qstmServers->setRecord(qlwServers->currentIndex().row(), r);
		qstmServers->submitAll();
	}

	qsServer = qleServer->text();
	qsUsername = qleUsername->text();
	qsPassword = qlePassword->text();
	iPort = qlePort->text().toInt();
	bUdp = true;

	g.qs->setValue("ServerAddress", qsServer);
	g.qs->setValue("ServerUsername", qsUsername);
	g.qs->setValue("ServerPassword", qsPassword);
	g.qs->setValue("ServerPort", iPort);

	QDialog::accept();
}

QSqlRecord ConnectDialog::toRecord() const
{
	QSqlRecord r = qstmServers->record();
	r.setValue("name", qleName->text());
	r.setValue("hostname", qleServer->text());
	r.setValue("username", qleUsername->text());
	r.setValue("password", qlePassword->text());
	r.setValue("port", qlePort->text().toInt());
	r.setValue("udp", true);
	return r;
}

void ConnectDialog::onSelection_Changed(const QModelIndex &index, const QModelIndex &previndex)
{
	QSqlRecord r;

	if (bDirty) {
		r = toRecord();
		qstmServers->setRecord(previndex.row(), r);
		qstmServers->submitAll();
	}
	r = qstmServers->record(index.row());
	qleName->setText(r.value("name").toString());
	qleServer->setText(r.value("hostname").toString());
	qleUsername->setText(r.value("username").toString());
	qlePassword->setText(r.value("password").toString());
	qlePort->setText(r.value("port").toString());
	bDirty = false;
}

void ConnectDialog::on_Add_clicked()
{
	QSqlRecord r = toRecord();
	qstmServers->insertRecord(-1, r);
	qstmServers->submitAll();
	bDirty = false;
}

void ConnectDialog::on_Remove_clicked()
{
	qstmServers->removeRows(qlwServers->currentIndex().row(), 1, QModelIndex());
	qstmServers->submitAll();
}

void ConnectDialog::onDirty(const QString &) {
	bDirty = true;
}
