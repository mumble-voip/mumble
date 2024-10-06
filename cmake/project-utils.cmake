# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

# This function calculates the SHA3-256 hash of the given FILE.
# The hash is written in numerical form (sum of bytes) into HASH.
function(get_numerical_file_hash FILE HASH)
	file(SHA3_256 ${FILE} HEX_HASH)

	# Split the hex hash string into pairs (each one representing a single byte).
	string(REGEX MATCHALL ".." BYTES ${HEX_HASH})

	set(NUMERICAL 0)

	foreach(BYTE ${BYTES})
		math(EXPR NUMERICAL "${NUMERICAL} + 0x${BYTE}" OUTPUT_FORMAT DECIMAL)
	endforeach()

	set(${HASH} ${NUMERICAL} PARENT_SCOPE)
endfunction()

# This function gets all included subdirectories in the given DIR recursively.
# It'll write the found subdirs into SUBDIRS.
# Note that the DIR itself will not be added to SUBDIRS
# Note also that a subdirectory in this context is a dir that has been included with the
# add_subdirectory command.
function(get_subdirectories SUBDIRS DIR)
	# Get the defined subdirs via the directory property
	get_directory_property(NEW_SUBDIRS DIRECTORY "${DIR}" SUBDIRECTORIES)

	# iterate all subdirs and call this function recursively to also get the
	# subdirs of the subdirs (if any)
	foreach(CURRENT_SUBDIR IN LISTS NEW_SUBDIRS)
		get_subdirectories(REC_SUBDIRS "${CURRENT_SUBDIR}")
		list(APPEND SUBDIRS ${REC_SUBDIRS})
	endforeach()

	list(APPEND SUBDIRS ${NEW_SUBDIRS})

	# "Return" SUBDIRS
	set(${SUBDIRS} PARENT_SCOPE)
endfunction()

# This function gets all defined targets in the given DIR or any of its
# subdirectories as returned by get_subdirectories. The found targets are
# written into DEFINED_TARGETS.
# Note: Only return compilable targets
function(get_targets DEFINED_TARGETS DIR)
	# First get all defined subdirectories
	get_subdirectories(SUBDIRS "${DIR}")

	if(EXISTS "${DIR}/CMakeLists.txt")
		# If the DIR itself contains a CMakeLists.txt file, add it to the
		# list of directories to process as it may already define some targets
		# itself.
		list(APPEND SUBDIRS "${DIR}")
	endif()

	# Iterate over all directories and check each for the defined targets via the
	# respective directory property.
	foreach(CURRENT_SUBDIR IN LISTS SUBDIRS)
		get_directory_property(NEW_TARGETS DIRECTORY "${CURRENT_SUBDIR}" BUILDSYSTEM_TARGETS)

		foreach(CURRENT_TARGET IN LISTS NEW_TARGETS)
			get_target_property(TARGET_TYPE "${CURRENT_TARGET}" TYPE)

			# Only add the target if it is compilable
			if("${TARGET_TYPE}" MATCHES "STATIC_LIBRARY|MODULE_LIBRARY|SHARED_LIBRARY|EXECUTABLE")
				list(APPEND DEFINED_TARGETS ${CURRENT_TARGET})
			endif()
		endforeach()
	endforeach()

	# "Return" DEFINED_TARGETS
	set(${DEFINED_TARGETS} PARENT_SCOPE)
endfunction()

# This function will disable all warnings for the targets defined in the given DIR
# or any of its subdirectories as returned by get_subdirectories.
function(disable_warnings_for_all_targets_in DIR)
	# First get all targets
	get_targets(DEFINED_TARGETS "${DIR}")

	message(STATUS "Disabling warnings for targets: ${DEFINED_TARGETS}")

	# Iterate over all targets and disable warnings for them
	foreach(CURRENT_TARGET IN LISTS DEFINED_TARGETS)
		target_disable_warnings("${CURRENT_TARGET}")
	endforeach()
endfunction()
