# Copyright 2005-2017 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

# This .pri file implements modern C++ detection for Mumble.
#
# With this file, it's possible to pass one of
#   c++03, c++11, c++14 or c++1z
# to qmake via CONFIG when building Mumble, to
# manually specify the C++ standard the build
# should target.
#
# If no C++ standard is explicitly chosen on
# the command-line, an appropriate default will
# be chosen by an auto-detection mechanism.
# This mechanism looks at QT_CONFIG and automatically
# adds c++11, c++14 or c++1z to CONFIG. If none of
# these are available, it adds c++03 to CONFIG.
#
# In Qt 5, qmake understands the c++11, c++14, and c++1z
# CONFIG options, and will automatically populate
# QMAKE_CXXFLAGS and QMAKE_LFLAGS with the appropriate
# -std=c++XX values for the chosen C++ standard.
# For Qt 4, this .pri files will add the proper flags,
# because Qt 4's qmake doesn't know of the c++XX CONFIG
# options.
# 
# The c++03 option is unknown for both Qt 4 and Qt 5.
# We handle setting up the proper CXXFLAGS and LFLAGS for
# both versions of Qt when c++03 is in CONFIG.

# First, auto-populate CONFIG with the latest C++ standard
# that Qt has detected support for.
!CONFIG(c++03):!CONFIG(c++11):!CONFIG(c++14):!CONFIG(c++1z) {
	contains(QT_CONFIG, c++1z) {
		CONFIG *= c++1z
	} else:contains(QT_CONFIG, c++14) {
		CONFIG *= c++14
	} else:contains(QT_CONFIG, c++11) {
		CONFIG *= c++11
	}
}
# If no appropriate C++ standard was found in QT_CONFIG, populate
# CONFIG with c++03.
!CONFIG(c++03):!CONFIG(c++11):!CONFIG(c++14):!CONFIG(c++1z) {
	CONFIG += c++03
}

# If CONFIG(c++1z) or CONFIG(c++14) is set, we
# also add the prior "modern" standards to
# CONFIG.
#
# This is because Qt 5.6 (and maybe earlier
# versions) use the presence of CONFIG(c++11)
# to mean "modern C++".
#
# For example, Qt 5.6's
# mkspecs/features/qt_module_headers.prf
# file will automatically add -std=c++98 to
# CXXFLAGS if CONFIG(c++11) isn't set.
#
# The result is that if we ONLY put c++1z in
# CONFIG, that check will cause us to have
# *both* -std=c++98 *and* -std=gnu++1z in
# CXXFLAGS when building on Qt 5.6, which
# is bad.
#
# In order to be compatible with such checks,
# ensure that we have all prior "modern"
# C++ standards in CONFIG, to please Qt.
CONFIG(c++1z) {
	CONFIG *= c++11 c++14
}
CONFIG(c++14) {
	CONFIG *= c++11
}

# Unix-specific handling of modern C++ features.
unix {
	# Qt 4, as opposed to Qt 5, doesn't automatically
	# populate QMAKE_CXXFLAGS and QMAKE_LFLAGS with the
	# appropriate -std=c++XX flags. So, for Qt 4, we'll
	# have to set them up manually.
	lessThan(QT_MAJOR_VERSION, 5) {
		CONFIG(c++1z) {
			QMAKE_CXXFLAGS += -std=c++1z
			QMAKE_LFLAGS += -std=c++1z
		} else:CONFIG(c++14) {
			QMAKE_CXXFLAGS += -std=c++14
			QMAKE_LFLAGS += -std=c++14
		} else:CONFIG(c++11) {
			QMAKE_CXXFLAGS += -std=c++11
			QMAKE_LFLAGS += -std=c++11
		}
	}
	# No Qt versions know about CONFIG(c++03), since
	# its our own internal hint -- so for CONFIG(c++03)
	# we set QMAKE_CXXFLAGS and QMAKE_LFLAGS for both Qt 4
	# and Qt 5.
	CONFIG(c++03):!CONFIG(c++11):!CONFIG(c++14):!CONFIG(c++1z) {
		# Note: we use -std=c++98 to support
		# older compilers. In GCC, the following
		# flags mean the same thing:
		# -std=c++98, -std=c++03, -ansi.
		QMAKE_CXXFLAGS += -std=c++98
		QMAKE_LFLAGS += -std=c++98
	}


	# Debian seems to put C++11 variants of shared libraries
	# in /usr/lib/$triple/c++11.
	#
	# At least, that's the case for ZeroC Ice.
	#
	# The expectation is that this is a general convention,
	# so add it to our library search path in C++11, C++14
	# and C++1z mode.
	CONFIG(c++11)|CONFIG(c++14)|CONFIG(c++1z) {
		MULTIARCH_TRIPLE = $$system($${QMAKE_CC} -print-multiarch 2>/dev/null)
		!isEmpty(MULTIARCH_TRIPLE) {
			# Don't add the Debian-specfic c++11 libdir when using
			# a Mumble buildenv.
			#
			# Adding this directory to QMAKE_LIBDIR this early in
			# the build process will  cause Mumble to prefer this
			# lib dir to the buildenv's own.
			# The easiest way to fix this is to simply avoid adding
			# this dir when building using a Mumble buildenv, since
			# it is irrelevant for us in that setting.
			!CONFIG(buildenv) {
				QMAKE_LIBDIR *= /usr/lib/$${MULTIARCH_TRIPLE}/c++11
			}
		}
	}
}
