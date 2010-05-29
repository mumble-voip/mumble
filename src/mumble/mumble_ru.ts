<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.0" language="ru_RU">
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
        <translation>Здесь можно задать группы пользователей, у которых будет доступ к этому каналу.&lt;br /&gt;Обратите внимание, что группа вычисляется в контексте канала, в котором находится этот канал.Например, стандартный ACL (список контроля доступа) для канала Root дает право &lt;i&gt;Записи&lt;/i&gt; для группы &lt;i&gt;admin&lt;/i&gt;. Если права этого канала(1) будут унаследованы каким-либо другим каналом(2), то пользователь, принадлежащий к группе &lt;i&gt;admin&lt;/i&gt; в  канале 2, будет иметь разрешение на запись, даже если он не принодлежит к группе &lt;i&gt;admin&lt;/i&gt; в канале 0, в котором находится канал 1.&lt;br /&gt;Если название группы начинается с &apos;!&apos;, эта группа не будет иметь прав доступа, а если с &apos;~&apos;,то права группы вычисляются в соответствии с каналом 0, вместо канала 1.&lt;br /&gt;Если имя группы начинается с &apos;#&apos;, то оно интерпретируется как маркер доступа.То что идет после &apos;#&apos; в имени группы, должно быть введено в список маркеров доступа пользователем, чтобы он мог войти.Это может быть использовано для очень простого доступа с паролем к каналу для не авторизированных пользователей.&lt;br /&gt; Если имя группы начинается с &apos;$&apos;, доступ будут иметь только пользователи, чей хэш сертификата соответствует тому, что следует в имени группы после &apos;$&apos;.&lt;br /&gt; Несколько предустановленных групп:&lt;br /&gt;&lt;b&gt;all&lt;/b&gt; - Соответствует абсолютно всем пользователям.&lt;br /&gt;&lt;b&gt;auth&lt;/b&gt; - Соответствует только авторизированным пользователям.&lt;br /&gt;&lt;b&gt;sub,a,b,c&lt;/b&gt; - Соответствует пользователям, которые находятся в подканале не выше &lt;i&gt;a&lt;/i&gt;, и между &lt;i&gt;b&lt;/i&gt; и &lt;i&gt;c&lt;/i&gt; вниз по цепочке. Более подробная документация на вебсайте программы.&lt;br /&gt;&lt;b&gt;in&lt;/b&gt; - Соответствует всем пользователям данного канала (для удобства, вместо &apos;&lt;i&gt;sub,0,0,0&lt;/i&gt;&apos;).&lt;br /&gt;&lt;b&gt;out&lt;/b&gt; - Соответствует пользователям, которых нет в данном канале (для удобства, вместо &apos;&lt;i&gt;!sub,0,0,0&lt;/i&gt;&apos;).&lt;br /&gt;Одна запись соответствует одному польвателю или группе, но не обоим.</translation>
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
        <translation>Имя</translation>
    </message>
    <message>
        <location/>
        <source>Enter the channel name here.</source>
        <translation>Введите название канала.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Name&lt;/b&gt;&lt;br&gt;Enter the channel name in this field. The name has to comply with the restriction imposed by the server you are connected to.</source>
        <translation>&lt;b&gt;Имя&lt;/b&gt;&lt;br&gt;Введите название канала в это поле. Имя должно соответствовать требованиям сервера, к которому Вы подключены.</translation>
    </message>
    <message>
        <location/>
        <source>Description</source>
        <translation>Описание</translation>
    </message>
    <message>
        <location/>
        <source>Password</source>
        <translation>Пароль</translation>
    </message>
    <message>
        <location/>
        <source>Enter the channel password here.</source>
        <translation>Введите здесь пароль для канала.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Password&lt;/b&gt;&lt;br&gt;This field allows you to easily set and change the password of a channel. In the background it uses Mumble&apos;s access tokens feature. To allow more fine grained and powerful access control directly use ACLs and groups instead (&lt;i&gt;Advanced configuration&lt;/i&gt; has to be checked to be able to see these settings).</source>
        <translation>&lt;b&gt;Пароль&lt;/b&gt;&lt;br&gt; Это поле позволяет Вам установить или изменить пароль канала. Эта опция использует маркеры доступа Mumble. Если вы хотите более мощный способ контроля доступом, используйте ACL (списки контроля доступа) и группы (Для отображения этих настроек необходимо установить флажок&lt;i&gt;Расширенные настройки&lt;/i&gt;).</translation>
    </message>
    <message>
        <location/>
        <source>Check to create a temporary channel.</source>
        <translation>Отметьте, чтобы создать временный канал.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Temporary&lt;/b&gt;&lt;br&gt;
When checked the channel created will be marked as temporary. This means when the last player leaves it the channel will be automatically deleted by the server.</source>
        <translation>&lt;b&gt;Временный&lt;/b&gt;&lt;br&gt;
Если отмечено - созданный канал будет обозначен как временный. Это означает, что когда все пользователи покинут этот канал, он будет удален сервером.</translation>
    </message>
    <message>
        <location/>
        <source>Temporary</source>
        <translation>Временный</translation>
    </message>
    <message>
        <location/>
        <source>Channel positioning facility value</source>
        <translation>Позиция канала в списке</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Position&lt;/b&gt;&lt;br/&gt;
This value enables you to change the way Mumble arranges the channels in the tree. A channel with a higher &lt;i&gt;Position&lt;/i&gt; value will always be placed below one with a lower value and the other way around. If the &lt;i&gt;Position&lt;/i&gt; value of two channels is equal they will get sorted alphabetically by their name.</source>
        <oldsource>&lt;b&gt;Position&lt;/b&gt;&lt;br/&gt;
This value enables you to change the way mumble arranges the channels in the tree. A channel with a higher &lt;i&gt;Position&lt;/i&gt; value will always be placed below one with a lower value and the other way around. If the &lt;i&gt;Position&lt;/i&gt; value of two channels is equal they will get sorted alphabetically by their name.</oldsource>
        <translation>&lt;b&gt;Позиция&lt;/b&gt;&lt;br/&gt;
Это значение позволяет изменять порядок следования каналов в списке каналов. Канал с более высоким значением &lt;i&gt;Позиции&lt;/i&gt; всегда будет находится под каналом с более низким значением, и наоборот. Если &lt;i&gt;Позиции&lt;/i&gt; у каналов равны, то они будут отсортированы в алфавитном порядке.</translation>
    </message>
    <message>
        <location/>
        <source>Position</source>
        <translation>Позиция</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Group&lt;/b&gt;&lt;br&gt;
These are all the groups currently defined for the channel. To create a new group, just type in the name and press enter.</source>
        <oldsource>&lt;b&gt;Group&lt;/b&gt;&lt;br&gt;
This is all the groups currently defined for the channel. To create a new group, just type in the name and press enter.</oldsource>
        <translation>&lt;b&gt;Группа&lt;/b&gt;&lt;br&gt;
В этом списке все группы, определенные для этого канала. Чтобы создать новую группу, просто введите название и нажмите Ввод.</translation>
    </message>
    <message>
        <location/>
        <source>Add new group</source>
        <translation>Добавить новую группу</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Add&lt;/b&gt;&lt;br/&gt;
Add a new group.</source>
        <translation>&lt;b&gt;Добавить&lt;/b&gt;&lt;br/&gt;
Добавить новую группу.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Remove&lt;/b&gt;&lt;br&gt;This removes the currently selected group. If the group was inherited, it will not be removed from the list, but all local information about the group will be cleared.</source>
        <translation>&lt;b&gt;Удалить&lt;/b&gt;&lt;br&gt;Удалить выбранную группу. Если группа наследуется, то она не будте удалена из писка, но местная информация о группе будет очищена.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Inherit&lt;/b&gt;&lt;br&gt;This inherits all the members in the group from the parent, if the group is marked as &lt;i&gt;Inheritable&lt;/i&gt; in the parent channel.</source>
        <translation>&lt;b&gt;Наследовать&lt;/b&gt;&lt;br&gt;Наследует всех пользователей из родительской группы, если группа обозначена как &lt;i&gt;Наследуемая&lt;/i&gt; в родительском канале.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Inheritable&lt;/b&gt;&lt;br&gt;This makes this group inheritable to sub-channels. If the group is non-inheritable, sub-channels are still free to create a new group with the same name.</source>
        <translation>&lt;b&gt;Наследуемая&lt;/b&gt;&lt;br&gt;Делает группу наследуемой для подканаловs. Если группа не наследуемая, то в подканалах все равно может быть создана группа с таким же именем.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Inherited&lt;/b&gt;&lt;br&gt;This indicates that the group was inherited from the parent channel. You cannot edit this flag, it&apos;s just for information.</source>
        <translation>&lt;b&gt;Наследована&lt;/b&gt;&lt;br&gt;Обозначает, что группа была наследована из родительского канала. Это значение не редактируется, и служит в информационных целях.</translation>
    </message>
    <message>
        <location/>
        <source>Inherited members</source>
        <translation>Унаследованные пользователи</translation>
    </message>
    <message>
        <location/>
        <source>Contains the list of members added to the group by this channel.</source>
        <translation>Содержит список пользователей, добавленных в группу этим каналом.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Members&lt;/b&gt;&lt;br&gt;
This list contains all members that were added to the group by the current channel. Be aware that this does not include members inherited by higher levels of the channel tree. These can be found in the &lt;i&gt;Inherited members&lt;/i&gt; list. To prevent this list to be inherited by lower level channels uncheck &lt;i&gt;Inheritable&lt;/i&gt; or manually add the members to the &lt;i&gt;Excluded members&lt;/i&gt; list.</source>
        <translation>&lt;b&gt;Пользователи&lt;/b&gt;&lt;br&gt;
Список содержит всех пользователей, которые были добавлены в группу текущим каналом. Имейте ввиду, что он не содержит пользователей, унаследованных из родительских каналов. Список таких пользователей можно найти в &lt;i&gt;Унаследованных пользователях&lt;/i&gt;. Чтобы э тот список не был унаследован дочерними каналами, снимите отметку с &lt;i&gt;Наследуемый&lt;/i&gt;, или вручную добавьте пользователей в список &lt;i&gt;Исключенные пользователи&lt;/i&gt;.</translation>
    </message>
    <message>
        <location/>
        <source>Contains a list of members whose group membership will not be inherited from the parent channel.</source>
        <translation>Содержит список пользователей, чье членство в группе не будет унаследовано из родительского канала.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Excluded members&lt;/b&gt;&lt;br&gt;
Contains a list of members whose group membership will not be inherited from the parent channel.</source>
        <translation>&lt;b&gt;Исключенные члены&lt;/b&gt;&lt;br&gt;
Содержит список пользователей, чье членство в группе не будет унаследовано из родительского канала.</translation>
    </message>
    <message>
        <location/>
        <source>Contains the list of members inherited by other channels.</source>
        <translation>Содержит список пользователей, наследуемых другими каналами.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Inherited members&lt;/b&gt;&lt;br&gt;
Contains the list of members inherited by the current channel. Uncheck &lt;i&gt;Inherit&lt;/i&gt; to prevent inheritance from higher level channels.</source>
        <translation>&lt;b&gt;Унаследованные пользователи&lt;/b&gt;&lt;br&gt;
Содержит список пользователей, унаследованных текущим каналом. Снимите отметку с &lt;i&gt;Наследоватьt&lt;/i&gt;, чтобы предотвратить наследование из родительских каналов.</translation>
    </message>
    <message>
        <location/>
        <source>Type in the name of a user you wish to add to the group and click Add.</source>
        <translation>Введите имя пользователя, которого Вы хотите добавить в группу и нажмите Добавить.</translation>
    </message>
    <message>
        <location/>
        <source>Type in the name of a user you wish to remove from the group and click Add.</source>
        <translation>Введите имя пользователя, которого вы хотите удалить из группы, и нажмите Добавить.</translation>
    </message>
    <message>
        <location/>
        <source>Exclude</source>
        <translation>Исключить</translation>
    </message>
    <message>
        <location/>
        <source>Excluded members</source>
        <translation>Исключенные пользователи</translation>
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
        <translation>Применять к каналу.</translation>
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
        <translation>Mumble - Добавить канал</translation>
    </message>
    <message>
        <location line="+22"/>
        <location line="+165"/>
        <source>Failed: Invalid channel</source>
        <translation>Ошибка: Неверный канал</translation>
    </message>
    <message>
        <location line="-153"/>
        <source>Mumble - Edit %1</source>
        <translation>Mumble - Изменить %1</translation>
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
        <translation>Диалог</translation>
    </message>
    <message>
        <location/>
        <source>This shows all the entries active on this channel. Entries inherited from parent channels will be show in italics.&lt;br /&gt;ACLs are evaluated top to bottom, meaning priority increases as you move down the list.</source>
        <translation>Список всех пунктов для этого канала. Пункты, унаследованные из родительских каналов обозначены курсивом.&lt;br /&gt;ACLы читаются сверху вниз, то есть нижние имеют более высокий приоритет.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Properties</source>
        <translation>&amp;Свойства</translation>
    </message>
</context>
<context>
    <name>ALSAAudioInput</name>
    <message>
        <location filename="ALSAAudio.cpp" line="+363"/>
        <source>Opening chosen ALSA Input failed: %1</source>
        <translation>Открытие ALSA Input неудачно: %1</translation>
    </message>
    <message>
        <location line="-191"/>
        <source>Default ALSA Card</source>
        <translation>Звуковая карта ALSA по умолчанию</translation>
    </message>
</context>
<context>
    <name>ALSAAudioOutput</name>
    <message>
        <location line="+326"/>
        <source>Opening chosen ALSA Output failed: %1</source>
        <translation>Открытие ALSA Output неудачно: %1</translation>
    </message>
    <message>
        <location line="-325"/>
        <source>Default ALSA Card</source>
        <translation>Звуковая карта ALSA по умолчанию</translation>
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
        <translation>Размер буфера</translation>
    </message>
    <message>
        <location/>
        <source>This will configure the input channels for ASIO. Make sure you select at least one channel as microphone and speaker. &lt;i&gt;Microphone&lt;/i&gt; should be where your microphone is attached, and &lt;i&gt;Speaker&lt;/i&gt; should be a channel that samples &apos;&lt;i&gt;What you hear&lt;/i&gt;&apos;.&lt;br /&gt;For example, on the Audigy 2 ZS, a good selection for Microphone would be &apos;&lt;i&gt;Mic L&lt;/i&gt;&apos; while Speaker should be &apos;&lt;i&gt;Mix L&lt;/i&gt;&apos; and &apos;&lt;i&gt;Mix R&lt;/i&gt;&apos;.</source>
        <translation>Здесь можно настроить входные каналы для  ASIO. Убедитесь, что выбран хотя бы один канал как микрофон и динамик. &lt;i&gt;Микрофон&lt;/i&gt; должен быть тот, который подключен, и &lt;i&gt;Динамиком&lt;/i&gt; должен быть канал, который воспроизводит звук &apos;&lt;i&gt;Который вы слышите&lt;/i&gt;&apos;.&lt;br /&gt;Например, на Audigy 2 ZS, правильным выбором Микрофона был бы  &apos;&lt;i&gt;Mic L&lt;/i&gt;&apos;, а Динамика был бы &apos;&lt;i&gt;Mix L&lt;/i&gt;&apos; и &apos;&lt;i&gt;Mix R&lt;/i&gt;&apos;.</translation>
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
        <translation>%1 (версия %2)</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>%1 -&gt; %2 samples buffer, with %3 sample resolution (%4 preferred) at %5 Hz</source>
        <oldsource>%1 -&gt; %2 samples buffer, with %3 sample resolution (%4 preferred) at %5Hz</oldsource>
        <translation>%1 -&gt; %2 буфер сэмплов, с %3 разрешением сэмплов (%4 предпочитаемый) на %5 Гц</translation>
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
        <translation>Форма</translation>
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
        <translation>О Мамбл</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>&lt;h3&gt;Mumble (%1)&lt;/h3&gt;&lt;p&gt;Copyright %3 Thorvald Natvig&lt;br /&gt;slicer@users.sourceforge.net&lt;/p&gt;&lt;p&gt;&lt;b&gt;A voice-chat utility for gamers&lt;/b&gt;&lt;/p&gt;&lt;p&gt;&lt;tt&gt;&lt;a href=&quot;%2&quot;&gt;%2&lt;/a&gt;&lt;/tt&gt;&lt;/p&gt;</source>
        <translation>&lt;h3&gt;Mumble (%1)&lt;/h3&gt;&lt;p&gt;Авторские праваt %3 Thorvald Natvig&lt;br /&gt;slicer@users.sourceforge.net&lt;/p&gt;&lt;p&gt;&lt;b&gt;Голосовой чат для геймеров&lt;/b&gt;&lt;/p&gt;&lt;p&gt;&lt;tt&gt;&lt;a href=&quot;%2&quot;&gt;%2&lt;/a&gt;&lt;/tt&gt;&lt;/p&gt;</translation>
    </message>
</context>
<context>
    <name>AboutSpeexDialog</name>
    <message>
        <location line="+25"/>
        <source>&lt;h3&gt;About Speex&lt;/h3&gt;&lt;p&gt;&lt;tt&gt;&lt;a href=&quot;%1&quot;&gt;%1&lt;/a&gt;&lt;/tt&gt;&lt;/p&gt;&lt;p&gt;This program uses SpeexDSP.&lt;/p&gt;&lt;p&gt;Speex is used for echo cancellation, noise&lt;br /&gt;filtering and voice activity detection.&lt;/p&gt;</source>
        <translation>&lt;h3&gt;О Speex&lt;/h3&gt;&lt;p&gt;&lt;tt&gt;&lt;a href=&quot;%1&quot;&gt;%1&lt;/a&gt;&lt;/tt&gt;&lt;/p&gt;&lt;p&gt;Эта программа использует SpeexDSP.&lt;/p&gt;&lt;p&gt;Speex используется для подавления эха, &lt;br /&gt;фильтрации шума и определения голосовой активности.&lt;/p&gt;</translation>
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
        <translation>О Speex</translation>
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
        <translation>Шрифт</translation>
    </message>
    <message>
        <location/>
        <source>Interface</source>
        <translation>Интерфейс</translation>
    </message>
    <message>
        <location/>
        <source>System</source>
        <translation>Система</translation>
    </message>
    <message>
        <location/>
        <source>Input method for audio</source>
        <translation>Аудио система ввода</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is the input method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation>&lt;b&gt;Этот метод ввода используется для аудио.&lt;/b&gt;&lt;br /&gt;Скорее всего вы захотите использовать DirectSound.</translation>
    </message>
    <message>
        <location/>
        <source>Device</source>
        <translation>Устройство</translation>
    </message>
    <message>
        <location/>
        <source>Input device for audio</source>
        <oldsource>Output method for audio</oldsource>
        <translation>Мытод вывода для аудио</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is the input device to use for audio.&lt;/b&gt;</source>
        <oldsource>&lt;b&gt;This is the output method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</oldsource>
        <translation>&lt;b&gt;Этот метод вывода используется для аудио.&lt;/b&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Transmission</source>
        <translation>Передача звука</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Transmit</source>
        <translation>&amp;Включение передачи</translation>
    </message>
    <message>
        <location/>
        <source>When to transmit your speech</source>
        <translation>Когда передавать вашу речь</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets when speech should be transmitted.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;Continuous&lt;/i&gt; - All the time&lt;br /&gt;&lt;i&gt;Voice Activity&lt;/i&gt; - When you are speaking clearly.&lt;br /&gt;&lt;i&gt;Push To Talk&lt;/i&gt; - When you hold down the hotkey set under &lt;i&gt;Shortcuts&lt;/i&gt;.</source>
        <translation>&lt;b&gt;Устанавливает, когда передавать речь.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;Постоянно&lt;/i&gt; - Все время&lt;br /&gt;&lt;i&gt;При голосовой активности&lt;/i&gt; - Когда вы говорите.&lt;br /&gt;&lt;i&gt;При нажатии клавиши&lt;/i&gt; - Когда вы зажимаете горячую клавишу, установленную в  &lt;i&gt;Ярлыках&lt;/i&gt;.</translation>
    </message>
    <message>
        <location/>
        <source>DoublePush Time</source>
        <translation>Скорость двойного нажатия</translation>
    </message>
    <message>
        <location/>
        <source>If you press the PTT key twice in this time it will get locked.</source>
        <translation>Если вы нажмете кнопку для включния передачи звука дважды за это время, то она зафиксируется, и не нужно будет удерживать кнопку для передачи звука, чтобы выключить передачу, нажмите на кнопку еще раз.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;DoublePush Time&lt;/b&gt;&lt;br /&gt;If you press the push-to-talk key twice during the configured interval of time it will be locked. Mumble will keep transmitting until you hit the key once more to unlock PTT again.</source>
        <translation>&lt;b&gt;Скорость двойного нажатия&lt;/b&gt;&lt;br /&gt;Если вы нажмете кнопку для включния передачи звука дважды за это время, то она зафиксируется, и не нужно будет удерживать кнопку для передачи звука, чтобы выключить передачу, нажмите на кнопку еще раз.</translation>
    </message>
    <message>
        <location/>
        <source>Gets played when the PTT button is pressed</source>
        <translation>Этот файл будет проигрываться при нажатии клавиши разговора</translation>
    </message>
    <message>
        <location/>
        <source>Gets played when the PTT button is released</source>
        <translation>Этот файл будет проигрываться при отпускании клавиши разговора</translation>
    </message>
    <message>
        <location/>
        <source>Reset audio cue to default</source>
        <translation>Сброс сигнала включени на стандартный</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Reset&lt;/b&gt;&lt;br/&gt;Reset the paths for the files to their default.</source>
        <translation>&lt;b&gt;Сброс&lt;/b&gt;&lt;br/&gt;Сброс путей файлов на стандартные.</translation>
    </message>
    <message>
        <location/>
        <source>Reset</source>
        <translation>Сброс</translation>
    </message>
    <message>
        <location/>
        <source>Browse for on audio file</source>
        <translation>Открыть аудио файл</translation>
    </message>
    <message>
        <location/>
        <source>Browse</source>
        <translation>Обзор</translation>
    </message>
    <message>
        <location/>
        <source>Browse for off audio file</source>
        <translation>Открыть аудио файл для звука отключения</translation>
    </message>
    <message>
        <location/>
        <source>Off</source>
        <translation>Звук выключения</translation>
    </message>
    <message>
        <location/>
        <source>On</source>
        <translation>Звук включения</translation>
    </message>
    <message>
        <location/>
        <source>Preview the audio cues</source>
        <translation>Проиграть звуки</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Preview&lt;/b&gt;&lt;br/&gt;Plays the current &lt;i&gt;on&lt;/i&gt; soundfile followed by the current &lt;i&gt;off&lt;/i&gt; soundfile.</source>
        <translation>&lt;b&gt;Проиграть&lt;/b&gt;&lt;br/&gt;Проиграть текущий звук &lt;i&gt;включения&lt;/i&gt; а затем текущий звук &lt;i&gt;выключения&lt;/i&gt;.</translation>
    </message>
    <message>
        <location/>
        <source>Preview</source>
        <translation>Проиграть</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This enables the audio cues for push to talk.&lt;/b&gt;&lt;br /&gt;Setting this will give you a short audio beep when push to talk is pressed and released.</source>
        <translation>&lt;b&gt;Включает звуковую индикацию для функции Push-To-Talk (PTT).&lt;/b&gt;&lt;br /&gt;Если включить, то вы будете слышать короткий звуковой сигнал при нажатии и отпускании кнопки активации микрофона (PTT - Push-To-Talk - Нажми-И-Говори).</translation>
    </message>
    <message>
        <location/>
        <source>PTT Audio cue</source>
        <translation>Звук нажатия</translation>
    </message>
    <message>
        <location/>
        <source>Use SNR based speech detection</source>
        <translation>Определение голосовой активности на основе сигнал/шум</translation>
    </message>
    <message>
        <location/>
        <source>Signal to Noise</source>
        <translation>Сигнал/Шум</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets speech detection to use Amplitude.&lt;/b&gt;&lt;br /&gt;In this mode, the raw strength of the input signal is used to detect speech.</source>
        <translation>&lt;b&gt;Установка определения голосовой активности с помощью амплитудного метода.&lt;/b&gt;&lt;br /&gt;В этом режиме для определения голоса будет использована громкость входного сигнала.</translation>
    </message>
    <message>
        <location/>
        <source>Amplitude</source>
        <translation>Амплитуда</translation>
    </message>
    <message>
        <location/>
        <source>Voice &amp;Hold</source>
        <translation>&amp;Голос и Тишина</translation>
    </message>
    <message>
        <location/>
        <source>How long to keep transmitting after silence</source>
        <translation>Как долго передавать после тишины</translation>
    </message>
    <message>
        <source>&lt;b&gt;This selects how long after a perceived stop in speech transmission should continue.&lt;/b&gt;&lt;br /&gt;Set this higher if your voice breaks up when you speak (seen by a rapidly blinking voice icon next to your name).&lt;br /&gt;Only makes sense when used with Voice Activity transmission.</source>
        <translation type="obsolete">&lt;b&gt;Выбирает, как долго после паузы в речи продолжать передачу.&lt;/b&gt;&lt;br /&gt;Установите ее больше, если у вас много пауз в речи (можно увидеть по часто мигающей иконке голоса рядом с вашим именем).&lt;br /&gt;Имеет смысл только когда используется с передачей по активности голоса.</translation>
    </message>
    <message>
        <location/>
        <source>Silence Below</source>
        <translation>Молчание тише, чем</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the trigger values for voice detection.&lt;/b&gt;&lt;br /&gt;Use this together with the Audio Statistics window to manually tune the trigger values for detecting speech. Input values below &quot;Silence Below&quot; always count as silence. Values above &quot;Speech Above&quot; always count as voice. Values in between will count as voice if you&apos;re already talking, but will not trigger a new detection.</source>
        <translation>&lt;b&gt;Здесь устанавливаются триггерные значения для определения речи.&lt;/b&gt;&lt;br /&gt;Используйте эти настройки вместе с окном Аудио Статистики, чтобы вручную настроить триггерные значения для определения речи. Если уровень входного сигнала тише, чем установлено ползунком &quot;Молчание тише, чем&quot;, то этот звук будет считаться тишиной. Если уровень входного сигнала громче, чем установлено ползунком &quot;Речь громче, чем&quot;, то такой звук будет считаться началом речи, и будет включен микрофон. Входной сигнал по уровню находящийся между этими значениями будет считаться как речь, как если бы вы продолжали говорить. Если уровень входного сигнала опустится ниже, чем первый ползунок - микрофон отключится.</translation>
    </message>
    <message>
        <location/>
        <source>Speech Above</source>
        <translation>Речь громче, чем</translation>
    </message>
    <message>
        <location/>
        <source>Signal values above this count as voice</source>
        <translation>Сигнал громче этого уровня будет распознан как речь</translation>
    </message>
    <message>
        <location/>
        <source>Compression</source>
        <translation>Сжатие</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Quality</source>
        <translation>&amp;Качество</translation>
    </message>
    <message>
        <location/>
        <source>Quality of compression (peak bandwidth)</source>
        <translation>Качество сжатия (пиковая передача)</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the quality of compression.&lt;/b&gt;&lt;br /&gt;This determines how much bandwidth Mumble is allowed to use for outgoing audio.</source>
        <translation>&lt;b&gt;Устанавливает качество сжатия.&lt;/b&gt;&lt;br /&gt;Определяет, какую загрузку канала Mumble разрешено использовать для исходящего аудио.</translation>
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
        <translation>Аудио на пакет</translation>
    </message>
    <message>
        <location/>
        <source>How many audio frames to send per packet</source>
        <translation>Как много аудио фрагментов посылать в пакете</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This selects how many audio frames should be put in one packet.&lt;/b&gt;&lt;br /&gt;Increasing this will increase the latency of your voice, but will also reduce bandwidth requirements.</source>
        <translation>&lt;b&gt;Выбирает, сколько аудиофрагментов помещать в пакет.&lt;/b&gt;&lt;br /&gt;Увеличение ее увеличит задержку голоса, но также уменьшит требования к толщине канала связи.</translation>
    </message>
    <message>
        <source>Maximum bandwidth used for sent audio</source>
        <translation type="obsolete">Максимальная скорость, используемая для посылки аудио</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This shows peak outgoing bandwidth used.&lt;/b&gt;&lt;br /&gt;This shows the peak amount of bandwidth sent out from your machine. Audio bitrate is the maximum bitrate (as we use VBR) for the audio data alone. Position is the bitrate used for positional information. Overhead is our framing and the IP packet headers (IP and UDP is 75% of this overhead).</source>
        <translation>&lt;b&gt;Показывает пиковую загрузку посланных сигналов.&lt;/b&gt;&lt;br /&gt;Показывает пиковое значение скорости передачи данных от вашего компьютера. Битрейт аудио - максимальный битрейт (по принятым сокращениям, VBR) только для аудиоданных. Позиция - битрейт, используемый для информации о позиции. Загрузка - раскадровка и заголовки IP пакетов (IP и UDP - 90% этой загрузки). {75%?}.</translation>
    </message>
    <message>
        <location/>
        <source>Audio Processing</source>
        <translation>Обработка звука</translation>
    </message>
    <message>
        <location/>
        <source>Noise Suppression</source>
        <translation>Подавление шума</translation>
    </message>
    <message>
        <location/>
        <source>Noise suppression</source>
        <translation>Подавление шума</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the amount of noise suppression to apply.&lt;/b&gt;&lt;br /&gt;The higher this value, the more aggressively stationary noise will be suppressed.</source>
        <translation>&lt;b&gt;Определяет эффективность шумоподавления.&lt;/b&gt;&lt;br /&gt;Чем выше значение, тем сильнее будет подавляться постоянный шум.</translation>
    </message>
    <message>
        <location/>
        <source>Amplification</source>
        <translation>Усиление</translation>
    </message>
    <message>
        <location/>
        <source>Maximum amplification of input sound</source>
        <translation>Максимальное усиление входящего звука</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Maximum amplification of input.&lt;/b&gt;&lt;br /&gt;Mumble normalizes the input volume before compressing, and this sets how much it&apos;s allowed to amplify.&lt;br /&gt;The actual level is continually updated based on your current speech pattern, but it will never go above the level specified here.&lt;br /&gt;If the &lt;i&gt;Microphone loudness&lt;/i&gt; level of the audio statistics hover around 100%, you probably want to set this to 2.0 or so, but if, like most people, you are unable to reach 100%, set this to something much higher.&lt;br /&gt;Ideally, set it so &lt;i&gt;Microphone Loudness * Amplification Factor &gt;= 100&lt;/i&gt;, even when you&apos;re speaking really soft.&lt;br /&gt;&lt;br /&gt;Note that there is no harm in setting this to maximum, but Mumble will start picking up other conversations if you leave it to auto-tune to that level.</source>
        <translation>&lt;b&gt;Максимальное усиление входного голоса.&lt;/b&gt;&lt;br /&gt;Mumble нормализует входную громкость до сжатия, эта опция устанавливает, на сколько можно его усилить.&lt;br /&gt;Текущий уровень постоянно обновляется на основе текущего образца речи, но никогда не будет выше установленного здесь уровня.&lt;br /&gt;Если уровень &lt;i&gt;Громкости микрофона&lt;/i&gt; аудиостатистики держится на уровне 100%, вы можете установить его на 2.0 или выше, но если, как многие люди, вы не можете достичь 100%, установите его на чуть более высоком уровне.&lt;br /&gt;В идеале, установите его так, чтобы &lt;i&gt;Громкость микрофона * Фактор усиления &gt;= 100&lt;/i&gt;, даже если вы говорите слишком мягко.&lt;br /&gt;&lt;br /&gt;Заметьте, что ничего плохого не случится, если вы установите его на максимум, но Mumble начнет подбирать другие переговоры, если вы оставите его на автоматической настройке на данном уровне.</translation>
    </message>
    <message>
        <location/>
        <source>Current speech detection chance</source>
        <translation>Текущий шанс распознавание речи</translation>
    </message>
    <message>
        <location/>
        <source>Cancel echo from speakers</source>
        <translation>Подавление эхо от колонок</translation>
    </message>
    <message>
        <location/>
        <source>Enabling this will cancel the echo from your speakers. Mixed has low CPU impact, but only works well if your speakers are equally loud and equidistant from the microphone. Multichannel echo cancellation provides much better echo cancellation, but at a higher CPU cost.</source>
        <translation>Включение этой опции позволит подавлять эхо от Ваших колонок. Смешанное подавление эхо меньше нагружает процессор, однако работает хорошо только если Ваши колонки достаточно громкие и находятся на одинаковом расстоянии от микрофона. Мультиканальное работает гораздо эффективнее, но больше нагружает процессор.</translation>
    </message>
    <message>
        <location/>
        <source>Disabled</source>
        <translation>Отключено</translation>
    </message>
    <message>
        <location/>
        <source>Mixed</source>
        <translation>Смешанное</translation>
    </message>
    <message>
        <location/>
        <source>Multichannel</source>
        <translation>Мультиканальное</translation>
    </message>
    <message>
        <location/>
        <source>Echo</source>
        <translation>Эхо</translation>
    </message>
    <message>
        <location/>
        <source>Audible audio cue when push-to-talk is activated or deactivated</source>
        <translation>Звуковой сигнал, когда кнопка включения отпускается</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets speech detection to use Signal to Noise ratio.&lt;/b&gt;&lt;br /&gt;In this mode, the input is analyzed for something resembling a clear signal, and the clarity of that signal is used to trigger speech detection.</source>
        <translation>&lt;b&gt;Здесь устанавливается определения речи методом оценки отношения Сигнал/Шум.&lt;/b&gt;&lt;br /&gt;В этом режиме входной сигнал будет анализирован на наличие &quot;чистого сигнала&quot;. Если входной звук будет распознан как &quot;чистый&quot; (не шум), то будет включен микрофон.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This shows the current speech detection settings.&lt;/b&gt;&lt;br /&gt;You can change the settings from the Settings dialog or from the Audio Wizard.</source>
        <translation>&lt;b&gt;Здесь показаны текущие настройки определения речи.&lt;/b&gt;&lt;br /&gt;Вы можете изменить настройки здесь, или в Мастере настройки звука.</translation>
    </message>
    <message>
        <source>Idle AutoMute</source>
        <translation type="obsolete">Автовыключение</translation>
    </message>
    <message>
        <source>How long to remain idle before auto-mute.</source>
        <translation type="obsolete">Длительность бездействия пользователя до автоприглушения.</translation>
    </message>
    <message>
        <source>This sets an idle timer. If the timer expires without any audio being sent to the server, you are muted.</source>
        <translation type="obsolete">Здесь устанавливается таймер автоотключения. Если не будет входного звука в течение этого времени, то Вы будете заглушены.</translation>
    </message>
    <message>
        <location/>
        <source>Signal values below this count as silence</source>
        <translation>Уровень сигнала ниже этого значение будет считаться как тишина</translation>
    </message>
    <message>
        <location/>
        <source>Maximum bandwidth used for sending audio</source>
        <translation>Максимальная ширина канала, используемая для отправки звука</translation>
    </message>
    <message>
        <location filename="AudioInput.cpp" line="+528"/>
        <source>Server maximum network bandwidth is only %1 kbit/s. Audio quality auto-adjusted to %2 kbit/s (%3ms)</source>
        <translation>Максимальная пропускная способность сервера только %1 кбит/с. Качество звука автоматически настроено на %2 кбит/с (%3мс)</translation>
    </message>
    <message>
        <location filename="AudioInput.ui"/>
        <source>Use Amplitude based speech detection</source>
        <translation>Использовать Амплитудный метод распознавания речи</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This selects how long after a perceived stop in speech transmission should continue.&lt;/b&gt;&lt;br /&gt;Set this higher if your voice breaks up when you speak (seen by a rapidly blinking voice icon next to your name).</source>
        <translation>&lt;b&gt;Здесь устанавливается, как долго после прекращения разговора будет включен микрофон.&lt;/b&gt;&lt;br /&gt;Установить это значение выше, если Ваш голос часто прерывается во время разговора (видно по часто мерцающей иконке рядом с Вашим ником).</translation>
    </message>
    <message>
        <location/>
        <source>Idle AutoDeafen</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>How long to remain idle before auto-deafen.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This sets an idle timer. If the timer expires without any audio being sent to the server, you are muted and deafened.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>AudioInputDialog</name>
    <message>
        <location filename="AudioConfigDialog.cpp" line="+74"/>
        <source>Continuous</source>
        <translation>Постоянно</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Voice Activity</source>
        <translation>При голосовой активности</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Push To Talk</source>
        <translation>При нажатии клавиши</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Audio Input</source>
        <translation>Ввод звука</translation>
    </message>
    <message>
        <location line="+96"/>
        <location line="+8"/>
        <source>%1 ms</source>
        <translation>%1 мс</translation>
    </message>
    <message>
        <location line="-2"/>
        <location line="+20"/>
        <location line="+18"/>
        <source>Off</source>
        <translation>Отключение</translation>
    </message>
    <message>
        <location line="-30"/>
        <source>%1 s</source>
        <translation>%1 с</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>%1 kb/s</source>
        <translation>%1 кб/с</translation>
    </message>
    <message>
        <location line="+11"/>
        <source>-%1 dB</source>
        <translation>-%1 дБ</translation>
    </message>
    <message>
        <location line="+55"/>
        <source>%1 kbit/s (Audio %2 %5, Position %4, Overhead %3)</source>
        <oldsource>%1kbit/s (Audio %2 %5, Position %4, Overhead %3)</oldsource>
        <translation>%1 кбит/с (Аудио %2 %5, Позиция %4, Излишки %3)</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>CELT</source>
        <translation>CELT</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Speex</source>
        <translation>Speex</translation>
    </message>
    <message>
        <source>%1kbit/s (Audio %2, Position %4, Overhead %3)</source>
        <translation type="obsolete">%1кбит/с (Аудио %2, Позиция %4, Загрузка %3)</translation>
    </message>
    <message>
        <location line="-42"/>
        <source>%1 min</source>
        <translation>%1 мин</translation>
    </message>
</context>
<context>
    <name>AudioOutput</name>
    <message>
        <location filename="AudioOutput.ui"/>
        <source>Form</source>
        <translation>Шрифт</translation>
    </message>
    <message>
        <location/>
        <source>Interface</source>
        <translation>Интерфейс</translation>
    </message>
    <message>
        <location/>
        <source>System</source>
        <translation>Система</translation>
    </message>
    <message>
        <location/>
        <source>Output method for audio</source>
        <oldsource>Input method for audio</oldsource>
        <translation>Аудио система вывода</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is the output method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <oldsource>&lt;b&gt;This is the input method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</oldsource>
        <translation>&lt;b&gt;Этот метод ввода используется для аудио.&lt;/b&gt;&lt;br /&gt;Скорее всего вы захотите использовать DirectSound.</translation>
    </message>
    <message>
        <location/>
        <source>Device</source>
        <translation>Устройство</translation>
    </message>
    <message>
        <location/>
        <source>Output device for audio</source>
        <translation>Аудио устройство вывода</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is the output device to use for audio.&lt;/b&gt;</source>
        <translation>Это аудио устройство вывода</translation>
    </message>
    <message>
        <location/>
        <source>Positional Audio</source>
        <translation>Позиционированное аудио</translation>
    </message>
    <message>
        <location/>
        <source>Audio Output</source>
        <translation>Вывод звука</translation>
    </message>
    <message>
        <location/>
        <source>Default &amp;Jitter Buffer</source>
        <translation>Стандартный  &amp;буфер</translation>
    </message>
    <message>
        <location/>
        <source>Safety margin for jitter buffer</source>
        <translation>Безопасный размер буфера</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the minimum safety margin for the jitter buffer.&lt;/b&gt;&lt;br /&gt;All incoming audio is buffered, and the jitter buffer continually tries to push the buffer to the minimum sustainable by your network, so latency can be as low as possible. This sets the minimum buffer size to use. If the start of sentences you hear is very jittery, increase this value.</source>
        <translation>&lt;b&gt;Здесь устанавливается минимальный безопасный размер аудио буфера.&lt;/b&gt;&lt;br /&gt;Все входящее аудио сначала буферизируется, и аудио буфер продолжает откладывать данные в буфер до минимального количества, которое может выдержать Ваша сеть, чтобы задержка была минимальной. Здесь задается минимальный размер буфера. Если речь, которую Вы слышите, будет трещать и прерываться, увеличьте это значение.</translation>
    </message>
    <message>
        <location/>
        <source>Minimum distance to user before sound volume decreases</source>
        <translation>Минимальное расстояние до пользователя, после которого громкость уменьшится</translation>
    </message>
    <message>
        <location/>
        <source>This sets the minimum distance for sound calculations. The volume of other users&apos; speech will not decrease until they are at least this far away from you.</source>
        <translation>Это минимальная дистанция для рассчета звука. Громкость речи других пользователей не уменьшится, до тех пор, пока они ближе, чем это значение от Вас.</translation>
    </message>
    <message>
        <location/>
        <source>This sets the maximum distance for sound calculations. When farther away than this, other users&apos; speech volume will not decrease any further.</source>
        <translation>Это максимальная дистанция для рассчета звука. Когда другие пользователи находятся дальше этого расстояния от Вас, громкость больше не уменьшится.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This enables one of the loopback test modes.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;None&lt;/i&gt; - Loopback disabled&lt;br /&gt;&lt;i&gt;Local&lt;/i&gt; - Emulate a local server.&lt;br /&gt;&lt;i&gt;Server&lt;/i&gt; - Request loopback from server.&lt;br /&gt;Please note than when loopback is enabled, no other users will hear your voice. This setting is not saved on application exit.</source>
        <translation>&lt;b&gt;Здесь включается один из тестов обратной связи.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;Нет&lt;/i&gt; - Обратная связь отлючена&lt;br /&gt;&lt;i&gt;Локальный&lt;/i&gt; - Эмуляция локального сервера.&lt;br /&gt;&lt;i&gt;Сервер&lt;/i&gt; - Запрос обратной связи с сервера.&lt;br /&gt;Когда включен тест Обратной связи, другие пользователи не смогут Вас слышать. Эта опция не сохраняется при выходе из приложения.</translation>
    </message>
    <message>
        <location/>
        <source>Volume</source>
        <translation>Громкость</translation>
    </message>
    <message>
        <location/>
        <source>Volume of incoming speech</source>
        <translation>Громкость входящей речи</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This adjusts the volume of incoming speech.&lt;/b&gt;&lt;br /&gt;Note that if you increase this beyond 100%, audio will be distorted.</source>
        <translation>&lt;b&gt;Здесь настраивается громкость входящей речи.&lt;/b&gt;&lt;br /&gt;Если увеличить более 100%, звук будет искажен.</translation>
    </message>
    <message>
        <location/>
        <source>Output Delay</source>
        <translation>Задержка вывода</translation>
    </message>
    <message>
        <location/>
        <source>Amount of data to buffer</source>
        <translation>Количество данных, которое будет буферизироваться</translation>
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
        <translation>Фактор уменьшения громкости звука</translation>
    </message>
    <message>
        <source>How fast should sound volume drop when passing beyond the minimum distance. The normal (1.0) is that sound volume halves each time the distance doubles. Increasing this value means sound volume drops faster, while decreasing it means it drops slower.</source>
        <translation type="obsolete">Как быстро будет падать громкость звука после достижения определеной дистанции. Обычно (1.0) - громкость уменьшается вполовину при удвоении расстояния. Увеличение этого значения обозначает, что громкость падает быстрее, уменьшение - замедление падения.</translation>
    </message>
    <message>
        <location/>
        <source>Bloom</source>
        <translation>Пик</translation>
    </message>
    <message>
        <location/>
        <source>Loopback Test</source>
        <translation>Тест обратной связи</translation>
    </message>
    <message>
        <location/>
        <source>Delay Variance</source>
        <translation>Разброс задержек</translation>
    </message>
    <message>
        <location/>
        <source>Variance in packet latency</source>
        <translation>Разброс задержек пакетов</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the packet latency variance for loopback testing.&lt;/b&gt;&lt;br /&gt;Most audio paths contain some variable latency. This allows you to set that variance for loopback mode testing. For example, if you set this to 15ms, this will emulate a network with 20-35ms ping latency or one with 80-95ms latency. Most domestic net connections have a variance of about 5ms.</source>
        <oldsource>&lt;b&gt;This sets the packet latency variance for loopback testing.&lt;/b&gt;&lt;br /&gt;Most audio paths contain some variable latency. This allows you set that variance for loopback mode testing. For example, if you set this to 15ms, this will emulate a network with 20-35ms ping latency or one with 80-95ms latency. Most domestic net connections have a variance of about 5ms.</oldsource>
        <translation>&lt;b&gt;Здесь устанавливается разброс задержек передачи пакетов для теста обратной связи.&lt;/b&gt;&lt;br /&gt;Большинство аудио потоков имеют некоторый разброс в величине задержек. Здесь можно установить разброс задержек для теста обратной связи. Например, если вы установите разброс 15мс, это будет эмулировать сеть с 20-35мс пингом, или с задержками 80-95мс. Большинство домашних сетей имеют разброс около 5мс.</translation>
    </message>
    <message>
        <location/>
        <source>Packet Loss</source>
        <translation>Потери пакетов</translation>
    </message>
    <message>
        <location/>
        <source>Packet loss for loopback mode</source>
        <translation>Потери пакетов для теста обратной связи</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the packet loss for loopback mode.&lt;/b&gt;&lt;br /&gt;This will be the ratio of packets lost. Unless your outgoing bandwidth is peaked or there&apos;s something wrong with your network connection, this will be 0%</source>
        <translation>&lt;b&gt;Здесь устанавливаются потери для теста обратной связи.&lt;/b&gt;&lt;br /&gt;Это отношение потерянных пактов. Если у Вас все в порядке с соединением, то это будет 0%</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Loopback</source>
        <translation>&amp;Обратная связь</translation>
    </message>
    <message>
        <location/>
        <source>Desired loopback mode</source>
        <translation>Желаемый режим обратной связи</translation>
    </message>
    <message>
        <location/>
        <source>Factor for sound volume increase</source>
        <translation>Фактор увеличения громкости звука</translation>
    </message>
    <message>
        <location/>
        <source>The connected &quot;speakers&quot; are actually headphones</source>
        <oldsource>The connected &quot;speakers&quot; are actually headphones.</oldsource>
        <translation>Подключенные &quot;динамики&quot; на самом деле - наушники</translation>
    </message>
    <message>
        <location/>
        <source>Minimum Distance</source>
        <translation>Мин. дистанция</translation>
    </message>
    <message>
        <location/>
        <source>Maximum Distance</source>
        <translation>Макс. дистанция</translation>
    </message>
    <message>
        <location/>
        <source>Maximum distance, beyond which speech volume won&apos;t decrease</source>
        <translation>Максимальная дистанция, после которой громкость не уменьшится</translation>
    </message>
    <message>
        <location/>
        <source>Minimum Volume</source>
        <translation>Мин. громкость</translation>
    </message>
    <message>
        <location/>
        <source>What should the volume be at the maximum distance?</source>
        <translation>Какая должна быть громкость на максимальной дистанции?</translation>
    </message>
    <message>
        <location/>
        <source>How much should sound volume increase for sources that are really close?</source>
        <translation>Какое должно быть увеличение громкости при максимальной близости от игрока?</translation>
    </message>
    <message>
        <location/>
        <source>Checking this indicates that you don&apos;t have speakers connected, just headphones. This is important, as speakers are usually in front of you, while headphones are directly to your left/right.</source>
        <translation>Отметьте здесь, если у Вас подключены наушники, иначе позиционирование будет работать не очень корректно.</translation>
    </message>
    <message>
        <location/>
        <source>Headphones</source>
        <translation>Наушники</translation>
    </message>
    <message>
        <location/>
        <source>This sets the amount of data to pre-buffer in the output buffer. Experiment with different values and set it to the lowest which doesn&apos;t cause rapid jitter in the sound.</source>
        <translation>Здесь задается количество данных для буферизации звука. Измените так, чтобы значение было минимальным, но при этом не было прерываний звука.</translation>
    </message>
    <message>
        <location/>
        <source>Attenuate applications by...</source>
        <translation>Приглушать приложения на...</translation>
    </message>
    <message>
        <location/>
        <source>Attenuation of other applications during speech</source>
        <translation>Приглушение других приложений во время разговора</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Attenuate volume of other applications during speech&lt;/b&gt;&lt;br /&gt;Mumble supports decreasing the volume of other applications during incoming and/or outgoing speech. This sets the attenuation of other applications if the feature is enabled.</source>
        <translation>&lt;b&gt;Приглушение других приложений во время разговора&lt;/b&gt;&lt;br /&gt;Mumble поддерживает уменьшение громкости других приложений во время входящей или исходящей речи.</translation>
    </message>
    <message>
        <location/>
        <source>If checked Mumble lowers the volume of other applications while other users talk</source>
        <translation>Если отмечено, то Мамбл уменьшит громкость других приложений, когда другие пользователи говорят</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Attenuate applications while other users talk&lt;/b&gt;&lt;br /&gt;Mumble supports decreasing the volume of other applications during incoming and/or outgoing speech. This makes mumble activate the feature while other users talk to you.</source>
        <translation>&lt;b&gt;Приглушать другие приложения, когда другие пользователи говорят&lt;/b&gt;&lt;br /&gt;Mumble поддерживает уменьшение громкости других приложений во время входящей или исходящей речи. Этот флажок активирует функцию, когда другие пользователи говорят.</translation>
    </message>
    <message>
        <location/>
        <source>while other users talk</source>
        <translation>когда говорят</translation>
    </message>
    <message>
        <location/>
        <source>If checked Mumble lowers the volume of other applications while you talk</source>
        <translation>Если отмечено - мамбл уменьшит громкость других приложений, когда Вы говорите</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Attenuate applications while you talk&lt;/b&gt;&lt;br /&gt;Mumble supports decreasing the volume of other applications during incoming and/or outgoing speech. This makes mumble activate the feature while you talk.</source>
        <translation>&lt;b&gt;Приглушать приложения, когда Вы говорите&lt;/b&gt;&lt;br /&gt;Mumble поддерживает уменьшение громкости других приложений во время входящей или исходящей речи. Этот флажок активирует функцию приглушения, когда говорите Вы.</translation>
    </message>
    <message>
        <location/>
        <source>while you talk</source>
        <translation>когда Вы говорите</translation>
    </message>
</context>
<context>
    <name>AudioOutputDialog</name>
    <message>
        <location filename="AudioConfigDialog.cpp" line="+167"/>
        <source>None</source>
        <translation>Нет</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Local</source>
        <translation>Локальный</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Server</source>
        <translation>Сервер</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Audio Output</source>
        <translation>Вывод звука</translation>
    </message>
    <message>
        <location line="+102"/>
        <location line="+19"/>
        <location line="+8"/>
        <source>%1 ms</source>
        <translation>%1 мс</translation>
    </message>
    <message>
        <location line="-16"/>
        <location line="+4"/>
        <location line="+8"/>
        <location line="+31"/>
        <location line="+4"/>
        <source>%1 %</source>
        <translation>%1 %</translation>
    </message>
    <message>
        <location line="-16"/>
        <location line="+6"/>
        <source>%1 m</source>
        <translation>%1 м</translation>
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
        <translation>Выберите звуковой файл</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Invalid sound file</source>
        <translation>Неверный звуковой файл</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The file &apos;%1&apos; cannot be used by Mumble. Please select a file with a compatible format and encoding.</source>
        <translation>Файл &apos;%1&apos; не может быть использован в Mumble. Пожалуйста, выберите файл совместимого формата и кодировки.</translation>
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
        <translation>Показывает пиковую мощность в последнем фрагменте (20 мс) динамиков. Пока вы используете мультиканальный метод проб (такой как ASIO),настроенный на каналы динамика, она будет равна 0. Если у вас они так сконфигурированы, все еще будет отображаться 0, пока вы будете прослушивать звук из других программ, ваши настройки не будут работать.</translation>
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
        <translation>Показывает пиковую мощность в последнем фрейме (20 мс) после всей обработки. В идеале здесь должно быть - 96 дБ, когда Вы не говорите. В реальной жизни в студии будет около 60 дБ, а Вы увидите в лучшем случае 20 дБ. Когда Вы говорите этот показатель будет увеличиваться в районе 5 - 10 дБ.&lt;br /&gt;Если Вы используете подавление эхо, и это значение повышается более 15 дБ, когда Вы говорите - Ваши настройки не работают, и Вы будете раздражать других пользователей эхом.</translation>
    </message>
    <message>
        <location/>
        <source>Signal-To-Noise ratio from the microphone</source>
        <translation>Соотношение Сигнал-шум микрофона</translation>
    </message>
    <message>
        <location/>
        <source>This is the Signal-To-Noise Ratio (SNR) of the microphone in the last frame (20 ms). It shows how much clearer the voice is compared to the noise.&lt;br /&gt;If this value is below 1.0, there&apos;s more noise than voice in the signal, and so quality is reduced.&lt;br /&gt;There is no upper limit to this value, but don&apos;t expect to see much above 40-50 without a sound studio.</source>
        <translation>Отношение Сигнал-шум (SNR) микрофона в последнем фрагменте (20 мс). Показывает, насколько чист голос по сравнению с шумом.&lt;br /&gt;Если значение ниже 1.0, в сигнале больше шума, нежели голоса, и поэтому ухудшается качество.&lt;br /&gt;Верхнего предела не существует, но не ожидайте увидеть выше 40-50, если Вы не в студии звукозаписи.</translation>
    </message>
    <message>
        <location/>
        <source>Probability of speech</source>
        <translation>Вероятность того, что во входящем звуке есть речь</translation>
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
        <translation>Аудио битрейт последнего сжатого фрагмента (20 мс), и поэтому это значение будет варьироваться, как VBR. Чтобы настроить пиковый битрейт, настройте &lt;b&gt;Сложность сжатия&lt;/b&gt; в диалоге настроек.</translation>
    </message>
    <message>
        <location/>
        <source>Time between last two Push-To-Talk presses</source>
        <translation>Время между последних двух нажатий Push-To-Talk</translation>
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
        <translation>Отображает значение подавителя эха, со временем растущего вниз и увеличением частоты вправо.&lt;br /&gt;В идеале, должен быть черным, отображая, что эха нет. В общем, у вас будут 1 или несколько горизонтальных полосок синеватого цвета, отображая время задержки эха. Вы должны увидеть значения в реальном времени.&lt;br /&gt;Заметьте пожалуйста, что если у Вас нет эхо, которое нужно подавить, вы не увидите здесь полезной информации. Запустите музыку, и все должно нормализоваться. &lt;br /&gt;Вы можете выбрать просмотр реальных или воображаемых частей доменно-частотных значений, или наоборот высчитанные модуль и фазу. Наиболее применимым из них будут, скорее всего модуль, который является амплитудой эха, и показывает, сколько сигнала было вырезано за промежуток времени. Другие режимы просмотра более полезны для людей, желающих настроить алгоритмы глушения эха.&lt;br /&gt;Пожалуйста, заметьте: Если картина в целом обширно варьируется в режиме модулей, подавитель эха не может найти никакого соответствия между двумя входными источниками (динамиками и микрофоном). Или у вас очень долгая задержка эха, или один из входных источников настроен неправильно.</translation>
    </message>
    <message>
        <source>Mumble</source>
        <translation type="obsolete">Mumble</translation>
    </message>
    <message>
        <location filename="AudioStats.cpp" line="+405"/>
        <source>&gt;1000 ms</source>
        <oldsource>&gt;1000ms</oldsource>
        <translation>&gt;1000 мс</translation>
    </message>
    <message>
        <location filename="AudioStats.ui"/>
        <source>Audio Statistics</source>
        <translation>Аудио Статистика</translation>
    </message>
    <message>
        <location/>
        <source>Input Levels</source>
        <translation>Входные уровни</translation>
    </message>
    <message>
        <location/>
        <source>This shows the peak power in the last frame (20 ms), and is the same measurement as you would usually find displayed as &quot;input power&quot;. Please disregard this and look at &lt;b&gt;Microphone power&lt;/b&gt; instead, which is much more steady and disregards outliers.</source>
        <translation>Показывает пиковую мощность последнего фрагмента (20 мс), и является тем же параметром, который Вы обычно увидите как &quot;входная мощность&quot;. Проигнорируйте этот параметр в пользу &lt;b&gt;Мощности микрофона&lt;/b&gt;, который является более корректным.</translation>
    </message>
    <message>
        <location/>
        <source>Signal Analysis</source>
        <translation>Анализ Сигнала</translation>
    </message>
    <message>
        <location/>
        <source>Microphone power</source>
        <translation>Мощность микрофона</translation>
    </message>
    <message>
        <location/>
        <source>This shows how close your current input volume is to the ideal. To adjust your microphone level, open whatever program you use to adjust the recording volume, and look at the value here while talking.&lt;br /&gt;&lt;b&gt;Talk loud, as you would when you&apos;re upset over getting fragged by a noob.&lt;/b&gt;&lt;br /&gt;Adjust the volume until this value is close to 100%, but make sure it doesn&apos;t go above. If it does go above, you are likely to get clipping in parts of your speech, which will degrade sound quality.</source>
        <translation>Показывает насколько близок Ваш текущий уровень входной громкости к идеалу. Чтобы настроить громкость микрофона, откройте любую программу настройки громкости, и настройте громкость записи, затем посмотрите это значение во время разговора. &lt;br /&gt;&lt;b&gt;Говорите громко, как будто расстроены или раздражены действиями нуба.&lt;/b&gt;&lt;br /&gt;Настройте громкость, до тех пор, пока число не будет близко к 100%, но убедитесь, что оно не поднимется выше. Если оно будет выше, скорее всего будут прерывания Вашей речи, что ухудшит качество звука.</translation>
    </message>
    <message>
        <location/>
        <source>Signal-To-Noise ratio</source>
        <translation>Отношение Сигнал/Шум</translation>
    </message>
    <message>
        <location/>
        <source>Configuration feedback</source>
        <translation>Информаци о настройках</translation>
    </message>
    <message>
        <location/>
        <source>Current audio bitrate</source>
        <translation>Текущий битрейт</translation>
    </message>
    <message>
        <location/>
        <source>DoublePush interval</source>
        <translation>Скорость двойного нажатия</translation>
    </message>
    <message>
        <location/>
        <source>Speech Detection</source>
        <translation>Распознавание речи</translation>
    </message>
    <message>
        <location/>
        <source>Current speech detection chance</source>
        <translation>Текущий шанс распознавания речи</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This shows the current speech detection settings.&lt;/b&gt;&lt;br /&gt;You can change the settings from the Settings dialog or from the Audio Wizard.</source>
        <translation>&lt;b&gt;Здесь показаны текущие настройки распознавания речи.&lt;/b&gt;&lt;br /&gt;Вы можете изменить эти настройки в Настройках или из Мастера настройки звука.</translation>
    </message>
    <message>
        <location/>
        <source>Signal and noise power spectrum</source>
        <translation>Спектр мощности сигнала и шума</translation>
    </message>
    <message>
        <location/>
        <source>Echo Analysis</source>
        <translation>Анализ Эхо</translation>
    </message>
</context>
<context>
    <name>AudioWizard</name>
    <message>
        <location filename="AudioWizard.ui"/>
        <source>Audio Tuning Wizard</source>
        <translation>Мастер настройки звука</translation>
    </message>
    <message>
        <location/>
        <source>Introduction</source>
        <translation>Вступление</translation>
    </message>
    <message>
        <location/>
        <source>Welcome to the Mumble Audio Wizard</source>
        <translation>Добро пожаловать в мастер настройки звука Mumble</translation>
    </message>
    <message>
        <location/>
        <source>Finished</source>
        <translation>Заврешено</translation>
    </message>
    <message>
        <location/>
        <source>Enjoy using Mumble</source>
        <translation>Наслаждайтесь Mumble</translation>
    </message>
    <message>
        <location/>
        <source>Device selection</source>
        <translation>Выбор устройства</translation>
    </message>
    <message>
        <location/>
        <source>Selecting the input and output device to use with Mumble.</source>
        <translation>Выбор устройств ввода-вывода звука, которые будут использоваться  Mumble.</translation>
    </message>
    <message>
        <location/>
        <source>This is the device your microphone is connected to.</source>
        <translation>Это устройство, к которому подключен Ваш микрофон.</translation>
    </message>
    <message>
        <location/>
        <source>System</source>
        <translation>Система</translation>
    </message>
    <message>
        <location/>
        <source>Input method for audio</source>
        <translation>Метод ввода для аудио</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is the input method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation>&lt;b&gt;Этот метод ввода используется для аудио.&lt;/b&gt;&lt;br /&gt;Скорее всего вы захотите использовать DirectSound.</translation>
    </message>
    <message>
        <location/>
        <source>Device</source>
        <translation>Устройство</translation>
    </message>
    <message>
        <location/>
        <source>Input device to use</source>
        <translation>Устройство ввода звука</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Selects which sound card to use for audio input.&lt;/b&gt;</source>
        <translation>&lt;b&gt;Здесь можно выбрать звуковую карту для ввода звука.&lt;/b&gt;</translation>
    </message>
    <message>
        <location/>
        <source>This is the device your speakers or headphones are connected to.</source>
        <translation>Это устройство, к которому подключены Ваши колонки или наушники.</translation>
    </message>
    <message>
        <location/>
        <source>Output method for audio</source>
        <translation>Мытод вывода для аудио</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is the Output method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation>&lt;b&gt;Это метод вывода звука.&lt;/b&gt;&lt;br /&gt;Скорее всего вы захотите использовать DirectSound.</translation>
    </message>
    <message>
        <location/>
        <source>Output device to use</source>
        <translation>Устройство вывода звука</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Selects which sound card to use for audio Output.&lt;/b&gt;</source>
        <translation>&lt;b&gt;Здесь можно выбрать звуковую карту для вывода звука.&lt;/b&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Volume tuning</source>
        <translation>Настройка громкости</translation>
    </message>
    <message>
        <location/>
        <source>Tuning microphone hardware volume to optimal settings.</source>
        <translation>Настройка громкости микрофона.</translation>
    </message>
    <message>
        <location/>
        <source>Now talk softly, as you would when talking late at night and you don&apos;t want to disturb anyone. Adjust the slider below so that the bar moves into green when you talk, but stays blue while you&apos;re silent.</source>
        <translation>Теперь говорите тихо, как если бы вы говорили поздно ночью, чтобы никого не разбудить. Переместите ползунок так, чтобы полоска вверху переходила в зеленую область когда вы говорите, а когда вы молчите не выходила за пределы синей области.</translation>
    </message>
    <message>
        <location/>
        <source>Voice Activity Detection</source>
        <translation>Определение голосовой активности</translation>
    </message>
    <message>
        <location/>
        <source>Letting Mumble figure out when you&apos;re talking and when you&apos;re silent.</source>
        <translation>Позволяет Mumble определять, когда Вы говорите, а когда молчите.</translation>
    </message>
    <message>
        <location/>
        <source>This will help Mumble figure out when you are talking. The first step is selecting which data value to use.</source>
        <translation>Это поможет Mumble определить, когда Вы говорите. Сначала выберите, какой метод определения голосовой активности будет использован.</translation>
    </message>
    <message>
        <location/>
        <source>Raw amplitude from input</source>
        <translation>Амплитуда входного звука</translation>
    </message>
    <message>
        <location/>
        <source>Signal-To-Noise ratio</source>
        <translation>Отношение сигнал-шум</translation>
    </message>
    <message>
        <location/>
        <source>Next you need to adjust the following slider. The first few utterances you say should end up in the green area (definitive speech). While talking, you should stay inside the yellow (might be speech) and when you&apos;re not talking, everything should be in the red (definitively not speech).</source>
        <oldsource>Next you need to adjust the following two sliders. The first few utterances you say should end up in the green area (definitive speech). While talking, you should stay inside the yellow (might be speech) and when you&apos;re not talking, everything should be in the red (definitively not speech).</oldsource>
        <translation>Переместите ползунок так, чтобы первые реплики, которые вы произнесете, заканчивались в зеленой области (будут определены Mumble как речь), во время разговора полоска должна оставаться в желтой области (будет определено как возможная речь), а когда вы молчите, полоска должна оставаться в красной области.</translation>
    </message>
    <message>
        <location/>
        <source>Device tuning</source>
        <translation>Настройка устройств</translation>
    </message>
    <message>
        <location/>
        <source>Changing hardware output delays to their minimum value.</source>
        <translation>Уменьшение задержек вывода звука.</translation>
    </message>
    <message>
        <source>%1ms</source>
        <translation type="obsolete">%1мс</translation>
    </message>
    <message>
        <location/>
        <source>Use echo cancellation</source>
        <translation>Использовать подавление эха</translation>
    </message>
    <message>
        <location/>
        <source>This enables echo cancellation of outgoing audio, which helps both on speakers and on headsets.</source>
        <translation>Включает подавление эхо исходящего звука от колонок или наушников.</translation>
    </message>
    <message>
        <location/>
        <source>Enable positional audio</source>
        <translation>Включить позиционирование звука</translation>
    </message>
    <message>
        <location/>
        <source>Allows positioning of sound</source>
        <oldsource>Allows positioning of sound.</oldsource>
        <translation>Позволяет позиционировать звук</translation>
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
Вас приветствует мастер настройки звука Mumble. Это средство поможет вам правильно настроить уровни громкости вашего микрофона, а также настроить параметры обработки звука в Mumble.
&lt;/p&gt;
&lt;p&gt;
Имейте ввиду, что во время работы мастера, звук будет перенаправлен из микрофона в наушники или колонки, чтобы вы могли слышать его. При этом звук НЕ БУДЕТ передаваться на сервер, к которому вы подключены.
&lt;/p&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Input Device</source>
        <translation>Устройство ввода</translation>
    </message>
    <message>
        <location/>
        <source>Cancel echo from headset or speakers</source>
        <translation>Подавление эха от наушников или колонок</translation>
    </message>
    <message>
        <location/>
        <source>Output Device</source>
        <translation>Устройство вывода</translation>
    </message>
    <message>
        <location/>
        <source>This allows Mumble to use positional audio to place voices.</source>
        <translation>Это позволяет Mumble позиционировать голоса говорящих, в зависимости от их расположения в игре.</translation>
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
Чтобы уменьшить задржки до минимума, необходимо максимально уменьшить аудио буфер звуковой карты. Тем не менее многие звуковые карты сообщают системе ошибочный размер аудио буфера, поэтому этот параметр необходимо устанавить вручную.
&lt;/p&gt;
&lt;p&gt;
Сейчас вы должны слышать голос диктора. Передвиньте ползунок как можно левее, но так, чтобы при этом вы не слышали щелчков и прерываний звука. Звук из вашего микрофона отключен во время этого теста.
&lt;/p&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Amount of data to buffer</source>
        <translation>Размер аудио буфера</translation>
    </message>
    <message>
        <location/>
        <source>This sets the amount of data to pre-buffer in the output buffer. Experiment with different values and set it to the lowest which doesn&apos;t cause rapid jitter in the sound.</source>
        <translation>Здесь можно задать аудио буфер для исходящего звука. Необходимо установить минимальное значение, при котором не будет прерываний звука.</translation>
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
В панели управления звуком откройте настройки записи. Убедитесь, что микрофон выбран и установлена максимальная громокость записи. Если имеется такая настройка, как &quot;Усиление микрофона&quot;, убедитесь, что она включена.
&lt;/p&gt;
&lt;p&gt;
Говорите громко, как когда Вы раздражены или взволнованы. Уменьшите громкость в звуковой панели так, чтобы при такой громкости речи, полоска внизу оставалась в синей и зеленой области, но при этом &lt;b&gt;не заходила&lt;/b&gt; на красную область.
&lt;/p&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Positional Audio</source>
        <translation>Позиционированное аудио</translation>
    </message>
    <message>
        <location/>
        <source>Adjusting attenuation of positional audio.</source>
        <translation>Настройка затухания позиционированного звука.</translation>
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
Мамбл поддерживает позиционирование звука для некоторых игр, и будет позиционировать голос говорящего относительно его положения в игре. В зависимости от положения в игре говорящего, будет настроено распределение звука по колонкам и громкость так, чтобы имитировать направление и расстояние до говорящего. Это позиционирование зависит от правильной конфигурации колонок в Вашей системе. Здесь небольшой тест для проверки настроек.
&lt;/p&gt;
&lt;p&gt;
Схема внизу показывает расположение &lt;font color=&quot;red&quot;&gt;Вас&lt;/font&gt;, &lt;font color=&quot;yellow&quot;&gt;колонок&lt;/font&gt; и &lt;font color=&quot;green&quot;&gt;движущегося источника звука&lt;/font&gt;. Вы должны слышать перемещение звука по колонкам.
&lt;/p&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Use headphones instead of speakers</source>
        <translation>Использовать наушники вместо колонок</translation>
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
Поздравляем! Все готово для использования Mumble.
&lt;/p&gt;
&lt;p&gt;
Mumble находится на стадии активной разработки. Команда разработчиков стремится добавлять функционал, который пригодится большинству пользователям. Для этого Mumble поддерживает отправку анонимной статистики о Ваших настройках разработчикам. Эта статистика необходима для дальнейшей разработки Мамбл.
&lt;/p&gt;
</translation>
    </message>
    <message>
        <location/>
        <source>Use headphones</source>
        <translation>Использовать наушники</translation>
    </message>
    <message>
        <location/>
        <source>This ignores the OS speaker configuration and configures the positioning for headphones instead.</source>
        <translation>Эта настройка игнорирует настройку колонок в системе и настраивает позиционирование для наушников.</translation>
    </message>
    <message>
        <location/>
        <source>Submit anonymous statistics to the Mumble project</source>
        <translation>Отправлять анонимную статистику в проект Mumble</translation>
    </message>
    <message>
        <location/>
        <source>Push To Talk:</source>
        <translation>Нажми-и-говори (PTT):</translation>
    </message>
    <message>
        <location/>
        <source>Quality &amp; Notifications</source>
        <translation>Качество звука и уведомления</translation>
    </message>
    <message>
        <location/>
        <source>Adjust quality and notification settings.</source>
        <translation>Настройка качества звука и уведомлений.</translation>
    </message>
    <message>
        <location/>
        <source>Quality settings</source>
        <translation>Настройки качества</translation>
    </message>
    <message>
        <location/>
        <source>Low</source>
        <translation>Низкое качество</translation>
    </message>
    <message>
        <location/>
        <source>Balanced</source>
        <translation>Сбалансированное</translation>
    </message>
    <message>
        <location/>
        <source>Notification settings</source>
        <translation>Настройки уведомлений</translation>
    </message>
    <message>
        <location/>
        <source>Use Text-To-Speech to read notifications and messages to you.</source>
        <translation>Использовать текст-в-речь для чтения уведомлений и сообщений механическим голосом.</translation>
    </message>
    <message>
        <location/>
        <source>Disable Text-To-Speech and use sounds instead.</source>
        <translation>Отключить текст-в-речь и использовать звуки вместо этого.</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="+304"/>
        <source>%1 ms</source>
        <translation>%1 мс</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui"/>
        <source>Enables attenuation of other applications while users talk to you</source>
        <translation>Включить приглушение других приложений, когда кто-то говорит с вами</translation>
    </message>
    <message>
        <location/>
        <source>Attenuate applications while other users talk</source>
        <translation>Приглушать звук в других приложениях, когда кто-то говорит</translation>
    </message>
    <message>
        <location/>
        <source>In this configuration Mumble will use a &lt;b&gt;low amount of bandwidth&lt;/b&gt;. This will inevitably result in high latency and poor quality. Choose this only if your connection cannot handle the other settings. (Speex 16kbit/s, 60ms per packet)</source>
        <translation>В этой конфигурации мамбл будет &lt;b&gt;сильно ограничивать&lt;/b&gt; использование траффика. Это неизбежно приведет к бОльшим задершкам и плохому качеству звука. Используйте только если ваше соединение не позволяет использовать другие настройки (Speex 16kбит/с, 60мс на пакет)</translation>
    </message>
    <message>
        <location/>
        <source>This is the &lt;b&gt;recommended default&lt;/b&gt; configuration. It provides a good balance between quality, latency, and bandwidth usage. (CELT 40kbit/s, 20ms per packet)</source>
        <translation>Это &lt;b&gt;рекомендуемая стандартная&lt;/b&gt; конфигурация. Обеспечивает баланс между качеством, задежками и использованию полосы пропускания Вашего соединения. (CELT 40kбит/с, 20мс на пакет)</translation>
    </message>
    <message>
        <location/>
        <source>High</source>
        <translation>Высокое качество</translation>
    </message>
    <message>
        <location/>
        <source>This configuration is only recommended for use in setups where bandwidth is not an issue, like a LAN. It provides the lowest latency supported by Mumble and &lt;b&gt;high quality&lt;/b&gt;. (CELT 72kbit/s, 10ms per packet)</source>
        <translation>Эта конфигурация рекомендуется только в случае, если полоса пропускания Вашего соединения не имеет значения (например локальная сеть). Обеспечивает минимальные задержки и &lt;b&gt;очень высокое качество&lt;/b&gt;. (CELT 72кбит/с, 10мс на пакет)</translation>
    </message>
    <message>
        <location/>
        <source>Custom</source>
        <translation>Другое</translation>
    </message>
    <message>
        <location/>
        <source>You already set a customized quality configuration in Mumble. Select this setting to keep it.</source>
        <translation>Вы уже настроили особые настройки качества в Мамбл. Выберите эту конфигурацию, чтобы оставить их.</translation>
    </message>
    <message>
        <location/>
        <source>Enables attenuation of other applications while users talk to you. This means that as soon someone starts to speak to you in Mumble, the sound of all other applications (like audio players) will get attenuated so you can hear them more clearly.</source>
        <translation>Включить приглушение других приложений, когда другие пользователи говорят с Вами. Это означает, что когда кто-то говорит в Мамбл, звук всех остальных приложений (например аудио проигрывателя), будет приглушен, для того чтобы Вы могли лучше слышать говорящего.</translation>
    </message>
    <message>
        <location/>
        <source>Apply some high contrast optimizations for visually impaired users</source>
        <translation>Применить высококонтрастную цветовую схему для людей со слабым зрением</translation>
    </message>
    <message>
        <location/>
        <source>Use high contrast graphics</source>
        <translation>Использовать высококонтраструю цветовую схему</translation>
    </message>
    <message>
        <location/>
        <source>Keep custom Text-To-Speech settings.</source>
        <translation>Оставить текущие настройки текст-в-речь.</translation>
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
        <translation>Причина</translation>
    </message>
    <message>
        <location/>
        <source>Start</source>
        <translation>Начало</translation>
    </message>
    <message>
        <location/>
        <source>End</source>
        <translation>Конец</translation>
    </message>
    <message>
        <location/>
        <source>User</source>
        <translation>Пользователь</translation>
    </message>
    <message>
        <location/>
        <source>Hash</source>
        <translation>Хэш</translation>
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
        <translation>&amp;Адрес</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Mask</source>
        <translation>&amp;Маска</translation>
    </message>
</context>
<context>
    <name>CertView</name>
    <message>
        <location filename="Cert.cpp" line="+50"/>
        <source>Name</source>
        <translation>Имя</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Email</source>
        <translation>Email</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Issuer</source>
        <translation>Организация</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Expiry Date</source>
        <translation>Истекает</translation>
    </message>
    <message>
        <location line="+34"/>
        <source>(none)</source>
        <translation>(нет)</translation>
    </message>
    <message>
        <location line="+11"/>
        <source>Self-signed</source>
        <translation>Самоподписанный</translation>
    </message>
</context>
<context>
    <name>CertWizard</name>
    <message>
        <location line="+90"/>
        <source>Resolving domain %1.</source>
        <translation>Поиск %1.</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Unable to validate email.&lt;br /&gt;Enter a valid (or blank) email to continue.</source>
        <translation>Неправильный email.&lt;br /&gt;Введите правильный (или пустой) email чтобы продолжить.</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>There was an error generating your certificate.&lt;br /&gt;Please try again.</source>
        <translation>Произошла ошибка при создании сертификата. &lt;br /&gt;Пожалуйста, попробуйте еще раз.</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Your certificate and key could not be exported to PKCS#12 format. There might be an error in your certificate.</source>
        <translation>Ваш сертификат не может быть экспортирован в формат PKCS#12. Возможно имеется ошибка в Вашем сертификате.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>The file could not be opened for writing. Please use another file.</source>
        <translation>Невозможно открыть файл для записи. Используйте другой файл.</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>The file could not be written successfully. Please use another file.</source>
        <translation>Невозможна запись в файл. Пожалуйста попробуйте другой файл.</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>The file could not be opened for reading. Please use another file.</source>
        <translation>Невозможно открыть файл для чтения. Используйте другой файл.</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>The file is empty or could not be read. Please use another file.</source>
        <translation>Файл пуст или не может быть прочтен. Используйте другой файл.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>The file did not contain a valid certificate and key. Please use another file.</source>
        <translation>Файл не содержит действительного сертификата или ключа.</translation>
    </message>
    <message>
        <location line="+22"/>
        <source>Select file to export certificate to</source>
        <translation>Выберите файл для экспорта</translation>
    </message>
    <message>
        <location line="+37"/>
        <source>Select file to import certificate from</source>
        <translation>Выберите файл для импорта</translation>
    </message>
    <message>
        <location line="+57"/>
        <source>Unable to resolve domain.</source>
        <translation>Невозможно найти домен.</translation>
    </message>
    <message>
        <location line="+43"/>
        <source>Mumble User</source>
        <translation>Пользователь Мамбл</translation>
    </message>
    <message>
        <location filename="main.cpp" line="+409"/>
        <source>&lt;b&gt;Certificate Expiry:&lt;/b&gt; Your certificate is about to expire. You need to renew it, or you will no longer be able to connect to servers you are registered on.</source>
        <translation>&lt;b&gt;Сертификат просрочен:&lt;/b&gt; Срок действия Вашего сертификата скоро истечет. Вам необходимо обновить его, иначе Вы не сможете подключиться к серверам, на которых зарегистрированы.</translation>
    </message>
</context>
<context>
    <name>Certificates</name>
    <message>
        <location filename="Cert.ui"/>
        <source>Certificate Management</source>
        <translation>Управление сертификатами</translation>
    </message>
    <message>
        <location/>
        <source>Certificate Authentication</source>
        <translation>Авторизация сертификатов</translation>
    </message>
    <message>
        <location/>
        <source>Authenticating to servers without using passwords</source>
        <translation>Авторизация на серверах без использования паролей</translation>
    </message>
    <message>
        <location/>
        <source>Current certificate</source>
        <translation>Текущий сертификат</translation>
    </message>
    <message>
        <location/>
        <source>This is the certificate Mumble currently uses.</source>
        <translation>Это сертификат, который в настоящий момент используется Мамбл.</translation>
    </message>
    <message>
        <location/>
        <source>Current Certificate</source>
        <translation>Текущий сертификат</translation>
    </message>
    <message>
        <location/>
        <source>Create a new certificate</source>
        <translation>Новый сертификат</translation>
    </message>
    <message>
        <location/>
        <source>This will create a new certificate.</source>
        <translation>Создать новый сертификат.</translation>
    </message>
    <message>
        <location/>
        <source>Import certificate from file</source>
        <translation>Импорт сертификата из файла</translation>
    </message>
    <message>
        <location/>
        <source>This will import a certificate from file.</source>
        <translation>Импортировать сертификат из файла.</translation>
    </message>
    <message>
        <location/>
        <source>Import a certificate</source>
        <translation>Импорт сертификата</translation>
    </message>
    <message>
        <location/>
        <source>Export Certificate</source>
        <translation>Экспорт сертификата</translation>
    </message>
    <message>
        <location/>
        <source>This will export a certificate to file.</source>
        <translation>Экспортировать сертификат в файл.</translation>
    </message>
    <message>
        <location/>
        <source>Export current certificate</source>
        <translation>Экспорт сертификата</translation>
    </message>
    <message>
        <location/>
        <source>Import Certificate</source>
        <translation>Импорт сертификата</translation>
    </message>
    <message>
        <location/>
        <source>PKCS #12 Certificate import</source>
        <translation>Импорт сертификата PKCS #12</translation>
    </message>
    <message>
        <location/>
        <source>&lt;p&gt;Mumble can import certificates stored in PKCS #12 format. This is the format used when exporting a key from Mumble, and also when exporting keys from Firefox, Internet Explorer, Opera etc.&lt;/p&gt;&lt;p&gt;If the file is password protected, you will need the password to import the certificate.&lt;/p&gt;</source>
        <oldsource>&lt;p&gt;Mumble can import certificates stored in PKCS #12 format. This is the format used when exporting a key from Mumble, and also when exporting keys from FireFox, Internet Explorer, Opera etc.&lt;/p&gt;&lt;p&gt;If the file is password protected, you will need the password to import the certificate.&lt;/p&gt;</oldsource>
        <translation>&lt;p&gt;Mumble может импортировать сертификат в формате PKCS #12. Это формат, который используется для экспорта ключей из Мамбл, или Firefox, Internet Explorer, Opera и т.д.&lt;/p&gt;&lt;p&gt;Если файл защищен паролем, Вам необходимо будет его ввести для импорта сертификата.&lt;/p&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Import from</source>
        <translation>Импортировать из</translation>
    </message>
    <message>
        <location/>
        <source>Filename to import from</source>
        <translation>Имя файла для импорта</translation>
    </message>
    <message>
        <location/>
        <source>This is the filename you wish to import a certificate from.</source>
        <translation>Это имя файла, из которого Вы хотите импортировать сертификат.</translation>
    </message>
    <message>
        <location/>
        <source>Select file to import from</source>
        <translation>Выберите файл для импорта</translation>
    </message>
    <message>
        <location/>
        <source>This opens a file selection dialog to choose a file to import a certificate from.</source>
        <translation>Открывает диалог выбора файла.</translation>
    </message>
    <message>
        <location/>
        <source>Open...</source>
        <translation>Открыть...</translation>
    </message>
    <message>
        <location/>
        <source>Password</source>
        <translation>Пароль</translation>
    </message>
    <message>
        <location/>
        <source>Password for PKCS#12 file</source>
        <translation>Пароль для файла PKCS#12</translation>
    </message>
    <message>
        <location/>
        <source>This is the password for the PKCS#12 file containing your certificate.</source>
        <translation>Это пароль для файла  PKCS#12, содержащего Ваш сертификат.</translation>
    </message>
    <message>
        <location/>
        <source>Certificate to import</source>
        <translation>Сертификат для импорта</translation>
    </message>
    <message>
        <location/>
        <source>This is the certificate you are importing.</source>
        <translation>Это сертификат, который Вы импортируете.</translation>
    </message>
    <message>
        <location/>
        <source>Certificate Details</source>
        <translation>Детали сертификата</translation>
    </message>
    <message>
        <location/>
        <source>Replace Certificate</source>
        <translation>Заменить сертификат</translation>
    </message>
    <message>
        <location/>
        <source>Replace existing certificate with new certificate?</source>
        <translation>Заменить существующий сертификат новым?</translation>
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
        <translation>&lt;p&gt;У Вас уже есть сертификат в Mumble, и вы собираетесь его заменить.&lt;/p&gt;
&lt;p&gt;Если Вы обновляете сертификат, предоставленный Вам доверенным CA и почтовый адрес соответствует адресу, указанному в Вашем текущем сертификате, то это полностью безопасно, и сервера, к которым Вы подключаетесь автоматически распознают более значимый сертификат для вашего email адреса.
&lt;/p&gt;
&lt;p&gt;В противном случае ВЫ больше не будете распознаны серверами, на которых авторизировались ранее. Если Вы не зарегистрированы ни на одном сервере, то Вам не о чем беспокоиться.
&lt;/p&gt;
Вы уверены, что хотите заменить Ваш сертификат?
&lt;p&gt;
</translation>
    </message>
    <message>
        <location/>
        <source>This is the certificate Mumble currently uses. It will be replaced.</source>
        <translation>Это текущий сертификат. Он будет заменен.</translation>
    </message>
    <message>
        <location/>
        <source>New certificate</source>
        <translation>Новый сертификат</translation>
    </message>
    <message>
        <location/>
        <source>This is the new certificate that will replace the old one.</source>
        <translation>Это новый сертификат, который заменит старый.</translation>
    </message>
    <message>
        <location/>
        <source>New Certificate</source>
        <translation>Новый Сертификат</translation>
    </message>
    <message>
        <location/>
        <source>Make a backup of your certificate</source>
        <translation>Сделать резервную копию Вашего сертификата</translation>
    </message>
    <message>
        <location/>
        <source>&lt;p&gt;If you ever lose your current certificate, which will happen if your computer suffers a hardware failure or you reinstall your machine, you will no longer be able to authenticate to any server you are registered on. It is therefore &lt;b&gt;mandatory&lt;/b&gt; that you make a backup of your certificate. We strongly recommend you store this backup on removable storage, such as an USB memory stick.&lt;/p&gt;
&lt;p&gt;Note that this file will not be encrypted, and if anyone gains access to it, they will be able to impersonate you, so take good care of it.&lt;/p&gt;</source>
        <translation>&lt;p&gt;Если Вы случайно потеряете Ваш текущий сертификат, что может произойти при неисправности Вашего компьютера, или при переустановке ОС, Вы более на сможете авторизироваться на серверы, на которых Вы зарегистрированы. Поэтому, &lt;b&gt;обязательно&lt;/b&gt; сделать резервную копию Вашего сертификата. Мы настоятельно рекомендуем хранить резервную копию на диске или флэшке.&lt;/p&gt;
&lt;p&gt;Этот файл не зашифрован, так что если кто-либо получит доступ к нему, он сможет авторизироваться на серверах под Вашим именем, так что храните его в надежном месте, или зашифрованном хранилище.&lt;/p&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Export to</source>
        <translation>Экспорт в</translation>
    </message>
    <message>
        <location/>
        <source>Filename to export to</source>
        <translation>Имя файла для экспорта</translation>
    </message>
    <message>
        <location/>
        <source>This is the filename you wish to export a certificate to.</source>
        <translation>Имя файла, в который Вы хотите экспортировать сертификат.</translation>
    </message>
    <message>
        <location/>
        <source>Save As...</source>
        <translation>Сохранить Как...</translation>
    </message>
    <message>
        <location/>
        <source>This is the certificate Mumble currently uses. It will be exported.</source>
        <translation>Это текущий сертификат. Он будет экспортирован.</translation>
    </message>
    <message>
        <location/>
        <source>Generate a new certificate for strong authentication</source>
        <translation>Сгенерировать новый сертификат для надежной авторизации</translation>
    </message>
    <message>
        <location/>
        <source>&lt;p&gt;Mumble will now generate a strong certificate for authentication to servers.&lt;/p&gt;&lt;p&gt;If you wish, you may provide some additional information to be stored in the certificate, which will be presented to servers when you connect. If you provide a valid email address, you can upgrade to a CA issued email certificate later on, which provides strong identification.&lt;/p&gt;</source>
        <translation>&lt;p&gt;Mumble сейчас сгенерирует надежный сертификат для авторизации на серверах.&lt;/p&gt;&lt;p&gt;Если хотите, можете предоставить дополнительную информацию, которая будет храниться в сертификате, эта информация будет предоставлена серверам, к которым Вы подключаетесь. Если Вы предоставите действительный email адрес, Вы сможете улучшить его в дальнейшем на доверенный сертификат CA, который обеспечивает надежную идентификацию.&lt;/p&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Name</source>
        <translation>Имя</translation>
    </message>
    <message>
        <location/>
        <source>Email</source>
        <translation>Email</translation>
    </message>
    <message>
        <location/>
        <source>Your email address (e.g. johndoe@mumble.info)</source>
        <translation>Ваш email адрес (например johndoe@mumble.info)</translation>
    </message>
    <message>
        <location/>
        <source>This is your email address. It is strongly recommended to provide a valid email address, as this will allow you to upgrade to a strong certificate without authentication problems.</source>
        <translation>Это Ваш  email адрес. Настоятельно рекомендуется давать правильный адрес, т. к. это позволит в дальнейшем обновить на надежный сертификат без проблем с авторизацией.</translation>
    </message>
    <message>
        <location/>
        <source>Your name (e.g. John Doe)</source>
        <translation>Ваше имя (например Вася Пупкин)</translation>
    </message>
    <message>
        <location/>
        <source>This is your name, and will be filled out in the certificate. This field is entirely optional.</source>
        <translation>Это Ваше имя, оно будет включено в сертификат. Это необязательное поле.</translation>
    </message>
    <message>
        <location/>
        <source>Finish</source>
        <translation>Заврешить</translation>
    </message>
    <message>
        <location/>
        <source>Certificate-based authentication is ready for use</source>
        <translation>Авторизация на основе сертификата готова к использованию</translation>
    </message>
    <message>
        <location/>
        <source>Enjoy using Mumble with strong authentication.</source>
        <translation>Наслаждайтесь Мамбл с надежной авторизацией.</translation>
    </message>
    <message>
        <location/>
        <source>Automatic certificate creation</source>
        <translation>Автосоздание сертификата</translation>
    </message>
    <message>
        <location/>
        <source>&lt;p&gt;Mumble can use certificates to authenticate with servers. Using certificates avoids passwords, meaning you don&apos;t need to disclose any password to the remote site. It also enables very easy user registration.&lt;/p&gt;&lt;p&gt;While Mumble can work without certificates, the majority of servers will expect you to have one.&lt;/p&gt;
&lt;p&gt;
It is &lt;b&gt;strongly&lt;/b&gt; recommended that you &lt;a href=&quot;http://mumble.info/certificate.php&quot;&gt;create a trusted certificate&lt;/a&gt;.
&lt;/p&gt;
</source>
        <translation>&lt;p&gt;Mumble может использовать сертификаты для авторизации на серверах. Использование сертификатов отменяет необходимость использовать пароли. Вам не придется вводить какие-либо пароли для входа на сервер. Это также позволяет легко и быстро регистрироваться на сервере.&lt;/p&gt;&lt;p&gt;Mumble может работать без сертификатов, однако большинство серверов ожидают наличия у Вас сертификата.&lt;/p&gt;
&lt;p&gt;
&lt;b&gt;Настоятельно&lt;/b&gt; рекомендуется &lt;a href=&quot;http://mumble.info/certificate.php&quot;&gt;создать доверенный сертификат&lt;/a&gt;.
&lt;/p&gt;
</translation>
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
        <translation>Входить</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Speak</source>
        <translation>Говорить</translation>
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
        <translation>Сделать временным</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Link channel</source>
        <translation>Связать канал</translation>
    </message>
    <message>
        <location line="-87"/>
        <source>This represents no privileges.</source>
        <translation>Это обозначает отсутствие привелегий.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>This represents total access to the channel, including the ability to change group and ACL information. This privilege implies all other privileges.</source>
        <translation>Это обозначает полный доступ к каналу, веключая возможность изменять информацию о группах и ACL. Эта привелегия включает все привелегии.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>This represents the permission to traverse the channel. If a user is denied this privilege, he will be unable to access this channel and any sub-channels in any way, regardless of other permissions in the sub-channels.</source>
        <translation>Это обозначает разрешение пересекать канал. Если у пользователя нет такой привилегии, он никак не сможет получить доступ к каналу и подканалам, независимо от разрешений подканалов.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>This represents the permission to join the channel. If you have a hierarchical channel structure, you might want to give everyone Traverse, but restrict Enter in the root of your hierarchy.</source>
        <translation>Это обозначает разрешение входить на канал. Если у канала иерархическая структура, вы можете дать каждому право пересекать канал, но ограничивать Вход в корне вашей иерархии.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>This represents the permission to speak in a channel. Users without this privilege will be suppressed by the server (seen as muted), and will be unable to speak until they are unmuted by someone with the appropriate privileges.</source>
        <translation>Это обозначает разрешение говорить на канале. Пользователи без этой привелегии будут заглушены сервером, и неспособны будут говорить, пока не получат разрешение от кого-то, имеющего соответствующие привелегии.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>This represents the permission to whisper to this channel from the outside. This works exactly like the &lt;i&gt;speak&lt;/i&gt; privilege, but applies to packets spoken with the Whisper key held down. This may be used to broadcast to a hierarchy of channels without linking.</source>
        <translation>Это обозначает разрешение шептать в этот канал из другого канала. Это работает точно также, как право &lt;i&gt;говорить&lt;/i&gt;, но применяется к речи при нажатой клавиши Шепота. Это может быть использовано для того чтобы объявить что-то в дерево каналов без необходимости их связывания.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>This represents the permission to mute and deafen other users. Once muted, a user will stay muted until he is unmuted by another privileged user or reconnects to the server.</source>
        <translation>Это обозначает разрешение заглушати или делать немыми других пользователей. Заглушенный пользователь останется заглушенным до тех пор, пока глушение не будет снято другим привелегированным пользователем, или пока не переподключится к серверу.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>This represents the permission to move a user to another channel or kick him from the server. To actually move the user, either the moving user must have Move privileges in the destination channel, or the user must normally be allowed to enter the channel. Users with this privilege can move users into channels the target user normally wouldn&apos;t have permission to enter.</source>
        <translation>Это обозначает разрешение перемещать пользователя в другой канал или кикать его с сервера. Чтобы на самом деле переместить пользователя, либо перемещающий пользователь должен иметь привелегии Перемещения в канале назначения, или пользователю должно быть позволено в ходить в тот канал. Пользователь с этой привилегией может перемещать пользователь в каналы, в которые перемещаемому пользователю доступ обычно закрыт.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>This represents the permission to make sub-channels. The user making the sub-channel will be added to the admin group of the sub-channel.</source>
        <translation>Это обозначает разрешение создавать подканалы. Пользователь создавший подканал будет добавлен в группу admin подканала.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>This represents the permission to make a temporary subchannel. The user making the sub-channel will be added to the admin group of the sub-channel. Temporary channels are not stored and disappear when the last user leaves.</source>
        <translation>Это обозначает разрешение создавать временный подканал. Пользователь создавший подканал будет добавлен в группу admin подканала. Временные каналы не сохраняются и исчезают, когда все пользователи их покинут.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>This represents the permission to link channels. Users in linked channels hear each other, as long as the speaking user has the &lt;i&gt;speak&lt;/i&gt; privilege in the channel of the listener. You need the link privilege in both channels to create a link, but just in either channel to remove it.</source>
        <translation>Это обозначает разрешение связывать каналы. Пользователи в связанных каналах будут слышать друг друга, до тех пор, пока пользователь, который говорит имеет право &lt;i&gt;говорить&lt;/i&gt; в канал того, кто слушает. Необходимо иметь разрешение связи в обоих каналах, чтобы создать связь, и только в одном из каналов, чтобы удалить связь.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>This represents the permission to write text messages to other users in this channel.</source>
        <translation>Это обозначает разрешение писать текстовое сообщение другим пользователям канала.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>This represents the permission to forcibly remove users from the server.</source>
        <translation>Это обозначает разрешение удалять пользователей с сервера.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>This represents the permission to permanently remove users from the server.</source>
        <translation>Это обозначает разрешение перманентно удалять пользователей с сервера.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>This represents the permission to register and unregister users on the server.</source>
        <translation>Это обозначает разрешение регистрировать и отменять регистрацию пользователей на сервере.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>This represents the permission to register oneself on the server.</source>
        <translation>Это обозначает разрешение регистрировать себя на сервере.</translation>
    </message>
    <message>
        <location line="+31"/>
        <source>Whisper</source>
        <translation>Шептать</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Move</source>
        <translation>Перемещать</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Text message</source>
        <translation>Текстовое сообщение</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Kick</source>
        <translation>Кикать</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Ban</source>
        <translation>Банить</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Register User</source>
        <translation>Регистрировать</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Register Self</source>
        <translation>Регистрировать себя</translation>
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
        <translation>Создавать  ACL</translation>
    </message>
</context>
<context>
    <name>ChatbarLineEdit</name>
    <message>
        <location filename="CustomElements.cpp" line="+79"/>
        <source>Paste and send</source>
        <translation>Вставить и отправить</translation>
    </message>
    <message>
        <location line="+25"/>
        <source>&lt;center&gt;Type chat message here&lt;/center&gt;</source>
        <translation>&lt;center&gt;Напишите здесь сообщение в чат&lt;/center&gt;</translation>
    </message>
</context>
<context>
    <name>ClientUser</name>
    <message>
        <location filename="ClientUser.cpp" line="+130"/>
        <source>Friend</source>
        <translation>Друг</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Authenticated</source>
        <translation>Авторизованный</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Priority speaker</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Muted (server)</source>
        <translation>Заглушенный (сервером)</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Deafened (server)</source>
        <translation>Наложена немота (сервером)</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Local Mute</source>
        <translation>Местное Глушение</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Muted (self)</source>
        <translation>Заглушен (самим)</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Deafened (self)</source>
        <translation>Наложена немота (самим)</translation>
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
        <translation>Расширенные настройки</translation>
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
        <translation>Применить</translation>
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
        <translation>Отменить изменения на текущей странице</translation>
    </message>
    <message>
        <location line="+1"/>
        <location filename="ConfigDialog.mm" line="+1"/>
        <source>This button will revert any changes done on the current page to the most recent applied settings.</source>
        <translation>Эта кнопка отменит любые изменения, совершенные на этой странице.</translation>
    </message>
    <message>
        <location line="+3"/>
        <location filename="ConfigDialog.mm" line="+3"/>
        <source>Restore defaults for current page</source>
        <translation>Восстанавлевает стандартные настройки на этой странице</translation>
    </message>
    <message>
        <location line="+1"/>
        <location filename="ConfigDialog.mm" line="+1"/>
        <source>This button will restore the settings for the current page only to their defaults. Other pages will not be changed.&lt;br /&gt;To restore all settings to their defaults, you will have to use this button on every page.</source>
        <translation>Эта кнопка вернет стандартные настройки для этой страницы. Другие страницы не будут изменены.&lt;br /&gt;Чтобы вернуть все настройки на стандартные, вы должны нажать эту кнопку на всех страницах.</translation>
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
        <translation>&amp;Подключиться</translation>
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
        <translation>Соединение с %1</translation>
    </message>
    <message>
        <location line="-470"/>
        <location line="+470"/>
        <source>Enter username</source>
        <translation>Введите имя пользователя</translation>
    </message>
    <message>
        <location line="-470"/>
        <source>Adding host %1</source>
        <translation>Добавляется сервер %1</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui"/>
        <location filename="ConnectDialog.cpp" line="+63"/>
        <source>Servername</source>
        <translation>Сервер</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="+1"/>
        <source>Hostname</source>
        <translation>Имя хоста</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Bonjour name</source>
        <translation>Имя Bonjour</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Port</source>
        <translation>Порт</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Addresses</source>
        <translation>Адреса</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Website</source>
        <translation>Вебсайт</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Packet loss</source>
        <translation>Потеря пакетов</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Ping (80%)</source>
        <translation>Пинг (80%)</translation>
    </message>
    <message>
        <location line="+0"/>
        <location line="+2"/>
        <source>%1 ms</source>
        <translation>%1 мс</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Ping (95%)</source>
        <translation>Пинг (95%)</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Bandwidth</source>
        <translation>Канал</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>%1 kbit/s</source>
        <translation>%1 кбит/с</translation>
    </message>
    <message>
        <location line="+292"/>
        <source>&amp;Filters</source>
        <translation>&amp;Фильтры</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui"/>
        <location filename="ConnectDialog.cpp" line="-291"/>
        <source>Users</source>
        <translation>Пользователи</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="+1"/>
        <source>Version</source>
        <translation>Версия</translation>
    </message>
    <message>
        <source>Mumble</source>
        <translation type="obsolete">Mumble</translation>
    </message>
    <message>
        <location line="+960"/>
        <source>Failed to fetch server list</source>
        <translation>Ошибка при получении списка серверов</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui"/>
        <source>Mumble Server Connect</source>
        <translation>Подключение к серверу Mumble</translation>
    </message>
    <message>
        <location/>
        <source>Ping</source>
        <translation>Пинг</translation>
    </message>
    <message>
        <location/>
        <source>Remove from Favorites</source>
        <translation>Удалить из закладок</translation>
    </message>
    <message>
        <location/>
        <source>Add custom server</source>
        <translation>Добавить новый сервер</translation>
    </message>
    <message>
        <location/>
        <source>Show all servers that respond to ping</source>
        <translation>Показать все серверы, которые отвечают на запрос</translation>
    </message>
    <message>
        <location/>
        <source>Show all servers with users</source>
        <translation>Показать все серверы и пользователей</translation>
    </message>
    <message>
        <location/>
        <source>Show all servers</source>
        <translation>Показть все серверы</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Copy</source>
        <translation>&amp;Копировать</translation>
    </message>
    <message>
        <location/>
        <source>Copy favorite link to clipboard</source>
        <translation>Копировать ссылку в буфер обмена</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Paste</source>
        <translation>&amp;Вставить</translation>
    </message>
    <message>
        <location/>
        <source>Paste favorite from clipboard</source>
        <translation>Вставить из буфера обмена</translation>
    </message>
    <message>
        <source>Add</source>
        <translation type="obsolete">Добавить</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Edit...</source>
        <translation>&amp;Изменить...</translation>
    </message>
    <message>
        <location/>
        <location filename="ConnectDialog.cpp" line="-707"/>
        <source>&amp;Add New...</source>
        <translation>&amp;Новый сервер...</translation>
    </message>
    <message>
        <location/>
        <source>Add to &amp;Favorites</source>
        <translation>Добавить в &amp;закладки</translation>
    </message>
    <message>
        <location/>
        <source>Open &amp;Webpage</source>
        <translation>Открыть &amp;вебстраницу</translation>
    </message>
    <message>
        <location/>
        <source>Show &amp;Reachable</source>
        <translation>Показать &amp;доступные</translation>
    </message>
    <message>
        <location/>
        <source>Show &amp;Populated</source>
        <translation>Показать &amp;населенные</translation>
    </message>
    <message>
        <location/>
        <source>Show &amp;All</source>
        <translation>&amp;Показать все</translation>
    </message>
</context>
<context>
    <name>ConnectDialogEdit</name>
    <message>
        <location filename="ConnectDialogEdit.ui"/>
        <source>Edit Server</source>
        <translation>Изменение/добавление сервера</translation>
    </message>
    <message>
        <location/>
        <source>Edit Server</source>
        <oldsource>&amp;Servername</oldsource>
        <translation type="unfinished">Название &amp;сервера</translation>
    </message>
    <message>
        <location/>
        <source>Name of the server</source>
        <translation>Название сервера (все_что_угодно)</translation>
    </message>
    <message>
        <source>&lt;b&gt;Name&lt;/b&gt;&lt;br/&gt;
Name of the server. This is what the server will be named like in your serverlist and can be chosen freely.</source>
        <translation type="obsolete">&lt;b&gt;Название сервера&lt;/b&gt;&lt;br/&gt;
То как сервер будет обозначаться в списке серверов. Вы можете назвать сервер как вам угодно, на подключение это не повлияет.</translation>
    </message>
    <message>
        <location/>
        <source>A&amp;ddress</source>
        <translation>А&amp;дрес</translation>
    </message>
    <message>
        <location/>
        <source>Internet address of the server.</source>
        <oldsource>Internet address of the server. </oldsource>
        <translation>Интернет адрес сервера.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Address&lt;/b&gt;&lt;br/&gt;
Internet address of the server. This can be a normal hostname, an IPv4/IPv6 address or a Bonjour service identifier. Bonjour service identifiers have to be prefixed with a &apos;@&apos; to be recognized by Mumble.</source>
        <oldsource>&lt;b&gt;Address&lt;/b&gt;&lt;/br&gt;
Internet address of the server. This can be a normal hostname, an ipv4/6 address or a bonjour service identifier. Bonjour service identifiers have to be prefixed with a &apos;@&apos; to be recognized by Mumble.</oldsource>
        <translation>&lt;b&gt;Адрес&lt;/b&gt;&lt;br/&gt;
Интернет адрес сервера. Может быть доменным именем или IPv4/IPv6 адресом, или идентификатором службы Bonjour. Идентификаторы Bonjour должны иметь префикс &apos;@&apos;.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Port</source>
        <translation>&amp;Порт</translation>
    </message>
    <message>
        <location/>
        <source>Port on which the server is listening</source>
        <translation>Порт сервера</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Port&lt;/b&gt;&lt;br/&gt;
Port on which the server is listening. If the server is identified by a Bonjour service identifier this field will be ignored.</source>
        <translation>&lt;b&gt;Порт&lt;/b&gt;&lt;br/&gt;
Порт сервера. Если у сервера есть идентификатор службы Bonjour, этот порт будет проигнорирован.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Username</source>
        <translation>Имя п&amp;ользователя</translation>
    </message>
    <message>
        <location/>
        <source>Username to send to the server</source>
        <translation>Имя пользователя, под которым вы зайдете на сервер</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Username&lt;/b&gt;&lt;br/&gt;
Username to send to the server. Be aware that the server can impose restrictions on how a username might look like. Also your username could already be taken by another user.</source>
        <translation>&lt;b&gt;Имя пользователя&lt;/b&gt;&lt;br/&gt;
Имя пользователя, под которым вы зайдете на сервер. Обратите внимание что на сервере могут быть ограничения. Также имя пользователя может быть уже занято.</translation>
    </message>
    <message>
        <location/>
        <source>Label</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Label&lt;/b&gt;&lt;br/&gt;
Label of the server. This is what the server will be named like in your serverlist and can be chosen freely.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>CoreAudioSystem</name>
    <message>
        <location filename="CoreAudio.cpp" line="+85"/>
        <source>Default Device</source>
        <translation>Устройство по умолчанию</translation>
    </message>
</context>
<context>
    <name>CrashReporter</name>
    <message>
        <location filename="CrashReporter.cpp" line="+37"/>
        <source>Mumble Crash Report</source>
        <translation>Отчет об ошибке Mumble</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>&lt;p&gt;&lt;b&gt;We&apos;re terribly sorry, but it seems Mumble has crashed. Do you want to send a crash report to the Mumble developers?&lt;/b&gt;&lt;/p&gt;&lt;p&gt;The crash report contains a partial copy of Mumble&apos;s memory at the time it crashed, and will help the developers fix the problem.&lt;/p&gt;</source>
        <translation>&lt;p&gt;&lt;b&gt;Мы сожалеем, но похоже Mumble совершил фатальную ошибку и закрылся. Не желаете ли отправить отчет об ошибке разработчикам Mumble?&lt;/b&gt;&lt;/p&gt;&lt;p&gt;Отчет об ошибке содержит частичную копию памяти Mumble&apos;s на момент вылета, и поможет разработчикам исправить проблему.&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Email address (optional)</source>
        <translation>Email адрес (необязательно)</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Please describe briefly, in English, what you were doing at the time of the crash</source>
        <oldsource>Please briefly describe what you were doing at the time of the crash</oldsource>
        <translation>Пожалуйста коротко опишите по Английски, что Вы делали в момент вылета</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Send Report</source>
        <translation>Отправить отчет</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Don&apos;t send report</source>
        <translation>Не отправлять</translation>
    </message>
    <message>
        <location line="+25"/>
        <source>Crash upload successful</source>
        <translation>Отправка успешна</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Thank you for helping make Mumble better!</source>
        <translation>Спасибо за помощь сделать Mumble лучше!</translation>
    </message>
    <message>
        <location line="+2"/>
        <location line="+2"/>
        <source>Crash upload failed</source>
        <translation>Отправка неудалась</translation>
    </message>
    <message>
        <location line="-2"/>
        <source>We&apos;re really sorry, but it appears the crash upload has failed with error %1 %2. Please inform a developer.</source>
        <translation>Сожалеем, но похоже отправка отчета об ошибке не удалась с ошибкой %1 %2. Пожалуйста проинформируйте об этом разработчика.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>This really isn&apos;t funny, but apparently there&apos;s a bug in the crash reporting code, and we&apos;ve failed to upload the report. You may inform a developer about error %1</source>
        <translation>Это действительно не смешно, но очевидно имеется ошибка в коде отправке отчета об ошибке, и его отправка не удалась. Вы можете проинформировать разработчика об ошибке %1</translation>
    </message>
    <message>
        <location line="+114"/>
        <source>Uploading crash report</source>
        <translation>Отправка отчета об ошибке</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Abort upload</source>
        <translation>Отмена отправки</translation>
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
        <translation>Голосовой вход DirectSound по умолчанию</translation>
    </message>
    <message>
        <location line="+461"/>
        <source>Opening chosen DirectSound Input device failed. No microphone capture will be done.</source>
        <translation>Открытие выбранного входа DirectSound невозможно. Микрофон не будет работать.</translation>
    </message>
    <message>
        <location line="-5"/>
        <source>Lost DirectSound input device.</source>
        <translation>Потерянное устройство входа DirectSound.</translation>
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
        <translation>Голосовой выход DirectSound по умолчанию</translation>
    </message>
    <message>
        <location line="+325"/>
        <source>Lost DirectSound output device.</source>
        <translation>Потерянное устройство вывода  DirectSound.</translation>
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
        <translation>База данных &apos;%1&apos; доступна только для чтения. Mumble не сможет хранить настройки сервера (SSL сертификаты) до тех пор, пока Вы не исправите эту проблему.</translation>
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
        <translation>Комбинации клавиш</translation>
    </message>
    <message>
        <location/>
        <source>List of configured shortcuts</source>
        <translation>Список привязок комбинаций клавиш</translation>
    </message>
    <message>
        <location/>
        <source>Function</source>
        <translation>Функция</translation>
    </message>
    <message>
        <location/>
        <source>Data</source>
        <translation>Данные</translation>
    </message>
    <message>
        <location/>
        <source>Shortcut</source>
        <translation>Комбинация клавиш</translation>
    </message>
    <message>
        <location/>
        <source>Suppress</source>
        <translation>Глобальная</translation>
    </message>
    <message>
        <location/>
        <source>Add new shortcut</source>
        <translation>Добавить новое сочетание клавишь</translation>
    </message>
    <message>
        <location/>
        <source>This will add a new global shortcut</source>
        <translation>Это добавит новую глобальную комбинацию клавиш</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Add</source>
        <translation>&amp;Добавить</translation>
    </message>
    <message>
        <location/>
        <source>Remove selected shortcut</source>
        <translation>Удалить выбранную комбинацию</translation>
    </message>
    <message>
        <location/>
        <source>This will permanently remove a selected shortcut.</source>
        <translation>Это полностью удалит выбранную комбинацию клавишь.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Remove</source>
        <translation>&amp;Удалить</translation>
    </message>
</context>
<context>
    <name>GlobalShortcutConfig</name>
    <message>
        <location filename="GlobalShortcut.cpp" line="+655"/>
        <source>Shortcuts</source>
        <translation>Сочетания клавиш</translation>
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
        <translation>Сочетание клавиш.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>&lt;b&gt;This is the global shortcut key combination.&lt;/b&gt;&lt;br /&gt;Click this field and then press the desired key/button combo to rebind. Double-click to clear.</source>
        <oldsource>&lt;b&gt;This is the global shortcut key combination.&lt;/b&gt;&lt;br /&gt;Double-click this field and then the desired key/button combo to rebind.</oldsource>
        <translation>&lt;b&gt;Это глобальная комбинация клавиш.&lt;/b&gt;&lt;br /&gt;Кликните на этом поле, и затем нажмите желаемую клавишу/комбинацию клавишь, чтобы изменить. Двойной клик для очистки.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Suppress keys from other applications</source>
        <translation>Скрыть нажатие этих кнопок от других приложений</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>&lt;b&gt;This hides the button presses from other applications.&lt;/b&gt;&lt;br /&gt;Enabling this will hide the button (or the last button of a multi-button combo) from other applications. Note that not all buttons can be suppressed.</source>
        <translation>&lt;b&gt;Эта опция скрывает нажатие  клавиши для других приложений.&lt;/b&gt;&lt;br /&gt;Задействовав эту опцию, нажатие клаваши  (или нажатие последней клавиши в комбинации) будет скрыто от других приложений. Не все клавши могут быть скрыты.</translation>
    </message>
</context>
<context>
    <name>GlobalShortcutMacInit</name>
    <message>
        <location filename="GlobalShortcut_macx.cpp" line="+60"/>
        <source>Mumble has detected that it is unable to receive Global Shortcut events when it is in the background.&lt;br /&gt;&lt;br /&gt;This is because the Universal Access feature called &apos;Enable access for assistive devices&apos; is currently disabled.&lt;br /&gt;&lt;br /&gt;Please &lt;a href=&quot; &quot;&gt;enable this setting&lt;/a&gt; and continue when done.</source>
        <translation>Mumble обнаружил, что не может распознавать Глобальные сочетания клавиш, когда свернут.&lt;br /&gt;&lt;br /&gt;Это происходит потому, что функция Универсального Доступа, называемая &quot;Включить доступ для вспомогательных устройств&quot;, в настоя щее время отключена.&lt;br /&gt;&lt;br /&gt;Пожалуйста, &lt;a href=&quot; &quot;&gt;включите эту настройку&lt;/a&gt; и продолжите, когда это будет сделано.</translation>
    </message>
</context>
<context>
    <name>GlobalShortcutTarget</name>
    <message>
        <location filename="GlobalShortcutTarget.ui"/>
        <source>Whisper Target</source>
        <translation>Цель Шепота</translation>
    </message>
    <message>
        <location/>
        <source>Whisper to list of Users</source>
        <translation>Шепнуть пользователям из списка</translation>
    </message>
    <message>
        <location/>
        <source>Channel Target</source>
        <translation>Целевой канал</translation>
    </message>
    <message>
        <location/>
        <source>Restrict to Group</source>
        <translation>Ограничить группой</translation>
    </message>
    <message>
        <location/>
        <source>If specified, only members of this group will receive the whisper.</source>
        <translation>Если указано, только члены этой группы будут слышать шепот.</translation>
    </message>
    <message>
        <location/>
        <source>If checked the whisper will also be transmitted to linked channels.</source>
        <translation>Если отмечено, шепот будет также передаваться в связанные каналы.</translation>
    </message>
    <message>
        <location/>
        <source>If checked this whisper will also be sent to the subchannels of the channel target.</source>
        <translation>Если отмечено, шепот будет также передаваться в подканалы канала назначения.</translation>
    </message>
    <message>
        <location/>
        <source>List of users</source>
        <translation>Список пользователей</translation>
    </message>
    <message>
        <location/>
        <source>Add</source>
        <translation>Добавить</translation>
    </message>
    <message>
        <location/>
        <source>Remove</source>
        <translation>Удалить</translation>
    </message>
    <message>
        <location/>
        <source>Modifiers</source>
        <translation>Модификаторы</translation>
    </message>
    <message>
        <location/>
        <source>Do not send positional audio information when using this whisper shortcut.</source>
        <translation>Не отсылать позицию в игре, когда используется это сочетание клавиш для шепота.</translation>
    </message>
    <message>
        <location/>
        <source>Ignore positional audio</source>
        <translation>Игнорировать позиционирование</translation>
    </message>
    <message>
        <location/>
        <source>Shout to Linked channels</source>
        <translation>Крикнуть в связаныые каналы</translation>
    </message>
    <message>
        <location/>
        <source>Shout to subchannels</source>
        <translation>Крикнуть в подканалы</translation>
    </message>
    <message>
        <location/>
        <source>Shout to Channel</source>
        <translation>Крикнуть в канал</translation>
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
        <translation>Клавиша мыши %1</translation>
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
        <translation>Не подлкючен</translation>
    </message>
</context>
<context>
    <name>LCDConfig</name>
    <message>
        <location line="-160"/>
        <source>Enable this device</source>
        <translation>Задействовать это устройство</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>LCD</source>
        <translation>LCD</translation>
    </message>
    <message>
        <location filename="LCD.ui"/>
        <source>Form</source>
        <translation>Форма</translation>
    </message>
    <message>
        <location/>
        <source>Devices</source>
        <translation>Устройства</translation>
    </message>
    <message>
        <location/>
        <source>Name</source>
        <translation>Имя</translation>
    </message>
    <message>
        <location/>
        <source>&lt;p&gt;This is the list of available LCD devices on your system.  It lists devices by name, but also includes the size of the display. Mumble supports outputting to several LCD devices at a time.&lt;/p&gt;
&lt;h3&gt;Size:&lt;/h3&gt;
&lt;p&gt;
This field describes the size of an LCD device. The size is given either in pixels (for Graphic LCDs) or in characters (for Character LCDs).&lt;/p&gt;
&lt;h3&gt;Enabled:&lt;/h3&gt;
&lt;p&gt;This decides whether Mumble should draw to a particular LCD device.&lt;/p&gt;</source>
        <translation>&lt;p&gt;Это список доступных LCD-устройств в Вашей системе. Устройства показаны по имени, а также указан размер дисплея. Mumble поддерживает вывод на несколько LCD дисплеев сразу.&lt;/p&gt;
&lt;h3&gt;Размер:&lt;/h3&gt;
&lt;p&gt;
В этой графе указан размер LCD дисплея. Размер указан либо в пикселях (для графических LCD) или в символах для текстовых LCD).&lt;/p&gt;
&lt;h3&gt;Включен:&lt;/h3&gt;
&lt;p&gt;Здесь указано должен ли Mumble выводить информацию на конкретный дисплей.&lt;/p&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Size</source>
        <translation>Размер</translation>
    </message>
    <message>
        <location/>
        <source>Enabled</source>
        <translation>Включено</translation>
    </message>
    <message>
        <location/>
        <source>Views</source>
        <translation>Отображение</translation>
    </message>
    <message>
        <location/>
        <source>Minimum Column Width</source>
        <translation>Мин. размер столбца</translation>
    </message>
    <message>
        <location/>
        <source>&lt;p&gt;This option decides the minimum width a column in the User View.&lt;/p&gt;
&lt;p&gt;If too many people are speaking at once, the User View will split itself into columns. You can use this option to pick a compromise between number of users shown on the LCD, and width of user names.&lt;/p&gt;
</source>
        <translation>&lt;p&gt;Эта опция определяет минимальную ширину колонке Списка Пользователей.&lt;/p&gt;
&lt;p&gt;Если слишком много людей говорят одновременно, Список Пользователей будет разбит на колонки. Вы можете использовать эту опцию, чтобы деражть компромисс между количеством пользователей, показанных на дисплее и шириной имени пользователя.&lt;/p&gt;</translation>
    </message>
    <message>
        <location/>
        <source>This setting decides the width of column splitter.</source>
        <translation>Этой настройкой определяется ширина колонки разделителя.</translation>
    </message>
    <message>
        <location/>
        <source>Splitter Width</source>
        <translation>Ширина разделителя</translation>
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
        <translation>Другой игрок заглушил(а) сам себя</translation>
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
        <translation>Текстовое сообщение</translation>
    </message>
    <message>
        <location line="-12"/>
        <source>User Joined Server</source>
        <translation>Пользователь вошел(а) на сервер</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>User Left Server</source>
        <translation>Пользователь покинул(а) сервер</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>User kicked (you or by you)</source>
        <translation>Пользователь выкинул(а)/выкинут(а) (Вас или Вами)</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>User kicked</source>
        <translation>Пользователь выкинут(а)</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>User muted (you)</source>
        <translation>Пользователь заглушил(а) (Вас)</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>User muted (by you)</source>
        <translation>Пользователь заглушен(а) (Вами)</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>User muted (other)</source>
        <translation>Пользователь заглушен(а) (кем-то)</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>User Joined Channel</source>
        <translation>Пользователь вошел(а) в канал</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>User Left Channel</source>
        <translation>Пользователь покинул(а) канал</translation>
    </message>
    <message>
        <location line="+54"/>
        <source>the server</source>
        <translation>сервер</translation>
    </message>
    <message>
        <location line="+117"/>
        <source>[[ Text object too large to display ]]</source>
        <translation>[[ Текст слишком большой ]]</translation>
    </message>
    <message>
        <location line="+42"/>
        <source>[Date changed to %1]
</source>
        <translation>[Дата сменилась на %1]</translation>
    </message>
    <message>
        <location line="+136"/>
        <source>link to %1</source>
        <translation>ссылка на %1</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>ftp link to %1</source>
        <translation>ftp ссылка на %1</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>player link</source>
        <translation>ссылка игрока</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>channel link</source>
        <translation>ссылка канала</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1 link</source>
        <translation>%1 связь</translation>
    </message>
</context>
<context>
    <name>LogConfig</name>
    <message>
        <location line="-514"/>
        <source>Toggle console for %1 events</source>
        <translation>Включение/выключение лога событий &quot;%1&quot;</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Toggle pop-up notifications for %1 events</source>
        <translation>Включение/выключение всплывающих уведомлений для событий &quot;%1&quot;</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Toggle Text-To-Speech for %1 events</source>
        <translation>Включение/выключение Текст-в речь для событий &quot;%1&quot;</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Click here to toggle sound notification for %1 events</source>
        <translation>Включение/выключение звуковых уведомлений для событий &quot;%1&quot;</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Path to sound file used for sound notifications in the case of %1 events&lt;br /&gt;Single click to play&lt;br /&gt;Double-click to change</source>
        <translation>Путь к звуковому файлу для звукового уведомления событий типа &quot;%1&quot;&lt;br /&gt;Кликните, чтобы проиграть&lt;br /&gt;Двойной клик, чтобы изменить</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Click here to toggle console output for %1 events.&lt;br /&gt;If checked, this option makes Mumble output all %1 events in its message log.</source>
        <translation>Кликните здесь, чтобы включить или выключить отображение событий типа &quot;%1&quot; в логе сервера.&lt;br /&gt;Если отмечено, Mumble будет выводить все события типа &quot;%1&quot; в логе сервера в главном окне.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Click here to toggle pop-up notifications for %1 events.&lt;br /&gt;If checked, a notification pop-up will be created by Mumble for every %1 event.</source>
        <oldsource>Click here to toggle pop-up notifications for %1 events.&lt;br /&gt;If checked, a notification pop-up will be created by mumble for every %1 event.</oldsource>
        <translation>Кликните здесь, чтобы включить или выключить всплывающие уведомления для событий типа &quot;%1&quot;.&lt;br /&gt;Если отмечено, Mumble будет показывать всплывающие уведомления для всех событий типа &quot;%1&quot;.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Path to sound file used for sound notifications in the case of %1 events.&lt;br /&gt;Single click to play&lt;br /&gt;Double-click to change&lt;br /&gt;Ensure that sound notifications for these events are enabled or this field will not have any effect.</source>
        <translation>Путь к звуковому файлу, который будет воспроизводится для событий типа &quot;%1&quot;.&lt;br /&gt;Кликните, чтобы проиграть&lt;br /&gt;Двойной клик, чтобы изменить&lt;br /&gt;Убедитесь, что звуковые уведомления для этих событий включены, иначе этот переключатель не будет иметь эффекта.</translation>
    </message>
    <message>
        <location line="-2"/>
        <source>Click here to toggle Text-To-Speech for %1 events.&lt;br /&gt;If checked, Mumble uses Text-To-Speech to read %1 events out loud to you. Text-To-Speech is also able to read the contents of the event which is not true for sound files. Text-To-Speech and sound files cannot be used at the same time.</source>
        <oldsource>Click here to toggle sound notification for %1 events.&lt;br /&gt;If checked, Mumble uses a soundfile predefined by you to indicate %1 events. Soundfiles and Text-To-Speech cannot be used at the same time.</oldsource>
        <translation>Кликните здесь, чтобы включить/выключить функцию Текст-в речь для событий типа &quot;%1&quot;.&lt;br /&gt;Если отмечено, Mumble будет использовать Текст-в-речь, чтобы прочитать события типа &quot;%1&quot; для Вас. Текст-в-речь также может читать содержание события, о котором не могут проинформировать звуковые файлы. Текст-в-речь и звуковые уведомления не могут быть использованы одновременно.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Click here to toggle sound notification for %1 events.&lt;br /&gt;If checked, Mumble uses a sound file predefined by you to indicate %1 events. Sound files and Text-To-Speech cannot be used at the same time.</source>
        <oldsource>Path to soundfile used for sound notifications in the case of %1 events.&lt;br /&gt;Single click to play&lt;br /&gt;Doubleclick to change&lt;br /&gt;Be sure that sound notifications for these events are enabled or this field will not have any effect.</oldsource>
        <translation>Кликните здесь, чтобы включить или выключить звуковые уведомления для событий типа &quot;%1&quot;.&lt;br /&gt;Если отмечено, Mumble будет проигрывать звуковой файл для уведомления о событиях типа &quot;%1&quot;. Звуковые файлы не могут быть использованы одновременно с функцией Текст-в-речь.</translation>
    </message>
    <message>
        <location filename="Log.ui"/>
        <source>Messages</source>
        <translation>Уведомления</translation>
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
        <translation>Макс. длинна</translation>
    </message>
    <message>
        <location/>
        <source>Message length threshold for Text-To-Speech Engine</source>
        <translation>Максимальная длинна текста, который будет прочитан функцией Текст-в-речь</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is the length threshold used for the Text-To-Speech Engine.&lt;/b&gt;&lt;br /&gt;Messages longer than this limit will not be read aloud in their full length.</source>
        <translation>&lt;b&gt;Это ограничение длины текста, который будет прочитан функцией Текст-в-речь.&lt;/b&gt;&lt;br /&gt;Сообщения длиннее этого лимита будут прочитаны не полностью.</translation>
    </message>
    <message>
        <location/>
        <source>Whisper</source>
        <translation>Шепот</translation>
    </message>
    <message>
        <location/>
        <source>If checked you will only hear whispers from users you added to your friend list.</source>
        <translation>Если отмечено, вы будете слышать шепот только от пользователей, которых добавили в свой список друзей.</translation>
    </message>
    <message>
        <location/>
        <source>Only accept whispers from friends</source>
        <translation>Слышать шепот только от друзей</translation>
    </message>
    <message>
        <location/>
        <source>Message</source>
        <translation>Сообщение</translation>
    </message>
    <message>
        <location/>
        <source>Notification</source>
        <translation>Уведомление</translation>
    </message>
    <message>
        <location/>
        <source>Text-To-Speech</source>
        <translation>Текст-в-Речь</translation>
    </message>
    <message>
        <location/>
        <source>Soundfile</source>
        <translation>Звуковой файл</translation>
    </message>
    <message>
        <location/>
        <source>Path</source>
        <translation>Путь</translation>
    </message>
    <message>
        <location/>
        <source> Characters</source>
        <translation> символов</translation>
    </message>
</context>
<context>
    <name>LookConfig</name>
    <message>
        <location filename="LookConfig.ui"/>
        <source>Language</source>
        <translation>Язык</translation>
    </message>
    <message>
        <location/>
        <source>Look and Feel</source>
        <translation>Внешний вид</translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="+52"/>
        <location line="+11"/>
        <source>System default</source>
        <translation>Системный</translation>
    </message>
    <message>
        <location filename="LookConfig.ui"/>
        <source>Language to use (requires restart)</source>
        <translation>Язык интерфейса (требуется перезапуск)</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets which language Mumble should use.&lt;/b&gt;&lt;br /&gt;You have to restart Mumble to use the new language.</source>
        <translation>&lt;b&gt;Здесь устанавливается язык интерфейса Mumble.&lt;/b&gt;&lt;br /&gt;Необходим перезапуск Mumble для использования другого языка.</translation>
    </message>
    <message>
        <location/>
        <source>Style</source>
        <translation>Стиль</translation>
    </message>
    <message>
        <location/>
        <source>Layout</source>
        <translation>Представление</translation>
    </message>
    <message>
        <location/>
        <source>Classic</source>
        <translation>Классический</translation>
    </message>
    <message>
        <location/>
        <source>Stacked</source>
        <translation>Вертикальный</translation>
    </message>
    <message>
        <location/>
        <source>Hybrid</source>
        <translation>Гибридный</translation>
    </message>
    <message>
        <location/>
        <source>Custom</source>
        <translation>Другой</translation>
    </message>
    <message>
        <location/>
        <source>This changes the behavior when moving channels.</source>
        <translation>Изменяет поведение при перемещении каналов.</translation>
    </message>
    <message>
        <location/>
        <source>This sets the behavior of channel drags; it can be used to prevent accidental dragging. &lt;i&gt;Move&lt;/i&gt; moves the channel without prompting. &lt;i&gt;Do Nothing&lt;/i&gt; does nothing and prints an error message. &lt;i&gt;Ask&lt;/i&gt; uses a message box to confirm if you really wanted to move the channel.</source>
        <oldsource>This sets the behavior of channel drags; it can be used to prevent accidental dragging. &lt;i&gt;Move Channel&lt;/i&gt; moves the channel without prompting. &lt;i&gt;Do Nothing&lt;/i&gt; does nothing and prints an error message. &lt;i&gt;Ask&lt;/i&gt; uses a message box to confirm if you really wanted to move the channel.</oldsource>
        <translation>Изменяет поведение при перемещении каналов, используется для исключения случайного перемещения канала. &lt;i&gt;Переместить&lt;/i&gt; - перемещает канал без запроса. &lt;i&gt;Ничего не делать&lt;/i&gt; - не перемещать канал и выдать сообщение об ошибке. &lt;i&gt;Спрашивать&lt;/i&gt; - выводит запрос при попытке переместить канал.</translation>
    </message>
    <message>
        <location/>
        <source>Basic widget style</source>
        <translation>Основной стиль виджетов</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the basic look and feel to use.&lt;/b&gt;</source>
        <translation>&lt;b&gt;Устанавливает основные параметры внешнего вида Мамбл.&lt;/b&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Skin</source>
        <translation>Скин</translation>
    </message>
    <message>
        <location/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location/>
        <source>Show number of users in each channel</source>
        <translation>Показывать количество пользователей в каждом канале</translation>
    </message>
    <message>
        <location/>
        <source>Show channel user count</source>
        <translation>Количество пользователей</translation>
    </message>
    <message>
        <location/>
        <source>Skin file to use</source>
        <translation>Файл скина</translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="+14"/>
        <source>User Interface</source>
        <translation>Интерфейс</translation>
    </message>
    <message>
        <location line="+127"/>
        <source>Choose skin file</source>
        <translation>Выберите файл скина</translation>
    </message>
    <message>
        <location filename="LookConfig.ui"/>
        <source>&lt;b&gt;This sets which skin Mumble should use.&lt;/b&gt;&lt;br /&gt;The skin is a style file applied on top of the basic widget style. If there are icons in the same directory as the style sheet, those will replace the default icons.</source>
        <translation>&lt;b&gt;Устанавливает скин для Mumble.&lt;/b&gt;&lt;br /&gt;Скин - это файл стиля, накладываемый поверх основного стиля. Если иконки стиля находятся в одной папке с ним, то они заменят иконки по умолчанию.</translation>
    </message>
    <message>
        <location/>
        <source>Form</source>
        <translation>Форма</translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="-137"/>
        <source>None</source>
        <translation>Не раскрывать</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>All</source>
        <translation>Всегда</translation>
    </message>
    <message>
        <location filename="LookConfig.ui"/>
        <source>Expand</source>
        <translation>Раскрывать каналы</translation>
    </message>
    <message>
        <location/>
        <source>When to automatically expand channels</source>
        <translation>Когда автоматически раскрывать дерево каналов</translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="-1"/>
        <source>Only with users</source>
        <translation>Только с пользователями</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Ask</source>
        <translation>Спрашивать</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Do Nothing</source>
        <translation>Ничего не делать</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Move</source>
        <translation>Переместить</translation>
    </message>
    <message>
        <location filename="LookConfig.ui"/>
        <source>Channel Dragging</source>
        <translation>Перетаскивание каналов</translation>
    </message>
    <message>
        <location/>
        <source>Ask whether to close or minimize when quitting Mumble.</source>
        <translation>Спрашивать закрыть или свернуть Мамбл при закрытии.</translation>
    </message>
    <message>
        <location/>
        <source>Ask on quit while connected</source>
        <translation>Спрашивать при закрытии</translation>
    </message>
    <message>
        <location/>
        <source>Always On Top</source>
        <translation>Всегда наверху</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;If set, minimizing the Mumble main window will cause it to be hidden and accessible only from the tray. Otherwise, it will be minimized as a window normally would.&lt;/b&gt;</source>
        <translation>&lt;b&gt;Если отмечено, основное окно Mumble будет скрыто при сворачивании и доступно только из системного лотка. Если нет - буте свернуто как обычное окно.&lt;/b&gt;</translation>
    </message>
    <message>
        <location/>
        <source>This sets which channels to automatically expand. &lt;i&gt;None&lt;/i&gt; and &lt;i&gt;All&lt;/i&gt; will expand no or all channels, while &lt;i&gt;Only with users&lt;/i&gt; will expand and collapse channels as users join and leave them.</source>
        <translation>Устанавливает какие каналы автоматически раскрывать. &lt;i&gt;Не раскрывать&lt;/i&gt; и &lt;i&gt;Всегда&lt;/i&gt; - каналы не будут раскрываться автоматически или будут раскрываться все. &lt;i&gt;Только с пользователями&lt;/i&gt; - будут автоматически раскрываться только каналы, в которых есть пользователи.</translation>
    </message>
    <message>
        <location/>
        <source>List users above subchannels (requires restart).</source>
        <translation>Список пользователей над подканалами (требуется перезапуск).</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;If set, users will be shown above subchannels in the channel view.&lt;/b&gt;&lt;br /&gt;A restart of Mumble is required to see the change.</source>
        <translation>&lt;b&gt;Если отмечено, список имен полльзователей будет находится над списком подканалов.&lt;/b&gt;&lt;br /&gt;Требуется перезапуск Mumble.</translation>
    </message>
    <message>
        <location/>
        <source>Users above Channels</source>
        <translation>Пользователи над каналами</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;If set, will verify you want to quit if connected.&lt;/b&gt;</source>
        <translation>&lt;b&gt;Если отмечено, будет запрос при попытке закрыть Мамбл (если Вы подключены к серверу).&lt;/b&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Hide in tray when minimized</source>
        <translation>Сворачивать в трей</translation>
    </message>
    <message>
        <location/>
        <source>Hide the main Mumble window in the tray when it is minimized.</source>
        <translation>Сворачивать Мамбл в системный лоток (трей).</translation>
    </message>
    <message>
        <location/>
        <source>This setting controls when the application will be always on top.</source>
        <translation>Здесь устанавливается будет ли окно мамбл отображаться поверх других окон.</translation>
    </message>
    <message>
        <location/>
        <source>Never</source>
        <translation>Никогда</translation>
    </message>
    <message>
        <location/>
        <source>Always</source>
        <translation>Всегда</translation>
    </message>
    <message>
        <location/>
        <source>In minimal view</source>
        <translation>В простом виде</translation>
    </message>
    <message>
        <location/>
        <source>In normal view</source>
        <translation>В нормальном виде</translation>
    </message>
    <message>
        <location/>
        <source>Displays talking status in system tray</source>
        <translation>Отображать статус разговора в системном лотке</translation>
    </message>
    <message>
        <location/>
        <source>Show talking status in tray icon</source>
        <translation>Показывать статус разговора на значке</translation>
    </message>
    <message>
        <location/>
        <source>This setting controls in which situations the application will stay always on top. If you select &lt;i&gt;Never&lt;/i&gt; the application will not stay on top. &lt;i&gt;Always&lt;/i&gt; will always keep the application on top. &lt;i&gt;In minimal view&lt;/i&gt; / &lt;i&gt;In normal view&lt;/i&gt; will only keep the application always on top when minimal view is activated / deactivated.</source>
        <translation>Здесь устанавливается, когда окно Мамбл будет отображаться поверх всех окон. Если выбрано &lt;i&gt;Никогда&lt;/i&gt; - Мамбл не будет отображаться поверх других окон. &lt;i&gt;Всегда&lt;/i&gt; - всегда будет отображать окно мамбл над другими окнами. &lt;i&gt;В простом виде&lt;/i&gt; / &lt;i&gt;В нормальном виде&lt;/i&gt;- Мамбл будет поверх всех окон в выбранном режиме.</translation>
    </message>
    <message>
        <location/>
        <source>Show context menu in menu bar</source>
        <translation>Контекстное меню в главном меню</translation>
    </message>
    <message>
        <location/>
        <source>Apply some high contrast optimizations for visually impaired users</source>
        <translation>Применить высококонтрастную цветовую схему для людей со слабым зрением</translation>
    </message>
    <message>
        <location/>
        <source>Optimize for high contrast</source>
        <translation>Контрастная цветовая схема</translation>
    </message>
    <message>
        <location/>
        <source>Application</source>
        <translation>Приложение</translation>
    </message>
    <message>
        <location/>
        <source>Adds user and channel context menus into the menu bar</source>
        <translation>Добавить контекстные меню пользователя и канала в главное меню</translation>
    </message>
    <message>
        <location/>
        <source>Tray Icon</source>
        <translation>Системный лоток</translation>
    </message>
    <message>
        <location/>
        <source>Channel Tree</source>
        <translation>Дерево каналов</translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <location filename="MainWindow.cpp" line="+151"/>
        <location line="+1866"/>
        <source>Root</source>
        <translation>Корень</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>&amp;Connect</source>
        <translation>&amp;Подключиться</translation>
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
        <translation>Отключиться от сервера</translation>
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
        <translation>Наложить на себя о&amp;немение</translation>
    </message>
    <message>
        <location/>
        <source>Mute yourself</source>
        <translation>Наложить на себя онемение</translation>
    </message>
    <message>
        <location/>
        <source>Mute or unmute yourself. When muted, you will not send any data to the server. Unmuting while deafened will also undeafen.</source>
        <translation>Накладывает онемение. При включении, Вы не сможете посылать данных на сервер (Вас не будут слышать).</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Deafen Self</source>
        <translation>&amp;Заглушить себя</translation>
    </message>
    <message>
        <location/>
        <source>Deafen yourself</source>
        <translation>Заглушить себя</translation>
    </message>
    <message>
        <location/>
        <source>Deafen or undeafen yourself. When deafened, you will not hear anything. Deafening yourself will also mute.</source>
        <translation>Заглушить себя. При глушении, вы не услышите никого. Глушение также накладывает онемение.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Text-To-Speech</source>
        <translation>&amp;Текст-в-Речь</translation>
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
        <translation>Изменить &amp;комментарий</translation>
    </message>
    <message>
        <location/>
        <source>Configure certificates for strong authentication</source>
        <translation>Настроить сертификаты для надежной авторизации</translation>
    </message>
    <message>
        <location/>
        <source>This starts the wizard for creating, importing and exporting certificates for authentication against servers.</source>
        <translation>Это запуск мастера создания, импорта и экспорта сертификатов для авторизации на серверах.</translation>
    </message>
    <message>
        <location/>
        <source>Register user on server</source>
        <translation>Зарегистрировать пользователя на сервере</translation>
    </message>
    <message>
        <location/>
        <source>This will permanently register the user on the server.</source>
        <translation>Регистрировать пользователя на сервере.</translation>
    </message>
    <message>
        <location/>
        <source>Add &amp;Friend</source>
        <translation>Добавить &amp;друга</translation>
    </message>
    <message>
        <location/>
        <source>Adds a user as your friend.</source>
        <translation>Добавляет пользователя в список друзей.</translation>
    </message>
    <message>
        <location/>
        <source>This will add the user as a friend, so you can recognize him on this and other servers.</source>
        <translation>Добавить пользователя в список друзей, Вы можете распознавать его на этом и других серверах.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Remove Friend</source>
        <translation>&amp;Удалить друга</translation>
    </message>
    <message>
        <location/>
        <source>Removes a user from your friends.</source>
        <translation>Удалить пользователя из списка друзей.</translation>
    </message>
    <message>
        <location/>
        <source>This will remove a user from your friends list.</source>
        <translation>Удалить пользователя из списка друзей.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Update Friend</source>
        <translation>&amp;Обновить друга</translation>
    </message>
    <message>
        <location/>
        <source>Update name of your friend.</source>
        <translation>Обновить имя Вашего друга.</translation>
    </message>
    <message>
        <location/>
        <source>Your friend uses a different name than what is in your database. This will update the name.</source>
        <translation>Ваш друг использует имя, отличное от того, что записано в списке друзей. Здесь можно обновить имя.</translation>
    </message>
    <message>
        <location/>
        <source>Registered &amp;Users</source>
        <translation>Поль&amp;зователи</translation>
    </message>
    <message>
        <location/>
        <source>Edit registered users list</source>
        <translation>Редактировать список зарегистрированных пользователей</translation>
    </message>
    <message>
        <location/>
        <source>This opens the editor for registered users, which allow you to change their name or unregister them.</source>
        <translation>Откыть редактор зарегистрированных пользователей, в котором Вы можете изменить  имя или удалить.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Access Tokens</source>
        <translation>&amp;Маркеры доступа</translation>
    </message>
    <message>
        <location/>
        <source>Add or remove text-based access tokens</source>
        <translation>Добавить или удалить текстовые маркеры доступа</translation>
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
        <location filename="MainWindow.cpp" line="-1772"/>
        <location line="+12"/>
        <location line="+1968"/>
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
        <location filename="MainWindow.cpp" line="-2058"/>
        <source>&amp;Channel</source>
        <translation>&amp;Канал</translation>
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
        <translation>Нажми-и-говори (Push-to-Talk)</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Push and hold this button to send voice.</source>
        <comment>Global Shortcut</comment>
        <translation>Нажать и удерживать эту кнопку, чтобы включить микрофон.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This configures the push-to-talk button, and as long as you hold this button down, you will transmit voice.</source>
        <comment>Global Shortcut</comment>
        <translation>Здесь настраивается кнопка для включения речи, пока Вы ее удерживаете, другие пользователи слышать Вас.</translation>
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
        <translation>Включить Наложение</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Toggle state of in-game overlay.</source>
        <comment>Global Shortcut</comment>
        <translation>Включение внутриигрового наложения.</translation>
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
        <location line="+989"/>
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
        <location line="+192"/>
        <source>Unmuted and undeafened.</source>
        <translation>Онемение и глушение снято.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Unmuted.</source>
        <translation>Онемение снято.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Muted.</source>
        <translation>Наложено онемение.</translation>
    </message>
    <message>
        <location line="+24"/>
        <source>Muted and deafened.</source>
        <translation>В онемении и заглушен(а).</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Deafened.</source>
        <translation>Заглушен(а).</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Undeafened.</source>
        <translation>Глушение снято.</translation>
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
        <location line="-1384"/>
        <source>Reconnecting.</source>
        <translation>Повтор связи.</translation>
    </message>
    <message>
        <source>Joined server: %1.</source>
        <translation type="obsolete">Присединились к серверу: %1.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="+289"/>
        <source>You were unmuted and undeafened by %1.</source>
        <translation>С Вас снял(а) глушение и онемение %1.</translation>
    </message>
    <message>
        <location line="+29"/>
        <source>You muted and deafened %1.</source>
        <translation>Вас сделал(а) немым и заглушил(а) %1.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>You unmuted and undeafened %1.</source>
        <translation>С Вас снял(а) онемение и глушение %1.</translation>
    </message>
    <message>
        <location line="+11"/>
        <source>You undeafened %1.</source>
        <translation>С Вас снял(а) онемение %1.</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>You suppressed %1.</source>
        <translation>Вы удержаны  %1.</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>%1 muted and deafened by %2.</source>
        <translation>%2 наложил(а) онемение и глушение на %1.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1 unmuted and undeafened by %2.</source>
        <translation>%2 снял(а) глушение и онемение с %1.</translation>
    </message>
    <message>
        <location line="+18"/>
        <source>%1 suppressed by %2.</source>
        <translation>%2 поставил(а) %1 на удержание.</translation>
    </message>
    <message>
        <location line="+25"/>
        <source>%1 moved to %2.</source>
        <translation>%1 перешел(а) в %2.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1 moved to %2 by %3.</source>
        <translation>%3 переместил(а) %1 в %2.</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>%1 moved in from %2 by %3.</source>
        <translation>%1 переместил(а) админ %3 из %2.</translation>
    </message>
    <message>
        <source>Left server: %1.</source>
        <translation type="obsolete">Покинут сервер: %1.</translation>
    </message>
    <message>
        <location line="-136"/>
        <source>%1 is now muted and deafened.</source>
        <translation>%1 сейчас в онемении и заглушен(а).</translation>
    </message>
    <message>
        <location line="-109"/>
        <source>You were denied %1 privileges in %2.</source>
        <translation>Вам отказано в привилегиях %1 в(на) %2.</translation>
    </message>
    <message>
        <location line="-61"/>
        <location line="+40"/>
        <source>Welcome message: %1</source>
        <translation>Приветствие: %1</translation>
    </message>
    <message>
        <location line="-7"/>
        <source>Mumble: %1</source>
        <translation>Mumble: %1</translation>
    </message>
    <message>
        <location line="+30"/>
        <source>%3 was denied %1 privileges in %2.</source>
        <translation>%3 были отклонены %1 привилегии в %2.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Denied: Cannot modify SuperUser.</source>
        <translation>Ошибка: невозможно изменить суперпользователя.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Denied: Invalid channel name.</source>
        <translation>Ошибка: неправильное имя канала.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Denied: Text message too long.</source>
        <translation>Ошибка: сообщение слишком длинное.</translation>
    </message>
    <message>
        <location line="+20"/>
        <source>Denied: Operation not permitted in temporary channel.</source>
        <translation>Ошибка: операция запрещена во временном канале.</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>You need a certificate to perform this operation.</source>
        <translation>Вам необходим сертификат для осуществления этих действий.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1 does not have a certificate.</source>
        <translation>У %1 нет сертификата.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Invalid username: %1.</source>
        <translation>Неверное имя пользователя: %1.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Invalid username.</source>
        <translation>Неверное имя пользователя.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Channel is full.</source>
        <translation>Канал переполнен.</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Permission denied.</source>
        <translation>В доступе отказано.</translation>
    </message>
    <message>
        <location line="+37"/>
        <source>%1 connected.</source>
        <translation>%1 вошел(а).</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>%1 is now muted.</source>
        <translation>%1 сейчас заглушен(а).</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1 is now unmuted.</source>
        <translation>С %1 снято глушение.</translation>
    </message>
    <message>
        <location line="+39"/>
        <source>You were unsuppressed.</source>
        <translation>Вы были сняты с удержания.</translation>
    </message>
    <message>
        <location line="+139"/>
        <source>%1 disconnected.</source>
        <translation>%1 вышел(а).</translation>
    </message>
    <message>
        <location line="+87"/>
        <location line="+1"/>
        <source>Server</source>
        <comment>message from</comment>
        <translation>Сервер</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>(Tree) </source>
        <translation>(Дерево) </translation>
    </message>
    <message>
        <location line="+2"/>
        <source>(Channel) </source>
        <translation>(Канал) </translation>
    </message>
    <message>
        <location line="+3"/>
        <source>%2%1: %3</source>
        <translation>%2%1: %3</translation>
    </message>
    <message>
        <source>server</source>
        <translation type="obsolete">сервер</translation>
    </message>
    <message>
        <location line="-251"/>
        <source>You were muted by %1.</source>
        <translation>На Вас наложил(а) онемение %1.</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>You were suppressed.</source>
        <oldsource>You were suppressed by %1.</oldsource>
        <translation>Вы были удержаны.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>You were unsuppressed by %1.</source>
        <translation>С Вас снял(а) удержание %1.</translation>
    </message>
    <message>
        <location line="-16"/>
        <source>You were unmuted by %1.</source>
        <translation>С Вас снял(а) онемение %1.</translation>
    </message>
    <message>
        <location line="+29"/>
        <source>You muted %1.</source>
        <translation>На вас онемение %1.</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>You unsuppressed %1.</source>
        <translation>С Вас снято удержание %1.</translation>
    </message>
    <message>
        <location line="-14"/>
        <source>You unmuted %1.</source>
        <translation>С Вас снято онемение %1.</translation>
    </message>
    <message>
        <location line="+25"/>
        <source>%1 muted by %2.</source>
        <translation>%2 заглушил(а) %1.</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>%1 unsuppressed by %2.</source>
        <translation>%2 снял(а) удержание с %1.</translation>
    </message>
    <message>
        <location line="-14"/>
        <source>%1 unmuted by %2.</source>
        <translation>%2 снял(а) онемение с %1.</translation>
    </message>
    <message>
        <location line="+327"/>
        <source>Unable to find matching CELT codecs with other clients. You will not be able to talk to all users.</source>
        <translation>Не найдены соответствующие CELT-кодеки у других пользователей. Вы не сможете говорить со всеми пользователями.</translation>
    </message>
    <message>
        <location line="-393"/>
        <source>You were muted and deafened by %1.</source>
        <oldsource>You were deafened by %1.</oldsource>
        <translation>%1 наложил(а) на Вас онемение и глушение.</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>You were undeafened by %1.</source>
        <translation>%1 снял(а) с Вас глушение.</translation>
    </message>
    <message>
        <source>%1 deafened by %2.</source>
        <translation type="obsolete">%1 в онемении из-за %2.</translation>
    </message>
    <message>
        <location line="+58"/>
        <source>%1 undeafened by %2.</source>
        <translation>%2 снял(а) глушение с %1.</translation>
    </message>
    <message>
        <location line="+83"/>
        <source>You were kicked from the server by %1: %2.</source>
        <translation>%1 выкинул(а) Вас с сервера: %2.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>%3 was kicked from the server by %1: %2.</source>
        <translation>%1 выкинул(а) с сервера %3: %2.</translation>
    </message>
    <message>
        <location line="-7"/>
        <source>You were kicked and banned from the server by %1: %2.</source>
        <translation>%1 выкинул(а) Вас с сервера и забанил(а): %2.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>%3 was kicked and banned from the server by %1: %2.</source>
        <translation>%1 выкинул(а) %3 с сервера и забанил(а): %2.</translation>
    </message>
    <message>
        <location line="-57"/>
        <source>You were moved to %1 by %2.</source>
        <translation>%2 переместил(а) Вас в %1.</translation>
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
        <translation>%1 вошел(а) в канал.</translation>
    </message>
    <message>
        <source>%1 moved in by %2.</source>
        <translation type="obsolete">%1 перемещен сюда %2.</translation>
    </message>
    <message>
        <location line="-312"/>
        <source>Server connection rejected: %1.</source>
        <translation>Попытка подключения отклонена: %1.</translation>
    </message>
    <message>
        <location line="+128"/>
        <source>Denied: %1.</source>
        <translation>Ошибка: %1.</translation>
    </message>
    <message>
        <location filename="main.cpp" line="-46"/>
        <source>Welcome to Mumble.</source>
        <translation>Добро пожаловать в Mumble.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>&amp;Quit Mumble</source>
        <translation>&amp;Выход</translation>
    </message>
    <message>
        <location/>
        <source>Closes the program</source>
        <translation>Закрывает программу</translation>
    </message>
    <message>
        <location/>
        <source>Exits the application.</source>
        <translation>Выход из Мамбл.</translation>
    </message>
    <message>
        <location/>
        <source>Send a Text Message</source>
        <translation>Послать текстовое сообщение</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="+449"/>
        <source>Sending message to %1</source>
        <translation>Отправка сообщения пользователю %1</translation>
    </message>
    <message>
        <location line="+10"/>
        <location line="+79"/>
        <location line="+4"/>
        <location line="+223"/>
        <source>To %1: %2</source>
        <translation>К %1: %2</translation>
    </message>
    <message>
        <location line="-306"/>
        <location line="+83"/>
        <source>Message to %1</source>
        <translation>Сообщение для %1</translation>
    </message>
    <message>
        <location line="-44"/>
        <source>Are you sure you want to reset the comment of user %1?</source>
        <translation>Вы уверены, что хотите очистить комментарий пользователя %1?</translation>
    </message>
    <message>
        <location line="+265"/>
        <source>To %1 (Tree): %2</source>
        <oldsource>(Tree) %1: %2</oldsource>
        <translation>К %1 (Дерево): %2</translation>
    </message>
    <message>
        <location line="+612"/>
        <source>Failed to launch compatibility client</source>
        <translation>Ошибка при запуске совместимого клиента</translation>
    </message>
    <message>
        <location line="+21"/>
        <source>Invalid username</source>
        <translation>Неверное имя пользователя</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>You connected with an invalid username, please try another one.</source>
        <translation>Вы подключились с неверным именем пользователя, пожалуйста попробуйте другое.</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>That username is already in use, please try another username.</source>
        <translation>Это имя уже занято, пожалуйста, попробуйте другое.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Wrong password</source>
        <translation>Неверный пароль</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Wrong password for registered users, please try again.</source>
        <translation>Неверный пароль для зарегистрированного пользователя, попробуйте снова.</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Wrong server password for unregistered user account, please try again.</source>
        <translation>Неверные пароль сервера, попробуйте еще.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="+329"/>
        <source>Message from %1</source>
        <translation>Сообщение от %1</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>&amp;Audio Wizard</source>
        <translation>Мастер настройки &amp;звука</translation>
    </message>
    <message>
        <location/>
        <source>Sends a text message to another user.</source>
        <translation>Послать текстовое сообщение другому пользователю.</translation>
    </message>
    <message>
        <location/>
        <source>This is the chatbar&lt;br /&gt;If you enter text here and then press enter the text is sent to the user or channel that was selected. If nothing is selected the message is sent to your current channel.</source>
        <translation>Это чат&lt;br /&gt;Если Вы введете здесь текст и нажмете enter - текст будет отправлен пользователю или в канал, который выделен в правом окне. Если ничего не выделено - сообщение будет отправлено в текущий канал.</translation>
    </message>
    <message>
        <location/>
        <source>Chatbar</source>
        <translation>Чат</translation>
    </message>
    <message>
        <location/>
        <source>Shows a dialog of registered servers, and also allows quick-connect.</source>
        <translation>Показывает диалог зарегистрированных пользователей, а также позволяет быстро подключаться.</translation>
    </message>
    <message>
        <location/>
        <source>Kick user (with reason)</source>
        <translation>Выкинуть пользователя (с указанием причины)</translation>
    </message>
    <message>
        <location/>
        <source>Kick selected user off server. You&apos;ll be asked to specify a reason.</source>
        <translation>Выкинуть выбранного пользователя с сервера. Вас попросят указать причину.</translation>
    </message>
    <message>
        <location/>
        <source>Mute user</source>
        <translation>Наложить онемение</translation>
    </message>
    <message>
        <location/>
        <source>Mute or unmute user on server. Unmuting a deafened user will also undeafen them.</source>
        <translation>Наложить или снять онемение с пользователя. Снятие онемения с заглушенного пользователя также снимит и глушение с него.</translation>
    </message>
    <message>
        <location/>
        <source>Kick and ban user (with reason)</source>
        <translation>Выкинуть пользователя и забанить (с указанием причины)</translation>
    </message>
    <message>
        <location/>
        <source>Kick and ban selected user from server. You&apos;ll be asked to specify a reason.</source>
        <translation>Выкинуть и забанить выбранного пользователя с сервера. Вас попросят указать причину.</translation>
    </message>
    <message>
        <location/>
        <source>Deafen user</source>
        <translation>Заглушить</translation>
    </message>
    <message>
        <location/>
        <source>Deafen or undeafen user on server. Deafening a user will also mute them.</source>
        <translation>Наложить или снять глушение с пользователя. Наложение глушения также наложит онемение.</translation>
    </message>
    <message>
        <location/>
        <source>Mute user locally</source>
        <translation>Местное онемение</translation>
    </message>
    <message>
        <location/>
        <source>Mute or unmute user locally. Use this on other users in the same room.</source>
        <translation>Наложить или снять местное онемение с пользователя. Наложение немоты на пользователя в данном канале.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Edit</source>
        <translation>&amp;Изменить</translation>
    </message>
    <message>
        <location/>
        <source>This links your current channel to the selected channel. If users in a channel have permission to speak in the other channel, users can now hear each other. This is a permanent link, and will last until manually unlinked or the server is restarted. Please see the shortcuts for push-to-link.</source>
        <oldsource>This links your current channel to the selected channel. If they have permission to speak in the other channel, users can now hear each other. This is a permanent link, and will last until manually unlinked or the server is restarted. Please see the shortcuts for push-to-link.</oldsource>
        <translation>Это свяжет Ваш текущий канал с выбранным каналом. Если пользователи могут разговаривать в другом канале, то после этого они будут слышать друг друга. Это ручная связь, и будет действовать, пока не будет снято вручную, или после перезагрузки сервера. Пожалуйста, посмотрите комбинацию клавишь для Нажми-и-Говори (push-to-link).</translation>
    </message>
    <message>
        <location/>
        <source>Start the audio configuration wizard</source>
        <translation>Запустить мастер настройки звука</translation>
    </message>
    <message>
        <location/>
        <source>This will guide you through the process of configuring your audio hardware.</source>
        <translation>Проведет Вас через процесс настройки звукового оборудования.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="-90"/>
        <source>SSL Verification failed: %1</source>
        <translation>Проверка SSL неудачна: %1</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>&lt;b&gt;WARNING:&lt;/b&gt; The server presented a certificate that was different from the stored one.</source>
        <translation>&lt;b&gt;ВНИМАНИЕ:&lt;/b&gt; Сервер предоставил сертификат, который отличается от сохраненного.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Sever presented a certificate which failed verification.</source>
        <translation>Сервер представил сертификат, который не удалось проверить.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>&amp;Information</source>
        <translation>&amp;Информация</translation>
    </message>
    <message>
        <location/>
        <source>Show information about the server connection</source>
        <translation>Показать информацию о соединении</translation>
    </message>
    <message>
        <location/>
        <source>This will show extended information about the connection to the server.</source>
        <translation>Показывает расширенную информацию о соединении с сервером.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="-1606"/>
        <source>Opening URL %1</source>
        <translation>Открытие ссылки %1</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>File does not exist</source>
        <translation>Файл не найден</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>File is not a configuration file.</source>
        <translation>Файл не является файлом конфигурации.</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Settings merged from file.</source>
        <translation>Настройки импортированы из файла.</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>URL scheme is not &apos;mumble&apos;</source>
        <translation>Схема ссылки не &apos;mumble&apos;</translation>
    </message>
    <message>
        <location line="+25"/>
        <source>This version of Mumble can&apos;t handle URLs for Mumble version %1.%2.%3</source>
        <translation>Эта версия Мамбл не может использовать ссылки из Мамбл версий %1.%2.%3</translation>
    </message>
    <message>
        <location line="+29"/>
        <source>Connecting to %1</source>
        <translation>Соединение с %1</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Enter username</source>
        <translation>Введите имя пользователя</translation>
    </message>
    <message>
        <location line="+18"/>
        <location line="+180"/>
        <source>Connecting to server %1.</source>
        <translation>Соединение с сервером %1.</translation>
    </message>
    <message>
        <location line="+48"/>
        <source>Change your comment</source>
        <translation>Изменить свой комментарий</translation>
    </message>
    <message>
        <location line="+92"/>
        <source>&lt;h2&gt;Version&lt;/h2&gt;&lt;p&gt;Protocol %1.%2.%3.&lt;/p&gt;</source>
        <translation>&lt;h2&gt;Версия&lt;/h2&gt;&lt;p&gt;Протокол %1.%2.%3.&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>&lt;p&gt;No build information or OS version available.&lt;/p&gt;</source>
        <translation>&lt;p&gt;Нет информации о сборке или версии OS.&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>&lt;p&gt;%1 (%2)&lt;br /&gt;%3&lt;/p&gt;</source>
        <translation>&lt;p&gt;%1 (%2)&lt;br /&gt;%3&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+20"/>
        <source>&lt;h2&gt;Voice channel&lt;/h2&gt;&lt;p&gt;Encrypted with 128 bit OCB-AES128&lt;br /&gt;%1 ms average latency (%4 deviation)&lt;/p&gt;</source>
        <translation>&lt;h2&gt;Голосовой канал&lt;/h2&gt;&lt;p&gt;Зашифрован 128 bit OCB-AES128&lt;br /&gt;%1 мс средняя задержка (%4 отклонение)&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+11"/>
        <source>&lt;h2&gt;Audio bandwidth&lt;/h2&gt;&lt;p&gt;Maximum %1 kbit/s&lt;br /&gt;Current %2 kbit/s&lt;/p&gt;</source>
        <translation>&lt;h2&gt;Пропускная способность&lt;/h2&gt;&lt;p&gt;Максимум %1 кбит/с&lt;br /&gt;Текущая %2 кбит/с&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Mumble Server Information</source>
        <translation>Информация о сервере Мамбл</translation>
    </message>
    <message>
        <location line="+4"/>
        <location line="+1158"/>
        <source>&amp;View Certificate</source>
        <translation>Просмотр &amp;Сертификата</translation>
    </message>
    <message>
        <location line="+253"/>
        <source>Images (*.png *.jpg *.svg)</source>
        <translation>Картинки (*.png *.jpg *.svg)</translation>
    </message>
    <message>
        <location line="-1521"/>
        <location line="+306"/>
        <source>Register yourself as %1</source>
        <translation>Зарегистрировать себя как %1</translation>
    </message>
    <message>
        <location line="-946"/>
        <source>This will switch the states of the in-game overlay.</source>
        <comment>Global Shortcut</comment>
        <translation>Переключает состояние внутиигрового наложения.</translation>
    </message>
    <message>
        <location line="+640"/>
        <location line="+306"/>
        <source>&lt;p&gt;You are about to register yourself on this server. This action cannot be undone, and your username cannot be changed once this is done. You will forever be known as &apos;%1&apos; on this server.&lt;/p&gt;&lt;p&gt;Are you sure you want to register yourself?&lt;/p&gt;</source>
        <translation>&lt;p&gt;Вы собираетесь зарегистрировать себя на сервере. Это действие нельзя отменить, и после этого выбранное имя нельзя будет изменить. Вы всегда будете известны как &apos;%1&apos; на этом сервере.&lt;/p&gt;&lt;p&gt;Вы уверены, что хотите зарегистрироваться?&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Register user %1</source>
        <translation>Зарегистрировать пользователя %1</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>&lt;p&gt;You are about to register %1 on the server. This action cannot be undone, the username cannot be changed, and as a registered user, %1 will have access to the server even if you change the server password.&lt;/p&gt;&lt;p&gt;From this point on, %1 will be authenticated with the certificate currently in use.&lt;/p&gt;&lt;p&gt;Are you sure you want to register %1?&lt;/p&gt;</source>
        <translation>&lt;p&gt;Вы собираетесь зарегистрировать пользователя %1 на сервере. Это действие нельзя отменить, имя пользователя нельзя будет изменить, а пользователь %1 будет иметь доступ на сервер, даже если Вы измените пароль сервера.&lt;/p&gt;&lt;p&gt;С этого момента, %1 будет авторизироваться с сертификатом, который у него в данный момент имеется.&lt;/p&gt;&lt;p&gt;Вы уверены, что хотите зарегистрировать %1?&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+40"/>
        <source>Kicking user %1</source>
        <translation>Выброс пользователяr %1</translation>
    </message>
    <message>
        <location line="+19"/>
        <source>Banning user %1</source>
        <translation>Бан пользователя %1</translation>
    </message>
    <message>
        <location line="+51"/>
        <source>View comment on user %1</source>
        <translation>Посмотреть комментарий пользователя %1</translation>
    </message>
    <message>
        <location line="+56"/>
        <location line="+227"/>
        <source>Message to channel %1</source>
        <translation>Сообщение в канал %1</translation>
    </message>
    <message>
        <location line="+437"/>
        <source>Connected.</source>
        <translation>Подключено.</translation>
    </message>
    <message>
        <location line="+147"/>
        <source>SSL Version mismatch</source>
        <translation>Версия SSL не совпадает</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>This server is using an older encryption standard. It might be an older 1.1 based Mumble server.&lt;br /&gt;Would you like to launch the compatibility client to connect to it?</source>
        <translation>Этот сервер использует устаревший стандарт шифрования. Возможно это устаревший сервер версии 1.1.&lt;br /&gt;Хотите запустить клиент совместимости для соединения?</translation>
    </message>
    <message>
        <location line="+26"/>
        <source>The compatibility client could not be found, or failed to start.&lt;br /&gt;Note that the compatibility client is an optional component for most installations, and might not be installed.</source>
        <translation>Клиент совместимости не найден или произошла ошибка при его запуске.&lt;br /&gt;Также возможно что клиент совместимости не установлен.</translation>
    </message>
    <message>
        <location line="-50"/>
        <source>&lt;p&gt;%1.&lt;br /&gt;The specific errors with this certificate are: &lt;/p&gt;&lt;ol&gt;%2&lt;/ol&gt;&lt;p&gt;Do you wish to accept this certificate anyway?&lt;br /&gt;(It will also be stored so you won&apos;t be asked this again.)&lt;/p&gt;</source>
        <translation>&lt;p&gt;%1.&lt;br /&gt;Ошибки в сертификате: &lt;/p&gt;&lt;ol&gt;%2&lt;/ol&gt;&lt;p&gt;Вы хотите принять этот сертификат?&lt;br /&gt;(Он также будет сохранен, так что этот вопрос не будет больше задан.)&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+166"/>
        <source>&lt;center&gt;Type message to channel &apos;%1&apos; here&lt;/center&gt;</source>
        <oldsource>Type message to channel &apos;%1&apos; here</oldsource>
        <translation>&lt;center&gt;Напечатайте здесь сообщение в канал &apos;%1&apos;&lt;/center&gt;</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>&lt;center&gt;Type message to user &apos;%1&apos; here&lt;/center&gt;</source>
        <oldsource>Type message to user &apos;%1&apos; here</oldsource>
        <translation>&lt;center&gt;Напечатайте здесь сообщение пользователю &apos;%1&apos;&lt;/center&gt;</translation>
    </message>
    <message>
        <location line="+91"/>
        <source>Choose image file</source>
        <translation>Выбрать файл картинки</translation>
    </message>
    <message>
        <location line="+7"/>
        <location line="+17"/>
        <source>Failed to load image</source>
        <translation>Ошибка при загрузке картинки</translation>
    </message>
    <message>
        <location line="-17"/>
        <source>Could not open file for reading.</source>
        <translation>Невозможно открыть файл для чтения.</translation>
    </message>
    <message>
        <location line="+17"/>
        <source>Image format not recognized.</source>
        <translation>Формат изображения не распознан.</translation>
    </message>
    <message>
        <location line="-1454"/>
        <source>Voice channel is sent over control channel.</source>
        <translation>Голосовой канал послан через канал управления.</translation>
    </message>
    <message>
        <location line="-797"/>
        <source>&amp;User</source>
        <translation>&amp;Пользователь</translation>
    </message>
    <message>
        <location line="+788"/>
        <source>&lt;h2&gt;Control channel&lt;/h2&gt;&lt;p&gt;Encrypted with %1 bit %2&lt;br /&gt;%3 ms average latency (%4 deviation)&lt;/p&gt;&lt;p&gt;Remote host %5 (port %6)&lt;/p&gt;</source>
        <translation>&lt;h2&gt;Контрольный канал&lt;/h2&gt;&lt;p&gt;Шифрование %1 бит %2&lt;br /&gt;%3 мс средняя задержка (%4 отклонение)&lt;/p&gt;&lt;p&gt;Хост %5 (порт %6)&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+18"/>
        <source>UDP Statistics</source>
        <translation>UDP Статистика</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>To Server</source>
        <translation>На сервер</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>From Server</source>
        <translation>От Сервера</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Good</source>
        <translation>Хороших</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Late</source>
        <translation>Поздних</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Lost</source>
        <translation>Потерь</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Resync</source>
        <translation>Ресинхр</translation>
    </message>
    <message>
        <location line="+590"/>
        <source>Sending message to channel %1</source>
        <translation>Отправка сообщения в канал %1</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Message to tree %1</source>
        <translation>Сообщение в дерево каналов %1</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>Ctrl+Q</source>
        <translation>Ctrl+Q</translation>
    </message>
    <message>
        <location/>
        <source>About &amp;Qt</source>
        <translation>О &amp;Qt</translation>
    </message>
    <message>
        <location/>
        <source>Sends a text message to all users in a channel.</source>
        <translation>Послать текстовое сообщение всем пользователям в канале.</translation>
    </message>
    <message>
        <location filename="main.cpp" line="+13"/>
        <source>This is the first time you&apos;re starting Mumble.&lt;br /&gt;Would you like to go through the Audio Wizard to configure your soundcard?</source>
        <translation>Это первый запуск Mumble.&lt;br /&gt;Желаете ли Вы пройти мастер настройки звука?</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="-1209"/>
        <source>Mumble is currently connected to a server. Do you want to Close or Minimize it?</source>
        <translation>В настоящий момент Вы подключены к серверу. Хотите Закрыть или Свернуть Мамбл?</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Close</source>
        <translation>Закрыть</translation>
    </message>
    <message>
        <location line="-100"/>
        <location line="+101"/>
        <source>Minimize</source>
        <translation>Свернуть</translation>
    </message>
    <message>
        <location line="-154"/>
        <source>Mute Self</source>
        <comment>Global Shortcut</comment>
        <translation>Сделать себя немым</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Set self-mute status.</source>
        <comment>Global Shortcut</comment>
        <translation>Наложить онемение на себя.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This will set or toggle your muted status. If you turn this off, you will also disable self-deafen.</source>
        <comment>Global Shortcut</comment>
        <translation>Это устанавливает или снимает онемение с Вас. Если включить - Вы также будете заглушены.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Deafen Self</source>
        <comment>Global Shortcut</comment>
        <translation>Заглушить себя</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Set self-deafen status.</source>
        <comment>Global Shortcut</comment>
        <translation>Устанавливает глушение на себя.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This will set or toggle your deafened status. If you turn this on, you will also enable self-mute.</source>
        <comment>Global Shortcut</comment>
        <translation>Это наложет или снимет с Вас глушение. Если включить - Вы также включите немоту.</translation>
    </message>
    <message>
        <location line="+30"/>
        <source>Whisper/Shout</source>
        <oldsource>Whisper</oldsource>
        <translation>Шепнуть/Крикнуть</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>&amp;Window</source>
        <translation>&amp;Окно</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Ctrl+M</source>
        <translation>Ctrl+M</translation>
    </message>
    <message>
        <location line="-28"/>
        <source>Toggle Minimal</source>
        <comment>Global Shortcut</comment>
        <translation>Включить простой режим</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>&amp;Minimal View</source>
        <translation>&amp;Простой вид</translation>
    </message>
    <message>
        <location/>
        <source>Toggle minimal window modes</source>
        <translation>Включает простой режим для окна</translation>
    </message>
    <message>
        <location/>
        <source>This will toggle minimal mode, where the log window and menu is hidden.</source>
        <translation>Это включит простой режим, при котором окно лога и меню будут скрыты.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="+3"/>
        <source>Volume Up (+10%)</source>
        <comment>Global Shortcut</comment>
        <translation>Увеличить громкость (+10%)</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Volume Down (-10%)</source>
        <comment>Global Shortcut</comment>
        <translation>Уменьшить громкость (-10%)</translation>
    </message>
    <message>
        <location line="+65"/>
        <location line="+1987"/>
        <source>&lt;center&gt;Not connected&lt;/center&gt;</source>
        <oldsource>Not connected</oldsource>
        <translation>&lt;center&gt;Нет соединения с сервером&lt;/center&gt;</translation>
    </message>
    <message>
        <location line="-1779"/>
        <source>Clear</source>
        <translation>Очистить</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>Log</source>
        <translation>Лог</translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="+1378"/>
        <source>You have Channel Dragging set to &quot;Do Nothing&quot; so the channel wasn&apos;t moved.</source>
        <translation>Установлен режин &quot;не перетаскивать&quot; в настройках канала.</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Unknown Channel Drag mode in UserModel::dropMimeData.</source>
        <translation>Неизвестный режим перетаскивания канала в UserModel::dropMimeData.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>&amp;Unlink</source>
        <comment>Channel</comment>
        <translation>&amp;Разорвать связь</translation>
    </message>
    <message>
        <source>&amp;Unlink</source>
        <comment>Plugin</comment>
        <translation type="obsolete">&amp;Разорвать связь</translation>
    </message>
    <message>
        <location/>
        <source>Hide Frame</source>
        <translation>Спрятать рамку</translation>
    </message>
    <message>
        <location/>
        <source>Toggle showing frame on minimal window</source>
        <translation>Переключает показ рамки в простом режиме</translation>
    </message>
    <message>
        <location/>
        <source>This will toggle whether the minimal window should have a frame for moving and resizing.</source>
        <translation>Переключает, будет ли рамка у окна программы в простом режиме для перемещения и изменения размера.</translation>
    </message>
    <message>
        <location/>
        <source>Send &amp;Message</source>
        <translation>&amp;Послать сообщение</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Unlink All</source>
        <translation>Отменить &amp;все связи</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Certificate Wizard</source>
        <translation>Мастер &amp;сертификатов</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Register</source>
        <translation>За&amp;регистрировать</translation>
    </message>
    <message>
        <location/>
        <source>Reset &amp;Comment</source>
        <translation>Очистить &amp;комментарий</translation>
    </message>
    <message>
        <location/>
        <source>Reset the comment of the selected user.</source>
        <translation>Очистить комментарий у выбранного пользователя.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Join Channel</source>
        <translation>&amp;Войти в канал</translation>
    </message>
    <message>
        <location/>
        <source>View Comment</source>
        <translation>Просмотреть комментарий</translation>
    </message>
    <message>
        <location/>
        <source>View comment in editor</source>
        <translation>Просмотреть комментарий в редакотре</translation>
    </message>
    <message>
        <location/>
        <source>Query server for connection information for user</source>
        <translation>Запрос сервера о соединении пользователя</translation>
    </message>
    <message>
        <location/>
        <source>S&amp;erver</source>
        <translation>&amp;Сервер</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Self</source>
        <translation>&amp;Пользователь</translation>
    </message>
    <message>
        <location/>
        <source>Audio S&amp;tatistics</source>
        <translation>Ау&amp;дио Статистика</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Unlink Plugins</source>
        <translation>О&amp;тключить плагины</translation>
    </message>
    <message>
        <location/>
        <source>R&amp;egister</source>
        <translation>За&amp;регистрировать</translation>
    </message>
    <message>
        <location/>
        <source>Register yourself on the server</source>
        <translation>Зарегистрировать себя на сервере</translation>
    </message>
    <message>
        <location/>
        <source>Change &amp;Avatar</source>
        <translation>Изменить &amp;Аватар</translation>
    </message>
    <message>
        <location/>
        <source>Change your avatar image on this server</source>
        <translation>Изменить картинку аватара на сервере</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Remove Avatar</source>
        <translation>&amp;Удалить Аватар</translation>
    </message>
    <message>
        <location/>
        <source>Remove currently defined avatar image.</source>
        <translation>Удалить текущую картинку аватара.</translation>
    </message>
    <message>
        <location/>
        <source>Icon Toolbar</source>
        <translation>Панель инструментов</translation>
    </message>
    <message>
        <location/>
        <source>Change your own comment</source>
        <translation>Изменить свой комментарий</translation>
    </message>
    <message>
        <location/>
        <source>Priority speaker</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>NetworkConfig</name>
    <message>
        <location filename="NetworkConfig.cpp" line="+53"/>
        <source>Network</source>
        <translation>Сеть</translation>
    </message>
    <message>
        <location line="+36"/>
        <source>Updates are mandatory when using snapshot releases.</source>
        <translation>Обновления обязательны при использовании тестовых релизов.</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui"/>
        <source>Form</source>
        <translation>Форма</translation>
    </message>
    <message>
        <location/>
        <source>Connection</source>
        <translation>Подключение</translation>
    </message>
    <message>
        <location/>
        <source>Use TCP compatibility mode</source>
        <oldsource>Use TCP compatability mode</oldsource>
        <translation>Использовать режим совместимости TCP</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Enable TCP compatibility mode&lt;/b&gt;.&lt;br /&gt;This will make Mumble use only TCP when communicating with the server. This will increase overhead and cause lost packets to produce noticeable pauses in communication, so this should only be used if you are unable to use the default (which uses UDP for voice and TCP for control).</source>
        <translation>&lt;b&gt;Включит режим совместимости TCP&lt;/b&gt;.&lt;br /&gt;Mumble будет использовать только TCP при соединении с сервером. Это увеличит нагрузку и вызовет потерю пакетов что создаст заметные паузы в соединении. Может быть использовано только если Вы не можете использовать протоколы по умолчанию (UDP для голоса и TCP для управляющего канала).</translation>
    </message>
    <message>
        <source>Use TCP mode</source>
        <translation type="obsolete">Использовать режим TCP</translation>
    </message>
    <message>
        <location/>
        <source>Reconnect when disconnected</source>
        <translation>Восстановление связи при обрыве</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Reconnect when disconnected&lt;/b&gt;.&lt;br /&gt;This will make Mumble try to automatically reconnect after 10 seconds if your server connection fails.</source>
        <translation>&lt;b&gt;Восстановить связь при обрыве&lt;/b&gt;.&lt;br /&gt;Mumble будет пытаться автоматически восстановить связь через 10 в случае обрыва связи с сервером.</translation>
    </message>
    <message>
        <location/>
        <source>Reconnect automatically</source>
        <translation>Автоматичекое переподключение</translation>
    </message>
    <message>
        <location/>
        <source>Proxy</source>
        <translation>Прокси</translation>
    </message>
    <message>
        <location/>
        <source>Type</source>
        <translation>Тип</translation>
    </message>
    <message>
        <location/>
        <source>Type of proxy to connect through</source>
        <translation>Тип прокси, черз который будет осуществляться соединение</translation>
    </message>
    <message>
        <location/>
        <source>Direct connection</source>
        <translation>Прямое соединение</translation>
    </message>
    <message>
        <location/>
        <source>HTTP(S) proxy</source>
        <translation>HTTP(S) прокси</translation>
    </message>
    <message>
        <location/>
        <source>SOCKS5 proxy</source>
        <translation>SOCKS5 прокси</translation>
    </message>
    <message>
        <location/>
        <source>Hostname</source>
        <translation>Адрес</translation>
    </message>
    <message>
        <location/>
        <source>Hostname of the proxy</source>
        <translation>Адрес прокси сервера</translation>
    </message>
    <message>
        <location/>
        <source>Force TCP mode</source>
        <translation>Усиленный режим TCP</translation>
    </message>
    <message>
        <location/>
        <source>Enable QoS to prioritize packets</source>
        <translation>Включить QoS для назначения приоритета пакетам</translation>
    </message>
    <message>
        <location/>
        <source>This will enable QoS, which will attempt to prioritize voice packets over other traffic.</source>
        <translation>Это включит QoS, который будет пытаться назначить голосовым пакетам более высокий приоритет для минимизации задержек.</translation>
    </message>
    <message>
        <location/>
        <source>Use Quality of Service</source>
        <translation>Использовать QoS</translation>
    </message>
    <message>
        <location/>
        <source>Don&apos;t send certificate to server and don&apos;t save passwords. (Not saved).</source>
        <translation>Не отсылать сертификат на сервер и не сохранять пароли. (Не сохраненные).</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This will suppress identity information from the client.&lt;/b&gt;&lt;p&gt;The client will not identify itself with a certificate, even if defined, and will not cache passwords for connections. This is primarily a test-option and is not saved.&lt;/p&gt;</source>
        <translation>&lt;b&gt;Это отключит отпрваку идентификационной информации на сервер.&lt;/b&gt;&lt;p&gt;Клиент не будет идентифицировать себя с помощью сертификата (даже если таковой имеется), и не будет кэшировать пароли для соединений. Это гтестовая опция, она не сохраняется.&lt;/p&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Suppress certificate and password storage</source>
        <translation>Не хранить сертификаты и пароли</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Type of proxy to connect through.&lt;/b&gt;&lt;br /&gt;This makes Mumble connect through a proxy for all outgoing connections. Note: Proxy tunneling forces Mumble into TCP compatibility mode, causing all voice data to be sent via the control channel.</source>
        <translation>&lt;b&gt;Тип прокси сервера.&lt;/b&gt;&lt;br /&gt;Использовать прокси-сервер для всех исходящих соединений. Внимание: туннелирование через прокси заставит Мамбл подключаться через режим совместимости TCP, голосовые данные будут отправляться через управляющий канал.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Hostname of the proxy.&lt;/b&gt;&lt;br /&gt;This field specifies the hostname of the proxy you wish to tunnel network traffic through.</source>
        <translation>&lt;b&gt;Арес прокси-сервера.&lt;/b&gt;&lt;br /&gt;Здесь указывается адрес прокси-сервера, через который Вы хотите соединяться.</translation>
    </message>
    <message>
        <location/>
        <source>Port</source>
        <translation>Порт</translation>
    </message>
    <message>
        <location/>
        <source>Port number of the proxy</source>
        <translation>Номер порта прокси-сервера</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Port number of the proxy.&lt;/b&gt;&lt;br /&gt;This field specifies the port number that the proxy expects connections on.</source>
        <translation>&lt;b&gt;Номер порта прокси-сервера.&lt;/b&gt;&lt;br /&gt;Здесь указывается порт на который прокси-сервер ожидает входящие соединения.</translation>
    </message>
    <message>
        <location/>
        <source>Username</source>
        <translation>Логин</translation>
    </message>
    <message>
        <location/>
        <source>Username for proxy authentication</source>
        <translation>Логин для авторизации на прокси-сервере</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Username for proxy authentication.&lt;/b&gt;&lt;br /&gt;This specifies the username you use for authenticating yourself with the proxy. In case the proxy does not use authentication, or you want to connect anonymously, simply leave this field blank.</source>
        <translation>&lt;b&gt;Логин для авторизации на прокси-сервере&lt;/b&gt;&lt;br /&gt;Здесь указывается имя пользователя для авторизации на прокси. Если прокси не поддерживает авторизацию, или Вы хотите подключиться анонимно - оставьте поле пустым.</translation>
    </message>
    <message>
        <location/>
        <source>Password</source>
        <translation>Пароль</translation>
    </message>
    <message>
        <location/>
        <source>Password for proxy authentication</source>
        <translation>Пароль для авторизации на прокси-сервере</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Password for proxy authentication.&lt;/b&gt;&lt;br /&gt;This specifies the password you use for authenticating yourself with the proxy. In case the proxy does not use authentication, or you want to connect anonymously, simply leave this field blank.</source>
        <translation>&lt;b&gt;Пароль для авторизации на прокси-сервере&lt;/b&gt;&lt;br /&gt;Здесь указывается пароль для авторизации на прокси. Если прокси не поддерживает авторизацию, или Вы хотите подключиться анонимно - оставьте поле пустым.</translation>
    </message>
    <message>
        <location/>
        <source>Misc</source>
        <translation>Разное</translation>
    </message>
    <message>
        <location/>
        <source>Prevent log from downloading images</source>
        <translation>Отключить вставку картинок в сообщениях</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Disable image download&lt;/b&gt;&lt;br/&gt;
Prevents the client from downloading images embedded into chat messages with the img tag.</source>
        <translation>&lt;b&gt;Отключить загрузку картинок&lt;/b&gt;&lt;br/&gt;
Отключает возможность вставлять картинки в сообщения чата с помощью тега &apos;img&apos;.</translation>
    </message>
    <message>
        <location/>
        <source>Disable image download</source>
        <translation>Отключить загрузку картинок</translation>
    </message>
    <message>
        <location/>
        <source>Mumble services</source>
        <translation>Службы Мамбл</translation>
    </message>
    <message>
        <location/>
        <source>Check for new releases of Mumble automatically.</source>
        <translation>Автоматически проверять обновления Мамбл.</translation>
    </message>
    <message>
        <location/>
        <source>This will check for new releases of Mumble every time you start the program, and notify you if one is available.</source>
        <translation>Мабл будет проверять обновления при каждом старте программы, и уведомлять, если доступно.</translation>
    </message>
    <message>
        <location/>
        <source>Check for application updates on startup</source>
        <translation>Проверять обновления при запуске</translation>
    </message>
    <message>
        <location/>
        <source>Check for new releases of plugins automatically.</source>
        <translation>Проверять обновления плагинов автоматически.</translation>
    </message>
    <message>
        <location/>
        <source>This will check for new releases of plugins every time you start the program, and download them automatically.</source>
        <translation>Мамбл будет проверять обновления плагинов каждый раз при запуске программы.</translation>
    </message>
    <message>
        <location/>
        <source>Download plugin updates on startup</source>
        <translation>Загружать обновления плагинов при запуске</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Submit anonymous statistics.&lt;/b&gt;&lt;br /&gt;Mumble has a small development team, and as such needs to focus its development where it is needed most. By submitting a bit of statistics you help the project determine where to focus development.</source>
        <translation>&lt;b&gt;Отправлять анонимную статистику.&lt;/b&gt;&lt;br /&gt;У Mumble есть небольшая группа разработчиков, которой нужно сосредоточить усилия на наиболее необходимых пользователям, функциях. Отправляя статистику, Вы помогаете проекту определять на что обратить внимание при разработке.</translation>
    </message>
    <message>
        <location/>
        <source>Submit anonymous statistics to the Mumble project</source>
        <translation>Отправлять анонимную статистику проекту Мамбл</translation>
    </message>
    <message>
        <location/>
        <source>Submit anonymous statistics</source>
        <translation>Отправлять анонимную статистику</translation>
    </message>
    <message>
        <location/>
        <source>Reconnect to last used server when starting Mumble</source>
        <translation>Подключаться к последнему серверу при запуске Мамбл</translation>
    </message>
    <message>
        <location/>
        <source>Reconnect to last server on startup</source>
        <translation>Подключаться к последнему серверу при запуске</translation>
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
        <location filename="Overlay.cpp" line="+424"/>
        <location line="+1076"/>
        <source>Silent</source>
        <translation>Тихо</translation>
    </message>
    <message>
        <location line="-1073"/>
        <location line="+1076"/>
        <source>Talking</source>
        <translation>Разговор</translation>
    </message>
    <message>
        <location line="-1073"/>
        <location line="+1076"/>
        <source>Whisper</source>
        <translation>Шепот</translation>
    </message>
    <message>
        <location line="-1073"/>
        <location line="+1076"/>
        <source>Shout</source>
        <translation>Крик</translation>
    </message>
    <message>
        <location line="-1057"/>
        <location line="+947"/>
        <source>Channel</source>
        <translation>Канал</translation>
    </message>
    <message>
        <location line="+1259"/>
        <source>Failed to create communication with overlay at %2: %1. No overlay will be available.</source>
        <translation>Не возможно соединиться с замещением на %2: %1. Замещение не будет доступно.</translation>
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
    <name>OverlayClient</name>
    <message>
        <location line="-865"/>
        <source>Filter</source>
        <translation>Фильтр</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Only talking</source>
        <translation>Только кто говорит</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>All in current channel</source>
        <translation>Все из этого канала</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>All in linked channels</source>
        <translation>Все из связанных каналов</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Always show yourself</source>
        <translation>Всегда показывать себя</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Columns</source>
        <translation>Колонок</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Edit...</source>
        <translation>Изменить...</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Reset Zoom</source>
        <translation>Сбросить масштаб</translation>
    </message>
    <message>
        <location line="+293"/>
        <source>FPS: %1</source>
        <translation type="unfinished"></translation>
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
        <source>Position</source>
        <translation type="obsolete">Позиция</translation>
    </message>
    <message>
        <location/>
        <source>Form</source>
        <oldsource>Font</oldsource>
        <translation>Шрифт</translation>
    </message>
    <message>
        <location/>
        <source>Enable Overlay</source>
        <translation>Включить Наложение</translation>
    </message>
    <message>
        <location/>
        <source>Enable overlay.</source>
        <translation>Включить наложение.</translation>
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
        <source>Who to show on the overlay</source>
        <translation type="obsolete">Кого показывать при замещении</translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets who to show in the in-game overlay.&lt;/b&gt;&lt;br /&gt;If many people are connected to the same channel, the overlay list might be very long. Use this to shorten it.&lt;br /&gt;&lt;i&gt;No one&lt;/i&gt; - Don&apos;t show anyone (but leave overlay running).&lt;br /&gt;&lt;i&gt;Only talking&lt;/i&gt; - Only show talking people.&lt;br /&gt;&lt;i&gt;Everyone&lt;/i&gt; - Show everyone.</source>
        <translation type="obsolete">&lt;b&gt;Определяет, кого показывать в игровом замещении.&lt;/b&gt;&lt;br /&gt;Если много людей на одном канале, замещение может быть очень долгим. Используйте это, чтобы сократить его.&lt;br /&gt;&lt;i&gt;Никого&lt;/i&gt; -Не показывать никого (но оставить замещение включенным).&lt;br /&gt;&lt;i&gt;Только говорящих&lt;/i&gt; - Показывать только говорящих людей.&lt;br /&gt;&lt;i&gt;Всех&lt;/i&gt; - Показывать всех.</translation>
    </message>
    <message>
        <source>Always Show Self</source>
        <translation type="obsolete">Всегда показывать меня</translation>
    </message>
    <message>
        <source>Always show yourself on overlay.</source>
        <translation type="obsolete">Всегда показывать себя в замещении.</translation>
    </message>
    <message>
        <source>This sets whether to always show yourself or not. This setting is useful if you aren&apos;t showing everyone in the overlay, as then you would only see your own status if you were talking, which wouldn&apos;t let you see that you were deafened or muted.</source>
        <translation type="obsolete">Устанавливает, всегда ли показывать себя. Настройка применима если вы не отображаете всех в замещении, поэтому тогда вы увидите собственный статус при разговоре, который вы не услышите, если в онемении или заглушены.</translation>
    </message>
    <message>
        <source>Grow Left</source>
        <translation type="obsolete">Расти влево</translation>
    </message>
    <message>
        <source>Let overlay grow to the left</source>
        <translation type="obsolete">Позволяет замещению расти влево</translation>
    </message>
    <message>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow to the left if needed.</source>
        <translation type="obsolete">Замещение пытается остаться как можно меньшим и на выбранной позиции. Позволяет замещению расти влево при необходимости.</translation>
    </message>
    <message>
        <source>Grow Right</source>
        <translation type="obsolete">Расти вправо</translation>
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
        <source>Grow Up</source>
        <translation type="obsolete">Расти вверх</translation>
    </message>
    <message>
        <source>Let overlay grow upwards</source>
        <translation type="obsolete">Позволяет замещению расти вверх</translation>
    </message>
    <message>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow to the top if needed.</source>
        <translation type="obsolete">Замещение попытается оставться как можно меньшим и на установленой позиции. Позволяет замещению расти вверх при необходимости.</translation>
    </message>
    <message>
        <source>Grow Down</source>
        <translation type="obsolete">Расти вниз</translation>
    </message>
    <message>
        <source>Let overlay grow downwards</source>
        <translation type="obsolete">Позволяет замещению расти вниз</translation>
    </message>
    <message>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow towards the bottom if needed.</source>
        <translation type="obsolete">Замещение попытается оставться как можно меньшим и на установленой позиции. Позволяет замещению расти вниз при необходимости.</translation>
    </message>
    <message>
        <source>X-Position of Overlay</source>
        <translation type="obsolete">Позиция Х замещения</translation>
    </message>
    <message>
        <source>This sets the relative X position of the overlay.</source>
        <translation type="obsolete">Устанавливает относительную Х позицию замещения.</translation>
    </message>
    <message>
        <source>Y-Position of Overlay</source>
        <translation type="obsolete">Y-позиция замещения</translation>
    </message>
    <message>
        <source>This sets the relative Y position of the overlay.</source>
        <translation type="obsolete">Устанавливает относительную Y - позицию замещения.</translation>
    </message>
    <message>
        <source>Current Font</source>
        <translation type="obsolete">Текущий шрифт</translation>
    </message>
    <message>
        <source>Set Font</source>
        <translation type="obsolete">Установить шрифт</translation>
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
        <source>Change</source>
        <translation type="obsolete">Изменить</translation>
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
        <source>Color for Channels</source>
        <translation type="obsolete">Цвет для каналов</translation>
    </message>
    <message>
        <source>Color for active Channels</source>
        <translation type="obsolete">Цвет для Активных каналов</translation>
    </message>
    <message>
        <location filename="Overlay.cpp" line="-2040"/>
        <source>To move the users, drag the little red dot.</source>
        <translation>Чтобы переместить список, перетащите красный кружок.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>To resize the users, mousewheel over a user.</source>
        <translation>Чтобы изменть размер пользователей - покрутите колесом мыши над ними.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>For more options, rightclick a user.</source>
        <translation>Для других настроек правый клик на пользователе.</translation>
    </message>
    <message>
        <location line="+147"/>
        <source>Overlay</source>
        <translation>Наложение</translation>
    </message>
    <message>
        <location line="+62"/>
        <source>Choose executable</source>
        <translation>Выберите выполняемый файл</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Choose application</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Overlay.ui"/>
        <source>This sets whether the overlay is enabled or not. This setting is only checked when applications are started, so make sure Mumble is running and this option is on before you start the application.&lt;br /&gt;Please note that if you start Mumble after starting the application, or if you disable the overlay while the application is running, there is no safe way to restart the overlay without also restarting the application.</source>
        <oldsource>This sets whether the overlay is enabled or not. This setting is only checked when applications are started, so make sure Mumble is running and this option is on before you start the application.&lt;br /&gt;Please note that if you start the application after starting Mumble, or if you disable the overlay while running, there is no safe way to restart the overlay without also restarting the application.</oldsource>
        <translation>Здесь можно включить или отключить Наложение. При включенном Наложении Мамбл должен быть запущен перед приложением (игрой), в котором оно необходимо..&lt;br /&gt;Если Мамбл будет запущен после другого приложения (игры) или Вы отключите эту опцию, когда приложение запущено, необходимо будет перезапустить приложение (игру) для включения или отключения наложения.</translation>
    </message>
    <message>
        <location/>
        <source>Layout</source>
        <translation>Размещение</translation>
    </message>
    <message>
        <location/>
        <source>Overlay exceptions</source>
        <translation>Исключения</translation>
    </message>
    <message>
        <location/>
        <source>Blacklist</source>
        <translation>Черный список</translation>
    </message>
    <message>
        <location/>
        <source>Whitelist</source>
        <translation>Белый список</translation>
    </message>
    <message>
        <location/>
        <source>Add</source>
        <translation>Добавить</translation>
    </message>
    <message>
        <location/>
        <source>Remove</source>
        <translation>Убрать</translation>
    </message>
    <message>
        <location/>
        <source>Uninstall Overlay</source>
        <translation>Удалить Наложение</translation>
    </message>
    <message>
        <location/>
        <source>Overlay Installation</source>
        <translation>Установка Наложения</translation>
    </message>
    <message>
        <location/>
        <source>Mumble has detected that you do not have the Mumble Overlay installed.

Click the button below to install the overlay.</source>
        <translation>Mumble обнаружил, что поддержка Наложения не установлена.

Нажмите на кнопку, чтобы установить Наложение.</translation>
    </message>
    <message>
        <location/>
        <source>Install Mumble Overlay</source>
        <translation>Установить налоожение Mumble</translation>
    </message>
    <message>
        <location/>
        <source>Overlay Upgrade</source>
        <translation>Обновление Наложения</translation>
    </message>
    <message>
        <location/>
        <source>Mumble has detected an old version of the overlay support files installed on your computer.

To upgrade these files to their latest versions, click the button below.</source>
        <translation>Mumble обнаружил старую версию Наложения.

Чтобы обновить, нажмите на кнопку.</translation>
    </message>
    <message>
        <location/>
        <source>Upgrade Mumble Overlay</source>
        <translation>Обновить Наложение Мамбл</translation>
    </message>
    <message>
        <location/>
        <source>Display a frame counter in the overlay</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Show FPS counter</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>OverlayEditor</name>
    <message>
        <source>Dialog</source>
        <translation type="obsolete">Диалог</translation>
    </message>
    <message>
        <location filename="OverlayEditor.ui"/>
        <source>State</source>
        <translation>Состояние</translation>
    </message>
    <message>
        <location/>
        <source>User is not talking</source>
        <translation>Когда пользователь молчит</translation>
    </message>
    <message>
        <location/>
        <source>Passive</source>
        <translation>Молчит</translation>
    </message>
    <message>
        <location/>
        <source>User is talking in your channel or a linked channel</source>
        <translation>Пользователь говорит в Вашем канале или связанном канале</translation>
    </message>
    <message>
        <location/>
        <source>Talking</source>
        <translation>Говорит</translation>
    </message>
    <message>
        <location/>
        <source>User is whispering to you privately</source>
        <translation>Пользователь шепчет Вам</translation>
    </message>
    <message>
        <location/>
        <source>Private Whisper</source>
        <translation>Шепчет</translation>
    </message>
    <message>
        <location/>
        <source>User is shouting to your channel</source>
        <translation>Пользователь кричит в Ваш канал</translation>
    </message>
    <message>
        <location/>
        <source>Channel Whisper</source>
        <translation>Кричит</translation>
    </message>
    <message>
        <location/>
        <source>Zoom</source>
        <translation>Размер</translation>
    </message>
    <message>
        <location/>
        <source>Zoom Factor</source>
        <translation>Размер</translation>
    </message>
    <message>
        <location/>
        <source>Enabled Elements</source>
        <translation>Элементы</translation>
    </message>
    <message>
        <location/>
        <source>User avatar, chosen by each user</source>
        <translation>Аватар пользователя</translation>
    </message>
    <message>
        <location/>
        <source>Avatar</source>
        <translation>Аватар</translation>
    </message>
    <message>
        <location/>
        <source>User&apos;s name</source>
        <translation>Имя пользователя</translation>
    </message>
    <message>
        <location/>
        <source>Username</source>
        <translation>Имя</translation>
    </message>
    <message>
        <location/>
        <source>Name of user&apos;s channel, if outside your current channel</source>
        <translation>Название канал пользователя, если он не в Вашем текущем канале</translation>
    </message>
    <message>
        <location/>
        <source>Channel</source>
        <translation>Канал</translation>
    </message>
    <message>
        <location/>
        <source>Muted or deafened</source>
        <translation>Заглушен или нем</translation>
    </message>
    <message>
        <location/>
        <source>Mute state</source>
        <translation>Глушение</translation>
    </message>
    <message>
        <location/>
        <source>Bounding box, automatically shrunk to minimum size to contain all visible elements</source>
        <translation>Окружающая рамка, в которой будут находится все видимые элементы</translation>
    </message>
    <message>
        <location/>
        <source>Bounding box</source>
        <translation>Рамка</translation>
    </message>
    <message>
        <location/>
        <source>Overlay Editor</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>OverlayEditorScene</name>
    <message>
        <location filename="Overlay.cpp" line="+555"/>
        <source>Layout preset</source>
        <translation>Стиль представления</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Large square avatar</source>
        <translation>Большой квадратный аватар</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Avatar and Name</source>
        <translation>Аватар и Имя</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>User Opacity</source>
        <translation>Прозрачность пользователя</translation>
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
        <translation>Прозрачность объекта</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Alignment</source>
        <translation>Выравнивание</translation>
    </message>
    <message>
        <location line="+11"/>
        <source>Left</source>
        <translation>Слева</translation>
    </message>
    <message>
        <location line="+5"/>
        <location line="+18"/>
        <source>Center</source>
        <translation>По центру</translation>
    </message>
    <message>
        <location line="-13"/>
        <source>Right</source>
        <translation>Справа</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Top</source>
        <translation>По верху</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Bottom</source>
        <translation>По низу</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Color...</source>
        <translation>Цвет...</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Font...</source>
        <translation>Шрифт...</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Bounding box</source>
        <translation>Рамка</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Pen width</source>
        <translation>Толщина линии</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Padding</source>
        <translation>Отступы</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Pen color</source>
        <translation>Цвет линии</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Fill color</source>
        <translation>Цвет заливки</translation>
    </message>
    <message>
        <location line="+87"/>
        <source>Pick pen color</source>
        <translation>Выбрать цвет линии</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Pick fill color</source>
        <translation>Выбрать цвет заливки</translation>
    </message>
    <message>
        <location line="+14"/>
        <source>Pick color</source>
        <translation>Выбрать цвет</translation>
    </message>
    <message>
        <location line="+21"/>
        <source>Pick font</source>
        <translation>Выбрать шрифт</translation>
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
        <translation>Ошибка при загрузке библиотеки Наложения. Это означает:
- либо библиотека (mumble_ol.dll) не была найдена в рабочем каталоге Mumble
- либо Ваша операционная система старее, чем WinXP SP2</translation>
    </message>
</context>
<context>
    <name>OverlayUser</name>
    <message>
        <source>%1%</source>
        <translation type="obsolete">%1%</translation>
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
        <translation>Это позволяет плагинам для поддеживаемых игр определять Ваше положение в игре и передавать с каждым голосовым пакетом. Это позволит другим игрокам слышать Ваш голос в игре, как будто он исходит от Вашего персонажа.</translation>
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
        <translation>Форма</translation>
    </message>
    <message>
        <location/>
        <source>Name</source>
        <translation>Имя</translation>
    </message>
    <message>
        <location/>
        <source>Enabled</source>
        <translation>Включен</translation>
    </message>
</context>
<context>
    <name>Plugins</name>
    <message>
        <location filename="Plugins.cpp" line="+319"/>
        <source>Skipping plugin update in debug mode.</source>
        <translation>Не обновляет плагин в режиме оталдки.</translation>
    </message>
    <message>
        <location line="+155"/>
        <location line="+6"/>
        <source>Downloaded new or updated plugin to %1.</source>
        <translation>Загрузить новый или обновленный плагин в %1.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Failed to install new plugin to %1.</source>
        <translation>Ошибка при установке нового плагина в %1.</translation>
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
        <translation>Устройство по умолчанию</translation>
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
        <translation>Вход по умолчанию</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Default Output</source>
        <translation>Выход по умолчанию</translation>
    </message>
</context>
<context>
    <name>RichTextEditor</name>
    <message>
        <location filename="RichTextEditor.cpp" line="+241"/>
        <source>Failed to load image</source>
        <translation>Ошибка при загрузке картинки</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Image file too large to embed in document. Please use images smaller than %1 kB.</source>
        <translation>Файл картинки слишком большой. Пожалуйста, используйте картинки менее %1 кб.</translation>
    </message>
    <message>
        <location line="+81"/>
        <source>Message is too long.</source>
        <translation>Сообщение слишком длинное.</translation>
    </message>
    <message>
        <location filename="RichTextEditor.ui"/>
        <source>Display</source>
        <translation>Просмотр</translation>
    </message>
    <message>
        <location/>
        <source>Source Text</source>
        <translation>Исходный код</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Bold</source>
        <translation>&amp;Жирный</translation>
    </message>
    <message>
        <location/>
        <source>Ctrl+B</source>
        <translation></translation>
    </message>
    <message>
        <location/>
        <source>&amp;Italic</source>
        <translation>&amp;Курсив</translation>
    </message>
    <message>
        <location/>
        <source>Italic</source>
        <translation>Курсив</translation>
    </message>
    <message>
        <location/>
        <source>Ctrl+I</source>
        <translation></translation>
    </message>
    <message>
        <location/>
        <source>Underline</source>
        <translation>Подчеркнутый</translation>
    </message>
    <message>
        <location/>
        <source>Ctrl+U</source>
        <translation></translation>
    </message>
    <message>
        <location/>
        <source>Color</source>
        <translation>Цвет</translation>
    </message>
    <message>
        <location/>
        <source>Insert Link</source>
        <translation>Вставить ссылку</translation>
    </message>
    <message>
        <location/>
        <source>Ctrl+L</source>
        <translation></translation>
    </message>
    <message>
        <location/>
        <source>Insert Image</source>
        <translation>Вставить картинку</translation>
    </message>
</context>
<context>
    <name>RichTextEditorLink</name>
    <message>
        <location filename="RichTextEditorLink.ui"/>
        <source>Add Link</source>
        <translation>Добавить Ссылку</translation>
    </message>
    <message>
        <location/>
        <source>URL</source>
        <translation>URL</translation>
    </message>
    <message>
        <location/>
        <source>Text</source>
        <translation>Текст</translation>
    </message>
</context>
<context>
    <name>ServerHandler</name>
    <message>
        <location filename="ServerHandler.cpp" line="+394"/>
        <source>UDP packets cannot be sent to or received from the server. Switching to TCP mode.</source>
        <translation>Пакеты UDP не могут быть отправлены на сервер и получены от сервера. Переключение в режим TCP.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>UDP packets cannot be sent to the server. Switching to TCP mode.</source>
        <translation>UDP пакеты не могут быть отправлены на сервер. Переключение в режим TCP.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>UDP packets cannot be received from the server. Switching to TCP mode.</source>
        <translation>Пакеты UDP не могут быть получены от сервера. Переключение в режим TCP.</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>UDP packets can be sent to and received from the server. Switching back to UDP mode.</source>
        <translation>Пакеты UDP могут быть отправлены на сервер и получены от сервера. Переключение на режим UDP.</translation>
    </message>
    <message>
        <location line="+29"/>
        <source>Connection timed out</source>
        <translation>Время соединения с сервером истекло</translation>
    </message>
</context>
<context>
    <name>ServerView</name>
    <message>
        <location filename="ConnectDialog.cpp" line="-664"/>
        <source>Favorite</source>
        <translation>Закладки</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>LAN</source>
        <translation>Локальная сеть (LAN)</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Public Internet</source>
        <translation>Интернет</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Africa</source>
        <translation>Африка</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Asia</source>
        <translation>Азия</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>North America</source>
        <translation>Северная Америка</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>South America</source>
        <translation>Южная Америка</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Europe</source>
        <translation>Европа</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Oceania</source>
        <translation>Океания</translation>
    </message>
</context>
<context>
    <name>ShortcutActionWidget</name>
    <message>
        <location filename="GlobalShortcut.cpp" line="-484"/>
        <source>Unassigned</source>
        <translation>Не назначено</translation>
    </message>
</context>
<context>
    <name>ShortcutDelegate</name>
    <message>
        <location line="+360"/>
        <source>On</source>
        <translation>Включить</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Off</source>
        <translation>Выключить</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Toggle</source>
        <translation>Переключить</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Unassigned</source>
        <translation>Не назначено</translation>
    </message>
</context>
<context>
    <name>ShortcutKeyWidget</name>
    <message>
        <location line="-434"/>
        <source>Press Shortcut</source>
        <translation>Нажмите клавишу</translation>
    </message>
</context>
<context>
    <name>ShortcutTargetDialog</name>
    <message>
        <location line="+203"/>
        <source>Root</source>
        <translation>Корень</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Parent</source>
        <translation>Родительский</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Current</source>
        <translation>Текущий</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Subchannel #%1</source>
        <translation>Подканал №%1</translation>
    </message>
</context>
<context>
    <name>ShortcutTargetWidget</name>
    <message>
        <location line="+87"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location line="+38"/>
        <source>, </source>
        <translation>, </translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Root</source>
        <translation>Корень</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Parent</source>
        <translation>Родительский</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Current</source>
        <translation>Текущий</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Subchannel #%1</source>
        <translation>Подканал №%1</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Invalid</source>
        <translation>Не верный</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>&lt;Empty&gt;</source>
        <translation>&lt;Пусто&gt;</translation>
    </message>
</context>
<context>
    <name>ShortcutToggleWidget</name>
    <message>
        <location line="-261"/>
        <source>Off</source>
        <translation>Выключить</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Toggle</source>
        <translation>Переключить</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>On</source>
        <translation>Включить</translation>
    </message>
</context>
<context>
    <name>TextMessage</name>
    <message>
        <location filename="TextMessage.h" line="+46"/>
        <source>Enter text</source>
        <translation>Введите текст</translation>
    </message>
    <message>
        <location filename="TextMessage.ui"/>
        <source>If checked the message is recursively sent to all subchannels</source>
        <translation>Если отмечено, сообщение будет отправлено во все подканалы</translation>
    </message>
    <message>
        <location/>
        <source>Send recursively to subchannels</source>
        <translation>Отправить в подканалы</translation>
    </message>
</context>
<context>
    <name>Tokens</name>
    <message>
        <location filename="Tokens.cpp" line="+69"/>
        <source>Empty Token</source>
        <translation>Пустой маркер</translation>
    </message>
    <message>
        <location filename="Tokens.ui"/>
        <source>Mumble - Access Tokens</source>
        <translation>Mumble - Маркеры Доступа</translation>
    </message>
    <message>
        <location/>
        <source>List of access tokens on current server</source>
        <translation>Список маркеров доступа на текущем сервере</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is an editable list of access tokens on the connected server.&lt;/b&gt;
&lt;br /&gt;
An access token is a text string, which can be used as a password for very simple access management on channels. Mumble will remember the tokens you&apos;ve used and resend them to the server next time you reconnect, so you don&apos;t have to enter these every time.
</source>
        <translation>&lt;b&gt;Это редкатируемый список маркеров доступа для текущего сервера.&lt;/b&gt;
&lt;br /&gt;
Маркер доступа - это текст, который будет использован как пароль для простого управления доступом в каналы. Мамбл запомнит маркеры, которые Вы ввели и отправит их заново на сервер при следующем подключении, так что Вам не потребуется вводить их снова.</translation>
    </message>
    <message>
        <location/>
        <source>Add a token</source>
        <translation>Добавить маркер</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Add</source>
        <translation>&amp;Добавить</translation>
    </message>
    <message>
        <location/>
        <source>Remove a token</source>
        <translation>Удалить маркер</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Remove</source>
        <translation>&amp;Удалить</translation>
    </message>
</context>
<context>
    <name>UserEdit</name>
    <message>
        <location filename="UserEdit.ui"/>
        <source>Registered Users</source>
        <translation>Зарегистрированные пользователи</translation>
    </message>
    <message>
        <location/>
        <location filename="UserEdit.cpp" line="+96"/>
        <source>Remove</source>
        <translation>Удалить</translation>
    </message>
    <message>
        <location filename="UserEdit.cpp" line="-5"/>
        <source>Rename</source>
        <translation>Переименовать</translation>
    </message>
</context>
<context>
    <name>UserInformation</name>
    <message>
        <location filename="UserInformation.ui"/>
        <source>User Information</source>
        <translation>Информация о пользователе</translation>
    </message>
    <message>
        <location/>
        <source>Connection Information</source>
        <translation>Соединение</translation>
    </message>
    <message>
        <location/>
        <source>Version</source>
        <translation>Версия</translation>
    </message>
    <message>
        <location/>
        <source>OS</source>
        <translation>OS</translation>
    </message>
    <message>
        <location/>
        <source>Certificate</source>
        <translation>Сертификат</translation>
    </message>
    <message>
        <location/>
        <source>IP Address</source>
        <translation>IP Адрес</translation>
    </message>
    <message>
        <location/>
        <source>CELT Versions</source>
        <translation>Версии CELT</translation>
    </message>
    <message>
        <location/>
        <source>Details...</source>
        <translation>Детали...</translation>
    </message>
    <message>
        <location/>
        <source>Ping Statistics</source>
        <translation>Статистика пинга</translation>
    </message>
    <message>
        <location/>
        <source>Pings received</source>
        <translation>Пинг</translation>
    </message>
    <message>
        <location/>
        <source>Average ping</source>
        <translation>Средний пинг</translation>
    </message>
    <message>
        <location/>
        <source>TCP (Control)</source>
        <translation>TCP (Управляющий)</translation>
    </message>
    <message>
        <location/>
        <source>UDP (Voice)</source>
        <translation>UDP (Голосовой)</translation>
    </message>
    <message>
        <location/>
        <source>UDP Network statistics</source>
        <translation>Статистика UDP</translation>
    </message>
    <message>
        <location/>
        <source>Good</source>
        <translation>Хороших</translation>
    </message>
    <message>
        <location/>
        <source>Late</source>
        <translation>Поздних</translation>
    </message>
    <message>
        <location/>
        <source>Lost</source>
        <translation>Потерянных</translation>
    </message>
    <message>
        <location/>
        <source>Resync</source>
        <translation>Ресинхр</translation>
    </message>
    <message>
        <location/>
        <source>From Client</source>
        <translation>От клиента</translation>
    </message>
    <message>
        <location/>
        <source>To Client</source>
        <translation>Клиенту</translation>
    </message>
    <message>
        <location/>
        <source>Connection time</source>
        <translation>Время соединения</translation>
    </message>
    <message>
        <location filename="UserInformation.cpp" line="+93"/>
        <source>%1w</source>
        <translation>%1нед</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1d</source>
        <translation>%1д</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1h</source>
        <translation>%1ч</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1m</source>
        <translation>%1мин</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>%1s</source>
        <translation>%1с</translation>
    </message>
    <message>
        <location line="+29"/>
        <location line="+43"/>
        <source>, </source>
        <translation>, </translation>
    </message>
    <message>
        <location line="-13"/>
        <source>%1.%2.%3 (%4)</source>
        <translation>%1.%2.%3 (%4)</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>%1 (%2)</source>
        <translation>%1 (%2)</translation>
    </message>
    <message>
        <location line="+45"/>
        <source>%1 online (%2 idle)</source>
        <translation>%1 подключен (%2 не активен)</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1 online</source>
        <translation>%1 подключен</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>%1 kbit/s</source>
        <translation>%1 кбит/с</translation>
    </message>
    <message>
        <location filename="UserInformation.ui"/>
        <source>Bandwidth</source>
        <comment>GroupBox</comment>
        <translation>Полоса пропускания</translation>
    </message>
    <message>
        <location/>
        <source>Bandwidth</source>
        <comment>Label</comment>
        <translation>Полоса пропускания</translation>
    </message>
</context>
<context>
    <name>UserModel</name>
    <message>
        <location filename="UserModel.cpp" line="-804"/>
        <source>This is a user connected to the server. The icon to the left of the user indicates whether or not they are talking:</source>
        <translation>Это пользователь, подключенный к серверу. Иконка слева от имени обозначает, говорит ли он в данный момент:</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Talking to your channel.</source>
        <translation>Говорит в Ваш канал.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Shouting directly to your channel.</source>
        <oldsource>Whispering directly to your channel.</oldsource>
        <translation>Кричит напрямую в Ваш канал.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Whispering directly to you.</source>
        <translation>Шепчет прямо Вам.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Not talking.</source>
        <translation>Не говорит.</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>This is a channel on the server. The icon indicates the state of the channel:</source>
        <translation>Это канал. Иконка обозначает состояние канала:</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Your current channel.</source>
        <translation>Ваш текущий канал.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>A channel that is linked with your channel. Linked channels can talk to each other.</source>
        <translation>Канал, связанный с Вашим каналом. Пользователи из связанных каналов слышат друг друга.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>A channel on the server that you are not linked to.</source>
        <translation>Канал на сервере, с которым не связан Ваш канал.</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>This shows the flags the user has on the server, if any:</source>
        <translation>Флажки обозначают, что пользователь:</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>On your friend list</source>
        <translation>В Вашем списке друзей</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Authenticated user</source>
        <translation>Зарегистрированный пользователь</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Muted (manually muted by self)</source>
        <translation>Нем (вручную наложил немоту на себя)</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Muted (manually muted by admin)</source>
        <translation>Нем (вручную наложена немота админом)</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Muted (not allowed to speak in current channel)</source>
        <translation>Нем (не может говорить в текущий канал)</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Muted (muted by you, only on your machine)</source>
        <translation>Нем (заглушен Вами только на Вашем компьютере)</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Deafened (by self)</source>
        <translation>Глух (отключил звук)</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Deafened (by admin)</source>
        <translation>Глух (заглушен админом)</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>User has a new comment set (click to show)</source>
        <translation>У пользователя новый комментарий (кликните, чтобы увидеть)</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>User has a comment set, which you&apos;ve already seen. (click to show)</source>
        <translation>У пользователя комментарий, который Вы уже видели (кликните, чтобы увидеть)</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>This shows the flags the channel has, if any:</source>
        <translation>Здесь показаны флаги для канала:</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Channel has a new comment set (click to show)</source>
        <translation>У канала новый комментарий (кликните чтобы увидеть)</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Channel has a comment set, which you&apos;ve already seen. (click to show)</source>
        <translation>У канала комментарий, который Вы уже видели (кликните чтобы увидеть)</translation>
    </message>
    <message>
        <location line="+18"/>
        <source>Name</source>
        <translation>Имя</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Flags</source>
        <translation>Флаги</translation>
    </message>
    <message>
        <source>Mumble</source>
        <translation type="obsolete">Mumble</translation>
    </message>
    <message>
        <location line="+721"/>
        <source>Are you sure you want to drag this channel?</source>
        <translation>Вы уверены, что хотите перетащить этот канал?</translation>
    </message>
    <message>
        <location line="+93"/>
        <location line="+20"/>
        <source>Cannot perform this movement automatically, please reset the numeric sorting indicators or adjust it manually.</source>
        <translation>Не возможно автоматически переместить, пожалуйста, снимите нумерацию каналов, или настройте вручную.</translation>
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
        <translation>Обновить Mumble</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>A new version of Mumble has been detected and automatically downloaded. It is recommended that you either upgrade to this version, or downgrade to the latest stable release. Do you want to launch the installer now?</source>
        <translation>Вышла новая версия Mumble, которая была автоматически загружена. Рекомендуется обновить эту версию, или установить последний стабильный релиз. Хотите запустить программу установки сейчас?</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Failed to launch snapshot installer.</source>
        <translation>Ошибка при запуске установщика тестовой сборки.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Corrupt download of new version detected. Automatically removed.</source>
        <translation>Обнаружена поврежденная при загрузке версия. Автоматически удалена.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Downloading new snapshot from %1 to %2</source>
        <translation>Загружается новая сборка из %1 в %2</translation>
    </message>
    <message>
        <location line="+18"/>
        <source>Failed to write new version to disc.</source>
        <translation>Невозможно записать новую версию на диск.</translation>
    </message>
    <message>
        <location line="+17"/>
        <source>Mumble failed to retrieve version information from the central server.</source>
        <translation>Mumble не удалось получить информацию об обновлениях с центрального сервера.</translation>
    </message>
</context>
<context>
    <name>ViewCert</name>
    <message>
        <location filename="ViewCert.cpp" line="+41"/>
        <source>Certificate Chain Details</source>
        <translation>Детали цепочки сертификатов</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Certificate chain</source>
        <translation>Цепочка сертификатов</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Certificate details</source>
        <translation>Детали сертификата</translation>
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
        <translation>Имя: %1</translation>
    </message>
    <message>
        <location line="-27"/>
        <location line="+28"/>
        <source>Organization: %1</source>
        <translation>Организация: %1</translation>
    </message>
    <message>
        <location line="-27"/>
        <source>Subunit: %1</source>
        <translation>Подразделелие: %1</translation>
    </message>
    <message>
        <location line="+1"/>
        <location line="+28"/>
        <source>Country: %1</source>
        <translation>Страна: %1</translation>
    </message>
    <message>
        <location line="-27"/>
        <location line="+28"/>
        <source>Locality: %1</source>
        <translation>Регион: %1</translation>
    </message>
    <message>
        <location line="-27"/>
        <location line="+28"/>
        <source>State: %1</source>
        <translation>Штат: %1</translation>
    </message>
    <message>
        <location line="-27"/>
        <source>Valid from: %1</source>
        <translation>Действует с: %1</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Valid to: %1</source>
        <translation>Истекает: %1</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Serial: %1</source>
        <translation>Серийный номер: %1</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Public Key: %1 bits %2</source>
        <translation>Публичный ключ: %1 бит %2</translation>
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
        <translation>Контрольная сумма (MD5): %1</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Email: %1</source>
        <translation>Email: %1</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>DNS: %1</source>
        <translation>DNS: %1</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Issued by:</source>
        <translation>Кем выдан:</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Unit Name: %1</source>
        <translation>Отдел: %1</translation>
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
        <translation>Устройство по умолчанию</translation>
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
