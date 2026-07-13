// MUMBLE-TFAR

#include "StormFavorites.h"

#include "../Database.h"
#include "../Global.h"

#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QSettings>
#include <QtCore/QStringList>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

#include "StormBranding.h"

void StormFavorites::sync() {
	new StormFavorites();
}

StormFavorites::StormFavorites(QObject *parent) : QObject(parent) {
	QNetworkAccessManager *nam = Global::get().nam;
	if (!nam || !Global::get().db) {
		deleteLater();
		return;
	}

	QNetworkRequest request(QUrl(QLatin1String(STORM_FAVORITES_URL)));
	request.setRawHeader("User-Agent", "mumble-tfar/" STORM_TFAR_VERSION);
	request.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);

	m_reply = nam->get(request);
	connect(m_reply, &QNetworkReply::finished, this, &StormFavorites::onListReceived);
}

void StormFavorites::onListReceived() {
	m_reply->deleteLater();
	deleteLater();

	if (m_reply->error() != QNetworkReply::NoError || !Global::get().db) {
		return;
	}

	// Accepted formats: a top-level array of entries, or an object with a
	// "favorites" array. Entry: { "name": ..., "host": ..., "port": 64738,
	// "username": optional, "url": optional }.
	const QJsonDocument doc = QJsonDocument::fromJson(m_reply->readAll());
	QJsonArray entries;
	if (doc.isArray()) {
		entries = doc.array();
	} else if (doc.isObject()) {
		entries = doc.object().value(QLatin1String("favorites")).toArray();
	}
	if (entries.isEmpty()) {
		return;
	}

	QSettings stormSettings(QLatin1String("mumble-tfar"), QLatin1String("TFAR"));
	const QStringList previouslyManaged = stormSettings.value(QLatin1String("remoteFavorites")).toStringList();

	QList< FavoriteServer > favorites = Global::get().db->getFavorites();
	QStringList nowManaged;
	bool changed = false;

	for (const QJsonValue &value : entries) {
		const QJsonObject entry = value.toObject();
		const QString name      = entry.value(QLatin1String("name")).toString().trimmed();
		const QString host      = entry.value(QLatin1String("host")).toString().trimmed();
		if (name.isEmpty() || host.isEmpty()) {
			continue;
		}
		const unsigned short port =
			static_cast< unsigned short >(entry.value(QLatin1String("port")).toInt(STORM_SERVER_PORT));
		const QString username = entry.value(QLatin1String("username")).toString();
		const QString url      = entry.value(QLatin1String("url")).toString();

		nowManaged << name;

		bool found = false;
		for (FavoriteServer &favorite : favorites) {
			if (favorite.qsName.compare(name, Qt::CaseInsensitive) != 0) {
				continue;
			}
			found = true;
			if (favorite.qsHostname != host || favorite.usPort != port
				|| (!url.isEmpty() && favorite.qsUrl != url)
				|| (!username.isEmpty() && favorite.qsUsername.isEmpty())) {
				favorite.qsHostname = host;
				favorite.usPort     = port;
				if (!url.isEmpty()) {
					favorite.qsUrl = url;
				}
				// Only fill in the username when the user has not set one.
				if (!username.isEmpty() && favorite.qsUsername.isEmpty()) {
					favorite.qsUsername = username;
				}
				changed = true;
			}
			break;
		}

		if (!found) {
			FavoriteServer favorite;
			favorite.qsName     = name;
			favorite.qsHostname = host;
			favorite.usPort     = port;
			favorite.qsUsername = username;
			favorite.qsUrl      = url;
			favorites << favorite;
			changed = true;
		}
	}

	// Entries added by an earlier sync that the remote list no longer
	// contains are removed again (user-created favorites are not touched).
	for (const QString &name : previouslyManaged) {
		if (nowManaged.contains(name, Qt::CaseInsensitive)) {
			continue;
		}
		for (qsizetype i = 0; i < favorites.size(); ++i) {
			if (favorites[i].qsName.compare(name, Qt::CaseInsensitive) == 0) {
				favorites.removeAt(i);
				changed = true;
				break;
			}
		}
	}

	if (changed) {
		Global::get().db->setFavorites(favorites);
	}
	stormSettings.setValue(QLatin1String("remoteFavorites"), nowManaged);
}
