# Mumble Network Protocol Documentation

The Mumble Network Protocol documentation is meant to be a reference for the
Mumble VoIP 1.2.X server-client communication protocol. It reflects the state of
the protocol implemented in the Mumble 1.2.8 client and might be outdated by the
time you are reading this.

For up to date message type and message structure information
refer to the [`Mumble.proto`](https://github.com/mumble-voip/mumble/blob/master/src/Mumble.proto)
and [`MumbleUDP.proto`](https://github.com/mumble-voip/mumble/blob/master/src/MumbleUDP.proto)
protobuf definition files.

This documentation is split into

* [Overview](overview.md)
* [Protocol Stack (TCP)](protocol_stack_tcp.md)
* [Establishing a Connection](establishing_connection.md)
* [Voice Data](voice_data.md)
