#ifndef _MURMUR_PCH_H
#define _MURMUR_PCH_H
#ifdef WIN32
#define _WIN32_IE 0x0600
#include <windows.h>
#include <shellapi.h>
#endif
#include <QtCore>
#include <QtNetwork>
#include <QtSql>
#include <QtXml>
#ifdef Q_OS_UNIX
#include <QtDBus>
#endif
#ifdef Q_OS_WIN
#include <QtGui>
#endif
#include <math.h>
#endif
