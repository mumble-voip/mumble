set(XINPUTCHECK_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../xinputcheck-src")

add_library(xinputcheck STATIC "${XINPUTCHECK_DIR}/xinputcheck.cpp")

target_include_directories(xinputcheck PUBLIC ${XINPUTCHECK_DIR})

target_disable_warnings(xinputcheck)
