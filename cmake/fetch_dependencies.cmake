include(FetchContent)

FetchContent_Declare(
	opus
	GIT_REPOSITORY https://github.com/xiph/opus
	GIT_TAG v1.3.1
)
FetchContent_Declare(
	findPythonInterpreter
	GIT_REPOSITORY https://github.com/Krzmbrzl/FindPythonInterpreter.git
	GIT_TAG bb4d3ea8434eebef40df35434a9b6ef410fce0b2
)

FetchContent_GetProperties(opus)
FetchContent_GetProperties(findPythonInterpreter)

if (NOT opus_POPULATED OR NOT findPythonInterpreter_POPULATED)
	message(STATUS "Fetching dependencies (might take a bit)...")

	if (NOT opus_POPULATED)
	  FetchContent_Populate(opus)
	  add_subdirectory(${opus_SOURCE_DIR} ${opus_BINARY_DIR})
	endif()

	if (NOT findPythonInterpreter_POPULATED)
	  FetchContent_Populate(findPythonInterpreter)
	endif()

	message(STATUS "All dependencies fetched")
endif()

list(APPEND CMAKE_MODULE_PATH "${findPythonInterpreter_SOURCE_DIR}")

message(STATUS "${CMAKE_MODULE_PATH}")
