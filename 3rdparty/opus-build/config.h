/* config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.ac by autoheader.  */

/* Use ARM NEON optimizations */
/* #undef ARM_HAVE_NEON */

/* Use generic ARMv4 asm optimizations */
/* #undef ARMv4_ASM */

/* Use ARMv5E asm optimizations */
/* #undef ARMv5E_ASM */

/* Use ARMv6 asm optimizations */
/* #undef ARMv6_ASM */

/* Custom modes */
/* #undef CUSTOM_MODES */

/* Assertions */
/* #undef ENABLE_ASSERTIONS */

/* Debug fixed-point implementation */
/* #undef FIXED_DEBUG */

/* Compile as fixed-point (for machines without a fast enough FPU) */
/* #undef FIXED_POINT */

/* Float approximations */
/* #undef FLOAT_APPROX */

/* Fuzzing */
/* #undef FUZZING */

/* Define to 1 if you have the <alloca.h> header file. */
/* #undef HAVE_ALLOCA_H */

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the `lrint' function. */
#define HAVE_LRINT 1

/* Define to 1 if you have the `lrintf' function. */
#define HAVE_LRINTF 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to 1 if you have the `__malloc_hook' function. */
#define HAVE___MALLOC_HOOK 1

/* Define to the sub-directory in which libtool stores uninstalled libraries.
   */
#define LT_OBJDIR ".libs/"

/* Define to 1 if your C compiler doesn't accept -c and -o together. */
/* #undef NO_MINUS_C_MINUS_O */

/* This is a build of OPUS */
#define OPUS_BUILD /**/

/* Use run-time CPU capabilities detection */
/* #undef OPUS_HAVE_RTCD */

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "opus@xiph.org"

/* Define to the full name of this package. */
#define PACKAGE_NAME "opus"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "opus 1.2.1"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "opus"

/* Define to the home page for this package. */
#define PACKAGE_URL ""

/* Define to the version of this package. */
#define PACKAGE_VERSION "1.2.1"

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Make use of alloca */
/* #undef USE_ALLOCA */

/* Use C99 variable-size arrays */
#define VAR_ARRAYS 1

/* Define to empty if `const' does not conform to ANSI C. */
/* #undef const */

/* Define to `__inline__' or `__inline' if that's what the C compiler
   calls it, or to nothing if 'inline' is not supported under any name.  */
#ifndef __cplusplus
/* #undef inline */
#endif

/* Define to the equivalent of the C99 'restrict' keyword, or to
   nothing if this is not supported.  Do not define if restrict is
   supported directly.  */
#define restrict __restrict
/* Work around a bug in Sun C++: it does not support _Restrict or
   __restrict__, even though the corresponding Sun C compiler ends up with
   "#define restrict _Restrict" or "#define restrict __restrict__" in the
   previous line.  Perhaps some future version of Sun C++ will work with
   restrict; if so, hopefully it defines __RESTRICT like Sun C does.  */
#if defined __SUNPRO_CC && !defined __RESTRICT
# define _Restrict
# define __restrict__
#endif

// For now, only enable for OS X. We're sure
// that modern OS X is x86-64, but we can't make
// the same assumption for other Unix-like OSes.
#if defined(__APPLE__) && defined(__x86_64__)
/* Use run-time CPU capabilities detection */
/* #undef OPUS_HAVE_RTCD */

/* Compiler supports X86 SSE Intrinsics */
#define OPUS_X86_MAY_HAVE_SSE 1

/* Compiler supports X86 SSE2 Intrinsics */
#define OPUS_X86_MAY_HAVE_SSE2 1

/* Compiler supports X86 SSE4.1 Intrinsics */
/* #undef OPUS_X86_MAY_HAVE_SSE4_1 */

/* Define if binary requires SSE intrinsics support */
#define OPUS_X86_PRESUME_SSE 1

/* Define if binary requires SSE2 intrinsics support */
#define OPUS_X86_PRESUME_SSE2 1

/* Define if binary requires SSE4.1 intrinsics support */
/* #undef OPUS_X86_PRESUME_SSE4_1 */
#endif
