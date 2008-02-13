#ifndef _MUMBLE_PCH_H
#define _MUMBLE_PCH_H
#define QT_NO_DEBUG_STREAM
#define QT_ASCII_CAST_WARNINGS
#define QT_NO_CAST_TO_ASCII
#include <QtCore/QtCore>
#include <QtDBus/QtDBus>
#include <QtGui/QtGui>
#include <QtNetwork/QtNetwork>
#include <QtOpenGL/QtOpenGL>
#include <QtSql/QtSql>
#include <QtXml/QtXml>

#ifdef Q_OS_WIN
#define _WIN32_IE 0x0600
#include <windows.h>
#include <shellapi.h>
#else
#define CopyMemory(dst,ptr,len) memcpy(dst,ptr,len)
#define ZeroMemory(ptr,len) memset(ptr, 0, len)
#define __cdecl
typedef WId HWND;
#include <arpa/inet.h>
#endif

#include <openssl/aes.h>
#include <openssl/rand.h>
#include <math.h>
#include <speex/speex.h>
#include <speex/speex_jitter.h>
#include <speex/speex_preprocess.h>
#include <speex/speex_echo.h>
#include <speex/speex_callbacks.h>
#include <speex/speex_resampler.h>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#define FLOATING_POINT
extern "C" {
#include "config.h"
#include "arch.h"
#include "os_support.h"
#include "fftwrap.h"
#include "pseudofloat.h"
}

#endif
