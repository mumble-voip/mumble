/* Copyright (C) 2005-2009, Thorvald Natvig <thorvald@natvig.com>
   Copyright (C) 2009, Mikkel Krautz <mikkel@krautz.dk>

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
#include "Global.h"

#import "ConfigDialogDelegate.h"

/*
 * This file implements the Mumble configuration dialog on Mac OS X.
 *
 * It implements a Mac OS X-style Preferences window (a toolbar at the
 * top of the window that blends in with the rest of the window, that
 * allows each item to be selected independently.)
 */

ConfigDialog::ConfigDialog(QWidget *p) : QDialog(p) {
	setupUi(this);

	s = g.s;

	cwCurrentWidget = NULL;
	setWindowTitle(tr("Preferences"));

	/* Remove QTabWidget from the layout. We don't use it on OSX. */
	qvblVertical->removeWidget(qtwWidgets);
	delete qtwWidgets;

	unsigned int idx = 0;
	ConfigWidgetNew cwn;
	foreach(cwn, *ConfigRegistrar::c_qmNew) {
		addPage(cwn(s), ++idx);
	}

	updateExpert(g.s.bExpert);

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
	restoreButton->setWhatsThis(tr("This button will restore the settings for the current page only to their defaults. Other pages will not be changed.<br />"
	                               "To restore all settings to their defaults, you will have to use this button on every page."
	                              ));

	if (! g.s.qbaConfigGeometry.isEmpty())
		restoreGeometry(g.s.qbaConfigGeometry);
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
		qsa->setWidgetResizable(true);
		qsa->setWidget(cw);
		qhPages.insert(cw, qsa);
	} else {
		qhPages.insert(cw, cw);
	}
	qmWidgets.insert(idx, cw);
	cw->load(g.s);
}

ConfigDialog::~ConfigDialog() {
	foreach(QWidget *qw, qhPages)
		delete qw;

	removeMacToolbar();
}

void ConfigDialog::on_pageButtonBox_clicked(QAbstractButton *b) {
	ConfigWidget *conf = currentWidget();

	switch (pageButtonBox->standardButton(b)) {
		case QDialogButtonBox::RestoreDefaults: {
				Settings def;
				if (conf)
					conf->load(def);
				break;
			}
		case QDialogButtonBox::Reset: {
				if (conf)
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

void ConfigDialog::setupMacToolbar(bool expert) {
	NSWindow *window = qt_mac_window_for(this);

	/* Allocate a NSToolbar for our Config Dialog. Most programs on Mac OS X use Preferences dialogs
	   with a toolbar on top, so this makes Mumble fit in well with native Mac apps.

	   The identifier string is simply an unique string for this particular toolbar. The OS will graphically
	   synchronize toolbars with the same identifier, so that if multiple NSToolbars with the same identifier
	   are used within the same application, they all stay in sync automatically. */
	NSToolbar *toolbar = [[NSToolbar alloc] initWithIdentifier: @"MumbleConfigDialog"];
        [toolbar setDisplayMode: NSToolbarDisplayModeIconAndLabel];
        [toolbar setSizeMode: NSToolbarSizeModeRegular];
	[toolbar setAllowsUserCustomization:NO];
	[toolbar setAutosavesConfiguration:NO];

	ConfigDialogDelegate *delegate = [[ConfigDialogDelegate alloc] initWithConfigDialog:this
	                                                               andWidgetMap:&qmWidgets
	                                                               inExpertMode:expert];
	[toolbar setDelegate: delegate];

	[window setToolbar: toolbar];
}

void ConfigDialog::removeMacToolbar() {
	NSWindow *window = qt_mac_window_for(this);
	NSToolbar *toolbar = [window toolbar];

	if (toolbar != nil) {
		[[toolbar delegate] release];
		[toolbar setDelegate: nil];
		[toolbar release];
	}
}

void ConfigDialog::setCurrentWidget(ConfigWidget *cw) {

	QWidget *w = qhPages[cw];
	QVBoxLayout *layout = qvblVertical;

	/*
	 * For flicker-free widget switching we do the following:
	 *  - Get the currently shown widget.
	 *  - Insert the new widget (it's hidden until we show() it).
	 *  - Hide the currently shown widget, then show the new one.
	 *  - Delete the QItemLayout for the old widget.
	 */

	QWidget *ccw = NULL;
	if (layout->count() > 1) {
		ccw = layout->itemAt(0)->widget();
	}

	if (w) {
		qvblVertical->insertWidget(0, w);
		if (ccw)
			ccw->hide();
		w->show();
		cwCurrentWidget = cw;
	}

	if (layout->count() > 2) {
		QLayoutItem *item = layout->takeAt(1);
		delete item;
	}

	NSWindow *window = qt_mac_window_for(this);
	ConfigDialogDelegate *delegate = [[window toolbar] delegate];
	[delegate selectItem: cw];
}

ConfigWidget *ConfigDialog::currentWidget() {
	return cwCurrentWidget;
}

void ConfigDialog::on_widgetSelected(ConfigWidget *cw) {
	setCurrentWidget(cw);
}

void ConfigDialog::updateExpert(bool b) {
	removeMacToolbar();
	setupMacToolbar(b);

	ConfigWidget *cw = currentWidget();
	/* If there's no 'current' widget (i.e. this is the first time the dialog is shown),
	 * simply use the first available widget. */
	if (cw == NULL)
		cw = *(qmWidgets.begin());

	setCurrentWidget(cw);
}

void ConfigDialog::apply() {
	foreach(ConfigWidget *cw, qmWidgets)
		cw->save();

	boost::weak_ptr<AudioInput> wai(g.ai);
	boost::weak_ptr<AudioOutput> wao(g.ao);

	g.ai.reset();
	g.ao.reset();

	while (! wai.expired() || ! wao.expired()) {
		// Where is QThread::yield() ?
	}

	g.s = s;

	foreach(ConfigWidget *cw, qmWidgets)
		cw->accept();

	g.ai = AudioInputRegistrar::newFromChoice(g.s.qsAudioInput);
	if (g.ai)
		g.ai->start(QThread::HighestPriority);
	g.ao = AudioOutputRegistrar::newFromChoice(g.s.qsAudioOutput);
	if (g.ao)
		g.ao->start(QThread::HighPriority);

	// They might have changed their keys.
	g.iPushToTalk = 0;
}

void ConfigDialog::accept() {
	apply();
	g.s.qbaConfigGeometry=saveGeometry();
	QDialog::accept();
}
