// Copyright 2005-2017 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MURMUR_UNIXMURMUR_H_
#define MUMBLE_MURMUR_UNIXMURMUR_H_

#include <QtCore/QObject>
#include <QtCore/QThread>

class QCoreApplication;
class QMutex;
class QSocketNotifier;
class QWaitCondition;

class LimitTest : public QThread {
		Q_OBJECT
		Q_DISABLE_COPY(LimitTest)
	protected:
		static QMutex *qm;
		static QWaitCondition *qw, *qstartw;
		LimitTest();
	public:
		int tid;
		void run();
		static void testLimits(QCoreApplication &);
};

class UnixMurmur : public QObject {
		Q_OBJECT
		Q_DISABLE_COPY(UnixMurmur)
	protected:
		bool bRoot;
		static int iHupFd[2], iTermFd[2];
		QSocketNotifier *qsnHup, *qsnTerm;

		static void hupSignalHandler(int);
		static void termSignalHandler(int);
	public slots:
		void handleSigHup();
		void handleSigTerm();
	public:
		bool logToSyslog;

		void setuid();
		void initialcap();
		void finalcap();
		const QString trySystemIniFiles(const QString& fname);

		UnixMurmur();
		~UnixMurmur();
};

#endif
