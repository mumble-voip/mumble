### OpenGL Overlay

<!-- Add a description -->

The OpenGL overlay works by intercepting the call to switch buffers, and just before the buffer switch, we draw our nice GUI.

To load a game with the overlay enabled, start the game like this:
  LD_PRELOAD=/path/to/libmumble.so.1.1 gamename

If you have Mumble installed through the binary packages, this can be done by
simply typing:
  mumble-overlay gamename