<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.0" language="pl">
<context>
    <name>ACLEditor</name>
    <message>
        <location filename="ACLEditor.ui" line="404"/>
        <source>Active ACLs</source>
        <translation>Aktywne ACL</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="506"/>
        <source>Context</source>
        <translation>Kontekst</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="547"/>
        <source>User/Group</source>
        <translation>Użytkownik / Grupa</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="572"/>
        <source>This controls which group of users this entry applies to.&lt;br /&gt;Note that the group is evaluated in the context of the channel the entry is used in. For example, the default ACL on the Root channel gives &lt;i&gt;Write&lt;/i&gt; permission to the &lt;i&gt;admin&lt;/i&gt; group. This entry, if inherited by a channel, will give a user write privileges if he belongs to the &lt;i&gt;admin&lt;/i&gt; group in that channel, even if he doesn&apos;t belong to the &lt;i&gt;admin&lt;/i&gt; group in the channel where the ACL originated.&lt;br /&gt;If a group name starts with a &apos;!&apos;, its membership is negated, and if it starts with a &apos;~&apos;, it is evaluated in the channel the ACL was defined in, rather than the channel the ACL is active in.&lt;br /&gt;If a group name starts with a  &apos;#&apos;, it is interpreted as an access token. Users must have entered whatever follows the &apos;#&apos; in their list of access tokens to match. This can be used for very simple password access to channels for non-authenticated users.&lt;br /&gt;If a group name starts with a  &apos;$&apos;, it will only match users whose certificate hash matches what follows the &apos;$&apos;.&lt;br /&gt;A few special predefined groups are:&lt;br /&gt;&lt;b&gt;all&lt;/b&gt; - Everyone will match.&lt;br /&gt;&lt;b&gt;auth&lt;/b&gt; - All authenticated users will match.&lt;br /&gt;&lt;b&gt;sub,a,b,c&lt;/b&gt; - User currently in a sub-channel minimum &lt;i&gt;a&lt;/i&gt; common parents, and between &lt;i&gt;b&lt;/i&gt; and &lt;i&gt;c&lt;/i&gt; channels down the chain. See the website for more extensive documentation on this one.&lt;br /&gt;&lt;b&gt;in&lt;/b&gt; - Users currently in the channel will match (convenience for &apos;&lt;i&gt;sub,0,0,0&lt;/i&gt;&apos;).&lt;br /&gt;&lt;b&gt;out&lt;/b&gt; - Users outside the channel will match (convenience for &apos;&lt;i&gt;!sub,0,0,0&lt;/i&gt;&apos;).&lt;br /&gt;Note that an entry applies to either a user or a group, not both.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="619"/>
        <source>Permissions</source>
        <translation>Uprawnienia</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="125"/>
        <location filename="ACLEditor.ui" line="553"/>
        <source>Group</source>
        <translation>Grupa</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="238"/>
        <location filename="ACLEditor.ui" line="244"/>
        <source>Members</source>
        <translation>Członkowie</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="471"/>
        <source>&amp;Add</source>
        <translation>&amp;Dodaj</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="487"/>
        <source>&amp;Remove</source>
        <translation>&amp;Usuń</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="439"/>
        <source>&amp;Up</source>
        <translation>W &amp;górę</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="455"/>
        <source>&amp;Down</source>
        <translation>W &amp;dół</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="426"/>
        <source>Inherit ACLs</source>
        <translation>Dziedzicz reguły ACL</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="531"/>
        <source>Applies to this channel</source>
        <translation>Stosowana do tego kanału</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="518"/>
        <source>Applies to sub-channels</source>
        <translation>Stosowana do tego pod-kanału</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="582"/>
        <source>User ID</source>
        <translation>ID Użytkownika</translation>
    </message>
    <message>
        <location filename="ACLEditor.cpp" line="111"/>
        <source>Deny</source>
        <translation>Zabroń</translation>
    </message>
    <message>
        <location filename="ACLEditor.cpp" line="113"/>
        <source>Allow</source>
        <translation>Zezwól</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="183"/>
        <location filename="ACLEditor.ui" line="361"/>
        <location filename="ACLEditor.ui" line="368"/>
        <source>Remove</source>
        <translation>Usuń</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="199"/>
        <source>Inherit</source>
        <translation>Dziedzicz</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="212"/>
        <source>Inheritable</source>
        <translation>Dziedziczny</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="228"/>
        <source>Inherited</source>
        <translation>Odziedziczony</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="170"/>
        <location filename="ACLEditor.ui" line="322"/>
        <location filename="ACLEditor.ui" line="354"/>
        <source>Add</source>
        <translation>Dodaj</translation>
    </message>
    <message>
        <source>Add to Remove</source>
        <translation type="obsolete">Dodaj do usunięcia</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="119"/>
        <source>&amp;Groups</source>
        <translation>&amp;Grupy</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="396"/>
        <source>&amp;ACL</source>
        <translation>&amp;ACL</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="410"/>
        <source>List of entries</source>
        <translation>Lista reguł</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="413"/>
        <source>This shows all the entries active on this channel. Entries inherited from parent channels will be show in italics.&lt;br /&gt;ACLs are evaluated top to bottom, meaning priority increases as you move down the list.</source>
        <translation>Pokazuje aktywną listę reguł na tym kanale. Pozycje odziedziczone z kanałów nadrzędnych wyświetlone są przy użyciu kursywy.&lt;br /&gt; Lista ACL przetwarzana jest od góry do dołu, co oznacza, że pozycje znajdujące się na dole mają wyższy priorytet.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="420"/>
        <source>Inherit ACL of parent?</source>
        <translation>Dziedziczyć reguły ACL z kanałów nadrzędnych?</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="423"/>
        <source>This sets whether or not the ACL up the chain of parent channels are applied to this object. Only those entries that are marked in the parent as &quot;Apply to sub-channels&quot; will be inherited.</source>
        <translation>Ustawia czy reguły ACL mają być dziedziczone z kanałów nadrzędnych. Tylko te reguły, które są oznaczone jako rodzic &quot;Dotyczy pod-kanałów&quot; będą przejmowane.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="465"/>
        <source>Add new entry</source>
        <translation>Dodaj nową regułę</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="468"/>
        <source>This adds a new entry, initially set with no permissions and applying to all.</source>
        <translation>Dodaje nową regułę, która początkowo nie posiada uprawnień oraz jest zastosowana dla wszystkich.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="481"/>
        <source>Remove entry</source>
        <translation>Usuń regułę</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="484"/>
        <source>This removes the currently selected entry.</source>
        <translation>Usuwa aktualnie wybraną regułę.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="433"/>
        <source>Move entry up</source>
        <translation>Przenieś regułę w górę</translation>
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
        <translation type="unfinished">Hasło</translation>
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
        <translation type="unfinished">Pozycja nakładki na ekranie</translation>
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
        <location filename="ACLEditor.ui" line="436"/>
        <source>This moves the entry up in the list. As entries are evaluated in order, this may change the effective permissions of users. You cannot move an entry above an inherited entry, if you really need that you&apos;ll have to duplicate the inherited entry.</source>
        <translation>Przenosi regułę w górę listy. Reguły są sprawdzane w kolejności, więc taka zmiana może mieć wpływ na uprawnienia użytkowników. Nie możesz przenieść raguły ponad regulę z której są dziedziczone ACL, jeżeli naprawdę tego potrzebujesz zduplikuj daną regułę.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="449"/>
        <source>Move entry down</source>
        <translation>Przenieś regułę w dół</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="452"/>
        <source>This moves the entry down in the list. As entries are evaluated in order, this may change the effective permissions of users.</source>
        <translation>Przenosi regułę w dół listy. Reguły są sprawdzane w kolejności, więc taka zmiana może mieć wpływ na uprawnienia użytkowników.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="525"/>
        <source>Entry should apply to this channel.</source>
        <oldsource>Entry should apply to this channel</oldsource>
        <translatorcomment>slicer nie wstawil kropki jak poprawi to nie bedzie na pomaranczowo //Zuko</translatorcomment>
        <translation>Reguła powinna być zastosowana do tego kanału.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="528"/>
        <source>This makes the entry apply to this channel.</source>
        <translation>Sprawia, że reguła dotyczy tego kanału.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="512"/>
        <source>Entry should apply to sub-channels.</source>
        <translation>Reguła powinna być zastosowana do tego pod-kanału.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="515"/>
        <source>This makes the entry apply to sub-channels of this channel.</source>
        <translation>Sprawia, że reguła dotyczy pod-kanałów tego kanału.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="569"/>
        <source>Group this entry applies to</source>
        <translation>Grupa tych reguł dotyczy</translation>
    </message>
    <message>
        <source>This controls which group of users this entry applies to.&lt;br /&gt;Note that the group is evaluated in the context of the channel the entry is used in. For example, the default ACL on the Root channel gives &lt;i&gt;Write&lt;/i&gt; permission to the &lt;i&gt;admin&lt;/i&gt; group. This entry, if inherited by a channel, will give a user write privileges if he belongs to the &lt;i&gt;admin&lt;/i&gt; group in that channel, even if he doesn&apos;t belong to the &lt;i&gt;admin&lt;/i&gt; group in the channel where the ACL originated.&lt;br /&gt;If a group name starts with a !, its membership is negated, and if it starts with a ~, it is evaluated in the channel the ACL was defined in, rather than the channel the ACL is active in. Order is important; &lt;i&gt;!~in&lt;/i&gt; is valid, but &lt;i&gt;~!in&lt;/i&gt; is not.&lt;br /&gt;A few special predefined groups are:&lt;br /&gt;&lt;b&gt;all&lt;/b&gt; - Everyone will match.&lt;br /&gt;&lt;b&gt;auth&lt;/b&gt; - All authenticated users will match.&lt;br /&gt;&lt;b&gt;sub,a,b,c&lt;/b&gt; - User currently in a sub-channel minimum &lt;i&gt;a&lt;/i&gt; common parents, and between &lt;i&gt;b&lt;/i&gt; and &lt;i&gt;c&lt;/i&gt; channels down the chain. See the website for more extensive documentation on this one.&lt;br /&gt;&lt;b&gt;in&lt;/b&gt; - Users currently in the channel will match (convenience for &lt;i&gt;sub,0,0,0&lt;/i&gt;).&lt;br /&gt;&lt;b&gt;out&lt;/b&gt; - Users outside the channel will match (convenience for &lt;i&gt;!sub,0,0,0&lt;/i&gt;).&lt;br /&gt;Note that an entry applies to either a user or a group, not both.</source>
        <translatorcomment>ja jebie kto to wymyslil ;) //Zuko</translatorcomment>
        <translation type="obsolete">Kontroluje, której grupy użytkowników dotyczy dana reguła.&lt;br /&gt;Należy pamiętać, że grupa jest sprawdzana w kontekście reguł jakie są w niej używane. Na przykład, na kanale Root domyślna reguła ACL nadaje uprawnienie &lt;i&gt;Zapisz ACL&lt;/i&gt; grupie &lt;i&gt;admin&lt;/i&gt;. Jeżeli ta reguła zostanie odziedziczona przez kanał, nada ona uprawnienia zapisu ACL użytkownikowi który należy do grupy &lt;i&gt;admin&lt;/i&gt; na tym kanale, nawet gdy użytkownik nie należy do grupy &lt;i&gt;admin&lt;/i&gt; na kanale z którego reguły ACL pochodza.&lt;br /&gt;Jeżeli nazwa grupy rozpoczyna od !, członkowstwo użtykownika jest negowane, a gdy rozpoczyna się od ~, reguła jest sprawdzana na kanale gdzie została zdefiniowana, zamiast na kanale w którym reguła ACL jest aktywna. Kolejność jest ważna: &lt;i&gt;!~in&lt;/i&gt; jest ważny, ale &lt;i&gt;~!in&lt;/i&gt; nie jest.&lt;br /&gt;Jest kilka predefiniowanych grup:&lt;br /&gt;&lt;b&gt;all&lt;/b&gt; - wszyscy użytkownicy będą pasować.&lt;br /&gt;&lt;b&gt;auth&lt;/b&gt; - Wszyscy uwierzytelnieni użytkownicy będą pasować.&lt;br /&gt;&lt;b&gt;sub,a,b,c&lt;/b&gt; - Użytkownicy znajdujący się w aktualnie w pod-kanele. Zobacz dokumentacje na stronie internetowej, aby dowiedzieć się więcej o tej funkcji.&lt;br /&gt;&lt;b&gt;in&lt;/b&gt; - Użytkownicy aktualnie znajdujący się na kanale (wygodniejsza metoda w porównaniu do &lt;i&gt;sub,0,0,0&lt;/i&gt;.&lt;br /&gt;&lt;b&gt;out&lt;/b&gt; - Użytkownicy nie znajdujący się na kanale będą pasować (wygodniejsza metoda w porównaniu do &lt;i&gt;!sub,0,0,0&lt;/i&gt;).&lt;br /&gt;Pamiętaj, że wpis może dotyczyć zarówno użytkownika lub grupy, a nie obu jednocześnie.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="598"/>
        <source>User this entry applies to</source>
        <translation>Reguła należy do użytkownika</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="601"/>
        <source>This controls which user this entry applies to. Just type in the user name and hit enter to query the server for a match.</source>
        <translation>Kontroluje do którego użytkownika należy ta reguła. Poprostu wpisz nazwę użytkownika i naciśnij Enter by wysłać zapytanie do serwera w celu sprawdzenia poprawności nazwy użytkownika.</translation>
    </message>
    <message>
        <location filename="ACLEditor.cpp" line="135"/>
        <source>Allow %1</source>
        <translation>Zezwól na %1</translation>
    </message>
    <message>
        <location filename="ACLEditor.cpp" line="126"/>
        <source>Deny %1</source>
        <translation>Zabroń %1</translation>
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
        <translation>Przydziela uprawnienie: %1. Jeżeli uprawnienie jest jednocześnie dozwolone i zabronione, to jest zabronione.&lt;br /&gt;%2</translation>
    </message>
    <message>
        <location filename="ACLEditor.cpp" line="127"/>
        <source>This revokes the %1 privilege. If a privilege is both allowed and denied, it is denied.&lt;br /&gt;%2</source>
        <translation>Odbiera uprawnienie %1. Jeżeli uprawnienie jest jednocześnie dozwolone i zabronione, to jest zabronione.&lt;br /&gt;%2</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="149"/>
        <source>List of groups</source>
        <translation>Lista grup</translation>
    </message>
    <message>
        <source>This is all the groups currently defined for the channel. To create a new group, just type in the name and press enter.</source>
        <translation type="obsolete">To jest lista wszystkich grup zdefiniowanych dla tego kanału. Aby utworzyć nową grupę wpisz jej nazwę i naciśnij Enter.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="177"/>
        <source>Remove selected group</source>
        <translation>Usuń zaznaczoną grupę</translation>
    </message>
    <message>
        <source>This removes the currently selected group. If the group was inherited, it will not be removed from the list, but all local information about the group will be cleared.</source>
        <translation type="obsolete">Usuwa aktualnie wybrana grupę. Jeśli grupa była dziedziczona, to nie zostanie usunięta z listy, ale wszystkie lokalne informacje o tej grupie zostaną wyczyszczone.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="193"/>
        <source>Inherit group members from parent</source>
        <translation>Dziedzicz członków grupy z grupy nadrzędnej</translation>
    </message>
    <message>
        <source>This inherits all the members in the group from the parent, if the group is marked as &lt;i&gt;Inheritable&lt;/i&gt; in the parent channel.</source>
        <translation type="obsolete">Dziedziczy wszystkich członków z grupy nadrzędnej, jeżeli grupa jest oznaczona jako .&lt;i&gt;Dziedziczna&lt;/i&gt; w kanale nadrzędnym.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="206"/>
        <source>Make group inheritable to sub-channels</source>
        <translation>Umożliw dziedziczenie grupy w pod-kanałach</translation>
    </message>
    <message>
        <source>This makes this group inheritable to sub-channels. If the group is non-inheritable, sub-channels are still free to create a new group with the same name.</source>
        <translation type="obsolete">Umożliwia dziedziczenie uprawnień grupy w pod-kanałach. Jeżeli grupa nie pozwala na dziedziczenie, to w pod-kanałach można twożyć grupy o tych samych nazwach.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="222"/>
        <source>Group was inherited from parent channel</source>
        <translation>Grupa została odziedziczona z kanału nadrzędnego</translation>
    </message>
    <message>
        <source>This indicates that the group was inherited from the parent channel. You cannot edit this flag, it&apos;s just for information.</source>
        <translation type="obsolete">Wskazuje czy grupa została odziedziczona z kanału nadrzędnego. Nie możesz edytować tej flagi, to jest tylko informacja.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="303"/>
        <source>Add member to group</source>
        <translation>Dodaj członka do grupy</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="335"/>
        <source>Remove member from group</source>
        <translation>Usuń członka z grupy</translation>
    </message>
    <message>
        <source>Mumble - Edit ACL for %1</source>
        <translation type="obsolete">Mumble - edycja reguł ACL dla kanału %1</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="14"/>
        <source>Dialog</source>
        <translation>Dialog</translation>
    </message>
    <message>
        <source>Type in the name of a player you wish to add to the group and click Add.</source>
        <translation type="obsolete">Wpisz nazwę użytkownika, którego chcesz dodać do grupy i kliknij Dodaj.</translation>
    </message>
    <message>
        <source>Type in the name of a player you wish to remove from the group and click Add.</source>
        <translation type="obsolete">Wpisz nazwę użytkownika, któego chcesz usunąć z grupy i kliknij Dodaj.</translation>
    </message>
</context>
<context>
    <name>ALSAAudioInput</name>
    <message>
        <location filename="ALSAAudio.cpp" line="363"/>
        <source>Opening chosen ALSA Input failed: %1</source>
        <translation>Otwieranie wybranego wejścia ALSA nie powiodło się: %1</translation>
    </message>
    <message>
        <location filename="ALSAAudio.cpp" line="172"/>
        <source>Default ALSA Card</source>
        <translation>Domyślna karta ALSA</translation>
    </message>
</context>
<context>
    <name>ALSAAudioOutput</name>
    <message>
        <location filename="ALSAAudio.cpp" line="498"/>
        <source>Opening chosen ALSA Output failed: %1</source>
        <translation>Otwieranie wybranego wyjścia ALSA nie powiodło się: %1</translation>
    </message>
    <message>
        <location filename="ALSAAudio.cpp" line="173"/>
        <source>Default ALSA Card</source>
        <translation>Domyślna karta ALSA</translation>
    </message>
</context>
<context>
    <name>ASIOConfig</name>
    <message>
        <location filename="ASIOInput.ui" line="20"/>
        <source>Device selection</source>
        <translation>Wybieranie urządzenia</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="81"/>
        <source>Capabilities</source>
        <translation>Możliwości</translation>
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
        <location filename="ASIOInput.ui" line="136"/>
        <source>Channels</source>
        <translation>Kanały</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="130"/>
        <source>Configure input channels</source>
        <translation>Konfiguruj kanały wejścia</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="42"/>
        <source>Device to use for microphone</source>
        <translation>Urządzenie do którego jest podłączony mikrofon</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="45"/>
        <source>This chooses what device to query. You still need to actually query the device and select which channels to use.</source>
        <translation>Wybiera które urządzenie sprawdzić. Musisz jeszcze sprawdzić urządzenie i wybrać kanały do wykorzystania.</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="26"/>
        <source>Device</source>
        <translation>Urządzenie</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="52"/>
        <source>Query selected device</source>
        <translation>Sprawdź zaznaczone urządzenie</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="55"/>
        <source>This queries the selected device for channels. Be aware that many ASIO drivers are buggy to the extreme, and querying them might cause a crash of either the application or the system.</source>
        <translation>Sprawdza dane urządzenie. &lt;b&gt;Uwaga:&lt;/b&gt; wiele sterowników ASIO jest tak wadliwych, że sprawdzanie ich może spowodować zawieszenie się programu lub całego systemu.</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="65"/>
        <source>Configure selected device</source>
        <translation>Konfiguruj zaznaczone urządzenie</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="68"/>
        <source>This configures the selected device. Be aware that many ASIO drivers are buggy to the extreme, and querying them might cause a crash of either the application or the system.</source>
        <translation>Konfiguruje dane urządzenie. &lt;b&gt;Uwaga:&lt;/b&gt; wiele sterowników ASIO jest tak wadliwych, że sprawdzanie ich może spowodować zawieszenie się programu lub całego systemu.</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="87"/>
        <source>Driver name</source>
        <translation>Nazwa sterownika</translation>
    </message>
    <message>
        <source>Buffersize</source>
        <translation type="obsolete">Rozmiar bufora</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="157"/>
        <source>Microphone</source>
        <translation>Mikrofon</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="206"/>
        <location filename="ASIOInput.ui" line="305"/>
        <source>&lt;-</source>
        <translation>&lt;-</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="219"/>
        <location filename="ASIOInput.ui" line="292"/>
        <source>-&gt;</source>
        <translation>-&gt;</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="243"/>
        <source>Unused</source>
        <translation>Nieużywany</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="329"/>
        <source>Speakers</source>
        <translation>Głośniki</translation>
    </message>
    <message>
        <source>%1 (ver %2)</source>
        <translation type="obsolete">%1 (wersja %2)</translation>
    </message>
    <message>
        <source>%1 ms -&gt; %2 ms (%3 ms resolution) %4Hz</source>
        <translatorcomment>resolution to moze bc prawdopodobnie probkowanie - nie mam jak sprawdzic //Zuko</translatorcomment>
        <translation type="obsolete">%1 ms -&gt; %2 ms (%3 ms rozdzielczość) %4Hz</translation>
    </message>
    <message>
        <source>%1 ms -&gt; %2 ms (%3 ms resolution) %4Hz -- Unusable</source>
        <translatorcomment>ew  -- nieprzydatny; nie nadający się do użytku</translatorcomment>
        <translation type="obsolete">%1 ms -&gt; %2 ms (%3 ms rozdzielczość) %4Hz -- Niemożliwy do wykorzystania</translation>
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
        <translation>Mumble</translation>
    </message>
    <message>
        <location filename="ASIOInput.cpp" line="269"/>
        <location filename="ASIOInput.cpp" line="296"/>
        <source>ASIO Initialization failed: %1</source>
        <translation>Inizjalizacja ASIO nie powiodła się: %1</translation>
    </message>
    <message>
        <location filename="ASIOInput.cpp" line="273"/>
        <location filename="ASIOInput.cpp" line="300"/>
        <source>Failed to instantiate ASIO driver</source>
        <translatorcomment>prawdopodobnie bledne tlumaczenie //Zuko</translatorcomment>
        <translation>Wytwarzanie sterownika ASIO nie powiodło się</translation>
    </message>
    <message>
        <location filename="ASIOInput.cpp" line="337"/>
        <source>ASIO</source>
        <translation>ASIO</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="58"/>
        <source>&amp;Query</source>
        <translation>&amp;Sprawdź</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="71"/>
        <source>&amp;Configure</source>
        <translation>&amp;Konfiguruj</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="14"/>
        <source>Form</source>
        <translation>Formularz</translation>
    </message>
    <message>
        <source>TextLabel</source>
        <translation type="obsolete">Tekst</translation>
    </message>
    <message>
        <source>This will configure the input channels for ASIO. Make sure you select at least one channel as microphone and speaker. &lt;i&gt;Microphone&lt;/i&gt; should be where your microphone is attached, and &lt;i&gt;Speaker&lt;/i&gt; should be a channel that samples &lt;i&gt;What you hear&lt;/i&gt;.&lt;br /&gt;For example, on the Audigy 2 ZS, a good selection for Microphone would be &lt;i&gt;Mic L&lt;/i&gt; while Speaker should be &lt;i&gt;Mix L&lt;/i&gt; and &lt;i&gt;Mix R&lt;/i&gt;.</source>
        <translation type="obsolete">Tutaj znajduje się konfiguracja kanałów wejściowych ASIO. Upewnij się, że wybierzesz przynajmniej jeden kanał mikrofonu i głośnika.&lt;br /&gt; &lt;i&gt;Mikrofon&lt;/i&gt;powinien być tam gdzie podłączyłeś swój mikrofon, a &lt;i&gt;Głóśniki&lt;/i&gt; powinny być w kanałach z &lt;i&gt;Co słyszysz&lt;/i&gt;.&lt;br /&gt;Dla przykładu: Audigy 2 ZS dobrym wyborem dla mikrofonu był &lt;i&gt;Mic L&lt;/i&gt; a głośniki powinny być w &lt;i&gt;MixR&lt;i&gt; i&lt;/i&gt; MixL&lt;/i&gt;.</translation>
    </message>
</context>
<context>
    <name>ASIOInput</name>
    <message>
        <location filename="ASIOInput.cpp" line="417"/>
        <location filename="ASIOInput.cpp" line="520"/>
        <source>Mumble</source>
        <translation>Mumble</translation>
    </message>
    <message>
        <location filename="ASIOInput.cpp" line="417"/>
        <source>You need to select at least one microphone and one speaker source to use ASIO. If you just need microphone sampling, use DirectSound.</source>
        <translation>Musisz zaznaczyć przynajmniej jeden mikrofon oraz jedno źródło głośnika, aby używać ASIO. Jeżeli potrzebujesz tylko próbkowania mikrofonu używaj DirectSound.</translation>
    </message>
    <message>
        <location filename="ASIOInput.cpp" line="520"/>
        <source>Opening selected ASIO device failed. No input will be done.</source>
        <translation>Otwieranie zaznaczonego urządzenia ASIO nie powiodło się. Żadne wejście nie jest gotowe.</translation>
    </message>
</context>
<context>
    <name>AboutDialog</name>
    <message>
        <location filename="About.cpp" line="63"/>
        <source>&amp;About Mumble</source>
        <translation>&amp;O Mumble</translation>
    </message>
    <message>
        <location filename="About.cpp" line="64"/>
        <source>&amp;License</source>
        <translation>&amp;Licencja</translation>
    </message>
    <message>
        <location filename="About.cpp" line="66"/>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <location filename="About.cpp" line="37"/>
        <source>About Mumble</source>
        <translation>O Mumble</translation>
    </message>
    <message>
        <location filename="About.cpp" line="53"/>
        <source>&lt;h3&gt;Mumble (%1)&lt;/h3&gt;&lt;p&gt;Copyright %3 Thorvald Natvig&lt;br /&gt;slicer@users.sourceforge.net&lt;/p&gt;&lt;p&gt;&lt;b&gt;A voice-chat utility for gamers&lt;/b&gt;&lt;/p&gt;&lt;p&gt;&lt;tt&gt;&lt;a href=&quot;%2&quot;&gt;%2&lt;/a&gt;&lt;/tt&gt;&lt;/p&gt;</source>
        <translation>&lt;h3&gt;Mumble v%1&lt;/h3&gt;&lt;p&gt;Copyright %3 Thorvald Natvig&lt;br /&gt;slicer@users.sourceforge.net&lt;/p&gt;&lt;p&gt;&lt;b&gt;Polskie tłumaczenie:&lt;/b&gt; Bartek &quot;stranded&quot; Sumowski&lt;br /&gt; &lt;a href=http://sumowski.com&gt;sumowski.com&lt;/a&gt;&lt;br&gt;&lt;b&gt;Korekta:&lt;/b&gt; Michał Żukowski&lt;br&gt;  &lt;br&gt;&lt;b&gt;Przy tłumaczeniu pomogł:&lt;/b&gt; _KaszpiR_&lt;br&gt;&lt;a href=http://www.hlds.pl&gt;hlds.pl&lt;/a&gt;&lt;/br&gt;&lt;/p&gt;&lt;p&gt;&lt;b&gt;Komunikator głosowy dla graczy&lt;/b&gt;&lt;/p&gt;&lt;p&gt;&lt;tt&gt;&lt;a href=&quot;%2&quot;&gt;%2&lt;/a&gt;&lt;/tt&gt;&lt;/p&gt;</translation>
    </message>
</context>
<context>
    <name>AboutSpeexDialog</name>
    <message>
        <location filename="About.cpp" line="78"/>
        <source>&lt;h3&gt;About Speex&lt;/h3&gt;&lt;p&gt;&lt;tt&gt;&lt;a href=&quot;%1&quot;&gt;%1&lt;/a&gt;&lt;/tt&gt;&lt;/p&gt;&lt;p&gt;This program uses SpeexDSP.&lt;/p&gt;&lt;p&gt;Speex is used for echo cancellation, noise&lt;br /&gt;filtering and voice activity detection.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="About.cpp" line="86"/>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <location filename="About.cpp" line="74"/>
        <source>About Speex</source>
        <translation>O Speex</translation>
    </message>
    <message>
        <source>&lt;h3&gt;About Speex&lt;/h3&gt;&lt;p&gt;&lt;tt&gt;&lt;a href=&quot;%2&quot;&gt;%2&lt;/a&gt;&lt;/tt&gt;&lt;/p&gt;&lt;p&gt;This program uses Speex version %1&lt;/p&gt;&lt;p&gt;Speex is used for echo cancellation, noise&lt;br /&gt;filtering, voice activity detection and speech&lt;br /&gt;compression.&lt;/p&gt;</source>
        <translation type="obsolete">&lt;h3&gt;O Speex&lt;/h3&gt;&lt;p&gt;&lt;tt&gt;&lt;a href=&quot;%2&quot;&gt;%2&lt;/a&gt;&lt;/tt&gt;&lt;/p&gt;&lt;p&gt;Program używa kodeka Speex w wersji %1&lt;/p&gt;&lt;p&gt;Speex jest używany do usuwania akustycznego echa, filtrowania szumów, detekcji aktywacji głosowej, kompresji.&lt;/p&gt;</translation>
    </message>
</context>
<context>
    <name>AudioInput</name>
    <message>
        <location filename="AudioInput.ui" line="14"/>
        <source>Form</source>
        <translation>Formularz</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="20"/>
        <source>Interface</source>
        <translation>Interfejs</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="26"/>
        <source>System</source>
        <translation>System</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="42"/>
        <source>Input method for audio</source>
        <translation>Metoda wejścia dźwięku</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="45"/>
        <source>&lt;b&gt;This is the input method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation>&lt;b&gt;Jest to metoda wejścia dźwięku.&lt;/b&gt;&lt;br /&gt;Najprawdopodobniej chcesz korzystać z DirectSound.</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="68"/>
        <source>Device</source>
        <translation>Urządzenie</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="84"/>
        <source>Input device for audio</source>
        <oldsource>Output method for audio</oldsource>
        <translation>Metoda wejścia dźwięku</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="87"/>
        <source>&lt;b&gt;This is the input device to use for audio.&lt;/b&gt;</source>
        <oldsource>&lt;b&gt;This is the output method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</oldsource>
        <translation>&lt;b&gt;Jest to metoda wejścia dźwięku.&lt;/b&gt;&lt;br /&gt;Najprawdopodobniej chcesz korzystać z DirectSound.</translation>
    </message>
    <message>
        <source>Cancel Echo</source>
        <translation type="obsolete">Usuwanie akustycznego echa</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="141"/>
        <source>Transmission</source>
        <translation>Transmisja</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="147"/>
        <source>&amp;Transmit</source>
        <translation>&amp;Sposób transmisji głosu</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="157"/>
        <source>When to transmit your speech</source>
        <translation>Kiedy wysyłać twój głos</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="160"/>
        <source>&lt;b&gt;This sets when speech should be transmitted.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;Continuous&lt;/i&gt; - All the time&lt;br /&gt;&lt;i&gt;Voice Activity&lt;/i&gt; - When you are speaking clearly.&lt;br /&gt;&lt;i&gt;Push To Talk&lt;/i&gt; - When you hold down the hotkey set under &lt;i&gt;Shortcuts&lt;/i&gt;.</source>
        <translation>&lt;b&gt;Ta opcja określa kiedy to co mówisz powinno zostać wysłane.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;Ciągłe nadawanie&lt;/i&gt;&lt;br /&gt;&lt;i&gt;Aktywacja głosowa&lt;/i&gt; - Gdy mówisz wyraźnie&lt;br /&gt;&lt;i&gt;Aktywacja przyciskiem&lt;/i&gt; - Gdy trzymasz wciśnięty przycisk ustawiony w &lt;i&gt;Skróty&lt;/i&gt;.</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="219"/>
        <source>DoublePush Time</source>
        <translation>Ustaw czas￼ potrzebny na włączenie podwójnego kliknięcia:&lt;br /&gt;&lt;br /&gt;Podwójne kliknięcie ￼włącza ciągłą transmisję głosu, &lt;br /&gt;ponowne podwójne￼ kliknięcie wyłącza ją.</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="251"/>
        <source>TextLabel</source>
        <translation>Tekst</translation>
    </message>
    <message>
        <source>Audible audio cue when push-to-talk pressed and released</source>
        <translation type="obsolete">&lt;b&gt;Aktywując tę opcję usłyszysz charakterystyczny dźwięk.&lt;/b&gt;&lt;br /&gt;Podczas wciskania przycisku do rozmowy usłyszysz dźwięk z gry PacMan.</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="341"/>
        <source>&lt;b&gt;This enables the audio cues for push to talk.&lt;/b&gt;&lt;br /&gt;Setting this will give you a short audio beep when push to talk is pressed and released.</source>
        <translation>&lt;b&gt;Ta opcja aktywuje potwierdzenie dźwiękowe przy użyciu opcji aktywacji przyciskiem&lt;/b&gt;&lt;br /&gt;Podczas wciskania  i zwalniania przycisku usłyszysz dźwięk potwierdzający rozpoczęcie oraz zakończenie transmisji.</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="344"/>
        <source>PTT Audio cue</source>
        <translatorcomment>Jak ktos wymysli lepsze tlumaczenie to bedzie fajnie ;D</translatorcomment>
        <translation>Odtwórz dźwięk, podczas wciskania przycisku aktywacji głosowej   </translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="357"/>
        <source>Use SNR based speech detection</source>
        <translation>&lt;b&gt;Analiza na podstawie czystego sygnału.&lt;/b&gt; W tym trybie, źródło dźwięku jest analizowane pod kątem tzw. czystego sygnału, czystość tego sygnału jest używana do uaktywnienia wykrywania mowy.</translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets speech detection to use Signal to Noise ratio.&lt;/b&gt;&lt;br /&gt;In this mode, the input is analyzed for something resembling a clear singal, and the clarity of that signal is used to trigger speech detection.</source>
        <translation type="obsolete">&lt;b&gt;Analiza na podstawie czystego sygnału.&lt;/b&gt; W tym trybie, źródło dźwięku jest analizowane pod kątem tzw. czystego sygnału, czystość tego sygnału jest używana do uaktywnienia wykrywania mowy.</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="363"/>
        <source>Signal to Noise</source>
        <translation>stosunek sygnału do szumu</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="370"/>
        <source>Use Amplitude based speech detection.</source>
        <translation>&lt;b&gt;Analiza na podstawie siły głosu.&lt;/b&gt; W tym trybie, źródło dźwięku jest analizowane pod kątem siły głosu, głośniejszy dźwięk uaktywnia transmisję głosu.</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="373"/>
        <source>&lt;b&gt;This sets speech detection to use Amplitude.&lt;/b&gt;&lt;br /&gt;In this mode, the raw strength of the input signal is used to detect speech.</source>
        <translation>&lt;b&gt;Ta opcja ustawia sposób detekcji głosu na analizę na podstawie siły głosu.&lt;/b&gt; W tym trybie, źródło dźwięku jest analizowane pod kątem siły głosu, głośniejszy dźwięk uaktywnia transmisję głosu.</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="376"/>
        <source>Amplitude</source>
        <translation>analiza na podstawie siły głosu</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="385"/>
        <source>Voice &amp;Hold</source>
        <translation>Czas podtrzymania &amp;transmisji</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="395"/>
        <source>How long to keep transmitting after silence</source>
        <translation>Jak długo utrzymywać nadawanie po zakończeniu wypowiedzi.</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="398"/>
        <source>&lt;b&gt;This selects how long after a perceived stop in speech transmission should continue.&lt;/b&gt;&lt;br /&gt;Set this higher if your voice breaks up when you speak (seen by a rapidly blinking voice icon next to your name).&lt;br /&gt;Only makes sense when used with Voice Activity transmission.</source>
        <translation>&lt;b&gt;Ta opcja określa jak długo utrzymywać nadawanie po zakończeniu wypowiedzi.&lt;/b&gt;&lt;br /&gt;Ustaw wyższą wartość gdy twoje wypowiedzi są przerywane (można to zauważyć poprzeż szybkie miganie ikony przy twoim nicku)&lt;br /&gt;Opcja ma jedyne sensowne zastosowanie gdy stosujesz aktywację głosową.</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="443"/>
        <source>Silence Below</source>
        <translation>Cisza</translation>
    </message>
    <message>
        <source>Signal values below this counts as silence</source>
        <translation type="obsolete">Sygnał poniżej podanej wartości traktowany jest jako cisza</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="453"/>
        <location filename="AudioInput.ui" line="485"/>
        <source>&lt;b&gt;This sets the trigger values for voice detection.&lt;/b&gt;&lt;br /&gt;Use this together with the Audio Statistics window to manually tune the trigger values for detecting speech. Input values below &quot;Silence Below&quot; always count as silence. Values above &quot;Speech Above&quot; always count as voice. Values in between will count as voice if you&apos;re already talking, but will not trigger a new detection.</source>
        <translation>&lt;b&gt;Ta opcja określa próg aktywacji detekcji mowy.&lt;/b&gt;&lt;br /&gt;Użyj tego razem z &lt;Statystykami&lt;/i&gt; aby manualnie wyregulować próg aktywacji głosowej. Wartości wejścia poniżej &quot;Cisza&quot; zawsze są traktowane jakie cisza. Wartości powyżej &quot;Mowa&quot; zawsze są traktowane jako mowa. Wartości poniżej są traktowane jako mowa ale nie są traktowane jako nowa detekcja mowy.</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="475"/>
        <source>Speech Above</source>
        <translation>Mowa</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="482"/>
        <source>Signal values above this count as voice</source>
        <translation>Sygnał powyżej podanej wartości traktowany jest jako mowa</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="515"/>
        <source>Compression</source>
        <translation>Kompresja</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="521"/>
        <source>&amp;Quality</source>
        <translation>&amp;Jakość</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="531"/>
        <source>Quality of compression (peak bandwidth)</source>
        <translation>Jakość kompresji (maksymalne pasmo)</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="534"/>
        <source>&lt;b&gt;This sets the quality of compression.&lt;/b&gt;&lt;br /&gt;This determines how much bandwidth Mumble is allowed to use for outgoing audio.</source>
        <translation>&lt;b&gt;Ta opcja określa jakość kompresji.&lt;/b&gt;&lt;br /&gt;Decyduje również, ile pasma Mumble może wykorzystać podczas wysyłania dźwięku.</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="572"/>
        <source>Audio per packet</source>
        <translation>Dźwięk na pakiet</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="582"/>
        <source>How many audio frames to send per packet</source>
        <translation>Ile ramek dźwięku ma być wysyłane na pakiet</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="585"/>
        <source>&lt;b&gt;This selects how many audio frames should be put in one packet.&lt;/b&gt;&lt;br /&gt;Increasing this will increase the latency of your voice, but will also reduce bandwidth requirements.</source>
        <translation>&lt;b&gt;Ta opcja ustawia ile ramek dźwięku powinno być umieszczonych w jednym pakiecie.&lt;/b&gt;&lt;br /&gt;Zwiększanie ilości milisekund opóźni przesyłanie twojego głosu, ale dzięki temu zużyjesz mniej pasma.</translation>
    </message>
    <message>
        <source>Maximum bandwidth used for sent audio</source>
        <translation type="obsolete">Maksymalna ilość przepustowości łącza</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="625"/>
        <source>&lt;b&gt;This shows peak outgoing bandwidth used.&lt;/b&gt;&lt;br /&gt;This shows the peak amount of bandwidth sent out from your machine. Audio bitrate is the maximum bitrate (as we use VBR) for the audio data alone. Position is the bitrate used for positional information. Overhead is our framing and the IP packet headers (IP and UDP is 75% of this overhead).</source>
        <translation>&lt;b&gt;Wyświetla maksymalne pasmo jakie może być użyte.&lt;/b&gt;&lt;br /&gt;Wyświetla maksymalne pasmo jakie może być wykorzystane przy wysyłaniu z twojego komputera. Bitrate dźwięku (użyte jest VBR) jest maksymalnym bitrate dla samego dźwięku. Położenie jest to bitrate zawierający informacje wykorzystywane przy dźwięku pozycyjnym. Obciążenie są to nagłówki pakietów (IP i UDP stanowi 75% obciążenia).</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="641"/>
        <source>Audio Processing</source>
        <translation>Przetwarzanie dźwięku</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="647"/>
        <source>Noise Suppression</source>
        <translation>Tłumienie hałasu</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="657"/>
        <source>Noise suppression</source>
        <translation>Maksymalny poziom tłumienia hałasu z otoczenia.</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="660"/>
        <source>&lt;b&gt;This sets the amount of noise suppression to apply.&lt;/b&gt;&lt;br /&gt;The higher this value, the more aggressively stationary noise will be suppressed.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="692"/>
        <source>Amplification</source>
        <translation>Wzmocnienie dźwięku</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="702"/>
        <source>Maximum amplification of input sound</source>
        <translation>Maksymalne wzmocnienie dźwięku wejściowego.</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="705"/>
        <source>&lt;b&gt;Maximum amplification of input.&lt;/b&gt;&lt;br /&gt;Mumble normalizes the input volume before compressing, and this sets how much it&apos;s allowed to amplify.&lt;br /&gt;The actual level is continually updated based on your current speech pattern, but it will never go above the level specified here.&lt;br /&gt;If the &lt;i&gt;Microphone loudness&lt;/i&gt; level of the audio statistics hover around 100%, you probably want to set this to 2.0 or so, but if, like most people, you are unable to reach 100%, set this to something much higher.&lt;br /&gt;Ideally, set it so &lt;i&gt;Microphone Loudness * Amplification Factor &gt;= 100&lt;/i&gt;, even when you&apos;re speaking really soft.&lt;br /&gt;&lt;br /&gt;Note that there is no harm in setting this to maximum, but Mumble will start picking up other conversations if you leave it to auto-tune to that level.</source>
        <translation>&lt;b&gt;Maksymalne wzmocnienie wejścia.&lt;/b&gt;&lt;br /&gt;Mumble normalizuje poziom głośności przed kompresją, opcja ta określa dozwolony poziom wzmocnienia.&lt;br /&gt;Rzeczywisty poziom jest stale aktualizowany w oparciu o aktualną strukturę twojej wypowiedzi, ale nigdy nie przekracza poziomu określonego tutaj.&lt;br /&gt; Jeśli poziom głośności mikrofonu w statystykach wynosi około 100% to prawdopodobnie chcesz ustawić wartość tej opcji na 2.0, ale gdy jak większość użytkowników nie będziesz w stanie osiągnąć 100%, ustaw wartość tej opcji trochę wyżej.&lt;br /&gt;Najlepiej ustawić jak tak: &lt;i&gt;głośność mikrofonu * czynnik wzmocnienia&gt;= 100&lt;/i&gt; nawet gdy mówisz bardzo miękko. Pamiętaj, że nie zaszkodzi w tym ustawienie maksymalnej wartości, ale Mumble zacznie przechwytywać inne rozmowy jeżeli zostawisz to do auto-dostrojenia.</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="433"/>
        <source>Current speech detection chance</source>
        <translation>Aktualna szansa na wykrycie mowy</translation>
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
        <translation type="unfinished">Wył.</translation>
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
        <translation type="unfinished">Podgląd</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="338"/>
        <source>Audible audio cue when push-to-talk is activated or deactivated</source>
        <translation>Kiedy wciskasz przycisk transmisji głosu zostanie odegrany dźwięk potwierdzający rozpoczęcie transmisji</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="360"/>
        <source>&lt;b&gt;This sets speech detection to use Signal to Noise ratio.&lt;/b&gt;&lt;br /&gt;In this mode, the input is analyzed for something resembling a clear signal, and the clarity of that signal is used to trigger speech detection.</source>
        <translation>&lt;b&gt;Ta opcja ustawia sposób detekcji głosu na stosunek sygnału do szumu.&lt;/b&gt;&lt;br /&gt;W tym trybie sygnał wejściowy jest analizowany jako czysty, a czystość tego sygnału używana jest do włączania transmisji głosowej.</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="436"/>
        <source>&lt;b&gt;This shows the current speech detection settings.&lt;/b&gt;&lt;br /&gt;You can change the settings from the Settings dialog or from the Audio Wizard.</source>
        <translation>&lt;b&gt;Wyświetla aktualne ustawienia wykrywania mowy.&lt;/b&gt;&lt;br /&gt;Możesz zmienić te ustawienia w &lt;i&gt;Opcjach&lt;/i&gt; lub w &lt;i&gt;Kreatorze ustawień audio&lt;/i&gt;.</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="167"/>
        <source>Idle AutoMute</source>
        <translation>Automatyczne wyciszanie mikrofonu&lt;br /&gt;po upłynięciu czasu bezczynności</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="174"/>
        <source>How long to remain idle before auto-mute.</source>
        <translation>Ile czasu musisz pozostać bezczynnym by zostać automatycznie wyciszonym.</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="177"/>
        <source>This sets an idle timer. If the timer expires without any audio being sent to the server, you are muted.</source>
        <translation>Tutaj możesz ustawić czas bezczynności. Jeżeli przez ten czas nie wyślesz żadnego pakietu do serwera, zostaniesz wyciszony.</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="450"/>
        <source>Signal values below this count as silence</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="622"/>
        <source>Maximum bandwidth used for sending audio</source>
        <translation>Maksymalne pasmo używane do wysyłania dźwięku</translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets the amount of noise suppression to apply.&lt;/b&gt;&lt;br /&gt;The higher this value, the more aggressively stationary noise will be supressed.</source>
        <translation type="obsolete">&lt;b&gt;Ta opcja określa wartość tłumienia hałasu.&lt;/b&gt;&lt;br /&gt;Zwiększenie tej wartości powoduje agresywniejsze tłumienie hałasu.</translation>
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
        <translation>ciągłe nadawanie</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="75"/>
        <source>Voice Activity</source>
        <translation>aktywacja głosowa</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="76"/>
        <source>Push To Talk</source>
        <translation>aktywacja przyciskiem</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="89"/>
        <source>Audio Input</source>
        <translation>Wejście audio</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="185"/>
        <location filename="AudioConfigDialog.cpp" line="193"/>
        <source>%1 ms</source>
        <translation>%1 ms</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="191"/>
        <location filename="AudioConfigDialog.cpp" line="211"/>
        <location filename="AudioConfigDialog.cpp" line="229"/>
        <source>Off</source>
        <translation>Wył.</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="199"/>
        <source>%1 s</source>
        <translation>%1 s</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="203"/>
        <source>%1 kb/s</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="214"/>
        <source>-%1 dB</source>
        <translation>%1 dB</translation>
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
        <translation type="obsolete">%1 kbit/s (Dźwięk %2, Położenie %4, Obciążenie %3)</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="227"/>
        <source>%1 min</source>
        <translation>%1 min</translation>
    </message>
</context>
<context>
    <name>AudioOutput</name>
    <message>
        <location filename="AudioOutput.ui" line="14"/>
        <source>Form</source>
        <translation>Formularz</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="20"/>
        <source>Interface</source>
        <translation>Interfejs</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="26"/>
        <source>System</source>
        <translation>System</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="42"/>
        <source>Output method for audio</source>
        <oldsource>Input method for audio</oldsource>
        <translation>Metoda wyjścia dźwięku</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="45"/>
        <source>&lt;b&gt;This is the output method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <oldsource>&lt;b&gt;This is the input method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</oldsource>
        <translation>&lt;b&gt;Jest to metoda wyjścia dźwięku.&lt;/b&gt;&lt;br /&gt;Najprawdopodobniej chcesz korzystać z DirectSound.</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="68"/>
        <source>Device</source>
        <translation>Urządzenie</translation>
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
        <translation>Dźwięk pozycyjny</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="110"/>
        <source>Audio Output</source>
        <translation>Wyjście audio</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="116"/>
        <source>Default &amp;Jitter Buffer</source>
        <translation>Domyślny bufor &amp;drgań</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="126"/>
        <source>Safety margin for jitter buffer</source>
        <translation>Granica bezpieczeństwa dla bufora drgań.</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="129"/>
        <source>&lt;b&gt;This sets the minimum safety margin for the jitter buffer.&lt;/b&gt;&lt;br /&gt;All incoming audio is buffered, and the jitter buffer continually tries to push the buffer to the minimum sustainable by your network, so latency can be as low as possible. This sets the minimum buffer size to use. If the start of sentences you hear is very jittery, increase this value.</source>
        <translation>&lt;b&gt;Granica bezpieczeństwa dla bufora drgań.&lt;/b&gt;&lt;br /&gt;Każdy przychodzący dźwięk jest najpierw buforowany, przez co opóźnienie jest najniższe jak to tylko możliwe. Ta opcja ustawia minimalny rozmiar bufora. Jeśli mowa jest dla ciebie lekko zniekształcona spróbuj zwiększyć ilość milisekund.</translation>
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
        <location filename="AudioOutput.ui" line="574"/>
        <source>&lt;b&gt;This enables one of the loopback test modes.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;None&lt;/i&gt; - Loopback disabled&lt;br /&gt;&lt;i&gt;Local&lt;/i&gt; - Emulate a local server.&lt;br /&gt;&lt;i&gt;Server&lt;/i&gt; - Request loopback from server.&lt;br /&gt;Please note than when loopback is enabled, no other users will hear your voice. This setting is not saved on application exit.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>TextLabel</source>
        <translation type="obsolete">Tekst</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="161"/>
        <source>Volume</source>
        <translation>Głośność</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="171"/>
        <source>Volume of incoming speech</source>
        <translation>Głośność przychodzącego dźwięku</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="174"/>
        <source>&lt;b&gt;This adjusts the volume of incoming speech.&lt;/b&gt;&lt;br /&gt;Note that if you increase this beyond 100%, audio will be distorted.</source>
        <translation>&lt;b&gt;Ta opcja pozwala na określenie głośności przychodzącej mowy.&lt;/b&gt;&lt;br /&gt;Każda wartość powyżej 100% sprawi, że dźwięk będzie trochę zniekształcony.</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="200"/>
        <source>Output Delay</source>
        <translation>Opóźnienie wyjścia</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="210"/>
        <source>Amount of data to buffer</source>
        <translation>Ilość danych trafiających do bufora.</translation>
    </message>
    <message>
        <source>This sets the amount of data to prebuffer in the output buffer. Experiment with different values and set it to the lowest which doesn&apos;t cause rapid jitter in the sound.</source>
        <translation type="obsolete">Ustala ilość danych które trafiają do przed-bufora w buforze wyjścia. Eksperymentowanie z różnymi wartościami i ustawienie ich na najniższy poziom nie skutkują natychmiastowym rozsynchronizowaniem się dźwięku.</translation>
    </message>
    <message>
        <source>Volume attenuation</source>
        <translation type="obsolete">Dźwięk pozycyjny</translation>
    </message>
    <message>
        <source>MinDistance</source>
        <translation type="obsolete">Minimalny zasięg</translation>
    </message>
    <message>
        <source>Minimum distance to player before sound decreases</source>
        <translation type="obsolete">Ustawia minimalny zasięg
przechwytywania źródeł dźwięku</translation>
    </message>
    <message>
        <source>This sets the minimum distance for sound calculations. The volume of other players&apos; speech will not decrease until they are at least this far away from you.</source>
        <translation type="obsolete">Ustawia minimalny zasięg przechwytywania źródeł dźwięku. Głośność innych użytkowników nie będzie się zmieniać dopóki nie znajdą się dalej niż wskazuje minimalny dystans.</translation>
    </message>
    <message>
        <source>MaxDistance</source>
        <translation type="obsolete">Maksymalny zasięg</translation>
    </message>
    <message>
        <source>Maximum distance, beyond which sound won&apos;t decrease</source>
        <translation type="obsolete">Ustawia maksymalny zasięg
przechwytywania źródeł dźwięku</translation>
    </message>
    <message>
        <source>This sets the maximum distance for sound calculations. When farther away than this, other players&apos; sound volume will not decrease any more.</source>
        <translation type="obsolete">Ustawia maksymalny zasięg przechwytywania źródeł dźwięku.</translation>
    </message>
    <message>
        <source>RollOff</source>
        <translation type="obsolete">Zanikanie dźwięku</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="397"/>
        <source>Factor for sound volume decrease</source>
        <translation>&lt;b&gt;Ustawia jak szybko odbierany dźwięk ma zanikać.&lt;/b&gt;&lt;br /&gt;Powiększając tą wartość sprawisz, że dźwięk zaniknie szybciej, zmniejszając - dźwięk zaniknie wolniej.</translation>
    </message>
    <message>
        <source>How fast should sound volume drop when passing beyond the minimum distance. The normal (1.0) is that sound volume halves each time the distance doubles. Increasing this value means sound volume drops faster, while decreasing it means it drops slower.</source>
        <translation type="obsolete">Jak szybko głośność powinna spadać, gdy przekroczy minimalny zasięg przechwytywania dźwięku.</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="426"/>
        <source>Bloom</source>
        <translation>Zmienna głośność</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="471"/>
        <source>Loopback Test</source>
        <translation>Tryb testowy</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="477"/>
        <source>Delay Variance</source>
        <translation>Opóźnienie pakietów</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="487"/>
        <source>Variance in packet latency</source>
        <translation>Zmienność w opóźnieniach pakietów</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="490"/>
        <source>&lt;b&gt;This sets the packet latency variance for loopback testing.&lt;/b&gt;&lt;br /&gt;Most audio paths contain some variable latency. This allows you set that variance for loopback mode testing. For example, if you set this to 15ms, this will emulate a network with 20-35ms ping latency or one with 80-95ms latency. Most domestic net connections have a variance of about 5ms.</source>
        <oldsource>&lt;b&gt;This sets the packet latency variance for loopback testing.&lt;/b&gt;&lt;br /&gt;Most audio paths contain some variable latency. This allows you set that variance for loopback mode testing. For example, if you set this to 15ms, this will emulate a network with 20-35ms ping latency or one with 80-95ms latency. Most domestic net connections have a variance of about 5ms</oldsource>
        <translation type="unfinished">&lt;b&gt;Ta opcja ustawia długość opóźnienia pakietów dla trybu testowego.&lt;/b&gt;&lt;br /&gt;Na przykład jeśli ustawisz 15 milisekund test zacznie emulować opóźnienie sieci z pingiem 20-35 milisekund lub 80-95 milisekund. Większość połączeń różni się między sobą o około 5 milisekund.</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="516"/>
        <source>Packet Loss</source>
        <translation>Utracone pakiety</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="526"/>
        <source>Packet loss for loopback mode</source>
        <translation>Ilość utraconych pakietów podczas testu.</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="529"/>
        <source>&lt;b&gt;This sets the packet loss for loopback mode.&lt;/b&gt;&lt;br /&gt;This will be the ratio of packets lost. Unless your outgoing bandwidth is peaked or there&apos;s something wrong with your network connection, this will be 0%</source>
        <translation>&lt;b&gt;Ta opcja ustawia ilość utraconych pakietów w trybie testowym.&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="561"/>
        <source>&amp;Loopback</source>
        <translation>&amp;Wybierz tryb</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="571"/>
        <source>Desired loopback mode</source>
        <translation>Tryb testowania</translation>
    </message>
    <message>
        <source>&lt;b&gt;This enables one of the loopback testmodes.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;None&lt;/i&gt; - Loopback disabled&lt;br /&gt;&lt;i&gt;Local&lt;/i&gt; - Emulate a local server.&lt;br /&gt;&lt;i&gt;Server&lt;/i&gt; - Request loopback from server.&lt;br /&gt;Please note than when loopback is enabled, no other players will hear your voice. This setting is not saved on application exit.</source>
        <translation type="obsolete">Ta opcja uaktywnia tryb testowy.</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="449"/>
        <source>Factor for sound volume increase</source>
        <translation>&lt;b&gt;Jak bardzo Mumble powinien zmieniać głośność dla źródeł dźwięku, które są zbyt blisko?&lt;/b&gt;
Ustaw odpowiednią liczbę %. Domyślnie 50%.</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="287"/>
        <source>The connected &quot;speakers&quot; are actually headphones</source>
        <oldsource>The connected &quot;speakers&quot; are actually headphones.</oldsource>
        <translation type="unfinished">Podłączone &quot;głośniki&quot; to w rzeczywistości słuchawki.</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="300"/>
        <source>Minimum Distance</source>
        <translation>Minimalna odległość</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="345"/>
        <source>Maximum Distance</source>
        <translation>Maksymalna odległość</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="355"/>
        <source>Maximum distance, beyond which speech volume won&apos;t decrease</source>
        <translation>Ta opcja ustawia maksymalną odległość słyszalności dźwięku. W zależności od ustawienia odległości, mowa innych użytkowników nie będzie słyszana głośniej.</translation>
    </message>
    <message>
        <source>This sets the maximum distance for sound calculations. When farther away than this, other players&apos; speech volume will not decrease any further.</source>
        <translation type="obsolete">Ta opcja ustawia maksymalną odległość słyszalności dźwięku. W zależności od ustawienia odległości, mowa innych użytkowników nie będzie słyszana głośniej.</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="387"/>
        <source>Minimum Volume</source>
        <translation>Minimalna głośność</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="400"/>
        <source>What should the volume be at the maximum distance?</source>
        <translation>Jaka powinna być głośność maksymalnej odległości?</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="452"/>
        <source>How much should sound volume increase for sources that are really close?</source>
        <translation>&lt;b&gt;Jak bardzo Mumble powinien zmieniać głośność dla źródeł dźwięku, które są zbyt blisko?&lt;/b&gt;
Ustaw odpowiednią liczbę %. Domyślnie 50%.</translation>
    </message>
    <message>
        <source>Connected &quot;speakers&quot; are actually headphones.</source>
        <translation type="obsolete">&lt;b&gt;Jeśli w tej chwli używasz słuchawek&lt;/b&gt; to zaznacz tę opcję. Jest to ważne, bo zazwyczaj głośniki są przed tobą a słuchawki bezpośrednio z lewej i prawej.</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="290"/>
        <source>Checking this indicates that you don&apos;t have speakers connected, just headphones. This is important, as speakers are usually in front of you, while headphones are directly to your left/right.</source>
        <translation>&lt;b&gt;Jeśli w tej chwli używasz słuchawek&lt;/b&gt; to zaznacz, że nie posiadasz głośników tylko słuchawki. Jest to ważne, bo zazwyczaj głośniki są przed tobą a słuchawki bezpośrednio z lewej i prawej.</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="293"/>
        <source>Headphones</source>
        <translation>W tej chwili używam słuchawek</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="246"/>
        <source>Other Applications</source>
        <translation>Inne aplikacje</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="213"/>
        <source>This sets the amount of data to pre-buffer in the output buffer. Experiment with different values and set it to the lowest which doesn&apos;t cause rapid jitter in the sound.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="256"/>
        <source>Volume of other applications during speech</source>
        <translation>Głośność innych aplikacji podczas rozmowy. &lt;b&gt;UWAGA!&lt;/b&gt; Działa tylko na Windows Vista / 7.</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="259"/>
        <source>&lt;b&gt;Decrease volume of other applications during speech.&lt;/b&gt;&lt;br /&gt;Mumble supports decreasing the volume of other applications during incoming speech. This sets the relative volume of other applications when others are talking.</source>
        <translation>&lt;b&gt;Zmniejsza głośność innych aplikacji podczas rozmowy.&lt;/b&gt;&lt;br /&gt;Mumble wspiera obniżanie głośności innych aplikacji podczas nadchodzącej rozmowy. Ta opcja ustawia względną głośność innych aplikacji kiedy inni mówią.
&lt;b&gt;UWAGA!&lt;/b&gt; Działa tylko na Windows VISTA SP1.</translation>
    </message>
</context>
<context>
    <name>AudioOutputDialog</name>
    <message>
        <location filename="AudioConfigDialog.cpp" line="394"/>
        <source>None</source>
        <translation>wyłącz tryb testowy</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="395"/>
        <source>Local</source>
        <translation>test lokalny</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="396"/>
        <source>Server</source>
        <translation>test na serwerze</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="402"/>
        <source>Audio Output</source>
        <translation>Wyjście audio</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="495"/>
        <location filename="AudioConfigDialog.cpp" line="514"/>
        <source>%1 ms</source>
        <translation>%1 ms</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="506"/>
        <location filename="AudioConfigDialog.cpp" line="510"/>
        <location filename="AudioConfigDialog.cpp" line="518"/>
        <location filename="AudioConfigDialog.cpp" line="549"/>
        <location filename="AudioConfigDialog.cpp" line="553"/>
        <source>%1%</source>
        <translation>%1%</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="522"/>
        <source>%1ms</source>
        <translation>%1 ms</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="537"/>
        <location filename="AudioConfigDialog.cpp" line="543"/>
        <source>%1m</source>
        <translation>%1m</translation>
    </message>
    <message>
        <source>%1</source>
        <translation type="obsolete">%1</translation>
    </message>
</context>
<context>
    <name>AudioOutputSample</name>
    <message>
        <location filename="AudioOutput.cpp" line="295"/>
        <source>Choose sound file</source>
        <translation type="unfinished">Wybierz plik dźwiękowy</translation>
    </message>
    <message>
        <location filename="AudioOutput.cpp" line="299"/>
        <source>Invalid sound file</source>
        <translation type="unfinished">Nieprawidłowy plik dźwiękowy</translation>
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
        <location filename="AudioStats.ui" line="28"/>
        <source>Peak microphone level</source>
        <translation></translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="48"/>
        <source>Peak speaker level</source>
        <translation></translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="68"/>
        <source>Peak clean level</source>
        <translation></translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="78"/>
        <source>This shows the peak power in the last frame (20 ms) after all processing. Ideally, this should be -96 dB when you&apos;re not talking. In reality, a sound studio should see -60 dB, and you should hopefully see somewhere around -20 dB. When you are talking, this should rise to somewhere between -5 and -10 dB.&lt;br /&gt;If you are using echo cancellation, and this rises to more than -15 dB when you&apos;re not talking, your setup is not working, and you&apos;ll annoy other users with echoes.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="137"/>
        <source>Speech Probability</source>
        <translation>Prawdopodobieństwo mowy</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="35"/>
        <location filename="AudioStats.ui" line="55"/>
        <location filename="AudioStats.ui" line="75"/>
        <source>Peak power in last frame</source>
        <translation></translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="58"/>
        <source>This shows the peak power of the speakers in the last frame (20 ms). Unless you are using a multi-channel sampling method (such as ASIO) with speaker channels configured, this will be 0. If you have such a setup configured, and this still shows 0 while you&apos;re playing audio from other programs, your setup is not working.</source>
        <translation>Ta opcja pokazuje moc głośników w ostatniej ramce (20 ms). Jednak jeśli używasz wielokanałowej metody próbkowania (np. ASIO) ze skonfigurowanymi kanałami ta opcja będzie wynosić 0. Jeśli posiadasz taki sprzęt skonfigurowany, a ta opcja nadal pokazuje 0, gdy odtwarzasz dźwięk z innych programów to znaczy, że twoje ustawienie nie działa prawidłowo.</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="104"/>
        <source>How close the current input level is to ideal</source>
        <translation>Jak blisko obecny poziom głośności jest bliski idealnemu</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="124"/>
        <source>Signal-To-Noise ratio from the microphone</source>
        <translation>Analiza na podstawie czystego sygnału.</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="127"/>
        <source>This is the Signal-To-Noise Ratio (SNR) of the microphone in the last frame (20 ms). It shows how much clearer the voice is compared to the noise.&lt;br /&gt;If this value is below 1.0, there&apos;s more noise than voice in the signal, and so quality is reduced.&lt;br /&gt;There is no upper limit to this value, but don&apos;t expect to see much above 40-50 without a sound studio.</source>
        <translation>Analiza na podstawie czystego sygnału.</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="144"/>
        <source>Probability of speech</source>
        <translation>Prawdopodobieństwo mowy</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="147"/>
        <source>This is the probability that the last frame (20 ms) was speech and not environment noise.&lt;br /&gt;Voice activity transmission depends on this being right. The trick with this is that the middle of a sentence is always detected as speech; the problem is the pauses between words and the start of speech. It&apos;s hard to distinguish a sigh from a word starting with &apos;h&apos;.&lt;br /&gt;If this is in bold font, it means Mumble is currently transmitting (if you&apos;re connected).</source>
        <translation></translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="181"/>
        <source>Bitrate of last frame</source>
        <translation>Bitrate ostatniej ramki</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="184"/>
        <source>This is the audio bitrate of the last compressed frame (20 ms), and as such will jump up and down as the VBR adjusts the quality. To adjust the peak bitrate, adjust &lt;b&gt;Compression Complexity&lt;/b&gt; in the Settings dialog.</source>
        <translation></translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="207"/>
        <source>Time between last two Push-To-Talk presses</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="262"/>
        <source>Power spectrum of input signal and noise estimate</source>
        <translation>Siła spektrum sygnału i szacowanie poziomu hałasu</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="265"/>
        <source>This shows the power spectrum of the current input signal (red line) and the current noise estimate (filled blue).&lt;br /&gt;All amplitudes are multiplied by 30 to show the interesting parts (how much more signal than noise is present in each waveband).&lt;br /&gt;This is probably only of interest if you&apos;re trying to fine-tune noise conditions on your microphone. Under good conditions, there should be just a tiny flutter of blue at the bottom. If the blue is more than halfway up on the graph, you have a seriously noisy environment.</source>
        <translation></translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="293"/>
        <source>Weights of the echo canceller</source>
        <translation>Ciężkość usuwania echa</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="296"/>
        <source>This shows the weights of the echo canceller, with time increasing downwards and frequency increasing to the right.&lt;br /&gt;Ideally, this should be black, indicating no echo exists at all. More commonly, you&apos;ll have one or more horizontal stripes of bluish color representing time delayed echo. You should be able to see the weights updated in real time.&lt;br /&gt;Please note that as long as you have nothing to echo off, you won&apos;t see much useful data here. Play some music and things should stabilize. &lt;br /&gt;You can choose to view the real or imaginary parts of the frequency-domain weights, or alternately the computed modulus and phase. The most useful of these will likely be modulus, which is the amplitude of the echo, and shows you how much of the outgoing signal is being removed at that time step. The other viewing modes are mostly useful to people who want to tune the echo cancellation algorithms.&lt;br /&gt;Please note: If the entire image fluctuates massively while in modulus mode, the echo canceller fails to find any correlation whatsoever between the two input sources (speakers and microphone). Either you have a very long delay on the echo, or one of the input sources is configured wrong.</source>
        <translation></translation>
    </message>
    <message>
        <location filename="AudioStats.cpp" line="384"/>
        <source>&gt;1000ms</source>
        <translation>&gt;1000ms</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="14"/>
        <source>Audio Statistics</source>
        <translation>Statystyki</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="22"/>
        <source>Input Levels</source>
        <translation>Poziomy wejścia</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="38"/>
        <source>This shows the peak power in the last frame (20 ms), and is the same measurement as you would usually find displayed as &quot;input power&quot;. Please disregard this and look at &lt;b&gt;Microphone power&lt;/b&gt; instead, which is much more steady and disregards outliers.</source>
        <translation></translation>
    </message>
    <message>
        <source>TextLabel</source>
        <translation type="obsolete">Tekst</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="91"/>
        <source>Signal Analysis</source>
        <translation>Analiza sygnału</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="97"/>
        <source>Microphone power</source>
        <translation>Siła mikrofonu</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="107"/>
        <source>This shows how close your current input volume is to the ideal. To adjust your microphone level, open whatever program you use to adjust the recording volume, and look at the value here while talking.&lt;br /&gt;&lt;b&gt;Talk loud, as you would when you&apos;re upset over getting fragged by a noob.&lt;/b&gt;&lt;br /&gt;Adjust the volume until this value is close to 100%, but make sure it doesn&apos;t go above. If it does go above, you are likely to get clipping in parts of your speech, which will degrade sound quality.</source>
        <translation></translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="117"/>
        <source>Signal-To-Noise ratio</source>
        <translation>Analiza na podstawie czystego sygnału</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="162"/>
        <source>Configuration feedback</source>
        <translation>Konfiguracja</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="168"/>
        <source>Current audio bitrate</source>
        <translation>Bitrate audio</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="194"/>
        <source>DoublePush interval</source>
        <translation>Szybkość podwójnego kliknięcia (aktywuje ciągłą transmisję głosu, ponowne podwójne kliknięcie wyłącza ją).</translation>
    </message>
    <message>
        <source>Time between last two PushToTalk presses</source>
        <translation type="obsolete">Czas pomiędzy dwoma ostatnimi naciśnięciami klawisza do mówienia</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="217"/>
        <source>Speech Detection</source>
        <translation>Wykrywanie mowy</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="224"/>
        <source>Current speech detection chance</source>
        <translation>Aktualna szansa na wykrycie</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="227"/>
        <source>&lt;b&gt;This shows the current speech detection settings.&lt;/b&gt;&lt;br /&gt;You can change the settings from the Settings dialog or from the Audio Wizard.</source>
        <translation>&lt;b&gt;Ta opcja pokazuje obecny stan sposobu transmisji głosu.&lt;/b&gt;&lt;br /&gt;Możesz zmienić te ustawienia z menu Ustawienia lub korzystając z Kreatora ustawień dźwięku.</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="256"/>
        <source>Signal and noise power spectrum</source>
        <translation>Widmo hałasu i siły</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="281"/>
        <source>Echo Analysis</source>
        <translation>Analiza echa</translation>
    </message>
</context>
<context>
    <name>AudioWizard</name>
    <message>
        <location filename="AudioWizard.ui" line="14"/>
        <source>Audio Tuning Wizard</source>
        <translation>Kreator ustawień dźwięku</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="18"/>
        <source>Introduction</source>
        <translation>Wprowadzenie</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="21"/>
        <source>Welcome to the Mumble Audio Wizard</source>
        <translation>Witamy w kreatorze ustawień dźwięku Mumble!</translation>
    </message>
    <message>
        <source>This is the audio tuning wizard for Mumble. This will help you correctly set the input levels of your sound card, and also set the correct parameters for sound processing in Mumble.</source>
        <translation type="obsolete">Dzięki temu kreatorowi poprawnie skonfigurujesz program Mumble!

</translation>
    </message>
    <message>
        <source>Please be aware that as long as this wizard is active, audio will be looped locally to allow you to listen to it, and no audio will be sent to the server.</source>
        <translation type="obsolete">Podczas działania tego kreatora dźwięk nadawany jest tak abyś sam siebie słyszał - przy czym nikt inny na żadnym serwerze cię nie usłyszy.

Przejdź Dalej, aby rozpocząć konfigurację!</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="545"/>
        <source>Finished</source>
        <translation>Zakończono</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="548"/>
        <source>Enjoy using Mumble</source>
        <translation>Miłej zabawy z Mumble!</translation>
    </message>
    <message>
        <source>Congratulations. You should now be ready to enjoy a richer sound experience with Mumble.</source>
        <translation type="obsolete">Gratulujemy! Od teraz powinieneś cieszyć się lepszą jakością dźwięku z Mumble.</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="56"/>
        <source>Device selection</source>
        <translation>Wybór urządzenia</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="59"/>
        <source>Selecting the input and output device to use with Mumble.</source>
        <translation>Wybierz urządzenia wejścia/wyjścia, z których Mumble będzie korzystać.</translation>
    </message>
    <message>
        <source>Input device</source>
        <translation type="obsolete">Urządzenie wejścia</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="71"/>
        <source>This is the device your microphone is connected to.</source>
        <translation>Urządzenie, do którego podłączony jest mikrofon.</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="81"/>
        <location filename="AudioWizard.ui" line="153"/>
        <source>System</source>
        <translation>System</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="91"/>
        <source>Input method for audio</source>
        <translation>System wejścia audio</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="94"/>
        <source>&lt;b&gt;This is the input method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation>&lt;b&gt;System wyjścia audio.&lt;/b&gt;&lt;br /&gt;DirectSound zalecane.</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="101"/>
        <location filename="AudioWizard.ui" line="173"/>
        <source>Device</source>
        <translation>Urządzenie</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="111"/>
        <source>Input device to use</source>
        <translation>Urządzenie wejścia</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="114"/>
        <source>&lt;b&gt;Selects which sound card to use for audio input.&lt;/b&gt;</source>
        <translation>&lt;b&gt;Zaznacza, które urządzenie użyć&lt;/b&gt;</translation>
    </message>
    <message>
        <source>Output device</source>
        <translation type="obsolete">Urządzenie wyjścia</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="143"/>
        <source>This is the device your speakers or headphones are connected to.</source>
        <translation>Urządzenie, do którego podłączączone są słuchawki/głośniki.</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="163"/>
        <source>Output method for audio</source>
        <translation>System wyjścia audio</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="166"/>
        <source>&lt;b&gt;This is the Output method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation>&lt;b&gt;System wyjścia audio.&lt;/b&gt;&lt;br /&gt;DirectSound zalecane.</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="183"/>
        <source>Output device to use</source>
        <translation>Urządzenie wyjścia</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="186"/>
        <source>&lt;b&gt;Selects which sound card to use for audio Output.&lt;/b&gt;</source>
        <translation>&lt;b&gt;Zaznacza, które urządzenie użyć&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="306"/>
        <source>Volume tuning</source>
        <translation>Regulacja głośności</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="309"/>
        <source>Tuning microphone hardware volume to optimal settings.</source>
        <translation>Ustawienie optymalnej głośności mikrofonu.</translation>
    </message>
    <message>
        <source>Open your sound control panel and go to the recording settings. Make sure the microphone is selected as active input with maximum recording volume. If there&apos;s an option to enable a &quot;Microphone boost&quot; make sure it&apos;s checked.</source>
        <translation type="obsolete">Wejdź do panelu regulacji głośności Windows (Start&gt;Uruchom&gt;sndvol32) i upewnij się, że mikrofon jest wybrany jako urządzenie do przechwytywania dźwięku. Użyj opcji &quot;Mic Boost&quot; jeżeli jest dostępna.</translation>
    </message>
    <message>
        <source>Speak loudly, as when you are annoyed or excited. Decrease the volume in the sound control panel until the bar below stays as high as possible in the blue and green but &lt;b&gt;not&lt;/b&gt; the red zone while you speak.</source>
        <translation type="obsolete">Mów głośno, tak jakbyś był podekscytowany albo wkurzony. Zmniejszaj stopniowo głośność w panelu aż pasek poniżej zostanie tak wysoko jak to możliwe w niebiesko-zielonym kolorze (nie może być w czerwonym).</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="341"/>
        <source>Now talk softly, as you would when talking late at night and you don&apos;t want to disturb anyone. Adjust the slider below so that the bar moves into green when you talk, but stays blue while you&apos;re silent.</source>
        <translation>Teraz mów łagodnie tak jakbyś mówił późno w nocy, aby nikogo nie obudzić.</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="381"/>
        <source>Voice Activity Detection</source>
        <translation>Wybór sposobu aktywacji głosowej</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="384"/>
        <source>Letting Mumble figure out when you&apos;re talking and when you&apos;re silent.</source>
        <translation>Teraz pozwól Mumble ustalić, kiedy słyszany jest twój głos a kiedy panuje cisza.</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="390"/>
        <source>This will help Mumble figure out when you are talking. The first step is selecting which data value to use.</source>
        <translation>Wskaż sposób aktywacji, którego chcesz używać.</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="421"/>
        <source>Raw amplitude from input</source>
        <translation>Analiza na podstawie siły głosu</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="414"/>
        <source>Signal-To-Noise ratio</source>
        <translation>Analiza na podstawie czystego sygnału</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="444"/>
        <source>Next you need to adjust the following slider. The first few utterances you say should end up in the green area (definitive speech). While talking, you should stay inside the yellow (might be speech) and when you&apos;re not talking, everything should be in the red (definitively not speech).</source>
        <oldsource>Next you need to adjust the following two sliders. The first few utterances you say should end up in the green area (definitive speech). While talking, you should stay inside the yellow (might be speech) and when you&apos;re not talking, everything should be in the red (definitively not speech).</oldsource>
        <translation type="unfinished">Następnie musisz ustawić podane wskaźniki.
Zielony - mowa zdecydowana, żółty - mowa normalna, czerwony - gdy nic nie mówisz.</translation>
    </message>
    <message>
        <source>Last, you need to adjust the grace time. Mumble may detect this much non-speech and still keep transmitting before it breaks off. This allows you to catch your breath while speaking.</source>
        <translation type="obsolete">Ostatecznie ustaw jak długo Mumble podtrzymuje transmisję po zakończeniu wypowiedzi.
W ten sposób możesz uniknąć ucinania głosu pomiędzy wypowiadanymi zdaniami.</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="223"/>
        <source>Device tuning</source>
        <translation>Ustawianie urządzenia</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="226"/>
        <source>Changing hardware output delays to their minimum value.</source>
        <translation>Zmiana opóźnień sprzętu do minimalnych wartości.</translation>
    </message>
    <message>
        <source>To keep latency to an absolute minium, it&apos;s important to buffer as little audio as possible on the soundcard. However, many soundcards report that they require a much smaller buffer than what they can actually work with, so the only way to set this value is to try and fail.</source>
        <translation type="obsolete">Aby opóźnienia były najmniejsze bufor powinien być jak najmniejszy, wówczas karta dźwiękowa szybciej wysyła dźwięk.
Niektóre karty dźwiękowe mają granice co do ustawień bufora, raz działają a raz nie.
Postaraj się ustawić wielkość bufora tak, aby nie występowały żadne zakłócenia w odtwarzanym dźwięku.</translation>
    </message>
    <message>
        <source>Amount of data to buffer.</source>
        <translation type="obsolete">Ilość danych do zbuforowania</translation>
    </message>
    <message>
        <source>This sets the amount of data to prebuffer in the output buffer. Experiment with different values and set it to the lowest which doesn&apos;t cause rapid jitter in the sound.</source>
        <translation type="obsolete">Ustaw jak najniżej możliwą wartość (UWAGA! Twoja karta dźwiękowa może nie działać na bardzo niskich wartościach).</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="273"/>
        <source>%1ms</source>
        <translation>%1ms</translation>
    </message>
    <message>
        <source>%1 s</source>
        <translation type="obsolete">%1 s</translation>
    </message>
    <message>
        <source>You should hear a single tone that&apos;s changing in frequency. Change the slider below to the lowest value which gives &lt;b&gt;no&lt;/b&gt; interruptions or jitter in the sound. Please note that local echo is disabled during this test to improve audio path recognition.</source>
        <translation type="obsolete">Powinieneś usłyszeć jeden dźwięk który ciągle zmienia częstotliwość. Zmień suwakiem wartość do najmniejszej a jednocześnie do takiej przy, której nie występują wahania dźwięku. Echo podczas tego testu jest wyłączone.</translation>
    </message>
    <message>
        <source>If you position the microphone so it can pick up the speakers or headset, Mumble will measure the total audio path delay in your system; this means the delay from a sample is placed in an outbound buffer until it&apos;s found in a matching incoming buffer.</source>
        <translation type="obsolete">Jeśli ustawisz swój mikrofon tak, aby przechwytywał dźwięk z głośników lub słuchawek to Mumble automatycznie ustawi opóźnienia audio tak, aby inni użytkownicy nie słyszeli dźwięków otoczenia (np. pisania na klawiaturze, oddechu).</translation>
    </message>
    <message>
        <source>Audio path is %1ms long.</source>
        <translation type="obsolete">Ścieżka dźwiękowa ma długość %1ms</translation>
    </message>
    <message>
        <source>Audio path cannot be determined. Input not recognized.</source>
        <translation type="obsolete">Ścieżka dźwiękowa nie może być określona.
Wejście dźwieku nie zostało rozpoznane!</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="127"/>
        <source>Use echo cancellation</source>
        <translation>Usuwanie akustycznego echa</translation>
    </message>
    <message>
        <source>Cancel echo from headset or speakers.</source>
        <translation type="obsolete">Ta opcja usuwa echo ze słuchawek lub głośników.</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="124"/>
        <source>This enables echo cancellation of outgoing audio, which helps both on speakers and on headsets.</source>
        <translation>Uaktywnia usuwanie echa pochodzącego z urządzeń (głośniki lub słuchawki).</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="199"/>
        <source>Enable positional audio</source>
        <translation>Uaktywnij dźwięk pozycyjny</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="193"/>
        <source>Allows positioning of sound.</source>
        <translation>Uaktywnia pozycjonowanie dźwięku.</translation>
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
        <location filename="AudioWizard.ui" line="121"/>
        <source>Cancel echo from headset or speakers</source>
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
        <translation>Pozwala to Mumble na używanie pozycjonowania dźwięku aby umieszczać głosy w różny sposób.</translation>
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
        <translation type="unfinished">Ilość danych trafiających do bufora.</translation>
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
        <translation>Dźwięk pozycyjny</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="493"/>
        <source>Adjusting attenuation of positional audio.</source>
        <translation>Szczegółowe ustawienia dźwięku pozycyjnego.</translation>
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
        <translation type="obsolete">Mumble wspiera dźwięk pozycyjny w kilku grach.</translation>
    </message>
    <message>
        <source>The graph below shows the position of &lt;font color=&quot;red&quot;&gt;you&lt;/font&gt;, the &lt;font color=&quot;yellow&quot;&gt;speakers&lt;/font&gt; and a &lt;font color=&quot;green&quot;&gt;moving sound source&lt;/font&gt; as if seen from above. You should hear the audio move between the channels.</source>
        <translation type="obsolete">Wykres poniżej pokazuje &lt;font color=&quot;red&quot;&gt;ciebie&lt;/font&gt;, &lt;font color=&quot;yellow&quot;&gt;głośniki&lt;/font&gt; oraz &lt;font color=&quot;green&quot;&gt;poruszające źródło dźwięku&lt;/font&gt;.
Powinieneś usłyszeć dźwięk przemieszczający się pomiędzy głośnikami.</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="521"/>
        <source>Use headphones</source>
        <translation>Zaznacz tą opcję jeśli masz podłączone słuchawki</translation>
    </message>
    <message>
        <source>Use headphones instead of speakers.</source>
        <translation type="obsolete">Używaj słuchawek zamiast głośników.</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="518"/>
        <source>This ignores the OS speaker configuration and configures the positioning for headphones instead.</source>
        <translation>Dzięki tej opcji ignorujesz ustawienia w systemie i wymuszasz słuchawki.</translation>
    </message>
    <message>
        <source>Mumble is under continuous development, and the development team wants to focus on the features that benefit the most users. To this end, Mumble supports submitting anonymous statistics about your configuration to the developers. These statistcs are essential for future development, and also make sure the features you use aren&apos;t deprecated.</source>
        <translation type="obsolete">Adnotacja twórców:
Mumble jest ciągle rozwijany, naszym priorytetem jest skupianie się na cechach najbardziej pożądanych przez użytkowników, dlatego dodaliśmy do programu opcję wysyłania anonimowych statystyk.
Dane te zawierają konfigurację twojego klienta i są nam bardzo potrzebne do przyszłego rozwoju aplikacji.</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="570"/>
        <source>Submit anonymous statistics to the Mumble project</source>
        <translation>Zezwalaj na wysyłanie anonimowych statystyk do twórców programu</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="402"/>
        <source>Push To Talk:</source>
        <translation>Aktywacja wybranym przyciskiem:</translation>
    </message>
</context>
<context>
    <name>BanEditor</name>
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
        <translation>&amp;Dodaj</translation>
    </message>
    <message>
        <location filename="BanEditor.ui" line="152"/>
        <source>&amp;Update</source>
        <translation>&amp;Aktualizuj</translation>
    </message>
    <message>
        <location filename="BanEditor.ui" line="159"/>
        <source>&amp;Remove</source>
        <translation>&amp;Usuń</translation>
    </message>
    <message>
        <location filename="BanEditor.ui" line="14"/>
        <source>Mumble - Edit Bans</source>
        <translation>Mumble - edycja banów</translation>
    </message>
    <message>
        <source>0.0.0.0</source>
        <translation type="obsolete">0.0.0.0</translation>
    </message>
    <message>
        <location filename="BanEditor.ui" line="29"/>
        <source>&amp;Address</source>
        <translation>&amp;Adres IP</translation>
    </message>
    <message>
        <location filename="BanEditor.ui" line="39"/>
        <source>&amp;Mask</source>
        <translation>&amp;Maska</translation>
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
        <location filename="Cert.cpp" line="93"/>
        <source>(none)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.cpp" line="99"/>
        <source>Self-signed</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>CertWizard</name>
    <message>
        <location filename="Cert.cpp" line="184"/>
        <source>Resolving domain %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.cpp" line="190"/>
        <source>Unable to validate email.&lt;br /&gt;Enter a valid (or blank) email to continue.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.cpp" line="198"/>
        <source>There was an error generating your certificate.&lt;br /&gt;Please try again.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.cpp" line="206"/>
        <source>Your certificate and key could not be exported to PKCS#12 format. There might be an error in your certificate.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.cpp" line="211"/>
        <source>The file could not be opened for writing. Please use another file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.cpp" line="217"/>
        <source>The file could not be written successfully. Please use another file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.cpp" line="224"/>
        <source>The file could not be opened for reading. Please use another file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.cpp" line="230"/>
        <source>The file is empty or could not be read. Please use another file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.cpp" line="235"/>
        <source>The file did not contain a valid certificate and key. Please use another file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.cpp" line="257"/>
        <source>Select file to export certificate to</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.cpp" line="294"/>
        <source>Select file to import certificate from</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.cpp" line="347"/>
        <source>Unable to resolve domain.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.cpp" line="390"/>
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
        <location filename="Cert.ui" line="17"/>
        <source>This opens a file selection dialog to choose a file to export the certificate to.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="21"/>
        <source>Certificate Authentication</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="24"/>
        <source>Authenticating to servers without using passwords</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="33"/>
        <source>&lt;p&gt;Mumble can use certificates to authenticate with servers. Using certificates avoids passwords, meaning you don&apos;t need to disclose any password to the remote site. It also enables very easy user registration.&lt;/p&gt;&lt;p&gt;While Mumble can work without certificates, the majority of servers will expect you to have one.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="43"/>
        <location filename="Cert.ui" line="264"/>
        <location filename="Cert.ui" line="351"/>
        <source>Current certificate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="46"/>
        <source>This is the certificate Mumble currently uses.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="49"/>
        <location filename="Cert.ui" line="270"/>
        <source>Current Certificate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="56"/>
        <location filename="Cert.ui" line="62"/>
        <source>Create a new certificate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="59"/>
        <source>This will create a new certificate.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="82"/>
        <source>Import certificate from file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="85"/>
        <source>This will import a certificate from file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="88"/>
        <source>Import a certificate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="108"/>
        <location filename="Cert.ui" line="297"/>
        <source>Export Certificate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="111"/>
        <source>This will export a certificate to file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="114"/>
        <source>Export current certificate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="132"/>
        <source>Import Certificate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="135"/>
        <source>PKCS #12 Certificate import</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="144"/>
        <source>&lt;p&gt;Mumble can import certificates stored in PKCS #12 format. This is the format used when exporting a key from Mumble, and also when exporting keys from Firefox, Internet Explorer, Opera etc.&lt;/p&gt;&lt;p&gt;If the file is password protected, you will need the password to import the certificate.&lt;/p&gt;</source>
        <oldsource>&lt;p&gt;Mumble can import certificates stored in PKCS #12 format. This is the format used when exporting a key from Mumble, and also when exporting keys from FireFox, Internet Explorer, Opera etc.&lt;/p&gt;&lt;p&gt;If the file is password protected, you will need the password to import the certificate.&lt;/p&gt;</oldsource>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="154"/>
        <source>Import from</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="164"/>
        <source>Filename to import from</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="167"/>
        <source>This is the filename you wish to import a certificate from.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="174"/>
        <source>Select file to import from</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="177"/>
        <source>This opens a file selection dialog to choose a file to import a certificate from.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="180"/>
        <source>Open...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="191"/>
        <source>Password</source>
        <translation type="unfinished">Hasło</translation>
    </message>
    <message>
        <location filename="Cert.ui" line="201"/>
        <source>Password for PKCS#12 file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="204"/>
        <source>This is the password for the PKCS#12 file containing your certificate.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="211"/>
        <source>Certificate to import</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="214"/>
        <source>This is the certificate you are importing.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="217"/>
        <location filename="Cert.ui" line="357"/>
        <source>Certificate Details</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="225"/>
        <source>Replace Certificate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="228"/>
        <source>Replace existing certificate with new certificate?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="240"/>
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
        <location filename="Cert.ui" line="267"/>
        <source>This is the certificate Mumble currently uses. It will be replaced.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="283"/>
        <source>New certificate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="286"/>
        <source>This is the new certificate that will replace the old one.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="289"/>
        <location filename="Cert.ui" line="365"/>
        <source>New Certificate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="300"/>
        <source>Make a backup of your certificate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="309"/>
        <source>&lt;p&gt;If you ever lose your current certificate, which will happen if your computer suffers a hardware failure or you reinstall your machine, you will no longer be able to authenticate to any server you are registered on. It is therefore &lt;b&gt;mandatory&lt;/b&gt; that you make a backup of your certificate. We strongly recommend you store this backup on removable storage, such as an USB memory stick.&lt;/p&gt;
&lt;p&gt;Note that this file will not be encrypted, and if anyone gains access to it, they will be able to impersonate you, so take good care of it.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="320"/>
        <source>Export to</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="330"/>
        <source>Filename to export to</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="333"/>
        <source>This is the filename you wish to export a certificate to.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="340"/>
        <source>Save As...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="354"/>
        <source>This is the certificate Mumble currently uses. It will be exported.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="368"/>
        <source>Generate a new certificate for strong authentication</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="377"/>
        <source>&lt;p&gt;Mumble will now generate a strong certificate for authentication to servers.&lt;/p&gt;&lt;p&gt;If you wish, you may provide some additional information to be stored in the certificate, which will be presented to servers when you connect. If you provide a valid email address, you can upgrade to a CA issued email certificate later on, which provides strong identification.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="387"/>
        <source>Name</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="397"/>
        <source>Email</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="407"/>
        <source>Your email address (e.g. johndoe@mumble.info)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="410"/>
        <source>This is your email address. It is strongly recommended to provide a valid email address, as this will allow you to upgrade to a strong certificate without authentication problems.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="424"/>
        <source>Your name (e.g. John Doe)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="427"/>
        <source>This is your name, and will be filled out in the certificate. This field is entirely optional.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="435"/>
        <source>Finish</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="438"/>
        <source>Certificate-based authentication is ready for use</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Cert.ui" line="447"/>
        <source>Enjoy using Mumble with strong authentication.</source>
        <translation type="unfinished"></translation>
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
        <location filename="../ACL.cpp" line="214"/>
        <source>None</source>
        <translation>Żaden</translation>
    </message>
    <message>
        <source>Write</source>
        <translation type="obsolete">Pisanie</translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="218"/>
        <source>Traverse</source>
        <translation>Wejście na pod-kanał</translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="220"/>
        <source>Enter</source>
        <translation>Wejście</translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="222"/>
        <source>Speak</source>
        <translation>Mowa</translation>
    </message>
    <message>
        <source>AltSpeak</source>
        <translation type="obsolete">Alternatywna mowa</translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="226"/>
        <source>Mute/Deafen</source>
        <translation>Wycisz słuchawki i mikrofon</translation>
    </message>
    <message>
        <source>Move/Kick</source>
        <translation type="obsolete">Przenieś / Kopnij</translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="230"/>
        <source>Make channel</source>
        <translation>Stwórz kanał</translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="232"/>
        <source>Make temporary</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="234"/>
        <source>Link channel</source>
        <translation>Połącz kanał</translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="147"/>
        <source>This represents no privileges.</source>
        <translation>Brak przywilejów.</translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="149"/>
        <source>This represents total access to the channel, including the ability to change group and ACL information. This privilege implies all other privileges.</source>
        <translation>Przedstawia całkowity dostęp do kanału, wliczjąc możliwość zmiany grup oraz informacji ACL. Ten przywilej zawiera wszystkie pozostałem przywileje.</translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="152"/>
        <source>This represents the permission to traverse the channel. If a user is denied this privilege, he will be unable to access this channel and any sub-channels in any way, regardless of other permissions in the sub-channels.</source>
        <translation>Przedstawia możliwość wchodzenia na pod-kanały. Jeżeli użytkownik ma zabroniony dostęp do tego przywileju nie będzie w stanie w żaden sposób wejść na pod-kanały, niezależnie od innych przywilejów jakie posiada w pod-kanałach.</translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="156"/>
        <source>This represents the permission to join the channel. If you have a hierarchical channel structure, you might want to give everyone Traverse, but restrict Enter in the root of your hierarchy.</source>
        <translation>Przedstawia możliwość wejścia na kanał. Jeżeli masz hierarchiczną strukturę, możesz nadać każdemu przywilej wchodzenia na pod-kanały jednocześnie blokując dostęp do głównego kanału w hierarchii.</translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="159"/>
        <source>This represents the permission to speak in a channel. Users without this privilege will be suppressed by the server (seen as muted), and will be unable to speak until they are unmuted by someone with the appropriate privileges.</source>
        <translation>Przedstawia możliwość mowy na kanale. Użytkownicy nieposiadający tego przywileju będą automatycznie wyciszani przez serwer, oraz nie będą w stanie mówić dopóki osoba z odpowiednimi przywilejami nie zdejmie im wyciszenia.</translation>
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
        <translation type="unfinished">przenieś kanał bez pytania</translation>
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
        <source>This represents the permission to speak in a channel with flagged speech. This works exactly like the &lt;i&gt;speak&lt;/i&gt; privilege, but applies to packets spoken with AltPushToTalk held down. This may be used to broadcast to a hierarchy of channels without linking.</source>
        <translation type="obsolete">Przedstawia możliwość alternatywnej mowy. Przywilej ten działa dokładnie tak jak &lt;i&gt;mowa&lt;/i&gt;, ale dotyczy użycia przycisku alternatywnej mowy. Przywilej ten może być użyty do ogłaszania czegoś ważnego do całego drzewa kanałów bez ich łączenia.</translation>
    </message>
    <message>
        <source>This represents the permission to mute and deafen other players. Once muted, a player will stay muted until he is unmuted by another privileged player or reconnects to the server.</source>
        <translation type="obsolete">Przedstawia możliwość wyciszania użytkowników. Gdy użytkownik zostanie wyciszony pozostanie wyciszony dopóki inny uprawniony użytkownik nie zdejmie wyciszenia lub do czasu ponownego połączenia z serwerem.</translation>
    </message>
    <message>
        <source>This represents the permission to move a player to another channel or kick him from the server. To actually move the player, either the moving player must have Move/Kick privileges in the destination channel, or the player must normally be allowed to enter the channel. Players with this privilege can move players into channels the target player normally wouldn&apos;t have permission to enter.</source>
        <translation type="obsolete">Przedstawia możliwość przenoszenia użytkowników na inne kanały oraz kopnięcia ich z serwera. Aby móc przenieść użytkownika musi on posiadać przywilej przenoszenia oraz kopnięcia użytkownika lub posiadać zezwolenie na wejście na kanale docelowym. Użytkownicy z tym przywilejem mogą przenosić innych użytkowników na kanały gdzie nie  mogli by normalnie wejść.</translation>
    </message>
    <message>
        <source>This represents the permission to make sub-channels. The player making the sub-channel will be added to the admin group of the sub-channel.</source>
        <translation type="obsolete">Przedstawia możliwość tworzenia pod-kanałów. Użytkownik tworzący pod-kanał zostaje automatycznie dodany do grupy &lt;i&gt;admin&lt;/i&gt; w pod-kanale.</translation>
    </message>
    <message>
        <source>This represents the permission to link channels. Players in linked channels hear each other, as long as the speaking player has the &lt;i&gt;speak&lt;/i&gt; privilege in the channel of the listener. You need the link privilege in both channels to create a link, but just in either channel to remove it.</source>
        <translation type="obsolete">Przedstawia możliwość łączenia kanałów. Gdy mówiący użytkownik posiada przywilej &lt;i&gt;speak&lt;/i&gt; na kanale gdzie są osoby które mogą go słuchać, użytkownicy połączonych kanałów słyszą się nawzajem. Aby połączyć kanały potrzebujesz przywileju łączenia kanałów na obu kanałach i tylko w jednym by rozłączyć kanały.</translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="216"/>
        <source>Write ACL</source>
        <translation>Zapisz ACL</translation>
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
        <translation type="obsolete">Uwierzytelniony</translation>
    </message>
    <message>
        <source>Muted (server)</source>
        <translation type="obsolete">Wyciszony mikrofon (przez serwer)</translation>
    </message>
    <message>
        <source>Deafened (server)</source>
        <translation type="obsolete">Wyciszone słuchawki (przez serwer)</translation>
    </message>
    <message>
        <source>Local Mute</source>
        <translation type="obsolete">Lokalne wyciszenie</translation>
    </message>
    <message>
        <source>Muted (self)</source>
        <translation type="obsolete">Wyciszony mikrofon (przez użytkownika)</translation>
    </message>
    <message>
        <source>Deafened (self)</source>
        <translation type="obsolete">Wyciszone słuchawki (przez użytkownika)</translation>
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
        <translation type="unfinished">Uwierzytelniony</translation>
    </message>
    <message>
        <location filename="ClientUser.cpp" line="101"/>
        <source>Muted (server)</source>
        <translation type="unfinished">Wyciszony mikrofon (przez serwer)</translation>
    </message>
    <message>
        <location filename="ClientUser.cpp" line="103"/>
        <source>Deafened (server)</source>
        <translation type="unfinished">Wyciszone słuchawki (przez serwer)</translation>
    </message>
    <message>
        <location filename="ClientUser.cpp" line="105"/>
        <source>Local Mute</source>
        <translation type="unfinished">Lokalne wyciszenie</translation>
    </message>
    <message>
        <location filename="ClientUser.cpp" line="107"/>
        <source>Muted (self)</source>
        <translation type="unfinished">Wyciszony mikrofon (przez użytkownika)</translation>
    </message>
    <message>
        <location filename="ClientUser.cpp" line="109"/>
        <source>Deafened (self)</source>
        <translation type="unfinished">Wyciszone słuchawki (przez użytkownika)</translation>
    </message>
</context>
<context>
    <name>ConfigDialog</name>
    <message>
        <location filename="ConfigDialog.ui" line="14"/>
        <source>Mumble Configuration</source>
        <translation>Konfiguracja Mumble</translation>
    </message>
    <message>
        <location filename="ConfigDialog.ui" line="54"/>
        <location filename="ConfigDialogDelegate.mm" line="135"/>
        <source>Advanced</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="51"/>
        <location filename="ConfigDialog.mm" line="65"/>
        <source>Accept changes</source>
        <translation>Akceptuj zmiany</translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="52"/>
        <location filename="ConfigDialog.mm" line="66"/>
        <source>This button will accept current settings and return to the application.&lt;br /&gt;The settings will be stored to disk when you leave the application.</source>
        <translation>Ten przycisk zaakceptuje zmiany i powróci do aplikacji.&lt;br /&gt;Ustawienia zostaną zapisane na dysku jak zamkniesz program.</translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="56"/>
        <location filename="ConfigDialog.mm" line="70"/>
        <source>Reject changes</source>
        <translation>Odrzuć zmiany</translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="57"/>
        <location filename="ConfigDialog.mm" line="71"/>
        <source>This button will reject all changes and return to the application.&lt;br /&gt;The settings will be reset to the previous positions.</source>
        <translation>Ten przycisk odrzuca zmiany i powraca do aplikacji.</translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="61"/>
        <location filename="ConfigDialog.mm" line="75"/>
        <source>Apply changes</source>
        <translation>Zastosuj zmiany</translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="62"/>
        <location filename="ConfigDialog.mm" line="76"/>
        <source>This button will immediately apply all changes.</source>
        <translation>Ten przycisk natychmiastowo uwzględnia zmiany.</translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="65"/>
        <location filename="ConfigDialog.mm" line="79"/>
        <source>Undo changes for current page</source>
        <translation>Cofnij zmiany na danej stronie</translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="66"/>
        <location filename="ConfigDialog.mm" line="80"/>
        <source>This button will revert any changes done on the current page to the most recent applied settings.</source>
        <translation>Cofa zmiany na danej stronie.</translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="69"/>
        <location filename="ConfigDialog.mm" line="83"/>
        <source>Restore defaults for current page</source>
        <translation>Przywróć domyślne dla danej strony</translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="70"/>
        <location filename="ConfigDialog.mm" line="84"/>
        <source>This button will restore the settings for the current page only to their defaults. Other pages will not be changed.&lt;br /&gt;To restore all settings to their defaults, you will have to use this button on every page.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This button will restore the settings for the current page only to their defaults. Other pages will be not be changed.&lt;br /&gt;To restore all settings to their defaults, you will have to use this button on every page.</source>
        <translation type="obsolete">Przywraca domyślne ustawienia dla danej strony.</translation>
    </message>
    <message>
        <source>Show all configuration items</source>
        <translation type="obsolete">Pokazuj wszystkie pozycje do skonfigurowania</translation>
    </message>
    <message>
        <source>&lt;b&gt;This will show all configuration items.&lt;/b&gt;&lt;br /&gt;Mumble contains a lot of configuration items that most users won&apos;t need to change. Checking this will show all configurable items.</source>
        <translation type="obsolete">&lt;b&gt;Pokazuj wszystkie pozycje do skonfigurowania</translation>
    </message>
    <message>
        <source>Expert Config</source>
        <translation type="obsolete">Zaawansowane</translation>
    </message>
    <message>
        <source>Tab 1</source>
        <translation type="obsolete">Zakładka 1</translation>
    </message>
</context>
<context>
    <name>ConnectDialog</name>
    <message>
        <source>A&amp;ddress</source>
        <translation type="obsolete">A&amp;dres</translation>
    </message>
    <message>
        <source>&amp;Port</source>
        <translation type="obsolete">&amp;Port</translation>
    </message>
    <message>
        <source>&amp;Username</source>
        <translation type="obsolete">&amp;Użytkownik</translation>
    </message>
    <message>
        <source>&amp;Password</source>
        <translation type="obsolete">&amp;Hasło</translation>
    </message>
    <message>
        <source>&amp;Connect</source>
        <translation type="obsolete">&amp;Połącz</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Anuluj</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation type="obsolete">&amp;Dodaj</translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation type="obsolete">&amp;Usuń</translation>
    </message>
    <message>
        <source>&amp;Custom Servers</source>
        <translation type="obsolete">&amp;Niestandardowe serwery</translation>
    </message>
    <message>
        <source>Server &amp;Browser</source>
        <translation type="obsolete">Przeglądarka &amp;serwerów</translation>
    </message>
    <message>
        <source>Address</source>
        <translation type="obsolete">Adres IP</translation>
    </message>
    <message>
        <source>URL</source>
        <translation type="obsolete">URL</translation>
    </message>
    <message>
        <source>C&amp;opy to custom</source>
        <translation type="obsolete">&amp;Kopiuj do niestandardowych</translation>
    </message>
    <message>
        <source>&amp;View Webpage</source>
        <translation type="obsolete">&amp;Zobacz WWW</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="838"/>
        <source>Connecting to %1</source>
        <translation>Podłączanie do %1</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="386"/>
        <location filename="ConnectDialog.cpp" line="838"/>
        <source>Enter username</source>
        <translation>Nazwa użytkownika</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="386"/>
        <source>Adding host %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="449"/>
        <location filename="ConnectDialog.ui" line="48"/>
        <source>Servername</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="450"/>
        <source>Hostname</source>
        <translation type="unfinished">Host</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="453"/>
        <source>Bonjour name</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="456"/>
        <source>Port</source>
        <translation type="unfinished">Port</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="457"/>
        <source>Addresses</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="460"/>
        <source>Website</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="463"/>
        <source>Packet loss</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="466"/>
        <source>Ping (80%)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="466"/>
        <location filename="ConnectDialog.cpp" line="468"/>
        <source>%1 ms</source>
        <translation type="unfinished">%1 ms</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="468"/>
        <source>Ping (95%)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="470"/>
        <source>Bandwidth</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="470"/>
        <source>%1 kbit/s</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="471"/>
        <location filename="ConnectDialog.ui" line="58"/>
        <source>Users</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="472"/>
        <source>Version</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="715"/>
        <source>Connect</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="717"/>
        <location filename="ConnectDialog.ui" line="83"/>
        <source>Add New...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="754"/>
        <source>Filters</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="1402"/>
        <source>Mumble</source>
        <translation>Mumble</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="1402"/>
        <source>Failed to fetch server list</source>
        <translation>Nie udało sie pobrać listy</translation>
    </message>
    <message>
        <source>&amp;Label</source>
        <translation type="obsolete">&amp;Nazwa</translation>
    </message>
    <message>
        <source>Label</source>
        <translation type="obsolete">Nazwa</translation>
    </message>
    <message>
        <source>-Unnamed entry-</source>
        <translation type="obsolete">Bez Nazwy</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui" line="14"/>
        <source>Mumble Server Connect</source>
        <translation>Połączenie Mumble</translation>
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
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Anuluj</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation type="obsolete">Nieznany</translation>
    </message>
    <message>
        <source>New</source>
        <translation type="obsolete">Nowy</translation>
    </message>
    <message>
        <source>Add</source>
        <translation type="obsolete">Dodaj</translation>
    </message>
    <message>
        <source>Update</source>
        <translation type="obsolete">Aktualizuj</translation>
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
        <translation type="unfinished">A&amp;dres</translation>
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
        <translation type="unfinished">&amp;Port</translation>
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
        <translation type="unfinished">&amp;Użytkownik</translation>
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
        <translation type="unfinished">Urządzenie domyślne</translation>
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
        <location filename="DirectSound.cpp" line="558"/>
        <source>Opening chosen DirectSound Input failed. Default device will be used.</source>
        <translation>Otwieranie wybranego wejścia dźwięku nie powiodło się. Domyślne urządzenie zostanie włączone.</translation>
    </message>
    <message>
        <location filename="DirectSound.cpp" line="174"/>
        <source>Default DirectSound Voice Input</source>
        <translation>Domyślne wejście dźwięku DirectSound</translation>
    </message>
    <message>
        <location filename="DirectSound.cpp" line="637"/>
        <source>Opening chosen DirectSound Input device failed. No microphone capture will be done.</source>
        <translation>Otwieranie wybranego wejścia dźwięku nie powiodło się. Dźwięk z mikrofonu nie będzie pobierany.</translation>
    </message>
    <message>
        <location filename="DirectSound.cpp" line="632"/>
        <source>Lost DirectSound input device.</source>
        <translation>Utracono urządzenie wejścia DirectSound.</translation>
    </message>
</context>
<context>
    <name>DXAudioOutput</name>
    <message>
        <location filename="DirectSound.cpp" line="464"/>
        <source>Opening chosen DirectSound Output failed. No audio will be heard.</source>
        <translation>Otwieranie wybranego wyjścia dźwięku nie powiodło się.</translation>
    </message>
    <message>
        <location filename="DirectSound.cpp" line="382"/>
        <source>Opening chosen DirectSound Output failed. Default device will be used.</source>
        <translation>Otwieranie wybranego wyjścia dźwięku nie powiodło się.</translation>
    </message>
    <message>
        <location filename="DirectSound.cpp" line="134"/>
        <source>Default DirectSound Voice Output</source>
        <translation>Domyślne wyjście dźwięku DirectSound</translation>
    </message>
    <message>
        <location filename="DirectSound.cpp" line="460"/>
        <source>Lost DirectSound output device.</source>
        <translation>Utracono urządzenie wyjścia DirectSound.</translation>
    </message>
</context>
<context>
    <name>Database</name>
    <message>
        <location filename="Database.cpp" line="83"/>
        <source>Mumble</source>
        <translation>Mumble</translation>
    </message>
    <message>
        <location filename="Database.cpp" line="83"/>
        <source>Mumble failed to initialize a database in any
of the possible locations.</source>
        <translation>Nie udało się zainicjalizować bazy danych
w żadnej z podanych lokacji.</translation>
    </message>
</context>
<context>
    <name>GlobalShortcut</name>
    <message>
        <location filename="GlobalShortcut.ui" line="17"/>
        <source>Shortcuts</source>
        <translation>Skróty</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.ui" line="65"/>
        <source>List of configured shortcuts</source>
        <translation>Lista skonfigurowanych skrótów</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.ui" line="99"/>
        <source>Function</source>
        <translation>Funkcja</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.ui" line="104"/>
        <source>Data</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="GlobalShortcut.ui" line="109"/>
        <source>Shortcut</source>
        <translation>Skrót</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.ui" line="114"/>
        <source>Suppress</source>
        <translation>Zarezerwuj</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.ui" line="23"/>
        <source>Add new shortcut</source>
        <translation>Dodaj nowy skrót</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.ui" line="26"/>
        <source>This will add a new global shortcut</source>
        <translation>Ta opcja doda nowy, globalny skrót</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.ui" line="29"/>
        <source>&amp;Add</source>
        <translation>&amp;Dodaj</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.ui" line="39"/>
        <source>Remove selected shortcut</source>
        <translation>Usuń zaznaczony skrót</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.ui" line="42"/>
        <source>This will permanently remove a selected shortcut.</source>
        <translation>Ta opcja na stałe usunie zaznaczony skrót.</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.ui" line="45"/>
        <source>&amp;Remove</source>
        <translation>&amp;Usuń</translation>
    </message>
</context>
<context>
    <name>GlobalShortcutConfig</name>
    <message>
        <location filename="GlobalShortcut.cpp" line="647"/>
        <source>Shortcuts</source>
        <translation>Skróty</translation>
    </message>
    <message>
        <source>Function</source>
        <translation type="obsolete">Funkcja</translation>
    </message>
    <message>
        <source>Shortcut</source>
        <translation type="obsolete">Skrót</translation>
    </message>
    <message>
        <source>Shortcut bound to %1.</source>
        <translation type="obsolete">Skrót przypisany do %1.</translation>
    </message>
    <message>
        <source>&lt;b&gt;This is the global shortcut bound to %1&lt;/b&gt;&lt;br /&gt;Click this field and then the desired key/button combo to rebind. Double-click to clear.</source>
        <translation type="obsolete">&lt;b&gt;To jest globalny skrót przypisany do %1&lt;/b&gt;&lt;br /&gt; Kliknij na tym polu a potem użyj jakiegoś klawisza. Aby wyczyścić dane pole kliknij dwa razy.</translation>
    </message>
    <message>
        <source>Double-click an entry to clear the shortcut.</source>
        <translation type="obsolete">Kliknij dwa razy, aby zresetować dany skrót.</translation>
    </message>
    <message>
        <source>Suppress</source>
        <translation type="obsolete">Zarezerwuj</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.cpp" line="685"/>
        <source>Shortcut button combination.</source>
        <translation>Kombinacja klawiszy</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.cpp" line="686"/>
        <source>&lt;b&gt;This is the global shortcut key combination.&lt;/b&gt;&lt;br /&gt;Double-click this field and then the desired key/button combo to rebind.</source>
        <translation>&lt;b&gt;To jest globalna kombinacja klawiszy.&lt;/b&gt;&lt;br /&gt;Podwójnie kliknij, a następnie wciskaj kombinację klawiszy aby je zapisać.</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.cpp" line="690"/>
        <source>Suppress keys from other applications</source>
        <translation>Zablokuj skrót innej aplikacji</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.cpp" line="691"/>
        <source>&lt;b&gt;This hides the button presses from other applications.&lt;/b&gt;&lt;br /&gt;Enabling this will hide the button (or the last button of a multi-button combo) from other applications. Note that not all buttons can be suppressed.</source>
        <translation>&lt;b&gt;Ta opcja ukrywa wciśnięcia przycisków przed innymi aplikacjami.&lt;/b&gt;&lt;br /&gt; Włączając tę opcję ukrywasz przycisk przed inną aplikacjją (lub ostatni z kombinacji multi-przycisków). Nie wszystkie przyciski da się tak ukryć.</translation>
    </message>
</context>
<context>
    <name>GlobalShortcutMacInit</name>
    <message>
        <location filename="GlobalShortcut_macx.cpp" line="60"/>
        <source>Mumble has detected that it is unable to receive Global Shortcut events when it is in the background.&lt;br /&gt;&lt;br /&gt;This is because the Universal Access feature called &apos;Enable access for assistive devices&apos; is currently disabled.&lt;br /&gt;&lt;br /&gt;Please &lt;a href=&quot; &quot;&gt;enable this setting&lt;/a&gt; and continue when done.</source>
        <translation>&lt;b&gt;Ta opcja pozwala na ustalenie wstrzymań pomiędzy nadawaniem głosu.&lt;/b&gt;&lt;br /&gt;Ustaw tę opcję wyżej jeśli twój głos jest niewyraźny..&lt;br /&gt;Ta opcja ma sens tyko, gdy używasz aktywacji głosowej!</translation>
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
        <translation type="unfinished">Dodaj</translation>
    </message>
    <message>
        <location filename="GlobalShortcutTarget.ui" line="129"/>
        <source>Remove</source>
        <translation type="unfinished">Usuń</translation>
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
        <translation>Mysz %1</translation>
    </message>
</context>
<context>
    <name>LCD</name>
    <message>
        <location filename="LCD.cpp" line="268"/>
        <source>Not connected</source>
        <translation>Niepołączony</translation>
    </message>
</context>
<context>
    <name>LCDConfig</name>
    <message>
        <source>Graphic</source>
        <translation type="obsolete">Grafika</translation>
    </message>
    <message>
        <source>Character</source>
        <translation type="obsolete">Tekst</translation>
    </message>
    <message>
        <location filename="LCD.cpp" line="120"/>
        <source>Enable this device</source>
        <translation>Uaktywnij te urządzenie</translation>
    </message>
    <message>
        <location filename="LCD.cpp" line="129"/>
        <source>LCD</source>
        <translation>LCD</translation>
    </message>
    <message>
        <location filename="LCD.ui" line="14"/>
        <source>Form</source>
        <translation>Formularz</translation>
    </message>
    <message>
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
        <translation type="obsolete">&lt;!DOCTYPE HTML PUBLIC &quot;-//W3C//DTD HTML 4.0//EN&quot; &quot;http://www.w3.org/TR/REC-html40/strict.dtd&quot;&gt;&lt;html&gt;&lt;head&gt;&lt;meta name=&quot;qrichtext&quot; content=&quot;1&quot; /&gt;&lt;style type=&quot;text/css&quot;&gt;p, li { white-space: pre-wrap; }&lt;/style&gt;&lt;/head&gt;&lt;body style=&quot; font-family:&apos;MS Shell Dlg 2&apos;; font-size:8.25pt; font-weight:400; font-style:normal;&quot;&gt;&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;To jest lista wszystkich dostępnych urządzeń LCD w twoim systemie.  Listuje wszystkie urządzdenia alfabetycznie oraz podaje informacje o rozmiarze ekranu czy jego typie. Mumble wspiera wyświetlanie napisów na kilku urządzeniach LCD jednocześnie.&lt;/p&gt;&lt;p style=&quot;-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;&lt;/p&gt;&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;&lt;span style=&quot; font-weight:600; font-style:italic;&quot;&gt;Typ:&lt;/span&gt;&lt;/p&gt;&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;Mumble wspierda 2 rodzaje urządzeń LCD. Graficzny ekran LCD pozwala na wypełnienie pojedyńczych pikseli na ekranie, natomiast tekstowy ekran LCD pozwala na pokazywanie poszczególnych znaków na ekranie. Niektóre opcje programu działają tylko na graficznym ekranie LCD.&lt;/p&gt;&lt;p style=&quot;-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;&lt;/p&gt;&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;&lt;span style=&quot; font-weight:600; font-style:italic;&quot;&gt;Rozmiar:&lt;/span&gt;&lt;/p&gt;&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;Te pole ocenia rozmiar urządzenia LCD. Piksele dla graficznych ekranów LCD lub tekstowe dla tekstowych LCD.&lt;/p&gt;&lt;p style=&quot;-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;&lt;/p&gt;&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;&lt;span style=&quot; font-weight:600; font-style:italic;&quot;&gt;Uaktywniony:&lt;/span&gt;&lt;/p&gt;&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt; font-weight:600; font-style:italic;&quot;&gt;&lt;span style=&quot; font-weight:400; font-style:normal;&quot;&gt;Ta opcja decyduje czy na danym ekranie LCD jest coś wyświetlane czy nie.&lt;/span&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="LCD.ui" line="34"/>
        <source>Devices</source>
        <translation>Urządzenia</translation>
    </message>
    <message>
        <location filename="LCD.ui" line="50"/>
        <source>Name</source>
        <translation>Nazwa</translation>
    </message>
    <message>
        <source>Type</source>
        <translation type="obsolete">Typ</translation>
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
        <translation>Rozmiar</translation>
    </message>
    <message>
        <location filename="LCD.ui" line="60"/>
        <source>Enabled</source>
        <translation>Uaktywniony</translation>
    </message>
    <message>
        <location filename="LCD.ui" line="77"/>
        <source>Views</source>
        <translation>Podgląd</translation>
    </message>
    <message>
        <location filename="LCD.ui" line="83"/>
        <source>Minimum Column Width</source>
        <translation>Minimalna szerokość kolumny</translation>
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
        <source>&lt;!DOCTYPE HTML PUBLIC &quot;-//W3C//DTD HTML 4.0//EN&quot; &quot;http://www.w3.org/TR/REC-html40/strict.dtd&quot;&gt;
&lt;html&gt;&lt;head&gt;&lt;meta name=&quot;qrichtext&quot; content=&quot;1&quot; /&gt;&lt;style type=&quot;text/css&quot;&gt;
p, li { white-space: pre-wrap; }
&lt;/style&gt;&lt;/head&gt;&lt;body style=&quot; font-family:&apos;MS Shell Dlg 2&apos;; font-size:8.25pt; font-weight:400; font-style:normal;&quot;&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;&lt;span style=&quot; font-size:8pt;&quot;&gt;This option decides the minimum width a column in the Player View.&lt;/span&gt;&lt;/p&gt;
&lt;p style=&quot;-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;If too many people are speaking at once, the Player View will split itself into columns.&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;You can use this option to pick a compromise between number of players shown on the&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;LCD, and width of player names.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="obsolete">&lt;!DOCTYPE HTML PUBLIC &quot;-//W3C//DTD HTML 4.0//EN&quot; &quot;http://www.w3.org/TR/REC-html40/strict.dtd&quot;&gt;&lt;html&gt;&lt;head&gt;&lt;meta name=&quot;qrichtext&quot; content=&quot;1&quot; /&gt;&lt;style type=&quot;text/css&quot;&gt;p, li { white-space: pre-wrap; }&lt;/style&gt;&lt;/head&gt;&lt;body style=&quot; font-family:&apos;MS Shell Dlg 2&apos;; font-size:8.25pt; font-weight:400; font-style:normal;&quot;&gt;&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;&lt;span style=&quot; font-size:8pt;&quot;&gt;Ta opcja ustawia minimalną szerokość kolumny w Widoku Graczy.&lt;/span&gt;&lt;/p&gt;&lt;p style=&quot;-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;&lt;/p&gt;&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;Jeśli rozmawia zbyt duża ilość ludzi jednocześnie to kolumna z Widokiem Graczy podzieli się na dwie różne kolumny.&lt;/p&gt;&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;Możesz użyc tej opcji aby wybrać kompromis pomiędzy liczbą wyświetlanych graczy na ekranie LCD oraz szerokością nazw graczy.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <source>TextLabel</source>
        <translation type="obsolete">Tekst</translation>
    </message>
    <message>
        <location filename="LCD.ui" line="121"/>
        <source>Splitter Width</source>
        <translation>Szerokość rozdzielnika</translation>
    </message>
    <message>
        <source>&lt;!DOCTYPE HTML PUBLIC &quot;-//W3C//DTD HTML 4.0//EN&quot; &quot;http://www.w3.org/TR/REC-html40/strict.dtd&quot;&gt;
&lt;html&gt;&lt;head&gt;&lt;meta name=&quot;qrichtext&quot; content=&quot;1&quot; /&gt;&lt;style type=&quot;text/css&quot;&gt;
p, li { white-space: pre-wrap; }
&lt;/style&gt;&lt;/head&gt;&lt;body style=&quot; font-family:&apos;MS Shell Dlg 2&apos;; font-size:8.25pt; font-weight:400; font-style:normal;&quot;&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;&lt;span style=&quot; font-size:8pt;&quot;&gt;This setting decides the width of column splitter.&lt;/span&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="obsolete">&lt;!DOCTYPE HTML PUBLIC &quot;-//W3C//DTD HTML 4.0//EN&quot; &quot;http://www.w3.org/TR/REC-html40/strict.dtd&quot;&gt;&lt;html&gt;&lt;head&gt;&lt;meta name=&quot;qrichtext&quot; content=&quot;1&quot; /&gt;&lt;style type=&quot;text/css&quot;&gt;p, li { white-space: pre-wrap; }&lt;/style&gt;&lt;/head&gt;&lt;body style=&quot; font-family:&apos;MS Shell Dlg 2&apos;; font-size:8.25pt; font-weight:400; font-style:normal;&quot;&gt;&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;&lt;span style=&quot; font-size:8pt;&quot;&gt;Ta opcja ustala szerkość rozdzielnika.&lt;/span&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
</context>
<context>
    <name>Log</name>
    <message>
        <location filename="Log.cpp" line="216"/>
        <source>Debug</source>
        <translation>Debug</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="217"/>
        <source>Critical</source>
        <translation>Błąd krytyczny</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="218"/>
        <source>Warning</source>
        <translation>Ostrzeżenie</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="219"/>
        <source>Information</source>
        <translation>Informacja</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="220"/>
        <source>Server Connected</source>
        <translation>Połączono z serwerem</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="221"/>
        <source>Server Disconnected</source>
        <translation>Rozłączono od serwera</translation>
    </message>
    <message>
        <source>Player Joined Server</source>
        <translation type="obsolete">Użytkownik dołączył do serwera</translation>
    </message>
    <message>
        <source>Player Left Server</source>
        <translation type="obsolete">Użytkownik opuścił serwer</translation>
    </message>
    <message>
        <source>Player kicked (you or by you)</source>
        <translation type="obsolete">Użytkownik wykopany (ty lub przez ciebie) </translation>
    </message>
    <message>
        <source>Player kicked</source>
        <translation type="obsolete">Użytkownik wykopany</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="226"/>
        <source>You self-muted/deafened</source>
        <translation>Wyciszyłeś mikrofon/słuchawki i mikrofon</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="227"/>
        <source>Other self-muted/deafened</source>
        <translation>Ktoś wyciszył swój mikrofon/słuchawki i mikrofon</translation>
    </message>
    <message>
        <source>Player muted (you)</source>
        <translation type="obsolete">Użytkownik wyciszył twój mikrofon</translation>
    </message>
    <message>
        <source>Player muted (by you)</source>
        <translation type="obsolete">Użytkownik ma wyciszony mikrofon (przez ciebie)</translation>
    </message>
    <message>
        <source>Player muted (other)</source>
        <translation type="obsolete">Użytkownik ma wyciszony mikrofon</translation>
    </message>
    <message>
        <source>Player Joined Channel</source>
        <translation type="obsolete">Użytkownik wszedł na kanał</translation>
    </message>
    <message>
        <source>Player Left Channel</source>
        <translation type="obsolete">Użytkownik opuścił kanał</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="233"/>
        <source>Permission Denied</source>
        <translation>Brak dostępu</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="234"/>
        <source>Text Message</source>
        <translation>Wiadomość tekstowa</translation>
    </message>
    <message>
        <source>Script Errors</source>
        <translation type="obsolete">Błędy skryptów</translation>
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
        <translation>Włącz konsolę dla zdarzeń: %1</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="76"/>
        <source>Toggle pop-up notifications for %1 events</source>
        <translation>Włącz powiadomienia dla zdarzeń: %1</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="77"/>
        <source>Toggle Text-To-Speech for %1 events</source>
        <translation>Włącz Tekst-na-mowę dla %1</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="78"/>
        <source>Click here to toggle sound notification for %1 events</source>
        <translation>Kliknij tutaj żeby włączyć powiadomienia dźwiękowe dla %1</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="79"/>
        <source>Path to sound file used for sound notifications in the case of %1 events&lt;br /&gt;Single click to play&lt;br /&gt;Double-click to change</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Log.cpp" line="81"/>
        <source>Click here to toggle console output for %1 events.&lt;br /&gt;If checked, this option makes Mumble output all %1 events in its message log.</source>
        <translation>Kliknij tutaj, aby włączyć wpisy w konsoli dla %1.&lt;br /&gt;Jeśli zaznaczone Mumble będzie pokazywać wszystkie %1 w logu.</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="82"/>
        <source>Click here to toggle pop-up notifications for %1 events.&lt;br /&gt;If checked, a notification pop-up will be created by Mumble for every %1 event.</source>
        <oldsource>Click here to toggle pop-up notifications for %1 events.&lt;br /&gt;If checked, a notification pop-up will be created by mumble for every %1 event.</oldsource>
        <translation>Kliknij tutaj, aby włączyć powiadomienia dla %1.&lt;br /&gt;Jeśli zaznaczone Mumble będzie pokazywać wszystkie %1 w obszarze powiadomień.</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="85"/>
        <source>Path to sound file used for sound notifications in the case of %1 events.&lt;br /&gt;Single click to play&lt;br /&gt;Double-click to change&lt;br /&gt;Ensure that sound notifications for these events are enabled or this field will not have any effect.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Click here to toggle Text-To-Speech for %1 events.&lt;br /&gt;If checked, Mumble uses Text-To-Speech to read %1 events out loud to you. Text-To-Speech is also able to read the contents of the event which is not true for soundfiles. Text-To-Speech and soundfiles cannot be used at the same time.</source>
        <oldsource>Click here to toggle Text-To-Speech for %1 events.&lt;br /&gt;If checked, mumble uses Text-To-Speech to read %1 events out loud to you. Text-To-Speech is also able to read the contents of the event which is not true for soundfiles. Text-To-Speech and soundfiles cannot be used at the same time.</oldsource>
        <translation type="obsolete">Kliknij tutaj, aby włączyć tekst na mowęi dla %1.&lt;br /&gt;Jeśli zaznaczone Mumble będzie używać tekstu na mowę do odczytania %1 na głos. Tekst na mowę i powiadomienia dźwiękowe nie mogą być uaktywnione razem.</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="83"/>
        <source>Click here to toggle Text-To-Speech for %1 events.&lt;br /&gt;If checked, Mumble uses Text-To-Speech to read %1 events out loud to you. Text-To-Speech is also able to read the contents of the event which is not true for sound files. Text-To-Speech and sound files cannot be used at the same time.</source>
        <oldsource>Click here to toggle sound notification for %1 events.&lt;br /&gt;If checked, Mumble uses a soundfile predefined by you to indicate %1 events. Soundfiles and Text-To-Speech cannot be used at the same time.</oldsource>
        <translation type="unfinished">Kliknij tutaj, aby włączyć powiadomienia dźwiękowe dla %1.&lt;br /&gt;Jeśli zaznaczone Mumble będzie odtwarzać dźwięki dla %1. Powiadomienia dźwiękowe nie mogą być uaktywnione razem z tekstem na mowę.</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="84"/>
        <source>Click here to toggle sound notification for %1 events.&lt;br /&gt;If checked, Mumble uses a sound file predefined by you to indicate %1 events. Sound files and Text-To-Speech cannot be used at the same time.</source>
        <oldsource>Path to soundfile used for sound notifications in the case of %1 events.&lt;br /&gt;Single click to play&lt;br /&gt;Doubleclick to change&lt;br /&gt;Be sure that sound notifications for these events are enabled or this field will not have any effect.</oldsource>
        <translation type="unfinished">Ścieżka do pliku użytego do powiadomień dźwiękowych w przypadku dla %1.&lt;br /&gt;Pojedyncze kliknięcie odtwarza dany dźwięk&lt;br /&gt;Podwójne kliknięcie zmienia plik&lt;br /&gt;Upewnij się, że powiadomienia dźwiękowe są aktywne inaczej to nie zadziała.</translation>
    </message>
    <message>
        <source>Choose sound file</source>
        <translation type="obsolete">Wybierz plik dźwiękowy</translation>
    </message>
    <message>
        <source>Invalid sound file</source>
        <translation type="obsolete">Nieprawidłowy plik dźwiękowy</translation>
    </message>
    <message>
        <location filename="Log.ui" line="14"/>
        <source>Messages</source>
        <translation>Wiadomości</translation>
    </message>
    <message>
        <location filename="Log.ui" line="41"/>
        <source>Console</source>
        <translation>Konsola</translation>
    </message>
    <message>
        <source>Enable console for %1</source>
        <translation type="obsolete">Uaktywnij konsolę dla %1</translation>
    </message>
    <message>
        <source>Enable Text-To-Speech for %1</source>
        <translation type="obsolete">Uaktywnij tekst na mowę dla %1</translation>
    </message>
    <message>
        <location filename="Log.ui" line="69"/>
        <source>Text To Speech</source>
        <translation>Tekst na mowę</translation>
    </message>
    <message>
        <location filename="Log.ui" line="75"/>
        <source>Volume</source>
        <translation>Głośność</translation>
    </message>
    <message>
        <location filename="Log.ui" line="85"/>
        <source>Volume of Text-To-Speech Engine</source>
        <translation>Głośność tekstu na mowę</translation>
    </message>
    <message>
        <location filename="Log.ui" line="88"/>
        <source>&lt;b&gt;This is the volume used for the speech synthesis.&lt;/b&gt;</source>
        <translation>&lt;b&gt;Głośność tekstu na mowę.&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="Log.ui" line="113"/>
        <source>Length threshold</source>
        <translation>Długość</translation>
    </message>
    <message>
        <location filename="Log.ui" line="123"/>
        <source>Message length threshold for Text-To-Speech Engine</source>
        <translation>Długość wiadomości dla tekstu na mowę</translation>
    </message>
    <message>
        <location filename="Log.ui" line="126"/>
        <source>&lt;b&gt;This is the length threshold used for the Text-To-Speech Engine.&lt;/b&gt;&lt;br /&gt;Messages longer than this limit will not be read aloud in their full length.</source>
        <translation>&lt;b&gt;Ustala długość wiadomości odczytywanej przez tekst na mowę.&lt;/b&gt;&lt;br /&gt;Wiadomości większe niż ustalony limit nie będą odczytywane.</translation>
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
    <message>
        <location filename="Log.ui" line="36"/>
        <source>Message</source>
        <translation>Wiadomość</translation>
    </message>
    <message>
        <location filename="Log.ui" line="46"/>
        <source>Notification</source>
        <translation>Powiadomienie</translation>
    </message>
    <message>
        <location filename="Log.ui" line="51"/>
        <source>Text-To-Speech</source>
        <translation>Tekst na mowę</translation>
    </message>
    <message>
        <location filename="Log.ui" line="56"/>
        <source>Soundfile</source>
        <translation>Plik dźwiękowy</translation>
    </message>
    <message>
        <location filename="Log.ui" line="61"/>
        <source>Path</source>
        <translation>Ścieżka do pliku dźwiękowego</translation>
    </message>
    <message>
        <location filename="Log.ui" line="132"/>
        <source> Characters</source>
        <translation> liter(y)</translation>
    </message>
</context>
<context>
    <name>LookConfig</name>
    <message>
        <location filename="LookConfig.ui" line="316"/>
        <location filename="LookConfig.ui" line="322"/>
        <source>Language</source>
        <translation>Język aplikacji</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="157"/>
        <source>Look and Feel</source>
        <translation>Wygląd</translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="47"/>
        <location filename="LookConfig.cpp" line="58"/>
        <source>System default</source>
        <translation>Domyślny (zgodny z systemem operacyjnym)</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="332"/>
        <source>Language to use (requires restart)</source>
        <translation>Wybierz język (potrzebny restart)</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="335"/>
        <source>&lt;b&gt;This sets which language Mumble should use.&lt;/b&gt;&lt;br /&gt;You have to restart Mumble to use the new language.</source>
        <translation>&lt;b&gt;Wybiera język interfejsu.</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="180"/>
        <source>Style</source>
        <translation>Styl okienek</translation>
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
        <source>This sets the behavior of channel drags; it can be used to prevent accidental dragging. &lt;i&gt;Move Channel&lt;/i&gt; moves the channel without prompting. &lt;i&gt;Do Nothing&lt;/i&gt; does nothing and prints an error message. &lt;i&gt;Ask&lt;/i&gt; uses a message box to confirm if you really wanted to move the channel.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="187"/>
        <source>Basic widget style</source>
        <translation>Styl okienek</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="190"/>
        <source>&lt;b&gt;This sets the basic look and feel to use.&lt;/b&gt;</source>
        <translation>&lt;b&gt;Ustawia wygląd.&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="197"/>
        <source>Skin</source>
        <translation>Skórka</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="204"/>
        <source>Skin file to use</source>
        <translation>Plik skórki</translation>
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
        <source>List players above subchannels (requires restart).</source>
        <translation type="obsolete">Listuj użytkowników nad kanałem (wymaga restartu).</translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="72"/>
        <source>User Interface</source>
        <translation>Interfejs</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="214"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="192"/>
        <source>Choose skin file</source>
        <translation>Zmień skórkę</translation>
    </message>
    <message>
        <source>&lt;b&gt;If set, players will be shown above subchannels in the channel view.&lt;/b&gt;&lt;br /&gt;A restart of Mumble is required to see the change.</source>
        <translation type="obsolete">&lt;b&gt;Użytkownicy będą wylistowani nad kanałem (wymaga restartu).</translation>
    </message>
    <message>
        <source>Players above Channels</source>
        <translation type="obsolete">Wyświetlaj nazwy użytkowników nad nazwą kanału</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="207"/>
        <source>&lt;b&gt;This sets which skin Mumble should use.&lt;/b&gt;&lt;br /&gt;The skin is a style file applied on top of the basic widget style. If there are icons in the same directory as the style sheet, those will replace the default icons.</source>
        <translation>&lt;b&gt;Ustawia jakiej skórki powinien używać Mumble.</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="14"/>
        <source>Form</source>
        <translation>Formularz</translation>
    </message>
    <message>
        <source>New version check</source>
        <translation type="obsolete">Aktualizacje programu</translation>
    </message>
    <message>
        <source>Check for updates on startup</source>
        <translation type="obsolete">Sprawdzaj, czy istnieje nowsza wersja Mumble przy starcie</translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="62"/>
        <source>None</source>
        <translation>nie rozwijaj kanałów</translation>
    </message>
    <message>
        <source>Only with players</source>
        <translation type="obsolete">rozwijaj tylko te kanały, na których są użytkownicy</translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="64"/>
        <source>All</source>
        <translation>rozwijaj wszystkie kanały</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="221"/>
        <source>Expand</source>
        <translation>Rozwijanie kanałów</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="228"/>
        <source>When to automatically expand channels</source>
        <translation>Ustawia kiedy automatycznie rozwijać kanały</translation>
    </message>
    <message>
        <source>This sets which channels to automatically expand. &lt;i&gt;None&lt;/i&gt; and &lt;i&gt;All&lt;/i&gt; will expand no or all channels, while &lt;i&gt;Only with players&lt;/i&gt; will expand and collapse channels as players join and leave them.</source>
        <translation type="obsolete">Ta opcja ustawia, które kanały powinny być automatycznie rozwijane.</translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="63"/>
        <source>Only with users</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="66"/>
        <source>Ask</source>
        <translation>pytaj przed podjęciem akcji</translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="67"/>
        <source>Do Nothing</source>
        <translation>wyłącz opcje przeciągania kanałów</translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="68"/>
        <source>Move</source>
        <translation>przenieś kanał bez pytania</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="163"/>
        <source>Channel Dragging</source>
        <translation>Przeciąganie kanałów</translation>
    </message>
    <message>
        <source>This changes the behaviour when moving channels.</source>
        <translation type="obsolete">Ta opcja zmienia zachowanie kiedy przenosisz kanał.</translation>
    </message>
    <message>
        <source>This sets the behaviour of channel drags, it can be used to prevent accidental dragging. &lt;i&gt;Move Channel&lt;/i&gt; moves the channel without prompting. &lt;i&gt;Do Nothing&lt;/i&gt; does nothing and prints an error message. &lt;i&gt;Ask&lt;/i&gt; uses a message box to confirm if you really wanted to move the channel.</source>
        <translation type="obsolete">Ogranicza możliwość przypadkowego przestawienia układu kanałów.</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="251"/>
        <source>Ask whether to close or minimize when quitting Mumble.</source>
        <translation>Okno potwierdzenia pokazuje się tylko wtedy, gdy jesteś połączony z jakimś serwerem.</translation>
    </message>
    <message>
        <source>&lt;b&gt;If set, will verify you want to quit if connected.&lt;b&gt;</source>
        <translation type="obsolete">&lt;b&gt;Jeśli ta opcja jest zaznaczona to zobaczysz komunikat czy rzeczywiście chcesz zamknąć Mumble (abyś przypadkiem nie zamknął programu), ujrzysz go tylko wtedy, gdy jesteś aktualnie połączony z jakimś serwerem.&lt;b&gt;</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="257"/>
        <source>Ask on quit while connected</source>
        <translation>Ostrzegaj przed zamknięciem programu</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="277"/>
        <source>Make the Mumble window appear on top of other windows.</source>
        <translation>Okno Mumble wyświetlane zawsze na wierzchu.</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="280"/>
        <source>&lt;b&gt;This makes the Mumble window a topmost window.&lt;/b&gt;</source>
        <translation>&lt;b&gt;Sprawia, że okno Mumble jest zawsze na pierwszym planie.&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="283"/>
        <source>Always On Top</source>
        <translation>Zawsze na wierzchu</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="267"/>
        <source>&lt;b&gt;If set, minimizing the Mumble main window will cause it to be hidden and accessible only from the tray. Otherwise, it will be minimized as a window normally would.&lt;/b&gt;</source>
        <translation>&lt;b&gt;Jeśli zaznaczone, minimalizacja głównego okna Mumble sprawi, że okno zostanie ukryte a dostęp do niego będzie dostępny tylko po przez ikonę w zasobniku systemowym. Jeśli opcja ta jest niezaznaczona, minimalizacja Mumble ukryje okno w normalny sposób (na pasku zadań).&lt;/b&gt;</translation>
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
        <location filename="LookConfig.ui" line="270"/>
        <source>Hide in tray when minimized</source>
        <translation>Ukryj w zasobniku systemowym podczas minimalizacji</translation>
    </message>
    <message>
        <source>Show all configuration items</source>
        <translation type="obsolete">Pokazuj wszystkie pozycje do skonfigurowania</translation>
    </message>
    <message>
        <source>&lt;b&gt;This will show all configuration items.&lt;/b&gt;&lt;br /&gt;Mumble contains a lot of configuration items that most users won&apos;t need to change. Checking this will show all configurable items.</source>
        <translation type="obsolete">&lt;b&gt;Pokazuj wszystkie opcje programu.&lt;/b&gt; Większość użytkowników Mumble i tak nie zmienia tych ustawień, uaktywnij tą opcję aby zobaczyć wszystkie funkcje programu.</translation>
    </message>
    <message>
        <source>Expert Config</source>
        <translation type="obsolete">Zaawansowana konfiguracja</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="264"/>
        <source>Hide the main Mumble window in the tray when it is minimized.</source>
        <translation>Ukryj Mumble w zasobniku systemowym, kiedy główne okno zostanie zminimalizowane.</translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <location filename="MainWindow.cpp" line="127"/>
        <location filename="MainWindow.cpp" line="1711"/>
        <source>Root</source>
        <translation>Źródło</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="180"/>
        <source>&amp;Connect</source>
        <translation>&amp;Połącz</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="183"/>
        <source>Open the server connection dialog</source>
        <translation>Otwórz okno połączenia serwera</translation>
    </message>
    <message>
        <source>Shows a dialog of registered servers, and also allows quick connect.</source>
        <translation type="obsolete">Pokazuje serwery zarejestrowane, pozwala na szybkie dołączenie.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="194"/>
        <source>&amp;Disconnect</source>
        <translation>&amp;Rozłącz</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="197"/>
        <source>Disconnect from server</source>
        <translation>Rozłącz od serwera</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="200"/>
        <source>Disconnects you from the server.</source>
        <translation>Rozłącza cię od serwera.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="208"/>
        <source>&amp;Ban lists</source>
        <translation>&amp;Lista banów</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="211"/>
        <source>Edit ban lists on server</source>
        <translation>Edytuj listę banów serwera</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="214"/>
        <source>This lets you edit the server-side IP ban lists.</source>
        <translation>Pozwala na edycję listy zbanowanych IP.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="233"/>
        <source>&amp;Kick</source>
        <translation>&amp;Kopnij</translation>
    </message>
    <message>
        <source>Kick player (with reason)</source>
        <translation type="obsolete">Wyrzuć użytkownika (podaj powód)</translation>
    </message>
    <message>
        <source>Kick selected player off server. You&apos;ll be asked to specify a reason.</source>
        <translation type="obsolete">Wyrzuca użytkownika z serwera.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="258"/>
        <source>&amp;Ban</source>
        <translation>&amp;Banuj</translation>
    </message>
    <message>
        <source>Kick and ban player (with reason)</source>
        <translation type="obsolete">Wyrzuć i zablokuj dostęp (podaj powód)</translation>
    </message>
    <message>
        <source>Kick and ban selected player from server. You&apos;ll be asked to specify a reason.</source>
        <translation type="obsolete">Blokuje dostęp do serwera temu użytkownikowi.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="247"/>
        <source>&amp;Mute</source>
        <translation>&amp;Wycisz</translation>
    </message>
    <message>
        <source>Mute player</source>
        <translation type="obsolete">Wycisz użytkownika</translation>
    </message>
    <message>
        <source>Mute or unmute player on server. Unmuting a deafened player will also undeafen them.</source>
        <translation type="obsolete">Wycisza użytkownika na serwerze.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="272"/>
        <source>&amp;Deafen</source>
        <translation>&amp;Wycisz słuchawki i mikrofon</translation>
    </message>
    <message>
        <source>Deafen player</source>
        <translation type="obsolete">Wycisz słuchawki i mikrofon użytkownika</translation>
    </message>
    <message>
        <source>Deafen or undeafen player on server. Deafening a player will also mute them.</source>
        <translation type="obsolete">Wycisz słuchawki i mikrofon użytkownika na serwerze.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="286"/>
        <source>&amp;Local Mute</source>
        <translation>&amp;Wycisz lokalnie</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="308"/>
        <source>&amp;Add</source>
        <translation>&amp;Dodaj</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="311"/>
        <source>Add new channel</source>
        <translation>Dodaj nowy kanał</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="314"/>
        <source>This adds a new sub-channel to the currently selected channel.</source>
        <translation>Dodaje nowy pod-kanał.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="319"/>
        <source>&amp;Remove</source>
        <translation>&amp;Usuń</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="322"/>
        <source>Remove channel</source>
        <translation>Usuń kanał</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="325"/>
        <source>This removes a channel and all sub-channels.</source>
        <translation>Usuwa kanał i wszystkie pod-kanały.</translation>
    </message>
    <message>
        <source>&amp;Edit ACL</source>
        <translation type="obsolete">&amp;Edytuj ACL</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="333"/>
        <source>Edit Groups and ACL for channel</source>
        <translation>Edytuje grupy i ACL kanału</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="336"/>
        <source>This opens the Group and ACL dialog for the channel, to control permissions.</source>
        <translation>Otwiera okno z zezwoleniami.</translation>
    </message>
    <message>
        <source>&amp;Rename</source>
        <translation type="obsolete">&amp;Zmień nazwę</translation>
    </message>
    <message>
        <source>&amp;Change Description</source>
        <translation type="obsolete">Z&amp;mień opis</translation>
    </message>
    <message>
        <source>Changes the channel description</source>
        <translation type="obsolete">Zmienia opis kanału</translation>
    </message>
    <message>
        <source>This changes the description of a channel.</source>
        <translation type="obsolete">Ta opcja zmienia opis kanału.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="341"/>
        <source>&amp;Link</source>
        <translation>&amp;Połącz</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="344"/>
        <source>Link your channel to another channel</source>
        <translation>Połącz kanał do innego kanału</translation>
    </message>
    <message>
        <source>This links your current channel to the selected channel. If they have permission to speak in the other channel, players can now hear each other. This is a permanent link, and will last until manually unlinked or the server is restarted. Please see the shortcuts for push-to-link.</source>
        <translation type="obsolete">Ta opcja łączy obecny kanał z zaznaczonym kanałem.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="355"/>
        <source>Unlink your channel from another channel</source>
        <translation>Rozłącz kanał od innego kanału</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="358"/>
        <source>This unlinks your current channel from the selected channel.</source>
        <translation>Rozłącza bieżący kanał od zaznaczonego kanału.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="363"/>
        <source>Unlink &amp;All</source>
        <translation>&amp;Rozłącz wszystkie</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="366"/>
        <source>Unlinks your channel from all linked channels.</source>
        <translation>Rozłącza twój kanał od reszty.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="369"/>
        <source>This unlinks your current channel (not the selected one) from all linked channels.</source>
        <translation>Ta opcja rozłącza twój obecny kanał od wszystkich innych kanałów.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="374"/>
        <source>&amp;Reset</source>
        <translation>&amp;Resetuj sprzęt audio</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="377"/>
        <source>Reset audio preprocessor</source>
        <translation>Resetuj sprzęt audio</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="380"/>
        <source>This will reset the audio preprocessor, including noise cancellation, automatic gain and voice activity detection. If something suddenly worsens the audio environment (like dropping the microphone) and it was temporary, use this to avoid having to wait for the preprocessor to readjust.</source>
        <translation>Ta opcja zresetuje twoje urządzenie audio. Użyj jej jeśli coś jest nie tak (hałas, zakłócenia).</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="388"/>
        <source>&amp;Mute Self</source>
        <translation>&amp;Wycisz mikrofon</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="391"/>
        <source>Mute yourself</source>
        <translation>Wycisz mikrofon</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="394"/>
        <source>Mute or unmute yourself. When muted, you will not send any data to the server. Unmuting while deafened will also undeafen.</source>
        <translation>Wycisza twój mikrofon. Nadal słyszysz innych.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="402"/>
        <source>&amp;Deafen Self</source>
        <translation>&amp;Wycisz słuchawki i mikrofon</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="405"/>
        <source>Deafen yourself</source>
        <translation>Wycisz słuchawki i mikrofon</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="408"/>
        <source>Deafen or undeafen yourself. When deafened, you will not hear anything. Deafening yourself will also mute.</source>
        <translation>Wyłącza twoje słuchawki i mikrofon. Nic nie będziesz słyszał ani nikt nie będzie słyszeć ciebie.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="416"/>
        <source>&amp;Text-To-Speech</source>
        <translation>&amp;Tekst na mowę</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="419"/>
        <source>Toggle Text-To-Speech</source>
        <translation>Włącz/wyłącz tekst na mowę</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="422"/>
        <source>Enable or disable the text-to-speech engine. Only messages enabled for TTS in the Configuration dialog will actually be spoken.</source>
        <translation>Włącza lub wyłącza tekst na mowę. Wszelkie wiadomości z dziennika zostaną przeczytane na głos przez Microsoft Sam&apos;a (lub inny domyślny dla danego komputera głos).</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="427"/>
        <source>S&amp;tatistics</source>
        <translation>S&amp;tatystyki</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="430"/>
        <source>Display audio statistics</source>
        <translation>Wyświetl statystyki audio</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="433"/>
        <source>Pops up a small dialog with information about your current audio input.</source>
        <translation>Pokazuje informacje o twoim wejściu audio.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="441"/>
        <source>Forcibly unlink plugin</source>
        <translation>Wymuś rozłączenie wtyczki</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="444"/>
        <source>This forces the current plugin to unlink, which is handy if it is reading completely wrong data.</source>
        <translation>Ta opcja wymusza rozłączenie bieżącej wtyczki, przydatne jeśli wysyła ona złe dane.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="449"/>
        <source>&amp;Settings</source>
        <translation>&amp;Ustawienia</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="452"/>
        <source>Configure Mumble</source>
        <translation>Konfiguracja Mumble</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="455"/>
        <source>Allows you to change most settings for Mumble.</source>
        <translation>Pozwala na zmianę większości ustawień Mumble.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="474"/>
        <source>&amp;What&apos;s This?</source>
        <translation>&amp;Co to jest?</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="477"/>
        <source>Enter What&apos;s This? mode</source>
        <translation>Wejdź w tryb &quot;Co to jest?&quot;</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="480"/>
        <source>Click this to enter &quot;What&apos;s This?&quot; mode. Your cursor will turn into a question mark. Click on any button, menu choice or area to show a description of what it is.</source>
        <translation>Uaktywnia tryb &quot;Co to jest?&quot;. Klikaj kursorem na poszczególne elementy interfejsu, aby dowiedzieć się do czego służą.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="485"/>
        <source>&amp;About</source>
        <translation>O &amp;programie</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="488"/>
        <source>Information about Mumble</source>
        <translation>Informacje na temat Mumble</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="491"/>
        <source>Shows a small dialog with information and license for Mumble.</source>
        <translation>Pokazuje małe okno z informacjami o licencji Mumble.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="499"/>
        <source>About &amp;Speex</source>
        <translation>O &amp;Speex</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="502"/>
        <source>Information about Speex</source>
        <translation>Informacje na temat Speex</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="505"/>
        <source>Shows a small dialog with information about Speex.</source>
        <translation>Pokazuje małe okno z informacjami na temat Speex.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="516"/>
        <source>Information about Qt</source>
        <translation>Informacje na temat Qt</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="519"/>
        <source>Shows a small dialog with information about Qt.</source>
        <translation>Pokazuje małe okno z informacjami na temat Qt.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="527"/>
        <source>Check for &amp;Updates</source>
        <translation>Sprawdź dostępność &amp;aktualizacji...</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="530"/>
        <source>Check for new version of Mumble</source>
        <translation>Sprawdź czy jest nowa wersja Mumble</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="533"/>
        <source>Connects to the Mumble webpage to check if a new version is available, and notifies you with an appropriate download URL if this is the case.</source>
        <translation>Łączy się ze stroną Mumble i sprawdza czy jest nowa wersja, jeśli tak to podaje link do niej.</translation>
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
        <location filename="MainWindow.cpp" line="209"/>
        <location filename="MainWindow.cpp" line="221"/>
        <source>Mumble -- %1</source>
        <translation>Mumble -- %1</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="120"/>
        <source>This shows all recent activity. Connecting to servers, errors and information messages all show up here.&lt;br /&gt;To configure exactly which messages show up here, use the &lt;b&gt;Settings&lt;/b&gt; command from the menu.</source>
        <translation>Dziennik pokazuje wszystkie ostatnie wydarzenia takie jak: próby połączenia z serwerem, błędy oraz wiadomości użytkowników.&lt;br /&gt;
Możesz dowolnie skonfigurować okno dziennika w Konfiguracja&gt;Ustawienia&gt;Wiadomości.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="53"/>
        <source>&amp;Server</source>
        <translation>&amp;Serwer</translation>
    </message>
    <message>
        <source>&amp;Player</source>
        <translation type="obsolete">&amp;Użytkownik</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="63"/>
        <source>&amp;Channel</source>
        <translation>&amp;Kanał</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="68"/>
        <source>&amp;Audio</source>
        <translation>&amp;Dźwięk</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="82"/>
        <source>C&amp;onfigure</source>
        <translation>K&amp;onfiguracja</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="88"/>
        <source>&amp;Help</source>
        <translation>&amp;Pomoc</translation>
    </message>
    <message>
        <source>Kicking player %1</source>
        <translation type="obsolete">Wykopywanie użytkownika %1</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="958"/>
        <location filename="MainWindow.cpp" line="980"/>
        <source>Enter reason</source>
        <translation>Podaj powód</translation>
    </message>
    <message>
        <source>Banning player %1</source>
        <translation type="obsolete">Banowanie użytkownika %1</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="311"/>
        <location filename="MainWindow.cpp" line="1200"/>
        <location filename="MainWindow.cpp" line="1834"/>
        <location filename="MainWindow.ui" line="14"/>
        <location filename="main.cpp" line="299"/>
        <source>Mumble</source>
        <translation>Mumble</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="167"/>
        <source>Push and hold this button to send voice.</source>
        <comment>Global Shortcut</comment>
        <translation>Naciśnij i przytrzymaj ten przycisk, aby transmitować głos.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="168"/>
        <source>This configures the push-to-talk button, and as long as you hold this button down, you will transmit voice.</source>
        <comment>Global Shortcut</comment>
        <translation></translation>
    </message>
    <message>
        <source>Toggle self-mute status.</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">Włącz tryb wyciszenia mikrofonu</translation>
    </message>
    <message>
        <source>This will toggle your muted status. If you toggle this off, you will also disable self-deafen.</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">Ta opcja pokaże twój status wyciszenia. Jeśli wyłączysz tą opcję zablokujesz wyciszenie słuchawek i mikrofonu.</translation>
    </message>
    <message>
        <source>Toggle self-deafen status.</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">Włącz tryb wycieszenia słuchawek</translation>
    </message>
    <message>
        <source>This will toggle your deafen status. If you toggle this on, you will also enable self-mute.</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">Ta opcja pokaże twój status wyciszenia słuchawek i mikrofonu. Jeśli włączysz tą opcję uaktywnisz również wyciszenie samego mikrofonu.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="195"/>
        <source>Toggle state of in-game overlay.</source>
        <comment>Global Shortcut</comment>
        <translation>Włącz tryb nakładki w grze.</translation>
    </message>
    <message>
        <source>This will switch the states of the ingame overlay between showing everybody, just the players who are talking, and nobody.</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">Ta opcja zmieni tryb nakładki w grze trzema dostępnymi trybami.</translation>
    </message>
    <message>
        <source>Channel Name</source>
        <translation type="obsolete">Podaj nazwę kanału:</translation>
    </message>
    <message>
        <source>Change description of channel %1</source>
        <translation type="obsolete">Zmień opis kanału %1</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1373"/>
        <source>Unmuted and undeafened.</source>
        <translation>Słuchawki i mikrofon ponownie działają.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1375"/>
        <source>Unmuted.</source>
        <translation>Mikrofon ponownie działa.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1377"/>
        <source>Muted.</source>
        <translation>Mikrofon został wyciszony.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1404"/>
        <source>Muted and deafened.</source>
        <translation>Słuchawki i mikrofon zostały wyciszone.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1406"/>
        <source>Deafened.</source>
        <translation>Słuchawki i mikrofon zostały wyciszone.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1408"/>
        <source>Undeafened.</source>
        <translation>Słuchawki ponownie działają.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1476"/>
        <source>About Qt</source>
        <translation>O Qt</translation>
    </message>
    <message>
        <source>Joining %1.</source>
        <translation type="obsolete">Dołączam %1.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1863"/>
        <source>Server connection failed: %1.</source>
        <translation>Połączenie z serwerem zostało przerwane: %1.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1865"/>
        <source>Disconnected from server.</source>
        <translation>Rozłączony od serwera.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="625"/>
        <source>Reconnecting.</source>
        <translation>Ponowne łączenie.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="201"/>
        <source>Joined server: %1.</source>
        <translation>%1 dołączył do serwera.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="248"/>
        <location filename="Messages.cpp" line="298"/>
        <location filename="Messages.cpp" line="338"/>
        <source>the server</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="305"/>
        <source>%1 moved to %2.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="307"/>
        <source>%1 moved to %2 by %3.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="317"/>
        <source>%1 moved in from %2 by %3.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="352"/>
        <source>Left server: %1.</source>
        <translation>%1 opuścił serwer.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="230"/>
        <source>%1 is now muted and deafened.</source>
        <translation>%1 ma teraz wyciszone słuchawki i mikrofon.</translation>
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
        <translation>%1 ma teraz wyciszony mikrofon.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="234"/>
        <source>%1 is now unmuted.</source>
        <translation>%1 nie ma już wyciszonego mikrofonu.</translation>
    </message>
    <message>
        <source>server</source>
        <translation type="obsolete">Serwer</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="254"/>
        <source>You were muted by %1.</source>
        <translation>%1 wyciszył twój mikrofon.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="256"/>
        <source>You were suppressed by %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="258"/>
        <source>You were unsuppressed by %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="260"/>
        <source>You were unmuted by %1.</source>
        <translation>%1 wyłączył wyciszenie mikrofonu.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="265"/>
        <source>You deafened %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="267"/>
        <source>You muted %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="269"/>
        <source>You unsuppressed %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="271"/>
        <source>You unmuted %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="277"/>
        <source>%1 muted by %2.</source>
        <translation>%2 wyciszył mikrofon %1.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="279"/>
        <source>%1 unsuppressed by %2.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="281"/>
        <source>%1 unmuted by %2.</source>
        <translation>%2 wyłączył wyciszenie mikrofonu %1.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="436"/>
        <source>the server</source>
        <comment>message from</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="575"/>
        <source>Unable to find matching CELT codecs with other clients. You will not be able to talk to all users.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="252"/>
        <source>You were deafened by %1.</source>
        <translation>Twoje słuchawki i mikrofon zostały wyciszone przez: %1.</translation>
    </message>
    <message>
        <source>You were undeafened by %1.</source>
        <translation type="obsolete">Twoje słuchawki i mikrofon nie są już wyciszone: %1.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="275"/>
        <source>%1 deafened by %2.</source>
        <translation>Słuchawki i mikrofon %1 zostały wyciszone przez %2.</translation>
    </message>
    <message>
        <source>%1 undeafened by %2.</source>
        <translation type="obsolete">%2 wyłączył wyciszenie słuchawek %1.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="345"/>
        <source>You were kicked from the server by %1: %2.</source>
        <translation>Zostałeś wykopany z serwera przez %1. Z powodu: %2.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="350"/>
        <source>%3 was kicked from the server by %1: %2.</source>
        <translation>%3 został wykopany z serwera przez %1. Z powodu: %2.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="343"/>
        <source>You were kicked and banned from the server by %1: %2.</source>
        <translation>Dostałeś bana od %1 z powodu %2.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="348"/>
        <source>%3 was kicked and banned from the server by %1: %2.</source>
        <translation>%3 dostał bana od %1 z powodu: %2.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="302"/>
        <source>You were moved to %1 by %2.</source>
        <translation>Zostałeś przeniesiony do kanału %1 przez %2.</translation>
    </message>
    <message>
        <source>%1 left channel.</source>
        <translation type="obsolete">%1 opuścił kanał.</translation>
    </message>
    <message>
        <source>%1 moved out by %2 to %3.</source>
        <translation type="obsolete">%1 przeniesiony przez %2 do kanału %3.</translation>
    </message>
    <message>
        <source>%1 moved in by %2 from %3.</source>
        <translation type="obsolete">%1 przeniesiony przez %2 do kanału %3.</translation>
    </message>
    <message>
        <source>%1 moved out by %2.</source>
        <translation type="obsolete">%2 przeniósł %1.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="315"/>
        <source>%1 entered channel.</source>
        <translation>%1 wszedł na kanał.</translation>
    </message>
    <message>
        <source>%1 moved in by %2.</source>
        <translation type="obsolete">%2 przeniósł %1.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="87"/>
        <source>Server connection rejected: %1.</source>
        <translation>Połączenie odrzucone: %1.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="182"/>
        <source>Denied: %1.</source>
        <translation>Zabroniono: %1.</translation>
    </message>
    <message>
        <location filename="main.cpp" line="282"/>
        <source>Welcome to Mumble.</source>
        <translation>Witamy w Mumble.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="165"/>
        <source>Push-to-Talk</source>
        <comment>Global Shortcut</comment>
        <translation>Aktywacja przyciskiem</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="171"/>
        <source>Reset Audio Processor</source>
        <comment>Global Shortcut</comment>
        <translation>Zresetuj sprzęt audio</translation>
    </message>
    <message>
        <source>Toggle Mute Self</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">Wycisz mikrofon</translation>
    </message>
    <message>
        <source>Toggle Deafen Self</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">Wycisz słuchawki i mikrofon</translation>
    </message>
    <message>
        <source>Force Center Position</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">Wymuś pozycję w środku</translation>
    </message>
    <message>
        <source>Chan Parent</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">Źródło kanału</translation>
    </message>
    <message>
        <source>Chan Sub#%1</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">Pod-Kanał # %1</translation>
    </message>
    <message>
        <source>Chan All Subs</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">Wszystkie pod-kanały</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="187"/>
        <source>Push-to-Mute</source>
        <comment>Global Shortcut</comment>
        <translation>Wyciszenie przyciskiem</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="190"/>
        <source>Join Channel</source>
        <comment>Global Shortcut</comment>
        <translation>Dołącz do kanału</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="193"/>
        <source>Toggle Overlay</source>
        <comment>Global Shortcut</comment>
        <translation>Włącz nakładkę</translation>
    </message>
    <message>
        <source>Alt Push-to-Talk</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">Alternatywny przycisk - aktywacji przyciskiem</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="184"/>
        <source>Unlink Plugin</source>
        <comment>Global Shortcut</comment>
        <translation>Wyłącz wtyczki</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="463"/>
        <location filename="MainWindow.cpp" line="615"/>
        <source>Connecting to server %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="700"/>
        <source>&lt;h2&gt;Version&lt;/h2&gt;&lt;p&gt;Protocol %1.%2.%3.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="705"/>
        <source>&lt;p&gt;No build information or OS version available.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="707"/>
        <source>&lt;p&gt;%1 (%2)&lt;br /&gt;%3&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="711"/>
        <source>&lt;h2&gt;Control channel&lt;/h2&gt;&lt;p&gt;Encrypted with %1 bit %2&lt;br /&gt;%3 ms average latency (%4 deviation)&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="720"/>
        <source>&lt;h2&gt;Voice channel&lt;/h2&gt;&lt;p&gt;Encrypted with 128 bit OCB-AES128&lt;br /&gt;%1 ms average latency (%4 deviation)&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="731"/>
        <source>&lt;h2&gt;Audio bandwidth&lt;/h2&gt;&lt;p&gt;Maximum %1 kbit/s&lt;br /&gt;Current %2 kbit/s&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="913"/>
        <source>Register yourself as %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="913"/>
        <source>&lt;p&gt;You are about to register yourself on this server. This action cannot be undone, and your username cannot be changed once this is done. You will forever be known as &apos;%1&apos; on this server.&lt;/p&gt;&lt;p&gt;Are you sure you want to register yourself?&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="915"/>
        <source>Register user %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="915"/>
        <source>&lt;p&gt;You are about to register %1 on the server. This action cannot be undone, the username cannot be changed, and as a registered user, %1 will have access to the server even if you change the server password.&lt;/p&gt;&lt;p&gt;From this point on, %1 will be authenticated with the certificate currently in use.&lt;/p&gt;&lt;p&gt;Are you sure you want to register %1?&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="958"/>
        <source>Kicking user %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="980"/>
        <source>Banning user %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1029"/>
        <source>Change comment on user %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1072"/>
        <location filename="MainWindow.cpp" line="1289"/>
        <source>To channel %1: %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1072"/>
        <location filename="MainWindow.cpp" line="1289"/>
        <source>Message to channel %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1200"/>
        <source>Are you sure you want to delete %1 and all its sub-channels?</source>
        <translation>Czy jesteś pewien, że chcesz usunąć %1 oraz wszystkie pod-kanały?</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1783"/>
        <source>SSL Version mismatch</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1783"/>
        <source>This server is using an older encryption standard. It might be an older 1.1 based Mumble server.&lt;br /&gt;Would you like to launch the compatibility client to connect to it?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1815"/>
        <source>Failed to launch compatibilty client</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1815"/>
        <source>The compatibility client could not be found, or failed to start.&lt;br /&gt;Note that the compatibility client is an optional component for most installations, and might not be installed.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1973"/>
        <source>Type message to channel &apos;%1&apos; here</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1976"/>
        <source>Type message to user &apos;%1&apos; here</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="2051"/>
        <source>Choose image file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="2051"/>
        <source>Images (*.png *.jpg)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="2058"/>
        <location filename="MainWindow.cpp" line="2070"/>
        <source>Failed to load image</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="2058"/>
        <source>Could not open file for reading.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="2070"/>
        <source>Image format not recognized.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="166"/>
        <source>&amp;Quit Mumble</source>
        <translation>&amp;Zakończ</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="169"/>
        <source>Closes the program</source>
        <translation>Zamyka program</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="172"/>
        <source>Exits the application.</source>
        <translation>Wyłącza aplikację.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="297"/>
        <location filename="MainWindow.ui" line="538"/>
        <source>Send Messa&amp;ge</source>
        <translation>Wyślij wiado&amp;mość</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="300"/>
        <location filename="MainWindow.ui" line="541"/>
        <source>Send a Text Message</source>
        <translation>Wyślij wiadomość tekstową</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1002"/>
        <source>Sending message to %1</source>
        <translation>Wysyłanie wiadomości do %1</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1015"/>
        <location filename="MainWindow.cpp" line="1076"/>
        <source>To %1: %2</source>
        <translation>Do %1: %2</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1015"/>
        <location filename="MainWindow.cpp" line="1076"/>
        <source>Message to %1</source>
        <translation>Wiadomość do %1</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1705"/>
        <source>Connected.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1877"/>
        <source>Invalid username</source>
        <translation>Niepoprawna nazwa użytkownika</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1877"/>
        <source>You connected with an invalid username, please try another one.</source>
        <translation>Połączyłeś się do serwera z niewłaściwą nazwą użytkownika, spróbuj innej nazwy.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1877"/>
        <source>That username is already in use, please try another username.</source>
        <translation>Ta nazwa użytkownika jest już zajęta, spróbuj innej.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1882"/>
        <source>Wrong password</source>
        <translation>Błędne hasło</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1882"/>
        <source>Wrong password for registered users, please try again.</source>
        <translation>Podałeś złe hasło dla zarejestrowanych użytkowników, spróbuj jeszcze raz.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1882"/>
        <source>Wrong server password for unregistered user account, please try again.</source>
        <translation>Podałeś złe hasło dla niezarejestrowanych użytkowników, spróbuj jeszcze raz.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="437"/>
        <source>From %1: %2</source>
        <translation>Od %1: %2</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="438"/>
        <source>Message from %1</source>
        <translation>Wiadomość od %1</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="463"/>
        <source>&amp;Audio Wizard</source>
        <translation>&amp;Kreator ustawień dźwięku</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="303"/>
        <source>Sends a text message to another user.</source>
        <translation>Wysyła wiadomość tekstową do innego użytkownika.</translation>
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
        <location filename="MainWindow.ui" line="466"/>
        <source>Start the audio configuration wizard</source>
        <translation>Rozpocznij konfiguracjęprogramu Mumble</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="469"/>
        <source>This will guide you through the process of configuring your audio hardware.</source>
        <translation>Ten kreator poprowadzi cię przez proces konfiguracji programu Mumble.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1821"/>
        <source>SSL Verification failed: %1</source>
        <translation>Weryfikacja SSL nie powiodła się: %1</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1826"/>
        <source>&lt;b&gt;WARNING:&lt;/b&gt; The server presented a certificate that was different from the stored one.</source>
        <translation>&lt;b&gt;OSTRZEŻENIE:&lt;/b&gt; Ten serwer posiada obecnie inny certyfikat niż poprzedni, który masz zachowany u siebie.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1828"/>
        <source>Sever presented a certificate which failed verification.</source>
        <translation>Serwer posiada certyfikat, który nie przeszedł weryfikacji.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1835"/>
        <source>&lt;p&gt;%1.&lt;br /&gt;The specific errors with this certificate are: &lt;/p&gt;&lt;ol&gt;%2&lt;/ol&gt;&lt;p&gt;Do you wish to accept this certificate anyway?&lt;br /&gt;(It will also be stored so you won&apos;t be asked this again.)&lt;/p&gt;</source>
        <translation>&lt;p&gt;%1.&lt;br /&gt;Błędy certyfikatu: &lt;/p&gt;&lt;ol&gt;%2&lt;/ol&gt;&lt;p&gt;Czy mimo to chcesz go zaakceptować?&lt;br /&gt;(Mumble więcej cię o to nie zapyta.)&lt;/p&gt;</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="222"/>
        <source>&amp;Information</source>
        <translation>&amp;Informacje</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="225"/>
        <source>Show information about the server connection</source>
        <translation>Pokaż informacje o serwerze</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="228"/>
        <source>This will show extended information about the connection to the server.</source>
        <translation>Ta opcja pokaże zaawansowane informacje na temat połączenia z serwerem.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="733"/>
        <source>Mumble Server Information</source>
        <translation>Serwer Mumble - Informacje</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="737"/>
        <location filename="MainWindow.cpp" line="1842"/>
        <source>&amp;View Certificate</source>
        <translation>&amp;Zobacz certyfikat</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="379"/>
        <source>Opening URL %1</source>
        <translation>Otwieranie adresu %1</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="174"/>
        <source>Mute Self</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="176"/>
        <source>Set self-mute status.</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="177"/>
        <source>This will set or toggle your muted status. If you turn this off, you will also disable self-deafen.</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="179"/>
        <source>Deafen Self</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="181"/>
        <source>Set self-deafen status.</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="182"/>
        <source>This will set or toggle your deafened status. If you turn this on, you will also enable self-mute.</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="196"/>
        <source>This will switch the states of the in-game overlay between showing everybody, just the users who are talking, and nobody.</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="212"/>
        <source>Whisper</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="264"/>
        <location filename="MainWindow.cpp" line="1967"/>
        <source>Not connected</source>
        <translation type="unfinished">Niepołączony</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="383"/>
        <source>File does not exist</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="393"/>
        <source>File is not a configuration file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="399"/>
        <source>Settings merged from file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="405"/>
        <source>URL scheme is not &apos;mumble&apos;</source>
        <translation>Adres URL nie jest obsługiwany przez Mumble (&apos;mumble&apos;)</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="424"/>
        <source>This version of Mumble can&apos;t handle URLs for Mumble version %1.%2.%3</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="450"/>
        <source>Connecting to %1</source>
        <translation>Łączenie z %1</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="450"/>
        <source>Enter username</source>
        <translation>Nazwa użytkownika</translation>
    </message>
    <message>
        <source>Renames the channel</source>
        <translation type="obsolete">Zmienia nazwę kanału</translation>
    </message>
    <message>
        <source>This renames a channel.</source>
        <translation type="obsolete">Zmienia nazwę kanału.</translation>
    </message>
    <message>
        <location filename="main.cpp" line="299"/>
        <source>This is the first time you&apos;re starting Mumble.&lt;br /&gt;Would you like to go through the Audio Wizard to configure your soundcard?</source>
        <translation>Program Mumble został uruchomiony po raz pierwszy na tym komputerze.&lt;br /&gt; Czy chcesz uruchomić kreator ustawień dźwięku, aby skonfigurować podstawowe opcje aplikacji?</translation>
    </message>
    <message>
        <source>&lt;h2&gt;Control channel&lt;/h2&gt;&lt;p&gt;Encrypted with %1 bit %2&lt;br /&gt;%3 ms average latency (%4 variance)&lt;/p&gt;</source>
        <translation type="obsolete">&lt;h2&gt;Kanał kontrolny&lt;/h2&gt;&lt;p&gt;Szyfrowanie %1 bit %2&lt;br /&gt;%1 ms przeciętnego opóźnienia (%4 niezgodności)&lt;/p&gt;</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="718"/>
        <source>Voice channel is sent over control channel.</source>
        <translation>Kanał głosowy jest wysyłany przez kanał kontrolny.</translation>
    </message>
    <message>
        <source>&lt;h2&gt;Voice channel&lt;/h2&gt;&lt;p&gt;Encrypted with 128 bit OCB-AES128&lt;br /&gt;%1 ms average latency (%4 variance)&lt;/p&gt;</source>
        <translation type="obsolete">&lt;h2&gt;Kanał głosowy&lt;/h2&gt;&lt;p&gt;Szyfrowanie 128-bitowym kluczem OCB-AES128&lt;br /&gt;%1 ms przeciętnego opóźnienia (%4 niezgodności)&lt;/p&gt;</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="727"/>
        <source>UDP Statistics</source>
        <translation>Statystyki UDP</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="727"/>
        <source>To Server</source>
        <translation>Do serwera</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="727"/>
        <source>From Server</source>
        <translation>Od serwera</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="727"/>
        <source>Good</source>
        <translation>Dobre</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="727"/>
        <source>Late</source>
        <translation>Opóźnione</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="727"/>
        <source>Lost</source>
        <translation>Utracone</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="727"/>
        <source>Resync</source>
        <translation>Re-synchronizowane</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="175"/>
        <source>Ctrl+Q</source>
        <translation>Ctrl+Q</translation>
    </message>
    <message>
        <source>Mute player locally</source>
        <translation type="obsolete">Wycisz użytkownika lokalnie</translation>
    </message>
    <message>
        <source>Mute or unmute player locally. Use this on other players in the same room.</source>
        <translation type="obsolete">Wyciszanie mikrofonu lub przywracanie głosu danego użytkownika lokalnie. Używaj tego na innych użytkownikach na tym samym kanale.</translation>
    </message>
    <message>
        <source>&amp;Rename Channel</source>
        <translation type="obsolete">&amp;Zmień nazwę</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="513"/>
        <source>About &amp;Qt</source>
        <translation>O &amp;Qt</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1272"/>
        <source>Sending message to channel %1</source>
        <translation>Wysyłanie wiadomości na kanał: %1</translation>
    </message>
    <message>
        <source>Sending message to channel tree %1</source>
        <translation type="obsolete">Wyślij wiadomość do wszystkich na: %1</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1287"/>
        <source>To tree %1: %2</source>
        <translation>Do kanału %1: %2</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1287"/>
        <source>Message to tree %1</source>
        <translation>Wyślij do kanału %1</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="544"/>
        <source>Sends a text message to all users in a channel.</source>
        <translation>Wysyła wiadomość do wszystkich na tym kanale.</translation>
    </message>
    <message>
        <source>Send &amp;Tree Message</source>
        <translation type="obsolete">Wyślij do &amp;drzewa kanałów</translation>
    </message>
    <message>
        <source>Send message to channel tree.</source>
        <translation type="obsolete">Wyślij wiadomość do wszystkich na tym kanale.</translation>
    </message>
    <message>
        <source>This sends a text message to a channel and it&apos;s subchannels.</source>
        <translation type="obsolete">Wysyła wiadomość do wszystkich na tym kanale oraz do wszystkich pod-kanałów.</translation>
    </message>
    <message>
        <source>Server maximum bandwidth is only %1 kbit/s. Quality auto-adjusted.</source>
        <translation type="obsolete">Maksymalny transfer serwera to %1 kbit/s. Jakość ustawiona automatycznie.</translation>
    </message>
    <message>
        <source>&lt;h2&gt;Audio bandwidth&lt;/h2&gt;&lt;p&gt;Maximum %1 kbit/s&lt;br /&gt;Current %2 kbit/s (Quality %3)&lt;/p&gt;</source>
        <translation type="obsolete">&lt;h2&gt;Szerokość pasma dźwięku&lt;/h2&gt;&lt;p&gt;Maksymalnie %1 kbit/s&lt;br /&gt;Obecnie %2 kbit/s (Jakość %3)&lt;/p&gt;</translation>
    </message>
    <message>
        <source>Connected to server %1.</source>
        <translation type="obsolete">Połączony z serwerem %1.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="311"/>
        <source>Mumble is currently connected to a server. Do you want to Close or Minimize it?</source>
        <translation>&lt;b&gt;UWAGA!&lt;/b&gt; Mumble jest obecnie połączony z serwerem.

Chcesz zakończyć czy zminimalizować program?</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="312"/>
        <source>Close</source>
        <translation>Zakończ</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="227"/>
        <location filename="MainWindow.cpp" line="313"/>
        <source>Minimize</source>
        <translation>Minimalizuj</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="225"/>
        <source>&amp;Window</source>
        <translation>&amp;Okno</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="227"/>
        <source>Ctrl+M</source>
        <translation>Ctrl+M</translation>
    </message>
    <message>
        <source>Ctrl+W</source>
        <translation type="obsolete">Ctrl+W</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="199"/>
        <source>Toggle Minimal</source>
        <comment>Global Shortcut</comment>
        <translation>Tryb minimalny</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="552"/>
        <source>&amp;Minimal View</source>
        <translation>&amp;Tryb minimalny</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="555"/>
        <source>Toggle minimal window modes</source>
        <translation>Uruchom minimalny tryb</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="558"/>
        <source>This will toggle minimal mode, where the log window and menu is hidden.</source>
        <translation>Ta opcja uruchomi tryb minimalny, gdzie okno z dziennikiem i menu są ukryte.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="110"/>
        <source>Log</source>
        <translation>Dziennik</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="202"/>
        <source>Volume Up (+10%)</source>
        <comment>Global Shortcut</comment>
        <translation>Głośność w górę (+10%)</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="205"/>
        <source>Volume Down (-10%)</source>
        <comment>Global Shortcut</comment>
        <translation>Głośność w dół (-10%)</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="373"/>
        <source>Clear</source>
        <translation>Wyczyść</translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="1248"/>
        <source>You have Channel Dragging set to &quot;Do Nothing&quot; so the channel wasn&apos;t moved.</source>
        <translation>Przeciąganie kanału masz ustatwione na &quot;wyłącz opcje przeciągania kanałów&quot; dlatego nie zostałeś przeniesiony.</translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="1254"/>
        <source>Unknown Channel Drag mode in UserModel::dropMimeData.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Unknown Channel Drag mode in PlayerModel::dropMimeData.</source>
        <translation type="obsolete">Nieznany tryb Przeciągania Kanału w PlayerModel::dropMimeData.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="352"/>
        <source>&amp;Unlink</source>
        <comment>Channel</comment>
        <translation>&amp;Rozłącz</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="438"/>
        <source>&amp;Unlink</source>
        <comment>Plugin</comment>
        <translation>&amp;Wyłącz wtyczki</translation>
    </message>
    <message>
        <source>Server</source>
        <comment>message from</comment>
        <translation type="obsolete">Serwer</translation>
    </message>
    <message>
        <source>&amp;Quit</source>
        <translation type="obsolete">&amp;Zakończ</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="566"/>
        <source>Hide Frame</source>
        <translation>Ukryj ramkę</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="569"/>
        <source>Toggle showing frame on minimal window</source>
        <translation>Uaktywnia obramowanie okna w trybie minimalnym</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="572"/>
        <source>This will toggle whether the minimal window should have a frame for moving and resizing.</source>
        <translation>Uaktywnia obramowanie okna w trybie minimalnym dzięki czemu można przenosić i zmieniać rozmiar okna.</translation>
    </message>
</context>
<context>
    <name>NetworkConfig</name>
    <message>
        <location filename="NetworkConfig.cpp" line="52"/>
        <source>Network</source>
        <translation>Sieć</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="14"/>
        <source>Form</source>
        <translation>Formularz</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="26"/>
        <source>Connection</source>
        <translation>Połączenie</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="32"/>
        <source>Use TCP compatibility mode</source>
        <translation>Używaj trybu zgodności TCP</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="35"/>
        <source>&lt;b&gt;Enable TCP compatibility mode&lt;/b&gt;.&lt;br /&gt;This will make Mumble use only TCP when communicating with the server. This will increase overhead and cause lost packets to produce noticeable pauses in communication, so this should only be used if you are unable to use the default (which uses UDP for voice and TCP for control).</source>
        <translation>&lt;b&gt;Uaktywnij tryb kompatybilności TCP.&lt;/b&gt; Dzięki tej opcji Mumble działa w przypadkach kiedy nie można się połączyć w trybie UDP. Negatywnym skutkiem tej opcji jest utracenie pewnej liczby pakietów.</translation>
    </message>
    <message>
        <source>Use TCP mode</source>
        <translation type="obsolete">Używaj trybu TCP</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="58"/>
        <source>Reconnect when disconnected</source>
        <translation>Automatycznie wznawiaj przerwane połączenie</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="61"/>
        <source>&lt;b&gt;Reconnect when disconnected&lt;/b&gt;.&lt;br /&gt;This will make Mumble try to automatically reconnect after 10 seconds if your server connection fails.</source>
        <translation>&lt;b&gt;Automatycznie wznawiaj przerwane połączenie&lt;/b&gt; Mumble będzie ponawiać połączenie co 10 sekund.</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="64"/>
        <source>Reconnect automatically</source>
        <translation>Automatycznie wznawiaj przerwane połączenie</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="93"/>
        <source>Proxy</source>
        <translation>Proxy</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="99"/>
        <source>Type</source>
        <translation>Typ</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="112"/>
        <source>Type of proxy to connect through</source>
        <translation>Typ proxy</translation>
    </message>
    <message>
        <source>&lt;b&gt;Type of proxy to connect through&lt;/b&gt;&lt;br /&gt;This makes Mumble connect through a proxy for all outgoing connections. Note: Proxy tunneling forces Mumble into TCP compatibility mode, causing all voice data to be sent via the control channel.</source>
        <translation type="obsolete">&lt;b&gt;Rodzaj proxy.&lt;/b&gt; Połączenia przez proxy wymuszają tryb zgodności TCP.</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="119"/>
        <source>Direct connection</source>
        <translation>połączenie bezpośrednie</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="124"/>
        <source>HTTP(S) proxy</source>
        <translation>HTTP(S) Proxy</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="129"/>
        <source>SOCKS5 proxy</source>
        <translation>SOCKS5 Proxy</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="137"/>
        <source>Hostname</source>
        <translation>Host</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="153"/>
        <source>Hostname of the proxy</source>
        <translation>Nazwa hosta proxy</translation>
    </message>
    <message>
        <source>&lt;b&gt;Hostname of the proxy.&lt;b&gt;&lt;br /&gt;This field specifies the hostname of the proxy you wish to tunnel network traffic through.</source>
        <translation type="obsolete">Nazwa serwera proxy.</translation>
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
        <translation>Port</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="203"/>
        <source>Port number of the proxy</source>
        <translation>Numer portu</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="206"/>
        <source>&lt;b&gt;Port number of the proxy.&lt;/b&gt;&lt;br /&gt;This field specifies the port number that the proxy expects connections on.</source>
        <translation>Port na zdalnym serwerze.</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="222"/>
        <source>Username</source>
        <translation>Użytkownik</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="229"/>
        <source>Username for proxy authentication</source>
        <translation>Nazwa użytkownika dla proxy</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="232"/>
        <source>&lt;b&gt;Username for proxy authentication.&lt;/b&gt;&lt;br /&gt;This specifies the username you use for authenticating yourself with the proxy. In case the proxy does not use authentication, or you want to connect anonymously, simply leave this field blank.</source>
        <translation>Nazwa użytkownika przy autoryzacji do proxy.</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="239"/>
        <source>Password</source>
        <translation>Hasło</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="246"/>
        <source>Password for proxy authentication</source>
        <translation>Hasło dla proxy</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="249"/>
        <source>&lt;b&gt;Password for proxy authentication.&lt;/b&gt;&lt;br /&gt;This specifies the password you use for authenticating yourself with the proxy. In case the proxy does not use authentication, or you want to connect anonymously, simply leave this field blank.</source>
        <translation>Hasło do serwera proxy. Jeżeli dostęp ma być anonimowy pozostaw te pole puste.</translation>
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
        <translation>Usługi Mumble</translation>
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
        <translation type="obsolete">Sprawdzaj podczas uruchamiania czy jest nowsza wersja Mumble</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="323"/>
        <source>Submit anonymous statistics to the Mumble project</source>
        <translation>Zezwalaj na wysyłanie anonimowych statystyk</translation>
    </message>
    <message>
        <source>&lt;b&gt;Submit anonymous statistics.&lt;/b&gt;&lt;br /&gt;Mumble has a small development team, and as such needs to focus it&apos;s development where it&apos;s needed most. By submitting a bit of statistics you help the project determine where to focus development.</source>
        <translation type="obsolete">&lt;b&gt;Zezwalaj na wysyłanie anonimowych statystyk.&lt;/b&gt;&lt;br /&gt;Mumble ma małą drużynę deweloperów, która musi się skupić na opcjach, które są jak najbardziej potrzebne. Zezwalając na wysyłanie statystyk pomagasz ustalić nad czym skupić dalszy rozwój aplikacji.</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="329"/>
        <source>Submit anonymous statistics</source>
        <translation>Zezwalaj na wysyłanie anonimowych statystyk</translation>
    </message>
</context>
<context>
    <name>Overlay</name>
    <message>
        <location filename="Overlay.cpp" line="192"/>
        <location filename="Overlay.cpp" line="204"/>
        <source>Mumble</source>
        <translation>Mumble</translation>
    </message>
    <message>
        <source>Failed to load overlay library. This means either that:
- the library (mumble_ol.dll) wasn&apos;t found in the directory you ran Mumble from
- you&apos;re on an OS earlier than WinXP SP2
- you do not have the June 2007 updated version of DX9.0c</source>
        <translation type="obsolete">Nie udało się załadowac biblioteki nakładki. Oznacza to, że:
- brakuje biblioteki w folderze Mumble (mumble_ol.dll)
- jesteś na starszym systemie niż Windows XP SP2
- nie masz zainstalowanego DirectX 9.0c z Czerwca 2007</translation>
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
        <location filename="Overlay.ui" line="20"/>
        <source>Options</source>
        <translation>Opcje</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="78"/>
        <source>Position</source>
        <translation>Pozycja nakładki na ekranie</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="171"/>
        <source>Font</source>
        <translation>Czcionka</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="32"/>
        <source>Enable Overlay</source>
        <translation>Włącz nakładkę</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="26"/>
        <source>Enable overlay.</source>
        <translation>Włącza nakładkę.</translation>
    </message>
    <message>
        <source>This sets whether the overlay is enabled or not. This setting is only checked with D3D9 applications are started, so make sure Mumble is running and this option is on before you start the application.&lt;br /&gt;Please note that if you start the application after starting Mumble, or if you disable the overlay while running, there is no safe way to restart the overlay without also restarting the application.</source>
        <translation type="obsolete">Uaktywnia nakładkę. Wyświetla listę użytkowników na ekranie podczas gry.</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="39"/>
        <source>Who to show on the overlay</source>
        <translation>Kogo pokazywać w nakładce</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="42"/>
        <source>&lt;b&gt;This sets who to show in the in-game overlay.&lt;/b&gt;&lt;br /&gt;If many people are connected to the same channel, the overlay list might be very long. Use this to shorten it.&lt;br /&gt;&lt;i&gt;No one&lt;/i&gt; - Don&apos;t show anyone (but leave overlay running).&lt;br /&gt;&lt;i&gt;Only talking&lt;/i&gt; - Only show talking people.&lt;br /&gt;&lt;i&gt;Everyone&lt;/i&gt; - Show everyone.</source>
        <translation>Wybierz kogo nakładka pokazuje podczas gry.</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="55"/>
        <source>Always Show Self</source>
        <translation>Pokazuj swój nick na nakładce</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="49"/>
        <source>Always show yourself on overlay.</source>
        <translation>Ta opcja sprawia, że Mumble zawsze pokazuje twoją nazwę użytkownika na nakładce.</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="52"/>
        <source>This sets whether to always show yourself or not. This setting is useful if you aren&apos;t showing everyone in the overlay, as then you would only see your own status if you were talking, which wouldn&apos;t let you see that you were deafened or muted.</source>
        <translation>Ustawia czy zobaczysz swoją nazwę użytkownika na nakładce czy też nie.</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="68"/>
        <source>Show User Textures</source>
        <translation>Pokazuj tekstury użytkowników zamiast ich nicków</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="62"/>
        <source>Show User custom textures instead of text on the overlay.</source>
        <translation>Pokazuje tekstury wybrane przez użytkowników zamiast ich nazw użytkownika.
Opcja dostępna tylko, gdy ustawisz teksturę na danym serwerze podczas rejestracji.</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="65"/>
        <source>This sets whether to download and use custom textures for registered users. If disabled, the regular outline text will be used instead.</source>
        <translation>Używa niestandardowe tekstury użytkowników zamiast ich nazw użytkownika.</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="119"/>
        <source>Grow Left</source>
        <translation>Do lewej</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="113"/>
        <source>Let overlay grow to the left</source>
        <translation>Do lewej</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="116"/>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow to the left if needed.</source>
        <translation>Nakładka stara się pozostać najmniejszą jak tylko może w pozycji, którą ustawiłeś. Ta opcja pozwala nakładce rosnąć w lewo jeśli trzeba.</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="132"/>
        <source>Grow Right</source>
        <translation>Do prawej</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="90"/>
        <source>Grow Up</source>
        <translation>Do góry</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="84"/>
        <source>Let overlay grow upwards</source>
        <translation>Pozwala nakładce rosnąć w górę</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="145"/>
        <source>Grow Down</source>
        <translation>Do dołu</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="139"/>
        <source>Let overlay grow downwards</source>
        <translation>Pozwala nakładce rosnąć do dołu</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="152"/>
        <source>X-Position of Overlay</source>
        <translation>Pozycja X Nakładki</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="155"/>
        <source>This sets the relative X position of the overlay.</source>
        <translation>Ustawia pozycję X nakładki.</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="97"/>
        <source>Y-Position of Overlay</source>
        <translation>Pozycja Y Nakładki</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="29"/>
        <source>This sets whether the overlay is enabled or not. This setting is only checked when applications are started, so make sure Mumble is running and this option is on before you start the application.&lt;br /&gt;Please note that if you start Mumble after starting the application, or if you disable the overlay while the application is running, there is no safe way to restart the overlay without also restarting the application.</source>
        <oldsource>This sets whether the overlay is enabled or not. This setting is only checked when applications are started, so make sure Mumble is running and this option is on before you start the application.&lt;br /&gt;Please note that if you start the application after starting Mumble, or if you disable the overlay while running, there is no safe way to restart the overlay without also restarting the application.</oldsource>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="100"/>
        <source>This sets the relative Y position of the overlay.</source>
        <translation>Ustawia pozycję Y nakładki.</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="177"/>
        <source>Current Font</source>
        <translation>Obecna czcionka</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="191"/>
        <source>Set Font</source>
        <translation>Czcionka</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="205"/>
        <source>Maximum height of names.</source>
        <translation>Maksymalna wysokość nazw użytkownika.</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="208"/>
        <source>This sets the maximum height of names shown, relative to the screen height. If your active 3D window is 800 pixels tall and this is set to 5%, each name will be 40 pixels tall. Note that the names will not be taller than 60 pixels no matter what you set here.</source>
        <translation>Maksymalna wysokość nazw użytkownika na nakładce.</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="198"/>
        <source>Maximum height</source>
        <translation>Maksymalna wysokość</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="238"/>
        <location filename="Overlay.ui" line="259"/>
        <location filename="Overlay.ui" line="280"/>
        <location filename="Overlay.ui" line="301"/>
        <location filename="Overlay.ui" line="322"/>
        <source>Change</source>
        <translation>Zmień</translation>
    </message>
    <message>
        <source>Color for players</source>
        <translation type="obsolete">Kolor użytkownika nieaktywnego</translation>
    </message>
    <message>
        <source>Color for talking players</source>
        <translation type="obsolete">Kolor użytkownika aktywnego</translation>
    </message>
    <message>
        <source>Color for alt-talking players</source>
        <translation type="obsolete">Alternatywny kolor użytkownika aktywnego</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="287"/>
        <source>Color for Channels</source>
        <translation>Kolor kanałów</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="308"/>
        <source>Color for active Channels</source>
        <translation>Kolor aktywnych kanałów</translation>
    </message>
    <message>
        <location filename="Overlay.cpp" line="142"/>
        <source>Overlay</source>
        <translation>Nakładka</translation>
    </message>
    <message>
        <location filename="Overlay.cpp" line="47"/>
        <source>Show no one</source>
        <translation>nie pokazuj nikogo</translation>
    </message>
    <message>
        <location filename="Overlay.cpp" line="48"/>
        <source>Show only talking</source>
        <translation>pokazuj tylko rozmawiających</translation>
    </message>
    <message>
        <location filename="Overlay.cpp" line="49"/>
        <source>Show everyone</source>
        <translation>pokazuj wszystkich</translation>
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
        <location filename="Overlay.ui" line="14"/>
        <source>Form</source>
        <translation>Formularz</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="87"/>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow upwards if needed.</source>
        <translation>Nakładka stara się pozostać najmniejszą jak tylko może w pozycji, którą ustawiłeś. Ta opcja pozwala nakładce rosnąć w górę jeśli trzeba.</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="126"/>
        <source>Let overlay grow to the right</source>
        <translation>Pozwala nakładce rosnąć do prawej</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="129"/>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow to the right if needed.</source>
        <translation>Nakładka stara się pozostać najmniejszą jak tylko może w pozycji, którą ustawiłeś. Ta opcja pozwala nakładce rosnąć w prawo jeśli trzeba.</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="142"/>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow downwards if needed.</source>
        <translation>Nakładka stara się pozostać najmniejszą jak tylko może w pozycji, którą ustawiłeś. Ta opcja pozwala nakładce rosnąć w dół jeśli trzeba.</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="224"/>
        <source>TextLabel</source>
        <translation>Tekst</translation>
    </message>
</context>
<context>
    <name>PlayerModel</name>
    <message>
        <source>This is a player connected to the server. The icon to the left of the player indicates whether or not they are talking:&lt;br /&gt;&lt;img src=&quot;skin:talking_on.png&quot; /&gt; Talking&lt;br /&gt;&lt;img src=&quot;skin:talking_off.png&quot; /&gt; Not talking</source>
        <translation type="obsolete">To jest użytkownik podłączony do serwera. Ikony wskazują następujące czynności: &lt;br /&gt;&lt;img src=&quot;skin:talking_on.png&quot; /&gt;Użytkownik mówiący&lt;br /&gt;&lt;img src=&quot;skin:talking_off.png&quot;/&gt;Użytkownik nie mówi</translation>
    </message>
    <message>
        <source>This is a channel on the server. Only players in the same channel can hear each other.</source>
        <translation type="obsolete">To jest kanał na serwerze, użytkownicy na tym samym kanale mogą rozmawiać między sobą.</translation>
    </message>
    <message>
        <source>This shows the flags the player has on the server, if any:&lt;br /&gt;&lt;img src=&quot;skin:authenticated.png&quot; /&gt;Authenticated user&lt;br /&gt;&lt;img src=&quot;skin:muted_self.png&quot; /&gt;Muted (by self)&lt;br /&gt;&lt;img src=&quot;skin:muted_server.png&quot; /&gt;Muted (by admin)&lt;br /&gt;&lt;img src=&quot;skin:deafened_self.png&quot; /&gt;Deafened (by self)&lt;br /&gt;&lt;img src=&quot;skin:deafened_server.png&quot; /&gt;Deafened (by admin)&lt;br /&gt;A player muted by himself is probably just away, talking on the phone or something like that.&lt;br /&gt;A player muted by an admin is probably also just away, and the noise the player is making was annoying enough that an admin muted him.</source>
        <translation type="obsolete">To są atrybuty jakie użytkownik ma na serwerze:&lt;br /&gt;&lt;img src=&quot;skin:authenticated.png&quot; /&gt;Zarejestrowany użytkownik&lt;br /&gt;&lt;img src=&quot;skin:muted_self.png&quot; /&gt;Wyciszony mikrofon (przez użytkownika)&lt;br /&gt;&lt;img src=&quot;skin:muted_server.png&quot; /&gt;Wyciszony mikrofon (przez administratora)&lt;br /&gt;&lt;img src=&quot;skin:deafened_self.png&quot; /&gt;Wyciszone słuchawki (przez użytkownika)&lt;br /&gt;&lt;img src=&quot;skin:deafened_server.png&quot; /&gt;Wyciszone słuchawki (przez administratora)&lt;br /&gt;
Użytkownik, który ma wyciszony mikrofon lub słuchawki przez administratora prawdopodobnie jest po prostu z dala od komputera lub denerwował innych i został wyciszony.
</translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="obsolete">Kanał(y)</translation>
    </message>
    <message>
        <source>Flags</source>
        <translation type="obsolete">Atrybut(y)</translation>
    </message>
    <message>
        <source>Mumble</source>
        <translation type="obsolete">Mumble</translation>
    </message>
    <message>
        <source>Are you sure you want to drag this channel?</source>
        <translation type="obsolete">Czy jesteś pewien, że chcesz przenieść ten kanał?</translation>
    </message>
</context>
<context>
    <name>PluginConfig</name>
    <message>
        <location filename="Plugins.ui" line="20"/>
        <source>Options</source>
        <translation>Opcje</translation>
    </message>
    <message>
        <location filename="Plugins.cpp" line="67"/>
        <location filename="Plugins.ui" line="42"/>
        <source>Plugins</source>
        <translation>Wtyczki</translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="32"/>
        <source>Link to Game and Transmit Position</source>
        <translation>Połącz z grami i nadawaj pozycję graczy</translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="26"/>
        <source>Enable plugins and transmit positional information</source>
        <translation>Uaktywnia wszystkie wtyczki, które są odpowiedzialne miedzy innymi za opcję dźwięku pozycyjnego</translation>
    </message>
    <message>
        <source>This enables plugins for supported games to fetch your in-game position and transmit that with each voice packet. This enables other players to hear your voice in-game from the direction your character is in relation to their own.</source>
        <translation type="obsolete">Uaktywnia to wtyczki, które przetwarzają dźwięk w zależności od położenia użytkownika na mapie, dzięki czemu możesz usłyszeć użytkowników ze względu na położenie na mapie. Ta funckja jest dostępna tylko do wybranych gier.</translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="59"/>
        <source>&amp;Reload plugins</source>
        <translation>&amp;Przeładuj wtyczkę</translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="53"/>
        <source>Reloads all plugins</source>
        <translation>Przeładuj wtyczki</translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="29"/>
        <source>This allows plugins for supported games to fetch your in-game position and transmit it with each voice packet. This enables other users to hear your voice in-game from the direction your character is in relation to their own.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="56"/>
        <source>This rescans and reloads plugins. Use this if you just added or changed a plugin to the plugins directory.</source>
        <translation>Przeładowuje wszystkie wtyczki. Użyj tej opcji jeśli dopiero co dodałeś jakąś nową wtyczkę do folderu plugins.</translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="85"/>
        <source>&amp;About</source>
        <translation>&amp;O wtyczce</translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="79"/>
        <source>Information about plugin</source>
        <translation>Informacje o wtyczce</translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="82"/>
        <source>This shows a small information message about the plugin.</source>
        <translation>Pokazuje informacje o wtyczce.</translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="98"/>
        <source>&amp;Configure</source>
        <translation>&amp;Konfiguruj</translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="92"/>
        <source>Show configuration page of plugin</source>
        <translation>Pokaż ustawienia wtyczki</translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="95"/>
        <source>This shows the configuration page of the plugin, if any.</source>
        <translation>Pokazuje ustawienia danej wtyczki, o ile jaką kolwiek posiada.</translation>
    </message>
    <message>
        <location filename="Plugins.cpp" line="95"/>
        <location filename="Plugins.cpp" line="107"/>
        <source>Mumble</source>
        <translation>Mumble</translation>
    </message>
    <message>
        <location filename="Plugins.cpp" line="95"/>
        <source>Plugin has no configure function.</source>
        <translation>Wtyczka nie posiada konfiguracji.</translation>
    </message>
    <message>
        <location filename="Plugins.cpp" line="107"/>
        <source>Plugin has no about function.</source>
        <translation>Nie ma informacji o wtyczce.</translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="14"/>
        <source>Form</source>
        <translation>Formularz</translation>
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
        <translation>%1 utracił połączenie.</translation>
    </message>
    <message>
        <location filename="Plugins.cpp" line="333"/>
        <source>%1 linked.</source>
        <translation>%1 połączony.</translation>
    </message>
</context>
<context>
    <name>PortAudioSystem</name>
    <message>
        <location filename="PAAudio.cpp" line="275"/>
        <source>Default Device</source>
        <translation>Urządzenie domyślne</translation>
    </message>
</context>
<context>
    <name>PulseAudioSystem</name>
    <message>
        <location filename="PulseAudio.cpp" line="549"/>
        <source>Default Input</source>
        <translation>Domyślne Wejście</translation>
    </message>
    <message>
        <location filename="PulseAudio.cpp" line="550"/>
        <source>Default Output</source>
        <translation>Domyślne Wyjście</translation>
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
        <translation type="unfinished">URL</translation>
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
        <translation>Nieprzypisany</translation>
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
        <translation type="unfinished">Wył.</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.cpp" line="578"/>
        <source>Toggle</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="GlobalShortcut.cpp" line="584"/>
        <source>Unassigned</source>
        <translation>Nieprzypisany</translation>
    </message>
</context>
<context>
    <name>ShortcutKeyWidget</name>
    <message>
        <location filename="GlobalShortcut.cpp" line="150"/>
        <source>Press Shortcut</source>
        <translation>Naciśnij klawisz, którego chcesz użyć jako skrótu</translation>
    </message>
</context>
<context>
    <name>ShortcutTargetDialog</name>
    <message>
        <location filename="GlobalShortcut.cpp" line="353"/>
        <source>Root</source>
        <translation type="unfinished">Źródło</translation>
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
        <translation type="unfinished">...</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.cpp" line="493"/>
        <source>, </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="GlobalShortcut.cpp" line="499"/>
        <source>Root</source>
        <translation type="unfinished">Źródło</translation>
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
        <translation type="unfinished">Wył.</translation>
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
        <source>Message to send</source>
        <translation type="obsolete">Wiadomość do wysłania</translation>
    </message>
    <message>
        <source>Preview</source>
        <translation type="obsolete">Podgląd</translation>
    </message>
    <message>
        <source>Send raw message</source>
        <translation type="obsolete">Wyślij wiadomość bez HTML</translation>
    </message>
    <message>
        <source>Disables html formating</source>
        <translation type="obsolete">Wyłącza HTML</translation>
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
        <translation type="unfinished">&amp;Dodaj</translation>
    </message>
    <message>
        <location filename="Tokens.ui" line="57"/>
        <source>Remove a token</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Tokens.ui" line="60"/>
        <source>&amp;Remove</source>
        <translation type="unfinished">&amp;Usuń</translation>
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
        <translation type="unfinished">Usuń</translation>
    </message>
</context>
<context>
    <name>UserModel</name>
    <message>
        <location filename="UserModel.cpp" line="524"/>
        <source>This is a user connected to the server. The icon to the left of the user indicates whether or not they are talking:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="525"/>
        <source>Talking to your channel.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="526"/>
        <source>Whispering directly to your channel.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="527"/>
        <source>Whispering directly to you.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="528"/>
        <source>Not talking.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="536"/>
        <source>This is a channel on the server. The icon indicates the state of the channel:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="537"/>
        <source>Your current channel.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="538"/>
        <source>A channel that is linked with your channel. Linked channels can talk to each other.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="539"/>
        <source>A channel on the server that you are not linked to.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="555"/>
        <source>This shows the flags the user has on the server, if any:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="556"/>
        <source>On your friend list</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="557"/>
        <source>Authenticated user</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="558"/>
        <source>Muted (manually muted by self)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="559"/>
        <source>Muted (manually muted by admin)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="560"/>
        <source>Muted (not allowed to speak in current channel)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="561"/>
        <source>Muted (muted by you, only on your machine)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="563"/>
        <source>Deafened (by self)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="564"/>
        <source>Deafened (by admin)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="565"/>
        <source>User has a new comment set (click to show)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="566"/>
        <source>User has a comment set, which you&apos;ve already seen. (click to show)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="573"/>
        <source>This shows the flags the channel has, if any:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="574"/>
        <source>Channel has a new comment set (click to show)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="575"/>
        <source>Channel has a comment set, which you&apos;ve already seen. (click to show)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="593"/>
        <source>Name</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="595"/>
        <source>Flags</source>
        <translation type="unfinished">Atrybut(y)</translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="1242"/>
        <location filename="UserModel.cpp" line="1332"/>
        <location filename="UserModel.cpp" line="1352"/>
        <source>Mumble</source>
        <translation type="unfinished">Mumble</translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="1242"/>
        <source>Are you sure you want to drag this channel?</source>
        <translation type="unfinished">Czy jesteś pewien, że chcesz przenieść ten kanał?</translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="1332"/>
        <location filename="UserModel.cpp" line="1352"/>
        <source>Cannot perform this movement automatically, please reset the numeric sorting indicators or adjust it manually.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>VersionCheck</name>
    <message>
        <location filename="VersionCheck.cpp" line="97"/>
        <source>Mumble failed to retrieve version information from the SourceForge server.</source>
        <translation>Uzyskanie informacji o najnowszej wersji nie powiodło się.</translation>
    </message>
</context>
<context>
    <name>ViewCert</name>
    <message>
        <location filename="ViewCert.cpp" line="36"/>
        <source>Certificate Chain Details</source>
        <translation>Szczegóły certyfikatu</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="42"/>
        <source>Certificate chain</source>
        <translation>Certyfikat</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="51"/>
        <source>Certificate details</source>
        <translation>Szczegóły certyfikatu</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="48"/>
        <source>%1 %2</source>
        <translation>%1 %2</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="76"/>
        <location filename="ViewCert.cpp" line="89"/>
        <source>Common Name: %1</source>
        <translation>Nazwa pospolita: %1</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="77"/>
        <location filename="ViewCert.cpp" line="90"/>
        <source>Organization: %1</source>
        <translation>Organizacja: %1</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="78"/>
        <source>Subunit: %1</source>
        <translation>Pod-jednostka: %1</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="79"/>
        <location filename="ViewCert.cpp" line="92"/>
        <source>Country: %1</source>
        <translation>Kraj: %1</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="80"/>
        <location filename="ViewCert.cpp" line="93"/>
        <source>Locality: %1</source>
        <translation>Lokalizacja: %1</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="81"/>
        <location filename="ViewCert.cpp" line="94"/>
        <source>State: %1</source>
        <translation>Kraj: %1</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="82"/>
        <source>Valid from: %1</source>
        <translation>Ważne od: %1</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="83"/>
        <source>Valid to: %1</source>
        <translation>Ważne do: %1</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="84"/>
        <source>Serial: %1</source>
        <translation>Numer seryjny: %1</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="85"/>
        <source>Public Key: %1 bits %2</source>
        <translation>Klucz publiczny: %1 bitów %2</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="85"/>
        <source>RSA</source>
        <translation>RSA</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="85"/>
        <source>DSA</source>
        <translation>DSA</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="86"/>
        <source>Digest (MD5): %1</source>
        <translation>Odcisk (MD5): %1</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="88"/>
        <source>Issued by:</source>
        <translation>Wydany przez:</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="91"/>
        <source>Unit Name: %1</source>
        <translation>Nazwa jednostki: %1</translation>
    </message>
</context>
<context>
    <name>WASAPISystem</name>
    <message>
        <location filename="WASAPI.cpp" line="177"/>
        <source>Default Device</source>
        <translation>Urządzenie domyślne</translation>
    </message>
</context>
</TS>
