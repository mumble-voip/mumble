# winpaths_default.pri
#
# This file contains the default paths for Windows dependencies.
# If you want to customize them do NOT CHANGE THIS FILE but instead
# create a winpaths_custom.pri file in this directory and overwrite
# the variables you need to customize. Do NOT COMMIT your custom file
# to the repository.
#

ICE_PATH = /dev/Ice
PROTOBUF_PATH = /dev/protobuf-2.3.0
OPENSSL_PATH = /dev/OpenSSL
LIBSNDFILE_PATH = /dev/libsndfile
BOOST_PATH = /dev/Boost
VLD_PATH = /dev/vld
BONJOUR_PATH = /dev/Bonjour
ASIO_PATH = /dev/asio
G15SDK_PATH = /dev/G15SDK/SDK
LIBOGG_PATH = /dev/libogg

# Include custom file if it exists
exists(winpaths_custom.pri) {
	include(winpaths_custom.pri)
}