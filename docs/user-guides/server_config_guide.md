# Server Configuration Guide

**Note:** For Installation and Starting of the Mumble Server, see the specific [Userguides](README.md) for your Operating System.

The Configuration involves three steps:

1. [Network configuration](#network-configuration)
2. [Main configuration file](#main-configuration-file) (`murmur.ini`)
3. [Administration with Mumble Client](#administration-with-mumble-client)

<!-- In addition there are some other interesting topics:

- Bots (Music Bots)
- Web interfaces
- gRPC (remote control)
- WebRTC bridge

-->

## Network Configuration

### Static vs. dynamic IP

Users connect to your server via an IP or a Domainname (e.g. `mumble.example.com`).

To make this work continuously, you either need a static IP (that means: it does not change) or in case you have a dynamic IP (it changes every few days), a Dynamic DNS Service.

Otherwise you would need to send the new IP to your users, every time the IP changes.

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
The Standardport is `64738` (recommended), but you can change it to every available port (it is defined in the main configuration file (`murmur.ini`)).

**Note:** Use a port that is not standardized (widely used by known programs/services), see e.g. [Wikipedia - List of Ports](https://en.wikipedia.org/wiki/List_of_TCP_and_UDP_port_numbers).

Allow it for both protocols (TCP and UDP).

Check and adjust your:

- Computer's/Server's Firewall
- (optional) Computer's Antivirus Program(s)
- (for Home-Use) Router's Firewall

<!-- more ports, for specific usecases? -->

## Main configuration file

<!-- List (&explain) the most important options -->

<!-- welcometext=
port=
serverpassword=
bandwidth=
users=
registerName= -->

For all available options, take a look at [Mumble Wiki - Murmur.ini](https://wiki.mumble.info/wiki/Murmur.ini).

## Administration with Mumble Client

- Use the Mumble client GUI for administration

<!-- 1. Promote a normal User to be admin -->

<!-- Channel creation, General Configuration, ACL and Groups -->

