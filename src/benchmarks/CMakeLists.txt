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
