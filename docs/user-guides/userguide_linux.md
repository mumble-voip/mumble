# Userguide for Linux/Unix

## Contents

- [Mumble Client](#mumble-client)
- [Mumble Server](#mumble-server-murmur)

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
The usual command is: 

```
mumble
```

Otherwise take a look at the package documentation of your distribution or the documentation for Flatpaks, Snapcraft etc.

**Note:** On first start of Mumble, a short Settings wizard (for Audio settings, certificate generation etc.) will start automatically.

### Configuration

<!-- Short introduction or just link to the Client Config Guide? -->
Take a look at the [Client Configuration Guide](client_config_guide.md).

## Mumble Server (Murmur)

### Installation

The best option to install the Mumble Server (Murmur) is through your distribution's repository.
Just search for `Mumble Server` or `Murmur` in your package manager and install it.

Alternatively you can also find a **Static Linux Server** package on our [website](https://www.mumble.info/downloads/#manual-download).

### Configuration

Take a look at the [Server Configuration Guide](server_config_guide.md).

### Start

The Mumble Server is either started [automatically](#auto-start) or [manually](#manual-start).

#### Auto Start

On many distributions the Mumble Server is started automatically on system boot.

You can also disable this and start the Server manually.
Most distributions use `systemd` now, so search in the systemd documentation on how to disable the Mumble Server service (e.g. `murmur.service`).

Next Steps: 

1. Set the SuperUser (Server Admin) password:

```
murmurd -ini /etc/murmur.ini -supw PASSWORD
```

2. Restart the Server:
If your system uses `systemd`, use e.g.:

```
systemctl restart murmur.service
```

3. Login & administrate the Server:
See [Server Configuration Guide](server_config_guide.md#administration-with-mumble-client) for details.

#### Manual Start

The Mumble Server should be run from a terminal.
(*If the `Static Linux Server` is installed or the binary is not linked, you have to change the directory to wherever the Mumble Server is installed, first.*)

<!-- Will the binary name change in 1.4? -->

<!-- mention different binary name of static linux server? -->

1. Set the SuperUser (Server Admin) password:

```
murmurd -supw <password>
```

2. Start the server:

*Note:* Flags are optional (see below).
The standard configuration file used, is `murmur.ini` in the same directory.

```
murmurd 
```


**Flags:**

```
murmurd [-supw <password>] [-ini <inifile>] [-fg] [v]

-supw   Set a new password for the SuperUser (Server Admin). Keep this password safe. Mumble Server will stop after this command.

-ini    Use a specific inifile. You can also use this to run several instances
        of murmur from the same directory. Make sure each instance is using a separate database.

-fg     Run in the foreground, logging to standard output.

-v      More verbose logging.
```

3. Login & administrate the Server:
See [Server Configuration Guide](server_config_guide.md#administration-with-mumble-client) for details.