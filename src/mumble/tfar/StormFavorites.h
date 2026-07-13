// MUMBLE-TFAR

#ifndef MUMBLE_MUMBLE_TFAR_STORMFAVORITES_H_
#define MUMBLE_MUMBLE_TFAR_STORMFAVORITES_H_

#include <QtCore/QObject>

class QNetworkReply;

/// Keeps the favorites list in sync with the community server list published
/// in the project repository ("favorites.json" in the repository root,
/// fetched via raw.githubusercontent.com).
///
/// Entries from the remote list are added to / updated in the favorites
/// database by name; entries that were added by an earlier sync and have
/// since disappeared from the remote list are removed again. Favorites the
/// user added himself are never touched. Credentials (username) that the
/// user configured on a synced entry are preserved.
class StormFavorites : public QObject {
	Q_OBJECT

public:
	/// Fires an asynchronous fetch + sync. Silent on any failure — the
	/// favorites database is simply left as it is (offline case).
	static void sync();

private slots:
	void onListReceived();

private:
	explicit StormFavorites(QObject *parent = nullptr);

	QNetworkReply *m_reply = nullptr;
};

#endif // MUMBLE_MUMBLE_TFAR_STORMFAVORITES_H_
