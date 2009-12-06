<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.0" language="cs">
<context>
    <name>ACLEditor</name>
    <message>
        <source>Mumble - Edit ACL for %1</source>
        <translation type="obsolete">Mumble - upravit ACL pro %1</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="404"/>
        <source>Active ACLs</source>
        <translation>Aktivní pravidla ACL</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="506"/>
        <source>Context</source>
        <translation>Kontext</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="547"/>
        <source>User/Group</source>
        <translation>Uživatel/skupina</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="572"/>
        <source>This controls which group of users this entry applies to.&lt;br /&gt;Note that the group is evaluated in the context of the channel the entry is used in. For example, the default ACL on the Root channel gives &lt;i&gt;Write&lt;/i&gt; permission to the &lt;i&gt;admin&lt;/i&gt; group. This entry, if inherited by a channel, will give a user write privileges if he belongs to the &lt;i&gt;admin&lt;/i&gt; group in that channel, even if he doesn&apos;t belong to the &lt;i&gt;admin&lt;/i&gt; group in the channel where the ACL originated.&lt;br /&gt;If a group name starts with a &apos;!&apos;, its membership is negated, and if it starts with a &apos;~&apos;, it is evaluated in the channel the ACL was defined in, rather than the channel the ACL is active in.&lt;br /&gt;If a group name starts with a  &apos;#&apos;, it is interpreted as an access token. Users must have entered whatever follows the &apos;#&apos; in their list of access tokens to match. This can be used for very simple password access to channels for non-authenticated users.&lt;br /&gt;If a group name starts with a  &apos;$&apos;, it will only match users whose certificate hash matches what follows the &apos;$&apos;.&lt;br /&gt;A few special predefined groups are:&lt;br /&gt;&lt;b&gt;all&lt;/b&gt; - Everyone will match.&lt;br /&gt;&lt;b&gt;auth&lt;/b&gt; - All authenticated users will match.&lt;br /&gt;&lt;b&gt;sub,a,b,c&lt;/b&gt; - User currently in a sub-channel minimum &lt;i&gt;a&lt;/i&gt; common parents, and between &lt;i&gt;b&lt;/i&gt; and &lt;i&gt;c&lt;/i&gt; channels down the chain. See the website for more extensive documentation on this one.&lt;br /&gt;&lt;b&gt;in&lt;/b&gt; - Users currently in the channel will match (convenience for &apos;&lt;i&gt;sub,0,0,0&lt;/i&gt;&apos;).&lt;br /&gt;&lt;b&gt;out&lt;/b&gt; - Users outside the channel will match (convenience for &apos;&lt;i&gt;!sub,0,0,0&lt;/i&gt;&apos;).&lt;br /&gt;Note that an entry applies to either a user or a group, not both.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="619"/>
        <source>Permissions</source>
        <translation>Oprávnění</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="125"/>
        <location filename="ACLEditor.ui" line="553"/>
        <source>Group</source>
        <translation>Skupina</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="238"/>
        <location filename="ACLEditor.ui" line="244"/>
        <source>Members</source>
        <translation>Členové</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="471"/>
        <source>&amp;Add</source>
        <translation>Přid&amp;at</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="487"/>
        <source>&amp;Remove</source>
        <translation>Odst&amp;ranit</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="439"/>
        <source>&amp;Up</source>
        <translation>Nahor&amp;u</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="455"/>
        <source>&amp;Down</source>
        <translation>&amp;Ðolů</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="426"/>
        <source>Inherit ACLs</source>
        <translation>Zdědit ACL</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="531"/>
        <source>Applies to this channel</source>
        <translation>Použít pro tento kanál</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="518"/>
        <source>Applies to sub-channels</source>
        <translation>Použít pro kanály umístěné v tomto kanále</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="582"/>
        <source>User ID</source>
        <translation>Uživatelovo ID</translation>
    </message>
    <message>
        <location filename="ACLEditor.cpp" line="111"/>
        <source>Deny</source>
        <translation>Zakázat</translation>
    </message>
    <message>
        <location filename="ACLEditor.cpp" line="113"/>
        <source>Allow</source>
        <translation>Povolit</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="183"/>
        <location filename="ACLEditor.ui" line="361"/>
        <location filename="ACLEditor.ui" line="368"/>
        <source>Remove</source>
        <translation>Odstranit</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="199"/>
        <source>Inherit</source>
        <translation>Zdědit</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="212"/>
        <source>Inheritable</source>
        <translation>Děditelné</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="228"/>
        <source>Inherited</source>
        <translation>Zděděno</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="170"/>
        <location filename="ACLEditor.ui" line="322"/>
        <location filename="ACLEditor.ui" line="354"/>
        <source>Add</source>
        <translation>Přidat</translation>
    </message>
    <message>
        <source>Add to Remove</source>
        <translation type="obsolete">Přidat k odstranění</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="119"/>
        <source>&amp;Groups</source>
        <translation>S&amp;kupiny</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="396"/>
        <source>&amp;ACL</source>
        <translation>&amp;ACL</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>Accept changes</source>
        <translation type="obsolete">Potvrdit změny</translation>
    </message>
    <message>
        <source>This button will accept current groups/ACLs and send them to the server. Note that if you mistakenly remove write permission from yourself, the server will add it.</source>
        <translation type="obsolete">Tímto tlačítkem přijmete současné skupiny/ALC a pošlete je na server. Pokud si omylem odstraníte pravomoci sami pro sebe, server je opětovně přidá.</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">Z&amp;rušit</translation>
    </message>
    <message>
        <source>Reject changes</source>
        <translation type="obsolete">Odmítnout změny</translation>
    </message>
    <message>
        <source>This button will cancel all changes and closes the dialog without updating the ACLs or groups on the server.</source>
        <translation type="obsolete">Toto tlačítko způsobí, že okno bude zavřeno, aniž by se upravily záznamy ACL nebo skupin na serveru.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="410"/>
        <source>List of entries</source>
        <translation>Seznam položek</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="420"/>
        <source>Inherit ACL of parent?</source>
        <translation>Zdědit ACL mateřského kanálu?</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="423"/>
        <source>This sets whether or not the ACL up the chain of parent channels are applied to this object. Only those entries that are marked in the parent as &quot;Apply to sub-channels&quot; will be inherited.</source>
        <translation>Nastavuje, jestli ACL nadřazeného kanálu bude aplikováno na tento objekt. Zděděny mohou být pouze položky označené v nadřazeném kanálu jako &quot;aplikovat na podkanály&quot;.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="465"/>
        <source>Add new entry</source>
        <translation>Přidat záznam</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="468"/>
        <source>This adds a new entry, initially set with no permissions and applying to all.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="481"/>
        <source>Remove entry</source>
        <translation>Odstranit záznam</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="484"/>
        <source>This removes the currently selected entry.</source>
        <translation>Odstraní vybraný záznam.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="433"/>
        <source>Move entry up</source>
        <translation>Přesunout záznam nahoru</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="24"/>
        <source>Properties</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="33"/>
        <source>Name</source>
        <translation type="unfinished">Jméno</translation>
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
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">Umístění</translation>
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
        <translation>Přesune záznam nahoru v seznamu. Protože záznamy pravidel jsou vyhodnocovány postupně, to může změnit privilegia přidělená uživateli. Nemůžete přesunout záznam nad zděděný záznam -- pokud je to opravdu potřeba pak musíte duplikovat zděděný záznam.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="449"/>
        <source>Move entry down</source>
        <translation>Přesunout záznam dolů</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="452"/>
        <source>This moves the entry down in the list. As entries are evaluated in order, this may change the effective permissions of users.</source>
        <translation>Přesune záznam dolů v seznamu. Protože záznamy pravidel jsou vyhodnocovány postupně, může to změnit privilegia přidělená uživateli.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="525"/>
        <source>Entry should apply to this channel.</source>
        <oldsource>Entry should apply to this channel</oldsource>
        <translation type="unfinished">Použít pro tento kanál</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="528"/>
        <source>This makes the entry apply to this channel.</source>
        <translation type="unfinished">Tímto se daná položka stane platnou pro tento kanál.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="512"/>
        <source>Entry should apply to sub-channels.</source>
        <translation>Použít v podkanálech.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="515"/>
        <source>This makes the entry apply to sub-channels of this channel.</source>
        <translation>Tímto se daná položka stane platnou pro podkanály tohoto kanálu.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="569"/>
        <source>Group this entry applies to</source>
        <translation></translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="598"/>
        <source>User this entry applies to</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="601"/>
        <source>This controls which user this entry applies to. Just type in the user name and hit enter to query the server for a match.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ACLEditor.cpp" line="135"/>
        <source>Allow %1</source>
        <translation>Povolit %1</translation>
    </message>
    <message>
        <location filename="ACLEditor.cpp" line="126"/>
        <source>Deny %1</source>
        <translation>Odepřít %1</translation>
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
        <translation>Tímto se povolí privilegium %1. Pokud je privilegium zároveň přiděleno i odepřeno, považuje se za odepřené.&lt;br /&gt;%2</translation>
    </message>
    <message>
        <location filename="ACLEditor.cpp" line="127"/>
        <source>This revokes the %1 privilege. If a privilege is both allowed and denied, it is denied.&lt;br /&gt;%2</source>
        <translation>Odebere privilegium %1. Pokud je privilegium  zároveň přiděleno i odepřeno, považuje se za odepřené.&lt;br /&gt;%2</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="149"/>
        <source>List of groups</source>
        <translation>Seznam skupin</translation>
    </message>
    <message>
        <source>This is all the groups currently defined for the channel. To create a new group, just type in the name and press enter.</source>
        <translation type="obsolete">Toto je seznam skupin definovaných pro tento kanál. Pro vytvoření nové skupiny jen napište její jméno a zmáčkněte klávesu Enter.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="177"/>
        <source>Remove selected group</source>
        <translation>Odstranit vybranou skupinu</translation>
    </message>
    <message>
        <source>This removes the currently selected group. If the group was inherited, it will not be removed from the list, but all local information about the group will be cleared.</source>
        <translation type="obsolete">Odstraní vybranou skupinu. Pokud byla skupina zděděna, nebude odstraněna ze seznamu, ale všechny místní informace o skupině budou smazány. </translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="193"/>
        <source>Inherit group members from parent</source>
        <translation>Zdědit členy skupiny od rodiče</translation>
    </message>
    <message>
        <source>This inherits all the members in the group from the parent, if the group is marked as &lt;i&gt;Inheritable&lt;/i&gt; in the parent channel.</source>
        <translation type="obsolete">Tímto se zdědí všichni členové skupiny z rodiče, pokud je skupina označena jako &lt;i&gt;děditelná&lt;/i&gt; v nadřazeném kanálu.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="206"/>
        <source>Make group inheritable to sub-channels</source>
        <translation>Učiní skupiny děditelnými pro podkanály</translation>
    </message>
    <message>
        <source>This makes this group inheritable to sub-channels. If the group is non-inheritable, sub-channels are still free to create a new group with the same name.</source>
        <translation type="obsolete">Učiní tuto skupinu děditelnou do podkanálů. Pokud je skupina neděditelná, podkanály mohou zvovy vytvořit stejně pojmenovanou skupinu.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="222"/>
        <source>Group was inherited from parent channel</source>
        <translation>Skupina byla zděděna od mateřského kanálu</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="303"/>
        <source>Add member to group</source>
        <translation>Přidat člena do skupiny</translation>
    </message>
    <message>
        <source>Type in the name of a player you wish to add to the group and press enter.</source>
        <translation type="obsolete">Napište jméno uživatele, kterého chcete přidat do skupiny a zmáčkněte enter.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="335"/>
        <source>Remove member from group</source>
        <translation>Odebrat uživatele ze skupiny</translation>
    </message>
    <message>
        <source>Type in the name of a player you wish to remove from the group and press enter.</source>
        <translation type="obsolete">Napište jméno uživatele, kterého chcete odstranit ze skupiny a zmáčkněte enter.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="14"/>
        <source>Dialog</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="413"/>
        <source>This shows all the entries active on this channel. Entries inherited from parent channels will be show in italics.&lt;br /&gt;ACLs are evaluated top to bottom, meaning priority increases as you move down the list.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ALSAAudioInput</name>
    <message>
        <location filename="ALSAAudio.cpp" line="363"/>
        <source>Opening chosen ALSA Input failed: %1</source>
        <translation>Otevírání zvoleného zvukového vstupu selhalo. Nebude tedy snímán zvuk ze žádného mikrofonu,</translation>
    </message>
    <message>
        <location filename="ALSAAudio.cpp" line="172"/>
        <source>Default ALSA Card</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ALSAAudioOutput</name>
    <message>
        <location filename="ALSAAudio.cpp" line="498"/>
        <source>Opening chosen ALSA Output failed: %1</source>
        <translation>Otevírání zvukového výstupu selhalo: %1</translation>
    </message>
    <message>
        <location filename="ALSAAudio.cpp" line="173"/>
        <source>Default ALSA Card</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ALSAConfig</name>
    <message>
        <source>Device selection</source>
        <translation type="obsolete">Výběr zařízení</translation>
    </message>
    <message>
        <source>Device to use for microphone</source>
        <translation type="obsolete">Zařízení, na kterém je mikrofon</translation>
    </message>
    <message>
        <source>This set which device mumble should use. The &lt;i&gt;default&lt;/i&gt; device is whatever you have configured in alsaconfig, the &lt;i&gt;hwplug&lt;/i&gt; devices are specific hardware devices backed by the ALSA mixer and the &lt;i&gt;hw&lt;/i&gt; devices are raw hardware access. Unless your soundcard supports hardware mixing of audio, using the &lt;i&gt;hw&lt;/i&gt; device will exclude all other programs from using audio.</source>
        <translation type="obsolete">Nastavení zařízení pro použití aplikací Mumble. &lt;i&gt;default&lt;/i&gt; (výchozí) je cokoliv je nakonfigurováno pomoci alsaconfigu, &lt;i&gt;hwplug&lt;/i&gt; zařízení jsou specifická zvuková zařízení podporovaná ALSA mixerem a &lt;i&gt;hw&lt;/i&gt; zařízní jsou pro přímý přístup na zařízení. Pokud vaše zvuková karta nepodporuje hw směšování, použití &lt;i&gt;hw&lt;/i&gt; zařázení znemožní ostatním aplikacím používat tento zvukový výstup.</translation>
    </message>
    <message>
        <source>Device to use for speakers/headphones</source>
        <translation type="obsolete">Zařízení pro zvukový výstup (bedničky nebo sluchátka)</translation>
    </message>
    <message>
        <source>Input</source>
        <translation type="obsolete">Vstup</translation>
    </message>
    <message>
        <source>Output</source>
        <translation type="obsolete">Výstup</translation>
    </message>
    <message>
        <source>Output Options</source>
        <translation type="obsolete">Výstupní volby</translation>
    </message>
    <message>
        <source>Output Delay</source>
        <translation type="obsolete">Výstupní opoždění</translation>
    </message>
    <message>
        <source>Amount of data to buffer for ALSA</source>
        <translation type="obsolete">Velikost datového zásobníku pro ALSU</translation>
    </message>
    <message>
        <source>This sets the amount of data to prebuffer in the output buffer. Experiment with different values and set it to the lowest which doesn&apos;t cause rapid jitter in the sound.</source>
        <translation type="obsolete">Nastaví velikost dat pro výstupní zásobník. Zkoušejte různé hodnoty a nastavte to na nejmenší hodnotu, která nezpůsobuje rychlé cvakání ve zvuku.</translation>
    </message>
    <message>
        <source>ALSA</source>
        <translation type="obsolete">ALSA</translation>
    </message>
    <message>
        <source>%1ms</source>
        <translation type="obsolete">%1ms</translation>
    </message>
</context>
<context>
    <name>ASIOConfig</name>
    <message>
        <location filename="ASIOInput.ui" line="20"/>
        <source>Device selection</source>
        <translation type="unfinished">Výběr zařízení</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="81"/>
        <source>Capabilities</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="130"/>
        <source>Configure input channels</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="42"/>
        <source>Device to use for microphone</source>
        <translation type="unfinished">Zařízení, na kterém je mikrofon</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="45"/>
        <source>This chooses what device to query. You still need to actually query the device and select which channels to use.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="26"/>
        <source>Device</source>
        <translation type="unfinished">Zařízení</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="58"/>
        <source>&amp;Query</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="52"/>
        <source>Query selected device</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="55"/>
        <source>This queries the selected device for channels. Be aware that many ASIO drivers are buggy to the extreme, and querying them might cause a crash of either the application or the system.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="71"/>
        <source>&amp;Configure</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="65"/>
        <source>Configure selected device</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="68"/>
        <source>This configures the selected device. Be aware that many ASIO drivers are buggy to the extreme, and querying them might cause a crash of either the application or the system.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="87"/>
        <source>Driver name</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="219"/>
        <location filename="ASIOInput.ui" line="292"/>
        <source>-&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="243"/>
        <source>Unused</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="329"/>
        <source>Speakers</source>
        <translation>Bedničky</translation>
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
        <translation type="unfinished">Mumble</translation>
    </message>
    <message>
        <location filename="ASIOInput.cpp" line="269"/>
        <location filename="ASIOInput.cpp" line="296"/>
        <source>ASIO Initialization failed: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ASIOInput.cpp" line="273"/>
        <location filename="ASIOInput.cpp" line="300"/>
        <source>Failed to instantiate ASIO driver</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ASIOInput.cpp" line="337"/>
        <source>ASIO</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="14"/>
        <source>Form</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ASIOInput</name>
    <message>
        <location filename="ASIOInput.cpp" line="417"/>
        <location filename="ASIOInput.cpp" line="520"/>
        <source>Mumble</source>
        <translation type="unfinished">Mumble</translation>
    </message>
    <message>
        <location filename="ASIOInput.cpp" line="417"/>
        <source>You need to select at least one microphone and one speaker source to use ASIO. If you just need microphone sampling, use DirectSound.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ASIOInput.cpp" line="520"/>
        <source>Opening selected ASIO device failed. No input will be done.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>AboutDialog</name>
    <message>
        <source>&lt;h3&gt;Mumble v%1&lt;/h3&gt;&lt;p&gt;Copyright 2005-2008 Thorvald Natvig&lt;br /&gt;slicer@users.sourceforge.net&lt;/p&gt;&lt;p&gt;&lt;b&gt;A voice-chat utility for gamers&lt;/b&gt;&lt;/p&gt;&lt;p&gt;&lt;tt&gt;http://mumble.sourceforge.net/&lt;/tt&gt;&lt;/p&gt;</source>
        <translation type="obsolete">&lt;h3&gt;Mumble v%1&lt;/h3&gt;&lt;p&gt;Copyright 2005-2007 Thorvald Natvig&lt;br /&gt;slicer@users.sourceforge.net&lt;/p&gt;&lt;p&gt;&lt;b&gt;Nástroj pro hlasovou skupinovou komunikaci.&lt;/b&gt;&lt;/p&gt;&lt;p&gt;Překlad do češtiny v roce 2007 připravil David Pravec, alekibango@danix.org.&lt;/p&gt;
&lt;p&gt;&lt;tt&gt;http://mumble.sourceforge.net/&lt;/tt&gt;&lt;/p&gt;</translation>
    </message>
    <message>
        <location filename="About.cpp" line="63"/>
        <source>&amp;About Mumble</source>
        <translation>O &amp;aplikaci Mumble</translation>
    </message>
    <message>
        <location filename="About.cpp" line="64"/>
        <source>&amp;License</source>
        <translation>&amp;Licenční ujednání</translation>
    </message>
    <message>
        <location filename="About.cpp" line="66"/>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <location filename="About.cpp" line="37"/>
        <source>About Mumble</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="About.cpp" line="53"/>
        <source>&lt;h3&gt;Mumble (%1)&lt;/h3&gt;&lt;p&gt;Copyright %3 Thorvald Natvig&lt;br /&gt;slicer@users.sourceforge.net&lt;/p&gt;&lt;p&gt;&lt;b&gt;A voice-chat utility for gamers&lt;/b&gt;&lt;/p&gt;&lt;p&gt;&lt;tt&gt;&lt;a href=&quot;%2&quot;&gt;%2&lt;/a&gt;&lt;/tt&gt;&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
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
        <source>&lt;h3&gt;About Speex&lt;/h3&gt;&lt;p&gt;&lt;tt&gt;http://www.speex.org/&lt;/tt&gt;&lt;/p&gt;&lt;p&gt;This program uses Speex version %1&lt;/p&gt;&lt;p&gt;Speex is used for echo cancellation, noise&lt;br /&gt;filtering, voice activity detection and speech&lt;br /&gt;compression.&lt;/p&gt;</source>
        <translation type="obsolete">&lt;h3&gt;O kodeku Speex&lt;/h3&gt;&lt;p&gt;&lt;tt&gt;http://www.speex.org/&lt;/tt&gt;&lt;/p&gt;&lt;p&gt;Tento program používá Speex verze %1&lt;/p&gt;&lt;p&gt;Speex je použit pro omezení ozvěny, pro redukci šumu, &lt;br /&gt;pro detekování hlasové aktivity a pro kompresi řeči&lt;/p&gt;</translation>
    </message>
    <message>
        <location filename="About.cpp" line="86"/>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <location filename="About.cpp" line="74"/>
        <source>About Speex</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>AudioConfigDialog</name>
    <message>
        <source>Interfaces</source>
        <translation type="obsolete">Rozhraní</translation>
    </message>
    <message>
        <source>Transmission</source>
        <translation type="obsolete">Přenos</translation>
    </message>
    <message>
        <source>Compression</source>
        <translation type="obsolete">Komprese</translation>
    </message>
    <message>
        <source>Jitter Buffer</source>
        <translation type="obsolete">Zásobník pro potlačení cvakání</translation>
    </message>
    <message>
        <source>Loopback Test</source>
        <translation type="obsolete">Testování místním propojením do smyčky</translation>
    </message>
    <message>
        <source>Input</source>
        <translation type="obsolete">Vstup</translation>
    </message>
    <message>
        <source>Input method for audio</source>
        <translation type="obsolete">Vstupní metoda pro audio</translation>
    </message>
    <message>
        <source>&lt;b&gt;This is the input method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation type="obsolete">&lt;b&gt;Toto je vstupní metoda pro zvuk.&lt;/b&gt;&lt;br /&gt;Asi budete chtít použít ALSA nebo  DirectSound.</translation>
    </message>
    <message>
        <source>Output</source>
        <translation type="obsolete">Výstup</translation>
    </message>
    <message>
        <source>Output method for audio</source>
        <translation type="obsolete">Výstupní metoda pro zvuk</translation>
    </message>
    <message>
        <source>&lt;b&gt;This is the output method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation type="obsolete">&lt;b&gt;To je metoda pro přehrávání zvuku.&lt;/b&gt;&lt;br /&gt; Pravděpodobně budete chtít použít ALSA nebo DirectSound.</translation>
    </message>
    <message>
        <source>Continuous</source>
        <translation type="obsolete">Stále</translation>
    </message>
    <message>
        <source>Voice Activity</source>
        <translation type="obsolete">Při hlasové aktivitě</translation>
    </message>
    <message>
        <source>Push To Talk</source>
        <translation type="obsolete">Mluvení pouze na stisk tlačítka</translation>
    </message>
    <message>
        <source>Transmit</source>
        <translation type="obsolete">Vysílání</translation>
    </message>
    <message>
        <source>When to transmit your speech</source>
        <translation type="obsolete">Kdy vysílat Vaši řeč</translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets when speech should be transmitted.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;Continuous&lt;/i&gt; - All the time&lt;br /&gt;&lt;i&gt;Voice Activity&lt;/i&gt; - When you are speaking clearly.&lt;br /&gt;&lt;i&gt;Push To Talk&lt;/i&gt; - When you hold down the hotkey set under &lt;i&gt;Shortcuts&lt;/i&gt;.</source>
        <translation type="obsolete">Nastaví, kdy bude Vaše řeč přenášena do internetu.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;Stále&lt;/i&gt; - neustálé vysílání&lt;br /&gt;&lt;i&gt;Při hlasové aktivitě&lt;/i&gt; - když je detekována hlasová aktivita .&lt;br /&gt;&lt;i&gt;Mluvení na stisk tlačítka&lt;/i&gt; - mluvení je přenášeno, pouze když držíte stisknutou nadefinovanou klávesu pro mluvení.</translation>
    </message>
    <message>
        <source>Voice Hold</source>
        <translation type="obsolete">Max. prodleva mezi větami</translation>
    </message>
    <message>
        <source>How long to keep transmitting after silence</source>
        <translation type="obsolete">Jak dlouho se ještě vysílá po ukončení hlasové aktivity</translation>
    </message>
    <message>
        <source>&lt;b&gt;This selects how long after a perceived stop in speech transmission should continue.&lt;/b&gt;&lt;br /&gt;Set this higher if your voice breaks up when you speak (seen by a rapidly blinking voice icon next to your name).&lt;br /&gt;Only makes sense when used with Voice Activity transmission.</source>
        <translation type="obsolete">&lt;b&gt;Nastaví, jak dlouho po ukončení hlasových aktivit zůstane vysílání aktivní.&lt;/b&gt;&lt;br /&gt; Nastavte na vyšší hodnotu, pokud je vaše vysílání přerušováno mezi větami (což je vidět i vizuálně na ikonce vedle vašeho jména).&lt;br /&gt;Toto nastavení má smysl pouze pokud je použito vysílání při hlasové aktivitě.</translation>
    </message>
    <message>
        <source>Audio per packet</source>
        <translation type="obsolete">Audio/blok</translation>
    </message>
    <message>
        <source>How many audio frames to send per packet</source>
        <translation type="obsolete">Množství úseků zvuku odeslaných v jednom bloku</translation>
    </message>
    <message>
        <source>&lt;b&gt;This selects how many audio frames should be put in one packet.&lt;/b&gt;&lt;br /&gt;Increasing this will increase the latency of your voice, but will also reduce bandwidth requirements.</source>
        <translation type="obsolete">&lt;b&gt;Vybere kolik úseků zvuku bude zasíláno v jednom bloku dat.&lt;/b&gt;&lt;br /&gt;Zvětšením této hodnoty se zvětší zpoždění v komunikaci, ale také bude efektivnější využití šířky pásma.</translation>
    </message>
    <message>
        <source>PTT Audio cue</source>
        <translation type="obsolete">Zvukový signál na počátku a konci vysílání</translation>
    </message>
    <message>
        <source>Audible audio cue when push-to-talk pressed and released</source>
        <translation type="obsolete">Slyšitelný zvuk signalizující každý začátek a konec vysílání</translation>
    </message>
    <message>
        <source>&lt;b&gt;This enables the audio cues for push to talk.&lt;/b&gt;&lt;br /&gt;Setting this will give you a short audio beep when push to talk is pressed and released.</source>
        <translation type="obsolete">&lt;b&gt;Zahraje krátký signál, když někdo začíná a končí vysílání.&lt;/b&gt;</translation>
    </message>
    <message>
        <source>Use TCP mode</source>
        <translation type="obsolete">Použít TCP režim</translation>
    </message>
    <message>
        <source>Use TCP Compatibility mode</source>
        <translation type="obsolete">Použít TCP režim pro zvýšení kompatibility</translation>
    </message>
    <message>
        <source>&lt;b&gt;Enable TCP Compatibility mode&lt;/b&gt;.&lt;br /&gt;This will make Mumble use only TCP when communicating with the server. This will increase overhead and cause lost packets to produce noticeable pauses in communication, so this should only be used if you are unable to use the default (which uses UDP for voice and TCP for control).</source>
        <translation type="obsolete">&lt;b&gt;Použít TCP režim&lt;/b&gt;&lt;br /&gt;TCP komunikace se serverem zvýší zatížení linky a může způsobit znatelné přestávky v komunikaci, takže by měl být použit pouze pokud výchozí režim (UDP) nemůže fungovat.</translation>
    </message>
    <message>
        <source>Automatic Reconnect</source>
        <translation type="obsolete">Automaticky znovu přihlásit </translation>
    </message>
    <message>
        <source>Reconnect when disconnected</source>
        <translation type="obsolete">Přihlásit zpátky na server, pokud došlo k výpadku spojení</translation>
    </message>
    <message>
        <source>&lt;b&gt;Reconnect when disconnected&lt;/b&gt;.&lt;br /&gt;This will make Mumble try to automatically reconnect after 10 seconds if your server connection fails.</source>
        <translation type="obsolete">&lt;b&gt;Automatické připojení&lt;/b&gt;.&lt;br /&gt;Pokud se ztratí spojení se serverem,  Mumble se pokusí automaticky spojení obnovit každých 10 sekund.</translation>
    </message>
    <message>
        <source>Maximum bandwidth used for sent audio</source>
        <translation type="obsolete">Maximální šířka pásma pro zasílané audio</translation>
    </message>
    <message>
        <source>Outgoing Bitrate</source>
        <translation type="obsolete">Výstupní šířka pásma</translation>
    </message>
    <message>
        <source>Quality</source>
        <translation type="obsolete">Kvalita</translation>
    </message>
    <message>
        <source>Quality of compression (peak bandwidth)</source>
        <translation type="obsolete">Kvalita komprese (maximální šířka pásma)</translation>
    </message>
    <message>
        <source>Complexity</source>
        <translation type="obsolete">Síla komprese</translation>
    </message>
    <message>
        <source>Complexity of compression (CPU)</source>
        <translation type="obsolete">Síla komprese (zvedne zatížení CPU)</translation>
    </message>
    <message>
        <source>Amp</source>
        <translation type="obsolete">Zesílení</translation>
    </message>
    <message>
        <source>Maximum amplification of input sound</source>
        <translation type="obsolete">Maximální zesílení vstupního audio signálu</translation>
    </message>
    <message>
        <source>Default Jitter Buffer</source>
        <translation type="obsolete">Výchozí zásobník pro omezení cvakání</translation>
    </message>
    <message>
        <source>How long to prebuffer on first packet</source>
        <translation type="obsolete">Kolik dat vložit do zásobníku pro první sadu vysílaných dat</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Žádný</translation>
    </message>
    <message>
        <source>Local</source>
        <translation type="obsolete">Místní</translation>
    </message>
    <message>
        <source>Server</source>
        <translation type="obsolete">Server</translation>
    </message>
    <message>
        <source>Loopback</source>
        <translation type="obsolete">Místní smyčka</translation>
    </message>
    <message>
        <source>Desired loopback mode</source>
        <translation type="obsolete">Vyžadovaný režim smyčky</translation>
    </message>
    <message>
        <source>Packet Loss</source>
        <translation type="obsolete">Ztráta paketů</translation>
    </message>
    <message>
        <source>Packet loss for loopback mode</source>
        <translation type="obsolete">Ztráta paketů v režimu místní smyčky</translation>
    </message>
    <message>
        <source>Basic Audio</source>
        <translation type="obsolete">Zvuk</translation>
    </message>
    <message>
        <source>%1 ms</source>
        <translation type="obsolete">%1 ms</translation>
    </message>
    <message>
        <source>%1 s</source>
        <translation type="obsolete">%1 s</translation>
    </message>
    <message>
        <source>%1%</source>
        <translation type="obsolete">%1%</translation>
    </message>
    <message>
        <source>%1kbit/s (Audio %2, Position %4, Overhead %3)</source>
        <translation type="obsolete">%1kbit/s (Audio %2, Pozice %4, režie %3)</translation>
    </message>
    <message>
        <source>Volume</source>
        <translation type="obsolete">Hlasitost</translation>
    </message>
</context>
<context>
    <name>AudioInput</name>
    <message>
        <location filename="AudioInput.ui" line="14"/>
        <source>Form</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="20"/>
        <source>Interface</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="26"/>
        <source>System</source>
        <translation type="unfinished">Systém</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="42"/>
        <source>Input method for audio</source>
        <translation type="unfinished">Vstupní metoda pro audio</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="45"/>
        <source>&lt;b&gt;This is the input method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="68"/>
        <source>Device</source>
        <translation type="unfinished">Zařízení</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="84"/>
        <source>Input device for audio</source>
        <oldsource>Output method for audio</oldsource>
        <translation type="unfinished">Výstupní metoda pro zvuk</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="87"/>
        <source>&lt;b&gt;This is the input device to use for audio.&lt;/b&gt;</source>
        <oldsource>&lt;b&gt;This is the output method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</oldsource>
        <translation type="unfinished">&lt;b&gt;To je metoda pro přehrávání zvuku.&lt;/b&gt;&lt;br /&gt; Pravděpodobně budete chtít použít ALSA nebo DirectSound.</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="141"/>
        <source>Transmission</source>
        <translation type="unfinished">Přenos</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="147"/>
        <source>&amp;Transmit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="157"/>
        <source>When to transmit your speech</source>
        <translation type="unfinished">Kdy vysílat Vaši řeč</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="160"/>
        <source>&lt;b&gt;This sets when speech should be transmitted.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;Continuous&lt;/i&gt; - All the time&lt;br /&gt;&lt;i&gt;Voice Activity&lt;/i&gt; - When you are speaking clearly.&lt;br /&gt;&lt;i&gt;Push To Talk&lt;/i&gt; - When you hold down the hotkey set under &lt;i&gt;Shortcuts&lt;/i&gt;.</source>
        <translation type="unfinished">Nastaví, kdy bude Vaše řeč přenášena do internetu.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;Stále&lt;/i&gt; - neustálé vysílání&lt;br /&gt;&lt;i&gt;Při hlasové aktivitě&lt;/i&gt; - když je detekována hlasová aktivita .&lt;br /&gt;&lt;i&gt;Mluvení na stisk tlačítka&lt;/i&gt; - mluvení je přenášeno, pouze když držíte stisknutou nadefinovanou klávesu pro mluvení.</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="219"/>
        <source>DoublePush Time</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="251"/>
        <source>TextLabel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Audible audio cue when push-to-talk pressed and released</source>
        <translation type="obsolete">Slyšitelný zvuk signalizující každý začátek a konec vysílání</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="341"/>
        <source>&lt;b&gt;This enables the audio cues for push to talk.&lt;/b&gt;&lt;br /&gt;Setting this will give you a short audio beep when push to talk is pressed and released.</source>
        <translation type="unfinished">&lt;b&gt;Zahraje krátký signál, když někdo začíná a končí vysílání.&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="344"/>
        <source>PTT Audio cue</source>
        <translation type="unfinished">Zvukový signál na počátku a konci vysílání</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="357"/>
        <source>Use SNR based speech detection</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="363"/>
        <source>Signal to Noise</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="370"/>
        <source>Use Amplitude based speech detection.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="373"/>
        <source>&lt;b&gt;This sets speech detection to use Amplitude.&lt;/b&gt;&lt;br /&gt;In this mode, the raw strength of the input signal is used to detect speech.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="376"/>
        <source>Amplitude</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="385"/>
        <source>Voice &amp;Hold</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="395"/>
        <source>How long to keep transmitting after silence</source>
        <translation type="unfinished">Jak dlouho se ještě vysílá po ukončení hlasové aktivity</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="398"/>
        <source>&lt;b&gt;This selects how long after a perceived stop in speech transmission should continue.&lt;/b&gt;&lt;br /&gt;Set this higher if your voice breaks up when you speak (seen by a rapidly blinking voice icon next to your name).&lt;br /&gt;Only makes sense when used with Voice Activity transmission.</source>
        <translation type="unfinished">&lt;b&gt;Nastaví, jak dlouho po ukončení hlasových aktivit zůstane vysílání aktivní.&lt;/b&gt;&lt;br /&gt; Nastavte na vyšší hodnotu, pokud je vaše vysílání přerušováno mezi větami (což je vidět i vizuálně na ikonce vedle vašeho jména).&lt;br /&gt;Toto nastavení má smysl pouze pokud je použito vysílání při hlasové aktivitě.</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="443"/>
        <source>Silence Below</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="453"/>
        <location filename="AudioInput.ui" line="485"/>
        <source>&lt;b&gt;This sets the trigger values for voice detection.&lt;/b&gt;&lt;br /&gt;Use this together with the Audio Statistics window to manually tune the trigger values for detecting speech. Input values below &quot;Silence Below&quot; always count as silence. Values above &quot;Speech Above&quot; always count as voice. Values in between will count as voice if you&apos;re already talking, but will not trigger a new detection.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="475"/>
        <source>Speech Above</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="482"/>
        <source>Signal values above this count as voice</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="515"/>
        <source>Compression</source>
        <translation type="unfinished">Komprese</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="521"/>
        <source>&amp;Quality</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="531"/>
        <source>Quality of compression (peak bandwidth)</source>
        <translation type="unfinished">Kvalita komprese (maximální šířka pásma)</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="534"/>
        <source>&lt;b&gt;This sets the quality of compression.&lt;/b&gt;&lt;br /&gt;This determines how much bandwidth Mumble is allowed to use for outgoing audio.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Complexity of compression (CPU)</source>
        <translation type="obsolete">Síla komprese (zvedne zatížení CPU)</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="572"/>
        <source>Audio per packet</source>
        <translation type="unfinished">Audio/blok</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="582"/>
        <source>How many audio frames to send per packet</source>
        <translation type="unfinished">Množství úseků zvuku odeslaných v jednom bloku</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="585"/>
        <source>&lt;b&gt;This selects how many audio frames should be put in one packet.&lt;/b&gt;&lt;br /&gt;Increasing this will increase the latency of your voice, but will also reduce bandwidth requirements.</source>
        <translation type="unfinished">&lt;b&gt;Vybere kolik úseků zvuku bude zasíláno v jednom bloku dat.&lt;/b&gt;&lt;br /&gt;Zvětšením této hodnoty se zvětší zpoždění v komunikaci, ale také bude efektivnější využití šířky pásma.</translation>
    </message>
    <message>
        <source>Maximum bandwidth used for sent audio</source>
        <translation type="obsolete">Maximální šířka pásma pro zasílané audio</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="625"/>
        <source>&lt;b&gt;This shows peak outgoing bandwidth used.&lt;/b&gt;&lt;br /&gt;This shows the peak amount of bandwidth sent out from your machine. Audio bitrate is the maximum bitrate (as we use VBR) for the audio data alone. Position is the bitrate used for positional information. Overhead is our framing and the IP packet headers (IP and UDP is 75% of this overhead).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="641"/>
        <source>Audio Processing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="647"/>
        <source>Noise Suppression</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="657"/>
        <source>Noise suppression</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="660"/>
        <source>&lt;b&gt;This sets the amount of noise suppression to apply.&lt;/b&gt;&lt;br /&gt;The higher this value, the more aggressively stationary noise will be suppressed.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="692"/>
        <source>Amplification</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="702"/>
        <source>Maximum amplification of input sound</source>
        <translation type="unfinished">Maximální zesílení vstupního audio signálu</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="705"/>
        <source>&lt;b&gt;Maximum amplification of input.&lt;/b&gt;&lt;br /&gt;Mumble normalizes the input volume before compressing, and this sets how much it&apos;s allowed to amplify.&lt;br /&gt;The actual level is continually updated based on your current speech pattern, but it will never go above the level specified here.&lt;br /&gt;If the &lt;i&gt;Microphone loudness&lt;/i&gt; level of the audio statistics hover around 100%, you probably want to set this to 2.0 or so, but if, like most people, you are unable to reach 100%, set this to something much higher.&lt;br /&gt;Ideally, set it so &lt;i&gt;Microphone Loudness * Amplification Factor &gt;= 100&lt;/i&gt;, even when you&apos;re speaking really soft.&lt;br /&gt;&lt;br /&gt;Note that there is no harm in setting this to maximum, but Mumble will start picking up other conversations if you leave it to auto-tune to that level.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="433"/>
        <source>Current speech detection chance</source>
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
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="338"/>
        <source>Audible audio cue when push-to-talk is activated or deactivated</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="360"/>
        <source>&lt;b&gt;This sets speech detection to use Signal to Noise ratio.&lt;/b&gt;&lt;br /&gt;In this mode, the input is analyzed for something resembling a clear signal, and the clarity of that signal is used to trigger speech detection.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="436"/>
        <source>&lt;b&gt;This shows the current speech detection settings.&lt;/b&gt;&lt;br /&gt;You can change the settings from the Settings dialog or from the Audio Wizard.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="167"/>
        <source>Idle AutoMute</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="174"/>
        <source>How long to remain idle before auto-mute.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="177"/>
        <source>This sets an idle timer. If the timer expires without any audio being sent to the server, you are muted.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="450"/>
        <source>Signal values below this count as silence</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="622"/>
        <source>Maximum bandwidth used for sending audio</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">Stále</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="75"/>
        <source>Voice Activity</source>
        <translation type="unfinished">Při hlasové aktivitě</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="76"/>
        <source>Push To Talk</source>
        <translation type="unfinished">Mluvení pouze na stisk tlačítka</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="89"/>
        <source>Audio Input</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="185"/>
        <location filename="AudioConfigDialog.cpp" line="193"/>
        <source>%1 ms</source>
        <translation type="unfinished">%1 ms</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="191"/>
        <location filename="AudioConfigDialog.cpp" line="211"/>
        <location filename="AudioConfigDialog.cpp" line="229"/>
        <source>Off</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="199"/>
        <source>%1 s</source>
        <translation type="unfinished">%1 s</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="203"/>
        <source>%1 kb/s</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="214"/>
        <source>-%1 dB</source>
        <translation type="unfinished"></translation>
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
        <translation type="obsolete">%1kbit/s (Audio %2, Pozice %4, režie %3)</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="227"/>
        <source>%1 min</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>AudioOutput</name>
    <message>
        <location filename="AudioOutput.ui" line="14"/>
        <source>Form</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="20"/>
        <source>Interface</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="26"/>
        <source>System</source>
        <translation type="unfinished">Systém</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="42"/>
        <source>Output method for audio</source>
        <oldsource>Input method for audio</oldsource>
        <translation type="unfinished">Vstupní metoda pro audio</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="45"/>
        <source>&lt;b&gt;This is the output method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <oldsource>&lt;b&gt;This is the input method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</oldsource>
        <translation type="unfinished">&lt;b&gt;To je metoda pro přehrávání zvuku.&lt;/b&gt;&lt;br /&gt; Pravděpodobně budete chtít použít ALSA nebo DirectSound.</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="68"/>
        <source>Device</source>
        <translation type="unfinished">Zařízení</translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="110"/>
        <source>Audio Output</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="116"/>
        <source>Default &amp;Jitter Buffer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="126"/>
        <source>Safety margin for jitter buffer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="129"/>
        <source>&lt;b&gt;This sets the minimum safety margin for the jitter buffer.&lt;/b&gt;&lt;br /&gt;All incoming audio is buffered, and the jitter buffer continually tries to push the buffer to the minimum sustainable by your network, so latency can be as low as possible. This sets the minimum buffer size to use. If the start of sentences you hear is very jittery, increase this value.</source>
        <translation type="unfinished"></translation>
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
        <location filename="AudioOutput.ui" line="161"/>
        <source>Volume</source>
        <translation type="unfinished">Hlasitost</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="171"/>
        <source>Volume of incoming speech</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="174"/>
        <source>&lt;b&gt;This adjusts the volume of incoming speech.&lt;/b&gt;&lt;br /&gt;Note that if you increase this beyond 100%, audio will be distorted.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="200"/>
        <source>Output Delay</source>
        <translation type="unfinished">Výstupní opoždění</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="210"/>
        <source>Amount of data to buffer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="397"/>
        <source>Factor for sound volume decrease</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="426"/>
        <source>Bloom</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="471"/>
        <source>Loopback Test</source>
        <translation type="unfinished">Testování místním propojením do smyčky</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="477"/>
        <source>Delay Variance</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="487"/>
        <source>Variance in packet latency</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="490"/>
        <source>&lt;b&gt;This sets the packet latency variance for loopback testing.&lt;/b&gt;&lt;br /&gt;Most audio paths contain some variable latency. This allows you to set that variance for loopback mode testing. For example, if you set this to 15ms, this will emulate a network with 20-35ms ping latency or one with 80-95ms latency. Most domestic net connections have a variance of about 5ms.</source>
        <oldsource>&lt;b&gt;This sets the packet latency variance for loopback testing.&lt;/b&gt;&lt;br /&gt;Most audio paths contain some variable latency. This allows you set that variance for loopback mode testing. For example, if you set this to 15ms, this will emulate a network with 20-35ms ping latency or one with 80-95ms latency. Most domestic net connections have a variance of about 5ms.</oldsource>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="516"/>
        <source>Packet Loss</source>
        <translation type="unfinished">Ztráta paketů</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="526"/>
        <source>Packet loss for loopback mode</source>
        <translation type="unfinished">Ztráta paketů v režimu místní smyčky</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="529"/>
        <source>&lt;b&gt;This sets the packet loss for loopback mode.&lt;/b&gt;&lt;br /&gt;This will be the ratio of packets lost. Unless your outgoing bandwidth is peaked or there&apos;s something wrong with your network connection, this will be 0%</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="561"/>
        <source>&amp;Loopback</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="571"/>
        <source>Desired loopback mode</source>
        <translation type="unfinished">Vyžadovaný režim smyčky</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="449"/>
        <source>Factor for sound volume increase</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="287"/>
        <source>The connected &quot;speakers&quot; are actually headphones</source>
        <oldsource>The connected &quot;speakers&quot; are actually headphones.</oldsource>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="300"/>
        <source>Minimum Distance</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="345"/>
        <source>Maximum Distance</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="355"/>
        <source>Maximum distance, beyond which speech volume won&apos;t decrease</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="387"/>
        <source>Minimum Volume</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="400"/>
        <source>What should the volume be at the maximum distance?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="452"/>
        <source>How much should sound volume increase for sources that are really close?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="290"/>
        <source>Checking this indicates that you don&apos;t have speakers connected, just headphones. This is important, as speakers are usually in front of you, while headphones are directly to your left/right.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="293"/>
        <source>Headphones</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="246"/>
        <source>Other Applications</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="213"/>
        <source>This sets the amount of data to pre-buffer in the output buffer. Experiment with different values and set it to the lowest which doesn&apos;t cause rapid jitter in the sound.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="256"/>
        <source>Volume of other applications during speech</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="259"/>
        <source>&lt;b&gt;Decrease volume of other applications during speech.&lt;/b&gt;&lt;br /&gt;Mumble supports decreasing the volume of other applications during incoming speech. This sets the relative volume of other applications when others are talking.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>AudioOutputDialog</name>
    <message>
        <location filename="AudioConfigDialog.cpp" line="394"/>
        <source>None</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="395"/>
        <source>Local</source>
        <translation type="unfinished">Místní</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="396"/>
        <source>Server</source>
        <translation type="unfinished">Server</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="402"/>
        <source>Audio Output</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="495"/>
        <location filename="AudioConfigDialog.cpp" line="514"/>
        <source>%1 ms</source>
        <translation type="unfinished">%1 ms</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="506"/>
        <location filename="AudioConfigDialog.cpp" line="510"/>
        <location filename="AudioConfigDialog.cpp" line="518"/>
        <location filename="AudioConfigDialog.cpp" line="549"/>
        <location filename="AudioConfigDialog.cpp" line="553"/>
        <source>%1%</source>
        <translation type="unfinished">%1%</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="522"/>
        <source>%1ms</source>
        <translation type="unfinished">%1ms</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="537"/>
        <location filename="AudioConfigDialog.cpp" line="543"/>
        <source>%1m</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>AudioOutputSample</name>
    <message>
        <location filename="AudioOutput.cpp" line="295"/>
        <source>Choose sound file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioOutput.cpp" line="299"/>
        <source>Invalid sound file</source>
        <translation type="unfinished"></translation>
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
        <source>Mumble</source>
        <translation type="obsolete">Mumble</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="28"/>
        <source>Peak microphone level</source>
        <translation>Nejvyšší úroveň mikrofonu</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="48"/>
        <source>Peak speaker level</source>
        <translation>Nejvyšší úroveň výstupu</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="68"/>
        <source>Peak clean level</source>
        <translation>Nejnižší klidová úroveň</translation>
    </message>
    <message>
        <source>Microphone loudness</source>
        <translation type="obsolete">Hlasitost mikrofonu</translation>
    </message>
    <message>
        <source>Mic Signal-To-Noise</source>
        <translation type="obsolete">Poměr signál/šum</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="137"/>
        <source>Speech Probability</source>
        <translation>Pravděpodobnost řeči</translation>
    </message>
    <message>
        <source>Audio bitrate</source>
        <translation type="obsolete">Šířka datového pásma</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="35"/>
        <location filename="AudioStats.ui" line="55"/>
        <location filename="AudioStats.ui" line="75"/>
        <source>Peak power in last frame</source>
        <translation>Max. výkon v posledním časovém úseku</translation>
    </message>
    <message>
        <source>This shows the peak power in the last frame (20 ms), and is the same measurement as you would usually find displayed as &quot;input power&quot;. Please disregard this and look at &lt;b&gt;Loudness&lt;/b&gt; instead, which is much more steady and disregards outliers.</source>
        <translation type="obsolete">Ukazuje maximální výkon v poslením úseku zvuku (20 ms). Této hodnoty si nemusíte všímat, pravděpodobně bude lepší sledovat &lt;b&gt;Hlasitost&lt;/b&gt;, jejíž hodnota se nemění tak rychle.</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="58"/>
        <source>This shows the peak power of the speakers in the last frame (20 ms). Unless you are using a multi-channel sampling method (such as ASIO) with speaker channels configured, this will be 0. If you have such a setup configured, and this still shows 0 while you&apos;re playing audio from other programs, your setup is not working.</source>
        <oldsource>This shows the peak power in the last frame (20 ms) of the speakers. Unless you are using a multi-channel sampling method (such as ASIO) with speaker channels configured, this will be 0. If you have such a setup configured, and this still shows 0 while you&apos;re playing audio from other programs, your setup is not working.</oldsource>
        <translation type="unfinished">Ukazuje maximální výkon zvukového výstupu za poslední časový úsek (20 ms). Pokud nepoužíváte vícekanálovou samplovací metodu (jako např. ASIO) s nastavenými kanály pro výstup, pak tato hodnota bude rovna 0. Pokud máte takové nastavení a stále vidíte 0 zatímco jiné aplikace hrají zvuk, Vaše nastavení asi není v pořádku.</translation>
    </message>
    <message>
        <source>This shows the peak power in the last frame (20 ms) after all processing. Ideally, this should be -96 dB when you&apos;re not talking. In reality, a sound studio should see -60 dB, and you should hopefully see somewhere around -20 dB. When you are talking, this should rise to somewhere between -5 and -10 dB.&lt;br /&gt;If you are using echo cancellation, and this rises to more than -15 dB when you&apos;re not talking, your setup is not working, and you&apos;ll annoy other players with echoes.</source>
        <translation type="obsolete">Ukazuje hodnotu maximálního výkonu za poslední časový úsek (20ms) po veškerém zpracování. Ideálně tato hodnota bude -96 dB když nemluvíte. V realném světě tato hodnota může být např. -60 dB ve zvukovém studiu jinak běžně asi 20 dB. Pokud mluvíte, tato hodnota stoupne na asi -10 až -5 dB.&lt;br /&gt; Pokud používáte používáte potlačení ozvěny a tato hodnota stoupne na více než -15 dB když nemluvíte, pak Vaše nastavení nefunguje správně a dozajista budete obtěžovat ostatní uživatele ozvěnou.</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="104"/>
        <source>How close the current input level is to ideal</source>
        <translation>Jak blízko je současná hlasitost vstupu k ideální hlasitosti</translation>
    </message>
    <message>
        <source>This shows how close your current input volume is to the ideal. To adjust your microphone level, open whatever program you use to adjust the recording volume, and look at the value here while talking.&lt;br /&gt;&lt;b&gt;Talk loud, as you would when you&apos;re upset over getting fragged by a noob.&lt;/b&gt;&lt;br /&gt;Adjust the volume until this value is close to 100%, but make sure it doesn&apos;t go above. If it does go above, you are likely to get clipping in parts of your speech, which will degrade sound quality</source>
        <translation type="obsolete">Ukazuje, jak blízko je současná úroveň hlasitosti k ideální. Pro nastavení úrovně mikrofonu, otevřete svou oblíbenou aplikaci pro nastavení zvuku, upravte úroveň záznamu a sledujte změny této hodnoty během mluvení.&lt;br /&gt;&lt;b&gt;Mluvte nahlas podobně jako při nadšení nebo rozčilení&lt;/b&gt;&lt;br /&gt; Upravte hlasitost tak, aby byla hodnota blízká 100 %, ale aby nepřesahovala. Přesahování vede k degradaci kvality zvuku (limitací) a možnému cvakání.</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="78"/>
        <source>This shows the peak power in the last frame (20 ms) after all processing. Ideally, this should be -96 dB when you&apos;re not talking. In reality, a sound studio should see -60 dB, and you should hopefully see somewhere around -20 dB. When you are talking, this should rise to somewhere between -5 and -10 dB.&lt;br /&gt;If you are using echo cancellation, and this rises to more than -15 dB when you&apos;re not talking, your setup is not working, and you&apos;ll annoy other users with echoes.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="124"/>
        <source>Signal-To-Noise ratio from the microphone</source>
        <translation>Poměr signál-šum mikrofonu (SNR)</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="127"/>
        <source>This is the Signal-To-Noise Ratio (SNR) of the microphone in the last frame (20 ms). It shows how much clearer the voice is compared to the noise.&lt;br /&gt;If this value is below 1.0, there&apos;s more noise than voice in the signal, and so quality is reduced.&lt;br /&gt;There is no upper limit to this value, but don&apos;t expect to see much above 40-50 without a sound studio.</source>
        <translation>Poměr signál-šum mikrofonu v posledním zvukovém rámci (20 ms). Ukazuje jak je hlas silný v porovnání se šumem.&lt;br /&gt;  Pokud je tato hodnota pod 1.0, je šum silnější než hlas a kvalita nebude dobrá. &lt;br /&gt; Neexistuje žádný horní limit pro tuto hodnotu, ale neočekávejte hodnoty větší než 40 nebo 50, pokud nejste ve zvukovém studiu.</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="144"/>
        <source>Probability of speech</source>
        <translation>Pravděpodobnost, že je mluveno</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="147"/>
        <source>This is the probability that the last frame (20 ms) was speech and not environment noise.&lt;br /&gt;Voice activity transmission depends on this being right. The trick with this is that the middle of a sentence is always detected as speech; the problem is the pauses between words and the start of speech. It&apos;s hard to distinguish a sigh from a word starting with &apos;h&apos;.&lt;br /&gt;If this is in bold font, it means Mumble is currently transmitting (if you&apos;re connected).</source>
        <translation>Toto je pravděpodobnost, že poslední  zvukový rámec (20 ms) byl řeč, ne jenom hluk okolního prostředí.&lt;br /&gt; Přenos při hlasová aktivitě závisí na tom, aby detekce proběhla správně. Trik je v tom, že uprostřed věty je vždy snadné rozpoznat řeč, problémem jsou pauzy mezi slovy a začátek mluvení. Je obtížné rozpoznat povzdech od písmenka &apos;h&apos; na začátku slova.&lt;br /&gt; Pokud tohle je zobrazeno tučně, znamená to, že hlasová aktivita byla rozpoznána.</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="181"/>
        <source>Bitrate of last frame</source>
        <translation>Šířka zvukového pásma </translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="184"/>
        <source>This is the audio bitrate of the last compressed frame (20 ms), and as such will jump up and down as the VBR adjusts the quality. To adjust the peak bitrate, adjust &lt;b&gt;Compression Complexity&lt;/b&gt; in the Settings dialog.</source>
        <translation>Šířka zvukového pásma pro poslední časový úsek (20 ms). Tato hodnota se prúběžně automaticky mění, aby se ideálně přizpůsobila potřebám kvality. V nastavení je možno upravit šířku pásma.</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="207"/>
        <source>Time between last two Push-To-Talk presses</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="262"/>
        <source>Power spectrum of input signal and noise estimate</source>
        <translation>Výkonové spektrum vstupního signálu a odhad šumu</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="265"/>
        <source>This shows the power spectrum of the current input signal (red line) and the current noise estimate (filled blue).&lt;br /&gt;All amplitudes are multiplied by 30 to show the interesting parts (how much more signal than noise is present in each waveband).&lt;br /&gt;This is probably only of interest if you&apos;re trying to fine-tune noise conditions on your microphone. Under good conditions, there should be just a tiny flutter of blue at the bottom. If the blue is more than halfway up on the graph, you have a seriously noisy environment.</source>
        <translation>Ukazuje výkonové spektrum aktuálního vstupu (červená čára) a aktuální odhad šumu (vyplněno modře).&lt;br /&gt;Všechny amplitudy jsou vynásobeny 30x aby byly vidět zajímavé části (jak moc je signál větší než zvuk pro různé frekvence).&lt;br /&gt; To je pravděpodobně zajímavé, pokud zkoušíte vyladit šumové podmínky mikrofonu. V dobrých podmínkách bude dole pouze modrá čára. Pokud dosahuje modrá oblast do poloviny výšky grafu, znamená to, že zvuk je rušen silným šumem.</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="293"/>
        <source>Weights of the echo canceller</source>
        <translation>Intenzita snahy o odstranění ozvěny</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="296"/>
        <source>This shows the weights of the echo canceller, with time increasing downwards and frequency increasing to the right.&lt;br /&gt;Ideally, this should be black, indicating no echo exists at all. More commonly, you&apos;ll have one or more horizontal stripes of bluish color representing time delayed echo. You should be able to see the weights updated in real time.&lt;br /&gt;Please note that as long as you have nothing to echo off, you won&apos;t see much useful data here. Play some music and things should stabilize. &lt;br /&gt;You can choose to view the real or imaginary parts of the frequency-domain weights, or alternately the computed modulus and phase. The most useful of these will likely be modulus, which is the amplitude of the echo, and shows you how much of the outgoing signal is being removed at that time step. The other viewing modes are mostly useful to people who want to tune the echo cancellation algorithms.&lt;br /&gt;Please note: If the entire image fluctuates massively while in modulus mode, the echo canceller fails to find any correlation whatsoever between the two input sources (speakers and microphone). Either you have a very long delay on the echo, or one of the input sources is configured wrong.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioStats.cpp" line="384"/>
        <source>&gt;1000ms</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="14"/>
        <source>Audio Statistics</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="22"/>
        <source>Input Levels</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="38"/>
        <source>This shows the peak power in the last frame (20 ms), and is the same measurement as you would usually find displayed as &quot;input power&quot;. Please disregard this and look at &lt;b&gt;Microphone power&lt;/b&gt; instead, which is much more steady and disregards outliers.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="91"/>
        <source>Signal Analysis</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="97"/>
        <source>Microphone power</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="107"/>
        <source>This shows how close your current input volume is to the ideal. To adjust your microphone level, open whatever program you use to adjust the recording volume, and look at the value here while talking.&lt;br /&gt;&lt;b&gt;Talk loud, as you would when you&apos;re upset over getting fragged by a noob.&lt;/b&gt;&lt;br /&gt;Adjust the volume until this value is close to 100%, but make sure it doesn&apos;t go above. If it does go above, you are likely to get clipping in parts of your speech, which will degrade sound quality.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="117"/>
        <source>Signal-To-Noise ratio</source>
        <translation type="unfinished">Poměr zvuku k šumu</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="162"/>
        <source>Configuration feedback</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="168"/>
        <source>Current audio bitrate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="194"/>
        <source>DoublePush interval</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="217"/>
        <source>Speech Detection</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="224"/>
        <source>Current speech detection chance</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="227"/>
        <source>&lt;b&gt;This shows the current speech detection settings.&lt;/b&gt;&lt;br /&gt;You can change the settings from the Settings dialog or from the Audio Wizard.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="256"/>
        <source>Signal and noise power spectrum</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="281"/>
        <source>Echo Analysis</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>AudioWizard</name>
    <message>
        <location filename="AudioWizard.ui" line="14"/>
        <source>Audio Tuning Wizard</source>
        <translation>Průvodce nastavením zvuku</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="18"/>
        <source>Introduction</source>
        <translation>Úvod</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="21"/>
        <source>Welcome to the Mumble Audio Wizard</source>
        <translation>Vítejte v průvodci pro nastavení zvuku pro aplikaci Mumble</translation>
    </message>
    <message>
        <source>This is the audio tuning wizard for Mumble. This will help you correctly set the input levels of your sound card, and also set the correct parameters for sound processing in Mumble.</source>
        <translation type="obsolete">Toto je průvodce nastavení zvuku pro Mumble. Pomůže vám  správně nastavit vstupní úrovně na zvukové kartě a také vybrat vhodné parametry pro zpracování zvuku v Mumble.</translation>
    </message>
    <message>
        <source>Please be aware that as long as this wizard is active, audio will be looped locally to allow you to listen to it, and no audio will be sent to the server.</source>
        <translation type="obsolete">Prosím všimněte si, že dokud je průvodce aktivní, zvuk bude používán pouze místně, abyste si jej mohli poslechnout - žádné zvuky se nepřenesou na případně připojený server.</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="545"/>
        <source>Finished</source>
        <translation>Dokončeno</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="548"/>
        <source>Enjoy using Mumble</source>
        <translation>Mumble je připraveno k použití</translation>
    </message>
    <message>
        <source>Congratulations. You should now be ready to enjoy a richer sound experience with Mumble.</source>
        <translation type="obsolete">Blahopřejeme. Nyní si můžete plně užít zvukovou komunikaci s Mumble.</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="56"/>
        <source>Device selection</source>
        <translation>Výběr zařízení</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="59"/>
        <source>Selecting the input and output device to use with Mumble.</source>
        <translation>Vyberte vstupní a výstupní zvukové zařízení pro použití v Mumble.</translation>
    </message>
    <message>
        <source>Input device</source>
        <translation type="obsolete">Vstupní zařízení</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="71"/>
        <source>This is the device your microphone is connected to.</source>
        <translation>Zařízení pro zvukový vstup (s mikrofonem).</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="81"/>
        <location filename="AudioWizard.ui" line="153"/>
        <source>System</source>
        <translation>Systém</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="91"/>
        <source>Input method for audio</source>
        <translation>Vstupní metoda pro audio</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="94"/>
        <source>&lt;b&gt;This is the input method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation>&lt;b&gt;To je metoda pro zaznamenávání zvuku.&lt;/b&gt;&lt;br /&gt; Pravděpodobně budete chtít použít ALSA nebo DirectSound.</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="101"/>
        <location filename="AudioWizard.ui" line="173"/>
        <source>Device</source>
        <translation>Zařízení</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="111"/>
        <source>Input device to use</source>
        <translation>Vstupní zařízení</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="114"/>
        <source>&lt;b&gt;Selects which sound card to use for audio input.&lt;/b&gt;</source>
        <translation>&lt;b&gt;Vyberte zvukovou kartu, která bude snímat zvuk.&lt;/b&gt;</translation>
    </message>
    <message>
        <source>Output device</source>
        <translation type="obsolete">Výstupní zařízení</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="143"/>
        <source>This is the device your speakers or headphones are connected to.</source>
        <translation>Zařízení, ke kterému jsou připojeny sluchátka nebo mikrofon.</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="163"/>
        <source>Output method for audio</source>
        <translation>Výstupní metoda pro zvuk</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="166"/>
        <source>&lt;b&gt;This is the Output method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation>&lt;b&gt;To je metoda pro přehrávání zvuku.&lt;/b&gt;&lt;br /&gt; Pravděpodobně budete chtít použít ALSA nebo DirectSound.</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="183"/>
        <source>Output device to use</source>
        <translation>Výstupní zařízení</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="186"/>
        <source>&lt;b&gt;Selects which sound card to use for audio Output.&lt;/b&gt;</source>
        <translation>&lt;b&gt;Vyberte zvukovou kartu, která bude přehrávat zvuk.&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="306"/>
        <source>Volume tuning</source>
        <translation>Vyladění hlasitosti</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="309"/>
        <source>Tuning microphone hardware volume to optimal settings.</source>
        <translation>Nastavení hlasitosti mikrofonu pro optimální hlasitost.</translation>
    </message>
    <message>
        <source>Open your sound control panel and go to the recording settings. Make sure the microphone is selected as active input with maximum recording volume. If there&apos;s an option to enable a &quot;Microphone boost&quot; make sure it&apos;s checked.</source>
        <translation type="obsolete">Otevřete ovládací panel nastavení zvuku a jděte do nastavení záznamu. Ujistěte se, že mikrofon je vybrán jako aktivní vstup a má maximální hlasitost pro záznam. Pokud je k dispozici volba &quot;Microphone boost&quot;, určitě ji zapněte.</translation>
    </message>
    <message>
        <source>Speak loudly, as when you are annoyed or excited. Decrease the volume in the sound control panel until the bar below stays as high as possible in the blue and green but &lt;b&gt;not&lt;/b&gt; the red zone while you speak.</source>
        <translation type="obsolete">Mluvte nahlas, jako kdybyste byl podrážděn nebo nadšen. Během mluvení nastavte hlasitost v ovladacim panelu zvuku tak, aby monitorovaný zvuk byl co možná nejvýše v modré a zelené oblastí, ale aby &lt;b&gt;nezasahoval&lt;/b&gt; do oblasti rudé.</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="341"/>
        <source>Now talk softly, as you would when talking late at night and you don&apos;t want to disturb anyone. Adjust the slider below so that the bar moves into green when you talk, but stays blue while you&apos;re silent.</source>
        <translation>Nyní se uvolněte a mluvte potichu, jako kdyby byla pozdní noc a nechtěli jste nikoho rušit ve spánku. Nastavte táhlo pod tímto textem tak, aby monitorovaný zvuk byl v zelené oblasti při mluvení, ale aby bezpečně zůstal v modré oblasti, když jste potichu.</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="381"/>
        <source>Voice Activity Detection</source>
        <translation>Detekce hlasové aktivity</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="384"/>
        <source>Letting Mumble figure out when you&apos;re talking and when you&apos;re silent.</source>
        <translation>Mumble umí rozpoznávat mluvení a ticho.</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="390"/>
        <source>This will help Mumble figure out when you are talking. The first step is selecting which data value to use.</source>
        <translation>Toto pomůže aplikaci Mumble rozpoznat, kdy mluvíte. První krok je výběr, podle které hodnoty se má provádět rozpoznání.</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="421"/>
        <source>Raw amplitude from input</source>
        <translation>Hlasitost vstupu</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="414"/>
        <source>Signal-To-Noise ratio</source>
        <translation>Poměr zvuku k šumu</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="444"/>
        <source>Next you need to adjust the following slider. The first few utterances you say should end up in the green area (definitive speech). While talking, you should stay inside the yellow (might be speech) and when you&apos;re not talking, everything should be in the red (definitively not speech).</source>
        <oldsource>Next you need to adjust the following two sliders. The first few utterances you say should end up in the green area (definitive speech). While talking, you should stay inside the yellow (might be speech) and when you&apos;re not talking, everything should be in the red (definitively not speech).</oldsource>
        <translation type="unfinished">Nyní je potřeba nastavit následující dvě hodnoty. Prvních několik vašich slabik by mělo skončit v zelené oblasti (rozpoznáno jako řeč). Pokud mluvíte, měl by monitor zůstávat ve žluté (rozpoznáno jako pravděpopdobná řeč). Když nemluvíte, vše by mělo být v červené oblasti (to dozajista není řeč).</translation>
    </message>
    <message>
        <source>Last, you need to adjust the grace time. Mumble may detect this much non-speech and still keep transmitting before it breaks off. This allows you to catch your breath while speaking.</source>
        <translation type="obsolete">Nakonec je potřeba nastavit čas prodlevy, kdy je po skončení řeči stále ješte mikrofon zapnut a můžete tak během řeči udělat malou pauzu, aniž by byl přenos ukončen.</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="223"/>
        <source>Device tuning</source>
        <translation>Vyladění zařízení</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="226"/>
        <source>Changing hardware output delays to their minimum value.</source>
        <translation>Změní výstupní parametry zvuku tak, aby prodleva v komunikaci mohla být minimální.</translation>
    </message>
    <message>
        <source>To keep latency to an absolute minium, it&apos;s important to buffer as little audio as possible on the soundcard. However, many soundcards report that they require a much smaller buffer than what they can actually work with, so the only way to set this value is to try and fail.</source>
        <translation type="obsolete">Aby zpoždění při komunikaci mohlo být minimalizováno, je důležité nastavit výstupní vyrovnávací paměť co nejmenší. Mnohé zvukové karty hlásí, že neumějí používat malé velikosti vyrovnávací paměti, i když to jinak technicky je možné, takže jediná možnost, jak zjistit nejmenší vhodnou hodnotu je pokud a omyl.</translation>
    </message>
    <message>
        <source>You should hear a multitone chord. Change the slider below to the lowest value which gives &lt;b&gt;no&lt;/b&gt; interruptions or jitter in the sound.</source>
        <translation type="obsolete">Nyní byste měli slyšet zvukový akord. Změňte nastavení pod tímto textem na nejmenší hodnotu, která bude fungovat &lt;b&gt;bez&lt;/b&gt; přerušování nebo cvakání ve zvuku.</translation>
    </message>
    <message>
        <source>Amount of data to buffer.</source>
        <translation type="obsolete">Velikost zásobníku pro výstup.</translation>
    </message>
    <message>
        <source>This sets the amount of data to prebuffer in the output buffer. Experiment with different values and set it to the lowest which doesn&apos;t cause rapid jitter in the sound.</source>
        <translation type="obsolete">To nastaví množství dat, která budou v výstupním zásobníku aplikace, než se pošlou na zvukový výstup. Experimentujte s rozdílnými hodnotami a nastavte si nejmenší hodnotu, která nezpůsobí přerušování nebo cvakání ve zvuku.</translation>
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
        <location filename="AudioWizard.ui" line="127"/>
        <source>Use echo cancellation</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="124"/>
        <source>This enables echo cancellation of outgoing audio, which helps both on speakers and on headsets.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="199"/>
        <source>Enable positional audio</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="193"/>
        <source>Allows positioning of sound</source>
        <oldsource>Allows positioning of sound.</oldsource>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="493"/>
        <source>Adjusting attenuation of positional audio.</source>
        <translation type="unfinished"></translation>
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
        <location filename="AudioWizard.ui" line="521"/>
        <source>Use headphones</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="518"/>
        <source>This ignores the OS speaker configuration and configures the positioning for headphones instead.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="570"/>
        <source>Submit anonymous statistics to the Mumble project</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="402"/>
        <source>Push To Talk:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>BanEditor</name>
    <message>
        <location filename="BanEditor.ui" line="14"/>
        <source>Mumble - Edit Bans</source>
        <translation>Mumble - Upravit bany (zákazy)</translation>
    </message>
    <message>
        <source>0.0.0.0</source>
        <translation type="obsolete">0.0.0.0</translation>
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
        <translation>Přid&amp;at</translation>
    </message>
    <message>
        <location filename="BanEditor.ui" line="152"/>
        <source>&amp;Update</source>
        <translation>&amp;Obnovit</translation>
    </message>
    <message>
        <location filename="BanEditor.ui" line="159"/>
        <source>&amp;Remove</source>
        <translation>Odst&amp;ranit</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Ok</translation>
    </message>
    <message>
        <source>Accept changes</source>
        <translation type="obsolete">Potvrdit změny</translation>
    </message>
    <message>
        <source>This button will accept current groups/ACLs and send them to the server. Note that if you mistakenly remove write permission from yourself, the server will add it.</source>
        <translation type="obsolete">Tímto tlačítkem přijmete nastavení skupin/ACL a pošlete je na server. Pokud si omylem odstraníte pravomoci sami pro sebe, server je opětovně přidá.</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Zrušit</translation>
    </message>
    <message>
        <source>Reject changes</source>
        <translation type="obsolete">Odmítnout změny</translation>
    </message>
    <message>
        <source>This button will cancels all changes and closes the dialog without updating the ACLs or groups on the server.</source>
        <translation type="obsolete">Toto tlačítko zruší veškeré právě provedené změny a uzavře dialog bez úpravy skupin a ACL na serveru.</translation>
    </message>
    <message>
        <location filename="BanEditor.ui" line="29"/>
        <source>&amp;Address</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="BanEditor.ui" line="39"/>
        <source>&amp;Mask</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>CertView</name>
    <message>
        <location filename="Cert.cpp" line="50"/>
        <source>Name</source>
        <translation type="unfinished">Jméno</translation>
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
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">Jméno</translation>
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
        <source>W</source>
        <translation type="obsolete">Z</translation>
    </message>
    <message>
        <source>T</source>
        <translation type="obsolete">P</translation>
    </message>
    <message>
        <source>E</source>
        <translation type="obsolete">V</translation>
    </message>
    <message>
        <source>S</source>
        <translation type="obsolete">M</translation>
    </message>
    <message>
        <source>A</source>
        <translation type="obsolete">A</translation>
    </message>
    <message>
        <source>M</source>
        <translation type="obsolete">U</translation>
    </message>
    <message>
        <source>K</source>
        <translation type="obsolete">V</translation>
    </message>
    <message>
        <source>C</source>
        <translation type="obsolete">T</translation>
    </message>
    <message>
        <source>L</source>
        <translation type="obsolete">N</translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="214"/>
        <source>None</source>
        <translation>Žádné</translation>
    </message>
    <message>
        <source>Write</source>
        <translation type="obsolete">Zápis</translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="218"/>
        <source>Traverse</source>
        <translation>Průchod</translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="220"/>
        <source>Enter</source>
        <translation>Vstup</translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="222"/>
        <source>Speak</source>
        <translation>Mluvení</translation>
    </message>
    <message>
        <source>AltSpeak</source>
        <translation type="obsolete">Alt_mluvení</translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="226"/>
        <source>Mute/Deafen</source>
        <translation>Zakázat mluvení/poslouchání</translation>
    </message>
    <message>
        <source>Move/Kick</source>
        <translation type="obsolete">Přesunout/Vykopnout</translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="230"/>
        <source>Make channel</source>
        <translation>Vytvoření kanálu</translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="232"/>
        <source>Make temporary</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="234"/>
        <source>Link channel</source>
        <translation>Propojení kanálů</translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="147"/>
        <source>This represents no privileges.</source>
        <translation>Toto představuje absenci jakýchkoliv privilegií.</translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="149"/>
        <source>This represents total access to the channel, including the ability to change group and ACL information. This privilege implies all other privileges.</source>
        <translation>Reprezentuje totální právo ke kanálu, včetně schopnosti změnit skupiny a ACL. Mít toto privilegium automaticky znamená mít všechna ostatní práva.</translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="152"/>
        <source>This represents the permission to traverse the channel. If a user is denied this privilege, he will be unable to access this channel and any sub-channels in any way, regardless of other permissions in the sub-channels.</source>
        <translation>Reprezentuje povolení procházet kanálem. Pokud je procházení uživateli odepřeno, nebude schopen přijít do kanálu či jeho podkanálů (i kdyby tam měl průchod povolen).</translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="156"/>
        <source>This represents the permission to join the channel. If you have a hierarchical channel structure, you might want to give everyone Traverse, but restrict Enter in the root of your hierarchy.</source>
        <translation>Povolení vejít do kanálu. V hierarchicky uspořádaných kanálech je často výhodné dám všem právo průchodu (traverse), ale omezit právo vstupu (enter) v kořeni hierarchie.</translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="159"/>
        <source>This represents the permission to speak in a channel. Users without this privilege will be suppressed by the server (seen as muted), and will be unable to speak until they are unmuted by someone with the appropriate privileges.</source>
        <translation>Representuje oprávnění mluvit v kanálu. Uživatelům bez tohoto privilegia bude serverem zakázáno mluvit a nebudou mít možnost mluvit, dokud jim nějaký administrátor tuto schopnost neumožní.</translation>
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
        <translation type="unfinished"></translation>
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
        <translation type="obsolete">Reprezentuje oprávnění  mluvit v kanálu &apos;alternativně&apos;. Na rozdíl od normálního mluvení se alternativní mluvení provádí stlačením tlačítka pro Alt_mluvení. To může být použito pro vysílání přes celou hierarchii kanálů aniž by bylo nutno kanály propojovat.</translation>
    </message>
    <message>
        <source>This represents the permission to mute and deafen other players. Once muted, a player will stay muted until he is unmuted by another privileged player or reconnects to the server.</source>
        <translation type="obsolete">Reprezentuje oprávnění zakázat mluvení nebo poslouchání ostatním uživatelům. Uživatelé, kterým bylo zakázáno mluvit nebo poslouchat, nedostanou zpět tuto schopnost, pokud jim opět nebude vrácena zpět privilegovaným uživatelem, nebo dokud se neodpojí a znova nepřipojí k serveru.</translation>
    </message>
    <message>
        <source>This represents the permission to move a player to another channel or kick him from the server. To actually move the player, either the moving player must have Move/Kick privileges in the destination channel, or the player must normally be allowed to enter the channel. Players with this privilege can move players into channels the target player normally wouldn&apos;t have permission to enter.</source>
        <translation type="obsolete">Reprezentuje oprávnění k přesunu uživatele do jiného kanálu nebo oprávnění jej zcela vykopnout ze serveru. Pro úspěšný přesun musí mít přesunující uživatel právo přesunu do cílového kanálu, nebo tam musí být povolen pro přesunovaného bežný vstup. Uživatelé s tímto oprávněním tedy mohou přesunovat do neveřejných  kanálů  uživatele, kteří by se tam jinak nedostali.</translation>
    </message>
    <message>
        <source>This represents the permission to make sub-channels. The player making the sub-channel will be added to the admin group of the sub-channel.</source>
        <translation type="obsolete">Reprezentuje oprávnění vytvářet v kanále podkanály. Uživatel vytvářející kanál přidán do skupiny administrátorů tohoto kanálu.</translation>
    </message>
    <message>
        <source>This represents the permission to link channels. Players in linked channels hear each other, as long as the speaking player has the &lt;i&gt;speak&lt;/i&gt; privilege in the channel of the listener. You need the link privilege in both channels to create a link, but just in either channel to remove it.</source>
        <translation type="obsolete">Reprezentuje oprávnění propojovat kanály. Uživatelé ve spojených kanálech slyší jeden druhého, pokud mluvící uživatel má oprávnění &lt;i&gt;mluvit&lt;/i&gt; v kanále, kde je posluchač. Pro vytvoření zvukového propojení je nutno mít toto oprávnění v obou kanálech, pří rušení stačí oprávnění v jednom z nich.</translation>
    </message>
    <message>
        <location filename="../ACL.cpp" line="216"/>
        <source>Write ACL</source>
        <translation type="unfinished"></translation>
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
    <name>ClientUser</name>
    <message>
        <location filename="ClientUser.cpp" line="99"/>
        <source>Friend</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ClientUser.cpp" line="101"/>
        <source>Authenticated</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ClientUser.cpp" line="103"/>
        <source>Muted (server)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ClientUser.cpp" line="105"/>
        <source>Deafened (server)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ClientUser.cpp" line="107"/>
        <source>Local Mute</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ClientUser.cpp" line="109"/>
        <source>Muted (self)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ClientUser.cpp" line="111"/>
        <source>Deafened (self)</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ConfigDialog</name>
    <message>
        <location filename="ConfigDialog.ui" line="14"/>
        <source>Mumble Configuration</source>
        <translation>Konfigurace Mumble</translation>
    </message>
    <message>
        <location filename="ConfigDialog.ui" line="54"/>
        <location filename="ConfigDialogDelegate.mm" line="135"/>
        <source>Advanced</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="51"/>
        <location filename="ConfigDialog.mm" line="73"/>
        <source>Accept changes</source>
        <translation>Potvrdit změny</translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="52"/>
        <location filename="ConfigDialog.mm" line="74"/>
        <source>This button will accept current settings and return to the application.&lt;br /&gt;The settings will be stored to disk when you leave the application.</source>
        <translation>Toto tlačítko potvrdí současné nastavení a navrátí se do aplikace.&lt;br /&gt;Nastavení bude uloženo na disku při ukončení aplikace.</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Zrušit</translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="56"/>
        <location filename="ConfigDialog.mm" line="78"/>
        <source>Reject changes</source>
        <translation>Odmítnout nové změny</translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="57"/>
        <location filename="ConfigDialog.mm" line="79"/>
        <source>This button will reject all changes and return to the application.&lt;br /&gt;The settings will be reset to the previous positions.</source>
        <translation>Tímto tlačítkem můžete odmítnout nastavované změny a vrátit se do aplikace.&lt;br /&gt;Nastavení nebude změněno.</translation>
    </message>
    <message>
        <source>&amp;Apply</source>
        <translation type="obsolete">&amp;Použít</translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="61"/>
        <location filename="ConfigDialog.mm" line="83"/>
        <source>Apply changes</source>
        <translation>Potvrdit změny</translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="62"/>
        <location filename="ConfigDialog.mm" line="84"/>
        <source>This button will immediately apply all changes.</source>
        <translation>Toto tlačítko okamžitě provede všechny vybrané změny.</translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="65"/>
        <location filename="ConfigDialog.mm" line="87"/>
        <source>Undo changes for current page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="66"/>
        <location filename="ConfigDialog.mm" line="88"/>
        <source>This button will revert any changes done on the current page to the most recent applied settings.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="69"/>
        <location filename="ConfigDialog.mm" line="91"/>
        <source>Restore defaults for current page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="70"/>
        <location filename="ConfigDialog.mm" line="92"/>
        <source>This button will restore the settings for the current page only to their defaults. Other pages will not be changed.&lt;br /&gt;To restore all settings to their defaults, you will have to use this button on every page.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ConnectDialog</name>
    <message>
        <source>&amp;Custom Servers</source>
        <translation type="obsolete">&amp;Moje servery</translation>
    </message>
    <message>
        <source>Server &amp;Browser</source>
        <translation type="obsolete">Výběr &amp;veřejných serverů</translation>
    </message>
    <message>
        <source>&amp;Name</source>
        <translation type="obsolete">&amp;Jméno</translation>
    </message>
    <message>
        <source>A&amp;ddress</source>
        <translation type="obsolete">A&amp;dresa</translation>
    </message>
    <message>
        <source>&amp;Port</source>
        <translation type="obsolete">&amp;Port</translation>
    </message>
    <message>
        <source>&amp;Username</source>
        <translation type="obsolete">&amp;Uživatelské jméno</translation>
    </message>
    <message>
        <source>&amp;Password</source>
        <translation type="obsolete">&amp;Heslo</translation>
    </message>
    <message>
        <source>&amp;Connect</source>
        <translation type="obsolete">&amp;Připojit</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Zrušit</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation type="obsolete">Přid&amp;at</translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation type="obsolete">Odst&amp;ranit</translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="obsolete">Jméno</translation>
    </message>
    <message>
        <source>Address</source>
        <translation type="obsolete">Adresa</translation>
    </message>
    <message>
        <source>URL</source>
        <translation type="obsolete">URL</translation>
    </message>
    <message>
        <source>C&amp;opy to custom</source>
        <translation type="obsolete">Zk&amp;opírovat do seznamu mých serverů</translation>
    </message>
    <message>
        <source>&amp;View Webpage</source>
        <translation type="obsolete">Otevřít &amp;Webovou stránku</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="837"/>
        <source>Connecting to %1</source>
        <translation>Připojování k %1</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="385"/>
        <location filename="ConnectDialog.cpp" line="837"/>
        <source>Enter username</source>
        <translation>Zadejte jmého uživatele</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="385"/>
        <source>Adding host %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui" line="48"/>
        <location filename="ConnectDialog.cpp" line="448"/>
        <source>Servername</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="449"/>
        <source>Hostname</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="452"/>
        <source>Bonjour name</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="455"/>
        <source>Port</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">%1 ms</translation>
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
        <location filename="ConnectDialog.ui" line="58"/>
        <location filename="ConnectDialog.cpp" line="470"/>
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
        <location filename="ConnectDialog.ui" line="83"/>
        <location filename="ConnectDialog.cpp" line="716"/>
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
        <translation>Mumble</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="1401"/>
        <source>Failed to fetch server list</source>
        <translation>Selhalo stažení seznamu dostupných serverů</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui" line="14"/>
        <source>Mumble Server Connect</source>
        <translation type="unfinished"></translation>
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
        <source>Add</source>
        <translation type="obsolete">Přidat</translation>
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
        <translation type="unfinished">A&amp;dresa</translation>
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
        <translation type="unfinished">&amp;Uživatelské jméno</translation>
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
        <translation type="unfinished"></translation>
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
        <source>Mumble</source>
        <translation type="obsolete">Mumble</translation>
    </message>
    <message>
        <source>Opening chosen DirectSound Input failed. No microphone capture will be done.</source>
        <translation type="obsolete">Otevírání vybraného vstupu DirectSound selhalo. Nebude prováděn záznam mikrofonu.</translation>
    </message>
    <message>
        <location filename="DirectSound.cpp" line="558"/>
        <source>Opening chosen DirectSound Input failed. Default device will be used.</source>
        <translation>Otevírání vybraného vstupu DirectSound  selhalo. Bude použito výchozí zařízení.</translation>
    </message>
    <message>
        <location filename="DirectSound.cpp" line="174"/>
        <source>Default DirectSound Voice Input</source>
        <translation type="unfinished">Výchozí DirectSound hlasový vstup</translation>
    </message>
    <message>
        <location filename="DirectSound.cpp" line="637"/>
        <source>Opening chosen DirectSound Input device failed. No microphone capture will be done.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="DirectSound.cpp" line="632"/>
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
        <location filename="DirectSound.cpp" line="464"/>
        <source>Opening chosen DirectSound Output failed. No audio will be heard.</source>
        <translation>Otevírání vybraného výstupu DirectSound selhalo. Nebude slyšet žádný zvukový výstup.</translation>
    </message>
    <message>
        <location filename="DirectSound.cpp" line="382"/>
        <source>Opening chosen DirectSound Output failed. Default device will be used.</source>
        <translation>Otevírání vybraného výstupu DirectSound  selhalo. Bude použito výchozí zařízení.</translation>
    </message>
    <message>
        <location filename="DirectSound.cpp" line="134"/>
        <source>Default DirectSound Voice Output</source>
        <translation type="unfinished">Výchozí DirectSound hlasový výstup</translation>
    </message>
    <message>
        <location filename="DirectSound.cpp" line="460"/>
        <source>Lost DirectSound output device.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>DXConfigDialog</name>
    <message>
        <source>Default DirectSound Voice Input</source>
        <translation type="obsolete">Výchozí DirectSound hlasový vstup</translation>
    </message>
    <message>
        <source>Default DirectSound Voice Output</source>
        <translation type="obsolete">Výchozí DirectSound hlasový výstup</translation>
    </message>
    <message>
        <source>Device selection</source>
        <translation type="obsolete">Výběr zařízení</translation>
    </message>
    <message>
        <source>Device to use for microphone</source>
        <translation type="obsolete">Zařízení, na kterém je mikrofon</translation>
    </message>
    <message>
        <source>This sets the input device to use, which is where you have connected the microphone.</source>
        <translation type="obsolete">Tohle zařízení bude použito pro zvukový vstup, měl by tam být připojen mikrofon.</translation>
    </message>
    <message>
        <source>Input</source>
        <translation type="obsolete">Vstup</translation>
    </message>
    <message>
        <source>Device to use for speakers/headphones</source>
        <translation type="obsolete">Zařízení pro zvukový výstup (bedničky nebo sluchátka)</translation>
    </message>
    <message>
        <source>This sets the output device to use, which is where you have connected your speakers or your headset.</source>
        <translation type="obsolete">Tohle zařízení bude použito pro zvukový výstup, měly by tam být připojeny bedničky nebo sluchátka.</translation>
    </message>
    <message>
        <source>Output</source>
        <translation type="obsolete">Výstup</translation>
    </message>
    <message>
        <source>Output Options</source>
        <translation type="obsolete">Výstupní volby</translation>
    </message>
    <message>
        <source>Output Delay</source>
        <translation type="obsolete">Výstupní opoždění</translation>
    </message>
    <message>
        <source>Amount of data to buffer for DirectSound</source>
        <translation type="obsolete">Velikost datového zásobníku pro DirectSound</translation>
    </message>
    <message>
        <source>This sets the amount of data to prebuffer in the DirectSound buffer. Experiment with different values and set it to the lowest which doesn&apos;t cause rapid jitter in the sound.</source>
        <translation type="obsolete">Nastaví velikost dat pro výstupní zásobník. Zkoušejte různé hodnoty a nastavte to na nejmenší hodnotu, která nezpůsobuje rychlé cvakání ve zvuku.</translation>
    </message>
    <message>
        <source>%1ms</source>
        <translation type="obsolete">%1ms</translation>
    </message>
</context>
<context>
    <name>Database</name>
    <message>
        <location filename="Database.cpp" line="83"/>
        <location filename="Database.cpp" line="90"/>
        <source>Mumble</source>
        <translation>Mumble</translation>
    </message>
    <message>
        <location filename="Database.cpp" line="83"/>
        <source>Mumble failed to initialize a database in any
of the possible locations.</source>
        <translation>Mumble nedokazalo inicializovat databázi
v žádném z možných míst.</translation>
    </message>
    <message>
        <location filename="Database.cpp" line="90"/>
        <source>The database &apos;%1&apos; is read-only. Mumble can not store server settings (ie. SSL certificates) until you fix this problem.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>DirectInputKeyWidget</name>
    <message>
        <source>Press Shortcut</source>
        <translation type="obsolete">Stiskněte klávesovou zkratku</translation>
    </message>
</context>
<context>
    <name>GlobalShortcut</name>
    <message>
        <location filename="GlobalShortcut.ui" line="17"/>
        <source>Shortcuts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="GlobalShortcut.ui" line="65"/>
        <source>List of configured shortcuts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="GlobalShortcut.ui" line="99"/>
        <source>Function</source>
        <translation type="unfinished">Funkce</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.ui" line="104"/>
        <source>Data</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="GlobalShortcut.ui" line="109"/>
        <source>Shortcut</source>
        <translation type="unfinished">Zkratka</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.ui" line="114"/>
        <source>Suppress</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="GlobalShortcut.ui" line="23"/>
        <source>Add new shortcut</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="GlobalShortcut.ui" line="26"/>
        <source>This will add a new global shortcut</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="GlobalShortcut.ui" line="29"/>
        <source>&amp;Add</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="GlobalShortcut.ui" line="39"/>
        <source>Remove selected shortcut</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="GlobalShortcut.ui" line="42"/>
        <source>This will permanently remove a selected shortcut.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="GlobalShortcut.ui" line="45"/>
        <source>&amp;Remove</source>
        <translation type="unfinished">Odst&amp;ranit</translation>
    </message>
</context>
<context>
    <name>GlobalShortcutConfig</name>
    <message>
        <location filename="GlobalShortcut.cpp" line="654"/>
        <source>Shortcuts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Function</source>
        <translation type="obsolete">Funkce</translation>
    </message>
    <message>
        <source>Shortcut</source>
        <translation type="obsolete">Zkratka</translation>
    </message>
    <message>
        <source>&lt;b&gt;This is the global shortcut bound to %1&lt;/b&gt;&lt;br /&gt;Click this field and then the desired key/button combo to rebind. Double-click to clear.</source>
        <translation type="obsolete">&lt;b&gt;Toto je globální klávesová zkratka přiřazena k &apos;%1&apos;&lt;/br&gt;Klikněte na toto políčko a pak zmáčkněte klávesu/tlačítko pro změnu. Dvojité kliknutí zruší přiřazení.</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.cpp" line="692"/>
        <source>Shortcut button combination.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="GlobalShortcut.cpp" line="693"/>
        <source>&lt;b&gt;This is the global shortcut key combination.&lt;/b&gt;&lt;br /&gt;Double-click this field and then the desired key/button combo to rebind.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="GlobalShortcut.cpp" line="697"/>
        <source>Suppress keys from other applications</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="GlobalShortcut.cpp" line="698"/>
        <source>&lt;b&gt;This hides the button presses from other applications.&lt;/b&gt;&lt;br /&gt;Enabling this will hide the button (or the last button of a multi-button combo) from other applications. Note that not all buttons can be suppressed.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>GlobalShortcutMacInit</name>
    <message>
        <location filename="GlobalShortcut_macx.cpp" line="60"/>
        <source>Mumble has detected that it is unable to receive Global Shortcut events when it is in the background.&lt;br /&gt;&lt;br /&gt;This is because the Universal Access feature called &apos;Enable access for assistive devices&apos; is currently disabled.&lt;br /&gt;&lt;br /&gt;Please &lt;a href=&quot; &quot;&gt;enable this setting&lt;/a&gt; and continue when done.</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">Přidat</translation>
    </message>
    <message>
        <location filename="GlobalShortcutTarget.ui" line="129"/>
        <source>Remove</source>
        <translation type="unfinished">Odstranit</translation>
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
    <name>GlobalShortcutWinConfig</name>
    <message>
        <source>Shortcuts</source>
        <translation type="obsolete">Kláv. zkratky</translation>
    </message>
    <message>
        <source>Function</source>
        <translation type="obsolete">Funkce</translation>
    </message>
    <message>
        <source>Shortcut</source>
        <translation type="obsolete">Zkratka</translation>
    </message>
    <message>
        <source>Shortcut bound to %1.</source>
        <translation type="obsolete">Klávesová zkratka přiřazena k %1.</translation>
    </message>
    <message>
        <source>&lt;b&gt;This is the global shortcut bound to %1&lt;/b&gt;&lt;br /&gt;Click this field and then the desired key/button combo to rebind. Double-click to clear.</source>
        <translation type="obsolete">&lt;b&gt;Toto je globální klávesová zkratka přiřazena k &apos;%1&apos;&lt;/br&gt;Klikněte na toto políčko a pak zmáčkněte klávesu/tlačítko pro změnu. Dvojité kliknutí zruší přiřazení.</translation>
    </message>
</context>
<context>
    <name>GlobalShortcutX</name>
    <message>
        <location filename="GlobalShortcut_unix.cpp" line="325"/>
        <source>Mouse %1</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>GlobalShortcutXConfig</name>
    <message>
        <source>Shortcuts</source>
        <translation type="obsolete">Klávesové zkratky</translation>
    </message>
    <message>
        <source>Function</source>
        <translation type="obsolete">Funkce</translation>
    </message>
    <message>
        <source>Shortcut</source>
        <translation type="obsolete">Zkratka</translation>
    </message>
    <message>
        <source>Shortcut bound to %1.</source>
        <translation type="obsolete">Klávesová zkratka  přiřazena k %1.</translation>
    </message>
    <message>
        <source>&lt;b&gt;This is the global shortcut bound to %1&lt;/b&gt;&lt;br /&gt;Click this field and then the desired key combo to rebind. Double-click to clear.</source>
        <translation type="obsolete">&lt;b&gt;Tohle je klávesová zkratka přiřazena k %1&lt;/b&gt;&lt;br /&gt;Klikněte na tohle pole a pak zvolte klávesovou zkratku pro přenastavení.</translation>
    </message>
</context>
<context>
    <name>LCD</name>
    <message>
        <location filename="LCD.cpp" line="280"/>
        <source>Not connected</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>LCDConfig</name>
    <message>
        <location filename="LCD.cpp" line="120"/>
        <source>Enable this device</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LCD.cpp" line="129"/>
        <source>LCD</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LCD.ui" line="14"/>
        <source>Form</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LCD.ui" line="34"/>
        <source>Devices</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LCD.ui" line="50"/>
        <source>Name</source>
        <translation type="unfinished">Jméno</translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LCD.ui" line="60"/>
        <source>Enabled</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LCD.ui" line="77"/>
        <source>Views</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LCD.ui" line="83"/>
        <source>Minimum Column Width</source>
        <translation type="unfinished"></translation>
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
        <location filename="LCD.ui" line="121"/>
        <source>Splitter Width</source>
        <translation type="unfinished"></translation>
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
        <translation>Critical</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="218"/>
        <source>Warning</source>
        <translation>Warning</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="219"/>
        <source>Information</source>
        <translation>Information</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="220"/>
        <source>Server Connected</source>
        <translation>Server připojen</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="221"/>
        <source>Server Disconnected</source>
        <translation>Server odpojen</translation>
    </message>
    <message>
        <source>Player Joined Server</source>
        <translation type="obsolete">Uživatel se připojil k serveru</translation>
    </message>
    <message>
        <source>Player Left Server</source>
        <translation type="obsolete">Uživatel opustil server</translation>
    </message>
    <message>
        <source>Player kicked (you or by you)</source>
        <translation type="obsolete">Uživatel byl vykopnut</translation>
    </message>
    <message>
        <source>Player kicked</source>
        <translation type="obsolete">Uživatel vykopnut</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="226"/>
        <source>You self-muted/deafened</source>
        <translation>Zakázal jste si možnost hovoru/poslechu</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="227"/>
        <source>Other self-muted/deafened</source>
        <translation>Uživatel si vypnul mluvení/poslech</translation>
    </message>
    <message>
        <source>Player muted (you)</source>
        <translation type="obsolete">Bylo zakázáno mluvení</translation>
    </message>
    <message>
        <source>Player muted (by you)</source>
        <translation type="obsolete">Uživateli bylo Vámi zakázano mluvit</translation>
    </message>
    <message>
        <source>Player muted (other)</source>
        <translation type="obsolete">Uživatelovi bylo zakázáno mluvit</translation>
    </message>
    <message>
        <source>Player Joined Channel</source>
        <translation type="obsolete">Uživatel vstoupil do kanálu</translation>
    </message>
    <message>
        <source>Player Left Channel</source>
        <translation type="obsolete">Uživatel opustil kanál</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="233"/>
        <source>Permission Denied</source>
        <translation>K provedení akce chybí potřebné oprávnění</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="234"/>
        <source>Text Message</source>
        <translation>Textová zpráva</translation>
    </message>
    <message>
        <source>[%2] %1</source>
        <translation type="obsolete">[%2] %1</translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Log.cpp" line="76"/>
        <source>Toggle pop-up notifications for %1 events</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Log.cpp" line="77"/>
        <source>Toggle Text-To-Speech for %1 events</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Log.cpp" line="78"/>
        <source>Click here to toggle sound notification for %1 events</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Log.cpp" line="79"/>
        <source>Path to sound file used for sound notifications in the case of %1 events&lt;br /&gt;Single click to play&lt;br /&gt;Double-click to change</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Log.cpp" line="81"/>
        <source>Click here to toggle console output for %1 events.&lt;br /&gt;If checked, this option makes Mumble output all %1 events in its message log.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Log.cpp" line="82"/>
        <source>Click here to toggle pop-up notifications for %1 events.&lt;br /&gt;If checked, a notification pop-up will be created by Mumble for every %1 event.</source>
        <oldsource>Click here to toggle pop-up notifications for %1 events.&lt;br /&gt;If checked, a notification pop-up will be created by mumble for every %1 event.</oldsource>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Log.cpp" line="85"/>
        <source>Path to sound file used for sound notifications in the case of %1 events.&lt;br /&gt;Single click to play&lt;br /&gt;Double-click to change&lt;br /&gt;Ensure that sound notifications for these events are enabled or this field will not have any effect.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Log.cpp" line="83"/>
        <source>Click here to toggle Text-To-Speech for %1 events.&lt;br /&gt;If checked, Mumble uses Text-To-Speech to read %1 events out loud to you. Text-To-Speech is also able to read the contents of the event which is not true for sound files. Text-To-Speech and sound files cannot be used at the same time.</source>
        <oldsource>Click here to toggle sound notification for %1 events.&lt;br /&gt;If checked, Mumble uses a soundfile predefined by you to indicate %1 events. Soundfiles and Text-To-Speech cannot be used at the same time.</oldsource>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Log.cpp" line="84"/>
        <source>Click here to toggle sound notification for %1 events.&lt;br /&gt;If checked, Mumble uses a sound file predefined by you to indicate %1 events. Sound files and Text-To-Speech cannot be used at the same time.</source>
        <oldsource>Path to soundfile used for sound notifications in the case of %1 events.&lt;br /&gt;Single click to play&lt;br /&gt;Doubleclick to change&lt;br /&gt;Be sure that sound notifications for these events are enabled or this field will not have any effect.</oldsource>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Log.ui" line="14"/>
        <source>Messages</source>
        <translation>Zprávy</translation>
    </message>
    <message>
        <location filename="Log.ui" line="41"/>
        <source>Console</source>
        <translation>Konsola</translation>
    </message>
    <message>
        <source>TTS</source>
        <translation type="obsolete">TTS</translation>
    </message>
    <message>
        <source>Enable console for %1</source>
        <translation type="obsolete">Povolit konsolu pro %1</translation>
    </message>
    <message>
        <source>Enable Text-To-Speech for %1</source>
        <translation type="obsolete">Povolit čtení textu nahlas pro %1</translation>
    </message>
    <message>
        <location filename="Log.ui" line="69"/>
        <source>Text To Speech</source>
        <translation>Čtení textu nahlas</translation>
    </message>
    <message>
        <location filename="Log.ui" line="75"/>
        <source>Volume</source>
        <translation>Hlasitost</translation>
    </message>
    <message>
        <location filename="Log.ui" line="85"/>
        <source>Volume of Text-To-Speech Engine</source>
        <translation>Hlasitost čtení textu nahlas</translation>
    </message>
    <message>
        <location filename="Log.ui" line="88"/>
        <source>&lt;b&gt;This is the volume used for the speech synthesis.&lt;/b&gt;</source>
        <translation>&lt;b&gt;Hlasitost pro hlasovou syntézu&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="Log.ui" line="113"/>
        <source>Length threshold</source>
        <translation>Maximální délka</translation>
    </message>
    <message>
        <location filename="Log.ui" line="123"/>
        <source>Message length threshold for Text-To-Speech Engine</source>
        <translation>Maximální délka zprávy pro přečtení hlasovým systémem</translation>
    </message>
    <message>
        <location filename="Log.ui" line="126"/>
        <source>&lt;b&gt;This is the length threshold used for the Text-To-Speech Engine.&lt;/b&gt;&lt;br /&gt;Messages longer than this limit will not be read aloud in their full length.</source>
        <translation>&lt;b&gt;Toto je maximální délka zprávy pro čtení&lt;/b&gt;&lt;br /&gt;Zprávy delší než tento limit nebudou přečteny v celé svojí délce.</translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Log.ui" line="46"/>
        <source>Notification</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Log.ui" line="51"/>
        <source>Text-To-Speech</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Log.ui" line="56"/>
        <source>Soundfile</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Log.ui" line="61"/>
        <source>Path</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Log.ui" line="132"/>
        <source> Characters</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>LookConfig</name>
    <message>
        <location filename="LookConfig.ui" line="316"/>
        <location filename="LookConfig.ui" line="322"/>
        <source>Language</source>
        <translation>Jazyk</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="157"/>
        <source>Look and Feel</source>
        <translation>Vzhled a motivy</translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="47"/>
        <location filename="LookConfig.cpp" line="58"/>
        <source>System default</source>
        <translation>Podle výchozího nastavení systému</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="332"/>
        <source>Language to use (requires restart)</source>
        <translation>Jazyk pro uživatelské rozhraní (vyžaduje restart)</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="335"/>
        <source>&lt;b&gt;This sets which language Mumble should use.&lt;/b&gt;&lt;br /&gt;You have to restart Mumble to use the new language.</source>
        <translation>&lt;b&gt;Nastaví jazyk, který má aplikace Mumble používat.&lt;/b&gt;&lt;br /&gt;Musíte restartovat Mumble, aby se změna projevila.</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="180"/>
        <source>Style</source>
        <translation>Styl</translation>
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
        <location filename="LookConfig.ui" line="187"/>
        <source>Basic widget style</source>
        <translation>Základní styl prvků grafického prostředí</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="190"/>
        <source>&lt;b&gt;This sets the basic look and feel to use.&lt;/b&gt;</source>
        <translation>&lt;b&gt;nastaví základní styl vzhledu aplikace.&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="197"/>
        <source>Skin</source>
        <translation>Motiv</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="214"/>
        <source>...</source>
        <translation>...</translation>
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
        <location filename="LookConfig.ui" line="204"/>
        <source>Skin file to use</source>
        <translation>Soubor motivu, který se bude používat</translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets which skin Mumble should use.&lt;/b&gt;&lt;br /&gt;The skin is a style file applied on top of the basic widget style.</source>
        <translation type="obsolete">&lt;b&gt;Takto se nastavi, který motiv bude Mumble používat.&lt;/b&gt;&lt;br /&gt;Motiv je soubor s definicí stylů aplikovaný navíc po použítí základního stylu prvků uživatelského rozhraní.</translation>
    </message>
    <message>
        <source>Use Horizontal Splitter</source>
        <translation type="obsolete">Použít horizontální rozdělení</translation>
    </message>
    <message>
        <source>Use horizontal or vertical splitter for the main window</source>
        <translation type="obsolete">Nastavení horizontálního nebo vertikálního rozdělení hlavního okna</translation>
    </message>
    <message>
        <source>&lt;b&gt;If set, players will be shown above subchannels in the channel view.&lt;/b&gt;&lt;br /&gt;A restart of Mumble is required to see the change.</source>
        <translation type="obsolete">&lt;b&gt;Pokud je tohle nastaveno, pak budou uživatelé zobrazeni v každém kanálu ještě před seznamem podkanálů v přehledu kanálu.&lt;/b&gt;&lt;br /&gt; Pro aktivaci této změny je nutno restartovat aplikaci Mumble.</translation>
    </message>
    <message>
        <source>Expand All Channels</source>
        <translation type="obsolete">Rozbalit všechny kanály</translation>
    </message>
    <message>
        <source>Expand all channels when connecting</source>
        <translation type="obsolete">Rozbalit všechny kanály při připojování</translation>
    </message>
    <message>
        <source>&lt;b&gt;If set, all channels will be expanded by default when you connect to a server.&lt;/b&gt;</source>
        <translation type="obsolete">&lt;b&gt;Pokud je nastaveno, strom komuikačních kanálů bude rozbalen při připojení k serveru.&lt;b&gt;</translation>
    </message>
    <message>
        <source>Players above Channels</source>
        <translation type="obsolete">Uživatelé nad kanály</translation>
    </message>
    <message>
        <source>List players above subchannels (requires restart).</source>
        <translation type="obsolete">Vypíše seznam uživatelů nad seznamem kanálů (vyžaduje restart aplikace).</translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="72"/>
        <source>User Interface</source>
        <translation>Rozhraní</translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="192"/>
        <source>Choose skin file</source>
        <translation>Vyberte soubor s motivem</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="207"/>
        <source>&lt;b&gt;This sets which skin Mumble should use.&lt;/b&gt;&lt;br /&gt;The skin is a style file applied on top of the basic widget style. If there are icons in the same directory as the style sheet, those will replace the default icons.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="14"/>
        <source>Form</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="62"/>
        <source>None</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="64"/>
        <source>All</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="221"/>
        <source>Expand</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="228"/>
        <source>When to automatically expand channels</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="63"/>
        <source>Only with users</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="66"/>
        <source>Ask</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="67"/>
        <source>Do Nothing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="68"/>
        <source>Move</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="163"/>
        <source>Channel Dragging</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="251"/>
        <source>Ask whether to close or minimize when quitting Mumble.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="257"/>
        <source>Ask on quit while connected</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="277"/>
        <source>Make the Mumble window appear on top of other windows.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="280"/>
        <source>&lt;b&gt;This makes the Mumble window a topmost window.&lt;/b&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="283"/>
        <source>Always On Top</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="267"/>
        <source>&lt;b&gt;If set, minimizing the Mumble main window will cause it to be hidden and accessible only from the tray. Otherwise, it will be minimized as a window normally would.&lt;/b&gt;</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="264"/>
        <source>Hide the main Mumble window in the tray when it is minimized.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <location filename="main.cpp" line="284"/>
        <source>Welcome to Mumble.</source>
        <translation>Vítejte v aplikaci Mumble.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="137"/>
        <location filename="MainWindow.cpp" line="1734"/>
        <source>Root</source>
        <translation>Kořen</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="166"/>
        <source>&amp;Quit Mumble</source>
        <translation type="unfinished">&amp;Ukončit</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="169"/>
        <source>Closes the program</source>
        <translation>Uzavře aplikaci</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="172"/>
        <source>Exits the application.</source>
        <translation>Ukončí aplikaci.</translation>
    </message>
    <message>
        <source>Ctrl+Q</source>
        <comment>Quit</comment>
        <translation type="obsolete">Ukončit</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="180"/>
        <source>&amp;Connect</source>
        <translation>&amp;Připojit</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="183"/>
        <source>Open the server connection dialog</source>
        <translation>Otevřít dialog nastavení spojení se serverem</translation>
    </message>
    <message>
        <source>Shows a dialog of registered servers, and also allows quick connect.</source>
        <translation type="obsolete">Ukáže dialog registrovaných serverů a umožňuje tak rychlé a snadné připojení k nim.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="194"/>
        <source>&amp;Disconnect</source>
        <translation>&amp;Odpojit</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="197"/>
        <source>Disconnect from server</source>
        <translation>Odpojit od serveru</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="200"/>
        <source>Disconnects you from the server.</source>
        <translation>Odpojí  k serveru otevřená spojení.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="208"/>
        <source>&amp;Ban lists</source>
        <translation>Seznam &amp;zákazů</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="211"/>
        <source>Edit ban lists on server</source>
        <translation>Upravit seznam banů</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="214"/>
        <source>This lets you edit the server-side IP ban lists.</source>
        <translation>Tady můžete upravit seznam banů (zakázaných ip adres) pro server.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="233"/>
        <source>&amp;Kick</source>
        <translation>&amp;Vykopnout</translation>
    </message>
    <message>
        <source>Kick player (with reason)</source>
        <translation type="obsolete">Vykonout uživatele s odůvodněním)</translation>
    </message>
    <message>
        <source>Kick selected player off server. You&apos;ll be asked to specify a reason.</source>
        <translation type="obsolete">Vykopne vybraného uživatele ze serveru. Budete dotázán na důvod.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="258"/>
        <source>&amp;Ban</source>
        <translation>Zákaz (&amp;ban)</translation>
    </message>
    <message>
        <source>Kick and ban player (with reason)</source>
        <translation type="obsolete">Vykopnout a zakázat uživatele (s odůvodněním)</translation>
    </message>
    <message>
        <source>Kick and ban selected player from server. You&apos;ll be asked to specify a reason.</source>
        <translation type="obsolete">Vykopne a zakáže vybraného uživatele. Budete požádání o doplnění důvodu.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="247"/>
        <source>&amp;Mute</source>
        <translation>Vypnout &amp;mluvení</translation>
    </message>
    <message>
        <source>Mute player</source>
        <translation type="obsolete">Zakázat uživateli mluvit</translation>
    </message>
    <message>
        <source>Mute or unmute player on server. Unmuting a deafened player will also undeafen them.</source>
        <translation type="obsolete">Zakáže vybranému uživateli na serveru schopost mluvit, nebo mu ji opět vrátí. Vrácením práva mluvit uživateli s vypnutým poslechem se mu zároveň zapne poslech.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="272"/>
        <source>&amp;Deafen</source>
        <translation>Zakázat &amp;poslouchání</translation>
    </message>
    <message>
        <source>Deafen player</source>
        <translation type="obsolete">Vypnout poslech uživateli</translation>
    </message>
    <message>
        <source>Deafen or undeafen player on server. Deafening a player will also mute them.</source>
        <translation type="obsolete">Vypnout/zapnout poslech uživateli.  Vypnutím poslechu se uživateli zároveň zakáže mluvit.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="286"/>
        <source>&amp;Local Mute</source>
        <translation>Lokální um&amp;lčení</translation>
    </message>
    <message>
        <source>Mute or unmute player locally.</source>
        <translation type="obsolete">Lokální zákaz/povolení mluvení uživateli. </translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="297"/>
        <location filename="MainWindow.ui" line="538"/>
        <source>Send Messa&amp;ge</source>
        <translation>&amp;Odeslat zprávu</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="300"/>
        <location filename="MainWindow.ui" line="541"/>
        <source>Send a Text Message</source>
        <translation>Odeslat textovou zprávu</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="303"/>
        <source>Sends a text message to another user.</source>
        <translation>Pošle textovou zprávu jinému uživateli.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="308"/>
        <source>&amp;Add</source>
        <translation>&amp;Přidat</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="311"/>
        <source>Add new channel</source>
        <translation>Přidat nový kanál</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="314"/>
        <source>This adds a new sub-channel to the currently selected channel.</source>
        <translation>Přidá nový kanál do vybraného kanálu. </translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="319"/>
        <source>&amp;Remove</source>
        <translation>Odst&amp;ranit</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="322"/>
        <source>Remove channel</source>
        <translation>Odstranit kanál</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="325"/>
        <source>This removes a channel and all sub-channels.</source>
        <translation>Odstraní kanál a všechny jeho podkanály.</translation>
    </message>
    <message>
        <source>&amp;Edit ACL</source>
        <translation type="obsolete">Uprav&amp;it ACL</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="333"/>
        <source>Edit Groups and ACL for channel</source>
        <translation>Upravit Skupiny a ACL pro kanál</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="336"/>
        <source>This opens the Group and ACL dialog for the channel, to control permissions.</source>
        <translation>Otevře dialogové okno skupin a ACL daného kanálu. Zde se mohou nastavit oprávnění k určitým operacím.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="341"/>
        <source>&amp;Link</source>
        <translation>&amp;Propojit</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="344"/>
        <source>Link your channel to another channel</source>
        <translation>Propojí současný kanál s kanálem označeným kurzorem</translation>
    </message>
    <message>
        <source>This links your current channel to the selected channel. If they have permission to speak in the other channel, players can now hear each other. This is a permanent link, and will last until manually unlinked or the server is restarted. Please see the shortcuts for push-to-link.</source>
        <translation type="obsolete">Propojí současný kanál s vybraným kanálem. Pokud mají povoleno mluvit, uslyší se účastníci navzájem. Toto spojení bude fungovat až do jeho zrušení nebo do restartování serveru. </translation>
    </message>
    <message>
        <source>&amp;Unlink</source>
        <translation type="obsolete">O&amp;dpojit</translation>
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
        <location filename="MainWindow.ui" line="355"/>
        <source>Unlink your channel from another channel</source>
        <translation>Zruší propojení současného a zvoleného kanálu</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="358"/>
        <source>This unlinks your current channel from the selected channel.</source>
        <translation>Zruší propojení mezi současným a vybraným kanálem.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="363"/>
        <source>Unlink &amp;All</source>
        <translation>Odpoj&amp;it vše</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="366"/>
        <source>Unlinks your channel from all linked channels.</source>
        <translation>Zruší propojení sučasného kanálu ke všem kanálům s ním spojeným.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="369"/>
        <source>This unlinks your current channel (not the selected one) from all linked channels.</source>
        <translation>Zruší propojení mezi současným kanálem uživatele a všech kanálů, které jsou s tímto kanálem propojeny. </translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="374"/>
        <source>&amp;Reset</source>
        <translation>&amp;Resetovat audio</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="377"/>
        <source>Reset audio preprocessor</source>
        <translation>Resetuje zvukový preprocesor</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="380"/>
        <source>This will reset the audio preprocessor, including noise cancellation, automatic gain and voice activity detection. If something suddenly worsens the audio environment (like dropping the microphone) and it was temporary, use this to avoid having to wait for the preprocessor to readjust.</source>
        <translation>Resetuje zvukový preprocesor, včetně rušení šumu, automatické hlasitosti a detekce hlasové aktivity. Pokud se nějak náhle zhorší zvukové prostředí (například při upuštění mikrofonu na stůl) a příčina problémů odezní, jde tak rychle znovunastavit rozumné hodnoty.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="388"/>
        <source>&amp;Mute Self</source>
        <translation>Vypnout si &amp;mluvení</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="391"/>
        <source>Mute yourself</source>
        <translation>Zakázat si mluvení</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="394"/>
        <source>Mute or unmute yourself. When muted, you will not send any data to the server. Unmuting while deafened will also undeafen.</source>
        <translation>Zakázat/povolit si mluvení.  Když je zakázáno mluvení, neposílají se na server žádná zvuková data. Povolením mluvení se zároveň zapína poslech.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="402"/>
        <source>&amp;Deafen Self</source>
        <translation>Vypnout si &amp;poslech</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="405"/>
        <source>Deafen yourself</source>
        <translation>Vypnout si poslech</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="408"/>
        <source>Deafen or undeafen yourself. When deafened, you will not hear anything. Deafening yourself will also mute.</source>
        <translation>Vypnout si poslech. Tak neuslyšíte komunikaci. Vypnutí poslechu automaticky zároveň zakáže mluvení.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="416"/>
        <source>&amp;Text-To-Speech</source>
        <translation>Výstup &amp;textu do hlasové syntézy</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="419"/>
        <source>Toggle Text-To-Speech</source>
        <translation>Přepnout výstup textu do hlasové syntézy</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="422"/>
        <source>Enable or disable the text-to-speech engine. Only messages enabled for TTS in the Configuration dialog will actually be spoken.</source>
        <translation>Povolí nebo zakáže převod textu na počítačem mluvený hlas. Budou přečtena pouze textová hlášení, u kterých je to nastaveno v konfiguračním dialogu.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="427"/>
        <source>S&amp;tatistics</source>
        <translation>S&amp;tatistika</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="430"/>
        <source>Display audio statistics</source>
        <translation>Zobrazí zvukovou statistiku</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="433"/>
        <source>Pops up a small dialog with information about your current audio input.</source>
        <translation>Vyskočí malé okénko s informacemi o současném audio vstupu.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="441"/>
        <source>Forcibly unlink plugin</source>
        <translation>Násilně odstraň plugin</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="444"/>
        <source>This forces the current plugin to unlink, which is handy if it is reading completely wrong data.</source>
        <translation>Násilně udpojí současný plugin, což může být užitečné, pokud čte a zpracovává úplně chybný data.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="449"/>
        <source>&amp;Settings</source>
        <translation>&amp;Nastavení</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="452"/>
        <source>Configure Mumble</source>
        <translation>Nastavit Mumble</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="455"/>
        <source>Allows you to change most settings for Mumble.</source>
        <translation>Nastavení většiny možností.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="463"/>
        <source>&amp;Audio Wizard</source>
        <translation>&amp;Zvukový Wizard</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="466"/>
        <source>Start the audio configuration wizard</source>
        <translation>Spustí zvukový Wizard pro nastavení</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="469"/>
        <source>This will guide you through the process of configuring your audio hardware.</source>
        <translation>Tohle vás provede zkrze proces konfigurace zvukového hardware.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="474"/>
        <source>&amp;What&apos;s This?</source>
        <translation>&amp;Co je to?</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="477"/>
        <source>Enter What&apos;s This? mode</source>
        <translation>Vstoupí do režimu &quot;co je tohle?&quot;</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="480"/>
        <source>Click this to enter &quot;What&apos;s This?&quot; mode. Your cursor will turn into a question mark. Click on any button, menu choice or area to show a description of what it is.</source>
        <translation>Klikněte pro vsoupení do režimu &quot;Co je tohle?&quot; Váš kurzor se změní na otazník. Jakmile máte tlačítko, výběr menu nebo cokoliv co chcete ukázat.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="485"/>
        <source>&amp;About</source>
        <translation>&amp;O aplikaci</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="488"/>
        <source>Information about Mumble</source>
        <translation>Informace o Mumble</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="491"/>
        <source>Shows a small dialog with information and license for Mumble.</source>
        <translation>Ukáže okno s informacemi a licenčním ujednáním pro používání Mumble.


</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="499"/>
        <source>About &amp;Speex</source>
        <translation>O kodeku &amp;Speex</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="502"/>
        <source>Information about Speex</source>
        <translation>Informace o kodeku Speex</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="505"/>
        <source>Shows a small dialog with information about Speex.</source>
        <translation>Ukáže informace o použitém kodeku Speex.</translation>
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
        <source>&amp;About Qt</source>
        <translation type="obsolete">O &amp;knihovně Qt</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="516"/>
        <source>Information about Qt</source>
        <translation>Informace o knihovně Qt</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="519"/>
        <source>Shows a small dialog with information about Qt.</source>
        <translation>Ukáže dialogové okno s informacemi o knihovně QT.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="527"/>
        <source>Check for &amp;Updates</source>
        <translation>Zjistit &amp;dostupnost nové verze</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="530"/>
        <source>Check for new version of Mumble</source>
        <translation>Ověřit dostupnost nových verzí aplikace Mumble</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="533"/>
        <source>Connects to the Mumble webpage to check if a new version is available, and notifies you with an appropriate download URL if this is the case.</source>
        <translation>Připojí se k webové stránce Mumble pro zjištění případné dostupnosti novějších verzí a vypíše vhodný odkaz na stažení, pokud je nová verze k dispozici.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="219"/>
        <location filename="MainWindow.cpp" line="231"/>
        <source>Mumble -- %1</source>
        <translation>Mumble =- %1 </translation>
    </message>
    <message>
        <source>Log of messages</source>
        <translation type="obsolete">Záznam zpráv</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="120"/>
        <source>This shows all recent activity. Connecting to servers, errors and information messages all show up here.&lt;br /&gt;To configure exactly which messages show up here, use the &lt;b&gt;Settings&lt;/b&gt; command from the menu.</source>
        <translation>Zobrazuje nedávné aktivity, jako např. připojování k serverům, chyby a informační zprávy.&lt;br /&gt; V nastavení jde určit, které z těchto zpráv se mají či nemají zobrazovat.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="53"/>
        <source>&amp;Server</source>
        <translation>&amp;Server</translation>
    </message>
    <message>
        <source>&amp;Player</source>
        <translation type="obsolete">&amp;Uživatel</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="63"/>
        <source>&amp;Channel</source>
        <translation>&amp;Kanál</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="68"/>
        <source>&amp;Audio</source>
        <translation>&amp;Zvuk</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="82"/>
        <source>C&amp;onfigure</source>
        <translation>&amp;Nastavit</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="88"/>
        <source>&amp;Help</source>
        <translation>&amp;Pomoc</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="175"/>
        <source>Push-to-Talk</source>
        <comment>Global Shortcut</comment>
        <translation>Mluvení při stisku klávesy</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="177"/>
        <source>Push and hold this button to send voice.</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="178"/>
        <source>This configures the push-to-talk button, and as long as you hold this button down, you will transmit voice.</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="181"/>
        <source>Reset Audio Processor</source>
        <comment>Global Shortcut</comment>
        <translation>Resetovat zvukový procesor</translation>
    </message>
    <message>
        <source>Toggle Mute Self</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">Zákázat/povolit si mluvení</translation>
    </message>
    <message>
        <source>Toggle Deafen Self</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">Poslech zapnout/vypnout</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="194"/>
        <source>Unlink Plugin</source>
        <comment>Global Shortcut</comment>
        <translation>Odpojit Plugin</translation>
    </message>
    <message>
        <source>Force Center Position</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">Vynutit použití střední pozice</translation>
    </message>
    <message>
        <source>Chan Parent</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">Nadřazený kanál</translation>
    </message>
    <message>
        <source>Chan Sub#%1</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">Podkanál #%1</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="197"/>
        <source>Push-to-Mute</source>
        <comment>Global Shortcut</comment>
        <translation>Zakázat si mluvení při stisku klávesy</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="200"/>
        <source>Join Channel</source>
        <comment>Global Shortcut</comment>
        <translation>Vstoupit do kanálu</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="203"/>
        <source>Toggle Overlay</source>
        <comment>Global Shortcut</comment>
        <translation>Přepnout Overlay</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="205"/>
        <source>Toggle state of in-game overlay.</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt Push-to-Talk</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">Alternativní klávesa pro mluvení</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="648"/>
        <source>Reconnecting.</source>
        <translation>Opětovné připojování.</translation>
    </message>
    <message>
        <source>Kicking player %1</source>
        <translation type="obsolete">Vykopnutí uživaetele %1</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="981"/>
        <location filename="MainWindow.cpp" line="1003"/>
        <source>Enter reason</source>
        <translation>Zadejte důvod</translation>
    </message>
    <message>
        <source>Banning player %1</source>
        <translation type="obsolete">Ban pro uživatele %1</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1025"/>
        <source>Sending message to %1</source>
        <translation>Posílam zprávu pro %1</translation>
    </message>
    <message>
        <source>Enter message</source>
        <translation type="obsolete">Zadejte zprávu</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1038"/>
        <location filename="MainWindow.cpp" line="1099"/>
        <source>To %1: %2</source>
        <translation>Pro %1: %2</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1038"/>
        <location filename="MainWindow.cpp" line="1099"/>
        <source>Message to %1</source>
        <translation>Zpráva pro %1</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="14"/>
        <location filename="MainWindow.cpp" line="328"/>
        <location filename="MainWindow.cpp" line="1223"/>
        <location filename="MainWindow.cpp" line="1819"/>
        <location filename="main.cpp" line="301"/>
        <source>Mumble</source>
        <translation>Mumble</translation>
    </message>
    <message>
        <source>Channel Name</source>
        <translation type="obsolete">Název kanálu</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1223"/>
        <source>Are you sure you want to delete %1 and all its sub-channels?</source>
        <translation>Opravdu chcete odstranit kanál %1 včetně všech jeho podkanálů?</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1396"/>
        <source>Unmuted and undeafened.</source>
        <translation>Opět schopen slyšet a mluvit.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1398"/>
        <source>Unmuted.</source>
        <translation>Opět schopen mluvit.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1400"/>
        <source>Muted.</source>
        <translation>Zakázáno mluvení.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1427"/>
        <source>Muted and deafened.</source>
        <translation>Zakázáno mluvit a poslouchat.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1429"/>
        <source>Deafened.</source>
        <translation>Vypnut poslech.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1431"/>
        <source>Undeafened.</source>
        <translation>Opět schopen slyšet.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1499"/>
        <source>About Qt</source>
        <translation>O knihovně Qt</translation>
    </message>
    <message>
        <source>Joining %1.</source>
        <translation type="obsolete">Vstupuji do %1.</translation>
    </message>
    <message>
        <source>Connected to server.</source>
        <translation type="obsolete">Připojen k serveru.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1877"/>
        <source>Server connection failed: %1.</source>
        <translation>Připojování k serveru selhalo: %1.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1891"/>
        <source>Invalid username</source>
        <translation>Špatné jméno uživatele</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1891"/>
        <source>You connected with an invalid username, please try another one.</source>
        <translation>Připojujete se s nesprávným jménem uživatele, prosím zkuste nějaké jiné.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1891"/>
        <source>That username is already in use, please try another username.</source>
        <translation>Toto uživatelské jméno je již používáno, prosím zkuste nějaké jiné.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1896"/>
        <source>Wrong password</source>
        <translation>Špatné heslo</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1896"/>
        <source>Wrong password for registered users, please try again.</source>
        <translation>Špatné heslo registrovaného uživatele, prosím zkuste znova.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1896"/>
        <source>Wrong server password for unregistered user account, please try again.</source>
        <translation>Špatné heslo serveru pro neregistrované uživatele, prosím zkuste znova.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1879"/>
        <source>Disconnected from server.</source>
        <translation>Spojení se serverem bylo zrušeno.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="201"/>
        <source>Joined server: %1.</source>
        <translation>K serveru se připojil: %1.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="397"/>
        <source>Left server: %1.</source>
        <translation>Uživatel %1 opustil server.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="230"/>
        <source>%1 is now muted and deafened.</source>
        <translation>%1 má zakázáno mluvit a poslouchat.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="232"/>
        <source>%1 is now muted.</source>
        <translation>%1 má zakázáno mluvit.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="234"/>
        <source>%1 is now unmuted.</source>
        <translation>%1 je nyní schopen mluvit.</translation>
    </message>
    <message>
        <source>server</source>
        <translation type="obsolete">server</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="258"/>
        <source>You were muted by %1.</source>
        <translation>%1 Vám zakázal mluvit.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="260"/>
        <source>You were unmuted by %1.</source>
        <translation>Vrácena schopnost mluvit od %1.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="310"/>
        <source>%1 muted by %2.</source>
        <translation>%2 zakázal mluvit uživateli %1.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="312"/>
        <source>%1 unmuted by %2.</source>
        <translation>%1 získává schopnost mluvit od %2.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="252"/>
        <source>You were muted and deafened by %1.</source>
        <oldsource>You were deafened by %1.</oldsource>
        <translation type="unfinished">Bylo vám vypnut poslech uživatelem %1.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="265"/>
        <source>You were undeafened by %1.</source>
        <translation type="unfinished">Byla Vám vrácena schopnost slyšet od %1.</translation>
    </message>
    <message>
        <source>%1 deafened by %2.</source>
        <translation type="obsolete">%2 vypnul poslech uživateli %1.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="317"/>
        <source>%1 undeafened by %2.</source>
        <translation type="unfinished">%1 byla vlrácena schopnost slyšet od %2.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="390"/>
        <source>You were kicked from the server by %1: %2.</source>
        <translation>Byl jste vykopnut ze serveru od %1: %2.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="395"/>
        <source>%3 was kicked from the server by %1: %2.</source>
        <translation>%3 byl vykopnut ze serveru od %1: %2.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="388"/>
        <source>You were kicked and banned from the server by %1: %2.</source>
        <translation>Byl jste vykopnut a dostal jste ban (zákaz) od %1: %2.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="393"/>
        <source>%3 was kicked and banned from the server by %1: %2.</source>
        <translation>%3 byl vykopnut a dostal ban (zákaz) od %1: %2.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="347"/>
        <source>You were moved to %1 by %2.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>%1 left channel.</source>
        <translation type="obsolete">%1 opustil kanál.</translation>
    </message>
    <message>
        <source>%1 moved out by %2.</source>
        <translation type="obsolete">%1 byl přesunut z kanálu uživatelem %2.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="360"/>
        <source>%1 entered channel.</source>
        <translation>%1 vstoupil do kanálu.</translation>
    </message>
    <message>
        <source>%1 moved in by %2.</source>
        <translation type="obsolete">%1 byl přesunut do kanálu zásahem uživatele %2.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="87"/>
        <source>Server connection rejected: %1.</source>
        <translation>Spojení se serverem bylo odmítnuto: %1.</translation>
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
        <location filename="Messages.cpp" line="182"/>
        <source>Denied: %1.</source>
        <translation>Zakázano: %1.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="184"/>
        <source>Permission denied.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="248"/>
        <location filename="Messages.cpp" line="343"/>
        <location filename="Messages.cpp" line="383"/>
        <source>the server</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="254"/>
        <source>You were unmuted and undeafened by %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="271"/>
        <source>You were suppressed by %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="273"/>
        <source>You were unsuppressed by %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="285"/>
        <source>You muted %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="298"/>
        <source>You suppressed %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="300"/>
        <source>You unsuppressed %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="287"/>
        <source>You unmuted %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="279"/>
        <source>You muted and deafened %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="281"/>
        <source>You unmuted and undeafened %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="292"/>
        <source>You undeafened %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="304"/>
        <source>%1 muted and deafened by %2.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="306"/>
        <source>%1 unmuted and undeafened by %2.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="323"/>
        <source>%1 suppressed by %2.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="325"/>
        <source>%1 unsuppressed by %2.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="350"/>
        <source>%1 moved to %2.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="352"/>
        <source>%1 moved to %2 by %3.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="362"/>
        <source>%1 moved in from %2 by %3.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="481"/>
        <source>the server</source>
        <comment>message from</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="482"/>
        <source>From %1: %2</source>
        <translation>Od %1: %2</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="483"/>
        <source>Message from %1</source>
        <translation></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="620"/>
        <source>Unable to find matching CELT codecs with other clients. You will not be able to talk to all users.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="222"/>
        <source>&amp;Information</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="225"/>
        <source>Show information about the server connection</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="228"/>
        <source>This will show extended information about the connection to the server.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="396"/>
        <source>Opening URL %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="400"/>
        <source>File does not exist</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="410"/>
        <source>File is not a configuration file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="416"/>
        <source>Settings merged from file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="422"/>
        <source>URL scheme is not &apos;mumble&apos;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="447"/>
        <source>This version of Mumble can&apos;t handle URLs for Mumble version %1.%2.%3</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="473"/>
        <source>Connecting to %1</source>
        <translation type="unfinished">Připojování k %1</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="473"/>
        <source>Enter username</source>
        <translation type="unfinished">Zadejte jmého uživatele</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="486"/>
        <location filename="MainWindow.cpp" line="638"/>
        <source>Connecting to server %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="723"/>
        <source>&lt;h2&gt;Version&lt;/h2&gt;&lt;p&gt;Protocol %1.%2.%3.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="728"/>
        <source>&lt;p&gt;No build information or OS version available.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="730"/>
        <source>&lt;p&gt;%1 (%2)&lt;br /&gt;%3&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="734"/>
        <source>&lt;h2&gt;Control channel&lt;/h2&gt;&lt;p&gt;Encrypted with %1 bit %2&lt;br /&gt;%3 ms average latency (%4 deviation)&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="743"/>
        <source>&lt;h2&gt;Voice channel&lt;/h2&gt;&lt;p&gt;Encrypted with 128 bit OCB-AES128&lt;br /&gt;%1 ms average latency (%4 deviation)&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="754"/>
        <source>&lt;h2&gt;Audio bandwidth&lt;/h2&gt;&lt;p&gt;Maximum %1 kbit/s&lt;br /&gt;Current %2 kbit/s&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="756"/>
        <source>Mumble Server Information</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="760"/>
        <location filename="MainWindow.cpp" line="1827"/>
        <source>&amp;View Certificate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="936"/>
        <source>Register yourself as %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="936"/>
        <source>&lt;p&gt;You are about to register yourself on this server. This action cannot be undone, and your username cannot be changed once this is done. You will forever be known as &apos;%1&apos; on this server.&lt;/p&gt;&lt;p&gt;Are you sure you want to register yourself?&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="938"/>
        <source>Register user %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="938"/>
        <source>&lt;p&gt;You are about to register %1 on the server. This action cannot be undone, the username cannot be changed, and as a registered user, %1 will have access to the server even if you change the server password.&lt;/p&gt;&lt;p&gt;From this point on, %1 will be authenticated with the certificate currently in use.&lt;/p&gt;&lt;p&gt;Are you sure you want to register %1?&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="981"/>
        <source>Kicking user %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1003"/>
        <source>Banning user %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1052"/>
        <source>Change comment on user %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1095"/>
        <location filename="MainWindow.cpp" line="1312"/>
        <source>To channel %1: %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1095"/>
        <location filename="MainWindow.cpp" line="1312"/>
        <source>Message to channel %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1728"/>
        <source>Connected.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1844"/>
        <source>SSL Version mismatch</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1844"/>
        <source>This server is using an older encryption standard. It might be an older 1.1 based Mumble server.&lt;br /&gt;Would you like to launch the compatibility client to connect to it?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1870"/>
        <source>The compatibility client could not be found, or failed to start.&lt;br /&gt;Note that the compatibility client is an optional component for most installations, and might not be installed.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1806"/>
        <source>SSL Verification failed: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1811"/>
        <source>&lt;b&gt;WARNING:&lt;/b&gt; The server presented a certificate that was different from the stored one.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1813"/>
        <source>Sever presented a certificate which failed verification.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1820"/>
        <source>&lt;p&gt;%1.&lt;br /&gt;The specific errors with this certificate are: &lt;/p&gt;&lt;ol&gt;%2&lt;/ol&gt;&lt;p&gt;Do you wish to accept this certificate anyway?&lt;br /&gt;(It will also be stored so you won&apos;t be asked this again.)&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1870"/>
        <source>Failed to launch compatibility client</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1987"/>
        <source>Type message to channel &apos;%1&apos; here</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1990"/>
        <source>Type message to user &apos;%1&apos; here</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="2069"/>
        <source>Choose image file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="2069"/>
        <source>Images (*.png *.jpg)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="2076"/>
        <location filename="MainWindow.cpp" line="2088"/>
        <source>Failed to load image</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="2076"/>
        <source>Could not open file for reading.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="2088"/>
        <source>Image format not recognized.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="741"/>
        <source>Voice channel is sent over control channel.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="750"/>
        <source>UDP Statistics</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="750"/>
        <source>To Server</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="750"/>
        <source>From Server</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="750"/>
        <source>Good</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="750"/>
        <source>Late</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="750"/>
        <source>Lost</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="750"/>
        <source>Resync</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1295"/>
        <source>Sending message to channel %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1310"/>
        <source>To tree %1: %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1310"/>
        <source>Message to tree %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="175"/>
        <source>Ctrl+Q</source>
        <translation type="unfinished">Ukončit</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="513"/>
        <source>About &amp;Qt</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="544"/>
        <source>Sends a text message to all users in a channel.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="main.cpp" line="301"/>
        <source>This is the first time you&apos;re starting Mumble.&lt;br /&gt;Would you like to go through the Audio Wizard to configure your soundcard?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="328"/>
        <source>Mumble is currently connected to a server. Do you want to Close or Minimize it?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="329"/>
        <source>Close</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="237"/>
        <location filename="MainWindow.cpp" line="330"/>
        <source>Minimize</source>
        <translation type="unfinished"></translation>
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
        <location filename="MainWindow.cpp" line="235"/>
        <source>&amp;Window</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="237"/>
        <source>Ctrl+M</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="209"/>
        <source>Toggle Minimal</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="552"/>
        <source>&amp;Minimal View</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="555"/>
        <source>Toggle minimal window modes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="558"/>
        <source>This will toggle minimal mode, where the log window and menu is hidden.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="212"/>
        <source>Volume Up (+10%)</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="215"/>
        <source>Volume Down (-10%)</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="279"/>
        <location filename="MainWindow.cpp" line="1981"/>
        <source>Not connected</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="390"/>
        <source>Clear</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="110"/>
        <source>Log</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="1251"/>
        <source>You have Channel Dragging set to &quot;Do Nothing&quot; so the channel wasn&apos;t moved.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="1257"/>
        <source>Unknown Channel Drag mode in UserModel::dropMimeData.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="352"/>
        <source>&amp;Unlink</source>
        <comment>Channel</comment>
        <translation type="unfinished">O&amp;dpojit</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="438"/>
        <source>&amp;Unlink</source>
        <comment>Plugin</comment>
        <translation type="unfinished">O&amp;dpojit</translation>
    </message>
    <message>
        <source>Server</source>
        <comment>message from</comment>
        <translation type="obsolete">Server</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="566"/>
        <source>Hide Frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="569"/>
        <source>Toggle showing frame on minimal window</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="572"/>
        <source>This will toggle whether the minimal window should have a frame for moving and resizing.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>NetworkConfig</name>
    <message>
        <location filename="NetworkConfig.cpp" line="52"/>
        <source>Network</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="14"/>
        <source>Form</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="26"/>
        <source>Connection</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="32"/>
        <source>Use TCP compatibility mode</source>
        <oldsource>Use TCP compatability mode</oldsource>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="35"/>
        <source>&lt;b&gt;Enable TCP compatibility mode&lt;/b&gt;.&lt;br /&gt;This will make Mumble use only TCP when communicating with the server. This will increase overhead and cause lost packets to produce noticeable pauses in communication, so this should only be used if you are unable to use the default (which uses UDP for voice and TCP for control).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Use TCP mode</source>
        <translation type="obsolete">Použít TCP režim</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="58"/>
        <source>Reconnect when disconnected</source>
        <translation type="unfinished">Přihlásit zpátky na server, pokud došlo k výpadku spojení</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="61"/>
        <source>&lt;b&gt;Reconnect when disconnected&lt;/b&gt;.&lt;br /&gt;This will make Mumble try to automatically reconnect after 10 seconds if your server connection fails.</source>
        <translation type="unfinished">&lt;b&gt;Automatické připojení&lt;/b&gt;.&lt;br /&gt;Pokud se ztratí spojení se serverem,  Mumble se pokusí automaticky spojení obnovit každých 10 sekund.</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="64"/>
        <source>Reconnect automatically</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="93"/>
        <source>Proxy</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="99"/>
        <source>Type</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="112"/>
        <source>Type of proxy to connect through</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="119"/>
        <source>Direct connection</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="124"/>
        <source>HTTP(S) proxy</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="129"/>
        <source>SOCKS5 proxy</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="137"/>
        <source>Hostname</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="153"/>
        <source>Hostname of the proxy</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="203"/>
        <source>Port number of the proxy</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="206"/>
        <source>&lt;b&gt;Port number of the proxy.&lt;/b&gt;&lt;br /&gt;This field specifies the port number that the proxy expects connections on.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="222"/>
        <source>Username</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="229"/>
        <source>Username for proxy authentication</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="232"/>
        <source>&lt;b&gt;Username for proxy authentication.&lt;/b&gt;&lt;br /&gt;This specifies the username you use for authenticating yourself with the proxy. In case the proxy does not use authentication, or you want to connect anonymously, simply leave this field blank.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="239"/>
        <source>Password</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="246"/>
        <source>Password for proxy authentication</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="249"/>
        <source>&lt;b&gt;Password for proxy authentication.&lt;/b&gt;&lt;br /&gt;This specifies the password you use for authenticating yourself with the proxy. In case the proxy does not use authentication, or you want to connect anonymously, simply leave this field blank.</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
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
        <location filename="NetworkConfig.ui" line="323"/>
        <source>Submit anonymous statistics to the Mumble project</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="329"/>
        <source>Submit anonymous statistics</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>OSSConfig</name>
    <message>
        <source>%1ms</source>
        <translation type="obsolete">%1ms</translation>
    </message>
    <message>
        <source>Device selection</source>
        <translation type="obsolete">Výběr zařízení</translation>
    </message>
    <message>
        <source>Input</source>
        <translation type="obsolete">Vstup</translation>
    </message>
    <message>
        <source>Device to use for microphone</source>
        <translation type="obsolete">Zařízení, na kterém je mikrofon</translation>
    </message>
    <message>
        <source>This set which device mumble should use. The &lt;i&gt;default&lt;/i&gt; device is whatever you have configured in alsaconfig, the &lt;i&gt;hwplug&lt;/i&gt; devices are specific hardware devices backed by the ALSA mixer and the &lt;i&gt;hw&lt;/i&gt; devices are raw hardware access. Unless your soundcard supports hardware mixing of audio, using the &lt;i&gt;hw&lt;/i&gt; device will exclude all other programs from using audio.</source>
        <translation type="obsolete">Nastavení zařízení pro použití aplikací Mumble. &lt;i&gt;default&lt;/i&gt; (výchozí) je cokoliv je nakonfigurováno pomoci alsaconfigu, &lt;i&gt;hwplug&lt;/i&gt; zařízení jsou specifická zvuková zařízení podporovaná ALSA mixerem a &lt;i&gt;hw&lt;/i&gt; zařízní jsou pro přímý přístup na zařízení. Pokud vaše zvuková karta nepodporuje hw směšování, použití &lt;i&gt;hw&lt;/i&gt; zařázení znemožní ostatním aplikacím používat tento zvukový výstup.</translation>
    </message>
    <message>
        <source>Output</source>
        <translation type="obsolete">Výstup</translation>
    </message>
    <message>
        <source>Device to use for speakers/headphones</source>
        <translation type="obsolete">Zařízení pro zvukový výstup (bedničky nebo sluchátka)</translation>
    </message>
    <message>
        <source>Output Options</source>
        <translation type="obsolete">Výstupní volby</translation>
    </message>
    <message>
        <source>Output Delay</source>
        <translation type="obsolete">Výstupní opoždění</translation>
    </message>
    <message>
        <source>Amount of data to buffer for OSS</source>
        <translation type="obsolete">Velikost datového zásobníku pro ALSU</translation>
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
        <translation type="obsolete">Selhalo načítání knihovny pro funkci overlay.
To může být způsobeno například těmito důvody:
- knihovna (mumble_ol.dll) nebyla nalezena v adresáři kde byla spuštěna aplikace Mumble
- váš OS je novější než WinXP SP2
- nemáte aktualizaci DX9.0c z června 2007</translation>
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
        <translation>Volby</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="78"/>
        <source>Position</source>
        <translation>Umístění</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="171"/>
        <source>Font</source>
        <translation>Písmo</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="32"/>
        <source>Enable Overlay</source>
        <translation>Zapnout Overlay</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="26"/>
        <source>Enable overlay.</source>
        <translation>Zapnout overlay.</translation>
    </message>
    <message>
        <source>This sets whether the overlay is enabled or not. This setting is only checked with D3D9 applications are started, so make sure Mumble is running and this option is on before you start the application.&lt;br /&gt;Please note that if you start the application after starting Mumble, or if you disable the overlay while running, there is no safe way to restart the overlay without also restarting the application.</source>
        <translation type="obsolete">Nastavuje funkci overlay.  Toto nastavení D3D9 applikace zjišťují při spouštění, takže se ujistěte, že Mumble běží a overlay je zapnut ještě než pustíte svoji aplikaci.&lt;br /&gt;Prosím všimněte si, že pokud aplikaci spustíte po Mumble, nebo pokud zakážete overlay funkci za běhu, není možné bezpečně vypnout overlay bez restartu dané aplikace.</translation>
    </message>
    <message>
        <source>No one</source>
        <translation type="obsolete">Nikoho</translation>
    </message>
    <message>
        <source>Only talking</source>
        <translation type="obsolete">Pouze mluvící</translation>
    </message>
    <message>
        <source>Everyone</source>
        <translation type="obsolete">Každého</translation>
    </message>
    <message>
        <source>Show</source>
        <translation type="obsolete">Ukázat</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="39"/>
        <source>Who to show on the overlay</source>
        <translation>Kdo bude zobrazen v overlay vrstvě</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="42"/>
        <source>&lt;b&gt;This sets who to show in the in-game overlay.&lt;/b&gt;&lt;br /&gt;If many people are connected to the same channel, the overlay list might be very long. Use this to shorten it.&lt;br /&gt;&lt;i&gt;No one&lt;/i&gt; - Don&apos;t show anyone (but leave overlay running).&lt;br /&gt;&lt;i&gt;Only talking&lt;/i&gt; - Only show talking people.&lt;br /&gt;&lt;i&gt;Everyone&lt;/i&gt; - Show everyone.</source>
        <translation>&lt;b&gt;Nastaví, kdo bude vidět v overlay vrstvě.&lt;/b&gt;&lt;br /&gt;Pokud bude připojeno příliš mnoho lidí, seznam se může stát nepříjemně dlouhým. Tímto nastavením jej můžete zkrátit.&lt;br /&gt;&lt;i&gt;Nikoho&lt;/i&gt; - nezobrazovat nikoho (ale nechat běžící overlay).&lt;br /&gt;&lt;i&gt;Pozue mluvící&lt;/i&gt; - zobrazí pouze uživatele, kteří právě mluví.&lt;br /&gt;&lt;i&gt;Každého&lt;/i&gt; - zobrazí úplně každého.</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="55"/>
        <source>Always Show Self</source>
        <translation>Vždy ukázat sebe</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="49"/>
        <source>Always show yourself on overlay.</source>
        <translation>Vždy ukáže vaše jméno v overlay vrstvě.</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="52"/>
        <source>This sets whether to always show yourself or not. This setting is useful if you aren&apos;t showing everyone in the overlay, as then you would only see your own status if you were talking, which wouldn&apos;t let you see that you were deafened or muted.</source>
        <translation>Nastavuje, jestli budete vidět vy sami. Toto nastavení je nejvíce užitečné, pokud nikoho ostatního vidět nechcete, pouze chcete vidět svůj vlastní stav - jestli mluvíte nebo ne, nebo jestli máte zakázáno mluvení/poslech.</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="68"/>
        <source>Show User Textures</source>
        <translation>Ukázat obrázky uživatelů</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="62"/>
        <source>Show User custom textures instead of text on the overlay.</source>
        <translation>Ukázat v overlay vrstvě uživatelův nastavený obrázek místo textu.</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="65"/>
        <source>This sets whether to download and use custom textures for registered users. If disabled, the regular outline text will be used instead.</source>
        <translation>Nastavuje, jestli se má stáhnout a použít navolený obrázek pro zobrazení registrovaných uživatelů. Pokud je zakázáno, zobrazí se uživatelé běžným textem.</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="119"/>
        <source>Grow Left</source>
        <translation>Zvětšovat doleva</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="113"/>
        <source>Let overlay grow to the left</source>
        <translation>Overlay se zvětšuje směrem doleva</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="116"/>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow to the left if needed.</source>
        <translation>Ovelay vrstva je  tak malá, jak je to jenom možné a je na zvolené pozici. Tímto nastavením se povolí zvětšování overlay na levou stranu, pokud je to potřeba.</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="132"/>
        <source>Grow Right</source>
        <translation>Zvětšovat doprava</translation>
    </message>
    <message>
        <source>Let overlay grow to the Right</source>
        <translation type="obsolete">Overlay se zvětšuje směrem doprava</translation>
    </message>
    <message>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow to the Right if needed.</source>
        <translation type="obsolete">Ovelay vrstva je  tak malá, jak je to jenom možné a je na zvolené pozici. Tímto nastavením se povolí zvětšování overlay na pravou stranu, pokud je to potřeba.</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="90"/>
        <source>Grow Up</source>
        <translation>Zvětšovat nahoru</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="84"/>
        <source>Let overlay grow upwards</source>
        <translation>Overlay se zvětšuje směrem nahoru</translation>
    </message>
    <message>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow to the top if needed.</source>
        <translation type="obsolete">Ovelay vrstva je  tak malá, jak je to jenom možné a je na zvolené pozici. Tímto nastavením se povolí zvětšování overlay nahoru, pokud je to potřeba.</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="145"/>
        <source>Grow Down</source>
        <translation>Zvětšovat dolů</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="139"/>
        <source>Let overlay grow downwards</source>
        <translation>Overlay se zvětšuje směrem dolů</translation>
    </message>
    <message>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow towards the bottom if needed.</source>
        <translation type="obsolete">Ovelay vrstva je  tak malá, jak je to jenom možné a je na zvolené pozici. Tímto nastavením se povolí zvětšování overlay směrem dolů, pokud je to potřeba.</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="152"/>
        <source>X-Position of Overlay</source>
        <translation>Pozice overlay na ose X</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="155"/>
        <source>This sets the relative X position of the overlay.</source>
        <translation>Relativni pozice na ose X.</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="97"/>
        <source>Y-Position of Overlay</source>
        <translation>Pozice overlay na ose Y</translation>
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
        <translation>Relativni pozice na ose Y.</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="177"/>
        <source>Current Font</source>
        <translation>Současný font</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="191"/>
        <source>Set Font</source>
        <translation>nastavit font</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="205"/>
        <source>Maximum height of names.</source>
        <translation>Maximální výška jmen.</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="208"/>
        <source>This sets the maximum height of names shown, relative to the screen height. If your active 3D window is 800 pixels tall and this is set to 5%, each name will be 40 pixels tall. Note that the names will not be taller than 60 pixels no matter what you set here.</source>
        <translation>Nastaví maximální výšku zobrazených jmen, relativně k výšce obrazovky. Pokud vaše 3D okno má 800 pixelů na výšku a tato hodnota je nastavena na 5%, každé jméno bude 40 pixelů vysoké. Jména nikdy nebudou větší než 60 pixelů, i kdybyste nastavili vyšší hodnotu.</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="198"/>
        <source>Maximum height</source>
        <translation>Maximální výška</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="238"/>
        <location filename="Overlay.ui" line="259"/>
        <location filename="Overlay.ui" line="280"/>
        <location filename="Overlay.ui" line="301"/>
        <location filename="Overlay.ui" line="322"/>
        <source>Change</source>
        <translation>Změnit</translation>
    </message>
    <message>
        <source>Color for players</source>
        <translation type="obsolete">Barva pro uživatele</translation>
    </message>
    <message>
        <source>Color for talking players</source>
        <translation type="obsolete">Barva pro mluvící uživatele</translation>
    </message>
    <message>
        <source>Color for alt-talking players</source>
        <translation type="obsolete">Barva pro Alt-mluvící uživatele</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="287"/>
        <source>Color for Channels</source>
        <translation>Barva pro kanály</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="308"/>
        <source>Color for active Channels</source>
        <translation>Barva pro aktivní kanály</translation>
    </message>
    <message>
        <location filename="Overlay.cpp" line="142"/>
        <source>Overlay</source>
        <translation>Overlay</translation>
    </message>
    <message>
        <location filename="Overlay.cpp" line="47"/>
        <source>Show no one</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Overlay.cpp" line="48"/>
        <source>Show only talking</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Overlay.cpp" line="49"/>
        <source>Show everyone</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="231"/>
        <location filename="Overlay.cpp" line="122"/>
        <source>Color for users</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="245"/>
        <location filename="Overlay.cpp" line="126"/>
        <source>Color for talking users</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="266"/>
        <location filename="Overlay.cpp" line="130"/>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="87"/>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow upwards if needed.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="126"/>
        <source>Let overlay grow to the right</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="129"/>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow to the right if needed.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="142"/>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow downwards if needed.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="224"/>
        <source>TextLabel</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PlayerModel</name>
    <message>
        <source>Name of player</source>
        <translation type="obsolete">Jméno uživatele</translation>
    </message>
    <message>
        <source>Name of channel</source>
        <translation type="obsolete">Jméno kanálu</translation>
    </message>
    <message>
        <source>Player flags</source>
        <translation type="obsolete">Příznaky uživatele</translation>
    </message>
    <message>
        <source>This is a player connected to the server. The icon to the left of the player indicates whether or not they are talking:&lt;br /&gt;&lt;img src=&quot;:/talking_on.png&quot; /&gt; Talking&lt;br /&gt;&lt;img src=&quot;:/talking_off.png&quot; /&gt; Not talking</source>
        <translation type="obsolete">Tento uživatel je připojen k serveru. Ikonka nalevo od jména názorně indikuje jeho mluvení: &lt;br /&gt;&lt;img src=&quot;:/talking_on.png&quot; /&gt;Mluví&lt;/br /&gt;&lt;img src=&quot;:/talking_off.png&quot; /&gt; Nemluví</translation>
    </message>
    <message>
        <source>This is a channel on the server. Only players in the same channel can hear each other.</source>
        <translation type="obsolete">Toto je komunikační kanál na serveru. Pouze učastníci, kteří jsou ve stejném kanálu, se budou navzájem slyšet.</translation>
    </message>
    <message>
        <source>This shows the flags the player has on the server, if any:&lt;br /&gt;&lt;img src=&quot;:/authenticated.png&quot; /&gt;Authenticated user&lt;br /&gt;&lt;img src=&quot;:/muted_self.png&quot; /&gt;Muted (by self)&lt;br /&gt;&lt;img src=&quot;:/muted_server.png&quot; /&gt;Muted (by admin)&lt;br /&gt;&lt;img src=&quot;:/deafened_self.png&quot; /&gt;Deafened (by self)&lt;br /&gt;&lt;img src=&quot;:/deafened_server.png&quot; /&gt;Deafened (by admin)&lt;br /&gt;A player muted by himself is probably just away, talking on the phone or something like that.&lt;br /&gt;A player muted by an admin is probably also just away, and the noise the player is making was annoying enough that an admin muted him.</source>
        <translation type="obsolete">Zobrazení příznaků uživatele (pokud nějaké má nastaveny):&lt;br /&gt;&lt;img src=&quot;:/authenticated.png&quot; /&gt;Přihlášený uživatel&lt;br /&gt;&lt;img src=&quot;:/muted_self.png&quot; /&gt;Zakázal si mluvení&lt;br /&gt;&lt;img src=&quot;:/muted_server.png&quot; /&gt;Bylo mu zakázáno mluvit&lt;br /&gt;&lt;img src=&quot;:/deafened_self.png&quot; /&gt;Vypnutý poslech&lt;br /&gt;&lt;img src=&quot;:/deafened_server.png&quot; /&gt;Byl mu vypnut poslech&lt;br /&gt;Uživatel, který sám sobě zakázal mluvit nebo poslouchat, je pravděpodobně vzdálen od počítače, nebo zrovna nemá čas na hovory (vaří si kávu)&lt;br /&gt;Uživatel, kterému bylo zakázano mluvit od  administrátora, se pravděpodobně choval nezpůsobně.</translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="obsolete">Jméno</translation>
    </message>
    <message>
        <source>Flags</source>
        <translation type="obsolete">Příznaky</translation>
    </message>
    <message>
        <source>Mumble</source>
        <translation type="obsolete">Mumble</translation>
    </message>
</context>
<context>
    <name>PluginConfig</name>
    <message>
        <location filename="Plugins.ui" line="20"/>
        <source>Options</source>
        <translation type="unfinished">Volby</translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="42"/>
        <location filename="Plugins.cpp" line="67"/>
        <source>Plugins</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="32"/>
        <source>Link to Game and Transmit Position</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="26"/>
        <source>Enable plugins and transmit positional information</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="59"/>
        <source>&amp;Reload plugins</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="53"/>
        <source>Reloads all plugins</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="29"/>
        <source>This allows plugins for supported games to fetch your in-game position and transmit it with each voice packet. This enables other users to hear your voice in-game from the direction your character is in relation to their own.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="56"/>
        <source>This rescans and reloads plugins. Use this if you just added or changed a plugin to the plugins directory.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="85"/>
        <source>&amp;About</source>
        <translation type="unfinished">&amp;O aplikaci</translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="79"/>
        <source>Information about plugin</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="82"/>
        <source>This shows a small information message about the plugin.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="98"/>
        <source>&amp;Configure</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="92"/>
        <source>Show configuration page of plugin</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="95"/>
        <source>This shows the configuration page of the plugin, if any.</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Plugins.cpp" line="107"/>
        <source>Plugin has no about function.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="14"/>
        <source>Form</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Plugins.cpp" line="333"/>
        <source>%1 linked.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PortAudioSystem</name>
    <message>
        <location filename="PAAudio.cpp" line="275"/>
        <source>Default Device</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PulseAudioConfig</name>
    <message>
        <source>%1ms</source>
        <translation type="obsolete">%1ms</translation>
    </message>
    <message>
        <source>Device selection</source>
        <translation type="obsolete">Výběr zařízení</translation>
    </message>
    <message>
        <source>Device to use for microphone</source>
        <translation type="obsolete">Zařízení, na kterém je mikrofon</translation>
    </message>
    <message>
        <source>Output</source>
        <translation type="obsolete">Výstup</translation>
    </message>
    <message>
        <source>Device to use for speakers/headphones</source>
        <translation type="obsolete">Zařízení pro zvukový výstup (bedničky nebo sluchátka)</translation>
    </message>
    <message>
        <source>Output Options</source>
        <translation type="obsolete">Výstupní volby</translation>
    </message>
    <message>
        <source>Output Delay</source>
        <translation type="obsolete">Výstupní opoždění</translation>
    </message>
    <message>
        <source>Amount of data to buffer for ALSA</source>
        <translation type="obsolete">Velikost datového zásobníku pro ALSU</translation>
    </message>
</context>
<context>
    <name>PulseAudioSystem</name>
    <message>
        <location filename="PulseAudio.cpp" line="549"/>
        <source>Default Input</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="PulseAudio.cpp" line="550"/>
        <source>Default Output</source>
        <translation type="unfinished"></translation>
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
        <location filename="ServerHandler.cpp" line="390"/>
        <source>UDP packets cannot be sent to or received from the server. Switching to TCP mode.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ServerHandler.cpp" line="392"/>
        <source>UDP packets cannot be sent to the server. Switching to TCP mode.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ServerHandler.cpp" line="394"/>
        <source>UDP packets cannot be received from the server. Switching to TCP mode.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ServerHandler.cpp" line="401"/>
        <source>UDP packets can be sent to and received from the server. Switching back to UDP mode.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ServerHandler.cpp" line="430"/>
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
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="GlobalShortcut.cpp" line="578"/>
        <source>Toggle</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="GlobalShortcut.cpp" line="584"/>
        <source>Unassigned</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ShortcutKeyWidget</name>
    <message>
        <location filename="GlobalShortcut.cpp" line="150"/>
        <source>Press Shortcut</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ShortcutTargetDialog</name>
    <message>
        <location filename="GlobalShortcut.cpp" line="353"/>
        <source>Root</source>
        <translation type="unfinished">Kořen</translation>
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
        <translation type="unfinished">Kořen</translation>
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
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Tokens.ui" line="57"/>
        <source>Remove a token</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Tokens.ui" line="60"/>
        <source>&amp;Remove</source>
        <translation type="unfinished">Odst&amp;ranit</translation>
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
        <translation type="unfinished">Odstranit</translation>
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
        <translation type="unfinished">Jméno</translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="596"/>
        <source>Flags</source>
        <translation type="unfinished">Příznaky</translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="1245"/>
        <location filename="UserModel.cpp" line="1335"/>
        <location filename="UserModel.cpp" line="1355"/>
        <source>Mumble</source>
        <translation type="unfinished">Mumble</translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="1245"/>
        <source>Are you sure you want to drag this channel?</source>
        <translation type="unfinished"></translation>
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
        <source>Mumble</source>
        <translation type="obsolete">Mumble</translation>
    </message>
    <message>
        <location filename="VersionCheck.cpp" line="97"/>
        <source>Mumble failed to retrieve version information from the SourceForge server.</source>
        <translation>Mumble nedokázalo získat informace o aktuálních verzích ze serveru SourceForge.</translation>
    </message>
</context>
<context>
    <name>ViewCert</name>
    <message>
        <location filename="ViewCert.cpp" line="36"/>
        <source>Certificate Chain Details</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="42"/>
        <source>Certificate chain</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="51"/>
        <source>Certificate details</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="48"/>
        <source>%1 %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="76"/>
        <location filename="ViewCert.cpp" line="89"/>
        <source>Common Name: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="77"/>
        <location filename="ViewCert.cpp" line="90"/>
        <source>Organization: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="78"/>
        <source>Subunit: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="79"/>
        <location filename="ViewCert.cpp" line="92"/>
        <source>Country: %1</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="82"/>
        <source>Valid from: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="83"/>
        <source>Valid to: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="84"/>
        <source>Serial: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="85"/>
        <source>Public Key: %1 bits %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="85"/>
        <source>RSA</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="85"/>
        <source>DSA</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="86"/>
        <source>Digest (MD5): %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="88"/>
        <source>Issued by:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="91"/>
        <source>Unit Name: %1</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>WASAPIConfig</name>
    <message>
        <source>Device selection</source>
        <translation type="obsolete">Výběr zařízení</translation>
    </message>
    <message>
        <source>Device to use for microphone</source>
        <translation type="obsolete">Zařízení, na kterém je mikrofon</translation>
    </message>
    <message>
        <source>Output</source>
        <translation type="obsolete">Výstup</translation>
    </message>
    <message>
        <source>Device to use for speakers/headphones</source>
        <translation type="obsolete">Zařízení pro zvukový výstup (bedničky nebo sluchátka)</translation>
    </message>
</context>
<context>
    <name>WASAPISystem</name>
    <message>
        <location filename="WASAPI.cpp" line="177"/>
        <source>Default Device</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>XInputKeyWidget</name>
    <message>
        <source>Press Shortcut</source>
        <translation type="obsolete">Vyberte klávesovou zkratku</translation>
    </message>
</context>
</TS>
