![Mumble screenshot](screenshots/Mumble.png)

# Mumble - Open Source voice-chat software 

[![https://www.mumble.info](https://img.shields.io/badge/Website-https%3A%2F%2Fwww.mumble.info-blue?style=for-the-badge)](https://www.mumble.info)

[![#mumble:matrix.org](https://img.shields.io/matrix/mumble:matrix.org?label=%23mumble:matrix.org&style=for-the-badge)](https://riot.im/app/#/room/#mumble:matrix.org)
[![#mumble@freenode.net](https://img.shields.io/badge/Freenode.net-%23mumble-blue?style=for-the-badge)](https://webchat.freenode.net/#mumble)

[![Codacy](https://img.shields.io/codacy/grade/262a5e20c83a40599050e22e700d8a3e?label=Codacy&style=for-the-badge)](https://app.codacy.com/manual/mumble-voip/mumble)
[![LGTM](https://img.shields.io/lgtm/grade/cpp/github/mumble-voip/mumble?label=LGTM&style=for-the-badge)](https://lgtm.com/projects/g/mumble-voip/mumble)
[![Azure](https://img.shields.io/azure-devops/build/Mumble-VoIP/c819eb06-7b22-4ef3-bbcd-860094454eb3/1?label=Azure&style=for-the-badge)](https://dev.azure.com/Mumble-VoIP/Mumble)
[![Cirrus CI](https://img.shields.io/cirrus/github/mumble-voip/mumble?label=Cirrus%20CI&style=for-the-badge)](https://cirrus-ci.com/github/mumble-voip/mumble)
[![Travis CI](https://img.shields.io/travis/com/mumble-voip/mumble?label=Travis%20CI&style=for-the-badge)](https://travis-ci.com/mumble-voip/mumble)

Mumble is an Open Source, low-latency and high-quality voice-chat program
written on top of Qt and Opus.

There are two modules in Mumble; the client (mumble) and the server
(murmur). The client works on Win32/64, Linux and Mac OS X, while the
server should work on anything Qt can be installed on.

Note that when we say Win32, we mean Windows XP or newer.

## Windows

### Running Mumble

After installation, you should have a new Mumble folder in your
Start Menu, from which you can start Mumble.

### Running Murmur

Doubleclick the Murmur icon to start murmur. There will be a small icon on your
taskbar from which you can view the log.

To set the superuser password, run murmur with the parameters `-supw <password>`.


## MacOS

### Running Mumble

To install Mumble, drag the application from the downloaded
disk image into your `/Applications` folder.

### Running Murmur

Murmur is distributed separately from the Mumble client on MacOS.
It is called Static OS X Server and can be downloaded from the main webpage.

Once downloaded it can be run in the same way as on any other Unix-like system.
For more information please see the "Running Murmur" in the Linux/Unix section below.


## Linux/Unix

### Running Mumble

If you have installed Mumble through your distributon's package
repostory, you should be able to find Mumble in your start menu. No
additional steps necessary.

### Running Murmur

Murmur should be run from the command line, so start a shell (command prompt)
and go to wherever you installed Mumble. Run murmur as

```
murmurd [-supw <password>] [-ini <inifile>] [-fg] [v]

-supw   Set a new password for the user SuperUser, which is hardcoded to
        bypass ACLs. Keep this password safe. Until you set a password,
        the SuperUser is disabled. If you use this option, murmur will
        set the password in the database and then exit.

-ini    Use an inifile other than murmur.ini, use this to run several instances
        of murmur from the same directory. Make sure each instance is using
        a separate database.

-fg     Run in the foreground, logging to standard output.

-v      More verbose logging.
```

### OpenGL Overlay

The OpenGL overlay works by intercepting the call to switch buffers, and just
before the buffer switch, we draw our nice GUI.

To load a game with the overlay enabled, start the game like this:
  LD_PRELOAD=/path/to/libmumble.so.1.1 gamename

If you have Mumble installed through the binary packages, this can be done by
simply typing:
  mumble-overlay gamename

### Global Keyboard Events

On Linux, Mumble supports reading the kernel input devices. To use this,
make sure mumble can open `/dev/input/event*` for reading. To test it out,
that should only require:

```
  sudo chmod a+r /dev/input/event*
```

However, be aware that this will most likely only last until the next reboot
(at which point devfs is recreated) and you also allow any program run by any
user to monitor the physical keyboard.

Without this, Mumble will continuously poll the mouse and keyboard state for
changes, which will use a tiny bit of processing power.

