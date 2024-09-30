# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

include(FetchContent)

FetchContent_Declare(
  googlebenchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG        v1.6.0
)

set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "")

FetchContent_MakeAvailable(googlebenchmark)

add_subdirectory(protocol)
add_subdirectory(AudioReceiverBuffer)
