# Windows Xbox input regression tests

`TestXboxInput` uses an injected state-query function, so it does not require a
controller. It checks packet-number caching, disconnect releases, independent
slots, and reconnects that reuse a packet number. The worker test supplies no raw
input messages: polling must still discover a controller, observe its disconnect,
and accept input after reconnection.

Build with `client=ON`, `tests=ON`, and `xboxinput=ON` on Windows, then run from the
build directory:

```sh
ctest -R '^TestXboxInput$' --output-on-failure
```

## Physical reconnect check

The fake backend cannot reproduce Windows driver teardown. Also validate the
client with an Xbox controller and the Xbox Wireless Adapter:

1. Close other Mumble instances, including any voice application that embeds
   Mumble. Leave the normal Windows controller drivers enabled.
2. Start the client under test with XInput shortcut support enabled. An isolated
   profile is sufficient; connecting to a voice server is not necessary.
3. Connect the controller and press A in Steam Big Picture or a game. Confirm
   that the input is accepted and the controller remains connected.
4. Exercise the controls for about ten seconds, turn the controller off normally,
   and wait five seconds.
5. Repeat the connect/input/disconnect sequence at least five times without
   restarting Mumble or the PC.

A powered-on controller with no usable game input is not a pass. Neither
disabling XInput shortcut support nor bypassing a Windows input driver validates
the reconnect behavior.

Historical context: [Mumble issue #2649](https://github.com/mumble-voip/mumble/issues/2649).
