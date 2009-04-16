<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.0" language="zh_CN">
<context>
    <name>ACLEditor</name>
    <message>
        <location filename="ACLEditor.cpp" line="47"/>
        <source>Mumble - Edit ACL for %1</source>
        <translation type="unfinished">Mumble - 编辑 访问控制列表 %1</translation>
    </message>
    <message>
        <location filename="ACLEditor.cpp" line="51"/>
        <source>Deny</source>
        <translation type="unfinished">禁止</translation>
    </message>
    <message>
        <location filename="ACLEditor.cpp" line="53"/>
        <source>Allow</source>
        <translation type="unfinished">允许</translation>
    </message>
    <message>
        <location filename="ACLEditor.cpp" line="155"/>
        <source>Allow %1</source>
        <translation type="unfinished">允许 %1</translation>
    </message>
    <message>
        <location filename="ACLEditor.cpp" line="156"/>
        <source>Deny %1</source>
        <translation type="unfinished">禁止 %1</translation>
    </message>
    <message>
        <location filename="ACLEditor.cpp" line="157"/>
        <source>This grants the %1 privilege. If a privilege is both allowed and denied, it is denied.&lt;br /&gt;%2</source>
        <translation type="unfinished">给予 %1 权限.如果一个权限同时被禁止和允许，该权限被视为禁止。&lt;br /&gt;%2</translation>
    </message>
    <message>
        <location filename="ACLEditor.cpp" line="158"/>
        <source>This revokes the %1 privilege. If a privilege is both allowed and denied, it is denied.&lt;br /&gt;%2</source>
        <translation type="unfinished">撤销 %1 权限.如果一个权限同时被禁止和允许，该权限被视为禁止。&lt;br /&gt;%2</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="13"/>
        <source>Dialog</source>
        <translation type="unfinished">会话</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="23"/>
        <source>&amp;Groups</source>
        <translation type="unfinished">&amp;群组</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="29"/>
        <location filename="ACLEditor.ui" line="363"/>
        <source>Group</source>
        <translation type="unfinished">群</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="35"/>
        <source>List of groups</source>
        <translation type="unfinished">群组列表</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="38"/>
        <source>This is all the groups currently defined for the channel. To create a new group, just type in the name and press enter.</source>
        <translation type="unfinished">当前频道内所有群组。要创建一个新群，请输入群名软后回车。</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="48"/>
        <source>Remove selected group</source>
        <translation type="unfinished">移除所选群</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="51"/>
        <source>This removes the currently selected group. If the group was inherited, it will not be removed from the list, but all local information about the group will be cleared.</source>
        <translation type="unfinished">移除当前所选群。如果该群有子群，将清楚该群内所有信息。</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="54"/>
        <location filename="ACLEditor.ui" line="125"/>
        <location filename="ACLEditor.ui" line="161"/>
        <location filename="ACLEditor.ui" line="168"/>
        <source>Remove</source>
        <translation type="unfinished">移除</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="64"/>
        <source>Inherit group members from parent</source>
        <translation type="unfinished">子群成员</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="67"/>
        <source>This inherits all the members in the group from the parent, if the group is marked as &lt;i&gt;Inheritable&lt;/i&gt; in the parent channel.</source>
        <translation type="unfinished">如果群在频道里被标记为&lt;i&gt;可继承&lt;/i&gt;,则该群将继承频道内的所有成员。</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="70"/>
        <location filename="ACLEditor.ui" line="135"/>
        <source>Inherit</source>
        <translation type="unfinished">继承</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="77"/>
        <source>Make group inheritable to sub-channels</source>
        <translation type="unfinished">使群可被子频道继承</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="80"/>
        <source>This makes this group inheritable to sub-channels. If the group is non-inheritable, sub-channels are still free to create a new group with the same name.</source>
        <translation type="unfinished">使该群可被子频道继承。如果该群不可被子频道继承，则可在子频道内创建同名群。</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="83"/>
        <source>Inheritable</source>
        <translation type="unfinished">可继承</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="93"/>
        <source>Group was inherited from parent channel</source>
        <translation type="unfinished">从父频道继承来的群</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="96"/>
        <source>This indicates that the group was inherited from the parent channel. You cannot edit this flag, it&apos;s just for information.</source>
        <translation type="unfinished">该标志表明该群继承自父频道.该标志仅提供信息,你不能编辑该标志。</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="99"/>
        <source>Inherited</source>
        <translation type="unfinished">继承的</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="109"/>
        <source>Members</source>
        <translation type="unfinished">成员</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="115"/>
        <location filename="ACLEditor.ui" line="195"/>
        <location filename="ACLEditor.ui" line="202"/>
        <source>Add</source>
        <translation type="unfinished">添加</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="154"/>
        <source>Add to Remove</source>
        <translation type="unfinished">添加删除</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="175"/>
        <source>Add member to group</source>
        <translation type="unfinished">往组内添加成员</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="178"/>
        <source>Type in the name of a player you wish to add to the group and click Add.</source>
        <translation type="unfinished">输入你想要添加到组内的玩家名然后点击添加.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="185"/>
        <source>Remove member from group</source>
        <translation type="unfinished">从群内移除成员</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="188"/>
        <source>Type in the name of a player you wish to remove from the group and click Add.</source>
        <translation type="unfinished">输入你想要从群内移除的成员然后点击添加。</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="213"/>
        <source>&amp;ACL</source>
        <translation type="unfinished">&amp;访问控制列表</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="219"/>
        <source>Active ACLs</source>
        <translation type="unfinished">当前起作用的访问控制列表</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="225"/>
        <source>List of entries</source>
        <translation type="unfinished">条目列表</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="228"/>
        <source>This shows all the entries active on this channel. Entries inherited from parent channels will be show in italics.&lt;br /&gt;ACLs are evaluated top to bottom, meaning priority increases as you move down the list.</source>
        <translation type="unfinished">显示当前频道内所有条目。从父频道继承的条目将以斜体显示.&lt;br/&gt;访问控制列表将被由上至下处理，这意味着越往下优先级越高.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="235"/>
        <source>Inherit ACL of parent?</source>
        <translation type="unfinished">继承上级访问控制列表?</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="238"/>
        <source>This sets whether or not the ACL up the chain of parent channels are applied to this object. Only those entries that are marked in the parent as &quot;Apply to sub-channels&quot; will be inherited.</source>
        <translation type="unfinished">设置父频道的访问控制列表是否对该对象可用。只有那些在父频道内被标记为“对子频道有效”的条目将被继承。</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="241"/>
        <source>Inherit ACLs</source>
        <translation type="unfinished">继承访问控制列表</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="248"/>
        <source>Move entry up</source>
        <translation type="unfinished">上移条目</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="251"/>
        <source>This moves the entry up in the list. As entries are evaluated in order, this may change the effective permissions of users. You cannot move an entry above an inherited entry, if you really need that you&apos;ll have to duplicate the inherited entry.</source>
        <translation type="unfinished">将条目上移.由于条目由上至下被处理，这将改变用户的有效权限.你不能将条目移到继承的条目之上，如果你确实需要，你将不得不复制继承的条目。</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="254"/>
        <source>&amp;Up</source>
        <translation type="unfinished">&amp;上</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="264"/>
        <source>Move entry down</source>
        <translation type="unfinished">下移条目</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="267"/>
        <source>This moves the entry down in the list. As entries are evaluated in order, this may change the effective permissions of users.</source>
        <translation type="unfinished">将条目在列表中下移。由于条目由上至下被处理，这将改变用户有效权限.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="270"/>
        <source>&amp;Down</source>
        <translation type="unfinished">&amp;下</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="280"/>
        <source>Add new entry</source>
        <translation type="unfinished">添加新条目</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="283"/>
        <source>This adds a new entry, initially set with no permissions and applying to all.</source>
        <translation type="unfinished">添加一个新的条目，初始设置为无权限且对所有可用。</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="286"/>
        <source>&amp;Add</source>
        <translation type="unfinished">&amp;添加</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="296"/>
        <source>Remove entry</source>
        <translation type="unfinished">移除条目</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="299"/>
        <source>This removes the currently selected entry.</source>
        <translation type="unfinished">移除所选条目。</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="302"/>
        <source>&amp;Remove</source>
        <translation type="unfinished">&amp;移除</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="315"/>
        <source>Context</source>
        <translation type="unfinished">背景</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="321"/>
        <source>Entry should apply to this channel</source>
        <translation type="unfinished">将用于该频道的条目</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="324"/>
        <source>This makes the entry apply to this channel.</source>
        <translation type="unfinished">对当前频道应用该条目。</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="327"/>
        <source>Applies to this channel</source>
        <translation type="unfinished">应用于该频道</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="334"/>
        <source>Entry should apply to sub-channels.</source>
        <translation type="unfinished">对子频道可用条目。</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="337"/>
        <source>This makes the entry apply to sub-channels of this channel.</source>
        <translation type="unfinished">使该条目对当前频道的子频道可用。</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="340"/>
        <source>Applies to sub-channels</source>
        <translation type="unfinished">应用于子频道</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="350"/>
        <source>Permissions</source>
        <translation type="unfinished">权限</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="357"/>
        <source>User/Group</source>
        <translation type="unfinished">用户/群</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="373"/>
        <source>Group this entry applies to</source>
        <translation type="unfinished">将条目应用于群</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="376"/>
        <source>This controls which group of users this entry applies to.&lt;br /&gt;Note that the group is evaluated in the context of the channel the entry is used in. For example, the default ACL on the Root channel gives &lt;i&gt;Write&lt;/i&gt; permission to the &lt;i&gt;admin&lt;/i&gt; group. This entry, if inherited by a channel, will give a user write privileges if he belongs to the &lt;i&gt;admin&lt;/i&gt; group in that channel, even if he doesn&apos;t belong to the &lt;i&gt;admin&lt;/i&gt; group in the channel where the ACL originated.&lt;br /&gt;If a group name starts with a !, its membership is negated, and if it starts with a ~, it is evaluated in the channel the ACL was defined in, rather than the channel the ACL is active in. Order is important; &lt;i&gt;!~in&lt;/i&gt; is valid, but &lt;i&gt;~!in&lt;/i&gt; is not.&lt;br /&gt;A few special predefined groups are:&lt;br /&gt;&lt;b&gt;all&lt;/b&gt; - Everyone will match.&lt;br /&gt;&lt;b&gt;auth&lt;/b&gt; - All authenticated users will match.&lt;br /&gt;&lt;b&gt;sub,a,b,c&lt;/b&gt; - User currently in a sub-channel minimum &lt;i&gt;a&lt;/i&gt; common parents, and between &lt;i&gt;b&lt;/i&gt; and &lt;i&gt;c&lt;/i&gt; channels down the chain. See the website for more extensive documentation on this one.&lt;br /&gt;&lt;b&gt;in&lt;/b&gt; - Users currently in the channel will match (convenience for &lt;i&gt;sub,0,0,0&lt;/i&gt;).&lt;br /&gt;&lt;b&gt;out&lt;/b&gt; - Users outside the channel will match (convenience for &lt;i&gt;!sub,0,0,0&lt;/i&gt;).&lt;br /&gt;Note that an entry applies to either a user or a group, not both.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="386"/>
        <source>User ID</source>
        <translation type="unfinished">用户名</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="396"/>
        <source>User this entry applies to</source>
        <translation type="unfinished">适用于该规则的用户</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="399"/>
        <source>This controls which user this entry applies to. Just type in the user name and hit enter to query the server for a match.</source>
        <translation type="unfinished">控制该规则对哪些用户可用。请如数用户名并输入回车来像服务器查询。</translation>
    </message>
</context>
<context>
    <name>ALSAAudioInput</name>
    <message>
        <location filename="ALSAAudio.cpp" line="172"/>
        <source>Default ALSA Card</source>
        <translation type="unfinished">默认ALSA声卡</translation>
    </message>
    <message>
        <location filename="ALSAAudio.cpp" line="364"/>
        <source>Opening chosen ALSA Input failed: %1</source>
        <translation type="unfinished">不能使用所选的ALSA输入:%1</translation>
    </message>
</context>
<context>
    <name>ALSAAudioOutput</name>
    <message>
        <location filename="ALSAAudio.cpp" line="173"/>
        <source>Default ALSA Card</source>
        <translation type="unfinished">默认ALSA声卡</translation>
    </message>
    <message>
        <location filename="ALSAAudio.cpp" line="484"/>
        <source>Opening chosen ALSA Output failed: %1</source>
        <translation>不能使用所选的ALSA输出:%1</translation>
    </message>
</context>
<context>
    <name>ASIOConfig</name>
    <message>
        <location filename="ASIOInput.cpp" line="268"/>
        <source>%1 (ver %2)</source>
        <translation type="unfinished">%1 (版本 %2)</translation>
    </message>
    <message>
        <location filename="ASIOInput.cpp" line="272"/>
        <source>%1 ms -&gt; %2 ms (%3 ms resolution) %4Hz</source>
        <translation type="unfinished">%1 毫秒 -&gt; %2 毫秒 (%3 毫秒 解析度) %4赫兹</translation>
    </message>
    <message>
        <location filename="ASIOInput.cpp" line="274"/>
        <source>%1 ms -&gt; %2 ms (%3 ms resolution) %4Hz -- Unusable</source>
        <translation type="unfinished">%1 毫秒 -&gt; %2 毫秒 (%3 毫秒 解析度) %4赫兹--不可用</translation>
    </message>
    <message>
        <location filename="ASIOInput.cpp" line="317"/>
        <location filename="ASIOInput.cpp" line="321"/>
        <location filename="ASIOInput.cpp" line="344"/>
        <location filename="ASIOInput.cpp" line="348"/>
        <source>Mumble</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ASIOInput.cpp" line="317"/>
        <location filename="ASIOInput.cpp" line="344"/>
        <source>ASIO Initialization failed: %1</source>
        <translation type="unfinished">ASIO初始化失败: %1</translation>
    </message>
    <message>
        <location filename="ASIOInput.cpp" line="321"/>
        <location filename="ASIOInput.cpp" line="348"/>
        <source>Failed to instantiate ASIO driver</source>
        <translation type="unfinished">初始化ASIO驱动失败</translation>
    </message>
    <message>
        <location filename="ASIOInput.cpp" line="385"/>
        <source>ASIO</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="13"/>
        <source>Form</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="19"/>
        <source>Device selection</source>
        <translation type="unfinished">选择设备</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="25"/>
        <source>Device</source>
        <translation type="unfinished">设备</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="38"/>
        <source>Device to use for microphone</source>
        <translation type="unfinished">麦克风设备</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="41"/>
        <source>This chooses what device to query. You still need to actually query the device and select which channels to use.</source>
        <translation type="unfinished">选择要查询的设备。你仍然需要查询所选设备并选择使用所选设备的频道.</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="48"/>
        <source>Query selected device</source>
        <translation type="unfinished">查询所选设备</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="51"/>
        <source>This queries the selected device for channels. Be aware that many ASIO drivers are buggy to the extreme, and querying them might cause a crash of either the application or the system.</source>
        <translation type="unfinished">查询频道所选设备。注意很多ASIO驱动有漏洞，查询设备可能导致应用程序或系统崩溃.</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="54"/>
        <source>&amp;Query</source>
        <translation type="unfinished">&amp;查询</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="61"/>
        <source>Configure selected device</source>
        <translation type="unfinished">配置所选设备</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="64"/>
        <source>This configures the selected device. Be aware that many ASIO drivers are buggy to the extreme, and querying them might cause a crash of either the application or the system.</source>
        <translation type="unfinished">配置所选设备。注意很多ASIO驱动有漏洞，查询设备可能导致应用程序或系统崩溃.</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="67"/>
        <source>&amp;Configure</source>
        <translation type="unfinished">&amp;配置</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="77"/>
        <source>Capabilities</source>
        <translation type="unfinished">设置信息</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="83"/>
        <source>Driver name</source>
        <translation type="unfinished">驱动名</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="96"/>
        <location filename="ASIOInput.ui" line="116"/>
        <source>TextLabel</source>
        <translation type="unfinished">标签</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="103"/>
        <source>Buffersize</source>
        <translation type="unfinished">缓冲区大小 </translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="126"/>
        <source>Configure input channels</source>
        <translation type="unfinished">配置输入频道</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="129"/>
        <source>This will configure the input channels for ASIO. Make sure you select at least one channel as microphone and speaker. &lt;i&gt;Microphone&lt;/i&gt; should be where your microphone is attached, and &lt;i&gt;Speaker&lt;/i&gt; should be a channel that samples &lt;i&gt;What you hear&lt;/i&gt;.&lt;br /&gt;For example, on the Audigy 2 ZS, a good selection for Microphone would be &lt;i&gt;Mic L&lt;/i&gt; while Speaker should be &lt;i&gt;Mix L&lt;/i&gt; and &lt;i&gt;Mix R&lt;/i&gt;.</source>
        <translation type="unfinished">配置ASIO的输入频道.请确保你选择了至少一个频道作为麦克风和扬声器。&lt;i&gt;麦克风&lt;/i&gt;应是你当前连接的麦克风，&lt;i&gt;扬声器&lt;/i&gt;应该是一个采样&lt;i&gt;你听到的东西&lt;/i&gt;的频道。&lt;br /&gt;例如在Audigy 2 ZS，好的麦克风的选择应当是&lt;i&gt;Mic L&lt;/i&gt;扬声器则是&lt;i&gt;Mix L&lt;/i&gt; and &lt;i&gt;Mix R&lt;/i&gt;.</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="132"/>
        <source>Channels</source>
        <translation type="unfinished">频道</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="153"/>
        <source>Microphone</source>
        <translation type="unfinished">麦克风</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="199"/>
        <location filename="ASIOInput.ui" line="295"/>
        <source>&lt;-</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="212"/>
        <location filename="ASIOInput.ui" line="282"/>
        <source>-&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="236"/>
        <source>Unused</source>
        <translation type="unfinished">未用的</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="319"/>
        <source>Speakers</source>
        <translation type="unfinished">扬声器</translation>
    </message>
</context>
<context>
    <name>ASIOInput</name>
    <message>
        <location filename="ASIOInput.cpp" line="475"/>
        <location filename="ASIOInput.cpp" line="543"/>
        <source>Mumble</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ASIOInput.cpp" line="475"/>
        <source>You need to select at least one microphone and one speaker source to use ASIO. If you just need microphone sampling, use DirectSound.</source>
        <translation type="unfinished">要用ASIO你应当选择至少一个麦克风和一个扬声器.如果你只需要麦克风采样,请用DirectSound.</translation>
    </message>
    <message>
        <location filename="ASIOInput.cpp" line="543"/>
        <source>Opening selected ASIO device failed. No input will be done.</source>
        <translation type="unfinished">打开所选ASIO设备是被。无法接受输入。</translation>
    </message>
</context>
<context>
    <name>AboutDialog</name>
    <message>
        <location filename="About.cpp" line="37"/>
        <source>About Mumble</source>
        <translation type="unfinished">关于 Mumble</translation>
    </message>
    <message>
        <location filename="About.cpp" line="53"/>
        <source>&lt;h3&gt;Mumble (%1)&lt;/h3&gt;&lt;p&gt;Copyright %3 Thorvald Natvig&lt;br /&gt;slicer@users.sourceforge.net&lt;/p&gt;&lt;p&gt;&lt;b&gt;A voice-chat utility for gamers&lt;/b&gt;&lt;/p&gt;&lt;p&gt;&lt;tt&gt;&lt;a href=&quot;%2&quot;&gt;%2&lt;/a&gt;&lt;/tt&gt;&lt;/p&gt;</source>
        <translation type="unfinished">&lt;h3&gt;Mumble (%1)&lt;/h3&gt;&lt;p&gt;版权所有 %3 Thorvald Natvig&lt;br /&gt;slicer@users.sourceforge.net&lt;/p&gt;&lt;p&gt;&lt;b&gt;一个游戏玩家的语音聊天工具&lt;/b&gt;&lt;/p&gt;&lt;p&gt;&lt;tt&gt;&lt;a href=&quot;%2&quot;&gt;%2&lt;/a&gt;&lt;/tt&gt;&lt;/p&gt;</translation>
    </message>
    <message>
        <location filename="About.cpp" line="63"/>
        <source>&amp;About Mumble</source>
        <translation type="unfinished">&amp;关于Mumble</translation>
    </message>
    <message>
        <location filename="About.cpp" line="64"/>
        <source>&amp;License</source>
        <translation type="unfinished">&amp;版权</translation>
    </message>
    <message>
        <location filename="About.cpp" line="66"/>
        <source>OK</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>AboutSpeexDialog</name>
    <message>
        <location filename="About.cpp" line="77"/>
        <source>About Speex</source>
        <translation type="unfinished">关于 Speex</translation>
    </message>
    <message>
        <location filename="About.cpp" line="81"/>
        <source>&lt;h3&gt;About Speex&lt;/h3&gt;&lt;p&gt;&lt;tt&gt;&lt;a href=&quot;%2&quot;&gt;%2&lt;/a&gt;&lt;/tt&gt;&lt;/p&gt;&lt;p&gt;This program uses Speex version %1&lt;/p&gt;&lt;p&gt;Speex is used for echo cancellation, noise&lt;br /&gt;filtering, voice activity detection and speech&lt;br /&gt;compression.&lt;/p&gt;</source>
        <translation type="unfinished">&lt;h3&gt;关于 Speex&lt;/h3&gt;&lt;p&gt;&lt;tt&gt;&lt;a href=&quot;%2&quot;&gt;%2&lt;/a&gt;&lt;/tt&gt;&lt;/p&gt;&lt;p&gt;该程序依赖于 Speex 版本 %1&lt;/p&gt;&lt;p&gt;Speex 被用于回声消除和降噪&lt;br /&gt;过滤,语音活动检测和语音&lt;br /&gt;压缩.&lt;/p&gt;</translation>
    </message>
    <message>
        <location filename="About.cpp" line="90"/>
        <source>OK</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>AudioInput</name>
    <message>
        <location filename="AudioInput.ui" line="13"/>
        <source>Form</source>
        <translation type="unfinished">表格</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="19"/>
        <source>Interface</source>
        <translation type="unfinished">接口</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="25"/>
        <source>System</source>
        <translation type="unfinished">系统</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="41"/>
        <source>Input method for audio</source>
        <translation type="unfinished">语音输入方法</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="44"/>
        <source>&lt;b&gt;This is the input method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation type="unfinished">&lt;b&gt;这是你想要用于语音输入的方法.&lt;/b&gt;&lt;br /&gt;很有可能你想要用DirectSound.</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="67"/>
        <source>Device</source>
        <translation type="unfinished">设备</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="83"/>
        <source>Output method for audio</source>
        <translation type="unfinished">语音输出方法</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="86"/>
        <source>&lt;b&gt;This is the output method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation type="unfinished">&lt;b&gt;这是你想要用于语音输出的方法.&lt;/b&gt;&lt;br /&gt;很有可能你想要用DirectSound.</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="99"/>
        <source>Cancel Echo</source>
        <translation type="unfinished">消除回音</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="115"/>
        <source>Transmission</source>
        <translation type="unfinished">传输</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="121"/>
        <source>&amp;Transmit</source>
        <translation type="unfinished">&amp;传输</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="131"/>
        <source>When to transmit your speech</source>
        <translation type="unfinished">何时传输你的语音</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="134"/>
        <source>&lt;b&gt;This sets when speech should be transmitted.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;Continuous&lt;/i&gt; - All the time&lt;br /&gt;&lt;i&gt;Voice Activity&lt;/i&gt; - When you are speaking clearly.&lt;br /&gt;&lt;i&gt;Push To Talk&lt;/i&gt; - When you hold down the hotkey set under &lt;i&gt;Shortcuts&lt;/i&gt;.</source>
        <translation type="unfinished">&lt;b&gt;设置何时你的语音将被传输.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;连续&lt;/i&gt; 一直&lt;br /&gt;&lt;i&gt;语音活动检测&lt;/i&gt; 当你清晰发音时&lt;br /&gt;&lt;i&gt;按键发音&lt;/i&gt;快捷键组合&lt;i&gt;捷径&lt;/i&gt;.</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="148"/>
        <source>DoublePush Time</source>
        <translation type="unfinished">按两次的时间间隔</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="174"/>
        <location filename="AudioInput.ui" line="263"/>
        <location filename="AudioInput.ui" line="387"/>
        <location filename="AudioInput.ui" line="444"/>
        <location filename="AudioInput.ui" line="489"/>
        <location filename="AudioInput.ui" line="507"/>
        <location filename="AudioInput.ui" line="564"/>
        <location filename="AudioInput.ui" line="609"/>
        <source>TextLabel</source>
        <translation type="unfinished">标签</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="184"/>
        <source>&lt;b&gt;This enables the audio cues for push to talk.&lt;/b&gt;&lt;br /&gt;Setting this will give you a short audio beep when push to talk is pressed and released.</source>
        <translation type="unfinished">&lt;b&gt;启用按键发音提示.&lt;/b&gt;&lt;br /&gt;当你按住或放掉案件发音键时会收到语音提示.</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="187"/>
        <source>PTT Audio cue</source>
        <translation type="unfinished">PTT按键发音提示</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="200"/>
        <source>Use SNR based speech detection</source>
        <translation type="unfinished">使用基于SNR的语音检测</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="206"/>
        <source>Signal to Noise</source>
        <translation type="unfinished">噪音信号</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="213"/>
        <source>Use Amplitude based speech detection.</source>
        <translation type="unfinished">使用基于震幅的语音检测.</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="216"/>
        <source>&lt;b&gt;This sets speech detection to use Amplitude.&lt;/b&gt;&lt;br /&gt;In this mode, the raw strength of the input signal is used to detect speech.</source>
        <translation type="unfinished">&lt;b&gt;用震幅来进行语音检测&lt;/b&gt;&lt;br /&gt;在这个模式下，输入信号强度将被用于语音检测。</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="219"/>
        <source>Amplitude</source>
        <translation type="unfinished">震幅</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="228"/>
        <source>Voice &amp;Hold</source>
        <translation type="unfinished">语音 &amp;保存</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="238"/>
        <source>How long to keep transmitting after silence</source>
        <translation type="unfinished">禁音后持续传输多久</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="241"/>
        <source>&lt;b&gt;This selects how long after a perceived stop in speech transmission should continue.&lt;/b&gt;&lt;br /&gt;Set this higher if your voice breaks up when you speak (seen by a rapidly blinking voice icon next to your name).&lt;br /&gt;Only makes sense when used with Voice Activity transmission.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="286"/>
        <source>Silence Below</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="293"/>
        <source>Signal values below this counts as silence</source>
        <translation type="unfinished">低于该值的信号被视为无声</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="296"/>
        <location filename="AudioInput.ui" line="328"/>
        <source>&lt;b&gt;This sets the trigger values for voice detection.&lt;/b&gt;&lt;br /&gt;Use this together with the Audio Statistics window to manually tune the trigger values for detecting speech. Input values below &quot;Silence Below&quot; always count as silence. Values above &quot;Speech Above&quot; always count as voice. Values in between will count as voice if you&apos;re already talking, but will not trigger a new detection.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="318"/>
        <source>Speech Above</source>
        <translation type="unfinished">音调高于</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="325"/>
        <source>Signal values above this count as voice</source>
        <translation type="unfinished">高于该值的信号被视为声音</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="397"/>
        <source>Compression</source>
        <translation type="unfinished">压缩</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="403"/>
        <source>&amp;Quality</source>
        <translation type="unfinished">&amp;质量</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="413"/>
        <source>Quality of compression (peak bandwidth)</source>
        <translation type="unfinished">压缩质量(贷款峰值）</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="416"/>
        <source>&lt;b&gt;This sets the quality of compression.&lt;/b&gt;&lt;br /&gt;This determines how much bandwidth Mumble is allowed to use for outgoing audio.</source>
        <translation type="unfinished">&lt;b&gt;设置压缩质量.&lt;/b&gt;&lt;br /&gt;该设置决定Mumble用于外出音频的带宽.</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="451"/>
        <source>Audio per packet</source>
        <translation type="unfinished">每个包的语音</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="461"/>
        <source>How many audio frames to send per packet</source>
        <translation type="unfinished">每个包里送出多少个音频帧</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="464"/>
        <source>&lt;b&gt;This selects how many audio frames should be put in one packet.&lt;/b&gt;&lt;br /&gt;Increasing this will increase the latency of your voice, but will also reduce bandwidth requirements.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="504"/>
        <source>&lt;b&gt;This shows peak outgoing bandwidth used.&lt;/b&gt;&lt;br /&gt;This shows the peak amount of bandwidth sent out from your machine. Audio bitrate is the maximum bitrate (as we use VBR) for the audio data alone. Position is the bitrate used for positional information. Overhead is our framing and the IP packet headers (IP and UDP is 75% of this overhead).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="520"/>
        <source>Audio Processing</source>
        <translation type="unfinished">语音处理</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="526"/>
        <source>Noise Suppression</source>
        <translation type="unfinished">降噪</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="536"/>
        <source>Noise suppression</source>
        <translation type="unfinished">降噪</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="571"/>
        <source>Amplification</source>
        <translation type="unfinished">扩音</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="581"/>
        <source>Maximum amplification of input sound</source>
        <translation type="unfinished">输入声音最大放大比</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="584"/>
        <source>&lt;b&gt;Maximum amplification of input.&lt;/b&gt;&lt;br /&gt;Mumble normalizes the input volume before compressing, and this sets how much it&apos;s allowed to amplify.&lt;br /&gt;The actual level is continually updated based on your current speech pattern, but it will never go above the level specified here.&lt;br /&gt;If the &lt;i&gt;Microphone loudness&lt;/i&gt; level of the audio statistics hover around 100%, you probably want to set this to 2.0 or so, but if, like most people, you are unable to reach 100%, set this to something much higher.&lt;br /&gt;Ideally, set it so &lt;i&gt;Microphone Loudness * Amplification Factor &gt;= 100&lt;/i&gt;, even when you&apos;re speaking really soft.&lt;br /&gt;&lt;br /&gt;Note that there is no harm in setting this to maximum, but Mumble will start picking up other conversations if you leave it to auto-tune to that level.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="276"/>
        <source>Current speech detection chance</source>
        <translation type="unfinished">当前语音检测几率</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="181"/>
        <source>Audible audio cue when push-to-talk is activated or deactivated</source>
        <translation type="unfinished">当按键发音被激活或不激活时听到的提示音</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="203"/>
        <source>&lt;b&gt;This sets speech detection to use Signal to Noise ratio.&lt;/b&gt;&lt;br /&gt;In this mode, the input is analyzed for something resembling a clear signal, and the clarity of that signal is used to trigger speech detection.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="279"/>
        <source>&lt;b&gt;This shows the current speech detection settings.&lt;/b&gt;&lt;br /&gt;You can change the settings from the Settings dialog or from the Audio Wizard.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="355"/>
        <source>Idle AutoMute</source>
        <translation type="unfinished">空闲时自动静音</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="362"/>
        <source>How long to remain idle before auto-mute.</source>
        <translation type="unfinished">多长时间后自动静音.</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="365"/>
        <source>This sets an idle timer. If the timer expires without any audio being sent to the server, you are muted.</source>
        <translation type="unfinished">设置空闲时间。如果在规定时间内没有任何音频送往服务器，用户将被静音.</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="501"/>
        <source>Maximum bandwidth used for sending audio</source>
        <translation type="unfinished">用于发送音频的最大带宽</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="539"/>
        <source>&lt;b&gt;This sets the amount of noise suppression to apply.&lt;/b&gt;&lt;br /&gt;The higher this value, the more aggressively stationary noise will be supressed.</source>
        <translation type="unfinished">&lt;b&gt;设置噪音抑制水平.&lt;/b&gt;&lt;br /&gt;数值越高，固定噪音会被越积极的抑制.</translation>
    </message>
</context>
<context>
    <name>AudioInputDialog</name>
    <message>
        <location filename="AudioConfigDialog.cpp" line="74"/>
        <source>Continuous</source>
        <translation type="unfinished">持续</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="75"/>
        <source>Voice Activity</source>
        <translation type="unfinished">语音活动</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="76"/>
        <source>Push To Talk</source>
        <translation type="unfinished">按键发音</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="88"/>
        <source>Audio Input</source>
        <translation type="unfinished">语音输入</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="167"/>
        <location filename="AudioConfigDialog.cpp" line="175"/>
        <source>%1 ms</source>
        <translation type="unfinished">%1 毫秒</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="173"/>
        <location filename="AudioConfigDialog.cpp" line="203"/>
        <source>Off</source>
        <translation type="unfinished">关闭</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="181"/>
        <source>%1 s</source>
        <translation type="unfinished">%1 秒</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="190"/>
        <source>-%1 dB</source>
        <translation type="unfinished">-%1 分贝</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="248"/>
        <source>%1kbit/s (Audio %2, Position %4, Overhead %3)</source>
        <translation type="unfinished">%1千比特/秒 (语音 %2, 位置 %4, 开销 %3)</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="201"/>
        <source>%1 min</source>
        <translation type="unfinished">%1 分钟</translation>
    </message>
</context>
<context>
    <name>AudioOutput</name>
    <message>
        <location filename="AudioOutput.ui" line="13"/>
        <source>Form</source>
        <translation type="unfinished">表单</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="19"/>
        <source>Interface</source>
        <translation type="unfinished">接口</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="25"/>
        <source>System</source>
        <translation type="unfinished">系统</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="41"/>
        <source>Input method for audio</source>
        <translation type="unfinished">语音输入放法</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="44"/>
        <source>&lt;b&gt;This is the input method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation type="unfinished">&lt;b&gt;语音输入方式.&lt;/b&gt;&lt;br /&gt;很可能你想用 DirectSound.</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="67"/>
        <source>Device</source>
        <translation type="unfinished">设备</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="83"/>
        <source>Output method for audio</source>
        <translation type="unfinished">语音输出方法</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="86"/>
        <source>&lt;b&gt;This is the output method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation type="unfinished">&lt;b&gt;语音输出方法.&lt;/b&gt;&lt;br /&gt;很可能你想用 DirectSound.</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="99"/>
        <location filename="AudioOutput.ui" line="280"/>
        <source>Positional Audio</source>
        <translation type="unfinished">声音定位</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="109"/>
        <source>Audio Output</source>
        <translation type="unfinished">声音输出</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="115"/>
        <source>Default &amp;Jitter Buffer</source>
        <translation type="unfinished">默认 &amp;Jitter 缓冲区</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="125"/>
        <source>Safety margin for jitter buffer</source>
        <translation type="unfinished">Jitter缓冲区安全界限</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="128"/>
        <source>&lt;b&gt;This sets the minimum safety margin for the jitter buffer.&lt;/b&gt;&lt;br /&gt;All incoming audio is buffered, and the jitter buffer continually tries to push the buffer to the minimum sustainable by your network, so latency can be as low as possible. This sets the minimum buffer size to use. If the start of sentences you hear is very jittery, increase this value.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="153"/>
        <location filename="AudioOutput.ui" line="192"/>
        <location filename="AudioOutput.ui" line="231"/>
        <location filename="AudioOutput.ui" line="238"/>
        <location filename="AudioOutput.ui" line="337"/>
        <location filename="AudioOutput.ui" line="379"/>
        <location filename="AudioOutput.ui" line="418"/>
        <location filename="AudioOutput.ui" line="441"/>
        <location filename="AudioOutput.ui" line="508"/>
        <location filename="AudioOutput.ui" line="553"/>
        <source>TextLabel</source>
        <translation type="unfinished">标签</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="160"/>
        <source>Volume</source>
        <translation type="unfinished">音量</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="170"/>
        <source>Volume of incoming speech</source>
        <translation type="unfinished">进入语音音量</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="173"/>
        <source>&lt;b&gt;This adjusts the volume of incoming speech.&lt;/b&gt;&lt;br /&gt;Note that if you increase this beyond 100%, audio will be distorted.</source>
        <translation type="unfinished">&lt;b&gt;调整接收语音音量.&lt;/b&gt;&lt;br /&gt;注意如果你将此值调高于 100%, 声音将被扭曲.</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="199"/>
        <source>Output Delay</source>
        <translation type="unfinished">输出延迟</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="209"/>
        <source>Amount of data to buffer</source>
        <translation type="unfinished">缓冲区大小</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="212"/>
        <source>This sets the amount of data to prebuffer in the output buffer. Experiment with different values and set it to the lowest which doesn&apos;t cause rapid jitter in the sound.</source>
        <translation type="unfinished">设置输出缓冲区大小。尝试不同值然后设成能不导致过度颤音的最低值.</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="286"/>
        <source>The connected &quot;speakers&quot; are actually headphones.</source>
        <translation type="unfinished">连接的“扬声器”实际是耳机.</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="315"/>
        <source>This sets the minimum distance for sound calculations. The volume of other players&apos; speech will not decrease until they are at least this far away from you.</source>
        <translation type="unfinished">设置声强计算的最小距离.当别的玩家离你不超过此距离时他的音量将不会减弱.</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="396"/>
        <source>Factor for sound volume decrease</source>
        <translation type="unfinished">音量减弱因子</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="425"/>
        <source>Bloom</source>
        <translation type="unfinished">最佳时期</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="448"/>
        <source>Factor for sound volume increase</source>
        <translation type="unfinished">音量增强因子</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="451"/>
        <source>How much should sound volume increase for sources that are really close?</source>
        <translation type="unfinished">当音源距离过近是音量该增加多少?</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="289"/>
        <source>Checking this indicates that you don&apos;t have speakers connected, just headphones. This is important, as speakers are usually in front of you, while headphones are directly to your left/right.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="292"/>
        <source>Headphones</source>
        <translation type="unfinished">耳机</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="299"/>
        <source>Minimum Distance</source>
        <translation type="unfinished">最小距离</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="309"/>
        <source>Minimum distance to player before sound volume decreases</source>
        <translation type="unfinished">距离玩家最少多远时声音开始减弱</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="344"/>
        <source>Maximum Distance</source>
        <translation type="unfinished">最大距离</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="354"/>
        <source>Maximum distance, beyond which speech volume won&apos;t decrease</source>
        <translation type="unfinished">超过该距离时声音将不再减弱</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="357"/>
        <source>This sets the maximum distance for sound calculations. When farther away than this, other players&apos; speech volume will not decrease any further.</source>
        <translation type="unfinished">设置声强计算最大距离.当超过此距离时，别的玩家的音量将不再减弱.</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="386"/>
        <source>Minimum Volume</source>
        <translation type="unfinished">最小音量</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="399"/>
        <source>What should the volume be at the maximum distance?</source>
        <translation type="unfinished">位于最大距离时的音量大小?</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="470"/>
        <source>Loopback Test</source>
        <translation type="unfinished">回环测试</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="476"/>
        <source>Delay Variance</source>
        <translation type="unfinished">延迟差异</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="486"/>
        <source>Variance in packet latency</source>
        <translation type="unfinished">包延迟差异</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="489"/>
        <source>&lt;b&gt;This sets the packet latency variance for loopback testing.&lt;/b&gt;&lt;br /&gt;Most audio paths contain some variable latency. This allows you set that variance for loopback mode testing. For example, if you set this to 15ms, this will emulate a network with 20-35ms ping latency or one with 80-95ms latency. Most domestic net connections have a variance of about 5ms</source>
        <oldsource>&lt;b&gt;This sets the packet latency variance for loopback testing&lt;/b&gt;&lt;br /&gt;Most audio paths contain some variable latency. This allows you set that variance for loopback mode testing. For example, if you set this to 15ms, this will emulate a network with 20-35ms ping latency or one with 80-95ms latency. Most domestic net connections have a variance of about 5ms</oldsource>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="515"/>
        <source>Packet Loss</source>
        <translation type="unfinished">丢包</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="525"/>
        <source>Packet loss for loopback mode</source>
        <translation type="unfinished">回环模式丢包</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="528"/>
        <source>&lt;b&gt;This sets the packet loss for loopback mode.&lt;/b&gt;&lt;br /&gt;This will be the ratio of packets lost. Unless your outgoing bandwidth is peaked or there&apos;s something wrong with your network connection, this will be 0%</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="560"/>
        <source>&amp;Loopback</source>
        <translation type="unfinished">&amp;回环</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="570"/>
        <source>Desired loopback mode</source>
        <translation type="unfinished">期望的回环模式</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="573"/>
        <source>&lt;b&gt;This enables one of the loopback testmodes.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;None&lt;/i&gt; - Loopback disabled&lt;br /&gt;&lt;i&gt;Local&lt;/i&gt; - Emulate a local server.&lt;br /&gt;&lt;i&gt;Server&lt;/i&gt; - Request loopback from server.&lt;br /&gt;Please note than when loopback is enabled, no other players will hear your voice. This setting is not saved on application exit.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="245"/>
        <source>Other Applications</source>
        <translation type="unfinished">别的应用程序</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="255"/>
        <source>Volume of other applications during speech</source>
        <translation type="unfinished">当说话时别的应用程序的音量</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="258"/>
        <source>&lt;b&gt;Decrease volume of other applications during speech.&lt;/b&gt;&lt;br /&gt;Mumble supports decreasing the volume of other applications during incoming speech. This sets the relative volume of other applications when others are talking.</source>
        <translation type="unfinished">&lt;b&gt;对话时减弱别的应用程序的音量.&lt;/b&gt;&lt;br /&gt;Mumble支持对话过程中减弱别的应用程序的音量. 设置对话时别的应用程序的音量。</translation>
    </message>
</context>
<context>
    <name>AudioOutputDialog</name>
    <message>
        <location filename="AudioConfigDialog.cpp" line="320"/>
        <source>None</source>
        <translation type="unfinished">没有</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="321"/>
        <source>Local</source>
        <translation type="unfinished">客户端</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="322"/>
        <source>Server</source>
        <translation type="unfinished">服务器</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="328"/>
        <source>Audio Output</source>
        <translation type="unfinished">语音输出</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="421"/>
        <location filename="AudioConfigDialog.cpp" line="440"/>
        <source>%1 ms</source>
        <translation type="unfinished">%1 毫秒</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="432"/>
        <location filename="AudioConfigDialog.cpp" line="436"/>
        <location filename="AudioConfigDialog.cpp" line="444"/>
        <location filename="AudioConfigDialog.cpp" line="475"/>
        <location filename="AudioConfigDialog.cpp" line="479"/>
        <source>%1%</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="448"/>
        <source>%1ms</source>
        <translation type="unfinished">%1毫秒</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="463"/>
        <location filename="AudioConfigDialog.cpp" line="469"/>
        <source>%1m</source>
        <translation type="unfinished">%1分钟</translation>
    </message>
</context>
<context>
    <name>AudioStats</name>
    <message>
        <location filename="AudioStats.cpp" line="383"/>
        <source>&gt;1000ms</source>
        <translation type="unfinished">&gt;1000毫秒</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="13"/>
        <source>Audio Statistics</source>
        <translation type="unfinished">音频统计</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="21"/>
        <source>Input Levels</source>
        <translation type="unfinished">输入等级</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="27"/>
        <source>Peak microphone level</source>
        <translation type="unfinished">麦克风峰值等级</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="34"/>
        <location filename="AudioStats.ui" line="54"/>
        <location filename="AudioStats.ui" line="74"/>
        <source>Peak power in last frame</source>
        <translation type="unfinished">最后一帧的峰值功率</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="37"/>
        <source>This shows the peak power in the last frame (20 ms), and is the same measurement as you would usually find displayed as &quot;input power&quot;. Please disregard this and look at &lt;b&gt;Microphone power&lt;/b&gt; instead, which is much more steady and disregards outliers.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="40"/>
        <location filename="AudioStats.ui" line="60"/>
        <location filename="AudioStats.ui" line="80"/>
        <location filename="AudioStats.ui" line="109"/>
        <location filename="AudioStats.ui" line="129"/>
        <location filename="AudioStats.ui" line="149"/>
        <location filename="AudioStats.ui" line="180"/>
        <location filename="AudioStats.ui" line="197"/>
        <source>TextLabel</source>
        <translation type="unfinished">标签</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="47"/>
        <source>Peak speaker level</source>
        <translation type="unfinished">扬声器等级峰值</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="57"/>
        <source>This shows the peak power of the speakers in the last frame (20 ms). Unless you are using a multi-channel sampling method (such as ASIO) with speaker channels configured, this will be 0. If you have such a setup configured, and this still shows 0 while you&apos;re playing audio from other programs, your setup is not working.</source>
        <oldsource>This shows the peak power in the last frame (20 ms) of the speakers. Unless you are using a multi-channel sampling method (such as ASIO) with speaker channels configured, this will be 0. If you have such a setup configured, and this still shows 0 while you&apos;re playing audio from other programs, your setup is not working.</oldsource>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="67"/>
        <source>Peak clean level</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="77"/>
        <source>This shows the peak power in the last frame (20 ms) after all processing. Ideally, this should be -96 dB when you&apos;re not talking. In reality, a sound studio should see -60 dB, and you should hopefully see somewhere around -20 dB. When you are talking, this should rise to somewhere between -5 and -10 dB.&lt;br /&gt;If you are using echo cancellation, and this rises to more than -15 dB when you&apos;re not talking, your setup is not working, and you&apos;ll annoy other players with echoes.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="90"/>
        <source>Signal Analysis</source>
        <translation type="unfinished">信号分析</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="96"/>
        <source>Microphone power</source>
        <translation type="unfinished">麦克风功率</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="103"/>
        <source>How close the current input level is to ideal</source>
        <translation type="unfinished">当前输入等级离完美有多近</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="106"/>
        <source>This shows how close your current input volume is to the ideal. To adjust your microphone level, open whatever program you use to adjust the recording volume, and look at the value here while talking.&lt;br /&gt;&lt;b&gt;Talk loud, as you would when you&apos;re upset over getting fragged by a noob.&lt;/b&gt;&lt;br /&gt;Adjust the volume until this value is close to 100%, but make sure it doesn&apos;t go above. If it does go above, you are likely to get clipping in parts of your speech, which will degrade sound quality.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="116"/>
        <source>Signal-To-Noise ratio</source>
        <translation type="unfinished">信号-噪音 比率</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="123"/>
        <source>Signal-To-Noise ratio from the microphone</source>
        <translation type="unfinished">麦克风的信号-噪音 比率</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="126"/>
        <source>This is the Signal-To-Noise Ratio (SNR) of the microphone in the last frame (20 ms). It shows how much clearer the voice is compared to the noise.&lt;br /&gt;If this value is below 1.0, there&apos;s more noise than voice in the signal, and so quality is reduced.&lt;br /&gt;There is no upper limit to this value, but don&apos;t expect to see much above 40-50 without a sound studio.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="136"/>
        <source>Speech Probability</source>
        <translation type="unfinished">发言概率</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="143"/>
        <source>Probability of speech</source>
        <translation type="unfinished">发言的概率</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="146"/>
        <source>This is the probability that the last frame (20 ms) was speech and not environment noise.&lt;br /&gt;Voice activity transmission depends on this being right. The trick with this is that the middle of a sentence is always detected as speech; the problem is the pauses between words and the start of speech. It&apos;s hard to distinguish a sigh from a word starting with &apos;h&apos;.&lt;br /&gt;If this is in bold font, it means Mumble is currently transmitting (if you&apos;re connected).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="161"/>
        <source>Configuration feedback</source>
        <translation type="unfinished">配置反馈信息</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="167"/>
        <source>Current audio bitrate</source>
        <translation type="unfinished">当前音频比特率</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="174"/>
        <source>Bitrate of last frame</source>
        <translation type="unfinished">最后一帧的比特率</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="177"/>
        <source>This is the audio bitrate of the last compressed frame (20 ms), and as such will jump up and down as the VBR adjusts the quality. To adjust the peak bitrate, adjust &lt;b&gt;Compression Complexity&lt;/b&gt; in the Settings dialog.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="187"/>
        <source>DoublePush interval</source>
        <translation type="unfinished">两次按键间隔</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="194"/>
        <source>Time between last two PushToTalk presses</source>
        <translation type="unfinished">两次案件发音的时间间隔</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="204"/>
        <source>Speech Detection</source>
        <translation type="unfinished">语音侦测</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="211"/>
        <source>Current speech detection chance</source>
        <translation type="unfinished">当前语音侦测几率</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="214"/>
        <source>&lt;b&gt;This shows the current speech detection settings.&lt;/b&gt;&lt;br /&gt;You can change the settings from the Settings dialog or from the Audio Wizard.</source>
        <translation type="unfinished">&lt;b&gt;显示语音侦测设置.&lt;/b&gt;&lt;br /&gt;你可在音频精灵或设置对话框里重新设置.</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="243"/>
        <source>Signal and noise power spectrum</source>
        <translation type="unfinished">信号和噪音功率光谱</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="249"/>
        <source>Power spectrum of input signal and noise estimate</source>
        <translation type="unfinished">输入信号的功率范围和噪音估计</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="252"/>
        <source>This shows the power spectrum of the current input signal (red line) and the current noise estimate (filled blue).&lt;br /&gt;All amplitudes are multiplied by 30 to show the interesting parts (how much more signal than noise is present in each waveband).&lt;br /&gt;This is probably only of interest if you&apos;re trying to fine-tune noise conditions on your microphone. Under good conditions, there should be just a tiny flutter of blue at the bottom. If the blue is more than halfway up on the graph, you have a seriously noisy environment.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="268"/>
        <source>Echo Analysis</source>
        <translation type="unfinished">回声分析</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="280"/>
        <source>Weights of the echo canceller</source>
        <translation type="unfinished">回声抵消分量</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="283"/>
        <source>This shows the weights of the echo canceller, with time increasing downwards and frequency increasing to the right.&lt;br /&gt;Ideally, this should be black, indicating no echo exists at all. More commonly, you&apos;ll have one or more horizontal stripes of bluish color representing time delayed echo. You should be able to see the weights updated in real time.&lt;br /&gt;Please note that as long as you have nothing to echo off, you won&apos;t see much useful data here. Play some music and things should stabilize. &lt;br /&gt;You can choose to view the real or imaginary parts of the frequency-domain weights, or alternately the computed modulus and phase. The most useful of these will likely be modulus, which is the amplitude of the echo, and shows you how much of the outgoing signal is being removed at that time step. The other viewing modes are mostly useful to people who want to tune the echo cancellation algorithms.&lt;br /&gt;Please note: If the entire image fluctuates massively while in modulus mode, the echo canceller fails to find any correlation whatsoever between the two input sources (speakers and microphone). Either you have a very long delay on the echo, or one of the input sources is configured wrong.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>AudioWizard</name>
    <message>
        <location filename="AudioWizard.cpp" line="65"/>
        <source>Audio Tuning Wizard</source>
        <translation type="unfinished">音频调整精灵</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="115"/>
        <source>Introduction</source>
        <translation type="unfinished">介绍</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="116"/>
        <source>Welcome to the Mumble Audio Wizard</source>
        <translation type="unfinished">欢迎来到Mumble音频精灵</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="118"/>
        <source>This is the audio tuning wizard for Mumble. This will help you correctly set the input levels of your sound card, and also set the correct parameters for sound processing in Mumble.</source>
        <translation type="unfinished">我是Mumble音频调整精灵.我将帮你正确设置你的声卡的输入级别，并正确设置Mumble的声音处理参数.</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="123"/>
        <source>Please be aware that as long as this wizard is active, audio will be looped locally to allow you to listen to it, and no audio will be sent to the server.</source>
        <translation type="unfinished">请注意当音频精灵活跃时，音频将被在本地环回以允许你听到它，没有音频会被发送到服务器.</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="133"/>
        <source>Finished</source>
        <translation type="unfinished">完成</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="134"/>
        <source>Enjoy using Mumble</source>
        <translation type="unfinished">Mumble祝你使用愉快</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="137"/>
        <source>Congratulations. You should now be ready to enjoy a richer sound experience with Mumble.</source>
        <translation type="unfinished">祝贺你.你现在可以享受Mumble提供给你的丰富的声音体验.</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="160"/>
        <source>Device selection</source>
        <translation type="unfinished">设备选择</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="161"/>
        <source>Selecting the input and output device to use with Mumble.</source>
        <translation type="unfinished">选择Mumble使用的输入和输出设备.</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="163"/>
        <source>Input device</source>
        <translation type="unfinished">输入设备</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="166"/>
        <source>This is the device your microphone is connected to.</source>
        <translation type="unfinished">这是你麦克风所连接的设备。</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="180"/>
        <location filename="AudioWizard.cpp" line="232"/>
        <source>System</source>
        <translation type="unfinished">系统</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="183"/>
        <source>Input method for audio</source>
        <translation type="unfinished">音频输入方法</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="184"/>
        <source>&lt;b&gt;This is the input method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation type="unfinished">&lt;b&gt;音频输入方法&lt;/b&gt;&lt;br /&gt;很有可能你想用DirectSound.</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="196"/>
        <location filename="AudioWizard.cpp" line="248"/>
        <source>Device</source>
        <translation type="unfinished">设备</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="199"/>
        <source>Input device to use</source>
        <translation type="unfinished">输入设备</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="200"/>
        <source>&lt;b&gt;Selects which sound card to use for audio input.&lt;/b&gt;</source>
        <translation type="unfinished">&lt;b&gt;选择用作音频输入的声卡&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="206"/>
        <source>Use echo cancellation</source>
        <translation type="unfinished">使用回声消除</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="207"/>
        <source>Cancel echo from headset or speakers.</source>
        <translation type="unfinished">消除来自耳机或扬声器的回声.</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="208"/>
        <source>This enables echo cancellation of outgoing audio, which helps both on speakers and on headsets.</source>
        <translation type="unfinished">允许输出音频的回声消除,在扬声器和耳机上都有效。</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="213"/>
        <source>Output device</source>
        <translation type="unfinished">输出设备</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="216"/>
        <source>This is the device your speakers or headphones are connected to.</source>
        <translation type="unfinished">这是你的扬声器和耳机所连接的设备.</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="235"/>
        <source>Output method for audio</source>
        <translation type="unfinished">音频输出方法</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="236"/>
        <source>&lt;b&gt;This is the Output method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation type="unfinished">&lt;b&gt;音频输出方法.&lt;/b&gt;&lt;br/&gt;很有可能你想用DirectSound.</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="251"/>
        <source>Output device to use</source>
        <translation type="unfinished">输出设备</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="252"/>
        <source>&lt;b&gt;Selects which sound card to use for audio Output.&lt;/b&gt;</source>
        <translation type="unfinished">&lt;b&gt;选择用作音频输出的声卡.&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="258"/>
        <source>Enable positional audio</source>
        <translation type="unfinished">允许定位音频</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="259"/>
        <source>Allows positioning of sound.</source>
        <translation type="unfinished">允许声音定位.</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="260"/>
        <source>This allows Mumble to use positional audio to place voices.</source>
        <translation type="unfinished">允许Mumble使用音频定位来定位语音.</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="277"/>
        <source>Positional Audio</source>
        <translation type="unfinished">音频定位</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="278"/>
        <source>Adjusting attenuation of positional audio.</source>
        <translation type="unfinished">调整音频定位的衰减度.</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="283"/>
        <source>Mumble supports positional audio for some games, and will position the voice of other players relative to their position in game. Depending on their position, the volume of the voice will be changed between the speakers to simulate the direction and distance the other player is at. Such positioning depends on your speaker configuration being correct in your operating system, so a test is done here.</source>
        <translation type="unfinished">Mumble支持在某些游戏里使用声音定位,而且能够根据其他玩家在游戏中的位置来定位他们的声音.取决于他们的位置，音量将会被改变来模拟其他玩家所在的方向和距离.这样的定位依赖于你的扬声器在你的操作系统上被正确的配置,所以我们将对扬声器做测试.</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="288"/>
        <source>The graph below shows the position of &lt;font color=&quot;red&quot;&gt;you&lt;/font&gt;, the &lt;font color=&quot;yellow&quot;&gt;speakers&lt;/font&gt; and a &lt;font color=&quot;green&quot;&gt;moving sound source&lt;/font&gt; as if seen from above. You should hear the audio move between the channels.</source>
        <translation type="unfinished">下图显示出&lt;font color=&quot;red&quot;&gt;你&lt;/font&gt;的位置,&lt;font color=&quot;yellow&quot;&gt;扬声器&lt;/font&gt;和一个&lt;font color=&quot;green&quot;&gt;移动声源&lt;/font&gt;好像从上看.你应当听到音频在频道间移动.</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="292"/>
        <source>Use headphones</source>
        <translation type="unfinished">使用耳机</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="293"/>
        <source>Use headphones instead of speakers.</source>
        <translation type="unfinished">使用耳机而不是扬声器.</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="294"/>
        <source>This ignores the OS speaker configuration and configures the positioning for headphones instead.</source>
        <translation type="unfinished">我们将忽略操作系统的扬声器配置并未耳机配置音频定位.</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="316"/>
        <source>Volume tuning</source>
        <translation type="unfinished">音量调整</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="317"/>
        <source>Tuning microphone hardware volume to optimal settings.</source>
        <translation type="unfinished">调整麦克风硬件音量到最佳设置.</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="322"/>
        <source>Open your sound control panel and go to the recording settings. Make sure the microphone is selected as active input with maximum recording volume. If there&apos;s an option to enable a &quot;Microphone boost&quot; make sure it&apos;s checked.</source>
        <translation type="unfinished">打开你的音频控制面板然后悬着录音设置.确保麦克风被选为正在使用的输入并使用最大录音音量.请确保麦克风加速选项被选上.</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="327"/>
        <source>Speak loudly, as when you are annoyed or excited. Decrease the volume in the sound control panel until the bar below stays as high as possible in the blue and green but &lt;b&gt;not&lt;/b&gt; the red zone while you speak.</source>
        <translation type="unfinished">请你像自己在愤怒或高兴时那样大声讲话.减小你的声音控制面板里的音量直到下面的进度条在你讲话时在蓝区和绿区而&lt;b&gt;不是&lt;/b&gt;红区内保持最高.</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="338"/>
        <source>Now talk softly, as you would when talking late at night and you don&apos;t want to disturb anyone. Adjust the slider below so that the bar moves into green when you talk, but stays blue while you&apos;re silent.</source>
        <translation type="unfinished">现在轻声讲话，就像你在深夜时不想打扰任何人一样.调整下面的滑块直到当你讲话时进度条进入绿区而当你不说话时进度条进入蓝区.</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="357"/>
        <source>Voice Activity Detection</source>
        <translation type="unfinished">语音活动检测</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="358"/>
        <source>Letting Mumble figure out when you&apos;re talking and when you&apos;re silent.</source>
        <translation type="unfinished">让Mumble弄明白何时你在讲话及何时你在沉默.</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="366"/>
        <source>This will help Mumble figure out when you are talking. The first step is selecting which data value to use.</source>
        <translation type="unfinished">下面我们将帮Mumble弄明白何时你在讲话.第一步是选择用哪个数据值.</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="392"/>
        <source>Raw amplitude from input</source>
        <translation type="unfinished">输入的原始振幅</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="388"/>
        <source>Signal-To-Noise ratio</source>
        <translation type="unfinished">信躁比</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="415"/>
        <source>Next you need to adjust the following two sliders. The first few utterances you say should end up in the green area (definitive speech). While talking, you should stay inside the yellow (might be speech) and when you&apos;re not talking, everything should be in the red (definitively not speech).</source>
        <translation type="unfinished">下一步你需要调整下面两个滑块.你最开始讲的几段话应当在绿色区域结束(明确是讲话）.当你讲话时,你应当停留在黄色区域(可能是讲话），当你沉默时，你应当停留在红色区域(显然你在沉默）.</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="439"/>
        <source>Last, you need to adjust the grace time. Mumble may detect this much non-speech and still keep transmitting before it breaks off. This allows you to catch your breath while speaking.</source>
        <translation type="unfinished">最后，你需要调整缓解期.Mumble可能侦测一定数量的非话音并持续传输然后停止.这允许你在讲话时喘息.</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="467"/>
        <source>Device tuning</source>
        <translation type="unfinished">设备调整</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="468"/>
        <source>Changing hardware output delays to their minimum value.</source>
        <translation type="unfinished">调整硬件输出延迟到最小值.</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="472"/>
        <source>To keep latency to an absolute minium, it&apos;s important to buffer as little audio as possible on the soundcard. However, many soundcards report that they require a much smaller buffer than what they can actually work with, so the only way to set this value is to try and fail.</source>
        <translation type="unfinished">为了保持延迟绝对最小,让声卡缓冲尽可能少的语音非常重要.然而大多数声卡声称它们需要比实际需求更小的缓冲区,因此确定这个值的唯一方式是去尝试.</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="478"/>
        <source>You should hear a single tone that&apos;s changing in frequency. Change the slider below to the lowest value which gives &lt;b&gt;no&lt;/b&gt; interruptions or jitter in the sound. Please note that local echo is disabled during this test to improve audio path recognition.</source>
        <translation type="unfinished">你应当能听到一个不断改变频率的单音.移动下面的滑块直到它处于一个&lt;b&gt;不&lt;/b&gt;造成声音中断或颤音的最低值.请注意在测试中本地回声被禁止以改善音频路径识别.</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="495"/>
        <source>Amount of data to buffer.</source>
        <translation type="unfinished">数据缓冲大小.</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="496"/>
        <source>This sets the amount of data to prebuffer in the output buffer. Experiment with different values and set it to the lowest which doesn&apos;t cause rapid jitter in the sound.</source>
        <translation type="unfinished">下面将设置输出缓冲区将预缓冲的数据量.尝试不同的值并选择一个不导致颤音的最低值.</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="502"/>
        <source>If you position the microphone so it can pick up the speakers or headset, Mumble will measure the total audio path delay in your system; this means the delay from a sample is placed in an outbound buffer until it&apos;s found in a matching incoming buffer.</source>
        <translation type="unfinished">把麦克风放置到能捕捉到扬声器或耳机声音的地方,Mumble将计算你的系统的总的音频路径延迟;这个延迟是当一个声音采样被放到输出缓冲区到它被在输入缓冲区里找到的时间.</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="603"/>
        <source>%1ms</source>
        <translation type="unfinished">%1毫秒</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="793"/>
        <source>Audio path is %1ms long.</source>
        <translation type="unfinished">语音路径长%1毫秒.</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="795"/>
        <source>Audio path cannot be determined. Input not recognized.</source>
        <translation type="unfinished">语音路径无法被确定.输入无法被识别.</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="846"/>
        <source>%1 s</source>
        <translation type="unfinished">%1 秒</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="141"/>
        <source>Mumble is under continuous development, and the development team wants to focus on the features that benefit the most users. To this end, Mumble supports submitting anonymous statistics about your configuration to the developers. These statistcs are essential for future development, and also make sure the features you use aren&apos;t deprecated.</source>
        <translation type="unfinished">Mumble在被持续开发,开发团队希望重点开发对大多数用户都有益的功能.Mumble支持匿名递交你的设备配置给开发人员.这些统计数据对于未来的开发非常重要,并确保你使用的功能不会被取消.</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="148"/>
        <source>Submit anonymous statistics to the Mumble project</source>
        <translation type="unfinished">递交匿名统计给Mumble项目</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="372"/>
        <source>Push To Talk:</source>
        <translation type="unfinished">案件发音:</translation>
    </message>
</context>
<context>
    <name>BanEditor</name>
    <message>
        <location filename="BanEditor.ui" line="13"/>
        <source>Mumble - Edit Bans</source>
        <translation type="unfinished">Mumble - 编辑黑名单</translation>
    </message>
    <message>
        <location filename="BanEditor.ui" line="28"/>
        <source>&amp;Address</source>
        <translation type="unfinished">&amp;地址</translation>
    </message>
    <message>
        <location filename="BanEditor.ui" line="38"/>
        <source>&amp;Mask</source>
        <translation type="unfinished">&amp;子网掩码</translation>
    </message>
    <message>
        <location filename="BanEditor.ui" line="48"/>
        <source>0.0.0.0</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="BanEditor.ui" line="72"/>
        <source>&amp;Add</source>
        <translation type="unfinished">&amp;添加</translation>
    </message>
    <message>
        <location filename="BanEditor.ui" line="79"/>
        <source>&amp;Update</source>
        <translation type="unfinished">&amp;更新</translation>
    </message>
    <message>
        <location filename="BanEditor.ui" line="86"/>
        <source>&amp;Remove</source>
        <translation type="unfinished">&amp;移除</translation>
    </message>
</context>
<context>
    <name>ChanACL</name>
    <message>
        <location filename="../ACL.cpp" line="137"/>
        <source>W</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="139"/>
        <source>T</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="141"/>
        <source>E</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="143"/>
        <source>S</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="145"/>
        <source>A</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="147"/>
        <source>M</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="149"/>
        <source>K</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="151"/>
        <source>C</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="153"/>
        <source>L</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="163"/>
        <source>None</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="167"/>
        <source>Traverse</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="169"/>
        <source>Enter</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="171"/>
        <source>Speak</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="173"/>
        <source>AltSpeak</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="175"/>
        <source>Mute/Deafen</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="177"/>
        <source>Move/Kick</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="179"/>
        <source>Make channel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="181"/>
        <source>Link channel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="191"/>
        <source>This represents no privileges.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="193"/>
        <source>This represents total access to the channel, including the ability to change group and ACL information. This privilege implies all other privileges.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="196"/>
        <source>This represents the permission to traverse the channel. If a user is denied this privilege, he will be unable to access this channel and any sub-channels in any way, regardless of other permissions in the sub-channels.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="200"/>
        <source>This represents the permission to join the channel. If you have a hierarchical channel structure, you might want to give everyone Traverse, but restrict Enter in the root of your hierarchy.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="203"/>
        <source>This represents the permission to speak in a channel. Users without this privilege will be suppressed by the server (seen as muted), and will be unable to speak until they are unmuted by someone with the appropriate privileges.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="207"/>
        <source>This represents the permission to speak in a channel with flagged speech. This works exactly like the &lt;i&gt;speak&lt;/i&gt; privilege, but applies to packets spoken with AltPushToTalk held down. This may be used to broadcast to a hierarchy of channels without linking.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="211"/>
        <source>This represents the permission to mute and deafen other players. Once muted, a player will stay muted until he is unmuted by another privileged player or reconnects to the server.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="214"/>
        <source>This represents the permission to move a player to another channel or kick him from the server. To actually move the player, either the moving player must have Move/Kick privileges in the destination channel, or the player must normally be allowed to enter the channel. Players with this privilege can move players into channels the target player normally wouldn&apos;t have permission to enter.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="219"/>
        <source>This represents the permission to make sub-channels. The player making the sub-channel will be added to the admin group of the sub-channel.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="222"/>
        <source>This represents the permission to link channels. Players in linked channels hear each other, as long as the speaking player has the &lt;i&gt;speak&lt;/i&gt; privilege in the channel of the listener. You need the link privilege in both channels to create a link, but just in either channel to remove it.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="165"/>
        <source>Write ACL</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ClientPlayer</name>
    <message>
        <location filename="../Player.cpp" line="53"/>
        <source>Authenticated</source>
        <translation type="unfinished">经过认证的</translation>
    </message>
    <message>
        <location filename="../Player.cpp" line="55"/>
        <source>Muted (server)</source>
        <translation type="unfinished">静音(服务器)</translation>
    </message>
    <message>
        <location filename="../Player.cpp" line="57"/>
        <source>Deafened (server)</source>
        <translation type="unfinished">聋(服务器)</translation>
    </message>
    <message>
        <location filename="../Player.cpp" line="59"/>
        <source>Local Mute</source>
        <translation type="unfinished">本地静音</translation>
    </message>
    <message>
        <location filename="../Player.cpp" line="61"/>
        <source>Muted (self)</source>
        <translation type="unfinished">静音(我）</translation>
    </message>
    <message>
        <location filename="../Player.cpp" line="63"/>
        <source>Deafened (self)</source>
        <translation type="unfinished">聋(我)</translation>
    </message>
</context>
<context>
    <name>ConfigDialog</name>
    <message>
        <location filename="ConfigDialog.cpp" line="107"/>
        <source>Accept changes</source>
        <translation type="unfinished">确定</translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="108"/>
        <source>This button will accept current settings and return to the application.&lt;br /&gt;The settings will be stored to disk when you leave the application.</source>
        <translation type="unfinished">该按钮将接受当前设置并返回到应用程序.&lt;br/&gt;这些设置当你退出应用程序时将被存盘.</translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="112"/>
        <source>Reject changes</source>
        <translation type="unfinished">取消</translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="113"/>
        <source>This button will reject all changes and return to the application.&lt;br /&gt;The settings will be reset to the previous positions.</source>
        <translation type="unfinished">该按钮将取消改变并返回到应用程序&lt;br/&gt;设置将被重置到先前的位置.</translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="117"/>
        <source>Apply changes</source>
        <translation type="unfinished">应用</translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="118"/>
        <source>This button will immediately apply all changes.</source>
        <translation type="unfinished">该按钮将立即应用所有改变.</translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="121"/>
        <source>Undo changes for current page</source>
        <translation type="unfinished">撤销</translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="122"/>
        <source>This button will revert any changes done on the current page to the most recent applied settings.</source>
        <translation type="unfinished">该按钮将所有最近变动设置都撤销.</translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="125"/>
        <source>Restore defaults for current page</source>
        <translation type="unfinished">恢复默认值</translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="126"/>
        <source>This button will restore the settings for the current page only to their defaults. Other pages will be not be changed.&lt;br /&gt;To restore all settings to their defaults, you will have to use this button on every page.</source>
        <translation type="unfinished">该按钮将恢复本页设置到默认值.别的页面的设置将不被改变.&lt;br/&gt;为了恢复设置到默认值,你需要在每个页面上都使用该按钮.</translation>
    </message>
    <message>
        <location filename="ConfigDialog.ui" line="13"/>
        <source>Mumble Configuration</source>
        <translation type="unfinished">Mumble 配置</translation>
    </message>
    <message>
        <location filename="ConfigDialog.ui" line="26"/>
        <source>Tab 1</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ConnectDialog</name>
    <message>
        <location filename="ConnectDialog.cpp" line="124"/>
        <source>Connecting to %1</source>
        <translation type="unfinished">连接到 %1</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="124"/>
        <source>Enter username</source>
        <translation type="unfinished">请输入用户名</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="209"/>
        <source>Unknown</source>
        <translation type="unfinished">未知的</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="223"/>
        <source>Mumble</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="223"/>
        <source>Failed to fetch server list</source>
        <translation type="unfinished">无法取得服务器列表</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="265"/>
        <location filename="ConnectDialog.cpp" line="287"/>
        <source>New</source>
        <translation type="unfinished">新建</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="325"/>
        <source>-Unnamed entry-</source>
        <translation type="unfinished">-未命名-</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="349"/>
        <location filename="ConnectDialog.cpp" line="376"/>
        <source>Add</source>
        <translation type="unfinished">添加</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="374"/>
        <source>Update</source>
        <translation type="unfinished">更新</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui" line="13"/>
        <source>Mumble Server Connect</source>
        <translation type="unfinished">Mumble服务器连接</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui" line="23"/>
        <source>&amp;Custom Servers</source>
        <translation type="unfinished">&amp;定制服务器</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui" line="40"/>
        <source>&amp;Label</source>
        <translation type="unfinished">&amp;标签</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui" line="50"/>
        <source>A&amp;ddress</source>
        <translation type="unfinished">&amp;地址</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui" line="60"/>
        <source>&amp;Port</source>
        <translation type="unfinished">&amp;端口</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui" line="70"/>
        <source>&amp;Username</source>
        <translation type="unfinished">&amp;用户名</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui" line="80"/>
        <source>&amp;Password</source>
        <translation type="unfinished">&amp;密码</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui" line="128"/>
        <location filename="ConnectDialog.ui" line="207"/>
        <source>&amp;Connect</source>
        <translation type="unfinished">&amp;连接</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui" line="138"/>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;取消</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui" line="145"/>
        <source>&amp;Add</source>
        <translation type="unfinished">&amp;添加</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui" line="155"/>
        <source>&amp;Remove</source>
        <translation type="unfinished">&amp;移除</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui" line="165"/>
        <source>Server &amp;Browser</source>
        <translation type="unfinished">服务器 &amp;浏览器</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui" line="187"/>
        <source>Label</source>
        <translation type="unfinished">标签</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui" line="192"/>
        <source>Address</source>
        <translation type="unfinished">地址</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui" line="197"/>
        <source>URL</source>
        <translation type="unfinished">网址</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui" line="217"/>
        <source>Cancel</source>
        <translation type="unfinished">取消</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui" line="224"/>
        <source>C&amp;opy to custom</source>
        <translation type="unfinished">&amp;拷贝定制</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui" line="231"/>
        <source>&amp;View Webpage</source>
        <translation type="unfinished">&amp;访问网页</translation>
    </message>
</context>
<context>
    <name>DXAudioInput</name>
    <message>
        <location filename="DirectSound.cpp" line="172"/>
        <source>Default DirectSound Voice Input</source>
        <translation type="unfinished">默认DirectSound语音输入</translation>
    </message>
    <message>
        <location filename="DirectSound.cpp" line="527"/>
        <source>Opening chosen DirectSound Input device failed. No microphone capture will be done.</source>
        <translation type="unfinished">不能打开所选的DirectSound输入设备.不会捕捉麦克风.</translation>
    </message>
    <message>
        <location filename="DirectSound.cpp" line="532"/>
        <source>Opening chosen DirectSound Input failed. Default device will be used.</source>
        <translation type="unfinished">不能打开所选的DirectSound输入.将使用默认设备.</translation>
    </message>
    <message>
        <location filename="DirectSound.cpp" line="638"/>
        <source>Lost DirectSound input device.</source>
        <translation type="unfinished">失去DirectSound输入设备.</translation>
    </message>
</context>
<context>
    <name>DXAudioOutput</name>
    <message>
        <location filename="DirectSound.cpp" line="132"/>
        <source>Default DirectSound Voice Output</source>
        <translation type="unfinished">默认DirectSound语音输出</translation>
    </message>
    <message>
        <location filename="DirectSound.cpp" line="380"/>
        <source>Opening chosen DirectSound Output failed. Default device will be used.</source>
        <translation type="unfinished">不能打开所选DirectSound输出.将使用默认设备.</translation>
    </message>
    <message>
        <location filename="DirectSound.cpp" line="458"/>
        <source>Lost DirectSound output device.</source>
        <translation type="unfinished">失去DirectSound输出设备.</translation>
    </message>
    <message>
        <location filename="DirectSound.cpp" line="462"/>
        <source>Opening chosen DirectSound Output failed. No audio will be heard.</source>
        <translation type="unfinished">不能打开所选DirectSound输出.将听不到声音.</translation>
    </message>
</context>
<context>
    <name>Database</name>
    <message>
        <location filename="Database.cpp" line="99"/>
        <source>Mumble</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Database.cpp" line="99"/>
        <source>Mumble failed to initialize a database in any
of the possible locations.</source>
        <translation type="unfinished">Mumble不能在任何可能的位置初始化数据库.</translation>
    </message>
</context>
<context>
    <name>GlobalShortcut</name>
    <message>
        <location filename="GlobalShortcut.ui" line="17"/>
        <source>Shortcuts</source>
        <translation type="unfinished">快捷键</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.ui" line="23"/>
        <source>List of configured shortcuts</source>
        <translation type="unfinished">配置的快捷键列表</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.ui" line="48"/>
        <source>Function</source>
        <translation type="unfinished">功能</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.ui" line="53"/>
        <source>Shortcut</source>
        <translation type="unfinished">快捷键</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.ui" line="58"/>
        <source>Suppress</source>
        <translation type="unfinished">禁用</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.ui" line="66"/>
        <source>Add new shortcut</source>
        <translation type="unfinished">添加新快捷键</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.ui" line="69"/>
        <source>This will add a new global shortcut</source>
        <translation type="unfinished">这将添加一个新的快捷键</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.ui" line="72"/>
        <source>&amp;Add</source>
        <translation type="unfinished">&amp;添加</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.ui" line="82"/>
        <source>Remove selected shortcut</source>
        <translation type="unfinished">移除所选快捷键</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.ui" line="85"/>
        <source>This will permanently remove a selected shortcut.</source>
        <translation type="unfinished">这将永久性的移除所选快捷键.</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.ui" line="88"/>
        <source>&amp;Remove</source>
        <translation type="unfinished">&amp;移除</translation>
    </message>
</context>
<context>
    <name>GlobalShortcutConfig</name>
    <message>
        <location filename="GlobalShortcut.cpp" line="228"/>
        <source>Shortcuts</source>
        <translation type="unfinished">快捷键</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.cpp" line="261"/>
        <source>Shortcut button combination.</source>
        <translation type="unfinished">快捷键按钮组合.</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.cpp" line="262"/>
        <source>&lt;b&gt;This is the global shortcut key combination.&lt;/b&gt;&lt;br /&gt;Double-click this field and then the desired key/button combo to rebind.</source>
        <translation type="unfinished">&lt;b&gt;这是全局快捷键组合.&lt;/b&gt;&lt;br/&gt;双击该区域然后绑定想要的按键/按钮组合.</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.cpp" line="266"/>
        <source>Suppress keys from other applications</source>
        <translation type="unfinished">禁止其他应用程序使用的案件</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.cpp" line="267"/>
        <source>&lt;b&gt;This hides the button presses from other applications.&lt;/b&gt;&lt;br /&gt;Enabling this will hide the button (or the last button of a multi-button combo) from other applications. Note that not all buttons can be suppressed.</source>
        <translation type="unfinished">&lt;b&gt;这将向其他应用程序隐藏这些按键.&lt;/b&gt;&lt;br/&gt;允许这个选项将像其他应用程序隐藏指定按键(或者组合按键的最后一个).注意并非所有按键都能被禁用.</translation>
    </message>
</context>
<context>
    <name>GlobalShortcutMacInit</name>
    <message>
        <location filename="GlobalShortcut_macx.cpp" line="60"/>
        <source>Mumble has detected that it is unable to receive Global Shortcut events when it is in the background.&lt;br /&gt;&lt;br /&gt;This is because the Universal Access feature called &apos;Enable access for assistive devices&apos; is currently disabled.&lt;br /&gt;&lt;br /&gt;Please &lt;a href=&quot; &quot;&gt;enable this setting&lt;/a&gt; and continue when done.</source>
        <translation type="unfinished">Mumble检测到当它位于后台工作时它不恩那个接收到全局快捷键事件.&lt;br/&gt;&lt;br/&gt;这是由于被称作允许使用辅助设备的辅助使用功能被禁用.&lt;br/&gt;&lt;br/&gt;请&lt;a href=&quot; &quot;&gt;打开此选项&lt;/a&gt;然后继续使用应用程序.</translation>
    </message>
</context>
<context>
    <name>GlobalShortcutX</name>
    <message>
        <location filename="GlobalShortcut_unix.cpp" line="319"/>
        <source>Mouse %1</source>
        <translation type="unfinished">鼠标 %1</translation>
    </message>
</context>
<context>
    <name>LCD</name>
    <message>
        <location filename="LCD.cpp" line="274"/>
        <source>Not connected</source>
        <translation type="unfinished">未连接</translation>
    </message>
</context>
<context>
    <name>LCDConfig</name>
    <message>
        <location filename="LCD.cpp" line="113"/>
        <source>Graphic</source>
        <translation type="unfinished">图形</translation>
    </message>
    <message>
        <location filename="LCD.cpp" line="115"/>
        <source>Character</source>
        <translation type="unfinished">字符</translation>
    </message>
    <message>
        <location filename="LCD.cpp" line="126"/>
        <source>Enable this device</source>
        <translation type="unfinished">允许该设备</translation>
    </message>
    <message>
        <location filename="LCD.cpp" line="135"/>
        <source>LCD</source>
        <translation type="unfinished">液晶显示器</translation>
    </message>
    <message>
        <location filename="LCD.ui" line="13"/>
        <source>Form</source>
        <translation type="unfinished">表单</translation>
    </message>
    <message>
        <location filename="LCD.ui" line="25"/>
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
        <location filename="LCD.ui" line="41"/>
        <source>Devices</source>
        <translation type="unfinished">设备</translation>
    </message>
    <message>
        <location filename="LCD.ui" line="57"/>
        <source>Name</source>
        <translation type="unfinished">名字</translation>
    </message>
    <message>
        <location filename="LCD.ui" line="62"/>
        <source>Type</source>
        <translation type="unfinished">类型</translation>
    </message>
    <message>
        <location filename="LCD.ui" line="67"/>
        <source>Size</source>
        <translation type="unfinished">大小</translation>
    </message>
    <message>
        <location filename="LCD.ui" line="72"/>
        <source>Enabled</source>
        <translation type="unfinished">启用</translation>
    </message>
    <message>
        <location filename="LCD.ui" line="89"/>
        <source>Views</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LCD.ui" line="95"/>
        <source>Minimum Column Width</source>
        <translation type="unfinished">最小列宽</translation>
    </message>
    <message>
        <location filename="LCD.ui" line="102"/>
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
        <location filename="LCD.ui" line="126"/>
        <location filename="LCD.ui" line="160"/>
        <source>TextLabel</source>
        <translation type="unfinished">文本标签</translation>
    </message>
    <message>
        <location filename="LCD.ui" line="133"/>
        <source>Splitter Width</source>
        <translation type="unfinished">切分窗口宽度</translation>
    </message>
    <message>
        <location filename="LCD.ui" line="140"/>
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
        <location filename="Log.cpp" line="220"/>
        <source>Debug</source>
        <translation type="unfinished">调试</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="221"/>
        <source>Critical</source>
        <translation type="unfinished">危急</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="222"/>
        <source>Warning</source>
        <translation type="unfinished">警告</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="223"/>
        <source>Information</source>
        <translation type="unfinished">信息</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="224"/>
        <source>Server Connected</source>
        <translation type="unfinished">服务器已连接</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="225"/>
        <source>Server Disconnected</source>
        <translation type="unfinished">服务器未连接</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="226"/>
        <source>Player Joined Server</source>
        <translation type="unfinished">玩家进入服务器</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="227"/>
        <source>Player Left Server</source>
        <translation type="unfinished">玩家离开服务器</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="228"/>
        <source>Player kicked (you or by you)</source>
        <translation type="unfinished">玩家踢了你（或被你踢了）</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="229"/>
        <source>Player kicked</source>
        <translation type="unfinished">玩家踢了你（或被你踢了）</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="230"/>
        <source>You self-muted/deafened</source>
        <translation type="unfinished">你静音/致聋</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="231"/>
        <source>Other self-muted/deafened</source>
        <translation type="unfinished">别人静音/致聋</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="232"/>
        <source>Player muted (you)</source>
        <translation type="unfinished">玩家对你静音</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="233"/>
        <source>Player muted (by you)</source>
        <translation type="unfinished">玩家被你静音</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="234"/>
        <source>Player muted (other)</source>
        <translation type="unfinished">玩家静音别人</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="235"/>
        <source>Player Joined Channel</source>
        <translation type="unfinished">玩家加入频道</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="236"/>
        <source>Player Left Channel</source>
        <translation type="unfinished">玩家离开频道</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="237"/>
        <source>Permission Denied</source>
        <translation type="unfinished">没有足够权限</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="238"/>
        <source>Text Message</source>
        <translation type="unfinished">文本消息</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="239"/>
        <source>Script Errors</source>
        <translation type="unfinished">脚本错误</translation>
    </message>
</context>
<context>
    <name>LogConfig</name>
    <message>
        <location filename="Log.cpp" line="74"/>
        <source>Toggle console for %1 events</source>
        <translation type="unfinished">为%1事件打开控制台</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="75"/>
        <source>Toggle pop-up notifications for %1 events</source>
        <translation type="unfinished">为%1事件打开弹出对话框</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="76"/>
        <source>Toggle Text-To-Speech for %1 events</source>
        <translation type="unfinished">为%1事件打开文本语音转换</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="77"/>
        <source>Click here to toggle sound notification for %1 events</source>
        <translation type="unfinished">点击这里为%1事件打开声音通知</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="78"/>
        <source>Path to soundfile used for sound notifications in the case of %1 events&lt;br /&gt;Single click to play&lt;br /&gt;Doubleclick to change</source>
        <translation type="unfinished">为%1时间用作声音通知的声音文件的路径&lt;br/&gt;单击播放&lt;br/&gt;双击更换</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="80"/>
        <source>Click here to toggle console output for %1 events.&lt;br /&gt;If checked, this option makes Mumble output all %1 events in its message log.</source>
        <translation type="unfinished">点击这里为%1事件打开控制台输出.&lt;br/&gt;如果选择该选项, Mumble将输出%1事件到它的消息记录文件中.</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="81"/>
        <source>Click here to toggle pop-up notifications for %1 events.&lt;br /&gt;If checked, a notification pop-up will be created by mumble for every %1 event.</source>
        <translation type="unfinished">点击这里为%1事件打开弹出式通知.&lt;br/&gt;如果选择这个选项,Mumble将为每个%1事件打开一个弹出式通知框.</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="82"/>
        <source>Click here to toggle Text-To-Speech for %1 events.&lt;br /&gt;If checked, mumble uses Text-To-Speech to read %1 events out loud to you. Text-To-Speech is also able to read the contents of the event which is not true for soundfiles. Text-To-Speech and soundfiles cannot be used at the same time.</source>
        <translation type="unfinished">点击这里为%1事件打开文本语音转换.&lt;br/&gt;如果选择这个选项,Mumble将使用文本语音转化来把%1事件大声的读给你.当声音文件无效时,文本语音转换同样能用于读取事件内容.文本语音转化和声音文件不能同时使用.</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="83"/>
        <source>Click here to toggle sound notification for %1 events.&lt;br /&gt;If checked, mumble uses a soundfile predefined by you to indicate %1 events. Soundfiles and Text-To-Speech cannot be used at the same time.</source>
        <translation type="unfinished">点击这里为%1事件打开声音通知.&lt;br/&gt;如果选择该选项,Mumble使用一个你事先制定的声音文件来指示%1事件.声音文件和文本语音转换不能同时被使用.</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="84"/>
        <source>Path to soundfile used for sound notifications in the case of %1 events.&lt;br /&gt;Single click to play&lt;br /&gt;Doubleclick to change&lt;br /&gt;Be sure that sound notifications for these events are enabled or this field will not have any effect.</source>
        <translation type="unfinished">为%1事件用作声音通知的声音文件的路径.&lt;br/&gt;单击播放&lt;br/&gt;双击更换&lt;br/&gt;请确保这些事件的声音通知被允许，否则这个文件将失效.</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="177"/>
        <source>Choose sound file</source>
        <translation type="unfinished">选择声音文件</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="181"/>
        <source>Invalid sound file</source>
        <translation type="unfinished">无效声音文件</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="182"/>
        <source>The file &apos;%1&apos; does not exist or is not a valid speex file.</source>
        <translation type="unfinished">%1文件不存在或者不是有效的语音文件.</translation>
    </message>
    <message>
        <location filename="Log.ui" line="13"/>
        <source>Messages</source>
        <translation type="unfinished">消息</translation>
    </message>
    <message>
        <location filename="Log.ui" line="35"/>
        <source>Message</source>
        <translation type="unfinished">消息</translation>
    </message>
    <message>
        <location filename="Log.ui" line="40"/>
        <source>Console</source>
        <translation type="unfinished">控制台</translation>
    </message>
    <message>
        <location filename="Log.ui" line="45"/>
        <source>Notification</source>
        <translation type="unfinished">通知</translation>
    </message>
    <message>
        <location filename="Log.ui" line="50"/>
        <source>Text-To-Speech</source>
        <translation type="unfinished">文本语音转换</translation>
    </message>
    <message>
        <location filename="Log.ui" line="55"/>
        <source>Soundfile</source>
        <translation type="unfinished">声音文件</translation>
    </message>
    <message>
        <location filename="Log.ui" line="60"/>
        <source>Path</source>
        <translation type="unfinished">路径</translation>
    </message>
    <message>
        <location filename="Log.ui" line="68"/>
        <source>Text To Speech</source>
        <translation type="unfinished">文本语音转换</translation>
    </message>
    <message>
        <location filename="Log.ui" line="74"/>
        <source>Volume</source>
        <translation type="unfinished">音量</translation>
    </message>
    <message>
        <location filename="Log.ui" line="84"/>
        <source>Volume of Text-To-Speech Engine</source>
        <translation type="unfinished">文本语音转换引擎的音量</translation>
    </message>
    <message>
        <location filename="Log.ui" line="87"/>
        <source>&lt;b&gt;This is the volume used for the speech synthesis.&lt;/b&gt;</source>
        <translation type="unfinished">&lt;b&gt;这是用作语音合成的音量.&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="Log.ui" line="112"/>
        <source>Length threshold</source>
        <translation type="unfinished">长度界限</translation>
    </message>
    <message>
        <location filename="Log.ui" line="122"/>
        <source>Message length threshold for Text-To-Speech Engine</source>
        <translation type="unfinished">文本语音转换的消息长度界限</translation>
    </message>
    <message>
        <location filename="Log.ui" line="125"/>
        <source>&lt;b&gt;This is the length threshold used for the Text-To-Speech Engine.&lt;/b&gt;&lt;br /&gt;Messages longer than this limit will not be read aloud in their full length.</source>
        <translation type="unfinished">&lt;b&gt;这是用作文本语音转换引擎的消息长度界限&lt;/b&gt;&lt;br/&gt;大于该长度的消息将不会被全部读出来.</translation>
    </message>
    <message>
        <location filename="Log.ui" line="131"/>
        <source> Characters</source>
        <translation type="unfinished">字符</translation>
    </message>
</context>
<context>
    <name>LookConfig</name>
    <message>
        <location filename="LookConfig.cpp" line="46"/>
        <location filename="LookConfig.cpp" line="57"/>
        <source>System default</source>
        <translation type="unfinished">系统默认</translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="61"/>
        <source>None</source>
        <translation type="unfinished">无</translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="62"/>
        <source>Only with players</source>
        <translation type="unfinished">仅和玩家一起</translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="63"/>
        <source>All</source>
        <translation type="unfinished">所有</translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="65"/>
        <source>Ask</source>
        <translation type="unfinished">问</translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="66"/>
        <source>Do Nothing</source>
        <translation type="unfinished">什么也不做</translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="67"/>
        <source>Move</source>
        <translation type="unfinished">移动</translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="71"/>
        <source>User Interface</source>
        <translation type="unfinished">用户界面</translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="159"/>
        <source>Choose skin file</source>
        <translation type="unfinished">选择皮肤文件</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="13"/>
        <source>Form</source>
        <translation type="unfinished">表单</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="19"/>
        <location filename="LookConfig.ui" line="25"/>
        <source>Language</source>
        <translation type="unfinished">语言</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="35"/>
        <source>Language to use (requires restart)</source>
        <translation type="unfinished">使用的语言(需要重启）</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="38"/>
        <source>&lt;b&gt;This sets which language Mumble should use.&lt;/b&gt;&lt;br /&gt;You have to restart Mumble to use the new language.</source>
        <translation type="unfinished">&lt;b&gt;设置Mumble使用何种语言.&lt;/b&gt;&lt;br/&gt;你需要重启Mumble来使用新语言.</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="48"/>
        <source>Look and Feel</source>
        <translation type="unfinished">外观</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="71"/>
        <source>Style</source>
        <translation type="unfinished">风格</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="78"/>
        <source>Basic widget style</source>
        <translation type="unfinished">基础窗口风格</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="81"/>
        <source>&lt;b&gt;This sets the basic look and feel to use.&lt;/b&gt;</source>
        <translation type="unfinished">&lt;b&gt;使用基础外观.&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="88"/>
        <source>Skin</source>
        <translation type="unfinished">面板</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="95"/>
        <source>Skin file to use</source>
        <translation type="unfinished">使用面板文件</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="98"/>
        <source>&lt;b&gt;This sets which skin Mumble should use.&lt;/b&gt;&lt;br /&gt;The skin is a style file applied on top of the basic widget style. If there are icons in the same directory as the style sheet, those will replace the default icons.</source>
        <translation type="unfinished">&lt;b&gt;设置Mumble使用那个面板文件.&lt;/b&gt;&lt;br/&gt;面板是一个用在基础窗口风格上的风格文件.样式表目录中的图标将取代默认图标.</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="105"/>
        <source>...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="112"/>
        <source>Expand</source>
        <translation type="unfinished">展开</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="119"/>
        <source>When to automatically expand channels</source>
        <translation type="unfinished">何时自动展开频道</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="122"/>
        <source>This sets which channels to automatically expand. &lt;i&gt;None&lt;/i&gt; and &lt;i&gt;All&lt;/i&gt; will expand no or all channels, while &lt;i&gt;Only with players&lt;/i&gt; will expand and collapse channels as players join and leave them.</source>
        <translation type="unfinished">设置哪些频道将自动展开.&lt;i&gt;无&lt;/i&gt;和&lt;i&gt;所有&lt;/i&gt;将展开零活所有频道,而&lt;i&gt;只有当有玩家时&lt;i&gt;将在玩家加入和离开时自动展开和折叠频道.</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="129"/>
        <source>List players above subchannels (requires restart).</source>
        <translation type="unfinished">将玩家置于之频道上方(需要重启).</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="132"/>
        <source>&lt;b&gt;If set, players will be shown above subchannels in the channel view.&lt;/b&gt;&lt;br /&gt;A restart of Mumble is required to see the change.</source>
        <translation type="unfinished">&lt;b&gt;设置该选项将使玩家显示在子频道之上.&lt;/b&gt;&lt;br/&gt;需要重启Mumble来应用该改变.</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="135"/>
        <source>Players above Channels</source>
        <translation type="unfinished">玩家位于频道之上</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="54"/>
        <source>Channel Dragging</source>
        <translation type="unfinished">频道拖动</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="61"/>
        <source>This changes the behaviour when moving channels.</source>
        <translation type="unfinished">改变当移动频道时的行为.</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="64"/>
        <source>This sets the behaviour of channel drags, it can be used to prevent accidental dragging. &lt;i&gt;Move Channel&lt;/i&gt; moves the channel without prompting. &lt;i&gt;Do Nothing&lt;/i&gt; does nothing and prints an error message. &lt;i&gt;Ask&lt;/i&gt; uses a message box to confirm if you really wanted to move the channel.</source>
        <translation type="unfinished">设置频道拖动时的行为,能被用来防止意外的拖动.&lt;i&gt;移动频道&lt;/i&gt;将在移动频道是不提示.&lt;i&gt;什么都不做&lt;/i&gt;将在试图移动频道时显示一个错误信息.&lt;i&gt;问&lt;/i&gt;使用一个消息框来确定你是真的想移动一个频道.</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="142"/>
        <source>Ask whether to close or minimize when quitting Mumble.</source>
        <translation type="unfinished">询问你推出Mumble时你想关闭还是最小化.</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="145"/>
        <source>&lt;b&gt;If set, will verify you want to quit if connected.&lt;b&gt;</source>
        <translation type="unfinished">&lt;b&gt;设置改选项将在你连接到服务器时验证你是否想退出.&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="148"/>
        <source>Ask on quit while connected</source>
        <translation type="unfinished">当你在处于连接状态询问是否退出</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="168"/>
        <source>Make the Mumble window appear on top of other windows.</source>
        <translation type="unfinished">使Mumble窗口位于别的窗口之上.</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="171"/>
        <source>&lt;b&gt;This makes the Mumble window a topmost window.&lt;/b&gt;</source>
        <translation type="unfinished">&lt;b&gt;使Mumble窗口成为最上面的窗口.&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="174"/>
        <source>Always On Top</source>
        <translation type="unfinished">窗口置顶</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="158"/>
        <source>&lt;b&gt;If set, minimizing the Mumble main window will cause it to be hidden and accessible only from the tray. Otherwise, it will be minimized as a window normally would.&lt;/b&gt;</source>
        <translation type="unfinished">&lt;b&gt;设置该选项将使Mumble主窗口被最小化时隐藏到系统托盘。而不是像普通窗口一样最小化.&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="161"/>
        <source>Hide in tray when minimized</source>
        <translation type="unfinished">最小化时隐藏到系统托盘</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="181"/>
        <source>Show all configuration items</source>
        <translation type="unfinished">显示左右配置选项</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="184"/>
        <source>&lt;b&gt;This will show all configuration items.&lt;/b&gt;&lt;br /&gt;Mumble contains a lot of configuration items that most users won&apos;t need to change. Checking this will show all configurable items.</source>
        <translation type="unfinished">&lt;b&gt;显示所有配置选项.&lt;/b&gt;&lt;br/&gt;Mumble包含许多普通用户不需要改变的选项.选择该选项将显示所有配置选项.</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="187"/>
        <source>Expert Config</source>
        <translation type="unfinished">专家配置</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="155"/>
        <source>Hide the main Mumble window in the tray when it is minimized.</source>
        <translation type="unfinished">当Mumble最小化时隐藏到系统托盘.</translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <location filename="MainWindow.cpp" line="128"/>
        <location filename="MainWindow.cpp" line="1322"/>
        <source>Root</source>
        <translation type="unfinished">根</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="161"/>
        <source>Push-to-Talk</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished">按键发音</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="163"/>
        <source>Push and hold this button to send voice.</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished">按住该按钮发送语音。</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="164"/>
        <source>This configures the push-to-talk button, and as long as you hold this button down, you will transmit voice.</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished">配置按键发音按键，只要按住该键，声音将被传输.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="167"/>
        <source>Reset Audio Processor</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished">重置音频处理器</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="170"/>
        <source>Toggle Mute Self</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished">静音自己</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="172"/>
        <source>Toggle self-mute status.</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished">静音自己状态.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="173"/>
        <source>This will toggle your muted status. If you toggle this off, you will also disable self-deafen.</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished">静音状态.如果你关闭静音状态，将同时关闭自己变聋状态.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="175"/>
        <source>Toggle Deafen Self</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished">自已变聋</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="177"/>
        <source>Toggle self-deafen status.</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished">自己变聋状态.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="178"/>
        <source>This will toggle your deafen status. If you toggle this on, you will also enable self-mute.</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished">该选项改变你的致聋状态。如果打开此选项，你想自我禁音.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="180"/>
        <source>Unlink Plugin</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished">卸载插件</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="183"/>
        <source>Force Center Position</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="188"/>
        <source>Chan Parent</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished">父频道</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="193"/>
        <source>Chan Sub#%1</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished">子频道#%1</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="198"/>
        <source>Chan All Subs</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished">所有子频道</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="202"/>
        <source>Push-to-Mute</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished">按键禁音</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="205"/>
        <source>Join Channel</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished">加入频道</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="208"/>
        <source>Toggle Overlay</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished">打开/关闭 重叠</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="210"/>
        <source>Toggle state of in-game overlay.</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished">打开/关闭游戏中的重叠状态.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="211"/>
        <source>This will switch the states of the ingame overlay between showing everybody, just the players who are talking, and nobody.</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished">在显示所有人，只有讲话的玩家和没有人之间切换游戏中的重叠状态.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="214"/>
        <source>Alt Push-to-Talk</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished">变更案件发音</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="217"/>
        <source>Toggle Minimal</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished">打开/关闭最小化</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="220"/>
        <source>Volume Up (+10%)</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished">增大音量(+10%)</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="223"/>
        <source>Volume Down (-10%)</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished">减小音量(-10%)</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="227"/>
        <location filename="MainWindow.cpp" line="316"/>
        <location filename="MainWindow.cpp" line="893"/>
        <location filename="MainWindow.cpp" line="915"/>
        <location filename="MainWindow.cpp" line="936"/>
        <location filename="MainWindow.cpp" line="1386"/>
        <location filename="MainWindow.ui" line="14"/>
        <location filename="main.cpp" line="217"/>
        <source>Mumble</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="236"/>
        <source>Mumble -- %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="240"/>
        <source>&amp;Window</source>
        <translation type="unfinished">&amp;窗口</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="242"/>
        <location filename="MainWindow.cpp" line="318"/>
        <source>Minimize</source>
        <translation type="unfinished">最小化</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="242"/>
        <source>Ctrl+M</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="317"/>
        <source>Close</source>
        <translation type="unfinished">关闭</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="316"/>
        <source>Mumble is currently connected to a server. Do you want to Close or Minimize it?</source>
        <translation type="unfinished">Mumble现在连接到了一个服务器.你想要关闭还是最小化?</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="399"/>
        <source>Clear</source>
        <translation type="unfinished">清楚</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="405"/>
        <source>Opening URL %1</source>
        <translation type="unfinished">打开网址 %1</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="407"/>
        <source>URL scheme is not &apos;mumble&apos;</source>
        <translation type="unfinished">&apos;Mumble&apos;不支持的网址</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="432"/>
        <source>Connecting to %1</source>
        <translation type="unfinished">连接到 %1</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="432"/>
        <source>Enter username</source>
        <translation type="unfinished">输入用户名</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="568"/>
        <source>Reconnecting.</source>
        <translation type="unfinished">重新连接中.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="618"/>
        <source>&lt;h2&gt;Control channel&lt;/h2&gt;&lt;p&gt;Encrypted with %1 bit %2&lt;br /&gt;%3 ms average latency (%4 variance)&lt;/p&gt;</source>
        <translation type="unfinished">&lt;h2&gt;控制信道&lt;/h2&gt;&lt;p&gt;使用%1比特%2加密&lt;br /&gt;平均延迟%3毫秒(%4 差值)&lt;/p&gt;</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="622"/>
        <source>Voice channel is sent over control channel.</source>
        <translation type="unfinished">语音频道通过控制信道发送.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="624"/>
        <source>&lt;h2&gt;Voice channel&lt;/h2&gt;&lt;p&gt;Encrypted with 128 bit OCB-AES128&lt;br /&gt;%1 ms average latency (%4 variance)&lt;/p&gt;</source>
        <translation type="unfinished">&lt;h2&gt;语音频道&lt;/h2&gt;&lt;p&gt;使用128比特OCB-AES128加密&lt;br/&gt;%1毫秒延迟(%4差值)&lt;/p&gt;</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="631"/>
        <source>UDP Statistics</source>
        <translation type="unfinished">UDP统计</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="631"/>
        <source>To Server</source>
        <translation type="unfinished">到服务器</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="631"/>
        <source>From Server</source>
        <translation type="unfinished">来至服务器</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="631"/>
        <source>Good</source>
        <translation type="unfinished">好的</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="631"/>
        <source>Late</source>
        <translation type="unfinished">迟的</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="631"/>
        <source>Lost</source>
        <translation type="unfinished">丢失</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="631"/>
        <source>Resync</source>
        <translation type="unfinished">重新同步</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="635"/>
        <source>&lt;h2&gt;Audio bandwidth&lt;/h2&gt;&lt;p&gt;Maximum %1 kbit/s&lt;br /&gt;Current %2 kbit/s (Quality %3)&lt;/p&gt;</source>
        <translation type="unfinished">&lt;h2&gt;音频带宽&lt;/h2&gt;&lt;p&gt;最大 %1千比特/秒&lt;br /&gt;当前 %2 千比特/秒 (质量 %3)&lt;/p&gt;</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="637"/>
        <source>Mumble Server Information</source>
        <translation type="unfinished">Mumble服务器信息</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="641"/>
        <location filename="MainWindow.cpp" line="1394"/>
        <source>&amp;View Certificate</source>
        <translation type="unfinished">&amp;查看认证</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="741"/>
        <source>Kicking player %1</source>
        <translation type="unfinished">踢出玩家%1</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="741"/>
        <location filename="MainWindow.cpp" line="763"/>
        <source>Enter reason</source>
        <translation type="unfinished">输入原因</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="763"/>
        <source>Banning player %1</source>
        <translation type="unfinished">禁止玩家%1</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="785"/>
        <source>Sending message to %1</source>
        <translation type="unfinished">发送消息给%1</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="798"/>
        <location filename="MainWindow.cpp" line="1050"/>
        <source>To %1: %2</source>
        <translation type="unfinished">发送给%1:%2</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="798"/>
        <location filename="MainWindow.cpp" line="1050"/>
        <source>Message to %1</source>
        <translation type="unfinished">发送消息给%1</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="893"/>
        <location filename="MainWindow.cpp" line="936"/>
        <source>Channel Name</source>
        <translation type="unfinished">频道名</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="915"/>
        <source>Are you sure you want to delete %1 and all its sub-channels?</source>
        <translation type="unfinished">你确信要删除%1和它的子频道吗?</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="958"/>
        <source>Change description of channel %1</source>
        <translation type="unfinished">改变频道%1的描叙</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1037"/>
        <source>Sending message to channel %1</source>
        <translation type="unfinished">发送消息给频道%1</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1064"/>
        <source>Sending message to channel tree %1</source>
        <translation type="unfinished">发送消息给频道树%1</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1077"/>
        <source>To tree %1: %2</source>
        <translation type="unfinished">发送给树%1:%2</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1077"/>
        <source>Message to tree %1</source>
        <translation type="unfinished">发送消息给树%1</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1098"/>
        <source>Unmuted and undeafened.</source>
        <translation type="unfinished">未被静音或致聋.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1100"/>
        <source>Unmuted.</source>
        <translation type="unfinished">未被静音.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1102"/>
        <source>Muted.</source>
        <translation type="unfinished">静音.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1122"/>
        <source>Muted and deafened.</source>
        <translation type="unfinished">禁音和致聋.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1124"/>
        <source>Deafened.</source>
        <translation type="unfinished">致聋.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1126"/>
        <source>Undeafened.</source>
        <translation type="unfinished">未被致聋.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1187"/>
        <source>About Qt</source>
        <translation type="unfinished">关于 sQt</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1284"/>
        <source>Joining %1.</source>
        <translation type="unfinished">加入 %1.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1317"/>
        <source>Connected to server %1.</source>
        <translation type="unfinished">连接到服务器%1.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1373"/>
        <source>SSL Verification failed: %1</source>
        <translation type="unfinished">SSL认证失败:%1</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1378"/>
        <source>&lt;b&gt;WARNING:&lt;/b&gt; The server presented a certificate that was different from the stored one.</source>
        <translation type="unfinished">&lt;b&gt;警告:&lt;/b&gt;服务器提供的认证与现有认证不同.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1380"/>
        <source>Sever presented a certificate which failed verification.</source>
        <translation type="unfinished">服务器提供的认证认证失败.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1387"/>
        <source>&lt;p&gt;%1.&lt;br /&gt;The specific errors with this certificate are: &lt;/p&gt;&lt;ol&gt;%2&lt;/ol&gt;&lt;p&gt;Do you wish to accept this certificate anyway?&lt;br /&gt;(It will also be stored so you won&apos;t be asked this again.)&lt;/p&gt;</source>
        <translation type="unfinished">&lt;p&gt;%1.&lt;br /&gt; 认证中的错误是:&lt;/p&gt;&lt;ol&gt;%2&lt;/ol&gt;&lt;p&gt;你希望接受认证吗?&lt;br /&gt;(认证将被保存从而你不会被再次索要认证.)&lt;/p&gt;</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1415"/>
        <source>Server connection failed: %1.</source>
        <translation type="unfinished">连接服务器失败: %1.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1417"/>
        <source>Disconnected from server.</source>
        <translation type="unfinished">与服务器断开连接.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1430"/>
        <source>Invalid username</source>
        <translation type="unfinished">无效用户名</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1430"/>
        <source>You connected with an invalid username, please try another one.</source>
        <translation type="unfinished">你使用一个无效的用户名连接,请尝试一个新的.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1430"/>
        <source>That username is already in use, please try another username.</source>
        <translation type="unfinished">用户名已经被使用，请使用新的用户名.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1435"/>
        <source>Wrong password</source>
        <translation type="unfinished">密码错误</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1435"/>
        <source>Wrong password for registered users, please try again.</source>
        <translation type="unfinished">注册用户密码错误，请重试.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1435"/>
        <source>Wrong server password for unregistered user account, please try again.</source>
        <translation type="unfinished">未注册用户的密码错误，请重试.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="47"/>
        <source>&amp;Server</source>
        <translation type="unfinished">&amp;服务器</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="52"/>
        <source>&amp;Player</source>
        <translation type="unfinished">&amp;玩家</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="57"/>
        <source>&amp;Channel</source>
        <translation type="unfinished">&amp;频道</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="62"/>
        <source>&amp;Audio</source>
        <translation type="unfinished">&amp;音频</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="76"/>
        <source>C&amp;onfigure</source>
        <translation type="unfinished">&amp;配置</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="82"/>
        <source>&amp;Help</source>
        <translation type="unfinished">&amp;帮助</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="104"/>
        <source>Log</source>
        <translation type="unfinished">日志</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="114"/>
        <source>This shows all recent activity. Connecting to servers, errors and information messages all show up here.&lt;br /&gt;To configure exactly which messages show up here, use the &lt;b&gt;Settings&lt;/b&gt; command from the menu.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="123"/>
        <source>&amp;Quit Mumble</source>
        <translation type="unfinished">&amp;退出 Mumble</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="126"/>
        <source>Closes the program</source>
        <translation type="unfinished">关闭程序</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="129"/>
        <source>Exits the application.</source>
        <translation type="unfinished">退出应用程序.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="132"/>
        <source>Ctrl+Q</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="137"/>
        <source>&amp;Connect</source>
        <translation type="unfinished">&amp;连接</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="140"/>
        <source>Open the server connection dialog</source>
        <translation type="unfinished">打开服务器连接对话框</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="143"/>
        <source>Shows a dialog of registered servers, and also allows quick connect.</source>
        <translation type="unfinished">显示注册服务器列表，并允许快速连接.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="151"/>
        <source>&amp;Disconnect</source>
        <translation type="unfinished">&amp;断开</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="154"/>
        <source>Disconnect from server</source>
        <translation type="unfinished">从服务器断开连接</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="157"/>
        <source>Disconnects you from the server.</source>
        <translation type="unfinished">使你从服务器断开连接.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="165"/>
        <source>&amp;Ban lists</source>
        <translation type="unfinished">&amp;黑名单</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="168"/>
        <source>Edit ban lists on server</source>
        <translation type="unfinished">编辑服务器上的黑名单</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="171"/>
        <source>This lets you edit the server-side IP ban lists.</source>
        <translation type="unfinished">让你编辑服务器端的网址黑名单.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="179"/>
        <source>&amp;Information</source>
        <translation type="unfinished">&amp;信息</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="182"/>
        <source>Show information about the server connection</source>
        <translation type="unfinished">显示服务器连接信息</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="185"/>
        <source>This will show extended information about the connection to the server.</source>
        <translation type="unfinished">显示服务器连接的扩展信息.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="190"/>
        <source>&amp;Kick</source>
        <translation type="unfinished">&amp;踢人</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="193"/>
        <source>Kick player (with reason)</source>
        <translation type="unfinished">踢除玩家(理由)</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="196"/>
        <source>Kick selected player off server. You&apos;ll be asked to specify a reason.</source>
        <translation type="unfinished">把指定玩家从服务器踢出.你必须制定一个原因.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="204"/>
        <source>&amp;Mute</source>
        <translation type="unfinished">&amp;静音</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="207"/>
        <source>Mute player</source>
        <translation type="unfinished">静音玩家</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="210"/>
        <source>Mute or unmute player on server. Unmuting a deafened player will also undeafen them.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="215"/>
        <source>&amp;Ban</source>
        <translation type="unfinished">&amp;禁止</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="218"/>
        <source>Kick and ban player (with reason)</source>
        <translation type="unfinished">踢出并禁止玩家（理由）</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="221"/>
        <source>Kick and ban selected player from server. You&apos;ll be asked to specify a reason.</source>
        <translation type="unfinished">从服务器踢出并禁止指定玩家.你需要指定一个原因.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="229"/>
        <source>&amp;Deafen</source>
        <translation type="unfinished">&amp;致聋</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="232"/>
        <source>Deafen player</source>
        <translation type="unfinished">致聋玩家</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="235"/>
        <source>Deafen or undeafen player on server. Deafening a player will also mute them.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="243"/>
        <source>&amp;Local Mute</source>
        <translation type="unfinished">&amp;本地静音</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="246"/>
        <source>Mute player locally</source>
        <translation type="unfinished">本地对玩家静音</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="249"/>
        <source>Mute or unmute player locally. Use this on other players in the same room.</source>
        <translation type="unfinished">本地对玩家静音或者取消静音.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="254"/>
        <location filename="MainWindow.ui" line="517"/>
        <source>Send Messa&amp;ge</source>
        <translation type="unfinished">发送消&amp;息</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="257"/>
        <location filename="MainWindow.ui" line="520"/>
        <source>Send a Text Message</source>
        <translation type="unfinished">发送文本消息</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="260"/>
        <source>Sends a text message to another user.</source>
        <translation type="unfinished">向其他用户发送文本消息.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="265"/>
        <source>&amp;Add</source>
        <translation type="unfinished">&amp;增加</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="268"/>
        <source>Add new channel</source>
        <translation type="unfinished">增加一个新频道</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="271"/>
        <source>This adds a new sub-channel to the currently selected channel.</source>
        <translation type="unfinished">在当前选定频道内添加一个子频道.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="276"/>
        <source>&amp;Remove</source>
        <translation type="unfinished">&amp;移除</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="279"/>
        <source>Remove channel</source>
        <translation type="unfinished">移除频道</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="282"/>
        <source>This removes a channel and all sub-channels.</source>
        <translation type="unfinished">移除一个频道及所有子频道.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="287"/>
        <source>&amp;Edit ACL</source>
        <translation type="unfinished">&amp;编辑 访问控制列表</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="290"/>
        <source>Edit Groups and ACL for channel</source>
        <translation type="unfinished">为频道编辑组和访问控制列表</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="293"/>
        <source>This opens the Group and ACL dialog for the channel, to control permissions.</source>
        <translation type="unfinished">打开频道的组和访问控制列表对话框，控制频道的访问权限.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="298"/>
        <source>&amp;Rename</source>
        <translation type="unfinished">&amp;重命名</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="301"/>
        <source>Renames the channel</source>
        <translation type="unfinished">重命名频道</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="304"/>
        <source>This renames a channel.</source>
        <translation type="unfinished">重命名一个频道.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="309"/>
        <source>&amp;Change Description</source>
        <translation type="unfinished">&amp;改变描叙</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="312"/>
        <source>Changes the channel description</source>
        <translation type="unfinished">改变频道描叙</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="315"/>
        <source>This changes the description of a channel.</source>
        <translation type="unfinished">改变一个频道的描叙.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="320"/>
        <source>&amp;Link</source>
        <translation type="unfinished">&amp;联接</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="323"/>
        <source>Link your channel to another channel</source>
        <translation type="unfinished">联接两个频道</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="326"/>
        <source>This links your current channel to the selected channel. If they have permission to speak in the other channel, players can now hear each other. This is a permanent link, and will last until manually unlinked or the server is restarted. Please see the shortcuts for push-to-link.</source>
        <translation type="unfinished">联接当前频道和所选频道.如果他们有权限在对方频道讲话，玩家能听到对方频道的声音.这是个永久性的联接,直到手动解除联接或者服务器重启.请参看按键联接快捷键.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="331"/>
        <source>&amp;Unlink</source>
        <comment>Channel</comment>
        <translation type="unfinished">&amp;接触联接</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="334"/>
        <source>Unlink your channel from another channel</source>
        <translation type="unfinished">解除两个频道之间的联接</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="337"/>
        <source>This unlinks your current channel from the selected channel.</source>
        <translation type="unfinished">解除当前频道和所选频道之间的联接.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="342"/>
        <source>Unlink &amp;All</source>
        <translation type="unfinished">解除&amp;所有频道间联接</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="345"/>
        <source>Unlinks your channel from all linked channels.</source>
        <translation type="unfinished">解除当前频道和所有频道之间的联接.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="348"/>
        <source>This unlinks your current channel (not the selected one) from all linked channels.</source>
        <translation type="unfinished">解除你的当前频道(非所选频道)和别的频道之间的联接.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="353"/>
        <source>&amp;Reset</source>
        <translation type="unfinished">&amp;重置</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="356"/>
        <source>Reset audio preprocessor</source>
        <translation type="unfinished">重置音频预处理器</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="359"/>
        <source>This will reset the audio preprocessor, including noise cancellation, automatic gain and voice activity detection. If something suddenly worsens the audio environment (like dropping the microphone) and it was temporary, use this to avoid having to wait for the preprocessor to readjust.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="367"/>
        <source>&amp;Mute Self</source>
        <translation type="unfinished">&amp;静音自己</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="370"/>
        <source>Mute yourself</source>
        <translation type="unfinished">对你自己静音</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="373"/>
        <source>Mute or unmute yourself. When muted, you will not send any data to the server. Unmuting while deafened will also undeafen.</source>
        <translation type="unfinished">对自己静音或取消静音. 当静音时，没有数据发送到服务器.当致聋时取消静音将同时取消致聋.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="381"/>
        <source>&amp;Deafen Self</source>
        <translation type="unfinished">&amp;致聋自己</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="384"/>
        <source>Deafen yourself</source>
        <translation type="unfinished">对自己致聋</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="387"/>
        <source>Deafen or undeafen yourself. When deafened, you will not hear anything. Deafening yourself will also mute.</source>
        <translation type="unfinished">对自己致聋或取消致聋.当致聋时,你将听不到声音.致聋自己将同时静音自己.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="395"/>
        <source>&amp;Text-To-Speech</source>
        <translation type="unfinished">&amp;文本语音转换</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="398"/>
        <source>Toggle Text-To-Speech</source>
        <translation type="unfinished">打开/关闭 文本语音转换</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="401"/>
        <source>Enable or disable the text-to-speech engine. Only messages enabled for TTS in the Configuration dialog will actually be spoken.</source>
        <translation type="unfinished">启用或禁用文本语音转换引擎.只有在配置对话框里被启用了文本语音转换的消息才会被读出来.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="406"/>
        <source>S&amp;tatistics</source>
        <translation type="unfinished">&amp;统计</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="409"/>
        <source>Display audio statistics</source>
        <translation type="unfinished">显示音频统计</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="412"/>
        <source>Pops up a small dialog with information about your current audio input.</source>
        <translation type="unfinished">弹出一个关于你当前音频输入信息的对话框.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="417"/>
        <source>&amp;Unlink</source>
        <comment>Plugin</comment>
        <translation type="unfinished">&amp;卸载</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="420"/>
        <source>Forcibly unlink plugin</source>
        <translation type="unfinished">强行卸载插件</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="423"/>
        <source>This forces the current plugin to unlink, which is handy if it is reading completely wrong data.</source>
        <translation type="unfinished">强行卸载当前插件,当插件读取的数据完全错误时相当有用.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="428"/>
        <source>&amp;Settings</source>
        <translation type="unfinished">&amp;设置</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="431"/>
        <source>Configure Mumble</source>
        <translation type="unfinished">配置 Mumble</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="434"/>
        <source>Allows you to change most settings for Mumble.</source>
        <translation type="unfinished">允许你改变Mumble的大多数设置.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="442"/>
        <source>&amp;Audio Wizard</source>
        <translation type="unfinished">&amp;音频精灵</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="445"/>
        <source>Start the audio configuration wizard</source>
        <translation type="unfinished">启动音频配置精灵</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="448"/>
        <source>This will guide you through the process of configuring your audio hardware.</source>
        <translation type="unfinished">音频精灵将引导你配置你的音频硬件.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="453"/>
        <source>&amp;What&apos;s This?</source>
        <translation type="unfinished">&amp;这是什么?</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="456"/>
        <source>Enter What&apos;s This? mode</source>
        <translation type="unfinished">进入 这是什么? 模式</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="459"/>
        <source>Click this to enter &quot;What&apos;s This?&quot; mode. Your cursor will turn into a question mark. Click on any button, menu choice or area to show a description of what it is.</source>
        <translation type="unfinished">点击进入&quot;这是什么?&quot;模式.你的鼠标指针将变成一个问号.点击任意按钮，菜单或区域来显示它是什么的描叙.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="464"/>
        <source>&amp;About</source>
        <translation type="unfinished">&amp;关于</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="467"/>
        <source>Information about Mumble</source>
        <translation type="unfinished">Mumble的相关信息</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="470"/>
        <source>Shows a small dialog with information and license for Mumble.</source>
        <translation type="unfinished">显示一个Mumble相关信息和许可的对话框.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="478"/>
        <source>About &amp;Speex</source>
        <translation type="unfinished">关于 &amp;Speex</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="481"/>
        <source>Information about Speex</source>
        <translation type="unfinished">Speex的相关信息</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="484"/>
        <source>Shows a small dialog with information about Speex.</source>
        <translation type="unfinished">显示一个Speex相关信息的小对话框.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="492"/>
        <source>About &amp;Qt</source>
        <translation type="unfinished">关于 &amp;Qt</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="495"/>
        <source>Information about Qt</source>
        <translation type="unfinished">Qt相关信息</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="498"/>
        <source>Shows a small dialog with information about Qt.</source>
        <translation type="unfinished">显示一个Qt相关信息的小对话框.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="506"/>
        <source>Check for &amp;Updates</source>
        <translation type="unfinished">检查&amp;更新</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="509"/>
        <source>Check for new version of Mumble</source>
        <translation type="unfinished">检查Mumble的新版本</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="512"/>
        <source>Connects to the Mumble webpage to check if a new version is available, and notifies you with an appropriate download URL if this is the case.</source>
        <translation type="unfinished">连接到Mumble网页来检查是否有新版本可用,如果有新版本可用将通知你下载.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="523"/>
        <source>Sends a text message to all users in a channel.</source>
        <translation type="unfinished">向频道内所有用户发送文本信息.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="528"/>
        <source>Send &amp;Tree Message</source>
        <translation type="unfinished">发送信息到频道&amp;树</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="531"/>
        <source>Send message to channel tree.</source>
        <translation type="unfinished">向频道树发送信息.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="534"/>
        <source>This sends a text message to a channel and it&apos;s subchannels.</source>
        <translation type="unfinished">向一个频道及其所有子频道发送文本消息.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="542"/>
        <source>&amp;Minimal View</source>
        <translation type="unfinished">&amp;简化窗口</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="545"/>
        <source>Toggle minimal window modes</source>
        <translation type="unfinished">打开/关闭 简化窗口模式</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="548"/>
        <source>This will toggle minimal mode, where the log window and menu is hidden.</source>
        <translation type="unfinished">打开简化窗口模式将隐藏日志窗口和菜单.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="57"/>
        <source>Joined server: %1.</source>
        <translation type="unfinished">进入服务器: %1.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="77"/>
        <source>Left server: %1.</source>
        <translation type="unfinished">离开服务器: %1.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="110"/>
        <source>%1 is now muted and deafened.</source>
        <translation type="unfinished">%1 被静音和致聋.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="112"/>
        <source>%1 is now muted.</source>
        <translation type="unfinished">%1 被静音.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="114"/>
        <source>%1 is now unmuted.</source>
        <translation type="unfinished">%1被取消静音.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="127"/>
        <location filename="Messages.cpp" line="147"/>
        <source>server</source>
        <translation type="unfinished">服务器</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="130"/>
        <source>You were muted by %1.</source>
        <translation type="unfinished">你被%1静音.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="130"/>
        <source>You were unmuted by %1.</source>
        <translation type="unfinished">你被%1取消静音.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="133"/>
        <source>%1 muted by %2.</source>
        <translation type="unfinished">%1被%2静音.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="133"/>
        <source>%1 unmuted by %2.</source>
        <translation type="unfinished">%1被%2取消静音.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="150"/>
        <source>You were deafened by %1.</source>
        <translation type="unfinished">你被%1致聋.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="150"/>
        <source>You were undeafened by %1.</source>
        <translation type="unfinished">你被%1取消致聋.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="153"/>
        <source>%1 deafened by %2.</source>
        <translation type="unfinished">%1被%2致聋.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="153"/>
        <source>%1 undeafened by %2.</source>
        <translation type="unfinished">%1被%2取消致聋.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="163"/>
        <source>You were kicked from the server by %1: %2.</source>
        <translation type="unfinished">你被%1从服务器踢出: %2.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="167"/>
        <source>%3 was kicked from the server by %1: %2.</source>
        <translation type="unfinished">%3被%1从服务器踢出: %2.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="176"/>
        <source>You were kicked and banned from the server by %1: %2.</source>
        <translation type="unfinished">你被%1从服务器踢出并查封: %2.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="180"/>
        <source>%3 was kicked and banned from the server by %1: %2.</source>
        <translation type="unfinished">%3被%1从服务器踢出并查封: %2.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="207"/>
        <source>You were moved to %1 by %2.</source>
        <translation type="unfinished">你被从%1移动到%2.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="210"/>
        <source>%1 left channel.</source>
        <translation type="unfinished">%1离开频道.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="212"/>
        <source>%1 moved out by %2 to %3.</source>
        <translation type="unfinished">%1被%2移动到%3.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="222"/>
        <source>%1 moved in by %2 from %3.</source>
        <translation type="unfinished">%1被%2从%3移进.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="220"/>
        <source>%1 entered channel.</source>
        <translation type="unfinished">%1进入频道.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="320"/>
        <source>Server connection rejected: %1.</source>
        <translation type="unfinished">服务器拒绝连接: %1.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="325"/>
        <source>Denied: %1.</source>
        <translation type="unfinished">拒绝: %1.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="341"/>
        <source>Server maximum bandwidth is only %1 kbit/s. Quality auto-adjusted.</source>
        <translation type="unfinished">服务器最大带宽是%1千比特/秒.自动调整质量.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="352"/>
        <source>From %1: %2</source>
        <translation type="unfinished">发送人 %1: %2</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="353"/>
        <source>Message from %1</source>
        <translation type="unfinished">来至%1的消息</translation>
    </message>
    <message>
        <location filename="PlayerModel.cpp" line="996"/>
        <source>You have Channel Dragging set to &quot;Do Nothing&quot; so the channel wasn&apos;t moved.</source>
        <translation type="unfinished">你设置频道拖动为“什么都不做&quot;因此频道将不被移动.</translation>
    </message>
    <message>
        <location filename="PlayerModel.cpp" line="1002"/>
        <source>Unknown Channel Drag mode in PlayerModel::dropMimeData.</source>
        <translation type="unfinished">未知的频道拖动模式 PlayerModel::dropMimeData.</translation>
    </message>
    <message>
        <location filename="main.cpp" line="200"/>
        <source>Welcome to Mumble.</source>
        <translation type="unfinished">欢迎来到Mumble.</translation>
    </message>
    <message>
        <location filename="main.cpp" line="217"/>
        <source>This is the first time you&apos;re starting Mumble.&lt;br /&gt;Would you like to go through the Audio Wizard to configure your soundcard?</source>
        <translation type="unfinished">这是你第一次启动Mumble.&lt;br /&gt;你想用音频精灵配置声卡吗?</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="351"/>
        <source>Server</source>
        <comment>message from</comment>
        <translation type="unfinished">服务器</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="556"/>
        <source>Hide Frame</source>
        <translation type="unfinished">隐藏边框</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="559"/>
        <source>Toggle showing frame on minimal window</source>
        <translation type="unfinished">在简化窗口模式时打开/关闭边框显示</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="562"/>
        <source>This will toggle whether the minimal window should have a frame for moving and resizing.</source>
        <translation type="unfinished">该选项确定简化窗口是否有个用来移动和改变大小的边框.</translation>
    </message>
</context>
<context>
    <name>NetworkConfig</name>
    <message>
        <location filename="NetworkConfig.cpp" line="47"/>
        <source>Network</source>
        <translation type="unfinished">网络</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="13"/>
        <source>Form</source>
        <translation type="unfinished">表单</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="25"/>
        <source>Connection</source>
        <translation type="unfinished">连接</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="31"/>
        <source>Use TCP compatibility mode</source>
        <oldsource>Use TCP compatability mode</oldsource>
        <translation type="unfinished">使用TCP兼容模式</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="34"/>
        <source>&lt;b&gt;Enable TCP compatibility mode&lt;/b&gt;.&lt;br /&gt;This will make Mumble use only TCP when communicating with the server. This will increase overhead and cause lost packets to produce noticeable pauses in communication, so this should only be used if you are unable to use the default (which uses UDP for voice and TCP for control).</source>
        <translation type="unfinished">&lt;b&gt;允许TCP兼容模式&lt;/b&gt;.&lt;br/&gt;这将使Mumble仅使用TCP与服务器进行通信.这将增加系统开销并导致丢包从而使通信暂停,请你只有在不能使用默认模式时使用它(默认模式使用UDP来传输语音，TCP来控制).</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="37"/>
        <source>Use TCP mode</source>
        <translation type="unfinished">使用TCP模式</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="44"/>
        <source>Reconnect when disconnected</source>
        <translation type="unfinished">断开连接时重新连接</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="47"/>
        <source>&lt;b&gt;Reconnect when disconnected&lt;/b&gt;.&lt;br /&gt;This will make Mumble try to automatically reconnect after 10 seconds if your server connection fails.</source>
        <translation type="unfinished">&lt;b&gt;断开连接时重新连接&lt;/b&gt;.&lt;br/&gt;这将使Mumble在与服务器断开连接10秒后自动重连.</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="50"/>
        <source>Reconnect automatically</source>
        <translation type="unfinished">自动重新连接</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="66"/>
        <source>Proxy</source>
        <translation type="unfinished">代理</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="72"/>
        <source>Type</source>
        <translation type="unfinished">类型</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="85"/>
        <source>Type of proxy to connect through</source>
        <translation type="unfinished">代理类型</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="88"/>
        <source>&lt;b&gt;Type of proxy to connect through&lt;/b&gt;&lt;br /&gt;This makes Mumble connect through a proxy for all outgoing connections. Note: Proxy tunneling forces Mumble into TCP compatibility mode, causing all voice data to be sent via the control channel.</source>
        <oldsource>&lt;b&gt;Type of proxy to connect through&lt;/b&gt;&lt;br /&gt;This makes Mumble connect through a proxy for all outgoing connections. Note: Proxy tunneling forces Mumble into TCP compatability mode, causing all voice data to be sent via the control channel.</oldsource>
        <translation type="unfinished">&lt;b&gt;代理类型&lt;/b&gt;&lt;br/&gt;使Mumble通过代理来连接到服务器.代理将迫使Mumble使用TCP兼容模式，导致所有语音数据都通过控制频道传输.</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="92"/>
        <source>Direct connection</source>
        <translation type="unfinished">直接连接</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="97"/>
        <source>HTTP(S) proxy</source>
        <translation type="unfinished">HTTP(S) 代理</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="102"/>
        <source>SOCKS5 proxy</source>
        <translation type="unfinished">SOCKS5 代理</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="110"/>
        <source>Hostname</source>
        <translation type="unfinished">主机名</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="126"/>
        <source>Hostname of the proxy</source>
        <translation type="unfinished">代理主机名</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="129"/>
        <source>&lt;b&gt;Hostname of the proxy.&lt;b&gt;&lt;br /&gt;This field specifies the hostname of the proxy you wish to tunnel network traffic through.</source>
        <translation type="unfinished">&lt;b&gt;代理主机名.&lt;/b&gt;&lt;br /&gt;指定你希望用来穿隧传输网络流量的代理主机名.</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="151"/>
        <source>Port</source>
        <translation type="unfinished">端口</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="176"/>
        <source>Port number of the proxy</source>
        <translation type="unfinished">代理端口号</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="179"/>
        <source>&lt;b&gt;Port number of the proxy.&lt;/b&gt;&lt;br /&gt;This field specifies the port number that the proxy expects connections on.</source>
        <translation type="unfinished">&lt;b&gt;代理服务器端口号.&lt;/b&gt;&lt;br/&gt;指定代理服务器使用的端口号.</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="195"/>
        <source>Username</source>
        <translation type="unfinished">用户名</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="202"/>
        <source>Username for proxy authentication</source>
        <translation type="unfinished">用于代理认证的用户名</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="205"/>
        <source>&lt;b&gt;Username for proxy authentication.&lt;/b&gt;&lt;br /&gt;This specifies the username you use for authenticating yourself with the proxy. In case the proxy does not use authentication, or you want to connect anonymously, simply leave this field blank.</source>
        <translation type="unfinished">&lt;b&gt;用于代理认证的用户名.&lt;/b&gt;&lt;br/&gt;指定用于代理认证的用户名.如果代理不需要认证，或者你想要使用匿名连接，请将此字段留白.</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="212"/>
        <source>Password</source>
        <translation type="unfinished">密码</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="219"/>
        <source>Password for proxy authentication</source>
        <translation type="unfinished">用于代理认证的密码</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="222"/>
        <source>&lt;b&gt;Password for proxy authentication.&lt;/b&gt;&lt;br /&gt;This specifies the password you use for authenticating yourself with the proxy. In case the proxy does not use authentication, or you want to connect anonymously, simply leave this field blank.</source>
        <translation type="unfinished">&lt;b&gt;用于代理认证的密码.&lt;/b&gt;&lt;br/&gt;指定用于代理认证的密码.如果代理不需要认证，或者你想要使用匿名连接，请将此字段留白.</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="241"/>
        <source>Mumble services</source>
        <translation type="unfinished">Mumble服务</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="247"/>
        <source>Check for updates on startup</source>
        <translation type="unfinished">启动时检测更新</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="254"/>
        <source>Submit anonymous statistics to the Mumble project</source>
        <translation type="unfinished">递交匿名统计数据给Mumble项目</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="257"/>
        <source>&lt;b&gt;Submit anonymous statistics.&lt;/b&gt;&lt;br /&gt;Mumble has a small development team, and as such needs to focus it&apos;s development where it&apos;s needed most. By submitting a bit of statistics you help the project determine where to focus development.</source>
        <translation type="unfinished">&lt;b&gt;递交匿名统计数据.&lt;/b&gt;&lt;br/&gt;Mumble有一个小的开发团队,并需要专注于用户最想要的功能.通过递交匿名统计数据你将帮助开发团队确定重点开发什么.</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="260"/>
        <source>Submit anonymous statistics</source>
        <translation type="unfinished">递交匿名统计数据</translation>
    </message>
</context>
<context>
    <name>Overlay</name>
    <message>
        <location filename="Overlay.cpp" line="194"/>
        <source>Mumble</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Overlay.cpp" line="194"/>
        <source>Failed to load overlay library. This means either that:
- the library (mumble_ol.dll) wasn&apos;t found in the directory you ran Mumble from
- you&apos;re on an OS earlier than WinXP SP2
- you do not have the June 2007 updated version of DX9.0c</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>OverlayConfig</name>
    <message>
        <location filename="Overlay.cpp" line="47"/>
        <source>Show no one</source>
        <translation type="unfinished">不显示任何人</translation>
    </message>
    <message>
        <location filename="Overlay.cpp" line="48"/>
        <source>Show only talking</source>
        <translation type="unfinished">当发言时显示</translation>
    </message>
    <message>
        <location filename="Overlay.cpp" line="49"/>
        <source>Show everyone</source>
        <translation type="unfinished">显示所有人</translation>
    </message>
    <message>
        <location filename="Overlay.cpp" line="154"/>
        <source>Overlay</source>
        <translation type="unfinished">重叠</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="13"/>
        <source>Form</source>
        <translation type="unfinished">表单</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="19"/>
        <source>Options</source>
        <translation type="unfinished">选项</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="25"/>
        <source>Enable overlay.</source>
        <translation type="unfinished">允许重叠.</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="28"/>
        <source>This sets whether the overlay is enabled or not. This setting is only checked with D3D9 applications are started, so make sure Mumble is running and this option is on before you start the application.&lt;br /&gt;Please note that if you start the application after starting Mumble, or if you disable the overlay while running, there is no safe way to restart the overlay without also restarting the application.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="31"/>
        <source>Enable Overlay</source>
        <translation type="unfinished">允许重叠</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="38"/>
        <source>Who to show on the overlay</source>
        <translation type="unfinished">在重叠中显示谁</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="41"/>
        <source>&lt;b&gt;This sets who to show in the in-game overlay.&lt;/b&gt;&lt;br /&gt;If many people are connected to the same channel, the overlay list might be very long. Use this to shorten it.&lt;br /&gt;&lt;i&gt;No one&lt;/i&gt; - Don&apos;t show anyone (but leave overlay running).&lt;br /&gt;&lt;i&gt;Only talking&lt;/i&gt; - Only show talking people.&lt;br /&gt;&lt;i&gt;Everyone&lt;/i&gt; - Show everyone.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="48"/>
        <source>Always show yourself on overlay.</source>
        <translation type="unfinished">在重叠中总是显示自己.</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="51"/>
        <source>This sets whether to always show yourself or not. This setting is useful if you aren&apos;t showing everyone in the overlay, as then you would only see your own status if you were talking, which wouldn&apos;t let you see that you were deafened or muted.</source>
        <translation type="unfinished">设置是否总是显示你自己.这个选项只有当你不在重叠中显示所有人时才有效,你在讲话时只能看到自己的状态,你将看不到自己被禁音或致聋.</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="54"/>
        <source>Always Show Self</source>
        <translation type="unfinished">总是显示自己</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="61"/>
        <source>Show User custom textures instead of text on the overlay.</source>
        <translation type="unfinished">在重叠时显示用户自定义文本。</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="64"/>
        <source>This sets whether to download and use custom textures for registered users. If disabled, the regular outline text will be used instead.</source>
        <translation type="unfinished">设置是否对注册用户下载并使用定制文本.如果禁用该选项,普通文本将被使用.</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="67"/>
        <source>Show User Textures</source>
        <translation type="unfinished">显示用户文本</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="77"/>
        <source>Position</source>
        <translation type="unfinished">位置</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="83"/>
        <source>Let overlay grow upwards</source>
        <translation type="unfinished">让重叠往上增长</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="86"/>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow upwards if needed.</source>
        <translation type="unfinished">重叠尝试保持在你选定的位置.这个选项允许重叠在需要时往上增长。</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="89"/>
        <source>Grow Up</source>
        <translation type="unfinished">往上增长</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="96"/>
        <source>Y-Position of Overlay</source>
        <translation type="unfinished">重叠的纵坐标</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="99"/>
        <source>This sets the relative Y position of the overlay.</source>
        <translation type="unfinished">设置重叠的相对纵坐标.</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="112"/>
        <source>Let overlay grow to the left</source>
        <translation type="unfinished">让重叠往左增长</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="115"/>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow to the left if needed.</source>
        <translation type="unfinished">重叠尝试保持在你选定的位置.这个选项允许重叠在需要时往左增长。</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="118"/>
        <source>Grow Left</source>
        <translation type="unfinished">往左增长</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="125"/>
        <source>Let overlay grow to the right</source>
        <translation type="unfinished">让重叠往右增长</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="128"/>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow to the right if needed.</source>
        <translation type="unfinished">重叠尝试保持在你选定的位置.这个选项允许重叠在需要时往右增长。</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="131"/>
        <source>Grow Right</source>
        <translation type="unfinished">往右增长</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="138"/>
        <source>Let overlay grow downwards</source>
        <translation type="unfinished">让重叠往下增长</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="141"/>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow downwards if needed.</source>
        <translation type="unfinished">重叠尝试保持在你选定的位置.这个选项允许重叠在需要时往下增长。</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="144"/>
        <source>Grow Down</source>
        <translation type="unfinished">往下增长</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="151"/>
        <source>X-Position of Overlay</source>
        <translation type="unfinished">重叠横坐标</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="154"/>
        <source>This sets the relative X position of the overlay.</source>
        <translation type="unfinished">设置重叠的相对横坐标.</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="170"/>
        <source>Font</source>
        <translation type="unfinished">字体</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="176"/>
        <source>Current Font</source>
        <translation type="unfinished">当前字体</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="183"/>
        <location filename="Overlay.ui" line="223"/>
        <location filename="Overlay.ui" line="237"/>
        <location filename="Overlay.ui" line="258"/>
        <location filename="Overlay.ui" line="279"/>
        <location filename="Overlay.ui" line="300"/>
        <location filename="Overlay.ui" line="321"/>
        <source>TextLabel</source>
        <translation type="unfinished">文本标签</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="190"/>
        <source>Set Font</source>
        <translation type="unfinished">设置字体</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="197"/>
        <source>Maximum height</source>
        <translation type="unfinished">最大高度</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="204"/>
        <source>Maximum height of names.</source>
        <translation type="unfinished">名字的最大高度.</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="207"/>
        <source>This sets the maximum height of names shown, relative to the screen height. If your active 3D window is 800 pixels tall and this is set to 5%, each name will be 40 pixels tall. Note that the names will not be taller than 60 pixels no matter what you set here.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="230"/>
        <source>Color for players</source>
        <translation type="unfinished">玩家颜色</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="244"/>
        <location filename="Overlay.ui" line="265"/>
        <location filename="Overlay.ui" line="286"/>
        <location filename="Overlay.ui" line="307"/>
        <location filename="Overlay.ui" line="328"/>
        <source>Change</source>
        <translation type="unfinished">改变</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="251"/>
        <source>Color for talking players</source>
        <translation type="unfinished">正在讲话玩家的颜色</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="272"/>
        <source>Color for alt-talking players</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="293"/>
        <source>Color for Channels</source>
        <translation type="unfinished">频道的颜色</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="314"/>
        <source>Color for active Channels</source>
        <translation type="unfinished">活动频道的颜色</translation>
    </message>
</context>
<context>
    <name>PlayerModel</name>
    <message>
        <location filename="PlayerModel.cpp" line="421"/>
        <source>This is a player connected to the server. The icon to the left of the player indicates whether or not they are talking:&lt;br /&gt;&lt;img src=&quot;skin:talking_on.png&quot; /&gt; Talking&lt;br /&gt;&lt;img src=&quot;skin:talking_off.png&quot; /&gt; Not talking</source>
        <translation type="unfinished">这是一个连接到服务器的玩家.玩家左边的图标显示他是否在讲话.&lt;br/&gt;&lt;img src=&quot;skin:talking_on.png&quot; /&gt;发言中&lt;br /&gt;&lt;img src=&quot;skin:talking_off.png&quot; /&gt; 沉默中</translation>
    </message>
    <message>
        <location filename="PlayerModel.cpp" line="427"/>
        <source>This is a channel on the server. Only players in the same channel can hear each other.</source>
        <translation type="unfinished">这是一个服务器上的频道.只有同一个频道的玩家能听到别人的声音.</translation>
    </message>
    <message>
        <location filename="PlayerModel.cpp" line="429"/>
        <source>This shows the flags the player has on the server, if any:&lt;br /&gt;&lt;img src=&quot;skin:authenticated.png&quot; /&gt;Authenticated user&lt;br /&gt;&lt;img src=&quot;skin:muted_self.png&quot; /&gt;Muted (by self)&lt;br /&gt;&lt;img src=&quot;skin:muted_server.png&quot; /&gt;Muted (by admin)&lt;br /&gt;&lt;img src=&quot;skin:deafened_self.png&quot; /&gt;Deafened (by self)&lt;br /&gt;&lt;img src=&quot;skin:deafened_server.png&quot; /&gt;Deafened (by admin)&lt;br /&gt;A player muted by himself is probably just away, talking on the phone or something like that.&lt;br /&gt;A player muted by an admin is probably also just away, and the noise the player is making was annoying enough that an admin muted him.</source>
        <translation type="unfinished">显示该玩家在服务器上的标志:&lt;br /&gt;&lt;img src=&quot;skin:authenticated.png&quot; /&gt;认证的用户&lt;br /&gt;&lt;img src=&quot;skin:muted_self.png&quot; /&gt;静音(被自己)&lt;br /&gt;&lt;img src=&quot;skin:muted_server.png&quot; /&gt;静音(被管理员)&lt;br /&gt;&lt;img src=&quot;skin:deafened_self.png&quot; /&gt;致聋 (被自己)&lt;br /&gt;&lt;img src=&quot;skin:deafened_server.png&quot; /&gt;致聋 (被管理员)&lt;br /&gt;玩家被自己禁音有可能是他处于离开状态，比如在接电话.&lt;br /&gt;玩家被管理员禁音也有可能是他处于离开状态，但是他发出了让管理员恼怒到禁音他的噪音.</translation>
    </message>
    <message>
        <location filename="PlayerModel.cpp" line="453"/>
        <source>Name</source>
        <translation type="unfinished">名字</translation>
    </message>
    <message>
        <location filename="PlayerModel.cpp" line="455"/>
        <source>Flags</source>
        <translation type="unfinished">标志</translation>
    </message>
    <message>
        <location filename="PlayerModel.cpp" line="990"/>
        <source>Mumble</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="PlayerModel.cpp" line="990"/>
        <source>Are you sure you want to drag this channel?</source>
        <translation type="unfinished">你确信要拖动这个频道吗?</translation>
    </message>
</context>
<context>
    <name>PluginConfig</name>
    <message>
        <location filename="Plugins.cpp" line="64"/>
        <location filename="Plugins.ui" line="41"/>
        <source>Plugins</source>
        <translation type="unfinished">插件</translation>
    </message>
    <message>
        <location filename="Plugins.cpp" line="92"/>
        <location filename="Plugins.cpp" line="104"/>
        <source>Mumble</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Plugins.cpp" line="92"/>
        <source>Plugin has no configure function.</source>
        <translation type="unfinished">插件没有配置选项.</translation>
    </message>
    <message>
        <location filename="Plugins.cpp" line="104"/>
        <source>Plugin has no about function.</source>
        <translation type="unfinished">插件没有关于选项.</translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="13"/>
        <source>Form</source>
        <translation type="unfinished">表单</translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="19"/>
        <source>Options</source>
        <translation type="unfinished">选项</translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="25"/>
        <source>Enable plugins and transmit positional information</source>
        <translation type="unfinished">允许插件传输位置信息</translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="28"/>
        <source>This enables plugins for supported games to fetch your in-game position and transmit that with each voice packet. This enables other players to hear your voice in-game from the direction your character is in relation to their own.</source>
        <translation type="unfinished">允许插件在支持的游戏中获取你的位置并在每个语音包中传输.这将使别的玩家能根据你的声音来判断你在游戏中的方位.</translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="31"/>
        <source>Link to Game and Transmit Position</source>
        <translation type="unfinished">链接到游戏并传输位置</translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="52"/>
        <source>Reloads all plugins</source>
        <translation type="unfinished">重载所有插件</translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="55"/>
        <source>This rescans and reloads plugins. Use this if you just added or changed a plugin to the plugins directory.</source>
        <translation type="unfinished">重新扫描和重载插件.当你添加或改变一个插件目录中的插件时使用该选项.</translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="58"/>
        <source>&amp;Reload plugins</source>
        <translation type="unfinished">&amp;重载插件</translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="78"/>
        <source>Information about plugin</source>
        <translation type="unfinished">插件信息</translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="81"/>
        <source>This shows a small information message about the plugin.</source>
        <translation type="unfinished">显示插件相关信息.</translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="84"/>
        <source>&amp;About</source>
        <translation type="unfinished">&amp;关于</translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="91"/>
        <source>Show configuration page of plugin</source>
        <translation type="unfinished">显示插件配置页面</translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="94"/>
        <source>This shows the configuration page of the plugin, if any.</source>
        <translation type="unfinished">显示插件配置页面.</translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="97"/>
        <source>&amp;Configure</source>
        <translation type="unfinished">&amp;配置</translation>
    </message>
</context>
<context>
    <name>Plugins</name>
    <message>
        <location filename="Plugins.cpp" line="223"/>
        <source>%1 lost link.</source>
        <translation type="unfinished">%1 失去链接.</translation>
    </message>
    <message>
        <location filename="Plugins.cpp" line="262"/>
        <source>%1 linked.</source>
        <translation type="unfinished">%1 链接上.</translation>
    </message>
</context>
<context>
    <name>PortAudioSystem</name>
    <message>
        <location filename="PAAudio.cpp" line="275"/>
        <source>Default Device</source>
        <translation type="unfinished">默认设备</translation>
    </message>
</context>
<context>
    <name>PulseAudioSystem</name>
    <message>
        <location filename="PulseAudio.cpp" line="531"/>
        <source>Default Input</source>
        <translation type="unfinished">默认输入</translation>
    </message>
    <message>
        <location filename="PulseAudio.cpp" line="532"/>
        <source>Default Output</source>
        <translation type="unfinished">默认输出</translation>
    </message>
</context>
<context>
    <name>ShortcutActionWidget</name>
    <message>
        <location filename="GlobalShortcut.cpp" line="124"/>
        <source>Unassigned</source>
        <translation type="unfinished">未分配</translation>
    </message>
</context>
<context>
    <name>ShortcutDelegate</name>
    <message>
        <location filename="GlobalShortcut.cpp" line="182"/>
        <source>Unassigned</source>
        <translation type="unfinished">未分配</translation>
    </message>
</context>
<context>
    <name>ShortcutKeyWidget</name>
    <message>
        <location filename="GlobalShortcut.cpp" line="60"/>
        <source>Press Shortcut</source>
        <translation type="unfinished">输入快捷键</translation>
    </message>
</context>
<context>
    <name>TextMessage</name>
    <message>
        <location filename="TextMessage.ui" line="16"/>
        <source>Send raw message</source>
        <translation type="unfinished">发送原始消息</translation>
    </message>
    <message>
        <location filename="TextMessage.ui" line="19"/>
        <source>Disables html formating</source>
        <translation type="unfinished">禁用超文本语言格式化</translation>
    </message>
    <message>
        <location filename="TextMessage.ui" line="32"/>
        <source>Message to send</source>
        <translation type="unfinished">要发送的消息</translation>
    </message>
    <message>
        <location filename="TextMessage.ui" line="50"/>
        <source>Preview</source>
        <translation type="unfinished">预览</translation>
    </message>
</context>
<context>
    <name>VersionCheck</name>
    <message>
        <location filename="VersionCheck.cpp" line="84"/>
        <source>Mumble failed to retrieve version information from the SourceForge server.</source>
        <translation type="unfinished">Mumble无法从SouceForge服务器取得版本信息.</translation>
    </message>
</context>
<context>
    <name>ViewCert</name>
    <message>
        <location filename="ViewCert.cpp" line="36"/>
        <source>Certificate Chain Details</source>
        <translation type="unfinished">认证链详细信息</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="42"/>
        <source>Certificate chain</source>
        <translation type="unfinished">认证链</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="48"/>
        <source>%1 %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="51"/>
        <source>Certificate details</source>
        <translation type="unfinished">认证消息信息</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="76"/>
        <location filename="ViewCert.cpp" line="89"/>
        <source>Common Name: %1</source>
        <translation type="unfinished">通用名: %1</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="77"/>
        <location filename="ViewCert.cpp" line="90"/>
        <source>Organization: %1</source>
        <translation type="unfinished">组织: %1</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="78"/>
        <source>Subunit: %1</source>
        <translation type="unfinished">部门: %1</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="79"/>
        <location filename="ViewCert.cpp" line="92"/>
        <source>Country: %1</source>
        <translation type="unfinished">国家: %1</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="80"/>
        <location filename="ViewCert.cpp" line="93"/>
        <source>Locality: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="81"/>
        <location filename="ViewCert.cpp" line="94"/>
        <source>State: %1</source>
        <translation type="unfinished">省(市): %1</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="82"/>
        <source>Valid from: %1</source>
        <translation type="unfinished">有效期始于: %1</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="83"/>
        <source>Valid to: %1</source>
        <translation type="unfinished">有效期到: %1</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="84"/>
        <source>Serial: %1</source>
        <translation type="unfinished">序列号: %1</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="85"/>
        <source>Public Key: %1 bits %2</source>
        <translation type="unfinished">公开密钥: %1 比特 %2</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="85"/>
        <source>RSA</source>
        <translation type="unfinished">非对称加密算法</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="85"/>
        <source>DSA</source>
        <translation type="unfinished">数字签名算法</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="86"/>
        <source>Digest (MD5): %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="88"/>
        <source>Issued by:</source>
        <translation type="unfinished">发布者:</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="91"/>
        <source>Unit Name: %1</source>
        <translation type="unfinished">部门名: %1</translation>
    </message>
</context>
<context>
    <name>WASAPISystem</name>
    <message>
        <location filename="WASAPI.cpp" line="202"/>
        <source>Default Device</source>
        <translation type="unfinished">默认设备</translation>
    </message>
</context>
</TS>
