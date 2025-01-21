# CMake options

Using CMake the build can be customized in a number of ways. The most prominent examples for this is the usage of different
options (flags). These can be set by using `-D<optionName>=<value>` where `<optionName>` is the name of the respective option
as listed below and `<value>` is either `ON` or `OFF` depending on whether the option shall be activated or inactivated.

An example would be `cmake -Dtests=ON ..`.


## Available options

### alsa

Build support for ALSA.
(Default: ON)

### asio

Build support for ASIO audio input.
(Default: OFF)

### benchmarks

Build benchmarks
(Default: OFF)

### BUILD_OVERLAY_XCOMPILE

Build an x86 overlay
(Default: OFF)

### bundle-qt-translations

Bundle Qt's translations as well
(Default: ${static})

### bundled-gsl

Use the bundled GSL version instead of looking for one on the system
(Default: ON)

### bundled-json

Build the included version of nlohmann_json instead of looking for one on the system
(Default: ON)

### bundled-renamenoise

Build the included version of ReNameNoise instead of looking for one on the system.
(Default: ${renamenoise})

### bundled-spdlog

Use the bundled spdlog version instead of looking for one on the system
(Default: ON)

### bundled-speex

Build the included version of Speex instead of looking for one on the system.
(Default: ON)

### client

Build the client (Mumble)
(Default: ON)

### coreaudio

Build support for CoreAudio.
(Default: ON)

### crash-report

Include support for reporting crashes to the Mumble developers.
(Default: ON)

### debug-dependency-search

Prints extended information during the search for the needed dependencies
(Default: OFF)

### display-install-paths

Print out base install paths during project configuration
(Default: OFF)

### elevation

Set \"uiAccess=true\", required for global shortcuts to work with privileged applications. Requires the client's executable to be signed with a trusted code signing certificate.
(Default: OFF)

### g15

Include support for the G15 keyboard (and compatible devices).
(Default: OFF)

### g15-emulator

Build the g15helper executable in emulator mode. This will cause an emulated G15 window to appear on screen. Allows the use of Mumble's G15 support without owning the physical hardware.
(Default: OFF)

### gkey

Build support for Logitech G-Keys. Note: This feature does not require any build-time dependencies, and requires Logitech Gaming Software to be installed to have any effect at runtime.
(Default: ON)

### ice

Build support for Ice RPC.
(Default: ON)

### jackaudio

Build support for JackAudio.
(Default: ON)

### lto

Enables link-time optimizations for release builds
(Default: ${LTO_DEFAULT})

### manual-plugin

Include the built-in \"manual\
(Default: positional audio plugin." ON)

### online-tests

Whether or not tests that need a working internet connection should be included
(Default: OFF)

### optimize

Build a heavily optimized version, specific to the machine it's being compiled on.
(Default: OFF)

### oss

Build support for OSS.
(Default: ON)

### overlay

Build overlay.
(Default: ${client})

### overlay-xcompile

Build 32 bit overlay library, necessary for the overlay to work with 32 bit processes.
(Default: ON)

### packaging

Build package.
(Default: OFF)

### pipewire

Build support for PipeWire.
(Default: ON)

### plugin-callback-debug

Build Mumble with debug output for plugin callbacks inside of Mumble.
(Default: OFF)

### plugin-debug

Build Mumble with debug output for plugin developers.
(Default: OFF)

### plugins

Build plugins.
(Default: ON)

### portaudio

Build support for PortAudio
(Default: ON)

### pulseaudio

Build support for PulseAudio.
(Default: ON)

### qssldiffiehellmanparameters

Build support for custom Diffie-Hellman parameters.
(Default: ON)

### qtspeech

Use Qt's text-to-speech system (part of the Qt Speech module) instead of Mumble's own OS-specific text-to-speech implementations.
(Default: OFF)

### renamenoise

Use ReNameNoise for machine learning noise reduction.
(Default: ON)

### retracted-plugins

Build redacted (outdated) plugins as well
(Default: OFF)

### server

Build the server (Murmur)
(Default: ON)

### speechd

Build support for Speech Dispatcher.
(Default: ON)

### static

Build static binaries.
(Default: OFF)

### symbols

Build binaries in a way that allows easier debugging.
(Default: OFF)

### tests

Build tests.
(Default: ${packaging})

### tracy

Enable the tracy profiler.
(Default: OFF)

### translations

Include languages other than English.
(Default: ON)

### update

Check for updates by default.
(Default: ON)

### use-pkgconf-install-paths

Try to query install paths from pkgconf - this is incompatible to using CMAKE_INSTALL_PREFIX
(Default: OFF)

### warnings-as-errors

All warnings are treated as errors.
(Default: ON)

### wasapi

Build support for WASAPI.
(Default: ON)

### xboxinput

Build support for global shortcuts from Xbox controllers via the XInput DLL.
(Default: ON)

### xinput2

Build support for XI2.
(Default: ON)

### zeroconf

Build support for zeroconf (mDNS/DNS-SD).
(Default: ON)


