# Global shortcuts

## Windows

### Generic devices

No additional steps should be required.

If you're encountering problems, feel free to open an issue.

### Xbox controllers

If you want to use the guide button in Mumble,
the trigger for the Game bar must be disabled.

![Windows screenshot](../screenshots/Windows_settings_game_bar.png)

When the option is enabled, Windows doesn't pass the input to applications.

Even `XInputGetStateEx()` doesn't detect it!

## Linux

### Poll-driven engine

Uses a little more processing power, but no additional steps are required.

### Event-driven engine

Mumble needs read permissions for `/dev/input/event*`.

The solution usually consists in adding your user to the `input` group:

```
sudo usermod -a -G input user
```

Please note that this allows any applications running under the same user
to read data sent by input devices.

If the solution doesn't work for you, you could allow all users to read the input:

```
  sudo chmod a+r /dev/input/event*
```

Please note that this is only meant to be a temporary solution, because:

- It will most likely only last until the next reboot
(at which point devfs is recreated).
- It allows any applications to read the input data,
regardless of the user they're running under!

### Wayland

On Wayland, global shortcuts must be configured by the compositor.

#### KDE Plasma

Using the default settings, KDE (at least since 6.5) forwards most keyboard
shortcuts except letters and digits. Check _System Settings > Application
Permissions > Legacy X11 App Support_:

![KDE Wayland](../screenshots/KDE_Wayland.png)

If you want to use a mouse button as a global shortcut, you must enable
_Listening for mouse buttons_ which is off by default.

## macOS

Mumble needs to be granted accessibility permissions.

![macOS screenshot](../screenshots/macOS_privacy_accessibility.png)
