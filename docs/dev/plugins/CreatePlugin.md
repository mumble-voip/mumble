# Creating Mumble plugins

In the following the necessary steps to create a working Mumble plugin are outlined. These instructions cover the plain C API. If you are using a
[language binding](LanguageBindings.md) for a different programming language, different steps are usually required. Please refer to the binding's
documentation for that.

In the spirit of the classical [Hello world program](https://en.wikipedia.org/wiki/%22Hello,_World!%22_program), this guide will step you through the
creation of a "Hello Mumble" plugin.

The source code of the example plugin described here can be found in [this repository](https://github.com/mumble-voip/mumble-plugin-template). This
also intended to be used as the basis for everyone that wants to start writing a plugin without having to worry about the boilerplate themselves.


## Preparations

What you need for creating a plugin is
- A working C compiler. It does not matter which one
- The Mumble plugin header file: [MumblePlugin.h](https://github.com/mumble-voip/mumble/blob/master/plugins/MumblePlugin.h)

Although not strictly required, it usually is handy to use a build system for managing your plugin project. In this guide we'll use
[cmake](https://cmake.org/). If you have never used cmake before, have a look at [this short guide](https://stackoverflow.com/a/26007567).

All in all the following file structure is assumed to be present on your device:
```
.
├── include
│   └── MumblePlugin.h
├── CMakeLists.txt
└── plugin.c
```

The headers in `include` are the ones listed above and the other files will be populated during this guide.


## CMakeLists.txt

The `CMakeLists.txt` file is our cmake project file that tells cmake what we expect it to do.

In it, you have to put the following:
```cmake
cmake_minimum_required(VERSION 3.15)

project(MumblePlugin
	VERSION "1.0.0"
	DESCRIPTION "Minimal Mumble plugin"
	LANGUAGES "C"
)

add_library(plugin
	SHARED
		plugin.c
)

target_include_directories(plugin
	PUBLIC "${CMAKE_SOURCE_DIR}/include/"
)
```

If you want to understand the details it would be best if you searched for a proper cmake tutorial. The gist of it is that we tell cmake that we want
to build a shared library from the source file `plugin.c` and that everything in the `include` directory should be includable from it.


## Writing the plugin

Now that the boilerplate is out of the way, we can start writing the actual plugin. This will be done in the `plugin.c` source file.

The first thing you should do is to include `MumblePlugin.h`. Furthermore we'll need a few more C headers that we'll include as well:
```c
#include "MumblePlugin.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
```

Furthermore every plugin needs a way to store at least the Mumble-API and its own ID. In C this can be done using global variables. Therefore go ahead
and create the respective variables in the global namespace:
```c
MumbleAPI mumbleAPI;
mumble_plugin_id_t ownID;
```

Both data types are defined by the API via the included headers.

As stated in the docs of the [plugin-API](PluginAPI.md) there are several functions that you must implement in your plugin. The first of these is
`mumble_init`:
```c
mumble_error_t mumble_init(mumble_plugin_id_t pluginID) {
	ownID = pluginID;

	if (mumbleAPI.log(ownID, "Hello Mumble") != MUMBLE_STATUS_OK) {
		// Logging failed -> usually you'd probably want to log things like this in your plugin's
		// logging system (if there is any)
	}

	return MUMBLE_STATUS_OK;
}
```

As you can see the function takes the plugin's ID as a parameter, so make sure you store that in our respective variable. As you can see our Hello
Mumble plugin will use the Mumble-API to log something in Mumble's console. Note that it is safe to access the API here already due to the rules for a
[plugin's initialization processs](PluginLifecycle.md#initialization).

The final step is to return `MUMBLE_STATUS_OK` in order to let Mumble know that the plugin's initialization was successfull.

The next function to be implement is `mumble_shutdown` which is structured very similarly to `mumble_init`:
```c
void mumble_shutdown() {
	if (mumbleAPI.log(ownID, "Goodbye Mumble") != MUMBLE_STATUS_OK) {
		// Logging failed -> usually you'd probably want to log things like this in your plugin's
		// logging system (if there is any)
	}
}
```

Next up is `mumble_getName`:
```c
struct MumbleStringWrapper mumble_getName() {
	static const char *name = "HelloMumble";

	struct MumbleStringWrapper wrapper;
	wrapper.data = name;
	wrapper.size = strlen(name);
	wrapper.needsReleasing = false;

	return wrapper;
}
```

If you want to read details about why a `MumbleStringWrapper` is required, have a look at the [resource management docs](ResourceManagement.md).

The implementation of `mumble_getAPIVersion` is almost trivial as long as you are sticking to the API version the headers you are using belong to
(which is strongly recommended). In that case the constant `MUMBLE_PLUGIN_API_VERSION` will hold the correct version and all you have to do is to
return it from this function:
```c
mumble_version_t mumble_getAPIVersion() {
	// This constant will always hold the API version  that fits the included header files
	return MUMBLE_PLUGIN_API_VERSION;
}
```

The function for receiving the Mumble-API function is implemented as follows:
```c
void mumble_registerAPIFunctions(void *apiStruct) {
	// Provided mumble_getAPIVersion returns MUMBLE_PLUGIN_API_VERSION, this cast will make sure
	// that the passed pointer will be cast to the proper type
	mumbleAPI = MUMBLE_API_CAST(apiStruct);
}
```

Note that the function takes a `void *` and thus has to cast this pointer to the correct type itself. In the case that you are using the API version
corresponding to the included headers (again: as you should), this is easy thanks to the pre-defined macro `MUMBLE_API_CAST`. It will automatically
cast the pointer to the correct API type.

The final function that needs to be implemented is `mumble_releaseResource`. Note that because our `MumbleStringWrapper` used above specifies
`needsReleasing = false`, this function will never actually be called (unless you implement other functions that do return resources that need
releasing - see [Resource management](ResourceManagement.md)) and therefore a dummy implementation is enough for our purposes:
```c
void mumble_releaseResource(const void *pointer) {
	// As we never pass a resource to Mumble that needs releasing, this function should never
	// get called
	printf("Called mumble_releaseResource but expected that this never gets called -> Aborting");
	abort();
}
```

And that's it. This is all that is strictly required in order to get a working plugin.

Note however that you will probably also want to implement the following functions (though from a functional point of view that is completely
optional):
- `mumble_getVersion`
- `mumble_getAuthor`
- `mumble_getDescription`

All available functions are listed and documented in the [plugin-API headers](PluginAPI.md#header-files).
