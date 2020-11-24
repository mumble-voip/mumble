# Building on Linux

These are instructions for performing a regular build using the libraries that are shipped with your distribution. For instructions to build a static
version of Mumble, checkout [this file](build_static.md).

## Dependencies

In order to install the needed dependencies on Ubuntu, you have to run the following command:
```bash
sudo apt install \
  build-essential \
  cmake \
  pkg-config \
  qt5-default \
  qttools5-dev \
  qttools5-dev-tools \
  libqt5svg5-dev \
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
  libspeechd-dev \
  libavahi-compat-libdnssd-dev \
  libzeroc-ice-dev \
  g++-multilib
```

If you intend to include grpc-support for the Mumble server (murmur), you also have to install the following packages: `libgrpc++-dev` and
`protobuf-compiler-grpc`

The dependence on `g++-multilib` only applies if you are on a 64bit system and want to cross-compile overlay support for 32bit applications as well
(which is enabled by default). If you don't do this (`-Doverlay-xcompile=OFF` when invoking cmake), you also don't have to install `g++-multilib`.

If you are using a distribution that is not based on Ubuntu, you probably have to change the command accordingly.

You will need `cmake` **v3.15 or later**. If the version shipped by your distribution is not recent enough, you can install a recent one via the
[official PPA](https://apt.kitware.com/) by following the instructions on the linked page.


## Running cmake

It is recommended to perform a so-called "out-of-source-build". In order to do so, navigate to the root of the Mumble directory and then issue the
following commands:
1. `mkdir build` (Creates a build directory)
2. `cd build` (Switches into the build directory)
3. `cmake ..` (Actually runs cmake)

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
