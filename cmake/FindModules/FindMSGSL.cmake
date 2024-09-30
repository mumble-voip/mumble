# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

# Taken from / inspired by https://github.com/shohirose/cmake-find-package/blob/4535cbcbf6488b6acefb2ed9aeb240cdd5e71701/FindMSGSL.cmake

# This module try to find the Guideline Support Library (GSL) provided by Microsoft.
# Once done this will define
#
#  MSGSL_FOUND - Microsoft GSL found
#  MSGSL_INCLUDE_DIR - Microsoft GSL include directory
#
# Available targets:
#  MSGSL::MSGSL
#
# This will define the following cache variables:
#  MSGSL_CONTRACT_VIOLATION_CONFIG = Throw/Terminate/Unenforced

find_path(MSGSL_INCLUDE_DIR gsl/gsl
  HINTS
    ENV MSGSL_ROOT
    ENV MSGSL_INCLUDE_DIR
    ${MSGSL_ROOT}
    /usr
    /usr/local
    $ENV{HOME}/.local
  PATH_SUFFIXES
    include
  )

mark_as_advanced(MSGSL_INCLUDE_DIR)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(MSGSL
  REQUIRED_VARS MSGSL_INCLUDE_DIR
  )

if(MSGSL_FOUND AND NOT TARGET MSGSL::MSGSL)
  add_library(MSGSL::MSGSL INTERFACE IMPORTED)
  target_include_directories(MSGSL::MSGSL
    INTERFACE ${MSGSL_INCLUDE_DIR}
    )
  target_compile_features(MSGSL::MSGSL
    INTERFACE cxx_std_14
    )

  set(MSGSL_CONTRACT_VIOLATION_CONFIG Throw CACHE STRING
    "Configuration options for Microsoft GSL implementation's behavior \
    when pre/post conditions on the Microsoft GSL types are violated. \
    Throw/Terminate/Unenforced"
    )

  target_compile_definitions(MSGSL::MSGSL
    INTERFACE
      $<$<STREQUAL:MSGSL_CONTRACT_VIOLATION_CONFIG,Throw>:GSL_THROW_ON_CONTRACT_VIOLATION>
      $<$<STREQUAL:MSGSL_CONTRACT_VIOLATION_CONFIG,Unenforced>:GSL_UNENFORCED_ON_CONTRACT_VIOLATION>
      $<$<STREQUAL:MSGSL_CONTRACT_VIOLATION_CONFIG,Terminate>:GSL_TERMINATE_ON_CONTRACT_VIOLATION>
    )
endif()
