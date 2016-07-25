// Copyright 2005-2016 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "mumble_pch.hpp"

#include "About.h"

#include "Global.h"
#include "MainWindow.h"
#include "licenses.h"

AboutDialog::AboutDialog(QWidget *p) : QDialog(p) {
	setWindowTitle(tr("About Mumble"));

	QTabWidget *qtwTab = new QTabWidget(this);
	QVBoxLayout *vblMain = new QVBoxLayout(this);

	QTextEdit *qteLicense=new QTextEdit(qtwTab);
	qteLicense->setReadOnly(true);
	qteLicense->setPlainText(QLatin1String(licenseMumble));

	QTextBrowser *qtb3rdPartyLicense = new QTextBrowser(qtwTab);
	qtb3rdPartyLicense->setReadOnly(true);
	qtb3rdPartyLicense->setOpenExternalLinks(true);

	for(int i = 0; !licenses3rdParties[i].isEmpty(); ++i)
	{
		qtb3rdPartyLicense->append(QString::fromLatin1("<h3>%1 (<a href=\"%2\">%2</a>)</h3><pre>%3</pre>")
				.arg(Qt::escape(QString::fromLatin1(licenses3rdParties[i].name)))
				.arg(Qt::escape(QString::fromLatin1(licenses3rdParties[i].url)))
				.arg(Qt::escape(QString::fromLatin1(licenses3rdParties[i].license))));
	}

	qtb3rdPartyLicense->moveCursor(QTextCursor::Start);

	QWidget *about=new QWidget(qtwTab);

	QLabel *icon=new QLabel(about);
	icon->setPixmap(g.mw->qiIcon.pixmap(g.mw->qiIcon.actualSize(QSize(128, 128))));

	QLabel *text=new QLabel(about);
	text->setOpenExternalLinks(true);
	text->setText(tr(
		"<h3>Mumble (%1)</h3>"
		"<p>%3</p>"
		"<p><b>A voice-chat utility for gamers</b></p>"
		"<p><tt><a href=\"%2\">%2</a></tt></p>"
	).arg(QLatin1String(MUMBLE_RELEASE))
	 .arg(QLatin1String("http://www.mumble.info/"))
	 .arg(QLatin1String("Copyright 2005-2016 The Mumble Developers")));
	QHBoxLayout *qhbl=new QHBoxLayout(about);
	qhbl->addWidget(icon);
	qhbl->addWidget(text);

	qtwTab->addTab(about, tr("&About Mumble"));
	qtwTab->addTab(qteLicense, tr("&License"));
	qtwTab->addTab(qtb3rdPartyLicense, tr("3rd &party licenses"));

	QPushButton *okButton = new QPushButton(tr("OK"), this);
	connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));

	vblMain->addWidget(qtwTab);
	vblMain->addWidget(okButton);
}
