# The Mumble plugin framework

Mumble supports loading plugins that provide extra functionality if enabled. Users can choose which plugins to install and which to enable at which
point in time.

One example use of plugins is the connection to a game that is running. In this case the plugin is responsible for fetching the current player
position from the game (usually by inspecting the game's in-memory representation) and providing this information to Mumble in order to make the
positional audio feature work.

This kind of plugin has existed for quite a while but only from 1.4.0 onwards does Mumble support general purpose plugins. That means that plugins can
do basically everything (provided the API supports it). For instance you could program a plugin that always sets your comment to "Bananaaaa" when you
connect to a server or one that automatically moves you into a channel named "AFK" once you deafen yourself.

## Contents

- [The very basics](#the-basics)
- [How to write your own plugin](CreatePlugin.md)
- [How to write a positional audio plugin](PositionalAudioPlugin.md)
- [The Mumble-API](MumbleAPI.md)
- [The plugin-API](PluginAPI.md)
- [Resource management in plugins](ResourceManagement.md)
- [Lifecycle of a plugin](PluginLifecycle.md)
- [Bindings for languages other than C](LanguageBindings.md)
- [Debugging your plugin](Debugging.md)
- [Plugin (framework) versioning](Versioning.md)
- [Bundling your plugin for the end user](Bundling.md)


## The basics

At its core a plugin is nothing more than a shared library that implements a set of pre-defined functions. This set of pre-defined functions is the
so-called [plugin-API](PluginAPI.md). It consists of a few functions that must be implemented by every plugin and even more functions that can be
implemented as needed.

The plugin-API is written in plain [C](https://en.wikipedia.org/wiki/C_(programming_language)). Thus any plugin must be written in C. There are also
[bindings for other languages](LanguageBindings.md).

Using C as the base language for the API has several advantages:
- Binary compatibility: It does not matter which compiler was used to create the plugin. This lifts a restriction that plugins prior to 1.4.0 had: You
  could only use those plugins that were compiled with the _exact_ same compiler that was used to compile Mumble.
- C is widely used as the smallest denominator of a variety of languages. This allows for relatively easy inter-operability between other languages
  which is the pre-condition for being able to create language-bindings for other programming languages

Of course using C also has a few drawbacks (lack of high-level programming features) but these can mostly be overcome by resorting to one of the
available [language bindings](LanguageBindings.md).

The plugin-API is the part that allows communication from Mumble to the plugin. Every time an event occurs, Mumble will call the corresponding
function in the plugin which can then act on it. However with only a one-way-communication there wouldn't actually be all that many interesting things
that could be done.

That's why there is also a second part to the communication scheme that allows the plugin to talk back to Mumble. This is done via the
[Mumble-API](MumbleAPI.md). The Mumble-API is a collection of functions that Mumble provides to every plugin during the initialization process.

These functions can be called by the plugin to query certain properties (e.g. a list of users on a given server) and to ask Mumble to perform certain
actions (e.g. move user X to channel Y).

Therefore the typical "working formula" for plugins is this: Mumble lets the plugin know about certain events (e.g. connected to a server) by calling
the respective function in the plugin-API. The plugin then has the possibility to act on these events by causing something to happen inside Mumble
(e.g. move to channel X) by using the Mumble-API.

