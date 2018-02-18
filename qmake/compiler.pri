# Copyright 2005-2018 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

include(qt.pri)
include(uname.pri)
include(buildenv.pri)
include(builddir.pri)
include(cplusplus.pri)

CONFIG *= warn_on

# Enable zlib compression for assets
# embedded via rcc. This is done to
# keep the size of rcc's output
# source files under control.
QMAKE_RESOURCE_FLAGS += -compress 9

# The architecture we're building for.
# On Windows, Mumble supports overriding the
# architecture of the compiler via the
# force-x86_64-toolchain and force-x86-toolchain
# CONFIG options. Because we have these, qmake's
# QMAKE_TARGET.arch doesn't suffice any longer, and
# we define MUMBLE_ARCH to be used in its place.
MUMBLE_ARCH = $$QMAKE_TARGET.arch
# When using Qt 5, use QT_ARCH instead of QMAKE_TARGET.
# It also works for cross-builds.
isEqual(QT_MAJOR_VERSION, 5) {
	MUMBLE_ARCH = $$QT_ARCH
	# QT_ARCH uses 'i386' instead of 'x86',
	# so map that value back to what we expect.
	equals(MUMBLE_ARCH, i386) {
		MUMBLE_ARCH=x86
	}
}

win32-g++ {
	DEFINES *= MINGW_HAS_SECURE_API

	# Enable SSE
	QMAKE_CFLAGS *= -msse -msse2
	QMAKE_CXXFLAGS *= -msse -msse2

	CONFIG(symbols) {
		# Configure build to be able to properly debug release builds
		QMAKE_CFLAGS *= -g
		QMAKE_CXXFLAGS *= -g
	}
}

win32-msvc* {
	# Define the CONFIG options 'force-x86-toolchain' and
	# 'force-x86_64-toolchain'. These can be used to force
	# the target of a .pro file to be built for a specific
	# architecture, regardless of the actual architecture
	# used by the current build environment.
	FULL_MKSPEC_PATH = $$QMAKESPEC
	CURRENT_MKSPEC = $$basename(QMAKESPEC)

	CONFIG(force-x86-toolchain) {
		MUMBLE_ARCH = x86
		include(toolchain/$${CURRENT_MKSPEC}/x86-xp.toolchain)
	}

	CONFIG(force-x86_64-toolchain) {
		MUMBLE_ARCH = x86_64
		include(toolchain/$${CURRENT_MKSPEC}/x64.toolchain)
	}

	# Import dependency paths for windows
	include(winpaths_default.pri)

	INCLUDEPATH *= "$$BOOST_PATH/include"
	QMAKE_LIBDIR *= "$$OPENSSL_PATH/lib" "$$LIBSNDFILE_PATH/lib" "$$BOOST_PATH/lib"
	INCLUDEPATH *= "$$OPENSSL_PATH/include" "$$LIBSNDFILE_PATH/include"

	# Sanity check that LIB and INCLUDE are properly set up.
	INCLUDE_VAR=$$(INCLUDE)
	LIB_VAR=$$(LIB)
	isEmpty(LIB_VAR) {
		error("The LIB environment variable is not set. Are you not in a build environment?")
	}
	isEmpty(INCLUDE_VAR) {
		error("The INCLUDE environment variable is not set. Are you not in a build environment?")
	}

	CONFIG(analyze) {
		QMAKE_CFLAGS_DEBUG *= /analyze
		QMAKE_CXXFLAGS_DEBUG *= /analyze
		QMAKE_CFLAGS_RELEASE *= /analyze
		QMAKE_CXXFLAGS_RELEASE *= /analyze
	}

	# Always enable warnings-as-errors
	# if we're inside a Mumble build environment.
	# Can be disabled with CONFIG+=no-warnings-as-errors.
	CONFIG(buildenv):!CONFIG(no-warnings-as-errors) {
		CONFIG += warnings-as-errors
	}

	CONFIG(warnings-as-errors) {
		QMAKE_CFLAGS *= -WX
		QMAKE_CXXFLAGS *= -WX
		QMAKE_LFLAGS *= -WX
	}

	# Increase the verbosity of the linker.
	# For now, only increases the verbosity
	# of searching for libraries.
	# This is useful when diagnosing libraries
	# that use a wrong runtime library DLL.
	CONFIG(verbose-linker) {
		QMAKE_LFLAGS *= /VERBOSE:LIB
	}

	# Increase PCH heap to 150MB: https://msdn.microsoft.com/en-us/library/bdscwf1c.aspx
	QMAKE_CFLAGS *= -Zm200
	QMAKE_CXXFLAGS *= -Zm200

	# Disable C4091 for x86 builds. We build against the v140_xp toolset, which uses
	# a modified Windows 7 SDK which is compatible with Windows XP. Many system headers
	# emit C4091, typically because of erroneous typedefs. For example 'GPFIDL_FLAGS' in
	# shlobj.h, and an anonymous enum that includes KSPROPERTY_ONESHOT_RECONNECT in ksmedia.h.
	equals(MUMBLE_ARCH, x86) {
		QMAKE_CFLAGS *= -wd4091
		QMAKE_CXXFLAGS *= -wd4091
	}

	QMAKE_CFLAGS_RELEASE *= -Ox /fp:fast
	QMAKE_CXXFLAGS_RELEASE *= -Ox /fp:fast

	equals(MUMBLE_ARCH, x86) {
		QMAKE_LFLAGS_RELEASE -= /SafeSEH
	}

	# MSVS 2012 and 2013's cl.exe will generate SSE2 code by default,
	# unless an explict arch is set.
	# For our non-64 x86 builds, our binaries should not contain any
	# SSE2 code, so override the default by using -arch:SSE.
	equals(MUMBLE_ARCH, x86) {
		QMAKE_CFLAGS_RELEASE *= -arch:SSE
		QMAKE_CXXFLAGS_RELEASE *= -arch:SSE
	}

	# Qt 5.4 uses -Zc:strictStrings by default on MSVS 2013.
	# TextToSpeech_win.cpp uses sapi.h, which isn't compatible
	# with the strictStrings option due to bad conversions
	# in some of its functions's default parameters.
	QMAKE_CFLAGS_RELEASE -= -Zc:strictStrings
	QMAKE_CXXFLAGS_RELEASE -= -Zc:strictStrings
	QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO -= -Zc:strictStrings
	QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO -= -Zc:strictStrings
	# The mkspec update for MSVC2015 puts the flag directly in
	# CFLAGS and CXXFLAGS, so try there, too.
	QMAKE_CFLAGS -= -Zc:strictStrings
	QMAKE_CXXFLAGS -= -Zc:strictStrings

	# Explicitly set the subsystem versions to
	# 5.01 (XP) for x86 and 6.00 (Vista) for x64.
	#
	# Qt expands the @QMAKE_SUBSYSTEM_SUFFIX@ via
	# qt_config.prf, which doesn't seem to trigger
	# for us. So we'll just try our luck.
	QMAKE_LFLAGS_CONSOLE -= /SUBSYSTEM:CONSOLE
	QMAKE_LFLAGS_CONSOLE -= /SUBSYSTEM:CONSOLE@QMAKE_SUBSYSTEM_SUFFIX@
	QMAKE_LFLAGS_WINDOWS -= /SUBSYSTEM:WINDOWS
	QMAKE_LFLAGS_WINDOWS -= /SUBSYSTEM:WINDOWS@QMAKE_SUBSYSTEM_SUFFIX@
	!isEmpty(QMAKE_LFLAGS_WINDOWS) {
		error("QMAKE_LFLAGS_WINDOWS is not empty. Please adjust the pri file.")
	}
	!isEmpty(QMAKE_LFLAGS_CONSOLE) {
		error("QMAKE_LFLAGS_CONSOLE is not empty. Please adjust the pri file.")
	}
	equals(MUMBLE_ARCH, x86) {
		QMAKE_LFLAGS_CONSOLE += /SUBSYSTEM:CONSOLE,5.01
		QMAKE_LFLAGS_WINDOWS += /SUBSYSTEM:WINDOWS,5.01
	}
	equals(MUMBLE_ARCH, x86_64) {
		QMAKE_LFLAGS_CONSOLE += /SUBSYSTEM:CONSOLE,6.00
		QMAKE_LFLAGS_WINDOWS += /SUBSYSTEM:WINDOWS,6.00
	}

	DEFINES *= RESTRICT=
	CONFIG(sse2) {
	      QMAKE_CFLAGS_RELEASE -= -arch:SSE
	      QMAKE_CFLAGS_DEBUG -= -arch:SSE
	      QMAKE_CFLAGS += -arch:SSE2
	}

	CONFIG(symbols) {
		# Configure build to be able to properly debug release builds
		# (https://msdn.microsoft.com/en-us/library/fsk896zz.aspx).
		# This includes explicitely disabling /Oy to help debugging
		# (https://msdn.microsoft.com/en-us/library/2kxx5t2c.aspx).
		# Also set /Zo to enhance optimized debugging
		# (https://msdn.microsoft.com/en-us/library/dn785163.aspx?f=255&MSPPError=-2147217396).
		QMAKE_CFLAGS_RELEASE -= -Oy
		QMAKE_CXXFLAGS_RELEASE -= -Oy

		QMAKE_CFLAGS_RELEASE *= -GR -Zi -Zo -Oy-
		QMAKE_CXXFLAGS_RELEASE *= -GR -Zi -Zo -Oy-

		QMAKE_LFLAGS *= /DEBUG /OPT:REF /OPT:ICF /INCREMENTAL:NO
	}

	CONFIG(vld) {
		CONFIG(debug, debug|release) {
			DEFINES *= USE_VLD
			INCLUDEPATH *= "$$VLD_PATH/include"
			QMAKE_LIBDIR *= "$$VLD_PATH/lib"
		}
	}
}

unix|win32-g++ {
	DEFINES *= RESTRICT=__restrict__
	QMAKE_CFLAGS *= -fvisibility=hidden
	QMAKE_CXXFLAGS *= -fvisibility=hidden
	QMAKE_OBJECTIVE_CFLAGS *= -fvisibility=hidden
	QMAKE_OBJECTIVE_CXXFLAGS *= -fvisibility=hidden

	QMAKE_CFLAGS	         *= -Wall -Wextra
	QMAKE_CXXFLAGS           *= -Wall -Wextra
	QMAKE_OBJECTIVE_CFLAGS   *= -Wall -Wextra
	QMAKE_OBJECTIVE_CXXFLAGS *= -Wall -Wextra

	# Always enable warnings-as-errors
	# if we're inside a Mumble build environment.
	# Can be disabled with CONFIG+=no-warnings-as-errors.
	CONFIG(buildenv):!CONFIG(no-warnings-as-errors) {
		CONFIG += warnings-as-errors
	}

	CONFIG(warnings-as-errors) {
		QMAKE_CFLAGS	         *= -Werror
		QMAKE_CXXFLAGS	         *= -Werror
		QMAKE_OBJECTIVE_CFLAGS   *= -Werror
		QMAKE_OBJECTIVE_CXXFLAGS *= -Werror
	}

	CONFIG(opt-gcc) {
		QMAKE_CC = /opt/gcc/bin/gcc
		QMAKE_CXX = /opt/gcc/bin/g++
		QMAKE_LINK = /opt/gcc/bin/g++
	}

	CONFIG(optgen) {
		QMAKE_CFLAGS *= -O3 -march=native -ffast-math -ftree-vectorize -fprofile-generate
		QMAKE_CXXFLAGS *= -O3 -march=native -ffast-math -ftree-vectorize -fprofile-generate
		QMAKE_OBJECTIVE_CFLAGS *= -O3 -march=native -ffast-math -ftree-vectorize -fprofile-generate
		QMAKE_OBJECTIVE_CXXFLAGS *= -O3 -march=native -ffast-math -ftree-vectorize -fprofile-generate
		QMAKE_LFLAGS *= -fprofile-generate
	}

	CONFIG(optimize) {
		QMAKE_CFLAGS *= -O3 -march=native -ffast-math -ftree-vectorize -fprofile-use
		QMAKE_CXXFLAGS *= -O3 -march=native -ffast-math -ftree-vectorize -fprofile-use
		QMAKE_OBJECTIVE_CFLAGS *= -O3 -march=native -ffast-math -ftree-vectorize -fprofile-use
		QMAKE_OBJECTIVE_CXXFLAGS *= -O3 -march=native -ffast-math -ftree-vectorize -fprofile-use
	}
}

contains(UNAME, FreeBSD)|contains(UNAME, OpenBSD) {
	QMAKE_CFLAGS *= "-I/usr/local/include" "-isystem /usr/local/include"
	QMAKE_CXXFLAGS *= "-I/usr/local/include" "-isystem /usr/local/include"
	QMAKE_LIBDIR *= /usr/lib
	QMAKE_LIBDIR *= /usr/local/lib
}

unix:!macx {
	# If we're building in a Mumble build environment,
	# add its include and lib dirs to the build configuration.
	CONFIG(buildenv) {
		SYSTEM_INCLUDES = $$(MUMBLE_PREFIX)/include $$[QT_INSTALL_HEADERS]
		QMAKE_LIBDIR *= $$(MUMBLE_PREFIX)/lib

		for(inc, $$list($$SYSTEM_INCLUDES)) {
			QMAKE_CFLAGS *= "-I$$inc" "-isystem $$inc"
			QMAKE_CXXFLAGS += "-I$$inc" "-isystem $$inc"
		}
	}

	CONFIG(dpkg-buildflags) {
		QMAKE_CFLAGS *= $$system(dpkg-buildflags --get CFLAGS)
		QMAKE_CXXFLAGS *= $$system(dpkg-buildflags --get CXXFLAGS)
		QMAKE_CPPFLAGS *= $$system(dpkg-buildflags --get CPPFLAGS)
		QMAKE_LFLAGS *= $$system(dpkg-buildflags --get LDFLAGS)
	}

	CONFIG(debug, debug|release) {
		QMAKE_CFLAGS *= -fstack-protector -fPIE
		QMAKE_CXXFLAGS *= -fstack-protector -fPIE
		QMAKE_LFLAGS *= -pie
		QMAKE_LFLAGS *= -Wl,--no-add-needed
	}

	# Ensure _FORTIFY_SOURCE is not used in debug builds.
	#
	# First, ensure _FORTIFY_SOURCE is undefined.
	# Then -- and, only for release builds -- set
	# _FORTIFY_SOURCE=2.
	#
	# We can't use _FORTIFY_SOURCE in debug builds (which are
	# built with -O0) because _FORTIFY_SOURCE=1 requires -O1
	# and _FORTIFY_SOURCE=2 requires -O2.
	# Modern GLIBCs warn about this since
	# https://sourceware.org/bugzilla/show_bug.cgi?id=13979.
	# In Mumble builds with warnings-as-errors, this will
	# cause build failures.
	#
	# We use the += operator because we care about the
	# ordering of unsetting versus setting the preprocessor
	# define. If they're in the wrong order, this will not
	# work as intended.
	QMAKE_CFLAGS += -U_FORTIFY_SOURCE
	QMAKE_CXXFLAGS += -U_FORTIFY_SOURCE
	CONFIG(release, debug|release) {
		QMAKE_CFLAGS += -D_FORTIFY_SOURCE=2
		QMAKE_CXXFLAGS += -D_FORTIFY_SOURCE=2
	}

	QMAKE_LFLAGS *= -Wl,-z,relro -Wl,-z,now

	CONFIG(symbols) {
		QMAKE_CFLAGS *= -g
		QMAKE_CXXFLAGS *= -g
	}
}

macx {
	SYSTEM_INCLUDES = $$(MUMBLE_PREFIX)/include $$[QT_INSTALL_HEADERS]
	QMAKE_LIBDIR *= $$(MUMBLE_PREFIX)/lib

	for(inc, $$list($$SYSTEM_INCLUDES)) {
		QMAKE_CFLAGS += "-I$$inc" "-isystem $$inc"
		QMAKE_CXXFLAGS += "-I$$inc" "-isystem $$inc"
		QMAKE_OBJECTIVE_CFLAGS += "-I$$inc" "-isystem $$inc"
		QMAKE_OBJECTIVE_CXXFLAGS += "-I$$inc" "-isystem $$inc"
	}

	!CONFIG(universal) {
		CONFIG += no-pch

		# Qt 5.1 and greater want short-form OS X SDKs.
		isEqual(QT_MAJOR_VERSION, 5) {
			QMAKE_MAC_SDK = macosx
		} else {
			QMAKE_MAC_SDK = $$system(xcrun --sdk macosx --show-sdk-path 2>/dev/null)
			isEmpty(QMAKE_MAC_SDK) {
				QMAKE_MAC_SDK = $$system(xcode-select --print-path)/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.8.sdk
				!exists($$QMAKE_MAC_SDK) {
					message("Unable to find usable OS X SDK")
					error("Aborting build")
				}
			}
		}

		QMAKE_CC = $$system(xcrun -find clang)
		QMAKE_CXX = $$system(xcrun -find clang++)
		QMAKE_LINK = $$system(xcrun -find clang++)

		# Set target macOS version to 10.8 for Qt 5.10 and above.
		MUMBLE_TARGET_MACOS_VERSION = 10.7
		isEqual(QT_MAJOR_VERSION, 5):greaterThan(QT_MINOR_VERSION, 9) {
			MUMBLE_TARGET_MACOS_VERSION = 10.8
		}

		QMAKE_MACOSX_DEPLOYMENT_TARGET = $$MUMBLE_TARGET_MACOS_VERSION
		QMAKE_CFLAGS += -mmacosx-version-min=$$MUMBLE_TARGET_MACOS_VERSION
		QMAKE_CXXFLAGS += -mmacosx-version-min=$$MUMBLE_TARGET_MACOS_VERSION
		QMAKE_OBJECTIVE_CFLAGS += -mmacosx-version-min=$$MUMBLE_TARGET_MACOS_VERSION
		QMAKE_OBJECTIVE_CXXFLAGS += -mmacosx-version-min=$$MUMBLE_TARGET_MACOS_VERSION
	} else {
		XCODE_PATH=$$system(xcode-select -print-path)
		CONFIG += x86 ppc no-cocoa
		QMAKE_MAC_SDK = $${XCODE_PATH}/SDKs/MacOSX10.5.sdk
		QMAKE_CC = $${XCODE_PATH}/usr/bin/gcc-4.2
		QMAKE_CXX = $${XCODE_PATH}/usr/bin/g++-4.2
		QMAKE_LINK = $${XCODE_PATH}/usr/bin/g++-4.2
		QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.4
		QMAKE_CFLAGS += -mmacosx-version-min=10.4 -Xarch_i386 -mmmx -Xarch_i386 -msse -Xarch_i386 -msse2
		QMAKE_CXXFLAGS += -mmacosx-version-min=10.4 -Xarch_i386 -mmmx -Xarch_i386 -msse -Xarch_i386 -msse2
		QMAKE_OBJECTIVE_CFLAGS += -mmacosx-version-min=10.4 -Xarch_i386 -mmmx -Xarch_i386 -msse -Xarch_i386 -msse2
		QMAKE_OBJECTIVE_CXXFLAGS += -mmacosx-version-min=10.4 -Xarch_i386 -mmmx -Xarch_i386 -msse -Xarch_i386 -msse2
		DEFINES += USE_MAC_UNIVERSAL
	}

	QMAKE_LFLAGS += -Wl,-dead_strip -framework Cocoa -framework Carbon

	CONFIG(symbols) {
		QMAKE_CFLAGS *= -gfull -gdwarf-2
		QMAKE_CXXFLAGS *= -gfull -gdwarf-2
		QMAKE_OBJECTIVE_CFLAGS *= -gfull -gdwarf-2
		QMAKE_OBJECTIVE_CXXFLAGS *= -gfull -gdwarf-2
	}
}

CONFIG(clang-analyzer) {
	QMAKE_CC = $$(CC)
	QMAKE_CXX = $$(CXX)
}

CONFIG(no-pch) {
	CONFIG -= precompile_header
} else {
 	CONFIG *= precompile_header
}

CONFIG(debug, debug|release) {
	DEFINES *= SNAPSHOT_BUILD
}
