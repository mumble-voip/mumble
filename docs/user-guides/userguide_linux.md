# Userguide for Linux/Unix

## Contents

- [Mumble Client](#Mumble Client)
- [Mumble Server](#Mumble Server Murmur)

## Mumble (Client)

### Installation

The best option to install Mumble is through your distribution's repository.
Just search for `Mumble` in your package manager and install it.

If Mumble is not available or outdated in your distribution's repos, you can take a look at the Linux Section on our [website](https://www.mumble.info/downloads/#linux).
There you will find multiple alternatives, including official Third-Party-Repos, Snap- and Flatpak-Packages.

More information can be found in our [wiki](https://wiki.mumble.info/wiki/Installing_Mumble#Linux).

### Start

If you have installed Mumble through your distribution's repository, you should be able to find Mumble in your start menu. 

Alternatively you can start it via a terminal.
The usual command is: `mumble`.

Otherwise take a look at the package documentation of your distribution or the documentation for Flatpaks, Snapcraft etc.

On first start of Mumble, a short Settings wizard (for Audio settings, certificate creation etc.) will start automatically.

### Configuration

<!-- Short introduction or just link to the Client Config Guide? -->
Take a look at the [Client Configuration Guide](client_config_guide.md).
<!-- Are there differences between the OS versions? -->

## Mumble Server (Murmur)

### Installation

The best option to install the Mumble Server (Murmur) is through your distribution's repository.
Just search for `Mumble Server` or `Murmur` in your package manager and install it.

Alternatively you can also find a **Static Linux Server** package on our [website](https://www.mumble.info/downloads/#manual-download).
<!-- Add a note that it contains outdated openssl? -->

### Configuration

Take a look at the [Server Configuration Guide](server_config_guide.md).

### Start

The Mumble Server is either started [automatically](#Auto Start) or [manually](#Manual Start).

#### Auto Start

On many distributions the Mumble Server is started automatically on system boot.

You can also disable this and start the Server manually.
Most distributions use `systemd` now, so search in the systemd documentation on how to disable the Mumble Server service.

#### Manual Start

The Mumble Server should be run from the command line, so start a terminal.
(*If the `Static Linux Server` is installed or the binary is not linked, you have to change directory to wherever the Mumble Server is installed, first.*)

Run the Mumble Server with:

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
