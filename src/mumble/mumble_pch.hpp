#ifndef _MUMBLE_PCH_H
#define _MUMBLE_PCH_H

#define QT_NO_DEBUG_STREAM
#define QT_ASCII_CAST_WARNINGS
#define QT_NO_CAST_TO_ASCII

#define _WINSOCKAPI_

#if defined(__INTEL_COMPILER)
#define _MSC_EXTENSIONS
#include <mathimf.h>
#else
#define BOOST_TYPEOF_SUPPRESS_UNNAMED_NAMESPACE
#endif

#ifdef __APPLE__
#include <Carbon/Carbon.h>
#include <CoreFoundation/CoreFoundation.h>
#include <ApplicationServices/ApplicationServices.h>
#undef check
#undef nil
#undef TYPE_BOOL
#endif

#include <QtCore/QtCore>
#include <QtGui/QtGui>
#include <QtSvg/QtSvg>
#ifdef USE_DBUS
#include <QtDBus/QtDBus>
#endif
#include <QtNetwork/QtNetwork>
#include <QtOpenGL/QtOpenGL>
#include <QtSql/QtSql>
#include <QtXml/QtXml>

#include <openssl/aes.h>
#include <openssl/rand.h>
#include <openssl/pem.h>
#include <openssl/conf.h>
#include <openssl/x509v3.h>
#include <openssl/pkcs12.h>
/* OpenSSL defines set_key. This breaks our protobuf-generated setters. */
#undef set_key

#include <celt.h>
#include <celt_header.h>
#include <speex/speex_jitter.h>
#include <speex/speex_preprocess.h>
#include <speex/speex_echo.h>
#include <speex/speex_resampler.h>
#define __int64_t __int64
#include <sndfile.h>
#undef __int64_t

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/variance.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/bind.hpp>
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
#include <winsock2.h>
#include <qos2.h>
#include <wintrust.h>
#include <Softpub.h>
#include <Dbt.h>
#include <delayimp.h>
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

#define iroundf(x) ( static_cast<int>(x) )

#ifdef USE_BONJOUR
#include <dns_sd.h>
#endif

#endif
