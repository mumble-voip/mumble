// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_PLUGINUPDATER_H_
#define MUMBLE_MUMBLE_PLUGINUPDATER_H_

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QtCore/QMutex>
#include <QtCore/QUrl>
#include <QtCore/QVector>

#include <atomic>
#include <limits>

#include "Plugin.h"
#include "ui_PluginUpdater.h"

/// A helper struct to store a pair of a CheckBox and a label corresponding to
/// a single plugin.
struct UpdateWidgetPair {
	QCheckBox *pluginCheckBox;
	QLabel *urlLabel;
};

/// A helper struct to store a pair of a plugin ID  and an URL corresponding to
/// the same plugin.
struct UpdateEntry {
	plugin_id_t pluginID = std::numeric_limits< plugin_id_t >::max();
	QUrl updateURL;
	QString fileName;
	int redirects = 0;
};

/// A class designed for managing plugin updates. At the same time this also represents
/// a Dialog that can be used to prompt the user whether certain updates should be updated.
class PluginUpdater : public QDialog, public Ui::PluginUpdater {
private:
	Q_OBJECT
	Q_DISABLE_COPY(PluginUpdater)

protected:
	/// An atomic flag indicating whether the plugin update has been interrupted. It is used
	/// to exit some loops in different threads before they are done.
	std::atomic< bool > m_wasInterrupted;
	/// A mutex for m_pluginsToUpdate.
	QMutex m_dataMutex;
	/// A vector holding plugins that can be updated by storing a pluginID and the download URL
	/// in form of an UpdateEntry.
	QVector< UpdateEntry > m_pluginsToUpdate;
	/// The NetworkManager used to perform the downloading of plugins.
	QNetworkAccessManager m_networkManager;
	/// A vector of the UI elements created for the individual plugins (in form of UpdateWidgetPairs).
	/// NOTE: This vector may only be accessed from the UI thread this dialog is living in!
	QVector< UpdateWidgetPair > m_pluginUpdateWidgets;

	/// Populates the UI with plugins that have been found to have an update available (through a call
	/// to checkForUpdates()).
	void populateUI();

public:
	/// Constructor
	///
	/// @param parent A pointer to the QWidget parent of this object
	PluginUpdater(QWidget *parent = nullptr);
	/// Destructor
	~PluginUpdater();

	// The maximum number of redirects to allow
	static constexpr int MAX_REDIRECTS = 10;

	/// Triggers an update check for all plugins that are currently recognized by Mumble. This is done
	/// in a non-blocking fashion (in another thread). Once all plugins have been checked and if there
	/// are updates available, the updatesAvailable signal is emitted.
	void checkForUpdates();
	/// Launches a Dialog that asks the user which of the plugins an update has been found for, shall be
	/// updated. If the user has selected at least selected one plugin and has accepted the dialog, this
	/// function will automatically call update().
	void promptAndUpdate();
	/// Starts the update process of the plugins. This is done asynchronously.
	void update();
public slots:
	/// Clears the UI from the widgets created for the individual plugins.
	void clearUI();
#if QT_VERSION >= QT_VERSION_CHECK(6, 7, 0)
	/// Slot triggered if the user changes the state of the selectAll CheckBox.
	void on_selectAll(Qt::CheckState checkState);
	/// Slot triggered if the user toggles the CheckBox for any individual plugin.
	void on_singleSelectionChanged(Qt::CheckState checkState);
#else
	/// Slot triggered if the user changes the state of the selectAll CheckBox.
	void on_selectAll(int checkState);
	/// Slot triggered if the user toggles the CheckBox for any individual plugin.
	void on_singleSelectionChanged(int checkState);
#endif
	/// Slot triggered when the dialog is being closed.
	void on_finished(int result);
	/// Slot that can be triggered to ask for the update process to be interrupted.
	void interrupt();
protected slots:
	/// Slot triggered once an update for a plugin has been downloaded.
	void on_updateDownloaded(QNetworkReply *reply);

signals:
	/// This signal is emitted once it has been determined that there are plugin updates available.
	void updatesAvailable();
	/// This signal is emitted once all plugin updates have been downloaded and processed.
	void updatingFinished();
	/// This signal is emitted every time the update process has been interrupted.
	void updateInterrupted();
};

#endif // MUMBLE_MUMBLE_PLUGINUPDATER_H_
