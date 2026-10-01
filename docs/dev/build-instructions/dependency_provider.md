# Mumble Dependency Provider

[Dependency Providers](https://cmake.org/cmake/help/latest/guide/using-dependencies/index.html#dependency-providers) are CMake's way of customizing
how dependencies are discovered (or, indeed, _provided_) during configuration.

We use this mechanism to provide a "download and build missing dependencies automatically" semantic for a subset of Mumble's dependencies. The
implementation of our provider lives under [cmake/mumble_dependency_provider.cmake](../../../cmake/mumble_dependency_provider.cmake).


## Using The Provider

By default the provider is _not used_, implying that all dependencies must be externally provided (e.g. via
[vcpkg](https://github.com/microsoft/vcpkg/) or by being installed on your system). In order to enable the dependency provider, you have to specify
the [CMAKE_PROJECT_TOP_LEVEL_INCLUDES](https://cmake.org/cmake/help/latest/variable/CMAKE_PROJECT_TOP_LEVEL_INCLUDES.html) CMake variable and point it
to the dependency provider implementation. Hence, your CMake invocation should look something like
```bash
cmake -DCMAKE_PROJECT_TOP_LEVEL_INCLUDES="cmake/mumble_dependency_provider.cmake" -S . -B build
```
(assuming CMake is being run from this repository's root).

Note: It seems to be impossible to use relative paths for this option that refer to a directory higher up, i.e. a path containing a `../` element.
Instead, you will have to use an absolute path in such cases.


### Customization

The following CMake variables can be set in order to influence and fine-tune the behavior of the dependency provider. `<name>` is a placeholder for
the name of a specific dependency (as passed to `find_package` but in uppercase) in which case the given option only applies to this particular
dependency. Options without `<name>` take global effect. However, dependency-specific options always take precedence over global ones.

- `MUMBLE_DEP_SKIP_FIND_PACKAGE` or `MUMBLE_DEP_<name>_SKIP_FIND_PACKAGE`: Skip trying to locate the dependency via a regular `find_package` call.
- `MUMBLE_DEP_SKIP_FETCHCONTENT` or `MUMBLE_DEP_<name>_SKIP_FETCHCONTENT`: Skip fetching and building the dependency via
  [FetchContent](https://cmake.org/cmake/help/latest/module/FetchContent.html).
- `MUMBLE_DEP_<name>_FIND_PACKAGE_ARGS`: Replaces the arguments (e.g. version and components) with which `find_package` is called for this dependency.
- `MUMBLE_DEP_<name>_FIND_PACKAGE_EXTRA_ARGS`: Additional arguments that are appended to the `find_package` call for this dependency.
- `MUMBLE_DEP_DEBUG_MODE`: Don't silence the `find_package` calls made by the provider.

The normal order of operation for any given dependency is
1. Attempt to locate it via a call to `find_package`
2. If not found, try fetching and building the dependency via `FetchContent`

Only dependencies that specify a `tracked_version` in [dependencies.json](../../../dependencies.json) can be fetched.

Note that if the provider fails to provide a dependency, CMake falls back to its regular `find_package` implementation.

