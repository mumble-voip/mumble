# Creating an installer

Currently, the installer creation has been tested on Windows only.

For creating the installer, [WixSharp](https://github.com/oleg-shilo/wixsharp) has to be present on your system.

1. Download the [latest WixSharp release](https://github.com/oleg-shilo/wixsharp/releases)
2. Extract into a folder
3. Add that folder to the PATH environment variable

When creating an installer, you _have to_ specify a build number using e.g. `-DBUILD_NUMBER=3` when invoking cmake. The build number is used to
differentiate multiple builds for the same Mumble version and makes sure that always the latest build will upgrade any previous builds. If you are
creating an installer for yourself, just use build number `0`.

The cmake option `packaging`, off by default, specifies whether or not to build the installer. If being built it will be multi-lingual by default. 

Use the cmake generate option `-Dpackaging=ON` to enable building the installer.

Use the additional cmake generate option `-Dtranslations=OFF` to build a single-language installer instead.

Note that the installers expect some components to be built. For example, the client installer expects the `g15` component to be built too. Without it packaging will fail on the missing expected file.

