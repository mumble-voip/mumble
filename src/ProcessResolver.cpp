// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ProcessResolver.h"
#include <cstring>

ProcessResolver::ProcessResolver(bool resolveImmediately) : m_processMap() {
	if (resolveImmediately) {
		resolve();
	}
}

ProcessResolver::~ProcessResolver() {
	m_processMap.clear();
}

const ProcessResolver::ProcessMap &ProcessResolver::getProcessMap() const {
	return m_processMap;
}

void ProcessResolver::resolve() {
	// first clear the current lists
	m_processMap.clear();

	doResolve();
}

size_t ProcessResolver::amountOfProcesses() const {
	return m_processMap.size();
}


/// Helper function for adding an entry to the given process map
///
/// @param pid The process's PID
/// @param processName The name of the process
/// @param map The map to add the entry to
void addEntry(uint64_t pid, const char *processName, ProcessResolver::ProcessMap &map) {
	// 	In order to make sure the name pointer stays valid until we need it, we have ot copy it
	const size_t nameLength            = std::strlen(processName) + 1; // +1 for terminating NULL-byte
	std::unique_ptr< char[] > nameCopy = std::make_unique< char[] >(nameLength);

	std::strcpy(nameCopy.get(), processName);

	map.insert(std::make_pair(pid, std::move(nameCopy)));
}

// The implementation of the doResolve-function is platfrom-dependent
// The different implementations are heavily inspired by the ones given at
// https://github.com/davidebeatrici/list-processes
#ifdef Q_OS_WIN
// Implementation for Windows
#	ifndef UNICODE
#		define UNICODE
#	endif

#	ifndef WIN32_LEAN_AND_MEAN
#		define WIN32_LEAN_AND_MEAN
#	endif

#	include <windows.h>
#	include <tlhelp32.h>
#	include <limits>

bool utf16ToUtf8(const wchar_t *source, const int size, char *destination) {
	if (!WideCharToMultiByte(CP_UTF8, 0, source, -1, destination, size, NULL, NULL)) {
#	ifndef QT_NO_DEBUG
		qCritical("ProcessResolver: WideCharToMultiByte() failed with error %d\n", GetLastError());
#	endif
		return false;
	}

	return true;
}

void ProcessResolver::doResolve() {
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap == INVALID_HANDLE_VALUE) {
#	ifndef QT_NO_DEBUG
		qCritical("ProcessResolver: CreateToolhelp32Snapshot() failed with error %d", GetLastError());
#	endif
		return;
	}

	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(pe);

	BOOL ok = Process32First(hSnap, &pe);
	if (!ok) {
#	ifndef QT_NO_DEBUG
		qCritical("ProcessResolver: Process32First() failed with error %d\n", GetLastError());
#	endif
		return;
	}

	char name[MAX_PATH];

	while (ok) {
		if (utf16ToUtf8(pe.szExeFile, sizeof(name), name)) {
			addEntry(pe.th32ProcessID, name, m_processMap);
		}
#	ifndef QT_NO_DEBUG
		else {
			qWarning("ProcessResolver: utf16ToUtf8() failed, skipping entry...");
		}
#	endif

		ok = Process32Next(hSnap, &pe);
	}

	CloseHandle(hSnap);
}
#elif defined(Q_OS_LINUX)
// Implementation for Linux
#	include <QtCore/QFile>
#	include <QtCore/QDir>
#	include <QtCore/QStringList>
#	include <QtCore/QFileInfo>
#	include <QtCore/QByteArray>
#	include <QtCore/QString>


static constexpr const char *PROC_DIR = "/proc/";

void ProcessResolver::doResolve() {
	QDir procDir(QString::fromLatin1(PROC_DIR));
	QStringList entries = procDir.entryList();

	bool ok;

	foreach (const QString &currentEntry, entries) {
		uint64_t pid = static_cast< unsigned long long int >(currentEntry.toLongLong(&ok, 10));

		if (!ok) {
			continue;
		}

		QString exe = QFile::symLinkTarget(QString::fromLatin1(PROC_DIR) + currentEntry + QString::fromLatin1("/exe"));
		QFileInfo fi(exe);
		QString firstPart      = fi.baseName();
		QString completeSuffix = fi.completeSuffix();
		QString baseName;
		if (completeSuffix.isEmpty()) {
			baseName = firstPart;
		} else {
			baseName = firstPart + QLatin1String(".") + completeSuffix;
		}

		if (baseName == QLatin1String("wine-preloader") || baseName == QLatin1String("wine64-preloader")) {
			QFile f(QString::fromLatin1(PROC_DIR) + currentEntry + QString::fromLatin1("/cmdline"));
			if (f.open(QIODevice::ReadOnly)) {
				QByteArray cmdline = f.readAll();
				f.close();

				const auto nul = cmdline.indexOf('\0');
				if (nul != -1) {
					cmdline.truncate(nul);
				}

				QString exe = QString::fromUtf8(cmdline);
				if (exe.contains(QLatin1String("\\"))) {
					const auto lastBackslash = exe.lastIndexOf(QLatin1String("\\"));
					if (exe.length() > lastBackslash + 1) {
						baseName = exe.mid(lastBackslash + 1);
					}
				}
			}
		}

		if (!baseName.isEmpty()) {
			addEntry(pid, baseName.toUtf8().constData(), m_processMap);
		}
	}
}
#elif defined(Q_OS_MACOS)
// Implementation for MacOS
// Code taken from https://stackoverflow.com/questions/49506579/how-to-find-the-pid-of-any-process-in-mac-osx-c
#	include <libproc.h>

void ProcessResolver::doResolve() {
	pid_t pids[2048];
	unsigned int bytes  = static_cast< unsigned int >(proc_listpids(PROC_ALL_PIDS, 0, pids, sizeof(pids)));
	unsigned int n_proc = static_cast< unsigned int >(bytes / sizeof(pids[0]));
	for (unsigned int i = 0; i < n_proc; i++) {
		struct proc_bsdinfo proc;
		int st = proc_pidinfo(pids[i], PROC_PIDTBSDINFO, 0, &proc, PROC_PIDTBSDINFO_SIZE);
		if (st == PROC_PIDTBSDINFO_SIZE) {
			addEntry(static_cast< std::uint64_t >(pids[i]), proc.pbi_name, m_processMap);
		}
	}
}
#elif defined(Q_OS_FREEBSD)
// Implementation for FreeBSD
#	include <libutil.h>
#	include <sys/types.h>
#	include <sys/user.h>

void ProcessResolver::doResolve() {
	int n_procs;
	struct kinfo_proc *procs_info = kinfo_getallproc(&n_procs);
	if (!procs_info) {
#	ifndef QT_NO_DEBUG
		qCritical("ProcessResolver: kinfo_getallproc() failed\n");
#	endif
		return;
	}

	for (int i = 0; i < n_procs; ++i) {
		addEntry(static_cast< uint64_t >(procs_info[i].ki_pid), procs_info[i].ki_comm, m_processMap);
	}

	free(procs_info);
}
#elif defined(Q_OS_BSD4)
// Implementation of generic BSD other than FreeBSD
#	include <limits.h>

#	include <fcntl.h>
#	include <kvm.h>
#	include <paths.h>
#	include <sys/sysctl.h>
#	include <sys/user.h>

bool kvm_cleanup(kvm_t *kd) {
	if (kvm_close(kd) == -1) {
#	ifndef QT_NO_DEBUG
		qCritical("ProcessResolver: kvm_close() failed with error %d\n", errno);
#	endif
		return false;
	}

	return true;
}

void ProcessResolver::doResolve() {
	char error[_POSIX2_LINE_MAX];
#	ifdef KVM_NO_FILES
	kvm_t *kd = kvm_openfiles(NULL, NULL, NULL, KVM_NO_FILES, error);
#	else
	kvm_t *kd                     = kvm_openfiles(NULL, _PATH_DEVNULL, NULL, O_RDONLY, error);
#	endif

	if (!kd) {
#	ifndef QT_NO_DEBUG
		qCritical("ProcessResolver: kvm_open2() failed with error: %s\n", error);
#	endif
		return;
	}

	int n_procs;
#	if defined(__NetBSD__)
	struct kinfo_proc *procs_info = kvm_getprocs(kd, KERN_PROC_ALL, 0, &n_procs);
#	elif defined(__OpenBSD__)
	struct kinfo_proc *procs_info = kvm_getprocs(kd, KERN_PROC_ALL, 0, sizeof(*procs_info), &n_procs);
#	else
	struct kinfo_proc *procs_info = kvm_getprocs(kd, KERN_PROC_PROC, 0, &n_procs);
#	endif
	if (!procs_info) {
#	ifndef QT_NO_DEBUG
		qCritical("ProcessResolver: kvm_getprocs() failed\n");
#	endif
		kvm_cleanup(kd);

		return;
	}

	for (int i = 0; i < n_procs; ++i) {
#	if defined(__NetBSD__) || defined(__OpenBSD__)
		addEntry(procs_info[i].p_pid, procs_info[i].p_comm, m_processMap);
#	else
		addEntry(procs_info[i].ki_pid, procs_info[i].ki_comm, m_processMap);
#	endif
	}

	kvm_cleanup(kd);
}
#else
#	error "No implementation of ProcessResolver::resolve() available for this operating system"
#endif
