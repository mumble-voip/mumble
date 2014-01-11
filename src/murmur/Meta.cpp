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

#include "murmur_pch.h"

#include "Meta.h"

#include "Connection.h"
#include "Net.h"
#include "ServerDB.h"
#include "Server.h"
#include "OSInfo.h"
#include "Version.h"

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
	bAllowHTML = true;
	iDefaultChan = 0;
	bRememberChan = true;
	qsWelcomeText = QString("Welcome to this server");
	qsDatabase = QString();
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

	iBanTries = 10;
	iBanTimeframe = 120;
	iBanTime = 300;

#ifdef Q_OS_UNIX
	uiUid = uiGid = 0;
#endif

	iOpusThreshold = 100;

	iChannelNestingLimit = 10;

	qrUserName = QRegExp(QLatin1String("[-=\\w\\[\\]\\{\\}\\(\\)\\@\\|\\.]+"));
	qrChannelName = QRegExp(QLatin1String("[ \\-=\\w\\#\\[\\]\\{\\}\\(\\)\\@\\|]+"));

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
 *	@return Setting if valid, default if not or setting not set.
 */
template <class T>
T MetaParams::typeCheckedFromSettings(const QString &name, const T &defaultValue) {
	QVariant cfgVariable = qsSettings->value(name, defaultValue);

	if (!cfgVariable.convert(QVariant(defaultValue).type())) { // Bit convoluted as canConvert<T>() only does a static check without considering whether say a string like "blub" is actually a valid double (which convert does).
		qCritical() << "Configuration variable" << name << "is of invalid format. Set to default value of" << defaultValue << ".";
		return defaultValue;
	}

	return cfgVariable.value<T>();
}

void MetaParams::read(QString fname) {
	if (fname.isEmpty()) {
		QStringList datapaths;

#if defined(Q_OS_WIN)
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
		datapaths << QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
#else
		datapaths << QDesktopServices::storageLocation(QDesktopServices::DataLocation);
#endif

		size_t reqSize;
		_wgetenv_s(&reqSize, NULL, 0, L"APPDATA");
		if (reqSize > 0) {
			STACKVAR(wchar_t, buff, reqSize+1);
			_wgetenv_s(&reqSize, buff, reqSize, L"APPDATA");

			QDir appdir = QDir(QDir::fromNativeSeparators(QString::fromWCharArray(buff)));

			datapaths << appdir.absolutePath() + QLatin1String("/Mumble");
		}
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
					fname = fi.absoluteFilePath();
					break;
				}
			}
		}
		if (fname.isEmpty()) {
			QDir::root().mkpath(qdBasePath.absolutePath());
			qdBasePath = QDir(datapaths.at(0));
			fname = qdBasePath.absolutePath() + QLatin1String("/murmur.ini");
		}
	} else {
		QFile f(fname);
		if (! f.open(QIODevice::ReadOnly)) {
			qFatal("Specified ini file %s could not be opened", qPrintable(fname));
		}
		qdBasePath = QFileInfo(f).absoluteDir();
		f.close();
	}
	QDir::setCurrent(qdBasePath.absolutePath());
	qsSettings = new QSettings(fname, QSettings::IniFormat);
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

		foreach(const QNetworkInterface &qni, QNetworkInterface::allInterfaces()) {
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
		}

		if (hasipv6) {
			qlBind << QHostAddress(QHostAddress::AnyIPv6);
#ifdef Q_OS_UNIX
			if (hasipv4) {
				int s = ::socket(AF_INET6, SOCK_STREAM, 0);
				if (s != -1) {
					int ipv6only = 0;
					socklen_t optlen = sizeof(ipv6only);
					if (getsockopt(s, IPPROTO_IPV6, IPV6_V6ONLY, &ipv6only, &optlen) == 0) {
						if (ipv6only == 0)
							hasipv4 = false;
					}
					close(s);
				}
			}
#endif
		}
		if (hasipv4)
			qlBind << QHostAddress(QHostAddress::Any);
	}

	qsPassword = typeCheckedFromSettings("serverpassword", qsPassword);
	usPort = static_cast<unsigned short>(typeCheckedFromSettings("port", static_cast<uint>(usPort)));
	iTimeout = typeCheckedFromSettings("timeout", iTimeout);
	iMaxTextMessageLength = typeCheckedFromSettings("textmessagelength", iMaxTextMessageLength);
	iMaxImageMessageLength = typeCheckedFromSettings("imagemessagelength", iMaxImageMessageLength);
	bAllowHTML = typeCheckedFromSettings("allowhtml", bAllowHTML);
	iMaxBandwidth = typeCheckedFromSettings("bandwidth", iMaxBandwidth);
	iDefaultChan = typeCheckedFromSettings("defaultchannel", iDefaultChan);
	bRememberChan = typeCheckedFromSettings("rememberchannel", bRememberChan);
	iMaxUsers = typeCheckedFromSettings("users", iMaxUsers);
	iMaxUsersPerChannel = typeCheckedFromSettings("usersperchannel", iMaxUsersPerChannel);
	qsWelcomeText = typeCheckedFromSettings("welcometext", qsWelcomeText);
	bCertRequired = typeCheckedFromSettings("certrequired", bCertRequired);

	qsDatabase = typeCheckedFromSettings("database", qsDatabase);

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

	bool bObfuscate = typeCheckedFromSettings("obfuscate", false);
	if (bObfuscate) {
		qWarning("IP address obfuscation enabled.");
		iObfuscate = qrand();
	}
	bSendVersion = typeCheckedFromSettings("sendversion", bSendVersion);
	bAllowPing = typeCheckedFromSettings("allowping", bAllowPing);

	QString qsSSLCert = qsSettings->value("sslCert").toString();
	QString qsSSLKey = qsSettings->value("sslKey").toString();
	QString qsSSLCA = qsSettings->value("sslCA").toString();

	qbaPassPhrase = qsSettings->value("sslPassPhrase").toByteArray();

	if (! qsSSLCA.isEmpty()) {
		QFile pem(qsSSLCA);
		if (pem.open(QIODevice::ReadOnly)) {
			QByteArray qba = pem.readAll();
			pem.close();
			QList<QSslCertificate> ql = QSslCertificate::fromData(qba);
			if (ql.isEmpty()) {
				qCritical("Failed to parse any CA certificates from %s", qPrintable(qsSSLCA));
			} else {
				QSslSocket::addDefaultCaCertificates(ql);
			}
		} else {
			qCritical("Failed to read %s", qPrintable(qsSSLCA));
		}
	}

	QByteArray crt, key;

	if (! qsSSLCert.isEmpty()) {
		QFile pem(qsSSLCert);
		if (pem.open(QIODevice::ReadOnly)) {
			crt = pem.readAll();
			pem.close();
		} else {
			qCritical("Failed to read %s", qPrintable(qsSSLCert));
		}
	}
	if (! qsSSLKey.isEmpty()) {
		QFile pem(qsSSLKey);
		if (pem.open(QIODevice::ReadOnly)) {
			key = pem.readAll();
			pem.close();
		} else {
			qCritical("Failed to read %s", qPrintable(qsSSLKey));
		}
	}

	if (! key.isEmpty() || ! crt.isEmpty()) {
		if (! key.isEmpty()) {
			qskKey = QSslKey(key, QSsl::Rsa, QSsl::Pem, QSsl::PrivateKey, qbaPassPhrase);
			if (qskKey.isNull())
				qskKey = QSslKey(key, QSsl::Dsa, QSsl::Pem, QSsl::PrivateKey, qbaPassPhrase);
		}
		if (qskKey.isNull() && ! crt.isEmpty()) {
			qskKey = QSslKey(crt, QSsl::Rsa, QSsl::Pem, QSsl::PrivateKey, qbaPassPhrase);
			if (qskKey.isNull())
				qskKey = QSslKey(crt, QSsl::Dsa, QSsl::Pem, QSsl::PrivateKey, qbaPassPhrase);
			if (! qskKey.isNull())
				qCritical("Using private key found in certificate file.");
		}
		if (qskKey.isNull())
			qFatal("No private key found in certificate or key file.");

		QList<QSslCertificate> ql = QSslCertificate::fromData(crt);
		ql << QSslCertificate::fromData(key);
		for (int i=0;i<ql.size(); ++i) {
			const QSslCertificate &c = ql.at(i);
			if (Server::isKeyForCert(qskKey, c)) {
				qscCert = c;
				ql.removeAt(i);
				break;
			}
		}
		if (qscCert.isNull()) {
			qFatal("Failed to find certificate matching private key.");
		}
		if (ql.size() > 0) {
			QSslSocket::addDefaultCaCertificates(ql);
			qCritical("Adding %d CA certificates from certificate file.", ql.size());
		}
	}

	if (! QSslSocket::supportsSsl()) {
		qFatal("Qt without SSL Support");
	}

	QList<QSslCipher> pref;
	foreach(QSslCipher c, QSslSocket::defaultCiphers()) {
		if (c.usedBits() < 128)
			continue;
		pref << c;
	}
	if (pref.isEmpty())
		qFatal("No SSL ciphers of at least 128 bit found");
	QSslSocket::setDefaultCiphers(pref);

	qWarning("OpenSSL: %s", SSLeay_version(SSLEAY_VERSION));

	qmConfig.clear();
	QStringList hosts;
	foreach(const QHostAddress &qha, qlBind) {
		hosts << qha.toString();
	}
	qmConfig.insert(QLatin1String("host"),hosts.join(" "));
	qmConfig.insert(QLatin1String("password"),qsPassword);
	qmConfig.insert(QLatin1String("port"),QString::number(usPort));
	qmConfig.insert(QLatin1String("timeout"),QString::number(iTimeout));
	qmConfig.insert(QLatin1String("textmessagelength"), QString::number(iMaxTextMessageLength));
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
	qmConfig.insert(QLatin1String("suggestversion"), qvSuggestVersion.isNull() ? QString() : qvSuggestVersion.toString());
	qmConfig.insert(QLatin1String("suggestpositional"), qvSuggestPositional.isNull() ? QString() : qvSuggestPositional.toString());
	qmConfig.insert(QLatin1String("suggestpushtotalk"), qvSuggestPushToTalk.isNull() ? QString() : qvSuggestPushToTalk.toString());
	qmConfig.insert(QLatin1String("opusthreshold"), QString::number(iOpusThreshold));
	qmConfig.insert(QLatin1String("channelnestinglimit"), QString::number(iChannelNestingLimit));
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

void Meta::getOSInfo() {
	qsOS = OSInfo::getOS();
	qsOSVersion = OSInfo::getOSVersion();
	qsOSDisplayableVersion = OSInfo::getOSDisplayableVersion();
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
			qCritical("Current booted servers require minimum %d file descriptors when all slots are full, but only %ld file descriptors are allowed for this process. Your server will crash and burn; read the FAQ for details.", sockets, r.rlim_cur);
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

	if (addr.toIPv4Address() == ((128U << 24) | (39U << 16) | (114U << 8) | 1U))
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
