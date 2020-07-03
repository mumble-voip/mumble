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

## Running Mumble

On Windows, after installation, you should have a new Mumble folder in your
Start Menu, from which you can start Mumble.

On Mac OS X, to install Mumble, drag the application from the downloaded
disk image into your `/Applications` folder.

On Linux, if you have installed Mumble through your distributon's package
repostory, you should be able to find Mumble in your start menu. No
additional steps necessary.

Once Mumble is launched, you need a server to connect to. Either create your
own or join a friend's.

## Running Murmur on Unix-like systems

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

## Running Murmur on Mac OS X

Murmur is distributed separately from the Mumble client on Mac OS X.
It is called Static OS X Server and can be downloaded from the main webpage.

Once downloaded it can be run in the same way as on any other Unix-like system.
For more information please see the 'Running Murmur on Unix-like systems' above.

## Running Murmur on Win32

Doubleclick the Murmur icon to start murmur. There will be a small icon on your
taskbar from which you can view the log.

To set the superuser password, run murmur with the parameters `-supw <password>`.
