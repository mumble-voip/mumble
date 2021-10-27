# Common build errors

This page is supposed to give a summary of the most common build errors that are encountered when building Mumble and how to fix them.

| Error message | Description and fix |
| ------------- | ------------------- |
| inlining failed in call to always_inline ‘vdupq_n_f32’: target specific option mismatch | This error message occurs when attempting to build Opus. It seems to be an issue specific to the Raspberry Pi (4) (or maybe to ARM in general). This is a [known problem in Opus](https://github.com/xiph/opus/issues/203).<br><br>It can be fixed relatively easy by specifying `-DOPUS_DISABLE_INTRINSICS=ON` when invoking CMake. |
