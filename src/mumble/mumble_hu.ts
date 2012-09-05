<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.0" language="hu_HU">
<context>
    <name>ACLEditor</name>
    <message>
        <location filename="ACLEditor.cpp" line="88"/>
        <source>Mumble - Edit %1</source>
        <translation>Mumble - Szerkesztés: %1</translation>
    </message>
    <message>
        <location filename="ACLEditor.cpp" line="100"/>
        <source>Deny</source>
        <translation>Tilt</translation>
    </message>
    <message>
        <location filename="ACLEditor.cpp" line="102"/>
        <source>Allow</source>
        <translation>Enged</translation>
    </message>
    <message>
        <location filename="ACLEditor.cpp" line="124"/>
        <source>Allow %1</source>
        <translation>Enged: %1</translation>
    </message>
    <message>
        <location filename="ACLEditor.cpp" line="115"/>
        <source>Deny %1</source>
        <translation>Tilt: %1</translation>
    </message>
    <message>
        <location filename="ACLEditor.cpp" line="54"/>
        <source>Mumble - Add channel</source>
        <translation>Mumble - Csatorna hozzáadása</translation>
    </message>
    <message>
        <location filename="ACLEditor.cpp" line="76"/>
        <location filename="ACLEditor.cpp" line="238"/>
        <source>Failed: Invalid channel</source>
        <translation>Hiba: Érvénytelen csatorna</translation>
    </message>
    <message>
        <location filename="ACLEditor.cpp" line="125"/>
        <source>This grants the %1 privilege. If a privilege is both allowed and denied, it is denied.&lt;br /&gt;%2</source>
        <translation>Ez biztosítja a jogot ehhez: %1. Ha egy jogosultságnál mindkettő ki van pipálva, akkor le van tiltva.&lt;br /&gt;%2</translation>
    </message>
    <message>
        <location filename="ACLEditor.cpp" line="116"/>
        <source>This revokes the %1 privilege. If a privilege is both allowed and denied, it is denied.&lt;br /&gt;%2</source>
        <translation>Ez vonja meg a jogot ettől: %1. Ha egy jogosultságnál mindkettő ki van pipálva, akkor le van tiltva.&lt;br /&gt;%2</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="14"/>
        <source>Dialog</source>
        <translation>Párbeszéd</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="24"/>
        <source>&amp;Properties</source>
        <translation>&amp;Tulajdonságok</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="33"/>
        <source>Name</source>
        <translation>Név</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="40"/>
        <source>Enter the channel name here.</source>
        <translation>Adja meg a csatorna nevét.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="43"/>
        <source>&lt;b&gt;Name&lt;/b&gt;&lt;br&gt;Enter the channel name in this field. The name has to comply with the restriction imposed by the server you are connected to.</source>
        <translation>&lt;b&gt;Name&lt;/b&gt;Írd be ebbe a mezőbe a csatorna nevét. A név eleget kell tegyen annak a szervernek a megkötéseivel, amelyhez kapcsolódsz.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="50"/>
        <source>Description</source>
        <translation>Leírás</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="57"/>
        <source>Password</source>
        <translation>Jelszó</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="64"/>
        <source>Enter the channel password here.</source>
        <translation>Adja meg a csatorna jelszavát.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="67"/>
        <source>&lt;b&gt;Password&lt;/b&gt;&lt;br&gt;This field allows you to easily set and change the password of a channel. In the background it uses Mumble&apos;s access tokens feature. To allow more fine grained and powerful access control directly use ACLs and groups instead (&lt;i&gt;Advanced configuration&lt;/i&gt; has to be checked to be able to see these settings).</source>
        <translation>&lt;b&gt;Jelszó&lt;/b&gt;Ennek a mezőnek a segítségével könnyen beállíthatja vagy módosíthatja a csatorna jelszavát. A háttérben használja a Mumble token elérési képességét. Az elérések finomabb és hathatósabb szabályozásához e helyett használja a hozzáférést szabályzó listát (ACL) és csoport beállításokat (&lt;i&gt;Haladó beállítások&lt;/i&gt; ki kell legyen jelölve ahhoz, hogy láthassa ezeket a beállításokat).</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="74"/>
        <source>Check to create a temporary channel.</source>
        <translation>Jelölje ki ideiglenes csatorna létrehozásához.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="77"/>
        <source>&lt;b&gt;Temporary&lt;/b&gt;&lt;br&gt;
When checked the channel created will be marked as temporary. This means when the last player leaves it the channel will be automatically deleted by the server.</source>
        <translation>&lt;b&gt;Ideiglenes&lt;/b&gt;&lt;br&gt;
Ha ki van jelölve, akkor a létrehozott csatorna ideiglenesként lesz megjelölve. Ez azt jelenti, hogy amikor az utolsó játékos is elhagyja ezt a csatornát, a szerver automatikusan törölni fogja.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="81"/>
        <source>Temporary</source>
        <translation>Ideiglenes</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="94"/>
        <source>Channel positioning facility value</source>
        <translation>Csatorna sorrendiséget meghatározó érték</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="97"/>
        <source>&lt;b&gt;Position&lt;/b&gt;&lt;br/&gt;
This value enables you to change the way Mumble arranges the channels in the tree. A channel with a higher &lt;i&gt;Position&lt;/i&gt; value will always be placed below one with a lower value and the other way around. If the &lt;i&gt;Position&lt;/i&gt; value of two channels is equal they will get sorted alphabetically by their name.</source>
        <translation>&lt;b&gt;Sorrend&lt;b&gt;&lt;br/&gt;
Ezzel az értékkel módosíthatja azt a sorrendet, ahogy a Mumble egy faszerkezetbe elrendezi a csatornákat. Egy magasabb &lt;i&gt;Sorrend&lt;/i&gt; értékkel rendelkező csatorna mindig lejjebb lesz elhelyezve, mint az alacsonyabb értékkel rendelkezők. Ha a &lt;i&gt;Sorrend&lt;/i&gt; értéke egyforma két csatornánál, akkor az abc sorrendbe kerülnek a nevük alapján.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="108"/>
        <source>Position</source>
        <translation>Sorrend</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="126"/>
        <source>&amp;Groups</source>
        <translation>&amp;Csoportok</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="132"/>
        <location filename="ACLEditor.ui" line="560"/>
        <source>Group</source>
        <translation>Csoport</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="156"/>
        <source>List of groups</source>
        <translation>Csoportok listája</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="184"/>
        <source>Remove selected group</source>
        <translation>Kiválasztott csoport eltávolítása</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="190"/>
        <location filename="ACLEditor.ui" line="368"/>
        <location filename="ACLEditor.ui" line="375"/>
        <source>Remove</source>
        <translation>Eltávolítás</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="200"/>
        <source>Inherit group members from parent</source>
        <translation>Csoport tagjainak öröklése a szülőtől</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="206"/>
        <source>Inherit</source>
        <translation>Öröklés</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="213"/>
        <source>Make group inheritable to sub-channels</source>
        <translation>Csoport örökölhetővé tétele az alcsatornák számára</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="219"/>
        <source>Inheritable</source>
        <translation>Örökölhető</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="229"/>
        <source>Group was inherited from parent channel</source>
        <translation>A csoport örökölve lett a szülő csatornától</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="235"/>
        <source>Inherited</source>
        <translation>Örökölt</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="245"/>
        <location filename="ACLEditor.ui" line="251"/>
        <source>Members</source>
        <translation>Tagok</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="177"/>
        <location filename="ACLEditor.ui" line="329"/>
        <location filename="ACLEditor.ui" line="361"/>
        <source>Add</source>
        <translation>Hozzáadás</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="310"/>
        <source>Add member to group</source>
        <translation>Tag hozzáadása a csoporthoz</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="342"/>
        <source>Remove member from group</source>
        <translation>Tag eltávolítása a csoportból</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="403"/>
        <source>&amp;ACL</source>
        <translation>&amp;ACL</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="411"/>
        <source>Active ACLs</source>
        <translation>Aktív ACL-ek</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="417"/>
        <source>List of entries</source>
        <translation>Bejegyzések listája</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="427"/>
        <source>Inherit ACL of parent?</source>
        <translation>Hozzáférést szabályzó lista (ACL) öröklése szülőtől?</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="430"/>
        <source>This sets whether or not the ACL up the chain of parent channels are applied to this object. Only those entries that are marked in the parent as &quot;Apply to sub-channels&quot; will be inherited.</source>
        <translation>Ezzel beállítható, hogy a szülőcsatornák láncolatában levő hozzáférést szabályzó listát (ACL) alkalmazza-e vagy sem erre az elemre. Csak a szülők &quot;Alkalmazza az alcsatornákra&quot; beállítással ellátott bejegyzéseit fogja örökölni.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="433"/>
        <source>Inherit ACLs</source>
        <translation>ACL öröklése</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="440"/>
        <source>Move entry up</source>
        <translation>Bejegyzés mozgatása fel</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="443"/>
        <source>This moves the entry up in the list. As entries are evaluated in order, this may change the effective permissions of users. You cannot move an entry above an inherited entry, if you really need that you&apos;ll have to duplicate the inherited entry.</source>
        <translation>A listában felfele mozgatja a bejegyzést. Mivel a bejegyzéseket a sorrend szerint értékeli ki, a mozgatással megváltozhatnak a felhasználó tényleges jogai. Egy örökölt bejegyzés fölé nem vihető egy másik, ha tényleg szükség van rá, akkor az örökölt bejegyzést le kell másolni.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="446"/>
        <source>&amp;Up</source>
        <translation>&amp;Fel</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="456"/>
        <source>Move entry down</source>
        <translation>Bejegyzés mozgatása le</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="459"/>
        <source>This moves the entry down in the list. As entries are evaluated in order, this may change the effective permissions of users.</source>
        <translation>A listában lefele mozgatja a bejegyzést. Mivel a bejegyzéseket a sorrend szerint értékeli ki, a mozgatással megváltozhatnak a felhasználó tényleges jogai.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="462"/>
        <source>&amp;Down</source>
        <translation>&amp;Le</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="472"/>
        <source>Add new entry</source>
        <translation>Új bejegyzés hozzáadása</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="475"/>
        <source>This adds a new entry, initially set with no permissions and applying to all.</source>
        <translation>Hozzáad egy új bejegyzést, jogok nélküli és mindenkire érvényes kezdeti beállítással.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="478"/>
        <source>&amp;Add</source>
        <translation>&amp;Hozzáadás</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="488"/>
        <source>Remove entry</source>
        <translation>Bejegyzés eltávolítása</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="491"/>
        <source>This removes the currently selected entry.</source>
        <translation>Eltávolítja a jelenleg kijelölt bejegyzést.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="494"/>
        <source>&amp;Remove</source>
        <translation>&amp;Eltávolítás</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="513"/>
        <source>Context</source>
        <translation>Összefüggések</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="535"/>
        <source>This makes the entry apply to this channel.</source>
        <translation>Alkalmazza a bejegyzést erre a csatornára.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="538"/>
        <source>Applies to this channel</source>
        <translation>Alkalmazás erre a mappára</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="519"/>
        <source>Entry should apply to sub-channels.</source>
        <translation>A bejegyzést alkalmazni kell az alcsatornákra is.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="159"/>
        <source>&lt;b&gt;Group&lt;/b&gt;&lt;br&gt;
These are all the groups currently defined for the channel. To create a new group, just type in the name and press enter.</source>
        <translation>&lt;b&gt;Csoport&lt;/b&gt;&lt;br&gt;
Ez a csatorna számára jelenleg meghatározott összes csoport. Új csoport létrehozásához csak írja be a nevét és üssön egy enter-t.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="170"/>
        <source>Add new group</source>
        <translation>Új csoport hozzáadása</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="173"/>
        <source>&lt;b&gt;Add&lt;/b&gt;&lt;br/&gt;
Add a new group.</source>
        <translation>&lt;b&gt;Hozzáad&lt;/b&gt;&lt;br/&gt;
Hozzáad egy új csoportot.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="187"/>
        <source>&lt;b&gt;Remove&lt;/b&gt;&lt;br&gt;This removes the currently selected group. If the group was inherited, it will not be removed from the list, but all local information about the group will be cleared.</source>
        <translation>&lt;b&gt;Eltávolít&lt;/b&gt;&lt;br&gt;Eltávolítja a jelenleg kijelölt csoportot. Ha a csoport örökölt, nem lesz eltávolítva a listáról, csak a rá vonatkozó összes helyi információ törlődik.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="203"/>
        <source>&lt;b&gt;Inherit&lt;/b&gt;&lt;br&gt;This inherits all the members in the group from the parent, if the group is marked as &lt;i&gt;Inheritable&lt;/i&gt; in the parent channel.</source>
        <translation>&lt;b&gt;Örököl&lt;/b&gt;&lt;br&gt;Ha a szülő csatornában a csoport &lt;i&gt;Örökölhető&lt;/i&gt;ként van megjelölve, akkor csoport minden tagja öröklődik a szülőtől.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="216"/>
        <source>&lt;b&gt;Inheritable&lt;/b&gt;&lt;br&gt;This makes this group inheritable to sub-channels. If the group is non-inheritable, sub-channels are still free to create a new group with the same name.</source>
        <translation>&lt;b&gt;Örökölhető&lt;/b&gt;&lt;br&gt;Ezt a csoportot örökölhetővé teszi az alcsatornák számára. Ha a csoport nem örökölhető, az alcsatornáknak akkor is megengedett, hogy létrehozzanak ugyanazzal a névvel egy új csoportot.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="232"/>
        <source>&lt;b&gt;Inherited&lt;/b&gt;&lt;br&gt;This indicates that the group was inherited from the parent channel. You cannot edit this flag, it&apos;s just for information.</source>
        <translation>&lt;b&gt;Öröklött&lt;/b&gt;&lt;br&gt;Azt jelenti, hogy a csoport a szülőcsatornától öröklődött. Ez nem egy szerkeszthető jelzés, csak információ.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="261"/>
        <source>Inherited members</source>
        <translation>Örökölt tagok</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="271"/>
        <source>Contains the list of members added to the group by this channel.</source>
        <translation>A csatorna által a csoporthoz hozzáadott tagok listáját tartalmazza.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="274"/>
        <source>&lt;b&gt;Members&lt;/b&gt;&lt;br&gt;
This list contains all members that were added to the group by the current channel. Be aware that this does not include members inherited by higher levels of the channel tree. These can be found in the &lt;i&gt;Inherited members&lt;/i&gt; list. To prevent this list to be inherited by lower level channels uncheck &lt;i&gt;Inheritable&lt;/i&gt; or manually add the members to the &lt;i&gt;Excluded members&lt;/i&gt; list.</source>
        <translation>&lt;b&gt;Tagok&lt;/b&gt;&lt;br&gt;
Ez a lista a jelenlegi csatorna által a csoporthoz hozzáadott minden tagot tartalmazza. Tudjon róla, hogy ez a lista nem tartalmazza azokat a tagokat, amelyek a csatornák faszerkezetében levő magasabb szintekről öröklődtek. Ezek megtalálhatók az &lt;i&gt;Öröklött tagok&lt;/i&gt; listájában. Hogy elkerülje ennek a listának az öröklését az alacsonyabb szinteken levő csatornákon szüntesse meg a kijelölést az &lt;i&gt;Örökölhető&lt;/i&gt; pontban vagy a tagokat manuálisan adja hozzá a &lt;i&gt;Kizárt tagok&lt;/i&gt; listájához.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="282"/>
        <source>Contains a list of members whose group membership will not be inherited from the parent channel.</source>
        <translation>Azoknak a tagoknak a listáját tartalmazza, amelyek csoporttagsága nem fog öröklődni a szülőcsatornából.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="285"/>
        <source>&lt;b&gt;Excluded members&lt;/b&gt;&lt;br&gt;
Contains a list of members whose group membership will not be inherited from the parent channel.</source>
        <translation>&lt;b&gt;Kizárt tagok&lt;/b&gt;&lt;br&gt;
Azoknak a tagoknak a listáját tartalmazza, amelyek csoporttagsága nem fog öröklődni a szülőcsatornából.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="293"/>
        <source>Contains the list of members inherited by other channels.</source>
        <translation>Azoknak a tagoknak a listáját tartalmazza, amelyek más csatornákból öröklődtek.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="296"/>
        <source>&lt;b&gt;Inherited members&lt;/b&gt;&lt;br&gt;
Contains the list of members inherited by the current channel. Uncheck &lt;i&gt;Inherit&lt;/i&gt; to prevent inheritance from higher level channels.</source>
        <translation>&lt;b&gt;Öröklött tagok&lt;/b&gt;&lt;br&gt;
Azoknak a tagoknak a listáját tartalmazza, amelyek a jelenlegi csatornából öröklődtek. Szüntesse meg a kijekölést az &lt;i&gt;Örököl&lt;/i&gt; pontban, hogy elkerülje a magasabb csatornákból való öröklődést.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="313"/>
        <source>Type in the name of a user you wish to add to the group and click Add.</source>
        <translation>Írja be a felhasználó nevét, amelyet hozzá szeretne adni a csoporthoz és kattintson a Hozzáadásra.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="345"/>
        <source>Type in the name of a user you wish to remove from the group and click Add.</source>
        <translation>Írja be a felhasználó nevét, amelyet el szeretne távolítani a csoportból és kattintson az Eltávolításra.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="382"/>
        <source>Exclude</source>
        <translation>Kizárás</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="389"/>
        <source>Excluded members</source>
        <translation>Kizárt tagok</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="420"/>
        <source>This shows all the entries active on this channel. Entries inherited from parent channels will be show in italics.&lt;br /&gt;ACLs are evaluated top to bottom, meaning priority increases as you move down the list.</source>
        <translation>Ezen a csatornán levő összes aktív bejegyzést mutatja. A szülőcsatornáktól örökölt bejegyzéseket dőlt betűvel írja ki. &lt;br /&gt;A hozzáférést szabályzó listákat (ACL) fentről lefele értékeli ki, azaz lefele haladva nő az ACL-ek fontossága.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="522"/>
        <source>This makes the entry apply to sub-channels of this channel.</source>
        <translation>Ennek a csatornának az alcsatornáira alkalmazza ezt a bejegyzést.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="525"/>
        <source>Applies to sub-channels</source>
        <translation>Alkalmazás az almappákra</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="532"/>
        <source>Entry should apply to this channel.</source>
        <translation>A bejegyzést erre a csatornára alkalmazza.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="579"/>
        <source>This controls which group of users this entry applies to.&lt;br /&gt;Note that the group is evaluated in the context of the channel the entry is used in. For example, the default ACL on the Root channel gives &lt;i&gt;Write&lt;/i&gt; permission to the &lt;i&gt;admin&lt;/i&gt; group. This entry, if inherited by a channel, will give a user write privileges if he belongs to the &lt;i&gt;admin&lt;/i&gt; group in that channel, even if he doesn&apos;t belong to the &lt;i&gt;admin&lt;/i&gt; group in the channel where the ACL originated.&lt;br /&gt;If a group name starts with a &apos;!&apos;, its membership is negated, and if it starts with a &apos;~&apos;, it is evaluated in the channel the ACL was defined in, rather than the channel the ACL is active in.&lt;br /&gt;If a group name starts with a  &apos;#&apos;, it is interpreted as an access token. Users must have entered whatever follows the &apos;#&apos; in their list of access tokens to match. This can be used for very simple password access to channels for non-authenticated users.&lt;br /&gt;If a group name starts with a  &apos;$&apos;, it will only match users whose certificate hash matches what follows the &apos;$&apos;.&lt;br /&gt;A few special predefined groups are:&lt;br /&gt;&lt;b&gt;all&lt;/b&gt; - Everyone will match.&lt;br /&gt;&lt;b&gt;auth&lt;/b&gt; - All authenticated users will match.&lt;br /&gt;&lt;b&gt;sub,a,b,c&lt;/b&gt; - User currently in a sub-channel minimum &lt;i&gt;a&lt;/i&gt; common parents, and between &lt;i&gt;b&lt;/i&gt; and &lt;i&gt;c&lt;/i&gt; channels down the chain. See the website for more extensive documentation on this one.&lt;br /&gt;&lt;b&gt;in&lt;/b&gt; - Users currently in the channel will match (convenience for &apos;&lt;i&gt;sub,0,0,0&lt;/i&gt;&apos;).&lt;br /&gt;&lt;b&gt;out&lt;/b&gt; - Users outside the channel will match (convenience for &apos;&lt;i&gt;!sub,0,0,0&lt;/i&gt;&apos;).&lt;br /&gt;Note that an entry applies to either a user or a group, not both.</source>
        <translation>Vezérli, hogy a felhasználók melyik csoportjára vonatkozik ez a bejegyzés.&lt;br /&gt;Megjegyzés képpen, a csoport azzal a csatornával összefüggésben lesz kiértékelve amelyben a bejegyzés van.Például: az alapértelmezett ACL a gyökér csatornában &lt;i&gt;Írás&lt;/i&gt; jogot ad az &lt;i&gt;admin&lt;/i&gt; csoportnak. Ez a bejegyzést, ha örököli egy csatorna, akkor egy &lt;i&gt;admin&lt;/i&gt; csoporthoz tartozó felhasználónak írási jogot fog adni ebben a csatornában még akkor is, ha az eredeti csatornában, ahonnan az ACL származik, nem tartozik az &lt;i&gt;admin&lt;/i&gt; csoporthoz.&lt;br /&gt;Ha egy csoportnév &apos;!&apos; jellel kezdődik, akkor a tagsága fordítottja lesz az igaz, és ha egy &apos;~&apos; jellel kezdődik, akkor a kiértékelése abban a csatornában történik, ahol az ACL meghatározásra került, nem pedig abban a csatornában ahol az ACL aktív.&lt;br /&gt;Ha egy csoportnév egy &apos;#&apos; jellel kezdődik, akkor a program egy hozzáférési tokennek értelmezi. A felhasználók a hozzáférési tokenlistájukban meg kell adják azt ami a &apos;#&apos; jel után van, bármi legyen is az. Ez a nem hitelesített felhasználók számára egy nagyon egyszerű, jelszóval védett belépési lehetőséget biztosít a csatornákra.&lt;br /&gt;Ha egy csoportnév egy &apos;$&apos; jellel kezdődik, akkor csak azok a felhasználók lesznek engedélyezve, amelyek tanúsítványának a kivonata (hash) egyezik a &apos;$&apos; jel utáni résszel.&lt;br /&gt;Néhány speciális előre meghatározott csoport létezik:&lt;br /&gt;&lt;b&gt;all&lt;/b&gt; - Mindenkire vonatkozik.&lt;br /&gt;&lt;b&gt;auth&lt;/b&gt; - Minden hitelesített felhasználóra vonatkozik.&lt;br /&gt;&lt;b&gt;sub,a,b,c&lt;/b&gt; - Jelenleg egy alcsatornában levő felhasználókra vonatkozik, amelyeknek minimálisan &lt;i&gt;a&lt;/i&gt; közösek a szülőik, és &lt;i&gt;b&lt;/i&gt; és &lt;i&gt;c&lt;/i&gt; csatornák között, a csatornaláncban lefelé helyezkednek el. Erről sokkal bővebb dokumentációt a weboldalon talál.&lt;br /&gt;&lt;b&gt;in&lt;/b&gt; - A csatornában jelenleg benntartózkodó felhasználókra vonatkozik (megfelel a következőnek: &apos;&lt;i&gt;sub,0,0,0&lt;/i&gt;&apos;).&lt;br /&gt;&lt;b&gt;out&lt;/b&gt; - A csatornán kívül levő felhasználókra vonatkozik (megfelel a következőnek: &apos;&lt;i&gt;!sub,0,0,0&lt;/i&gt;&apos;).&lt;br /&gt;Megjegyzés képpen, egy bejegyzés vagy egy felhasználóra vagy egy csoportra vonatkozik, de nem mindkettőre.</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="626"/>
        <source>Permissions</source>
        <translation>Jogosultságok</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="554"/>
        <source>User/Group</source>
        <translation>Felhasználó/Csoport</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="576"/>
        <source>Group this entry applies to</source>
        <translation>Csoport, amire ez a bejegyzés vonatkozik</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="589"/>
        <source>User ID</source>
        <translation>Felhasználó azonosító</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="605"/>
        <source>User this entry applies to</source>
        <translation>Felhasználó, amire ez a bejegyzés vonatkozik</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui" line="608"/>
        <source>This controls which user this entry applies to. Just type in the user name and hit enter to query the server for a match.</source>
        <translation>Ezzel ellenőrizheti, hogy melyik felhasználóra érvényes ez a bejegyzés. Csak írja be a felhasználó nevét és üssön enter-t a szerver lekérdezéséhez.</translation>
    </message>
</context>
<context>
    <name>ALSAAudioInput</name>
    <message>
        <location filename="ALSAAudio.cpp" line="172"/>
        <source>Default ALSA Card</source>
        <translation>Alapértelmezett ALSA kártya</translation>
    </message>
    <message>
        <location filename="ALSAAudio.cpp" line="363"/>
        <source>Opening chosen ALSA Input failed: %1</source>
        <translation>A kiválasztott ALSA bemenet megnyitása sikertelen: %1</translation>
    </message>
</context>
<context>
    <name>ALSAAudioOutput</name>
    <message>
        <location filename="ALSAAudio.cpp" line="173"/>
        <source>Default ALSA Card</source>
        <translation>Alapértelmezett ALSA kártya</translation>
    </message>
    <message>
        <location filename="ALSAAudio.cpp" line="498"/>
        <source>Opening chosen ALSA Output failed: %1</source>
        <translation>A kiválasztott ALSA kimenet megnyitása sikertelen: %1</translation>
    </message>
</context>
<context>
    <name>ASIOConfig</name>
    <message>
        <location filename="ASIOInput.cpp" line="224"/>
        <source>%1 (version %2)</source>
        <translation>%1 (verzió: %2)</translation>
    </message>
    <message>
        <location filename="ASIOInput.cpp" line="227"/>
        <source>%1 -&gt; %2 samples buffer, with %3 sample resolution (%4 preferred) at %5 Hz</source>
        <translation>%1 -&gt; %2 puffer méret %3 minta bitmélységgel (%4 preferált) %5 Hz-nél</translation>
    </message>
    <message>
        <location filename="ASIOInput.cpp" line="269"/>
        <location filename="ASIOInput.cpp" line="296"/>
        <source>ASIO Initialization failed: %1</source>
        <translation>ASIO inicializálása nem sikerült: %1</translation>
    </message>
    <message>
        <location filename="ASIOInput.cpp" line="273"/>
        <location filename="ASIOInput.cpp" line="300"/>
        <source>Failed to instantiate ASIO driver</source>
        <translation>ASIO meghajtó példányosítása nem sikerült</translation>
    </message>
    <message>
        <location filename="ASIOInput.cpp" line="337"/>
        <source>ASIO</source>
        <translation>ASIO</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="14"/>
        <source>Form</source>
        <translation>Űrlap</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="20"/>
        <source>Device selection</source>
        <translation>Eszköz választó</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="26"/>
        <source>Device</source>
        <translation>Eszköz</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="42"/>
        <source>Device to use for microphone</source>
        <translation>Eszköz a mikrofon használatához</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="45"/>
        <source>This chooses what device to query. You still need to actually query the device and select which channels to use.</source>
        <translation>Ezzel kiválaszthatja, hogy melyik eszközt kérdezze. De utána még önnek kell lekérdeznie az eszközt és ki kell választania a használni kívánt csatornákat.</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="52"/>
        <source>Query selected device</source>
        <translation>Kiválasztott eszköz lekérdezése</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="55"/>
        <source>This queries the selected device for channels. Be aware that many ASIO drivers are buggy to the extreme, and querying them might cause a crash of either the application or the system.</source>
        <translation>Ezzel lekérdezhetőek a kiválasztott eszköz csatornái. Figyelem, sok ASIO meghajtó rendkívül sok hibát tartalmaz, ezért a lekérdezésük az alkalmazás vagy akár a rendszer összeomlásához vezethet.</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="58"/>
        <source>&amp;Query</source>
        <translation>&amp;Lekérdezés</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="65"/>
        <source>Configure selected device</source>
        <translation>Kiválasztott eszköz beállítása</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="68"/>
        <source>This configures the selected device. Be aware that many ASIO drivers are buggy to the extreme, and querying them might cause a crash of either the application or the system.</source>
        <translation>Ezzel beállítható a kiválasztott eszköz. Figyelem, sok ASIO meghajtó rendkívül sok hibát tartalmaz, ezért a lekérdezésük az alkalmazás vagy akár a rendszer összeomlásához vezethet.</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="71"/>
        <source>&amp;Configure</source>
        <translation>&amp;Beállítás</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="81"/>
        <source>Capabilities</source>
        <translation>Képességek</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="87"/>
        <source>Driver name</source>
        <translation>Meghajtó neve</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="107"/>
        <source>Buffer size</source>
        <translation>Puffer mérete</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="133"/>
        <source>This will configure the input channels for ASIO. Make sure you select at least one channel as microphone and speaker. &lt;i&gt;Microphone&lt;/i&gt; should be where your microphone is attached, and &lt;i&gt;Speaker&lt;/i&gt; should be a channel that samples &apos;&lt;i&gt;What you hear&lt;/i&gt;&apos;.&lt;br /&gt;For example, on the Audigy 2 ZS, a good selection for Microphone would be &apos;&lt;i&gt;Mic L&lt;/i&gt;&apos; while Speaker should be &apos;&lt;i&gt;Mix L&lt;/i&gt;&apos; and &apos;&lt;i&gt;Mix R&lt;/i&gt;&apos;.</source>
        <translation>Ezzel beállíthatóak az ASIO számára a bemeneti csatornák. Bizonyosodjon meg arról, hogy kiválasztott legalább egy csatornát a mikrofonnak és a hangszórónak. A &lt;i&gt;mikrofon&lt;/i&gt; csatornája ott kell legyen, ahova az ön mikrofonja van csatlakoztatva, a &lt;i&gt;hangszóró&lt;/i&gt; pedig az a csatorna kell legyen &apos;&lt;i&gt;amit ön hall&lt;/i&gt;&apos;.&lt;br /&gt;Például, az Audigy 2 ZS esetében egy jó választás lehet: a mikrofon &apos;&lt;i&gt;Mic L&lt;/i&gt;&apos;, míg a hangszóró&apos;&lt;i&gt;Mix L&lt;/i&gt;&apos; és &apos;&lt;i&gt;Mix R&lt;/i&gt;&apos;.</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="130"/>
        <source>Configure input channels</source>
        <translation>Bemeneti csatornák beállítása</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="136"/>
        <source>Channels</source>
        <translation>Csatornák</translation>
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
        <translation>Használaton kívül</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui" line="329"/>
        <source>Speakers</source>
        <translation>Hangszórók</translation>
    </message>
</context>
<context>
    <name>ASIOInput</name>
    <message>
        <location filename="ASIOInput.cpp" line="417"/>
        <source>You need to select at least one microphone and one speaker source to use ASIO. If you just need microphone sampling, use DirectSound.</source>
        <translation>Az ASIO használatához válasszon ki legalább egy mikrofont és hangszórót. Ha csak a mikrofon mintavételezésére van szüksége, használja a DirectSound-t.</translation>
    </message>
    <message>
        <location filename="ASIOInput.cpp" line="520"/>
        <source>Opening selected ASIO device failed. No input will be done.</source>
        <translation>A kiválasztott ASIO eszköz megnyitása nem sikerült. Nem lesz bemenet feldolgozva.</translation>
    </message>
</context>
<context>
    <name>AboutDialog</name>
    <message>
        <location filename="About.cpp" line="37"/>
        <source>About Mumble</source>
        <translation>Névjegy: Mumble</translation>
    </message>
    <message>
        <location filename="About.cpp" line="53"/>
        <source>&lt;h3&gt;Mumble (%1)&lt;/h3&gt;&lt;p&gt;Copyright %3 Thorvald Natvig&lt;br /&gt;slicer@users.sourceforge.net&lt;/p&gt;&lt;p&gt;&lt;b&gt;A voice-chat utility for gamers&lt;/b&gt;&lt;/p&gt;&lt;p&gt;&lt;tt&gt;&lt;a href=&quot;%2&quot;&gt;%2&lt;/a&gt;&lt;/tt&gt;&lt;/p&gt;</source>
        <translation>&lt;h3&gt;Mumble (%1)&lt;/h3&gt;&lt;p&gt;Copyright %3 Thorvald Natvig&lt;br /&gt;slicer@users.sourceforge.net&lt;/p&gt;&lt;p&gt;&lt;b&gt;Egy beszélgető program játékosoknak&lt;/b&gt;&lt;/p&gt;&lt;p&gt;&lt;tt&gt;&lt;a href=&quot;%2&quot;&gt;%2&lt;/a&gt;&lt;/tt&gt;&lt;/p&gt;</translation>
    </message>
    <message>
        <location filename="About.cpp" line="63"/>
        <source>&amp;About Mumble</source>
        <translation>&amp;Névjegy: Mumble</translation>
    </message>
    <message>
        <location filename="About.cpp" line="64"/>
        <source>&amp;License</source>
        <translation>&amp;Licence</translation>
    </message>
    <message>
        <location filename="About.cpp" line="66"/>
        <source>OK</source>
        <translation>OK</translation>
    </message>
</context>
<context>
    <name>AboutSpeexDialog</name>
    <message>
        <location filename="About.cpp" line="74"/>
        <source>About Speex</source>
        <translation>Speex névjegye</translation>
    </message>
    <message>
        <location filename="About.cpp" line="78"/>
        <source>&lt;h3&gt;About Speex&lt;/h3&gt;&lt;p&gt;&lt;tt&gt;&lt;a href=&quot;%1&quot;&gt;%1&lt;/a&gt;&lt;/tt&gt;&lt;/p&gt;&lt;p&gt;This program uses SpeexDSP.&lt;/p&gt;&lt;p&gt;Speex is used for echo cancellation, noise&lt;br /&gt;filtering and voice activity detection.&lt;/p&gt;</source>
        <translation>&lt;h3&gt;Speex névjegye&lt;/h3&gt;&lt;p&gt;&lt;tt&gt;&lt;a href=&quot;%1&quot;&gt;%1&lt;/a&gt;&lt;/tt&gt;&lt;/p&gt;&lt;p&gt;Ez a program a SpeexDSP-t használja.&lt;/p&gt;&lt;p&gt;A Speex a visszhang és a zajok&lt;br /&gt;kiszűrésére, a beszéd érzékelésére szolgál.&lt;/p&gt;</translation>
    </message>
    <message>
        <location filename="About.cpp" line="86"/>
        <source>OK</source>
        <translation>Rendben</translation>
    </message>
</context>
<context>
    <name>AudioInput</name>
    <message>
        <location filename="AudioInput.ui" line="14"/>
        <source>Form</source>
        <translation>Űrlap</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="20"/>
        <source>Interface</source>
        <translation>Interfész</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="26"/>
        <source>System</source>
        <translation>Rendszer</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="42"/>
        <source>Input method for audio</source>
        <translation>Hangbemenet módja</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="45"/>
        <source>&lt;b&gt;This is the input method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation>&lt;b&gt;A hangbemenethez használt hangrendszer.&lt;/b&gt;&lt;br /&gt;Valószínűleg a DirectSound-t .akarja használni.</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="68"/>
        <source>Device</source>
        <translation>Eszköz</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="141"/>
        <source>Transmission</source>
        <translation>Adás</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="147"/>
        <source>&amp;Transmit</source>
        <translation>&amp;Beszéd küldése</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="157"/>
        <source>When to transmit your speech</source>
        <translation>Mikor küldje az ön hangját</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="160"/>
        <source>&lt;b&gt;This sets when speech should be transmitted.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;Continuous&lt;/i&gt; - All the time&lt;br /&gt;&lt;i&gt;Voice Activity&lt;/i&gt; - When you are speaking clearly.&lt;br /&gt;&lt;i&gt;Push To Talk&lt;/i&gt; - When you hold down the hotkey set under &lt;i&gt;Shortcuts&lt;/i&gt;.</source>
        <translation>&lt;b&gt;Beállítja, hogy mikor kell küldeni a beszédet.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;Folyamatosan&lt;/i&gt; - Mindig küldi.&lt;br /&gt;&lt;i&gt;Beszéd érzékelése&lt;/i&gt; - Amikor egyértelmüen beszél.&lt;br /&gt;&lt;i&gt;Beszédhez-nyomd&lt;/i&gt; - Amikor nyomva tartja a &lt;i&gt;Gyorsbillentyük&lt;/i&gt;alatt beállított billentyűt.</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="167"/>
        <source>Idle AutoMute</source>
        <translation>Némítás késleltetése</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="174"/>
        <source>How long to remain idle before auto-mute.</source>
        <translation>Mennyi ideig várjon az automatikus némítás bekapcsolása előtt.</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="177"/>
        <source>This sets an idle timer. If the timer expires without any audio being sent to the server, you are muted.</source>
        <translation>Beállít egy üresjárati időzítőt. Ha az időzítő lejár, úgy hogy közben semmi hangot küldött a szervernek, lenémítja a mikrofont.</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="219"/>
        <source>DoublePush Time</source>
        <translation>Dupla leütés</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="278"/>
        <source>Reset</source>
        <translation>Alapértelmezett</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="288"/>
        <location filename="AudioInput.ui" line="298"/>
        <source>Browse</source>
        <translation>Böngészés</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="328"/>
        <source>&lt;b&gt;Preview&lt;/b&gt;&lt;br/&gt;Plays the current &lt;i&gt;on&lt;/i&gt; soundfile followed by the current &lt;i&gt;off&lt;/i&gt; soundfile.</source>
        <translation>&lt;b&gt;Előnézet&lt;/b&gt;&lt;br/&gt;Lejátsza az aktuális &lt;i&gt;bekapcsolás&lt;/i&gt; hangfájlt, majd az aktuális &lt;i&gt;kikapcsolás&lt;/i&gt; hangfájlt.</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="331"/>
        <source>Preview</source>
        <translatorcomment>Előnézet helyett</translatorcomment>
        <translation>Meghallgatás</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="341"/>
        <source>&lt;b&gt;This enables the audio cues for push to talk.&lt;/b&gt;&lt;br /&gt;Setting this will give you a short audio beep when push to talk is pressed and released.</source>
        <translation>&lt;b&gt;A Beszédhez-nyomd módban engedélyezi a jelzőhangokat.&lt;/b&gt;&lt;br /&gt;Ha ezt beállítod, akkor a beszédhez használt gomb megnyomása és elengedése egy rövid hangjelzést fog eredményezni.</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="344"/>
        <source>PTT Audio cue</source>
        <translation>PTT hangjelzés</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="357"/>
        <source>Use SNR based speech detection</source>
        <translation>Jel-zaj viszonyra (SNR) épülő hangérzékelés használata</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="363"/>
        <source>Signal to Noise</source>
        <translation>Jel-zaj viszony</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="373"/>
        <source>&lt;b&gt;This sets speech detection to use Amplitude.&lt;/b&gt;&lt;br /&gt;In this mode, the raw strength of the input signal is used to detect speech.</source>
        <translation>&lt;b&gt;Hangérzékelés a hang amplitúdója alapján.&lt;/b&gt;&lt;br /&gt;Ebben a módban a feldolgozatlan bemeneti jel erőssége alapján érzékeli a beszédet.</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="376"/>
        <source>Amplitude</source>
        <translation>Amplitúdó</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="385"/>
        <source>Voice &amp;Hold</source>
        <translation>&amp;Beszéd tartása</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="395"/>
        <source>How long to keep transmitting after silence</source>
        <translation>A beszéd után mennyi ideig közvetít csendet</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="443"/>
        <source>Silence Below</source>
        <translation>Csend küszöb</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="453"/>
        <location filename="AudioInput.ui" line="485"/>
        <source>&lt;b&gt;This sets the trigger values for voice detection.&lt;/b&gt;&lt;br /&gt;Use this together with the Audio Statistics window to manually tune the trigger values for detecting speech. Input values below &quot;Silence Below&quot; always count as silence. Values above &quot;Speech Above&quot; always count as voice. Values in between will count as voice if you&apos;re already talking, but will not trigger a new detection.</source>
        <translation>&lt;b&gt;A hangérzékelés küszöbértékeit állítja be.&lt;/b&gt;&lt;br /&gt;A hangérzékelés küszöbértékeinek kézi beállításaihoz használd ezt a beállítást a Hangstatisztika ablakkal együtt. A bemenet &quot;Csend küszöb&quot; alatti értékei mindig csendet jelentenek. A &quot;Beszéd küszöb&quot; feletti értékek mindig beszédet jelentenek. Ha már beszél, a kettő közötti értékek beszédet fognak jelenteni, de új beszéd érzékelést nem váltanak ki.</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="475"/>
        <source>Speech Above</source>
        <translation>Beszéd küszöb</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="482"/>
        <source>Signal values above this count as voice</source>
        <translation>E fölötti értékek beszédet jelentenek</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="515"/>
        <source>Compression</source>
        <translation>Tömörítés</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="521"/>
        <source>&amp;Quality</source>
        <translation>&amp;Minőség</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="531"/>
        <source>Quality of compression (peak bandwidth)</source>
        <translation>A tömörítés minősége (sávszélesség csúcsa)</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="534"/>
        <source>&lt;b&gt;This sets the quality of compression.&lt;/b&gt;&lt;br /&gt;This determines how much bandwidth Mumble is allowed to use for outgoing audio.</source>
        <translation>&lt;b&gt;Beállítja a tömörítés minőségét.&lt;b/&gt;Meghatározza a Mumble által hangküldésre használható sávszélességet.</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="572"/>
        <source>Audio per packet</source>
        <translation>Csomagonkénti hang</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="582"/>
        <source>How many audio frames to send per packet</source>
        <translation>Mennyi hangminta küldhető egy csomagban</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="585"/>
        <source>&lt;b&gt;This selects how many audio frames should be put in one packet.&lt;/b&gt;&lt;br /&gt;Increasing this will increase the latency of your voice, but will also reduce bandwidth requirements.</source>
        <translation>&lt;b&gt;Egy csomagba helyezhető hangminták számát választja ki.&lt;/b&gt;&lt;br /&gt;E szám növelése a hang késleltetését fogja növelni, de a sávszélesség követelményeit csökkenteni fogja.</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="625"/>
        <source>&lt;b&gt;This shows peak outgoing bandwidth used.&lt;/b&gt;&lt;br /&gt;This shows the peak amount of bandwidth sent out from your machine. Audio bitrate is the maximum bitrate (as we use VBR) for the audio data alone. Position is the bitrate used for positional information. Overhead is our framing and the IP packet headers (IP and UDP is 75% of this overhead).</source>
        <translation>&lt;b&gt;A kimenő sávszélesség csúcsát mutatja.&lt;/b&gt;&lt;br /&gt;A gépéből küldött adatfolyam sávszélességének csúcsértékét mutatja. A hangbitráta a maximum értéke (ha VBR tömörítést használunk) a hang adatoknak. A pozicionálás bitrátája a pozicionáláshoz küldött adatok mennyiségét jelenti. A többletterhelés a saját keret és az IP csomag fejléceiből ered (az IP és UDP a 75%-t adja ennek a többletnek).</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="641"/>
        <source>Audio Processing</source>
        <translation>Hangfeldolgozás</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="647"/>
        <source>Noise Suppression</source>
        <translation>Zajszűrés</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="657"/>
        <source>Noise suppression</source>
        <translation>Zajszűrés</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="660"/>
        <source>&lt;b&gt;This sets the amount of noise suppression to apply.&lt;/b&gt;&lt;br /&gt;The higher this value, the more aggressively stationary noise will be suppressed.</source>
        <translation>&lt;b&gt;A használandó zajszűrés mértékét állítja be.&lt;/b&gt;&lt;br /&gt;Minél magasabb ez az érték, annál agresszívabban fogja az állandó zajt elnyomni.</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="692"/>
        <source>Amplification</source>
        <translation>Erősítés</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="702"/>
        <source>Maximum amplification of input sound</source>
        <translation>A bemeneti hang maximális erősítése</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="705"/>
        <source>&lt;b&gt;Maximum amplification of input.&lt;/b&gt;&lt;br /&gt;Mumble normalizes the input volume before compressing, and this sets how much it&apos;s allowed to amplify.&lt;br /&gt;The actual level is continually updated based on your current speech pattern, but it will never go above the level specified here.&lt;br /&gt;If the &lt;i&gt;Microphone loudness&lt;/i&gt; level of the audio statistics hover around 100%, you probably want to set this to 2.0 or so, but if, like most people, you are unable to reach 100%, set this to something much higher.&lt;br /&gt;Ideally, set it so &lt;i&gt;Microphone Loudness * Amplification Factor &gt;= 100&lt;/i&gt;, even when you&apos;re speaking really soft.&lt;br /&gt;&lt;br /&gt;Note that there is no harm in setting this to maximum, but Mumble will start picking up other conversations if you leave it to auto-tune to that level.</source>
        <translation>&lt;b&gt;A bemenet hang maximális erősítése.&lt;/b&gt;&lt;br /&gt;A Mumble a bemeneti hangerőt a tömörítés előtt normalizálja, itt beállíthatja, hogy mennyit erősíthet rajta.&lt;br /&gt;Az aktuális szintet az ön beszédmintája alapján frissíti, de ez sohasem fog az itt megadott szint fölé menni.&lt;br /&gt;Ha a hangstatisztikában a &lt;i&gt;Mikrofon hangereje&lt;/i&gt; 100% körül mozog, akkor célszerű a 2.0 körüli értékre állítani, de a legtöbb embernél nem éri el a 100%-t, ebben az esetben sokkal magasabb érték a kívánatos.&lt;br /&gt;Ideális esetben a &lt;i&gt;Mikrofon hangereje x erősítés együtthatója &gt;= 100&lt;/i&gt;, még akkor is, ha tényleg halkan beszél.&lt;br /&gt;&lt;br /&gt;Megjegyzésképpen, az sem árt, ha a maximális értékre állítja, de a Mumble más beszélgetéseket fog választani, ha hagyja, hogy azt a szintet automatikus hangolással érje el.</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="433"/>
        <source>Current speech detection chance</source>
        <translation>Hangérzékelés aktuális valószínűsége</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="84"/>
        <source>Input device for audio</source>
        <translation>Hangbemeneti eszköz</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="87"/>
        <source>&lt;b&gt;This is the input device to use for audio.&lt;/b&gt;</source>
        <translation>&lt;b&gt;Ez a hangrendszer által használt bemeneti eszköz.&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="100"/>
        <source>Cancel echo from speakers</source>
        <translation>Hangszórók visszhangmentesítése</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="103"/>
        <source>Enabling this will cancel the echo from your speakers. Mixed has low CPU impact, but only works well if your speakers are equally loud and equidistant from the microphone. Multichannel echo cancellation provides much better echo cancellation, but at a higher CPU cost.</source>
        <translation>Bekapcsolja a hangszórók visszhangmentesítését. A vegyes mód kevésbé terheli a processzort, de csak akkor működik jól, ha a hangszórók egyforma hangosak és egyforma távolságban vannak a mikrofontól. A többcsatornás mód jobb visszhangmentesítést tesz lehetővé, de jobban terheli a processzort.</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="107"/>
        <source>Disabled</source>
        <translation>Tiltva</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="112"/>
        <source>Mixed</source>
        <translation>Vegyes</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="117"/>
        <source>Multichannel</source>
        <translation>Többcsatornás</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="125"/>
        <source>Echo</source>
        <translation>Visszhang</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="229"/>
        <source>If you press the PTT key twice in this time it will get locked.</source>
        <translation>Zárolja a PTT billentyűt, ha a megadott időn belül kétszer leüti.</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="232"/>
        <source>&lt;b&gt;DoublePush Time&lt;/b&gt;&lt;br /&gt;If you press the push-to-talk key twice during the configured interval of time it will be locked. Mumble will keep transmitting until you hit the key once more to unlock PTT again.</source>
        <translation>&lt;b&gt;Dupla leütés&lt;/b&gt;&lt;br /&gt;Zárolja a PTT billentyűt, ha a beállított időn belül kétszer leüti. A Mumble addig fogja sugározni az ön hangját, amíg mégegyszer meg nem nyomja a PTT billentyűt a kioldáshoz.</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="258"/>
        <source>Gets played when the PTT button is pressed</source>
        <translation>A PTT billentyű lenyomásakor lejátszásra kerül</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="265"/>
        <source>Gets played when the PTT button is released</source>
        <translation>A PTT billentyű elengedésekor lejátszásra kerül</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="272"/>
        <source>Reset audio cue to default</source>
        <translation>A jelző hangot az alapértelmezettre állítja</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="275"/>
        <source>&lt;b&gt;Reset&lt;/b&gt;&lt;br/&gt;Reset the paths for the files to their default.</source>
        <translation>&lt;b&gt;Alapértelmezett&lt;/&gt;&lt;br /&gt;A fájlok elérési útját az alapértelmezettre állítja.</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="285"/>
        <source>Browse for on audio file</source>
        <translation>Böngészés egy bekapcsolást jelző hangfájl után</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="295"/>
        <source>Browse for off audio file</source>
        <translation>Böngészés egy kikapcsolást jelző hangfájl után</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="305"/>
        <source>Off</source>
        <translation>Ki</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="315"/>
        <source>On</source>
        <translation>Be</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="325"/>
        <source>Preview the audio cues</source>
        <translation>A jelző hangok meghallgatása</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="338"/>
        <source>Audible audio cue when push-to-talk is activated or deactivated</source>
        <translation>Hallható jelzések a Beszédhez-nyomd (PTT) billentyű lenyomásakor és elengedésekor</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="370"/>
        <source>Use Amplitude based speech detection</source>
        <translation>A hang amplitúdóján alapuló beszédérzékelés használata</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="360"/>
        <source>&lt;b&gt;This sets speech detection to use Signal to Noise ratio.&lt;/b&gt;&lt;br /&gt;In this mode, the input is analyzed for something resembling a clear signal, and the clarity of that signal is used to trigger speech detection.</source>
        <translation>&lt;b&gt;Jel-zaj viszonyon alapuló beszédérzékelést állít be.&lt;/b&gt;Ebben a módban a bemenetet a jel tisztasága szempontjából elemzi és a jel tisztasága fogja kiváltani a beszéd érzékelését.</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="398"/>
        <source>&lt;b&gt;This selects how long after a perceived stop in speech transmission should continue.&lt;/b&gt;&lt;br /&gt;Set this higher if your voice breaks up when you speak (seen by a rapidly blinking voice icon next to your name).</source>
        <translation>&lt;b&gt;Beállítja, hogy a beszéd végének érzékelése után még mennyi ideig küldje a hangot.&lt;/b&gt;&lt;br /&gt;Állítsa magasabb értékre, ha a beszéde szakadozottá válik (amit egy gyorsan villogó hangikon mutat a neve mellett).</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="436"/>
        <source>&lt;b&gt;This shows the current speech detection settings.&lt;/b&gt;&lt;br /&gt;You can change the settings from the Settings dialog or from the Audio Wizard.</source>
        <translation>&lt;b&gt;A beszédérzékelés jelenlegi beállításait mutatja.&lt;/b&gt;&lt;br /&gt;A Beállítások párbeszédablakban vagy a Hangvarázslóban tudja módosítani ezeket a beállításokat.</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="450"/>
        <source>Signal values below this count as silence</source>
        <translation>Ez alatti jelértékek csendnek számítanak</translation>
    </message>
    <message>
        <location filename="AudioInput.ui" line="622"/>
        <source>Maximum bandwidth used for sending audio</source>
        <translation>Maximális sávszélesség a hang küldéséhez</translation>
    </message>
    <message>
        <location filename="AudioInput.cpp" line="528"/>
        <source>Server maximum network bandwidth is only %1 kbit/s. Audio quality auto-adjusted to %2 kbit/s (%3ms)</source>
        <translation>A szerver maximális hálózati sávszélessége csak %1 kbit/s. A hangminőséget automatikusan %2 kbit/s-ra (%3ms-ra) állítottam</translation>
    </message>
</context>
<context>
    <name>AudioInputDialog</name>
    <message>
        <location filename="AudioConfigDialog.cpp" line="74"/>
        <source>Continuous</source>
        <translation>Folyamatos</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="75"/>
        <source>Voice Activity</source>
        <translation>Beszéd érzékelése</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="76"/>
        <source>Push To Talk</source>
        <translation>Beszédhez-nyomd</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="89"/>
        <source>Audio Input</source>
        <translation>Hangbemenet</translation>
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
        <translation>Ki</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="199"/>
        <source>%1 s</source>
        <translation>%1 s</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="203"/>
        <source>%1 kb/s</source>
        <translation>%1 kb/s</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="214"/>
        <source>-%1 dB</source>
        <translation>-%1 dB</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="269"/>
        <source>%1 kbit/s (Audio %2 %5, Position %4, Overhead %3)</source>
        <translation>%1 kbit/s (Hang %2 %5, Pozíció %4, Többletterhelés %3)</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="269"/>
        <source>CELT</source>
        <translation>CELT</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="269"/>
        <source>Speex</source>
        <translation>Speex</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="227"/>
        <source>%1 min</source>
        <translation>%1 perc</translation>
    </message>
</context>
<context>
    <name>AudioOutput</name>
    <message>
        <location filename="AudioOutput.ui" line="14"/>
        <source>Form</source>
        <translation>Űrlap</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="20"/>
        <source>Interface</source>
        <translation>Interfész</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="26"/>
        <source>System</source>
        <translation>Rendszer</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="68"/>
        <source>Device</source>
        <translation>Eszköz</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="42"/>
        <source>Output method for audio</source>
        <translation>Hangkimenet módja</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="45"/>
        <source>&lt;b&gt;This is the output method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation>&lt;b&gt;A hangkimenethez használt hangrendszer.&lt;/b&gt;&lt;br /&gt;Valószínűleg a DirectSound-t .akarja használni.</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="84"/>
        <source>Output device for audio</source>
        <translation>Hangkimeneti eszköz</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="87"/>
        <source>&lt;b&gt;This is the output device to use for audio.&lt;/b&gt;</source>
        <translation>&lt;b&gt;Ez a hangrendszer által használt kimeneti eszköz.&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="100"/>
        <location filename="AudioOutput.ui" line="320"/>
        <source>Positional Audio</source>
        <translation>Hang pozicionálás</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="110"/>
        <source>Audio Output</source>
        <translation>Hangkimenet</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="116"/>
        <source>Default &amp;Jitter Buffer</source>
        <translation>&amp;Alapértelmezett puffer</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="126"/>
        <source>Safety margin for jitter buffer</source>
        <translation>A nyávogás elleni puffer méretének biztonsági korlátja</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="129"/>
        <source>&lt;b&gt;This sets the minimum safety margin for the jitter buffer.&lt;/b&gt;&lt;br /&gt;All incoming audio is buffered, and the jitter buffer continually tries to push the buffer to the minimum sustainable by your network, so latency can be as low as possible. This sets the minimum buffer size to use. If the start of sentences you hear is very jittery, increase this value.</source>
        <translation>&lt;b&gt;A nyávogás elleni puffer minimális méretét állítja be.&lt;/b&gt;&lt;br /&gt;Minden bejövő hang pufferelve van és azért, hogy a késleltetés minél kisebb legyen, a nyávogásmentesítő pufferelés folyamán, a puffer méretét, a hálózat szempontjából fenntartható, legkisebb méretűre kell csökkenteni. Ha az ön által hallott mondatok eleje nyávogós, akkor növelje meg ezt az értéket.</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="213"/>
        <source>This sets the amount of data to pre-buffer in the output buffer. Experiment with different values and set it to the lowest which doesn&apos;t cause rapid jitter in the sound.</source>
        <translation>A kimeneti puffer előpufferelését állítja be. Próbálkozzon különböző értékekkel és allítsa be a legkisebbre, amely nem okoz hirtelen nyávogásokat.</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="239"/>
        <source>Attenuate applications by...</source>
        <translation>Alkalmazások halkítása...</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="255"/>
        <source>Attenuation of other applications during speech</source>
        <translation>Beszéd közben a többi alkalmazást halkítja</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="258"/>
        <source>&lt;b&gt;Attenuate volume of other applications during speech&lt;/b&gt;&lt;br /&gt;Mumble supports decreasing the volume of other applications during incoming and/or outgoing speech. This sets the attenuation of other applications if the feature is enabled.</source>
        <translation>&lt;b&gt;Beszéd közben a többi alkalmazás hangját csökkenti&lt;/b&gt;&lt;br /&gt;A Mumble támogatja a többi alkalmazás hangerejének csökkentését kimenő és/vagy bejövő beszéd közben. Ezzel beállíthatja a csökkentés mértékét, ha a lehetőség engedélyezett.</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="286"/>
        <source>If checked Mumble lowers the volume of other applications while other users talk</source>
        <translation>Ha ki van jelölve, a Mumble csökkenti a többi alkalmazás hangerejét miközben a többi felhasználó beszél</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="289"/>
        <source>&lt;b&gt;Attenuate applications while other users talk&lt;/b&gt;&lt;br /&gt;Mumble supports decreasing the volume of other applications during incoming and/or outgoing speech. This makes mumble activate the feature while other users talk to you.</source>
        <translation>&lt;b&gt;Ha más felhasználók beszélnek, akkor a többi alkalmazás hangját csökkenti&lt;/b&gt;&lt;br /&gt;A Mumble támogatja a többi alkalmazás hangerejének csökkentését kimenő és/vagy bejövő beszéd közben. Ezzel engedélyezheti a Mumble-nak, hogy használja ezt a lehetőséget míg más felhasználók beszélnek önnel.</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="292"/>
        <source>while other users talk</source>
        <translation>míg mások beszélnek</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="299"/>
        <source>If checked Mumble lowers the volume of other applications while you talk</source>
        <translation>Ha ki van jelölve, a Mumble csökkenti a többi alkalmazás hangerejét miközben ön beszél</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="302"/>
        <source>&lt;b&gt;Attenuate applications while you talk&lt;/b&gt;&lt;br /&gt;Mumble supports decreasing the volume of other applications during incoming and/or outgoing speech. This makes mumble activate the feature while you talk.</source>
        <translation>&lt;b&gt;Ha ön beszél, akkor a többi alkalmazás hangját csökkenti&lt;/b&gt;&lt;br /&gt;A Mumble támogatja a többi alkalmazás hangerejének csökkentését kimenő és/vagy bejövő beszéd közben. Ezzel engedélyezheti a Mumble-nak, hogy használja ezt a lehetőséget míg ön beszél.</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="305"/>
        <source>while you talk</source>
        <translation>míg ön beszél</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="326"/>
        <source>The connected &quot;speakers&quot; are actually headphones</source>
        <translation>A bekötött &quot;hangszórók&quot; valójában fejhallgatók</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="529"/>
        <source>&lt;b&gt;This sets the packet latency variance for loopback testing.&lt;/b&gt;&lt;br /&gt;Most audio paths contain some variable latency. This allows you to set that variance for loopback mode testing. For example, if you set this to 15ms, this will emulate a network with 20-35ms ping latency or one with 80-95ms latency. Most domestic net connections have a variance of about 5ms.</source>
        <translation>&lt;b&gt;A visszacsatolási teszthez beállítja a csomagkésés ingadozását.&lt;/b&gt;&lt;br /&gt;A hangtovábbítás késleltetése a legtöbb útvonalon ingadozik valamennyit. A visszacsatolási teszthez, ezzel beállíthatja ennek az ingadozásnak a mértékét. Például. ha 15ms-ra állítja, akkor egy olyan hálózatot fog emulálni, amelynek a késleltetése (ping) 20-35ms vagy 80-95 ms. A legtöbb otthoni hálózatkapcsolat ingadozása 5ms körül van.</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="613"/>
        <source>&lt;b&gt;This enables one of the loopback test modes.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;None&lt;/i&gt; - Loopback disabled&lt;br /&gt;&lt;i&gt;Local&lt;/i&gt; - Emulate a local server.&lt;br /&gt;&lt;i&gt;Server&lt;/i&gt; - Request loopback from server.&lt;br /&gt;Please note than when loopback is enabled, no other users will hear your voice. This setting is not saved on application exit.</source>
        <translation>&lt;b&gt;Ezzel engedlyezi az egyik visszacsatolási tesztet.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;Nincs&lt;/i&gt; - Visszacsatolás tiltva&lt;br /&gt;&lt;i&gt;Helyi&lt;/i&gt; - Egy helyi szervert emulál.&lt;br /&gt;&lt;i&gt;Szerver&lt;/i&gt; - Szervertől kér visszacsatolást.&lt;br /&gt;Kérem vegye figyelembe, hogy amikor a visszacsatolás be van kapcsolva senki sem fogja hallani önt. Ez a beállítás nincs elmentve a programból való kilépéskor.</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="161"/>
        <source>Volume</source>
        <translation>Hangerő</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="171"/>
        <source>Volume of incoming speech</source>
        <translation>Bejövő beszéd hangereje</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="174"/>
        <source>&lt;b&gt;This adjusts the volume of incoming speech.&lt;/b&gt;&lt;br /&gt;Note that if you increase this beyond 100%, audio will be distorted.</source>
        <translation>&lt;b&gt;A bejövő beszéd hangerejét állítja.&lt;/p&gt;&lt;br /&gt;Figyelem, ha 100% fölé erősíti, akkor a hang torzítani fog.</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="200"/>
        <source>Output Delay</source>
        <translation>Kimenet késleltetése</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="210"/>
        <source>Amount of data to buffer</source>
        <translation>Pufferelendő adatmennyiség</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="436"/>
        <source>Factor for sound volume decrease</source>
        <translation>Hangerőcsökkentés együtthatója</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="465"/>
        <source>Bloom</source>
        <translation>Hangosítás</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="488"/>
        <source>Factor for sound volume increase</source>
        <translation>Hangerőnövelés együtthatója</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="491"/>
        <source>How much should sound volume increase for sources that are really close?</source>
        <translation>Mennyire kell a hangerőt növelni a tényleg közel levő források esetén?</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="329"/>
        <source>Checking this indicates that you don&apos;t have speakers connected, just headphones. This is important, as speakers are usually in front of you, while headphones are directly to your left/right.</source>
        <translation>Ha kijelöli ezt, azt jelenti, hogy nincsenek bekapcsolt hangszórói, csak fejhallgatók. Ez azért fontos, mert a hangszórók általában ön előtt vannak, míg a fejhallgatók közvetlenül az ön jobb illetve baloldalán.</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="332"/>
        <source>Headphones</source>
        <translation>Fejhallgatók</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="339"/>
        <source>Minimum Distance</source>
        <translation>Minimális távolság</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="349"/>
        <source>Minimum distance to user before sound volume decreases</source>
        <translation>A minimális távolság a felhasználótól, mielőtt a hangerő csökkenne</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="355"/>
        <source>This sets the minimum distance for sound calculations. The volume of other users&apos; speech will not decrease until they are at least this far away from you.</source>
        <translation>A hang számításaihoz beállítja a minimális távolságot. A többi felhasználó hangereje nem fog csökkenni míg legfeljebb ilyen távolságra vannak öntől.</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="384"/>
        <source>Maximum Distance</source>
        <translation>Maximális távolság</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="394"/>
        <source>Maximum distance, beyond which speech volume won&apos;t decrease</source>
        <translation>A maximális távolság, ami felett a hangerő nem csökken</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="397"/>
        <source>This sets the maximum distance for sound calculations. When farther away than this, other users&apos; speech volume will not decrease any further.</source>
        <translation>A hang számításaihoz beállítja a maximális távolságot. A többi felhasználó hangereje nem fog csökkenni ha ennél a távolságnál messzebb vannak öntől.</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="426"/>
        <source>Minimum Volume</source>
        <translation>Minimális hangerő</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="439"/>
        <source>What should the volume be at the maximum distance?</source>
        <translation>Mennyi legyen a hangerő a maximális távolságnál?</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="510"/>
        <source>Loopback Test</source>
        <translation>Visszacsatolási teszt</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="516"/>
        <source>Delay Variance</source>
        <translation>Késleltetés ingadozása</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="526"/>
        <source>Variance in packet latency</source>
        <translation>A csomagok késésének ingadozása</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="555"/>
        <source>Packet Loss</source>
        <translation>Csomagvesztés</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="565"/>
        <source>Packet loss for loopback mode</source>
        <translation>Csomagvesztés a visszacsatoláshoz</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="568"/>
        <source>&lt;b&gt;This sets the packet loss for loopback mode.&lt;/b&gt;&lt;br /&gt;This will be the ratio of packets lost. Unless your outgoing bandwidth is peaked or there&apos;s something wrong with your network connection, this will be 0%</source>
        <translation>&lt;b&gt;Beállítja a csomagvesztés mértékét a visszacsatolt módban.&lt;/b&gt;&lt;br /&gt;Ez lesz a csomagvesztés aránya. Hacsak a kimenő adatforgalma nincs csúcson vagy nincs valami baj a hálózati kapcsolatával, akkor ez 0% lesz</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="600"/>
        <source>&amp;Loopback</source>
        <translation>&amp;Visszacsatolás</translation>
    </message>
    <message>
        <location filename="AudioOutput.ui" line="610"/>
        <source>Desired loopback mode</source>
        <translation>A kívánt visszacsatolási mód</translation>
    </message>
</context>
<context>
    <name>AudioOutputDialog</name>
    <message>
        <location filename="AudioConfigDialog.cpp" line="394"/>
        <source>None</source>
        <translation>Nincs</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="395"/>
        <source>Local</source>
        <translation>Helyi</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="396"/>
        <source>Server</source>
        <translation>Szerver</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="402"/>
        <source>Audio Output</source>
        <translation>Hangkimenet</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="504"/>
        <location filename="AudioConfigDialog.cpp" line="523"/>
        <location filename="AudioConfigDialog.cpp" line="531"/>
        <source>%1 ms</source>
        <translation>%1 ms</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="515"/>
        <location filename="AudioConfigDialog.cpp" line="519"/>
        <location filename="AudioConfigDialog.cpp" line="527"/>
        <location filename="AudioConfigDialog.cpp" line="558"/>
        <location filename="AudioConfigDialog.cpp" line="562"/>
        <source>%1 %</source>
        <translation>%1 %</translation>
    </message>
    <message>
        <location filename="AudioConfigDialog.cpp" line="546"/>
        <location filename="AudioConfigDialog.cpp" line="552"/>
        <source>%1 m</source>
        <translation>%1 m</translation>
    </message>
</context>
<context>
    <name>AudioOutputSample</name>
    <message>
        <location filename="AudioOutput.cpp" line="295"/>
        <source>Choose sound file</source>
        <translation>Válasszon hangfájlt</translation>
    </message>
    <message>
        <location filename="AudioOutput.cpp" line="299"/>
        <source>Invalid sound file</source>
        <translation>Érvénytelen hangfájl</translation>
    </message>
    <message>
        <location filename="AudioOutput.cpp" line="300"/>
        <source>The file &apos;%1&apos; cannot be used by Mumble. Please select a file with a compatible format and encoding.</source>
        <translation>A &apos;%1&apos; fájlt a Mumble nem tudja használni. Kérem válasszon egy kompatibilis formátumú és kódolású fájlt.</translation>
    </message>
</context>
<context>
    <name>AudioStats</name>
    <message>
        <location filename="AudioStats.ui" line="14"/>
        <source>Audio Statistics</source>
        <translation>Hang statisztika</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="22"/>
        <source>Input Levels</source>
        <translation>Bemeneti szint</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="28"/>
        <source>Peak microphone level</source>
        <translation>Mikrofon csúcsértéke</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="35"/>
        <location filename="AudioStats.ui" line="55"/>
        <location filename="AudioStats.ui" line="75"/>
        <source>Peak power in last frame</source>
        <translation>Utolsó hangkeret csúcsértéke</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="38"/>
        <source>This shows the peak power in the last frame (20 ms), and is the same measurement as you would usually find displayed as &quot;input power&quot;. Please disregard this and look at &lt;b&gt;Microphone power&lt;/b&gt; instead, which is much more steady and disregards outliers.</source>
        <translation>Az utolsó hangkeret (20 ms) csúcsértékét mutatja, ami hasonló ahhoz, mint amit szokásosan a &quot;bemeneti hangerőnek&quot; neveznek. Kérem hagyja ezt figyelmen kívül és helyette inkább a &lt;b&gt;Mikrofon hangerejét&lt;/b&gt; figyelje, ami sokkal biztosabb és elhanyagolja a szélsőséges értékeket.</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="48"/>
        <source>Peak speaker level</source>
        <translation>Hangszóró csúcsértéke</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="68"/>
        <source>Peak clean level</source>
        <translation>Csúcsérték törlése</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="58"/>
        <source>This shows the peak power of the speakers in the last frame (20 ms). Unless you are using a multi-channel sampling method (such as ASIO) with speaker channels configured, this will be 0. If you have such a setup configured, and this still shows 0 while you&apos;re playing audio from other programs, your setup is not working.</source>
        <translation>A hangszórók hangerejének csúcsát mutatja az utolsó hangkeretben (20 ms). Hacsak nem használ egy többcsatornás mintavételezési módszert (mint az ASIO) a beállított hangszóró csatornákra, ez az érték 0 lesz. Ha ilyen beállítással rendelkezik és mégis 0-t mutat mialatt más programok hangot játszanak le, akkor az ön beállításai nem jók.</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="78"/>
        <source>This shows the peak power in the last frame (20 ms) after all processing. Ideally, this should be -96 dB when you&apos;re not talking. In reality, a sound studio should see -60 dB, and you should hopefully see somewhere around -20 dB. When you are talking, this should rise to somewhere between -5 and -10 dB.&lt;br /&gt;If you are using echo cancellation, and this rises to more than -15 dB when you&apos;re not talking, your setup is not working, and you&apos;ll annoy other users with echoes.</source>
        <translation>A teljes feldolgozás után, a hangerő csúcsát mutatja az utolsó hangkeretben (20 ms). Ha nincs beszéd, ideális esetben ez -96 dB körül kellene legyen. A valóságban a hangstúdiók látnak -60 dB körüli értékeket és remélhetőleg ön lát valahol -20 db körüli értéket. Amikor beszél, ennek fel kell ugornia valahova -5 és -10 dB közzé.&lt;br /&gt;Ha nem beszél és -15 dB főlé megy úgy, hogy közben visszhang csökkentést is használ, akkor a beállításai nem jók és csak bosszantja a többieket a visszhanggal.</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="91"/>
        <source>Signal Analysis</source>
        <translation>Jelalak elemzés</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="97"/>
        <source>Microphone power</source>
        <translation>Mikrofon hangereje</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="104"/>
        <source>How close the current input level is to ideal</source>
        <translation>Mennyire van közel a jelenlegi bemeneti jel az ideálishoz</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="107"/>
        <source>This shows how close your current input volume is to the ideal. To adjust your microphone level, open whatever program you use to adjust the recording volume, and look at the value here while talking.&lt;br /&gt;&lt;b&gt;Talk loud, as you would when you&apos;re upset over getting fragged by a noob.&lt;/b&gt;&lt;br /&gt;Adjust the volume until this value is close to 100%, but make sure it doesn&apos;t go above. If it does go above, you are likely to get clipping in parts of your speech, which will degrade sound quality.</source>
        <translatorcomment>A &quot;getting fragged by a noob&quot; angol kifejezésnek nincs magyar változata, ez egy számítógépes játékok használatával elterjedt kifejezés.</translatorcomment>
        <translation>Mutatja, hogy mennyire van közel a jelenlegi bemeneti jel az ideálishoz. A mikrofon jelszintjének a beállításához nyisson meg egy bármilyen programot, amellyel a felvételi hangerőt szokta állítani és miközben beszél, nézze ezt az értéket.&lt;br /&gt;&lt;b&gt;Beszéljen hangosan, úgy mintha izgatott lenne&lt;/b&gt;&lt;br /&gt;Állítsa a hangerőt a 100% közelébe, de az biztosan ne haladja meg. Amikor meghaladja a 100%-t, akkor az ön hangjának azon részeit levágja, ami a hangminőség csökkenését jelenti.</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="117"/>
        <source>Signal-To-Noise ratio</source>
        <translation>Jel-zaj viszony</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="124"/>
        <source>Signal-To-Noise ratio from the microphone</source>
        <translation>A mikrofon hangjának jel-zaj viszonya</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="127"/>
        <source>This is the Signal-To-Noise Ratio (SNR) of the microphone in the last frame (20 ms). It shows how much clearer the voice is compared to the noise.&lt;br /&gt;If this value is below 1.0, there&apos;s more noise than voice in the signal, and so quality is reduced.&lt;br /&gt;There is no upper limit to this value, but don&apos;t expect to see much above 40-50 without a sound studio.</source>
        <translation>A mikrofon hangjának utolsó hangkeretének (20 ms) a jel-zaj viszonya (SNR). Azt mutatja, hogy a hang mennyire tiszta a zajhoz képest.&lt;br /&gt;Ha ez az érték 1.0 alatt van, akkor a jelben több a zaj, mint a hang. azaz a minőség nagyon gyenge.&lt;br /&gt;Ennek az értéknek nincs felső határa, de egy hangstúdió nélkül ne számítson nagyobb értékre mint 40-50.</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="137"/>
        <source>Speech Probability</source>
        <translation>Hang valószínűsége</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="144"/>
        <source>Probability of speech</source>
        <translation>Beszéd valószínűsége</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="147"/>
        <source>This is the probability that the last frame (20 ms) was speech and not environment noise.&lt;br /&gt;Voice activity transmission depends on this being right. The trick with this is that the middle of a sentence is always detected as speech; the problem is the pauses between words and the start of speech. It&apos;s hard to distinguish a sigh from a word starting with &apos;h&apos;.&lt;br /&gt;If this is in bold font, it means Mumble is currently transmitting (if you&apos;re connected).</source>
        <translation>Annak a valószínűsége, hogy az utolsó hangkeretben (20 ms) beszéd van és nem csak zaj.&lt;br /&gt;A beszéd küldése ennek az értéknek a helyességén múlik. A trükk az, hogy ugyan a mondat közepét mindig beszédnek érzékeli, de a szünetek és a beszéd kezdetének felismerése problémát okoz. Nehéz különbséget tenni és sóhaj és egy &apos;h&apos; betűvel kezdődő szó között.&lt;br /&gt;Ha ezt az értéket kövér betűvel írta ki, akkor azt jelenti, hogy a Mumble éppen továbbítja a hangot (ha éppen csatlakozva van).</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="162"/>
        <source>Configuration feedback</source>
        <translation>Beállítások visszajelzése</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="168"/>
        <source>Current audio bitrate</source>
        <translation>Jelenlegi hangbitráta</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="181"/>
        <source>Bitrate of last frame</source>
        <translation>Utolsó hangkeret bitrátája</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="184"/>
        <source>This is the audio bitrate of the last compressed frame (20 ms), and as such will jump up and down as the VBR adjusts the quality. To adjust the peak bitrate, adjust &lt;b&gt;Compression Complexity&lt;/b&gt; in the Settings dialog.</source>
        <translation>Az utolsó tömörített hangkeret (20 ms) bitrátája, és fel, le mozog, úgy ahogy a VBR változtatja a minőséget. A bitráta csúcsértékének beállításához, állítsa a &lt;b&gt;Tömörítés bonyolultsága&lt;/b&gt; értéket a Beállítások párbeszédablakban.</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="207"/>
        <source>Time between last two Push-To-Talk presses</source>
        <translation>Az utolsó két Beszédhez-nyomd leütés közötti idő</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="194"/>
        <source>DoublePush interval</source>
        <translation>Dupla leütés ideje</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="217"/>
        <source>Speech Detection</source>
        <translation>Beszédfelismerés</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="224"/>
        <source>Current speech detection chance</source>
        <translation>Pillanatnyi beszédérzékelés valószínűsége</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="227"/>
        <source>&lt;b&gt;This shows the current speech detection settings.&lt;/b&gt;&lt;br /&gt;You can change the settings from the Settings dialog or from the Audio Wizard.</source>
        <translation>&lt;b&gt;A beszédérzékelés jelenlegi beállításait mutatja.&lt;/b&gt;&lt;br /&gt;A Beállítások párbeszédablakban vagy a Hangvarázslóban módosíthatja a beállításokat.</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="256"/>
        <source>Signal and noise power spectrum</source>
        <translation>Jel- és zajerősség spektruma</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="262"/>
        <source>Power spectrum of input signal and noise estimate</source>
        <translation>A bemeneti jel hangerejének spektuma és becsült zaja</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="265"/>
        <source>This shows the power spectrum of the current input signal (red line) and the current noise estimate (filled blue).&lt;br /&gt;All amplitudes are multiplied by 30 to show the interesting parts (how much more signal than noise is present in each waveband).&lt;br /&gt;This is probably only of interest if you&apos;re trying to fine-tune noise conditions on your microphone. Under good conditions, there should be just a tiny flutter of blue at the bottom. If the blue is more than halfway up on the graph, you have a seriously noisy environment.</source>
        <translation>A jelenlegi bemeneti jel (vörös vonal) hangerejének spektuma és becsült zaja (kékkel jelölve).&lt;br /&gt;Hogy az érdekes részt mutassa (hogy mennyivel több a jel, mint a zaj mindegyik sávban) az összes amplitúdó 30-al lett megszorozva.&lt;br /&gt;Ez csak akkor érdekes, ha a mikrofonjának zajosságát próbálja finomhangolni. Jó feltételek mellett az alján csak egy vékony, kék, ingadozó csík látható. Ha a kék rész a grafikon felénél nagyobb, akkor nagyon zajos környezetben van.</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="281"/>
        <source>Echo Analysis</source>
        <translation>Visszhang elemzés</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="293"/>
        <source>Weights of the echo canceller</source>
        <translation>Visszhang elnyomás mértéke</translation>
    </message>
    <message>
        <location filename="AudioStats.ui" line="296"/>
        <source>This shows the weights of the echo canceller, with time increasing downwards and frequency increasing to the right.&lt;br /&gt;Ideally, this should be black, indicating no echo exists at all. More commonly, you&apos;ll have one or more horizontal stripes of bluish color representing time delayed echo. You should be able to see the weights updated in real time.&lt;br /&gt;Please note that as long as you have nothing to echo off, you won&apos;t see much useful data here. Play some music and things should stabilize. &lt;br /&gt;You can choose to view the real or imaginary parts of the frequency-domain weights, or alternately the computed modulus and phase. The most useful of these will likely be modulus, which is the amplitude of the echo, and shows you how much of the outgoing signal is being removed at that time step. The other viewing modes are mostly useful to people who want to tune the echo cancellation algorithms.&lt;br /&gt;Please note: If the entire image fluctuates massively while in modulus mode, the echo canceller fails to find any correlation whatsoever between the two input sources (speakers and microphone). Either you have a very long delay on the echo, or one of the input sources is configured wrong.</source>
        <translation>Visszhang elnyomás mértékét mutatja, az idő lefelé növekszik és a frekvencia jobbra nő.&lt;br /&gt;Ideális esetben fekete kell legyen, ami azt jelentené, hogy egyáltalán nincs visszhang. Leggyakrabban, egy vagy több vízszintes, kékes csík jelzi az időben eltolt visszhangot. Önnek itt valós időben kell látnia az elnyomás súlyait.&lt;br /&gt;Kérem vegye figyelembe, hogy itt nem fog semmi hasznos adatot látni, amíg nincs elnyomható visszhang. Játsszon le valamilyen zenét és a dolgok stabilizálódni fognak.&lt;br /&gt;Megtekintheti a frekvenciatartományok valós vagy képzett részeit vagy alternatívaként a kiszámított modulus és fázist. Valószínűleg ezek közül a leghasznosabb a modulus, ami a visszhang amplitúdója és megmutatja, hogy abban az időintervallumban a kimeneti jelből mennyit távolít el. A többi megjelenítési mód csak azoknak az embereknek hasznos, akik a visszhangcsökkentő algoritmust akarják hangolni.&lt;br /&gt;Megjegyzés képpen, ha a modulusz módban az egész kép hullámzik, akkor a visszhangcsökkentő nem talál egyezést a két bemeneti forrásban (hangszóró és mikrofon). Ez azt jelenti, hogy vagy nagyon nagy a késleltetés a visszhangban, vagy az egyik bemeneti forrás rosszul van beállítva.</translation>
    </message>
    <message>
        <location filename="AudioStats.cpp" line="402"/>
        <source>&gt;1000 ms</source>
        <translation>&gt;1000 ms</translation>
    </message>
</context>
<context>
    <name>AudioWizard</name>
    <message>
        <location filename="AudioWizard.ui" line="14"/>
        <source>Audio Tuning Wizard</source>
        <translation>Hangbeállító varázsló</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="18"/>
        <source>Introduction</source>
        <translation>Bevezetés</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="21"/>
        <source>Welcome to the Mumble Audio Wizard</source>
        <translation>Üdvözli a Mumble hangbeállító varázslója</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="761"/>
        <source>Finished</source>
        <translation>Befejezve</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="764"/>
        <source>Enjoy using Mumble</source>
        <translation>Élvezze a Mumble használatát</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="56"/>
        <source>Device selection</source>
        <translation>Eszköz választás</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="59"/>
        <source>Selecting the input and output device to use with Mumble.</source>
        <translation>A ki- és bemeneti eszköz megválasztása a Mumble számára.</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="71"/>
        <source>This is the device your microphone is connected to.</source>
        <translation>Ehhez az eszközhöz van kapcsolva a mikrofonja.</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="81"/>
        <location filename="AudioWizard.ui" line="153"/>
        <source>System</source>
        <translation>Rendszer</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="91"/>
        <source>Input method for audio</source>
        <translation>Hangbemenet módja</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="94"/>
        <source>&lt;b&gt;This is the input method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation>&lt;b&gt;A hangbemenethez használt hangrendszer.&lt;/b&gt;&lt;br /&gt;Valószínűleg a DirectSound-t .akarja használni.</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="101"/>
        <location filename="AudioWizard.ui" line="173"/>
        <source>Device</source>
        <translation>Eszköz</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="111"/>
        <source>Input device to use</source>
        <translation>Használandó bemeneti eszköz</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="114"/>
        <source>&lt;b&gt;Selects which sound card to use for audio input.&lt;/b&gt;</source>
        <translation>&lt;b&gt;Kiválasztja a hangbemenethez használni kívánt hangkártyát.&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="121"/>
        <source>Cancel echo from headset or speakers</source>
        <translation>Visszhang kiiktatása a fejhallgatóból vagy a hangszórókból</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="127"/>
        <source>Use echo cancellation</source>
        <translation>Visszhangcsökkentés használata</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="124"/>
        <source>This enables echo cancellation of outgoing audio, which helps both on speakers and on headsets.</source>
        <translation>Engedélyezi a kimenő hang visszhangcsökkentését, ez segít mind a hangszórók, mind a fejhallgató esetében is.</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="143"/>
        <source>This is the device your speakers or headphones are connected to.</source>
        <translation>Ehhez az eszközhöz vannak az ön hangszórói vagy fejhallgatója csatlakoztatva.</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="163"/>
        <source>Output method for audio</source>
        <translation>Hangkimenet módja</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="166"/>
        <source>&lt;b&gt;This is the Output method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation>&lt;b&gt;Ez a hangkimeneti mód a hang használatához.&lt;/b&gt;&lt;br /&gt;Valószínűleg a DirectSound-t .akarja használni.</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="183"/>
        <source>Output device to use</source>
        <translation>Használandó kimeneti eszköz</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="186"/>
        <source>&lt;b&gt;Selects which sound card to use for audio Output.&lt;/b&gt;</source>
        <translation>&lt;b&gt;Kiválasztja a hangkimenethez használni kívánt hangkártyát.&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="193"/>
        <source>Allows positioning of sound</source>
        <translation>Engedélyezi a hang pozicionálását</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="199"/>
        <source>Enable positional audio</source>
        <translation>Engedélyezi a pozicionált hangot</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="196"/>
        <source>This allows Mumble to use positional audio to place voices.</source>
        <translation>Ezzel engedélyezi, hogy a Mumble használhassa a hang pozicionálást, hogy a hangokat térben elhelyezhesse.</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="328"/>
        <source>&lt;p&gt;
Open your sound control panel and go to the recording settings. Make sure the microphone is selected as active input with maximum recording volume. If there&apos;s an option to enable a &quot;Microphone boost&quot; make sure it&apos;s checked.
&lt;/p&gt;
&lt;p&gt;
Speak loudly, as when you are annoyed or excited. Decrease the volume in the sound control panel until the bar below stays as high as possible in the blue and green but &lt;b&gt;not&lt;/b&gt; the red zone while you speak.
&lt;/p&gt;
</source>
        <translation>&lt;p&gt;
Nyissa meg a hangbeállítások ablakát és menjen a felvétel beállításaihoz. Győződjön meg róla, hogy a mikrofon maximális felvételi hangerőre van állítva és ki van választva, mint aktív bemenet. Ha talál egy olyan beállítást, mint a &quot;Mikrofon boost&quot;, ellenőrizze, hogy ki van jelölve.
&lt;/p&gt;
&lt;p&gt;
Beszéljen hangosan, mint mikor bosszús vagy izgatott. Csökkentse a hangerőt a hangbeállítások ablakában addig, amíg beszéd közben a csík olyan magasan van, amennyire csak lehet a kék és zöld zónán belül, de &lt;b&gt;ne&lt;/b&gt; érje el a piros zónát.
&lt;/p&gt;
</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="408"/>
        <source>Apply some high contrast optimizations for visually impaired users</source>
        <translation>Látáscsökkent felhasználóknak néhány kontraszt növelő optimalizálást végez</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="411"/>
        <source>Use high contrast graphics</source>
        <translation>Nagy kontrasztú grafika használata</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="497"/>
        <source>Next you need to adjust the following slider. The first few utterances you say should end up in the green area (definitive speech). While talking, you should stay inside the yellow (might be speech) and when you&apos;re not talking, everything should be in the red (definitively not speech).</source>
        <translation>Ezután a következő csúszkát kell beállítani. Az ön beszédének első néhány kiejtett hangja a zöld zónáig kell vigye a kijelzést (biztosan beszéd). Amíg beszél, a kijelzés a sárga zónában kell maradjon (lehetséges beszéd) és amikor nem beszél, minden a vörös zónában kell legyen (biztosan nem beszéd).</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="543"/>
        <source>Quality &amp; Notifications</source>
        <translation>Minőség és értesítések</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="546"/>
        <source>Adjust quality and notification settings.</source>
        <translation>Minőség és értesítések beállítása.</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="558"/>
        <source>Quality settings</source>
        <translation>Minőség beállításai</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="576"/>
        <source>Low</source>
        <translation>Alacsony</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="583"/>
        <source>In this configuration Mumble will use a &lt;b&gt;low amount of bandwidth&lt;/b&gt;. This will inevitably result in high latency and poor quality. Choose this only if your connection cannot handle the other settings. (Speex 16kbit/s, 60ms per packet)</source>
        <translation>Ezzel a beállítással a Mumble &lt;b&gt;kevés sávszélességet&lt;/b&gt; fog használni. Elkerülhetetlenül nagyobb késleltetést és gyenge hangminőséget eredményez. Csak akkor válassza ezt, ha az Internet kapcsolata miatt más beállítás nem lehetséges. (Speex 16kbit/s, 60ms csomagonként)</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="599"/>
        <source>Balanced</source>
        <translation>Kiegyensúlyozott</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="609"/>
        <source>This is the &lt;b&gt;recommended default&lt;/b&gt; configuration. It provides a good balance between quality, latency, and bandwidth usage. (CELT 40kbit/s, 20ms per packet)</source>
        <translation>Ez az &lt;b&gt;alapértelmezetten ajánlott&lt;/b&gt; beállítás. Jó egyensúlyt biztosít a hangminőség, késleltetés és a használt sávszélesség között. (CELT 40kbit/s, 20ms csomagonként)</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="625"/>
        <source>High</source>
        <translation>Magas</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="632"/>
        <source>This configuration is only recommended for use in setups where bandwidth is not an issue, like a LAN. It provides the lowest latency supported by Mumble and &lt;b&gt;high quality&lt;/b&gt;. (CELT 72kbit/s, 10ms per packet)</source>
        <translation>Ez a beállítás csak a nagy sávszélességű hálózatokon ajánlott, mint például a helyi hálózatokon (LAN). A Mumble által támogatott legkisebb késleltetést biztosítja éa a &lt;b&gt;legmagasabb hangminőséget&lt;/b&gt;. (CELT 72kbit/s, 10ms csomagonként)</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="642"/>
        <source>Custom</source>
        <translation>Egyéni</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="649"/>
        <source>You already set a customized quality configuration in Mumble. Select this setting to keep it.</source>
        <translation>Ön a Mumble-ban már kiválasztott egy saját minőségbeállítást. Válassza ezt, ennek a megtartásához.</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="659"/>
        <source>Notification settings</source>
        <translation>Értesítések beállításai</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="665"/>
        <source>Use Text-To-Speech to read notifications and messages to you.</source>
        <translation>Az értesítések és üzenetek szövegének felolvasása.</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="675"/>
        <source>Disable Text-To-Speech and use sounds instead.</source>
        <translation>Szövegfelolvasás tiltása és helyette hangjelzések használata.</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="682"/>
        <source>Keep custom Text-To-Speech settings.</source>
        <translation>Saját szövegfelolvasási beállítások megtartása.</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="706"/>
        <source>Positional Audio</source>
        <translation>Hang pozicionálás</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="709"/>
        <source>Adjusting attenuation of positional audio.</source>
        <translation>A pozicionált hang csillapításának beállítása.</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="715"/>
        <source>&lt;p&gt;
Mumble supports positional audio for some games, and will position the voice of other users relative to their position in game. Depending on their position, the volume of the voice will be changed between the speakers to simulate the direction and distance the other user is at. Such positioning depends on your speaker configuration being correct in your operating system, so a test is done here.
&lt;/p&gt;
&lt;p&gt;
The graph below shows the position of &lt;font color=&quot;red&quot;&gt;you&lt;/font&gt;, the &lt;font color=&quot;yellow&quot;&gt;speakers&lt;/font&gt; and a &lt;font color=&quot;green&quot;&gt;moving sound source&lt;/font&gt; as if seen from above. You should hear the audio move between the channels.
&lt;/p&gt;
</source>
        <translation>&lt;p&gt;
Néhány játék esetén a Mumble támogatja a hangpozicionálást, és a többi játékos hangját, a játékban elfoglalt helyüknek megfelelően helyezi el térben. A többiek hangereje változni fog a két hangszóró között, hogy szimulálja a játékban elfoglalt helyük irányát és távolságát. Ez a fajta pozicionálás függ az ön operációs rendszerének korrekt hangszóróbeállításaitól, ezért itt egy teszt következik.
&lt;/p&gt;
&lt;p&gt;
A következő ábra felső nézetből mutatja az &lt;font color=&quot;red&quot;&gt;ön&lt;/font&gt; helyzetét, a &lt;font color=&quot;yellow&quot;&gt;hangszórókat&lt;/font&gt; és egy &lt;font color=&quot;green&quot;&gt;mozgó hangforrást&lt;/font&gt;. Önnek hallania kell a hang mozgását a csatornák között.
&lt;/p&gt;
</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="731"/>
        <source>Use headphones instead of speakers</source>
        <translation>Fejhallgató használata a hangszórók helyett</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="770"/>
        <source>&lt;p&gt;
Congratulations. You should now be ready to enjoy a richer sound experience with Mumble.
&lt;/p&gt;
&lt;p&gt;
Mumble is under continuous development, and the development team wants to focus on the features that benefit the most users. To this end, Mumble supports submitting anonymous statistics about your configuration to the developers. These statistics are essential for future development, and also make sure the features you use aren&apos;t deprecated.
&lt;/p&gt;
</source>
        <translation>&lt;p&gt;
Gratulálunk. Ön ezután egy gazdagabb hangzású Mumble-t használhat.
&lt;/p&gt;
&lt;p&gt;
A Mumble-t folyamatosan fejlesztik, és a fejlesztők azokra a tulajdonságokra szeretnének fókuszálni, amelyek a legtöbb felhasználónak hasznosak. Végül, a Mumble támogatja a beállításairól, név nélküli statisztika küldését a fejlesztőknek. Ezek a statisztikák lényegesek a jövőbeli fejlesztések szempontjából és biztossá teszik, hogy az ön használt tulajdonságok a fejlesztők fókuszában maradnak.
&lt;/p&gt;
</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="737"/>
        <source>Use headphones</source>
        <translation>Fejhallgatók használata</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="734"/>
        <source>This ignores the OS speaker configuration and configures the positioning for headphones instead.</source>
        <translation>Figyelmen kívül hagyja az operációs rendszer beállításait és helyette beállítja a fejhallgatókat a hangpzicionálásra.</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="319"/>
        <source>Volume tuning</source>
        <translation>Hangerő állítás</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="27"/>
        <source>&lt;p&gt;
This is the audio tuning wizard for Mumble. This will help you correctly set the input levels of your sound card, and also set the correct parameters for sound processing in Mumble.
&lt;/p&gt;
&lt;p&gt;
Please be aware that as long as this wizard is active, audio will be looped locally to allow you to listen to it, and no audio will be sent to the server.
&lt;/p&gt;</source>
        <translation>&lt;p&gt;
Ez a Mumble hangbeállító varázslója. Segíteni fogja önt a hangkártya bemeneti szintjeinek és a Mumble hangfeldolgozó paramétereinek helyes beállításában.
&lt;/p&gt;
&lt;p&gt;
Kérem legyen tudatában annak, hogy amíg ez a varázsló aktív, a hang csak helyileg, az ön számára lesz hallható, és a szerver felé semmilyen hang nem lesz továbbítva.
&lt;/p&gt;</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="65"/>
        <source>Input Device</source>
        <translation>Bemeneti eszköz</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="137"/>
        <source>Output Device</source>
        <translation>Kimeneti eszköz</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="206"/>
        <source>Enables attenuation of other applications while users talk to you</source>
        <translation>Amikor mások beszélnek önhöz, engedélyezi a többi alkalmazás hangjának csökkentését</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="209"/>
        <source>Enables attenuation of other applications while users talk to you. This means that as soon someone starts to speak to you in Mumble, the sound of all other applications (like audio players) will get attenuated so you can hear them more clearly.</source>
        <translation>Amikor mások beszélnek önhöz, engedélyezi a többi alkalmazás hangjának a csökkentését. Ez azt jelenti, hogy amikor a Mumble programban valaki elkezd önhöz beszélni, a többi alkalmazás (mint például a zenelejátszók) hangját lecsökkenti, hogy tisztábban hallhassa a beszélőt.</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="212"/>
        <source>Attenuate applications while other users talk</source>
        <translation>Míg mások beszélnek halkítja az alkalmazásokat</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="245"/>
        <source>&lt;p&gt;
To keep latency to an absolute minimum, it&apos;s important to buffer as little audio as possible on the soundcard. However, many soundcards report that they require a much smaller buffer than what they can actually work with, so the only way to set this value is to try and fail.
&lt;/p&gt;
&lt;p&gt;
You should hear a voice sample. Change the slider below to the lowest value which gives &lt;b&gt;no&lt;/b&gt; interruptions or jitter in the sound. Please note that local echo is disabled during this test.
&lt;/p&gt;
</source>
        <translation>&lt;p&gt;
Ahhoz, hogy a késleltetés minimális legyen fontos, hogy a hangkártyán minél kevesebb hangot puffereljen. Mivel sok hangkártya az jelzi, hogy kevesebb puffert igényel, mint amennyivel ténylegesen működni tud, a jó beállítás csak próbálgatással lehetséges.
&lt;/p&gt;
&lt;p&gt;
Egy hangmintát kell halljon. Állítsa a csúszkát a legkisebb érték alá, amely &lt;b&gt;nem&lt;/b&gt; okoz szakadásokat vagy nyávogást a hangban. Vegye figyelembe, hogy tesztelés közben a helyi visszhang ki van kapcsolva.
&lt;/p&gt;
</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="267"/>
        <source>Amount of data to buffer</source>
        <translation>Pufferelendő adatmennyiség</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="270"/>
        <source>This sets the amount of data to pre-buffer in the output buffer. Experiment with different values and set it to the lowest which doesn&apos;t cause rapid jitter in the sound.</source>
        <translation>A kimeneti tárolóba előpufferelt adatmennyiséget állítja be. Kísérletezzen különböző értékekkel és válassza a legkisebbet, amely nem okoz hirtelen nyávogást a hangban.</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="322"/>
        <source>Tuning microphone hardware volume to optimal settings.</source>
        <translation>A mikrofon hardverének optimális hangerő beállítása.</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="354"/>
        <source>Now talk softly, as you would when talking late at night and you don&apos;t want to disturb anyone. Adjust the slider below so that the bar moves into green when you talk, but stays blue while you&apos;re silent.</source>
        <translation>Most beszéljen halkan úgy, mint amikor késő este nem szeretne senkit zavarni. Állítsa a lenti csúszkát úgy, hogy a csík beszéd esetén a zöldben, ha nem beszél, akkor pedig a kékben tartózkodjon.</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="421"/>
        <source>Voice Activity Detection</source>
        <translation>Beszéd érzékelése</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="424"/>
        <source>Letting Mumble figure out when you&apos;re talking and when you&apos;re silent.</source>
        <translation>Engedélyezi, hogy a Mumble megállapítsa, hogy mikor beszél és mikor hallgat.</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="430"/>
        <source>This will help Mumble figure out when you are talking. The first step is selecting which data value to use.</source>
        <translation>Segíti a Mumble-t, hogy megállapítsa mikor beszél. Az első lépés a használni kívánt adat kiválasztása.</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="474"/>
        <source>Raw amplitude from input</source>
        <translation>Feldolgozatlan bemeneti amplitúdó</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="467"/>
        <source>Signal-To-Noise ratio</source>
        <translation>Jel-zaj viszony</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="236"/>
        <source>Device tuning</source>
        <translation>Eszköz beállítása</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="239"/>
        <source>Changing hardware output delays to their minimum value.</source>
        <translation>A hardver kimenet késleltetésének minimalizálása.</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="786"/>
        <source>Submit anonymous statistics to the Mumble project</source>
        <translation>Névtelen statisztika küldése a Mumble fejlesztőinek</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui" line="442"/>
        <source>Push To Talk:</source>
        <translation>Beszédhez-nyomd:</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="314"/>
        <source>%1 ms</source>
        <translation>%1 ms</translation>
    </message>
</context>
<context>
    <name>BanEditor</name>
    <message>
        <location filename="BanEditor.ui" line="14"/>
        <source>Mumble - Edit Bans</source>
        <translation>Mumble - Kitiltások szerkesztése</translation>
    </message>
    <message>
        <location filename="BanEditor.ui" line="29"/>
        <source>&amp;Address</source>
        <translation>&amp;Cím</translation>
    </message>
    <message>
        <location filename="BanEditor.ui" line="39"/>
        <source>&amp;Mask</source>
        <translation>&amp;Maszk</translation>
    </message>
    <message>
        <location filename="BanEditor.ui" line="72"/>
        <source>Reason</source>
        <translation>Ok</translation>
    </message>
    <message>
        <location filename="BanEditor.ui" line="92"/>
        <source>Start</source>
        <translation>Kezdete</translation>
    </message>
    <message>
        <location filename="BanEditor.ui" line="99"/>
        <source>End</source>
        <translation>Vége</translation>
    </message>
    <message>
        <location filename="BanEditor.ui" line="113"/>
        <source>User</source>
        <translation>Felhasználó</translation>
    </message>
    <message>
        <location filename="BanEditor.ui" line="127"/>
        <source>Hash</source>
        <translation>Kivonat</translation>
    </message>
    <message>
        <location filename="BanEditor.ui" line="145"/>
        <source>&amp;Add</source>
        <translation>&amp;Hozzáadás</translation>
    </message>
    <message>
        <location filename="BanEditor.ui" line="152"/>
        <source>&amp;Update</source>
        <translation>&amp;Frissítés</translation>
    </message>
    <message>
        <location filename="BanEditor.ui" line="159"/>
        <source>&amp;Remove</source>
        <translation>&amp;Eltávolítás</translation>
    </message>
</context>
<context>
    <name>CertView</name>
    <message>
        <location filename="Cert.cpp" line="50"/>
        <source>Name</source>
        <translation>Név</translation>
    </message>
    <message>
        <location filename="Cert.cpp" line="57"/>
        <source>Email</source>
        <translation>Email</translation>
    </message>
    <message>
        <location filename="Cert.cpp" line="64"/>
        <source>Issuer</source>
        <translation>Kibocsájtó</translation>
    </message>
    <message>
        <location filename="Cert.cpp" line="71"/>
        <source>Expiry Date</source>
        <translation>Lejárat dátuma</translation>
    </message>
    <message>
        <location filename="Cert.cpp" line="105"/>
        <source>(none)</source>
        <translation>(semmi)</translation>
    </message>
    <message>
        <location filename="Cert.cpp" line="116"/>
        <source>Self-signed</source>
        <translation>saját aláírású</translation>
    </message>
</context>
<context>
    <name>CertWizard</name>
    <message>
        <location filename="Cert.cpp" line="206"/>
        <source>Resolving domain %1.</source>
        <translation>%1 doménnév feloldása.</translation>
    </message>
    <message>
        <location filename="Cert.cpp" line="212"/>
        <source>Unable to validate email.&lt;br /&gt;Enter a valid (or blank) email to continue.</source>
        <translation>Nem tudom jóváhagyni az email címet.&lt;br /&gt;Adjon meg egy érvényes (vagy üres) email címet a folytatáshoz.</translation>
    </message>
    <message>
        <location filename="Cert.cpp" line="220"/>
        <source>There was an error generating your certificate.&lt;br /&gt;Please try again.</source>
        <translation>Egy hiba történt az ön tanúsítványának létrehozása közben.&lt;br /&gt;Kérem próbálja meg újra.</translation>
    </message>
    <message>
        <location filename="Cert.cpp" line="228"/>
        <source>Your certificate and key could not be exported to PKCS#12 format. There might be an error in your certificate.</source>
        <translation>Az ön tanúsítványa és kulcsa nem exportálható PKCS#12 formátumba. A tanúsítványa hibás lehet.</translation>
    </message>
    <message>
        <location filename="Cert.cpp" line="233"/>
        <source>The file could not be opened for writing. Please use another file.</source>
        <translation>A fájl nem nyitható meg írásra. Kérem használjon másik fájlt.</translation>
    </message>
    <message>
        <location filename="Cert.cpp" line="239"/>
        <source>The file could not be written successfully. Please use another file.</source>
        <translation>A fájlba nem sikerült írni. Kérem használjon másik fájlt.</translation>
    </message>
    <message>
        <location filename="Cert.cpp" line="246"/>
        <source>The file could not be opened for reading. Please use another file.</source>
        <translation>A fájl nem nyitható meg olvasásra. Kérem használjon másik fájlt.</translation>
    </message>
    <message>
        <location filename="Cert.cpp" line="252"/>
        <source>The file is empty or could not be read. Please use another file.</source>
        <translation>A fájl üres vagy nem olvasható. Kérem használjon másik fájlt.</translation>
    </message>
    <message>
        <location filename="Cert.cpp" line="257"/>
        <source>The file did not contain a valid certificate and key. Please use another file.</source>
        <translation>A fájl nem tartalmaz érvényes tanúsítványt és kulcsot. Kérem használjon másik fájlt.</translation>
    </message>
    <message>
        <location filename="Cert.cpp" line="279"/>
        <source>Select file to export certificate to</source>
        <translation>Válasszon egy fájlt a tanúsítvány exportálásához</translation>
    </message>
    <message>
        <location filename="Cert.cpp" line="316"/>
        <source>Select file to import certificate from</source>
        <translation>Válasszon egy fájlt, amiből tanúsítvány importálható</translation>
    </message>
    <message>
        <location filename="Cert.cpp" line="373"/>
        <source>Unable to resolve domain.</source>
        <translation>Nem sikerült feloldani a doménnevet.</translation>
    </message>
    <message>
        <location filename="Cert.cpp" line="416"/>
        <source>Mumble User</source>
        <translation>Mumble felhasználó</translation>
    </message>
    <message>
        <location filename="main.cpp" line="390"/>
        <source>&lt;b&gt;Certificate Expiry:&lt;/b&gt; Your certificate is about to expire. You need to renew it, or you will no longer be able to connect to servers you are registered on.</source>
        <translation>&lt;b&gt;Tanúsítvány lejár:&lt;/b&gt; Az ön tanúsítványa nem sokára lejár. Meg kell újitsa, vagy nem lesz képes többet bejelentkezni azokra a szerverekre, ahol regisztrálva van.</translation>
    </message>
</context>
<context>
    <name>Certificates</name>
    <message>
        <location filename="Cert.ui" line="14"/>
        <source>Certificate Management</source>
        <translation>Tanúsítvány kezelés</translation>
    </message>
    <message>
        <location filename="Cert.ui" line="18"/>
        <source>Certificate Authentication</source>
        <translation>Hitelesítés tanusítvánnyal</translation>
    </message>
    <message>
        <location filename="Cert.ui" line="21"/>
        <source>Authenticating to servers without using passwords</source>
        <translation>Hitelesítés a szerveren jelszó nélkül</translation>
    </message>
    <message>
        <location filename="Cert.ui" line="30"/>
        <source>&lt;p&gt;Mumble can use certificates to authenticate with servers. Using certificates avoids passwords, meaning you don&apos;t need to disclose any password to the remote site. It also enables very easy user registration.&lt;/p&gt;&lt;p&gt;While Mumble can work without certificates, the majority of servers will expect you to have one.&lt;/p&gt;
&lt;p&gt;
It is &lt;b&gt;strongly&lt;/b&gt; recommended that you &lt;a href=&quot;http://mumble.info/certificate.php&quot;&gt;create a trusted certificate&lt;/a&gt;.
&lt;/p&gt;
</source>
        <translation>&lt;p&gt;A Mumble képes a szervereken tanusítvánnyal hitelesíteni magát. A tanúsítványok használatával nincs szükség a jelszavakra, ami azt jelenti, hogy nem kell felfednie semmilyen jelszót a másik fél előtt. Ugyanakkor lehetővé teszi a nagyon egyszerű felhasználó regisztrálást.&lt;/p&gt;&lt;p&gt;Ugyan a Mumble képes tanúsítványok nélkül is dolgozni, de a legtöbb szerver elvárja, hogy rendelkezzen legalább egyel.&lt;/p&gt;
&lt;p&gt;
&lt;b&gt;Erősen&lt;/b&gt; ajánlott, hogy &lt;a href=&quot;http://mumble.info/certificate.php&quot;&gt;készítsen egy megbízható (hitelesített) tanúsítványt&lt;/a&gt;.
&lt;/p&gt;
</translation>
    </message>
    <message>
        <location filename="Cert.ui" line="44"/>
        <location filename="Cert.ui" line="282"/>
        <location filename="Cert.ui" line="369"/>
        <source>Current certificate</source>
        <translation>Jelenlegi tanúsítvány</translation>
    </message>
    <message>
        <location filename="Cert.ui" line="47"/>
        <source>This is the certificate Mumble currently uses.</source>
        <translation>Jelenleg a Mumble ezt a tanúsítványt használja.</translation>
    </message>
    <message>
        <location filename="Cert.ui" line="50"/>
        <location filename="Cert.ui" line="288"/>
        <source>Current Certificate</source>
        <translation>Jelenlegi tanúsítvány</translation>
    </message>
    <message>
        <location filename="Cert.ui" line="57"/>
        <source>Automatic certificate creation</source>
        <translation>Tanúsítvány készítése automatikusan</translation>
    </message>
    <message>
        <location filename="Cert.ui" line="74"/>
        <location filename="Cert.ui" line="80"/>
        <source>Create a new certificate</source>
        <translation>Új tanúsítvány készítése</translation>
    </message>
    <message>
        <location filename="Cert.ui" line="77"/>
        <source>This will create a new certificate.</source>
        <translation>Ezzel egy új tanúsítványt hoz létre.</translation>
    </message>
    <message>
        <location filename="Cert.ui" line="100"/>
        <source>Import certificate from file</source>
        <translation>Tanúsítvány importálása fájlból</translation>
    </message>
    <message>
        <location filename="Cert.ui" line="103"/>
        <source>This will import a certificate from file.</source>
        <translation>Ezzel egy fájlból importál egy tanúsítványt.</translation>
    </message>
    <message>
        <location filename="Cert.ui" line="106"/>
        <source>Import a certificate</source>
        <translation>Tanúsítvány importálása</translation>
    </message>
    <message>
        <location filename="Cert.ui" line="126"/>
        <location filename="Cert.ui" line="315"/>
        <source>Export Certificate</source>
        <translation>Tanúsítvány exportálása</translation>
    </message>
    <message>
        <location filename="Cert.ui" line="129"/>
        <source>This will export a certificate to file.</source>
        <translation>Ezzel a tanúsítványt egy fájlba írja.</translation>
    </message>
    <message>
        <location filename="Cert.ui" line="132"/>
        <source>Export current certificate</source>
        <translation>Aktuális tanúsítvány exportálása</translation>
    </message>
    <message>
        <location filename="Cert.ui" line="150"/>
        <source>Import Certificate</source>
        <translation>Tanúsítvány importálása</translation>
    </message>
    <message>
        <location filename="Cert.ui" line="153"/>
        <source>PKCS #12 Certificate import</source>
        <translation>PKCS #12 Tanúsítvány importálása</translation>
    </message>
    <message>
        <location filename="Cert.ui" line="162"/>
        <source>&lt;p&gt;Mumble can import certificates stored in PKCS #12 format. This is the format used when exporting a key from Mumble, and also when exporting keys from Firefox, Internet Explorer, Opera etc.&lt;/p&gt;&lt;p&gt;If the file is password protected, you will need the password to import the certificate.&lt;/p&gt;</source>
        <translation>&lt;p&gt;A Mumble képes a PKCS #12 formátumban tárolt tanúsítványokat importálni. Ez az a formátum, amiben egy kulcsot (tanúsítványt) exportál a Mumble és szintén ebben a formátumban exportál a Firefox, az Internet Explorer, az Opera, stb.&lt;/p&gt;&lt;p&gt;Ha a fájl jelszóval védett, a tanúsítvány importáláshoz szüksége lesz erre a jelszóra.&lt;/p&gt;</translation>
    </message>
    <message>
        <location filename="Cert.ui" line="172"/>
        <source>Import from</source>
        <translation>Importálás</translation>
    </message>
    <message>
        <location filename="Cert.ui" line="182"/>
        <source>Filename to import from</source>
        <translation>Fájlnév az importáláshoz</translation>
    </message>
    <message>
        <location filename="Cert.ui" line="185"/>
        <source>This is the filename you wish to import a certificate from.</source>
        <translation>Ebből a fájlból fog importálni egy tanúsítványt.</translation>
    </message>
    <message>
        <location filename="Cert.ui" line="192"/>
        <source>Select file to import from</source>
        <translation>Importáláshoz válasszon fájlt</translation>
    </message>
    <message>
        <location filename="Cert.ui" line="195"/>
        <source>This opens a file selection dialog to choose a file to import a certificate from.</source>
        <translation>Megnyit egy fájlválasztó párbeszédablakot, hogy kiválaszthassa a fájlt a tanúsítvány importáláshoz.</translation>
    </message>
    <message>
        <location filename="Cert.ui" line="198"/>
        <source>Open...</source>
        <translation>Megnyitás...</translation>
    </message>
    <message>
        <location filename="Cert.ui" line="209"/>
        <source>Password</source>
        <translation>Jelszó</translation>
    </message>
    <message>
        <location filename="Cert.ui" line="219"/>
        <source>Password for PKCS#12 file</source>
        <translation>Jelszó a PKCS#12 fájlhoz</translation>
    </message>
    <message>
        <location filename="Cert.ui" line="222"/>
        <source>This is the password for the PKCS#12 file containing your certificate.</source>
        <translation>Az ön tanúsítványát tartalmazó PKCS#12 fájl jelszava.</translation>
    </message>
    <message>
        <location filename="Cert.ui" line="229"/>
        <source>Certificate to import</source>
        <translation>Importálandó tanúsítvány</translation>
    </message>
    <message>
        <location filename="Cert.ui" line="232"/>
        <source>This is the certificate you are importing.</source>
        <translation>Az ön által importált tanúsítvány.</translation>
    </message>
    <message>
        <location filename="Cert.ui" line="235"/>
        <location filename="Cert.ui" line="375"/>
        <source>Certificate Details</source>
        <translation>Tanúsítvány részletei</translation>
    </message>
    <message>
        <location filename="Cert.ui" line="243"/>
        <source>Replace Certificate</source>
        <translation>Tanúsítvány cseréje</translation>
    </message>
    <message>
        <location filename="Cert.ui" line="246"/>
        <source>Replace existing certificate with new certificate?</source>
        <translation>Kicseréli a jelenlegi tanúsítványt egy újjal?</translation>
    </message>
    <message>
        <location filename="Cert.ui" line="258"/>
        <source>&lt;p&gt;You already have a certificate stored in Mumble, and you are about to replace it.&lt;/p&gt;
&lt;p&gt;If you are upgrading to a certificate issued to you by a trusted CA and the email addresses match your current certificate, this is completely safe, and servers you connect to will automatically recognize the strong certificate for your email address.
&lt;/p&gt;
&lt;p&gt;If this is not the case, you will no longer be recognized by any server you previously have authenticated with. If you haven&apos;t been registered on any server yet, this is nothing to worry about.
&lt;/p&gt;
&lt;p&gt;
Are you sure you wish to replace your certificate?
&lt;/p&gt;
</source>
        <translation>&lt;p&gt;Már van egy elmentett tanúsítványa a Mumble-ban és most éppen a kicserélésére készül.&lt;/p&gt;
&lt;p&gt;Ha egy olyan tanúsítványra cseréli a mostanit, amely megbízható tanúsítványkiadó központ (CA) adott ki és az email címe egyezik a jelenleg használttal, akkor ez teljesen biztonságos és a szerverek, amelyekhez kapcsolódik, automatikusan fel fogják ismerni az ön email címével kiállított erős titkosítású tanúsítványt.
&lt;/p&gt;
&lt;p&gt;Ha nem ez a helyzet, akkor ezután nem fogja tudni azonosítani egy szerver sem, amelyeknél korábban hitelesítette magát. Ha még nem regisztrálta magát egy szerveren sem, akkor nincs miért aggódnia.
&lt;/p&gt;
&lt;p&gt;
Biztos abban, hogy le akarja cserélni a tanúsítványát?
&lt;/p&gt;
</translation>
    </message>
    <message>
        <location filename="Cert.ui" line="285"/>
        <source>This is the certificate Mumble currently uses. It will be replaced.</source>
        <translation>A Mumble jelenleg ezt a tanúsítványt használja. Ki lesz cserélve.</translation>
    </message>
    <message>
        <location filename="Cert.ui" line="301"/>
        <source>New certificate</source>
        <translation>Új tanúsítvány</translation>
    </message>
    <message>
        <location filename="Cert.ui" line="304"/>
        <source>This is the new certificate that will replace the old one.</source>
        <translation>Ez az új tanúsítvány, ami cserélni fogja az újat.</translation>
    </message>
    <message>
        <location filename="Cert.ui" line="307"/>
        <location filename="Cert.ui" line="383"/>
        <source>New Certificate</source>
        <translation>Új tanúsítvány</translation>
    </message>
    <message>
        <location filename="Cert.ui" line="318"/>
        <source>Make a backup of your certificate</source>
        <translation>Készítsen egy másolatot a tanúsítványáról</translation>
    </message>
    <message>
        <location filename="Cert.ui" line="327"/>
        <source>&lt;p&gt;If you ever lose your current certificate, which will happen if your computer suffers a hardware failure or you reinstall your machine, you will no longer be able to authenticate to any server you are registered on. It is therefore &lt;b&gt;mandatory&lt;/b&gt; that you make a backup of your certificate. We strongly recommend you store this backup on removable storage, such as an USB memory stick.&lt;/p&gt;
&lt;p&gt;Note that this file will not be encrypted, and if anyone gains access to it, they will be able to impersonate you, so take good care of it.&lt;/p&gt;</source>
        <translation>&lt;p&gt;Ha valaha is elveszti a jelenlegi tanúsítványát, ami megtörténhet a számítógépének hardveres meghibásodása vagy egy ujratelepítés miatt, nem lesz képes többé hitelesítenie magát egyik szerveren sem, amelyeken előzőleg regisztrálta magát. Ezért &lt;b&gt;kötelező&lt;/b&gt; dolog a tanúsítványáról biztonsági másolatot készítenie. Erősen ajánljuk, hogy ezt a másolatot egy kivehető tároló eszközön tartsa, mint amilyen egy USB-s pendrive.&lt;/p&gt;
&lt;p&gt;Figyelem, ez a fájl nem lesz titkosítva és ha bárki megszerzi azt, képes lesz az ön nevében cselekedni, tehát gondosan őrizze.&lt;/p&gt;</translation>
    </message>
    <message>
        <location filename="Cert.ui" line="338"/>
        <source>Export to</source>
        <translation>Exportálás</translation>
    </message>
    <message>
        <location filename="Cert.ui" line="348"/>
        <source>Filename to export to</source>
        <translation>Fájlnév az exportáláshoz</translation>
    </message>
    <message>
        <location filename="Cert.ui" line="351"/>
        <source>This is the filename you wish to export a certificate to.</source>
        <translation>Ebbe a fájlba fogja exportálni a tanúsítványt.</translation>
    </message>
    <message>
        <location filename="Cert.ui" line="358"/>
        <source>Save As...</source>
        <translation>Mentés másként...</translation>
    </message>
    <message>
        <location filename="Cert.ui" line="372"/>
        <source>This is the certificate Mumble currently uses. It will be exported.</source>
        <translation>Jelenleg a Mumble ezt a tanúsítványt használja. Exportálva lesz.</translation>
    </message>
    <message>
        <location filename="Cert.ui" line="386"/>
        <source>Generate a new certificate for strong authentication</source>
        <translation>Új tanúsítvány készítése erős titkosítással</translation>
    </message>
    <message>
        <location filename="Cert.ui" line="395"/>
        <source>&lt;p&gt;Mumble will now generate a strong certificate for authentication to servers.&lt;/p&gt;&lt;p&gt;If you wish, you may provide some additional information to be stored in the certificate, which will be presented to servers when you connect. If you provide a valid email address, you can upgrade to a CA issued email certificate later on, which provides strong identification.&lt;/p&gt;</source>
        <translation>&lt;p&gt;A mumble a szervereken való hitelesítéshez készíteni fog egy erős titkosítású tanúsítványt.&lt;/p&gt;&lt;p&gt;Ha szeretné, a tanúsítványhoz megadhat néhány további információt, amely kapcsolódáskor megjelenik a szervereken. Ha megad egy érvényes email címet, akkor később lecserélheti a jelenlegi tanúsítványt egy tanúsítványkiadó központ (CA) által hitelesítettre, amely erősebb titkosítást biztosít.&lt;/p&gt;</translation>
    </message>
    <message>
        <location filename="Cert.ui" line="405"/>
        <source>Name</source>
        <translation>Név</translation>
    </message>
    <message>
        <location filename="Cert.ui" line="415"/>
        <source>Email</source>
        <translation>Email</translation>
    </message>
    <message>
        <location filename="Cert.ui" line="425"/>
        <source>Your email address (e.g. johndoe@mumble.info)</source>
        <translation>Az ön email címe (pl. johndoe@mumble.info)</translation>
    </message>
    <message>
        <location filename="Cert.ui" line="428"/>
        <source>This is your email address. It is strongly recommended to provide a valid email address, as this will allow you to upgrade to a strong certificate without authentication problems.</source>
        <translation>Ez az ön email címe. Erősen ajánlott, hogy egy valódi email címet adjon meg, mert ez teszi lehetővé, hogy lecserélje a tanúsítványát erősebbre, amellyel probléma mentes a hitelesítés.</translation>
    </message>
    <message>
        <location filename="Cert.ui" line="442"/>
        <source>Your name (e.g. John Doe)</source>
        <translation>Az ön neve (pl. John Doe)</translation>
    </message>
    <message>
        <location filename="Cert.ui" line="445"/>
        <source>This is your name, and will be filled out in the certificate. This field is entirely optional.</source>
        <translation>Ez az ön neve, ami a tanúsítványban szerepelni fog. Ezt a mezőt nem kötelező kitölteni.</translation>
    </message>
    <message>
        <location filename="Cert.ui" line="453"/>
        <source>Finish</source>
        <translation>Befejezés</translation>
    </message>
    <message>
        <location filename="Cert.ui" line="456"/>
        <source>Certificate-based authentication is ready for use</source>
        <translation>A tanúsítvány alapú hitelesítés készen áll a használatra</translation>
    </message>
    <message>
        <location filename="Cert.ui" line="465"/>
        <source>Enjoy using Mumble with strong authentication.</source>
        <translation>Élvezze az erős hitelesítéssel rendelkező Mumble-t.</translation>
    </message>
</context>
<context>
    <name>ChatbarLineEdit</name>
    <message>
        <location filename="CustomElements.cpp" line="71"/>
        <source>Paste and send</source>
        <translation>Beillesztés és küldés</translation>
    </message>
    <message>
        <location filename="CustomElements.cpp" line="85"/>
        <source>Type chat message here</source>
        <translation>A csevegés üzenetét írja ide</translation>
    </message>
</context>
<context>
    <name>ClientUser</name>
    <message>
        <location filename="ClientUser.cpp" line="122"/>
        <source>Friend</source>
        <translation>Barát</translation>
    </message>
    <message>
        <location filename="ClientUser.cpp" line="124"/>
        <source>Authenticated</source>
        <translation>Azonosítva</translation>
    </message>
    <message>
        <location filename="ClientUser.cpp" line="126"/>
        <source>Muted (server)</source>
        <translation>Némítva (szerver)</translation>
    </message>
    <message>
        <location filename="ClientUser.cpp" line="128"/>
        <source>Deafened (server)</source>
        <translation>Hallgatás ki (szerver)</translation>
    </message>
    <message>
        <location filename="ClientUser.cpp" line="130"/>
        <source>Local Mute</source>
        <translation>Helyi némítás</translation>
    </message>
    <message>
        <location filename="ClientUser.cpp" line="132"/>
        <source>Muted (self)</source>
        <translation>Némítva (magát)</translation>
    </message>
    <message>
        <location filename="ClientUser.cpp" line="134"/>
        <source>Deafened (self)</source>
        <translation>Hallgatás ki (magát)</translation>
    </message>
</context>
<context>
    <name>ConfigDialog</name>
    <message>
        <location filename="ConfigDialog.cpp" line="51"/>
        <source>Accept changes</source>
        <translation>Változások elfogadása</translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="52"/>
        <source>This button will accept current settings and return to the application.&lt;br /&gt;The settings will be stored to disk when you leave the application.</source>
        <translation>Ezzel a gombbal elfogadja a jelenlegi beállításokat és visszatér az alkalmazáshoz.&lt;br /&gt;Kikapcsoláskor elmenti lemezre a beállításokat.</translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="56"/>
        <source>Reject changes</source>
        <translation>Változtatások elvetése</translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="57"/>
        <source>This button will reject all changes and return to the application.&lt;br /&gt;The settings will be reset to the previous positions.</source>
        <translation>Ezzel a gombbal elveti az összes változtatást és visszatér az alkalmazáshoz.&lt;br /&gt;A beállítások az előző állapotba kerülnek.</translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="61"/>
        <source>Apply changes</source>
        <translation>Változtatások alkalmazása</translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="62"/>
        <source>This button will immediately apply all changes.</source>
        <translation>Ezzel a gombbal a változtatásokat azonnal alkalmazza.</translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="65"/>
        <source>Undo changes for current page</source>
        <translation>Az oldal módosításait megszünteti</translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="66"/>
        <source>This button will revert any changes done on the current page to the most recent applied settings.</source>
        <translation>Ezzel a gombbal az aktuális oldalon történt változtatásokat megszüntetve visszatér a legutóbbi beállításokhoz.</translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="69"/>
        <source>Restore defaults for current page</source>
        <translation>Az oldalt alapértelmezettre állítja</translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="70"/>
        <source>This button will restore the settings for the current page only to their defaults. Other pages will not be changed.&lt;br /&gt;To restore all settings to their defaults, you will have to use this button on every page.</source>
        <translation>Ezzel a gombbal az aktuális oldal beállításait az alapértelmezett értékre hozza. Más oldalakat nem módosít.&lt;br /&gt;Ahhoz, hogy minden beállítást alapértelmezettre állítson, minden oldalon használnia kell ezt a gombot.</translation>
    </message>
    <message>
        <location filename="ConfigDialog.ui" line="14"/>
        <source>Mumble Configuration</source>
        <translation>Mumble beállítások</translation>
    </message>
    <message>
        <location filename="ConfigDialog.ui" line="54"/>
        <source>Advanced</source>
        <translation>Haladó</translation>
    </message>
</context>
<context>
    <name>ConnectDialog</name>
    <message>
        <location filename="ConnectDialog.cpp" line="397"/>
        <source>Adding host %1</source>
        <translation>Gép hozzáadása: %1</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="461"/>
        <source>Hostname</source>
        <translation>Gépnév</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="464"/>
        <source>Bonjour name</source>
        <translation>Bonjour-név</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="467"/>
        <source>Port</source>
        <translation>Portszám</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="468"/>
        <source>Addresses</source>
        <translation>Címek</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="471"/>
        <source>Website</source>
        <translation>Web-oldal</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="474"/>
        <source>Packet loss</source>
        <translation>Elveszett csomagok</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="477"/>
        <source>Ping (80%)</source>
        <translation>Kopogtatás (80%)</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="477"/>
        <location filename="ConnectDialog.cpp" line="479"/>
        <source>%1 ms</source>
        <translation>%1 ms</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="479"/>
        <source>Ping (95%)</source>
        <translation>Kopogtatás (95%)</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="481"/>
        <source>Bandwidth</source>
        <translation>Sávszélesség</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="481"/>
        <source>%1 kbit/s</source>
        <translation>%1 kbit/s</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="483"/>
        <source>Version</source>
        <translation>Verzió</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="771"/>
        <source>&amp;Filters</source>
        <translation>&amp;Szűrők</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="863"/>
        <source>Connecting to %1</source>
        <translation>Kapcsolódás: %1</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="397"/>
        <location filename="ConnectDialog.cpp" line="863"/>
        <source>Enter username</source>
        <translation>Adja meg a felhasználónevet</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="1431"/>
        <source>Failed to fetch server list</source>
        <translation>Szerverlista betöltése nem sikerült</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui" line="14"/>
        <source>Mumble Server Connect</source>
        <translation>Kapcsolódás Mumble szerverhez</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui" line="48"/>
        <location filename="ConnectDialog.cpp" line="460"/>
        <source>Servername</source>
        <translation>Szervernév</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui" line="53"/>
        <source>Ping</source>
        <translation>Kopogtatás</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui" line="58"/>
        <location filename="ConnectDialog.cpp" line="482"/>
        <source>Users</source>
        <translation>Felhasználók</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui" line="73"/>
        <source>Remove from Favorites</source>
        <translation>Eltávolítja a kedvencekből</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui" line="78"/>
        <source>&amp;Edit...</source>
        <translation>&amp;Szerkeszt...</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui" line="83"/>
        <location filename="ConnectDialog.cpp" line="734"/>
        <source>&amp;Add New...</source>
        <translation>Ú&amp;j hozzáadása...</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui" line="86"/>
        <source>Add custom server</source>
        <translation>Egyedi szerver hozzáadása</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui" line="91"/>
        <source>Add to &amp;Favorites</source>
        <translation>&amp;Hozzáadás a kedvencekhez</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui" line="96"/>
        <source>Open &amp;Webpage</source>
        <translation>&amp;Weblap megnyitása</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui" line="107"/>
        <source>Show &amp;Reachable</source>
        <translation>&amp;Elérhetőket mutatja</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui" line="110"/>
        <source>Show all servers that respond to ping</source>
        <translation>Az összes kopogtatásra (ping) válaszoló szervert mutatja</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui" line="121"/>
        <source>Show &amp;Populated</source>
        <translatorcomment>Bejelentkezett felhasználókkal rendelkezőket mutatja</translatorcomment>
        <translation>&amp;Benépesülteket mutatja</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui" line="124"/>
        <source>Show all servers with users</source>
        <translation>Az összes szervert mutatja, amelyeken felhasználók vannak</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui" line="132"/>
        <source>Show &amp;All</source>
        <translation>&amp;Mindet mutatja</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui" line="135"/>
        <source>Show all servers</source>
        <translation>Az összes szervert mutatja</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui" line="140"/>
        <source>&amp;Copy</source>
        <translation>&amp;Másolás</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui" line="143"/>
        <source>Copy favorite link to clipboard</source>
        <translation>Kedvenc kapcsolat másolása vágólapra</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui" line="148"/>
        <source>&amp;Paste</source>
        <translation>&amp;Beillesztés</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui" line="151"/>
        <source>Paste favorite from clipboard</source>
        <translatorcomment>Kedvenc beillesztése a vágólapról</translatorcomment>
        <translation></translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="732"/>
        <source>&amp;Connect</source>
        <translation>&amp;Kapcsolódás</translation>
    </message>
</context>
<context>
    <name>ConnectDialogEdit</name>
    <message>
        <location filename="ConnectDialogEdit.ui" line="14"/>
        <source>Edit Server</source>
        <translation>Szerver szerkesztése</translation>
    </message>
    <message>
        <location filename="ConnectDialogEdit.ui" line="30"/>
        <source>Name of the server</source>
        <translation>Szerver neve</translation>
    </message>
    <message>
        <location filename="ConnectDialogEdit.ui" line="33"/>
        <source>&lt;b&gt;Name&lt;/b&gt;&lt;br/&gt;
Name of the server. This is what the server will be named like in your serverlist and can be chosen freely.</source>
        <translation>&lt;b&gt;Szervernév&lt;/b&gt;&lt;/br&gt;
A szerver neve. Ezzel a névvel jelenik meg az ön szerver listájában és e név alapján választható ki.</translation>
    </message>
    <message>
        <location filename="ConnectDialogEdit.ui" line="41"/>
        <source>A&amp;ddress</source>
        <translation>&amp;Cím</translation>
    </message>
    <message>
        <location filename="ConnectDialogEdit.ui" line="51"/>
        <source>Internet address of the server.</source>
        <translation>A szerver Internet címe.</translation>
    </message>
    <message>
        <location filename="ConnectDialogEdit.ui" line="54"/>
        <source>&lt;b&gt;Address&lt;/b&gt;&lt;br/&gt;
Internet address of the server. This can be a normal hostname, an IPv4/IPv6 address or a Bonjour service identifier. Bonjour service identifiers have to be prefixed with a &apos;@&apos; to be recognized by Mumble.</source>
        <translation>&lt;b&gt;Cím&lt;/b&gt;&lt;br/&gt;
A szerver Internet címe. Ez lehet egy normál gépnév vagy IPV4/IPV6 cím vagy egy Bonjour szolgáltatás azonosító. A Bonjour szolgáltatás azonosítók elé egy @ jelet kell tenni, hogy a Mumble felismerje őket.</translation>
    </message>
    <message>
        <location filename="ConnectDialogEdit.ui" line="62"/>
        <source>&amp;Port</source>
        <translation>&amp;Portszám</translation>
    </message>
    <message>
        <location filename="ConnectDialogEdit.ui" line="72"/>
        <source>Port on which the server is listening</source>
        <translation>A portszám, amit a szerver figyel</translation>
    </message>
    <message>
        <location filename="ConnectDialogEdit.ui" line="75"/>
        <source>&lt;b&gt;Port&lt;/b&gt;&lt;br/&gt;
Port on which the server is listening. If the server is identified by a Bonjour service identifier this field will be ignored.</source>
        <translation>&lt;b&gt;Portszám&lt;/b&gt;&lt;br/&gt;
A portszám, amit a szerver figyel. Ha a szervert egy Bonjour szolgáltatás azonosító azonosítja, akkor ez a mező figyelmen kívül lesz hagyva.</translation>
    </message>
    <message>
        <location filename="ConnectDialogEdit.ui" line="83"/>
        <source>&amp;Username</source>
        <translation>&amp;Felhasználónév</translation>
    </message>
    <message>
        <location filename="ConnectDialogEdit.ui" line="93"/>
        <source>Username to send to the server</source>
        <translation>A szervernek küldött felhasználónév</translation>
    </message>
    <message>
        <location filename="ConnectDialogEdit.ui" line="96"/>
        <source>&lt;b&gt;Username&lt;/b&gt;&lt;br/&gt;
Username to send to the server. Be aware that the server can impose restrictions on how a username might look like. Also your username could already be taken by another user.</source>
        <translation>&lt;b&gt;Felhasználónév&lt;/b&gt;&lt;br/&gt;
A szervernek küldött felhasználónév. Figyeljen arra, hogy a szerver megszabhatja, hogy nézzen ki a felhasználónév. Az is lehet, hogy a nevét már más felhasználó használja.</translation>
    </message>
</context>
<context>
    <name>CoreAudioSystem</name>
    <message>
        <location filename="CoreAudio.cpp" line="85"/>
        <source>Default Device</source>
        <translation>Alapértelmezett eszköz</translation>
    </message>
</context>
<context>
    <name>CrashReporter</name>
    <message>
        <location filename="CrashReporter.cpp" line="36"/>
        <source>Mumble Crash Report</source>
        <translation>Mumble összeomlás-jelentés</translation>
    </message>
    <message>
        <location filename="CrashReporter.cpp" line="42"/>
        <source>&lt;p&gt;&lt;b&gt;We&apos;re terribly sorry, but it seems Mumble has crashed. Do you want to send a crash report to the Mumble developers?&lt;/b&gt;&lt;/p&gt;&lt;p&gt;The crash report contains a partial copy of Mumble&apos;s memory at the time it crashed, and will help the developers fix the problem.&lt;/p&gt;</source>
        <translation>&lt;p&gt;&lt;b&gt;Nagyon sajnáljuk, de úgy tűnik, hogy a Mumble összeomlott. Szeretne összeomlás-jelentést küldeni a Mumble fejlesztőinek?&lt;/b&gt;&lt;/p&gt;&lt;p&gt;Az összeomlás-jelentés a Mumble által, az összeomlás pillanatában használt memória részleges másolatát tartalmazza, és segíteni fogja a fejlesztőket, hogy kijavíthassák a hibát.&lt;/p&gt;</translation>
    </message>
    <message>
        <location filename="CrashReporter.cpp" line="50"/>
        <source>Email address (optional)</source>
        <translation>Email cím (nem kötelező)</translation>
    </message>
    <message>
        <location filename="CrashReporter.cpp" line="59"/>
        <source>Please describe briefly, in English, what you were doing at the time of the crash</source>
        <translation>Kérem írja le röviden, angol nyelven, hogy az összeomláskor éppen mit csinált</translation>
    </message>
    <message>
        <location filename="CrashReporter.cpp" line="64"/>
        <source>Send Report</source>
        <translation>Jelentés küldése</translation>
    </message>
    <message>
        <location filename="CrashReporter.cpp" line="67"/>
        <source>Don&apos;t send report</source>
        <translation>Ne küldje a jelentést</translation>
    </message>
    <message>
        <location filename="CrashReporter.cpp" line="92"/>
        <source>Crash upload successful</source>
        <translation>Összeomlás-jelentés feltöltése sikerült</translation>
    </message>
    <message>
        <location filename="CrashReporter.cpp" line="92"/>
        <source>Thank you for helping make Mumble better!</source>
        <translation>Köszönjük, hogy segíti a Mumble jobbá tételét!</translation>
    </message>
    <message>
        <location filename="CrashReporter.cpp" line="94"/>
        <location filename="CrashReporter.cpp" line="96"/>
        <source>Crash upload failed</source>
        <translation>Összeomlás-jelentés feltöltése nem sikerült</translation>
    </message>
    <message>
        <location filename="CrashReporter.cpp" line="94"/>
        <source>We&apos;re really sorry, but it appears the crash upload has failed with error %1 %2. Please inform a developer.</source>
        <translation>Nagyon sajnáljuk, de úgy tűnik, hogy az összeomlás-jelentés feltöltése nem sikerült, hiba oka: %1 %2. Kérjük értesítse a fejlesztőket.</translation>
    </message>
    <message>
        <location filename="CrashReporter.cpp" line="96"/>
        <source>This really isn&apos;t funny, but apparently there&apos;s a bug in the crash reporting code, and we&apos;ve failed to upload the report. You may inform a developer about error %1</source>
        <translation>Ez nem túl mulatságos, de kétségtelenül az összeomlás-jelentő kódjában van egy hiba, és a jelentés feltöltése nem sikerült. Értesítheti a fejlesztőket a hibáról: %1</translation>
    </message>
    <message>
        <location filename="CrashReporter.cpp" line="211"/>
        <source>Uploading crash report</source>
        <translation>Összeomlás-jelentés feltöltése</translation>
    </message>
    <message>
        <location filename="CrashReporter.cpp" line="211"/>
        <source>Abort upload</source>
        <translation>Feltöltés megszakítása</translation>
    </message>
</context>
<context>
    <name>DXAudioInput</name>
    <message>
        <location filename="DirectSound.cpp" line="174"/>
        <source>Default DirectSound Voice Input</source>
        <translation>Alapértelmezett DirectSound hangbemenet</translation>
    </message>
    <message>
        <location filename="DirectSound.cpp" line="637"/>
        <source>Opening chosen DirectSound Input device failed. No microphone capture will be done.</source>
        <translation>A kiválasztott DirectSound bemeneti eszköz megnyitása nem sikerült. Nem lesz mikrofon használatban.</translation>
    </message>
    <message>
        <location filename="DirectSound.cpp" line="558"/>
        <source>Opening chosen DirectSound Input failed. Default device will be used.</source>
        <translation>A kiválasztott DirectSound bemenet megnyitása nem sikerült. Az alapértelmezett eszköz fogom használni.</translation>
    </message>
    <message>
        <location filename="DirectSound.cpp" line="632"/>
        <source>Lost DirectSound input device.</source>
        <translation>A DirectSound bemeneti eszközt elvesztettem.</translation>
    </message>
</context>
<context>
    <name>DXAudioOutput</name>
    <message>
        <location filename="DirectSound.cpp" line="134"/>
        <source>Default DirectSound Voice Output</source>
        <translation>Alapértelmezett DirectSound hangkimenet</translation>
    </message>
    <message>
        <location filename="DirectSound.cpp" line="382"/>
        <source>Opening chosen DirectSound Output failed. Default device will be used.</source>
        <translation>A kiválasztott DirectSound kimenet megnyitása nem sikerült. Az alapértelmezett eszköz fogom használni.</translation>
    </message>
    <message>
        <location filename="DirectSound.cpp" line="460"/>
        <source>Lost DirectSound output device.</source>
        <translation>A DirectSound kimeneti eszközt elvesztettem.</translation>
    </message>
    <message>
        <location filename="DirectSound.cpp" line="464"/>
        <source>Opening chosen DirectSound Output failed. No audio will be heard.</source>
        <translation>A kiválasztott DirectSound kimenet megnyitása nem sikerült. Nem lesz hallható hang.</translation>
    </message>
</context>
<context>
    <name>Database</name>
    <message>
        <location filename="Database.cpp" line="84"/>
        <source>Mumble failed to initialize a database in any
of the possible locations.</source>
        <translation>A Mumble nem tudta betölteni az adatbázist
egyik lehetséges útvonalon sem.</translation>
    </message>
    <message>
        <location filename="Database.cpp" line="91"/>
        <source>The database &apos;%1&apos; is read-only. Mumble can not store server settings (ie. SSL certificates) until you fix this problem.</source>
        <translation>A %1 adatbázis csak olvasható. A Mumble nem tudja elmenteni a szerver beállításait (például az SSL tanúsítványokat), ameddig nem oldja meg ezt a problémát.</translation>
    </message>
</context>
<context>
    <name>GlobalShortcut</name>
    <message>
        <location filename="GlobalShortcut.ui" line="17"/>
        <source>Shortcuts</source>
        <translation>Gyorsbillentyűk</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.ui" line="23"/>
        <source>Add new shortcut</source>
        <translation>Gyorsbillentyű hozzáadása</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.ui" line="26"/>
        <source>This will add a new global shortcut</source>
        <translation>Ezzel hozzáadhat egy új globális gyorsbillentyűt</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.ui" line="29"/>
        <source>&amp;Add</source>
        <translation>&amp;Hozzáadás</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.ui" line="39"/>
        <source>Remove selected shortcut</source>
        <translation>A kiválasztott gyorsbillentyű eltávolítása</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.ui" line="42"/>
        <source>This will permanently remove a selected shortcut.</source>
        <translation>Ezzel véglegesen eltávolíthatja a kiválasztott gyorsbillentyűt.</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.ui" line="45"/>
        <source>&amp;Remove</source>
        <translation>&amp;Eltávolítás</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.ui" line="65"/>
        <source>List of configured shortcuts</source>
        <translation>A beállított gyorsbillentyűk listája</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.ui" line="99"/>
        <source>Function</source>
        <translation>Funkció</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.ui" line="104"/>
        <source>Data</source>
        <translation>Adat</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.ui" line="109"/>
        <source>Shortcut</source>
        <translation>Gyorsbillentyű</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.ui" line="114"/>
        <source>Suppress</source>
        <translation>Elnyomás</translation>
    </message>
</context>
<context>
    <name>GlobalShortcutConfig</name>
    <message>
        <location filename="GlobalShortcut.cpp" line="654"/>
        <source>Shortcuts</source>
        <translation>Gyorsbillentyű</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.cpp" line="692"/>
        <source>Shortcut button combination.</source>
        <translation>Gyorsbillentyű kombináció.</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.cpp" line="693"/>
        <source>&lt;b&gt;This is the global shortcut key combination.&lt;/b&gt;&lt;br /&gt;Click this field and then press the desired key/button combo to rebind. Double-click to clear.</source>
        <translation>&lt;b&gt;Ezek a globális gyorsbillentyű kombinációk.&lt;/b&gt;&lt;br /&gt;Kattintson erre a mezőre majd nyomja meg a kívánt billentyű/gomb kombinációt a rögzítéshez. A dupla-kattintással törölheti.</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.cpp" line="697"/>
        <source>Suppress keys from other applications</source>
        <translation>Más alkalmazásoktól jövő billentyű parancsok elnyomása</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.cpp" line="698"/>
        <source>&lt;b&gt;This hides the button presses from other applications.&lt;/b&gt;&lt;br /&gt;Enabling this will hide the button (or the last button of a multi-button combo) from other applications. Note that not all buttons can be suppressed.</source>
        <translation>&lt;b&gt;Ezzel elrejtheti a más alkalmazásoktól jövő gombnyomásokat.&lt;/b&gt;&lt;br /&gt;Engedélyezve ezt, elrejti a más alkalmazásokból jövő gombnyomásokat (vagy az utolsó gombnyomást egy többgombos kombinációból). Megjegyzés: nem minden gombnyomás rejthető el.</translation>
    </message>
</context>
<context>
    <name>GlobalShortcutMacInit</name>
    <message>
        <location filename="GlobalShortcut_macx.cpp" line="60"/>
        <source>Mumble has detected that it is unable to receive Global Shortcut events when it is in the background.&lt;br /&gt;&lt;br /&gt;This is because the Universal Access feature called &apos;Enable access for assistive devices&apos; is currently disabled.&lt;br /&gt;&lt;br /&gt;Please &lt;a href=&quot; &quot;&gt;enable this setting&lt;/a&gt; and continue when done.</source>
        <translation>A Mumble érzékeli, hogy ha a háttérben fut nem képes fogadni a globális gyorsbillentyű eseményeket.&lt;br /&gt;&lt;br /&gt;Ennek az oka, hogy le van tiltva az univerzális hozzáférés: &apos;Engedélyezi a segédeszközökhöz a hozzáférést&apos;&lt;br /&gt;&lt;br /&gt;Kérem a folytatáshoz &lt;a href=&quot; &quot;&gt;engedélyezze ezt a beállítást&lt;/a&gt;.</translation>
    </message>
</context>
<context>
    <name>GlobalShortcutTarget</name>
    <message>
        <location filename="GlobalShortcutTarget.ui" line="14"/>
        <source>Whisper Target</source>
        <translation>Kinek suttog</translation>
    </message>
    <message>
        <location filename="GlobalShortcutTarget.ui" line="20"/>
        <source>Whisper to list of Users</source>
        <translation>Felhasználóknak suttog</translation>
    </message>
    <message>
        <location filename="GlobalShortcutTarget.ui" line="34"/>
        <source>Channel Target</source>
        <translation>Kiválasztott csatorna</translation>
    </message>
    <message>
        <location filename="GlobalShortcutTarget.ui" line="63"/>
        <source>Restrict to Group</source>
        <translation>Csoportra korlátoz</translation>
    </message>
    <message>
        <location filename="GlobalShortcutTarget.ui" line="70"/>
        <source>If specified, only members of this group will receive the whisper.</source>
        <translation>Ha megadva, akkor csak a csoporttagok hallják a suttogást.</translation>
    </message>
    <message>
        <location filename="GlobalShortcutTarget.ui" line="81"/>
        <source>If checked the whisper will also be transmitted to linked channels.</source>
        <translation>Ha kijelölve, akkor a suttogás a bekapcsolt csatornákba is hallható.</translation>
    </message>
    <message>
        <location filename="GlobalShortcutTarget.ui" line="84"/>
        <source>Whisper to Linked channels</source>
        <translation>Suttog a bekapcsolt csatornákba</translation>
    </message>
    <message>
        <location filename="GlobalShortcutTarget.ui" line="91"/>
        <source>If checked this whisper will also be sent to the subchannels of the channel target.</source>
        <translation>Ha ezt kijelöli, akkor a suttogást a kiválasztott csatorna alcsatornáiba is hallják.</translation>
    </message>
    <message>
        <location filename="GlobalShortcutTarget.ui" line="94"/>
        <source>Whisper to subchannels</source>
        <translation>Alcsatornába suttog</translation>
    </message>
    <message>
        <location filename="GlobalShortcutTarget.ui" line="110"/>
        <source>List of users</source>
        <translation>Felhasználók listája</translation>
    </message>
    <message>
        <location filename="GlobalShortcutTarget.ui" line="122"/>
        <source>Add</source>
        <translation>Hozzáadás</translation>
    </message>
    <message>
        <location filename="GlobalShortcutTarget.ui" line="129"/>
        <source>Remove</source>
        <translation>Eltávolítás</translation>
    </message>
    <message>
        <location filename="GlobalShortcutTarget.ui" line="166"/>
        <source>Whisper to Channel</source>
        <translation>Csatornába suttog</translation>
    </message>
    <message>
        <location filename="GlobalShortcutTarget.ui" line="173"/>
        <source>Modifiers</source>
        <translation>Módosítók</translation>
    </message>
    <message>
        <location filename="GlobalShortcutTarget.ui" line="179"/>
        <source>Do not send positional audio information when using this whisper shortcut.</source>
        <translation>Ha ezt a suttogást használja, ne küldje az információkat a pozíciójáról a játékban.</translation>
    </message>
    <message>
        <location filename="GlobalShortcutTarget.ui" line="182"/>
        <source>Ignore positional audio</source>
        <translation>Mellőzze a pozíció információkat</translation>
    </message>
</context>
<context>
    <name>GlobalShortcutX</name>
    <message>
        <location filename="GlobalShortcut_unix.cpp" line="325"/>
        <source>Mouse %1</source>
        <translation>Egér %1</translation>
    </message>
</context>
<context>
    <name>LCD</name>
    <message>
        <location filename="LCD.cpp" line="280"/>
        <source>Not connected</source>
        <translation>Nincs csatlakozva</translation>
    </message>
</context>
<context>
    <name>LCDConfig</name>
    <message>
        <location filename="LCD.cpp" line="120"/>
        <source>Enable this device</source>
        <translation>Engedélyezi ezt az eszközt</translation>
    </message>
    <message>
        <location filename="LCD.cpp" line="129"/>
        <source>LCD</source>
        <translation>LCD</translation>
    </message>
    <message>
        <location filename="LCD.ui" line="14"/>
        <source>Form</source>
        <translation>Űrlap</translation>
    </message>
    <message>
        <location filename="LCD.ui" line="34"/>
        <source>Devices</source>
        <translation>Eszközök</translation>
    </message>
    <message>
        <location filename="LCD.ui" line="50"/>
        <source>Name</source>
        <translation>Név</translation>
    </message>
    <message>
        <location filename="LCD.ui" line="26"/>
        <source>&lt;p&gt;This is the list of available LCD devices on your system.  It lists devices by name, but also includes the size of the display. Mumble supports outputting to several LCD devices at a time.&lt;/p&gt;
&lt;h3&gt;Size:&lt;/h3&gt;
&lt;p&gt;
This field describes the size of an LCD device. The size is given either in pixels (for Graphic LCDs) or in characters (for Character LCDs).&lt;/p&gt;
&lt;h3&gt;Enabled:&lt;/h3&gt;
&lt;p&gt;This decides whether Mumble should draw to a particular LCD device.&lt;/p&gt;</source>
        <translation>&lt;p&gt;Ez az ön rendszerén rendelkezésre álló LCD eszközök listája. A név szerinti lista tartalmazza a kijelzők méretét is. A Mumble támogatja több LCD eszközre való kiírást egyidejűleg.&lt;/p&gt;
&lt;h3&gt;Méret:&lt;/h3&gt;
&lt;p&gt;
Ez a mező mutatja egy LCD eszköz méretét. A méret vagy pixelben (a grafikus LCD-k esetében) vagy karakterben (a karakteres LCD-k esetében) van megadva.&lt;/p&gt;
&lt;h3&gt;Engedélyezve:&lt;/h3&gt;
&lt;p&gt;Ez dönti el, hogy a Mumble rajzolni fog vagy sem az adott eszközre.&lt;/p&gt;</translation>
    </message>
    <message>
        <location filename="LCD.ui" line="55"/>
        <source>Size</source>
        <translation>Méret</translation>
    </message>
    <message>
        <location filename="LCD.ui" line="60"/>
        <source>Enabled</source>
        <translation>Engedélyezve</translation>
    </message>
    <message>
        <location filename="LCD.ui" line="77"/>
        <source>Views</source>
        <translation>Nézet</translation>
    </message>
    <message>
        <location filename="LCD.ui" line="83"/>
        <source>Minimum Column Width</source>
        <translation>Minimális oszlop szélesség</translation>
    </message>
    <message>
        <location filename="LCD.ui" line="90"/>
        <source>&lt;p&gt;This option decides the minimum width a column in the User View.&lt;/p&gt;
&lt;p&gt;If too many people are speaking at once, the User View will split itself into columns. You can use this option to pick a compromise between number of users shown on the LCD, and width of user names.&lt;/p&gt;
</source>
        <translation>&lt;p&gt;Ez a beállítás határozza meg a minimális oszlop szélességet a Felhasználó nézetben.&lt;/p&gt;
&lt;p&gt;Ha túl sok ember beszél egyszerre, akkor a Felhasználó nézet oszlopokra bomlik. Ezzel a beállítással kompromisszumot köthet az LCD kijelzőn egyszerre megjelenő felhasználók száma és felhasználói nevek hossza között.&lt;/p&gt;
</translation>
    </message>
    <message>
        <location filename="LCD.ui" line="128"/>
        <source>This setting decides the width of column splitter.</source>
        <translation>Ezzel a beállítással adja meg az oszlop-elválasztó szélességét.</translation>
    </message>
    <message>
        <location filename="LCD.ui" line="121"/>
        <source>Splitter Width</source>
        <translation>Elosztó szélessége</translation>
    </message>
</context>
<context>
    <name>Log</name>
    <message>
        <location filename="Log.cpp" line="218"/>
        <source>Debug</source>
        <translation>Hibakereső</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="219"/>
        <source>Critical</source>
        <translation>Kritikus</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="220"/>
        <source>Warning</source>
        <translation>Figyelmeztetés</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="221"/>
        <source>Information</source>
        <translation>Információ</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="222"/>
        <source>Server Connected</source>
        <translation>Szerver csatlakozva</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="223"/>
        <source>Server Disconnected</source>
        <translation>Szerver lecsatlakozva</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="224"/>
        <source>User Joined Server</source>
        <translation>Felhasználó csatlakozott a szerverhez</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="225"/>
        <source>User Left Server</source>
        <translation>Felhasználó elhagyta a szervert</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="226"/>
        <source>User kicked (you or by you)</source>
        <translation>Felhasználó kidobva (ön vagy ön által)</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="227"/>
        <source>User kicked</source>
        <translation>Felhasználó kidobva</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="230"/>
        <source>User muted (you)</source>
        <translation>Felhasználó némítva (ön)</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="231"/>
        <source>User muted (by you)</source>
        <translation>Felhasználó némítva (ön által)</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="232"/>
        <source>User muted (other)</source>
        <translation>Felhasználó némítva (más)</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="233"/>
        <source>User Joined Channel</source>
        <translation>Felhasználó csatornához csatlakozott</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="234"/>
        <source>User Left Channel</source>
        <translation>Felhasználó elhagyta a csatornát</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="286"/>
        <source>the server</source>
        <translation>a szerver</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="403"/>
        <source>[[ Text object too large to display ]]</source>
        <translation>[[ Szöveg túl hosszú a kiíráshoz ]]</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="445"/>
        <source>[Date changed to %1]
</source>
        <translation>[Dátum módosítva: %1]
</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="578"/>
        <source>link to %1</source>
        <translation>hivatkozás: %1</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="580"/>
        <source>ftp link to %1</source>
        <translation>ftp hivatkozás: %1</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="582"/>
        <source>player link</source>
        <translation>hivatkozás játékosra</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="584"/>
        <source>channel link</source>
        <translation>hivatkozás csatornára</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="586"/>
        <source>%1 link</source>
        <translation>%1 hivatkozás</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="228"/>
        <source>You self-muted/deafened</source>
        <translation>Önmagát némítva/hangszóró ki</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="229"/>
        <source>Other self-muted/deafened</source>
        <translation>Más (magát) némítva/hangszóró ki</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="235"/>
        <source>Permission Denied</source>
        <translation>Hozzáférés megtagadva</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="236"/>
        <source>Text Message</source>
        <translation>Szöveges üzenet</translation>
    </message>
</context>
<context>
    <name>LogConfig</name>
    <message>
        <location filename="Log.ui" line="14"/>
        <source>Messages</source>
        <translation>Üzenetek</translation>
    </message>
    <message>
        <location filename="Log.ui" line="36"/>
        <source>Message</source>
        <translation>Üzenet</translation>
    </message>
    <message>
        <location filename="Log.ui" line="41"/>
        <source>Console</source>
        <translation>Konzol</translation>
    </message>
    <message>
        <location filename="Log.ui" line="46"/>
        <source>Notification</source>
        <translation>Értesítés</translation>
    </message>
    <message>
        <location filename="Log.ui" line="51"/>
        <source>Text-To-Speech</source>
        <translation>Szövegfelolvasás</translation>
    </message>
    <message>
        <location filename="Log.ui" line="56"/>
        <source>Soundfile</source>
        <translation>Hangfájl</translation>
    </message>
    <message>
        <location filename="Log.ui" line="61"/>
        <source>Path</source>
        <translation>Elérési út</translation>
    </message>
    <message>
        <location filename="Log.ui" line="69"/>
        <source>Text To Speech</source>
        <translation>Szövegfelolvasás</translation>
    </message>
    <message>
        <location filename="Log.ui" line="75"/>
        <source>Volume</source>
        <translation>Hangerő</translation>
    </message>
    <message>
        <location filename="Log.ui" line="85"/>
        <source>Volume of Text-To-Speech Engine</source>
        <translation>Szövegfelolvasás hangereje</translation>
    </message>
    <message>
        <location filename="Log.ui" line="88"/>
        <source>&lt;b&gt;This is the volume used for the speech synthesis.&lt;/b&gt;</source>
        <translation>&lt;b&gt;Ezt a hangerőt használja a beszéd-szintetizátor.&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="Log.ui" line="113"/>
        <source>Length threshold</source>
        <translation>Hossz korlátja</translation>
    </message>
    <message>
        <location filename="Log.ui" line="123"/>
        <source>Message length threshold for Text-To-Speech Engine</source>
        <translation>A szövegfelolvasó által felolvasott üzenetek maximális hossza</translation>
    </message>
    <message>
        <location filename="Log.ui" line="126"/>
        <source>&lt;b&gt;This is the length threshold used for the Text-To-Speech Engine.&lt;/b&gt;&lt;br /&gt;Messages longer than this limit will not be read aloud in their full length.</source>
        <translation>&lt;b&gt;Ez a szövegfelolvasó által használt szöveghossz korlát.&lt;/b&gt;&lt;br /&gt;Ennél a korlátnál hosszabb üzenetet nem fog hangosan felolvasni teljes terjedelmükben.</translation>
    </message>
    <message>
        <location filename="Log.ui" line="132"/>
        <source> Characters</source>
        <translation>.betű</translation>
    </message>
    <message>
        <location filename="Log.ui" line="161"/>
        <source>Whisper</source>
        <translation>Suttogás</translation>
    </message>
    <message>
        <location filename="Log.ui" line="167"/>
        <source>If checked you will only hear whispers from users you added to your friend list.</source>
        <translation>Ha ki van jelölve, akkor hallani fogja a barátainak a suttogását.</translation>
    </message>
    <message>
        <location filename="Log.ui" line="170"/>
        <source>Only accept whispers from friends</source>
        <translation>Csak barát suttogását hallgatja</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="77"/>
        <source>Toggle console for %1 events</source>
        <translation>Ki/bekapcsolja a konzolt a(z) %1 eseményekhez</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="78"/>
        <source>Toggle pop-up notifications for %1 events</source>
        <translation>Ki/bekapcsolja a felugró értesítéseket a(z) %1 eseményekhez</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="79"/>
        <source>Toggle Text-To-Speech for %1 events</source>
        <translation>Ki/bekapcsolja a szövegfelolvasást a(z) %1 eseményekhez</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="80"/>
        <source>Click here to toggle sound notification for %1 events</source>
        <translation>Kattintson ide a hangjelzések ki/bekapcsolásához a(z) %1 eseményekhez</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="81"/>
        <source>Path to sound file used for sound notifications in the case of %1 events&lt;br /&gt;Single click to play&lt;br /&gt;Double-click to change</source>
        <translation>A hangjelzésekhez használt hangfájl elérési útja a(z) %1 eseményekhez&lt;br /&gt;Egy kattintás: lejátszás&lt;br /&gt;Dupla kattintás: módosítás</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="83"/>
        <source>Click here to toggle console output for %1 events.&lt;br /&gt;If checked, this option makes Mumble output all %1 events in its message log.</source>
        <translation>Kattintson ide a konzol ki/bekapcsolásához a(z) %1 eseményekhez.&lt;br /&gt;Ha ki van jelölve, akkor a Mumble kiírja a(z) %1 összes eseményt az üzenetek naplójába.</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="84"/>
        <source>Click here to toggle pop-up notifications for %1 events.&lt;br /&gt;If checked, a notification pop-up will be created by Mumble for every %1 event.</source>
        <translation>Kattintson ide a felugró értesítések ki/bekapcsolásához a(z) %1 eseményekhez.&lt;br /&gt;Ha ki van jelölve, a felugró értesítést a Mumble létre hozza minden %1 eseményhez.</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="85"/>
        <source>Click here to toggle Text-To-Speech for %1 events.&lt;br /&gt;If checked, Mumble uses Text-To-Speech to read %1 events out loud to you. Text-To-Speech is also able to read the contents of the event which is not true for sound files. Text-To-Speech and sound files cannot be used at the same time.</source>
        <translation>Kattintson ide a szövegfelolvasó ki/bekapcsolásához a(z) %1 eseményekhez.&lt;br-/&gt;Ha ki van jelölve, a Mumble felolvassa önnek hangosan a(z) %1 eseményeket. A szövegfelolvasó képes elolvasni az események tartalmát, ami nem igaz a hangfájlok esetében. A szövegfelolvasás és a hangfájlok nem használhatóak egy időben.</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="86"/>
        <source>Click here to toggle sound notification for %1 events.&lt;br /&gt;If checked, Mumble uses a sound file predefined by you to indicate %1 events. Sound files and Text-To-Speech cannot be used at the same time.</source>
        <translation>Kattintson ide a hangjelzések ki/bekapcsolásához a(z) %1 eseményekhez.&lt;br-/&gt;Ha ki van jelölve, a Mumble az előre kijelölt hangfájlt használja a(z) %1 események jelzésére. A hangfájlok és a szövegfelolvasás nem használhatóak egy időben.</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="87"/>
        <source>Path to sound file used for sound notifications in the case of %1 events.&lt;br /&gt;Single click to play&lt;br /&gt;Double-click to change&lt;br /&gt;Ensure that sound notifications for these events are enabled or this field will not have any effect.</source>
        <translation>A hangjelzésekhez használt hangfájl elérési útja a(z) %1 eseményekhez.&lt;br /&gt;Egy kattintás: lejátszás&lt;br /&gt;Dupla kattintás: módosítás&lt;br /&gt;Győződjön meg arról, hogy a hangértesítések engedélyezve vannak ezekhez az eseményekhez, máskülönben ez a beállítás hatástalan lesz.</translation>
    </message>
</context>
<context>
    <name>LookConfig</name>
    <message>
        <location filename="LookConfig.cpp" line="52"/>
        <location filename="LookConfig.cpp" line="63"/>
        <source>System default</source>
        <translation>Rendszer alapértelmezett</translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="67"/>
        <source>None</source>
        <translation>Semmi</translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="68"/>
        <source>Only with users</source>
        <translation>Csak felhasználókkal</translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="69"/>
        <source>All</source>
        <translation>Mind</translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="71"/>
        <source>Ask</source>
        <translation>Kérdez</translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="72"/>
        <source>Do Nothing</source>
        <translation>Nem csinál semmit</translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="73"/>
        <source>Move</source>
        <translation>Mozgat</translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="77"/>
        <source>User Interface</source>
        <translation>Felhasználói felület</translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="204"/>
        <source>Choose skin file</source>
        <translation>Válasszon bőrfájlt</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="204"/>
        <source>Language</source>
        <translation>Nyelv</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="214"/>
        <source>Language to use (requires restart)</source>
        <translation>Használandó nyelv (újraindítást kíván)</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="217"/>
        <source>&lt;b&gt;This sets which language Mumble should use.&lt;/b&gt;&lt;br /&gt;You have to restart Mumble to use the new language.</source>
        <translation>&lt;b&gt;Ezzel kiválasztható, hogy a Mumble melyik nyelvet használja.&lt;/b&gt;&lt;br /&gt;Újra kell indítsa a Mumble-t az új nyelv használatához.</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="157"/>
        <source>Look and Feel</source>
        <translation>Kinézet</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="20"/>
        <source>Layout</source>
        <translation>Elrendezés</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="32"/>
        <source>Classic</source>
        <translation>Klasszikus</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="51"/>
        <source>Stacked</source>
        <translation>Tömött</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="105"/>
        <source>Hybrid</source>
        <translation>Hibrid</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="124"/>
        <source>Custom</source>
        <translation>Egyéni</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="163"/>
        <source>Style</source>
        <translation>Stílus</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="170"/>
        <source>Basic widget style</source>
        <translation>Alap widget stílus</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="173"/>
        <source>&lt;b&gt;This sets the basic look and feel to use.&lt;/b&gt;</source>
        <translation>&lt;b&gt;Ez beállítja a alapvető kinézetet.&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="180"/>
        <source>Skin</source>
        <translation>Bőrök</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="187"/>
        <source>Skin file to use</source>
        <translation>Használandó bőrfájl</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="190"/>
        <source>&lt;b&gt;This sets which skin Mumble should use.&lt;/b&gt;&lt;br /&gt;The skin is a style file applied on top of the basic widget style. If there are icons in the same directory as the style sheet, those will replace the default icons.</source>
        <translation>&lt;b&gt;Ezzel beállíthatja a Mumble által használandó bőrt.&lt;/b&gt;&lt;br /&gt;A bőr egy stílus fájl, ami az alapvető kinézetnek más stílust ad. Ha stíluslappal egy könyvtárban ikonok is vannak, akkor ezek lecserélik az alapértelmezetteket.</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="224"/>
        <source>Apply some high contrast optimizations for visually impaired users</source>
        <translation>A csökkentett látási képességekkel rendelkező felhasználóknak nagy kontrasztra optimizál</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="227"/>
        <source>Optimize for high contrast</source>
        <translation>Nagy kontraszt</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="250"/>
        <source>Application</source>
        <translation>Alkalmazás</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="338"/>
        <source>Displays talking status in system tray</source>
        <translation>Kijelzi a beszélgetés státuszát a tálcán</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="344"/>
        <source>Show talking status in tray icon</source>
        <translation>Beszélgetés státusza a tálcán</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="354"/>
        <source>Channel Tree</source>
        <translation>Csatornák fája</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="367"/>
        <source>This changes the behavior when moving channels.</source>
        <translation>Ezzel módosítja a csatorna mozgatások viselkedését.</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="370"/>
        <source>This sets the behavior of channel drags; it can be used to prevent accidental dragging. &lt;i&gt;Move&lt;/i&gt; moves the channel without prompting. &lt;i&gt;Do Nothing&lt;/i&gt; does nothing and prints an error message. &lt;i&gt;Ask&lt;/i&gt; uses a message box to confirm if you really wanted to move the channel.</source>
        <translation>Beállítja a csatorna húzás viselkedését; megelőzhető vele a véletlen áthelyezés. A &lt;i&gt;Mozgat&lt;/i&gt; módban a csatorna figyelmeztetés nélkül húzható. A &lt;i&gt;Nem csinál semmit&lt;/i&gt; módban nem mozdítható a csatorna és hiba üzenetet ír ki. A &lt;i&gt;Kérdez&lt;/i&gt; módban egy üzenetablakban kérdezi meg, hogy tényleg át szeretné helyezni a csatornát.</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="387"/>
        <source>This sets which channels to automatically expand. &lt;i&gt;None&lt;/i&gt; and &lt;i&gt;All&lt;/i&gt; will expand no or all channels, while &lt;i&gt;Only with users&lt;/i&gt; will expand and collapse channels as users join and leave them.</source>
        <translation>Beállítja, hogy mely csatornákat bontsa ki automatikusan. A &lt;i&gt;Semmi&lt;/i&gt; illetve a &lt;i&gt;Mind&lt;/i&gt; mód nem bont ki vagy mindet kibontja, míg a &lt;i&gt;Csak felhasználókkal&lt;/i&gt; mód kibontja és összecsomagolja a csatornák faszerkezetét annak megfelelően ahogy a felhasználók belépnek és kilépnek a csatornákból.</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="394"/>
        <source>List users above subchannels (requires restart).</source>
        <translation>Mutatja az alcsatornákban levő felhasználókat a csatorna nézetben (újraindítást igényel).</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="397"/>
        <source>&lt;b&gt;If set, users will be shown above subchannels in the channel view.&lt;/b&gt;&lt;br /&gt;A restart of Mumble is required to see the change.</source>
        <translation>&lt;b&gt;Ha ki van jelölve, mutatja az alcsatornákban levő felhasználókat a csatorna nézetben.&lt;/b&gt;&lt;br /&gt;A beállítás a Mumble újraindítása után lép életbe.</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="400"/>
        <source>Users above Channels</source>
        <translation>Felhasználók a csatornák felett</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="407"/>
        <source>Show number of users in each channel</source>
        <translation>Felhasználók számát mutatja minden csatornában</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="410"/>
        <source>Show channel user count</source>
        <translation>Mutatja a felhasználók számát</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="377"/>
        <source>Expand</source>
        <translation>Kibontása</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="384"/>
        <source>When to automatically expand channels</source>
        <translation>Mikor bontsa ki automatikusan a csatornák faszerkezetét</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="360"/>
        <source>Channel Dragging</source>
        <translation>Csatorna húzása</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="303"/>
        <source>Ask whether to close or minimize when quitting Mumble.</source>
        <translation>Amikor kilép a Mumble-ból megkérdezi, hogy bezárja vagy minimalizálj a programot.</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="309"/>
        <source>Ask on quit while connected</source>
        <translation>Kilépés megerősítése</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="256"/>
        <source>Always On Top</source>
        <translation>Mindig előtérben</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="14"/>
        <source>Form</source>
        <translation>Űrlap</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="197"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="263"/>
        <source>This setting controls when the application will be always on top.</source>
        <translation>Beállítja, hogy az alkalmazás ablakát mikor tartsa előtérben.</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="266"/>
        <source>This setting controls in which situations the application will stay always on top. If you select &lt;i&gt;Never&lt;/i&gt; the application will not stay on top. &lt;i&gt;Always&lt;/i&gt; will always keep the application on top. &lt;i&gt;In minimal view&lt;/i&gt; / &lt;i&gt;In normal view&lt;/i&gt; will only keep the application always on top when minimal view is activated / deactivated.</source>
        <translation>Beállítja, hogy az alkalmazás ablakát mikor tartsa előtérben. Ha a &lt;i&gt;Soha&lt;/i&gt; lehetőséget választja, az alkalmazás nem lesz előtérben. A &lt;i&gt;Mindig&lt;/i&gt; lehetőség folyamatosan az előtérben tartja az alkalmazást. A &lt;i&gt;Minimális nézetben&lt;/i&gt; / &lt;i&gt;Normál nézetben&lt;/i&gt; lehetőség az alkalmazást csak akkor tartja előtérben, amikor a minimális nézet aktív / nem aktív.</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="270"/>
        <source>Never</source>
        <translation>Soha</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="275"/>
        <source>Always</source>
        <translation>Mindig</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="280"/>
        <source>In minimal view</source>
        <translation>Minimális nézetben</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="285"/>
        <source>In normal view</source>
        <translation>Normál nézetben</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="293"/>
        <source>Adds user and channel context menus into the menu bar</source>
        <translation>Felhasználó és csatorna helyi menüjeit hozzáadja a főmenühöz</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="296"/>
        <source>Show context menu in menu bar</source>
        <translation>Helyi menük mutatása</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="306"/>
        <source>&lt;b&gt;If set, will verify you want to quit if connected.&lt;/b&gt;</source>
        <translation>&lt;b&gt;Ha be van állítva, akkor leellenőrzi, hogy ha kapcsolódva van, akkor ki akar-e lépni.&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="319"/>
        <source>Tray Icon</source>
        <translation>Tálca ikon</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="328"/>
        <source>&lt;b&gt;If set, minimizing the Mumble main window will cause it to be hidden and accessible only from the tray. Otherwise, it will be minimized as a window normally would.&lt;/b&gt;</source>
        <translation>&lt;b&gt;Ha be van állítva, akkor a Mumble fő ablaka minimalizáláskor eltünik és csak a tálcáról lesz elérhető. Ha nincs beállítva, akkor minimalizáláskor úgy viselkedik, mint egy normális ablak.&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="331"/>
        <source>Hide in tray when minimized</source>
        <translation>Tálcára küldi minimalizáláskor</translation>
    </message>
    <message>
        <location filename="LookConfig.ui" line="325"/>
        <source>Hide the main Mumble window in the tray when it is minimized.</source>
        <translation>Minimalizáláskor elrejti a Mumble főablakát a tálcán.</translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <location filename="MainWindow.cpp" line="148"/>
        <location filename="MainWindow.cpp" line="1979"/>
        <source>Root</source>
        <translation>Gyökér</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="199"/>
        <source>Push-to-Talk</source>
        <comment>Global Shortcut</comment>
        <translation>Beszédhez-nyomd</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="205"/>
        <source>Reset Audio Processor</source>
        <comment>Global Shortcut</comment>
        <translation>Hangfeldolgozás újraindítása</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="218"/>
        <source>Unlink Plugin</source>
        <comment>Global Shortcut</comment>
        <translation>Plugin leválasztása</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="221"/>
        <source>Push-to-Mute</source>
        <comment>Global Shortcut</comment>
        <translation>Némításhoz-nyomd</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="224"/>
        <source>Join Channel</source>
        <comment>Global Shortcut</comment>
        <translation>Csatlakozás a csatornához</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="227"/>
        <source>Toggle Overlay</source>
        <comment>Global Shortcut</comment>
        <translation>Overlay átkapcsolása</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="233"/>
        <source>Toggle Minimal</source>
        <comment>Global Shortcut</comment>
        <translation>Minimálisra vált</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="236"/>
        <source>Volume Up (+10%)</source>
        <comment>Global Shortcut</comment>
        <translation>Hangerő fel (+10%)</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="239"/>
        <source>Volume Down (-10%)</source>
        <comment>Global Shortcut</comment>
        <translation>Hangerő le (-10%)</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="14"/>
        <source>Mumble</source>
        <translation>Mumble</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="243"/>
        <location filename="MainWindow.cpp" line="255"/>
        <location filename="MainWindow.cpp" line="2187"/>
        <source>Mumble -- %1</source>
        <translation>Mumble -- %1</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="259"/>
        <source>&amp;Window</source>
        <translation>&amp;Ablak</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="261"/>
        <location filename="MainWindow.cpp" line="362"/>
        <source>Minimize</source>
        <translation>Minimalizál</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="261"/>
        <source>Ctrl+M</source>
        <translation>Ctrl+M</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="361"/>
        <source>Close</source>
        <translation>Bezárás</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="360"/>
        <source>Mumble is currently connected to a server. Do you want to Close or Minimize it?</source>
        <translation>A Mumble egy szerverrel van kapcsolatban. A programot be szeretné zárni vagy csak minimalizálni?</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="230"/>
        <source>This will switch the states of the in-game overlay between showing everybody, just the users who are talking, and nobody.</source>
        <comment>Global Shortcut</comment>
        <translation>Ezzel a kapcsolóval beállíthatja a játék közben használt kijelző (overlay) állapotát: mindenkit mutat, csak a beszélőket mutatja, senkit sem mutat.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="246"/>
        <source>Whisper</source>
        <translation>Suttog</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="304"/>
        <location filename="MainWindow.cpp" line="2253"/>
        <source>Not connected</source>
        <translation>Nincs csatlakozva</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="505"/>
        <source>Clear</source>
        <translation>Töröl</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="511"/>
        <source>Opening URL %1</source>
        <translation>Megnyitott webcím: %1</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="537"/>
        <source>URL scheme is not &apos;mumble&apos;</source>
        <translation>A webcím protokollja nem &apos;mumble&apos;</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="591"/>
        <source>Connecting to %1</source>
        <translation>Kapcsolódás: %1</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="591"/>
        <source>Enter username</source>
        <translation>Adja meg a felhasználónevet</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="803"/>
        <source>Reconnecting.</source>
        <translation>Újrakapcsolódás.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="951"/>
        <source>Voice channel is sent over control channel.</source>
        <translation>A hangcsatorna a vezérlőcsatornán keresztül küldve.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="960"/>
        <source>UDP Statistics</source>
        <translation>UDP statisztika</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="960"/>
        <source>To Server</source>
        <translation>Szerver felé</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="960"/>
        <source>From Server</source>
        <translation>Szervertől</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="960"/>
        <source>Good</source>
        <translation>Jó</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="960"/>
        <source>Late</source>
        <translation>Késésben</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="960"/>
        <source>Lost</source>
        <translation>Elveszett</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="960"/>
        <source>Resync</source>
        <translation>Újra szinkronizálva</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="966"/>
        <source>Mumble Server Information</source>
        <translation>Mumble szerver információk</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="970"/>
        <location filename="MainWindow.cpp" line="2102"/>
        <source>&amp;View Certificate</source>
        <translation>&amp;Tanúsítvány megtekintése</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1183"/>
        <location filename="MainWindow.cpp" line="1202"/>
        <source>Enter reason</source>
        <translation>Adja meg az okot</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="164"/>
        <source>&amp;User</source>
        <translation>&amp;Felhasználó</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="201"/>
        <source>Push and hold this button to send voice.</source>
        <comment>Global Shortcut</comment>
        <translation>Nyomja le és tartsa lenyomva ezt a gombot a beszéd küldéséhez.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="202"/>
        <source>This configures the push-to-talk button, and as long as you hold this button down, you will transmit voice.</source>
        <comment>Global Shortcut</comment>
        <translation>Ezzel beállítja a Beszédhez-nyomd gombot, ameddig nyomva tartja, hangot fog sugározni.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="208"/>
        <source>Mute Self</source>
        <comment>Global Shortcut</comment>
        <translation>Önmaga némítása</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="210"/>
        <source>Set self-mute status.</source>
        <comment>Global Shortcut</comment>
        <translation>Saját magát lenémítja.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="211"/>
        <source>This will set or toggle your muted status. If you turn this off, you will also disable self-deafen.</source>
        <comment>Global Shortcut</comment>
        <translation>Ezzel beállíthatja vagy átválthatja az ön némítottságát. Ha ezt kikapcsolja, akkor egyúttal letiltja a hangszórók kikapcsolását is.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="213"/>
        <source>Deafen Self</source>
        <comment>Global Shortcut</comment>
        <translation>Saját hangszórók ki</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="215"/>
        <source>Set self-deafen status.</source>
        <comment>Global Shortcut</comment>
        <translation>Kikapcsolja a saját hangszóróit.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="216"/>
        <source>This will set or toggle your deafened status. If you turn this on, you will also enable self-mute.</source>
        <comment>Global Shortcut</comment>
        <translation>Ezzel beállíthatja vagy átválthatja az ön hangszóróinak ki-, illetve bekapcsolt állapotát. Ha ezt bekapcsolja, akkor egyúttal engedélyezi a saját maga némítását is.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="229"/>
        <source>Toggle state of in-game overlay.</source>
        <comment>Global Shortcut</comment>
        <translation>Átváltja a játék közbeni kijelző (overlay) állapotát.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="515"/>
        <source>File does not exist</source>
        <translation>A fájl nem létezik</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="525"/>
        <source>File is not a configuration file.</source>
        <translation>A fájl nem egy beállításokat tartalmazó fájl.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="531"/>
        <source>Settings merged from file.</source>
        <translation>Beállítások összefésülve a fájlból.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="562"/>
        <source>This version of Mumble can&apos;t handle URLs for Mumble version %1.%2.%3</source>
        <translation>A Mumble jelen verziója nem képes kezelni a %1.%2.%3 verziójú Mumble címeket</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="609"/>
        <location filename="MainWindow.cpp" line="793"/>
        <source>Connecting to server %1.</source>
        <translation>%1 szerverhez kapcsolódás.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="834"/>
        <source>Change your comment</source>
        <translation>Módosítja a megjegyzését</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="860"/>
        <location filename="MainWindow.cpp" line="1141"/>
        <source>Register yourself as %1</source>
        <translation>Regisztrálja magát mint %1</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="860"/>
        <location filename="MainWindow.cpp" line="1141"/>
        <source>&lt;p&gt;You are about to register yourself on this server. This action cannot be undone, and your username cannot be changed once this is done. You will forever be known as &apos;%1&apos; on this server.&lt;/p&gt;&lt;p&gt;Are you sure you want to register yourself?&lt;/p&gt;</source>
        <translation>&lt;p&gt;Ön éppen regisztrálja magát ezen a szerveren. Ez a művelet nem vonható vissza, és az ön felhasználó neve többet már nem módosítható. Ezen a szerveren önt ezentúl &apos;%1&apos; néven fogják ismerni.&lt;/p&gt;&lt;p&gt;Biztos abban, hogy regisztrálni akarja magát?&lt;/p&gt;</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="926"/>
        <source>&lt;h2&gt;Version&lt;/h2&gt;&lt;p&gt;Protocol %1.%2.%3.&lt;/p&gt;</source>
        <translation>&lt;h2&gt;Verzió&lt;/h2&gt;&lt;p&gt;Protokoll %1.%2.%3.&lt;/p&gt;</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="931"/>
        <source>&lt;p&gt;No build information or OS version available.&lt;/p&gt;</source>
        <translation>&lt;p&gt;Nincs fordításról vagy operációs rendszer verziójáról információ&lt;/p&gt;</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="933"/>
        <source>&lt;p&gt;%1 (%2)&lt;br /&gt;%3&lt;/p&gt;</source>
        <translation>&lt;p&gt;%1 (%2)&lt;br /&gt;%3&lt;/p&gt;</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="942"/>
        <source>&lt;h2&gt;Control channel&lt;/h2&gt;&lt;p&gt;Encrypted with %1 bit %2&lt;br /&gt;%3 ms average latency (%4 deviation)&lt;/p&gt;&lt;p&gt;Remote host %5 (port %6)&lt;/p&gt;</source>
        <translation>&lt;h2&gt;Vezérlő csatorna&lt;/h2&gt;&lt;p&gt;Titkosítva: %1 bit %2&lt;br /&gt;%3 ms átlagos késleltetés (%4 eltérés)&lt;/p&gt;&lt;p&gt;Távoli gép %5 (portszám %6)&lt;/p&gt;</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="953"/>
        <source>&lt;h2&gt;Voice channel&lt;/h2&gt;&lt;p&gt;Encrypted with 128 bit OCB-AES128&lt;br /&gt;%1 ms average latency (%4 deviation)&lt;/p&gt;</source>
        <translation>&lt;h2&gt;Hangcsatorna&lt;/h2&gt;&lt;p&gt;Titkosítva: 128 bit OCB-AES128&lt;br /&gt;%1 ms átlagos késleltetés (%4 eltérés)&lt;/p&gt;</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="964"/>
        <source>&lt;h2&gt;Audio bandwidth&lt;/h2&gt;&lt;p&gt;Maximum %1 kbit/s&lt;br /&gt;Current %2 kbit/s&lt;/p&gt;</source>
        <translation>&lt;h2&gt;Hang sávszélessége&lt;/h2&gt;&lt;p&gt;Maximum %1 kbit/s&lt;br /&gt;Jelenlegi %2 kbit/s&lt;/p&gt;</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1143"/>
        <source>Register user %1</source>
        <translation>Regisztrálja %1 felhasználót</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1143"/>
        <source>&lt;p&gt;You are about to register %1 on the server. This action cannot be undone, the username cannot be changed, and as a registered user, %1 will have access to the server even if you change the server password.&lt;/p&gt;&lt;p&gt;From this point on, %1 will be authenticated with the certificate currently in use.&lt;/p&gt;&lt;p&gt;Are you sure you want to register %1?&lt;/p&gt;</source>
        <translation>&lt;p&gt;Ön éppen regisztrálja %1 felhasználót ezen a szerveren. Ez a művelet nem vonható vissza, a felhasználónév többet már nem módosítható és mint %1 felhasználó hozzáférése lesz a szerverhez, akkor is ha megváltoztatja a szerver jelszavát.&lt;/p&gt;&lt;p&gt;Ezen a szerveren %1 ezentúl a jelenleg használt tanusítvánnyal fogja magát hitelesíteni.&lt;/p&gt;&lt;p&gt;Biztos abban, hogy regisztrálni akarja %1 felhasználót?&lt;/p&gt;</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1183"/>
        <source>Kicking user %1</source>
        <translation>%1 felhasználó kidobása</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1202"/>
        <source>Banning user %1</source>
        <translation>%1 felhasználó kitiltása</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1220"/>
        <source>Sending message to %1</source>
        <translation>%1 felhasználónak üzenetet küld</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1230"/>
        <location filename="MainWindow.cpp" line="1309"/>
        <location filename="MainWindow.cpp" line="1313"/>
        <source>To %1: %2</source>
        <translation>%1 felhasználónak: %2</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1230"/>
        <location filename="MainWindow.cpp" line="1313"/>
        <source>Message to %1</source>
        <translation>Üzenet %1 felhasználónak</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1253"/>
        <source>View comment on user %1</source>
        <translation>%1 felhasználó megjegyzésének megtekintése</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1269"/>
        <source>Are you sure you want to reset the comment of user %1?</source>
        <translation>Biztos benne, hogy törli a %1 felhasználó megjegyzését?</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1309"/>
        <location filename="MainWindow.cpp" line="1536"/>
        <source>Message to channel %1</source>
        <translation>Üzenet a %1 csatornába</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1534"/>
        <source>(Tree) %1: %2</source>
        <translation>(Fa) %1: %2</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1536"/>
        <source>%1: %2</source>
        <translation>%1: %2</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1972"/>
        <source>Connected.</source>
        <translation>Kapcsolódva.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="2259"/>
        <source>Type message to channel &apos;%1&apos; here</source>
        <translation>Írjon ide, hogy üzenjen a %1 csatornába</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="2262"/>
        <source>Type message to user &apos;%1&apos; here</source>
        <translation>Írjon ide, hogy üzenjen a %1 felhasználónak</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="2353"/>
        <source>Images (*.png *.jpg *.svg)</source>
        <translation>Képek (*.png *.jpg *.svg)</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="2353"/>
        <source>Choose image file</source>
        <translation>Válasszon kép fájlt</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="2360"/>
        <location filename="MainWindow.cpp" line="2377"/>
        <source>Failed to load image</source>
        <translation>Nem sikerült a kép betöltése</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="2360"/>
        <source>Could not open file for reading.</source>
        <translation>Nem tudom olvasásra megnyitni a fájlt.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="2377"/>
        <source>Image format not recognized.</source>
        <translation>A kép formátuma nem felismerhető.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1454"/>
        <source>Are you sure you want to delete %1 and all its sub-channels?</source>
        <translation>Biztos abban, hogy törli %1 csatornát és az összes alcsatornáját?</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1525"/>
        <source>Sending message to channel %1</source>
        <translation>Üzenet küldése %1 csatornába</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1534"/>
        <source>Message to tree %1</source>
        <translation>Üzenet a %1 fába</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1645"/>
        <source>Unmuted and undeafened.</source>
        <translation>Mikrofon és hangszóró bekapcsolva.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1647"/>
        <source>Unmuted.</source>
        <translation>Hangosítva.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1649"/>
        <source>Muted.</source>
        <translation>Némítva.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1673"/>
        <source>Muted and deafened.</source>
        <translation>Mikrofon és hangszóró kikapcsolva.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1675"/>
        <source>Deafened.</source>
        <translation>Hangszóró kikapcsolva.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1677"/>
        <source>Undeafened.</source>
        <translation>Hangszóró bekapcsolva.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="1744"/>
        <source>About Qt</source>
        <translation>Qt névjegye</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="2081"/>
        <source>SSL Verification failed: %1</source>
        <translation>SSL ellenőrzés hibát jelzett: %1</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="2086"/>
        <source>&lt;b&gt;WARNING:&lt;/b&gt; The server presented a certificate that was different from the stored one.</source>
        <translation>&lt;b&gt;FIGYELMEZTETÉS:&lt;/b&gt; A szerver az elmentett tanúsítványtól eltérőt mutatott.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="2088"/>
        <source>Sever presented a certificate which failed verification.</source>
        <translation>A szerver egy olyan tanúsítványt mutatott, amely ellenőrzéskor hibásnak bizonyult.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="2095"/>
        <source>&lt;p&gt;%1.&lt;br /&gt;The specific errors with this certificate are: &lt;/p&gt;&lt;ol&gt;%2&lt;/ol&gt;&lt;p&gt;Do you wish to accept this certificate anyway?&lt;br /&gt;(It will also be stored so you won&apos;t be asked this again.)&lt;/p&gt;</source>
        <translation>&lt;p&gt;%1.&lt;br /&gt;Ennek a tanúsítványnak a hibái: &lt;/p&gt;&lt;ol&gt;%2&lt;/ol&gt;&lt;p&gt;Mindenképpen elszeretné fogadni a tanúsítványt?&lt;br /&gt;(Mivel mentésre kerül, legközelebb ezt már nem fogom megkérdezni.)&lt;/p&gt;</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="2119"/>
        <source>SSL Version mismatch</source>
        <translation>SSL verziója nem egyezik</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="2119"/>
        <source>This server is using an older encryption standard. It might be an older 1.1 based Mumble server.&lt;br /&gt;Would you like to launch the compatibility client to connect to it?</source>
        <translation>Ez a szerver egy régebbi titkosítási eljárást használ, lehet, hogy egy régebbi, 1.1 verzióra épülő Mumble szerver.&lt;br /&gt;Szeretné elindítani a kompatibilis kliens a csatlakozáshoz?</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="2145"/>
        <source>Failed to launch compatibility client</source>
        <translation>Nem sikerült a kompatibilis kliens indítása</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="2145"/>
        <source>The compatibility client could not be found, or failed to start.&lt;br /&gt;Note that the compatibility client is an optional component for most installations, and might not be installed.</source>
        <translation>A kompatibilis kliens nem található, vagy nem sikerült elindítani.&lt;br /&gt;Megjegyzendő, hogy a kompatibilis kliens a legtöbb telepítőnek nem kötelezően települő része és lehet, hogy nincs is telepítve.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="2152"/>
        <source>Server connection failed: %1.</source>
        <translation>Nem sikerült kapcsolódni a szerverhez: %1.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="2154"/>
        <source>Disconnected from server.</source>
        <translation>Szerverről lekapcsolva.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="2166"/>
        <source>Invalid username</source>
        <translation>Érvénytelen felhasználónév</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="2166"/>
        <source>You connected with an invalid username, please try another one.</source>
        <translation>Érvénytelen felhasználónévvel csatlakozott, kérem próbáljon meg egy másikat.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="2166"/>
        <source>That username is already in use, please try another username.</source>
        <translation>Ez a felhasználónév már használatban van, kérem próbáljon egy másikat.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="2171"/>
        <source>Wrong password</source>
        <translation>Hibás jelszó</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="2171"/>
        <source>Wrong password for registered users, please try again.</source>
        <translation>A regisztrált felhasználói jelszó hibás, próbálja újra.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="2171"/>
        <source>Wrong server password for unregistered user account, please try again.</source>
        <translation>A nem regisztrált felhasználói jelszó hibás, próbálja újra.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="165"/>
        <source>&amp;Channel</source>
        <translation>&amp;Csatorna</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="59"/>
        <source>C&amp;onfigure</source>
        <translation>&amp;Beállítások</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="65"/>
        <source>&amp;Help</source>
        <translation>&amp;Súgó</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="77"/>
        <source>S&amp;erver</source>
        <translation>Sz&amp;erver</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="82"/>
        <source>&amp;Self</source>
        <translation>S&amp;aját</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="104"/>
        <source>Log</source>
        <translation>Napló</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="114"/>
        <source>This shows all recent activity. Connecting to servers, errors and information messages all show up here.&lt;br /&gt;To configure exactly which messages show up here, use the &lt;b&gt;Settings&lt;/b&gt; command from the menu.</source>
        <translation>Mutatja a nemrég végrehajtott műveleteket. Itt láthatóak a szerverekhez való kapcsolódások, hiba és egyéb információkat tartalmazó üzenetek.&lt;br./&gt;A &lt;b&gt;Beállítások&lt;b/&gt; menü parancs használatával pontosan testreszabható, hogy mi jelenjen meg ebben az ablakban.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="132"/>
        <source>This is the chatbar&lt;br /&gt;If you enter text here and then press enter the text is sent to the user or channel that was selected. If nothing is selected the message is sent to your current channel.</source>
        <translation>Ez a csevegőablak&lt;br /&gt;Ha ide beír egy szöveget és enter-t nyom, a szöveget a kijelölt felhasználó vagy csatorna kapja meg. Ha semmi sincs kiválasztva, akkor az ön jelenlegi csatornája kapja a szöveget.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="138"/>
        <source>Chatbar</source>
        <translation>Csevegőablak</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="160"/>
        <source>Icon Toolbar</source>
        <translation>Eszköztár</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="180"/>
        <source>&amp;Quit Mumble</source>
        <translation>&amp;Kilépés</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="183"/>
        <source>Closes the program</source>
        <translation>Leállítja a programot</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="186"/>
        <source>Exits the application.</source>
        <translation>Kilép az alkalmazásból.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="189"/>
        <source>Ctrl+Q</source>
        <translation>Ctrl+K</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="198"/>
        <source>&amp;Connect</source>
        <translation>Ka&amp;pcsolódás</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="201"/>
        <source>Open the server connection dialog</source>
        <translation>Megnyitja a kapcsolódás a szerverhez párbeszédablakot</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="215"/>
        <source>&amp;Disconnect</source>
        <translation>Szé&amp;tkapcsolás</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="218"/>
        <source>Disconnect from server</source>
        <translation>Bontja a kapcsolatot a szerverrel</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="221"/>
        <source>Disconnects you from the server.</source>
        <translation>Bontja az ön kapcsolatát a szerverrel.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="229"/>
        <source>&amp;Ban lists</source>
        <translation>&amp;Tiltó lista</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="232"/>
        <source>Edit ban lists on server</source>
        <translation>A szerveren levő tiltó lista szerkesztése</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="235"/>
        <source>This lets you edit the server-side IP ban lists.</source>
        <translation>Ezzel ön szerkesztheti a szerver oldali IP tiltó listát.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="247"/>
        <location filename="MainWindow.ui" line="741"/>
        <source>&amp;Information</source>
        <translation>&amp;Információ</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="250"/>
        <source>Show information about the server connection</source>
        <translation>A szerverrel való kapcsolat információit mutatja</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="253"/>
        <source>This will show extended information about the connection to the server.</source>
        <translation>Megmutatja a szerverrel való kapcsolat részletes információit.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="261"/>
        <source>&amp;Kick</source>
        <translation>&amp;Kidob</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="275"/>
        <source>&amp;Mute</source>
        <translation>&amp;Némít</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="286"/>
        <source>&amp;Ban</source>
        <translation>&amp;Tilt</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="300"/>
        <source>&amp;Deafen</source>
        <translation>&amp;Hangszóró ki</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="314"/>
        <source>&amp;Local Mute</source>
        <translation>&amp;Helyi némítás</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="328"/>
        <location filename="MainWindow.ui" line="594"/>
        <source>Send a Text Message</source>
        <translation>Szöveges üzenetet küld</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="331"/>
        <source>Sends a text message to another user.</source>
        <translation>Szöveges üzenetet küld egy másik felhasználónak.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="336"/>
        <source>&amp;Add</source>
        <translation>&amp;Hozzáadás</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="339"/>
        <source>Add new channel</source>
        <translation>Új csatorna hozzáadása</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="342"/>
        <source>This adds a new sub-channel to the currently selected channel.</source>
        <translation>Ezzel létrehoz egy új alcsatornát a jelenleg kiválasztott csatornában.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="347"/>
        <source>&amp;Remove</source>
        <translation>&amp;Eltávolítás</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="350"/>
        <source>Remove channel</source>
        <translation>Csatorna eltávolítása</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="353"/>
        <source>This removes a channel and all sub-channels.</source>
        <translation>Ezzel eltávolít egy csatornát és minden alcsatornáját.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="361"/>
        <source>Edit Groups and ACL for channel</source>
        <translation>Szerkeszti egy csatorna csoportjait és a hozzáférést szabályzó listáját (ACL)</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="364"/>
        <source>This opens the Group and ACL dialog for the channel, to control permissions.</source>
        <translation>Ezzel megnyitja a csatorna csoport és hozzáférést szabályozó lista (ACL) párbeszédablakát a jogosultságok beállításához.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="369"/>
        <source>&amp;Link</source>
        <translation>&amp;Csatolás</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="372"/>
        <source>Link your channel to another channel</source>
        <translation>Csatolja az ön csatornáját egy másikhoz</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="380"/>
        <source>&amp;Unlink</source>
        <comment>Channel</comment>
        <translation>&amp;Leválasztás</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="383"/>
        <source>Unlink your channel from another channel</source>
        <translation>Leválasztja az ön csatornáját egy másiktól</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="386"/>
        <source>This unlinks your current channel from the selected channel.</source>
        <translation>Ezzel leválasztja az ön jelenlegi csatornáját a kijelölt csatornától.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="394"/>
        <source>Unlinks your channel from all linked channels.</source>
        <translation>Leválasztja az ön csatornáját minden csatolt csatornától.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="397"/>
        <source>This unlinks your current channel (not the selected one) from all linked channels.</source>
        <translation>Ezzel leválasztja az ön jelenlegi csatornáját (nem a kijelöltektől) az összes csatolt csatornától.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="402"/>
        <source>&amp;Reset</source>
        <translation>&amp;Lenullázás</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="405"/>
        <source>Reset audio preprocessor</source>
        <translation>Alaphelyzetbe hozza a hangelőfeldolgozót</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="408"/>
        <source>This will reset the audio preprocessor, including noise cancellation, automatic gain and voice activity detection. If something suddenly worsens the audio environment (like dropping the microphone) and it was temporary, use this to avoid having to wait for the preprocessor to readjust.</source>
        <translation>Ezzel lenullázza a hangelőfeldolgozó egységet, beleértve a zajszűrőt. az automatikus hangerőszabályzót és beszéd érzékelést. Ha valami hirtelen elrontja a hangot (például a mikrofon elejtése) és ez csak ideiglenes, akkor használja ezt a lehetőséget, hogy elkerülhesse az előfeldolgozó visszaállására való várakozást.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="422"/>
        <source>&amp;Mute Self</source>
        <translation>&amp;Saját maga némítása</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="425"/>
        <source>Mute yourself</source>
        <translation>Lenémítja saját magát</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="428"/>
        <source>Mute or unmute yourself. When muted, you will not send any data to the server. Unmuting while deafened will also undeafen.</source>
        <translation>Némítja vagy hangosítja saját magát. Ha némítva van, akkor nem fog küldeni semmilyen adatot a szervernek. A hangosítás a hangszórókat is bekapcsolja, ha ki voltak kapcsolva.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="445"/>
        <source>&amp;Deafen Self</source>
        <translation>&amp;Hangszóró ki</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="448"/>
        <source>Deafen yourself</source>
        <translation>Saját hangszórókat kikapcsolja</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="451"/>
        <source>Deafen or undeafen yourself. When deafened, you will not hear anything. Deafening yourself will also mute.</source>
        <translation>Saját hangszóróit ki- vagy bekapcsolja. Ha a hangszórók ki vannak kapcsolva, nem fog semmit hallani. Ha kikapcsolja a hangszóróit, egyúttal némítja is magát.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="462"/>
        <source>&amp;Text-To-Speech</source>
        <translation>&amp;Felolvasás</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="465"/>
        <source>Toggle Text-To-Speech</source>
        <translation>Ki-, bekapcsolja a felolvasást</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="468"/>
        <source>Enable or disable the text-to-speech engine. Only messages enabled for TTS in the Configuration dialog will actually be spoken.</source>
        <translation>Engedélyezi vagy tiltja a szöveget felolvasó motort. A beállítások párbeszédablakban felolvasásra (TTS) engedélyezett üzeneteket fogja felolvasni.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="476"/>
        <source>Display audio statistics</source>
        <translation>Hang statisztika megjelenítése</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="479"/>
        <source>Pops up a small dialog with information about your current audio input.</source>
        <translation>Felugraszt egy kis párbeszédablakot a hangbemenet aktuális adataival.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="204"/>
        <source>Shows a dialog of registered servers, and also allows quick-connect.</source>
        <translation>A regisztrált szerverekkel mutat egy párbeszédablakot és a gyors kapcsolódást is lehetővé teszi.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="264"/>
        <source>Kick user (with reason)</source>
        <translation>Felhasználó kidobása (okkal)</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="267"/>
        <source>Kick selected user off server. You&apos;ll be asked to specify a reason.</source>
        <translation>Kidobja a kiválasztott felhasználót a szerverről. Meg kell adnia egy okot.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="278"/>
        <source>Mute user</source>
        <translation>Felhasználó némítása</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="281"/>
        <source>Mute or unmute user on server. Unmuting a deafened user will also undeafen them.</source>
        <translation>Némítja vagy hangosítja a felhasználót a szerveren. A hangosítás a hangszórókat is bekapcsolja, ha ki voltak kapcsolva.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="289"/>
        <source>Kick and ban user (with reason)</source>
        <translation>Felhasználó kidobása és kitiltása (okkal)</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="292"/>
        <source>Kick and ban selected user from server. You&apos;ll be asked to specify a reason.</source>
        <translation>Kidobja és kitiltja a kiválasztott felhasználót a szerverről. Meg kell adnia egy okot.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="303"/>
        <source>Deafen user</source>
        <translation>Felhasználó hangszórói ki</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="306"/>
        <source>Deafen or undeafen user on server. Deafening a user will also mute them.</source>
        <translation>Ki- vagy bekapcsolja a szerveren levő felhasználó hangszóróit. A hangszórók kikapcsolása némítja is a felhasználót.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="317"/>
        <source>Mute user locally</source>
        <translation>Felhasználó helyi némítása</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="320"/>
        <source>Mute or unmute user locally. Use this on other users in the same room.</source>
        <translation>Felhasználó helyi némítása vagy hangosítása. Használja ezt azokon, akikkel egy szobában van.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="325"/>
        <location filename="MainWindow.ui" line="591"/>
        <source>Send &amp;Message</source>
        <translation>Ü&amp;zenet küldése</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="358"/>
        <source>&amp;Edit</source>
        <translation>&amp;Szerkesztés</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="375"/>
        <source>This links your current channel to the selected channel. If users in a channel have permission to speak in the other channel, users can now hear each other. This is a permanent link, and will last until manually unlinked or the server is restarted. Please see the shortcuts for push-to-link.</source>
        <translation>Ezzel összeköti az ön jelenlegi csatornáját a kiválasztottal. Ha a felhasználóknak joguk van más csatornákba való beszédhez, akkor hallhatják egymást. Ez egy állandó kötés és addig fog tartani, amíg kézileg szét nem bontja valaki vagy a szervert újra nem indítja. Kérem nézze meg a gyorsbillentyűket a csatoláshoz-nyomd használatához.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="391"/>
        <source>&amp;Unlink All</source>
        <translation>&amp;Minden leválasztása</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="473"/>
        <source>Audio S&amp;tatistics</source>
        <translation>Hang &amp;statisztika</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="484"/>
        <source>&amp;Unlink Plugins</source>
        <translation>Pluginok &amp;bontása</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="487"/>
        <source>Forcibly unlink plugin</source>
        <translation>Pluginok kikényszerített bontása</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="490"/>
        <source>This forces the current plugin to unlink, which is handy if it is reading completely wrong data.</source>
        <translation>Ezzel kikényszerítheti a jelenlegi plugin leválasztását, amely például akkor hasznos, ha teljesen rossz adatot olvas.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="499"/>
        <source>&amp;Settings</source>
        <translation>&amp;Beállítások</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="502"/>
        <source>Configure Mumble</source>
        <translation>Beállítja a Mumble-t</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="505"/>
        <source>Allows you to change most settings for Mumble.</source>
        <translation>Lehetővé teszi a Mumble legtöbb beállításának módosítását.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="516"/>
        <source>&amp;Audio Wizard</source>
        <translation>&amp;Hangvarázsló</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="519"/>
        <source>Start the audio configuration wizard</source>
        <translation>Elindítja a hangbeállító varázslót</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="522"/>
        <source>This will guide you through the process of configuring your audio hardware.</source>
        <translation>Végig vezet a hangeszközök beállításának folyamatán.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="527"/>
        <source>&amp;What&apos;s This?</source>
        <translation>&amp;Mi ez?</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="530"/>
        <source>Enter What&apos;s This? mode</source>
        <translation>Mi ez? módba lép</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="533"/>
        <source>Click this to enter &quot;What&apos;s This?&quot; mode. Your cursor will turn into a question mark. Click on any button, menu choice or area to show a description of what it is.</source>
        <translation>Kattints erre, hogy Mi ez? módba lépj. A kurzor kérdőjellé fog válni. Kattintson bármely gombra vagy menüpontra vagy egyéb területre a leírásért.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="538"/>
        <source>&amp;About</source>
        <translation>&amp;Névjegy</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="541"/>
        <source>Information about Mumble</source>
        <translation>Információk a Mumble-ról</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="544"/>
        <source>Shows a small dialog with information and license for Mumble.</source>
        <translation>Egy kis ablakban mutatja a Mumble-ról szóló információkat és a licencét.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="552"/>
        <source>About &amp;Speex</source>
        <translation>&amp;Speex névjegye</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="555"/>
        <source>Information about Speex</source>
        <translation>Információk a Speex-ről</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="558"/>
        <source>Shows a small dialog with information about Speex.</source>
        <translation>Egy kis ablakban tájékoztat a Speex-ről.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="566"/>
        <source>About &amp;Qt</source>
        <translation>&amp;Qt névjegye</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="569"/>
        <source>Information about Qt</source>
        <translation>Információk a Qt-ról</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="572"/>
        <source>Shows a small dialog with information about Qt.</source>
        <translation>Kis ablakban tájékoztat a Qt-ról.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="580"/>
        <source>Check for &amp;Updates</source>
        <translation>&amp;Frissítéseket keres</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="583"/>
        <source>Check for new version of Mumble</source>
        <translation>A Mumble újabb verzióját keresi</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="586"/>
        <source>Connects to the Mumble webpage to check if a new version is available, and notifies you with an appropriate download URL if this is the case.</source>
        <translation>Kapcsolódik a Mumble honlapjához, hogy ellenőrizze az újabb verzió elérhetőségét és ha kell, értesíti önt a megfelelő letöltési címmel (URL).</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="597"/>
        <source>Sends a text message to all users in a channel.</source>
        <translation>Szöveges üzenetet küld a csatornában levő összes felhasználónak.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="630"/>
        <source>&amp;Certificate Wizard</source>
        <translation>&amp;Tanúsítvány varázsló</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="633"/>
        <source>Configure certificates for strong authentication</source>
        <translation>Beállítja a tanúsítványok titkosítását erősebbre</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="636"/>
        <source>This starts the wizard for creating, importing and exporting certificates for authentication against servers.</source>
        <translation>Elindítja a szervereken hitelesítésre használt tanúsítványok létrehozását, importálását és exportálását végző varázslót.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="641"/>
        <source>&amp;Register</source>
        <translation>&amp;Regisztrál</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="644"/>
        <source>Register user on server</source>
        <translation>Regisztrálja a felhasználót a szerveren</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="647"/>
        <source>This will permanently register the user on the server.</source>
        <translation>Véglegesen regisztrálja a felhasználót a szerveren.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="652"/>
        <source>Add &amp;Friend</source>
        <translation>&amp;Barát hozzáadása</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="655"/>
        <source>Adds a user as your friend.</source>
        <translation>Felhasználót barátnak jegyzi meg.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="658"/>
        <source>This will add the user as a friend, so you can recognize him on this and other servers.</source>
        <translation>Ezzel a felhasználó barátként lesz megjegyezve, ezáltal ezen és más szervereken is fel fogja ismerve.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="663"/>
        <source>&amp;Remove Friend</source>
        <translation>Barát &amp;eltávolítása</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="666"/>
        <source>Removes a user from your friends.</source>
        <translation>Eltávolítja a felhasználót a barátok közül.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="669"/>
        <source>This will remove a user from your friends list.</source>
        <translation>Eltávolítja a felhasználót a barátok listájából.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="674"/>
        <source>&amp;Update Friend</source>
        <translation>Barátok &amp;frissítése</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="677"/>
        <source>Update name of your friend.</source>
        <translation>Barát nevének frissítése.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="680"/>
        <source>Your friend uses a different name than what is in your database. This will update the name.</source>
        <translation>Az ön barátja egy másik nevet használ, mint ami az ön adatbázisában van. Ezzel frissíteni fogja a nevet.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="685"/>
        <source>Registered &amp;Users</source>
        <translation>&amp;Regisztrált felhasználók</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="688"/>
        <source>Edit registered users list</source>
        <translation>Szerkeszti a regisztrált felhasználók listáját</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="691"/>
        <source>This opens the editor for registered users, which allow you to change their name or unregister them.</source>
        <translation>Megnyitja a regisztrált felhasználók szerkesztőjét, amivel módosíthatja a nevüket vagy törölheti regisztrációjukat.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="696"/>
        <source>Change &amp;Avatar</source>
        <translation>&amp;Avatár módosítása</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="699"/>
        <source>Change your avatar image on this server</source>
        <translation>Az ön avatár képét módosítja ezen a szerveren</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="704"/>
        <source>&amp;Access Tokens</source>
        <translation>&amp;Elérési tokenek</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="707"/>
        <source>Add or remove text-based access tokens</source>
        <translation>Hozzáad vagy eltávolít szöveg alapú hozzáférési tokeneket</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="712"/>
        <source>&amp;Remove Avatar</source>
        <translation>Avatár &amp;eltávolítása</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="715"/>
        <source>Remove currently defined avatar image.</source>
        <translation>Eltávolítja a jelenleg beállított avatár képet.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="720"/>
        <source>Reset &amp;Comment</source>
        <translation>&amp;Megjegyzés törlése</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="723"/>
        <source>Reset the comment of the selected user.</source>
        <translation>A kiválasztott felhasználó megjegyzését törli.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="728"/>
        <source>&amp;Join Channel</source>
        <translation>&amp;Csatlakozás csatornához</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="733"/>
        <source>View Comment</source>
        <translation>Megjegyzés megtekintése</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="736"/>
        <source>View comment in editor</source>
        <translation>Megjegyzés megtekintése a szerkesztőben</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="744"/>
        <source>Query server for connection information for user</source>
        <translation>Lekérdezi a szerverről a felhasználókapcsolatának adatait</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="753"/>
        <source>&amp;Change Comment</source>
        <translation>&amp;Megjegyzés módosítása</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="756"/>
        <source>Change your own comment.</source>
        <translation>A saját megjegyzését módosítja.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="764"/>
        <source>R&amp;egister</source>
        <translation>R&amp;egisztrálás</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="767"/>
        <source>Register yourself on the server</source>
        <translation>Regisztrálja saját magát a szerveren</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="605"/>
        <source>&amp;Minimal View</source>
        <translation>&amp;Minimális nézet</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="608"/>
        <source>Toggle minimal window modes</source>
        <translation>Váltja az ablak minimális nézet módját</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="611"/>
        <source>This will toggle minimal mode, where the log window and menu is hidden.</source>
        <translation>Ezzel válthatja a minimális nézet módot, amelyben a napló ablaka és a menü nem látszik.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="266"/>
        <source>%1 is now muted and deafened.</source>
        <translation>%1 most némítva és hangszórói kikapcsolva.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="268"/>
        <source>%1 is now muted.</source>
        <translation>%1 most némítva.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="270"/>
        <source>%1 is now unmuted.</source>
        <translation>%1 most hangosítva.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="291"/>
        <source>You were muted by %1.</source>
        <translation>Önt %1 lenémította.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="293"/>
        <source>You were unmuted by %1.</source>
        <translation>Önt %1 hangosította.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="349"/>
        <source>%1 muted by %2.</source>
        <translation>%2 elnémította %1-t.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="351"/>
        <source>%1 unmuted by %2.</source>
        <translation>%2 hangosította %1-t.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="298"/>
        <source>You were undeafened by %1.</source>
        <translation>Hangszóróit bekapcsolta %1.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="356"/>
        <source>%1 undeafened by %2.</source>
        <translation>%1 hangszóróit bekapcsolta %2.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="439"/>
        <source>You were kicked from the server by %1: %2.</source>
        <translation>%1 kidobta önt a szerverről: %2.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="444"/>
        <source>%3 was kicked from the server by %1: %2.</source>
        <translation>%1 kidobta %3-t a szerverről: %2.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="437"/>
        <source>You were kicked and banned from the server by %1: %2.</source>
        <translation>%1 kidobta és kitiltotta önt a szerverről: %2.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="96"/>
        <location filename="Messages.cpp" line="136"/>
        <source>Welcome message: %1</source>
        <translation>Üdvözlő üzenet: %1</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="129"/>
        <source>Mumble: %1</source>
        <translation>Mumble: %1</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="157"/>
        <source>You were denied %1 privileges in %2.</source>
        <translation>Az ön %1 joga a %2 csatornában letiltva.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="159"/>
        <source>%3 was denied %1 privileges in %2.</source>
        <translation>%3 %1 joga a %2 csatornában letiltva.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="163"/>
        <source>Denied: Cannot modify SuperUser.</source>
        <translation>Megtagadva: a SuperUser nem módosítható.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="167"/>
        <source>Denied: Invalid channel name.</source>
        <translation>Megtagadva: érvénytelen csatornanév.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="171"/>
        <source>Denied: Text message too long.</source>
        <translation>Megtagadva: Túl hosszú szöveg.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="191"/>
        <source>Denied: Operation not permitted in temporary channel.</source>
        <translation>Megtagadva: Az ideiglenes csatornában nem engedélyezett a művelet.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="198"/>
        <source>You need a certificate to perform this operation.</source>
        <translation>Ennek a műveletnek a végrehajtásához egy tanúsítványra van szüksége.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="200"/>
        <source>%1 does not have a certificate.</source>
        <translation>%1 felhasználónak nincs tanúsítványa.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="205"/>
        <source>Invalid username: %1.</source>
        <translation>Érvénytelen felhasználónév: %1.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="207"/>
        <source>Invalid username.</source>
        <translation>Érvénytelen felhasználónév.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="211"/>
        <source>Channel is full.</source>
        <translation>Csatorna tele van.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="218"/>
        <source>Permission denied.</source>
        <translation>Engedély megtagadva.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="255"/>
        <source>%1 connected.</source>
        <translation>%1 kapcsolódott.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="285"/>
        <source>You were muted and deafened by %1.</source>
        <translation>%1 kikapcsolta az ön mikrofonját és hangszóróit.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="287"/>
        <source>You were unmuted and undeafened by %1.</source>
        <translation>%1 bekapcsolta az ön mikrofonját és hangszóróit.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="304"/>
        <source>You were suppressed.</source>
        <translation>Az ön hangját elnyomták.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="307"/>
        <source>You were unsuppressed.</source>
        <translation>Az ön hangjának elnyomása megszűnt.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="309"/>
        <source>You were unsuppressed by %1.</source>
        <translation>%1 megszüntette az ön hangjának elnyomását.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="316"/>
        <source>You muted and deafened %1.</source>
        <translation>Ön kikapcsolta %1 mikrofonját és hangszóróit.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="318"/>
        <source>You unmuted and undeafened %1.</source>
        <translation>Ön bekapcsolta %1 mikrofonját és hangszóróit.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="322"/>
        <source>You muted %1.</source>
        <translation>Ön némította %1 felhasználót.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="324"/>
        <source>You unmuted %1.</source>
        <translation>Ön hangosította %1 felhasználót.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="329"/>
        <source>You undeafened %1.</source>
        <translation>Ön bekapcsolta %1 hangszóróit.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="336"/>
        <source>You suppressed %1.</source>
        <translation>Ön elnyomta %1 hangját.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="338"/>
        <source>You unsuppressed %1.</source>
        <translation>Ön megszüntette %1 hangjának elnyomását.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="343"/>
        <source>%1 muted and deafened by %2.</source>
        <translation>%1 mikrofonját és hangszóróit kikapcsolta %2.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="345"/>
        <source>%1 unmuted and undeafened by %2.</source>
        <translation>%1 mikrofonját és hangszóróit bekapcsolta %2.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="363"/>
        <source>%1 suppressed by %2.</source>
        <translation>%1 hangját %2 elnyomta.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="365"/>
        <source>%1 unsuppressed by %2.</source>
        <translation>%1 hangjának elnyomását %2 megszüntette.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="385"/>
        <source>You were moved to %1 by %2.</source>
        <translation>%2 önt áthelyezte a %1 csatornára.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="388"/>
        <source>%1 moved to %2.</source>
        <translation>%1 felhasználó át lett helyezve a %2 csatornára.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="390"/>
        <source>%1 moved to %2 by %3.</source>
        <translation>%1 felhasználót %3 áthelyezte a %2 csatornára.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="400"/>
        <source>%1 moved in from %2 by %3.</source>
        <translation>%1 felhasználót %3 ide helyezte a %2 csatornáról.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="442"/>
        <source>%3 was kicked and banned from the server by %1: %2.</source>
        <translation>%1 kidobta és kitiltotta a szerverről %3 felhasználót: %2.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="446"/>
        <source>%1 disconnected.</source>
        <translation>%1 lekapcsolódott.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="537"/>
        <source>(Tree) </source>
        <translation>(Fa) </translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="539"/>
        <source>(Channel) </source>
        <translation>(Csatorna) </translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="542"/>
        <source>%2%1: %3</source>
        <translation>%2%1: %3</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="678"/>
        <source>Unable to find matching CELT codecs with other clients. You will not be able to talk to all users.</source>
        <translation>Nem sikerült a többi kliens CELT kódolójának megfelelő kódolót találni. Ön nem lesz képes minden felhasználóval beszélni.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="398"/>
        <source>%1 entered channel.</source>
        <translation>%1 csatlakozott a csatornához.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="88"/>
        <source>Server connection rejected: %1.</source>
        <translation>A kapcsolódást a szerver visszautasította: %1.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="216"/>
        <source>Denied: %1.</source>
        <translation>Megtagadva: %1.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="543"/>
        <source>Message from %1</source>
        <translation>Üzenetet küldött %1</translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="1382"/>
        <source>You have Channel Dragging set to &quot;Do Nothing&quot; so the channel wasn&apos;t moved.</source>
        <translation>Az ön Csatorna húzása &quot;Ne tegyen semmit&quot;-re van állítva, így a csatorna nem lett mozgatva.</translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="1388"/>
        <source>Unknown Channel Drag mode in UserModel::dropMimeData.</source>
        <translation>Ismeretlen Csatorna húzási mód a UserModel::dropMimeData beállításokban.</translation>
    </message>
    <message>
        <location filename="main.cpp" line="338"/>
        <source>Welcome to Mumble.</source>
        <translation>Üdvözli a Mumble.</translation>
    </message>
    <message>
        <location filename="main.cpp" line="357"/>
        <source>This is the first time you&apos;re starting Mumble.&lt;br /&gt;Would you like to go through the Audio Wizard to configure your soundcard?</source>
        <translation>Első alkalommal indította el a Mumble programot.&lt;br /&gt;Szeretné, hogy a Hangvarázsló végigvezesse önt a hangkártya beállításain?</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="533"/>
        <location filename="Messages.cpp" line="534"/>
        <source>Server</source>
        <comment>message from</comment>
        <translation>Szerver</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="619"/>
        <source>Hide Frame</source>
        <translation>Keret elrejtése</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="622"/>
        <source>Toggle showing frame on minimal window</source>
        <translation>A minimális ablak keretének megjelenítését váltja</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="625"/>
        <source>This will toggle whether the minimal window should have a frame for moving and resizing.</source>
        <translation>Ezzel tudja váltani azt, hogy a minimális ablaknak legyen vagy ne legyen kerete, amivel mozgatni és átméretezni lehet az ablakot.</translation>
    </message>
</context>
<context>
    <name>NetworkConfig</name>
    <message>
        <location filename="NetworkConfig.cpp" line="52"/>
        <source>Network</source>
        <translation>Hálózat</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="26"/>
        <source>Connection</source>
        <translation>Kapcsolat</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="35"/>
        <source>&lt;b&gt;Enable TCP compatibility mode&lt;/b&gt;.&lt;br /&gt;This will make Mumble use only TCP when communicating with the server. This will increase overhead and cause lost packets to produce noticeable pauses in communication, so this should only be used if you are unable to use the default (which uses UDP for voice and TCP for control).</source>
        <translation>&lt;b&gt;Engedélyezi a TCP kompatibilitási módot&lt;/&gt;.&lt;br /&gt;Ez azt eredményezi, hogy a Mumble csak a TCP protokollt használja a szerverrel való kommunikációban. Ez megnöveli a többletterhelést és a beszédben hallható szüneteket okozhat a csomagok elvesztése, ezért csak akkor használja, ha nem tudja az alapértelmezett módot használni (UDP a hangcsatornának és TCP a vezérlő csatornának).</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="58"/>
        <source>Reconnect when disconnected</source>
        <translation>Újrakapcsolódik amikor megszakad a kapcsolata</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="61"/>
        <source>&lt;b&gt;Reconnect when disconnected&lt;/b&gt;.&lt;br /&gt;This will make Mumble try to automatically reconnect after 10 seconds if your server connection fails.</source>
        <translation>&lt;b&gt;Újrakapcsolódik amikor megszakad a kapcsolata&lt;/b&gt;.&lt;br /&gt;Azt eredményezi, hogy ha megszakad a szerverrel a kapcsolata, akkor 10 másodperc múlva a Mumble megpróbál automatikusan újrakapcsolódni.</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="64"/>
        <source>Reconnect automatically</source>
        <translation>Automatikus újracsatlakozás</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="93"/>
        <source>Proxy</source>
        <translation>Proxy</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="99"/>
        <source>Type</source>
        <translation>Típus</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="112"/>
        <source>Type of proxy to connect through</source>
        <translation>Használandó proxy típusa</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="119"/>
        <source>Direct connection</source>
        <translation>Közvetlen kapcsolat</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="124"/>
        <source>HTTP(S) proxy</source>
        <translation>HTTP(S) proxy</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="129"/>
        <source>SOCKS5 proxy</source>
        <translation>SOCKS5 proxy</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="137"/>
        <source>Hostname</source>
        <translation>Gépnév</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="153"/>
        <source>Hostname of the proxy</source>
        <translation>Proxy gépneve</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="32"/>
        <source>Use TCP compatibility mode</source>
        <translation>Használja a TCP kompatibilis módot</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="14"/>
        <source>Form</source>
        <translation>Űrlap</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="38"/>
        <source>Force TCP mode</source>
        <translation>Kikényszeríti a TCP módot</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="45"/>
        <source>Enable QoS to prioritize packets</source>
        <translation>Engedélyezi a csomagok, QoS által, sürgősség szerinti besorolását</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="48"/>
        <source>This will enable QoS, which will attempt to prioritize voice packets over other traffic.</source>
        <translation>Engedélyezi a QoS-t, amely megpróbálja a hangcsomagokat elsőbbségben részesíteni a többivel szemben.</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="51"/>
        <source>Use Quality of Service</source>
        <translation>Használja a Szolgáltatás minőségét (QoS)</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="71"/>
        <source>Don&apos;t send certificate to server and don&apos;t save passwords. (Not saved).</source>
        <translation>Nem küld tanúsítványt a szervernek és nem menti el a jelszavakat. (Nincs elmentve).</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="74"/>
        <source>&lt;b&gt;This will suppress identity information from the client.&lt;/b&gt;&lt;p&gt;The client will not identify itself with a certificate, even if defined, and will not cache passwords for connections. This is primarily a test-option and is not saved.&lt;/p&gt;</source>
        <translation>&lt;b&gt;Elrejti a kliens azonosságát&lt;/b&gt;&lt;p&gt;A kliens nem fogja tanusítvánnyal hitelesíteni magát, még ha van is, és nem fogja tárolni a kapcsolat jelszavait. Elsődlegesen csak tesztelésre használható és ezt a beállítást nem menti el.&lt;/p&gt;</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="77"/>
        <source>Suppress certificate and password storage</source>
        <translation>Elnyomja a tanúsítvány és jelszó tárolását</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="115"/>
        <source>&lt;b&gt;Type of proxy to connect through.&lt;/b&gt;&lt;br /&gt;This makes Mumble connect through a proxy for all outgoing connections. Note: Proxy tunneling forces Mumble into TCP compatibility mode, causing all voice data to be sent via the control channel.</source>
        <translation>&lt;b&gt;A használandó proxy típusa.&lt;/b&gt;&lt;br /&gt;Azt eredményezi, hogy a Mumble minden kimenő kapcsolatát egy proxyn keresztül létesíti. Megjegyzés: A proxy használata arra kényszeríti a Mumble-t, hogy TCP kompatibilitási módot használjon, ami azt jelenti, hogy minden hangot a vezérlő csatornán keresztül küld.</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="156"/>
        <source>&lt;b&gt;Hostname of the proxy.&lt;/b&gt;&lt;br /&gt;This field specifies the hostname of the proxy you wish to tunnel network traffic through.</source>
        <translation>&lt;b&gt;A proxy gépneve.&lt;/b&gt;&lt;br /&gt;Ebben a mezőben adhatja meg annak a proxy-nak a gépnevét, amelyen keresztül szeretné bonyolítani a halózati forgalmát.</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="178"/>
        <source>Port</source>
        <translation>Portszám</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="203"/>
        <source>Port number of the proxy</source>
        <translation>A proxy portszáma</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="206"/>
        <source>&lt;b&gt;Port number of the proxy.&lt;/b&gt;&lt;br /&gt;This field specifies the port number that the proxy expects connections on.</source>
        <translation>&lt;b&gt;A proxy portszáma.&lt;/b&gt;&lt;br /&gt;Ebben a mezőben adhatja meg annak a proxy-nak a portszámát, amelyhez kapcsolódni kíván.</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="222"/>
        <source>Username</source>
        <translation>Felhasználónév</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="229"/>
        <source>Username for proxy authentication</source>
        <translation>A felhasználónév a proxy-nál való hitelesítéshez</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="232"/>
        <source>&lt;b&gt;Username for proxy authentication.&lt;/b&gt;&lt;br /&gt;This specifies the username you use for authenticating yourself with the proxy. In case the proxy does not use authentication, or you want to connect anonymously, simply leave this field blank.</source>
        <translation>&lt;b&gt;A felhasználónév a proxy-nál való hitelesítéshez.&lt;/b&gt;&lt;br /&gt;Itt adhatja meg a felhasználónevet, amellyel hitelesítheti magát a proxy-nál. Ha a proxy nem használ hitelesítést vagy névtelenül szeretne kapcsolódni, egyszerűen hagyja üresen a mezőt.</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="239"/>
        <source>Password</source>
        <translation>Jelszó</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="246"/>
        <source>Password for proxy authentication</source>
        <translation>A jelszó a proxy-nál való hitelesítéshez</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="249"/>
        <source>&lt;b&gt;Password for proxy authentication.&lt;/b&gt;&lt;br /&gt;This specifies the password you use for authenticating yourself with the proxy. In case the proxy does not use authentication, or you want to connect anonymously, simply leave this field blank.</source>
        <translation>&lt;b&gt;A jelszó a proxy-nál való hitelesítéshez.&lt;/b&gt;&lt;br /&gt;Itt adhatja meg a jelszót, amellyel hitelesítheti magát a proxy-nál. Ha a proxy nem használ hitelesítést vagy névtelenül szeretne kapcsolódni, egyszerűen hagyja üresen a mezőt.</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="268"/>
        <source>Misc</source>
        <translation>Egyéb</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="274"/>
        <source>Prevent log from downloading images</source>
        <translation>Megakadályozza, hogy a naplózás képeket töltsön le</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="277"/>
        <source>&lt;b&gt;Disable image download&lt;/b&gt;&lt;br/&gt;
Prevents the client from downloading images embedded into chat messages with the img tag.</source>
        <translation>&lt;b&gt;Képek letöltésének tiltása&lt;/b&gt;&lt;br /&gt;
Megakadályozza a klienst, hogy olyan képeket töltsön le, amelyek a csevegés üzeneteiben az &apos;img&apos; kifejezéssel jelöltek.</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="281"/>
        <source>Disable image download</source>
        <translation>Képek letöltésének tiltása</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="291"/>
        <source>Mumble services</source>
        <translation>Mumble szolgáltatások</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="297"/>
        <source>Check for new releases of Mumble automatically.</source>
        <translation>A Mumble új verzióinak automatikus keresése.</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="300"/>
        <source>This will check for new releases of Mumble every time you start the program, and notify you if one is available.</source>
        <translation>Keresni fogja a Mumble új verzióit minden alkalommal, amikor elindítja a programot és ha van elérhető frissítés értesíti önt.</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="303"/>
        <source>Check for application updates on startup</source>
        <translation>Indításkor alkalmazásfrissítés keresése</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="310"/>
        <source>Check for new releases of plugins automatically.</source>
        <translation>A pluginek új verzióinak automatikus keresése.</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="313"/>
        <source>This will check for new releases of plugins every time you start the program, and download them automatically.</source>
        <translation>Keresni fogja a pluginek új verzióit minden alkalommal, amikor elindítja a programot és ha talál, automatikusan letölti őket.</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="326"/>
        <source>&lt;b&gt;Submit anonymous statistics.&lt;/b&gt;&lt;br /&gt;Mumble has a small development team, and as such needs to focus its development where it is needed most. By submitting a bit of statistics you help the project determine where to focus development.</source>
        <translation>&lt;b&gt;Statisztikát küld névtelenül.&lt;/b&gt;&lt;br /&gt;A Mumble-nak van egy kis fejlesztő csapata és mint olyanoknak, arra kell fókuszálniuk a fejlesztés irányát, amerre a legnagyobb igény van. Egy kis statisztika küldésével segítheti a fejlesztőket a fejlesztés irányának meghatározásában.</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="323"/>
        <source>Submit anonymous statistics to the Mumble project</source>
        <translation>Statisztikát küld névtelenül a Mumble fejlesztőknek</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="316"/>
        <source>Download plugin updates on startup</source>
        <translation>Indításkor plugin frissítések letöltése</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui" line="329"/>
        <source>Submit anonymous statistics</source>
        <translation>Statisztikák küldése névtelenül</translation>
    </message>
</context>
<context>
    <name>Overlay</name>
    <message>
        <location filename="Overlay.cpp" line="642"/>
        <source>Failed to create communication with overlay at %2: %1. No overlay will be available.</source>
        <translatorcomment>Nem sikerült a kommunikáció a overlay-el: %2: %1. Az overlay (játék közbeni kijelző) nem lesz elérhető.</translatorcomment>
        <translation></translation>
    </message>
</context>
<context>
    <name>OverlayConfig</name>
    <message>
        <location filename="Overlay.cpp" line="48"/>
        <source>Show no one</source>
        <translation>Senkit</translation>
    </message>
    <message>
        <location filename="Overlay.cpp" line="49"/>
        <source>Show only talking</source>
        <translation>Csak beszélőket</translation>
    </message>
    <message>
        <location filename="Overlay.cpp" line="50"/>
        <source>Show everyone</source>
        <translation>Mindenkit</translation>
    </message>
    <message>
        <location filename="Overlay.cpp" line="129"/>
        <source>Color for channels</source>
        <translation>Csatornák színe</translation>
    </message>
    <message>
        <location filename="Overlay.cpp" line="133"/>
        <source>Color for active channels</source>
        <translation>Aktív csatornák színe</translation>
    </message>
    <message>
        <location filename="Overlay.cpp" line="137"/>
        <source>Overlay</source>
        <translation>Overlay</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="14"/>
        <source>Form</source>
        <translation>Űrlap</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="20"/>
        <source>Options</source>
        <translation>Beállítások</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="26"/>
        <source>Enable overlay.</source>
        <translation>Overlay engedélyezése.</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="29"/>
        <source>This sets whether the overlay is enabled or not. This setting is only checked when applications are started, so make sure Mumble is running and this option is on before you start the application.&lt;br /&gt;Please note that if you start Mumble after starting the application, or if you disable the overlay while the application is running, there is no safe way to restart the overlay without also restarting the application.</source>
        <translation>A játék közben használt kijelzőt (overlay) engedélyezi. Ezt a beállítást csak az alkalmazások indításakor ellenőrzi, tehát bizonyosodjon meg róla, hogy fut a Mumble és az overlay itt be van kapcsolva, mielőtt elindítaná az alkalmazást (játék programot).&lt;br /&gt;Kérem vegye figyelembe, hogy ha a Mumble-t az alkalmazás futása közben indítja vagy ha kikapcsolja az overlay-t míg az alkalmazás fut, nincs biztonságos mód az overlay újraindítására anélkül, hogy újraindítaná az alkalmazást.</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="32"/>
        <source>Enable Overlay</source>
        <translation>Overlay engedélyezése</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="39"/>
        <source>Who to show on the overlay</source>
        <translation>Kit mutasson az overlay</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="42"/>
        <source>&lt;b&gt;This sets who to show in the in-game overlay.&lt;/b&gt;&lt;br /&gt;If many people are connected to the same channel, the overlay list might be very long. Use this to shorten it.&lt;br /&gt;&lt;i&gt;No one&lt;/i&gt; - Don&apos;t show anyone (but leave overlay running).&lt;br /&gt;&lt;i&gt;Only talking&lt;/i&gt; - Only show talking people.&lt;br /&gt;&lt;i&gt;Everyone&lt;/i&gt; - Show everyone.</source>
        <translation>&lt;b&gt;Kit mutasson a játék közben használt kijelzőn (overlay).&lt;/b&gt;&lt;br /&gt;Ha sok ember kapcsolódott ugyanarra a csatornára, az overlay listája túl hosszú lehet. Ezzel rövidítheti a listát.&lt;br /&gt;&lt;i&gt;Senkit&lt;/i&gt; - Senkit sem mutat (de az overlay működik).&lt;br /&gt;&lt;i&gt;Csak beszélőket&lt;/i&gt; - Csak a beszélőket mutatja.&lt;br /&gt;&lt;i&gt;Mindenkit&lt;/i&gt; - Mindenkit mutat.</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="49"/>
        <source>Always show yourself on overlay.</source>
        <translation>Mindig mutatja önmagát az overlay-en.</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="52"/>
        <source>This sets whether to always show yourself or not. This setting is useful if you aren&apos;t showing everyone in the overlay, as then you would only see your own status if you were talking, which wouldn&apos;t let you see that you were deafened or muted.</source>
        <translation>Beállítja, hogy mindig mutatja önmagát. Ez a beállítás akkor hasznos, ha a játék közben használt kijelzőn (overlay) nem mutat senkit és így a saját státuszát láthatja beszéd közben, amit nem láthatna, ha el lenne némítva vagy a hangszórói ki lennének kapcsolva.</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="55"/>
        <source>Always Show Self</source>
        <translation>Mindig mutatja önmagát</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="65"/>
        <source>Position</source>
        <translation>Pozíció</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="71"/>
        <source>Let overlay grow upwards</source>
        <translation>Engedi az overlay növekedését felfelé</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="74"/>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow upwards if needed.</source>
        <translation>A játék közben használt kijelző (overlay) megpróbál olyan kicsinek lenni, amilyen csak tud és abban a helyzetben maradni, amit ön kiválasztott. Ez a beállítás lehetővé teszi az overlay-nek, hogy ha szükséges felfele növekedjen.</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="77"/>
        <source>Grow Up</source>
        <translation>Felfele nő</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="84"/>
        <source>Y-Position of Overlay</source>
        <translation>Az overlay függőleges helyzete</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="87"/>
        <source>This sets the relative Y position of the overlay.</source>
        <translation>Az overlay relatív függőleges helyzetét állítja be.</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="100"/>
        <source>Let overlay grow to the left</source>
        <translation>Engedi az overlay növekedését a baloldali irányba</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="103"/>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow to the left if needed.</source>
        <translation>A játék közben használt kijelző (overlay) megpróbál olyan kicsinek lenni, amilyen csak tud és abban a helyzetben maradni, amit ön kiválasztott. Ez a beállítás lehetővé teszi az overlay-nek, hogy ha szükséges baloldali irányba növekedjen.</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="106"/>
        <source>Grow Left</source>
        <translation>Baloldalra nő</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="113"/>
        <source>Let overlay grow to the right</source>
        <translation>Engedi az overlay növekedését a jobboldali irányba</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="116"/>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow to the right if needed.</source>
        <translation>A játék közben használt kijelző (overlay) megpróbál olyan kicsinek lenni, amilyen csak tud és abban a helyzetben maradni, amit ön kiválasztott. Ez a beállítás lehetővé teszi az overlay-nek, hogy ha szükséges a jobboldali irányba növekedjen.</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="119"/>
        <source>Grow Right</source>
        <translation>Jobboldalra nő</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="126"/>
        <source>Let overlay grow downwards</source>
        <translation>Engedi az overlay növekedését lefelé</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="129"/>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow downwards if needed.</source>
        <translation>A játék közben használt kijelző (overlay) megpróbál olyan kicsinek lenni, amilyen csak tud és abban a helyzetben maradni, amit ön kiválasztott. Ez a beállítás lehetővé teszi az overlay-nek, hogy ha szükséges lefele növekedjen.</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="132"/>
        <source>Grow Down</source>
        <translation>Lefele nő</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="139"/>
        <source>X-Position of Overlay</source>
        <translation>Az overlay vízszintes helyzete</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="142"/>
        <source>This sets the relative X position of the overlay.</source>
        <translation>Az overlay relatív vízszintes helyzetét állítja be.</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="158"/>
        <source>Font</source>
        <translation>Betűkészlet</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="164"/>
        <source>Current Font</source>
        <translation>Jelenlegi betűkészlet</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="178"/>
        <source>Set Font</source>
        <translation>Betűkészlet beállítása</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="185"/>
        <source>Maximum height</source>
        <translation>Maximális magasság</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="192"/>
        <source>Maximum height of names.</source>
        <translation>Nevek maximális magassága.</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="195"/>
        <source>This sets the maximum height of names shown, relative to the screen height. If your active 3D window is 800 pixels tall and this is set to 5%, each name will be 40 pixels tall. Note that the names will not be taller than 60 pixels no matter what you set here.</source>
        <translation>A képernyő magasságához képest, a mutatott nevek relatív, maximális magasságát állítja be. Ha az ön aktív 3D-s ablaka 800 képpont magas és ez az érték 5%-ra van állítva, akkor minden név 40 képpont magas lesz. Megjegyzés képpen, a nevek nem lesznek magasabbak, mint 60 képpont bármit állít itt be.</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="211"/>
        <source>TextLabel</source>
        <translation>Szöveg</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="218"/>
        <location filename="Overlay.cpp" line="117"/>
        <source>Color for users</source>
        <translation>Felhasználók színe</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="225"/>
        <location filename="Overlay.ui" line="246"/>
        <location filename="Overlay.ui" line="267"/>
        <location filename="Overlay.ui" line="288"/>
        <location filename="Overlay.ui" line="309"/>
        <source>Change</source>
        <translation>Módosítás</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="232"/>
        <location filename="Overlay.cpp" line="121"/>
        <source>Color for talking users</source>
        <translation>Beszélő színe</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="253"/>
        <location filename="Overlay.cpp" line="125"/>
        <source>Color for whispering users</source>
        <translation>Suttogó színe</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="274"/>
        <source>Color for Channels</source>
        <translation>Csatornák színe</translation>
    </message>
    <message>
        <location filename="Overlay.ui" line="295"/>
        <source>Color for active Channels</source>
        <translation>Aktív csatorna színe</translation>
    </message>
</context>
<context>
    <name>OverlayPrivateWin</name>
    <message>
        <location filename="Overlay_win.cpp" line="62"/>
        <source>Failed to load overlay library. This means either that:
- the library (mumble_ol.dll) wasn&apos;t found in the directory you ran Mumble from
- you&apos;re on an OS earlier than WinXP SP2</source>
        <translation>Az overlay függvénykönyvtár betöltése nem sikerült. Ez a következőket jelentheti::
- a függvénykönyvtár (mumble_ol.dll) nem található a mappában, ahonnan a Mumble-t futtatja
- Windows XP SP2-nél régebbi operációs rendszere van</translation>
    </message>
</context>
<context>
    <name>PluginConfig</name>
    <message>
        <location filename="Plugins.ui" line="42"/>
        <location filename="Plugins.cpp" line="72"/>
        <source>Plugins</source>
        <translation>Pluginok</translation>
    </message>
    <message>
        <location filename="Plugins.cpp" line="110"/>
        <source>Plugin has no configure function.</source>
        <translation>A pluginnak nincs beállítási funkciója.</translation>
    </message>
    <message>
        <location filename="Plugins.cpp" line="122"/>
        <source>Plugin has no about function.</source>
        <translation>A pluginnak nincs névjegye.</translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="14"/>
        <source>Form</source>
        <translation>Űrlap</translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="20"/>
        <source>Options</source>
        <translation>Beállítások</translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="26"/>
        <source>Enable plugins and transmit positional information</source>
        <translation>Pluginok engedélyezése és a pozíció továbbítása</translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="29"/>
        <source>This allows plugins for supported games to fetch your in-game position and transmit it with each voice packet. This enables other users to hear your voice in-game from the direction your character is in relation to their own.</source>
        <translation>Ezzel engedélyezi a pluginoknak, a támogatott játékoktól lekérdezze az ön pozícióját a játékban és elküldje azt minden hangcsomagban. Ez lehetővé teszi a többi játékos számára, hogy az ön hangját a játékban a saját személyükhöz képest megfelelő irányból hallják.</translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="32"/>
        <source>Link to Game and Transmit Position</source>
        <translation>Kapcsolódik a játékhoz és továbbítja a pozícióját</translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="61"/>
        <source>Name</source>
        <translation>Név</translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="66"/>
        <source>Enabled</source>
        <translation>Engedélyezett</translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="76"/>
        <source>Reloads all plugins</source>
        <translation>Újratölt minden plugint</translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="79"/>
        <source>This rescans and reloads plugins. Use this if you just added or changed a plugin to the plugins directory.</source>
        <translation>Újra átnézi és betölti a pluginokat. Akkor használja, ha épp most tett be új plugint a mappába, vagy frissítette a korábbit.</translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="82"/>
        <source>&amp;Reload plugins</source>
        <translation>Pluginok új&amp;ratöltése</translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="102"/>
        <source>Information about plugin</source>
        <translation>Információ a pluginról</translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="105"/>
        <source>This shows a small information message about the plugin.</source>
        <translation>Ez egy kis információt ad a pluginról.</translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="108"/>
        <source>&amp;About</source>
        <translation>&amp;Névjegy</translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="115"/>
        <source>Show configuration page of plugin</source>
        <translation>Megmutatja a beállító ablakot</translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="118"/>
        <source>This shows the configuration page of the plugin, if any.</source>
        <translation>Ez mutatja a beállító ablakát a pluginnak, ha van neki.</translation>
    </message>
    <message>
        <location filename="Plugins.ui" line="121"/>
        <source>&amp;Configure</source>
        <translation>&amp;Beállítás</translation>
    </message>
</context>
<context>
    <name>Plugins</name>
    <message>
        <location filename="Plugins.cpp" line="319"/>
        <source>%1 lost link.</source>
        <translation>%1 elveszett kapcsolat.</translation>
    </message>
    <message>
        <location filename="Plugins.cpp" line="366"/>
        <source>%1 linked.</source>
        <translation>%1 kapcsolva.</translation>
    </message>
    <message>
        <location filename="Plugins.cpp" line="539"/>
        <location filename="Plugins.cpp" line="545"/>
        <source>Downloaded new or updated plugin to %1.</source>
        <translation>Új plugin letöltve vagy frissítve: %1.</translation>
    </message>
    <message>
        <location filename="Plugins.cpp" line="547"/>
        <source>Failed to install new plugin to %1.</source>
        <translation>Nem sikerült új plugin telepítése: %1.</translation>
    </message>
</context>
<context>
    <name>PortAudioSystem</name>
    <message>
        <location filename="PAAudio.cpp" line="275"/>
        <source>Default Device</source>
        <translation>Alapértelmezett eszköz</translation>
    </message>
</context>
<context>
    <name>PulseAudioSystem</name>
    <message>
        <location filename="PulseAudio.cpp" line="549"/>
        <source>Default Input</source>
        <translation>Alapértelmezett bemenet</translation>
    </message>
    <message>
        <location filename="PulseAudio.cpp" line="550"/>
        <source>Default Output</source>
        <translation>Alapértelmezett kimenet</translation>
    </message>
</context>
<context>
    <name>RichTextEditor</name>
    <message>
        <location filename="RichTextEditor.ui" line="24"/>
        <source>Display</source>
        <translation>Kijelző</translation>
    </message>
    <message>
        <location filename="RichTextEditor.ui" line="41"/>
        <source>Source Text</source>
        <translation>Szöveg forrása</translation>
    </message>
    <message>
        <location filename="RichTextEditor.ui" line="58"/>
        <source>&amp;Bold</source>
        <translation>&amp;Kövér</translation>
    </message>
    <message>
        <location filename="RichTextEditor.ui" line="61"/>
        <source>Ctrl+B</source>
        <translation>Ctrl+K</translation>
    </message>
    <message>
        <location filename="RichTextEditor.ui" line="73"/>
        <source>&amp;Italic</source>
        <translation>&amp;Dőlt</translation>
    </message>
    <message>
        <location filename="RichTextEditor.ui" line="76"/>
        <source>Italic</source>
        <translation>Dőlt betű</translation>
    </message>
    <message>
        <location filename="RichTextEditor.ui" line="79"/>
        <source>Ctrl+I</source>
        <translation>Ctrl+D</translation>
    </message>
    <message>
        <location filename="RichTextEditor.ui" line="91"/>
        <source>Underline</source>
        <translation>Aláhúzott</translation>
    </message>
    <message>
        <location filename="RichTextEditor.ui" line="94"/>
        <source>Ctrl+U</source>
        <translation>Ctrl+A</translation>
    </message>
    <message>
        <location filename="RichTextEditor.ui" line="99"/>
        <source>Color</source>
        <translation>Szín</translation>
    </message>
    <message>
        <location filename="RichTextEditor.ui" line="108"/>
        <source>Insert Link</source>
        <translation>Hivatkozás beillesztése</translation>
    </message>
    <message>
        <location filename="RichTextEditor.ui" line="111"/>
        <source>Ctrl+L</source>
        <translation>Ctrl+H</translation>
    </message>
    <message>
        <location filename="RichTextEditor.ui" line="120"/>
        <source>Insert Image</source>
        <translation>Kép beillesztése</translation>
    </message>
    <message>
        <location filename="RichTextEditor.cpp" line="241"/>
        <source>Failed to load image</source>
        <translation>Kép betöltése nem sikerült</translation>
    </message>
    <message>
        <location filename="RichTextEditor.cpp" line="241"/>
        <source>Image file too large to embed in document. Please use images smaller than %1 kB.</source>
        <translation>A képfájl túl nagy ahhoz, hogy beilleszthesse a dokumentumba. Kérem válasszon kisebb méretű képet mint %1 kB.</translation>
    </message>
    <message>
        <location filename="RichTextEditor.cpp" line="322"/>
        <source>Message is too long.</source>
        <translation>Az üzenet túl hosszú.</translation>
    </message>
</context>
<context>
    <name>RichTextEditorLink</name>
    <message>
        <location filename="RichTextEditorLink.ui" line="14"/>
        <source>Add Link</source>
        <translation>Kapcsolat hozzáadása</translation>
    </message>
    <message>
        <location filename="RichTextEditorLink.ui" line="20"/>
        <source>URL</source>
        <translation>URL</translation>
    </message>
    <message>
        <location filename="RichTextEditorLink.ui" line="33"/>
        <source>Text</source>
        <translation>Szöveg</translation>
    </message>
</context>
<context>
    <name>ServerHandler</name>
    <message>
        <location filename="ServerHandler.cpp" line="394"/>
        <source>UDP packets cannot be sent to or received from the server. Switching to TCP mode.</source>
        <translation>A szerver felé nem küldhető vagy a szervertől nem fogadható UDP csomag. TCP módra váltok.</translation>
    </message>
    <message>
        <location filename="ServerHandler.cpp" line="396"/>
        <source>UDP packets cannot be sent to the server. Switching to TCP mode.</source>
        <translation>A szerver felé nem küldhető UDP csomag. TCP módra váltok.</translation>
    </message>
    <message>
        <location filename="ServerHandler.cpp" line="398"/>
        <source>UDP packets cannot be received from the server. Switching to TCP mode.</source>
        <translation>A szervertől nem fogadható UDP csomag. TCP módra váltok.</translation>
    </message>
    <message>
        <location filename="ServerHandler.cpp" line="405"/>
        <source>UDP packets can be sent to and received from the server. Switching back to UDP mode.</source>
        <translation>A szerver felé küldhető és a szervertől fogadható UDP csomag. Vissza váltok UDP módra.</translation>
    </message>
    <message>
        <location filename="ServerHandler.cpp" line="434"/>
        <source>Connection timed out</source>
        <translation>A kapcsolat ideje lejárt</translation>
    </message>
</context>
<context>
    <name>ServerView</name>
    <message>
        <location filename="ConnectDialog.cpp" line="72"/>
        <source>Favorite</source>
        <translation>Kedvencek</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="78"/>
        <source>LAN</source>
        <translation>Helyi hálózat</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="86"/>
        <source>Public Internet</source>
        <translation>Nyilvános Internet</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="92"/>
        <source>Africa</source>
        <translation>Afrika</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="93"/>
        <source>Asia</source>
        <translation>Ázsia</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="94"/>
        <source>North America</source>
        <translation>Észak Amerika</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="95"/>
        <source>South America</source>
        <translation>Dél Amerika</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="96"/>
        <source>Europe</source>
        <translation>Európa</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="97"/>
        <source>Oceania</source>
        <translation>Óceánia</translation>
    </message>
</context>
<context>
    <name>ShortcutActionWidget</name>
    <message>
        <location filename="GlobalShortcut.cpp" line="214"/>
        <source>Unassigned</source>
        <translation>Nincs megadva</translation>
    </message>
</context>
<context>
    <name>ShortcutDelegate</name>
    <message>
        <location filename="GlobalShortcut.cpp" line="574"/>
        <source>On</source>
        <translation>Be</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.cpp" line="576"/>
        <source>Off</source>
        <translation>Ki</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.cpp" line="578"/>
        <source>Toggle</source>
        <translation>Vált</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.cpp" line="584"/>
        <source>Unassigned</source>
        <translation>Semmi</translation>
    </message>
</context>
<context>
    <name>ShortcutKeyWidget</name>
    <message>
        <location filename="GlobalShortcut.cpp" line="150"/>
        <source>Press Shortcut</source>
        <translation>Nyomd meg a gyorsbillentyűt</translation>
    </message>
</context>
<context>
    <name>ShortcutTargetDialog</name>
    <message>
        <location filename="GlobalShortcut.cpp" line="353"/>
        <source>Root</source>
        <translation>Gyökér</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.cpp" line="358"/>
        <source>Parent</source>
        <translation>Szülő</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.cpp" line="363"/>
        <source>Current</source>
        <translation>Aktuális</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.cpp" line="368"/>
        <source>Subchannel #%1</source>
        <translation>Alcsatorna: #%1</translation>
    </message>
</context>
<context>
    <name>ShortcutTargetWidget</name>
    <message>
        <location filename="GlobalShortcut.cpp" line="455"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.cpp" line="493"/>
        <source>, </source>
        <translation>, </translation>
    </message>
    <message>
        <location filename="GlobalShortcut.cpp" line="499"/>
        <source>Root</source>
        <translation>Gyökér</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.cpp" line="501"/>
        <source>Parent</source>
        <translation>Szülő</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.cpp" line="503"/>
        <source>Current</source>
        <translation>Aktuális</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.cpp" line="505"/>
        <source>Subchannel #%1</source>
        <translation>Alcsatorna: #%1</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.cpp" line="512"/>
        <source>Invalid</source>
        <translation>Érvénytelen</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.cpp" line="515"/>
        <source>&lt;Empty&gt;</source>
        <translation>&lt;Üres&gt;</translation>
    </message>
</context>
<context>
    <name>ShortcutToggleWidget</name>
    <message>
        <location filename="GlobalShortcut.cpp" line="254"/>
        <source>Off</source>
        <translation>Ki</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.cpp" line="258"/>
        <source>Toggle</source>
        <translation>Vált</translation>
    </message>
    <message>
        <location filename="GlobalShortcut.cpp" line="262"/>
        <source>On</source>
        <translation>Be</translation>
    </message>
</context>
<context>
    <name>TextMessage</name>
    <message>
        <location filename="TextMessage.ui" line="24"/>
        <source>If checked the message is recursively sent to all subchannels</source>
        <translation>Ha kipipálva, akkor az üzenetet rekuzívan küldi minden alcsatornába</translation>
    </message>
    <message>
        <location filename="TextMessage.ui" line="27"/>
        <source>Send recursively to subchannels</source>
        <translation>Rekurzívan küldi minden alcsatornába</translation>
    </message>
</context>
<context>
    <name>Tokens</name>
    <message>
        <location filename="Tokens.ui" line="14"/>
        <source>Mumble - Access Tokens</source>
        <translation>Mumble - Hozzáférési token</translation>
    </message>
    <message>
        <location filename="Tokens.ui" line="20"/>
        <source>List of access tokens on current server</source>
        <translation>Az aktuális szerver hozzáférési token-listája</translation>
    </message>
    <message>
        <location filename="Tokens.ui" line="23"/>
        <source>&lt;b&gt;This is an editable list of access tokens on the connected server.&lt;/b&gt;
&lt;br /&gt;
An access token is a text string, which can be used as a password for very simple access management on channels. Mumble will remember the tokens you&apos;ve used and resend them to the server next time you reconnect, so you don&apos;t have to enter these every time.
</source>
        <translation>&lt;b&gt;Ez egy szerkeszthető listája a hozzáférési tokeneknek, amelyek a kapcsolatban levő szerveren vannak.&lt;/b&gt;
&lt;br /&gt;
A hozzáférési token egy szöveges karaktersorozat, amely jelszóként használható a csatornák hozzáféréseinek nagyon egyszerű kezeléshez. A Mumble megjegyzi az ön által használt tokeneket és a legközelebbi kapcsolódáskor újraküldi őket a szervernek, így önnek nem kell mindig újra beírnia.</translation>
    </message>
    <message>
        <location filename="Tokens.ui" line="47"/>
        <source>Add a token</source>
        <translation>Token hozzáadása</translation>
    </message>
    <message>
        <location filename="Tokens.ui" line="50"/>
        <source>&amp;Add</source>
        <translation>&amp;Hozzáadás</translation>
    </message>
    <message>
        <location filename="Tokens.ui" line="57"/>
        <source>Remove a token</source>
        <translation>Token eltávolítása</translation>
    </message>
    <message>
        <location filename="Tokens.ui" line="60"/>
        <source>&amp;Remove</source>
        <translation>&amp;Eltávolítás</translation>
    </message>
    <message>
        <location filename="Tokens.cpp" line="69"/>
        <source>Empty Token</source>
        <translation>Üres token</translation>
    </message>
</context>
<context>
    <name>UserEdit</name>
    <message>
        <location filename="UserEdit.ui" line="14"/>
        <location filename="UserEdit.ui" line="20"/>
        <source>Registered Users</source>
        <translation>Regisztrált felhasználók</translation>
    </message>
    <message>
        <location filename="UserEdit.ui" line="26"/>
        <location filename="UserEdit.cpp" line="96"/>
        <source>Remove</source>
        <translation>Eltávolítás</translation>
    </message>
    <message>
        <location filename="UserEdit.cpp" line="91"/>
        <source>Rename</source>
        <translation>Átnevezés</translation>
    </message>
</context>
<context>
    <name>UserInformation</name>
    <message>
        <location filename="UserInformation.ui" line="14"/>
        <source>User Information</source>
        <translation>Felhasználó információi</translation>
    </message>
    <message>
        <location filename="UserInformation.ui" line="20"/>
        <source>Connection Information</source>
        <translation>Kapcsolat információi</translation>
    </message>
    <message>
        <location filename="UserInformation.ui" line="26"/>
        <source>Version</source>
        <translation>Verzió</translation>
    </message>
    <message>
        <location filename="UserInformation.ui" line="52"/>
        <source>OS</source>
        <translation>OP. rendszer</translation>
    </message>
    <message>
        <location filename="UserInformation.ui" line="78"/>
        <source>Certificate</source>
        <translation>Tanúsítvány</translation>
    </message>
    <message>
        <location filename="UserInformation.ui" line="104"/>
        <source>IP Address</source>
        <translation>IP cím</translation>
    </message>
    <message>
        <location filename="UserInformation.ui" line="127"/>
        <source>CELT Versions</source>
        <translation>CELT verzió</translation>
    </message>
    <message>
        <location filename="UserInformation.ui" line="165"/>
        <source>Details...</source>
        <translation>Részletek...</translation>
    </message>
    <message>
        <location filename="UserInformation.ui" line="175"/>
        <source>Ping Statistics</source>
        <translation>Kopogtatás statisztika</translation>
    </message>
    <message>
        <location filename="UserInformation.ui" line="181"/>
        <source>Pings recieved</source>
        <oldsource>Pings received</oldsource>
        <translation>Fogadott</translation>
    </message>
    <message>
        <location filename="UserInformation.ui" line="191"/>
        <source>Average ping</source>
        <translation>Átlag</translation>
    </message>
    <message>
        <location filename="UserInformation.ui" line="201"/>
        <source>Ping deviation</source>
        <translation>Eltérés</translation>
    </message>
    <message>
        <location filename="UserInformation.ui" line="211"/>
        <source>TCP (Control)</source>
        <translation>TCP (Vezérlés)</translation>
    </message>
    <message>
        <location filename="UserInformation.ui" line="257"/>
        <source>UDP (Voice)</source>
        <translation>UDP (Hang)</translation>
    </message>
    <message>
        <location filename="UserInformation.ui" line="306"/>
        <source>UDP Network statistics</source>
        <translation>UDP hálózati statisztika</translation>
    </message>
    <message>
        <location filename="UserInformation.ui" line="312"/>
        <source>Good</source>
        <translation>Jó</translation>
    </message>
    <message>
        <location filename="UserInformation.ui" line="322"/>
        <source>Late</source>
        <translation>Elkésett</translation>
    </message>
    <message>
        <location filename="UserInformation.ui" line="332"/>
        <source>Lost</source>
        <translation>Elveszett</translation>
    </message>
    <message>
        <location filename="UserInformation.ui" line="342"/>
        <source>Resync</source>
        <translation>Újra szinkronizálva</translation>
    </message>
    <message>
        <location filename="UserInformation.ui" line="352"/>
        <source>From Client</source>
        <translation>Klienstől</translation>
    </message>
    <message>
        <location filename="UserInformation.ui" line="411"/>
        <source>To Client</source>
        <translation>Kliensnek</translation>
    </message>
    <message>
        <location filename="UserInformation.ui" line="473"/>
        <source>Bandwidth</source>
        <comment>GroupBox</comment>
        <translation>Sávszélesség</translation>
    </message>
    <message>
        <location filename="UserInformation.ui" line="479"/>
        <source>Connection time</source>
        <translation>Kapcsolat ideje</translation>
    </message>
    <message>
        <location filename="UserInformation.ui" line="502"/>
        <source>Bandwidth</source>
        <comment>Label</comment>
        <translation>Sávszélesség</translation>
    </message>
    <message>
        <location filename="UserInformation.cpp" line="88"/>
        <source>%1w</source>
        <translation>%1h</translation>
    </message>
    <message>
        <location filename="UserInformation.cpp" line="90"/>
        <source>%1d</source>
        <translation>%1n</translation>
    </message>
    <message>
        <location filename="UserInformation.cpp" line="92"/>
        <source>%1h</source>
        <translation>%1ó</translation>
    </message>
    <message>
        <location filename="UserInformation.cpp" line="94"/>
        <source>%1m</source>
        <translation>%1p</translation>
    </message>
    <message>
        <location filename="UserInformation.cpp" line="95"/>
        <source>%1s</source>
        <translation>%1mp</translation>
    </message>
    <message>
        <location filename="UserInformation.cpp" line="120"/>
        <location filename="UserInformation.cpp" line="163"/>
        <source>, </source>
        <translation>, </translation>
    </message>
    <message>
        <location filename="UserInformation.cpp" line="150"/>
        <source>%1.%2.%3 (%4)</source>
        <translation>%1.%2.%3 (%4)</translation>
    </message>
    <message>
        <location filename="UserInformation.cpp" line="151"/>
        <source>%1 (%2)</source>
        <translation>%1 (%2)</translation>
    </message>
    <message>
        <location filename="UserInformation.cpp" line="196"/>
        <source>%1 online (%2 idle)</source>
        <translation>%1 online (%2 üresjárat)</translation>
    </message>
    <message>
        <location filename="UserInformation.cpp" line="198"/>
        <source>%1 online</source>
        <translation>%1 online</translation>
    </message>
    <message>
        <location filename="UserInformation.cpp" line="203"/>
        <source>%1 kbit/s</source>
        <translation>%1 kbit/s</translation>
    </message>
</context>
<context>
    <name>UserModel</name>
    <message>
        <location filename="UserModel.cpp" line="577"/>
        <source>This is a user connected to the server. The icon to the left of the user indicates whether or not they are talking:</source>
        <translation>Ez egy szerverhez kapcsolódott felhasználó. A felhasználó baloldalán levő ikon mutatja, hogy éppen beszél vagy sem:</translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="578"/>
        <source>Talking to your channel.</source>
        <translation>Az ön csatornáján beszél.</translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="579"/>
        <source>Whispering directly to your channel.</source>
        <translation>Közvetlenül az ön csatornáján suttog.</translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="580"/>
        <source>Whispering directly to you.</source>
        <translation>Közvetlenül önhöz beszél.</translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="581"/>
        <source>Not talking.</source>
        <translation>Hallgat.</translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="589"/>
        <source>This is a channel on the server. The icon indicates the state of the channel:</source>
        <translation>Ez egy csatorna a szerveren. Az ikon mutatja a csatorna állapotát:</translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="590"/>
        <source>Your current channel.</source>
        <translation>Az ön jelenlegi csatornája.</translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="591"/>
        <source>A channel that is linked with your channel. Linked channels can talk to each other.</source>
        <translation>Egy csatorna, amely az önével van összekötve. Az összekötött csatornák beszélhetnek egymással.</translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="592"/>
        <source>A channel on the server that you are not linked to.</source>
        <translation>Egy csatorna a szerveren, amellyel nincs kapcsolatban.</translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="608"/>
        <source>This shows the flags the user has on the server, if any:</source>
        <translation>Mutatja a felhasználó jelzéseit a szerveren, ha vannak:</translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="609"/>
        <source>On your friend list</source>
        <translation>Az ön barátainak a listáján</translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="610"/>
        <source>Authenticated user</source>
        <translation>Hitelesített felhasználó</translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="611"/>
        <source>Muted (manually muted by self)</source>
        <translation>Némítva (némítva saját kezűleg)</translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="612"/>
        <source>Muted (manually muted by admin)</source>
        <translation>Némítva (némítva az admin által)</translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="613"/>
        <source>Muted (not allowed to speak in current channel)</source>
        <translation>Némítva (jelenlegi csatornában nincs megengedve, hogy beszéljen)</translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="614"/>
        <source>Muted (muted by you, only on your machine)</source>
        <translation>Némítva (némítva saját kezűleg, csak a saját gépen)</translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="616"/>
        <source>Deafened (by self)</source>
        <translation>Hangszórók ki (saját maga)</translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="617"/>
        <source>Deafened (by admin)</source>
        <translation>Hangszórók ki (admin által)</translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="618"/>
        <source>User has a new comment set (click to show)</source>
        <translation>A felhasználónak új megjegyzése van (megnézéshez kattintson rá)</translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="619"/>
        <source>User has a comment set, which you&apos;ve already seen. (click to show)</source>
        <translation>A felhasználónak megjegyzését már látta. (megnézéshez kattintson rá)</translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="626"/>
        <source>This shows the flags the channel has, if any:</source>
        <translation>Mutatja a csatorna jelzéseit, ha vannak:</translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="627"/>
        <source>Channel has a new comment set (click to show)</source>
        <translation>A csatornának új megjegyzése van (megnézéshez kattintson rá)</translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="628"/>
        <source>Channel has a comment set, which you&apos;ve already seen. (click to show)</source>
        <translation>A csatorna megjegyzését már látta (megnézéshez kattintson rá)</translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="646"/>
        <source>Name</source>
        <translation>Név</translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="648"/>
        <source>Flags</source>
        <translation>Jelzések</translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="1376"/>
        <source>Are you sure you want to drag this channel?</source>
        <translation>Biztos benne, hogy ebbe a csatornába szeretné húzni?</translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="1466"/>
        <location filename="UserModel.cpp" line="1486"/>
        <source>Cannot perform this movement automatically, please reset the numeric sorting indicators or adjust it manually.</source>
        <translation>Ezt a mozgatást nem tudom automatikusan végrehajtani, kérem a sorszámozást hozza alaphelyzetbe vagy állítsa be kézileg.</translation>
    </message>
</context>
<context>
    <name>VersionCheck</name>
    <message>
        <location filename="VersionCheck.cpp" line="97"/>
        <source>Mumble failed to retrieve version information from the SourceForge server.</source>
        <translation>A Mumble nem tudta letölteni a verzióra vonatkozó információkat a SourceForge szerverről.</translation>
    </message>
</context>
<context>
    <name>ViewCert</name>
    <message>
        <location filename="ViewCert.cpp" line="36"/>
        <source>Certificate Chain Details</source>
        <translation>Tanúsítványlánc részletei</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="42"/>
        <source>Certificate chain</source>
        <translation>Tanúsítványlánc</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="48"/>
        <source>%1 %2</source>
        <translation>%1 %2</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="51"/>
        <source>Certificate details</source>
        <translation>Tanúsítvány részletei</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="79"/>
        <location filename="ViewCert.cpp" line="107"/>
        <source>Common Name: %1</source>
        <translation>Közös név: %1</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="80"/>
        <location filename="ViewCert.cpp" line="108"/>
        <source>Organization: %1</source>
        <translation>Szervezet: %1</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="81"/>
        <source>Subunit: %1</source>
        <translation>Alegység: %1</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="82"/>
        <location filename="ViewCert.cpp" line="110"/>
        <source>Country: %1</source>
        <translation>Ország: %1</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="83"/>
        <location filename="ViewCert.cpp" line="111"/>
        <source>Locality: %1</source>
        <translation>Helység: %1</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="84"/>
        <location filename="ViewCert.cpp" line="112"/>
        <source>State: %1</source>
        <translation>Állam: %1</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="85"/>
        <source>Valid from: %1</source>
        <translation>Érvényesség kezdete: %1</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="86"/>
        <source>Valid to: %1</source>
        <translation>Érvényesség vége: %1</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="87"/>
        <source>Serial: %1</source>
        <translation>Sorszám: %1</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="88"/>
        <source>Public Key: %1 bits %2</source>
        <translation>Nyilvános kulcs: %1 bit %2</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="88"/>
        <source>RSA</source>
        <translation>RSA</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="88"/>
        <source>DSA</source>
        <translation>DSA</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="89"/>
        <source>Digest (SHA-1): %1</source>
        <translation>kivonat (MD%): %1</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="93"/>
        <source>Email: %1</source>
        <translation>Email: %1</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="97"/>
        <source>DNS: %1</source>
        <translation>DNS: %1</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="106"/>
        <source>Issued by:</source>
        <translation>Kibocsájtó:</translation>
    </message>
    <message>
        <location filename="ViewCert.cpp" line="109"/>
        <source>Unit Name: %1</source>
        <translation>Egység neve: %1</translation>
    </message>
</context>
<context>
    <name>WASAPISystem</name>
    <message>
        <location filename="WASAPI.cpp" line="172"/>
        <source>Default Device</source>
        <translation>Alapértelmezett eszköz</translation>
    </message>
</context>
</TS>
