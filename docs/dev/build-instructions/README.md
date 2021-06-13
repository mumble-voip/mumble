# Build Mumble

A fundamental dependency for building Mumble is [git](https://git-scm.com/), so make sure you have it installed. If you are new to git, make sure to
checkout [this guide](https://guides.github.com/introduction/git-handbook/) on git's basics in order to be able to follow the given instructions.

Furthermore Mumble requires a **Cpp14**-conform compiler.

The first step in building Mumble is to clone this repository via `git clone https://github.com/mumble-voip/mumble.git` and then going into the cloned
directory and run `git submodule update --init --recursive` in order to also clone all submodules.

_Tip:_ You can also [build a specific version or commit](faq.md#build-a-specific-version-or-commit) of Mumble.

Then go into the cloned repository and run `git submodule update --init --recursive` in order to also clone all submodules.

In order to actually build Mumble, you can follow one of the following instruction sets:
- [Build on Windows](build_windows.md)
- [Build on Linux](build_linux.md)
- [Build on macOS](build_macos.md)
- [Static build](build_static.md)


Furthermore you might find these helpful as well:
- [FAQ](faq.md)
- [List of available cmake options](cmake_options.md)
- [IDE integration](ide_integration.md)
- [Create an installer](build_installer.md)
- [Setup Visual Studio](setup_visual_studio.md)
