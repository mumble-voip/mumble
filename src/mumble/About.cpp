// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "About.h"

#include "License.h"
#include "MainWindow.h"

#include "Utils.h"

#include <QtWidgets/QPushButton>

#include "Global.h"

#define DOQUOTE(arg) #arg
#define QUOTE(arg) DOQUOTE(arg)

AboutDialog::AboutDialog(QWidget *p) : QDialog(p) {
	setWindowTitle(tr("About Mumble"));

	QTabWidget *qtwTab   = new QTabWidget(this);
	QVBoxLayout *vblMain = new QVBoxLayout(this);

	QTextEdit *qteLicense = new QTextEdit(qtwTab);
	qteLicense->setReadOnly(true);
	qteLicense->setPlainText(License::license());
	qteLicense->setAccessibleName(tr("License agreement"));

	QTextBrowser *authors = new QTextBrowser(qtwTab);
	authors->setReadOnly(true);
	authors->setOpenExternalLinks(true);
	authors->setText(tr("For a list of authors, please see <a "
						"href=\"https://github.com/mumble-voip/mumble/graphs/contributors\">https://github.com/"
						"mumble-voip/mumble/graphs/contributors</a>"));
	authors->setAccessibleName(tr("Authors"));

	QTextBrowser *qtb3rdPartyLicense = new QTextBrowser(qtwTab);
	qtb3rdPartyLicense->setReadOnly(true);
	qtb3rdPartyLicense->setOpenExternalLinks(true);
	qtb3rdPartyLicense->setAccessibleName(tr("Third-party license agreements"));

	QList< LicenseInfo > thirdPartyLicenses = License::thirdPartyLicenses();
	foreach (LicenseInfo li, thirdPartyLicenses) {
		qtb3rdPartyLicense->append(QString::fromLatin1("<h3>%1 (<a href=\"%2\">%2</a>)</h3><pre>%3</pre>")
									   .arg(li.name.toHtmlEscaped())
									   .arg(li.url.toHtmlEscaped())
									   .arg(li.license.toHtmlEscaped()));
	}

	qtb3rdPartyLicense->moveCursor(QTextCursor::Start);

	QWidget *about = new QWidget(qtwTab);

	QLabel *icon = new QLabel(about);
	icon->setPixmap(Global::get().mw->qiIcon.pixmap(Global::get().mw->qiIcon.actualSize(QSize(128, 128))));

	QLabel *text = new QLabel(about);
	text->setTextInteractionFlags(Qt::TextBrowserInteraction);
	text->setOpenExternalLinks(true);

	QString copyrightText;
#ifdef MUMBLE_BUILD_YEAR
	copyrightText = "Copyright 2005-" QUOTE(MUMBLE_BUILD_YEAR) " The Mumble Developers";
#else  // MUMBLE_BUILD_YEAR
	copyrightText = "Copyright 2005-now The Mumble Developers";
#endif // MUMBLE_BUILD_YEAR

	text->setText(tr("<h3>Mumble (%1)</h3>"
					 "<p>%3</p>"
					 "<p><b>An Open Source, low-latency, high quality voice-chat utility</b></p>"
					 "<p><tt><a href=\"%2\">%2</a></tt></p>")
					  .arg(Version::getRelease())
					  .arg(QLatin1String("https://www.mumble.info/"))
					  .arg(copyrightText));
	QHBoxLayout *qhbl = new QHBoxLayout(about);
	qhbl->addWidget(icon);
	qhbl->addWidget(text);

	qtwTab->addTab(about, tr("&About Mumble"));
	qtwTab->addTab(qteLicense, tr("&License"));
	qtwTab->addTab(authors, tr("A&uthors"));
	qtwTab->addTab(qtb3rdPartyLicense, tr("&Third-Party Licenses"));

	QPushButton *okButton = new QPushButton(tr("OK"), this);
	connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));

	vblMain->addWidget(qtwTab);
	vblMain->addWidget(okButton);
}

#undef DOQUOTE
#undef QUOTE
