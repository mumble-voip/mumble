#ifndef _MURMUR_PCH_H
#define _MURMUR_PCH_H
#include <QtCore>
#include <QtNetwork>
#include <QtSql>
#include <QtXml>
#include <QtDBus>
#ifdef Q_OS_WIN
#include <QtGui>
#endif
#include <math.h>

#include <openssl/pem.h>
#include <openssl/conf.h>
#include <openssl/x509v3.h>
#ifndef OPENSSL_NO_ENGINE
#include <openssl/engine.h>
#endif

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
#define _WIN32_IE 0x0600
#include <windows.h>
#include <shellapi.h>
#endif

#endif
