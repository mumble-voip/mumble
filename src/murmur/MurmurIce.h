/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>

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

#ifdef USE_ICE
#ifndef _MURMURICE_H
#define _MURMURICE_H

#include "murmur_pch.h"

#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include "MurmurI.h"
#include "Server.h"

class MurmurIce : public QObject {
		friend class MurmurLocker;
		Q_OBJECT;
	protected:
		int count;
		QMutex qmEvent;
		QWaitCondition qwcEvent;
		void customEvent(QEvent *evt);
		void badMetaProxy(const ::Murmur::MetaCallbackPrx &prx);
		void badServerProxy(const ::Murmur::ServerCallbackPrx &prx, int id);
		void badAuthenticator(Server *);
	public:
		Ice::CommunicatorPtr communicator;
		Ice::ObjectAdapterPtr adapter;
		QList< ::Murmur::MetaCallbackPrx> qmMetaCallbacks;
		QMap<int, QList< ::Murmur::ServerCallbackPrx> > qmServerCallbacks;
		QMap<int, QMap<int, QMap<QString, ::Murmur::ServerContextCallbackPrx> > > qmServerContextCallbacks;
		QMap<int, ::Murmur::ServerAuthenticatorPrx> qmServerAuthenticator;
		QMap<int, ::Murmur::ServerUpdatingAuthenticatorPrx> qmServerUpdatingAuthenticator;
		MurmurIce();
		~MurmurIce();
	public slots:
		void started(Server *);
		void stopped(Server *);

		void authenticateSlot(int &res, QString &uname, const QList<QSslCertificate> &certlist, const QString &certhash, bool certstrong, const QString &pw);
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
		void userConnected(const User *p);
		void userDisconnected(const User *p);

		void channelStateChanged(const Channel *c);
		void channelCreated(const Channel *c);
		void channelRemoved(const Channel *c);

		void contextAction(const User *, const QString &, unsigned int, int);
};
#endif
#endif
