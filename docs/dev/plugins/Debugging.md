# Debugging a Mumble plugin

Generally you have to resort to the standard debugging techniques when it comes to debugging the functionality inside your plugin. If however you want
to find out why Mumble doesn't recognize any given function in your plugin or you think that the issue may lie within Mumble somewhere, you have to
[build Mumble](../build-instructions/README.md) yourself.

When doing so, you activate the `plugin-debug` and/or `plugin-callback-debug` options when invoking cmake:
```bash
cmake -Dplugin-debug=ON -Dplugin-callback-debug=ON ..
```

The first option (`plugin-debug`) will cause Mumble to be verbose about the function resolution process in plugins. That means it will report which
functions of the plugin interface Mumble recognizes as implemented in any given plugin.

The second option will cause Mumble to print a log message to the console every time it encounters an event that is passed to interested plugins via
the respective callback function. Note that this output can be excessively verbose due to the audio-related events.

