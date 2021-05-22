# Creating an installer

Currently, the installer creation has been tested on Windows only.

For creating the installer, [WixSharp](https://github.com/oleg-shilo/wixsharp/releases) has to be present on your system. Please see the following [README](https://github.com/oleg-shilo/wixsharp/blob/master/README.md) for install information.

An installer can be created after invoking cmake with the `-Dpackaging=ON` and `-Dtranslations=OFF` options, and building. This creates a *single-language* installer.

To create a *multi-language* installer, make sure the following option is set `-Dpackaging=ON` and (re-)run the cmake configure step. Multi-language packaging is the default.

