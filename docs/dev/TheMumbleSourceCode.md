# Introduction to the Mumble source code

Hello fellow programmer. If you are reading this specific piece of documentation, you are probably about to (or consider to) make a contribution to
Mumble. Or maybe you just want to patch something for your own purpose. In any case: this is aimed at people who are unfamiliar with the general
layout and concepts of the Mumble source code.

While this documentation is aimed at providing a good entry point for picking up development with the Mumble source code, it naturally can't cover it
exhaustively. Furthermore, just as any documentation, it is prone to becoming stale at some point. Therefore the golden rule is: If you don't
understand something or have a question otherwise, feel free to ask. We will be happy to help!

For all development-related questions, feel free to join our public Matrix development channel at https://matrix.to/#/#mumble-dev:matrix.org. Creating
an issue with your question here on GitHub is also fine.


## Source tree layout

When cloning the repo, the source tree should look something like this:
```
<repo root>
├── 3rdparty
├── 3rdPartyLicenses
├── cmake
│   └── FindModules
├── docs
│   ├── additional-readmes
│   ├── dev
│   └── media
├── g15helper
├── helpers
├── icons
├── installer
├── macx
├── man
├── overlay
├── overlay_gl
├── overlay_winx64
├── plugins
├── samples
├── screenshots
├── scripts
├── src
│   ├── crypto
│   ├── mumble
│   ├── murmur
│   └── tests
└── themes
    └── Default
```

The `3rdparty*` directories contain external libraries that we make use of (generally as git submodules) and their corresponding licenses. Unless you
are planning on adding a new external library into the code base, these directories are probably of virtual zero interest for you. Unless you forget
to initialize the git submodules before attempting to compile Mumble, in which case you will experience build errors. Thus, execute `git submodule
update --init` and forget about these directories.

The `cmake` directory contains a few cmake scripts that are needed to make the build work in the way it does. Unless you are trying to debug a build
issue (or extend the build system), you can probably ignore this directory.

Obviously you are already aware of the `docs` directory (you are reading this file, which lives in there). It contains mostly development related
documentation. The more extended user documentation is available on our website. The most relevant documentation for you is probably within the `dev` (development) subdirectory.

`g15helper` and `helper` will generally not concern you. We could probably find a better place for these, but at the moment they are where they are…

`icons` is pretty much what you would expect, given its name. The same can be said about `installer` which contains all files related to building our
Windows installer. If you want to tinker with that, you have to know that we are using [WixSharp](https://github.com/oleg-shilo/wixsharp) for managing
it.

`macx` contains some macOS specific files (mostly overlay-related and scripts used for release mangement), whereas `man` contains the man entries
(used on Windows) for the Mumble client and server.

The `overlay*` directories contain the implementation for the Mumble overlay for the various platforms. We will not go into detail for those here, as
they aren't really part of the main Mumble source (I mean they are, but yet they aren't).

If you are into plugin development or you want to mess with existing plugins, the `plugins` directory is where you want to look. It contains the
implementation of all plugins shipped with Mumble by default and also the plugin headers needed to create new ones. For further documentation on that,
you are referred to the dedicated plugin documentation within the `docs/dev` directory.

`samples` and `screenshots` are self-describing and generally of very little interest when dealing with the Mumble source code as such. `scripts` is
mainly occupied by various scripts used throughout various tasks surrounding Mumble. In addition to that, it also contains a few things such as a
default configuration file for the Mumble server.

Your main focus should lie on the `src` directory as this is where the bulk of Mumble's source code is living. Directly in `src/` are shared sources
that are used by the Mumble client as well as by the server that live in `src/mumble` and `src/murmur` respectively. The remaining directories within
`src` should be somewhat self-explanatory.

The `themes` directory contains all built-in themes that are currently shipped with Mumble. At this point there is only one theme available: the
`Default` theme (note that it contains the Lite and the Dark theme variant). If you want to customize any kind of icons within Mumble, this is the
place to check out. It contains its own documentation, so you are encouraged to have a read.


## Important files

Now that we have established our general bearings, it is time to get a little bit more specific about the individual files in the source tree
(`src/*`).

To begin with a general note: In Mumble the name of the header file in which a class and its function is declared, is not necessarily a guarantee that
the implementation of that function is also in the source file with the corresponding name. So it can happen that the file `MyClass.h` defines a class
`MyClass` that declares a function `X`. That function may not end up being implemented in `MyClass.cpp`. Classes that make extensive use of this
pattern are the `Server` and `MainWindow` class respectively. If the header doesn't mention where the given set of functions is implemented, your best
chance is to search the source files in the same directory for a matching implementation (in almost all cases, the implementation lives in a source
file within the same directory as the header in question).


### Client

First off: all the `*.ts` are used for localizations (translations) are are handled by external services. Thus, you should not modify them by hand
as your changes would likely be overwritten by said service.

- `main.cpp`: This contains the main entry point into the client (the "main" function) in which all command-line arguments are processed and a bunch
  of objects are instantiated and prepared for further use. The main purpose (as far as most developers are concerned) is the instantiation of the
  `MumbleApplication` which mostly is just a `QApplication`. That means that from this point on the program is mainly event-driven.
- `MainWindow.cpp`: This can be pretty much be considered the heart of the Mumble client. It is responsible for managing the main Mumble UI and also
  for coordinating all sorts of events that are received and sent. If you are tracing down some functionality, chances are high that the `MainWindow`
  class is involved in it in one way or another.
- `UserModel.cpp`: This class is responsible for managing the in-memory representation of the channel and user tree. All user and channel objects on
  the client are created here.
- `Messages.cpp`: This class implements all Protobuf message handling that is performed on the client-side. Technically all these functions belong to
  the `MainWindow` class, but their implementation is separated into this dedicated source file.
- `ServerHandler.cpp`: This class is responsible for managing a connection to a given server. It handles the immediate network connection to the
  server and makes sure that all messages are sent and received in the appropriate thread. If you need to send any request to the server, the server
  handler is the one to perform the request for you.
- `AudioInput.cpp`, `AudioOutput.cpp`: These classes implement the general audio input and output handling. The actual interaction with the
  system-specific audio backend (e.g. PulseAudio, CoreAudio or WASAPI) is handled by dedicated sub-classes which are implemented in the files named
  after the supported audio backend (e.g. `PulseAudio.cpp`, `CoreAudio.mm` or `WASAPI.cpp`). These typically contain an implementation for the input
  _and_ output handling.
- `PluginManager.cpp`: Everything that is related to loading and running plugins within Mumble, is handled by the `PluginManager` class.
- `API_v*`: These are the various implementations of the plugin API functions. These are the functions plugins may call in order to interact with
  Mumble.
- `Global.cpp`: The `Global` class is a singleton accessed via `Global::get()` and it holds a variety of shared data used throughout the client.

For many UI elements, we use `.ui` files which are XML-files that describe the UI elements in a way that is understood by
[Qt Designer](https://doc.qt.io/qt-5/qtdesigner-manual.html). With this tool, you can edit the elements in a WYSIWYG fashion (at least for the most
part). In any case, it is a great tool, when you are trying to figure out what will happen once you click a certain button in a given UI element. Just
open the element in Qt Designer, check the button's name and search for that in the corresponding `.cpp` implementation. Note that we are using
implicit signal-connecting which is based on a special naming scheme of slots in a given UI class (e.g. `on_xy_actived` where `xy` is the name of the
corresponding UI element).

When creating or changing existing UI elements, always consider the [accessibility checklist](/docs/dev/Accessibility.md).

### Server

When working with the server, it is useful to know that the Mumble server has the concept of "virtual servers" where one running instance of the
Mumble server program, may simultaneously serve multiple Mumble servers (aka: they will show up as separate servers in a client's server list).

- `main.cpp`: This file (again) contains the main entry point into the server code (the "main" function)
- `Meta.cpp`: The `Meta` class is responsible for managing the different virtual servers. That includes booting them.
- `Messages.cpp`: Just as for the client, this file contains the implementation of all Protobuf message handling.
- `Server.cpp`: This is the heart of every virtual server instance. It contains a bunch of auxiliary functionality and completely handles the UDP
  audio thread.
- `ServerDB.cpp`: As the name suggests, this file implements database access of all kinds.

If you want to mess with or extend the server's Ice RPC framework, check out the dedicated documentation for that.

