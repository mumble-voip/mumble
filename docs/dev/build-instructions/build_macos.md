# Building on macOS

These instructions are for performing a regular build of Mumble that will only run on systems that have the necessary libraries installed on them. For
building a static version, checkout [this file](build_static.md).

## Dependencies

On macOS, you can use [homebrew](https://brew.sh/) to install the needed packages. If you don't have it installed already, you can follow the
instruction on their official website to install homebrew itself.

Once homebrew is installed, you can run the following command to install all required packages:
```bash
brew update && brew install \
  cmake \
  pkg-config \
  qt5 \
  boost \
  libogg \
  libvorbis \
  flac \
  libsndfile \
  protobuf \
  openssl \
  poco \
  ice
```


## Running cmake

It is recommended to perform a so-called "out-of-source-build". In order to do so, navigate to the root of the Mumble directory and the issue the
following commands:
1. `mkdir build` (Creates a build directory)
2. `cd build` (Switches into the build directory)
3. `cmake ..` (Actually runs cmake)

This will cause cmake to create the necessary build files for you. If you want to customize your build, you can pass special flags to cmake in step 3.
For all available build options, have a look [here](cmake_options.md).

E.g. if you only want to build the server, use `cmake -Dclient=OFF ..`.


## Building

Once cmake has been run, you can issue `cmake --build .` from the build directory in order to actually start compiling the sources. If you want to
parallelize the build, use `cmake --build . -j <jobs>` where `<jobs>` is the amount of parallel jobs to be run concurrently.


## FAQ

See the general [build-FAQ](faq.md).


### CMake chooses Apple's SSL library

It can happen that cmake will find Apple's own SSL library that comes pre-installed on your system. This is usually incompatible with Mumble though
and you'll usually get errors about undefined OpenSSL symbols during link-time:
```
ld: symbol(s) not found
```

You can circumvent this problem by pointing cmake to the OpenSSL version you installed following the instructions from above. For how to do this,
please refer to [our build-FAQ](faq.md#cmake-selects-wrong-openssl-version).
