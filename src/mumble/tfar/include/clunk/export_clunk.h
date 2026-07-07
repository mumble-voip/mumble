#ifndef CLUNK_EXPORT_H__
#define CLUNK_EXPORT_H__

#include <clunk/config.h>

/* Shared library support */
#ifdef _MSC_VER
#	pragma warning(disable:4251)  /* needs to have dll-interface used by client */
#	pragma warning(disable:4275)  /* non dll-interface struct */
#	pragma warning(disable:4786) /* debug info exceedes 255 bytes */

#	define DLLIMPORT __declspec(dllimport)
#	define DLLEXPORT __declspec(dllexport)
#	define DLLDLLLOCAL
#	define DLLDLLPUBLIC
#else
#	ifdef GCC_HASCLASSVISIBILITY
#		define DLLIMPORT __attribute__ ((visibility("default")))
#		define DLLEXPORT __attribute__ ((visibility("default")))
#		define DLLDLLLOCAL __attribute__ ((visibility("hidden")))
#		define DLLDLLPUBLIC __attribute__ ((visibility("default")))
#	else
#		define DLLIMPORT
#		define DLLEXPORT
#		define DLLDLLLOCAL
#		define DLLDLLPUBLIC
#	endif
#endif

#ifndef CLUNKAPI
#	define CLUNKAPI DLLIMPORT
#endif

#ifndef CLUNKCAPI
#       ifdef __cplusplus
#               define CLUNKCAPI extern "C" CLUNKAPI
#       else
#               define CLUNKCAPI CLUNKAPI
#       endif
#endif

#endif
