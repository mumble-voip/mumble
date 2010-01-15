/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>

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
#include "UnixMurmur.h"
#include "Meta.h"

QMutex *LimitTest::qm;
QWaitCondition *LimitTest::qw;
QWaitCondition *LimitTest::qstartw;

LimitTest::LimitTest() : QThread() {
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
		qWarning("Not running with glib. While you may be able to open more descriptors, sockets above %d will not work", FD_SETSIZE);
	qWarning("Running descriptor test.");
	int count;
	QList<QFile *> ql;
	for (count=0;count < 524288; ++count) {
		QFile *qf = new QFile(a.applicationFilePath());
		if (qf->open(QIODevice::ReadOnly))
			ql.prepend(qf);
		else
			break;
		if ((count & 1023) == 0)
			qWarning("%d descriptors...", count);
	}
	foreach(QFile *qf, ql)
		delete qf;
	ql.clear();
	qCritical("Managed to open %d descriptors", count);

	qm = new QMutex();
	qw = new QWaitCondition();
	qstartw = new QWaitCondition();

	int fdcount = count / 8;
	if (sizeof(void *) < 8)
		if (fdcount > 1024)
			fdcount = 1024;

	QList<LimitTest *> qtl;
	for (count=0;count < fdcount; ++count) {
		LimitTest *t = new LimitTest();
		t->tid = count;
		qtl << t;
		qm->lock();
		t->start();
		qstartw->wait(qm);
		qm->unlock();
		if (! t->isRunning())
			break;
		if ((count & 511) == 0)
			qWarning("%d threads...", count);
	}
	qm->lock();
	qw->wakeAll();
	qm->unlock();

	foreach(LimitTest *qt, qtl) {
		if (! qt->wait(1000)) {
			qWarning("Thread %d failed to terminate...", qt->tid);
			qt->terminate();
		}
	}
	qFatal("Managed to spawn %d threads", count);
}

extern QFile *qfLog;

int UnixMurmur::iHupFd[2];
int UnixMurmur::iTermFd[2];

UnixMurmur::UnixMurmur() {
	bRoot = true;

	if (geteuid() != 0 && getuid() != 0) {
		bRoot = false;
	}
	if (::socketpair(AF_UNIX, SOCK_STREAM, 0, iHupFd))
		qFatal("Couldn't create HUP socketpair");

	if (::socketpair(AF_UNIX, SOCK_STREAM, 0, iTermFd))
		qFatal("Couldn't create TERM socketpair");

	qsnHup = new QSocketNotifier(iHupFd[1], QSocketNotifier::Read, this);
	qsnTerm = new QSocketNotifier(iTermFd[1], QSocketNotifier::Read, this);

	connect(qsnHup, SIGNAL(activated(int)), this, SLOT(handleSigHup()));
	connect(qsnTerm, SIGNAL(activated(int)), this, SLOT(handleSigTerm()));

	struct sigaction hup, term;

	hup.sa_handler = hupSignalHandler;
	sigemptyset(&hup.sa_mask);
	hup.sa_flags = SA_RESTART;

	if (sigaction(SIGHUP, &hup, NULL))
		qFatal("Failed to install SIGHUP handler");

	term.sa_handler = termSignalHandler;
	sigemptyset(&term.sa_mask);
	term.sa_flags = SA_RESTART;

	if (sigaction(SIGTERM, &term, NULL))
		qFatal("Failed to install SIGTERM handler");

	umask(S_IRWXO);
}

UnixMurmur::~UnixMurmur() {
	delete qsnHup;
	delete qsnTerm;

	qsnHup = NULL;
	qsnTerm = NULL;

	close(iHupFd[0]);
	close(iHupFd[1]);
	close(iTermFd[0]);
	close(iTermFd[1]);
}

void UnixMurmur::hupSignalHandler(int) {
	char a = 1;
	::write(iHupFd[0], &a, sizeof(a));
}

void UnixMurmur::termSignalHandler(int) {
	char a = 1;
	::write(iTermFd[0], &a, sizeof(a));
}


// Keep these two synchronized with matching actions in DBus.cpp

void UnixMurmur::handleSigHup() {
	qsnHup->setEnabled(false);
	char tmp;
	::read(iHupFd[1], &tmp, sizeof(tmp));

	if (! qfLog || ! qfLog->isOpen()) {
		if (qfLog) {
			qWarning("Caught SIGHUP, but logfile not in use -- interpreting as hint to quit");
			QCoreApplication::instance()->quit();
		} else {
			qWarning("Caught SIGHUP, but logfile not in use");
		}
	} else {
		qWarning("Caught SIGHUP, will reopen %s", qPrintable(Meta::mp.qsLogfile));
		qfLog->close();
		qfLog->setFileName(Meta::mp.qsLogfile);
		bool result = qfLog->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
		if (! result) {
			delete qfLog;
			qfLog = NULL;
		} else {
			qfLog->setTextModeEnabled(true);
			qWarning("Log rotated successfully");
		}
	}
	qsnHup->setEnabled(true);
}

void UnixMurmur::handleSigTerm() {
	qsnTerm->setEnabled(false);
	char tmp;
	::read(iTermFd[1], &tmp, sizeof(tmp));

	qCritical("Caught SIGTERM, exiting");

	QCoreApplication::instance()->quit();

	qsnTerm->setEnabled(true);
}

void UnixMurmur::setuid() {
	if (Meta::mp.uiUid != 0) {
#ifdef Q_OS_DARWIN
		qCritical("WARNING: You are launching murmurd as root on Mac OS X or Darwin. Murmur does not need "
		          "special privileges to set itself up on these systems, so this behavior is highly discouraged.");

		if (::setgid(Meta::mp.uiGid) != 0)
			qFatal("Failed to switch to gid %d", Meta::mp.uiGid);
		if (::setuid(Meta::mp.uiUid) != 0)
			qFatal("Failed to switch to uid %d", Meta::mp.uiUid);

		uid_t uid = getuid(), euid = geteuid();
		gid_t gid = getgid(), egid = getegid();
		if (uid == Meta::mp.uiUid && euid == Meta::mp.uiUid
		        && gid == Meta::mp.uiGid && egid == Meta::mp.uiGid) {
			qCritical("Successfully switched to uid %d", Meta::mp.uiUid);
		} else
			qFatal("Couldn't switch uid/gid.");
#else
		if (setregid(Meta::mp.uiGid, Meta::mp.uiGid) != 0)
			qCritical("Failed to switch to gid %d", Meta::mp.uiGid);
		if (setresuid(Meta::mp.uiUid, Meta::mp.uiUid, 0) != 0) {
			qFatal("Failed to become uid %d", Meta::mp.uiUid);
		} else {
			qCritical("Successfully switched to uid %d", Meta::mp.uiUid);
			initialcap();
		}
#endif
	} else if (bRoot) {
		qCritical("WARNING: You are running murmurd as root, without setting a uname in the ini file. This might be a security risk.");
	}
}

void UnixMurmur::initialcap() {
#ifdef Q_OS_LINUX
	cap_value_t caps[] = {CAP_NET_ADMIN, CAP_SETUID, CAP_SETGID, CAP_SYS_RESOURCE, CAP_DAC_OVERRIDE };

	if (! bRoot)
		return;

	int ncap = sizeof(caps)/sizeof(cap_value_t);

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
	cap_value_t caps[] = {CAP_NET_ADMIN, CAP_SYS_RESOURCE};
	struct rlimit r;

	if (! bRoot)
		return;

	if (getrlimit(RLIMIT_RTPRIO, &r) != 0) {
		qCritical("Failed to get priority limits.");
	} else {
		qWarning("Resource limits were %ld %ld", r.rlim_cur, r.rlim_max);
		r.rlim_cur = r.rlim_max = 1;
		if (setrlimit(RLIMIT_RTPRIO, &r) != 0) {
			qCritical("Failed to set priority limits.");
		}
	}

	int ncap = sizeof(caps)/sizeof(cap_value_t);

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
