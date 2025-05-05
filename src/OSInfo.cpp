// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include <QtCore/QtGlobal>

#if defined(Q_OS_WIN)
#	include "win.h"
#	include "versionhelpers.h"
#endif

#include "OSInfo.h"

#include "Version.h"

#include <QtCore/QCryptographicHash>
#include <QtCore/QSysInfo>
#include <QtNetwork/QNetworkInterface>
#include <QtXml/QDomDocument>

#if defined(Q_OS_WIN)
#	include <intrin.h>
#endif

#if defined(Q_OS_MACOS)
#	include <Carbon/Carbon.h>
#	include <mach-o/arch.h>
#	include <sys/sysctl.h>
#	include <sys/types.h>

// Ignore deprecation warnings for Gestalt.
// See mumble-voip/mumble#3290 for more information.
#	pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

#if defined(Q_OS_WIN)
// regString converts a wchar_t string of size to a
// QString. If the string contains a NUL value, that
// NUL value will terminate the string.
static QString regString(wchar_t *string, int size) {
	if (size <= 0) {
		return QString();
	}
	// If string contains a NUL, adjust the size such
	// that the NUL is not included in the returned
	// string.
	const size_t adjustedSize = wcsnlen(string, static_cast< size_t >(size));
	// The return value of wcsnlen is <= size which is
	// an int, so casting adjustedSize to int is safe.
	return QString::fromWCharArray(string, static_cast< int >(adjustedSize));
}

/// Query for a Windows 10-style displayable version.
///
/// This returns a string of the kind:
///
///    Windows 10 Enterprise 2009 19042.804
///
/// which is:
///
///    $ProductName $Version $Build.$Ubr
///
/// Of note, $Version is formatted as YYMM.
/// So, 2009 is a Windows 10 update released in September 2020.
///
/// This function can be called on non-Windows 10 OSes.
/// On those, this functions fails to query some of the
/// registry keys used for building the version string
/// to be returned. Because of that, it is safe to call
/// this function, and if it returns an empty/null string,
/// a legacy version string can be displayed.
static QString win10DisplayableVersion() {
	HKEY key = 0;
	auto err = RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", 0, KEY_READ, &key);
	if (err != ERROR_SUCCESS) {
		RegCloseKey(key);
		return QString();
	}

	wchar_t buf[64];
	DWORD len = sizeof(buf);
	err       = RegQueryValueEx(key, L"ProductName", nullptr, nullptr, reinterpret_cast< LPBYTE >(&buf[0]), &len);
	if (err != ERROR_SUCCESS) {
		RegCloseKey(key);
		return QString();
	}
	const auto productName = regString(buf, static_cast< int >(len / sizeof(buf[0])));

	len = sizeof(buf);
	err = RegQueryValueEx(key, L"ReleaseId", nullptr, nullptr, reinterpret_cast< LPBYTE >(&buf[0]), &len);
	if (err != ERROR_SUCCESS) {
		RegCloseKey(key);
		return QString();
	}
	const auto releaseId = regString(buf, static_cast< int >(len / sizeof(buf[0])));

	len = sizeof(buf);
	err = RegQueryValueEx(key, L"CurrentBuild", nullptr, nullptr, reinterpret_cast< LPBYTE >(&buf[0]), &len);
	if (err != ERROR_SUCCESS) {
		RegCloseKey(key);
		return QString();
	}
	const auto currentBuild = regString(buf, static_cast< int >(len / sizeof(buf[0])));

	DWORD dw = 0;
	len      = sizeof(dw);
	err      = RegQueryValueEx(key, L"UBR", nullptr, nullptr, reinterpret_cast< LPBYTE >(&dw), &len);
	if (err != ERROR_SUCCESS) {
		RegCloseKey(key);
		return QString();
	}
	const auto ubr = QString::number(static_cast< ulong >(dw), 10);

	RegCloseKey(key);

	return QString::fromLatin1("%1 %2 %3.%4").arg(productName, releaseId, currentBuild, ubr);
}
#endif

QString OSInfo::getArchitecture(const bool build) {
	QString architecture = build ? QSysInfo::buildCpuArchitecture() : QSysInfo::currentCpuArchitecture();
	if (architecture == QLatin1String("x86_64")) {
		architecture = QLatin1String("x64");
	} else if (architecture == QLatin1String("i386")) {
		architecture = QLatin1String("x86");
	}

	return architecture;
}

QString OSInfo::getMacHash(const QList< QHostAddress > &qlBind) {
	QString first, second, third;
	for (const QNetworkInterface &qni : QNetworkInterface::allInterfaces()) {
		if (!qni.isValid())
			continue;
		if (qni.flags() & QNetworkInterface::IsLoopBack)
			continue;
		if (qni.hardwareAddress().isEmpty())
			continue;

		QString hash = QString::fromUtf8(
			QCryptographicHash::hash(qni.hardwareAddress().toUtf8(), QCryptographicHash::Sha1).toHex());

		if (third.isEmpty() || third > hash)
			third = hash;

		if (!(qni.flags() & (QNetworkInterface::IsUp | QNetworkInterface::IsRunning)))
			continue;

		if (second.isEmpty() || second > hash)
			second = hash;

		for (const QNetworkAddressEntry &qnae : qni.addressEntries()) {
			const QHostAddress &qha = qnae.ip();
			if (qlBind.isEmpty() || qlBind.contains(qha)) {
				if (first.isEmpty() || first > hash)
					first = hash;
			}
		}
	}
	if (!first.isEmpty())
		return first;
	if (!second.isEmpty())
		return second;
	if (!third.isEmpty())
		return third;
	return QString();
}

QString OSInfo::getOS() {
#if defined(Q_OS_WIN)
	return QLatin1String("Windows");
#elif defined(Q_OS_LINUX)
	return QLatin1String("Linux");
#elif defined(Q_OS_MACOS)
	return QLatin1String("macOS");
#elif defined(Q_OS_FREEBSD)
	return QLatin1String("FreeBSD");
#elif defined(Q_OS_NETBSD)
	return QLatin1String("NetBSD");
#elif defined(Q_OS_OPENBSD)
	return QLatin1String("OpenBSD");
#elif defined(Q_OS_BSD4)
	return QLatin1String("BSD");
#elif defined(Q_OS_UNIX)
	return QLatin1String("UNIX");
#else
	return QLatin1String("Unknown");
#endif
}

QString OSInfo::getOSDisplayableVersion(const bool appendArch) {
#if defined(Q_OS_WIN)
	QString os = win10DisplayableVersion();
	if (os.isEmpty()) {
		if (IsWindows10OrGreater()) {
			if (!IsWindowsServer()) {
				os = QLatin1String("Windows 10");
			} else {
				os = QLatin1String("Windows 10 Server");
			}
		} else if (IsWindows8Point1OrGreater()) {
			if (!IsWindowsServer()) {
				os = QLatin1String("Windows 8.1");
			} else {
				os = QLatin1String("Windows 8.1 Server");
			}
		} else if (IsWindows8OrGreater()) {
			if (!IsWindowsServer()) {
				os = QLatin1String("Windows 8");
			} else {
				os = QLatin1String("Windows 8 Server");
			}
		} else if (IsWindows7OrGreater()) {
			if (!IsWindowsServer()) {
				os = QLatin1String("Windows 7");
			} else {
				os = QLatin1String("Windows 7 Server");
			}
		} else if (IsWindowsVistaOrGreater()) {
			if (!IsWindowsServer()) {
				os = QLatin1String("Windows Vista");
			} else {
				os = QLatin1String("Windows Vista Server");
			}
		} else if (IsWindowsXPOrGreater()) {
			if (!IsWindowsServer()) {
				os = QLatin1String("Windows XP");
			} else {
				os = QLatin1String("Windows XP Server");
			}
		} else {
			os = QLatin1String("Ancient Windows version");
		}

		return os;
	}
#elif defined(Q_OS_MACOS)
	const QString os = QLatin1String("macOS ") + getOSVersion();
#else
	const QString os = QSysInfo::prettyProductName();
#endif
	if (!appendArch) {
		return os;
	}

	return os + QString(" [%1]").arg(getArchitecture(false));
}

QString OSInfo::getOSVersion() {
#if defined(Q_OS_MACOS)
	SInt32 major, minor, bugfix;
	OSErr err = Gestalt(gestaltSystemVersionMajor, &major);
	if (err == noErr)
		err = Gestalt(gestaltSystemVersionMinor, &minor);
	if (err == noErr)
		err = Gestalt(gestaltSystemVersionBugFix, &bugfix);
	if (err != noErr)
		return QSysInfo::productVersion();

	char *buildno = nullptr;
	char buildno_buf[32];
	size_t sz_buildno_buf = sizeof(buildno);
	int ret               = sysctlbyname("kern.osversion", buildno_buf, &sz_buildno_buf, nullptr, 0);
	if (ret == 0) {
		buildno = &buildno_buf[0];
	}

	return QString::asprintf("%lu.%lu.%lu %s", static_cast< unsigned long >(major), static_cast< unsigned long >(minor),
							 static_cast< unsigned long >(bugfix), buildno ? buildno : "unknown");
#else
	return QSysInfo::productVersion();
#endif
}

void OSInfo::fillXml(QDomDocument &doc, QDomElement &root, const QList< QHostAddress > &qlBind) {
	QDomElement tag = doc.createElement(QLatin1String("machash"));
	root.appendChild(tag);
	QDomText t = doc.createTextNode(getMacHash(qlBind));
	tag.appendChild(t);

	tag = doc.createElement(QLatin1String("arch"));
	root.appendChild(tag);
	t = doc.createTextNode(getArchitecture(true));
	tag.appendChild(t);

	tag = doc.createElement(QLatin1String("version"));
	root.appendChild(tag);
	t = doc.createTextNode(Version::getRelease());
	tag.appendChild(t);

	tag = doc.createElement(QLatin1String("release"));
	root.appendChild(tag);
	t = doc.createTextNode(Version::getRelease());
	tag.appendChild(t);

	tag = doc.createElement(QLatin1String("os"));
	root.appendChild(tag);
	t = doc.createTextNode(getOS());
	tag.appendChild(t);

	tag = doc.createElement(QLatin1String("osarch"));
	root.appendChild(tag);
	t = doc.createTextNode(getArchitecture(false));
	tag.appendChild(t);

	tag = doc.createElement(QLatin1String("osver"));
	root.appendChild(tag);
	t = doc.createTextNode(getOSVersion());
	tag.appendChild(t);

	tag = doc.createElement(QLatin1String("osverbose"));
	root.appendChild(tag);
	t = doc.createTextNode(getOSDisplayableVersion(false));
	tag.appendChild(t);

	tag = doc.createElement(QLatin1String("qt"));
	root.appendChild(tag);
	t = doc.createTextNode(QString::fromLatin1(qVersion()));
	tag.appendChild(t);

	QString cpu_id, cpu_extid;
	bool bSSE2 = false;
#if defined(Q_OS_WIN)
#	define regstr(x) QString::fromLatin1(reinterpret_cast< const char * >(&x), 4)
	int chop;
	int cpuinfo[4];

	__cpuid(cpuinfo, 1);
	bSSE2 = (cpuinfo[3] & 0x04000000);

	__cpuid(cpuinfo, 0);

	cpu_id = regstr(cpuinfo[1]) + regstr(cpuinfo[3]) + regstr(cpuinfo[2]);

	for (unsigned int j = 2; j <= 4; ++j) {
		__cpuid(cpuinfo, 0x80000000 + j);
		cpu_extid += regstr(cpuinfo[0]) + regstr(cpuinfo[1]) + regstr(cpuinfo[2]) + regstr(cpuinfo[3]);
	}

	cpu_id = cpu_id.trimmed();
	chop   = cpu_id.indexOf(QLatin1Char('\0'));
	if (chop != -1)
		cpu_id.truncate(chop);

	cpu_extid = cpu_extid.trimmed();
	chop      = cpu_extid.indexOf(QLatin1Char('\0'));
	if (chop != -1)
		cpu_extid.truncate(chop);
#endif

	tag = doc.createElement(QLatin1String("cpu_id"));
	root.appendChild(tag);
	t = doc.createTextNode(cpu_id);
	tag.appendChild(t);

	tag = doc.createElement(QLatin1String("cpu_extid"));
	root.appendChild(tag);
	t = doc.createTextNode(cpu_extid);
	tag.appendChild(t);

	tag = doc.createElement(QLatin1String("cpu_sse2"));
	root.appendChild(tag);
	t = doc.createTextNode(QString::number(bSSE2 ? 1 : 0));
	tag.appendChild(t);
}
