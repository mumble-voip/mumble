#ifndef _MUMBLE_PCH_H
#define _MUMBLE_PCH_H
#define QT_NO_DEBUG_STREAM
#define QT_ASCII_CAST_WARNINGS
#define QT_NO_CAST_TO_ASCII

#if defined(__INTEL_COMPILER)
#define _MSC_EXTENSIONS
#include <mathimf.h>
#else
#define BOOST_TYPEOF_SUPPRESS_UNNAMED_NAMESPACE
#endif

#include <QtCore/QtCore>
#include <QtGui/QtGui>
#ifdef USE_DBUS
#include <QtDBus/QtDBus>
#endif
#include <QtNetwork/QtNetwork>
#include <QtOpenGL/QtOpenGL>
#include <QtSql/QtSql>
#include <QtXml/QtXml>

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
#ifdef Q_CC_GNU
#define BOOST_TYPEOF(expr) __typeof__(expr)
#define BOOST_TYPEOF_INCREMENT_REGISTRATION_GROUP() "mumble_pch.hpp"
#define BOOST_TYPEOF_REGISTER_TYPE(expr)
#define BOOST_TYPEOF_REGISTER_TEMPLATE(expra,exprb)
#else
#include <boost/typeof/typeof.hpp>
#endif

#ifdef Q_OS_WIN
#include <windows.h>
#include <shellapi.h>
#ifndef Q_CC_INTEL
#include <math.h>
#define lroundf(x) ( static_cast<long int>( (x) + ((x) >= 0.0f ? 0.5f : -0.5f) ) )
#define lround(x) ( static_cast<long int>( (x) + ((x) >= 0.0 ? 0.5 : -0.5) ) )
#endif
#define M_PI 3.14159265358979323846
#define STACKVAR(type, varname, count) type *varname=reinterpret_cast<type *>(_alloca(sizeof(type) * (count)))
#else
#include <math.h>
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
