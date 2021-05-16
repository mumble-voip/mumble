# Copyright 2021 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

include(CPack)

set(CPACK_PACKAGE_NAME "${CMAKE_PROJECT_NAME}" CACHE STRING "")
set(CPACK_PACKAGE_VENDOR "The Mumble Developers" CACHE STRING "")
set(CPACK_PACKAGE_DESCRIPTION "${CMAKE_PROJECT_DESCRIPTION}" CACHE STRING "")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://www.mumble.info/" CACHE STRING "")
set(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}/icons/mumble_256x256.png" CACHE STRING "")
set(CPACK_PACKAGE_CHECKSUM "SHA256" CACHE STRING "")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE" CACHE STRING "")
set(CPACK_PACKAGE_CONTACT "Mumble Developers <contact@mumble.info>" CACHE STRING "")

# DEB packages
set(CPACK_DEBIAN_PACKAGE_SECTION "comm" CACHE STRING "")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "${CPACK_PACKAGE_CONTACT}" CACHE STRING "")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "TODO" CACHE STRING "")

message(STATUS "Dummy here ${CPACK_DEBIAN_PACKAGE_MAINTAINER}")
