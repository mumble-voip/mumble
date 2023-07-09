# Incrementing the Mumble API version

## Overview
[Plugins](/docs/dev/plugins/) are used to extend the Mumble client with custom functionality. They are compiled against the
[MumblePlugin.h](/plugins/MumblePlugin.h) header, which contains all required definitions (and utilities) that a plugin needs to "do its thing".
Among these definitions, are the functions that are part of the [Mumble API](/docs/dev/plugins/MumbleAPI.md) (search for `Mumble API`). If you modify
these definitions, you'll have to update the API's version number.

Which version to increment, depends on what kind of change you have made:
- The change is only a minor correction (e.g. fixing a typo in a parameter name or fixing a bug in Mumble's implementation of an API function).
  Increase the value of `MUMBLE_PLUGIN_API_PATCH_MACRO` by one.
- The change breaks existing plugins (e.g. adding a new parameter to an existing function or adding a new function altogether). Increase the value of
  `MUMBLE_PLUGIN_API_MINOR_MACRO` by one and adapt the API code in Mumble (see below).
- The change is not compatible with the existing framework at all. You should never, ever do this without thorough discussions with the project
  maintainers. As for the versioning: a custom procedure will have to be agreed on.


## Adapting the Mumble logic

If the API's minor or major version number have been increased, you will have to adapt Mumble's code to make sure that it will be able to serve
individual plugins different versions of the API. For that, you will have to adapt the code inside the `init()` function of the `Plugin` class. More
specifically, you will have to add another branch to the if-statement that selects the requested API struct.

The required `API::getMumbleAPI_v_*` functions are implemented inside the `API.h` and `API.cpp` files. Simply follow the already present examples and
create a specific implementation for the new version. Pointers to functions that have not changed can be used as before.

If you have added a new parameter to an existing function, calling the old function (which should still be available in most cases) should internally
call the new implementation, with that parameter set to a sensible default. This way, all API function calls will end up using the same implementation
on Mumble's side (which reduces maintenance work).

