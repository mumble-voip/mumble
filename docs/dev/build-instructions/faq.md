# Build FAQ

### How can I specify the build type

In order to set the build type (Debug vs Release), you have to use the `CMAKE_BUILD_TYPE` variable when invoking cmake like so: `cmake
-DCMAKE_BUILD_TYPE=Release`. The supported types currently are: "Debug" and "Release".

Note that this only works for *single-config* generators. If you are using a *multi-config* generator (e.g. visual studio projects), the steps
mentioned above won't have any effect. Instead, you'll have to choose the config type when invoking the build itself. This works by using e.g. `cmake
--build . --config Release`.

### CMake selects wrong OpenSSL version

If the automatic search process of cmake doesn't result in the proper version of OpenSSL being selected (e.g. an older/incompatible system version
could be chosen over the manually installed one), you always have the option of explicitly telling cmake where to look for OpenSSL via the
`OPENSSL_ROOT_DIR` option.

If for instance you have installed OpenSSL to `/usr/local/opt/openssl`, you can invoke cmake like this:
```
cmake -DOPENSSL_ROOT_DIR=/usr/local/opt/openssl ..
```

