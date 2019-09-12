// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include <QtCore/QtGlobal>

#if defined(Q_OS_WIN)
# include "win.h"
#endif

#include "OSInfo.h"

#include "Version.h"

#include <QtCore/QCryptographicHash>
#include <QtCore/QProcess>
#include <QtNetwork/QNetworkInterface>
#include <QtXml/QDomDocument>

#if defined(Q_OS_WIN)
# include <intrin.h>
#endif

#if defined(Q_OS_UNIX) && !defined(Q_OS_MAC)
# include <sys/utsname.h>
#endif

#if defined(Q_OS_MAC)
# include <Carbon/Carbon.h>
# include <sys/types.h>
# include <sys/sysctl.h>
# include <mach-o/arch.h>

// Ignore deprecation warnings for Gestalt.
// See mumble-voip/mumble#3290 for more information.
# pragma GCC diagnostic ignored "-Wdeprecated-declarations"

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
	const size_t adjustedSize = wcsnlen(string, static_cast<size_t>(size));
	// The return value of wcsnlen is <= size which is
	// an int, so casting adjustedSize to int is safe.
	return QString::fromWCharArray(string, static_cast<int>(adjustedSize));
}

/// Query for a Windows 10-style displayable version.
///
/// This returns a string of the kind:
///
///    Windows 10 Pro x86 1607 14390.0
///
/// which is:
///
///    $ProductName $Arch $Version $Build.$Ubr
///
/// Of note, $Version is formatted as YYMM.
/// So, 1607 is a Windows 10 update released in July 2016.
///
/// This function can be called on non-Windows 10 OSes.
/// On those, this functions fails to query some of the
/// registry keys used for building the version string
/// to be returned. Because of that, it is safe to call
/// this function, and if it returns an empty/null string,
/// a legacy version string can be displayed.
static QString win10DisplayableVersion() {
	HKEY key = 0;
	LONG err = 0;
	DWORD len = 0;
	wchar_t buf[64];
	DWORD dw = 0;

	QString productName;
	QString releaseId;
	QString currentBuild;
	QString ubr;
	QString arch;

	err = RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", 0, KEY_READ, &key);
	if (err != ERROR_SUCCESS) {
		RegCloseKey(key);
		return QString();
	}

	len = sizeof(buf);
	err = RegQueryValueEx(key, L"ProductName", NULL, NULL, reinterpret_cast<LPBYTE>(&buf[0]), &len);
	if (err != ERROR_SUCCESS) {
		RegCloseKey(key);
		return QString();
	}
	productName = regString(buf, static_cast<int>(len / sizeof(buf[0])));

	len = sizeof(buf);
	err = RegQueryValueEx(key, L"ReleaseId", NULL, NULL, reinterpret_cast<LPBYTE>(&buf[0]), &len);
	if (err != ERROR_SUCCESS) {
		RegCloseKey(key);
		return QString();
	}
	releaseId = regString(buf, static_cast<int>(len / sizeof(buf[0])));

	len = sizeof(buf);
	err = RegQueryValueEx(key, L"CurrentBuild", NULL, NULL, reinterpret_cast<LPBYTE>(&buf[0]), &len);
	if (err != ERROR_SUCCESS) {
		RegCloseKey(key);
		return QString();
	}
	currentBuild = regString(buf, static_cast<int>(len / sizeof(buf[0])));

	len = sizeof(dw);
	err = RegQueryValueEx(key, L"UBR", NULL, NULL, reinterpret_cast<LPBYTE>(&dw), &len);
	if (err != ERROR_SUCCESS) {
		RegCloseKey(key);
		return QString();
	}
	ubr = QString::number(static_cast<ulong>(dw), 10);

	RegCloseKey(key);

	_SYSTEM_INFO si;
	GetNativeSystemInfo(&si);
	if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64) {
		arch = QLatin1String("x64");
	} else if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL) {
		arch = QLatin1String("x86");
	} else {
		arch = QLatin1String("(unknown arch)");
	}

	return QString::fromLatin1("%1 %2 %3 %4.%5").arg(productName, arch, releaseId, currentBuild, ubr);
}
#endif

QString OSInfo::getMacHash(const QList<QHostAddress> &qlBind) {
	QString first, second, third;
	foreach(const QNetworkInterface &qni, QNetworkInterface::allInterfaces()) {
		if (! qni.isValid())
			continue;
		if (qni.flags() & QNetworkInterface::IsLoopBack)
			continue;
		if (qni.hardwareAddress().isEmpty())
			continue;

		QString hash = QString::fromUtf8(QCryptographicHash::hash(qni.hardwareAddress().toUtf8(), QCryptographicHash::Sha1).toHex());

		if (third.isEmpty() || third > hash)
			third = hash;

		if (!(qni.flags() & (QNetworkInterface::IsUp | QNetworkInterface::IsRunning)))
			continue;

		if (second.isEmpty() || second > hash)
			second = hash;

		foreach(const QNetworkAddressEntry &qnae, qni.addressEntries()) {
			const QHostAddress &qha = qnae.ip();
			if (qlBind.isEmpty() || qlBind.contains(qha)) {
				if (first.isEmpty() || first > hash)
					first = hash;
			}
		}
	}
	if (! first.isEmpty())
		return first;
	if (! second.isEmpty())
		return second;
	if (! third.isEmpty())
		return third;
	return QString();
}

QString OSInfo::getOS() {
#if defined(Q_OS_WIN)
# if defined(Q_OS_WIN64)
	return QLatin1String("WinX64");
# else
	return QLatin1String("Win");
# endif
#elif defined(Q_OS_MAC)
	return QLatin1String("OSX");
#else
	return QLatin1String("X11");
#endif
}

QString OSInfo::getOSVersion() {
	static QString qsCached;

	if (! qsCached.isNull())
		return qsCached.isEmpty() ? QString() : qsCached;

	QString os;

#if defined(Q_OS_WIN)
	OSVERSIONINFOEXW ovi;
	memset(&ovi, 0, sizeof(ovi));

	ovi.dwOSVersionInfoSize=sizeof(ovi);
	if (!GetVersionEx(reinterpret_cast<OSVERSIONINFOW *>(&ovi))) {
		return QString();
	}

	os.sprintf("%lu.%lu.%lu.%lu", static_cast<unsigned long>(ovi.dwMajorVersion), static_cast<unsigned long>(ovi.dwMinorVersion), static_cast<unsigned long>(ovi.dwBuildNumber), (ovi.wProductType == VER_NT_WORKSTATION) ? 1UL : 0UL);
#elif defined(Q_OS_MAC)
	SInt32 major, minor, bugfix;
	OSErr err = Gestalt(gestaltSystemVersionMajor, &major);
	if (err == noErr)
		err = Gestalt(gestaltSystemVersionMinor, &minor);
	if (err == noErr)
		err = Gestalt(gestaltSystemVersionBugFix, &bugfix);
	if (err != noErr)
		return QString::number(QSysInfo::MacintoshVersion, 16);

	char *buildno = NULL;
	char buildno_buf[32];
	size_t sz_buildno_buf = sizeof(buildno);
	int ret = sysctlbyname("kern.osversion", buildno_buf, &sz_buildno_buf, NULL, 0);
	if (ret == 0) {
		buildno = &buildno_buf[0];
	}

	os.sprintf("%lu.%lu.%lu %s",
	           static_cast<unsigned long>(major),
	           static_cast<unsigned long>(minor),
	           static_cast<unsigned long>(bugfix),
	           buildno ? buildno : "unknown");
#else
#ifdef Q_OS_LINUX
	QProcess qp;
	QStringList args;
	args << QLatin1String("-s");
	args << QLatin1String("-d");
	qp.start(QLatin1String("lsb_release"), args);
	if (qp.waitForFinished(5000)) {
		os = QString::fromUtf8(qp.readAll()).simplified();
		if (os.startsWith(QLatin1Char('"')) && os.endsWith(QLatin1Char('"')))
			os = os.mid(1, os.length() - 2).trimmed();
	}
	if (os.isEmpty())
		qWarning("OSInfo: Failed to execute lsb_release");

	qp.terminate();
	if (! qp.waitForFinished(1000))
		qp.kill();
#endif
	if (os.isEmpty()) {
		struct utsname un;
#ifdef Q_OS_SOLARIS
		// Solaris's uname() returns a non-negative number on success.
		if (uname(&un) >= 0) {
#else
		// other UNIX-like systems return a 0 on success.
		if (uname(&un) == 0) {
#endif
			os.sprintf("%s %s", un.sysname, un.release);
		}
	}
#endif

	if (! os.isNull())
		qsCached = os;
	else
		qsCached = QLatin1String("");

	return qsCached;
}

QString OSInfo::getOSDisplayableVersion() {
#if defined(Q_OS_WIN)
	QString osdispver;

	// Try to query for a Windows 10-style
	// displayable version. If this call
	// returns a non-empty string, we're
	// on Windows 10 or greater and should
	// show that string.
	osdispver = win10DisplayableVersion();
	if (!osdispver.isEmpty()) {
		return osdispver;
	}

	OSVERSIONINFOEXW ovi;
	memset(&ovi, 0, sizeof(ovi));
	ovi.dwOSVersionInfoSize = sizeof(ovi);
	if (!GetVersionEx(reinterpret_cast<OSVERSIONINFOW *>(&ovi))) {
		return QString();
	}

	_SYSTEM_INFO si;
	GetNativeSystemInfo(&si);

	if (ovi.dwMajorVersion >= 6) {
		if (ovi.dwMajorVersion == 10) {
			if (ovi.wProductType == VER_NT_WORKSTATION) {
				osdispver = QLatin1String("Windows 10");
			} else {
				osdispver = QLatin1String("Windows 10 Server");
			}
		} else if (ovi.dwMajorVersion == 6) {
			if (ovi.dwMinorVersion == 0) {
				if (ovi.wProductType == VER_NT_WORKSTATION) {
					osdispver = QLatin1String("Windows Vista");
				} else {
					osdispver = QLatin1String("Windows Server 2008");
				}
			} else if (ovi.dwMinorVersion == 1) {
				if (ovi.wProductType == VER_NT_WORKSTATION) {
					osdispver = QLatin1String("Windows 7");
				} else {
					osdispver = QLatin1String("Windows Server 2008 R2");
				}
			} else if (ovi.dwMinorVersion == 2) {
				if (ovi.wProductType == VER_NT_WORKSTATION) {
					osdispver = QLatin1String("Windows 8");
				} else {
					osdispver = QLatin1String("Windows Server 2012");
				}
			} else if (ovi.dwMinorVersion == 3) {
				if (ovi.wProductType == VER_NT_WORKSTATION) {
					osdispver = QLatin1String("Windows 8.1");
				} else {
					osdispver = QLatin1String("Windows Server 2012 R2");
				}
			} else if (ovi.dwMinorVersion == 4) {
				if (ovi.wProductType == VER_NT_WORKSTATION) {
					osdispver = QLatin1String("Windows 10");
				} else {
					osdispver = QLatin1String("Windows 10 Server");
				}
			}
		}

		typedef BOOL (WINAPI *PGPI)(DWORD, DWORD, DWORD, DWORD, PDWORD);
		PGPI pGetProductInfo = (PGPI) GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "GetProductInfo");
		if (pGetProductInfo == NULL) {
			return QString();
		}

		DWORD dwType = 0;
		if (!pGetProductInfo(ovi.dwMajorVersion, ovi.dwMinorVersion, 0, 0, &dwType)) {
			return QString();
		}

		switch(dwType) {
		case PRODUCT_ULTIMATE:
			osdispver.append(QLatin1String(" Ultimate Edition"));
			break;
		case PRODUCT_PROFESSIONAL:
			osdispver.append(QLatin1String(" Professional"));
			break;
		case PRODUCT_HOME_PREMIUM:
			osdispver.append(QLatin1String(" Home Premium Edition"));
			break;
		case PRODUCT_HOME_BASIC:
			osdispver.append(QLatin1String(" Home Basic Edition"));
			break;
		case PRODUCT_ENTERPRISE:
			osdispver.append(QLatin1String(" Enterprise Edition"));
			break;
		case PRODUCT_BUSINESS:
			osdispver.append(QLatin1String(" Business Edition"));
			break;
		case PRODUCT_STARTER:
			osdispver.append(QLatin1String(" Starter Edition"));
			break;
		case PRODUCT_CLUSTER_SERVER:
			osdispver.append(QLatin1String(" Cluster Server Edition"));
			break;
		case PRODUCT_DATACENTER_SERVER:
			osdispver.append(QLatin1String(" Datacenter Edition"));
			break;
		case PRODUCT_DATACENTER_SERVER_CORE:
			osdispver.append(QLatin1String(" Datacenter Edition (core installation)"));
			break;
		case PRODUCT_ENTERPRISE_SERVER:
			osdispver.append(QLatin1String(" Enterprise Edition"));
			break;
		case PRODUCT_ENTERPRISE_SERVER_CORE:
			osdispver.append(QLatin1String(" Enterprise Edition (core installation)"));
			break;
		case PRODUCT_ENTERPRISE_SERVER_IA64:
			osdispver.append(QLatin1String(" Enterprise Edition for Itanium-based Systems"));
			break;
		case PRODUCT_SMALLBUSINESS_SERVER:
			osdispver.append(QLatin1String(" Small Business Server"));
			break;
		case PRODUCT_SMALLBUSINESS_SERVER_PREMIUM:
			osdispver.append(QLatin1String(" Small Business Server Premium Edition"));
			break;
		case PRODUCT_STANDARD_SERVER:
			osdispver.append(QLatin1String(" Standard Edition"));
			break;
		case PRODUCT_STANDARD_SERVER_CORE:
			osdispver.append(QLatin1String(" Standard Edition (core installation)"));
			break;
		case PRODUCT_WEB_SERVER:
			osdispver.append(QLatin1String(" Web Server Edition"));
			break;
		}
	} else if (ovi.dwMajorVersion == 5 && ovi.dwMinorVersion == 0) {
		osdispver = QLatin1String("Windows 2000");
		if (ovi.wProductType == VER_NT_WORKSTATION) {
			osdispver.append(QLatin1String(" Professional"));
		} else {
			if (ovi.wSuiteMask & VER_SUITE_DATACENTER) {
				osdispver.append(QLatin1String(" Datacenter Server"));
			} else if (ovi.wSuiteMask & VER_SUITE_ENTERPRISE) {
				osdispver.append(QLatin1String(" Advanced Server"));
			} else {
				osdispver.append(QLatin1String(" Server"));
			}
		}
	} else if (ovi.dwMajorVersion == 5 && ovi.dwMinorVersion == 1) {
		osdispver = QLatin1String("Windows XP");
		if (ovi.wSuiteMask & VER_SUITE_PERSONAL) {
			osdispver.append(QLatin1String(" Home Edition"));
		} else {
			osdispver.append(QLatin1String(" Professional"));
		}
	} else if (ovi.dwMajorVersion == 5 && ovi.dwMinorVersion == 2) {
		if (GetSystemMetrics(SM_SERVERR2)) {
			osdispver = QLatin1String("Windows Server 2003 R2");
		} else if (ovi.wSuiteMask & VER_SUITE_STORAGE_SERVER) {
			osdispver = QLatin1String("Windows Storage Server 2003");
		} else if (ovi.wSuiteMask & VER_SUITE_WH_SERVER) {
			osdispver = QLatin1String("Windows Home Server");
		} else if (ovi.wProductType == VER_NT_WORKSTATION && si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64) {
			osdispver = QLatin1String("Windows XP Professional x64 Edition");
		} else {
			osdispver = QLatin1String("Windows Server 2003");
		}

		if (ovi.wProductType != VER_NT_WORKSTATION) {
			if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64) {
				if (ovi.wSuiteMask & VER_SUITE_DATACENTER) {
					osdispver.append(QLatin1String(" Datacenter x64 Edition"));
				} else if (ovi.wSuiteMask & VER_SUITE_ENTERPRISE) {
					osdispver.append(QLatin1String(" Enterprise x64 Edition"));
				} else {
					osdispver.append(QLatin1String(" Standard x64 Edition"));
				}
			} else {
				if (ovi.wSuiteMask & VER_SUITE_COMPUTE_SERVER) {
					osdispver.append(QLatin1String(" Compute Cluster Edition"));
				} else if (ovi.wSuiteMask & VER_SUITE_DATACENTER) {
					osdispver.append(QLatin1String(" Datacenter Edition"));
				} else if (ovi.wSuiteMask & VER_SUITE_ENTERPRISE) {
					osdispver.append(QLatin1String(" Enterprise Edition"));
				} else if (ovi.wSuiteMask & VER_SUITE_BLADE) {
					osdispver.append(QLatin1String(" Web Edition"));
				} else {
					osdispver.append(QLatin1String(" Standard Edition"));
				}
			}
		}
	}

	// Service Packs (may be empty)
	static_assert(sizeof(TCHAR) == sizeof(wchar_t), "Expected Unicode TCHAR.");
	QString sp = QString::fromWCharArray(ovi.szCSDVersion);
	if (!sp.isEmpty()) {
		osdispver.append(QLatin1String(" "));
		osdispver.append(sp);
	}

	// Architecture
	if (ovi.dwMajorVersion >= 6) {
		if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64) {
			osdispver.append(QLatin1String(" x64"));
		} else if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL) {
			osdispver.append(QLatin1String(" x86"));
		}
	}

	QString osv;
	osv.sprintf(" - %lu.%lu.%lu", static_cast<unsigned long>(ovi.dwMajorVersion), static_cast<unsigned long>(ovi.dwMinorVersion), static_cast<unsigned long>(ovi.dwBuildNumber));
	osdispver.append(osv);

	return osdispver;
#else
	return OSInfo::getOSVersion();
#endif
}

void OSInfo::fillXml(QDomDocument &doc, QDomElement &root, const QString &os, const QString &osver, const QList<QHostAddress> &qlBind) {
	QDomElement tag;
	QDomText t;
	bool bIs64;
	bool bSSE2 = false;
	QString cpu_id, cpu_extid;

	tag=doc.createElement(QLatin1String("machash"));
	root.appendChild(tag);
	t=doc.createTextNode(OSInfo::getMacHash(qlBind));
	tag.appendChild(t);

	tag=doc.createElement(QLatin1String("version"));
	root.appendChild(tag);
	t=doc.createTextNode(QLatin1String(MUMTEXT(MUMBLE_VERSION_STRING)));
	tag.appendChild(t);

	tag=doc.createElement(QLatin1String("release"));
	root.appendChild(tag);
	t=doc.createTextNode(QLatin1String(MUMBLE_RELEASE));
	tag.appendChild(t);

	tag=doc.createElement(QLatin1String("os"));
	root.appendChild(tag);
	t=doc.createTextNode(os);
	tag.appendChild(t);

	tag=doc.createElement(QLatin1String("osver"));
	root.appendChild(tag);
	t=doc.createTextNode(osver);
	tag.appendChild(t);

	tag=doc.createElement(QLatin1String("qt"));
	root.appendChild(tag);
	t=doc.createTextNode(QString::fromLatin1(qVersion()));
	tag.appendChild(t);

#if defined(Q_OS_WIN)
	BOOL bIsWow64 = FALSE;
	IsWow64Process(GetCurrentProcess(), &bIsWow64);
	bIs64 = bIsWow64;
#else
	bIs64 = (QSysInfo::WordSize == 64);
#endif
	tag=doc.createElement(QLatin1String("is64bit"));
	root.appendChild(tag);
	t=doc.createTextNode(QString::number(bIs64 ? 1 : 0));
	tag.appendChild(t);

#if defined(Q_OS_WIN)
#define regstr(x) QString::fromLatin1(reinterpret_cast<const char *>(& x), 4)
	int chop;
	int cpuinfo[4];

	__cpuid(cpuinfo, 1);
	bSSE2 = (cpuinfo[3] & 0x04000000);

	__cpuid(cpuinfo, 0);

	cpu_id = regstr(cpuinfo[1]) + regstr(cpuinfo[3]) + regstr(cpuinfo[2]);

	for (unsigned int j=2; j<=4;++j) {
		__cpuid(cpuinfo, 0x80000000 + j);
		cpu_extid += regstr(cpuinfo[0]) + regstr(cpuinfo[1]) + regstr(cpuinfo[2]) + regstr(cpuinfo[3]);
	}

	cpu_id = cpu_id.trimmed();
	chop = cpu_id.indexOf(QLatin1Char('\0'));
	if (chop != -1)
		cpu_id.truncate(chop);

	cpu_extid = cpu_extid.trimmed();
	chop = cpu_extid.indexOf(QLatin1Char('\0'));
	if (chop != -1)
		cpu_extid.truncate(chop);
#endif

	tag=doc.createElement(QLatin1String("cpu_id"));
	root.appendChild(tag);
	t=doc.createTextNode(cpu_id);
	tag.appendChild(t);

	tag=doc.createElement(QLatin1String("cpu_extid"));
	root.appendChild(tag);
	t=doc.createTextNode(cpu_extid);
	tag.appendChild(t);

	tag=doc.createElement(QLatin1String("cpu_sse2"));
	root.appendChild(tag);
	t=doc.createTextNode(QString::number(bSSE2 ? 1 : 0));
	tag.appendChild(t);
}
