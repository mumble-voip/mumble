#ifdef WIN32
#define _WIN32_IE 0x0600
#include <windows.h>
#include <shellapi.h>
#else
#define CopyMemory(dst,ptr,len) memcpy(dst,ptr,len)
#define ZeroMemory(ptr,len) memset(ptr, 0, len)
#define __cdecl
#endif

#include <QtCore>
#include <QtGui>
#include <QtNetwork>
#include <QtOpenGL>
#include <QtSql>
#include <speex/speex.h>

#include <math.h>
#include <speex/speex.h>
#include <speex/speex_jitter.h>
#include <speex/speex_preprocess.h>
#include <speex/speex_echo.h>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#ifndef JITTER_BUFFER_OK
#define SPEEX_ANCIENT
#endif

#ifndef WIN32
typedef WId HWND;
#endif
