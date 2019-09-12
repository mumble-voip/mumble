// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Meta.h"

#include "Connection.h"
#include "Net.h"
#include "ServerDB.h"
#include "Server.h"
#include "OSInfo.h"
#include "Version.h"
#include "SSL.h"
#include "EnvUtils.h"
#include "FFDHE.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QSettings>

#ifdef Q_OS_WIN
# include <QtCore/QStandardPaths>
#endif

#include <QtNetwork/QHostInfo>
#include <QtNetwork/QNetworkInterface>

#if defined(USE_QSSLDIFFIEHELLMANPARAMETERS)
# include <QtNetwork/QSslDiffieHellmanParameters>
#endif

#ifdef Q_OS_WIN
# include <qos2.h>
#else
# include <pwd.h>
# include <sys/resource.h>
#endif

MetaParams Meta::mp;

#ifdef Q_OS_WIN
HANDLE Meta::hQoS = NULL;
#endif

MetaParams::MetaParams() {
	qsPassword = QString();
	usPort = DEFAULT_MUMBLE_PORT;
	iTimeout = 30;
	iMaxBandwidth = 72000;
	iMaxUsers = 1000;
	iMaxUsersPerChannel = 0;
	iMaxTextMessageLength = 5000;
	iMaxImageMessageLength = 131072;
	legacyPasswordHash = false;
	kdfIterations = -1;
	bAllowHTML = true;
	iDefaultChan = 0;
	bRememberChan = true;
	qsWelcomeText = QString();
	qsDatabase = QString();
	iSQLiteWAL = 0;
	iDBPort = 0;
	qsDBusService = "net.sourceforge.mumble.murmur";
	qsDBDriver = "QSQLITE";
	qsLogfile = "murmur.log";

	iLogDays = 31;

	iObfuscate = 0;
	bSendVersion = true;
	bBonjour = true;
	bAllowPing = true;
	bCertRequired = false;
	bForceExternalAuth = false;

	iBanTries = 10;
	iBanTimeframe = 120;
	iBanTime = 300;

#ifdef Q_OS_UNIX
	uiUid = uiGid = 0;
#endif

	iOpusThreshold = 100;

	iChannelNestingLimit = 10;
	iChannelCountLimit = 1000;

	qrUserName = QRegExp(QLatin1String("[-=\\w\\[\\]\\{\\}\\(\\)\\@\\|\\.]+"));
	qrChannelName = QRegExp(QLatin1String("[ \\-=\\w\\#\\[\\]\\{\\}\\(\\)\\@\\|]+"));

	iMessageLimit = 1;
	iMessageBurst = 5;

	qsCiphers = MumbleSSL::defaultOpenSSLCipherString();

	qsSettings = NULL;
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
template <class T>
T MetaParams::typeCheckedFromSettings(const QString &name, const T &defaultValue, QSettings *settings) {
	// Use qsSettings unless a specific QSettings instance
	// is requested.
	if (settings == NULL) {
		settings = qsSettings;
	}

	QVariant cfgVariable = settings->value(name, defaultValue);

	if (!cfgVariable.convert(QVariant(defaultValue).type())) { // Bit convoluted as canConvert<T>() only does a static check without considering whether say a string like "blub" is actually a valid double (which convert does).
		qCritical() << "Configuration variable" << name << "is of invalid format. Set to default value of" << defaultValue << ".";
		return defaultValue;
	}

	return cfgVariable.value<T>();
}

void MetaParams::read(QString fname) {
	qmConfig.clear();

	if (fname.isEmpty()) {
		QStringList datapaths;

#if defined(Q_OS_WIN)
#if QT_VERSION >= 0x050000
		datapaths << QStandardPaths::writableLocation(QStandardPaths::DataLocation);
#else
		datapaths << QDesktopServices::storageLocation(QDesktopServices::DataLocation);
#endif

		QDir appdir = QDir(QDir::fromNativeSeparators(EnvUtils::getenv(QLatin1String("APPDATA"))));
		datapaths << appdir.absolutePath() + QLatin1String("/Mumble");
#else
		datapaths << QDir::homePath() + QLatin1String("/.murmurd");
		datapaths << QDir::homePath() + QLatin1String("/.config/Mumble");
#endif

#if defined(Q_OS_MAC)
		/* Old Mac data path. */
		datapaths << QDir::homePath() + QLatin1String("/Library/Preferences/Mumble/");
#endif

		datapaths << QDir::homePath();
		datapaths << QDir::currentPath();
		datapaths << QCoreApplication::instance()->applicationDirPath();

		foreach(const QString &p, datapaths) {
			if (! p.isEmpty()) {
				QFileInfo fi(p, "murmur.ini");
				if (fi.exists() && fi.isReadable()) {
					qdBasePath = QDir(p);
					qsAbsSettingsFilePath = fi.absoluteFilePath();
					break;
				}
			}
		}
		if (qsAbsSettingsFilePath.isEmpty()) {
			QDir::root().mkpath(qdBasePath.absolutePath());
			qdBasePath = QDir(datapaths.at(0));
			qsAbsSettingsFilePath = qdBasePath.absolutePath() + QLatin1String("/murmur.ini");
		}
	} else {
		QFile f(fname);
		if (! f.open(QIODevice::ReadOnly)) {
			qFatal("Specified ini file %s could not be opened", qPrintable(fname));
		}
		qdBasePath = QFileInfo(f).absoluteDir();
		qsAbsSettingsFilePath = QFileInfo(f).absoluteFilePath();
		f.close();
	}
	QDir::setCurrent(qdBasePath.absolutePath());
	qsSettings = new QSettings(qsAbsSettingsFilePath, QSettings::IniFormat);
#if QT_VERSION >= 0x040500
	qsSettings->setIniCodec("UTF-8");
#endif
	qWarning("Initializing settings from %s (basepath %s)", qPrintable(qsSettings->fileName()), qPrintable(qdBasePath.absolutePath()));

	QString qsHost = qsSettings->value("host", QString()).toString();
	if (! qsHost.isEmpty()) {
		foreach(const QString &host, qsHost.split(QRegExp(QLatin1String("\\s+")), QString::SkipEmptyParts)) {
			QHostAddress qhaddr;
			if (qhaddr.setAddress(host)) {
				qlBind << qhaddr;
			} else {
				bool found = false;
				QHostInfo hi = QHostInfo::fromName(host);
				foreach(QHostAddress qha, hi.addresses()) {
					if ((qha.protocol() == QAbstractSocket::IPv4Protocol) || (qha.protocol() == QAbstractSocket::IPv6Protocol)) {
						qlBind << qha;
						found = true;
					}
				}
				if (! found) {
					qFatal("Lookup of bind hostname %s failed", qPrintable(host));
				}
			}
		}
		foreach(const QHostAddress &qha, qlBind)
			qWarning("Binding to address %s", qPrintable(qha.toString()));
	}

	if (qlBind.isEmpty()) {
		bool hasipv6 = false;
		bool hasipv4 = false;
		int nif = 0;

		QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
		if (interfaces.isEmpty()) {
			qWarning("Meta: Unable to acquire list of network interfaces.");
		} else {
			foreach(const QNetworkInterface &qni, interfaces) {
				if (!(qni.flags() & QNetworkInterface::IsUp))
					continue;
				if (!(qni.flags() & QNetworkInterface::IsRunning))
					continue;
				if (qni.flags() & QNetworkInterface::IsLoopBack)
					continue;

				foreach(const QNetworkAddressEntry &qna, qni.addressEntries()) {
					const QHostAddress &qha = qna.ip();
					switch (qha.protocol()) {
						case QAbstractSocket::IPv4Protocol:
							hasipv4 = true;
							break;
						case QAbstractSocket::IPv6Protocol:
							hasipv6 = true;
							break;
						default:
							break;
					}
				}

				++nif;
			}
		}

		if (nif == 0) {
			qWarning("Meta: Could not determine IPv4/IPv6 support via network interfaces, assuming support for both.");
			hasipv6 = true;
			hasipv4 = true;
		}

#if QT_VERSION >= 0x050000
		if (hasipv6) {
			if (SslServer::hasDualStackSupport() && hasipv4) {
				qlBind << QHostAddress(QHostAddress::Any);
				hasipv4 = false; // No need to add a separate ipv4 socket
			} else {
				qlBind << QHostAddress(QHostAddress::AnyIPv6);
			}
		}

		if (hasipv4) {
			qlBind << QHostAddress(QHostAddress::AnyIPv4);
		}
#else // QT_VERSION < 0x050000
		// For Qt 4 AnyIPv6 resulted in a dual stack socket on dual stack
		// capable systems while Any resulted in an IPv4 only socket. For
		// Qt 5 this has been reworked so that AnyIPv6/v4 are now exclusive
		// IPv6/4 sockets while Any is the dual stack socket.

		if (hasipv6) {
			qlBind << QHostAddress(QHostAddress::AnyIPv6);
			if (SslServer::hasDualStackSupport() && hasipv4) {
				hasipv4 = false; // No need to add a separate ipv4 socket
			}
		}

		if (hasipv4) {
			qlBind << QHostAddress(QHostAddress::Any);
		}
#endif
	}

	qsPassword = typeCheckedFromSettings("serverpassword", qsPassword);
	usPort = static_cast<unsigned short>(typeCheckedFromSettings("port", static_cast<uint>(usPort)));
	iTimeout = typeCheckedFromSettings("timeout", iTimeout);
	iMaxTextMessageLength = typeCheckedFromSettings("textmessagelength", iMaxTextMessageLength);
	iMaxImageMessageLength = typeCheckedFromSettings("imagemessagelength", iMaxImageMessageLength);
	legacyPasswordHash = typeCheckedFromSettings("legacypasswordhash", legacyPasswordHash);
	kdfIterations = typeCheckedFromSettings("kdfiterations", -1);
	bAllowHTML = typeCheckedFromSettings("allowhtml", bAllowHTML);
	iMaxBandwidth = typeCheckedFromSettings("bandwidth", iMaxBandwidth);
	iDefaultChan = typeCheckedFromSettings("defaultchannel", iDefaultChan);
	bRememberChan = typeCheckedFromSettings("rememberchannel", bRememberChan);
	iMaxUsers = typeCheckedFromSettings("users", iMaxUsers);
	iMaxUsersPerChannel = typeCheckedFromSettings("usersperchannel", iMaxUsersPerChannel);
	qsWelcomeText = typeCheckedFromSettings("welcometext", qsWelcomeText);
	bCertRequired = typeCheckedFromSettings("certrequired", bCertRequired);
	bForceExternalAuth = typeCheckedFromSettings("forceExternalAuth", bForceExternalAuth);

	qsDatabase = typeCheckedFromSettings("database", qsDatabase);
	iSQLiteWAL = typeCheckedFromSettings("sqlite_wal", iSQLiteWAL);

	qsDBDriver = typeCheckedFromSettings("dbDriver", qsDBDriver);
	qsDBUserName = typeCheckedFromSettings("dbUsername", qsDBUserName);
	qsDBPassword = typeCheckedFromSettings("dbPassword", qsDBPassword);
	qsDBHostName = typeCheckedFromSettings("dbHost", qsDBHostName);
	qsDBPrefix = typeCheckedFromSettings("dbPrefix", qsDBPrefix);
	qsDBOpts = typeCheckedFromSettings("dbOpts", qsDBOpts);
	iDBPort = typeCheckedFromSettings("dbPort", iDBPort);

	qsIceEndpoint = typeCheckedFromSettings("ice", qsIceEndpoint);
	qsIceSecretRead = typeCheckedFromSettings("icesecret", qsIceSecretRead);
	qsIceSecretRead = typeCheckedFromSettings("icesecretread", qsIceSecretRead);
	qsIceSecretWrite = typeCheckedFromSettings("icesecretwrite", qsIceSecretRead);

	qsGRPCAddress = typeCheckedFromSettings("grpc", qsGRPCAddress);
	qsGRPCCert = typeCheckedFromSettings("grpccert", qsGRPCCert);
	qsGRPCKey = typeCheckedFromSettings("grpckey", qsGRPCKey);

	iLogDays = typeCheckedFromSettings("logdays", iLogDays);

	qsDBus = typeCheckedFromSettings("dbus", qsDBus);
	qsDBusService = typeCheckedFromSettings("dbusservice", qsDBusService);
	qsLogfile = typeCheckedFromSettings("logfile", qsLogfile);
	qsPid = typeCheckedFromSettings("pidfile", qsPid);

	qsRegName = typeCheckedFromSettings("registerName", qsRegName);
	qsRegPassword = typeCheckedFromSettings("registerPassword", qsRegPassword);
	qsRegHost = typeCheckedFromSettings("registerHostname", qsRegHost);
	qsRegLocation = typeCheckedFromSettings("registerLocation", qsRegLocation);
	qurlRegWeb = QUrl(typeCheckedFromSettings("registerUrl", qurlRegWeb).toString());
	bBonjour = typeCheckedFromSettings("bonjour", bBonjour);

	iBanTries = typeCheckedFromSettings("autobanAttempts", iBanTries);
	iBanTimeframe = typeCheckedFromSettings("autobanTimeframe", iBanTimeframe);
	iBanTime = typeCheckedFromSettings("autobanTime", iBanTime);

	qvSuggestVersion = MumbleVersion::getRaw(qsSettings->value("suggestVersion").toString());
	if (qvSuggestVersion.toUInt() == 0)
		qvSuggestVersion = QVariant();

	qvSuggestPositional = qsSettings->value("suggestPositional");
	if (qvSuggestPositional.toString().trimmed().isEmpty())
		qvSuggestPositional = QVariant();

	qvSuggestPushToTalk = qsSettings->value("suggestPushToTalk");
	if (qvSuggestPushToTalk.toString().trimmed().isEmpty())
		qvSuggestPushToTalk = QVariant();

	iOpusThreshold = typeCheckedFromSettings("opusthreshold", iOpusThreshold);

	iChannelNestingLimit = typeCheckedFromSettings("channelnestinglimit", iChannelNestingLimit);
	iChannelCountLimit = typeCheckedFromSettings("channelcountlimit", iChannelCountLimit);

#ifdef Q_OS_UNIX
	qsName = qsSettings->value("uname").toString();
	if (geteuid() == 0) {
		// TODO: remove this silent fallback to enforce running as non-root
		bool requested = true;
		if (qsName.isEmpty()) {
			// default server user name
			qsName = "mumble-server";
			requested = false;
		}
		struct passwd *pw = getpwnam(qPrintable(qsName));
		if (pw) {
			uiUid = pw->pw_uid;
			uiGid = pw->pw_gid;
			qsHome = pw->pw_dir;
		} else if (requested) {
			qFatal("Cannot find username %s", qPrintable(qsName));
		}
		endpwent();
	}
#endif

	qrUserName = QRegExp(typeCheckedFromSettings("username", qrUserName.pattern()));
	qrChannelName = QRegExp(typeCheckedFromSettings("channelname", qrChannelName.pattern()));

	iMessageLimit = typeCheckedFromSettings("messagelimit", 1);
	iMessageBurst = typeCheckedFromSettings("messageburst", 5);

	bool bObfuscate = typeCheckedFromSettings("obfuscate", false);
	if (bObfuscate) {
		qWarning("IP address obfuscation enabled.");
		iObfuscate = qrand();
	}
	bSendVersion = typeCheckedFromSettings("sendversion", bSendVersion);
	bAllowPing = typeCheckedFromSettings("allowping", bAllowPing);

	if (!loadSSLSettings()) {
		qFatal("MetaParams: Failed to load SSL settings. See previous errors.");
	}

	QStringList hosts;
	foreach(const QHostAddress &qha, qlBind) {
		hosts << qha.toString();
	}
	qmConfig.insert(QLatin1String("host"),hosts.join(" "));
	qmConfig.insert(QLatin1String("password"),qsPassword);
	qmConfig.insert(QLatin1String("port"),QString::number(usPort));
	qmConfig.insert(QLatin1String("timeout"),QString::number(iTimeout));
	qmConfig.insert(QLatin1String("textmessagelength"), QString::number(iMaxTextMessageLength));
	qmConfig.insert(QLatin1String("legacypasswordhash"), legacyPasswordHash ?  QLatin1String("true") : QLatin1String("false"));
	qmConfig.insert(QLatin1String("kdfiterations"), QString::number(kdfIterations));
	qmConfig.insert(QLatin1String("allowhtml"), bAllowHTML ? QLatin1String("true") : QLatin1String("false"));
	qmConfig.insert(QLatin1String("bandwidth"),QString::number(iMaxBandwidth));
	qmConfig.insert(QLatin1String("users"),QString::number(iMaxUsers));
	qmConfig.insert(QLatin1String("defaultchannel"),QString::number(iDefaultChan));
	qmConfig.insert(QLatin1String("rememberchannel"),bRememberChan ? QLatin1String("true") : QLatin1String("false"));
	qmConfig.insert(QLatin1String("welcometext"),qsWelcomeText);
	qmConfig.insert(QLatin1String("registername"),qsRegName);
	qmConfig.insert(QLatin1String("registerpassword"),qsRegPassword);
	qmConfig.insert(QLatin1String("registerhostname"),qsRegHost);
	qmConfig.insert(QLatin1String("registerlocation"), qsRegLocation);
	qmConfig.insert(QLatin1String("registerurl"),qurlRegWeb.toString());
	qmConfig.insert(QLatin1String("bonjour"), bBonjour ? QLatin1String("true") : QLatin1String("false"));
	qmConfig.insert(QLatin1String("certificate"),qscCert.toPem());
	qmConfig.insert(QLatin1String("key"),qskKey.toPem());
	qmConfig.insert(QLatin1String("obfuscate"),bObfuscate ? QLatin1String("true") : QLatin1String("false"));
	qmConfig.insert(QLatin1String("username"),qrUserName.pattern());
	qmConfig.insert(QLatin1String("channelname"),qrChannelName.pattern());
	qmConfig.insert(QLatin1String("certrequired"), bCertRequired ? QLatin1String("true") : QLatin1String("false"));
	qmConfig.insert(QLatin1String("forceExternalAuth"), bForceExternalAuth ? QLatin1String("true") : QLatin1String("false"));
	qmConfig.insert(QLatin1String("suggestversion"), qvSuggestVersion.isNull() ? QString() : qvSuggestVersion.toString());
	qmConfig.insert(QLatin1String("suggestpositional"), qvSuggestPositional.isNull() ? QString() : qvSuggestPositional.toString());
	qmConfig.insert(QLatin1String("suggestpushtotalk"), qvSuggestPushToTalk.isNull() ? QString() : qvSuggestPushToTalk.toString());
	qmConfig.insert(QLatin1String("opusthreshold"), QString::number(iOpusThreshold));
	qmConfig.insert(QLatin1String("channelnestinglimit"), QString::number(iChannelNestingLimit));
	qmConfig.insert(QLatin1String("channelcountlimit"), QString::number(iChannelCountLimit));
	qmConfig.insert(QLatin1String("sslCiphers"), qsCiphers);
	qmConfig.insert(QLatin1String("sslDHParams"), QString::fromLatin1(qbaDHParams.constData()));
}

bool MetaParams::loadSSLSettings() {
	QSettings updatedSettings(qsAbsSettingsFilePath, QSettings::IniFormat);
#if QT_VERSION >= 0x040500
	updatedSettings.setIniCodec("UTF-8");
#endif

	QString tmpCiphersStr = typeCheckedFromSettings("sslCiphers", qsCiphers);

	QString qsSSLCert = qsSettings->value("sslCert").toString();
	QString qsSSLKey = qsSettings->value("sslKey").toString();
	QString qsSSLCA = qsSettings->value("sslCA").toString();
	QString qsSSLDHParams = typeCheckedFromSettings(QLatin1String("sslDHParams"), QString(QLatin1String("@ffdhe2048")));

	qbaPassPhrase = qsSettings->value("sslPassPhrase").toByteArray();

	QSslCertificate tmpCert;
	QList<QSslCertificate> tmpCA;
	QList<QSslCertificate> tmpIntermediates;
	QSslKey tmpKey;
	QByteArray tmpDHParams;
	QList<QSslCipher> tmpCiphers;


	if (! qsSSLCA.isEmpty()) {
		QFile pem(qsSSLCA);
		if (pem.open(QIODevice::ReadOnly)) {
			QByteArray qba = pem.readAll();
			pem.close();
			QList<QSslCertificate> ql = QSslCertificate::fromData(qba);
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

	if (! qsSSLCert.isEmpty()) {
		QFile pem(qsSSLCert);
		if (pem.open(QIODevice::ReadOnly)) {
			crt = pem.readAll();
			pem.close();
		} else {
			qCritical("MetaParams: Failed to read %s", qPrintable(qsSSLCert));
			return false;
		}
	}
	if (! qsSSLKey.isEmpty()) {
		QFile pem(qsSSLKey);
		if (pem.open(QIODevice::ReadOnly)) {
			key = pem.readAll();
			pem.close();
		} else {
			qCritical("MetaParams: Failed to read %s", qPrintable(qsSSLKey));
			return false;
		}
	}

	if (! key.isEmpty() || ! crt.isEmpty()) {
		if (! key.isEmpty()) {
			tmpKey = Server::privateKeyFromPEM(key, qbaPassPhrase);
		}
		if (tmpKey.isNull() && ! crt.isEmpty()) {
			tmpKey = Server::privateKeyFromPEM(crt, qbaPassPhrase);
			if (! tmpKey.isNull())
				qCritical("MetaParams: Using private key found in certificate file.");
		}
		if (tmpKey.isNull()) {
			qCritical("MetaParams: No private key found in certificate or key file.");
			return false;
		}

		QList<QSslCertificate> ql = QSslCertificate::fromData(crt);
		ql << QSslCertificate::fromData(key);
		for (int i=0;i<ql.size(); ++i) {
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
			qCritical("MetaParams: Adding %d intermediate certificates from certificate file.", ql.size());
		}
	}

	QByteArray dhparams;

#if defined(USE_QSSLDIFFIEHELLMANPARAMETERS)
	if (! qsSSLDHParams.isEmpty()) {
		if (qsSSLDHParams.startsWith(QLatin1String("@"))) {
			QString group = qsSSLDHParams.mid(1).trimmed();
			QByteArray pem = FFDHE::PEMForNamedGroup(group);
			if (pem.isEmpty()) {
				QStringList names = FFDHE::NamedGroups();
				QStringList atNames;
				foreach (QString name, names) {
					atNames << QLatin1String("@") + name;
				}
				QString supported = atNames.join(QLatin1String(", "));
				qFatal("MetaParms: Diffie-Hellman parameters with name '%s' is not available. (Supported: %s)", qPrintable(qsSSLDHParams), qPrintable(supported));
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

	if (! dhparams.isEmpty()) {
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
	if (! qsSSLDHParamsIniValue.isEmpty()) {
		qFatal("MetaParams: This version of Murmur does not support Diffie-Hellman parameters (sslDHParams). Murmur will not start unless you remove the option from your murmur.ini file.");
		return false;
	}
#endif

	{
		QList<QSslCipher> ciphers = MumbleSSL::ciphersFromOpenSSLCipherString(tmpCiphersStr);
		if (ciphers.isEmpty()) {
			qCritical("MetaParams: Invalid sslCiphers option. Either the cipher string is invalid or none of the ciphers are available: \"%s\"", qPrintable(qsCiphers));
			return false;
		}

#if !defined(USE_QSSLDIFFIEHELLMANPARAMETERS)
		// If the version of Qt we're building against doesn't support
		// QSslDiffieHellmanParameters, then we must filter out Diffie-
		// Hellman cipher suites in order to guarantee that we do not
		// use Qt's default Diffie-Hellman parameters.
		{
			QList<QSslCipher> filtered;
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
		foreach (QSslCipher c, tmpCiphers) {
			pref << c.name();
		}
		qWarning("MetaParams: TLS cipher preference is \"%s\"", qPrintable(pref.join(QLatin1String(":"))));
	}

	qscCert = tmpCert;
	qlCA = tmpCA;
	qlIntermediates = tmpIntermediates;
	qskKey = tmpKey;
	qbaDHParams = tmpDHParams;
	qsCiphers = tmpCiphersStr;
	qlCiphers = tmpCiphers;

	qmConfig.insert(QLatin1String("certificate"), qscCert.toPem());
	qmConfig.insert(QLatin1String("key"), qskKey.toPem());
	qmConfig.insert(QLatin1String("sslCiphers"), qsCiphers);
	qmConfig.insert(QLatin1String("sslDHParams"), QString::fromLatin1(qbaDHParams.constData()));

	return true;
}

Meta::Meta() {
#ifdef Q_OS_WIN
	QOS_VERSION qvVer;
	qvVer.MajorVersion = 1;
	qvVer.MinorVersion = 0;

	hQoS = NULL;

	HMODULE hLib = LoadLibrary(L"qWave.dll");
	if (hLib == NULL) {
		qWarning("Meta: Failed to load qWave.dll, no QoS available");
	} else {
		FreeLibrary(hLib);
		if (! QOSCreateHandle(&qvVer, &hQoS))
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
		Connection::setQoS(NULL);
	}
#endif
}

bool Meta::reloadSSLSettings() {
	// Reload SSL settings.
	if (!Meta::mp.loadSSLSettings()) {
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
	qsOS = OSInfo::getOS();
	qsOSVersion = OSInfo::getOSDisplayableVersion();
}

void Meta::bootAll() {
	QList<int> ql = ServerDB::getBootServers();
	foreach(int snum, ql)
		boot(snum);
}

bool Meta::boot(int srvnum) {
	if (qhServers.contains(srvnum))
		return false;
	if (! ServerDB::serverExists(srvnum))
		return false;
	Server *s = new Server(srvnum, this);
	if (! s->bValid) {
		delete s;
		return false;
	}
	qhServers.insert(srvnum, s);
	emit started(s);

#ifdef Q_OS_UNIX
	unsigned int sockets = 19; // Base
	foreach(s, qhServers) {
		sockets += 11; // Listen sockets, signal pipes etc.
		sockets += s->iMaxUsers; // One per user
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
			qCritical("Current booted servers require minimum %d file descriptors when all slots are full, but only %lu file descriptors are allowed for this process. Your server will crash and burn; read the FAQ for details.", sockets, static_cast<unsigned long>(r.rlim_cur));
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
	foreach(Server *s, qhServers) {
		emit stopped(s);
		delete s;
	}
	qhServers.clear();
}

bool Meta::banCheck(const QHostAddress &addr) {
	if ((mp.iBanTries == 0) || (mp.iBanTimeframe == 0))
		return false;

	if (qhBans.contains(addr)) {
		Timer t = qhBans.value(addr);
		if (t.elapsed() < (1000000ULL * mp.iBanTime))
			return true;
		qhBans.remove(addr);
	}

	QList<Timer> &ql = qhAttempts[addr];

	ql.append(Timer());
	while (! ql.isEmpty() && (ql.at(0).elapsed() > (1000000ULL * mp.iBanTimeframe)))
		ql.removeFirst();

	if (ql.count() > mp.iBanTries) {
		qhBans.insert(addr, Timer());
		return true;
	}
	return false;
}
