// Copyright 2005-2018 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "TestPlugins.h"
#include "PlatformCheck.h"

#include <QtTest>

#ifdef Q_OS_WIN
#include <windows.h>
#include <tlhelp32.h>
#endif

class TestPlugins : public QObject
{
	Q_OBJECT

private:
	bool trylock();

private slots:
	void initTestCase();
	void check();
	void cleanupTestCase();

private:
	float avatarPosition[3], avatarFront[3], avatarTop[3];
	float cameraPosition[3], cameraFront[3], cameraTop[3];
	std::string context;
	std::wstring identity;

	QProcess process;
	QStringList games;
	QSharedPointer<PluginInfo> plugin;
	QList<QSharedPointer<PluginInfo>> plugins;
};

void TestPlugins::initTestCase()
{
	QDir pluginsDir("../plugins", QString(), QDir::Name, QDir::Files | QDir::Readable);
	QFileInfoList files = pluginsDir.entryInfoList();
	QSet<QString> loaded;

	foreach(const QFileInfo &file, files) {
		QString filePath = file.canonicalFilePath();
		if (!QLibrary::isLibrary(filePath)) {
			continue;
		}

		if (loaded.contains(filePath)) {
			qWarning() << "Plugin" << filePath <<  "already loaded!";
			continue;
		}

		QSharedPointer<PluginInfo> pi(new PluginInfo());
		pi->lib.setFileName(filePath);
		qDebug() << "Plugin:" << pi->lib.fileName();
		if (!pi->lib.load()) {
			qWarning() << "Failed to load" << pi->lib.fileName() << ":" << pi->lib.errorString();
			continue;
		}

		mumblePluginFunc mpf = reinterpret_cast<mumblePluginFunc>(pi->lib.resolve("getMumblePlugin"));
		if (!mpf) {
			qWarning() << "Failed to resolve getMumblePlugin():" << pi->lib.errorString();
			pi->lib.unload();
			continue;
		}

		pi->p = mpf();
		if (pi->p->magic != MUMBLE_PLUGIN_MAGIC) {
			qDebug("MUMBLE_PLUGIN_MAGIC not corresponding!");
			qDebug() << "TestPlugins:" << MUMBLE_PLUGIN_MAGIC;
			qDebug() << "Plugin:" << pi->p->magic;

			pi->lib.unload();
			continue;
		}

		pi->shortname = QString::fromStdWString(pi->p->shortname);
		qDebug() << "Shortname:" << pi->shortname;
		pi->description = QString::fromStdWString(pi->p->description);
		qDebug() << "Description:" << pi->description;

		mumblePlugin2Func mpf2 = reinterpret_cast<mumblePlugin2Func>(pi->lib.resolve("getMumblePlugin2"));
		if (mpf2) {
			pi->p2 = mpf2();
			if (pi->p2->magic != MUMBLE_PLUGIN_MAGIC_2) {
				qDebug("MUMBLE_PLUGIN_MAGIC_2 not corresponding!");
				qDebug() << "TestPlugins:" << MUMBLE_PLUGIN_MAGIC_2;
				qDebug() << "Plugin:" << pi->p2->magic;

				pi->p2 = NULL;
			}
		} else {
			qWarning() << "Failed to resolve getMumblePlugin2():" << pi->lib.errorString();
		}

		pi->tested = false;

		plugins << pi;
		loaded.insert(filePath);
	}

	QDir gamesDir("games", QString(), QDir::Name, QDir::Files | QDir::Readable);
	files = gamesDir.entryInfoList();
	foreach(const QFileInfo &file, files) {
		if (file.isExecutable()) {
			QString filePath = file.canonicalFilePath();
			qDebug() << "Game:" << filePath;
			games.append(filePath);
		}
	}

	process.setProcessChannelMode(QProcess::ForwardedChannels);
}

void TestPlugins::check()
{
	if (plugins.isEmpty()) {
		QSKIP("No plugins available!");
	}

	if (games.isEmpty()) {
		QSKIP("No test executables available!");
	}

	foreach(QFileInfo file, games) {
		if (PlatformCheck::IsWine()) {
			process.setProgram("wine");
			process.setArguments(QStringList(file.canonicalFilePath()));
		} else {
			process.setProgram(file.canonicalFilePath());
		}

		qDebug() << "Starting" << file.fileName();
		process.start();

		QVERIFY2(process.waitForStarted(), "Failed to start process.");

		// To ensure that the process is running
		QThread::sleep(5);

		QVERIFY2(trylock(), "Failed to find a suitable plugin for the current game.");

		qDebug() << "Testing plugin:" << plugin->lib.fileName();

		plugin->p->fetch(avatarPosition, avatarFront, avatarTop, cameraPosition, cameraFront, cameraTop, context, identity);

		QVERIFY2(avatarPosition[0] == 1.0f, qPrintable(QString("avatarPosition[0] is %1 instead of 1.0").arg(QString::number(avatarPosition[0]))));
		QVERIFY2(avatarPosition[1] == 2.0f, qPrintable(QString("avatarPosition[1] is %2 instead of 2.0").arg(QString::number(avatarPosition[0]))));
		QVERIFY2(avatarPosition[2] == 3.0f, qPrintable(QString("avatarPosition[2] is %3 instead of 3.0").arg(QString::number(avatarPosition[0]))));

		QVERIFY2(cameraPosition[0] == 1.0f, qPrintable(QString("cameraPosition[0] is %1 instead of 1.0").arg(QString::number(cameraPosition[0]))));
		QVERIFY2(cameraPosition[1] == 2.0f, qPrintable(QString("cameraPosition[1] is %1 instead of 2.0").arg(QString::number(cameraPosition[1]))));
		QVERIFY2(cameraPosition[2] == 3.0f, qPrintable(QString("cameraPosition[2] is %1 instead of 3.0").arg(QString::number(cameraPosition[2]))));

		QVERIFY2(avatarFront[0] == 1.0f, qPrintable(QString("avatarFront[0] is %1 instead of 1.0").arg(QString::number(avatarFront[0]))));
		QVERIFY2(avatarFront[1] == 2.0f, qPrintable(QString("avatarFront[1] is %1 instead of 2.0").arg(QString::number(avatarFront[1]))));
		QVERIFY2(avatarFront[2] == 3.0f, qPrintable(QString("avatarFront[2] is %1 instead of 3.0").arg(QString::number(avatarFront[2]))));

		QVERIFY2(cameraFront[0] == 1.0f, qPrintable(QString("cameraFront[0] is %1 instead of 1.0").arg(QString::number(cameraFront[0]))));
		QVERIFY2(cameraFront[1] == 2.0f, qPrintable(QString("cameraFront[1] is %1 instead of 2.0").arg(QString::number(cameraFront[1]))));
		QVERIFY2(cameraFront[2] == 3.0f, qPrintable(QString("cameraFront[2] is %1 instead of 3.0").arg(QString::number(cameraFront[2]))));

		/*
		QVERIFY2(avatarTop[0] == 1.0f, qPrintable(QString("avatarTop[0] is %1 instead of 1.0").arg(QString::number(avatarTop[0]))));
		QVERIFY2(avatarTop[1] == 2.0f, qPrintable(QString("avatarTop[1] is %1 instead of 2.0").arg(QString::number(avatarTop[1]))));
		QVERIFY2(avatarTop[2] == 3.0f, qPrintable(QString("avatarTop[2] is %1 instead of 3.0").arg(QString::number(avatarTop[2]))));
		*/

		QVERIFY2(avatarTop[0] == 0.0f, qPrintable(QString("avatarTop[0] is %1 instead of 0.0").arg(QString::number(avatarTop[0]))));
		QVERIFY2(avatarTop[1] == 0.0f, qPrintable(QString("avatarTop[1] is %1 instead of 0.0").arg(QString::number(avatarTop[1]))));
		QVERIFY2(avatarTop[2] == -1.0f, qPrintable(QString("avatarTop[2] is %1 instead of 0.0").arg(QString::number(avatarTop[2]))));

		QVERIFY2(cameraTop[0] == 1.0f, qPrintable(QString("cameraTop[0] is %1 instead of 1.0").arg(QString::number(cameraTop[0]))));
		QVERIFY2(cameraTop[1] == 2.0f, qPrintable(QString("cameraTop[1] is %1 instead of 2.0").arg(QString::number(cameraTop[1]))));
		QVERIFY2(cameraTop[2] == 3.0f, qPrintable(QString("cameraTop[2] is %1 instead of 3.0").arg(QString::number(cameraTop[2]))));

		plugin->tested = true;

		process.close();
	}
}

bool TestPlugins::trylock()
{
	int index = 0;

	while (index < plugins.count()) {
		std::multimap<std::wstring, unsigned long long int> pids;
#if defined(Q_OS_WIN)
		PROCESSENTRY32 pe;
		pe.dwSize = sizeof(pe);
		HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		if (hSnap != INVALID_HANDLE_VALUE) {
			BOOL ok = Process32First(hSnap, &pe);

			while (ok) {
				pids.insert(std::pair<std::wstring, unsigned long long int>(std::wstring(pe.szExeFile), pe.th32ProcessID));
				ok = Process32Next(hSnap, &pe);
			}

			CloseHandle(hSnap);
		}
#elif defined(Q_OS_LINUX)
		QDir d(QLatin1String("/proc"));
		QStringList entries = d.entryList();
		bool ok;

		foreach (const QString &entry, entries) {
			// Check if the entry is a PID
			// by checking whether it's a number.
			// If it is not, skip it.
			unsigned long long int pid = static_cast<unsigned long long int>(entry.toLongLong(&ok, 10));
			if (!ok) {
				continue;
			}

			QString exe = QFile::symLinkTarget(QString(QLatin1String("/proc/%1/exe")).arg(entry));
			QFileInfo fi(exe);
			QString firstPart = fi.baseName();
			QString completeSuffix = fi.completeSuffix();
			QString baseName;
			if (completeSuffix.isEmpty()) {
				baseName = firstPart;
			} else {
				baseName = firstPart + QLatin1String(".") + completeSuffix;
			}

			if (baseName == QLatin1String("wine-preloader") || baseName == QLatin1String("wine64-preloader")) {
				QFile f(QString(QLatin1String("/proc/%1/cmdline")).arg(entry));
				if (f.open(QIODevice::ReadOnly)) {
					QByteArray cmdline = f.readAll();
					f.close();

					int nul = cmdline.indexOf('\0');
					if (nul != -1) {
						cmdline.truncate(nul);
					}

					QString exe = QString::fromUtf8(cmdline);
					if (exe.contains(QLatin1String("\\"))) {
						int lastBackslash = exe.lastIndexOf(QLatin1String("\\"));
						if (exe.count() > lastBackslash + 1) {
							baseName = exe.mid(lastBackslash + 1);
						}
					}
				}
			}

			if (!baseName.isEmpty()) {
				pids.insert(std::pair<std::wstring, unsigned long long int>(baseName.toStdWString(), pid));
			}
		}
#endif

		QSharedPointer<PluginInfo> pi = plugins.at(index);
		if (pi->p2 ? pi->p2->trylock(pids) : pi->p->trylock()) {
			pi->shortname = QString::fromStdWString(pi->p->shortname);
			plugin = pi;
			return true;
		}

		++index;
	}

	return false;
}

void TestPlugins::cleanupTestCase()
{
	process.close();
	plugin.clear();

	foreach(QSharedPointer<PluginInfo> pi, plugins) {
		if (!pi->tested) {
			qWarning() << "Plugin" << pi->lib.fileName() << "has not been tested!";
		}

		pi.clear();
	}
}

QTEST_MAIN(TestPlugins)
#include "TestPlugins.moc"
