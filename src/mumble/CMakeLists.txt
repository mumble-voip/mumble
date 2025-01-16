# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

set(MUMBLE_RC "${CMAKE_CURRENT_BINARY_DIR}/mumble.rc")
set(MUMBLE_PLIST "${CMAKE_CURRENT_BINARY_DIR}/mumble.plist")

set(MUMBLE_ICON "${CMAKE_SOURCE_DIR}/icons/mumble.ico")
set(MUMBLE_ICNS "${CMAKE_SOURCE_DIR}/icons/mumble.icns")

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/mumble.rc.in" "${MUMBLE_RC}")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/mumble.plist.in" "${MUMBLE_PLIST}")

include(qt-utils)
include(install-library)

option(update "Check for updates by default." ON)

option(translations "Include languages other than English." ON)
option(bundle-qt-translations "Bundle Qt's translations as well" ${static}) 

option(bundled-speex "Build the included version of Speex instead of looking for one on the system." ON)
option(renamenoise "Use ReNameNoise for machine learning noise reduction." ON)
option(bundled-renamenoise "Build the included version of ReNameNoise instead of looking for one on the system." ${renamenoise})
option(bundled-json "Build the included version of nlohmann_json instead of looking for one on the system" ON)

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

find_pkg(Qt6
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
	"Accessibility.cpp"
	"Accessibility.h"
	"ACLEditor.cpp"
	"ACLEditor.h"
	"ACLEditor.ui"
	"API_v_1_x_x.cpp"
	"API.h"
	"AudioConfigDialog.cpp"
	"AudioConfigDialog.h"
	"Audio.cpp"
	"Audio.h"
	"AudioOutputCache.cpp"
	"AudioOutputCache.h"
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
	"AudioOutputBuffer.cpp"
	"AudioOutputBuffer.h"
	"AudioOutputToken.h"
	"AudioPreprocessor.cpp"
	"AudioPreprocessor.h"
	"AudioStats.cpp"
	"AudioStats.h"
	"AudioStats.ui"
	"AudioWizard.cpp"
	"AudioWizard.h"
	"AudioWizard.ui"
	"BanEditor.cpp"
	"BanEditor.h"
	"BanEditor.ui"
	"Cert.cpp"
	"Cert.h"
	"Cert.ui"
	"ChannelFilterMode.h"
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
	"QuitBehavior.h"
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
	"GlobalShortcutTypes.h"
	"JSONSerialization.cpp"
	"JSONSerialization.h"
	"LCD.cpp"
	"LCD.h"
	"LCD.ui"
	"LegacyPlugin.cpp"
	"LegacyPlugin.h"
	"ListenerVolumeSlider.cpp"
	"ListenerVolumeSlider.h"
	"Log.cpp"
	"Log.h"
	"Log.ui"
	"Logger.cpp"
	"LookConfig.cpp"
	"LookConfig.h"
	"LookConfig.ui"
	"MainWindow.cpp"
	"MainWindow.h"
	"MainWindow.ui"
	"Markdown.cpp"
	"Markdown.h"
	"MenuLabel.cpp"
	"MenuLabel.h"
	"Messages.cpp"
	"MumbleApplication.cpp"
	"MumbleApplication.h"
	"NetworkConfig.cpp"
	"NetworkConfig.h"
	"NetworkConfig.ui"
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
	"UserLocalVolumeSlider.cpp"
	"UserLocalVolumeSlider.h"
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
	"VolumeSliderWidgetAction.cpp"
	"VolumeSliderWidgetAction.h"
	"WebFetch.cpp"
	"WebFetch.h"
	"XMLTools.cpp"
	"XMLTools.h"

	"widgets/AccessibleQGroupBox.cpp"
	"widgets/AccessibleQGroupBox.h"
	"widgets/CompletablePage.cpp"
	"widgets/CompletablePage.h"
	"widgets/EventFilters.cpp"
	"widgets/EventFilters.h"
	"widgets/MUComboBox.cpp"
	"widgets/MUComboBox.h"
	"widgets/MultiStyleWidgetWrapper.cpp"
	"widgets/MultiStyleWidgetWrapper.h"
	"widgets/MultiColumnTreeWidget.cpp"
	"widgets/MultiColumnTreeWidget.h"
	"widgets/RichTextItemDelegate.cpp"
	"widgets/RichTextItemDelegate.h"
	"widgets/SearchDialogItemDelegate.cpp"
	"widgets/SearchDialogItemDelegate.h"
	"widgets/SearchDialogTree.cpp"
	"widgets/SearchDialogTree.h"
	"widgets/SemanticSlider.cpp"
	"widgets/SemanticSlider.h"
	"widgets/TrayIcon.cpp"
	"widgets/TrayIcon.h"


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

	"mumble.qrc"
	"${CMAKE_SOURCE_DIR}/themes/DefaultTheme.qrc"
)

add_library(smallft STATIC "${3RDPARTY_DIR}/smallft/smallft.cpp")
target_include_directories(smallft PUBLIC "${3RDPARTY_DIR}/smallft")
target_disable_warnings(smallft)

add_custom_command(
	OUTPUT "${CMAKE_BINARY_DIR}/mumble_flags.qrc"
	COMMAND ${PYTHON_INTERPRETER}
	ARGS "${CMAKE_SOURCE_DIR}/scripts/generate_flag_qrc.py" --flag-dir "${CMAKE_SOURCE_DIR}/3rdparty/flag-icons/flags/4x3" --output "${CMAKE_BINARY_DIR}/mumble_flags.qrc"
	COMMENT "Generating mumble_flags.qrc"
)
list(APPEND MUMBLE_SOURCES "${CMAKE_BINARY_DIR}/mumble_flags.qrc")

add_library(mumble_client_object_lib OBJECT ${MUMBLE_SOURCES})
target_link_libraries(mumble_client_object_lib PUBLIC smallft)

if(WIN32 AND NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
	# We don't want the console to appear in release builds.
	add_executable(mumble WIN32 "main.cpp")
else()
	add_executable(mumble "main.cpp")
endif()

if(WIN32)
	target_sources(mumble_client_object_lib
		PRIVATE
			"${CMAKE_SOURCE_DIR}/auxiliary_files/mumble.appcompat.manifest"
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

add_custom_command(
	OUTPUT "ApplicationPalette.h"
	COMMAND ${PYTHON_INTERPRETER}
	ARGS ${CMAKE_SOURCE_DIR}/scripts/generate-ApplicationPalette-class.py --template ${CMAKE_CURRENT_SOURCE_DIR}/ApplicationPaletteTemplate.h
		--output ApplicationPalette.h
	COMMENT "Generating ApplicationPalette.h"
	DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/ApplicationPaletteTemplate.h"
)
target_sources(mumble PRIVATE "ApplicationPalette.h")
target_include_directories(mumble PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")

target_link_libraries(mumble mumble_client_object_lib)

target_compile_definitions(mumble_client_object_lib
	PUBLIC
	"MUMBLE_LIBRARY_PATH=${MUMBLE_INSTALL_ABS_LIBDIR}"
	"MUMBLE_PLUGIN_PATH=${MUMBLE_INSTALL_ABS_PLUGINDIR}"
)

set_target_properties(mumble PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set_target_properties(mumble_client_object_lib
	PROPERTIES
		AUTOMOC ON
		AUTORCC ON
		AUTOUIC ON
)

set_target_properties(mumble
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
endif()

# This is used to invalidate the saved widget state if a change in the source is detected.
get_numerical_file_hash("${CMAKE_CURRENT_SOURCE_DIR}/MainWindow.ui" MAINWINDOW_UI_HASH)

message(STATUS "MAINWINDOW_UI_HASH: ${MAINWINDOW_UI_HASH}")

target_compile_definitions(mumble_client_object_lib
	PUBLIC
		"MUMBLE"
		"MUMBLE_MAINWINDOW_UI_HASH=${MAINWINDOW_UI_HASH}"
		"QT_RESTRICTED_CAST_FROM_ASCII"
)

target_include_directories(mumble_client_object_lib
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR} # This is required for includes in current folder to be found by files from the shared directory.
		"widgets"
		${SHARED_SOURCE_DIR}
		"${PLUGINS_DIR}"
)

find_pkg(Poco
	COMPONENTS
		XML
		Zip
)

if(TARGET Poco::Zip)
	target_link_libraries(mumble_client_object_lib
		PUBLIC
			Poco::XML
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


if(bundled-json)
	set(JSON_BuildTests OFF CACHE INTERNAL "")
	set(JSON_ImplicitConversions ON CACHE INTERNAL "")
	set(JSON_SystemInclude ON CACHE INTERNAL "")
	add_subdirectory("${3RDPARTY_DIR}/nlohmann_json/" "${CMAKE_CURRENT_BINARY_DIR}/nlohmann_json/" EXCLUDE_FROM_ALL)
else()
	find_pkg("nlohmann_json" REQUIRED)
endif()

target_link_libraries(mumble_client_object_lib PUBLIC nlohmann_json::nlohmann_json)

find_pkg("SndFile;LibSndFile;sndfile" REQUIRED)

# Check if sndfile version supports opus
if("${sndfile_VERSION}" VERSION_GREATER_EQUAL "1.0.29")
	target_compile_definitions(mumble_client_object_lib PUBLIC USE_SNDFILE_OPUS)
else()
	message(WARNING "libsndfile is missing Opus-support -> No Opus-format recording")
endif()

# Check if sndfile version supports mp3
if("${sndfile_VERSION}" VERSION_GREATER_EQUAL "1.1.0")
	target_compile_definitions(mumble_client_object_lib PUBLIC USE_SNDFILE_MP3)
else()
	message(WARNING "libsndfile is missing Mp3-support -> No Mp3-format recording")
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
		utfcpp
		Qt6::Concurrent
		Qt6::Sql
		Qt6::Svg
		Qt6::Widgets
)

if(static)
	if(TARGET Qt6::QSQLiteDriverPlugin)
		include_qt_plugin(mumble_client_object_lib PRIVATE "QSQLiteDriverPlugin")
		target_link_libraries(mumble_client_object_lib PUBLIC Qt6::QSQLiteDriverPlugin)
	endif()
	if(TARGET Qt6::QSvgIconPlugin)
		include_qt_plugin(mumble_client_object_lib PRIVATE "QSvgIconPlugin")
		target_link_libraries(mumble_client_object_lib PUBLIC Qt6::QSvgIconPlugin)
	endif()
	if(TARGET Qt6::QSvgPlugin)
		include_qt_plugin(mumble_client_object_lib PRIVATE "QSvgPlugin")
		target_link_libraries(mumble_client_object_lib PUBLIC Qt6::QSvgPlugin)
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

	if(static AND TARGET Qt6::QWindowsIntegrationPlugin)
		include_qt_plugin(mumble_client_object_lib PRIVATE QWindowsIntegrationPlugin)
		target_link_libraries(mumble_client_object_lib PUBLIC Qt6::QWindowsIntegrationPlugin)
	endif()

	add_subdirectory("${3RDPARTY_DIR}/SPSCQueue" "${CMAKE_CURRENT_BINARY_DIR}/SPSCQueue" EXCLUDE_FROM_ALL)
	add_subdirectory("${3RDPARTY_DIR}/xinputcheck-build" "${CMAKE_CURRENT_BINARY_DIR}/xinputcheck" EXCLUDE_FROM_ALL)

	# Disable all warnings that the xinputcheck code may emit
	disable_warnings_for_all_targets_in("${3RDPARTY_DIR}/xinputcheck-build")

	target_link_libraries(mumble_client_object_lib
		PUBLIC
			SPSCQueue
			xinputcheck
	)

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

		if(static AND TARGET Qt6::QXcbIntegrationPlugin)
			include_qt_plugin(mumble_client_object_lib PRIVATE QXcbIntegrationPlugin)
			target_link_libraries(mumble_client_object_lib PUBLIC Qt6::QXcbIntegrationPlugin)
		endif()

		target_sources(mumble_client_object_lib
			PRIVATE
				"GlobalShortcut_unix.cpp"
				"GlobalShortcut_unix.h"
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
				"os_macx.mm"
		)

		if(static AND TARGET Qt6::QCocoaIntegrationPlugin)
			include_qt_plugin(mumble_client_object_lib PRIVATE QCocoaIntegrationPlugin)
			target_link_libraries(mumble_client_object_lib PUBLIC Qt6::QCocoaIntegrationPlugin)
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

find_pkg("opus;Opus" REQUIRED)
target_include_directories(mumble_client_object_lib PUBLIC ${opus_INCLUDE_DIRS})
target_link_libraries(mumble_client_object_lib PUBLIC ${opus_LIBRARIES})
if(TARGET opus)
	target_link_libraries(mumble_client_object_lib PUBLIC opus)
elseif(TARGET Opus)
	target_link_libraries(mumble_client_object_lib PUBLIC Opus)
elseif(TARGET Opus::opus)
	target_link_libraries(mumble_client_object_lib PUBLIC Opus::opus)
endif()

if(bundled-speex)
	add_subdirectory("${3RDPARTY_DIR}/speexdsp-build" "${CMAKE_CURRENT_BINARY_DIR}/speexdsp" EXCLUDE_FROM_ALL)

	# Disable all warnings that the speexdsp code may emit
	disable_warnings_for_all_targets_in("${3RDPARTY_DIR}/speexdsp-build")

	target_link_libraries(mumble_client_object_lib PUBLIC speexdsp)

	if(WIN32)
		# Shared library on Windows (e.g. ".dll")
		set_target_properties(speexdsp PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
	else()
		# Shared library on UNIX (e.g. ".so")
		set_target_properties(speexdsp PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
	endif()

	install_library(speexdsp mumble_client)
else()
	find_pkg(speexdsp REQUIRED)

	target_link_libraries(mumble_client_object_lib
		PUBLIC
			${speexdsp_LIBRARIES}
	)
endif()

if(renamenoise)
	target_compile_definitions(mumble_client_object_lib PRIVATE "USE_RENAMENOISE")

	if(bundled-renamenoise)
		set(RENAMENOISE_DEMO_EXECUTABLE OFF CACHE INTERNAL "")

		add_subdirectory("${3RDPARTY_DIR}/renamenoise" "${CMAKE_CURRENT_BINARY_DIR}/renamenoise" EXCLUDE_FROM_ALL)

		# Disable all warnings that the ReNameNoise code may emit
		disable_warnings_for_all_targets_in("${3RDPARTY_DIR}/renamenoise")

		target_link_libraries(mumble_client_object_lib PRIVATE renamenoise)

		if(WIN32)
			# Shared library on Windows (e.g. ".dll")
			set_target_properties(renamenoise PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
		else()
			# Shared library on UNIX (e.g. ".so")
			set_target_properties(renamenoise PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
		endif()

		install_library(renamenoise mumble_client)
	else()
		find_pkg(renamenoise REQUIRED)

		target_link_libraries(mumble_client_object_lib PRIVATE ${renamenoise_LIBRARIES})
	endif()
endif()

if(qtspeech)
	find_pkg(Qt6 COMPONENTS TextToSpeech REQUIRED)
	target_sources(mumble_client_object_lib PRIVATE "TextToSpeech.cpp")
	target_link_libraries(mumble_client_object_lib PUBLIC Qt6::TextToSpeech)
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

if(NOT WIN32 AND NOT APPLE)
	find_pkg(Qt6 COMPONENTS DBus REQUIRED)

	target_sources(mumble_client_object_lib
		PRIVATE
			"DBus.cpp"
			"DBus.h"
	)
	
	target_compile_definitions(mumble_client_object_lib PUBLIC "USE_DBUS")
	target_link_libraries(mumble_client_object_lib PUBLIC Qt6::DBus)
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

		add_dependencies(mumble overlay)
	else()
		if(APPLE)
			target_sources(mumble_client_object_lib PRIVATE "Overlay_macx.mm")
		else()
			target_sources(mumble_client_object_lib PRIVATE "Overlay_unix.cpp")
		endif()

		add_dependencies(mumble overlay_gl)
	endif()

	target_compile_definitions(mumble_client_object_lib PUBLIC "USE_OVERLAY")
endif()

if(plugins)
	add_dependencies(mumble plugins)
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
	target_include_directories(mumble_client_object_lib PUBLIC "${CMAKE_SOURCE_DIR}/helpers")
	add_dependencies(mumble g15-helper)
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

if(packaging AND WIN32)
	set(overlay ON)
	set(plugins ON)

	if(translations)
		list(APPEND installer_vars "--all-languages")
	endif()


	list(APPEND installer_vars
		"--version" ${PROJECT_VERSION}
		"--arch" "${MUMBLE_TARGET_ARCH}"
	)

	if(overlay)
		list(APPEND installer_vars
			"--overlay"
		)
	endif()

	if(g15)
		list(APPEND installer_vars
			"--g15"
		)
	endif()

	file(COPY
		${CMAKE_SOURCE_DIR}/installer/MumbleInstall.cs
		${CMAKE_SOURCE_DIR}/installer/ClientInstaller.cs
		DESTINATION
			${CMAKE_BINARY_DIR}/installer/client
	)

	add_custom_command(TARGET mumble
		POST_BUILD
		COMMAND cscs.exe -cd MumbleInstall.cs
		COMMAND cscs.exe ClientInstaller.cs ${installer_vars}
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/installer/client
	)
endif()
