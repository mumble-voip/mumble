#ifndef _CELT_SUPPORT_MUMBLE
#define _CELT_SUPPORT_MUMBLE

#ifdef __APPLE__

#include <stdio.h>

#define OVERRIDE_CELT_FATAL

extern mumble_macx_celt_fatal(const char *str, const char *file, int line);

static inline void _celt_fatal(const char *str, const char *file, int line) {
	if (! mumble_macx_celt_fatal) {
		fprintf(stderr, "Fatal (internal) libcelt error in %s, line %d: %s\n", file, line, str);
		abort();
	}

	mumble_macx_celt_fatal(str, file, line);
}

#endif /* __APPLE__ */

#endif /* _CELT_SUPPORT_MUMBLE */
