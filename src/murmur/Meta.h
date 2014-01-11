/* Copyright (C) 2005-2011, Thorvald Natvig <thorvald@natvig.com>

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

#ifndef MUMBLE_MURMUR_META_H_
#define MUMBLE_MURMUR_META_H_

#include <QtCore/QDir>
#include <QtCore/QList>
#include <QtCore/QUrl>
#include <QtCore/QVariant>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QSslCertificate>
#include <QtNetwork/QSslKey>
#ifdef Q_OS_WIN
#include <windows.h>
#endif

#include "Timer.h"

class Server;
class QSettings;

class MetaParams {
public:
	QDir qdBasePath;

	QList<QHostAddress> qlBind;
	unsigned short usPort;
	int iTimeout;
	int iMaxBandwidth;
	int iMaxUsers;
	int iMaxUsersPerChannel;
	int iDefaultChan;
	bool bRememberChan;
	int iMaxTextMessageLength;
	int iMaxImageMessageLength;
	int iOpusThreshold;
	int iChannelNestingLimit;
	bool bAllowHTML;
	QString qsPassword;
	QString qsWelcomeText;
	bool bCertRequired;

	int iBanTries;
	int iBanTimeframe;
	int iBanTime;

	QString qsDatabase;
	QString qsDBDriver;
	QString qsDBUserName;
	QString qsDBPassword;
	QString qsDBHostName;
	QString qsDBPrefix;
	QString qsDBOpts;
	int iDBPort;

	int iLogDays;

	int iObfuscate;
	bool bSendVersion;
	bool bAllowPing;

	QString qsDBus;
	QString qsDBusService;
	QString qsLogfile;
	QString qsPid;
	QString qsIceEndpoint;
	QString qsIceSecretRead, qsIceSecretWrite;

	QString qsRegName;
	QString qsRegPassword;
	QString qsRegHost;
	QString qsRegLocation;
	QUrl qurlRegWeb;
	bool bBonjour;

	QRegExp qrUserName;
	QRegExp qrChannelName;

	QSslCertificate qscCert;
	QSslKey qskKey;
	QByteArray qbaPassPhrase;

	QMap<QString, QString> qmConfig;

#ifdef Q_OS_UNIX
	unsigned int uiUid, uiGid;
	QString qsHome;
	QString qsName;
#endif

	QVariant qvSuggestVersion;
	QVariant qvSuggestPositional;
	QVariant qvSuggestPushToTalk;

	QSettings *qsSettings;

	MetaParams();
	~MetaParams();
	void read(QString fname = QString("murmur.ini"));

private:
		template <class T>
		T typeCheckedFromSettings(const QString &name, const T &variable);
};

class Meta : public QObject {
	private:
		Q_OBJECT;
		Q_DISABLE_COPY(Meta);
	public:
		static MetaParams mp;
		QHash<int, Server *> qhServers;
		QHash<QHostAddress, QList<Timer> > qhAttempts;
		QHash<QHostAddress, Timer> qhBans;
		QString qsOS, qsOSVersion, qsOSDisplayableVersion;
		Timer tUptime;

#ifdef Q_OS_WIN
		static HANDLE hQoS;
#endif

		Meta();
		~Meta();
		void bootAll();
		bool boot(int);
		bool banCheck(const QHostAddress &);
		void kill(int);
		void killAll();
		void getOSInfo();
		void connectListener(QObject *);
		static void getVersion(int &major, int &minor, int &patch, QString &string);
	signals:
		void started(Server *);
		void stopped(Server *);
};

extern Meta *meta;

#endif
