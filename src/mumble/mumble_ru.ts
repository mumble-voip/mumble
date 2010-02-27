<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.0">
<context>
    <name>ACLEditor</name>
    <message>
        <source>Mumble - Edit ACL for %1</source>
        <translation type="obsolete">Mumble - Изменить ACL (список контроля доступа) для %1</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui"/>
        <source>Active ACLs</source>
        <translation>Активные ACL(списки контроля доступа)</translation>
    </message>
    <message>
        <location/>
        <source>Context</source>
        <translation>Контекст</translation>
    </message>
    <message>
        <location/>
        <source>User/Group</source>
        <translation>Пользователь/Группа</translation>
    </message>
    <message>
        <location/>
        <source>This controls which group of users this entry applies to.&lt;br /&gt;Note that the group is evaluated in the context of the channel the entry is used in. For example, the default ACL on the Root channel gives &lt;i&gt;Write&lt;/i&gt; permission to the &lt;i&gt;admin&lt;/i&gt; group. This entry, if inherited by a channel, will give a user write privileges if he belongs to the &lt;i&gt;admin&lt;/i&gt; group in that channel, even if he doesn&apos;t belong to the &lt;i&gt;admin&lt;/i&gt; group in the channel where the ACL originated.&lt;br /&gt;If a group name starts with a &apos;!&apos;, its membership is negated, and if it starts with a &apos;~&apos;, it is evaluated in the channel the ACL was defined in, rather than the channel the ACL is active in.&lt;br /&gt;If a group name starts with a  &apos;#&apos;, it is interpreted as an access token. Users must have entered whatever follows the &apos;#&apos; in their list of access tokens to match. This can be used for very simple password access to channels for non-authenticated users.&lt;br /&gt;If a group name starts with a  &apos;$&apos;, it will only match users whose certificate hash matches what follows the &apos;$&apos;.&lt;br /&gt;A few special predefined groups are:&lt;br /&gt;&lt;b&gt;all&lt;/b&gt; - Everyone will match.&lt;br /&gt;&lt;b&gt;auth&lt;/b&gt; - All authenticated users will match.&lt;br /&gt;&lt;b&gt;sub,a,b,c&lt;/b&gt; - User currently in a sub-channel minimum &lt;i&gt;a&lt;/i&gt; common parents, and between &lt;i&gt;b&lt;/i&gt; and &lt;i&gt;c&lt;/i&gt; channels down the chain. See the website for more extensive documentation on this one.&lt;br /&gt;&lt;b&gt;in&lt;/b&gt; - Users currently in the channel will match (convenience for &apos;&lt;i&gt;sub,0,0,0&lt;/i&gt;&apos;).&lt;br /&gt;&lt;b&gt;out&lt;/b&gt; - Users outside the channel will match (convenience for &apos;&lt;i&gt;!sub,0,0,0&lt;/i&gt;&apos;).&lt;br /&gt;Note that an entry applies to either a user or a group, not both.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Permissions</source>
        <translation>Разрешения</translation>
    </message>
    <message>
        <location/>
        <source>Group</source>
        <translation>Группа</translation>
    </message>
    <message>
        <location/>
        <source>Members</source>
        <translation>Члены группы</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Add</source>
        <translation>&amp;Добавить</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Remove</source>
        <translation>&amp;Удалить</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Up</source>
        <translation>&amp;Вверх</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Down</source>
        <translation>Вн&amp;из</translation>
    </message>
    <message>
        <location/>
        <source>Inherit ACLs</source>
        <translation>Наследовать ACL(список контроля доступа)</translation>
    </message>
    <message>
        <location/>
        <source>Applies to this channel</source>
        <translation>Примененные к этому каналу</translation>
    </message>
    <message>
        <location/>
        <source>Applies to sub-channels</source>
        <translation>Примененные к подканалам</translation>
    </message>
    <message>
        <location/>
        <source>User ID</source>
        <translation>Идентификатор пользователя</translation>
    </message>
    <message>
        <location filename="ACLEditor.cpp" line="+100"/>
        <source>Deny</source>
        <translation>Отказать</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Allow</source>
        <translation>Разрешить</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui"/>
        <source>Remove</source>
        <translation>Убрать</translation>
    </message>
    <message>
        <location/>
        <source>Inherit</source>
        <translation>Наследовать</translation>
    </message>
    <message>
        <location/>
        <source>Inheritable</source>
        <translation>Наследуемый</translation>
    </message>
    <message>
        <location/>
        <source>Inherited</source>
        <translation>Унаследованный</translation>
    </message>
    <message>
        <location/>
        <source>Add</source>
        <translation>Добавить</translation>
    </message>
    <message>
        <source>Add to Remove</source>
        <translation type="obsolete">Добавить к удаленным</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Groups</source>
        <translation>&amp;Группы</translation>
    </message>
    <message>
        <location/>
        <source>&amp;ACL</source>
        <translation>&amp;Списки контроля доступа (ACL)</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Ок</translation>
    </message>
    <message>
        <source>Accept changes</source>
        <translation type="obsolete">Принять изменения</translation>
    </message>
    <message>
        <source>This button will accept current groups/ACLs and send them to the server. Note that if you mistakenly remove write permission from yourself, the server will add it.</source>
        <translation type="obsolete">Эта кнопка примет текущие группы/списки ACL и пошлет их на сервер.Заметьте, что если вы по ошибке сняли с себя разрешения на запись, сервер добавит его.</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">Отм&amp;енить</translation>
    </message>
    <message>
        <source>Reject changes</source>
        <translation type="obsolete">Отклонить изменения</translation>
    </message>
    <message>
        <source>This button will cancel all changes and closes the dialog without updating the ACLs or groups on the server.</source>
        <translation type="obsolete">Эта кнопка отменит все изменения и закроет диалоговое окно без обновления списков ACL или групп на сервере.</translation>
    </message>
    <message>
        <location/>
        <source>List of entries</source>
        <translation>Список вхождений</translation>
    </message>
    <message>
        <source>This shows all the entries active on this channel. Entries inherited from parent channels will be show in italics.</source>
        <translation type="obsolete">Это покажет все активные вхождения на этом канале. Вхождения, наследованные от родительских каналов будут выделены курсивом.</translation>
    </message>
    <message>
        <location/>
        <source>Inherit ACL of parent?</source>
        <translation>Унаследовать списки ACL от предков?</translation>
    </message>
    <message>
        <location/>
        <source>This sets whether or not the ACL up the chain of parent channels are applied to this object. Only those entries that are marked in the parent as &quot;Apply to sub-channels&quot; will be inherited.</source>
        <translation>Это унаследует либо нет цепочку списков ACL от каналов-предков для этого объекта. Унаследованы будут только вхождения, помеченные у предков как &quot;Применить к подканалам&quot;.</translation>
    </message>
    <message>
        <location/>
        <source>Add new entry</source>
        <translation>Добавить новый</translation>
    </message>
    <message>
        <location/>
        <source>This adds a new entry, initially set with no permissions and applying to all.</source>
        <translation>Добавляет новое вхождение, по умолчанию без прав доступа иприменяемое ко всем.</translation>
    </message>
    <message>
        <location/>
        <source>Remove entry</source>
        <translation>Удалить вхождение</translation>
    </message>
    <message>
        <location/>
        <source>This removes the currently selected entry.</source>
        <translation>Удалит выделенное вхождение.</translation>
    </message>
    <message>
        <location/>
        <source>Move entry up</source>
        <translation>Переместить выше</translation>
    </message>
    <message>
        <location/>
        <source>Name</source>
        <translation type="unfinished">Имя</translation>
    </message>
    <message>
        <location/>
        <source>Enter the channel name here.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Name&lt;/b&gt;&lt;br&gt;Enter the channel name in this field. The name has to comply with the restriction imposed by the server you are connected to.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Description</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Password</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Enter the channel password here.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Password&lt;/b&gt;&lt;br&gt;This field allows you to easily set and change the password of a channel. In the background it uses Mumble&apos;s access tokens feature. To allow more fine grained and powerful access control directly use ACLs and groups instead (&lt;i&gt;Advanced configuration&lt;/i&gt; has to be checked to be able to see these settings).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Check to create a temporary channel.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Temporary&lt;/b&gt;&lt;br&gt;
When checked the channel created will be marked as temporary. This means when the last player leaves it the channel will be automatically deleted by the server.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Temporary</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Channel positioning facility value</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Position&lt;/b&gt;&lt;br/&gt;
This value enables you to change the way Mumble arranges the channels in the tree. A channel with a higher &lt;i&gt;Position&lt;/i&gt; value will always be placed below one with a lower value and the other way around. If the &lt;i&gt;Position&lt;/i&gt; value of two channels is equal they will get sorted alphabetically by their name.</source>
        <oldsource>&lt;b&gt;Position&lt;/b&gt;&lt;br/&gt;
This value enables you to change the way mumble arranges the channels in the tree. A channel with a higher &lt;i&gt;Position&lt;/i&gt; value will always be placed below one with a lower value and the other way around. If the &lt;i&gt;Position&lt;/i&gt; value of two channels is equal they will get sorted alphabetically by their name.</oldsource>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Position</source>
        <translation type="unfinished">Позиция</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Group&lt;/b&gt;&lt;br&gt;
These are all the groups currently defined for the channel. To create a new group, just type in the name and press enter.</source>
        <oldsource>&lt;b&gt;Group&lt;/b&gt;&lt;br&gt;
This is all the groups currently defined for the channel. To create a new group, just type in the name and press enter.</oldsource>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Add new group</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Add&lt;/b&gt;&lt;br/&gt;
Add a new group.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Remove&lt;/b&gt;&lt;br&gt;This removes the currently selected group. If the group was inherited, it will not be removed from the list, but all local information about the group will be cleared.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Inherit&lt;/b&gt;&lt;br&gt;This inherits all the members in the group from the parent, if the group is marked as &lt;i&gt;Inheritable&lt;/i&gt; in the parent channel.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Inheritable&lt;/b&gt;&lt;br&gt;This makes this group inheritable to sub-channels. If the group is non-inheritable, sub-channels are still free to create a new group with the same name.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Inherited&lt;/b&gt;&lt;br&gt;This indicates that the group was inherited from the parent channel. You cannot edit this flag, it&apos;s just for information.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Inherited members</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Contains the list of members added to the group by this channel.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Members&lt;/b&gt;&lt;br&gt;
This list contains all members that were added to the group by the current channel. Be aware that this does not include members inherited by higher levels of the channel tree. These can be found in the &lt;i&gt;Inherited members&lt;/i&gt; list. To prevent this list to be inherited by lower level channels uncheck &lt;i&gt;Inheritable&lt;/i&gt; or manually add the members to the &lt;i&gt;Excluded members&lt;/i&gt; list.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Contains a list of members whose group membership will not be inherited from the parent channel.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Excluded members&lt;/b&gt;&lt;br&gt;
Contains a list of members whose group membership will not be inherited from the parent channel.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Contains the list of members inherited by other channels.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Inherited members&lt;/b&gt;&lt;br&gt;
Contains the list of members inherited by the current channel. Uncheck &lt;i&gt;Inherit&lt;/i&gt; to prevent inheritance from higher level channels.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Type in the name of a user you wish to add to the group and click Add.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Type in the name of a user you wish to remove from the group and click Add.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Exclude</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Excluded members</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This moves the entry up in the list. As entries are evaluated in order, this may change the effective permissions of users. You cannot move an entry above an inherited entry, if you really need that you&apos;ll have to duplicate the inherited entry.</source>
        <translation>Перемещает вхождение выше по списку. Так как вхождения упорядочены, это может изменить эффективные привелегии пользователей. Вы не сможете переместить вхождение выше унаследованного, если вы действительно хотите его переместить, вам придется продублировать унаследованое вхождение.</translation>
    </message>
    <message>
        <location/>
        <source>Move entry down</source>
        <translation>Переместить ниже</translation>
    </message>
    <message>
        <location/>
        <source>This moves the entry down in the list. As entries are evaluated in order, this may change the effective permissions of users.</source>
        <translation>Перемещает вхождение ниже по списку. Так как вхождения упорядочены, это может изменить эффективные привелегии пользователей.</translation>
    </message>
    <message>
        <location/>
        <source>Entry should apply to this channel.</source>
        <oldsource>Entry should apply to this channel</oldsource>
        <translation type="unfinished">Применять к каналу</translation>
    </message>
    <message>
        <location/>
        <source>This makes the entry apply to this channel.</source>
        <translation>Применяет вхождение к этому каналу.</translation>
    </message>
    <message>
        <location/>
        <source>Entry should apply to sub-channels.</source>
        <translation>Применять к подканалам.</translation>
    </message>
    <message>
        <location/>
        <source>This makes the entry apply to sub-channels of this channel.</source>
        <translation>Применяет вхождение ко всем подканалам этого канала.</translation>
    </message>
    <message>
        <location/>
        <source>Group this entry applies to</source>
        <translation>Применить к группе</translation>
    </message>
    <message>
        <source>This controls which group of users this entry applies to.&lt;br /&gt;Note that the group is evaluated in the context of the channel the entry is used in. For example, the default ACL on the Root channel gives &lt;i&gt;Write&lt;/i&gt; permission to the &lt;i&gt;admin&lt;/i&gt; group. This entry, if inherited by a channel, will give a user write privileges if he belongs to the &lt;i&gt;admin&lt;/i&gt; group in that channel, even if he doesn&apos;t belong to the &lt;i&gt;admin&lt;/i&gt; group in the channel where the ACL originated.&lt;br /&gt;If a group name starts with a !, its membership is negated, and if it starts with a ~, it is evaluated in the channel the ACL was defined in, rather than the channel the ACL is active in. Order is important; &lt;i&gt;!~in&lt;/i&gt; is valid, but &lt;i&gt;~!in&lt;/i&gt; is not.&lt;br /&gt;A few special predefined groups are:&lt;br /&gt;&lt;b&gt;all&lt;/b&gt; - Everyone will match.&lt;br /&gt;&lt;b&gt;auth&lt;/b&gt; - All authenticated users will match.&lt;br /&gt;&lt;b&gt;sub,a,b,c&lt;/b&gt; - User currently in a sub-channel minimum &lt;i&gt;a&lt;/i&gt; common parents, and between &lt;i&gt;b&lt;/i&gt; and &lt;i&gt;c&lt;/i&gt; channels down the chain. See the website for more extensive documentation on this one.&lt;br /&gt;&lt;b&gt;in&lt;/b&gt; - Users currently in the channel will match (convenience for &lt;i&gt;sub,0,0,0&lt;/i&gt;).&lt;br /&gt;&lt;b&gt;out&lt;/b&gt; - Users outside the channel will match (convenience for &lt;i&gt;!sub,0,0,0&lt;/i&gt;).&lt;br /&gt;Note that an entry applies to either a user or a group, not both.</source>
        <translation type="obsolete">Управляет, к какой группе пользователей применять вхождение.&lt;br /&gt;Заметьте, что группа оценивается как контекст канала, в котором используется вхождение. Например, по умолчанию список ACL канала Root дает права на &lt;i&gt;Запись&lt;/i&gt; группе &lt;i&gt;admin&lt;/i&gt;. Если это вхождение унаследуется каналом, даст пользователю права на запись если он принадлежит к группе &lt;i&gt;admin&lt;/i&gt; на этом канале, даже если он не принадлежит к группе &lt;i&gt;admin&lt;/i&gt; на канале, от которого унаследовали список ACL.&lt;br /&gt;Если имя группы начинается с a !, его членство отрицается, а если начинается с a ~, оно оценивается для канала, которому ACL был определен, чем канал, в котором есть активный ACL. Порядок важен; &lt;i&gt;!~in&lt;/i&gt; принимается, но &lt;i&gt;~!in&lt;/i&gt; нет.&lt;br /&gt;Несколько особых предопределенных групп:&lt;br /&gt;&lt;b&gt;all&lt;/b&gt; - Соответствует всем.&lt;br /&gt;&lt;b&gt;auth&lt;/b&gt; - Соответствует всем авторизованным пользователям.&lt;br /&gt;&lt;b&gt;sub,a,b,c&lt;/b&gt; - Пользователи на подканале с минимум &lt;i&gt;a&lt;/i&gt; общими предками, а также между &lt;i&gt;b&lt;/i&gt; и &lt;i&gt;c&lt;/i&gt; каналами в цепочке. Для более подробной информации посетите вебсайт.&lt;br /&gt;&lt;b&gt;in&lt;/b&gt; - Подойдут пользователи на текущем канале (удобство для &lt;i&gt;sub,0,0,0&lt;/i&gt;).&lt;br /&gt;&lt;b&gt;out&lt;/b&gt; - Соответствует всем пользователям вне канала (удобство для &lt;i&gt;!sub,0,0,0&lt;/i&gt;).&lt;br /&gt;Заметьте, что вхождение применяется только к пользователю или к группе, а не к обоим.</translation>
    </message>
    <message>
        <location/>
        <source>User this entry applies to</source>
        <translation>Применять для пользователя</translation>
    </message>
    <message>
        <location/>
        <source>This controls which user this entry applies to. Just type in the user name and hit enter to query the server for a match.</source>
        <translation>Управляет, к какому пользователю применять вхождение. Просто введите имя пользователя и нажмите Enter, чтобы послать запрос на сервер.</translation>
    </message>
    <message>
        <location filename="ACLEditor.cpp" line="+22"/>
        <source>Allow %1</source>
        <translation>Позволить %1</translation>
    </message>
    <message>
        <location line="-9"/>
        <source>Deny %1</source>
        <translation>Запретить %1</translation>
    </message>
    <message>
        <location line="-61"/>
        <source>Mumble - Add channel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+22"/>
        <location line="+162"/>
        <source>Failed: Invalid channel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-150"/>
        <source>Mumble - Edit %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+37"/>
        <source>This grants the %1 privilege. If a privilege is both allowed and denied, it is denied.&lt;br /&gt;%2</source>
        <translation>Это дает привелегию на %1. Если привелегия и позволяет, и запрещает, она запрещает.&lt;br /&gt;%2</translation>
    </message>
    <message>
        <location line="-9"/>
        <source>This revokes the %1 privilege. If a privilege is both allowed and denied, it is denied.&lt;br /&gt;%2</source>
        <translation>Отбирает привелегию на %1. Если привелегия и позволяет, и запрещает, она запрещает.&lt;br /&gt;%2</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui"/>
        <source>List of groups</source>
        <translation>Список групп</translation>
    </message>
    <message>
        <source>This is all the groups currently defined for the channel. To create a new group, just type in the name and press enter.</source>
        <translation type="obsolete">Показывает все группы, определенные для канала. Чтобы создать новую группу, просто введите имя и нажмите Enter.</translation>
    </message>
    <message>
        <location/>
        <source>Remove selected group</source>
        <translation>Удалить выбранную группу</translation>
    </message>
    <message>
        <source>This removes the currently selected group. If the group was inherited, it will not be removed from the list, but all local information about the group will be cleared.</source>
        <translation type="obsolete">Удаляет выбранную группу. Если группа была унаследована, она не уберется из списка, но вся локальная информация о группе будет стерта.</translation>
    </message>
    <message>
        <location/>
        <source>Inherit group members from parent</source>
        <translation>Унаследовать членов группы от</translation>
    </message>
    <message>
        <source>This inherits all the members in the group from the parent, if the group is marked as &lt;i&gt;Inheritable&lt;/i&gt; in the parent channel.</source>
        <translation type="obsolete">Наследует всех членов группы от предка, если группа отмечена как &lt;i&gt;Наследуемая&lt;/i&gt; в родительском канале.</translation>
    </message>
    <message>
        <location/>
        <source>Make group inheritable to sub-channels</source>
        <translation>Сделать группу наследуемой для подканалов</translation>
    </message>
    <message>
        <source>This makes this group inheritable to sub-channels. If the group is non-inheritable, sub-channels are still free to create a new group with the same name.</source>
        <translation type="obsolete">Сделать группу наследуемой для подканалов. Если группа ненаследованная, подканалы все еще свободны для создания новой группы с таким же именем.</translation>
    </message>
    <message>
        <location/>
        <source>Group was inherited from parent channel</source>
        <translation>Группа была унаследована от родительского канала</translation>
    </message>
    <message>
        <source>This indicates that the group was inherited from the parent channel. You cannot edit this flag, it&apos;s just for information.</source>
        <translation type="obsolete">Показывает, что группа унаследована от канала-родителя. Вы не можете изменить этот флаг, он просто для информации.</translation>
    </message>
    <message>
        <location/>
        <source>Add member to group</source>
        <translation>Добавить члена группы</translation>
    </message>
    <message>
        <source>Type in the name of a player you wish to add to the group and press enter.</source>
        <translation type="obsolete">Введите имя игрока, которого вы хотите добавить в группу и нажмите Enter.</translation>
    </message>
    <message>
        <location/>
        <source>Remove member from group</source>
        <translation>Удалить члена группы</translation>
    </message>
    <message>
        <source>Type in the name of a player you wish to remove from the group and press enter.</source>
        <translation type="obsolete">Введите имя игрока, которого вы хотите удалить из группы и нажмите Enter.</translation>
    </message>
    <message>
        <location/>
        <source>Dialog</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This shows all the entries active on this channel. Entries inherited from parent channels will be show in italics.&lt;br /&gt;ACLs are evaluated top to bottom, meaning priority increases as you move down the list.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&amp;Properties</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ALSAAudioInput</name>
    <message>
        <location filename="ALSAAudio.cpp" line="+363"/>
        <source>Opening chosen ALSA Input failed: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-191"/>
        <source>Default ALSA Card</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ALSAAudioOutput</name>
    <message>
        <location line="+326"/>
        <source>Opening chosen ALSA Output failed: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-325"/>
        <source>Default ALSA Card</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ALSAConfig</name>
    <message>
        <source>Device selection</source>
        <translation type="obsolete">Выбор устройства</translation>
    </message>
    <message>
        <source>Device to use for speakers/headphones</source>
        <translation type="obsolete">Устройство, используемое как динамики/наушники</translation>
    </message>
    <message>
        <source>Input</source>
        <translation type="obsolete">Ввод</translation>
    </message>
    <message>
        <source>Output</source>
        <translation type="obsolete">Вывод</translation>
    </message>
    <message>
        <source>Output Options</source>
        <translation type="obsolete">Свойства вывода</translation>
    </message>
    <message>
        <source>Output Delay</source>
        <translation type="obsolete">Задержка вывода</translation>
    </message>
    <message>
        <source>%1ms</source>
        <translation type="obsolete">%1мс</translation>
    </message>
</context>
<context>
    <name>ASIOConfig</name>
    <message>
        <location filename="ASIOInput.ui"/>
        <source>Device selection</source>
        <translation>Выбор устройства</translation>
    </message>
    <message>
        <location/>
        <source>Capabilities</source>
        <translation>Возможности</translation>
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
    <message>
        <location/>
        <source>Channels</source>
        <translation>Каналы</translation>
    </message>
    <message>
        <location/>
        <source>Configure input channels</source>
        <translation>Настроить каналы ввода</translation>
    </message>
    <message>
        <source>This will configure the input channels for ASIO. Make sure you select at least one channel as microphone and speaker. &lt;i&gt;Microphone&lt;/i&gt; should be where your microphone is attached, and &lt;i&gt;Speaker&lt;/i&gt; should be a channel that samples &quot;What you hear&quot;.&lt;br /&gt;For example, on the Audigy 2 ZS, a good selection for Microphone would be &quot;Mic L&quot; while Speaker should be &quot;Mix L&quot; and &quot;Mix R&quot;</source>
        <translation type="obsolete">Настраивает входные каналы для ASIO. Убедитесь, что выбрали минимум 1 канал как микрофон и динамик. &lt;i&gt;Микрофон&lt;/i&gt; должен быть там, куда подсоединен ваш микрофон, и &lt;i&gt;Динамик&lt;/i&gt; должен быть каналом, передающим &quot;Что вы слышите&quot;.&lt;br /&gt;Например, на Audigy 2 ZS, отличным выбором для Микрофона будет &quot;Mic L&quot;, в то время, как динамик - &quot;Mix L&quot; и &quot;Mix R&quot;</translation>
    </message>
    <message>
        <location/>
        <source>Device to use for microphone</source>
        <translation>Устройство для подключения микрофона</translation>
    </message>
    <message>
        <location/>
        <source>This chooses what device to query. You still need to actually query the device and select which channels to use.</source>
        <translation>Выбирает, какое устройство опрашивать. Вам все еще надо опросить устройство и выбрать канал, который будете использовать.</translation>
    </message>
    <message>
        <location/>
        <source>Device</source>
        <translation>Устройство</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Query</source>
        <translation>&amp;Запрос</translation>
    </message>
    <message>
        <location/>
        <source>Query selected device</source>
        <translation>Опросить выбранное устройство</translation>
    </message>
    <message>
        <location/>
        <source>This queries the selected device for channels. Be aware that many ASIO drivers are buggy to the extreme, and querying them might cause a crash of either the application or the system.</source>
        <translation>Опрашивает выбранное устройство поканально. Опасайтесь того, что многие драйвера ASIO не доделаны, и опрос их может вызвать &quot;зависание&quot; приложения или всей системы.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Configure</source>
        <translation>Н&amp;астроить</translation>
    </message>
    <message>
        <location/>
        <source>Configure selected device</source>
        <translation>Настроить выбранное устройство</translation>
    </message>
    <message>
        <location/>
        <source>This configures the selected device. Be aware that many ASIO drivers are buggy to the extreme, and querying them might cause a crash of either the application or the system.</source>
        <translation>Настраивает выбранное устройство. Опасайтесь того, что многие драйвера ASIO не доделаны, и опрос их может вызвать &quot;зависание&quot; приложения или всей системы.</translation>
    </message>
    <message>
        <location/>
        <source>Driver name</source>
        <translation>Имя привода</translation>
    </message>
    <message>
        <source>Buffersize</source>
        <translation type="obsolete">Размер буфера</translation>
    </message>
    <message>
        <location/>
        <source>Microphone</source>
        <translation>Микрофон</translation>
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
        <translation>Не используется</translation>
    </message>
    <message>
        <location/>
        <source>Speakers</source>
        <translation>Динамики</translation>
    </message>
    <message>
        <source>%1 (ver %2)</source>
        <translation type="obsolete">%1 (версия %2)</translation>
    </message>
    <message>
        <source>%1 ms -&gt; %2 ms (%3 ms resolution) %4Hz</source>
        <translation type="obsolete">%1 мс -&gt; %2 мс (%3 мс разрешение) %4Гц</translation>
    </message>
    <message>
        <source>%1 ms -&gt; %2 ms (%3 ms resolution) %4Hz -- Unusable</source>
        <translation type="obsolete">%1 мс -&gt; %2 мс (%3 мс разрешение) %4Гц -- Не используемо</translation>
    </message>
    <message>
        <location filename="ASIOInput.cpp" line="+224"/>
        <source>%1 (version %2)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>%1 -&gt; %2 samples buffer, with %3 sample resolution (%4 preferred) at %5 Hz</source>
        <oldsource>%1 -&gt; %2 samples buffer, with %3 sample resolution (%4 preferred) at %5Hz</oldsource>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mumble</source>
        <translation type="obsolete">Mumble</translation>
    </message>
    <message>
        <location line="+42"/>
        <location line="+27"/>
        <source>ASIO Initialization failed: %1</source>
        <translation>Ошибка инициализации ASIO: %1</translation>
    </message>
    <message>
        <location line="-23"/>
        <location line="+27"/>
        <source>Failed to instantiate ASIO driver</source>
        <translation>Невозможно установить драйвер ASIO</translation>
    </message>
    <message>
        <location line="+37"/>
        <source>ASIO</source>
        <translation>ASIO</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui"/>
        <source>Form</source>
        <translation type="unfinished"></translation>
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
        <translation>Вам нужно выбрать хотя бы одно устройство для микрофона и одно для динамика для использования ASIO. Если вам нужно только протестировать микрофон, используйет DirectSound.</translation>
    </message>
    <message>
        <location line="+103"/>
        <source>Opening selected ASIO device failed. No input will be done.</source>
        <translation>Невозможно открыть устройство ASIO. Входных данных не получено.</translation>
    </message>
</context>
<context>
    <name>AboutDialog</name>
    <message>
        <source>&lt;h3&gt;Mumble v1.0.0&lt;/h3&gt;&lt;p&gt;Copyright 2005-2006 Thorvald Natvig&lt;br /&gt;slicer@users.sourceforge.net&lt;/p&gt;&lt;p&gt;&lt;b&gt;A voice-chat utility for gamers&lt;/b&gt;&lt;/p&gt;&lt;p&gt;&lt;tt&gt;http://mumble.sourceforge.net/&lt;/tt&gt;&lt;/p&gt;</source>
        <translation type="obsolete">&lt;h3&gt;Mumble v1.0.0&lt;/h3&gt;&lt;p&gt;Все права принадлежат 2005-2006 Thorvald Natvig&lt;br /&gt;slicer@users.sourceforge.net&lt;/p&gt;&lt;p&gt;
Русский перевод Khazov Nick&lt;br /&gt;m2k3d0n@users.sourceforge.net&lt;/p&gt;&lt;p&gt;&lt;b&gt;Программа для голосового общения игроков&lt;/b&gt;&lt;/p&gt;&lt;p&gt;&lt;tt&gt;http://mumble.sourceforge.net/&lt;/tt&gt;&lt;/p&gt;</translation>
    </message>
    <message>
        <location filename="About.cpp" line="+63"/>
        <source>&amp;About Mumble</source>
        <translation>&amp;О Mumble</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>&amp;License</source>
        <translation>Л&amp;ицензия</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>OK</source>
        <translation>ОК</translation>
    </message>
    <message>
        <location line="-29"/>
        <source>About Mumble</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+16"/>
        <source>&lt;h3&gt;Mumble (%1)&lt;/h3&gt;&lt;p&gt;Copyright %3 Thorvald Natvig&lt;br /&gt;slicer@users.sourceforge.net&lt;/p&gt;&lt;p&gt;&lt;b&gt;A voice-chat utility for gamers&lt;/b&gt;&lt;/p&gt;&lt;p&gt;&lt;tt&gt;&lt;a href=&quot;%2&quot;&gt;%2&lt;/a&gt;&lt;/tt&gt;&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>AboutSpeexDialog</name>
    <message>
        <location line="+25"/>
        <source>&lt;h3&gt;About Speex&lt;/h3&gt;&lt;p&gt;&lt;tt&gt;&lt;a href=&quot;%1&quot;&gt;%1&lt;/a&gt;&lt;/tt&gt;&lt;/p&gt;&lt;p&gt;This program uses SpeexDSP.&lt;/p&gt;&lt;p&gt;Speex is used for echo cancellation, noise&lt;br /&gt;filtering and voice activity detection.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;h3&gt;About Speex&lt;/h3&gt;&lt;p&gt;&lt;tt&gt;http://www.speex.org/&lt;/tt&gt;&lt;/p&gt;&lt;p&gt;This program uses Speex version %1&lt;/p&gt;&lt;p&gt;Speex is used for echo cancellation, noise&lt;br /&gt;filtering, voice activity detection and speech&lt;br /&gt;compression.&lt;/p&gt;</source>
        <translation type="obsolete">&lt;h3&gt;О Speex&lt;/h3&gt;&lt;p&gt;&lt;tt&gt;http://www.speex.org/&lt;/tt&gt;&lt;/p&gt;&lt;p&gt;Эта программа использует Speex версии %1&lt;/p&gt;&lt;p&gt;Speex используется для подавления эха, шумов&lt;br /&gt;фильтрации, определения голосовой активности и сжатия&lt;br /&gt;речи.&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>OK</source>
        <translation>ОК</translation>
    </message>
    <message>
        <location line="-12"/>
        <source>About Speex</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>AudioConfigDialog</name>
    <message>
        <source>Interfaces</source>
        <translation type="obsolete">Интерфейсы</translation>
    </message>
    <message>
        <source>Transmission</source>
        <translation type="obsolete">Передача</translation>
    </message>
    <message>
        <source>Compression</source>
        <translation type="obsolete">Сжатие</translation>
    </message>
    <message>
        <source>Jitter Buffer</source>
        <translation type="obsolete">Буфер Jitter</translation>
    </message>
    <message>
        <source>Input</source>
        <translation type="obsolete">Ввод</translation>
    </message>
    <message>
        <source>Input method for audio</source>
        <translation type="obsolete">Метод ввода для аудио</translation>
    </message>
    <message>
        <source>&lt;b&gt;This is the input method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation type="obsolete">&lt;b&gt;Этот метод ввода используется для аудио.&lt;/b&gt;&lt;br /&gt;Скорее всего вы захотите использовать DirectSound.</translation>
    </message>
    <message>
        <source>Output</source>
        <translation type="obsolete">Вывод</translation>
    </message>
    <message>
        <source>Output method for audio</source>
        <translation type="obsolete">Мытод вывода для аудио</translation>
    </message>
    <message>
        <source>&lt;b&gt;This is the output method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation type="obsolete">&lt;b&gt;Этот метод вывода используется для аудио.&lt;/b&gt;&lt;br /&gt;Скорее всего вы захотите использовать DirectSound.</translation>
    </message>
    <message>
        <source>Continuous</source>
        <translation type="obsolete">Продолжительный</translation>
    </message>
    <message>
        <source>Voice Activity</source>
        <translation type="obsolete">Голосовая активность</translation>
    </message>
    <message>
        <source>Push To Talk</source>
        <translation type="obsolete">Нажмите для разговора</translation>
    </message>
    <message>
        <source>Transmit</source>
        <translation type="obsolete">Передать</translation>
    </message>
    <message>
        <source>When to transmit your speech</source>
        <translation type="obsolete">Когда передавать вашу речь</translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets when speech should be transmitted.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;Continuous&lt;/i&gt; - All the time&lt;br /&gt;&lt;i&gt;Voice Activity&lt;/i&gt; - When you are speaking clearly.&lt;br /&gt;&lt;i&gt;Push To Talk&lt;/i&gt; - When you hold down the hotkey set under &lt;i&gt;Shortcuts&lt;/i&gt;.</source>
        <translation type="obsolete">&lt;b&gt;Устанавливает, когда передавать речь.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;Постоянно&lt;/i&gt; - Все время&lt;br /&gt;&lt;i&gt;При активность голоса&lt;/i&gt; - Когда вы говорите четко.&lt;br /&gt;&lt;i&gt;Нажмите для разговора&lt;/i&gt; - Когда вы зажимаете горячую клавишу, установленную в  &lt;i&gt;Ярлыках&lt;/i&gt;.</translation>
    </message>
    <message>
        <source>Voice Hold</source>
        <translation type="obsolete">Задержка голоса</translation>
    </message>
    <message>
        <source>How long to keep transmitting after silence</source>
        <translation type="obsolete">Как долго передавать после тишины</translation>
    </message>
    <message>
        <source>&lt;b&gt;This selects how long after a perceived stop in speech transmission should continue.&lt;/b&gt;&lt;br /&gt;Set this higher if your voice breaks up when you speak (seen by a rapidly blinking voice icon next to your name).&lt;br /&gt;Only makes sense when used with Voice Activity transmission.</source>
        <translation type="obsolete">&lt;b&gt;Выбирает, как долго после паузы в речи продолжать передачу.&lt;/b&gt;&lt;br /&gt;Установите ее больше, если у вас много пауз в речи (можно увидеть по часто мигающей иконке голоса рядом с вашим именем).&lt;br /&gt;Имеет смысл только когда используется с передачей по активности голоса.</translation>
    </message>
    <message>
        <source>Audio per packet</source>
        <translation type="obsolete">Аудио на пакет</translation>
    </message>
    <message>
        <source>How many audio frames to send per packet</source>
        <translation type="obsolete">Как много аудио фрагментов посылать в пакете</translation>
    </message>
    <message>
        <source>&lt;b&gt;This selects how many audio frames should be put in one packet.&lt;/b&gt;&lt;br /&gt;Increasing this will increase the latency of your voice, but will also reduce bandwidth requirements.</source>
        <translation type="obsolete">&lt;b&gt;Выбирает, сколько аудиофрагментов помещать в пакет.&lt;/b&gt;&lt;br /&gt;Увеличение ее увеличит задержку голоса, но также уменьшит требования к толщине канала связи.</translation>
    </message>
    <message>
        <source>Use TCP mode</source>
        <translation type="obsolete">Использовать режим TCP</translation>
    </message>
    <message>
        <source>Use TCP Compatibility mode</source>
        <translation type="obsolete">Включить режим TCP совместимости</translation>
    </message>
    <message>
        <source>&lt;b&gt;Enable TCP Compatibility mode&lt;/b&gt;.&lt;br /&gt;This will make Mumble use only TCP when communicating with the server. This will increase overhead and cause lost packets to produce noticeable pauses in communication, so this should only be used if you are unable to use the default (which uses UDP for voice and TCP for control).</source>
        <translation type="obsolete">&lt;b&gt;Включить режим TCP совместимости&lt;/b&gt;.&lt;br /&gt;Заставит Mumble использовать только TCP при связи с сервером. Это увеличит загрузку, вызовет потерю пакетов, что приведет к существенным паузам в общении, поэтому следует его использовать только в случае, если вы не можете использовать режим по умолчанию (применяющий UDP для голоса и TCP для контроля).</translation>
    </message>
    <message>
        <source>Automatic Reconnect</source>
        <translation type="obsolete">Автоматическое восстановление связи</translation>
    </message>
    <message>
        <source>Reconnect when disconnected</source>
        <translation type="obsolete">Восстановление связи при обрыве</translation>
    </message>
    <message>
        <source>&lt;b&gt;Reconnect when disconnected&lt;/b&gt;.&lt;br /&gt;This will make Mumble try to automatically reconnect after 10 seconds if your server connection fails.</source>
        <translation type="obsolete">&lt;b&gt;Восстановить связь при обрыве&lt;/b&gt;.&lt;br /&gt;Mumble будет пытаться автоматически восстановить связь через 10 в случае обрыва связи с сервером.</translation>
    </message>
    <message>
        <source>Maximum bandwidth used for sent audio</source>
        <translation type="obsolete">Максимальная скорость, используемая для посылки аудио</translation>
    </message>
    <message>
        <source>&lt;b&gt;This shows peak outgoing bandwidth used.&lt;/b&gt;&lt;br /&gt;This shows the peak amount of bandwidth sent out from your machine. Audio bitrate is the maximum bitrate (as we use VBR) for the audio data alone. Position is the bitrate used for positional information. Overhead is our framing and the IP packet headers (IP and UDP is 90% of this overhead).</source>
        <translation type="obsolete">&lt;b&gt;Показывает пиковую загрузку посланных сигналов.&lt;/b&gt;&lt;br /&gt;Показывает пиковое значение скорости передачи данных от вашего компьютера. Битрейт аудио - максимальный битрейт (по принятым сокращениям, VBR) только для аудиоданных. Позиция - битрейт, используемый для информации о позиции. Загрузка - раскадровка и заголовки IP пакетов (IP и UDP - 90% этой загрузки).</translation>
    </message>
    <message>
        <source>Outgoing Bitrate</source>
        <translation type="obsolete">Исходящий битрейт</translation>
    </message>
    <message>
        <source>Quality</source>
        <translation type="obsolete">Качество</translation>
    </message>
    <message>
        <source>Quality of compression (peak bandwidth)</source>
        <translation type="obsolete">Качество сжатия (пиковая передача)</translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets the quality of compression.&lt;/b&gt;&lt;br /&gt;This determines how much bandwidth Mumble is allowed to use for outgoing audio.</source>
        <translation type="obsolete">&lt;b&gt;Устанавливает качество сжатия.&lt;/b&gt;&lt;br /&gt;Определяет, какую загрузку канала Mumble разрешено использовать для исходящего аудио.</translation>
    </message>
    <message>
        <source>Complexity</source>
        <translation type="obsolete">Сложность</translation>
    </message>
    <message>
        <source>Complexity of compression (CPU)</source>
        <translation type="obsolete">Сложность сжатия (ЦП)</translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets the complexity of compression.&lt;/b&gt;&lt;br /&gt;This determines how much CPU Mumble is allowed to use to increase transmitted voice quality. Settings above 5 give only marginal gain.</source>
        <translation type="obsolete">&lt;b&gt;Устанавливает сложность сжатия.&lt;/b&gt;&lt;br /&gt;Определяет, сколько мощности ЦП Mumble разрешено использовать для увеличения качества передаваемого голоса. Установка выше 5 даст только предельный прирост.</translation>
    </message>
    <message>
        <source>Amp</source>
        <translation type="obsolete">Усиление</translation>
    </message>
    <message>
        <source>Maximum amplification of input sound</source>
        <translation type="obsolete">Максимальное усиление входящего звука</translation>
    </message>
    <message>
        <source>&lt;b&gt;Maximum amplification of input.&lt;/b&gt;&lt;br /&gt;Mumble normalizes the input volume before compressing, and this sets how much it&apos;s allowed to amplify.&lt;br /&gt;The actual level is continually updated based on your current speech pattern, but it will never go above the level specified here.&lt;br /&gt;If the &lt;i&gt;Microphone loudness&lt;/i&gt; level of the audio statistics hover around 100%, you probably want to set this to 2.0 or so, but if, like most people, you are unable to reach 100%, set this to something much higher.&lt;br /&gt;Ideally, set it so &lt;i&gt;Microphone Loudness * Amplification Factor &gt;= 100&lt;/i&gt;, even when you&apos;re speaking really soft.&lt;br /&gt;&lt;br /&gt;Note that there is no harm in setting this to maximum, but Mumble will start picking up other conversations if you leave it to auto-tune to that level.</source>
        <translation type="obsolete">&lt;b&gt;Максимальное усиление входного голоса.&lt;/b&gt;&lt;br /&gt;Mumble нормализует входную громкость до сжатия, эта опция устанавливает, на сколько можно его усилить.&lt;br /&gt;Текущий уровень постоянно обновляется на основе текущего образца речи, но никогда не будет выше установленного здесь уровня.&lt;br /&gt;Если уровень &lt;i&gt;Громкости микрофона&lt;/i&gt; аудиостатистики держится на уровне 100%, вы можете установить его на 2.0 или выше, но если, как многие люди, вы не можете достичь 100%, установите его на чуть более высоком уровне.&lt;br /&gt;В идеале, установите его так, чтобы &lt;i&gt;Громкость микрофона * Фактор усиления &gt;= 100&lt;/i&gt;, даже если вы говорите слишком мягко.&lt;br /&gt;&lt;br /&gt;Заметьте, что ничего плохого не случится, если вы установите его на максимум, но Mumble начнет подбирать другие переговоры, если вы оставите его на автоматической настройке на данном уровне.</translation>
    </message>
    <message>
        <source>Default Jitter Buffer</source>
        <translation type="obsolete">Буфер Jitter по умолчанию</translation>
    </message>
    <message>
        <source>How long to prebuffer on first packet</source>
        <translation type="obsolete">Как долго буферизовать первый пакет</translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets the default buffer size for the jitter buffer&lt;/b&gt;.&lt;br /&gt;All incoming audio is buffered, and the jitter buffer continually tries to push the buffer to the minimum sustainable by your network, so latency can be as low as possible. This sets the default buffer size to use on the first packet to arrive from a new speaker, or when a speaker using Voice Activity or Push-To-Talk just started talking again. If the start of sentences you hear is very jittery, increase this value.</source>
        <translation type="obsolete">&lt;b&gt;Устанавливает размер буфера Jitter по умолчанию&lt;/b&gt;.&lt;br /&gt;Все входящее аудио буферизуется, и буфер jitter постоянно пытается послать буфер в минимально поддерживаемый вашей сетью, так что задержка может быть как можно меньшей. Устанавливает размер буфера по умолчанию для использования при приходе первого пакета от нового говорящего, или когда говорящий использует Голосовую активность или Нажмите для разговора и только начал говорить. Если начало предложений, которые вы слишите, неразборчивы, увеличьте это значение.</translation>
    </message>
    <message>
        <source>Basic Audio</source>
        <translation type="obsolete">Основное аудио</translation>
    </message>
    <message>
        <source>%1 ms</source>
        <translation type="obsolete">%1 мс</translation>
    </message>
    <message>
        <source>%1s</source>
        <translation type="obsolete">%1 с</translation>
    </message>
    <message>
        <source>%1kbit/s (Audio %2, Position %4, Overhead %3)</source>
        <translation type="obsolete">%1кбит/с (Аудио %2, Позиция %4, Загрузка %3)</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Нет</translation>
    </message>
    <message>
        <source>&lt;b&gt;This shows peak outgoing bandwidth used.&lt;/b&gt;&lt;br /&gt;This shows the peak amount of bandwidth sent out from your machine. Audio bitrate is the maximum bitrate (as we use VBR) for the audio data alone. Position is the bitrate used for positional information. Overhead is our framing and the IP packet headers (IP and UDP is 75% of this overhead).</source>
        <translation type="obsolete">&lt;b&gt;Показывает пиковую загрузку посланных сигналов.&lt;/b&gt;&lt;br /&gt;Показывает пиковое значение скорости передачи данных от вашего компьютера. Битрейт аудио - максимальный битрейт (по принятым сокращениям, VBR) только для аудиоданных. Позиция - битрейт, используемый для информации о позиции. Загрузка - раскадровка и заголовки IP пакетов (IP и UDP - 90% этой загрузки). {75%?}</translation>
    </message>
    <message>
        <source>Volume</source>
        <translation type="obsolete">Громкость</translation>
    </message>
</context>
<context>
    <name>AudioInput</name>
    <message>
        <location filename="AudioInput.ui"/>
        <source>Form</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Interface</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>System</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Input method for audio</source>
        <translation type="unfinished">Метод ввода для аудио</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is the input method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation type="unfinished">&lt;b&gt;Этот метод ввода используется для аудио.&lt;/b&gt;&lt;br /&gt;Скорее всего вы захотите использовать DirectSound.</translation>
    </message>
    <message>
        <location/>
        <source>Device</source>
        <translation type="unfinished">Устройство</translation>
    </message>
    <message>
        <location/>
        <source>Input device for audio</source>
        <oldsource>Output method for audio</oldsource>
        <translation type="unfinished">Мытод вывода для аудио</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is the input device to use for audio.&lt;/b&gt;</source>
        <oldsource>&lt;b&gt;This is the output method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</oldsource>
        <translation type="unfinished">&lt;b&gt;Этот метод вывода используется для аудио.&lt;/b&gt;&lt;br /&gt;Скорее всего вы захотите использовать DirectSound.</translation>
    </message>
    <message>
        <location/>
        <source>Transmission</source>
        <translation type="unfinished">Передача</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Transmit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>When to transmit your speech</source>
        <translation type="unfinished">Когда передавать вашу речь</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets when speech should be transmitted.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;Continuous&lt;/i&gt; - All the time&lt;br /&gt;&lt;i&gt;Voice Activity&lt;/i&gt; - When you are speaking clearly.&lt;br /&gt;&lt;i&gt;Push To Talk&lt;/i&gt; - When you hold down the hotkey set under &lt;i&gt;Shortcuts&lt;/i&gt;.</source>
        <translation type="unfinished">&lt;b&gt;Устанавливает, когда передавать речь.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;Постоянно&lt;/i&gt; - Все время&lt;br /&gt;&lt;i&gt;При активность голоса&lt;/i&gt; - Когда вы говорите четко.&lt;br /&gt;&lt;i&gt;Нажмите для разговора&lt;/i&gt; - Когда вы зажимаете горячую клавишу, установленную в  &lt;i&gt;Ярлыках&lt;/i&gt;.</translation>
    </message>
    <message>
        <location/>
        <source>DoublePush Time</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>If you press the PTT key twice in this time it will get locked.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;DoublePush Time&lt;/b&gt;&lt;br /&gt;If you press the push-to-talk key twice during the configured interval of time it will be locked. Mumble will keep transmitting until you hit the key once more to unlock PTT again.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Gets played when the PTT button is pressed</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Gets played when the PTT button is released</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Reset audio cue to default</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Reset&lt;/b&gt;&lt;br/&gt;Reset the paths for the files to their default.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Reset</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Browse for on audio file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Browse</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Browse for off audio file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Off</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>On</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Preview the audio cues</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Preview&lt;/b&gt;&lt;br/&gt;Plays the current &lt;i&gt;on&lt;/i&gt; soundfile followed by the current &lt;i&gt;off&lt;/i&gt; soundfile.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Preview</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This enables the audio cues for push to talk.&lt;/b&gt;&lt;br /&gt;Setting this will give you a short audio beep when push to talk is pressed and released.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>PTT Audio cue</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Use SNR based speech detection</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Signal to Noise</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets speech detection to use Amplitude.&lt;/b&gt;&lt;br /&gt;In this mode, the raw strength of the input signal is used to detect speech.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Amplitude</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Voice &amp;Hold</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>How long to keep transmitting after silence</source>
        <translation type="unfinished">Как долго передавать после тишины</translation>
    </message>
    <message>
        <source>&lt;b&gt;This selects how long after a perceived stop in speech transmission should continue.&lt;/b&gt;&lt;br /&gt;Set this higher if your voice breaks up when you speak (seen by a rapidly blinking voice icon next to your name).&lt;br /&gt;Only makes sense when used with Voice Activity transmission.</source>
        <translation type="obsolete">&lt;b&gt;Выбирает, как долго после паузы в речи продолжать передачу.&lt;/b&gt;&lt;br /&gt;Установите ее больше, если у вас много пауз в речи (можно увидеть по часто мигающей иконке голоса рядом с вашим именем).&lt;br /&gt;Имеет смысл только когда используется с передачей по активности голоса.</translation>
    </message>
    <message>
        <location/>
        <source>Silence Below</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the trigger values for voice detection.&lt;/b&gt;&lt;br /&gt;Use this together with the Audio Statistics window to manually tune the trigger values for detecting speech. Input values below &quot;Silence Below&quot; always count as silence. Values above &quot;Speech Above&quot; always count as voice. Values in between will count as voice if you&apos;re already talking, but will not trigger a new detection.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Speech Above</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Signal values above this count as voice</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Compression</source>
        <translation type="unfinished">Сжатие</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Quality</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Quality of compression (peak bandwidth)</source>
        <translation type="unfinished">Качество сжатия (пиковая передача)</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the quality of compression.&lt;/b&gt;&lt;br /&gt;This determines how much bandwidth Mumble is allowed to use for outgoing audio.</source>
        <translation type="unfinished">&lt;b&gt;Устанавливает качество сжатия.&lt;/b&gt;&lt;br /&gt;Определяет, какую загрузку канала Mumble разрешено использовать для исходящего аудио.</translation>
    </message>
    <message>
        <source>Complexity of compression (CPU)</source>
        <translation type="obsolete">Сложность сжатия (ЦП)</translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets the complexity of compression.&lt;/b&gt;&lt;br /&gt;This determines how much CPU Mumble is allowed to use to increase transmitted voice quality. Settings above 5 give only marginal gain.</source>
        <translation type="obsolete">&lt;b&gt;Устанавливает сложность сжатия.&lt;/b&gt;&lt;br /&gt;Определяет, сколько мощности ЦП Mumble разрешено использовать для увеличения качества передаваемого голоса. Установка выше 5 даст только предельный прирост.</translation>
    </message>
    <message>
        <location/>
        <source>Audio per packet</source>
        <translation type="unfinished">Аудио на пакет</translation>
    </message>
    <message>
        <location/>
        <source>How many audio frames to send per packet</source>
        <translation type="unfinished">Как много аудио фрагментов посылать в пакете</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This selects how many audio frames should be put in one packet.&lt;/b&gt;&lt;br /&gt;Increasing this will increase the latency of your voice, but will also reduce bandwidth requirements.</source>
        <translation type="unfinished">&lt;b&gt;Выбирает, сколько аудиофрагментов помещать в пакет.&lt;/b&gt;&lt;br /&gt;Увеличение ее увеличит задержку голоса, но также уменьшит требования к толщине канала связи.</translation>
    </message>
    <message>
        <source>Maximum bandwidth used for sent audio</source>
        <translation type="obsolete">Максимальная скорость, используемая для посылки аудио</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This shows peak outgoing bandwidth used.&lt;/b&gt;&lt;br /&gt;This shows the peak amount of bandwidth sent out from your machine. Audio bitrate is the maximum bitrate (as we use VBR) for the audio data alone. Position is the bitrate used for positional information. Overhead is our framing and the IP packet headers (IP and UDP is 75% of this overhead).</source>
        <translation type="unfinished">&lt;b&gt;Показывает пиковую загрузку посланных сигналов.&lt;/b&gt;&lt;br /&gt;Показывает пиковое значение скорости передачи данных от вашего компьютера. Битрейт аудио - максимальный битрейт (по принятым сокращениям, VBR) только для аудиоданных. Позиция - битрейт, используемый для информации о позиции. Загрузка - раскадровка и заголовки IP пакетов (IP и UDP - 90% этой загрузки). {75%?}</translation>
    </message>
    <message>
        <location/>
        <source>Audio Processing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Noise Suppression</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Noise suppression</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the amount of noise suppression to apply.&lt;/b&gt;&lt;br /&gt;The higher this value, the more aggressively stationary noise will be suppressed.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Amplification</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Maximum amplification of input sound</source>
        <translation type="unfinished">Максимальное усиление входящего звука</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Maximum amplification of input.&lt;/b&gt;&lt;br /&gt;Mumble normalizes the input volume before compressing, and this sets how much it&apos;s allowed to amplify.&lt;br /&gt;The actual level is continually updated based on your current speech pattern, but it will never go above the level specified here.&lt;br /&gt;If the &lt;i&gt;Microphone loudness&lt;/i&gt; level of the audio statistics hover around 100%, you probably want to set this to 2.0 or so, but if, like most people, you are unable to reach 100%, set this to something much higher.&lt;br /&gt;Ideally, set it so &lt;i&gt;Microphone Loudness * Amplification Factor &gt;= 100&lt;/i&gt;, even when you&apos;re speaking really soft.&lt;br /&gt;&lt;br /&gt;Note that there is no harm in setting this to maximum, but Mumble will start picking up other conversations if you leave it to auto-tune to that level.</source>
        <translation type="unfinished">&lt;b&gt;Максимальное усиление входного голоса.&lt;/b&gt;&lt;br /&gt;Mumble нормализует входную громкость до сжатия, эта опция устанавливает, на сколько можно его усилить.&lt;br /&gt;Текущий уровень постоянно обновляется на основе текущего образца речи, но никогда не будет выше установленного здесь уровня.&lt;br /&gt;Если уровень &lt;i&gt;Громкости микрофона&lt;/i&gt; аудиостатистики держится на уровне 100%, вы можете установить его на 2.0 или выше, но если, как многие люди, вы не можете достичь 100%, установите его на чуть более высоком уровне.&lt;br /&gt;В идеале, установите его так, чтобы &lt;i&gt;Громкость микрофона * Фактор усиления &gt;= 100&lt;/i&gt;, даже если вы говорите слишком мягко.&lt;br /&gt;&lt;br /&gt;Заметьте, что ничего плохого не случится, если вы установите его на максимум, но Mumble начнет подбирать другие переговоры, если вы оставите его на автоматической настройке на данном уровне.</translation>
    </message>
    <message>
        <location/>
        <source>Current speech detection chance</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Cancel echo from speakers</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Enabling this will cancel the echo from your speakers. Mixed has low CPU impact, but only works well if your speakers are equally loud and equidistant from the microphone. Multichannel echo cancellation provides much better echo cancellation, but at a higher CPU cost.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Disabled</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Mixed</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Multichannel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Echo</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Audible audio cue when push-to-talk is activated or deactivated</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets speech detection to use Signal to Noise ratio.&lt;/b&gt;&lt;br /&gt;In this mode, the input is analyzed for something resembling a clear signal, and the clarity of that signal is used to trigger speech detection.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This shows the current speech detection settings.&lt;/b&gt;&lt;br /&gt;You can change the settings from the Settings dialog or from the Audio Wizard.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Idle AutoMute</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>How long to remain idle before auto-mute.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This sets an idle timer. If the timer expires without any audio being sent to the server, you are muted.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Signal values below this count as silence</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Maximum bandwidth used for sending audio</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.cpp" line="+528"/>
        <source>Server maximum network bandwidth is only %1 kbit/s. Audio quality auto-adjusted to %2 kbit/s (%3ms)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui"/>
        <source>Use Amplitude based speech detection</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This selects how long after a perceived stop in speech transmission should continue.&lt;/b&gt;&lt;br /&gt;Set this higher if your voice breaks up when you speak (seen by a rapidly blinking voice icon next to your name).</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>AudioInputDialog</name>
    <message>
        <location filename="AudioConfigDialog.cpp" line="+74"/>
        <source>Continuous</source>
        <translation type="unfinished">Продолжительный</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Voice Activity</source>
        <translation type="unfinished">Голосовая активность</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Push To Talk</source>
        <translation type="unfinished">Нажмите для разговора</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Audio Input</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+96"/>
        <location line="+8"/>
        <source>%1 ms</source>
        <translation type="unfinished">%1 мс</translation>
    </message>
    <message>
        <location line="-2"/>
        <location line="+20"/>
        <location line="+18"/>
        <source>Off</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-30"/>
        <source>%1 s</source>
        <translation type="unfinished">%1мс {1 ?}</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>%1 kb/s</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+11"/>
        <source>-%1 dB</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+55"/>
        <source>%1 kbit/s (Audio %2 %5, Position %4, Overhead %3)</source>
        <oldsource>%1kbit/s (Audio %2 %5, Position %4, Overhead %3)</oldsource>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+0"/>
        <source>CELT</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Speex</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>%1kbit/s (Audio %2, Position %4, Overhead %3)</source>
        <translation type="obsolete">%1кбит/с (Аудио %2, Позиция %4, Загрузка %3)</translation>
    </message>
    <message>
        <location line="-42"/>
        <source>%1 min</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>AudioOutput</name>
    <message>
        <location filename="AudioOutput.ui"/>
        <source>Form</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Interface</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>System</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Output method for audio</source>
        <oldsource>Input method for audio</oldsource>
        <translation type="unfinished">Метод ввода для аудио</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is the output method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <oldsource>&lt;b&gt;This is the input method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</oldsource>
        <translation type="unfinished">&lt;b&gt;Этот метод ввода используется для аудио.&lt;/b&gt;&lt;br /&gt;Скорее всего вы захотите использовать DirectSound.</translation>
    </message>
    <message>
        <location/>
        <source>Device</source>
        <translation type="unfinished">Устройство</translation>
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
    <message>
        <location/>
        <source>Positional Audio</source>
        <translation type="unfinished">Позиционированное аудио</translation>
    </message>
    <message>
        <location/>
        <source>Audio Output</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Default &amp;Jitter Buffer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Safety margin for jitter buffer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the minimum safety margin for the jitter buffer.&lt;/b&gt;&lt;br /&gt;All incoming audio is buffered, and the jitter buffer continually tries to push the buffer to the minimum sustainable by your network, so latency can be as low as possible. This sets the minimum buffer size to use. If the start of sentences you hear is very jittery, increase this value.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Minimum distance to user before sound volume decreases</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This sets the minimum distance for sound calculations. The volume of other users&apos; speech will not decrease until they are at least this far away from you.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This sets the maximum distance for sound calculations. When farther away than this, other users&apos; speech volume will not decrease any further.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This enables one of the loopback test modes.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;None&lt;/i&gt; - Loopback disabled&lt;br /&gt;&lt;i&gt;Local&lt;/i&gt; - Emulate a local server.&lt;br /&gt;&lt;i&gt;Server&lt;/i&gt; - Request loopback from server.&lt;br /&gt;Please note than when loopback is enabled, no other users will hear your voice. This setting is not saved on application exit.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Volume</source>
        <translation type="unfinished">Громкость</translation>
    </message>
    <message>
        <location/>
        <source>Volume of incoming speech</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This adjusts the volume of incoming speech.&lt;/b&gt;&lt;br /&gt;Note that if you increase this beyond 100%, audio will be distorted.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Output Delay</source>
        <translation type="unfinished">Задержка вывода</translation>
    </message>
    <message>
        <location/>
        <source>Amount of data to buffer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>MinDistance</source>
        <translation type="obsolete">МинДистанция</translation>
    </message>
    <message>
        <source>Minimum distance to player before sound decreases</source>
        <translation type="obsolete">Минимальное расстояние до игрока до уменьшения звука</translation>
    </message>
    <message>
        <source>This sets the minimum distance for sound calculations. The volume of other players&apos; speech will not decrease until they are at least this far away from you.</source>
        <translation type="obsolete">Устанавливает минимальное расстояние для расчета звука. Громкость речи других игроков не уменьшится до тех пор, пока они по крайней мере на таком расстоянии от вас.</translation>
    </message>
    <message>
        <source>MaxDistance</source>
        <translation type="obsolete">МаксДистанция</translation>
    </message>
    <message>
        <source>Maximum distance, beyond which sound won&apos;t decrease</source>
        <translation type="obsolete">Максимальное расстояние, больше которого звук не уменьшится</translation>
    </message>
    <message>
        <source>This sets the maximum distance for sound calculations. When farther away than this, other players&apos; sound volume will not decrease any more.</source>
        <translation type="obsolete">Устанавливает максимальное расстояния для рассчета звука. Даже дальше от вас, громкость других игроков больше не уменьшится.</translation>
    </message>
    <message>
        <source>RollOff</source>
        <translation type="obsolete">Откат</translation>
    </message>
    <message>
        <location/>
        <source>Factor for sound volume decrease</source>
        <translation type="unfinished">Фактор уменьшения громкости звука</translation>
    </message>
    <message>
        <source>How fast should sound volume drop when passing beyond the minimum distance. The normal (1.0) is that sound volume halves each time the distance doubles. Increasing this value means sound volume drops faster, while decreasing it means it drops slower.</source>
        <translation type="obsolete">Как быстро будет падать громкость звука после достижения определеной дистанции. Обычно (1.0) - громкость уменьшается вполовину при удвоении расстояния. Увеличение этого значения обозначает, что громкость падает быстрее, уменьшение - замедление падения.</translation>
    </message>
    <message>
        <location/>
        <source>Bloom</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Loopback Test</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Delay Variance</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Variance in packet latency</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the packet latency variance for loopback testing.&lt;/b&gt;&lt;br /&gt;Most audio paths contain some variable latency. This allows you to set that variance for loopback mode testing. For example, if you set this to 15ms, this will emulate a network with 20-35ms ping latency or one with 80-95ms latency. Most domestic net connections have a variance of about 5ms.</source>
        <oldsource>&lt;b&gt;This sets the packet latency variance for loopback testing.&lt;/b&gt;&lt;br /&gt;Most audio paths contain some variable latency. This allows you set that variance for loopback mode testing. For example, if you set this to 15ms, this will emulate a network with 20-35ms ping latency or one with 80-95ms latency. Most domestic net connections have a variance of about 5ms.</oldsource>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Packet Loss</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Packet loss for loopback mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the packet loss for loopback mode.&lt;/b&gt;&lt;br /&gt;This will be the ratio of packets lost. Unless your outgoing bandwidth is peaked or there&apos;s something wrong with your network connection, this will be 0%</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&amp;Loopback</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Desired loopback mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Factor for sound volume increase</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>The connected &quot;speakers&quot; are actually headphones</source>
        <oldsource>The connected &quot;speakers&quot; are actually headphones.</oldsource>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Minimum Distance</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Maximum Distance</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Maximum distance, beyond which speech volume won&apos;t decrease</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Minimum Volume</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>What should the volume be at the maximum distance?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>How much should sound volume increase for sources that are really close?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Checking this indicates that you don&apos;t have speakers connected, just headphones. This is important, as speakers are usually in front of you, while headphones are directly to your left/right.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Headphones</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This sets the amount of data to pre-buffer in the output buffer. Experiment with different values and set it to the lowest which doesn&apos;t cause rapid jitter in the sound.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Attenuate applications by...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Attenuation of other applications during speech</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Attenuate volume of other applications during speech&lt;/b&gt;&lt;br /&gt;Mumble supports decreasing the volume of other applications during incoming and/or outgoing speech. This sets the attenuation of other applications if the feature is enabled.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>If checked Mumble lowers the volume of other applications while other users talk</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Attenuate applications while other users talk&lt;/b&gt;&lt;br /&gt;Mumble supports decreasing the volume of other applications during incoming and/or outgoing speech. This makes mumble activate the feature while other users talk to you.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>while other users talk</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>If checked Mumble lowers the volume of other applications while you talk</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Attenuate applications while you talk&lt;/b&gt;&lt;br /&gt;Mumble supports decreasing the volume of other applications during incoming and/or outgoing speech. This makes mumble activate the feature while you talk.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>while you talk</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>AudioOutputDialog</name>
    <message>
        <location filename="AudioConfigDialog.cpp" line="+167"/>
        <source>None</source>
        <translation type="unfinished">Нет</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Local</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Server</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Audio Output</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+102"/>
        <location line="+19"/>
        <location line="+8"/>
        <source>%1 ms</source>
        <translation type="unfinished">%1 мс</translation>
    </message>
    <message>
        <location line="-16"/>
        <location line="+4"/>
        <location line="+8"/>
        <location line="+31"/>
        <location line="+4"/>
        <source>%1 %</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-16"/>
        <location line="+6"/>
        <source>%1 m</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>%1ms</source>
        <translation type="obsolete">%1мс</translation>
    </message>
    <message>
        <source>%1m</source>
        <translation type="obsolete">%1м</translation>
    </message>
    <message>
        <source>%1</source>
        <translation type="obsolete">%1</translation>
    </message>
</context>
<context>
    <name>AudioOutputSample</name>
    <message>
        <location filename="AudioOutput.cpp" line="+295"/>
        <source>Choose sound file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Invalid sound file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The file &apos;%1&apos; cannot be used by Mumble. Please select a file with a compatible format and encoding.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>AudioStats</name>
    <message>
        <location filename="AudioStats.ui"/>
        <source>Peak microphone level</source>
        <translation>Пиковый уровень микрофона</translation>
    </message>
    <message>
        <location/>
        <source>Peak speaker level</source>
        <translation>Пиковый уровень динамика</translation>
    </message>
    <message>
        <location/>
        <source>Peak clean level</source>
        <translation>Пиковый уровень очистки</translation>
    </message>
    <message>
        <source>Microphone loudness</source>
        <translation type="obsolete">Громкость микрофона</translation>
    </message>
    <message>
        <source>Mic Signal-To-Noise</source>
        <translation type="obsolete">Сигнал-шум Микрофона</translation>
    </message>
    <message>
        <location/>
        <source>Speech Probability</source>
        <translation>Вероятность речи</translation>
    </message>
    <message>
        <source>Audio bitrate</source>
        <translation type="obsolete">Битрейт аудио</translation>
    </message>
    <message>
        <location/>
        <source>Peak power in last frame</source>
        <translation>Пиковая мощность в последнем фрагменте</translation>
    </message>
    <message>
        <source>This shows the peak power in the last frame (20 ms), and is the same measurement as you would usually find displayed as &quot;input power&quot;. Please disregard this and look at &lt;b&gt;Loudness&lt;/b&gt; instead, which is much more steady and disregards outliers.</source>
        <translation type="obsolete">Показывает пиковую мощность в последнем фрагменте (20 мс), и измеряется также, как и &quot;входная мощность&quot;. Пожалуйста, оставьте ее и посмотрите на &lt;b&gt;Громкость&lt;/b&gt;, которая более устойчива и не пренебрегается.</translation>
    </message>
    <message>
        <location/>
        <source>This shows the peak power of the speakers in the last frame (20 ms). Unless you are using a multi-channel sampling method (such as ASIO) with speaker channels configured, this will be 0. If you have such a setup configured, and this still shows 0 while you&apos;re playing audio from other programs, your setup is not working.</source>
        <oldsource>This shows the peak power in the last frame (20 ms) of the speakers. Unless you are using a multi-channel sampling method (such as ASIO) with speaker channels configured, this will be 0. If you have such a setup configured, and this still shows 0 while you&apos;re playing audio from other programs, your setup is not working.</oldsource>
        <translation type="unfinished">Показывает пиковую мощность в последнем фрагменте (20 мс) динамиков. Пока вы используете мультиканальный метод проб (такой как ASIO),настроенный на каналы динамика, она будет равна 0. Если у вас они так сконфигурированы, все еще будет отображаться 0, пока вы будете прослушивать звук из других программ, ваши настройки не будут работать.</translation>
    </message>
    <message>
        <source>This shows the peak power in the last frame (20 ms) after all processing. Ideally, this should be -96 dB when you&apos;re not talking. In reality, a sound studio should see -60 dB, and you should hopefully see somewhere around -20 dB. When you are talking, this should rise to somewhere between -5 and -10 dB.&lt;br /&gt;If you are using echo cancellation, and this rises to more than -15 dB when you&apos;re not talking, your setup is not working, and you&apos;ll annoy other players with echoes.</source>
        <translation type="obsolete">Показывает пиковую мощность в последнем фрагменте (20 мс) после всех обработок. В идеале, должна быть -96 dB когда вы не разговариваете. На самом деле, звуковая программа примет -60 dB, мы можете с большой вероятностью увидеть -20 dB. Когда вы говорите, его следует поднять до уровня от -5 до -10 dB.&lt;br /&gt;Если вы используете подавление эха, и оно поднимается выше -15 dB когда вы не говорите, ваши настройки не работают, и вы будете надоедать другим своим эхом.</translation>
    </message>
    <message>
        <location/>
        <source>How close the current input level is to ideal</source>
        <translation>Как близок текущий входной уровень к идеалу</translation>
    </message>
    <message>
        <source>This shows how close your current input volume is to the ideal. To adjust your microphone level, open whatever program you use to adjust the recording volume, and look at the value here while talking.&lt;br /&gt;&lt;b&gt;Talk loud, as you would when you&apos;re upset over getting fragged by a noob.&lt;/b&gt;&lt;br /&gt;Adjust the volume until this value is close to 100%, but make sure it doesn&apos;t go above. If it does go above, you are likely to get clipping in parts of your speech, which will degrade sound quality</source>
        <translation type="obsolete">Как близок текущий входной уровень к идеалу. Чтобы приспособить уровень микрофона, откройте любую программу, которую вы используете для приспособления громкости записи, и посмотрите на значение во время разговора.&lt;br /&gt;&lt;b&gt;Говорите громко, как будто от досады, когда вас только что подстрелил нуб.&lt;/b&gt;&lt;br /&gt;Подгоняйте громкость пока это значение не приблизится к 100%, но убедитесь, что оно не выше. Если оно заходит выше, вы будто обрезаете часть своей речи, что понизит качество звука</translation>
    </message>
    <message>
        <location/>
        <source>This shows the peak power in the last frame (20 ms) after all processing. Ideally, this should be -96 dB when you&apos;re not talking. In reality, a sound studio should see -60 dB, and you should hopefully see somewhere around -20 dB. When you are talking, this should rise to somewhere between -5 and -10 dB.&lt;br /&gt;If you are using echo cancellation, and this rises to more than -15 dB when you&apos;re not talking, your setup is not working, and you&apos;ll annoy other users with echoes.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Signal-To-Noise ratio from the microphone</source>
        <translation>Соотношение Сигнал-шум микрофона</translation>
    </message>
    <message>
        <location/>
        <source>This is the Signal-To-Noise Ratio (SNR) of the microphone in the last frame (20 ms). It shows how much clearer the voice is compared to the noise.&lt;br /&gt;If this value is below 1.0, there&apos;s more noise than voice in the signal, and so quality is reduced.&lt;br /&gt;There is no upper limit to this value, but don&apos;t expect to see much above 40-50 without a sound studio.</source>
        <translation>Отношение Сигнал-шум (SNR) микрофона в последнем фрагменте (20 мс). Показывает, насколько чист голос по сравнению с шумом.&lt;br /&gt;Если значение ниже 1.0, в сигнале больше шума, нежели голоса, и поэтому ухудшается качество.&lt;br /&gt;Верхнего предела не существует, но не ожидайте увидеть выше 40-50 без студии звукозаписи.</translation>
    </message>
    <message>
        <location/>
        <source>Probability of speech</source>
        <translation>Вероятность речи</translation>
    </message>
    <message>
        <location/>
        <source>This is the probability that the last frame (20 ms) was speech and not environment noise.&lt;br /&gt;Voice activity transmission depends on this being right. The trick with this is that the middle of a sentence is always detected as speech; the problem is the pauses between words and the start of speech. It&apos;s hard to distinguish a sigh from a word starting with &apos;h&apos;.&lt;br /&gt;If this is in bold font, it means Mumble is currently transmitting (if you&apos;re connected).</source>
        <translation>Вероятность того, что последний фрагмент (20 мс) был речью, а не шумом.&lt;br /&gt;Передача голосовой активности зависит от верности этого. Фишка в том, что середина предложения всегда распознается как речь; проблема в паузах между словами и началом разговора. Трудно отделить кашель от слова, начинающегося на  &apos;х&apos;.&lt;br /&gt;Если это выделено жирным шрифтом, значит Mumble сейчас передает (если вы подключены).</translation>
    </message>
    <message>
        <location/>
        <source>Bitrate of last frame</source>
        <translation>Битрейт последнего фрагмента</translation>
    </message>
    <message>
        <location/>
        <source>This is the audio bitrate of the last compressed frame (20 ms), and as such will jump up and down as the VBR adjusts the quality. To adjust the peak bitrate, adjust &lt;b&gt;Compression Complexity&lt;/b&gt; in the Settings dialog.</source>
        <translation>Аудио битрейт последнего сжатого фрагмента (20 мс), и поэтому оно может варьироваться, как и VBR применительно к качеству. Чтобы приспособить пиковый битрейт, приспособьте &lt;b&gt;Сложность сжатия&lt;/b&gt; в диалоге настроек.</translation>
    </message>
    <message>
        <location/>
        <source>Time between last two Push-To-Talk presses</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Power spectrum of input signal and noise estimate</source>
        <translation>Мощностной спектр входного сигнала и оценка шума</translation>
    </message>
    <message>
        <location/>
        <source>This shows the power spectrum of the current input signal (red line) and the current noise estimate (filled blue).&lt;br /&gt;All amplitudes are multiplied by 30 to show the interesting parts (how much more signal than noise is present in each waveband).&lt;br /&gt;This is probably only of interest if you&apos;re trying to fine-tune noise conditions on your microphone. Under good conditions, there should be just a tiny flutter of blue at the bottom. If the blue is more than halfway up on the graph, you have a seriously noisy environment.</source>
        <translation>Показывает мощностной спектр текущего входного сигнала (красная линия) и текущая оценка шума (выполнена синим).&lt;br /&gt;Все амплитуды умножены на 30, чтобы показать интересные части (на сколько больше сигнала, чем шума представлено в каждом отрезке).&lt;br /&gt;Это только если вы интересуетесь точными условиями шума вашего микрофона. При хороших условиях, это будет всего лишь как крошечный синий отрезок внизу. Если синего на графике более половины, у вас серьезные проблемы с шумом среды.</translation>
    </message>
    <message>
        <location/>
        <source>Weights of the echo canceller</source>
        <translation>Значения подавителя эха</translation>
    </message>
    <message>
        <location/>
        <source>This shows the weights of the echo canceller, with time increasing downwards and frequency increasing to the right.&lt;br /&gt;Ideally, this should be black, indicating no echo exists at all. More commonly, you&apos;ll have one or more horizontal stripes of bluish color representing time delayed echo. You should be able to see the weights updated in real time.&lt;br /&gt;Please note that as long as you have nothing to echo off, you won&apos;t see much useful data here. Play some music and things should stabilize. &lt;br /&gt;You can choose to view the real or imaginary parts of the frequency-domain weights, or alternately the computed modulus and phase. The most useful of these will likely be modulus, which is the amplitude of the echo, and shows you how much of the outgoing signal is being removed at that time step. The other viewing modes are mostly useful to people who want to tune the echo cancellation algorithms.&lt;br /&gt;Please note: If the entire image fluctuates massively while in modulus mode, the echo canceller fails to find any correlation whatsoever between the two input sources (speakers and microphone). Either you have a very long delay on the echo, or one of the input sources is configured wrong.</source>
        <translation>Отображает значение подавителя эха, со временем растущего вниз и увеличением частоты вправо.&lt;br /&gt;В идеале, должен быть черным, отображая, что эха нет. В общем, у вас будут 1 или несколько горизонтальных полосок синеватого цвета, отображая время задержки эха. Вы должны увидеть значения в реальном времени.&lt;br /&gt;Заметьте пожалуйста, что если вы не отключите эхо, вы не увидите здесь полезной информации. Запустите музыку, и все должно нормализоваться. &lt;br /&gt;Вы можете выбрать просмотр реальных или воображаемых частей доменно-частотных значений, или наоборот высчитанные модуль и фазу. Наиболее применимым из них будут, скорее всего модуль, который является амплитудой эха, и показывает, сколько сигнала было вырезано за промежуток времени. Другие режимы просмотра более полезны для людей, желающих настроить алгоритмы глушения эха.&lt;br /&gt;Пожалуйста, заметьте: Если картина в целом обширно варьируется в режиме модулей, подавитель эха не модет найти никакого соответствия между двумя входными источниками (динамиками и микрофоном). Или у вас очень долгая задержка эха, или один из входных источников настроен неправильно.</translation>
    </message>
    <message>
        <source>Mumble</source>
        <translation type="obsolete">Mumble</translation>
    </message>
    <message>
        <location filename="AudioStats.cpp" line="+405"/>
        <source>&gt;1000 ms</source>
        <oldsource>&gt;1000ms</oldsource>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioStats.ui"/>
        <source>Audio Statistics</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Input Levels</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This shows the peak power in the last frame (20 ms), and is the same measurement as you would usually find displayed as &quot;input power&quot;. Please disregard this and look at &lt;b&gt;Microphone power&lt;/b&gt; instead, which is much more steady and disregards outliers.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Signal Analysis</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Microphone power</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This shows how close your current input volume is to the ideal. To adjust your microphone level, open whatever program you use to adjust the recording volume, and look at the value here while talking.&lt;br /&gt;&lt;b&gt;Talk loud, as you would when you&apos;re upset over getting fragged by a noob.&lt;/b&gt;&lt;br /&gt;Adjust the volume until this value is close to 100%, but make sure it doesn&apos;t go above. If it does go above, you are likely to get clipping in parts of your speech, which will degrade sound quality.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Signal-To-Noise ratio</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Configuration feedback</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Current audio bitrate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>DoublePush interval</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Speech Detection</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Current speech detection chance</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This shows the current speech detection settings.&lt;/b&gt;&lt;br /&gt;You can change the settings from the Settings dialog or from the Audio Wizard.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Signal and noise power spectrum</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Echo Analysis</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>AudioWizard</name>
    <message>
        <location filename="AudioWizard.ui"/>
        <source>Audio Tuning Wizard</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Introduction</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Welcome to the Mumble Audio Wizard</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Finished</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Enjoy using Mumble</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Device selection</source>
        <translation type="unfinished">Выбор устройства</translation>
    </message>
    <message>
        <location/>
        <source>Selecting the input and output device to use with Mumble.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This is the device your microphone is connected to.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>System</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Input method for audio</source>
        <translation type="unfinished">Метод ввода для аудио</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is the input method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation type="unfinished">&lt;b&gt;Этот метод ввода используется для аудио.&lt;/b&gt;&lt;br /&gt;Скорее всего вы захотите использовать DirectSound.</translation>
    </message>
    <message>
        <location/>
        <source>Device</source>
        <translation type="unfinished">Устройство</translation>
    </message>
    <message>
        <location/>
        <source>Input device to use</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Selects which sound card to use for audio input.&lt;/b&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This is the device your speakers or headphones are connected to.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Output method for audio</source>
        <translation type="unfinished">Мытод вывода для аудио</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is the Output method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Output device to use</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Selects which sound card to use for audio Output.&lt;/b&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Volume tuning</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Tuning microphone hardware volume to optimal settings.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Now talk softly, as you would when talking late at night and you don&apos;t want to disturb anyone. Adjust the slider below so that the bar moves into green when you talk, but stays blue while you&apos;re silent.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Voice Activity Detection</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Letting Mumble figure out when you&apos;re talking and when you&apos;re silent.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This will help Mumble figure out when you are talking. The first step is selecting which data value to use.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Raw amplitude from input</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Signal-To-Noise ratio</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Next you need to adjust the following slider. The first few utterances you say should end up in the green area (definitive speech). While talking, you should stay inside the yellow (might be speech) and when you&apos;re not talking, everything should be in the red (definitively not speech).</source>
        <oldsource>Next you need to adjust the following two sliders. The first few utterances you say should end up in the green area (definitive speech). While talking, you should stay inside the yellow (might be speech) and when you&apos;re not talking, everything should be in the red (definitively not speech).</oldsource>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Device tuning</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Changing hardware output delays to their minimum value.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>%1ms</source>
        <translation type="obsolete">%1мс</translation>
    </message>
    <message>
        <location/>
        <source>Use echo cancellation</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This enables echo cancellation of outgoing audio, which helps both on speakers and on headsets.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Enable positional audio</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Allows positioning of sound</source>
        <oldsource>Allows positioning of sound.</oldsource>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;p&gt;
This is the audio tuning wizard for Mumble. This will help you correctly set the input levels of your sound card, and also set the correct parameters for sound processing in Mumble.
&lt;/p&gt;
&lt;p&gt;
Please be aware that as long as this wizard is active, audio will be looped locally to allow you to listen to it, and no audio will be sent to the server.
&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Input Device</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Cancel echo from headset or speakers</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Output Device</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This allows Mumble to use positional audio to place voices.</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Amount of data to buffer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This sets the amount of data to pre-buffer in the output buffer. Experiment with different values and set it to the lowest which doesn&apos;t cause rapid jitter in the sound.</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Positional Audio</source>
        <translation type="unfinished">Позиционированное аудио</translation>
    </message>
    <message>
        <location/>
        <source>Adjusting attenuation of positional audio.</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Use headphones instead of speakers</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Use headphones</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This ignores the OS speaker configuration and configures the positioning for headphones instead.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Submit anonymous statistics to the Mumble project</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Push To Talk:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Quality &amp; Notifications</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Adjust quality and notification settings.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Quality settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Low</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Balanced</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Notification settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Use Text-To-Speech to read notifications and messages to you.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Disable Text-To-Speech and use sounds instead.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="+304"/>
        <source>%1 ms</source>
        <translation type="unfinished">%1 мс</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui"/>
        <source>Enables attenuation of other applications while users talk to you</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Attenuate applications while other users talk</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>In this configuration Mumble will use a &lt;b&gt;low amount of bandwidth&lt;/b&gt;. This will inevitably result in high latency and poor quality. Choose this only if your connection cannot handle the other settings. (Speex 16kbit/s, 60ms per packet)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This is the &lt;b&gt;recommended default&lt;/b&gt; configuration. It provides a good balance between quality, latency, and bandwidth usage. (CELT 40kbit/s, 20ms per packet)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>High</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This configuration is only recommended for use in setups where bandwidth is not an issue, like a LAN. It provides the lowest latency supported by Mumble and &lt;b&gt;high quality&lt;/b&gt;. (CELT 72kbit/s, 10ms per packet)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Custom</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>You already set a customized quality configuration in Mumble. Select this setting to keep it.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Enables attenuation of other applications while users talk to you. This means that as soon someone starts to speak to you in Mumble, the sound of all other applications (like audio players) will get attenuated so you can hear them more clearly.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Apply some high contrast optimizations for visually impaired users</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Use high contrast graphics</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Keep custom Text-To-Speech settings.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>BanEditor</name>
    <message>
        <location filename="BanEditor.ui"/>
        <source>Mumble - Edit Bans</source>
        <translation>Mumble - изменить баны</translation>
    </message>
    <message>
        <location/>
        <source>Reason</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Start</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>End</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>User</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Hash</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&amp;Add</source>
        <translation>&amp;Добавить</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Update</source>
        <translation>&amp;Обновить</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Remove</source>
        <translation>&amp;Удалить</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">О&amp;к</translation>
    </message>
    <message>
        <source>Accept changes</source>
        <translation type="obsolete">Принять изменения</translation>
    </message>
    <message>
        <source>This button will accept current groups/ACLs and send them to the server. Note that if you mistakenly remove write permission from yourself, the server will add it.</source>
        <translation type="obsolete">Данная кнопка примет текущие группы/списки ACL и пошлет их серверу. Заметьте, что если вы по ошибке уберете права на запись с себя, сервер добавит их.</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">Отм&amp;ена</translation>
    </message>
    <message>
        <source>Reject changes</source>
        <translation type="obsolete">Отменяет изменения</translation>
    </message>
    <message>
        <source>This button will cancels all changes and closes the dialog without updating the ACLs or groups on the server.</source>
        <translation type="obsolete">Данная кнопка отменит все изменения и закроет окно без обновления списков ACL или групп на сервере.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Address</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&amp;Mask</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>CertView</name>
    <message>
        <location filename="Cert.cpp" line="+50"/>
        <source>Name</source>
        <translation type="unfinished">Имя</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Email</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Issuer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Expiry Date</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+34"/>
        <source>(none)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+11"/>
        <source>Self-signed</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>CertWizard</name>
    <message>
        <location line="+90"/>
        <source>Resolving domain %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Unable to validate email.&lt;br /&gt;Enter a valid (or blank) email to continue.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+8"/>
        <source>There was an error generating your certificate.&lt;br /&gt;Please try again.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Your certificate and key could not be exported to PKCS#12 format. There might be an error in your certificate.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>The file could not be opened for writing. Please use another file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>The file could not be written successfully. Please use another file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+7"/>
        <source>The file could not be opened for reading. Please use another file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>The file is empty or could not be read. Please use another file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>The file did not contain a valid certificate and key. Please use another file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+22"/>
        <source>Select file to export certificate to</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+37"/>
        <source>Select file to import certificate from</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+57"/>
        <source>Unable to resolve domain.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+43"/>
        <source>Mumble User</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="main.cpp" line="+409"/>
        <source>&lt;b&gt;Certificate Expiry:&lt;/b&gt; Your certificate is about to expire. You need to renew it, or you will no longer be able to connect to servers you are registered on.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Certificates</name>
    <message>
        <location filename="Cert.ui"/>
        <source>Certificate Management</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Certificate Authentication</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Authenticating to servers without using passwords</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Current certificate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This is the certificate Mumble currently uses.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Current Certificate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Create a new certificate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This will create a new certificate.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Import certificate from file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This will import a certificate from file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Import a certificate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Export Certificate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This will export a certificate to file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Export current certificate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Import Certificate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>PKCS #12 Certificate import</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;p&gt;Mumble can import certificates stored in PKCS #12 format. This is the format used when exporting a key from Mumble, and also when exporting keys from Firefox, Internet Explorer, Opera etc.&lt;/p&gt;&lt;p&gt;If the file is password protected, you will need the password to import the certificate.&lt;/p&gt;</source>
        <oldsource>&lt;p&gt;Mumble can import certificates stored in PKCS #12 format. This is the format used when exporting a key from Mumble, and also when exporting keys from FireFox, Internet Explorer, Opera etc.&lt;/p&gt;&lt;p&gt;If the file is password protected, you will need the password to import the certificate.&lt;/p&gt;</oldsource>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Import from</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Filename to import from</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This is the filename you wish to import a certificate from.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Select file to import from</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This opens a file selection dialog to choose a file to import a certificate from.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Open...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Password</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Password for PKCS#12 file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This is the password for the PKCS#12 file containing your certificate.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Certificate to import</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This is the certificate you are importing.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Certificate Details</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Replace Certificate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Replace existing certificate with new certificate?</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This is the certificate Mumble currently uses. It will be replaced.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>New certificate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This is the new certificate that will replace the old one.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>New Certificate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Make a backup of your certificate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;p&gt;If you ever lose your current certificate, which will happen if your computer suffers a hardware failure or you reinstall your machine, you will no longer be able to authenticate to any server you are registered on. It is therefore &lt;b&gt;mandatory&lt;/b&gt; that you make a backup of your certificate. We strongly recommend you store this backup on removable storage, such as an USB memory stick.&lt;/p&gt;
&lt;p&gt;Note that this file will not be encrypted, and if anyone gains access to it, they will be able to impersonate you, so take good care of it.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Export to</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Filename to export to</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This is the filename you wish to export a certificate to.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Save As...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This is the certificate Mumble currently uses. It will be exported.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Generate a new certificate for strong authentication</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;p&gt;Mumble will now generate a strong certificate for authentication to servers.&lt;/p&gt;&lt;p&gt;If you wish, you may provide some additional information to be stored in the certificate, which will be presented to servers when you connect. If you provide a valid email address, you can upgrade to a CA issued email certificate later on, which provides strong identification.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Name</source>
        <translation type="unfinished">Имя</translation>
    </message>
    <message>
        <location/>
        <source>Email</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Your email address (e.g. johndoe@mumble.info)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This is your email address. It is strongly recommended to provide a valid email address, as this will allow you to upgrade to a strong certificate without authentication problems.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Your name (e.g. John Doe)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This is your name, and will be filled out in the certificate. This field is entirely optional.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Finish</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Certificate-based authentication is ready for use</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Enjoy using Mumble with strong authentication.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Automatic certificate creation</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;p&gt;Mumble can use certificates to authenticate with servers. Using certificates avoids passwords, meaning you don&apos;t need to disclose any password to the remote site. It also enables very easy user registration.&lt;/p&gt;&lt;p&gt;While Mumble can work without certificates, the majority of servers will expect you to have one.&lt;/p&gt;
&lt;p&gt;
It is &lt;b&gt;strongly&lt;/b&gt; recommended that you &lt;a href=&quot;http://mumble.info/certificate.php&quot;&gt;create a trusted certificate&lt;/a&gt;.
&lt;/p&gt;
</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ChanACL</name>
    <message>
        <source>W</source>
        <translation type="obsolete">З</translation>
    </message>
    <message>
        <source>T</source>
        <translation type="obsolete">Пер</translation>
    </message>
    <message>
        <source>E</source>
        <translation type="obsolete">В</translation>
    </message>
    <message>
        <source>S</source>
        <translation type="obsolete">Р</translation>
    </message>
    <message>
        <source>A</source>
        <translation type="obsolete">А</translation>
    </message>
    <message>
        <source>M</source>
        <translation type="obsolete">Г</translation>
    </message>
    <message>
        <source>K</source>
        <translation type="obsolete">П</translation>
    </message>
    <message>
        <source>C</source>
        <translation type="obsolete">Сзд</translation>
    </message>
    <message>
        <source>L</source>
        <translation type="obsolete">Св</translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="+214"/>
        <source>None</source>
        <translation>Нет</translation>
    </message>
    <message>
        <source>Write</source>
        <translation type="obsolete">Запись</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Traverse</source>
        <translation>Пересекать</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Enter</source>
        <translation>Вход</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Speak</source>
        <translation>Разговор</translation>
    </message>
    <message>
        <source>AltSpeak</source>
        <translation type="obsolete">Альтернативный разговор</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Mute/Deafen</source>
        <translation>Глушить</translation>
    </message>
    <message>
        <source>Move/Kick</source>
        <translation type="obsolete">Переместить/выкинуть</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Make channel</source>
        <translation>Создать канал</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Make temporary</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Link channel</source>
        <translation>Связать канал</translation>
    </message>
    <message>
        <location line="-87"/>
        <source>This represents no privileges.</source>
        <translation>Представляет отсутствие привелегий.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>This represents total access to the channel, including the ability to change group and ACL information. This privilege implies all other privileges.</source>
        <translation>Предоставляет полный доступ к каналу, веключая возможность изменять информацию о группах и ACL. Эта привелегия включает все привелегии.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>This represents the permission to traverse the channel. If a user is denied this privilege, he will be unable to access this channel and any sub-channels in any way, regardless of other permissions in the sub-channels.</source>
        <translation>Предоставляет разрешение пересекать канал. Если у пользователя нет такой привилегии, он никак не сможет получить доступ к каналу и подканалам, независимо от разрешений подканалов.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>This represents the permission to join the channel. If you have a hierarchical channel structure, you might want to give everyone Traverse, but restrict Enter in the root of your hierarchy.</source>
        <translation>Предоставляет разрешение входить на канал. Если у канала иерархическая структура, вы можете дать каждому право пересекать канал, но ограничивать Вход в корне вашей иерархии.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>This represents the permission to speak in a channel. Users without this privilege will be suppressed by the server (seen as muted), and will be unable to speak until they are unmuted by someone with the appropriate privileges.</source>
        <translation>Предоставляет разрешение говорить на канале. Пользователи без этой привелегии будут заглушены сервером, и неспособны будут говорить, пока не получат разрешение от кого-то, имеющего соответствующие привелегии.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>This represents the permission to whisper to this channel from the outside. This works exactly like the &lt;i&gt;speak&lt;/i&gt; privilege, but applies to packets spoken with the Whisper key held down. This may be used to broadcast to a hierarchy of channels without linking.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>This represents the permission to mute and deafen other users. Once muted, a user will stay muted until he is unmuted by another privileged user or reconnects to the server.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>This represents the permission to move a user to another channel or kick him from the server. To actually move the user, either the moving user must have Move privileges in the destination channel, or the user must normally be allowed to enter the channel. Users with this privilege can move users into channels the target user normally wouldn&apos;t have permission to enter.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>This represents the permission to make sub-channels. The user making the sub-channel will be added to the admin group of the sub-channel.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>This represents the permission to make a temporary subchannel. The user making the sub-channel will be added to the admin group of the sub-channel. Temporary channels are not stored and disappear when the last user leaves.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>This represents the permission to link channels. Users in linked channels hear each other, as long as the speaking user has the &lt;i&gt;speak&lt;/i&gt; privilege in the channel of the listener. You need the link privilege in both channels to create a link, but just in either channel to remove it.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>This represents the permission to write text messages to other users in this channel.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>This represents the permission to forcibly remove users from the server.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>This represents the permission to permanently remove users from the server.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>This represents the permission to register and unregister users on the server.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>This represents the permission to register oneself on the server.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+31"/>
        <source>Whisper</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Move</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Text message</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Kick</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Ban</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Register User</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Register Self</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This represents the permission to speak in a channel with flagged speech. This works exactly like the &lt;i&gt;speak&lt;/i&gt; privilege, but applies to packets spoken with AltPushToTalk held down. This may be used to broadcast to a hierarchy of channels without linking.</source>
        <translation type="obsolete">Преставляет разрешение говорить на канале с ослабленной речью. Работает также, как а привелегия &lt;i&gt;говорить&lt;/i&gt;, но применяется к пакетам, с зажатой кнопкой Нажмите чтобы говорить - переменно. Может использоваться для передачи к иерархии каналов без связи.</translation>
    </message>
    <message>
        <source>This represents the permission to mute and deafen other players. Once muted, a player will stay muted until he is unmuted by another privileged player or reconnects to the server.</source>
        <translation type="obsolete">Предоставляет привелегии заглушать других игроков. Будучи заглушенным, игрок останется заглушенным пока не будет разглушен другим привелигериванным игроком или не переподключится к серверу.</translation>
    </message>
    <message>
        <source>This represents the permission to move a player to another channel or kick him from the server. To actually move the player, either the moving player must have Move/Kick privileges in the destination channel, or the player must normally be allowed to enter the channel. Players with this privilege can move players into channels the target player normally wouldn&apos;t have permission to enter.</source>
        <translation type="obsolete">Предоставляет разрешение перемещать игрока на другие каналы или выкидывать его с сервера. Чтобы действительно переместить игрока, игрок должен обладать привелегиями Переместить/Выкинуть на сервере назначения, или игрок должен быть допущен ко входу на сервер. Игроки с этой привелегией могут перемещать игроков на каналы, входить на которые у пользователя нет разрешения.</translation>
    </message>
    <message>
        <source>This represents the permission to make sub-channels. The player making the sub-channel will be added to the admin group of the sub-channel.</source>
        <translation type="obsolete">Предоставляет разрешение создавать подканалы. Игрок, создающий подканал, будет добавлен в группу администраторов подканала.</translation>
    </message>
    <message>
        <source>This represents the permission to link channels. Players in linked channels hear each other, as long as the speaking player has the &lt;i&gt;speak&lt;/i&gt; privilege in the channel of the listener. You need the link privilege in both channels to create a link, but just in either channel to remove it.</source>
        <translation type="obsolete">Предоставляет привелегию связывать каналы. Игроки на связанных каналах могут слышат друг друга, также, как и говорящий игрок имеет право &lt;i&gt;говорить&lt;/i&gt; на канале слушающего. Вы должны связать привелегии на обоих каналах, чтобы создать связь, но на другом канале, чтобы удалить ее.</translation>
    </message>
    <message>
        <location line="-28"/>
        <source>Write ACL</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ChatbarLineEdit</name>
    <message>
        <location filename="CustomElements.cpp" line="+71"/>
        <source>Paste and send</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+14"/>
        <source>Type chat message here</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ClientUser</name>
    <message>
        <location filename="ClientUser.cpp" line="+122"/>
        <source>Friend</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Authenticated</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Muted (server)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Deafened (server)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Local Mute</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Muted (self)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Deafened (self)</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ConfigDialog</name>
    <message>
        <location filename="ConfigDialog.ui"/>
        <source>Mumble Configuration</source>
        <translation>Настройки Mumble</translation>
    </message>
    <message>
        <location/>
        <location filename="ConfigDialogDelegate.mm" line="+135"/>
        <source>Advanced</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Ок</translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="+51"/>
        <location filename="ConfigDialog.mm" line="+73"/>
        <source>Accept changes</source>
        <translation>Принять изменения</translation>
    </message>
    <message>
        <location line="+1"/>
        <location filename="ConfigDialog.mm" line="+1"/>
        <source>This button will accept current settings and return to the application.&lt;br /&gt;The settings will be stored to disk when you leave the application.</source>
        <translation>Кнопка примет текущие настройки и вернется в приложении.&lt;br /&gt;Настройки будут сохранены на диск, когда вы выйдете из приложения.</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Отмена</translation>
    </message>
    <message>
        <location line="+4"/>
        <location filename="ConfigDialog.mm" line="+4"/>
        <source>Reject changes</source>
        <translation>Отменить изменения</translation>
    </message>
    <message>
        <location line="+1"/>
        <location filename="ConfigDialog.mm" line="+1"/>
        <source>This button will reject all changes and return to the application.&lt;br /&gt;The settings will be reset to the previous positions.</source>
        <translation>Отменит все изменения и вернется в приложение.&lt;br /&gt;Настройки будут сброшены до предыдущих значений.</translation>
    </message>
    <message>
        <source>&amp;Apply</source>
        <translation type="obsolete">&amp;Применить</translation>
    </message>
    <message>
        <location line="+4"/>
        <location filename="ConfigDialog.mm" line="+4"/>
        <source>Apply changes</source>
        <translation>Применить изменения</translation>
    </message>
    <message>
        <location line="+1"/>
        <location filename="ConfigDialog.mm" line="+1"/>
        <source>This button will immediately apply all changes.</source>
        <translation>Немедленно применяет все изменения.</translation>
    </message>
    <message>
        <location line="+3"/>
        <location filename="ConfigDialog.mm" line="+3"/>
        <source>Undo changes for current page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <location filename="ConfigDialog.mm" line="+1"/>
        <source>This button will revert any changes done on the current page to the most recent applied settings.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <location filename="ConfigDialog.mm" line="+3"/>
        <source>Restore defaults for current page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <location filename="ConfigDialog.mm" line="+1"/>
        <source>This button will restore the settings for the current page only to their defaults. Other pages will not be changed.&lt;br /&gt;To restore all settings to their defaults, you will have to use this button on every page.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ConnectDialog</name>
    <message>
        <source>&amp;Name</source>
        <translation type="obsolete">&amp;Имя</translation>
    </message>
    <message>
        <source>A&amp;ddress</source>
        <translation type="obsolete">А&amp;дрес</translation>
    </message>
    <message>
        <source>&amp;Port</source>
        <translation type="obsolete">&amp;Порт</translation>
    </message>
    <message>
        <source>&amp;Username</source>
        <translation type="obsolete">Имя п&amp;ользователя</translation>
    </message>
    <message>
        <source>&amp;Password</source>
        <translation type="obsolete">П&amp;ароль</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="+735"/>
        <source>&amp;Connect</source>
        <translation type="unfinished">&amp;Связаться</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Отмена</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation type="obsolete">&amp;Добавить</translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation type="obsolete">&amp;Удалить</translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="obsolete">Имя</translation>
    </message>
    <message>
        <location line="+133"/>
        <source>Connecting to %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-470"/>
        <location line="+470"/>
        <source>Enter username</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-470"/>
        <source>Adding host %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui"/>
        <location filename="ConnectDialog.cpp" line="+63"/>
        <source>Servername</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="+1"/>
        <source>Hostname</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Bonjour name</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Port</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Addresses</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Website</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Packet loss</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Ping (80%)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+0"/>
        <location line="+2"/>
        <source>%1 ms</source>
        <translation type="unfinished">%1 мс</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Ping (95%)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Bandwidth</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+0"/>
        <source>%1 kbit/s</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+292"/>
        <source>&amp;Filters</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui"/>
        <location filename="ConnectDialog.cpp" line="-291"/>
        <source>Users</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="+1"/>
        <source>Version</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mumble</source>
        <translation type="obsolete">Mumble</translation>
    </message>
    <message>
        <location line="+960"/>
        <source>Failed to fetch server list</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui"/>
        <source>Mumble Server Connect</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Ping</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Remove from Favorites</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Add custom server</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Show all servers that respond to ping</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Show all servers with users</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Show all servers</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&amp;Copy</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Copy favorite link to clipboard</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&amp;Paste</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Paste favorite from clipboard</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Add</source>
        <translation type="obsolete">Добавить</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Edit...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <location filename="ConnectDialog.cpp" line="-707"/>
        <source>&amp;Add New...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Add to &amp;Favorites</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Open &amp;Webpage</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Show &amp;Reachable</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Show &amp;Populated</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Show &amp;All</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ConnectDialogEdit</name>
    <message>
        <location filename="ConnectDialogEdit.ui"/>
        <source>Edit Server</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&amp;Servername</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Name of the server</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Name&lt;/b&gt;&lt;br/&gt;
Name of the server. This is what the server will be named like in your serverlist and can be chosen freely.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>A&amp;ddress</source>
        <translation type="unfinished">А&amp;дрес</translation>
    </message>
    <message>
        <location/>
        <source>Internet address of the server.</source>
        <oldsource>Internet address of the server. </oldsource>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Address&lt;/b&gt;&lt;br/&gt;
Internet address of the server. This can be a normal hostname, an IPv4/IPv6 address or a Bonjour service identifier. Bonjour service identifiers have to be prefixed with a &apos;@&apos; to be recognized by Mumble.</source>
        <oldsource>&lt;b&gt;Address&lt;/b&gt;&lt;/br&gt;
Internet address of the server. This can be a normal hostname, an ipv4/6 address or a bonjour service identifier. Bonjour service identifiers have to be prefixed with a &apos;@&apos; to be recognized by Mumble.</oldsource>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&amp;Port</source>
        <translation type="unfinished">&amp;Порт</translation>
    </message>
    <message>
        <location/>
        <source>Port on which the server is listening</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Port&lt;/b&gt;&lt;br/&gt;
Port on which the server is listening. If the server is identified by a Bonjour service identifier this field will be ignored.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&amp;Username</source>
        <translation type="unfinished">Имя п&amp;ользователя</translation>
    </message>
    <message>
        <location/>
        <source>Username to send to the server</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Username&lt;/b&gt;&lt;br/&gt;
Username to send to the server. Be aware that the server can impose restrictions on how a username might look like. Also your username could already be taken by another user.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>CoreAudioSystem</name>
    <message>
        <location filename="CoreAudio.cpp" line="+85"/>
        <source>Default Device</source>
        <translation type="unfinished"></translation>
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
        <location line="+25"/>
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
        <source>Mumble</source>
        <translation type="obsolete">Mumble</translation>
    </message>
    <message>
        <source>Opening chosen DirectSound Input failed. No microphone capture will be done.</source>
        <translation type="obsolete">Открытие выбранного входа DirectSound невозможно. Ввод с микрофона неосуществим.</translation>
    </message>
    <message>
        <location filename="DirectSound.cpp" line="+556"/>
        <source>Opening chosen DirectSound Input failed. Default device will be used.</source>
        <translation>Открытие выбранного входа DirectSound невозможно. Будет использовано устройство по умолчанию.</translation>
    </message>
    <message>
        <location line="-382"/>
        <source>Default DirectSound Voice Input</source>
        <translation type="unfinished">Голосовой вход DirectSound по умолчанию</translation>
    </message>
    <message>
        <location line="+461"/>
        <source>Opening chosen DirectSound Input device failed. No microphone capture will be done.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-5"/>
        <source>Lost DirectSound input device.</source>
        <translation type="unfinished"></translation>
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
        <translation>Открытие выбранного выхода DirectSound невозможно. Вы не услышите звука.</translation>
    </message>
    <message>
        <location line="-82"/>
        <source>Opening chosen DirectSound Output failed. Default device will be used.</source>
        <translation>Открытие выбранного выхода DirectSound невозможно. Будет использовано устройство по умолчанию.</translation>
    </message>
    <message>
        <location line="-247"/>
        <source>Default DirectSound Voice Output</source>
        <translation type="unfinished">Голосовой выход DirectSound по умолчанию</translation>
    </message>
    <message>
        <location line="+325"/>
        <source>Lost DirectSound output device.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>DXConfigDialog</name>
    <message>
        <source>Default DirectSound Voice Input</source>
        <translation type="obsolete">Голосовой вход DirectSound по умолчанию</translation>
    </message>
    <message>
        <source>Default DirectSound Voice Output</source>
        <translation type="obsolete">Голосовой выход DirectSound по умолчанию</translation>
    </message>
    <message>
        <source>Device selection</source>
        <translation type="obsolete">Выбор устройства</translation>
    </message>
    <message>
        <source>Device to use for microphone</source>
        <translation type="obsolete">Устройство, используемое как микрофон</translation>
    </message>
    <message>
        <source>This sets the input device to use, which is where you have connected the microphone.</source>
        <translation type="obsolete">Устанавливает используемое устройство ввода, куда вы подсоединили микрофон.</translation>
    </message>
    <message>
        <source>Input</source>
        <translation type="obsolete">Ввод</translation>
    </message>
    <message>
        <source>Device to use for speakers/headphones</source>
        <translation type="obsolete">Устройство, используемое как динамики/наушники</translation>
    </message>
    <message>
        <source>This sets the output device to use, which is where you have connected your speakers or your headset.</source>
        <translation type="obsolete">Устанавливает используемое устройство вывода, куда вы подсоединили динамики или наушники.</translation>
    </message>
    <message>
        <source>Output</source>
        <translation type="obsolete">Вывод</translation>
    </message>
    <message>
        <source>Output Options</source>
        <translation type="obsolete">Свойства вывода</translation>
    </message>
    <message>
        <source>Output Delay</source>
        <translation type="obsolete">Задержка вывода</translation>
    </message>
    <message>
        <source>Amount of data to buffer for DirectSound</source>
        <translation type="obsolete">Количество данных буфера для DirectSound</translation>
    </message>
    <message>
        <source>This sets the amount of data to prebuffer in the DirectSound buffer. Experiment with different values and set it to the lowest which doesn&apos;t cause rapid jitter in the sound.</source>
        <translation type="obsolete">Устанавливает размер данных для буферизации в буфере DirectSound. Экспериментируйте с различными значениями и установите его на наименьшее, которое не вызывает быстрого ухудшения звука.</translation>
    </message>
    <message>
        <source>Positional Audio</source>
        <translation type="obsolete">Позиционированное аудио</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Нет</translation>
    </message>
    <message>
        <source>Panning</source>
        <translation type="obsolete">Размытие</translation>
    </message>
    <message>
        <source>Light HRTF</source>
        <translation type="obsolete">Легкий HRTF</translation>
    </message>
    <message>
        <source>Full HRTF</source>
        <translation type="obsolete">Полный HRTF</translation>
    </message>
    <message>
        <source>3D Sound Algorithm</source>
        <translation type="obsolete">Алгоритм 3D звучания</translation>
    </message>
    <message>
        <source>This sets what 3D Sound algorithm to use.&lt;br /&gt;&lt;b&gt;None&lt;/b&gt; - Disable 3D Sound (least CPU).&lt;br /&gt;&lt;b&gt;Panning&lt;/b&gt; - Just use stereo panning (some CPU).&lt;br /&gt;&lt;b&gt;Light/Full HRTF&lt;/b&gt; - Head-Related Transfer Functions enabled. This may use a small amount of CPU.&lt;br /&gt;Note that if you have a soundcard with &lt;i&gt;hardware&lt;/i&gt; 3D processing, HRTF processing will be done on the soundcard and will use practically no processing power.</source>
        <translation type="obsolete">Устанавливает, какой алгоритм 3D звучания использовать.&lt;br /&gt;&lt;b&gt;Нет&lt;/b&gt; - Выключить 3D Звучание (наименьшая нагрузка на ЦП).&lt;br /&gt;&lt;b&gt;Размытие&lt;/b&gt; - Просто ипсользоватьб стьерео размытие (небольшая загрузка ЦП).&lt;br /&gt;&lt;b&gt;Легкое/Полное HRTF&lt;/b&gt; - Включает Направленные функции передачи. Может немного загружать ЦП.&lt;br /&gt;Заметьте, что если ваша звуковая карта поддерживает &lt;i&gt;аппаратную&lt;/i&gt; обработку 3D, обработка HRTF будет осуществлятся практически без использования ЦП.</translation>
    </message>
    <message>
        <source>Method</source>
        <translation type="obsolete">Метод</translation>
    </message>
    <message>
        <source>MinDistance</source>
        <translation type="obsolete">МинДистанция</translation>
    </message>
    <message>
        <source>Minimum distance to player before sound decreases</source>
        <translation type="obsolete">Минимальное расстояние до игрока до уменьшения звука</translation>
    </message>
    <message>
        <source>This sets the minimum distance for sound calculations. The volume of other players&apos; speech will not decrease until they are at least this far away from you.</source>
        <translation type="obsolete">Устанавливает минимальное расстояние для расчета звука. Громкость речи других игроков не уменьшится до тех пор, пока они по крайней мере на таком расстоянии от вас.</translation>
    </message>
    <message>
        <source>MaxDistance</source>
        <translation type="obsolete">МаксДистанция</translation>
    </message>
    <message>
        <source>Maximum distance, beyond which sound won&apos;t decrease</source>
        <translation type="obsolete">Максимальное расстояние, больше которого звук не уменьшится</translation>
    </message>
    <message>
        <source>This sets the maximum distance for sound calculations. When farther away than this, other players&apos; sound volume will not decrease any more.</source>
        <translation type="obsolete">Устанавливает максимальное расстояния для рассчета звука. Даже дальше от вас, громкость других игроков больше не уменьшится.</translation>
    </message>
    <message>
        <source>RollOff</source>
        <translation type="obsolete">Откат</translation>
    </message>
    <message>
        <source>Factor for sound volume decrease</source>
        <translation type="obsolete">Фактор уменьшения громкости звука</translation>
    </message>
    <message>
        <source>How fast should sound volume drop when passing beyond the minimum distance. The normal (1.0) is that sound volume halves each time the distance doubles. Increasing this value means sound volume drops faster, while decreasing it means it drops slower.</source>
        <translation type="obsolete">Как быстро будет падать громкость звука после достижения определеной дистанции. Обычно (1.0) - громкость уменьшается вполовину при удвоении расстояния. Увеличение этого значения обозначает, что громкость падает быстрее, уменьшение - замедление падения.</translation>
    </message>
    <message>
        <source>DirectSound</source>
        <translation type="obsolete">DirectSound</translation>
    </message>
    <message>
        <source>%1ms</source>
        <translation type="obsolete">%1мс</translation>
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
        <translation type="obsolete">Игроки дальше, чем %1 метров дальше будут слышать сигнал %2% интенсивности</translation>
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
        <translation>Mumble не удалось инициализировать базу данных ни в
одном из доступных мест.</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>The database &apos;%1&apos; is read-only. Mumble can not store server settings (ie. SSL certificates) until you fix this problem.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>DirectInputKeyWidget</name>
    <message>
        <source>Press Shortcut</source>
        <translation type="obsolete">Нажмите ярлык</translation>
    </message>
</context>
<context>
    <name>GlobalShortcut</name>
    <message>
        <location filename="GlobalShortcut.ui"/>
        <source>Shortcuts</source>
        <translation type="unfinished">Ярлыки</translation>
    </message>
    <message>
        <location/>
        <source>List of configured shortcuts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Function</source>
        <translation type="unfinished">Функция</translation>
    </message>
    <message>
        <location/>
        <source>Data</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Shortcut</source>
        <translation type="unfinished">Ярлык</translation>
    </message>
    <message>
        <location/>
        <source>Suppress</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Add new shortcut</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This will add a new global shortcut</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&amp;Add</source>
        <translation type="unfinished">&amp;Добавить</translation>
    </message>
    <message>
        <location/>
        <source>Remove selected shortcut</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This will permanently remove a selected shortcut.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&amp;Remove</source>
        <translation type="unfinished">&amp;Удалить</translation>
    </message>
</context>
<context>
    <name>GlobalShortcutConfig</name>
    <message>
        <location filename="GlobalShortcut.cpp" line="+655"/>
        <source>Shortcuts</source>
        <translation type="unfinished">Ярлыки</translation>
    </message>
    <message>
        <source>Function</source>
        <translation type="obsolete">Функция</translation>
    </message>
    <message>
        <source>Shortcut</source>
        <translation type="obsolete">Ярлык</translation>
    </message>
    <message>
        <source>Shortcut bound to %1.</source>
        <translation type="obsolete">Ярлык указывает на %1.</translation>
    </message>
    <message>
        <source>&lt;b&gt;This is the global shortcut bound to %1&lt;/b&gt;&lt;br /&gt;Click this field and then the desired key/button combo to rebind. Double-click to clear.</source>
        <translation type="obsolete">&lt;b&gt;Этот глобальный ярлык указывает на %1&lt;/b&gt;&lt;br /&gt;Щелкните на этом поле и затем на выбренной клавише/сочетании клавиш для применения. Дважды кликните для очистки.</translation>
    </message>
    <message>
        <location line="+38"/>
        <source>Shortcut button combination.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>&lt;b&gt;This is the global shortcut key combination.&lt;/b&gt;&lt;br /&gt;Click this field and then press the desired key/button combo to rebind. Double-click to clear.</source>
        <oldsource>&lt;b&gt;This is the global shortcut key combination.&lt;/b&gt;&lt;br /&gt;Double-click this field and then the desired key/button combo to rebind.</oldsource>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Suppress keys from other applications</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>&lt;b&gt;This hides the button presses from other applications.&lt;/b&gt;&lt;br /&gt;Enabling this will hide the button (or the last button of a multi-button combo) from other applications. Note that not all buttons can be suppressed.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>GlobalShortcutMacInit</name>
    <message>
        <location filename="GlobalShortcut_macx.cpp" line="+60"/>
        <source>Mumble has detected that it is unable to receive Global Shortcut events when it is in the background.&lt;br /&gt;&lt;br /&gt;This is because the Universal Access feature called &apos;Enable access for assistive devices&apos; is currently disabled.&lt;br /&gt;&lt;br /&gt;Please &lt;a href=&quot; &quot;&gt;enable this setting&lt;/a&gt; and continue when done.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>GlobalShortcutTarget</name>
    <message>
        <location filename="GlobalShortcutTarget.ui"/>
        <source>Whisper Target</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Whisper to list of Users</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Channel Target</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Restrict to Group</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>If specified, only members of this group will receive the whisper.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>If checked the whisper will also be transmitted to linked channels.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Whisper to Linked channels</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>If checked this whisper will also be sent to the subchannels of the channel target.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Whisper to subchannels</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>List of users</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Add</source>
        <translation type="unfinished">Добавить</translation>
    </message>
    <message>
        <location/>
        <source>Remove</source>
        <translation type="unfinished">Убрать</translation>
    </message>
    <message>
        <location/>
        <source>Whisper to Channel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Modifiers</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Do not send positional audio information when using this whisper shortcut.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Ignore positional audio</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>GlobalShortcutWinConfig</name>
    <message>
        <source>Shortcuts</source>
        <translation type="obsolete">Ярлыки</translation>
    </message>
    <message>
        <source>Function</source>
        <translation type="obsolete">Функция</translation>
    </message>
    <message>
        <source>Shortcut</source>
        <translation type="obsolete">Ярлык</translation>
    </message>
    <message>
        <source>Shortcut bound to %1.</source>
        <translation type="obsolete">Ярлык указывает на %1.</translation>
    </message>
    <message>
        <source>&lt;b&gt;This is the global shortcut bound to %1&lt;/b&gt;&lt;br /&gt;Click this field and then the desired key/button combo to rebind. Double-click to clear.</source>
        <translation type="obsolete">&lt;b&gt;Этот глобальный ярлык указывает на %1&lt;/b&gt;&lt;br /&gt;Щелкните на этом поле и затем на выбренной клавише/сочетании клавиш для применения. Дважды кликните для очистки.</translation>
    </message>
</context>
<context>
    <name>GlobalShortcutX</name>
    <message>
        <location filename="GlobalShortcut_unix.cpp" line="+325"/>
        <source>Mouse %1</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>GlobalShortcutXConfig</name>
    <message>
        <source>Shortcuts</source>
        <translation type="obsolete">Ярлыки</translation>
    </message>
    <message>
        <source>Function</source>
        <translation type="obsolete">Функция</translation>
    </message>
    <message>
        <source>Shortcut</source>
        <translation type="obsolete">Ярлык</translation>
    </message>
    <message>
        <source>Shortcut bound to %1.</source>
        <translation type="obsolete">Ярлык указывает на %1.</translation>
    </message>
    <message>
        <source>&lt;b&gt;This is the global shortcut bound to %1&lt;/b&gt;&lt;br /&gt;Click this field and then the desired key combo to rebind. Double-click to clear.</source>
        <translation type="obsolete">&lt;b&gt;Этот глобальный ярлык указывает на %1&lt;/b&gt;&lt;br /&gt;Щелкните на этом поле и затем на выбренной клавише/сочетании клавиш для применения. Дважды кликните для очистки.</translation>
    </message>
</context>
<context>
    <name>LCD</name>
    <message>
        <location filename="LCD.cpp" line="+280"/>
        <source>Not connected</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>LCDConfig</name>
    <message>
        <location line="-160"/>
        <source>Enable this device</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+9"/>
        <source>LCD</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LCD.ui"/>
        <source>Form</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Devices</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Name</source>
        <translation type="unfinished">Имя</translation>
    </message>
    <message>
        <location/>
        <source>&lt;p&gt;This is the list of available LCD devices on your system.  It lists devices by name, but also includes the size of the display. Mumble supports outputting to several LCD devices at a time.&lt;/p&gt;
&lt;h3&gt;Size:&lt;/h3&gt;
&lt;p&gt;
This field describes the size of an LCD device. The size is given either in pixels (for Graphic LCDs) or in characters (for Character LCDs).&lt;/p&gt;
&lt;h3&gt;Enabled:&lt;/h3&gt;
&lt;p&gt;This decides whether Mumble should draw to a particular LCD device.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Enabled</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Views</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Minimum Column Width</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;p&gt;This option decides the minimum width a column in the User View.&lt;/p&gt;
&lt;p&gt;If too many people are speaking at once, the User View will split itself into columns. You can use this option to pick a compromise between number of users shown on the LCD, and width of user names.&lt;/p&gt;
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This setting decides the width of column splitter.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Splitter Width</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Log</name>
    <message>
        <location filename="Log.cpp" line="+220"/>
        <source>Debug</source>
        <translation>Отладка</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Critical</source>
        <translation>Критичный</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Warning</source>
        <translation>Предупреждение</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Information</source>
        <translation>Информация</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Server Connected</source>
        <translation>Сервер подключен</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Server Disconnected</source>
        <translation>Сервер отключен</translation>
    </message>
    <message>
        <source>Player Joined Server</source>
        <translation type="obsolete">Игнрок присоединился к серверу</translation>
    </message>
    <message>
        <source>Player Left Server</source>
        <translation type="obsolete">Игрок покинул сервер</translation>
    </message>
    <message>
        <source>Player kicked (you or by you)</source>
        <translation type="obsolete">Игрок выброшен (Вы или Вами)</translation>
    </message>
    <message>
        <source>Player kicked</source>
        <translation type="obsolete">Игрок выброшен</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>You self-muted/deafened</source>
        <translation>Вы заглушили сами себя</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Other self-muted/deafened</source>
        <translation>Другой игрок заглушил сам себя</translation>
    </message>
    <message>
        <source>Player muted (you)</source>
        <translation type="obsolete">Игрок заглушил вас</translation>
    </message>
    <message>
        <source>Player muted (by you)</source>
        <translation type="obsolete">Вы заглушили игрока</translation>
    </message>
    <message>
        <source>Player muted (other)</source>
        <translation type="obsolete">Игрок заглушен (другим)</translation>
    </message>
    <message>
        <source>Player Joined Channel</source>
        <translation type="obsolete">Игрок присоединился к каналу</translation>
    </message>
    <message>
        <source>Player Left Channel</source>
        <translation type="obsolete">Игрок покинул канал</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Permission Denied</source>
        <translation>Доступ запрещен</translation>
    </message>
    <message>
        <source>[%2] %1</source>
        <translation type="obsolete">[%2] %1</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Text Message</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-12"/>
        <source>User Joined Server</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>User Left Server</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>User kicked (you or by you)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>User kicked</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>User muted (you)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>User muted (by you)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>User muted (other)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>User Joined Channel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>User Left Channel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+54"/>
        <source>the server</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+117"/>
        <source>[[ Text object too large to display ]]</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+42"/>
        <source>[Date changed to %1]
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+133"/>
        <source>link to %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>ftp link to %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>player link</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>channel link</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1 link</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>LogConfig</name>
    <message>
        <location line="-511"/>
        <source>Toggle console for %1 events</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Toggle pop-up notifications for %1 events</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Toggle Text-To-Speech for %1 events</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Click here to toggle sound notification for %1 events</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Path to sound file used for sound notifications in the case of %1 events&lt;br /&gt;Single click to play&lt;br /&gt;Double-click to change</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Click here to toggle console output for %1 events.&lt;br /&gt;If checked, this option makes Mumble output all %1 events in its message log.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Click here to toggle pop-up notifications for %1 events.&lt;br /&gt;If checked, a notification pop-up will be created by Mumble for every %1 event.</source>
        <oldsource>Click here to toggle pop-up notifications for %1 events.&lt;br /&gt;If checked, a notification pop-up will be created by mumble for every %1 event.</oldsource>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Path to sound file used for sound notifications in the case of %1 events.&lt;br /&gt;Single click to play&lt;br /&gt;Double-click to change&lt;br /&gt;Ensure that sound notifications for these events are enabled or this field will not have any effect.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-2"/>
        <source>Click here to toggle Text-To-Speech for %1 events.&lt;br /&gt;If checked, Mumble uses Text-To-Speech to read %1 events out loud to you. Text-To-Speech is also able to read the contents of the event which is not true for sound files. Text-To-Speech and sound files cannot be used at the same time.</source>
        <oldsource>Click here to toggle sound notification for %1 events.&lt;br /&gt;If checked, Mumble uses a soundfile predefined by you to indicate %1 events. Soundfiles and Text-To-Speech cannot be used at the same time.</oldsource>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Click here to toggle sound notification for %1 events.&lt;br /&gt;If checked, Mumble uses a sound file predefined by you to indicate %1 events. Sound files and Text-To-Speech cannot be used at the same time.</source>
        <oldsource>Path to soundfile used for sound notifications in the case of %1 events.&lt;br /&gt;Single click to play&lt;br /&gt;Doubleclick to change&lt;br /&gt;Be sure that sound notifications for these events are enabled or this field will not have any effect.</oldsource>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Log.ui"/>
        <source>Messages</source>
        <translation>Сообщения</translation>
    </message>
    <message>
        <location/>
        <source>Console</source>
        <translation>Консоль</translation>
    </message>
    <message>
        <source>TTS</source>
        <translation type="obsolete">Текст в речь</translation>
    </message>
    <message>
        <source>Enable console for %1</source>
        <translation type="obsolete">Включить консоль для %1</translation>
    </message>
    <message>
        <source>Enable Text-To-Speech for %1</source>
        <translation type="obsolete">Включить преобразователь текста в речь для %1</translation>
    </message>
    <message>
        <location/>
        <source>Text To Speech</source>
        <translation>Текст в речь</translation>
    </message>
    <message>
        <location/>
        <source>Volume</source>
        <translation>Громкость</translation>
    </message>
    <message>
        <location/>
        <source>Volume of Text-To-Speech Engine</source>
        <translation>Громкость преобразователя текста в речь</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is the volume used for the speech synthesis.&lt;/b&gt;</source>
        <translation>&lt;b&gt;Эта громкость используется для создания речи.&lt;/b&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Length threshold</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Message length threshold for Text-To-Speech Engine</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is the length threshold used for the Text-To-Speech Engine.&lt;/b&gt;&lt;br /&gt;Messages longer than this limit will not be read aloud in their full length.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Whisper</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>If checked you will only hear whispers from users you added to your friend list.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Only accept whispers from friends</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Message</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Notification</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Text-To-Speech</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Soundfile</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Path</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source> Characters</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>LookConfig</name>
    <message>
        <location filename="LookConfig.ui"/>
        <source>Language</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Look and Feel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="+52"/>
        <location line="+11"/>
        <source>System default</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LookConfig.ui"/>
        <source>Language to use (requires restart)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets which language Mumble should use.&lt;/b&gt;&lt;br /&gt;You have to restart Mumble to use the new language.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Layout</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Classic</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Stacked</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Hybrid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Custom</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This changes the behavior when moving channels.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This sets the behavior of channel drags; it can be used to prevent accidental dragging. &lt;i&gt;Move&lt;/i&gt; moves the channel without prompting. &lt;i&gt;Do Nothing&lt;/i&gt; does nothing and prints an error message. &lt;i&gt;Ask&lt;/i&gt; uses a message box to confirm if you really wanted to move the channel.</source>
        <oldsource>This sets the behavior of channel drags; it can be used to prevent accidental dragging. &lt;i&gt;Move Channel&lt;/i&gt; moves the channel without prompting. &lt;i&gt;Do Nothing&lt;/i&gt; does nothing and prints an error message. &lt;i&gt;Ask&lt;/i&gt; uses a message box to confirm if you really wanted to move the channel.</oldsource>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Basic widget style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the basic look and feel to use.&lt;/b&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Skin</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Show number of users in each channel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Show channel user count</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Skin file to use</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="+14"/>
        <source>User Interface</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+127"/>
        <source>Choose skin file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LookConfig.ui"/>
        <source>&lt;b&gt;This sets which skin Mumble should use.&lt;/b&gt;&lt;br /&gt;The skin is a style file applied on top of the basic widget style. If there are icons in the same directory as the style sheet, those will replace the default icons.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Form</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="-137"/>
        <source>None</source>
        <translation type="unfinished">Нет</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>All</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LookConfig.ui"/>
        <source>Expand</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>When to automatically expand channels</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="-1"/>
        <source>Only with users</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Ask</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Do Nothing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Move</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LookConfig.ui"/>
        <source>Channel Dragging</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Ask whether to close or minimize when quitting Mumble.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Ask on quit while connected</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Always On Top</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;If set, minimizing the Mumble main window will cause it to be hidden and accessible only from the tray. Otherwise, it will be minimized as a window normally would.&lt;/b&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This sets which channels to automatically expand. &lt;i&gt;None&lt;/i&gt; and &lt;i&gt;All&lt;/i&gt; will expand no or all channels, while &lt;i&gt;Only with users&lt;/i&gt; will expand and collapse channels as users join and leave them.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>List users above subchannels (requires restart).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;If set, users will be shown above subchannels in the channel view.&lt;/b&gt;&lt;br /&gt;A restart of Mumble is required to see the change.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Users above Channels</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;If set, will verify you want to quit if connected.&lt;/b&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Hide in tray when minimized</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Hide the main Mumble window in the tray when it is minimized.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This setting controls when the application will be always on top.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Never</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Always</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>In minimal view</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>In normal view</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Displays talking status in system tray</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Show talking status in tray icon</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This setting controls in which situations the application will stay always on top. If you select &lt;i&gt;Never&lt;/i&gt; the application will not stay on top. &lt;i&gt;Always&lt;/i&gt; will always keep the application on top. &lt;i&gt;In minimal view&lt;/i&gt; / &lt;i&gt;In normal view&lt;/i&gt; will only keep the application always on top when minimal view is activated / deactivated.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Show context menu in menu bar</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Apply some high contrast optimizations for visually impaired users</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Optimize for high contrast</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Application</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Adds user and channel context menus into the menu bar</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Tray Icon</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Channel Tree</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <location filename="MainWindow.cpp" line="+148"/>
        <location line="+1840"/>
        <source>Root</source>
        <translation>Корень</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>&amp;Connect</source>
        <translation>&amp;Присоединиться</translation>
    </message>
    <message>
        <location/>
        <source>Open the server connection dialog</source>
        <translation>Открыть окно соединения с сервером</translation>
    </message>
    <message>
        <source>Shows a dialog of registered servers, and also allows quick connect.</source>
        <translation type="obsolete">Показывает окно зарегистрированных серверов, а также позволяет быстро связываться.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Disconnect</source>
        <translation>&amp;Отключиться</translation>
    </message>
    <message>
        <location/>
        <source>Disconnect from server</source>
        <translation>Отключить от сервера</translation>
    </message>
    <message>
        <location/>
        <source>Disconnects you from the server.</source>
        <translation>Отключает вас от сервера.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Ban lists</source>
        <translation>&amp;Списки забаненных</translation>
    </message>
    <message>
        <location/>
        <source>Edit ban lists on server</source>
        <translation>Редактировать список забаненных на сервере</translation>
    </message>
    <message>
        <location/>
        <source>This lets you edit the server-side IP ban lists.</source>
        <translation>Позволяет вам изменять серверные банлисты по IP.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Kick</source>
        <translation>В&amp;ыбросить</translation>
    </message>
    <message>
        <source>Kick player (with reason)</source>
        <translation type="obsolete">Выкинуть игрока (с причиной)</translation>
    </message>
    <message>
        <source>Kick selected player off server. You&apos;ll be asked to specify a reason.</source>
        <translation type="obsolete">Выкидывает игрока с сервера. Вас спросят о причине.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Ban</source>
        <translation>За&amp;банить</translation>
    </message>
    <message>
        <source>Kick and ban player (with reason)</source>
        <translation type="obsolete">Выкинуть и забанить игрока (с причиной)</translation>
    </message>
    <message>
        <source>Kick and ban selected player from server. You&apos;ll be asked to specify a reason.</source>
        <translation type="obsolete">Выкидывает и банит выбранного игрока с сервера. Вас попросят уточнить причину.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Mute</source>
        <translation>Сделать &amp;немым</translation>
    </message>
    <message>
        <source>Mute player</source>
        <translation type="obsolete">Сделать игрока немым</translation>
    </message>
    <message>
        <source>Mute or unmute player on server. Unmuting a deafened player will also undeafen them.</source>
        <translation type="obsolete">Глушить или разглушить игрока на сервере. Разглушение игрока даст ему возможность говорить.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Deafen</source>
        <translation>Заг&amp;лушить</translation>
    </message>
    <message>
        <source>Deafen player</source>
        <translation type="obsolete">Заглушить игрока</translation>
    </message>
    <message>
        <source>Deafen or undeafen player on server. Deafening a player will also mute them.</source>
        <translation type="obsolete">Глушит или снимает глушение с игрока на сервере. Глушение игрока также сделает его немым.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Local Mute</source>
        <translation>&amp;Местное глушение</translation>
    </message>
    <message>
        <source>Mute or unmute player locally.</source>
        <translation type="obsolete">Делает игрока немым локально.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Add</source>
        <translation>&amp;Добавить</translation>
    </message>
    <message>
        <location/>
        <source>Add new channel</source>
        <translation>Добавить новый канал</translation>
    </message>
    <message>
        <location/>
        <source>This adds a new sub-channel to the currently selected channel.</source>
        <translation>Добавляет новый подканал в выбранный канал.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Remove</source>
        <translation>&amp;Удалить</translation>
    </message>
    <message>
        <location/>
        <source>Remove channel</source>
        <translation>Удалить канал</translation>
    </message>
    <message>
        <location/>
        <source>This removes a channel and all sub-channels.</source>
        <translation>Удаляет канал и все подканалы.</translation>
    </message>
    <message>
        <source>&amp;Edit ACL</source>
        <translation type="obsolete">&amp;Редактировать списки ACL</translation>
    </message>
    <message>
        <location/>
        <source>Edit Groups and ACL for channel</source>
        <translation>Редактировать Группы и списки ACL для канала</translation>
    </message>
    <message>
        <location/>
        <source>This opens the Group and ACL dialog for the channel, to control permissions.</source>
        <translation>Открывает диалоговое окно групп и списков ACL для каналов, чтобы управлять привилегиями.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Link</source>
        <translation>&amp;Связать</translation>
    </message>
    <message>
        <location/>
        <source>Link your channel to another channel</source>
        <translation>Связывает ваш канал с другими каналами</translation>
    </message>
    <message>
        <source>This links your current channel to the selected channel. If they have permission to speak in the other channel, players can now hear each other. This is a permanent link, and will last until manually unlinked or the server is restarted. Please see the shortcuts for push-to-link.</source>
        <translation type="obsolete">Связывает ваш текущий канал с выбранным каналом. Если у игроков есть разрешение говорить на канале, они могут слышать друг друга. Эта связь постоянна, и длится до разрыва связи вручную или перезапуска сервера. Пожалуйста, просмотрите ярлыки для нажмите-для-связи.</translation>
    </message>
    <message>
        <source>&amp;Unlink</source>
        <translation type="obsolete">&amp;Разорвать связь</translation>
    </message>
    <message>
        <location/>
        <source>Unlink your channel from another channel</source>
        <translation>Разорвать связь  канала с другим</translation>
    </message>
    <message>
        <location/>
        <source>This unlinks your current channel from the selected channel.</source>
        <translation>Разрывает связь вашего канала с выбранным каналом.</translation>
    </message>
    <message>
        <source>Unlink &amp;All</source>
        <translation type="obsolete">Разорвать &amp;Все</translation>
    </message>
    <message>
        <location/>
        <source>Unlinks your channel from all linked channels.</source>
        <translation>Разрывает канал с остальными каналами.</translation>
    </message>
    <message>
        <location/>
        <source>This unlinks your current channel (not the selected one) from all linked channels.</source>
        <translation>Разрывает связь вашего канала со всеми (не только с выбранными) каналами.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Reset</source>
        <translation>Сб&amp;росить</translation>
    </message>
    <message>
        <location/>
        <source>Reset audio preprocessor</source>
        <translation>Сбросить аудио препроцессор</translation>
    </message>
    <message>
        <location/>
        <source>This will reset the audio preprocessor, including noise cancellation, automatic gain and voice activity detection. If something suddenly worsens the audio environment (like dropping the microphone) and it was temporary, use this to avoid having to wait for the preprocessor to readjust.</source>
        <translation>Сбросит аудио препроцессор, включая подавление шумов, автоматическое получение и определение активности голоса. Если что-то внезапно вмешается в звуковую среду (например падение микрофона) на короткое время, используйте это, чтобы не ждать приспособления препроцессора.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Mute Self</source>
        <translation>Загл&amp;ушить себя</translation>
    </message>
    <message>
        <location/>
        <source>Mute yourself</source>
        <translation>Заглушить себя</translation>
    </message>
    <message>
        <location/>
        <source>Mute or unmute yourself. When muted, you will not send any data to the server. Unmuting while deafened will also undeafen.</source>
        <translation>Заглушает себя. При включении, вы не сможете посылать данных на сервер. </translation>
    </message>
    <message>
        <location/>
        <source>&amp;Deafen Self</source>
        <translation>Сделать се&amp;бя немым</translation>
    </message>
    <message>
        <location/>
        <source>Deafen yourself</source>
        <translation>Делает себя немым</translation>
    </message>
    <message>
        <location/>
        <source>Deafen or undeafen yourself. When deafened, you will not hear anything. Deafening yourself will also mute.</source>
        <translation>Делает себя немым или возвращает голос. Когда вы нем, вы не услышите никого. Онемение также и заглушает.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Text-To-Speech</source>
        <translation>&amp;Text-To-Speech</translation>
    </message>
    <message>
        <location/>
        <source>Toggle Text-To-Speech</source>
        <translation>Включить Text-To-Speech</translation>
    </message>
    <message>
        <location/>
        <source>Enable or disable the text-to-speech engine. Only messages enabled for TTS in the Configuration dialog will actually be spoken.</source>
        <translation>Включает или выключает движок Text-To-Speech. Будут произносится сообщения, включенные в диалоговом окне конфигурации TTS.</translation>
    </message>
    <message>
        <source>S&amp;tatistics</source>
        <translation type="obsolete">С&amp;татистика</translation>
    </message>
    <message>
        <location/>
        <source>Display audio statistics</source>
        <translation>Показывает аудио статистику</translation>
    </message>
    <message>
        <location/>
        <source>Pops up a small dialog with information about your current audio input.</source>
        <translation>Вызывает маленькую подсказку о вашем текущем аудио вводе.</translation>
    </message>
    <message>
        <location/>
        <source>Forcibly unlink plugin</source>
        <translation>Насильно отключить плагин</translation>
    </message>
    <message>
        <location/>
        <source>This forces the current plugin to unlink, which is handy if it is reading completely wrong data.</source>
        <translation>Заставляет текущий плагин разорвать связь, что удобно про считываниии абсолютно неверных данных.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Settings</source>
        <translation>&amp;Настройки</translation>
    </message>
    <message>
        <location/>
        <source>Configure Mumble</source>
        <translation>Настроить Mumble</translation>
    </message>
    <message>
        <location/>
        <source>Allows you to change most settings for Mumble.</source>
        <translation>Позволяет вам изменять большинство настроек Mumble.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;What&apos;s This?</source>
        <translation>&amp;Что это?</translation>
    </message>
    <message>
        <location/>
        <source>Enter What&apos;s This? mode</source>
        <translation>Войти в Что это? режим </translation>
    </message>
    <message>
        <location/>
        <source>Click this to enter &quot;What&apos;s This?&quot; mode. Your cursor will turn into a question mark. Click on any button, menu choice or area to show a description of what it is.</source>
        <translation>Нажмите, чтобы войти в режим &quot;Что это?&quot;. Ваш курсор превратится в знак вопроса. Нажмите на любой кнопке, пункту меню или зоне, чтобы получить пояснение.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;About</source>
        <translation>&amp;О программе</translation>
    </message>
    <message>
        <location/>
        <source>Information about Mumble</source>
        <translation>Информация о Mumble</translation>
    </message>
    <message>
        <location/>
        <source>Shows a small dialog with information and license for Mumble.</source>
        <translation>Показывает маленькое диалоговое окно с информацией и лицензией для Mumble.</translation>
    </message>
    <message>
        <location/>
        <source>About &amp;Speex</source>
        <translation>О &amp;Speex</translation>
    </message>
    <message>
        <location/>
        <source>Information about Speex</source>
        <translation>Информация о Speex</translation>
    </message>
    <message>
        <location/>
        <source>Shows a small dialog with information about Speex.</source>
        <translation>Показывает небольшое диалоговое окно с информацией о Speex.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Change Comment</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Configure certificates for strong authentication</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This starts the wizard for creating, importing and exporting certificates for authentication against servers.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Register user on server</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This will permanently register the user on the server.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Add &amp;Friend</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Adds a user as your friend.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This will add the user as a friend, so you can recognize him on this and other servers.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&amp;Remove Friend</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Removes a user from your friends.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This will remove a user from your friends list.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&amp;Update Friend</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Update name of your friend.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Your friend uses a different name than what is in your database. This will update the name.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Registered &amp;Users</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Edit registered users list</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This opens the editor for registered users, which allow you to change their name or unregister them.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&amp;Access Tokens</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Add or remove text-based access tokens</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;About Qt</source>
        <translation type="obsolete">&amp;О Qt</translation>
    </message>
    <message>
        <location/>
        <source>Information about Qt</source>
        <translation>Информация о Qt</translation>
    </message>
    <message>
        <location/>
        <source>Shows a small dialog with information about Qt.</source>
        <translation>Показывает небольшое диалоговое окно с информацией о Qt.</translation>
    </message>
    <message>
        <location/>
        <source>Check for &amp;Updates</source>
        <translation>Проверить об&amp;новления</translation>
    </message>
    <message>
        <location/>
        <source>Check for new version of Mumble</source>
        <translation>Проверить новую версию Mumble</translation>
    </message>
    <message>
        <location/>
        <source>Connects to the Mumble webpage to check if a new version is available, and notifies you with an appropriate download URL if this is the case.</source>
        <translation>Присоединяется к вебстранице Mumble, чтобы проверить доступность новой версии, и уведомляет вас о ссылке для закачки в этом случае.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="-1745"/>
        <location line="+12"/>
        <location line="+1941"/>
        <source>Mumble -- %1</source>
        <translation>Mumble -- %1</translation>
    </message>
    <message>
        <source>Log of messages</source>
        <translation type="obsolete">История сообщений</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>This shows all recent activity. Connecting to servers, errors and information messages all show up here.&lt;br /&gt;To configure exactly which messages show up here, use the &lt;b&gt;Settings&lt;/b&gt; command from the menu.</source>
        <translation>Показывает все недавние действия. Соединяясь с сервером, сообщения о всех ошибках и информации будут показаны все.&lt;br /&gt;Чтобы настроить, какие точно сообщения отображать там, используйте команду &lt;b&gt;Настройки&lt;/b&gt; из меню.</translation>
    </message>
    <message>
        <source>&amp;Server</source>
        <translation type="obsolete">&amp;Сервер</translation>
    </message>
    <message>
        <source>&amp;Player</source>
        <translation type="obsolete">&amp;Игрок</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="-2031"/>
        <source>&amp;Channel</source>
        <translation type="unfinished">&amp;Канал</translation>
    </message>
    <message>
        <source>&amp;Audio</source>
        <translation type="obsolete">&amp;Аудио</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>C&amp;onfigure</source>
        <translation>Н&amp;астройки</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Help</source>
        <translation>&amp;Помощь</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="+34"/>
        <source>Push-to-Talk</source>
        <comment>Global Shortcut</comment>
        <translation>Нажмите чтобы говорить</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Push and hold this button to send voice.</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This configures the push-to-talk button, and as long as you hold this button down, you will transmit voice.</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Reset Audio Processor</source>
        <comment>Global Shortcut</comment>
        <translation>Сбросить Аудио препроцессор</translation>
    </message>
    <message>
        <source>Toggle Mute Self</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">Включить заглушение себя</translation>
    </message>
    <message>
        <source>Toggle Deafen Self</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">Включить онемение себя</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Unlink Plugin</source>
        <comment>Global Shortcut</comment>
        <translation>Отключить плагин</translation>
    </message>
    <message>
        <source>Force Center Position</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">Принудительная центральная позиция</translation>
    </message>
    <message>
        <source>Chan Parent</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">
Предок канала</translation>
    </message>
    <message>
        <source>Chan Sub#%1</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">Подканал#%1</translation>
    </message>
    <message>
        <source>Chan All Subs</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">Все подканалы</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Push-to-Mute</source>
        <comment>Global Shortcut</comment>
        <translation>Нажмите чтобы заглушить</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Join Channel</source>
        <comment>Global Shortcut</comment>
        <translation>Присоединиться к каналу</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Toggle Overlay</source>
        <comment>Global Shortcut</comment>
        <translation>Включить Замещение</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Toggle state of in-game overlay.</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt Push-to-Talk</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">Усиленное нажмите чтобы говорить</translation>
    </message>
    <message>
        <source>Kicking player %1</source>
        <translation type="obsolete">Выкидывание игрока %1</translation>
    </message>
    <message>
        <location line="+963"/>
        <location line="+19"/>
        <source>Enter reason</source>
        <translation>Введите причину</translation>
    </message>
    <message>
        <source>Banning player %1</source>
        <translation type="obsolete">Бан игрока %1</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>Mumble</source>
        <translation>Mumble</translation>
    </message>
    <message>
        <source>Channel Name</source>
        <translation type="obsolete">Имя канала</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="+252"/>
        <source>Are you sure you want to delete %1 and all its sub-channels?</source>
        <translation>Вы уверены, что хотите удалить %1 и все его подканалы?</translation>
    </message>
    <message>
        <location line="+191"/>
        <source>Unmuted and undeafened.</source>
        <translation>Онемение и глушение снято.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Unmuted.</source>
        <translation>Глушение снято.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Muted.</source>
        <translation>Заглушен.</translation>
    </message>
    <message>
        <location line="+24"/>
        <source>Muted and deafened.</source>
        <translation>В онемении и заглушен.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Deafened.</source>
        <translation>В онемении.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Undeafened.</source>
        <translation>Онемение снято.</translation>
    </message>
    <message>
        <location line="+67"/>
        <source>About Qt</source>
        <translation>О Qt</translation>
    </message>
    <message>
        <source>Joining %1.</source>
        <translation type="obsolete">Соединяемся с %1.</translation>
    </message>
    <message>
        <source>Connected to server.</source>
        <translation type="obsolete">Соединен с сервером.</translation>
    </message>
    <message>
        <location line="+408"/>
        <source>Server connection failed: %1.</source>
        <translation>Попытка соединения с сервером не удалась: %1.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Disconnected from server.</source>
        <translation>Отключен от сервера.</translation>
    </message>
    <message>
        <location line="-1351"/>
        <source>Reconnecting.</source>
        <translation>Повтор связи.</translation>
    </message>
    <message>
        <source>Joined server: %1.</source>
        <translation type="obsolete">Присединились к серверу: %1.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="+287"/>
        <source>You were unmuted and undeafened by %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+29"/>
        <source>You muted and deafened %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>You unmuted and undeafened %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+11"/>
        <source>You undeafened %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+7"/>
        <source>You suppressed %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+7"/>
        <source>%1 muted and deafened by %2.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1 unmuted and undeafened by %2.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+18"/>
        <source>%1 suppressed by %2.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+25"/>
        <source>%1 moved to %2.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1 moved to %2 by %3.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+10"/>
        <source>%1 moved in from %2 by %3.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Left server: %1.</source>
        <translation type="obsolete">Покинут сервер: %1.</translation>
    </message>
    <message>
        <location line="-134"/>
        <source>%1 is now muted and deafened.</source>
        <translation>%1 сейчас в онемении и заглушен.</translation>
    </message>
    <message>
        <location line="-109"/>
        <source>You were denied %1 privileges in %2.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-61"/>
        <location line="+40"/>
        <source>Welcome message: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-7"/>
        <source>Mumble: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+30"/>
        <source>%3 was denied %1 privileges in %2.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Denied: Cannot modify SuperUser.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Denied: Invalid channel name.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Denied: Text message too long.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+20"/>
        <source>Denied: Operation not permitted in temporary channel.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+7"/>
        <source>You need a certificate to perform this operation.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1 does not have a certificate.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Invalid username: %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Invalid username.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Channel is full.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Permission denied.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+37"/>
        <source>%1 connected.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+13"/>
        <source>%1 is now muted.</source>
        <translation>%1 сейчас заглушен.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1 is now unmuted.</source>
        <translation>С %1 снято глушение.</translation>
    </message>
    <message>
        <location line="+37"/>
        <source>You were unsuppressed.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+139"/>
        <source>%1 disconnected.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+87"/>
        <location line="+1"/>
        <source>Server</source>
        <comment>message from</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>(Tree) </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>(Channel) </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>%2%1: %3</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>server</source>
        <translation type="obsolete">сервер</translation>
    </message>
    <message>
        <location line="-251"/>
        <source>You were muted by %1.</source>
        <translation>Вы заглушили %1.</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>You were suppressed.</source>
        <oldsource>You were suppressed by %1.</oldsource>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>You were unsuppressed by %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-16"/>
        <source>You were unmuted by %1.</source>
        <translation>Вы сняли глушение с %1.</translation>
    </message>
    <message>
        <location line="+29"/>
        <source>You muted %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+16"/>
        <source>You unsuppressed %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-14"/>
        <source>You unmuted %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+25"/>
        <source>%1 muted by %2.</source>
        <translation>%1 заглушен %2.</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>%1 unsuppressed by %2.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-14"/>
        <source>%1 unmuted by %2.</source>
        <translation>%2 снял глушение с %1.</translation>
    </message>
    <message>
        <location line="+327"/>
        <source>Unable to find matching CELT codecs with other clients. You will not be able to talk to all users.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-393"/>
        <source>You were muted and deafened by %1.</source>
        <oldsource>You were deafened by %1.</oldsource>
        <translation type="unfinished">На вас наложил онемение %1.</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>You were undeafened by %1.</source>
        <translation type="unfinished">С вас снял онемение %1.</translation>
    </message>
    <message>
        <source>%1 deafened by %2.</source>
        <translation type="obsolete">%1 в онемении из-за %2.</translation>
    </message>
    <message>
        <location line="+58"/>
        <source>%1 undeafened by %2.</source>
        <translation type="unfinished">С %1 снято онемение %2.</translation>
    </message>
    <message>
        <location line="+83"/>
        <source>You were kicked from the server by %1: %2.</source>
        <translation>Вы были выкинуты с сервера благодаря %1: %2.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>%3 was kicked from the server by %1: %2.</source>
        <translation>%3 был выкинут с сервера %1: %2.</translation>
    </message>
    <message>
        <location line="-7"/>
        <source>You were kicked and banned from the server by %1: %2.</source>
        <translation>Вы были выкинуты и забанены на сервере администратором %1: %2.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>%3 was kicked and banned from the server by %1: %2.</source>
        <translation>%3 был выкинут с сервера и забанен администратором %1: %2.</translation>
    </message>
    <message>
        <location line="-57"/>
        <source>You were moved to %1 by %2.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>%1 left channel.</source>
        <translation type="obsolete">%1 покинул канал.</translation>
    </message>
    <message>
        <source>%1 moved out by %2.</source>
        <translation type="obsolete">%1 перемещен из канала %2.</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>%1 entered channel.</source>
        <translation>%1 вошел на канал.</translation>
    </message>
    <message>
        <source>%1 moved in by %2.</source>
        <translation type="obsolete">%1 перемещен сюда %2.</translation>
    </message>
    <message>
        <location line="-310"/>
        <source>Server connection rejected: %1.</source>
        <translation>Попытка подключения отклонена: %1.</translation>
    </message>
    <message>
        <location line="+128"/>
        <source>Denied: %1.</source>
        <translation>Запрещено: %1.</translation>
    </message>
    <message>
        <location filename="main.cpp" line="-46"/>
        <source>Welcome to Mumble.</source>
        <translation>Добро пожаловать в Mumble.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>&amp;Quit Mumble</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Closes the program</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Exits the application.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Send a Text Message</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="+417"/>
        <source>Sending message to %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+10"/>
        <location line="+79"/>
        <location line="+4"/>
        <location line="+223"/>
        <source>To %1: %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-306"/>
        <location line="+83"/>
        <source>Message to %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-44"/>
        <source>Are you sure you want to reset the comment of user %1?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+265"/>
        <source>To %1 (Tree): %2</source>
        <oldsource>(Tree) %1: %2</oldsource>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+611"/>
        <source>Failed to launch compatibility client</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+21"/>
        <source>Invalid username</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+0"/>
        <source>You connected with an invalid username, please try another one.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+0"/>
        <source>That username is already in use, please try another username.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Wrong password</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Wrong password for registered users, please try again.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Wrong server password for unregistered user account, please try again.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="+327"/>
        <source>Message from %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>&amp;Audio Wizard</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Sends a text message to another user.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This is the chatbar&lt;br /&gt;If you enter text here and then press enter the text is sent to the user or channel that was selected. If nothing is selected the message is sent to your current channel.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Chatbar</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Shows a dialog of registered servers, and also allows quick-connect.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Kick user (with reason)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Kick selected user off server. You&apos;ll be asked to specify a reason.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Mute user</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Mute or unmute user on server. Unmuting a deafened user will also undeafen them.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Kick and ban user (with reason)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Kick and ban selected user from server. You&apos;ll be asked to specify a reason.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Deafen user</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Deafen or undeafen user on server. Deafening a user will also mute them.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Mute user locally</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Mute or unmute user locally. Use this on other users in the same room.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&amp;Edit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This links your current channel to the selected channel. If users in a channel have permission to speak in the other channel, users can now hear each other. This is a permanent link, and will last until manually unlinked or the server is restarted. Please see the shortcuts for push-to-link.</source>
        <oldsource>This links your current channel to the selected channel. If they have permission to speak in the other channel, users can now hear each other. This is a permanent link, and will last until manually unlinked or the server is restarted. Please see the shortcuts for push-to-link.</oldsource>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Start the audio configuration wizard</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This will guide you through the process of configuring your audio hardware.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="-90"/>
        <source>SSL Verification failed: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>&lt;b&gt;WARNING:&lt;/b&gt; The server presented a certificate that was different from the stored one.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Sever presented a certificate which failed verification.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>&amp;Information</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Show information about the server connection</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This will show extended information about the connection to the server.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="-1578"/>
        <source>Opening URL %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>File does not exist</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+10"/>
        <source>File is not a configuration file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Settings merged from file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>URL scheme is not &apos;mumble&apos;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+25"/>
        <source>This version of Mumble can&apos;t handle URLs for Mumble version %1.%2.%3</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+29"/>
        <source>Connecting to %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Enter username</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+18"/>
        <location line="+185"/>
        <source>Connecting to server %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+41"/>
        <source>Change your comment</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+92"/>
        <source>&lt;h2&gt;Version&lt;/h2&gt;&lt;p&gt;Protocol %1.%2.%3.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>&lt;p&gt;No build information or OS version available.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>&lt;p&gt;%1 (%2)&lt;br /&gt;%3&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+20"/>
        <source>&lt;h2&gt;Voice channel&lt;/h2&gt;&lt;p&gt;Encrypted with 128 bit OCB-AES128&lt;br /&gt;%1 ms average latency (%4 deviation)&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+11"/>
        <source>&lt;h2&gt;Audio bandwidth&lt;/h2&gt;&lt;p&gt;Maximum %1 kbit/s&lt;br /&gt;Current %2 kbit/s&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Mumble Server Information</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <location line="+1132"/>
        <source>&amp;View Certificate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+252"/>
        <source>Images (*.png *.jpg *.svg)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-1494"/>
        <location line="+281"/>
        <source>Register yourself as %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-920"/>
        <source>This will switch the states of the in-game overlay.</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+639"/>
        <location line="+281"/>
        <source>&lt;p&gt;You are about to register yourself on this server. This action cannot be undone, and your username cannot be changed once this is done. You will forever be known as &apos;%1&apos; on this server.&lt;/p&gt;&lt;p&gt;Are you sure you want to register yourself?&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Register user %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+0"/>
        <source>&lt;p&gt;You are about to register %1 on the server. This action cannot be undone, the username cannot be changed, and as a registered user, %1 will have access to the server even if you change the server password.&lt;/p&gt;&lt;p&gt;From this point on, %1 will be authenticated with the certificate currently in use.&lt;/p&gt;&lt;p&gt;Are you sure you want to register %1?&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+40"/>
        <source>Kicking user %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+19"/>
        <source>Banning user %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+51"/>
        <source>View comment on user %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+56"/>
        <location line="+227"/>
        <source>Message to channel %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+436"/>
        <source>Connected.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+147"/>
        <source>SSL Version mismatch</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+0"/>
        <source>This server is using an older encryption standard. It might be an older 1.1 based Mumble server.&lt;br /&gt;Would you like to launch the compatibility client to connect to it?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+26"/>
        <source>The compatibility client could not be found, or failed to start.&lt;br /&gt;Note that the compatibility client is an optional component for most installations, and might not be installed.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-50"/>
        <source>&lt;p&gt;%1.&lt;br /&gt;The specific errors with this certificate are: &lt;/p&gt;&lt;ol&gt;%2&lt;/ol&gt;&lt;p&gt;Do you wish to accept this certificate anyway?&lt;br /&gt;(It will also be stored so you won&apos;t be asked this again.)&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+165"/>
        <source>Type message to channel &apos;%1&apos; here</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Type message to user &apos;%1&apos; here</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+91"/>
        <source>Choose image file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+7"/>
        <location line="+17"/>
        <source>Failed to load image</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-17"/>
        <source>Could not open file for reading.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+17"/>
        <source>Image format not recognized.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-1427"/>
        <source>Voice channel is sent over control channel.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-796"/>
        <source>&amp;User</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+787"/>
        <source>&lt;h2&gt;Control channel&lt;/h2&gt;&lt;p&gt;Encrypted with %1 bit %2&lt;br /&gt;%3 ms average latency (%4 deviation)&lt;/p&gt;&lt;p&gt;Remote host %5 (port %6)&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+18"/>
        <source>UDP Statistics</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+0"/>
        <source>To Server</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+0"/>
        <source>From Server</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Good</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Late</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Lost</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Resync</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+565"/>
        <source>Sending message to channel %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Message to tree %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>Ctrl+Q</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>About &amp;Qt</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Sends a text message to all users in a channel.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="main.cpp" line="+13"/>
        <source>This is the first time you&apos;re starting Mumble.&lt;br /&gt;Would you like to go through the Audio Wizard to configure your soundcard?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="-1183"/>
        <source>Mumble is currently connected to a server. Do you want to Close or Minimize it?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Close</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-100"/>
        <location line="+101"/>
        <source>Minimize</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-154"/>
        <source>Mute Self</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Set self-mute status.</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This will set or toggle your muted status. If you turn this off, you will also disable self-deafen.</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Deafen Self</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Set self-deafen status.</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This will set or toggle your deafened status. If you turn this on, you will also enable self-mute.</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+30"/>
        <source>Whisper</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+13"/>
        <source>&amp;Window</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Ctrl+M</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-28"/>
        <source>Toggle Minimal</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>&amp;Minimal View</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Toggle minimal window modes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This will toggle minimal mode, where the log window and menu is hidden.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="+3"/>
        <source>Volume Up (+10%)</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Volume Down (-10%)</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+65"/>
        <location line="+1959"/>
        <source>Not connected</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-1750"/>
        <source>Clear</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>Log</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="+1382"/>
        <source>You have Channel Dragging set to &quot;Do Nothing&quot; so the channel wasn&apos;t moved.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Unknown Channel Drag mode in UserModel::dropMimeData.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>&amp;Unlink</source>
        <comment>Channel</comment>
        <translation type="unfinished">&amp;Разорвать связь</translation>
    </message>
    <message>
        <source>&amp;Unlink</source>
        <comment>Plugin</comment>
        <translation type="obsolete">&amp;Разорвать связь</translation>
    </message>
    <message>
        <location/>
        <source>Hide Frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Toggle showing frame on minimal window</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This will toggle whether the minimal window should have a frame for moving and resizing.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Send &amp;Message</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&amp;Unlink All</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&amp;Certificate Wizard</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&amp;Register</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Reset &amp;Comment</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Reset the comment of the selected user.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&amp;Join Channel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>View Comment</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>View comment in editor</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Query server for connection information for user</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>S&amp;erver</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&amp;Self</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Audio S&amp;tatistics</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&amp;Unlink Plugins</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>R&amp;egister</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Register yourself on the server</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Change &amp;Avatar</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Change your avatar image on this server</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&amp;Remove Avatar</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Remove currently defined avatar image.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Icon Toolbar</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Change your own comment</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>NetworkConfig</name>
    <message>
        <location filename="NetworkConfig.cpp" line="+53"/>
        <source>Network</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+36"/>
        <source>Updates are mandatory when using snapshot releases.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui"/>
        <source>Form</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Connection</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Use TCP compatibility mode</source>
        <oldsource>Use TCP compatability mode</oldsource>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Enable TCP compatibility mode&lt;/b&gt;.&lt;br /&gt;This will make Mumble use only TCP when communicating with the server. This will increase overhead and cause lost packets to produce noticeable pauses in communication, so this should only be used if you are unable to use the default (which uses UDP for voice and TCP for control).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Use TCP mode</source>
        <translation type="obsolete">Использовать режим TCP</translation>
    </message>
    <message>
        <location/>
        <source>Reconnect when disconnected</source>
        <translation type="unfinished">Восстановление связи при обрыве</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Reconnect when disconnected&lt;/b&gt;.&lt;br /&gt;This will make Mumble try to automatically reconnect after 10 seconds if your server connection fails.</source>
        <translation type="unfinished">&lt;b&gt;Восстановить связь при обрыве&lt;/b&gt;.&lt;br /&gt;Mumble будет пытаться автоматически восстановить связь через 10 в случае обрыва связи с сервером.</translation>
    </message>
    <message>
        <location/>
        <source>Reconnect automatically</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Proxy</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Type</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Type of proxy to connect through</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Direct connection</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>HTTP(S) proxy</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>SOCKS5 proxy</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Hostname</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Hostname of the proxy</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Force TCP mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Enable QoS to prioritize packets</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This will enable QoS, which will attempt to prioritize voice packets over other traffic.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Use Quality of Service</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Don&apos;t send certificate to server and don&apos;t save passwords. (Not saved).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This will suppress identity information from the client.&lt;/b&gt;&lt;p&gt;The client will not identify itself with a certificate, even if defined, and will not cache passwords for connections. This is primarily a test-option and is not saved.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Suppress certificate and password storage</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Type of proxy to connect through.&lt;/b&gt;&lt;br /&gt;This makes Mumble connect through a proxy for all outgoing connections. Note: Proxy tunneling forces Mumble into TCP compatibility mode, causing all voice data to be sent via the control channel.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Hostname of the proxy.&lt;/b&gt;&lt;br /&gt;This field specifies the hostname of the proxy you wish to tunnel network traffic through.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Port</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Port number of the proxy</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Port number of the proxy.&lt;/b&gt;&lt;br /&gt;This field specifies the port number that the proxy expects connections on.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Username</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Username for proxy authentication</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Username for proxy authentication.&lt;/b&gt;&lt;br /&gt;This specifies the username you use for authenticating yourself with the proxy. In case the proxy does not use authentication, or you want to connect anonymously, simply leave this field blank.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Password</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Password for proxy authentication</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Password for proxy authentication.&lt;/b&gt;&lt;br /&gt;This specifies the password you use for authenticating yourself with the proxy. In case the proxy does not use authentication, or you want to connect anonymously, simply leave this field blank.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Misc</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Prevent log from downloading images</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Disable image download&lt;/b&gt;&lt;br/&gt;
Prevents the client from downloading images embedded into chat messages with the img tag.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Disable image download</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Mumble services</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Check for new releases of Mumble automatically.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This will check for new releases of Mumble every time you start the program, and notify you if one is available.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Check for application updates on startup</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Check for new releases of plugins automatically.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This will check for new releases of plugins every time you start the program, and download them automatically.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Download plugin updates on startup</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Submit anonymous statistics.&lt;/b&gt;&lt;br /&gt;Mumble has a small development team, and as such needs to focus its development where it is needed most. By submitting a bit of statistics you help the project determine where to focus development.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Submit anonymous statistics to the Mumble project</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Submit anonymous statistics</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Reconnect to last used server when starting Mumble</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Reconnect to last server on startup</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>OSSConfig</name>
    <message>
        <source>%1ms</source>
        <translation type="obsolete">%1мс</translation>
    </message>
    <message>
        <source>Device selection</source>
        <translation type="obsolete">Выбор устройства</translation>
    </message>
    <message>
        <source>Input</source>
        <translation type="obsolete">Ввод</translation>
    </message>
    <message>
        <source>Output</source>
        <translation type="obsolete">Вывод</translation>
    </message>
    <message>
        <source>Device to use for speakers/headphones</source>
        <translation type="obsolete">Устройство, используемое как динамики/наушники</translation>
    </message>
    <message>
        <source>Output Options</source>
        <translation type="obsolete">Свойства вывода</translation>
    </message>
    <message>
        <source>Output Delay</source>
        <translation type="obsolete">Задержка вывода</translation>
    </message>
</context>
<context>
    <name>Overlay</name>
    <message>
        <source>Mumble</source>
        <translation type="obsolete">Mumble</translation>
    </message>
    <message>
        <location filename="Overlay.cpp" line="+1588"/>
        <source>Failed to create communication with overlay at %2: %1. No overlay will be available.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Failed to load overlay library. This means either that:
- the library (mumble_ol.dll) wasn&apos;t found in the directory you ran Mumble from
- you&apos;re on an OS earlier than WinXP SP2
- you do not have the August 2006 updated version of DX9.0c</source>
        <translation type="obsolete">Невозможно загрузить библиотеку замещения. Это означает, что либо:
- библиотека (mumble_ol.dll) не была найдена в папке, из которой вы запустили Mumble
- Ваша ОС старше, чем WinXP SP2
- у Вас нет обновленной в августе 2006 версии DX9.0c</translation>
    </message>
</context>
<context>
    <name>OverlayConfig</name>
    <message>
        <location filename="Overlay.ui"/>
        <source>Options</source>
        <translation>Настройки</translation>
    </message>
    <message>
        <location/>
        <source>Position</source>
        <translation>Позиция</translation>
    </message>
    <message>
        <location/>
        <source>Font</source>
        <translation>Шрифт</translation>
    </message>
    <message>
        <location/>
        <source>Enable Overlay</source>
        <translation>Включить замещение</translation>
    </message>
    <message>
        <location/>
        <source>Enable overlay.</source>
        <translation>Включить замещение.</translation>
    </message>
    <message>
        <source>This sets whether the overlay is enabled or not. This setting is only checked with D3D9 applications are started, so make sure Mumble is running and this option is on before you start the application.&lt;br /&gt;Please note that if you start the application after starting Mumble, or if you disable the overlay while running, there is no safe way to restart the overlay without also restarting the application.</source>
        <translation type="obsolete">Устанавливает, включить замещение или нет. Опция отмечена, если запущены приложения D3D9, так что удостоверьтесь, что Mumble запущен и эта опция включена перед стартом приложения.&lt;br /&gt;Заметьте, что если вы запустите приложение после запуска Mumble, или вы отключите замещение во время действия программы, не будет безопасного способа заново включить замещение без перезапуска программы.</translation>
    </message>
    <message>
        <source>No one</source>
        <translation type="obsolete">Никого</translation>
    </message>
    <message>
        <source>Only talking</source>
        <translation type="obsolete">Только разговаривающие</translation>
    </message>
    <message>
        <source>Everyone</source>
        <translation type="obsolete">Всем</translation>
    </message>
    <message>
        <source>Show</source>
        <translation type="obsolete">Показать</translation>
    </message>
    <message>
        <location/>
        <source>Who to show on the overlay</source>
        <translation>Кого показывать при замещении</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets who to show in the in-game overlay.&lt;/b&gt;&lt;br /&gt;If many people are connected to the same channel, the overlay list might be very long. Use this to shorten it.&lt;br /&gt;&lt;i&gt;No one&lt;/i&gt; - Don&apos;t show anyone (but leave overlay running).&lt;br /&gt;&lt;i&gt;Only talking&lt;/i&gt; - Only show talking people.&lt;br /&gt;&lt;i&gt;Everyone&lt;/i&gt; - Show everyone.</source>
        <translation>&lt;b&gt;Определяет, кого показывать в игровом замещении.&lt;/b&gt;&lt;br /&gt;Если много людей на одном канале, замещение может быть очень долгим. Используйте это, чтобы сократить его.&lt;br /&gt;&lt;i&gt;Никого&lt;/i&gt; -Не показывать никого (но оставить замещение включенным).&lt;br /&gt;&lt;i&gt;Только говорящих&lt;/i&gt; - Показывать только говорящих людей.&lt;br /&gt;&lt;i&gt;Всех&lt;/i&gt; - Показывать всех.</translation>
    </message>
    <message>
        <location/>
        <source>Always Show Self</source>
        <translation>Всегда показывать меня</translation>
    </message>
    <message>
        <location/>
        <source>Always show yourself on overlay.</source>
        <translation>Всегда показывать себя в замещении.</translation>
    </message>
    <message>
        <location/>
        <source>This sets whether to always show yourself or not. This setting is useful if you aren&apos;t showing everyone in the overlay, as then you would only see your own status if you were talking, which wouldn&apos;t let you see that you were deafened or muted.</source>
        <translation>Устанавливает, всегда ли показывать себя. Настройка применима если вы не отображаете всех в замещении, поэтому тогда вы увидите собственный статус при разговоре, который вы не услышите, если в онемении или заглушены.</translation>
    </message>
    <message>
        <location/>
        <source>Grow Left</source>
        <translation>Расти влево</translation>
    </message>
    <message>
        <location/>
        <source>Let overlay grow to the left</source>
        <translation>Позволяет замещению расти влево</translation>
    </message>
    <message>
        <location/>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow to the left if needed.</source>
        <translation>Замещение пытается остаться как можно меньшим и на выбранной позиции. Позволяет замещению расти влево при необходимости.</translation>
    </message>
    <message>
        <location/>
        <source>Grow Right</source>
        <translation>Расти вправо</translation>
    </message>
    <message>
        <source>Let overlay grow to the Right</source>
        <translation type="obsolete">Позволяет замещению расти вправо</translation>
    </message>
    <message>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow to the Right if needed.</source>
        <translation type="obsolete">Замещение попытается оставться как можно меньшим и на установленой позиции. Позволяет замещению расти вправо при необходимости.</translation>
    </message>
    <message>
        <location/>
        <source>Grow Up</source>
        <translation>Расти вверх</translation>
    </message>
    <message>
        <location/>
        <source>Let overlay grow upwards</source>
        <translation>Позволяет замещению расти вверх</translation>
    </message>
    <message>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow to the top if needed.</source>
        <translation type="obsolete">Замещение попытается оставться как можно меньшим и на установленой позиции. Позволяет замещению расти вверх при необходимости.</translation>
    </message>
    <message>
        <location/>
        <source>Grow Down</source>
        <translation>Расти вниз</translation>
    </message>
    <message>
        <location/>
        <source>Let overlay grow downwards</source>
        <translation>Позволяет замещению расти вниз</translation>
    </message>
    <message>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow towards the bottom if needed.</source>
        <translation type="obsolete">Замещение попытается оставться как можно меньшим и на установленой позиции. Позволяет замещению расти вниз при необходимости.</translation>
    </message>
    <message>
        <location/>
        <source>X-Position of Overlay</source>
        <translation>Позиция Х замещения</translation>
    </message>
    <message>
        <location/>
        <source>This sets the relative X position of the overlay.</source>
        <translation>Устанавливает относительную Х позицию замещения.</translation>
    </message>
    <message>
        <location/>
        <source>Y-Position of Overlay</source>
        <translation>Y-позиция замещения</translation>
    </message>
    <message>
        <location/>
        <source>This sets the relative Y position of the overlay.</source>
        <translation>Устанавливает относительную Y - позицию замещения.</translation>
    </message>
    <message>
        <location/>
        <source>Current Font</source>
        <translation>Текущий шрифт</translation>
    </message>
    <message>
        <location/>
        <source>Set Font</source>
        <translation>Установить шрифт</translation>
    </message>
    <message>
        <source>Maximum width of names.</source>
        <translation type="obsolete">Максимальная длина имен.</translation>
    </message>
    <message>
        <source>This sets the maximum width of names shown, relative to the height. If you set this to 100%, no line of text will be wider than it is high. At 500%, no line will be more than 5 times wider than its height, and so on.</source>
        <translation type="obsolete">Устанавливает максимальную ширину отображаемых имен, относительно высоты. Если установить на 100%, ни одна строка в тексте не будет шире его высоты. На 500% ни одна строка не будет больше ширины в 5 раз, и так далее.</translation>
    </message>
    <message>
        <source>Maximum width</source>
        <translation type="obsolete">Максимальная ширина</translation>
    </message>
    <message>
        <location/>
        <source>Change</source>
        <translation>Изменить</translation>
    </message>
    <message>
        <source>Color for players</source>
        <translation type="obsolete">Цвет для игроков</translation>
    </message>
    <message>
        <source>Color for talking players</source>
        <translation type="obsolete">Цвет для разговаривающих игроков</translation>
    </message>
    <message>
        <source>Color for alt-talking players</source>
        <translation type="obsolete">Цвета для усиленно говорящих игроков</translation>
    </message>
    <message>
        <location/>
        <source>Color for Channels</source>
        <translation>Цвет для каналов</translation>
    </message>
    <message>
        <location/>
        <source>Color for active Channels</source>
        <translation>Цвет для Активных каналов</translation>
    </message>
    <message>
        <location filename="Overlay.cpp" line="-1449"/>
        <source>Overlay</source>
        <translation>Замещение</translation>
    </message>
    <message>
        <location filename="Overlay.ui"/>
        <source>Maximum height of names.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This sets the maximum height of names shown, relative to the screen height. If your active 3D window is 800 pixels tall and this is set to 5%, each name will be 40 pixels tall. Note that the names will not be taller than 60 pixels no matter what you set here.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Maximum height</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Overlay.cpp" line="-89"/>
        <source>Show no one</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Show only talking</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Show everyone</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Overlay.ui"/>
        <location filename="Overlay.cpp" line="+67"/>
        <source>Color for users</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <location filename="Overlay.cpp" line="+4"/>
        <source>Color for talking users</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <location filename="Overlay.cpp" line="+4"/>
        <source>Color for whispering users</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Overlay.cpp" line="+4"/>
        <source>Color for channels</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Color for active channels</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Overlay.ui"/>
        <source>Form</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This sets whether the overlay is enabled or not. This setting is only checked when applications are started, so make sure Mumble is running and this option is on before you start the application.&lt;br /&gt;Please note that if you start Mumble after starting the application, or if you disable the overlay while the application is running, there is no safe way to restart the overlay without also restarting the application.</source>
        <oldsource>This sets whether the overlay is enabled or not. This setting is only checked when applications are started, so make sure Mumble is running and this option is on before you start the application.&lt;br /&gt;Please note that if you start the application after starting Mumble, or if you disable the overlay while running, there is no safe way to restart the overlay without also restarting the application.</oldsource>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow upwards if needed.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Let overlay grow to the right</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow to the right if needed.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow downwards if needed.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>TextLabel</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>OverlayUser</name>
    <message>
        <location filename="Overlay.cpp" line="+46"/>
        <source>Channel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Silent</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Talking</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Whisper</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Shout</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+600"/>
        <source>User Opacity</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <location line="+16"/>
        <source>%1%</source>
        <translation type="unfinished">%1%</translation>
    </message>
    <message>
        <location line="-6"/>
        <source>Object Opacity</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+19"/>
        <source>Color...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Font...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+43"/>
        <source>Pick color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+17"/>
        <source>Pick font</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PlayerModel</name>
    <message>
        <source>Name of player</source>
        <translation type="obsolete">Имя игрока</translation>
    </message>
    <message>
        <source>Name of channel</source>
        <translation type="obsolete">Имя канала</translation>
    </message>
    <message>
        <source>Player flags</source>
        <translation type="obsolete">Флаги игрока</translation>
    </message>
    <message>
        <source>This is a player connected to the server. The icon to the left of the player indicates whether or not they are talking:&lt;br /&gt;&lt;img src=&quot;:/icons/talking_on.png&quot; /&gt; Talking&lt;br /&gt;&lt;img src=&quot;:/icons/talking_off.png&quot; /&gt; Not talking</source>
        <translation type="obsolete">Этот игрок подключен к серверу. Иконка слева от игрока отображает, разговаривает он сейчас или нет:&lt;br /&gt;&lt;img src=&quot;:/icons/talking_on.png&quot; /&gt;Говорит&lt;br /&gt;&lt;img src=&quot;:/icons/talking_off.png&quot; /&gt;Не говорит</translation>
    </message>
    <message>
        <source>This is a channel on the server. Only players in the same channel can hear each other.</source>
        <translation type="obsolete">Канал на сервере. Только игроки на одном канале могут слушать друг друга.</translation>
    </message>
    <message>
        <source>This shows the flags the player has on the server, if any:&lt;br /&gt;&lt;img src=&quot;:/icons/authenticated.png&quot; /&gt;Authenticated user&lt;br /&gt;&lt;img src=&quot;:/icons/muted_self.png&quot; /&gt;Muted (by self)&lt;br /&gt;&lt;img src=&quot;:/icons/muted_server.png&quot; /&gt;Muted (by admin)&lt;br /&gt;&lt;img src=&quot;:/icons/deafened_self.png&quot; /&gt;Deafened (by self)&lt;br /&gt;&lt;img src=&quot;:/icons/deafened_server.png&quot; /&gt;Deafened (by admin)&lt;br /&gt;A player muted by himself is probably just away, talking on the phone or something like that.&lt;br /&gt;A player muted by an admin is probably also just away, and the noise the player is making was annoying enough that an admin muted him.</source>
        <translation type="obsolete">Показывает флаги игрока на сервере,если любой:&lt;br /&gt;&lt;img src=&quot;:/icons/authenticated.png&quot; /&gt;Зарегистрированный пользователь&lt;br /&gt;&lt;img src=&quot;:/icons/muted_self.png&quot; /&gt;Заглушенный (собой)&lt;br /&gt;&lt;img src=&quot;:/icons/muted_server.png&quot; /&gt;Заглушенный (администратором)&lt;br /&gt;&lt;img src=&quot;:/icons/deafened_self.png&quot; /&gt;В онемении (собственном)&lt;br /&gt;&lt;img src=&quot;:/icons/deafened_server.png&quot; /&gt;В онемении (администратора)&lt;br /&gt;Пользователь,заглушенный собой, обычно отошел от компьютера, говорит по телефону, или что-то в этом роде.&lt;br /&gt;Игрок, заглушенный администратором, возможно просто отошел, и шум игрока надоедал администратору,и он заглушил его.</translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="obsolete">Имя</translation>
    </message>
    <message>
        <source>Flags</source>
        <translation type="obsolete">Флаги</translation>
    </message>
    <message>
        <source>Mumble</source>
        <translation type="obsolete">Mumble</translation>
    </message>
</context>
<context>
    <name>PluginConfig</name>
    <message>
        <location filename="Plugins.ui"/>
        <source>Options</source>
        <translation>Настройки</translation>
    </message>
    <message>
        <location/>
        <location filename="Plugins.cpp" line="+75"/>
        <source>Plugins</source>
        <translation>Плагины</translation>
    </message>
    <message>
        <location/>
        <source>Link to Game and Transmit Position</source>
        <translation>Связаться с игрой и передать позицию</translation>
    </message>
    <message>
        <location/>
        <source>Enable plugins and transmit positional information</source>
        <translation>Включить плагины и передать информацию о позиции</translation>
    </message>
    <message>
        <source>This enables plugins for supported games to fetch your in-game position and transmit that with each voice packet. This enables other players to hear your voice in-game from the direction your character is in relation to their own.</source>
        <translation type="obsolete">Включает плагины для поддерживаемых игр, чтобы соответствовать вашей позиции в игре и передавать их с каждым голосовым пакетом. Позволяет другим игрокам слушать ваш голос в игре оттуда, откуда он расположен по отношению к ним.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Reload plugins</source>
        <translation>&amp;Перезагрузить плагины</translation>
    </message>
    <message>
        <location/>
        <source>Reloads all plugins</source>
        <translation>Перезагрузить все плагины</translation>
    </message>
    <message>
        <location/>
        <source>This allows plugins for supported games to fetch your in-game position and transmit it with each voice packet. This enables other users to hear your voice in-game from the direction your character is in relation to their own.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This rescans and reloads plugins. Use this if you just added or changed a plugin to the plugins directory.</source>
        <translation>Пересканирует и перезагружает плагины. Используйте, если вы только что добавили или изменили плагин в папке с плагинами.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;About</source>
        <translation>&amp;О плагине</translation>
    </message>
    <message>
        <location/>
        <source>Information about plugin</source>
        <translation>Информация о плагине</translation>
    </message>
    <message>
        <location/>
        <source>This shows a small information message about the plugin.</source>
        <translation>Покажет небольшую справку о плагине.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Configure</source>
        <translation>&amp;Настроить</translation>
    </message>
    <message>
        <location/>
        <source>Show configuration page of plugin</source>
        <translation>Показать страницу настроек плагина</translation>
    </message>
    <message>
        <location/>
        <source>This shows the configuration page of the plugin, if any.</source>
        <translation>Показывает страницу настроек плагина, если есть.</translation>
    </message>
    <message>
        <source>Mumble</source>
        <translation type="obsolete">Mumble</translation>
    </message>
    <message>
        <location filename="Plugins.cpp" line="+54"/>
        <source>Plugin has no configure function.</source>
        <translation>Плагин не имеет функции конфигурирования.</translation>
    </message>
    <message>
        <location line="+14"/>
        <source>Plugin has no about function.</source>
        <translation>У плагина нет функции информации.</translation>
    </message>
    <message>
        <location filename="Plugins.ui"/>
        <source>Form</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Name</source>
        <translation type="unfinished">Имя</translation>
    </message>
    <message>
        <location/>
        <source>Enabled</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Plugins</name>
    <message>
        <location filename="Plugins.cpp" line="+309"/>
        <source>Skipping plugin update in debug mode.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+155"/>
        <location line="+6"/>
        <source>Downloaded new or updated plugin to %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Failed to install new plugin to %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Plugin %1 lost link.</source>
        <translation type="obsolete">Плагин %1 потерял связь.</translation>
    </message>
    <message>
        <location line="-258"/>
        <source>%1 lost link.</source>
        <translation>%1 потерял связь.</translation>
    </message>
    <message>
        <source>Plugin %1 linked.</source>
        <translation type="obsolete">Плагин %1 подключен.</translation>
    </message>
    <message>
        <location line="+68"/>
        <source>%1 linked.</source>
        <translation>%1 подключен.</translation>
    </message>
</context>
<context>
    <name>PortAudioSystem</name>
    <message>
        <location filename="PAAudio.cpp" line="+275"/>
        <source>Default Device</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PulseAudioConfig</name>
    <message>
        <source>%1ms</source>
        <translation type="obsolete">%1мс</translation>
    </message>
    <message>
        <source>Device selection</source>
        <translation type="obsolete">Выбор устройства</translation>
    </message>
    <message>
        <source>Output</source>
        <translation type="obsolete">Вывод</translation>
    </message>
    <message>
        <source>Device to use for speakers/headphones</source>
        <translation type="obsolete">Устройство, используемое как динамики/наушники</translation>
    </message>
    <message>
        <source>Output Options</source>
        <translation type="obsolete">Свойства вывода</translation>
    </message>
    <message>
        <source>Output Delay</source>
        <translation type="obsolete">Задержка вывода</translation>
    </message>
</context>
<context>
    <name>PulseAudioSystem</name>
    <message>
        <location filename="PulseAudio.cpp" line="+549"/>
        <source>Default Input</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Default Output</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>RichTextEditor</name>
    <message>
        <location filename="RichTextEditor.cpp" line="+241"/>
        <source>Failed to load image</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Image file too large to embed in document. Please use images smaller than %1 kB.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+81"/>
        <source>Message is too long.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="RichTextEditor.ui"/>
        <source>Display</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Source Text</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&amp;Bold</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Ctrl+B</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&amp;Italic</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Italic</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Ctrl+I</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Underline</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Ctrl+U</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Insert Link</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Ctrl+L</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Insert Image</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>RichTextEditorLink</name>
    <message>
        <location filename="RichTextEditorLink.ui"/>
        <source>Add Link</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>URL</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Text</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ServerHandler</name>
    <message>
        <location filename="ServerHandler.cpp" line="+394"/>
        <source>UDP packets cannot be sent to or received from the server. Switching to TCP mode.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>UDP packets cannot be sent to the server. Switching to TCP mode.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>UDP packets cannot be received from the server. Switching to TCP mode.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+7"/>
        <source>UDP packets can be sent to and received from the server. Switching back to UDP mode.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+29"/>
        <source>Connection timed out</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ServerView</name>
    <message>
        <location filename="ConnectDialog.cpp" line="-664"/>
        <source>Favorite</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>LAN</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Public Internet</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Africa</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Asia</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>North America</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>South America</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Europe</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Oceania</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ShortcutActionWidget</name>
    <message>
        <location filename="GlobalShortcut.cpp" line="-484"/>
        <source>Unassigned</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ShortcutDelegate</name>
    <message>
        <location line="+360"/>
        <source>On</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Off</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Toggle</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Unassigned</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ShortcutKeyWidget</name>
    <message>
        <location line="-434"/>
        <source>Press Shortcut</source>
        <translation type="unfinished">Нажмите ярлык</translation>
    </message>
</context>
<context>
    <name>ShortcutTargetDialog</name>
    <message>
        <location line="+203"/>
        <source>Root</source>
        <translation type="unfinished">Корень</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Parent</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Current</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Subchannel #%1</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ShortcutTargetWidget</name>
    <message>
        <location line="+87"/>
        <source>...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+38"/>
        <source>, </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Root</source>
        <translation type="unfinished">Корень</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Parent</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Current</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Subchannel #%1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Invalid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>&lt;Empty&gt;</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ShortcutToggleWidget</name>
    <message>
        <location line="-261"/>
        <source>Off</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Toggle</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>On</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>TextMessage</name>
    <message>
        <location filename="TextMessage.h" line="+46"/>
        <source>Enter text</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="TextMessage.ui"/>
        <source>If checked the message is recursively sent to all subchannels</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Send recursively to subchannels</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Tokens</name>
    <message>
        <location filename="Tokens.cpp" line="+69"/>
        <source>Empty Token</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Tokens.ui"/>
        <source>Mumble - Access Tokens</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>List of access tokens on current server</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is an editable list of access tokens on the connected server.&lt;/b&gt;
&lt;br /&gt;
An access token is a text string, which can be used as a password for very simple access management on channels. Mumble will remember the tokens you&apos;ve used and resend them to the server next time you reconnect, so you don&apos;t have to enter these every time.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Add a token</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&amp;Add</source>
        <translation type="unfinished">&amp;Добавить</translation>
    </message>
    <message>
        <location/>
        <source>Remove a token</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&amp;Remove</source>
        <translation type="unfinished">&amp;Удалить</translation>
    </message>
</context>
<context>
    <name>UserEdit</name>
    <message>
        <location filename="UserEdit.ui"/>
        <source>Registered Users</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <location filename="UserEdit.cpp" line="+96"/>
        <source>Remove</source>
        <translation type="unfinished">Убрать</translation>
    </message>
    <message>
        <location filename="UserEdit.cpp" line="-5"/>
        <source>Rename</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>UserInformation</name>
    <message>
        <location filename="UserInformation.ui"/>
        <source>User Information</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Connection Information</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Version</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>OS</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Certificate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>IP Address</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>CELT Versions</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Details...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Ping Statistics</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Pings recieved</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Average ping</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>TCP (Control)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>UDP (Voice)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>UDP Network statistics</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Good</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Late</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Lost</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Resync</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>From Client</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>To Client</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Connection time</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="UserInformation.cpp" line="+93"/>
        <source>%1w</source>
        <translation type="unfinished">%1w</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1d</source>
        <translation type="unfinished">%1d</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1h</source>
        <translation type="unfinished">%1h</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1m</source>
        <translation type="unfinished">%1м</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>%1s</source>
        <translation type="unfinished">%1 с</translation>
    </message>
    <message>
        <location line="+29"/>
        <location line="+43"/>
        <source>, </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-13"/>
        <source>%1.%2.%3 (%4)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>%1 (%2)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+45"/>
        <source>%1 online (%2 idle)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1 online</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>%1 kbit/s</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="UserInformation.ui"/>
        <source>Bandwidth</source>
        <comment>GroupBox</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Bandwidth</source>
        <comment>Label</comment>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>UserModel</name>
    <message>
        <location filename="UserModel.cpp" line="-811"/>
        <source>This is a user connected to the server. The icon to the left of the user indicates whether or not they are talking:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Talking to your channel.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Whispering directly to your channel.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Whispering directly to you.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Not talking.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+8"/>
        <source>This is a channel on the server. The icon indicates the state of the channel:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Your current channel.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>A channel that is linked with your channel. Linked channels can talk to each other.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>A channel on the server that you are not linked to.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+16"/>
        <source>This shows the flags the user has on the server, if any:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>On your friend list</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Authenticated user</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Muted (manually muted by self)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Muted (manually muted by admin)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Muted (not allowed to speak in current channel)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Muted (muted by you, only on your machine)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Deafened (by self)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Deafened (by admin)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>User has a new comment set (click to show)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>User has a comment set, which you&apos;ve already seen. (click to show)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+7"/>
        <source>This shows the flags the channel has, if any:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Channel has a new comment set (click to show)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Channel has a comment set, which you&apos;ve already seen. (click to show)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+18"/>
        <source>Name</source>
        <translation type="unfinished">Имя</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Flags</source>
        <translation type="unfinished">Флаги</translation>
    </message>
    <message>
        <source>Mumble</source>
        <translation type="obsolete">Mumble</translation>
    </message>
    <message>
        <location line="+728"/>
        <source>Are you sure you want to drag this channel?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+93"/>
        <location line="+20"/>
        <source>Cannot perform this movement automatically, please reset the numeric sorting indicators or adjust it manually.</source>
        <translation type="unfinished"></translation>
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
        <translation type="obsolete">Mumble возвратил ошибку при запросе информации о версии с сервера SourceForge.</translation>
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
        <source>Failed to write new version to disc.</source>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Certificate chain</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Certificate details</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-3"/>
        <source>%1 %2</source>
        <translation type="unfinished">%1 %2</translation>
    </message>
    <message>
        <location line="+31"/>
        <location line="+28"/>
        <source>Common Name: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-27"/>
        <location line="+28"/>
        <source>Organization: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-27"/>
        <source>Subunit: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <location line="+28"/>
        <source>Country: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-27"/>
        <location line="+28"/>
        <source>Locality: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-27"/>
        <location line="+28"/>
        <source>State: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-27"/>
        <source>Valid from: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Valid to: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Serial: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Public Key: %1 bits %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+0"/>
        <source>RSA</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+0"/>
        <source>DSA</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Digest (MD5): %1</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Unit Name: %1</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>WASAPIConfig</name>
    <message>
        <source>Device selection</source>
        <translation type="obsolete">Выбор устройства</translation>
    </message>
    <message>
        <source>Output</source>
        <translation type="obsolete">Вывод</translation>
    </message>
    <message>
        <source>Device to use for speakers/headphones</source>
        <translation type="obsolete">Устройство, используемое как динамики/наушники</translation>
    </message>
</context>
<context>
    <name>WASAPISystem</name>
    <message>
        <location filename="WASAPI.cpp" line="+172"/>
        <source>Default Device</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>XInputKeyWidget</name>
    <message>
        <source>Press Shortcut</source>
        <translation type="obsolete">Нажмите ярлык</translation>
    </message>
</context>
</TS>
