# Client Configuration Guide


<!-- Pictures or even Gifs could be added for better presentation-->

<!-- 
![Alt-Text](/Path/to/picture.jpg)
--->


<!--## Audio Wizard? -->

<!--## Important Config Options -->

<!-- Topics suggested by Krzmbrzl: 
change theme between Lite and Dark mode, echo cancellation, transmission modes (VAD, PTT, continous), shortcuts -->

## Configuration Options

<!-- where to find config options, add gif? -->

### Audio Input

#### Interface

##### Echo Cancellation

If enabled, this will filter echo from the audio you send to others.

You should usually enable it, because even when you use headsets, they might transmit undesired noise.

Choose one of the following options:

| Option: |	Description: | Usecase: |
| --- | --- | --- |
| Disabled | Disables echo cancellation (not recommended).  | You use a very good headset. Or you use external echo cancellation (for example via pulseaudio). Or echo cancellation is causing problems. |
| Mixed echo cancellation | This is the basic Option: It will process all loudspeaker outputs bundled together.
This is less accurate than the Multichannel option, but will also use less CPU. 	| Sufficient for setups with loudspeakers near to the microphone. |
| Multichannel echo cancellation | Extended option: It will process all audio channels seperately, this is more accurate, but will result in higher CPU usage. |	For setups with (multiple) loudspeakers farther away from the microphone. |

#### Transmission



### Overlay

<!--
![Here should be a picture](overlay.jpg)
-->

If enabled, it implements a small window inside other applications (mostly Games), that will show the users and their status in your Mumble channel.

