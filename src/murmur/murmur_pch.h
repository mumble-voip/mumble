#ifndef _MURMUR_PCH_H
#define _MURMUR_PCH_H

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

#define _USE_MATH_DEFINES
#include <math.h>

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
#include <netinet/in.h>
#include <netinet/ip.h>
#include <errno.h>
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#endif

#ifdef Q_OS_WIN
#define ALLOCA(x) _alloca(x)
#define STACKVAR(type, varname, count) type *varname=reinterpret_cast<type *>(ALLOCA(sizeof(type) * (count)))
#define lround(x) static_cast<long int>((x) + (((x) >= 0.0) ? 0.5 : -0.5))
#define sqrt(x) ::sqrt(static_cast<double>(x))
#define sqrtf(x) ::sqrtf(static_cast<float>(x))
#define snprintf ::_snprintf
#else
#include <alloca.h>
#define ALLOCA(x) alloca(x)
#define STACKVAR(type, varname, count) type varname[count]
#endif

#endif
