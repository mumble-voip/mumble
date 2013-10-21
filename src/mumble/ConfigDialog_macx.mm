/* Copyright (C) 2005-2011, Thorvald Natvig <thorvald@natvig.com>
   Copyright (C) 2009-2011, Mikkel Krautz <mikkel@krautz.dk>

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

#include "ConfigDialog_macx.h"
#include "AudioInput.h"
#include "AudioOutput.h"

#import "ConfigDialogDelegate.h"

#include "Global.h"

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#include <qpa/qplatformnativeinterface.h>

static NSWindow *qt_mac_window_for(QWidget *w) {
	QWindow *window = w->windowHandle();
	if (window) {	
		return static_cast<NSWindow *>(QGuiApplication::platformNativeInterface()->nativeResourceForWindow("nswindow", window));
	}
	return nil;
}
#endif

class QWidgetPrivate {
	public:
		void updateFrameStrut();
};

/*
 * This file implements the Mumble configuration dialog on Mac OS X.
 *
 * It implements a Mac OS X-style Preferences window (a toolbar at the
 * top of the window that blends in with the rest of the window, that
 * allows each item to be selected independently.)
 */

ConfigDialogMac::ConfigDialogMac(QWidget *p) : QDialog(p) {
	setupUi(this);

	s = g.s;

	/* Hide widgets we don't use on the Cocoa config dialog. */
	qcbExpert->hide();
	qlwIcons->hide();

	unsigned int idx = 0;
	ConfigWidgetNew cwn;
	foreach(cwn, *ConfigRegistrar::c_qmNew) {
		addPage(cwn(s), idx++);
	}

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

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
	QTimer::singleShot(0, this, SLOT(delayedInit()));
#else
	delayedInit();
#endif
}

void ConfigDialogMac::delayedInit() {
	updateExpert(g.s.bExpert);

	if (! g.s.qbaConfigGeometry.isEmpty())
		restoreGeometry(g.s.qbaConfigGeometry);
}

void ConfigDialogMac::addPage(ConfigWidget *cw, unsigned int idx) {
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

ConfigDialogMac::~ConfigDialogMac() {
	foreach(QWidget *qw, qhPages)
		delete qw;

	removeMacToolbar();
}

void ConfigDialogMac::on_pageButtonBox_clicked(QAbstractButton *b) {
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
				def.bExpert = g.s.bExpert;
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

void ConfigDialogMac::on_dialogButtonBox_clicked(QAbstractButton *b) {
	switch (dialogButtonBox->standardButton(b)) {
		case QDialogButtonBox::Apply: {
				apply();
				break;
			}
		default:
			break;
	}
}

void ConfigDialogMac::setupMacToolbar(bool expert) {
	this->winId();
	NSWindow *window = qt_mac_window_for(this);

	/* Allocate a NSToolbar for our Config Dialog. Most programs on Mac OS X use Preferences dialogs
	   with a toolbar on top, so this makes Mumble fit in well with native Mac apps.

	   The identifier string is simply an unique string for this particular toolbar. The OS will graphically
	   synchronize toolbars with the same identifier, so that if multiple NSToolbars with the same identifier
	   are used within the same application, they all stay in sync automatically. */
	NSToolbar *toolbar = [[NSToolbar alloc] initWithIdentifier:@"MumbleConfigDialog"];
	[toolbar setDisplayMode:NSToolbarDisplayModeIconAndLabel];
	[toolbar setSizeMode:NSToolbarSizeModeRegular];
	[toolbar setAllowsUserCustomization:NO];
	[toolbar setAutosavesConfiguration:NO];

	// We explicitly don't autorelease here. We don't really want to have to store a reference
	// to the delegate inside ConfigDialogMac's header. Instead, we'll just make sure to release
	// the delegate when removing the Mac toolbar. (See ::removeMacToolbar)
	ConfigDialogDelegate *delegate = [[ConfigDialogDelegate alloc] initWithConfigDialog:this
	                                                               andToolbar:toolbar
	                                                               andWidgetMap:&qmWidgets
	                                                               inExpertMode:expert];
	[toolbar setDelegate:delegate];
	[window setToolbar:toolbar];

	/* Hack alert: Qt doesn't export its Cocoa helper utilities, so this is the best we
	 * can do to make the window the right size after setting up a NSToolbar manually. */
	qt_widget_private(reinterpret_cast<QWidget *>(this))->updateFrameStrut();
}

void ConfigDialogMac::removeMacToolbar() {
	NSWindow *window = qt_mac_window_for(this);
	NSToolbar *toolbar = [window toolbar];

	[[toolbar delegate] release];
	[toolbar setDelegate:nil];
	[toolbar release];
}

void ConfigDialogMac::on_widgetSelected(ConfigWidget *cw) {
	QWidget *w = qhPages.value(cw);
	if (w) {
		setWindowTitle(cw->title());
		qswPages->setCurrentWidget(w);
	}
}

void ConfigDialogMac::updateExpert(bool b) {
	removeMacToolbar();
	setupMacToolbar(b);

	ConfigWidget *cw = qobject_cast<ConfigWidget *>(qswPages->currentWidget());
	if (! cw) {
		QScrollArea *qsa = qobject_cast<QScrollArea *>(qswPages->currentWidget());
		if (qsa)
			cw = qobject_cast<ConfigWidget *>(qsa->widget());
	}

	if (cw) {
		// Did our current widget become invisible when we switched to expert mode?
		if (! cw->expert(b))
			cw = qmWidgets.value(0);

		on_widgetSelected(cw);
		NSWindow *window = qt_mac_window_for(this);
		ConfigDialogDelegate *delegate = [[window toolbar] delegate];
		[delegate selectItem: cw];
	}
}

void ConfigDialogMac::apply() {
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

	NSWindow *window = qt_mac_window_for(this);
	ConfigDialogDelegate *delegate = [[window toolbar] delegate];
	g.s.bExpert = [delegate expertMode];

	Audio::start();
}

void ConfigDialogMac::accept() {
	apply();
	g.s.qbaConfigGeometry=saveGeometry();
	QDialog::accept();
}

void ConfigDialogMac::on_qlwIcons_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous) {
}

void ConfigDialogMac::on_qcbExpert_clicked(bool) {
}

