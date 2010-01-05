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

	QWidget *about=new QWidget(qtwTab);

	QLabel *icon=new QLabel(about);
	icon->setPixmap(g.mw->qiIcon.pixmap(g.mw->qiIcon.actualSize(QSize(128,128))));

	QLabel *text=new QLabel(about);
	text->setOpenExternalLinks(true);
	text->setText(tr(
	                  "<h3>Mumble (%1)</h3>"
	                  "<p>Copyright %3 Thorvald Natvig<br />slicer@users.sourceforge.net</p>"
	                  "<p><b>A voice-chat utility for gamers</b></p>"
	                  "<p><tt><a href=\"%2\">%2</a></tt></p>"
	              ).arg(QLatin1String(MUMBLE_RELEASE)).arg(QLatin1String("http://mumble.sourceforge.net/")).arg(QLatin1String("2005-2008")));
	QHBoxLayout *qhbl=new QHBoxLayout(about);
	qhbl->addWidget(icon);
	qhbl->addWidget(text);

	qtwTab->addTab(about, tr("&About Mumble"));
	qtwTab->addTab(qteLicense, tr("&License"));

	QPushButton *okButton = new QPushButton(tr("OK"), this);
	connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));

	vblMain->addWidget(qtwTab);
	vblMain->addWidget(okButton);
}

AboutSpeexDialog::AboutSpeexDialog(QWidget *p) : QDialog(p) {
	char *verptr;
	speex_lib_ctl(SPEEX_LIB_GET_VERSION_STRING, &verptr);

	setWindowTitle(tr("About Speex"));

	QLabel *text=new QLabel(this);
	text->setOpenExternalLinks(true);
	text->setText(tr(
	                  "<h3>About Speex</h3>"
	                  "<p><tt><a href=\"%2\">%2</a></tt></p>"
	                  "<p>This program uses Speex version %1</p>"
	                  "<p>Speex is used for echo cancellation, noise<br />"
	                  "filtering, voice activity detection and speech<br />"
	                  "compression.</p>"
	              ).arg(QLatin1String(verptr)).arg(QLatin1String("http://www.speex.org/")));

	QPushButton *okButton = new QPushButton(tr("OK"), this);
	connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));

	QVBoxLayout *vblMain = new QVBoxLayout(this);
	QHBoxLayout *qhbl=new QHBoxLayout();
	qhbl->addWidget(text);

	vblMain->addLayout(qhbl);
	vblMain->addWidget(okButton);
}
