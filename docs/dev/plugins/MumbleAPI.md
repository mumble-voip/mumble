# The Mumble-API

The Mumble-API is a set of function pointers that is given to the plugin during the [initialization process](PluginLifecycle.md#initialization) via
the `mumble_registerAPIFunctions` function in the [plugin-API](PluginAPI.md).


## How it works

As a plugin developer you don't have to worry about the implementation of these functions. They are implemented on Mumble's side. All you have to care
about is that certain functions exist and that you are free to call any of them.

In order to do so, you need to know the function signature which is defined in the Mumble-API header file. Functions in there are defined as e.g.
```cpp
mumble_error_t(PLUGIN_CALLING_CONVENTION *isConnectionSynchronized)(mumble_plugin_id_t callerID, mumble_connection_t connection, bool *synchronized);
```
The definition may seem a bit odd and intimidating at first glance but that is only because these functions need to be specified as [function
pointers](https://www.geeksforgeeks.org/function-pointer-in-c/). Here are a set of easy steps to (mentally) convert this notation into a (probably)
more familiar format:
1. Ignore `PLUGIN_CALLING_CONVENTION`. The [calling convention](https://en.wikipedia.org/wiki/Calling_convention) is nothing you have to worry about.
   This is handled for you by your compiler
2. Remove the first set of parenthesis and the star within them. What remains is the function name

Applying these steps to the above function yields the following definition:
```cpp
mumble_error_t isConnectionSynchronized(mumble_plugin_id_t callerID, mumble_connection_t connection, bool *synchronized);
```

Thus we can see that this is a function called `isConnectionSynchronized` that takes 3 parameters and returns a `mumble_error_t`. Therefore if we
assume that you have stored the Mumble-API in a variable named `mumbleAPI` in your plugin, you could call this function as
```cpp
mumble_error_t returnedError = mumbleAPI.isConnectionSynchronized(...);
```

## General function structure

Each function follows the general signature
```cpp
mumble_error_t myFunction(mumble_plugin_id_t callerID /* potentially more arguments */);
```

That means that each function _always_ returns an error code that indicates whether the API call was successful. How to handle this returned error
code is described in the [error handling section](#error-handling).

Furthermore the first parameter is _always_ the ID of the plugin that makes the API call. Your plugin's ID is given to you during the [initialization
process](PluginLifecycle.md#initialization) via the `mumble_init` function. You have to store this ID and use it every time you make a call to the
Mumble-API as otherwise your calls will be ignored by Mumble.

Given that the return value of Mumble-API functions are always occupied with the error code, all functions that query some sort of information use an
_out parameter_. That means that you pre-allocate a variable of the respective type and then pass a pointer to that variable to the API function. On
successful execution this function will then set the value of your variable through the given pointer.

A good example is obtaining the currently active server connection:
```cpp
mumble_connection_t activeConnection;
if (mumbleAPI.getActiveServerConnection(pluginID, &activeConnection) == MUMBLE_STATUS_OK) {
    // Do something with activeConnection
}
```


## Error handling

The error code returned by Mumble-API functions can (and often must) be used to determine whether the given function call was successful. If the error
code compares equal to `MUMBLE_STATUS_OK` (a macro defined in the `PluginComponents` header that is automatically included in the Mumble-API header),
then the API call was successful. **Every other value indicates failure**.

In case of an error, the error code will give information on the exact problem that was found. The possible error codes are defined in the
`Mumble_ErrorCode` enum (also defined in `PluginComponents`). These values can be used to check the error code against certain expected errors in the
error-handling branch of your code.

If you want to log the error in some way, it is recommended to use the `errorMessage` function (yet again defined in the `PluginComponents` header).
This function will return a String-representation of the respective error.

Given that errors are only reported in form of error codes, it is essential to always explicitly check for them. This is especially important when
these API function are used to query information. Accessing the variable passed as an out-parameter to a Mumble-API function may lead to undefined
behavior unless you have initialized that variable to a defined value before passing it to the function.

The only guarantee about variables given as out-parameters to API functions in case of an error is that they remain unchanged by the function.


## Multithreading

All API functions are synchronized. That means they can be called from an arbitrary thread. In order to achieve this synchronization, the functions
are executed in the main thread on Mumble's side. As the functions return a value, the caller has to wait for the execution to have finished in order
to continue. In other words: API function calls are **blocking**.

Therefore special care is to be taken as this can easily lead to deadlocks as soon as the plugin spawns a custom thread and uses the API functions
from it. An example of an easy way to run into such a deadlock is as follows:

Assume the plugin has spawned a worker thread that performs some kind of work asynchronously. Let's call that thread "W". Now an event occurs that
causes a event callback to be called in the plugin-API which usually happens in the already mentioned main thread (see also
[Threading](PluginAPI.md#threading)). Inside this event callback, the plugin notifies W to perform some sort of work and waits for that to finish. For
performing that work W needs to call an API function which blocks until the function can be executed in the main thread. Given that the main thread is
currently blocked by the plugin callback, a deadlock occurs.

That is to say: You must not wait for a job to finish asynchronously that might call an API function from the plugin-API.


## Header files

The Mumble API definition is contained in the [MumblePlugin.h](https://github.com/mumble-voip/mumble/blob/master/plugins/MumblePlugin.h) header. It
contains a list of all available functions as well as documentation for every function.

