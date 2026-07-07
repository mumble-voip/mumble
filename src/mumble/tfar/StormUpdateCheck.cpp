// MUMBLE-TFAR

#include "StormUpdateCheck.h"

#include "../Global.h"
#include "../Log.h"
#include "../MainWindow.h"

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QProcess>
#include <QtCore/QSettings>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QtWidgets/QMessageBox>

#include "StormBranding.h"

static QList< int > parseVersion(QString tag) {
    if (tag.startsWith(QLatin1Char('v'), Qt::CaseInsensitive))
        tag.remove(0, 1);
    QList< int > parts;
    for (const QString &part : tag.split(QLatin1Char('.'))) {
        bool ok       = false;
        const int num = part.toInt(&ok);
        parts.append(ok ? num : 0);
    }
    return parts;
}

static bool isNewer(const QList< int > &remote, const QList< int > &local) {
    const int count = qMax(remote.size(), local.size());
    for (int i = 0; i < count; ++i) {
        const int r = i < remote.size() ? remote[i] : 0;
        const int l = i < local.size() ? local[i] : 0;
        if (r != l)
            return r > l;
    }
    return false;
}

bool StormUpdateCheck::autoUpdateEnabled() {
    QSettings settings(QLatin1String("mumble-tfar"), QLatin1String("TFAR"));
    return settings.value(QLatin1String("autoUpdate"), true).toBool();
}

void StormUpdateCheck::setAutoUpdateEnabled(bool enabled) {
    QSettings settings(QLatin1String("mumble-tfar"), QLatin1String("TFAR"));
    settings.setValue(QLatin1String("autoUpdate"), enabled);
}

void StormUpdateCheck::checkForUpdates(bool verbose) {
    new StormUpdateCheck(verbose);
}

StormUpdateCheck::StormUpdateCheck(bool verbose, QObject *parent) : QObject(parent), m_verbose(verbose) {
    QNetworkAccessManager *nam = Global::get().nam;
    if (!nam) {
        deleteLater();
        return;
    }

    QNetworkRequest request(QUrl(QLatin1String(STORM_RELEASES_API)));
    request.setRawHeader("Accept", "application/vnd.github+json");
    request.setRawHeader("User-Agent", "mumble-tfar/" STORM_TFAR_VERSION);
    request.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);

    m_reply = nam->get(request);
    connect(m_reply, &QNetworkReply::finished, this, &StormUpdateCheck::onMetadataReceived);
}

void StormUpdateCheck::logInfo(const QString &message) {
    if (Global::get().l)
        Global::get().l->log(Log::Information, message);
}

void StormUpdateCheck::logWarning(const QString &message) {
    if (Global::get().l)
        Global::get().l->log(Log::Warning, message);
}

void StormUpdateCheck::onMetadataReceived() {
    m_reply->deleteLater();

    if (m_reply->error() != QNetworkReply::NoError) {
        if (m_verbose)
            logWarning(tr("TFAR: update check failed: %1").arg(m_reply->errorString()));
        deleteLater();
        return;
    }

    const QJsonObject release = QJsonDocument::fromJson(m_reply->readAll()).object();
    const QString tag         = release.value(QLatin1String("tag_name")).toString();
    if (tag.isEmpty()) {
        if (m_verbose)
            logWarning(tr("TFAR: update check failed: unexpected answer from GitHub."));
        deleteLater();
        return;
    }

    if (!isNewer(parseVersion(tag), parseVersion(QLatin1String(STORM_TFAR_VERSION)))) {
        if (m_verbose)
            logInfo(tr("TFAR: you are running the latest version (%1).").arg(QLatin1String(STORM_TFAR_VERSION)));
        deleteLater();
        return;
    }
    m_newVersion = tag;

    // Find the installer asset ("mumble_client-*.exe", fallback: the MSI).
    const QJsonArray assets = release.value(QLatin1String("assets")).toArray();
    for (const QJsonValue &value : assets) {
        const QJsonObject asset = value.toObject();
        const QString name      = asset.value(QLatin1String("name")).toString();
        if (name.startsWith(QLatin1String("mumble_client-"))
            && (name.endsWith(QLatin1String(".exe")) || name.endsWith(QLatin1String(".msi")))) {
            const bool isExe = name.endsWith(QLatin1String(".exe"));
            if (m_assetName.isEmpty() || isExe) {
                m_assetName = name;
                m_assetUrl  = QUrl(asset.value(QLatin1String("browser_download_url")).toString());
            }
            if (isExe)
                break;
        }
    }

    const QString releaseLink =
        tr("TFAR: a new version of %1 is available: <b>%2</b> (installed: %3). <a href=\"%4\">Download</a>")
            .arg(QLatin1String(STORM_APP_NAME), m_newVersion.toHtmlEscaped(), QLatin1String(STORM_TFAR_VERSION),
                 QLatin1String(STORM_RELEASES_URL));

    if (m_assetUrl.isEmpty() || (!autoUpdateEnabled() && !m_verbose)) {
        // No installer asset or auto-update disabled — just announce the release.
        logInfo(releaseLink);
        deleteLater();
        return;
    }

    const QMessageBox::StandardButton answer = QMessageBox::question(
        Global::get().mw, tr("%1 — update available").arg(QLatin1String(STORM_APP_NAME)),
        tr("A new version of %1 is available: %2 (installed: %3).\n\n"
           "Download and install it now? %1 will close to run the installer.")
            .arg(QLatin1String(STORM_APP_NAME), m_newVersion, QLatin1String(STORM_TFAR_VERSION)),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

    if (answer != QMessageBox::Yes) {
        logInfo(releaseLink);
        deleteLater();
        return;
    }

    logInfo(tr("TFAR: downloading update %1...").arg(m_assetName.toHtmlEscaped()));

    QNetworkRequest request(m_assetUrl);
    request.setRawHeader("Accept", "application/octet-stream");
    request.setRawHeader("User-Agent", "mumble-tfar/" STORM_TFAR_VERSION);
    request.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);

    m_reply = Global::get().nam->get(request);
    connect(m_reply, &QNetworkReply::finished, this, &StormUpdateCheck::onInstallerDownloaded);
}

void StormUpdateCheck::onInstallerDownloaded() {
    m_reply->deleteLater();
    deleteLater();

    const QByteArray data = m_reply->readAll();
    if (m_reply->error() != QNetworkReply::NoError || data.isEmpty()) {
        logWarning(tr("TFAR: update download failed: %1. <a href=\"%2\">Download manually</a>")
                       .arg(m_reply->errorString(), QLatin1String(STORM_RELEASES_URL)));
        return;
    }

    const QString filePath = QDir::temp().absoluteFilePath(m_assetName);
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly) || file.write(data) != data.size()) {
        logWarning(tr("TFAR: could not save the update to %1.").arg(filePath.toHtmlEscaped()));
        return;
    }
    file.close();

    bool started = false;
    if (m_assetName.endsWith(QLatin1String(".msi"))) {
        started = QProcess::startDetached(QLatin1String("msiexec"),
                                          { QLatin1String("/i"), QDir::toNativeSeparators(filePath) });
    } else {
        started = QProcess::startDetached(filePath, {});
    }

    if (!started) {
        logWarning(tr("TFAR: could not start the installer %1.").arg(filePath.toHtmlEscaped()));
        return;
    }

    // Close Mumble so the installer can replace the files.
    if (Global::get().mw)
        QMetaObject::invokeMethod(Global::get().mw, "close", Qt::QueuedConnection);
}
