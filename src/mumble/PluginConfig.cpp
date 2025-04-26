// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "PluginConfig.h"

#include "Log.h"
#include "MainWindow.h"
#include "MumbleApplication.h"
#include "PluginInstaller.h"
#include "PluginManager.h"
#include "ServerHandler.h"
#include "Utils.h"
#include "WebFetch.h"

#include <QtCore/QDir>
#include <QtCore/QStandardPaths>
#include <QtCore/QUrl>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include "Global.h"

const QString PluginConfig::name = QLatin1String("PluginConfig");

static ConfigWidget *PluginConfigDialogNew(Settings &st) {
	return new PluginConfig(st);
}

static ConfigRegistrar registrarPluginConfig(5000, PluginConfigDialogNew);


PluginConfig::PluginConfig(Settings &st) : ConfigWidget(st) {
	setupUi(this);

	qtwPlugins->header()->setSectionResizeMode(0, QHeaderView::Stretch);
	qtwPlugins->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
	qtwPlugins->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
	qtwPlugins->header()->setSectionResizeMode(3, QHeaderView::ResizeToContents);

	qtwPlugins->headerItem()->setData(0, Qt::AccessibleTextRole, tr("Plugin name"));
	qtwPlugins->headerItem()->setData(1, Qt::AccessibleTextRole, tr("Plugin enabled checkbox"));
	qtwPlugins->headerItem()->setData(2, Qt::AccessibleTextRole, tr("Plugin positional audio permission checkbox"));
	qtwPlugins->headerItem()->setData(3, Qt::AccessibleTextRole,
									  tr("Plugin keyboard event listen permission checkbox"));

	qpbUnload->setEnabled(false);

	refillPluginList();
}

QString PluginConfig::title() const {
	return tr("Plugins");
}

const QString &PluginConfig::getName() const {
	return PluginConfig::name;
}

QIcon PluginConfig::icon() const {
	return QIcon(QLatin1String("skin:config_plugin.png"));
}

void PluginConfig::load(const Settings &r) {
	loadCheckBox(qcbTransmit, r.bTransmitPosition);
}

void PluginConfig::on_qpbInstallPlugin_clicked() {
	QString pluginFile = QFileDialog::getOpenFileName(this, tr("Install plugin..."), QDir::homePath());

	if (pluginFile.isEmpty()) {
		return;
	}

	try {
		PluginInstaller installer(pluginFile, this);
		if (installer.exec() == QDialog::Accepted) {
			// Reload plugins so the new one actually shows up
			on_qpbReload_clicked();

			QMessageBox::information(this, "Mumble", tr("The plugin was installed successfully"), QMessageBox::Ok,
									 QMessageBox::NoButton);
		}
	} catch (const PluginInstallException &e) {
		QMessageBox::critical(this, "Mumble", e.getMessage(), QMessageBox::Ok, QMessageBox::NoButton);
	}
}

void PluginConfig::save() const {
	s.bTransmitPosition = qcbTransmit->isChecked();
	s.qhPluginSettings.clear();

	if (!s.bTransmitPosition) {
		// Make sure that if posData is currently running, it gets reset
		// The setting will prevent the system from reactivating
		Global::get().pluginManager->unlinkPositionalData();
	}

	constexpr int ENABLE_COL           = 1;
	constexpr int POSITIONAL_DATA_COL  = 2;
	constexpr int KEYBOARD_MONITOR_COL = 3;

	QList< QTreeWidgetItem * > list = qtwPlugins->findItems(QString(), Qt::MatchContains);
	for (QTreeWidgetItem *i : list) {
		bool enable                    = (i->checkState(ENABLE_COL) == Qt::Checked);
		bool positionalDataEnabled     = (i->checkState(POSITIONAL_DATA_COL) == Qt::Checked);
		bool keyboardMonitoringEnabled = (i->checkState(KEYBOARD_MONITOR_COL) == Qt::Checked);

		const_plugin_ptr_t plugin = pluginForItem(i);
		if (plugin) {
			// insert plugin to settings
			Global::get().pluginManager->enablePositionalDataFor(plugin->getID(), positionalDataEnabled);
			Global::get().pluginManager->allowKeyboardMonitoringFor(plugin->getID(), keyboardMonitoringEnabled);

			if (enable) {
				if (Global::get().pluginManager->loadPlugin(plugin->getID())) {
					// potentially deactivate plugin features
					// A plugin's feature is considered to be enabled by default after loading. Thus we only need to
					// deactivate the ones we don't want
					uint32_t featuresToDeactivate = MUMBLE_FEATURE_NONE;
					const uint32_t pluginFeatures = plugin->getFeatures();

					if (!positionalDataEnabled && (pluginFeatures & MUMBLE_FEATURE_POSITIONAL)) {
						// deactivate this feature only if it is available in the first place
						featuresToDeactivate |= MUMBLE_FEATURE_POSITIONAL;
					}

					if (featuresToDeactivate != MUMBLE_FEATURE_NONE) {
						uint32_t remainingFeatures =
							Global::get().pluginManager->deactivateFeaturesFor(plugin->getID(), featuresToDeactivate);

						if (remainingFeatures != MUMBLE_FEATURE_NONE) {
							Global::get().l->log(Log::Warning,
												 tr("Unable to deactivate all requested features for plugin \"%1\"")
													 .arg(plugin->getName()));
						}
					}
				} else {
					// loading failed
					enable = false;
					Global::get().l->log(Log::Warning, tr("Unable to load plugin \"%1\"").arg(plugin->getName()));
				}
			} else {
				Global::get().pluginManager->unloadPlugin(plugin->getID());
			}

			QString pluginKey = QLatin1String(
				QCryptographicHash::hash(plugin->getFilePath().toUtf8(), QCryptographicHash::Sha1).toHex());
			s.qhPluginSettings.insert(
				pluginKey, { plugin->getFilePath(), enable, positionalDataEnabled, keyboardMonitoringEnabled });
		}
	}
}

const_plugin_ptr_t PluginConfig::pluginForItem(QTreeWidgetItem *i) const {
	if (i) {
		return Global::get().pluginManager->getPlugin(i->data(0, Qt::UserRole).toUInt());
	}

	return nullptr;
}

void PluginConfig::on_qpbConfig_clicked() {
	const_plugin_ptr_t plugin = pluginForItem(qtwPlugins->currentItem());

	if (plugin) {
		if (!plugin->showConfigDialog(this)) {
			// if the plugin doesn't support showing such a dialog, we'll show a default one
			QMessageBox::information(this, QLatin1String("Mumble"), tr("Plugin has no configure function."),
									 QMessageBox::Ok, QMessageBox::NoButton);
		}
	}
}

void PluginConfig::on_qpbAbout_clicked() {
	const_plugin_ptr_t plugin = pluginForItem(qtwPlugins->currentItem());

	if (plugin) {
		if (!plugin->showAboutDialog(this)) {
			// if the plugin doesn't support showing such a dialog, we'll show a default one
			QMessageBox::information(this, QLatin1String("Mumble"), tr("Plugin has no about function."),
									 QMessageBox::Ok, QMessageBox::NoButton);
		}
	}
}

void PluginConfig::on_qpbReload_clicked() {
	Global::get().pluginManager->rescanPlugins();
	refillPluginList();
}

void PluginConfig::on_qpbUnload_clicked() {
	QTreeWidgetItem *currentItem = qtwPlugins->currentItem();
	if (!currentItem) {
		return;
	}

	const_plugin_ptr_t plugin = pluginForItem(currentItem);
	if (!plugin) {
		return;
	}

	if (Global::get().pluginManager->clearPlugin(plugin->getID())) {
		// Plugin was successfully cleared
		currentItem = qtwPlugins->takeTopLevelItem(qtwPlugins->indexOfTopLevelItem(currentItem));

		delete currentItem;
	} else {
		qWarning("PluginConfig.cpp: Failed to delete unloaded plugin entry");
	}
}

void PluginConfig::refillPluginList() {
	qtwPlugins->clear();

	// get plugins already sorted according to their name
	const QVector< const_plugin_ptr_t > plugins = Global::get().pluginManager->getPlugins(true);

	foreach (const_plugin_ptr_t currentPlugin, plugins) {
		QTreeWidgetItem *i = new QTreeWidgetItem(qtwPlugins);
		i->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
		i->setCheckState(1, currentPlugin->isLoaded() ? Qt::Checked : Qt::Unchecked);

		if (currentPlugin->getFeatures() & MUMBLE_FEATURE_POSITIONAL) {
			i->setCheckState(2, currentPlugin->isPositionalDataEnabled() ? Qt::Checked : Qt::Unchecked);
			i->setToolTip(2, tr("Whether the positional audio feature of this plugin should be enabled"));
		} else {
			i->setToolTip(2, tr("This plugin does not provide support for positional audio"));
		}

		i->setCheckState(3, currentPlugin->isKeyboardMonitoringAllowed() ? Qt::Checked : Qt::Unchecked);
		i->setToolTip(3, tr("Whether this plugin has the permission to be listening to all keyboard events that occur "
							"while Mumble has focus"));

		i->setText(0, currentPlugin->getName());
		i->setToolTip(0, currentPlugin->getDescription().toHtmlEscaped());
		i->setToolTip(1, tr("Whether this plugin should be enabled"));
		i->setData(0, Qt::UserRole, currentPlugin->getID());

		on_qtwPlugins_itemChanged(i, 1);
		on_qtwPlugins_itemChanged(i, 2);
		on_qtwPlugins_itemChanged(i, 3);
	}

	qtwPlugins->setCurrentItem(qtwPlugins->topLevelItem(0));
	on_qtwPlugins_currentItemChanged(qtwPlugins->topLevelItem(0), NULL);
}

void PluginConfig::on_qtwPlugins_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *) {
	const_plugin_ptr_t plugin = pluginForItem(current);

	if (plugin) {
		qpbAbout->setEnabled(plugin->providesAboutDialog());

		qpbConfig->setEnabled(plugin->providesConfigDialog());

		qpbUnload->setEnabled(true);
	} else {
		qpbAbout->setEnabled(false);
		qpbConfig->setEnabled(false);
		qpbUnload->setEnabled(false);
	}
}

void PluginConfig::on_qtwPlugins_itemChanged(QTreeWidgetItem *item, int column) {
	const_plugin_ptr_t plugin = pluginForItem(item);

	if (!plugin) {
		return;
	}

	switch (column) {
		case 1:
		case 3:
			item->setData(column, Qt::AccessibleDescriptionRole,
						  item->checkState(column) == Qt::Checked ? tr("checked") : tr("unchecked"));
			break;
		case 2:
			if (plugin->getFeatures() & MUMBLE_FEATURE_POSITIONAL) {
				item->setData(column, Qt::AccessibleDescriptionRole,
							  item->checkState(column) == Qt::Checked ? tr("checked") : tr("unchecked"));
			} else {
				item->setData(column, Qt::AccessibleDescriptionRole, tr("Not available"));
			}
			break;
	}
}
