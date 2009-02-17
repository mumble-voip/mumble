/* Copyright (C) 2005-2009, Thorvald Natvig <thorvald@natvig.com>

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

#include <signal.h>

#include "murmur_pch.h"
#include "UnixMurmur.h"
#include "Meta.h"

QMutex *LimitTest::qm;
QWaitCondition *LimitTest::qw;

LimitTest::LimitTest() : QThread() {
}

void LimitTest::run() {
	qm->lock();
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
			ql << qf;
		else
			break;
		if ((count & 511) == 0)
			qWarning("%d descriptors...", count);
	}
	foreach(QFile *qf, ql)
		delete qf;
	ql.clear();
	qCritical("Managed to open %d descriptors", count);

	qm = new QMutex();
	qw = new QWaitCondition();

	int fdcount = count / 2;

	QList<QThread *> qtl;
	for (count=0;count < fdcount; ++count) {
		QThread *t = new LimitTest();
		qtl << t;
		t->start();
		if (! t->isRunning())
			break;
		if ((count & 511) == 0)
			qWarning("%d threads...", count);
	}
	sleep(1);
	qm->lock();
	qw->wakeAll();
	qm->unlock();
	foreach(QThread *qt, qtl) {
		if (! qt->wait(1000)) {
			qWarning("Thread failed to terminate...");
			qt->terminate();
		}
	}
	qFatal("Managed to spawn %d threads", count);
}

extern QFile *qfLog;

int UnixMurmur::iHupFd[2];
int UnixMurmur::iTermFd[2];

UnixMurmur::UnixMurmur() {
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
		qWarning("Caught SIGHUP, but logfile not in use");
	} else {
		qWarning("Caught SIGHUP, will reopen %s", qPrintable(Meta::mp.qsLogfile));
		qfLog->close();
		qfLog->setFileName(Meta::mp.qsLogfile);
		if (! qfLog->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
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
