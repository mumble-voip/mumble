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

#include "ConfigDialog.h"
#include "AudioInput.h"
#include "AudioOutput.h"
#ifndef COMPAT_CLIENT
#include "Overlay.h"
#endif
#include "Global.h"

ConfigDialog::ConfigDialog(QWidget *p) : QDialog(p) {
	setupUi(this);

	s = g.s;

	unsigned int idx = 0;
	ConfigWidgetNew cwn;
	foreach(cwn, *ConfigRegistrar::c_qmNew) {
		addPage(cwn(s), ++idx);
	}

	qcbExpert->setChecked(g.s.bExpert);
	on_qcbExpert_clicked(g.s.bExpert);

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
#ifndef COMPAT_CLIENT
		if (! g.ocIntercept)
#endif
			restoreGeometry(g.s.qbaConfigGeometry);
	}
}

void ConfigDialog::addPage(ConfigWidget *cw, unsigned int idx) {
	QDesktopWidget dw;

	int w = INT_MAX, h = INT_MAX;

	for (int i=0;i<dw.numScreens();++i) {
		QRect ds=dw.availableGeometry(i);
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

void ConfigDialog::on_qcbExpert_clicked(bool b) {
	QWidget *ccw = qmIconWidgets.value(qlwIcons->currentItem());
	QListWidgetItem *sel = NULL;

	qmIconWidgets.clear();
	qlwIcons->clear();

	QFontMetrics qfm(qlwIcons->font());
	int width = 0;

	foreach(ConfigWidget *cw, qmWidgets) {
		bool showit = cw->expert(b);
		width = qMax(width, qfm.width(cw->title()));
		if (showit || b)  {
			QListWidgetItem *i = new QListWidgetItem(qlwIcons);
			i->setIcon(cw->icon());
			i->setText(cw->title());
			i->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);


			qmIconWidgets.insert(i, cw);
			if (cw == ccw)
				sel = i;
		}
	}

	// Add space for icon and some padding.
	width += qlwIcons->iconSize().width() + 25;

	qlwIcons->setMinimumWidth(width);
	qlwIcons->setMaximumWidth(width);

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
	g.s.bExpert = qcbExpert->isChecked();

	Audio::start();
}

void ConfigDialog::accept() {
	apply();

#ifndef COMPAT_CLIENT
	if (! g.ocIntercept)
#endif
		g.s.qbaConfigGeometry=saveGeometry();

	QDialog::accept();
}
