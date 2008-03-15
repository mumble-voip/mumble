#ifndef _MUMBLE_PCH_H
#define _MUMBLE_PCH_H
#define QT_NO_DEBUG_STREAM
#define QT_ASCII_CAST_WARNINGS
#define QT_NO_CAST_TO_ASCII

#define _MSC_EXTENSIONS

#ifndef _MSC_EXTENSIONS
#error Snufs
#endif

#include <QtCore/QtCore>
#include <QtGui/QtGui>
#include <QtNetwork/QtNetwork>
#include <QtOpenGL/QtOpenGL>
#include <QtSql/QtSql>
#include <QtXml/QtXml>
#ifndef Q_OS_MAC
 #include <QtDBus/QtDBus>
#endif


#include <openssl/aes.h>
#include <openssl/rand.h>

#include <speex/speex.h>
#include <speex/speex_jitter.h>
#include <speex/speex_preprocess.h>
#include <speex/speex_echo.h>
#include <speex/speex_callbacks.h>
#include <speex/speex_resampler.h>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/typeof/typeof.hpp>

#ifdef Q_OS_WIN
#include <windows.h>
#include <shellapi.h>
#include <mathimf.h>
#define M_PI 3.14159265358979323846
#define ALLOCA(x) _alloca(x)
#define STACKVAR(type, varname, count) type *varname=reinterpret_cast<type *>(ALLOCA(sizeof(type) * (count)))
#else
#include <alloca.h>
#include <math.h>
#define ALLOCA(x) alloca(x)
#define STACKVAR(type, varname, count) type varname[count]
#define CopyMemory(dst,ptr,len) memcpy(dst,ptr,len)
#define ZeroMemory(ptr,len) memset(ptr, 0, len)
#define __cdecl
typedef WId HWND;
#include <arpa/inet.h>
#endif

#if defined(__MMX__) || defined(Q_OS_WIN)
#include <mmintrin.h>
#endif

#endif
