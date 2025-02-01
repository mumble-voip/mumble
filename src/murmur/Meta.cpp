// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Meta.h"

#include "Connection.h"
#include "EnvUtils.h"
#include "FFDHE.h"
#include "Net.h"
#include "OSInfo.h"
#include "SSL.h"
#include "Server.h"
#include "ServerDB.h"
#include "Version.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QSettings>

#ifdef Q_OS_WIN
#	include <QtCore/QStandardPaths>
#endif

#include <QtNetwork/QHostInfo>

#if defined(USE_QSSLDIFFIEHELLMANPARAMETERS)
#	include <QtNetwork/QSslDiffieHellmanParameters>
#endif

#ifdef Q_OS_WIN
#	include <qos2.h>
#else
#	include <pwd.h>
#	include <sys/resource.h>
#endif

#include <QRandomGenerator>

std::unique_ptr< MetaParams > Meta::mp;

#ifdef Q_OS_WIN
HANDLE Meta::hQoS = nullptr;
#endif

MetaParams::MetaParams() {
	qsPassword = QString();
	usPort     = DEFAULT_MUMBLE_PORT;
	iTimeout   = 30;
	// This represents the maximum possible bandwidth using 10 ms audio TCP packets with position data
	// (restricted by the maximum bitrate Opus supports)
	// 558000 = 510000 (Opus) + 9600 (position) + 38400 (TCP overhead)
	iMaxBandwidth              = 558000;
	iMaxUsers                  = 1000;
	iMaxUsersPerChannel        = 0;
	iMaxListenersPerChannel    = -1;
	iMaxListenerProxiesPerUser = -1;
	iMaxTextMessageLength      = 5000;
	iMaxImageMessageLength     = 131072;
	legacyPasswordHash         = false;
	kdfIterations              = -1;
	bAllowHTML                 = true;
	iDefaultChan               = 0;
	bRememberChan              = true;
	iRememberChanDuration      = 0;
	qsWelcomeText              = QString();
	qsWelcomeTextFile          = QString();
	qsDatabase                 = QString();
	iSQLiteWAL                 = 0;
	iDBPort                    = 0;
	qsDBDriver                 = "QSQLITE";
	qsLogfile                  = "mumble-server.log";

	iLogDays = 31;

	iObfuscate         = 0;
	bSendVersion       = true;
	bBonjour           = true;
	bAllowPing         = true;
	bCertRequired      = false;
	bForceExternalAuth = false;

	iBanTries      = 10;
	iBanTimeframe  = 120;
	iBanTime       = 300;
	bBanSuccessful = true;

#ifdef Q_OS_UNIX
	uiUid = uiGid = 0;
#endif

	iOpusThreshold = 0;

	iChannelNestingLimit = 10;
	iChannelCountLimit   = 1000;

	qrUserName    = QRegularExpression(QLatin1String("[ -=\\w\\[\\]\\{\\}\\(\\)\\@\\|\\.]+"));
	qrChannelName = QRegularExpression(QLatin1String("[ -=\\w\\#\\[\\]\\{\\}\\(\\)\\@\\|]+"));

	iMessageLimit = 1;
	iMessageBurst = 5;

	iPluginMessageLimit = 4;
	iPluginMessageBurst = 15;

	broadcastListenerVolumeAdjustments = false;

	qsCiphers = MumbleSSL::defaultOpenSSLCipherString();

	bLogGroupChanges = false;
	bLogACLChanges   = false;

	allowRecording = true;

	rollingStatsWindow = 300;

	qsSettings = nullptr;
}

MetaParams::~MetaParams() {
	delete qsSettings;
}

/**
 *	Checks whether a qsSettings config variable named 'name' was set to a valid value for the type
 *  we want to convert it to. Otherwise a error message is logged and 'defaultValue' is returned.
 *
 *  E.g. checkedFromSettings<QString>("blub", bla) would output an error message and leave bla unchanged
 *		 if the user had set the ini parameter to "blub = has, commas, in, it" which QSettings will interpret
 *		 not as a String but as a list of strings.
 *
 *	@param T Conversion target type (type of 'defaultValue', auto inducable)
 *	@param name qsSettings variable name
 *	@param defaultValue Default value for 'name'
 *	@param settings The QSettings object to read from. If null, the Meta's qsSettings will be used.
 *	@return Setting if valid, default if not or setting not set.
 */
template< class ValueType, class ReturnType >
ReturnType MetaParams::typeCheckedFromSettings(const QString &name, const ValueType &defaultValue,
											   QSettings *settings) {
	// Use qsSettings unless a specific QSettings instance
	// is requested.
	if (!settings) {
		settings = qsSettings;
	}

	QVariant cfgVariable = settings->value(name, defaultValue);

	// Bit convoluted as canConvert<T>() only does a static check without considering whether
	// say a string like "blub" is actually a valid double (which convert does).
	if (!cfgVariable.convert(QMetaType(QVariant(defaultValue).metaType()))) {
		qCritical() << "Configuration variable" << name << "is of invalid format. Set to default value of"
					<< defaultValue << ".";
		return static_cast< ReturnType >(defaultValue);
	}

	return cfgVariable.value< ReturnType >();
}

void MetaParams::read(QString fname) {
	qmConfig.clear();

	if (fname.isEmpty()) {
		QStringList datapaths;

#if defined(Q_OS_WIN)
		datapaths << QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);

		QDir appdir = QDir(QDir::fromNativeSeparators(EnvUtils::getenv(QLatin1String("APPDATA"))));
		datapaths << appdir.absolutePath() + QLatin1String("/Mumble");
#else
		datapaths << QDir::homePath() + QLatin1String("/.murmurd");
		datapaths << QDir::homePath() + QLatin1String("/.mumble-server");
		datapaths << QDir::homePath() + QLatin1String("/.config/Mumble");
#endif

#if defined(Q_OS_MAC)
		/* Old Mac data path. */
		datapaths << QDir::homePath() + QLatin1String("/Library/Preferences/Mumble/");
#endif

		datapaths << QDir::homePath();
		datapaths << QDir::currentPath();
		datapaths << QCoreApplication::instance()->applicationDirPath();

		for (const QString &p : datapaths) {
			if (!p.isEmpty()) {
				// Prefer "mumble-server.ini" but for legacy reasons also keep looking for "murmur.ini"
				for (const QString &currentFileName :
					 { QStringLiteral("mumble-server.ini"), QStringLiteral("murmur.ini") }) {
					QFileInfo fi(p, currentFileName);
					if (fi.exists() && fi.isReadable()) {
						qdBasePath            = QDir(p);
						qsAbsSettingsFilePath = fi.absoluteFilePath();
						break;
					}
				}
			}
		}
		if (qsAbsSettingsFilePath.isEmpty()) {
			qdBasePath            = QDir(datapaths.at(0));
			qsAbsSettingsFilePath = qdBasePath.absolutePath() + QLatin1String("/mumble-server.ini");
		}
	} else {
		QFile f(fname);
		if (!f.open(QIODevice::ReadOnly)) {
			qFatal("Specified ini file %s could not be opened", qPrintable(fname));
		}
		qdBasePath            = QFileInfo(f).absoluteDir();
		qsAbsSettingsFilePath = QFileInfo(f).absoluteFilePath();
		f.close();
	}
	QDir::setCurrent(qdBasePath.absolutePath());
	qsSettings = new QSettings(qsAbsSettingsFilePath, QSettings::IniFormat);

	qsSettings->sync();
	switch (qsSettings->status()) {
		case QSettings::NoError:
			break;
		case QSettings::AccessError:
			qFatal("Access error while trying to access %s", qPrintable(qsSettings->fileName()));
			break;
		case QSettings::FormatError:
			qFatal("Your INI file at %s is invalid - check for syntax errors!", qPrintable(qsSettings->fileName()));
			break;
	}

	if (QFile::exists(qsAbsSettingsFilePath)) {
		qWarning("Initializing settings from %s (basepath %s)", qPrintable(qsSettings->fileName()),
				 qPrintable(qdBasePath.absolutePath()));
	} else {
		qWarning("No ini file at %s (basepath %s). Initializing with default settings.",
				 qPrintable(qsSettings->fileName()), qPrintable(qdBasePath.absolutePath()));
	}

	QString qsHost = qsSettings->value("host", QString()).toString();
	if (!qsHost.isEmpty()) {
		foreach (const QString &host, qsHost.split(QRegularExpression(QLatin1String("\\s+")), Qt::SkipEmptyParts)) {
			QHostAddress qhaddr;
			if (qhaddr.setAddress(host)) {
				qlBind << qhaddr;
			} else {
				bool found   = false;
				QHostInfo hi = QHostInfo::fromName(host);
				foreach (QHostAddress qha, hi.addresses()) {
					if ((qha.protocol() == QAbstractSocket::IPv4Protocol)
						|| (qha.protocol() == QAbstractSocket::IPv6Protocol)) {
						qlBind << qha;
						found = true;
					}
				}
				if (!found) {
					qFatal("Lookup of bind hostname %s failed", qPrintable(host));
				}
			}
		}
		foreach (const QHostAddress &qha, qlBind)
			qWarning("Binding to address %s", qPrintable(qha.toString()));
	}

	if (qlBind.isEmpty()) {
		qlBind << QHostAddress(QHostAddress::Any);
	}

	qsPassword            = typeCheckedFromSettings("serverpassword", qsPassword);
	usPort                = static_cast< unsigned short >(typeCheckedFromSettings("port", static_cast< uint >(usPort)));
	iTimeout              = typeCheckedFromSettings("timeout", iTimeout);
	iMaxTextMessageLength = typeCheckedFromSettings("textmessagelength", iMaxTextMessageLength);
	iMaxImageMessageLength     = typeCheckedFromSettings("imagemessagelength", iMaxImageMessageLength);
	legacyPasswordHash         = typeCheckedFromSettings("legacypasswordhash", legacyPasswordHash);
	kdfIterations              = typeCheckedFromSettings("kdfiterations", -1);
	bAllowHTML                 = typeCheckedFromSettings("allowhtml", bAllowHTML);
	iMaxBandwidth              = typeCheckedFromSettings("bandwidth", iMaxBandwidth);
	iDefaultChan               = typeCheckedFromSettings("defaultchannel", iDefaultChan);
	bRememberChan              = typeCheckedFromSettings("rememberchannel", bRememberChan);
	iRememberChanDuration      = typeCheckedFromSettings("rememberchannelduration", iRememberChanDuration);
	iMaxUsers                  = typeCheckedFromSettings("users", iMaxUsers);
	iMaxUsersPerChannel        = typeCheckedFromSettings("usersperchannel", iMaxUsersPerChannel);
	iMaxListenersPerChannel    = typeCheckedFromSettings("listenersperchannel", iMaxListenersPerChannel);
	iMaxListenerProxiesPerUser = typeCheckedFromSettings("listenersperuser", iMaxListenerProxiesPerUser);
	qsWelcomeText              = typeCheckedFromSettings("welcometext", qsWelcomeText);
	qsWelcomeTextFile          = typeCheckedFromSettings("welcometextfile", qsWelcomeTextFile);
	bCertRequired              = typeCheckedFromSettings("certrequired", bCertRequired);
	bForceExternalAuth         = typeCheckedFromSettings("forceExternalAuth", bForceExternalAuth);

	qsDatabase = typeCheckedFromSettings("database", qsDatabase);
	iSQLiteWAL = typeCheckedFromSettings("sqlite_wal", iSQLiteWAL);

	qsDBDriver   = typeCheckedFromSettings("dbDriver", qsDBDriver);
	qsDBUserName = typeCheckedFromSettings("dbUsername", qsDBUserName);
	qsDBPassword = typeCheckedFromSettings("dbPassword", qsDBPassword);
	qsDBHostName = typeCheckedFromSettings("dbHost", qsDBHostName);
	qsDBPrefix   = typeCheckedFromSettings("dbPrefix", qsDBPrefix);
	qsDBOpts     = typeCheckedFromSettings("dbOpts", qsDBOpts);
	iDBPort      = typeCheckedFromSettings("dbPort", iDBPort);

	qsIceEndpoint    = typeCheckedFromSettings("ice", qsIceEndpoint);
	qsIceSecretRead  = typeCheckedFromSettings("icesecret", qsIceSecretRead);
	qsIceSecretRead  = typeCheckedFromSettings("icesecretread", qsIceSecretRead);
	qsIceSecretWrite = typeCheckedFromSettings("icesecretwrite", qsIceSecretRead);

	iLogDays = typeCheckedFromSettings("logdays", iLogDays);

	qsLogfile = typeCheckedFromSettings("logfile", qsLogfile);
	qsPid     = typeCheckedFromSettings("pidfile", qsPid);

	qsRegName     = typeCheckedFromSettings("registerName", qsRegName);
	qsRegPassword = typeCheckedFromSettings("registerPassword", qsRegPassword);
	qsRegHost     = typeCheckedFromSettings("registerHostname", qsRegHost);
	qsRegLocation = typeCheckedFromSettings("registerLocation", qsRegLocation);
	qurlRegWeb    = QUrl(typeCheckedFromSettings("registerUrl", qurlRegWeb).toString());
	bBonjour      = typeCheckedFromSettings("bonjour", bBonjour);

	iBanTries      = typeCheckedFromSettings("autobanAttempts", iBanTries);
	iBanTimeframe  = typeCheckedFromSettings("autobanTimeframe", iBanTimeframe);
	iBanTime       = typeCheckedFromSettings("autobanTime", iBanTime);
	bBanSuccessful = typeCheckedFromSettings("autobanSuccessfulConnections", bBanSuccessful);

	m_suggestVersion = Version::fromConfig(qsSettings->value("suggestVersion"));

	qvSuggestPositional = qsSettings->value("suggestPositional");
	if (qvSuggestPositional.toString().trimmed().isEmpty())
		qvSuggestPositional = QVariant();

	qvSuggestPushToTalk = qsSettings->value("suggestPushToTalk");
	if (qvSuggestPushToTalk.toString().trimmed().isEmpty())
		qvSuggestPushToTalk = QVariant();

	bLogGroupChanges = typeCheckedFromSettings("loggroupchanges", bLogGroupChanges);
	bLogACLChanges   = typeCheckedFromSettings("logaclchanges", bLogACLChanges);

	allowRecording = typeCheckedFromSettings("allowRecording", allowRecording);

	rollingStatsWindow = typeCheckedFromSettings("rollingStatsWindow", rollingStatsWindow);

	iOpusThreshold = typeCheckedFromSettings("opusthreshold", iOpusThreshold);

	iChannelNestingLimit = typeCheckedFromSettings("channelnestinglimit", iChannelNestingLimit);
	iChannelCountLimit   = typeCheckedFromSettings("channelcountlimit", iChannelCountLimit);

#ifdef Q_OS_UNIX
	qsName = qsSettings->value("uname").toString();
	if (geteuid() == 0) {
		// TODO: remove this silent fallback to enforce running as non-root
		bool requested = true;
		if (qsName.isEmpty()) {
			// default server user name
			qsName    = "mumble-server";
			requested = false;
		}
		struct passwd *pw = getpwnam(qPrintable(qsName));
		if (pw) {
			uiUid  = pw->pw_uid;
			uiGid  = pw->pw_gid;
			qsHome = QString::fromUtf8(pw->pw_dir);
		} else if (requested) {
			qFatal("Cannot find username %s", qPrintable(qsName));
		}
		endpwent();
	}
#endif

	qrUserName    = QRegularExpression(typeCheckedFromSettings("username", qrUserName.pattern()));
	qrChannelName = QRegularExpression(typeCheckedFromSettings("channelname", qrChannelName.pattern()));

	iMessageLimit = typeCheckedFromSettings< unsigned int >("messagelimit", 1);
	iMessageBurst = typeCheckedFromSettings< unsigned int >("messageburst", 5);

	iPluginMessageLimit = typeCheckedFromSettings< unsigned int >("pluginmessagelimit", 4);
	iPluginMessageBurst = typeCheckedFromSettings< unsigned int >("pluginmessageburst", 15);

	broadcastListenerVolumeAdjustments = typeCheckedFromSettings("broadcastlistenervolumeadjustments", false);

	bool bObfuscate = typeCheckedFromSettings("obfuscate", false);
	if (bObfuscate) {
		qWarning("IP address obfuscation enabled.");
		iObfuscate = static_cast< int >(QRandomGenerator::global()->generate());
	}
	bSendVersion = typeCheckedFromSettings("sendversion", bSendVersion);
	bAllowPing   = typeCheckedFromSettings("allowping", bAllowPing);

	if (!loadSSLSettings()) {
		qFatal("MetaParams: Failed to load SSL settings. See previous errors.");
	}

	QStringList hosts;
	foreach (const QHostAddress &qha, qlBind) { hosts << qha.toString(); }
	qmConfig.insert(QLatin1String("host"), hosts.join(" "));
	qmConfig.insert(QLatin1String("password"), qsPassword);
	qmConfig.insert(QLatin1String("port"), QString::number(usPort));
	qmConfig.insert(QLatin1String("timeout"), QString::number(iTimeout));
	qmConfig.insert(QLatin1String("textmessagelength"), QString::number(iMaxTextMessageLength));
	qmConfig.insert(QLatin1String("legacypasswordhash"),
					legacyPasswordHash ? QLatin1String("true") : QLatin1String("false"));
	qmConfig.insert(QLatin1String("kdfiterations"), QString::number(kdfIterations));
	qmConfig.insert(QLatin1String("allowhtml"), bAllowHTML ? QLatin1String("true") : QLatin1String("false"));
	qmConfig.insert(QLatin1String("bandwidth"), QString::number(iMaxBandwidth));
	qmConfig.insert(QLatin1String("users"), QString::number(iMaxUsers));
	qmConfig.insert(QLatin1String("defaultchannel"), QString::number(iDefaultChan));
	qmConfig.insert(QLatin1String("rememberchannel"), bRememberChan ? QLatin1String("true") : QLatin1String("false"));
	qmConfig.insert(QLatin1String("rememberchannelduration"), QString::number(iRememberChanDuration));
	qmConfig.insert(QLatin1String("welcometext"), qsWelcomeText);
	qmConfig.insert(QLatin1String("welcometextfile"), qsWelcomeTextFile);
	qmConfig.insert(QLatin1String("registername"), qsRegName);
	qmConfig.insert(QLatin1String("registerpassword"), qsRegPassword);
	qmConfig.insert(QLatin1String("registerhostname"), qsRegHost);
	qmConfig.insert(QLatin1String("registerlocation"), qsRegLocation);
	qmConfig.insert(QLatin1String("registerurl"), qurlRegWeb.toString());
	qmConfig.insert(QLatin1String("bonjour"), bBonjour ? QLatin1String("true") : QLatin1String("false"));
	qmConfig.insert(QLatin1String("certificate"), QString::fromUtf8(qscCert.toPem()));
	qmConfig.insert(QLatin1String("key"), QString::fromUtf8(qskKey.toPem()));
	qmConfig.insert(QLatin1String("obfuscate"), bObfuscate ? QLatin1String("true") : QLatin1String("false"));
	qmConfig.insert(QLatin1String("username"), qrUserName.pattern());
	qmConfig.insert(QLatin1String("channelname"), qrChannelName.pattern());
	qmConfig.insert(QLatin1String("certrequired"), bCertRequired ? QLatin1String("true") : QLatin1String("false"));
	qmConfig.insert(QLatin1String("forceExternalAuth"),
					bForceExternalAuth ? QLatin1String("true") : QLatin1String("false"));
	qmConfig.insert(QLatin1String("suggestversion"), Version::toConfigString(m_suggestVersion));
	qmConfig.insert(QLatin1String("suggestpositional"),
					qvSuggestPositional.isNull() ? QString() : qvSuggestPositional.toString());
	qmConfig.insert(QLatin1String("suggestpushtotalk"),
					qvSuggestPushToTalk.isNull() ? QString() : qvSuggestPushToTalk.toString());
	qmConfig.insert(QLatin1String("opusthreshold"), QString::number(iOpusThreshold));
	qmConfig.insert(QLatin1String("channelnestinglimit"), QString::number(iChannelNestingLimit));
	qmConfig.insert(QLatin1String("channelcountlimit"), QString::number(iChannelCountLimit));
	qmConfig.insert(QLatin1String("sslCiphers"), qsCiphers);
	qmConfig.insert(QLatin1String("sslDHParams"), QString::fromLatin1(qbaDHParams.constData()));
}

bool MetaParams::loadSSLSettings() {
	QSettings updatedSettings(qsAbsSettingsFilePath, QSettings::IniFormat);

	QString tmpCiphersStr = typeCheckedFromSettings("sslCiphers", qsCiphers);

	QString qsSSLCert     = qsSettings->value("sslCert").toString();
	QString qsSSLKey      = qsSettings->value("sslKey").toString();
	QString qsSSLCA       = qsSettings->value("sslCA").toString();
	QString qsSSLDHParams = typeCheckedFromSettings(QLatin1String("sslDHParams"), QString(QLatin1String("@ffdhe2048")));

	qbaPassPhrase = qsSettings->value("sslPassPhrase").toByteArray();

	QSslCertificate tmpCert;
	QList< QSslCertificate > tmpCA;
	QList< QSslCertificate > tmpIntermediates;
	QSslKey tmpKey;
	QByteArray tmpDHParams;
	QList< QSslCipher > tmpCiphers;


	if (!qsSSLCA.isEmpty()) {
		QFile pem(qsSSLCA);
		if (pem.open(QIODevice::ReadOnly)) {
			QByteArray qba = pem.readAll();
			pem.close();
			QList< QSslCertificate > ql = QSslCertificate::fromData(qba);
			if (ql.isEmpty()) {
				qCritical("MetaParams: Failed to parse any CA certificates from %s", qPrintable(qsSSLCA));
				return false;
			} else {
				tmpCA = ql;
			}
		} else {
			qCritical("MetaParams: Failed to read %s", qPrintable(qsSSLCA));
			return false;
		}
	}

	QByteArray crt, key;

	if (!qsSSLCert.isEmpty()) {
		QFile pem(qsSSLCert);
		if (pem.open(QIODevice::ReadOnly)) {
			crt = pem.readAll();
			pem.close();
		} else {
			qCritical("MetaParams: Failed to read %s", qPrintable(qsSSLCert));
			return false;
		}
	}
	if (!qsSSLKey.isEmpty()) {
		QFile pem(qsSSLKey);
		if (pem.open(QIODevice::ReadOnly)) {
			key = pem.readAll();
			pem.close();
		} else {
			qCritical("MetaParams: Failed to read %s", qPrintable(qsSSLKey));
			return false;
		}
	}

	if (!key.isEmpty() || !crt.isEmpty()) {
		if (!key.isEmpty()) {
			tmpKey = Server::privateKeyFromPEM(key, qbaPassPhrase);
		}
		if (tmpKey.isNull() && !crt.isEmpty()) {
			tmpKey = Server::privateKeyFromPEM(crt, qbaPassPhrase);
			if (!tmpKey.isNull())
				qCritical("MetaParams: Using private key found in certificate file.");
		}
		if (tmpKey.isNull()) {
			qCritical("MetaParams: No private key found in certificate or key file.");
			return false;
		}

		QList< QSslCertificate > ql = QSslCertificate::fromData(crt);
		ql << QSslCertificate::fromData(key);
		for (int i = 0; i < ql.size(); ++i) {
			const QSslCertificate &c = ql.at(i);
			if (Server::isKeyForCert(tmpKey, c)) {
				tmpCert = c;
				ql.removeAt(i);
				break;
			}
		}
		if (tmpCert.isNull()) {
			qCritical("MetaParams: Failed to find certificate matching private key.");
			return false;
		}
		if (ql.size() > 0) {
			tmpIntermediates = ql;
			qCritical("MetaParams: Adding %lld intermediate certificates from certificate file.",
					  static_cast< qsizetype >(ql.size()));
		}
	}

	QByteArray dhparams;

#if defined(USE_QSSLDIFFIEHELLMANPARAMETERS)
	if (!qsSSLDHParams.isEmpty()) {
		if (qsSSLDHParams.startsWith(QLatin1String("@"))) {
			QString group  = qsSSLDHParams.mid(1).trimmed();
			QByteArray pem = FFDHE::PEMForNamedGroup(group);
			if (pem.isEmpty()) {
				QStringList names = FFDHE::NamedGroups();
				QStringList atNames;
				foreach (QString name, names) { atNames << QLatin1String("@") + name; }
				QString supported = atNames.join(QLatin1String(", "));
				qFatal("MetaParms: Diffie-Hellman parameters with name '%s' is not available. (Supported: %s)",
					   qPrintable(qsSSLDHParams), qPrintable(supported));
			}
			dhparams = pem;
		} else {
			QFile pem(qsSSLDHParams);
			if (pem.open(QIODevice::ReadOnly)) {
				dhparams = pem.readAll();
				pem.close();
			} else {
				qFatal("MetaParams: Failed to read %s", qPrintable(qsSSLDHParams));
			}
		}
	}

	if (!dhparams.isEmpty()) {
		QSslDiffieHellmanParameters qdhp = QSslDiffieHellmanParameters::fromEncoded(dhparams);
		if (qdhp.isValid()) {
			tmpDHParams = dhparams;
		} else {
			qFatal("MetaParams: Unable to use specified Diffie-Hellman parameters: %s", qPrintable(qdhp.errorString()));
			return false;
		}
	}
#else
	QString qsSSLDHParamsIniValue = qsSettings->value(QLatin1String("sslDHParams")).toString();
	if (!qsSSLDHParamsIniValue.isEmpty()) {
		qFatal("MetaParams: This version of Murmur does not support Diffie-Hellman parameters (sslDHParams). Murmur "
			   "will not start unless you remove the option from your mumble-server.ini (murmur.ini)file.");
		return false;
	}
#endif

	{
		QList< QSslCipher > ciphers = MumbleSSL::ciphersFromOpenSSLCipherString(tmpCiphersStr);
		if (ciphers.isEmpty()) {
			qCritical("MetaParams: Invalid sslCiphers option. Either the cipher string is invalid or none of the "
					  "ciphers are available: \"%s\"",
					  qPrintable(qsCiphers));
			return false;
		}

#if !defined(USE_QSSLDIFFIEHELLMANPARAMETERS)
		// If the version of Qt we're building against doesn't support
		// QSslDiffieHellmanParameters, then we must filter out Diffie-
		// Hellman cipher suites in order to guarantee that we do not
		// use Qt's default Diffie-Hellman parameters.
		{
			QList< QSslCipher > filtered;
			foreach (QSslCipher c, ciphers) {
				if (c.keyExchangeMethod() == QLatin1String("DH")) {
					continue;
				}
				filtered << c;
			}
			if (ciphers.size() != filtered.size()) {
				qWarning("MetaParams: Warning: all cipher suites in sslCiphers using Diffie-Hellman key exchange "
						 "have been removed. Qt %s does not support custom Diffie-Hellman parameters.",
						 qVersion());
			}

			tmpCiphers = filtered;
		}
#else
		tmpCiphers = ciphers;
#endif

		QStringList pref;
		foreach (QSslCipher c, tmpCiphers) { pref << c.name(); }
		qWarning("MetaParams: TLS cipher preference is \"%s\"", qPrintable(pref.join(QLatin1String(":"))));
	}

	qscCert         = tmpCert;
	qlCA            = tmpCA;
	qlIntermediates = tmpIntermediates;
	qskKey          = tmpKey;
	qbaDHParams     = tmpDHParams;
	qsCiphers       = tmpCiphersStr;
	qlCiphers       = tmpCiphers;

	qmConfig.insert(QLatin1String("certificate"), QString::fromUtf8(qscCert.toPem()));
	qmConfig.insert(QLatin1String("key"), QString::fromUtf8(qskKey.toPem()));
	qmConfig.insert(QLatin1String("sslCiphers"), qsCiphers);
	qmConfig.insert(QLatin1String("sslDHParams"), QString::fromLatin1(qbaDHParams.constData()));

	return true;
}

Meta::Meta() {
#ifdef Q_OS_WIN
	QOS_VERSION qvVer;
	qvVer.MajorVersion = 1;
	qvVer.MinorVersion = 0;

	hQoS = nullptr;

	HMODULE hLib = LoadLibrary(L"qWave.dll");
	if (!hLib) {
		qWarning("Meta: Failed to load qWave.dll, no QoS available");
	} else {
		FreeLibrary(hLib);
		if (!QOSCreateHandle(&qvVer, &hQoS))
			qWarning("Meta: Failed to create QOS2 handle");
		else
			Connection::setQoS(hQoS);
	}
#endif
}

Meta::~Meta() {
#ifdef Q_OS_WIN
	if (hQoS) {
		QOSCloseHandle(hQoS);
		Connection::setQoS(nullptr);
	}
#endif
}

bool Meta::reloadSSLSettings() {
	// Reload SSL settings.
	if (!Meta::mp->loadSSLSettings()) {
		return false;
	}

	// Re-initialize certificates for all
	// virtual servers using the Meta server's
	// certificate and private key.
	foreach (Server *s, qhServers) {
		if (s->bUsingMetaCert) {
			s->log("Reloading certificates...");
			s->initializeCert();
		} else {
			s->log("Not reloading certificates; server does not use Meta certificate");
		}
	}

	return true;
}

void Meta::getOSInfo() {
	qsOS        = OSInfo::getOS();
	qsOSVersion = OSInfo::getOSDisplayableVersion();
}

void Meta::bootAll() {
	QList< int > ql = ServerDB::getBootServers();
	foreach (int snum, ql)
		boot(snum);
}

bool Meta::boot(int srvnum) {
	if (qhServers.contains(srvnum))
		return false;
	if (!ServerDB::serverExists(srvnum))
		return false;
	Server *s = new Server(srvnum, this);
	if (!s->bValid) {
		delete s;
		return false;
	}
	qhServers.insert(srvnum, s);
	emit started(s);

#ifdef Q_OS_UNIX
	unsigned int sockets = 19; // Base
	foreach (s, qhServers) {
		sockets += 11;                                        // Listen sockets, signal pipes etc.
		sockets += static_cast< unsigned int >(s->iMaxUsers); // One per user
	}

	struct rlimit r;
	if (getrlimit(RLIMIT_NOFILE, &r) == 0) {
		if (r.rlim_cur < sockets) {
			r.rlim_cur = sockets;
			if (r.rlim_max < sockets)
				r.rlim_max = sockets;
			if (setrlimit(RLIMIT_NOFILE, &r) != 0) {
				getrlimit(RLIMIT_NOFILE, &r);
				if (r.rlim_cur < r.rlim_max) {
					r.rlim_cur = r.rlim_max;
					setrlimit(RLIMIT_NOFILE, &r);
					getrlimit(RLIMIT_NOFILE, &r);
				}
			}
		}
		if (r.rlim_cur < sockets)
			qCritical(
				"Current booted servers require minimum %d file descriptors when all slots are full, but only %lu file "
				"descriptors are allowed for this process. Your server will crash and burn; read the FAQ for details.",
				sockets, static_cast< unsigned long >(r.rlim_cur));
	}
#endif

	return true;
}

void Meta::kill(int srvnum) {
	Server *s = qhServers.take(srvnum);
	if (!s)
		return;
	emit stopped(s);
	delete s;
}

void Meta::killAll() {
	foreach (Server *s, qhServers) {
		emit stopped(s);
		delete s;
	}
	qhServers.clear();
}

void Meta::successfulConnectionFrom(const QHostAddress &addr) {
	if (!mp->bBanSuccessful) {
		QList< Timer > &ql = qhAttempts[addr];
		// Seems like this is the most efficient way to clear the list, given:
		// 1. ql.clear() allocates a new array
		// 2. ql has less than iBanAttempts members
		// 3. seems like ql.removeFirst() might actually copy elements to shift to the front
		while (!ql.empty()) {
			ql.removeLast();
		}
	}
}

bool Meta::banCheck(const QHostAddress &addr) {
	if ((mp->iBanTries <= 0) || (mp->iBanTimeframe <= 0))
		return false;

	if (qhBans.contains(addr)) {
		Timer t = qhBans.value(addr);
		if (t.elapsed() < (1000000ULL * static_cast< unsigned long long >(mp->iBanTime)))
			return true;
		qhBans.remove(addr);
	}

	QList< Timer > &ql = qhAttempts[addr];

	ql.append(Timer());
	while (!ql.isEmpty() && (ql.at(0).elapsed() > (1000000ULL * static_cast< unsigned long long >(mp->iBanTimeframe))))
		ql.removeFirst();

	if (ql.count() > mp->iBanTries) {
		qhBans.insert(addr, Timer());
		return true;
	}
	return false;
}
