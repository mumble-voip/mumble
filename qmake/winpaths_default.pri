# Copyright 2005-2019 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

# winpaths_default.pri
#
# This file contains the default paths for Windows dependencies.
# If you want to customize them do NOT CHANGE THIS FILE but instead
# create a winpaths_custom.pri file in this directory and overwrite
# the variables you need to customize. Do NOT COMMIT your custom file
# to the repository.
#

ICE_PATH = %PROGPATH%/ZeroC/Ice-3.4.1
BZIP2_PATH = /dev/bzip2
PROTOBUF_PATH = /dev/protobuf-2.4.1
OPENSSL_PATH = /dev/OpenSSL
LIBSNDFILE_PATH = %PROGPATH%/Mega-Nerd/libsndfile
BOOST_PATH = /dev/Boost
VLD_PATH = %PROGPATH%/Visual Leak Detector
BONJOUR_PATH = %PROGRAMFILES%/Bonjour SDK
ASIO_PATH = /dev/asio
G15SDK_PATH = /dev/G15SDK/LCDSDK

# If we're building in win32-static, include
# winpaths_static.pri to override the defaults
# above.
CONFIG(static) {
	include(winpaths_static.pri)
}

# Include custom file if it exists
exists(winpaths_custom.pri) {
	include(winpaths_custom.pri)
}
