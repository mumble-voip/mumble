// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "UnixMurmur.h"

#include "EnvUtils.h"
#include "Meta.h"

#include <QtCore/QAbstractEventDispatcher>
#include <QtCore/QCoreApplication>
#include <QtCore/QMutex>
#include <QtCore/QSocketNotifier>
#include <QtCore/QWaitCondition>

#include <grp.h>
#include <signal.h>

#ifdef Q_OS_LINUX
#	include <sys/capability.h>
#	include <sys/prctl.h>
#	include <sys/resource.h>
#endif

#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>

#include <limits>

QMutex *LimitTest::qm;
QWaitCondition *LimitTest::qw;
QWaitCondition *LimitTest::qstartw;

LimitTest::LimitTest() : QThread(), tid(-1) {
}

void LimitTest::run() {
	qm->lock();
	qstartw->wakeAll();

	qw->wait(qm);
	qm->unlock();
}

void LimitTest::testLimits(QCoreApplication &a) {
	QAbstractEventDispatcher *ed = QAbstractEventDispatcher::instance();
	if (QLatin1String(ed->metaObject()->className()) != QLatin1String("QEventDispatcherGlib"))
		qWarning(
			"Not running with glib. While you may be able to open more descriptors, sockets above %d will not work",
			FD_SETSIZE);
	qWarning("Running descriptor test.");
	int count;
	QList< QFile * > ql;
	for (count = 0; count < 524288; ++count) {
		QFile *qf = new QFile(a.applicationFilePath());
		if (qf->open(QIODevice::ReadOnly))
			ql.prepend(qf);
		else
			break;
		if ((count & 1023) == 0)
			qWarning("%d descriptors...", count);
	}
	foreach (QFile *qf, ql)
		delete qf;
	ql.clear();
	qCritical("Managed to open %d descriptors", count);

	qm      = new QMutex();
	qw      = new QWaitCondition();
	qstartw = new QWaitCondition();

	int fdcount = count / 8;
	if (sizeof(void *) < 8)
		if (fdcount > 1024)
			fdcount = 1024;

	QList< LimitTest * > qtl;
	for (count = 0; count < fdcount; ++count) {
		LimitTest *t = new LimitTest();
		t->tid       = count;
		qtl << t;
		qm->lock();
		t->start();
		qstartw->wait(qm);
		qm->unlock();
		if (!t->isRunning())
			break;
		if ((count & 511) == 0)
			qWarning("%d threads...", count);
	}
	qm->lock();
	qw->wakeAll();
	qm->unlock();

	foreach (LimitTest *qt, qtl) {
		if (!qt->wait(1000)) {
			qWarning("Thread %d failed to terminate...", qt->tid);
			qt->terminate();
		}
	}
	qFatal("Managed to spawn %d threads", count);
}

extern QFile *qfLog;

int UnixMurmur::iHupFd[2];
int UnixMurmur::iTermFd[2];
int UnixMurmur::iUsr1Fd[2];

UnixMurmur::UnixMurmur() {
	bRoot       = true;
	logToSyslog = false;

	if (geteuid() != 0 && getuid() != 0) {
		bRoot = false;
	}
	if (::socketpair(AF_UNIX, SOCK_STREAM, 0, iHupFd))
		qFatal("Couldn't create HUP socketpair");

	if (::socketpair(AF_UNIX, SOCK_STREAM, 0, iTermFd))
		qFatal("Couldn't create TERM socketpair");

	if (::socketpair(AF_UNIX, SOCK_STREAM, 0, iUsr1Fd))
		qFatal("Couldn't create USR1 socketpair");

	qsnHup  = new QSocketNotifier(iHupFd[1], QSocketNotifier::Read, this);
	qsnTerm = new QSocketNotifier(iTermFd[1], QSocketNotifier::Read, this);
	qsnUsr1 = new QSocketNotifier(iUsr1Fd[1], QSocketNotifier::Read, this);

	connect(qsnHup, SIGNAL(activated(int)), this, SLOT(handleSigHup()));
	connect(qsnTerm, SIGNAL(activated(int)), this, SLOT(handleSigTerm()));
	connect(qsnUsr1, SIGNAL(activated(int)), this, SLOT(handleSigUsr1()));

	struct sigaction hup, term, usr1;

	hup.sa_handler = hupSignalHandler;
	sigemptyset(&hup.sa_mask);
	hup.sa_flags = SA_RESTART;

	if (sigaction(SIGHUP, &hup, nullptr))
		qFatal("Failed to install SIGHUP handler");

	term.sa_handler = termSignalHandler;
	sigemptyset(&term.sa_mask);
	term.sa_flags = SA_RESTART;

	if (sigaction(SIGTERM, &term, nullptr))
		qFatal("Failed to install SIGTERM handler");

	usr1.sa_handler = usr1SignalHandler;
	sigemptyset(&usr1.sa_mask);
	usr1.sa_flags = SA_RESTART;

	if (sigaction(SIGUSR1, &usr1, nullptr))
		qFatal("Failed to install SIGUSR1 handler");

	umask(S_IRWXO);
}

UnixMurmur::~UnixMurmur() {
	delete qsnHup;
	delete qsnTerm;
	delete qsnUsr1;

	qsnHup  = nullptr;
	qsnTerm = nullptr;
	qsnUsr1 = nullptr;

	close(iHupFd[0]);
	close(iHupFd[1]);
	close(iTermFd[0]);
	close(iTermFd[1]);
	close(iUsr1Fd[0]);
	close(iUsr1Fd[1]);
}

void UnixMurmur::hupSignalHandler(int) {
	char a      = 1;
	ssize_t len = ::write(iHupFd[0], &a, sizeof(a));
	Q_UNUSED(len);
}

void UnixMurmur::termSignalHandler(int) {
	char a      = 1;
	ssize_t len = ::write(iTermFd[0], &a, sizeof(a));
	Q_UNUSED(len);
}

void UnixMurmur::usr1SignalHandler(int) {
	char a      = 1;
	ssize_t len = ::write(iUsr1Fd[0], &a, sizeof(a));
	Q_UNUSED(len);
}


void UnixMurmur::handleSigHup() {
	qsnHup->setEnabled(false);
	char tmp;
	ssize_t len = ::read(iHupFd[1], &tmp, sizeof(tmp));
	Q_UNUSED(len);

	if (logToSyslog) {
		qWarning("Caught SIGHUP, but logging to syslog");
	} else if (!qfLog) {
		qWarning("Caught SIGHUP, but logfile not in use");
	} else if (!qfLog->isOpen()) {
		qWarning("Caught SIGHUP, but logfile not in use -- interpreting as hint to quit");
		QCoreApplication::instance()->quit();
	} else {
		qWarning("Caught SIGHUP, will reopen %s", qPrintable(Meta::mp->qsLogfile));

		QFile *newlog = new QFile(Meta::mp->qsLogfile);
		bool result   = newlog->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
		if (!result) {
			delete newlog;
			qCritical("Failed to reopen logfile for writing, keeping old log");
		} else {
			QFile *oldlog = qfLog;

			newlog->setTextModeEnabled(true);
			qfLog = newlog;
			oldlog->close();
			delete oldlog;
			qWarning("Log rotated successfully");
		}
	}
	qsnHup->setEnabled(true);
}

void UnixMurmur::handleSigTerm() {
	qsnTerm->setEnabled(false);
	char tmp;
	ssize_t len = ::read(iTermFd[1], &tmp, sizeof(tmp));
	Q_UNUSED(len);

	qCritical("Caught SIGTERM, exiting");

	QCoreApplication::instance()->quit();

	qsnTerm->setEnabled(true);
}

void UnixMurmur::handleSigUsr1() {
	qsnUsr1->setEnabled(false);
	char tmp;
	ssize_t len = ::read(iUsr1Fd[1], &tmp, sizeof(tmp));
	Q_UNUSED(len);

	if (meta) {
		qWarning("UnixMurmur: Trying to reload SSL settings...");
		bool ok = meta->reloadSSLSettings();
		if (ok) {
			qWarning("UnixMurmur: Done reloading SSL settings.");
		} else {
			qWarning("UnixMurmur: Failed to reload SSL settings. Server state is intact and fully operational. No "
					 "configuration changes were made.");
		}
	}

	qsnUsr1->setEnabled(true);
}

void UnixMurmur::setuid() {
	if (Meta::mp->uiUid != 0) {
#ifdef Q_OS_DARWIN
		qCritical("WARNING: You are launching murmurd as root on Mac OS X or Darwin. Murmur does not need "
				  "special privileges to set itself up on these systems, so this behavior is highly discouraged.");

		if (::setgid(Meta::mp->uiGid) != 0)
			qFatal("Failed to switch to gid %d", Meta::mp->uiGid);
		if (::setuid(Meta::mp->uiUid) != 0)
			qFatal("Failed to switch to uid %d", Meta::mp->uiUid);

		uid_t uid = getuid(), euid = geteuid();
		gid_t gid = getgid(), egid = getegid();
		if (uid == Meta::mp->uiUid && euid == Meta::mp->uiUid && gid == Meta::mp->uiGid && egid == Meta::mp->uiGid) {
			qCritical("Successfully switched to uid %d", Meta::mp->uiUid);
		} else
			qFatal("Couldn't switch uid/gid.");
#else
		if (::initgroups(qPrintable(Meta::mp->qsName), Meta::mp->uiGid) != 0)
			qCritical("Can't initialize supplementary groups");
		if (::setgid(Meta::mp->uiGid) != 0)
			qCritical("Failed to switch to gid %d", Meta::mp->uiGid);
		if (::setuid(Meta::mp->uiUid) != 0) {
			qFatal("Failed to become uid %d", Meta::mp->uiUid);
		} else {
			qCritical("Successfully switched to uid %d", Meta::mp->uiUid);
			initialcap();
		}
		if (!Meta::mp->qsHome.isEmpty()) {
			// QDir::homePath is broken. It only looks at $HOME
			// instead of getpwuid() so we have to set our home
			// ourselves
			EnvUtils::setenv(QLatin1String("HOME"), Meta::mp->qsHome);
		}
#endif
	} else if (bRoot) {
		qCritical("WARNING: You are running murmurd as root, without setting a uname in the ini file. This might be a "
				  "security risk.");
	}
}

void UnixMurmur::initialcap() {
#ifdef Q_OS_LINUX
	cap_value_t caps[] = { CAP_NET_ADMIN, CAP_SETUID, CAP_SETGID, CAP_CHOWN, CAP_SYS_RESOURCE, CAP_DAC_OVERRIDE };

	if (!bRoot)
		return;

	int ncap = sizeof(caps) / sizeof(cap_value_t);

	if (geteuid() != 0)
		ncap--;

	cap_t c = cap_init();
	cap_clear(c);
	cap_set_flag(c, CAP_EFFECTIVE, ncap, caps, CAP_SET);
	cap_set_flag(c, CAP_INHERITABLE, ncap, caps, CAP_SET);
	cap_set_flag(c, CAP_PERMITTED, ncap, caps, CAP_SET);
	if (cap_set_proc(c) != 0) {
		qCritical("Failed to set initial capabilities");
	} else {
		prctl(PR_SET_KEEPCAPS, 1, 0, 0, 0);
	}
	cap_free(c);
#endif
}

void UnixMurmur::finalcap() {
#ifdef Q_OS_LINUX
	cap_value_t caps[] = { CAP_SYS_RESOURCE };
	struct rlimit r;

	if (!bRoot)
		return;

	if (getrlimit(RLIMIT_RTPRIO, &r) != 0) {
		qCritical("Failed to get priority limits.");
	} else {
		using ulong_t = unsigned long long int;
		static_assert(std::numeric_limits< ulong_t >::max() >= std::numeric_limits< rlim_t >::max(),
					  "rlim_t is unexpectedly large");
		ulong_t current = r.rlim_cur;
		ulong_t max     = r.rlim_max;
		qWarning("Resource limits were %llu %llu", current, max);

		r.rlim_cur = r.rlim_max = 1;
		if (setrlimit(RLIMIT_RTPRIO, &r) != 0) {
			qCritical("Failed to set priority limits.");
		}
	}

	int ncap = sizeof(caps) / sizeof(cap_value_t);

	cap_t c = cap_init();
	cap_clear(c);
	cap_set_flag(c, CAP_EFFECTIVE, ncap, caps, CAP_SET);
	cap_set_flag(c, CAP_PERMITTED, ncap, caps, CAP_SET);
	if (cap_set_proc(c) != 0) {
		qCritical("Failed to set final capabilities");
	} else {
		qWarning("Successfully dropped capabilities");
	}
	cap_free(c);
#endif
}

const QString UnixMurmur::trySystemIniFiles(const QString &fname) {
	QString file = fname;
	if (!file.isEmpty())
		return file;
#if defined(Q_OS_LINUX)
	if (!bRoot)
		return file;

	QStringList inipaths;

	inipaths << QLatin1String("/usr/local/etc/mumble-server.ini");
	inipaths << QLatin1String("/usr/local/etc/murmur.ini");
	inipaths << QLatin1String("/etc/mumble-server.ini");
	inipaths << QLatin1String("/etc/murmur.ini");

	foreach (const QString &f, inipaths) {
		QFileInfo fi(f);
		if (fi.exists() && fi.isReadable()) {
			file = fi.absoluteFilePath();
			break;
		}
	}
#endif
	return file;
}
