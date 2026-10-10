# WebRTC AEC3 Echo Cancellation

Mumble's built-in echo cancellation uses SpeexDSP — a linear MDF filter from ~2007. It works adequately with headphones but struggles with real-world speaker setups: nonlinear distortion, room reverb, and rapidly changing acoustic conditions produce audible echo artifacts.

This optional mode adds **WebRTC AEC3**, the same algorithm used in Chrome, Teams, and Discord. It is enabled at build time with `-Dwebrtc-apm=ON` and appears in Audio Input settings as **"Echo cancellation (WebRTC AEC3)"**.

## How it works

WebRTC APM separates render (speaker) and capture (microphone) processing, which lets it bypass Mumble's existing Resynchronizer queue:

- **`addEcho()`** — speaker samples feed directly into `ProcessReverseStream()`. The `short[]` allocation and `resync.addSpeaker()` are skipped.
- **`encodeAudioFrame()`** — mic samples feed directly into `ProcessStream()` instead of the Speex path. The measured output latency (hardware path from WASAPI's `GetStreamLatency` + software buffer occupancy) is passed as the stream delay so AEC3 can align the two streams.

The existing SpeexDSP preprocessor (VAD, AGC, denoising) still runs on the cleaned signal afterward. The Resynchronizer and Speex AEC paths are untouched — `SPEEX_MIXED` and `SPEEX_MULTICHANNEL` behave exactly as before.

## Platform support

| Backend | Supported |
|---|---|
| Windows (WASAPI) | Yes |
| Linux (PulseAudio) | Yes |
| macOS | No — use `APPLE_AEC` instead |
| ALSA | No — no loopback capture available |

## Getting the dependency

`webrtc-audio-processing` (≥ 2.0, freedesktop fork) must be installed separately — it is not bundled with Mumble.

**Linux:**
```sh
# Ubuntu/Debian
sudo apt install libwebrtc-audio-processing-dev
# Fedora
sudo dnf install webrtc-audio-processing-devel
```

**Windows (vcpkg):**
```sh
vcpkg install webrtc-audio-processing:x64-windows-static-md
```

> **Note:** Mumble's vcpkg fork (`mumble-voip/vcpkg`) does not yet include this port. For now, install from upstream vcpkg and point `CMAKE_PREFIX_PATH` at your vcpkg installed tree, or build from source.

**From source:**
```sh
git clone https://gitlab.freedesktop.org/pipewire/webrtc-audio-processing.git
cd webrtc-audio-processing
meson setup build --prefix=/your/install/prefix
ninja -C build install
```

## Building Mumble with WebRTC AEC3

```sh
cmake -Dwebrtc-apm=ON -DCMAKE_PREFIX_PATH=/your/install/prefix ..
ninja mumble
```

On Windows, run this from an MSVC x64 developer environment. The DLL (`webrtc-audio-processing-2-1.dll`) is automatically copied to the build output directory by a `POST_BUILD` step.

Builds without the flag (`-Dwebrtc-apm=OFF`, the default) are unaffected — no behavior change, no new dependency.

## Related

- `src/mumble/AudioInput.cpp` — `resetAudioProcessor()`, `addMic()`, `addEcho()`, `encodeAudioFrame()`
- `src/mumble/EchoCancelOption.h` / `.cpp` — enum and option list
- `docs/dev/build-instructions/cmake_options.md` — `webrtc-apm` option reference
- `docs/dev/AudioInputDebug.md` — how to tap the DSP chain for debugging
