# Userguide for Windows

## Contents

- [Mumble Client](#mumble-client)
- [Mumble Server](#mumble-server-murmur)

## Mumble (Client)

### Installation

Download Mumble from [our website](https://www.mumble.info/downloads/).

Doubleclick on the downloaded file to start the installation.
The Installer will guide you through the installation steps.

You can find some more information in our [wiki](https://wiki.mumble.info/wiki/Installing_Mumble#Windows).

### Start

Start Mumble via your Start Menu.

On first start of Mumble, a short Settings wizard (for Audio settings, certificate creation etc.) will start automatically.

### Configuration

<!-- Short introduction or just link to the Client Config Guide? -->
Take a look at the [Client Configuration Guide](client_config_guide.md).
<!-- Are there differences between the OS versions? -->

## Mumble Server (Murmur)

### Installation

The Mumble Installer includes an option to install the Mumble-Server (see above). 

### Configuration

Take a look at the [Server Configuration Guide](server_config_guide.md).

### Start

Doubleclick the Murmur icon to start murmur. A small icon on your taskbar will appear from which you can view the log.

To set the superuser password, run murmur with the parameters `-supw <password>`.

<!-- Start via console? -->

1. Set the SuperUser (Server Admin) password:

```
./murmur.x86 -supw <password>
```

2. Start the server:

*Note:* Flags are optional (see below).
The standard configuration file used, is `murmur.ini` in the same directory.

```
./murmur.x86
```

<!-- Add more flags, like in the Linux guide? -->

**Flags:**

```
murmurd [-supw <password>] [-ini <inifile>] [-fg] [v]

-supw   Set a new password for the SuperUser (Server Admin). Keep this password safe. Mumble Server will stop after this command.

-ini    Use a specific inifile. You can also use this to run several instances
        of murmur from the same directory. Make sure each instance is using a separate database.

-fg     Run in the foreground, logging to standard output.

-v      More verbose logging.
```