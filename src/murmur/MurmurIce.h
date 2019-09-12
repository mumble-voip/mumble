// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifdef USE_ICE
#ifndef MUMBLE_MURMUR_MURMURICE_H_
#define MUMBLE_MURMUR_MURMURICE_H_

#include <QtCore/QtGlobal>

#ifdef Q_OS_WIN
// To prevent <windows.h> (included by Ice) from including <winsock.h>.
# define WIN32_LEAN_AND_MEAN
#endif

#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QMutex>
#include <QtCore/QObject>
#include <QtCore/QWaitCondition>
#include <QtNetwork/QSslCertificate>

#include "MurmurI.h"

class Channel;
class Server;
class User;
struct TextMessage;

class MurmurIce : public QObject {
		friend class MurmurLocker;
		Q_OBJECT;
	protected:
		int count;
		QMutex qmEvent;
		QWaitCondition qwcEvent;
		void customEvent(QEvent *evt);
		void badMetaProxy(const ::Murmur::MetaCallbackPrx &prx);
		void badServerProxy(const ::Murmur::ServerCallbackPrx &prx, const ::Server* server);
		void badAuthenticator(::Server *);
		QList< ::Murmur::MetaCallbackPrx> qlMetaCallbacks;
		QMap<int, QList< ::Murmur::ServerCallbackPrx> > qmServerCallbacks;
		QMap<int, QMap<int, QMap<QString, ::Murmur::ServerContextCallbackPrx> > > qmServerContextCallbacks;
		QMap<int, ::Murmur::ServerAuthenticatorPrx> qmServerAuthenticator;
		QMap<int, ::Murmur::ServerUpdatingAuthenticatorPrx> qmServerUpdatingAuthenticator;
	public:
		Ice::CommunicatorPtr communicator;
		Ice::ObjectAdapterPtr adapter;
		MurmurIce();
		~MurmurIce();

		void addMetaCallback(const ::Murmur::MetaCallbackPrx& prx);
		void removeMetaCallback(const ::Murmur::MetaCallbackPrx& prx);
		void addServerCallback(const ::Server* server, const ::Murmur::ServerCallbackPrx& prx);
		void removeServerCallback(const ::Server* server, const ::Murmur::ServerCallbackPrx& prx);
		void removeServerCallbacks(const ::Server* server);
		void addServerContextCallback(const ::Server* server, int session_id, const QString& action, const ::Murmur::ServerContextCallbackPrx& prx);
		const QMap< int, QMap<QString, ::Murmur::ServerContextCallbackPrx> > getServerContextCallbacks(const ::Server* server) const;
		void removeServerContextCallback(const ::Server* server, int session_id, const QString& action);
		void setServerAuthenticator(const ::Server* server, const ::Murmur::ServerAuthenticatorPrx& prx);
		const ::Murmur::ServerAuthenticatorPrx getServerAuthenticator(const ::Server* server) const;
		void removeServerAuthenticator(const ::Server* server);
		void setServerUpdatingAuthenticator(const ::Server* server, const ::Murmur::ServerUpdatingAuthenticatorPrx& prx);
		const ::Murmur::ServerUpdatingAuthenticatorPrx getServerUpdatingAuthenticator(const ::Server* server) const;
		void removeServerUpdatingAuthenticator(const ::Server* server);

	public slots:
		void started(Server *);
		void stopped(Server *);

		void authenticateSlot(int &res, QString &uname, int sessionId, const QList<QSslCertificate> &certlist, const QString &certhash, bool certstrong, const QString &pw);
		void registerUserSlot(int &res, const QMap<int, QString> &);
		void unregisterUserSlot(int &res, int id);
		void getRegisteredUsersSlot(const QString &filter, QMap<int, QString> &res);
		void getRegistrationSlot(int &, int, QMap<int, QString> &);
		void setInfoSlot(int &, int, const QMap<int, QString> &);
		void setTextureSlot(int &res, int id, const QByteArray &texture);
		void nameToIdSlot(int &res, const QString &name);
		void idToNameSlot(QString &res, int id);
		void idToTextureSlot(QByteArray &res, int id);

		void userStateChanged(const User *p);
		void userTextMessage(const User *p, const TextMessage &);
		void userConnected(const User *p);
		void userDisconnected(const User *p);

		void channelStateChanged(const Channel *c);
		void channelCreated(const Channel *c);
		void channelRemoved(const Channel *c);

		void contextAction(const User *, const QString &, unsigned int, int);
};
#endif
#endif
