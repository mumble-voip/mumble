#ifndef _SPEEX_SUPPORT_MUMBLE
#define _SPEEX_SUPPORT_MUMBLE

#ifdef __APPLE__

#include <stdio.h>

#define OVERRIDE_SPEEX_FATAL

extern mumble_macx_speex_fatal(const char *str, const char *file, int line);

static inline void _speex_fatal(const char *str, const char *file, int line) {
	if (! mumble_macx_speex_fatal) {
		fprintf(stderr, "Fatal (internal) libspeex error in %s, line %d: %s\n", file, line, str);
		abort();
	}

	mumble_macx_speex_fatal(str, file, line);
}

#endif /* __APPLE__ */

#endif /* _SPEEX_SUPPORT_MUMBLE */
