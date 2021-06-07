# Bundling a Mumble plugin

After you have built your plugin you will have a shared library for the respective target OS. While it is possible to just give users this shared
library and tell them to install them, it might get inconvenient at times. Especially if you want to support your plugin for multiple OS.

Therefore Mumble provides a way to package your plugin in a user-friendly and cross-platform way.

It works by putting the shared libraries for the different platforms into a single zip-file and then changing the archive's file extension from `.zip`
to `.mumble_plugin`.

The archive must not contain anything besides the plugin libraries and these should follow the following naming conventions:
- The shared library should contain the name of the plugin
- followed by the OS specifier
- followed by the architecture specifier

The OS specifier may be one of the following
| **OS** | **Specifier** |
| ------ | ------------- |
| Windows | `_win` |
| Linux   | `_linux` |
| macOS   | `_macos` |

and the architecture specifier is one of
| **Architecture** | **Specifier** |
| ---------------- | ------------- |
| x86 (64-bit) | `_x86_64` |
| x86 (32-bit) | `_i386` |
| ARM (v5)     | `_armv5` |
| ARM (v6)     | `_armv6` |
| ARM (v7)     | `_armv7` |

Therefore a plugin library may be named for instance
```
myPlugin_linux_x86_64.so
```
or
```
myPlugin_win_i386.dll
```

Platform-specific prefixes such as `lib` on Unix-systems are allowed as well. Therefore
```
libmyPlugin_linux_x86_64.so
```
is completely equivalent to the example given above.

Note that plugin names **must not** contain version numbers. At least not when they are distributed to end users. This is because Mumble assumes that
the new version of a plugin will use a shared library with the exact same name as the old one. If this assumption is violated, the update mechanism
will not work properly causing the new and the old version of the plugin to be installed in parallel.

