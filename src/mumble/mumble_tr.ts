<!DOCTYPE TS><TS>
<context>
    <name>ACLEditor</name>
    <message>
        <source>Active ACLs</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Context</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>User/Group</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Permissions</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Group</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Members</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Up</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Down</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Inherit ACLs</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Applies to this channel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Applies to sub-channels</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>User ID</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Deny</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Allow</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Remove</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Inherit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Inheritable</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Inherited</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Add</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Add to Remove</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Groups</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;ACL</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Accept changes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This button will accept current groups/ACLs and send them to the server. Note that if you mistakenly remove write permission from yourself, the server will add it.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Reject changes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This button will cancel all changes and closes the dialog without updating the ACLs or groups on the server.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>List of entries</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This shows all the entries active on this channel. Entries inherited from parent channels will be show in italics.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Inherit ACL of parent?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This sets whether or not the ACL up the chain of parent channels are applied to this object. Only those entries that are marked in the parent as &quot;Apply to sub-channels&quot; will be inherited.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Add new entry</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This adds a new entry, initially set with no permissions and applying to all.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Remove entry</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This removes the currently selected entry.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Move entry up</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This moves the entry up in the list. As entries are evaluated in order, this may change the effective permissions of users. You cannot move an entry above an inherited entry, if you really need that you&apos;ll have to duplicate the inherited entry.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Move entry down</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This moves the entry down in the list. As entries are evaluated in order, this may change the effective permissions of users.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Entry should apply to this channel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This makes the entry apply to this channel.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Entry should apply to sub-channels.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This makes the entry apply to sub-channels of this channel.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Group this entry applies to</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This controls which group of users this entry applies to.&lt;br /&gt;Note that the group is evaluated in the context of the channel the entry is used in. For example, the default ACL on the Root channel gives &lt;i&gt;Write&lt;/i&gt; permission to the &lt;i&gt;admin&lt;/i&gt; group. This entry, if inherited by a channel, will give a user write privileges if he belongs to the &lt;i&gt;admin&lt;/i&gt; group in that channel, even if he doesn&apos;t belong to the &lt;i&gt;admin&lt;/i&gt; group in the channel where the ACL originated.&lt;br /&gt;If a group name starts with a !, its membership is negated, and if it starts with a ~, it is evaluated in the channel the ACL was defined in, rather than the channel the ACL is active in. Order is important; &lt;i&gt;!~in&lt;/i&gt; is valid, but &lt;i&gt;~!in&lt;/i&gt; is not.&lt;br /&gt;A few special predefined groups are:&lt;br /&gt;&lt;b&gt;all&lt;/b&gt; - Everyone will match.&lt;br /&gt;&lt;b&gt;auth&lt;/b&gt; - All authenticated users will match.&lt;br /&gt;&lt;b&gt;sub,a,b,c&lt;/b&gt; - User currently in a sub-channel minimum &lt;i&gt;a&lt;/i&gt; common parents, and between &lt;i&gt;b&lt;/i&gt; and &lt;i&gt;c&lt;/i&gt; channels down the chain. See the website for more extensive documentation on this one.&lt;br /&gt;&lt;b&gt;in&lt;/b&gt; - Users currently in the channel will match (convenience for &lt;i&gt;sub,0,0,0&lt;/i&gt;).&lt;br /&gt;&lt;b&gt;out&lt;/b&gt; - Users outside the channel will match (convenience for &lt;i&gt;!sub,0,0,0&lt;/i&gt;).&lt;br /&gt;Note that an entry applies to either a user or a group, not both.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>User this entry applies to</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This controls which user this entry applies to. Just type in the user name and hit enter to query the server for a match.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Allow %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Deny %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This grants the %1 privilege. If a privilege is both allowed and denied, it is denied.&lt;br /&gt;%2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This revokes the %1 privilege. If a privilege is both allowed and denied, it is denied.&lt;br /&gt;%2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>List of groups</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This is all the groups currently defined for the channel. To create a new group, just type in the name and press enter.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Remove selected group</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This removes the currently selected group. If the group was inherited, it will not be removed from the list, but all local information about the group will be cleared.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Inherit group members from parent</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This inherits all the members in the group from the parent, if the group is marked as &lt;i&gt;Inheritable&lt;/i&gt; in the parent channel.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Make group inheritable to sub-channels</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This makes this group inheritable to sub-channels. If the group is non-inheritable, sub-channels are still free to create a new group with the same name.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Group was inherited from parent channel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This indicates that the group was inherited from the parent channel. You cannot edit this flag, it&apos;s just for information.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Add member to group</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Type in the name of a player you wish to add to the group and press enter.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Remove member from group</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Type in the name of a player you wish to remove from the group and press enter.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mumble - Edit ACL for %1</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ASIOConfig</name>
    <message>
        <source>Device selection</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Capabilities</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Channels</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Configure input channels</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This will configure the input channels for ASIO. Make sure you select at least one channel as microphone and speaker. &lt;i&gt;Microphone&lt;/i&gt; should be where your microphone is attached, and &lt;i&gt;Speaker&lt;/i&gt; should be a channel that samples &quot;What you hear&quot;.&lt;br /&gt;For example, on the Audigy 2 ZS, a good selection for Microphone would be &quot;Mic L&quot; while Speaker should be &quot;Mix L&quot; and &quot;Mix R&quot;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Device to use for microphone</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This chooses what device to query. You still need to actually query the device and select which channels to use.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Device</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Query selected device</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This queries the selected device for channels. Be aware that many ASIO drivers are buggy to the extreme, and querying them might cause a crash of either the application or the system.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Configure selected device</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This configures the selected device. Be aware that many ASIO drivers are buggy to the extreme, and querying them might cause a crash of either the application or the system.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Driver name</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Buffersize</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Microphone</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;-</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>-&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Unused</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Speakers</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>%1 (ver %2)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>%1 ms -&gt; %2 ms (%3 ms resolution) %4Hz</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>%1 ms -&gt; %2 ms (%3 ms resolution) %4Hz -- Unusable</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mumble</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>ASIO Initialization failed: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Failed to instantiate ASIO driver</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>ASIO</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Query</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Configure</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ASIOInput</name>
    <message>
        <source>Mumble</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>You need to select at least one microphone and one speaker source to use ASIO. If you just need microphone sampling, use DirectSound.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Opening selected ASIO device failed. No input will be done.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>AboutDialog</name>
    <message>
        <source>&amp;About Mumble</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;License</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;h3&gt;Mumble v%1&lt;/h3&gt;&lt;p&gt;Copyright 2005-2006 Thorvald Natvig&lt;br /&gt;slicer@users.sourceforge.net&lt;/p&gt;&lt;p&gt;&lt;b&gt;A voice-chat utility for gamers&lt;/b&gt;&lt;/p&gt;&lt;p&gt;&lt;tt&gt;http://mumble.sourceforge.net/&lt;/tt&gt;&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>AboutSpeexDialog</name>
    <message>
        <source>&lt;h3&gt;About Speex&lt;/h3&gt;&lt;p&gt;&lt;tt&gt;http://www.speex.org/&lt;/tt&gt;&lt;/p&gt;&lt;p&gt;This program uses Speex version %1&lt;/p&gt;&lt;p&gt;Speex is used for echo cancellation, noise&lt;br /&gt;filtering, voice activity detection and speech&lt;br /&gt;compression.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>AudioConfigDialog</name>
    <message>
        <source>Interfaces</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Transmission</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Compression</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Jitter Buffer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Input</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Input method for audio</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;b&gt;This is the input method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Output</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Output method for audio</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;b&gt;This is the output method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Continuous</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Voice Activity</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Push To Talk</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Transmit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>When to transmit your speech</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets when speech should be transmitted.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;Continuous&lt;/i&gt; - All the time&lt;br /&gt;&lt;i&gt;Voice Activity&lt;/i&gt; - When you are speaking clearly.&lt;br /&gt;&lt;i&gt;Push To Talk&lt;/i&gt; - When you hold down the hotkey set under &lt;i&gt;Shortcuts&lt;/i&gt;.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Voice Hold</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>How long to keep transmitting after silence</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;b&gt;This selects how long after a perceived stop in speech transmission should continue.&lt;/b&gt;&lt;br /&gt;Set this higher if your voice breaks up when you speak (seen by a rapidly blinking voice icon next to your name).&lt;br /&gt;Only makes sense when used with Voice Activity transmission.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Audio per packet</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>How many audio frames to send per packet</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;b&gt;This selects how many audio frames should be put in one packet.&lt;/b&gt;&lt;br /&gt;Increasing this will increase the latency of your voice, but will also reduce bandwidth requirements.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Use TCP mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Use TCP Compatibility mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;b&gt;Enable TCP Compatibility mode&lt;/b&gt;.&lt;br /&gt;This will make Mumble use only TCP when communicating with the server. This will increase overhead and cause lost packets to produce noticeable pauses in communication, so this should only be used if you are unable to use the default (which uses UDP for voice and TCP for control).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Automatic Reconnect</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Reconnect when disconnected</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;b&gt;Reconnect when disconnected&lt;/b&gt;.&lt;br /&gt;This will make Mumble try to automatically reconnect after 10 seconds if your server connection fails.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Maximum bandwidth used for sent audio</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;b&gt;This shows peak outgoing bandwidth used.&lt;/b&gt;&lt;br /&gt;This shows the peak amount of bandwidth sent out from your machine. Audio bitrate is the maximum bitrate (as we use VBR) for the audio data alone. Position is the bitrate used for positional information. Overhead is our framing and the IP packet headers (IP and UDP is 90% of this overhead).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Outgoing Bitrate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Quality</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Quality of compression (peak bandwidth)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets the quality of compression.&lt;/b&gt;&lt;br /&gt;This determines how much bandwidth Mumble is allowed to use for outgoing audio.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Complexity</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Complexity of compression (CPU)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets the complexity of compression.&lt;/b&gt;&lt;br /&gt;This determines how much CPU Mumble is allowed to use to increase transmitted voice quality. Settings above 5 give only marginal gain.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Amp</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Maximum amplification of input sound</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;b&gt;Maximum amplification of input.&lt;/b&gt;&lt;br /&gt;Mumble normalizes the input volume before compressing, and this sets how much it&apos;s allowed to amplify.&lt;br /&gt;The actual level is continually updated based on your current speech pattern, but it will never go above the level specified here.&lt;br /&gt;If the &lt;i&gt;Microphone loudness&lt;/i&gt; level of the audio statistics hover around 100%, you probably want to set this to 2.0 or so, but if, like most people, you are unable to reach 100%, set this to something much higher.&lt;br /&gt;Ideally, set it so &lt;i&gt;Microphone Loudness * Amplification Factor &gt;= 100&lt;/i&gt;, even when you&apos;re speaking really soft.&lt;br /&gt;&lt;br /&gt;Note that there is no harm in setting this to maximum, but Mumble will start picking up other conversations if you leave it to auto-tune to that level.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Default Jitter Buffer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>How long to prebuffer on first packet</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets the default buffer size for the jitter buffer&lt;/b&gt;.&lt;br /&gt;All incoming audio is buffered, and the jitter buffer continually tries to push the buffer to the minimum sustainable by your network, so latency can be as low as possible. This sets the default buffer size to use on the first packet to arrive from a new speaker, or when a speaker using Voice Activity or Push-To-Talk just started talking again. If the start of sentences you hear is very jittery, increase this value.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Basic Audio</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>%1 ms</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>%1s</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>%1kbit/s (Audio %2, Position %4, Overhead %3)</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>AudioStats</name>
    <message>
        <source>Peak microphone level</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Peak speaker level</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Peak clean level</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Microphone loudness</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mic Signal-To-Noise</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Speech Probability</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Audio bitrate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Peak power in last frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This shows the peak power in the last frame (20 ms), and is the same measurement as you would usually find displayed as &quot;input power&quot;. Please disregard this and look at &lt;b&gt;Loudness&lt;/b&gt; instead, which is much more steady and disregards outliers.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This shows the peak power in the last frame (20 ms) of the speakers. Unless you are using a multi-channel sampling method (such as ASIO) with speaker channels configured, this will be 0. If you have such a setup configured, and this still shows 0 while you&apos;re playing audio from other programs, your setup is not working.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This shows the peak power in the last frame (20 ms) after all processing. Ideally, this should be -96 dB when you&apos;re not talking. In reality, a sound studio should see -60 dB, and you should hopefully see somewhere around -20 dB. When you are talking, this should rise to somewhere between -5 and -10 dB.&lt;br /&gt;If you are using echo cancellation, and this rises to more than -15 dB when you&apos;re not talking, your setup is not working, and you&apos;ll annoy other players with echoes.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>How close the current input level is to ideal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This shows how close your current input volume is to the ideal. To adjust your microphone level, open whatever program you use to adjust the recording volume, and look at the value here while talking.&lt;br /&gt;&lt;b&gt;Talk loud, as you would when you&apos;re upset over getting fragged by a noob.&lt;/b&gt;&lt;br /&gt;Adjust the volume until this value is close to 100%, but make sure it doesn&apos;t go above. If it does go above, you are likely to get clipping in parts of your speech, which will degrade sound quality</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Signal-To-Noise ratio from the microphone</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This is the Signal-To-Noise Ratio (SNR) of the microphone in the last frame (20 ms). It shows how much clearer the voice is compared to the noise.&lt;br /&gt;If this value is below 1.0, there&apos;s more noise than voice in the signal, and so quality is reduced.&lt;br /&gt;There is no upper limit to this value, but don&apos;t expect to see much above 40-50 without a sound studio.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Probability of speech</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This is the probability that the last frame (20 ms) was speech and not environment noise.&lt;br /&gt;Voice activity transmission depends on this being right. The trick with this is that the middle of a sentence is always detected as speech; the problem is the pauses between words and the start of speech. It&apos;s hard to distinguish a sigh from a word starting with &apos;h&apos;.&lt;br /&gt;If this is in bold font, it means Mumble is currently transmitting (if you&apos;re connected).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Bitrate of last frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This is the audio bitrate of the last compressed frame (20 ms), and as such will jump up and down as the VBR adjusts the quality. To adjust the peak bitrate, adjust &lt;b&gt;Compression Complexity&lt;/b&gt; in the Settings dialog.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Power spectrum of input signal and noise estimate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This shows the power spectrum of the current input signal (red line) and the current noise estimate (filled blue).&lt;br /&gt;All amplitudes are multiplied by 30 to show the interesting parts (how much more signal than noise is present in each waveband).&lt;br /&gt;This is probably only of interest if you&apos;re trying to fine-tune noise conditions on your microphone. Under good conditions, there should be just a tiny flutter of blue at the bottom. If the blue is more than halfway up on the graph, you have a seriously noisy environment.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Weights of the echo canceller</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This shows the weights of the echo canceller, with time increasing downwards and frequency increasing to the right.&lt;br /&gt;Ideally, this should be black, indicating no echo exists at all. More commonly, you&apos;ll have one or more horizontal stripes of bluish color representing time delayed echo. You should be able to see the weights updated in real time.&lt;br /&gt;Please note that as long as you have nothing to echo off, you won&apos;t see much useful data here. Play some music and things should stabilize. &lt;br /&gt;You can choose to view the real or imaginary parts of the frequency-domain weights, or alternately the computed modulus and phase. The most useful of these will likely be modulus, which is the amplitude of the echo, and shows you how much of the outgoing signal is being removed at that time step. The other viewing modes are mostly useful to people who want to tune the echo cancellation algorithms.&lt;br /&gt;Please note: If the entire image fluctuates massively while in modulus mode, the echo canceller fails to find any correlation whatsoever between the two input sources (speakers and microphone). Either you have a very long delay on the echo, or one of the input sources is configured wrong.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>BanEditor</name>
    <message>
        <source>&amp;Add</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Update</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Accept changes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This button will accept current groups/ACLs and send them to the server. Note that if you mistakenly remove write permission from yourself, the server will add it.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Reject changes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This button will cancels all changes and closes the dialog without updating the ACLs or groups on the server.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mumble - Edit Bans</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ChanACL</name>
    <message>
        <source>W</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>T</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>E</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>S</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>A</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>M</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>K</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>C</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>L</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>None</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Write</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Traverse</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enter</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Speak</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>AltSpeak</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mute/Deafen</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Move/Kick</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Make channel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Link channel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This represents no privileges.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This represents total access to the channel, including the ability to change group and ACL information. This privilege implies all other privileges.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This represents the permission to traverse the channel. If a user is denied this privilege, he will be unable to access this channel and any sub-channels in any way, regardless of other permissions in the sub-channels.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This represents the permission to join the channel. If you have a hierarchical channel structure, you might want to give everyone Traverse, but restrict Enter in the root of your hierarchy.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This represents the permission to speak in a channel. Users without this privilege will be suppressed by the server (seen as muted), and will be unable to speak until they are unmuted by someone with the appropriate privileges.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This represents the permission to speak in a channel with flagged speech. This works exactly like the &lt;i&gt;speak&lt;/i&gt; privilege, but applies to packets spoken with AltPushToTalk held down. This may be used to broadcast to a hierarchy of channels without linking.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This represents the permission to mute and deafen other players. Once muted, a player will stay muted until he is unmuted by another privileged player or reconnects to the server.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This represents the permission to move a player to another channel or kick him from the server. To actually move the player, either the moving player must have Move/Kick privileges in the destination channel, or the player must normally be allowed to enter the channel. Players with this privilege can move players into channels the target player normally wouldn&apos;t have permission to enter.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This represents the permission to make sub-channels. The player making the sub-channel will be added to the admin group of the sub-channel.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This represents the permission to link channels. Players in linked channels hear each other, as long as the speaking player has the &lt;i&gt;speak&lt;/i&gt; privilege in the channel of the listener. You need the link privilege in both channels to create a link, but just in either channel to remove it.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ConfigDialog</name>
    <message>
        <source>Mumble Configuration</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Accept changes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This button will accept current settings and return to the application.&lt;br /&gt;The settings will be stored to disk when you leave the application.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Reject changes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This button will reject all changes and return to the application.&lt;br /&gt;The settings will be reset to the previous positions.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Apply</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Apply changes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This button will immediately apply all changes.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ConnectDialog</name>
    <message>
        <source>&amp;Name</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>A&amp;ddress</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Port</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Username</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Password</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Connect</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>DXAudioInput</name>
    <message>
        <source>Mumble</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Opening chosen DirectSound Input failed. No microphone capture will be done.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Opening chosen DirectSound Input failed. Default device will be used.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>DXAudioOutput</name>
    <message>
        <source>Mumble</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Opening chosen DirectSound Output failed. No audio will be heard.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Opening chosen DirectSound Output failed. Default device will be used.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>DXConfigDialog</name>
    <message>
        <source>Default DirectSound Voice Input</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Default DirectSound Voice Output</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Device selection</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Device to use for microphone</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This sets the input device to use, which is where you have connected the microphone.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Input</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Device to use for speakers/headphones</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This sets the output device to use, which is where you have connected your speakers or your headset.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Output</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Output Options</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Output Delay</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Amount of data to buffer for DirectSound</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This sets the amount of data to prebuffer in the DirectSound buffer. Experiment with different values and set it to the lowest which doesn&apos;t cause rapid jitter in the sound.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Positional Audio</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>None</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Panning</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Light HRTF</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Full HRTF</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>3D Sound Algorithm</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This sets what 3D Sound algorithm to use.&lt;br /&gt;&lt;b&gt;None&lt;/b&gt; - Disable 3D Sound (least CPU).&lt;br /&gt;&lt;b&gt;Panning&lt;/b&gt; - Just use stereo panning (some CPU).&lt;br /&gt;&lt;b&gt;Light/Full HRTF&lt;/b&gt; - Head-Related Transfer Functions enabled. This may use a small amount of CPU.&lt;br /&gt;Note that if you have a soundcard with &lt;i&gt;hardware&lt;/i&gt; 3D processing, HRTF processing will be done on the soundcard and will use practically no processing power.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Method</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>MinDistance</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Minimum distance to player before sound decreases</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This sets the minimum distance for sound calculations. The volume of other players&apos; speech will not decrease until they are at least this far away from you.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>MaxDistance</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Maximum distance, beyond which sound won&apos;t decrease</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This sets the maximum distance for sound calculations. When farther away than this, other players&apos; sound volume will not decrease any more.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>RollOff</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Factor for sound volume decrease</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>How fast should sound volume drop when passing beyond the minimum distance. The normal (1.0) is that sound volume halves each time the distance doubles. Increasing this value means sound volume drops faster, while decreasing it means it drops slower.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>DirectSound</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>%1m</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>%1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Players more than %1 meters away have %2% intensity</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>%1ms</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Database</name>
    <message>
        <source>Mumble</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mumble failed to initialize a database in any
of the possible locations.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>DirectInputKeyWidget</name>
    <message>
        <source>Press Shortcut</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>GlobalShortcutWinConfig</name>
    <message>
        <source>Shortcuts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Function</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Shortcut</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Shortcut bound to %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;b&gt;This is the global shortcut bound to %1&lt;/b&gt;&lt;br /&gt;Click this field and then the desired key/button combo to rebind. Double-click to clear.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>GlobalShortcutXConfig</name>
    <message>
        <source>Shortcuts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Function</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Shortcut</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Shortcut bound to %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;b&gt;This is the global shortcut bound to %1&lt;/b&gt;&lt;br /&gt;Click this field and then the desired key combo to rebind. Double-click to clear.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Log</name>
    <message>
        <source>[%2] %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Debug</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Critical</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Information</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Server Connected</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Server Disconnected</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Player Joined Server</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Player Left Server</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Player kicked (you or by you)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Player kicked</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>You self-muted/deafened</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Other self-muted/deafened</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Player muted (you)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Player muted (by you)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Player muted (other)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Player Joined Channel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Player Left Channel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Permission Denied</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>LogConfig</name>
    <message>
        <source>Messages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Console</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>TTS</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enable console for %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enable Text-To-Speech for %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Text To Speech</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Volume</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Volume of Text-To-Speech Engine</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;b&gt;This is the volume used for the speech synthesis.&lt;/b&gt;</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>LookConfig</name>
    <message>
        <source>Language</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Look and Feel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>System default</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Language to use (requires restart)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets which language Mumble should use.&lt;/b&gt;&lt;br /&gt;You have to restart Mumble to use the new language.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Basic widget style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets the basic look and feel to use.&lt;/b&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Skin</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Skin file to use</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets which skin Mumble should use.&lt;/b&gt;&lt;br /&gt;The skin is a style file applied on top of the basic widget style.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Use Horizontal Splitter</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Use horizontal or vertical splitter for the main window</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets whether the split in the main window is horizontal or vertical.&lt;/b&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>User Interface</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Choose skin file</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <source>Root</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Connect</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Open the server connection dialog</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Shows a dialog of registered servers, and also allows quick connect.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Disconnect</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Disconnect from server</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Disconnects you from the server.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Ban lists</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Edit ban lists on server</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This lets you edit the server-side IP ban lists.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Kick</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Kick player (with reason)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Kick selected player off server. You&apos;ll be asked to specify a reason.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Ban</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Kick and ban player (with reason)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Kick and ban selected player from server. You&apos;ll be asked to specify a reason.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Mute</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mute player</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mute or unmute player on server. Unmuting a deafened player will also undeafen them.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Deafen</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Deafen player</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Deafen or undeafen player on server. Deafening a player will also mute them.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Local Mute</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mute or unmute player locally.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Add new channel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This adds a new sub-channel to the currently selected channel.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Remove channel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This removes a channel and all sub-channels.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Edit ACL</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Edit Groups and ACL for channel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This opens the Group and ACL dialog for the channel, to control permissions.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Link</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Link your channel to another channel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This links your current channel to the selected channel. If they have permission to speak in the other channel, players can now hear each other. This is a permanent link, and will last until manually unlinked or the server is restarted. Please see the shortcuts for push-to-link.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Unlink</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Unlink your channel from another channel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This unlinks your current channel from the selected channel.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Unlink &amp;All</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Unlinks your channel from all linked channels.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This unlinks your current channel (not the selected one) from all linked channels.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Reset</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Reset audio preprocessor</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This will reset the audio preprocessor, including noise cancellation, automatic gain and voice activity detection. If something suddenly worsens the audio environment (like dropping the microphone) and it was temporary, use this to avoid having to wait for the preprocessor to readjust.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Mute Self</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mute yourself</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mute or unmute yourself. When muted, you will not send any data to the server. Unmuting while deafened will also undeafen.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Deafen Self</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Deafen yourself</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Deafen or undeafen yourself. When deafened, you will not hear anything. Deafening yourself will also mute.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Text-To-Speech</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Toggle Text-To-Speech</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enable or disable the text-to-speech engine. Only messages enabled for TTS in the Configuration dialog will actually be spoken.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>S&amp;tatistics</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Display audio statistics</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Pops up a small dialog with information about your current audio input.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Forcibly unlink plugin</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This forces the current plugin to unlink, which is handy if it is reading completely wrong data.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Configure Mumble</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Allows you to change most settings for Mumble.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;What&apos;s This?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enter What&apos;s This? mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Click this to enter &quot;What&apos;s This?&quot; mode. Your cursor will turn into a question mark. Click on any button, menu choice or area to show a description of what it is.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;About</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Information about Mumble</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Shows a small dialog with information and license for Mumble.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>About &amp;Speex</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Information about Speex</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Shows a small dialog with information about Speex.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;About Qt</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Information about Qt</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Shows a small dialog with information about Qt.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Check for &amp;Updates</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Check for new version of Mumble</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Connects to the Mumble webpage to check if a new version is available, and notifies you with an appropriate download URL if this is the case.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mumble -- %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Log of messages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This shows all recent activity. Connecting to servers, errors and information messages all show up here.&lt;br /&gt;To configure exactly which messages show up here, use the &lt;b&gt;Settings&lt;/b&gt; command from the menu.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Server</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Player</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Channel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Audio</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>C&amp;onfigure</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Help</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Kicking player %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enter reason</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Banning player %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mumble</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Channel Name</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Unmuted and undeafened.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Unmuted.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Muted.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Muted and deafened.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Deafened.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Undeafened.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>About Qt</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Joining %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Connected to server.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Server connection failed: %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Disconnected from server.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Reconnecting.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Joined server: %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Left server: %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>%1 is now muted and deafened.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>%1 is now muted.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>%1 is now unmuted.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>server</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>You were muted by %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>You were unmuted by %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>%1 muted by %2.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>%1 unmuted by %2.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>You were deafened by %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>You were undeafened by %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>%1 deafened by %2.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>%1 undeafened by %2.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>You were kicked from the server by %1: %2.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>%3 was kicked from the server by %1: %2.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>You were kicked and banned from the server by %1: %2.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>%3 was kicked and banned from the server by %1: %2.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>%1 left channel.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>%1 moved out by %2.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>%1 entered channel.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>%1 moved in by %2.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Server connection rejected: %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Denied: %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Welcome to Mumble.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Push-to-Talk</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Reset Audio Processor</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Toggle Mute Self</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Toggle Deafen Self</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Force Center Position</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Chan Parent</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Chan Sub#%1</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Chan All Subs</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Push-to-Mute</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Join Channel</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Toggle Overlay</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt Push-to-Talk</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Unlink Plugin</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Are you sure you want to delete %1 and all its sub-channels?</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Overlay</name>
    <message>
        <source>Mumble</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Failed to load overlay library. This means either that:
- the library (mumble_ol.dll) wasn&apos;t found in the directory you ran Mumble from
- you&apos;re on an OS earlier than WinXP SP2
- you do not have the August 2006 updated version of DX9.0c</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>OverlayConfig</name>
    <message>
        <source>Options</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Position</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Font</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enable Overlay</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enable overlay.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This sets whether the overlay is enabled or not. This setting is only checked with D3D9 applications are started, so make sure Mumble is running and this option is on before you start the application.&lt;br /&gt;Please note that if you start the application after starting Mumble, or if you disable the overlay while running, there is no safe way to restart the overlay without also restarting the application.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>No one</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Only talking</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Everyone</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Who to show on the overlay</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets who to show in the in-game overlay.&lt;/b&gt;&lt;br /&gt;If many people are connected to the same channel, the overlay list might be very long. Use this to shorten it.&lt;br /&gt;&lt;i&gt;No one&lt;/i&gt; - Don&apos;t show anyone (but leave overlay running).&lt;br /&gt;&lt;i&gt;Only talking&lt;/i&gt; - Only show talking people.&lt;br /&gt;&lt;i&gt;Everyone&lt;/i&gt; - Show everyone.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Always Show Self</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Always show yourself on overlay.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This sets whether to always show yourself or not. This setting is useful if you aren&apos;t showing everyone in the overlay, as then you would only see your own status if you were talking, which wouldn&apos;t let you see that you were deafened or muted.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Grow Left</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Let overlay grow to the left</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow to the left if needed.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Grow Right</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Let overlay grow to the Right</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow to the Right if needed.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Grow Up</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Let overlay grow upwards</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow to the top if needed.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Grow Down</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Let overlay grow downwards</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow towards the bottom if needed.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>X-Position of Overlay</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This sets the relative X position of the overlay.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Y-Position of Overlay</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This sets the relative Y position of the overlay.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Current Font</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set Font</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Maximum width of names.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This sets the maximum width of names shown, relative to the height. If you set this to 100%, no line of text will be wider than it is high. At 500%, no line will be more than 5 times wider than its height, and so on.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Maximum width</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Change</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color for players</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color for talking players</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color for alt-talking players</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color for Channels</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color for active Channels</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Overlay</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PlayerModel</name>
    <message>
        <source>Name of player</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Name of channel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Player flags</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This is a channel on the server. Only players in the same channel can hear each other.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Flags</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This is a player connected to the server. The icon to the left of the player indicates whether or not they are talking:&lt;br /&gt;&lt;img src=&quot;:/talking_on.png&quot; /&gt; Talking&lt;br /&gt;&lt;img src=&quot;:/talking_off.png&quot; /&gt; Not talking</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This shows the flags the player has on the server, if any:&lt;br /&gt;&lt;img src=&quot;:/authenticated.png&quot; /&gt;Authenticated user&lt;br /&gt;&lt;img src=&quot;:/muted_self.png&quot; /&gt;Muted (by self)&lt;br /&gt;&lt;img src=&quot;:/muted_server.png&quot; /&gt;Muted (by admin)&lt;br /&gt;&lt;img src=&quot;:/deafened_self.png&quot; /&gt;Deafened (by self)&lt;br /&gt;&lt;img src=&quot;:/deafened_server.png&quot; /&gt;Deafened (by admin)&lt;br /&gt;A player muted by himself is probably just away, talking on the phone or something like that.&lt;br /&gt;A player muted by an admin is probably also just away, and the noise the player is making was annoying enough that an admin muted him.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PluginConfig</name>
    <message>
        <source>Options</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Plugins</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enable plugins and transmit positional information</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This enables plugins for supported games to fetch your in-game position and transmit that with each voice packet. This enables other players to hear your voice in-game from the direction your character is in relation to their own.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Reloads all plugins</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This rescans and reloads plugins. Use this if you just added or changed a plugin to the plugins directory.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Information about plugin</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This shows a small information message about the plugin.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show configuration page of plugin</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This shows the configuration page of the plugin, if any.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mumble</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Plugin has no configure function.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Link to Game and Transmit Position</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Reload plugins</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;About</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Configure</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Plugin has no about function.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Plugins</name>
    <message>
        <source>Plugin %1 lost link.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>%1 lost link.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Plugin %1 linked.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>%1 linked.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>VersionCheck</name>
    <message>
        <source>Mumble</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mumble failed to retrieve version information from the SourceForge server.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>XInputKeyWidget</name>
    <message>
        <source>Press Shortcut</source>
        <translation type="unfinished"></translation>
    </message>
</context>
</TS>
