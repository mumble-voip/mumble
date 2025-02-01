// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MURMUR_META_H_
#define MUMBLE_MURMUR_META_H_

#include "Timer.h"

#include "Version.h"

#ifdef Q_OS_WIN
#	include "win.h"
#endif

#include <QtCore/QDir>
#include <QtCore/QList>
#include <QtCore/QRegularExpression>
#include <QtCore/QUrl>
#include <QtCore/QVariant>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QSslCertificate>
#include <QtNetwork/QSslCipher>
#include <QtNetwork/QSslKey>

#include <memory>

class Server;
class QSettings;

class MetaParams {
public:
	QDir qdBasePath;

	QList< QHostAddress > qlBind;
	unsigned short usPort;
	int iTimeout;
	int iMaxBandwidth;
	unsigned int iMaxUsers;
	unsigned int iMaxUsersPerChannel;
	int iMaxListenersPerChannel;
	int iMaxListenerProxiesPerUser;
	unsigned int iDefaultChan;
	bool bRememberChan;
	int iRememberChanDuration;
	int iMaxTextMessageLength;
	int iMaxImageMessageLength;
	int iOpusThreshold;
	int iChannelNestingLimit;
	int iChannelCountLimit;
	/// If true the old SHA1 password hashing is used instead of PBKDF2
	bool legacyPasswordHash;
	/// Contains the default number of PBKDF2 iterations to use
	/// when hashing passwords. If the value loaded from config
	/// is <= 0 the value is loaded from the database and if not
	/// available there yet found by a benchmark.
	int kdfIterations;
	bool bAllowHTML;
	QString qsPassword;
	QString qsWelcomeText;
	QString qsWelcomeTextFile;
	bool bCertRequired;
	bool bForceExternalAuth;

	int iBanTries;
	int iBanTimeframe;
	int iBanTime;
	bool bBanSuccessful;

	QString qsDatabase;
	int iSQLiteWAL;
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

	QRegularExpression qrUserName;
	QRegularExpression qrChannelName;

	unsigned int iMessageLimit;
	unsigned int iMessageBurst;

	unsigned int iPluginMessageLimit;
	unsigned int iPluginMessageBurst;

	bool broadcastListenerVolumeAdjustments;

	QSslCertificate qscCert;
	QSslKey qskKey;

	/// qlIntermediates contains the certificates
	/// from PEM bundle pointed to by murmur.ini's
	/// sslCert option that do not match the key
	/// pointed to by murmur.ini's sslKey option.
	///
	/// Simply put: it contains any certificates
	/// that aren't the main certificate, or "leaf"
	/// certificate.
	QList< QSslCertificate > qlIntermediates;

	/// qlCA contains all certificates read from
	/// the PEM bundle pointed to by murmur.ini's
	/// sslCA option.
	QList< QSslCertificate > qlCA;

	/// qlCiphers contains the list of supported
	/// cipher suites.
	QList< QSslCipher > qlCiphers;

	QByteArray qbaDHParams;
	QByteArray qbaPassPhrase;
	QString qsCiphers;

	QMap< QString, QString > qmConfig;

#ifdef Q_OS_UNIX
	unsigned int uiUid, uiGid;
	QString qsHome;
	QString qsName;
#endif

	Version::full_t m_suggestVersion;
	QVariant qvSuggestPositional;
	QVariant qvSuggestPushToTalk;

	/// A flag indicating whether changes in groups should be logged
	bool bLogGroupChanges;
	/// A flag indicating whether changes in ACLs should be logged
	bool bLogACLChanges;

	/// A flag indicating whether recording is allowed on this server
	bool allowRecording;

	/// The number of seconds to keep rolling stats for per client
	unsigned int rollingStatsWindow;

	/// qsAbsSettingsFilePath is the absolute path to
	/// the murmur.ini used by this Meta instance.
	QString qsAbsSettingsFilePath;
	QSettings *qsSettings;

	MetaParams();
	~MetaParams();
	void read(QString fname = QString("murmur.ini"));

	/// Attempt to load SSL settings from murmur.ini.
	/// Returns true if successful. Returns false if
	/// the operation failed. On failure, the MetaParams
	/// object is left 100% intact.
	bool loadSSLSettings();

private:
	template< class ValueType, class ReturnType = ValueType >
	ReturnType typeCheckedFromSettings(const QString &name, const ValueType &variable, QSettings *settings = nullptr);
};

class Meta : public QObject {
private:
	Q_OBJECT
	Q_DISABLE_COPY(Meta)

public:
	static std::unique_ptr< MetaParams > mp;
	QHash< int, Server * > qhServers;
	QHash< QHostAddress, QList< Timer > > qhAttempts;
	QHash< QHostAddress, Timer > qhBans;
	QString qsOS, qsOSVersion;
	Timer tUptime;

#ifdef Q_OS_WIN
	static HANDLE hQoS;
#endif

	Meta();
	~Meta();

	/// reloadSSLSettings reloads Murmur's MetaParams's
	/// SSL settings, and updates the certificate and
	/// private key for all virtual servers that use the
	/// Meta server's certificate and private key.
	bool reloadSSLSettings();

	void bootAll();
	bool boot(int);
	bool banCheck(const QHostAddress &);

	/// Called whenever we get a successful connection from a client.
	/// Used to reset autoban tracking for the address.
	void successfulConnectionFrom(const QHostAddress &);
	void kill(int);
	void killAll();
	void getOSInfo();
	void connectListener(QObject *);
	static void getVersion(Version::component_t &major, Version::component_t &minor, Version::component_t &patch,
						   QString &string);
signals:
	void started(Server *);
	void stopped(Server *);
};

extern Meta *meta;

#endif
