<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.0" language="ja_JP">
<context>
    <name>ACLEditor</name>
    <message>
        <location filename="ACLEditor.cpp" line="+47"/>
        <source>Mumble - Edit ACL for %1</source>
        <translation>Mumble - %1のACLを編集</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui"/>
        <source>Active ACLs</source>
        <translation>ACLを有効にする</translation>
    </message>
    <message>
        <location/>
        <source>Context</source>
        <translation>コンテキスト</translation>
    </message>
    <message>
        <location/>
        <source>User/Group</source>
        <translation>ユーザ/グループ</translation>
    </message>
    <message>
        <location/>
        <source>Permissions</source>
        <translation>権限</translation>
    </message>
    <message>
        <location/>
        <source>Group</source>
        <translation>グループ</translation>
    </message>
    <message>
        <location/>
        <source>Members</source>
        <translation>メンバー</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Add</source>
        <translation>追加(&amp;A)</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Remove</source>
        <translation>削除(&amp;R)</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Up</source>
        <translation>上へ(&amp;U)</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Down</source>
        <translation>下へ(&amp;D)</translation>
    </message>
    <message>
        <location/>
        <source>Inherit ACLs</source>
        <translation>ACLを継承する</translation>
    </message>
    <message>
        <location/>
        <source>Applies to this channel</source>
        <translation>このチャンネルに適用する</translation>
    </message>
    <message>
        <location/>
        <source>Applies to sub-channels</source>
        <translation>サブチャンネルに適用する</translation>
    </message>
    <message>
        <location/>
        <source>User ID</source>
        <translation>ユーザID</translation>
    </message>
    <message>
        <location filename="ACLEditor.cpp" line="+4"/>
        <source>Deny</source>
        <translation>禁止</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Allow</source>
        <translation>許可</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui"/>
        <source>Remove</source>
        <translation>削除</translation>
    </message>
    <message>
        <location/>
        <source>Inherit</source>
        <translation>継承</translation>
    </message>
    <message>
        <location/>
        <source>Inheritable</source>
        <translation>継承可</translation>
    </message>
    <message>
        <location/>
        <source>Inherited</source>
        <translation>継承済み</translation>
    </message>
    <message>
        <location/>
        <source>Add</source>
        <translation>追加</translation>
    </message>
    <message>
        <location/>
        <source>Add to Remove</source>
        <translation>削除リストに追加</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Groups</source>
        <translation>グループ(&amp;G)</translation>
    </message>
    <message>
        <location/>
        <source>&amp;ACL</source>
        <translation>&amp;ACL</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>Accept changes</source>
        <translation type="obsolete">変更を確定</translation>
    </message>
    <message>
        <source>This button will accept current groups/ACLs and send them to the server. Note that if you mistakenly remove write permission from yourself, the server will add it.</source>
        <translation type="obsolete">このボタンは現在の グループとACLを 適用しサーバーに送信します。もし誤ってあなた自身の書き込み権限を削除してしまうとサーバーに追加されてしまいますのでご注意ください。</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">キャンセル(&amp;C)</translation>
    </message>
    <message>
        <source>Reject changes</source>
        <translation type="obsolete">変更を破棄</translation>
    </message>
    <message>
        <source>This button will cancel all changes and closes the dialog without updating the ACLs or groups on the server.</source>
        <translation type="obsolete">このボタンは全ての変更をキャンセルし、サーバーのACLやグループを変更せずにダイアログを閉じます。</translation>
    </message>
    <message>
        <location/>
        <source>List of entries</source>
        <translation>項目リスト</translation>
    </message>
    <message>
        <source>This shows all the entries active on this channel. Entries inherited from parent channels will be show in italics.</source>
        <translation type="obsolete">このチャンネルの全てのアクティブな項目を表示します。親チャンネルから継承される項目はイタリック(斜字)体で表示されます</translation>
    </message>
    <message>
        <location/>
        <source>Inherit ACL of parent?</source>
        <translation>親チャンネルのACLを継承しますか?</translation>
    </message>
    <message>
        <location/>
        <source>This sets whether or not the ACL up the chain of parent channels are applied to this object. Only those entries that are marked in the parent as &quot;Apply to sub-channels&quot; will be inherited.</source>
        <translation>これは親チャンネルのチェーンへのACLが適用されるか否かに関係なく設定されます。親の&quot;サブチャンネルに適用する&quot;にチェックされた項目だけが継承されます。</translation>
    </message>
    <message>
        <location/>
        <source>Add new entry</source>
        <translation>新しい項目を追加</translation>
    </message>
    <message>
        <location/>
        <source>This adds a new entry, initially set with no permissions and applying to all.</source>
        <translation>新たに項目を作成します。初期状態はパーミッションが無く、全てに適用されます。</translation>
    </message>
    <message>
        <location/>
        <source>Remove entry</source>
        <translation>項目を削除</translation>
    </message>
    <message>
        <location/>
        <source>This removes the currently selected entry.</source>
        <translation>選択されている項目を削除します。</translation>
    </message>
    <message>
        <location/>
        <source>Move entry up</source>
        <translation>項目を上に移動</translation>
    </message>
    <message>
        <location/>
        <source>This moves the entry up in the list. As entries are evaluated in order, this may change the effective permissions of users. You cannot move an entry above an inherited entry, if you really need that you&apos;ll have to duplicate the inherited entry.</source>
        <translation>リスト内の項目を上に動かします。項目が適切に評価されると、ユーザに影響する権限が変更される場合があります。継承した項目より上に動かすことは出来ません。もし本当に必要なら継承された項目を複製する必要があります。</translation>
    </message>
    <message>
        <location/>
        <source>Move entry down</source>
        <translation>項目を下に移動</translation>
    </message>
    <message>
        <location/>
        <source>This moves the entry down in the list. As entries are evaluated in order, this may change the effective permissions of users.</source>
        <translation>リスト内の項目を下に動かします。項目が適切に評価されると、ユーザに影響する権限が変更される場合があります。</translation>
    </message>
    <message>
        <location/>
        <source>Entry should apply to this channel.</source>
        <oldsource>Entry should apply to this channel</oldsource>
        <translation>入力項目はこのチャンネルに適用されます。</translation>
    </message>
    <message>
        <location/>
        <source>This makes the entry apply to this channel.</source>
        <translation>項目をこのチャンネルに適用します。</translation>
    </message>
    <message>
        <location/>
        <source>Entry should apply to sub-channels.</source>
        <translation>入力項目はサブチャンネルに適用されます。</translation>
    </message>
    <message>
        <location/>
        <source>This makes the entry apply to sub-channels of this channel.</source>
        <translation>項目をこのチャンネルのサブチャンネルに適用します。</translation>
    </message>
    <message>
        <location/>
        <source>Group this entry applies to</source>
        <translation>この項目を適用するグループ</translation>
    </message>
    <message>
        <location/>
        <source>This controls which group of users this entry applies to.&lt;br /&gt;Note that the group is evaluated in the context of the channel the entry is used in. For example, the default ACL on the Root channel gives &lt;i&gt;Write&lt;/i&gt; permission to the &lt;i&gt;admin&lt;/i&gt; group. This entry, if inherited by a channel, will give a user write privileges if he belongs to the &lt;i&gt;admin&lt;/i&gt; group in that channel, even if he doesn&apos;t belong to the &lt;i&gt;admin&lt;/i&gt; group in the channel where the ACL originated.&lt;br /&gt;If a group name starts with a !, its membership is negated, and if it starts with a ~, it is evaluated in the channel the ACL was defined in, rather than the channel the ACL is active in. Order is important; &lt;i&gt;!~in&lt;/i&gt; is valid, but &lt;i&gt;~!in&lt;/i&gt; is not.&lt;br /&gt;A few special predefined groups are:&lt;br /&gt;&lt;b&gt;all&lt;/b&gt; - Everyone will match.&lt;br /&gt;&lt;b&gt;auth&lt;/b&gt; - All authenticated users will match.&lt;br /&gt;&lt;b&gt;sub,a,b,c&lt;/b&gt; - User currently in a sub-channel minimum &lt;i&gt;a&lt;/i&gt; common parents, and between &lt;i&gt;b&lt;/i&gt; and &lt;i&gt;c&lt;/i&gt; channels down the chain. See the website for more extensive documentation on this one.&lt;br /&gt;&lt;b&gt;in&lt;/b&gt; - Users currently in the channel will match (convenience for &lt;i&gt;sub,0,0,0&lt;/i&gt;).&lt;br /&gt;&lt;b&gt;out&lt;/b&gt; - Users outside the channel will match (convenience for &lt;i&gt;!sub,0,0,0&lt;/i&gt;).&lt;br /&gt;Note that an entry applies to either a user or a group, not both.</source>
        <translation>この項目がどのユーザグループに適用されるかを制御します。</translation>
    </message>
    <message>
        <location/>
        <source>User this entry applies to</source>
        <translation>この項目を適用するユーザ</translation>
    </message>
    <message>
        <location/>
        <source>This controls which user this entry applies to. Just type in the user name and hit enter to query the server for a match.</source>
        <translation>この項目がどのユーザに適用されるかを制御します。検索するにはユーザ名を入力してEnterキーを押してください。</translation>
    </message>
    <message>
        <location filename="ACLEditor.cpp" line="+102"/>
        <source>Allow %1</source>
        <translation>%1を許可する</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Deny %1</source>
        <translation>%1を禁止する</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This grants the %1 privilege. If a privilege is both allowed and denied, it is denied.&lt;br /&gt;%2</source>
        <translation>%1に特権を付与します。特権の許可と禁止の両方が設定されている場合、禁止になります。&lt;br /&gt;%2</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This revokes the %1 privilege. If a privilege is both allowed and denied, it is denied.&lt;br /&gt;%2</source>
        <translation>%1から特権を剥奪します。特権の許可と禁止の両方が設定されている場合、禁止になります。&lt;br /&gt;%2</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui"/>
        <source>List of groups</source>
        <translation>グループリスト</translation>
    </message>
    <message>
        <location/>
        <source>This is all the groups currently defined for the channel. To create a new group, just type in the name and press enter.</source>
        <translation>チャンネルに定義されている全てのグループです。新たにグループを作成するには名前を入力してEnterを押してください。</translation>
    </message>
    <message>
        <location/>
        <source>Remove selected group</source>
        <translation>選択されたグループを削除</translation>
    </message>
    <message>
        <location/>
        <source>This removes the currently selected group. If the group was inherited, it will not be removed from the list, but all local information about the group will be cleared.</source>
        <translation>選択されたグループを削除します。もしグループが継承されていたらリストからは削除されませんが、グループに関するローカルな情報は消えてしまいます。</translation>
    </message>
    <message>
        <location/>
        <source>Inherit group members from parent</source>
        <translation>親からグループメンバーを継承する</translation>
    </message>
    <message>
        <location/>
        <source>This inherits all the members in the group from the parent, if the group is marked as &lt;i&gt;Inheritable&lt;/i&gt; in the parent channel.</source>
        <translation>親チャンネルでグループが継承可になっている場合、親グループから全てのメンバーを継承します。</translation>
    </message>
    <message>
        <location/>
        <source>Make group inheritable to sub-channels</source>
        <translation>サブチャンネルに対してグループを継承可能にする</translation>
    </message>
    <message>
        <location/>
        <source>This makes this group inheritable to sub-channels. If the group is non-inheritable, sub-channels are still free to create a new group with the same name.</source>
        <translation>サブチャンネルに対してグループを継承可能にします。もしグループが継承不可ならばサブチャンネルでは同じ名前の新しいグループを作成することが出来ます。</translation>
    </message>
    <message>
        <location/>
        <source>Group was inherited from parent channel</source>
        <translation>親チャンネルから継承したグループ</translation>
    </message>
    <message>
        <location/>
        <source>This indicates that the group was inherited from the parent channel. You cannot edit this flag, it&apos;s just for information.</source>
        <translation>グループが親チャンネルから継承されたことを示しています。このフラグは変更できません。これは単なる情報です。</translation>
    </message>
    <message>
        <location/>
        <source>Add member to group</source>
        <translation>グループにメンバーを追加</translation>
    </message>
    <message>
        <source>Type in the name of a player you wish to add to the group and press enter.</source>
        <translation type="obsolete">グループに追加したいプレイヤーの名前を入力してEnterキーを押してください。</translation>
    </message>
    <message>
        <location/>
        <source>Remove member from group</source>
        <translation>グループからメンバーを削除</translation>
    </message>
    <message>
        <source>Type in the name of a player you wish to remove from the group and press enter.</source>
        <translation type="obsolete">グループから削除したいプレイヤーの名前を入力してEnterキーを押してください。</translation>
    </message>
    <message>
        <location/>
        <source>Dialog</source>
        <translation>ダイアログ</translation>
    </message>
    <message>
        <location/>
        <source>Type in the name of a player you wish to add to the group and click Add.</source>
        <translation>追加したいプレイヤー名を入力して追加ボタンをクリックしてください。</translation>
    </message>
    <message>
        <location/>
        <source>Type in the name of a player you wish to remove from the group and click Add.</source>
        <translation>削除したいプレイヤー名を入力してAddボタンをクリックしてください。</translation>
    </message>
    <message>
        <location/>
        <source>This shows all the entries active on this channel. Entries inherited from parent channels will be show in italics.&lt;br /&gt;ACLs are evaluated top to bottom, meaning priority increases as you move down the list.</source>
        <translation>このチャンネルで有効な項目を全て表示します。親チャンネルから継承した項目はイタリック(斜字)体で表示されます。&lt;br /&gt;ACLは上から順に評価されます。つまり、リストの順位を下げる事は優先度が上がる事を意味します。</translation>
    </message>
</context>
<context>
    <name>ALSAAudioInput</name>
    <message>
        <location filename="ALSAAudio.cpp" line="+360"/>
        <source>Opening chosen ALSA Input failed: %1</source>
        <translation>選択されたALSA入力のオープンに失敗しました: %1</translation>
    </message>
    <message>
        <location line="-191"/>
        <source>Default ALSA Card</source>
        <translation>デフォルトALSAカード</translation>
    </message>
</context>
<context>
    <name>ALSAAudioOutput</name>
    <message>
        <location line="+326"/>
        <source>Opening chosen ALSA Output failed: %1</source>
        <translation>選択されたALSA出力のオープンに失敗しました: %1</translation>
    </message>
    <message>
        <location line="-325"/>
        <source>Default ALSA Card</source>
        <translation>デフォルトALSAカード</translation>
    </message>
</context>
<context>
    <name>ALSAConfig</name>
    <message>
        <source>Device selection</source>
        <translation type="obsolete">デバイス選択</translation>
    </message>
    <message>
        <source>Device to use for microphone</source>
        <translation type="obsolete">マイクに使用するデバイス</translation>
    </message>
    <message>
        <source>This set which device mumble should use. The &lt;i&gt;default&lt;/i&gt; device is whatever you have configured in alsaconfig, the &lt;i&gt;hwplug&lt;/i&gt; devices are specific hardware devices backed by the ALSA mixer and the &lt;i&gt;hw&lt;/i&gt; devices are raw hardware access. Unless your soundcard supports hardware mixing of audio, using the &lt;i&gt;hw&lt;/i&gt; device will exclude all other programs from using audio.</source>
        <translation type="obsolete">Mumbleが使用するデバイスを設定します。&lt;i&gt;デフォルト&lt;/i&gt;のデバイスは alsaconfig で設定したものです。 &lt;i&gt;hwplug&lt;/i&gt; デバイスはALSAのミキサーに基づく特別なデバイスです。&lt;i&gt;hw&lt;/i&gt; デバイスはハードウェアに直接アクセスします。ご使用のサウンドカードがオーディオのハードウェアミキシングをサポートしていない限り、&lt;i&gt;hw&lt;/i&gt; デバイス設定では他の全てのプログラムでオーディオを使用することが出来なくなってしまいます。</translation>
    </message>
    <message>
        <source>Device to use for speakers/headphones</source>
        <translation type="obsolete">スピーカー/ヘッドホンに使用するデバイス</translation>
    </message>
    <message>
        <source>Input</source>
        <translation type="obsolete">入力</translation>
    </message>
    <message>
        <source>Output</source>
        <translation type="obsolete">出力</translation>
    </message>
    <message>
        <source>Output Options</source>
        <translation type="obsolete">出力オプション</translation>
    </message>
    <message>
        <source>Output Delay</source>
        <translation type="obsolete">出力猶予</translation>
    </message>
    <message>
        <source>Amount of data to buffer for ALSA</source>
        <translation type="obsolete">ALSA用バッファデータの量</translation>
    </message>
    <message>
        <source>This sets the amount of data to prebuffer in the output buffer. Experiment with different values and set it to the lowest which doesn&apos;t cause rapid jitter in the sound.</source>
        <translation type="obsolete">出力バッファにおけるプリバッファの量を設定します。いろいろな値で試してみて、急に不安定にならない一番小さな値に設定してください。</translation>
    </message>
    <message>
        <source>ALSA</source>
        <translation type="obsolete">ALSA</translation>
    </message>
    <message>
        <source>%1ms</source>
        <translation type="obsolete">%1ms</translation>
    </message>
    <message>
        <source>Form</source>
        <translation type="obsolete">フォーム</translation>
    </message>
    <message>
        <source>TextLabel</source>
        <translation type="obsolete">テキストラベル</translation>
    </message>
    <message>
        <source>Default ALSA Card</source>
        <translation type="obsolete">デフォルトALSAカード</translation>
    </message>
</context>
<context>
    <name>ASIOConfig</name>
    <message>
        <location filename="ASIOInput.ui"/>
        <source>Device selection</source>
        <translation>デバイス選択</translation>
    </message>
    <message>
        <location/>
        <source>Capabilities</source>
        <translation>ケーパビリティ</translation>
    </message>
    <message>
        <location/>
        <source>Channels</source>
        <translation>チャンネル</translation>
    </message>
    <message>
        <location/>
        <source>Configure input channels</source>
        <translation>入力チャネルを設定します</translation>
    </message>
    <message>
        <source>This will configure the input channels for ASIO. Make sure you select at least one channel as microphone and speaker. &lt;i&gt;Microphone&lt;/i&gt; should be where your microphone is attached, and &lt;i&gt;Speaker&lt;/i&gt; should be a channel that samples &quot;What you hear&quot;.&lt;br /&gt;For example, on the Audigy 2 ZS, a good selection for Microphone would be &quot;Mic L&quot; while Speaker should be &quot;Mix L&quot; and &quot;Mix R&quot;</source>
        <translation type="obsolete">ASIOのための入力チャンネルを設定します。少なくとも1つのチャンネルをマイクとスピーカーとして選んでください。&lt;i&gt;マイク&lt;/i&gt; はあなたのマイクが接続されているところです。&lt;i&gt;スピーカー&lt;/i&gt;は&quot;聞くもの&quot;を試してみるチャンネルです。Sound Blaster Audigy 2 ZS の例ではスピーカーが&quot;Mix L&quot; and &quot;Mix R&quot;の時、マイクを&quot;Mic L&quot;にするのが良い選択のひとつです。</translation>
    </message>
    <message>
        <location/>
        <source>Device to use for microphone</source>
        <translation>マイクに使用するデバイス</translation>
    </message>
    <message>
        <location/>
        <source>This chooses what device to query. You still need to actually query the device and select which channels to use.</source>
        <translation>問い合わせるデバイスを選択します。あなたはまだデバイスを実際に問い合わせ、使用するチャンネルを選ぶ必要があります。</translation>
    </message>
    <message>
        <location/>
        <source>Device</source>
        <translation>デバイス</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Query</source>
        <translation>問い合わせ(&amp;Q)</translation>
    </message>
    <message>
        <location/>
        <source>Query selected device</source>
        <translation>選択されたデバイスの問い合わせを行います</translation>
    </message>
    <message>
        <location/>
        <source>This queries the selected device for channels. Be aware that many ASIO drivers are buggy to the extreme, and querying them might cause a crash of either the application or the system.</source>
        <translation>チャンネルで選択されたデバイスを問い合わせます。多くのASIOドライバーが極端に不安定であることに注意してください。それらに問い合わせをすることはアプリケーションやシステムのクラッシュを引き起こすかもしれません。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Configure</source>
        <translation>設定(&amp;C)</translation>
    </message>
    <message>
        <location/>
        <source>Configure selected device</source>
        <translation>選択されたデバイスの設定を行います</translation>
    </message>
    <message>
        <location/>
        <source>This configures the selected device. Be aware that many ASIO drivers are buggy to the extreme, and querying them might cause a crash of either the application or the system.</source>
        <translation>選択されたデバイスの設定を行います。多くのASIOドライバーが極端に不安定であることに注意してください。それらに問い合わせをすることはアプリケーションやシステムのクラッシュを引き起こすかもしれません。</translation>
    </message>
    <message>
        <location/>
        <source>Driver name</source>
        <translation>ドライバー名</translation>
    </message>
    <message>
        <source>Buffersize</source>
        <translation type="obsolete">バッファサイズ</translation>
    </message>
    <message>
        <location/>
        <source>Microphone</source>
        <translation>マイク</translation>
    </message>
    <message>
        <location/>
        <source>&lt;-</source>
        <translation>&lt;-</translation>
    </message>
    <message>
        <location/>
        <source>-&gt;</source>
        <translation>-&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Unused</source>
        <translation>未使用</translation>
    </message>
    <message>
        <location/>
        <source>Speakers</source>
        <translation>スピーカー</translation>
    </message>
    <message>
        <source>%1 (ver %2)</source>
        <translation type="obsolete">%1 (ver %2)</translation>
    </message>
    <message>
        <source>%1 ms -&gt; %2 ms (%3 ms resolution) %4Hz</source>
        <translation type="obsolete">%1 ms -&gt; %2 ms (%3 ms resolution) %4Hz</translation>
    </message>
    <message>
        <source>%1 ms -&gt; %2 ms (%3 ms resolution) %4Hz -- Unusable</source>
        <translation type="obsolete">%1 ms -&gt; %2 ms (%3 ms resolution) %4Hz -- 未使用</translation>
    </message>
    <message>
        <location filename="ASIOInput.cpp" line="+222"/>
        <source>%1 (version %2)</source>
        <translation></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>%1 -&gt; %2 samples buffer, with %3 sample resolution (%4 preferred) at %5 Hz</source>
        <translation></translation>
    </message>
    <message>
        <source>Mumble</source>
        <translation type="obsolete">Mumble</translation>
    </message>
    <message>
        <location line="+43"/>
        <location line="+25"/>
        <source>ASIO Initialization failed: %1</source>
        <translation>ASIOの初期化に失敗しました: %1</translation>
    </message>
    <message>
        <location line="-21"/>
        <location line="+25"/>
        <source>Failed to instantiate ASIO driver</source>
        <translation>ASIOドライバのインスタンス化に失敗しました</translation>
    </message>
    <message>
        <location line="+37"/>
        <source>ASIO</source>
        <translation>ASIO</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui"/>
        <source>Form</source>
        <translation>フォーム</translation>
    </message>
    <message>
        <source>TextLabel</source>
        <translation type="obsolete">テキストラベル</translation>
    </message>
    <message>
        <source>This will configure the input channels for ASIO. Make sure you select at least one channel as microphone and speaker. &lt;i&gt;Microphone&lt;/i&gt; should be where your microphone is attached, and &lt;i&gt;Speaker&lt;/i&gt; should be a channel that samples &lt;i&gt;What you hear&lt;/i&gt;.&lt;br /&gt;For example, on the Audigy 2 ZS, a good selection for Microphone would be &lt;i&gt;Mic L&lt;/i&gt; while Speaker should be &lt;i&gt;Mix L&lt;/i&gt; and &lt;i&gt;Mix R&lt;/i&gt;.</source>
        <translation type="obsolete">ASIOのための入力チャンネルを設定します。少なくとも1つのチャンネルをマイクとスピーカーとして選んでください。&lt;i&gt;マイク&lt;/i&gt; はあなたのマイクが接続されているところで、&lt;i&gt;スピーカー&lt;/i&gt;は&quot;聞くもの&quot;を試してみるチャンネルです。Sound Blaster Audigy 2 ZS の例ではスピーカーが&quot;Mix L&quot; and &quot;Mix R&quot;の時、マイクを&quot;Mic L&quot;にするのが良い選択のひとつです。</translation>
    </message>
    <message>
        <location/>
        <source>Buffer size</source>
        <translation>バッファサイズ</translation>
    </message>
    <message>
        <location/>
        <source>This will configure the input channels for ASIO. Make sure you select at least one channel as microphone and speaker. &lt;i&gt;Microphone&lt;/i&gt; should be where your microphone is attached, and &lt;i&gt;Speaker&lt;/i&gt; should be a channel that samples &apos;&lt;i&gt;What you hear&lt;/i&gt;&apos;.&lt;br /&gt;For example, on the Audigy 2 ZS, a good selection for Microphone would be &apos;&lt;i&gt;Mic L&lt;/i&gt;&apos; while Speaker should be &apos;&lt;i&gt;Mix L&lt;/i&gt;&apos; and &apos;&lt;i&gt;Mix R&lt;/i&gt;&apos;.</source>
        <translation>ASIOのための入力チャンネルを設定します。少なくとも1つのチャンネルをマイクとスピーカーとして選んでください。&lt;i&gt;マイク&lt;/i&gt; はあなたのマイクが接続されているところで、&lt;i&gt;スピーカー&lt;/i&gt;は&quot;聞くもの&quot;を試してみるチャンネルです。Sound Blaster Audigy 2 ZS の例ではスピーカーが&quot;Mix L&quot; and &quot;Mix R&quot;の時、マイクを&quot;Mic L&quot;にするのが良い選択のひとつです。</translation>
    </message>
</context>
<context>
    <name>ASIOInput</name>
    <message>
        <source>Mumble</source>
        <translation type="obsolete">Mumble</translation>
    </message>
    <message>
        <location filename="ASIOInput.cpp" line="+80"/>
        <source>You need to select at least one microphone and one speaker source to use ASIO. If you just need microphone sampling, use DirectSound.</source>
        <translation>ASIOを使用するためには少なくとも1つのマイクと1つのスピーカーを選択しなければなりません。もしマイクサンプリングが必要なだけなら DirectSound を使用してください。</translation>
    </message>
    <message>
        <location line="+103"/>
        <source>Opening selected ASIO device failed. No input will be done.</source>
        <translation>選択されたASIOドライバを開くのに失敗しました。入力は行われません。</translation>
    </message>
</context>
<context>
    <name>AboutDialog</name>
    <message>
        <source>&lt;h3&gt;Mumble v1.0.0&lt;/h3&gt;&lt;p&gt;Copyright 2005-2006 Thorvald Natvig&lt;br /&gt;slicer@users.sourceforge.net&lt;/p&gt;&lt;p&gt;&lt;b&gt;A voice-chat utility for gamers&lt;/b&gt;&lt;/p&gt;&lt;p&gt;&lt;tt&gt;http://mumble.sourceforge.net/&lt;/tt&gt;&lt;/p&gt;</source>
        <translation type="obsolete">&lt;h3&gt;Mumble v1.0.0&lt;/h3&gt;&lt;p&gt;Copyright 2005-2006 Thorvald Natvig&lt;br /&gt;slicer@users.sourceforge.net&lt;/p&gt;&lt;p&gt;&lt;b&gt;A voice-chat utility for gamers&lt;/b&gt;&lt;/p&gt;&lt;p&gt;&lt;tt&gt;http://mumble.sourceforge.net/&lt;/tt&gt;&lt;/p&gt;
        </translation>
    </message>
    <message>
        <location filename="About.cpp" line="+63"/>
        <source>&amp;About Mumble</source>
        <translation>Mumbleについて(&amp;A)</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>&amp;License</source>
        <translation>ライセンス(&amp;L)</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <location line="-29"/>
        <source>About Mumble</source>
        <translation>Mumbleについて</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>&lt;h3&gt;Mumble (%1)&lt;/h3&gt;&lt;p&gt;Copyright %3 Thorvald Natvig&lt;br /&gt;slicer@users.sourceforge.net&lt;/p&gt;&lt;p&gt;&lt;b&gt;A voice-chat utility for gamers&lt;/b&gt;&lt;/p&gt;&lt;p&gt;&lt;tt&gt;&lt;a href=&quot;%2&quot;&gt;%2&lt;/a&gt;&lt;/tt&gt;&lt;/p&gt;</source>
        <translation></translation>
    </message>
</context>
<context>
    <name>AboutSpeexDialog</name>
    <message>
        <source>&lt;h3&gt;About Speex&lt;/h3&gt;&lt;p&gt;&lt;tt&gt;http://www.speex.org/&lt;/tt&gt;&lt;/p&gt;&lt;p&gt;This program uses Speex version %1&lt;/p&gt;&lt;p&gt;Speex is used for echo cancellation, noise&lt;br /&gt;filtering, voice activity detection and speech&lt;br /&gt;compression.&lt;/p&gt;</source>
        <translation type="obsolete">&lt;h3&gt;Speexについて&lt;/h3&gt;&lt;p&gt;&lt;tt&gt;http://www.speex.org/&lt;/tt&gt;&lt;/p&gt;&lt;p&gt;This program uses Speex version %1&lt;/p&gt;&lt;p&gt;Speex is used for echo cancellation, noise&lt;br /&gt;filtering, voice activity detection and speech&lt;br /&gt;compression.&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+37"/>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <location line="-13"/>
        <source>About Speex</source>
        <translation>Speexについて</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>&lt;h3&gt;About Speex&lt;/h3&gt;&lt;p&gt;&lt;tt&gt;&lt;a href=&quot;%2&quot;&gt;%2&lt;/a&gt;&lt;/tt&gt;&lt;/p&gt;&lt;p&gt;This program uses Speex version %1&lt;/p&gt;&lt;p&gt;Speex is used for echo cancellation, noise&lt;br /&gt;filtering, voice activity detection and speech&lt;br /&gt;compression.&lt;/p&gt;</source>
        <translation>&lt;h3&gt;Speexについて&lt;/h3&gt;&lt;p&gt;&lt;tt&gt;&lt;a href=&quot;%2&quot;&gt;%2&lt;/a&gt;&lt;/tt&gt;&lt;/p&gt;&lt;p&gt;This program uses Speex version %1&lt;/p&gt;&lt;p&gt;Speex is used for echo cancellation, noise&lt;br /&gt;filtering, voice activity detection and speech&lt;br /&gt;compression.&lt;/p&gt;</translation>
    </message>
</context>
<context>
    <name>AudioConfigDialog</name>
    <message>
        <source>Interfaces</source>
        <translation type="obsolete">インターフェース</translation>
    </message>
    <message>
        <source>Transmission</source>
        <translation type="obsolete">伝送方式</translation>
    </message>
    <message>
        <source>Compression</source>
        <translation type="obsolete">圧縮</translation>
    </message>
    <message>
        <source>Jitter Buffer</source>
        <translation type="obsolete">ジッタバッファ</translation>
    </message>
    <message>
        <source>Input</source>
        <translation type="obsolete">入力</translation>
    </message>
    <message>
        <source>Input method for audio</source>
        <translation type="obsolete">音声入力方法</translation>
    </message>
    <message>
        <source>&lt;b&gt;This is the input method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation type="obsolete">&lt;b&gt;音声の入力方法です。&lt;/b&gt;&lt;br /&gt;ほとんどの場合DirectSoundで問題ありません。</translation>
    </message>
    <message>
        <source>Output</source>
        <translation type="obsolete">出力</translation>
    </message>
    <message>
        <source>Output method for audio</source>
        <translation type="obsolete">音声入力方法</translation>
    </message>
    <message>
        <source>&lt;b&gt;This is the output method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation type="obsolete">&lt;b&gt;音声の出力方法です。&lt;/b&gt;&lt;br /&gt;ほとんどの場合DirectSoundで問題ありません。</translation>
    </message>
    <message>
        <source>Continuous</source>
        <translation type="obsolete">常に有効</translation>
    </message>
    <message>
        <source>Voice Activity</source>
        <translation type="obsolete">声で有効化</translation>
    </message>
    <message>
        <source>Push To Talk</source>
        <translation type="obsolete">キー押下で有効化</translation>
    </message>
    <message>
        <source>Transmit</source>
        <translation type="obsolete">送信</translation>
    </message>
    <message>
        <source>When to transmit your speech</source>
        <translation type="obsolete">発言を送信するタイミング</translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets when speech should be transmitted.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;Continuous&lt;/i&gt; - All the time&lt;br /&gt;&lt;i&gt;Voice Activity&lt;/i&gt; - When you are speaking clearly.&lt;br /&gt;&lt;i&gt;Push To Talk&lt;/i&gt; - When you hold down the hotkey set under &lt;i&gt;Shortcuts&lt;/i&gt;.</source>
        <translation type="obsolete">&lt;b&gt;発言を送信するタイミングを設定します。&lt;/b&gt;&lt;br /&gt;&lt;i&gt;常に有効&lt;/i&gt; - 常時音声入力を受け付けます&lt;br /&gt;&lt;i&gt;声で有効化&lt;/i&gt; - はっきりとしゃべっている時に受け付けます。&lt;br /&gt;&lt;i&gt;キー押下で有効化&lt;/i&gt; - &lt;i&gt;ショートカットキー&lt;/i&gt;で設定したキーを押している間音声入力を受け付けます。</translation>
    </message>
    <message>
        <source>Voice Hold</source>
        <translation type="obsolete">送信継続時間</translation>
    </message>
    <message>
        <source>How long to keep transmitting after silence</source>
        <translation type="obsolete">音声が無くなった後、どれくらい送信を続けるか</translation>
    </message>
    <message>
        <source>&lt;b&gt;This selects how long after a perceived stop in speech transmission should continue.&lt;/b&gt;&lt;br /&gt;Set this higher if your voice breaks up when you speak (seen by a rapidly blinking voice icon next to your name).&lt;br /&gt;Only makes sense when used with Voice Activity transmission.</source>
        <translation type="obsolete">&lt;b&gt;音声が無くなった後、送信がどれくらい続くかを選択します。&lt;/b&gt;&lt;br /&gt;あなたが話している時(名前の後ろで点滅しているアイコンで確認できます)に音声が途切れてしまうようならば、この設定をより高くしてください。&lt;br /&gt;この設定は&quot;声で有効化&quot;の場合のみ効果があります。</translation>
    </message>
    <message>
        <source>Audio per packet</source>
        <translation type="obsolete">パケットあたりの音声長</translation>
    </message>
    <message>
        <source>How many audio frames to send per packet</source>
        <translation type="obsolete">送信1パケットに含む音声フレームの長さ</translation>
    </message>
    <message>
        <source>&lt;b&gt;This selects how many audio frames should be put in one packet.&lt;/b&gt;&lt;br /&gt;Increasing this will increase the latency of your voice, but will also reduce bandwidth requirements.</source>
        <translation type="obsolete">&lt;b&gt;送信1パケットに含む音声フレームの長さを選択します。&lt;/b&gt;&lt;br /&gt;これを大きくするとあなたの音声の遅延が大きくなりますが、必要とするネットワーク帯域幅は下がります。</translation>
    </message>
    <message>
        <source>Use TCP mode</source>
        <translation type="obsolete">TCPモードを使用する</translation>
    </message>
    <message>
        <source>Use TCP Compatibility mode</source>
        <translation type="obsolete">TCP互換モードを使用します</translation>
    </message>
    <message>
        <source>&lt;b&gt;Enable TCP Compatibility mode&lt;/b&gt;.&lt;br /&gt;This will make Mumble use only TCP when communicating with the server. This will increase overhead and cause lost packets to produce noticeable pauses in communication, so this should only be used if you are unable to use the default (which uses UDP for voice and TCP for control).</source>
        <translation type="obsolete">&lt;b&gt;TCP互換モードを有効にします。&lt;/b&gt;.&lt;br /&gt;この設定をするとMumbleがサーバーとの通信にTCPのみを使用するようになります。この設定は通信のオーバーヘッドを増大させ、通信時に音声が途切れる原因になるので、デフォルトの設定(音声:UDP 操作:TCP)が使用できない場合だけ使用するのが良いでしょう。</translation>
    </message>
    <message>
        <source>Automatic Reconnect</source>
        <translation type="obsolete">自動再接続</translation>
    </message>
    <message>
        <source>Reconnect when disconnected</source>
        <translation type="obsolete">切断時に再接続を試みます</translation>
    </message>
    <message>
        <source>&lt;b&gt;Reconnect when disconnected&lt;/b&gt;.&lt;br /&gt;This will make Mumble try to automatically reconnect after 10 seconds if your server connection fails.</source>
        <translation type="obsolete">&lt;b&gt;切断時に再接続を試みます。&lt;/b&gt;&lt;br /&gt;この設定をすると、サーバーへの接続が失敗した際、10秒後に再接続を行います。</translation>
    </message>
    <message>
        <source>Maximum bandwidth used for sent audio</source>
        <translation type="obsolete">音声の送信に使用するネットワーク帯域幅のピーク値</translation>
    </message>
    <message>
        <source>&lt;b&gt;This shows peak outgoing bandwidth used.&lt;/b&gt;&lt;br /&gt;This shows the peak amount of bandwidth sent out from your machine. Audio bitrate is the maximum bitrate (as we use VBR) for the audio data alone. Position is the bitrate used for positional information. Overhead is our framing and the IP packet headers (IP and UDP is 90% of this overhead).</source>
        <translation type="obsolete">&lt;b&gt;音声の送信に使用される帯域の最大値を示します。&lt;/b&gt;&lt;br /&gt;これは、あなたのPCから送出される帯域幅のピーク値を示します。Audioビットレートは音声データ単独の最大ビットレート(VBR使用時)です。 Positionは位置情報のために使用されるビットレートです。 Overheadは独自のフレーミングとIPパケットヘッダ(IP と UDP はこのOverheadの90%)です。</translation>
    </message>
    <message>
        <source>Outgoing Bitrate</source>
        <translation type="obsolete">送信方向のビットレート</translation>
    </message>
    <message>
        <source>Quality</source>
        <translation type="obsolete">音質</translation>
    </message>
    <message>
        <source>Quality of compression (peak bandwidth)</source>
        <translation type="obsolete">圧縮品質(ピーク帯域幅)</translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets the quality of compression.&lt;/b&gt;&lt;br /&gt;This determines how much bandwidth Mumble is allowed to use for outgoing audio.</source>
        <translation type="obsolete">&lt;b&gt;圧縮品質を設定します。&lt;/b&gt;&lt;br /&gt;送出される音声のためにどれくらいの帯域幅を使えるか決定します。</translation>
    </message>
    <message>
        <source>Complexity</source>
        <translation type="obsolete">計算量</translation>
    </message>
    <message>
        <source>Complexity of compression (CPU)</source>
        <translation type="obsolete">圧縮の計算量(CPU)</translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets the complexity of compression.&lt;/b&gt;&lt;br /&gt;This determines how much CPU Mumble is allowed to use to increase transmitted voice quality. Settings above 5 give only marginal gain.</source>
        <translation type="obsolete">&lt;b&gt;圧縮の計算量を設定します。&lt;/b&gt;&lt;br /&gt;送信される声の音質を向上させるためにどれくらいのCPUリソースを使えるか決定します。</translation>
    </message>
    <message>
        <source>Amp</source>
        <translation type="obsolete">アンプ</translation>
    </message>
    <message>
        <source>Maximum amplification of input sound</source>
        <translation type="obsolete">音声入力の最大増幅量</translation>
    </message>
    <message>
        <source>&lt;b&gt;Maximum amplification of input.&lt;/b&gt;&lt;br /&gt;Mumble normalizes the input volume before compressing, and this sets how much it&apos;s allowed to amplify.&lt;br /&gt;The actual level is continually updated based on your current speech pattern, but it will never go above the level specified here.&lt;br /&gt;If the &lt;i&gt;Microphone loudness&lt;/i&gt; level of the audio statistics hover around 100%, you probably want to set this to 2.0 or so, but if, like most people, you are unable to reach 100%, set this to something much higher.&lt;br /&gt;Ideally, set it so &lt;i&gt;Microphone Loudness * Amplification Factor &gt;= 100&lt;/i&gt;, even when you&apos;re speaking really soft.&lt;br /&gt;&lt;br /&gt;Note that there is no harm in setting this to maximum, but Mumble will start picking up other conversations if you leave it to auto-tune to that level.</source>
        <translation type="obsolete">&lt;b&gt;音声入力の最大増幅量です。&lt;/b&gt;&lt;br /&gt;Mumbleは圧縮を行う前に入力量を正常化します。この設定は、どれくらい音量を増幅させるかを決定します。&lt;br /&gt;実際の音量はその時の発言パターンによって変化しますが、ここで指定された音量を上回ることはありません。&lt;br /&gt;音声統計で&lt;i&gt;マイクの音量&lt;/i&gt;レベルが常に100%近くに達しているなら、この値を2.0くらいにしたくなるかも知れません。しかし、(大部分の人がそうだと思いますが)100%に届かないなら、この値をより高くしてください。&lt;br /&gt;あなたが非常に穏やかに話している時に&lt;i&gt;マイク音量 * 増幅値 &gt;= 100&lt;/i&gt; となるのが理想的です。&lt;br /&gt;&lt;br /&gt;この値を最大にすることに実害はありませんが、だからといって適当に最大値に合わせてしまうと他の会話まで拾ってしまうので気をつけてください。</translation>
    </message>
    <message>
        <source>Default Jitter Buffer</source>
        <translation type="obsolete">ジッタバッファのデフォルト</translation>
    </message>
    <message>
        <source>How long to prebuffer on first packet</source>
        <translation type="obsolete">初回パケットのプリバッファまでの長さ</translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets the default buffer size for the jitter buffer&lt;/b&gt;.&lt;br /&gt;All incoming audio is buffered, and the jitter buffer continually tries to push the buffer to the minimum sustainable by your network, so latency can be as low as possible. This sets the default buffer size to use on the first packet to arrive from a new speaker, or when a speaker using Voice Activity or Push-To-Talk just started talking again. If the start of sentences you hear is very jittery, increase this value.</source>
        <translation type="obsolete">&lt;b&gt;ジッタバッファのデフォルトサイズを設定します。&lt;/b&gt;&lt;br /&gt;全ての受信音声は一時保存されます。ジッタバッファは、現在のネットワークの状態に合わせて一時保存の領域を小さくしようとし続けるので、レイテンシーは可能な限り小さくなります。この設定は、使用する最小のバッファサイズを決定します。これは、新たな話し手から到着する最初のパケット(または&quot;声で有効化&quot;または&quot;キー押下で有効化&quot;にしている話し手の音声が再開したとき)に使用されるデフォルトバッファサイズを設定します。聞こえてくる会話の出だしが不安定なら、この値を増やしてください。</translation>
    </message>
    <message>
        <source>Basic Audio</source>
        <translation type="obsolete">基本設定</translation>
    </message>
    <message>
        <source>%1 ms</source>
        <translation type="obsolete">%1 ms</translation>
    </message>
    <message>
        <source>%1s</source>
        <translation type="obsolete">%1s</translation>
    </message>
    <message>
        <source>%1kbit/s (Audio %2, Position %4, Overhead %3)</source>
        <translation type="obsolete">%1kbit/s (Audio %2, Position %4, Overhead %3)</translation>
    </message>
    <message>
        <source>Loopback Test</source>
        <translation type="obsolete">ループバックテスト</translation>
    </message>
    <message>
        <source>PTT Audio cue</source>
        <translation type="obsolete">キー押下を音で通知</translation>
    </message>
    <message>
        <source>Audible audio cue when push-to-talk pressed and released</source>
        <translation type="obsolete">&quot;キー押下で有効化&quot;設定の時、ショートカットキーが押されるのと離されるタイミングで音による通知を行います。</translation>
    </message>
    <message>
        <source>&lt;b&gt;This enables the audio cues for &quot;push to talk&quot;&lt;/b&gt;&lt;br /&gt;Setting this will give you a short audio beep when push to talk is pressed and released.</source>
        <translation type="obsolete">&lt;b&gt;&quot;キー押下で有効化&quot;設定の際、音による通知を行います。&lt;/b&gt;&lt;br /&gt;これを設定するとキーを押した時と離した時に短いビープ音が聞こえるようになります。</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">なし</translation>
    </message>
    <message>
        <source>Local</source>
        <translation type="obsolete">ローカル</translation>
    </message>
    <message>
        <source>Server</source>
        <translation type="obsolete">サーバー</translation>
    </message>
    <message>
        <source>Desired loopback mode</source>
        <translation type="obsolete">使用したいループバックモード</translation>
    </message>
    <message>
        <source>&lt;b&gt;This enables one of the loopback testmodes.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;None&lt;/i&gt; - Loopback disabled&lt;br /&gt;&lt;i&gt;Local&lt;/i&gt; - Emulate a local server.&lt;br /&gt;&lt;i&gt;Server&lt;/i&gt; - Request loopback from server.&lt;br /&gt;Please note than when loopback is enabled, no other players will hear your voice. This setting is not saved on application exit.</source>
        <translation type="obsolete">&lt;b&gt;ループバックテストモードの一つが有効になります。&lt;/b&gt;&lt;br /&gt;&lt;i&gt;なし&lt;/i&gt; - ループバックしない&lt;br /&gt;&lt;i&gt;ローカル&lt;/i&gt; - ローカルなサーバーをエミュレートします。&lt;br /&gt;&lt;i&gt;サーバー&lt;/i&gt; - サーバーにループバックを要求します。&lt;br /&gt;ループバックが有効になっている間、他のプレイヤーはあなたの声を聞くことが出来ないのでご注意ください。この設定はアプリケーション終了時に保存されません。</translation>
    </message>
    <message>
        <source>Delay Variance</source>
        <translation type="obsolete">レイテンシー調整</translation>
    </message>
    <message>
        <source>Variance in packet latency</source>
        <translation type="obsolete">パケットレイテンシーの変動値</translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets the packet latency variance for loopback testing&lt;/b&gt;&lt;br /&gt;Most audio paths contain some variable latency. This allows you set that variance for loopback mode testing. For example, if you set this to 15ms, this will emulate a network with 20-35ms ping latency or one with 80-95ms latency. Most domestic net connections have a variance of about 5ms</source>
        <translation type="obsolete">&lt;b&gt;ループバックテストの為のパケットレイテンシーの値を調整します。&lt;/b&gt;&lt;br /&gt;音声のネットワーク経路は、ほとんどの場合若干のレイテンシーを含んでいます。 この設定はループバックモードでのテスト時、レイテンシーの値を変更できるようにします。例えば、この値を15msにすると、20-35msのレイテンシーや80ms-95msのレイテンシーを持ったネットワークをエミュレートするでしょう。 国内のネット接続ではおよそ5msくらいのレイテンシーがあります。</translation>
    </message>
    <message>
        <source>Packet Loss</source>
        <translation type="obsolete">パケットロス</translation>
    </message>
    <message>
        <source>Packet loss for loopback mode</source>
        <translation type="obsolete">ループバックモードでのパケットロス</translation>
    </message>
    <message>
        <source>Form</source>
        <translation type="obsolete">フォーム</translation>
    </message>
    <message>
        <source>&amp;Input</source>
        <translation type="obsolete">入力(&amp;I)</translation>
    </message>
    <message>
        <source>&amp;Output</source>
        <translation type="obsolete">出力(&amp;O)</translation>
    </message>
    <message>
        <source>&amp;Transmit</source>
        <translation type="obsolete">送信(&amp;T)</translation>
    </message>
    <message>
        <source>Voice &amp;Hold</source>
        <translation type="obsolete">送信継続時間(&amp;H)</translation>
    </message>
    <message>
        <source>TextLabel</source>
        <translation type="obsolete">テキストラベル</translation>
    </message>
    <message>
        <source>&lt;b&gt;This shows peak outgoing bandwidth used.&lt;/b&gt;&lt;br /&gt;This shows the peak amount of bandwidth sent out from your machine. Audio bitrate is the maximum bitrate (as we use VBR) for the audio data alone. Position is the bitrate used for positional information. Overhead is our framing and the IP packet headers (IP and UDP is 75% of this overhead).</source>
        <translation type="obsolete">&lt;b&gt;音声の送信に使用される帯域幅の最大値を示します。&lt;/b&gt;&lt;br /&gt;これは、あなたのPCから送出される帯域の最も大きな値を表示しています。Audioビットレートは音声データ単体の最大ビットレート(VBR使用時)を表します。 Positionは位置情報のために使用されるビットレート表し、Overheadは独自のフレーミングとIPパケットヘッダ(IP と UDP はこのOverheadの90%)を表しています。</translation>
    </message>
    <message>
        <source>&amp;Quality</source>
        <translation type="obsolete">音質(&amp;Q)</translation>
    </message>
    <message>
        <source>&amp;Complexity</source>
        <translation type="obsolete">計算量(&amp;C)</translation>
    </message>
    <message>
        <source>Audio Processing</source>
        <translation type="obsolete">音声処理</translation>
    </message>
    <message>
        <source>Noise Suppression</source>
        <translation type="obsolete">ノイズ抑制</translation>
    </message>
    <message>
        <source>Noise suppression</source>
        <translation type="obsolete">ノイズ抑制</translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets the ammount of noise suppression to apply.&lt;/b&gt;&lt;br /&gt;The higher this value, the more aggressively stationary noise will be supressed.</source>
        <translation type="obsolete">&lt;b&gt;ノイズ抑制の量を設定します。&lt;/b&gt;&lt;br /&gt;値を大きくすると、より積極的に定常ノイズを抑制しようとします。</translation>
    </message>
    <message>
        <source>Amplification</source>
        <translation type="obsolete">音量の増幅</translation>
    </message>
    <message>
        <source>Audio Output</source>
        <translation type="obsolete">音声出力</translation>
    </message>
    <message>
        <source>Default &amp;Jitter Buffer</source>
        <translation type="obsolete">ジッタバッファのデフォルト(&amp;J)</translation>
    </message>
    <message>
        <source>Safety margin for jitter buffer</source>
        <translation type="obsolete">ジッタバッファの安全マージン</translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets the minimum safety margin for the jitter buffer.&lt;/b&gt;&lt;br /&gt;All incoming audio is buffered, and the jitter buffer continually tries to push the buffer to the minimum sustainable by your network, so latency can be as low as possible. This sets the minimum buffer size to use. If the start of sentences you hear is very jittery, increase this value.</source>
        <translation type="obsolete">&lt;b&gt;ジッタバッファのための最小の安全マージンを設定します。&lt;/b&gt;&lt;br /&gt;全ての受信音声は一時保存されます。ジッタバッファは、現在のネットワークの状態に合わせて一時保存の領域を小さくしようとし続けるので、レイテンシーは可能な限り小さくなります。この設定は、使用する最小のバッファサイズを決定します。聞こえてくる会話の出だしが不安定なら、この値を増やしてください。</translation>
    </message>
    <message>
        <source>Volume</source>
        <translation type="obsolete">音量</translation>
    </message>
    <message>
        <source>Volume of incoming speech</source>
        <translation type="obsolete">受信音声の音量</translation>
    </message>
    <message>
        <source>&lt;b&gt;This adjusts the volume of incoming speech.&lt;/b&gt;&lt;br /&gt;Note that this can only be used to decrease the volume. No amplification is possible.</source>
        <translation type="obsolete">&lt;b&gt;受信する音声の音量を調整します。&lt;/b&gt;&lt;br /&gt;ここでは、音量を下げる事のみが可能です。増やす事は出来ません。</translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets the packet loss for loopback mode.&lt;/b&gt;&lt;br /&gt;This will be the ratio of packets lost. Unless your outgoing bandwidth is peaked or there&apos;s something wrong with your network connection, this will be 0%</source>
        <translation type="obsolete">&lt;b&gt;ループバックモードでのパケットロスを設定します。&lt;/b&gt;&lt;br /&gt;これはパケットロスの割合です。送信方向の帯域幅のピークに達するか、ネットワーク接続に問題がない限り、これは0%のはずです。</translation>
    </message>
    <message>
        <source>&amp;Loopback</source>
        <translation type="obsolete">ループバック(&amp;L)</translation>
    </message>
    <message>
        <source>Off</source>
        <translation type="obsolete">オフ</translation>
    </message>
    <message>
        <source>DoublePush Time</source>
        <translation type="obsolete">二重押し時間</translation>
    </message>
    <message>
        <source>Use SNR based speech detection</source>
        <translation type="obsolete">S/N比に基づく発言認識を使用</translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets speech detection to use Signal to Noise ratio.&lt;/b&gt;&lt;br /&gt;In this mode, the input is analyzed for something resembling a clear singal, and the clarity of that signal is used to trigger speech detection.</source>
        <translation type="obsolete">&lt;b&gt;発言認識にS/N比を使うかどうかを決定します。&lt;/b&gt;&lt;br /&gt;このモードでは、ノイズ無しに近い状態に比較して入力が解析され、その明瞭度が発言の検出に使用されます。</translation>
    </message>
    <message>
        <source>Signal to Noise</source>
        <translation type="obsolete">S/N比</translation>
    </message>
    <message>
        <source>Use Amplitude based speech detection.</source>
        <translation type="obsolete">信号の強さに基づく発言認識を使用</translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets speech detection to use Amplitude.&lt;/b&gt;&lt;br /&gt;In this mode, the raw strength of the input signal is used to detect speech.</source>
        <translation type="obsolete">&lt;b&gt;発言認識に信号の強さを使用するかを設定します。&lt;/b&gt;このモードにすると、入力信号そのものの強さが発言の検出に使用されます。</translation>
    </message>
    <message>
        <source>Amplitude</source>
        <translation type="obsolete">信号の強さ</translation>
    </message>
    <message>
        <source>Silence Below</source>
        <translation type="obsolete">沈黙しきい値</translation>
    </message>
    <message>
        <source>Signal values below this counts as silence</source>
        <translation type="obsolete">沈黙と判定される信号値のしきい値</translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets the trigger values for voice detection.&lt;/b&gt;&lt;br /&gt;Use this together with the Audio Statistics window to manually tune the trigger values for detecting speech. Input values below &quot;Silence Below&quot; always count as silence. Values above &quot;Speech Above&quot; always count as voice. Values in between will count as voice if you&apos;re already talking, but will not trigger a new detection.</source>
        <translation type="obsolete">&lt;b&gt;声を検出するための基準値を設定します。&lt;/b&gt;手動で調整するためには音声統計ウインドウを一緒にご利用ください。&quot;沈黙しきい値&quot;以下の値は常に発言していない状態と見なされ、&quot;発言しきい値&quot;より上の値は発言と見なされます。中間の値は既に話し中であれば発言と判断されますが、新たな発言であると判断する材料にはなりません。</translation>
    </message>
    <message>
        <source>Speech Above</source>
        <translation type="obsolete">発言しきい値</translation>
    </message>
    <message>
        <source>Signal values above this count as voice</source>
        <translation type="obsolete">発言と判定される信号値のしきい値</translation>
    </message>
    <message>
        <source>Connection</source>
        <translation type="obsolete">接続</translation>
    </message>
</context>
<context>
    <name>AudioInput</name>
    <message>
        <location filename="AudioInput.ui"/>
        <source>Form</source>
        <translation>フォーム</translation>
    </message>
    <message>
        <location/>
        <source>Interface</source>
        <translation>インターフェース</translation>
    </message>
    <message>
        <location/>
        <source>System</source>
        <translation>システム</translation>
    </message>
    <message>
        <location/>
        <source>Input method for audio</source>
        <translation>音声入力方法</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is the input method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation>&lt;b&gt;音声の入力方法です。&lt;/b&gt;&lt;br /&gt;ほとんどの場合DirectSoundで問題ありません。</translation>
    </message>
    <message>
        <location/>
        <source>Device</source>
        <translation>デバイス</translation>
    </message>
    <message>
        <location/>
        <source>Input device for audio</source>
        <oldsource>Output method for audio</oldsource>
        <translation>音声入力方法</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is the input device to use for audio.&lt;/b&gt;</source>
        <oldsource>&lt;b&gt;This is the output method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</oldsource>
        <translation>&lt;b&gt;音声のための入力デバイスです。&lt;/b&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Cancel Echo</source>
        <translation>エコーキャンセル</translation>
    </message>
    <message>
        <location/>
        <source>Transmission</source>
        <translation>伝送方式</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Transmit</source>
        <translation>送信(&amp;T)</translation>
    </message>
    <message>
        <location/>
        <source>When to transmit your speech</source>
        <translation>発言を送信するタイミング</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets when speech should be transmitted.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;Continuous&lt;/i&gt; - All the time&lt;br /&gt;&lt;i&gt;Voice Activity&lt;/i&gt; - When you are speaking clearly.&lt;br /&gt;&lt;i&gt;Push To Talk&lt;/i&gt; - When you hold down the hotkey set under &lt;i&gt;Shortcuts&lt;/i&gt;.</source>
        <translation>&lt;b&gt;発言を送信するタイミングを設定します。&lt;/b&gt;&lt;br /&gt;&lt;i&gt;常に有効&lt;/i&gt; - 常時音声入力を受け付けます&lt;br /&gt;&lt;i&gt;声で有効化&lt;/i&gt; - はっきりとしゃべっている時に受け付けます。&lt;br /&gt;&lt;i&gt;キー押下で有効化&lt;/i&gt; - &lt;i&gt;ショートカットキー&lt;/i&gt;で設定したキーを押している間音声入力を受け付けます。</translation>
    </message>
    <message>
        <location/>
        <source>DoublePush Time</source>
        <translation>二重押し時間</translation>
    </message>
    <message>
        <location/>
        <source>TextLabel</source>
        <translation>テキストラベル</translation>
    </message>
    <message>
        <source>Audible audio cue when push-to-talk pressed and released</source>
        <translation type="obsolete">&quot;キー押下で有効化&quot;設定の時、ショートカットキーが押される/離されるといったタイミングで音による通知を行います。</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This enables the audio cues for push to talk.&lt;/b&gt;&lt;br /&gt;Setting this will give you a short audio beep when push to talk is pressed and released.</source>
        <translation>&lt;b&gt;プッシュトークの際に通知を行います&lt;/b&gt;&lt;br /&gt;この設定を有効にすると、ショートカットキーが押されたり離されたりするタイミング短いビープ音を鳴らします。</translation>
    </message>
    <message>
        <location/>
        <source>PTT Audio cue</source>
        <translation>キー押下を音で通知</translation>
    </message>
    <message>
        <location/>
        <source>Use SNR based speech detection</source>
        <translation>S/N比に基づく発言認識を使用</translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets speech detection to use Signal to Noise ratio.&lt;/b&gt;&lt;br /&gt;In this mode, the input is analyzed for something resembling a clear singal, and the clarity of that signal is used to trigger speech detection.</source>
        <translation type="obsolete">&lt;b&gt;発言認識にS/N比を使うかどうかを決定します。&lt;/b&gt;&lt;br /&gt;このモードでは、ノイズ無しに近い状態に比較して入力が解析され、その明瞭度が発言の検出に使用されます。</translation>
    </message>
    <message>
        <location/>
        <source>Signal to Noise</source>
        <translation>S/N比</translation>
    </message>
    <message>
        <location/>
        <source>Use Amplitude based speech detection.</source>
        <translation>信号の強さに基づく発言認識を使用。</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets speech detection to use Amplitude.&lt;/b&gt;&lt;br /&gt;In this mode, the raw strength of the input signal is used to detect speech.</source>
        <translation>&lt;b&gt;発言認識に信号の強さを使用するかを設定します。&lt;/b&gt;このモードにすると、入力信号そのものの強さが発言の検出に使用されます。</translation>
    </message>
    <message>
        <location/>
        <source>Amplitude</source>
        <translation>信号の強さ</translation>
    </message>
    <message>
        <location/>
        <source>Voice &amp;Hold</source>
        <translation>送信継続時間(&amp;H)</translation>
    </message>
    <message>
        <location/>
        <source>How long to keep transmitting after silence</source>
        <translation>音声が無くなった後、どれくらい送信を続けるか</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This selects how long after a perceived stop in speech transmission should continue.&lt;/b&gt;&lt;br /&gt;Set this higher if your voice breaks up when you speak (seen by a rapidly blinking voice icon next to your name).&lt;br /&gt;Only makes sense when used with Voice Activity transmission.</source>
        <translation>&lt;b&gt;音声が無くなった後、送信がどれくらい続くかを選択します。&lt;/b&gt;&lt;br /&gt;あなたが話している時(名前の後ろで点滅しているアイコンで確認できます)に音声が途切れてしまうようならば、この設定をより高くしてください。&lt;br /&gt;この設定は&quot;声で有効化&quot;の場合のみ効果があります。</translation>
    </message>
    <message>
        <location/>
        <source>Silence Below</source>
        <translation>沈黙しきい値</translation>
    </message>
    <message>
        <location/>
        <source>Signal values below this counts as silence</source>
        <translation>沈黙と判定される信号値のしきい値</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the trigger values for voice detection.&lt;/b&gt;&lt;br /&gt;Use this together with the Audio Statistics window to manually tune the trigger values for detecting speech. Input values below &quot;Silence Below&quot; always count as silence. Values above &quot;Speech Above&quot; always count as voice. Values in between will count as voice if you&apos;re already talking, but will not trigger a new detection.</source>
        <translation>&lt;b&gt;声を検出するための基準値を設定します。&lt;/b&gt;手動で調整するためには音声統計ウインドウを一緒にご利用ください。&quot;沈黙しきい値&quot;以下の値は常に発言していない状態と見なされ、&quot;発言しきい値&quot;より上の値は発言と見なされます。中間の値は既に話し中であれば発言と判断されますが、新たな発言であると判断する材料にはなりません。</translation>
    </message>
    <message>
        <location/>
        <source>Speech Above</source>
        <translation>発言しきい値</translation>
    </message>
    <message>
        <location/>
        <source>Signal values above this count as voice</source>
        <translation>発言と判定される信号値のしきい値</translation>
    </message>
    <message>
        <location/>
        <source>Compression</source>
        <translation>圧縮</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Quality</source>
        <translation>音質(&amp;Q)</translation>
    </message>
    <message>
        <location/>
        <source>Quality of compression (peak bandwidth)</source>
        <translation>圧縮品質(ピーク帯域幅)</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the quality of compression.&lt;/b&gt;&lt;br /&gt;This determines how much bandwidth Mumble is allowed to use for outgoing audio.</source>
        <translation>&lt;b&gt;圧縮品質を設定します。&lt;/b&gt;&lt;br /&gt;送出される音声のためにどれくらいの帯域幅を使えるか決定します。</translation>
    </message>
    <message>
        <source>&amp;Complexity</source>
        <translation type="obsolete">計算量(&amp;C)</translation>
    </message>
    <message>
        <source>Complexity of compression (CPU)</source>
        <translation type="obsolete">圧縮の計算量(CPU)</translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets the complexity of compression.&lt;/b&gt;&lt;br /&gt;This determines how much CPU Mumble is allowed to use to increase transmitted voice quality. Settings above 5 give only marginal gain.</source>
        <translation type="obsolete">&lt;b&gt;圧縮の計算量を設定します。&lt;/b&gt;&lt;br /&gt;送信される声の音質を向上させるためにどれくらいのCPUリソースを使えるか決定します。</translation>
    </message>
    <message>
        <location/>
        <source>Audio per packet</source>
        <translation>パケットあたりの音声長</translation>
    </message>
    <message>
        <location/>
        <source>How many audio frames to send per packet</source>
        <translation>送信1パケットに含む音声フレームの長さ</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This selects how many audio frames should be put in one packet.&lt;/b&gt;&lt;br /&gt;Increasing this will increase the latency of your voice, but will also reduce bandwidth requirements.</source>
        <translation>&lt;b&gt;送信1パケットに含む音声フレームの長さを選択します。&lt;/b&gt;&lt;br /&gt;これを大きくするとあなたの音声の遅延が大きくなりますが、必要とするネットワーク帯域幅は下がります。</translation>
    </message>
    <message>
        <source>Maximum bandwidth used for sent audio</source>
        <translation type="obsolete">音声の送信に使用するネットワーク帯域幅のピーク値</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This shows peak outgoing bandwidth used.&lt;/b&gt;&lt;br /&gt;This shows the peak amount of bandwidth sent out from your machine. Audio bitrate is the maximum bitrate (as we use VBR) for the audio data alone. Position is the bitrate used for positional information. Overhead is our framing and the IP packet headers (IP and UDP is 75% of this overhead).</source>
        <translation>&lt;b&gt;音声の送信に使用される帯域幅の最大値を示します。&lt;/b&gt;&lt;br /&gt;これは、あなたのPCから送出される帯域の最も大きな値を表示しています。Audioビットレートは音声データ単体の最大ビットレート(VBR使用時)を表します。 Positionは位置情報のために使用されるビットレート表し、Overheadは独自のフレーミングとIPパケットヘッダ(IP と UDP はこのOverheadの90%)を表しています。</translation>
    </message>
    <message>
        <location/>
        <source>Audio Processing</source>
        <translation>音声処理</translation>
    </message>
    <message>
        <location/>
        <source>Noise Suppression</source>
        <translation>ノイズ抑制</translation>
    </message>
    <message>
        <location/>
        <source>Noise suppression</source>
        <translation>ノイズ抑制</translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets the ammount of noise suppression to apply.&lt;/b&gt;&lt;br /&gt;The higher this value, the more aggressively stationary noise will be supressed.</source>
        <translation type="obsolete">&lt;b&gt;ノイズ抑制の量を設定します。&lt;/b&gt;&lt;br /&gt;値を大きくすると、より積極的に定常ノイズを抑制しようとします。</translation>
    </message>
    <message>
        <location/>
        <source>Amplification</source>
        <translation>音量の増幅</translation>
    </message>
    <message>
        <location/>
        <source>Maximum amplification of input sound</source>
        <translation>音声入力の最大増幅量</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Maximum amplification of input.&lt;/b&gt;&lt;br /&gt;Mumble normalizes the input volume before compressing, and this sets how much it&apos;s allowed to amplify.&lt;br /&gt;The actual level is continually updated based on your current speech pattern, but it will never go above the level specified here.&lt;br /&gt;If the &lt;i&gt;Microphone loudness&lt;/i&gt; level of the audio statistics hover around 100%, you probably want to set this to 2.0 or so, but if, like most people, you are unable to reach 100%, set this to something much higher.&lt;br /&gt;Ideally, set it so &lt;i&gt;Microphone Loudness * Amplification Factor &gt;= 100&lt;/i&gt;, even when you&apos;re speaking really soft.&lt;br /&gt;&lt;br /&gt;Note that there is no harm in setting this to maximum, but Mumble will start picking up other conversations if you leave it to auto-tune to that level.</source>
        <translation>&lt;b&gt;音声入力の最大増幅量です。&lt;/b&gt;&lt;br /&gt;Mumbleは圧縮を行う前に入力量を正常化します。この設定は、どれくらい音量を増幅させるかを決定します。&lt;br /&gt;実際の音量はその時の発言パターンによって変化しますが、ここで指定された音量を上回ることはありません。&lt;br /&gt;音声統計で&lt;i&gt;マイクの音量&lt;/i&gt;レベルが常に100%近くに達しているなら、この値を2.0くらいにしたくなるかも知れません。しかし、(大部分の人がそうだと思いますが)100%に届かないなら、この値をより高くしてください。&lt;br /&gt;あなたが非常に穏やかに話している時に&lt;i&gt;マイク音量 * 増幅値 &gt;= 100&lt;/i&gt; となるのが理想的です。&lt;br /&gt;&lt;br /&gt;この値を最大にすることに実害はありませんが、だからといって適当に最大値に合わせてしまうと他の会話まで拾ってしまうので気をつけてください。</translation>
    </message>
    <message>
        <location/>
        <source>Current speech detection chance</source>
        <translation>現在の発言検出見込み</translation>
    </message>
    <message>
        <location/>
        <source>Audible audio cue when push-to-talk is activated or deactivated</source>
        <translation>&quot;プッシュトーク&quot;の設定の時、ショートカットキーが押される/離されるといったタイミングで音による通知を行います</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets speech detection to use Signal to Noise ratio.&lt;/b&gt;&lt;br /&gt;In this mode, the input is analyzed for something resembling a clear signal, and the clarity of that signal is used to trigger speech detection.</source>
        <translation>&lt;b&gt;S/N比を使用して発言認識を設定します。&lt;/b&gt;&lt;br /&gt;このモードでは、入力は明らかな信号に似た何かに分析され、その信号の明快さは、音声検出ののために使用されます。</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This shows the current speech detection settings.&lt;/b&gt;&lt;br /&gt;You can change the settings from the Settings dialog or from the Audio Wizard.</source>
        <translation>&lt;b&gt;現在の発言検出設定を表します。&lt;/b&gt;&lt;br /&gt;設定ダイアログか音声ウィザードから設定を変更することができます。</translation>
    </message>
    <message>
        <location/>
        <source>Idle AutoMute</source>
        <translation>自動ミュートまでの待ち時間</translation>
    </message>
    <message>
        <location/>
        <source>How long to remain idle before auto-mute.</source>
        <translation>自動ミュートまで必要なアイドル時間。</translation>
    </message>
    <message>
        <location/>
        <source>This sets an idle timer. If the timer expires without any audio being sent to the server, you are muted.</source>
        <translation>アイドリング時間を設定します。指定時間のあいだ、サーバへ何の音声も送信されなかった場合、ミュートになります。</translation>
    </message>
    <message>
        <location/>
        <source>Maximum bandwidth used for sending audio</source>
        <translation>音声を送信するために使う最大帯域</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the amount of noise suppression to apply.&lt;/b&gt;&lt;br /&gt;The higher this value, the more aggressively stationary noise will be supressed.</source>
        <translation>&lt;b&gt;ノイズ抑制の量を設定します。&lt;/b&gt;&lt;br /&gt;値を大きくすると、より積極的に定常ノイズを抑制しようとします。</translation>
    </message>
</context>
<context>
    <name>AudioInputDialog</name>
    <message>
        <location filename="AudioConfigDialog.cpp" line="+74"/>
        <source>Continuous</source>
        <translation>常に有効</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Voice Activity</source>
        <translation>声で有効化</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Push To Talk</source>
        <translation>キー押下で有効化</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>Audio Input</source>
        <translation>音声入力</translation>
    </message>
    <message>
        <location line="+79"/>
        <location line="+8"/>
        <source>%1 ms</source>
        <translation>%1 ms</translation>
    </message>
    <message>
        <location line="-2"/>
        <location line="+30"/>
        <source>Off</source>
        <translation>オフ</translation>
    </message>
    <message>
        <location line="-22"/>
        <source>%1 s</source>
        <translation>%1 s</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>-%1 dB</source>
        <translation>-%1 dB</translation>
    </message>
    <message>
        <location line="+58"/>
        <source>%1kbit/s (Audio %2, Position %4, Overhead %3)</source>
        <translation>%1kbit/s (Audio %2, Position %4, Overhead %3)</translation>
    </message>
    <message>
        <location line="-47"/>
        <source>%1 min</source>
        <translation>%1 min</translation>
    </message>
</context>
<context>
    <name>AudioOutput</name>
    <message>
        <location filename="AudioOutput.ui"/>
        <source>Form</source>
        <translation>フォーム</translation>
    </message>
    <message>
        <location/>
        <source>Interface</source>
        <translation>インターフェース</translation>
    </message>
    <message>
        <location/>
        <source>System</source>
        <translation>システム</translation>
    </message>
    <message>
        <location/>
        <source>Output method for audio</source>
        <oldsource>Input method for audio</oldsource>
        <translation>音声入力方法</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is the output method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <oldsource>&lt;b&gt;This is the input method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</oldsource>
        <translation>&lt;b&gt;音声の出力方法です。&lt;/b&gt;&lt;br /&gt;ほとんどの場合DirectSoundで問題ありません。</translation>
    </message>
    <message>
        <location/>
        <source>Device</source>
        <translation>デバイス</translation>
    </message>
    <message>
        <location/>
        <source>Output device for audio</source>
        <translation>音声のための出力デバイス</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is the output device to use for audio.&lt;/b&gt;</source>
        <translation>&lt;b&gt;これは音声を出力するために使うデバイスです。&lt;/b&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Positional Audio</source>
        <translation>位置音響を使う</translation>
    </message>
    <message>
        <location/>
        <source>Audio Output</source>
        <translation>音声出力</translation>
    </message>
    <message>
        <location/>
        <source>Default &amp;Jitter Buffer</source>
        <translation>ジッタバッファのデフォルト(&amp;J)</translation>
    </message>
    <message>
        <location/>
        <source>Safety margin for jitter buffer</source>
        <translation>ジッタバッファの安全マージン</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the minimum safety margin for the jitter buffer.&lt;/b&gt;&lt;br /&gt;All incoming audio is buffered, and the jitter buffer continually tries to push the buffer to the minimum sustainable by your network, so latency can be as low as possible. This sets the minimum buffer size to use. If the start of sentences you hear is very jittery, increase this value.</source>
        <translation>&lt;b&gt;ジッタバッファのための最小の安全マージンを設定します。&lt;/b&gt;&lt;br /&gt;全ての受信音声は一時保存されます。ジッタバッファは、現在のネットワークの状態に合わせて一時保存の領域を小さくしようとし続けるので、レイテンシーは可能な限り小さくなります。この設定は、使用する最小のバッファサイズを決定します。聞こえてくる会話の出だしが不安定なら、この値を増やしてください。</translation>
    </message>
    <message>
        <location/>
        <source>TextLabel</source>
        <translation>テキストラベル</translation>
    </message>
    <message>
        <location/>
        <source>Volume</source>
        <translation>音量</translation>
    </message>
    <message>
        <location/>
        <source>Volume of incoming speech</source>
        <translation>受信音声の音量</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This adjusts the volume of incoming speech.&lt;/b&gt;&lt;br /&gt;Note that if you increase this beyond 100%, audio will be distorted.</source>
        <translation>&lt;b&gt;受信する音声の音量を調整します。&lt;/b&gt;&lt;br /&gt;ここでは、音量を下げる事のみが可能です。増やす事は出来ません。</translation>
    </message>
    <message>
        <location/>
        <source>Output Delay</source>
        <translation>出力猶予</translation>
    </message>
    <message>
        <location/>
        <source>Amount of data to buffer</source>
        <translation>バッファデータ量</translation>
    </message>
    <message>
        <location/>
        <source>This sets the amount of data to prebuffer in the output buffer. Experiment with different values and set it to the lowest which doesn&apos;t cause rapid jitter in the sound.</source>
        <translation>出力バッファにおけるプリバッファの量を設定します。いろいろな値で試してみて、急に不安定にならない一番小さな値に設定してください。</translation>
    </message>
    <message>
        <source>Volume attenuation</source>
        <translation type="obsolete">ボリューム減衰</translation>
    </message>
    <message>
        <source>MinDistance</source>
        <translation type="obsolete">最小距離</translation>
    </message>
    <message>
        <source>Minimum distance to player before sound decreases</source>
        <translation type="obsolete">音が小さくなる前のプレイヤーへの最小距離</translation>
    </message>
    <message>
        <location/>
        <source>This sets the minimum distance for sound calculations. The volume of other players&apos; speech will not decrease until they are at least this far away from you.</source>
        <translation>サウンドの計算に使う最小距離を設定します。少なくともこの距離だけ離れていれば、他のプレイヤーの音声は減衰しません。</translation>
    </message>
    <message>
        <source>MaxDistance</source>
        <translation type="obsolete">最大距離</translation>
    </message>
    <message>
        <source>Maximum distance, beyond which sound won&apos;t decrease</source>
        <translation type="obsolete">音が小さくならない最大距離</translation>
    </message>
    <message>
        <source>This sets the maximum distance for sound calculations. When farther away than this, other players&apos; sound volume will not decrease any more.</source>
        <translation type="obsolete">サウンドの計算に使う最大距離を設定します。これよりも遠くなると、他のプレイヤーの音声はこれ以上減衰しなくなります。</translation>
    </message>
    <message>
        <source>RollOff</source>
        <translation type="obsolete">ロールオフ</translation>
    </message>
    <message>
        <location/>
        <source>Factor for sound volume decrease</source>
        <translation>ボリューム減少の割合</translation>
    </message>
    <message>
        <source>How fast should sound volume drop when passing beyond the minimum distance. The normal (1.0) is that sound volume halves each time the distance doubles. Increasing this value means sound volume drops faster, while decreasing it means it drops slower.</source>
        <translation type="obsolete">最小距離を超えた時、音量がどのくらい落ちるかについての値です。普通(1.0)の設定では距離が2倍になるたびに音量は半分になります。これを大きくすると音量の落ち込みが速くなり、小さくすると音量の落ち込みは遅くなります。</translation>
    </message>
    <message>
        <location/>
        <source>Bloom</source>
        <translation>ブルーム</translation>
    </message>
    <message>
        <location/>
        <source>Loopback Test</source>
        <translation>ループバックテスト</translation>
    </message>
    <message>
        <location/>
        <source>Delay Variance</source>
        <translation>レイテンシー調整</translation>
    </message>
    <message>
        <location/>
        <source>Variance in packet latency</source>
        <translation>パケットレイテンシーの変動値</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the packet latency variance for loopback testing.&lt;/b&gt;&lt;br /&gt;Most audio paths contain some variable latency. This allows you set that variance for loopback mode testing. For example, if you set this to 15ms, this will emulate a network with 20-35ms ping latency or one with 80-95ms latency. Most domestic net connections have a variance of about 5ms</source>
        <oldsource>&lt;b&gt;This sets the packet latency variance for loopback testing&lt;/b&gt;&lt;br /&gt;Most audio paths contain some variable latency. This allows you set that variance for loopback mode testing. For example, if you set this to 15ms, this will emulate a network with 20-35ms ping latency or one with 80-95ms latency. Most domestic net connections have a variance of about 5ms</oldsource>
        <translation>&lt;b&gt;ループバックテストの為のパケットレイテンシーの値を調整します。&lt;/b&gt;&lt;br /&gt;音声のネットワーク経路は、ほとんどの場合若干のレイテンシーを含んでいます。 この設定はループバックモードでのテスト時、レイテンシーの値を変更できるようにします。例えば、この値を15msにすると、20-35msのレイテンシーや80ms-95msのレイテンシーを持ったネットワークをエミュレートするでしょう。 国内のネット接続ではおよそ5msくらいのレイテンシーがあります</translation>
    </message>
    <message>
        <location/>
        <source>Packet Loss</source>
        <translation>パケットロス</translation>
    </message>
    <message>
        <location/>
        <source>Packet loss for loopback mode</source>
        <translation>ループバックモードでのパケットロス</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the packet loss for loopback mode.&lt;/b&gt;&lt;br /&gt;This will be the ratio of packets lost. Unless your outgoing bandwidth is peaked or there&apos;s something wrong with your network connection, this will be 0%</source>
        <translation>&lt;b&gt;ループバックモードでのパケットロスを設定します。&lt;/b&gt;&lt;br /&gt;これはパケットロスの割合です。送信方向の帯域幅のピークに達するか、ネットワーク接続に問題がない限り、これは0%のはずです</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Loopback</source>
        <translation>ループバック(&amp;L)</translation>
    </message>
    <message>
        <location/>
        <source>Desired loopback mode</source>
        <translation>使用したいループバックモード</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This enables one of the loopback testmodes.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;None&lt;/i&gt; - Loopback disabled&lt;br /&gt;&lt;i&gt;Local&lt;/i&gt; - Emulate a local server.&lt;br /&gt;&lt;i&gt;Server&lt;/i&gt; - Request loopback from server.&lt;br /&gt;Please note than when loopback is enabled, no other players will hear your voice. This setting is not saved on application exit.</source>
        <translation>&lt;b&gt;ループバックテストモードの一つが有効になります。&lt;/b&gt;&lt;br /&gt;&lt;i&gt;なし&lt;/i&gt; - ループバックしない&lt;br /&gt;&lt;i&gt;ローカル&lt;/i&gt; - ローカルなサーバをエミュレートします。&lt;br /&gt;&lt;i&gt;サーバ&lt;/i&gt; - サーバにループバックを要求します。&lt;br /&gt;ループバックが有効になっている間、他のプレイヤーはあなたの声を聞くことが出来ないのでご注意ください。この設定はアプリケーション終了時に保存されません。</translation>
    </message>
    <message>
        <location/>
        <source>Factor for sound volume increase</source>
        <translation>ボリューム増加の割合</translation>
    </message>
    <message>
        <location/>
        <source>The connected &quot;speakers&quot; are actually headphones.</source>
        <translation>接続している &quot;出力機器&quot; はヘッドホンです。</translation>
    </message>
    <message>
        <location/>
        <source>Minimum Distance</source>
        <translation>最小距離</translation>
    </message>
    <message>
        <location/>
        <source>Minimum distance to player before sound volume decreases</source>
        <translation>サウンドのボリュームが減る前のプレイヤに対する最小の距離です</translation>
    </message>
    <message>
        <location/>
        <source>Maximum Distance</source>
        <translation>最大距離</translation>
    </message>
    <message>
        <location/>
        <source>Maximum distance, beyond which speech volume won&apos;t decrease</source>
        <translation>音声のボリュームが減少しない最大の距離</translation>
    </message>
    <message>
        <location/>
        <source>This sets the maximum distance for sound calculations. When farther away than this, other players&apos; speech volume will not decrease any further.</source>
        <translation>音の計算のために最大距離の設定します。 これよりより遠くに離れているとき、他のプレーヤーのスピーチボリュームはこれ以上減少しないでしょう。</translation>
    </message>
    <message>
        <location/>
        <source>Minimum Volume</source>
        <translation>最小音量</translation>
    </message>
    <message>
        <location/>
        <source>What should the volume be at the maximum distance?</source>
        <translation>ボリュームは最大距離がどのくらいであるべきか?</translation>
    </message>
    <message>
        <location/>
        <source>How much should sound volume increase for sources that are really close?</source>
        <translation>入力ソースの近さによってどのくらいボリュームを上げるか?</translation>
    </message>
    <message>
        <source>Connected &quot;speakers&quot; are actually headphones.</source>
        <translation type="obsolete">接続している &quot;出力機器&quot; はヘッドホンです。</translation>
    </message>
    <message>
        <location/>
        <source>Checking this indicates that you don&apos;t have speakers connected, just headphones. This is important, as speakers are usually in front of you, while headphones are directly to your left/right.</source>
        <translation>スピーカーを接続しておらず、ヘッドホンだけであることを確認してください。ヘッドホンはあなたからまっすぐ右と左に位置しているのに対し、スピーカーは一般的にあなたの正面に位置しているため、これは重要になります。</translation>
    </message>
    <message>
        <location/>
        <source>Headphones</source>
        <translation>ヘッドホン</translation>
    </message>
    <message>
        <location/>
        <source>Other Applications</source>
        <translation>他のアプリケーションの音量</translation>
    </message>
    <message>
        <location/>
        <source>Volume of other applications during speech</source>
        <translation>発言中の他のアプリケーションの音量</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Decrease volume of other applications during speech.&lt;/b&gt;&lt;br /&gt;Mumble supports decreasing the volume of other applications during incoming speech. This sets the relative volume of other applications when others are talking.</source>
        <translation>&lt;b&gt;発言中における他のアプリケーションの音量減少&lt;/b&gt;&lt;br /&gt;Mumbleは音声受信中に他のアプリケーションの音量を下げるのを手助けします。ここでは、あなたが話している間の他アプリケーションの音量比率を設定します。</translation>
    </message>
</context>
<context>
    <name>AudioOutputDialog</name>
    <message>
        <location filename="AudioConfigDialog.cpp" line="+119"/>
        <source>None</source>
        <translation>なし</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Local</source>
        <translation>ローカル</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Server</source>
        <translation>サーバ</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Audio Output</source>
        <translation>音声出力</translation>
    </message>
    <message>
        <location line="+93"/>
        <location line="+19"/>
        <source>%1 ms</source>
        <translation>%1 ms</translation>
    </message>
    <message>
        <location line="-8"/>
        <location line="+4"/>
        <location line="+8"/>
        <location line="+31"/>
        <location line="+4"/>
        <source>%1%</source>
        <translation>%1%</translation>
    </message>
    <message>
        <location line="-31"/>
        <source>%1ms</source>
        <translation>%1ms</translation>
    </message>
    <message>
        <location line="+15"/>
        <location line="+6"/>
        <source>%1m</source>
        <translation>%1m</translation>
    </message>
    <message>
        <source>%1</source>
        <translation type="obsolete">%1</translation>
    </message>
</context>
<context>
    <name>AudioStats</name>
    <message>
        <location filename="AudioStats.ui"/>
        <source>Peak microphone level</source>
        <translation>最大マイクレベル</translation>
    </message>
    <message>
        <location/>
        <source>Peak speaker level</source>
        <translation>最大スピーカーレベル</translation>
    </message>
    <message>
        <location/>
        <source>Peak clean level</source>
        <translation>最大クリーンレベル</translation>
    </message>
    <message>
        <source>Microphone loudness</source>
        <translation type="obsolete">マイク音量</translation>
    </message>
    <message>
        <source>Mic Signal-To-Noise</source>
        <translation type="obsolete">マイクS/N比</translation>
    </message>
    <message>
        <location/>
        <source>Speech Probability</source>
        <translation>発言確率</translation>
    </message>
    <message>
        <source>Audio bitrate</source>
        <translation type="obsolete">音声ビットレート</translation>
    </message>
    <message>
        <location/>
        <source>Peak power in last frame</source>
        <translation>直近フレームでの最大の強さ</translation>
    </message>
    <message>
        <source>This shows the peak power in the last frame (20 ms), and is the same measurement as you would usually find displayed as &quot;input power&quot;. Please disregard this and look at &lt;b&gt;Loudness&lt;/b&gt; instead, which is much more steady and disregards outliers.</source>
        <translation type="obsolete">直近のフレーム(20 ms)での最大音量さを表示します。そしてこれは、通常&quot;入力の強さ&quot;として表示されている測定値と同じものです。これは無視して&lt;b&gt;音量&lt;/b&gt;を見るようにしてください。こちらの方がより安定していて外部要素に影響されにくいです。</translation>
    </message>
    <message>
        <location/>
        <source>This shows the peak power of the speakers in the last frame (20 ms). Unless you are using a multi-channel sampling method (such as ASIO) with speaker channels configured, this will be 0. If you have such a setup configured, and this still shows 0 while you&apos;re playing audio from other programs, your setup is not working.</source>
        <oldsource>This shows the peak power in the last frame (20 ms) of the speakers. Unless you are using a multi-channel sampling method (such as ASIO) with speaker channels configured, this will be 0. If you have such a setup configured, and this still shows 0 while you&apos;re playing audio from other programs, your setup is not working.</oldsource>
        <translation>直近のフレーム(20 ms)におけるスピーカーの最大音量を表示します。スピーカの設定を変更して多重チャンネルサンプリング法(例えばASIO)を使用していない限り、これは0になります。セットアップでそのような構成をしているのに、ほかのプログラムが音声を再生している間も0を表示しているなら、セットアップは正しく動いていません。</translation>
    </message>
    <message>
        <source>This shows the peak power in the last frame (20 ms) after all processing. Ideally, this should be -96 dB when you&apos;re not talking. In reality, a sound studio should see -60 dB, and you should hopefully see somewhere around -20 dB. When you are talking, this should rise to somewhere between -5 and -10 dB.&lt;br /&gt;If you are using echo cancellation, and this rises to more than -15 dB when you&apos;re not talking, your setup is not working, and you&apos;ll annoy other players with echoes.</source>
        <translation type="obsolete">直近のフレーム(20 ms)における全ての処理後の最大音量を表示します。あなたが話していないとき、これは -96 dBであるのが理想です。しかし実際には音楽スタジオでも -60 dB、あなたの環境では -20 dB くらい出れば良い方でしょう。あなたが話している時は、-5 から -10 dB のあたりまで上がります。&lt;br /&gt;もしあなたがエコーキャンセルを使っていて、話してもいないのに -15 dB まで上がるようならセットアップは失敗しています。この場合、反響によってほかのプレイヤーを悩ますことになります。</translation>
    </message>
    <message>
        <location/>
        <source>How close the current input level is to ideal</source>
        <translation>入力レベルの理想値までの差</translation>
    </message>
    <message>
        <source>This shows how close your current input volume is to the ideal. To adjust your microphone level, open whatever program you use to adjust the recording volume, and look at the value here while talking.&lt;br /&gt;&lt;b&gt;Talk loud, as you would when you&apos;re upset over getting fragged by a noob.&lt;/b&gt;&lt;br /&gt;Adjust the volume until this value is close to 100%, but make sure it doesn&apos;t go above. If it does go above, you are likely to get clipping in parts of your speech, which will degrade sound quality</source>
        <translation type="obsolete">現在の入力音量が理想の値からどれくらい離れているかを表示します。マイクの音量を調整するためには、録音ボリュームを調整するプログラムを開いて、話している間この値を見てください。&lt;br /&gt;&lt;b&gt;noob(訳注: 初心者の事)にやられて怒り狂っているときのように大声で叫んでください。&lt;/b&gt;&lt;br /&gt;この値が100%近くになるまで音量を調整してください。しかし100%を超えないように注意しましょう。もし超えてしまうとあなたの音声の一部が省略されて、音質が低下するでしょう。</translation>
    </message>
    <message>
        <location/>
        <source>Signal-To-Noise ratio from the microphone</source>
        <translation>マイクからのS/N比</translation>
    </message>
    <message>
        <location/>
        <source>This is the Signal-To-Noise Ratio (SNR) of the microphone in the last frame (20 ms). It shows how much clearer the voice is compared to the noise.&lt;br /&gt;If this value is below 1.0, there&apos;s more noise than voice in the signal, and so quality is reduced.&lt;br /&gt;There is no upper limit to this value, but don&apos;t expect to see much above 40-50 without a sound studio.</source>
        <translation>直近のフレーム(20 ms)におけるマイクの信号対雑音比(S/N比)です。これは、声がノイズと比較してどれくらいはっきりしているかを表しています。&lt;br /&gt;もしこの値が1.0以下なら、信号の中に声よりもノイズのほうが多くなっており、音質は低下しているでしょう。&lt;br /&gt;この値に上限はありませんが、音楽スタジオでもなければ40-50を大きく上回るとは思わないでください。</translation>
    </message>
    <message>
        <location/>
        <source>Probability of speech</source>
        <translation>発言確率</translation>
    </message>
    <message>
        <location/>
        <source>This is the probability that the last frame (20 ms) was speech and not environment noise.&lt;br /&gt;Voice activity transmission depends on this being right. The trick with this is that the middle of a sentence is always detected as speech; the problem is the pauses between words and the start of speech. It&apos;s hard to distinguish a sigh from a word starting with &apos;h&apos;.&lt;br /&gt;If this is in bold font, it means Mumble is currently transmitting (if you&apos;re connected).</source>
        <translation>直近のフレーム(20 ms)が発言であったり、環境ノイズ以外の音であった確率です。&lt;br /&gt;声で有効化する送信方式はこの値が正しい事に依存します。この事による優れた点は話の途中は常に発言と見なされることです。問題は、単語と発言開始の間にある休止です。ため息と&apos;h&apos;で始まる単語を見分けるのは難しい事です。&lt;br /&gt;これが太字になっていたら、Mumbleが現在送信中である事を表しています(接続中の場合)。</translation>
    </message>
    <message>
        <location/>
        <source>Bitrate of last frame</source>
        <translation>直近フレームのビットレート</translation>
    </message>
    <message>
        <source>This is the audio bitrate of the last compressed frame (20 ms), and as such will jump up and down as the VBR adjusts the quality. To adjust the peak bitrate, adjust &lt;b&gt;Compression Complexity&lt;/b&gt; in the Settings dialog.</source>
        <translation type="obsolete">直近の圧縮されたフレーム(20 ms)における音声ビットレートです。VBRが品質を調整するので値は上下します。最大ビットレートを調整するには、設定ダイアログで&lt;b&gt;圧縮 計算量&lt;/b&gt;を調整してください。</translation>
    </message>
    <message>
        <location/>
        <source>Power spectrum of input signal and noise estimate</source>
        <translation>入力信号とノイズ予想のパワースペクトル</translation>
    </message>
    <message>
        <location/>
        <source>This shows the power spectrum of the current input signal (red line) and the current noise estimate (filled blue).&lt;br /&gt;All amplitudes are multiplied by 30 to show the interesting parts (how much more signal than noise is present in each waveband).&lt;br /&gt;This is probably only of interest if you&apos;re trying to fine-tune noise conditions on your microphone. Under good conditions, there should be just a tiny flutter of blue at the bottom. If the blue is more than halfway up on the graph, you have a seriously noisy environment.</source>
        <translation>現在の入力信号(赤線)とノイズ予想(青線)のパワースペクトルを表します。&lt;br /&gt;すべての振幅は判りやすい結果(各周波帯でノイズよりどれくらい多くの信号が存在するか)を提供するために30倍されます。&lt;br /&gt;マイクの雑音状況を改善しようと微調整しているならば、多分に興味深いものとなるでしょう。良好な状態では、一番下に小さく揺れている青線だけがあるべきです。もし青線がグラフの中ほどより上にあるなら、あなたは深刻にノイズの多い環境にあると言えます。</translation>
    </message>
    <message>
        <location/>
        <source>Weights of the echo canceller</source>
        <translation>エコーキャンセラの重み</translation>
    </message>
    <message>
        <location/>
        <source>This shows the weights of the echo canceller, with time increasing downwards and frequency increasing to the right.&lt;br /&gt;Ideally, this should be black, indicating no echo exists at all. More commonly, you&apos;ll have one or more horizontal stripes of bluish color representing time delayed echo. You should be able to see the weights updated in real time.&lt;br /&gt;Please note that as long as you have nothing to echo off, you won&apos;t see much useful data here. Play some music and things should stabilize. &lt;br /&gt;You can choose to view the real or imaginary parts of the frequency-domain weights, or alternately the computed modulus and phase. The most useful of these will likely be modulus, which is the amplitude of the echo, and shows you how much of the outgoing signal is being removed at that time step. The other viewing modes are mostly useful to people who want to tune the echo cancellation algorithms.&lt;br /&gt;Please note: If the entire image fluctuates massively while in modulus mode, the echo canceller fails to find any correlation whatsoever between the two input sources (speakers and microphone). Either you have a very long delay on the echo, or one of the input sources is configured wrong.</source>
        <translation>下に向かっている時間と右に向かっている周波数でエコーキャンセラの重みを表示します。&lt;br /&gt;理想的には、これは反響が全くない事を示す黒でなくてはなりません。普通は、反響が遅れる時間を表す青っぽい色の横縞が一つ以上あるでしょう。あなたは、重みがリアルタイムに更新されるのを見る事ができます。&lt;br /&gt;反響しないものがない限り、ここでは何も有用なデータが得られない事に注意してください。安定した状態で何曲かの音楽を再生してください。&lt;br /&gt;あなたは周波数領域における重みの実数または虚数の部分、もしくは計算された係数と位相の表示を選んで見ることができます。これらで最も役に立つのはおそらく係数です。係数とは反響の大きさであり、送信方向の信号が一定時間毎にどのくらい取り除かれているかを表します。他の表示は主にエコーキャンセラアルゴリズムを調整したい人にとって役に立ちます。&lt;br /&gt;注意： 係数モードの時に全ての表示が大幅に変動する場合、エコーキャンセラは2つの入力源(スピーカーとマイク)の間にどんな相関関係も見出せていません。反響に非常に長い遅れがあるか、入力源のうち1つが誤って設定されているかのどちらかでしょう。</translation>
    </message>
    <message>
        <source>Mumble</source>
        <translation type="obsolete">Mumble</translation>
    </message>
    <message>
        <source>Real</source>
        <translation type="obsolete">実数</translation>
    </message>
    <message>
        <source>Imaginary</source>
        <translation type="obsolete">虚数</translation>
    </message>
    <message>
        <source>Modulus</source>
        <translation type="obsolete">係数</translation>
    </message>
    <message>
        <source>Phase</source>
        <translation type="obsolete">位相</translation>
    </message>
    <message>
        <source>&gt;1000ms</source>
        <translation type="obsolete">&gt;1000ms</translation>
    </message>
    <message>
        <location/>
        <source>Audio Statistics</source>
        <translation>音声統計</translation>
    </message>
    <message>
        <location/>
        <source>Input Levels</source>
        <translation>入力レベル</translation>
    </message>
    <message>
        <location/>
        <source>This shows the peak power in the last frame (20 ms), and is the same measurement as you would usually find displayed as &quot;input power&quot;. Please disregard this and look at &lt;b&gt;Microphone power&lt;/b&gt; instead, which is much more steady and disregards outliers.</source>
        <translation>直近のフレーム(20 ms)での最大の強さを表示します。そしてこれは、通常&quot;入力の強さ&quot;として表示されている測定値と同じものです。これは無視して&lt;b&gt;マイクの強さ&lt;/b&gt;を見るようにしてください。こちらの方がより安定していて外部要素に影響されにくいです。</translation>
    </message>
    <message>
        <source>TextLabel</source>
        <translation type="obsolete">テキストラベル</translation>
    </message>
    <message>
        <location/>
        <source>Signal Analysis</source>
        <translation>信号分析</translation>
    </message>
    <message>
        <location/>
        <source>Microphone power</source>
        <translation>マイクの強さ</translation>
    </message>
    <message>
        <location/>
        <source>This shows how close your current input volume is to the ideal. To adjust your microphone level, open whatever program you use to adjust the recording volume, and look at the value here while talking.&lt;br /&gt;&lt;b&gt;Talk loud, as you would when you&apos;re upset over getting fragged by a noob.&lt;/b&gt;&lt;br /&gt;Adjust the volume until this value is close to 100%, but make sure it doesn&apos;t go above. If it does go above, you are likely to get clipping in parts of your speech, which will degrade sound quality.</source>
        <translation>現在の入力音量が理想の値からどれくらい離れているかを表します。マイクの音量を調整するためには、録音ボリュームを調整するプログラムを開いて、話している間この値を見てください。&lt;br /&gt;&lt;b&gt;noob(訳注: 初心者の事)にやられて怒り狂っているときのように大声で叫んでください。&lt;/b&gt;&lt;br /&gt;この値が100%近くになるまで音量を調整してください。しかし100%を超えないように注意しましょう。もし超えてしまうとあなたの音声の一部が省略されて、音質が低下するでしょう。</translation>
    </message>
    <message>
        <location/>
        <source>Signal-To-Noise ratio</source>
        <translation>S/N比</translation>
    </message>
    <message>
        <location/>
        <source>Configuration feedback</source>
        <translation>設定フィードバック</translation>
    </message>
    <message>
        <location/>
        <source>Current audio bitrate</source>
        <translation>現在の音声ビットレート</translation>
    </message>
    <message>
        <location/>
        <source>DoublePush interval</source>
        <translation>二重押しの間隔</translation>
    </message>
    <message>
        <source>Time between last two PushToTalk presses</source>
        <translation type="obsolete">直近2回のショートカットキー押下の間隔</translation>
    </message>
    <message>
        <location/>
        <source>Speech Detection</source>
        <translation>発言検出</translation>
    </message>
    <message>
        <location/>
        <source>Current speech detection chance</source>
        <translation>現在の発言検出見込み</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This shows the current speech detection settings.&lt;/b&gt;&lt;br /&gt;You can change the settings from the Settings dialog or from the Audio Wizard.</source>
        <translation>&lt;b&gt;現在の発言検出設定を表します。&lt;/b&gt;&lt;br /&gt;設定ダイアログか音声ウィザードから設定を変更することができます。</translation>
    </message>
    <message>
        <location/>
        <source>Signal and noise power spectrum</source>
        <translation>信号とノイズのパワースペクトル</translation>
    </message>
    <message>
        <location/>
        <source>Echo Analysis</source>
        <translation>反響分析</translation>
    </message>
    <message>
        <location/>
        <source>This shows the peak power in the last frame (20 ms) after all processing. Ideally, this should be -96 dB when you&apos;re not talking. In reality, a sound studio should see -60 dB, and you should hopefully see somewhere around -20 dB. When you are talking, this should rise to somewhere between -5 and -10 dB.&lt;br /&gt;If you are using echo cancellation, and this rises to more than -15 dB when you&apos;re not talking, your setup is not working, and you&apos;ll annoy other users with echoes.</source>
        <translation>直近のフレーム(20 ms)における全ての処理後の最大音量を表示します。あなたが話していないとき、これは -96 dBであるのが理想です。しかし実際には音楽スタジオでも -60 dB、あなたの環境では -20 dB くらい出れば良い方でしょう。あなたが話している時は、-5 から -10 dB のあたりまで上がります。&lt;br /&gt;もしあなたがエコーキャンセルを使っていて、話してもいないのに -15 dB まで上がるようならセットアップは失敗しています。この場合、反響によってほかのプレイヤーを悩ますことになります。</translation>
    </message>
    <message>
        <location/>
        <source>Time between last two Push-To-Talk presses</source>
        <translation>直近2回のプッシュトークの間隔</translation>
    </message>
    <message>
        <location filename="AudioStats.cpp" line="+405"/>
        <source>&gt;1000 ms</source>
        <translation>&gt;1000ms</translation>
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
        <translation>音声調整ウィザード</translation>
    </message>
    <message>
        <location line="+50"/>
        <source>Introduction</source>
        <translation>イントロダクション</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Welcome to the Mumble Audio Wizard</source>
        <translation>Mumble 音声ウィザードへようこそ</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>This is the audio tuning wizard for Mumble. This will help you correctly set the input levels of your sound card, and also set the correct parameters for sound processing in Mumble.</source>
        <translation>これはMumbleの音声を調整するためのウィザードです。サウンドカードの入力レベルとMumbleでの音声処理のパラメータを正しく設定するお手伝いをします。</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Please be aware that as long as this wizard is active, audio will be looped locally to allow you to listen to it, and no audio will be sent to the server.</source>
        <translation>ウィザードが動作中の間、音声はあなたが聞けるようPC上だけで完結し、サーバには送信されませんのでご注意ください。</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Finished</source>
        <translation>完了</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Enjoy using Mumble</source>
        <translation>Mumbleをお楽しみください</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Congratulations. You should now be ready to enjoy a richer sound experience with Mumble.</source>
        <translation>おめでとうございます！Mumbleでリッチなサウンド体験をする準備が整いました。</translation>
    </message>
    <message>
        <location line="+23"/>
        <source>Device selection</source>
        <translation>デバイス選択</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Selecting the input and output device to use with Mumble.</source>
        <translation>Mumbleで使用する入力および出力のデバイスを選択します。</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Input device</source>
        <translation>入力デバイス</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>This is the device your microphone is connected to.</source>
        <translation>あなたが接続しているデバイスです。</translation>
    </message>
    <message>
        <location line="+14"/>
        <location line="+52"/>
        <source>System</source>
        <translation>システム</translation>
    </message>
    <message>
        <location line="-49"/>
        <source>Input method for audio</source>
        <translation>音声入力方法</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>&lt;b&gt;This is the input method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation>&lt;b&gt;音声の入力方法です。&lt;/b&gt;&lt;br /&gt;ほとんどの場合DirectSoundで問題ありません。</translation>
    </message>
    <message>
        <location line="+12"/>
        <location line="+52"/>
        <source>Device</source>
        <translation>デバイス</translation>
    </message>
    <message>
        <location line="-49"/>
        <source>Input device to use</source>
        <translation>入力に使用するデバイス</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>&lt;b&gt;Selects which sound card to use for audio input.&lt;/b&gt;</source>
        <translation>&lt;b&gt;音声の入力にどのサウンドカードを使用するか選択してください。&lt;/b&gt;</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Output device</source>
        <translation>出力デバイス</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>This is the device your speakers or headphones are connected to.</source>
        <translation>あなたのスピーカ(もしくはヘッドホン)が接続しているデバイスです。</translation>
    </message>
    <message>
        <location line="+19"/>
        <source>Output method for audio</source>
        <translation>音声入力方法</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>&lt;b&gt;This is the Output method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation>&lt;b&gt;音声の出力方法です。&lt;/b&gt;&lt;br /&gt;ほとんどの場合DirectSoundで問題ありません。</translation>
    </message>
    <message>
        <location line="+15"/>
        <source>Output device to use</source>
        <translation>出力に使用するデバイス</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>&lt;b&gt;Selects which sound card to use for audio Output.&lt;/b&gt;</source>
        <translation>&lt;b&gt;音声の出力にどのサウンドカードを使用するか選択してください。&lt;/b&gt;</translation>
    </message>
    <message>
        <location line="+64"/>
        <source>Volume tuning</source>
        <translation>音量調整</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Tuning microphone hardware volume to optimal settings.</source>
        <translation>マイクの音量を最適な設定になるよう調整します。</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Open your sound control panel and go to the recording settings. Make sure the microphone is selected as active input with maximum recording volume. If there&apos;s an option to enable a &quot;Microphone boost&quot; make sure it&apos;s checked.</source>
        <translation>ボリュームコントロールパネルを開き、録音設定画面に移動しましょう。そして、マイクが選択されていて、音量が最大になっている事を確認してください。もし&quot;マイクブースト&quot;を有効にするオプションがあるなら、それもチェックされているか確認してください。</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Speak loudly, as when you are annoyed or excited. Decrease the volume in the sound control panel until the bar below stays as high as possible in the blue and green but &lt;b&gt;not&lt;/b&gt; the red zone while you speak.</source>
        <translation>イライラしていたり、興奮していたりする時のように大声で話してみてください。話している間、下にあるバーが青と緑の区域のなるべく赤に近い場所に留まるまでサウンドコントロールパネルの音量を下げてください。(赤い区域には&lt;b&gt;入らない&lt;/b&gt;ようにしましょう)。</translation>
    </message>
    <message>
        <location line="+11"/>
        <source>Now talk softly, as you would when talking late at night and you don&apos;t want to disturb anyone. Adjust the slider below so that the bar moves into green when you talk, but stays blue while you&apos;re silent.</source>
        <translation>次に、誰も起こさないようにしている夜中のようにそっと話してみましょう。話している間、バーが緑にかかり、静かにしている時は青に留まるように下のスライダーを調整してください。</translation>
    </message>
    <message>
        <location line="+19"/>
        <source>Voice Activity Detection</source>
        <translation>発声の検出</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Letting Mumble figure out when you&apos;re talking and when you&apos;re silent.</source>
        <translation>話している状態と静かにしている状態をMumbleに判別させます。</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>This will help Mumble figure out when you are talking. The first step is selecting which data value to use.</source>
        <translation>あなたがいつ話しているのかをMumbleに判別させるお手伝いをします。まず最初に、どのデータの値を使うかの選択を行います。</translation>
    </message>
    <message>
        <location line="+26"/>
        <source>Raw amplitude from input</source>
        <translation>音声信号の大きさ</translation>
    </message>
    <message>
        <location line="-4"/>
        <source>Signal-To-Noise ratio</source>
        <translation>S/N比</translation>
    </message>
    <message>
        <location line="+27"/>
        <source>Next you need to adjust the following two sliders. The first few utterances you say should end up in the green area (definitive speech). While talking, you should stay inside the yellow (might be speech) and when you&apos;re not talking, everything should be in the red (definitively not speech).</source>
        <translation>次に、以下の2つのスライダーを調節しましょう。最初の発声は緑(確実に発言と判定)になると良いでしょう。話している最中は黄色(発言だろうと判定)の中に収まり、話していない時はすべて赤(発言ではないと判定)に入っているようにしてください。</translation>
    </message>
    <message>
        <location line="+24"/>
        <source>Last, you need to adjust the grace time. Mumble may detect this much non-speech and still keep transmitting before it breaks off. This allows you to catch your breath while speaking.</source>
        <translation>最後に、猶予時間を調整しましょう。発言以外のものを検出する許容量が上がり、話が途切れずに送信を続けることができるでしょう。これにより、話の間に小休止を入れることができるようになります。</translation>
    </message>
    <message>
        <location line="+28"/>
        <source>Device tuning</source>
        <translation>デバイス調整</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Changing hardware output delays to their minimum value.</source>
        <translation>ハードウェアの出力遅延が最小値になるように変更します。</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>To keep latency to an absolute minium, it&apos;s important to buffer as little audio as possible on the soundcard. However, many soundcards report that they require a much smaller buffer than what they can actually work with, so the only way to set this value is to try and fail.</source>
        <translation>遅延を最小限に保つためには、サウンドカード上で音声のバッファを出来るだけ小さくすることが重要です。しかし、多くのサウンドカードが要求してくるバッファは、実際に動作可能な大きさよりかなり小さなものです。結局、この値を設定するにはいろいろ試してみるほかありません。</translation>
    </message>
    <message>
        <location line="+23"/>
        <source>Amount of data to buffer.</source>
        <translation>バッファデータの量。</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This sets the amount of data to prebuffer in the output buffer. Experiment with different values and set it to the lowest which doesn&apos;t cause rapid jitter in the sound.</source>
        <translation>出力バッファにおけるプリバッファの量を設定します。いろいろな値で試してみて、急に不安定にならない一番小さな値に設定してください。</translation>
    </message>
    <message>
        <location line="+109"/>
        <source>%1ms</source>
        <translation>%1ms</translation>
    </message>
    <message>
        <location line="+244"/>
        <source>%1 s</source>
        <translation>%1 s</translation>
    </message>
    <message>
        <location line="-371"/>
        <source>You should hear a single tone that&apos;s changing in frequency. Change the slider below to the lowest value which gives &lt;b&gt;no&lt;/b&gt; interruptions or jitter in the sound. Please note that local echo is disabled during this test to improve audio path recognition.</source>
        <translation>周波数が変わる1つの音色が聞こえますでしょうか。音が途切れたり不安定に&lt;b&gt;ならない&lt;/b&gt;一番小さな値まで下のスライダーを動かしてください。なお、音声経路の認識率を向上するため、このテストの間ローカルエコーは使えなくなっている事をお忘れなく。</translation>
    </message>
    <message>
        <location line="+24"/>
        <source>If you position the microphone so it can pick up the speakers or headset, Mumble will measure the total audio path delay in your system; this means the delay from a sample is placed in an outbound buffer until it&apos;s found in a matching incoming buffer.</source>
        <translation>スピーカーやヘッドセットの出力をマイクが拾ってしまうような配置にしている場合、あなたのシステムで音声経路の遅延が起きていると判断されるでしょう; つまり、対応する受信バッファ内に含まれるまでサンプルが送信バッファに放置されて、遅延をもたらします。</translation>
    </message>
    <message>
        <location line="+294"/>
        <source>Audio path is %1ms long.</source>
        <translation>音声経路は %1ms の長さ。</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Audio path cannot be determined. Input not recognized.</source>
        <translation>音声経路が決定できません。入力が不明です。</translation>
    </message>
    <message>
        <location line="-592"/>
        <source>Use echo cancellation</source>
        <translation>エコーキャンセルを使う</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Cancel echo from headset or speakers.</source>
        <translation>ヘッドセットやスピーカーからのエコーをキャンセルします。</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This enables echo cancellation of outgoing audio, which helps both on speakers and on headsets.</source>
        <translation>出力音声のエコーキャンセルを有効にします。スピーカーとヘッドセットの両方に対して有効です。</translation>
    </message>
    <message>
        <location line="+50"/>
        <source>Enable positional audio</source>
        <translation>位置音響を有効にする</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Allows positioning of sound.</source>
        <translation>音声の位置。</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This allows Mumble to use positional audio to place voices.</source>
        <translation>声の位置を定めるために位置音響を使えるようにします。</translation>
    </message>
    <message>
        <location line="+17"/>
        <source>Positional Audio</source>
        <translation>位置音響を使う</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Adjusting attenuation of positional audio.</source>
        <translation>位置音響の減衰効果を調整します。</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Mumble supports positional audio for some games, and will position the voice of other players relative to their position in game. Depending on their position, the volume of the voice will be changed between the speakers to simulate the direction and distance the other player is at. Such positioning depends on your speaker configuration being correct in your operating system, so a test is done here.</source>
        <translation>Mumbleは、いくつかのゲームにおいてゲーム内での他プレイヤーの位置に対応した位置音響をサポートします。他プレイヤーの位置によって、方向と距離をシミュレートして音量が変化します。このシミュレーションはお使いのオペレーティング・システムでスピーカーが正しく設定されていることが前提となります。そのテストはここで行うことが出来ます。</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>The graph below shows the position of &lt;font color=&quot;red&quot;&gt;you&lt;/font&gt;, the &lt;font color=&quot;yellow&quot;&gt;speakers&lt;/font&gt; and a &lt;font color=&quot;green&quot;&gt;moving sound source&lt;/font&gt; as if seen from above. You should hear the audio move between the channels.</source>
        <translation>このグラフはこれらの位置を表しています: &lt;font color=&quot;red&quot;&gt;あなた&lt;/font&gt;, the &lt;font color=&quot;yellow&quot;&gt;スピーカー&lt;/font&gt; and a &lt;font color=&quot;green&quot;&gt;移動する音の発生源&lt;/font&gt; 上からに見えるようなら、チャンネルの間に移動して聞くのがよいでしょう。</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Use headphones</source>
        <translation>ヘッドホンを使う</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Use headphones instead of speakers.</source>
        <translation>スピーカーの代わりにヘッドホンを使う。</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This ignores the OS speaker configuration and configures the positioning for headphones instead.</source>
        <translation>OSのスピーカー構成を無視して、代わりにヘッドホン用の位置設定を使用します。</translation>
    </message>
    <message>
        <location line="-153"/>
        <source>Mumble is under continuous development, and the development team wants to focus on the features that benefit the most users. To this end, Mumble supports submitting anonymous statistics about your configuration to the developers. These statistcs are essential for future development, and also make sure the features you use aren&apos;t deprecated.</source>
        <translation>Mumbleは現在開発中であり、開発チームはより多くのユーザの利益となる機能に注力したいと考えています。そのため、匿名で設定に関する統計を送信する機能を追加しました。これらの統計情報は主に将来の開発のために使用されるか、特定の機能が使用されていないかどうかを確認するために使用されます。</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Submit anonymous statistics to the Mumble project</source>
        <translation>Mumbleプロジェクトに匿名で統計を送信する</translation>
    </message>
    <message>
        <location line="+224"/>
        <source>Push To Talk:</source>
        <translation>プッシュトーク:</translation>
    </message>
</context>
<context>
    <name>BanEditor</name>
    <message>
        <location filename="BanEditor.ui"/>
        <source>Mumble - Edit Bans</source>
        <translation>Mumble - Ban編集</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Add</source>
        <translation>追加(&amp;A)</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Update</source>
        <translation>更新(&amp;U)</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Remove</source>
        <translation>削除(&amp;R)</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>Accept changes</source>
        <translation type="obsolete">変更を確定</translation>
    </message>
    <message>
        <source>This button will accept current groups/ACLs and send them to the server. Note that if you mistakenly remove write permission from yourself, the server will add it.</source>
        <translation type="obsolete">このボタンは現在の グループとACLを 適用しサーバーに送信します。もし誤ってあなた自身の書き込み権限を削除してしまうとサーバーに追加されてしまいますのでご注意ください。</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">キャンセル(&amp;C)</translation>
    </message>
    <message>
        <source>Reject changes</source>
        <translation type="obsolete">変更を破棄</translation>
    </message>
    <message>
        <source>This button will cancels all changes and closes the dialog without updating the ACLs or groups on the server.</source>
        <translation type="obsolete">このボタンは全ての変更をキャンセルし、サーバーのACLやグループを変更せずにダイアログを閉じます。</translation>
    </message>
    <message>
        <location/>
        <source>0.0.0.0</source>
        <translation>0.0.0.0</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Address</source>
        <translation>アドレス(&amp;A)</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Mask</source>
        <translation>ネットマスク(&amp;M)</translation>
    </message>
</context>
<context>
    <name>ChanACL</name>
    <message>
        <location filename="ACL.cpp" line="+49"/>
        <source>W</source>
        <translation>W</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>T</source>
        <translation>T</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>E</source>
        <translation>E</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>S</source>
        <translation>S</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>A</source>
        <translation>A</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>M</source>
        <translation>M</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>K</source>
        <translation>K</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>C</source>
        <translation>C</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>L</source>
        <translation>L</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>None</source>
        <translation>なし</translation>
    </message>
    <message>
        <source>Write</source>
        <translation type="obsolete">ACLの更新</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Traverse</source>
        <translation>トラバース</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Enter</source>
        <translation>チャンネルへの参加</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Speak</source>
        <translation>発言</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>AltSpeak</source>
        <translation>Alt発言</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Mute/Deafen</source>
        <translation>発言禁止/聴取禁止</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Move/Kick</source>
        <translation>移動/キック</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Make channel</source>
        <translation>チャンネル作成</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Link channel</source>
        <translation>チャンネルをリンク</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>This represents no privileges.</source>
        <translation>権限がないことを表します。</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>This represents total access to the channel, including the ability to change group and ACL information. This privilege implies all other privileges.</source>
        <translation>グループとACL情報の変更を含む、チャンネルへの完全なアクセスを表します。この権限は、必然的に他の全ての権限を含んでいます。</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>This represents the permission to traverse the channel. If a user is denied this privilege, he will be unable to access this channel and any sub-channels in any way, regardless of other permissions in the sub-channels.</source>
        <translation>チャンネル間を移動する権限を表します。あるユーザがこの権限を与えられない場合、サブチャンネルの権限に関わらず、どんな方法でもこのチャンネルおよびサブチャンネルにアクセス出来なくなります。</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>This represents the permission to join the channel. If you have a hierarchical channel structure, you might want to give everyone Traverse, but restrict Enter in the root of your hierarchy.</source>
        <translation>チャンネルに参加する権限を表します。階層的なチャンネル構造がある場合、全ての人にチャンネルアクセスを与えたくなるかもしれませんが、階層の最下部で参加が制限されるかもしれません。</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>This represents the permission to speak in a channel. Users without this privilege will be suppressed by the server (seen as muted), and will be unable to speak until they are unmuted by someone with the appropriate privileges.</source>
        <translation>このチャンネル内で発言する権限を表します。この権限がないユーザはサーバによって発言を抑制され(発言禁止にされているように見えます)、適切な権限を持つ誰かが発言禁止を解くまで話すことができません。</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>This represents the permission to speak in a channel with flagged speech. This works exactly like the &lt;i&gt;speak&lt;/i&gt; privilege, but applies to packets spoken with AltPushToTalk held down. This may be used to broadcast to a hierarchy of channels without linking.</source>
        <translation>このチャンネル内でフラグ付きの発言をする権限を表します。これは&lt;i&gt;発言&lt;/i&gt;権限と全く同じ動作をしますが、Alt+ショートカットキーが押された時のパケットのみに適用されます。これはリンクしていないチャンネルの階層全体に送信をするために使うことが出来ます。</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>This represents the permission to mute and deafen other players. Once muted, a player will stay muted until he is unmuted by another privileged player or reconnects to the server.</source>
        <translation>他の人を発言禁止にしたり、聴取禁止にしたりする権限を表します。一度発言禁止になると、他に権限を持った人が発言禁止を解くか、サーバに接続しなおすまで発言禁止のままになります。</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>This represents the permission to move a player to another channel or kick him from the server. To actually move the player, either the moving player must have Move/Kick privileges in the destination channel, or the player must normally be allowed to enter the channel. Players with this privilege can move players into channels the target player normally wouldn&apos;t have permission to enter.</source>
        <translation>他の人を別のチャンネルに移動させたり、サーバからキックしたりする権限を表します。実際にプレイヤーを動かすには、実行する人が移動先のチャンネルの移動/キック権限を持っているか、プレイヤーが移動先チャンネルに入る権限を持っていなければなりません。この権限を持っている人は、対象のプレイヤーが通常参加を許可されていないチャンネルに移動させることができます。</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>This represents the permission to make sub-channels. The player making the sub-channel will be added to the admin group of the sub-channel.</source>
        <translation>サブチャンネルを作成する権限を表します。サブチャンネルを作った人は、そのチャンネルの管理者グループに追加されます。</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>This represents the permission to link channels. Players in linked channels hear each other, as long as the speaking player has the &lt;i&gt;speak&lt;/i&gt; privilege in the channel of the listener. You need the link privilege in both channels to create a link, but just in either channel to remove it.</source>
        <translation>チャンネルをリンクする権限を表します。話している人が聞き手のチャンネルに&lt;i&gt;発言&lt;/i&gt;の権限を持っている限り、リンクされたチャンネルのプレイヤーは互いに聞く事ができます。</translation>
    </message>
    <message>
        <location line="-57"/>
        <source>Write ACL</source>
        <translation>ACLの上書き</translation>
    </message>
</context>
<context>
    <name>ClientPlayer</name>
    <message>
        <location filename="Player.cpp" line="+53"/>
        <source>Authenticated</source>
        <translation>認証済み</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Muted (server)</source>
        <translation>発言禁止 (server)</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Deafened (server)</source>
        <translation>聴取禁止 (server)</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Local Mute</source>
        <translation>ローカルでの発言禁止</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Muted (self)</source>
        <translation>発言禁止(セルフ)</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Deafened (self)</source>
        <translation>聴取禁止(セルフ)</translation>
    </message>
</context>
<context>
    <name>ConfigDialog</name>
    <message>
        <location filename="ConfigDialog.ui"/>
        <source>Mumble Configuration</source>
        <translation>Mumbleの設定</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="+54"/>
        <source>Accept changes</source>
        <translation>変更を確定</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This button will accept current settings and return to the application.&lt;br /&gt;The settings will be stored to disk when you leave the application.</source>
        <translation>このボタンは現在の設定を適用してアプリケーションに戻ります。&lt;br /&gt;この設定はアプリケーションを終了してもディスクに保存されます。</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">キャンセル(&amp;C)</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Reject changes</source>
        <translation>変更を破棄</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This button will reject all changes and return to the application.&lt;br /&gt;The settings will be reset to the previous positions.</source>
        <translation>このボタンは変更を破棄してアプリケーションに戻ります。&lt;br /&gt;設定は前の状態に戻ります。</translation>
    </message>
    <message>
        <source>&amp;Apply</source>
        <translation type="obsolete">適用(&amp;A)</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Apply changes</source>
        <translation>変更を適用</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This button will immediately apply all changes.</source>
        <translation>全ての変更を、すぐに適用します。</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Undo changes for current page</source>
        <translation>このページの変更を元に戻す</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This button will revert any changes done on the current page to the most recent applied settings.</source>
        <translation>このボタンは、現在のページで最後に適用した変更を元に戻します。</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Restore defaults for current page</source>
        <translation>このページをデフォルト設定に戻す</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This button will restore the defaults for the settings on the current page. Other pages will not be changed.&lt;br /&gt;To restore all settings to their defaults, you will have to use this button on every page.</source>
        <oldsource>This button will restore the settings for the current page only to their defaults. Other pages will not be changed.&lt;br /&gt;To restore all settings to their defaults, you will have to use this button on every page.</oldsource>
        <translation type="unfinished">このボタンは現在のページの設定をデフォルトに戻します。他のページは変更されません。&lt;br /&gt;全ての設定をデフォルト戻すには、各ページでこのボタンを使わなければなりません。</translation>
    </message>
    <message>
        <source>This button will restore the settings for the current page only to their defaults. Other pages will be not be changed.&lt;br /&gt;To restore all settings to their defaults, you will have to use this button on every page.</source>
        <translation type="obsolete">このボタンは現在のページの設定をデフォルトに戻します。他のページは変更されません。&lt;br /&gt;全ての設定をデフォルト戻すには、各ページでこのボタンを使わなければなりません。</translation>
    </message>
    <message>
        <source>Show all configuration items</source>
        <translation type="obsolete">全ての設定項目を表示</translation>
    </message>
    <message>
        <source>&lt;b&gt;This will show all configuration items.&lt;/b&gt;&lt;br /&gt;Mumble contains a lot of configuration items that most users won&apos;t need to change. Checking this will show all configurable items.</source>
        <translation type="obsolete">&lt;b&gt;全ての設定項目を表示します。&lt;/b&gt;&lt;br /&gt;Mumbleは大部分のユーザーが変更する必要のない設定項目を多く含んでいます。ここにチェックを入れると全ての設定項目を表示します。</translation>
    </message>
    <message>
        <source>Expert Config</source>
        <translation type="obsolete">上級者向け設定</translation>
    </message>
    <message>
        <source>Tab 1</source>
        <translation type="obsolete">Tab 1</translation>
    </message>
    <message>
        <location filename="ConfigDialog.ui"/>
        <source>Advanced</source>
        <translation>高度な設定を表示する</translation>
    </message>
</context>
<context>
    <name>ConnectDialog</name>
    <message>
        <source>&amp;Name</source>
        <translation type="obsolete">名前(&amp;N)</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui"/>
        <source>A&amp;ddress</source>
        <translation>アドレス(&amp;D)</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Port</source>
        <translation>ポート(&amp;P)</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Username</source>
        <translation>ユーザ名(&amp;U)</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Password</source>
        <translation>パスワード(&amp;P)</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Connect</source>
        <translation>接続(&amp;C)</translation>
    </message>
    <message>
        <location/>
        <source>Cancel</source>
        <translation>キャンセル</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Add</source>
        <translation>追加(&amp;A)</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Remove</source>
        <translation>削除(&amp;R)</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Custom Servers</source>
        <translation>お気に入りサーバ(&amp;C)</translation>
    </message>
    <message>
        <location/>
        <source>Server &amp;Browser</source>
        <translation>サーバブラウザ(&amp;B)</translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="obsolete">名前</translation>
    </message>
    <message>
        <location/>
        <source>Address</source>
        <translation>アドレス</translation>
    </message>
    <message>
        <location/>
        <source>URL</source>
        <translation>URL</translation>
    </message>
    <message>
        <location/>
        <source>C&amp;opy to custom</source>
        <translation>お気に入りにコピー(&amp;O)</translation>
    </message>
    <message>
        <location/>
        <source>&amp;View Webpage</source>
        <translation>ウェブサイトを見る(&amp;V)</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="+124"/>
        <source>Connecting to %1</source>
        <translation>%1に接続中</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Enter username</source>
        <translation>ユーザ名を入力してください</translation>
    </message>
    <message>
        <source>Mumble</source>
        <translation type="obsolete">Mumble</translation>
    </message>
    <message>
        <location line="+99"/>
        <source>Failed to fetch server list</source>
        <translation>サーバリストを取得出来ませんでした</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui"/>
        <source>&amp;Label</source>
        <translation>ラベル(&amp;L)</translation>
    </message>
    <message>
        <location/>
        <source>Label</source>
        <translation>ラベル</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="-14"/>
        <source>Unknown</source>
        <translation>不明</translation>
    </message>
    <message>
        <location line="+116"/>
        <source>-Unnamed entry-</source>
        <translation>-Unnamed entry-</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui"/>
        <source>Mumble Server Connect</source>
        <translation>Mumbleサーバ接続</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Cancel</source>
        <translation>キャンセル(&amp;C)</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="-60"/>
        <location line="+22"/>
        <source>New</source>
        <translation>新規</translation>
    </message>
    <message>
        <location line="+62"/>
        <location line="+27"/>
        <source>Add</source>
        <translation>追加</translation>
    </message>
    <message>
        <location line="-2"/>
        <source>Update</source>
        <translation>更新</translation>
    </message>
</context>
<context>
    <name>CoreAudioSystem</name>
    <message>
        <location filename="CoreAudio.cpp" line="+83"/>
        <source>Default Device</source>
        <translation>デフォルトデバイス</translation>
    </message>
</context>
<context>
    <name>CrashReporter</name>
    <message>
        <location filename="CrashReporter.cpp" line="+37"/>
        <source>Mumble Crash Report</source>
        <translation></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>&lt;p&gt;&lt;b&gt;We&apos;re terribly sorry, but it seems Mumble has crashed. Do you want to send a crash report to the Mumble developers?&lt;/b&gt;&lt;/p&gt;&lt;p&gt;The crash report contains a partial copy of Mumble&apos;s memory at the time it crashed, and will help the developers fix the problem.&lt;/p&gt;</source>
        <translation></translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Email address (optional)</source>
        <translation></translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Please describe briefly, in English, what you were doing at the time of the crash</source>
        <oldsource>Please briefly describe what you were doing at the time of the crash</oldsource>
        <translation>クラッシュしたときにあなたが何をしていたかを簡潔に&lt;b&gt;英語で&lt;/b&gt;書いてください</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Send Report</source>
        <translation></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Don&apos;t send report</source>
        <translation></translation>
    </message>
    <message>
        <location line="+24"/>
        <source>Crash upload successful</source>
        <translation></translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Thank you for helping make Mumble better!</source>
        <translation></translation>
    </message>
    <message>
        <location line="+2"/>
        <location line="+2"/>
        <source>Crash upload failed</source>
        <translation></translation>
    </message>
    <message>
        <location line="-2"/>
        <source>We&apos;re really sorry, but it appears the crash upload has failed with error %1 %2. Please inform a developer.</source>
        <translation></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>This really isn&apos;t funny, but apparently there&apos;s a bug in the crash reporting code, and we&apos;ve failed to upload the report. You may inform a developer about error %1</source>
        <translation></translation>
    </message>
    <message>
        <location line="+114"/>
        <source>Uploading crash report</source>
        <translation></translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Abort upload</source>
        <translation></translation>
    </message>
</context>
<context>
    <name>DXAudioInput</name>
    <message>
        <source>Mumble</source>
        <translation type="obsolete">Mumble</translation>
    </message>
    <message>
        <source>Opening chosen DirectSound Input failed. No microphone capture will be done.</source>
        <translation type="obsolete">選択されたDirectSound Inputのオープンに失敗しました。マイクからの入力は行われません。</translation>
    </message>
    <message>
        <location filename="DirectSound.cpp" line="+554"/>
        <source>Opening chosen DirectSound Input failed. Default device will be used.</source>
        <translation>選択されたDirectSound Inputのオープンに失敗しました。デフォルトのデバイスが使用されます。</translation>
    </message>
    <message>
        <location line="-382"/>
        <source>Default DirectSound Voice Input</source>
        <translation>デフォルトのDirectSound音声入力</translation>
    </message>
    <message>
        <location line="+461"/>
        <source>Opening chosen DirectSound Input device failed. No microphone capture will be done.</source>
        <translation>DirectSound入力デバイスのオープンに失敗しました。 マイクの取得ができませんでした。</translation>
    </message>
    <message>
        <location line="-5"/>
        <source>Lost DirectSound input device.</source>
        <translation>DirectSound入力デバイスを失いました。</translation>
    </message>
</context>
<context>
    <name>DXAudioOutput</name>
    <message>
        <source>Mumble</source>
        <translation type="obsolete">Mumble</translation>
    </message>
    <message>
        <location line="-167"/>
        <source>Opening chosen DirectSound Output failed. No audio will be heard.</source>
        <translation>選択されたDirectSound出力のオープンに失敗しました。音声は出力されません。</translation>
    </message>
    <message>
        <location line="-82"/>
        <source>Opening chosen DirectSound Output failed. Default device will be used.</source>
        <translation>選択されたDirectSound出力のオープンに失敗しました。音声は出力されません。</translation>
    </message>
    <message>
        <location line="-247"/>
        <source>Default DirectSound Voice Output</source>
        <translation>デフォルトのDirectSound音声出力</translation>
    </message>
    <message>
        <location line="+325"/>
        <source>Lost DirectSound output device.</source>
        <translation>DirectSound出力デバイスを失いました。</translation>
    </message>
</context>
<context>
    <name>DXConfigDialog</name>
    <message>
        <source>Default DirectSound Voice Input</source>
        <translation type="obsolete">デフォルトのDirectSound音声入力</translation>
    </message>
    <message>
        <source>Default DirectSound Voice Output</source>
        <translation type="obsolete">デフォルトのDirectSound音声出力</translation>
    </message>
    <message>
        <source>Device selection</source>
        <translation type="obsolete">デバイス選択</translation>
    </message>
    <message>
        <source>Device to use for microphone</source>
        <translation type="obsolete">マイクを使用するデバイス</translation>
    </message>
    <message>
        <source>This sets the input device to use, which is where you have connected the microphone.</source>
        <translation type="obsolete">使用する入力デバイスを設定します。これは、マイクを接続しているデバイスです。</translation>
    </message>
    <message>
        <source>Input</source>
        <translation type="obsolete">入力</translation>
    </message>
    <message>
        <source>Device to use for speakers/headphones</source>
        <translation type="obsolete">スピーカー/ヘッドホンに使用するデバイス</translation>
    </message>
    <message>
        <source>This sets the output device to use, which is where you have connected your speakers or your headset.</source>
        <translation type="obsolete">使用する出力デバイスを設定します。これは、あなたのスピーカ(もしくはヘッドホン)が接続しているデバイスです。</translation>
    </message>
    <message>
        <source>Output</source>
        <translation type="obsolete">出力</translation>
    </message>
    <message>
        <source>Output Options</source>
        <translation type="obsolete">出力オプション</translation>
    </message>
    <message>
        <source>Output Delay</source>
        <translation type="obsolete">出力猶予</translation>
    </message>
    <message>
        <source>Amount of data to buffer for DirectSound</source>
        <translation type="obsolete">DirectSoundのバッファデータの量</translation>
    </message>
    <message>
        <source>This sets the amount of data to prebuffer in the DirectSound buffer. Experiment with different values and set it to the lowest which doesn&apos;t cause rapid jitter in the sound.</source>
        <translation type="obsolete">DirectSoundバッファにおけるプリバッファの量を設定します。いろいろな値で試してみて、急に不安定にならない一番小さな値に設定してください。</translation>
    </message>
    <message>
        <source>Positional Audio</source>
        <translation type="obsolete">位置音響</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">なし</translation>
    </message>
    <message>
        <source>Panning</source>
        <translation type="obsolete">パンニング</translation>
    </message>
    <message>
        <source>Light HRTF</source>
        <translation type="obsolete">少量のHRTF</translation>
    </message>
    <message>
        <source>Full HRTF</source>
        <translation type="obsolete">フルHRTF</translation>
    </message>
    <message>
        <source>3D Sound Algorithm</source>
        <translation type="obsolete">3Dサウンドのアルゴリズム</translation>
    </message>
    <message>
        <source>This sets what 3D Sound algorithm to use.&lt;br /&gt;&lt;b&gt;None&lt;/b&gt; - Disable 3D Sound (least CPU).&lt;br /&gt;&lt;b&gt;Panning&lt;/b&gt; - Just use stereo panning (some CPU).&lt;br /&gt;&lt;b&gt;Light/Full HRTF&lt;/b&gt; - Head-Related Transfer Functions enabled. This may use a small amount of CPU.&lt;br /&gt;Note that if you have a soundcard with &lt;i&gt;hardware&lt;/i&gt; 3D processing, HRTF processing will be done on the soundcard and will use practically no processing power.</source>
        <translation type="obsolete">3Dサウンドのアルゴリズムに何を使用するか設定します。&lt;br /&gt;&lt;b&gt;なし&lt;/b&gt; - 3Dサウンドを使いません(最小のCPU消費)。&lt;br /&gt;&lt;b&gt;パンニング&lt;/b&gt; - ステレオパンニングだけを使います(若干のCPU消費)。&lt;br /&gt;&lt;b&gt;少量の/フル HRTF&lt;/b&gt; - 頭部伝達関数(Head-Related Transfer Functions)を使用します。これは、少量のCPU消費を伴うことがあります。&lt;br /&gt;&lt;i&gt;ハードウェア&lt;/i&gt;3D処理に対応したサウンドカードをお使いの場合、HRTFはサウンドカードで処理され、CPUパワーをほとんど使いません。</translation>
    </message>
    <message>
        <source>Method</source>
        <translation type="obsolete">方式</translation>
    </message>
    <message>
        <source>MinDistance</source>
        <translation type="obsolete">最小距離</translation>
    </message>
    <message>
        <source>Minimum distance to player before sound decreases</source>
        <translation type="obsolete">音が小さくなる前のプレイヤーへの最小距離</translation>
    </message>
    <message>
        <source>This sets the minimum distance for sound calculations. The volume of other players&apos; speech will not decrease until they are at least this far away from you.</source>
        <translation type="obsolete">サウンドの計算に使う最小距離を設定します。少なくともこの距離だけ離れていれば、他のプレイヤーの音声は減衰しません。</translation>
    </message>
    <message>
        <source>MaxDistance</source>
        <translation type="obsolete">最大距離</translation>
    </message>
    <message>
        <source>Maximum distance, beyond which sound won&apos;t decrease</source>
        <translation type="obsolete">音が小さくならない最大距離</translation>
    </message>
    <message>
        <source>This sets the maximum distance for sound calculations. When farther away than this, other players&apos; sound volume will not decrease any more.</source>
        <translation type="obsolete">サウンドの計算に使う最大距離を設定します。これよりも遠くなると、他のプレイヤーの音声はこれ以上減衰しなくなります。</translation>
    </message>
    <message>
        <source>RollOff</source>
        <translation type="obsolete">ロールオフ</translation>
    </message>
    <message>
        <source>Factor for sound volume decrease</source>
        <translation type="obsolete">ボリューム減少の割合</translation>
    </message>
    <message>
        <source>How fast should sound volume drop when passing beyond the minimum distance. The normal (1.0) is that sound volume halves each time the distance doubles. Increasing this value means sound volume drops faster, while decreasing it means it drops slower.</source>
        <translation type="obsolete">最小距離を超えた時、音量がどのくらい落ちるかについての値です。普通(1.0)の設定では距離が2倍になるたびに音量は半分になります。これを大きくすると音量の落ち込みが速くなり、小さくすると音量の落ち込みは遅くなります。</translation>
    </message>
    <message>
        <source>DirectSound</source>
        <translation type="obsolete">DirectSound</translation>
    </message>
    <message>
        <source>%1ms</source>
        <translation type="obsolete">%1ms</translation>
    </message>
    <message>
        <source>%1m</source>
        <translation type="obsolete">%1м</translation>
    </message>
    <message>
        <source>%1</source>
        <translation type="obsolete">%1</translation>
    </message>
    <message>
        <source>Players more than %1 meters away have %2% intensity</source>
        <translation type="obsolete">%1 メートル以上離れたプレイヤーで %2% の強さ</translation>
    </message>
    <message>
        <source>Form</source>
        <translation type="obsolete">フォーム</translation>
    </message>
    <message>
        <source>TextLabel</source>
        <translation type="obsolete">テキストラベル</translation>
    </message>
</context>
<context>
    <name>Database</name>
    <message>
        <source>Mumble</source>
        <translation type="obsolete">Mumble</translation>
    </message>
    <message>
        <location filename="Database.cpp" line="+127"/>
        <source>Mumble failed to initialize a database in any
of the possible locations.</source>
        <translation>Mumble: データベースの初期化が可能な場所がなく、
操作に失敗しました。
</translation>
    </message>
</context>
<context>
    <name>DirectInputKeyWidget</name>
    <message>
        <source>Press Shortcut</source>
        <translation type="obsolete">DirectInputKeyウィジェット</translation>
    </message>
</context>
<context>
    <name>GlobalShortcut</name>
    <message>
        <location filename="GlobalShortcut.ui"/>
        <source>Shortcuts</source>
        <translation>ショートカット</translation>
    </message>
    <message>
        <location/>
        <source>List of configured shortcuts</source>
        <translation>設定されたショートカットのリスト</translation>
    </message>
    <message>
        <location/>
        <source>Function</source>
        <translation>機能</translation>
    </message>
    <message>
        <location/>
        <source>Shortcut</source>
        <translation>ショートカット</translation>
    </message>
    <message>
        <location/>
        <source>Suppress</source>
        <translation>抑制</translation>
    </message>
    <message>
        <location/>
        <source>Add new shortcut</source>
        <translation>新しいショートカットを追加</translation>
    </message>
    <message>
        <location/>
        <source>This will add a new global shortcut</source>
        <translation>これにより新しいグローバルショートカットを追加するだろう</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Add</source>
        <translation>追加(&amp;A)</translation>
    </message>
    <message>
        <location/>
        <source>Remove selected shortcut</source>
        <translation>選択されたショートカットを削除します</translation>
    </message>
    <message>
        <location/>
        <source>This will permanently remove a selected shortcut.</source>
        <translation>これにより選択されたショートカットを永久に削除します。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Remove</source>
        <translation>削除(&amp;R)</translation>
    </message>
</context>
<context>
    <name>GlobalShortcutConfig</name>
    <message>
        <location filename="GlobalShortcut.cpp" line="+228"/>
        <source>Shortcuts</source>
        <translation>ショートカット</translation>
    </message>
    <message>
        <source>Function</source>
        <translation type="obsolete">機能</translation>
    </message>
    <message>
        <source>Shortcut</source>
        <translation type="obsolete">ショートカット</translation>
    </message>
    <message>
        <source>Shortcut bound to %1.</source>
        <translation type="obsolete">%1 に登録されているショートカット</translation>
    </message>
    <message>
        <source>&lt;b&gt;This is the global shortcut bound to %1&lt;/b&gt;&lt;br /&gt;Click this field and then the desired key/button combo to rebind. Double-click to clear.</source>
        <translation type="obsolete">&lt;b&gt;%1 に登録するショートカットキーです。&lt;/b&gt;&lt;br /&gt;このフィールドをクリックして希望のキー/ボタンの組み合わせを押すと登録できます。ダブルクリックで削除します。</translation>
    </message>
    <message>
        <source>Double-click an entry to clear the shortcut.</source>
        <translation type="obsolete">項目をダブルクリックするとショートカットを削除します。</translation>
    </message>
    <message>
        <source>Suppress</source>
        <translation type="obsolete">抑制</translation>
    </message>
    <message>
        <location line="+33"/>
        <source>Shortcut button combination.</source>
        <translation>ショートカットボタンの組み合わせ。</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>&lt;b&gt;This is the global shortcut key combination.&lt;/b&gt;&lt;br /&gt;Double-click this field and then the desired key/button combo to rebind.</source>
        <translation>&lt;b&gt;これはグローバルショットカットキーの組み合わせです。&lt;/b&gt;&lt;br /&gt;このフィールドをダブルクリックして、バインドしなおしたいキーかボタンのコンボをダブルクリックしてください。</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Suppress keys from other applications</source>
        <translation>他のアプリケーションのキーを抑制します</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>&lt;b&gt;This hides the button presses from other applications.&lt;/b&gt;&lt;br /&gt;Enabling this will hide the button (or the last button of a multi-button combo) from other applications. Note that not all buttons can be suppressed.</source>
        <translation>&lt;b&gt;他のアプリケーションで押されたボタンを隠します.&lt;/b&gt;&lt;br /&gt;この設定を有効にすると、他のアプリケーションで押されたボタン(または、複数ボタンの組み合わせの最後のもの)を隠します. 全てのボタンを抑制できるわけではないことにご注意ください。</translation>
    </message>
</context>
<context>
    <name>GlobalShortcutMacInit</name>
    <message>
        <location filename="GlobalShortcut_macx.cpp" line="+63"/>
        <source>Mumble has detected that it is unable to receive Global Shortcut events when it is in the background.&lt;br /&gt;&lt;br /&gt;This is because the Universal Access feature called &apos;Enable access for assistive devices&apos; is currently disabled.&lt;br /&gt;&lt;br /&gt;Please &lt;a href=&quot; &quot;&gt;enable this setting&lt;/a&gt; and continue when done.</source>
        <translation>バックグラウンドでグローバルなショートカットイベントが発生し、受け入れることができませんでした。&lt;br /&gt;&lt;br /&gt;&apos;補助デバイスへのアクセス&apos; のようなユニバーサルアクセス機能が無効になっているのが原因です。&lt;br /&gt;&lt;br /&gt;&lt;a href=&quot; &quot;&gt;この設定を有効にして&lt;/a&gt; その後、再び設定してください。 </translation>
    </message>
</context>
<context>
    <name>GlobalShortcutWinConfig</name>
    <message>
        <source>Shortcuts</source>
        <translation type="obsolete">ショートカット一覧</translation>
    </message>
    <message>
        <source>Function</source>
        <translation type="obsolete">機能</translation>
    </message>
    <message>
        <source>Shortcut</source>
        <translation type="obsolete">ショートカット</translation>
    </message>
    <message>
        <source>Shortcut bound to %1.</source>
        <translation type="obsolete">%1 に登録されているショートカット</translation>
    </message>
    <message>
        <source>&lt;b&gt;This is the global shortcut bound to %1&lt;/b&gt;&lt;br /&gt;Click this field and then the desired key/button combo to rebind. Double-click to clear.</source>
        <translation type="obsolete">&lt;b&gt;%1 に登録するショートカットキーです。&lt;/b&gt;&lt;br /&gt;このフィールドをクリックして希望のキー/ボタンの組み合わせを押すと登録できます。ダブルクリックで削除します。</translation>
    </message>
</context>
<context>
    <name>GlobalShortcutX</name>
    <message>
        <location filename="GlobalShortcut_unix.cpp" line="+325"/>
        <source>Mouse %1</source>
        <translation>マウス %1</translation>
    </message>
</context>
<context>
    <name>GlobalShortcutXConfig</name>
    <message>
        <source>Shortcuts</source>
        <translation type="obsolete">ショートカット一覧</translation>
    </message>
    <message>
        <source>Function</source>
        <translation type="obsolete">機能</translation>
    </message>
    <message>
        <source>Shortcut</source>
        <translation type="obsolete">ショートカット</translation>
    </message>
    <message>
        <source>Shortcut bound to %1.</source>
        <translation type="obsolete">%1に登録されているショートカット</translation>
    </message>
    <message>
        <source>&lt;b&gt;This is the global shortcut bound to %1&lt;/b&gt;&lt;br /&gt;Click this field and then the desired key combo to rebind. Double-click to clear.</source>
        <translation type="obsolete">&lt;b&gt;%1 に登録するショートカットキーです。&lt;/b&gt;&lt;br /&gt;このフィールドをクリックして希望のキー/ボタンの組み合わせを押すと登録できます。ダブルクリックで削除します。</translation>
    </message>
</context>
<context>
    <name>LCD</name>
    <message>
        <location filename="LCD.cpp" line="+274"/>
        <source>Not connected</source>
        <translation>接続されていません</translation>
    </message>
</context>
<context>
    <name>LCDConfig</name>
    <message>
        <location line="-161"/>
        <source>Graphic</source>
        <translation>画像</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Character</source>
        <translation>キャラクタ</translation>
    </message>
    <message>
        <location line="+11"/>
        <source>Enable this device</source>
        <translation>このデバイスを有効にする</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>LCD</source>
        <translation>LCD</translation>
    </message>
    <message>
        <location filename="LCD.ui"/>
        <source>Form</source>
        <translation>フォーム</translation>
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
        <translation>デバイス</translation>
    </message>
    <message>
        <location/>
        <source>Name</source>
        <translation>チャンネル名</translation>
    </message>
    <message>
        <location/>
        <source>Type</source>
        <translation>タイプ</translation>
    </message>
    <message>
        <location/>
        <source>Size</source>
        <translation>サイズ</translation>
    </message>
    <message>
        <location/>
        <source>Enabled</source>
        <translation>有効</translation>
    </message>
    <message>
        <location/>
        <source>Views</source>
        <translation>ビュー</translation>
    </message>
    <message>
        <location/>
        <source>Minimum Column Width</source>
        <translation>列の最少幅</translation>
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
        <translation>テキストラベル</translation>
    </message>
    <message>
        <location/>
        <source>Splitter Width</source>
        <translation>分割線の幅</translation>
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
        <translation type="unfinished">デバッグ</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Critical</source>
        <translation type="unfinished">致命的</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Warning</source>
        <translation type="unfinished">警告</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Information</source>
        <translation type="unfinished">情報</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Server Connected</source>
        <translation type="unfinished">サーバー接続</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Server Disconnected</source>
        <translation type="unfinished">サーバー切断</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Player Joined Server</source>
        <translation type="unfinished">誰かがサーバーに参加</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Player Left Server</source>
        <translation type="unfinished">誰かがサーバーから退出</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Player kicked (you or by you)</source>
        <translation type="unfinished">誰かがキックされた時(あなた自身、またはあなたによって)</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Player kicked</source>
        <translation type="unfinished">誰かがキックされた時</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>You self-muted/deafened</source>
        <translation type="unfinished">自分を発言禁止/聴取禁止にした時</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Other self-muted/deafened</source>
        <translation type="unfinished">他の人が発言禁止/聴取禁止になった時</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Player muted (you)</source>
        <translation type="unfinished">誰かがあなたを発言禁止にした時</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Player muted (by you)</source>
        <translation type="unfinished">(あなたによって)誰かが発言禁止になった時</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Player muted (other)</source>
        <translation type="unfinished">(他の人によって)誰かが発言禁止になった時</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Player Joined Channel</source>
        <translation type="unfinished">誰かがチャンネルに参加</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Player Left Channel</source>
        <translation type="unfinished">誰かがチャンネルから退出</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Permission Denied</source>
        <translation type="unfinished">権限がない場合</translation>
    </message>
    <message>
        <source>[%2] %1</source>
        <translation type="obsolete">[%2] %1</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Text Message</source>
        <translation type="unfinished">テキストメッセージ</translation>
    </message>
</context>
<context>
    <name>LogConfig</name>
    <message>
        <location line="-164"/>
        <source>Toggle console for %1 events</source>
        <translation>イベント「%1」 のためのコンソールを切り替える</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Toggle pop-up notifications for %1 events</source>
        <translation>イベント「%1」のためのポップアップ通知を切り替える</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Toggle Text-To-Speech for %1 events</source>
        <translation>イベント「%1」のためのテキスト読み上げを切り替える</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Click here to toggle sound notification for %1 events</source>
        <translation>イベント「%1」の音声通知を切り替えるためにはここをクリック</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Path to soundfile used for sound notifications in the case of %1 events&lt;br /&gt;Single click to play&lt;br /&gt;Doubleclick to change</source>
        <translation>イベント「%1」のときに音声通知のために使用するサウンドファイルのパス&lt;br /&gt;シングルクリックで再生&lt;br /&gt;ダブルクリックで変更</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Click here to toggle console output for %1 events.&lt;br /&gt;If checked, this option makes Mumble output all %1 events in its message log.</source>
        <translation>イベント「%1」のコンソール出力を切り替えるためにはここをクリック。&lt;br /&gt;チェックされた場合、メッセージログに全てのイベント「%1」を出力する。</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Click here to toggle pop-up notifications for %1 events.&lt;br /&gt;If checked, a notification pop-up will be created by Mumble for every %1 event.</source>
        <oldsource>Click here to toggle pop-up notifications for %1 events.&lt;br /&gt;If checked, a notification pop-up will be created by mumble for every %1 event.</oldsource>
        <translation>イベント「%1」のポップアップ通知を切り替えるためにはここをクリック。&lt;br /&gt;チェックされた場合、イベント「%1」がおきるたびポップアップ通知が行われる。</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Click here to toggle Text-To-Speech for %1 events.&lt;br /&gt;If checked, Mumble uses Text-To-Speech to read %1 events out loud to you. Text-To-Speech is also able to read the contents of the event which is not true for soundfiles. Text-To-Speech and soundfiles cannot be used at the same time.</source>
        <oldsource>Click here to toggle Text-To-Speech for %1 events.&lt;br /&gt;If checked, mumble uses Text-To-Speech to read %1 events out loud to you. Text-To-Speech is also able to read the contents of the event which is not true for soundfiles. Text-To-Speech and soundfiles cannot be used at the same time.</oldsource>
        <translation>イベント「%1」の適すと読み上げを切り替えるためにはここをクリック。&lt;br /&gt;チェックされた場合、イベント「%1」が読み上げられる。テキスト読み上げはサウンドファイルではでいない、イベントの内容を読み上げがが可能である。テキスト読み上げとサウンドファイルは同時に使用することはできない。</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Click here to toggle sound notification for %1 events.&lt;br /&gt;If checked, Mumble uses a soundfile predefined by you to indicate %1 events. Soundfiles and Text-To-Speech cannot be used at the same time.</source>
        <oldsource>Click here to toggle sound notification for %1 events.&lt;br /&gt;If checked, mumble uses a soundfile predefined by you to indicate %1 events. Soundfiles and Text-To-Speech cannot be used at the same time.</oldsource>
        <translation>イベント「%1」のときに音声通知を切り替えるにはここをクリックしてください。&lt;br /&gt; もしチェックしたなら、Mumble はイベント「%1」に設定された音声ファイルを再生します。音声ファイルとTTSは同時に使えません。</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Path to soundfile used for sound notifications in the case of %1 events.&lt;br /&gt;Single click to play&lt;br /&gt;Doubleclick to change&lt;br /&gt;Be sure that sound notifications for these events are enabled or this field will not have any effect.</source>
        <translation>イベント「%1」のときに音声通知のために使用するサウンドファイルのパス&lt;br /&gt;シングルクリックで再生&lt;br /&gt;ダブルクリックで変更&lt;br /&gt;これらのイベントのための音声通知が有効であるか、またはこのフィールドが無効であることを確認してください。</translation>
    </message>
    <message>
        <location line="+93"/>
        <source>Choose sound file</source>
        <translation>サウンドファイルを選択してください</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Invalid sound file</source>
        <translation>不正なサウンドファイル</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The file &apos;%1&apos; does not exist or is not a valid speex file.</source>
        <translation>ファイル「%1」は存在しないか、適切な speex ファイルではありません。</translation>
    </message>
    <message>
        <location filename="Log.ui"/>
        <source>Messages</source>
        <translation>メッセージ</translation>
    </message>
    <message>
        <location/>
        <source>Console</source>
        <translation>コンソール</translation>
    </message>
    <message>
        <source>TTS</source>
        <translation type="obsolete">読み上げ</translation>
    </message>
    <message>
        <source>Enable console for %1</source>
        <translation type="obsolete">%1をコンソールで有効にする</translation>
    </message>
    <message>
        <source>Enable Text-To-Speech for %1</source>
        <translation type="obsolete">%1のテキスト読み上げを有効にする</translation>
    </message>
    <message>
        <location/>
        <source>Text To Speech</source>
        <translation>テキスト読み上げ</translation>
    </message>
    <message>
        <location/>
        <source>Volume</source>
        <translation>音量</translation>
    </message>
    <message>
        <location/>
        <source>Volume of Text-To-Speech Engine</source>
        <translation>テキスト読み上げエンジンの音量</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is the volume used for the speech synthesis.&lt;/b&gt;</source>
        <translation>&lt;b&gt;この音量は合成音声に用いられます。&lt;/b&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Length threshold</source>
        <translation>長さの上限</translation>
    </message>
    <message>
        <location/>
        <source>Message length threshold for Text-To-Speech Engine</source>
        <translation>テキスト読み上げエンジンにおける長さの上限値</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is the length threshold used for the Text-To-Speech Engine.&lt;/b&gt;&lt;br /&gt;Messages longer than this limit will not be read aloud in their full length.</source>
        <translation>&lt;b&gt;テキスト読み上げエンジンにおける長さの上限値です。&lt;/b&gt;&lt;br /&gt;上限より長いものは、最後まで読み上げられません。</translation>
    </message>
    <message>
        <location/>
        <source>Message</source>
        <translation>メッセージ</translation>
    </message>
    <message>
        <location/>
        <source>Notification</source>
        <translation>通知</translation>
    </message>
    <message>
        <location/>
        <source>Text-To-Speech</source>
        <translation>テキスト読み上げ</translation>
    </message>
    <message>
        <location/>
        <source>Soundfile</source>
        <translation>サウンドファイル</translation>
    </message>
    <message>
        <location/>
        <source>Path</source>
        <translation>パス</translation>
    </message>
    <message>
        <location/>
        <source> Characters</source>
        <translation> 文字</translation>
    </message>
</context>
<context>
    <name>LookConfig</name>
    <message>
        <location filename="LookConfig.ui"/>
        <source>Language</source>
        <translation>言語</translation>
    </message>
    <message>
        <location/>
        <source>Look and Feel</source>
        <translation>外観と手触り</translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="+46"/>
        <location line="+11"/>
        <source>System default</source>
        <translation>システムのデフォルト</translation>
    </message>
    <message>
        <location filename="LookConfig.ui"/>
        <source>Language to use (requires restart)</source>
        <translation>使用する言語(再起動が必要)</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets which language Mumble should use.&lt;/b&gt;&lt;br /&gt;You have to restart Mumble to use the new language.</source>
        <translation>&lt;b&gt;Mumbleが使用する言語を決定します。&lt;/b&gt;&lt;br /&gt;新しい言語を使うためには再起動が必要です。</translation>
    </message>
    <message>
        <location/>
        <source>Style</source>
        <translation>スタイル</translation>
    </message>
    <message>
        <location/>
        <source>Basic widget style</source>
        <translation>基本的なウインドウのスタイル</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the basic look and feel to use.&lt;/b&gt;</source>
        <translation>&lt;b&gt;基本となる見た目と使用感を設定します。&lt;/b&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Skin</source>
        <translation>スキン</translation>
    </message>
    <message>
        <location/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location/>
        <source>Skin file to use</source>
        <translation>使用するスキンファイル</translation>
    </message>
    <message>
        <source>Use Horizontal Splitter</source>
        <translation type="obsolete">上下に分割する</translation>
    </message>
    <message>
        <source>Use horizontal or vertical splitter for the main window</source>
        <translation type="obsolete">メインウインドウの区切りに水平もしくは垂直の区切りを使う</translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="+14"/>
        <source>User Interface</source>
        <translation>インターフェース</translation>
    </message>
    <message>
        <location line="+85"/>
        <source>Choose skin file</source>
        <translation>スキンファイルを選択</translation>
    </message>
    <message>
        <location filename="LookConfig.ui"/>
        <source>&lt;b&gt;If set, players will be shown above subchannels in the channel view.&lt;/b&gt;&lt;br /&gt;A restart of Mumble is required to see the change.</source>
        <translation>&lt;b&gt;これを設定していると、チャンネル表示の時にサブチャンネルより上にプレイヤーが表示されるようになります。&lt;/b&gt;&lt;br /&gt;この設定を反映するにはMumbleの再起動が必要です。</translation>
    </message>
    <message>
        <source>Expand All Channels</source>
        <translation type="obsolete">全てのチャンネルを展開する</translation>
    </message>
    <message>
        <source>Expand all channels when connecting</source>
        <translation type="obsolete">接続時に全てのチャンネルを展開する</translation>
    </message>
    <message>
        <source>&lt;b&gt;If set, all channels will be expanded by default when you connect to a server.&lt;/b&gt;</source>
        <translation type="obsolete">&lt;b&gt;これを設定していると、サーバに接続したときに全てのチャンネルを展開します。&lt;/b&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Players above Channels</source>
        <translation>チャンネルより上にプレイヤーを表示</translation>
    </message>
    <message>
        <location/>
        <source>List players above subchannels (requires restart).</source>
        <translation>サブチャンネルより上にプレイヤーが表示されるように並べます。(再起動が必要)。</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets which skin Mumble should use.&lt;/b&gt;&lt;br /&gt;The skin is a style file applied on top of the basic widget style. If there are icons in the same directory as the style sheet, those will replace the default icons.</source>
        <translation>&lt;b&gt;Mumbleで使用するスキンを設定します。&lt;/b&gt;&lt;br /&gt;スキンは基本的なウィジェットスタイルで用いられるスタイルファイルです。アイコンがスタイルシートと同じディレクトリにあるなら、デフォルトのアイコンを置き換えます。</translation>
    </message>
    <message>
        <location/>
        <source>Form</source>
        <translation>フォーム</translation>
    </message>
    <message>
        <source>New version check</source>
        <translation type="obsolete">新しいバージョンをチェック</translation>
    </message>
    <message>
        <source>Check for updates on startup</source>
        <translation type="obsolete">起動時に更新をチェックする</translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="-95"/>
        <source>None</source>
        <translation>なし</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Only with players</source>
        <translation>人が居るチャンネルのみ</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>All</source>
        <translation>全て</translation>
    </message>
    <message>
        <location filename="LookConfig.ui"/>
        <source>Expand</source>
        <translation>展開</translation>
    </message>
    <message>
        <location/>
        <source>When to automatically expand channels</source>
        <translation>自動的にチャンネルを展開する</translation>
    </message>
    <message>
        <location/>
        <source>This sets which channels to automatically expand. &lt;i&gt;None&lt;/i&gt; and &lt;i&gt;All&lt;/i&gt; will expand no or all channels, while &lt;i&gt;Only with players&lt;/i&gt; will expand and collapse channels as players join and leave them.</source>
        <translation>どのチャンネルを自動的に展開するかを決定します。 &lt;i&gt;なし&lt;/i&gt; と &lt;i&gt;全て&lt;/i&gt; は全てのチャンネルを展開する/しない設定にします。&lt;i&gt;人が居るチャンネルのみ&lt;/i&gt; はプレイヤーの入室状況に応じて展開と折りたたみが変わります。</translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="+2"/>
        <source>Ask</source>
        <translation>確認する</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Do Nothing</source>
        <translation>何もしない</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Move</source>
        <translation>ユーザの移動</translation>
    </message>
    <message>
        <location filename="LookConfig.ui"/>
        <source>Channel Dragging</source>
        <translation>チャンネルのドラッグ操作</translation>
    </message>
    <message>
        <location/>
        <source>This changes the behaviour when moving channels.</source>
        <translation>チャンネルを動かすときのふるまいを変更します。</translation>
    </message>
    <message>
        <location/>
        <source>This sets the behaviour of channel drags, it can be used to prevent accidental dragging. &lt;i&gt;Move Channel&lt;/i&gt; moves the channel without prompting. &lt;i&gt;Do Nothing&lt;/i&gt; does nothing and prints an error message. &lt;i&gt;Ask&lt;/i&gt; uses a message box to confirm if you really wanted to move the channel.</source>
        <translation>チャンネルを動かすときのふるまいを変更します。これを設定すると誤ってチャンネルをドラッグしてしまうのを防ぐことができます。 &lt;i&gt;チャンネルを移動&lt;/i&gt; 確認なしにチャンネルを動かします。 &lt;i&gt;何もしない&lt;/i&gt; 何も行わず、エラーメッセージを表示します。 &lt;i&gt;確認する&lt;/i&gt; 確認メッセージボックスを出してから動かします。</translation>
    </message>
    <message>
        <location/>
        <source>Ask whether to close or minimize when quitting Mumble.</source>
        <translation>終了時に最小化するか閉じるかを確認します。</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;If set, will verify you want to quit if connected.&lt;b&gt;</source>
        <translation>&lt;b&gt;これがセットされている場合、接続中に終了するかを確認します。&lt;/b&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Ask on quit while connected</source>
        <translation>接続中の終了は確認する</translation>
    </message>
    <message>
        <location/>
        <source>Make the Mumble window appear on top of other windows.</source>
        <translation>最前面に表示する。</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This makes the Mumble window a topmost window.&lt;/b&gt;</source>
        <translation>&lt;b&gt;Mumbleのウインドウを最前面に表示します。&lt;/b&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Always On Top</source>
        <translation>前面表示</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;If set, minimizing the Mumble main window will cause it to be hidden and accessible only from the tray. Otherwise, it will be minimized as a window normally would.&lt;/b&gt;</source>
        <translation>&lt;b&gt;これがセットされていた場合、最小化されたMumbleのメインウインドウは非表示になり、タスクトレイからのみアクセスできるようになります。これがセットされていない場合、ウインドウは普通に最小化します。&lt;/b&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Hide in tray when minimized</source>
        <translation>最小化時、タスクトレイに格納する</translation>
    </message>
    <message>
        <source>Show all configuration items</source>
        <translation type="obsolete">全ての設定項目を表示</translation>
    </message>
    <message>
        <source>&lt;b&gt;This will show all configuration items.&lt;/b&gt;&lt;br /&gt;Mumble contains a lot of configuration items that most users won&apos;t need to change. Checking this will show all configurable items.</source>
        <translation type="obsolete">&lt;b&gt;全ての設定項目を表示します。&lt;/b&gt;&lt;br /&gt;Mumbleは大部分のユーザーが変更する必要のない設定項目を多く含んでいます。ここにチェックを入れると全ての設定項目を表示します。</translation>
    </message>
    <message>
        <source>Expert Config</source>
        <translation type="obsolete">上級者向け設定</translation>
    </message>
    <message>
        <location/>
        <source>Hide the main Mumble window in the tray when it is minimized.</source>
        <translation>最小化されている時にメインウインドウをタスクトレイに格納します。</translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <location filename="MainWindow.cpp" line="+127"/>
        <location line="+1196"/>
        <source>Root</source>
        <translation>ルート</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>&amp;Connect</source>
        <translation>接続(&amp;C)</translation>
    </message>
    <message>
        <location/>
        <source>Open the server connection dialog</source>
        <translation>サーバ接続ダイアログを開く</translation>
    </message>
    <message>
        <location/>
        <source>Shows a dialog of registered servers, and also allows quick connect.</source>
        <translation>登録されたサーバのダイアログを開きます。クイック接続もできるようになります。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Disconnect</source>
        <translation>切断(&amp;D)</translation>
    </message>
    <message>
        <location/>
        <source>Disconnect from server</source>
        <translation>サーバから切断</translation>
    </message>
    <message>
        <location/>
        <source>Disconnects you from the server.</source>
        <translation>サーバから切断します。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Ban lists</source>
        <translation>&amp;BAN一覧</translation>
    </message>
    <message>
        <location/>
        <source>Edit ban lists on server</source>
        <translation>サーバのBAN一覧を編集</translation>
    </message>
    <message>
        <location/>
        <source>This lets you edit the server-side IP ban lists.</source>
        <translation>サーバ側の禁止IPリストを編集します。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Kick</source>
        <translation>キック(&amp;K)</translation>
    </message>
    <message>
        <location/>
        <source>Kick player (with reason)</source>
        <translation>プレイヤーをキック(理由付き)</translation>
    </message>
    <message>
        <location/>
        <source>Kick selected player off server. You&apos;ll be asked to specify a reason.</source>
        <translation>選択されたプレイヤーをサーバから追い出します。あなたは、理由を尋ねられるでしょう。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Ban</source>
        <translation>&amp;BAN</translation>
    </message>
    <message>
        <location/>
        <source>Kick and ban player (with reason)</source>
        <translation>キックしてBAN(理由付き)</translation>
    </message>
    <message>
        <location/>
        <source>Kick and ban selected player from server. You&apos;ll be asked to specify a reason.</source>
        <translation>選択されたプレイヤーをサーバからキックしてBANします。あなたは、理由を尋ねられるでしょう。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Mute</source>
        <translation>発言禁止(&amp;M)</translation>
    </message>
    <message>
        <location/>
        <source>Mute player</source>
        <translation>プレイヤーを発言禁止にする</translation>
    </message>
    <message>
        <location/>
        <source>Mute or unmute player on server. Unmuting a deafened player will also undeafen them.</source>
        <translation>サーバにいるプレイヤーを発言禁止にしたり、解除したりします。聴取禁止になっているプレイヤーに対して発言禁止の解除を行うと、聴取禁止も解除されます。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Deafen</source>
        <translation>聴取禁止(&amp;D)</translation>
    </message>
    <message>
        <location/>
        <source>Deafen player</source>
        <translation>プレイヤーを聴取禁止にする</translation>
    </message>
    <message>
        <location/>
        <source>Deafen or undeafen player on server. Deafening a player will also mute them.</source>
        <translation>サーバにいるプレイヤーを聴取禁止にしたり、解除したりします。聴取禁止にすると発言も禁止にします。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Local Mute</source>
        <translation>ローカルで発言禁止(&amp;L)</translation>
    </message>
    <message>
        <source>Mute or unmute player locally.</source>
        <translation type="obsolete">ローカルで発言禁止にしたり解除したりします。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Add</source>
        <translation>追加(&amp;A)</translation>
    </message>
    <message>
        <location/>
        <source>Add new channel</source>
        <translation>新しいチャンネルを追加</translation>
    </message>
    <message>
        <location/>
        <source>This adds a new sub-channel to the currently selected channel.</source>
        <translation>現在選択されているチャンネルに新たなサブチャンネルを追加します。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Remove</source>
        <translation>削除(&amp;R)</translation>
    </message>
    <message>
        <location/>
        <source>Remove channel</source>
        <translation>チャンネルを削除</translation>
    </message>
    <message>
        <location/>
        <source>This removes a channel and all sub-channels.</source>
        <translation>チャンネルと全てのサブチャンネルを削除します。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Edit ACL</source>
        <translation>ACLを編集(&amp;E)</translation>
    </message>
    <message>
        <location/>
        <source>Edit Groups and ACL for channel</source>
        <translation>チャンネルのグループとACLを編集</translation>
    </message>
    <message>
        <location/>
        <source>This opens the Group and ACL dialog for the channel, to control permissions.</source>
        <translation>権限を管理するため、チャンネルのグループとACLダイアログを開きます。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Rename</source>
        <translation>チャンネル名を変更(&amp;R)</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Change Description</source>
        <translation>説明を変更する(&amp;C)</translation>
    </message>
    <message>
        <location/>
        <source>Changes the channel description</source>
        <translation>チャンネルの説明を変更する</translation>
    </message>
    <message>
        <location/>
        <source>This changes the description of a channel.</source>
        <translation>チャンネルの説明を変更する。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Link</source>
        <translation>リンク(&amp;L)</translation>
    </message>
    <message>
        <location/>
        <source>Link your channel to another channel</source>
        <translation>あなたのいるチャンネルを他のチャンネルとリンクします</translation>
    </message>
    <message>
        <location/>
        <source>This links your current channel to the selected channel. If they have permission to speak in the other channel, players can now hear each other. This is a permanent link, and will last until manually unlinked or the server is restarted. Please see the shortcuts for push-to-link.</source>
        <translation>あなたのいるチャンネルを選択されたチャンネルとリンクします。他方のチャンネルで発言する権限を持っていたら、プレイヤーはお互いを聞くことができます。これは恒久的なリンクであり、手動でリンクを解除されるかサーバがリスタートするまで続きます。push-to-linkのショートカットを見てください。</translation>
    </message>
    <message>
        <source>&amp;Unlink</source>
        <translation type="obsolete">リンク解除(&amp;U)</translation>
    </message>
    <message>
        <location/>
        <source>Unlink your channel from another channel</source>
        <translation>他のチャンネルとのリンクを解除します</translation>
    </message>
    <message>
        <location/>
        <source>This unlinks your current channel from the selected channel.</source>
        <translation>選択されたチャンネルとあなたのいるチャンネルのリンクを解除します。</translation>
    </message>
    <message>
        <location/>
        <source>Unlink &amp;All</source>
        <translation>全てのリンクを解除(&amp;A)</translation>
    </message>
    <message>
        <location/>
        <source>Unlinks your channel from all linked channels.</source>
        <translation>他のチャンネルとのリンクを全て解除します。</translation>
    </message>
    <message>
        <location/>
        <source>This unlinks your current channel (not the selected one) from all linked channels.</source>
        <translation>全てのリンクされたチャンネルとあなたのいるチャンネル(選択されたものではありません)のリンクを解除します。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Reset</source>
        <translation>リセット(&amp;R)</translation>
    </message>
    <message>
        <location/>
        <source>Reset audio preprocessor</source>
        <translation>音声プリプロセッサをリセット</translation>
    </message>
    <message>
        <location/>
        <source>This will reset the audio preprocessor, including noise cancellation, automatic gain and voice activity detection. If something suddenly worsens the audio environment (like dropping the microphone) and it was temporary, use this to avoid having to wait for the preprocessor to readjust.</source>
        <translation>ノイズキャンセルや音声増幅や音声有効化の検出といった音声の前処理をリセットします。何かが起こって一時的に音声の環境が悪化するとき(マイクを落とした時など)、プリプロセッサの対応を待つのを避けるため、これを使ってください。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Mute Self</source>
        <translation>自分を発言禁止(&amp;M)</translation>
    </message>
    <message>
        <location/>
        <source>Mute yourself</source>
        <translation>あなた自身を発言禁止にします</translation>
    </message>
    <message>
        <location/>
        <source>Mute or unmute yourself. When muted, you will not send any data to the server. Unmuting while deafened will also undeafen.</source>
        <translation>あなた自身を発言禁止にしたり、解除したりします。発言禁止のとき、サーバに何もデータを送りません。聴取禁止の時に発言禁止の解除を行うと、聴取禁止も解除されます。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Deafen Self</source>
        <translation>自分を聴取禁止(&amp;D)</translation>
    </message>
    <message>
        <location/>
        <source>Deafen yourself</source>
        <translation>あなた自身を聴取禁止にします</translation>
    </message>
    <message>
        <location/>
        <source>Deafen or undeafen yourself. When deafened, you will not hear anything. Deafening yourself will also mute.</source>
        <translation>あなた自身を聴取禁止にしたり、解除したりします。あなたは何も聞くことができません。聴取禁止にすると発言も禁止にします。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Text-To-Speech</source>
        <translation>テキスト読み上げ(&amp;T)</translation>
    </message>
    <message>
        <location/>
        <source>Toggle Text-To-Speech</source>
        <translation>テキスト読み上げの切り替え</translation>
    </message>
    <message>
        <location/>
        <source>Enable or disable the text-to-speech engine. Only messages enabled for TTS in the Configuration dialog will actually be spoken.</source>
        <translation>テキスト読み上げエンジンを有効にするかどうか。設定ダイアログにてテキスト読み上げが有効になっているメッセージのみ読み上げられます。</translation>
    </message>
    <message>
        <location/>
        <source>S&amp;tatistics</source>
        <translation>統計(&amp;T)</translation>
    </message>
    <message>
        <location/>
        <source>Display audio statistics</source>
        <translation>音声統計を表示</translation>
    </message>
    <message>
        <location/>
        <source>Pops up a small dialog with information about your current audio input.</source>
        <translation>現在の音声入力の情報を表示する小さなダイアログがポップアップします。</translation>
    </message>
    <message>
        <location/>
        <source>Forcibly unlink plugin</source>
        <translation>強制的にプラグインを外す</translation>
    </message>
    <message>
        <location/>
        <source>This forces the current plugin to unlink, which is handy if it is reading completely wrong data.</source>
        <translation>現在のプラグインを強制的に外します。完全におかしなデータを読み込んでいるときに便利でしょう。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Settings</source>
        <translation>設定(&amp;S)</translation>
    </message>
    <message>
        <location/>
        <source>Configure Mumble</source>
        <translation>Mumbleを設定</translation>
    </message>
    <message>
        <location/>
        <source>Allows you to change most settings for Mumble.</source>
        <translation>Mumbleのいろいろな設定をします。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;What&apos;s This?</source>
        <translation>&amp;What&apos;s This?</translation>
    </message>
    <message>
        <location/>
        <source>Enter What&apos;s This? mode</source>
        <translation>What&apos;s This?モードに移行します</translation>
    </message>
    <message>
        <location/>
        <source>Click this to enter &quot;What&apos;s This?&quot; mode. Your cursor will turn into a question mark. Click on any button, menu choice or area to show a description of what it is.</source>
        <translation>クリックして&quot;What&apos;s This?&quot;モードに入ります。マウスカーソルはクエスチョンマークになって、何かボタンを押すと、選択されたメニューやエリアが何をするかの説明を表示します。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;About</source>
        <translation>Mumbleについて(&amp;A)</translation>
    </message>
    <message>
        <location/>
        <source>Information about Mumble</source>
        <translation>Mumbleについての情報</translation>
    </message>
    <message>
        <location/>
        <source>Shows a small dialog with information and license for Mumble.</source>
        <translation>Mumbleの情報やライセンスについての小さなダイアログを表示します。</translation>
    </message>
    <message>
        <location/>
        <source>About &amp;Speex</source>
        <translation>&amp;Speexについて</translation>
    </message>
    <message>
        <location/>
        <source>Information about Speex</source>
        <translation>Speexに関する情報</translation>
    </message>
    <message>
        <location/>
        <source>Shows a small dialog with information about Speex.</source>
        <translation>Speexに関する情報のための小さなダイアログを表示します。</translation>
    </message>
    <message>
        <source>&amp;About Qt</source>
        <translation type="obsolete">Qtについて(&amp;A)</translation>
    </message>
    <message>
        <location/>
        <source>Information about Qt</source>
        <translation>Qtについての情報</translation>
    </message>
    <message>
        <location/>
        <source>Shows a small dialog with information about Qt.</source>
        <translation>Qtに関する情報のための小さなダイアログを表示します。</translation>
    </message>
    <message>
        <location/>
        <source>Check for &amp;Updates</source>
        <translation>更新を確認(&amp;U)</translation>
    </message>
    <message>
        <location/>
        <source>Check for new version of Mumble</source>
        <translation>Mumbleの新しいバージョンがあるか確認します</translation>
    </message>
    <message>
        <location/>
        <source>Connects to the Mumble webpage to check if a new version is available, and notifies you with an appropriate download URL if this is the case.</source>
        <translation>Mumbleのウェブページに接続して新しいバージョンが出ているかを確認します。もし新しいのが出ていたら適切なダウンロードURLを通知します。</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="-1086"/>
        <source>Mumble -- %1</source>
        <translation>Mumble -- %1</translation>
    </message>
    <message>
        <source>Log of messages</source>
        <translation type="obsolete">メッセージのログ</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>This shows all recent activity. Connecting to servers, errors and information messages all show up here.&lt;br /&gt;To configure exactly which messages show up here, use the &lt;b&gt;Settings&lt;/b&gt; command from the menu.</source>
        <translation>最近の動作を全て表示します。サーバへの接続、エラーや情報メッセージは全てここに表示されます。&lt;br /&gt;ここにどのメッセージを表示するかを指定するには、メニューから&lt;b&gt;設定&lt;/b&gt;コマンドを選んでください。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Server</source>
        <translation>サーバ(&amp;S)</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Player</source>
        <translation>プレイヤー(&amp;P)</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Channel</source>
        <translation>チャンネル(&amp;C)</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Audio</source>
        <translation>音声(&amp;A)</translation>
    </message>
    <message>
        <location/>
        <source>C&amp;onfigure</source>
        <translation>設定(&amp;O)</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Help</source>
        <translation>ヘルプ(&amp;H)</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="-74"/>
        <source>Push-to-Talk</source>
        <comment>Global Shortcut</comment>
        <translation>プッシュトーク</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Push and hold this button to send voice.</source>
        <comment>Global Shortcut</comment>
        <translation>声を送るときに押し続けるボタン。</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This configures the push-to-talk button, and as long as you hold this button down, you will transmit voice.</source>
        <comment>Global Shortcut</comment>
        <translation>プッシュトークのボタンを設定する。このボタンを押している間あなたの声は送信される。</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Reset Audio Processor</source>
        <comment>Global Shortcut</comment>
        <translation>音声処理をリセット</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Toggle Mute Self</source>
        <comment>Global Shortcut</comment>
        <translation>自分の発言禁止を切り替え</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Toggle self-mute status.</source>
        <comment>Global Shortcut</comment>
        <translation>自分の発言禁止状態を切り替え。</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This will toggle your muted status. If you toggle this off, you will also disable self-deafen.</source>
        <comment>Global Shortcut</comment>
        <translation>これはあなたの消音状態を切り換えるでしょう。また、これをオフに切り替えると、聴取禁止も無効になるだろう。</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Toggle Deafen Self</source>
        <comment>Global Shortcut</comment>
        <translation>自分の聴取禁止を切り替え</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Toggle self-deafen status.</source>
        <comment>Global Shortcut</comment>
        <translation>自分の聴取禁止状態を切り替える。</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This will toggle your deafen status. If you toggle this on, you will also enable self-mute.</source>
        <comment>Global Shortcut</comment>
        <translation>これはあなたの聴取禁止状態を切り換えるでしょう。また、これをオンに切り替えると、発言禁止も有効になるだろう。</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Unlink Plugin</source>
        <comment>Global Shortcut</comment>
        <translation>プラグインを外す</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Force Center Position</source>
        <comment>Global Shortcut</comment>
        <translation>中央に移動させる</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Chan Parent</source>
        <comment>Global Shortcut</comment>
        <translation>親チャンネルに流す</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Chan Sub#%1</source>
        <comment>Global Shortcut</comment>
        <translation>サブチャンネル#%1に流す</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Chan All Subs</source>
        <comment>Global Shortcut</comment>
        <translation>全サブチャンネルに流す</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Push-to-Mute</source>
        <comment>Global Shortcut</comment>
        <translation>押してる間発言禁止</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Join Channel</source>
        <comment>Global Shortcut</comment>
        <translation>チャンネルに入る</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Toggle Overlay</source>
        <comment>Global Shortcut</comment>
        <translation>オーバーレイ表示切替</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Toggle state of in-game overlay.</source>
        <comment>Global Shortcut</comment>
        <translation>ゲーム中でのオーバーレイの状態を切り替える。</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This will switch the states of the ingame overlay between showing everybody, just the players who are talking, and nobody.</source>
        <comment>Global Shortcut</comment>
        <translation>ゲーム中でのオーバーレイの状態を「全員」と「話している人だけ」、「なし」の間で切り替える。</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Alt Push-to-Talk</source>
        <comment>Global Shortcut</comment>
        <translation>Alt発言有効化</translation>
    </message>
    <message>
        <location line="+527"/>
        <source>Kicking player %1</source>
        <translation>プレイヤー %1 をキックしました</translation>
    </message>
    <message>
        <location line="+0"/>
        <location line="+22"/>
        <source>Enter reason</source>
        <translation>理由を入力してください</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Banning player %1</source>
        <translation>プレイヤー %1 をBANしました</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>Mumble</source>
        <translation>Mumble</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="+129"/>
        <location line="+43"/>
        <source>Channel Name</source>
        <translation>チャンネル名</translation>
    </message>
    <message>
        <location line="-21"/>
        <source>Are you sure you want to delete %1 and all its sub-channels?</source>
        <translation>%1 とそのサブチャンネルを全て削除します。よろしいですか?</translation>
    </message>
    <message>
        <location line="+183"/>
        <source>Unmuted and undeafened.</source>
        <translation>発言禁止を解除し、聴取禁止も解除。</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Unmuted.</source>
        <translation>発言禁止を解除。</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Muted.</source>
        <translation>発言禁止。</translation>
    </message>
    <message>
        <location line="+20"/>
        <source>Muted and deafened.</source>
        <translation>発言禁止と聴取禁止。</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Deafened.</source>
        <translation>聴取禁止。</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Undeafened.</source>
        <translation>聴取禁止を解除。</translation>
    </message>
    <message>
        <location line="+62"/>
        <source>About Qt</source>
        <translation>Qtについて</translation>
    </message>
    <message>
        <location line="+97"/>
        <source>Joining %1.</source>
        <translation>%1 に入る。</translation>
    </message>
    <message>
        <source>Connected to server.</source>
        <translation type="obsolete">サーバーに接続しました。</translation>
    </message>
    <message>
        <location line="+131"/>
        <source>Server connection failed: %1.</source>
        <translation>サーバへの接続に失敗しました: %1.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Disconnected from server.</source>
        <translation>サーバから切断しました。</translation>
    </message>
    <message>
        <location line="-849"/>
        <source>Reconnecting.</source>
        <translation>再接続中.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="+56"/>
        <source>Joined server: %1.</source>
        <translation>サーバに接続しました: %1.</translation>
    </message>
    <message>
        <location line="+20"/>
        <source>Left server: %1.</source>
        <translation>サーバから切断しました: %1.</translation>
    </message>
    <message>
        <location line="+33"/>
        <source>%1 is now muted and deafened.</source>
        <translation>%1 が発言禁止/聴取禁止状態になりました。</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1 is now muted.</source>
        <translation>%1 が発言禁止状態になりました。</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1 is now unmuted.</source>
        <translation>%1 の発言禁止状態が解除されました。</translation>
    </message>
    <message>
        <location line="+13"/>
        <location line="+20"/>
        <source>server</source>
        <translation>サーバ</translation>
    </message>
    <message>
        <location line="-17"/>
        <source>You were muted by %1.</source>
        <translation>あなたは %1 によって発言禁止にされました。</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>You were unmuted by %1.</source>
        <translation>あなたは %1 によって発言禁止を解除されました。</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>%1 muted by %2.</source>
        <translation>%1 は %2 によって発言禁止にされました。</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>%1 unmuted by %2.</source>
        <translation>%1 は %2 によって発言禁止を解除されました。</translation>
    </message>
    <message>
        <location line="+17"/>
        <source>You were deafened by %1.</source>
        <translation>あなたは %1 によって聴取禁止にされました。</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>You were undeafened by %1.</source>
        <translation>あなたは %1 によって聴取禁止を解除されました。</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>%1 deafened by %2.</source>
        <translation>%1 は %2 によって聴取禁止にされました。</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>%1 undeafened by %2.</source>
        <translation>%1 は %2 によって聴取禁止を解除されました。</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>You were kicked from the server by %1: %2.</source>
        <translation>あなたは %1 によってサーバからキックされました: %2.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>%3 was kicked from the server by %1: %2.</source>
        <translation>%3 は %1 によってサーバからキックされました: %2.</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>You were kicked and banned from the server by %1: %2.</source>
        <translation>あなたは %1 によってキックされ、サーバからBANされました: %2.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>%3 was kicked and banned from the server by %1: %2.</source>
        <translation>%3 は %1 によってキックされ、サーバからBANされました: %2.</translation>
    </message>
    <message>
        <location line="+27"/>
        <source>You were moved to %1 by %2.</source>
        <translation>あなたは %2 によって %1 に移動されました。</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>%1 left channel.</source>
        <translation>%1 がチャンネルから退出しました。</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1 moved out by %2 to %3.</source>
        <translation>%1 は %2 によって %3 に移動させられました。</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>%1 moved in by %2 from %3.</source>
        <translation>%1 は %2 によって %3 から移動させられました。</translation>
    </message>
    <message>
        <source>%1 moved out by %2.</source>
        <translation type="obsolete">%1 は %2 によって移動させられました。</translation>
    </message>
    <message>
        <location line="-2"/>
        <source>%1 entered channel.</source>
        <translation>%1 がチャンネルに入りました。</translation>
    </message>
    <message>
        <source>%1 moved in by %2.</source>
        <translation type="obsolete">%1 は %2 によって移動させられました。</translation>
    </message>
    <message>
        <location line="+100"/>
        <source>Server connection rejected: %1.</source>
        <translation>サーバへの接続は拒否されました: %1.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Denied: %1.</source>
        <translation>拒否されました: %1.</translation>
    </message>
    <message>
        <location filename="main.cpp" line="+239"/>
        <source>Welcome to Mumble.</source>
        <translation>Mumbleへようこそ。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>&amp;Quit Mumble</source>
        <translation>終了(&amp;Q)</translation>
    </message>
    <message>
        <location/>
        <source>Closes the program</source>
        <translation>プログラムを終了</translation>
    </message>
    <message>
        <location/>
        <source>Exits the application.</source>
        <translation>アプリケーションを終了。</translation>
    </message>
    <message>
        <location/>
        <source>Send Messa&amp;ge</source>
        <translation>メッセージを送信(&amp;G)</translation>
    </message>
    <message>
        <location/>
        <source>Send a Text Message</source>
        <translation>テキストメッセージを送信</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="+217"/>
        <source>Sending message to %1</source>
        <translation>%1 にメッセージを送信しています</translation>
    </message>
    <message>
        <location line="+11"/>
        <location line="+253"/>
        <source>To %1: %2</source>
        <translation>%1 宛: %2</translation>
    </message>
    <message>
        <location line="-253"/>
        <location line="+253"/>
        <source>Message to %1</source>
        <translation>%1 へのメッセージ</translation>
    </message>
    <message>
        <location line="+381"/>
        <source>Invalid username</source>
        <translation>不正なユーザ名</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>You connected with an invalid username, please try another one.</source>
        <translation>不正なユーザ名で接続しました。他の名前を試してください。</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>That username is already in use, please try another username.</source>
        <translation>そのユーザ名は既に使われています。他の名前を試してください。</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Wrong password</source>
        <translation>誤ったパスワード</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Wrong password for registered users, please try again.</source>
        <translation>登録されたユーザのパスワードが違います。再度試してください。</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Wrong server password for unregistered user account, please try again.</source>
        <translation>未登録ユーザのパスワードが違います。再度試してください。</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="+27"/>
        <source>From %1: %2</source>
        <translation>%1 より: %2</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Message from %1</source>
        <translation>%1 からのメッセージ</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>&amp;Audio Wizard</source>
        <translation>音声ウィザード(&amp;A)</translation>
    </message>
    <message>
        <location/>
        <source>Sends a text message to another user.</source>
        <translation>他のユーザにテキストメッセージを送信します。</translation>
    </message>
    <message>
        <location/>
        <source>Start the audio configuration wizard</source>
        <translation>音声の設定ウィザードを開始します</translation>
    </message>
    <message>
        <location/>
        <source>This will guide you through the process of configuring your audio hardware.</source>
        <translation>サウンドハードウェアの設定プロセスに案内します。</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="-62"/>
        <source>SSL Verification failed: %1</source>
        <translation>SSL検証エラー: %1</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>&lt;b&gt;WARNING:&lt;/b&gt; The server presented a certificate that was different from the stored one.</source>
        <translation>&lt;b&gt;警告:&lt;/b&gt; 送信された証明書は保存されたものとは異なっています。</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Sever presented a certificate which failed verification.</source>
        <translation>送信された証明書の検証に失敗しました。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>&amp;Information</source>
        <translation>情報(&amp;I)</translation>
    </message>
    <message>
        <location/>
        <source>Show information about the server connection</source>
        <translation>サーバとの接続についての情報を表示</translation>
    </message>
    <message>
        <location/>
        <source>This will show extended information about the connection to the server.</source>
        <translation>サーバとの接続についての詳細な情報を表示します。</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="-975"/>
        <source>Opening URL %1</source>
        <translation>URL %1 を開きます</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>URL scheme is not &apos;mumble&apos;</source>
        <translation>URLスキームは &apos;mumble&apos; ではありません</translation>
    </message>
    <message>
        <location line="+25"/>
        <source>Connecting to %1</source>
        <translation>%1に接続中</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Enter username</source>
        <translation>ユーザ名を入力してください</translation>
    </message>
    <message>
        <location line="+205"/>
        <source>Mumble Server Information</source>
        <translation>Mumble サーバの情報</translation>
    </message>
    <message>
        <location line="+4"/>
        <location line="+753"/>
        <source>&amp;View Certificate</source>
        <translation>証明書を見る(&amp;V)</translation>
    </message>
    <message>
        <location line="-7"/>
        <source>&lt;p&gt;%1.&lt;br /&gt;The specific errors with this certificate are: &lt;/p&gt;&lt;ol&gt;%2&lt;/ol&gt;&lt;p&gt;Do you wish to accept this certificate anyway?&lt;br /&gt;(It will also be stored so you won&apos;t be asked this again.)&lt;/p&gt;</source>
        <translation>&lt;p&gt;%1.&lt;br /&gt;詳細な証明書エラーの内容: &lt;/p&gt;&lt;ol&gt;%2&lt;/ol&gt;&lt;p&gt;この証明書を受け入れますか？&lt;br /&gt;(同時に証明書を保存するので、今後この質問は表示されません。)&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="-769"/>
        <source>&lt;h2&gt;Control channel&lt;/h2&gt;&lt;p&gt;Encrypted with %1 bit %2&lt;br /&gt;%3 ms average latency (%4 variance)&lt;/p&gt;</source>
        <translation>&lt;h2&gt;制御チャンネル&lt;/h2&gt;&lt;p&gt;%1 bit %2 で暗号化されています。&lt;br /&gt;平均 %3 ms の遅延 (平方偏差 %4)&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Voice channel is sent over control channel.</source>
        <translation>音声チャンネルは管理チャンネルを通して送信されます。</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>&lt;h2&gt;Voice channel&lt;/h2&gt;&lt;p&gt;Encrypted with 128 bit OCB-AES128&lt;br /&gt;%1 ms average latency (%4 variance)&lt;/p&gt;</source>
        <translation>&lt;h2&gt;音声チャンネル&lt;/h2&gt;&lt;p&gt;128 bit OCB-AES128 で暗号化されています。&lt;br /&gt;平均 %1 ms の遅延 (平方偏差 %4)&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>UDP Statistics</source>
        <translation>UDP統計</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>To Server</source>
        <translation>サーバ宛</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>From Server</source>
        <translation>サーバから</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Good</source>
        <translation>良好</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Late</source>
        <translation>遅延</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Lost</source>
        <translation>消失</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Resync</source>
        <translation>最同期</translation>
    </message>
    <message>
        <location line="+326"/>
        <source>Change description of channel %1</source>
        <translation>チャンネル 「%1」 の説明を変更する</translation>
    </message>
    <message>
        <location line="+79"/>
        <source>Sending message to channel %1</source>
        <translation>チャンネル %1 に送信するメッセージ</translation>
    </message>
    <message>
        <location line="+27"/>
        <source>Sending message to channel tree %1</source>
        <translation>チャンネルツリー %1 に送信するメッセージ</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>To tree %1: %2</source>
        <translation>tree %1 宛: %2</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Message to tree %1</source>
        <translation>tree %1 宛メッセージ</translation>
    </message>
    <message>
        <source>You are now in local deafen mode. This mode is not reflected on the server, and you will still be transmitting voice to the server. This mode should only be used if there are several people in the same room and one of them have Mumble on loudspeakers.</source>
        <translation type="obsolete">ローカルな聴取禁止モードになりました。このモードはサーバーには反映されません。そしてあなたの声はまだ送信され続けています。このモードは、同じ部屋に数人の人がいて、そのうちの1人がMumbleを大音量のスピーカーで使っているような時だけ使用すればよいでしょう。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>Ctrl+Q</source>
        <translation>Ctrl+Q</translation>
    </message>
    <message>
        <location/>
        <source>Mute player locally</source>
        <translation>ローカルでプレイヤーを発言禁止にする</translation>
    </message>
    <message>
        <location/>
        <source>Mute or unmute player locally. Use this on other players in the same room.</source>
        <translation>ローカルでプレイヤーを発言禁止にします。同じ部屋にいるプレイヤーに使ってください。</translation>
    </message>
    <message>
        <source>&amp;Rename Channel</source>
        <translation type="obsolete">チャンネル名を変更(&amp;R)</translation>
    </message>
    <message>
        <location/>
        <source>Renames the channel</source>
        <translation>チャンネル名を変更します</translation>
    </message>
    <message>
        <location/>
        <source>This renames a channel.</source>
        <translation>チャンネルの名前を変更します。</translation>
    </message>
    <message>
        <source>&amp;Local deafen</source>
        <translation type="obsolete">ローカルで聴取禁止にする(&amp;L)</translation>
    </message>
    <message>
        <source>Silence loudspeakers</source>
        <translation type="obsolete">スピーカーをとめる</translation>
    </message>
    <message>
        <source>Sets you in local deafen mode, wherein your speakers are silenced but you are still transmitting audio.</source>
        <translation type="obsolete">ローカルな聴取禁止モードに設定します。あなたのスピーカーは音を出しませんが、送信はなお続いています。</translation>
    </message>
    <message>
        <location/>
        <source>About &amp;Qt</source>
        <translation>&amp;Qtについて</translation>
    </message>
    <message>
        <location/>
        <source>Sends a text message to all users in a channel.</source>
        <translation>チャンネル内の全てのユーザにテキストメッセージを送信します。</translation>
    </message>
    <message>
        <location/>
        <source>Send &amp;Tree Message</source>
        <translation>ツリーにメッセージを送信(&amp;T)</translation>
    </message>
    <message>
        <location/>
        <source>Send message to channel tree.</source>
        <translation>チャンネルツリーにメッセージを送信します。</translation>
    </message>
    <message>
        <location/>
        <source>This sends a text message to a channel and it&apos;s subchannels.</source>
        <translation>チャンネルとそのサブチャンネルにテキストメッセージを送信します。</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="-12"/>
        <source>Server maximum bandwidth is only %1 kbit/s. Quality auto-adjusted.</source>
        <translation>サーバの最大帯域幅は %1 kbit/s しかありません。品質は自動的に調整されます。</translation>
    </message>
    <message>
        <location filename="main.cpp" line="+17"/>
        <source>This is the first time you&apos;re starting Mumble.&lt;br /&gt;Would you like to go through the Audio Wizard to configure your soundcard?</source>
        <translation>初めての起動です。&lt;br /&gt;サウンドカードの設定をするために音声ウィザードに移動しますか?</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="-441"/>
        <source>&lt;h2&gt;Audio bandwidth&lt;/h2&gt;&lt;p&gt;Maximum %1 kbit/s&lt;br /&gt;Current %2 kbit/s (Quality %3)&lt;/p&gt;</source>
        <translation>&lt;h2&gt;音声帯域幅&lt;/h2&gt;&lt;p&gt;最大値 %1 kbit/s&lt;br /&gt;現在値 %2 kbit/s (品質 %3)&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+682"/>
        <source>Connected to server %1.</source>
        <translation>サーバ %1 に接続しました。</translation>
    </message>
    <message>
        <location line="-1001"/>
        <source>Mumble is currently connected to a server. Do you want to Close or Minimize it?</source>
        <translation>サーバに接続中です。接続を閉じるか最小化したいですか?</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Close</source>
        <translation>閉じる</translation>
    </message>
    <message>
        <location line="-75"/>
        <location line="+76"/>
        <source>Minimize</source>
        <translation>最小化</translation>
    </message>
    <message>
        <location line="-78"/>
        <source>&amp;Window</source>
        <translation>ウインドウ(&amp;W)</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Ctrl+M</source>
        <translation>Ctrl+M</translation>
    </message>
    <message>
        <source>Ctrl+W</source>
        <translation type="obsolete">Ctrl+W</translation>
    </message>
    <message>
        <location line="-25"/>
        <source>Toggle Minimal</source>
        <comment>Global Shortcut</comment>
        <translation>小型切り替え</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>&amp;Minimal View</source>
        <translation>小さく表示(&amp;M)</translation>
    </message>
    <message>
        <location/>
        <source>Toggle minimal window modes</source>
        <translation>小型ウインドウモードの切り替え</translation>
    </message>
    <message>
        <location/>
        <source>This will toggle minimal mode, where the log window and menu is hidden.</source>
        <translation>ログウインドウとメニューを省いた小型ウインドウの切り替えをします。</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="+3"/>
        <source>Volume Up (+10%)</source>
        <comment>Global Shortcut</comment>
        <translation>音量を上げる(+10%)</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Volume Down (-10%)</source>
        <comment>Global Shortcut</comment>
        <translation>音量を下げる(-10%)</translation>
    </message>
    <message>
        <location line="+176"/>
        <source>Clear</source>
        <translation>クリア</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>Log</source>
        <translation>ログ</translation>
    </message>
    <message>
        <location filename="PlayerModel.cpp" line="+995"/>
        <source>You have Channel Dragging set to &quot;Do Nothing&quot; so the channel wasn&apos;t moved.</source>
        <translation>チャンネルをドラッグしたときに &quot;何もしない&quot; ようにします; チャンネルが移動しなくなります。</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Unknown Channel Drag mode in PlayerModel::dropMimeData.</source>
        <translation>不明なチャンネルドラッグモード, PlayerModel::dropMimeData。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>&amp;Unlink</source>
        <comment>Channel</comment>
        <translation>リンク解除(&amp;U)</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Unlink</source>
        <comment>Plugin</comment>
        <translation>リンク解除(&amp;U)</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="+10"/>
        <source>Server</source>
        <comment>message from</comment>
        <translation>サーバ</translation>
    </message>
    <message>
        <source>&amp;Quit</source>
        <translation type="obsolete">終了(&amp;Q)</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>Hide Frame</source>
        <translation>フレームを隠す</translation>
    </message>
    <message>
        <location/>
        <source>Toggle showing frame on minimal window</source>
        <translation>小さく表示したときにフレームを見せるか見せないかを切り替えます</translation>
    </message>
    <message>
        <location/>
        <source>This will toggle whether the minimal window should have a frame for moving and resizing.</source>
        <translation>小さく表示したとき移動やサイズ変更のためにフレームを表示するかを切り替えます。</translation>
    </message>
</context>
<context>
    <name>NetworkConfig</name>
    <message>
        <location filename="NetworkConfig.cpp" line="+51"/>
        <source>Network</source>
        <translation>ネットワーク</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui"/>
        <source>Form</source>
        <translation>フォーム</translation>
    </message>
    <message>
        <location/>
        <source>Connection</source>
        <translation>接続</translation>
    </message>
    <message>
        <location/>
        <source>Use TCP compatibility mode</source>
        <oldsource>Use TCP compatability mode</oldsource>
        <translation>TCP 互換モードを使用する</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Enable TCP compatibility mode&lt;/b&gt;.&lt;br /&gt;This will make Mumble use only TCP when communicating with the server. This will increase overhead and cause lost packets to produce noticeable pauses in communication, so this should only be used if you are unable to use the default (which uses UDP for voice and TCP for control).</source>
        <translation>&lt;b&gt;TCP互換モードを有効にします。&lt;/b&gt;.&lt;br /&gt;この設定をするとMumbleがサーバとの通信にTCPのみを使用するようになります。この設定は通信のオーバーヘッドを増大させ、通信時に音声が途切れる原因になるので、デフォルトの設定(音声:UDP 操作:TCP)が使用できない場合だけ使用するのが良いでしょう。</translation>
    </message>
    <message>
        <location/>
        <source>Use TCP mode</source>
        <translation>TCPモードを使用する</translation>
    </message>
    <message>
        <location/>
        <source>Reconnect when disconnected</source>
        <translation>切断時に再接続を試みます</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Reconnect when disconnected&lt;/b&gt;.&lt;br /&gt;This will make Mumble try to automatically reconnect after 10 seconds if your server connection fails.</source>
        <translation>&lt;b&gt;切断時に再接続を試みます。&lt;/b&gt;&lt;br /&gt;この設定をすると、サーバへの接続が失敗した際、10秒後に再接続を行います。</translation>
    </message>
    <message>
        <location/>
        <source>Reconnect automatically</source>
        <translation>自動的に再接続する</translation>
    </message>
    <message>
        <location/>
        <source>Proxy</source>
        <translation>プロキシ</translation>
    </message>
    <message>
        <location/>
        <source>Type</source>
        <translation>タイプ</translation>
    </message>
    <message>
        <location/>
        <source>Type of proxy to connect through</source>
        <translation>プロキシの接続タイプ</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Type of proxy to connect through&lt;/b&gt;&lt;br /&gt;This makes Mumble connect through a proxy for all outgoing connections. Note: Proxy tunneling forces Mumble into TCP compatibility mode, causing all voice data to be sent via the control channel.</source>
        <oldsource>&lt;b&gt;Type of proxy to connect through&lt;/b&gt;&lt;br /&gt;This makes Mumble connect through a proxy for all outgoing connections. Note: Proxy tunneling forces Mumble into TCP compatability mode, causing all voice data to be sent via the control channel.</oldsource>
        <translation>&lt;b&gt;プロキシの接続タイプ&lt;/b&gt;&lt;br /&gt;プロキシを通して外向きの接続を行うようにします。 注意: プロキシ接続をする場合、強制的にTCP互換モードに変更されます。 全ての音声データは管理チャンネルを通して送信されるようになります。</translation>
    </message>
    <message>
        <location/>
        <source>Direct connection</source>
        <translation>直接接続</translation>
    </message>
    <message>
        <location/>
        <source>HTTP(S) proxy</source>
        <translation>HTTP(S) プロキシ</translation>
    </message>
    <message>
        <location/>
        <source>SOCKS5 proxy</source>
        <translation>SOCKS5 プロキシ</translation>
    </message>
    <message>
        <location/>
        <source>Hostname</source>
        <translation>ホスト名</translation>
    </message>
    <message>
        <location/>
        <source>Hostname of the proxy</source>
        <translation>プロキシのホスト名</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Hostname of the proxy.&lt;b&gt;&lt;br /&gt;This field specifies the hostname of the proxy you wish to tunnel network traffic through.</source>
        <translation>&lt;b&gt;プロキシサーバのホスト名&lt;b&gt;&lt;br /&gt;あなたが使用したいプロキシサーバのホスト名を指定します。</translation>
    </message>
    <message>
        <location/>
        <source>Port</source>
        <translation>ポート</translation>
    </message>
    <message>
        <location/>
        <source>Port number of the proxy</source>
        <translation>プロキシのポート番号</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Port number of the proxy.&lt;/b&gt;&lt;br /&gt;This field specifies the port number that the proxy expects connections on.</source>
        <translation>&lt;b&gt;プロキシのポート番号&lt;/b&gt;&lt;br /&gt;プロキシへの接続に使うポート番号を指定します。</translation>
    </message>
    <message>
        <location/>
        <source>Username</source>
        <translation>ユーザ名</translation>
    </message>
    <message>
        <location/>
        <source>Username for proxy authentication</source>
        <translation>プロキシ認証のユーザ名</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Username for proxy authentication.&lt;/b&gt;&lt;br /&gt;This specifies the username you use for authenticating yourself with the proxy. In case the proxy does not use authentication, or you want to connect anonymously, simply leave this field blank.</source>
        <translation>&lt;b&gt;プロキシ認証のユーザ名&lt;/b&gt;&lt;br /&gt;プロキシサーバの認証に使用するユーザ名を指定します。プロキシサーバが認証を行わなない場合や、匿名で接続する場合はこのフィールドを空にしてください。</translation>
    </message>
    <message>
        <location/>
        <source>Password</source>
        <translation>パスワード</translation>
    </message>
    <message>
        <location/>
        <source>Password for proxy authentication</source>
        <translation>プロキシ認証のパスワード</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Password for proxy authentication.&lt;/b&gt;&lt;br /&gt;This specifies the password you use for authenticating yourself with the proxy. In case the proxy does not use authentication, or you want to connect anonymously, simply leave this field blank.</source>
        <translation>&lt;b&gt;プロキシ認証のパスワード&lt;/b&gt;&lt;br /&gt;プロキシサーバの認証に使うパスワードを指定します。プロキシサーバが認証を行わない場合や、匿名で接続する場合はこのフィールドを空にしてください。</translation>
    </message>
    <message>
        <location/>
        <source>Mumble services</source>
        <translation>Mumble サービス</translation>
    </message>
    <message>
        <location/>
        <source>Check for updates on startup</source>
        <translation>起動時に更新をチェックする</translation>
    </message>
    <message>
        <location/>
        <source>Submit anonymous statistics to the Mumble project</source>
        <translation>Mumbleプロジェクトに匿名で統計を送信する</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Submit anonymous statistics.&lt;/b&gt;&lt;br /&gt;Mumble has a small development team, and as such needs to focus it&apos;s development where it&apos;s needed most. By submitting a bit of statistics you help the project determine where to focus development.</source>
        <translation>&lt;b&gt;匿名で統計を送信します。&lt;/b&gt;&lt;br /&gt;Mumbleは小さなチームで開発を行っているためもっとも必要な機能に絞って開発をしなければなりません。統計情報の送信はプロジェクトがどこに注力して開発すれば良いかを決定するための指針になります。</translation>
    </message>
    <message>
        <location/>
        <source>Submit anonymous statistics</source>
        <translation>匿名で利用状況の統計を送信する</translation>
    </message>
</context>
<context>
    <name>OSSConfig</name>
    <message>
        <source>%1ms</source>
        <translation type="obsolete">%1ms</translation>
    </message>
    <message>
        <source>Form</source>
        <translation type="obsolete">フォーム</translation>
    </message>
    <message>
        <source>Device selection</source>
        <translation type="obsolete">デバイス選択</translation>
    </message>
    <message>
        <source>Input</source>
        <translation type="obsolete">入力</translation>
    </message>
    <message>
        <source>Device to use for microphone</source>
        <translation type="obsolete">マイクに使用するデバイス</translation>
    </message>
    <message>
        <source>Output</source>
        <translation type="obsolete">出力</translation>
    </message>
    <message>
        <source>Device to use for speakers/headphones</source>
        <translation type="obsolete">スピーカー/ヘッドホンに使用するデバイス</translation>
    </message>
    <message>
        <source>Output Options</source>
        <translation type="obsolete">出力オプション</translation>
    </message>
    <message>
        <source>Output Delay</source>
        <translation type="obsolete">出力猶予</translation>
    </message>
    <message>
        <source>Amount of data to buffer for OSS</source>
        <translation type="obsolete">OSS用バッファデータの量</translation>
    </message>
    <message>
        <source>This sets the amount of data to prebuffer in the output buffer. Experiment with different values and set it to the lowest which doesn&apos;t cause rapid jitter in the sound.</source>
        <translation type="obsolete">出力バッファにおけるプリバッファの量を設定します。いろいろな値で試してみて、急に不安定にならない一番小さな値に設定してください。</translation>
    </message>
    <message>
        <source>TextLabel</source>
        <translation type="obsolete">テキストラベル</translation>
    </message>
    <message>
        <source>This set which device mumble should use. The &lt;i&gt;default&lt;/i&gt; device is your primary sound device.</source>
        <translation type="obsolete">Mumbleがどのデバイスを使用するか設定します。&lt;i&gt;デフォルト&lt;/i&gt;デバイスはプライマリーのサウンドデバイスです。</translation>
    </message>
</context>
<context>
    <name>Overlay</name>
    <message>
        <source>Mumble</source>
        <translation type="obsolete">Mumble</translation>
    </message>
    <message>
        <source>Failed to load overlay library. This means either that:
- the library (mumble_ol.dll) wasn&apos;t found in the directory you ran Mumble from
- you&apos;re on an OS earlier than WinXP SP2
- you do not have the August 2006 updated version of DX9.0c</source>
        <translation type="obsolete">オーバーレイライブラリの読み込みに失敗しました。 原因は次のうちのどれかです:
- ライブラリ (mumble_ol.dll) が起動していたMumbleのディレクトリに存在しなかった
- WinXP SP2 以前のOSを使っている
- 2006年8月にアップデートされたバージョンの DX9.0x を使っていない</translation>
    </message>
    <message>
        <source>Failed to load overlay library. This means either that:
- the library (mumble_ol.dll) wasn&apos;t found in the directory you ran Mumble from
- you&apos;re on an OS earlier than WinXP SP2
- you do not have the June 2007 updated version of DX9.0c</source>
        <translation type="obsolete">オーバーレイライブラリの読み込みに失敗しました。 原因は次のうちのどれかです:
- ライブラリ (mumble_ol.dll) が起動していたMumbleのディレクトリに存在しなかった
- WinXP SP2 以前のOSを使っている
- 2007年6月にアップデートされたバージョンの DX9.0x を使っていない</translation>
    </message>
</context>
<context>
    <name>OverlayConfig</name>
    <message>
        <source>Options</source>
        <translation type="obsolete">オプション</translation>
    </message>
    <message>
        <source>Position</source>
        <translation type="obsolete">ポジション</translation>
    </message>
    <message>
        <source>Font</source>
        <translation type="obsolete">フォント</translation>
    </message>
    <message>
        <source>Enable Overlay</source>
        <translation type="obsolete">オーバーレイを有効にする</translation>
    </message>
    <message>
        <source>Enable overlay.</source>
        <translation type="obsolete">オーバーレイを有効にします。</translation>
    </message>
    <message>
        <source>This sets whether the overlay is enabled or not. This setting is only checked with D3D9 applications are started, so make sure Mumble is running and this option is on before you start the application.&lt;br /&gt;Please note that if you start the application after starting Mumble, or if you disable the overlay while running, there is no safe way to restart the overlay without also restarting the application.</source>
        <translation type="obsolete">オーバーレイを有効にするかどうか決定します。この設定はD3D9を使ったアプリケーションが起動された時だけチェックされます。ですので、アプリケーションを起動する前にMumbleが起動していて、このオプションが有効になっているか確認してください。&lt;br /&gt;Mumbleを起動してからアプリケーションを起動し、起動中にオーバーレイを無効にした場合、アプリケーションの再起動をしないでオーバーレイを再開させる安全な方法はありませんのでご注意ください。</translation>
    </message>
    <message>
        <source>No one</source>
        <translation type="obsolete">なし</translation>
    </message>
    <message>
        <source>Only talking</source>
        <translation type="obsolete">話している人だけ</translation>
    </message>
    <message>
        <source>Everyone</source>
        <translation type="obsolete">全員</translation>
    </message>
    <message>
        <source>Show</source>
        <translation type="obsolete">表示</translation>
    </message>
    <message>
        <source>Who to show on the overlay</source>
        <translation type="obsolete">オーバーレイに誰を表示するか</translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets who to show in the in-game overlay.&lt;/b&gt;&lt;br /&gt;If many people are connected to the same channel, the overlay list might be very long. Use this to shorten it.&lt;br /&gt;&lt;i&gt;No one&lt;/i&gt; - Don&apos;t show anyone (but leave overlay running).&lt;br /&gt;&lt;i&gt;Only talking&lt;/i&gt; - Only show talking people.&lt;br /&gt;&lt;i&gt;Everyone&lt;/i&gt; - Show everyone.</source>
        <translation type="obsolete">&lt;b&gt;ゲーム中のオーバーレイに誰を表示するか決定します。&lt;/b&gt;&lt;br /&gt;多くの人が同じチャンネルにいる場合、リストはとても長くなるでしょう。&lt;br /&gt;&lt;i&gt;なし&lt;/i&gt; - 誰も表示ません。(それでもオーバーレイは動いています)&lt;br /&gt;&lt;i&gt;話している人だけ&lt;/i&gt; - 話している人だけ表示します。&lt;br /&gt;&lt;i&gt;全員&lt;/i&gt; - 全員表示します。</translation>
    </message>
    <message>
        <source>Always Show Self</source>
        <translation type="obsolete">常に自分を表示</translation>
    </message>
    <message>
        <source>Always show yourself on overlay.</source>
        <translation type="obsolete">オーバーレイで常に自分を表示</translation>
    </message>
    <message>
        <source>This sets whether to always show yourself or not. This setting is useful if you aren&apos;t showing everyone in the overlay, as then you would only see your own status if you were talking, which wouldn&apos;t let you see that you were deafened or muted.</source>
        <translation type="obsolete">自分自身を常に表示するかどうか決定します。この設定は全員表示にしていない時に効果的です。自分が話しているかの状況だけを確認することができます。あなたが発言禁止や聴取禁止になっている場合には表示されません。</translation>
    </message>
    <message>
        <source>Grow Left</source>
        <translation type="obsolete">左に延長</translation>
    </message>
    <message>
        <source>Let overlay grow to the left</source>
        <translation type="obsolete">オーバーレイを左に延長させる</translation>
    </message>
    <message>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow to the left if needed.</source>
        <translation type="obsolete">オーバーレイは選択した位置にできるだけ小さく留まろうとします。この設定は、必要ならオーバーレイが左に伸びることを許可します。</translation>
    </message>
    <message>
        <source>Grow Right</source>
        <translation type="obsolete">右に延長</translation>
    </message>
    <message>
        <source>Let overlay grow to the Right</source>
        <translation type="obsolete">オーバーレイを右に延長させる</translation>
    </message>
    <message>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow to the Right if needed.</source>
        <translation type="obsolete">オーバーレイは選択した位置にできるだけ小さく留まろうとします。この設定は、必要ならオーバーレイが右に伸びることを許可します。</translation>
    </message>
    <message>
        <source>Grow Up</source>
        <translation type="obsolete">上に延長</translation>
    </message>
    <message>
        <source>Let overlay grow upwards</source>
        <translation type="obsolete">オーバーレイを上方向に延長させる</translation>
    </message>
    <message>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow to the top if needed.</source>
        <translation type="obsolete">オーバーレイは選択した位置にできるだけ小さく留まろうとします。この設定は、必要ならオーバーレイが上に伸びることを許可します。</translation>
    </message>
    <message>
        <source>Grow Down</source>
        <translation type="obsolete">下に延長</translation>
    </message>
    <message>
        <source>Let overlay grow downwards</source>
        <translation type="obsolete">オーバーレイを下方向に延長させる</translation>
    </message>
    <message>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow towards the bottom if needed.</source>
        <translation type="obsolete">オーバーレイは選択した位置にできるだけ小さく留まろうとします。この設定は、必要ならオーバーレイが下に伸びることを許可します。</translation>
    </message>
    <message>
        <source>X-Position of Overlay</source>
        <translation type="obsolete">オーバーレイの横軸の位置</translation>
    </message>
    <message>
        <source>This sets the relative X position of the overlay.</source>
        <translation type="obsolete">オーバーレイの相対的なX座標を決定します。</translation>
    </message>
    <message>
        <source>Y-Position of Overlay</source>
        <translation type="obsolete">オーバーレイの縦軸の位置</translation>
    </message>
    <message>
        <source>This sets the relative Y position of the overlay.</source>
        <translation type="obsolete">オーバーレイの相対的なY座標を決定します。</translation>
    </message>
    <message>
        <source>Current Font</source>
        <translation type="obsolete">現在のフォント</translation>
    </message>
    <message>
        <source>Set Font</source>
        <translation type="obsolete">フォント設定</translation>
    </message>
    <message>
        <source>Maximum width of names.</source>
        <translation type="obsolete">最大の高さ</translation>
    </message>
    <message>
        <source>This sets the maximum width of names shown, relative to the height. If you set this to 100%, no line of text will be wider than it is high. At 500%, no line will be more than 5 times wider than its height, and so on.</source>
        <translation type="obsolete">高さに対する名前表示の最大サイズを設定します。100%にした場合、テキスト行の高さはそれより大きくなりません。500%で、行はそれらの5倍より大きくはなりませんといった感じです。</translation>
    </message>
    <message>
        <source>Maximum width</source>
        <translation type="obsolete">最大の幅</translation>
    </message>
    <message>
        <source>Change</source>
        <translation type="obsolete">変更</translation>
    </message>
    <message>
        <source>Color for players</source>
        <translation type="obsolete">プレイヤーの色</translation>
    </message>
    <message>
        <source>Color for talking players</source>
        <translation type="obsolete">話しているプレイヤーの色</translation>
    </message>
    <message>
        <source>Color for alt-talking players</source>
        <translation type="obsolete">Alt発言をしているプレイヤーの色</translation>
    </message>
    <message>
        <source>Color for Channels</source>
        <translation type="obsolete">チャンネルの色</translation>
    </message>
    <message>
        <source>Color for active Channels</source>
        <translation type="obsolete">有効なチャンネルの色</translation>
    </message>
    <message>
        <source>Overlay</source>
        <translation type="obsolete">オーバーレイ</translation>
    </message>
    <message>
        <source>Show User Textures</source>
        <translation type="obsolete">ユーザテクスチャを表示</translation>
    </message>
    <message>
        <source>Show User custom textures instead of text on the overlay.</source>
        <translation type="obsolete">テキストの代わりにユーザのカスタムテクスチャをオーバーレイに表示</translation>
    </message>
    <message>
        <source>This sets whether to download and use custom textures for registered users. If disabled, the regular outline text will be used instead.</source>
        <translation type="obsolete">登録されたユーザのカスタムテクスチャをダウンロードして使用するかどうかを決定します。無効の時は通常のテキストが代わりに使われます。</translation>
    </message>
    <message>
        <source>Maximum height of names.</source>
        <translation type="obsolete">名前の最大の高さ</translation>
    </message>
    <message>
        <source>This sets the maximum height of names shown, relative to the screen height. If your active 3D window is 800 pixels tall and this is set to 5%, each name will be 40 pixels tall. Note that the names will not be taller than 60 pixels no matter what you set here.</source>
        <translation type="obsolete">画面の高さに対する名前表示の最大の高さを決定します。あなたのゲーム画面が 800ピクセルの高さでこれが5%にセットされているとき、各名前は 40ピクセルの高さで表示されます。注意: ここの値に関わらず、名前の高さは 60ピクセル以上にはなりません。</translation>
    </message>
    <message>
        <source>Maximum height</source>
        <translation type="obsolete">最大の高さ</translation>
    </message>
    <message>
        <source>Show no one</source>
        <translation type="obsolete">なし</translation>
    </message>
    <message>
        <source>Show only talking</source>
        <translation type="obsolete">話している人だけ</translation>
    </message>
    <message>
        <source>Show everyone</source>
        <translation type="obsolete">全員</translation>
    </message>
    <message>
        <source>Form</source>
        <translation type="obsolete">フォーム</translation>
    </message>
    <message>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow upwards if needed.</source>
        <translation type="obsolete">オーバーレイはあなたが選択した位置で最小の状態で表示されます。この設定を有効にすると、必要に応じてオーバーレイが上方向に伸びます。</translation>
    </message>
    <message>
        <source>Let overlay grow to the right</source>
        <translation type="obsolete">オーバーレイを右に延長させる</translation>
    </message>
    <message>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow to the right if needed.</source>
        <translation type="obsolete">オーバーレイはあなたが選択した位置で最小の状態で表示されます。この設定を有効にすると、必要に応じてオーバーレイが右方向に伸びます。</translation>
    </message>
    <message>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow downwards if needed.</source>
        <translation type="obsolete">オーバーレイはあなたが選択した位置で最小の状態で表示されます。この設定を有効にすると、必要に応じてオーバーレイが下方向に伸びます。</translation>
    </message>
    <message>
        <source>TextLabel</source>
        <translation type="obsolete">テキストラベル</translation>
    </message>
</context>
<context>
    <name>PAAudioConfig</name>
    <message>
        <source>PortAudio</source>
        <translation type="obsolete">PortAudio</translation>
    </message>
    <message>
        <source>Input Device</source>
        <translation type="obsolete">入力デバイス</translation>
    </message>
    <message>
        <source>Output Device</source>
        <translation type="obsolete">出力デバイス</translation>
    </message>
</context>
<context>
    <name>PlayerModel</name>
    <message>
        <source>Name of player</source>
        <translation type="obsolete">プレイヤーの名前</translation>
    </message>
    <message>
        <source>Name of channel</source>
        <translation type="obsolete">チャンネルの名前</translation>
    </message>
    <message>
        <source>Player flags</source>
        <translation type="obsolete">プレイヤーのフラグ</translation>
    </message>
    <message>
        <source>This is a player connected to the server. The icon to the left of the player indicates whether or not they are talking:&lt;br /&gt;&lt;img src=&quot;:/icons/talking_on.png&quot; /&gt; Talking&lt;br /&gt;&lt;img src=&quot;:/icons/talking_off.png&quot; /&gt; Not talking</source>
        <translation type="obsolete">サーバーに接続しているプレイヤーです。プレイヤーの左のアイコンはその人が話しているかどうかを示しています:&lt;br /&gt;&lt;img src=&quot;:/icons/talking_on.png&quot; /&gt; 話している&lt;br /&gt;&lt;img src=&quot;:/icons/talking_off.png&quot; /&gt; 話していない</translation>
    </message>
    <message>
        <location filename="PlayerModel.cpp" line="-575"/>
        <source>This is a channel on the server. Only players in the same channel can hear each other.</source>
        <translation>サーバにあるチャンネルです。同じチャンネルにいるプレイヤーしかお互いの会話を聞くことはできません。</translation>
    </message>
    <message>
        <source>This shows the flags the player has on the server, if any:&lt;br /&gt;&lt;img src=&quot;:/icons/authenticated.png&quot; /&gt;Authenticated user&lt;br /&gt;&lt;img src=&quot;:/icons/muted_self.png&quot; /&gt;Muted (by self)&lt;br /&gt;&lt;img src=&quot;:/icons/muted_server.png&quot; /&gt;Muted (by admin)&lt;br /&gt;&lt;img src=&quot;:/icons/deafened_self.png&quot; /&gt;Deafened (by self)&lt;br /&gt;&lt;img src=&quot;:/icons/deafened_server.png&quot; /&gt;Deafened (by admin)&lt;br /&gt;A player muted by himself is probably just away, talking on the phone or something like that.&lt;br /&gt;A player muted by an admin is probably also just away, and the noise the player is making was annoying enough that an admin muted him.</source>
        <translation type="obsolete">サーバー内のプレイヤーが持っているフラグを表示します:&lt;br /&gt;&lt;img src=&quot;:/icons/authenticated.png&quot; /&gt;認証されたユーザー&lt;br /&gt;&lt;img src=&quot;:/icons/muted_self.png&quot; /&gt;発言禁止(自分によって)&lt;br /&gt;&lt;img src=&quot;:/icons/muted_server.png&quot; /&gt;発言禁止(管理者によって)&lt;br /&gt;&lt;img src=&quot;:/icons/deafened_self.png&quot; /&gt;聴取禁止(自分によって)&lt;br /&gt;&lt;img src=&quot;:/icons/deafened_server.png&quot; /&gt;聴取禁止(管理者によって)&lt;br /&gt;自分で発言禁止にしているプレイヤーは、おそらくちょっと離席しているだけでしょう。&lt;br /&gt;管理者によって発言禁止にされているプレイヤーは同じく離席しているだけか、管理者が発言禁止するに値するほどその人の発する雑音がうるさかったのでしょう。</translation>
    </message>
    <message>
        <location line="+26"/>
        <source>Name</source>
        <translation>チャンネル名</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Flags</source>
        <translation>フラグ</translation>
    </message>
    <message>
        <location line="-34"/>
        <source>This is a player connected to the server. The icon to the left of the player indicates whether or not they are talking:&lt;br /&gt;&lt;img src=&quot;skin:talking_on.png&quot; /&gt; Talking&lt;br /&gt;&lt;img src=&quot;skin:talking_off.png&quot; /&gt; Not talking</source>
        <translation>サーバに接続しているプレイヤーです。プレイヤーの左のアイコンはその人が話しているかどうかを示しています:&lt;br /&gt;&lt;img src=&quot;skin:talking_on.png&quot; /&gt; 話している&lt;br /&gt;&lt;img src=&quot;skin:talking_off.png&quot; /&gt; 話していない</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>This shows the flags the player has on the server, if any:&lt;br /&gt;&lt;img src=&quot;skin:authenticated.png&quot; /&gt;Authenticated user&lt;br /&gt;&lt;img src=&quot;skin:muted_self.png&quot; /&gt;Muted (by self)&lt;br /&gt;&lt;img src=&quot;skin:muted_server.png&quot; /&gt;Muted (by admin)&lt;br /&gt;&lt;img src=&quot;skin:deafened_self.png&quot; /&gt;Deafened (by self)&lt;br /&gt;&lt;img src=&quot;skin:deafened_server.png&quot; /&gt;Deafened (by admin)&lt;br /&gt;A player muted by himself is probably just away, talking on the phone or something like that.&lt;br /&gt;A player muted by an admin is probably also just away, and the noise the player is making was annoying enough that an admin muted him.</source>
        <translation>サーバ内のプレイヤーが持っているフラグを表示します:&lt;br /&gt;&lt;img src=&quot;skin:authenticated.png&quot; /&gt;認証されたユーザ&lt;br /&gt;&lt;img src=&quot;skin:muted_self.png&quot; /&gt;発言禁止(自分によって)&lt;br /&gt;&lt;img src=&quot;skin:muted_server.png&quot; /&gt;発言禁止(管理者によって)&lt;br /&gt;&lt;img src=&quot;skin:deafened_self.png&quot; /&gt;聴取禁止(自分によって)&lt;br /&gt;&lt;img src=&quot;skin:deafened_server.png&quot; /&gt;聴取禁止(管理者によって)&lt;br /&gt;自分で発言禁止にしているプレイヤーは、おそらくちょっと離席しているだけでしょう。&lt;br /&gt;管理者によって発言禁止にされているプレイヤーは同じく離席しているだけか、管理者が発言禁止するに値するほどその人の発する雑音がうるさかったのでしょう。</translation>
    </message>
    <message>
        <source>Mumble</source>
        <translation type="obsolete">Mumble</translation>
    </message>
    <message>
        <location line="+561"/>
        <source>Are you sure you want to drag this channel?</source>
        <translation>本当にこのチャンネルをドラッグしますか?</translation>
    </message>
</context>
<context>
    <name>PluginConfig</name>
    <message>
        <location filename="Plugins.ui"/>
        <source>Options</source>
        <translation>オプション</translation>
    </message>
    <message>
        <location/>
        <location filename="Plugins.cpp" line="+64"/>
        <source>Plugins</source>
        <translation>プラグイン</translation>
    </message>
    <message>
        <location/>
        <source>Link to Game and Transmit Position</source>
        <translation>ゲームと送信位置をリンクさせる</translation>
    </message>
    <message>
        <location/>
        <source>Enable plugins and transmit positional information</source>
        <translation>プラグインを有効にすると位置情報を送信します</translation>
    </message>
    <message>
        <location/>
        <source>This enables plugins for supported games to fetch your in-game position and transmit that with each voice packet. This enables other players to hear your voice in-game from the direction your character is in relation to their own.</source>
        <translation>この設定は、サポートされたゲームのプラグインが位置情報を取得して、音声パケットと共に送信することを可能にします。他のプレイヤーから見て、ゲーム内であなたのキャラクターがいる方向から声が聞こえるようにすることができます。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Reload plugins</source>
        <translation>プラグイン再読み込み(&amp;R)</translation>
    </message>
    <message>
        <location/>
        <source>Reloads all plugins</source>
        <translation>全てのプラグインを再読み込みします</translation>
    </message>
    <message>
        <location/>
        <source>This rescans and reloads plugins. Use this if you just added or changed a plugin to the plugins directory.</source>
        <translation>プラグインを再読み込みします。プラグインのディレクトリに追加したり、変更を行ったときに使用してください。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;About</source>
        <translation>Mumbleについて(&amp;A)</translation>
    </message>
    <message>
        <location/>
        <source>Information about plugin</source>
        <translation>プラグインについての情報</translation>
    </message>
    <message>
        <location/>
        <source>This shows a small information message about the plugin.</source>
        <translation>プラグインについての簡単な情報を表示します。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Configure</source>
        <translation>設定(&amp;C)</translation>
    </message>
    <message>
        <location/>
        <source>Show configuration page of plugin</source>
        <translation>プラグインの設定項目を表示</translation>
    </message>
    <message>
        <location/>
        <source>This shows the configuration page of the plugin, if any.</source>
        <translation>プラグインの設定ページがあれば表示します。</translation>
    </message>
    <message>
        <source>Mumble</source>
        <translation type="obsolete">Mumble</translation>
    </message>
    <message>
        <location filename="Plugins.cpp" line="+28"/>
        <source>Plugin has no configure function.</source>
        <translation>プラグインに設定機能がありません。</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>Plugin has no about function.</source>
        <translation>プラグインの設定項目がありません。</translation>
    </message>
    <message>
        <location filename="Plugins.ui"/>
        <source>Form</source>
        <translation>フォーム</translation>
    </message>
</context>
<context>
    <name>Plugins</name>
    <message>
        <source>Plugin %1 lost link.</source>
        <translation type="obsolete">プラグイン %1 はリンクを失いました。</translation>
    </message>
    <message>
        <location filename="Plugins.cpp" line="+127"/>
        <source>%1 lost link.</source>
        <translation>%1 はリンクを失いました。</translation>
    </message>
    <message>
        <source>Plugin %1 linked.</source>
        <translation type="obsolete">プラグイン %1 はリンクされました。</translation>
    </message>
    <message>
        <location line="+39"/>
        <source>%1 linked.</source>
        <translation>%1 はリンクされました。</translation>
    </message>
</context>
<context>
    <name>PortAudioSystem</name>
    <message>
        <location filename="PAAudio.cpp" line="+273"/>
        <source>Default Device</source>
        <translation>デフォルトデバイス</translation>
    </message>
</context>
<context>
    <name>PulseAudioConfig</name>
    <message>
        <source>PulseAudio</source>
        <translation type="obsolete">PulseAudio</translation>
    </message>
    <message>
        <source>%1ms</source>
        <translation type="obsolete">%1ms</translation>
    </message>
    <message>
        <source>Form</source>
        <translation type="obsolete">フォーム</translation>
    </message>
    <message>
        <source>Device selection</source>
        <translation type="obsolete">デバイス選択</translation>
    </message>
    <message>
        <source>Input (Microphone)</source>
        <translation type="obsolete">入力(マイク)</translation>
    </message>
    <message>
        <source>Device to use for microphone</source>
        <translation type="obsolete">マイクに使用するデバイス</translation>
    </message>
    <message>
        <source>This set which source mumble should use for the microphone. The &lt;i&gt;default&lt;/i&gt; device means to use the PulseAudio defaults.</source>
        <translation type="obsolete">どのソースをマイクに使用するか設定します。&lt;i&gt;デフォルト&lt;/i&gt; デバイスは PulseAudio のデフォルトです。 </translation>
    </message>
    <message>
        <source>Output</source>
        <translation type="obsolete">出力</translation>
    </message>
    <message>
        <source>Device to use for speakers/headphones</source>
        <translation type="obsolete">スピーカー/ヘッドホンに使用するデバイス</translation>
    </message>
    <message>
        <source>Enable echo cancellation</source>
        <translation type="obsolete">エコーキャンセル</translation>
    </message>
    <message>
        <source>This enables echo cancellation. Don&apos;t enable this unless you are listening on speakers; for headphones it will cause audio degradation.</source>
        <translation type="obsolete">エコーキャンセルを有効にします。スピーカーで聞いていない限りこれを有効にしないでください; ヘッドホンでは音質劣化の原因となります。</translation>
    </message>
    <message>
        <source>Use echo cancellation</source>
        <translation type="obsolete">エコーキャンセルを使う</translation>
    </message>
    <message>
        <source>Output Options</source>
        <translation type="obsolete">出力オプション</translation>
    </message>
    <message>
        <source>Output Delay</source>
        <translation type="obsolete">出力猶予</translation>
    </message>
    <message>
        <source>Amount of data to buffer for ALSA</source>
        <translation type="obsolete">ALSA用バッファデータの量</translation>
    </message>
    <message>
        <source>This sets the amount of data to prebuffer in the output buffer. Experiment with different values and set it to the lowest which doesn&apos;t cause rapid jitter in the sound.</source>
        <translation type="obsolete">出力バッファにおけるプリバッファの量を設定します。いろいろな値で試してみて、急に不安定にならない一番小さな値に設定してください。</translation>
    </message>
    <message>
        <source>TextLabel</source>
        <translation type="obsolete">テキストラベル</translation>
    </message>
</context>
<context>
    <name>PulseAudioSystem</name>
    <message>
        <location filename="PulseAudio.cpp" line="+545"/>
        <source>Default Input</source>
        <translation>デフォルト入力</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Default Output</source>
        <translation>デフォルト出力</translation>
    </message>
</context>
<context>
    <name>ShortcutActionWidget</name>
    <message>
        <location filename="GlobalShortcut.cpp" line="-143"/>
        <source>Unassigned</source>
        <translation>未割り当て</translation>
    </message>
</context>
<context>
    <name>ShortcutDelegate</name>
    <message>
        <location line="+58"/>
        <source>Unassigned</source>
        <translation>未割り当て</translation>
    </message>
</context>
<context>
    <name>ShortcutKeyWidget</name>
    <message>
        <location line="-122"/>
        <source>Press Shortcut</source>
        <translation>DirectInputKeyウィジェット</translation>
    </message>
</context>
<context>
    <name>TextMessage</name>
    <message>
        <location filename="TextMessage.ui"/>
        <source>Message to send</source>
        <translation>送信するメッセージ</translation>
    </message>
    <message>
        <location/>
        <source>Preview</source>
        <translation>プレビュー</translation>
    </message>
    <message>
        <source>Send raw message</source>
        <translation type="obsolete">生のメッセージを送信する</translation>
    </message>
    <message>
        <source>Disables html formating</source>
        <translation type="obsolete">htmlフォーマットを無効にする</translation>
    </message>
    <message>
        <location/>
        <source>Disables html formatting</source>
        <translation>HTML形式を無効にする</translation>
    </message>
    <message>
        <location/>
        <source>Disable text formatting</source>
        <translation>TEXT形式を無効にする</translation>
    </message>
    <message>
        <location/>
        <source>This field reflects how the message will look like in the chat.</source>
        <translation>このフィールドはどのようにメッセージガチャッ途上で表示されるかを反映します。</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Preview&lt;/b&gt;&lt;br /&gt;This field takes the raw string entered above and renders it like it will look like in the log. For example  &amp;lt;b&amp;gt;Test&amp;lt;/b&amp;gt; will display as: &lt;b&gt;Test&lt;b&gt;.</source>
        <translation>&lt;b&gt;プレビュー&lt;/b&gt;&lt;br /&gt;このフィールドは上で入力された生の文字列をとり、それをログ上で見れる形にレンダリングします。例えば  &amp;lt;b&amp;gt;Test&amp;lt;/b&amp;gt; は次のように表示します :&lt;b&gt;Test&lt;b&gt;。</translation>
    </message>
</context>
<context>
    <name>VersionCheck</name>
    <message>
        <source>Mumble</source>
        <translation type="obsolete">Mumble</translation>
    </message>
    <message>
        <source>Mumble failed to retrieve version information from the SourceForge server.</source>
        <translation type="obsolete">SourceForgeサーバからのバージョン情報の取得に失敗しました。</translation>
    </message>
    <message>
        <location filename="VersionCheck.cpp" line="+130"/>
        <source>Upgrade Mumble</source>
        <translation>Mumble のアップグレード</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>A new version of Mumble has been detected and automatically downloaded. It is recommended that you either upgrade to this version, or downgrade to the latest stable release. Do you want to launch the installer now?</source>
        <translation>Mumble の新しいバージョンが見つかり、自動的にダウンロードされました。このバージョンにアップグレードするか、最新の安定リリースにダウングレードすることが推奨されます。このインストーラを起動しますか?</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Failed to launch snapshot installer.</source>
        <translation>snapshot のインストーラの起動に失敗しました。</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Corrupt download of new version detected. Automatically removed.</source>
        <translation>新しいバージョンの破損したダウンロードが検知されました。自動的に削除されました。</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Downloading new snapshot from %1 to %2</source>
        <translation>%1 から %2 に新しい snapshot をダウンロード中です</translation>
    </message>
    <message>
        <location line="+18"/>
        <source>Failed to write new version to disk.</source>
        <oldsource>Failed to write new version to disc.</oldsource>
        <translation type="unfinished">ディスクに新しいバージョンを書きこむことに失敗しました。</translation>
    </message>
    <message>
        <location line="+17"/>
        <source>Mumble failed to retrieve version information from the central server.</source>
        <translation>Mumble は中央サーバからのバージョン情報の取得に失敗しました。</translation>
    </message>
</context>
<context>
    <name>ViewCert</name>
    <message>
        <location filename="ViewCert.cpp" line="+41"/>
        <source>Certificate Chain Details</source>
        <translation>証明書階層の詳細</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Certificate chain</source>
        <translation>証明書の階層</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Certificate details</source>
        <translation>証明の内容</translation>
    </message>
    <message>
        <location line="-3"/>
        <source>%1 %2</source>
        <translation>%1 %2</translation>
    </message>
    <message>
        <location line="+31"/>
        <location line="+28"/>
        <source>Common Name: %1</source>
        <translation>一般名称(CN): %1</translation>
    </message>
    <message>
        <location line="-27"/>
        <location line="+28"/>
        <source>Organization: %1</source>
        <translation>組織(O): %1</translation>
    </message>
    <message>
        <location line="-27"/>
        <source>Subunit: %1</source>
        <translation>部門(OU): %1</translation>
    </message>
    <message>
        <location line="+1"/>
        <location line="+28"/>
        <source>Country: %1</source>
        <translation>国(C): %1</translation>
    </message>
    <message>
        <location line="-27"/>
        <location line="+28"/>
        <source>Locality: %1</source>
        <translation>地域(L): %1</translation>
    </message>
    <message>
        <location line="-27"/>
        <location line="+28"/>
        <source>State: %1</source>
        <translation>地方(ST): %1</translation>
    </message>
    <message>
        <location line="-27"/>
        <source>Valid from: %1</source>
        <translation>発行者: %1</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Valid to: %1</source>
        <translation>発行対象: %1</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Serial: %1</source>
        <translation>シリアル番号: %1</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Public Key: %1 bits %2</source>
        <translation>公開鍵: %1 ビット %2</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>RSA</source>
        <translation>RSA</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>DSA</source>
        <translation>DSA</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Digest (MD5): %1</source>
        <translation>ダイジェスト(MD5): %1</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Email: %1</source>
        <translation></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>DNS: %1</source>
        <translation></translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Issued by:</source>
        <translation>発行者:</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Unit Name: %1</source>
        <translation>部門名: %1</translation>
    </message>
</context>
<context>
    <name>WASAPIConfig</name>
    <message>
        <source>Form</source>
        <translation type="obsolete">フォーム</translation>
    </message>
    <message>
        <source>Device selection</source>
        <translation type="obsolete">デバイス選択</translation>
    </message>
    <message>
        <source>Input (Microphone)</source>
        <translation type="obsolete">入力(マイク)</translation>
    </message>
    <message>
        <source>This set which source mumble should use for the microphone. The &lt;i&gt;default&lt;/i&gt; device means to use the PulseAudio defaults.</source>
        <translation type="obsolete">どのソースをマイクに使用するか設定します。&lt;i&gt;デフォルト&lt;/i&gt; デバイスは PulseAudio のデフォルトです。 </translation>
    </message>
    <message>
        <source>Output</source>
        <translation type="obsolete">出力</translation>
    </message>
    <message>
        <source>Device to use for speakers/headphones</source>
        <translation type="obsolete">スピーカー/ヘッドホンに使用するデバイス</translation>
    </message>
    <message>
        <source>Enable echo cancellation</source>
        <translation type="obsolete">エコーキャンセル</translation>
    </message>
    <message>
        <source>This enables echo cancellation. Don&apos;t enable this unless you are listening on speakers; for headphones it will cause audio degradation.</source>
        <translation type="obsolete">エコーキャンセルを有効にします。スピーカーを使っている場合のみこれを有効にしてください; ヘッドホンでは音質劣化の原因となります。</translation>
    </message>
    <message>
        <source>Use echo cancellation</source>
        <translation type="obsolete">エコーキャンセルを使う</translation>
    </message>
</context>
<context>
    <name>WASAPISystem</name>
    <message>
        <location filename="WASAPI.cpp" line="+180"/>
        <source>Default Device</source>
        <translation>デフォルトデバイス</translation>
    </message>
</context>
<context>
    <name>XInputKeyWidget</name>
    <message>
        <source>Press Shortcut</source>
        <translation type="obsolete">ショートカット押下</translation>
    </message>
</context>
</TS>
