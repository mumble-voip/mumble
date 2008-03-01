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

#define _USE_MATH_DEFINES
#include <math.h>

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
#define ALLOCA(x) _alloca(x)
#define STACKVAR(type, varname, count) type *varname=reinterpret_cast<type *>(ALLOCA(sizeof(type) * (count)))
#define lround(x) static_cast<long int>((x) + (((x) >= 0.0) ? 0.5 : -0.5))
#define sqrt(x) ::sqrt(static_cast<double>(x))
#define sqrtf(x) ::sqrtf(static_cast<float>(x))
#else
#include <alloca.h>
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
