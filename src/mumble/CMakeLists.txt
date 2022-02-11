# Copyright 2020-2022 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

set(MUMBLE_RC "${CMAKE_CURRENT_BINARY_DIR}/mumble.rc")
set(MUMBLE_DLL_RC "${CMAKE_CURRENT_BINARY_DIR}/mumble_dll.rc")
set(MUMBLE_PLIST "${CMAKE_CURRENT_BINARY_DIR}/mumble.plist")

set(MUMBLE_ICON "${CMAKE_SOURCE_DIR}/icons/mumble.ico")
set(MUMBLE_ICNS "${CMAKE_SOURCE_DIR}/icons/mumble.icns")

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/mumble.rc.in" "${MUMBLE_RC}")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/mumble_dll.rc.in" "${MUMBLE_DLL_RC}")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/mumble.plist.in" "${MUMBLE_PLIST}")

include(qt-utils)
include(install-library)

option(update "Check for updates by default." ON)

option(translations "Include languages other than English." ON)
option(bundle-qt-translations "Bundle Qt's translations as well" ${static}) 

option(bundled-opus "Build the included version of Opus instead of looking for one on the system." ON)
option(bundled-celt "Build the included version of CELT instead of looking for one on the system." ON)
option(bundled-speex "Build the included version of Speex instead of looking for one on the system." ON)
option(rnnoise "Use RNNoise for machine learning noise reduction." ON)
option(bundled-rnnoise "Build the included version of RNNoise instead of looking for one on the system." ${rnnoise})

option(manual-plugin "Include the built-in \"manual\" positional audio plugin." ON)

option(qtspeech "Use Qt's text-to-speech system (part of the Qt Speech module) instead of Mumble's own OS-specific text-to-speech implementations." OFF)

option(jackaudio "Build support for JackAudio." ON)
option(portaudio "Build support for PortAudio" ON)

option(plugin-debug "Build Mumble with debug output for plugin developers." OFF)
option(plugin-callback-debug "Build Mumble with debug output for plugin callbacks inside of Mumble." OFF)

if(WIN32)
	option(asio "Build support for ASIO audio input." OFF)
	option(wasapi "Build support for WASAPI." ON)
	option(xboxinput "Build support for global shortcuts from Xbox controllers via the XInput DLL." ON)
	option(gkey "Build support for Logitech G-Keys. Note: This feature does not require any build-time dependencies, and requires Logitech Gaming Software to be installed to have any effect at runtime." ON)
elseif(UNIX)
	if(APPLE)
		option(coreaudio "Build support for CoreAudio." ON)
	elseif(${CMAKE_SYSTEM_NAME} STREQUAL "FreeBSD")
		option(oss "Build support for OSS." ON)
	elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
		option(alsa "Build support for ALSA." ON)
		option(pipewire "Build support for PipeWire." ON)
		option(pulseaudio "Build support for PulseAudio." ON)
		option(speechd "Build support for Speech Dispatcher." ON)
	endif()

	# scripts/generate_cmake_options_docs.py does not cope with duplicate option() lines,
	# so single out common options into combined conditions.
	# https://github.com/mumble-voip/mumble/issues/5488
	if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux" OR
	   ${CMAKE_SYSTEM_NAME} STREQUAL "OpenBSD")
		option(xinput2 "Build support for XI2." ON)
	endif()
endif()

if(NOT APPLE)
	option(g15 "Include support for the G15 keyboard (and compatible devices)." OFF)
endif()

if(WIN32 OR APPLE)
	option(crash-report "Include support for reporting crashes to the Mumble developers." ON)
endif()

if(MSVC)
	option(elevation "Set \"uiAccess=true\", required for global shortcuts to work with privileged applications. Requires the client's executable to be signed with a trusted code signing certificate." OFF)
endif()

find_pkg(Qt5
	COMPONENTS
		Concurrent
		Sql
		Svg
		Widgets
	REQUIRED
)

set(MUMBLE_SOURCES
	"About.cpp"
	"About.h"
	"ACLEditor.cpp"
	"ACLEditor.h"
	"ACLEditor.ui"
	"API_v_1_0_x.cpp"
	"API.h"
	"ApplicationPalette.h"
	"AudioConfigDialog.cpp"
	"AudioConfigDialog.h"
	"Audio.cpp"
	"Audio.h"
	"AudioInput.cpp"
	"AudioInput.h"
	"AudioInput.ui"
	"AudioOutput.cpp"
	"AudioOutput.h"
	"AudioOutputSample.cpp"
	"AudioOutputSample.h"
	"AudioOutputSpeech.cpp"
	"AudioOutputSpeech.h"
	"AudioOutput.ui"
	"AudioOutputUser.cpp"
	"AudioOutputUser.h"
	"AudioStats.cpp"
	"AudioStats.h"
	"AudioStats.ui"
	"AudioWizard.cpp"
	"AudioWizard.h"
	"AudioWizard.ui"
	"BanEditor.cpp"
	"BanEditor.h"
	"BanEditor.ui"
	"CELTCodec.cpp"
	"CELTCodec.h"
	"Cert.cpp"
	"Cert.h"
	"Cert.ui"
	"ClientUser.cpp"
	"ClientUser.h"
	"ConfigDialog.cpp"
	"ConfigDialog.h"
	"ConfigDialog.ui"
	"ConfigWidget.cpp"
	"ConfigWidget.h"
	"ConnectDialog.cpp"
	"ConnectDialogEdit.ui"
	"ConnectDialog.h"
	"ConnectDialog.ui"
	"CustomElements.cpp"
	"CustomElements.h"
	"Database.cpp"
	"Database.h"
	"DeveloperConsole.cpp"
	"DeveloperConsole.h"
	"EchoCancelOption.cpp"
	"EchoCancelOption.h"
	"EnumStringConversions.cpp"
	"EnumStringConversions.h"
	"Global.cpp"
	"Global.h"
	"GlobalShortcut.cpp"
	"GlobalShortcut.h"
	"GlobalShortcut.ui"
	"GlobalShortcutButtons.cpp"
	"GlobalShortcutButtons.h"
	"GlobalShortcutButtons.ui"
	"GlobalShortcutTarget.ui"
	"JSONSerialization.cpp"
	"JSONSerialization.h"
	"LCD.cpp"
	"LCD.h"
	"LCD.ui"
	"LegacyPlugin.cpp"
	"LegacyPlugin.h"
	"ListenerLocalVolumeDialog.cpp"
	"Log.cpp"
	"Log.h"
	"Log.ui"
	"LookConfig.cpp"
	"LookConfig.h"
	"LookConfig.ui"
	"MainWindow.cpp"
	"MainWindow.h"
	"MainWindow.ui"
	"Markdown.cpp"
	"Markdown.h"
	"Messages.cpp"
	"MumbleApplication.cpp"
	"MumbleApplication.h"
	"NetworkConfig.cpp"
	"NetworkConfig.h"
	"NetworkConfig.ui"
	"OpusCodec.cpp"
	"OpusCodec.h"
	"PluginConfig.cpp"
	"PluginConfig.h"
	"PluginConfig.ui"
	"Plugin.cpp"
	"Plugin.h"
	"PluginInstaller.cpp"
	"PluginInstaller.h"
	"PluginInstaller.ui"
	"PluginManager.cpp"
	"PluginManager.h"
	"PluginManifest.cpp"
	"PluginManifest.h"
	"PluginUpdater.cpp"
	"PluginUpdater.h"
	"PluginUpdater.ui"
	"PositionalAudioViewer.cpp"
	"PositionalAudioViewer.h"
	"PositionalAudioViewer.ui"
	"PositionalData.cpp"
	"PositionalData.h"
	"PTTButtonWidget.cpp"
	"PTTButtonWidget.h"
	"PTTButtonWidget.ui"
	"QtWidgetUtils.cpp"
	"QtWidgetUtils.h"
	"RichTextEditor.cpp"
	"RichTextEditor.h"
	"RichTextEditorLink.ui"
	"RichTextEditor.ui"
	"Screen.cpp"
	"Screen.h"
	"SearchDialog.cpp"
	"SearchDialog.h"
	"SearchDialog.ui"
	"ServerHandler.cpp"
	"ServerHandler.h"
	"ServerInformation.cpp"
	"ServerInformation.h"
	"ServerInformation.ui"
	"SettingsKeys.cpp"
	"SettingsKeys.h"
	"Settings.cpp"
	"Settings.h"
	"SharedMemory.cpp"
	"SharedMemory.h"
	"SocketRPC.cpp"
	"SocketRPC.h"
	"SvgIcon.cpp"
	"SvgIcon.h"
	"TalkingUI.cpp"
	"TalkingUI.h"
	"TalkingUIContainer.cpp"
	"TalkingUIContainer.h"
	"TalkingUIEntry.cpp"
	"TalkingUIEntry.h"
	"TalkingUISelection.cpp"
	"TalkingUISelection.h"
	"TextMessage.cpp"
	"TextMessage.h"
	"TextMessage.ui"
	"TextToSpeech.h"
	"ThemeInfo.cpp"
	"ThemeInfo.h"
	"Themes.cpp"
	"Themes.h"
	"Tokens.cpp"
	"Tokens.h"
	"Tokens.ui"
	"Translations.cpp"
	"Translations.h"
	"Usage.cpp"
	"Usage.h"
	"UserEdit.cpp"
	"UserEdit.h"
	"UserEdit.ui"
	"UserInformation.cpp"
	"UserInformation.h"
	"UserInformation.ui"
	"UserListModel.cpp"
	"UserListModel.h"
	"UserLocalNicknameDialog.cpp"
	"UserLocalNicknameDialog.h"
	"UserLocalNicknameDialog.ui"
	"UserLocalVolumeDialog.cpp"
	"UserLocalVolumeDialog.h"
	"UserLocalVolumeDialog.ui"
	"UserModel.cpp"
	"UserModel.h"
	"UserView.cpp"
	"UserView.h"
	"VersionCheck.cpp"
	"VersionCheck.h"
	"ViewCert.cpp"
	"ViewCert.h"
	"VoiceRecorder.cpp"
	"VoiceRecorderDialog.cpp"
	"VoiceRecorderDialog.h"
	"VoiceRecorderDialog.ui"
	"VoiceRecorder.h"
	"WebFetch.cpp"
	"WebFetch.h"
	"XMLTools.cpp"
	"XMLTools.h"

	"widgets/CompletablePage.cpp"
	"widgets/CompletablePage.h"
	"widgets/MUComboBox.cpp"
	"widgets/MUComboBox.h"
	"widgets/MultiStyleWidgetWrapper.cpp"
	"widgets/MultiStyleWidgetWrapper.h"
	"widgets/RichTextItemDelegate.cpp"
	"widgets/RichTextItemDelegate.h"
	"widgets/SearchDialogItemDelegate.cpp"
	"widgets/SearchDialogItemDelegate.h"
	"widgets/SearchDialogTree.cpp"
	"widgets/SearchDialogTree.h"


	"${SHARED_SOURCE_DIR}/ACL.cpp"
	"${SHARED_SOURCE_DIR}/ACL.h"
	"${SHARED_SOURCE_DIR}/Channel.cpp"
	"${SHARED_SOURCE_DIR}/Channel.h"
	"${SHARED_SOURCE_DIR}/ChannelListenerManager.cpp"
	"${SHARED_SOURCE_DIR}/ChannelListenerManager.h"
	"${SHARED_SOURCE_DIR}/Connection.cpp"
	"${SHARED_SOURCE_DIR}/Connection.h"
	"${SHARED_SOURCE_DIR}/Group.cpp"
	"${SHARED_SOURCE_DIR}/Group.h"
	"${SHARED_SOURCE_DIR}/SignalCurry.h"
	"${SHARED_SOURCE_DIR}/User.cpp"
	"${SHARED_SOURCE_DIR}/User.h"

	"${3RDPARTY_DIR}/smallft/smallft.cpp"

	"mumble.qrc"
	"flags/mumble_flags_0.qrc"
	"${CMAKE_SOURCE_DIR}/themes/DefaultTheme.qrc"
)

add_library(mumble_client_object_lib OBJECT ${MUMBLE_SOURCES})

if(static AND WIN32)
	# On Windows, building a static client means building the main app into a DLL.
	add_library(mumble SHARED "main.cpp")
	target_compile_definitions(mumble_client_object_lib PUBLIC "MUMBLEAPP_DLL")

	set_target_properties(mumble PROPERTIES OUTPUT_NAME "mumble_app")
	if(MINGW)
		# Remove "lib" prefix.
		set_target_properties(mumble PROPERTIES PREFIX "")
	endif()

	target_sources(mumble_client_object_lib PRIVATE "${MUMBLE_DLL_RC}")

	add_subdirectory("${SHARED_SOURCE_DIR}/mumble_exe" "${CMAKE_BINARY_DIR}/mumble_exe")
else()
	add_executable(mumble "main.cpp")

	if(WIN32)
		target_sources(mumble_client_object_lib
			PRIVATE
				"mumble.appcompat.manifest"
				"${MUMBLE_RC}"
		)

		if(elevation)
			set_property(TARGET mumble APPEND_STRING PROPERTY LINK_FLAGS " /MANIFESTUAC:\"level=\'asInvoker\' uiAccess=\'true\'\"")
		endif()
	elseif(APPLE)
		set_target_properties(mumble
			PROPERTIES
				OUTPUT_NAME "Mumble"
				MACOSX_BUNDLE TRUE
				RESOURCE ${MUMBLE_ICNS}
				MACOSX_BUNDLE_INFO_PLIST ${MUMBLE_PLIST}
		)
	endif()
endif()

set_property(TARGET mumble_client_object_lib PROPERTY INTERPROCEDURAL_OPTIMIZATION ${lto})
set_property(TARGET mumble PROPERTY INTERPROCEDURAL_OPTIMIZATION ${lto})

target_link_libraries(mumble mumble_client_object_lib)

target_compile_definitions(mumble_client_object_lib
	PUBLIC
		"MUMBLE_LIBRARY_PATH=${MUMBLE_INSTALL_LIBDIR}"
		"MUMBLE_PLUGIN_PATH=${MUMBLE_INSTALL_PLUGINDIR}"
)

set_target_properties(mumble PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set_target_properties(mumble_client_object_lib
	PROPERTIES
		AUTOMOC ON
		AUTORCC ON
		AUTOUIC ON
)

if(WIN32)
	install(TARGETS mumble RUNTIME DESTINATION "${MUMBLE_INSTALL_EXECUTABLEDIR}" COMPONENT mumble_client)
else()
	if(NOT APPLE)
		install(TARGETS mumble RUNTIME DESTINATION "${MUMBLE_INSTALL_EXECUTABLEDIR}" COMPONENT mumble_client)
	else()
		install(TARGETS mumble BUNDLE DESTINATION "${MUMBLE_INSTALL_EXECUTABLEDIR}" COMPONENT mumble_client)
	endif()

	# Install Mumble man files
	install(FILES "${CMAKE_SOURCE_DIR}/man/mumble.1" DESTINATION "${MUMBLE_INSTALL_MANDIR}" COMPONENT doc)
endif()

target_compile_definitions(mumble_client_object_lib
	PUBLIC
		"MUMBLE"
		"QT_RESTRICTED_CAST_FROM_ASCII"
)

target_include_directories(mumble_client_object_lib
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR} # This is required for includes in current folder to be found by files from the shared directory.
		"widgets"
		${SHARED_SOURCE_DIR}
		"${3RDPARTY_DIR}/smallft"
		"${PLUGINS_DIR}"
)

find_pkg(Poco COMPONENTS Zip)

if(TARGET Poco::Zip)
	target_link_libraries(mumble_client_object_lib
		PUBLIC
			Poco::Zip
	)
else()
	message(STATUS "Regular Poco search failed - looking for Poco include dir manually...")

	if(MINGW)
		# These are the paths for our MXE environment
		if(32_BIT)
			set(POCO_INCLUDE_DIR_HINT "/usr/lib/mxe/usr/i686-w64-mingw32.static/include/")
		else()
			set(POCO_INCLUDE_DIR_HINT "/usr/lib/mxe/usr/x86_64-w64-mingw32.static/include/")
		endif()
	else()
		set(POCO_INCLUDE_DIR_HINT "/usr/include")
	endif()

	find_path(POCO_INCLUDE_DIR "Poco/Poco.h" HINTS ${POCO_INCLUDE_DIR_HINT})

	if(POCO_INCLUDE_DIR)
		message(STATUS "Found Poco include dir at \"${POCO_INCLUDE_DIR}\"")
	else()
		message(FATAL_ERROR "Unable to locate Poco include directory")
	endif()

	find_library(POCO_LIB_FOUNDATION NAMES PocoFoundation PocoFoundationmd REQUIRED)
	find_library(POCO_LIB_UTIL NAMES PocoUtil PocoUtilmd REQUIRED)
	find_library(POCO_LIB_XML NAMES PocoXML PocoXMLmd REQUIRED)
	find_library(POCO_LIB_ZIP NAMES PocoZip PocoZipmd REQUIRED)

	if(POCO_LIB_ZIP)
		message(STATUS "Found Poco Zip library at \"${POCO_LIB_ZIP}\"")
	else()
		message(FATAL_ERROR "Unable to find Poco Zip library")
	endif()


	# Now use the found include dir and libraries by linking it to the target
	target_include_directories(mumble_client_object_lib
		PUBLIC
			${POCO_INCLUDE_DIR}
	)

	target_link_libraries(mumble_client_object_lib
		PUBLIC
			${POCO_LIB_ZIP}
			${POCO_LIB_XML}
			${POCO_LIB_UTIL}
			${POCO_LIB_FOUNDATION}
	)

	if(static)
		target_compile_definitions(mumble_client_object_lib
			PUBLIC
				POCO_STATIC
		)
	endif()
endif()


set(JSON_BuildTests OFF CACHE INTERNAL "")
set(JSON_ImplicitConversions OFF CACHE INTERNAL "")
add_subdirectory("${3RDPARTY_DIR}/nlohmann_json/" "${CMAKE_CURRENT_BINARY_DIR}/nlohmann_json/")

target_link_libraries(mumble_client_object_lib PUBLIC nlohmann_json::nlohmann_json)

find_pkg("SndFile;LibSndFile;sndfile" REQUIRED)

# Check if sndfile version supports opus
if("${sndfile_VERSION}" VERSION_GREATER_EQUAL "1.0.29")
	target_compile_definitions(mumble_client_object_lib PUBLIC USE_SNDFILE_OPUS)
else()
	message(WARNING "libsnfile is missing Opus-support -> No Opus-format recording")
endif()

# Look for various targets as they are named differently on different platforms
if(static AND TARGET sndfile-static)
	target_link_libraries(mumble_client_object_lib PUBLIC sndfile-static)
elseif(TARGET SndFile::sndfile)
	target_link_libraries(mumble_client_object_lib PUBLIC SndFile::sndfile)
elseif(TARGET sndfile)
	target_link_libraries(mumble_client_object_lib PUBLIC sndfile)
else()
	target_link_libraries(mumble_client_object_lib PUBLIC ${sndfile_LIBRARIES})
endif()

target_link_libraries(mumble_client_object_lib
	PUBLIC
		shared
		Qt5::Concurrent
		Qt5::Sql
		Qt5::Svg
		Qt5::Widgets
)

if(static)
	if(TARGET Qt5::QSQLiteDriverPlugin)
		include_qt_plugin(mumble_client_object_lib PRIVATE "QSQLiteDriverPlugin")
		target_link_libraries(mumble_client_object_lib PUBLIC Qt5::QSQLiteDriverPlugin)
	endif()
	if(TARGET Qt5::QSvgIconPlugin)
		include_qt_plugin(mumble_client_object_lib PRIVATE "QSvgIconPlugin")
		target_link_libraries(mumble_client_object_lib PUBLIC Qt5::QSvgIconPlugin)
	endif()
	if(TARGET Qt5::QSvgPlugin)
		include_qt_plugin(mumble_client_object_lib PRIVATE "QSvgPlugin")
		target_link_libraries(mumble_client_object_lib PUBLIC Qt5::QSvgPlugin)
	endif()
endif()

if(MSVC)
	target_compile_definitions(mumble_client_object_lib PUBLIC "RESTRICT=")
else()
	target_compile_definitions(mumble_client_object_lib PUBLIC "RESTRICT=__restrict__")
endif()

if(WIN32)
	target_sources(mumble_client_object_lib PRIVATE
		"GlobalShortcut_win.cpp"
		"GlobalShortcut_win.h"
		"Log_win.cpp"
		"SharedMemory_win.cpp"
		"TaskList.cpp"
		"UserLockFile_win.cpp"
		"WinGUIDs.cpp"
		"os_early_win.cpp"
		"os_win.cpp"

		"${CMAKE_SOURCE_DIR}/overlay/ods.cpp"
	)

	# WinGUIDs.cpp includes initguid.h which causes a macro definition in guiddef.h to be
	# changed in such a way that subsequent usages break (redefinition errors).
	# Thus we must not package this source file in a unity build allowing other files
	# to include plain guiddef.h without having to worry about side-effects.
	set_source_files_properties("WinGUIDs.cpp" PROPERTIES SKIP_UNITY_BUILD_INCLUSION TRUE)

	find_pkg(Boost
		COMPONENTS
			system
			thread
		REQUIRED
	)

	if(static AND TARGET Qt5::QWindowsIntegrationPlugin)
		include_qt_plugin(mumble_client_object_lib PRIVATE QWindowsIntegrationPlugin)
		target_link_libraries(mumble_client_object_lib PUBLIC Qt5::QWindowsIntegrationPlugin)
	endif()

	add_subdirectory("${3RDPARTY_DIR}/xinputcheck-build" "${CMAKE_CURRENT_BINARY_DIR}/xinputcheck")

	# Disable all warnings that the xinputcheck code may emit
	disable_warnings_for_all_targets_in("${3RDPARTY_DIR}/xinputcheck-build")

	target_link_libraries(mumble_client_object_lib PUBLIC xinputcheck)

	if(MSVC)
		target_link_libraries(mumble_client_object_lib
			PUBLIC
				Boost::dynamic_linking
				Delayimp.lib
			)
		set_property(TARGET mumble_client_object_lib APPEND_STRING PROPERTY LINK_FLAGS " /DELAYLOAD:user32.dll /DELAYLOAD:qwave.dll")
	endif()

	target_link_libraries(mumble_client_object_lib
		PUBLIC
			Boost::system
			Boost::thread
	)

	target_link_libraries(mumble_client_object_lib
		PUBLIC
			dbghelp.lib
			dxguid.lib
			hid.lib
			wintrust.lib
	)
else()
	target_sources(mumble_client_object_lib PRIVATE "SharedMemory_unix.cpp")

	if(NOT APPLE)
		find_pkg(X11 COMPONENTS Xext REQUIRED)

		if(xinput2)
			find_pkg(X11 COMPONENTS Xi REQUIRED)
			target_link_libraries(mumble_client_object_lib PUBLIC X11::Xi)
		else()
			target_compile_definitions(mumble_client_object_lib PUBLIC "NO_XINPUT2")
		endif()

		if(static AND TARGET Qt5::QXcbIntegrationPlugin)
			include_qt_plugin(mumble_client_object_lib PRIVATE QXcbIntegrationPlugin)
			target_link_libraries(mumble_client_object_lib PUBLIC Qt5::QXcbIntegrationPlugin)
		endif()

		target_sources(mumble_client_object_lib
			PRIVATE
				"GlobalShortcut_unix.cpp"
				"GlobalShortcut_unix.h"
				"Log_unix.cpp"
				"os_unix.cpp"
		)

		if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
			find_library(LIB_RT rt)
			target_link_libraries(mumble_client_object_lib PUBLIC ${LIB_RT})
		endif()

		target_link_libraries(mumble_client_object_lib PUBLIC X11::Xext)
	else()
		find_library(LIB_APPKIT "AppKit")
		find_library(LIB_APPLICATIONSERVICES "ApplicationServices")
		find_library(LIB_CARBON "Carbon")
		find_library(LIB_SCRIPTINGBRIDGE "ScriptingBridge")
		find_library(LIB_SECURITY "Security")
		find_library(LIB_XAR "xar")

		target_sources(mumble_client_object_lib
			PRIVATE
				"AppNap.h"
				"AppNap.mm"
				"GlobalShortcut_macx.h"
				"GlobalShortcut_macx.mm"
				"Log_macx.mm"
				"os_macx.mm"
		)

		if(static AND TARGET Qt5::QCocoaIntegrationPlugin)
			include_qt_plugin(mumble_client_object_lib PRIVATE QCocoaIntegrationPlugin)
			target_link_libraries(mumble_client_object_lib PUBLIC Qt5::QCocoaIntegrationPlugin)
		endif()

		target_link_libraries(mumble_client_object_lib
			PUBLIC
				${LIB_APPKIT}
				${LIB_APPLICATIONSERVICES}
				${LIB_CARBON}
				${LIB_SCRIPTINGBRIDGE}
				${LIB_SECURITY}
				${LIB_XAR}
		)
	endif()
endif()

if(bundled-opus)
	option(OPUS_BUILD_SHARED_LIBRARY "" ON)
	if(MINGW)
		option(OPUS_STACK_PROTECTOR "" OFF)
	endif()

	add_subdirectory("${3RDPARTY_DIR}/opus" "${CMAKE_CURRENT_BINARY_DIR}/opus")

	# Disable all warnings that the Opus code may emit
	disable_warnings_for_all_targets_in("${3RDPARTY_DIR}/opus")

	# Opus doesn't work in unity builds
	set_target_properties(opus PROPERTIES UNITY_BUILD FALSE)

	add_dependencies(mumble_client_object_lib opus)

	target_include_directories(mumble_client_object_lib PUBLIC "${3RDPARTY_DIR}/opus/include")

	if(WIN32)
		# Shared library on Windows (e.g. ".dll")
		set_target_properties(opus PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
	else()
		# Shared library on UNIX (e.g. ".so")
		set_target_properties(opus PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
	endif()

	install_library(opus mumble_client)
else()
	find_pkg(opus REQUIRED)
	target_include_directories(mumble_client_object_lib PUBLIC ${opus_INCLUDE_DIRS})
endif()

if(bundled-celt)
	add_subdirectory("${3RDPARTY_DIR}/celt-0.7.0-build" "${CMAKE_CURRENT_BINARY_DIR}/celt")

	# Disable all warnings that the Celt code may emit
	disable_warnings_for_all_targets_in("${3RDPARTY_DIR}/celt-0.7.0-build")

	add_dependencies(mumble_client_object_lib celt)

	target_include_directories(mumble_client_object_lib PUBLIC "${3RDPARTY_DIR}/celt-0.7.0-src/libcelt")

	if(WIN32)
		# Shared library on Windows (e.g. ".dll")
		set_target_properties(celt PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
	else()
		# Shared library on UNIX (e.g. ".so")
		set_target_properties(celt PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
	endif()

	install_library(celt mumble_client)
else()
	find_pkg(celt REQUIRED)
	if(${celt_VERSION} VERSION_LESS          0.7 OR
	   ${celt_VERSION} VERSION_GREATER_EQUAL 0.8)
		message(FATAL_ERROR "CELT 0.7.x is required, found ${celt_VERSION}!")
	endif()
	target_include_directories(mumble_client_object_lib PUBLIC ${celt_INCLUDE_DIRS})
endif()

if(bundled-speex)
	add_subdirectory("${3RDPARTY_DIR}/speex-build" "${CMAKE_CURRENT_BINARY_DIR}/speex")

	# Disable all warnings that the speex code may emit
	disable_warnings_for_all_targets_in("${3RDPARTY_DIR}/speex-build")

	target_link_libraries(mumble_client_object_lib PUBLIC speex)

	if(WIN32)
		# Shared library on Windows (e.g. ".dll")
		set_target_properties(speex PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
	else()
		# Shared library on UNIX (e.g. ".so")
		set_target_properties(speex PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
	endif()

	install_library(speex mumble_client)
else()
	find_pkg(speex REQUIRED)
	find_pkg(speexdsp REQUIRED)

	target_link_libraries(mumble_client_object_lib
		PUBLIC
			${speex_LIBRARIES}
			${speexdsp_LIBRARIES}
	)
endif()

if(rnnoise)
	target_compile_definitions(mumble_client_object_lib PRIVATE "USE_RNNOISE")

	if(bundled-rnnoise)
		add_subdirectory("${3RDPARTY_DIR}/rnnoise-build" "${CMAKE_CURRENT_BINARY_DIR}/rnnoise")

		# Disable all warnings that the RNNoise code may emit
		disable_warnings_for_all_targets_in("${3RDPARTY_DIR}/rnnoise-build")

		target_link_libraries(mumble_client_object_lib PRIVATE rnnoise)

		if(WIN32)
			# Shared library on Windows (e.g. ".dll")
			set_target_properties(rnnoise PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
		else()
			# Shared library on UNIX (e.g. ".so")
			set_target_properties(rnnoise PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
		endif()

		install_library(rnnoise mumble_client)
	else()
		find_pkg(rnnoise REQUIRED)

		target_link_libraries(mumble_client_object_lib PRIVATE ${rnnoise_LIBRARIES})
	endif()
endif()

if(qtspeech)
	find_pkg(Qt5 COMPONENTS TextToSpeech REQUIRED)
	target_sources(mumble_client_object_lib PRIVATE "TextToSpeech.cpp")
	target_link_libraries(mumble_client_object_lib PUBLIC Qt5::TextToSpeech)
elseif(WIN32)
	target_sources(mumble_client_object_lib PRIVATE "TextToSpeech_win.cpp")
	if(MINGW)
		target_link_libraries(mumble_client_object_lib PUBLIC sapi.lib)
	endif()
elseif(APPLE)
	target_sources(mumble_client_object_lib PRIVATE "TextToSpeech_macx.mm")
else()
	target_sources(mumble_client_object_lib PRIVATE "TextToSpeech_unix.cpp")

	if(speechd)
		find_pkg("speech-dispatcher" REQUIRED)

		target_compile_definitions(mumble_client_object_lib
			PUBLIC
				"USE_SPEECHD"
				"USE_SPEECHD_PKGCONFIG"
		)
		target_link_libraries(mumble_client_object_lib PUBLIC ${speech-dispatcher_LIBRARIES})
	else()
		target_compile_definitions(mumble_client_object_lib PUBLIC "USE_NO_TTS")
	endif()
endif()

if(crash-report)
	target_sources(mumble_client_object_lib
		PRIVATE
			"CrashReporter.cpp"
			"CrashReporter.h"
	)
else()
	target_compile_definitions(mumble_client_object_lib PUBLIC "NO_CRASH_REPORT")
endif()

if(manual-plugin)
	target_sources(mumble_client_object_lib
		PRIVATE
			"ManualPlugin.cpp"
			"ManualPlugin.h"
			"ManualPlugin.ui"
	)

target_compile_definitions(mumble_client_object_lib PUBLIC "USE_MANUAL_PLUGIN")
endif()

if(NOT update)
	target_compile_definitions(mumble_client_object_lib PUBLIC "NO_UPDATE_CHECK")
endif()

if(dbus AND NOT WIN32 AND NOT APPLE)
	find_pkg(Qt5 COMPONENTS DBus REQUIRED)

	target_sources(mumble_client_object_lib
		PRIVATE
			"DBus.cpp"
			"DBus.h"
	)

	target_compile_definitions(mumble_client_object_lib PUBLIC "USE_DBUS")
	target_link_libraries(mumble_client_object_lib PUBLIC Qt5::DBus)
endif()

if(translations)
	# Glob for all translation files in the source dir
	file(GLOB TS_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.ts")

	include_translations(QRC_FILE ${CMAKE_CURRENT_BINARY_DIR} "${TS_FILES}")
	target_sources(mumble_client_object_lib PRIVATE "${QRC_FILE}")

	if(bundle-qt-translations)
		bundle_qt_translations(mumble_client_object_lib)
	endif()
elseif(bundle-qt-translations)
	message(WARNING "Can't bundle Qt translations if translations are disabled!")
endif()

if(overlay)
	target_sources(mumble_client_object_lib
		PRIVATE
			"Overlay.cpp"
			"Overlay.h"
			"Overlay.ui"
			"OverlayClient.cpp"
			"OverlayClient.h"
			"OverlayConfig.cpp"
			"OverlayConfig.h"
			"OverlayEditor.cpp"
			"OverlayEditor.h"
			"OverlayEditor.ui"
			"OverlayEditorScene.cpp"
			"OverlayEditorScene.h"
			"OverlayPositionableItem.cpp"
			"OverlayPositionableItem.h"
			"OverlayText.cpp"
			"OverlayText.h"
			"OverlayUser.cpp"
			"OverlayUser.h"
			"OverlayUserGroup.cpp"
			"OverlayUserGroup.h"
			"PathListWidget.cpp"
			"PathListWidget.h"
	)

	if(WIN32)
		target_sources(mumble_client_object_lib
			PRIVATE
				"Overlay_win.cpp"
				"Overlay_win.h"
		)
	else()
		if(APPLE)
			target_sources(mumble_client_object_lib PRIVATE "Overlay_macx.mm")
		else()
			target_sources(mumble_client_object_lib PRIVATE "Overlay_unix.cpp")
		endif()
	endif()

	target_compile_definitions(mumble_client_object_lib PUBLIC "USE_OVERLAY")
endif()

if(xboxinput)
	target_sources(mumble_client_object_lib
		PRIVATE
			"XboxInput.cpp"
			"XboxInput.h"
	)

	target_compile_definitions(mumble_client_object_lib PUBLIC "USE_XBOXINPUT")
endif()

if(gkey)
	target_sources(mumble_client_object_lib
		PRIVATE
			"GKey.cpp"
			"GKey.h"
	)

	target_compile_definitions(mumble_client_object_lib PUBLIC "USE_GKEY")
endif()

if(g15)
	if(WIN32 OR APPLE)
		target_sources(mumble_client_object_lib
			PRIVATE
				"G15LCDEngine_helper.cpp"
				"G15LCDEngine_helper.h"
	)
	else()
		find_library(LIB_G15DAEMON_CLIENT "g15daemon_client")
		if(LIB_G15DAEMON_CLIENT-NOTFOUND)
			message(FATAL_ERROR "G15 library not found!")
		endif()

		target_sources(mumble_client_object_lib
			PRIVATE
				"G15LCDEngine_unix.cpp"
				"G15LCDEngine_unix.h"
		)

		target_compile_definitions(mumble_client_object_lib PUBLIC "USE_G15")
		target_link_libraries(mumble_client_object_lib PUBLIC ${LIB_G15DAEMON_CLIENT})
	endif()
endif()

if(zeroconf)
	if(NOT APPLE)
		find_pkg(avahi-compat-libdns_sd QUIET)
		if(avahi-compat-libdns_sd_FOUND)
			target_include_directories(mumble_client_object_lib PUBLIC ${avahi-compat-libdns_sd_INCLUDE_DIRS})
			target_link_libraries(mumble_client_object_lib PUBLIC ${avahi-compat-libdns_sd_LIBRARIES})
		else()
			find_library(LIB_DNSSD "dnssd")
			if(${LIB_DNSSD} STREQUAL "LIB_DNSSD-NOTFOUND")
				message(FATAL_ERROR "DNS-SD library not found!")
			endif()
			target_link_libraries(mumble_client_object_lib PUBLIC ${LIB_DNSSD})
		endif()
	endif()

	target_sources(mumble_client_object_lib
		PRIVATE
			"Zeroconf.cpp"
			"Zeroconf.h"
			# Unlike what the name implies, this 3rdparty helper is not actually related to Bonjour.
			# It just uses the API provided by mDNSResponder, making it compatible with Avahi too.
			"${3RDPARTY_DIR}/qqbonjour/BonjourRecord.h"
			"${3RDPARTY_DIR}/qqbonjour/BonjourServiceBrowser.cpp"
			"${3RDPARTY_DIR}/qqbonjour/BonjourServiceBrowser.h"
			"${3RDPARTY_DIR}/qqbonjour/BonjourServiceResolver.cpp"
			"${3RDPARTY_DIR}/qqbonjour/BonjourServiceResolver.h"
	)

	target_compile_definitions(mumble_client_object_lib PUBLIC "USE_ZEROCONF")
	target_include_directories(mumble_client_object_lib PUBLIC "${3RDPARTY_DIR}/qqbonjour")
endif()

if(alsa)
	find_pkg(ALSA REQUIRED)
	target_sources(mumble_client_object_lib
		PRIVATE
			"ALSAAudio.cpp"
			"ALSAAudio.h"
	)

	target_compile_definitions(mumble_client_object_lib PUBLIC "USE_ALSA")
	target_link_libraries(mumble_client_object_lib PUBLIC ALSA::ALSA)
endif()

if(asio)
	if(NOT ASIO_DIR)
		set(ASIO_DIR "${3RDPARTY_DIR}/asio")
	endif()

	target_sources(mumble_client_object_lib
		PRIVATE
			"ASIOInput.cpp"
			"ASIOInput.h"
			"ASIOInput.ui"
	)

	target_compile_definitions(mumble_client_object_lib PUBLIC "USE_ASIO")
	target_include_directories(mumble_client_object_lib
		SYSTEM PUBLIC
			"${ASIO_DIR}/common"
			"${ASIO_DIR}/host"
			"${ASIO_DIR}/host/pc"
	)
endif()

if(coreaudio)
	find_library(LIB_AUDIOUNIT "AudioUnit")
	find_library(LIB_COREAUDIO "CoreAudio")

	target_sources(mumble_client_object_lib
		PRIVATE
			"CoreAudio.mm"
			"CoreAudio.h"
	)

	target_link_libraries(mumble_client_object_lib
		PUBLIC
			${LIB_AUDIOUNIT}
			${LIB_COREAUDIO}
			"-framework AVFoundation"
	)
endif()

if(jackaudio)
	target_sources(mumble_client_object_lib
		PRIVATE
			"JackAudio.cpp"
			"JackAudio.h"
	)

	target_compile_definitions(mumble_client_object_lib PUBLIC "USE_JACKAUDIO")
	target_include_directories(mumble_client_object_lib SYSTEM PUBLIC "${3RDPARTY_DIR}/jack")
endif()

if(oss)
	target_sources(mumble_client_object_lib
		PRIVATE
			"OSS.cpp"
			"OSS.h"
	)

	target_compile_definitions(mumble_client_object_lib PUBLIC "USE_OSS")
	target_include_directories(mumble_client_object_lib SYSTEM PUBLIC "/usr/lib/oss/include")
endif()

if(pipewire)
	target_sources(mumble_client_object_lib
		PRIVATE
			"PipeWire.cpp"
			"PipeWire.h"
	)

	target_compile_definitions(mumble_client_object_lib PUBLIC "USE_PIPEWIRE")
	target_include_directories(mumble_client_object_lib SYSTEM PUBLIC "${3RDPARTY_DIR}/pipewire")
endif()

if(portaudio)
	target_sources(mumble_client_object_lib
		PRIVATE
			"PAAudio.cpp"
			"PAAudio.h"
	)

	target_compile_definitions(mumble_client_object_lib PUBLIC "USE_PORTAUDIO")
	target_include_directories(mumble_client_object_lib SYSTEM PUBLIC "${3RDPARTY_DIR}/portaudio")
endif()

if(pulseaudio)
	target_sources(mumble_client_object_lib
		PRIVATE
			"PulseAudio.cpp"
			"PulseAudio.h"
	)

	target_compile_definitions(mumble_client_object_lib PUBLIC "USE_PULSEAUDIO")
	target_include_directories(mumble_client_object_lib SYSTEM PUBLIC "${3RDPARTY_DIR}/pulseaudio")
endif()

if(wasapi)
	target_sources(mumble_client_object_lib
		PRIVATE
			"WASAPI.cpp"
			"WASAPI.h"
			"WASAPINotificationClient.cpp"
			"WASAPINotificationClient.h"
	)

	target_compile_definitions(mumble_client_object_lib PUBLIC "USE_WASAPI")
	target_link_libraries(mumble_client_object_lib PUBLIC avrt.lib)
	if(MINGW)
		target_link_libraries(mumble_client_object_lib PUBLIC ksuser.lib)
	endif()

	if(MSVC)
		set_property(TARGET mumble_client_object_lib APPEND_STRING PROPERTY LINK_FLAGS " /DELAYLOAD:avrt.dll")
	endif()
endif()

if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.16.0")
	if (APPLE)
		# Prevent objective C files from being included in unity builds as that causes issues
		set_source_files_properties(
			"AppNap.mm"
			"GlobalShortcut_macx.mm"
			"Log_macx.mm"
			"os_macx.mm"
			"TextToSpeech_macx.mm"
			"Overlay_macx.mm"
			"CoreAudio.mm"
			PROPERTIES
				SKIP_UNITY_BUILD_INCLUSION TRUE
		)
	elseif(UNIX)
		# Exclude source files that include the X11 headers as these define
		# an awful lot of macros that can conflict with other code
		set_source_files_properties(
			"GlobalShortcut_unix.cpp"
			PROPERTIES
				SKIP_UNITY_BUILD_INCLUSION TRUE
		)
	endif()
endif()

if(plugin-debug)
	target_compile_definitions(mumble_client_object_lib PUBLIC "MUMBLE_PLUGIN_DEBUG")
endif()

if(plugin-callback-debug)
	target_compile_definitions(mumble_client_object_lib PUBLIC "MUMBLE_PLUGIN_CALLBACK_DEBUG")
endif()

if(UNIX)
	if(${CMAKE_SYSTEM_NAME} STREQUAL "FreeBSD")
		# On FreeBSD we need the util library for src/ProcessResolver.cpp to work
		target_link_libraries(mumble_client_object_lib PUBLIC util)
	elseif(${CMAKE_SYSTEM_NAME} MATCHES ".*BSD")
		# On any other BSD we need the kvm library for src/ProcessResolver.cpp to work
		target_link_libraries(mumble_client_object_lib PUBLIC kvm)
	endif()
endif()


# Workaround for a cmake bug that causes the auto-generated UI files (AutoUIC) to not be added
# to the target's interface include directories. This can cause missing include errors in targets
# linking to mumble_client_object_lib.
# cmake bug report: https://gitlab.kitware.com/cmake/cmake/-/issues/17456
set(TARGET_NAME "mumble_client_object_lib")
get_property(_isMultiConfig GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(_isMultiConfig)
  set(AUTOGEN_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}_autogen/include_$<CONFIG>)
else()
  set(AUTOGEN_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}_autogen/include)
endif()

target_include_directories(${TARGET_NAME} INTERFACE
  $<BUILD_INTERFACE:${AUTOGEN_INCLUDE_DIR}>
)
