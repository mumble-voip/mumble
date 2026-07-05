# Mumble Dependency Provider

[Dependency Providers](https://cmake.org/cmake/help/latest/guide/using-dependencies/index.html#dependency-providers) are CMake's way of customizing
how dependencies are discovered (or, indeed, _provided_) during configuration.

We use this mechanism to provide a "download and build missing dependencies automatically" semantic for a subset of Mumble's dependencies. The
implementation of our provider lives under [cmake/mumble_dependency_provider.cmake](/mumble_dependency_provider.cmake).


## Using The Provider

By default the provider is _not used_, implying that all dependencies must be externally provided (e.g. via
[vcpkg](https://github.com/microsoft/vcpkg/) or by being installed on your system). In order to enable the dependency provider, you have to specify
the [CMAKE_PROJECT_TOP_LEVEL_INCLUDES](https://cmake.org/cmake/help/latest/variable/CMAKE_PROJECT_TOP_LEVEL_INCLUDES.html) CMake variable and point it
to the dependency provider implementation. Hence, your CMake invocation should look something like
```bash
cmake -DCMAKE_PROJECT_TOP_LEVEL_INCLUDES="cmake/mumble_dependency_provider.cmake" -S . -B build
```
(assuming CMake is being run from this repository's root).


### Customization

The following CMake variables can be set in order to influence and fine-tune the behavior of the dependency provider. `<name>` is a placeholder for
the name of a specific dependency (uppercase) in which case the given option only applies to this particular dependency. Options without `<name>` take
global effect. However, dependency-specific options always take precedence over global ones.

- `MUMBLE_DEP_SKIP_FIND_PACKAGE` or `MUMBLE_DEP_<name>_SKIP_FIND_PACKAGE`: Whether to use a basic CMake `find_package` call first to see whether we
  can find the dependency in this way.
- `MUMBLE_DEP_SKIP_PKGCONF` or `MUMBLE_DEP_<name>_SKIP_PKGCONF`: Whether to use `pkgconf` to locate dependencies.
- `MUMBLE_DEP_SKIP_FETCHCONTENT` or `MUMBLE_DEP_<name>_SKIP_FETCHCONTENT`: Whether to use
  [FetchContent](https://cmake.org/cmake/help/latest/module/FetchContent.html) for automatically fetching dependencies.

The normal order of operation for any given dependency is
1. Attempt to locate it via a call to `find_package`
2. If not found, try locate via `pkgconf`
3. If not found, try fetching and building the dependency via `FetchContent` (if supported)

