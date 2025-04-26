# Bundling a Mumble plugin

After you have built your plugin you will have a shared library for the respective target OS. While it is possible to just give users this shared
library and tell them to install them, it might get inconvenient at times. Especially if you want to support your plugin for multiple OS.

Therefore Mumble provides a way to package your plugin in a user-friendly and cross-platform way.

Mumble plugin bundles are essentially zip-files with a special contents structure and the file extension `.mumble_plugin` instead of `.zip`. Such a
bundle must contain a `manifest.xml` as a top-level entry in the archive. The manifest file is used to specify which plugin library (also bundled
within the archive) is to be used for which OS and architecture.

## The manifest file format

The manifest file _always_ has to be named `manifest.xml` (case-sensitive!) and it must use the `bundle` tag as the top-level element of the XML
structure. Furthermore the `bundle` opening tag needs to specify the `format` attribute which will determine the rules the rest of the XML tree has to
follow.

Unless otherwise noted the entire manifest file is case-sensitive (including paths specified in it)!

### v1.0.0

| Property | Value |
| -------- | ----- |
| `version` attribute | `1.0.0` |
| Available since: | Mumble v1.4.0 |

Inside  the `bundle` element there have to be the `assets`, `name` and `version` elements. `name` is just the name of your plugin (arbitrary String)
whereas `version` is the version of your plugin. Note that the version must be given in the format `<major>.<minor>.<patch>`.

Inside the `assets` element comes a list of one or more `plugin` elements. Each of these must specify the `os` and `arch` attributes for specifying
the operating system and architecture respectively. The contents of these elements is the path to the corresponding plugin library inside the archive
(relative to the `manifest.xml` file). Note that the paths _must not_ start with `./` and as path separator a single forward slash is to be used (even
on Windows).

The possible values for the `os` attribute are
| **OS** | **Attribute** |
| ------ | ------------- |
| Windows | `windows` |
| Linux | `linux` |
| macOS | `macos` |

and the possible values for the `arch` attribute are
| **Architecture** | **Attribute** |
| ---------------- | ------------- |
| x86 (32-bit) | `x86` |
| x86 (64-bit) | `x64`|

Putting this together a sample `manifest.xml` can look like this:

```xml
<?xml version="1.0" encoding="UTF-8"?>
<bundle version="1.0.0">
  <assets>
    <plugin os="windows" arch="x86">myPlugin.dll</plugin>
    <plugin os="windows" arch="x64">sub/myPlugin.dll</plugin>
    <plugin os="linux" arch="x64">sub/libmyPlugin.so</plugin>
  </assets>
  <name>MyPlugin</name>
  <version>1.0.0</version>
</bundle>
```

## Creating a plugin bundle

As written before, a plugin bundle essentially is just a zip file. The only restriction here is that it must not be created using the proprietary
`Deflate64` algorithm.

The steps to create a bundle file are as follows
1. Create an empty working directory
2. Copy the plugin libraries for the different OS and architectures into this directory (potentially in sub-directories)
3. Create a `manifest.xml` at the root of the working directory
4. Add all files and folders in your working directory (but not the working directory itself!) to a zip file
5. Change the file extension from `.zip` to `.mumble_plugin`

On Linux you can do this by using the `zip` command line tool like this:
```bash
zip my_plugin.mumble_plugin myPlugin.dll sub/myPlugin.dll sub/libmyPlugin.so manifest.xml
```

## Notes

- You **must not** add any additional files to this archive. Keep the contents of the created archive strictly to the parts mentioned above. It
  might be tempting to treat the plugin bundle as a regular zip file that can be filled with arbitrary contents but this is not allowed! Instead
  consider the plugin bundle a binary of its own that has to follow a strict format as outlined above.
- Plugin names **must not** contain version numbers. At least not when they are distributed to end users. This is because Mumble assumes that
  the new version of a plugin will use a shared library with the exact same name as the old one. If this assumption is violated, the update mechanism
  will not work properly causing the new and the old version of the plugin to be installed in parallel.

