# Creating a positional audio plugin

This is a description that covers how a plugin that wants to provide positional audio support in Mumble for a given game can be created using the
plugin framework that was introduced with Mumble 1.4.0. This is **not** a description of how one can try to interface or peek into games in order to
figure out the player's position. For this see our [guide on positional audio
fetching](https://www.mumble.info/documentation/developer/positional-audio/create-plugin/).

A positional audio plugin needs to fulfill the same requirements that any other Mumble plugin has to match. Therefore the [guide on writing a
plugin](CreatePlugin.md) is a must-read before continuing further.

Starting from the plugin that was written in the linked guide (minus the calls to th `log` API function), the following functions have to be
implemented in addition:
```c
uint32_t mumble_getFeatures() {
    return MUMBLE_FEATURE_POSITIONAL;
}

uint8_t mumble_initPositionalData(const char *const *programNames, const uint64_t *programPIDs, size_t programCount) {
    // Check if the supported game is in the list of programs and if yes
	// check if the position can be obtained from the program

	// If everything went well
    return MUMBLE_PDEC_OK;
	// Other potential return values are:
	// MUMBLE_PDEC_ERROR_TEMP -> The plugin can temporarily not deliver positional data
	// MUMBLE_PDEC_PERM -> Permanenet error. The plugin will never be able to deliver positional data
}

bool mumble_fetchPositionalData(float *avatarPos, float *avatarDir, float *avatarAxis, float *cameraPos, float *cameraDir,
                                float *cameraAxis, const char **context, const char **identity) {
    // fetch positional data and store it in the respective variables. All fields that can't be filled properly
	// have to be set to 0 or the empty String ""

	// If positional data could be fetched successfully
	return true;
	// otherwise return false
}

void mumble_shutdownPositionalData() {
	// Unlink the connection to the supported game
    // Perform potential clean-up code
}
```

The first function is necessary to let Mumble know that this plugin is generally capable of gathering positional data from games. Only then will the
init function be called every now and then, checking whether the plugin can currently link to the game's data. If so, the fetch function will be
called repeatedly in order to pass the gathered positional data to Mumble for use in positional audio.

After the plugin has lost link or Mumble (the user) decides that positional data from this plugin is no longer needed, the shutdown function is called
in which the plugin should terminate its connection to the game and clean up after itself.

