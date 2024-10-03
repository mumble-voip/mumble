# Building static Mumble

These instructions are for creating a static build of Mumble. "Static" means that it bundles most (all) of its dependencies into the built library in
order to make it run on systems that don't have the necessary dependencies installed.


## Dependencies

We are using [vcpkg](https://github.com/Microsoft/vcpkg) in order to manage the dependencies needed for the build. This package manager is available
on all major platforms (Windows, Linux, macOS, probably more), so the instructions here apply to all of these systems.
In order to ensure maximum reproducibility, we host our own fork of vcpkg at https://github.com/mumble-voip/vcpkg that contains vcpkg packages at the
exact state that have been used by us to build Mumble. There are also [pre-built environments](https://github.com/mumble-voip/vcpkg/releases)
available there.

The entire build environment will require between 30GB and 60GB of space on your hard-drive. If installed via script, it tends to converge towards the
lower bound whereas a manual installation usually tends towards the higher bound.

In addition to the dependencies installed via vcpkg, you'll also need [cmake](https://cmake.org/) (v3.15 or later). On Linux you might have to install
cmake using a [PPA](https://apt.kitware.com/) and on macOS you can install it using [homebrew](https://formulae.brew.sh/formula/cmake).

Furthermore you need a C++ compiler. On Linux and MacOS that'd usually be `gcc` (`g++`) or `clang`. On Windows you probably want to use MSVC
(checkout [these instructions](setup_visual_studio.md) on how to install it).


### Installing via script

The recommended way of installing the dependencies on your system is to clone our vcpkg fork and run the installation script located in the
repository's root.

On Windows you'll have to run the script by opening a PowerShell window, executing `Set-ExecutionPolicy -Scope CurrentUser RemoteSigned` and then
running the mentioned script. On Linux and macOS no such preliminaries are necessary.

If you already have vcpkg installed on your system, you should probably prefer using the manual installation of dependencies in order to avoid a
second vcpkg installation.


### Manual installation

Again, we advice to use our vcpkg fork. If you are using upstream vcpkg, you do not have access to a compatible version of ZeroC Ice, so you'll have
to disable that when building the server.

The current list of vcpkg packages required to build Mumble can be found at https://github.com/mumble-voip/vcpkg/blob/master/mumble_dependencies.txt.

On Windows, you'll also have to install the `mdnsresponder` package.

The command for installing a package is `vcpkg install <packageName> --triplet <triplet>` where `<packageName>` is to be replaced with the name of the
package you want to install and `<triplet>` is the desired target triplet. We recommend using these triplets:

| **OS** | **Triplet** |
|--------|-------------|
| Windows | x64-windows-static-md |
| Linux | x64-linux |
| macOS | x64-osx |

Therefore if you are on Windows, you'd install `boost` as `vcpkg install boost --triplet x64-windows-static-md`.


### Additional dependencies on Linux

Additional `dev` packages will need to be installed for some components in vcpkg on GNU/Linux (package names match Ubuntu packages):

* `libxi(X11)`
* `libgl1-mesa`
* `libglu1-mesa`
* `mesa-common`
* `libxrandr`
* `libxxf86vm`
* `libbluetooth`
* `libx11-xcb`

vcpkg will also require installation of:

* `python3`
* `python2`
* `python`

Most GNU/Linux distros should have these or equivalent packages.


### Additional dependencies on macOS

The following is required for macOS:

* `Xquartz`


## Building Mumble

After all dependencies are installed, you can follow these steps in order to actually build Mumble itself:

1. Start a command line (on Windows see caveats listed below)
2. Clone the Mumble repo <https://github.com/mumble-voip/mumble.git> (if not done already)
3. Navigate into the cloned directory
5. Create a directory named `build` and navigate into it (`mkdir build && cd build`)
6. Run the cmake generator with relative target path `..` (more on that further down)
7. Run the cmake in build-mode: `cmake --build .` or invoke your buildsystem of choice directly (e.g. `make -j $(nproc)`)

cmake will generate a bunch of files so you should call it from a dedicated, empty directory ("out-of-source build"). In the list above we suggest
`build`.


#### Windows caveats

On Windows you can't use the default command-prompt (as is) as it won't have the needed development tools in its PATH. Instead you have to use
a "Developer Command Prompt". You can find it by searching in the start-menu. If you are on a 64bit system, then special care must be taken
that you use a "x64" version of the Developer Prompt (often these are then called "x64 Native Tools Command Prompt"). The easiest way to get a hold of
the correct command prompt is to search for "x64" and usually that is enough to bring up the x64 developer prompt in the search results.

Note also that you **have** to use the command prompt and **not** the Developer Powershell as the latter is always 32bit only.

If you are on a 64bit system, then you'll know that you have opened the correct prompt, if it prints `Environment initialized for: 'x64'`.

However, you can use powershell 64bit if you run the following script prior to cmake. You can either run it as required, or include it in your powershell profile. 

```powershell
# Based on the version of Visual Studio you have installed, the VC++ tools will be in a specific location.
# Using the "x64 Native Tools Command Prompt" shortcut referenced above, you can find the root folder by viewing the properties of the shortcut.

# e.g. Visual Studio 2015 path
# "${Env:PROGRAMFILES(X86)}\Microsoft Visual Studio 14.0\VC\"

# e.g. Vistual Studio 2019 path
# "${Env:PROGRAMFILES(X86)}\Microsoft Visual Studio\2019\Professional\VC\Auxiliary\Build\"

# Here we're using VS 2015.
$vcToolsPath = "${Env:PROGRAMFILES(X86)}\Microsoft Visual Studio 14.0\VC\"

# Shift context to VS
Push-Location $vcToolsPath

$architecture = 'x64' # amd64 and x64 resolve to the same path in vcvarsall.bat and are interchangeable.

# Have vcvarsall.bat load the 64 bit environment variables.
cmd /c "vcvarsall.bat $architecture&set" |
foreach {
	if ($_ -match "=") {
		$v = $_.split("="); set-item -force -path "ENV:\$($v[0])"  -value "$($v[1])"
	}
}
# Go back to where we were before.
Pop-Location
```

#### CMake Generator

Important configuration options

| Option | Value | Description |
| --- | --- | --- |
| `VCPKG_TARGET_TRIPLET` | `x64-windows-static-md`, `x64-linux`, `x64-osx` or whatever the pre-built environment used | The vcpkg triplet chosen when installing the dependencies via vcpkg |
| `CMAKE_TOOLCHAIN_FILE` | `<vcpkg_root>/scripts/buildsystems/vcpkg.cmake` | |
| `Ice_HOME` | `<vcpkg dir>/installed/x64-windows-static-md` | Required if you build with Ice (enabled by default) |
| `static` | `ON` | |

`<vcpkg dir>` is a placeholder for your prepared build environment vcpkg setup (the path to the vcpkg directory created by the get_dependency script).

For Linux the cmake invokation may be (using the default generator `make`)

```bash
cmake "-DVCPKG_TARGET_TRIPLET=x64-linux" "-Dstatic=ON" "-DCMAKE_TOOLCHAIN_FILE=<vcpkg dir>/scripts/buildsystems/vcpkg.cmake" "-DIce_HOME=<vcpkg dir>/installed/x64-linux" "-DCMAKE_BUILD_TYPE=Release" ..
```

For macOS the command may be
```bash
cmake "-DVCPKG_TARGET_TRIPLET=x64-osx" "-Dstatic=ON" "-DCMAKE_TOOLCHAIN_FILE=<vcpkg dir>/scripts/buildsystems/vcpkg.cmake" "-DIce_HOME=<vcpkg dir>/installed/x64-osx" "-DCMAKE_BUILD_TYPE=Release" ..
```

For Windows the command may be

```bash
cmake -G "NMake Makefiles" "-DVCPKG_TARGET_TRIPLET=x64-windows-static-md" "-Dstatic=ON" "-DCMAKE_TOOLCHAIN_FILE=<vcpkg_root>/scripts/buildsystems/vcpkg.cmake" "-DIce_HOME=<vcpkg_root>/installed/x64-windows-static-md" "-DCMAKE_BUILD_TYPE=Release" ..
```

Optionally you can use `-G "Ninja"` to use the [Ninja buildsystem](https://ninja-build.org/) (which probably has to be installed separately).
Especially on Windows this is recommended as the default `NMake Makefiles` only compile using a single thread (which takes quite a while).


### Customizing the build

There are various options that can be passed to cmake when performing step 6 from above. See [here](cmake_options.md) for details.


## Troubleshooting

See also our [general build FAQ](faq.md) if your problem is not listed below.

### sndfile not found

This is an error that is often encountered on 64bit Windows systems. The problem is usually that you have used the wrong developer command prompt and
therefore cmake is trying to build a 32bit version of Mumble. The `./Get-MumbleDeps.ps1` script automatically detects your system's architecture and
only builds the 64bit version of the libraries (including `sndfile`). cmake then tries to locate a 32bit version of the library and fails as only the
64bit version is present.

The solution is to use a x64 developer command prompt. You can see what kind of build cmake is performing at the top of the cmake output. For 64 bit
it should say `Architecture: 64bit`.

NOTE: If you initially have run cmake from the wrong prompt (32bit), then you'll have to delete all files in your `build` directory before running
cmake again from the new prompt. Otherwise cmake will not check the architecture again and proceed with the cached 32bit variant.

### CMake can't find library

If cmake doesn't find a library and you don't really know why this might be, you can use `-Ddebug-dependency-search=ON` when running cmake in order to
get a lot of debug information regarding the search for the needed dependencies. Chances are that this will shed some light on the topic.

### Unable to download from https://repo.msys2.org

This can happen if you're using a system that doesn't support TLS 1.3 (which https://repo.msys2.org requires) such as Windows 7. In this case the only
possible workaround is either to download the respective files manually using a brower that does support TLS 1.3 (e.g. Firefox) or to replace all
occurrences of `https://repo.msys2.org` in the vcpkg dir with `http://repo.msys2.org` and thereby forxing vcpkg to use the HTTP mirror instead. Note
though that this is inherently unsafer than using HTTPS.

A common error message for this scenario could be
```
-- Acquiring MSYS2...
-- Downloading https://sourceforge.net/projects/msys2/files/Base/x86_64/msys2-base-x86_64-20190524.tar.xz/download...
-- Downloading https://repo.msys2.org/msys/x86_64/msys2-keyring-r21.b39fb11-1-any.pkg.tar.xz...
-- Downloading https://repo.msys2.org/msys/x86_64/msys2-keyring-r21.b39fb11-1-any.pkg.tar.xz... Failed. Status: 35;"SSL connect error"
CMake Error at scripts/cmake/vcpkg_download_distfile.cmake:173 (message):

      Failed to download file.
      If you use a proxy, please set the HTTPS_PROXY and HTTP_PROXY environment
      variables to "https://user:password@your-proxy-ip-address:port/".
      Otherwise, please submit an issue at https://github.com/Microsoft/vcpkg/issue
```
Ref: https://github.com/microsoft/vcpkg/issues/13217

### `afxres.h` not found

When installing the dependency zlib via vcpkg, it may fail to locate the include file `afxres.h` (when building its mdnsresponder dependency).

```
dll.rc(10): fatal error RC1015: cannot open include file 'afxres.h'.
```

To resolve this, ensure MFC has been installed in your Visual Studio installation.
