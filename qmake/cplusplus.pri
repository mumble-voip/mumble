# Copyright 2005-2020 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

# We enforce (at least) cpp14
CONFIG *= c++14

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
	# No Qt versions know about CONFIG(c++03), since
	# its our own internal hint -- so for CONFIG(c++03)
	# we set QMAKE_CXXFLAGS and QMAKE_LFLAGS.
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
