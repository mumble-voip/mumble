# Server Configuration Guide

**Note:** For Installation and Starting of the Mumble Server, see the specific [Userguides](README.md) for your Operating System.

The Configuration involves three steps:

1. [Network configuration](#network-configuration)
2. [Main configuration file](#main-configuration-file) (`murmur.ini`)
3. [Administration with Mumble Client](#administration-with-mumble-client)

<!-- In addition there are some other interesting topics:

- Bots (Music Bots)
- Web interfaces (outdated)
- gRPC (remote control) (buggy, not ready)
- WebRTC bridge

-->

## Network Configuration

### Static vs. dynamic IP

Users connect to your server via an IP or a Domainname (e.g. `mumble.example.com`).

To make this work continuously, you either need a static IP (that means: the IP does not change) or in case you have a dynamic IP (it changes every few days), a Dynamic DNS Service (see below). Otherwise you would need to send the new IP to your users, every time the IP changes.

*Tip:*

* If your Mumble Server is running on a computer at your home, you will likely have a dynamic IP, thus you need a Dynamic DNS Service.
* If your Mumble Server is running on a rented server at a Provider, you will in most cases have a static IP (already).

#### Dynamic DNS Service 

A Dynamic DNS Service will give you a subdomain (e.g. `mymumbleserver123.dyndns-example.com`) and/or a static IP which your users can use to connect to your server.

In the background the service will route them to your dynamic IP.
A special software running on your computer will tell the service whenever your dynamic IP changes.

For more information and a list of services and software, take a look at the [Arch Wiki - Dynamic DNS](https://wiki.archlinux.org/index.php/Dynamic_DNS) for example.

### Firewall & Ports

In order for your users to be able to connect to your server, you need to allow incoming connections to a specific Port that the Mumble Server uses. 

The Standardport is `64738` (recommended).
You can also change it to every available port by editing the main configuration file (`murmur.ini`; see below).

**Note:** Use a port that is not standardized (widely used by known programs/services), see e.g. [Wikipedia - List of Ports](https://en.wikipedia.org/wiki/List_of_TCP_and_UDP_port_numbers).

Allow connections to the Port for both protocols (TCP and UDP).

Check and adjust your:

- Computer's/Server's Firewall
- (optional) Computer's Antivirus Program(s)
- (for Home-Use) Router's Firewall

<!-- more ports, for specific usecases? -->

## Main configuration file

The main server configuration file is called `murmur.ini` (it can be renamed though).

You should find an example murmur.ini file alongside your installation, but you can also find the most recent **example file (with explanations) on [Github](https://github.com/mumble-voip/mumble/blob/master/scripts/murmur.ini)**.

Below is a list of the most important options:

`welcometext="Welcome on our Server!"` - the text will be send to every new user via chatmessage; if it's empty, no message will be send

`port=64738` - sets the port (see above); `64738` is the standard port

`serverpassword=` - sets the Serverpassword; if it's empty, users can connect without a password.

`bandwidth=72000` - sets the maximum bandwidth per user (in bits per second); setting it higher can increase the audio quality

`users=100` - sets the maximum number of online users

`registerName=` - sets the Servername and the name of the main channel

For all available options and explanations, take a look at [Mumble Wiki - Murmur.ini](https://wiki.mumble.info/wiki/Murmur.ini).

## Administration with Mumble Client

- Use the Mumble client GUI for administration

<!-- 1. Promote a normal User to be admin -->

<!-- Channel creation, General Configuration, ACL and Groups -->

