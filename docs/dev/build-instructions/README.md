# Build Mumble

This documentation is version-specific. The current ``master`` branch contains the unstable code for a future release of Mumble.
If you want to build an already released stable version of Mumble, use the respective branch.

A fundamental dependency for building Mumble is [git](https://git-scm.com/), so make sure you have it installed. If you are new to git, make sure to
checkout [this guide](https://guides.github.com/introduction/git-handbook/) on git's basics in order to be able to follow the given instructions.

Furthermore Mumble requires a **Cpp17**-conform compiler.

The first step in building Mumble is to clone this repository and all the submodules via `git clone --depth 1 --recursive https://github.com/mumble-voip/mumble.git`.

_Tip:_ You can also [build a specific version or commit](faq.md#build-a-specific-version-or-commit) of Mumble.

In order to actually build Mumble, you can follow one of the following instruction sets:
- [Build on Windows](build_windows.md)
- [Build on Linux](build_linux.md)
- [Build on macOS](build_macos.md)
- [Static build](build_static.md)


Furthermore you might find these helpful as well:
- [FAQ](faq.md)
- [Determine build number](find_build_number.md)
- [Common build errors](common_build_errors.md)
- [List of available cmake options](cmake_options.md)
- [IDE integration](ide_integration.md)
- [Create an installer](build_installer.md)
- [Setup Visual Studio](setup_visual_studio.md)
