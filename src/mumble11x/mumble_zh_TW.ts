<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.0" language="zh_TW">
<context>
    <name>ACLEditor</name>
    <message>
        <location filename="ACLEditor.cpp" line="+47"/>
        <source>Mumble - Edit ACL for %1</source>
        <translation>Mumble - 編輯 訪問控制列表 %1</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Deny</source>
        <translation>拒絕</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Allow</source>
        <translation>允許</translation>
    </message>
    <message>
        <location line="+102"/>
        <source>Allow %1</source>
        <translation>允許 %1</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Deny %1</source>
        <translation>拒絕 %1</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This grants the %1 privilege. If a privilege is both allowed and denied, it is denied.&lt;br /&gt;%2</source>
        <translation>授予 %1 權限.如果一個權限同時被拒絕和允許，則被視為拒絕。&lt;br /&gt;%2</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This revokes the %1 privilege. If a privilege is both allowed and denied, it is denied.&lt;br /&gt;%2</source>
        <translation>撤銷 %1 權限.如果一個權限同時被拒絕和允許，則被視為拒絕。&lt;br /&gt;%2</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui"/>
        <source>Dialog</source>
        <translation>對話框</translation>
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
        <translation>這是當前頻道內定義的所有群。要創建一個新群，請輸入群名然后按回車。</translation>
    </message>
    <message>
        <location/>
        <source>Remove selected group</source>
        <translation>刪除選中的群</translation>
    </message>
    <message>
        <location/>
        <source>This removes the currently selected group. If the group was inherited, it will not be removed from the list, but all local information about the group will be cleared.</source>
        <translation>刪除當前選中的群。如果該群有子群，它將不會被從列表中刪除，但是與該群相關的所有本地信息將被清除.</translation>
    </message>
    <message>
        <location/>
        <source>Remove</source>
        <translation>刪除</translation>
    </message>
    <message>
        <location/>
        <source>Inherit group members from parent</source>
        <translation>從父繼承群成員</translation>
    </message>
    <message>
        <location/>
        <source>This inherits all the members in the group from the parent, if the group is marked as &lt;i&gt;Inheritable&lt;/i&gt; in the parent channel.</source>
        <translation>如果群在父頻道里被標記為&lt;i&gt;可繼承&lt;/i&gt;,則從父繼承所有群成員。</translation>
    </message>
    <message>
        <location/>
        <source>Inherit</source>
        <translation>繼承</translation>
    </message>
    <message>
        <location/>
        <source>Make group inheritable to sub-channels</source>
        <translation>使群可被子頻道繼承</translation>
    </message>
    <message>
        <location/>
        <source>This makes this group inheritable to sub-channels. If the group is non-inheritable, sub-channels are still free to create a new group with the same name.</source>
        <translation>使該群可被子頻道繼承。如果該群不可被繼承，則可在子頻道內創建同名新群。</translation>
    </message>
    <message>
        <location/>
        <source>Inheritable</source>
        <translation>可繼承</translation>
    </message>
    <message>
        <location/>
        <source>Group was inherited from parent channel</source>
        <translation>繼承自父頻道的群</translation>
    </message>
    <message>
        <location/>
        <source>This indicates that the group was inherited from the parent channel. You cannot edit this flag, it&apos;s just for information.</source>
        <translation>這表明該群繼承自父頻道。該標志僅供參考，你不能編輯該標志。</translation>
    </message>
    <message>
        <location/>
        <source>Inherited</source>
        <translation>繼承的</translation>
    </message>
    <message>
        <location/>
        <source>Members</source>
        <translation>成員</translation>
    </message>
    <message>
        <location/>
        <source>Add</source>
        <translation>添加</translation>
    </message>
    <message>
        <location/>
        <source>Add to Remove</source>
        <translation>添加到刪除列表</translation>
    </message>
    <message>
        <location/>
        <source>Add member to group</source>
        <translation>添加群成員</translation>
    </message>
    <message>
        <location/>
        <source>Type in the name of a player you wish to add to the group and click Add.</source>
        <translation>輸入你要添加到群里的玩家名字然后點擊添加。</translation>
    </message>
    <message>
        <location/>
        <source>Remove member from group</source>
        <translation>從群里刪除成員</translation>
    </message>
    <message>
        <location/>
        <source>Type in the name of a player you wish to remove from the group and click Add.</source>
        <translation>輸入你要從群內刪除的成員然后點擊刪除。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;ACL</source>
        <translation>&amp;訪問控制列表</translation>
    </message>
    <message>
        <location/>
        <source>Active ACLs</source>
        <translation>當前訪問控制列表</translation>
    </message>
    <message>
        <location/>
        <source>List of entries</source>
        <translation>條目列表</translation>
    </message>
    <message>
        <location/>
        <source>This shows all the entries active on this channel. Entries inherited from parent channels will be show in italics.&lt;br /&gt;ACLs are evaluated top to bottom, meaning priority increases as you move down the list.</source>
        <translation>顯示該頻道內所有活躍條目。繼承自父頻道的條目將以斜體顯示.&lt;br/&gt;訪問控制列表是自上而下處理，意味著列表越往下優先級越高。</translation>
    </message>
    <message>
        <location/>
        <source>Inherit ACL of parent?</source>
        <translation>繼承父訪問控制列表？</translation>
    </message>
    <message>
        <location/>
        <source>This sets whether or not the ACL up the chain of parent channels are applied to this object. Only those entries that are marked in the parent as &quot;Apply to sub-channels&quot; will be inherited.</source>
        <translation>設置是否對當前對象應用父頻道的訪問控制列表。只有那些在父頻道內被標記為“應用到子頻道”的條目將被繼承。</translation>
    </message>
    <message>
        <location/>
        <source>Inherit ACLs</source>
        <translation>繼承訪問控制列表</translation>
    </message>
    <message>
        <location/>
        <source>Move entry up</source>
        <translation>上移條目</translation>
    </message>
    <message>
        <location/>
        <source>This moves the entry up in the list. As entries are evaluated in order, this may change the effective permissions of users. You cannot move an entry above an inherited entry, if you really need that you&apos;ll have to duplicate the inherited entry.</source>
        <translation>在列表中上移條目。由于條目自上而下被處理，這有可能改變用戶的有效權限.你不能將一個條目移到繼承的條目之上，如果你確實想這么做，你需要復制繼承的條目。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Up</source>
        <translation>&amp;上移</translation>
    </message>
    <message>
        <location/>
        <source>Move entry down</source>
        <translation>下移條目</translation>
    </message>
    <message>
        <location/>
        <source>This moves the entry down in the list. As entries are evaluated in order, this may change the effective permissions of users.</source>
        <translation>在列表中下移條目。由于條目自上而下被處理，這有可能改變用戶的有效權限。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Down</source>
        <translation>&amp;下移</translation>
    </message>
    <message>
        <location/>
        <source>Add new entry</source>
        <translation>添加新條目</translation>
    </message>
    <message>
        <location/>
        <source>This adds a new entry, initially set with no permissions and applying to all.</source>
        <translation>添加一個新條目，初始設置為無權限并應用到所有。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Add</source>
        <translation>&amp;添加</translation>
    </message>
    <message>
        <location/>
        <source>Remove entry</source>
        <translation>刪除條目</translation>
    </message>
    <message>
        <location/>
        <source>This removes the currently selected entry.</source>
        <translation>刪除當前選中的條目。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Remove</source>
        <translation>&amp;刪除</translation>
    </message>
    <message>
        <location/>
        <source>Context</source>
        <translation>上下文</translation>
    </message>
    <message>
        <source>Entry should apply to this channel</source>
        <translation type="obsolete">應用到該頻道的條目</translation>
    </message>
    <message>
        <location/>
        <source>This makes the entry apply to this channel.</source>
        <translation>應用該條目到當前頻道。</translation>
    </message>
    <message>
        <location/>
        <source>Applies to this channel</source>
        <translation>應用到當前頻道</translation>
    </message>
    <message>
        <location/>
        <source>Entry should apply to sub-channels.</source>
        <translation>應用到子頻道的條目。</translation>
    </message>
    <message>
        <location/>
        <source>This makes the entry apply to sub-channels of this channel.</source>
        <translation>應用該條目到當前頻道的子頻道。</translation>
    </message>
    <message>
        <location/>
        <source>Applies to sub-channels</source>
        <translation>應用到子頻道</translation>
    </message>
    <message>
        <location/>
        <source>Permissions</source>
        <translation>權限</translation>
    </message>
    <message>
        <location/>
        <source>User/Group</source>
        <translation>用戶/群</translation>
    </message>
    <message>
        <location/>
        <source>Group this entry applies to</source>
        <translation>應用該條目的群</translation>
    </message>
    <message>
        <location/>
        <source>This controls which group of users this entry applies to.&lt;br /&gt;Note that the group is evaluated in the context of the channel the entry is used in. For example, the default ACL on the Root channel gives &lt;i&gt;Write&lt;/i&gt; permission to the &lt;i&gt;admin&lt;/i&gt; group. This entry, if inherited by a channel, will give a user write privileges if he belongs to the &lt;i&gt;admin&lt;/i&gt; group in that channel, even if he doesn&apos;t belong to the &lt;i&gt;admin&lt;/i&gt; group in the channel where the ACL originated.&lt;br /&gt;If a group name starts with a !, its membership is negated, and if it starts with a ~, it is evaluated in the channel the ACL was defined in, rather than the channel the ACL is active in. Order is important; &lt;i&gt;!~in&lt;/i&gt; is valid, but &lt;i&gt;~!in&lt;/i&gt; is not.&lt;br /&gt;A few special predefined groups are:&lt;br /&gt;&lt;b&gt;all&lt;/b&gt; - Everyone will match.&lt;br /&gt;&lt;b&gt;auth&lt;/b&gt; - All authenticated users will match.&lt;br /&gt;&lt;b&gt;sub,a,b,c&lt;/b&gt; - User currently in a sub-channel minimum &lt;i&gt;a&lt;/i&gt; common parents, and between &lt;i&gt;b&lt;/i&gt; and &lt;i&gt;c&lt;/i&gt; channels down the chain. See the website for more extensive documentation on this one.&lt;br /&gt;&lt;b&gt;in&lt;/b&gt; - Users currently in the channel will match (convenience for &lt;i&gt;sub,0,0,0&lt;/i&gt;).&lt;br /&gt;&lt;b&gt;out&lt;/b&gt; - Users outside the channel will match (convenience for &lt;i&gt;!sub,0,0,0&lt;/i&gt;).&lt;br /&gt;Note that an entry applies to either a user or a group, not both.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>User ID</source>
        <translation>用戶名</translation>
    </message>
    <message>
        <location/>
        <source>User this entry applies to</source>
        <translation>應用該條目的用戶</translation>
    </message>
    <message>
        <location/>
        <source>This controls which user this entry applies to. Just type in the user name and hit enter to query the server for a match.</source>
        <translation>控制該條目應用到哪個用戶。請輸入用戶名然后按回車來向服務器做匹配查詢。</translation>
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
        <translation>默認 ALSA 聲卡</translation>
    </message>
    <message>
        <location line="+191"/>
        <source>Opening chosen ALSA Input failed: %1</source>
        <translation>無法打開所選 ALSA 輸入: %1</translation>
    </message>
</context>
<context>
    <name>ALSAAudioOutput</name>
    <message>
        <location line="-190"/>
        <source>Default ALSA Card</source>
        <translation>默認 ALSA 聲卡</translation>
    </message>
    <message>
        <location line="+325"/>
        <source>Opening chosen ALSA Output failed: %1</source>
        <translation>無法打開所選 ALSA 輸出: %1</translation>
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
        <translation type="obsolete">%1 毫秒 -&gt; %2 毫秒 （%3 毫秒 分辨率） %4赫茲</translation>
    </message>
    <message>
        <source>%1 ms -&gt; %2 ms (%3 ms resolution) %4Hz -- Unusable</source>
        <translation type="obsolete">%1 毫秒 -&gt; %2 毫秒 （%3 毫秒 分辨率） %4 赫茲--不可用</translation>
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
        <translation>ASIO初始化失敗: %1</translation>
    </message>
    <message>
        <location line="-21"/>
        <location line="+25"/>
        <source>Failed to instantiate ASIO driver</source>
        <translation>無法初始化 ASIO 驅動</translation>
    </message>
    <message>
        <location line="+37"/>
        <source>ASIO</source>
        <translation></translation>
    </message>
    <message>
        <location filename="ASIOInput.ui"/>
        <source>Form</source>
        <translation>表單</translation>
    </message>
    <message>
        <location/>
        <source>Device selection</source>
        <translation>設備選擇</translation>
    </message>
    <message>
        <location/>
        <source>Device</source>
        <translation>設備</translation>
    </message>
    <message>
        <location/>
        <source>Device to use for microphone</source>
        <translation>用作麥克風的設備</translation>
    </message>
    <message>
        <location/>
        <source>This chooses what device to query. You still need to actually query the device and select which channels to use.</source>
        <translation>選擇要查詢的設備。你仍需要實際查詢該設備然后選擇要使用哪些聲道。</translation>
    </message>
    <message>
        <location/>
        <source>Query selected device</source>
        <translation>查詢所選設備</translation>
    </message>
    <message>
        <location/>
        <source>This queries the selected device for channels. Be aware that many ASIO drivers are buggy to the extreme, and querying them might cause a crash of either the application or the system.</source>
        <translation>查詢所選設備的聲道。注意大多數 ASIO 驅動有極端漏洞，查詢設備可能導致應用程序或系統崩潰。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Query</source>
        <translation>&amp;查詢</translation>
    </message>
    <message>
        <location/>
        <source>Configure selected device</source>
        <translation>配置所選設備</translation>
    </message>
    <message>
        <location/>
        <source>This configures the selected device. Be aware that many ASIO drivers are buggy to the extreme, and querying them might cause a crash of either the application or the system.</source>
        <translation>配置所選設備。注意大多數 ASIO 驅動有極端漏洞，查詢設備可能導致應用程序或系統崩潰。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Configure</source>
        <translation>&amp;配置</translation>
    </message>
    <message>
        <location/>
        <source>Capabilities</source>
        <translation>設置信息</translation>
    </message>
    <message>
        <location/>
        <source>Driver name</source>
        <translation>驅動名稱</translation>
    </message>
    <message>
        <source>TextLabel</source>
        <translation type="obsolete">文本標簽</translation>
    </message>
    <message>
        <source>Buffersize</source>
        <translation type="obsolete">緩沖區大小</translation>
    </message>
    <message>
        <location/>
        <source>Configure input channels</source>
        <translation>配置輸入聲道</translation>
    </message>
    <message>
        <source>This will configure the input channels for ASIO. Make sure you select at least one channel as microphone and speaker. &lt;i&gt;Microphone&lt;/i&gt; should be where your microphone is attached, and &lt;i&gt;Speaker&lt;/i&gt; should be a channel that samples &lt;i&gt;What you hear&lt;/i&gt;.&lt;br /&gt;For example, on the Audigy 2 ZS, a good selection for Microphone would be &lt;i&gt;Mic L&lt;/i&gt; while Speaker should be &lt;i&gt;Mix L&lt;/i&gt; and &lt;i&gt;Mix R&lt;/i&gt;.</source>
        <translation type="obsolete">配置 ASIO 的輸入聲道.請確保你選擇了至少一個聲道作為麥克風和揚聲器。&lt;i&gt;麥克風&lt;/i&gt;應該是你的已連接的麥克風連，&lt;i&gt;揚聲器&lt;/i&gt;應該是一個采樣&lt;i&gt;你聽到的聲音&lt;/i&gt;的聲道。&lt;br /&gt;比如在 Audigy 2 ZS 上，一個好的麥克風的選擇會是&lt;i&gt; Mic L &lt;/i&gt;而揚聲器會是&lt;i&gt; Mix L &lt;/i&gt;和&lt;i&gt; Mix R&lt;/i&gt;.</translation>
    </message>
    <message>
        <location/>
        <source>Channels</source>
        <translation>聲道</translation>
    </message>
    <message>
        <location/>
        <source>Microphone</source>
        <translation>麥克風</translation>
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
        <translation>揚聲器</translation>
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
        <translation>你需要選擇至少一個麥克風和一個揚聲器源來使用 ASIO.如果你只需要使用要麥克風進行采樣,請使用 DirectSound.</translation>
    </message>
    <message>
        <location line="+103"/>
        <source>Opening selected ASIO device failed. No input will be done.</source>
        <translation>無法打開所選 ASIO 設備。沒有可用輸入。</translation>
    </message>
</context>
<context>
    <name>AboutDialog</name>
    <message>
        <location filename="About.cpp" line="+37"/>
        <source>About Mumble</source>
        <translation>關于 Mumble</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>&lt;h3&gt;Mumble (%1)&lt;/h3&gt;&lt;p&gt;Copyright %3 Thorvald Natvig&lt;br /&gt;slicer@users.sourceforge.net&lt;/p&gt;&lt;p&gt;&lt;b&gt;A voice-chat utility for gamers&lt;/b&gt;&lt;/p&gt;&lt;p&gt;&lt;tt&gt;&lt;a href=&quot;%2&quot;&gt;%2&lt;/a&gt;&lt;/tt&gt;&lt;/p&gt;</source>
        <translation>&lt;h3&gt;Mumble (%1)&lt;/h3&gt;&lt;p&gt;版權所有 %3 Thorvald Natvig&lt;br /&gt;slicer@users.sourceforge.net&lt;/p&gt;&lt;p&gt;&lt;b&gt;一個游戲玩家的語音聊天工具&lt;/b&gt;&lt;/p&gt;&lt;p&gt;&lt;tt&gt;&lt;a href=&quot;%2&quot;&gt;%2&lt;/a&gt;&lt;/tt&gt;&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>&amp;About Mumble</source>
        <translation>&amp;關于 Mumble</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>&amp;License</source>
        <translation>&amp;授權</translation>
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
        <translation>關于 Speex</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>&lt;h3&gt;About Speex&lt;/h3&gt;&lt;p&gt;&lt;tt&gt;&lt;a href=&quot;%2&quot;&gt;%2&lt;/a&gt;&lt;/tt&gt;&lt;/p&gt;&lt;p&gt;This program uses Speex version %1&lt;/p&gt;&lt;p&gt;Speex is used for echo cancellation, noise&lt;br /&gt;filtering, voice activity detection and speech&lt;br /&gt;compression.&lt;/p&gt;</source>
        <translation>&lt;h3&gt;關于 Speex&lt;/h3&gt;&lt;p&gt;&lt;tt&gt;&lt;a href=&quot;%2&quot;&gt;%2&lt;/a&gt;&lt;/tt&gt;&lt;/p&gt;&lt;p&gt;該程序使用 Speex 版本 %1&lt;/p&gt;&lt;p&gt;Speex 被用于回聲消除，噪音&lt;br /&gt;過濾，語言活動檢測和語音&lt;br /&gt;壓縮。&lt;/p&gt;</translation>
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
        <translation>表單</translation>
    </message>
    <message>
        <location/>
        <source>Interface</source>
        <translation>接口</translation>
    </message>
    <message>
        <location/>
        <source>System</source>
        <translation>系統</translation>
    </message>
    <message>
        <location/>
        <source>Input method for audio</source>
        <translation>音頻輸入方法</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is the input method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation>&lt;b&gt;這是用于音頻的輸入方法。&lt;/b&gt;&lt;br /&gt;很有可能你想使用 DirectSound。</translation>
    </message>
    <message>
        <location/>
        <source>Device</source>
        <translation>設備</translation>
    </message>
    <message>
        <source>Output method for audio</source>
        <translation type="obsolete">音頻輸出方法</translation>
    </message>
    <message>
        <source>&lt;b&gt;This is the output method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation type="obsolete">&lt;b&gt;這是用于音頻輸出的方法。&lt;/b&gt;&lt;br /&gt;很有可能你想使用 DirectSound。</translation>
    </message>
    <message>
        <location/>
        <source>Cancel Echo</source>
        <translation>消除回聲</translation>
    </message>
    <message>
        <location/>
        <source>Transmission</source>
        <translation>傳送</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Transmit</source>
        <translation>&amp;傳送</translation>
    </message>
    <message>
        <location/>
        <source>When to transmit your speech</source>
        <translation>何時傳送你的語音</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets when speech should be transmitted.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;Continuous&lt;/i&gt; - All the time&lt;br /&gt;&lt;i&gt;Voice Activity&lt;/i&gt; - When you are speaking clearly.&lt;br /&gt;&lt;i&gt;Push To Talk&lt;/i&gt; - When you hold down the hotkey set under &lt;i&gt;Shortcuts&lt;/i&gt;.</source>
        <translation>&lt;b&gt;設置何時語音將被傳送。&lt;/b&gt;&lt;br /&gt;&lt;i&gt;連續&lt;/i&gt;一直&lt;br /&gt;&lt;i&gt;有語音活動&lt;/i&gt; 當你說話清晰時。&lt;br /&gt;&lt;i&gt;按鍵通話&lt;/i&gt; - 當你按住作為&lt;i&gt;快捷鍵&lt;/i&gt;的自定義按鍵組合時。</translation>
    </message>
    <message>
        <location/>
        <source>DoublePush Time</source>
        <translation>兩次按鍵時間間隔</translation>
    </message>
    <message>
        <location/>
        <source>TextLabel</source>
        <translation>文本標簽</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This enables the audio cues for push to talk.&lt;/b&gt;&lt;br /&gt;Setting this will give you a short audio beep when push to talk is pressed and released.</source>
        <translation>&lt;b&gt;啟用按鍵通話的音頻提示。&lt;/b&gt;&lt;br /&gt;當你按下或釋放按鍵通話快捷鍵時會聽到一聲短的蜂鳴聲。</translation>
    </message>
    <message>
        <location/>
        <source>PTT Audio cue</source>
        <translation>PTT 提示音</translation>
    </message>
    <message>
        <location/>
        <source>Use SNR based speech detection</source>
        <translation>使用基于 SNR 的語音偵測</translation>
    </message>
    <message>
        <location/>
        <source>Signal to Noise</source>
        <translation>信噪比</translation>
    </message>
    <message>
        <location/>
        <source>Use Amplitude based speech detection.</source>
        <translation>使用基于振幅的語音偵測。</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets speech detection to use Amplitude.&lt;/b&gt;&lt;br /&gt;In this mode, the raw strength of the input signal is used to detect speech.</source>
        <translation>&lt;b&gt;設置用振幅來做語音偵測。&lt;/b&gt;&lt;br /&gt;在此模式下，輸入信號的原始強度被用于語音偵測。</translation>
    </message>
    <message>
        <location/>
        <source>Amplitude</source>
        <translation>振幅</translation>
    </message>
    <message>
        <location/>
        <source>Voice &amp;Hold</source>
        <translation>&amp;暫停 發言</translation>
    </message>
    <message>
        <location/>
        <source>How long to keep transmitting after silence</source>
        <translation>無聲后多長時間內繼續傳送</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This selects how long after a perceived stop in speech transmission should continue.&lt;/b&gt;&lt;br /&gt;Set this higher if your voice breaks up when you speak (seen by a rapidly blinking voice icon next to your name).&lt;br /&gt;Only makes sense when used with Voice Activity transmission.</source>
        <translation>&lt;b&gt;設置發言暫停被感知到后多長時間內繼續傳送。&lt;/b&gt;如果你在講話時你的語音破碎（在你的名字旁邊會顯示一個快速閃爍的語音圖標），增大這個值。&lt;br /&gt;只有在使用檢測到語音活動時傳送時該選項才有用。</translation>
    </message>
    <message>
        <location/>
        <source>Silence Below</source>
        <translation>無聲 低于</translation>
    </message>
    <message>
        <location/>
        <source>Signal values below this counts as silence</source>
        <translation>信號值低于這個數值時視為無聲</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the trigger values for voice detection.&lt;/b&gt;&lt;br /&gt;Use this together with the Audio Statistics window to manually tune the trigger values for detecting speech. Input values below &quot;Silence Below&quot; always count as silence. Values above &quot;Speech Above&quot; always count as voice. Values in between will count as voice if you&apos;re already talking, but will not trigger a new detection.</source>
        <translation>&lt;b&gt;設置語音檢測的觸發值。&lt;/b&gt;&lt;br /&gt;與音頻統計窗口一起來使用該值以手工調整語音偵測的觸發值。輸入值低于&quot;無聲 低于“時總是會被視為無聲。值高于”語音 高于“時總是被視為語音。如果你正在講話，兩者之間的值會被視為語音，但是不會觸發一個新的偵測。</translation>
    </message>
    <message>
        <location/>
        <source>Speech Above</source>
        <translation>語言 高于</translation>
    </message>
    <message>
        <location/>
        <source>Signal values above this count as voice</source>
        <translation>信號值高于這個數值時視為語音</translation>
    </message>
    <message>
        <location/>
        <source>Compression</source>
        <translation>壓縮</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Quality</source>
        <translation>&amp;質量</translation>
    </message>
    <message>
        <location/>
        <source>Quality of compression (peak bandwidth)</source>
        <translation>壓縮質量(峰值帶寬）</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the quality of compression.&lt;/b&gt;&lt;br /&gt;This determines how much bandwidth Mumble is allowed to use for outgoing audio.</source>
        <translation>&lt;b&gt;設置壓縮質量。&lt;/b&gt;&lt;br /&gt;確定 Mumble 可用于發送音頻的帶寬大小。</translation>
    </message>
    <message>
        <location/>
        <source>Audio per packet</source>
        <translation>每個數據包的音頻幀數</translation>
    </message>
    <message>
        <location/>
        <source>How many audio frames to send per packet</source>
        <translation>每個數據包里發送多少個音頻幀</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This selects how many audio frames should be put in one packet.&lt;/b&gt;&lt;br /&gt;Increasing this will increase the latency of your voice, but will also reduce bandwidth requirements.</source>
        <translation>&lt;b&gt;選擇一個數據包里放多少個音頻幀。&lt;/b&gt;&lt;br /&gt;增加這個數值會增加你的語音延遲，但也會減少帶寬需求。</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This shows peak outgoing bandwidth used.&lt;/b&gt;&lt;br /&gt;This shows the peak amount of bandwidth sent out from your machine. Audio bitrate is the maximum bitrate (as we use VBR) for the audio data alone. Position is the bitrate used for positional information. Overhead is our framing and the IP packet headers (IP and UDP is 75% of this overhead).</source>
        <translation>&lt;b&gt;顯示已用的發送帶寬峰值。&lt;/b&gt;&lt;br /&gt;顯示從你的機器發出的峰值帶寬大小。音頻比特率是僅用于音頻數據的最大比特率(由于我們使用VBR)。位置是用于位置信息的比特率。系統開銷是我們的組幀和 IP 數據包頭(IP 和 UDP 是該系統開銷的75%)。</translation>
    </message>
    <message>
        <location/>
        <source>Audio Processing</source>
        <translation>音頻處理</translation>
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
        <translation>輸入聲音的最大放大率</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Maximum amplification of input.&lt;/b&gt;&lt;br /&gt;Mumble normalizes the input volume before compressing, and this sets how much it&apos;s allowed to amplify.&lt;br /&gt;The actual level is continually updated based on your current speech pattern, but it will never go above the level specified here.&lt;br /&gt;If the &lt;i&gt;Microphone loudness&lt;/i&gt; level of the audio statistics hover around 100%, you probably want to set this to 2.0 or so, but if, like most people, you are unable to reach 100%, set this to something much higher.&lt;br /&gt;Ideally, set it so &lt;i&gt;Microphone Loudness * Amplification Factor &gt;= 100&lt;/i&gt;, even when you&apos;re speaking really soft.&lt;br /&gt;&lt;br /&gt;Note that there is no harm in setting this to maximum, but Mumble will start picking up other conversations if you leave it to auto-tune to that level.</source>
        <translation>&lt;b&gt;輸入的最大放大率。&lt;/b&gt;&lt;br /&gt;Mumble 在壓縮前會規格化輸入音量，該選項設置音量被允許放大多少。&lt;br /&gt;實際等級將根據你的語音模式來持續更新，但是不會高于這里指定的等級。&lt;br /&gt;如果音頻統計的&lt;i&gt;麥克風響度&lt;/i&gt;等級在 100%左右，你很有可能想把該值設為大約 2.0，但是如果像大多數人一樣，你不能達到 100%，設置一個更高的值。&lt;br /&gt;理想狀態下，設置該值使&lt;i&gt;麥克風響度 * 放大率 &gt;= 100&lt;/i&gt;,甚至當你相當輕聲的講話時。&lt;br /&gt;&lt;br /&gt;注意設置該值為其最大值不會有任何副作用，但是如果你將該值設為自動調節到其最大值， Mumble 會開始拾取其他人的談話。</translation>
    </message>
    <message>
        <location/>
        <source>Current speech detection chance</source>
        <translation>當前語音偵測几率</translation>
    </message>
    <message>
        <location/>
        <source>Audible audio cue when push-to-talk is activated or deactivated</source>
        <translation>當按鍵通話激活或未激活時聽到的提示音頻</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets speech detection to use Signal to Noise ratio.&lt;/b&gt;&lt;br /&gt;In this mode, the input is analyzed for something resembling a clear signal, and the clarity of that signal is used to trigger speech detection.</source>
        <translation>&lt;b&gt;設置使用信噪比來進行語音偵測。&lt;/b&gt;&lt;br /&gt; 在此模式下，將從輸入中尋找并分析出一個近似清晰的信號，而這個信號的清晰度被用于觸發語音偵測。</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This shows the current speech detection settings.&lt;/b&gt;&lt;br /&gt;You can change the settings from the Settings dialog or from the Audio Wizard.</source>
        <translation>&lt;b&gt;顯示當前語音偵測設置。&lt;/b&gt;&lt;br /&gt;你能在設置對話框或音頻精靈里修改當前設置。</translation>
    </message>
    <message>
        <location/>
        <source>Idle AutoMute</source>
        <translation>空閑時自動靜音</translation>
    </message>
    <message>
        <location/>
        <source>How long to remain idle before auto-mute.</source>
        <translation>空閑多長時間后自動靜音。</translation>
    </message>
    <message>
        <location/>
        <source>This sets an idle timer. If the timer expires without any audio being sent to the server, you are muted.</source>
        <translation>設置空閑定時器。如果定時器過期時沒有任何音頻發送到服務器，你將被靜音。</translation>
    </message>
    <message>
        <location/>
        <source>Maximum bandwidth used for sending audio</source>
        <translation>用于發送音頻的最大帶寬</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the amount of noise suppression to apply.&lt;/b&gt;&lt;br /&gt;The higher this value, the more aggressively stationary noise will be supressed.</source>
        <translation>&lt;b&gt;設置應用于噪音抑制的數值大小.&lt;/b&gt;&lt;br /&gt;數值越高，穩態噪聲越會被積極的抑制。</translation>
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
        <translation>連續</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Voice Activity</source>
        <translation>語音活動</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Push To Talk</source>
        <translation>按鍵通話</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>Audio Input</source>
        <translation>音頻輸入</translation>
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
        <translation>關閉</translation>
    </message>
    <message>
        <location line="-22"/>
        <source>%1 s</source>
        <translation>%1 秒</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>-%1 dB</source>
        <translation>-%1 分貝</translation>
    </message>
    <message>
        <location line="+58"/>
        <source>%1kbit/s (Audio %2, Position %4, Overhead %3)</source>
        <translation>%1 千比特/秒 （音頻 %2, 位置 %4, 系統開銷 %3）</translation>
    </message>
    <message>
        <location line="-47"/>
        <source>%1 min</source>
        <translation>%1 分鐘</translation>
    </message>
</context>
<context>
    <name>AudioOutput</name>
    <message>
        <location filename="AudioOutput.ui"/>
        <source>Form</source>
        <translation>表單</translation>
    </message>
    <message>
        <location/>
        <source>Interface</source>
        <translation>接口</translation>
    </message>
    <message>
        <location/>
        <source>System</source>
        <translation>系統</translation>
    </message>
    <message>
        <source>Input method for audio</source>
        <translation type="obsolete">音頻輸入方法</translation>
    </message>
    <message>
        <source>&lt;b&gt;This is the input method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation type="obsolete">&lt;b&gt;這是用于音頻的輸入方法。&lt;/b&gt;&lt;br /&gt;很有可能你想使用 DirectSound。</translation>
    </message>
    <message>
        <location/>
        <source>Device</source>
        <translation>設備</translation>
    </message>
    <message>
        <location/>
        <source>Output method for audio</source>
        <translation>音頻輸出方法</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is the output method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation>&lt;b&gt;這是用于音頻輸出的方法。&lt;/b&gt;&lt;br /&gt;很有可能你想使用 DirectSound。</translation>
    </message>
    <message>
        <location/>
        <source>Positional Audio</source>
        <translation>定位音頻</translation>
    </message>
    <message>
        <location/>
        <source>Audio Output</source>
        <translation>音頻輸出</translation>
    </message>
    <message>
        <location/>
        <source>Default &amp;Jitter Buffer</source>
        <translation>默認 &amp;Jitter 緩沖區</translation>
    </message>
    <message>
        <location/>
        <source>Safety margin for jitter buffer</source>
        <translation>Jitter 緩沖區的安全邊界</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the minimum safety margin for the jitter buffer.&lt;/b&gt;&lt;br /&gt;All incoming audio is buffered, and the jitter buffer continually tries to push the buffer to the minimum sustainable by your network, so latency can be as low as possible. This sets the minimum buffer size to use. If the start of sentences you hear is very jittery, increase this value.</source>
        <translation>&lt;b&gt;設置 Jitter 緩沖區的最小安全邊界. &lt;/b&gt;&lt;br /&gt;所有收到的音頻都會被緩沖，而 Jitter 緩沖區不斷嘗試使該緩沖區保持能被你的網絡所允許的最小值, 從而使延遲盡可能的低。 設置使用的緩沖區大小的最小值。如果你聽到的句子的起始部分是顫音，請增加這個值。</translation>
    </message>
    <message>
        <location/>
        <source>TextLabel</source>
        <translation>文本標簽</translation>
    </message>
    <message>
        <location/>
        <source>Volume</source>
        <translation>音量</translation>
    </message>
    <message>
        <location/>
        <source>Volume of incoming speech</source>
        <translation>收到的語音的音量</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This adjusts the volume of incoming speech.&lt;/b&gt;&lt;br /&gt;Note that if you increase this beyond 100%, audio will be distorted.</source>
        <translation>&lt;b&gt;調節收到的語音的音量。&lt;/b&gt;&lt;br /&gt;注意如果你增加此值至超過 100%，聲音會失真。</translation>
    </message>
    <message>
        <location/>
        <source>Output Delay</source>
        <translation>輸出延時</translation>
    </message>
    <message>
        <location/>
        <source>Amount of data to buffer</source>
        <translation>緩沖數據大小</translation>
    </message>
    <message>
        <location/>
        <source>This sets the amount of data to prebuffer in the output buffer. Experiment with different values and set it to the lowest which doesn&apos;t cause rapid jitter in the sound.</source>
        <translation>設置輸出緩沖區里前置緩沖的數據大小。試驗不同值然后設置成不引起快速顫音的最低值.</translation>
    </message>
    <message>
        <location/>
        <source>The connected &quot;speakers&quot; are actually headphones.</source>
        <translation>已連接的“揚聲器”實際上是耳機.</translation>
    </message>
    <message>
        <location/>
        <source>This sets the minimum distance for sound calculations. The volume of other players&apos; speech will not decrease until they are at least this far away from you.</source>
        <translation>設置聲強計算的最小距離.當別的玩家離你小于這個距離時他的語音將不會減弱.</translation>
    </message>
    <message>
        <location/>
        <source>Factor for sound volume decrease</source>
        <translation>音量減弱因子</translation>
    </message>
    <message>
        <location/>
        <source>Bloom</source>
        <translation>聲音增強</translation>
    </message>
    <message>
        <location/>
        <source>Factor for sound volume increase</source>
        <translation>音量增強因子</translation>
    </message>
    <message>
        <location/>
        <source>How much should sound volume increase for sources that are really close?</source>
        <translation>當音源距離過近時音量該增強多少?</translation>
    </message>
    <message>
        <location/>
        <source>Checking this indicates that you don&apos;t have speakers connected, just headphones. This is important, as speakers are usually in front of you, while headphones are directly to your left/right.</source>
        <translation>選中此選項表明你沒有連接揚聲器，只有耳機。這很重要，因為揚聲器通常在你面前，而耳機直接在你左右。</translation>
    </message>
    <message>
        <location/>
        <source>Headphones</source>
        <translation>耳機</translation>
    </message>
    <message>
        <location/>
        <source>Minimum Distance</source>
        <translation>最小距離</translation>
    </message>
    <message>
        <location/>
        <source>Minimum distance to player before sound volume decreases</source>
        <translation>距離玩家至少多遠時聲音開始減弱</translation>
    </message>
    <message>
        <location/>
        <source>Maximum Distance</source>
        <translation>最大距離</translation>
    </message>
    <message>
        <location/>
        <source>Maximum distance, beyond which speech volume won&apos;t decrease</source>
        <translation>超過最大距離時音量將不再減弱</translation>
    </message>
    <message>
        <location/>
        <source>This sets the maximum distance for sound calculations. When farther away than this, other players&apos; speech volume will not decrease any further.</source>
        <translation>設置聲強計算最大距離。當超過此距離時，別的玩家的音量將不再減弱。</translation>
    </message>
    <message>
        <location/>
        <source>Minimum Volume</source>
        <translation>最小音量</translation>
    </message>
    <message>
        <location/>
        <source>What should the volume be at the maximum distance?</source>
        <translation>位于最大距離時的音量大小?</translation>
    </message>
    <message>
        <location/>
        <source>Loopback Test</source>
        <translation>回環測試</translation>
    </message>
    <message>
        <location/>
        <source>Delay Variance</source>
        <translation>延時方差</translation>
    </message>
    <message>
        <location/>
        <source>Variance in packet latency</source>
        <translation>數據包延時的方差</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the packet latency variance for loopback testing.&lt;/b&gt;&lt;br /&gt;Most audio paths contain some variable latency. This allows you set that variance for loopback mode testing. For example, if you set this to 15ms, this will emulate a network with 20-35ms ping latency or one with 80-95ms latency. Most domestic net connections have a variance of about 5ms</source>
        <oldsource>&lt;b&gt;This sets the packet latency variance for loopback testing&lt;/b&gt;&lt;br /&gt;Most audio paths contain some variable latency. This allows you set that variance for loopback mode testing. For example, if you set this to 15ms, this will emulate a network with 20-35ms ping latency or one with 80-95ms latency. Most domestic net connections have a variance of about 5ms</oldsource>
        <translation>&lt;b&gt;設置回環測試時的數據包延遲方差。&lt;/b&gt;&lt;br /&gt;大多數音頻路徑含有一定的延遲方差。該選項允許你設置回環測試模式時的方差。例如，如果你設置該值為 15毫秒，將模擬一個 20-35毫秒 ping 延遲或一個 80-95毫秒延遲的網絡。大多數國內網絡的延遲方差啊大概是5毫秒</translation>
    </message>
    <message>
        <location/>
        <source>Packet Loss</source>
        <translation>數據包丟失</translation>
    </message>
    <message>
        <location/>
        <source>Packet loss for loopback mode</source>
        <translation>回環模式的數據包丟失</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the packet loss for loopback mode.&lt;/b&gt;&lt;br /&gt;This will be the ratio of packets lost. Unless your outgoing bandwidth is peaked or there&apos;s something wrong with your network connection, this will be 0%</source>
        <translation>&lt;b&gt;設置回環模式時的數據包丟失。&lt;/b&gt;&lt;br /&gt;該值即為丟包率。除非你的出外的帶寬達到峰值或者你的網絡連接有問題，該值會是 0%</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Loopback</source>
        <translation>&amp;回環</translation>
    </message>
    <message>
        <location/>
        <source>Desired loopback mode</source>
        <translation>期望的回環模式</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This enables one of the loopback testmodes.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;None&lt;/i&gt; - Loopback disabled&lt;br /&gt;&lt;i&gt;Local&lt;/i&gt; - Emulate a local server.&lt;br /&gt;&lt;i&gt;Server&lt;/i&gt; - Request loopback from server.&lt;br /&gt;Please note than when loopback is enabled, no other players will hear your voice. This setting is not saved on application exit.</source>
        <translation>&lt;b&gt;該選項啟用某種回環測試模式。&lt;/b&gt;&lt;br /&gt;&lt;i&gt;無&lt;/i&gt; - 禁用回環&lt;br /&gt;&lt;i&gt;本地&lt;/i&gt; - 模擬一個本地服務器。&lt;br /&gt;&lt;i&gt;服務器&lt;/i&gt; - 請求來自服務器的回環。&lt;br /&gt;請注意當回環模式啟用時，別的玩家不會聽到你的聲音。當應用程序退出時該選項不會被存盤。</translation>
    </message>
    <message>
        <location/>
        <source>Other Applications</source>
        <translation>其他應用程序</translation>
    </message>
    <message>
        <location/>
        <source>Volume of other applications during speech</source>
        <translation>講話時別的應用程序的音量</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Decrease volume of other applications during speech.&lt;/b&gt;&lt;br /&gt;Mumble supports decreasing the volume of other applications during incoming speech. This sets the relative volume of other applications when others are talking.</source>
        <translation>&lt;b&gt;講話時減小其他應用程序的音量.&lt;/b&gt;&lt;br /&gt;Mumble 支持收到語音時減小其他應用程序的音量。這將設置當其他的人交談時其他應用程序的相對音量。</translation>
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
        <translation>無</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Local</source>
        <translation>本地</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Server</source>
        <translation>服務器</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Audio Output</source>
        <translation>音頻輸出</translation>
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
        <translation>音頻統計</translation>
    </message>
    <message>
        <location/>
        <source>Input Levels</source>
        <translation>輸入級別</translation>
    </message>
    <message>
        <location/>
        <source>Peak microphone level</source>
        <translation>麥克風級別峰值</translation>
    </message>
    <message>
        <location/>
        <source>Peak power in last frame</source>
        <translation>最后幀的峰值功率</translation>
    </message>
    <message>
        <location/>
        <source>This shows the peak power in the last frame (20 ms), and is the same measurement as you would usually find displayed as &quot;input power&quot;. Please disregard this and look at &lt;b&gt;Microphone power&lt;/b&gt; instead, which is much more steady and disregards outliers.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>TextLabel</source>
        <translation type="obsolete">文本標簽</translation>
    </message>
    <message>
        <location/>
        <source>Peak speaker level</source>
        <translation>揚聲器級別峰值</translation>
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
        <translation>靜輸入級別峰值</translation>
    </message>
    <message>
        <location/>
        <source>Signal Analysis</source>
        <translation>信號分析</translation>
    </message>
    <message>
        <location/>
        <source>Microphone power</source>
        <translation>麥克風功率</translation>
    </message>
    <message>
        <location/>
        <source>How close the current input level is to ideal</source>
        <translation>當前輸入級別離完美有多近</translation>
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
        <translation>麥克風的信噪音比</translation>
    </message>
    <message>
        <location/>
        <source>This is the Signal-To-Noise Ratio (SNR) of the microphone in the last frame (20 ms). It shows how much clearer the voice is compared to the noise.&lt;br /&gt;If this value is below 1.0, there&apos;s more noise than voice in the signal, and so quality is reduced.&lt;br /&gt;There is no upper limit to this value, but don&apos;t expect to see much above 40-50 without a sound studio.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Speech Probability</source>
        <translation>語音的概率</translation>
    </message>
    <message>
        <location/>
        <source>Probability of speech</source>
        <translation>語音的概率</translation>
    </message>
    <message>
        <location/>
        <source>This is the probability that the last frame (20 ms) was speech and not environment noise.&lt;br /&gt;Voice activity transmission depends on this being right. The trick with this is that the middle of a sentence is always detected as speech; the problem is the pauses between words and the start of speech. It&apos;s hard to distinguish a sigh from a word starting with &apos;h&apos;.&lt;br /&gt;If this is in bold font, it means Mumble is currently transmitting (if you&apos;re connected).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Configuration feedback</source>
        <translation>配置反饋</translation>
    </message>
    <message>
        <location/>
        <source>Current audio bitrate</source>
        <translation>當前音頻比特率</translation>
    </message>
    <message>
        <location/>
        <source>Bitrate of last frame</source>
        <translation>最后幀的比特率</translation>
    </message>
    <message>
        <location/>
        <source>DoublePush interval</source>
        <translation>兩次按鍵時間間隔</translation>
    </message>
    <message>
        <source>Time between last two PushToTalk presses</source>
        <translation type="obsolete">最后兩次按下按鍵通話鍵的時間間隔</translation>
    </message>
    <message>
        <location/>
        <source>Speech Detection</source>
        <translation>語音偵測</translation>
    </message>
    <message>
        <location/>
        <source>Current speech detection chance</source>
        <translation>當前語音偵測几率</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This shows the current speech detection settings.&lt;/b&gt;&lt;br /&gt;You can change the settings from the Settings dialog or from the Audio Wizard.</source>
        <translation>&lt;b&gt;顯示當前語音偵測設置。&lt;/b&gt;&lt;br /&gt;你能在設置對話框或音頻精靈里修改當前設置。</translation>
    </message>
    <message>
        <location/>
        <source>Signal and noise power spectrum</source>
        <translation>信號和噪音功率頻譜</translation>
    </message>
    <message>
        <location/>
        <source>Power spectrum of input signal and noise estimate</source>
        <translation>輸入信號和噪音估計的功率頻譜</translation>
    </message>
    <message>
        <location/>
        <source>This shows the power spectrum of the current input signal (red line) and the current noise estimate (filled blue).&lt;br /&gt;All amplitudes are multiplied by 30 to show the interesting parts (how much more signal than noise is present in each waveband).&lt;br /&gt;This is probably only of interest if you&apos;re trying to fine-tune noise conditions on your microphone. Under good conditions, there should be just a tiny flutter of blue at the bottom. If the blue is more than halfway up on the graph, you have a seriously noisy environment.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Echo Analysis</source>
        <translation>回聲分析</translation>
    </message>
    <message>
        <location/>
        <source>Weights of the echo canceller</source>
        <translation>回聲抵消器的影響</translation>
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
        <translation>音頻調節精靈</translation>
    </message>
    <message>
        <location line="+50"/>
        <source>Introduction</source>
        <translation>介紹</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Welcome to the Mumble Audio Wizard</source>
        <translation>歡迎使用 Mumble 音頻精靈</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>This is the audio tuning wizard for Mumble. This will help you correctly set the input levels of your sound card, and also set the correct parameters for sound processing in Mumble.</source>
        <translation>這是 Mumble 音頻調節精靈。這將幫你正確設置你的聲卡的輸入級別，并正確設置 Mumble 的聲音處理參數。</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Please be aware that as long as this wizard is active, audio will be looped locally to allow you to listen to it, and no audio will be sent to the server.</source>
        <translation>請注意當音頻精靈活躍時，音頻將被在本地環回以允許你聽到它，沒有音頻會被發送到服務器。</translation>
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
        <translation>恭喜。現在你可以准備好享受 Mumble 帶給你的丰富聲音體驗.</translation>
    </message>
    <message>
        <location line="+23"/>
        <source>Device selection</source>
        <translation>設備選擇</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Selecting the input and output device to use with Mumble.</source>
        <translation>選擇 Mumble 用來輸入和輸出的設備.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Input device</source>
        <translation>輸入設備</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>This is the device your microphone is connected to.</source>
        <translation>這是你的麥克風連接到的設備。</translation>
    </message>
    <message>
        <location line="+14"/>
        <location line="+52"/>
        <source>System</source>
        <translation>系統</translation>
    </message>
    <message>
        <location line="-49"/>
        <source>Input method for audio</source>
        <translation>音頻輸入方法</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>&lt;b&gt;This is the input method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation>&lt;b&gt;這是用于音頻的輸入方法。&lt;/b&gt;&lt;br /&gt;很有可能你想使用 DirectSound。</translation>
    </message>
    <message>
        <location line="+12"/>
        <location line="+52"/>
        <source>Device</source>
        <translation>設備</translation>
    </message>
    <message>
        <location line="-49"/>
        <source>Input device to use</source>
        <translation>使用的輸入設備</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>&lt;b&gt;Selects which sound card to use for audio input.&lt;/b&gt;</source>
        <translation>&lt;b&gt;選擇哪個聲卡用于音頻輸入。&lt;/b&gt;</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Use echo cancellation</source>
        <translation>使用回聲消除</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Cancel echo from headset or speakers.</source>
        <translation>消除來自耳機或揚聲器的回聲.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This enables echo cancellation of outgoing audio, which helps both on speakers and on headsets.</source>
        <translation>啟用送出音頻的回聲消除，這在揚聲器和耳機上都有幫助。</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Output device</source>
        <translation>輸出設備</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>This is the device your speakers or headphones are connected to.</source>
        <translation>這是你的揚聲器或耳機連接到的設備.</translation>
    </message>
    <message>
        <location line="+19"/>
        <source>Output method for audio</source>
        <translation>音頻輸出方法</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>&lt;b&gt;This is the Output method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation>&lt;b&gt;這是用于音頻輸出的方法。&lt;/b&gt;&lt;br /&gt;很有可能你想使用 DirectSound。</translation>
    </message>
    <message>
        <location line="+15"/>
        <source>Output device to use</source>
        <translation>使用的輸出設備</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>&lt;b&gt;Selects which sound card to use for audio Output.&lt;/b&gt;</source>
        <translation>&lt;b&gt;選擇哪個聲卡用于音頻輸出。&lt;/b&gt;</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Enable positional audio</source>
        <translation>啟用定位音頻</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Allows positioning of sound.</source>
        <translation>允許聲音定位.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This allows Mumble to use positional audio to place voices.</source>
        <translation>這允許 Mumble 使用定位音頻來放置語音。</translation>
    </message>
    <message>
        <location line="+17"/>
        <source>Positional Audio</source>
        <translation>定位音頻</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Adjusting attenuation of positional audio.</source>
        <translation>調節定位音頻的衰減量.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Mumble supports positional audio for some games, and will position the voice of other players relative to their position in game. Depending on their position, the volume of the voice will be changed between the speakers to simulate the direction and distance the other player is at. Such positioning depends on your speaker configuration being correct in your operating system, so a test is done here.</source>
        <translation>Mumble 支持在某些游戲里使用定位音頻，并能根據其他玩家在游戲中的位置來定位他們的發言。音量將會根據說話者的位置來改變以模擬他相對于其他玩家的方向和距離。定位音頻依賴于你的操作系統正確的配置了揚聲器，因此我們對揚聲器進行測試。</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>The graph below shows the position of &lt;font color=&quot;red&quot;&gt;you&lt;/font&gt;, the &lt;font color=&quot;yellow&quot;&gt;speakers&lt;/font&gt; and a &lt;font color=&quot;green&quot;&gt;moving sound source&lt;/font&gt; as if seen from above. You should hear the audio move between the channels.</source>
        <translation>下圖中顯示&lt;font color=&quot;red&quot;&gt;你&lt;/font&gt;的位置，&lt;font color=&quot;yellow&quot;&gt;揚聲器&lt;/font&gt;和一個&lt;font color=&quot;green&quot;&gt;移動的聲源&lt;/font&gt;。你應該聽到音頻在聲道間切換.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Use headphones</source>
        <translation>使用耳機</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Use headphones instead of speakers.</source>
        <translation>使用耳機而不是揚聲器.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This ignores the OS speaker configuration and configures the positioning for headphones instead.</source>
        <translation>這將忽略操作系統的揚聲器配置并為耳機配置音頻定位.</translation>
    </message>
    <message>
        <location line="+22"/>
        <source>Volume tuning</source>
        <translation>音量調節</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Tuning microphone hardware volume to optimal settings.</source>
        <translation>調節麥克風硬件音量到最佳設置。</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Open your sound control panel and go to the recording settings. Make sure the microphone is selected as active input with maximum recording volume. If there&apos;s an option to enable a &quot;Microphone boost&quot; make sure it&apos;s checked.</source>
        <translation>打開你的聲音控制面板然后打開錄音設置。確保麥克風被選為當前使用的輸入并使用最大錄音音量。如果有啟用“麥克風加強”選項，請確保它被選上。</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Speak loudly, as when you are annoyed or excited. Decrease the volume in the sound control panel until the bar below stays as high as possible in the blue and green but &lt;b&gt;not&lt;/b&gt; the red zone while you speak.</source>
        <translation>請大聲講話，就像你在憤怒或興奮時那樣。減小你的聲音控制面板里的音量直到下面的進度條在你講話時盡可能的停留在藍區和綠區，而&lt;b&gt;不是&lt;/b&gt;紅區里.</translation>
    </message>
    <message>
        <location line="+11"/>
        <source>Now talk softly, as you would when talking late at night and you don&apos;t want to disturb anyone. Adjust the slider below so that the bar moves into green when you talk, but stays blue while you&apos;re silent.</source>
        <translation>現在輕聲講話，就像你在深夜時不想打擾任何人一樣。調節下面的滑塊直到下面的進度條在你講話時進入綠區，而在你沉默時停留在藍區。</translation>
    </message>
    <message>
        <location line="+19"/>
        <source>Voice Activity Detection</source>
        <translation>語言活動檢測</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Letting Mumble figure out when you&apos;re talking and when you&apos;re silent.</source>
        <translation>讓 Mumble 分辨出你是在講話還是在沉默.</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>This will help Mumble figure out when you are talking. The first step is selecting which data value to use.</source>
        <translation>這將幫助 Mumble 知道你是在講話。第一步是選擇用哪個數據值。</translation>
    </message>
    <message>
        <location line="+26"/>
        <source>Raw amplitude from input</source>
        <translation>輸入的原始振幅</translation>
    </message>
    <message>
        <location line="-4"/>
        <source>Signal-To-Noise ratio</source>
        <translation>信噪比</translation>
    </message>
    <message>
        <location line="+27"/>
        <source>Next you need to adjust the following two sliders. The first few utterances you say should end up in the green area (definitive speech). While talking, you should stay inside the yellow (might be speech) and when you&apos;re not talking, everything should be in the red (definitively not speech).</source>
        <translation>下一步你需要調節下面兩個滑塊。你最初講几句話時進度條應該停留在綠區（確定是講話）。當你講話時，進度條應該停留在黃區（可能是講話）﹔當你沒講話時，進度條應該停留在紅區(明確沒在講話）.</translation>
    </message>
    <message>
        <location line="+24"/>
        <source>Last, you need to adjust the grace time. Mumble may detect this much non-speech and still keep transmitting before it breaks off. This allows you to catch your breath while speaking.</source>
        <translation>最后，你需要調節寬限時間.在寬限時間內 Mumble 能夠偵測到你沒在講話并繼續傳送，然后斷開連接。這允許你在講話時喘息。</translation>
    </message>
    <message>
        <location line="+28"/>
        <source>Device tuning</source>
        <translation>設備調節</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Changing hardware output delays to their minimum value.</source>
        <translation>修改硬件輸出延時到它們的最小值。</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>To keep latency to an absolute minium, it&apos;s important to buffer as little audio as possible on the soundcard. However, many soundcards report that they require a much smaller buffer than what they can actually work with, so the only way to set this value is to try and fail.</source>
        <translation>為了保証延時是絕對最小值，讓聲卡緩沖盡可能少的音頻非常重要。然而大多數聲卡聲稱它們需要比實際需求更小的緩沖區，因此設置這個值的唯一方式是去嘗試。</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>You should hear a single tone that&apos;s changing in frequency. Change the slider below to the lowest value which gives &lt;b&gt;no&lt;/b&gt; interruptions or jitter in the sound. Please note that local echo is disabled during this test to improve audio path recognition.</source>
        <translation>你應該聽到一個不斷改變頻率的單音。移動下面的滑塊直到它是&lt;b&gt;不&lt;/b&gt;導致聲音中斷或顫音的最小值。請注意在測試中本地回聲將被禁止以改善音頻路徑識別.</translation>
    </message>
    <message>
        <location line="+17"/>
        <source>Amount of data to buffer.</source>
        <translation>緩沖數據大小。</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This sets the amount of data to prebuffer in the output buffer. Experiment with different values and set it to the lowest which doesn&apos;t cause rapid jitter in the sound.</source>
        <translation>設置輸出緩沖區里前置緩沖的數據大小。試驗不同值然后設置成不引起快速顫音的最低值.</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>If you position the microphone so it can pick up the speakers or headset, Mumble will measure the total audio path delay in your system; this means the delay from a sample is placed in an outbound buffer until it&apos;s found in a matching incoming buffer.</source>
        <translation>如果你將麥克風放到能捕捉揚聲器或耳機聲音的地方, Mumble 將計算你的系統中的總的音頻路徑延時;這個延時是自一個聲音樣本被放到輸出緩沖區后直到它在匹配的輸入緩沖區里被找到的時間間隔.</translation>
    </message>
    <message>
        <location line="+103"/>
        <source>%1ms</source>
        <translation>%1毫秒</translation>
    </message>
    <message>
        <location line="+191"/>
        <source>Audio path is %1ms long.</source>
        <translation>音頻路徑長%1毫秒.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Audio path cannot be determined. Input not recognized.</source>
        <translation>音頻路徑無法被確定。輸入無法識別.</translation>
    </message>
    <message>
        <location line="+51"/>
        <source>%1 s</source>
        <translation>%1 秒</translation>
    </message>
    <message>
        <location line="-708"/>
        <source>Mumble is under continuous development, and the development team wants to focus on the features that benefit the most users. To this end, Mumble supports submitting anonymous statistics about your configuration to the developers. These statistcs are essential for future development, and also make sure the features you use aren&apos;t deprecated.</source>
        <translation>Mumble 正處于開發階段，開發團隊致力于開發能使大多數用戶受益的功能。Mumble 支持匿名遞交你的配置統計信息給開發人員。這些統計信息對于將來的開發非常重要，并確保你使用的功能不會被棄用。</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Submit anonymous statistics to the Mumble project</source>
        <translation>遞交匿名統計給 Mumble 項目組</translation>
    </message>
    <message>
        <location line="+224"/>
        <source>Push To Talk:</source>
        <translation>按鍵通話:</translation>
    </message>
</context>
<context>
    <name>BanEditor</name>
    <message>
        <location filename="BanEditor.ui"/>
        <source>Mumble - Edit Bans</source>
        <translation>Mumble - 編輯黑名單</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Address</source>
        <translation>&amp;地址</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Mask</source>
        <translation>&amp;子網掩碼</translation>
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
        <translation>&amp;刪除</translation>
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
        <translation>無</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Traverse</source>
        <translation>遍歷</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Enter</source>
        <translation>進入</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Speak</source>
        <translation>發言</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>AltSpeak</source>
        <translation>試圖發言</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Mute/Deafen</source>
        <translation>禁音/耳聾</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Move/Kick</source>
        <translation>移動/踢出</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Make channel</source>
        <translation>創建頻道</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Link channel</source>
        <translation>鏈接頻道</translation>
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
        <translation>創建訪問控制列表</translation>
    </message>
</context>
<context>
    <name>ClientPlayer</name>
    <message>
        <location filename="Player.cpp" line="+53"/>
        <source>Authenticated</source>
        <translation>已認証</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Muted (server)</source>
        <translation>靜音(服務器)</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Deafened (server)</source>
        <translation>耳聾(服務器)</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Local Mute</source>
        <translation>本地靜音</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Muted (self)</source>
        <translation>靜音(自己）</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Deafened (self)</source>
        <translation>耳聾(自己)</translation>
    </message>
</context>
<context>
    <name>ConfigDialog</name>
    <message>
        <location filename="ConfigDialog.cpp" line="+54"/>
        <source>Accept changes</source>
        <translation>確定</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This button will accept current settings and return to the application.&lt;br /&gt;The settings will be stored to disk when you leave the application.</source>
        <translation>該按鈕將接受當前設置并返回到應用程序。&lt;br/&gt;當你退出應用程序時這些設置將被存盤。</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Reject changes</source>
        <translation>取消</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This button will reject all changes and return to the application.&lt;br /&gt;The settings will be reset to the previous positions.</source>
        <translation>該按鈕將拒絕所有修改并返回到應用程序。&lt;br/&gt;這些設置將被重置到先前的設定。</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Apply changes</source>
        <translation>應用</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This button will immediately apply all changes.</source>
        <translation>該按鈕將立即應用所有修改.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Undo changes for current page</source>
        <translation>撤銷</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This button will revert any changes done on the current page to the most recent applied settings.</source>
        <translation>該按鈕將復原對當前頁的任意修改到最近使用的設置。</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Restore defaults for current page</source>
        <translation>恢復默認值</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This button will restore the defaults for the settings on the current page. Other pages will not be changed.&lt;br /&gt;To restore all settings to their defaults, you will have to use this button on every page.</source>
        <oldsource>This button will restore the settings for the current page only to their defaults. Other pages will not be changed.&lt;br /&gt;To restore all settings to their defaults, you will have to use this button on every page.</oldsource>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This button will restore the settings for the current page only to their defaults. Other pages will be not be changed.&lt;br /&gt;To restore all settings to their defaults, you will have to use this button on every page.</source>
        <translation type="obsolete">該按鈕將恢復當前頁設置到默認值。其他頁面的設置將不被修改。&lt;b r/&gt;為了恢復其他頁面上的設置到默認值，你需要在每個頁面上都使用該按鈕。</translation>
    </message>
    <message>
        <location filename="ConfigDialog.ui"/>
        <source>Mumble Configuration</source>
        <translation>Mumble 配置</translation>
    </message>
    <message>
        <source>Tab 1</source>
        <translation type="obsolete">標簽 1</translation>
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
        <translation>正在連接到 %1</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Enter username</source>
        <translation>輸入用戶名</translation>
    </message>
    <message>
        <location line="+85"/>
        <source>Unknown</source>
        <translation>未知</translation>
    </message>
    <message>
        <location line="+14"/>
        <source>Failed to fetch server list</source>
        <translation>無法獲取服務器列表</translation>
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
        <translation>-未命名條目-</translation>
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
        <translation>Mumble 服務器連接</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Custom Servers</source>
        <translation>&amp;自定義服務器</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Label</source>
        <translation>&amp;標簽</translation>
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
        <translation>&amp;用戶名</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Password</source>
        <translation>&amp;密碼</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Connect</source>
        <translation>&amp;連接</translation>
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
        <translation>&amp;刪除</translation>
    </message>
    <message>
        <location/>
        <source>Server &amp;Browser</source>
        <translation>服務器&amp;瀏覽器</translation>
    </message>
    <message>
        <location/>
        <source>Label</source>
        <translation>標簽</translation>
    </message>
    <message>
        <location/>
        <source>Address</source>
        <translation>地址</translation>
    </message>
    <message>
        <location/>
        <source>URL</source>
        <translation>網址</translation>
    </message>
    <message>
        <location/>
        <source>Cancel</source>
        <translation>取消</translation>
    </message>
    <message>
        <location/>
        <source>C&amp;opy to custom</source>
        <translation>&amp;拷貝到自定義</translation>
    </message>
    <message>
        <location/>
        <source>&amp;View Webpage</source>
        <translation>&amp;查看主頁</translation>
    </message>
</context>
<context>
    <name>CoreAudioSystem</name>
    <message>
        <location filename="CoreAudio.cpp" line="+83"/>
        <source>Default Device</source>
        <translation type="unfinished">默認設備</translation>
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
        <translation>默認 DirectSound 語音輸入</translation>
    </message>
    <message>
        <location line="+461"/>
        <source>Opening chosen DirectSound Input device failed. No microphone capture will be done.</source>
        <translation>不能打開所選 DirectSound 輸入設備。不能捕捉麥克風。</translation>
    </message>
    <message>
        <location line="-79"/>
        <source>Opening chosen DirectSound Input failed. Default device will be used.</source>
        <translation>不能打開所選 DirectSound 輸入。將使用默認設備。</translation>
    </message>
    <message>
        <location line="+74"/>
        <source>Lost DirectSound input device.</source>
        <translation>丟失 DirectSound 輸入設備。</translation>
    </message>
</context>
<context>
    <name>DXAudioOutput</name>
    <message>
        <location line="-496"/>
        <source>Default DirectSound Voice Output</source>
        <translation>默認 DirectSound 語音輸出</translation>
    </message>
    <message>
        <location line="+247"/>
        <source>Opening chosen DirectSound Output failed. Default device will be used.</source>
        <translation>不能打開所選 DirectSound 輸出。將使用默認設備。</translation>
    </message>
    <message>
        <location line="+78"/>
        <source>Lost DirectSound output device.</source>
        <translation>丟失 DirectSound 輸出設備。</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Opening chosen DirectSound Output failed. No audio will be heard.</source>
        <translation>不能打開所選 DirectSound 輸出。將聽不到音頻。</translation>
    </message>
</context>
<context>
    <name>Database</name>
    <message>
        <location filename="Database.cpp" line="+127"/>
        <source>Mumble failed to initialize a database in any
of the possible locations.</source>
        <translation>Mumble 無法在任何可能的位置里初始化數據庫.</translation>
    </message>
</context>
<context>
    <name>GlobalShortcut</name>
    <message>
        <location filename="GlobalShortcut.ui"/>
        <source>Shortcuts</source>
        <translation>快捷鍵</translation>
    </message>
    <message>
        <location/>
        <source>List of configured shortcuts</source>
        <translation>配置好的快捷鍵列表</translation>
    </message>
    <message>
        <location/>
        <source>Function</source>
        <translation>功能</translation>
    </message>
    <message>
        <location/>
        <source>Shortcut</source>
        <translation>快捷鍵</translation>
    </message>
    <message>
        <location/>
        <source>Suppress</source>
        <translation>禁止</translation>
    </message>
    <message>
        <location/>
        <source>Add new shortcut</source>
        <translation>添加新快捷鍵</translation>
    </message>
    <message>
        <location/>
        <source>This will add a new global shortcut</source>
        <translation>這將添加一個新的全局快捷鍵</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Add</source>
        <translation>&amp;添加</translation>
    </message>
    <message>
        <location/>
        <source>Remove selected shortcut</source>
        <translation>移除所選快捷鍵</translation>
    </message>
    <message>
        <location/>
        <source>This will permanently remove a selected shortcut.</source>
        <translation>這將永久性的刪除所選快捷鍵。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Remove</source>
        <translation>&amp;刪除</translation>
    </message>
</context>
<context>
    <name>GlobalShortcutConfig</name>
    <message>
        <location filename="GlobalShortcut.cpp" line="+228"/>
        <source>Shortcuts</source>
        <translation>快捷鍵</translation>
    </message>
    <message>
        <location line="+33"/>
        <source>Shortcut button combination.</source>
        <translation>快捷鍵按鈕組合.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>&lt;b&gt;This is the global shortcut key combination.&lt;/b&gt;&lt;br /&gt;Double-click this field and then the desired key/button combo to rebind.</source>
        <translation>&lt;b&gt;這是全局快捷鍵組合。&lt;/b&gt;&lt;br /&gt;雙擊該區域然后綁定想要的按鍵/按鈕組合。</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Suppress keys from other applications</source>
        <translation>禁止其他應用程序使用的按鍵</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>&lt;b&gt;This hides the button presses from other applications.&lt;/b&gt;&lt;br /&gt;Enabling this will hide the button (or the last button of a multi-button combo) from other applications. Note that not all buttons can be suppressed.</source>
        <translation>&lt;b&gt;向其他應用程序隱藏這些按鍵。&lt;/b&gt;&lt;br/&gt;允許該選項將向其他應用程序隱藏指定按鍵（或者組合按鍵的最后一個按鍵）。注意并不是所有按鍵都能被禁用。</translation>
    </message>
</context>
<context>
    <name>GlobalShortcutMacInit</name>
    <message>
        <location filename="GlobalShortcut_macx.cpp" line="+63"/>
        <source>Mumble has detected that it is unable to receive Global Shortcut events when it is in the background.&lt;br /&gt;&lt;br /&gt;This is because the Universal Access feature called &apos;Enable access for assistive devices&apos; is currently disabled.&lt;br /&gt;&lt;br /&gt;Please &lt;a href=&quot; &quot;&gt;enable this setting&lt;/a&gt; and continue when done.</source>
        <translation>Mumble 檢測到當它位于后台時它不能接收到全局快捷鍵事件。&lt;br /&gt;&lt;br /&gt;這是因為被稱為‘允許輔助設備的使用’的輔助使用功能當前被禁用。&lt;br /&gt;&lt;br /&gt;請&lt;a href=&quot; &quot;&gt;啟用該選項&lt;/a&gt;然后繼續。</translation>
    </message>
</context>
<context>
    <name>GlobalShortcutX</name>
    <message>
        <location filename="GlobalShortcut_unix.cpp" line="+325"/>
        <source>Mouse %1</source>
        <translation>鼠標 %1</translation>
    </message>
</context>
<context>
    <name>LCD</name>
    <message>
        <location filename="LCD.cpp" line="+274"/>
        <source>Not connected</source>
        <translation>未連接</translation>
    </message>
</context>
<context>
    <name>LCDConfig</name>
    <message>
        <location line="-161"/>
        <source>Graphic</source>
        <translation>圖形</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Character</source>
        <translation>字符</translation>
    </message>
    <message>
        <location line="+11"/>
        <source>Enable this device</source>
        <translation>啟用該設備</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>LCD</source>
        <translation></translation>
    </message>
    <message>
        <location filename="LCD.ui"/>
        <source>Form</source>
        <translation>表單</translation>
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
        <translation>設備</translation>
    </message>
    <message>
        <location/>
        <source>Name</source>
        <translation>名稱</translation>
    </message>
    <message>
        <location/>
        <source>Type</source>
        <translation>類型</translation>
    </message>
    <message>
        <location/>
        <source>Size</source>
        <translation>大小</translation>
    </message>
    <message>
        <location/>
        <source>Enabled</source>
        <translation>已啟用</translation>
    </message>
    <message>
        <location/>
        <source>Views</source>
        <translation>視圖</translation>
    </message>
    <message>
        <location/>
        <source>Minimum Column Width</source>
        <translation>最小列寬</translation>
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
        <translation>文本標簽</translation>
    </message>
    <message>
        <location/>
        <source>Splitter Width</source>
        <translation>分割線寬度</translation>
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
        <translation type="unfinished">調試</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Critical</source>
        <translation type="unfinished">緊急</translation>
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
        <translation type="unfinished">服務器已連接</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Server Disconnected</source>
        <translation type="unfinished">服務器已斷開連接</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Player Joined Server</source>
        <translation type="unfinished">玩家加入服務器</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Player Left Server</source>
        <translation type="unfinished">玩家離開服務器</translation>
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
        <translation type="unfinished">你自我靜音/耳聾</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Other self-muted/deafened</source>
        <translation type="unfinished">其他人自我靜音/耳聾</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Player muted (you)</source>
        <translation type="unfinished">玩家對你靜音</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Player muted (by you)</source>
        <translation type="unfinished">玩家被你靜音</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Player muted (other)</source>
        <translation type="unfinished">玩家靜音其他人</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Player Joined Channel</source>
        <translation type="unfinished">玩家加入頻道</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Player Left Channel</source>
        <translation type="unfinished">玩家離開頻道</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Permission Denied</source>
        <translation type="unfinished">沒有權限</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Text Message</source>
        <translation type="unfinished">文本消息</translation>
    </message>
    <message>
        <source>Script Errors</source>
        <translation type="obsolete">腳本錯誤</translation>
    </message>
</context>
<context>
    <name>LogConfig</name>
    <message>
        <location line="-164"/>
        <source>Toggle console for %1 events</source>
        <translation>為 %1 事件啟用控制台</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Toggle pop-up notifications for %1 events</source>
        <translation>為 %1 事件啟用彈出窗口通知</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Toggle Text-To-Speech for %1 events</source>
        <translation>為%1事件啟用文本語音轉換</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Click here to toggle sound notification for %1 events</source>
        <translation>點擊這里為 %1 事件啟用聲音通知</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Path to soundfile used for sound notifications in the case of %1 events&lt;br /&gt;Single click to play&lt;br /&gt;Doubleclick to change</source>
        <translation>在 %1 事件發生時用于聲音通知的聲音文件的路徑&lt;br /&gt;單擊播放&lt;br/&gt;雙擊更改</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Click here to toggle console output for %1 events.&lt;br /&gt;If checked, this option makes Mumble output all %1 events in its message log.</source>
        <translation>點擊這里為 %1 事件啟用控制台輸出。&lt;br /&gt;如果選中，該選項使 Mumble 輸出所有 %1 事件到它的消息日志中。</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Click here to toggle pop-up notifications for %1 events.&lt;br /&gt;If checked, a notification pop-up will be created by Mumble for every %1 event.</source>
        <oldsource>Click here to toggle pop-up notifications for %1 events.&lt;br /&gt;If checked, a notification pop-up will be created by mumble for every %1 event.</oldsource>
        <translation type="unfinished">點擊這里為 %1 事件啟用彈出窗口通知。&lt;br /&gt;如果選中， Mumble 將為每個 %1 事件創建一個彈出窗口。</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Click here to toggle Text-To-Speech for %1 events.&lt;br /&gt;If checked, Mumble uses Text-To-Speech to read %1 events out loud to you. Text-To-Speech is also able to read the contents of the event which is not true for soundfiles. Text-To-Speech and soundfiles cannot be used at the same time.</source>
        <oldsource>Click here to toggle Text-To-Speech for %1 events.&lt;br /&gt;If checked, mumble uses Text-To-Speech to read %1 events out loud to you. Text-To-Speech is also able to read the contents of the event which is not true for soundfiles. Text-To-Speech and soundfiles cannot be used at the same time.</oldsource>
        <translation type="unfinished">點擊這里為 %1 事件啟用文本語音轉換.&lt;br /&gt;如果選中， Mumble 會使用文本語音轉化將 %1 事件大聲的讀出來給你聽。當聲音文件無效時，文本語音轉換也能讀取事件內容。文本語音轉化和聲音文件不能同時使用。</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Click here to toggle sound notification for %1 events.&lt;br /&gt;If checked, Mumble uses a soundfile predefined by you to indicate %1 events. Soundfiles and Text-To-Speech cannot be used at the same time.</source>
        <oldsource>Click here to toggle sound notification for %1 events.&lt;br /&gt;If checked, mumble uses a soundfile predefined by you to indicate %1 events. Soundfiles and Text-To-Speech cannot be used at the same time.</oldsource>
        <translation type="unfinished">點擊這里為 %1 事件啟用聲音通知。&lt;br /&gt;如果選中， Mumble 會使用一個你預先定義的聲音文件來標示 %1 事件。聲音文件和文本語音轉換不能同時被使用。</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Path to soundfile used for sound notifications in the case of %1 events.&lt;br /&gt;Single click to play&lt;br /&gt;Doubleclick to change&lt;br /&gt;Be sure that sound notifications for these events are enabled or this field will not have any effect.</source>
        <translation>在 %1 事件發生時用于聲音通知的聲音文件的路徑。&lt;br /&gt;單擊播放&lt;br /&gt;雙擊更換&lt;br /&gt;請確認這些事件的聲音通知被啟用，否則這個字段將沒有任何效果。</translation>
    </message>
    <message>
        <location line="+93"/>
        <source>Choose sound file</source>
        <translation>選擇聲音文件</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Invalid sound file</source>
        <translation>無效聲音文件</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The file &apos;%1&apos; does not exist or is not a valid speex file.</source>
        <translation>文件 ‘%1’ 不存在或者不是一個有效的語音編碼文件。</translation>
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
        <translation>文本語音轉換</translation>
    </message>
    <message>
        <location/>
        <source>Soundfile</source>
        <translation>聲音文件</translation>
    </message>
    <message>
        <location/>
        <source>Path</source>
        <translation>路徑</translation>
    </message>
    <message>
        <location/>
        <source>Text To Speech</source>
        <translation>文本語音轉換</translation>
    </message>
    <message>
        <location/>
        <source>Volume</source>
        <translation>音量</translation>
    </message>
    <message>
        <location/>
        <source>Volume of Text-To-Speech Engine</source>
        <translation>文本語音轉換引擎的音量</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is the volume used for the speech synthesis.&lt;/b&gt;</source>
        <translation>&lt;b&gt;這是用于語音合成的音量。&lt;/b&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Length threshold</source>
        <translation>長度閾值</translation>
    </message>
    <message>
        <location/>
        <source>Message length threshold for Text-To-Speech Engine</source>
        <translation>用于文本語音轉換的消息長度閾值</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is the length threshold used for the Text-To-Speech Engine.&lt;/b&gt;&lt;br /&gt;Messages longer than this limit will not be read aloud in their full length.</source>
        <translation>&lt;b&gt;這是用于文本語音轉換引擎的消息長度閾值。&lt;/b&gt;&lt;br /&gt;消息長于此值時將不會被全部讀出來。</translation>
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
        <translation>系統默認</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>None</source>
        <translation>無</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Only with players</source>
        <translation>僅當有玩家時</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>All</source>
        <translation>所有</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Ask</source>
        <translation>詢問</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Do Nothing</source>
        <translation>無動作</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Move</source>
        <translation>移動</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>User Interface</source>
        <translation>用戶界面</translation>
    </message>
    <message>
        <location line="+85"/>
        <source>Choose skin file</source>
        <translation>選擇皮膚文件</translation>
    </message>
    <message>
        <location filename="LookConfig.ui"/>
        <source>Form</source>
        <translation>表單</translation>
    </message>
    <message>
        <location/>
        <source>Language</source>
        <translation>語言</translation>
    </message>
    <message>
        <location/>
        <source>Language to use (requires restart)</source>
        <translation>使用的語言（需要重啟）</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets which language Mumble should use.&lt;/b&gt;&lt;br /&gt;You have to restart Mumble to use the new language.</source>
        <translation>&lt;b&gt;設置 Mumble 使用哪種語言。&lt;/b&gt;&lt;br /&gt;你需要重啟 Mumble 來使用新語言。</translation>
    </message>
    <message>
        <location/>
        <source>Look and Feel</source>
        <translation>外觀</translation>
    </message>
    <message>
        <location/>
        <source>Style</source>
        <translation>風格</translation>
    </message>
    <message>
        <location/>
        <source>Basic widget style</source>
        <translation>基本窗口風格</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the basic look and feel to use.&lt;/b&gt;</source>
        <translation>&lt;b&gt;設置使用基本外觀。&lt;/b&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Skin</source>
        <translation>皮膚</translation>
    </message>
    <message>
        <location/>
        <source>Skin file to use</source>
        <translation>使用的皮膚文件</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets which skin Mumble should use.&lt;/b&gt;&lt;br /&gt;The skin is a style file applied on top of the basic widget style. If there are icons in the same directory as the style sheet, those will replace the default icons.</source>
        <translation>&lt;b&gt;設置 Mumble 應該使用哪個皮膚文件。&lt;/b&gt;&lt;br/&gt;皮膚是一個應用在基本窗口風格上的風格文件。如果在與風格相同的目錄里有圖標，目錄中的圖標將替換默認圖標。</translation>
    </message>
    <message>
        <location/>
        <source>...</source>
        <translation></translation>
    </message>
    <message>
        <location/>
        <source>Expand</source>
        <translation>展開</translation>
    </message>
    <message>
        <location/>
        <source>When to automatically expand channels</source>
        <translation>什么時候自動展開頻道</translation>
    </message>
    <message>
        <location/>
        <source>This sets which channels to automatically expand. &lt;i&gt;None&lt;/i&gt; and &lt;i&gt;All&lt;/i&gt; will expand no or all channels, while &lt;i&gt;Only with players&lt;/i&gt; will expand and collapse channels as players join and leave them.</source>
        <translation>設置哪些頻道將自動展開。&lt;i&gt;無&lt;/i&gt;和&lt;i&gt;所有&lt;/i&gt;將展開零個或所有頻道，而&lt;i&gt;僅當有玩家時&lt;i&gt;將在玩家加入和離開時自動展開和折疊頻道。</translation>
    </message>
    <message>
        <location/>
        <source>List players above subchannels (requires restart).</source>
        <translation>在子頻道之上列出玩家（需要重啟）。</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;If set, players will be shown above subchannels in the channel view.&lt;/b&gt;&lt;br /&gt;A restart of Mumble is required to see the change.</source>
        <translation>&lt;b&gt;如果選中，玩家將被顯示在子頻道之上.&lt;/b&gt;&lt;br/&gt;需要重啟 Mumble 來使改變生效。</translation>
    </message>
    <message>
        <location/>
        <source>Players above Channels</source>
        <translation>玩家位于頻道之上</translation>
    </message>
    <message>
        <location/>
        <source>Channel Dragging</source>
        <translation>頻道拖動</translation>
    </message>
    <message>
        <location/>
        <source>This changes the behaviour when moving channels.</source>
        <translation>改變當移動頻道時的行為。</translation>
    </message>
    <message>
        <location/>
        <source>This sets the behaviour of channel drags, it can be used to prevent accidental dragging. &lt;i&gt;Move Channel&lt;/i&gt; moves the channel without prompting. &lt;i&gt;Do Nothing&lt;/i&gt; does nothing and prints an error message. &lt;i&gt;Ask&lt;/i&gt; uses a message box to confirm if you really wanted to move the channel.</source>
        <translation>設置頻道拖動時的行為，能被用來防止意外的拖動。&lt;i&gt;移動頻道&lt;/i&gt;將在移動頻道時不做提示.&lt;i&gt;無動作&lt;/i&gt;將在試圖移動頻道時顯示一個出錯信息。&lt;i&gt;詢問&lt;/i&gt;使用一個消息框來確認你是真的想移動一個頻道。</translation>
    </message>
    <message>
        <location/>
        <source>Ask whether to close or minimize when quitting Mumble.</source>
        <translation>詢問退出 Mumble 時是關閉還是最小化。</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;If set, will verify you want to quit if connected.&lt;b&gt;</source>
        <translation>&lt;b&gt;如果選中，當你連接到服務器時將驗証你是否想退出.&lt;/b&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Ask on quit while connected</source>
        <translation>當你連接到服務器時詢問是否退出</translation>
    </message>
    <message>
        <location/>
        <source>Make the Mumble window appear on top of other windows.</source>
        <translation>使 Mumble 窗口位于其他窗口之上.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This makes the Mumble window a topmost window.&lt;/b&gt;</source>
        <translation>&lt;b&gt;這將使 Mumble 窗口成為最上面的窗口。&lt;/b&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Always On Top</source>
        <translation>總在最上</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;If set, minimizing the Mumble main window will cause it to be hidden and accessible only from the tray. Otherwise, it will be minimized as a window normally would.&lt;/b&gt;</source>
        <translation>&lt;b&gt;如果選中，最小化 Mumble 主窗口時將使得它被隱藏到系統托盤。否則，它將會像通常一樣被最小化。&lt;/b&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Hide in tray when minimized</source>
        <translation>最小化時隱藏到系統托盤</translation>
    </message>
    <message>
        <source>Show all configuration items</source>
        <translation type="obsolete">顯示所有配置選項</translation>
    </message>
    <message>
        <source>&lt;b&gt;This will show all configuration items.&lt;/b&gt;&lt;br /&gt;Mumble contains a lot of configuration items that most users won&apos;t need to change. Checking this will show all configurable items.</source>
        <translation type="obsolete">&lt;b&gt;顯示所有配置選項.&lt;/b&gt;&lt;br/&gt; Mumble 包含許多大多數用戶不需要改變的選項。選中該選項將顯示所有可配置選項。</translation>
    </message>
    <message>
        <source>Expert Config</source>
        <translation type="obsolete">專家配置選項</translation>
    </message>
    <message>
        <location/>
        <source>Hide the main Mumble window in the tray when it is minimized.</source>
        <translation>當最小化時，隱藏 Mumble 主窗口到系統托盤。</translation>
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
        <translation>按鍵通話</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Push and hold this button to send voice.</source>
        <comment>Global Shortcut</comment>
        <translation>按住該按鈕來發送語音。</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This configures the push-to-talk button, and as long as you hold this button down, you will transmit voice.</source>
        <comment>Global Shortcut</comment>
        <translation>配置按鍵發音按鍵，只要你按住該按鍵，語音將被傳送。</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Reset Audio Processor</source>
        <comment>Global Shortcut</comment>
        <translation>重置音頻處理器</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Toggle Mute Self</source>
        <comment>Global Shortcut</comment>
        <translation>切換自我靜音</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Toggle self-mute status.</source>
        <comment>Global Shortcut</comment>
        <translation>切換自我靜音狀態。</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This will toggle your muted status. If you toggle this off, you will also disable self-deafen.</source>
        <comment>Global Shortcut</comment>
        <translation>該選項將切換自我靜音狀態。如果你關閉靜音狀態，將同時關閉自我耳聾狀態。</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Toggle Deafen Self</source>
        <comment>Global Shortcut</comment>
        <translation>切換自我耳聾</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Toggle self-deafen status.</source>
        <comment>Global Shortcut</comment>
        <translation>切換自我耳聾狀態。</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This will toggle your deafen status. If you toggle this on, you will also enable self-mute.</source>
        <comment>Global Shortcut</comment>
        <translation>該選項切換自我耳聾狀態。如果打開此選項，將同時啟用自我禁音。</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Unlink Plugin</source>
        <comment>Global Shortcut</comment>
        <translation>卸載插件</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Force Center Position</source>
        <comment>Global Shortcut</comment>
        <translation>強制居中</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Chan Parent</source>
        <comment>Global Shortcut</comment>
        <translation>父頻道</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Chan Sub#%1</source>
        <comment>Global Shortcut</comment>
        <translation>子頻道#%1</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Chan All Subs</source>
        <comment>Global Shortcut</comment>
        <translation>所有子頻道</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Push-to-Mute</source>
        <comment>Global Shortcut</comment>
        <translation>按鍵禁音</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Join Channel</source>
        <comment>Global Shortcut</comment>
        <translation>加入頻道</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Toggle Overlay</source>
        <comment>Global Shortcut</comment>
        <translation>切換 Overlay</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Toggle state of in-game overlay.</source>
        <comment>Global Shortcut</comment>
        <translation>切換游戲中的 overlay 狀態。</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This will switch the states of the ingame overlay between showing everybody, just the players who are talking, and nobody.</source>
        <comment>Global Shortcut</comment>
        <translation>在顯示所有人，只有發言的玩家和不顯示任何人之間切換游戲中的 overlay 狀態。</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Alt Push-to-Talk</source>
        <comment>Global Shortcut</comment>
        <translation>交替按鍵通話</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Toggle Minimal</source>
        <comment>Global Shortcut</comment>
        <translation>切換最小化</translation>
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
        <translation>減小音量（-10%）</translation>
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
        <translation>關閉</translation>
    </message>
    <message>
        <location line="-1"/>
        <source>Mumble is currently connected to a server. Do you want to Close or Minimize it?</source>
        <translation>Mumble 現在連接到了一個服務器。你想要關閉還是最小化它？</translation>
    </message>
    <message>
        <location line="+83"/>
        <source>Clear</source>
        <translation>清除</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Opening URL %1</source>
        <translation>打開網址 %1</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>URL scheme is not &apos;mumble&apos;</source>
        <translation>&apos;Mumble&apos; 不支持的網址方案</translation>
    </message>
    <message>
        <location line="+25"/>
        <source>Connecting to %1</source>
        <translation>正在連接到 %1</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Enter username</source>
        <translation>輸入用戶名</translation>
    </message>
    <message>
        <location line="+136"/>
        <source>Reconnecting.</source>
        <translation>正在重新連接中。</translation>
    </message>
    <message>
        <location line="+50"/>
        <source>&lt;h2&gt;Control channel&lt;/h2&gt;&lt;p&gt;Encrypted with %1 bit %2&lt;br /&gt;%3 ms average latency (%4 variance)&lt;/p&gt;</source>
        <translation>&lt;h2&gt;控制信道&lt;/h2&gt;&lt;p&gt;使用 %1 比特 %2 加密&lt;br /&gt;平均延遲 %3 毫秒（%4 方差）&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Voice channel is sent over control channel.</source>
        <translation>語音頻道通過控制信道發送。</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>&lt;h2&gt;Voice channel&lt;/h2&gt;&lt;p&gt;Encrypted with 128 bit OCB-AES128&lt;br /&gt;%1 ms average latency (%4 variance)&lt;/p&gt;</source>
        <translation>&lt;h2&gt;語音頻道&lt;/h2&gt;&lt;p&gt;使用 128 比特 OCB-AES128 加密&lt;br/&gt;%1 毫秒延遲（%4 方差）&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>UDP Statistics</source>
        <translation>UDP 統計</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>To Server</source>
        <translation>到服務器</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>From Server</source>
        <translation>來至服務器</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Good</source>
        <translation>良好</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Late</source>
        <translation>遲到</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Lost</source>
        <translation>丟失</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Resync</source>
        <translation>重新同步</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>&lt;h2&gt;Audio bandwidth&lt;/h2&gt;&lt;p&gt;Maximum %1 kbit/s&lt;br /&gt;Current %2 kbit/s (Quality %3)&lt;/p&gt;</source>
        <translation>&lt;h2&gt;音頻帶寬&lt;/h2&gt;&lt;p&gt;最大 %1千比特/秒&lt;br /&gt;當前 %2 千比特/秒 （質量 %3）&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Mumble Server Information</source>
        <translation>Mumble 服務器信息</translation>
    </message>
    <message>
        <location line="+4"/>
        <location line="+753"/>
        <source>&amp;View Certificate</source>
        <translation>&amp;查看認証</translation>
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
        <translation>輸入原因</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Banning player %1</source>
        <translation>封禁玩家 %1</translation>
    </message>
    <message>
        <location line="+22"/>
        <source>Sending message to %1</source>
        <translation>發送消息給 %1</translation>
    </message>
    <message>
        <location line="+11"/>
        <location line="+253"/>
        <source>To %1: %2</source>
        <translation>發送給 %1: %2</translation>
    </message>
    <message>
        <location line="-253"/>
        <location line="+253"/>
        <source>Message to %1</source>
        <translation>發送消息給 %1</translation>
    </message>
    <message>
        <location line="-157"/>
        <location line="+43"/>
        <source>Channel Name</source>
        <translation>頻道名</translation>
    </message>
    <message>
        <location line="-21"/>
        <source>Are you sure you want to delete %1 and all its sub-channels?</source>
        <translation>你確信要刪除 %1和它的子頻道嗎？</translation>
    </message>
    <message>
        <location line="+43"/>
        <source>Change description of channel %1</source>
        <translation>改變頻道 %1的描敘</translation>
    </message>
    <message>
        <location line="+79"/>
        <source>Sending message to channel %1</source>
        <translation>發送消息給頻道 %1</translation>
    </message>
    <message>
        <location line="+27"/>
        <source>Sending message to channel tree %1</source>
        <translation>發送消息給頻道樹 %1</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>To tree %1: %2</source>
        <translation>發送給樹 %1: %2</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Message to tree %1</source>
        <translation>發送消息給樹 %1</translation>
    </message>
    <message>
        <location line="+21"/>
        <source>Unmuted and undeafened.</source>
        <translation>解除靜音并解除耳聾。</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Unmuted.</source>
        <translation>解除靜音。</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Muted.</source>
        <translation>靜音。</translation>
    </message>
    <message>
        <location line="+20"/>
        <source>Muted and deafened.</source>
        <translation>禁音和耳聾.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Deafened.</source>
        <translation>耳聾.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Undeafened.</source>
        <translation>解除耳聾。</translation>
    </message>
    <message>
        <location line="+62"/>
        <source>About Qt</source>
        <translation>關于 Qt</translation>
    </message>
    <message>
        <location line="+97"/>
        <source>Joining %1.</source>
        <translation>加入 %1.</translation>
    </message>
    <message>
        <location line="+33"/>
        <source>Connected to server %1.</source>
        <translation>已連接到服務器 %1.</translation>
    </message>
    <message>
        <location line="+56"/>
        <source>SSL Verification failed: %1</source>
        <translation>SSL 認証失敗: %1</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>&lt;b&gt;WARNING:&lt;/b&gt; The server presented a certificate that was different from the stored one.</source>
        <translation>&lt;b&gt;警告:&lt;/b&gt;服務器提供的認証與保存的認証不同。</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Sever presented a certificate which failed verification.</source>
        <translation>服務器提供的認証認証失敗。</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>&lt;p&gt;%1.&lt;br /&gt;The specific errors with this certificate are: &lt;/p&gt;&lt;ol&gt;%2&lt;/ol&gt;&lt;p&gt;Do you wish to accept this certificate anyway?&lt;br /&gt;(It will also be stored so you won&apos;t be asked this again.)&lt;/p&gt;</source>
        <translation>&lt;p&gt;%1.&lt;br /&gt; 認証中的錯誤是：&lt;/p&gt;&lt;ol&gt;%2&lt;/ol&gt;&lt;p&gt;你希望接受認証嗎？&lt;br /&gt;（認証將被保存從而你不會被再次索要認証。）&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+28"/>
        <source>Server connection failed: %1.</source>
        <translation>連接服務器失敗: %1.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Disconnected from server.</source>
        <translation>與服務器斷開連接.</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Invalid username</source>
        <translation>無效用戶名</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>You connected with an invalid username, please try another one.</source>
        <translation>你使用一個無效的用戶名連接，請嘗試別外一個。</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>That username is already in use, please try another username.</source>
        <translation>用戶名已經被使用，請使用另外一個用戶名。</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Wrong password</source>
        <translation>密碼錯誤</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Wrong password for registered users, please try again.</source>
        <translation>注冊用戶密碼錯誤，請重試。</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Wrong server password for unregistered user account, please try again.</source>
        <translation>未注冊用戶的服務器密碼錯誤，請重試。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>&amp;Server</source>
        <translation>&amp;服務器</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Player</source>
        <translation>&amp;玩家</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Channel</source>
        <translation>&amp;頻道</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Audio</source>
        <translation>&amp;音頻</translation>
    </message>
    <message>
        <location/>
        <source>C&amp;onfigure</source>
        <translation>&amp;配置</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Help</source>
        <translation>&amp;幫助</translation>
    </message>
    <message>
        <location/>
        <source>Log</source>
        <translation>日志</translation>
    </message>
    <message>
        <location/>
        <source>This shows all recent activity. Connecting to servers, errors and information messages all show up here.&lt;br /&gt;To configure exactly which messages show up here, use the &lt;b&gt;Settings&lt;/b&gt; command from the menu.</source>
        <translation>這里顯示所有最近的活動。正在連接到服務器，錯誤和通知消息都將在這里顯示。&lt;br /&gt;要准確配置什么消息將被顯示在這里，請從菜單中使用&lt;b&gt;設置&lt;/b&gt;命令。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Quit Mumble</source>
        <translation>&amp;退出 Mumble</translation>
    </message>
    <message>
        <location/>
        <source>Closes the program</source>
        <translation>關閉程序</translation>
    </message>
    <message>
        <location/>
        <source>Exits the application.</source>
        <translation>退出應用程序。</translation>
    </message>
    <message>
        <location/>
        <source>Ctrl+Q</source>
        <translation></translation>
    </message>
    <message>
        <location/>
        <source>&amp;Connect</source>
        <translation>&amp;連接</translation>
    </message>
    <message>
        <location/>
        <source>Open the server connection dialog</source>
        <translation>打開服務器連接對話框</translation>
    </message>
    <message>
        <location/>
        <source>Shows a dialog of registered servers, and also allows quick connect.</source>
        <translation>顯示一個已注冊服務器的對話框，并允許快速連接。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Disconnect</source>
        <translation>&amp;斷開</translation>
    </message>
    <message>
        <location/>
        <source>Disconnect from server</source>
        <translation>從服務器斷開連接</translation>
    </message>
    <message>
        <location/>
        <source>Disconnects you from the server.</source>
        <translation>使你從服務器斷開連接。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Ban lists</source>
        <translation>&amp;黑名單</translation>
    </message>
    <message>
        <location/>
        <source>Edit ban lists on server</source>
        <translation>編輯服務器上的黑名單</translation>
    </message>
    <message>
        <location/>
        <source>This lets you edit the server-side IP ban lists.</source>
        <translation>這讓你編輯服務器端的網址黑名單。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Information</source>
        <translation>&amp;信息</translation>
    </message>
    <message>
        <location/>
        <source>Show information about the server connection</source>
        <translation>顯示服務器連接信息</translation>
    </message>
    <message>
        <location/>
        <source>This will show extended information about the connection to the server.</source>
        <translation>顯示服務器連接的擴展信息.</translation>
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
        <translation>從服務器踢出所選玩家。你將被要求指定一個原因。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Mute</source>
        <translation>&amp;靜音</translation>
    </message>
    <message>
        <location/>
        <source>Mute player</source>
        <translation>靜音玩家</translation>
    </message>
    <message>
        <location/>
        <source>Mute or unmute player on server. Unmuting a deafened player will also undeafen them.</source>
        <translation>在服務器上對玩家靜音或取消靜音。取消靜音一個耳聾的玩家將同時對他取消耳聾。</translation>
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
        <translation>從服務器踢出并封禁所選玩家。你將被要求指定一個原因。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Deafen</source>
        <translation>&amp;耳聾</translation>
    </message>
    <message>
        <location/>
        <source>Deafen player</source>
        <translation>耳聾玩家</translation>
    </message>
    <message>
        <location/>
        <source>Deafen or undeafen player on server. Deafening a player will also mute them.</source>
        <translation>在服務器上使玩家耳聾或取消耳聾。耳聾一個玩家將同時對他靜音。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Local Mute</source>
        <translation>&amp;本地靜音</translation>
    </message>
    <message>
        <location/>
        <source>Mute player locally</source>
        <translation>本地對玩家靜音</translation>
    </message>
    <message>
        <location/>
        <source>Mute or unmute player locally. Use this on other players in the same room.</source>
        <translation>本地對玩家靜音或者取消靜音. 對相同房間的其他玩家使用該選項。</translation>
    </message>
    <message>
        <location/>
        <source>Send Messa&amp;ge</source>
        <translation>發送&amp;消息</translation>
    </message>
    <message>
        <location/>
        <source>Send a Text Message</source>
        <translation>發送一條文本消息</translation>
    </message>
    <message>
        <location/>
        <source>Sends a text message to another user.</source>
        <translation>向另一個用戶發送一條文本消息。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Add</source>
        <translation>&amp;添加</translation>
    </message>
    <message>
        <location/>
        <source>Add new channel</source>
        <translation>添加一個新頻道</translation>
    </message>
    <message>
        <location/>
        <source>This adds a new sub-channel to the currently selected channel.</source>
        <translation>在當前所選頻道內添加一個子頻道。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Remove</source>
        <translation>&amp;刪除</translation>
    </message>
    <message>
        <location/>
        <source>Remove channel</source>
        <translation>刪除頻道</translation>
    </message>
    <message>
        <location/>
        <source>This removes a channel and all sub-channels.</source>
        <translation>刪除一個頻道及其所有子頻道。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Edit ACL</source>
        <translation>&amp;編輯 訪問控制列表</translation>
    </message>
    <message>
        <location/>
        <source>Edit Groups and ACL for channel</source>
        <translation>為頻道編輯群和訪問控制列表</translation>
    </message>
    <message>
        <location/>
        <source>This opens the Group and ACL dialog for the channel, to control permissions.</source>
        <translation>打開頻道的群和訪問控制列表對話框，來控制頻道的訪問權限。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Rename</source>
        <translation>&amp;重命名</translation>
    </message>
    <message>
        <location/>
        <source>Renames the channel</source>
        <translation>重命名頻道</translation>
    </message>
    <message>
        <location/>
        <source>This renames a channel.</source>
        <translation>重命名一個頻道。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Change Description</source>
        <translation>&amp;修改描敘</translation>
    </message>
    <message>
        <location/>
        <source>Changes the channel description</source>
        <translation>修改頻道描敘</translation>
    </message>
    <message>
        <location/>
        <source>This changes the description of a channel.</source>
        <translation>修改一個頻道的描敘。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Link</source>
        <translation>&amp;鏈接</translation>
    </message>
    <message>
        <location/>
        <source>Link your channel to another channel</source>
        <translation>鏈接你的頻道和另一個頻道</translation>
    </message>
    <message>
        <location/>
        <source>This links your current channel to the selected channel. If they have permission to speak in the other channel, players can now hear each other. This is a permanent link, and will last until manually unlinked or the server is restarted. Please see the shortcuts for push-to-link.</source>
        <translation>鏈接你的當前頻道和所選頻道。如果他們有權限在對方頻道講話，玩家現在能聽到彼此的聲音。這是個永久性的鏈接，直到手動取消鏈接或者服務器重啟。請查看按鍵鏈接快捷鍵。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Unlink</source>
        <comment>Channel</comment>
        <translation>&amp;取消鏈接</translation>
    </message>
    <message>
        <location/>
        <source>Unlink your channel from another channel</source>
        <translation>取消你的頻道和另一個頻道之間的鏈接</translation>
    </message>
    <message>
        <location/>
        <source>This unlinks your current channel from the selected channel.</source>
        <translation>取消你的當前頻道和所選頻道之間的鏈接。</translation>
    </message>
    <message>
        <location/>
        <source>Unlink &amp;All</source>
        <translation>取消&amp;所有鏈接</translation>
    </message>
    <message>
        <location/>
        <source>Unlinks your channel from all linked channels.</source>
        <translation>取消你的頻道和所有已鏈接頻道之間的鏈接。</translation>
    </message>
    <message>
        <location/>
        <source>This unlinks your current channel (not the selected one) from all linked channels.</source>
        <translation>取消你的當前頻道（非所選頻道）和所有已鏈接的頻道之間的鏈接。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Reset</source>
        <translation>&amp;重置</translation>
    </message>
    <message>
        <location/>
        <source>Reset audio preprocessor</source>
        <translation>重置音頻預處理器</translation>
    </message>
    <message>
        <location/>
        <source>This will reset the audio preprocessor, including noise cancellation, automatic gain and voice activity detection. If something suddenly worsens the audio environment (like dropping the microphone) and it was temporary, use this to avoid having to wait for the preprocessor to readjust.</source>
        <translation>該選項將重置音頻預處理器，包含噪音消除，自動獲取和語音活動檢測。如果音頻環境突然變的惡劣（比如掉麥）并且是暫時的，使用該選項來避免等待預處理器自我調節。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Mute Self</source>
        <translation>&amp;靜音自己</translation>
    </message>
    <message>
        <location/>
        <source>Mute yourself</source>
        <translation>自我靜音</translation>
    </message>
    <message>
        <location/>
        <source>Mute or unmute yourself. When muted, you will not send any data to the server. Unmuting while deafened will also undeafen.</source>
        <translation>對自己靜音或取消靜音。當靜音時，你將不會發送數據到服務器。當耳聾時取消靜音將同時取消耳聾。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Deafen Self</source>
        <translation>&amp;耳聾自己</translation>
    </message>
    <message>
        <location/>
        <source>Deafen yourself</source>
        <translation>自我耳聾</translation>
    </message>
    <message>
        <location/>
        <source>Deafen or undeafen yourself. When deafened, you will not hear anything. Deafening yourself will also mute.</source>
        <translation>使自己耳聾或取消耳聾。當耳聾時，你將聽不到聲音。耳聾自己將同時靜音自己。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Text-To-Speech</source>
        <translation>&amp;文本語音轉換</translation>
    </message>
    <message>
        <location/>
        <source>Toggle Text-To-Speech</source>
        <translation>切換文本語音轉換</translation>
    </message>
    <message>
        <location/>
        <source>Enable or disable the text-to-speech engine. Only messages enabled for TTS in the Configuration dialog will actually be spoken.</source>
        <translation>啟用或禁用文本語音轉換引擎。只有在配置對話框里被啟用了文本語音轉換的消息才會被讀出來。</translation>
    </message>
    <message>
        <location/>
        <source>S&amp;tatistics</source>
        <translation>&amp;統計</translation>
    </message>
    <message>
        <location/>
        <source>Display audio statistics</source>
        <translation>顯示音頻統計</translation>
    </message>
    <message>
        <location/>
        <source>Pops up a small dialog with information about your current audio input.</source>
        <translation>彈出一個關于你當前音頻輸入信息的對話框。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Unlink</source>
        <comment>Plugin</comment>
        <translation>&amp;卸載</translation>
    </message>
    <message>
        <location/>
        <source>Forcibly unlink plugin</source>
        <translation>強行卸載插件</translation>
    </message>
    <message>
        <location/>
        <source>This forces the current plugin to unlink, which is handy if it is reading completely wrong data.</source>
        <translation>強行卸載當前插件，當插件讀取的數據完全錯誤時相當有用。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Settings</source>
        <translation>&amp;設置</translation>
    </message>
    <message>
        <location/>
        <source>Configure Mumble</source>
        <translation>配置 Mumble</translation>
    </message>
    <message>
        <location/>
        <source>Allows you to change most settings for Mumble.</source>
        <translation>允許你改變 Mumble 的大多數設置。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Audio Wizard</source>
        <translation>&amp;音頻精靈</translation>
    </message>
    <message>
        <location/>
        <source>Start the audio configuration wizard</source>
        <translation>啟動音頻配置精靈</translation>
    </message>
    <message>
        <location/>
        <source>This will guide you through the process of configuring your audio hardware.</source>
        <translation>這將引導你進入配置你的音頻硬件的流程。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;What&apos;s This?</source>
        <translation>&amp;這是什么?</translation>
    </message>
    <message>
        <location/>
        <source>Enter What&apos;s This? mode</source>
        <translation>進入 這是什么? 模式</translation>
    </message>
    <message>
        <location/>
        <source>Click this to enter &quot;What&apos;s This?&quot; mode. Your cursor will turn into a question mark. Click on any button, menu choice or area to show a description of what it is.</source>
        <translation>點擊進入&quot;這是什么?&quot;模式。你的鼠標指針將變成一個問號。點擊任意按鈕，菜單或區域來顯示它是什么的描敘。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;About</source>
        <translation>&amp;關于</translation>
    </message>
    <message>
        <location/>
        <source>Information about Mumble</source>
        <translation>Mumble 的相關信息</translation>
    </message>
    <message>
        <location/>
        <source>Shows a small dialog with information and license for Mumble.</source>
        <translation>顯示一個 Mumble 相關信息和許可的對話框。</translation>
    </message>
    <message>
        <location/>
        <source>About &amp;Speex</source>
        <translation>關于 &amp;Speex</translation>
    </message>
    <message>
        <location/>
        <source>Information about Speex</source>
        <translation>Speex 的相關信息</translation>
    </message>
    <message>
        <location/>
        <source>Shows a small dialog with information about Speex.</source>
        <translation>顯示一個 Speex 相關信息的小對話框。</translation>
    </message>
    <message>
        <location/>
        <source>About &amp;Qt</source>
        <translation>關于 &amp;Qt</translation>
    </message>
    <message>
        <location/>
        <source>Information about Qt</source>
        <translation>Qt 相關信息</translation>
    </message>
    <message>
        <location/>
        <source>Shows a small dialog with information about Qt.</source>
        <translation>顯示一個 Qt 相關信息的小對話框。</translation>
    </message>
    <message>
        <location/>
        <source>Check for &amp;Updates</source>
        <translation>檢測&amp;更新</translation>
    </message>
    <message>
        <location/>
        <source>Check for new version of Mumble</source>
        <translation>檢測 Mumble 的新版本</translation>
    </message>
    <message>
        <location/>
        <source>Connects to the Mumble webpage to check if a new version is available, and notifies you with an appropriate download URL if this is the case.</source>
        <translation>連接到 Mumble 主頁來檢測是否有新版本可用，如果有新版本可用將通知你下載。</translation>
    </message>
    <message>
        <location/>
        <source>Sends a text message to all users in a channel.</source>
        <translation>向頻道內所有用戶發送文本信息。</translation>
    </message>
    <message>
        <location/>
        <source>Send &amp;Tree Message</source>
        <translation>發送信息到頻道&amp;樹</translation>
    </message>
    <message>
        <location/>
        <source>Send message to channel tree.</source>
        <translation>向頻道樹發送信息.</translation>
    </message>
    <message>
        <location/>
        <source>This sends a text message to a channel and it&apos;s subchannels.</source>
        <translation>向一個頻道及其所有子頻道發送文本消息。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Minimal View</source>
        <translation>&amp;簡化窗口</translation>
    </message>
    <message>
        <location/>
        <source>Toggle minimal window modes</source>
        <translation>啟用簡化窗口模式</translation>
    </message>
    <message>
        <location/>
        <source>This will toggle minimal mode, where the log window and menu is hidden.</source>
        <translation>這將啟用簡化窗口模式，該模式將隱藏日志窗口和菜單。</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="+56"/>
        <source>Joined server: %1.</source>
        <translation>加入服務器: %1.</translation>
    </message>
    <message>
        <location line="+20"/>
        <source>Left server: %1.</source>
        <translation>離開服務器: %1.</translation>
    </message>
    <message>
        <location line="+33"/>
        <source>%1 is now muted and deafened.</source>
        <translation>%1 現在被靜音和耳聾.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1 is now muted.</source>
        <translation>%1 現在被靜音。</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1 is now unmuted.</source>
        <translation>%1 現在被取消靜音。</translation>
    </message>
    <message>
        <location line="+13"/>
        <location line="+20"/>
        <source>server</source>
        <translation>服務器</translation>
    </message>
    <message>
        <location line="-17"/>
        <source>You were muted by %1.</source>
        <translation>你被 %1 靜音。</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>You were unmuted by %1.</source>
        <translation>你被 %1 取消靜音。</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>%1 muted by %2.</source>
        <translation>%1 被 %2 靜音。</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>%1 unmuted by %2.</source>
        <translation>%1 被 %2 取消靜音。</translation>
    </message>
    <message>
        <location line="+17"/>
        <source>You were deafened by %1.</source>
        <translation>你被 %1 耳聾。</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>You were undeafened by %1.</source>
        <translation>你被 %1 取消耳聾。</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>%1 deafened by %2.</source>
        <translation>%1 被 %2 耳聾。</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>%1 undeafened by %2.</source>
        <translation>%1 被 %2 取消耳聾。</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>You were kicked from the server by %1: %2.</source>
        <translation>你被 %1 從服務器踢出: %2。</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>%3 was kicked from the server by %1: %2.</source>
        <translation>%3 被 %1 從服務器踢出: %2。</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>You were kicked and banned from the server by %1: %2.</source>
        <translation>你被 %1 從服務器踢出并封禁: %2。</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>%3 was kicked and banned from the server by %1: %2.</source>
        <translation>%3 被 %1 從服務器踢出并封禁: %2。</translation>
    </message>
    <message>
        <location line="+27"/>
        <source>You were moved to %1 by %2.</source>
        <translation>你被從 %1 移動到 %2。</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>%1 left channel.</source>
        <translation>%1 離開頻道。</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1 moved out by %2 to %3.</source>
        <translation>%1 被 %2 移動到 %3。</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>%1 moved in by %2 from %3.</source>
        <translation>%1 被 %2 從 %3 移入。</translation>
    </message>
    <message>
        <location line="-2"/>
        <source>%1 entered channel.</source>
        <translation>%1 進入頻道。</translation>
    </message>
    <message>
        <location line="+100"/>
        <source>Server connection rejected: %1.</source>
        <translation>服務器拒絕連接: %1。</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Denied: %1.</source>
        <translation>拒絕: %1。</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Server maximum bandwidth is only %1 kbit/s. Quality auto-adjusted.</source>
        <translation>服務器最大帶寬是 %1 千比特/秒。自動調整質量。</translation>
    </message>
    <message>
        <location line="+11"/>
        <source>From %1: %2</source>
        <translation>發送自 %1: %2</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Message from %1</source>
        <translation>來至 %1 的消息</translation>
    </message>
    <message>
        <location filename="PlayerModel.cpp" line="+995"/>
        <source>You have Channel Dragging set to &quot;Do Nothing&quot; so the channel wasn&apos;t moved.</source>
        <translation>你設置頻道拖動為“無動作&quot;因此頻道沒有被移動。</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Unknown Channel Drag mode in PlayerModel::dropMimeData.</source>
        <translation>未知的頻道拖動模式 PlayerModel::dropMimeData。</translation>
    </message>
    <message>
        <location filename="main.cpp" line="+239"/>
        <source>Welcome to Mumble.</source>
        <translation>歡迎來到 Mumble。</translation>
    </message>
    <message>
        <location line="+17"/>
        <source>This is the first time you&apos;re starting Mumble.&lt;br /&gt;Would you like to go through the Audio Wizard to configure your soundcard?</source>
        <translation>這是你第一次啟動 Mumble.&lt;br /&gt;你想用音頻精靈配置聲卡嗎？</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="-2"/>
        <source>Server</source>
        <comment>message from</comment>
        <translation>服務器</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>Hide Frame</source>
        <translation>隱藏邊框</translation>
    </message>
    <message>
        <location/>
        <source>Toggle showing frame on minimal window</source>
        <translation>在簡化窗口模式下切換邊框顯示</translation>
    </message>
    <message>
        <location/>
        <source>This will toggle whether the minimal window should have a frame for moving and resizing.</source>
        <translation>該選項切換簡化窗口是否有個用來移動和改變大小的邊框。</translation>
    </message>
</context>
<context>
    <name>NetworkConfig</name>
    <message>
        <location filename="NetworkConfig.cpp" line="+51"/>
        <source>Network</source>
        <translation>網絡</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui"/>
        <source>Form</source>
        <translation>表單</translation>
    </message>
    <message>
        <location/>
        <source>Connection</source>
        <translation>連接</translation>
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
        <translation>&lt;b&gt;啟用 TCP 兼容模式&lt;/b&gt;。&lt;br/&gt;這將使 Mumble 與服務器通信時只使用 TCP。這將增加系統開銷并導致丟包從而使通信時出現明顯的暫停，因此只有在你不能使用默認模式時使用它（默認模式使用 UDP 來傳輸語音，TCP 來控制）。</translation>
    </message>
    <message>
        <location/>
        <source>Use TCP mode</source>
        <translation>使用 TCP 模式</translation>
    </message>
    <message>
        <location/>
        <source>Reconnect when disconnected</source>
        <translation>連接斷開后重新連接</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Reconnect when disconnected&lt;/b&gt;.&lt;br /&gt;This will make Mumble try to automatically reconnect after 10 seconds if your server connection fails.</source>
        <translation>&lt;b&gt;連接斷開后重新連接&lt;/b&gt;。&lt;br/&gt;這將使 Mumble 在與服務器的連接斷開 10秒后自動重新連接。</translation>
    </message>
    <message>
        <location/>
        <source>Reconnect automatically</source>
        <translation>自動重新連接</translation>
    </message>
    <message>
        <location/>
        <source>Proxy</source>
        <translation>代理</translation>
    </message>
    <message>
        <location/>
        <source>Type</source>
        <translation>類型</translation>
    </message>
    <message>
        <location/>
        <source>Type of proxy to connect through</source>
        <translation>連接通過的代理類型</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Type of proxy to connect through&lt;/b&gt;&lt;br /&gt;This makes Mumble connect through a proxy for all outgoing connections. Note: Proxy tunneling forces Mumble into TCP compatibility mode, causing all voice data to be sent via the control channel.</source>
        <oldsource>&lt;b&gt;Type of proxy to connect through&lt;/b&gt;&lt;br /&gt;This makes Mumble connect through a proxy for all outgoing connections. Note: Proxy tunneling forces Mumble into TCP compatability mode, causing all voice data to be sent via the control channel.</oldsource>
        <translation>&lt;b&gt;連接通過的代理類型&lt;/b&gt;&lt;br/&gt;這使 Mumble 通過代理來連接到所有出外的連接。注意: 代理隧道封裝會迫使 Mumble 進入 TCP 兼容模式，導致所有語音數據都通過控制信道發送。</translation>
    </message>
    <message>
        <location/>
        <source>Direct connection</source>
        <translation>直接連接</translation>
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
        <translation>主機名</translation>
    </message>
    <message>
        <location/>
        <source>Hostname of the proxy</source>
        <translation>代理主機名</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Hostname of the proxy.&lt;b&gt;&lt;br /&gt;This field specifies the hostname of the proxy you wish to tunnel network traffic through.</source>
        <translation>&lt;b&gt;代理主機名。&lt;/b&gt;&lt;br /&gt;該字段指定你希望用來穿隧傳輸網絡流量的代理主機名。</translation>
    </message>
    <message>
        <location/>
        <source>Port</source>
        <translation>端口</translation>
    </message>
    <message>
        <location/>
        <source>Port number of the proxy</source>
        <translation>代理端口號</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Port number of the proxy.&lt;/b&gt;&lt;br /&gt;This field specifies the port number that the proxy expects connections on.</source>
        <translation>&lt;b&gt;代理服務器端口號。&lt;/b&gt;&lt;br/&gt;該字段指定代理服務器使用的端口號。</translation>
    </message>
    <message>
        <location/>
        <source>Username</source>
        <translation>用戶名</translation>
    </message>
    <message>
        <location/>
        <source>Username for proxy authentication</source>
        <translation>用于代理認証的用戶名</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Username for proxy authentication.&lt;/b&gt;&lt;br /&gt;This specifies the username you use for authenticating yourself with the proxy. In case the proxy does not use authentication, or you want to connect anonymously, simply leave this field blank.</source>
        <translation>&lt;b&gt;用于代理認証的用戶名。&lt;/b&gt;&lt;br /&gt;該字段指定用于代理認証的用戶名。如果代理不需要認証，或者你想要使用匿名連接，請將此字段留白。</translation>
    </message>
    <message>
        <location/>
        <source>Password</source>
        <translation>密碼</translation>
    </message>
    <message>
        <location/>
        <source>Password for proxy authentication</source>
        <translation>用于代理認証的密碼</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Password for proxy authentication.&lt;/b&gt;&lt;br /&gt;This specifies the password you use for authenticating yourself with the proxy. In case the proxy does not use authentication, or you want to connect anonymously, simply leave this field blank.</source>
        <translation>&lt;b&gt;用于代理認証的密碼。&lt;/b&gt;&lt;br/&gt;該字段指定用于代理認証的密碼。如果代理不需要認証，或者你想要使用匿名連接，請將此字段留白。</translation>
    </message>
    <message>
        <location/>
        <source>Mumble services</source>
        <translation>Mumble 服務</translation>
    </message>
    <message>
        <location/>
        <source>Check for updates on startup</source>
        <translation>啟動時檢測更新</translation>
    </message>
    <message>
        <location/>
        <source>Submit anonymous statistics to the Mumble project</source>
        <translation>遞交匿名統計數據給 Mumble 項目</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Submit anonymous statistics.&lt;/b&gt;&lt;br /&gt;Mumble has a small development team, and as such needs to focus it&apos;s development where it&apos;s needed most. By submitting a bit of statistics you help the project determine where to focus development.</source>
        <translation>&lt;b&gt;遞交匿名統計數據。&lt;/b&gt;&lt;br /&gt;Mumble 有一個小的開發團隊，并需要專注于開發用戶最想要的功能。通過遞交匿名統計數據你將幫助開發團隊確定重點開發什么。</translation>
    </message>
    <message>
        <location/>
        <source>Submit anonymous statistics</source>
        <translation>遞交匿名統計數據</translation>
    </message>
</context>
<context>
    <name>Overlay</name>
    <message>
        <source>Failed to load overlay library. This means either that:
- the library (mumble_ol.dll) wasn&apos;t found in the directory you ran Mumble from
- you&apos;re on an OS earlier than WinXP SP2
- you do not have the June 2007 updated version of DX9.0c</source>
        <translation type="obsolete">無法加載 overlay 庫。這意味著：
- 庫（mumble_ol.dll）沒有在 Mumble 的運行目錄里被找到。
- 你的操作系統版本低于 WinXP SP2
- 你的 DX9.0C 版本低于 2007年6月升級版</translation>
    </message>
</context>
<context>
    <name>OverlayConfig</name>
    <message>
        <source>Show no one</source>
        <translation type="obsolete">不顯示任何人</translation>
    </message>
    <message>
        <source>Show only talking</source>
        <translation type="obsolete">當發言時顯示</translation>
    </message>
    <message>
        <source>Show everyone</source>
        <translation type="obsolete">顯示所有人</translation>
    </message>
    <message>
        <source>Form</source>
        <translation type="obsolete">表單</translation>
    </message>
    <message>
        <source>Options</source>
        <translation type="obsolete">選項</translation>
    </message>
    <message>
        <source>Enable overlay.</source>
        <translation type="obsolete">啟用 overlay .</translation>
    </message>
    <message>
        <source>This sets whether the overlay is enabled or not. This setting is only checked with D3D9 applications are started, so make sure Mumble is running and this option is on before you start the application.&lt;br /&gt;Please note that if you start the application after starting Mumble, or if you disable the overlay while running, there is no safe way to restart the overlay without also restarting the application.</source>
        <translation type="obsolete">設置是否啟用 overlay。該設置僅當應用程序已啟動時才會被檢查，因此確保 Mumble 正在運行并且該選項在你啟動應用程序之前已被打開。&lt;br /&gt;請注意如果你在啟動 Mumble 后啟動應用程序，或者你在運行時禁用 overlay，重新啟動 overlay 需要重新啟動應用程序。</translation>
    </message>
    <message>
        <source>Enable Overlay</source>
        <translation type="obsolete">啟用 Overlay</translation>
    </message>
    <message>
        <source>Who to show on the overlay</source>
        <translation type="obsolete">在 overlay 上顯示誰</translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets who to show in the in-game overlay.&lt;/b&gt;&lt;br /&gt;If many people are connected to the same channel, the overlay list might be very long. Use this to shorten it.&lt;br /&gt;&lt;i&gt;No one&lt;/i&gt; - Don&apos;t show anyone (but leave overlay running).&lt;br /&gt;&lt;i&gt;Only talking&lt;/i&gt; - Only show talking people.&lt;br /&gt;&lt;i&gt;Everyone&lt;/i&gt; - Show everyone.</source>
        <translation type="obsolete">&lt;b&gt;設置在游戲中的 overlay 內顯示誰。&lt;/b&gt;&lt;br /&gt;如果許多玩家已經連接到同一個頻道，overlay 列表可能會非常長。使用該選項來讓列表變短。&lt;br /&gt;&lt;i&gt;無人&lt;/i&gt; - 不顯示任何人（但是讓 overlay 運行).&lt;br /&gt;&lt;i&gt;僅當用戶發言時&lt;/i&gt; - 僅顯示正在發言的人。&lt;br /&gt;&lt;i&gt;所有人&lt;/i&gt; - 顯示所有人。</translation>
    </message>
    <message>
        <source>Always show yourself on overlay.</source>
        <translation type="obsolete">在 overlay 上總是顯示自己。</translation>
    </message>
    <message>
        <source>This sets whether to always show yourself or not. This setting is useful if you aren&apos;t showing everyone in the overlay, as then you would only see your own status if you were talking, which wouldn&apos;t let you see that you were deafened or muted.</source>
        <translation type="obsolete">設置是否總是顯示自己。該選項只有當你不在 overlay 中顯示所有人時才有用，你在講話時只能看到自己的狀態，你將看不到自己被禁音或耳聾。</translation>
    </message>
    <message>
        <source>Always Show Self</source>
        <translation type="obsolete">總是顯示自己</translation>
    </message>
    <message>
        <source>Show User custom textures instead of text on the overlay.</source>
        <translation type="obsolete">在 overlay 上顯示用戶自定義紋理而不是文本。</translation>
    </message>
    <message>
        <source>This sets whether to download and use custom textures for registered users. If disabled, the regular outline text will be used instead.</source>
        <translation type="obsolete">設置是否讓注冊用戶下載并使用自定義紋理。如果禁用該選項，常規邊框文字將被使用。</translation>
    </message>
    <message>
        <source>Show User Textures</source>
        <translation type="obsolete">顯示用戶紋理</translation>
    </message>
    <message>
        <source>Position</source>
        <translation type="obsolete">位置</translation>
    </message>
    <message>
        <source>Let overlay grow upwards</source>
        <translation type="obsolete">讓 overlay 往上增長</translation>
    </message>
    <message>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow upwards if needed.</source>
        <translation type="obsolete">Overlay 試著保持足夠小并固定在你選定的位置上。該選項允許 overlay 在需要時往上增長。</translation>
    </message>
    <message>
        <source>Grow Up</source>
        <translation type="obsolete">往上增長</translation>
    </message>
    <message>
        <source>Y-Position of Overlay</source>
        <translation type="obsolete">Overlay 的 Y-坐標</translation>
    </message>
    <message>
        <source>This sets the relative Y position of the overlay.</source>
        <translation type="obsolete">設置 overlay 的相對 Y 坐標。</translation>
    </message>
    <message>
        <source>Let overlay grow to the left</source>
        <translation type="obsolete">讓 overlay 往左增長</translation>
    </message>
    <message>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow to the left if needed.</source>
        <translation type="obsolete">Overlay 試著保持足夠小并固定在你選定的位置上。該選項允許 overlay 在需要時往左增長。</translation>
    </message>
    <message>
        <source>Grow Left</source>
        <translation type="obsolete">往左增長</translation>
    </message>
    <message>
        <source>Let overlay grow to the right</source>
        <translation type="obsolete">讓 overlay 往右增長</translation>
    </message>
    <message>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow to the right if needed.</source>
        <translation type="obsolete">Overlay 試著保持足夠小并固定在你選定的位置上。該選項允許 overlay 在需要時往右增長。</translation>
    </message>
    <message>
        <source>Grow Right</source>
        <translation type="obsolete">往右增長</translation>
    </message>
    <message>
        <source>Let overlay grow downwards</source>
        <translation type="obsolete">讓 overlay 往下增長</translation>
    </message>
    <message>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow downwards if needed.</source>
        <translation type="obsolete">Overlay 試著保持足夠小并固定在你選定的位置上。該選項允許 overlay 在需要時往下增長。</translation>
    </message>
    <message>
        <source>Grow Down</source>
        <translation type="obsolete">往下增長</translation>
    </message>
    <message>
        <source>X-Position of Overlay</source>
        <translation type="obsolete">Overlay 的 X-坐標</translation>
    </message>
    <message>
        <source>This sets the relative X position of the overlay.</source>
        <translation type="obsolete">設置 overlay 的相對 X 坐標。</translation>
    </message>
    <message>
        <source>Font</source>
        <translation type="obsolete">字體</translation>
    </message>
    <message>
        <source>Current Font</source>
        <translation type="obsolete">當前字體</translation>
    </message>
    <message>
        <source>TextLabel</source>
        <translation type="obsolete">文本標簽</translation>
    </message>
    <message>
        <source>Set Font</source>
        <translation type="obsolete">設置字體</translation>
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
        <translation type="obsolete">設置顯示的名字的最大高度，相對于屏幕高度來說。如果你的活躍 3D 窗口是 800像素高而該值被設成 5%，每個名字會是 40像素高。注意無論你在這里設置什么值名字都不會高于 60像素.</translation>
    </message>
    <message>
        <source>Color for players</source>
        <translation type="obsolete">玩家顏色</translation>
    </message>
    <message>
        <source>Change</source>
        <translation type="obsolete">更改</translation>
    </message>
    <message>
        <source>Color for talking players</source>
        <translation type="obsolete">正在發言玩家的顏色</translation>
    </message>
    <message>
        <source>Color for alt-talking players</source>
        <translation type="obsolete">試圖發言玩家的顏色</translation>
    </message>
    <message>
        <source>Color for Channels</source>
        <translation type="obsolete">頻道的顏色</translation>
    </message>
    <message>
        <source>Color for active Channels</source>
        <translation type="obsolete">活躍頻道的顏色</translation>
    </message>
</context>
<context>
    <name>PlayerModel</name>
    <message>
        <location filename="PlayerModel.cpp" line="-581"/>
        <source>This is a player connected to the server. The icon to the left of the player indicates whether or not they are talking:&lt;br /&gt;&lt;img src=&quot;skin:talking_on.png&quot; /&gt; Talking&lt;br /&gt;&lt;img src=&quot;skin:talking_off.png&quot; /&gt; Not talking</source>
        <translation>這是一個連接到服務器的玩家。玩家左邊的圖標顯示他是否在講話。&lt;br /&gt;&lt;img src=&quot;skin:talking_on.png&quot; /&gt;發言中&lt;br /&gt;&lt;img src=&quot;skin:talking_off.png&quot; /&gt; 沉默中</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>This is a channel on the server. Only players in the same channel can hear each other.</source>
        <translation>這是服務器上的一個頻道。只有同一頻道的玩家能聽到彼此的聲音。</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>This shows the flags the player has on the server, if any:&lt;br /&gt;&lt;img src=&quot;skin:authenticated.png&quot; /&gt;Authenticated user&lt;br /&gt;&lt;img src=&quot;skin:muted_self.png&quot; /&gt;Muted (by self)&lt;br /&gt;&lt;img src=&quot;skin:muted_server.png&quot; /&gt;Muted (by admin)&lt;br /&gt;&lt;img src=&quot;skin:deafened_self.png&quot; /&gt;Deafened (by self)&lt;br /&gt;&lt;img src=&quot;skin:deafened_server.png&quot; /&gt;Deafened (by admin)&lt;br /&gt;A player muted by himself is probably just away, talking on the phone or something like that.&lt;br /&gt;A player muted by an admin is probably also just away, and the noise the player is making was annoying enough that an admin muted him.</source>
        <translation>顯示該玩家在服務器上的標志:&lt;br /&gt;&lt;img src=&quot;skin:authenticated.png&quot; /&gt;已認証的用戶&lt;br /&gt;&lt;img src=&quot;skin:muted_self.png&quot; /&gt;靜音(被自己)&lt;br /&gt;&lt;img src=&quot;skin:muted_server.png&quot; /&gt;靜音(被管理員)&lt;br /&gt;&lt;img src=&quot;skin:deafened_self.png&quot; /&gt;耳聾 (被自己)&lt;br /&gt;&lt;img src=&quot;skin:deafened_server.png&quot; /&gt;耳聾 (被管理員)&lt;br /&gt;玩家被自己禁音有可能是他處于離開狀態，比如在接電話。&lt;br /&gt;玩家被管理員禁音也有可能是他處于離開狀態，但是他發出了讓管理員惱怒到禁音他的噪音。</translation>
    </message>
    <message>
        <location line="+24"/>
        <source>Name</source>
        <translation>名字</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Flags</source>
        <translation>標志</translation>
    </message>
    <message>
        <location line="+535"/>
        <source>Are you sure you want to drag this channel?</source>
        <translation>你確信要拖動這個頻道嗎？</translation>
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
        <translation>插件沒有配置選項。</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>Plugin has no about function.</source>
        <translation>插件沒有關于選項.</translation>
    </message>
    <message>
        <location filename="Plugins.ui"/>
        <source>Form</source>
        <translation>表單</translation>
    </message>
    <message>
        <location/>
        <source>Options</source>
        <translation>選項</translation>
    </message>
    <message>
        <location/>
        <source>Enable plugins and transmit positional information</source>
        <translation>啟用插件并傳送位置信息</translation>
    </message>
    <message>
        <location/>
        <source>This enables plugins for supported games to fetch your in-game position and transmit that with each voice packet. This enables other players to hear your voice in-game from the direction your character is in relation to their own.</source>
        <translation>對支持的游戲啟用插件以獲取你在游戲中的位置并在每個語音包中傳送。這將使別的玩家能根據你在游戲中的聲音來判斷你在游戲中相對于他們自己的方位。</translation>
    </message>
    <message>
        <location/>
        <source>Link to Game and Transmit Position</source>
        <translation>鏈接到游戲并傳送位置</translation>
    </message>
    <message>
        <location/>
        <source>Reloads all plugins</source>
        <translation>重載所有插件</translation>
    </message>
    <message>
        <location/>
        <source>This rescans and reloads plugins. Use this if you just added or changed a plugin to the plugins directory.</source>
        <translation>重新掃描和重載插件。當你添加或改變一個插件目錄中的插件時使用該選項。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Reload plugins</source>
        <translation>&amp;重載插件</translation>
    </message>
    <message>
        <location/>
        <source>Information about plugin</source>
        <translation>插件相關信息</translation>
    </message>
    <message>
        <location/>
        <source>This shows a small information message about the plugin.</source>
        <translation>顯示插件相關信息。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;About</source>
        <translation>&amp;關于</translation>
    </message>
    <message>
        <location/>
        <source>Show configuration page of plugin</source>
        <translation>顯示插件配置頁面</translation>
    </message>
    <message>
        <location/>
        <source>This shows the configuration page of the plugin, if any.</source>
        <translation>顯示插件配置頁面。</translation>
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
        <translation>%1 的鏈接丟失。</translation>
    </message>
    <message>
        <location line="+39"/>
        <source>%1 linked.</source>
        <translation>%1 已鏈接上。</translation>
    </message>
</context>
<context>
    <name>PortAudioSystem</name>
    <message>
        <location filename="PAAudio.cpp" line="+273"/>
        <source>Default Device</source>
        <translation>默認設備</translation>
    </message>
</context>
<context>
    <name>PulseAudioSystem</name>
    <message>
        <location filename="PulseAudio.cpp" line="+545"/>
        <source>Default Input</source>
        <translation>默認輸入</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Default Output</source>
        <translation>默認輸出</translation>
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
        <translation>輸入快捷鍵</translation>
    </message>
</context>
<context>
    <name>TextMessage</name>
    <message>
        <source>Send raw message</source>
        <translation type="obsolete">發送原始消息</translation>
    </message>
    <message>
        <source>Disables html formating</source>
        <translation type="obsolete">禁用 html 格式化</translation>
    </message>
    <message>
        <location filename="TextMessage.ui"/>
        <source>Message to send</source>
        <translation>要發送的消息</translation>
    </message>
    <message>
        <location/>
        <source>Preview</source>
        <translation>預覽</translation>
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
        <translation type="obsolete">Mumble 無法從 SouceForge 服務器獲取版本信息。</translation>
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
        <translation>認証鏈詳細信息</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Certificate chain</source>
        <translation>認証鏈</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>%1 %2</source>
        <translation></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Certificate details</source>
        <translation>認証詳細信息</translation>
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
        <translation>組織: %1</translation>
    </message>
    <message>
        <location line="-27"/>
        <source>Subunit: %1</source>
        <translation>部門: %1</translation>
    </message>
    <message>
        <location line="+1"/>
        <location line="+28"/>
        <source>Country: %1</source>
        <translation>國家: %1</translation>
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
        <translation>序列號: %1</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Public Key: %1 bits %2</source>
        <translation>公開密鑰: %1 比特 %2</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>RSA</source>
        <translation>非對稱加密算法</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>DSA</source>
        <translation>數字簽名算法</translation>
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
        <translation>發布者:</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Unit Name: %1</source>
        <translation>部門名: %1</translation>
    </message>
</context>
<context>
    <name>WASAPISystem</name>
    <message>
        <location filename="WASAPI.cpp" line="+180"/>
        <source>Default Device</source>
        <translation>默認設備</translation>
    </message>
</context>
</TS>
