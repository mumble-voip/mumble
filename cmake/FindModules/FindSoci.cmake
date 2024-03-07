# Adapted from https://github.com/SOCI/soci/blob/master/cmake/modules/FindSoci.cmake
###############################################################################
# CMake module to search for SOCI library
#
# This module defines:
#  Soci_INCLUDE_DIRS        = include dirs to be used when using the soci library
#  Soci_LIBRARY             = full path to the soci library
#  Soci_VERSION             = the soci version found
#  Soci_FOUND               = true if soci was found
#
# This module respects:
#  LIB_SUFFIX         = (64|32|"") Specifies the suffix for the lib directory
#
# For each component you specify in find_package(), the following variables are set.
#
#  Soci_${COMPONENT}_PLUGIN = full path to the soci plugin (not set for the "core" component)
#  Soci_${COMPONENT}_FOUND
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#
###############################################################################
#
### Global Configuration Section
#
SET(_SOCI_ALL_PLUGINS    mysql odbc postgresql sqlite3)
SET(_SOCI_REQUIRED_VARS  Soci_INCLUDE_DIR Soci_LIBRARY)

#
### FIRST STEP: Find the soci headers.
#
FIND_PATH(
    Soci_INCLUDE_DIR soci.h
	HINTS "/usr/local"
    PATH_SUFFIXES "" "soci"
    DOC "Soci (http://soci.sourceforge.net) include directory")
MARK_AS_ADVANCED(SOCI_INCLUDE_DIR)

SET(Soci_INCLUDE_DIRS ${Soci_INCLUDE_DIR} CACHE STRING "")

#
### SECOND STEP: Find the soci core library. Respect LIB_SUFFIX
#
FIND_LIBRARY(
    Soci_LIBRARY
    NAMES soci_core
    HINTS ${Soci_INCLUDE_DIR}/..
    PATH_SUFFIXES lib${LIB_SUFFIX})
MARK_AS_ADVANCED(SOCI_LIBRARY)

GET_FILENAME_COMPONENT(Soci_LIBRARY_DIR ${Soci_LIBRARY} PATH)
MARK_AS_ADVANCED(Soci_LIBRARY_DIR)

#
### THIRD STEP: Find all installed plugins if the library was found
#
IF(Soci_INCLUDE_DIR AND Soci_LIBRARY)
	SET(Soci_core_FOUND TRUE CACHE BOOL "")

	#
	### FOURTH STEP: Obtain SOCI version
	#
	set(Soci_VERSION_FILE "${SOCI_INCLUDE_DIR}/version.h")
	IF(EXISTS "${Soci_VERSION_FILE}")
		file(READ "${Soci_VERSION_FILE}" VERSION_CONTENT)
		string(REGEX MATCH "#define[ \t]*SOCI_VERSION[ \t]*[0-9]+" VERSION_MATCH "${VERSION_CONTENT}")
		string(REGEX REPLACE "#define[ \t]*SOCI_VERSION[ \t]*" "" VERSION_MATCH "${VERSION_MATCH}")

		IF(NOT VERSION_MATCH)
			message(WARNING "Failed to extract SOCI version")
		ELSE()
			math(EXPR MAJOR "${VERSION_MATCH} / 100000" OUTPUT_FORMAT DECIMAL)
			math(EXPR MINOR "${VERSION_MATCH} / 100 % 1000" OUTPUT_FORMAT DECIMAL)
			math(EXPR PATCH "${VERSION_MATCH} % 100" OUTPUT_FORMAT DECIMAL)

			set(Soci_VERSION "${MAJOR}.${MINOR}.${PATCH}" CACHE STRING "")
		ENDIF()
	ELSE()
		message(WARNING "Unable to check SOCI version")
	ENDIF()

    FOREACH(plugin IN LISTS _SOCI_ALL_PLUGINS)

        FIND_LIBRARY(
            Soci_${plugin}_PLUGIN
            NAMES soci_${plugin}
            HINTS ${Soci_INCLUDE_DIR}/..
            PATH_SUFFIXES lib${LIB_SUFFIX})
        MARK_AS_ADVANCED(Soci_${plugin}_PLUGIN)

        IF(Soci_${plugin}_PLUGIN)
			SET(Soci_${plugin}_FOUND TRUE CACHE BOOL "")
        ELSE()
			SET(Soci_${plugin}_FOUND FALSE CACHE BOOL "")
        ENDIF()

    ENDFOREACH()

ENDIF()

#
### ADHERE TO STANDARDS
#
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Soci
	REQUIRED_VARS ${_SOCI_REQUIRED_VARS}
	VERSION_VAR Soci_VERSION
	HANDLE_COMPONENTS
)
