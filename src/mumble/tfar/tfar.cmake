# MUMBLE-TFAR
# Adds the Task Force Arrowhead Radio compatibility layer to the Mumble client.
# Windows-only (Arma 3 extension IPC + X3DAudio).

set(TFAR_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tfar")

set(TFAR_SOURCES
	# Mumble integration (new code)
	"${TFAR_DIR}/TFARBridge.cpp"
	"${TFAR_DIR}/TFARBridge.h"
	"${TFAR_DIR}/TS3Compat.h"
	"${TFAR_DIR}/TeamspeakMumble.cpp"
	"${TFAR_DIR}/TFARConfig.cpp"
	"${TFAR_DIR}/TFARConfig.h"
	"${TFAR_DIR}/StormBranding.h"
	"${TFAR_DIR}/StormUpdateCheck.cpp"
	"${TFAR_DIR}/StormUpdateCheck.h"
	"${TFAR_DIR}/DebugUI.cpp"
	"${TFAR_DIR}/DebugUI.h"

	# TFAR plugin core (ported unchanged from task-force-arma-3-radio/ts/src)
	"${TFAR_DIR}/antennaManager.cpp"
	"${TFAR_DIR}/antennaManager.h"
	"${TFAR_DIR}/clientData.cpp"
	"${TFAR_DIR}/clientData.hpp"
	"${TFAR_DIR}/CommandProcessor.cpp"
	"${TFAR_DIR}/CommandProcessor.hpp"
	"${TFAR_DIR}/datatypes.cpp"
	"${TFAR_DIR}/datatypes.hpp"
	"${TFAR_DIR}/helpers.cpp"
	"${TFAR_DIR}/helpers.hpp"
	"${TFAR_DIR}/Logger.cpp"
	"${TFAR_DIR}/Logger.hpp"
	"${TFAR_DIR}/PlaybackHandler.cpp"
	"${TFAR_DIR}/PlaybackHandler.hpp"
	"${TFAR_DIR}/pipe_handler.cpp"
	"${TFAR_DIR}/pipe_handler.hpp"
	"${TFAR_DIR}/plugin.cpp"
	"${TFAR_DIR}/plugin.h"
	"${TFAR_DIR}/profilers.cpp"
	"${TFAR_DIR}/profilers.hpp"
	"${TFAR_DIR}/SampleBuffer.cpp"
	"${TFAR_DIR}/SampleBuffer.hpp"
	"${TFAR_DIR}/serverData.cpp"
	"${TFAR_DIR}/serverData.hpp"
	# NOTE: server_radio_data.{cpp,hpp} are dead legacy files in the TFAR repo
	# (they reference non-existent headers) and were not part of the original
	# vcxproj either — do not add them to the build.
	"${TFAR_DIR}/SharedMemoryHandler.cpp"
	"${TFAR_DIR}/SharedMemoryHandler.hpp"
	"${TFAR_DIR}/task_force_radio.cpp"
	"${TFAR_DIR}/task_force_radio.hpp"
	"${TFAR_DIR}/Teamspeak.hpp"

	# bundled third party libraries used by TFAR
	"${TFAR_DIR}/include/clunk/buffer.cpp"
	"${TFAR_DIR}/include/clunk/clunk_ex.cpp"
	"${TFAR_DIR}/include/clunk/clunk_logger.cpp"
	"${TFAR_DIR}/include/clunk/context.cpp"
	"${TFAR_DIR}/include/clunk/distance_model.cpp"
	"${TFAR_DIR}/include/clunk/hrtf.cpp"
	"${TFAR_DIR}/include/clunk/kemar.c"
	"${TFAR_DIR}/include/clunk/object.cpp"
	"${TFAR_DIR}/include/clunk/sample.cpp"
	"${TFAR_DIR}/include/clunk/source.cpp"
	"${TFAR_DIR}/include/clunk/sse_fft_context.cpp"
	"${TFAR_DIR}/include/clunk/stream.cpp"
	"${TFAR_DIR}/include/clunk/wav_file.cpp"
	"${TFAR_DIR}/include/dspfilters/source/Bessel.cpp"
	"${TFAR_DIR}/include/dspfilters/source/Biquad.cpp"
	"${TFAR_DIR}/include/dspfilters/source/Butterworth.cpp"
	"${TFAR_DIR}/include/dspfilters/source/Cascade.cpp"
	"${TFAR_DIR}/include/dspfilters/source/ChebyshevI.cpp"
	"${TFAR_DIR}/include/dspfilters/source/ChebyshevII.cpp"
	"${TFAR_DIR}/include/dspfilters/source/Custom.cpp"
	"${TFAR_DIR}/include/dspfilters/source/Design.cpp"
	"${TFAR_DIR}/include/dspfilters/source/Documentation.cpp"
	"${TFAR_DIR}/include/dspfilters/source/Elliptic.cpp"
	"${TFAR_DIR}/include/dspfilters/source/Filter.cpp"
	"${TFAR_DIR}/include/dspfilters/source/Legendre.cpp"
	"${TFAR_DIR}/include/dspfilters/source/Param.cpp"
	"${TFAR_DIR}/include/dspfilters/source/PoleFilter.cpp"
	"${TFAR_DIR}/include/dspfilters/source/RBJ.cpp"
	"${TFAR_DIR}/include/dspfilters/source/RootFinder.cpp"
	"${TFAR_DIR}/include/dspfilters/source/State.cpp"
	"${TFAR_DIR}/include/simpleSource/SimpleComp.cpp"
	"${TFAR_DIR}/include/simpleSource/SimpleEnvelope.cpp"
	"${TFAR_DIR}/include/simpleSource/SimpleGate.cpp"
	"${TFAR_DIR}/include/simpleSource/SimpleLimit.cpp"
)

target_sources(mumble_client_object_lib PRIVATE ${TFAR_SOURCES})

target_compile_definitions(mumble_client_object_lib PUBLIC "USE_TFAR")

# The TFAR include directories are applied per source file only: adding them to
# the whole target would shadow Mumble's own headers on case-insensitive file
# systems (e.g. tfar/version.h vs src/Version.h, tfar/plugin.h vs Plugin.h).
#
# The TFAR sources are third party code — don't apply Mumble's strict warning
# policy (warnings-as-errors) to them either.
if(MSVC)
	set(TFAR_WARNING_FLAGS "/W0")
else()
	set(TFAR_WARNING_FLAGS "-w")
endif()

set_source_files_properties(${TFAR_SOURCES}
	PROPERTIES
		INCLUDE_DIRECTORIES "${TFAR_DIR};${TFAR_DIR}/include"
		COMPILE_OPTIONS "${TFAR_WARNING_FLAGS}"
		# CLUNKAPI= : clunk is linked statically (no dllimport/dllexport).
		# X64BUILD / CLUNK_USES_SSE / NOMINMAX match the original TFAR project.
		COMPILE_DEFINITIONS "NOMINMAX;_CRT_SECURE_NO_WARNINGS;CLUNKAPI=;CLUNK_USES_SSE;X64BUILD;WINDOWS"
)

# The original TFAR/bundled-library sources predate MSVC's conformance mode
# (/std:c++20 implies /permissive-). Compile just them with /permissive.
# The Qt-facing integration sources (TFARBridge, TFARConfig, TeamspeakMumble,
# StormUpdateCheck, DebugUI) are new code and stay conformant.
if(MSVC)
	set(TFAR_LEGACY_SOURCES ${TFAR_SOURCES})
	list(FILTER TFAR_LEGACY_SOURCES EXCLUDE REGEX
		"(TFARBridge|TFARConfig|TeamspeakMumble|StormUpdateCheck|DebugUI|TS3Compat|StormBranding)")
	set_property(SOURCE ${TFAR_LEGACY_SOURCES} APPEND PROPERTY COMPILE_OPTIONS "/permissive")
endif()

# WinInet: legacy TFAR networking helpers. XAudio2: the X3DAudio* functions
# (ILD spatialization) are exported from XAudio2_9.dll in current Windows SDKs
# (the standalone x3daudio.lib only existed in the legacy DirectX SDK).
target_link_libraries(mumble_client_object_lib PUBLIC wininet.lib xaudio2.lib)

# Radio sounds are looked up at runtime in "<dir of mumble.exe>/tfar/radio-sounds"
# (configurable on the TFAR settings page). Copy them next to the built binary
# and ship them with the installed client.
add_custom_command(TARGET mumble POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${TFAR_DIR}/radio-sounds"
		"$<TARGET_FILE_DIR:mumble>/tfar/radio-sounds"
	COMMENT "Copying TFAR radio sounds"
)
install(DIRECTORY "${TFAR_DIR}/radio-sounds"
	DESTINATION "${MUMBLE_INSTALL_EXECUTABLEDIR}/tfar"
	COMPONENT mumble_client
)
