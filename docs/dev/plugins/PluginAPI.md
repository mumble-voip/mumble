# The plugin-API

The plugin-API consist of a collection of C functions that can be implemented by a plugin. A hand full of these must be implemented by every plugin in
order for it to get recognized as such by Mumble.

Generally all plugin-API functions are only called if the plugin is currently active. That means that it is save to assume that these function calls
happen between the call to `mumble_init` and `mumble_shutdown`. This is important if the plugin allocates some resources to use during its lifetime
(allocated in the init function and deallocated again in the shutdown function) as these can then always be safely accessed in these functions.

There are a few exceptions to this rule though. Certain functions are supposed to (and will be) called even if the plugin is not active. In these
functions you must not make any assumptions about certain resources in your plugin being initialized. Furthermore you must not allocate resources in
these functions as the plugin might never get the chance to clean them up again (thereby causing a memory leak).

An example of such a function is `mumble_getName`.

All functions that fall in this category are documented with a special note pointing this behavior out in the function's documentation.


## Mandatory functions

These functions must be implemented by every plugin. All other functions in the plugin-API are optional and whether or not to implement them is up to
the plugin developer.

| **Name** | **Description** |
| -------- | --------------- |
| `mumble_init` | Used to initialize the plugin upon starting it |
| `mumble_shutdown` | Called when the plugin gets shut down. This is required for plugins to perform any sort of clean-up |
| `mumble_getName` | Gets the name of the plugin |
| `mumble_getAPIVersion` | The version of the API this plugin is using. This simultaneously determines the version of the plugin-API as well as the Mumble-API |
| `mumble_registerAPIFunctions` | Used to provide the plugin with the [Mumble-API](MumbleAPI.md) functions |
| `mumble_releaseResource` | Used to ask the plugin to free any resource it has allocated for passing them to Mumble |


## Threading

Unless otherwise noted in the function's description, all functions are called from the same thread.


## Header files

The plugin function definitions are contained in the [MumblePlugin.h](https://github.com/mumble-voip/mumble/blob/master/plugins/MumblePlugin.h) header. It
contains a list of all available functions as well as documentation for every function.

