<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.0" language="zh_CN">
<context>
    <name>ACLEditor</name>
    <message>
        <source>Mumble - Edit ACL for %1</source>
        <translation type="obsolete">Mumble - 编辑 访问控制列表 %1</translation>
    </message>
    <message>
        <location filename="ACLEditor.cpp" line="111"/>
        <source>Deny</source>
        <translation>拒绝</translation>
    </message>
    <message>
        <location filename="ACLEditor.cpp" line="113"/>
        <source>Allow</source>
        <translation>允许</translation>
    </message>
    <message>
        <location filename="ACLEditor.cpp" line="135"/>
        <source>Allow %1</source>
        <translation>允许 %1</translation>
    </message>
    <message>
        <location filename="ACLEditor.cpp" line="126"/>
        <source>Deny %1</source>
        <translation>拒绝 %1</translation>
    </message>
    <message>
        <location filename="ACLEditor.cpp" line="54"/>
        <source>Mumble - Add channel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ACLEditor.cpp" line="81"/>
        <location filename="ACLEditor.cpp" line="249"/>
        <source>Failed: Invalid channel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ACLEditor.cpp" line="99"/>
        <source>Mumble - Edit %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ACLEditor.cpp" line="136"/>
        <source>This grants the %1 privilege. If a privilege is both allowed and denied, it is denied.&lt;br /&gt;%2</source>
        <translation>授予 %1 权限.如果一个权限同时被拒绝和允许，则被视为拒绝。&lt;br /&gt;%2</translation>
    </message>
    <message>
        <location filename="ACLEditor.cpp" line="127"/>
        <source>This revokes the %1 privilege. If a privilege is both allowed and denied, it is denied.&lt;br /&gt;%2</source>
        <translation>撤销 %1 权限.如果一个权限同时被拒绝和允许，则被视为拒绝。&lt;br /&gt;%2</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="14"/>
        <source>Dialog</source>
        <translation>对话框</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="24"/>
        <source>Properties</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="33"/>
        <source>Name</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="40"/>
        <source>Enter the channel name here.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="43"/>
        <source>&lt;b&gt;Name&lt;/b&gt;&lt;br&gt;Enter the channel name in this field. The name has to comply with the restriction imposed by the server you are connected to.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="50"/>
        <source>Description</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="57"/>
        <source>Password</source>
        <translation type="unfinished">密码</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="64"/>
        <source>Enter the channel password here.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="67"/>
        <source>&lt;b&gt;Password&lt;/b&gt;&lt;br&gt;This field allows you to easily set and change the password of a channel. In the background it uses Mumble&apos;s access tokens feature. To allow more fine grained and powerful access control directly use ACLs and groups instead (&lt;i&gt;Advanced configuration&lt;/i&gt; has to be checked to be able to see these settings).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="74"/>
        <source>Check to create a temporary channel.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="77"/>
        <source>&lt;b&gt;Temporary&lt;/b&gt;&lt;br&gt;
When checked the channel created will be marked as temporary. This means when the last player leaves it the channel will be automatically deleted by the server.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="81"/>
        <source>Temporary</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="94"/>
        <source>Channel positioning facility value</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="97"/>
        <source>&lt;b&gt;Position&lt;/b&gt;&lt;br/&gt;
This value enables you to change the way Mumble arranges the channels in the tree. A channel with a higher &lt;i&gt;Position&lt;/i&gt; value will always be placed below one with a lower value and the other way around. If the &lt;i&gt;Position&lt;/i&gt; value of two channels is equal they will get sorted alphabetically by their name.</source>
        <oldsource>&lt;b&gt;Position&lt;/b&gt;&lt;br/&gt;
This value enables you to change the way mumble arranges the channels in the tree. A channel with a higher &lt;i&gt;Position&lt;/i&gt; value will always be placed below one with a lower value and the other way around. If the &lt;i&gt;Position&lt;/i&gt; value of two channels is equal they will get sorted alphabetically by their name.</oldsource>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="108"/>
        <source>Position</source>
        <translation type="unfinished">位置</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="119"/>
        <source>&amp;Groups</source>
        <translation>&amp;群</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="125"/>
        <location filename="ACLEditor.ui" line="553"/>
        <source>Group</source>
        <translation>群</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="149"/>
        <source>List of groups</source>
        <translation>群列表</translation>
    </message>
    <message>
        <source>This is all the groups currently defined for the channel. To create a new group, just type in the name and press enter.</source>
        <translation type="obsolete">这是当前频道内定义的所有群。要创建一个新群，请输入群名然后按回车。</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="177"/>
        <source>Remove selected group</source>
        <translation>删除选中的群</translation>
    </message>
    <message>
        <source>This removes the currently selected group. If the group was inherited, it will not be removed from the list, but all local information about the group will be cleared.</source>
        <translation type="obsolete">删除当前选中的群。如果该群有子群，它将不会被从列表中删除，但是与该群相关的所有本地信息将被清除.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="183"/>
        <location filename="ACLEditor.ui" line="361"/>
        <location filename="ACLEditor.ui" line="368"/>
        <source>Remove</source>
        <translation>删除</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="193"/>
        <source>Inherit group members from parent</source>
        <translation>从父继承群成员</translation>
    </message>
    <message>
        <source>This inherits all the members in the group from the parent, if the group is marked as &lt;i&gt;Inheritable&lt;/i&gt; in the parent channel.</source>
        <translation type="obsolete">如果群在父频道里被标记为&lt;i&gt;可继承&lt;/i&gt;,则从父继承所有群成员。</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="199"/>
        <source>Inherit</source>
        <translation>继承</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="206"/>
        <source>Make group inheritable to sub-channels</source>
        <translation>使群可被子频道继承</translation>
    </message>
    <message>
        <source>This makes this group inheritable to sub-channels. If the group is non-inheritable, sub-channels are still free to create a new group with the same name.</source>
        <translation type="obsolete">使该群可被子频道继承。如果该群不可被继承，则可在子频道内创建同名新群。</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="212"/>
        <source>Inheritable</source>
        <translation>可继承</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="222"/>
        <source>Group was inherited from parent channel</source>
        <translation>继承自父频道的群</translation>
    </message>
    <message>
        <source>This indicates that the group was inherited from the parent channel. You cannot edit this flag, it&apos;s just for information.</source>
        <translation type="obsolete">这表明该群继承自父频道。该标志仅供参考，你不能编辑该标志。</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="228"/>
        <source>Inherited</source>
        <translation>继承的</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="238"/>
        <location filename="ACLEditor.ui" line="244"/>
        <source>Members</source>
        <translation>成员</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="170"/>
        <location filename="ACLEditor.ui" line="322"/>
        <location filename="ACLEditor.ui" line="354"/>
        <source>Add</source>
        <translation>添加</translation>
    </message>
    <message>
        <source>Add to Remove</source>
        <translation type="obsolete">添加到删除列表</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="303"/>
        <source>Add member to group</source>
        <translation>添加群成员</translation>
    </message>
    <message>
        <source>Type in the name of a player you wish to add to the group and click Add.</source>
        <translation type="obsolete">输入你要添加到群里的玩家名字然后点击添加。</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="335"/>
        <source>Remove member from group</source>
        <translation>从群里删除成员</translation>
    </message>
    <message>
        <source>Type in the name of a player you wish to remove from the group and click Add.</source>
        <translation type="obsolete">输入你要从群内删除的成员然后点击删除。</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="396"/>
        <source>&amp;ACL</source>
        <translation>&amp;访问控制列表</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="404"/>
        <source>Active ACLs</source>
        <translation>当前访问控制列表</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="410"/>
        <source>List of entries</source>
        <translation>条目列表</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="413"/>
        <source>This shows all the entries active on this channel. Entries inherited from parent channels will be show in italics.&lt;br /&gt;ACLs are evaluated top to bottom, meaning priority increases as you move down the list.</source>
        <translation>显示该频道内所有活跃条目。继承自父频道的条目将以斜体显示.&lt;br/&gt;访问控制列表是自上而下处理，意味着列表越往下优先级越高。</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="420"/>
        <source>Inherit ACL of parent?</source>
        <translation>继承父访问控制列表？</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="423"/>
        <source>This sets whether or not the ACL up the chain of parent channels are applied to this object. Only those entries that are marked in the parent as &quot;Apply to sub-channels&quot; will be inherited.</source>
        <translation>设置是否对当前对象应用父频道的访问控制列表。只有那些在父频道内被标记为“应用到子频道”的条目将被继承。</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="426"/>
        <source>Inherit ACLs</source>
        <translation>继承访问控制列表</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="433"/>
        <source>Move entry up</source>
        <translation>上移条目</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="436"/>
        <source>This moves the entry up in the list. As entries are evaluated in order, this may change the effective permissions of users. You cannot move an entry above an inherited entry, if you really need that you&apos;ll have to duplicate the inherited entry.</source>
        <translation>在列表中上移条目。由于条目自上而下被处理，这有可能改变用户的有效权限.你不能将一个条目移到继承的条目之上，如果你确实想这么做，你需要复制继承的条目。</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="439"/>
        <source>&amp;Up</source>
        <translation>&amp;上移</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="449"/>
        <source>Move entry down</source>
        <translation>下移条目</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="452"/>
        <source>This moves the entry down in the list. As entries are evaluated in order, this may change the effective permissions of users.</source>
        <translation>在列表中下移条目。由于条目自上而下被处理，这有可能改变用户的有效权限。</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="455"/>
        <source>&amp;Down</source>
        <translation>&amp;下移</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="465"/>
        <source>Add new entry</source>
        <translation>添加新条目</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="468"/>
        <source>This adds a new entry, initially set with no permissions and applying to all.</source>
        <translation>添加一个新条目，初始设置为无权限并应用到所有。</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="471"/>
        <source>&amp;Add</source>
        <translation>&amp;添加</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="481"/>
        <source>Remove entry</source>
        <translation>删除条目</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="484"/>
        <source>This removes the currently selected entry.</source>
        <translation>删除当前选中的条目。</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="487"/>
        <source>&amp;Remove</source>
        <translation>&amp;删除</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="506"/>
        <source>Context</source>
        <translation>上下文</translation>
    </message>
    <message>
        <source>Entry should apply to this channel</source>
        <translation type="obsolete">应用到该频道的条目</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="528"/>
        <source>This makes the entry apply to this channel.</source>
        <translation>应用该条目到当前频道。</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="531"/>
        <source>Applies to this channel</source>
        <translation>应用到当前频道</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="512"/>
        <source>Entry should apply to sub-channels.</source>
        <translation>应用到子频道的条目。</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="152"/>
        <source>&lt;b&gt;Group&lt;/b&gt;&lt;br&gt;
These are all the groups currently defined for the channel. To create a new group, just type in the name and press enter.</source>
        <oldsource>&lt;b&gt;Group&lt;/b&gt;&lt;br&gt;
This is all the groups currently defined for the channel. To create a new group, just type in the name and press enter.</oldsource>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="163"/>
        <source>Add new group</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="166"/>
        <source>&lt;b&gt;Add&lt;/b&gt;&lt;br/&gt;
Add a new group.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="180"/>
        <source>&lt;b&gt;Remove&lt;/b&gt;&lt;br&gt;This removes the currently selected group. If the group was inherited, it will not be removed from the list, but all local information about the group will be cleared.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="196"/>
        <source>&lt;b&gt;Inherit&lt;/b&gt;&lt;br&gt;This inherits all the members in the group from the parent, if the group is marked as &lt;i&gt;Inheritable&lt;/i&gt; in the parent channel.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="209"/>
        <source>&lt;b&gt;Inheritable&lt;/b&gt;&lt;br&gt;This makes this group inheritable to sub-channels. If the group is non-inheritable, sub-channels are still free to create a new group with the same name.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="225"/>
        <source>&lt;b&gt;Inherited&lt;/b&gt;&lt;br&gt;This indicates that the group was inherited from the parent channel. You cannot edit this flag, it&apos;s just for information.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="254"/>
        <source>Inherited members</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="264"/>
        <source>Contains the list of members added to the group by this channel.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="267"/>
        <source>&lt;b&gt;Members&lt;/b&gt;&lt;br&gt;
This list contains all members that were added to the group by the current channel. Be aware that this does not include members inherited by higher levels of the channel tree. These can be found in the &lt;i&gt;Inherited members&lt;/i&gt; list. To prevent this list to be inherited by lower level channels uncheck &lt;i&gt;Inheritable&lt;/i&gt; or manually add the members to the &lt;i&gt;Excluded members&lt;/i&gt; list.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="275"/>
        <source>Contains a list of members whose group membership will not be inherited from the parent channel.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="278"/>
        <source>&lt;b&gt;Excluded members&lt;/b&gt;&lt;br&gt;
Contains a list of members whose group membership will not be inherited from the parent channel.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="286"/>
        <source>Contains the list of members inherited by other channels.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="289"/>
        <source>&lt;b&gt;Inherited members&lt;/b&gt;&lt;br&gt;
Contains the list of members inherited by the current channel. Uncheck &lt;i&gt;Inherit&lt;/i&gt; to prevent inheritance from higher level channels.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="306"/>
        <source>Type in the name of a user you wish to add to the group and click Add.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="338"/>
        <source>Type in the name of a user you wish to remove from the group and click Add.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="375"/>
        <source>Exclude</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="382"/>
        <source>Excluded members</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="515"/>
        <source>This makes the entry apply to sub-channels of this channel.</source>
        <translation>应用该条目到当前频道的子频道。</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="518"/>
        <source>Applies to sub-channels</source>
        <translation>应用到子频道</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="525"/>
        <source>Entry should apply to this channel.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="572"/>
        <source>This controls which group of users this entry applies to.&lt;br /&gt;Note that the group is evaluated in the context of the channel the entry is used in. For example, the default ACL on the Root channel gives &lt;i&gt;Write&lt;/i&gt; permission to the &lt;i&gt;admin&lt;/i&gt; group. This entry, if inherited by a channel, will give a user write privileges if he belongs to the &lt;i&gt;admin&lt;/i&gt; group in that channel, even if he doesn&apos;t belong to the &lt;i&gt;admin&lt;/i&gt; group in the channel where the ACL originated.&lt;br /&gt;If a group name starts with a &apos;!&apos;, its membership is negated, and if it starts with a &apos;~&apos;, it is evaluated in the channel the ACL was defined in, rather than the channel the ACL is active in.&lt;br /&gt;If a group name starts with a  &apos;#&apos;, it is interpreted as an access token. Users must have entered whatever follows the &apos;#&apos; in their list of access tokens to match. This can be used for very simple password access to channels for non-authenticated users.&lt;br /&gt;If a group name starts with a  &apos;$&apos;, it will only match users whose certificate hash matches what follows the &apos;$&apos;.&lt;br /&gt;A few special predefined groups are:&lt;br /&gt;&lt;b&gt;all&lt;/b&gt; - Everyone will match.&lt;br /&gt;&lt;b&gt;auth&lt;/b&gt; - All authenticated users will match.&lt;br /&gt;&lt;b&gt;sub,a,b,c&lt;/b&gt; - User currently in a sub-channel minimum &lt;i&gt;a&lt;/i&gt; common parents, and between &lt;i&gt;b&lt;/i&gt; and &lt;i&gt;c&lt;/i&gt; channels down the chain. See the website for more extensive documentation on this one.&lt;br /&gt;&lt;b&gt;in&lt;/b&gt; - Users currently in the channel will match (convenience for &apos;&lt;i&gt;sub,0,0,0&lt;/i&gt;&apos;).&lt;br /&gt;&lt;b&gt;out&lt;/b&gt; - Users outside the channel will match (convenience for &apos;&lt;i&gt;!sub,0,0,0&lt;/i&gt;&apos;).&lt;br /&gt;Note that an entry applies to either a user or a group, not both.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="619"/>
        <source>Permissions</source>
        <translation>权限</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="547"/>
        <source>User/Group</source>
        <translation>用户/群</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="569"/>
        <source>Group this entry applies to</source>
        <translation>应用该条目的群</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="582"/>
        <source>User ID</source>
        <translation>用户名</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="598"/>
        <source>User this entry applies to</source>
        <translation>应用该条目的用户</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="601"/>
        <source>This controls which user this entry applies to. Just type in the user name and hit enter to query the server for a match.</source>
        <translation>控制该条目应用到哪个用户。请输入用户名然后按回车来向服务器做匹配查询。</translation>
    </message>
</context>
<context>
    <name>ALSAAudioInput</name>
    <message>
        <location filename="ALSAAudio.cpp" line="172"/>
        <source>Default ALSA Card</source>
        <translation>默认 ALSA 声卡</translation>
    </message>
    <message>
        <location filename="ALSAAudio.cpp" line="363"/>
        <source>Opening chosen ALSA Input failed: %1</source>
        <translation>无法打开所选 ALSA 输入: %1</translation>
    </message>
</context>
<context>
    <name>ALSAAudioOutput</name>
    <message>
        <location filename="ALSAAudio.cpp" line="173"/>
        <source>Default ALSA Card</source>
        <translation>默认 ALSA 声卡</translation>
    </message>
    <message>
        <location filename="ALSAAudio.cpp" line="498"/>
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
        <location filename="ASIOInput.cpp" line="224"/>
        <source>%1 (version %2)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ASIOInput.cpp" line="227"/>
        <source>%1 -&gt; %2 samples buffer, with %3 sample resolution (%4 preferred) at %5 Hz</source>
        <oldsource>%1 -&gt; %2 samples buffer, with %3 sample resolution (%4 preferred) at %5Hz</oldsource>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ASIOInput.cpp" line="269"/>
        <location filename="ASIOInput.cpp" line="273"/>
        <location filename="ASIOInput.cpp" line="296"/>
        <location filename="ASIOInput.cpp" line="300"/>
        <source>Mumble</source>
        <translation></translation>
    </message>
    <message>
        <location filename="ASIOInput.cpp" line="269"/>
        <location filename="ASIOInput.cpp" line="296"/>
        <source>ASIO Initialization failed: %1</source>
        <translation>ASIO初始化失败: %1</translation>
    </message>
    <message>
        <location filename="ASIOInput.cpp" line="273"/>
        <location filename="ASIOInput.cpp" line="300"/>
        <source>Failed to instantiate ASIO driver</source>
        <translation>无法初始化 ASIO 驱动</translation>
    </message>
    <message>
        <location filename="ASIOInput.cpp" line="337"/>
        <source>ASIO</source>
        <translation></translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="14"/>
        <source>Form</source>
        <translation>表单</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="20"/>
        <source>Device selection</source>
        <translation>设备选择</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="26"/>
        <source>Device</source>
        <translation>设备</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="42"/>
        <source>Device to use for microphone</source>
        <translation>用作麦克风的设备</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="45"/>
        <source>This chooses what device to query. You still need to actually query the device and select which channels to use.</source>
        <translation>选择要查询的设备。你仍需要实际查询该设备然后选择要使用哪些声道。</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="52"/>
        <source>Query selected device</source>
        <translation>查询所选设备</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="55"/>
        <source>This queries the selected device for channels. Be aware that many ASIO drivers are buggy to the extreme, and querying them might cause a crash of either the application or the system.</source>
        <translation>查询所选设备的声道。注意大多数 ASIO 驱动有极端漏洞，查询设备可能导致应用程序或系统崩溃。</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="58"/>
        <source>&amp;Query</source>
        <translation>&amp;查询</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="65"/>
        <source>Configure selected device</source>
        <translation>配置所选设备</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="68"/>
        <source>This configures the selected device. Be aware that many ASIO drivers are buggy to the extreme, and querying them might cause a crash of either the application or the system.</source>
        <translation>配置所选设备。注意大多数 ASIO 驱动有极端漏洞，查询设备可能导致应用程序或系统崩溃。</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="71"/>
        <source>&amp;Configure</source>
        <translation>&amp;配置</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="81"/>
        <source>Capabilities</source>
        <translation>设置信息</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="87"/>
        <source>Driver name</source>
        <translation>驱动名称</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="107"/>
        <source>Buffer size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="133"/>
        <source>This will configure the input channels for ASIO. Make sure you select at least one channel as microphone and speaker. &lt;i&gt;Microphone&lt;/i&gt; should be where your microphone is attached, and &lt;i&gt;Speaker&lt;/i&gt; should be a channel that samples &apos;&lt;i&gt;What you hear&lt;/i&gt;&apos;.&lt;br /&gt;For example, on the Audigy 2 ZS, a good selection for Microphone would be &apos;&lt;i&gt;Mic L&lt;/i&gt;&apos; while Speaker should be &apos;&lt;i&gt;Mix L&lt;/i&gt;&apos; and &apos;&lt;i&gt;Mix R&lt;/i&gt;&apos;.</source>
        <translation type="unfinished"></translation>
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
        <location filename="ASIOInput.ui" line="130"/>
        <source>Configure input channels</source>
        <translation>配置输入声道</translation>
    </message>
    <message>
        <source>This will configure the input channels for ASIO. Make sure you select at least one channel as microphone and speaker. &lt;i&gt;Microphone&lt;/i&gt; should be where your microphone is attached, and &lt;i&gt;Speaker&lt;/i&gt; should be a channel that samples &lt;i&gt;What you hear&lt;/i&gt;.&lt;br /&gt;For example, on the Audigy 2 ZS, a good selection for Microphone would be &lt;i&gt;Mic L&lt;/i&gt; while Speaker should be &lt;i&gt;Mix L&lt;/i&gt; and &lt;i&gt;Mix R&lt;/i&gt;.</source>
        <translation type="obsolete">配置 ASIO 的输入声道.请确保你选择了至少一个声道作为麦克风和扬声器。&lt;i&gt;麦克风&lt;/i&gt;应该是你的已连接的麦克风连，&lt;i&gt;扬声器&lt;/i&gt;应该是一个采样&lt;i&gt;你听到的声音&lt;/i&gt;的声道。&lt;br /&gt;比如在 Audigy 2 ZS 上，一个好的麦克风的选择会是&lt;i&gt; Mic L &lt;/i&gt;而扬声器会是&lt;i&gt; Mix L &lt;/i&gt;和&lt;i&gt; Mix R&lt;/i&gt;.</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="136"/>
        <source>Channels</source>
        <translation>声道</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="157"/>
        <source>Microphone</source>
        <translation>麦克风</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="206"/>
        <location filename="ASIOInput.ui" line="305"/>
        <source>&lt;-</source>
        <translation></translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="219"/>
        <location filename="ASIOInput.ui" line="292"/>
        <source>-&gt;</source>
        <translation></translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="243"/>
        <source>Unused</source>
        <translation>未使用</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="329"/>
        <source>Speakers</source>
        <translation>扬声器</translation>
    </message>
</context>
<context>
    <name>ASIOInput</name>
    <message>
        <location filename="ASIOInput.cpp" line="417"/>
        <location filename="ASIOInput.cpp" line="520"/>
        <source>Mumble</source>
        <translation></translation>
    </message>
    <message>
        <location filename="ASIOInput.cpp" line="417"/>
        <source>You need to select at least one microphone and one speaker source to use ASIO. If you just need microphone sampling, use DirectSound.</source>
        <translation>你需要选择至少一个麦克风和一个扬声器源来使用 ASIO.如果你只需要使用要麦克风进行采样,请使用 DirectSound.</translation>
    </message>
    <message>
        <location filename="ASIOInput.cpp" line="520"/>
        <source>Opening selected ASIO device failed. No input will be done.</source>
        <translation>无法打开所选 ASIO 设备。没有可用输入。</translation>
    </message>
</context>
<context>
    <name>AboutDialog</name>
    <message>
        <location filename="About.cpp" line="37"/>
        <source>About Mumble</source>
        <translation>关于 Mumble</translation>
    </message>
    <message>
        <location filename="About.cpp" line="53"/>
        <source>&lt;h3&gt;Mumble (%1)&lt;/h3&gt;&lt;p&gt;Copyright %3 Thorvald Natvig&lt;br /&gt;slicer@users.sourceforge.net&lt;/p&gt;&lt;p&gt;&lt;b&gt;A voice-chat utility for gamers&lt;/b&gt;&lt;/p&gt;&lt;p&gt;&lt;tt&gt;&lt;a href=&quot;%2&quot;&gt;%2&lt;/a&gt;&lt;/tt&gt;&lt;/p&gt;</source>
        <translation>&lt;h3&gt;Mumble (%1)&lt;/h3&gt;&lt;p&gt;版权所有 %3 Thorvald Natvig&lt;br /&gt;slicer@users.sourceforge.net&lt;/p&gt;&lt;p&gt;&lt;b&gt;一个游戏玩家的语音聊天工具&lt;/b&gt;&lt;/p&gt;&lt;p&gt;&lt;tt&gt;&lt;a href=&quot;%2&quot;&gt;%2&lt;/a&gt;&lt;/tt&gt;&lt;/p&gt;</translation>
    </message>
    <message>
        <location filename="About.cpp" line="63"/>
        <source>&amp;About Mumble</source>
        <translation>&amp;关于 Mumble</translation>
    </message>
    <message>
        <location filename="About.cpp" line="64"/>
        <source>&amp;License</source>
        <translation>&amp;授权</translation>
    </message>
    <message>
        <location filename="About.cpp" line="66"/>
        <source>OK</source>
        <translation></translation>
    </message>
</context>
<context>
    <name>AboutSpeexDialog</name>
    <message>
        <location filename="About.cpp" line="74"/>
        <source>About Speex</source>
        <translation>关于 Speex</translation>
    </message>
    <message>
        <location filename="About.cpp" line="78"/>
        <source>&lt;h3&gt;About Speex&lt;/h3&gt;&lt;p&gt;&lt;tt&gt;&lt;a href=&quot;%1&quot;&gt;%1&lt;/a&gt;&lt;/tt&gt;&lt;/p&gt;&lt;p&gt;This program uses SpeexDSP.&lt;/p&gt;&lt;p&gt;Speex is used for echo cancellation, noise&lt;br /&gt;filtering and voice activity detection.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;h3&gt;About Speex&lt;/h3&gt;&lt;p&gt;&lt;tt&gt;&lt;a href=&quot;%2&quot;&gt;%2&lt;/a&gt;&lt;/tt&gt;&lt;/p&gt;&lt;p&gt;This program uses Speex version %1&lt;/p&gt;&lt;p&gt;Speex is used for echo cancellation, noise&lt;br /&gt;filtering, voice activity detection and speech&lt;br /&gt;compression.&lt;/p&gt;</source>
        <translation type="obsolete">&lt;h3&gt;关于 Speex&lt;/h3&gt;&lt;p&gt;&lt;tt&gt;&lt;a href=&quot;%2&quot;&gt;%2&lt;/a&gt;&lt;/tt&gt;&lt;/p&gt;&lt;p&gt;该程序使用 Speex 版本 %1&lt;/p&gt;&lt;p&gt;Speex 被用于回声消除，噪音&lt;br /&gt;过滤，语言活动检测和语音&lt;br /&gt;压缩。&lt;/p&gt;</translation>
    </message>
    <message>
        <location filename="About.cpp" line="86"/>
        <source>OK</source>
        <translation></translation>
    </message>
</context>
<context>
    <name>AudioInput</name>
    <message>
        <location filename="AudioInput.ui" line="14"/>
        <source>Form</source>
        <translation>表单</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="20"/>
        <source>Interface</source>
        <translation>接口</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="26"/>
        <source>System</source>
        <translation>系统</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="42"/>
        <source>Input method for audio</source>
        <translation>音频输入方法</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="45"/>
        <source>&lt;b&gt;This is the input method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation>&lt;b&gt;这是用于音频的输入方法。&lt;/b&gt;&lt;br /&gt;很有可能你想使用 DirectSound。</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="68"/>
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
        <source>Cancel Echo</source>
        <translation type="obsolete">消除回声</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="141"/>
        <source>Transmission</source>
        <translation>传送</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="147"/>
        <source>&amp;Transmit</source>
        <translation>&amp;传送</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="157"/>
        <source>When to transmit your speech</source>
        <translation>何时传送你的语音</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="160"/>
        <source>&lt;b&gt;This sets when speech should be transmitted.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;Continuous&lt;/i&gt; - All the time&lt;br /&gt;&lt;i&gt;Voice Activity&lt;/i&gt; - When you are speaking clearly.&lt;br /&gt;&lt;i&gt;Push To Talk&lt;/i&gt; - When you hold down the hotkey set under &lt;i&gt;Shortcuts&lt;/i&gt;.</source>
        <translation>&lt;b&gt;设置何时语音将被传送。&lt;/b&gt;&lt;br /&gt;&lt;i&gt;连续&lt;/i&gt;一直&lt;br /&gt;&lt;i&gt;有语音活动&lt;/i&gt; 当你说话清晰时。&lt;br /&gt;&lt;i&gt;按键通话&lt;/i&gt; - 当你按住作为&lt;i&gt;快捷键&lt;/i&gt;的自定义按键组合时。</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="219"/>
        <source>DoublePush Time</source>
        <translation>两次按键时间间隔</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="229"/>
        <source>If you press the PTT key twice in this time it will get locked.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="232"/>
        <source>&lt;b&gt;DoublePush Time&lt;/b&gt;&lt;br /&gt;If you press the push-to-talk key twice during the configured interval of time it will be locked. Mumble will keep transmitting until you hit the key once more to unlock PTT again.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="251"/>
        <source>TextLabel</source>
        <translation>文本标签</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="258"/>
        <source>Gets played when the PTT button is pressed</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="265"/>
        <source>Gets played when the PTT button is released</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="272"/>
        <source>Reset audio cue to default</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="275"/>
        <source>&lt;b&gt;Reset&lt;/b&gt;&lt;br/&gt;Reset the paths for the files to their default.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="278"/>
        <source>Reset</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="285"/>
        <source>Browse for on audio file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="288"/>
        <location filename="AudioInput.ui" line="298"/>
        <source>Browse</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="295"/>
        <source>Browse for off audio file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="305"/>
        <source>Off</source>
        <translation type="unfinished">关闭</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="315"/>
        <source>On</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="325"/>
        <source>Preview the audio cues</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="328"/>
        <source>&lt;b&gt;Preview&lt;/b&gt;&lt;br/&gt;Plays the current &lt;i&gt;on&lt;/i&gt; soundfile followed by the current &lt;i&gt;off&lt;/i&gt; soundfile.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="331"/>
        <source>Preview</source>
        <translation type="unfinished">预览</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="341"/>
        <source>&lt;b&gt;This enables the audio cues for push to talk.&lt;/b&gt;&lt;br /&gt;Setting this will give you a short audio beep when push to talk is pressed and released.</source>
        <translation>&lt;b&gt;启用按键通话的音频提示。&lt;/b&gt;&lt;br /&gt;当你按下或释放按键通话快捷键时会听到一声短的蜂鸣声。</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="344"/>
        <source>PTT Audio cue</source>
        <translation>PTT 提示音</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="357"/>
        <source>Use SNR based speech detection</source>
        <translation>使用基于 SNR 的语音侦测</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="363"/>
        <source>Signal to Noise</source>
        <translation>信噪比</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="370"/>
        <source>Use Amplitude based speech detection.</source>
        <translation>使用基于振幅的语音侦测。</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="373"/>
        <source>&lt;b&gt;This sets speech detection to use Amplitude.&lt;/b&gt;&lt;br /&gt;In this mode, the raw strength of the input signal is used to detect speech.</source>
        <translation>&lt;b&gt;设置用振幅来做语音侦测。&lt;/b&gt;&lt;br /&gt;在此模式下，输入信号的原始强度被用于语音侦测。</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="376"/>
        <source>Amplitude</source>
        <translation>振幅</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="385"/>
        <source>Voice &amp;Hold</source>
        <translation>&amp;暂停 发言</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="395"/>
        <source>How long to keep transmitting after silence</source>
        <translation>无声后多长时间内继续传送</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="398"/>
        <source>&lt;b&gt;This selects how long after a perceived stop in speech transmission should continue.&lt;/b&gt;&lt;br /&gt;Set this higher if your voice breaks up when you speak (seen by a rapidly blinking voice icon next to your name).&lt;br /&gt;Only makes sense when used with Voice Activity transmission.</source>
        <translation>&lt;b&gt;设置发言暂停被感知到后多长时间内继续传送。&lt;/b&gt;如果你在讲话时你的语音破碎（在你的名字旁边会显示一个快速闪烁的语音图标），增大这个值。&lt;br /&gt;只有在使用检测到语音活动时传送时该选项才有用。</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="443"/>
        <source>Silence Below</source>
        <translation>无声 低于</translation>
    </message>
    <message>
        <source>Signal values below this counts as silence</source>
        <translation type="obsolete">信号值低于这个数值时视为无声</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="453"/>
        <location filename="AudioInput.ui" line="485"/>
        <source>&lt;b&gt;This sets the trigger values for voice detection.&lt;/b&gt;&lt;br /&gt;Use this together with the Audio Statistics window to manually tune the trigger values for detecting speech. Input values below &quot;Silence Below&quot; always count as silence. Values above &quot;Speech Above&quot; always count as voice. Values in between will count as voice if you&apos;re already talking, but will not trigger a new detection.</source>
        <translation>&lt;b&gt;设置语音检测的触发值。&lt;/b&gt;&lt;br /&gt;与音频统计窗口一起来使用该值以手工调整语音侦测的触发值。输入值低于&quot;无声 低于“时总是会被视为无声。值高于”语音 高于“时总是被视为语音。如果你正在讲话，两者之间的值会被视为语音，但是不会触发一个新的侦测。</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="475"/>
        <source>Speech Above</source>
        <translation>语言 高于</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="482"/>
        <source>Signal values above this count as voice</source>
        <translation>信号值高于这个数值时视为语音</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="515"/>
        <source>Compression</source>
        <translation>压缩</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="521"/>
        <source>&amp;Quality</source>
        <translation>&amp;质量</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="531"/>
        <source>Quality of compression (peak bandwidth)</source>
        <translation>压缩质量(峰值带宽）</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="534"/>
        <source>&lt;b&gt;This sets the quality of compression.&lt;/b&gt;&lt;br /&gt;This determines how much bandwidth Mumble is allowed to use for outgoing audio.</source>
        <translation>&lt;b&gt;设置压缩质量。&lt;/b&gt;&lt;br /&gt;确定 Mumble 可用于发送音频的带宽大小。</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="572"/>
        <source>Audio per packet</source>
        <translation>每个数据包的音频帧数</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="582"/>
        <source>How many audio frames to send per packet</source>
        <translation>每个数据包里发送多少个音频帧</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="585"/>
        <source>&lt;b&gt;This selects how many audio frames should be put in one packet.&lt;/b&gt;&lt;br /&gt;Increasing this will increase the latency of your voice, but will also reduce bandwidth requirements.</source>
        <translation>&lt;b&gt;选择一个数据包里放多少个音频帧。&lt;/b&gt;&lt;br /&gt;增加这个数值会增加你的语音延迟，但也会减少带宽需求。</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="625"/>
        <source>&lt;b&gt;This shows peak outgoing bandwidth used.&lt;/b&gt;&lt;br /&gt;This shows the peak amount of bandwidth sent out from your machine. Audio bitrate is the maximum bitrate (as we use VBR) for the audio data alone. Position is the bitrate used for positional information. Overhead is our framing and the IP packet headers (IP and UDP is 75% of this overhead).</source>
        <translation>&lt;b&gt;显示已用的发送带宽峰值。&lt;/b&gt;&lt;br /&gt;显示从你的机器发出的峰值带宽大小。音频比特率是仅用于音频数据的最大比特率(由于我们使用VBR)。位置是用于位置信息的比特率。系统开销是我们的组帧和 IP 数据包头(IP 和 UDP 是该系统开销的75%)。</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="641"/>
        <source>Audio Processing</source>
        <translation>音频处理</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="647"/>
        <source>Noise Suppression</source>
        <translation>噪音抑制</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="657"/>
        <source>Noise suppression</source>
        <translation>噪音抑制</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="660"/>
        <source>&lt;b&gt;This sets the amount of noise suppression to apply.&lt;/b&gt;&lt;br /&gt;The higher this value, the more aggressively stationary noise will be suppressed.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="692"/>
        <source>Amplification</source>
        <translation>放大率</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="702"/>
        <source>Maximum amplification of input sound</source>
        <translation>输入声音的最大放大率</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="705"/>
        <source>&lt;b&gt;Maximum amplification of input.&lt;/b&gt;&lt;br /&gt;Mumble normalizes the input volume before compressing, and this sets how much it&apos;s allowed to amplify.&lt;br /&gt;The actual level is continually updated based on your current speech pattern, but it will never go above the level specified here.&lt;br /&gt;If the &lt;i&gt;Microphone loudness&lt;/i&gt; level of the audio statistics hover around 100%, you probably want to set this to 2.0 or so, but if, like most people, you are unable to reach 100%, set this to something much higher.&lt;br /&gt;Ideally, set it so &lt;i&gt;Microphone Loudness * Amplification Factor &gt;= 100&lt;/i&gt;, even when you&apos;re speaking really soft.&lt;br /&gt;&lt;br /&gt;Note that there is no harm in setting this to maximum, but Mumble will start picking up other conversations if you leave it to auto-tune to that level.</source>
        <translation>&lt;b&gt;输入的最大放大率。&lt;/b&gt;&lt;br /&gt;Mumble 在压缩前会规格化输入音量，该选项设置音量被允许放大多少。&lt;br /&gt;实际等级将根据你的语音模式来持续更新，但是不会高于这里指定的等级。&lt;br /&gt;如果音频统计的&lt;i&gt;麦克风响度&lt;/i&gt;等级在 100%左右，你很有可能想把该值设为大约 2.0，但是如果像大多数人一样，你不能达到 100%，设置一个更高的值。&lt;br /&gt;理想状态下，设置该值使&lt;i&gt;麦克风响度 * 放大率 &gt;= 100&lt;/i&gt;,甚至当你相当轻声的讲话时。&lt;br /&gt;&lt;br /&gt;注意设置该值为其最大值不会有任何副作用，但是如果你将该值设为自动调节到其最大值， Mumble 会开始拾取其他人的谈话。</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="433"/>
        <source>Current speech detection chance</source>
        <translation>当前语音侦测几率</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="84"/>
        <source>Input device for audio</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="87"/>
        <source>&lt;b&gt;This is the input device to use for audio.&lt;/b&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="100"/>
        <source>Cancel echo from speakers</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="103"/>
        <source>Enabling this will cancel the echo from your speakers. Mixed has low CPU impact, but only works well if your speakers are equally loud and equidistant from the microphone. Multichannel echo cancellation provides much better echo cancellation, but at a higher CPU cost.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="107"/>
        <source>Disabled</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="112"/>
        <source>Mixed</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="117"/>
        <source>Multichannel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="125"/>
        <source>Echo</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="338"/>
        <source>Audible audio cue when push-to-talk is activated or deactivated</source>
        <translation>当按键通话激活或未激活时听到的提示音频</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="360"/>
        <source>&lt;b&gt;This sets speech detection to use Signal to Noise ratio.&lt;/b&gt;&lt;br /&gt;In this mode, the input is analyzed for something resembling a clear signal, and the clarity of that signal is used to trigger speech detection.</source>
        <translation>&lt;b&gt;设置使用信噪比来进行语音侦测。&lt;/b&gt;&lt;br /&gt; 在此模式下，将从输入中寻找并分析出一个近似清晰的信号，而这个信号的清晰度被用于触发语音侦测。</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="436"/>
        <source>&lt;b&gt;This shows the current speech detection settings.&lt;/b&gt;&lt;br /&gt;You can change the settings from the Settings dialog or from the Audio Wizard.</source>
        <translation>&lt;b&gt;显示当前语音侦测设置。&lt;/b&gt;&lt;br /&gt;你能在设置对话框或音频精灵里修改当前设置。</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="167"/>
        <source>Idle AutoMute</source>
        <translation>空闲时自动静音</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="174"/>
        <source>How long to remain idle before auto-mute.</source>
        <translation>空闲多长时间后自动静音。</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="177"/>
        <source>This sets an idle timer. If the timer expires without any audio being sent to the server, you are muted.</source>
        <translation>设置空闲定时器。如果定时器过期时没有任何音频发送到服务器，你将被静音。</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="450"/>
        <source>Signal values below this count as silence</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="622"/>
        <source>Maximum bandwidth used for sending audio</source>
        <translation>用于发送音频的最大带宽</translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets the amount of noise suppression to apply.&lt;/b&gt;&lt;br /&gt;The higher this value, the more aggressively stationary noise will be supressed.</source>
        <translation type="obsolete">&lt;b&gt;设置应用于噪音抑制的数值大小.&lt;/b&gt;&lt;br /&gt;数值越高，稳态噪声越会被积极的抑制。</translation>
    </message>
    <message>
        <location filename="AudioInput.cpp" line="525"/>
        <source>Server maximum network bandwidth is only %1 kbit/s. Audio quality auto-adjusted to %2 kbit/s (%3ms)</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>AudioInputDialog</name>
    <message>
        <location filename="AudioConfigDialog.cpp" line="74"/>
        <source>Continuous</source>
        <translation>连续</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="75"/>
        <source>Voice Activity</source>
        <translation>语音活动</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="76"/>
        <source>Push To Talk</source>
        <translation>按键通话</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="89"/>
        <source>Audio Input</source>
        <translation>音频输入</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="185"/>
        <location filename="AudioConfigDialog.cpp" line="193"/>
        <source>%1 ms</source>
        <translation>%1 毫秒</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="191"/>
        <location filename="AudioConfigDialog.cpp" line="211"/>
        <location filename="AudioConfigDialog.cpp" line="229"/>
        <source>Off</source>
        <translation>关闭</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="199"/>
        <source>%1 s</source>
        <translation>%1 秒</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="203"/>
        <source>%1 kb/s</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="214"/>
        <source>-%1 dB</source>
        <translation>-%1 分贝</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="269"/>
        <source>%1kbit/s (Audio %2 %5, Position %4, Overhead %3)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="269"/>
        <source>CELT</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="269"/>
        <source>Speex</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>%1kbit/s (Audio %2, Position %4, Overhead %3)</source>
        <translation type="obsolete">%1 千比特/秒 （音频 %2, 位置 %4, 系统开销 %3）</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="227"/>
        <source>%1 min</source>
        <translation>%1 分钟</translation>
    </message>
</context>
<context>
    <name>AudioOutput</name>
    <message>
        <location filename="AudioOutput.ui" line="14"/>
        <source>Form</source>
        <translation>表单</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="20"/>
        <source>Interface</source>
        <translation>接口</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="26"/>
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
        <location filename="AudioOutput.ui" line="68"/>
        <source>Device</source>
        <translation>设备</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="42"/>
        <source>Output method for audio</source>
        <translation>音频输出方法</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="45"/>
        <source>&lt;b&gt;This is the output method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation>&lt;b&gt;这是用于音频输出的方法。&lt;/b&gt;&lt;br /&gt;很有可能你想使用 DirectSound。</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="84"/>
        <source>Output device for audio</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="87"/>
        <source>&lt;b&gt;This is the output device to use for audio.&lt;/b&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="100"/>
        <location filename="AudioOutput.ui" line="281"/>
        <source>Positional Audio</source>
        <translation>定位音频</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="110"/>
        <source>Audio Output</source>
        <translation>音频输出</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="116"/>
        <source>Default &amp;Jitter Buffer</source>
        <translation>默认 &amp;Jitter 缓冲区</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="126"/>
        <source>Safety margin for jitter buffer</source>
        <translation>Jitter 缓冲区的安全边界</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="129"/>
        <source>&lt;b&gt;This sets the minimum safety margin for the jitter buffer.&lt;/b&gt;&lt;br /&gt;All incoming audio is buffered, and the jitter buffer continually tries to push the buffer to the minimum sustainable by your network, so latency can be as low as possible. This sets the minimum buffer size to use. If the start of sentences you hear is very jittery, increase this value.</source>
        <translation>&lt;b&gt;设置 Jitter 缓冲区的最小安全边界. &lt;/b&gt;&lt;br /&gt;所有收到的音频都会被缓冲，而 Jitter 缓冲区不断尝试使该缓冲区保持能被你的网络所允许的最小值, 从而使延迟尽可能的低。 设置使用的缓冲区大小的最小值。如果你听到的句子的起始部分是颤音，请增加这个值。</translation>
    </message>
    <message>
        <source>TextLabel</source>
        <translation type="obsolete">文本标签</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="161"/>
        <source>Volume</source>
        <translation>音量</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="171"/>
        <source>Volume of incoming speech</source>
        <translation>收到的语音的音量</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="174"/>
        <source>&lt;b&gt;This adjusts the volume of incoming speech.&lt;/b&gt;&lt;br /&gt;Note that if you increase this beyond 100%, audio will be distorted.</source>
        <translation>&lt;b&gt;调节收到的语音的音量。&lt;/b&gt;&lt;br /&gt;注意如果你增加此值至超过 100%，声音会失真。</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="200"/>
        <source>Output Delay</source>
        <translation>输出延时</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="210"/>
        <source>Amount of data to buffer</source>
        <translation>缓冲数据大小</translation>
    </message>
    <message>
        <source>This sets the amount of data to prebuffer in the output buffer. Experiment with different values and set it to the lowest which doesn&apos;t cause rapid jitter in the sound.</source>
        <translation type="obsolete">设置输出缓冲区里前置缓冲的数据大小。试验不同值然后设置成不引起快速颤音的最低值.</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="287"/>
        <source>The connected &quot;speakers&quot; are actually headphones</source>
        <oldsource>The connected &quot;speakers&quot; are actually headphones.</oldsource>
        <translation type="unfinished">已连接的“扬声器”实际上是耳机.</translation>
    </message>
    <message>
        <source>This sets the minimum distance for sound calculations. The volume of other players&apos; speech will not decrease until they are at least this far away from you.</source>
        <translation type="obsolete">设置声强计算的最小距离.当别的玩家离你小于这个距离时他的语音将不会减弱.</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="397"/>
        <source>Factor for sound volume decrease</source>
        <translation>音量减弱因子</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="426"/>
        <source>Bloom</source>
        <translation>声音增强</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="449"/>
        <source>Factor for sound volume increase</source>
        <translation>音量增强因子</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="452"/>
        <source>How much should sound volume increase for sources that are really close?</source>
        <translation>当音源距离过近时音量该增强多少?</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="290"/>
        <source>Checking this indicates that you don&apos;t have speakers connected, just headphones. This is important, as speakers are usually in front of you, while headphones are directly to your left/right.</source>
        <translation>选中此选项表明你没有连接扬声器，只有耳机。这很重要，因为扬声器通常在你面前，而耳机直接在你左右。</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="213"/>
        <source>This sets the amount of data to pre-buffer in the output buffer. Experiment with different values and set it to the lowest which doesn&apos;t cause rapid jitter in the sound.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="293"/>
        <source>Headphones</source>
        <translation>耳机</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="300"/>
        <source>Minimum Distance</source>
        <translation>最小距离</translation>
    </message>
    <message>
        <source>Minimum distance to player before sound volume decreases</source>
        <translation type="obsolete">距离玩家至少多远时声音开始减弱</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="345"/>
        <source>Maximum Distance</source>
        <translation>最大距离</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="355"/>
        <source>Maximum distance, beyond which speech volume won&apos;t decrease</source>
        <translation>超过最大距离时音量将不再减弱</translation>
    </message>
    <message>
        <source>This sets the maximum distance for sound calculations. When farther away than this, other players&apos; speech volume will not decrease any further.</source>
        <translation type="obsolete">设置声强计算最大距离。当超过此距离时，别的玩家的音量将不再减弱。</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="310"/>
        <source>Minimum distance to user before sound volume decreases</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="316"/>
        <source>This sets the minimum distance for sound calculations. The volume of other users&apos; speech will not decrease until they are at least this far away from you.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="358"/>
        <source>This sets the maximum distance for sound calculations. When farther away than this, other users&apos; speech volume will not decrease any further.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="387"/>
        <source>Minimum Volume</source>
        <translation>最小音量</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="400"/>
        <source>What should the volume be at the maximum distance?</source>
        <translation>位于最大距离时的音量大小?</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="471"/>
        <source>Loopback Test</source>
        <translation>回环测试</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="477"/>
        <source>Delay Variance</source>
        <translation>延时方差</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="487"/>
        <source>Variance in packet latency</source>
        <translation>数据包延时的方差</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="490"/>
        <source>&lt;b&gt;This sets the packet latency variance for loopback testing.&lt;/b&gt;&lt;br /&gt;Most audio paths contain some variable latency. This allows you to set that variance for loopback mode testing. For example, if you set this to 15ms, this will emulate a network with 20-35ms ping latency or one with 80-95ms latency. Most domestic net connections have a variance of about 5ms.</source>
        <oldsource>&lt;b&gt;This sets the packet latency variance for loopback testing.&lt;/b&gt;&lt;br /&gt;Most audio paths contain some variable latency. This allows you set that variance for loopback mode testing. For example, if you set this to 15ms, this will emulate a network with 20-35ms ping latency or one with 80-95ms latency. Most domestic net connections have a variance of about 5ms.</oldsource>
        <translation type="unfinished">&lt;b&gt;设置回环测试时的数据包延迟方差。&lt;/b&gt;&lt;br /&gt;大多数音频路径含有一定的延迟方差。该选项允许你设置回环测试模式时的方差。例如，如果你设置该值为 15毫秒，将模拟一个 20-35毫秒 ping 延迟或一个 80-95毫秒延迟的网络。大多数国内网络的延迟方差啊大概是5毫秒</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="516"/>
        <source>Packet Loss</source>
        <translation>数据包丢失</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="526"/>
        <source>Packet loss for loopback mode</source>
        <translation>回环模式的数据包丢失</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="529"/>
        <source>&lt;b&gt;This sets the packet loss for loopback mode.&lt;/b&gt;&lt;br /&gt;This will be the ratio of packets lost. Unless your outgoing bandwidth is peaked or there&apos;s something wrong with your network connection, this will be 0%</source>
        <translation>&lt;b&gt;设置回环模式时的数据包丢失。&lt;/b&gt;&lt;br /&gt;该值即为丢包率。除非你的出外的带宽达到峰值或者你的网络连接有问题，该值会是 0%</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="561"/>
        <source>&amp;Loopback</source>
        <translation>&amp;回环</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="571"/>
        <source>Desired loopback mode</source>
        <translation>期望的回环模式</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="574"/>
        <source>&lt;b&gt;This enables one of the loopback test modes.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;None&lt;/i&gt; - Loopback disabled&lt;br /&gt;&lt;i&gt;Local&lt;/i&gt; - Emulate a local server.&lt;br /&gt;&lt;i&gt;Server&lt;/i&gt; - Request loopback from server.&lt;br /&gt;Please note than when loopback is enabled, no other users will hear your voice. This setting is not saved on application exit.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;b&gt;This enables one of the loopback testmodes.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;None&lt;/i&gt; - Loopback disabled&lt;br /&gt;&lt;i&gt;Local&lt;/i&gt; - Emulate a local server.&lt;br /&gt;&lt;i&gt;Server&lt;/i&gt; - Request loopback from server.&lt;br /&gt;Please note than when loopback is enabled, no other players will hear your voice. This setting is not saved on application exit.</source>
        <translation type="obsolete">&lt;b&gt;该选项启用某种回环测试模式。&lt;/b&gt;&lt;br /&gt;&lt;i&gt;无&lt;/i&gt; - 禁用回环&lt;br /&gt;&lt;i&gt;本地&lt;/i&gt; - 模拟一个本地服务器。&lt;br /&gt;&lt;i&gt;服务器&lt;/i&gt; - 请求来自服务器的回环。&lt;br /&gt;请注意当回环模式启用时，别的玩家不会听到你的声音。当应用程序退出时该选项不会被存盘。</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="246"/>
        <source>Other Applications</source>
        <translation>其他应用程序</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="256"/>
        <source>Volume of other applications during speech</source>
        <translation>讲话时别的应用程序的音量</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="259"/>
        <source>&lt;b&gt;Decrease volume of other applications during speech.&lt;/b&gt;&lt;br /&gt;Mumble supports decreasing the volume of other applications during incoming speech. This sets the relative volume of other applications when others are talking.</source>
        <translation>&lt;b&gt;讲话时减小其他应用程序的音量.&lt;/b&gt;&lt;br /&gt;Mumble 支持收到语音时减小其他应用程序的音量。这将设置当其他的人交谈时其他应用程序的相对音量。</translation>
    </message>
</context>
<context>
    <name>AudioOutputDialog</name>
    <message>
        <location filename="AudioConfigDialog.cpp" line="394"/>
        <source>None</source>
        <translation>无</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="395"/>
        <source>Local</source>
        <translation>本地</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="396"/>
        <source>Server</source>
        <translation>服务器</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="402"/>
        <source>Audio Output</source>
        <translation>音频输出</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="495"/>
        <location filename="AudioConfigDialog.cpp" line="514"/>
        <source>%1 ms</source>
        <translation>%1 毫秒</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="506"/>
        <location filename="AudioConfigDialog.cpp" line="510"/>
        <location filename="AudioConfigDialog.cpp" line="518"/>
        <location filename="AudioConfigDialog.cpp" line="549"/>
        <location filename="AudioConfigDialog.cpp" line="553"/>
        <source>%1%</source>
        <translation></translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="522"/>
        <source>%1ms</source>
        <translation>%1 毫秒</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="537"/>
        <location filename="AudioConfigDialog.cpp" line="543"/>
        <source>%1m</source>
        <translation>%1 米</translation>
    </message>
</context>
<context>
    <name>AudioOutputSample</name>
    <message>
        <location filename="AudioOutput.cpp" line="295"/>
        <source>Choose sound file</source>
        <translation type="unfinished">选择声音文件</translation>
    </message>
    <message>
        <location filename="AudioOutput.cpp" line="299"/>
        <source>Invalid sound file</source>
        <translation type="unfinished">无效声音文件</translation>
    </message>
    <message>
        <location filename="AudioOutput.cpp" line="300"/>
        <source>The file &apos;%1&apos; cannot be used by Mumble. Please select a file with a compatible format and encoding.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>AudioStats</name>
    <message>
        <location filename="AudioStats.cpp" line="384"/>
        <source>&gt;1000ms</source>
        <translation>&gt;1000毫秒</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="14"/>
        <source>Audio Statistics</source>
        <translation>音频统计</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="22"/>
        <source>Input Levels</source>
        <translation>输入级别</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="28"/>
        <source>Peak microphone level</source>
        <translation>麦克风级别峰值</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="35"/>
        <location filename="AudioStats.ui" line="55"/>
        <location filename="AudioStats.ui" line="75"/>
        <source>Peak power in last frame</source>
        <translation>最后帧的峰值功率</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="38"/>
        <source>This shows the peak power in the last frame (20 ms), and is the same measurement as you would usually find displayed as &quot;input power&quot;. Please disregard this and look at &lt;b&gt;Microphone power&lt;/b&gt; instead, which is much more steady and disregards outliers.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>TextLabel</source>
        <translation type="obsolete">文本标签</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="48"/>
        <source>Peak speaker level</source>
        <translation>扬声器级别峰值</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="58"/>
        <source>This shows the peak power of the speakers in the last frame (20 ms). Unless you are using a multi-channel sampling method (such as ASIO) with speaker channels configured, this will be 0. If you have such a setup configured, and this still shows 0 while you&apos;re playing audio from other programs, your setup is not working.</source>
        <oldsource>This shows the peak power in the last frame (20 ms) of the speakers. Unless you are using a multi-channel sampling method (such as ASIO) with speaker channels configured, this will be 0. If you have such a setup configured, and this still shows 0 while you&apos;re playing audio from other programs, your setup is not working.</oldsource>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="68"/>
        <source>Peak clean level</source>
        <translation>静输入级别峰值</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="78"/>
        <source>This shows the peak power in the last frame (20 ms) after all processing. Ideally, this should be -96 dB when you&apos;re not talking. In reality, a sound studio should see -60 dB, and you should hopefully see somewhere around -20 dB. When you are talking, this should rise to somewhere between -5 and -10 dB.&lt;br /&gt;If you are using echo cancellation, and this rises to more than -15 dB when you&apos;re not talking, your setup is not working, and you&apos;ll annoy other users with echoes.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="91"/>
        <source>Signal Analysis</source>
        <translation>信号分析</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="97"/>
        <source>Microphone power</source>
        <translation>麦克风功率</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="104"/>
        <source>How close the current input level is to ideal</source>
        <translation>当前输入级别离完美有多近</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="107"/>
        <source>This shows how close your current input volume is to the ideal. To adjust your microphone level, open whatever program you use to adjust the recording volume, and look at the value here while talking.&lt;br /&gt;&lt;b&gt;Talk loud, as you would when you&apos;re upset over getting fragged by a noob.&lt;/b&gt;&lt;br /&gt;Adjust the volume until this value is close to 100%, but make sure it doesn&apos;t go above. If it does go above, you are likely to get clipping in parts of your speech, which will degrade sound quality.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="117"/>
        <source>Signal-To-Noise ratio</source>
        <translation>信噪比</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="124"/>
        <source>Signal-To-Noise ratio from the microphone</source>
        <translation>麦克风的信噪音比</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="127"/>
        <source>This is the Signal-To-Noise Ratio (SNR) of the microphone in the last frame (20 ms). It shows how much clearer the voice is compared to the noise.&lt;br /&gt;If this value is below 1.0, there&apos;s more noise than voice in the signal, and so quality is reduced.&lt;br /&gt;There is no upper limit to this value, but don&apos;t expect to see much above 40-50 without a sound studio.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="137"/>
        <source>Speech Probability</source>
        <translation>语音的概率</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="144"/>
        <source>Probability of speech</source>
        <translation>语音的概率</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="147"/>
        <source>This is the probability that the last frame (20 ms) was speech and not environment noise.&lt;br /&gt;Voice activity transmission depends on this being right. The trick with this is that the middle of a sentence is always detected as speech; the problem is the pauses between words and the start of speech. It&apos;s hard to distinguish a sigh from a word starting with &apos;h&apos;.&lt;br /&gt;If this is in bold font, it means Mumble is currently transmitting (if you&apos;re connected).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="162"/>
        <source>Configuration feedback</source>
        <translation>配置反馈</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="168"/>
        <source>Current audio bitrate</source>
        <translation>当前音频比特率</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="181"/>
        <source>Bitrate of last frame</source>
        <translation>最后帧的比特率</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="184"/>
        <source>This is the audio bitrate of the last compressed frame (20 ms), and as such will jump up and down as the VBR adjusts the quality. To adjust the peak bitrate, adjust &lt;b&gt;Compression Complexity&lt;/b&gt; in the Settings dialog.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="194"/>
        <source>DoublePush interval</source>
        <translation>两次按键时间间隔</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="207"/>
        <source>Time between last two Push-To-Talk presses</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Time between last two PushToTalk presses</source>
        <translation type="obsolete">最后两次按下按键通话键的时间间隔</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="217"/>
        <source>Speech Detection</source>
        <translation>语音侦测</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="224"/>
        <source>Current speech detection chance</source>
        <translation>当前语音侦测几率</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="227"/>
        <source>&lt;b&gt;This shows the current speech detection settings.&lt;/b&gt;&lt;br /&gt;You can change the settings from the Settings dialog or from the Audio Wizard.</source>
        <translation>&lt;b&gt;显示当前语音侦测设置。&lt;/b&gt;&lt;br /&gt;你能在设置对话框或音频精灵里修改当前设置。</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="256"/>
        <source>Signal and noise power spectrum</source>
        <translation>信号和噪音功率频谱</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="262"/>
        <source>Power spectrum of input signal and noise estimate</source>
        <translation>输入信号和噪音估计的功率频谱</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="265"/>
        <source>This shows the power spectrum of the current input signal (red line) and the current noise estimate (filled blue).&lt;br /&gt;All amplitudes are multiplied by 30 to show the interesting parts (how much more signal than noise is present in each waveband).&lt;br /&gt;This is probably only of interest if you&apos;re trying to fine-tune noise conditions on your microphone. Under good conditions, there should be just a tiny flutter of blue at the bottom. If the blue is more than halfway up on the graph, you have a seriously noisy environment.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="281"/>
        <source>Echo Analysis</source>
        <translation>回声分析</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="293"/>
        <source>Weights of the echo canceller</source>
        <translation>回声抵消器的影响</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="296"/>
        <source>This shows the weights of the echo canceller, with time increasing downwards and frequency increasing to the right.&lt;br /&gt;Ideally, this should be black, indicating no echo exists at all. More commonly, you&apos;ll have one or more horizontal stripes of bluish color representing time delayed echo. You should be able to see the weights updated in real time.&lt;br /&gt;Please note that as long as you have nothing to echo off, you won&apos;t see much useful data here. Play some music and things should stabilize. &lt;br /&gt;You can choose to view the real or imaginary parts of the frequency-domain weights, or alternately the computed modulus and phase. The most useful of these will likely be modulus, which is the amplitude of the echo, and shows you how much of the outgoing signal is being removed at that time step. The other viewing modes are mostly useful to people who want to tune the echo cancellation algorithms.&lt;br /&gt;Please note: If the entire image fluctuates massively while in modulus mode, the echo canceller fails to find any correlation whatsoever between the two input sources (speakers and microphone). Either you have a very long delay on the echo, or one of the input sources is configured wrong.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>AudioWizard</name>
    <message>
        <location filename="AudioWizard.ui" line="14"/>
        <source>Audio Tuning Wizard</source>
        <translation>音频调节精灵</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="18"/>
        <source>Introduction</source>
        <translation>介绍</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="21"/>
        <source>Welcome to the Mumble Audio Wizard</source>
        <translation>欢迎使用 Mumble 音频精灵</translation>
    </message>
    <message>
        <source>This is the audio tuning wizard for Mumble. This will help you correctly set the input levels of your sound card, and also set the correct parameters for sound processing in Mumble.</source>
        <translation type="obsolete">这是 Mumble 音频调节精灵。这将帮你正确设置你的声卡的输入级别，并正确设置 Mumble 的声音处理参数。</translation>
    </message>
    <message>
        <source>Please be aware that as long as this wizard is active, audio will be looped locally to allow you to listen to it, and no audio will be sent to the server.</source>
        <translation type="obsolete">请注意当音频精灵活跃时，音频将被在本地环回以允许你听到它，没有音频会被发送到服务器。</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="545"/>
        <source>Finished</source>
        <translation>完成</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="548"/>
        <source>Enjoy using Mumble</source>
        <translation>Mumble 祝你使用愉快</translation>
    </message>
    <message>
        <source>Congratulations. You should now be ready to enjoy a richer sound experience with Mumble.</source>
        <translation type="obsolete">恭喜。现在你可以准备好享受 Mumble 带给你的丰富声音体验.</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="56"/>
        <source>Device selection</source>
        <translation>设备选择</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="59"/>
        <source>Selecting the input and output device to use with Mumble.</source>
        <translation>选择 Mumble 用来输入和输出的设备.</translation>
    </message>
    <message>
        <source>Input device</source>
        <translation type="obsolete">输入设备</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="71"/>
        <source>This is the device your microphone is connected to.</source>
        <translation>这是你的麦克风连接到的设备。</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="81"/>
        <location filename="AudioWizard.ui" line="153"/>
        <source>System</source>
        <translation>系统</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="91"/>
        <source>Input method for audio</source>
        <translation>音频输入方法</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="94"/>
        <source>&lt;b&gt;This is the input method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation>&lt;b&gt;这是用于音频的输入方法。&lt;/b&gt;&lt;br /&gt;很有可能你想使用 DirectSound。</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="101"/>
        <location filename="AudioWizard.ui" line="173"/>
        <source>Device</source>
        <translation>设备</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="111"/>
        <source>Input device to use</source>
        <translation>使用的输入设备</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="114"/>
        <source>&lt;b&gt;Selects which sound card to use for audio input.&lt;/b&gt;</source>
        <translation>&lt;b&gt;选择哪个声卡用于音频输入。&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="121"/>
        <source>Cancel echo from headset or speakers</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="127"/>
        <source>Use echo cancellation</source>
        <translation>使用回声消除</translation>
    </message>
    <message>
        <source>Cancel echo from headset or speakers.</source>
        <translation type="obsolete">消除来自耳机或扬声器的回声.</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="124"/>
        <source>This enables echo cancellation of outgoing audio, which helps both on speakers and on headsets.</source>
        <translation>启用送出音频的回声消除，这在扬声器和耳机上都有帮助。</translation>
    </message>
    <message>
        <source>Output device</source>
        <translation type="obsolete">输出设备</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="143"/>
        <source>This is the device your speakers or headphones are connected to.</source>
        <translation>这是你的扬声器或耳机连接到的设备.</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="163"/>
        <source>Output method for audio</source>
        <translation>音频输出方法</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="166"/>
        <source>&lt;b&gt;This is the Output method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation>&lt;b&gt;这是用于音频输出的方法。&lt;/b&gt;&lt;br /&gt;很有可能你想使用 DirectSound。</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="183"/>
        <source>Output device to use</source>
        <translation>使用的输出设备</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="186"/>
        <source>&lt;b&gt;Selects which sound card to use for audio Output.&lt;/b&gt;</source>
        <translation>&lt;b&gt;选择哪个声卡用于音频输出。&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="199"/>
        <source>Enable positional audio</source>
        <translation>启用定位音频</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="193"/>
        <source>Allows positioning of sound</source>
        <oldsource>Allows positioning of sound.</oldsource>
        <translation type="unfinished">允许声音定位.</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="27"/>
        <source>&lt;p&gt;
This is the audio tuning wizard for Mumble. This will help you correctly set the input levels of your sound card, and also set the correct parameters for sound processing in Mumble.
&lt;/p&gt;
&lt;p&gt;
Please be aware that as long as this wizard is active, audio will be looped locally to allow you to listen to it, and no audio will be sent to the server.
&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="65"/>
        <source>Input Device</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="137"/>
        <source>Output Device</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="196"/>
        <source>This allows Mumble to use positional audio to place voices.</source>
        <translation>这允许 Mumble 使用定位音频来放置语音。</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="232"/>
        <source>&lt;p&gt;
To keep latency to an absolute minimum, it&apos;s important to buffer as little audio as possible on the soundcard. However, many soundcards report that they require a much smaller buffer than what they can actually work with, so the only way to set this value is to try and fail.
&lt;/p&gt;
&lt;p&gt;
You should hear a voice sample. Change the slider below to the lowest value which gives &lt;b&gt;no&lt;/b&gt; interruptions or jitter in the sound. Please note that local echo is disabled during this test.
&lt;/p&gt;
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="254"/>
        <source>Amount of data to buffer</source>
        <translation type="unfinished">缓冲数据大小</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="257"/>
        <source>This sets the amount of data to pre-buffer in the output buffer. Experiment with different values and set it to the lowest which doesn&apos;t cause rapid jitter in the sound.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="315"/>
        <source>&lt;p&gt;
Open your sound control panel and go to the recording settings. Make sure the microphone is selected as active input with maximum recording volume. If there&apos;s an option to enable a &quot;Microphone boost&quot; make sure it&apos;s checked.
&lt;/p&gt;
&lt;p&gt;
Speak loudly, as when you are annoyed or excited. Decrease the volume in the sound control panel until the bar below stays as high as possible in the blue and green but &lt;b&gt;not&lt;/b&gt; the red zone while you speak.
&lt;/p&gt;
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="490"/>
        <source>Positional Audio</source>
        <translation>定位音频</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="493"/>
        <source>Adjusting attenuation of positional audio.</source>
        <translation>调节定位音频的衰减量.</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="499"/>
        <source>&lt;p&gt;
Mumble supports positional audio for some games, and will position the voice of other users relative to their position in game. Depending on their position, the volume of the voice will be changed between the speakers to simulate the direction and distance the other user is at. Such positioning depends on your speaker configuration being correct in your operating system, so a test is done here.
&lt;/p&gt;
&lt;p&gt;
The graph below shows the position of &lt;font color=&quot;red&quot;&gt;you&lt;/font&gt;, the &lt;font color=&quot;yellow&quot;&gt;speakers&lt;/font&gt; and a &lt;font color=&quot;green&quot;&gt;moving sound source&lt;/font&gt; as if seen from above. You should hear the audio move between the channels.
&lt;/p&gt;
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="515"/>
        <source>Use headphones instead of speakers</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="554"/>
        <source>&lt;p&gt;
Congratulations. You should now be ready to enjoy a richer sound experience with Mumble.
&lt;/p&gt;
&lt;p&gt;
Mumble is under continuous development, and the development team wants to focus on the features that benefit the most users. To this end, Mumble supports submitting anonymous statistics about your configuration to the developers. These statistics are essential for future development, and also make sure the features you use aren&apos;t deprecated.
&lt;/p&gt;
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mumble supports positional audio for some games, and will position the voice of other players relative to their position in game. Depending on their position, the volume of the voice will be changed between the speakers to simulate the direction and distance the other player is at. Such positioning depends on your speaker configuration being correct in your operating system, so a test is done here.</source>
        <translation type="obsolete">Mumble 支持在某些游戏里使用定位音频，并能根据其他玩家在游戏中的位置来定位他们的发言。音量将会根据说话者的位置来改变以模拟他相对于其他玩家的方向和距离。定位音频依赖于你的操作系统正确的配置了扬声器，因此我们对扬声器进行测试。</translation>
    </message>
    <message>
        <source>The graph below shows the position of &lt;font color=&quot;red&quot;&gt;you&lt;/font&gt;, the &lt;font color=&quot;yellow&quot;&gt;speakers&lt;/font&gt; and a &lt;font color=&quot;green&quot;&gt;moving sound source&lt;/font&gt; as if seen from above. You should hear the audio move between the channels.</source>
        <translation type="obsolete">下图中显示&lt;font color=&quot;red&quot;&gt;你&lt;/font&gt;的位置，&lt;font color=&quot;yellow&quot;&gt;扬声器&lt;/font&gt;和一个&lt;font color=&quot;green&quot;&gt;移动的声源&lt;/font&gt;。你应该听到音频在声道间切换.</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="521"/>
        <source>Use headphones</source>
        <translation>使用耳机</translation>
    </message>
    <message>
        <source>Use headphones instead of speakers.</source>
        <translation type="obsolete">使用耳机而不是扬声器.</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="518"/>
        <source>This ignores the OS speaker configuration and configures the positioning for headphones instead.</source>
        <translation>这将忽略操作系统的扬声器配置并为耳机配置音频定位.</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="306"/>
        <source>Volume tuning</source>
        <translation>音量调节</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="309"/>
        <source>Tuning microphone hardware volume to optimal settings.</source>
        <translation>调节麦克风硬件音量到最佳设置。</translation>
    </message>
    <message>
        <source>Open your sound control panel and go to the recording settings. Make sure the microphone is selected as active input with maximum recording volume. If there&apos;s an option to enable a &quot;Microphone boost&quot; make sure it&apos;s checked.</source>
        <translation type="obsolete">打开你的声音控制面板然后打开录音设置。确保麦克风被选为当前使用的输入并使用最大录音音量。如果有启用“麦克风加强”选项，请确保它被选上。</translation>
    </message>
    <message>
        <source>Speak loudly, as when you are annoyed or excited. Decrease the volume in the sound control panel until the bar below stays as high as possible in the blue and green but &lt;b&gt;not&lt;/b&gt; the red zone while you speak.</source>
        <translation type="obsolete">请大声讲话，就像你在愤怒或兴奋时那样。减小你的声音控制面板里的音量直到下面的进度条在你讲话时尽可能的停留在蓝区和绿区，而&lt;b&gt;不是&lt;/b&gt;红区里.</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="341"/>
        <source>Now talk softly, as you would when talking late at night and you don&apos;t want to disturb anyone. Adjust the slider below so that the bar moves into green when you talk, but stays blue while you&apos;re silent.</source>
        <translation>现在轻声讲话，就像你在深夜时不想打扰任何人一样。调节下面的滑块直到下面的进度条在你讲话时进入绿区，而在你沉默时停留在蓝区。</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="381"/>
        <source>Voice Activity Detection</source>
        <translation>语言活动检测</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="384"/>
        <source>Letting Mumble figure out when you&apos;re talking and when you&apos;re silent.</source>
        <translation>让 Mumble 分辨出你是在讲话还是在沉默.</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="390"/>
        <source>This will help Mumble figure out when you are talking. The first step is selecting which data value to use.</source>
        <translation>这将帮助 Mumble 知道你是在讲话。第一步是选择用哪个数据值。</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="421"/>
        <source>Raw amplitude from input</source>
        <translation>输入的原始振幅</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="414"/>
        <source>Signal-To-Noise ratio</source>
        <translation>信噪比</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="444"/>
        <source>Next you need to adjust the following slider. The first few utterances you say should end up in the green area (definitive speech). While talking, you should stay inside the yellow (might be speech) and when you&apos;re not talking, everything should be in the red (definitively not speech).</source>
        <oldsource>Next you need to adjust the following two sliders. The first few utterances you say should end up in the green area (definitive speech). While talking, you should stay inside the yellow (might be speech) and when you&apos;re not talking, everything should be in the red (definitively not speech).</oldsource>
        <translation type="unfinished">下一步你需要调节下面两个滑块。你最初讲几句话时进度条应该停留在绿区（确定是讲话）。当你讲话时，进度条应该停留在黄区（可能是讲话）；当你没讲话时，进度条应该停留在红区(明确没在讲话）.</translation>
    </message>
    <message>
        <source>Last, you need to adjust the grace time. Mumble may detect this much non-speech and still keep transmitting before it breaks off. This allows you to catch your breath while speaking.</source>
        <translation type="obsolete">最后，你需要调节宽限时间.在宽限时间内 Mumble 能够侦测到你没在讲话并继续传送，然后断开连接。这允许你在讲话时喘息。</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="223"/>
        <source>Device tuning</source>
        <translation>设备调节</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="226"/>
        <source>Changing hardware output delays to their minimum value.</source>
        <translation>修改硬件输出延时到它们的最小值。</translation>
    </message>
    <message>
        <source>To keep latency to an absolute minium, it&apos;s important to buffer as little audio as possible on the soundcard. However, many soundcards report that they require a much smaller buffer than what they can actually work with, so the only way to set this value is to try and fail.</source>
        <translation type="obsolete">为了保证延时是绝对最小值，让声卡缓冲尽可能少的音频非常重要。然而大多数声卡声称它们需要比实际需求更小的缓冲区，因此设置这个值的唯一方式是去尝试。</translation>
    </message>
    <message>
        <source>You should hear a single tone that&apos;s changing in frequency. Change the slider below to the lowest value which gives &lt;b&gt;no&lt;/b&gt; interruptions or jitter in the sound. Please note that local echo is disabled during this test to improve audio path recognition.</source>
        <translation type="obsolete">你应该听到一个不断改变频率的单音。移动下面的滑块直到它是&lt;b&gt;不&lt;/b&gt;导致声音中断或颤音的最小值。请注意在测试中本地回声将被禁止以改善音频路径识别.</translation>
    </message>
    <message>
        <source>Amount of data to buffer.</source>
        <translation type="obsolete">缓冲数据大小。</translation>
    </message>
    <message>
        <source>This sets the amount of data to prebuffer in the output buffer. Experiment with different values and set it to the lowest which doesn&apos;t cause rapid jitter in the sound.</source>
        <translation type="obsolete">设置输出缓冲区里前置缓冲的数据大小。试验不同值然后设置成不引起快速颤音的最低值.</translation>
    </message>
    <message>
        <source>If you position the microphone so it can pick up the speakers or headset, Mumble will measure the total audio path delay in your system; this means the delay from a sample is placed in an outbound buffer until it&apos;s found in a matching incoming buffer.</source>
        <translation type="obsolete">如果你将麦克风放到能捕捉扬声器或耳机声音的地方, Mumble 将计算你的系统中的总的音频路径延时;这个延时是自一个声音样本被放到输出缓冲区后直到它在匹配的输入缓冲区里被找到的时间间隔.</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="273"/>
        <source>%1ms</source>
        <translation>%1毫秒</translation>
    </message>
    <message>
        <source>Audio path is %1ms long.</source>
        <translation type="obsolete">音频路径长%1毫秒.</translation>
    </message>
    <message>
        <source>Audio path cannot be determined. Input not recognized.</source>
        <translation type="obsolete">音频路径无法被确定。输入无法识别.</translation>
    </message>
    <message>
        <source>%1 s</source>
        <translation type="obsolete">%1 秒</translation>
    </message>
    <message>
        <source>Mumble is under continuous development, and the development team wants to focus on the features that benefit the most users. To this end, Mumble supports submitting anonymous statistics about your configuration to the developers. These statistcs are essential for future development, and also make sure the features you use aren&apos;t deprecated.</source>
        <translation type="obsolete">Mumble 正处于开发阶段，开发团队致力于开发能使大多数用户受益的功能。Mumble 支持匿名递交你的配置统计信息给开发人员。这些统计信息对于将来的开发非常重要，并确保你使用的功能不会被弃用。</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="570"/>
        <source>Submit anonymous statistics to the Mumble project</source>
        <translation>递交匿名统计给 Mumble 项目组</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="402"/>
        <source>Push To Talk:</source>
        <translation>按键通话:</translation>
    </message>
</context>
<context>
    <name>BanEditor</name>
    <message>
        <location filename="BanEditor.ui" line="14"/>
        <source>Mumble - Edit Bans</source>
        <translation>Mumble - 编辑黑名单</translation>
    </message>
    <message>
        <location filename="BanEditor.ui" line="29"/>
        <source>&amp;Address</source>
        <translation>&amp;地址</translation>
    </message>
    <message>
        <location filename="BanEditor.ui" line="39"/>
        <source>&amp;Mask</source>
        <translation>&amp;子网掩码</translation>
    </message>
    <message>
        <location filename="BanEditor.ui" line="72"/>
        <source>Reason</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="BanEditor.ui" line="92"/>
        <source>Start</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="BanEditor.ui" line="99"/>
        <source>End</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="BanEditor.ui" line="113"/>
        <source>User</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="BanEditor.ui" line="127"/>
        <source>Hash</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="BanEditor.ui" line="145"/>
        <source>&amp;Add</source>
        <translation>&amp;添加</translation>
    </message>
    <message>
        <location filename="BanEditor.ui" line="152"/>
        <source>&amp;Update</source>
        <translation>&amp;更新</translation>
    </message>
    <message>
        <location filename="BanEditor.ui" line="159"/>
        <source>&amp;Remove</source>
        <translation>&amp;删除</translation>
    </message>
</context>
<context>
    <name>CertView</name>
    <message>
        <location filename="Cert.cpp" line="50"/>
        <source>Name</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.cpp" line="57"/>
        <source>Email</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.cpp" line="64"/>
        <source>Issuer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.cpp" line="97"/>
        <source>(none)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.cpp" line="103"/>
        <source>Self-signed</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>CertWizard</name>
    <message>
        <location filename="Cert.cpp" line="188"/>
        <source>Resolving domain %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.cpp" line="194"/>
        <source>Unable to validate email.&lt;br /&gt;Enter a valid (or blank) email to continue.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.cpp" line="202"/>
        <source>There was an error generating your certificate.&lt;br /&gt;Please try again.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.cpp" line="210"/>
        <source>Your certificate and key could not be exported to PKCS#12 format. There might be an error in your certificate.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.cpp" line="215"/>
        <source>The file could not be opened for writing. Please use another file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.cpp" line="221"/>
        <source>The file could not be written successfully. Please use another file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.cpp" line="228"/>
        <source>The file could not be opened for reading. Please use another file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.cpp" line="234"/>
        <source>The file is empty or could not be read. Please use another file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.cpp" line="239"/>
        <source>The file did not contain a valid certificate and key. Please use another file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.cpp" line="261"/>
        <source>Select file to export certificate to</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.cpp" line="298"/>
        <source>Select file to import certificate from</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.cpp" line="351"/>
        <source>Unable to resolve domain.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.cpp" line="394"/>
        <source>Mumble User</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Certificates</name>
    <message>
        <location filename="Cert.ui" line="14"/>
        <source>Certificate Management</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="18"/>
        <source>Certificate Authentication</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="21"/>
        <source>Authenticating to servers without using passwords</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="30"/>
        <source>&lt;p&gt;Mumble can use certificates to authenticate with servers. Using certificates avoids passwords, meaning you don&apos;t need to disclose any password to the remote site. It also enables very easy user registration.&lt;/p&gt;&lt;p&gt;While Mumble can work without certificates, the majority of servers will expect you to have one.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="40"/>
        <location filename="Cert.ui" line="261"/>
        <location filename="Cert.ui" line="348"/>
        <source>Current certificate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="43"/>
        <source>This is the certificate Mumble currently uses.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="46"/>
        <location filename="Cert.ui" line="267"/>
        <source>Current Certificate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="53"/>
        <location filename="Cert.ui" line="59"/>
        <source>Create a new certificate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="56"/>
        <source>This will create a new certificate.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="79"/>
        <source>Import certificate from file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="82"/>
        <source>This will import a certificate from file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="85"/>
        <source>Import a certificate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="105"/>
        <location filename="Cert.ui" line="294"/>
        <source>Export Certificate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="108"/>
        <source>This will export a certificate to file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="111"/>
        <source>Export current certificate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="129"/>
        <source>Import Certificate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="132"/>
        <source>PKCS #12 Certificate import</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="141"/>
        <source>&lt;p&gt;Mumble can import certificates stored in PKCS #12 format. This is the format used when exporting a key from Mumble, and also when exporting keys from Firefox, Internet Explorer, Opera etc.&lt;/p&gt;&lt;p&gt;If the file is password protected, you will need the password to import the certificate.&lt;/p&gt;</source>
        <oldsource>&lt;p&gt;Mumble can import certificates stored in PKCS #12 format. This is the format used when exporting a key from Mumble, and also when exporting keys from FireFox, Internet Explorer, Opera etc.&lt;/p&gt;&lt;p&gt;If the file is password protected, you will need the password to import the certificate.&lt;/p&gt;</oldsource>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="151"/>
        <source>Import from</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="161"/>
        <source>Filename to import from</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="164"/>
        <source>This is the filename you wish to import a certificate from.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="171"/>
        <source>Select file to import from</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="174"/>
        <source>This opens a file selection dialog to choose a file to import a certificate from.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="177"/>
        <source>Open...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="188"/>
        <source>Password</source>
        <translation type="unfinished">密码</translation>
    </message>
    <message>
        <location filename="Cert.ui" line="198"/>
        <source>Password for PKCS#12 file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="201"/>
        <source>This is the password for the PKCS#12 file containing your certificate.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="208"/>
        <source>Certificate to import</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="211"/>
        <source>This is the certificate you are importing.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="214"/>
        <location filename="Cert.ui" line="354"/>
        <source>Certificate Details</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="222"/>
        <source>Replace Certificate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="225"/>
        <source>Replace existing certificate with new certificate?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="237"/>
        <source>&lt;p&gt;You already have a certificate stored in Mumble, and you are about to replace it.&lt;/p&gt;
&lt;p&gt;If you are upgrading to a certificate issued to you by a trusted CA and the email addresses match your current certificate, this is completely safe, and servers you connect to will automatically recognize the strong certificate for your email address.
&lt;/p&gt;
&lt;p&gt;If this is not the case, you will no longer be recognized by any server you previously have authenticated with. If you haven&apos;t been registered on any server yet, this is nothing to worry about.
&lt;/p&gt;
&lt;p&gt;
Are you sure you wish to replace your certificate?
&lt;/p&gt;
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="264"/>
        <source>This is the certificate Mumble currently uses. It will be replaced.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="280"/>
        <source>New certificate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="283"/>
        <source>This is the new certificate that will replace the old one.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="286"/>
        <location filename="Cert.ui" line="362"/>
        <source>New Certificate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="297"/>
        <source>Make a backup of your certificate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="306"/>
        <source>&lt;p&gt;If you ever lose your current certificate, which will happen if your computer suffers a hardware failure or you reinstall your machine, you will no longer be able to authenticate to any server you are registered on. It is therefore &lt;b&gt;mandatory&lt;/b&gt; that you make a backup of your certificate. We strongly recommend you store this backup on removable storage, such as an USB memory stick.&lt;/p&gt;
&lt;p&gt;Note that this file will not be encrypted, and if anyone gains access to it, they will be able to impersonate you, so take good care of it.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="317"/>
        <source>Export to</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="327"/>
        <source>Filename to export to</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="330"/>
        <source>This is the filename you wish to export a certificate to.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="337"/>
        <source>Save As...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="351"/>
        <source>This is the certificate Mumble currently uses. It will be exported.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="365"/>
        <source>Generate a new certificate for strong authentication</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="374"/>
        <source>&lt;p&gt;Mumble will now generate a strong certificate for authentication to servers.&lt;/p&gt;&lt;p&gt;If you wish, you may provide some additional information to be stored in the certificate, which will be presented to servers when you connect. If you provide a valid email address, you can upgrade to a CA issued email certificate later on, which provides strong identification.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="384"/>
        <source>Name</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="394"/>
        <source>Email</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="404"/>
        <source>Your email address (e.g. johndoe@mumble.info)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="407"/>
        <source>This is your email address. It is strongly recommended to provide a valid email address, as this will allow you to upgrade to a strong certificate without authentication problems.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="421"/>
        <source>Your name (e.g. John Doe)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="424"/>
        <source>This is your name, and will be filled out in the certificate. This field is entirely optional.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="432"/>
        <source>Finish</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="435"/>
        <source>Certificate-based authentication is ready for use</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="444"/>
        <source>Enjoy using Mumble with strong authentication.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ChanACL</name>
    <message>
        <location filename="../ACL.cpp" line="214"/>
        <source>None</source>
        <translation>无</translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="218"/>
        <source>Traverse</source>
        <translation>遍历</translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="220"/>
        <source>Enter</source>
        <translation>进入</translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="222"/>
        <source>Speak</source>
        <translation>发言</translation>
    </message>
    <message>
        <source>AltSpeak</source>
        <translation type="obsolete">试图发言</translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="226"/>
        <source>Mute/Deafen</source>
        <translation>禁音/耳聋</translation>
    </message>
    <message>
        <source>Move/Kick</source>
        <translation type="obsolete">移动/踢出</translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="230"/>
        <source>Make channel</source>
        <translation>创建频道</translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="232"/>
        <source>Make temporary</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="234"/>
        <source>Link channel</source>
        <translation>链接频道</translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="147"/>
        <source>This represents no privileges.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="149"/>
        <source>This represents total access to the channel, including the ability to change group and ACL information. This privilege implies all other privileges.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="152"/>
        <source>This represents the permission to traverse the channel. If a user is denied this privilege, he will be unable to access this channel and any sub-channels in any way, regardless of other permissions in the sub-channels.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="156"/>
        <source>This represents the permission to join the channel. If you have a hierarchical channel structure, you might want to give everyone Traverse, but restrict Enter in the root of your hierarchy.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="159"/>
        <source>This represents the permission to speak in a channel. Users without this privilege will be suppressed by the server (seen as muted), and will be unable to speak until they are unmuted by someone with the appropriate privileges.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="163"/>
        <source>This represents the permission to whisper to this channel from the outside. This works exactly like the &lt;i&gt;speak&lt;/i&gt; privilege, but applies to packets spoken with the Whisper key held down. This may be used to broadcast to a hierarchy of channels without linking.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="167"/>
        <source>This represents the permission to mute and deafen other users. Once muted, a user will stay muted until he is unmuted by another privileged user or reconnects to the server.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="170"/>
        <source>This represents the permission to move a user to another channel or kick him from the server. To actually move the user, either the moving user must have Move privileges in the destination channel, or the user must normally be allowed to enter the channel. Users with this privilege can move users into channels the target user normally wouldn&apos;t have permission to enter.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="175"/>
        <source>This represents the permission to make sub-channels. The user making the sub-channel will be added to the admin group of the sub-channel.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="178"/>
        <source>This represents the permission to make a temporary subchannel. The user making the sub-channel will be added to the admin group of the sub-channel. Temporary channels are not stored and disappear when the last user leaves.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="181"/>
        <source>This represents the permission to link channels. Users in linked channels hear each other, as long as the speaking user has the &lt;i&gt;speak&lt;/i&gt; privilege in the channel of the listener. You need the link privilege in both channels to create a link, but just in either channel to remove it.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="185"/>
        <source>This represents the permission to write text messages to other users in this channel.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="187"/>
        <source>This represents the permission to forcibly remove users from the server.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="189"/>
        <source>This represents the permission to permanently remove users from the server.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="191"/>
        <source>This represents the permission to register and unregister users on the server.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="193"/>
        <source>This represents the permission to register oneself on the server.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="224"/>
        <source>Whisper</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="228"/>
        <source>Move</source>
        <translation type="unfinished">移动</translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="236"/>
        <source>Text message</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="238"/>
        <source>Kick</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="240"/>
        <source>Ban</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="242"/>
        <source>Register User</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="244"/>
        <source>Register Self</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="216"/>
        <source>Write ACL</source>
        <translation>创建访问控制列表</translation>
    </message>
</context>
<context>
    <name>ChatbarLineEdit</name>
    <message>
        <location filename="CustomElements.cpp" line="70"/>
        <source>Type chat message here</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ClientPlayer</name>
    <message>
        <source>Authenticated</source>
        <translation type="obsolete">已认证</translation>
    </message>
    <message>
        <source>Muted (server)</source>
        <translation type="obsolete">静音(服务器)</translation>
    </message>
    <message>
        <source>Deafened (server)</source>
        <translation type="obsolete">耳聋(服务器)</translation>
    </message>
    <message>
        <source>Local Mute</source>
        <translation type="obsolete">本地静音</translation>
    </message>
    <message>
        <source>Muted (self)</source>
        <translation type="obsolete">静音(自己）</translation>
    </message>
    <message>
        <source>Deafened (self)</source>
        <translation type="obsolete">耳聋(自己)</translation>
    </message>
</context>
<context>
    <name>ClientUser</name>
    <message>
        <location filename="ClientUser.cpp" line="97"/>
        <source>Friend</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ClientUser.cpp" line="99"/>
        <source>Authenticated</source>
        <translation type="unfinished">已认证</translation>
    </message>
    <message>
        <location filename="ClientUser.cpp" line="101"/>
        <source>Muted (server)</source>
        <translation type="unfinished">静音(服务器)</translation>
    </message>
    <message>
        <location filename="ClientUser.cpp" line="103"/>
        <source>Deafened (server)</source>
        <translation type="unfinished">耳聋(服务器)</translation>
    </message>
    <message>
        <location filename="ClientUser.cpp" line="105"/>
        <source>Local Mute</source>
        <translation type="unfinished">本地静音</translation>
    </message>
    <message>
        <location filename="ClientUser.cpp" line="107"/>
        <source>Muted (self)</source>
        <translation type="unfinished">静音(自己）</translation>
    </message>
    <message>
        <location filename="ClientUser.cpp" line="109"/>
        <source>Deafened (self)</source>
        <translation type="unfinished">耳聋(自己)</translation>
    </message>
</context>
<context>
    <name>ConfigDialog</name>
    <message>
        <location filename="ConfigDialog.cpp" line="51"/>
        <location filename="ConfigDialog.mm" line="70"/>
        <source>Accept changes</source>
        <translation>确定</translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="52"/>
        <location filename="ConfigDialog.mm" line="71"/>
        <source>This button will accept current settings and return to the application.&lt;br /&gt;The settings will be stored to disk when you leave the application.</source>
        <translation>该按钮将接受当前设置并返回到应用程序。&lt;br/&gt;当你退出应用程序时这些设置将被存盘。</translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="56"/>
        <location filename="ConfigDialog.mm" line="75"/>
        <source>Reject changes</source>
        <translation>取消</translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="57"/>
        <location filename="ConfigDialog.mm" line="76"/>
        <source>This button will reject all changes and return to the application.&lt;br /&gt;The settings will be reset to the previous positions.</source>
        <translation>该按钮将拒绝所有修改并返回到应用程序。&lt;br/&gt;这些设置将被重置到先前的设定。</translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="61"/>
        <location filename="ConfigDialog.mm" line="80"/>
        <source>Apply changes</source>
        <translation>应用</translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="62"/>
        <location filename="ConfigDialog.mm" line="81"/>
        <source>This button will immediately apply all changes.</source>
        <translation>该按钮将立即应用所有修改.</translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="65"/>
        <location filename="ConfigDialog.mm" line="84"/>
        <source>Undo changes for current page</source>
        <translation>撤销</translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="66"/>
        <location filename="ConfigDialog.mm" line="85"/>
        <source>This button will revert any changes done on the current page to the most recent applied settings.</source>
        <translation>该按钮将复原对当前页的任意修改到最近使用的设置。</translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="69"/>
        <location filename="ConfigDialog.mm" line="88"/>
        <source>Restore defaults for current page</source>
        <translation>恢复默认值</translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="70"/>
        <location filename="ConfigDialog.mm" line="89"/>
        <source>This button will restore the settings for the current page only to their defaults. Other pages will not be changed.&lt;br /&gt;To restore all settings to their defaults, you will have to use this button on every page.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This button will restore the settings for the current page only to their defaults. Other pages will be not be changed.&lt;br /&gt;To restore all settings to their defaults, you will have to use this button on every page.</source>
        <translation type="obsolete">该按钮将恢复当前页设置到默认值。其他页面的设置将不被修改。&lt;b r/&gt;为了恢复其他页面上的设置到默认值，你需要在每个页面上都使用该按钮。</translation>
    </message>
    <message>
        <location filename="ConfigDialog.ui" line="14"/>
        <source>Mumble Configuration</source>
        <translation>Mumble 配置</translation>
    </message>
    <message>
        <location filename="ConfigDialog.ui" line="54"/>
        <location filename="ConfigDialogDelegate.mm" line="135"/>
        <source>Advanced</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Tab 1</source>
        <translation type="obsolete">标签 1</translation>
    </message>
</context>
<context>
    <name>ConnectDialog</name>
    <message>
        <location filename="ConnectDialog.cpp" line="837"/>
        <source>Connecting to %1</source>
        <translation>正在连接到 %1</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="385"/>
        <location filename="ConnectDialog.cpp" line="837"/>
        <source>Enter username</source>
        <translation>输入用户名</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation type="obsolete">未知</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="385"/>
        <source>Adding host %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="448"/>
        <location filename="ConnectDialog.ui" line="48"/>
        <source>Servername</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="449"/>
        <source>Hostname</source>
        <translation type="unfinished">主机名</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="452"/>
        <source>Bonjour name</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="455"/>
        <source>Port</source>
        <translation type="unfinished">端口</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="456"/>
        <source>Addresses</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="459"/>
        <source>Website</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="462"/>
        <source>Packet loss</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="465"/>
        <source>Ping (80%)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="465"/>
        <location filename="ConnectDialog.cpp" line="467"/>
        <source>%1 ms</source>
        <translation type="unfinished">%1 毫秒</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="467"/>
        <source>Ping (95%)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="469"/>
        <source>Bandwidth</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="469"/>
        <source>%1 kbit/s</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="470"/>
        <location filename="ConnectDialog.ui" line="58"/>
        <source>Users</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="471"/>
        <source>Version</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="714"/>
        <source>Connect</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="716"/>
        <location filename="ConnectDialog.ui" line="83"/>
        <source>Add New...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="753"/>
        <source>Filters</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="1401"/>
        <source>Mumble</source>
        <translation></translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="1401"/>
        <source>Failed to fetch server list</source>
        <translation>无法获取服务器列表</translation>
    </message>
    <message>
        <source>New</source>
        <translation type="obsolete">新建</translation>
    </message>
    <message>
        <source>-Unnamed entry-</source>
        <translation type="obsolete">-未命名条目-</translation>
    </message>
    <message>
        <source>Add</source>
        <translation type="obsolete">添加</translation>
    </message>
    <message>
        <source>Update</source>
        <translation type="obsolete">更新</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui" line="14"/>
        <source>Mumble Server Connect</source>
        <translation>Mumble 服务器连接</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui" line="53"/>
        <source>Ping</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui" line="73"/>
        <source>Remove from Favorites</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui" line="78"/>
        <source>Edit...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui" line="86"/>
        <source>Add custom server</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui" line="91"/>
        <source>Add to Favorites</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui" line="96"/>
        <source>Open Webpage</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui" line="107"/>
        <source>Show Reachable</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui" line="110"/>
        <source>Show all servers that respond to ping</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui" line="121"/>
        <source>Show Populated</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui" line="124"/>
        <source>Show all servers with users</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui" line="132"/>
        <source>Show All</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui" line="135"/>
        <source>Show all servers</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui" line="140"/>
        <source>&amp;Copy</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui" line="143"/>
        <source>Copy favorite link to clipboard</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui" line="148"/>
        <source>&amp;Paste</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui" line="151"/>
        <source>Paste favorite from clipboard</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Custom Servers</source>
        <translation type="obsolete">&amp;自定义服务器</translation>
    </message>
    <message>
        <source>&amp;Label</source>
        <translation type="obsolete">&amp;标签</translation>
    </message>
    <message>
        <source>A&amp;ddress</source>
        <translation type="obsolete">&amp;地址</translation>
    </message>
    <message>
        <source>&amp;Port</source>
        <translation type="obsolete">&amp;端口</translation>
    </message>
    <message>
        <source>&amp;Username</source>
        <translation type="obsolete">&amp;用户名</translation>
    </message>
    <message>
        <source>&amp;Password</source>
        <translation type="obsolete">&amp;密码</translation>
    </message>
    <message>
        <source>&amp;Connect</source>
        <translation type="obsolete">&amp;连接</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;取消</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation type="obsolete">&amp;添加</translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation type="obsolete">&amp;删除</translation>
    </message>
    <message>
        <source>Server &amp;Browser</source>
        <translation type="obsolete">服务器&amp;浏览器</translation>
    </message>
    <message>
        <source>Label</source>
        <translation type="obsolete">标签</translation>
    </message>
    <message>
        <source>Address</source>
        <translation type="obsolete">地址</translation>
    </message>
    <message>
        <source>URL</source>
        <translation type="obsolete">网址</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">取消</translation>
    </message>
    <message>
        <source>C&amp;opy to custom</source>
        <translation type="obsolete">&amp;拷贝到自定义</translation>
    </message>
    <message>
        <source>&amp;View Webpage</source>
        <translation type="obsolete">&amp;查看主页</translation>
    </message>
</context>
<context>
    <name>ConnectDialogEdit</name>
    <message>
        <location filename="ConnectDialogEdit.ui" line="14"/>
        <source>Edit Server</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialogEdit.ui" line="20"/>
        <source>&amp;Servername</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialogEdit.ui" line="30"/>
        <source>Name of the server</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialogEdit.ui" line="33"/>
        <source>&lt;b&gt;Name&lt;/b&gt;&lt;br/&gt;
Name of the server. This is what the server will be named like in your serverlist and can be chosen freely.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialogEdit.ui" line="41"/>
        <source>A&amp;ddress</source>
        <translation type="unfinished">&amp;地址</translation>
    </message>
    <message>
        <location filename="ConnectDialogEdit.ui" line="51"/>
        <source>Internet address of the server.</source>
        <oldsource>Internet address of the server. </oldsource>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialogEdit.ui" line="54"/>
        <source>&lt;b&gt;Address&lt;/b&gt;&lt;br/&gt;
Internet address of the server. This can be a normal hostname, an IPv4/IPv6 address or a Bonjour service identifier. Bonjour service identifiers have to be prefixed with a &apos;@&apos; to be recognized by Mumble.</source>
        <oldsource>&lt;b&gt;Address&lt;/b&gt;&lt;/br&gt;
Internet address of the server. This can be a normal hostname, an ipv4/6 address or a bonjour service identifier. Bonjour service identifiers have to be prefixed with a &apos;@&apos; to be recognized by Mumble.</oldsource>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialogEdit.ui" line="62"/>
        <source>&amp;Port</source>
        <translation type="unfinished">&amp;端口</translation>
    </message>
    <message>
        <location filename="ConnectDialogEdit.ui" line="72"/>
        <source>Port on which the server is listening</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialogEdit.ui" line="75"/>
        <source>&lt;b&gt;Port&lt;/b&gt;&lt;br/&gt;
Port on which the server is listening. If the server is identified by a Bonjour service identifier this field will be ignored.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialogEdit.ui" line="83"/>
        <source>&amp;Username</source>
        <translation type="unfinished">&amp;用户名</translation>
    </message>
    <message>
        <location filename="ConnectDialogEdit.ui" line="93"/>
        <source>Username to send to the server</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialogEdit.ui" line="96"/>
        <source>&lt;b&gt;Username&lt;/b&gt;&lt;br/&gt;
Username to send to the server. Be aware that the server can impose restrictions on how a username might look like. Also your username could already be taken by another user.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>CoreAudioSystem</name>
    <message>
        <location filename="CoreAudio.cpp" line="85"/>
        <source>Default Device</source>
        <translation type="unfinished">默认设备</translation>
    </message>
</context>
<context>
    <name>CrashReporter</name>
    <message>
        <location filename="CrashReporter.cpp" line="36"/>
        <source>Mumble Crash Report</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="CrashReporter.cpp" line="42"/>
        <source>&lt;p&gt;&lt;b&gt;We&apos;re terribly sorry, but it seems Mumble has crashed. Do you want to send a crash report to the Mumble developers?&lt;/b&gt;&lt;/p&gt;&lt;p&gt;The crash report contains a partial copy of Mumble&apos;s memory at the time it crashed, and will help the developers fix the problem.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="CrashReporter.cpp" line="50"/>
        <source>Email address (optional)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="CrashReporter.cpp" line="59"/>
        <source>Please briefly describe what you were doing at the time of the crash</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="CrashReporter.cpp" line="64"/>
        <source>Send Report</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="CrashReporter.cpp" line="67"/>
        <source>Don&apos;t send report</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="CrashReporter.cpp" line="92"/>
        <source>Crash upload successful</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="CrashReporter.cpp" line="92"/>
        <source>Thank you for helping make Mumble better!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="CrashReporter.cpp" line="94"/>
        <location filename="CrashReporter.cpp" line="96"/>
        <source>Crash upload failed</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="CrashReporter.cpp" line="94"/>
        <source>We&apos;re really sorry, but it appears the crash upload has failed with error %1 %2. Please inform a developer.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="CrashReporter.cpp" line="96"/>
        <source>This really isn&apos;t funny, but apparently there&apos;s a bug in the crash reporting code, and we&apos;ve failed to upload the report. You may inform a developer about error %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="CrashReporter.cpp" line="163"/>
        <source>Uploading crash report</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="CrashReporter.cpp" line="163"/>
        <source>Abort upload</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>DXAudioInput</name>
    <message>
        <location filename="DirectSound.cpp" line="174"/>
        <source>Default DirectSound Voice Input</source>
        <translation>默认 DirectSound 语音输入</translation>
    </message>
    <message>
        <location filename="DirectSound.cpp" line="637"/>
        <source>Opening chosen DirectSound Input device failed. No microphone capture will be done.</source>
        <translation>不能打开所选 DirectSound 输入设备。不能捕捉麦克风。</translation>
    </message>
    <message>
        <location filename="DirectSound.cpp" line="558"/>
        <source>Opening chosen DirectSound Input failed. Default device will be used.</source>
        <translation>不能打开所选 DirectSound 输入。将使用默认设备。</translation>
    </message>
    <message>
        <location filename="DirectSound.cpp" line="632"/>
        <source>Lost DirectSound input device.</source>
        <translation>丢失 DirectSound 输入设备。</translation>
    </message>
</context>
<context>
    <name>DXAudioOutput</name>
    <message>
        <location filename="DirectSound.cpp" line="134"/>
        <source>Default DirectSound Voice Output</source>
        <translation>默认 DirectSound 语音输出</translation>
    </message>
    <message>
        <location filename="DirectSound.cpp" line="382"/>
        <source>Opening chosen DirectSound Output failed. Default device will be used.</source>
        <translation>不能打开所选 DirectSound 输出。将使用默认设备。</translation>
    </message>
    <message>
        <location filename="DirectSound.cpp" line="460"/>
        <source>Lost DirectSound output device.</source>
        <translation>丢失 DirectSound 输出设备。</translation>
    </message>
    <message>
        <location filename="DirectSound.cpp" line="464"/>
        <source>Opening chosen DirectSound Output failed. No audio will be heard.</source>
        <translation>不能打开所选 DirectSound 输出。将听不到音频。</translation>
    </message>
</context>
<context>
    <name>Database</name>
    <message>
        <location filename="Database.cpp" line="83"/>
        <location filename="Database.cpp" line="90"/>
        <source>Mumble</source>
        <translation></translation>
    </message>
    <message>
        <location filename="Database.cpp" line="83"/>
        <source>Mumble failed to initialize a database in any
of the possible locations.</source>
        <translation>Mumble 无法在任何可能的位置里初始化数据库.</translation>
    </message>
    <message>
        <location filename="Database.cpp" line="90"/>
        <source>The database &apos;%1&apos; is read-only. Mumble can not store server settings (ie. SSL certificates) until you fix this problem.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>GlobalShortcut</name>
    <message>
        <location filename="GlobalShortcut.ui" line="17"/>
        <source>Shortcuts</source>
        <translation>快捷键</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.ui" line="65"/>
        <source>List of configured shortcuts</source>
        <translation>配置好的快捷键列表</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.ui" line="99"/>
        <source>Function</source>
        <translation>功能</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.ui" line="104"/>
        <source>Data</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="GlobalShortcut.ui" line="109"/>
        <source>Shortcut</source>
        <translation>快捷键</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.ui" line="114"/>
        <source>Suppress</source>
        <translation>禁止</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.ui" line="23"/>
        <source>Add new shortcut</source>
        <translation>添加新快捷键</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.ui" line="26"/>
        <source>This will add a new global shortcut</source>
        <translation>这将添加一个新的全局快捷键</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.ui" line="29"/>
        <source>&amp;Add</source>
        <translation>&amp;添加</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.ui" line="39"/>
        <source>Remove selected shortcut</source>
        <translation>移除所选快捷键</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.ui" line="42"/>
        <source>This will permanently remove a selected shortcut.</source>
        <translation>这将永久性的删除所选快捷键。</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.ui" line="45"/>
        <source>&amp;Remove</source>
        <translation>&amp;删除</translation>
    </message>
</context>
<context>
    <name>GlobalShortcutConfig</name>
    <message>
        <location filename="GlobalShortcut.cpp" line="647"/>
        <source>Shortcuts</source>
        <translation>快捷键</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.cpp" line="685"/>
        <source>Shortcut button combination.</source>
        <translation>快捷键按钮组合.</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.cpp" line="686"/>
        <source>&lt;b&gt;This is the global shortcut key combination.&lt;/b&gt;&lt;br /&gt;Double-click this field and then the desired key/button combo to rebind.</source>
        <translation>&lt;b&gt;这是全局快捷键组合。&lt;/b&gt;&lt;br /&gt;双击该区域然后绑定想要的按键/按钮组合。</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.cpp" line="690"/>
        <source>Suppress keys from other applications</source>
        <translation>禁止其他应用程序使用的按键</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.cpp" line="691"/>
        <source>&lt;b&gt;This hides the button presses from other applications.&lt;/b&gt;&lt;br /&gt;Enabling this will hide the button (or the last button of a multi-button combo) from other applications. Note that not all buttons can be suppressed.</source>
        <translation>&lt;b&gt;向其他应用程序隐藏这些按键。&lt;/b&gt;&lt;br/&gt;允许该选项将向其他应用程序隐藏指定按键（或者组合按键的最后一个按键）。注意并不是所有按键都能被禁用。</translation>
    </message>
</context>
<context>
    <name>GlobalShortcutMacInit</name>
    <message>
        <location filename="GlobalShortcut_macx.cpp" line="60"/>
        <source>Mumble has detected that it is unable to receive Global Shortcut events when it is in the background.&lt;br /&gt;&lt;br /&gt;This is because the Universal Access feature called &apos;Enable access for assistive devices&apos; is currently disabled.&lt;br /&gt;&lt;br /&gt;Please &lt;a href=&quot; &quot;&gt;enable this setting&lt;/a&gt; and continue when done.</source>
        <translation>Mumble 检测到当它位于后台时它不能接收到全局快捷键事件。&lt;br /&gt;&lt;br /&gt;这是因为被称为‘允许辅助设备的使用’的辅助使用功能当前被禁用。&lt;br /&gt;&lt;br /&gt;请&lt;a href=&quot; &quot;&gt;启用该选项&lt;/a&gt;然后继续。</translation>
    </message>
</context>
<context>
    <name>GlobalShortcutTarget</name>
    <message>
        <location filename="GlobalShortcutTarget.ui" line="14"/>
        <source>Whisper Target</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="GlobalShortcutTarget.ui" line="20"/>
        <source>Whisper to list of Users</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="GlobalShortcutTarget.ui" line="34"/>
        <source>Channel Target</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="GlobalShortcutTarget.ui" line="63"/>
        <source>Restrict to Group</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="GlobalShortcutTarget.ui" line="70"/>
        <source>If specified, only members of this group will receive the whisper.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="GlobalShortcutTarget.ui" line="81"/>
        <source>If checked the whisper will also be transmitted to linked channels.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="GlobalShortcutTarget.ui" line="84"/>
        <source>Whisper to Linked channels</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="GlobalShortcutTarget.ui" line="91"/>
        <source>If checked this whisper will also be sent to the subchannels of the channel target.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="GlobalShortcutTarget.ui" line="94"/>
        <source>Whisper to subchannels</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="GlobalShortcutTarget.ui" line="110"/>
        <source>List of users</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="GlobalShortcutTarget.ui" line="122"/>
        <source>Add</source>
        <translation type="unfinished">添加</translation>
    </message>
    <message>
        <location filename="GlobalShortcutTarget.ui" line="129"/>
        <source>Remove</source>
        <translation type="unfinished">删除</translation>
    </message>
    <message>
        <location filename="GlobalShortcutTarget.ui" line="166"/>
        <source>Whisper to Channel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="GlobalShortcutTarget.ui" line="173"/>
        <source>Modifiers</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="GlobalShortcutTarget.ui" line="179"/>
        <source>Do not send positional audio information when using this whisper shortcut.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="GlobalShortcutTarget.ui" line="182"/>
        <source>Ignore positional audio</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>GlobalShortcutX</name>
    <message>
        <location filename="GlobalShortcut_unix.cpp" line="325"/>
        <source>Mouse %1</source>
        <translation>鼠标 %1</translation>
    </message>
</context>
<context>
    <name>LCD</name>
    <message>
        <location filename="LCD.cpp" line="280"/>
        <source>Not connected</source>
        <translation>未连接</translation>
    </message>
</context>
<context>
    <name>LCDConfig</name>
    <message>
        <source>Graphic</source>
        <translation type="obsolete">图形</translation>
    </message>
    <message>
        <source>Character</source>
        <translation type="obsolete">字符</translation>
    </message>
    <message>
        <location filename="LCD.cpp" line="120"/>
        <source>Enable this device</source>
        <translation>启用该设备</translation>
    </message>
    <message>
        <location filename="LCD.cpp" line="129"/>
        <source>LCD</source>
        <translation></translation>
    </message>
    <message>
        <location filename="LCD.ui" line="14"/>
        <source>Form</source>
        <translation>表单</translation>
    </message>
    <message>
        <location filename="LCD.ui" line="34"/>
        <source>Devices</source>
        <translation>设备</translation>
    </message>
    <message>
        <location filename="LCD.ui" line="50"/>
        <source>Name</source>
        <translation>名称</translation>
    </message>
    <message>
        <source>Type</source>
        <translation type="obsolete">类型</translation>
    </message>
    <message>
        <location filename="LCD.ui" line="26"/>
        <source>&lt;p&gt;This is the list of available LCD devices on your system.  It lists devices by name, but also includes the size of the display. Mumble supports outputting to several LCD devices at a time.&lt;/p&gt;
&lt;h3&gt;Size:&lt;/h3&gt;
&lt;p&gt;
This field describes the size of an LCD device. The size is given either in pixels (for Graphic LCDs) or in characters (for Character LCDs).&lt;/p&gt;
&lt;h3&gt;Enabled:&lt;/h3&gt;
&lt;p&gt;This decides whether Mumble should draw to a particular LCD device.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LCD.ui" line="55"/>
        <source>Size</source>
        <translation>大小</translation>
    </message>
    <message>
        <location filename="LCD.ui" line="60"/>
        <source>Enabled</source>
        <translation>已启用</translation>
    </message>
    <message>
        <location filename="LCD.ui" line="77"/>
        <source>Views</source>
        <translation>视图</translation>
    </message>
    <message>
        <location filename="LCD.ui" line="83"/>
        <source>Minimum Column Width</source>
        <translation>最小列宽</translation>
    </message>
    <message>
        <location filename="LCD.ui" line="90"/>
        <source>&lt;p&gt;This option decides the minimum width a column in the User View.&lt;/p&gt;
&lt;p&gt;If too many people are speaking at once, the User View will split itself into columns. You can use this option to pick a compromise between number of users shown on the LCD, and width of user names.&lt;/p&gt;
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LCD.ui" line="128"/>
        <source>This setting decides the width of column splitter.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>TextLabel</source>
        <translation type="obsolete">文本标签</translation>
    </message>
    <message>
        <location filename="LCD.ui" line="121"/>
        <source>Splitter Width</source>
        <translation>分割线宽度</translation>
    </message>
</context>
<context>
    <name>Log</name>
    <message>
        <location filename="Log.cpp" line="216"/>
        <source>Debug</source>
        <translation>调试</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="217"/>
        <source>Critical</source>
        <translation>紧急</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="218"/>
        <source>Warning</source>
        <translation>警告</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="219"/>
        <source>Information</source>
        <translation>信息</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="220"/>
        <source>Server Connected</source>
        <translation>服务器已连接</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="221"/>
        <source>Server Disconnected</source>
        <translation>服务器已断开连接</translation>
    </message>
    <message>
        <source>Player Joined Server</source>
        <translation type="obsolete">玩家加入服务器</translation>
    </message>
    <message>
        <source>Player Left Server</source>
        <translation type="obsolete">玩家离开服务器</translation>
    </message>
    <message>
        <source>Player kicked (you or by you)</source>
        <translation type="obsolete">玩家踢出你（或被你踢出）</translation>
    </message>
    <message>
        <source>Player kicked</source>
        <translation type="obsolete">玩家被踢出</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="226"/>
        <source>You self-muted/deafened</source>
        <translation>你自我静音/耳聋</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="227"/>
        <source>Other self-muted/deafened</source>
        <translation>其他人自我静音/耳聋</translation>
    </message>
    <message>
        <source>Player muted (you)</source>
        <translation type="obsolete">玩家对你静音</translation>
    </message>
    <message>
        <source>Player muted (by you)</source>
        <translation type="obsolete">玩家被你静音</translation>
    </message>
    <message>
        <source>Player muted (other)</source>
        <translation type="obsolete">玩家静音其他人</translation>
    </message>
    <message>
        <source>Player Joined Channel</source>
        <translation type="obsolete">玩家加入频道</translation>
    </message>
    <message>
        <source>Player Left Channel</source>
        <translation type="obsolete">玩家离开频道</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="233"/>
        <source>Permission Denied</source>
        <translation>没有权限</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="234"/>
        <source>Text Message</source>
        <translation>文本消息</translation>
    </message>
    <message>
        <source>Script Errors</source>
        <translation type="obsolete">脚本错误</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="222"/>
        <source>User Joined Server</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Log.cpp" line="223"/>
        <source>User Left Server</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Log.cpp" line="224"/>
        <source>User kicked (you or by you)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Log.cpp" line="225"/>
        <source>User kicked</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Log.cpp" line="228"/>
        <source>User muted (you)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Log.cpp" line="229"/>
        <source>User muted (by you)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Log.cpp" line="230"/>
        <source>User muted (other)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Log.cpp" line="231"/>
        <source>User Joined Channel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Log.cpp" line="232"/>
        <source>User Left Channel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Log.cpp" line="352"/>
        <source>[[ Text object too large to display ]]</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Log.cpp" line="381"/>
        <source>[Date changed to %1]
</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>LogConfig</name>
    <message>
        <location filename="Log.cpp" line="75"/>
        <source>Toggle console for %1 events</source>
        <translation>为 %1 事件启用控制台</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="76"/>
        <source>Toggle pop-up notifications for %1 events</source>
        <translation>为 %1 事件启用弹出窗口通知</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="77"/>
        <source>Toggle Text-To-Speech for %1 events</source>
        <translation>为%1事件启用文本语音转换</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="78"/>
        <source>Click here to toggle sound notification for %1 events</source>
        <translation>点击这里为 %1 事件启用声音通知</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="79"/>
        <source>Path to sound file used for sound notifications in the case of %1 events&lt;br /&gt;Single click to play&lt;br /&gt;Double-click to change</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Log.cpp" line="81"/>
        <source>Click here to toggle console output for %1 events.&lt;br /&gt;If checked, this option makes Mumble output all %1 events in its message log.</source>
        <translation>点击这里为 %1 事件启用控制台输出。&lt;br /&gt;如果选中，该选项使 Mumble 输出所有 %1 事件到它的消息日志中。</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="82"/>
        <source>Click here to toggle pop-up notifications for %1 events.&lt;br /&gt;If checked, a notification pop-up will be created by Mumble for every %1 event.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Log.cpp" line="85"/>
        <source>Path to sound file used for sound notifications in the case of %1 events.&lt;br /&gt;Single click to play&lt;br /&gt;Double-click to change&lt;br /&gt;Ensure that sound notifications for these events are enabled or this field will not have any effect.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Click here to toggle Text-To-Speech for %1 events.&lt;br /&gt;If checked, mumble uses Text-To-Speech to read %1 events out loud to you. Text-To-Speech is also able to read the contents of the event which is not true for soundfiles. Text-To-Speech and soundfiles cannot be used at the same time.</source>
        <translation type="obsolete">点击这里为 %1 事件启用文本语音转换.&lt;br /&gt;如果选中， Mumble 会使用文本语音转化将 %1 事件大声的读出来给你听。当声音文件无效时，文本语音转换也能读取事件内容。文本语音转化和声音文件不能同时使用。</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="83"/>
        <source>Click here to toggle Text-To-Speech for %1 events.&lt;br /&gt;If checked, Mumble uses Text-To-Speech to read %1 events out loud to you. Text-To-Speech is also able to read the contents of the event which is not true for sound files. Text-To-Speech and sound files cannot be used at the same time.</source>
        <oldsource>Click here to toggle sound notification for %1 events.&lt;br /&gt;If checked, mumble uses a soundfile predefined by you to indicate %1 events. Soundfiles and Text-To-Speech cannot be used at the same time.</oldsource>
        <translation type="unfinished">点击这里为 %1 事件启用声音通知。&lt;br /&gt;如果选中， Mumble 会使用一个你预先定义的声音文件来标示 %1 事件。声音文件和文本语音转换不能同时被使用。</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="84"/>
        <source>Click here to toggle sound notification for %1 events.&lt;br /&gt;If checked, Mumble uses a sound file predefined by you to indicate %1 events. Sound files and Text-To-Speech cannot be used at the same time.</source>
        <oldsource>Path to soundfile used for sound notifications in the case of %1 events.&lt;br /&gt;Single click to play&lt;br /&gt;Doubleclick to change&lt;br /&gt;Be sure that sound notifications for these events are enabled or this field will not have any effect.</oldsource>
        <translation type="unfinished">在 %1 事件发生时用于声音通知的声音文件的路径。&lt;br /&gt;单击播放&lt;br /&gt;双击更换&lt;br /&gt;请确认这些事件的声音通知被启用，否则这个字段将没有任何效果。</translation>
    </message>
    <message>
        <source>Choose sound file</source>
        <translation type="obsolete">选择声音文件</translation>
    </message>
    <message>
        <source>Invalid sound file</source>
        <translation type="obsolete">无效声音文件</translation>
    </message>
    <message>
        <source>The file &apos;%1&apos; does not exist or is not a valid speex file.</source>
        <translation type="obsolete">文件 ‘%1’ 不存在或者不是一个有效的语音编码文件。</translation>
    </message>
    <message>
        <location filename="Log.ui" line="14"/>
        <source>Messages</source>
        <translation>消息</translation>
    </message>
    <message>
        <location filename="Log.ui" line="36"/>
        <source>Message</source>
        <translation>消息</translation>
    </message>
    <message>
        <location filename="Log.ui" line="41"/>
        <source>Console</source>
        <translation>控制台</translation>
    </message>
    <message>
        <location filename="Log.ui" line="46"/>
        <source>Notification</source>
        <translation>通知</translation>
    </message>
    <message>
        <location filename="Log.ui" line="51"/>
        <source>Text-To-Speech</source>
        <translation>文本语音转换</translation>
    </message>
    <message>
        <location filename="Log.ui" line="56"/>
        <source>Soundfile</source>
        <translation>声音文件</translation>
    </message>
    <message>
        <location filename="Log.ui" line="61"/>
        <source>Path</source>
        <translation>路径</translation>
    </message>
    <message>
        <location filename="Log.ui" line="69"/>
        <source>Text To Speech</source>
        <translation>文本语音转换</translation>
    </message>
    <message>
        <location filename="Log.ui" line="75"/>
        <source>Volume</source>
        <translation>音量</translation>
    </message>
    <message>
        <location filename="Log.ui" line="85"/>
        <source>Volume of Text-To-Speech Engine</source>
        <translation>文本语音转换引擎的音量</translation>
    </message>
    <message>
        <location filename="Log.ui" line="88"/>
        <source>&lt;b&gt;This is the volume used for the speech synthesis.&lt;/b&gt;</source>
        <translation>&lt;b&gt;这是用于语音合成的音量。&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="Log.ui" line="113"/>
        <source>Length threshold</source>
        <translation>长度阈值</translation>
    </message>
    <message>
        <location filename="Log.ui" line="123"/>
        <source>Message length threshold for Text-To-Speech Engine</source>
        <translation>用于文本语音转换的消息长度阈值</translation>
    </message>
    <message>
        <location filename="Log.ui" line="126"/>
        <source>&lt;b&gt;This is the length threshold used for the Text-To-Speech Engine.&lt;/b&gt;&lt;br /&gt;Messages longer than this limit will not be read aloud in their full length.</source>
        <translation>&lt;b&gt;这是用于文本语音转换引擎的消息长度阈值。&lt;/b&gt;&lt;br /&gt;消息长于此值时将不会被全部读出来。</translation>
    </message>
    <message>
        <location filename="Log.ui" line="132"/>
        <source> Characters</source>
        <translation>字符</translation>
    </message>
    <message>
        <location filename="Log.ui" line="161"/>
        <source>Whisper</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Log.ui" line="167"/>
        <source>If checked you will only hear whispers from users you added to your friend list.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Log.ui" line="170"/>
        <source>Only accept whispers from friends</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>LookConfig</name>
    <message>
        <location filename="LookConfig.cpp" line="47"/>
        <location filename="LookConfig.cpp" line="58"/>
        <source>System default</source>
        <translation>系统默认</translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="62"/>
        <source>None</source>
        <translation>无</translation>
    </message>
    <message>
        <source>Only with players</source>
        <translation type="obsolete">仅当有玩家时</translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="63"/>
        <source>Only with users</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="64"/>
        <source>All</source>
        <translation>所有</translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="66"/>
        <source>Ask</source>
        <translation>询问</translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="67"/>
        <source>Do Nothing</source>
        <translation>无动作</translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="68"/>
        <source>Move</source>
        <translation>移动</translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="72"/>
        <source>User Interface</source>
        <translation>用户界面</translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="192"/>
        <source>Choose skin file</source>
        <translation>选择皮肤文件</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="14"/>
        <source>Form</source>
        <translation>表单</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="231"/>
        <source>This sets which channels to automatically expand. &lt;i&gt;None&lt;/i&gt; and &lt;i&gt;All&lt;/i&gt; will expand no or all channels, while &lt;i&gt;Only with users&lt;/i&gt; will expand and collapse channels as users join and leave them.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="238"/>
        <source>List users above subchannels (requires restart).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="241"/>
        <source>&lt;b&gt;If set, users will be shown above subchannels in the channel view.&lt;/b&gt;&lt;br /&gt;A restart of Mumble is required to see the change.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="244"/>
        <source>Users above Channels</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="254"/>
        <source>&lt;b&gt;If set, will verify you want to quit if connected.&lt;/b&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="290"/>
        <source>Check to show chat bar</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="293"/>
        <source>&lt;b&gt;If checked the chat bar is shown.&lt;/b&gt;&lt;br /&gt;Uncheck this to hide it.</source>
        <oldsource>&lt;b&gt;If checked the chat bar is shown&lt;/b&gt;&lt;br /&gt;Uncheck to hide it</oldsource>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="296"/>
        <source>Show chatbar</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="303"/>
        <source>Show number of users in each channel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="306"/>
        <source>Show channel user count</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="316"/>
        <location filename="LookConfig.ui" line="322"/>
        <source>Language</source>
        <translation>语言</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="332"/>
        <source>Language to use (requires restart)</source>
        <translation>使用的语言（需要重启）</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="335"/>
        <source>&lt;b&gt;This sets which language Mumble should use.&lt;/b&gt;&lt;br /&gt;You have to restart Mumble to use the new language.</source>
        <translation>&lt;b&gt;设置 Mumble 使用哪种语言。&lt;/b&gt;&lt;br /&gt;你需要重启 Mumble 来使用新语言。</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="157"/>
        <source>Look and Feel</source>
        <translation>外观</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="20"/>
        <source>Layout</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="32"/>
        <source>Classic</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="51"/>
        <source>Stacked</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="105"/>
        <source>Hybrid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="124"/>
        <source>Custom</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="170"/>
        <source>This changes the behavior when moving channels.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="173"/>
        <source>This sets the behavior of channel drags; it can be used to prevent accidental dragging. &lt;i&gt;Move&lt;/i&gt; moves the channel without prompting. &lt;i&gt;Do Nothing&lt;/i&gt; does nothing and prints an error message. &lt;i&gt;Ask&lt;/i&gt; uses a message box to confirm if you really wanted to move the channel.</source>
        <oldsource>This sets the behavior of channel drags; it can be used to prevent accidental dragging. &lt;i&gt;Move Channel&lt;/i&gt; moves the channel without prompting. &lt;i&gt;Do Nothing&lt;/i&gt; does nothing and prints an error message. &lt;i&gt;Ask&lt;/i&gt; uses a message box to confirm if you really wanted to move the channel.</oldsource>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="180"/>
        <source>Style</source>
        <translation>风格</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="187"/>
        <source>Basic widget style</source>
        <translation>基本窗口风格</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="190"/>
        <source>&lt;b&gt;This sets the basic look and feel to use.&lt;/b&gt;</source>
        <translation>&lt;b&gt;设置使用基本外观。&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="197"/>
        <source>Skin</source>
        <translation>皮肤</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="204"/>
        <source>Skin file to use</source>
        <translation>使用的皮肤文件</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="207"/>
        <source>&lt;b&gt;This sets which skin Mumble should use.&lt;/b&gt;&lt;br /&gt;The skin is a style file applied on top of the basic widget style. If there are icons in the same directory as the style sheet, those will replace the default icons.</source>
        <translation>&lt;b&gt;设置 Mumble 应该使用哪个皮肤文件。&lt;/b&gt;&lt;br/&gt;皮肤是一个应用在基本窗口风格上的风格文件。如果在与风格相同的目录里有图标，目录中的图标将替换默认图标。</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="214"/>
        <source>...</source>
        <translation></translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="221"/>
        <source>Expand</source>
        <translation>展开</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="228"/>
        <source>When to automatically expand channels</source>
        <translation>什么时候自动展开频道</translation>
    </message>
    <message>
        <source>This sets which channels to automatically expand. &lt;i&gt;None&lt;/i&gt; and &lt;i&gt;All&lt;/i&gt; will expand no or all channels, while &lt;i&gt;Only with players&lt;/i&gt; will expand and collapse channels as players join and leave them.</source>
        <translation type="obsolete">设置哪些频道将自动展开。&lt;i&gt;无&lt;/i&gt;和&lt;i&gt;所有&lt;/i&gt;将展开零个或所有频道，而&lt;i&gt;仅当有玩家时&lt;i&gt;将在玩家加入和离开时自动展开和折叠频道。</translation>
    </message>
    <message>
        <source>List players above subchannels (requires restart).</source>
        <translation type="obsolete">在子频道之上列出玩家（需要重启）。</translation>
    </message>
    <message>
        <source>&lt;b&gt;If set, players will be shown above subchannels in the channel view.&lt;/b&gt;&lt;br /&gt;A restart of Mumble is required to see the change.</source>
        <translation type="obsolete">&lt;b&gt;如果选中，玩家将被显示在子频道之上.&lt;/b&gt;&lt;br/&gt;需要重启 Mumble 来使改变生效。</translation>
    </message>
    <message>
        <source>Players above Channels</source>
        <translation type="obsolete">玩家位于频道之上</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="163"/>
        <source>Channel Dragging</source>
        <translation>频道拖动</translation>
    </message>
    <message>
        <source>This changes the behaviour when moving channels.</source>
        <translation type="obsolete">改变当移动频道时的行为。</translation>
    </message>
    <message>
        <source>This sets the behaviour of channel drags, it can be used to prevent accidental dragging. &lt;i&gt;Move Channel&lt;/i&gt; moves the channel without prompting. &lt;i&gt;Do Nothing&lt;/i&gt; does nothing and prints an error message. &lt;i&gt;Ask&lt;/i&gt; uses a message box to confirm if you really wanted to move the channel.</source>
        <translation type="obsolete">设置频道拖动时的行为，能被用来防止意外的拖动。&lt;i&gt;移动频道&lt;/i&gt;将在移动频道时不做提示.&lt;i&gt;无动作&lt;/i&gt;将在试图移动频道时显示一个出错信息。&lt;i&gt;询问&lt;/i&gt;使用一个消息框来确认你是真的想移动一个频道。</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="251"/>
        <source>Ask whether to close or minimize when quitting Mumble.</source>
        <translation>询问退出 Mumble 时是关闭还是最小化。</translation>
    </message>
    <message>
        <source>&lt;b&gt;If set, will verify you want to quit if connected.&lt;b&gt;</source>
        <translation type="obsolete">&lt;b&gt;如果选中，当你连接到服务器时将验证你是否想退出.&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="257"/>
        <source>Ask on quit while connected</source>
        <translation>当你连接到服务器时询问是否退出</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="277"/>
        <source>Make the Mumble window appear on top of other windows.</source>
        <translation>使 Mumble 窗口位于其他窗口之上.</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="280"/>
        <source>&lt;b&gt;This makes the Mumble window a topmost window.&lt;/b&gt;</source>
        <translation>&lt;b&gt;这将使 Mumble 窗口成为最上面的窗口。&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="283"/>
        <source>Always On Top</source>
        <translation>总在最上</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="267"/>
        <source>&lt;b&gt;If set, minimizing the Mumble main window will cause it to be hidden and accessible only from the tray. Otherwise, it will be minimized as a window normally would.&lt;/b&gt;</source>
        <translation>&lt;b&gt;如果选中，最小化 Mumble 主窗口时将使得它被隐藏到系统托盘。否则，它将会像通常一样被最小化。&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="270"/>
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
        <location filename="LookConfig.ui" line="264"/>
        <source>Hide the main Mumble window in the tray when it is minimized.</source>
        <translation>当最小化时，隐藏 Mumble 主窗口到系统托盘。</translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <location filename="MainWindow.cpp" line="137"/>
        <location filename="MainWindow.cpp" line="1732"/>
        <source>Root</source>
        <translation>根</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="175"/>
        <source>Push-to-Talk</source>
        <comment>Global Shortcut</comment>
        <translation>按键通话</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="177"/>
        <source>Push and hold this button to send voice.</source>
        <comment>Global Shortcut</comment>
        <translation>按住该按钮来发送语音。</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="178"/>
        <source>This configures the push-to-talk button, and as long as you hold this button down, you will transmit voice.</source>
        <comment>Global Shortcut</comment>
        <translation>配置按键发音按键，只要你按住该按键，语音将被传送。</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="181"/>
        <source>Reset Audio Processor</source>
        <comment>Global Shortcut</comment>
        <translation>重置音频处理器</translation>
    </message>
    <message>
        <source>Toggle Mute Self</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">切换自我静音</translation>
    </message>
    <message>
        <source>Toggle self-mute status.</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">切换自我静音状态。</translation>
    </message>
    <message>
        <source>This will toggle your muted status. If you toggle this off, you will also disable self-deafen.</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">该选项将切换自我静音状态。如果你关闭静音状态，将同时关闭自我耳聋状态。</translation>
    </message>
    <message>
        <source>Toggle Deafen Self</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">切换自我耳聋</translation>
    </message>
    <message>
        <source>Toggle self-deafen status.</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">切换自我耳聋状态。</translation>
    </message>
    <message>
        <source>This will toggle your deafen status. If you toggle this on, you will also enable self-mute.</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">该选项切换自我耳聋状态。如果打开此选项，将同时启用自我禁音。</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="194"/>
        <source>Unlink Plugin</source>
        <comment>Global Shortcut</comment>
        <translation>卸载插件</translation>
    </message>
    <message>
        <source>Force Center Position</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">强制居中</translation>
    </message>
    <message>
        <source>Chan Parent</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">父频道</translation>
    </message>
    <message>
        <source>Chan Sub#%1</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">子频道#%1</translation>
    </message>
    <message>
        <source>Chan All Subs</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">所有子频道</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="197"/>
        <source>Push-to-Mute</source>
        <comment>Global Shortcut</comment>
        <translation>按键禁音</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="200"/>
        <source>Join Channel</source>
        <comment>Global Shortcut</comment>
        <translation>加入频道</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="203"/>
        <source>Toggle Overlay</source>
        <comment>Global Shortcut</comment>
        <translation>切换 Overlay</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="205"/>
        <source>Toggle state of in-game overlay.</source>
        <comment>Global Shortcut</comment>
        <translation>切换游戏中的 overlay 状态。</translation>
    </message>
    <message>
        <source>This will switch the states of the ingame overlay between showing everybody, just the players who are talking, and nobody.</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">在显示所有人，只有发言的玩家和不显示任何人之间切换游戏中的 overlay 状态。</translation>
    </message>
    <message>
        <source>Alt Push-to-Talk</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">交替按键通话</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="209"/>
        <source>Toggle Minimal</source>
        <comment>Global Shortcut</comment>
        <translation>切换最小化</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="212"/>
        <source>Volume Up (+10%)</source>
        <comment>Global Shortcut</comment>
        <translation>增大音量（+10%）</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="215"/>
        <source>Volume Down (-10%)</source>
        <comment>Global Shortcut</comment>
        <translation>减小音量（-10%）</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="326"/>
        <location filename="MainWindow.cpp" line="1221"/>
        <location filename="MainWindow.cpp" line="1817"/>
        <location filename="MainWindow.ui" line="14"/>
        <location filename="main.cpp" line="301"/>
        <source>Mumble</source>
        <translation></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="219"/>
        <location filename="MainWindow.cpp" line="231"/>
        <source>Mumble -- %1</source>
        <translation></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="235"/>
        <source>&amp;Window</source>
        <translation>&amp;窗口</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="237"/>
        <location filename="MainWindow.cpp" line="328"/>
        <source>Minimize</source>
        <translation>最小化</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="237"/>
        <source>Ctrl+M</source>
        <translation></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="327"/>
        <source>Close</source>
        <translation>关闭</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="326"/>
        <source>Mumble is currently connected to a server. Do you want to Close or Minimize it?</source>
        <translation>Mumble 现在连接到了一个服务器。你想要关闭还是最小化它？</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="184"/>
        <source>Mute Self</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="186"/>
        <source>Set self-mute status.</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="187"/>
        <source>This will set or toggle your muted status. If you turn this off, you will also disable self-deafen.</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="189"/>
        <source>Deafen Self</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="191"/>
        <source>Set self-deafen status.</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="192"/>
        <source>This will set or toggle your deafened status. If you turn this on, you will also enable self-mute.</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="206"/>
        <source>This will switch the states of the in-game overlay between showing everybody, just the users who are talking, and nobody.</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="222"/>
        <source>Whisper</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="279"/>
        <location filename="MainWindow.cpp" line="1980"/>
        <source>Not connected</source>
        <translation type="unfinished">未连接</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="388"/>
        <source>Clear</source>
        <translation>清除</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="394"/>
        <source>Opening URL %1</source>
        <translation>打开网址 %1</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="398"/>
        <source>File does not exist</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="408"/>
        <source>File is not a configuration file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="414"/>
        <source>Settings merged from file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="420"/>
        <source>URL scheme is not &apos;mumble&apos;</source>
        <translation>&apos;Mumble&apos; 不支持的网址方案</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="445"/>
        <source>This version of Mumble can&apos;t handle URLs for Mumble version %1.%2.%3</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="471"/>
        <source>Connecting to %1</source>
        <translation>正在连接到 %1</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="471"/>
        <source>Enter username</source>
        <translation>输入用户名</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="484"/>
        <location filename="MainWindow.cpp" line="636"/>
        <source>Connecting to server %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="646"/>
        <source>Reconnecting.</source>
        <translation>正在重新连接中。</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="721"/>
        <source>&lt;h2&gt;Version&lt;/h2&gt;&lt;p&gt;Protocol %1.%2.%3.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="726"/>
        <source>&lt;p&gt;No build information or OS version available.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="728"/>
        <source>&lt;p&gt;%1 (%2)&lt;br /&gt;%3&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="732"/>
        <source>&lt;h2&gt;Control channel&lt;/h2&gt;&lt;p&gt;Encrypted with %1 bit %2&lt;br /&gt;%3 ms average latency (%4 deviation)&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="741"/>
        <source>&lt;h2&gt;Voice channel&lt;/h2&gt;&lt;p&gt;Encrypted with 128 bit OCB-AES128&lt;br /&gt;%1 ms average latency (%4 deviation)&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="752"/>
        <source>&lt;h2&gt;Audio bandwidth&lt;/h2&gt;&lt;p&gt;Maximum %1 kbit/s&lt;br /&gt;Current %2 kbit/s&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="934"/>
        <source>Register yourself as %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="934"/>
        <source>&lt;p&gt;You are about to register yourself on this server. This action cannot be undone, and your username cannot be changed once this is done. You will forever be known as &apos;%1&apos; on this server.&lt;/p&gt;&lt;p&gt;Are you sure you want to register yourself?&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="936"/>
        <source>Register user %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="936"/>
        <source>&lt;p&gt;You are about to register %1 on the server. This action cannot be undone, the username cannot be changed, and as a registered user, %1 will have access to the server even if you change the server password.&lt;/p&gt;&lt;p&gt;From this point on, %1 will be authenticated with the certificate currently in use.&lt;/p&gt;&lt;p&gt;Are you sure you want to register %1?&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="979"/>
        <source>Kicking user %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1001"/>
        <source>Banning user %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1050"/>
        <source>Change comment on user %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1093"/>
        <location filename="MainWindow.cpp" line="1310"/>
        <source>To channel %1: %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1093"/>
        <location filename="MainWindow.cpp" line="1310"/>
        <source>Message to channel %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1726"/>
        <source>Connected.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1843"/>
        <source>SSL Version mismatch</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1843"/>
        <source>This server is using an older encryption standard. It might be an older 1.1 based Mumble server.&lt;br /&gt;Would you like to launch the compatibility client to connect to it?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1869"/>
        <source>The compatibility client could not be found, or failed to start.&lt;br /&gt;Note that the compatibility client is an optional component for most installations, and might not be installed.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1986"/>
        <source>Type message to channel &apos;%1&apos; here</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1989"/>
        <source>Type message to user &apos;%1&apos; here</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="2068"/>
        <source>Choose image file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="2068"/>
        <source>Images (*.png *.jpg)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="2075"/>
        <location filename="MainWindow.cpp" line="2087"/>
        <source>Failed to load image</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="2075"/>
        <source>Could not open file for reading.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="2087"/>
        <source>Image format not recognized.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;h2&gt;Control channel&lt;/h2&gt;&lt;p&gt;Encrypted with %1 bit %2&lt;br /&gt;%3 ms average latency (%4 variance)&lt;/p&gt;</source>
        <translation type="obsolete">&lt;h2&gt;控制信道&lt;/h2&gt;&lt;p&gt;使用 %1 比特 %2 加密&lt;br /&gt;平均延迟 %3 毫秒（%4 方差）&lt;/p&gt;</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="739"/>
        <source>Voice channel is sent over control channel.</source>
        <translation>语音频道通过控制信道发送。</translation>
    </message>
    <message>
        <source>&lt;h2&gt;Voice channel&lt;/h2&gt;&lt;p&gt;Encrypted with 128 bit OCB-AES128&lt;br /&gt;%1 ms average latency (%4 variance)&lt;/p&gt;</source>
        <translation type="obsolete">&lt;h2&gt;语音频道&lt;/h2&gt;&lt;p&gt;使用 128 比特 OCB-AES128 加密&lt;br/&gt;%1 毫秒延迟（%4 方差）&lt;/p&gt;</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="748"/>
        <source>UDP Statistics</source>
        <translation>UDP 统计</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="748"/>
        <source>To Server</source>
        <translation>到服务器</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="748"/>
        <source>From Server</source>
        <translation>来至服务器</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="748"/>
        <source>Good</source>
        <translation>良好</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="748"/>
        <source>Late</source>
        <translation>迟到</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="748"/>
        <source>Lost</source>
        <translation>丢失</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="748"/>
        <source>Resync</source>
        <translation>重新同步</translation>
    </message>
    <message>
        <source>&lt;h2&gt;Audio bandwidth&lt;/h2&gt;&lt;p&gt;Maximum %1 kbit/s&lt;br /&gt;Current %2 kbit/s (Quality %3)&lt;/p&gt;</source>
        <translation type="obsolete">&lt;h2&gt;音频带宽&lt;/h2&gt;&lt;p&gt;最大 %1千比特/秒&lt;br /&gt;当前 %2 千比特/秒 （质量 %3）&lt;/p&gt;</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="754"/>
        <source>Mumble Server Information</source>
        <translation>Mumble 服务器信息</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="758"/>
        <location filename="MainWindow.cpp" line="1825"/>
        <source>&amp;View Certificate</source>
        <translation>&amp;查看认证</translation>
    </message>
    <message>
        <source>Kicking player %1</source>
        <translation type="obsolete">踢出玩家 %1</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="979"/>
        <location filename="MainWindow.cpp" line="1001"/>
        <source>Enter reason</source>
        <translation>输入原因</translation>
    </message>
    <message>
        <source>Banning player %1</source>
        <translation type="obsolete">封禁玩家 %1</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1023"/>
        <source>Sending message to %1</source>
        <translation>发送消息给 %1</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1036"/>
        <location filename="MainWindow.cpp" line="1097"/>
        <source>To %1: %2</source>
        <translation>发送给 %1: %2</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1036"/>
        <location filename="MainWindow.cpp" line="1097"/>
        <source>Message to %1</source>
        <translation>发送消息给 %1</translation>
    </message>
    <message>
        <source>Channel Name</source>
        <translation type="obsolete">频道名</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1221"/>
        <source>Are you sure you want to delete %1 and all its sub-channels?</source>
        <translation>你确信要删除 %1和它的子频道吗？</translation>
    </message>
    <message>
        <source>Change description of channel %1</source>
        <translation type="obsolete">改变频道 %1的描叙</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1293"/>
        <source>Sending message to channel %1</source>
        <translation>发送消息给频道 %1</translation>
    </message>
    <message>
        <source>Sending message to channel tree %1</source>
        <translation type="obsolete">发送消息给频道树 %1</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1308"/>
        <source>To tree %1: %2</source>
        <translation>发送给树 %1: %2</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1308"/>
        <source>Message to tree %1</source>
        <translation>发送消息给树 %1</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1394"/>
        <source>Unmuted and undeafened.</source>
        <translation>解除静音并解除耳聋。</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1396"/>
        <source>Unmuted.</source>
        <translation>解除静音。</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1398"/>
        <source>Muted.</source>
        <translation>静音。</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1425"/>
        <source>Muted and deafened.</source>
        <translation>禁音和耳聋.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1427"/>
        <source>Deafened.</source>
        <translation>耳聋.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1429"/>
        <source>Undeafened.</source>
        <translation>解除耳聋。</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1497"/>
        <source>About Qt</source>
        <translation>关于 Qt</translation>
    </message>
    <message>
        <source>Joining %1.</source>
        <translation type="obsolete">加入 %1.</translation>
    </message>
    <message>
        <source>Connected to server %1.</source>
        <translation type="obsolete">已连接到服务器 %1.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1804"/>
        <source>SSL Verification failed: %1</source>
        <translation>SSL 认证失败: %1</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1809"/>
        <source>&lt;b&gt;WARNING:&lt;/b&gt; The server presented a certificate that was different from the stored one.</source>
        <translation>&lt;b&gt;警告:&lt;/b&gt;服务器提供的认证与保存的认证不同。</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1811"/>
        <source>Sever presented a certificate which failed verification.</source>
        <translation>服务器提供的认证认证失败。</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1818"/>
        <source>&lt;p&gt;%1.&lt;br /&gt;The specific errors with this certificate are: &lt;/p&gt;&lt;ol&gt;%2&lt;/ol&gt;&lt;p&gt;Do you wish to accept this certificate anyway?&lt;br /&gt;(It will also be stored so you won&apos;t be asked this again.)&lt;/p&gt;</source>
        <translation>&lt;p&gt;%1.&lt;br /&gt; 认证中的错误是：&lt;/p&gt;&lt;ol&gt;%2&lt;/ol&gt;&lt;p&gt;你希望接受认证吗？&lt;br /&gt;（认证将被保存从而你不会被再次索要认证。）&lt;/p&gt;</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1869"/>
        <source>Failed to launch compatibility client</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1876"/>
        <source>Server connection failed: %1.</source>
        <translation>连接服务器失败: %1.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1878"/>
        <source>Disconnected from server.</source>
        <translation>与服务器断开连接.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1890"/>
        <source>Invalid username</source>
        <translation>无效用户名</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1890"/>
        <source>You connected with an invalid username, please try another one.</source>
        <translation>你使用一个无效的用户名连接，请尝试别外一个。</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1890"/>
        <source>That username is already in use, please try another username.</source>
        <translation>用户名已经被使用，请使用另外一个用户名。</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1895"/>
        <source>Wrong password</source>
        <translation>密码错误</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1895"/>
        <source>Wrong password for registered users, please try again.</source>
        <translation>注册用户密码错误，请重试。</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1895"/>
        <source>Wrong server password for unregistered user account, please try again.</source>
        <translation>未注册用户的服务器密码错误，请重试。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="53"/>
        <source>&amp;Server</source>
        <translation>&amp;服务器</translation>
    </message>
    <message>
        <source>&amp;Player</source>
        <translation type="obsolete">&amp;玩家</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="63"/>
        <source>&amp;Channel</source>
        <translation>&amp;频道</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="68"/>
        <source>&amp;Audio</source>
        <translation>&amp;音频</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="82"/>
        <source>C&amp;onfigure</source>
        <translation>&amp;配置</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="88"/>
        <source>&amp;Help</source>
        <translation>&amp;帮助</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="110"/>
        <source>Log</source>
        <translation>日志</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="120"/>
        <source>This shows all recent activity. Connecting to servers, errors and information messages all show up here.&lt;br /&gt;To configure exactly which messages show up here, use the &lt;b&gt;Settings&lt;/b&gt; command from the menu.</source>
        <translation>这里显示所有最近的活动。正在连接到服务器，错误和通知消息都将在这里显示。&lt;br /&gt;要准确配置什么消息将被显示在这里，请从菜单中使用&lt;b&gt;设置&lt;/b&gt;命令。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="166"/>
        <source>&amp;Quit Mumble</source>
        <translation>&amp;退出 Mumble</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="169"/>
        <source>Closes the program</source>
        <translation>关闭程序</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="172"/>
        <source>Exits the application.</source>
        <translation>退出应用程序。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="175"/>
        <source>Ctrl+Q</source>
        <translation></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="180"/>
        <source>&amp;Connect</source>
        <translation>&amp;连接</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="183"/>
        <source>Open the server connection dialog</source>
        <translation>打开服务器连接对话框</translation>
    </message>
    <message>
        <source>Shows a dialog of registered servers, and also allows quick connect.</source>
        <translation type="obsolete">显示一个已注册服务器的对话框，并允许快速连接。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="194"/>
        <source>&amp;Disconnect</source>
        <translation>&amp;断开</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="197"/>
        <source>Disconnect from server</source>
        <translation>从服务器断开连接</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="200"/>
        <source>Disconnects you from the server.</source>
        <translation>使你从服务器断开连接。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="208"/>
        <source>&amp;Ban lists</source>
        <translation>&amp;黑名单</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="211"/>
        <source>Edit ban lists on server</source>
        <translation>编辑服务器上的黑名单</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="214"/>
        <source>This lets you edit the server-side IP ban lists.</source>
        <translation>这让你编辑服务器端的网址黑名单。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="222"/>
        <source>&amp;Information</source>
        <translation>&amp;信息</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="225"/>
        <source>Show information about the server connection</source>
        <translation>显示服务器连接信息</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="228"/>
        <source>This will show extended information about the connection to the server.</source>
        <translation>显示服务器连接的扩展信息.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="233"/>
        <source>&amp;Kick</source>
        <translation>&amp;踢出</translation>
    </message>
    <message>
        <source>Kick player (with reason)</source>
        <translation type="obsolete">踢出玩家（理由）</translation>
    </message>
    <message>
        <source>Kick selected player off server. You&apos;ll be asked to specify a reason.</source>
        <translation type="obsolete">从服务器踢出所选玩家。你将被要求指定一个原因。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="247"/>
        <source>&amp;Mute</source>
        <translation>&amp;静音</translation>
    </message>
    <message>
        <source>Mute player</source>
        <translation type="obsolete">静音玩家</translation>
    </message>
    <message>
        <source>Mute or unmute player on server. Unmuting a deafened player will also undeafen them.</source>
        <translation type="obsolete">在服务器上对玩家静音或取消静音。取消静音一个耳聋的玩家将同时对他取消耳聋。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="258"/>
        <source>&amp;Ban</source>
        <translation>&amp;封禁</translation>
    </message>
    <message>
        <source>Kick and ban player (with reason)</source>
        <translation type="obsolete">踢出并封禁玩家（理由）</translation>
    </message>
    <message>
        <source>Kick and ban selected player from server. You&apos;ll be asked to specify a reason.</source>
        <translation type="obsolete">从服务器踢出并封禁所选玩家。你将被要求指定一个原因。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="272"/>
        <source>&amp;Deafen</source>
        <translation>&amp;耳聋</translation>
    </message>
    <message>
        <source>Deafen player</source>
        <translation type="obsolete">耳聋玩家</translation>
    </message>
    <message>
        <source>Deafen or undeafen player on server. Deafening a player will also mute them.</source>
        <translation type="obsolete">在服务器上使玩家耳聋或取消耳聋。耳聋一个玩家将同时对他静音。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="286"/>
        <source>&amp;Local Mute</source>
        <translation>&amp;本地静音</translation>
    </message>
    <message>
        <source>Mute player locally</source>
        <translation type="obsolete">本地对玩家静音</translation>
    </message>
    <message>
        <source>Mute or unmute player locally. Use this on other players in the same room.</source>
        <translation type="obsolete">本地对玩家静音或者取消静音. 对相同房间的其他玩家使用该选项。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="297"/>
        <location filename="MainWindow.ui" line="538"/>
        <source>Send Messa&amp;ge</source>
        <translation>发送&amp;消息</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="300"/>
        <location filename="MainWindow.ui" line="541"/>
        <source>Send a Text Message</source>
        <translation>发送一条文本消息</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="303"/>
        <source>Sends a text message to another user.</source>
        <translation>向另一个用户发送一条文本消息。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="308"/>
        <source>&amp;Add</source>
        <translation>&amp;添加</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="311"/>
        <source>Add new channel</source>
        <translation>添加一个新频道</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="314"/>
        <source>This adds a new sub-channel to the currently selected channel.</source>
        <translation>在当前所选频道内添加一个子频道。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="319"/>
        <source>&amp;Remove</source>
        <translation>&amp;删除</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="322"/>
        <source>Remove channel</source>
        <translation>删除频道</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="325"/>
        <source>This removes a channel and all sub-channels.</source>
        <translation>删除一个频道及其所有子频道。</translation>
    </message>
    <message>
        <source>&amp;Edit ACL</source>
        <translation type="obsolete">&amp;编辑 访问控制列表</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="333"/>
        <source>Edit Groups and ACL for channel</source>
        <translation>为频道编辑群和访问控制列表</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="336"/>
        <source>This opens the Group and ACL dialog for the channel, to control permissions.</source>
        <translation>打开频道的群和访问控制列表对话框，来控制频道的访问权限。</translation>
    </message>
    <message>
        <source>&amp;Rename</source>
        <translation type="obsolete">&amp;重命名</translation>
    </message>
    <message>
        <source>Renames the channel</source>
        <translation type="obsolete">重命名频道</translation>
    </message>
    <message>
        <source>This renames a channel.</source>
        <translation type="obsolete">重命名一个频道。</translation>
    </message>
    <message>
        <source>&amp;Change Description</source>
        <translation type="obsolete">&amp;修改描叙</translation>
    </message>
    <message>
        <source>Changes the channel description</source>
        <translation type="obsolete">修改频道描叙</translation>
    </message>
    <message>
        <source>This changes the description of a channel.</source>
        <translation type="obsolete">修改一个频道的描叙。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="341"/>
        <source>&amp;Link</source>
        <translation>&amp;链接</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="344"/>
        <source>Link your channel to another channel</source>
        <translation>链接你的频道和另一个频道</translation>
    </message>
    <message>
        <source>This links your current channel to the selected channel. If they have permission to speak in the other channel, players can now hear each other. This is a permanent link, and will last until manually unlinked or the server is restarted. Please see the shortcuts for push-to-link.</source>
        <translation type="obsolete">链接你的当前频道和所选频道。如果他们有权限在对方频道讲话，玩家现在能听到彼此的声音。这是个永久性的链接，直到手动取消链接或者服务器重启。请查看按键链接快捷键。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="58"/>
        <source>&amp;User</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="138"/>
        <source>This is the chatbar&lt;br /&gt;If you enter text here and then press enter the text is sent to the user or channel that was selected. If nothing is selected the message is sent to your current channel.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="144"/>
        <source>Chatbar</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="186"/>
        <source>Shows a dialog of registered servers, and also allows quick-connect.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="236"/>
        <source>Kick user (with reason)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="239"/>
        <source>Kick selected user off server. You&apos;ll be asked to specify a reason.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="250"/>
        <source>Mute user</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="253"/>
        <source>Mute or unmute user on server. Unmuting a deafened user will also undeafen them.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="261"/>
        <source>Kick and ban user (with reason)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="264"/>
        <source>Kick and ban selected user from server. You&apos;ll be asked to specify a reason.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="275"/>
        <source>Deafen user</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="278"/>
        <source>Deafen or undeafen user on server. Deafening a user will also mute them.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="289"/>
        <source>Mute user locally</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="292"/>
        <source>Mute or unmute user locally. Use this on other users in the same room.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="330"/>
        <source>&amp;Edit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="347"/>
        <source>This links your current channel to the selected channel. If users in a channel have permission to speak in the other channel, users can now hear each other. This is a permanent link, and will last until manually unlinked or the server is restarted. Please see the shortcuts for push-to-link.</source>
        <oldsource>This links your current channel to the selected channel. If they have permission to speak in the other channel, users can now hear each other. This is a permanent link, and will last until manually unlinked or the server is restarted. Please see the shortcuts for push-to-link.</oldsource>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="352"/>
        <source>&amp;Unlink</source>
        <comment>Channel</comment>
        <translation>&amp;取消链接</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="355"/>
        <source>Unlink your channel from another channel</source>
        <translation>取消你的频道和另一个频道之间的链接</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="358"/>
        <source>This unlinks your current channel from the selected channel.</source>
        <translation>取消你的当前频道和所选频道之间的链接。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="363"/>
        <source>Unlink &amp;All</source>
        <translation>取消&amp;所有链接</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="366"/>
        <source>Unlinks your channel from all linked channels.</source>
        <translation>取消你的频道和所有已链接频道之间的链接。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="369"/>
        <source>This unlinks your current channel (not the selected one) from all linked channels.</source>
        <translation>取消你的当前频道（非所选频道）和所有已链接的频道之间的链接。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="374"/>
        <source>&amp;Reset</source>
        <translation>&amp;重置</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="377"/>
        <source>Reset audio preprocessor</source>
        <translation>重置音频预处理器</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="380"/>
        <source>This will reset the audio preprocessor, including noise cancellation, automatic gain and voice activity detection. If something suddenly worsens the audio environment (like dropping the microphone) and it was temporary, use this to avoid having to wait for the preprocessor to readjust.</source>
        <translation>该选项将重置音频预处理器，包含噪音消除，自动获取和语音活动检测。如果音频环境突然变的恶劣（比如掉麦）并且是暂时的，使用该选项来避免等待预处理器自我调节。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="388"/>
        <source>&amp;Mute Self</source>
        <translation>&amp;静音自己</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="391"/>
        <source>Mute yourself</source>
        <translation>自我静音</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="394"/>
        <source>Mute or unmute yourself. When muted, you will not send any data to the server. Unmuting while deafened will also undeafen.</source>
        <translation>对自己静音或取消静音。当静音时，你将不会发送数据到服务器。当耳聋时取消静音将同时取消耳聋。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="402"/>
        <source>&amp;Deafen Self</source>
        <translation>&amp;耳聋自己</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="405"/>
        <source>Deafen yourself</source>
        <translation>自我耳聋</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="408"/>
        <source>Deafen or undeafen yourself. When deafened, you will not hear anything. Deafening yourself will also mute.</source>
        <translation>使自己耳聋或取消耳聋。当耳聋时，你将听不到声音。耳聋自己将同时静音自己。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="416"/>
        <source>&amp;Text-To-Speech</source>
        <translation>&amp;文本语音转换</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="419"/>
        <source>Toggle Text-To-Speech</source>
        <translation>切换文本语音转换</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="422"/>
        <source>Enable or disable the text-to-speech engine. Only messages enabled for TTS in the Configuration dialog will actually be spoken.</source>
        <translation>启用或禁用文本语音转换引擎。只有在配置对话框里被启用了文本语音转换的消息才会被读出来。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="427"/>
        <source>S&amp;tatistics</source>
        <translation>&amp;统计</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="430"/>
        <source>Display audio statistics</source>
        <translation>显示音频统计</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="433"/>
        <source>Pops up a small dialog with information about your current audio input.</source>
        <translation>弹出一个关于你当前音频输入信息的对话框。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="438"/>
        <source>&amp;Unlink</source>
        <comment>Plugin</comment>
        <translation>&amp;卸载</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="441"/>
        <source>Forcibly unlink plugin</source>
        <translation>强行卸载插件</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="444"/>
        <source>This forces the current plugin to unlink, which is handy if it is reading completely wrong data.</source>
        <translation>强行卸载当前插件，当插件读取的数据完全错误时相当有用。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="449"/>
        <source>&amp;Settings</source>
        <translation>&amp;设置</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="452"/>
        <source>Configure Mumble</source>
        <translation>配置 Mumble</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="455"/>
        <source>Allows you to change most settings for Mumble.</source>
        <translation>允许你改变 Mumble 的大多数设置。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="463"/>
        <source>&amp;Audio Wizard</source>
        <translation>&amp;音频精灵</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="466"/>
        <source>Start the audio configuration wizard</source>
        <translation>启动音频配置精灵</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="469"/>
        <source>This will guide you through the process of configuring your audio hardware.</source>
        <translation>这将引导你进入配置你的音频硬件的流程。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="474"/>
        <source>&amp;What&apos;s This?</source>
        <translation>&amp;这是什么?</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="477"/>
        <source>Enter What&apos;s This? mode</source>
        <translation>进入 这是什么? 模式</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="480"/>
        <source>Click this to enter &quot;What&apos;s This?&quot; mode. Your cursor will turn into a question mark. Click on any button, menu choice or area to show a description of what it is.</source>
        <translation>点击进入&quot;这是什么?&quot;模式。你的鼠标指针将变成一个问号。点击任意按钮，菜单或区域来显示它是什么的描叙。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="485"/>
        <source>&amp;About</source>
        <translation>&amp;关于</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="488"/>
        <source>Information about Mumble</source>
        <translation>Mumble 的相关信息</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="491"/>
        <source>Shows a small dialog with information and license for Mumble.</source>
        <translation>显示一个 Mumble 相关信息和许可的对话框。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="499"/>
        <source>About &amp;Speex</source>
        <translation>关于 &amp;Speex</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="502"/>
        <source>Information about Speex</source>
        <translation>Speex 的相关信息</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="505"/>
        <source>Shows a small dialog with information about Speex.</source>
        <translation>显示一个 Speex 相关信息的小对话框。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="513"/>
        <source>About &amp;Qt</source>
        <translation>关于 &amp;Qt</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="516"/>
        <source>Information about Qt</source>
        <translation>Qt 相关信息</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="519"/>
        <source>Shows a small dialog with information about Qt.</source>
        <translation>显示一个 Qt 相关信息的小对话框。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="527"/>
        <source>Check for &amp;Updates</source>
        <translation>检测&amp;更新</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="530"/>
        <source>Check for new version of Mumble</source>
        <translation>检测 Mumble 的新版本</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="533"/>
        <source>Connects to the Mumble webpage to check if a new version is available, and notifies you with an appropriate download URL if this is the case.</source>
        <translation>连接到 Mumble 主页来检测是否有新版本可用，如果有新版本可用将通知你下载。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="544"/>
        <source>Sends a text message to all users in a channel.</source>
        <translation>向频道内所有用户发送文本信息。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="577"/>
        <source>&amp;Change Comment</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="580"/>
        <source>Change the comment on the selected user.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="583"/>
        <source>This allows you to change the comment (shown as a tooltip) of a user. Unless you&apos;ve been given extra privileges on the server, you can only change your own comment.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="588"/>
        <source>Certificate Wizard</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="591"/>
        <source>Configure certificates for strong authentication</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="594"/>
        <source>This starts the wizard for creating, importing and exporting certificates for authentication against servers.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="599"/>
        <source>Register</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="602"/>
        <source>Register user on server</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="605"/>
        <source>This will permanently register the user on the server.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="610"/>
        <source>Add &amp;Friend</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="613"/>
        <source>Adds a user as your friend.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="616"/>
        <source>This will add the user as a friend, so you can recognize him on this and other servers.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="621"/>
        <source>&amp;Remove Friend</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="624"/>
        <source>Removes a user from your friends.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="627"/>
        <source>This will remove a user from your friends list.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="632"/>
        <source>&amp;Update Friend</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="635"/>
        <source>Update name of your friend.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="638"/>
        <source>Your friend uses a different name than what is in your database. This will update the name.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="643"/>
        <source>Registered &amp;Users</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="646"/>
        <source>Edit registered users list</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="649"/>
        <source>This opens the editor for registered users, which allow you to change their name or unregister them.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="654"/>
        <source>Change Texture</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="657"/>
        <source>Change your overlay texture on this server</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="662"/>
        <source>&amp;Access Tokens</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="665"/>
        <source>Add or remove text-based access tokens</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="670"/>
        <source>Remove Texture</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="673"/>
        <source>Remove currently defined user texture.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Send &amp;Tree Message</source>
        <translation type="obsolete">发送信息到频道&amp;树</translation>
    </message>
    <message>
        <source>Send message to channel tree.</source>
        <translation type="obsolete">向频道树发送信息.</translation>
    </message>
    <message>
        <source>This sends a text message to a channel and it&apos;s subchannels.</source>
        <translation type="obsolete">向一个频道及其所有子频道发送文本消息。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="552"/>
        <source>&amp;Minimal View</source>
        <translation>&amp;简化窗口</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="555"/>
        <source>Toggle minimal window modes</source>
        <translation>启用简化窗口模式</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="558"/>
        <source>This will toggle minimal mode, where the log window and menu is hidden.</source>
        <translation>这将启用简化窗口模式，该模式将隐藏日志窗口和菜单。</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="201"/>
        <source>Joined server: %1.</source>
        <translation>加入服务器: %1.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="248"/>
        <location filename="Messages.cpp" line="349"/>
        <location filename="Messages.cpp" line="389"/>
        <source>the server</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="255"/>
        <source>You were unmuted and undeafened by %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="281"/>
        <source>You muted and deafened %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="284"/>
        <source>You unmuted and undeafened %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="296"/>
        <source>You undeafened %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="308"/>
        <source>%1 muted and deafened by %2.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="311"/>
        <source>%1 unmuted and undeafened by %2.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="329"/>
        <source>%1 suppressed by %2.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="356"/>
        <source>%1 moved to %2.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="358"/>
        <source>%1 moved to %2 by %3.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="368"/>
        <source>%1 moved in from %2 by %3.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="403"/>
        <source>Left server: %1.</source>
        <translation>离开服务器: %1.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="230"/>
        <source>%1 is now muted and deafened.</source>
        <translation>%1 现在被静音和耳聋.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="134"/>
        <source>You were denied %1 privileges in %2.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="136"/>
        <source>%3 was denied %1 privileges in %2.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="140"/>
        <source>Denied: Cannot modify SuperUser.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="144"/>
        <source>Denied: Invalid channel name.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="148"/>
        <source>Denied: Text message too long.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="168"/>
        <source>Denied: Operation not permitted in temporary channel.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="175"/>
        <source>You need a certificate to perform this operation.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="177"/>
        <source>%1 does not have a certificate.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="184"/>
        <source>Permission denied.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="232"/>
        <source>%1 is now muted.</source>
        <translation>%1 现在被静音。</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="234"/>
        <source>%1 is now unmuted.</source>
        <translation>%1 现在被取消静音。</translation>
    </message>
    <message>
        <source>server</source>
        <translation type="obsolete">服务器</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="260"/>
        <source>You were muted by %1.</source>
        <translation>你被 %1 静音。</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="273"/>
        <source>You were suppressed by %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="275"/>
        <source>You were unsuppressed by %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="262"/>
        <source>You were unmuted by %1.</source>
        <translation>你被 %1 取消静音。</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="289"/>
        <source>You muted %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="304"/>
        <source>You unsuppressed %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="291"/>
        <source>You unmuted %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="316"/>
        <source>%1 muted by %2.</source>
        <translation>%1 被 %2 静音。</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="331"/>
        <source>%1 unsuppressed by %2.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="318"/>
        <source>%1 unmuted by %2.</source>
        <translation>%1 被 %2 取消静音。</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="487"/>
        <source>the server</source>
        <comment>message from</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="626"/>
        <source>Unable to find matching CELT codecs with other clients. You will not be able to talk to all users.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="252"/>
        <source>You were muted and deafened by %1.</source>
        <oldsource>You were deafened by %1.</oldsource>
        <translation type="unfinished">你被 %1 耳聋。</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="267"/>
        <source>You were undeafened by %1.</source>
        <translation type="unfinished">你被 %1 取消耳聋。</translation>
    </message>
    <message>
        <source>%1 deafened by %2.</source>
        <translation type="obsolete">%1 被 %2 耳聋。</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="323"/>
        <source>%1 undeafened by %2.</source>
        <translation type="unfinished">%1 被 %2 取消耳聋。</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="396"/>
        <source>You were kicked from the server by %1: %2.</source>
        <translation>你被 %1 从服务器踢出: %2。</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="401"/>
        <source>%3 was kicked from the server by %1: %2.</source>
        <translation>%3 被 %1 从服务器踢出: %2。</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="394"/>
        <source>You were kicked and banned from the server by %1: %2.</source>
        <translation>你被 %1 从服务器踢出并封禁: %2。</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="399"/>
        <source>%3 was kicked and banned from the server by %1: %2.</source>
        <translation>%3 被 %1 从服务器踢出并封禁: %2。</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="353"/>
        <source>You were moved to %1 by %2.</source>
        <translation>你被从 %1 移动到 %2。</translation>
    </message>
    <message>
        <source>%1 left channel.</source>
        <translation type="obsolete">%1 离开频道。</translation>
    </message>
    <message>
        <source>%1 moved out by %2 to %3.</source>
        <translation type="obsolete">%1 被 %2 移动到 %3。</translation>
    </message>
    <message>
        <source>%1 moved in by %2 from %3.</source>
        <translation type="obsolete">%1 被 %2 从 %3 移入。</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="366"/>
        <source>%1 entered channel.</source>
        <translation>%1 进入频道。</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="87"/>
        <source>Server connection rejected: %1.</source>
        <translation>服务器拒绝连接: %1。</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="182"/>
        <source>Denied: %1.</source>
        <translation>拒绝: %1。</translation>
    </message>
    <message>
        <source>Server maximum bandwidth is only %1 kbit/s. Quality auto-adjusted.</source>
        <translation type="obsolete">服务器最大带宽是 %1 千比特/秒。自动调整质量。</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="488"/>
        <source>From %1: %2</source>
        <translation>发送自 %1: %2</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="489"/>
        <source>Message from %1</source>
        <translation>来至 %1 的消息</translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="1251"/>
        <source>You have Channel Dragging set to &quot;Do Nothing&quot; so the channel wasn&apos;t moved.</source>
        <translation>你设置频道拖动为“无动作&quot;因此频道没有被移动。</translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="1257"/>
        <source>Unknown Channel Drag mode in UserModel::dropMimeData.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Unknown Channel Drag mode in PlayerModel::dropMimeData.</source>
        <translation type="obsolete">未知的频道拖动模式 PlayerModel::dropMimeData。</translation>
    </message>
    <message>
        <location filename="main.cpp" line="284"/>
        <source>Welcome to Mumble.</source>
        <translation>欢迎来到 Mumble。</translation>
    </message>
    <message>
        <location filename="main.cpp" line="301"/>
        <source>This is the first time you&apos;re starting Mumble.&lt;br /&gt;Would you like to go through the Audio Wizard to configure your soundcard?</source>
        <translation>这是你第一次启动 Mumble.&lt;br /&gt;你想用音频精灵配置声卡吗？</translation>
    </message>
    <message>
        <source>Server</source>
        <comment>message from</comment>
        <translation type="obsolete">服务器</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="566"/>
        <source>Hide Frame</source>
        <translation>隐藏边框</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="569"/>
        <source>Toggle showing frame on minimal window</source>
        <translation>在简化窗口模式下切换边框显示</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="572"/>
        <source>This will toggle whether the minimal window should have a frame for moving and resizing.</source>
        <translation>该选项切换简化窗口是否有个用来移动和改变大小的边框。</translation>
    </message>
</context>
<context>
    <name>NetworkConfig</name>
    <message>
        <location filename="NetworkConfig.cpp" line="52"/>
        <source>Network</source>
        <translation>网络</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="14"/>
        <source>Form</source>
        <translation>表单</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="26"/>
        <source>Connection</source>
        <translation>连接</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="32"/>
        <source>Use TCP compatibility mode</source>
        <oldsource>Use TCP compatability mode</oldsource>
        <translation>使用 TCP 兼容模式</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="35"/>
        <source>&lt;b&gt;Enable TCP compatibility mode&lt;/b&gt;.&lt;br /&gt;This will make Mumble use only TCP when communicating with the server. This will increase overhead and cause lost packets to produce noticeable pauses in communication, so this should only be used if you are unable to use the default (which uses UDP for voice and TCP for control).</source>
        <translation>&lt;b&gt;启用 TCP 兼容模式&lt;/b&gt;。&lt;br/&gt;这将使 Mumble 与服务器通信时只使用 TCP。这将增加系统开销并导致丢包从而使通信时出现明显的暂停，因此只有在你不能使用默认模式时使用它（默认模式使用 UDP 来传输语音，TCP 来控制）。</translation>
    </message>
    <message>
        <source>Use TCP mode</source>
        <translation type="obsolete">使用 TCP 模式</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="58"/>
        <source>Reconnect when disconnected</source>
        <translation>连接断开后重新连接</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="61"/>
        <source>&lt;b&gt;Reconnect when disconnected&lt;/b&gt;.&lt;br /&gt;This will make Mumble try to automatically reconnect after 10 seconds if your server connection fails.</source>
        <translation>&lt;b&gt;连接断开后重新连接&lt;/b&gt;。&lt;br/&gt;这将使 Mumble 在与服务器的连接断开 10秒后自动重新连接。</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="64"/>
        <source>Reconnect automatically</source>
        <translation>自动重新连接</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="93"/>
        <source>Proxy</source>
        <translation>代理</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="99"/>
        <source>Type</source>
        <translation>类型</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="112"/>
        <source>Type of proxy to connect through</source>
        <translation>连接通过的代理类型</translation>
    </message>
    <message>
        <source>&lt;b&gt;Type of proxy to connect through&lt;/b&gt;&lt;br /&gt;This makes Mumble connect through a proxy for all outgoing connections. Note: Proxy tunneling forces Mumble into TCP compatibility mode, causing all voice data to be sent via the control channel.</source>
        <oldsource>&lt;b&gt;Type of proxy to connect through&lt;/b&gt;&lt;br /&gt;This makes Mumble connect through a proxy for all outgoing connections. Note: Proxy tunneling forces Mumble into TCP compatability mode, causing all voice data to be sent via the control channel.</oldsource>
        <translation type="obsolete">&lt;b&gt;连接通过的代理类型&lt;/b&gt;&lt;br/&gt;这使 Mumble 通过代理来连接到所有出外的连接。注意: 代理隧道封装会迫使 Mumble 进入 TCP 兼容模式，导致所有语音数据都通过控制信道发送。</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="119"/>
        <source>Direct connection</source>
        <translation>直接连接</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="124"/>
        <source>HTTP(S) proxy</source>
        <translation>HTTP(S) 代理</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="129"/>
        <source>SOCKS5 proxy</source>
        <translation>SOCKS5 代理</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="137"/>
        <source>Hostname</source>
        <translation>主机名</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="153"/>
        <source>Hostname of the proxy</source>
        <translation>代理主机名</translation>
    </message>
    <message>
        <source>&lt;b&gt;Hostname of the proxy.&lt;b&gt;&lt;br /&gt;This field specifies the hostname of the proxy you wish to tunnel network traffic through.</source>
        <translation type="obsolete">&lt;b&gt;代理主机名。&lt;/b&gt;&lt;br /&gt;该字段指定你希望用来穿隧传输网络流量的代理主机名。</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="38"/>
        <source>Force TCP mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="45"/>
        <source>Enable QoS to prioritize packets</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="48"/>
        <source>This will enable QoS, which will attempt to prioritize voice packets over other traffic.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="51"/>
        <source>Use Quality of Service</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="71"/>
        <source>Don&apos;t send certificate to server and don&apos;t save passwords. (Not saved).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="74"/>
        <source>&lt;b&gt;This will suppress identity information from the client.&lt;/b&gt;&lt;p&gt;The client will not identify itself with a certificate, even if defined, and will not cache passwords for connections. This is primarily a test-option and is not saved.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="77"/>
        <source>Suppress certificate and password storage</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="115"/>
        <source>&lt;b&gt;Type of proxy to connect through.&lt;/b&gt;&lt;br /&gt;This makes Mumble connect through a proxy for all outgoing connections. Note: Proxy tunneling forces Mumble into TCP compatibility mode, causing all voice data to be sent via the control channel.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="156"/>
        <source>&lt;b&gt;Hostname of the proxy.&lt;/b&gt;&lt;br /&gt;This field specifies the hostname of the proxy you wish to tunnel network traffic through.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="178"/>
        <source>Port</source>
        <translation>端口</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="203"/>
        <source>Port number of the proxy</source>
        <translation>代理端口号</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="206"/>
        <source>&lt;b&gt;Port number of the proxy.&lt;/b&gt;&lt;br /&gt;This field specifies the port number that the proxy expects connections on.</source>
        <translation>&lt;b&gt;代理服务器端口号。&lt;/b&gt;&lt;br/&gt;该字段指定代理服务器使用的端口号。</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="222"/>
        <source>Username</source>
        <translation>用户名</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="229"/>
        <source>Username for proxy authentication</source>
        <translation>用于代理认证的用户名</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="232"/>
        <source>&lt;b&gt;Username for proxy authentication.&lt;/b&gt;&lt;br /&gt;This specifies the username you use for authenticating yourself with the proxy. In case the proxy does not use authentication, or you want to connect anonymously, simply leave this field blank.</source>
        <translation>&lt;b&gt;用于代理认证的用户名。&lt;/b&gt;&lt;br /&gt;该字段指定用于代理认证的用户名。如果代理不需要认证，或者你想要使用匿名连接，请将此字段留白。</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="239"/>
        <source>Password</source>
        <translation>密码</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="246"/>
        <source>Password for proxy authentication</source>
        <translation>用于代理认证的密码</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="249"/>
        <source>&lt;b&gt;Password for proxy authentication.&lt;/b&gt;&lt;br /&gt;This specifies the password you use for authenticating yourself with the proxy. In case the proxy does not use authentication, or you want to connect anonymously, simply leave this field blank.</source>
        <translation>&lt;b&gt;用于代理认证的密码。&lt;/b&gt;&lt;br/&gt;该字段指定用于代理认证的密码。如果代理不需要认证，或者你想要使用匿名连接，请将此字段留白。</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="268"/>
        <source>Misc</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="274"/>
        <source>Prevent log from downloading images</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="277"/>
        <source>&lt;b&gt;Disable image download&lt;/b&gt;&lt;br/&gt;
Prevents the client from downloading images embedded into chat messages with the img tag.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="281"/>
        <source>Disable image download</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="291"/>
        <source>Mumble services</source>
        <translation>Mumble 服务</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="297"/>
        <source>Check for new releases of Mumble automatically.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="300"/>
        <source>This will check for new releases of Mumble every time you start the program, and notify you if one is available.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="303"/>
        <source>Check for application updates on startup</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="310"/>
        <source>Check for new releases of plugins automatically.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="313"/>
        <source>This will check for new releases of plugins every time you start the program, and download them automatically.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="316"/>
        <source>Download plugin updates on startup</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="326"/>
        <source>&lt;b&gt;Submit anonymous statistics.&lt;/b&gt;&lt;br /&gt;Mumble has a small development team, and as such needs to focus its development where it is needed most. By submitting a bit of statistics you help the project determine where to focus development.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Check for updates on startup</source>
        <translation type="obsolete">启动时检测更新</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="323"/>
        <source>Submit anonymous statistics to the Mumble project</source>
        <translation>递交匿名统计数据给 Mumble 项目</translation>
    </message>
    <message>
        <source>&lt;b&gt;Submit anonymous statistics.&lt;/b&gt;&lt;br /&gt;Mumble has a small development team, and as such needs to focus it&apos;s development where it&apos;s needed most. By submitting a bit of statistics you help the project determine where to focus development.</source>
        <translation type="obsolete">&lt;b&gt;递交匿名统计数据。&lt;/b&gt;&lt;br /&gt;Mumble 有一个小的开发团队，并需要专注于开发用户最想要的功能。通过递交匿名统计数据你将帮助开发团队确定重点开发什么。</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="329"/>
        <source>Submit anonymous statistics</source>
        <translation>递交匿名统计数据</translation>
    </message>
</context>
<context>
    <name>Overlay</name>
    <message>
        <location filename="Overlay.cpp" line="192"/>
        <location filename="Overlay.cpp" line="204"/>
        <source>Mumble</source>
        <translation></translation>
    </message>
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
    <message>
        <location filename="Overlay.cpp" line="192"/>
        <source>Failed to load overlay library. This means either that:
- the library (mumble_ol.dll) wasn&apos;t found in the directory you ran Mumble from
- you&apos;re on an OS earlier than WinXP SP2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Overlay.cpp" line="204"/>
        <source>Failed to initialize overlay memory. This usually means that the shared memory is locked by the OS, and you need to reboot to release it.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>OverlayConfig</name>
    <message>
        <location filename="Overlay.cpp" line="47"/>
        <source>Show no one</source>
        <translation>不显示任何人</translation>
    </message>
    <message>
        <location filename="Overlay.cpp" line="48"/>
        <source>Show only talking</source>
        <translation>当发言时显示</translation>
    </message>
    <message>
        <location filename="Overlay.cpp" line="49"/>
        <source>Show everyone</source>
        <translation>显示所有人</translation>
    </message>
    <message>
        <location filename="Overlay.cpp" line="122"/>
        <location filename="Overlay.ui" line="231"/>
        <source>Color for users</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Overlay.cpp" line="126"/>
        <location filename="Overlay.ui" line="245"/>
        <source>Color for talking users</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Overlay.cpp" line="130"/>
        <location filename="Overlay.ui" line="266"/>
        <source>Color for whispering users</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Overlay.cpp" line="134"/>
        <source>Color for channels</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Overlay.cpp" line="138"/>
        <source>Color for active channels</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Overlay.cpp" line="142"/>
        <source>Overlay</source>
        <translation></translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="14"/>
        <source>Form</source>
        <translation>表单</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="20"/>
        <source>Options</source>
        <translation>选项</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="26"/>
        <source>Enable overlay.</source>
        <translation>启用 overlay .</translation>
    </message>
    <message>
        <source>This sets whether the overlay is enabled or not. This setting is only checked with D3D9 applications are started, so make sure Mumble is running and this option is on before you start the application.&lt;br /&gt;Please note that if you start the application after starting Mumble, or if you disable the overlay while running, there is no safe way to restart the overlay without also restarting the application.</source>
        <translation type="obsolete">设置是否启用 overlay。该设置仅当应用程序已启动时才会被检查，因此确保 Mumble 正在运行并且该选项在你启动应用程序之前已被打开。&lt;br /&gt;请注意如果你在启动 Mumble 后启动应用程序，或者你在运行时禁用 overlay，重新启动 overlay 需要重新启动应用程序。</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="32"/>
        <source>Enable Overlay</source>
        <translation>启用 Overlay</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="39"/>
        <source>Who to show on the overlay</source>
        <translation>在 overlay 上显示谁</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="42"/>
        <source>&lt;b&gt;This sets who to show in the in-game overlay.&lt;/b&gt;&lt;br /&gt;If many people are connected to the same channel, the overlay list might be very long. Use this to shorten it.&lt;br /&gt;&lt;i&gt;No one&lt;/i&gt; - Don&apos;t show anyone (but leave overlay running).&lt;br /&gt;&lt;i&gt;Only talking&lt;/i&gt; - Only show talking people.&lt;br /&gt;&lt;i&gt;Everyone&lt;/i&gt; - Show everyone.</source>
        <translation>&lt;b&gt;设置在游戏中的 overlay 内显示谁。&lt;/b&gt;&lt;br /&gt;如果许多玩家已经连接到同一个频道，overlay 列表可能会非常长。使用该选项来让列表变短。&lt;br /&gt;&lt;i&gt;无人&lt;/i&gt; - 不显示任何人（但是让 overlay 运行).&lt;br /&gt;&lt;i&gt;仅当用户发言时&lt;/i&gt; - 仅显示正在发言的人。&lt;br /&gt;&lt;i&gt;所有人&lt;/i&gt; - 显示所有人。</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="49"/>
        <source>Always show yourself on overlay.</source>
        <translation>在 overlay 上总是显示自己。</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="52"/>
        <source>This sets whether to always show yourself or not. This setting is useful if you aren&apos;t showing everyone in the overlay, as then you would only see your own status if you were talking, which wouldn&apos;t let you see that you were deafened or muted.</source>
        <translation>设置是否总是显示自己。该选项只有当你不在 overlay 中显示所有人时才有用，你在讲话时只能看到自己的状态，你将看不到自己被禁音或耳聋。</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="55"/>
        <source>Always Show Self</source>
        <translation>总是显示自己</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="62"/>
        <source>Show User custom textures instead of text on the overlay.</source>
        <translation>在 overlay 上显示用户自定义纹理而不是文本。</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="65"/>
        <source>This sets whether to download and use custom textures for registered users. If disabled, the regular outline text will be used instead.</source>
        <translation>设置是否让注册用户下载并使用自定义纹理。如果禁用该选项，常规边框文字将被使用。</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="68"/>
        <source>Show User Textures</source>
        <translation>显示用户纹理</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="78"/>
        <source>Position</source>
        <translation>位置</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="84"/>
        <source>Let overlay grow upwards</source>
        <translation>让 overlay 往上增长</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="87"/>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow upwards if needed.</source>
        <translation>Overlay 试着保持足够小并固定在你选定的位置上。该选项允许 overlay 在需要时往上增长。</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="90"/>
        <source>Grow Up</source>
        <translation>往上增长</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="97"/>
        <source>Y-Position of Overlay</source>
        <translation>Overlay 的 Y-坐标</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="100"/>
        <source>This sets the relative Y position of the overlay.</source>
        <translation>设置 overlay 的相对 Y 坐标。</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="113"/>
        <source>Let overlay grow to the left</source>
        <translation>让 overlay 往左增长</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="116"/>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow to the left if needed.</source>
        <translation>Overlay 试着保持足够小并固定在你选定的位置上。该选项允许 overlay 在需要时往左增长。</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="119"/>
        <source>Grow Left</source>
        <translation>往左增长</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="126"/>
        <source>Let overlay grow to the right</source>
        <translation>让 overlay 往右增长</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="129"/>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow to the right if needed.</source>
        <translation>Overlay 试着保持足够小并固定在你选定的位置上。该选项允许 overlay 在需要时往右增长。</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="132"/>
        <source>Grow Right</source>
        <translation>往右增长</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="139"/>
        <source>Let overlay grow downwards</source>
        <translation>让 overlay 往下增长</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="142"/>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow downwards if needed.</source>
        <translation>Overlay 试着保持足够小并固定在你选定的位置上。该选项允许 overlay 在需要时往下增长。</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="145"/>
        <source>Grow Down</source>
        <translation>往下增长</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="152"/>
        <source>X-Position of Overlay</source>
        <translation>Overlay 的 X-坐标</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="155"/>
        <source>This sets the relative X position of the overlay.</source>
        <translation>设置 overlay 的相对 X 坐标。</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="171"/>
        <source>Font</source>
        <translation>字体</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="177"/>
        <source>Current Font</source>
        <translation>当前字体</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="224"/>
        <source>TextLabel</source>
        <translation>文本标签</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="191"/>
        <source>Set Font</source>
        <translation>设置字体</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="29"/>
        <source>This sets whether the overlay is enabled or not. This setting is only checked when applications are started, so make sure Mumble is running and this option is on before you start the application.&lt;br /&gt;Please note that if you start Mumble after starting the application, or if you disable the overlay while the application is running, there is no safe way to restart the overlay without also restarting the application.</source>
        <oldsource>This sets whether the overlay is enabled or not. This setting is only checked when applications are started, so make sure Mumble is running and this option is on before you start the application.&lt;br /&gt;Please note that if you start the application after starting Mumble, or if you disable the overlay while running, there is no safe way to restart the overlay without also restarting the application.</oldsource>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="198"/>
        <source>Maximum height</source>
        <translation>最大高度</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="205"/>
        <source>Maximum height of names.</source>
        <translation>名字的最大高度.</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="208"/>
        <source>This sets the maximum height of names shown, relative to the screen height. If your active 3D window is 800 pixels tall and this is set to 5%, each name will be 40 pixels tall. Note that the names will not be taller than 60 pixels no matter what you set here.</source>
        <translation>设置显示的名字的最大高度，相对于屏幕高度来说。如果你的活跃 3D 窗口是 800像素高而该值被设成 5%，每个名字会是 40像素高。注意无论你在这里设置什么值名字都不会高于 60像素.</translation>
    </message>
    <message>
        <source>Color for players</source>
        <translation type="obsolete">玩家颜色</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="238"/>
        <location filename="Overlay.ui" line="259"/>
        <location filename="Overlay.ui" line="280"/>
        <location filename="Overlay.ui" line="301"/>
        <location filename="Overlay.ui" line="322"/>
        <source>Change</source>
        <translation>更改</translation>
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
        <location filename="Overlay.ui" line="287"/>
        <source>Color for Channels</source>
        <translation>频道的颜色</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="308"/>
        <source>Color for active Channels</source>
        <translation>活跃频道的颜色</translation>
    </message>
</context>
<context>
    <name>PlayerModel</name>
    <message>
        <source>This is a player connected to the server. The icon to the left of the player indicates whether or not they are talking:&lt;br /&gt;&lt;img src=&quot;skin:talking_on.png&quot; /&gt; Talking&lt;br /&gt;&lt;img src=&quot;skin:talking_off.png&quot; /&gt; Not talking</source>
        <translation type="obsolete">这是一个连接到服务器的玩家。玩家左边的图标显示他是否在讲话。&lt;br /&gt;&lt;img src=&quot;skin:talking_on.png&quot; /&gt;发言中&lt;br /&gt;&lt;img src=&quot;skin:talking_off.png&quot; /&gt; 沉默中</translation>
    </message>
    <message>
        <source>This is a channel on the server. Only players in the same channel can hear each other.</source>
        <translation type="obsolete">这是服务器上的一个频道。只有同一频道的玩家能听到彼此的声音。</translation>
    </message>
    <message>
        <source>This shows the flags the player has on the server, if any:&lt;br /&gt;&lt;img src=&quot;skin:authenticated.png&quot; /&gt;Authenticated user&lt;br /&gt;&lt;img src=&quot;skin:muted_self.png&quot; /&gt;Muted (by self)&lt;br /&gt;&lt;img src=&quot;skin:muted_server.png&quot; /&gt;Muted (by admin)&lt;br /&gt;&lt;img src=&quot;skin:deafened_self.png&quot; /&gt;Deafened (by self)&lt;br /&gt;&lt;img src=&quot;skin:deafened_server.png&quot; /&gt;Deafened (by admin)&lt;br /&gt;A player muted by himself is probably just away, talking on the phone or something like that.&lt;br /&gt;A player muted by an admin is probably also just away, and the noise the player is making was annoying enough that an admin muted him.</source>
        <translation type="obsolete">显示该玩家在服务器上的标志:&lt;br /&gt;&lt;img src=&quot;skin:authenticated.png&quot; /&gt;已认证的用户&lt;br /&gt;&lt;img src=&quot;skin:muted_self.png&quot; /&gt;静音(被自己)&lt;br /&gt;&lt;img src=&quot;skin:muted_server.png&quot; /&gt;静音(被管理员)&lt;br /&gt;&lt;img src=&quot;skin:deafened_self.png&quot; /&gt;耳聋 (被自己)&lt;br /&gt;&lt;img src=&quot;skin:deafened_server.png&quot; /&gt;耳聋 (被管理员)&lt;br /&gt;玩家被自己禁音有可能是他处于离开状态，比如在接电话。&lt;br /&gt;玩家被管理员禁音也有可能是他处于离开状态，但是他发出了让管理员恼怒到禁音他的噪音。</translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="obsolete">名字</translation>
    </message>
    <message>
        <source>Flags</source>
        <translation type="obsolete">标志</translation>
    </message>
    <message>
        <source>Are you sure you want to drag this channel?</source>
        <translation type="obsolete">你确信要拖动这个频道吗？</translation>
    </message>
</context>
<context>
    <name>PluginConfig</name>
    <message>
        <location filename="Plugins.cpp" line="67"/>
        <location filename="Plugins.ui" line="42"/>
        <source>Plugins</source>
        <translation>插件</translation>
    </message>
    <message>
        <location filename="Plugins.cpp" line="95"/>
        <location filename="Plugins.cpp" line="107"/>
        <source>Mumble</source>
        <translation></translation>
    </message>
    <message>
        <location filename="Plugins.cpp" line="95"/>
        <source>Plugin has no configure function.</source>
        <translation>插件没有配置选项。</translation>
    </message>
    <message>
        <location filename="Plugins.cpp" line="107"/>
        <source>Plugin has no about function.</source>
        <translation>插件没有关于选项.</translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="14"/>
        <source>Form</source>
        <translation>表单</translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="20"/>
        <source>Options</source>
        <translation>选项</translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="26"/>
        <source>Enable plugins and transmit positional information</source>
        <translation>启用插件并传送位置信息</translation>
    </message>
    <message>
        <source>This enables plugins for supported games to fetch your in-game position and transmit that with each voice packet. This enables other players to hear your voice in-game from the direction your character is in relation to their own.</source>
        <translation type="obsolete">对支持的游戏启用插件以获取你在游戏中的位置并在每个语音包中传送。这将使别的玩家能根据你在游戏中的声音来判断你在游戏中相对于他们自己的方位。</translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="29"/>
        <source>This allows plugins for supported games to fetch your in-game position and transmit it with each voice packet. This enables other users to hear your voice in-game from the direction your character is in relation to their own.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="32"/>
        <source>Link to Game and Transmit Position</source>
        <translation>链接到游戏并传送位置</translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="53"/>
        <source>Reloads all plugins</source>
        <translation>重载所有插件</translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="56"/>
        <source>This rescans and reloads plugins. Use this if you just added or changed a plugin to the plugins directory.</source>
        <translation>重新扫描和重载插件。当你添加或改变一个插件目录中的插件时使用该选项。</translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="59"/>
        <source>&amp;Reload plugins</source>
        <translation>&amp;重载插件</translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="79"/>
        <source>Information about plugin</source>
        <translation>插件相关信息</translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="82"/>
        <source>This shows a small information message about the plugin.</source>
        <translation>显示插件相关信息。</translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="85"/>
        <source>&amp;About</source>
        <translation>&amp;关于</translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="92"/>
        <source>Show configuration page of plugin</source>
        <translation>显示插件配置页面</translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="95"/>
        <source>This shows the configuration page of the plugin, if any.</source>
        <translation>显示插件配置页面。</translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="98"/>
        <source>&amp;Configure</source>
        <translation>&amp;配置</translation>
    </message>
</context>
<context>
    <name>Plugins</name>
    <message>
        <location filename="Plugins.cpp" line="499"/>
        <location filename="Plugins.cpp" line="505"/>
        <source>Downloaded new or updated plugin to %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Plugins.cpp" line="507"/>
        <source>Failed to install new plugin to %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Plugins.cpp" line="286"/>
        <source>%1 lost link.</source>
        <translation>%1 的链接丢失。</translation>
    </message>
    <message>
        <location filename="Plugins.cpp" line="333"/>
        <source>%1 linked.</source>
        <translation>%1 已链接上。</translation>
    </message>
</context>
<context>
    <name>PortAudioSystem</name>
    <message>
        <location filename="PAAudio.cpp" line="275"/>
        <source>Default Device</source>
        <translation>默认设备</translation>
    </message>
</context>
<context>
    <name>PulseAudioSystem</name>
    <message>
        <location filename="PulseAudio.cpp" line="549"/>
        <source>Default Input</source>
        <translation>默认输入</translation>
    </message>
    <message>
        <location filename="PulseAudio.cpp" line="550"/>
        <source>Default Output</source>
        <translation>默认输出</translation>
    </message>
</context>
<context>
    <name>RichTextEditor</name>
    <message>
        <location filename="RichTextEditor.cpp" line="240"/>
        <source>Failed to load image</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="RichTextEditor.cpp" line="240"/>
        <source>Image file too large to embed in document. Please use images smaller than %1 kB.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="RichTextEditor.ui" line="14"/>
        <source>TabWidget</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="RichTextEditor.ui" line="24"/>
        <source>Display</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="RichTextEditor.ui" line="41"/>
        <source>Source Text</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="RichTextEditor.ui" line="58"/>
        <source>&amp;Bold</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="RichTextEditor.ui" line="61"/>
        <source>Ctrl+B</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="RichTextEditor.ui" line="73"/>
        <source>&amp;Italic</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="RichTextEditor.ui" line="76"/>
        <source>Italic</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="RichTextEditor.ui" line="79"/>
        <source>Ctrl+I</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="RichTextEditor.ui" line="91"/>
        <source>Underline</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="RichTextEditor.ui" line="94"/>
        <source>Ctrl+U</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="RichTextEditor.ui" line="99"/>
        <source>Color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="RichTextEditor.ui" line="108"/>
        <source>Insert Link</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="RichTextEditor.ui" line="111"/>
        <source>Ctrl+L</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="RichTextEditor.ui" line="120"/>
        <source>Insert Image</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>RichTextEditorLink</name>
    <message>
        <location filename="RichTextEditorLink.ui" line="14"/>
        <source>Add Link</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="RichTextEditorLink.ui" line="20"/>
        <source>URL</source>
        <translation type="unfinished">网址</translation>
    </message>
    <message>
        <location filename="RichTextEditorLink.ui" line="33"/>
        <source>Text</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ServerHandler</name>
    <message>
        <location filename="ServerHandler.cpp" line="369"/>
        <source>UDP packets cannot be sent to or received from the server. Switching to TCP mode.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ServerHandler.cpp" line="371"/>
        <source>UDP packets cannot be sent to the server. Switching to TCP mode.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ServerHandler.cpp" line="373"/>
        <source>UDP packets cannot be received from the server. Switching to TCP mode.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ServerHandler.cpp" line="380"/>
        <source>UDP packets can be sent to and received from the server. Switching back to UDP mode.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ServerHandler.cpp" line="409"/>
        <source>Connection timed out</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ServerView</name>
    <message>
        <location filename="ConnectDialog.cpp" line="72"/>
        <source>Favorite</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="78"/>
        <source>LAN</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="86"/>
        <source>Public Internet</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="92"/>
        <source>Asia</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="93"/>
        <source>North America</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="94"/>
        <source>South America</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="95"/>
        <source>Europe</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="96"/>
        <source>Oceania</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ShortcutActionWidget</name>
    <message>
        <location filename="GlobalShortcut.cpp" line="214"/>
        <source>Unassigned</source>
        <translation>未分配</translation>
    </message>
</context>
<context>
    <name>ShortcutDelegate</name>
    <message>
        <location filename="GlobalShortcut.cpp" line="574"/>
        <source>On</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="GlobalShortcut.cpp" line="576"/>
        <source>Off</source>
        <translation type="unfinished">关闭</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.cpp" line="578"/>
        <source>Toggle</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="GlobalShortcut.cpp" line="584"/>
        <source>Unassigned</source>
        <translation>未分配</translation>
    </message>
</context>
<context>
    <name>ShortcutKeyWidget</name>
    <message>
        <location filename="GlobalShortcut.cpp" line="150"/>
        <source>Press Shortcut</source>
        <translation>输入快捷键</translation>
    </message>
</context>
<context>
    <name>ShortcutTargetDialog</name>
    <message>
        <location filename="GlobalShortcut.cpp" line="353"/>
        <source>Root</source>
        <translation type="unfinished">根</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.cpp" line="358"/>
        <source>Parent</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="GlobalShortcut.cpp" line="363"/>
        <source>Current</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="GlobalShortcut.cpp" line="368"/>
        <source>Subchannel #%1</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ShortcutTargetWidget</name>
    <message>
        <location filename="GlobalShortcut.cpp" line="455"/>
        <source>...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="GlobalShortcut.cpp" line="493"/>
        <source>, </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="GlobalShortcut.cpp" line="499"/>
        <source>Root</source>
        <translation type="unfinished">根</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.cpp" line="501"/>
        <source>Parent</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="GlobalShortcut.cpp" line="503"/>
        <source>Current</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="GlobalShortcut.cpp" line="505"/>
        <source>Subchannel #%1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="GlobalShortcut.cpp" line="512"/>
        <source>Invalid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="GlobalShortcut.cpp" line="515"/>
        <source>&lt;Empty&gt;</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ShortcutToggleWidget</name>
    <message>
        <location filename="GlobalShortcut.cpp" line="254"/>
        <source>Off</source>
        <translation type="unfinished">关闭</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.cpp" line="258"/>
        <source>Toggle</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="GlobalShortcut.cpp" line="262"/>
        <source>On</source>
        <translation type="unfinished"></translation>
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
        <source>Message to send</source>
        <translation type="obsolete">要发送的消息</translation>
    </message>
    <message>
        <source>Preview</source>
        <translation type="obsolete">预览</translation>
    </message>
    <message>
        <location filename="TextMessage.h" line="46"/>
        <source>Enter text</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="TextMessage.ui" line="24"/>
        <source>If checked the message is recursively sent to all subchannels</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="TextMessage.ui" line="27"/>
        <source>Send recursively to subchannels</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Tokens</name>
    <message>
        <location filename="Tokens.cpp" line="69"/>
        <source>Empty Token</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Tokens.ui" line="14"/>
        <source>Mumble - Access Tokens</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Tokens.ui" line="20"/>
        <source>List of access tokens on current server</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Tokens.ui" line="23"/>
        <source>&lt;b&gt;This is an editable list of access tokens on the connected server.&lt;/b&gt;
&lt;br /&gt;
An access token is a text string, which can be used as a password for very simple access management on channels. Mumble will remember the tokens you&apos;ve used and resend them to the server next time you reconnect, so you don&apos;t have to enter these every time.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Tokens.ui" line="47"/>
        <source>Add a token</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Tokens.ui" line="50"/>
        <source>&amp;Add</source>
        <translation type="unfinished">&amp;添加</translation>
    </message>
    <message>
        <location filename="Tokens.ui" line="57"/>
        <source>Remove a token</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Tokens.ui" line="60"/>
        <source>&amp;Remove</source>
        <translation type="unfinished">&amp;删除</translation>
    </message>
</context>
<context>
    <name>UserEdit</name>
    <message>
        <location filename="UserEdit.ui" line="14"/>
        <location filename="UserEdit.ui" line="20"/>
        <source>Registered Users</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="UserEdit.ui" line="26"/>
        <source>Remove</source>
        <translation type="unfinished">删除</translation>
    </message>
</context>
<context>
    <name>UserModel</name>
    <message>
        <location filename="UserModel.cpp" line="525"/>
        <source>This is a user connected to the server. The icon to the left of the user indicates whether or not they are talking:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="526"/>
        <source>Talking to your channel.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="527"/>
        <source>Whispering directly to your channel.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="528"/>
        <source>Whispering directly to you.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="529"/>
        <source>Not talking.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="537"/>
        <source>This is a channel on the server. The icon indicates the state of the channel:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="538"/>
        <source>Your current channel.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="539"/>
        <source>A channel that is linked with your channel. Linked channels can talk to each other.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="540"/>
        <source>A channel on the server that you are not linked to.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="556"/>
        <source>This shows the flags the user has on the server, if any:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="557"/>
        <source>On your friend list</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="558"/>
        <source>Authenticated user</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="559"/>
        <source>Muted (manually muted by self)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="560"/>
        <source>Muted (manually muted by admin)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="561"/>
        <source>Muted (not allowed to speak in current channel)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="562"/>
        <source>Muted (muted by you, only on your machine)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="564"/>
        <source>Deafened (by self)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="565"/>
        <source>Deafened (by admin)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="566"/>
        <source>User has a new comment set (click to show)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="567"/>
        <source>User has a comment set, which you&apos;ve already seen. (click to show)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="574"/>
        <source>This shows the flags the channel has, if any:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="575"/>
        <source>Channel has a new comment set (click to show)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="576"/>
        <source>Channel has a comment set, which you&apos;ve already seen. (click to show)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="594"/>
        <source>Name</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="596"/>
        <source>Flags</source>
        <translation type="unfinished">标志</translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="1245"/>
        <location filename="UserModel.cpp" line="1335"/>
        <location filename="UserModel.cpp" line="1355"/>
        <source>Mumble</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="1245"/>
        <source>Are you sure you want to drag this channel?</source>
        <translation type="unfinished">你确信要拖动这个频道吗？</translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="1335"/>
        <location filename="UserModel.cpp" line="1355"/>
        <source>Cannot perform this movement automatically, please reset the numeric sorting indicators or adjust it manually.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>VersionCheck</name>
    <message>
        <location filename="VersionCheck.cpp" line="97"/>
        <source>Mumble failed to retrieve version information from the SourceForge server.</source>
        <translation>Mumble 无法从 SouceForge 服务器获取版本信息。</translation>
    </message>
</context>
<context>
    <name>ViewCert</name>
    <message>
        <location filename="ViewCert.cpp" line="36"/>
        <source>Certificate Chain Details</source>
        <translation>认证链详细信息</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="42"/>
        <source>Certificate chain</source>
        <translation>认证链</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="48"/>
        <source>%1 %2</source>
        <translation></translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="51"/>
        <source>Certificate details</source>
        <translation>认证详细信息</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="76"/>
        <location filename="ViewCert.cpp" line="89"/>
        <source>Common Name: %1</source>
        <translation>通用名: %1</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="77"/>
        <location filename="ViewCert.cpp" line="90"/>
        <source>Organization: %1</source>
        <translation>组织: %1</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="78"/>
        <source>Subunit: %1</source>
        <translation>部门: %1</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="79"/>
        <location filename="ViewCert.cpp" line="92"/>
        <source>Country: %1</source>
        <translation>国家: %1</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="80"/>
        <location filename="ViewCert.cpp" line="93"/>
        <source>Locality: %1</source>
        <translation>位置: %1</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="81"/>
        <location filename="ViewCert.cpp" line="94"/>
        <source>State: %1</source>
        <translation>省(市): %1</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="82"/>
        <source>Valid from: %1</source>
        <translation>有效期始于: %1</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="83"/>
        <source>Valid to: %1</source>
        <translation>有效期到: %1</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="84"/>
        <source>Serial: %1</source>
        <translation>序列号: %1</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="85"/>
        <source>Public Key: %1 bits %2</source>
        <translation>公开密钥: %1 比特 %2</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="85"/>
        <source>RSA</source>
        <translation>非对称加密算法</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="85"/>
        <source>DSA</source>
        <translation>数字签名算法</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="86"/>
        <source>Digest (MD5): %1</source>
        <translation></translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="88"/>
        <source>Issued by:</source>
        <translation>发布者:</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="91"/>
        <source>Unit Name: %1</source>
        <translation>部门名: %1</translation>
    </message>
</context>
<context>
    <name>WASAPISystem</name>
    <message>
        <location filename="WASAPI.cpp" line="177"/>
        <source>Default Device</source>
        <translation>默认设备</translation>
    </message>
</context>
</TS>
