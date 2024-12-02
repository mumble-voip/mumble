# Building on Linux

These are instructions for performing a regular build using the libraries that are shipped with your distribution. For instructions to build a static
version of Mumble, checkout [this file](build_static.md).

## Dependencies

<details>
  <summary><b>Ubuntu</b></summary>
In order to install the needed dependencies on Ubuntu, you have to run the following command (but see notes below the command):
  
```bash
sudo apt install \
  build-essential \
  cmake \
  pkgconf \
  qt6-base-dev \
  qt6-tools-dev \
  qt6-tools-dev-tools \
  libqt6svg6-dev \
  qt6-l10n-tools \
  libgl-dev \
  libboost-dev \
  libssl-dev \
  libprotobuf-dev \
  protobuf-compiler \
  libprotoc-dev \
  libcap-dev \
  libxi-dev \
  libasound2-dev \
  libogg-dev \
  libsndfile1-dev \
  libopus-dev \
  libspeechd-dev \
  libavahi-compat-libdnssd-dev \
  libxcb-xinerama0 \
  libzeroc-ice-dev \
  libpoco-dev \
  g++-multilib
```

The dependence on `g++-multilib` only applies if you are on a 64bit system and want to cross-compile overlay support for 32bit applications as well
(which is enabled by default). If you don't do this (`-Doverlay-xcompile=OFF` when invoking cmake), you also don't have to install `g++-multilib`.

You will need `cmake` **v3.15 or later**. If the version shipped by your distribution is not recent enough, you can install a recent one via the
[official PPA](https://apt.kitware.com/) or from the [linked page](https://cmake.org/download/).
</details>

<details>
  <summary><b>Debian (Stable)</b></summary>
In order to install the needed dependencies on Debian, you may
run the following command.

```bash
sudo apt install \
  build-essential \
  cmake \
  g++-multilib \
  libasound2-dev \
  libavahi-compat-libdnssd-dev \
  libboost-dev \
  libcap-dev \
  libgl-dev \
  libmsgsl-dev \
  libogg-dev \
  libpoco-dev \
  libprotobuf-dev \
  libprotoc-dev \
  libqt6svg6-dev \
  libsndfile1-dev \
  libspeechd-dev \
  libspeexdsp-dev \
  libssl-dev \
  libxcb-xinerama0 \
  libxi-dev \
  libzeroc-ice-dev \
  nlohmann-json3-dev \
  pkgconf \
  protobuf-compiler \
  qt6-base-dev \
  qt6-base-dev-tools \
  qt6-l10n-tools \
  qt6-tools-dev \
  qt6-tools-dev-tools \
  qtchooser
```

1. There is a problem with Opus, causing crashes on some systems. Disabling the bundled opus version when running cmake might be necessary for now:
   `cmake -Dbundled-opus=OFF ..`
2. _Also, see Ubuntu notes, which explain some things relevant to Debian builds._

</details>

<details>
  <summary><b>CentOS 9 Stream</b></summary>
 In order to install the needed dependencies on Cent OS 9, you have to run the following command:
  
```bash
sudo dnf -y install epel-release
sudo dnf config-manager --set-enabled PowerTools
sudo dnf group install "Development Tools"
sudo dnf install https://zeroc.com/download/ice/3.7/el8/ice-repo-3.7.el8.noarch.rpm
sudo dnf install libice-c++-devel libice3.7-c++
sudo dnf install \
cmake \
qt6-devel \
qt6-qtsvg-devel  \
openssl-devel \
protobuf-devel \
libsndfile-devel \
libXi-devel \
libXext-devel \
speech-dispatcher-devel \
avahi-compat-libdns_sd-devel \
alsa-lib-devel \
libICE-devel \
libogg-devel \
boost-devel \
libcap-devel \
poco-devel \
gcc-toolset-9-gcc-c++ 
```

</details>

<details>
  <summary><b>OpenSUSE Tumbleweed</b></summary>
In order to install the needed dependencies on OpenSUSE Tumbleweed (not Leap),
you have to run the following commands:

```bash
sudo zypper install -t pattern devel_basis devel_qt6
sudo zypper install \
  libopenssl-devel \
  protobuf-devel \
  poco-devel \
  libsndfile-devel \
  libXi-devel \
  libspeechd-devel \
  avahi-compat-mDNSResponder-devel \
  alsa-devel \
  libcap-devel \
  gcc-c++-32bit
```

There is no official package for Ice on OpenSUSE Tumbleweed.
This means you need to generate the files like so:
`cmake -Dice=OFF ..`
</details>

If you are using any other distribution that is not based on one of the distros listed above, you probably have to adapt the commands to your distro manually.

## Running cmake

It is recommended to perform a so-called "out-of-source-build". In order to do so, navigate to the root of the Mumble directory and run `cmake -B build`

This will cause cmake to create the necessary build files for you. If you want to customize your build, you can pass special flags to cmake in step 3.
For all available build options, have a look [here](cmake_options.md).

E.g. if you only want to build the server, use `cmake -Dclient=OFF ..`.


## Building

By default cmake will create Makefiles if run on Linux, so in order to build, you can simply run `make` in the build directory. If you want to
parallelize the build (to speed things up), you can use `make -j $(nproc)`.

If you want to stay independent of the type of the generated build files, you should issue `cmake --build .` (from the build directory) in order to
start the compilation. If you want to parallelize the build, use `cmake --build . -j <jobs>` where `<jobs>` is the amount of parallel jobs to be run
concurrently. A good choice usually is `<jobs> = $(nproc)`.


## FAQ

See the general [build-FAQ](faq.md).
