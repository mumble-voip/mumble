<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.0" language="zh_CN">
<context>
    <name>ACLEditor</name>
    <message>
        <location filename="ACLEditor.cpp" line="+47"/>
        <source>Mumble - Edit ACL for %1</source>
        <translation>Mumble - 编辑 访问控制列表 %1</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Deny</source>
        <translation>拒绝</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Allow</source>
        <translation>允许</translation>
    </message>
    <message>
        <location line="+102"/>
        <source>Allow %1</source>
        <translation>允许 %1</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Deny %1</source>
        <translation>拒绝 %1</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This grants the %1 privilege. If a privilege is both allowed and denied, it is denied.&lt;br /&gt;%2</source>
        <translation>授予 %1 权限.如果一个权限同时被拒绝和允许，则被视为拒绝。&lt;br /&gt;%2</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This revokes the %1 privilege. If a privilege is both allowed and denied, it is denied.&lt;br /&gt;%2</source>
        <translation>撤销 %1 权限.如果一个权限同时被拒绝和允许，则被视为拒绝。&lt;br /&gt;%2</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui"/>
        <source>Dialog</source>
        <translation>对话框</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Groups</source>
        <translation>&amp;群</translation>
    </message>
    <message>
        <location/>
        <source>Group</source>
        <translation>群</translation>
    </message>
    <message>
        <location/>
        <source>List of groups</source>
        <translation>群列表</translation>
    </message>
    <message>
        <location/>
        <source>This is all the groups currently defined for the channel. To create a new group, just type in the name and press enter.</source>
        <translation>这是当前频道内定义的所有群。要创建一个新群，请输入群名然后按回车。</translation>
    </message>
    <message>
        <location/>
        <source>Remove selected group</source>
        <translation>删除选中的群</translation>
    </message>
    <message>
        <location/>
        <source>This removes the currently selected group. If the group was inherited, it will not be removed from the list, but all local information about the group will be cleared.</source>
        <translation>删除当前选中的群。如果该群有子群，它将不会被从列表中删除，但是与该群相关的所有本地信息将被清除.</translation>
    </message>
    <message>
        <location/>
        <source>Remove</source>
        <translation>删除</translation>
    </message>
    <message>
        <location/>
        <source>Inherit group members from parent</source>
        <translation>从父继承群成员</translation>
    </message>
    <message>
        <location/>
        <source>This inherits all the members in the group from the parent, if the group is marked as &lt;i&gt;Inheritable&lt;/i&gt; in the parent channel.</source>
        <translation>如果群在父频道里被标记为&lt;i&gt;可继承&lt;/i&gt;,则从父继承所有群成员。</translation>
    </message>
    <message>
        <location/>
        <source>Inherit</source>
        <translation>继承</translation>
    </message>
    <message>
        <location/>
        <source>Make group inheritable to sub-channels</source>
        <translation>使群可被子频道继承</translation>
    </message>
    <message>
        <location/>
        <source>This makes this group inheritable to sub-channels. If the group is non-inheritable, sub-channels are still free to create a new group with the same name.</source>
        <translation>使该群可被子频道继承。如果该群不可被继承，则可在子频道内创建同名新群。</translation>
    </message>
    <message>
        <location/>
        <source>Inheritable</source>
        <translation>可继承</translation>
    </message>
    <message>
        <location/>
        <source>Group was inherited from parent channel</source>
        <translation>继承自父频道的群</translation>
    </message>
    <message>
        <location/>
        <source>This indicates that the group was inherited from the parent channel. You cannot edit this flag, it&apos;s just for information.</source>
        <translation>这表明该群继承自父频道。该标志仅供参考，你不能编辑该标志。</translation>
    </message>
    <message>
        <location/>
        <source>Inherited</source>
        <translation>继承的</translation>
    </message>
    <message>
        <location/>
        <source>Members</source>
        <translation>成员</translation>
    </message>
    <message>
        <location/>
        <source>Add</source>
        <translation>添加</translation>
    </message>
    <message>
        <location/>
        <source>Add to Remove</source>
        <translation>添加到删除列表</translation>
    </message>
    <message>
        <location/>
        <source>Add member to group</source>
        <translation>添加群成员</translation>
    </message>
    <message>
        <location/>
        <source>Type in the name of a player you wish to add to the group and click Add.</source>
        <translation>输入你要添加到群里的玩家名字然后点击添加。</translation>
    </message>
    <message>
        <location/>
        <source>Remove member from group</source>
        <translation>从群里删除成员</translation>
    </message>
    <message>
        <location/>
        <source>Type in the name of a player you wish to remove from the group and click Add.</source>
        <translation>输入你要从群内删除的成员然后点击删除。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;ACL</source>
        <translation>&amp;访问控制列表</translation>
    </message>
    <message>
        <location/>
        <source>Active ACLs</source>
        <translation>当前访问控制列表</translation>
    </message>
    <message>
        <location/>
        <source>List of entries</source>
        <translation>条目列表</translation>
    </message>
    <message>
        <location/>
        <source>This shows all the entries active on this channel. Entries inherited from parent channels will be show in italics.&lt;br /&gt;ACLs are evaluated top to bottom, meaning priority increases as you move down the list.</source>
        <translation>显示该频道内所有活跃条目。继承自父频道的条目将以斜体显示.&lt;br/&gt;访问控制列表是自上而下处理，意味着列表越往下优先级越高。</translation>
    </message>
    <message>
        <location/>
        <source>Inherit ACL of parent?</source>
        <translation>继承父访问控制列表？</translation>
    </message>
    <message>
        <location/>
        <source>This sets whether or not the ACL up the chain of parent channels are applied to this object. Only those entries that are marked in the parent as &quot;Apply to sub-channels&quot; will be inherited.</source>
        <translation>设置是否对当前对象应用父频道的访问控制列表。只有那些在父频道内被标记为“应用到子频道”的条目将被继承。</translation>
    </message>
    <message>
        <location/>
        <source>Inherit ACLs</source>
        <translation>继承访问控制列表</translation>
    </message>
    <message>
        <location/>
        <source>Move entry up</source>
        <translation>上移条目</translation>
    </message>
    <message>
        <location/>
        <source>This moves the entry up in the list. As entries are evaluated in order, this may change the effective permissions of users. You cannot move an entry above an inherited entry, if you really need that you&apos;ll have to duplicate the inherited entry.</source>
        <translation>在列表中上移条目。由于条目自上而下被处理，这有可能改变用户的有效权限.你不能将一个条目移到继承的条目之上，如果你确实想这么做，你需要复制继承的条目。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Up</source>
        <translation>&amp;上移</translation>
    </message>
    <message>
        <location/>
        <source>Move entry down</source>
        <translation>下移条目</translation>
    </message>
    <message>
        <location/>
        <source>This moves the entry down in the list. As entries are evaluated in order, this may change the effective permissions of users.</source>
        <translation>在列表中下移条目。由于条目自上而下被处理，这有可能改变用户的有效权限。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Down</source>
        <translation>&amp;下移</translation>
    </message>
    <message>
        <location/>
        <source>Add new entry</source>
        <translation>添加新条目</translation>
    </message>
    <message>
        <location/>
        <source>This adds a new entry, initially set with no permissions and applying to all.</source>
        <translation>添加一个新条目，初始设置为无权限并应用到所有。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Add</source>
        <translation>&amp;添加</translation>
    </message>
    <message>
        <location/>
        <source>Remove entry</source>
        <translation>删除条目</translation>
    </message>
    <message>
        <location/>
        <source>This removes the currently selected entry.</source>
        <translation>删除当前选中的条目。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Remove</source>
        <translation>&amp;删除</translation>
    </message>
    <message>
        <location/>
        <source>Context</source>
        <translation>上下文</translation>
    </message>
    <message>
        <source>Entry should apply to this channel</source>
        <translation type="obsolete">应用到该频道的条目</translation>
    </message>
    <message>
        <location/>
        <source>This makes the entry apply to this channel.</source>
        <translation>应用该条目到当前频道。</translation>
    </message>
    <message>
        <location/>
        <source>Applies to this channel</source>
        <translation>应用到当前频道</translation>
    </message>
    <message>
        <location/>
        <source>Entry should apply to sub-channels.</source>
        <translation>应用到子频道的条目。</translation>
    </message>
    <message>
        <location/>
        <source>This makes the entry apply to sub-channels of this channel.</source>
        <translation>应用该条目到当前频道的子频道。</translation>
    </message>
    <message>
        <location/>
        <source>Applies to sub-channels</source>
        <translation>应用到子频道</translation>
    </message>
    <message>
        <location/>
        <source>Permissions</source>
        <translation>权限</translation>
    </message>
    <message>
        <location/>
        <source>User/Group</source>
        <translation>用户/群</translation>
    </message>
    <message>
        <location/>
        <source>Group this entry applies to</source>
        <translation>应用该条目的群</translation>
    </message>
    <message>
        <location/>
        <source>This controls which group of users this entry applies to.&lt;br /&gt;Note that the group is evaluated in the context of the channel the entry is used in. For example, the default ACL on the Root channel gives &lt;i&gt;Write&lt;/i&gt; permission to the &lt;i&gt;admin&lt;/i&gt; group. This entry, if inherited by a channel, will give a user write privileges if he belongs to the &lt;i&gt;admin&lt;/i&gt; group in that channel, even if he doesn&apos;t belong to the &lt;i&gt;admin&lt;/i&gt; group in the channel where the ACL originated.&lt;br /&gt;If a group name starts with a !, its membership is negated, and if it starts with a ~, it is evaluated in the channel the ACL was defined in, rather than the channel the ACL is active in. Order is important; &lt;i&gt;!~in&lt;/i&gt; is valid, but &lt;i&gt;~!in&lt;/i&gt; is not.&lt;br /&gt;A few special predefined groups are:&lt;br /&gt;&lt;b&gt;all&lt;/b&gt; - Everyone will match.&lt;br /&gt;&lt;b&gt;auth&lt;/b&gt; - All authenticated users will match.&lt;br /&gt;&lt;b&gt;sub,a,b,c&lt;/b&gt; - User currently in a sub-channel minimum &lt;i&gt;a&lt;/i&gt; common parents, and between &lt;i&gt;b&lt;/i&gt; and &lt;i&gt;c&lt;/i&gt; channels down the chain. See the website for more extensive documentation on this one.&lt;br /&gt;&lt;b&gt;in&lt;/b&gt; - Users currently in the channel will match (convenience for &lt;i&gt;sub,0,0,0&lt;/i&gt;).&lt;br /&gt;&lt;b&gt;out&lt;/b&gt; - Users outside the channel will match (convenience for &lt;i&gt;!sub,0,0,0&lt;/i&gt;).&lt;br /&gt;Note that an entry applies to either a user or a group, not both.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>User ID</source>
        <translation>用户名</translation>
    </message>
    <message>
        <location/>
        <source>User this entry applies to</source>
        <translation>应用该条目的用户</translation>
    </message>
    <message>
        <location/>
        <source>This controls which user this entry applies to. Just type in the user name and hit enter to query the server for a match.</source>
        <translation>控制该条目应用到哪个用户。请输入用户名然后按回车来向服务器做匹配查询。</translation>
    </message>
    <message>
        <location/>
        <source>Entry should apply to this channel.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ALSAAudioInput</name>
    <message>
        <location filename="ALSAAudio.cpp" line="+169"/>
        <source>Default ALSA Card</source>
        <translation>默认 ALSA 声卡</translation>
    </message>
    <message>
        <location line="+191"/>
        <source>Opening chosen ALSA Input failed: %1</source>
        <translation>无法打开所选 ALSA 输入: %1</translation>
    </message>
</context>
<context>
    <name>ALSAAudioOutput</name>
    <message>
        <location line="-190"/>
        <source>Default ALSA Card</source>
        <translation>默认 ALSA 声卡</translation>
    </message>
    <message>
        <location line="+325"/>
        <source>Opening chosen ALSA Output failed: %1</source>
        <translation>无法打开所选 ALSA 输出: %1</translation>
    </message>
</context>
<context>
    <name>ASIOConfig</name>
    <message>
        <source>%1 (ver %2)</source>
        <translation type="obsolete">%1 （版本 %2）</translation>
    </message>
    <message>
        <source>%1 ms -&gt; %2 ms (%3 ms resolution) %4Hz</source>
        <translation type="obsolete">%1 毫秒 -&gt; %2 毫秒 （%3 毫秒 分辨率） %4赫兹</translation>
    </message>
    <message>
        <source>%1 ms -&gt; %2 ms (%3 ms resolution) %4Hz -- Unusable</source>
        <translation type="obsolete">%1 毫秒 -&gt; %2 毫秒 （%3 毫秒 分辨率） %4 赫兹--不可用</translation>
    </message>
    <message>
        <location filename="ASIOInput.cpp" line="+222"/>
        <source>%1 (version %2)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>%1 -&gt; %2 samples buffer, with %3 sample resolution (%4 preferred) at %5 Hz</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+43"/>
        <location line="+25"/>
        <source>ASIO Initialization failed: %1</source>
        <translation>ASIO初始化失败: %1</translation>
    </message>
    <message>
        <location line="-21"/>
        <location line="+25"/>
        <source>Failed to instantiate ASIO driver</source>
        <translation>无法初始化 ASIO 驱动</translation>
    </message>
    <message>
        <location line="+37"/>
        <source>ASIO</source>
        <translation></translation>
    </message>
    <message>
        <location filename="ASIOInput.ui"/>
        <source>Form</source>
        <translation>表单</translation>
    </message>
    <message>
        <location/>
        <source>Device selection</source>
        <translation>设备选择</translation>
    </message>
    <message>
        <location/>
        <source>Device</source>
        <translation>设备</translation>
    </message>
    <message>
        <location/>
        <source>Device to use for microphone</source>
        <translation>用作麦克风的设备</translation>
    </message>
    <message>
        <location/>
        <source>This chooses what device to query. You still need to actually query the device and select which channels to use.</source>
        <translation>选择要查询的设备。你仍需要实际查询该设备然后选择要使用哪些声道。</translation>
    </message>
    <message>
        <location/>
        <source>Query selected device</source>
        <translation>查询所选设备</translation>
    </message>
    <message>
        <location/>
        <source>This queries the selected device for channels. Be aware that many ASIO drivers are buggy to the extreme, and querying them might cause a crash of either the application or the system.</source>
        <translation>查询所选设备的声道。注意大多数 ASIO 驱动有极端漏洞，查询设备可能导致应用程序或系统崩溃。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Query</source>
        <translation>&amp;查询</translation>
    </message>
    <message>
        <location/>
        <source>Configure selected device</source>
        <translation>配置所选设备</translation>
    </message>
    <message>
        <location/>
        <source>This configures the selected device. Be aware that many ASIO drivers are buggy to the extreme, and querying them might cause a crash of either the application or the system.</source>
        <translation>配置所选设备。注意大多数 ASIO 驱动有极端漏洞，查询设备可能导致应用程序或系统崩溃。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Configure</source>
        <translation>&amp;配置</translation>
    </message>
    <message>
        <location/>
        <source>Capabilities</source>
        <translation>设置信息</translation>
    </message>
    <message>
        <location/>
        <source>Driver name</source>
        <translation>驱动名称</translation>
    </message>
    <message>
        <source>TextLabel</source>
        <translation type="obsolete">文本标签</translation>
    </message>
    <message>
        <source>Buffersize</source>
        <translation type="obsolete">缓冲区大小</translation>
    </message>
    <message>
        <location/>
        <source>Configure input channels</source>
        <translation>配置输入声道</translation>
    </message>
    <message>
        <source>This will configure the input channels for ASIO. Make sure you select at least one channel as microphone and speaker. &lt;i&gt;Microphone&lt;/i&gt; should be where your microphone is attached, and &lt;i&gt;Speaker&lt;/i&gt; should be a channel that samples &lt;i&gt;What you hear&lt;/i&gt;.&lt;br /&gt;For example, on the Audigy 2 ZS, a good selection for Microphone would be &lt;i&gt;Mic L&lt;/i&gt; while Speaker should be &lt;i&gt;Mix L&lt;/i&gt; and &lt;i&gt;Mix R&lt;/i&gt;.</source>
        <translation type="obsolete">配置 ASIO 的输入声道.请确保你选择了至少一个声道作为麦克风和扬声器。&lt;i&gt;麦克风&lt;/i&gt;应该是你的已连接的麦克风连，&lt;i&gt;扬声器&lt;/i&gt;应该是一个采样&lt;i&gt;你听到的声音&lt;/i&gt;的声道。&lt;br /&gt;比如在 Audigy 2 ZS 上，一个好的麦克风的选择会是&lt;i&gt; Mic L &lt;/i&gt;而扬声器会是&lt;i&gt; Mix L &lt;/i&gt;和&lt;i&gt; Mix R&lt;/i&gt;.</translation>
    </message>
    <message>
        <location/>
        <source>Channels</source>
        <translation>声道</translation>
    </message>
    <message>
        <location/>
        <source>Microphone</source>
        <translation>麦克风</translation>
    </message>
    <message>
        <location/>
        <source>&lt;-</source>
        <translation></translation>
    </message>
    <message>
        <location/>
        <source>-&gt;</source>
        <translation></translation>
    </message>
    <message>
        <location/>
        <source>Unused</source>
        <translation>未使用</translation>
    </message>
    <message>
        <location/>
        <source>Speakers</source>
        <translation>扬声器</translation>
    </message>
    <message>
        <location/>
        <source>Buffer size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This will configure the input channels for ASIO. Make sure you select at least one channel as microphone and speaker. &lt;i&gt;Microphone&lt;/i&gt; should be where your microphone is attached, and &lt;i&gt;Speaker&lt;/i&gt; should be a channel that samples &apos;&lt;i&gt;What you hear&lt;/i&gt;&apos;.&lt;br /&gt;For example, on the Audigy 2 ZS, a good selection for Microphone would be &apos;&lt;i&gt;Mic L&lt;/i&gt;&apos; while Speaker should be &apos;&lt;i&gt;Mix L&lt;/i&gt;&apos; and &apos;&lt;i&gt;Mix R&lt;/i&gt;&apos;.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ASIOInput</name>
    <message>
        <location filename="ASIOInput.cpp" line="+80"/>
        <source>You need to select at least one microphone and one speaker source to use ASIO. If you just need microphone sampling, use DirectSound.</source>
        <translation>你需要选择至少一个麦克风和一个扬声器源来使用 ASIO.如果你只需要使用要麦克风进行采样,请使用 DirectSound.</translation>
    </message>
    <message>
        <location line="+103"/>
        <source>Opening selected ASIO device failed. No input will be done.</source>
        <translation>无法打开所选 ASIO 设备。没有可用输入。</translation>
    </message>
</context>
<context>
    <name>AboutDialog</name>
    <message>
        <location filename="About.cpp" line="+37"/>
        <source>About Mumble</source>
        <translation>关于 Mumble</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>&lt;h3&gt;Mumble (%1)&lt;/h3&gt;&lt;p&gt;Copyright %3 Thorvald Natvig&lt;br /&gt;slicer@users.sourceforge.net&lt;/p&gt;&lt;p&gt;&lt;b&gt;A voice-chat utility for gamers&lt;/b&gt;&lt;/p&gt;&lt;p&gt;&lt;tt&gt;&lt;a href=&quot;%2&quot;&gt;%2&lt;/a&gt;&lt;/tt&gt;&lt;/p&gt;</source>
        <translation>&lt;h3&gt;Mumble (%1)&lt;/h3&gt;&lt;p&gt;版权所有 %3 Thorvald Natvig&lt;br /&gt;slicer@users.sourceforge.net&lt;/p&gt;&lt;p&gt;&lt;b&gt;一个游戏玩家的语音聊天工具&lt;/b&gt;&lt;/p&gt;&lt;p&gt;&lt;tt&gt;&lt;a href=&quot;%2&quot;&gt;%2&lt;/a&gt;&lt;/tt&gt;&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>&amp;About Mumble</source>
        <translation>&amp;关于 Mumble</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>&amp;License</source>
        <translation>&amp;授权</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>OK</source>
        <translation></translation>
    </message>
</context>
<context>
    <name>AboutSpeexDialog</name>
    <message>
        <location line="+11"/>
        <source>About Speex</source>
        <translation>关于 Speex</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>&lt;h3&gt;About Speex&lt;/h3&gt;&lt;p&gt;&lt;tt&gt;&lt;a href=&quot;%2&quot;&gt;%2&lt;/a&gt;&lt;/tt&gt;&lt;/p&gt;&lt;p&gt;This program uses Speex version %1&lt;/p&gt;&lt;p&gt;Speex is used for echo cancellation, noise&lt;br /&gt;filtering, voice activity detection and speech&lt;br /&gt;compression.&lt;/p&gt;</source>
        <translation>&lt;h3&gt;关于 Speex&lt;/h3&gt;&lt;p&gt;&lt;tt&gt;&lt;a href=&quot;%2&quot;&gt;%2&lt;/a&gt;&lt;/tt&gt;&lt;/p&gt;&lt;p&gt;该程序使用 Speex 版本 %1&lt;/p&gt;&lt;p&gt;Speex 被用于回声消除，噪音&lt;br /&gt;过滤，语言活动检测和语音&lt;br /&gt;压缩。&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>OK</source>
        <translation></translation>
    </message>
</context>
<context>
    <name>AudioInput</name>
    <message>
        <location filename="AudioInput.ui"/>
        <source>Form</source>
        <translation>表单</translation>
    </message>
    <message>
        <location/>
        <source>Interface</source>
        <translation>接口</translation>
    </message>
    <message>
        <location/>
        <source>System</source>
        <translation>系统</translation>
    </message>
    <message>
        <location/>
        <source>Input method for audio</source>
        <translation>音频输入方法</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is the input method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation>&lt;b&gt;这是用于音频的输入方法。&lt;/b&gt;&lt;br /&gt;很有可能你想使用 DirectSound。</translation>
    </message>
    <message>
        <location/>
        <source>Device</source>
        <translation>设备</translation>
    </message>
    <message>
        <source>Output method for audio</source>
        <translation type="obsolete">音频输出方法</translation>
    </message>
    <message>
        <source>&lt;b&gt;This is the output method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation type="obsolete">&lt;b&gt;这是用于音频输出的方法。&lt;/b&gt;&lt;br /&gt;很有可能你想使用 DirectSound。</translation>
    </message>
    <message>
        <location/>
        <source>Cancel Echo</source>
        <translation>消除回声</translation>
    </message>
    <message>
        <location/>
        <source>Transmission</source>
        <translation>传送</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Transmit</source>
        <translation>&amp;传送</translation>
    </message>
    <message>
        <location/>
        <source>When to transmit your speech</source>
        <translation>何时传送你的语音</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets when speech should be transmitted.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;Continuous&lt;/i&gt; - All the time&lt;br /&gt;&lt;i&gt;Voice Activity&lt;/i&gt; - When you are speaking clearly.&lt;br /&gt;&lt;i&gt;Push To Talk&lt;/i&gt; - When you hold down the hotkey set under &lt;i&gt;Shortcuts&lt;/i&gt;.</source>
        <translation>&lt;b&gt;设置何时语音将被传送。&lt;/b&gt;&lt;br /&gt;&lt;i&gt;连续&lt;/i&gt;一直&lt;br /&gt;&lt;i&gt;有语音活动&lt;/i&gt; 当你说话清晰时。&lt;br /&gt;&lt;i&gt;按键通话&lt;/i&gt; - 当你按住作为&lt;i&gt;快捷键&lt;/i&gt;的自定义按键组合时。</translation>
    </message>
    <message>
        <location/>
        <source>DoublePush Time</source>
        <translation>两次按键时间间隔</translation>
    </message>
    <message>
        <location/>
        <source>TextLabel</source>
        <translation>文本标签</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This enables the audio cues for push to talk.&lt;/b&gt;&lt;br /&gt;Setting this will give you a short audio beep when push to talk is pressed and released.</source>
        <translation>&lt;b&gt;启用按键通话的音频提示。&lt;/b&gt;&lt;br /&gt;当你按下或释放按键通话快捷键时会听到一声短的蜂鸣声。</translation>
    </message>
    <message>
        <location/>
        <source>PTT Audio cue</source>
        <translation>PTT 提示音</translation>
    </message>
    <message>
        <location/>
        <source>Use SNR based speech detection</source>
        <translation>使用基于 SNR 的语音侦测</translation>
    </message>
    <message>
        <location/>
        <source>Signal to Noise</source>
        <translation>信噪比</translation>
    </message>
    <message>
        <location/>
        <source>Use Amplitude based speech detection.</source>
        <translation>使用基于振幅的语音侦测。</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets speech detection to use Amplitude.&lt;/b&gt;&lt;br /&gt;In this mode, the raw strength of the input signal is used to detect speech.</source>
        <translation>&lt;b&gt;设置用振幅来做语音侦测。&lt;/b&gt;&lt;br /&gt;在此模式下，输入信号的原始强度被用于语音侦测。</translation>
    </message>
    <message>
        <location/>
        <source>Amplitude</source>
        <translation>振幅</translation>
    </message>
    <message>
        <location/>
        <source>Voice &amp;Hold</source>
        <translation>&amp;暂停 发言</translation>
    </message>
    <message>
        <location/>
        <source>How long to keep transmitting after silence</source>
        <translation>无声后多长时间内继续传送</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This selects how long after a perceived stop in speech transmission should continue.&lt;/b&gt;&lt;br /&gt;Set this higher if your voice breaks up when you speak (seen by a rapidly blinking voice icon next to your name).&lt;br /&gt;Only makes sense when used with Voice Activity transmission.</source>
        <translation>&lt;b&gt;设置发言暂停被感知到后多长时间内继续传送。&lt;/b&gt;如果你在讲话时你的语音破碎（在你的名字旁边会显示一个快速闪烁的语音图标），增大这个值。&lt;br /&gt;只有在使用检测到语音活动时传送时该选项才有用。</translation>
    </message>
    <message>
        <location/>
        <source>Silence Below</source>
        <translation>无声 低于</translation>
    </message>
    <message>
        <location/>
        <source>Signal values below this counts as silence</source>
        <translation>信号值低于这个数值时视为无声</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the trigger values for voice detection.&lt;/b&gt;&lt;br /&gt;Use this together with the Audio Statistics window to manually tune the trigger values for detecting speech. Input values below &quot;Silence Below&quot; always count as silence. Values above &quot;Speech Above&quot; always count as voice. Values in between will count as voice if you&apos;re already talking, but will not trigger a new detection.</source>
        <translation>&lt;b&gt;设置语音检测的触发值。&lt;/b&gt;&lt;br /&gt;与音频统计窗口一起来使用该值以手工调整语音侦测的触发值。输入值低于&quot;无声 低于“时总是会被视为无声。值高于”语音 高于“时总是被视为语音。如果你正在讲话，两者之间的值会被视为语音，但是不会触发一个新的侦测。</translation>
    </message>
    <message>
        <location/>
        <source>Speech Above</source>
        <translation>语言 高于</translation>
    </message>
    <message>
        <location/>
        <source>Signal values above this count as voice</source>
        <translation>信号值高于这个数值时视为语音</translation>
    </message>
    <message>
        <location/>
        <source>Compression</source>
        <translation>压缩</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Quality</source>
        <translation>&amp;质量</translation>
    </message>
    <message>
        <location/>
        <source>Quality of compression (peak bandwidth)</source>
        <translation>压缩质量(峰值带宽）</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the quality of compression.&lt;/b&gt;&lt;br /&gt;This determines how much bandwidth Mumble is allowed to use for outgoing audio.</source>
        <translation>&lt;b&gt;设置压缩质量。&lt;/b&gt;&lt;br /&gt;确定 Mumble 可用于发送音频的带宽大小。</translation>
    </message>
    <message>
        <location/>
        <source>Audio per packet</source>
        <translation>每个数据包的音频帧数</translation>
    </message>
    <message>
        <location/>
        <source>How many audio frames to send per packet</source>
        <translation>每个数据包里发送多少个音频帧</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This selects how many audio frames should be put in one packet.&lt;/b&gt;&lt;br /&gt;Increasing this will increase the latency of your voice, but will also reduce bandwidth requirements.</source>
        <translation>&lt;b&gt;选择一个数据包里放多少个音频帧。&lt;/b&gt;&lt;br /&gt;增加这个数值会增加你的语音延迟，但也会减少带宽需求。</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This shows peak outgoing bandwidth used.&lt;/b&gt;&lt;br /&gt;This shows the peak amount of bandwidth sent out from your machine. Audio bitrate is the maximum bitrate (as we use VBR) for the audio data alone. Position is the bitrate used for positional information. Overhead is our framing and the IP packet headers (IP and UDP is 75% of this overhead).</source>
        <translation>&lt;b&gt;显示已用的发送带宽峰值。&lt;/b&gt;&lt;br /&gt;显示从你的机器发出的峰值带宽大小。音频比特率是仅用于音频数据的最大比特率(由于我们使用VBR)。位置是用于位置信息的比特率。系统开销是我们的组帧和 IP 数据包头(IP 和 UDP 是该系统开销的75%)。</translation>
    </message>
    <message>
        <location/>
        <source>Audio Processing</source>
        <translation>音频处理</translation>
    </message>
    <message>
        <location/>
        <source>Noise Suppression</source>
        <translation>噪音抑制</translation>
    </message>
    <message>
        <location/>
        <source>Noise suppression</source>
        <translation>噪音抑制</translation>
    </message>
    <message>
        <location/>
        <source>Amplification</source>
        <translation>放大率</translation>
    </message>
    <message>
        <location/>
        <source>Maximum amplification of input sound</source>
        <translation>输入声音的最大放大率</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Maximum amplification of input.&lt;/b&gt;&lt;br /&gt;Mumble normalizes the input volume before compressing, and this sets how much it&apos;s allowed to amplify.&lt;br /&gt;The actual level is continually updated based on your current speech pattern, but it will never go above the level specified here.&lt;br /&gt;If the &lt;i&gt;Microphone loudness&lt;/i&gt; level of the audio statistics hover around 100%, you probably want to set this to 2.0 or so, but if, like most people, you are unable to reach 100%, set this to something much higher.&lt;br /&gt;Ideally, set it so &lt;i&gt;Microphone Loudness * Amplification Factor &gt;= 100&lt;/i&gt;, even when you&apos;re speaking really soft.&lt;br /&gt;&lt;br /&gt;Note that there is no harm in setting this to maximum, but Mumble will start picking up other conversations if you leave it to auto-tune to that level.</source>
        <translation>&lt;b&gt;输入的最大放大率。&lt;/b&gt;&lt;br /&gt;Mumble 在压缩前会规格化输入音量，该选项设置音量被允许放大多少。&lt;br /&gt;实际等级将根据你的语音模式来持续更新，但是不会高于这里指定的等级。&lt;br /&gt;如果音频统计的&lt;i&gt;麦克风响度&lt;/i&gt;等级在 100%左右，你很有可能想把该值设为大约 2.0，但是如果像大多数人一样，你不能达到 100%，设置一个更高的值。&lt;br /&gt;理想状态下，设置该值使&lt;i&gt;麦克风响度 * 放大率 &gt;= 100&lt;/i&gt;,甚至当你相当轻声的讲话时。&lt;br /&gt;&lt;br /&gt;注意设置该值为其最大值不会有任何副作用，但是如果你将该值设为自动调节到其最大值， Mumble 会开始拾取其他人的谈话。</translation>
    </message>
    <message>
        <location/>
        <source>Current speech detection chance</source>
        <translation>当前语音侦测几率</translation>
    </message>
    <message>
        <location/>
        <source>Audible audio cue when push-to-talk is activated or deactivated</source>
        <translation>当按键通话激活或未激活时听到的提示音频</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets speech detection to use Signal to Noise ratio.&lt;/b&gt;&lt;br /&gt;In this mode, the input is analyzed for something resembling a clear signal, and the clarity of that signal is used to trigger speech detection.</source>
        <translation>&lt;b&gt;设置使用信噪比来进行语音侦测。&lt;/b&gt;&lt;br /&gt; 在此模式下，将从输入中寻找并分析出一个近似清晰的信号，而这个信号的清晰度被用于触发语音侦测。</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This shows the current speech detection settings.&lt;/b&gt;&lt;br /&gt;You can change the settings from the Settings dialog or from the Audio Wizard.</source>
        <translation>&lt;b&gt;显示当前语音侦测设置。&lt;/b&gt;&lt;br /&gt;你能在设置对话框或音频精灵里修改当前设置。</translation>
    </message>
    <message>
        <location/>
        <source>Idle AutoMute</source>
        <translation>空闲时自动静音</translation>
    </message>
    <message>
        <location/>
        <source>How long to remain idle before auto-mute.</source>
        <translation>空闲多长时间后自动静音。</translation>
    </message>
    <message>
        <location/>
        <source>This sets an idle timer. If the timer expires without any audio being sent to the server, you are muted.</source>
        <translation>设置空闲定时器。如果定时器过期时没有任何音频发送到服务器，你将被静音。</translation>
    </message>
    <message>
        <location/>
        <source>Maximum bandwidth used for sending audio</source>
        <translation>用于发送音频的最大带宽</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the amount of noise suppression to apply.&lt;/b&gt;&lt;br /&gt;The higher this value, the more aggressively stationary noise will be supressed.</source>
        <translation>&lt;b&gt;设置应用于噪音抑制的数值大小.&lt;/b&gt;&lt;br /&gt;数值越高，稳态噪声越会被积极的抑制。</translation>
    </message>
    <message>
        <location/>
        <source>Input device for audio</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is the input device to use for audio.&lt;/b&gt;</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>AudioInputDialog</name>
    <message>
        <location filename="AudioConfigDialog.cpp" line="+74"/>
        <source>Continuous</source>
        <translation>连续</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Voice Activity</source>
        <translation>语音活动</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Push To Talk</source>
        <translation>按键通话</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>Audio Input</source>
        <translation>音频输入</translation>
    </message>
    <message>
        <location line="+79"/>
        <location line="+8"/>
        <source>%1 ms</source>
        <translation>%1 毫秒</translation>
    </message>
    <message>
        <location line="-2"/>
        <location line="+30"/>
        <source>Off</source>
        <translation>关闭</translation>
    </message>
    <message>
        <location line="-22"/>
        <source>%1 s</source>
        <translation>%1 秒</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>-%1 dB</source>
        <translation>-%1 分贝</translation>
    </message>
    <message>
        <location line="+58"/>
        <source>%1kbit/s (Audio %2, Position %4, Overhead %3)</source>
        <translation>%1 千比特/秒 （音频 %2, 位置 %4, 系统开销 %3）</translation>
    </message>
    <message>
        <location line="-47"/>
        <source>%1 min</source>
        <translation>%1 分钟</translation>
    </message>
</context>
<context>
    <name>AudioOutput</name>
    <message>
        <location filename="AudioOutput.ui"/>
        <source>Form</source>
        <translation>表单</translation>
    </message>
    <message>
        <location/>
        <source>Interface</source>
        <translation>接口</translation>
    </message>
    <message>
        <location/>
        <source>System</source>
        <translation>系统</translation>
    </message>
    <message>
        <source>Input method for audio</source>
        <translation type="obsolete">音频输入方法</translation>
    </message>
    <message>
        <source>&lt;b&gt;This is the input method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation type="obsolete">&lt;b&gt;这是用于音频的输入方法。&lt;/b&gt;&lt;br /&gt;很有可能你想使用 DirectSound。</translation>
    </message>
    <message>
        <location/>
        <source>Device</source>
        <translation>设备</translation>
    </message>
    <message>
        <location/>
        <source>Output method for audio</source>
        <translation>音频输出方法</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is the output method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation>&lt;b&gt;这是用于音频输出的方法。&lt;/b&gt;&lt;br /&gt;很有可能你想使用 DirectSound。</translation>
    </message>
    <message>
        <location/>
        <source>Positional Audio</source>
        <translation>定位音频</translation>
    </message>
    <message>
        <location/>
        <source>Audio Output</source>
        <translation>音频输出</translation>
    </message>
    <message>
        <location/>
        <source>Default &amp;Jitter Buffer</source>
        <translation>默认 &amp;Jitter 缓冲区</translation>
    </message>
    <message>
        <location/>
        <source>Safety margin for jitter buffer</source>
        <translation>Jitter 缓冲区的安全边界</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the minimum safety margin for the jitter buffer.&lt;/b&gt;&lt;br /&gt;All incoming audio is buffered, and the jitter buffer continually tries to push the buffer to the minimum sustainable by your network, so latency can be as low as possible. This sets the minimum buffer size to use. If the start of sentences you hear is very jittery, increase this value.</source>
        <translation>&lt;b&gt;设置 Jitter 缓冲区的最小安全边界. &lt;/b&gt;&lt;br /&gt;所有收到的音频都会被缓冲，而 Jitter 缓冲区不断尝试使该缓冲区保持能被你的网络所允许的最小值, 从而使延迟尽可能的低。 设置使用的缓冲区大小的最小值。如果你听到的句子的起始部分是颤音，请增加这个值。</translation>
    </message>
    <message>
        <location/>
        <source>TextLabel</source>
        <translation>文本标签</translation>
    </message>
    <message>
        <location/>
        <source>Volume</source>
        <translation>音量</translation>
    </message>
    <message>
        <location/>
        <source>Volume of incoming speech</source>
        <translation>收到的语音的音量</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This adjusts the volume of incoming speech.&lt;/b&gt;&lt;br /&gt;Note that if you increase this beyond 100%, audio will be distorted.</source>
        <translation>&lt;b&gt;调节收到的语音的音量。&lt;/b&gt;&lt;br /&gt;注意如果你增加此值至超过 100%，声音会失真。</translation>
    </message>
    <message>
        <location/>
        <source>Output Delay</source>
        <translation>输出延时</translation>
    </message>
    <message>
        <location/>
        <source>Amount of data to buffer</source>
        <translation>缓冲数据大小</translation>
    </message>
    <message>
        <location/>
        <source>This sets the amount of data to prebuffer in the output buffer. Experiment with different values and set it to the lowest which doesn&apos;t cause rapid jitter in the sound.</source>
        <translation>设置输出缓冲区里前置缓冲的数据大小。试验不同值然后设置成不引起快速颤音的最低值.</translation>
    </message>
    <message>
        <location/>
        <source>The connected &quot;speakers&quot; are actually headphones.</source>
        <translation>已连接的“扬声器”实际上是耳机.</translation>
    </message>
    <message>
        <location/>
        <source>This sets the minimum distance for sound calculations. The volume of other players&apos; speech will not decrease until they are at least this far away from you.</source>
        <translation>设置声强计算的最小距离.当别的玩家离你小于这个距离时他的语音将不会减弱.</translation>
    </message>
    <message>
        <location/>
        <source>Factor for sound volume decrease</source>
        <translation>音量减弱因子</translation>
    </message>
    <message>
        <location/>
        <source>Bloom</source>
        <translation>声音增强</translation>
    </message>
    <message>
        <location/>
        <source>Factor for sound volume increase</source>
        <translation>音量增强因子</translation>
    </message>
    <message>
        <location/>
        <source>How much should sound volume increase for sources that are really close?</source>
        <translation>当音源距离过近时音量该增强多少?</translation>
    </message>
    <message>
        <location/>
        <source>Checking this indicates that you don&apos;t have speakers connected, just headphones. This is important, as speakers are usually in front of you, while headphones are directly to your left/right.</source>
        <translation>选中此选项表明你没有连接扬声器，只有耳机。这很重要，因为扬声器通常在你面前，而耳机直接在你左右。</translation>
    </message>
    <message>
        <location/>
        <source>Headphones</source>
        <translation>耳机</translation>
    </message>
    <message>
        <location/>
        <source>Minimum Distance</source>
        <translation>最小距离</translation>
    </message>
    <message>
        <location/>
        <source>Minimum distance to player before sound volume decreases</source>
        <translation>距离玩家至少多远时声音开始减弱</translation>
    </message>
    <message>
        <location/>
        <source>Maximum Distance</source>
        <translation>最大距离</translation>
    </message>
    <message>
        <location/>
        <source>Maximum distance, beyond which speech volume won&apos;t decrease</source>
        <translation>超过最大距离时音量将不再减弱</translation>
    </message>
    <message>
        <location/>
        <source>This sets the maximum distance for sound calculations. When farther away than this, other players&apos; speech volume will not decrease any further.</source>
        <translation>设置声强计算最大距离。当超过此距离时，别的玩家的音量将不再减弱。</translation>
    </message>
    <message>
        <location/>
        <source>Minimum Volume</source>
        <translation>最小音量</translation>
    </message>
    <message>
        <location/>
        <source>What should the volume be at the maximum distance?</source>
        <translation>位于最大距离时的音量大小?</translation>
    </message>
    <message>
        <location/>
        <source>Loopback Test</source>
        <translation>回环测试</translation>
    </message>
    <message>
        <location/>
        <source>Delay Variance</source>
        <translation>延时方差</translation>
    </message>
    <message>
        <location/>
        <source>Variance in packet latency</source>
        <translation>数据包延时的方差</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the packet latency variance for loopback testing.&lt;/b&gt;&lt;br /&gt;Most audio paths contain some variable latency. This allows you set that variance for loopback mode testing. For example, if you set this to 15ms, this will emulate a network with 20-35ms ping latency or one with 80-95ms latency. Most domestic net connections have a variance of about 5ms</source>
        <oldsource>&lt;b&gt;This sets the packet latency variance for loopback testing&lt;/b&gt;&lt;br /&gt;Most audio paths contain some variable latency. This allows you set that variance for loopback mode testing. For example, if you set this to 15ms, this will emulate a network with 20-35ms ping latency or one with 80-95ms latency. Most domestic net connections have a variance of about 5ms</oldsource>
        <translation>&lt;b&gt;设置回环测试时的数据包延迟方差。&lt;/b&gt;&lt;br /&gt;大多数音频路径含有一定的延迟方差。该选项允许你设置回环测试模式时的方差。例如，如果你设置该值为 15毫秒，将模拟一个 20-35毫秒 ping 延迟或一个 80-95毫秒延迟的网络。大多数国内网络的延迟方差啊大概是5毫秒</translation>
    </message>
    <message>
        <location/>
        <source>Packet Loss</source>
        <translation>数据包丢失</translation>
    </message>
    <message>
        <location/>
        <source>Packet loss for loopback mode</source>
        <translation>回环模式的数据包丢失</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the packet loss for loopback mode.&lt;/b&gt;&lt;br /&gt;This will be the ratio of packets lost. Unless your outgoing bandwidth is peaked or there&apos;s something wrong with your network connection, this will be 0%</source>
        <translation>&lt;b&gt;设置回环模式时的数据包丢失。&lt;/b&gt;&lt;br /&gt;该值即为丢包率。除非你的出外的带宽达到峰值或者你的网络连接有问题，该值会是 0%</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Loopback</source>
        <translation>&amp;回环</translation>
    </message>
    <message>
        <location/>
        <source>Desired loopback mode</source>
        <translation>期望的回环模式</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This enables one of the loopback testmodes.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;None&lt;/i&gt; - Loopback disabled&lt;br /&gt;&lt;i&gt;Local&lt;/i&gt; - Emulate a local server.&lt;br /&gt;&lt;i&gt;Server&lt;/i&gt; - Request loopback from server.&lt;br /&gt;Please note than when loopback is enabled, no other players will hear your voice. This setting is not saved on application exit.</source>
        <translation>&lt;b&gt;该选项启用某种回环测试模式。&lt;/b&gt;&lt;br /&gt;&lt;i&gt;无&lt;/i&gt; - 禁用回环&lt;br /&gt;&lt;i&gt;本地&lt;/i&gt; - 模拟一个本地服务器。&lt;br /&gt;&lt;i&gt;服务器&lt;/i&gt; - 请求来自服务器的回环。&lt;br /&gt;请注意当回环模式启用时，别的玩家不会听到你的声音。当应用程序退出时该选项不会被存盘。</translation>
    </message>
    <message>
        <location/>
        <source>Other Applications</source>
        <translation>其他应用程序</translation>
    </message>
    <message>
        <location/>
        <source>Volume of other applications during speech</source>
        <translation>讲话时别的应用程序的音量</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Decrease volume of other applications during speech.&lt;/b&gt;&lt;br /&gt;Mumble supports decreasing the volume of other applications during incoming speech. This sets the relative volume of other applications when others are talking.</source>
        <translation>&lt;b&gt;讲话时减小其他应用程序的音量.&lt;/b&gt;&lt;br /&gt;Mumble 支持收到语音时减小其他应用程序的音量。这将设置当其他的人交谈时其他应用程序的相对音量。</translation>
    </message>
    <message>
        <location/>
        <source>Output device for audio</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is the output device to use for audio.&lt;/b&gt;</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>AudioOutputDialog</name>
    <message>
        <location filename="AudioConfigDialog.cpp" line="+119"/>
        <source>None</source>
        <translation>无</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Local</source>
        <translation>本地</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Server</source>
        <translation>服务器</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Audio Output</source>
        <translation>音频输出</translation>
    </message>
    <message>
        <location line="+93"/>
        <location line="+19"/>
        <source>%1 ms</source>
        <translation>%1 毫秒</translation>
    </message>
    <message>
        <location line="-8"/>
        <location line="+4"/>
        <location line="+8"/>
        <location line="+31"/>
        <location line="+4"/>
        <source>%1%</source>
        <translation></translation>
    </message>
    <message>
        <location line="-31"/>
        <source>%1ms</source>
        <translation>%1 毫秒</translation>
    </message>
    <message>
        <location line="+15"/>
        <location line="+6"/>
        <source>%1m</source>
        <translation>%1 米</translation>
    </message>
</context>
<context>
    <name>AudioStats</name>
    <message>
        <source>&gt;1000ms</source>
        <translation type="obsolete">&gt;1000毫秒</translation>
    </message>
    <message>
        <location filename="AudioStats.ui"/>
        <source>Audio Statistics</source>
        <translation>音频统计</translation>
    </message>
    <message>
        <location/>
        <source>Input Levels</source>
        <translation>输入级别</translation>
    </message>
    <message>
        <location/>
        <source>Peak microphone level</source>
        <translation>麦克风级别峰值</translation>
    </message>
    <message>
        <location/>
        <source>Peak power in last frame</source>
        <translation>最后帧的峰值功率</translation>
    </message>
    <message>
        <location/>
        <source>This shows the peak power in the last frame (20 ms), and is the same measurement as you would usually find displayed as &quot;input power&quot;. Please disregard this and look at &lt;b&gt;Microphone power&lt;/b&gt; instead, which is much more steady and disregards outliers.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>TextLabel</source>
        <translation type="obsolete">文本标签</translation>
    </message>
    <message>
        <location/>
        <source>Peak speaker level</source>
        <translation>扬声器级别峰值</translation>
    </message>
    <message>
        <location/>
        <source>This shows the peak power of the speakers in the last frame (20 ms). Unless you are using a multi-channel sampling method (such as ASIO) with speaker channels configured, this will be 0. If you have such a setup configured, and this still shows 0 while you&apos;re playing audio from other programs, your setup is not working.</source>
        <oldsource>This shows the peak power in the last frame (20 ms) of the speakers. Unless you are using a multi-channel sampling method (such as ASIO) with speaker channels configured, this will be 0. If you have such a setup configured, and this still shows 0 while you&apos;re playing audio from other programs, your setup is not working.</oldsource>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Peak clean level</source>
        <translation>静输入级别峰值</translation>
    </message>
    <message>
        <location/>
        <source>Signal Analysis</source>
        <translation>信号分析</translation>
    </message>
    <message>
        <location/>
        <source>Microphone power</source>
        <translation>麦克风功率</translation>
    </message>
    <message>
        <location/>
        <source>How close the current input level is to ideal</source>
        <translation>当前输入级别离完美有多近</translation>
    </message>
    <message>
        <location/>
        <source>This shows how close your current input volume is to the ideal. To adjust your microphone level, open whatever program you use to adjust the recording volume, and look at the value here while talking.&lt;br /&gt;&lt;b&gt;Talk loud, as you would when you&apos;re upset over getting fragged by a noob.&lt;/b&gt;&lt;br /&gt;Adjust the volume until this value is close to 100%, but make sure it doesn&apos;t go above. If it does go above, you are likely to get clipping in parts of your speech, which will degrade sound quality.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Signal-To-Noise ratio</source>
        <translation>信噪比</translation>
    </message>
    <message>
        <location/>
        <source>Signal-To-Noise ratio from the microphone</source>
        <translation>麦克风的信噪音比</translation>
    </message>
    <message>
        <location/>
        <source>This is the Signal-To-Noise Ratio (SNR) of the microphone in the last frame (20 ms). It shows how much clearer the voice is compared to the noise.&lt;br /&gt;If this value is below 1.0, there&apos;s more noise than voice in the signal, and so quality is reduced.&lt;br /&gt;There is no upper limit to this value, but don&apos;t expect to see much above 40-50 without a sound studio.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Speech Probability</source>
        <translation>语音的概率</translation>
    </message>
    <message>
        <location/>
        <source>Probability of speech</source>
        <translation>语音的概率</translation>
    </message>
    <message>
        <location/>
        <source>This is the probability that the last frame (20 ms) was speech and not environment noise.&lt;br /&gt;Voice activity transmission depends on this being right. The trick with this is that the middle of a sentence is always detected as speech; the problem is the pauses between words and the start of speech. It&apos;s hard to distinguish a sigh from a word starting with &apos;h&apos;.&lt;br /&gt;If this is in bold font, it means Mumble is currently transmitting (if you&apos;re connected).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Configuration feedback</source>
        <translation>配置反馈</translation>
    </message>
    <message>
        <location/>
        <source>Current audio bitrate</source>
        <translation>当前音频比特率</translation>
    </message>
    <message>
        <location/>
        <source>Bitrate of last frame</source>
        <translation>最后帧的比特率</translation>
    </message>
    <message>
        <location/>
        <source>DoublePush interval</source>
        <translation>两次按键时间间隔</translation>
    </message>
    <message>
        <source>Time between last two PushToTalk presses</source>
        <translation type="obsolete">最后两次按下按键通话键的时间间隔</translation>
    </message>
    <message>
        <location/>
        <source>Speech Detection</source>
        <translation>语音侦测</translation>
    </message>
    <message>
        <location/>
        <source>Current speech detection chance</source>
        <translation>当前语音侦测几率</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This shows the current speech detection settings.&lt;/b&gt;&lt;br /&gt;You can change the settings from the Settings dialog or from the Audio Wizard.</source>
        <translation>&lt;b&gt;显示当前语音侦测设置。&lt;/b&gt;&lt;br /&gt;你能在设置对话框或音频精灵里修改当前设置。</translation>
    </message>
    <message>
        <location/>
        <source>Signal and noise power spectrum</source>
        <translation>信号和噪音功率频谱</translation>
    </message>
    <message>
        <location/>
        <source>Power spectrum of input signal and noise estimate</source>
        <translation>输入信号和噪音估计的功率频谱</translation>
    </message>
    <message>
        <location/>
        <source>This shows the power spectrum of the current input signal (red line) and the current noise estimate (filled blue).&lt;br /&gt;All amplitudes are multiplied by 30 to show the interesting parts (how much more signal than noise is present in each waveband).&lt;br /&gt;This is probably only of interest if you&apos;re trying to fine-tune noise conditions on your microphone. Under good conditions, there should be just a tiny flutter of blue at the bottom. If the blue is more than halfway up on the graph, you have a seriously noisy environment.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Echo Analysis</source>
        <translation>回声分析</translation>
    </message>
    <message>
        <location/>
        <source>Weights of the echo canceller</source>
        <translation>回声抵消器的影响</translation>
    </message>
    <message>
        <location/>
        <source>This shows the weights of the echo canceller, with time increasing downwards and frequency increasing to the right.&lt;br /&gt;Ideally, this should be black, indicating no echo exists at all. More commonly, you&apos;ll have one or more horizontal stripes of bluish color representing time delayed echo. You should be able to see the weights updated in real time.&lt;br /&gt;Please note that as long as you have nothing to echo off, you won&apos;t see much useful data here. Play some music and things should stabilize. &lt;br /&gt;You can choose to view the real or imaginary parts of the frequency-domain weights, or alternately the computed modulus and phase. The most useful of these will likely be modulus, which is the amplitude of the echo, and shows you how much of the outgoing signal is being removed at that time step. The other viewing modes are mostly useful to people who want to tune the echo cancellation algorithms.&lt;br /&gt;Please note: If the entire image fluctuates massively while in modulus mode, the echo canceller fails to find any correlation whatsoever between the two input sources (speakers and microphone). Either you have a very long delay on the echo, or one of the input sources is configured wrong.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This shows the peak power in the last frame (20 ms) after all processing. Ideally, this should be -96 dB when you&apos;re not talking. In reality, a sound studio should see -60 dB, and you should hopefully see somewhere around -20 dB. When you are talking, this should rise to somewhere between -5 and -10 dB.&lt;br /&gt;If you are using echo cancellation, and this rises to more than -15 dB when you&apos;re not talking, your setup is not working, and you&apos;ll annoy other users with echoes.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Time between last two Push-To-Talk presses</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioStats.cpp" line="+405"/>
        <source>&gt;1000 ms</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioStats.ui"/>
        <source>This is the audio bitrate of the last compressed frame (20 ms), and as such will jump up and down as the VBR adjusts the quality. The peak bitrate can be adjusted in the Settings dialog.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>AudioWizard</name>
    <message>
        <location filename="AudioWizard.cpp" line="+65"/>
        <source>Audio Tuning Wizard</source>
        <translation>音频调节精灵</translation>
    </message>
    <message>
        <location line="+50"/>
        <source>Introduction</source>
        <translation>介绍</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Welcome to the Mumble Audio Wizard</source>
        <translation>欢迎使用 Mumble 音频精灵</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>This is the audio tuning wizard for Mumble. This will help you correctly set the input levels of your sound card, and also set the correct parameters for sound processing in Mumble.</source>
        <translation>这是 Mumble 音频调节精灵。这将帮你正确设置你的声卡的输入级别，并正确设置 Mumble 的声音处理参数。</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Please be aware that as long as this wizard is active, audio will be looped locally to allow you to listen to it, and no audio will be sent to the server.</source>
        <translation>请注意当音频精灵活跃时，音频将被在本地环回以允许你听到它，没有音频会被发送到服务器。</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Finished</source>
        <translation>完成</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Enjoy using Mumble</source>
        <translation>Mumble 祝你使用愉快</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Congratulations. You should now be ready to enjoy a richer sound experience with Mumble.</source>
        <translation>恭喜。现在你可以准备好享受 Mumble 带给你的丰富声音体验.</translation>
    </message>
    <message>
        <location line="+23"/>
        <source>Device selection</source>
        <translation>设备选择</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Selecting the input and output device to use with Mumble.</source>
        <translation>选择 Mumble 用来输入和输出的设备.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Input device</source>
        <translation>输入设备</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>This is the device your microphone is connected to.</source>
        <translation>这是你的麦克风连接到的设备。</translation>
    </message>
    <message>
        <location line="+14"/>
        <location line="+52"/>
        <source>System</source>
        <translation>系统</translation>
    </message>
    <message>
        <location line="-49"/>
        <source>Input method for audio</source>
        <translation>音频输入方法</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>&lt;b&gt;This is the input method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation>&lt;b&gt;这是用于音频的输入方法。&lt;/b&gt;&lt;br /&gt;很有可能你想使用 DirectSound。</translation>
    </message>
    <message>
        <location line="+12"/>
        <location line="+52"/>
        <source>Device</source>
        <translation>设备</translation>
    </message>
    <message>
        <location line="-49"/>
        <source>Input device to use</source>
        <translation>使用的输入设备</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>&lt;b&gt;Selects which sound card to use for audio input.&lt;/b&gt;</source>
        <translation>&lt;b&gt;选择哪个声卡用于音频输入。&lt;/b&gt;</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Use echo cancellation</source>
        <translation>使用回声消除</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Cancel echo from headset or speakers.</source>
        <translation>消除来自耳机或扬声器的回声.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This enables echo cancellation of outgoing audio, which helps both on speakers and on headsets.</source>
        <translation>启用送出音频的回声消除，这在扬声器和耳机上都有帮助。</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Output device</source>
        <translation>输出设备</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>This is the device your speakers or headphones are connected to.</source>
        <translation>这是你的扬声器或耳机连接到的设备.</translation>
    </message>
    <message>
        <location line="+19"/>
        <source>Output method for audio</source>
        <translation>音频输出方法</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>&lt;b&gt;This is the Output method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation>&lt;b&gt;这是用于音频输出的方法。&lt;/b&gt;&lt;br /&gt;很有可能你想使用 DirectSound。</translation>
    </message>
    <message>
        <location line="+15"/>
        <source>Output device to use</source>
        <translation>使用的输出设备</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>&lt;b&gt;Selects which sound card to use for audio Output.&lt;/b&gt;</source>
        <translation>&lt;b&gt;选择哪个声卡用于音频输出。&lt;/b&gt;</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Enable positional audio</source>
        <translation>启用定位音频</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Allows positioning of sound.</source>
        <translation>允许声音定位.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This allows Mumble to use positional audio to place voices.</source>
        <translation>这允许 Mumble 使用定位音频来放置语音。</translation>
    </message>
    <message>
        <location line="+17"/>
        <source>Positional Audio</source>
        <translation>定位音频</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Adjusting attenuation of positional audio.</source>
        <translation>调节定位音频的衰减量.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Mumble supports positional audio for some games, and will position the voice of other players relative to their position in game. Depending on their position, the volume of the voice will be changed between the speakers to simulate the direction and distance the other player is at. Such positioning depends on your speaker configuration being correct in your operating system, so a test is done here.</source>
        <translation>Mumble 支持在某些游戏里使用定位音频，并能根据其他玩家在游戏中的位置来定位他们的发言。音量将会根据说话者的位置来改变以模拟他相对于其他玩家的方向和距离。定位音频依赖于你的操作系统正确的配置了扬声器，因此我们对扬声器进行测试。</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>The graph below shows the position of &lt;font color=&quot;red&quot;&gt;you&lt;/font&gt;, the &lt;font color=&quot;yellow&quot;&gt;speakers&lt;/font&gt; and a &lt;font color=&quot;green&quot;&gt;moving sound source&lt;/font&gt; as if seen from above. You should hear the audio move between the channels.</source>
        <translation>下图中显示&lt;font color=&quot;red&quot;&gt;你&lt;/font&gt;的位置，&lt;font color=&quot;yellow&quot;&gt;扬声器&lt;/font&gt;和一个&lt;font color=&quot;green&quot;&gt;移动的声源&lt;/font&gt;。你应该听到音频在声道间切换.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Use headphones</source>
        <translation>使用耳机</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Use headphones instead of speakers.</source>
        <translation>使用耳机而不是扬声器.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This ignores the OS speaker configuration and configures the positioning for headphones instead.</source>
        <translation>这将忽略操作系统的扬声器配置并为耳机配置音频定位.</translation>
    </message>
    <message>
        <location line="+22"/>
        <source>Volume tuning</source>
        <translation>音量调节</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Tuning microphone hardware volume to optimal settings.</source>
        <translation>调节麦克风硬件音量到最佳设置。</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Open your sound control panel and go to the recording settings. Make sure the microphone is selected as active input with maximum recording volume. If there&apos;s an option to enable a &quot;Microphone boost&quot; make sure it&apos;s checked.</source>
        <translation>打开你的声音控制面板然后打开录音设置。确保麦克风被选为当前使用的输入并使用最大录音音量。如果有启用“麦克风加强”选项，请确保它被选上。</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Speak loudly, as when you are annoyed or excited. Decrease the volume in the sound control panel until the bar below stays as high as possible in the blue and green but &lt;b&gt;not&lt;/b&gt; the red zone while you speak.</source>
        <translation>请大声讲话，就像你在愤怒或兴奋时那样。减小你的声音控制面板里的音量直到下面的进度条在你讲话时尽可能的停留在蓝区和绿区，而&lt;b&gt;不是&lt;/b&gt;红区里.</translation>
    </message>
    <message>
        <location line="+11"/>
        <source>Now talk softly, as you would when talking late at night and you don&apos;t want to disturb anyone. Adjust the slider below so that the bar moves into green when you talk, but stays blue while you&apos;re silent.</source>
        <translation>现在轻声讲话，就像你在深夜时不想打扰任何人一样。调节下面的滑块直到下面的进度条在你讲话时进入绿区，而在你沉默时停留在蓝区。</translation>
    </message>
    <message>
        <location line="+19"/>
        <source>Voice Activity Detection</source>
        <translation>语言活动检测</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Letting Mumble figure out when you&apos;re talking and when you&apos;re silent.</source>
        <translation>让 Mumble 分辨出你是在讲话还是在沉默.</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>This will help Mumble figure out when you are talking. The first step is selecting which data value to use.</source>
        <translation>这将帮助 Mumble 知道你是在讲话。第一步是选择用哪个数据值。</translation>
    </message>
    <message>
        <location line="+26"/>
        <source>Raw amplitude from input</source>
        <translation>输入的原始振幅</translation>
    </message>
    <message>
        <location line="-4"/>
        <source>Signal-To-Noise ratio</source>
        <translation>信噪比</translation>
    </message>
    <message>
        <location line="+27"/>
        <source>Next you need to adjust the following two sliders. The first few utterances you say should end up in the green area (definitive speech). While talking, you should stay inside the yellow (might be speech) and when you&apos;re not talking, everything should be in the red (definitively not speech).</source>
        <translation>下一步你需要调节下面两个滑块。你最初讲几句话时进度条应该停留在绿区（确定是讲话）。当你讲话时，进度条应该停留在黄区（可能是讲话）；当你没讲话时，进度条应该停留在红区(明确没在讲话）.</translation>
    </message>
    <message>
        <location line="+24"/>
        <source>Last, you need to adjust the grace time. Mumble may detect this much non-speech and still keep transmitting before it breaks off. This allows you to catch your breath while speaking.</source>
        <translation>最后，你需要调节宽限时间.在宽限时间内 Mumble 能够侦测到你没在讲话并继续传送，然后断开连接。这允许你在讲话时喘息。</translation>
    </message>
    <message>
        <location line="+28"/>
        <source>Device tuning</source>
        <translation>设备调节</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Changing hardware output delays to their minimum value.</source>
        <translation>修改硬件输出延时到它们的最小值。</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>To keep latency to an absolute minium, it&apos;s important to buffer as little audio as possible on the soundcard. However, many soundcards report that they require a much smaller buffer than what they can actually work with, so the only way to set this value is to try and fail.</source>
        <translation>为了保证延时是绝对最小值，让声卡缓冲尽可能少的音频非常重要。然而大多数声卡声称它们需要比实际需求更小的缓冲区，因此设置这个值的唯一方式是去尝试。</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>You should hear a single tone that&apos;s changing in frequency. Change the slider below to the lowest value which gives &lt;b&gt;no&lt;/b&gt; interruptions or jitter in the sound. Please note that local echo is disabled during this test to improve audio path recognition.</source>
        <translation>你应该听到一个不断改变频率的单音。移动下面的滑块直到它是&lt;b&gt;不&lt;/b&gt;导致声音中断或颤音的最小值。请注意在测试中本地回声将被禁止以改善音频路径识别.</translation>
    </message>
    <message>
        <location line="+17"/>
        <source>Amount of data to buffer.</source>
        <translation>缓冲数据大小。</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This sets the amount of data to prebuffer in the output buffer. Experiment with different values and set it to the lowest which doesn&apos;t cause rapid jitter in the sound.</source>
        <translation>设置输出缓冲区里前置缓冲的数据大小。试验不同值然后设置成不引起快速颤音的最低值.</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>If you position the microphone so it can pick up the speakers or headset, Mumble will measure the total audio path delay in your system; this means the delay from a sample is placed in an outbound buffer until it&apos;s found in a matching incoming buffer.</source>
        <translation>如果你将麦克风放到能捕捉扬声器或耳机声音的地方, Mumble 将计算你的系统中的总的音频路径延时;这个延时是自一个声音样本被放到输出缓冲区后直到它在匹配的输入缓冲区里被找到的时间间隔.</translation>
    </message>
    <message>
        <location line="+103"/>
        <source>%1ms</source>
        <translation>%1毫秒</translation>
    </message>
    <message>
        <location line="+191"/>
        <source>Audio path is %1ms long.</source>
        <translation>音频路径长%1毫秒.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Audio path cannot be determined. Input not recognized.</source>
        <translation>音频路径无法被确定。输入无法识别.</translation>
    </message>
    <message>
        <location line="+51"/>
        <source>%1 s</source>
        <translation>%1 秒</translation>
    </message>
    <message>
        <location line="-708"/>
        <source>Mumble is under continuous development, and the development team wants to focus on the features that benefit the most users. To this end, Mumble supports submitting anonymous statistics about your configuration to the developers. These statistcs are essential for future development, and also make sure the features you use aren&apos;t deprecated.</source>
        <translation>Mumble 正处于开发阶段，开发团队致力于开发能使大多数用户受益的功能。Mumble 支持匿名递交你的配置统计信息给开发人员。这些统计信息对于将来的开发非常重要，并确保你使用的功能不会被弃用。</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Submit anonymous statistics to the Mumble project</source>
        <translation>递交匿名统计给 Mumble 项目组</translation>
    </message>
    <message>
        <location line="+224"/>
        <source>Push To Talk:</source>
        <translation>按键通话:</translation>
    </message>
</context>
<context>
    <name>BanEditor</name>
    <message>
        <location filename="BanEditor.ui"/>
        <source>Mumble - Edit Bans</source>
        <translation>Mumble - 编辑黑名单</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Address</source>
        <translation>&amp;地址</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Mask</source>
        <translation>&amp;子网掩码</translation>
    </message>
    <message>
        <location/>
        <source>0.0.0.0</source>
        <translation></translation>
    </message>
    <message>
        <location/>
        <source>&amp;Add</source>
        <translation>&amp;添加</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Update</source>
        <translation>&amp;更新</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Remove</source>
        <translation>&amp;删除</translation>
    </message>
</context>
<context>
    <name>ChanACL</name>
    <message>
        <location filename="ACL.cpp" line="+49"/>
        <source>W</source>
        <translation></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>T</source>
        <translation></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>E</source>
        <translation></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>S</source>
        <translation></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>A</source>
        <translation></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>M</source>
        <translation></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>K</source>
        <translation></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>C</source>
        <translation></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>L</source>
        <translation></translation>
    </message>
    <message>
        <location line="+10"/>
        <source>None</source>
        <translation>无</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Traverse</source>
        <translation>遍历</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Enter</source>
        <translation>进入</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Speak</source>
        <translation>发言</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>AltSpeak</source>
        <translation>试图发言</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Mute/Deafen</source>
        <translation>禁音/耳聋</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Move/Kick</source>
        <translation>移动/踢出</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Make channel</source>
        <translation>创建频道</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Link channel</source>
        <translation>链接频道</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>This represents no privileges.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>This represents total access to the channel, including the ability to change group and ACL information. This privilege implies all other privileges.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>This represents the permission to traverse the channel. If a user is denied this privilege, he will be unable to access this channel and any sub-channels in any way, regardless of other permissions in the sub-channels.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>This represents the permission to join the channel. If you have a hierarchical channel structure, you might want to give everyone Traverse, but restrict Enter in the root of your hierarchy.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>This represents the permission to speak in a channel. Users without this privilege will be suppressed by the server (seen as muted), and will be unable to speak until they are unmuted by someone with the appropriate privileges.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>This represents the permission to speak in a channel with flagged speech. This works exactly like the &lt;i&gt;speak&lt;/i&gt; privilege, but applies to packets spoken with AltPushToTalk held down. This may be used to broadcast to a hierarchy of channels without linking.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>This represents the permission to mute and deafen other players. Once muted, a player will stay muted until he is unmuted by another privileged player or reconnects to the server.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>This represents the permission to move a player to another channel or kick him from the server. To actually move the player, either the moving player must have Move/Kick privileges in the destination channel, or the player must normally be allowed to enter the channel. Players with this privilege can move players into channels the target player normally wouldn&apos;t have permission to enter.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>This represents the permission to make sub-channels. The player making the sub-channel will be added to the admin group of the sub-channel.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>This represents the permission to link channels. Players in linked channels hear each other, as long as the speaking player has the &lt;i&gt;speak&lt;/i&gt; privilege in the channel of the listener. You need the link privilege in both channels to create a link, but just in either channel to remove it.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-57"/>
        <source>Write ACL</source>
        <translation>创建访问控制列表</translation>
    </message>
</context>
<context>
    <name>ClientPlayer</name>
    <message>
        <location filename="Player.cpp" line="+53"/>
        <source>Authenticated</source>
        <translation>已认证</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Muted (server)</source>
        <translation>静音(服务器)</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Deafened (server)</source>
        <translation>耳聋(服务器)</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Local Mute</source>
        <translation>本地静音</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Muted (self)</source>
        <translation>静音(自己）</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Deafened (self)</source>
        <translation>耳聋(自己)</translation>
    </message>
</context>
<context>
    <name>ConfigDialog</name>
    <message>
        <location filename="ConfigDialog.cpp" line="+54"/>
        <source>Accept changes</source>
        <translation>确定</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This button will accept current settings and return to the application.&lt;br /&gt;The settings will be stored to disk when you leave the application.</source>
        <translation>该按钮将接受当前设置并返回到应用程序。&lt;br/&gt;当你退出应用程序时这些设置将被存盘。</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Reject changes</source>
        <translation>取消</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This button will reject all changes and return to the application.&lt;br /&gt;The settings will be reset to the previous positions.</source>
        <translation>该按钮将拒绝所有修改并返回到应用程序。&lt;br/&gt;这些设置将被重置到先前的设定。</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Apply changes</source>
        <translation>应用</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This button will immediately apply all changes.</source>
        <translation>该按钮将立即应用所有修改.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Undo changes for current page</source>
        <translation>撤销</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This button will revert any changes done on the current page to the most recent applied settings.</source>
        <translation>该按钮将复原对当前页的任意修改到最近使用的设置。</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Restore defaults for current page</source>
        <translation>恢复默认值</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This button will restore the defaults for the settings on the current page. Other pages will not be changed.&lt;br /&gt;To restore all settings to their defaults, you will have to use this button on every page.</source>
        <oldsource>This button will restore the settings for the current page only to their defaults. Other pages will not be changed.&lt;br /&gt;To restore all settings to their defaults, you will have to use this button on every page.</oldsource>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This button will restore the settings for the current page only to their defaults. Other pages will be not be changed.&lt;br /&gt;To restore all settings to their defaults, you will have to use this button on every page.</source>
        <translation type="obsolete">该按钮将恢复当前页设置到默认值。其他页面的设置将不被修改。&lt;b r/&gt;为了恢复其他页面上的设置到默认值，你需要在每个页面上都使用该按钮。</translation>
    </message>
    <message>
        <location filename="ConfigDialog.ui"/>
        <source>Mumble Configuration</source>
        <translation>Mumble 配置</translation>
    </message>
    <message>
        <source>Tab 1</source>
        <translation type="obsolete">标签 1</translation>
    </message>
    <message>
        <location/>
        <source>Advanced</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ConnectDialog</name>
    <message>
        <location filename="ConnectDialog.cpp" line="+124"/>
        <source>Connecting to %1</source>
        <translation>正在连接到 %1</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Enter username</source>
        <translation>输入用户名</translation>
    </message>
    <message>
        <location line="+85"/>
        <source>Unknown</source>
        <translation>未知</translation>
    </message>
    <message>
        <location line="+14"/>
        <source>Failed to fetch server list</source>
        <translation>无法获取服务器列表</translation>
    </message>
    <message>
        <location line="+42"/>
        <location line="+22"/>
        <source>New</source>
        <translation>新建</translation>
    </message>
    <message>
        <location line="+38"/>
        <source>-Unnamed entry-</source>
        <translation>-未命名条目-</translation>
    </message>
    <message>
        <location line="+24"/>
        <location line="+27"/>
        <source>Add</source>
        <translation>添加</translation>
    </message>
    <message>
        <location line="-2"/>
        <source>Update</source>
        <translation>更新</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui"/>
        <source>Mumble Server Connect</source>
        <translation>Mumble 服务器连接</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Custom Servers</source>
        <translation>&amp;自定义服务器</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Label</source>
        <translation>&amp;标签</translation>
    </message>
    <message>
        <location/>
        <source>A&amp;ddress</source>
        <translation>&amp;地址</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Port</source>
        <translation>&amp;端口</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Username</source>
        <translation>&amp;用户名</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Password</source>
        <translation>&amp;密码</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Connect</source>
        <translation>&amp;连接</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Cancel</source>
        <translation>&amp;取消</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Add</source>
        <translation>&amp;添加</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Remove</source>
        <translation>&amp;删除</translation>
    </message>
    <message>
        <location/>
        <source>Server &amp;Browser</source>
        <translation>服务器&amp;浏览器</translation>
    </message>
    <message>
        <location/>
        <source>Label</source>
        <translation>标签</translation>
    </message>
    <message>
        <location/>
        <source>Address</source>
        <translation>地址</translation>
    </message>
    <message>
        <location/>
        <source>URL</source>
        <translation>网址</translation>
    </message>
    <message>
        <location/>
        <source>Cancel</source>
        <translation>取消</translation>
    </message>
    <message>
        <location/>
        <source>C&amp;opy to custom</source>
        <translation>&amp;拷贝到自定义</translation>
    </message>
    <message>
        <location/>
        <source>&amp;View Webpage</source>
        <translation>&amp;查看主页</translation>
    </message>
</context>
<context>
    <name>CoreAudioSystem</name>
    <message>
        <location filename="CoreAudio.cpp" line="+83"/>
        <source>Default Device</source>
        <translation type="unfinished">默认设备</translation>
    </message>
</context>
<context>
    <name>CrashReporter</name>
    <message>
        <location filename="CrashReporter.cpp" line="+37"/>
        <source>Mumble Crash Report</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>&lt;p&gt;&lt;b&gt;We&apos;re terribly sorry, but it seems Mumble has crashed. Do you want to send a crash report to the Mumble developers?&lt;/b&gt;&lt;/p&gt;&lt;p&gt;The crash report contains a partial copy of Mumble&apos;s memory at the time it crashed, and will help the developers fix the problem.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Email address (optional)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Please describe briefly, in English, what you were doing at the time of the crash</source>
        <oldsource>Please briefly describe what you were doing at the time of the crash</oldsource>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Send Report</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Don&apos;t send report</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+24"/>
        <source>Crash upload successful</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Thank you for helping make Mumble better!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <location line="+2"/>
        <source>Crash upload failed</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-2"/>
        <source>We&apos;re really sorry, but it appears the crash upload has failed with error %1 %2. Please inform a developer.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>This really isn&apos;t funny, but apparently there&apos;s a bug in the crash reporting code, and we&apos;ve failed to upload the report. You may inform a developer about error %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+114"/>
        <source>Uploading crash report</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Abort upload</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>DXAudioInput</name>
    <message>
        <location filename="DirectSound.cpp" line="+172"/>
        <source>Default DirectSound Voice Input</source>
        <translation>默认 DirectSound 语音输入</translation>
    </message>
    <message>
        <location line="+461"/>
        <source>Opening chosen DirectSound Input device failed. No microphone capture will be done.</source>
        <translation>不能打开所选 DirectSound 输入设备。不能捕捉麦克风。</translation>
    </message>
    <message>
        <location line="-79"/>
        <source>Opening chosen DirectSound Input failed. Default device will be used.</source>
        <translation>不能打开所选 DirectSound 输入。将使用默认设备。</translation>
    </message>
    <message>
        <location line="+74"/>
        <source>Lost DirectSound input device.</source>
        <translation>丢失 DirectSound 输入设备。</translation>
    </message>
</context>
<context>
    <name>DXAudioOutput</name>
    <message>
        <location line="-496"/>
        <source>Default DirectSound Voice Output</source>
        <translation>默认 DirectSound 语音输出</translation>
    </message>
    <message>
        <location line="+247"/>
        <source>Opening chosen DirectSound Output failed. Default device will be used.</source>
        <translation>不能打开所选 DirectSound 输出。将使用默认设备。</translation>
    </message>
    <message>
        <location line="+78"/>
        <source>Lost DirectSound output device.</source>
        <translation>丢失 DirectSound 输出设备。</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Opening chosen DirectSound Output failed. No audio will be heard.</source>
        <translation>不能打开所选 DirectSound 输出。将听不到音频。</translation>
    </message>
</context>
<context>
    <name>Database</name>
    <message>
        <location filename="Database.cpp" line="+127"/>
        <source>Mumble failed to initialize a database in any
of the possible locations.</source>
        <translation>Mumble 无法在任何可能的位置里初始化数据库.</translation>
    </message>
</context>
<context>
    <name>GlobalShortcut</name>
    <message>
        <location filename="GlobalShortcut.ui"/>
        <source>Shortcuts</source>
        <translation>快捷键</translation>
    </message>
    <message>
        <location/>
        <source>List of configured shortcuts</source>
        <translation>配置好的快捷键列表</translation>
    </message>
    <message>
        <location/>
        <source>Function</source>
        <translation>功能</translation>
    </message>
    <message>
        <location/>
        <source>Shortcut</source>
        <translation>快捷键</translation>
    </message>
    <message>
        <location/>
        <source>Suppress</source>
        <translation>禁止</translation>
    </message>
    <message>
        <location/>
        <source>Add new shortcut</source>
        <translation>添加新快捷键</translation>
    </message>
    <message>
        <location/>
        <source>This will add a new global shortcut</source>
        <translation>这将添加一个新的全局快捷键</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Add</source>
        <translation>&amp;添加</translation>
    </message>
    <message>
        <location/>
        <source>Remove selected shortcut</source>
        <translation>移除所选快捷键</translation>
    </message>
    <message>
        <location/>
        <source>This will permanently remove a selected shortcut.</source>
        <translation>这将永久性的删除所选快捷键。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Remove</source>
        <translation>&amp;删除</translation>
    </message>
</context>
<context>
    <name>GlobalShortcutConfig</name>
    <message>
        <location filename="GlobalShortcut.cpp" line="+228"/>
        <source>Shortcuts</source>
        <translation>快捷键</translation>
    </message>
    <message>
        <location line="+33"/>
        <source>Shortcut button combination.</source>
        <translation>快捷键按钮组合.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>&lt;b&gt;This is the global shortcut key combination.&lt;/b&gt;&lt;br /&gt;Double-click this field and then the desired key/button combo to rebind.</source>
        <translation>&lt;b&gt;这是全局快捷键组合。&lt;/b&gt;&lt;br /&gt;双击该区域然后绑定想要的按键/按钮组合。</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Suppress keys from other applications</source>
        <translation>禁止其他应用程序使用的按键</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>&lt;b&gt;This hides the button presses from other applications.&lt;/b&gt;&lt;br /&gt;Enabling this will hide the button (or the last button of a multi-button combo) from other applications. Note that not all buttons can be suppressed.</source>
        <translation>&lt;b&gt;向其他应用程序隐藏这些按键。&lt;/b&gt;&lt;br/&gt;允许该选项将向其他应用程序隐藏指定按键（或者组合按键的最后一个按键）。注意并不是所有按键都能被禁用。</translation>
    </message>
</context>
<context>
    <name>GlobalShortcutMacInit</name>
    <message>
        <location filename="GlobalShortcut_macx.cpp" line="+63"/>
        <source>Mumble has detected that it is unable to receive Global Shortcut events when it is in the background.&lt;br /&gt;&lt;br /&gt;This is because the Universal Access feature called &apos;Enable access for assistive devices&apos; is currently disabled.&lt;br /&gt;&lt;br /&gt;Please &lt;a href=&quot; &quot;&gt;enable this setting&lt;/a&gt; and continue when done.</source>
        <translation>Mumble 检测到当它位于后台时它不能接收到全局快捷键事件。&lt;br /&gt;&lt;br /&gt;这是因为被称为‘允许辅助设备的使用’的辅助使用功能当前被禁用。&lt;br /&gt;&lt;br /&gt;请&lt;a href=&quot; &quot;&gt;启用该选项&lt;/a&gt;然后继续。</translation>
    </message>
</context>
<context>
    <name>GlobalShortcutX</name>
    <message>
        <location filename="GlobalShortcut_unix.cpp" line="+325"/>
        <source>Mouse %1</source>
        <translation>鼠标 %1</translation>
    </message>
</context>
<context>
    <name>LCD</name>
    <message>
        <location filename="LCD.cpp" line="+274"/>
        <source>Not connected</source>
        <translation>未连接</translation>
    </message>
</context>
<context>
    <name>LCDConfig</name>
    <message>
        <location line="-161"/>
        <source>Graphic</source>
        <translation>图形</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Character</source>
        <translation>字符</translation>
    </message>
    <message>
        <location line="+11"/>
        <source>Enable this device</source>
        <translation>启用该设备</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>LCD</source>
        <translation></translation>
    </message>
    <message>
        <location filename="LCD.ui"/>
        <source>Form</source>
        <translation>表单</translation>
    </message>
    <message>
        <location/>
        <source>&lt;!DOCTYPE HTML PUBLIC &quot;-//W3C//DTD HTML 4.0//EN&quot; &quot;http://www.w3.org/TR/REC-html40/strict.dtd&quot;&gt;
&lt;html&gt;&lt;head&gt;&lt;meta name=&quot;qrichtext&quot; content=&quot;1&quot; /&gt;&lt;style type=&quot;text/css&quot;&gt;
p, li { white-space: pre-wrap; }
&lt;/style&gt;&lt;/head&gt;&lt;body style=&quot; font-family:&apos;MS Shell Dlg 2&apos;; font-size:8.25pt; font-weight:400; font-style:normal;&quot;&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;This is the list of available LCD devices on your system.  It lists devices by name, but also includes the size of the display and the LCD type. Mumble supports outputting to several LCD devices at a time.&lt;/p&gt;
&lt;p style=&quot;-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;&lt;span style=&quot; font-weight:600; font-style:italic;&quot;&gt;Type:&lt;/span&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;Mumble supports two different types of LCD devices. A Graphic LCD allows Mumble to draw each individual pixel of the display, whereas a Character LCD only allows Mumble to show characters. Some features are only available to Graphic LCDs.&lt;/p&gt;
&lt;p style=&quot;-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;&lt;span style=&quot; font-weight:600; font-style:italic;&quot;&gt;Size:&lt;/span&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;This field describes the size of an LCD device. The size is given either in pixels (for Graphic LCDs) or in characters (for Character LCDs).&lt;/p&gt;
&lt;p style=&quot;-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;&lt;span style=&quot; font-weight:600; font-style:italic;&quot;&gt;Enabled:&lt;/span&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt; font-weight:600; font-style:italic;&quot;&gt;&lt;span style=&quot; font-weight:400; font-style:normal;&quot;&gt;This decides whether Mumble should draw to a particular LCD device.&lt;/span&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <oldsource>&lt;!DOCTYPE HTML PUBLIC &quot;-//W3C//DTD HTML 4.0//EN&quot; &quot;http://www.w3.org/TR/REC-html40/strict.dtd&quot;&gt;
&lt;html&gt;&lt;head&gt;&lt;meta name=&quot;qrichtext&quot; content=&quot;1&quot; /&gt;&lt;style type=&quot;text/css&quot;&gt;
p, li { white-space: pre-wrap; }
&lt;/style&gt;&lt;/head&gt;&lt;body style=&quot; font-family:&apos;MS Shell Dlg 2&apos;; font-size:8.25pt; font-weight:400; font-style:normal;&quot;&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;This is the list of available LCD devices on your system.  It lists devices by name, but also includes the size of the display and the LCD type. Mumble supports outputting to several LCD devices at a time.&lt;/p&gt;
&lt;p style=&quot;-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;&lt;span style=&quot; font-weight:600; font-style:italic;&quot;&gt;Type:&lt;/span&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;Mumble supports two different types of LCD devices. A Graphic LCD allows Mumble to draw each individual pixel of the display, whereas a Character LCD only allows Mumble to show characters. Some features are only available to Graphic LCDs.&lt;/p&gt;
&lt;p style=&quot;-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;&lt;span style=&quot; font-weight:600; font-style:italic;&quot;&gt;Size:&lt;/span&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;This field describes the size of an LCD device. The size is given in either in pixels (for Graphic LCDs) or in characters (for Character LCDs).&lt;/p&gt;
&lt;p style=&quot;-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;&lt;span style=&quot; font-weight:600; font-style:italic;&quot;&gt;Enabled:&lt;/span&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt; font-weight:600; font-style:italic;&quot;&gt;&lt;span style=&quot; font-weight:400; font-style:normal;&quot;&gt;This decides whether Mumble should draw to a particular LCD device.&lt;/span&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</oldsource>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Devices</source>
        <translation>设备</translation>
    </message>
    <message>
        <location/>
        <source>Name</source>
        <translation>名称</translation>
    </message>
    <message>
        <location/>
        <source>Type</source>
        <translation>类型</translation>
    </message>
    <message>
        <location/>
        <source>Size</source>
        <translation>大小</translation>
    </message>
    <message>
        <location/>
        <source>Enabled</source>
        <translation>已启用</translation>
    </message>
    <message>
        <location/>
        <source>Views</source>
        <translation>视图</translation>
    </message>
    <message>
        <location/>
        <source>Minimum Column Width</source>
        <translation>最小列宽</translation>
    </message>
    <message>
        <location/>
        <source>&lt;!DOCTYPE HTML PUBLIC &quot;-//W3C//DTD HTML 4.0//EN&quot; &quot;http://www.w3.org/TR/REC-html40/strict.dtd&quot;&gt;
&lt;html&gt;&lt;head&gt;&lt;meta name=&quot;qrichtext&quot; content=&quot;1&quot; /&gt;&lt;style type=&quot;text/css&quot;&gt;
p, li { white-space: pre-wrap; }
&lt;/style&gt;&lt;/head&gt;&lt;body style=&quot; font-family:&apos;MS Shell Dlg 2&apos;; font-size:8.25pt; font-weight:400; font-style:normal;&quot;&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;&lt;span style=&quot; font-size:8pt;&quot;&gt;This option decides the minimum width a column in the Player View.&lt;/span&gt;&lt;/p&gt;
&lt;p style=&quot;-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;If too many people are speaking at once, the Player View will split itself into columns.&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;You can use this option to pick a compromise between number of players shown on the&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;LCD, and width of player names.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>TextLabel</source>
        <translation>文本标签</translation>
    </message>
    <message>
        <location/>
        <source>Splitter Width</source>
        <translation>分割线宽度</translation>
    </message>
    <message>
        <location/>
        <source>&lt;!DOCTYPE HTML PUBLIC &quot;-//W3C//DTD HTML 4.0//EN&quot; &quot;http://www.w3.org/TR/REC-html40/strict.dtd&quot;&gt;
&lt;html&gt;&lt;head&gt;&lt;meta name=&quot;qrichtext&quot; content=&quot;1&quot; /&gt;&lt;style type=&quot;text/css&quot;&gt;
p, li { white-space: pre-wrap; }
&lt;/style&gt;&lt;/head&gt;&lt;body style=&quot; font-family:&apos;MS Shell Dlg 2&apos;; font-size:8.25pt; font-weight:400; font-style:normal;&quot;&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;&lt;span style=&quot; font-size:8pt;&quot;&gt;This setting decides the width of column splitter.&lt;/span&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Log</name>
    <message>
        <location filename="Log.cpp" line="+220"/>
        <source>Debug</source>
        <translation type="unfinished">调试</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Critical</source>
        <translation type="unfinished">紧急</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Warning</source>
        <translation type="unfinished">警告</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Information</source>
        <translation type="unfinished">信息</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Server Connected</source>
        <translation type="unfinished">服务器已连接</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Server Disconnected</source>
        <translation type="unfinished">服务器已断开连接</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Player Joined Server</source>
        <translation type="unfinished">玩家加入服务器</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Player Left Server</source>
        <translation type="unfinished">玩家离开服务器</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Player kicked (you or by you)</source>
        <translation type="unfinished">玩家踢出你（或被你踢出）</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Player kicked</source>
        <translation type="unfinished">玩家被踢出</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>You self-muted/deafened</source>
        <translation type="unfinished">你自我静音/耳聋</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Other self-muted/deafened</source>
        <translation type="unfinished">其他人自我静音/耳聋</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Player muted (you)</source>
        <translation type="unfinished">玩家对你静音</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Player muted (by you)</source>
        <translation type="unfinished">玩家被你静音</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Player muted (other)</source>
        <translation type="unfinished">玩家静音其他人</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Player Joined Channel</source>
        <translation type="unfinished">玩家加入频道</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Player Left Channel</source>
        <translation type="unfinished">玩家离开频道</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Permission Denied</source>
        <translation type="unfinished">没有权限</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Text Message</source>
        <translation type="unfinished">文本消息</translation>
    </message>
    <message>
        <source>Script Errors</source>
        <translation type="obsolete">脚本错误</translation>
    </message>
</context>
<context>
    <name>LogConfig</name>
    <message>
        <location line="-164"/>
        <source>Toggle console for %1 events</source>
        <translation>为 %1 事件启用控制台</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Toggle pop-up notifications for %1 events</source>
        <translation>为 %1 事件启用弹出窗口通知</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Toggle Text-To-Speech for %1 events</source>
        <translation>为%1事件启用文本语音转换</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Click here to toggle sound notification for %1 events</source>
        <translation>点击这里为 %1 事件启用声音通知</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Path to soundfile used for sound notifications in the case of %1 events&lt;br /&gt;Single click to play&lt;br /&gt;Doubleclick to change</source>
        <translation>在 %1 事件发生时用于声音通知的声音文件的路径&lt;br /&gt;单击播放&lt;br/&gt;双击更改</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Click here to toggle console output for %1 events.&lt;br /&gt;If checked, this option makes Mumble output all %1 events in its message log.</source>
        <translation>点击这里为 %1 事件启用控制台输出。&lt;br /&gt;如果选中，该选项使 Mumble 输出所有 %1 事件到它的消息日志中。</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Click here to toggle pop-up notifications for %1 events.&lt;br /&gt;If checked, a notification pop-up will be created by Mumble for every %1 event.</source>
        <oldsource>Click here to toggle pop-up notifications for %1 events.&lt;br /&gt;If checked, a notification pop-up will be created by mumble for every %1 event.</oldsource>
        <translation type="unfinished">点击这里为 %1 事件启用弹出窗口通知。&lt;br /&gt;如果选中， Mumble 将为每个 %1 事件创建一个弹出窗口。</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Click here to toggle Text-To-Speech for %1 events.&lt;br /&gt;If checked, Mumble uses Text-To-Speech to read %1 events out loud to you. Text-To-Speech is also able to read the contents of the event which is not true for soundfiles. Text-To-Speech and soundfiles cannot be used at the same time.</source>
        <oldsource>Click here to toggle Text-To-Speech for %1 events.&lt;br /&gt;If checked, mumble uses Text-To-Speech to read %1 events out loud to you. Text-To-Speech is also able to read the contents of the event which is not true for soundfiles. Text-To-Speech and soundfiles cannot be used at the same time.</oldsource>
        <translation type="unfinished">点击这里为 %1 事件启用文本语音转换.&lt;br /&gt;如果选中， Mumble 会使用文本语音转化将 %1 事件大声的读出来给你听。当声音文件无效时，文本语音转换也能读取事件内容。文本语音转化和声音文件不能同时使用。</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Click here to toggle sound notification for %1 events.&lt;br /&gt;If checked, Mumble uses a soundfile predefined by you to indicate %1 events. Soundfiles and Text-To-Speech cannot be used at the same time.</source>
        <oldsource>Click here to toggle sound notification for %1 events.&lt;br /&gt;If checked, mumble uses a soundfile predefined by you to indicate %1 events. Soundfiles and Text-To-Speech cannot be used at the same time.</oldsource>
        <translation type="unfinished">点击这里为 %1 事件启用声音通知。&lt;br /&gt;如果选中， Mumble 会使用一个你预先定义的声音文件来标示 %1 事件。声音文件和文本语音转换不能同时被使用。</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Path to soundfile used for sound notifications in the case of %1 events.&lt;br /&gt;Single click to play&lt;br /&gt;Doubleclick to change&lt;br /&gt;Be sure that sound notifications for these events are enabled or this field will not have any effect.</source>
        <translation>在 %1 事件发生时用于声音通知的声音文件的路径。&lt;br /&gt;单击播放&lt;br /&gt;双击更换&lt;br /&gt;请确认这些事件的声音通知被启用，否则这个字段将没有任何效果。</translation>
    </message>
    <message>
        <location line="+93"/>
        <source>Choose sound file</source>
        <translation>选择声音文件</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Invalid sound file</source>
        <translation>无效声音文件</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The file &apos;%1&apos; does not exist or is not a valid speex file.</source>
        <translation>文件 ‘%1’ 不存在或者不是一个有效的语音编码文件。</translation>
    </message>
    <message>
        <location filename="Log.ui"/>
        <source>Messages</source>
        <translation>消息</translation>
    </message>
    <message>
        <location/>
        <source>Message</source>
        <translation>消息</translation>
    </message>
    <message>
        <location/>
        <source>Console</source>
        <translation>控制台</translation>
    </message>
    <message>
        <location/>
        <source>Notification</source>
        <translation>通知</translation>
    </message>
    <message>
        <location/>
        <source>Text-To-Speech</source>
        <translation>文本语音转换</translation>
    </message>
    <message>
        <location/>
        <source>Soundfile</source>
        <translation>声音文件</translation>
    </message>
    <message>
        <location/>
        <source>Path</source>
        <translation>路径</translation>
    </message>
    <message>
        <location/>
        <source>Text To Speech</source>
        <translation>文本语音转换</translation>
    </message>
    <message>
        <location/>
        <source>Volume</source>
        <translation>音量</translation>
    </message>
    <message>
        <location/>
        <source>Volume of Text-To-Speech Engine</source>
        <translation>文本语音转换引擎的音量</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is the volume used for the speech synthesis.&lt;/b&gt;</source>
        <translation>&lt;b&gt;这是用于语音合成的音量。&lt;/b&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Length threshold</source>
        <translation>长度阈值</translation>
    </message>
    <message>
        <location/>
        <source>Message length threshold for Text-To-Speech Engine</source>
        <translation>用于文本语音转换的消息长度阈值</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is the length threshold used for the Text-To-Speech Engine.&lt;/b&gt;&lt;br /&gt;Messages longer than this limit will not be read aloud in their full length.</source>
        <translation>&lt;b&gt;这是用于文本语音转换引擎的消息长度阈值。&lt;/b&gt;&lt;br /&gt;消息长于此值时将不会被全部读出来。</translation>
    </message>
    <message>
        <location/>
        <source> Characters</source>
        <translation>字符</translation>
    </message>
</context>
<context>
    <name>LookConfig</name>
    <message>
        <location filename="LookConfig.cpp" line="+46"/>
        <location line="+11"/>
        <source>System default</source>
        <translation>系统默认</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>None</source>
        <translation>无</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Only with players</source>
        <translation>仅当有玩家时</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>All</source>
        <translation>所有</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Ask</source>
        <translation>询问</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Do Nothing</source>
        <translation>无动作</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Move</source>
        <translation>移动</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>User Interface</source>
        <translation>用户界面</translation>
    </message>
    <message>
        <location line="+85"/>
        <source>Choose skin file</source>
        <translation>选择皮肤文件</translation>
    </message>
    <message>
        <location filename="LookConfig.ui"/>
        <source>Form</source>
        <translation>表单</translation>
    </message>
    <message>
        <location/>
        <source>Language</source>
        <translation>语言</translation>
    </message>
    <message>
        <location/>
        <source>Language to use (requires restart)</source>
        <translation>使用的语言（需要重启）</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets which language Mumble should use.&lt;/b&gt;&lt;br /&gt;You have to restart Mumble to use the new language.</source>
        <translation>&lt;b&gt;设置 Mumble 使用哪种语言。&lt;/b&gt;&lt;br /&gt;你需要重启 Mumble 来使用新语言。</translation>
    </message>
    <message>
        <location/>
        <source>Look and Feel</source>
        <translation>外观</translation>
    </message>
    <message>
        <location/>
        <source>Style</source>
        <translation>风格</translation>
    </message>
    <message>
        <location/>
        <source>Basic widget style</source>
        <translation>基本窗口风格</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the basic look and feel to use.&lt;/b&gt;</source>
        <translation>&lt;b&gt;设置使用基本外观。&lt;/b&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Skin</source>
        <translation>皮肤</translation>
    </message>
    <message>
        <location/>
        <source>Skin file to use</source>
        <translation>使用的皮肤文件</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets which skin Mumble should use.&lt;/b&gt;&lt;br /&gt;The skin is a style file applied on top of the basic widget style. If there are icons in the same directory as the style sheet, those will replace the default icons.</source>
        <translation>&lt;b&gt;设置 Mumble 应该使用哪个皮肤文件。&lt;/b&gt;&lt;br/&gt;皮肤是一个应用在基本窗口风格上的风格文件。如果在与风格相同的目录里有图标，目录中的图标将替换默认图标。</translation>
    </message>
    <message>
        <location/>
        <source>...</source>
        <translation></translation>
    </message>
    <message>
        <location/>
        <source>Expand</source>
        <translation>展开</translation>
    </message>
    <message>
        <location/>
        <source>When to automatically expand channels</source>
        <translation>什么时候自动展开频道</translation>
    </message>
    <message>
        <location/>
        <source>This sets which channels to automatically expand. &lt;i&gt;None&lt;/i&gt; and &lt;i&gt;All&lt;/i&gt; will expand no or all channels, while &lt;i&gt;Only with players&lt;/i&gt; will expand and collapse channels as players join and leave them.</source>
        <translation>设置哪些频道将自动展开。&lt;i&gt;无&lt;/i&gt;和&lt;i&gt;所有&lt;/i&gt;将展开零个或所有频道，而&lt;i&gt;仅当有玩家时&lt;i&gt;将在玩家加入和离开时自动展开和折叠频道。</translation>
    </message>
    <message>
        <location/>
        <source>List players above subchannels (requires restart).</source>
        <translation>在子频道之上列出玩家（需要重启）。</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;If set, players will be shown above subchannels in the channel view.&lt;/b&gt;&lt;br /&gt;A restart of Mumble is required to see the change.</source>
        <translation>&lt;b&gt;如果选中，玩家将被显示在子频道之上.&lt;/b&gt;&lt;br/&gt;需要重启 Mumble 来使改变生效。</translation>
    </message>
    <message>
        <location/>
        <source>Players above Channels</source>
        <translation>玩家位于频道之上</translation>
    </message>
    <message>
        <location/>
        <source>Channel Dragging</source>
        <translation>频道拖动</translation>
    </message>
    <message>
        <location/>
        <source>This changes the behaviour when moving channels.</source>
        <translation>改变当移动频道时的行为。</translation>
    </message>
    <message>
        <location/>
        <source>This sets the behaviour of channel drags, it can be used to prevent accidental dragging. &lt;i&gt;Move Channel&lt;/i&gt; moves the channel without prompting. &lt;i&gt;Do Nothing&lt;/i&gt; does nothing and prints an error message. &lt;i&gt;Ask&lt;/i&gt; uses a message box to confirm if you really wanted to move the channel.</source>
        <translation>设置频道拖动时的行为，能被用来防止意外的拖动。&lt;i&gt;移动频道&lt;/i&gt;将在移动频道时不做提示.&lt;i&gt;无动作&lt;/i&gt;将在试图移动频道时显示一个出错信息。&lt;i&gt;询问&lt;/i&gt;使用一个消息框来确认你是真的想移动一个频道。</translation>
    </message>
    <message>
        <location/>
        <source>Ask whether to close or minimize when quitting Mumble.</source>
        <translation>询问退出 Mumble 时是关闭还是最小化。</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;If set, will verify you want to quit if connected.&lt;b&gt;</source>
        <translation>&lt;b&gt;如果选中，当你连接到服务器时将验证你是否想退出.&lt;/b&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Ask on quit while connected</source>
        <translation>当你连接到服务器时询问是否退出</translation>
    </message>
    <message>
        <location/>
        <source>Make the Mumble window appear on top of other windows.</source>
        <translation>使 Mumble 窗口位于其他窗口之上.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This makes the Mumble window a topmost window.&lt;/b&gt;</source>
        <translation>&lt;b&gt;这将使 Mumble 窗口成为最上面的窗口。&lt;/b&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Always On Top</source>
        <translation>总在最上</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;If set, minimizing the Mumble main window will cause it to be hidden and accessible only from the tray. Otherwise, it will be minimized as a window normally would.&lt;/b&gt;</source>
        <translation>&lt;b&gt;如果选中，最小化 Mumble 主窗口时将使得它被隐藏到系统托盘。否则，它将会像通常一样被最小化。&lt;/b&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Hide in tray when minimized</source>
        <translation>最小化时隐藏到系统托盘</translation>
    </message>
    <message>
        <source>Show all configuration items</source>
        <translation type="obsolete">显示所有配置选项</translation>
    </message>
    <message>
        <source>&lt;b&gt;This will show all configuration items.&lt;/b&gt;&lt;br /&gt;Mumble contains a lot of configuration items that most users won&apos;t need to change. Checking this will show all configurable items.</source>
        <translation type="obsolete">&lt;b&gt;显示所有配置选项.&lt;/b&gt;&lt;br/&gt; Mumble 包含许多大多数用户不需要改变的选项。选中该选项将显示所有可配置选项。</translation>
    </message>
    <message>
        <source>Expert Config</source>
        <translation type="obsolete">专家配置选项</translation>
    </message>
    <message>
        <location/>
        <source>Hide the main Mumble window in the tray when it is minimized.</source>
        <translation>当最小化时，隐藏 Mumble 主窗口到系统托盘。</translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <location filename="MainWindow.cpp" line="+127"/>
        <location line="+1196"/>
        <source>Root</source>
        <translation>根</translation>
    </message>
    <message>
        <location line="-1160"/>
        <source>Push-to-Talk</source>
        <comment>Global Shortcut</comment>
        <translation>按键通话</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Push and hold this button to send voice.</source>
        <comment>Global Shortcut</comment>
        <translation>按住该按钮来发送语音。</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This configures the push-to-talk button, and as long as you hold this button down, you will transmit voice.</source>
        <comment>Global Shortcut</comment>
        <translation>配置按键发音按键，只要你按住该按键，语音将被传送。</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Reset Audio Processor</source>
        <comment>Global Shortcut</comment>
        <translation>重置音频处理器</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Toggle Mute Self</source>
        <comment>Global Shortcut</comment>
        <translation>切换自我静音</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Toggle self-mute status.</source>
        <comment>Global Shortcut</comment>
        <translation>切换自我静音状态。</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This will toggle your muted status. If you toggle this off, you will also disable self-deafen.</source>
        <comment>Global Shortcut</comment>
        <translation>该选项将切换自我静音状态。如果你关闭静音状态，将同时关闭自我耳聋状态。</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Toggle Deafen Self</source>
        <comment>Global Shortcut</comment>
        <translation>切换自我耳聋</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Toggle self-deafen status.</source>
        <comment>Global Shortcut</comment>
        <translation>切换自我耳聋状态。</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This will toggle your deafen status. If you toggle this on, you will also enable self-mute.</source>
        <comment>Global Shortcut</comment>
        <translation>该选项切换自我耳聋状态。如果打开此选项，将同时启用自我禁音。</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Unlink Plugin</source>
        <comment>Global Shortcut</comment>
        <translation>卸载插件</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Force Center Position</source>
        <comment>Global Shortcut</comment>
        <translation>强制居中</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Chan Parent</source>
        <comment>Global Shortcut</comment>
        <translation>父频道</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Chan Sub#%1</source>
        <comment>Global Shortcut</comment>
        <translation>子频道#%1</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Chan All Subs</source>
        <comment>Global Shortcut</comment>
        <translation>所有子频道</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Push-to-Mute</source>
        <comment>Global Shortcut</comment>
        <translation>按键禁音</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Join Channel</source>
        <comment>Global Shortcut</comment>
        <translation>加入频道</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Toggle Overlay</source>
        <comment>Global Shortcut</comment>
        <translation>切换 Overlay</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Toggle state of in-game overlay.</source>
        <comment>Global Shortcut</comment>
        <translation>切换游戏中的 overlay 状态。</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This will switch the states of the ingame overlay between showing everybody, just the players who are talking, and nobody.</source>
        <comment>Global Shortcut</comment>
        <translation>在显示所有人，只有发言的玩家和不显示任何人之间切换游戏中的 overlay 状态。</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Alt Push-to-Talk</source>
        <comment>Global Shortcut</comment>
        <translation>交替按键通话</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Toggle Minimal</source>
        <comment>Global Shortcut</comment>
        <translation>切换最小化</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Volume Up (+10%)</source>
        <comment>Global Shortcut</comment>
        <translation>增大音量（+10%）</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Volume Down (-10%)</source>
        <comment>Global Shortcut</comment>
        <translation>减小音量（-10%）</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>Mumble</source>
        <translation></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="+13"/>
        <source>Mumble -- %1</source>
        <translation></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>&amp;Window</source>
        <translation>&amp;窗口</translation>
    </message>
    <message>
        <location line="+2"/>
        <location line="+76"/>
        <source>Minimize</source>
        <translation>最小化</translation>
    </message>
    <message>
        <location line="-76"/>
        <source>Ctrl+M</source>
        <translation></translation>
    </message>
    <message>
        <location line="+75"/>
        <source>Close</source>
        <translation>关闭</translation>
    </message>
    <message>
        <location line="-1"/>
        <source>Mumble is currently connected to a server. Do you want to Close or Minimize it?</source>
        <translation>Mumble 现在连接到了一个服务器。你想要关闭还是最小化它？</translation>
    </message>
    <message>
        <location line="+83"/>
        <source>Clear</source>
        <translation>清除</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Opening URL %1</source>
        <translation>打开网址 %1</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>URL scheme is not &apos;mumble&apos;</source>
        <translation>&apos;Mumble&apos; 不支持的网址方案</translation>
    </message>
    <message>
        <location line="+25"/>
        <source>Connecting to %1</source>
        <translation>正在连接到 %1</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Enter username</source>
        <translation>输入用户名</translation>
    </message>
    <message>
        <location line="+136"/>
        <source>Reconnecting.</source>
        <translation>正在重新连接中。</translation>
    </message>
    <message>
        <location line="+50"/>
        <source>&lt;h2&gt;Control channel&lt;/h2&gt;&lt;p&gt;Encrypted with %1 bit %2&lt;br /&gt;%3 ms average latency (%4 variance)&lt;/p&gt;</source>
        <translation>&lt;h2&gt;控制信道&lt;/h2&gt;&lt;p&gt;使用 %1 比特 %2 加密&lt;br /&gt;平均延迟 %3 毫秒（%4 方差）&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Voice channel is sent over control channel.</source>
        <translation>语音频道通过控制信道发送。</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>&lt;h2&gt;Voice channel&lt;/h2&gt;&lt;p&gt;Encrypted with 128 bit OCB-AES128&lt;br /&gt;%1 ms average latency (%4 variance)&lt;/p&gt;</source>
        <translation>&lt;h2&gt;语音频道&lt;/h2&gt;&lt;p&gt;使用 128 比特 OCB-AES128 加密&lt;br/&gt;%1 毫秒延迟（%4 方差）&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>UDP Statistics</source>
        <translation>UDP 统计</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>To Server</source>
        <translation>到服务器</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>From Server</source>
        <translation>来至服务器</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Good</source>
        <translation>良好</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Late</source>
        <translation>迟到</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Lost</source>
        <translation>丢失</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Resync</source>
        <translation>重新同步</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>&lt;h2&gt;Audio bandwidth&lt;/h2&gt;&lt;p&gt;Maximum %1 kbit/s&lt;br /&gt;Current %2 kbit/s (Quality %3)&lt;/p&gt;</source>
        <translation>&lt;h2&gt;音频带宽&lt;/h2&gt;&lt;p&gt;最大 %1千比特/秒&lt;br /&gt;当前 %2 千比特/秒 （质量 %3）&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Mumble Server Information</source>
        <translation>Mumble 服务器信息</translation>
    </message>
    <message>
        <location line="+4"/>
        <location line="+753"/>
        <source>&amp;View Certificate</source>
        <translation>&amp;查看认证</translation>
    </message>
    <message>
        <location line="-653"/>
        <source>Kicking player %1</source>
        <translation>踢出玩家 %1</translation>
    </message>
    <message>
        <location line="+0"/>
        <location line="+22"/>
        <source>Enter reason</source>
        <translation>输入原因</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Banning player %1</source>
        <translation>封禁玩家 %1</translation>
    </message>
    <message>
        <location line="+22"/>
        <source>Sending message to %1</source>
        <translation>发送消息给 %1</translation>
    </message>
    <message>
        <location line="+11"/>
        <location line="+253"/>
        <source>To %1: %2</source>
        <translation>发送给 %1: %2</translation>
    </message>
    <message>
        <location line="-253"/>
        <location line="+253"/>
        <source>Message to %1</source>
        <translation>发送消息给 %1</translation>
    </message>
    <message>
        <location line="-157"/>
        <location line="+43"/>
        <source>Channel Name</source>
        <translation>频道名</translation>
    </message>
    <message>
        <location line="-21"/>
        <source>Are you sure you want to delete %1 and all its sub-channels?</source>
        <translation>你确信要删除 %1和它的子频道吗？</translation>
    </message>
    <message>
        <location line="+43"/>
        <source>Change description of channel %1</source>
        <translation>改变频道 %1的描叙</translation>
    </message>
    <message>
        <location line="+79"/>
        <source>Sending message to channel %1</source>
        <translation>发送消息给频道 %1</translation>
    </message>
    <message>
        <location line="+27"/>
        <source>Sending message to channel tree %1</source>
        <translation>发送消息给频道树 %1</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>To tree %1: %2</source>
        <translation>发送给树 %1: %2</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Message to tree %1</source>
        <translation>发送消息给树 %1</translation>
    </message>
    <message>
        <location line="+21"/>
        <source>Unmuted and undeafened.</source>
        <translation>解除静音并解除耳聋。</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Unmuted.</source>
        <translation>解除静音。</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Muted.</source>
        <translation>静音。</translation>
    </message>
    <message>
        <location line="+20"/>
        <source>Muted and deafened.</source>
        <translation>禁音和耳聋.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Deafened.</source>
        <translation>耳聋.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Undeafened.</source>
        <translation>解除耳聋。</translation>
    </message>
    <message>
        <location line="+62"/>
        <source>About Qt</source>
        <translation>关于 Qt</translation>
    </message>
    <message>
        <location line="+97"/>
        <source>Joining %1.</source>
        <translation>加入 %1.</translation>
    </message>
    <message>
        <location line="+33"/>
        <source>Connected to server %1.</source>
        <translation>已连接到服务器 %1.</translation>
    </message>
    <message>
        <location line="+56"/>
        <source>SSL Verification failed: %1</source>
        <translation>SSL 认证失败: %1</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>&lt;b&gt;WARNING:&lt;/b&gt; The server presented a certificate that was different from the stored one.</source>
        <translation>&lt;b&gt;警告:&lt;/b&gt;服务器提供的认证与保存的认证不同。</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Sever presented a certificate which failed verification.</source>
        <translation>服务器提供的认证认证失败。</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>&lt;p&gt;%1.&lt;br /&gt;The specific errors with this certificate are: &lt;/p&gt;&lt;ol&gt;%2&lt;/ol&gt;&lt;p&gt;Do you wish to accept this certificate anyway?&lt;br /&gt;(It will also be stored so you won&apos;t be asked this again.)&lt;/p&gt;</source>
        <translation>&lt;p&gt;%1.&lt;br /&gt; 认证中的错误是：&lt;/p&gt;&lt;ol&gt;%2&lt;/ol&gt;&lt;p&gt;你希望接受认证吗？&lt;br /&gt;（认证将被保存从而你不会被再次索要认证。）&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+28"/>
        <source>Server connection failed: %1.</source>
        <translation>连接服务器失败: %1.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Disconnected from server.</source>
        <translation>与服务器断开连接.</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Invalid username</source>
        <translation>无效用户名</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>You connected with an invalid username, please try another one.</source>
        <translation>你使用一个无效的用户名连接，请尝试别外一个。</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>That username is already in use, please try another username.</source>
        <translation>用户名已经被使用，请使用另外一个用户名。</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Wrong password</source>
        <translation>密码错误</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Wrong password for registered users, please try again.</source>
        <translation>注册用户密码错误，请重试。</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Wrong server password for unregistered user account, please try again.</source>
        <translation>未注册用户的服务器密码错误，请重试。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>&amp;Server</source>
        <translation>&amp;服务器</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Player</source>
        <translation>&amp;玩家</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Channel</source>
        <translation>&amp;频道</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Audio</source>
        <translation>&amp;音频</translation>
    </message>
    <message>
        <location/>
        <source>C&amp;onfigure</source>
        <translation>&amp;配置</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Help</source>
        <translation>&amp;帮助</translation>
    </message>
    <message>
        <location/>
        <source>Log</source>
        <translation>日志</translation>
    </message>
    <message>
        <location/>
        <source>This shows all recent activity. Connecting to servers, errors and information messages all show up here.&lt;br /&gt;To configure exactly which messages show up here, use the &lt;b&gt;Settings&lt;/b&gt; command from the menu.</source>
        <translation>这里显示所有最近的活动。正在连接到服务器，错误和通知消息都将在这里显示。&lt;br /&gt;要准确配置什么消息将被显示在这里，请从菜单中使用&lt;b&gt;设置&lt;/b&gt;命令。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Quit Mumble</source>
        <translation>&amp;退出 Mumble</translation>
    </message>
    <message>
        <location/>
        <source>Closes the program</source>
        <translation>关闭程序</translation>
    </message>
    <message>
        <location/>
        <source>Exits the application.</source>
        <translation>退出应用程序。</translation>
    </message>
    <message>
        <location/>
        <source>Ctrl+Q</source>
        <translation></translation>
    </message>
    <message>
        <location/>
        <source>&amp;Connect</source>
        <translation>&amp;连接</translation>
    </message>
    <message>
        <location/>
        <source>Open the server connection dialog</source>
        <translation>打开服务器连接对话框</translation>
    </message>
    <message>
        <location/>
        <source>Shows a dialog of registered servers, and also allows quick connect.</source>
        <translation>显示一个已注册服务器的对话框，并允许快速连接。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Disconnect</source>
        <translation>&amp;断开</translation>
    </message>
    <message>
        <location/>
        <source>Disconnect from server</source>
        <translation>从服务器断开连接</translation>
    </message>
    <message>
        <location/>
        <source>Disconnects you from the server.</source>
        <translation>使你从服务器断开连接。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Ban lists</source>
        <translation>&amp;黑名单</translation>
    </message>
    <message>
        <location/>
        <source>Edit ban lists on server</source>
        <translation>编辑服务器上的黑名单</translation>
    </message>
    <message>
        <location/>
        <source>This lets you edit the server-side IP ban lists.</source>
        <translation>这让你编辑服务器端的网址黑名单。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Information</source>
        <translation>&amp;信息</translation>
    </message>
    <message>
        <location/>
        <source>Show information about the server connection</source>
        <translation>显示服务器连接信息</translation>
    </message>
    <message>
        <location/>
        <source>This will show extended information about the connection to the server.</source>
        <translation>显示服务器连接的扩展信息.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Kick</source>
        <translation>&amp;踢出</translation>
    </message>
    <message>
        <location/>
        <source>Kick player (with reason)</source>
        <translation>踢出玩家（理由）</translation>
    </message>
    <message>
        <location/>
        <source>Kick selected player off server. You&apos;ll be asked to specify a reason.</source>
        <translation>从服务器踢出所选玩家。你将被要求指定一个原因。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Mute</source>
        <translation>&amp;静音</translation>
    </message>
    <message>
        <location/>
        <source>Mute player</source>
        <translation>静音玩家</translation>
    </message>
    <message>
        <location/>
        <source>Mute or unmute player on server. Unmuting a deafened player will also undeafen them.</source>
        <translation>在服务器上对玩家静音或取消静音。取消静音一个耳聋的玩家将同时对他取消耳聋。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Ban</source>
        <translation>&amp;封禁</translation>
    </message>
    <message>
        <location/>
        <source>Kick and ban player (with reason)</source>
        <translation>踢出并封禁玩家（理由）</translation>
    </message>
    <message>
        <location/>
        <source>Kick and ban selected player from server. You&apos;ll be asked to specify a reason.</source>
        <translation>从服务器踢出并封禁所选玩家。你将被要求指定一个原因。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Deafen</source>
        <translation>&amp;耳聋</translation>
    </message>
    <message>
        <location/>
        <source>Deafen player</source>
        <translation>耳聋玩家</translation>
    </message>
    <message>
        <location/>
        <source>Deafen or undeafen player on server. Deafening a player will also mute them.</source>
        <translation>在服务器上使玩家耳聋或取消耳聋。耳聋一个玩家将同时对他静音。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Local Mute</source>
        <translation>&amp;本地静音</translation>
    </message>
    <message>
        <location/>
        <source>Mute player locally</source>
        <translation>本地对玩家静音</translation>
    </message>
    <message>
        <location/>
        <source>Mute or unmute player locally. Use this on other players in the same room.</source>
        <translation>本地对玩家静音或者取消静音. 对相同房间的其他玩家使用该选项。</translation>
    </message>
    <message>
        <location/>
        <source>Send Messa&amp;ge</source>
        <translation>发送&amp;消息</translation>
    </message>
    <message>
        <location/>
        <source>Send a Text Message</source>
        <translation>发送一条文本消息</translation>
    </message>
    <message>
        <location/>
        <source>Sends a text message to another user.</source>
        <translation>向另一个用户发送一条文本消息。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Add</source>
        <translation>&amp;添加</translation>
    </message>
    <message>
        <location/>
        <source>Add new channel</source>
        <translation>添加一个新频道</translation>
    </message>
    <message>
        <location/>
        <source>This adds a new sub-channel to the currently selected channel.</source>
        <translation>在当前所选频道内添加一个子频道。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Remove</source>
        <translation>&amp;删除</translation>
    </message>
    <message>
        <location/>
        <source>Remove channel</source>
        <translation>删除频道</translation>
    </message>
    <message>
        <location/>
        <source>This removes a channel and all sub-channels.</source>
        <translation>删除一个频道及其所有子频道。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Edit ACL</source>
        <translation>&amp;编辑 访问控制列表</translation>
    </message>
    <message>
        <location/>
        <source>Edit Groups and ACL for channel</source>
        <translation>为频道编辑群和访问控制列表</translation>
    </message>
    <message>
        <location/>
        <source>This opens the Group and ACL dialog for the channel, to control permissions.</source>
        <translation>打开频道的群和访问控制列表对话框，来控制频道的访问权限。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Rename</source>
        <translation>&amp;重命名</translation>
    </message>
    <message>
        <location/>
        <source>Renames the channel</source>
        <translation>重命名频道</translation>
    </message>
    <message>
        <location/>
        <source>This renames a channel.</source>
        <translation>重命名一个频道。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Change Description</source>
        <translation>&amp;修改描叙</translation>
    </message>
    <message>
        <location/>
        <source>Changes the channel description</source>
        <translation>修改频道描叙</translation>
    </message>
    <message>
        <location/>
        <source>This changes the description of a channel.</source>
        <translation>修改一个频道的描叙。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Link</source>
        <translation>&amp;链接</translation>
    </message>
    <message>
        <location/>
        <source>Link your channel to another channel</source>
        <translation>链接你的频道和另一个频道</translation>
    </message>
    <message>
        <location/>
        <source>This links your current channel to the selected channel. If they have permission to speak in the other channel, players can now hear each other. This is a permanent link, and will last until manually unlinked or the server is restarted. Please see the shortcuts for push-to-link.</source>
        <translation>链接你的当前频道和所选频道。如果他们有权限在对方频道讲话，玩家现在能听到彼此的声音。这是个永久性的链接，直到手动取消链接或者服务器重启。请查看按键链接快捷键。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Unlink</source>
        <comment>Channel</comment>
        <translation>&amp;取消链接</translation>
    </message>
    <message>
        <location/>
        <source>Unlink your channel from another channel</source>
        <translation>取消你的频道和另一个频道之间的链接</translation>
    </message>
    <message>
        <location/>
        <source>This unlinks your current channel from the selected channel.</source>
        <translation>取消你的当前频道和所选频道之间的链接。</translation>
    </message>
    <message>
        <location/>
        <source>Unlink &amp;All</source>
        <translation>取消&amp;所有链接</translation>
    </message>
    <message>
        <location/>
        <source>Unlinks your channel from all linked channels.</source>
        <translation>取消你的频道和所有已链接频道之间的链接。</translation>
    </message>
    <message>
        <location/>
        <source>This unlinks your current channel (not the selected one) from all linked channels.</source>
        <translation>取消你的当前频道（非所选频道）和所有已链接的频道之间的链接。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Reset</source>
        <translation>&amp;重置</translation>
    </message>
    <message>
        <location/>
        <source>Reset audio preprocessor</source>
        <translation>重置音频预处理器</translation>
    </message>
    <message>
        <location/>
        <source>This will reset the audio preprocessor, including noise cancellation, automatic gain and voice activity detection. If something suddenly worsens the audio environment (like dropping the microphone) and it was temporary, use this to avoid having to wait for the preprocessor to readjust.</source>
        <translation>该选项将重置音频预处理器，包含噪音消除，自动获取和语音活动检测。如果音频环境突然变的恶劣（比如掉麦）并且是暂时的，使用该选项来避免等待预处理器自我调节。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Mute Self</source>
        <translation>&amp;静音自己</translation>
    </message>
    <message>
        <location/>
        <source>Mute yourself</source>
        <translation>自我静音</translation>
    </message>
    <message>
        <location/>
        <source>Mute or unmute yourself. When muted, you will not send any data to the server. Unmuting while deafened will also undeafen.</source>
        <translation>对自己静音或取消静音。当静音时，你将不会发送数据到服务器。当耳聋时取消静音将同时取消耳聋。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Deafen Self</source>
        <translation>&amp;耳聋自己</translation>
    </message>
    <message>
        <location/>
        <source>Deafen yourself</source>
        <translation>自我耳聋</translation>
    </message>
    <message>
        <location/>
        <source>Deafen or undeafen yourself. When deafened, you will not hear anything. Deafening yourself will also mute.</source>
        <translation>使自己耳聋或取消耳聋。当耳聋时，你将听不到声音。耳聋自己将同时静音自己。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Text-To-Speech</source>
        <translation>&amp;文本语音转换</translation>
    </message>
    <message>
        <location/>
        <source>Toggle Text-To-Speech</source>
        <translation>切换文本语音转换</translation>
    </message>
    <message>
        <location/>
        <source>Enable or disable the text-to-speech engine. Only messages enabled for TTS in the Configuration dialog will actually be spoken.</source>
        <translation>启用或禁用文本语音转换引擎。只有在配置对话框里被启用了文本语音转换的消息才会被读出来。</translation>
    </message>
    <message>
        <location/>
        <source>S&amp;tatistics</source>
        <translation>&amp;统计</translation>
    </message>
    <message>
        <location/>
        <source>Display audio statistics</source>
        <translation>显示音频统计</translation>
    </message>
    <message>
        <location/>
        <source>Pops up a small dialog with information about your current audio input.</source>
        <translation>弹出一个关于你当前音频输入信息的对话框。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Unlink</source>
        <comment>Plugin</comment>
        <translation>&amp;卸载</translation>
    </message>
    <message>
        <location/>
        <source>Forcibly unlink plugin</source>
        <translation>强行卸载插件</translation>
    </message>
    <message>
        <location/>
        <source>This forces the current plugin to unlink, which is handy if it is reading completely wrong data.</source>
        <translation>强行卸载当前插件，当插件读取的数据完全错误时相当有用。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Settings</source>
        <translation>&amp;设置</translation>
    </message>
    <message>
        <location/>
        <source>Configure Mumble</source>
        <translation>配置 Mumble</translation>
    </message>
    <message>
        <location/>
        <source>Allows you to change most settings for Mumble.</source>
        <translation>允许你改变 Mumble 的大多数设置。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Audio Wizard</source>
        <translation>&amp;音频精灵</translation>
    </message>
    <message>
        <location/>
        <source>Start the audio configuration wizard</source>
        <translation>启动音频配置精灵</translation>
    </message>
    <message>
        <location/>
        <source>This will guide you through the process of configuring your audio hardware.</source>
        <translation>这将引导你进入配置你的音频硬件的流程。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;What&apos;s This?</source>
        <translation>&amp;这是什么?</translation>
    </message>
    <message>
        <location/>
        <source>Enter What&apos;s This? mode</source>
        <translation>进入 这是什么? 模式</translation>
    </message>
    <message>
        <location/>
        <source>Click this to enter &quot;What&apos;s This?&quot; mode. Your cursor will turn into a question mark. Click on any button, menu choice or area to show a description of what it is.</source>
        <translation>点击进入&quot;这是什么?&quot;模式。你的鼠标指针将变成一个问号。点击任意按钮，菜单或区域来显示它是什么的描叙。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;About</source>
        <translation>&amp;关于</translation>
    </message>
    <message>
        <location/>
        <source>Information about Mumble</source>
        <translation>Mumble 的相关信息</translation>
    </message>
    <message>
        <location/>
        <source>Shows a small dialog with information and license for Mumble.</source>
        <translation>显示一个 Mumble 相关信息和许可的对话框。</translation>
    </message>
    <message>
        <location/>
        <source>About &amp;Speex</source>
        <translation>关于 &amp;Speex</translation>
    </message>
    <message>
        <location/>
        <source>Information about Speex</source>
        <translation>Speex 的相关信息</translation>
    </message>
    <message>
        <location/>
        <source>Shows a small dialog with information about Speex.</source>
        <translation>显示一个 Speex 相关信息的小对话框。</translation>
    </message>
    <message>
        <location/>
        <source>About &amp;Qt</source>
        <translation>关于 &amp;Qt</translation>
    </message>
    <message>
        <location/>
        <source>Information about Qt</source>
        <translation>Qt 相关信息</translation>
    </message>
    <message>
        <location/>
        <source>Shows a small dialog with information about Qt.</source>
        <translation>显示一个 Qt 相关信息的小对话框。</translation>
    </message>
    <message>
        <location/>
        <source>Check for &amp;Updates</source>
        <translation>检测&amp;更新</translation>
    </message>
    <message>
        <location/>
        <source>Check for new version of Mumble</source>
        <translation>检测 Mumble 的新版本</translation>
    </message>
    <message>
        <location/>
        <source>Connects to the Mumble webpage to check if a new version is available, and notifies you with an appropriate download URL if this is the case.</source>
        <translation>连接到 Mumble 主页来检测是否有新版本可用，如果有新版本可用将通知你下载。</translation>
    </message>
    <message>
        <location/>
        <source>Sends a text message to all users in a channel.</source>
        <translation>向频道内所有用户发送文本信息。</translation>
    </message>
    <message>
        <location/>
        <source>Send &amp;Tree Message</source>
        <translation>发送信息到频道&amp;树</translation>
    </message>
    <message>
        <location/>
        <source>Send message to channel tree.</source>
        <translation>向频道树发送信息.</translation>
    </message>
    <message>
        <location/>
        <source>This sends a text message to a channel and it&apos;s subchannels.</source>
        <translation>向一个频道及其所有子频道发送文本消息。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Minimal View</source>
        <translation>&amp;简化窗口</translation>
    </message>
    <message>
        <location/>
        <source>Toggle minimal window modes</source>
        <translation>启用简化窗口模式</translation>
    </message>
    <message>
        <location/>
        <source>This will toggle minimal mode, where the log window and menu is hidden.</source>
        <translation>这将启用简化窗口模式，该模式将隐藏日志窗口和菜单。</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="+56"/>
        <source>Joined server: %1.</source>
        <translation>加入服务器: %1.</translation>
    </message>
    <message>
        <location line="+20"/>
        <source>Left server: %1.</source>
        <translation>离开服务器: %1.</translation>
    </message>
    <message>
        <location line="+33"/>
        <source>%1 is now muted and deafened.</source>
        <translation>%1 现在被静音和耳聋.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1 is now muted.</source>
        <translation>%1 现在被静音。</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1 is now unmuted.</source>
        <translation>%1 现在被取消静音。</translation>
    </message>
    <message>
        <location line="+13"/>
        <location line="+20"/>
        <source>server</source>
        <translation>服务器</translation>
    </message>
    <message>
        <location line="-17"/>
        <source>You were muted by %1.</source>
        <translation>你被 %1 静音。</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>You were unmuted by %1.</source>
        <translation>你被 %1 取消静音。</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>%1 muted by %2.</source>
        <translation>%1 被 %2 静音。</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>%1 unmuted by %2.</source>
        <translation>%1 被 %2 取消静音。</translation>
    </message>
    <message>
        <location line="+17"/>
        <source>You were deafened by %1.</source>
        <translation>你被 %1 耳聋。</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>You were undeafened by %1.</source>
        <translation>你被 %1 取消耳聋。</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>%1 deafened by %2.</source>
        <translation>%1 被 %2 耳聋。</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>%1 undeafened by %2.</source>
        <translation>%1 被 %2 取消耳聋。</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>You were kicked from the server by %1: %2.</source>
        <translation>你被 %1 从服务器踢出: %2。</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>%3 was kicked from the server by %1: %2.</source>
        <translation>%3 被 %1 从服务器踢出: %2。</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>You were kicked and banned from the server by %1: %2.</source>
        <translation>你被 %1 从服务器踢出并封禁: %2。</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>%3 was kicked and banned from the server by %1: %2.</source>
        <translation>%3 被 %1 从服务器踢出并封禁: %2。</translation>
    </message>
    <message>
        <location line="+27"/>
        <source>You were moved to %1 by %2.</source>
        <translation>你被从 %1 移动到 %2。</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>%1 left channel.</source>
        <translation>%1 离开频道。</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1 moved out by %2 to %3.</source>
        <translation>%1 被 %2 移动到 %3。</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>%1 moved in by %2 from %3.</source>
        <translation>%1 被 %2 从 %3 移入。</translation>
    </message>
    <message>
        <location line="-2"/>
        <source>%1 entered channel.</source>
        <translation>%1 进入频道。</translation>
    </message>
    <message>
        <location line="+100"/>
        <source>Server connection rejected: %1.</source>
        <translation>服务器拒绝连接: %1。</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Denied: %1.</source>
        <translation>拒绝: %1。</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Server maximum bandwidth is only %1 kbit/s. Quality auto-adjusted.</source>
        <translation>服务器最大带宽是 %1 千比特/秒。自动调整质量。</translation>
    </message>
    <message>
        <location line="+11"/>
        <source>From %1: %2</source>
        <translation>发送自 %1: %2</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Message from %1</source>
        <translation>来至 %1 的消息</translation>
    </message>
    <message>
        <location filename="PlayerModel.cpp" line="+995"/>
        <source>You have Channel Dragging set to &quot;Do Nothing&quot; so the channel wasn&apos;t moved.</source>
        <translation>你设置频道拖动为“无动作&quot;因此频道没有被移动。</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Unknown Channel Drag mode in PlayerModel::dropMimeData.</source>
        <translation>未知的频道拖动模式 PlayerModel::dropMimeData。</translation>
    </message>
    <message>
        <location filename="main.cpp" line="+239"/>
        <source>Welcome to Mumble.</source>
        <translation>欢迎来到 Mumble。</translation>
    </message>
    <message>
        <location line="+17"/>
        <source>This is the first time you&apos;re starting Mumble.&lt;br /&gt;Would you like to go through the Audio Wizard to configure your soundcard?</source>
        <translation>这是你第一次启动 Mumble.&lt;br /&gt;你想用音频精灵配置声卡吗？</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="-2"/>
        <source>Server</source>
        <comment>message from</comment>
        <translation>服务器</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>Hide Frame</source>
        <translation>隐藏边框</translation>
    </message>
    <message>
        <location/>
        <source>Toggle showing frame on minimal window</source>
        <translation>在简化窗口模式下切换边框显示</translation>
    </message>
    <message>
        <location/>
        <source>This will toggle whether the minimal window should have a frame for moving and resizing.</source>
        <translation>该选项切换简化窗口是否有个用来移动和改变大小的边框。</translation>
    </message>
</context>
<context>
    <name>NetworkConfig</name>
    <message>
        <location filename="NetworkConfig.cpp" line="+51"/>
        <source>Network</source>
        <translation>网络</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui"/>
        <source>Form</source>
        <translation>表单</translation>
    </message>
    <message>
        <location/>
        <source>Connection</source>
        <translation>连接</translation>
    </message>
    <message>
        <location/>
        <source>Use TCP compatibility mode</source>
        <oldsource>Use TCP compatability mode</oldsource>
        <translation>使用 TCP 兼容模式</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Enable TCP compatibility mode&lt;/b&gt;.&lt;br /&gt;This will make Mumble use only TCP when communicating with the server. This will increase overhead and cause lost packets to produce noticeable pauses in communication, so this should only be used if you are unable to use the default (which uses UDP for voice and TCP for control).</source>
        <translation>&lt;b&gt;启用 TCP 兼容模式&lt;/b&gt;。&lt;br/&gt;这将使 Mumble 与服务器通信时只使用 TCP。这将增加系统开销并导致丢包从而使通信时出现明显的暂停，因此只有在你不能使用默认模式时使用它（默认模式使用 UDP 来传输语音，TCP 来控制）。</translation>
    </message>
    <message>
        <location/>
        <source>Use TCP mode</source>
        <translation>使用 TCP 模式</translation>
    </message>
    <message>
        <location/>
        <source>Reconnect when disconnected</source>
        <translation>连接断开后重新连接</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Reconnect when disconnected&lt;/b&gt;.&lt;br /&gt;This will make Mumble try to automatically reconnect after 10 seconds if your server connection fails.</source>
        <translation>&lt;b&gt;连接断开后重新连接&lt;/b&gt;。&lt;br/&gt;这将使 Mumble 在与服务器的连接断开 10秒后自动重新连接。</translation>
    </message>
    <message>
        <location/>
        <source>Reconnect automatically</source>
        <translation>自动重新连接</translation>
    </message>
    <message>
        <location/>
        <source>Proxy</source>
        <translation>代理</translation>
    </message>
    <message>
        <location/>
        <source>Type</source>
        <translation>类型</translation>
    </message>
    <message>
        <location/>
        <source>Type of proxy to connect through</source>
        <translation>连接通过的代理类型</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Type of proxy to connect through&lt;/b&gt;&lt;br /&gt;This makes Mumble connect through a proxy for all outgoing connections. Note: Proxy tunneling forces Mumble into TCP compatibility mode, causing all voice data to be sent via the control channel.</source>
        <oldsource>&lt;b&gt;Type of proxy to connect through&lt;/b&gt;&lt;br /&gt;This makes Mumble connect through a proxy for all outgoing connections. Note: Proxy tunneling forces Mumble into TCP compatability mode, causing all voice data to be sent via the control channel.</oldsource>
        <translation>&lt;b&gt;连接通过的代理类型&lt;/b&gt;&lt;br/&gt;这使 Mumble 通过代理来连接到所有出外的连接。注意: 代理隧道封装会迫使 Mumble 进入 TCP 兼容模式，导致所有语音数据都通过控制信道发送。</translation>
    </message>
    <message>
        <location/>
        <source>Direct connection</source>
        <translation>直接连接</translation>
    </message>
    <message>
        <location/>
        <source>HTTP(S) proxy</source>
        <translation>HTTP(S) 代理</translation>
    </message>
    <message>
        <location/>
        <source>SOCKS5 proxy</source>
        <translation>SOCKS5 代理</translation>
    </message>
    <message>
        <location/>
        <source>Hostname</source>
        <translation>主机名</translation>
    </message>
    <message>
        <location/>
        <source>Hostname of the proxy</source>
        <translation>代理主机名</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Hostname of the proxy.&lt;b&gt;&lt;br /&gt;This field specifies the hostname of the proxy you wish to tunnel network traffic through.</source>
        <translation>&lt;b&gt;代理主机名。&lt;/b&gt;&lt;br /&gt;该字段指定你希望用来穿隧传输网络流量的代理主机名。</translation>
    </message>
    <message>
        <location/>
        <source>Port</source>
        <translation>端口</translation>
    </message>
    <message>
        <location/>
        <source>Port number of the proxy</source>
        <translation>代理端口号</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Port number of the proxy.&lt;/b&gt;&lt;br /&gt;This field specifies the port number that the proxy expects connections on.</source>
        <translation>&lt;b&gt;代理服务器端口号。&lt;/b&gt;&lt;br/&gt;该字段指定代理服务器使用的端口号。</translation>
    </message>
    <message>
        <location/>
        <source>Username</source>
        <translation>用户名</translation>
    </message>
    <message>
        <location/>
        <source>Username for proxy authentication</source>
        <translation>用于代理认证的用户名</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Username for proxy authentication.&lt;/b&gt;&lt;br /&gt;This specifies the username you use for authenticating yourself with the proxy. In case the proxy does not use authentication, or you want to connect anonymously, simply leave this field blank.</source>
        <translation>&lt;b&gt;用于代理认证的用户名。&lt;/b&gt;&lt;br /&gt;该字段指定用于代理认证的用户名。如果代理不需要认证，或者你想要使用匿名连接，请将此字段留白。</translation>
    </message>
    <message>
        <location/>
        <source>Password</source>
        <translation>密码</translation>
    </message>
    <message>
        <location/>
        <source>Password for proxy authentication</source>
        <translation>用于代理认证的密码</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Password for proxy authentication.&lt;/b&gt;&lt;br /&gt;This specifies the password you use for authenticating yourself with the proxy. In case the proxy does not use authentication, or you want to connect anonymously, simply leave this field blank.</source>
        <translation>&lt;b&gt;用于代理认证的密码。&lt;/b&gt;&lt;br/&gt;该字段指定用于代理认证的密码。如果代理不需要认证，或者你想要使用匿名连接，请将此字段留白。</translation>
    </message>
    <message>
        <location/>
        <source>Mumble services</source>
        <translation>Mumble 服务</translation>
    </message>
    <message>
        <location/>
        <source>Check for updates on startup</source>
        <translation>启动时检测更新</translation>
    </message>
    <message>
        <location/>
        <source>Submit anonymous statistics to the Mumble project</source>
        <translation>递交匿名统计数据给 Mumble 项目</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Submit anonymous statistics.&lt;/b&gt;&lt;br /&gt;Mumble has a small development team, and as such needs to focus it&apos;s development where it&apos;s needed most. By submitting a bit of statistics you help the project determine where to focus development.</source>
        <translation>&lt;b&gt;递交匿名统计数据。&lt;/b&gt;&lt;br /&gt;Mumble 有一个小的开发团队，并需要专注于开发用户最想要的功能。通过递交匿名统计数据你将帮助开发团队确定重点开发什么。</translation>
    </message>
    <message>
        <location/>
        <source>Submit anonymous statistics</source>
        <translation>递交匿名统计数据</translation>
    </message>
</context>
<context>
    <name>Overlay</name>
    <message>
        <source>Failed to load overlay library. This means either that:
- the library (mumble_ol.dll) wasn&apos;t found in the directory you ran Mumble from
- you&apos;re on an OS earlier than WinXP SP2
- you do not have the June 2007 updated version of DX9.0c</source>
        <translation type="obsolete">无法加载 overlay 库。这意味着：
- 库（mumble_ol.dll）没有在 Mumble 的运行目录里被找到。
- 你的操作系统版本低于 WinXP SP2
- 你的 DX9.0C 版本低于 2007年6月升级版</translation>
    </message>
</context>
<context>
    <name>OverlayConfig</name>
    <message>
        <source>Show no one</source>
        <translation type="obsolete">不显示任何人</translation>
    </message>
    <message>
        <source>Show only talking</source>
        <translation type="obsolete">当发言时显示</translation>
    </message>
    <message>
        <source>Show everyone</source>
        <translation type="obsolete">显示所有人</translation>
    </message>
    <message>
        <source>Form</source>
        <translation type="obsolete">表单</translation>
    </message>
    <message>
        <source>Options</source>
        <translation type="obsolete">选项</translation>
    </message>
    <message>
        <source>Enable overlay.</source>
        <translation type="obsolete">启用 overlay .</translation>
    </message>
    <message>
        <source>This sets whether the overlay is enabled or not. This setting is only checked with D3D9 applications are started, so make sure Mumble is running and this option is on before you start the application.&lt;br /&gt;Please note that if you start the application after starting Mumble, or if you disable the overlay while running, there is no safe way to restart the overlay without also restarting the application.</source>
        <translation type="obsolete">设置是否启用 overlay。该设置仅当应用程序已启动时才会被检查，因此确保 Mumble 正在运行并且该选项在你启动应用程序之前已被打开。&lt;br /&gt;请注意如果你在启动 Mumble 后启动应用程序，或者你在运行时禁用 overlay，重新启动 overlay 需要重新启动应用程序。</translation>
    </message>
    <message>
        <source>Enable Overlay</source>
        <translation type="obsolete">启用 Overlay</translation>
    </message>
    <message>
        <source>Who to show on the overlay</source>
        <translation type="obsolete">在 overlay 上显示谁</translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets who to show in the in-game overlay.&lt;/b&gt;&lt;br /&gt;If many people are connected to the same channel, the overlay list might be very long. Use this to shorten it.&lt;br /&gt;&lt;i&gt;No one&lt;/i&gt; - Don&apos;t show anyone (but leave overlay running).&lt;br /&gt;&lt;i&gt;Only talking&lt;/i&gt; - Only show talking people.&lt;br /&gt;&lt;i&gt;Everyone&lt;/i&gt; - Show everyone.</source>
        <translation type="obsolete">&lt;b&gt;设置在游戏中的 overlay 内显示谁。&lt;/b&gt;&lt;br /&gt;如果许多玩家已经连接到同一个频道，overlay 列表可能会非常长。使用该选项来让列表变短。&lt;br /&gt;&lt;i&gt;无人&lt;/i&gt; - 不显示任何人（但是让 overlay 运行).&lt;br /&gt;&lt;i&gt;仅当用户发言时&lt;/i&gt; - 仅显示正在发言的人。&lt;br /&gt;&lt;i&gt;所有人&lt;/i&gt; - 显示所有人。</translation>
    </message>
    <message>
        <source>Always show yourself on overlay.</source>
        <translation type="obsolete">在 overlay 上总是显示自己。</translation>
    </message>
    <message>
        <source>This sets whether to always show yourself or not. This setting is useful if you aren&apos;t showing everyone in the overlay, as then you would only see your own status if you were talking, which wouldn&apos;t let you see that you were deafened or muted.</source>
        <translation type="obsolete">设置是否总是显示自己。该选项只有当你不在 overlay 中显示所有人时才有用，你在讲话时只能看到自己的状态，你将看不到自己被禁音或耳聋。</translation>
    </message>
    <message>
        <source>Always Show Self</source>
        <translation type="obsolete">总是显示自己</translation>
    </message>
    <message>
        <source>Show User custom textures instead of text on the overlay.</source>
        <translation type="obsolete">在 overlay 上显示用户自定义纹理而不是文本。</translation>
    </message>
    <message>
        <source>This sets whether to download and use custom textures for registered users. If disabled, the regular outline text will be used instead.</source>
        <translation type="obsolete">设置是否让注册用户下载并使用自定义纹理。如果禁用该选项，常规边框文字将被使用。</translation>
    </message>
    <message>
        <source>Show User Textures</source>
        <translation type="obsolete">显示用户纹理</translation>
    </message>
    <message>
        <source>Position</source>
        <translation type="obsolete">位置</translation>
    </message>
    <message>
        <source>Let overlay grow upwards</source>
        <translation type="obsolete">让 overlay 往上增长</translation>
    </message>
    <message>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow upwards if needed.</source>
        <translation type="obsolete">Overlay 试着保持足够小并固定在你选定的位置上。该选项允许 overlay 在需要时往上增长。</translation>
    </message>
    <message>
        <source>Grow Up</source>
        <translation type="obsolete">往上增长</translation>
    </message>
    <message>
        <source>Y-Position of Overlay</source>
        <translation type="obsolete">Overlay 的 Y-坐标</translation>
    </message>
    <message>
        <source>This sets the relative Y position of the overlay.</source>
        <translation type="obsolete">设置 overlay 的相对 Y 坐标。</translation>
    </message>
    <message>
        <source>Let overlay grow to the left</source>
        <translation type="obsolete">让 overlay 往左增长</translation>
    </message>
    <message>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow to the left if needed.</source>
        <translation type="obsolete">Overlay 试着保持足够小并固定在你选定的位置上。该选项允许 overlay 在需要时往左增长。</translation>
    </message>
    <message>
        <source>Grow Left</source>
        <translation type="obsolete">往左增长</translation>
    </message>
    <message>
        <source>Let overlay grow to the right</source>
        <translation type="obsolete">让 overlay 往右增长</translation>
    </message>
    <message>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow to the right if needed.</source>
        <translation type="obsolete">Overlay 试着保持足够小并固定在你选定的位置上。该选项允许 overlay 在需要时往右增长。</translation>
    </message>
    <message>
        <source>Grow Right</source>
        <translation type="obsolete">往右增长</translation>
    </message>
    <message>
        <source>Let overlay grow downwards</source>
        <translation type="obsolete">让 overlay 往下增长</translation>
    </message>
    <message>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow downwards if needed.</source>
        <translation type="obsolete">Overlay 试着保持足够小并固定在你选定的位置上。该选项允许 overlay 在需要时往下增长。</translation>
    </message>
    <message>
        <source>Grow Down</source>
        <translation type="obsolete">往下增长</translation>
    </message>
    <message>
        <source>X-Position of Overlay</source>
        <translation type="obsolete">Overlay 的 X-坐标</translation>
    </message>
    <message>
        <source>This sets the relative X position of the overlay.</source>
        <translation type="obsolete">设置 overlay 的相对 X 坐标。</translation>
    </message>
    <message>
        <source>Font</source>
        <translation type="obsolete">字体</translation>
    </message>
    <message>
        <source>Current Font</source>
        <translation type="obsolete">当前字体</translation>
    </message>
    <message>
        <source>TextLabel</source>
        <translation type="obsolete">文本标签</translation>
    </message>
    <message>
        <source>Set Font</source>
        <translation type="obsolete">设置字体</translation>
    </message>
    <message>
        <source>Maximum height</source>
        <translation type="obsolete">最大高度</translation>
    </message>
    <message>
        <source>Maximum height of names.</source>
        <translation type="obsolete">名字的最大高度.</translation>
    </message>
    <message>
        <source>This sets the maximum height of names shown, relative to the screen height. If your active 3D window is 800 pixels tall and this is set to 5%, each name will be 40 pixels tall. Note that the names will not be taller than 60 pixels no matter what you set here.</source>
        <translation type="obsolete">设置显示的名字的最大高度，相对于屏幕高度来说。如果你的活跃 3D 窗口是 800像素高而该值被设成 5%，每个名字会是 40像素高。注意无论你在这里设置什么值名字都不会高于 60像素.</translation>
    </message>
    <message>
        <source>Color for players</source>
        <translation type="obsolete">玩家颜色</translation>
    </message>
    <message>
        <source>Change</source>
        <translation type="obsolete">更改</translation>
    </message>
    <message>
        <source>Color for talking players</source>
        <translation type="obsolete">正在发言玩家的颜色</translation>
    </message>
    <message>
        <source>Color for alt-talking players</source>
        <translation type="obsolete">试图发言玩家的颜色</translation>
    </message>
    <message>
        <source>Color for Channels</source>
        <translation type="obsolete">频道的颜色</translation>
    </message>
    <message>
        <source>Color for active Channels</source>
        <translation type="obsolete">活跃频道的颜色</translation>
    </message>
</context>
<context>
    <name>PlayerModel</name>
    <message>
        <location filename="PlayerModel.cpp" line="-581"/>
        <source>This is a player connected to the server. The icon to the left of the player indicates whether or not they are talking:&lt;br /&gt;&lt;img src=&quot;skin:talking_on.png&quot; /&gt; Talking&lt;br /&gt;&lt;img src=&quot;skin:talking_off.png&quot; /&gt; Not talking</source>
        <translation>这是一个连接到服务器的玩家。玩家左边的图标显示他是否在讲话。&lt;br /&gt;&lt;img src=&quot;skin:talking_on.png&quot; /&gt;发言中&lt;br /&gt;&lt;img src=&quot;skin:talking_off.png&quot; /&gt; 沉默中</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>This is a channel on the server. Only players in the same channel can hear each other.</source>
        <translation>这是服务器上的一个频道。只有同一频道的玩家能听到彼此的声音。</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>This shows the flags the player has on the server, if any:&lt;br /&gt;&lt;img src=&quot;skin:authenticated.png&quot; /&gt;Authenticated user&lt;br /&gt;&lt;img src=&quot;skin:muted_self.png&quot; /&gt;Muted (by self)&lt;br /&gt;&lt;img src=&quot;skin:muted_server.png&quot; /&gt;Muted (by admin)&lt;br /&gt;&lt;img src=&quot;skin:deafened_self.png&quot; /&gt;Deafened (by self)&lt;br /&gt;&lt;img src=&quot;skin:deafened_server.png&quot; /&gt;Deafened (by admin)&lt;br /&gt;A player muted by himself is probably just away, talking on the phone or something like that.&lt;br /&gt;A player muted by an admin is probably also just away, and the noise the player is making was annoying enough that an admin muted him.</source>
        <translation>显示该玩家在服务器上的标志:&lt;br /&gt;&lt;img src=&quot;skin:authenticated.png&quot; /&gt;已认证的用户&lt;br /&gt;&lt;img src=&quot;skin:muted_self.png&quot; /&gt;静音(被自己)&lt;br /&gt;&lt;img src=&quot;skin:muted_server.png&quot; /&gt;静音(被管理员)&lt;br /&gt;&lt;img src=&quot;skin:deafened_self.png&quot; /&gt;耳聋 (被自己)&lt;br /&gt;&lt;img src=&quot;skin:deafened_server.png&quot; /&gt;耳聋 (被管理员)&lt;br /&gt;玩家被自己禁音有可能是他处于离开状态，比如在接电话。&lt;br /&gt;玩家被管理员禁音也有可能是他处于离开状态，但是他发出了让管理员恼怒到禁音他的噪音。</translation>
    </message>
    <message>
        <location line="+24"/>
        <source>Name</source>
        <translation>名字</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Flags</source>
        <translation>标志</translation>
    </message>
    <message>
        <location line="+535"/>
        <source>Are you sure you want to drag this channel?</source>
        <translation>你确信要拖动这个频道吗？</translation>
    </message>
</context>
<context>
    <name>PluginConfig</name>
    <message>
        <location filename="Plugins.ui"/>
        <location filename="Plugins.cpp" line="+64"/>
        <source>Plugins</source>
        <translation>插件</translation>
    </message>
    <message>
        <location filename="Plugins.cpp" line="+28"/>
        <source>Plugin has no configure function.</source>
        <translation>插件没有配置选项。</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>Plugin has no about function.</source>
        <translation>插件没有关于选项.</translation>
    </message>
    <message>
        <location filename="Plugins.ui"/>
        <source>Form</source>
        <translation>表单</translation>
    </message>
    <message>
        <location/>
        <source>Options</source>
        <translation>选项</translation>
    </message>
    <message>
        <location/>
        <source>Enable plugins and transmit positional information</source>
        <translation>启用插件并传送位置信息</translation>
    </message>
    <message>
        <location/>
        <source>This enables plugins for supported games to fetch your in-game position and transmit that with each voice packet. This enables other players to hear your voice in-game from the direction your character is in relation to their own.</source>
        <translation>对支持的游戏启用插件以获取你在游戏中的位置并在每个语音包中传送。这将使别的玩家能根据你在游戏中的声音来判断你在游戏中相对于他们自己的方位。</translation>
    </message>
    <message>
        <location/>
        <source>Link to Game and Transmit Position</source>
        <translation>链接到游戏并传送位置</translation>
    </message>
    <message>
        <location/>
        <source>Reloads all plugins</source>
        <translation>重载所有插件</translation>
    </message>
    <message>
        <location/>
        <source>This rescans and reloads plugins. Use this if you just added or changed a plugin to the plugins directory.</source>
        <translation>重新扫描和重载插件。当你添加或改变一个插件目录中的插件时使用该选项。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Reload plugins</source>
        <translation>&amp;重载插件</translation>
    </message>
    <message>
        <location/>
        <source>Information about plugin</source>
        <translation>插件相关信息</translation>
    </message>
    <message>
        <location/>
        <source>This shows a small information message about the plugin.</source>
        <translation>显示插件相关信息。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;About</source>
        <translation>&amp;关于</translation>
    </message>
    <message>
        <location/>
        <source>Show configuration page of plugin</source>
        <translation>显示插件配置页面</translation>
    </message>
    <message>
        <location/>
        <source>This shows the configuration page of the plugin, if any.</source>
        <translation>显示插件配置页面。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Configure</source>
        <translation>&amp;配置</translation>
    </message>
</context>
<context>
    <name>Plugins</name>
    <message>
        <location filename="Plugins.cpp" line="+127"/>
        <source>%1 lost link.</source>
        <translation>%1 的链接丢失。</translation>
    </message>
    <message>
        <location line="+39"/>
        <source>%1 linked.</source>
        <translation>%1 已链接上。</translation>
    </message>
</context>
<context>
    <name>PortAudioSystem</name>
    <message>
        <location filename="PAAudio.cpp" line="+273"/>
        <source>Default Device</source>
        <translation>默认设备</translation>
    </message>
</context>
<context>
    <name>PulseAudioSystem</name>
    <message>
        <location filename="PulseAudio.cpp" line="+545"/>
        <source>Default Input</source>
        <translation>默认输入</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Default Output</source>
        <translation>默认输出</translation>
    </message>
</context>
<context>
    <name>ShortcutActionWidget</name>
    <message>
        <location filename="GlobalShortcut.cpp" line="-143"/>
        <source>Unassigned</source>
        <translation>未分配</translation>
    </message>
</context>
<context>
    <name>ShortcutDelegate</name>
    <message>
        <location line="+58"/>
        <source>Unassigned</source>
        <translation>未分配</translation>
    </message>
</context>
<context>
    <name>ShortcutKeyWidget</name>
    <message>
        <location line="-122"/>
        <source>Press Shortcut</source>
        <translation>输入快捷键</translation>
    </message>
</context>
<context>
    <name>TextMessage</name>
    <message>
        <source>Send raw message</source>
        <translation type="obsolete">发送原始消息</translation>
    </message>
    <message>
        <source>Disables html formating</source>
        <translation type="obsolete">禁用 html 格式化</translation>
    </message>
    <message>
        <location filename="TextMessage.ui"/>
        <source>Message to send</source>
        <translation>要发送的消息</translation>
    </message>
    <message>
        <location/>
        <source>Preview</source>
        <translation>预览</translation>
    </message>
    <message>
        <location/>
        <source>Disables html formatting</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Disable text formatting</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This field reflects how the message will look like in the chat.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Preview&lt;/b&gt;&lt;br /&gt;This field takes the raw string entered above and renders it like it will look like in the log. For example  &amp;lt;b&amp;gt;Test&amp;lt;/b&amp;gt; will display as: &lt;b&gt;Test&lt;b&gt;.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>VersionCheck</name>
    <message>
        <source>Mumble failed to retrieve version information from the SourceForge server.</source>
        <translation type="obsolete">Mumble 无法从 SouceForge 服务器获取版本信息。</translation>
    </message>
    <message>
        <location filename="VersionCheck.cpp" line="+130"/>
        <source>Upgrade Mumble</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>A new version of Mumble has been detected and automatically downloaded. It is recommended that you either upgrade to this version, or downgrade to the latest stable release. Do you want to launch the installer now?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Failed to launch snapshot installer.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Corrupt download of new version detected. Automatically removed.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Downloading new snapshot from %1 to %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+18"/>
        <source>Failed to write new version to disk.</source>
        <oldsource>Failed to write new version to disc.</oldsource>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+17"/>
        <source>Mumble failed to retrieve version information from the central server.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ViewCert</name>
    <message>
        <location filename="ViewCert.cpp" line="+41"/>
        <source>Certificate Chain Details</source>
        <translation>认证链详细信息</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Certificate chain</source>
        <translation>认证链</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>%1 %2</source>
        <translation></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Certificate details</source>
        <translation>认证详细信息</translation>
    </message>
    <message>
        <location line="+28"/>
        <location line="+28"/>
        <source>Common Name: %1</source>
        <translation>通用名: %1</translation>
    </message>
    <message>
        <location line="-27"/>
        <location line="+28"/>
        <source>Organization: %1</source>
        <translation>组织: %1</translation>
    </message>
    <message>
        <location line="-27"/>
        <source>Subunit: %1</source>
        <translation>部门: %1</translation>
    </message>
    <message>
        <location line="+1"/>
        <location line="+28"/>
        <source>Country: %1</source>
        <translation>国家: %1</translation>
    </message>
    <message>
        <location line="-27"/>
        <location line="+28"/>
        <source>Locality: %1</source>
        <translation>位置: %1</translation>
    </message>
    <message>
        <location line="-27"/>
        <location line="+28"/>
        <source>State: %1</source>
        <translation>省(市): %1</translation>
    </message>
    <message>
        <location line="-27"/>
        <source>Valid from: %1</source>
        <translation>有效期始于: %1</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Valid to: %1</source>
        <translation>有效期到: %1</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Serial: %1</source>
        <translation>序列号: %1</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Public Key: %1 bits %2</source>
        <translation>公开密钥: %1 比特 %2</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>RSA</source>
        <translation>非对称加密算法</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>DSA</source>
        <translation>数字签名算法</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Digest (MD5): %1</source>
        <translation></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Email: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>DNS: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Issued by:</source>
        <translation>发布者:</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Unit Name: %1</source>
        <translation>部门名: %1</translation>
    </message>
</context>
<context>
    <name>WASAPISystem</name>
    <message>
        <location filename="WASAPI.cpp" line="+180"/>
        <source>Default Device</source>
        <translation>默认设备</translation>
    </message>
</context>
</TS>
