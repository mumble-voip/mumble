# Build Mumble

This documentation is version-specific. The current ``master`` branch contains the unstable code for a future release of Mumble.
If you want to build an already released stable version of Mumble, use the respective branch.

A fundamental dependency for building Mumble is [git](https://git-scm.com/), so make sure you have it installed. If you are new to git, make sure to
checkout [this guide](https://guides.github.com/introduction/git-handbook/) on git's basics in order to be able to follow the given instructions.

Furthermore Mumble requires a **Cpp17**-conform compiler.

The first step in building Mumble is to clone this repository and all the submodules via `git clone --depth 1 --recursive https://github.com/mumble-voip/mumble.git`.

_Tip:_ You can also [build a specific version or commit](faq.md#build-a-specific-version-or-commit) of Mumble.


## Using the BUILD_NUMBER variable

We are currently using a [versioning scheme](https://www.mumble.info/blog/new-versioning-scheme/) that monotonically increases
the "build" or "patch" component of the version string. This build number is specified via cmake as a variable e.g. ``-DBUILD_NUMBER=123``.
Since we are exposing the complete version string in the Mumble UI as well as over the network protocol, it would be nice if the ``BUILD_NUMBER``
variable is set correctly when building the binaries. This is especially relevant for **distribution package maintainers**.
If not specified, the version string will end in ``.0``. This is also fine and not break anything, but it will be harder for
us to determine the version of Mumble in bug reports and usage statistics.

There exists a mapping between commit hash and ``BUILD_NUMBER``. If a ``BUILD_NUMBER`` exists for a given commit, it can be
retrieved from our website by requesting ``https://mumble.info/get-build-number?commit=<commit_hash>&version=<mumble_version>``
where ``<mumble_version>`` refers to the major and minor version of Mumble you are building e.g. ``1.6``. If there is no ``BUILD_NUMBER``
for a given commit, feel free to fallback to ``0``. There is also a [Python script](/scripts/mumble-build-number.py) which can be used
to perform this HTTP request automatically.


## Specific Instructions

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
