# winpaths_default.pri
#
# This file contains the default paths for Windows dependencies.
# If you want to customize them do NOT CHANGE THIS FILE but instead
# create a winpaths_custom.pri file in this directory and overwrite
# the variables you need to customize. Do NOT COMMIT your custom file
# to the repository.
#

ICE_PATH = /dev/Ice
PROTOBUF_PATH = /dev/protobuf-2.4.0a
PROTOBUF_PATH = G:/dev/mumble/mumble/dev/protobuf-2.4.0a
OPENSSL_PATH = /dev/OpenSSL
OPENSSL_PATH = G:/dev/mumble/mumble/dev/OpenSSL
LIBSNDFILE_PATH = /dev/libsndfile
LIBSNDFILE_PATH = G:/dev/mumble/mumble/dev/libsndfile
BOOST_PATH = /dev/Boost
BOOST_PATH = G:/dev/mumble/mumble/dev/boost
VLD_PATH = /dev/vld
VLD_PATH = G:/dev/mumble/mumble/dev/vld
BONJOUR_PATH = /Program Files/Bonjour SDK
ASIO_PATH = /dev/asio
G15SDK_PATH = /dev/G15SDK/LCDSDK

# Include custom file if it exists
exists(winpaths_custom.pri) {
	include(winpaths_custom.pri)
}
