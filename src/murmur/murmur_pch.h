#ifndef Q_MOC_RUN
#ifndef _MURMUR_PCH_H
#define _MURMUR_PCH_H

#define _WINSOCKAPI_

#define _USE_MATH_DEFINES
#if defined(__INTEL_COMPILER)
#include <mathimf.h>
#endif

#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#include <CoreServices/CoreServices.h>
#undef check
#undef TYPE_BOOL
#endif

#include <QtCore/QtCore>
#include <QtNetwork/QtNetwork>
#include <QtSql/QtSql>
#include <QtXml/QtXml>
#ifdef USE_DBUS
#include <QtDBus/QtDBus>
#endif

#ifdef Q_OS_WIN
#include <QtGui/QtGui>
#include <winsock2.h>
#include <qos2.h>
#include <windows.h>
#include <shellapi.h>
#include <delayimp.h>
extern "C" {
	void __cpuid(int a[4], int b);
};
#endif

#include <openssl/aes.h>
#include <openssl/rand.h>
#include <openssl/pem.h>
#include <openssl/conf.h>
#include <openssl/x509v3.h>
/* OpenSSL defines set_key. This breaks our protobuf-generated setters. */
#undef set_key

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
#ifdef Q_OS_LINUX
#include <sys/capability.h>
#include <sys/prctl.h>
#endif
#include <pwd.h>
#ifdef __FreeBSD__
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

#if !defined (Q_CC_INTEL) && !defined (Q_OS_WIN)
#include <math.h>
#endif
#if defined (Q_OS_WIN) && (defined (Q_CC_INTEL) || defined (Q_CC_MSVC))
#define lroundf(x) ( static_cast<int>( (x) + ((x) >= 0 ? 0.5 : -0.5) ) )
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

#endif
#endif
