// MUMBLE-TFAR
// Update check + auto-update against the GitHub releases of the project
// repository (StormofTheGalaxy/mumble-tfar). Replaces Mumble's own version
// check, which would advertise vanilla Mumble builds without TFAR.
//
// Flow: fetch latest release metadata -> if newer: ask the user (unless
// auto-update is disabled — then only log a link) -> download the installer
// asset into the temp directory -> launch it and close Mumble.

#ifndef MUMBLE_MUMBLE_TFAR_STORMUPDATECHECK_H_
#define MUMBLE_MUMBLE_TFAR_STORMUPDATECHECK_H_

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QUrl>

class QNetworkReply;

class StormUpdateCheck : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY(StormUpdateCheck)

public:
    /// Queries the latest GitHub release; offers to download & install when a
    /// newer build is available. With verbose=true also reports "up to date"
    /// and errors (used by the manual "check for updates" actions).
    /// The object deletes itself when done. Must be called on the main thread.
    static void checkForUpdates(bool verbose);

    /// Whether the client may offer downloading updates automatically
    /// (persisted, toggled on the TFAR settings page).
    static bool autoUpdateEnabled();
    static void setAutoUpdateEnabled(bool enabled);

private slots:
    void onMetadataReceived();
    void onInstallerDownloaded();

private:
    explicit StormUpdateCheck(bool verbose, QObject *parent = nullptr);

    void logInfo(const QString &message);
    void logWarning(const QString &message);

    bool m_verbose;
    QString m_newVersion;
    QString m_assetName;
    QUrl m_assetUrl;
    QNetworkReply *m_reply = nullptr;
};

#endif // MUMBLE_MUMBLE_TFAR_STORMUPDATECHECK_H_
