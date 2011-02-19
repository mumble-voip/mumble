<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.0" language="ja_JP">
<context>
    <name>ACLEditor</name>
    <message>
        <source>Mumble - Edit ACL for %1</source>
        <translation type="obsolete">Mumble - %1のACLを編集</translation>
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
        <source>This controls which group of users this entry applies to.&lt;br /&gt;Note that the group is evaluated in the context of the channel the entry is used in. For example, the default ACL on the Root channel gives &lt;i&gt;Write&lt;/i&gt; permission to the &lt;i&gt;admin&lt;/i&gt; group. This entry, if inherited by a channel, will give a user write privileges if he belongs to the &lt;i&gt;admin&lt;/i&gt; group in that channel, even if he doesn&apos;t belong to the &lt;i&gt;admin&lt;/i&gt; group in the channel where the ACL originated.&lt;br /&gt;If a group name starts with a &apos;!&apos;, its membership is negated, and if it starts with a &apos;~&apos;, it is evaluated in the channel the ACL was defined in, rather than the channel the ACL is active in.&lt;br /&gt;If a group name starts with a  &apos;#&apos;, it is interpreted as an access token. Users must have entered whatever follows the &apos;#&apos; in their list of access tokens to match. This can be used for very simple password access to channels for non-authenticated users.&lt;br /&gt;If a group name starts with a  &apos;$&apos;, it will only match users whose certificate hash matches what follows the &apos;$&apos;.&lt;br /&gt;A few special predefined groups are:&lt;br /&gt;&lt;b&gt;all&lt;/b&gt; - Everyone will match.&lt;br /&gt;&lt;b&gt;auth&lt;/b&gt; - All authenticated users will match.&lt;br /&gt;&lt;b&gt;sub,a,b,c&lt;/b&gt; - User currently in a sub-channel minimum &lt;i&gt;a&lt;/i&gt; common parents, and between &lt;i&gt;b&lt;/i&gt; and &lt;i&gt;c&lt;/i&gt; channels down the chain. See the website for more extensive documentation on this one.&lt;br /&gt;&lt;b&gt;in&lt;/b&gt; - Users currently in the channel will match (convenience for &apos;&lt;i&gt;sub,0,0,0&lt;/i&gt;&apos;).&lt;br /&gt;&lt;b&gt;out&lt;/b&gt; - Users outside the channel will match (convenience for &apos;&lt;i&gt;!sub,0,0,0&lt;/i&gt;&apos;).&lt;br /&gt;Note that an entry applies to either a user or a group, not both.</source>
        <translation type="obsolete">この項目がどのユーザのグループに適用されるかを制御します。グループはそのエントリが使用されるチャンネルのコンテキストそして評価されます。たとえば、Root チャンネル上のデフォルトのACL は &lt;i&gt;書き込み&lt;/i&gt;権限を&lt;i&gt;admin&lt;/i&gt;グループに与えています。このエントリは、もしチャンネルで継承済みでなければ、たとえそのACLに由来するチャンネルの&lt;i&gt;admin&lt;/i&gt;グループに属していなくても&lt;i&gt;admin&lt;/i&gt;グループがそのチャンネルに所属するならユーザに書き込み権限を与えます。グループの名前が ! で始まっていれば、そのメンバは否定されます。そして ~ で始まるなら　そのチャンネルで有効なACLよりもそのチャンネルのACLが定義された方が優先されます。グループの名前が # で始まる場合は、それはアクセストークンとして解釈されます。ユーザはアクセストークンのリストの中に # の後に続く文字列を持っている必要があります。これはとても非認証ユーザにたいして非常に単純なシンプルなパスワードアクセスの方法として使えます。グループの名前が $ で始まる場合、それは $ に続く文字列がユーザの証明書のハッシュにマッチする場合です。特別な定義済みグループは次のとおりです。&lt;br/&gt;&lt;b&gt;all&lt;/b&gt; - すべてにマッチ。&lt;br/&gt;&lt;b&gt;auth&lt;/b&gt; - 認証済みのすべてのユーザにマッチ。&lt;br /&gt;&lt;b&gt;sub,a,b,c&lt;/b&gt; - サブチャンネル &lt;i&gt;a&lt;/i&gt; &lt;i&gt;b&lt;/i&gt;&lt;i&gt;c&lt;/i&gt;最小で&lt;i&gt;a&lt;/i&gt;個の共通の両親を持ち、チェーンの下側へ&lt;i&gt;b&lt;/i&gt;個から&lt;i&gt;c個&lt;/i&gt;の間のサブチャンネルのユーザにマッチします。この項目についてのより多くのドキュメントを見るには公式サイトを確認してください。&lt;b&gt;in&lt;/b&gt; - 現在のチャンネルにいるすべてのユーザにマッチ。（これは &lt;i&gt;sub,0,0,0&lt;/i&gt;の簡易的な表記です。）&lt;b&gt;out&lt;/b&gt; - 現在のチャンネルの外にいるすべてのユーザにマッチ。（これは &lt;i&gt;！sub,0,0,0&lt;/i&gt;の簡易的な表記です。）
エントリはユーザかグループのどちらかに適用され、両方には適用されません。</translation>
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
        <translation type="unfinished">追加(&amp;A)</translation>
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
        <location filename="ACLEditor.cpp" line="+101"/>
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
        <source>Add to Remove</source>
        <translation type="obsolete">削除リストに追加</translation>
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
        <translation type="obsolete">このボタンは現在の グループとACLを 適用しサーバに送信します。もし誤ってあなた自身の書き込み権限を削除してしまうとサーバに追加されてしまいますのでご注意ください。</translation>
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
        <translation type="obsolete">このボタンは全ての変更をキャンセルし、サーバのACLやグループを変更せずにダイアログを閉じます。</translation>
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
        <source>Properties</source>
        <translation type="obsolete">プロパティ</translation>
    </message>
    <message>
        <location/>
        <source>Name</source>
        <translation>チャンネル名</translation>
    </message>
    <message>
        <location/>
        <source>Enter the channel name here.</source>
        <translation>チャンネル名をここに入力してください。</translation>
    </message>
    <message>
        <source>&lt;b&gt;Name&lt;/b&gt;&lt;br&gt;Enter the channel name in this field. The name has to comply with the restriction imposed by the server you are connected to.</source>
        <translation type="obsolete">&lt;b&gt;チャンネル名&lt;/b&gt;&lt;br&gt;このフィールドにチャンネル名を入力してください。チャンネル名は接続しているサーバの制限に従う必要があります。</translation>
    </message>
    <message>
        <location/>
        <source>Description</source>
        <translation>説明</translation>
    </message>
    <message>
        <location/>
        <source>Password</source>
        <translation>パスワード</translation>
    </message>
    <message>
        <location/>
        <source>Enter the channel password here.</source>
        <translation>チャンネルに参加するためのパスワードをここに入力してください。</translation>
    </message>
    <message>
        <source>&lt;b&gt;Password&lt;/b&gt;&lt;br&gt;This field allows you to easily set and change the password of a channel. In the background it uses Mumble&apos;s access tokens feature. To allow more fine grained and powerful access control directly use ACLs and groups instead (&lt;i&gt;Advanced configuration&lt;/i&gt; has to be checked to be able to see these settings).</source>
        <translation type="obsolete">&lt;b&gt;パスワード&lt;/b&gt;&lt;br&gt;このフィールドで簡単にチャンネルのパスワードを設定・変更できます。 背景にはMumble のアクセストークンの機能の使用があります。より細やかで協力なアクセス制御をするには、ACLとおグループを直接に使用してください。（&lt;i&gt;高度な設定&lt;/i&gt;はこれらの設定を見るためにチェックする必要があります）。</translation>
    </message>
    <message>
        <location/>
        <source>Check to create a temporary channel.</source>
        <translation>一時チャンネルを作る場合はチェックしてください。</translation>
    </message>
    <message>
        <source>&lt;b&gt;Temporary&lt;/b&gt;&lt;br&gt;
When checked the channel created will be marked as temporary. This means when the last player leaves it the channel will be automatically deleted by the server.</source>
        <translation type="obsolete">&lt;b&gt;一時チャンネルとして設定する&lt;/b&gt;&lt;br&gt;
チェックしてチャンネルを作ると一時としてマークされます。最後に残っていたユーザがチャンネルから離れるとサーバによって自動的にそのチャンネルが削除されます。</translation>
    </message>
    <message>
        <location/>
        <source>Temporary</source>
        <translation>一時チャンネルとして設定する</translation>
    </message>
    <message>
        <location/>
        <source>Channel positioning facility value</source>
        <translation>チャンネルの位置を指定するための値</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Position&lt;/b&gt;&lt;br/&gt;
This value enables you to change the way Mumble arranges the channels in the tree. A channel with a higher &lt;i&gt;Position&lt;/i&gt; value will always be placed below one with a lower value and the other way around. If the &lt;i&gt;Position&lt;/i&gt; value of two channels is equal they will get sorted alphabetically by their name.</source>
        <oldsource>&lt;b&gt;Position&lt;/b&gt;&lt;br/&gt;
This value enables you to change the way mumble arranges the channels in the tree. A channel with a higher &lt;i&gt;Position&lt;/i&gt; value will always be placed below one with a lower value and the other way around. If the &lt;i&gt;Position&lt;/i&gt; value of two channels is equal they will get sorted alphabetically by their name.</oldsource>
        <translation>&lt;b&gt;ポジション&lt;/b&gt;&lt;br /&gt;
この値を設定することで、このチャンネルがチャンネルツリー上で並ぶ順番を変更することができます。より高い&lt;i&gt;ポジション&lt;/i&gt;値を持つチャンネルは常により低い値を持つチャンネルよりも下に表示されます。また、２つのチャンネルの&lt;i&gt;ポジション&lt;/i&gt;値が等しいなら、それらのチャンネル名について辞書順で並べ替えられます。</translation>
    </message>
    <message>
        <location/>
        <source>Position</source>
        <translation>ポジション</translation>
    </message>
    <message>
        <source>&lt;b&gt;Group&lt;/b&gt;&lt;br&gt;
These are all the groups currently defined for the channel. To create a new group, just type in the name and press enter.</source>
        <oldsource>&lt;b&gt;Group&lt;/b&gt;&lt;br&gt;
This is all the groups currently defined for the channel. To create a new group, just type in the name and press enter.</oldsource>
        <translation type="obsolete">&lt;b&gt;グループ&lt;/b&gt;&lt;br /&gt;
このチャンネルで現在定義されている全てのグループです。新しいグループを作るには、グループ名を入力してEnterキーを押してください。</translation>
    </message>
    <message>
        <location/>
        <source>Add new group</source>
        <translation>新しいグループを追加</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Add&lt;/b&gt;&lt;br/&gt;
Add a new group.</source>
        <translation>&lt;b&gt;追加&lt;/b&gt;&lt;br /&gt;
新しいグループを追加する。</translation>
    </message>
    <message>
        <source>&lt;b&gt;Remove&lt;/b&gt;&lt;br&gt;This removes the currently selected group. If the group was inherited, it will not be removed from the list, but all local information about the group will be cleared.</source>
        <translation type="obsolete">&lt;b&gt;削除&lt;/b&gt;&lt;br /&gt;現在選択されているグループを削除します。もし、グループが継承されているなら、それはリストから削除されません。しかし、グループについての全ての局所情報はクリアされます。</translation>
    </message>
    <message>
        <source>&lt;b&gt;Inherit&lt;/b&gt;&lt;br&gt;This inherits all the members in the group from the parent, if the group is marked as &lt;i&gt;Inheritable&lt;/i&gt; in the parent channel.</source>
        <translation type="obsolete">&lt;b&gt;継承&lt;/b&gt;&lt;br /&gt;そのグループが親チャンネルで&lt;i&gt;継承可能&lt;/i&gt;と設定されているなら、そのグループの全てのメンバを継承します。</translation>
    </message>
    <message>
        <source>&lt;b&gt;Inheritable&lt;/b&gt;&lt;br&gt;This makes this group inheritable to sub-channels. If the group is non-inheritable, sub-channels are still free to create a new group with the same name.</source>
        <translation type="obsolete">&lt;b&gt;継承可能&lt;/b&gt;&lt;br /&gt;このグループがサブチャンネルに継承可能であることを意味します。グループが継承不可能であれば、サブチャンネルは同じ名前の新しいグループを自由に作ることが可能です。</translation>
    </message>
    <message>
        <source>&lt;b&gt;Inherited&lt;/b&gt;&lt;br&gt;This indicates that the group was inherited from the parent channel. You cannot edit this flag, it&apos;s just for information.</source>
        <translation type="obsolete">&lt;b&gt;継承済み&lt;/b&gt;これはグループが親チャンネルから継承されたことを意味します。このフラグはただの情報なので編集できません。</translation>
    </message>
    <message>
        <location/>
        <source>Inherited members</source>
        <translation>継承済みメンバ</translation>
    </message>
    <message>
        <location/>
        <source>Contains the list of members added to the group by this channel.</source>
        <translation>このチャンネルのグループに追加されたメンバのリスト。</translation>
    </message>
    <message>
        <source>&lt;b&gt;Members&lt;/b&gt;&lt;br&gt;
This list contains all members that were added to the group by the current channel. Be aware that this does not include members inherited by higher levels of the channel tree. These can be found in the &lt;i&gt;Inherited members&lt;/i&gt; list. To prevent this list to be inherited by lower level channels uncheck &lt;i&gt;Inheritable&lt;/i&gt; or manually add the members to the &lt;i&gt;Excluded members&lt;/i&gt; list.</source>
        <translation type="obsolete">&lt;b&gt;メンバ&lt;/b&gt;&lt;br /&gt;
このリストは現在のチャンネルのグループに追加された全てのメンバーを含みます。より高い階層のチャンネルツリーから継承されたメンバは含まないことに注意してください。それらのメンバは&lt;i&gt;継承済みメンバー&lt;/i&gt;のリストで確認できます。より低い階層のチャンネルでこのリストを継承させたくない場合は&lt;i&gt;継承可能&lt;/i&gt;のチェックを外すか、手動で&lt;i&gt;除外メンバ&lt;/i&gt;のリストにメンバを追加してください。</translation>
    </message>
    <message>
        <location/>
        <source>Contains a list of members whose group membership will not be inherited from the parent channel.</source>
        <translation>親チャンネルから継承されないグループのメンバーのリスト。</translation>
    </message>
    <message>
        <source>&lt;b&gt;Excluded members&lt;/b&gt;&lt;br&gt;
Contains a list of members whose group membership will not be inherited from the parent channel.</source>
        <translation type="obsolete">&lt;b&gt;除外メンバ&lt;/b&gt;&lt;br /&gt;
親チャンネルに継承されないグループのメンバーのリストを含む。</translation>
    </message>
    <message>
        <location/>
        <source>Contains the list of members inherited by other channels.</source>
        <translation>他のチャンネルから継承されたメンバのリストです。</translation>
    </message>
    <message>
        <source>&lt;b&gt;Inherited members&lt;/b&gt;&lt;br&gt;
Contains the list of members inherited by the current channel. Uncheck &lt;i&gt;Inherit&lt;/i&gt; to prevent inheritance from higher level channels.</source>
        <translation type="obsolete">&lt;b&gt;継承済みメンバ&lt;/b&gt;&lt;br /&gt;
現在のチャンネルで継承済みのメンバのリストです。より高い階層のチャンネルからの継承を防ぐには&lt;i&gt;継承&lt;/i&gt;のチェックを外してください。</translation>
    </message>
    <message>
        <location/>
        <source>Type in the name of a user you wish to add to the group and click Add.</source>
        <translation>グループに追加したいユーザ名をタイプして追加をクリックしてください。</translation>
    </message>
    <message>
        <location/>
        <source>Type in the name of a user you wish to remove from the group and click Add.</source>
        <translation>グループから削除したいユーザ名を入力して追加をクリックしてください。</translation>
    </message>
    <message>
        <location/>
        <source>Exclude</source>
        <translation>除外</translation>
    </message>
    <message>
        <location/>
        <source>Excluded members</source>
        <translation>除外メンバ</translation>
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
        <source>This controls which group of users this entry applies to.&lt;br /&gt;Note that the group is evaluated in the context of the channel the entry is used in. For example, the default ACL on the Root channel gives &lt;i&gt;Write&lt;/i&gt; permission to the &lt;i&gt;admin&lt;/i&gt; group. This entry, if inherited by a channel, will give a user write privileges if he belongs to the &lt;i&gt;admin&lt;/i&gt; group in that channel, even if he doesn&apos;t belong to the &lt;i&gt;admin&lt;/i&gt; group in the channel where the ACL originated.&lt;br /&gt;If a group name starts with a !, its membership is negated, and if it starts with a ~, it is evaluated in the channel the ACL was defined in, rather than the channel the ACL is active in. Order is important; &lt;i&gt;!~in&lt;/i&gt; is valid, but &lt;i&gt;~!in&lt;/i&gt; is not.&lt;br /&gt;A few special predefined groups are:&lt;br /&gt;&lt;b&gt;all&lt;/b&gt; - Everyone will match.&lt;br /&gt;&lt;b&gt;auth&lt;/b&gt; - All authenticated users will match.&lt;br /&gt;&lt;b&gt;sub,a,b,c&lt;/b&gt; - User currently in a sub-channel minimum &lt;i&gt;a&lt;/i&gt; common parents, and between &lt;i&gt;b&lt;/i&gt; and &lt;i&gt;c&lt;/i&gt; channels down the chain. See the website for more extensive documentation on this one.&lt;br /&gt;&lt;b&gt;in&lt;/b&gt; - Users currently in the channel will match (convenience for &lt;i&gt;sub,0,0,0&lt;/i&gt;).&lt;br /&gt;&lt;b&gt;out&lt;/b&gt; - Users outside the channel will match (convenience for &lt;i&gt;!sub,0,0,0&lt;/i&gt;).&lt;br /&gt;Note that an entry applies to either a user or a group, not both.</source>
        <translation type="obsolete">この項目がどのユーザグループに適用されるかを制御します。</translation>
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
        <location filename="ACLEditor.cpp" line="+21"/>
        <source>Allow %1</source>
        <translation>%1を許可する</translation>
    </message>
    <message>
        <location line="-8"/>
        <source>Deny %1</source>
        <translation>%1を禁止する</translation>
    </message>
    <message>
        <location line="-62"/>
        <source>Mumble - Add channel</source>
        <translation>Mumble - チャンネル追加</translation>
    </message>
    <message>
        <location line="+22"/>
        <location line="+165"/>
        <source>Failed: Invalid channel</source>
        <translation>失敗: 無効なチャンネル</translation>
    </message>
    <message>
        <location line="-153"/>
        <source>Mumble - Edit %1</source>
        <translation>Mumble - %1 の編集</translation>
    </message>
    <message>
        <location line="+37"/>
        <source>This grants the %1 privilege. If a privilege is both allowed and denied, it is denied.&lt;br /&gt;%2</source>
        <translation>%1に特権を付与します。特権の許可と禁止の両方が設定されている場合、禁止になります。&lt;br /&gt;%2</translation>
    </message>
    <message>
        <location line="-8"/>
        <source>This revokes the %1 privilege. If a privilege is both allowed and denied, it is denied.&lt;br /&gt;%2</source>
        <translation>%1から特権を剥奪します。特権の許可と禁止の両方が設定されている場合、禁止になります。&lt;br /&gt;%2</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui"/>
        <source>List of groups</source>
        <translation>グループリスト</translation>
    </message>
    <message>
        <source>This is all the groups currently defined for the channel. To create a new group, just type in the name and press enter.</source>
        <translation type="obsolete">チャンネルに定義されている全てのグループです。新たにグループを作成するには名前を入力してEnterを押してください。</translation>
    </message>
    <message>
        <location/>
        <source>Remove selected group</source>
        <translation>選択されたグループを削除</translation>
    </message>
    <message>
        <source>This removes the currently selected group. If the group was inherited, it will not be removed from the list, but all local information about the group will be cleared.</source>
        <translation type="obsolete">選択されたグループを削除します。もしグループが継承されていたらリストからは削除されませんが、グループに関するローカルな情報は消えてしまいます。</translation>
    </message>
    <message>
        <location/>
        <source>Inherit group members from parent</source>
        <translation>親からグループメンバーを継承する</translation>
    </message>
    <message>
        <source>This inherits all the members in the group from the parent, if the group is marked as &lt;i&gt;Inheritable&lt;/i&gt; in the parent channel.</source>
        <translation type="obsolete">親チャンネルでグループが継承可になっている場合、親グループから全てのメンバーを継承します。</translation>
    </message>
    <message>
        <location/>
        <source>Make group inheritable to sub-channels</source>
        <translation>サブチャンネルに対してグループを継承可能にする</translation>
    </message>
    <message>
        <source>This makes this group inheritable to sub-channels. If the group is non-inheritable, sub-channels are still free to create a new group with the same name.</source>
        <translation type="obsolete">サブチャンネルに対してグループを継承可能にします。もしグループが継承不可ならばサブチャンネルでは同じ名前の新しいグループを作成することが出来ます。</translation>
    </message>
    <message>
        <location/>
        <source>Group was inherited from parent channel</source>
        <translation>親チャンネルから継承したグループ</translation>
    </message>
    <message>
        <source>This indicates that the group was inherited from the parent channel. You cannot edit this flag, it&apos;s just for information.</source>
        <translation type="obsolete">グループが親チャンネルから継承されたことを示しています。このフラグは変更できません。これは単なる情報です。</translation>
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
        <source>Type in the name of a player you wish to add to the group and click Add.</source>
        <translation type="obsolete">追加したいプレイヤー名を入力して追加ボタンをクリックしてください。</translation>
    </message>
    <message>
        <source>Type in the name of a player you wish to remove from the group and click Add.</source>
        <translation type="obsolete">削除したいプレイヤー名を入力してAddボタンをクリックしてください。</translation>
    </message>
    <message>
        <source>This shows all the entries active on this channel. Entries inherited from parent channels will be show in italics.&lt;br /&gt;ACLs are evaluated top to bottom, meaning priority increases as you move down the list.</source>
        <translation type="obsolete">このチャンネルで有効な項目を全て表示します。親チャンネルから継承した項目はイタリック(斜字)体で表示されます。&lt;br /&gt;ACLは上から順に評価されます。つまり、リストの順位を下げる事は優先度が上がる事を意味します。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Properties</source>
        <translation>プロパティ(&amp;P)</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Name&lt;/b&gt;&lt;br /&gt;Enter the channel name in this field. The name has to comply with the restriction imposed by the server you are connected to.</source>
        <translation type="unfinished">&lt;b&gt;チャンネル名&lt;/b&gt;&lt;br /&gt;このフィールドにチャンネル名を入力してください。チャンネル名は接続しているサーバの制限に従う必要があります。</translation>
    </message>
    <message>
        <source>&lt;b&gt;Password&lt;/b&gt;&lt;br /&gt;This field allows you to easily set and change the password of a channel. In the background it uses Mumble&apos;s access tokens feature. To allow more fine grained and powerful access control directly, use ACLs and groups instead (&lt;i&gt;Advanced configuration&lt;/i&gt; has to be checked to be able to see these settings).</source>
        <translation type="obsolete">&lt;b&gt;パスワード&lt;/b&gt;&lt;br /&gt;このフィールドで簡単にチャンネルのパスワードを設定・変更できます。 背景にはMumble のアクセストークンの機能の使用があります。より細やかで協力なアクセス制御をするには、ACLとおグループを直接に使用してください。（&lt;i&gt;高度な設定&lt;/i&gt;はこれらの設定を見るためにチェックする必要があります）。</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Temporary&lt;/b&gt;&lt;br /&gt;
When checked the channel created will be marked as temporary. This means when the last player leaves it the channel will be automatically deleted by the server.</source>
        <translation type="unfinished">&lt;b&gt;一時チャンネルとして設定する&lt;/b&gt;&lt;br /&gt;
チェックしてチャンネルを作ると一時としてマークされます。最後に残っていたユーザがチャンネルから離れるとサーバによって自動的にそのチャンネルが削除されます。</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Group&lt;/b&gt;&lt;br /&gt;
These are all the groups currently defined for the channel. To create a new group, just type in the name and press enter.</source>
        <translation type="unfinished">&lt;b&gt;グループ&lt;/b&gt;&lt;br /&gt;
このチャンネルで現在定義されている全てのグループです。新しいグループを作るには、グループ名を入力してEnterキーを押してください。</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Remove&lt;/b&gt;&lt;br /&gt;This removes the currently selected group. If the group was inherited, it will not be removed from the list, but all local information about the group will be cleared.</source>
        <translation type="unfinished">&lt;b&gt;削除&lt;/b&gt;&lt;br /&gt;現在選択されているグループを削除します。もし、グループが継承されているなら、それはリストから削除されません。しかし、グループについての全ての局所情報はクリアされます。</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Inherit&lt;/b&gt;&lt;br /&gt;This inherits all the members in the group from the parent, if the group is marked as &lt;i&gt;Inheritable&lt;/i&gt; in the parent channel.</source>
        <translation type="unfinished">&lt;b&gt;継承&lt;/b&gt;&lt;br /&gt;そのグループが親チャンネルで&lt;i&gt;継承可能&lt;/i&gt;と設定されているなら、そのグループの全てのメンバを継承します。</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Inheritable&lt;/b&gt;&lt;br /&gt;This makes this group inheritable to sub-channels. If the group is non-inheritable, sub-channels are still free to create a new group with the same name.</source>
        <translation type="unfinished">&lt;b&gt;継承可能&lt;/b&gt;&lt;br /&gt;このグループがサブチャンネルに継承可能であることを意味します。グループが継承不可能であれば、サブチャンネルは同じ名前の新しいグループを自由に作ることが可能です。</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Inherited&lt;/b&gt;&lt;br /&gt;This indicates that the group was inherited from the parent channel. You cannot edit this flag, it&apos;s just for information.</source>
        <translation type="unfinished">&lt;b&gt;継承済み&lt;/b&gt;これはグループが親チャンネルから継承されたことを意味します。このフラグはただの情報なので編集できません。</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Members&lt;/b&gt;&lt;br /&gt;
This list contains all members that were added to the group by the current channel. Be aware that this does not include members inherited by higher levels of the channel tree. These can be found in the &lt;i&gt;Inherited members&lt;/i&gt; list. To prevent this list to be inherited by lower level channels uncheck &lt;i&gt;Inheritable&lt;/i&gt; or manually add the members to the &lt;i&gt;Excluded members&lt;/i&gt; list.</source>
        <translation type="unfinished">&lt;b&gt;メンバ&lt;/b&gt;&lt;br /&gt;
このリストは現在のチャンネルのグループに追加された全てのメンバーを含みます。より高い階層のチャンネルツリーから継承されたメンバは含まないことに注意してください。それらのメンバは&lt;i&gt;継承済みメンバー&lt;/i&gt;のリストで確認できます。より低い階層のチャンネルでこのリストを継承させたくない場合は&lt;i&gt;継承可能&lt;/i&gt;のチェックを外すか、手動で&lt;i&gt;除外メンバ&lt;/i&gt;のリストにメンバを追加してください。</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Excluded members&lt;/b&gt;&lt;br /&gt;
Contains a list of members whose group membership will not be inherited from the parent channel.</source>
        <translation type="unfinished">&lt;b&gt;除外メンバ&lt;/b&gt;&lt;br /&gt;
親チャンネルに継承されないグループのメンバーのリストを含む。</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Inherited members&lt;/b&gt;&lt;br /&gt;
Contains the list of members inherited by the current channel. Uncheck &lt;i&gt;Inherit&lt;/i&gt; to prevent inheritance from higher level channels.</source>
        <translation type="unfinished">&lt;b&gt;継承済みメンバ&lt;/b&gt;&lt;br /&gt;
現在のチャンネルで継承済みのメンバのリストです。より高い階層のチャンネルからの継承を防ぐには&lt;i&gt;継承&lt;/i&gt;のチェックを外してください。</translation>
    </message>
    <message>
        <location/>
        <source>This controls which group of users this entry applies to.&lt;br /&gt;Note that the group is evaluated in the context of the channel the entry is used in. For example, the default ACL on the Root channel gives &lt;i&gt;Write&lt;/i&gt; permission to the &lt;i&gt;admin&lt;/i&gt; group. This entry, if inherited by a channel, will give a user write privileges if he belongs to the &lt;i&gt;admin&lt;/i&gt; group in that channel, even if he doesn&apos;t belong to the &lt;i&gt;admin&lt;/i&gt; group in the channel where the ACL originated.&lt;br /&gt;If a group name starts with &apos;!&apos;, its membership is negated, and if it starts with &apos;~&apos;, it is evaluated in the channel the ACL was defined in, rather than the channel the ACL is active in.&lt;br /&gt;If a group name starts with &apos;#&apos;, it is interpreted as an access token. Users must have entered whatever follows the &apos;#&apos; in their list of access tokens to match. This can be used for very simple password access to channels for non-authenticated users.&lt;br /&gt;If a group name starts with &apos;$&apos;, it will only match users whose certificate hash matches what follows the &apos;$&apos;.&lt;br /&gt;A few special predefined groups are:&lt;br /&gt;&lt;b&gt;all&lt;/b&gt; - Everyone will match.&lt;br /&gt;&lt;b&gt;auth&lt;/b&gt; - All authenticated users will match.&lt;br /&gt;&lt;b&gt;sub,a,b,c&lt;/b&gt; - User currently in a sub-channel minimum &lt;i&gt;a&lt;/i&gt; common parents, and between &lt;i&gt;b&lt;/i&gt; and &lt;i&gt;c&lt;/i&gt; channels down the chain. See the website for more extensive documentation on this one.&lt;br /&gt;&lt;b&gt;in&lt;/b&gt; - Users currently in the channel will match (convenience for &apos;&lt;i&gt;sub,0,0,0&lt;/i&gt;&apos;).&lt;br /&gt;&lt;b&gt;out&lt;/b&gt; - Users outside the channel will match (convenience for &apos;&lt;i&gt;!sub,0,0,0&lt;/i&gt;&apos;).&lt;br /&gt;Note that an entry applies to either a user or a group, not both.</source>
        <translation type="unfinished">この項目がどのユーザのグループに適用されるかを制御します。グループはそのエントリが使用されるチャンネルのコンテキストそして評価されます。たとえば、Root チャンネル上のデフォルトのACL は &lt;i&gt;書き込み&lt;/i&gt;権限を&lt;i&gt;admin&lt;/i&gt;グループに与えています。このエントリは、もしチャンネルで継承済みでなければ、たとえそのACLに由来するチャンネルの&lt;i&gt;admin&lt;/i&gt;グループに属していなくても&lt;i&gt;admin&lt;/i&gt;グループがそのチャンネルに所属するならユーザに書き込み権限を与えます。グループの名前が ! で始まっていれば、そのメンバは否定されます。そして ~ で始まるなら　そのチャンネルで有効なACLよりもそのチャンネルのACLが定義された方が優先されます。グループの名前が # で始まる場合は、それはアクセストークンとして解釈されます。ユーザはアクセストークンのリストの中に # の後に続く文字列を持っている必要があります。これはとても非認証ユーザにたいして非常に単純なシンプルなパスワードアクセスの方法として使えます。グループの名前が $ で始まる場合、それは $ に続く文字列がユーザの証明書のハッシュにマッチする場合です。特別な定義済みグループは次のとおりです。&lt;br/&gt;&lt;b&gt;all&lt;/b&gt; - すべてにマッチ。&lt;br/&gt;&lt;b&gt;auth&lt;/b&gt; - 認証済みのすべてのユーザにマッチ。&lt;br /&gt;&lt;b&gt;sub,a,b,c&lt;/b&gt; - サブチャンネル &lt;i&gt;a&lt;/i&gt; &lt;i&gt;b&lt;/i&gt;&lt;i&gt;c&lt;/i&gt;最小で&lt;i&gt;a&lt;/i&gt;個の共通の両親を持ち、チェーンの下側へ&lt;i&gt;b&lt;/i&gt;個から&lt;i&gt;c個&lt;/i&gt;の間のサブチャンネルのユーザにマッチします。この項目についてのより多くのドキュメントを見るには公式サイトを確認してください。&lt;b&gt;in&lt;/b&gt; - 現在のチャンネルにいるすべてのユーザにマッチ。（これは &lt;i&gt;sub,0,0,0&lt;/i&gt;の簡易的な表記です。）&lt;b&gt;out&lt;/b&gt; - 現在のチャンネルの外にいるすべてのユーザにマッチ。（これは &lt;i&gt;！sub,0,0,0&lt;/i&gt;の簡易的な表記です。）
エントリはユーザかグループのどちらかに適用され、両方には適用されません。</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Password&lt;/b&gt;&lt;br /&gt;This field allows you to easily set and change the password of a channel. It uses Mumble&apos;s access tokens feature in the background. Use ACLs and groups if you need more fine grained and powerful access control.</source>
        <translation type="unfinished">&lt;b&gt;パスワード&lt;/b&gt;&lt;br&gt;このフィールドで簡単にチャンネルのパスワードを設定・変更できます。基礎としてMumbleのアクセストークンの機能を使っています。より細やかで協力なアクセス制御をするには、ACLとグループを使ってください。</translation>
    </message>
    <message>
        <location/>
        <source>This shows all the entries active on this channel. Entries inherited from parent channels will be shown in italics.&lt;br /&gt;ACLs are evaluated top to bottom, meaning priority increases as you move down the list.</source>
        <translation type="unfinished">このチャンネルで有効な項目を全て表示します。親チャンネルから継承した項目はイタリック(斜字)体で表示されます。&lt;br /&gt;ACLは上から順に評価されます。つまり、リストの順位を下げる事は優先度が上がる事を意味します。</translation>
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
        <source>Buffer size</source>
        <translation>バッファサイズ</translation>
    </message>
    <message>
        <location/>
        <source>This will configure the input channels for ASIO. Make sure you select at least one channel as microphone and speaker. &lt;i&gt;Microphone&lt;/i&gt; should be where your microphone is attached, and &lt;i&gt;Speaker&lt;/i&gt; should be a channel that samples &apos;&lt;i&gt;What you hear&lt;/i&gt;&apos;.&lt;br /&gt;For example, on the Audigy 2 ZS, a good selection for Microphone would be &apos;&lt;i&gt;Mic L&lt;/i&gt;&apos; while Speaker should be &apos;&lt;i&gt;Mix L&lt;/i&gt;&apos; and &apos;&lt;i&gt;Mix R&lt;/i&gt;&apos;.</source>
        <translation>ASIOのための入力チャンネルを設定します。少なくとも1つのチャンネルをマイクとスピーカーとして選んでください。&lt;i&gt;マイク&lt;/i&gt; はあなたのマイクが接続されているところで、&lt;i&gt;スピーカー&lt;/i&gt;は&quot;聞くもの&quot;を試してみるチャンネルです。Sound Blaster Audigy 2 ZS の例ではスピーカーが&quot;Mix L&quot; and &quot;Mix R&quot;の時、マイクを&quot;Mic L&quot;にするのが良い選択のひとつです。</translation>
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
        <oldsource>%1 -&gt; %2 samples buffer, with %3 sample resolution (%4 preferred) at %5Hz</oldsource>
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
        <translation>ASIOドライバのインスタンス化に失敗</translation>
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
        <location line="+25"/>
        <source>&lt;h3&gt;About Speex&lt;/h3&gt;&lt;p&gt;&lt;tt&gt;&lt;a href=&quot;%1&quot;&gt;%1&lt;/a&gt;&lt;/tt&gt;&lt;/p&gt;&lt;p&gt;This program uses SpeexDSP.&lt;/p&gt;&lt;p&gt;Speex is used for echo cancellation, noise&lt;br /&gt;filtering and voice activity detection.&lt;/p&gt;</source>
        <translation></translation>
    </message>
    <message>
        <source>&lt;h3&gt;About Speex&lt;/h3&gt;&lt;p&gt;&lt;tt&gt;http://www.speex.org/&lt;/tt&gt;&lt;/p&gt;&lt;p&gt;This program uses Speex version %1&lt;/p&gt;&lt;p&gt;Speex is used for echo cancellation, noise&lt;br /&gt;filtering, voice activity detection and speech&lt;br /&gt;compression.&lt;/p&gt;</source>
        <translation type="obsolete">&lt;h3&gt;Speexについて&lt;/h3&gt;&lt;p&gt;&lt;tt&gt;http://www.speex.org/&lt;/tt&gt;&lt;/p&gt;&lt;p&gt;This program uses Speex version %1&lt;/p&gt;&lt;p&gt;Speex is used for echo cancellation, noise&lt;br /&gt;filtering, voice activity detection and speech&lt;br /&gt;compression.&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <location line="-12"/>
        <source>About Speex</source>
        <translation>Speexについて</translation>
    </message>
    <message>
        <source>&lt;h3&gt;About Speex&lt;/h3&gt;&lt;p&gt;&lt;tt&gt;&lt;a href=&quot;%2&quot;&gt;%2&lt;/a&gt;&lt;/tt&gt;&lt;/p&gt;&lt;p&gt;This program uses Speex version %1&lt;/p&gt;&lt;p&gt;Speex is used for echo cancellation, noise&lt;br /&gt;filtering, voice activity detection and speech&lt;br /&gt;compression.&lt;/p&gt;</source>
        <translation type="obsolete">&lt;h3&gt;Speexについて&lt;/h3&gt;&lt;p&gt;&lt;tt&gt;&lt;a href=&quot;%2&quot;&gt;%2&lt;/a&gt;&lt;/tt&gt;&lt;/p&gt;&lt;p&gt;This program uses Speex version %1&lt;/p&gt;&lt;p&gt;Speex is used for echo cancellation, noise&lt;br /&gt;filtering, voice activity detection and speech&lt;br /&gt;compression.&lt;/p&gt;</translation>
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
        <translation type="obsolete">&lt;b&gt;TCP互換モードを有効にします。&lt;/b&gt;.&lt;br /&gt;この設定をするとMumbleがサーバとの通信にTCPのみを使用するようになります。この設定は通信のオーバーヘッドを増大させ、通信時に音声が途切れる原因になるので、デフォルトの設定(音声:UDP 操作:TCP)が使用できない場合だけ使用するのが良いでしょう。</translation>
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
        <translation type="obsolete">&lt;b&gt;切断時に再接続を試みます。&lt;/b&gt;&lt;br /&gt;この設定をすると、サーバへの接続が失敗した際、10秒後に再接続を行います。</translation>
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
        <translation type="obsolete">サーバ</translation>
    </message>
    <message>
        <source>Desired loopback mode</source>
        <translation type="obsolete">使用したいループバックモード</translation>
    </message>
    <message>
        <source>&lt;b&gt;This enables one of the loopback testmodes.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;None&lt;/i&gt; - Loopback disabled&lt;br /&gt;&lt;i&gt;Local&lt;/i&gt; - Emulate a local server.&lt;br /&gt;&lt;i&gt;Server&lt;/i&gt; - Request loopback from server.&lt;br /&gt;Please note than when loopback is enabled, no other players will hear your voice. This setting is not saved on application exit.</source>
        <translation type="obsolete">&lt;b&gt;ループバックテストモードの一つが有効になります。&lt;/b&gt;&lt;br /&gt;&lt;i&gt;なし&lt;/i&gt; - ループバックしない&lt;br /&gt;&lt;i&gt;ローカル&lt;/i&gt; - ローカルなサーバをエミュレートします。&lt;br /&gt;&lt;i&gt;サーバ&lt;/i&gt; - サーバにループバックを要求します。&lt;br /&gt;ループバックが有効になっている間、他のプレイヤーはあなたの声を聞くことが出来ないのでご注意ください。この設定はアプリケーション終了時に保存されません。</translation>
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
        <source>Cancel Echo</source>
        <translation type="obsolete">エコーキャンセル</translation>
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
        <translation>&lt;b&gt;発言を送信するタイミングを設定します。&lt;/b&gt;&lt;br /&gt;&lt;i&gt;常に有効&lt;/i&gt; - 常時音声入力を受け付けます&lt;br /&gt;&lt;i&gt;声で有効化&lt;/i&gt; - はっきりとしゃべっている時に受け付けます。&lt;br /&gt;&lt;i&gt;プッシュ・トゥ・トーク&lt;/i&gt; - &lt;i&gt;ショートカットキー&lt;/i&gt;で設定したキーを押している間音声入力を受け付けます。</translation>
    </message>
    <message>
        <location/>
        <source>DoublePush Time</source>
        <translation>二重押し時間</translation>
    </message>
    <message>
        <source>TextLabel</source>
        <translation type="obsolete">テキストラベル</translation>
    </message>
    <message>
        <source>Audible audio cue when push-to-talk pressed and released</source>
        <translation type="obsolete">&quot;キー押下で有効化&quot;設定の時、ショートカットキーが押される/離されるといったタイミングで音による通知を行います。</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This enables the audio cues for push to talk.&lt;/b&gt;&lt;br /&gt;Setting this will give you a short audio beep when push to talk is pressed and released.</source>
        <translation>&lt;b&gt;プッシュ・トゥ・トークの際に通知を行います&lt;/b&gt;&lt;br /&gt;この設定を有効にすると、ショートカットキーが押されたり離されたりするタイミング短いビープ音を鳴らします。</translation>
    </message>
    <message>
        <location/>
        <source>PTT Audio cue</source>
        <translation>プッシュ・トゥ・トークを音で通知</translation>
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
        <source>Use Amplitude based speech detection.</source>
        <translation type="obsolete">信号の強さに基づく発言認識を使用します。</translation>
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
        <source>&lt;b&gt;This selects how long after a perceived stop in speech transmission should continue.&lt;/b&gt;&lt;br /&gt;Set this higher if your voice breaks up when you speak (seen by a rapidly blinking voice icon next to your name).&lt;br /&gt;Only makes sense when used with Voice Activity transmission.</source>
        <translation type="obsolete">&lt;b&gt;音声が無くなった後、送信がどれくらい続くかを選択します。&lt;/b&gt;&lt;br /&gt;あなたが話している時(名前の後ろで点滅しているアイコンで確認できます)に音声が途切れてしまうようならば、この設定をより高くしてください。&lt;br /&gt;この設定は&quot;声で有効化&quot;の場合のみ効果があります。</translation>
    </message>
    <message>
        <location/>
        <source>Silence Below</source>
        <translation>非発言のしきい値</translation>
    </message>
    <message>
        <source>Signal values below this counts as silence</source>
        <translation type="obsolete">発言していないと判定される信号値のしきい値</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the trigger values for voice detection.&lt;/b&gt;&lt;br /&gt;Use this together with the Audio Statistics window to manually tune the trigger values for detecting speech. Input values below &quot;Silence Below&quot; always count as silence. Values above &quot;Speech Above&quot; always count as voice. Values in between will count as voice if you&apos;re already talking, but will not trigger a new detection.</source>
        <translation>&lt;b&gt;声を検出するための基準値を設定します。&lt;/b&gt;手動で調整するためには音声統計ウインドウを一緒にご利用ください。&quot;非発言しきい値&quot;以下の値は常に発言していない状態と見なされ、&quot;発言しきい値&quot;より上の値は発言と見なされます。これらの間の値は既に話し中であれば発言と判断されますが、新たな発言であると判断する材料にはなりません。</translation>
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
        <translation>１送信パケットあたりの音声フレームの長さ</translation>
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
        <source>Cancel echo from speakers</source>
        <translation>スピーカからのエコーをキャンセルします</translation>
    </message>
    <message>
        <location/>
        <source>Enabling this will cancel the echo from your speakers. Mixed has low CPU impact, but only works well if your speakers are equally loud and equidistant from the microphone. Multichannel echo cancellation provides much better echo cancellation, but at a higher CPU cost.</source>
        <translation>あなたのスピーカからのエコーをキャンセルすることができます。ミックスはCPU負荷が低いが、スピーカから一定の大きさの音を出していて、マイクから一定の距離を保っている場合にしかうまく働きません。マルチチャンネル エコーキャンセルはよりよいエコーキャンセルを提供するが、CPU負荷がミックスより高いです。</translation>
    </message>
    <message>
        <location/>
        <source>Disabled</source>
        <translation>無効</translation>
    </message>
    <message>
        <location/>
        <source>Mixed</source>
        <translation>ミックス</translation>
    </message>
    <message>
        <location/>
        <source>Multichannel</source>
        <translation>マルチチャンネル</translation>
    </message>
    <message>
        <location/>
        <source>Echo</source>
        <translation>エコー</translation>
    </message>
    <message>
        <location/>
        <source>If you press the PTT key twice in this time it will get locked.</source>
        <translation>プッシュ・トゥ・トーク キーを2回押すとロックされます。</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;DoublePush Time&lt;/b&gt;&lt;br /&gt;If you press the push-to-talk key twice during the configured interval of time it will be locked. Mumble will keep transmitting until you hit the key once more to unlock PTT again.</source>
        <translation>&lt;b&gt;2度押し時間&lt;/b&gt;&lt;br /&gt;もしあなたがプッシュ・トゥ・トークキーを2回、ここで設定した感覚の間に押したなら、キーはロックされます。Mumble は あなたがプッシュ・トゥ・トークをアンロックするために一回以上キーを押すまで転送状態のままです。</translation>
    </message>
    <message>
        <location/>
        <source>Gets played when the PTT button is pressed</source>
        <translation>プッシュ・トゥ・トーク ボタンが押されたときに再生させる</translation>
    </message>
    <message>
        <location/>
        <source>Gets played when the PTT button is released</source>
        <translation>プッシュ・トゥ・トーク ボタンが放されたときに再生させる</translation>
    </message>
    <message>
        <location/>
        <source>Reset audio cue to default</source>
        <translation>オーディオキューをデフォルトにリセット</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Reset&lt;/b&gt;&lt;br/&gt;Reset the paths for the files to their default.</source>
        <translation>&lt;b&gt;リセット&lt;/b&gt;&lt;br /&gt;デフォルトにファイルへのパスをリセットします。</translation>
    </message>
    <message>
        <source>Reset</source>
        <translation type="obsolete">リセット</translation>
    </message>
    <message>
        <location/>
        <source>Browse for on audio file</source>
        <translation>オン時の音声ファイルを参照</translation>
    </message>
    <message>
        <source>Browse</source>
        <translation type="obsolete">参照</translation>
    </message>
    <message>
        <location/>
        <source>Browse for off audio file</source>
        <translation>オフ時の音声ファイルを参照</translation>
    </message>
    <message>
        <location/>
        <source>Off</source>
        <translation>オフ</translation>
    </message>
    <message>
        <location/>
        <source>On</source>
        <translation>オン</translation>
    </message>
    <message>
        <location/>
        <source>Preview the audio cues</source>
        <translation>オーディオキューをプレビュー</translation>
    </message>
    <message>
        <source>&lt;b&gt;Preview&lt;/b&gt;&lt;br/&gt;Plays the current &lt;i&gt;on&lt;/i&gt; soundfile followed by the current &lt;i&gt;off&lt;/i&gt; soundfile.</source>
        <translation type="obsolete">&lt;b&gt;プレビュー&lt;/b&gt;&lt;br /&gt;現在のオンのときのサウンドファイルを再生しそのあとにオフのときのサウンドファイルを再生する。</translation>
    </message>
    <message>
        <source>Preview</source>
        <translation type="obsolete">プレビュー</translation>
    </message>
    <message>
        <location/>
        <source>Audible audio cue when push-to-talk is activated or deactivated</source>
        <translation>&quot;プッシュ・トゥ・トーク&quot;の設定の時、ショートカットキーが押される/離されるといったタイミングで音による通知を行います</translation>
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
        <source>&lt;b&gt;This sets the amount of noise suppression to apply.&lt;/b&gt;&lt;br /&gt;The higher this value, the more aggressively stationary noise will be suppressed.</source>
        <translation>&lt;b&gt;この設定は多くのノイズを抑制します。&lt;/b&gt;&lt;br /&gt;より大きい値を設定すると、より多くのノイズが抑制されます。</translation>
    </message>
    <message>
        <source>Idle AutoMute</source>
        <translation type="obsolete">自動ミュートまでの待ち時間</translation>
    </message>
    <message>
        <source>How long to remain idle before auto-mute.</source>
        <translation type="obsolete">自動的にミュートになるまでに必要なアイドル時間です。</translation>
    </message>
    <message>
        <source>This sets an idle timer. If the timer expires without any audio being sent to the server, you are muted.</source>
        <translation type="obsolete">アイドリング時間を設定します。指定時間のあいだ、サーバへ何の音声も送信されなかった場合、ミュートになります。</translation>
    </message>
    <message>
        <location/>
        <source>Signal values below this count as silence</source>
        <translation>発言していないと判定される信号値のしきい値</translation>
    </message>
    <message>
        <location/>
        <source>Maximum bandwidth used for sending audio</source>
        <translation>音声を送信するために使う最大帯域</translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets the amount of noise suppression to apply.&lt;/b&gt;&lt;br /&gt;The higher this value, the more aggressively stationary noise will be supressed.</source>
        <translation type="obsolete">&lt;b&gt;ノイズ抑制の量を設定します。&lt;/b&gt;&lt;br /&gt;値を大きくすると、より積極的に定常ノイズを抑制しようとします。</translation>
    </message>
    <message>
        <location filename="AudioInput.cpp" line="+520"/>
        <source>Server maximum network bandwidth is only %1 kbit/s. Audio quality auto-adjusted to %2 kbit/s (%3ms)</source>
        <translation>サーバの最大ネットワーク帯域は%1kbit/s しかありません。音質を自動的に%2kbit/s (%3ms) に調整します</translation>
    </message>
    <message>
        <location filename="AudioInput.ui"/>
        <source>Use Amplitude based speech detection</source>
        <translation>振幅による発言検出を使う</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This selects how long after a perceived stop in speech transmission should continue.&lt;/b&gt;&lt;br /&gt;Set this higher if your voice breaks up when you speak (seen by a rapidly blinking voice icon next to your name).</source>
        <translation>&lt;b&gt;発言の転送をどれだけ継続するべきかを設定します。&lt;/b&gt;&lt;br /&gt;あなたの声が発言中に破綻する（あなたの名前の次にある音声アイコンがすぐに点滅する場合）ならにこの値を高く設置してください。</translation>
    </message>
    <message>
        <location/>
        <source>Idle AutoDeafen</source>
        <translation>自動聴取禁止までの待ち時間</translation>
    </message>
    <message>
        <location/>
        <source>How long to remain idle before auto-deafen.</source>
        <translation>どれだけのアイドル時間が続いたら自動聴取禁止にするか。</translation>
    </message>
    <message>
        <location/>
        <source>This sets an idle timer. If the timer expires without any audio being sent to the server, you are muted and deafened.</source>
        <translation>この設定はアイドルタイマーです。サーバに音声を送信しないままこの時間が過ぎると、あなたは発言禁止と聴取禁止状態になります。</translation>
    </message>
    <message>
        <location/>
        <source>Exclusive mode</source>
        <translation>排他モード</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This opens the device in exclusive mode.&lt;/b&gt;&lt;br /&gt;No other application will be able to use the device.</source>
        <translation>&lt;b&gt;これによりデバイスを排他モードで開きます。&lt;/b&gt;&lt;br /&gt;他のアプリケーションがデバイスを使えなくなります。</translation>
    </message>
    <message>
        <location/>
        <source>Exclusive</source>
        <translation>排他モード</translation>
    </message>
    <message>
        <location/>
        <source>R&amp;eset</source>
        <translation>リセット(&amp;R)</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Browse...</source>
        <translation>参照(&amp;B)...</translation>
    </message>
    <message>
        <location/>
        <source>B&amp;rowse...</source>
        <translation>参照(&amp;R)...</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Preview</source>
        <translation>プレビュー(&amp;P)</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Preview&lt;/b&gt;&lt;br/&gt;Plays the current &lt;i&gt;on&lt;/i&gt; sound followed by the current &lt;i&gt;off&lt;/i&gt; sound.</source>
        <translation type="unfinished">&lt;b&gt;プレビュー&lt;/b&gt;&lt;br /&gt;現在のオンのときのサウンドを再生しそのあとにオフのときのサウンドを再生する。</translation>
    </message>
</context>
<context>
    <name>AudioInputDialog</name>
    <message>
        <location filename="AudioConfigDialog.cpp" line="+71"/>
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
        <translation>プッシュ・トゥ・トーク</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Audio Input</source>
        <translation>音声入力</translation>
    </message>
    <message>
        <location line="+99"/>
        <location line="+8"/>
        <source>%1 ms</source>
        <translation>%1 ms</translation>
    </message>
    <message>
        <location line="-2"/>
        <location line="+20"/>
        <location line="+18"/>
        <source>Off</source>
        <translation>オフ</translation>
    </message>
    <message>
        <location line="-30"/>
        <source>%1 s</source>
        <translation>%1 s</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>%1 kb/s</source>
        <translation></translation>
    </message>
    <message>
        <location line="+11"/>
        <source>-%1 dB</source>
        <translation>-%1 dB</translation>
    </message>
    <message>
        <location line="+55"/>
        <source>%1 kbit/s (Audio %2 %5, Position %4, Overhead %3)</source>
        <oldsource>%1kbit/s (Audio %2 %5, Position %4, Overhead %3)</oldsource>
        <translation>%1kbit/s (音声 %2 %5, 位置 %4, オーバヘッド %3)</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>CELT</source>
        <translation></translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Speex</source>
        <translation></translation>
    </message>
    <message>
        <source>%1kbit/s (Audio %2, Position %4, Overhead %3)</source>
        <translation type="obsolete">%1kbit/s (Audio %2, Position %4, Overhead %3)</translation>
    </message>
    <message>
        <location line="-42"/>
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
        <translation>音声出力方法</translation>
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
        <translation>位置音響</translation>
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
        <source>Minimum distance to user before sound volume decreases</source>
        <translation>音が小さくなる前のユーザへの最小距離</translation>
    </message>
    <message>
        <location/>
        <source>This sets the minimum distance for sound calculations. The volume of other users&apos; speech will not decrease until they are at least this far away from you.</source>
        <translation>サウンドの計算に使う最小距離を設定します。少なくともこの距離だけ離れていれば、他のプレイヤーの音声は減衰しません。</translation>
    </message>
    <message>
        <location/>
        <source>This sets the maximum distance for sound calculations. When farther away than this, other users&apos; speech volume will not decrease any further.</source>
        <translation>音の計算のために最大距離の設定します。 これよりより遠くに離れているとき、他のプレーヤーのスピーチボリュームはこれ以上減少しないでしょう。</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This enables one of the loopback test modes.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;None&lt;/i&gt; - Loopback disabled&lt;br /&gt;&lt;i&gt;Local&lt;/i&gt; - Emulate a local server.&lt;br /&gt;&lt;i&gt;Server&lt;/i&gt; - Request loopback from server.&lt;br /&gt;Please note than when loopback is enabled, no other users will hear your voice. This setting is not saved on application exit.</source>
        <translation>&lt;b&gt;ループバックテストモードの一つが有効になります。&lt;/b&gt;&lt;br /&gt;&lt;i&gt;なし&lt;/i&gt; - ループバックしない&lt;br /&gt;&lt;i&gt;ローカル&lt;/i&gt; - ローカルなサーバをエミュレートします。&lt;br /&gt;&lt;i&gt;サーバ&lt;/i&gt; - サーバにループバックを要求します。&lt;br /&gt;ループバックが有効になっている間、他のプレイヤーはあなたの声を聞くことが出来ないのでご注意ください。この設定はアプリケーション終了時に保存されません。</translation>
    </message>
    <message>
        <source>TextLabel</source>
        <translation type="obsolete">テキストラベル</translation>
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
        <source>This sets the amount of data to prebuffer in the output buffer. Experiment with different values and set it to the lowest which doesn&apos;t cause rapid jitter in the sound.</source>
        <translation type="obsolete">出力バッファにおけるプリバッファの量を設定します。いろいろな値で試してみて、急に不安定にならない一番小さな値に設定してください。</translation>
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
        <source>&lt;b&gt;This sets the packet latency variance for loopback testing.&lt;/b&gt;&lt;br /&gt;Most audio paths contain some variable latency. This allows you to set that variance for loopback mode testing. For example, if you set this to 15ms, this will emulate a network with 20-35ms ping latency or one with 80-95ms latency. Most domestic net connections have a variance of about 5ms.</source>
        <oldsource>&lt;b&gt;This sets the packet latency variance for loopback testing.&lt;/b&gt;&lt;br /&gt;Most audio paths contain some variable latency. This allows you set that variance for loopback mode testing. For example, if you set this to 15ms, this will emulate a network with 20-35ms ping latency or one with 80-95ms latency. Most domestic net connections have a variance of about 5ms.</oldsource>
        <translation>&lt;b&gt;ループバックテストの為のパケットレイテンシーの値を調整します。&lt;/b&gt;&lt;br /&gt;音声のネットワーク経路は、ほとんどの場合若干のレイテンシーを含んでいます。 この設定はループバックモードでのテスト時、レイテンシーの値を変更できるようにします。例えば、この値を15msにすると、20-35msのレイテンシーや80ms-95msのレイテンシーを持ったネットワークをエミュレートするでしょう。 国内のネット接続ではおよそ5msくらいのレイテンシーがあります。</translation>
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
        <source>&lt;b&gt;This enables one of the loopback testmodes.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;None&lt;/i&gt; - Loopback disabled&lt;br /&gt;&lt;i&gt;Local&lt;/i&gt; - Emulate a local server.&lt;br /&gt;&lt;i&gt;Server&lt;/i&gt; - Request loopback from server.&lt;br /&gt;Please note than when loopback is enabled, no other players will hear your voice. This setting is not saved on application exit.</source>
        <translation type="obsolete">&lt;b&gt;ループバックテストモードの一つが有効になります。&lt;/b&gt;&lt;br /&gt;&lt;i&gt;なし&lt;/i&gt; - ループバックしない&lt;br /&gt;&lt;i&gt;ローカル&lt;/i&gt; - ローカルなサーバをエミュレートします。&lt;br /&gt;&lt;i&gt;サーバ&lt;/i&gt; - サーバにループバックを要求します。&lt;br /&gt;ループバックが有効になっている間、他のプレイヤーはあなたの声を聞くことが出来ないのでご注意ください。この設定はアプリケーション終了時に保存されません。</translation>
    </message>
    <message>
        <location/>
        <source>Factor for sound volume increase</source>
        <translation>ボリューム増加の割合</translation>
    </message>
    <message>
        <location/>
        <source>The connected &quot;speakers&quot; are actually headphones</source>
        <oldsource>The connected &quot;speakers&quot; are actually headphones.</oldsource>
        <translation>接続している &quot;出力機器&quot; はヘッドホンです</translation>
    </message>
    <message>
        <location/>
        <source>Minimum Distance</source>
        <translation>最小距離</translation>
    </message>
    <message>
        <source>Minimum distance to player before sound volume decreases</source>
        <translation type="obsolete">サウンドのボリュームが減る前のプレイヤに対する最小の距離です。</translation>
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
        <source>This sets the maximum distance for sound calculations. When farther away than this, other players&apos; speech volume will not decrease any further.</source>
        <translation type="obsolete">音の計算のために最大距離の設定します。 これよりより遠くに離れているとき、他のプレーヤーのスピーチボリュームはこれ以上減少しないでしょう。</translation>
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
        <translation>入力ソースの近さによってどのくらいボリュームを上げるべきか?</translation>
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
        <source>Other Applications</source>
        <translation type="obsolete">他のアプリケーションの音量</translation>
    </message>
    <message>
        <location/>
        <source>This sets the amount of data to pre-buffer in the output buffer. Experiment with different values and set it to the lowest which doesn&apos;t cause rapid jitter in the sound.</source>
        <translation>出力バッファにおけるプリバッファの量を設定します。いろいろな値で試してみて、急に不安定にならない一番小さな値に設定してください。</translation>
    </message>
    <message>
        <source>Volume of other applications during speech</source>
        <translation type="obsolete">発言中の他のアプリケーションの音量</translation>
    </message>
    <message>
        <source>&lt;b&gt;Decrease volume of other applications during speech.&lt;/b&gt;&lt;br /&gt;Mumble supports decreasing the volume of other applications during incoming speech. This sets the relative volume of other applications when others are talking.</source>
        <translation type="obsolete">&lt;b&gt;発言中における他のアプリケーションの音量減少&lt;/b&gt;&lt;br /&gt;Mumbleは音声受信中に他のアプリケーションの音量を下げるのを手助けします。ここでは、あなたが話している間の他アプリケーションの音量比率を設定します。</translation>
    </message>
    <message>
        <source>If checked Mumble also lowers the volume of other applications when you talk</source>
        <translation type="obsolete">チェックすると、Mumble はあなたが発言しているときに他のアプリケーションの音量を下げます</translation>
    </message>
    <message>
        <source>Attenuate other applications while you talk</source>
        <translation type="obsolete">あなたが発言しているとき、他のアプリケーションの音量を下げる</translation>
    </message>
    <message>
        <location/>
        <source>Attenuate applications by...</source>
        <translation>他のアプリの音量を減衰...</translation>
    </message>
    <message>
        <location/>
        <source>Attenuation of other applications during speech</source>
        <translation>会話中の他のアプリケーションの減衰</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Attenuate volume of other applications during speech&lt;/b&gt;&lt;br /&gt;Mumble supports decreasing the volume of other applications during incoming and/or outgoing speech. This sets the attenuation of other applications if the feature is enabled.</source>
        <translation>&lt;b&gt;会話中に他のアプリケーションの音量の減衰させます&lt;/b&gt;&lt;br /&gt;Mumbleは、相手が発言しているときや、発言を相手に伝えている間に他のアプリケーションの音量を減らすことをサポートします。　この機能が有効になっているなら、アプリケーションが減衰されます。</translation>
    </message>
    <message>
        <location/>
        <source>If checked Mumble lowers the volume of other applications while other users talk</source>
        <translation>チェックすると他のユーザが発言しているとき、Mumble は他のアプリケーションの音量を下げます</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Attenuate applications while other users talk&lt;/b&gt;&lt;br /&gt;Mumble supports decreasing the volume of other applications during incoming and/or outgoing speech. This makes mumble activate the feature while other users talk to you.</source>
        <translation>&lt;b&gt;他のユーザが発言しているときに他のアプリケーションの音量の減衰させます&lt;/b&gt;&lt;br /&gt;Mumbleは、相手が発言しているときや、発言を相手に伝えている間に他のアプリケーションの音量を減らすことをサポートします。　この機能が有効になっているなら、他のユーザが発言しているときアプリケーションが減衰されます。</translation>
    </message>
    <message>
        <location/>
        <source>while other users talk</source>
        <translation>他のユーザの発言中</translation>
    </message>
    <message>
        <location/>
        <source>If checked Mumble lowers the volume of other applications while you talk</source>
        <translation>チェックするとあなたが発言しているとき、Mumble は他のアプリケーションの音量を下げます</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Attenuate applications while you talk&lt;/b&gt;&lt;br /&gt;Mumble supports decreasing the volume of other applications during incoming and/or outgoing speech. This makes mumble activate the feature while you talk.</source>
        <translation>&lt;b&gt;あなたが発言しているときに他のアプリケーションの音量の減衰させます&lt;/b&gt;&lt;br /&gt;Mumbleは、相手が発言しているときや、発言を相手に伝えている間に他のアプリケーションの音量を減らすことをサポートします。　この機能が有効になっているなら、あなたが発言しているときアプリケーションが減衰されます。</translation>
    </message>
    <message>
        <location/>
        <source>while you talk</source>
        <translation>あなたの発言中</translation>
    </message>
    <message>
        <location/>
        <source>Exclusive mode, not recommended.</source>
        <translation>排他モード、推奨されません。</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This opens the device in exclusive mode.&lt;/b&gt;&lt;br /&gt;No other application will be able to use the device.</source>
        <translation>&lt;b&gt;これによりデバイスを排他モードで開きます。&lt;/b&gt;&lt;br /&gt;他のアプリケーションがデバイスを使えなくなります。</translation>
    </message>
    <message>
        <location/>
        <source>Exclusive</source>
        <translation>排他モード</translation>
    </message>
</context>
<context>
    <name>AudioOutputDialog</name>
    <message>
        <location filename="AudioConfigDialog.cpp" line="+165"/>
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
        <location line="+106"/>
        <location line="+19"/>
        <location line="+8"/>
        <source>%1 ms</source>
        <translation>%1 ms</translation>
    </message>
    <message>
        <location line="-16"/>
        <location line="+4"/>
        <location line="+8"/>
        <location line="+31"/>
        <location line="+4"/>
        <source>%1 %</source>
        <translation></translation>
    </message>
    <message>
        <location line="-16"/>
        <location line="+6"/>
        <source>%1 m</source>
        <translation></translation>
    </message>
    <message>
        <source>%1%</source>
        <translation type="obsolete">%1%</translation>
    </message>
    <message>
        <source>%1ms</source>
        <translation type="obsolete">%1ms</translation>
    </message>
    <message>
        <source>%1m</source>
        <translation type="obsolete">%1m</translation>
    </message>
    <message>
        <source>%1</source>
        <translation type="obsolete">%1</translation>
    </message>
</context>
<context>
    <name>AudioOutputSample</name>
    <message>
        <location filename="AudioOutput.cpp" line="+297"/>
        <source>Choose sound file</source>
        <translation>サウンドファイルを選択してください</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Invalid sound file</source>
        <translation>無効なサウンドファイル</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The file &apos;%1&apos; cannot be used by Mumble. Please select a file with a compatible format and encoding.</source>
        <translation>ファイル &apos;%1&apos; はMumble で使用できません。対応するフォーマットとエンコードのファイルを選択してください。</translation>
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
        <source>This shows the peak power in the last frame (20 ms) after all processing. Ideally, this should be -96 dB when you&apos;re not talking. In reality, a sound studio should see -60 dB, and you should hopefully see somewhere around -20 dB. When you are talking, this should rise to somewhere between -5 and -10 dB.&lt;br /&gt;If you are using echo cancellation, and this rises to more than -15 dB when you&apos;re not talking, your setup is not working, and you&apos;ll annoy other users with echoes.</source>
        <translation>直近のフレーム(20 ms)における全ての処理後の最大音量を表示します。あなたが話していないとき、これは -96 dBであるのが理想です。しかし実際には音楽スタジオでも -60 dB、あなたの環境では -20 dB くらい出れば良い方でしょう。あなたが話している時は、-5 から -10 dB のあたりまで上がります。&lt;br /&gt;もしあなたがエコーキャンセルを使っていて、話してもいないのに -15 dB まで上がるようならセットアップは失敗しています。この場合、反響によってほかのプレイヤーを悩ますことになります。</translation>
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
        <source>Time between last two Push-To-Talk presses</source>
        <translation>直近2回のプッシュ・トゥ・トークの間隔</translation>
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
        <location filename="AudioStats.cpp" line="+405"/>
        <source>&gt;1000 ms</source>
        <oldsource>&gt;1000ms</oldsource>
        <translation>&gt;1000ms</translation>
    </message>
    <message>
        <location filename="AudioStats.ui"/>
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
        <source>This is the audio bitrate of the last compressed frame (20 ms), and as such will jump up and down as the VBR adjusts the quality. The peak bitrate can be adjusted in the Settings dialog.</source>
        <translation type="unfinished">直近の圧縮されたフレーム(20 ms)における音声ビットレートです。VBRが品質を調整するので値は上下します。ピークビットレートを調整するには、設定ダイアログで調整できます。</translation>
    </message>
</context>
<context>
    <name>AudioWizard</name>
    <message>
        <location filename="AudioWizard.ui"/>
        <source>Audio Tuning Wizard</source>
        <translation>音声調整ウィザード</translation>
    </message>
    <message>
        <location/>
        <source>Introduction</source>
        <translation>イントロダクション</translation>
    </message>
    <message>
        <location/>
        <source>Welcome to the Mumble Audio Wizard</source>
        <translation>Mumble 音声ウィザードへようこそ</translation>
    </message>
    <message>
        <source>This is the audio tuning wizard for Mumble. This will help you correctly set the input levels of your sound card, and also set the correct parameters for sound processing in Mumble.</source>
        <translation type="obsolete">これはMumbleの音声を調整するためのウィザードです。サウンドカードの入力レベルとMumbleでの音声処理のパラメータを正しく設定するお手伝いをします。</translation>
    </message>
    <message>
        <source>Please be aware that as long as this wizard is active, audio will be looped locally to allow you to listen to it, and no audio will be sent to the server.</source>
        <translation type="obsolete">ウィザードが動作中の間、音声はあなたが聞けるようPC上だけで完結し、サーバには送信されませんのでご注意ください。</translation>
    </message>
    <message>
        <location/>
        <source>Finished</source>
        <translation>完了</translation>
    </message>
    <message>
        <location/>
        <source>Enjoy using Mumble</source>
        <translation>Mumbleをお楽しみください</translation>
    </message>
    <message>
        <source>Congratulations. You should now be ready to enjoy a richer sound experience with Mumble.</source>
        <translation type="obsolete">おめでとうございます！Mumbleでリッチなサウンド体験をする準備が整いました。</translation>
    </message>
    <message>
        <location/>
        <source>Device selection</source>
        <translation>デバイス選択</translation>
    </message>
    <message>
        <location/>
        <source>Selecting the input and output device to use with Mumble.</source>
        <translation>Mumbleで使用する入力および出力のデバイスを選択します。</translation>
    </message>
    <message>
        <source>Input device</source>
        <translation type="obsolete">入力デバイス</translation>
    </message>
    <message>
        <location/>
        <source>This is the device your microphone is connected to.</source>
        <translation>あなたが接続しているデバイスです。</translation>
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
        <source>Input device to use</source>
        <translation>入力に使用するデバイス</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Selects which sound card to use for audio input.&lt;/b&gt;</source>
        <translation>&lt;b&gt;音声の入力にどのサウンドカードを使用するか選択してください。&lt;/b&gt;</translation>
    </message>
    <message>
        <source>Output device</source>
        <translation type="obsolete">出力デバイス</translation>
    </message>
    <message>
        <location/>
        <source>This is the device your speakers or headphones are connected to.</source>
        <translation>あなたのスピーカ(もしくはヘッドホン)が接続しているデバイスです。</translation>
    </message>
    <message>
        <location/>
        <source>Output method for audio</source>
        <translation>音声の出力方法</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is the Output method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation>&lt;b&gt;音声の出力方法です。&lt;/b&gt;&lt;br /&gt;ほとんどの場合DirectSoundで問題ありません。</translation>
    </message>
    <message>
        <location/>
        <source>Output device to use</source>
        <translation>出力に使用するデバイス</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Selects which sound card to use for audio Output.&lt;/b&gt;</source>
        <translation>&lt;b&gt;音声の出力にどのサウンドカードを使用するか選択してください。&lt;/b&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Volume tuning</source>
        <translation>音量調整</translation>
    </message>
    <message>
        <location/>
        <source>Tuning microphone hardware volume to optimal settings.</source>
        <translation>マイクの音量を最適な設定になるよう調整します。</translation>
    </message>
    <message>
        <source>Open your sound control panel and go to the recording settings. Make sure the microphone is selected as active input with maximum recording volume. If there&apos;s an option to enable a &quot;Microphone boost&quot; make sure it&apos;s checked.</source>
        <translation type="obsolete">ボリュームコントロールパネルを開き、録音設定画面に移動しましょう。そして、マイクが選択されていて、音量が最大になっている事を確認してください。もし&quot;マイクブースト&quot;を有効にするオプションがあるなら、それもチェックされているか確認してください。</translation>
    </message>
    <message>
        <source>Speak loudly, as when you are annoyed or excited. Decrease the volume in the sound control panel until the bar below stays as high as possible in the blue and green but &lt;b&gt;not&lt;/b&gt; the red zone while you speak.</source>
        <translation type="obsolete">イライラしていたり、興奮していたりする時のように大声で話してみてください。話している間、下にあるバーが青と緑の区域のなるべく赤に近い場所に留まるまでサウンドコントロールパネルの音量を下げてください。(赤い区域には&lt;b&gt;入らない&lt;/b&gt;ようにしましょう)</translation>
    </message>
    <message>
        <location/>
        <source>Now talk softly, as you would when talking late at night and you don&apos;t want to disturb anyone. Adjust the slider below so that the bar moves into green when you talk, but stays blue while you&apos;re silent.</source>
        <translation>次に、誰も起こさないようにしている夜中のようにそっと話してみましょう。話している間、バーが緑にかかり、静かにしている時は青に留まるように下のスライダーを調整してください。</translation>
    </message>
    <message>
        <location/>
        <source>Voice Activity Detection</source>
        <translation>発声の検出</translation>
    </message>
    <message>
        <location/>
        <source>Letting Mumble figure out when you&apos;re talking and when you&apos;re silent.</source>
        <translation>話している状態と静かにしている状態をMumbleに判別させます。</translation>
    </message>
    <message>
        <location/>
        <source>This will help Mumble figure out when you are talking. The first step is selecting which data value to use.</source>
        <translation>あなたがいつ話しているのかをMumbleに判別させるお手伝いをします。まず最初に、どのデータの値を使うかの選択を行います。</translation>
    </message>
    <message>
        <location/>
        <source>Raw amplitude from input</source>
        <translation>音声信号の大きさ</translation>
    </message>
    <message>
        <location/>
        <source>Signal-To-Noise ratio</source>
        <translation>信号対雑音比(S/N比)</translation>
    </message>
    <message>
        <location/>
        <source>Next you need to adjust the following slider. The first few utterances you say should end up in the green area (definitive speech). While talking, you should stay inside the yellow (might be speech) and when you&apos;re not talking, everything should be in the red (definitively not speech).</source>
        <oldsource>Next you need to adjust the following two sliders. The first few utterances you say should end up in the green area (definitive speech). While talking, you should stay inside the yellow (might be speech) and when you&apos;re not talking, everything should be in the red (definitively not speech).</oldsource>
        <translation>次に、以下の2つのスライダーを調節しましょう。最初の発声は緑(確実に発言と判定)になると良いでしょう。話している最中は黄色(発言だろうと判定)の中に収まり、話していない時はすべて赤(発言ではないと判定)に入っているようにしてください。</translation>
    </message>
    <message>
        <source>Last, you need to adjust the grace time. Mumble may detect this much non-speech and still keep transmitting before it breaks off. This allows you to catch your breath while speaking.</source>
        <translation type="obsolete">最後に、猶予時間を調整しましょう。発言以外のものを検出する許容量が上がり、話が途切れずに送信を続けることができるでしょう。これにより、話の間に小休止を入れることができるようになります。</translation>
    </message>
    <message>
        <location/>
        <source>Device tuning</source>
        <translation>デバイス調整</translation>
    </message>
    <message>
        <location/>
        <source>Changing hardware output delays to their minimum value.</source>
        <translation>ハードウェアの出力遅延が最小値になるように変更します。</translation>
    </message>
    <message>
        <source>To keep latency to an absolute minium, it&apos;s important to buffer as little audio as possible on the soundcard. However, many soundcards report that they require a much smaller buffer than what they can actually work with, so the only way to set this value is to try and fail.</source>
        <translation type="obsolete">遅延を最小限に保つためには、サウンドカード上で音声のバッファを出来るだけ小さくすることが重要です。しかし、多くのサウンドカードが要求してくるバッファは、実際に動作可能な大きさよりかなり小さなものです。結局、この値を設定するにはいろいろ試してみるほかありません。</translation>
    </message>
    <message>
        <source>Amount of data to buffer.</source>
        <translation type="obsolete">バッファデータの量</translation>
    </message>
    <message>
        <source>This sets the amount of data to prebuffer in the output buffer. Experiment with different values and set it to the lowest which doesn&apos;t cause rapid jitter in the sound.</source>
        <translation type="obsolete">出力バッファにおけるプリバッファの量を設定します。いろいろな値で試してみて、急に不安定にならない一番小さな値に設定してください。</translation>
    </message>
    <message>
        <source>%1ms</source>
        <translation type="obsolete">%1ms</translation>
    </message>
    <message>
        <source>%1 s</source>
        <translation type="obsolete">%1 s</translation>
    </message>
    <message>
        <source>You should hear a single tone that&apos;s changing in frequency. Change the slider below to the lowest value which gives &lt;b&gt;no&lt;/b&gt; interruptions or jitter in the sound. Please note that local echo is disabled during this test to improve audio path recognition.</source>
        <translation type="obsolete">周波数が変わる1つの音色が聞こえますでしょうか。音が途切れたり不安定に&lt;b&gt;ならない&lt;/b&gt;一番小さな値まで下のスライダーを動かしてください。なお、音声経路の認識率を向上するため、このテストの間ローカルエコーは使えなくなっている事をお忘れなく。</translation>
    </message>
    <message>
        <source>If you position the microphone so it can pick up the speakers or headset, Mumble will measure the total audio path delay in your system; this means the delay from a sample is placed in an outbound buffer until it&apos;s found in a matching incoming buffer.</source>
        <translation type="obsolete">スピーカーやヘッドセットの出力をマイクが拾ってしまうような配置にしている場合、あなたのシステムで音声経路の遅延が起きていると判断されるでしょう; つまり、対応する受信バッファ内に含まれるまでサンプルが送信バッファに放置されて、遅延をもたらします。</translation>
    </message>
    <message>
        <source>Audio path is %1ms long.</source>
        <translation type="obsolete">音声経路は %1ms の長さ</translation>
    </message>
    <message>
        <source>Audio path cannot be determined. Input not recognized.</source>
        <translation type="obsolete">音声経路が決定できません。入力が不明です。</translation>
    </message>
    <message>
        <location/>
        <source>Use echo cancellation</source>
        <translation>エコーキャンセルを使う</translation>
    </message>
    <message>
        <source>Cancel echo from headset or speakers.</source>
        <translation type="obsolete">ヘッドセットやスピーカーからのエコーをキャンセルします。</translation>
    </message>
    <message>
        <location/>
        <source>This enables echo cancellation of outgoing audio, which helps both on speakers and on headsets.</source>
        <translation>出力音声のエコーキャンセルを有効にします。スピーカーとヘッドセットの両方に対して有効です。</translation>
    </message>
    <message>
        <location/>
        <source>Enable positional audio</source>
        <translation>位置音響を有効にする</translation>
    </message>
    <message>
        <location/>
        <source>Allows positioning of sound</source>
        <oldsource>Allows positioning of sound.</oldsource>
        <translation>音声の位置</translation>
    </message>
    <message>
        <location/>
        <source>&lt;p&gt;
This is the audio tuning wizard for Mumble. This will help you correctly set the input levels of your sound card, and also set the correct parameters for sound processing in Mumble.
&lt;/p&gt;
&lt;p&gt;
Please be aware that as long as this wizard is active, audio will be looped locally to allow you to listen to it, and no audio will be sent to the server.
&lt;/p&gt;</source>
        <translation>&lt;p&gt;
これはMumbleの音声を調整するためのウィザードです。サウンドカードの入力レベルとMumbleでの音声処理のパラメータを正しく設定するお手伝いをします。
&lt;/p&gt;
&lt;p&gt;
ウィザードが動作中の間、音声はあなたが聞けるようPC上だけで完結し、サーバには送信されませんのでご注意ください。
&lt;/p&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Input Device</source>
        <translation>入力デバイス</translation>
    </message>
    <message>
        <location/>
        <source>Cancel echo from headset or speakers</source>
        <translation>ヘッドセットやスピーカからのエコーをキャンセル</translation>
    </message>
    <message>
        <location/>
        <source>Output Device</source>
        <translation>出力デバイス</translation>
    </message>
    <message>
        <location/>
        <source>This allows Mumble to use positional audio to place voices.</source>
        <translation>声の位置を定めるために位置音響を使えるようにします。</translation>
    </message>
    <message>
        <location/>
        <source>&lt;p&gt;
To keep latency to an absolute minimum, it&apos;s important to buffer as little audio as possible on the soundcard. However, many soundcards report that they require a much smaller buffer than what they can actually work with, so the only way to set this value is to try and fail.
&lt;/p&gt;
&lt;p&gt;
You should hear a voice sample. Change the slider below to the lowest value which gives &lt;b&gt;no&lt;/b&gt; interruptions or jitter in the sound. Please note that local echo is disabled during this test.
&lt;/p&gt;
</source>
        <translation>&lt;p&gt;
遅延を最小限に保つためには、サウンドカード上で音声のバッファを出来るだけ小さくすることが重要です。しかし、多くのサウンドカードが要求してくるバッファは、実際に動作可能な大きさよりかなり小さなものです。結局、この値を設定するにはいろいろ試してみるほかありません。
&lt;/p&gt;
&lt;p&gt;
周波数が変わる1つの音色が聞こえますでしょうか。音が途切れたり不安定に&lt;b&gt;ならない&lt;/b&gt;一番小さな値まで下のスライダーを動かしてください。なお、音声経路の認識率を向上するため、このテストの間ローカルエコーは使えなくなっている事をお忘れなく。
&lt;/p&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Amount of data to buffer</source>
        <translation>バッファデータ量</translation>
    </message>
    <message>
        <location/>
        <source>This sets the amount of data to pre-buffer in the output buffer. Experiment with different values and set it to the lowest which doesn&apos;t cause rapid jitter in the sound.</source>
        <translation>出力バッファにおけるプリバッファの量を設定します。いろいろな値で試してみて、急に不安定にならない一番小さな値に設定してください。</translation>
    </message>
    <message>
        <location/>
        <source>&lt;p&gt;
Open your sound control panel and go to the recording settings. Make sure the microphone is selected as active input with maximum recording volume. If there&apos;s an option to enable a &quot;Microphone boost&quot; make sure it&apos;s checked.
&lt;/p&gt;
&lt;p&gt;
Speak loudly, as when you are annoyed or excited. Decrease the volume in the sound control panel until the bar below stays as high as possible in the blue and green but &lt;b&gt;not&lt;/b&gt; the red zone while you speak.
&lt;/p&gt;
</source>
        <translation>&lt;p&gt;
ボリュームコントロールパネルを開き、録音設定画面に移動しましょう。そして、マイクが選択されていて、音量が最大になっている事を確認してください。もし&quot;マイクブースト&quot;を有効にするオプションがあるなら、それもチェックされているか確認してください。
&lt;/p&gt;
&lt;p&gt;
イライラしていたり、興奮していたりする時のように大声で話してみてください。話している間、下にあるバーが青と緑の区域のなるべく赤に近い場所に留まるまでサウンドコントロールパネルの音量を下げてください。(赤い区域には&lt;b&gt;入らない&lt;/b&gt;ようにしましょう)
&lt;/p&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Positional Audio</source>
        <translation>位置音響</translation>
    </message>
    <message>
        <location/>
        <source>Adjusting attenuation of positional audio.</source>
        <translation>位置音響の減衰効果を調整します。</translation>
    </message>
    <message>
        <location/>
        <source>&lt;p&gt;
Mumble supports positional audio for some games, and will position the voice of other users relative to their position in game. Depending on their position, the volume of the voice will be changed between the speakers to simulate the direction and distance the other user is at. Such positioning depends on your speaker configuration being correct in your operating system, so a test is done here.
&lt;/p&gt;
&lt;p&gt;
The graph below shows the position of &lt;font color=&quot;red&quot;&gt;you&lt;/font&gt;, the &lt;font color=&quot;yellow&quot;&gt;speakers&lt;/font&gt; and a &lt;font color=&quot;green&quot;&gt;moving sound source&lt;/font&gt; as if seen from above. You should hear the audio move between the channels.
&lt;/p&gt;
</source>
        <translation>&lt;p&gt;
Mumbleは、いくつかのゲームにおいてゲーム内での他プレイヤーの位置に対応した位置音響をサポートします。他プレイヤーの位置によって、方向と距離をシミュレートして音量が変化します。このシミュレーションはお使いのオペレーティング・システムでスピーカーが正しく設定されていることが前提となります。そのテストはここで行うことが出来ます。
&lt;/p&gt;
&lt;p&gt;
このグラフはこれらの位置を表しています: &lt;font color=&quot;red&quot;&gt;あなた&lt;/font&gt;, the &lt;font color=&quot;yellow&quot;&gt;スピーカー&lt;/font&gt; and a &lt;font color=&quot;green&quot;&gt;移動する音の発生源&lt;/font&gt; 上からに見えるようなら、チャンネルの間に移動して聞くのがよいでしょう。
&lt;/p&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Use headphones instead of speakers</source>
        <translation>スピーカーの代わりにヘッドホンを使います</translation>
    </message>
    <message>
        <location/>
        <source>&lt;p&gt;
Congratulations. You should now be ready to enjoy a richer sound experience with Mumble.
&lt;/p&gt;
&lt;p&gt;
Mumble is under continuous development, and the development team wants to focus on the features that benefit the most users. To this end, Mumble supports submitting anonymous statistics about your configuration to the developers. These statistics are essential for future development, and also make sure the features you use aren&apos;t deprecated.
&lt;/p&gt;
</source>
        <translation>&lt;p&gt;
おめでとうございます！Mumbleでリッチなサウンド体験をする準備が整いました。
&lt;/p&gt;
&lt;p&gt;
Mumbleは現在開発中であり、開発チームはより多くのユーザの利益となる機能に注力したいと考えています。そのため、匿名で設定に関する統計を送信する機能を追加しました。これらの統計情報は主に将来の開発のために使用されるか、特定の機能が使用されていないかどうかを確認するために使用されます。
&lt;/p&gt;</translation>
    </message>
    <message>
        <source>Mumble supports positional audio for some games, and will position the voice of other players relative to their position in game. Depending on their position, the volume of the voice will be changed between the speakers to simulate the direction and distance the other player is at. Such positioning depends on your speaker configuration being correct in your operating system, so a test is done here.</source>
        <translation type="obsolete">Mumbleは、いくつかのゲームにおいてゲーム内での他プレイヤーの位置に対応した位置音響をサポートします。他プレイヤーの位置によって、方向と距離をシミュレートして音量が変化します。このシミュレーションはお使いのオペレーティング・システムでスピーカーが正しく設定されていることが前提となります。そのテストはここで行うことが出来ます。</translation>
    </message>
    <message>
        <source>The graph below shows the position of &lt;font color=&quot;red&quot;&gt;you&lt;/font&gt;, the &lt;font color=&quot;yellow&quot;&gt;speakers&lt;/font&gt; and a &lt;font color=&quot;green&quot;&gt;moving sound source&lt;/font&gt; as if seen from above. You should hear the audio move between the channels.</source>
        <translation type="obsolete">このグラフはこれらの位置を表しています: &lt;font color=&quot;red&quot;&gt;あなた&lt;/font&gt;, the &lt;font color=&quot;yellow&quot;&gt;スピーカー&lt;/font&gt; and a &lt;font color=&quot;green&quot;&gt;移動する音の発生源&lt;/font&gt; 上からに見えるようなら、チャンネルの間に移動して聞くのがよいでしょう。</translation>
    </message>
    <message>
        <location/>
        <source>Use headphones</source>
        <translation>ヘッドホンを使う</translation>
    </message>
    <message>
        <source>Use headphones instead of speakers.</source>
        <translation type="obsolete">スピーカーの代わりにヘッドホンを使う</translation>
    </message>
    <message>
        <location/>
        <source>This ignores the OS speaker configuration and configures the positioning for headphones instead.</source>
        <translation>OSのスピーカー構成を無視して、代わりにヘッドホン用の位置設定を使用します。</translation>
    </message>
    <message>
        <source>Mumble is under continuous development, and the development team wants to focus on the features that benefit the most users. To this end, Mumble supports submitting anonymous statistics about your configuration to the developers. These statistcs are essential for future development, and also make sure the features you use aren&apos;t deprecated.</source>
        <translation type="obsolete">Mumbleは現在開発中であり、開発チームはより多くのユーザの利益となる機能に注力したいと考えています。そのため、匿名で設定に関する統計を送信する機能を追加しました。これらの統計情報は主に将来の開発のために使用されるか、特定の機能が使用されていないかどうかを確認するために使用されます。</translation>
    </message>
    <message>
        <location/>
        <source>Submit anonymous statistics to the Mumble project</source>
        <translation>Mumbleプロジェクトに匿名で統計を送信する</translation>
    </message>
    <message>
        <location/>
        <source>Push To Talk:</source>
        <translation>プッシュ・トゥ・トーク:</translation>
    </message>
    <message>
        <location/>
        <source>Quality &amp; Notifications</source>
        <translation>品質 &amp; 通知</translation>
    </message>
    <message>
        <location/>
        <source>Adjust quality and notification settings.</source>
        <translation>音質と通知の設定を調整します。</translation>
    </message>
    <message>
        <location/>
        <source>Quality settings</source>
        <translation>品質の設定</translation>
    </message>
    <message>
        <location/>
        <source>Low</source>
        <translation>低</translation>
    </message>
    <message>
        <source>In this configuration Mumble will use a &lt;b&gt;low amount of bandwidth&lt;/b&gt;. This will inevitable result in high latency and poor quality. Choose this only if your connection cannot handle the other settings. (Speex 16kbit/s, 60ms per packet)</source>
        <translation type="obsolete">この設定ではMumbleは&lt;b&gt;少量の帯域&lt;/b&gt;を使います。 これにより当然ながら高い遅延と残念な音質になります。 あなたのネットワーク環境で他の設定がうまく動作しない場合のみこの設定を選んでください。. (Speex 16kbit/s, 60ms / packet)</translation>
    </message>
    <message>
        <location/>
        <source>Balanced</source>
        <translation>バランス</translation>
    </message>
    <message>
        <source>This is the &lt;b&gt;recommended default&lt;/b&gt; configuration. It provides a good balance between quality, latency and bandwidth usage. (CELT 40kbit/s, 20ms per packet)</source>
        <translation type="obsolete">これは&lt;b&gt;標準で推奨される&lt;/b&gt;設定です。音質・遅延と帯域との間に素晴らしいバランスを提供します。 (CELT 40kbit/s, 20ms per packet)</translation>
    </message>
    <message>
        <source>Ultra</source>
        <translation type="obsolete">ウルトラ</translation>
    </message>
    <message>
        <source>This configuration is only recommended for use in setups where bandwidth is not an issue like a LAN. It provides the lowest latency supported by Mumble and &lt;b&gt;high quality&lt;/b&gt;. (CELT 72kbit/s, 10ms per packet)</source>
        <translation type="obsolete">この設定はLAN内のような通信帯域が問題とならない状況で使うことが推奨されます。 Mumble がサポートする最小の遅延と&lt;b&gt;高い品質&lt;/b&gt;提供します。. (CELT 72kbit/s, 10ms / packet)</translation>
    </message>
    <message>
        <location/>
        <source>Notification settings</source>
        <translation>通知設定</translation>
    </message>
    <message>
        <location/>
        <source>Use Text-To-Speech to read notifications and messages to you.</source>
        <translation>通知を読み上げあなたに伝えます。</translation>
    </message>
    <message>
        <location/>
        <source>Disable Text-To-Speech and use sounds instead.</source>
        <translation>テキスト読み上げを無効にして、代わりにサウンドファイルを再生します。</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="+306"/>
        <source>%1 ms</source>
        <translation>%1ms</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui"/>
        <source>Enables attenuation of other applications while users talk to you</source>
        <translation>ユーザが会話しているときに他のアプリケーションの減衰を有効化</translation>
    </message>
    <message>
        <source>Enables attenuation of other applications while users talk to you. This means that as soon someone is starting to speak to you in mumble the sound of all other applications, like audio players, will get attenuated so you can understand them better.</source>
        <translation type="obsolete">ユーザが会話しているときに他のアプリケーションの減衰を有効にする。 Mumble で誰かがあなたに話し始めたらすぐに、他のすべてのアプリケーションの音量はあなたが他人の会話を良く理解出来るように減衰されます。</translation>
    </message>
    <message>
        <location/>
        <source>Attenuate applications while other users talk</source>
        <translation>他のユーザが話しているとき他のアプリケーションの音量を減らす</translation>
    </message>
    <message>
        <location/>
        <source>In this configuration Mumble will use a &lt;b&gt;low amount of bandwidth&lt;/b&gt;. This will inevitably result in high latency and poor quality. Choose this only if your connection cannot handle the other settings. (Speex 16kbit/s, 60ms per packet)</source>
        <translation>この設定ではMumbleは&lt;b&gt;少量の帯域&lt;/b&gt;を使います。 これにより当然ながら高い遅延と残念な音質になります。 あなたのネットワーク環境で他の設定がうまく動作しない場合のみこの設定を選んでください。. (Speex 16kbit/s, 60ms / packet)</translation>
    </message>
    <message>
        <location/>
        <source>This is the &lt;b&gt;recommended default&lt;/b&gt; configuration. It provides a good balance between quality, latency, and bandwidth usage. (CELT 40kbit/s, 20ms per packet)</source>
        <translation>これは&lt;b&gt;標準で推奨される&lt;/b&gt;設定です。音質・遅延と帯域との間に素晴らしいバランスを提供します。 (CELT 40kbit/s, 20ms per packet)</translation>
    </message>
    <message>
        <location/>
        <source>High</source>
        <translation>高</translation>
    </message>
    <message>
        <location/>
        <source>This configuration is only recommended for use in setups where bandwidth is not an issue, like a LAN. It provides the lowest latency supported by Mumble and &lt;b&gt;high quality&lt;/b&gt;. (CELT 72kbit/s, 10ms per packet)</source>
        <translation>この設定はLAN内のような通信帯域が問題とならない状況で使うことが推奨されます。 Mumble がサポートする最小の遅延と&lt;b&gt;高い品質&lt;/b&gt;提供します。. (CELT 72kbit/s, 10ms / packet)</translation>
    </message>
    <message>
        <location/>
        <source>Custom</source>
        <translation>カスタム</translation>
    </message>
    <message>
        <location/>
        <source>You already set a customized quality configuration in Mumble. Select this setting to keep it.</source>
        <translation>あなたはすでに音質設定をカスタマイズしています。設定を保持したい場合はこれを選んでください。</translation>
    </message>
    <message>
        <location/>
        <source>Enables attenuation of other applications while users talk to you. This means that as soon someone starts to speak to you in Mumble, the sound of all other applications (like audio players) will get attenuated so you can hear them more clearly.</source>
        <translation>ユーザが会話しているときに他のアプリケーションの減衰を有効にします。 Mumble で誰かがあなたに話し始めたらすぐに、他のすべてのアプリケーションの音量はあなたが他人の会話を良く理解出来るように減衰されます。</translation>
    </message>
    <message>
        <location/>
        <source>Apply some high contrast optimizations for visually impaired users</source>
        <translation>視覚障害者のためにハイコントラストへ最適化します</translation>
    </message>
    <message>
        <location/>
        <source>Use high contrast graphics</source>
        <translation>ハイコントラストの画像を使う</translation>
    </message>
    <message>
        <location/>
        <source>Keep custom Text-To-Speech settings.</source>
        <translation>カスタムのテキスト・トゥ・スピーク設定を保持します。</translation>
    </message>
    <message>
        <location/>
        <source>&lt;p&gt;
Open your sound control panel and go to the recording settings. Make sure the microphone is selected as active input with maximum recording volume. If there&apos;s an option to enable a &quot;Microphone boost&quot; make sure it&apos;s checked.
&lt;/p&gt;
&lt;p&gt;
Speak loudly, as when you are annoyed or excited. Decrease the volume in the sound control panel until the bar below stays as high as possible in the striped and the empty but &lt;b&gt;not&lt;/b&gt; the crisscrossed zone while you speak.
&lt;/p&gt;
</source>
        <oldsource>&lt;p&gt;
Open your sound control panel and go to the recording settings. Make sure the microphone is selected as active input with maximum recording volume. If there&apos;s an option to enable a &quot;Microphone boost&quot; make sure it&apos;s checked.
&lt;/p&gt;
&lt;p&gt;
Speak loudly, as when you are annoyed or excited. Decrease the volume in the sound control panel until the bar below stays as high as possible in the striped and the empty but &lt;b&gt;not&lt;/b&gt; the criss-crossed zone while you speak.
&lt;/p&gt;
</oldsource>
        <comment>For high contrast mode</comment>
        <translation type="unfinished">&lt;p&gt;
ボリュームコントロールパネルを開き、録音設定画面に移動しましょう。そして、マイクが選択されていて、音量が最大になっている事を確認してください。もし&quot;マイクブースト&quot;を有効にするオプションがあるなら、それもチェックされているか確認してください。
&lt;/p&gt;
&lt;p&gt;
イライラしていたり、興奮していたりする時のように大声で話してみてください。話している間、下にあるバーが青と緑の区域のなるべく空の領域に近い場所に留まるまでサウンドコントロールパネルの音量を下げてください。(赤い区域には&lt;b&gt;入らない&lt;/b&gt;ようにしましょう)
&lt;/p&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Now talk softly, as you would when talking late at night and you don&apos;t want to disturb anyone. Adjust the slider below so that the bar moves into empty zone when you talk, but stays in the striped one while you&apos;re silent.</source>
        <comment>For high contrast mode</comment>
        <translation>次に、誰も起こさないようにしている夜中のようにそっと話してみましょう。話している間、バーが空の領域にかかり、静かにしている時は青に留まるように下のスライダーを調整してください。</translation>
    </message>
    <message>
        <source>Next you need to adjust the following slider. The first few utterances you say should end up in the empty area (definitive speech). While talking, you should stay inside the striped (might be speech) and when you&apos;re not talking, everything should be in the criss-crossed (definitively not speech).</source>
        <comment>For high contrast mode</comment>
        <translation type="obsolete">次に、以下の2つのスライダーを調節しましょう。最初の発声は空の領域(確実に発言と判定)になると良いでしょう。話している最中は黄色(発言だろうと判定)の中に収まり、話していない時はすべて赤(発言ではないと判定)に入っているようにしてください。</translation>
    </message>
    <message>
        <location/>
        <source>Next you need to adjust the following slider. The first few utterances you say should end up in the empty area (definitive speech). While talking, you should stay inside the striped (might be speech) and when you&apos;re not talking, everything should be in the crisscrossed (definitively not speech).</source>
        <comment>For high contrast mode</comment>
        <translation type="unfinished">次に、以下の2つのスライダーを調節しましょう。最初の発声は空の領域(確実に発言と判定)になると良いでしょう。話している最中は黄色(発言だろうと判定)の中に収まり、話していない時はすべて赤(発言ではないと判定)に入っているようにしてください。</translation>
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
        <source>Reason</source>
        <translation>理由</translation>
    </message>
    <message>
        <location/>
        <source>Start</source>
        <translation>開始</translation>
    </message>
    <message>
        <location/>
        <source>End</source>
        <translation>終了</translation>
    </message>
    <message>
        <location/>
        <source>User</source>
        <translation>ユーザ</translation>
    </message>
    <message>
        <location/>
        <source>Hash</source>
        <translation>ハッシュ</translation>
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
        <translation type="obsolete">このボタンは現在の グループとACLを 適用しサーバに送信します。もし誤ってあなた自身の書き込み権限を削除してしまうとサーバに追加されてしまいますのでご注意ください。</translation>
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
        <translation type="obsolete">このボタンは全ての変更をキャンセルし、サーバのACLやグループを変更せずにダイアログを閉じます。</translation>
    </message>
    <message>
        <source>0.0.0.0</source>
        <translation type="obsolete">0.0.0.0</translation>
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
    <name>CertView</name>
    <message>
        <location filename="Cert.cpp" line="+50"/>
        <source>Name</source>
        <translation>名前</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Email</source>
        <translation>Eメール</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Issuer</source>
        <translation>発行者</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Expiry Date</source>
        <translation>有効期限</translation>
    </message>
    <message>
        <location line="+34"/>
        <source>(none)</source>
        <translation>（空欄）</translation>
    </message>
    <message>
        <location line="+11"/>
        <source>Self-signed</source>
        <translation>自己署名</translation>
    </message>
</context>
<context>
    <name>CertWizard</name>
    <message>
        <location line="+90"/>
        <source>Resolving domain %1.</source>
        <translation>ドメイン名 %1 を解決しています。</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Unable to validate email.&lt;br /&gt;Enter a valid (or blank) email to continue.</source>
        <translation>Eメールアドレス を認証できません。&lt;br /&gt;継続するには有効なEメールアドレスを入力するか、空欄のままにしてください。</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>There was an error generating your certificate.&lt;br /&gt;Please try again.</source>
        <translation>証明書の生成でエラーが発生しました。&lt;br /&gt;もう一度、試してみてください。</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Your certificate and key could not be exported to PKCS#12 format. There might be an error in your certificate.</source>
        <translation>証明書とキーはPKCS#12 フォーマットでエクスポートできません。証明書にエラーがある可能性があります。</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>The file could not be opened for writing. Please use another file.</source>
        <translation>そのファイルを書き込みモードで開けませんでした。他のファイルを使用してください。</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>The file could not be written successfully. Please use another file.</source>
        <translation>そのファイルに正しく書き込めませんでした。他のファイルを使ってください。</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>The file could not be opened for reading. Please use another file.</source>
        <translation>そのファイルを読み込みモードで開けませんでした。他のファイルを使用してください。</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>The file is empty or could not be read. Please use another file.</source>
        <translation>そのファイルは空であるか読み込むことができません。他のファイルを使用してください。</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>The file did not contain a valid certificate and key. Please use another file.</source>
        <translation>そのファイルは正しい証明書とキーを含んでいません。他のファイルを使用してください。</translation>
    </message>
    <message>
        <location line="+22"/>
        <source>Select file to export certificate to</source>
        <translation>証明書をエクスポートするためのファイルを選択してください</translation>
    </message>
    <message>
        <location line="+37"/>
        <source>Select file to import certificate from</source>
        <translation>証明書をインポートするためのファイルを選択してください</translation>
    </message>
    <message>
        <location line="+57"/>
        <source>Unable to resolve domain.</source>
        <translation>ドメイン名を解決することができません。</translation>
    </message>
    <message>
        <location line="+43"/>
        <source>Mumble User</source>
        <translation>Mumble ユーザ</translation>
    </message>
    <message>
        <location filename="main.cpp" line="+425"/>
        <source>&lt;b&gt;Certificate Expiry:&lt;/b&gt; Your certificate is about to expire. You need to renew it, or you will no longer be able to connect to servers you are registered on.</source>
        <translation>&lt;b&gt;証明書の有効期限:&lt;/b&gt;あなたの証明書の有効期限がもうすぐ切れます。証明書を更新する必要があります。さもないとあなたはユーザ登録したサーバに接続できなくなるでしょう。</translation>
    </message>
</context>
<context>
    <name>Certificates</name>
    <message>
        <location filename="Cert.ui"/>
        <source>Certificate Management</source>
        <translation>証明書 管理</translation>
    </message>
    <message>
        <source>This opens a file selection dialog to choose a file to export the certificate to.</source>
        <translation type="obsolete">証明書をエクスポートするファイルを選択するためのダイアログを開きます。</translation>
    </message>
    <message>
        <location/>
        <source>Certificate Authentication</source>
        <translation>証明書による認証</translation>
    </message>
    <message>
        <location/>
        <source>Authenticating to servers without using passwords</source>
        <translation>パスワードを使わないサーバへのユーザ認証</translation>
    </message>
    <message>
        <source>&lt;p&gt;Mumble can use certificates to authenticate with servers. Using certificates avoids passwords, meaning you don&apos;t need to disclose any password to the remote site. It also enables very easy user registration.&lt;/p&gt;&lt;p&gt;While Mumble can work without certificates, the majority of servers will expect you to have one.&lt;/p&gt;</source>
        <translation type="obsolete">&lt;p&gt;Mumble はサーバと認証するために証明書を使います。証明書の使用することでパスワードの入力が不要になります。つまりパスワードを使用しないでサーバにアクセスできます。このことでとても簡単にユーザ登録ができるようになります。&lt;/p&gt;&lt;p&gt;Mumble は証明書なしでも動きますが、多くのサーバの管理者はあなたが証明書を持っていることを望んでいます。&lt;/p&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Current certificate</source>
        <translation>現在の証明書</translation>
    </message>
    <message>
        <location/>
        <source>This is the certificate Mumble currently uses.</source>
        <translation>Mumble が現在使用している証明書です。</translation>
    </message>
    <message>
        <location/>
        <source>Current Certificate</source>
        <translation>現在の証明書</translation>
    </message>
    <message>
        <location/>
        <source>Create a new certificate</source>
        <translation>新しい証明書を作る</translation>
    </message>
    <message>
        <location/>
        <source>This will create a new certificate.</source>
        <translation>新しい証明書を作ります。</translation>
    </message>
    <message>
        <location/>
        <source>Import certificate from file</source>
        <translation>ファイルから証明書をインポートします</translation>
    </message>
    <message>
        <location/>
        <source>This will import a certificate from file.</source>
        <translation>ファイルから証明書をインポートします。</translation>
    </message>
    <message>
        <location/>
        <source>Import a certificate</source>
        <translation>証明書をインポートする</translation>
    </message>
    <message>
        <location/>
        <source>Export Certificate</source>
        <translation>証明書をエクスポートする</translation>
    </message>
    <message>
        <location/>
        <source>This will export a certificate to file.</source>
        <translation>ファイルに証明書をエクスポートします。</translation>
    </message>
    <message>
        <location/>
        <source>Export current certificate</source>
        <translation>現在の証明書をエクスポートする</translation>
    </message>
    <message>
        <location/>
        <source>Import Certificate</source>
        <translation>証明書のインポートする</translation>
    </message>
    <message>
        <location/>
        <source>PKCS #12 Certificate import</source>
        <translation>PKCS #12 証明書のインポート</translation>
    </message>
    <message>
        <location/>
        <source>&lt;p&gt;Mumble can import certificates stored in PKCS #12 format. This is the format used when exporting a key from Mumble, and also when exporting keys from Firefox, Internet Explorer, Opera etc.&lt;/p&gt;&lt;p&gt;If the file is password protected, you will need the password to import the certificate.&lt;/p&gt;</source>
        <oldsource>&lt;p&gt;Mumble can import certificates stored in PKCS #12 format. This is the format used when exporting a key from Mumble, and also when exporting keys from FireFox, Internet Explorer, Opera etc.&lt;/p&gt;&lt;p&gt;If the file is password protected, you will need the password to import the certificate.&lt;/p&gt;</oldsource>
        <translation>&lt;p&gt;Mumble は PKCS #12 形式で保存された証明書をインポートできます。 これは Mumble がキーをエクスポートする時に使われる形式です。これは FireFox や Internet Explorer 、 Opera などでも使われます。&lt;/p&gt;&lt;p&gt;そのファイルがパスワードで保護されていたら、証明書をインポートするためにパスワードが必要になります。&lt;/p&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Import from</source>
        <translation>インポート元</translation>
    </message>
    <message>
        <location/>
        <source>Filename to import from</source>
        <translation>インポート元のファイル名</translation>
    </message>
    <message>
        <location/>
        <source>This is the filename you wish to import a certificate from.</source>
        <translation>これは証明書をインポートするファイル名です。</translation>
    </message>
    <message>
        <location/>
        <source>Select file to import from</source>
        <translation>インポート元となるファイルを選択</translation>
    </message>
    <message>
        <location/>
        <source>This opens a file selection dialog to choose a file to import a certificate from.</source>
        <translation>証明書のインポート元となるファイルを選択するためのダイアログを開きます。</translation>
    </message>
    <message>
        <location/>
        <source>Open...</source>
        <translation>開く....</translation>
    </message>
    <message>
        <location/>
        <source>Password</source>
        <translation>パスワード</translation>
    </message>
    <message>
        <location/>
        <source>Password for PKCS#12 file</source>
        <translation>PKCS#12 ファイルのためのパスワード</translation>
    </message>
    <message>
        <location/>
        <source>This is the password for the PKCS#12 file containing your certificate.</source>
        <translation>証明書を含むPKCS#12ファイルのためのパスワードです。</translation>
    </message>
    <message>
        <location/>
        <source>Certificate to import</source>
        <translation>インポートする証明書</translation>
    </message>
    <message>
        <location/>
        <source>This is the certificate you are importing.</source>
        <translation>これはあなたがインポートする証明書です。</translation>
    </message>
    <message>
        <location/>
        <source>Certificate Details</source>
        <translation>証明書の詳細</translation>
    </message>
    <message>
        <location/>
        <source>Replace Certificate</source>
        <translation>証明書の置き換え</translation>
    </message>
    <message>
        <location/>
        <source>Replace existing certificate with new certificate?</source>
        <translation>存在する証明書を新しい証明書に置き換えますか?</translation>
    </message>
    <message>
        <location/>
        <source>&lt;p&gt;You already have a certificate stored in Mumble, and you are about to replace it.&lt;/p&gt;
&lt;p&gt;If you are upgrading to a certificate issued to you by a trusted CA and the email addresses match your current certificate, this is completely safe, and servers you connect to will automatically recognize the strong certificate for your email address.
&lt;/p&gt;
&lt;p&gt;If this is not the case, you will no longer be recognized by any server you previously have authenticated with. If you haven&apos;t been registered on any server yet, this is nothing to worry about.
&lt;/p&gt;
&lt;p&gt;
Are you sure you wish to replace your certificate?
&lt;/p&gt;
</source>
        <translation>&lt;p&gt;あなたはすでに Mumbleに記憶された証明書を持っており、それを置き換えようとしています。&lt;/p&gt;
&lt;p&gt;信頼された認証局によって発行された証明書にアップグレードしようとしています。新しい証明書のEメールアドレスがあなたの現在の証明書のものと一致しているなら問題はありません。今までにユーザ登録をしたサーバは、あなたのEメールアドレスを元に強力な証明書を自動的に判別するでしょう。
&lt;/p&gt;
&lt;p&gt;そうでないなら、あなたはどのサーバでも以前に受けた認証を失います。あなたがまだ、どのサーバにもユーザ登録されていないなら、この心配は無用です。
&lt;/p&gt;
&lt;p&gt;
本当に証明書を置き換えますか？
&lt;/p&gt;</translation>
    </message>
    <message>
        <location/>
        <source>This is the certificate Mumble currently uses. It will be replaced.</source>
        <translation>Mumbleが現在使用している証明書です。これは新しい物に置き換えられます。</translation>
    </message>
    <message>
        <location/>
        <source>New certificate</source>
        <translation>新しい証明書</translation>
    </message>
    <message>
        <location/>
        <source>This is the new certificate that will replace the old one.</source>
        <translation>これは古い証明書と置き換えられる新しい証明書です。</translation>
    </message>
    <message>
        <location/>
        <source>New Certificate</source>
        <translation>新しい証明書</translation>
    </message>
    <message>
        <location/>
        <source>Make a backup of your certificate</source>
        <translation>証明書のバックアップを作成</translation>
    </message>
    <message>
        <source>&lt;p&gt;If you ever lose your current certificate, which will happen if your computer suffers a hardware failure or you reinstall your machine, you will no longer be able to authenticate to any server you are registered on. It is therefore &lt;b&gt;mandatory&lt;/b&gt; that you make a backup of your certificate. We strongly recommend you store this backup on removable storage, such as an USB memory stick.&lt;/p&gt;
&lt;p&gt;Note that this file will not be encrypted, and if anyone gains access to it, they will be able to impersonate you, so take good care of it.&lt;/p&gt;</source>
        <translation type="obsolete">&lt;p&gt;現在の証明書をハードウェアの故障やマシンの再インストールなどで失ったなら、登録したサーバから認証を受けることが出来なくなります。そのため、証明書のバックアップを作成することは&lt;b&gt;必須&lt;/b&gt;です。USBメモリのようなリムーバブルストレージに証明書のバックアップを保存することを強く推奨します。&lt;/p&gt;
&lt;p&gt;このファイルは暗号化されておらず、もし、誰かがアクセスしたら、あなたに成りすます可能性があります。十分に気をつけてください。&lt;/p&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Export to</source>
        <translation>エクスポート先</translation>
    </message>
    <message>
        <location/>
        <source>Filename to export to</source>
        <translation>エクスポート先のファイル名</translation>
    </message>
    <message>
        <location/>
        <source>This is the filename you wish to export a certificate to.</source>
        <translation>証明書をエクスポートするファイル名です。</translation>
    </message>
    <message>
        <location/>
        <source>Save As...</source>
        <translation>名前をつけて保存...</translation>
    </message>
    <message>
        <location/>
        <source>This is the certificate Mumble currently uses. It will be exported.</source>
        <translation>Mumbleが現在使用している証明書です。エクスポートされます。</translation>
    </message>
    <message>
        <location/>
        <source>Generate a new certificate for strong authentication</source>
        <translation>強力な認証のための新しい証明書を生成しましょう</translation>
    </message>
    <message>
        <location/>
        <source>&lt;p&gt;Mumble will now generate a strong certificate for authentication to servers.&lt;/p&gt;&lt;p&gt;If you wish, you may provide some additional information to be stored in the certificate, which will be presented to servers when you connect. If you provide a valid email address, you can upgrade to a CA issued email certificate later on, which provides strong identification.&lt;/p&gt;</source>
        <translation>&lt;p&gt;Mumble はサーバに対する認証のために強力な証明書を生成します。&lt;/p&gt;&lt;p&gt;証明書に追加情報を記録できます。これらの情報はサーバに接続する時にそのサーバに送信されます。正しいEメールアドレスを提供したなら、後で認証局が発行したEメール証明書にアップグレードできます。&lt;/p&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Name</source>
        <translation>名前</translation>
    </message>
    <message>
        <location/>
        <source>Email</source>
        <translation>Eメール</translation>
    </message>
    <message>
        <location/>
        <source>Your email address (e.g. johndoe@mumble.info)</source>
        <translation>あなたのEメールアドレス（例えば jhondoe@mumble.info)</translation>
    </message>
    <message>
        <location/>
        <source>This is your email address. It is strongly recommended to provide a valid email address, as this will allow you to upgrade to a strong certificate without authentication problems.</source>
        <translation>あなたのEメールアドレスです。正しいEメールアドレスを提供することが推奨されます。認証の問題なく強力な証明書にアップグレードできるからです。</translation>
    </message>
    <message>
        <location/>
        <source>Your name (e.g. John Doe)</source>
        <translation>あなたの名前です（例えば山田太郎）</translation>
    </message>
    <message>
        <location/>
        <source>This is your name, and will be filled out in the certificate. This field is entirely optional.</source>
        <translation>あなたの名前であり証明書に記入されれます。このフィールの入力は任意です。</translation>
    </message>
    <message>
        <location/>
        <source>Finish</source>
        <translation>終了</translation>
    </message>
    <message>
        <location/>
        <source>Certificate-based authentication is ready for use</source>
        <translation>証明書ベースの認証が使えるようになりました</translation>
    </message>
    <message>
        <location/>
        <source>Enjoy using Mumble with strong authentication.</source>
        <translation>強力な認証とともに Mumble を楽しんでください。</translation>
    </message>
    <message>
        <location/>
        <source>Automatic certificate creation</source>
        <translation>証明書の自動発行</translation>
    </message>
    <message>
        <location/>
        <source>&lt;p&gt;Mumble can use certificates to authenticate with servers. Using certificates avoids passwords, meaning you don&apos;t need to disclose any password to the remote site. It also enables very easy user registration.&lt;/p&gt;&lt;p&gt;While Mumble can work without certificates, the majority of servers will expect you to have one.&lt;/p&gt;
&lt;p&gt;
It is &lt;b&gt;strongly&lt;/b&gt; recommended that you &lt;a href=&quot;http://mumble.info/certificate.php&quot;&gt;create a trusted certificate&lt;/a&gt;.
&lt;/p&gt;
</source>
        <translation>&lt;p&gt;Mumbleではサーバへの認証に証明書を使えます。証明書を使うとパスワードが必要なくなります。これはパスワードをリモートサイトに公開する必要がないことを意味します。また、非常に簡単にユーザ登録できるようになります。&lt;/p&gt;
&lt;p&gt;Mumbleは証明書なしでも動きますが、多くのサーバはあなたが証明書を持っていることを期待しています。&lt;/p&gt;
&lt;p&gt;
&lt;a href=&quot;http://mumble.info/certificate.php&quot;&gt;信頼された証明書&lt;/a&gt;を作ることが&lt;b&gt;強く&lt;/b&gt; 推奨されます。
&lt;/p&gt;</translation>
    </message>
    <message>
        <location/>
        <source>&lt;p&gt;If you ever lose your current certificate, which will happen if your computer suffers a hardware failure or you reinstall your machine, you will no longer be able to authenticate to any server you are registered on. It is therefore &lt;b&gt;mandatory&lt;/b&gt; that you make a backup of your certificate. We strongly recommend you store this backup on removable storage, such as a USB flash drive.&lt;/p&gt;
&lt;p&gt;Note that this file will not be encrypted, and if anyone gains access to it, they will be able to impersonate you, so take good care of it.&lt;/p&gt;</source>
        <translation type="unfinished">&lt;p&gt;現在の証明書をハードウェアの故障やマシンの再インストールなどで失ったなら、登録したサーバから認証を受けることが出来なくなります。そのため、証明書のバックアップを作成することは&lt;b&gt;必須&lt;/b&gt;です。USBメモリのようなリムーバブルストレージに証明書のバックアップを保存することを強く推奨します。&lt;/p&gt;
&lt;p&gt;このファイルは暗号化されておらず、もし、誰かがアクセスしたら、あなたに成りすます可能性があります。十分に気をつけてください。&lt;/p&gt;</translation>
    </message>
</context>
<context>
    <name>ChanACL</name>
    <message>
        <source>W</source>
        <translation type="obsolete">W</translation>
    </message>
    <message>
        <source>T</source>
        <translation type="obsolete">T</translation>
    </message>
    <message>
        <source>E</source>
        <translation type="obsolete">E</translation>
    </message>
    <message>
        <source>S</source>
        <translation type="obsolete">S</translation>
    </message>
    <message>
        <source>A</source>
        <translation type="obsolete">A</translation>
    </message>
    <message>
        <source>M</source>
        <translation type="obsolete">M</translation>
    </message>
    <message>
        <source>K</source>
        <translation type="obsolete">K</translation>
    </message>
    <message>
        <source>C</source>
        <translation type="obsolete">C</translation>
    </message>
    <message>
        <source>L</source>
        <translation type="obsolete">L</translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="+214"/>
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
        <source>AltSpeak</source>
        <translation type="obsolete">Alt発言</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Mute/Deafen</source>
        <translation>発言禁止/聴取禁止</translation>
    </message>
    <message>
        <source>Move/Kick</source>
        <translation type="obsolete">移動/キック</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Make channel</source>
        <translation>チャンネル作成</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Make temporary</source>
        <translation>一時チャンネルの作成</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Link channel</source>
        <translation>チャンネルをリンク</translation>
    </message>
    <message>
        <location line="-87"/>
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
        <source>This represents the permission to whisper to this channel from the outside. This works exactly like the &lt;i&gt;speak&lt;/i&gt; privilege, but applies to packets spoken with the Whisper key held down. This may be used to broadcast to a hierarchy of channels without linking.</source>
        <translation>このチャンネルの外側から中へささやきを行う権限です。この権限は&lt;i&gt;発言&lt;/i&gt;の権限のように働きます。しかし、ささやきのキーが押されている発言パケットに適用されます。リンクしていないチャンネルの階層にブロードキャストするかもしれません。</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>This represents the permission to mute and deafen other users. Once muted, a user will stay muted until he is unmuted by another privileged user or reconnects to the server.</source>
        <translation>他のユーザを発言禁止にしたり、聴取禁止にしたりする権限を表します。一度発言禁止になると、他に権限を持ったユーザが発言禁止を解くか、サーバに接続しなおすまで発言禁止のままになります。</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>This represents the permission to move a user to another channel or kick him from the server. To actually move the user, either the moving user must have Move privileges in the destination channel, or the user must normally be allowed to enter the channel. Users with this privilege can move users into channels the target user normally wouldn&apos;t have permission to enter.</source>
        <translation>他の人を別のチャンネルに移動させたり、サーバからキックしたりする権限を表します。実際にユーザを動かすには、実行する人が移動先のチャンネルの移動/キック権限を持っているか、ユーザが移動先チャンネルに参加する権限を持っていなければなりません。この権限を持っている人は、対象のユーザが通常参加を許可されていないチャンネルに移動させることができます。</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>This represents the permission to make sub-channels. The user making the sub-channel will be added to the admin group of the sub-channel.</source>
        <translation>サブチャンネルを作成する権限を表します。サブチャンネルを作ったユーザは、そのチャンネルの管理者グループに追加されます。</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>This represents the permission to make a temporary subchannel. The user making the sub-channel will be added to the admin group of the sub-channel. Temporary channels are not stored and disappear when the last user leaves.</source>
        <translation>サブチャンネルとして一時チャンネルを作成する権限を表します。サブチャンネルを作ったユーザはそのチャンネルの admin グループに追加されます。一時チャンネルは、チャンネルからユーザがいなくなった場合に保持されずに消えます。</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>This represents the permission to link channels. Users in linked channels hear each other, as long as the speaking user has the &lt;i&gt;speak&lt;/i&gt; privilege in the channel of the listener. You need the link privilege in both channels to create a link, but just in either channel to remove it.</source>
        <translation>チャンネルをリンクする権限を表します。話している人が聞き手のチャンネルに&lt;i&gt;発言&lt;/i&gt;の権限を持っている限り、リンクされたチャンネルのユーザは互いに聞く事ができます。</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>This represents the permission to write text messages to other users in this channel.</source>
        <translation>このチャンネルで他のユーザにテキストメッセージを送る権限を表します。</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>This represents the permission to forcibly remove users from the server.</source>
        <translation>サーバからユーザを追い出す権限を表します。</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>This represents the permission to permanently remove users from the server.</source>
        <translation>サーバからユーザを永久的に追放する権限を表します。</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>This represents the permission to register and unregister users on the server.</source>
        <translation>サーバ上のユーザをユーザ登録したり登録を抹消する権利を表します。</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>This represents the permission to register oneself on the server.</source>
        <translation>サーバ上でユーザ自身がユーザ登録を行う権限を表します。</translation>
    </message>
    <message>
        <location line="+31"/>
        <source>Whisper</source>
        <translation>ささやき</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Move</source>
        <translation>ユーザの移動</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Text message</source>
        <translation>テキストメッセージ</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Kick</source>
        <translation>ユーザのキック</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Ban</source>
        <translation>ユーザのバン</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Register User</source>
        <translation>他ユーザを登録</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Register Self</source>
        <translation>自分をユーザ登録</translation>
    </message>
    <message>
        <source>This represents the permission to speak in a channel with flagged speech. This works exactly like the &lt;i&gt;speak&lt;/i&gt; privilege, but applies to packets spoken with AltPushToTalk held down. This may be used to broadcast to a hierarchy of channels without linking.</source>
        <translation type="obsolete">このチャンネル内でフラグ付きの発言をする権限を表します。これは&lt;i&gt;発言&lt;/i&gt;権限と全く同じ動作をしますが、Alt+ショートカットキーが押された時のパケットのみに適用されます。これはリンクしていないチャンネルの階層全体に送信をするために使うことが出来ます。</translation>
    </message>
    <message>
        <source>This represents the permission to mute and deafen other players. Once muted, a player will stay muted until he is unmuted by another privileged player or reconnects to the server.</source>
        <translation type="obsolete">他の人を発言禁止にしたり、聴取禁止にしたりする権限を表します。一度発言禁止になると、他に権限を持った人が発言禁止を解くか、サーバに接続しなおすまで発言禁止のままになります。</translation>
    </message>
    <message>
        <source>This represents the permission to move a player to another channel or kick him from the server. To actually move the player, either the moving player must have Move/Kick privileges in the destination channel, or the player must normally be allowed to enter the channel. Players with this privilege can move players into channels the target player normally wouldn&apos;t have permission to enter.</source>
        <translation type="obsolete">他の人を別のチャンネルに移動させたり、サーバからキックしたりする権限を表します。実際にプレイヤーを動かすには、実行する人が移動先のチャンネルの移動/キック権限を持っているか、プレイヤーが移動先チャンネルに入る権限を持っていなければなりません。この権限を持っている人は、対象のプレイヤーが通常参加を許可されていないチャンネルに移動させることができます。</translation>
    </message>
    <message>
        <source>This represents the permission to make sub-channels. The player making the sub-channel will be added to the admin group of the sub-channel.</source>
        <translation type="obsolete">サブチャンネルを作成する権限を表します。サブチャンネルを作った人は、そのチャンネルの管理者グループに追加されます。</translation>
    </message>
    <message>
        <source>This represents the permission to link channels. Players in linked channels hear each other, as long as the speaking player has the &lt;i&gt;speak&lt;/i&gt; privilege in the channel of the listener. You need the link privilege in both channels to create a link, but just in either channel to remove it.</source>
        <translation type="obsolete">チャンネルをリンクする権限を表します。話している人が聞き手のチャンネルに&lt;i&gt;発言&lt;/i&gt;の権限を持っている限り、リンクされたチャンネルのプレイヤーは互いに聞く事ができます。</translation>
    </message>
    <message>
        <location line="-28"/>
        <source>Write ACL</source>
        <translation>ACLの上書き</translation>
    </message>
</context>
<context>
    <name>ChatbarLineEdit</name>
    <message>
        <source>Paste and send</source>
        <translation type="obsolete">貼り付けて送信</translation>
    </message>
    <message>
        <source>&lt;center&gt;Type chat message here&lt;/center&gt;</source>
        <translation type="obsolete">&lt;center&gt;ここにメッセージを入力&lt;/center&gt;</translation>
    </message>
    <message>
        <source>Type chat message here</source>
        <translation type="obsolete">ここにメッセージを入力</translation>
    </message>
</context>
<context>
    <name>ChatbarTextEdit</name>
    <message>
        <location filename="CustomElements.cpp" line="+79"/>
        <source>Paste and send</source>
        <translation>貼り付けて送信</translation>
    </message>
    <message>
        <location line="+25"/>
        <source>&lt;center&gt;Type chat message here&lt;/center&gt;</source>
        <translation>&lt;center&gt;ここにメッセージを入力&lt;/center&gt;</translation>
    </message>
</context>
<context>
    <name>ClientPlayer</name>
    <message>
        <source>Authenticated</source>
        <translation type="obsolete">認証済み</translation>
    </message>
    <message>
        <source>Muted (server)</source>
        <translation type="obsolete">発言禁止 (server)</translation>
    </message>
    <message>
        <source>Deafened (server)</source>
        <translation type="obsolete">聴取禁止 (server)</translation>
    </message>
    <message>
        <source>Local Mute</source>
        <translation type="obsolete">ローカルでの発言禁止</translation>
    </message>
    <message>
        <source>Muted (self)</source>
        <translation type="obsolete">発言禁止(セルフ)</translation>
    </message>
    <message>
        <source>Deafened (self)</source>
        <translation type="obsolete">聴取禁止(セルフ)</translation>
    </message>
</context>
<context>
    <name>ClientUser</name>
    <message>
        <location filename="ClientUser.cpp" line="+130"/>
        <source>Friend</source>
        <translation>フレンド</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Authenticated</source>
        <translation>認証済み</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Priority speaker</source>
        <translation>優先度スピーカ</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Recording</source>
        <translation>録音</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Muted (server)</source>
        <translation>発言禁止 (server)</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Deafened (server)</source>
        <translation>聴取禁止 (サーバ)</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Local Mute</source>
        <translation>ローカルでの発言禁止</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Muted (self)</source>
        <translation>発言禁止 (セルフ)</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Deafened (self)</source>
        <translation>聴取禁止 (セルフ)</translation>
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
        <location/>
        <location filename="ConfigDialogDelegate.mm" line="+142"/>
        <source>Advanced</source>
        <translation>高度な設定を表示する</translation>
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
        <translation type="obsolete">&lt;b&gt;全ての設定項目を表示します。&lt;/b&gt;&lt;br /&gt;Mumbleは大部分のユーザが変更する必要のない設定項目を多く含んでいます。ここにチェックを入れると全ての設定項目を表示します。</translation>
    </message>
    <message>
        <source>Expert Config</source>
        <translation type="obsolete">上級者向け設定</translation>
    </message>
    <message>
        <source>Tab 1</source>
        <translation type="obsolete">Tab 1</translation>
    </message>
</context>
<context>
    <name>ConfigDialogMac</name>
    <message>
        <location filename="ConfigDialog_macx.mm" line="+70"/>
        <source>Accept changes</source>
        <translation>変更を確定</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This button will accept current settings and return to the application.&lt;br /&gt;The settings will be stored to disk when you leave the application.</source>
        <translation>このボタンは現在の設定を適用してアプリケーションに戻ります。&lt;br /&gt;この設定はアプリケーションを終了してもディスクに保存されます。</translation>
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
</context>
<context>
    <name>ConnectDialog</name>
    <message>
        <source>&amp;Name</source>
        <translation type="obsolete">名前(&amp;N)</translation>
    </message>
    <message>
        <source>A&amp;ddress</source>
        <translation type="obsolete">アドレス(&amp;D)</translation>
    </message>
    <message>
        <source>&amp;Port</source>
        <translation type="obsolete">ポート(&amp;P)</translation>
    </message>
    <message>
        <source>&amp;Username</source>
        <translation type="obsolete">ユーザ名(&amp;U)</translation>
    </message>
    <message>
        <source>&amp;Password</source>
        <translation type="obsolete">パスワード(&amp;P)</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="+757"/>
        <source>&amp;Connect</source>
        <translation>接続(&amp;C)</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">キャンセル</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation type="obsolete">追加(&amp;A)</translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation type="obsolete">削除(&amp;R)</translation>
    </message>
    <message>
        <source>&amp;Custom Servers</source>
        <translation type="obsolete">お気に入りサーバ(&amp;C)</translation>
    </message>
    <message>
        <source>Server &amp;Browser</source>
        <translation type="obsolete">サーバブラウザ(&amp;B)</translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="obsolete">名前</translation>
    </message>
    <message>
        <source>Address</source>
        <translation type="obsolete">アドレス</translation>
    </message>
    <message>
        <source>URL</source>
        <translation type="obsolete">URL</translation>
    </message>
    <message>
        <source>C&amp;opy to custom</source>
        <translation type="obsolete">お気に入りにコピー(&amp;O)</translation>
    </message>
    <message>
        <source>&amp;View Webpage</source>
        <translation type="obsolete">ウェブサイトを見る(&amp;V)</translation>
    </message>
    <message>
        <location line="+133"/>
        <source>Connecting to %1</source>
        <translation>%1に接続中</translation>
    </message>
    <message>
        <location line="-491"/>
        <location line="+491"/>
        <source>Enter username</source>
        <translation>ユーザ名を入力してください</translation>
    </message>
    <message>
        <location line="-491"/>
        <source>Adding host %1</source>
        <translation>ホスト %1 を追加中</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui"/>
        <location filename="ConnectDialog.cpp" line="+61"/>
        <source>Servername</source>
        <translation>サーバ名</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="+1"/>
        <source>Hostname</source>
        <translation>ホスト名</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Bonjour name</source>
        <translation>Bonjour 名</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Port</source>
        <translation>ポート</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Addresses</source>
        <translation>アドレス</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Website</source>
        <translation>WEBサイト</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Packet loss</source>
        <translation>パケット損失</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Ping (80%)</source>
        <translation></translation>
    </message>
    <message>
        <location line="+0"/>
        <location line="+2"/>
        <source>%1 ms</source>
        <translation></translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Ping (95%)</source>
        <translation></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Bandwidth</source>
        <translation>ネットワーク帯域</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>%1 kbit/s</source>
        <translation></translation>
    </message>
    <message>
        <location line="+315"/>
        <source>&amp;Filters</source>
        <translation>フィルタ(&amp;F)</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui"/>
        <location filename="ConnectDialog.cpp" line="-314"/>
        <source>Users</source>
        <translation>ユーザ</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="+1"/>
        <source>Version</source>
        <translation>バージョン</translation>
    </message>
    <message>
        <source>Connect</source>
        <translation type="obsolete">接続</translation>
    </message>
    <message>
        <source>Add New...</source>
        <translation type="obsolete">新しく追加 ...</translation>
    </message>
    <message>
        <source>Filters</source>
        <translation type="obsolete">フィルタ</translation>
    </message>
    <message>
        <source>Mumble</source>
        <translation type="obsolete">Mumble</translation>
    </message>
    <message>
        <location line="+1005"/>
        <source>Failed to fetch server list</source>
        <translation>サーバリストを取得出来ませんでした</translation>
    </message>
    <message>
        <source>&amp;Label</source>
        <translation type="obsolete">ラベル(&amp;L)</translation>
    </message>
    <message>
        <source>Label</source>
        <translation type="obsolete">ラベル</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation type="obsolete">不明</translation>
    </message>
    <message>
        <source>-Unnamed entry-</source>
        <translation type="obsolete">-Unnamed entry-</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui"/>
        <source>Mumble Server Connect</source>
        <translation>Mumbleサーバ接続</translation>
    </message>
    <message>
        <location/>
        <source>Ping</source>
        <translation></translation>
    </message>
    <message>
        <location/>
        <source>Remove from Favorites</source>
        <translation>お気に入りから削除する</translation>
    </message>
    <message>
        <source>Edit...</source>
        <translation type="obsolete">編集...</translation>
    </message>
    <message>
        <location/>
        <source>Add custom server</source>
        <translation>カスタムサーバを追加</translation>
    </message>
    <message>
        <source>Add to Favorites</source>
        <translation type="obsolete">お気に入りに追加</translation>
    </message>
    <message>
        <source>Open Webpage</source>
        <translation type="obsolete">Webページを開く</translation>
    </message>
    <message>
        <source>Show Reachable</source>
        <translation type="obsolete">到達可能なものを表示する</translation>
    </message>
    <message>
        <location/>
        <source>Show all servers that respond to ping</source>
        <translation>ping に応答する全てのサーバを表示します</translation>
    </message>
    <message>
        <source>Show Populated</source>
        <translation type="obsolete">ユーザのいるサーバを表示</translation>
    </message>
    <message>
        <location/>
        <source>Show all servers with users</source>
        <translation>ユーザがいる全てのサーバを表示します</translation>
    </message>
    <message>
        <source>Show All</source>
        <translation type="obsolete">全て表示</translation>
    </message>
    <message>
        <location/>
        <source>Show all servers</source>
        <translation>全てのサーバを表示</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Copy</source>
        <translation>コピー(&amp;C)</translation>
    </message>
    <message>
        <location/>
        <source>Copy favorite link to clipboard</source>
        <translation>クリップボードにお気に入りのリンクをコピー</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Paste</source>
        <translation>ペースト(&amp;P)</translation>
    </message>
    <message>
        <location/>
        <source>Paste favorite from clipboard</source>
        <translation>クリップボードからお気に入りへペースト</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">キャンセル(&amp;C)</translation>
    </message>
    <message>
        <source>New</source>
        <translation type="obsolete">新規</translation>
    </message>
    <message>
        <source>Add</source>
        <translation type="obsolete">追加</translation>
    </message>
    <message>
        <source>Update</source>
        <translation type="obsolete">更新</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Edit...</source>
        <translation>編集(&amp;E)...</translation>
    </message>
    <message>
        <location/>
        <location filename="ConnectDialog.cpp" line="-729"/>
        <source>&amp;Add New...</source>
        <translation>新しく追加(&amp;A) ...</translation>
    </message>
    <message>
        <location/>
        <source>Add to &amp;Favorites</source>
        <translation>お気に入りに追加(&amp;F)</translation>
    </message>
    <message>
        <location/>
        <source>Open &amp;Webpage</source>
        <translation>Webページを開く(&amp;W)</translation>
    </message>
    <message>
        <location/>
        <source>Show &amp;Reachable</source>
        <translation>到達可能なものを表示(&amp;R)</translation>
    </message>
    <message>
        <location/>
        <source>Show &amp;Populated</source>
        <translation>ユーザのいるサーバを表示(&amp;P)</translation>
    </message>
    <message>
        <location/>
        <source>Show &amp;All</source>
        <translation>全て表示(&amp;A)</translation>
    </message>
</context>
<context>
    <name>ConnectDialogEdit</name>
    <message>
        <location filename="ConnectDialogEdit.ui"/>
        <source>Edit Server</source>
        <translation>サーバを編集</translation>
    </message>
    <message>
        <location/>
        <source>Name of the server</source>
        <translation>サーバの名前</translation>
    </message>
    <message>
        <source>&lt;b&gt;Name&lt;/b&gt;&lt;br/&gt;
Name of the server. This is what the server will be named like in your serverlist and can be chosen freely.</source>
        <translation type="obsolete">&lt;b&gt;名前&lt;/b&gt;&lt;br/&gt;
サーバの名前。これはサーバリスト上でサーバの名前であり、自由に変更が可能です。</translation>
    </message>
    <message>
        <location/>
        <source>A&amp;ddress</source>
        <translation>アドレス(&amp;D)</translation>
    </message>
    <message>
        <location/>
        <source>Internet address of the server.</source>
        <oldsource>Internet address of the server. </oldsource>
        <translation>サーバのインターネットアドレス。</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Address&lt;/b&gt;&lt;br/&gt;
Internet address of the server. This can be a normal hostname, an IPv4/IPv6 address or a Bonjour service identifier. Bonjour service identifiers have to be prefixed with a &apos;@&apos; to be recognized by Mumble.</source>
        <oldsource>&lt;b&gt;Address&lt;/b&gt;&lt;/br&gt;
Internet address of the server. This can be a normal hostname, an ipv4/6 address or a bonjour service identifier. Bonjour service identifiers have to be prefixed with a &apos;@&apos; to be recognized by Mumble.</oldsource>
        <translation>&lt;b&gt;アドレス&lt;/b&gt;&lt;br/&gt;
サーバのインターネットアドレス。これは通常はホスト名・ip v4/6 アドレス、Bonjour サービス識別子です。Bonjourサービス識別子はMumbleに区別されるために &apos;@&apos; を接頭辞としてつける必要があります。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Port</source>
        <translation>ポート(&amp;P)</translation>
    </message>
    <message>
        <location/>
        <source>Port on which the server is listening</source>
        <translation>サーバが待ち受けているポート</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Port&lt;/b&gt;&lt;br/&gt;
Port on which the server is listening. If the server is identified by a Bonjour service identifier this field will be ignored.</source>
        <translation>&lt;b&gt;ポート&lt;/b&gt;&lt;br /&gt;
サーバが待ち受けるポート。サーバが Bonjour サービス識別子によって識別されるならこのフィールドは無視されます。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Username</source>
        <translation>ユーザ名(&amp;U)</translation>
    </message>
    <message>
        <location/>
        <source>Username to send to the server</source>
        <translation>サーバに送信するユーザ名</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Username&lt;/b&gt;&lt;br/&gt;
Username to send to the server. Be aware that the server can impose restrictions on how a username might look like. Also your username could already be taken by another user.</source>
        <translation>&lt;b&gt;ユーザ名&lt;/b&gt;&lt;br /&gt;
サーバに送信するユーザ名。サーバのユーザ名に対する制約に気をつけてください。また、ユーザ名が既に他のユーザに使われているかもしれないことにも注意してください。</translation>
    </message>
    <message>
        <location/>
        <source>Label</source>
        <translation>ラベル</translation>
    </message>
    <message>
        <source>&lt;b&gt;Label&lt;/b&gt;&lt;br/&gt;
Label of the server. This is what the server will be named like in your serverlist and can be chosen freely.</source>
        <translation type="obsolete">&lt;b&gt;ラベル&lt;/b&gt;&lt;br /&gt;
そのサーバのラベルです。あなたのサーバリスト上でどのようにそのサーバが名前をつけるもので
自由に選べます。</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="-75"/>
        <source>Add Server</source>
        <translation>サーバの追加</translation>
    </message>
    <message>
        <location filename="ConnectDialogEdit.ui"/>
        <source>Password</source>
        <translation>パスワード</translation>
    </message>
    <message>
        <location/>
        <source>Password to send to the server</source>
        <translation type="unfinished">サーバに送信するパスワード</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Password&lt;/b&gt;&lt;br/&gt;
Password to be sent to the server on connect. This password is needed when connecting as &lt;i&gt;SuperUser&lt;/i&gt; or to a server using password authentication. If not entered here the password will be queried on connect.</source>
        <translation type="unfinished">&lt;b&gt;Password&lt;/b&gt;&lt;br /&gt;
接続したサーバに送信されるパスワードです。このパスワードはSuperUserとして接続するときやパスワード認証を使うサーバに接続するとき必要です。パスワードをここに入力しなければ接続時に要求されます。</translation>
    </message>
    <message>
        <location/>
        <source>Show password</source>
        <translation type="unfinished">パスワードを表示する</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Label&lt;/b&gt;&lt;br/&gt;
Label of the server. This is what the server will be named like in your server list and can be chosen freely.</source>
        <translation type="unfinished">&lt;b&gt;ラベル&lt;/b&gt;&lt;br /&gt;
そのサーバのラベルです。あなたのサーバリスト上でどのようにそのサーバが名前をつけるもので
自由に選べます。</translation>
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
        <translatorcomment>ここは翻訳しないでください。翻訳すると日本語でエラーレポートを送って良いとユーザが誤解するかもしれません。</translatorcomment>
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
        <translation></translation>
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
        <location filename="Database.cpp" line="+84"/>
        <source>Mumble failed to initialize a database in any
of the possible locations.</source>
        <translation>Mumble: データベースの初期化が可能な場所がなく、
操作に失敗しました。
</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>The database &apos;%1&apos; is read-only. Mumble cannot store server settings (i.e. SSL certificates) until you fix this problem.</source>
        <oldsource>The database &apos;%1&apos; is read-only. Mumble can not store server settings (ie. SSL certificates) until you fix this problem.</oldsource>
        <translation type="unfinished">データベース &quot;%1&quot; は読み込み専用です。 この問題を修正するまで、Mumble はサーバの設定（例えばSSL証明書）を保存できません。</translation>
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
    <name>DockTitleBar</name>
    <message>
        <location filename="CustomElements.cpp" line="+135"/>
        <source>Drag here</source>
        <translation>ここにドラッグ</translation>
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
        <source>Data</source>
        <translation>データ</translation>
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
        <location filename="GlobalShortcut.cpp" line="+654"/>
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
        <location line="+38"/>
        <source>Shortcut button combination.</source>
        <translation>ショートカットボタンの組み合わせ。</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>&lt;b&gt;This is the global shortcut key combination.&lt;/b&gt;&lt;br /&gt;Click this field and then press the desired key/button combo to rebind. Double-click to clear.</source>
        <oldsource>&lt;b&gt;This is the global shortcut key combination.&lt;/b&gt;&lt;br /&gt;Double-click this field and then the desired key/button combo to rebind.</oldsource>
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
        <location filename="GlobalShortcut_macx.mm" line="+65"/>
        <source>Mumble has detected that it is unable to receive Global Shortcut events when it is in the background.&lt;br /&gt;&lt;br /&gt;This is because the Universal Access feature called &apos;Enable access for assistive devices&apos; is currently disabled.&lt;br /&gt;&lt;br /&gt;Please &lt;a href=&quot; &quot;&gt;enable this setting&lt;/a&gt; and continue when done.</source>
        <translation>バックグラウンドでグローバルなショートカットイベントが発生し、受け入れることができませんでした。&lt;br /&gt;&lt;br /&gt;&apos;補助デバイスへのアクセス&apos; のようなユニバーサルアクセス機能が無効になっているのが原因です。&lt;br /&gt;&lt;br /&gt;&lt;a href=&quot; &quot;&gt;この設定を有効にして&lt;/a&gt; その後、再び設定してください。 </translation>
    </message>
</context>
<context>
    <name>GlobalShortcutTarget</name>
    <message>
        <location filename="GlobalShortcutTarget.ui"/>
        <source>Whisper Target</source>
        <translation>ささやきのターゲット</translation>
    </message>
    <message>
        <location/>
        <source>Whisper to list of Users</source>
        <translation>ユーザのリストにささやき</translation>
    </message>
    <message>
        <location/>
        <source>Channel Target</source>
        <translation>チャンネルターゲット</translation>
    </message>
    <message>
        <location/>
        <source>Restrict to Group</source>
        <translation>グループの制限</translation>
    </message>
    <message>
        <location/>
        <source>If specified, only members of this group will receive the whisper.</source>
        <translation>設定すると、このグループのメンバーのみがささやきを受けとります。</translation>
    </message>
    <message>
        <source>If checked the whisper will also be transmitted to linked channels.</source>
        <translation type="obsolete">チェックすると、ささやきがリンクしているチャンネルにも転送されます。</translation>
    </message>
    <message>
        <source>Whisper to Linked channels</source>
        <translation type="obsolete">リンク済みのチャンネルにささやき</translation>
    </message>
    <message>
        <source>If checked this whisper will also be sent to the subchannels of the channel target.</source>
        <translation type="obsolete">チェックすると対象チャンネルのサブチャンネルにもささやきが送信されます。</translation>
    </message>
    <message>
        <location/>
        <source>List of users</source>
        <translation>ユーザのリスト</translation>
    </message>
    <message>
        <location/>
        <source>Add</source>
        <translation>追加</translation>
    </message>
    <message>
        <location/>
        <source>Remove</source>
        <translation>削除</translation>
    </message>
    <message>
        <location/>
        <source>Modifiers</source>
        <translation>変更</translation>
    </message>
    <message>
        <location/>
        <source>Do not send positional audio information when using this whisper shortcut.</source>
        <translation>このささやきのショートカットを使用して位置音響の情報を送信しません。</translation>
    </message>
    <message>
        <location/>
        <source>Ignore positional audio</source>
        <translation>位置音響を無効にする</translation>
    </message>
    <message>
        <location/>
        <source>Shout to Linked channels</source>
        <translation>リンクしているチャンネルへ叫ぶ</translation>
    </message>
    <message>
        <location/>
        <source>Shout to subchannels</source>
        <translation>サブチャンネルへ叫ぶ</translation>
    </message>
    <message>
        <location/>
        <source>Shout to Channel</source>
        <translation>チャンネルへ叫ぶ</translation>
    </message>
    <message>
        <location/>
        <source>The whisper will also be transmitted to linked channels.</source>
        <translation type="unfinished">ささやきはリンクしているチャンネルにも送信されます。</translation>
    </message>
    <message>
        <location/>
        <source>The whisper will also be sent to the subchannels of the channel target.</source>
        <translation type="unfinished">ささやきは対象のチャンネルのサブチャンネルにも送信されます。</translation>
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
        <location filename="LCD.cpp" line="+279"/>
        <source>Not connected</source>
        <translation>接続されていません</translation>
    </message>
</context>
<context>
    <name>LCDConfig</name>
    <message>
        <source>Graphic</source>
        <translation type="obsolete">画像</translation>
    </message>
    <message>
        <source>Character</source>
        <translation type="obsolete">キャラクタ</translation>
    </message>
    <message>
        <location line="-160"/>
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
        <source>Devices</source>
        <translation>デバイス</translation>
    </message>
    <message>
        <location/>
        <source>Name</source>
        <translation>名前</translation>
    </message>
    <message>
        <source>Type</source>
        <translation type="obsolete">タイプ</translation>
    </message>
    <message>
        <location/>
        <source>&lt;p&gt;This is the list of available LCD devices on your system.  It lists devices by name, but also includes the size of the display. Mumble supports outputting to several LCD devices at a time.&lt;/p&gt;
&lt;h3&gt;Size:&lt;/h3&gt;
&lt;p&gt;
This field describes the size of an LCD device. The size is given either in pixels (for Graphic LCDs) or in characters (for Character LCDs).&lt;/p&gt;
&lt;h3&gt;Enabled:&lt;/h3&gt;
&lt;p&gt;This decides whether Mumble should draw to a particular LCD device.&lt;/p&gt;</source>
        <translation>&lt;p&gt;これはあなたのシステム上にある利用可能なLCDデバイスのリストです。名前でデバイスを列挙しますが、ディスプレイのサイズも含んでいます。Mumbleは複数のLCDデバイスへ同時に出力することをサポートしています。&lt;/p&gt;
&lt;h3&gt;サイズ:&lt;/h3&gt;
&lt;p&gt;
このフィールドはLCDデバイスのサイズを示します。そのサイズはピクセル（LCDの画像）か文字数（LCDの文字）で与えられます。&lt;/p&gt;
&lt;h3&gt;有効化:&lt;/h3&gt;
&lt;p&gt;これはMumbleが特定のLCDデバイスに描画するべきかを決定します。&lt;/p&gt;</translation>
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
        <source>&lt;p&gt;This option decides the minimum width a column in the User View.&lt;/p&gt;
&lt;p&gt;If too many people are speaking at once, the User View will split itself into columns. You can use this option to pick a compromise between number of users shown on the LCD, and width of user names.&lt;/p&gt;
</source>
        <translation>&lt;p&gt;このオプションはユーザビューの中で最小の幅の列を決めます。&lt;/p&gt;
&lt;p&gt;あまりにも多い人々が同時に喋ったとき、ユーザビューはそれを列に分けます。あなたはLCD上に表示されるユーザの数とユーザ名の幅の間の妥協案を得るためにこのオプションを使えます。&lt;/p&gt;
</translation>
    </message>
    <message>
        <location/>
        <source>This setting decides the width of column splitter.</source>
        <translation>この設定は列の分割の幅を決めます。</translation>
    </message>
    <message>
        <source>TextLabel</source>
        <translation type="obsolete">テキストラベル</translation>
    </message>
    <message>
        <location/>
        <source>Splitter Width</source>
        <translation>分割線の幅</translation>
    </message>
</context>
<context>
    <name>Log</name>
    <message>
        <location filename="Log.cpp" line="+220"/>
        <source>Debug</source>
        <translation>デバッグ</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Critical</source>
        <translation>致命的</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Warning</source>
        <translation>警告</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Information</source>
        <translation>情報</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Server Connected</source>
        <translation>サーバに接続</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Server Disconnected</source>
        <translation>サーバから切断</translation>
    </message>
    <message>
        <source>Player Joined Server</source>
        <translation type="obsolete">誰かがサーバに参加</translation>
    </message>
    <message>
        <source>Player Left Server</source>
        <translation type="obsolete">誰かがサーバから退出</translation>
    </message>
    <message>
        <source>Player kicked (you or by you)</source>
        <translation type="obsolete">誰かがキックされた時(あなた自身、またはあなたによって)</translation>
    </message>
    <message>
        <source>Player kicked</source>
        <translation type="obsolete">誰かがキックされた時</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>You self-muted/deafened</source>
        <translation>自分を発言禁止/聴取禁止にした時</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Other self-muted/deafened</source>
        <translation>他の人が発言禁止/聴取禁止になった時</translation>
    </message>
    <message>
        <source>Player muted (you)</source>
        <translation type="obsolete">誰かがあなたを発言禁止にした時</translation>
    </message>
    <message>
        <source>Player muted (by you)</source>
        <translation type="obsolete">(あなたによって)誰かが発言禁止になった時</translation>
    </message>
    <message>
        <source>Player muted (other)</source>
        <translation type="obsolete">(他の人によって)誰かが発言禁止になった時</translation>
    </message>
    <message>
        <source>Player Joined Channel</source>
        <translation type="obsolete">誰かがチャンネルに参加</translation>
    </message>
    <message>
        <source>Player Left Channel</source>
        <translation type="obsolete">誰かがチャンネルから退出</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Permission Denied</source>
        <translation>権限がない場合</translation>
    </message>
    <message>
        <source>[%2] %1</source>
        <translation type="obsolete">[%2] %1</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Text Message</source>
        <translation>テキストメッセージ</translation>
    </message>
    <message>
        <location line="-13"/>
        <source>User Joined Server</source>
        <translation>ユーザがサーバに参加</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>User Left Server</source>
        <translation>ユーザがサーバから退出</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>User recording state changed</source>
        <translation>ユーザの録音状態の変更</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>User kicked (you or by you)</source>
        <translation>ユーザがキックされた時 (あなた自身、またはあなたによって)</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>User kicked</source>
        <translation>ユーザがキックされた時</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>User muted (you)</source>
        <translation>ユーザが発言禁止になった時 （あなたが）</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>User muted (by you)</source>
        <translation>ユーザが発言禁止になった時 （あなたによって)</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>User muted (other)</source>
        <translation>ユーザが発言禁止になった時 （その他）</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>User Joined Channel</source>
        <translation>ユーザがチャンネルに参加</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>User Left Channel</source>
        <translation>ユーザがチャンネルから退出</translation>
    </message>
    <message>
        <location line="+54"/>
        <source>the server</source>
        <translation>サーバ</translation>
    </message>
    <message>
        <location line="+117"/>
        <source>[[ Text object too large to display ]]</source>
        <translation>[[ テキストが表示するには大きすぎます ]]</translation>
    </message>
    <message>
        <location line="+42"/>
        <source>[Date changed to %1]
</source>
        <translation>[ 日付が %1 に変更されます ]</translation>
    </message>
    <message>
        <location line="+142"/>
        <source>link to %1</source>
        <translation>%1 へのリンク</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>ftp link to %1</source>
        <translation>%1 へのftpリンク</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>player link</source>
        <translation>プレイヤリンク</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>channel link</source>
        <translation>チャンネルリンク</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1 link</source>
        <translation>%1 リンク</translation>
    </message>
</context>
<context>
    <name>LogConfig</name>
    <message>
        <location line="-521"/>
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
        <source>Path to sound file used for sound notifications in the case of %1 events&lt;br /&gt;Single click to play&lt;br /&gt;Double-click to change</source>
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
        <location line="+3"/>
        <source>Path to sound file used for sound notifications in the case of %1 events.&lt;br /&gt;Single click to play&lt;br /&gt;Double-click to change&lt;br /&gt;Ensure that sound notifications for these events are enabled or this field will not have any effect.</source>
        <translation>イベント「%1」のときに音声通知のために使用するサウンドファイルのパス&lt;br /&gt;シングルクリックで再生&lt;br /&gt;ダブルクリックで変更&lt;br /&gt;これらのイベントのための音声通知が有効であるか、またはこのフィールドが無効であることを確認してください。</translation>
    </message>
    <message>
        <source>Click here to toggle Text-To-Speech for %1 events.&lt;br /&gt;If checked, Mumble uses Text-To-Speech to read %1 events out loud to you. Text-To-Speech is also able to read the contents of the event which is not true for soundfiles. Text-To-Speech and soundfiles cannot be used at the same time.</source>
        <oldsource>Click here to toggle Text-To-Speech for %1 events.&lt;br /&gt;If checked, mumble uses Text-To-Speech to read %1 events out loud to you. Text-To-Speech is also able to read the contents of the event which is not true for soundfiles. Text-To-Speech and soundfiles cannot be used at the same time.</oldsource>
        <translation type="obsolete">イベント「%1」の適すと読み上げを切り替えるためにはここをクリック。&lt;br /&gt;チェックされた場合、イベント「%1」が読み上げられる。テキスト読み上げはサウンドファイルではでいない、イベントの内容を読み上げがが可能である。テキスト読み上げとサウンドファイルは同時に使用することはできない。</translation>
    </message>
    <message>
        <location line="-2"/>
        <source>Click here to toggle Text-To-Speech for %1 events.&lt;br /&gt;If checked, Mumble uses Text-To-Speech to read %1 events out loud to you. Text-To-Speech is also able to read the contents of the event which is not true for sound files. Text-To-Speech and sound files cannot be used at the same time.</source>
        <oldsource>Click here to toggle sound notification for %1 events.&lt;br /&gt;If checked, Mumble uses a soundfile predefined by you to indicate %1 events. Soundfiles and Text-To-Speech cannot be used at the same time.</oldsource>
        <translation>イベント「%1」の音声通知を切り替えるためにはここをクリック。&lt;br /&gt;チェックされた場合、あなたが用意したイベント「%1」を示す音声ファイルが使用さる。サウンドファイルと適すと読み上げは同時に使用することはできない。</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Click here to toggle sound notification for %1 events.&lt;br /&gt;If checked, Mumble uses a sound file predefined by you to indicate %1 events. Sound files and Text-To-Speech cannot be used at the same time.</source>
        <oldsource>Path to soundfile used for sound notifications in the case of %1 events.&lt;br /&gt;Single click to play&lt;br /&gt;Doubleclick to change&lt;br /&gt;Be sure that sound notifications for these events are enabled or this field will not have any effect.</oldsource>
        <translation>イベント「%1」のときに音声通知を切り替えるにはここをクリックしてください。&lt;br /&gt; もしチェックしたなら、Mumble はイベント「%1」に設定された音声ファイルを再生します。音声ファイルとTTSは同時に使えません。</translation>
    </message>
    <message>
        <source>Choose sound file</source>
        <translation type="obsolete">サウンドファイルを選択してください</translation>
    </message>
    <message>
        <source>Invalid sound file</source>
        <translation type="obsolete">不正なサウンドファイル</translation>
    </message>
    <message>
        <source>The file &apos;%1&apos; does not exist or is not a valid speex file.</source>
        <translation type="obsolete">ファイル「%1」は存在しないか、適切な speex ファイルではありません。</translation>
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
        <source>Whisper</source>
        <translation>ささやき</translation>
    </message>
    <message>
        <location/>
        <source>If checked you will only hear whispers from users you added to your friend list.</source>
        <translation>チェックするとフレンドリストに追加されたユーザからのみささやきのみを聞くようになります。</translation>
    </message>
    <message>
        <location/>
        <source>Only accept whispers from friends</source>
        <translation>フレンドからのささやきのみ受け付ける</translation>
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
        <location filename="LookConfig.cpp" line="+52"/>
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
        <source>Layout</source>
        <translation>レイアウト</translation>
    </message>
    <message>
        <location/>
        <source>Classic</source>
        <translation>クラシック</translation>
    </message>
    <message>
        <location/>
        <source>Stacked</source>
        <translation>スタック</translation>
    </message>
    <message>
        <location/>
        <source>Hybrid</source>
        <translation>ハイブリッド</translation>
    </message>
    <message>
        <location/>
        <source>Custom</source>
        <translation>カスタム</translation>
    </message>
    <message>
        <location/>
        <source>This changes the behavior when moving channels.</source>
        <translation>チャンネルを移動したときの動作を変更します。</translation>
    </message>
    <message>
        <location/>
        <source>This sets the behavior of channel drags; it can be used to prevent accidental dragging. &lt;i&gt;Move&lt;/i&gt; moves the channel without prompting. &lt;i&gt;Do Nothing&lt;/i&gt; does nothing and prints an error message. &lt;i&gt;Ask&lt;/i&gt; uses a message box to confirm if you really wanted to move the channel.</source>
        <oldsource>This sets the behavior of channel drags; it can be used to prevent accidental dragging. &lt;i&gt;Move Channel&lt;/i&gt; moves the channel without prompting. &lt;i&gt;Do Nothing&lt;/i&gt; does nothing and prints an error message. &lt;i&gt;Ask&lt;/i&gt; uses a message box to confirm if you really wanted to move the channel.</oldsource>
        <translation>チャンネルを動かすときの動作を変更します。これを設定すると誤ってチャンネルをドラッグしてしまうのを防ぐことができます。 &lt;i&gt;チャンネルを移動&lt;/i&gt; 確認なしにチャンネルを動かします。 &lt;i&gt;何もしない&lt;/i&gt; 何も行わず、エラーメッセージを表示します。 &lt;i&gt;確認する&lt;/i&gt; 確認メッセージボックスを出してから動かします。</translation>
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
        <source>...</source>
        <translation type="obsolete">...</translation>
    </message>
    <message>
        <source>Check to show chat bar</source>
        <translation type="obsolete">チャットバーを表示するにはチェック</translation>
    </message>
    <message>
        <source>&lt;b&gt;If checked the chat bar is shown.&lt;/b&gt;&lt;br /&gt;Uncheck this to hide it.</source>
        <oldsource>&lt;b&gt;If checked the chat bar is shown&lt;/b&gt;&lt;br /&gt;Uncheck to hide it</oldsource>
        <translation type="obsolete">&lt;b&gt;チェックしたらチャットバーが現れます&lt;/b&gt;&lt;br /&gt;チェックをはずすと隠す。</translation>
    </message>
    <message>
        <source>Show chatbar</source>
        <translation type="obsolete">チャットバーを表示する</translation>
    </message>
    <message>
        <location/>
        <source>Show number of users in each channel</source>
        <translation>それぞれのチャンネルのユーザ数を表示</translation>
    </message>
    <message>
        <location/>
        <source>Show channel user count</source>
        <translation>チャンネルにいるユーザ数を表示する</translation>
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
        <location line="+130"/>
        <source>Choose skin file</source>
        <translation>スキンファイルを選択</translation>
    </message>
    <message>
        <source>&lt;b&gt;If set, players will be shown above subchannels in the channel view.&lt;/b&gt;&lt;br /&gt;A restart of Mumble is required to see the change.</source>
        <translation type="obsolete">&lt;b&gt;これを設定していると、チャンネル表示の時にサブチャンネルより上にプレイヤーが表示されるようになります。&lt;/b&gt;&lt;br /&gt;この設定を反映するにはMumbleの再起動が必要です。</translation>
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
        <source>Players above Channels</source>
        <translation type="obsolete">チャンネルより上にプレイヤーを表示</translation>
    </message>
    <message>
        <source>List players above subchannels (requires restart).</source>
        <translation type="obsolete">サブチャンネルより上にプレイヤーが表示されるように並べます。(再起動が必要)</translation>
    </message>
    <message>
        <location filename="LookConfig.ui"/>
        <source>&lt;b&gt;This sets which skin Mumble should use.&lt;/b&gt;&lt;br /&gt;The skin is a style file applied on top of the basic widget style. If there are icons in the same directory as the style sheet, those will replace the default icons.</source>
        <translation>&lt;b&gt;Mumbleで使用するスキンを設定します。&lt;/b&gt;&lt;br /&gt;スキンは基本的なウィジェットスタイルで用いられるスタイルファイルです。アイコンがスタイルシートと同じディレクトリにあるなら、デフォルトのアイコンを置き換えます。</translation>
    </message>
    <message>
        <source>Form</source>
        <translation type="obsolete">フォーム</translation>
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
        <location filename="LookConfig.cpp" line="-140"/>
        <source>None</source>
        <translation>なし</translation>
    </message>
    <message>
        <source>Only with players</source>
        <translation type="obsolete">人が居るチャンネルのみ</translation>
    </message>
    <message>
        <location line="+2"/>
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
        <source>This sets which channels to automatically expand. &lt;i&gt;None&lt;/i&gt; and &lt;i&gt;All&lt;/i&gt; will expand no or all channels, while &lt;i&gt;Only with players&lt;/i&gt; will expand and collapse channels as players join and leave them.</source>
        <translation type="obsolete">どのチャンネルを自動的に展開するかを決定します。 &lt;i&gt;なし&lt;/i&gt; と &lt;i&gt;全て&lt;/i&gt; は全てのチャンネルを展開する/しない設定にします。&lt;i&gt;人が居るチャンネルのみ&lt;/i&gt; はプレイヤーの入室状況に応じて展開と折りたたみが変わります。</translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="-1"/>
        <source>Only with users</source>
        <translation>ユーザが居るチャンネルのみ</translation>
    </message>
    <message>
        <location line="+3"/>
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
        <translation>移動する</translation>
    </message>
    <message>
        <location filename="LookConfig.ui"/>
        <source>Channel Dragging</source>
        <translation>チャンネルのドラッグ操作</translation>
    </message>
    <message>
        <source>This changes the behaviour when moving channels.</source>
        <translation type="obsolete">チャンネルを動かすときのふるまいを変更します。</translation>
    </message>
    <message>
        <source>This sets the behaviour of channel drags, it can be used to prevent accidental dragging. &lt;i&gt;Move Channel&lt;/i&gt; moves the channel without prompting. &lt;i&gt;Do Nothing&lt;/i&gt; does nothing and prints an error message. &lt;i&gt;Ask&lt;/i&gt; uses a message box to confirm if you really wanted to move the channel.</source>
        <translation type="obsolete">チャンネルを動かすときのふるまいを変更します。これを設定すると誤ってチャンネルをドラッグしてしまうのを防ぐことができます。 &lt;i&gt;チャンネルを移動&lt;/i&gt; 確認なしにチャンネルを動かします。 &lt;i&gt;何もしない&lt;/i&gt; 何も行わず、エラーメッセージを表示します。 &lt;i&gt;確認する&lt;/i&gt; 確認メッセージボックスを出してから動かします。</translation>
    </message>
    <message>
        <location/>
        <source>Ask whether to close or minimize when quitting Mumble.</source>
        <translation>終了時に最小化するか閉じるかを確認します。</translation>
    </message>
    <message>
        <source>&lt;b&gt;If set, will verify you want to quit if connected.&lt;b&gt;</source>
        <translation type="obsolete">&lt;b&gt;これがセットされている場合、接続中に終了するかを確認します。&lt;/b&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Ask on quit while connected</source>
        <translation>接続中の終了は確認する</translation>
    </message>
    <message>
        <source>Make the Mumble window appear on top of other windows.</source>
        <translation type="obsolete">最前面に表示する</translation>
    </message>
    <message>
        <source>&lt;b&gt;This makes the Mumble window a topmost window.&lt;/b&gt;</source>
        <translation type="obsolete">&lt;b&gt;Mumbleのウインドウを最前面に表示します。&lt;/b&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Always On Top</source>
        <translation>常に手前に表示</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;If set, minimizing the Mumble main window will cause it to be hidden and accessible only from the tray. Otherwise, it will be minimized as a window normally would.&lt;/b&gt;</source>
        <translation>&lt;b&gt;これがセットされていた場合、最小化されたMumbleのメインウインドウは非表示になり、タスクトレイからのみアクセスできるようになります。これがセットされていない場合、ウインドウは普通に最小化します。&lt;/b&gt;</translation>
    </message>
    <message>
        <location/>
        <source>This sets which channels to automatically expand. &lt;i&gt;None&lt;/i&gt; and &lt;i&gt;All&lt;/i&gt; will expand no or all channels, while &lt;i&gt;Only with users&lt;/i&gt; will expand and collapse channels as users join and leave them.</source>
        <translation>どのチャンネルを自動的に展開するかを決定します。 &lt;i&gt;なし&lt;/i&gt; と &lt;i&gt;全て&lt;/i&gt; は全てのチャンネルを展開する/しない設定にします。&lt;i&gt;人が居るチャンネルのみ&lt;/i&gt; はユーザの入室状況に応じて展開と折りたたみが変わります。</translation>
    </message>
    <message>
        <location/>
        <source>List users above subchannels (requires restart).</source>
        <translation>サブチャンネルの上にユーザを表示する（再起動が必要です）。</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;If set, users will be shown above subchannels in the channel view.&lt;/b&gt;&lt;br /&gt;A restart of Mumble is required to see the change.</source>
        <translation>&lt;b&gt;これを設定していると、チャンネル表示の時にサブチャンネルより上にユーザが表示されるようになります。&lt;/b&gt;&lt;br /&gt;この設定を反映するにはMumbleの再起動が必要です。</translation>
    </message>
    <message>
        <location/>
        <source>Users above Channels</source>
        <translation>チャンネルの上にユーザを表示する</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;If set, will verify you want to quit if connected.&lt;/b&gt;</source>
        <translation>&lt;b&gt;これがセットされている場合、接続中に終了するかを確認します。&lt;/b&gt;</translation>
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
        <translation type="obsolete">&lt;b&gt;全ての設定項目を表示します。&lt;/b&gt;&lt;br /&gt;Mumbleは大部分のユーザが変更する必要のない設定項目を多く含んでいます。ここにチェックを入れると全ての設定項目を表示します。</translation>
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
    <message>
        <location/>
        <source>This setting controls when the application will be always on top.</source>
        <translation>この設定はどのような状況でアプリケーションを常に手前に表示するかを制御します。</translation>
    </message>
    <message>
        <source>This setting controls in which situations the application will stay always on top. If you select &lt;i&gt;Never&lt;/i&gt; the application will not stay on top. &lt;i&gt;Always&lt;/i&gt; will always keep the application on top. &lt;i&gt;In minimal view&lt;/i&gt; / &lt;i&gt;In normal mode&lt;/i&gt; will only keep the application always on top when minimal view is activated / deactivated.</source>
        <translation type="obsolete">この設定はどのような状況でアプリケーションを常に手前で表示するかを制御します。</translation>
    </message>
    <message>
        <location/>
        <source>Never</source>
        <translation>手前に表示しない</translation>
    </message>
    <message>
        <location/>
        <source>Always</source>
        <translation>常に手前に表示する</translation>
    </message>
    <message>
        <location/>
        <source>In minimal view</source>
        <translation>小さい表示でのみ常に手前で表示する</translation>
    </message>
    <message>
        <location/>
        <source>In normal view</source>
        <translation>通常の表示でのみ常に手前で表示する</translation>
    </message>
    <message>
        <location/>
        <source>Displays talking status in system tray</source>
        <translation>システムトレイに発現状態を表示する</translation>
    </message>
    <message>
        <location/>
        <source>Show talking status in tray icon</source>
        <translation>発言状態をトレイアイコンで表示する</translation>
    </message>
    <message>
        <location/>
        <source>This setting controls in which situations the application will stay always on top. If you select &lt;i&gt;Never&lt;/i&gt; the application will not stay on top. &lt;i&gt;Always&lt;/i&gt; will always keep the application on top. &lt;i&gt;In minimal view&lt;/i&gt; / &lt;i&gt;In normal view&lt;/i&gt; will only keep the application always on top when minimal view is activated / deactivated.</source>
        <translation>この設定はどのような状況でアプリケーションを常に手前で表示するかを制御します。</translation>
    </message>
    <message>
        <location/>
        <source>Show context menu in menu bar</source>
        <translation>メニューバーにコンテキストメニューを表示する</translation>
    </message>
    <message>
        <location/>
        <source>Apply some high contrast optimizations for visually impaired users</source>
        <translation>視覚障害者のためにハイコントラストへの最適化を適用します</translation>
    </message>
    <message>
        <location/>
        <source>Optimize for high contrast</source>
        <translation>ハイコントラストに最適化する</translation>
    </message>
    <message>
        <location/>
        <source>Application</source>
        <translation>アプリケーション</translation>
    </message>
    <message>
        <location/>
        <source>Adds user and channel context menus into the menu bar</source>
        <translation>ユーザとチャンネルのコンテキストメニューをメニューバーに追加します</translation>
    </message>
    <message>
        <location/>
        <source>Tray Icon</source>
        <translation>トレイアイコン</translation>
    </message>
    <message>
        <location/>
        <source>Channel Tree</source>
        <translation>チャンネルツリー</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Browse...</source>
        <translation>参照(&amp;B)...</translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <location filename="MainWindow.cpp" line="+171"/>
        <location line="+2048"/>
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
        <source>Shows a dialog of registered servers, and also allows quick connect.</source>
        <translation type="obsolete">登録されたサーバのダイアログを開きます。クイック接続もできるようになります。</translation>
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
        <source>Kick player (with reason)</source>
        <translation type="obsolete">プレイヤーをキック(理由付き)</translation>
    </message>
    <message>
        <source>Kick selected player off server. You&apos;ll be asked to specify a reason.</source>
        <translation type="obsolete">選択されたプレイヤーをサーバから追い出します。あなたは、理由を尋ねられるでしょう。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Ban</source>
        <translation>&amp;BAN</translation>
    </message>
    <message>
        <source>Kick and ban player (with reason)</source>
        <translation type="obsolete">キックしてBAN(理由付き)</translation>
    </message>
    <message>
        <source>Kick and ban selected player from server. You&apos;ll be asked to specify a reason.</source>
        <translation type="obsolete">選択されたプレイヤーをサーバからキックしてBANします。あなたは、理由を尋ねられるでしょう。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Mute</source>
        <translation>発言禁止(&amp;M)</translation>
    </message>
    <message>
        <source>Mute player</source>
        <translation type="obsolete">プレイヤーを発言禁止にする</translation>
    </message>
    <message>
        <source>Mute or unmute player on server. Unmuting a deafened player will also undeafen them.</source>
        <translation type="obsolete">サーバにいるプレイヤーを発言禁止にしたり、解除したりします。聴取禁止になっているプレイヤーに対して発言禁止の解除を行うと、聴取禁止も解除されます。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Deafen</source>
        <translation>聴取禁止(&amp;D)</translation>
    </message>
    <message>
        <source>Deafen player</source>
        <translation type="obsolete">プレイヤーを聴取禁止にする</translation>
    </message>
    <message>
        <source>Deafen or undeafen player on server. Deafening a player will also mute them.</source>
        <translation type="obsolete">サーバにいるプレイヤーを聴取禁止にしたり、解除したりします。聴取禁止にすると発言も禁止にします。</translation>
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
        <source>&amp;Edit ACL</source>
        <translation type="obsolete">ACLを編集(&amp;E)</translation>
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
        <source>&amp;Rename</source>
        <translation type="obsolete">チャンネル名を変更する</translation>
    </message>
    <message>
        <source>&amp;Change Description</source>
        <translation type="obsolete">説明を変更する(&amp;C)</translation>
    </message>
    <message>
        <source>Changes the channel description</source>
        <translation type="obsolete">チャンネルの説明を変更する</translation>
    </message>
    <message>
        <source>This changes the description of a channel.</source>
        <translation type="obsolete">チャンネルの説明を変更する</translation>
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
        <source>This links your current channel to the selected channel. If they have permission to speak in the other channel, players can now hear each other. This is a permanent link, and will last until manually unlinked or the server is restarted. Please see the shortcuts for push-to-link.</source>
        <translation type="obsolete">あなたのいるチャンネルを選択されたチャンネルとリンクします。他方のチャンネルで発言する権限を持っていたら、プレイヤーはお互いを聞くことができます。これは恒久的なリンクであり、手動でリンクを解除されるかサーバがリスタートするまで続きます。push-to-linkのショートカットを見てください。</translation>
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
        <source>Unlink &amp;All</source>
        <translation type="obsolete">全てのリンクを解除(&amp;A)</translation>
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
        <source>S&amp;tatistics</source>
        <translation type="obsolete">統計(&amp;T)</translation>
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
        <location/>
        <source>&amp;Change Comment</source>
        <translation>コメントを変更(&amp;C)</translation>
    </message>
    <message>
        <source>Change the comment on the selected user.</source>
        <translation type="obsolete">選択されたユーザのコメントを変更する。</translation>
    </message>
    <message>
        <source>This allows you to change the comment (shown as a tooltip) of a user. Unless you&apos;ve been given extra privileges on the server, you can only change your own comment.</source>
        <translation type="obsolete">ツールチップとして表示される、ユーザのコメントを変更することができます。サーバ上で追加の権限を与えられない限り、 自分自身のコメントしか変更することができません。</translation>
    </message>
    <message>
        <source>Certificate Wizard</source>
        <translation type="obsolete">証明書発行ウィザード</translation>
    </message>
    <message>
        <location/>
        <source>Configure certificates for strong authentication</source>
        <translation>強固な認証のための証明書を設定する</translation>
    </message>
    <message>
        <location/>
        <source>This starts the wizard for creating, importing and exporting certificates for authentication against servers.</source>
        <translation>サーバに対する認証のための証明書を、作成・インポート・エクスポートするためのウィザードを開始します。</translation>
    </message>
    <message>
        <source>Register</source>
        <translation type="obsolete">ユーザ登録</translation>
    </message>
    <message>
        <location/>
        <source>Register user on server</source>
        <translation>サーバ上でユーザを登録</translation>
    </message>
    <message>
        <location/>
        <source>This will permanently register the user on the server.</source>
        <translation>永久的にサーバにユーザ登録します。</translation>
    </message>
    <message>
        <source>This will permanently register the user on the server. Note that you can&apos;t change the username later on, nor can you easily unregister a user, so make sure you really want to do this.</source>
        <translation type="obsolete">サーバ上のユーザを永久的にユーザ登録します。登録後にユーザ名を変更できないことと、簡単にユーザ登録を取り消せることを覚えておいてください。あなたが本当にそうしたいときにしてください。</translation>
    </message>
    <message>
        <location/>
        <source>Add &amp;Friend</source>
        <translation>フレンドに追加(&amp;F)</translation>
    </message>
    <message>
        <location/>
        <source>Adds a user as your friend.</source>
        <translation>あなたのフレンドにユーザを追加する。</translation>
    </message>
    <message>
        <location/>
        <source>This will add the user as a friend, so you can recognize him on this and other servers.</source>
        <translation>ユーザをフレンドとして追加します。他のサーバ上でもそのユーザを区別することができます。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Remove Friend</source>
        <translation>フレンドを削除する(&amp;R)</translation>
    </message>
    <message>
        <location/>
        <source>Removes a user from your friends.</source>
        <translation>フレンドからユーザを削除します。</translation>
    </message>
    <message>
        <location/>
        <source>This will remove a user from your friends list.</source>
        <translation>フレンドリストからユーザを削除します。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Update Friend</source>
        <translation>フレンドを更新(&amp;U)</translation>
    </message>
    <message>
        <location/>
        <source>Update name of your friend.</source>
        <translation>フレンドの名前を更新します。</translation>
    </message>
    <message>
        <location/>
        <source>Your friend uses a different name than what is in your database. This will update the name.</source>
        <translation>あなたのフレンドがデータベースに登録されているものと違う名前を使用しています。名前が更新されます。</translation>
    </message>
    <message>
        <location/>
        <source>Registered &amp;Users</source>
        <translation>登録済みユーザ（&amp;U）</translation>
    </message>
    <message>
        <location/>
        <source>Edit registered users list</source>
        <translation>登録済みユーザのリストを編集</translation>
    </message>
    <message>
        <location/>
        <source>This opens the editor for registered users, which allow you to change their name or unregister them.</source>
        <translation>権限を管理するため、登録済みユーザの編集ダイアログを開きます。（ユーザの名前を変更したり、登録を削除したり出来ます）。</translation>
    </message>
    <message>
        <source>Change Texture</source>
        <translation type="obsolete">テクスチャを変更</translation>
    </message>
    <message>
        <source>Change your overlay texture on this server</source>
        <translation type="obsolete">このサーバでのあなたのオーバレイテクスチャを変更する</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Access Tokens</source>
        <translation>アクセストークン(&amp;A)</translation>
    </message>
    <message>
        <location/>
        <source>Add or remove text-based access tokens</source>
        <translation>アクセストークンを追加または削除する</translation>
    </message>
    <message>
        <source>Remove Texture</source>
        <translation type="obsolete">テクスチャを削除</translation>
    </message>
    <message>
        <source>Remove currently defined user texture.</source>
        <translation type="obsolete">現在定義されているユーザテクスチャを削除します。</translation>
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
        <location filename="MainWindow.cpp" line="-1950"/>
        <location line="+12"/>
        <location line="+2159"/>
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
        <source>&amp;Server</source>
        <translation type="obsolete">サーバ(&amp;S)</translation>
    </message>
    <message>
        <source>&amp;Player</source>
        <translation type="obsolete">プレイヤー(&amp;P)</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="-2246"/>
        <source>&amp;Channel</source>
        <translation>チャンネル(&amp;C)</translation>
    </message>
    <message>
        <source>&amp;Audio</source>
        <translation type="obsolete">音声(&amp;A)</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>C&amp;onfigure</source>
        <translation>設定(&amp;O)</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Help</source>
        <translation>ヘルプ(&amp;H)</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="+31"/>
        <source>Push-to-Talk</source>
        <comment>Global Shortcut</comment>
        <translation>プッシュ・トゥ・トーク</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Push and hold this button to send voice.</source>
        <comment>Global Shortcut</comment>
        <translation>声を送るときに押し続けるボタンです。</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This configures the push-to-talk button, and as long as you hold this button down, you will transmit voice.</source>
        <comment>Global Shortcut</comment>
        <translation>プッシュ・トゥ・トークのボタンを設定する。このボタンを押している間あなたの声は送信される。</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Reset Audio Processor</source>
        <comment>Global Shortcut</comment>
        <translation>音声処理をリセット</translation>
    </message>
    <message>
        <source>Toggle Mute Self</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">自分の発言禁止を切り替え</translation>
    </message>
    <message>
        <source>Toggle self-mute status.</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">自分の発言禁止状態を切り替え</translation>
    </message>
    <message>
        <source>This will toggle your muted status. If you toggle this off, you will also disable self-deafen.</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">これはあなたの消音状態を切り換えるでしょう。また、これをオフに切り替えると、聴取禁止も無効になるだろう。</translation>
    </message>
    <message>
        <source>Toggle Deafen Self</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">自分の聴取禁止を切り替え</translation>
    </message>
    <message>
        <source>Toggle self-deafen status.</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">自分の聴取禁止状態を切り替える</translation>
    </message>
    <message>
        <source>This will toggle your deafen status. If you toggle this on, you will also enable self-mute.</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">これはあなたの聴取禁止状態を切り換えるでしょう。また、これをオンに切り替えると、発言禁止も有効になるだろう。</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Unlink Plugin</source>
        <comment>Global Shortcut</comment>
        <translation>プラグインを外す</translation>
    </message>
    <message>
        <source>Force Center Position</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">中央に移動させる</translation>
    </message>
    <message>
        <source>Chan Parent</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">親チャンネルに流す</translation>
    </message>
    <message>
        <source>Chan Sub#%1</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">サブチャンネル#%1に流す</translation>
    </message>
    <message>
        <source>Chan All Subs</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">全サブチャンネルに流す</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Push-to-Mute</source>
        <comment>Global Shortcut</comment>
        <translation>押してる間発言禁止</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Join Channel</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished">チャンネルへの参加</translation>
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
        <source>This will switch the states of the ingame overlay between showing everybody, just the players who are talking, and nobody.</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">ゲーム中でのオーバーレイの状態を「全員」と「話している人だけ」、「なし」の間で切り替える。</translation>
    </message>
    <message>
        <source>Alt Push-to-Talk</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">Alt発言有効化</translation>
    </message>
    <message>
        <source>Kicking player %1</source>
        <translation type="obsolete">プレイヤー %1 をキックしました。</translation>
    </message>
    <message>
        <location line="+1095"/>
        <location line="+19"/>
        <source>Enter reason</source>
        <translation>理由を入力してください</translation>
    </message>
    <message>
        <source>Banning player %1</source>
        <translation type="obsolete">プレイヤー %1 をBANしました</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>Mumble</source>
        <translation>Mumble</translation>
    </message>
    <message>
        <source>Channel Name</source>
        <translation type="obsolete">チャンネル名</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="+272"/>
        <source>Are you sure you want to delete %1 and all its sub-channels?</source>
        <translation>%1 とそのサブチャンネルを全て削除します。よろしいですか?</translation>
    </message>
    <message>
        <location line="+210"/>
        <source>Unmuted and undeafened.</source>
        <translation type="unfinished">発言禁止と聴取禁止も解除しました。</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Unmuted.</source>
        <translation type="unfinished">発言禁止を解除しました。</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Muted.</source>
        <translation type="unfinished">発言を禁止しました。</translation>
    </message>
    <message>
        <location line="+31"/>
        <source>Muted and deafened.</source>
        <translation>発言と聴取が禁止になりました。</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Deafened.</source>
        <translation type="unfinished">聴取を禁止しました。</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Undeafened.</source>
        <translation type="unfinished">聴取禁止を解除しました。</translation>
    </message>
    <message>
        <location line="+94"/>
        <source>About Qt</source>
        <translation>Qtについて</translation>
    </message>
    <message>
        <source>Joining %1.</source>
        <translation type="obsolete">%1 に入る</translation>
    </message>
    <message>
        <source>Connected to server.</source>
        <translation type="obsolete">サーバに接続しました。</translation>
    </message>
    <message>
        <location line="+420"/>
        <source>Server connection failed: %1.</source>
        <translation>サーバへの接続に失敗しました: %1.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Disconnected from server.</source>
        <translation>サーバから切断しました。</translation>
    </message>
    <message>
        <location line="-1492"/>
        <source>Reconnecting.</source>
        <translation type="unfinished">再接続中です。</translation>
    </message>
    <message>
        <source>Joined server: %1.</source>
        <translation type="obsolete">サーバに接続しました: %1.</translation>
    </message>
    <message>
        <source>the server</source>
        <translation type="obsolete">サーバ</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="+323"/>
        <source>You were unmuted and undeafened by %1.</source>
        <translation>%1 があなたを発言・聴取を禁止しました。</translation>
    </message>
    <message>
        <location line="+29"/>
        <source>You muted and deafened %1.</source>
        <translation>%1 を発言・聴取禁止にしました。</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>You unmuted and undeafened %1.</source>
        <translation>%1 の発言・聴取の禁止を解除しました。</translation>
    </message>
    <message>
        <location line="+11"/>
        <source>You undeafened %1.</source>
        <translation>%1 の聴取禁止を解除しました。</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>You suppressed %1.</source>
        <translation>%1 の発言を禁止しました。</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>%1 muted and deafened by %2.</source>
        <translation type="unfinished">%2が%1の発言・聴取を禁止しました。</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1 unmuted and undeafened by %2.</source>
        <translation type="unfinished">%2が%1の発言・聴取の禁止を解除しました。</translation>
    </message>
    <message>
        <location line="+18"/>
        <source>%1 suppressed by %2.</source>
        <translation type="unfinished">%2が%1の発言を禁止しました。</translation>
    </message>
    <message>
        <location line="+25"/>
        <source>%1 moved to %2.</source>
        <translation type="unfinished">%1は%2に移動しました。</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1 moved to %2 by %3.</source>
        <translation type="unfinished">%1は%3によって%2に移動させられました。</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>%1 moved in from %2 by %3.</source>
        <translation type="unfinished">%1は%3によって%2から移動させられました。</translation>
    </message>
    <message>
        <source>Left server: %1.</source>
        <translation type="obsolete">サーバから切断しました: %1.</translation>
    </message>
    <message>
        <location line="-157"/>
        <source>%1 is now muted and deafened.</source>
        <translation type="unfinished">%1が発言禁止/聴取禁止状態になりました。</translation>
    </message>
    <message>
        <location line="-114"/>
        <source>You were denied %1 privileges in %2.</source>
        <translation>あなたは %2 で %1 の権限を拒否されました。</translation>
    </message>
    <message>
        <location line="-69"/>
        <location line="+48"/>
        <source>Welcome message: %1</source>
        <translation>ようこそメッセージ: %1</translation>
    </message>
    <message>
        <location line="-15"/>
        <source>Mumble: %1</source>
        <translation></translation>
    </message>
    <message>
        <location line="+38"/>
        <source>%3 was denied %1 privileges in %2.</source>
        <translation type="unfinished">%3は%2で%1の権限がありません。</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Denied: Cannot modify SuperUser.</source>
        <translation>拒否: SuperUserを参照できません。</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Denied: Invalid channel name.</source>
        <translation>拒否: 無効なチャンネル名です。</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Denied: Text message too long.</source>
        <translation>拒否: テキストメッセージが長すぎます。</translation>
    </message>
    <message>
        <location line="+25"/>
        <source>Denied: Operation not permitted in temporary channel.</source>
        <translation>拒否: 一時チャンネル上では許可されない操作です。</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>You need a certificate to perform this operation.</source>
        <translation>この操作を実行するには証明書が必要です。</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1 does not have a certificate.</source>
        <translation type="unfinished">%1は証明書を持っていません。</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Invalid username: %1.</source>
        <translation>無効なユーザ名: %1。</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Invalid username.</source>
        <translation>無効なユーザ名です。</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Channel is full.</source>
        <translation>チャンネルが満員です。</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Permission denied.</source>
        <translation>権限がありません。</translation>
    </message>
    <message>
        <location line="+37"/>
        <source>%1 connected.</source>
        <translation type="unfinished">%1が接続しました。</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>%1 is now muted.</source>
        <translation type="unfinished">%1が発言禁止状態になりました。</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1 is now unmuted.</source>
        <translation type="unfinished">%1の発言禁止状態が解除されました。</translation>
    </message>
    <message>
        <location line="+11"/>
        <source>Recording started</source>
        <translation type="unfinished">録音開始</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Recording stopped</source>
        <translation type="unfinished">録音停止</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>%1 started recording.</source>
        <translation type="unfinished">%1が録音を開始しました。</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1 stopped recording.</source>
        <translation type="unfinished">%1が録音を停止しました。</translation>
    </message>
    <message>
        <location line="+41"/>
        <source>You were unsuppressed.</source>
        <translation>あなたの発言禁止が解除されました。</translation>
    </message>
    <message>
        <location line="+96"/>
        <source>%1 is recording</source>
        <translation type="unfinished">%1は録音しています</translation>
    </message>
    <message>
        <location line="+46"/>
        <source>%1 disconnected.</source>
        <translation type="unfinished">%1が切断しました。</translation>
    </message>
    <message>
        <location line="+91"/>
        <source>(Tree) </source>
        <translation>(ツリー) </translation>
    </message>
    <message>
        <location line="+2"/>
        <source>(Channel) </source>
        <translation>(チャンネル) </translation>
    </message>
    <message>
        <location line="+3"/>
        <source>%2%1: %3</source>
        <translation></translation>
    </message>
    <message>
        <source>server</source>
        <translation type="obsolete">サーバ</translation>
    </message>
    <message>
        <location line="-254"/>
        <source>You were muted by %1.</source>
        <translation>%1 があなたの発言を禁止しました。</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>You were suppressed.</source>
        <oldsource>You were suppressed by %1.</oldsource>
        <translation>あなたの発言が禁止されました。</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>You were unsuppressed by %1.</source>
        <translation>あなたは %1 によって発言禁止を解除されました。</translation>
    </message>
    <message>
        <location line="-16"/>
        <source>You were unmuted by %1.</source>
        <translation>あなたは %1 によって発言禁止を解除されました。</translation>
    </message>
    <message>
        <source>You deafened %1.</source>
        <translation type="obsolete">あなたは %1 を聴取禁止にしました。</translation>
    </message>
    <message>
        <location line="+29"/>
        <source>You muted %1.</source>
        <translation>あなたは %1 を発言禁止にしました。</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>You unsuppressed %1.</source>
        <translation>あなたは %1 の発言禁止を解除しました。</translation>
    </message>
    <message>
        <location line="-14"/>
        <source>You unmuted %1.</source>
        <translation>あなたは %1 の発言禁止を解除しました。</translation>
    </message>
    <message>
        <location line="+25"/>
        <source>%1 muted by %2.</source>
        <translation type="unfinished">%2が%1を発言禁止にしました。</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>%1 unsuppressed by %2.</source>
        <translation type="unfinished">%1が%2によって発言禁止を解除されました。</translation>
    </message>
    <message>
        <location line="-14"/>
        <source>%1 unmuted by %2.</source>
        <translation type="unfinished">%1は%2によって発言禁止を解除されました。</translation>
    </message>
    <message>
        <source>the server</source>
        <comment>message from</comment>
        <translation type="obsolete">サーバ</translation>
    </message>
    <message>
        <location line="+338"/>
        <source>Unable to find matching CELT codecs with other clients. You will not be able to talk to all users.</source>
        <translation>他のクライアントとマッチするCELTコーデックが見つかりません。あなたは全てのユーザとは話せないでしょう。</translation>
    </message>
    <message>
        <location line="-404"/>
        <source>You were muted and deafened by %1.</source>
        <oldsource>You were deafened by %1.</oldsource>
        <translation>%1 があなたの聴取を禁止しました。</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>You were undeafened by %1.</source>
        <translation>%1 があなたの聴取禁止を解除しました。</translation>
    </message>
    <message>
        <source>%1 deafened by %2.</source>
        <translation type="obsolete">%1 は %2 によって聴取禁止にされました。</translation>
    </message>
    <message>
        <location line="+58"/>
        <source>%1 undeafened by %2.</source>
        <translation type="unfinished">%2が%1の聴取禁止を解除しました。</translation>
    </message>
    <message>
        <location line="+86"/>
        <source>You were kicked from the server by %1: %2.</source>
        <translation>あなたは %1 によってサーバからキックされました: %2.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>%3 was kicked from the server by %1: %2.</source>
        <translation type="unfinished">%3は%1によってサーバからキックされました: %2.</translation>
    </message>
    <message>
        <location line="-7"/>
        <source>You were kicked and banned from the server by %1: %2.</source>
        <translation>あなたは %1 によってキックされ、サーバからBANされました: %2.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>%3 was kicked and banned from the server by %1: %2.</source>
        <translation type="unfinished">%3は%1によってキックされ、サーバからBANされました: %2.</translation>
    </message>
    <message>
        <location line="-60"/>
        <source>You were moved to %1 by %2.</source>
        <translation>%2 があなたを %1 に移動させました。</translation>
    </message>
    <message>
        <source>%1 left channel.</source>
        <translation type="obsolete">%1 がチャンネルから退出しました</translation>
    </message>
    <message>
        <source>%1 moved out by %2 to %3.</source>
        <translation type="obsolete">%1 は %2 によって %3 に移動させられました。</translation>
    </message>
    <message>
        <source>%1 moved in by %2 from %3.</source>
        <translation type="obsolete">%1 は %2 によって %3 から移動させられました。</translation>
    </message>
    <message>
        <source>%1 moved out by %2.</source>
        <translation type="obsolete">%1 は %2 によって移動させられました。</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>%1 entered channel.</source>
        <translation type="unfinished">%1がチャンネルに入りました。</translation>
    </message>
    <message>
        <source>%1 moved in by %2.</source>
        <translation type="obsolete">%1 は %2 によって移動させられました。</translation>
    </message>
    <message>
        <location line="-346"/>
        <source>Server connection rejected: %1.</source>
        <translation>サーバへの接続は拒否されました: %1.</translation>
    </message>
    <message>
        <location line="+141"/>
        <source>Denied: %1.</source>
        <translation>拒否されました: %1.</translation>
    </message>
    <message>
        <location filename="main.cpp" line="-52"/>
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
        <translation>アプリケーションを終了します。</translation>
    </message>
    <message>
        <source>Send Messa&amp;ge</source>
        <translation type="obsolete">メッセージを送信(&amp;G)</translation>
    </message>
    <message>
        <location/>
        <source>Send a Text Message</source>
        <translation>テキストメッセージを送信</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="+476"/>
        <source>Sending message to %1</source>
        <translation>%1 にメッセージを送信しています</translation>
    </message>
    <message>
        <location line="+12"/>
        <location line="+84"/>
        <location line="+4"/>
        <location line="+232"/>
        <source>To %1: %2</source>
        <translation>%1 宛: %2</translation>
    </message>
    <message>
        <location line="-320"/>
        <location line="+88"/>
        <source>Message to %1</source>
        <translation>%1 へのメッセージ</translation>
    </message>
    <message>
        <location line="-59"/>
        <source>View comment on user %1</source>
        <translation>ユーザ %1 のコメントを見ます</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Are you sure you want to reset the comment of user %1?</source>
        <translation>ユーザ %1 のコメントをリセットしますか?</translation>
    </message>
    <message>
        <location line="+273"/>
        <source>To %1 (Tree): %2</source>
        <oldsource>(Tree) %1: %2</oldsource>
        <translation>(ツリー） %1宛: %2</translation>
    </message>
    <message>
        <location line="+677"/>
        <source>Failed to launch compatibility client</source>
        <translation>互換クライアントの起動に失敗しました</translation>
    </message>
    <message>
        <location line="+21"/>
        <source>Invalid username</source>
        <translation>無効なユーザ名</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>You connected with an invalid username, please try another one.</source>
        <translation>無効なユーザ名で接続しました。他の名前を試してください。</translation>
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
        <translation>登録されたユーザのパスワードが違うか証明書が一致しません。再度試してください。</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Wrong server password for unregistered user account, please try again.</source>
        <translation>未登録ユーザのパスワードが違います。再度試してください。</translation>
    </message>
    <message>
        <location line="+209"/>
        <source>Images (*.png *.jpg *.jpeg *.svg)</source>
        <translation type="unfinished">画像ファイル (*.png *.jpg *.jpeg *.svg)</translation>
    </message>
    <message>
        <source>Images (*.png *.jpg *.svg)</source>
        <translation type="obsolete">画像 (*.png *.jpg *.svg)</translation>
    </message>
    <message>
        <source>From %1: %2</source>
        <translation type="obsolete">%1 より: %2</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="+353"/>
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
        <location filename="MainWindow.cpp" line="-2440"/>
        <source>&amp;User</source>
        <translation>ユーザ(&amp;U)</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>This is the chatbar&lt;br /&gt;If you enter text here and then press enter the text is sent to the user or channel that was selected. If nothing is selected the message is sent to your current channel.</source>
        <translation>これはチャットバーです。&lt;br /&gt;
テキストをここに打ち込み ENTERキーを押せば、テキストは選択されたユーザかチャンネルに送られます。どこも選択しなければ、あなたが現在いるチャンネルに送信されます。</translation>
    </message>
    <message>
        <location/>
        <source>Chatbar</source>
        <translation>チャットバー</translation>
    </message>
    <message>
        <location/>
        <source>Shows a dialog of registered servers, and also allows quick-connect.</source>
        <translation>登録済みのサーバのダイアログを表示し、高速に接続することを許可する。</translation>
    </message>
    <message>
        <location/>
        <source>Kick user (with reason)</source>
        <translation>（理由付で）ユーザをキック</translation>
    </message>
    <message>
        <location/>
        <source>Kick selected user off server. You&apos;ll be asked to specify a reason.</source>
        <translation>サーバから選択されたユーザを追放します。あなたは追放する理由を尋ねられます。</translation>
    </message>
    <message>
        <location/>
        <source>Mute user</source>
        <translation>ユーザの発言を禁止しました</translation>
    </message>
    <message>
        <location/>
        <source>Mute or unmute user on server. Unmuting a deafened user will also undeafen them.</source>
        <translation>サーバにいるユーザを発言禁止にしたり、解除したりします。聴取禁止になっているユーザに対して発言禁止の解除を行うと、聴取禁止も解除されます。</translation>
    </message>
    <message>
        <location/>
        <source>Kick and ban user (with reason)</source>
        <translation>（理由付きで）ユーザをキックした後にバン</translation>
    </message>
    <message>
        <location/>
        <source>Kick and ban selected user from server. You&apos;ll be asked to specify a reason.</source>
        <translation>サーバから選択されたユーザのキックとバン。あなたは理由を尋ねられます。</translation>
    </message>
    <message>
        <location/>
        <source>Deafen user</source>
        <translation>発言禁止のユーザ</translation>
    </message>
    <message>
        <location/>
        <source>Deafen or undeafen user on server. Deafening a user will also mute them.</source>
        <translation>サーバ上のユーザを聴取禁止にしたり解除したりします。聴取禁止のユーザは発言も禁止になります。</translation>
    </message>
    <message>
        <location/>
        <source>Mute user locally</source>
        <translation>ローカルでユーザを発言禁止にしました</translation>
    </message>
    <message>
        <location/>
        <source>Mute or unmute user locally. Use this on other users in the same room.</source>
        <translation>ローカルでユーザを発言禁止にします。同じ部屋にいるユーザに使ってください。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Edit</source>
        <translation>編集(&amp;E)</translation>
    </message>
    <message>
        <location/>
        <source>This links your current channel to the selected channel. If users in a channel have permission to speak in the other channel, users can now hear each other. This is a permanent link, and will last until manually unlinked or the server is restarted. Please see the shortcuts for push-to-link.</source>
        <oldsource>This links your current channel to the selected channel. If they have permission to speak in the other channel, users can now hear each other. This is a permanent link, and will last until manually unlinked or the server is restarted. Please see the shortcuts for push-to-link.</oldsource>
        <translation>あなたのいるチャンネルを選択されたチャンネルとリンクします。他方のチャンネルで発言する権限を持っていたら、ユーザはお互いを聞くことができます。これは恒久的なリンクであり、手動でリンクを解除されるかサーバがリスタートするまで続きます。push-to-linkのショートカットを見てください。</translation>
    </message>
    <message>
        <location/>
        <source>Start the audio configuration wizard</source>
        <translation>音声設定の設定ウィザードを開始します</translation>
    </message>
    <message>
        <location/>
        <source>This will guide you through the process of configuring your audio hardware.</source>
        <translation>サウンドハードウェアの設定プロセスに案内します。</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="+2141"/>
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
        <location filename="MainWindow.cpp" line="-1700"/>
        <source>Opening URL %1</source>
        <translation>URL %1 を開きます</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>File does not exist</source>
        <translation>ファイルが存在しません</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>File is not a configuration file.</source>
        <translation>設定ファイルが見つかりません。</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Settings merged from file.</source>
        <translation>設定をファイルからマージする。</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>URL scheme is not &apos;mumble&apos;</source>
        <translation>URLスキームは &apos;mumble&apos; ではありません</translation>
    </message>
    <message>
        <location line="+19"/>
        <source>This version of Mumble can&apos;t handle URLs for Mumble version %1.%2.%3</source>
        <translation>このバージョンのMumble は Mumble %1.%2.%3 のための URL を扱えません</translation>
    </message>
    <message>
        <location line="+29"/>
        <source>Connecting to %1</source>
        <translation>%1 に接続しています</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Enter username</source>
        <translation>ユーザ名を入力してください</translation>
    </message>
    <message>
        <location line="+14"/>
        <location line="+176"/>
        <source>Connecting to server %1.</source>
        <translation>サーバ %1 に接続しています。</translation>
    </message>
    <message>
        <location line="+50"/>
        <source>Change your comment</source>
        <translation>コメントを変更します</translation>
    </message>
    <message>
        <location line="+92"/>
        <source>&lt;h2&gt;Version&lt;/h2&gt;&lt;p&gt;Protocol %1.%2.%3.&lt;/p&gt;</source>
        <translation>&lt;h2&gt;バージョン&lt;/h2&gt;&lt;p&gt;プロトコル %1.%2.%3.&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>&lt;p&gt;No build information or OS version available.&lt;/p&gt;</source>
        <translation>&lt;p&gt;ビルド情報がないか OS のバージョンが有効です。&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>&lt;p&gt;%1 (%2)&lt;br /&gt;%3&lt;/p&gt;</source>
        <translation></translation>
    </message>
    <message>
        <source>&lt;h2&gt;Control channel&lt;/h2&gt;&lt;p&gt;Encrypted with %1 bit %2&lt;br /&gt;%3 ms average latency (%4 deviation)&lt;/p&gt;</source>
        <translation type="obsolete">&lt;h2&gt;制御チャンネル&lt;/h2&gt;&lt;p&gt;%1 bit %2 で暗号化されています。&lt;br /&gt;平均 %3 ms の遅延 (平方偏差 %4)&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+20"/>
        <source>&lt;h2&gt;Voice channel&lt;/h2&gt;&lt;p&gt;Encrypted with 128 bit OCB-AES128&lt;br /&gt;%1 ms average latency (%4 deviation)&lt;/p&gt;</source>
        <translation>&lt;h2&gt;音声チャンネル&lt;/h2&gt;&lt;p&gt;128 bit OCB-AES128 で暗号化されています。&lt;br /&gt;平均 %1 ms の遅延 (平方偏差 %4)&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+11"/>
        <source>&lt;h2&gt;Audio bandwidth&lt;/h2&gt;&lt;p&gt;Maximum %1 kbit/s&lt;br /&gt;Current %2 kbit/s&lt;/p&gt;</source>
        <translation>&lt;h2&gt;音声帯域幅&lt;/h2&gt;&lt;p&gt;最大値 %1 kbit/s&lt;br /&gt;現在値 %2 kbit/s&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Mumble Server Information</source>
        <translation>Mumble サーバの情報</translation>
    </message>
    <message>
        <location line="+4"/>
        <location line="+1264"/>
        <source>&amp;View Certificate</source>
        <translation>証明書を見る(&amp;V)</translation>
    </message>
    <message>
        <location line="-1374"/>
        <location line="+327"/>
        <source>Register yourself as %1</source>
        <translation>%1 の名前で自分をユーザ登録します</translation>
    </message>
    <message>
        <location line="-1052"/>
        <source>This will switch the states of the in-game overlay.</source>
        <comment>Global Shortcut</comment>
        <translation>ゲームオーバレイの状態を切り替えます。</translation>
    </message>
    <message>
        <location line="+725"/>
        <location line="+327"/>
        <source>&lt;p&gt;You are about to register yourself on this server. This action cannot be undone, and your username cannot be changed once this is done. You will forever be known as &apos;%1&apos; on this server.&lt;/p&gt;&lt;p&gt;Are you sure you want to register yourself?&lt;/p&gt;</source>
        <translation>&lt;p&gt;サーバ上で自分自身をユーザ登録しようとしています。このアクションは取り消せません。このサーバでのあなたのユーザ名をこの操作の後に変更できなくなります。あなたはこのサーバ上で &apos;%1&apos; のユーザ名を使うことになります。&lt;/p&gt;&lt;p&gt;本当に自分自身を登録しますか？&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Register user %1</source>
        <translation>ユーザ %1 を登録</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>&lt;p&gt;You are about to register %1 on the server. This action cannot be undone, the username cannot be changed, and as a registered user, %1 will have access to the server even if you change the server password.&lt;/p&gt;&lt;p&gt;From this point on, %1 will be authenticated with the certificate currently in use.&lt;/p&gt;&lt;p&gt;Are you sure you want to register %1?&lt;/p&gt;</source>
        <translation>&lt;p&gt;あなたはサーバ上で %1 をユーザ登録しようとしています。このアクションは取り消せませんし登録済みのユーザ名を変えることもできません。登録済みのユーザ &apos;%1&apos; は、サーバパスワードを変更したとしても、サーバにアクセスするでしょう。&lt;/p&gt;&lt;p&gt;この時点から、&apos;%1&apos; は 現在の証明書でユーザ認証されます。&lt;/p&gt;&lt;p&gt;本当に %1 を登録しますか？&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+40"/>
        <source>Kicking user %1</source>
        <translation>ユーザをキック : %1</translation>
    </message>
    <message>
        <location line="+19"/>
        <source>Banning user %1</source>
        <translation>ユーザ：%1 をバンする</translation>
    </message>
    <message>
        <source>Change comment on user %1</source>
        <translation type="obsolete">ユーザ %1 のコメントを変更する</translation>
    </message>
    <message>
        <source>To channel %1: %2</source>
        <translation type="obsolete">チャンネル %1 宛: %2</translation>
    </message>
    <message>
        <location line="+118"/>
        <location line="+236"/>
        <source>Message to channel %1</source>
        <translation>チャンネル %1 へのメッセージ</translation>
    </message>
    <message>
        <location line="+490"/>
        <source>Connected.</source>
        <translation type="unfinished">サーバに接続しました。</translation>
    </message>
    <message>
        <location line="+159"/>
        <source>SSL Version mismatch</source>
        <translation>SSL のバージョンが一致しません</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>This server is using an older encryption standard. It might be an older 1.1 based Mumble server.&lt;br /&gt;Would you like to launch the compatibility client to connect to it?</source>
        <translation>このサーバはより古い暗号化手順を使用しています。 1.1 ベースの Mumble サーバである可能性があります。&lt;br /&gt;このサーバに接続するために互換クライアントを起動しますか?</translation>
    </message>
    <message>
        <source>Failed to launch compatibilty client</source>
        <translation type="obsolete">互換クライアントの起動に失敗</translation>
    </message>
    <message>
        <location line="+26"/>
        <source>The compatibility client could not be found, or failed to start.&lt;br /&gt;Note that the compatibility client is an optional component for most installations, and might not be installed.</source>
        <translation>互換クライアントが見つからないか起動に失敗しました。&lt;br /&gt;互換クライアントはほとんどのインストールに置いて任意の項目であり、インストールされていない可能性があります。</translation>
    </message>
    <message>
        <location line="-50"/>
        <source>&lt;p&gt;%1.&lt;br /&gt;The specific errors with this certificate are: &lt;/p&gt;&lt;ol&gt;%2&lt;/ol&gt;&lt;p&gt;Do you wish to accept this certificate anyway?&lt;br /&gt;(It will also be stored so you won&apos;t be asked this again.)&lt;/p&gt;</source>
        <translation>&lt;p&gt;%1.&lt;br /&gt;詳細な証明書エラーの内容: &lt;/p&gt;&lt;ol&gt;%2&lt;/ol&gt;&lt;p&gt;この証明書を受け入れますか？&lt;br /&gt;(同時に証明書を保存するので、今後この質問は表示されません。)&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="-1285"/>
        <source>&lt;h2&gt;Control channel&lt;/h2&gt;&lt;p&gt;Encrypted with %1 bit %2&lt;br /&gt;%3 ms average latency (%4 deviation)&lt;/p&gt;&lt;p&gt;Remote host %5 (port %6)&lt;/p&gt;</source>
        <translation>&lt;h2&gt;制御チャンネル&lt;/h2&gt;&lt;p&gt;%1 bit %2 で暗号化されています。&lt;br /&gt;平均 %3 ms の遅延 (平方偏差 %4)&lt;/p&gt;&lt;p&gt;リモートホスト %5 (ポート %6)&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+1462"/>
        <source>&lt;center&gt;Type message to channel &apos;%1&apos; here&lt;/center&gt;</source>
        <oldsource>Type message to channel &apos;%1&apos; here</oldsource>
        <translation>チャンネル &apos;%1&apos; へのメッセージをここに入力</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>&lt;center&gt;Type message to user &apos;%1&apos; here&lt;/center&gt;</source>
        <oldsource>Type message to user &apos;%1&apos; here</oldsource>
        <translation>ユーザ &apos;%1&apos; へのメッセージをここに入力</translation>
    </message>
    <message>
        <location line="+105"/>
        <source>Choose image file</source>
        <translation>イメージファイルを選択</translation>
    </message>
    <message>
        <source>Images (*.png *.jpg)</source>
        <translation type="obsolete">イメージ (*.png *.jpg)</translation>
    </message>
    <message>
        <location line="+7"/>
        <location line="+17"/>
        <source>Failed to load image</source>
        <translation>画像の読み込みに失敗</translation>
    </message>
    <message>
        <location line="-17"/>
        <source>Could not open file for reading.</source>
        <translation>読み込みモードでファイルを開けません。</translation>
    </message>
    <message>
        <location line="+17"/>
        <source>Image format not recognized.</source>
        <translation>画像の形式を認識できません。</translation>
    </message>
    <message>
        <source>&lt;h2&gt;Control channel&lt;/h2&gt;&lt;p&gt;Encrypted with %1 bit %2&lt;br /&gt;%3 ms average latency (%4 variance)&lt;/p&gt;</source>
        <translation type="obsolete">&lt;h2&gt;制御チャンネル&lt;/h2&gt;&lt;p&gt;%1 bit %2 で暗号化されています。&lt;br /&gt;平均 %3 ms の遅延 (平方偏差 %4)&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="-1585"/>
        <source>Voice channel is sent over control channel.</source>
        <translation>音声チャンネルは管理チャンネルを通して送信されます。</translation>
    </message>
    <message>
        <source>&lt;h2&gt;Voice channel&lt;/h2&gt;&lt;p&gt;Encrypted with 128 bit OCB-AES128&lt;br /&gt;%1 ms average latency (%4 variance)&lt;/p&gt;</source>
        <translation type="obsolete">&lt;h2&gt;音声チャンネル&lt;/h2&gt;&lt;p&gt;128 bit OCB-AES128 で暗号化されています。&lt;br /&gt;平均 %1 ms の遅延 (平方偏差 %4)&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+9"/>
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
        <source>Change description of channel %1</source>
        <translation type="obsolete">チャンネル 「%1」 の説明を変更する</translation>
    </message>
    <message>
        <location line="+631"/>
        <source>Sending message to channel %1</source>
        <translation>チャンネル %1 に送信するメッセージ</translation>
    </message>
    <message>
        <source>Sending message to channel tree %1</source>
        <translation type="obsolete">チャンネルツリー %1 に送信するメッセージ</translation>
    </message>
    <message>
        <source>To tree %1: %2</source>
        <translation type="obsolete">tree %1 宛: %2</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Message to tree %1</source>
        <translation>tree %1 宛メッセージ</translation>
    </message>
    <message>
        <source>You are now in local deafen mode. This mode is not reflected on the server, and you will still be transmitting voice to the server. This mode should only be used if there are several people in the same room and one of them have Mumble on loudspeakers.</source>
        <translation type="obsolete">ローカルな聴取禁止モードになりました。このモードはサーバには反映されません。そしてあなたの声はまだ送信され続けています。このモードは、同じ部屋に数人の人がいて、そのうちの1人がMumbleを大音量のスピーカーで使っているような時だけ使用すればよいでしょう。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>Ctrl+Q</source>
        <translation>Ctrl+Q</translation>
    </message>
    <message>
        <source>Mute player locally</source>
        <translation type="obsolete">ローカルでプレイヤーを発言禁止にする</translation>
    </message>
    <message>
        <source>Mute or unmute player locally. Use this on other players in the same room.</source>
        <translation type="obsolete">ローカルでプレイヤーを発言禁止にします。同じ部屋にいるプレイヤーに使ってください。</translation>
    </message>
    <message>
        <source>&amp;Rename Channel</source>
        <translation type="obsolete">チャンネル名を変更(&amp;R)</translation>
    </message>
    <message>
        <source>Renames the channel</source>
        <translation type="obsolete">チャンネル名を変更します</translation>
    </message>
    <message>
        <source>This renames a channel.</source>
        <translation type="obsolete">チャンネルの名前を変更します。</translation>
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
        <source>Send &amp;Tree Message</source>
        <translation type="obsolete">ツリーにメッセージを送信(&amp;T)</translation>
    </message>
    <message>
        <source>Send message to channel tree.</source>
        <translation type="obsolete">チャンネルツリーにメッセージを送信します</translation>
    </message>
    <message>
        <source>This sends a text message to a channel and it&apos;s subchannels.</source>
        <translation type="obsolete">チャンネルとそのサブチャンネルにテキストメッセージを送信します。</translation>
    </message>
    <message>
        <source>Server maximum bandwidth is only %1 kbit/s. Quality auto-adjusted.</source>
        <translation type="obsolete">サーバの最大帯域幅は %1 kbit/s しかありません。品質は自動的に調整されます。</translation>
    </message>
    <message>
        <source>This is the first time you&apos;re starting Mumble.&lt;br /&gt;Would you like to go through the Audio Wizard to configure your soundcard?</source>
        <translation type="obsolete">初めての起動です。&lt;br /&gt;サウンドカードの設定をするために音声ウィザードに移動しますか?</translation>
    </message>
    <message>
        <source>&lt;h2&gt;Audio bandwidth&lt;/h2&gt;&lt;p&gt;Maximum %1 kbit/s&lt;br /&gt;Current %2 kbit/s (Quality %3)&lt;/p&gt;</source>
        <translation type="obsolete">&lt;h2&gt;音声帯域幅&lt;/h2&gt;&lt;p&gt;最大値 %1 kbit/s&lt;br /&gt;現在値 %2 kbit/s (品質 %3)&lt;/p&gt;</translation>
    </message>
    <message>
        <source>Connected to server %1.</source>
        <translation type="obsolete">サーバ %1 に接続しました。</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="-1310"/>
        <source>Mumble is currently connected to a server. Do you want to Close or Minimize it?</source>
        <translation>Mumble は現在、サーバに接続しています。接続を閉じるか最小化しますか?</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Close</source>
        <translation>閉じる</translation>
    </message>
    <message>
        <location line="-125"/>
        <location line="+126"/>
        <source>Minimize</source>
        <translation>最小化</translation>
    </message>
    <message>
        <location line="-179"/>
        <source>Mute Self</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished">発言禁止状態</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Set self-mute status.</source>
        <comment>Global Shortcut</comment>
        <translation>自分自身を発言禁止状態に設定。</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This will set or toggle your muted status. If you turn this off, you will also disable self-deafen.</source>
        <comment>Global Shortcut</comment>
        <translation>発言禁止の状態を設定するか切り替えます。オフになると、聴取禁止も無効になります。</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Deafen Self</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished">聴取禁止状態</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Set self-deafen status.</source>
        <comment>Global Shortcut</comment>
        <translation>自分自身を聴取禁止状態に変更。</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This will set or toggle your deafened status. If you turn this on, you will also enable self-mute.</source>
        <comment>Global Shortcut</comment>
        <translation>あなたの聴取禁止状態を設定するか切り替えます。オンになれば、発言禁止も有効になります。</translation>
    </message>
    <message>
        <source>This will switch the states of the in-game overlay between showing everybody, just the users who are talking, and nobody.</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">ゲーム中でのオーバーレイの状態を「全員」と「話している人だけ」、「なし」の間で切り替える。</translation>
    </message>
    <message>
        <location line="+30"/>
        <source>Whisper/Shout</source>
        <oldsource>Whisper</oldsource>
        <translation>ささやき</translation>
    </message>
    <message>
        <location line="+13"/>
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
        <location line="-28"/>
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
        <location line="+65"/>
        <location line="+2189"/>
        <source>&lt;center&gt;Not connected&lt;/center&gt;</source>
        <oldsource>Not connected</oldsource>
        <translation>接続されていません</translation>
    </message>
    <message>
        <location line="-1905"/>
        <source>Clear</source>
        <translation>クリア</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>Log</source>
        <translation>ログ</translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="+1381"/>
        <source>You have Channel Dragging set to &quot;Do Nothing&quot; so the channel wasn&apos;t moved.</source>
        <translation>チャンネルをドラッグしたときに &quot;何もしない&quot; ようにします; チャンネルが移動しなくなります。</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Unknown Channel Drag mode in UserModel::dropMimeData.</source>
        <translation>UserModel::dropMimeData で不明なチャンネルドラッグモードです。</translation>
    </message>
    <message>
        <source>Unknown Channel Drag mode in PlayerModel::dropMimeData.</source>
        <translation type="obsolete">不明なチャンネルドラッグモード, PlayerModel::dropMimeData</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>&amp;Unlink</source>
        <comment>Channel</comment>
        <translation>リンク解除(&amp;U)</translation>
    </message>
    <message>
        <source>&amp;Unlink</source>
        <comment>Plugin</comment>
        <translation type="obsolete">リンク解除(&amp;U)</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="-10"/>
        <location line="+1"/>
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
    <message>
        <location/>
        <source>Send &amp;Message</source>
        <translation>メッセージを送信(&amp;M)</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Unlink All</source>
        <translation>全てのリンクを解除(&amp;U)</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Certificate Wizard</source>
        <translation>証明書ウィザード(&amp;C)</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Register</source>
        <translation>ユーザ登録(&amp;R)</translation>
    </message>
    <message>
        <source>Change &amp;Texture</source>
        <translation type="obsolete">テクスチャを変更(&amp;T)</translation>
    </message>
    <message>
        <source>&amp;Remove Texture</source>
        <translation type="obsolete">テクスチャを削除(&amp;R)</translation>
    </message>
    <message>
        <location/>
        <source>Reset &amp;Comment</source>
        <translation>コメントをリセット(&amp;R)</translation>
    </message>
    <message>
        <location/>
        <source>Reset the comment of the selected user.</source>
        <translation>選択したユーザのコメントをリセットする。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Join Channel</source>
        <translation>チャンネルに参加(&amp;J)</translation>
    </message>
    <message>
        <location/>
        <source>View Comment</source>
        <translation>コメントを見る</translation>
    </message>
    <message>
        <location/>
        <source>View comment in editor</source>
        <translation>エディタでコメントを見ます</translation>
    </message>
    <message>
        <location/>
        <source>Query server for connection information for user</source>
        <translation>ユーザの接続情報のためのクエリ</translation>
    </message>
    <message>
        <location/>
        <source>S&amp;erver</source>
        <translation>サーバ(&amp;E)</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Self</source>
        <translation>状態(&amp;S)</translation>
    </message>
    <message>
        <location/>
        <source>Audio S&amp;tatistics</source>
        <translation>音声統計(&amp;T)</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Unlink Plugins</source>
        <translation>プラグインをアンリンク(&amp;U)</translation>
    </message>
    <message>
        <source>Change your own comment.</source>
        <translation type="obsolete">自分のコメントを変更する。</translation>
    </message>
    <message>
        <location/>
        <source>R&amp;egister</source>
        <translation>登録(&amp;E)</translation>
    </message>
    <message>
        <location/>
        <source>Register yourself on the server</source>
        <translation>サーバにあなたを登録します</translation>
    </message>
    <message>
        <location/>
        <source>Change &amp;Avatar</source>
        <translation>アバターを変更(&amp;A)</translation>
    </message>
    <message>
        <location/>
        <source>Change your avatar image on this server</source>
        <translation>このサーバでのアバターイメージを変更します</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Remove Avatar</source>
        <translation>アバターを削除(&amp;R)</translation>
    </message>
    <message>
        <location/>
        <source>Remove currently defined avatar image.</source>
        <translation>現在、設定されている画像を削除します。</translation>
    </message>
    <message>
        <location/>
        <source>Icon Toolbar</source>
        <translation>アイコンツールバー</translation>
    </message>
    <message>
        <location/>
        <source>Change your own comment</source>
        <translation>自分のコメントを変更</translation>
    </message>
    <message>
        <source>Priority speaker</source>
        <translation type="obsolete">優先度スピーカ</translation>
    </message>
    <message>
        <location/>
        <source>Recording</source>
        <translation type="unfinished">録音</translation>
    </message>
    <message>
        <location/>
        <source>Priority Speaker</source>
        <translation type="unfinished">優先度スピーカ</translation>
    </message>
</context>
<context>
    <name>NetworkConfig</name>
    <message>
        <location filename="NetworkConfig.cpp" line="+53"/>
        <source>Network</source>
        <translation>ネットワーク</translation>
    </message>
    <message>
        <location line="+36"/>
        <source>Updates are mandatory when using snapshot releases.</source>
        <translation>snapshot がリリースされたときはアップデートが必須です。</translation>
    </message>
    <message>
        <source>Form</source>
        <translation type="obsolete">フォーム</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui"/>
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
        <source>Use TCP mode</source>
        <translation type="obsolete">TCPモードを使用する</translation>
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
        <source>&lt;b&gt;Type of proxy to connect through&lt;/b&gt;&lt;br /&gt;This makes Mumble connect through a proxy for all outgoing connections. Note: Proxy tunneling forces Mumble into TCP compatibility mode, causing all voice data to be sent via the control channel.</source>
        <oldsource>&lt;b&gt;Type of proxy to connect through&lt;/b&gt;&lt;br /&gt;This makes Mumble connect through a proxy for all outgoing connections. Note: Proxy tunneling forces Mumble into TCP compatability mode, causing all voice data to be sent via the control channel.</oldsource>
        <translation type="obsolete">&lt;b&gt;プロキシの接続タイプ&lt;/b&gt;&lt;br /&gt;プロキシを通して外向きの接続を行うようにします。 注意: プロキシ接続をする場合、強制的にTCP互換モードに変更されます。 全ての音声データは管理チャンネルを通して送信されるようになります。</translation>
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
        <source>&lt;b&gt;Hostname of the proxy.&lt;b&gt;&lt;br /&gt;This field specifies the hostname of the proxy you wish to tunnel network traffic through.</source>
        <translation type="obsolete">&lt;b&gt;プロキシサーバのホスト名&lt;b&gt;&lt;br /&gt;あなたが使用したいプロキシサーバのホスト名を指定します。</translation>
    </message>
    <message>
        <location/>
        <source>Force TCP mode</source>
        <translation>強制的にTCPモードを使用する</translation>
    </message>
    <message>
        <location/>
        <source>Enable QoS to prioritize packets</source>
        <translation>優先度付パケットのためにQoSを有効化</translation>
    </message>
    <message>
        <location/>
        <source>This will enable QoS, which will attempt to prioritize voice packets over other traffic.</source>
        <translation>QoSを有効にします。QoS 他の通信トラフィックより音声パケットの優先順位を上げようとするものです。</translation>
    </message>
    <message>
        <location/>
        <source>Use Quality of Service</source>
        <translation>QoS を使用する</translation>
    </message>
    <message>
        <location/>
        <source>Don&apos;t send certificate to server and don&apos;t save passwords. (Not saved).</source>
        <translation>サーバに証明書を送らず、パスワードも保存しない。</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This will suppress identity information from the client.&lt;/b&gt;&lt;p&gt;The client will not identify itself with a certificate, even if defined, and will not cache passwords for connections. This is primarily a test-option and is not saved.&lt;/p&gt;</source>
        <translation>&lt;b&gt;これはクライアントから個人情報の送信を抑制します。&lt;/b&gt;&lt;p&gt;クライアントは証明書を使用しませんし、接続のためのパスワードを保存しません。これはテストオプションで設定は保存されません。&lt;/p&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Suppress certificate and password storage</source>
        <translation>証明書とパスワードの保存を抑制する</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Type of proxy to connect through.&lt;/b&gt;&lt;br /&gt;This makes Mumble connect through a proxy for all outgoing connections. Note: Proxy tunneling forces Mumble into TCP compatibility mode, causing all voice data to be sent via the control channel.</source>
        <translation>&lt;b&gt;プロキシの接続タイプ。&lt;/b&gt;&lt;br /&gt;プロキシを通して外向きの接続を行うようにします。 注意: プロキシ接続をする場合、強制的にTCP互換モードに変更されます。 全ての音声データは管理チャンネルを通して送信されるようになります。</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Hostname of the proxy.&lt;/b&gt;&lt;br /&gt;This field specifies the hostname of the proxy you wish to tunnel network traffic through.</source>
        <translation>&lt;b&gt;プロキシのホスト名&lt;/b&gt;&lt;br /&gt;このフィールドにはネットワークトラフィックを通過させたいプロキシのホスト名を設定します。</translation>
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
        <source>Misc</source>
        <translation></translation>
    </message>
    <message>
        <location/>
        <source>Prevent log from downloading images</source>
        <translation>ログ上で画像のダウンロードをしない</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Disable image download&lt;/b&gt;&lt;br/&gt;
Prevents the client from downloading images embedded into chat messages with the img tag.</source>
        <translation>&lt;b&gt;画像のダウンロードを無効化&lt;/b&gt;&lt;br /&gt;
img タグでチャットメッセージに埋め込まれた画像のダウンロードを無効化します。</translation>
    </message>
    <message>
        <location/>
        <source>Disable image download</source>
        <translation>画像のダウンロードを無効にする</translation>
    </message>
    <message>
        <location/>
        <source>Mumble services</source>
        <translation>Mumble サービス</translation>
    </message>
    <message>
        <location/>
        <source>Check for new releases of Mumble automatically.</source>
        <translation>Mumbleの新しいリリースを自動的に確認する。</translation>
    </message>
    <message>
        <location/>
        <source>This will check for new releases of Mumble every time you start the program, and notify you if one is available.</source>
        <translation>プログラムを起動する度に Mumble の新しいバージョンを確認し、存在すれば通知します。</translation>
    </message>
    <message>
        <location/>
        <source>Check for application updates on startup</source>
        <translation>開始時に Mumble の更新をチェックする</translation>
    </message>
    <message>
        <location/>
        <source>Check for new releases of plugins automatically.</source>
        <translation>プラグインの新しいリリースを自動的に確認する。</translation>
    </message>
    <message>
        <location/>
        <source>This will check for new releases of plugins every time you start the program, and download them automatically.</source>
        <translation>プログラムが開始する度に新しいプラグインを確認し、存在すれば自動的にダウンロードする。</translation>
    </message>
    <message>
        <source>Download plugin updates on startup</source>
        <translation type="obsolete">開始時にプラグインのアップデートをダウンロードする</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Submit anonymous statistics.&lt;/b&gt;&lt;br /&gt;Mumble has a small development team, and as such needs to focus its development where it is needed most. By submitting a bit of statistics you help the project determine where to focus development.</source>
        <translation>&lt;b&gt;匿名で統計情報の送信&lt;/b&gt;Mumble の開発チームの規模は小さいので、必要なものに開発の焦点をしぼる必要があります。統計情報を送信することで、プロジェクトがユーザが頻繁に使う機能が何であるのかを知り、開発の焦点をどこに定めるかを決める手助けとなります。</translation>
    </message>
    <message>
        <source>Check for updates on startup</source>
        <translation type="obsolete">起動時に更新をチェックする</translation>
    </message>
    <message>
        <location/>
        <source>Submit anonymous statistics to the Mumble project</source>
        <translation>Mumbleプロジェクトに匿名で統計を送信</translation>
    </message>
    <message>
        <source>&lt;b&gt;Submit anonymous statistics.&lt;/b&gt;&lt;br /&gt;Mumble has a small development team, and as such needs to focus it&apos;s development where it&apos;s needed most. By submitting a bit of statistics you help the project determine where to focus development.</source>
        <translation type="obsolete">&lt;b&gt;匿名で統計を送信します。&lt;/b&gt;&lt;br /&gt;Mumbleは小さなチームで開発を行っているためもっとも必要な機能に絞って開発をしなければなりません。統計情報の送信はプロジェクトがどこに注力して開発すれば良いかを決定するための指針になります。</translation>
    </message>
    <message>
        <location/>
        <source>Submit anonymous statistics</source>
        <translation>匿名で利用状況の統計を送信する</translation>
    </message>
    <message>
        <location/>
        <source>Reconnect to last used server when starting Mumble</source>
        <translation>Mumbleを起動したときに最後に使っていたサーバに再接続します</translation>
    </message>
    <message>
        <location/>
        <source>Reconnect to last server on startup</source>
        <translation>起動時に最後に接続したサーバに再接続する</translation>
    </message>
    <message>
        <location/>
        <source>Download plugin and overlay updates on startup</source>
        <translation>開始時にプラグインとオーバレイの更新をダウンロードする</translation>
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
        <location filename="OverlayEditorScene.cpp" line="+121"/>
        <location filename="OverlayUser.cpp" line="+150"/>
        <source>Silent</source>
        <translation>発言していない</translation>
    </message>
    <message>
        <location line="+3"/>
        <location filename="OverlayUser.cpp" line="+3"/>
        <source>Talking</source>
        <translation>発言中</translation>
    </message>
    <message>
        <location line="+3"/>
        <location filename="OverlayUser.cpp" line="+3"/>
        <source>Whisper</source>
        <translation>ささやき</translation>
    </message>
    <message>
        <location line="+3"/>
        <location filename="OverlayUser.cpp" line="+3"/>
        <source>Shout</source>
        <translation>発言している</translation>
    </message>
    <message>
        <location line="+17"/>
        <location filename="OverlayUser.cpp" line="-110"/>
        <source>Channel</source>
        <translation>チャンネル</translation>
    </message>
    <message>
        <location filename="Overlay.cpp" line="+100"/>
        <source>Failed to create communication with overlay at %2: %1. No overlay will be available.</source>
        <translation>オーバレイ:  %2: %1 との通信の作成に失敗しました。有効なオーバレイはありません。</translation>
    </message>
    <message>
        <location line="+255"/>
        <source>Skipping overlay update in debug mode.</source>
        <translation>デバッグモードではオーバレイの更新をスキップします。</translation>
    </message>
    <message>
        <location line="+66"/>
        <source>Downloaded new or updated overlay support file to %1.</source>
        <translation>%1に新規か更新されたオーバレイサポートファイルをダウンロードしました。</translation>
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
    <message>
        <source>Failed to load overlay library. This means either that:
- the library (mumble_ol.dll) wasn&apos;t found in the directory you ran Mumble from
- you&apos;re on an OS earlier than WinXP SP2</source>
        <translation type="obsolete">オーバレイライブラリの読み込みに失敗しました. 次の原因が考えられます:
- ライブラリファイル（mumble_ol.dll)が Mumble の実行されているフォルダに存在しない
- WinXP SP2 より古い OS を使用している</translation>
    </message>
    <message>
        <source>Failed to initialize overlay memory. This usually means that the shared memory is locked by the OS, and you need to reboot to release it.</source>
        <translation type="obsolete">オーバーレイメモリの初期化に失敗しました。共有メモリが OS によってロックされていることが原因であり、それを解放するためには OS を再起動する必要があります。</translation>
    </message>
    <message>
        <source>Failed to create communication with overlay: %1. No overlay will be available.</source>
        <translation type="obsolete">オーバレイ: %1 との通信の作成に失敗しました。有効なオーバレイはありません。</translation>
    </message>
</context>
<context>
    <name>OverlayClient</name>
    <message>
        <location filename="OverlayUserGroup.cpp" line="+94"/>
        <source>Filter</source>
        <translation>フィルタ</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Only talking</source>
        <translation>話している人だけ</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>All in current channel</source>
        <translation>同じチャンネルにいる人全て</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>All in linked channels</source>
        <translation>リンクしているチャンネルにいる人全て</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Always show yourself</source>
        <translation>自分自身を常に表示する</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Columns</source>
        <translation>列</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Edit...</source>
        <translation>編集...</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Reset Zoom</source>
        <translation>ズームをリセット</translation>
    </message>
    <message>
        <location filename="OverlayClient.cpp" line="+108"/>
        <source>FPS: %1</source>
        <translation></translation>
    </message>
</context>
<context>
    <name>OverlayConfig</name>
    <message>
        <location filename="Overlay.ui"/>
        <source>Options</source>
        <translation>オプション</translation>
    </message>
    <message>
        <source>Position</source>
        <translation type="obsolete">位置</translation>
    </message>
    <message>
        <source>Form</source>
        <oldsource>Font</oldsource>
        <translation type="obsolete">フォーム</translation>
    </message>
    <message>
        <location/>
        <source>Enable Overlay</source>
        <translation>オーバーレイを有効にする</translation>
    </message>
    <message>
        <location/>
        <source>Enable overlay.</source>
        <translation>オーバーレイを有効にします。</translation>
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
        <translation type="obsolete">常に自分を表示する</translation>
    </message>
    <message>
        <source>Always show yourself on overlay.</source>
        <translation type="obsolete">オーバーレイで常に自分を表示します。</translation>
    </message>
    <message>
        <source>This sets whether to always show yourself or not. This setting is useful if you aren&apos;t showing everyone in the overlay, as then you would only see your own status if you were talking, which wouldn&apos;t let you see that you were deafened or muted.</source>
        <translation type="obsolete">自分自身を常に表示するかどうか決定します。この設定は全員表示にしていない時に効果的です。自分が話しているかの状況だけを確認することができます。あなたが発言禁止や聴取禁止になっている場合には表示されません。</translation>
    </message>
    <message>
        <source>Grow Left</source>
        <translation type="obsolete">左に延長する</translation>
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
        <translation type="obsolete">右に延長する</translation>
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
        <translation type="obsolete">上に延長する</translation>
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
        <translation type="obsolete">下に延長する</translation>
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
        <location filename="OverlayConfig.cpp" line="+78"/>
        <source>To move the users, drag the little red dot.</source>
        <translation>ユーザを動かすには、小さな赤い点をドラッグしてください。</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>To resize the users, mouse wheel over a user.</source>
        <oldsource>To resize the users, mousewheel over a user.</oldsource>
        <translation type="unfinished">ユーザのアバターのサイズを変えるには、ユーザの上でマウスホィールを動かしてください。</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>For more options, right click a user.</source>
        <oldsource>For more options, rightclick a user.</oldsource>
        <translation type="unfinished">更なる設定をするには、ユーザを右クリックしてください。</translation>
    </message>
    <message>
        <location line="+30"/>
        <source>FPS: %1</source>
        <translation></translation>
    </message>
    <message>
        <location line="+194"/>
        <source>Overlay</source>
        <translation>オーバーレイ</translation>
    </message>
    <message>
        <location line="+75"/>
        <source>Choose executable</source>
        <translation>実行ファイルを選択</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Choose application</source>
        <translation>アプリケーションを選んでください</translation>
    </message>
    <message>
        <location line="+38"/>
        <source>Mumble has deemed the installer valid.</source>
        <translation>Mumbleはインストーラが有効であると認識しました。</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Mumble was unable to verify the authenticity of the installer.</source>
        <oldsource>Mumble was unable to validate authenticity of the installer.</oldsource>
        <translation>Mumbleはインストーラの信頼性を検証できません。</translation>
    </message>
    <message>
        <location line="+87"/>
        <source>Load Overlay Presets</source>
        <translation>オーバレイプリセットを読み込み</translation>
    </message>
    <message>
        <location line="+2"/>
        <location line="+28"/>
        <source>Mumble overlay presets (*.mumblelay)</source>
        <translation>Mumbleオーバレイプリセット (*.mumblelay)</translation>
    </message>
    <message>
        <location line="-2"/>
        <source>Save Overlay Presets</source>
        <translation>オーバレイのプリセットを保存</translation>
    </message>
    <message>
        <source>Show User Textures</source>
        <translation type="obsolete">ユーザテクスチャを表示する</translation>
    </message>
    <message>
        <source>Show User custom textures instead of text on the overlay.</source>
        <translation type="obsolete">テキストの代わりにユーザのカスタムテクスチャをオーバーレイに表示します。</translation>
    </message>
    <message>
        <source>This sets whether to download and use custom textures for registered users. If disabled, the regular outline text will be used instead.</source>
        <translation type="obsolete">登録されたユーザのカスタムテクスチャをダウンロードして使用するかどうかを決定します。無効の時は通常のテキストが代わりに使われます。</translation>
    </message>
    <message>
        <source>Maximum height of names.</source>
        <translation type="obsolete">名前の最大の高さです。</translation>
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
        <source>Color for users</source>
        <translation type="obsolete">ユーザの色</translation>
    </message>
    <message>
        <source>Color for talking users</source>
        <translation type="obsolete">しゃべっているユーザの色</translation>
    </message>
    <message>
        <source>Color for whispering users</source>
        <translation type="obsolete">ささやいているユーザの色</translation>
    </message>
    <message>
        <source>Color for channels</source>
        <translation type="obsolete">チャンネルの色</translation>
    </message>
    <message>
        <source>Color for active channels</source>
        <translation type="obsolete">有効なチャンネルの色</translation>
    </message>
    <message>
        <location filename="Overlay.ui"/>
        <source>This sets whether the overlay is enabled or not. This setting is only checked when applications are started, so make sure Mumble is running and this option is on before you start the application.&lt;br /&gt;Please note that if you start Mumble after starting the application, or if you disable the overlay while the application is running, there is no safe way to restart the overlay without also restarting the application.</source>
        <oldsource>This sets whether the overlay is enabled or not. This setting is only checked when applications are started, so make sure Mumble is running and this option is on before you start the application.&lt;br /&gt;Please note that if you start the application after starting Mumble, or if you disable the overlay while running, there is no safe way to restart the overlay without also restarting the application.</oldsource>
        <translation>これはオーバレイを有功にするかそうでないかを設定します。この設定はアプリケーションが開始したときにのみチェックされます。Mumble が動作中であることと、アプリケーションが開始する前にこのオプションが有効になっていることを確認してください。&lt;br /&gt;Mumble が開始した後にアプリケーションを開始しても、起動中にオーバレイを無効にしても、アプリケーションを際スタートしないとオーバレイを再開する安全な方法ではありません。</translation>
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
    <message>
        <location/>
        <source>Layout</source>
        <translation>レイアウト</translation>
    </message>
    <message>
        <location/>
        <source>Overlay exceptions</source>
        <translation>オーバレイの例外設定</translation>
    </message>
    <message>
        <location/>
        <source>Blacklist</source>
        <translation>ブラックリスト</translation>
    </message>
    <message>
        <location/>
        <source>Whitelist</source>
        <translation>ホワイトリスト</translation>
    </message>
    <message>
        <source>Add</source>
        <translation type="obsolete">追加</translation>
    </message>
    <message>
        <location/>
        <source>Remove</source>
        <translation>削除</translation>
    </message>
    <message>
        <location/>
        <source>Uninstall Overlay</source>
        <translation>オーバレイをアンインストール</translation>
    </message>
    <message>
        <location/>
        <source>Overlay Installation</source>
        <translation>オーバレイのインストール</translation>
    </message>
    <message>
        <location/>
        <source>Mumble has detected that you do not have the Mumble Overlay installed.

Click the button below to install the overlay.</source>
        <translation>Mumble は Mumble オーバレイをインストールされていないことを検知しました。

下のボタンをクリックしてオーバレイをインストールしてください。</translation>
    </message>
    <message>
        <location/>
        <source>Install Mumble Overlay</source>
        <translation>Mumble オーバレイのインストール</translation>
    </message>
    <message>
        <location/>
        <source>Overlay Upgrade</source>
        <translation>オーバレイのアップグレード</translation>
    </message>
    <message>
        <location/>
        <source>Mumble has detected an old version of the overlay support files installed on your computer.

To upgrade these files to their latest versions, click the button below.</source>
        <translation>Mumble は古いバージョンのオーバレイサポートファイルがインストールされていることを検知しました。

これらのファイルを最新バージョンにアップグレードするために次のボタンを押してください。</translation>
    </message>
    <message>
        <location/>
        <source>Upgrade Mumble Overlay</source>
        <translation>Mumble オーバレイのアップグレード</translation>
    </message>
    <message>
        <location/>
        <source>Display a frame counter in the overlay</source>
        <translation>オーバレイ上にフレームカウンタを表示します</translation>
    </message>
    <message>
        <location/>
        <source>Show FPS counter</source>
        <translation>FPSカウンタを表示する</translation>
    </message>
    <message>
        <location/>
        <source>View Certificates</source>
        <translation>証明書を見る</translation>
    </message>
    <message>
        <source>&amp;Add...</source>
        <translation type="obsolete">追加(&amp;A)...</translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation type="obsolete">削除(&amp;R)</translation>
    </message>
    <message>
        <location/>
        <source>Load an overlay preset from file</source>
        <translation>ファイルからオーバレイのプリセットを読み込みます</translation>
    </message>
    <message utf8="true">
        <location/>
        <source>Load…</source>
        <translation>読込…</translation>
    </message>
    <message>
        <location/>
        <source>Save your overlay settings to file</source>
        <translation>ファイルにオーバレイの設定を保存します</translation>
    </message>
    <message utf8="true">
        <location/>
        <source>Save…</source>
        <translation>保存…</translation>
    </message>
    <message>
        <location/>
        <source>FPS Display</source>
        <translation>FPS表示</translation>
    </message>
    <message>
        <location/>
        <source>Set the overlay font.</source>
        <translation>オーバレイのフォントを設定します。</translation>
    </message>
    <message>
        <location/>
        <source>Font</source>
        <translation>フォント</translation>
    </message>
    <message>
        <location/>
        <source>Set the overlay text color.</source>
        <translation>オーバレイテキストの色を設定します。</translation>
    </message>
    <message>
        <location/>
        <source>Color</source>
        <translation>色</translation>
    </message>
    <message>
        <location/>
        <source>Add...</source>
        <translation>追加...</translation>
    </message>
</context>
<context>
    <name>OverlayEditor</name>
    <message>
        <source>Dialog</source>
        <translation type="obsolete">ダイアログ</translation>
    </message>
    <message>
        <location filename="OverlayEditor.ui"/>
        <source>State</source>
        <translation>状態</translation>
    </message>
    <message>
        <location/>
        <source>User is not talking</source>
        <translation>ユーザは話していない</translation>
    </message>
    <message>
        <location/>
        <source>Passive</source>
        <translation>受身</translation>
    </message>
    <message>
        <location/>
        <source>User is talking in your channel or a linked channel</source>
        <translation>ユーザはあなたのいるチャンネルかリンクしているチャンネルで話しています</translation>
    </message>
    <message>
        <location/>
        <source>Talking</source>
        <translation>発言中</translation>
    </message>
    <message>
        <location/>
        <source>User is whispering to you privately</source>
        <translation>ユーザはあなたに個人的にささやいています</translation>
    </message>
    <message>
        <location/>
        <source>Private Whisper</source>
        <translation>個人的なささやき</translation>
    </message>
    <message>
        <location/>
        <source>User is shouting to your channel</source>
        <translation>ユーザはあなたのいるチャンネルに叫んでいます</translation>
    </message>
    <message>
        <location/>
        <source>Channel Whisper</source>
        <translation>チャンネルへのささやき</translation>
    </message>
    <message>
        <location/>
        <source>Zoom</source>
        <translation>ズーム</translation>
    </message>
    <message>
        <location/>
        <source>Zoom Factor</source>
        <translation>拡大する要素</translation>
    </message>
    <message>
        <location/>
        <source>Enabled Elements</source>
        <translation>有効な要素</translation>
    </message>
    <message>
        <location/>
        <source>User avatar, chosen by each user</source>
        <translation>ユーザのアバター、それぞれのユーザによって選択されたものです</translation>
    </message>
    <message>
        <location/>
        <source>Avatar</source>
        <translation>アバター</translation>
    </message>
    <message>
        <location/>
        <source>User&apos;s name</source>
        <translation>ユーザの名前</translation>
    </message>
    <message>
        <location/>
        <source>Username</source>
        <translation>ユーザ名</translation>
    </message>
    <message>
        <location/>
        <source>Name of user&apos;s channel, if outside your current channel</source>
        <translation>ユーザのチャンネルの名前、あなたのいるチャンネルの外にいる場合</translation>
    </message>
    <message>
        <location/>
        <source>Channel</source>
        <translation>チャンネル</translation>
    </message>
    <message>
        <location/>
        <source>Muted or deafened</source>
        <translation>発言禁止か聴取禁止</translation>
    </message>
    <message>
        <location/>
        <source>Mute state</source>
        <translation>発言禁止状態</translation>
    </message>
    <message>
        <location/>
        <source>Bounding box, automatically shrunk to minimum size to contain all visible elements</source>
        <translation>バウンディグボックス、全ての可視要素を含む最小のサイズに自動的に縮みます</translation>
    </message>
    <message>
        <location/>
        <source>Bounding box</source>
        <translation>バウンディングボックス</translation>
    </message>
    <message>
        <location/>
        <source>Overlay Editor</source>
        <translation>オーバレイ エディタ</translation>
    </message>
</context>
<context>
    <name>OverlayEditorScene</name>
    <message>
        <location filename="OverlayEditorScene.cpp" line="+396"/>
        <source>Layout preset</source>
        <translation>レイアウトプリセット</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Large square avatar</source>
        <translation>大きな四角のアバター</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Avatar and Name</source>
        <translation>アバターと名前</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>User Opacity</source>
        <translation>ユーザの不透明度</translation>
    </message>
    <message>
        <location line="+6"/>
        <location line="+31"/>
        <source>%1%</source>
        <translation>%1%</translation>
    </message>
    <message>
        <location line="-5"/>
        <source>Object Opacity</source>
        <translation>オブジェクトの不透明度</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Alignment</source>
        <translation>位置付け</translation>
    </message>
    <message>
        <location line="+11"/>
        <source>Left</source>
        <translation>左</translation>
    </message>
    <message>
        <location line="+5"/>
        <location line="+18"/>
        <source>Center</source>
        <translation>中央</translation>
    </message>
    <message>
        <location line="-13"/>
        <source>Right</source>
        <translation>右</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Top</source>
        <translation>上</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Bottom</source>
        <translation>下</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Color...</source>
        <translation>色...</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Font...</source>
        <translation>フォント...</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Bounding box</source>
        <translation>バウンディングボックス</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Pen width</source>
        <translation>ペン幅</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Padding</source>
        <translation>パディング</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Pen color</source>
        <translation>ペンの色</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Fill color</source>
        <translation>塗りつぶしの色</translation>
    </message>
    <message>
        <location line="+87"/>
        <source>Pick pen color</source>
        <translation>ペンの色を取得</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Pick fill color</source>
        <translation>塗りつぶしの色を取得</translation>
    </message>
    <message>
        <location line="+14"/>
        <source>Pick color</source>
        <translation>色の選択</translation>
    </message>
    <message>
        <location line="+21"/>
        <source>Pick font</source>
        <translation>フォントの選択</translation>
    </message>
</context>
<context>
    <name>OverlayPrivateWin</name>
    <message>
        <source>Mumble</source>
        <translation type="obsolete">Mumble</translation>
    </message>
    <message>
        <location filename="Overlay_win.cpp" line="+62"/>
        <source>Failed to load overlay library. This means either that:
- the library (mumble_ol.dll) wasn&apos;t found in the directory you ran Mumble from
- you&apos;re on an OS earlier than WinXP SP2</source>
        <translation>オーバレイライブラリの読み込みに失敗しました. 次の原因が考えられます:
- ライブラリファイル（mumble_ol.dll)が Mumble の実行されているフォルダに存在しない
- WinXP SP2 より古い OS を使用している</translation>
    </message>
</context>
<context>
    <name>OverlayUser</name>
    <message>
        <source>Channel</source>
        <translation type="obsolete">チャンネル</translation>
    </message>
    <message>
        <source>Silent</source>
        <translation type="obsolete">静か</translation>
    </message>
    <message>
        <source>Talking</source>
        <translation type="obsolete">発言中</translation>
    </message>
    <message>
        <source>Whisper</source>
        <translation type="obsolete">ささやき</translation>
    </message>
    <message>
        <source>Shout</source>
        <translation type="obsolete">叫んでいる</translation>
    </message>
    <message>
        <source>User Opacity</source>
        <translation type="obsolete">ユーザの不透明度</translation>
    </message>
    <message>
        <source>%1%</source>
        <translation type="obsolete">%1%</translation>
    </message>
    <message>
        <source>Object Opacity</source>
        <translation type="obsolete">オブジェクトの不透明度</translation>
    </message>
    <message>
        <source>Color...</source>
        <translation type="obsolete">色...</translation>
    </message>
    <message>
        <source>Font...</source>
        <translation type="obsolete">フォント...</translation>
    </message>
    <message>
        <source>Pick color</source>
        <translation type="obsolete">色の選択</translation>
    </message>
    <message>
        <source>Pick font</source>
        <translation type="obsolete">フォントの選択</translation>
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
        <translation type="obsolete">サーバに接続しているプレイヤーです。プレイヤーの左のアイコンはその人が話しているかどうかを示しています:&lt;br /&gt;&lt;img src=&quot;:/icons/talking_on.png&quot; /&gt; 話している&lt;br /&gt;&lt;img src=&quot;:/icons/talking_off.png&quot; /&gt; 話していない</translation>
    </message>
    <message>
        <source>This is a channel on the server. Only players in the same channel can hear each other.</source>
        <translation type="obsolete">サーバにあるチャンネルです。同じチャンネルにいるプレイヤーしかお互いの会話を聞くことはできません。</translation>
    </message>
    <message>
        <source>This shows the flags the player has on the server, if any:&lt;br /&gt;&lt;img src=&quot;:/icons/authenticated.png&quot; /&gt;Authenticated user&lt;br /&gt;&lt;img src=&quot;:/icons/muted_self.png&quot; /&gt;Muted (by self)&lt;br /&gt;&lt;img src=&quot;:/icons/muted_server.png&quot; /&gt;Muted (by admin)&lt;br /&gt;&lt;img src=&quot;:/icons/deafened_self.png&quot; /&gt;Deafened (by self)&lt;br /&gt;&lt;img src=&quot;:/icons/deafened_server.png&quot; /&gt;Deafened (by admin)&lt;br /&gt;A player muted by himself is probably just away, talking on the phone or something like that.&lt;br /&gt;A player muted by an admin is probably also just away, and the noise the player is making was annoying enough that an admin muted him.</source>
        <translation type="obsolete">サーバ内のプレイヤーが持っているフラグを表示します:&lt;br /&gt;&lt;img src=&quot;:/icons/authenticated.png&quot; /&gt;認証されたユーザ&lt;br /&gt;&lt;img src=&quot;:/icons/muted_self.png&quot; /&gt;発言禁止(自分によって)&lt;br /&gt;&lt;img src=&quot;:/icons/muted_server.png&quot; /&gt;発言禁止(管理者によって)&lt;br /&gt;&lt;img src=&quot;:/icons/deafened_self.png&quot; /&gt;聴取禁止(自分によって)&lt;br /&gt;&lt;img src=&quot;:/icons/deafened_server.png&quot; /&gt;聴取禁止(管理者によって)&lt;br /&gt;自分で発言禁止にしているプレイヤーは、おそらくちょっと離席しているだけでしょう。&lt;br /&gt;管理者によって発言禁止にされているプレイヤーは同じく離席しているだけか、管理者が発言禁止するに値するほどその人の発する雑音がうるさかったのでしょう。</translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="obsolete">名前</translation>
    </message>
    <message>
        <source>Flags</source>
        <translation type="obsolete">フラグ</translation>
    </message>
    <message>
        <source>This is a player connected to the server. The icon to the left of the player indicates whether or not they are talking:&lt;br /&gt;&lt;img src=&quot;skin:talking_on.png&quot; /&gt; Talking&lt;br /&gt;&lt;img src=&quot;skin:talking_off.png&quot; /&gt; Not talking</source>
        <translation type="obsolete">サーバに接続しているプレイヤーです。プレイヤーの左のアイコンはその人が話しているかどうかを示しています:&lt;br /&gt;&lt;img src=&quot;skin:talking_on.png&quot; /&gt; 話している&lt;br /&gt;&lt;img src=&quot;skin:talking_off.png&quot; /&gt; 話していない</translation>
    </message>
    <message>
        <source>This shows the flags the player has on the server, if any:&lt;br /&gt;&lt;img src=&quot;skin:authenticated.png&quot; /&gt;Authenticated user&lt;br /&gt;&lt;img src=&quot;skin:muted_self.png&quot; /&gt;Muted (by self)&lt;br /&gt;&lt;img src=&quot;skin:muted_server.png&quot; /&gt;Muted (by admin)&lt;br /&gt;&lt;img src=&quot;skin:deafened_self.png&quot; /&gt;Deafened (by self)&lt;br /&gt;&lt;img src=&quot;skin:deafened_server.png&quot; /&gt;Deafened (by admin)&lt;br /&gt;A player muted by himself is probably just away, talking on the phone or something like that.&lt;br /&gt;A player muted by an admin is probably also just away, and the noise the player is making was annoying enough that an admin muted him.</source>
        <translation type="obsolete">サーバ内のプレイヤーが持っているフラグを表示します:&lt;br /&gt;&lt;img src=&quot;skin:authenticated.png&quot; /&gt;認証されたユーザ&lt;br /&gt;&lt;img src=&quot;skin:muted_self.png&quot; /&gt;発言禁止(自分によって)&lt;br /&gt;&lt;img src=&quot;skin:muted_server.png&quot; /&gt;発言禁止(管理者によって)&lt;br /&gt;&lt;img src=&quot;skin:deafened_self.png&quot; /&gt;聴取禁止(自分によって)&lt;br /&gt;&lt;img src=&quot;skin:deafened_server.png&quot; /&gt;聴取禁止(管理者によって)&lt;br /&gt;自分で発言禁止にしているプレイヤーは、おそらくちょっと離席しているだけでしょう。&lt;br /&gt;管理者によって発言禁止にされているプレイヤーは同じく離席しているだけか、管理者が発言禁止するに値するほどその人の発する雑音がうるさかったのでしょう。</translation>
    </message>
    <message>
        <source>Mumble</source>
        <translation type="obsolete">Mumble</translation>
    </message>
    <message>
        <source>Are you sure you want to drag this channel?</source>
        <translation type="obsolete">本当にこのチャンネルをドラッグしますか？</translation>
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
        <location filename="Plugins.cpp" line="+75"/>
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
        <source>This enables plugins for supported games to fetch your in-game position and transmit that with each voice packet. This enables other players to hear your voice in-game from the direction your character is in relation to their own.</source>
        <translation type="obsolete">この設定は、サポートされたゲームのプラグインが位置情報を取得して、音声パケットと共に送信することを可能にします。他のプレイヤーから見て、ゲーム内であなたのキャラクターがいる方向から声が聞こえるようにすることができます。</translation>
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
        <source>This allows plugins for supported games to fetch your in-game position and transmit it with each voice packet. This enables other users to hear your voice in-game from the direction your character is in relation to their own.</source>
        <translation>この設定は、サポートされたゲームのプラグインが位置情報を取得して、音声パケットと共に送信することを可能にします。他のユーザから見て、ゲーム内であなたのキャラクタがいる方向から声が聞こえるようにすることができます。</translation>
    </message>
    <message>
        <location/>
        <source>This rescans and reloads plugins. Use this if you just added or changed a plugin to the plugins directory.</source>
        <translation>プラグインを再読み込みします。プラグインのディレクトリに追加したり、変更を行ったときに使用してください。</translation>
    </message>
    <message>
        <location/>
        <source>&amp;About</source>
        <translation>概要(&amp;A)</translation>
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
        <location filename="Plugins.cpp" line="+56"/>
        <source>Plugin has no configure function.</source>
        <translation>プラグインに設定機能がありません。</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Plugin has no about function.</source>
        <translation>プラグインの設定項目がありません。</translation>
    </message>
    <message>
        <location filename="Plugins.ui"/>
        <source>Form</source>
        <translation>フォーム</translation>
    </message>
    <message>
        <location/>
        <source>Name</source>
        <translation>名前</translation>
    </message>
    <message>
        <location/>
        <source>Enabled</source>
        <translation>有効化</translation>
    </message>
</context>
<context>
    <name>Plugins</name>
    <message>
        <location filename="Plugins.cpp" line="+330"/>
        <source>Skipping plugin update in debug mode.</source>
        <translation>デバッグモードではプラグインのアップデートはスキップします。</translation>
    </message>
    <message>
        <location line="+155"/>
        <location line="+6"/>
        <source>Downloaded new or updated plugin to %1.</source>
        <translation>更新されたプラグインを %1 にダウンロードしました。</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Failed to install new plugin to %1.</source>
        <translation>%1 に新しいプラグインをインストールできません。</translation>
    </message>
    <message>
        <source>Plugin %1 lost link.</source>
        <translation type="obsolete">プラグイン %1 はリンクを失いました。</translation>
    </message>
    <message>
        <location line="-265"/>
        <source>%1 lost link.</source>
        <translation>%1 はリンクを失いました。</translation>
    </message>
    <message>
        <source>Plugin %1 linked.</source>
        <translation type="obsolete">プラグイン %1 はリンクされました。</translation>
    </message>
    <message>
        <location line="+74"/>
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
    <name>RichTextEditor</name>
    <message>
        <location filename="RichTextEditor.cpp" line="+241"/>
        <source>Failed to load image</source>
        <translation>画像の読み込みに失敗</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Image file too large to embed in document. Please use images smaller than %1 kB.</source>
        <translation>ドキュメントに埋め込むには画像ファイルが大きすぎます。 %1 kB より小さい画像を使用してください。</translation>
    </message>
    <message>
        <location line="+81"/>
        <source>Message is too long.</source>
        <translation>メッセージが長すぎます。</translation>
    </message>
    <message>
        <source>TabWidget</source>
        <translation type="obsolete">タブウィジット</translation>
    </message>
    <message>
        <location filename="RichTextEditor.ui"/>
        <source>Display</source>
        <translation>表示</translation>
    </message>
    <message>
        <location/>
        <source>Source Text</source>
        <translation>ソーステキスト</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Bold</source>
        <translation>太字体(&amp;B)</translation>
    </message>
    <message>
        <location/>
        <source>Ctrl+B</source>
        <translation></translation>
    </message>
    <message>
        <location/>
        <source>&amp;Italic</source>
        <translation>イタリック体(&amp;I)</translation>
    </message>
    <message>
        <location/>
        <source>Italic</source>
        <translation>イタリック体</translation>
    </message>
    <message>
        <location/>
        <source>Ctrl+I</source>
        <translation></translation>
    </message>
    <message>
        <location/>
        <source>Underline</source>
        <translation>下線</translation>
    </message>
    <message>
        <location/>
        <source>Ctrl+U</source>
        <translation></translation>
    </message>
    <message>
        <location/>
        <source>Color</source>
        <translation>色</translation>
    </message>
    <message>
        <location/>
        <source>Insert Link</source>
        <translation>リンクを挿入</translation>
    </message>
    <message>
        <location/>
        <source>Ctrl+L</source>
        <translation></translation>
    </message>
    <message>
        <location/>
        <source>Insert Image</source>
        <translation>画像を挿入</translation>
    </message>
</context>
<context>
    <name>RichTextEditorLink</name>
    <message>
        <location filename="RichTextEditorLink.ui"/>
        <source>Add Link</source>
        <translation>リンクを追加</translation>
    </message>
    <message>
        <location/>
        <source>URL</source>
        <translation></translation>
    </message>
    <message>
        <location/>
        <source>Text</source>
        <translation>テキスト</translation>
    </message>
</context>
<context>
    <name>ServerHandler</name>
    <message>
        <location filename="ServerHandler.cpp" line="+401"/>
        <source>UDP packets cannot be sent to or received from the server. Switching to TCP mode.</source>
        <translation>UDP パケットをサーバに送ることができないか、サーバから受けとることができません。TCPモードに切り替えます。</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>UDP packets cannot be sent to the server. Switching to TCP mode.</source>
        <translation>UDPパケットをサーバに送ることができません。TCPモードに切り替えます。</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>UDP packets cannot be received from the server. Switching to TCP mode.</source>
        <translation>UDPパケットをサーバから受けとることができません。TCPモードに切り替えます。</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>UDP packets can be sent to and received from the server. Switching back to UDP mode.</source>
        <translation>UDPパケットのサーバへの送受信ができるようになりました。UDPモードに切り替えます。</translation>
    </message>
    <message>
        <location line="+36"/>
        <source>Connection timed out</source>
        <translation>接続がタイムアウトになりました</translation>
    </message>
</context>
<context>
    <name>ServerView</name>
    <message>
        <location filename="ConnectDialog.cpp" line="-611"/>
        <source>Favorite</source>
        <translation>お気に入り</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>LAN</source>
        <translation>ローカル</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Public Internet</source>
        <translation>インターネット</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Africa</source>
        <translation>アフリカ</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Asia</source>
        <translation>アジア</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>North America</source>
        <translation>北アメリカ</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>South America</source>
        <translation>南アメリカ</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Europe</source>
        <translation>ヨーロッパ</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Oceania</source>
        <translation>オセアニア</translation>
    </message>
</context>
<context>
    <name>ShortcutActionWidget</name>
    <message>
        <location filename="GlobalShortcut.cpp" line="-483"/>
        <source>Unassigned</source>
        <translation>未割り当て</translation>
    </message>
</context>
<context>
    <name>ShortcutDelegate</name>
    <message>
        <location line="+361"/>
        <source>On</source>
        <translation>オン</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Off</source>
        <translation>オフ</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Toggle</source>
        <translation>切り替え</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Unassigned</source>
        <translation>未割り当て</translation>
    </message>
</context>
<context>
    <name>ShortcutKeyWidget</name>
    <message>
        <location line="-435"/>
        <source>Press Shortcut</source>
        <translation>ショートカットを押下</translation>
    </message>
</context>
<context>
    <name>ShortcutTargetDialog</name>
    <message>
        <location line="+206"/>
        <source>Root</source>
        <translation>ルート</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Parent</source>
        <translation>親</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Current</source>
        <translation>現在</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Subchannel #%1</source>
        <translation>サブチャンネル #%1</translation>
    </message>
</context>
<context>
    <name>ShortcutTargetWidget</name>
    <message>
        <location line="+85"/>
        <source>...</source>
        <translation></translation>
    </message>
    <message>
        <location line="+38"/>
        <source>, </source>
        <translation></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Root</source>
        <translation>ルート</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Parent</source>
        <translation>親</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Current</source>
        <translation>現在</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Subchannel #%1</source>
        <translation>サブチャンネル #%1</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Invalid</source>
        <translation>無効</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Empty</source>
        <oldsource>&lt;Empty&gt;</oldsource>
        <translation type="unfinished">&lt;空欄&gt;</translation>
    </message>
</context>
<context>
    <name>ShortcutToggleWidget</name>
    <message>
        <location line="-259"/>
        <source>Off</source>
        <translation>オフ</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Toggle</source>
        <translation>切り替え</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>On</source>
        <translation>オン</translation>
    </message>
</context>
<context>
    <name>TextMessage</name>
    <message>
        <source>Message to send</source>
        <translation type="obsolete">送信するメッセージ</translation>
    </message>
    <message>
        <source>Preview</source>
        <translation type="obsolete">プレビュー</translation>
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
        <location filename="TextMessage.h" line="+46"/>
        <source>Enter text</source>
        <translation>テキストを入力</translation>
    </message>
    <message>
        <location filename="TextMessage.ui"/>
        <source>If checked the message is recursively sent to all subchannels</source>
        <translation>チェックすると全てのサブチャンネルに再帰的にメッセージが送信されます</translation>
    </message>
    <message>
        <location/>
        <source>Send recursively to subchannels</source>
        <translation>再帰的にサブチャンネルに送る</translation>
    </message>
</context>
<context>
    <name>Tokens</name>
    <message>
        <location filename="Tokens.cpp" line="+69"/>
        <source>Empty Token</source>
        <translation>空のトークン</translation>
    </message>
    <message>
        <location filename="Tokens.ui"/>
        <source>Mumble - Access Tokens</source>
        <translation>Mumble - アクセストークン</translation>
    </message>
    <message>
        <location/>
        <source>List of access tokens on current server</source>
        <translation>現在のサーバのアクセストークンのリスト</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is an editable list of access tokens on the connected server.&lt;/b&gt;
&lt;br /&gt;
An access token is a text string, which can be used as a password for very simple access management on channels. Mumble will remember the tokens you&apos;ve used and resend them to the server next time you reconnect, so you don&apos;t have to enter these every time.
</source>
        <translation>&lt;b&gt;接続しているサーバのアクセストークンを編集することができます。&lt;/b&gt;
&lt;br /&gt;
アクセストークンはチャンネルへのアクセスをとても単純に管理するパスワードとして使うことが可能な文字列です。 Mumble はあなたが使ったトークンを記録して、次にサーバに接続した際に送り直します。つまり、毎回入力する必要は無いのです。</translation>
    </message>
    <message>
        <location/>
        <source>Add a token</source>
        <translation>トークンを追加</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Add</source>
        <translation>追加(&amp;A)</translation>
    </message>
    <message>
        <location/>
        <source>Remove a token</source>
        <translation>トークンを削除</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Remove</source>
        <translation>削除(&amp;R)</translation>
    </message>
</context>
<context>
    <name>UserEdit</name>
    <message>
        <location filename="UserEdit.ui"/>
        <source>Registered Users</source>
        <translation>登録済みのユーザ</translation>
    </message>
    <message>
        <location/>
        <location filename="UserEdit.cpp" line="+96"/>
        <source>Remove</source>
        <translation>削除</translation>
    </message>
    <message>
        <location filename="UserEdit.cpp" line="-5"/>
        <source>Rename</source>
        <translation>名前を変更</translation>
    </message>
</context>
<context>
    <name>UserInformation</name>
    <message>
        <location filename="UserInformation.ui"/>
        <source>User Information</source>
        <translation>ユーザ情報</translation>
    </message>
    <message>
        <location/>
        <source>Connection Information</source>
        <translation>接続情報</translation>
    </message>
    <message>
        <location/>
        <source>Version</source>
        <translation>バージョン</translation>
    </message>
    <message>
        <location/>
        <source>OS</source>
        <translation></translation>
    </message>
    <message>
        <location/>
        <source>Certificate</source>
        <translation>証明書</translation>
    </message>
    <message>
        <location/>
        <source>IP Address</source>
        <translation>IPアドレス</translation>
    </message>
    <message>
        <location/>
        <source>CELT Versions</source>
        <translation>CELTバージョン</translation>
    </message>
    <message>
        <location/>
        <source>Details...</source>
        <translation>詳細...</translation>
    </message>
    <message>
        <location/>
        <source>Ping Statistics</source>
        <translation>Ping 統計</translation>
    </message>
    <message>
        <location/>
        <source>Pings received</source>
        <translation>受け取ったPing</translation>
    </message>
    <message>
        <location/>
        <source>Average ping</source>
        <translation>平均ping</translation>
    </message>
    <message>
        <location/>
        <source>TCP (Control)</source>
        <translation>TCP (制御)</translation>
    </message>
    <message>
        <location/>
        <source>UDP (Voice)</source>
        <translation>UDP (音声)</translation>
    </message>
    <message>
        <location/>
        <source>UDP Network statistics</source>
        <translation>UDP ネットワーク統計</translation>
    </message>
    <message>
        <location/>
        <source>Good</source>
        <translation>良好</translation>
    </message>
    <message>
        <location/>
        <source>Late</source>
        <translation>遅延</translation>
    </message>
    <message>
        <location/>
        <source>Lost</source>
        <translation>消失</translation>
    </message>
    <message>
        <location/>
        <source>Resync</source>
        <translation>再同期</translation>
    </message>
    <message>
        <location/>
        <source>From Client</source>
        <translation>クライアントから</translation>
    </message>
    <message>
        <location/>
        <source>To Client</source>
        <translation>クライアント宛</translation>
    </message>
    <message>
        <source>Bandwidth</source>
        <translation type="obsolete">ネットワーク帯域</translation>
    </message>
    <message>
        <location/>
        <source>Connection time</source>
        <translation>接続時間</translation>
    </message>
    <message>
        <location filename="UserInformation.cpp" line="+93"/>
        <source>%1w</source>
        <translation>%1週間</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1d</source>
        <translation>%1日</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1h</source>
        <translation>%1時間</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1m</source>
        <translation>%1分</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>%1s</source>
        <translation>%1秒</translation>
    </message>
    <message>
        <location line="+29"/>
        <location line="+43"/>
        <source>, </source>
        <translation></translation>
    </message>
    <message>
        <location line="-13"/>
        <source>%1.%2.%3 (%4)</source>
        <translation></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>%1 (%2)</source>
        <translation></translation>
    </message>
    <message>
        <location line="+45"/>
        <source>%1 online (%2 idle)</source>
        <translation>オンライン時間 %1  (アイドル時間 %2)</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1 online</source>
        <translation>オンライン時間 %1</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>%1 kbit/s</source>
        <translation></translation>
    </message>
    <message>
        <location filename="UserInformation.ui"/>
        <source>Bandwidth</source>
        <comment>GroupBox</comment>
        <translation>ネットワーク帯域</translation>
    </message>
    <message>
        <location/>
        <source>Bandwidth</source>
        <comment>Label</comment>
        <translation>ネットワーク帯域</translation>
    </message>
</context>
<context>
    <name>UserModel</name>
    <message>
        <location filename="UserModel.cpp" line="-804"/>
        <source>This is a user connected to the server. The icon to the left of the user indicates whether or not they are talking:</source>
        <translation>サーバに接続しているユーザです。ユーザの左側のアイコンは彼らが話しているかどうかを意味します:</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Talking to your channel.</source>
        <translation>あなたのいるチャンネルに話しています。</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Shouting directly to your channel.</source>
        <oldsource>Whispering directly to your channel.</oldsource>
        <translation>チャンネルに直接ささやいています。</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Whispering directly to you.</source>
        <translation>あなたに直接ささやいています。</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Not talking.</source>
        <translation>話していません。</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>This is a channel on the server. The icon indicates the state of the channel:</source>
        <translation>サーバ上のチャンネルです。アイコンはチャンネルの状態を意味します :</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Your current channel.</source>
        <translation>現在のチャンネル。</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>A channel that is linked with your channel. Linked channels can talk to each other.</source>
        <translation>あなたのチャンネルとリンクしているチャンネルです。リンクしているチャンネルは互いに話せます。</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>A channel on the server that you are not linked to.</source>
        <translation>あなたがリンクしていないサーバ上のチャンネルです。</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>This shows the flags the user has on the server, if any:</source>
        <translation>ユーザがサーバ上で持っているフラグを表示します:</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>On your friend list</source>
        <translation>友人リスト上</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Authenticated user</source>
        <translation>認証されたユーザ</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Muted (manually muted by self)</source>
        <translation>発言禁止（自身で発言を禁止に）</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Muted (manually muted by admin)</source>
        <translation>発言禁止（管理者によって発言禁止に）</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Muted (not allowed to speak in current channel)</source>
        <translation>発言禁止（このチャンネルで発言が許されていない）</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Muted (muted by you, only on your machine)</source>
        <translation>発言禁止（自分で発言禁止にし、またあなたのマシンのみ）</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Deafened (by self)</source>
        <translation>聴取禁止（自身により）</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Deafened (by admin)</source>
        <translation>聴取禁止（管理者により）</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>User has a new comment set (click to show)</source>
        <translation>ユーザは新しいコメントを持っています（見るためにはクリックしてください）</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>User has a comment set, which you&apos;ve already seen. (click to show)</source>
        <translation>ユーザはあなたが既に見たコメントを持っています。（見るにはクリックしてください）</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>This shows the flags the channel has, if any:</source>
        <translation>チャンネルが持っているフラグを表示します:</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Channel has a new comment set (click to show)</source>
        <translation>チャンネルは新しいコメントをもっています。（見るためにはクリックしてください）</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Channel has a comment set, which you&apos;ve already seen. (click to show)</source>
        <translation>チャンネルはあなたが既に見たコメントを持っています。（見るためにはクリックしてください）</translation>
    </message>
    <message>
        <location line="+18"/>
        <source>Name</source>
        <translation>名前</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Flags</source>
        <translation>フラグ</translation>
    </message>
    <message>
        <location line="+721"/>
        <source>Are you sure you want to drag this channel?</source>
        <translation>本当にこのチャンネルをドラッグしますか ?</translation>
    </message>
    <message>
        <location line="+93"/>
        <location line="+20"/>
        <source>Cannot perform this movement automatically, please reset the numeric sorting indicators or adjust it manually.</source>
        <translation>自動的にこの移動を実行できません。数値的な並び替えの指標をリセットするか手動で調整してください。</translation>
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
        <translation>Eメール: %1</translation>
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
    <name>VoiceRecorder</name>
    <message>
        <location filename="VoiceRecorder.cpp" line="+184"/>
        <source>Invalid sample rate given to recorder</source>
        <translation>レコーダに与えられたサンプルレートは無効です</translation>
    </message>
    <message>
        <location line="+98"/>
        <source>Recorder failed to create directory &apos;%1&apos;</source>
        <translation>レコーダはディレクトリ&apos;%1&apos;の作成に失敗しました</translation>
    </message>
    <message>
        <location line="+14"/>
        <source>Recorder failed to open file &apos;%s&apos;</source>
        <translation>レコーダはファイル &apos;%s&apos; のオープンに失敗しました</translation>
    </message>
    <message>
        <location line="+109"/>
        <source>.wav - Uncompressed</source>
        <translation>.wav - 無圧縮</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>.ogg (Vorbis) - Compressed</source>
        <translation>.ogg (Vorbis) - 圧縮</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>.au - Uncompressed</source>
        <translation>.au - 無圧縮</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>.flac - Lossless compressed</source>
        <translation>.flac - ロスレス圧縮</translation>
    </message>
</context>
<context>
    <name>VoiceRecorderDialog</name>
    <message>
        <location filename="VoiceRecorderDialog.ui"/>
        <location filename="VoiceRecorderDialog.cpp" line="+98"/>
        <location line="+33"/>
        <location line="+8"/>
        <location line="+10"/>
        <location line="+9"/>
        <location line="+136"/>
        <source>Recorder</source>
        <translation>レコーダ</translation>
    </message>
    <message>
        <location/>
        <source>Control</source>
        <translation>制御</translation>
    </message>
    <message>
        <location/>
        <source>00:00:00</source>
        <translation>00:00:00</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Start</source>
        <translation>開始(&amp;S)</translation>
    </message>
    <message>
        <location/>
        <source>S&amp;top</source>
        <translation>停止(&amp;T)</translation>
    </message>
    <message>
        <location/>
        <source>Mode</source>
        <translation>モード</translation>
    </message>
    <message>
        <source>Mixdown</source>
        <translation type="obsolete">ミックスダウン</translation>
    </message>
    <message>
        <location/>
        <source>Multichannel</source>
        <translation>マルチチャンネル</translation>
    </message>
    <message>
        <location/>
        <source>Output</source>
        <translation>出力</translation>
    </message>
    <message>
        <location/>
        <source>Output format</source>
        <translation>出力形式</translation>
    </message>
    <message>
        <location/>
        <source>Target directory</source>
        <translation>対象のディレクトリ</translation>
    </message>
    <message>
        <location/>
        <source>Filename</source>
        <translation>ファイル名</translation>
    </message>
    <message>
        <source>&lt;pre&gt;
Valid variables are:
%user	Inserts the users name
%date	Inserts the current date
%time	Inserts the current time
%host	Inserts the hostname
&lt;/pre&gt;</source>
        <translation type="obsolete">&lt;pre&gt;
有効な変数は:
%user	ユーザ名を追加します
%date	現在の日付を追加します
%time	現在の時間を追加します
%host	ホスト名を追加します
&lt;/pre&gt;</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Browse...</source>
        <translation>参照(&amp;B)...</translation>
    </message>
    <message>
        <location filename="VoiceRecorderDialog.cpp" line="-195"/>
        <source>Closing the recorder will stop your current recording. Do you really want to close the recorder?</source>
        <translation>レコーダを閉じると現在の録音が停止します。本当にレコーダを閉じますか?</translation>
    </message>
    <message>
        <source>Recoder</source>
        <translation type="obsolete">レコーダ</translation>
    </message>
    <message>
        <location line="-31"/>
        <source>Valid variables are:</source>
        <translation type="unfinished">有効な変数は:</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Inserts the user&apos;s name</source>
        <translation type="unfinished">ユーザ名を挿入</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Inserts the current date</source>
        <translation type="unfinished">現在の日付を挿入</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Inserts the current time</source>
        <translation type="unfinished">現在の時間を挿入</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Inserts the hostname</source>
        <translation type="unfinished">ホスト名を挿入</translation>
    </message>
    <message>
        <location line="+60"/>
        <source>Unable to start recording. Not connected to a server.</source>
        <translation>録音を開始できません。サーバに接続していません。</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>The server you are currently connected to is version 1.2.2 or older. For privacy reasons, recording on servers of versions older than 1.2.3 is not possible.
Please contact your server administrator for further information.</source>
        <translation>現在接続しているサーバは1.2.2以前のバージョンです。プライバシーの問題で、1.2.3より古いバージョンのサーバ上では録音できません。
詳しくはサーバ管理者に連絡してください。</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>There is already a recorder active for this server.</source>
        <translation>このサーバのレコーダがすでに有効です。</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Please select a recording format.</source>
        <translation>録音形式を選択してください。</translation>
    </message>
    <message>
        <location line="+95"/>
        <source>Select target directory</source>
        <translation>対象のディレクトリを選択</translation>
    </message>
    <message>
        <location filename="VoiceRecorderDialog.ui"/>
        <source>Downmix</source>
        <translation type="unfinished">ダウンミックス</translation>
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
