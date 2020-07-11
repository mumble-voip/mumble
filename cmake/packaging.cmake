# Copyright 2020 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

if(WIN32)
	set(CPACK_GENERATOR WIX)

	if(64_BIT)
		set(CPACK_WIX_PRODUCT_GUID "E028BDFC-3FE2-4BEE-A33B-EB9C80611555")
	elseif(32_BIT)
		set(CPACK_WIX_PRODUCT_GUID "B0EEFCC7-8A9C-4471-AB10-CBD35BE3161D")
	endif()

	set(CPACK_WIX_CULTURES "en-us")
	set(CPACK_WIX_LCID "1033")
	set(CPACK_WIX_UI_BANNER "${CMAKE_SOURCE_DIR}/installer/bannrbmp.bmp")
	set(CPACK_WIX_UI_DIALOG "${CMAKE_SOURCE_DIR}/installer/dlgbmp.bmp")
	set(CPACK_WIX_PRODUCT_GUID "84afea8b-15e5-4cc7-b77d-27dd17030944")
	set(CPACK_WIX_PRODUCT_ICON "${CMAKE_SOURCE_DIR}/icons/mumble.ico")
	set(CPACK_WIX_EXTENSIONS "WiXUtilExtension")
	set(CPACK_WIX_TEMPLATE "wix-template.xml")
endif()

set(CPACK_PACKAGE_EXECUTABLES "mumble;Mumble" "murmur;Murmur")
set(CPACK_CREATE_DESKTOP_LINKS "mumble;Mumble" "murmur;Murmur")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "Mumble")

if(client)
	set(CPACK_PACKAGE_NAME "Mumble (client)")
	set(CPACK_PACKAGE_FILE_NAME "mumble_client-${version}")
	set(CPACK_PACKAGE_VENDOR "Mumble VoIP")
	set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Mumble is a free, open source, low latency, high quality voice chat application.")
	set(CPACK_PACKAGE_HOMEPAGE_URL "https://mumble.info")
	set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/installer/gpl.rtf")
	set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")
	set(CPACK_COMPONENTS_ALL mumble_client)
	set(CPACK_COMPONENT_MUMBLE_CLIENT_DISPLAY_NAME "Client")
	set(CPACK_COMPONENT_MUMBLE_CLIENT_DESCRIPTION "The Mumble VoIP Client")
	#set(CPACK_WIX_PATCH_FILE "${CMAKE_SOURCE_DIR}/installer/client-shortcuts.xml")
	set(CPACK_COMPONENT_MUMBLE_CLIENT_REQUIRED ON)

	install(FILES
		"${CMAKE_SOURCE_DIR}/CHANGES"
		"${CMAKE_SOURCE_DIR}/installer/gpl.txt"
		"${CMAKE_SOURCE_DIR}/installer/qt.txt"
		"${CMAKE_SOURCE_DIR}/installer/speex.txt"
		DESTINATION "."
		COMPONENT mumble_client
	)
endif()

if(server)
	if(NOT client)
		if(WIN32)
			set(CPACK_WIX_PRODUCT_GUID "ccc5eaa4-0b88-4197-9e54-580d37003903")
			set(CPACK_WIX_UPGRADE_GUID "03e9476f-0f75-4661-bfc9-a9daeb23d3a0")
		endif()

		set(CPACK_PACKAGE_NAME "Mumble (server)")
		set(CPACK_PACKAGE_FILE_NAME "mumble_server-${version}")
		set(CPACK_PACKAGE_VENDOR "Mumble VoIP")
		set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Mumble is a free, open source, low latency, high quality voice chat application.")
		set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/installer/gpl.rtf")
		set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README")
		set(CPACK_COMPONENT_MUMBLE_SERVER_REQUIRED ON)

		install(FILES
			"${CMAKE_SOURCE_DIR}/CHANGES"
			"${CMAKE_SOURCE_DIR}/installer/gpl.txt"
			"${CMAKE_SOURCE_DIR}/installer/qt.txt"
			DESTINATION "."
			COMPONENT mumble_server
		)
	endif()

	set(CPACK_COMPONENTS_ALL mumble_server)
	set(CPACK_COMPONENT_MUMBLE_SERVER_DISPLAY_NAME "Server")
	set(CPACK_COMPONENT_MUMBLE_SERVER_DESCRIPTION "The Mumble VoIP Server")
	#set(CPACK_WIX_PATCH_FILE "${CMAKE_SOURCE_DIR}/installer/server-shortcuts.xml")

	if(WIN32)
		install(FILES "${CMAKE_SOURCE_DIR}/scripts/murmur.ini" DESTINATION "." COMPONENT mumble_server)
	endif()
endif()

if(client AND server)
	if(WIN32)
		set(CPACK_PACKAGE_NAME "Mumble")
		set(CPACK_PACKAGE_FILE_NAME "Mumble-${version}")
		set(CPACK_PACKAGE_VENDOR "Mumble VoIP")
		set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Mumble is a free, open source, low latency, high quality voice chat application.")
		set(CPACK_COMPONENT_MUMBLE_SERVER_DISABLED ON)
		# set(CPACK_WIX_PATCH_FILE
		# 	"${CMAKE_SOURCE_DIR}/installer/client-shortcuts.xml"
		# 	"${CMAKE_SOURCE_DIR}/installer/server-shortcuts.xml"
		# )
	endif()

	set(CPACK_COMPONENTS_ALL mumble_client mumble_server)
endif()

if(WIN32)
	set(CPACK_WIX_UPGRADE_GUID ${CPACK_WIX_PRODUCT_GUID})
	configure_file("${CMAKE_SOURCE_DIR}/installer/wix-template.xml" ${CMAKE_CURRENT_BINARY_DIR})
endif()

include(CPack)
