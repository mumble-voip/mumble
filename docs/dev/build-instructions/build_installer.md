# Creating an installer

Currently, the installer creation has been tested on Windows only.

For creating the installer, the [WiX Toolset](https://wixtoolset.org/) has to be installed on your system.

An installer can be created after invoking cmake with the `-Dpackaging=ON` option and building.

To create a *single-language* installer (default English) run `cpack -C Release`.

To create a *multi-language* installer run the script `scripts/Create-Win32InstallerMUI.ps1` from the root of this repository.

