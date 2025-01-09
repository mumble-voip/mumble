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
        // Make sure positional data is reset if it is currently running
        Global::get().pluginManager->unlinkPositionalData();
    }

    constexpr int ENABLE_COL           = 1;
    constexpr int POSITIONAL_DATA_COL  = 2;
    constexpr int KEYBOARD_MONITOR_COL = 3;

    QList<QTreeWidgetItem *> list = qtwPlugins->findItems(QString(), Qt::MatchContains);
    for (QTreeWidgetItem *i : list) {
        bool enable                    = (i->checkState(ENABLE_COL) == Qt::Checked);
        bool positionalDataEnabled     = (i->checkState(POSITIONAL_DATA_COL) == Qt::Checked);
        bool keyboardMonitoringEnabled = (i->checkState(KEYBOARD_MONITOR_COL) == Qt::Checked);

        const_plugin_ptr_t plugin = pluginForItem(i);
        if (plugin) {
            Global::get().pluginManager->enablePositionalDataFor(plugin->getID(), positionalDataEnabled);
            Global::get().pluginManager->allowKeyboardMonitoringFor(plugin->getID(), keyboardMonitoringEnabled);

            if (enable) {
                // Load plugin safely
                if (Global::get().pluginManager->loadPlugin(plugin->getID())) {
                    uint32_t featuresToDeactivate = MUMBLE_FEATURE_NONE;
                    const uint32_t pluginFeatures = plugin->getFeatures();

                    if (!positionalDataEnabled && (pluginFeatures & MUMBLE_FEATURE_POSITIONAL)) {
                        featuresToDeactivate |= MUMBLE_FEATURE_POSITIONAL;
                    }

                    if (featuresToDeactivate != MUMBLE_FEATURE_NONE) {
                        uint32_t remainingFeatures = Global::get().pluginManager->deactivateFeaturesFor(plugin->getID(), featuresToDeactivate);

                        if (remainingFeatures != MUMBLE_FEATURE_NONE) {
                            Global::get().l->log(Log::Warning,
                                                 tr("Unable to deactivate all requested features for plugin \"%1\"")
                                                     .arg(plugin->getName()));
                        }
                    }
                } else {
                    enable = false;
                    Global::get().l->log(Log::Warning, tr("Unable to load plugin \"%1\"").arg(plugin->getName()));
                }
            } else {
                // Safely unload plugin
                try {
                    if (!Global::get().pluginManager->unloadPlugin(plugin->getID())) {
                        Global::get().l->log(Log::Warning, tr("Failed to unload plugin \"%1\"").arg(plugin->getName()));
                    }
                } catch (const std::exception &e) {
                    Global::get().l->log(Log::Error, tr("Exception occurred while unloading plugin: %1").arg(e.what()));
                }
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
	const_plugin_ptr_t plugin = pluginForItem(item);
if (!plugin) {
    qWarning("Invalid plugin ID");
    return;
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
	QMutexLocker locker(&Global::get().pluginManager->mutex());

	refillPluginList();
}

void PluginConfig::on_qpbUnload_clicked() {
    QTreeWidgetItem *currentItem = qtwPlugins->currentItem();
    if (!currentItem) {
        qWarning() << "No item selected for unloading.";
        return;
    }

    const_plugin_ptr_t plugin = pluginForItem(currentItem);
    if (!plugin) {
        qWarning() << "Failed to resolve plugin for the selected item.";
        return;
    }

    try {
        if (Global::get().pluginManager->clearPlugin(plugin->getID())) {
            qtwPlugins->takeTopLevelItem(qtwPlugins->indexOfTopLevelItem(currentItem));
            delete currentItem;  // Ensure safe deletion
        } else {
            Global::get().l->log(Log::Warning, tr("Failed to clear plugin \"%1\"").arg(plugin->getName()));
        }
    } catch (const std::exception &e) {
        Global::get().l->log(Log::Error, tr("Exception during plugin clearing: %1").arg(e.what()));
    }

    // Ensure no dangling references remain
    Global::get().pluginManager->clearPluginReferences(plugin->getID());
}
void PluginConfig::refillPluginList() {
    qtwPlugins->clear();
    qtwPlugins->blockSignals(true);

    const auto plugins = Global::get().pluginManager->getPlugins(true);
    for (const auto &currentPlugin : plugins) {
        auto *item = new QTreeWidgetItem(qtwPlugins);
        item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        item->setCheckState(1, currentPlugin->isLoaded() ? Qt::Checked : Qt::Unchecked);

        if (currentPlugin->getFeatures() & MUMBLE_FEATURE_POSITIONAL) {
            item->setCheckState(2, currentPlugin->isPositionalDataEnabled() ? Qt::Checked : Qt::Unchecked);
            item->setToolTip(2, tr("Whether the positional audio feature of this plugin should be enabled"));
        } else {
            item->setToolTip(2, tr("This plugin does not provide support for positional audio"));
        }

        item->setCheckState(3, currentPlugin->isKeyboardMonitoringAllowed() ? Qt::Checked : Qt::Unchecked);
        item->setToolTip(3, tr("Whether this plugin has the permission to monitor keyboard events"));

        item->setText(0, currentPlugin->getName());
        item->setToolTip(0, currentPlugin->getDescription().toHtmlEscaped());
        item->setToolTip(1, tr("Whether this plugin should be enabled"));
        item->setData(0, Qt::UserRole, currentPlugin->getID());
    }

    qtwPlugins->blockSignals(false);
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
