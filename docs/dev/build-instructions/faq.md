# Build FAQ

### How to speed up the compilation

Mumble supports being compiled as a [unity build](https://en.wikipedia.org/wiki/Unity_build), if you are using cmake 3.16 or newer. To use this
feature, use `-DCMAKE_UNITY_BUILD=ON` option when invoking cmake.

Don't use this option when also using `-DCMAKE_EXPORT_COMPILE_COMMANDS=ON` though as the unity build will render the produced compilation database
useless.

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


### Build a specific version or commit

#### Version

Use tags to build a specific release version of Mumble.
You can find the `tag`-list [here](https://github.com/mumble-voip/mumble/tags) or by using the command `git tag` inside a cloned repository.

Use:
```
git clone -b [tag] https://github.com/mumble-voip/mumble.git
```

For example (for version 1.3.3): 
```
git clone -b 1.3.3 https://github.com/mumble-voip/mumble.git
```

**or inside an already cloned repository:**

```
git checkout [tag]
```

or (if you want to create a new branch):
```
git checkout -b [new-branch-name] [tag]
```

#### Commit

Use commit hashes to build a specific development state of Mumble.

You can find the list of commits [here (for master branch)](https://github.com/mumble-voip/mumble/commits/master) or by using the command `git log` inside a cloned repository.

Go into the cloned repository and use:

```
git checkout [commit_sha]
```

or (if you want to create a new branch):
```
git checkout -b [new-branch-name] [commit_sha]
```
