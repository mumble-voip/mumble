# Mumble plugin lifecycle

## Initialization

When the user chooses to activate an installed plugin, the following functions inside that plugin will be called (if implemented):
1. `mumble_setMumbleInfo` - Tells the plugin about which version of Mumble is about to load it
2. `mumble_getAPIVersion` - Ask the plugin which API version it is using
3. `mumble_registerAPIFunctions` - Provides the [Mumble-API](MumbleAPI.md) functions to the plugin
4. `mumble_init` - Actually initialize the plugin

Most notably: The plugin will _always_ receive the Mumble-API function _before_  `mumble_init` is called. Therefore the API can already be used in
`mumble_init`, if required.


## Shutdown

When deactivating a plugin, `mumble_shutdown` will be called. Note that during this shutdown method it is still safe to use the Mumble-API. As soon as
this function returns though, the Mumble-API that was given to the plugin must be considered invalid and must no longer be used. This is important if
your plugin has spawned a separate thread that might end asynchronously.

