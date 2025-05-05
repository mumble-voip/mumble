# Common build errors

This page is supposed to give a summary of the most common build errors that are encountered when building Mumble and how to fix them.

| Error message | Description and fix |
| ------------- | ------------------- |
| inlining failed in call to always_inline ‘vdupq_n_f32’: target specific option mismatch | This error message occurs when attempting to build Opus. It seems to be an issue specific to the Raspberry Pi (4) (or maybe to ARM in general). This is a [known problem in Opus](https://github.com/xiph/opus/issues/203).<br><br>It can be fixed relatively easy by specifying `-DOPUS_DISABLE_INTRINSICS=ON` when invoking CMake. |
|CMake Error at cmake/qt-utils.cmake:6 (include):<br>  include could not find requested file:<br><br>    FindPythonInterpreter<br>Call Stack (most recent call first):<br> <br>src/murmur/CMakeLists.txt:16 (include) | Submodules are outdated<br> Run the command 'git submodule update --init' |
