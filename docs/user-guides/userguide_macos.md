# Usage on MacOS

## Mumble (Client)

### Installation

Download Mumble from [our website](https://www.mumble.info/downloads/).

To install Mumble, drag the application from the downloaded
disk image into your `/Applications` folder.

Some more information can be found in our [wiki](https://wiki.mumble.info/wiki/Installing_Mumble#Mac_OS_X).

#### Overlay

In order to be able to use the Overlay it has to be installed separately.
To install it: launch Mumble, go into Mumble's settings -> Overlay section and you should see an option to install it.

*Info:* The reason for the separate installation is that Mumble itself does not need administrative rights to install, but the overlay does. 

### Start

On first start of Mumble, a short Settings wizard (for Audio settings, certificate creation etc.) will start automatically.

### Configuration

## Mumble Server (Murmur)

### Installation

The Mumble Server (Murmur) is distributed separately from the Mumble client on MacOS.
It is called **Static macOS Server** and can be downloaded from [our website](https://www.mumble.info/downloads/).

### Configuration

Take a look at the [Server Configuration Guide](server_config_guide.md).

### Start

The Mumble Server should be run from the command line, so start a shell (command prompt) and go to wherever you installed Mumble. Run the Mumble Server with:

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
