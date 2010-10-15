<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.0" language="cs">
<context>
    <name>ACLEditor</name>
    <message>
        <location filename="ACLEditor.cpp" line="+47"/>
        <source>Mumble - Edit ACL for %1</source>
        <translation>Mumble - upravit ACL pro %1</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui"/>
        <source>Active ACLs</source>
        <translation>Aktivní pravidla ACL</translation>
    </message>
    <message>
        <location/>
        <source>Context</source>
        <translation>Kontext</translation>
    </message>
    <message>
        <location/>
        <source>User/Group</source>
        <translation>Uživatel/skupina</translation>
    </message>
    <message>
        <location/>
        <source>Permissions</source>
        <translation>Oprávnění</translation>
    </message>
    <message>
        <location/>
        <source>Group</source>
        <translation>Skupina</translation>
    </message>
    <message>
        <location/>
        <source>Members</source>
        <translation>Členové</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Add</source>
        <translation>Přid&amp;at</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Remove</source>
        <translation>Odst&amp;ranit</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Up</source>
        <translation>Nahor&amp;u</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Down</source>
        <translation>&amp;Ðolů</translation>
    </message>
    <message>
        <location/>
        <source>Inherit ACLs</source>
        <translation>Zdědit ACL</translation>
    </message>
    <message>
        <location/>
        <source>Applies to this channel</source>
        <translation>Použít pro tento kanál</translation>
    </message>
    <message>
        <location/>
        <source>Applies to sub-channels</source>
        <translation>Použít pro kanály umístěné v tomto kanále</translation>
    </message>
    <message>
        <location/>
        <source>User ID</source>
        <translation>Uživatelovo ID</translation>
    </message>
    <message>
        <location filename="ACLEditor.cpp" line="+4"/>
        <source>Deny</source>
        <translation>Zakázat</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Allow</source>
        <translation>Povolit</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui"/>
        <source>Remove</source>
        <translation>Odstranit</translation>
    </message>
    <message>
        <location/>
        <source>Inherit</source>
        <translation>Zdědit</translation>
    </message>
    <message>
        <location/>
        <source>Inheritable</source>
        <translation>Děditelné</translation>
    </message>
    <message>
        <location/>
        <source>Inherited</source>
        <translation>Zděděno</translation>
    </message>
    <message>
        <location/>
        <source>Add</source>
        <translation>Přidat</translation>
    </message>
    <message>
        <location/>
        <source>Add to Remove</source>
        <translation>Přidat k odstranění</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Groups</source>
        <translation>S&amp;kupiny</translation>
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
        <location/>
        <source>List of entries</source>
        <translation>Seznam položek</translation>
    </message>
    <message>
        <location/>
        <source>Inherit ACL of parent?</source>
        <translation>Zdědit ACL mateřského kanálu?</translation>
    </message>
    <message>
        <location/>
        <source>This sets whether or not the ACL up the chain of parent channels are applied to this object. Only those entries that are marked in the parent as &quot;Apply to sub-channels&quot; will be inherited.</source>
        <translation>Nastavuje, jestli ACL nadřazeného kanálu bude aplikováno na tento objekt. Zděděny mohou být pouze položky označené v nadřazeném kanálu jako &quot;aplikovat na podkanály&quot;.</translation>
    </message>
    <message>
        <location/>
        <source>Add new entry</source>
        <translation>Přidat záznam</translation>
    </message>
    <message>
        <location/>
        <source>This adds a new entry, initially set with no permissions and applying to all.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Remove entry</source>
        <translation>Odstranit záznam</translation>
    </message>
    <message>
        <location/>
        <source>This removes the currently selected entry.</source>
        <translation>Odstraní vybraný záznam.</translation>
    </message>
    <message>
        <location/>
        <source>Move entry up</source>
        <translation>Přesunout záznam nahoru</translation>
    </message>
    <message>
        <location/>
        <source>This moves the entry up in the list. As entries are evaluated in order, this may change the effective permissions of users. You cannot move an entry above an inherited entry, if you really need that you&apos;ll have to duplicate the inherited entry.</source>
        <translation>Přesune záznam nahoru v seznamu. Protože záznamy pravidel jsou vyhodnocovány postupně, to může změnit privilegia přidělená uživateli. Nemůžete přesunout záznam nad zděděný záznam -- pokud je to opravdu potřeba pak musíte duplikovat zděděný záznam.</translation>
    </message>
    <message>
        <location/>
        <source>Move entry down</source>
        <translation>Přesunout záznam dolů</translation>
    </message>
    <message>
        <location/>
        <source>This moves the entry down in the list. As entries are evaluated in order, this may change the effective permissions of users.</source>
        <translation>Přesune záznam dolů v seznamu. Protože záznamy pravidel jsou vyhodnocovány postupně, může to změnit privilegia přidělená uživateli.</translation>
    </message>
    <message>
        <location/>
        <source>Entry should apply to this channel.</source>
        <oldsource>Entry should apply to this channel</oldsource>
        <translation type="unfinished">Použít pro tento kanál</translation>
    </message>
    <message>
        <location/>
        <source>This makes the entry apply to this channel.</source>
        <translation type="unfinished">Tímto se daná položka stane platnou pro tento kanál.</translation>
    </message>
    <message>
        <location/>
        <source>Entry should apply to sub-channels.</source>
        <translation>Použít v podkanálech.</translation>
    </message>
    <message>
        <location/>
        <source>This makes the entry apply to sub-channels of this channel.</source>
        <translation>Tímto se daná položka stane platnou pro podkanály tohoto kanálu.</translation>
    </message>
    <message>
        <location/>
        <source>Group this entry applies to</source>
        <translation></translation>
    </message>
    <message>
        <location/>
        <source>This controls which group of users this entry applies to.&lt;br /&gt;Note that the group is evaluated in the context of the channel the entry is used in. For example, the default ACL on the Root channel gives &lt;i&gt;Write&lt;/i&gt; permission to the &lt;i&gt;admin&lt;/i&gt; group. This entry, if inherited by a channel, will give a user write privileges if he belongs to the &lt;i&gt;admin&lt;/i&gt; group in that channel, even if he doesn&apos;t belong to the &lt;i&gt;admin&lt;/i&gt; group in the channel where the ACL originated.&lt;br /&gt;If a group name starts with a !, its membership is negated, and if it starts with a ~, it is evaluated in the channel the ACL was defined in, rather than the channel the ACL is active in. Order is important; &lt;i&gt;!~in&lt;/i&gt; is valid, but &lt;i&gt;~!in&lt;/i&gt; is not.&lt;br /&gt;A few special predefined groups are:&lt;br /&gt;&lt;b&gt;all&lt;/b&gt; - Everyone will match.&lt;br /&gt;&lt;b&gt;auth&lt;/b&gt; - All authenticated users will match.&lt;br /&gt;&lt;b&gt;sub,a,b,c&lt;/b&gt; - User currently in a sub-channel minimum &lt;i&gt;a&lt;/i&gt; common parents, and between &lt;i&gt;b&lt;/i&gt; and &lt;i&gt;c&lt;/i&gt; channels down the chain. See the website for more extensive documentation on this one.&lt;br /&gt;&lt;b&gt;in&lt;/b&gt; - Users currently in the channel will match (convenience for &lt;i&gt;sub,0,0,0&lt;/i&gt;).&lt;br /&gt;&lt;b&gt;out&lt;/b&gt; - Users outside the channel will match (convenience for &lt;i&gt;!sub,0,0,0&lt;/i&gt;).&lt;br /&gt;Note that an entry applies to either a user or a group, not both.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>User this entry applies to</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This controls which user this entry applies to. Just type in the user name and hit enter to query the server for a match.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ACLEditor.cpp" line="+102"/>
        <source>Allow %1</source>
        <translation>Povolit %1</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Deny %1</source>
        <translation>Odepřít %1</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This grants the %1 privilege. If a privilege is both allowed and denied, it is denied.&lt;br /&gt;%2</source>
        <translation>Tímto se povolí privilegium %1. Pokud je privilegium zároveň přiděleno i odepřeno, považuje se za odepřené.&lt;br /&gt;%2</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This revokes the %1 privilege. If a privilege is both allowed and denied, it is denied.&lt;br /&gt;%2</source>
        <translation>Odebere privilegium %1. Pokud je privilegium  zároveň přiděleno i odepřeno, považuje se za odepřené.&lt;br /&gt;%2</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui"/>
        <source>List of groups</source>
        <translation>Seznam skupin</translation>
    </message>
    <message>
        <location/>
        <source>This is all the groups currently defined for the channel. To create a new group, just type in the name and press enter.</source>
        <translation>Toto je seznam skupin definovaných pro tento kanál. Pro vytvoření nové skupiny jen napište její jméno a zmáčkněte klávesu Enter.</translation>
    </message>
    <message>
        <location/>
        <source>Remove selected group</source>
        <translation>Odstranit vybranou skupinu</translation>
    </message>
    <message>
        <location/>
        <source>This removes the currently selected group. If the group was inherited, it will not be removed from the list, but all local information about the group will be cleared.</source>
        <translation>Odstraní vybranou skupinu. Pokud byla skupina zděděna, nebude odstraněna ze seznamu, ale všechny místní informace o skupině budou smazány. </translation>
    </message>
    <message>
        <location/>
        <source>Inherit group members from parent</source>
        <translation>Zdědit členy skupiny od rodiče</translation>
    </message>
    <message>
        <location/>
        <source>This inherits all the members in the group from the parent, if the group is marked as &lt;i&gt;Inheritable&lt;/i&gt; in the parent channel.</source>
        <translation>Tímto se zdědí všichni členové skupiny z rodiče, pokud je skupina označena jako &lt;i&gt;děditelná&lt;/i&gt; v nadřazeném kanálu.</translation>
    </message>
    <message>
        <location/>
        <source>Make group inheritable to sub-channels</source>
        <translation>Učiní skupiny děditelnými pro podkanály</translation>
    </message>
    <message>
        <location/>
        <source>This makes this group inheritable to sub-channels. If the group is non-inheritable, sub-channels are still free to create a new group with the same name.</source>
        <translation>Učiní tuto skupinu děditelnou do podkanálů. Pokud je skupina neděditelná, podkanály mohou zvovy vytvořit stejně pojmenovanou skupinu.</translation>
    </message>
    <message>
        <location/>
        <source>Group was inherited from parent channel</source>
        <translation>Skupina byla zděděna od mateřského kanálu</translation>
    </message>
    <message>
        <location/>
        <source>This indicates that the group was inherited from the parent channel. You cannot edit this flag, it&apos;s just for information.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Add member to group</source>
        <translation>Přidat člena do skupiny</translation>
    </message>
    <message>
        <source>Type in the name of a player you wish to add to the group and press enter.</source>
        <translation type="obsolete">Napište jméno uživatele, kterého chcete přidat do skupiny a zmáčkněte enter.</translation>
    </message>
    <message>
        <location/>
        <source>Remove member from group</source>
        <translation>Odebrat uživatele ze skupiny</translation>
    </message>
    <message>
        <source>Type in the name of a player you wish to remove from the group and press enter.</source>
        <translation type="obsolete">Napište jméno uživatele, kterého chcete odstranit ze skupiny a zmáčkněte enter.</translation>
    </message>
    <message>
        <location/>
        <source>Dialog</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Type in the name of a player you wish to add to the group and click Add.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Type in the name of a player you wish to remove from the group and click Add.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This shows all the entries active on this channel. Entries inherited from parent channels will be show in italics.&lt;br /&gt;ACLs are evaluated top to bottom, meaning priority increases as you move down the list.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ALSAAudioInput</name>
    <message>
        <location filename="ALSAAudio.cpp" line="+360"/>
        <source>Opening chosen ALSA Input failed: %1</source>
        <translation>Otevírání zvoleného zvukového vstupu selhalo. Nebude tedy snímán zvuk ze žádného mikrofonu,</translation>
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
        <translation>Otevírání zvukového výstupu selhalo: %1</translation>
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
        <location filename="ASIOInput.ui"/>
        <source>Device selection</source>
        <translation type="unfinished">Výběr zařízení</translation>
    </message>
    <message>
        <location/>
        <source>Capabilities</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Channels</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Configure input channels</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Device to use for microphone</source>
        <translation type="unfinished">Zařízení, na kterém je mikrofon</translation>
    </message>
    <message>
        <location/>
        <source>This chooses what device to query. You still need to actually query the device and select which channels to use.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Device</source>
        <translation type="unfinished">Zařízení</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Query</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Query selected device</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This queries the selected device for channels. Be aware that many ASIO drivers are buggy to the extreme, and querying them might cause a crash of either the application or the system.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&amp;Configure</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Configure selected device</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This configures the selected device. Be aware that many ASIO drivers are buggy to the extreme, and querying them might cause a crash of either the application or the system.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Driver name</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Microphone</source>
        <translation>Mikrofon</translation>
    </message>
    <message>
        <location/>
        <source>&lt;-</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>-&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Unused</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Speakers</source>
        <translation>Bedničky</translation>
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
        <source>Mumble</source>
        <translation type="obsolete">Mumble</translation>
    </message>
    <message>
        <location line="+43"/>
        <location line="+25"/>
        <source>ASIO Initialization failed: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-21"/>
        <location line="+25"/>
        <source>Failed to instantiate ASIO driver</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+37"/>
        <source>ASIO</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ASIOInput.ui"/>
        <source>Form</source>
        <translation type="unfinished"></translation>
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
        <source>Mumble</source>
        <translation type="obsolete">Mumble</translation>
    </message>
    <message>
        <location filename="ASIOInput.cpp" line="+80"/>
        <source>You need to select at least one microphone and one speaker source to use ASIO. If you just need microphone sampling, use DirectSound.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+103"/>
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
        <location filename="About.cpp" line="+63"/>
        <source>&amp;About Mumble</source>
        <translation>O &amp;aplikaci Mumble</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>&amp;License</source>
        <translation>&amp;Licenční ujednání</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>OK</source>
        <translation>OK</translation>
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
        <source>&lt;h3&gt;About Speex&lt;/h3&gt;&lt;p&gt;&lt;tt&gt;http://www.speex.org/&lt;/tt&gt;&lt;/p&gt;&lt;p&gt;This program uses Speex version %1&lt;/p&gt;&lt;p&gt;Speex is used for echo cancellation, noise&lt;br /&gt;filtering, voice activity detection and speech&lt;br /&gt;compression.&lt;/p&gt;</source>
        <translation type="obsolete">&lt;h3&gt;O kodeku Speex&lt;/h3&gt;&lt;p&gt;&lt;tt&gt;http://www.speex.org/&lt;/tt&gt;&lt;/p&gt;&lt;p&gt;Tento program používá Speex verze %1&lt;/p&gt;&lt;p&gt;Speex je použit pro omezení ozvěny, pro redukci šumu, &lt;br /&gt;pro detekování hlasové aktivity a pro kompresi řeči&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+37"/>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <location line="-13"/>
        <source>About Speex</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>&lt;h3&gt;About Speex&lt;/h3&gt;&lt;p&gt;&lt;tt&gt;&lt;a href=&quot;%2&quot;&gt;%2&lt;/a&gt;&lt;/tt&gt;&lt;/p&gt;&lt;p&gt;This program uses Speex version %1&lt;/p&gt;&lt;p&gt;Speex is used for echo cancellation, noise&lt;br /&gt;filtering, voice activity detection and speech&lt;br /&gt;compression.&lt;/p&gt;</source>
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
        <translation type="unfinished">Systém</translation>
    </message>
    <message>
        <location/>
        <source>Input method for audio</source>
        <translation type="unfinished">Vstupní metoda pro audio</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is the input method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Device</source>
        <translation type="unfinished">Zařízení</translation>
    </message>
    <message>
        <location/>
        <source>Input device for audio</source>
        <oldsource>Output method for audio</oldsource>
        <translation type="unfinished">Výstupní metoda pro zvuk</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is the input device to use for audio.&lt;/b&gt;</source>
        <oldsource>&lt;b&gt;This is the output method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</oldsource>
        <translation type="unfinished">&lt;b&gt;To je metoda pro přehrávání zvuku.&lt;/b&gt;&lt;br /&gt; Pravděpodobně budete chtít použít ALSA nebo DirectSound.</translation>
    </message>
    <message>
        <location/>
        <source>Cancel Echo</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Transmission</source>
        <translation type="unfinished">Přenos</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Transmit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>When to transmit your speech</source>
        <translation type="unfinished">Kdy vysílat Vaši řeč</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets when speech should be transmitted.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;Continuous&lt;/i&gt; - All the time&lt;br /&gt;&lt;i&gt;Voice Activity&lt;/i&gt; - When you are speaking clearly.&lt;br /&gt;&lt;i&gt;Push To Talk&lt;/i&gt; - When you hold down the hotkey set under &lt;i&gt;Shortcuts&lt;/i&gt;.</source>
        <translation type="unfinished">Nastaví, kdy bude Vaše řeč přenášena do internetu.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;Stále&lt;/i&gt; - neustálé vysílání&lt;br /&gt;&lt;i&gt;Při hlasové aktivitě&lt;/i&gt; - když je detekována hlasová aktivita .&lt;br /&gt;&lt;i&gt;Mluvení na stisk tlačítka&lt;/i&gt; - mluvení je přenášeno, pouze když držíte stisknutou nadefinovanou klávesu pro mluvení.</translation>
    </message>
    <message>
        <location/>
        <source>DoublePush Time</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>TextLabel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Audible audio cue when push-to-talk pressed and released</source>
        <translation type="obsolete">Slyšitelný zvuk signalizující každý začátek a konec vysílání</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This enables the audio cues for push to talk.&lt;/b&gt;&lt;br /&gt;Setting this will give you a short audio beep when push to talk is pressed and released.</source>
        <translation type="unfinished">&lt;b&gt;Zahraje krátký signál, když někdo začíná a končí vysílání.&lt;/b&gt;</translation>
    </message>
    <message>
        <location/>
        <source>PTT Audio cue</source>
        <translation type="unfinished">Zvukový signál na počátku a konci vysílání</translation>
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
        <source>Use Amplitude based speech detection.</source>
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
        <translation type="unfinished">Jak dlouho se ještě vysílá po ukončení hlasové aktivity</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This selects how long after a perceived stop in speech transmission should continue.&lt;/b&gt;&lt;br /&gt;Set this higher if your voice breaks up when you speak (seen by a rapidly blinking voice icon next to your name).&lt;br /&gt;Only makes sense when used with Voice Activity transmission.</source>
        <translation type="unfinished">&lt;b&gt;Nastaví, jak dlouho po ukončení hlasových aktivit zůstane vysílání aktivní.&lt;/b&gt;&lt;br /&gt; Nastavte na vyšší hodnotu, pokud je vaše vysílání přerušováno mezi větami (což je vidět i vizuálně na ikonce vedle vašeho jména).&lt;br /&gt;Toto nastavení má smysl pouze pokud je použito vysílání při hlasové aktivitě.</translation>
    </message>
    <message>
        <location/>
        <source>Silence Below</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Signal values below this counts as silence</source>
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
        <translation type="unfinished">Komprese</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Quality</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Quality of compression (peak bandwidth)</source>
        <translation type="unfinished">Kvalita komprese (maximální šířka pásma)</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the quality of compression.&lt;/b&gt;&lt;br /&gt;This determines how much bandwidth Mumble is allowed to use for outgoing audio.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Complexity of compression (CPU)</source>
        <translation type="obsolete">Síla komprese (zvedne zatížení CPU)</translation>
    </message>
    <message>
        <location/>
        <source>Audio per packet</source>
        <translation type="unfinished">Audio/blok</translation>
    </message>
    <message>
        <location/>
        <source>How many audio frames to send per packet</source>
        <translation type="unfinished">Množství úseků zvuku odeslaných v jednom bloku</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This selects how many audio frames should be put in one packet.&lt;/b&gt;&lt;br /&gt;Increasing this will increase the latency of your voice, but will also reduce bandwidth requirements.</source>
        <translation type="unfinished">&lt;b&gt;Vybere kolik úseků zvuku bude zasíláno v jednom bloku dat.&lt;/b&gt;&lt;br /&gt;Zvětšením této hodnoty se zvětší zpoždění v komunikaci, ale také bude efektivnější využití šířky pásma.</translation>
    </message>
    <message>
        <source>Maximum bandwidth used for sent audio</source>
        <translation type="obsolete">Maximální šířka pásma pro zasílané audio</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This shows peak outgoing bandwidth used.&lt;/b&gt;&lt;br /&gt;This shows the peak amount of bandwidth sent out from your machine. Audio bitrate is the maximum bitrate (as we use VBR) for the audio data alone. Position is the bitrate used for positional information. Overhead is our framing and the IP packet headers (IP and UDP is 75% of this overhead).</source>
        <translation type="unfinished"></translation>
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
        <source>Amplification</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Maximum amplification of input sound</source>
        <translation type="unfinished">Maximální zesílení vstupního audio signálu</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Maximum amplification of input.&lt;/b&gt;&lt;br /&gt;Mumble normalizes the input volume before compressing, and this sets how much it&apos;s allowed to amplify.&lt;br /&gt;The actual level is continually updated based on your current speech pattern, but it will never go above the level specified here.&lt;br /&gt;If the &lt;i&gt;Microphone loudness&lt;/i&gt; level of the audio statistics hover around 100%, you probably want to set this to 2.0 or so, but if, like most people, you are unable to reach 100%, set this to something much higher.&lt;br /&gt;Ideally, set it so &lt;i&gt;Microphone Loudness * Amplification Factor &gt;= 100&lt;/i&gt;, even when you&apos;re speaking really soft.&lt;br /&gt;&lt;br /&gt;Note that there is no harm in setting this to maximum, but Mumble will start picking up other conversations if you leave it to auto-tune to that level.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Current speech detection chance</source>
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
        <source>Maximum bandwidth used for sending audio</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the amount of noise suppression to apply.&lt;/b&gt;&lt;br /&gt;The higher this value, the more aggressively stationary noise will be supressed.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>AudioInputDialog</name>
    <message>
        <location filename="AudioConfigDialog.cpp" line="+74"/>
        <source>Continuous</source>
        <translation type="unfinished">Stále</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Voice Activity</source>
        <translation type="unfinished">Při hlasové aktivitě</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Push To Talk</source>
        <translation type="unfinished">Mluvení pouze na stisk tlačítka</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>Audio Input</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+79"/>
        <location line="+8"/>
        <source>%1 ms</source>
        <translation type="unfinished">%1 ms</translation>
    </message>
    <message>
        <location line="-2"/>
        <location line="+30"/>
        <source>Off</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-22"/>
        <source>%1 s</source>
        <translation type="unfinished">%1 s</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>-%1 dB</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+58"/>
        <source>%1kbit/s (Audio %2, Position %4, Overhead %3)</source>
        <translation type="unfinished">%1kbit/s (Audio %2, Pozice %4, režie %3)</translation>
    </message>
    <message>
        <location line="-47"/>
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
        <translation type="unfinished">Systém</translation>
    </message>
    <message>
        <location/>
        <source>Output method for audio</source>
        <oldsource>Input method for audio</oldsource>
        <translation type="unfinished">Vstupní metoda pro audio</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is the output method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <oldsource>&lt;b&gt;This is the input method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</oldsource>
        <translation type="unfinished">&lt;b&gt;To je metoda pro přehrávání zvuku.&lt;/b&gt;&lt;br /&gt; Pravděpodobně budete chtít použít ALSA nebo DirectSound.</translation>
    </message>
    <message>
        <location/>
        <source>Device</source>
        <translation type="unfinished">Zařízení</translation>
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
        <translation type="unfinished"></translation>
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
        <source>TextLabel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Volume</source>
        <translation type="unfinished">Hlasitost</translation>
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
        <translation type="unfinished">Výstupní opoždění</translation>
    </message>
    <message>
        <location/>
        <source>Amount of data to buffer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This sets the amount of data to prebuffer in the output buffer. Experiment with different values and set it to the lowest which doesn&apos;t cause rapid jitter in the sound.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This sets the minimum distance for sound calculations. The volume of other players&apos; speech will not decrease until they are at least this far away from you.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Factor for sound volume decrease</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Bloom</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Loopback Test</source>
        <translation type="unfinished">Testování místním propojením do smyčky</translation>
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
        <source>&lt;b&gt;This sets the packet latency variance for loopback testing.&lt;/b&gt;&lt;br /&gt;Most audio paths contain some variable latency. This allows you set that variance for loopback mode testing. For example, if you set this to 15ms, this will emulate a network with 20-35ms ping latency or one with 80-95ms latency. Most domestic net connections have a variance of about 5ms</source>
        <oldsource>&lt;b&gt;This sets the packet latency variance for loopback testing&lt;/b&gt;&lt;br /&gt;Most audio paths contain some variable latency. This allows you set that variance for loopback mode testing. For example, if you set this to 15ms, this will emulate a network with 20-35ms ping latency or one with 80-95ms latency. Most domestic net connections have a variance of about 5ms</oldsource>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Packet Loss</source>
        <translation type="unfinished">Ztráta paketů</translation>
    </message>
    <message>
        <location/>
        <source>Packet loss for loopback mode</source>
        <translation type="unfinished">Ztráta paketů v režimu místní smyčky</translation>
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
        <translation type="unfinished">Vyžadovaný režim smyčky</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This enables one of the loopback testmodes.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;None&lt;/i&gt; - Loopback disabled&lt;br /&gt;&lt;i&gt;Local&lt;/i&gt; - Emulate a local server.&lt;br /&gt;&lt;i&gt;Server&lt;/i&gt; - Request loopback from server.&lt;br /&gt;Please note than when loopback is enabled, no other players will hear your voice. This setting is not saved on application exit.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Factor for sound volume increase</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>The connected &quot;speakers&quot; are actually headphones.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Minimum Distance</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Minimum distance to player before sound volume decreases</source>
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
        <source>This sets the maximum distance for sound calculations. When farther away than this, other players&apos; speech volume will not decrease any further.</source>
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
        <source>Other Applications</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Volume of other applications during speech</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Decrease volume of other applications during speech.&lt;/b&gt;&lt;br /&gt;Mumble supports decreasing the volume of other applications during incoming speech. This sets the relative volume of other applications when others are talking.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>AudioOutputDialog</name>
    <message>
        <location filename="AudioConfigDialog.cpp" line="+119"/>
        <source>None</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Local</source>
        <translation type="unfinished">Místní</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Server</source>
        <translation type="unfinished">Server</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Audio Output</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+93"/>
        <location line="+19"/>
        <source>%1 ms</source>
        <translation type="unfinished">%1 ms</translation>
    </message>
    <message>
        <location line="-8"/>
        <location line="+4"/>
        <location line="+8"/>
        <location line="+31"/>
        <location line="+4"/>
        <source>%1%</source>
        <translation type="unfinished">%1%</translation>
    </message>
    <message>
        <location line="-31"/>
        <source>%1ms</source>
        <translation type="unfinished">%1ms</translation>
    </message>
    <message>
        <location line="+15"/>
        <location line="+6"/>
        <source>%1m</source>
        <translation type="unfinished">%1% {1m?}</translation>
    </message>
</context>
<context>
    <name>AudioStats</name>
    <message>
        <source>Mumble</source>
        <translation type="obsolete">Mumble</translation>
    </message>
    <message>
        <location filename="AudioStats.ui"/>
        <source>Peak microphone level</source>
        <translation>Nejvyšší úroveň mikrofonu</translation>
    </message>
    <message>
        <location/>
        <source>Peak speaker level</source>
        <translation>Nejvyšší úroveň výstupu</translation>
    </message>
    <message>
        <location/>
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
        <location/>
        <source>Speech Probability</source>
        <translation>Pravděpodobnost řeči</translation>
    </message>
    <message>
        <source>Audio bitrate</source>
        <translation type="obsolete">Šířka datového pásma</translation>
    </message>
    <message>
        <location/>
        <source>Peak power in last frame</source>
        <translation>Max. výkon v posledním časovém úseku</translation>
    </message>
    <message>
        <source>This shows the peak power in the last frame (20 ms), and is the same measurement as you would usually find displayed as &quot;input power&quot;. Please disregard this and look at &lt;b&gt;Loudness&lt;/b&gt; instead, which is much more steady and disregards outliers.</source>
        <translation type="obsolete">Ukazuje maximální výkon v poslením úseku zvuku (20 ms). Této hodnoty si nemusíte všímat, pravděpodobně bude lepší sledovat &lt;b&gt;Hlasitost&lt;/b&gt;, jejíž hodnota se nemění tak rychle.</translation>
    </message>
    <message>
        <location/>
        <source>This shows the peak power of the speakers in the last frame (20 ms). Unless you are using a multi-channel sampling method (such as ASIO) with speaker channels configured, this will be 0. If you have such a setup configured, and this still shows 0 while you&apos;re playing audio from other programs, your setup is not working.</source>
        <oldsource>This shows the peak power in the last frame (20 ms) of the speakers. Unless you are using a multi-channel sampling method (such as ASIO) with speaker channels configured, this will be 0. If you have such a setup configured, and this still shows 0 while you&apos;re playing audio from other programs, your setup is not working.</oldsource>
        <translation type="unfinished">Ukazuje maximální výkon zvukového výstupu za poslední časový úsek (20 ms). Pokud nepoužíváte vícekanálovou samplovací metodu (jako např. ASIO) s nastavenými kanály pro výstup, pak tato hodnota bude rovna 0. Pokud máte takové nastavení a stále vidíte 0 zatímco jiné aplikace hrají zvuk, Vaše nastavení asi není v pořádku.</translation>
    </message>
    <message>
        <source>This shows the peak power in the last frame (20 ms) after all processing. Ideally, this should be -96 dB when you&apos;re not talking. In reality, a sound studio should see -60 dB, and you should hopefully see somewhere around -20 dB. When you are talking, this should rise to somewhere between -5 and -10 dB.&lt;br /&gt;If you are using echo cancellation, and this rises to more than -15 dB when you&apos;re not talking, your setup is not working, and you&apos;ll annoy other players with echoes.</source>
        <translation type="obsolete">Ukazuje hodnotu maximálního výkonu za poslední časový úsek (20ms) po veškerém zpracování. Ideálně tato hodnota bude -96 dB když nemluvíte. V realném světě tato hodnota může být např. -60 dB ve zvukovém studiu jinak běžně asi 20 dB. Pokud mluvíte, tato hodnota stoupne na asi -10 až -5 dB.&lt;br /&gt; Pokud používáte používáte potlačení ozvěny a tato hodnota stoupne na více než -15 dB když nemluvíte, pak Vaše nastavení nefunguje správně a dozajista budete obtěžovat ostatní uživatele ozvěnou.</translation>
    </message>
    <message>
        <location/>
        <source>How close the current input level is to ideal</source>
        <translation>Jak blízko je současná hlasitost vstupu k ideální hlasitosti</translation>
    </message>
    <message>
        <source>This shows how close your current input volume is to the ideal. To adjust your microphone level, open whatever program you use to adjust the recording volume, and look at the value here while talking.&lt;br /&gt;&lt;b&gt;Talk loud, as you would when you&apos;re upset over getting fragged by a noob.&lt;/b&gt;&lt;br /&gt;Adjust the volume until this value is close to 100%, but make sure it doesn&apos;t go above. If it does go above, you are likely to get clipping in parts of your speech, which will degrade sound quality</source>
        <translation type="obsolete">Ukazuje, jak blízko je současná úroveň hlasitosti k ideální. Pro nastavení úrovně mikrofonu, otevřete svou oblíbenou aplikaci pro nastavení zvuku, upravte úroveň záznamu a sledujte změny této hodnoty během mluvení.&lt;br /&gt;&lt;b&gt;Mluvte nahlas podobně jako při nadšení nebo rozčilení&lt;/b&gt;&lt;br /&gt; Upravte hlasitost tak, aby byla hodnota blízká 100 %, ale aby nepřesahovala. Přesahování vede k degradaci kvality zvuku (limitací) a možnému cvakání.</translation>
    </message>
    <message>
        <location/>
        <source>Signal-To-Noise ratio from the microphone</source>
        <translation>Poměr signál-šum mikrofonu (SNR)</translation>
    </message>
    <message>
        <location/>
        <source>This is the Signal-To-Noise Ratio (SNR) of the microphone in the last frame (20 ms). It shows how much clearer the voice is compared to the noise.&lt;br /&gt;If this value is below 1.0, there&apos;s more noise than voice in the signal, and so quality is reduced.&lt;br /&gt;There is no upper limit to this value, but don&apos;t expect to see much above 40-50 without a sound studio.</source>
        <translation>Poměr signál-šum mikrofonu v posledním zvukovém rámci (20 ms). Ukazuje jak je hlas silný v porovnání se šumem.&lt;br /&gt;  Pokud je tato hodnota pod 1.0, je šum silnější než hlas a kvalita nebude dobrá. &lt;br /&gt; Neexistuje žádný horní limit pro tuto hodnotu, ale neočekávejte hodnoty větší než 40 nebo 50, pokud nejste ve zvukovém studiu.</translation>
    </message>
    <message>
        <location/>
        <source>Probability of speech</source>
        <translation>Pravděpodobnost, že je mluveno</translation>
    </message>
    <message>
        <location/>
        <source>This is the probability that the last frame (20 ms) was speech and not environment noise.&lt;br /&gt;Voice activity transmission depends on this being right. The trick with this is that the middle of a sentence is always detected as speech; the problem is the pauses between words and the start of speech. It&apos;s hard to distinguish a sigh from a word starting with &apos;h&apos;.&lt;br /&gt;If this is in bold font, it means Mumble is currently transmitting (if you&apos;re connected).</source>
        <translation>Toto je pravděpodobnost, že poslední  zvukový rámec (20 ms) byl řeč, ne jenom hluk okolního prostředí.&lt;br /&gt; Přenos při hlasová aktivitě závisí na tom, aby detekce proběhla správně. Trik je v tom, že uprostřed věty je vždy snadné rozpoznat řeč, problémem jsou pauzy mezi slovy a začátek mluvení. Je obtížné rozpoznat povzdech od písmenka &apos;h&apos; na začátku slova.&lt;br /&gt; Pokud tohle je zobrazeno tučně, znamená to, že hlasová aktivita byla rozpoznána.</translation>
    </message>
    <message>
        <location/>
        <source>Bitrate of last frame</source>
        <translation>Šířka zvukového pásma </translation>
    </message>
    <message>
        <source>This is the audio bitrate of the last compressed frame (20 ms), and as such will jump up and down as the VBR adjusts the quality. To adjust the peak bitrate, adjust &lt;b&gt;Compression Complexity&lt;/b&gt; in the Settings dialog.</source>
        <translation type="obsolete">Šířka zvukového pásma pro poslední časový úsek (20 ms). Tato hodnota se prúběžně automaticky mění, aby se ideálně přizpůsobila potřebám kvality. V nastavení je možno upravit šířku pásma.</translation>
    </message>
    <message>
        <location/>
        <source>Power spectrum of input signal and noise estimate</source>
        <translation>Výkonové spektrum vstupního signálu a odhad šumu</translation>
    </message>
    <message>
        <location/>
        <source>This shows the power spectrum of the current input signal (red line) and the current noise estimate (filled blue).&lt;br /&gt;All amplitudes are multiplied by 30 to show the interesting parts (how much more signal than noise is present in each waveband).&lt;br /&gt;This is probably only of interest if you&apos;re trying to fine-tune noise conditions on your microphone. Under good conditions, there should be just a tiny flutter of blue at the bottom. If the blue is more than halfway up on the graph, you have a seriously noisy environment.</source>
        <translation>Ukazuje výkonové spektrum aktuálního vstupu (červená čára) a aktuální odhad šumu (vyplněno modře).&lt;br /&gt;Všechny amplitudy jsou vynásobeny 30x aby byly vidět zajímavé části (jak moc je signál větší než zvuk pro různé frekvence).&lt;br /&gt; To je pravděpodobně zajímavé, pokud zkoušíte vyladit šumové podmínky mikrofonu. V dobrých podmínkách bude dole pouze modrá čára. Pokud dosahuje modrá oblast do poloviny výšky grafu, znamená to, že zvuk je rušen silným šumem.</translation>
    </message>
    <message>
        <location/>
        <source>Weights of the echo canceller</source>
        <translation>Intenzita snahy o odstranění ozvěny</translation>
    </message>
    <message>
        <location/>
        <source>This shows the weights of the echo canceller, with time increasing downwards and frequency increasing to the right.&lt;br /&gt;Ideally, this should be black, indicating no echo exists at all. More commonly, you&apos;ll have one or more horizontal stripes of bluish color representing time delayed echo. You should be able to see the weights updated in real time.&lt;br /&gt;Please note that as long as you have nothing to echo off, you won&apos;t see much useful data here. Play some music and things should stabilize. &lt;br /&gt;You can choose to view the real or imaginary parts of the frequency-domain weights, or alternately the computed modulus and phase. The most useful of these will likely be modulus, which is the amplitude of the echo, and shows you how much of the outgoing signal is being removed at that time step. The other viewing modes are mostly useful to people who want to tune the echo cancellation algorithms.&lt;br /&gt;Please note: If the entire image fluctuates massively while in modulus mode, the echo canceller fails to find any correlation whatsoever between the two input sources (speakers and microphone). Either you have a very long delay on the echo, or one of the input sources is configured wrong.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
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
        <translation type="unfinished">Poměr zvuku k šumu</translation>
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
        <translation>Průvodce nastavením zvuku</translation>
    </message>
    <message>
        <location line="+50"/>
        <source>Introduction</source>
        <translation>Úvod</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Welcome to the Mumble Audio Wizard</source>
        <translation>Vítejte v průvodci pro nastavení zvuku pro aplikaci Mumble</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>This is the audio tuning wizard for Mumble. This will help you correctly set the input levels of your sound card, and also set the correct parameters for sound processing in Mumble.</source>
        <translation>Toto je průvodce nastavení zvuku pro Mumble. Pomůže vám  správně nastavit vstupní úrovně na zvukové kartě a také vybrat vhodné parametry pro zpracování zvuku v Mumble.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Please be aware that as long as this wizard is active, audio will be looped locally to allow you to listen to it, and no audio will be sent to the server.</source>
        <translation>Prosím všimněte si, že dokud je průvodce aktivní, zvuk bude používán pouze místně, abyste si jej mohli poslechnout - žádné zvuky se nepřenesou na případně připojený server.</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Finished</source>
        <translation>Dokončeno</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Enjoy using Mumble</source>
        <translation>Mumble je připraveno k použití</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Congratulations. You should now be ready to enjoy a richer sound experience with Mumble.</source>
        <translation>Blahopřejeme. Nyní si můžete plně užít zvukovou komunikaci s Mumble.</translation>
    </message>
    <message>
        <location line="+23"/>
        <source>Device selection</source>
        <translation>Výběr zařízení</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Selecting the input and output device to use with Mumble.</source>
        <translation>Vyberte vstupní a výstupní zvukové zařízení pro použití v Mumble.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Input device</source>
        <translation>Vstupní zařízení</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>This is the device your microphone is connected to.</source>
        <translation>Zařízení pro zvukový vstup (s mikrofonem).</translation>
    </message>
    <message>
        <location line="+14"/>
        <location line="+52"/>
        <source>System</source>
        <translation>Systém</translation>
    </message>
    <message>
        <location line="-49"/>
        <source>Input method for audio</source>
        <translation>Vstupní metoda pro audio</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>&lt;b&gt;This is the input method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation>&lt;b&gt;To je metoda pro zaznamenávání zvuku.&lt;/b&gt;&lt;br /&gt; Pravděpodobně budete chtít použít ALSA nebo DirectSound.</translation>
    </message>
    <message>
        <location line="+12"/>
        <location line="+52"/>
        <source>Device</source>
        <translation>Zařízení</translation>
    </message>
    <message>
        <location line="-49"/>
        <source>Input device to use</source>
        <translation>Vstupní zařízení</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>&lt;b&gt;Selects which sound card to use for audio input.&lt;/b&gt;</source>
        <translation>&lt;b&gt;Vyberte zvukovou kartu, která bude snímat zvuk.&lt;/b&gt;</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Output device</source>
        <translation>Výstupní zařízení</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>This is the device your speakers or headphones are connected to.</source>
        <translation>Zařízení, ke kterému jsou připojeny sluchátka nebo mikrofon.</translation>
    </message>
    <message>
        <location line="+19"/>
        <source>Output method for audio</source>
        <translation>Výstupní metoda pro zvuk</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>&lt;b&gt;This is the Output method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation>&lt;b&gt;To je metoda pro přehrávání zvuku.&lt;/b&gt;&lt;br /&gt; Pravděpodobně budete chtít použít ALSA nebo DirectSound.</translation>
    </message>
    <message>
        <location line="+15"/>
        <source>Output device to use</source>
        <translation>Výstupní zařízení</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>&lt;b&gt;Selects which sound card to use for audio Output.&lt;/b&gt;</source>
        <translation>&lt;b&gt;Vyberte zvukovou kartu, která bude přehrávat zvuk.&lt;/b&gt;</translation>
    </message>
    <message>
        <location line="+64"/>
        <source>Volume tuning</source>
        <translation>Vyladění hlasitosti</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Tuning microphone hardware volume to optimal settings.</source>
        <translation>Nastavení hlasitosti mikrofonu pro optimální hlasitost.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Open your sound control panel and go to the recording settings. Make sure the microphone is selected as active input with maximum recording volume. If there&apos;s an option to enable a &quot;Microphone boost&quot; make sure it&apos;s checked.</source>
        <translation>Otevřete ovládací panel nastavení zvuku a jděte do nastavení záznamu. Ujistěte se, že mikrofon je vybrán jako aktivní vstup a má maximální hlasitost pro záznam. Pokud je k dispozici volba &quot;Microphone boost&quot;, určitě ji zapněte.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Speak loudly, as when you are annoyed or excited. Decrease the volume in the sound control panel until the bar below stays as high as possible in the blue and green but &lt;b&gt;not&lt;/b&gt; the red zone while you speak.</source>
        <translation>Mluvte nahlas, jako kdybyste byl podrážděn nebo nadšen. Během mluvení nastavte hlasitost v ovladacim panelu zvuku tak, aby monitorovaný zvuk byl co možná nejvýše v modré a zelené oblastí, ale aby &lt;b&gt;nezasahoval&lt;/b&gt; do oblasti rudé.</translation>
    </message>
    <message>
        <location line="+11"/>
        <source>Now talk softly, as you would when talking late at night and you don&apos;t want to disturb anyone. Adjust the slider below so that the bar moves into green when you talk, but stays blue while you&apos;re silent.</source>
        <translation>Nyní se uvolněte a mluvte potichu, jako kdyby byla pozdní noc a nechtěli jste nikoho rušit ve spánku. Nastavte táhlo pod tímto textem tak, aby monitorovaný zvuk byl v zelené oblasti při mluvení, ale aby bezpečně zůstal v modré oblasti, když jste potichu.</translation>
    </message>
    <message>
        <location line="+19"/>
        <source>Voice Activity Detection</source>
        <translation>Detekce hlasové aktivity</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Letting Mumble figure out when you&apos;re talking and when you&apos;re silent.</source>
        <translation>Mumble umí rozpoznávat mluvení a ticho.</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>This will help Mumble figure out when you are talking. The first step is selecting which data value to use.</source>
        <translation>Toto pomůže aplikaci Mumble rozpoznat, kdy mluvíte. První krok je výběr, podle které hodnoty se má provádět rozpoznání.</translation>
    </message>
    <message>
        <location line="+26"/>
        <source>Raw amplitude from input</source>
        <translation>Hlasitost vstupu</translation>
    </message>
    <message>
        <location line="-4"/>
        <source>Signal-To-Noise ratio</source>
        <translation>Poměr zvuku k šumu</translation>
    </message>
    <message>
        <location line="+27"/>
        <source>Next you need to adjust the following two sliders. The first few utterances you say should end up in the green area (definitive speech). While talking, you should stay inside the yellow (might be speech) and when you&apos;re not talking, everything should be in the red (definitively not speech).</source>
        <translation>Nyní je potřeba nastavit následující dvě hodnoty. Prvních několik vašich slabik by mělo skončit v zelené oblasti (rozpoznáno jako řeč). Pokud mluvíte, měl by monitor zůstávat ve žluté (rozpoznáno jako pravděpopdobná řeč). Když nemluvíte, vše by mělo být v červené oblasti (to dozajista není řeč).</translation>
    </message>
    <message>
        <location line="+24"/>
        <source>Last, you need to adjust the grace time. Mumble may detect this much non-speech and still keep transmitting before it breaks off. This allows you to catch your breath while speaking.</source>
        <translation>Nakonec je potřeba nastavit čas prodlevy, kdy je po skončení řeči stále ješte mikrofon zapnut a můžete tak během řeči udělat malou pauzu, aniž by byl přenos ukončen.</translation>
    </message>
    <message>
        <location line="+28"/>
        <source>Device tuning</source>
        <translation>Vyladění zařízení</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Changing hardware output delays to their minimum value.</source>
        <translation>Změní výstupní parametry zvuku tak, aby prodleva v komunikaci mohla být minimální.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>To keep latency to an absolute minium, it&apos;s important to buffer as little audio as possible on the soundcard. However, many soundcards report that they require a much smaller buffer than what they can actually work with, so the only way to set this value is to try and fail.</source>
        <translation>Aby zpoždění při komunikaci mohlo být minimalizováno, je důležité nastavit výstupní vyrovnávací paměť co nejmenší. Mnohé zvukové karty hlásí, že neumějí používat malé velikosti vyrovnávací paměti, i když to jinak technicky je možné, takže jediná možnost, jak zjistit nejmenší vhodnou hodnotu je pokud a omyl.</translation>
    </message>
    <message>
        <source>You should hear a multitone chord. Change the slider below to the lowest value which gives &lt;b&gt;no&lt;/b&gt; interruptions or jitter in the sound.</source>
        <translation type="obsolete">Nyní byste měli slyšet zvukový akord. Změňte nastavení pod tímto textem na nejmenší hodnotu, která bude fungovat &lt;b&gt;bez&lt;/b&gt; přerušování nebo cvakání ve zvuku.</translation>
    </message>
    <message>
        <location line="+23"/>
        <source>Amount of data to buffer.</source>
        <translation>Velikost zásobníku pro výstup.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This sets the amount of data to prebuffer in the output buffer. Experiment with different values and set it to the lowest which doesn&apos;t cause rapid jitter in the sound.</source>
        <translation>To nastaví množství dat, která budou v výstupním zásobníku aplikace, než se pošlou na zvukový výstup. Experimentujte s rozdílnými hodnotami a nastavte si nejmenší hodnotu, která nezpůsobí přerušování nebo cvakání ve zvuku.</translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+24"/>
        <source>If you position the microphone so it can pick up the speakers or headset, Mumble will measure the total audio path delay in your system; this means the delay from a sample is placed in an outbound buffer until it&apos;s found in a matching incoming buffer.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+294"/>
        <source>Audio path is %1ms long.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Audio path cannot be determined. Input not recognized.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-592"/>
        <source>Use echo cancellation</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Cancel echo from headset or speakers.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This enables echo cancellation of outgoing audio, which helps both on speakers and on headsets.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+50"/>
        <source>Enable positional audio</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Allows positioning of sound.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This allows Mumble to use positional audio to place voices.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+17"/>
        <source>Positional Audio</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Adjusting attenuation of positional audio.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Mumble supports positional audio for some games, and will position the voice of other players relative to their position in game. Depending on their position, the volume of the voice will be changed between the speakers to simulate the direction and distance the other player is at. Such positioning depends on your speaker configuration being correct in your operating system, so a test is done here.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>The graph below shows the position of &lt;font color=&quot;red&quot;&gt;you&lt;/font&gt;, the &lt;font color=&quot;yellow&quot;&gt;speakers&lt;/font&gt; and a &lt;font color=&quot;green&quot;&gt;moving sound source&lt;/font&gt; as if seen from above. You should hear the audio move between the channels.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Use headphones</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Use headphones instead of speakers.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This ignores the OS speaker configuration and configures the positioning for headphones instead.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-153"/>
        <source>Mumble is under continuous development, and the development team wants to focus on the features that benefit the most users. To this end, Mumble supports submitting anonymous statistics about your configuration to the developers. These statistcs are essential for future development, and also make sure the features you use aren&apos;t deprecated.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Submit anonymous statistics to the Mumble project</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+224"/>
        <source>Push To Talk:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>BanEditor</name>
    <message>
        <location filename="BanEditor.ui"/>
        <source>Mumble - Edit Bans</source>
        <translation>Mumble - Upravit bany (zákazy)</translation>
    </message>
    <message>
        <location/>
        <source>0.0.0.0</source>
        <translation>0.0.0.0</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Add</source>
        <translation>Přid&amp;at</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Update</source>
        <translation>&amp;Obnovit</translation>
    </message>
    <message>
        <location/>
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
    <name>ChanACL</name>
    <message>
        <location filename="ACL.cpp" line="+49"/>
        <source>W</source>
        <translation>Z</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>T</source>
        <translation>P</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>E</source>
        <translation>V</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>S</source>
        <translation>M</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>A</source>
        <translation>A</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>M</source>
        <translation>U</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>K</source>
        <translation>V</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>C</source>
        <translation>T</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>L</source>
        <translation>N</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>None</source>
        <translation>Žádné</translation>
    </message>
    <message>
        <source>Write</source>
        <translation type="obsolete">Zápis</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Traverse</source>
        <translation>Průchod</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Enter</source>
        <translation>Vstup</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Speak</source>
        <translation>Mluvení</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>AltSpeak</source>
        <translation>Alt_mluvení</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Mute/Deafen</source>
        <translation>Zakázat mluvení/poslouchání</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Move/Kick</source>
        <translation>Přesunout/Vykopnout</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Make channel</source>
        <translation>Vytvoření kanálu</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Link channel</source>
        <translation>Propojení kanálů</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>This represents no privileges.</source>
        <translation>Toto představuje absenci jakýchkoliv privilegií.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>This represents total access to the channel, including the ability to change group and ACL information. This privilege implies all other privileges.</source>
        <translation>Reprezentuje totální právo ke kanálu, včetně schopnosti změnit skupiny a ACL. Mít toto privilegium automaticky znamená mít všechna ostatní práva.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>This represents the permission to traverse the channel. If a user is denied this privilege, he will be unable to access this channel and any sub-channels in any way, regardless of other permissions in the sub-channels.</source>
        <translation>Reprezentuje povolení procházet kanálem. Pokud je procházení uživateli odepřeno, nebude schopen přijít do kanálu či jeho podkanálů (i kdyby tam měl průchod povolen).</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>This represents the permission to join the channel. If you have a hierarchical channel structure, you might want to give everyone Traverse, but restrict Enter in the root of your hierarchy.</source>
        <translation>Povolení vejít do kanálu. V hierarchicky uspořádaných kanálech je často výhodné dám všem právo průchodu (traverse), ale omezit právo vstupu (enter) v kořeni hierarchie.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>This represents the permission to speak in a channel. Users without this privilege will be suppressed by the server (seen as muted), and will be unable to speak until they are unmuted by someone with the appropriate privileges.</source>
        <translation>Representuje oprávnění mluvit v kanálu. Uživatelům bez tohoto privilegia bude serverem zakázáno mluvit a nebudou mít možnost mluvit, dokud jim nějaký administrátor tuto schopnost neumožní.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>This represents the permission to speak in a channel with flagged speech. This works exactly like the &lt;i&gt;speak&lt;/i&gt; privilege, but applies to packets spoken with AltPushToTalk held down. This may be used to broadcast to a hierarchy of channels without linking.</source>
        <translation>Reprezentuje oprávnění  mluvit v kanálu &apos;alternativně&apos;. Na rozdíl od normálního mluvení se alternativní mluvení provádí stlačením tlačítka pro Alt_mluvení. To může být použito pro vysílání přes celou hierarchii kanálů aniž by bylo nutno kanály propojovat.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>This represents the permission to mute and deafen other players. Once muted, a player will stay muted until he is unmuted by another privileged player or reconnects to the server.</source>
        <translation>Reprezentuje oprávnění zakázat mluvení nebo poslouchání ostatním uživatelům. Uživatelé, kterým bylo zakázáno mluvit nebo poslouchat, nedostanou zpět tuto schopnost, pokud jim opět nebude vrácena zpět privilegovaným uživatelem, nebo dokud se neodpojí a znova nepřipojí k serveru.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>This represents the permission to move a player to another channel or kick him from the server. To actually move the player, either the moving player must have Move/Kick privileges in the destination channel, or the player must normally be allowed to enter the channel. Players with this privilege can move players into channels the target player normally wouldn&apos;t have permission to enter.</source>
        <translation>Reprezentuje oprávnění k přesunu uživatele do jiného kanálu nebo oprávnění jej zcela vykopnout ze serveru. Pro úspěšný přesun musí mít přesunující uživatel právo přesunu do cílového kanálu, nebo tam musí být povolen pro přesunovaného bežný vstup. Uživatelé s tímto oprávněním tedy mohou přesunovat do neveřejných  kanálů  uživatele, kteří by se tam jinak nedostali.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>This represents the permission to make sub-channels. The player making the sub-channel will be added to the admin group of the sub-channel.</source>
        <translation>Reprezentuje oprávnění vytvářet v kanále podkanály. Uživatel vytvářející kanál přidán do skupiny administrátorů tohoto kanálu.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>This represents the permission to link channels. Players in linked channels hear each other, as long as the speaking player has the &lt;i&gt;speak&lt;/i&gt; privilege in the channel of the listener. You need the link privilege in both channels to create a link, but just in either channel to remove it.</source>
        <translation>Reprezentuje oprávnění propojovat kanály. Uživatelé ve spojených kanálech slyší jeden druhého, pokud mluvící uživatel má oprávnění &lt;i&gt;mluvit&lt;/i&gt; v kanále, kde je posluchač. Pro vytvoření zvukového propojení je nutno mít toto oprávnění v obou kanálech, pří rušení stačí oprávnění v jednom z nich.</translation>
    </message>
    <message>
        <location line="-57"/>
        <source>Write ACL</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ClientPlayer</name>
    <message>
        <location filename="Player.cpp" line="+53"/>
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
        <translation>Konfigurace Mumble</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="+54"/>
        <source>Accept changes</source>
        <translation>Potvrdit změny</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This button will accept current settings and return to the application.&lt;br /&gt;The settings will be stored to disk when you leave the application.</source>
        <translation>Toto tlačítko potvrdí současné nastavení a navrátí se do aplikace.&lt;br /&gt;Nastavení bude uloženo na disku při ukončení aplikace.</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Zrušit</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Reject changes</source>
        <translation>Odmítnout nové změny</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This button will reject all changes and return to the application.&lt;br /&gt;The settings will be reset to the previous positions.</source>
        <translation>Tímto tlačítkem můžete odmítnout nastavované změny a vrátit se do aplikace.&lt;br /&gt;Nastavení nebude změněno.</translation>
    </message>
    <message>
        <source>&amp;Apply</source>
        <translation type="obsolete">&amp;Použít</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Apply changes</source>
        <translation>Potvrdit změny</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This button will immediately apply all changes.</source>
        <translation>Toto tlačítko okamžitě provede všechny vybrané změny.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Undo changes for current page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This button will revert any changes done on the current page to the most recent applied settings.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Restore defaults for current page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This button will restore the defaults for the settings on the current page. Other pages will not be changed.&lt;br /&gt;To restore all settings to their defaults, you will have to use this button on every page.</source>
        <oldsource>This button will restore the settings for the current page only to their defaults. Other pages will not be changed.&lt;br /&gt;To restore all settings to their defaults, you will have to use this button on every page.</oldsource>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConfigDialog.ui"/>
        <source>Advanced</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ConnectDialog</name>
    <message>
        <location filename="ConnectDialog.ui"/>
        <source>&amp;Custom Servers</source>
        <translation>&amp;Moje servery</translation>
    </message>
    <message>
        <location/>
        <source>Server &amp;Browser</source>
        <translation>Výběr &amp;veřejných serverů</translation>
    </message>
    <message>
        <source>&amp;Name</source>
        <translation type="obsolete">&amp;Jméno</translation>
    </message>
    <message>
        <location/>
        <source>A&amp;ddress</source>
        <translation>A&amp;dresa</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Port</source>
        <translation>&amp;Port</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Username</source>
        <translation>&amp;Uživatelské jméno</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Password</source>
        <translation>&amp;Heslo</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Connect</source>
        <translation>&amp;Připojit</translation>
    </message>
    <message>
        <location/>
        <source>Cancel</source>
        <translation type="unfinished">Zrušit</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Add</source>
        <translation>Přid&amp;at</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Remove</source>
        <translation>Odst&amp;ranit</translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="obsolete">Jméno</translation>
    </message>
    <message>
        <location/>
        <source>Address</source>
        <translation>Adresa</translation>
    </message>
    <message>
        <location/>
        <source>URL</source>
        <translation>URL</translation>
    </message>
    <message>
        <location/>
        <source>C&amp;opy to custom</source>
        <translation>Zk&amp;opírovat do seznamu mých serverů</translation>
    </message>
    <message>
        <location/>
        <source>&amp;View Webpage</source>
        <translation>Otevřít &amp;Webovou stránku</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="+124"/>
        <source>Connecting to %1</source>
        <translation>Připojování k %1</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Enter username</source>
        <translation>Zadejte jmého uživatele</translation>
    </message>
    <message>
        <source>Mumble</source>
        <translation type="obsolete">Mumble</translation>
    </message>
    <message>
        <location line="+99"/>
        <source>Failed to fetch server list</source>
        <translation>Selhalo stažení seznamu dostupných serverů</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui"/>
        <source>&amp;Label</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Label</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="-14"/>
        <source>Unknown</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+116"/>
        <source>-Unnamed entry-</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui"/>
        <source>Mumble Server Connect</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&amp;Cancel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="-60"/>
        <location line="+22"/>
        <source>New</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+62"/>
        <location line="+27"/>
        <source>Add</source>
        <translation type="unfinished">Přidat</translation>
    </message>
    <message>
        <location line="-2"/>
        <source>Update</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>CoreAudioSystem</name>
    <message>
        <location filename="CoreAudio.cpp" line="+83"/>
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
        <source>Mumble</source>
        <translation type="obsolete">Mumble</translation>
    </message>
    <message>
        <source>Opening chosen DirectSound Input failed. No microphone capture will be done.</source>
        <translation type="obsolete">Otevírání vybraného vstupu DirectSound selhalo. Nebude prováděn záznam mikrofonu.</translation>
    </message>
    <message>
        <location filename="DirectSound.cpp" line="+554"/>
        <source>Opening chosen DirectSound Input failed. Default device will be used.</source>
        <translation>Otevírání vybraného vstupu DirectSound  selhalo. Bude použito výchozí zařízení.</translation>
    </message>
    <message>
        <location line="-382"/>
        <source>Default DirectSound Voice Input</source>
        <translation type="unfinished">Výchozí DirectSound hlasový vstup</translation>
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
        <translation>Otevírání vybraného výstupu DirectSound selhalo. Nebude slyšet žádný zvukový výstup.</translation>
    </message>
    <message>
        <location line="-82"/>
        <source>Opening chosen DirectSound Output failed. Default device will be used.</source>
        <translation>Otevírání vybraného výstupu DirectSound  selhalo. Bude použito výchozí zařízení.</translation>
    </message>
    <message>
        <location line="-247"/>
        <source>Default DirectSound Voice Output</source>
        <translation type="unfinished">Výchozí DirectSound hlasový výstup</translation>
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
        <source>Mumble</source>
        <translation type="obsolete">Mumble</translation>
    </message>
    <message>
        <location filename="Database.cpp" line="+127"/>
        <source>Mumble failed to initialize a database in any
of the possible locations.</source>
        <translation>Mumble nedokazalo inicializovat databázi
v žádném z možných míst.</translation>
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
        <location filename="GlobalShortcut.ui"/>
        <source>Shortcuts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>List of configured shortcuts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Function</source>
        <translation type="unfinished">Funkce</translation>
    </message>
    <message>
        <location/>
        <source>Shortcut</source>
        <translation type="unfinished">Zkratka</translation>
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
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">Odst&amp;ranit</translation>
    </message>
</context>
<context>
    <name>GlobalShortcutConfig</name>
    <message>
        <location filename="GlobalShortcut.cpp" line="+228"/>
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
        <location line="+33"/>
        <source>Shortcut button combination.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>&lt;b&gt;This is the global shortcut key combination.&lt;/b&gt;&lt;br /&gt;Double-click this field and then the desired key/button combo to rebind.</source>
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
        <location filename="GlobalShortcut_macx.cpp" line="+63"/>
        <source>Mumble has detected that it is unable to receive Global Shortcut events when it is in the background.&lt;br /&gt;&lt;br /&gt;This is because the Universal Access feature called &apos;Enable access for assistive devices&apos; is currently disabled.&lt;br /&gt;&lt;br /&gt;Please &lt;a href=&quot; &quot;&gt;enable this setting&lt;/a&gt; and continue when done.</source>
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
        <location filename="GlobalShortcut_unix.cpp" line="+325"/>
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
        <location filename="LCD.cpp" line="+274"/>
        <source>Not connected</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>LCDConfig</name>
    <message>
        <location line="-161"/>
        <source>Graphic</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Character</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+11"/>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Name</source>
        <translation type="unfinished">Jméno</translation>
    </message>
    <message>
        <location/>
        <source>Type</source>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Splitter Width</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">Debug</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Critical</source>
        <translation type="unfinished">Critical</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Warning</source>
        <translation type="unfinished">Warning</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Information</source>
        <translation type="unfinished">Information</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Server Connected</source>
        <translation type="unfinished">Server připojen</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Server Disconnected</source>
        <translation type="unfinished">Server odpojen</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Player Joined Server</source>
        <translation type="unfinished">Uživatel se připojil k serveru</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Player Left Server</source>
        <translation type="unfinished">Uživatel opustil server</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Player kicked (you or by you)</source>
        <translation type="unfinished">Uživatel byl vykopnut</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Player kicked</source>
        <translation type="unfinished">Uživatel vykopnut</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>You self-muted/deafened</source>
        <translation type="unfinished">Zakázal jste si možnost hovoru/poslechu</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Other self-muted/deafened</source>
        <translation type="unfinished">Uživatel si vypnul mluvení/poslech</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Player muted (you)</source>
        <translation type="unfinished">Bylo zakázáno mluvení</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Player muted (by you)</source>
        <translation type="unfinished">Uživateli bylo Vámi zakázano mluvit</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Player muted (other)</source>
        <translation type="unfinished">Uživatelovi bylo zakázáno mluvit</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Player Joined Channel</source>
        <translation type="unfinished">Uživatel vstoupil do kanálu</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Player Left Channel</source>
        <translation type="unfinished">Uživatel opustil kanál</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Permission Denied</source>
        <translation type="unfinished">K provedení akce chybí potřebné oprávnění</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Text Message</source>
        <translation type="unfinished">Textová zpráva</translation>
    </message>
    <message>
        <source>[%2] %1</source>
        <translation type="obsolete">[%2] %1</translation>
    </message>
</context>
<context>
    <name>LogConfig</name>
    <message>
        <location line="-164"/>
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
        <source>Path to soundfile used for sound notifications in the case of %1 events&lt;br /&gt;Single click to play&lt;br /&gt;Doubleclick to change</source>
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
        <location line="+1"/>
        <source>Click here to toggle Text-To-Speech for %1 events.&lt;br /&gt;If checked, Mumble uses Text-To-Speech to read %1 events out loud to you. Text-To-Speech is also able to read the contents of the event which is not true for soundfiles. Text-To-Speech and soundfiles cannot be used at the same time.</source>
        <oldsource>Click here to toggle Text-To-Speech for %1 events.&lt;br /&gt;If checked, mumble uses Text-To-Speech to read %1 events out loud to you. Text-To-Speech is also able to read the contents of the event which is not true for soundfiles. Text-To-Speech and soundfiles cannot be used at the same time.</oldsource>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Click here to toggle sound notification for %1 events.&lt;br /&gt;If checked, Mumble uses a soundfile predefined by you to indicate %1 events. Soundfiles and Text-To-Speech cannot be used at the same time.</source>
        <oldsource>Click here to toggle sound notification for %1 events.&lt;br /&gt;If checked, mumble uses a soundfile predefined by you to indicate %1 events. Soundfiles and Text-To-Speech cannot be used at the same time.</oldsource>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Path to soundfile used for sound notifications in the case of %1 events.&lt;br /&gt;Single click to play&lt;br /&gt;Doubleclick to change&lt;br /&gt;Be sure that sound notifications for these events are enabled or this field will not have any effect.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+93"/>
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
        <source>The file &apos;%1&apos; does not exist or is not a valid speex file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Log.ui"/>
        <source>Messages</source>
        <translation>Zprávy</translation>
    </message>
    <message>
        <location/>
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
        <location/>
        <source>Text To Speech</source>
        <translation>Čtení textu nahlas</translation>
    </message>
    <message>
        <location/>
        <source>Volume</source>
        <translation>Hlasitost</translation>
    </message>
    <message>
        <location/>
        <source>Volume of Text-To-Speech Engine</source>
        <translation>Hlasitost čtení textu nahlas</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is the volume used for the speech synthesis.&lt;/b&gt;</source>
        <translation>&lt;b&gt;Hlasitost pro hlasovou syntézu&lt;/b&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Length threshold</source>
        <translation>Maximální délka</translation>
    </message>
    <message>
        <location/>
        <source>Message length threshold for Text-To-Speech Engine</source>
        <translation>Maximální délka zprávy pro přečtení hlasovým systémem</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is the length threshold used for the Text-To-Speech Engine.&lt;/b&gt;&lt;br /&gt;Messages longer than this limit will not be read aloud in their full length.</source>
        <translation>&lt;b&gt;Toto je maximální délka zprávy pro čtení&lt;/b&gt;&lt;br /&gt;Zprávy delší než tento limit nebudou přečteny v celé svojí délce.</translation>
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
        <translation>Jazyk</translation>
    </message>
    <message>
        <location/>
        <source>Look and Feel</source>
        <translation>Vzhled a motivy</translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="+46"/>
        <location line="+11"/>
        <source>System default</source>
        <translation>Podle výchozího nastavení systému</translation>
    </message>
    <message>
        <location filename="LookConfig.ui"/>
        <source>Language to use (requires restart)</source>
        <translation>Jazyk pro uživatelské rozhraní (vyžaduje restart)</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets which language Mumble should use.&lt;/b&gt;&lt;br /&gt;You have to restart Mumble to use the new language.</source>
        <translation>&lt;b&gt;Nastaví jazyk, který má aplikace Mumble používat.&lt;/b&gt;&lt;br /&gt;Musíte restartovat Mumble, aby se změna projevila.</translation>
    </message>
    <message>
        <location/>
        <source>Style</source>
        <translation>Styl</translation>
    </message>
    <message>
        <location/>
        <source>Basic widget style</source>
        <translation>Základní styl prvků grafického prostředí</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the basic look and feel to use.&lt;/b&gt;</source>
        <translation>&lt;b&gt;nastaví základní styl vzhledu aplikace.&lt;/b&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Skin</source>
        <translation>Motiv</translation>
    </message>
    <message>
        <location/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location/>
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
        <location/>
        <source>&lt;b&gt;If set, players will be shown above subchannels in the channel view.&lt;/b&gt;&lt;br /&gt;A restart of Mumble is required to see the change.</source>
        <translation>&lt;b&gt;Pokud je tohle nastaveno, pak budou uživatelé zobrazeni v každém kanálu ještě před seznamem podkanálů v přehledu kanálu.&lt;/b&gt;&lt;br /&gt; Pro aktivaci této změny je nutno restartovat aplikaci Mumble.</translation>
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
        <location/>
        <source>Players above Channels</source>
        <translation>Uživatelé nad kanály</translation>
    </message>
    <message>
        <location/>
        <source>List players above subchannels (requires restart).</source>
        <translation>Vypíše seznam uživatelů nad seznamem kanálů (vyžaduje restart aplikace).</translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="+14"/>
        <source>User Interface</source>
        <translation>Rozhraní</translation>
    </message>
    <message>
        <location line="+85"/>
        <source>Choose skin file</source>
        <translation>Vyberte soubor s motivem</translation>
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
        <location filename="LookConfig.cpp" line="-95"/>
        <source>None</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Only with players</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
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
        <location/>
        <source>This sets which channels to automatically expand. &lt;i&gt;None&lt;/i&gt; and &lt;i&gt;All&lt;/i&gt; will expand no or all channels, while &lt;i&gt;Only with players&lt;/i&gt; will expand and collapse channels as players join and leave them.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="+2"/>
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
        <source>This changes the behaviour when moving channels.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This sets the behaviour of channel drags, it can be used to prevent accidental dragging. &lt;i&gt;Move Channel&lt;/i&gt; moves the channel without prompting. &lt;i&gt;Do Nothing&lt;/i&gt; does nothing and prints an error message. &lt;i&gt;Ask&lt;/i&gt; uses a message box to confirm if you really wanted to move the channel.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Ask whether to close or minimize when quitting Mumble.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;If set, will verify you want to quit if connected.&lt;b&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Ask on quit while connected</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Make the Mumble window appear on top of other windows.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This makes the Mumble window a topmost window.&lt;/b&gt;</source>
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
        <source>Hide in tray when minimized</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Hide the main Mumble window in the tray when it is minimized.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <location filename="main.cpp" line="+239"/>
        <source>Welcome to Mumble.</source>
        <translation>Vítejte v aplikaci Mumble.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="+127"/>
        <location line="+1196"/>
        <source>Root</source>
        <translation>Kořen</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>&amp;Quit Mumble</source>
        <translation type="unfinished">&amp;Ukončit</translation>
    </message>
    <message>
        <location/>
        <source>Closes the program</source>
        <translation>Uzavře aplikaci</translation>
    </message>
    <message>
        <location/>
        <source>Exits the application.</source>
        <translation>Ukončí aplikaci.</translation>
    </message>
    <message>
        <source>Ctrl+Q</source>
        <comment>Quit</comment>
        <translation type="obsolete">Ukončit</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Connect</source>
        <translation>&amp;Připojit</translation>
    </message>
    <message>
        <location/>
        <source>Open the server connection dialog</source>
        <translation>Otevřít dialog nastavení spojení se serverem</translation>
    </message>
    <message>
        <location/>
        <source>Shows a dialog of registered servers, and also allows quick connect.</source>
        <translation>Ukáže dialog registrovaných serverů a umožňuje tak rychlé a snadné připojení k nim.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Disconnect</source>
        <translation>&amp;Odpojit</translation>
    </message>
    <message>
        <location/>
        <source>Disconnect from server</source>
        <translation>Odpojit od serveru</translation>
    </message>
    <message>
        <location/>
        <source>Disconnects you from the server.</source>
        <translation>Odpojí  k serveru otevřená spojení.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Ban lists</source>
        <translation>Seznam &amp;zákazů</translation>
    </message>
    <message>
        <location/>
        <source>Edit ban lists on server</source>
        <translation>Upravit seznam banů</translation>
    </message>
    <message>
        <location/>
        <source>This lets you edit the server-side IP ban lists.</source>
        <translation>Tady můžete upravit seznam banů (zakázaných ip adres) pro server.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Kick</source>
        <translation>&amp;Vykopnout</translation>
    </message>
    <message>
        <location/>
        <source>Kick player (with reason)</source>
        <translation>Vykonout uživatele s odůvodněním)</translation>
    </message>
    <message>
        <location/>
        <source>Kick selected player off server. You&apos;ll be asked to specify a reason.</source>
        <translation>Vykopne vybraného uživatele ze serveru. Budete dotázán na důvod.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Ban</source>
        <translation>Zákaz (&amp;ban)</translation>
    </message>
    <message>
        <location/>
        <source>Kick and ban player (with reason)</source>
        <translation>Vykopnout a zakázat uživatele (s odůvodněním)</translation>
    </message>
    <message>
        <location/>
        <source>Kick and ban selected player from server. You&apos;ll be asked to specify a reason.</source>
        <translation>Vykopne a zakáže vybraného uživatele. Budete požádání o doplnění důvodu.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Mute</source>
        <translation>Vypnout &amp;mluvení</translation>
    </message>
    <message>
        <location/>
        <source>Mute player</source>
        <translation>Zakázat uživateli mluvit</translation>
    </message>
    <message>
        <location/>
        <source>Mute or unmute player on server. Unmuting a deafened player will also undeafen them.</source>
        <translation>Zakáže vybranému uživateli na serveru schopost mluvit, nebo mu ji opět vrátí. Vrácením práva mluvit uživateli s vypnutým poslechem se mu zároveň zapne poslech.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Deafen</source>
        <translation>Zakázat &amp;poslouchání</translation>
    </message>
    <message>
        <location/>
        <source>Deafen player</source>
        <translation>Vypnout poslech uživateli</translation>
    </message>
    <message>
        <location/>
        <source>Deafen or undeafen player on server. Deafening a player will also mute them.</source>
        <translation>Vypnout/zapnout poslech uživateli.  Vypnutím poslechu se uživateli zároveň zakáže mluvit.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Local Mute</source>
        <translation>Lokální um&amp;lčení</translation>
    </message>
    <message>
        <source>Mute or unmute player locally.</source>
        <translation type="obsolete">Lokální zákaz/povolení mluvení uživateli. </translation>
    </message>
    <message>
        <location/>
        <source>Send Messa&amp;ge</source>
        <translation>&amp;Odeslat zprávu</translation>
    </message>
    <message>
        <location/>
        <source>Send a Text Message</source>
        <translation>Odeslat textovou zprávu</translation>
    </message>
    <message>
        <location/>
        <source>Sends a text message to another user.</source>
        <translation>Pošle textovou zprávu jinému uživateli.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Add</source>
        <translation>&amp;Přidat</translation>
    </message>
    <message>
        <location/>
        <source>Add new channel</source>
        <translation>Přidat nový kanál</translation>
    </message>
    <message>
        <location/>
        <source>This adds a new sub-channel to the currently selected channel.</source>
        <translation>Přidá nový kanál do vybraného kanálu. </translation>
    </message>
    <message>
        <location/>
        <source>&amp;Remove</source>
        <translation>Odst&amp;ranit</translation>
    </message>
    <message>
        <location/>
        <source>Remove channel</source>
        <translation>Odstranit kanál</translation>
    </message>
    <message>
        <location/>
        <source>This removes a channel and all sub-channels.</source>
        <translation>Odstraní kanál a všechny jeho podkanály.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Edit ACL</source>
        <translation>Uprav&amp;it ACL</translation>
    </message>
    <message>
        <location/>
        <source>Edit Groups and ACL for channel</source>
        <translation>Upravit Skupiny a ACL pro kanál</translation>
    </message>
    <message>
        <location/>
        <source>This opens the Group and ACL dialog for the channel, to control permissions.</source>
        <translation>Otevře dialogové okno skupin a ACL daného kanálu. Zde se mohou nastavit oprávnění k určitým operacím.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Rename</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&amp;Change Description</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Changes the channel description</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This changes the description of a channel.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&amp;Link</source>
        <translation>&amp;Propojit</translation>
    </message>
    <message>
        <location/>
        <source>Link your channel to another channel</source>
        <translation>Propojí současný kanál s kanálem označeným kurzorem</translation>
    </message>
    <message>
        <location/>
        <source>This links your current channel to the selected channel. If they have permission to speak in the other channel, players can now hear each other. This is a permanent link, and will last until manually unlinked or the server is restarted. Please see the shortcuts for push-to-link.</source>
        <translation>Propojí současný kanál s vybraným kanálem. Pokud mají povoleno mluvit, uslyší se účastníci navzájem. Toto spojení bude fungovat až do jeho zrušení nebo do restartování serveru. </translation>
    </message>
    <message>
        <source>&amp;Unlink</source>
        <translation type="obsolete">O&amp;dpojit</translation>
    </message>
    <message>
        <location/>
        <source>Unlink your channel from another channel</source>
        <translation>Zruší propojení současného a zvoleného kanálu</translation>
    </message>
    <message>
        <location/>
        <source>This unlinks your current channel from the selected channel.</source>
        <translation>Zruší propojení mezi současným a vybraným kanálem.</translation>
    </message>
    <message>
        <location/>
        <source>Unlink &amp;All</source>
        <translation>Odpoj&amp;it vše</translation>
    </message>
    <message>
        <location/>
        <source>Unlinks your channel from all linked channels.</source>
        <translation>Zruší propojení sučasného kanálu ke všem kanálům s ním spojeným.</translation>
    </message>
    <message>
        <location/>
        <source>This unlinks your current channel (not the selected one) from all linked channels.</source>
        <translation>Zruší propojení mezi současným kanálem uživatele a všech kanálů, které jsou s tímto kanálem propojeny. </translation>
    </message>
    <message>
        <location/>
        <source>&amp;Reset</source>
        <translation>&amp;Resetovat audio</translation>
    </message>
    <message>
        <location/>
        <source>Reset audio preprocessor</source>
        <translation>Resetuje zvukový preprocesor</translation>
    </message>
    <message>
        <location/>
        <source>This will reset the audio preprocessor, including noise cancellation, automatic gain and voice activity detection. If something suddenly worsens the audio environment (like dropping the microphone) and it was temporary, use this to avoid having to wait for the preprocessor to readjust.</source>
        <translation>Resetuje zvukový preprocesor, včetně rušení šumu, automatické hlasitosti a detekce hlasové aktivity. Pokud se nějak náhle zhorší zvukové prostředí (například při upuštění mikrofonu na stůl) a příčina problémů odezní, jde tak rychle znovunastavit rozumné hodnoty.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Mute Self</source>
        <translation>Vypnout si &amp;mluvení</translation>
    </message>
    <message>
        <location/>
        <source>Mute yourself</source>
        <translation>Zakázat si mluvení</translation>
    </message>
    <message>
        <location/>
        <source>Mute or unmute yourself. When muted, you will not send any data to the server. Unmuting while deafened will also undeafen.</source>
        <translation>Zakázat/povolit si mluvení.  Když je zakázáno mluvení, neposílají se na server žádná zvuková data. Povolením mluvení se zároveň zapína poslech.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Deafen Self</source>
        <translation>Vypnout si &amp;poslech</translation>
    </message>
    <message>
        <location/>
        <source>Deafen yourself</source>
        <translation>Vypnout si poslech</translation>
    </message>
    <message>
        <location/>
        <source>Deafen or undeafen yourself. When deafened, you will not hear anything. Deafening yourself will also mute.</source>
        <translation>Vypnout si poslech. Tak neuslyšíte komunikaci. Vypnutí poslechu automaticky zároveň zakáže mluvení.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Text-To-Speech</source>
        <translation>Výstup &amp;textu do hlasové syntézy</translation>
    </message>
    <message>
        <location/>
        <source>Toggle Text-To-Speech</source>
        <translation>Přepnout výstup textu do hlasové syntézy</translation>
    </message>
    <message>
        <location/>
        <source>Enable or disable the text-to-speech engine. Only messages enabled for TTS in the Configuration dialog will actually be spoken.</source>
        <translation>Povolí nebo zakáže převod textu na počítačem mluvený hlas. Budou přečtena pouze textová hlášení, u kterých je to nastaveno v konfiguračním dialogu.</translation>
    </message>
    <message>
        <location/>
        <source>S&amp;tatistics</source>
        <translation>S&amp;tatistika</translation>
    </message>
    <message>
        <location/>
        <source>Display audio statistics</source>
        <translation>Zobrazí zvukovou statistiku</translation>
    </message>
    <message>
        <location/>
        <source>Pops up a small dialog with information about your current audio input.</source>
        <translation>Vyskočí malé okénko s informacemi o současném audio vstupu.</translation>
    </message>
    <message>
        <location/>
        <source>Forcibly unlink plugin</source>
        <translation>Násilně odstraň plugin</translation>
    </message>
    <message>
        <location/>
        <source>This forces the current plugin to unlink, which is handy if it is reading completely wrong data.</source>
        <translation>Násilně udpojí současný plugin, což může být užitečné, pokud čte a zpracovává úplně chybný data.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Settings</source>
        <translation>&amp;Nastavení</translation>
    </message>
    <message>
        <location/>
        <source>Configure Mumble</source>
        <translation>Nastavit Mumble</translation>
    </message>
    <message>
        <location/>
        <source>Allows you to change most settings for Mumble.</source>
        <translation>Nastavení většiny možností.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Audio Wizard</source>
        <translation>&amp;Zvukový Wizard</translation>
    </message>
    <message>
        <location/>
        <source>Start the audio configuration wizard</source>
        <translation>Spustí zvukový Wizard pro nastavení</translation>
    </message>
    <message>
        <location/>
        <source>This will guide you through the process of configuring your audio hardware.</source>
        <translation>Tohle vás provede zkrze proces konfigurace zvukového hardware.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;What&apos;s This?</source>
        <translation>&amp;Co je to?</translation>
    </message>
    <message>
        <location/>
        <source>Enter What&apos;s This? mode</source>
        <translation>Vstoupí do režimu &quot;co je tohle?&quot;</translation>
    </message>
    <message>
        <location/>
        <source>Click this to enter &quot;What&apos;s This?&quot; mode. Your cursor will turn into a question mark. Click on any button, menu choice or area to show a description of what it is.</source>
        <translation>Klikněte pro vsoupení do režimu &quot;Co je tohle?&quot; Váš kurzor se změní na otazník. Jakmile máte tlačítko, výběr menu nebo cokoliv co chcete ukázat.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;About</source>
        <translation>&amp;O aplikaci</translation>
    </message>
    <message>
        <location/>
        <source>Information about Mumble</source>
        <translation>Informace o Mumble</translation>
    </message>
    <message>
        <location/>
        <source>Shows a small dialog with information and license for Mumble.</source>
        <translation>Ukáže okno s informacemi a licenčním ujednáním pro používání Mumble.


</translation>
    </message>
    <message>
        <location/>
        <source>About &amp;Speex</source>
        <translation>O kodeku &amp;Speex</translation>
    </message>
    <message>
        <location/>
        <source>Information about Speex</source>
        <translation>Informace o kodeku Speex</translation>
    </message>
    <message>
        <location/>
        <source>Shows a small dialog with information about Speex.</source>
        <translation>Ukáže informace o použitém kodeku Speex.</translation>
    </message>
    <message>
        <source>&amp;About Qt</source>
        <translation type="obsolete">O &amp;knihovně Qt</translation>
    </message>
    <message>
        <location/>
        <source>Information about Qt</source>
        <translation>Informace o knihovně Qt</translation>
    </message>
    <message>
        <location/>
        <source>Shows a small dialog with information about Qt.</source>
        <translation>Ukáže dialogové okno s informacemi o knihovně QT.</translation>
    </message>
    <message>
        <location/>
        <source>Check for &amp;Updates</source>
        <translation>Zjistit &amp;dostupnost nové verze</translation>
    </message>
    <message>
        <location/>
        <source>Check for new version of Mumble</source>
        <translation>Ověřit dostupnost nových verzí aplikace Mumble</translation>
    </message>
    <message>
        <location/>
        <source>Connects to the Mumble webpage to check if a new version is available, and notifies you with an appropriate download URL if this is the case.</source>
        <translation>Připojí se k webové stránce Mumble pro zjištění případné dostupnosti novějších verzí a vypíše vhodný odkaz na stažení, pokud je nová verze k dispozici.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="-1086"/>
        <source>Mumble -- %1</source>
        <translation>Mumble =- %1 </translation>
    </message>
    <message>
        <source>Log of messages</source>
        <translation type="obsolete">Záznam zpráv</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>This shows all recent activity. Connecting to servers, errors and information messages all show up here.&lt;br /&gt;To configure exactly which messages show up here, use the &lt;b&gt;Settings&lt;/b&gt; command from the menu.</source>
        <translation>Zobrazuje nedávné aktivity, jako např. připojování k serverům, chyby a informační zprávy.&lt;br /&gt; V nastavení jde určit, které z těchto zpráv se mají či nemají zobrazovat.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Server</source>
        <translation>&amp;Server</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Player</source>
        <translation>&amp;Uživatel</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Channel</source>
        <translation>&amp;Kanál</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Audio</source>
        <translation>&amp;Zvuk</translation>
    </message>
    <message>
        <location/>
        <source>C&amp;onfigure</source>
        <translation>&amp;Nastavit</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Help</source>
        <translation>&amp;Pomoc</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="-74"/>
        <source>Push-to-Talk</source>
        <comment>Global Shortcut</comment>
        <translation>Mluvení při stisku klávesy</translation>
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
        <translation>Resetovat zvukový procesor</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Toggle Mute Self</source>
        <comment>Global Shortcut</comment>
        <translation>Zákázat/povolit si mluvení</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Toggle self-mute status.</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This will toggle your muted status. If you toggle this off, you will also disable self-deafen.</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Toggle Deafen Self</source>
        <comment>Global Shortcut</comment>
        <translation>Poslech zapnout/vypnout</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Toggle self-deafen status.</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This will toggle your deafen status. If you toggle this on, you will also enable self-mute.</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Unlink Plugin</source>
        <comment>Global Shortcut</comment>
        <translation>Odpojit Plugin</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Force Center Position</source>
        <comment>Global Shortcut</comment>
        <translation>Vynutit použití střední pozice</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Chan Parent</source>
        <comment>Global Shortcut</comment>
        <translation>Nadřazený kanál</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Chan Sub#%1</source>
        <comment>Global Shortcut</comment>
        <translation>Podkanál #%1</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Chan All Subs</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Push-to-Mute</source>
        <comment>Global Shortcut</comment>
        <translation>Zakázat si mluvení při stisku klávesy</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Join Channel</source>
        <comment>Global Shortcut</comment>
        <translation>Vstoupit do kanálu</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Toggle Overlay</source>
        <comment>Global Shortcut</comment>
        <translation>Přepnout Overlay</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Toggle state of in-game overlay.</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This will switch the states of the ingame overlay between showing everybody, just the players who are talking, and nobody.</source>
        <comment>Global Shortcut</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Alt Push-to-Talk</source>
        <comment>Global Shortcut</comment>
        <translation>Alternativní klávesa pro mluvení</translation>
    </message>
    <message>
        <location line="+354"/>
        <source>Reconnecting.</source>
        <translation>Opětovné připojování.</translation>
    </message>
    <message>
        <location line="+173"/>
        <source>Kicking player %1</source>
        <translation>Vykopnutí uživaetele %1</translation>
    </message>
    <message>
        <location line="+0"/>
        <location line="+22"/>
        <source>Enter reason</source>
        <translation>Zadejte důvod</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Banning player %1</source>
        <translation>Ban pro uživatele %1</translation>
    </message>
    <message>
        <location line="+22"/>
        <source>Sending message to %1</source>
        <translation>Posílam zprávu pro %1</translation>
    </message>
    <message>
        <source>Enter message</source>
        <translation type="obsolete">Zadejte zprávu</translation>
    </message>
    <message>
        <location line="+11"/>
        <location line="+253"/>
        <source>To %1: %2</source>
        <translation>Pro %1: %2</translation>
    </message>
    <message>
        <location line="-253"/>
        <location line="+253"/>
        <source>Message to %1</source>
        <translation>Zpráva pro %1</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>Mumble</source>
        <translation>Mumble</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="-157"/>
        <location line="+43"/>
        <source>Channel Name</source>
        <translation>Název kanálu</translation>
    </message>
    <message>
        <location line="-21"/>
        <source>Are you sure you want to delete %1 and all its sub-channels?</source>
        <translation>Opravdu chcete odstranit kanál %1 včetně všech jeho podkanálů?</translation>
    </message>
    <message>
        <location line="+183"/>
        <source>Unmuted and undeafened.</source>
        <translation>Opět schopen slyšet a mluvit.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Unmuted.</source>
        <translation>Opět schopen mluvit.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Muted.</source>
        <translation>Zakázáno mluvení.</translation>
    </message>
    <message>
        <location line="+20"/>
        <source>Muted and deafened.</source>
        <translation>Zakázáno mluvit a poslouchat.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Deafened.</source>
        <translation>Vypnut poslech.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Undeafened.</source>
        <translation>Opět schopen slyšet.</translation>
    </message>
    <message>
        <location line="+62"/>
        <source>About Qt</source>
        <translation>O knihovně Qt</translation>
    </message>
    <message>
        <location line="+97"/>
        <source>Joining %1.</source>
        <translation>Vstupuji do %1.</translation>
    </message>
    <message>
        <source>Connected to server.</source>
        <translation type="obsolete">Připojen k serveru.</translation>
    </message>
    <message>
        <location line="+131"/>
        <source>Server connection failed: %1.</source>
        <translation>Připojování k serveru selhalo: %1.</translation>
    </message>
    <message>
        <location line="+15"/>
        <source>Invalid username</source>
        <translation>Špatné jméno uživatele</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>You connected with an invalid username, please try another one.</source>
        <translation>Připojujete se s nesprávným jménem uživatele, prosím zkuste nějaké jiné.</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>That username is already in use, please try another username.</source>
        <translation>Toto uživatelské jméno je již používáno, prosím zkuste nějaké jiné.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Wrong password</source>
        <translation>Špatné heslo</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Wrong password for registered users, please try again.</source>
        <translation>Špatné heslo registrovaného uživatele, prosím zkuste znova.</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Wrong server password for unregistered user account, please try again.</source>
        <translation>Špatné heslo serveru pro neregistrované uživatele, prosím zkuste znova.</translation>
    </message>
    <message>
        <location line="-18"/>
        <source>Disconnected from server.</source>
        <translation>Spojení se serverem bylo zrušeno.</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="+56"/>
        <source>Joined server: %1.</source>
        <translation>K serveru se připojil: %1.</translation>
    </message>
    <message>
        <location line="+20"/>
        <source>Left server: %1.</source>
        <translation>Uživatel %1 opustil server.</translation>
    </message>
    <message>
        <location line="+33"/>
        <source>%1 is now muted and deafened.</source>
        <translation>%1 má zakázáno mluvit a poslouchat.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1 is now muted.</source>
        <translation>%1 má zakázáno mluvit.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1 is now unmuted.</source>
        <translation>%1 je nyní schopen mluvit.</translation>
    </message>
    <message>
        <location line="+13"/>
        <location line="+20"/>
        <source>server</source>
        <translation>server</translation>
    </message>
    <message>
        <location line="-17"/>
        <source>You were muted by %1.</source>
        <translation>%1 Vám zakázal mluvit.</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>You were unmuted by %1.</source>
        <translation>Vrácena schopnost mluvit od %1.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>%1 muted by %2.</source>
        <translation>%2 zakázal mluvit uživateli %1.</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>%1 unmuted by %2.</source>
        <translation>%1 získává schopnost mluvit od %2.</translation>
    </message>
    <message>
        <location line="+17"/>
        <source>You were deafened by %1.</source>
        <translation>Bylo vám vypnut poslech uživatelem %1.</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>You were undeafened by %1.</source>
        <translation>Byla Vám vrácena schopnost slyšet od %1.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>%1 deafened by %2.</source>
        <translation>%2 vypnul poslech uživateli %1.</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>%1 undeafened by %2.</source>
        <translation>%1 byla vlrácena schopnost slyšet od %2.</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>You were kicked from the server by %1: %2.</source>
        <translation>Byl jste vykopnut ze serveru od %1: %2.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>%3 was kicked from the server by %1: %2.</source>
        <translation>%3 byl vykopnut ze serveru od %1: %2.</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>You were kicked and banned from the server by %1: %2.</source>
        <translation>Byl jste vykopnut a dostal jste ban (zákaz) od %1: %2.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>%3 was kicked and banned from the server by %1: %2.</source>
        <translation>%3 byl vykopnut a dostal ban (zákaz) od %1: %2.</translation>
    </message>
    <message>
        <location line="+27"/>
        <source>You were moved to %1 by %2.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>%1 left channel.</source>
        <translation>%1 opustil kanál.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1 moved out by %2 to %3.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+10"/>
        <source>%1 moved in by %2 from %3.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>%1 moved out by %2.</source>
        <translation type="obsolete">%1 byl přesunut z kanálu uživatelem %2.</translation>
    </message>
    <message>
        <location line="-2"/>
        <source>%1 entered channel.</source>
        <translation>%1 vstoupil do kanálu.</translation>
    </message>
    <message>
        <source>%1 moved in by %2.</source>
        <translation type="obsolete">%1 byl přesunut do kanálu zásahem uživatele %2.</translation>
    </message>
    <message>
        <location line="+100"/>
        <source>Server connection rejected: %1.</source>
        <translation>Spojení se serverem bylo odmítnuto: %1.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Denied: %1.</source>
        <translation>Zakázano: %1.</translation>
    </message>
    <message>
        <location line="+27"/>
        <source>From %1: %2</source>
        <translation>Od %1: %2</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Message from %1</source>
        <translation></translation>
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
        <location filename="MainWindow.cpp" line="-1012"/>
        <source>Opening URL %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>URL scheme is not &apos;mumble&apos;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+25"/>
        <source>Connecting to %1</source>
        <translation type="unfinished">Připojování k %1</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Enter username</source>
        <translation type="unfinished">Zadejte jmého uživatele</translation>
    </message>
    <message>
        <location line="+205"/>
        <source>Mumble Server Information</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <location line="+753"/>
        <source>&amp;View Certificate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-21"/>
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
        <location line="+7"/>
        <source>&lt;p&gt;%1.&lt;br /&gt;The specific errors with this certificate are: &lt;/p&gt;&lt;ol&gt;%2&lt;/ol&gt;&lt;p&gt;Do you wish to accept this certificate anyway?&lt;br /&gt;(It will also be stored so you won&apos;t be asked this again.)&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-769"/>
        <source>&lt;h2&gt;Control channel&lt;/h2&gt;&lt;p&gt;Encrypted with %1 bit %2&lt;br /&gt;%3 ms average latency (%4 variance)&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Voice channel is sent over control channel.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>&lt;h2&gt;Voice channel&lt;/h2&gt;&lt;p&gt;Encrypted with 128 bit OCB-AES128&lt;br /&gt;%1 ms average latency (%4 variance)&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+7"/>
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
        <location line="+326"/>
        <source>Change description of channel %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+79"/>
        <source>Sending message to channel %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+27"/>
        <source>Sending message to channel tree %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+13"/>
        <source>To tree %1: %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Message to tree %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>Ctrl+Q</source>
        <translation type="unfinished">Ukončit</translation>
    </message>
    <message>
        <location/>
        <source>Mute player locally</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Mute or unmute player locally. Use this on other players in the same room.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Renames the channel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This renames a channel.</source>
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
        <location/>
        <source>Send &amp;Tree Message</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Send message to channel tree.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This sends a text message to a channel and it&apos;s subchannels.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="-12"/>
        <source>Server maximum bandwidth is only %1 kbit/s. Quality auto-adjusted.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="main.cpp" line="+17"/>
        <source>This is the first time you&apos;re starting Mumble.&lt;br /&gt;Would you like to go through the Audio Wizard to configure your soundcard?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="-441"/>
        <source>&lt;h2&gt;Audio bandwidth&lt;/h2&gt;&lt;p&gt;Maximum %1 kbit/s&lt;br /&gt;Current %2 kbit/s (Quality %3)&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+682"/>
        <source>Connected to server %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-1001"/>
        <source>Mumble is currently connected to a server. Do you want to Close or Minimize it?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Close</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-75"/>
        <location line="+76"/>
        <source>Minimize</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-78"/>
        <source>&amp;Window</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Ctrl+M</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-25"/>
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
        <location line="+176"/>
        <source>Clear</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>Log</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="PlayerModel.cpp" line="+995"/>
        <source>You have Channel Dragging set to &quot;Do Nothing&quot; so the channel wasn&apos;t moved.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Unknown Channel Drag mode in PlayerModel::dropMimeData.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>&amp;Unlink</source>
        <comment>Channel</comment>
        <translation type="unfinished">O&amp;dpojit</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Unlink</source>
        <comment>Plugin</comment>
        <translation type="unfinished">O&amp;dpojit</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="+10"/>
        <source>Server</source>
        <comment>message from</comment>
        <translation type="unfinished">Server</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
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
</context>
<context>
    <name>NetworkConfig</name>
    <message>
        <location filename="NetworkConfig.cpp" line="+51"/>
        <source>Network</source>
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
        <location/>
        <source>Use TCP mode</source>
        <translation type="unfinished">Použít TCP režim</translation>
    </message>
    <message>
        <location/>
        <source>Reconnect when disconnected</source>
        <translation type="unfinished">Přihlásit zpátky na server, pokud došlo k výpadku spojení</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Reconnect when disconnected&lt;/b&gt;.&lt;br /&gt;This will make Mumble try to automatically reconnect after 10 seconds if your server connection fails.</source>
        <translation type="unfinished">&lt;b&gt;Automatické připojení&lt;/b&gt;.&lt;br /&gt;Pokud se ztratí spojení se serverem,  Mumble se pokusí automaticky spojení obnovit každých 10 sekund.</translation>
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
        <source>&lt;b&gt;Type of proxy to connect through&lt;/b&gt;&lt;br /&gt;This makes Mumble connect through a proxy for all outgoing connections. Note: Proxy tunneling forces Mumble into TCP compatibility mode, causing all voice data to be sent via the control channel.</source>
        <oldsource>&lt;b&gt;Type of proxy to connect through&lt;/b&gt;&lt;br /&gt;This makes Mumble connect through a proxy for all outgoing connections. Note: Proxy tunneling forces Mumble into TCP compatability mode, causing all voice data to be sent via the control channel.</oldsource>
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
        <source>&lt;b&gt;Hostname of the proxy.&lt;b&gt;&lt;br /&gt;This field specifies the hostname of the proxy you wish to tunnel network traffic through.</source>
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
        <source>Mumble services</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Check for updates on startup</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Submit anonymous statistics to the Mumble project</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Submit anonymous statistics.&lt;/b&gt;&lt;br /&gt;Mumble has a small development team, and as such needs to focus it&apos;s development where it&apos;s needed most. By submitting a bit of statistics you help the project determine where to focus development.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
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
        <source>Mumble</source>
        <translation type="obsolete">Mumble</translation>
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
</context>
<context>
    <name>OverlayConfig</name>
    <message>
        <source>Options</source>
        <translation type="obsolete">Volby</translation>
    </message>
    <message>
        <source>Position</source>
        <translation type="obsolete">Umístění</translation>
    </message>
    <message>
        <source>Font</source>
        <translation type="obsolete">Písmo</translation>
    </message>
    <message>
        <source>Enable Overlay</source>
        <translation type="obsolete">Zapnout Overlay</translation>
    </message>
    <message>
        <source>Enable overlay.</source>
        <translation type="obsolete">Zapnout overlay.</translation>
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
        <source>Who to show on the overlay</source>
        <translation type="obsolete">Kdo bude zobrazen v overlay vrstvě</translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets who to show in the in-game overlay.&lt;/b&gt;&lt;br /&gt;If many people are connected to the same channel, the overlay list might be very long. Use this to shorten it.&lt;br /&gt;&lt;i&gt;No one&lt;/i&gt; - Don&apos;t show anyone (but leave overlay running).&lt;br /&gt;&lt;i&gt;Only talking&lt;/i&gt; - Only show talking people.&lt;br /&gt;&lt;i&gt;Everyone&lt;/i&gt; - Show everyone.</source>
        <translation type="obsolete">&lt;b&gt;Nastaví, kdo bude vidět v overlay vrstvě.&lt;/b&gt;&lt;br /&gt;Pokud bude připojeno příliš mnoho lidí, seznam se může stát nepříjemně dlouhým. Tímto nastavením jej můžete zkrátit.&lt;br /&gt;&lt;i&gt;Nikoho&lt;/i&gt; - nezobrazovat nikoho (ale nechat běžící overlay).&lt;br /&gt;&lt;i&gt;Pozue mluvící&lt;/i&gt; - zobrazí pouze uživatele, kteří právě mluví.&lt;br /&gt;&lt;i&gt;Každého&lt;/i&gt; - zobrazí úplně každého.</translation>
    </message>
    <message>
        <source>Always Show Self</source>
        <translation type="obsolete">Vždy ukázat sebe</translation>
    </message>
    <message>
        <source>Always show yourself on overlay.</source>
        <translation type="obsolete">Vždy ukáže vaše jméno v overlay vrstvě.</translation>
    </message>
    <message>
        <source>This sets whether to always show yourself or not. This setting is useful if you aren&apos;t showing everyone in the overlay, as then you would only see your own status if you were talking, which wouldn&apos;t let you see that you were deafened or muted.</source>
        <translation type="obsolete">Nastavuje, jestli budete vidět vy sami. Toto nastavení je nejvíce užitečné, pokud nikoho ostatního vidět nechcete, pouze chcete vidět svůj vlastní stav - jestli mluvíte nebo ne, nebo jestli máte zakázáno mluvení/poslech.</translation>
    </message>
    <message>
        <source>Show User Textures</source>
        <translation type="obsolete">Ukázat obrázky uživatelů</translation>
    </message>
    <message>
        <source>Show User custom textures instead of text on the overlay.</source>
        <translation type="obsolete">Ukázat v overlay vrstvě uživatelův nastavený obrázek místo textu.</translation>
    </message>
    <message>
        <source>This sets whether to download and use custom textures for registered users. If disabled, the regular outline text will be used instead.</source>
        <translation type="obsolete">Nastavuje, jestli se má stáhnout a použít navolený obrázek pro zobrazení registrovaných uživatelů. Pokud je zakázáno, zobrazí se uživatelé běžným textem.</translation>
    </message>
    <message>
        <source>Grow Left</source>
        <translation type="obsolete">Zvětšovat doleva</translation>
    </message>
    <message>
        <source>Let overlay grow to the left</source>
        <translation type="obsolete">Overlay se zvětšuje směrem doleva</translation>
    </message>
    <message>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow to the left if needed.</source>
        <translation type="obsolete">Ovelay vrstva je  tak malá, jak je to jenom možné a je na zvolené pozici. Tímto nastavením se povolí zvětšování overlay na levou stranu, pokud je to potřeba.</translation>
    </message>
    <message>
        <source>Grow Right</source>
        <translation type="obsolete">Zvětšovat doprava</translation>
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
        <source>Grow Up</source>
        <translation type="obsolete">Zvětšovat nahoru</translation>
    </message>
    <message>
        <source>Let overlay grow upwards</source>
        <translation type="obsolete">Overlay se zvětšuje směrem nahoru</translation>
    </message>
    <message>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow to the top if needed.</source>
        <translation type="obsolete">Ovelay vrstva je  tak malá, jak je to jenom možné a je na zvolené pozici. Tímto nastavením se povolí zvětšování overlay nahoru, pokud je to potřeba.</translation>
    </message>
    <message>
        <source>Grow Down</source>
        <translation type="obsolete">Zvětšovat dolů</translation>
    </message>
    <message>
        <source>Let overlay grow downwards</source>
        <translation type="obsolete">Overlay se zvětšuje směrem dolů</translation>
    </message>
    <message>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow towards the bottom if needed.</source>
        <translation type="obsolete">Ovelay vrstva je  tak malá, jak je to jenom možné a je na zvolené pozici. Tímto nastavením se povolí zvětšování overlay směrem dolů, pokud je to potřeba.</translation>
    </message>
    <message>
        <source>X-Position of Overlay</source>
        <translation type="obsolete">Pozice overlay na ose X</translation>
    </message>
    <message>
        <source>This sets the relative X position of the overlay.</source>
        <translation type="obsolete">Relativni pozice na ose X.</translation>
    </message>
    <message>
        <source>Y-Position of Overlay</source>
        <translation type="obsolete">Pozice overlay na ose Y</translation>
    </message>
    <message>
        <source>This sets the relative Y position of the overlay.</source>
        <translation type="obsolete">Relativni pozice na ose Y.</translation>
    </message>
    <message>
        <source>Current Font</source>
        <translation type="obsolete">Současný font</translation>
    </message>
    <message>
        <source>Set Font</source>
        <translation type="obsolete">nastavit font</translation>
    </message>
    <message>
        <source>Maximum height of names.</source>
        <translation type="obsolete">Maximální výška jmen.</translation>
    </message>
    <message>
        <source>This sets the maximum height of names shown, relative to the screen height. If your active 3D window is 800 pixels tall and this is set to 5%, each name will be 40 pixels tall. Note that the names will not be taller than 60 pixels no matter what you set here.</source>
        <translation type="obsolete">Nastaví maximální výšku zobrazených jmen, relativně k výšce obrazovky. Pokud vaše 3D okno má 800 pixelů na výšku a tato hodnota je nastavena na 5%, každé jméno bude 40 pixelů vysoké. Jména nikdy nebudou větší než 60 pixelů, i kdybyste nastavili vyšší hodnotu.</translation>
    </message>
    <message>
        <source>Maximum height</source>
        <translation type="obsolete">Maximální výška</translation>
    </message>
    <message>
        <source>Change</source>
        <translation type="obsolete">Změnit</translation>
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
        <source>Color for Channels</source>
        <translation type="obsolete">Barva pro kanály</translation>
    </message>
    <message>
        <source>Color for active Channels</source>
        <translation type="obsolete">Barva pro aktivní kanály</translation>
    </message>
    <message>
        <source>Overlay</source>
        <translation type="obsolete">Overlay</translation>
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
        <location filename="PlayerModel.cpp" line="-575"/>
        <source>This is a channel on the server. Only players in the same channel can hear each other.</source>
        <translation>Toto je komunikační kanál na serveru. Pouze učastníci, kteří jsou ve stejném kanálu, se budou navzájem slyšet.</translation>
    </message>
    <message>
        <source>This shows the flags the player has on the server, if any:&lt;br /&gt;&lt;img src=&quot;:/authenticated.png&quot; /&gt;Authenticated user&lt;br /&gt;&lt;img src=&quot;:/muted_self.png&quot; /&gt;Muted (by self)&lt;br /&gt;&lt;img src=&quot;:/muted_server.png&quot; /&gt;Muted (by admin)&lt;br /&gt;&lt;img src=&quot;:/deafened_self.png&quot; /&gt;Deafened (by self)&lt;br /&gt;&lt;img src=&quot;:/deafened_server.png&quot; /&gt;Deafened (by admin)&lt;br /&gt;A player muted by himself is probably just away, talking on the phone or something like that.&lt;br /&gt;A player muted by an admin is probably also just away, and the noise the player is making was annoying enough that an admin muted him.</source>
        <translation type="obsolete">Zobrazení příznaků uživatele (pokud nějaké má nastaveny):&lt;br /&gt;&lt;img src=&quot;:/authenticated.png&quot; /&gt;Přihlášený uživatel&lt;br /&gt;&lt;img src=&quot;:/muted_self.png&quot; /&gt;Zakázal si mluvení&lt;br /&gt;&lt;img src=&quot;:/muted_server.png&quot; /&gt;Bylo mu zakázáno mluvit&lt;br /&gt;&lt;img src=&quot;:/deafened_self.png&quot; /&gt;Vypnutý poslech&lt;br /&gt;&lt;img src=&quot;:/deafened_server.png&quot; /&gt;Byl mu vypnut poslech&lt;br /&gt;Uživatel, který sám sobě zakázal mluvit nebo poslouchat, je pravděpodobně vzdálen od počítače, nebo zrovna nemá čas na hovory (vaří si kávu)&lt;br /&gt;Uživatel, kterému bylo zakázano mluvit od  administrátora, se pravděpodobně choval nezpůsobně.</translation>
    </message>
    <message>
        <location line="+26"/>
        <source>Name</source>
        <translation>Jméno</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Flags</source>
        <translation>Příznaky</translation>
    </message>
    <message>
        <location line="-34"/>
        <source>This is a player connected to the server. The icon to the left of the player indicates whether or not they are talking:&lt;br /&gt;&lt;img src=&quot;skin:talking_on.png&quot; /&gt; Talking&lt;br /&gt;&lt;img src=&quot;skin:talking_off.png&quot; /&gt; Not talking</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+8"/>
        <source>This shows the flags the player has on the server, if any:&lt;br /&gt;&lt;img src=&quot;skin:authenticated.png&quot; /&gt;Authenticated user&lt;br /&gt;&lt;img src=&quot;skin:muted_self.png&quot; /&gt;Muted (by self)&lt;br /&gt;&lt;img src=&quot;skin:muted_server.png&quot; /&gt;Muted (by admin)&lt;br /&gt;&lt;img src=&quot;skin:deafened_self.png&quot; /&gt;Deafened (by self)&lt;br /&gt;&lt;img src=&quot;skin:deafened_server.png&quot; /&gt;Deafened (by admin)&lt;br /&gt;A player muted by himself is probably just away, talking on the phone or something like that.&lt;br /&gt;A player muted by an admin is probably also just away, and the noise the player is making was annoying enough that an admin muted him.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mumble</source>
        <translation type="obsolete">Mumble</translation>
    </message>
    <message>
        <location line="+561"/>
        <source>Are you sure you want to drag this channel?</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PluginConfig</name>
    <message>
        <location filename="Plugins.ui"/>
        <source>Options</source>
        <translation type="unfinished">Volby</translation>
    </message>
    <message>
        <location/>
        <location filename="Plugins.cpp" line="+64"/>
        <source>Plugins</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Link to Game and Transmit Position</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Enable plugins and transmit positional information</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This enables plugins for supported games to fetch your in-game position and transmit that with each voice packet. This enables other players to hear your voice in-game from the direction your character is in relation to their own.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&amp;Reload plugins</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Reloads all plugins</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This rescans and reloads plugins. Use this if you just added or changed a plugin to the plugins directory.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&amp;About</source>
        <translation type="unfinished">&amp;O aplikaci</translation>
    </message>
    <message>
        <location/>
        <source>Information about plugin</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This shows a small information message about the plugin.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&amp;Configure</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Show configuration page of plugin</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>This shows the configuration page of the plugin, if any.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mumble</source>
        <translation type="obsolete">Mumble</translation>
    </message>
    <message>
        <location filename="Plugins.cpp" line="+28"/>
        <source>Plugin has no configure function.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+12"/>
        <source>Plugin has no about function.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="Plugins.ui"/>
        <source>Form</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Plugins</name>
    <message>
        <location filename="Plugins.cpp" line="+127"/>
        <source>%1 lost link.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+39"/>
        <source>%1 linked.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PortAudioSystem</name>
    <message>
        <location filename="PAAudio.cpp" line="+273"/>
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
        <location filename="PulseAudio.cpp" line="+545"/>
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
    <name>ShortcutActionWidget</name>
    <message>
        <location filename="GlobalShortcut.cpp" line="-143"/>
        <source>Unassigned</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ShortcutDelegate</name>
    <message>
        <location line="+58"/>
        <source>Unassigned</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ShortcutKeyWidget</name>
    <message>
        <location line="-122"/>
        <source>Press Shortcut</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>TextMessage</name>
    <message>
        <location filename="TextMessage.ui"/>
        <source>Message to send</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Preview</source>
        <translation type="unfinished"></translation>
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
        <source>Mumble</source>
        <translation type="obsolete">Mumble</translation>
    </message>
    <message>
        <source>Mumble failed to retrieve version information from the SourceForge server.</source>
        <translation type="obsolete">Mumble nedokázalo získat informace o aktuálních verzích ze serveru SourceForge.</translation>
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
        <translation type="unfinished">%1% {1 %2?}</translation>
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
        <location filename="WASAPI.cpp" line="+180"/>
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
