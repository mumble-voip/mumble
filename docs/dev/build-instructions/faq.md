# Build FAQ

### How can I specify the build type

In order to set the build type (Debug vs Release), you have to use the `CMAKE_BUILD_TYPE` variable when invoking cmake like so: `cmake
-DCMAKE_BUILD_TYPE=Release`. The supported types currently are: "Debug" and "Release".

Note that this only works for *single-config* generators. If you are using a *multi-config* generator (e.g. visual studio projects), the steps
mentioned above won't have any effect. Instead, you'll have to choose the config type when invoking the build itself. This works by using e.g. `cmake
--build . --config Release`.
