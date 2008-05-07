#ifndef _MURMUR_PCH_H
#define _MURMUR_PCH_H

#if defined(__INTEL_COMPILER)
#include <mathimf.h>
#endif

#include <QtCore/QtCore>
#include <QtNetwork/QtNetwork>
#include <QtSql/QtSql>
#include <QtXml/QtXml>
#include <QtDBus/QtDBus>
#ifdef Q_OS_WIN
#include <QtGui/QtGui>
#endif

#ifdef Q_OS_WIN
#include <winsock2.h>
#include <windows.h>
#include <shellapi.h>
#endif


#include <openssl/aes.h>
#include <openssl/rand.h>
#include <openssl/pem.h>
#include <openssl/conf.h>
#include <openssl/x509v3.h>

#ifdef Q_OS_UNIX
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#ifdef __FreeBSD__
#include <netinet/in_systm.h>
#endif
#include <netinet/in.h>
#include <netinet/ip.h>
#include <errno.h>
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#endif

#ifdef Q_OS_WIN
#ifndef Q_CC_INTEL
#include <math.h>
#define lroundf(x) ( static_cast<int>( (x) + ((x) >= 0 ? 0.5 : -0.5) ) )
#endif
#define M_PI 3.14159265358979323846
#define ALLOCA(x) _alloca(x)
#define STACKVAR(type, varname, count) type *varname=reinterpret_cast<type *>(ALLOCA(sizeof(type) * (count)))
#define snprintf ::_snprintf
#else
#include <math.h>
#define STACKVAR(type, varname, count) type varname[count]
#endif

#endif
