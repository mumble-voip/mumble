// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ConfigDialog.h"

#include "AudioInput.h"
#include "AudioOutput.h"
#include "widgets/EventFilters.h"
#include "Global.h"

#include <QScrollArea>
#include <QtCore/QMutexLocker>
#include <QtGui/QScreen>
#include <QtWidgets/QInputDialog>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QPushButton>


// init static member fields
QMutex ConfigDialog::s_existingWidgetsMutex;
QHash< QString, ConfigWidget * > ConfigDialog::s_existingWidgets;

ConfigDialog::ConfigDialog(QWidget *p) : QDialog(p) {
	setupUi(this);

	s = Global::get().s;

	unsigned int idx = 0;

	{
		for (ConfigWidget *cw : s_existingWidgets.values()) {
			qDebug() << "Add Config Dialog page " << cw->getName() << " " << cw;
			addPage(cw, ++idx);
		}
	}

	qDebug() << "Done creating dialog";

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

	QPushButton *resetButton = pageButtonBox->addButton(QDialogButtonBox::Reset);
	resetButton->setToolTip(tr("Undo changes for current page"));
	resetButton->setWhatsThis(
		tr("This button will revert any changes done on the current page to the most recent applied settings."));

	QPushButton *restoreButton = pageButtonBox->addButton(QDialogButtonBox::RestoreDefaults);
	restoreButton->setToolTip(tr("Restore defaults for current page"));
	restoreButton->setWhatsThis(
		tr("This button will restore the defaults for the settings on the current page. Other pages will not be "
		   "changed.<br />"
		   "To restore all settings to their defaults, you can press the \"Defaults (All)\" button."));

	QPushButton *restoreAllButton = pageButtonBox->addButton(tr("Defaults (All)"), QDialogButtonBox::ResetRole);
	restoreAllButton->setToolTip(tr("Restore all defaults"));
	restoreAllButton->setWhatsThis(tr("This button will restore the defaults for all settings."));
	restoreAllButton->installEventFilter(new OverrideTabOrderFilter(restoreAllButton, applyButton));

	if (!Global::get().s.qbaConfigGeometry.isEmpty()) {
#ifdef USE_OVERLAY
		if (!Global::get().ocIntercept)
#endif
			restoreGeometry(Global::get().s.qbaConfigGeometry);
	}

	m_profileMenu = new QMenu(this);

	m_profileAddAction = new QAction(tr("&Add"), this);
	QObject::connect(m_profileAddAction, &QAction::triggered, this, &ConfigDialog::profile_add);
	m_profileMenu->addAction(m_profileAddAction);

	m_profileRenameAction = new QAction(tr("&Rename"), this);
	QObject::connect(m_profileRenameAction, &QAction::triggered, this, &ConfigDialog::profile_rename);
	m_profileMenu->addAction(m_profileRenameAction);

	m_profileDeleteAction = new QAction(tr("&Delete"), this);
	QObject::connect(m_profileDeleteAction, &QAction::triggered, this, &ConfigDialog::profile_delete);
	m_profileMenu->addAction(m_profileDeleteAction);

	qtbProfileActions->setMenu(m_profileMenu);

	QObject::connect(qcbProfiles, &QComboBox::currentIndexChanged, this, &ConfigDialog::profile_selected);

	updateProfileList();
	updateTabOrder();
	qlwIcons->setFocus();
}

void ConfigDialog::addPage(ConfigWidget *cw, unsigned int idx) {
	int w = INT_MAX, h = INT_MAX;

	cw->s = s;

	const QList< QScreen * > screens = qApp->screens();
	for (int i = 0; i < screens.size(); ++i) {
		const QRect ds = screens[i]->availableGeometry();
		if (ds.isValid()) {
			w = qMin(w, ds.width());
			h = qMin(h, ds.height());
		}
	}

	QSize ms = cw->minimumSizeHint();
	cw->resize(ms);
	cw->setMinimumSize(ms);

	ms.rwidth() += 128;
	ms.rheight() += 192;
	if ((ms.width() > w) || (ms.height() > h)) {
		QScrollArea *qsa = new QScrollArea();
		qsa->setFrameShape(QFrame::NoFrame);
		qsa->setWidgetResizable(true);
		qsa->setWidget(cw);
		qsa->setFocusPolicy(Qt::NoFocus);
		qhPages.insert(cw, qsa);
		qswPages->addWidget(qsa);
	} else {
		qhPages.insert(cw, cw);
		qswPages->addWidget(cw);
	}
	qmWidgets.insert(idx, cw);
	cw->load(Global::get().s);
}

ConfigDialog::~ConfigDialog() {
	while (qswPages->count() > 0) {
		QWidget *qw = qswPages->currentWidget();
		qswPages->removeWidget(qw);
		qw->setParent(nullptr);

		ConfigWidget *cw = static_cast< ConfigWidget * >(qw);
		if (cw) {
			continue;
		}

		QScrollArea *qsa = static_cast< QScrollArea * >(qw);
		if (qsa) {
			qsa->takeWidget();
		}

		delete qw;
	}
}

#include <QDebug>

void ConfigDialog::initializeConfigWidgets() {
	qDebug() << "Initialize ConfigWidgets";

	if (!s_existingWidgets.isEmpty()) {
		qDebug() << "Abort initialize";
		return;
	}

	for (ConfigWidgetNew cwn : *ConfigRegistrar::c_qmNew) {
		ConfigWidget *cw = cwn(Global::get().s);
		QMutexLocker lock(&s_existingWidgetsMutex);
		s_existingWidgets.insert(cw->getName(), cw);
		qDebug() << "Generate " << cw->getName();
	}

	qDebug() << "Done initializing widgets";
}

ConfigWidget *ConfigDialog::getConfigWidget(const QString &name) {
	QMutexLocker lock(&s_existingWidgetsMutex);

	return s_existingWidgets.value(name, nullptr);
}

void ConfigDialog::on_pageButtonBox_clicked(QAbstractButton *b) {
	ConfigWidget *conf = qobject_cast< ConfigWidget * >(qswPages->currentWidget());
	if (!conf) {
		QScrollArea *qsa = qobject_cast< QScrollArea * >(qswPages->currentWidget());
		if (qsa)
			conf = qobject_cast< ConfigWidget * >(qsa->widget());
	}
	if (!conf)
		return;
	switch (pageButtonBox->standardButton(b)) {
		case QDialogButtonBox::RestoreDefaults: {
			Settings def;
			conf->load(def);
			break;
		}
		case QDialogButtonBox::Reset: {
			conf->load(Global::get().s);
			break;
		}
		// standardButton returns NoButton for any custom buttons. The only custom button
		// in the pageButtonBox is the one for resetting all settings.
		case QDialogButtonBox::NoButton: {
			// Ask for confirmation before resetting **all** settings
			QMessageBox msgBox;
			msgBox.setIcon(QMessageBox::Question);
			msgBox.setText(QObject::tr("Reset all settings?"));
			msgBox.setInformativeText(QObject::tr("Do you really want to reset all settings (not only the ones "
												  "currently visible) to their default value?"));
			msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
			msgBox.setDefaultButton(QMessageBox::No);

			if (msgBox.exec() == QMessageBox::Yes) {
				Settings defaultSetting;
				for (ConfigWidget *cw : qmWidgets) {
					cw->load(defaultSetting);
				}
			}
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
		if (previous) {
			updateTabOrder();
		}
	}
}

void ConfigDialog::updateProfileList() {
	// Prevent changing the profile unintentionally while filling the ComboBox
	const QSignalBlocker blocker(qcbProfiles);

	qcbProfiles->clear();

	// Always sort the default profile before anything else
	qcbProfiles->addItem(tr("Default Profile"), Profiles::s_default_profile_name);

	Profiles &profiles = Global::get().profiles;

	std::vector< QString > profileNames;
	for (auto it = profiles.allProfiles.begin(); it != profiles.allProfiles.end(); ++it) {
		profileNames.push_back(it->first);
	}
	std::sort(profileNames.begin(), profileNames.end());
	for (const QString &profile : profileNames) {
		if (profile == Profiles::s_default_profile_name) {
			continue;
		}
		qcbProfiles->addItem(profile, profile);
	}

	qcbProfiles->setCurrentIndex(qcbProfiles->findData(profiles.activeProfileName));

	bool isDefault = qcbProfiles->currentData().toString() == Profiles::s_default_profile_name;
	m_profileRenameAction->setEnabled(!isDefault);
	m_profileDeleteAction->setEnabled(!isDefault);
}

void ConfigDialog::switchProfile(const QString &newProfile, bool saveActiveProfile) {
	Profiles &profiles = Global::get().profiles;

	if (saveActiveProfile) {
		profiles.allProfiles[profiles.activeProfileName] = Global::get().s;
	}
	Global::get().s.loadProfile(newProfile);
	s = Global::get().s;
	for (ConfigWidget *cw : s_existingWidgets.values()) {
		cw->load(s);
	}

	updateProfileList();
}

void ConfigDialog::profile_selected(int) {
	QString selectedProfile = qcbProfiles->currentData().toString();

	Profiles &profiles = Global::get().profiles;

	if (selectedProfile == profiles.activeProfileName) {
		return;
	}

	if (!profiles.allProfiles.contains(selectedProfile)) {
		return;
	}

	switchProfile(selectedProfile, true);
}

void ConfigDialog::profile_add() {
	Profiles &profiles = Global::get().profiles;

	bool ok;
	QString profileName =
		QInputDialog::getText(this, tr("Creating settings profile"), tr("Enter new settings profile name"),
							  QLineEdit::Normal, profiles.activeProfileName, &ok);

	if (!ok || profileName.isEmpty()) {
		return;
	}

	if (profiles.allProfiles.contains(profileName)) {
		QMessageBox::critical(this, tr("Creating settings profile"),
							  tr("A settings profile with this name already exists"));
		return;
	}

	// Instead of "resetting" when creating a new profile, use the currently
	// (possibly not applied) settings for the new profile
	profiles.allProfiles[profiles.activeProfileName] = Global::get().s;
	apply();
	profiles.allProfiles[profileName] = Global::get().s;
	switchProfile(profileName, false);
}

void ConfigDialog::profile_rename() {
	QString oldProfileName = qcbProfiles->currentData().toString();

	if (oldProfileName == Profiles::s_default_profile_name) {
		return;
	}

	bool ok;
	QString profileName =
		QInputDialog::getText(this, tr("Renaming settings profile"), tr("Enter new settings profile name"),
							  QLineEdit::Normal, oldProfileName, &ok);

	if (!ok || profileName.isEmpty()) {
		return;
	}

	Profiles &profiles = Global::get().profiles;

	if (profiles.allProfiles.contains(profileName)) {
		QMessageBox::critical(this, tr("Renaming settings profile"),
							  tr("A settings profile with this name already exists"));
		return;
	}

	profiles.allProfiles[profileName] = Global::get().s;
	profiles.allProfiles.erase(oldProfileName);
	switchProfile(profileName, false);
}

void ConfigDialog::profile_delete() {
	QString oldProfileName = qcbProfiles->currentData().toString();

	if (oldProfileName == Profiles::s_default_profile_name) {
		return;
	}

	Profiles &profiles = Global::get().profiles;

	if (!profiles.allProfiles.contains(oldProfileName)) {
		return;
	}

	QMessageBox::StandardButton confirmation = QMessageBox::question(
		this, tr("Delete settings profile"),
		tr("Are you sure you want to permanently delete settings profile '%1'").arg(oldProfileName));

	if (confirmation != QMessageBox::Yes) {
		return;
	}

	profiles.allProfiles.erase(oldProfileName);
	switchProfile(Profiles::s_default_profile_name, false);
}

void ConfigDialog::updateTabOrder() {
	QPushButton *okButton         = dialogButtonBox->button(QDialogButtonBox::Ok);
	QPushButton *cancelButton     = dialogButtonBox->button(QDialogButtonBox::Cancel);
	QPushButton *applyButton      = dialogButtonBox->button(QDialogButtonBox::Apply);
	QPushButton *resetButton      = pageButtonBox->button(QDialogButtonBox::Reset);
	QPushButton *restoreButton    = pageButtonBox->button(QDialogButtonBox::RestoreDefaults);
	QPushButton *restoreAllButton = static_cast< QPushButton * >(pageButtonBox->buttons().last());

	QWidget *contentFocusWidget = qswPages;

	ConfigWidget *page;
	QScrollArea *qsa = qobject_cast< QScrollArea * >(qswPages->currentWidget());
	if (qsa) {
		page = qobject_cast< ConfigWidget * >(qsa->widget());
	} else {
		page = qobject_cast< ConfigWidget * >(qswPages->currentWidget());
	}

	if (page) {
		contentFocusWidget = page;
	}

	setTabOrder(cancelButton, okButton);
	setTabOrder(okButton, qcbProfiles);

	setTabOrder(qcbProfiles, qtbProfileActions);
	setTabOrder(qtbProfileActions, qlwIcons);

	setTabOrder(qlwIcons, contentFocusWidget);
	if (resetButton && restoreButton && restoreAllButton) {
		setTabOrder(contentFocusWidget, resetButton);
		setTabOrder(resetButton, restoreButton);
		setTabOrder(restoreButton, restoreAllButton);
		setTabOrder(restoreAllButton, applyButton);
	} else {
		setTabOrder(contentFocusWidget, applyButton);
	}
	setTabOrder(applyButton, cancelButton);
}

void ConfigDialog::updateListView() {
	QWidget *ccw         = qmIconWidgets.value(qlwIcons->currentItem());
	QListWidgetItem *sel = nullptr;

	qmIconWidgets.clear();
	qlwIcons->clear();

	QFontMetrics qfm(qlwIcons->font());
	int configNavbarWidth = 0;

	for (ConfigWidget *cw : qmWidgets) {
		configNavbarWidth = qMax(configNavbarWidth, qfm.horizontalAdvance(cw->title()));

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

	for (ConfigWidget *cw : qmWidgets) {
		cw->save();
	}

	Global::get().s = s;

	for (ConfigWidget *cw : qmWidgets) {
		cw->accept();
	}

	if (!Global::get().s.bAttenuateOthersOnTalk)
		Global::get().bAttenuateOthers = false;

	// They might have changed their keys.
	Global::get().iPushToTalk = 0;

	Audio::start();

	emit settingsAccepted();
}

void ConfigDialog::accept() {
	apply();

#ifdef USE_OVERLAY
	if (!Global::get().ocIntercept)
#endif
		Global::get().s.qbaConfigGeometry = saveGeometry();

	// Save settings to disk
	Global::get().s.save();

	QDialog::accept();
}
