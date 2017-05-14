// Copyright 2005-2017 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef Q_MOC_RUN
#ifndef MUMBLE_MURMUR_MURMUR_PCH_H_
#define MUMBLE_MURMUR_MURMUR_PCH_H_

#define WIN32_LEAN_AND_MEAN

#define _USE_MATH_DEFINES

#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#include <CoreServices/CoreServices.h>
#undef check
#undef TYPE_BOOL
#endif

#ifdef __MINGW32__
#define _WIN32_WINNT 0x0600
#include <ws2tcpip.h>
#include <mswsock.h>
#endif

#include <QtCore/QtCore>
#include <QtNetwork/QtNetwork>
#include <QtSql/QtSql>
#include <QtXml/QtXml>
#ifdef USE_DBUS
#include <QtDBus/QtDBus>
#endif

#include "QAtomicIntCompat.h"

#ifdef Q_OS_WIN
#include <QtGui/QtGui>
#if QT_VERSION >= 0x050000
# include "Qt4Compat.h"
# include <QtWidgets/QtWidgets>
#endif

#include "../qos2_mingw.h"

#include <winsock2.h>
#include <qos2.h>
#include <windows.h>
#include <shellapi.h>
#include <delayimp.h>
extern "C" {
	void __cpuid(int a[4], int b);
};
#endif

#ifdef Q_OS_UNIX
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <syslog.h>
#ifdef Q_OS_LINUX
#include <linux/types.h> // needed to work around evil magic stuff in capability.h
#include <sys/capability.h>
#include <sys/prctl.h>
#endif
#include <pwd.h>
#include <grp.h>
#if defined __FreeBSD__ || defined __OpenBSD__
#include <netinet/in_systm.h>
#endif
#include <netinet/in.h>
#include <netinet/ip.h>
#include <errno.h>
#include <signal.h>
#include <poll.h>
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#ifdef Q_OS_DARWIN
#include <poll.h>
#endif
#endif

#include <math.h>

#if defined (Q_OS_WIN)
#define snprintf ::_snprintf
#define STACKVAR(type, varname, count) type *varname=reinterpret_cast<type *>(_alloca(sizeof(type) * (count)))
#else
#define STACKVAR(type, varname, count) type varname[count]
#endif

#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#ifdef USE_BONJOUR
#include <dns_sd.h>
#endif

#include <openssl/opensslv.h>
#include <openssl/bn.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <openssl/pem.h>
#include <openssl/conf.h>
#include <openssl/x509v3.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
/* OpenSSL defines set_key. This breaks our protobuf-generated setters. */
#undef set_key

#endif
#endif
