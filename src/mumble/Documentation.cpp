// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Documentation.h"

#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>

DocumentationDialog::DocumentationDialog(QWidget *parent) : QDialog(parent) {
	setWindowTitle(tr("Mumble Documentation"));
	resize(640, 520);

	QVBoxLayout *mainLayout = new QVBoxLayout(this);

	QTextBrowser *documentationView = new QTextBrowser(this);
	documentationView->setReadOnly(true);
	documentationView->setOpenExternalLinks(true);
	documentationView->setAccessibleName(tr("Documentation links"));
	documentationView->setHtml(
		tr("<h2>Welcome to Mumble Documentation</h2>"
		   "<h3>Documentation</h3>"
		   "<p>You can find official Mumble documentation and support resources below.</p>"
		   "<p>For new users, start with the <a href=\"https://www.mumble.info/documentation/user/\">User Guide</a>.</p>"
		   "<p>Configure key bindings with <a href=\"https://www.mumble.info/documentation/user/global-shortcuts/\">Global Shortcuts</a>.</p>"
		   "<p>Tune your setup with the <a href=\"https://www.mumble.info/documentation/user/audio-settings/\">Audio Settings guide</a>.</p>"
		   "<p>Browse all topics in <a href=\"https://www.mumble.info/documentation/\">Main Documentation</a>.</p>"
		   "<h3>Help and Resources</h3>"
		   "<p>Download the latest release from <a href=\"https://www.mumble.info/downloads/\">Downloads</a>.</p>"
		   "<p>Report issues or get technical help via <a href=\"https://github.com/mumble-voip/mumble/issues\">GitHub Issues</a>.</p>"
		   "<p>Read project updates on the <a href=\"https://www.mumble.info/blog/\">Blog</a>.</p>"
		   "<p>Reach the team through the <a href=\"https://www.mumble.info/contact/\">Contact page</a>.</p>"));

	QPushButton *okButton = new QPushButton(tr("OK"), this);
	connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));

	mainLayout->addWidget(documentationView);
	mainLayout->addWidget(okButton);
}
