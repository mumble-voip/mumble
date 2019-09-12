// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ConfigDialog.h"

#include "AudioInput.h"
#include "AudioOutput.h"
#include "Global.h"
#include "Overlay.h"

#include <QtGui/QScreen>
#include <QtWidgets/QDesktopWidget>
#include <QtWidgets/QPushButton>


ConfigDialog::ConfigDialog(QWidget *p) : QDialog(p) {
	setupUi(this);

	s = g.s;

	unsigned int idx = 0;
	ConfigWidgetNew cwn;
	foreach(cwn, *ConfigRegistrar::c_qmNew) {
		addPage(cwn(s), ++idx);
	}

	updateListView();

	QPushButton *okButton = dialogButtonBox->button(QDialogButtonBox::Ok);
	okButton->setToolTip(tr("Accept changes"));
	okButton->setWhatsThis(tr("This button will accept current settings and return to the application.<br />"
	                          "The settings will be stored to disk when you leave the application."));

	QPushButton *cancelButton = dialogButtonBox->button(QDialogButtonBox::Cancel);
	cancelButton->setToolTip(tr("Reject changes"));
	cancelButton->setWhatsThis(tr("This button will reject all changes and return to the application.<br />"
	                              "The settings will be reset to the previous positions."));

	QPushButton *applyButton = dialogButtonBox->button(QDialogButtonBox::Apply);
	applyButton->setToolTip(tr("Apply changes"));
	applyButton->setWhatsThis(tr("This button will immediately apply all changes."));

	QPushButton *resetButton = pageButtonBox->button(QDialogButtonBox::Reset);
	resetButton->setToolTip(tr("Undo changes for current page"));
	resetButton->setWhatsThis(tr("This button will revert any changes done on the current page to the most recent applied settings."));

	QPushButton *restoreButton = pageButtonBox->button(QDialogButtonBox::RestoreDefaults);
	restoreButton->setToolTip(tr("Restore defaults for current page"));
	restoreButton->setWhatsThis(tr("This button will restore the defaults for the settings on the current page. Other pages will not be changed.<br />"
	                               "To restore all settings to their defaults, you will have to use this button on every page."
	                              ));

	if (! g.s.qbaConfigGeometry.isEmpty()) {
		if (! g.ocIntercept)
			restoreGeometry(g.s.qbaConfigGeometry);
	}
}

void ConfigDialog::addPage(ConfigWidget *cw, unsigned int idx) {
	int w = INT_MAX, h = INT_MAX;

	const QList<QScreen *> screens = qApp->screens();
	for (int i = 0; i < screens.size(); ++i) {
		const QRect ds = screens[i]->availableGeometry();
		if (ds.isValid()) {
			w = qMin(w, ds.width());
			h = qMin(h, ds.height());
		}
	}

	QSize ms=cw->minimumSizeHint();
	cw->resize(ms);
	cw->setMinimumSize(ms);

	ms.rwidth() += 128;
	ms.rheight() += 192;
	if ((ms.width() > w) || (ms.height() > h)) {
		QScrollArea *qsa=new QScrollArea();
		qsa->setFrameShape(QFrame::NoFrame);
		qsa->setWidgetResizable(true);
		qsa->setWidget(cw);
		qhPages.insert(cw, qsa);
		qswPages->addWidget(qsa);
	} else {
		qhPages.insert(cw, cw);
		qswPages->addWidget(cw);
	}
	qmWidgets.insert(idx, cw);
	cw->load(g.s);
}

ConfigDialog::~ConfigDialog() {
	foreach(QWidget *qw, qhPages)
		delete qw;
}

void ConfigDialog::on_pageButtonBox_clicked(QAbstractButton *b) {
	ConfigWidget *conf = qobject_cast<ConfigWidget *>(qswPages->currentWidget());
	if (! conf) {
		QScrollArea *qsa = qobject_cast<QScrollArea *>(qswPages->currentWidget());
		if (qsa)
			conf = qobject_cast<ConfigWidget *>(qsa->widget());
	}
	if (! conf)
		return;
	switch (pageButtonBox->standardButton(b)) {
		case QDialogButtonBox::RestoreDefaults: {
				Settings def;
				conf->load(def);
				break;
			}
		case QDialogButtonBox::Reset: {
				conf->load(g.s);
				break;
			}
		default:
			break;
	}
}

void ConfigDialog::on_dialogButtonBox_clicked(QAbstractButton *b) {
	switch (dialogButtonBox->standardButton(b)) {
		case QDialogButtonBox::Apply: {
				apply();
				break;
			}
		default:
			break;
	}
}

void ConfigDialog::on_qlwIcons_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous) {
	if (!current)
		current = previous;

	if (current) {
		QWidget *w = qhPages.value(qmIconWidgets.value(current));
		if (w)
			qswPages->setCurrentWidget(w);
	}
}

void ConfigDialog::updateListView() {
	QWidget *ccw = qmIconWidgets.value(qlwIcons->currentItem());
	QListWidgetItem *sel = NULL;

	qmIconWidgets.clear();
	qlwIcons->clear();

	QFontMetrics qfm(qlwIcons->font());
	int configNavbarWidth = 0;

	foreach(ConfigWidget *cw, qmWidgets) {
#if QT_VERSION >= QT_VERSION_CHECK(5, 11, 0)
		configNavbarWidth = qMax(configNavbarWidth, qfm.horizontalAdvance(cw->title()));
#else
		configNavbarWidth = qMax(configNavbarWidth, qfm.width(cw->title()));
#endif

		QListWidgetItem *i = new QListWidgetItem(qlwIcons);
		i->setIcon(cw->icon());
		i->setText(cw->title());
		i->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

		qmIconWidgets.insert(i, cw);
		if (cw == ccw)
			sel = i;
	}

	// Add space for icon and some padding.
	configNavbarWidth += qlwIcons->iconSize().width() + 25;

	qlwIcons->setMinimumWidth(configNavbarWidth);
	qlwIcons->setMaximumWidth(configNavbarWidth);

	if (sel)
		qlwIcons->setCurrentItem(sel);
	else
		qlwIcons->setCurrentRow(0);
}

void ConfigDialog::apply() {
	Audio::stop();

	foreach(ConfigWidget *cw, qmWidgets)
		cw->save();

	g.s = s;

	foreach(ConfigWidget *cw, qmWidgets)
		cw->accept();
	
	if (!g.s.bAttenuateOthersOnTalk)
		g.bAttenuateOthers = false;

	// They might have changed their keys.
	g.iPushToTalk = 0;

	Audio::start();
}

void ConfigDialog::accept() {
	apply();

	if (! g.ocIntercept)
		g.s.qbaConfigGeometry=saveGeometry();

	QDialog::accept();
}
