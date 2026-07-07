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

#ifdef USE_TFAR
#	include "tfar/StormBranding.h"
#endif

#define DOQUOTE(arg) #arg
#define QUOTE(arg) DOQUOTE(arg)

AboutDialog::AboutDialog(QWidget *p) : QDialog(p) {
#ifdef USE_TFAR
	setWindowTitle(tr("About %1").arg(QLatin1String(STORM_APP_NAME)));
#else
	setWindowTitle(tr("About Mumble"));
#endif

	QTabWidget *qtwTab   = new QTabWidget(this);
	QVBoxLayout *vblMain = new QVBoxLayout(this);

	QTextEdit *qteLicense = new QTextEdit(qtwTab);
	qteLicense->setReadOnly(true);
	qteLicense->setPlainText(License::license());
	qteLicense->setAccessibleName(tr("License agreement"));

	QTextBrowser *authors = new QTextBrowser(qtwTab);
	authors->setReadOnly(true);
	authors->setOpenExternalLinks(true);
#ifdef USE_TFAR
	authors->setText(tr("Storm Voice / TFAR integration: <a href=\"%1/graphs/contributors\">%1/graphs/contributors</a>"
						"<br/>Mumble authors: <a href=\"https://github.com/mumble-voip/mumble/graphs/contributors\">"
						"https://github.com/mumble-voip/mumble/graphs/contributors</a>"
						"<br/>TFAR authors: <a href=\"https://github.com/michail-nikolaev/task-force-arma-3-radio/"
						"graphs/contributors\">michail-nikolaev/task-force-arma-3-radio</a>")
						.arg(QLatin1String(STORM_GITHUB_URL)));
#else
	authors->setText(tr("For a list of authors, please see <a "
						"href=\"https://github.com/mumble-voip/mumble/graphs/contributors\">https://github.com/"
						"mumble-voip/mumble/graphs/contributors</a>"));
#endif
	authors->setAccessibleName(tr("Authors"));

	QTextBrowser *qtb3rdPartyLicense = new QTextBrowser(qtwTab);
	qtb3rdPartyLicense->setReadOnly(true);
	qtb3rdPartyLicense->setOpenExternalLinks(true);
	qtb3rdPartyLicense->setAccessibleName(tr("Third-party license agreements"));

	QList< LicenseInfo > thirdPartyLicenses = License::thirdPartyLicenses();
	for (const LicenseInfo &li : thirdPartyLicenses) {
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

	QString aboutText = QString();
#ifdef USE_TFAR
	// MUMBLE-TFAR: product branding + the attribution required by the licenses
	// of the works this build is derived from (BSD, APL-SA, LGPL).
	aboutText += QString("<h3>%1 %2</h3>").arg(QLatin1String(STORM_APP_NAME_FULL), QLatin1String(STORM_TFAR_VERSION));
	aboutText += QString("<p><b>%1</b></p>").arg(QLatin1String(STORM_PROJECT_NAME));
	aboutText += QString("<p>") + tr("Voice client with integrated Task Force Arrowhead Radio (TFAR) support.")
				 + QString("</p>");
	aboutText += QString("<p><tt><a href=\"%1\">%1</a></tt></p>").arg(QLatin1String(STORM_GITHUB_URL));
	aboutText += QString("<hr/><p><small>");
	aboutText += tr("Based on <a href=\"%1\">Mumble</a> %2 &mdash; %3 (BSD license).")
					 .arg(QLatin1String("https://www.mumble.info/"), Version::getRelease(),
						  copyrightText.toHtmlEscaped());
	aboutText += QString("<br/>");
	aboutText += tr("Radio simulation: <a href=\"%1\">Task Force Arrowhead Radio</a> &copy; 2013 Michail Nikolaev, "
					"[TF]Nkey, Dedmen and contributors (Arma Public License Share Alike).")
					 .arg(QLatin1String("https://github.com/michail-nikolaev/task-force-arma-3-radio"));
	aboutText += QString("<br/>");
	aboutText += tr("3D audio: clunk &copy; Netive Media Group (LGPL 2.1). Full list: THIRD-PARTY-TFAR.md.");
	aboutText += QString("<br/>");
	aboutText += tr("This project is not affiliated with Mumble, TeamSpeak or Bohemia Interactive. "
					"Distributed free of charge.");
	aboutText += QString("</small></p>");
#else
	aboutText +=
		QString("<h3>Mumble (%1 %2)</h3>").arg(Version::getRelease()).arg(QString::fromUtf8(MUMBLE_TARGET_ARCH));
	aboutText += QString("<p>%1</p>").arg(copyrightText);
	aboutText +=
		QString("<p><b>") + tr("An Open Source, low-latency, high quality voice-chat utility") + QString("</b></p>");
	aboutText += QString("<p><tt><a href=\"%1\">%1</a></tt></p>").arg(QLatin1String("https://www.mumble.info/"));
#endif
	text->setText(aboutText);

	QHBoxLayout *qhbl = new QHBoxLayout(about);
	qhbl->addWidget(icon);
	qhbl->addWidget(text);

#ifdef USE_TFAR
	qtwTab->addTab(about, tr("&About %1").arg(QLatin1String(STORM_APP_NAME)));
#else
	qtwTab->addTab(about, tr("&About Mumble"));
#endif
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
