/* Copyright (C) 2005-2008, Thorvald Natvig <thorvald@natvig.com>

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

#include "ServerDB.h"
#include "Server.h"
#include "Meta.h"
#include "DBus.h"

MetaParams Meta::mp;

MetaParams::MetaParams() {
	qsPassword = QString();
	usPort = 64738;
	iTimeout = 30;
	iMaxBandwidth = 10000;
	iMaxUsers = 1000;
	iDefaultChan = 0;
	qsWelcomeText = QString("Welcome to this server");
	qsDatabase = QString();
	iDBPort = 0;
	qsDBusService = "net.sourceforge.mumble.murmur";
	qsDBDriver = "QSQLITE";
	qsLogfile = "murmur.log";
	qhaBind = QHostAddress(QHostAddress::Any);

	iLogDays = 31;

	iBanTries = 10;
	iBanTimeframe = 120;
	iBanTime = 300;
}

void MetaParams::read(QString fname) {
	if (fname.isEmpty()) {
		QStringList datapaths;

#if defined(Q_OS_WIN)
		size_t reqSize;
		_wgetenv_s(&reqSize, NULL, 0, L"APPDATA");
		if (reqSize > 0) {
			STACKVAR(wchar_t, buff, reqSize+1);
			_wgetenv_s(&reqSize, buff, reqSize, L"APPDATA");

			QDir appdir = QDir(QDir::fromNativeSeparators(QString::fromWCharArray(buff)));

			datapaths << appdir.absolutePath() + QLatin1String("/Mumble");
		}
#elif defined(Q_OS_MAC)
		datapaths << QDir::homePath() + QLatin1String("/Library/Preferences/Mumble/");
#else
		datapaths << QDir::homePath() + QLatin1String("/.config/Mumble");
#endif
		datapaths << QDir::homePath();
		datapaths << QDir::currentPath();
		datapaths << QCoreApplication::instance()->applicationDirPath();

		foreach(const QString &p, datapaths) {
			QFileInfo fi(p, "murmur.ini");
			if (fi.exists() && fi.isReadable()) {
				qdBasePath = QDir(p);
				fname = fi.absoluteFilePath();
				break;
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
	QSettings qs(fname, QSettings::IniFormat);

	qWarning("Initializing settings from %s (basepath %s)", qPrintable(qs.fileName()), qPrintable(qdBasePath.absolutePath()));

	QString qsHost = qs.value("host", QString()).toString();
	if (! qsHost.isEmpty()) {
		if (! qhaBind.setAddress(qsHost)) {
			QHostInfo hi = QHostInfo::fromName(qsHost);
			foreach(QHostAddress qha, hi.addresses()) {
				if (qha.protocol() == QAbstractSocket::IPv4Protocol) {
					qhaBind = qha;
					break;
				}
			}
			if ((qhaBind == QHostAddress::Any) || (qhaBind.isNull())) {
				qFatal("Lookup of bind hostname %s failed", qPrintable(qsHost));
			}

		}
		qWarning("Binding to address %s", qPrintable(qhaBind.toString()));
	}

	qsPassword = qs.value("serverpassword", qsPassword).toString();
	usPort = static_cast<unsigned short>(qs.value("port", usPort).toUInt());
	iTimeout = qs.value("timeout", iTimeout).toInt();
	iMaxBandwidth = qs.value("bandwidth", iMaxBandwidth).toInt();
	iDefaultChan = qs.value("defaultchannel", iDefaultChan).toInt();
	iMaxUsers = qs.value("users", iMaxUsers).toInt();
	qsWelcomeText = qs.value("welcometext", qsWelcomeText).toString();

	qsDatabase = qs.value("database", qsDatabase).toString();

	qsDBDriver = qs.value("dbDriver", qsDBDriver).toString();
	qsDBUserName = qs.value("dbUsername", qsDBUserName).toString();
	qsDBPassword = qs.value("dbPassword", qsDBPassword).toString();
	qsDBHostName = qs.value("dbHost", qsDBHostName).toString();
	qsDBPrefix = qs.value("dbPrefix", qsDBPrefix).toString();
	qsIceEndpoint = qs.value("ice", qsIceEndpoint).toString();
	iDBPort = qs.value("dbPort", iDBPort).toInt();

	iLogDays = qs.value("logdays", iLogDays).toInt();

	qsDBus = qs.value("dbus", qsDBus).toString();
	qsDBusService = qs.value("dbusservice", qsDBusService).toString();
	qsLogfile = qs.value("logfile", qsLogfile).toString();
	qsPid = qs.value("pidfile", qsPid).toString();

	qsRegName = qs.value("registerName", qsRegName).toString();
	qsRegPassword = qs.value("registerPassword", qsRegPassword).toString();
	qsRegHost = qs.value("registerHostname", qsRegHost).toString();
	qurlRegWeb = QUrl(qs.value("registerUrl", qurlRegWeb.toString()).toString());

	iBanTries = qs.value("autobanAttempts", iBanTries).toInt();
	iBanTimeframe = qs.value("autobanTimeframe", iBanTimeframe).toInt();
	iBanTime = qs.value("autobanTime", iBanTime).toInt();

	QString qsSSLCert = qs.value("sslCert").toString();
	QString qsSSLKey = qs.value("sslKey").toString();
	
	qbaPassPhrase = qs.value("sslPassPhrase").toByteArray();

	QByteArray crt, key;

	if (! qsSSLCert.isEmpty()) {
		QFile pem(qsSSLCert);
		if (pem.open(QIODevice::ReadOnly)) {
			crt = pem.readAll();
			pem.close();
		} else {
			qWarning("Failed to read %s", qPrintable(qsSSLCert));
		}
	}
	if (! qsSSLKey.isEmpty()) {
		QFile pem(qsSSLKey);
		if (pem.open(QIODevice::ReadOnly)) {
			key = pem.readAll();
			pem.close();
		} else {
			qWarning("Failed to read %s", qPrintable(qsSSLKey));
		}
	}

	if (! crt.isEmpty()) {
		qscCert = QSslCertificate(crt);
		if (qscCert.isNull()) {
			qWarning("Failed to parse certificate.");
		}
	}

	if (! key.isEmpty() && qscCert.isNull()) {
		qscCert = QSslCertificate(key);
		if (! qscCert.isNull()) {
			qWarning("Using certificate from key file.");
		}
	}

	if (! qscCert.isNull()) {
		QSsl::KeyAlgorithm alg = qscCert.publicKey().algorithm();

		if (! key.isEmpty()) {
			qskKey = QSslKey(key, alg, QSsl::Pem, QSsl::PrivateKey, qbaPassPhrase);
			if (qskKey.isNull()) {
				qWarning("Failed to parse key file.");
			}
		}

		if (! crt.isEmpty() && qskKey.isNull()) {
			qskKey = QSslKey(crt, alg, QSsl::Pem, QSsl::PrivateKey, qbaPassPhrase);
			if (! qskKey.isNull()) {
				qWarning("Using key from certificate file.");
			}
		}
		if (qskKey.isNull())
			qscCert = QSslCertificate();
	}

	if (qscCert.isNull() || qskKey.isNull()) {
		if (! key.isEmpty() || ! crt.isEmpty()) {
			qFatal("Certificate specified, but failed to load.");
		}
	}

	qmConfig.clear();
	qmConfig.insert(QLatin1String("host"),qhaBind.toString());
	qmConfig.insert(QLatin1String("password"),qsPassword);
	qmConfig.insert(QLatin1String("port"),QString::number(usPort));
	qmConfig.insert(QLatin1String("timeout"),QString::number(iTimeout));
	qmConfig.insert(QLatin1String("bandwidth"),QString::number(iMaxBandwidth));
	qmConfig.insert(QLatin1String("users"),QString::number(iMaxUsers));
	qmConfig.insert(QLatin1String("defaultchannel"),QString::number(iDefaultChan));
	qmConfig.insert(QLatin1String("welcometext"),qsWelcomeText);
	qmConfig.insert(QLatin1String("registername"),qsRegName);
	qmConfig.insert(QLatin1String("registerpassword"),qsRegPassword);
	qmConfig.insert(QLatin1String("registerhostname"),qsRegHost);
	qmConfig.insert(QLatin1String("registerurl"),qurlRegWeb.toString());
	qmConfig.insert(QLatin1String("certificate"),qscCert.toPem());
	qmConfig.insert(QLatin1String("key"),qskKey.toPem());
}

Meta::Meta() {
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
	s->start(QThread::HighestPriority);
	qhServers.insert(srvnum, s);
	return true;
}

void Meta::kill(int srvnum) {
	Server *s = qhServers.take(srvnum);
	if (!s)
		return;
	delete s;
}

void Meta::killAll() {
	foreach(Server *s, qhServers) {
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
