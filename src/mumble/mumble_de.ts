<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.0" language="de">
<context>
    <name>ACLEditor</name>
    <message>
        <location filename="ACLEditor.ui"/>
        <source>Active ACLs</source>
        <translation>Aktive Berechtigungen</translation>
    </message>
    <message>
        <location/>
        <source>Context</source>
        <translation>Kontext</translation>
    </message>
    <message>
        <location/>
        <source>User/Group</source>
        <translation>Benutzer/Gruppe</translation>
    </message>
    <message>
        <location/>
        <source>Permissions</source>
        <translation>Rechte</translation>
    </message>
    <message>
        <location/>
        <source>Group</source>
        <translation>Gruppe</translation>
    </message>
    <message>
        <source>&lt;b&gt;Password&lt;/b&gt;&lt;br&gt;This field allows you to easily set and change the password of a channel. In the background it uses Mumble&apos;s access tokens feature. To allow more fine grained and powerful access control directly use ACLs and groups instead (&lt;i&gt;Advanced configuration&lt;/i&gt; has to be checked to be able to see these settings).</source>
        <translation type="obsolete">&lt;b&gt;Passwort&lt;/b&gt;&lt;br&gt;Dieses Feld erlaubt es Ihnen auf einfache Weise ein Passwort für den Kanal zu setzen und zu ändern. Im Hintergrund werden dazu die Zugriffscodes verwendet. Für noch genauere und feiner justierbarere Einstellungen verwenden Sie die ACLs und Gruppen (&lt;i&gt;Erweiterte Einstellungen&lt;/i&gt; muss aktiviert sein um diese Einstellungen zu sehen).</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Position&lt;/b&gt;&lt;br/&gt;
This value enables you to change the way Mumble arranges the channels in the tree. A channel with a higher &lt;i&gt;Position&lt;/i&gt; value will always be placed below one with a lower value and the other way around. If the &lt;i&gt;Position&lt;/i&gt; value of two channels is equal they will get sorted alphabetically by their name.</source>
        <oldsource>&lt;b&gt;Position&lt;/b&gt;&lt;br/&gt;
This value enables you to change the way mumble arranges the channels in the tree. A channel with a higher &lt;i&gt;Position&lt;/i&gt; value will always be placed below one with a lower value and the other way around. If the &lt;i&gt;Position&lt;/i&gt; value of two channels is equal they will get sorted alphabetically by their name.</oldsource>
        <translation>&lt;b&gt;Position&lt;/b&gt;&lt;br /&gt;
Dieser Wert erlaubt es Ihnen die Reihenfolge der Kanäle innerhalb des Baumes festzulegen. Ein Kanal mit einem höheren &lt;i&gt;Positionswert&lt;/i&gt; wird immer unter einem Kanal mit einem niedrigeren &lt;i&gt;Positionswert&lt;/i&gt; platziert und umgekehrt. Wenn der &lt;i&gt;Positionswert&lt;/i&gt; zweier Kanäle gleich ist, dann werden diese ihrem Namen nach alphabetisch sortiert.</translation>
    </message>
    <message>
        <location/>
        <source>Members</source>
        <translation>Mitglieder</translation>
    </message>
    <message>
        <location/>
        <source>Contains a list of members whose group membership will not be inherited from the parent channel.</source>
        <translation>Enthält eine Liste von Mitgliedern, deren Gruppenmitgliedschaft nicht vom übergeordneten Kanal geerbt wird.</translation>
    </message>
    <message>
        <source>&lt;b&gt;Excluded members&lt;/b&gt;&lt;br&gt;
Contains a list of members whose group membership will not be inherited from the parent channel.</source>
        <translation type="obsolete">&lt;b&gt;Ausgeschlossene Mitglieder&lt;/b&gt;&lt;br&gt;
Enthält eine Liste von Mitgliedern, deren Gruppenmitgliedschaft nicht vom übergeordneten Kanal geerbt wird.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Add</source>
        <translation>&amp;Hinzufügen</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Remove</source>
        <translation>&amp;Entfernen</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Up</source>
        <translation>&amp;Hoch</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Down</source>
        <translation>&amp;Runter</translation>
    </message>
    <message>
        <location/>
        <source>Inherit ACLs</source>
        <translation>Berechtigungen erben</translation>
    </message>
    <message>
        <location/>
        <source>Applies to this channel</source>
        <translation>Betrifft diesen Kanal</translation>
    </message>
    <message>
        <location/>
        <source>Applies to sub-channels</source>
        <translation>Betrifft Unterkanäle</translation>
    </message>
    <message>
        <source>This controls which group of users this entry applies to.&lt;br /&gt;Note that the group is evaluated in the context of the channel the entry is used in. For example, the default ACL on the Root channel gives &lt;i&gt;Write&lt;/i&gt; permission to the &lt;i&gt;admin&lt;/i&gt; group. This entry, if inherited by a channel, will give a user write privileges if he belongs to the &lt;i&gt;admin&lt;/i&gt; group in that channel, even if he doesn&apos;t belong to the &lt;i&gt;admin&lt;/i&gt; group in the channel where the ACL originated.&lt;br /&gt;If a group name starts with a &apos;!&apos;, its membership is negated, and if it starts with a &apos;~&apos;, it is evaluated in the channel the ACL was defined in, rather than the channel the ACL is active in.&lt;br /&gt;If a group name starts with a  &apos;#&apos;, it is interpreted as an access token. Users must have entered whatever follows the &apos;#&apos; in their list of access tokens to match. This can be used for very simple password access to channels for non-authenticated users.&lt;br /&gt;If a group name starts with a  &apos;$&apos;, it will only match users whose certificate hash matches what follows the &apos;$&apos;.&lt;br /&gt;A few special predefined groups are:&lt;br /&gt;&lt;b&gt;all&lt;/b&gt; - Everyone will match.&lt;br /&gt;&lt;b&gt;auth&lt;/b&gt; - All authenticated users will match.&lt;br /&gt;&lt;b&gt;sub,a,b,c&lt;/b&gt; - User currently in a sub-channel minimum &lt;i&gt;a&lt;/i&gt; common parents, and between &lt;i&gt;b&lt;/i&gt; and &lt;i&gt;c&lt;/i&gt; channels down the chain. See the website for more extensive documentation on this one.&lt;br /&gt;&lt;b&gt;in&lt;/b&gt; - Users currently in the channel will match (convenience for &apos;&lt;i&gt;sub,0,0,0&lt;/i&gt;&apos;).&lt;br /&gt;&lt;b&gt;out&lt;/b&gt; - Users outside the channel will match (convenience for &apos;&lt;i&gt;!sub,0,0,0&lt;/i&gt;&apos;).&lt;br /&gt;Note that an entry applies to either a user or a group, not both.</source>
        <translation type="obsolete">Dieses Feld legt fest, für welche Gruppe dieser Eintrag gilt.&lt;br /&gt;Beachten Sie, dass diese Gruppe im Kontext des Kanals des Eintrags evaluiert wird. Zum Beispiel gibt die Standardberechtigung der admin-Gruppe im Wurzel-Kanal &lt;i&gt;Schreib&lt;/i&gt;rechte. Dieser Eintrag, wenn von einem Kanal geerbt, gibt einem Benutzer Schreibrechte wenn er in diesem Kanal zur &lt;i&gt;admin&lt;/i&gt;-Gruppe gehört, selbst wenn er nicht zur &lt;i&gt;admin&lt;/i&gt;-Gruppe des Kanals, in welchem die Berechtigung ursprünglich definiert wurde, gehört.&lt;br /&gt;Wenn ein Gruppenname mit &apos;!&apos; beginnt, werden seine Mitgliedschaften umgekehrt und wenn sie mit einem &apos;~&apos; beginnt werden die Mitgliedschaften im Kanal, in welchem die Berechtigung definiert wurde, geprüft statt in jenem, in dem die Berechtigung gerade aktiv ist.&lt;br /&gt;Wenn ein Gruppenname mit einem &apos;#&apos;-Zeichen beginnt wird dieser als Zugriffscode interpretiert. Benutzer müssen das was hinter dem &apos;#&apos;-Zeichen stehen in ihre Zugriffscode-Liste eingegeben haben um die entsprechende Berechtigung zu erhalten. Dies kann für einen einfachen Passwortschutz, auch für nicht authentifizierte Benutzer, verwendet werden.&lt;br /&gt;Wenn ein Gruppenname mit &apos;$&apos; beginnt gilt der Eintrag nur für Benutzer deren Zertifikat-Hash mit dem dem &apos;$&apos;-Zeichen folgenden übereinstimmt.&lt;br /&gt;Einige spezielle, vordefinierte Gruppen sind:&lt;br /&gt;&lt;b&gt;all&lt;/b&gt;: Trifft auf jeden Benutzer zu.&lt;br /&gt;&lt;b&gt;auth&lt;/b&gt;: Trifft auf alle authentifizierten Benutzer.&lt;br /&gt;&lt;b&gt;sub,a,b,c&lt;/b&gt;: Benutzer in einem Unterkanal mit mindestens &lt;i&gt;a&lt;/i&gt; gemeinsamen übgeordneten Kanälen und zwischen &lt;i&gt;b&lt;/i&gt; und &lt;i&gt;c&lt;/i&gt; Kanälen dazwischen (zwischen gemeinsamem übergeordneten Kanal und aktuellem Kanal). Auf der Webseite gibt es hierzu eine weiterführende Dokumentation.&lt;br /&gt;&lt;b&gt;in&lt;/b&gt;: Trifft auf Benutzer im aktuellen Kanal zu (Kurzform für &apos;&lt;i&gt;sub,0,0,0&lt;/i&gt;&apos;).&lt;br /&gt;&lt;b&gt;out&lt;/b&gt;: Trifft auf Benutzer die nicht im aktuellen Kanal sind zu (Kurzform für &apos;&lt;i&gt;!sub,0,0,0&lt;/i&gt;&apos;).&lt;br /&gt;Beachten Sie, dass ein Eintrag entweder für einen bestimmten Benutzer oder eine Gruppe zutrifft, nicht für beide gleichzeitig.</translation>
    </message>
    <message>
        <location/>
        <source>User ID</source>
        <translation>Benutzer ID</translation>
    </message>
    <message>
        <location filename="ACLEditor.cpp" line="+101"/>
        <source>Deny</source>
        <translation>Verweigern</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Allow</source>
        <translation>Erlauben</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui"/>
        <source>Remove</source>
        <translation>Entfernen</translation>
    </message>
    <message>
        <location/>
        <source>Inherit</source>
        <translation>Erben</translation>
    </message>
    <message>
        <location/>
        <source>Inheritable</source>
        <translation>Vererbbar</translation>
    </message>
    <message>
        <location/>
        <source>Inherited</source>
        <translation>Vererbt</translation>
    </message>
    <message>
        <location/>
        <source>Add</source>
        <translation>Hinzufügen</translation>
    </message>
    <message>
        <source>Add to Remove</source>
        <translation type="obsolete">Entfernen</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Groups</source>
        <translation>&amp;Gruppen</translation>
    </message>
    <message>
        <location/>
        <source>&amp;ACL</source>
        <translation>&amp;Berechtigungen</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>Accept changes</source>
        <translation type="obsolete">Änderungen übernehmen</translation>
    </message>
    <message>
        <source>This button will accept current groups/ACLs and send them to the server. Note that if you mistakenly remove write permission from yourself, the server will add it.</source>
        <translation type="obsolete">Dieser Button wird aktuelle Gruppen/ACLs akzeptieren und sie zum Server schicken. Falls man sich versehentlich Schreibrechte entzogen hat, wird der Server sie hinzufügen.</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Abbrechen</translation>
    </message>
    <message>
        <source>Reject changes</source>
        <translation type="obsolete">Änderungen verwerfen</translation>
    </message>
    <message>
        <source>This button will cancel all changes and closes the dialog without updating the ACLs or groups on the server.</source>
        <translation type="obsolete">Dieser Button macht alle Änderungen rückgängig und schließt den Dialog ohne ACLs oder Gruppen auf dem Server zu aktualisieren.</translation>
    </message>
    <message>
        <location/>
        <source>List of entries</source>
        <translation>Liste von Einträgen</translation>
    </message>
    <message>
        <source>This shows all the entries active on this channel. Entries inherited from parent channels will be show in italics.&lt;br /&gt;ACLs are evaluated top to bottom, meaning priority increases as you move down the list.</source>
        <translation type="obsolete">Dies Feld zeigt alle aktiven Einträge in diesem Kanal an. Von übergeordneten Kanälen geerbte Einträge werden kursiv angezeigt.&lt;br /&gt;Berechtigungen werden von oben nach unten ausgewertet. Das bedeutet, dass sich die Priorität nach unten hin erhöht.</translation>
    </message>
    <message>
        <location/>
        <source>Inherit ACL of parent?</source>
        <translation>Berechtigungen von übergeordneten Kanälen erben?</translation>
    </message>
    <message>
        <location/>
        <source>This sets whether or not the ACL up the chain of parent channels are applied to this object. Only those entries that are marked in the parent as &quot;Apply to sub-channels&quot; will be inherited.</source>
        <translation>Dieses Feld legt fest ob die Berechtigung am Anfang von übergeordneten Kanälen geerbt werden. Nur Einträge bei denen die Option &quot;Betrifft Unterkanäle&quot; markiert ist, werden geerbt.</translation>
    </message>
    <message>
        <location/>
        <source>Add new entry</source>
        <translation>Neuen Eintrag hinzufügen</translation>
    </message>
    <message>
        <location/>
        <source>This adds a new entry, initially set with no permissions and applying to all.</source>
        <translation>Dies fügt einen neuen Eintrag hinzu, der Anfangs alle Benutzer betrifft und keine Berechtigungen vergibt.</translation>
    </message>
    <message>
        <location/>
        <source>Remove entry</source>
        <translation>Eintrag entfernen</translation>
    </message>
    <message>
        <location/>
        <source>This removes the currently selected entry.</source>
        <translation>Dies entfernt den aktuell ausgewählten Eintrag.</translation>
    </message>
    <message>
        <location/>
        <source>Move entry up</source>
        <translation>Eintrag nach oben verschieben</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation type="obsolete">Eigenschaften</translation>
    </message>
    <message>
        <location/>
        <source>Name</source>
        <translation>Name</translation>
    </message>
    <message>
        <location/>
        <source>Enter the channel name here.</source>
        <translation>Geben Sie hier den Kanalnamen ein.</translation>
    </message>
    <message>
        <source>&lt;b&gt;Name&lt;/b&gt;&lt;br&gt;Enter the channel name in this field. The name has to comply with the restriction imposed by the server you are connected to.</source>
        <translation type="obsolete">&lt;b&gt;Name&lt;/b&gt;&lt;br&gt;Geben Sie den Kanalnamen in dieses Feld ein. Der Name muss von den Einschränkungen des aktuellen Servers erlaubt sein.</translation>
    </message>
    <message>
        <location/>
        <source>Description</source>
        <translation>Beschreibung</translation>
    </message>
    <message>
        <source>Enter the channel description here.</source>
        <translation type="obsolete">Kanalbeschreibung hier eingeben.</translation>
    </message>
    <message>
        <location/>
        <source>Password</source>
        <translation>Passwort</translation>
    </message>
    <message>
        <location/>
        <source>Enter the channel password here.</source>
        <translation>Geben Sie hier das Kanalpasswort ein.</translation>
    </message>
    <message>
        <location/>
        <source>Check to create a temporary channel.</source>
        <translation>Aktivieren Sie dieses Feld um einen temporären Kanal zu erstellen.</translation>
    </message>
    <message>
        <source>&lt;b&gt;Temporary&lt;/b&gt;&lt;br&gt;
When checked the channel created will be marked as temporary. This means when the last player leaves it the channel will be automatically deleted by the server.</source>
        <translation type="obsolete">&lt;b&gt;Temporär&lt;/b&gt;&lt;br/&gt;
Dieses Feld markiert den Kanal als temporär. Das heißt, dass er automatisch gelöscht wird, sobald der letzte Benutzer den Kanal verlässt.</translation>
    </message>
    <message>
        <location/>
        <source>Temporary</source>
        <translation>Temporär</translation>
    </message>
    <message>
        <location/>
        <source>Channel positioning facility value</source>
        <translation>Kanalpositionswert</translation>
    </message>
    <message>
        <location/>
        <source>Position</source>
        <translation>Position</translation>
    </message>
    <message>
        <source>&lt;b&gt;Group&lt;/b&gt;&lt;br&gt;
These are all the groups currently defined for the channel. To create a new group, just type in the name and press enter.</source>
        <oldsource>&lt;b&gt;Group&lt;/b&gt;&lt;br&gt;
This is all the groups currently defined for the channel. To create a new group, just type in the name and press enter.</oldsource>
        <translation type="obsolete">&lt;b&gt;Gruppe&lt;/b&gt;&lt;br/&gt;
Dies sind alle für diesen Kanal definierten Gruppen. Um eine neue Gruppe zu erstellen geben Sie einfach deren Namen ein und drücken Enter.</translation>
    </message>
    <message>
        <location/>
        <source>Add new group</source>
        <translation>Neue Gruppe hinzufügen</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Add&lt;/b&gt;&lt;br/&gt;
Add a new group.</source>
        <translation>&lt;b&gt;Hinzufügen&lt;/b&gt;&lt;br /&gt;
Fügt eine neue Gruppe hinzu.</translation>
    </message>
    <message>
        <source>&lt;b&gt;Remove&lt;/b&gt;&lt;br&gt;This removes the currently selected group. If the group was inherited, it will not be removed from the list, but all local information about the group will be cleared.</source>
        <translation type="obsolete">&lt;b&gt;Entfernen&lt;/b&gt;&lt;br/&gt;
Dies löscht die aktuell ausgewählte Gruppe. Wenn die Gruppe von einem übergeordneten Kanal geerbt wurde wird sie nicht aus der Liste gelöscht, aber alle lokalen Informationen für diese Gruppe werden entfernt.</translation>
    </message>
    <message>
        <source>&lt;b&gt;Inherit&lt;/b&gt;&lt;br&gt;This inherits all the members in the group from the parent, if the group is marked as &lt;i&gt;Inheritable&lt;/i&gt; in the parent channel.</source>
        <translation type="obsolete">&lt;b&gt;Erben&lt;/b&gt;&lt;br/&gt;
Dies erbt alle Mitglieder der Gruppe von übergeordneten Kanälen, sofern die Gruppe in übergeordneten Kanälen als &lt;i&gt;vererbbar&lt;/i&gt; markiert ist.</translation>
    </message>
    <message>
        <source>&lt;b&gt;Inheritable&lt;/b&gt;&lt;br&gt;This makes this group inheritable to sub-channels. If the group is non-inheritable, sub-channels are still free to create a new group with the same name.</source>
        <translation type="obsolete">&lt;b&gt;Vererbbar&lt;/b&gt;&lt;br/&gt;
Dies macht die Gruppe an Unterkanäle vererbbar. Wenn die Gruppe nicht vererbbar ist können die Unterkanäle trotzdem eine Gruppe mit dem gleichen Namen erstellen.</translation>
    </message>
    <message>
        <source>&lt;b&gt;Inherited&lt;/b&gt;&lt;br&gt;This indicates that the group was inherited from the parent channel. You cannot edit this flag, it&apos;s just for information.</source>
        <translation type="obsolete">&lt;b&gt;Vererbt&lt;/b&gt;&lt;br/&gt;
Dies gibt an, ob die Gruppe von einem übergeordneten Kanal geerbt wurde. Sie können diese Option hier nicht ändern, sie dient nur der Information.</translation>
    </message>
    <message>
        <location/>
        <source>Inherited members</source>
        <translation>Geerbte Mitglieder</translation>
    </message>
    <message>
        <location/>
        <source>Contains the list of members added to the group by this channel.</source>
        <translation>Enthält die Liste der zur Gruppe gehörenden Mitglieder in diesem Kanal.</translation>
    </message>
    <message>
        <source>&lt;b&gt;Members&lt;/b&gt;&lt;br&gt;
This list contains all members that were added to the group by the current channel. Be aware that this does not include members inherited by higher levels of the channel tree. These can be found in the &lt;i&gt;Inherited members&lt;/i&gt; list. To prevent this list to be inherited by lower level channels uncheck &lt;i&gt;Inheritable&lt;/i&gt; or manually add the members to the &lt;i&gt;Excluded members&lt;/i&gt; list.</source>
        <translation type="obsolete">&lt;b&gt;Mitglieder&lt;/b&gt;&lt;br/&gt;
Diese Liste enthält alle Mitglieder welche der Gruppe in diesem Kanal hinzugefügt wurden. Beachten Sie, dass dies nicht vererbte Mitglieder von höheren Kanälen einschließt. Diese werden in der Liste &lt;i&gt;Vererbte Mitglieder&lt;/i&gt; angezeigt. Um zu verhindern, dass diese Liste in Unterkanäle vererbt wird deaktivieren Sie die Option &lt;i&gt;Vererbbar&lt;/i&gt; oder fügen Sie die Mitglieder manuell zur Liste &lt;i&gt;Mitglieder ausschließen&lt;/i&gt; hinzu.</translation>
    </message>
    <message>
        <source>Contains a list of members whose group membership will not be inherited to subchannels.</source>
        <translation type="obsolete">Enthält eine Liste der Mitglieder, deren Gruppenmitgliedschaft nicht an die Unterkanäle vererbt wird.</translation>
    </message>
    <message>
        <location/>
        <source>Contains the list of members inherited by other channels.</source>
        <translation>Enthält die Liste der Mitglieder die von anderen Kanälen geerbt wurden.</translation>
    </message>
    <message>
        <source>&lt;b&gt;Inherited members&lt;/b&gt;&lt;br&gt;
Contains the list of members inherited by the current channel. Uncheck &lt;i&gt;Inherit&lt;/i&gt; to prevent inheritance from higher level channels.</source>
        <translation type="obsolete">&lt;b&gt;Geerbte Mitglieder&lt;/b&gt;&lt;br/&gt;
Enthält die Liste von Mitgliedern, welche vom aktuellen Kanal geerbt wurden. Deaktivieren Sie die Option &lt;i&gt;Erben&lt;/i&gt; um zu verhindern, dass von übergeordneten Kanälen geerbt wird.</translation>
    </message>
    <message>
        <location/>
        <source>Type in the name of a user you wish to add to the group and click Add.</source>
        <translation>Geben sie den Namen des Benutzers ein der zur Gruppe hinzugefügt werden soll und klicken sie auf Hinzufügen.</translation>
    </message>
    <message>
        <location/>
        <source>Type in the name of a user you wish to remove from the group and click Add.</source>
        <translation>Geben sie den Namen des Benutzers ein der aus der Gruppe entfernt werden soll und klicken sie auf Hinzufügen.</translation>
    </message>
    <message>
        <location/>
        <source>Exclude</source>
        <translation>Ausschließen</translation>
    </message>
    <message>
        <location/>
        <source>Excluded members</source>
        <translation>Ausgeschlossene Mitglieder</translation>
    </message>
    <message>
        <location/>
        <source>This moves the entry up in the list. As entries are evaluated in order, this may change the effective permissions of users. You cannot move an entry above an inherited entry, if you really need that you&apos;ll have to duplicate the inherited entry.</source>
        <translation>Dies verschiebt den Eintrag in der Liste nach oben. Da Einträge der Reihenfolge nach ausgewertet werden, könnte dies die effektiven Rechte der Benutzer ändern. Man kann keinen Eintrag über einen geerbten Eintrag verschieben. Sollte dies jedoch notwendig sein, so muss der Eintrag dupliziert werden.</translation>
    </message>
    <message>
        <location/>
        <source>Move entry down</source>
        <translation>Eintrag nach unten verschieben</translation>
    </message>
    <message>
        <location/>
        <source>This moves the entry down in the list. As entries are evaluated in order, this may change the effective permissions of users.</source>
        <translation>Dies verschiebt den Eintrag in der Liste nach unten. Da Einträge der Reihenfolge nach ausgewertet werden, könnte dies die effektiven Rechte der Benutzer ändern.</translation>
    </message>
    <message>
        <location/>
        <source>Entry should apply to this channel.</source>
        <translation>Eintrag soll für diesen Kanal gelten.</translation>
    </message>
    <message>
        <location/>
        <source>This makes the entry apply to this channel.</source>
        <translation>Dies lässt den Eintrag für diesen Kanal gelten.</translation>
    </message>
    <message>
        <location/>
        <source>Entry should apply to sub-channels.</source>
        <translation>Eintrag soll für Unterkanäle gelten.</translation>
    </message>
    <message>
        <location/>
        <source>This makes the entry apply to sub-channels of this channel.</source>
        <translation>Dies lässt den Eintrag für Unterkanäle gelten.</translation>
    </message>
    <message>
        <location/>
        <source>Group this entry applies to</source>
        <translation>Gruppe, für die dieser Eintrag gilt</translation>
    </message>
    <message>
        <source>This controls which group of users this entry applies to.&lt;br /&gt;Note that the group is evaluated in the context of the channel the entry is used in. For example, the default ACL on the Root channel gives &lt;i&gt;Write&lt;/i&gt; permission to the &lt;i&gt;admin&lt;/i&gt; group. This entry, if inherited by a channel, will give a user write privileges if he belongs to the &lt;i&gt;admin&lt;/i&gt; group in that channel, even if he doesn&apos;t belong to the &lt;i&gt;admin&lt;/i&gt; group in the channel where the ACL originated.&lt;br /&gt;If a group name starts with a !, its membership is negated, and if it starts with a ~, it is evaluated in the channel the ACL was defined in, rather than the channel the ACL is active in. Order is important; &lt;i&gt;!~in&lt;/i&gt; is valid, but &lt;i&gt;~!in&lt;/i&gt; is not.&lt;br /&gt;A few special predefined groups are:&lt;br /&gt;&lt;b&gt;all&lt;/b&gt; - Everyone will match.&lt;br /&gt;&lt;b&gt;auth&lt;/b&gt; - All authenticated users will match.&lt;br /&gt;&lt;b&gt;sub,a,b,c&lt;/b&gt; - User currently in a sub-channel minimum &lt;i&gt;a&lt;/i&gt; common parents, and between &lt;i&gt;b&lt;/i&gt; and &lt;i&gt;c&lt;/i&gt; channels down the chain. See the website for more extensive documentation on this one.&lt;br /&gt;&lt;b&gt;in&lt;/b&gt; - Users currently in the channel will match (convenience for &lt;i&gt;sub,0,0,0&lt;/i&gt;).&lt;br /&gt;&lt;b&gt;out&lt;/b&gt; - Users outside the channel will match (convenience for &lt;i&gt;!sub,0,0,0&lt;/i&gt;).&lt;br /&gt;Note that an entry applies to either a user or a group, not both.</source>
        <translation type="obsolete">Dies steuert welche Gruppe von Beutzern dieser Eintrag betrifft.&lt;br /&gt;Die Gruppen werden im Kontext des Kanals ausgewertet in dem sich der Eintrag befindet. Zum Beispiel gibt der Standard ACL in den Wurzelkanälen &lt;i&gt;Schreib&lt;/i&gt;-Rechte der &lt;i&gt;Admin&lt;/i&gt;-Gruppe. Dieser Eintrag gibt den Benutzern - wenn von einem Kanal geerbt - Schreibrecht, wenn er zur Gruppe der &lt;i&gt;Admin&lt;/i&gt;s in diesem Kanal gehört, selbst wenn er nicht zu der &lt;i&gt;Admin&lt;/i&gt;-Gruppe gehört woher der ACL eigentlich stammt.&lt;br /&gt;Beginnt ein Gruppenname mit einem &quot;!&quot;, wird die Mitgliedschaft negiert und wenn es mit einem &quot;~&quot; beginnt, wird es in dem Kanal ausgewertet in dem die ACL definiert wurde. Die Reihenfolge ist wichtig; &lt;i&gt;!~in&lt;/i&gt; ist gültig, &lt;i&gt;~!in&lt;/i&gt; jedoch nicht. Einige wenige spezielle vordefinierte Gruppen sind: &lt;br /&gt;&lt;b&gt;all&lt;/b&gt; - Jeder trifft zu.&lt;br /&gt;&lt;b&gt;auth&lt;/b&gt; - Alle authentifizierten Benutzer treffen zu.&lt;br /&gt;&lt;b&gt;in&lt;/b&gt; - Benutzer im Channel treffen zu.&lt;br /&gt;&lt;b&gt;out&lt;/b&gt; - Benutzer außerhalb des Kanals treffen zu.&lt;br /&gt;Ein Eintrag trifft dabei entweder auf einen Benutzer oder eine Gruppe zu, nicht auf beide.</translation>
    </message>
    <message>
        <location/>
        <source>User this entry applies to</source>
        <translation>Benutzer, für den der Eintrag gilt</translation>
    </message>
    <message>
        <location/>
        <source>This controls which user this entry applies to. Just type in the user name and hit enter to query the server for a match.</source>
        <translation>Dieses Feld steuert welchen Benutzer dieser Eintrag betrifft. Geben Sie einen Benutzernamen ein und drücken Sie Enter um die Existenz des Accounts auf dem Server zu überprüfen.</translation>
    </message>
    <message>
        <location filename="ACLEditor.cpp" line="+21"/>
        <source>Allow %1</source>
        <translation>%1 erlauben</translation>
    </message>
    <message>
        <location line="-8"/>
        <source>Deny %1</source>
        <translation>%1 ablehnen</translation>
    </message>
    <message>
        <location line="-62"/>
        <source>Mumble - Add channel</source>
        <translation>Mumble - Kanal hinzufügen</translation>
    </message>
    <message>
        <location line="+22"/>
        <location line="+165"/>
        <source>Failed: Invalid channel</source>
        <translation>Fehlgeschlagen: Ungültiger Kanal</translation>
    </message>
    <message>
        <location line="-153"/>
        <source>Mumble - Edit %1</source>
        <translation>Mumble - Bearbeite %1</translation>
    </message>
    <message>
        <location line="+37"/>
        <source>This grants the %1 privilege. If a privilege is both allowed and denied, it is denied.&lt;br /&gt;%2</source>
        <translation>Dies gibt das %1-Recht. Wenn ein Recht sowohl erlaubt als auch verweigert wird, so wird es verweigert.&lt;br /&gt;%2</translation>
    </message>
    <message>
        <location line="-8"/>
        <source>This revokes the %1 privilege. If a privilege is both allowed and denied, it is denied.&lt;br /&gt;%2</source>
        <translation>Dies nimmt das %1-Recht. Wenn ein Recht sowohl erlaubt als auch verweigert wird, so wird es verweigert.&lt;br /&gt;%2</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui"/>
        <source>List of groups</source>
        <translation>Liste von Gruppen</translation>
    </message>
    <message>
        <source>This is all the groups currently defined for the channel. To create a new group, just type in the name and press enter.</source>
        <translation type="obsolete">Dies sind alle aktuell im Kanal definierten Gruppen. Um eine neue Gruppe zu erstellen, den Namen eingeben und Enter drücken.</translation>
    </message>
    <message>
        <location/>
        <source>Remove selected group</source>
        <translation>Entferne ausgewählte Gruppe</translation>
    </message>
    <message>
        <source>This removes the currently selected group. If the group was inherited, it will not be removed from the list, but all local information about the group will be cleared.</source>
        <translation type="obsolete">Dies entfernt die momentan ausgewählte Gruppe. Wenn die Gruppe geerbt wurde wird sie nicht von der Liste entfernt, aber alle lokalen Informationen über diese Gruppe werden gelöscht.</translation>
    </message>
    <message>
        <location/>
        <source>Inherit group members from parent</source>
        <translation>Erbe Gruppenmitglieder von übergeordneten Kanälen</translation>
    </message>
    <message>
        <source>This inherits all the members in the group from the parent, if the group is marked as &lt;i&gt;Inheritable&lt;/i&gt; in the parent channel.</source>
        <translation type="obsolete">Dies erbt alle Mitglieder der Elterngruppe, wenn die Gruppe als &lt;i&gt;Vererbbar&lt;/i&gt; im Elternkanal markiert ist.</translation>
    </message>
    <message>
        <location/>
        <source>Make group inheritable to sub-channels</source>
        <translation>Erlaubt, dass Unterkanäle die Gruppe erben können</translation>
    </message>
    <message>
        <source>This makes this group inheritable to sub-channels. If the group is non-inheritable, sub-channels are still free to create a new group with the same name.</source>
        <translation type="obsolete">Dies macht die Gruppe zu Unterkanälen vererbbar. Ist die Gruppe nicht vererbbar können Unterkanäle Gruppen mit demselben Namen erstellen.</translation>
    </message>
    <message>
        <location/>
        <source>Group was inherited from parent channel</source>
        <translation>Gruppe wurde von einem übergeordneten Kanal geerbt</translation>
    </message>
    <message>
        <source>This indicates that the group was inherited from the parent channel. You cannot edit this flag, it&apos;s just for information.</source>
        <translation type="obsolete">Dies zeigt an, dass die Gruppe vom Elternkanal geerbt wurde. Dieses Attribut dient nur zur Information und kann nicht bearbeitet werden.</translation>
    </message>
    <message>
        <location/>
        <source>Add member to group</source>
        <translation>Mitglied zur Gruppe hinzufügen</translation>
    </message>
    <message>
        <source>Type in the name of a player you wish to add to the group and press enter.</source>
        <translation type="obsolete">Namen des Spielers eingeben, der zur Gruppe hinzugefügt werden soll.</translation>
    </message>
    <message>
        <location/>
        <source>Remove member from group</source>
        <translation>Entferne Mitglied aus der Gruppe</translation>
    </message>
    <message>
        <source>Type in the name of a player you wish to remove from the group and press enter.</source>
        <translation type="obsolete">Namen des Players eingeben, der aus der Gruppe entfernt werden soll.</translation>
    </message>
    <message>
        <source>Mumble - Edit ACL for %1</source>
        <translation type="obsolete">Mumble - ACL von %1 editieren</translation>
    </message>
    <message>
        <location/>
        <source>Dialog</source>
        <translation>Dialog</translation>
    </message>
    <message>
        <source>Type in the name of a player you wish to add to the group and click Add.</source>
        <translation type="obsolete">Namen des Benutzers, der zur Gruppe hinzugefügt werden soll, hier eingeben und Hinzufügen drücken.</translation>
    </message>
    <message>
        <source>Type in the name of a player you wish to remove from the group and click Add.</source>
        <translation type="obsolete">Namen des Benutzers, der aus der Gruppe entfernt werden soll, hier eingeben und Entfernen drücken.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Properties</source>
        <translation>&amp;Eigenschaften</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Name&lt;/b&gt;&lt;br /&gt;Enter the channel name in this field. The name has to comply with the restriction imposed by the server you are connected to.</source>
        <translation>&lt;b&gt;Name&lt;/b&gt;&lt;br /&gt;Geben Sie den Kanalnamen in dieses Feld ein. Der Name darf keine vom Server unerlaubten Zeichen enthalten.</translation>
    </message>
    <message>
        <source>&lt;b&gt;Password&lt;/b&gt;&lt;br /&gt;This field allows you to easily set and change the password of a channel. In the background it uses Mumble&apos;s access tokens feature. To allow more fine grained and powerful access control directly, use ACLs and groups instead (&lt;i&gt;Advanced configuration&lt;/i&gt; has to be checked to be able to see these settings).</source>
        <translation type="obsolete">&lt;b&gt;Passwort&lt;/b&gt;&lt;br /&gt;Dieses Feld erlaubt es Ihnen auf einfache Weise ein Passwort für den Kanal zu setzen und zu ändern. Im Hintergrund werden dazu die Zugriffscodes verwendet. Für noch genauere und feiner justierbarere Einstellungen verwenden Sie die ACLs und Gruppen.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Temporary&lt;/b&gt;&lt;br /&gt;
When checked the channel created will be marked as temporary. This means when the last player leaves it the channel will be automatically deleted by the server.</source>
        <translation>&lt;b&gt;Temporär&lt;/b&gt;&lt;br /&gt;
Dieses Feld markiert den Kanal als temporär. Das heißt, dass er automatisch gelöscht wird, sobald der letzte Benutzer den Kanal verlässt.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Group&lt;/b&gt;&lt;br /&gt;
These are all the groups currently defined for the channel. To create a new group, just type in the name and press enter.</source>
        <translation>&lt;b&gt;Gruppe&lt;/b&gt;&lt;br /&gt;
Dies sind alle für diesen Kanal definierten Gruppen. Um eine neue Gruppe zu erstellen geben Sie einfach deren Namen ein und drücken Enter.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Remove&lt;/b&gt;&lt;br /&gt;This removes the currently selected group. If the group was inherited, it will not be removed from the list, but all local information about the group will be cleared.</source>
        <translation>&lt;b&gt;Entfernen&lt;/b&gt;&lt;br /&gt;Dies löscht die aktuell ausgewählte Gruppe. Wenn die Gruppe von einem übergeordneten Kanal geerbt wurde wird sie nicht aus der Liste gelöscht, aber alle lokalen Informationen für diese Gruppe werden entfernt.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Inherit&lt;/b&gt;&lt;br /&gt;This inherits all the members in the group from the parent, if the group is marked as &lt;i&gt;Inheritable&lt;/i&gt; in the parent channel.</source>
        <translation>&lt;b&gt;Erben&lt;/b&gt;&lt;br /&gt;Dies erbt alle Mitglieder der Gruppe von übergeordneten Kanälen, sofern die Gruppe in übergeordneten Kanälen als &lt;i&gt;vererbbar&lt;/i&gt; markiert ist.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Inheritable&lt;/b&gt;&lt;br /&gt;This makes this group inheritable to sub-channels. If the group is non-inheritable, sub-channels are still free to create a new group with the same name.</source>
        <translation>&lt;b&gt;Vererbbar&lt;/b&gt;&lt;br /&gt;Dies macht die Gruppe an Unterkanäle vererbbar. Wenn die Gruppe nicht vererbbar ist können die Unterkanäle trotzdem eine Gruppe mit dem gleichen Namen erstellen.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Inherited&lt;/b&gt;&lt;br /&gt;This indicates that the group was inherited from the parent channel. You cannot edit this flag, it&apos;s just for information.</source>
        <translation>&lt;b&gt;Vererbt&lt;/b&gt;&lt;br /&gt;Dies gibt an, ob die Gruppe von einem übergeordneten Kanal geerbt wurde. Sie können diese Option hier nicht ändern, sie dient nur der Information.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Members&lt;/b&gt;&lt;br /&gt;
This list contains all members that were added to the group by the current channel. Be aware that this does not include members inherited by higher levels of the channel tree. These can be found in the &lt;i&gt;Inherited members&lt;/i&gt; list. To prevent this list to be inherited by lower level channels uncheck &lt;i&gt;Inheritable&lt;/i&gt; or manually add the members to the &lt;i&gt;Excluded members&lt;/i&gt; list.</source>
        <translation>&lt;b&gt;Mitglieder&lt;/b&gt;&lt;br /&gt;
Diese Liste enthält alle Mitglieder welche der Gruppe in diesem Kanal hinzugefügt wurden. Beachten Sie, dass dies nicht vererbte Mitglieder von höheren Kanälen einschließt. Diese werden in der Liste &lt;i&gt;Vererbte Mitglieder&lt;/i&gt; angezeigt. Um zu verhindern, dass diese Liste in Unterkanäle vererbt wird deaktivieren Sie die Option &lt;i&gt;Vererbbar&lt;/i&gt; oder fügen Sie die Mitglieder manuell zur Liste &lt;i&gt;Mitglieder ausschließen&lt;/i&gt; hinzu.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Excluded members&lt;/b&gt;&lt;br /&gt;
Contains a list of members whose group membership will not be inherited from the parent channel.</source>
        <translation>&lt;b&gt;Ausgeschlossene Mitglieder&lt;/b&gt;&lt;br /&gt;
Enthält eine Liste von Mitgliedern, deren Gruppenmitgliedschaft nicht vom übergeordneten Kanal geerbt wird.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Inherited members&lt;/b&gt;&lt;br /&gt;
Contains the list of members inherited by the current channel. Uncheck &lt;i&gt;Inherit&lt;/i&gt; to prevent inheritance from higher level channels.</source>
        <translation>&lt;b&gt;Geerbte Mitglieder&lt;/b&gt;&lt;br /&gt;
Enthält die Liste von Mitgliedern, welche vom aktuellen Kanal geerbt wurden. Deaktivieren Sie die Option &lt;i&gt;Erben&lt;/i&gt; um zu verhindern, dass von übergeordneten Kanälen geerbt wird.</translation>
    </message>
    <message>
        <location/>
        <source>This controls which group of users this entry applies to.&lt;br /&gt;Note that the group is evaluated in the context of the channel the entry is used in. For example, the default ACL on the Root channel gives &lt;i&gt;Write&lt;/i&gt; permission to the &lt;i&gt;admin&lt;/i&gt; group. This entry, if inherited by a channel, will give a user write privileges if he belongs to the &lt;i&gt;admin&lt;/i&gt; group in that channel, even if he doesn&apos;t belong to the &lt;i&gt;admin&lt;/i&gt; group in the channel where the ACL originated.&lt;br /&gt;If a group name starts with &apos;!&apos;, its membership is negated, and if it starts with &apos;~&apos;, it is evaluated in the channel the ACL was defined in, rather than the channel the ACL is active in.&lt;br /&gt;If a group name starts with &apos;#&apos;, it is interpreted as an access token. Users must have entered whatever follows the &apos;#&apos; in their list of access tokens to match. This can be used for very simple password access to channels for non-authenticated users.&lt;br /&gt;If a group name starts with &apos;$&apos;, it will only match users whose certificate hash matches what follows the &apos;$&apos;.&lt;br /&gt;A few special predefined groups are:&lt;br /&gt;&lt;b&gt;all&lt;/b&gt; - Everyone will match.&lt;br /&gt;&lt;b&gt;auth&lt;/b&gt; - All authenticated users will match.&lt;br /&gt;&lt;b&gt;sub,a,b,c&lt;/b&gt; - User currently in a sub-channel minimum &lt;i&gt;a&lt;/i&gt; common parents, and between &lt;i&gt;b&lt;/i&gt; and &lt;i&gt;c&lt;/i&gt; channels down the chain. See the website for more extensive documentation on this one.&lt;br /&gt;&lt;b&gt;in&lt;/b&gt; - Users currently in the channel will match (convenience for &apos;&lt;i&gt;sub,0,0,0&lt;/i&gt;&apos;).&lt;br /&gt;&lt;b&gt;out&lt;/b&gt; - Users outside the channel will match (convenience for &apos;&lt;i&gt;!sub,0,0,0&lt;/i&gt;&apos;).&lt;br /&gt;Note that an entry applies to either a user or a group, not both.</source>
        <translation>Dieses Feld legt fest, für welche Gruppe dieser Eintrag gilt.&lt;br /&gt;Beachten Sie, dass diese Gruppe im Kontext des Kanals des Eintrags evaluiert wird. Zum Beispiel gibt die Standardberechtigung der admin-Gruppe im Wurzel-Kanal &lt;i&gt;Schreib&lt;/i&gt;rechte. Dieser Eintrag, wenn von einem Kanal geerbt, gibt einem Benutzer Schreibrechte wenn er in diesem Kanal zur &lt;i&gt;admin&lt;/i&gt;-Gruppe gehört, selbst wenn er nicht zur &lt;i&gt;admin&lt;/i&gt;-Gruppe des Kanals, in welchem die Berechtigung ursprünglich definiert wurde, gehört.&lt;br /&gt;Wenn ein Gruppenname mit &apos;!&apos; beginnt, werden seine Mitgliedschaften umgekehrt und wenn sie mit einem &apos;~&apos; beginnt werden die Mitgliedschaften im Kanal, in welchem die Berechtigung definiert wurde, geprüft statt in jenem, in dem die Berechtigung gerade aktiv ist.&lt;br /&gt;Wenn ein Gruppenname mit einem &apos;#&apos;-Zeichen beginnt wird dieser als Zugriffscode interpretiert. Benutzer müssen das was hinter dem &apos;#&apos;-Zeichen stehen in ihre Zugriffscode-Liste eingegeben haben um die entsprechende Berechtigung zu erhalten. Dies kann für einen einfachen Passwortschutz, auch für nicht authentifizierte Benutzer, verwendet werden.&lt;br /&gt;Wenn ein Gruppenname mit &apos;$&apos; beginnt gilt der Eintrag nur für Benutzer deren Zertifikat-Hash mit dem dem &apos;$&apos;-Zeichen folgenden übereinstimmt.&lt;br /&gt;Einige spezielle, vordefinierte Gruppen sind:&lt;br /&gt;&lt;b&gt;all&lt;/b&gt;: Trifft auf jeden Benutzer zu.&lt;br /&gt;&lt;b&gt;auth&lt;/b&gt;: Trifft auf alle authentifizierten Benutzer.&lt;br /&gt;&lt;b&gt;sub,a,b,c&lt;/b&gt;: Benutzer in einem Unterkanal mit mindestens &lt;i&gt;a&lt;/i&gt; gemeinsamen übgeordneten Kanälen und zwischen &lt;i&gt;b&lt;/i&gt; und &lt;i&gt;c&lt;/i&gt; Kanälen dazwischen (zwischen gemeinsamem übergeordneten Kanal und aktuellem Kanal). Auf der Webseite gibt es hierzu eine weiterführende Dokumentation.&lt;br /&gt;&lt;b&gt;in&lt;/b&gt;: Trifft auf Benutzer im aktuellen Kanal zu (Kurzform für &apos;&lt;i&gt;sub,0,0,0&lt;/i&gt;&apos;).&lt;br /&gt;&lt;b&gt;out&lt;/b&gt;: Trifft auf Benutzer die nicht im aktuellen Kanal sind zu (Kurzform für &apos;&lt;i&gt;!sub,0,0,0&lt;/i&gt;&apos;).&lt;br /&gt;Beachten Sie, dass ein Eintrag entweder für einen bestimmten Benutzer oder eine Gruppe zutrifft, nicht für beide gleichzeitig.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Password&lt;/b&gt;&lt;br /&gt;This field allows you to easily set and change the password of a channel. It uses Mumble&apos;s access tokens feature in the background. Use ACLs and groups if you need more fine grained and powerful access control.</source>
        <translation>&lt;b&gt;Passwort&lt;/b&gt;&lt;br /&gt;Dieses Feld ermöglicht das Setzen und Entfernen eines Passworts für einen Kanal. Es verwendet im Hintergrund die Zugriffscode-Funktion von Mumble. Verwenden Sie Berechtigungen (ACL) und Gruppen für eine feiner abgestufte und mächtige Zugriffsteuerung.</translation>
    </message>
    <message>
        <location/>
        <source>This shows all the entries active on this channel. Entries inherited from parent channels will be shown in italics.&lt;br /&gt;ACLs are evaluated top to bottom, meaning priority increases as you move down the list.</source>
        <translation>Dieses Feld zeigt alle aktiven Einträge in diesem Kanal an. Von übergeordneten Kanälen geerbte Einträge werden kursiv angezeigt.&lt;br /&gt;Berechtigungen werden von oben nach unten ausgewertet. Das bedeutet, dass sich die Priorität nach unten hin erhöht.</translation>
    </message>
</context>
<context>
    <name>ALSAAudioInput</name>
    <message>
        <location filename="ALSAAudio.cpp" line="+360"/>
        <source>Opening chosen ALSA Input failed: %1</source>
        <translation>Öffnen des ALSA-Eingabegerätes fehlgeschlagen: %1</translation>
    </message>
    <message>
        <location line="-191"/>
        <source>Default ALSA Card</source>
        <translation>Standard ALSA-Karte</translation>
    </message>
</context>
<context>
    <name>ALSAAudioOutput</name>
    <message>
        <location line="+326"/>
        <source>Opening chosen ALSA Output failed: %1</source>
        <translation>Öffnen des ALSA-Ausgabegerätes fehlgeschlagen: %1</translation>
    </message>
    <message>
        <location line="-325"/>
        <source>Default ALSA Card</source>
        <translation>Standard ALSA-Karte</translation>
    </message>
</context>
<context>
    <name>ALSAConfig</name>
    <message>
        <source>Device selection</source>
        <translation type="obsolete">Geräteauswahl</translation>
    </message>
    <message>
        <source>Device to use for microphone</source>
        <translation type="obsolete">Mikrofon</translation>
    </message>
    <message>
        <source>This set which device mumble should use. The &lt;i&gt;default&lt;/i&gt; device is whatever you have configured in alsaconfig, the &lt;i&gt;hwplug&lt;/i&gt; devices are specific hardware devices backed by the ALSA mixer and the &lt;i&gt;hw&lt;/i&gt; devices are raw hardware access. Unless your soundcard supports hardware mixing of audio, using the &lt;i&gt;hw&lt;/i&gt; device will exclude all other programs from using audio.</source>
        <translation type="obsolete">Dies legt fest, welches Gerät Mumble nutzen soll. Das &lt;i&gt;Standard&lt;/i&gt;-Gerät ist durch alsaconfig konfiguriert, die &lt;i&gt;hwplug&lt;/i&gt; Geräte sind spezielle Hardware-Geräte die vom ALSA-Mixer genutzt werden und die &lt;i&gt;hw&lt;/i&gt;-Geräte bieten rohen Hardwarezugriff. Falls ihre Soundkarte Hardware Mixing von Audio nicht unterstützt, wird die Verwendung von &lt;i&gt;hw&lt;/i&gt;-Geräten alle anderen Programme vom Audiozugriff ausschließen.</translation>
    </message>
    <message>
        <source>Device to use for speakers/headphones</source>
        <translation type="obsolete">Gerät für Lautsprecher/Kopfhörer</translation>
    </message>
    <message>
        <source>Input</source>
        <translation type="obsolete">Eingang</translation>
    </message>
    <message>
        <source>Output</source>
        <translation type="obsolete">Ausgabe</translation>
    </message>
    <message>
        <source>Output Options</source>
        <translation type="obsolete">Ausgabeoptionen</translation>
    </message>
    <message>
        <source>Output Delay</source>
        <translation type="obsolete">Ausgabeverzögerung</translation>
    </message>
    <message>
        <source>Amount of data to buffer for ALSA</source>
        <translation type="obsolete">Menge an Daten die ALSA puffern soll</translation>
    </message>
    <message>
        <source>This sets the amount of data to prebuffer in the output buffer. Experiment with different values and set it to the lowest which doesn&apos;t cause rapid jitter in the sound.</source>
        <translation type="obsolete">Dies liegt die Menge an Daten fest, die im Ausgabepuffer vorgepuffert werden sollen. Experimentiere mit verschiedenen Werten und setze es auf den niedrigsten Wert, der noch kein schnelles Jitter im Sound verursacht.</translation>
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
        <translation type="obsolete">Formular</translation>
    </message>
    <message>
        <source>TextLabel</source>
        <translation type="obsolete">Textlabel</translation>
    </message>
    <message>
        <source>Default ALSA Card</source>
        <translation type="obsolete">Standard ALSA-Karten</translation>
    </message>
</context>
<context>
    <name>ASIOConfig</name>
    <message>
        <location filename="ASIOInput.ui"/>
        <source>Device selection</source>
        <translation>Geräteauswahl</translation>
    </message>
    <message>
        <location/>
        <source>Capabilities</source>
        <translation>Funktionalitäten</translation>
    </message>
    <message>
        <location/>
        <source>Buffer size</source>
        <translation>Puffergröße</translation>
    </message>
    <message>
        <location/>
        <source>This will configure the input channels for ASIO. Make sure you select at least one channel as microphone and speaker. &lt;i&gt;Microphone&lt;/i&gt; should be where your microphone is attached, and &lt;i&gt;Speaker&lt;/i&gt; should be a channel that samples &apos;&lt;i&gt;What you hear&lt;/i&gt;&apos;.&lt;br /&gt;For example, on the Audigy 2 ZS, a good selection for Microphone would be &apos;&lt;i&gt;Mic L&lt;/i&gt;&apos; while Speaker should be &apos;&lt;i&gt;Mix L&lt;/i&gt;&apos; and &apos;&lt;i&gt;Mix R&lt;/i&gt;&apos;.</source>
        <translation>Dies konfiguriert die Eingangskanäle für ASIO. Stellen Sie sicher, dass Sie mindestens einen Kanal für Mikrofon und Lautsprecher wählen. &lt;i&gt;Mikrofon&lt;/i&gt; sollte jener sein, wo an dem Ihr Mikrofon hängt und &lt;i&gt;Lautsprecher&lt;/b&gt; sollte ein Kanal sein, welcher ausgibt &apos;&lt;i&gt;was Sie hören&lt;/i&gt;&apos;.&lt;br /&gt;
Zum Beispiel wäre für die Audigy 2 ZS &apos;&lt;i&gt;Mic L&lt;/i&gt;&apos; eine gute Wahl für Mikrofon und &apos;&lt;i&gt;Mix L&lt;/i&gt;&apos; und &apos;&lt;i&gt;Mix R&lt;/i&gt;&apos; für Lautsprecher.</translation>
    </message>
    <message>
        <location/>
        <source>Channels</source>
        <translation>Kanäle</translation>
    </message>
    <message>
        <location/>
        <source>Configure input channels</source>
        <translation>Eingabekanäle konfigurieren</translation>
    </message>
    <message>
        <source>This will configure the input channels for ASIO. Make sure you select at least one channel as microphone and speaker. &lt;i&gt;Microphone&lt;/i&gt; should be where your microphone is attached, and &lt;i&gt;Speaker&lt;/i&gt; should be a channel that samples &quot;What you hear&quot;.&lt;br /&gt;For example, on the Audigy 2 ZS, a good selection for Microphone would be &quot;Mic L&quot; while Speaker should be &quot;Mix L&quot; and &quot;Mix R&quot;</source>
        <translation type="obsolete">Dies konfiguriert die Eingangskanäle für ASIO. Es muss sichergestellt werden, dass wenigstens ein Kanal als Mikrofon und Lautsprecher ausgewählt wurde. &lt;br /&gt;Eine gute Auswahl für eine Audigy 2 ZS wäre für Mikrofon &quot;Mic L&quot;, während es für die Lautsprecher &quot;Mix L&quot; und &quot;Mix R&quot; sein sollte</translation>
    </message>
    <message>
        <location/>
        <source>Device to use for microphone</source>
        <translation>Mikrofongerät</translation>
    </message>
    <message>
        <location/>
        <source>This chooses what device to query. You still need to actually query the device and select which channels to use.</source>
        <translation>Dies wählt das abzufragende Gerät aus. Man muss trotzdem das Gerät abfragen und die zu nutzenden Kanäle auswählen.</translation>
    </message>
    <message>
        <location/>
        <source>Device</source>
        <translation>Gerät</translation>
    </message>
    <message>
        <location/>
        <source>Query selected device</source>
        <translation>Gewähltes Gerät abfragen</translation>
    </message>
    <message>
        <location/>
        <source>This queries the selected device for channels. Be aware that many ASIO drivers are buggy to the extreme, and querying them might cause a crash of either the application or the system.</source>
        <translation>Dies fragt das ausgewählte Gerät nach Kanälen ab. Achtung: Viele ASIO-Treiber sind extrem fehlerbehaftet und sie abzufragen könnte die Anwendung oder das System zum Absturz bringen.</translation>
    </message>
    <message>
        <location/>
        <source>Configure selected device</source>
        <translation>Ausgewähltes Gerät konfigurieren</translation>
    </message>
    <message>
        <location/>
        <source>This configures the selected device. Be aware that many ASIO drivers are buggy to the extreme, and querying them might cause a crash of either the application or the system.</source>
        <translation>Dies konfiguriert das ausgewählte Gerät. Achtung: Viele ASIO-Treiber sind extrem fehlerbehaftet und sie abzufragen könnte die Anwendung oder das System zum Absturz bringen.</translation>
    </message>
    <message>
        <location/>
        <source>Driver name</source>
        <translation>Treibername</translation>
    </message>
    <message>
        <source>Buffersize</source>
        <translation type="obsolete">Puffergröße</translation>
    </message>
    <message>
        <location/>
        <source>Microphone</source>
        <translation>Mikrofon</translation>
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
        <translation>Ungenutzt</translation>
    </message>
    <message>
        <location/>
        <source>Speakers</source>
        <translation>Lautsprecher</translation>
    </message>
    <message>
        <location filename="ASIOInput.cpp" line="+222"/>
        <source>%1 (version %2)</source>
        <oldsource>%1 (ver %2)</oldsource>
        <translation>%1 (Version %2)</translation>
    </message>
    <message>
        <source>%1 ms -&gt; %2 ms (%3 ms resolution) %4Hz</source>
        <translation type="obsolete">%1 ms -&gt; %2 ms (%3 ms Auflösung) %4Hz</translation>
    </message>
    <message>
        <source>%1 ms -&gt; %2 ms (%3 ms resolution) %4Hz -- Unusable</source>
        <translation type="obsolete">%1 ms -&gt; %2 ms (%3 ms Auflösung) %4Hz -- Unbenutzbar</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>%1 -&gt; %2 samples buffer, with %3 sample resolution (%4 preferred) at %5 Hz</source>
        <oldsource>%1 -&gt; %2 samples buffer, with %3 sample resolution (%4 preferred) at %5Hz</oldsource>
        <translatorcomment>sample = Abtastung/Abtastrate, sample geläufig genug/einzige richtige Wahl?</translatorcomment>
        <translation>%1 -&gt; %2 Samples-Puffer mit %3 Sample-Auflösung (%4 bevorzugt) bei %5 Hz</translation>
    </message>
    <message>
        <source>Mumble</source>
        <translation type="obsolete">Mumble</translation>
    </message>
    <message>
        <location line="+43"/>
        <location line="+25"/>
        <source>ASIO Initialization failed: %1</source>
        <translation>ASIO-Initialisierung fehlgeschlagen: %1</translation>
    </message>
    <message>
        <location line="-21"/>
        <location line="+25"/>
        <source>Failed to instantiate ASIO driver</source>
        <translation>Konnte ASIO-Treiber nicht instanziieren</translation>
    </message>
    <message>
        <location line="+37"/>
        <source>ASIO</source>
        <translation>ASIO</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui"/>
        <source>&amp;Query</source>
        <translation>&amp;Abfragen</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Configure</source>
        <translation>&amp;Konfigurieren</translation>
    </message>
    <message>
        <location/>
        <source>Form</source>
        <translation>Formular</translation>
    </message>
    <message>
        <source>TextLabel</source>
        <translation type="obsolete">Textlabel</translation>
    </message>
    <message>
        <source>This will configure the input channels for ASIO. Make sure you select at least one channel as microphone and speaker. &lt;i&gt;Microphone&lt;/i&gt; should be where your microphone is attached, and &lt;i&gt;Speaker&lt;/i&gt; should be a channel that samples &lt;i&gt;What you hear&lt;/i&gt;.&lt;br /&gt;For example, on the Audigy 2 ZS, a good selection for Microphone would be &lt;i&gt;Mic L&lt;/i&gt; while Speaker should be &lt;i&gt;Mix L&lt;/i&gt; and &lt;i&gt;Mix R&lt;/i&gt;.</source>
        <translation type="obsolete">Dies konfiguriert die Eingangskanäle für ASIO. Es muss sichergestellt werden, dass wenigstens ein Kanal als Mikrofon und Lautsprecher ausgewählt wurden. &lt;i&gt;Mikrofon&lt;/i&gt; sollte das angeschlossene Mikrofon sein und &lt;i&gt;Lautsprecher&lt;/i&gt; sollte ein Kanal sein, der samplet &lt;i&gt;Was sie hören&lt;/i&gt;. &lt;br /&gt;Eine gute Auswahl für eine Audigy 2 ZS wäre für Mikrofon &lt;i&gt;Mic L&lt;/i&gt;, während es für die Lautsprecher &lt;i&gt;Mix L&lt;/i&gt; und &lt;i&gt;Mix R&lt;/i&gt; sein sollte.</translation>
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
        <translation>Es muss mindestens eine Mikrofon- und eine Lautsprecherquelle ausgewählt sein um ASIO zu nutzen. Wenn Sie lediglich den Mikrofoneingang nutzen wählen Sie DirectSound.</translation>
    </message>
    <message>
        <location line="+103"/>
        <source>Opening selected ASIO device failed. No input will be done.</source>
        <translation>Öffnen des ausgewählten ASIO-Gerätes fehlgeschlagen. Es wird kein Eingang benutzt.</translation>
    </message>
</context>
<context>
    <name>AboutDialog</name>
    <message>
        <source>&lt;h3&gt;Mumble v1.0.0&lt;/h3&gt;&lt;p&gt;Copyright 2005-2006 Thorvald Natvig&lt;br /&gt;slicer@users.sourceforge.net&lt;/p&gt;&lt;p&gt;&lt;b&gt;A voice-chat utility for gamers&lt;/b&gt;&lt;/p&gt;&lt;p&gt;&lt;tt&gt;http://mumble.sourceforge.net/&lt;/tt&gt;&lt;/p&gt;</source>
        <translation type="obsolete">&lt;h3&gt;Mumble v0.9.4&lt;/h3&gt;&lt;p&gt;Copyright 2005-2006 Thorvald Natvig&lt;br /&gt;slicer@users.sourceforge.net&lt;/p&gt;&lt;p&gt;Deutsche Übersetzung von Matthias Vogelgesang&lt;br /&gt; m0ta@users.sourceforge.net&lt;/p&gt;&lt;p&gt;&lt;b&gt;Ein Voicechat-Programm für Spieler&lt;/b&gt;&lt;/p&gt;&lt;p&gt;&lt;tt&gt;http://mumble.sourceforge.net/&lt;/tt&gt;&lt;/p&gt;</translation>
    </message>
    <message>
        <location filename="About.cpp" line="+63"/>
        <source>&amp;About Mumble</source>
        <translation>Ü&amp;ber Mumble</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>&amp;License</source>
        <translation>&amp;Lizenz</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>&lt;h3&gt;Mumble v%1&lt;/h3&gt;&lt;p&gt;Copyright 2005-2008 Thorvald Natvig&lt;br /&gt;slicer@users.sourceforge.net&lt;/p&gt;&lt;p&gt;&lt;b&gt;A voice-chat utility for gamers&lt;/b&gt;&lt;/p&gt;&lt;p&gt;&lt;tt&gt;http://mumble.sourceforge.net/&lt;/tt&gt;&lt;/p&gt;</source>
        <translation type="obsolete">&lt;h3&gt;Mumble v%1&lt;/h3&gt;&lt;p&gt;Copyright 2005-2007 Thorvald Natvig&lt;br /&gt;slicer@users.sourceforge.net&lt;/p&gt;&lt;p&gt;Deutsche Übersetzung von Matthias Vogelgesang&lt;br /&gt; m0ta@users.sourceforge.net&lt;/p&gt;&lt;p&gt;&lt;b&gt;Ein Voicechat-Programm für Spieler&lt;/b&gt;&lt;/p&gt;&lt;p&gt;&lt;tt&gt;http://mumble.sourceforge.net/&lt;/tt&gt;&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="-29"/>
        <source>About Mumble</source>
        <translation>Über Mumble</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>&lt;h3&gt;Mumble (%1)&lt;/h3&gt;&lt;p&gt;Copyright %3 Thorvald Natvig&lt;br /&gt;slicer@users.sourceforge.net&lt;/p&gt;&lt;p&gt;&lt;b&gt;A voice-chat utility for gamers&lt;/b&gt;&lt;/p&gt;&lt;p&gt;&lt;tt&gt;&lt;a href=&quot;%2&quot;&gt;%2&lt;/a&gt;&lt;/tt&gt;&lt;/p&gt;</source>
        <translation>&lt;h3&gt;Mumble (%1)&lt;/h3&gt;&lt;p&gt;Copyright %3 Thorvald Natvig&lt;br /&gt;slicer@users.sourceforge.net&lt;/p&gt;&lt;p&gt;Deutsche Übersetzung von Jan Klass und Matthias Vogelgesang&lt;br /&gt; m0ta@users.sourceforge.net&lt;/p&gt;&lt;p&gt;&lt;b&gt;Ein Voicechat-Programm für Spieler&lt;/b&gt;&lt;/p&gt;&lt;p&gt;&lt;tt&gt;&lt;a href=&quot;%2&quot;&gt;%2&lt;/a&gt;/&lt;/tt&gt;&lt;/p&gt;</translation>
    </message>
</context>
<context>
    <name>AboutSpeexDialog</name>
    <message>
        <location line="+25"/>
        <source>&lt;h3&gt;About Speex&lt;/h3&gt;&lt;p&gt;&lt;tt&gt;&lt;a href=&quot;%1&quot;&gt;%1&lt;/a&gt;&lt;/tt&gt;&lt;/p&gt;&lt;p&gt;This program uses SpeexDSP.&lt;/p&gt;&lt;p&gt;Speex is used for echo cancellation, noise&lt;br /&gt;filtering and voice activity detection.&lt;/p&gt;</source>
        <translation>&lt;h3&gt;Über Speex&lt;/h3&gt;&lt;p&gt;&lt;tt&gt;&lt;a href=&quot;%1&quot;&gt;%1&lt;/a&gt;&lt;/tt&gt;&lt;/p&gt;&lt;p&gt;Dieses Programm benutzt SpeexDSP.&lt;/p&gt;&lt;p&gt;Speex wird für die Echo-Auslöschung, Rausch-&lt;br /&gt;unterdrückung und die Stimmaktivierung verwendet.&lt;/p&gt;</translation>
    </message>
    <message>
        <source>&lt;h3&gt;About Speex&lt;/h3&gt;&lt;p&gt;&lt;tt&gt;http://www.speex.org/&lt;/tt&gt;&lt;/p&gt;&lt;p&gt;This program uses Speex version %1&lt;/p&gt;&lt;p&gt;Speex is used for echo cancellation, noise&lt;br /&gt;filtering, voice activity detection and speech&lt;br /&gt;compression.&lt;/p&gt;</source>
        <translation type="obsolete">&lt;h3&gt;Über Speex&lt;/h3&gt;&lt;p&gt;&lt;tt&gt;http://www.speex.org/&lt;/tt&gt;&lt;/p&gt;&lt;p&gt;Dieses Programm nutzt Speex Version %1&lt;/p&gt;&lt;p&gt;Speex wird zur Echoreduktion, Geräuschfilterung&lt;br /&gt;, Stimmaktivierungserkennung und&lt;br /&gt;Sprachkompression genutzt.&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <location line="-12"/>
        <source>About Speex</source>
        <translation>Über Speex</translation>
    </message>
    <message>
        <source>&lt;h3&gt;About Speex&lt;/h3&gt;&lt;p&gt;&lt;tt&gt;&lt;a href=&quot;%2&quot;&gt;%2&lt;/a&gt;&lt;/tt&gt;&lt;/p&gt;&lt;p&gt;This program uses Speex version %1&lt;/p&gt;&lt;p&gt;Speex is used for echo cancellation, noise&lt;br /&gt;filtering, voice activity detection and speech&lt;br /&gt;compression.&lt;/p&gt;</source>
        <translation type="obsolete">&lt;h3&gt;Über Speex&lt;/h3&gt;&lt;p&gt;&lt;tt&gt;&lt;a href=&quot;%2&quot;&gt;%2&lt;/a&gt;&lt;/tt&gt;&lt;/p&gt;&lt;p&gt;Dieses Programm nutzt Speex Version %1&lt;/p&gt;&lt;p&gt;Speex wird zur Echoreduktion, Rauschfilterung,&lt;br /&gt; Stimmaktivierungserkennung und&lt;br /&gt;Sprachkompression verwendet.&lt;/p&gt;</translation>
    </message>
</context>
<context>
    <name>AudioConfigDialog</name>
    <message>
        <source>Interfaces</source>
        <translation type="obsolete">Schnittstellen</translation>
    </message>
    <message>
        <source>Transmission</source>
        <translation type="obsolete">Übertragung</translation>
    </message>
    <message>
        <source>Compression</source>
        <translation type="obsolete">Kompression</translation>
    </message>
    <message>
        <source>Jitter Buffer</source>
        <translation type="obsolete">Jitterpuffer</translation>
    </message>
    <message>
        <source>Input</source>
        <translation type="obsolete">Eingang</translation>
    </message>
    <message>
        <source>Input method for audio</source>
        <translation type="obsolete">Eingabemethode für Audio</translation>
    </message>
    <message>
        <source>&lt;b&gt;This is the input method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation type="obsolete">&lt;b&gt;Dies ist die Eingabemethode für Audo.&lt;/b&gt;&lt;br /&gt;Am wahrscheinlichsten sollte DirectSound ausgewählt werden.</translation>
    </message>
    <message>
        <source>Output</source>
        <translation type="obsolete">Ausgabe</translation>
    </message>
    <message>
        <source>Output method for audio</source>
        <translation type="obsolete">Ausgabemethode für Audio</translation>
    </message>
    <message>
        <source>&lt;b&gt;This is the output method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation type="obsolete">&lt;b&gt;Dies ist die Ausgabemethode für Audio.&lt;/b&gt;&lt;br /&gt;Am wahrscheinlichsten sollte DirectSound ausgewählt werden.</translation>
    </message>
    <message>
        <source>Continuous</source>
        <translation type="obsolete">Kontinuierlich</translation>
    </message>
    <message>
        <source>Voice Activity</source>
        <translation type="obsolete">Stimmaktivierung</translation>
    </message>
    <message>
        <source>Push To Talk</source>
        <translation type="obsolete">Push-To-Talk</translation>
    </message>
    <message>
        <source>Transmit</source>
        <translation type="obsolete">Übertragen</translation>
    </message>
    <message>
        <source>When to transmit your speech</source>
        <translation type="obsolete">Art der Sprachübertragung</translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets when speech should be transmitted.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;Continuous&lt;/i&gt; - All the time&lt;br /&gt;&lt;i&gt;Voice Activity&lt;/i&gt; - When you are speaking clearly.&lt;br /&gt;&lt;i&gt;Push To Talk&lt;/i&gt; - When you hold down the hotkey set under &lt;i&gt;Shortcuts&lt;/i&gt;.</source>
        <translation type="obsolete">&lt;b&gt;Dies legt fest, wann Sprache übertragen werden soll.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;Kontinuierlich&lt;/i&gt; - Die ganze Zeit&lt;br /&gt;&lt;i&gt;Stimmaktivierung&lt;/i&gt; - Sobald man deutlich spricht.&lt;/br&gt;&lt;i&gt;Push-To-Talk&lt;/i&gt; - Wenn ein Hotkey gedrückt wird (siehe &lt;i&gt;Shortcuts&lt;/i&gt;).</translation>
    </message>
    <message>
        <source>Voice Hold</source>
        <translation type="obsolete">Stimme halten</translation>
    </message>
    <message>
        <source>How long to keep transmitting after silence</source>
        <translation type="obsolete">Wie lange nach Einsetzen der Stille übertragen werden soll</translation>
    </message>
    <message>
        <source>&lt;b&gt;This selects how long after a perceived stop in speech transmission should continue.&lt;/b&gt;&lt;br /&gt;Set this higher if your voice breaks up when you speak (seen by a rapidly blinking voice icon next to your name).&lt;br /&gt;Only makes sense when used with Voice Activity transmission.</source>
        <translation type="obsolete">&lt;b&gt;Dies wählt aus, wie lange nach Beenden des Gesprächs noch übertragen werden soll.&lt;/b&gt;&lt;br /&gt;Höhere Werte nutzen, wenn die Stimme plötzlich abbricht (erkennbar an einem flackerndem Voice-Icon neben dem Namen).&lt;br /&gt;Dies ist nur bei Stimmaktivierungs-Übertragung sinnvoll.</translation>
    </message>
    <message>
        <source>Audio per packet</source>
        <translation type="obsolete">Audio pro Paket</translation>
    </message>
    <message>
        <source>How many audio frames to send per packet</source>
        <translation type="obsolete">Wieviele Audioframes pro Paket zu senden sind</translation>
    </message>
    <message>
        <source>&lt;b&gt;This selects how many audio frames should be put in one packet.&lt;/b&gt;&lt;br /&gt;Increasing this will increase the latency of your voice, but will also reduce bandwidth requirements.</source>
        <translation type="obsolete">&lt;b&gt;Dies legt fest, wieviele Audioframes in ein Paket gepackt werden sollen.&lt;/b&gt;&lt;br /&gt;Dies zu erhöhen, steigert auch die Latenz der Sprache, senkt aber ebenso Bandbreitenbedarf.</translation>
    </message>
    <message>
        <source>Use TCP mode</source>
        <translation type="obsolete">Benutze TCP-Modus</translation>
    </message>
    <message>
        <source>Use TCP Compatibility mode</source>
        <translation type="obsolete">Benutze TCP-Kompatibilitätsmodus</translation>
    </message>
    <message>
        <source>&lt;b&gt;Enable TCP Compatibility mode&lt;/b&gt;.&lt;br /&gt;This will make Mumble use only TCP when communicating with the server. This will increase overhead and cause lost packets to produce noticeable pauses in communication, so this should only be used if you are unable to use the default (which uses UDP for voice and TCP for control).</source>
        <translation type="obsolete">&lt;b&gt;TCP-Kompatibilitätsmodus aktivieren&lt;/b&gt;. &lt;br /&gt;Dies lässt Mumble nur TCP zu Kommunikation mit dem Server nutzen. Es wird den Overhead erhöhen und bei verlorenen Paketen hörbare Pausen produzieren. Es wird nur empfohlen wenn die Standardeinstellungen nicht funktioniert.</translation>
    </message>
    <message>
        <source>Automatic Reconnect</source>
        <translation type="obsolete">Automatisches Neuverbinden</translation>
    </message>
    <message>
        <source>Reconnect when disconnected</source>
        <translation type="obsolete">Bei Unterbrechung erneut verbinden</translation>
    </message>
    <message>
        <source>&lt;b&gt;Reconnect when disconnected&lt;/b&gt;.&lt;br /&gt;This will make Mumble try to automatically reconnect after 10 seconds if your server connection fails.</source>
        <translation type="obsolete">&lt;b&gt;Neuverbinden nach Verbindungsabbruch&lt;/b&gt;.&lt;br /&gt;Dies führt eine automatische Neuverbindung nach 10 Sekunden durch, wenn die Serververbindung abbricht.</translation>
    </message>
    <message>
        <source>Maximum bandwidth used for sent audio</source>
        <translation type="obsolete">Maximale Bandbreite für Audiodaten</translation>
    </message>
    <message>
        <source>&lt;b&gt;This shows peak outgoing bandwidth used.&lt;/b&gt;&lt;br /&gt;This shows the peak amount of bandwidth sent out from your machine. Audio bitrate is the maximum bitrate (as we use VBR) for the audio data alone. Position is the bitrate used for positional information. Overhead is our framing and the IP packet headers (IP and UDP is 75% of this overhead).</source>
        <translation type="obsolete">&lt;b&gt;Dies zeigt die Spitzenbandbreite für Ausgang an.&lt;/b&gt;&lt;br /&gt;Dies zeigt die maximale Menge an Bandbreite zum Senden an. Audio Bitrate ist die maximale Bitrate (da VBR genutzt wird) nur für die Audio Daten. Position ist die Bitrate für Positionsinformationen. Overhead ist das Framing und die IP Paketheader (TCP/IP und UPD machen75% des Overhead aus).</translation>
    </message>
    <message>
        <source>Outgoing Bitrate</source>
        <translation type="obsolete">Ausgehende Bitrate</translation>
    </message>
    <message>
        <source>Quality</source>
        <translation type="obsolete">Qualität</translation>
    </message>
    <message>
        <source>Quality of compression (peak bandwidth)</source>
        <translation type="obsolete">Qualität der Kompression (Spitzenbandbreite)</translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets the quality of compression.&lt;/b&gt;&lt;br /&gt;This determines how much bandwidth Mumble is allowed to use for outgoing audio.</source>
        <translation type="obsolete">&lt;b&gt;Dies setzt die Qualität der Kompression.&lt;/b&gt;&lt;br /&gt;Dies bestimmt wieviel Bandbreite Mumble für ausgehendes Audio nutzen darf.</translation>
    </message>
    <message>
        <source>Complexity</source>
        <translation type="obsolete">Komplexität</translation>
    </message>
    <message>
        <source>Complexity of compression (CPU)</source>
        <translation type="obsolete">Komplexität der Kompression (CPU)</translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets the complexity of compression.&lt;/b&gt;&lt;br /&gt;This determines how much CPU Mumble is allowed to use to increase transmitted voice quality. Settings above 5 give only marginal gain.</source>
        <translation type="obsolete">&lt;b&gt;Dies setzt die Komplexität der Kompression.&lt;/b&gt;&lt;br /&gt;Es bestimmt wieviel CPU-Zeit Mumble nutzen darf um die übertragene Sprachqualität zu steigern. Einstellungen über 5 bringen nur minimale Verbesserungen.</translation>
    </message>
    <message>
        <source>Amp</source>
        <translation type="obsolete">Verstärkung</translation>
    </message>
    <message>
        <source>Maximum amplification of input sound</source>
        <translation type="obsolete">Maximale Verstärkung des Eingangssounds</translation>
    </message>
    <message>
        <source>&lt;b&gt;Maximum amplification of input.&lt;/b&gt;&lt;br /&gt;Mumble normalizes the input volume before compressing, and this sets how much it&apos;s allowed to amplify.&lt;br /&gt;The actual level is continually updated based on your current speech pattern, but it will never go above the level specified here.&lt;br /&gt;If the &lt;i&gt;Microphone loudness&lt;/i&gt; level of the audio statistics hover around 100%, you probably want to set this to 2.0 or so, but if, like most people, you are unable to reach 100%, set this to something much higher.&lt;br /&gt;Ideally, set it so &lt;i&gt;Microphone Loudness * Amplification Factor &gt;= 100&lt;/i&gt;, even when you&apos;re speaking really soft.&lt;br /&gt;&lt;br /&gt;Note that there is no harm in setting this to maximum, but Mumble will start picking up other conversations if you leave it to auto-tune to that level.</source>
        <translation type="obsolete">&lt;b&gt;Maximale Verstärkung des Eingangs.&lt;/b&gt;&lt;br /&gt;Mumble normalisiert die Eingangslautstärke vor der Kompression, wobei dies einstellt wie sehr verstärkt werden darf.&lt;br /&gt;Der tatsächliche Level wird kontinuierlich, vom Sprachmuster abhängig, aktualisiert; allerdings nie höher als hier festgelegt.&lt;br /&gt;Wenn die Mikrofonlautstärke in den Audiostatistiken um 100% liegt, sollte man dies auf 2.0 setzen. Für Leute die dies kaum erreichen, muss es deutlich höher angesetzt werden.&lt;br /&gt;Idealerweise sollte es folgendermaßen gesetzt werden: &lt;i&gt;Mikrofon Lautstärke * Verstärkungsfaktor &gt;= 100&lt;/i&gt;, selbst wenn man wirklich leise spricht.&lt;br /&gt;Es ist nicht schädlich dies auf das Maximum zu setzen, aber Mumble wird auch Umgebungsgeräusche aufnehmen.</translation>
    </message>
    <message>
        <source>Default Jitter Buffer</source>
        <translation type="obsolete">Standard Jitter-Puffer</translation>
    </message>
    <message>
        <source>How long to prebuffer on first packet</source>
        <translation type="obsolete">Wie lange beim ersten Paket vorzupuffern</translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets the default buffer size for the jitter buffer&lt;/b&gt;.&lt;br /&gt;All incoming audio is buffered, and the jitter buffer continually tries to push the buffer to the minimum sustainable by your network, so latency can be as low as possible. This sets the default buffer size to use on the first packet to arrive from a new speaker, or when a speaker using Voice Activity or Push-To-Talk just started talking again. If the start of sentences you hear is very jittery, increase this value.</source>
        <translation type="obsolete">&lt;b&gt;Dies legt die Standard-Puffergröße für den Jitterpuffer fest&lt;/b&gt;.&lt;br /&gt;Ankommende Audiodaten werden gepuffert und der Jitterpuffer versucht ständig diesen Puffer so schnell wie möglich ans Netzwerk zu übertragen, um die Latenz zu verringern.</translation>
    </message>
    <message>
        <source>Basic Audio</source>
        <translation type="obsolete">Grundeinstellungen</translation>
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
        <translation type="obsolete">%1 kbit/2 (Audio %2, Position %4, Overhead %3)</translation>
    </message>
    <message>
        <source>Loopback Test</source>
        <translation type="obsolete">Loopback Test</translation>
    </message>
    <message>
        <source>PTT Audio cue</source>
        <translation type="obsolete">PTT Audio Hinweis</translation>
    </message>
    <message>
        <source>Audible audio cue when push-to-talk pressed and released</source>
        <translation type="obsolete">Hörbarer Audio Hinweis, wenn Push-To-Talk gedrückt und losgelassen wird</translation>
    </message>
    <message>
        <source>&lt;b&gt;This enables the audio cues for push to talk.&lt;/b&gt;&lt;br /&gt;Setting this will give you a short audio beep when push to talk is pressed and released.</source>
        <translation type="obsolete">&lt;b&gt;Dies aktiviert die Audio Hinweise für Push-To-Talk.&lt;/b&gt;&lt;br /&gt;Wenn dies gesetzt wird, gibt es einen kurzen Piepton wenn Push-To-Talk gedrückt und losgelassen wird.</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Keiner</translation>
    </message>
    <message>
        <source>Local</source>
        <translation type="obsolete">Lokal</translation>
    </message>
    <message>
        <source>Server</source>
        <translation type="obsolete">Server</translation>
    </message>
    <message>
        <source>Loopback</source>
        <translation type="obsolete">Loopback</translation>
    </message>
    <message>
        <source>Desired loopback mode</source>
        <translation type="obsolete">Gewünschter Loopback-Modus</translation>
    </message>
    <message>
        <source>&lt;b&gt;This enables one of the loopback testmodes.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;None&lt;/i&gt; - Loopback disabled&lt;br /&gt;&lt;i&gt;Local&lt;/i&gt; - Emulate a local server.&lt;br /&gt;&lt;i&gt;Server&lt;/i&gt; - Request loopback from server.&lt;br /&gt;Please note than when loopback is enabled, no other players will hear your voice. This setting is not saved on application exit.</source>
        <translation type="obsolete">&lt;b&gt;Dies aktiviert einen der Loopback Testmodi.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;Keiner&lt;/i&gt; - Loopback deaktiviert&lt;br /&gt;&lt;i&gt;Lokal&lt;/i&gt; - Emuliert einen lokalen Server.&lt;br /&gt;&lt;i&gt;Server&lt;/i&gt; - Fordere Loopback vom Server.&lt;br /&gt;Beachten Sie, dass Sie niemand bei aktiviertem Loopback hören kann. Diese Einstellung wird beim Beenden der Anwendung nicht gespeichert.</translation>
    </message>
    <message>
        <source>Delay Variance</source>
        <translation type="obsolete">Verzögerungsvarianz</translation>
    </message>
    <message>
        <source>Variance in packet latency</source>
        <translation type="obsolete">Varianz der Packetlatenz</translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets the packet latency variance for loopback testing&lt;/b&gt;&lt;br /&gt;Most audio paths contain some variable latency. This allows you set that variance for loopback mode testing. For example, if you set this to 15ms, this will emulate a network with 20-35ms ping latency or one with 80-95ms latency. Most domestic net connections have a variance of about 5ms</source>
        <translation type="obsolete">&lt;b&gt;Dies legt die Varianz der Paketlatenz für den Loopback Test fest&lt;/b&gt;&lt;br /&gt;Die meisten Audiopfade haben eine variable Latenz. Hiermit können Sie die Varianz für den Loopback Test festlegen. Falls Sie dies zum Beispiel auf 15ms setzen, wird ein Netzwerk mit 20-35ms oder eines mit 80-95ms Latenz emuliert. Die meisten Netzverbindungen haben eine Varianz von etwa 5ms</translation>
    </message>
    <message>
        <source>Packet Loss</source>
        <translation type="obsolete">Packet Loss</translation>
    </message>
    <message>
        <source>Packet loss for loopback mode</source>
        <translation type="obsolete">Packet loss für Loopback Modus</translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets the packet loss for loopback mode.&lt;/b&gt;&lt;br /&gt;This will be the ratio of packets lost. Unless your outgoing bandwidth is peaked or there&apos;s something wrong with your network connection, this will be 0%%</source>
        <translation type="obsolete">&lt;b&gt;Dies legt den Packet Loss für den Loopback Modus fest.&lt;/b&gt;&lt;br /&gt;Dies ist das Verhältnis an verlorenen Paketen. Es wird fast immer 0%% betragen, es sei den sie haben ein Bandbreitenproblem oder ihr Netzwerk ist nicht korrekt konfiguriert</translation>
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
        <source>-%1 dB</source>
        <translation type="obsolete">-%1 dB</translation>
    </message>
    <message>
        <source>Form</source>
        <translation type="obsolete">Form</translation>
    </message>
    <message>
        <source>&amp;Input</source>
        <translation type="obsolete">&amp;Eingang</translation>
    </message>
    <message>
        <source>&amp;Output</source>
        <translation type="obsolete">&amp;Ausgang</translation>
    </message>
    <message>
        <source>&amp;Transmit</source>
        <translation type="obsolete">Über&amp;tragen</translation>
    </message>
    <message>
        <source>Voice &amp;Hold</source>
        <translation type="obsolete">Stimme &amp;halten</translation>
    </message>
    <message>
        <source>TextLabel</source>
        <translation type="obsolete">Textlabel</translation>
    </message>
    <message>
        <source>&amp;Quality</source>
        <translation type="obsolete">&amp;Qualität</translation>
    </message>
    <message>
        <source>&amp;Complexity</source>
        <translation type="obsolete">&amp;Komplexität</translation>
    </message>
    <message>
        <source>Audio Processing</source>
        <translation type="obsolete">Audioprocessing</translation>
    </message>
    <message>
        <source>Noise Suppression</source>
        <translation type="obsolete">Rauschunterdrückung</translation>
    </message>
    <message>
        <source>Noise suppression</source>
        <translation type="obsolete">Rauschunterdrückung</translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets the ammount of noise suppression to apply.&lt;/b&gt;&lt;br /&gt;The higher this value, the more aggressively stationary noise will be supressed.</source>
        <translation type="obsolete">&lt;b&gt;Dies legt die Menge an Rauschunterdrückung fest.&lt;/b&gt;&lt;br /&gt;Je höher dieser Wert ist, desto aggressiver werden stationäre Geräusche unterdrückt.</translation>
    </message>
    <message>
        <source>Amplification</source>
        <translation type="obsolete">Verstärkung</translation>
    </message>
    <message>
        <source>Default &amp;Jitter Buffer</source>
        <translation type="obsolete">Standard &amp;Jitter Puffer</translation>
    </message>
    <message>
        <source>Safety margin for jitter buffer</source>
        <translation type="obsolete">Sichere Grenze für Jitterpuffer</translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets the minimum safety margin for the jitter buffer.&lt;/b&gt;&lt;br /&gt;All incoming audio is buffered, and the jitter buffer continually tries to push the buffer to the minimum sustainable by your network, so latency can be as low as possible. This sets the minimum buffer size to use. If the start of sentences you hear is very jittery, increase this value.</source>
        <translation type="obsolete">&lt;b&gt;Dies legt die minimale Sicherheitsgrenze für den Jitterpuffer fest.&lt;/b&gt;&lt;br /&gt;Jedes eingehende Audio wird gepuffert und der Jitterpuffer versucht den Puffer so klein wie möglich zu halten so dass die Latenz so gering wie möglich sein kann. Dies legt die minimale Puffergröße fest. Falls sich der Anfang eines Satzes kaputt anhört, erhöhen Sie diesen Wert.</translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets the packet loss for loopback mode.&lt;/b&gt;&lt;br /&gt;This will be the ratio of packets lost. Unless your outgoing bandwidth is peaked or there&apos;s something wrong with your network connection, this will be 0%</source>
        <translation type="obsolete">&lt;b&gt;Dies legt den Packet Loss für den Loopback Modus fest.&lt;/b&gt;&lt;br /&gt;Dies ist das Verhältnis an verlorenen Paketen. Außer ihre Ausgangsbandbreite stößt an ihre Grenze oder etwas stimmt nicht mit Ihrer Netzwerkverbindung, so wird dies 0% betragen</translation>
    </message>
    <message>
        <source>&amp;Loopback</source>
        <translation type="obsolete">&amp;Loopback</translation>
    </message>
    <message>
        <source>Audio Output</source>
        <translation type="obsolete">Audioausgabe</translation>
    </message>
    <message>
        <source>Volume</source>
        <translation type="obsolete">Lautstärke</translation>
    </message>
    <message>
        <source>Volume of incoming speech</source>
        <translation type="obsolete">Lautstärke der ankommenden Sprache</translation>
    </message>
    <message>
        <source>&lt;b&gt;This adjusts the volume of incoming speech.&lt;/b&gt;&lt;br /&gt;Note that this can only be used to decrease the volume. No amplification is possible.</source>
        <translation type="obsolete">&lt;b&gt;Dies legt die Lautstärke der eingehenden Sprache fest.&lt;/b&gt;&lt;br /&gt;Beachten Sie, dass dies nur genutzt werden kann, um die Lautstärke zu verringern. Eine Verstärkung ist nicht möglich.</translation>
    </message>
    <message>
        <source>Off</source>
        <translation type="obsolete">Aus</translation>
    </message>
    <message>
        <source>DoublePush Time</source>
        <translation type="obsolete">DoublePush Zeit</translation>
    </message>
</context>
<context>
    <name>AudioInput</name>
    <message>
        <location filename="AudioInput.ui"/>
        <source>Form</source>
        <translation>Formular</translation>
    </message>
    <message>
        <location/>
        <source>Interface</source>
        <translation>Schnittstelle</translation>
    </message>
    <message>
        <location/>
        <source>System</source>
        <translation>System</translation>
    </message>
    <message>
        <location/>
        <source>Input method for audio</source>
        <translation>Audio-Eingabemethode</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is the input method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation>Dies ist die zu verwendende Audio-Eingabemethode.</translation>
    </message>
    <message>
        <location/>
        <source>Device</source>
        <translation>Gerät</translation>
    </message>
    <message>
        <location/>
        <source>Input device for audio</source>
        <translation>Audio-Eingabegerät</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is the input device to use for audio.&lt;/b&gt;</source>
        <translation>&lt;b&gt;Dies ist das zu verwendende Audio-Eingabegerät.&lt;/b&gt;</translation>
    </message>
    <message>
        <source>Cancel Echo</source>
        <translation type="obsolete">Echo auslöschen</translation>
    </message>
    <message>
        <location/>
        <source>Transmission</source>
        <translation>Übertragung</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Transmit</source>
        <translation>Über&amp;tragen</translation>
    </message>
    <message>
        <location/>
        <source>When to transmit your speech</source>
        <translation>Wann Sprache übertragen werden soll</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets when speech should be transmitted.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;Continuous&lt;/i&gt; - All the time&lt;br /&gt;&lt;i&gt;Voice Activity&lt;/i&gt; - When you are speaking clearly.&lt;br /&gt;&lt;i&gt;Push To Talk&lt;/i&gt; - When you hold down the hotkey set under &lt;i&gt;Shortcuts&lt;/i&gt;.</source>
        <translation>&lt;b&gt;Dies legt fest, wann Sprache übertragen werden soll.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;Kontinuierlich&lt;/i&gt; - Die ganze Zeit&lt;br /&gt;&lt;i&gt;Stimmaktivierung&lt;/i&gt; - Sobald man deutlich spricht.&lt;/br&gt;&lt;i&gt;Push-To-Talk&lt;/i&gt; - Wenn ein Hotkey gedrückt wird (siehe &lt;i&gt;Shortcuts&lt;/i&gt;).</translation>
    </message>
    <message>
        <location/>
        <source>DoublePush Time</source>
        <translation>Doppeldruck-Zeit</translation>
    </message>
    <message>
        <source>TextLabel</source>
        <translation type="obsolete">Textlabel</translation>
    </message>
    <message>
        <source>Audible audio cue when push-to-talk pressed and released</source>
        <translation type="obsolete">Hörbarer Audio Hinweis, wenn Push-To-Talk gedrückt und losgelassen wird</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This enables the audio cues for push to talk.&lt;/b&gt;&lt;br /&gt;Setting this will give you a short audio beep when push to talk is pressed and released.</source>
        <translation>&lt;b&gt;Dies aktiviert die Push-To-Talk Audio-Hinweise.&lt;/b&gt;&lt;br /&gt;Wenn diese Option aktiviert ist, gibt es wird der eingestellte Ton abgespielt wenn Push-To-Talk gedrückt und losgelassen wird.</translation>
    </message>
    <message>
        <location/>
        <source>PTT Audio cue</source>
        <translation>PTT Audio-Hinweis</translation>
    </message>
    <message>
        <location/>
        <source>Use SNR based speech detection</source>
        <translation>Benutze SNR basierte Spracherkennung</translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets speech detection to use Signal to Noise ratio.&lt;/b&gt;&lt;br /&gt;In this mode, the input is analyzed for something resembling a clear singal, and the clarity of that signal is used to trigger speech detection.</source>
        <translation type="obsolete">&lt;b&gt;Dies legt fest, dass Sprache durch ein Signal-Rausch-Verhältnis erkannt wird.&lt;/b&gt;&lt;br /&gt;In diesem Modus wird die Eingabe auf ein klares Signal hin analysiert und die Klarheit des Signals für die Spracherkennung genutzt.</translation>
    </message>
    <message>
        <location/>
        <source>Signal to Noise</source>
        <translation>Signal zu Rauschen</translation>
    </message>
    <message>
        <source>Use Amplitude based speech detection.</source>
        <translation type="obsolete">Benutze amplitudenbasierte Spracherkennung.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets speech detection to use Amplitude.&lt;/b&gt;&lt;br /&gt;In this mode, the raw strength of the input signal is used to detect speech.</source>
        <translation>&lt;b&gt;Dies legt fest, dass Sprache durch die Amplitude erkannt wird.&lt;/b&gt;&lt;br /&gt;In diesem Modus wird die Stärke des Eingangssignals genutzt um Sprache zu erkennen.</translation>
    </message>
    <message>
        <location/>
        <source>Amplitude</source>
        <translation>Amplitude</translation>
    </message>
    <message>
        <location/>
        <source>Voice &amp;Hold</source>
        <translation>Stimme &amp;halten</translation>
    </message>
    <message>
        <location/>
        <source>How long to keep transmitting after silence</source>
        <translation>Wie lange nach dem Einsetzen von Stille übertragen werden soll</translation>
    </message>
    <message>
        <source>&lt;b&gt;This selects how long after a perceived stop in speech transmission should continue.&lt;/b&gt;&lt;br /&gt;Set this higher if your voice breaks up when you speak (seen by a rapidly blinking voice icon next to your name).&lt;br /&gt;Only makes sense when used with Voice Activity transmission.</source>
        <translation type="obsolete">&lt;b&gt;Dies wählt aus, wie lange nach Beenden des Gesprächs noch übertragen werden soll.&lt;/b&gt;&lt;br /&gt;Höhere Werte nutzen können helfen wenn die Stimme plötzlich abbricht (erkennbar an einem flackernden Voice-Icon neben dem Namen).&lt;br /&gt;Dies ist nur bei sprachaktivierter Übertragung sinnvoll.</translation>
    </message>
    <message>
        <location/>
        <source>Silence Below</source>
        <translation>Stille bis</translation>
    </message>
    <message>
        <source>Signal values below this counts as silence</source>
        <translation type="obsolete">Signalwerte darunter zählen als Stille</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the trigger values for voice detection.&lt;/b&gt;&lt;br /&gt;Use this together with the Audio Statistics window to manually tune the trigger values for detecting speech. Input values below &quot;Silence Below&quot; always count as silence. Values above &quot;Speech Above&quot; always count as voice. Values in between will count as voice if you&apos;re already talking, but will not trigger a new detection.</source>
        <translation>&lt;b&gt;Dies setzt die Auslösewerte für die Spracherkennung.&lt;/b&gt;&lt;br /&gt;Zusammen mit dem Audiostatistik Fenster können die Auslösewerte für die Spracherkennung manuell eingestellt werden. Eingabewerte unter &quot;Stille bis&quot; zählen immer als Stille, Werte über &quot;Sprache über&quot; immer als Sprache. Werte dazwischen zählen als Sprache wenn schon gesprochen wird, lösen aber keine Erkennung (und damit Übertragung) aus.</translation>
    </message>
    <message>
        <location/>
        <source>Speech Above</source>
        <translation>Sprache über</translation>
    </message>
    <message>
        <location/>
        <source>Signal values above this count as voice</source>
        <translation>Signalwerte darüber zählen als Sprache</translation>
    </message>
    <message>
        <location/>
        <source>Compression</source>
        <translation>Kompression</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Quality</source>
        <translation>&amp;Qualität</translation>
    </message>
    <message>
        <location/>
        <source>Quality of compression (peak bandwidth)</source>
        <translation>Qualität der Kompression (Spitzenbandbreite)</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the quality of compression.&lt;/b&gt;&lt;br /&gt;This determines how much bandwidth Mumble is allowed to use for outgoing audio.</source>
        <translation>&lt;b&gt;Dies setzt die Qualität der Kompression.&lt;/b&gt;&lt;br /&gt;Dies bestimmt wieviel Bandbreite Mumble für ausgehendes Audio nutzen darf.</translation>
    </message>
    <message>
        <source>&amp;Complexity</source>
        <translation type="obsolete">&amp;Komplexität</translation>
    </message>
    <message>
        <source>Complexity of compression (CPU)</source>
        <translation type="obsolete">Komplexität der Kompression (CPU)</translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets the complexity of compression.&lt;/b&gt;&lt;br /&gt;This determines how much CPU Mumble is allowed to use to increase transmitted voice quality. Settings above 5 give only marginal gain.</source>
        <translation type="obsolete">&lt;b&gt;Dies legt die Komplexität der Kompression fest.&lt;/b&gt;&lt;br /&gt;Es bestimmt wieviel CPU-Zeit Mumble nutzen darf um die übertragene Sprachqualität zu steigern. Einstellungen über 5 bringen nur minimale Verbesserungen.</translation>
    </message>
    <message>
        <location/>
        <source>Audio per packet</source>
        <translation>Audio pro Paket</translation>
    </message>
    <message>
        <location/>
        <source>How many audio frames to send per packet</source>
        <translation>Wieviele Audioframes pro Paket zu senden sind</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This selects how many audio frames should be put in one packet.&lt;/b&gt;&lt;br /&gt;Increasing this will increase the latency of your voice, but will also reduce bandwidth requirements.</source>
        <translation>&lt;b&gt;Dies legt fest, wieviele Audioframes in ein Paket gepackt werden sollen.&lt;/b&gt;&lt;br /&gt;Durch Erhöhung steigt die Latenz der Sprache, gleichzeitig sinkt aber der Bandbreitenbedarf.</translation>
    </message>
    <message>
        <source>Maximum bandwidth used for sent audio</source>
        <translation type="obsolete">Maximal verwendete Bandbreite für Audiodaten</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This shows peak outgoing bandwidth used.&lt;/b&gt;&lt;br /&gt;This shows the peak amount of bandwidth sent out from your machine. Audio bitrate is the maximum bitrate (as we use VBR) for the audio data alone. Position is the bitrate used for positional information. Overhead is our framing and the IP packet headers (IP and UDP is 75% of this overhead).</source>
        <translation>&lt;b&gt;Dies zeigt die Spitze der ausgehenden Bandbreite an.&lt;/b&gt;&lt;br /&gt;Dies zeigt die maximale Bandbreite an, die zum Senden verwendet wurde. Die Audio-Bitrate ist die maximale Bitrate, die für Audio-Daten genutzt wird (maximal da VBR verwendet wird). Position ist die Bitrate für Positionsinformationen. Overhead wird durch das verpacken der Audiodaten in Netzwerkpakete verursacht (75% dieses Overheads enstehen durch die IP- und UDP-Paketheader).</translation>
    </message>
    <message>
        <location/>
        <source>Audio Processing</source>
        <translation>Audioverarbeitung</translation>
    </message>
    <message>
        <location/>
        <source>Noise Suppression</source>
        <translation>Rauschunterdrückung</translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets the ammount of noise suppression to apply.&lt;/b&gt;&lt;br /&gt;The higher this value, the more aggressively stationary noise will be supressed.</source>
        <translation type="obsolete">&lt;b&gt;Dies legt die Menge an Rauschunterdrückung fest.&lt;/b&gt;&lt;br /&gt;Je höher dieser Wert ist, desto aggressiver werden stationäre Geräusche unterdrückt.</translation>
    </message>
    <message>
        <location/>
        <source>Amplification</source>
        <translation>Verstärkung</translation>
    </message>
    <message>
        <location/>
        <source>Maximum amplification of input sound</source>
        <translation>Maximale Verstärkung des Eingangssignals</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Maximum amplification of input.&lt;/b&gt;&lt;br /&gt;Mumble normalizes the input volume before compressing, and this sets how much it&apos;s allowed to amplify.&lt;br /&gt;The actual level is continually updated based on your current speech pattern, but it will never go above the level specified here.&lt;br /&gt;If the &lt;i&gt;Microphone loudness&lt;/i&gt; level of the audio statistics hover around 100%, you probably want to set this to 2.0 or so, but if, like most people, you are unable to reach 100%, set this to something much higher.&lt;br /&gt;Ideally, set it so &lt;i&gt;Microphone Loudness * Amplification Factor &gt;= 100&lt;/i&gt;, even when you&apos;re speaking really soft.&lt;br /&gt;&lt;br /&gt;Note that there is no harm in setting this to maximum, but Mumble will start picking up other conversations if you leave it to auto-tune to that level.</source>
        <translation>&lt;b&gt;Maximale Verstärkung des Eingangssignals.&lt;/b&gt;&lt;br /&gt;Mumble normalisiert die Eingangslautstärke vor der Kompression, wobei diese Option festlegt wie sehr verstärkt werden darf.&lt;br /&gt;Der tatsächliche Level wird kontinuierlich, abhängig vom Sprachmuster, aktualisiert; allerdings nie höher als hier festgelegt.&lt;br /&gt;Wenn die Mikrofonlautstärke in den Audiostatistiken um 100% liegt, sollte man dies auf 2.0 setzen. Für Leute die dies kaum erreichen, muss es deutlich höher angesetzt werden.&lt;br /&gt;Idealerweise sollte es folgendermaßen gesetzt werden: &lt;i&gt;Mikrofon Lautstärke * Verstärkungsfaktor &gt;= 100&lt;/i&gt;, selbst wenn man wirklich leise spricht.&lt;br /&gt;Es ist nicht schädlich dies auf das Maximum zu setzen, aber Mumble wird dadurch auch Umgebungsgeräusche aufnehmen.</translation>
    </message>
    <message>
        <location/>
        <source>Current speech detection chance</source>
        <translation>Aktuelle Spracherkennungswahrscheinlichkeit</translation>
    </message>
    <message>
        <location/>
        <source>Cancel echo from speakers</source>
        <translation>Unterdrücke das Echo der Lautsprecher</translation>
    </message>
    <message>
        <location/>
        <source>Enabling this will cancel the echo from your speakers. Mixed has low CPU impact, but only works well if your speakers are equally loud and equidistant from the microphone. Multichannel echo cancellation provides much better echo cancellation, but at a higher CPU cost.</source>
        <translation>Bei Aktivieren dieser Option wird versucht das Echo der Lautsprecher zu unterdrücken. Gemischt hat geringe Auswirkung auf die CPU-Auslastung aber funktioniert nur wenn Ihre Lautsprecher gleich laut und vom Mikrofon gleich weit entfernt sind. Mehrkanal-Echo-Unterdrückung bietet weitaus bessere Echo-Unterdrückung, verbraucht aber mehr CPU-Zeit.</translation>
    </message>
    <message>
        <location/>
        <source>Disabled</source>
        <translation>Deaktiviert</translation>
    </message>
    <message>
        <location/>
        <source>Mixed</source>
        <translation>Gemischt</translation>
    </message>
    <message>
        <location/>
        <source>Multichannel</source>
        <translation>Mehrkanal</translation>
    </message>
    <message>
        <location/>
        <source>Echo</source>
        <translation>Echounterdrückung</translation>
    </message>
    <message>
        <location/>
        <source>If you press the PTT key twice in this time it will get locked.</source>
        <translation>Wenn Sie die PTT-Taste (Sprech-Taste) zweimal innerhalb dieser Zeit drücken wird die Sprachübertragung dauerhaft aktiviert.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;DoublePush Time&lt;/b&gt;&lt;br /&gt;If you press the push-to-talk key twice during the configured interval of time it will be locked. Mumble will keep transmitting until you hit the key once more to unlock PTT again.</source>
        <translation>&lt;b&gt;Doppeldruck-Zeit&lt;/b&gt;&lt;br /&gt;Wenn Sie die PTT-Taste zweimal innerhalb der Doppeldruck-Zeit drücken wird die Sprachübertragung dauerhaft aktiviert. Diese wird beendet wenn sie die Taste ein weiteres mal drücken.</translation>
    </message>
    <message>
        <location/>
        <source>Gets played when the PTT button is pressed</source>
        <translation>Wird abgespielt wenn die PTT-Taste gedrückt wird</translation>
    </message>
    <message>
        <location/>
        <source>Gets played when the PTT button is released</source>
        <translation>Wird abgespielt wenn die PTT-Taste losgelassen wird</translation>
    </message>
    <message>
        <location/>
        <source>Reset audio cue to default</source>
        <translation>Audio-Hinweis auf Standard zurücksetzen</translation>
    </message>
    <message>
        <source>Reset</source>
        <translation type="obsolete">Zurücksetzen</translation>
    </message>
    <message>
        <location/>
        <source>Browse for on audio file</source>
        <translation>Nach einer Datei für den Beginn der Audioübertragung suchen</translation>
    </message>
    <message>
        <source>Browse</source>
        <translation type="obsolete">Durchsuchen</translation>
    </message>
    <message>
        <location/>
        <source>Browse for off audio file</source>
        <translation>Nach einer Datei für das Ende der Audioübertragung suchen</translation>
    </message>
    <message>
        <location/>
        <source>Off</source>
        <translation>Aus</translation>
    </message>
    <message>
        <location/>
        <source>On</source>
        <translation>An</translation>
    </message>
    <message>
        <location/>
        <source>Preview the audio cues</source>
        <translation>Die Audio-Hinweise probeweise abspielen</translation>
    </message>
    <message>
        <source>&lt;b&gt;Preview&lt;/b&gt;&lt;br/&gt;Plays the current &lt;i&gt;on&lt;/i&gt; soundfile followed by the current &lt;i&gt;off&lt;/i&gt; soundfile.</source>
        <translation type="obsolete">&lt;b&gt;Vorschau&lt;/b&gt;&lt;br/&gt;
Spielt den aktuellen &lt;i&gt;An&lt;/i&gt;-Audio-Hinweis gefolgt vom aktuellen &lt;i&gt;Aus&lt;/i&gt;-Hinweis ab.</translation>
    </message>
    <message>
        <source>Preview</source>
        <translation type="obsolete">Vorschau</translation>
    </message>
    <message>
        <location/>
        <source>Audible audio cue when push-to-talk is activated or deactivated</source>
        <translation>Akustischer Hinweis, wenn Push-to-Talk aktiviert oder deaktiviert wird</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets speech detection to use Signal to Noise ratio.&lt;/b&gt;&lt;br /&gt;In this mode, the input is analyzed for something resembling a clear signal, and the clarity of that signal is used to trigger speech detection.</source>
        <translation>&lt;b&gt;Dies wählt als Spracherkennungsmechanismus das Signal-zu-Rausch-Verhältnis.&lt;/b&gt;&lt;br /&gt;In diesem Modus werden die eingehenden Audiodaten auf ein klares Signal hin überprüft. Die Klarheit des Signals aktiviert dann die Spracherkennung.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This shows the current speech detection settings.&lt;/b&gt;&lt;br /&gt;You can change the settings from the Settings dialog or from the Audio Wizard.</source>
        <translation>&lt;b&gt;Dies zeigt die aktuellen Spracherkennungs-Einstellungen an.&lt;/b&gt;&lt;br /&gt;Die Einstellung können im Einstellungsdialog oder dem Audio-Assistenten geändert werden.</translation>
    </message>
    <message>
        <location/>
        <source>Noise suppression</source>
        <translation>Rauschunterdrückung</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the amount of noise suppression to apply.&lt;/b&gt;&lt;br /&gt;The higher this value, the more aggressively stationary noise will be suppressed.</source>
        <translation>&lt;b&gt;Dies setzt die Stärke der Rauschunterdrückung die angewandt werden soll&lt;/b&gt;&lt;br /&gt;Je höher der Wert, desto aggressiver wird Rauschen unterdrückt.</translation>
    </message>
    <message>
        <source>Idle AutoMute</source>
        <translation type="obsolete">AutoStumm bei Untätigkeit</translation>
    </message>
    <message>
        <source>How long to remain idle before auto-mute.</source>
        <translation type="obsolete">Gibt an wie lange Sie untätig sein dürfen bevor Sie stumm gestellt werden.</translation>
    </message>
    <message>
        <source>This sets an idle timer. If the timer expires without any audio being sent to the server, you are muted.</source>
        <translation type="obsolete">Dies setzt den Untätigkeits-Timer. Wenn der Timer ausläuft, ohne dass Ton-Daten an den Server gesendet werden, werden Sie stumm geschaltet.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Reset&lt;/b&gt;&lt;br/&gt;Reset the paths for the files to their default.</source>
        <translation>&lt;b&gt;Zurücksetzen&lt;/b&gt;&lt;br /&gt;Setzt die Pfade zu den Dateien auf die Standardwerte zurück.</translation>
    </message>
    <message>
        <location/>
        <source>Signal values below this count as silence</source>
        <translation>Signalwerte darunter zählen als Stille</translation>
    </message>
    <message>
        <location/>
        <source>Maximum bandwidth used for sending audio</source>
        <translation>Maximale Bandbreite, die zum Senden von Audiodaten verwendet wird</translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets the amount of noise suppression to apply.&lt;/b&gt;&lt;br /&gt;The higher this value, the more aggressively stationary noise will be supressed.</source>
        <translation type="obsolete">&lt;b&gt;Dies legt die Menge an Rauschunterdrückung fest, die verwendet werden soll.&lt;/b&gt;&lt;br /&gt;Je höher dieser Wert, desto aggressiver wird stationäres Rauschen unterdrückt.</translation>
    </message>
    <message>
        <location filename="AudioInput.cpp" line="+520"/>
        <source>Server maximum network bandwidth is only %1 kbit/s. Audio quality auto-adjusted to %2 kbit/s (%3ms)</source>
        <translation>Die maximal mögliche Bandbreite des Servers beträgt nur %1 kbit/s. Die Audio-Qualität wurde automatisch auf %2 kbit/s (%3ms) gesetzt</translation>
    </message>
    <message>
        <location filename="AudioInput.ui"/>
        <source>Use Amplitude based speech detection</source>
        <translation>Benutze amplitudenbasierte Spracherkennung</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This selects how long after a perceived stop in speech transmission should continue.&lt;/b&gt;&lt;br /&gt;Set this higher if your voice breaks up when you speak (seen by a rapidly blinking voice icon next to your name).</source>
        <translation>&lt;b&gt;Hiermit bestimmen Sie, wie lange nach Beenden des Gesprächs noch übertragen werden soll.&lt;/b&gt;&lt;br /&gt;Höhere Werte sind hilfreich, wenn die Stimme plötzlich abbricht (erkennbar an einem flackerndem Voice-Icon neben dem Namen).</translation>
    </message>
    <message>
        <location/>
        <source>Idle AutoDeafen</source>
        <translation>Automatisch taub stellen</translation>
    </message>
    <message>
        <location/>
        <source>How long to remain idle before auto-deafen.</source>
        <translation>Die Zeit nach der man automatisch stumm und taub gestellt wird.</translation>
    </message>
    <message>
        <location/>
        <source>This sets an idle timer. If the timer expires without any audio being sent to the server, you are muted and deafened.</source>
        <translation>Dies setzt den Untätigkeits-Timer. Wenn der Timer ausläuft, ohne dass Ton-Daten an den Server gesendet werden, werden Sie stumm und taub geschaltet.</translation>
    </message>
    <message>
        <location/>
        <source>Exclusive mode</source>
        <translation>Exklusiv-Modus</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This opens the device in exclusive mode.&lt;/b&gt;&lt;br /&gt;No other application will be able to use the device.</source>
        <translation>&lt;b&gt;Öffnet das Gerät im Exklusiv-Modus.&lt;/b&gt;&lt;br /&gt;Keine andere Anwendung wird das Gerät verwenden können.</translation>
    </message>
    <message>
        <location/>
        <source>Exclusive</source>
        <translation>Exklusiv</translation>
    </message>
    <message>
        <location/>
        <source>R&amp;eset</source>
        <translation>Zurücks&amp;etzen</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Browse...</source>
        <translation>&amp;Durchsuchen...</translation>
    </message>
    <message>
        <location/>
        <source>B&amp;rowse...</source>
        <translation>D&amp;urchsuchen...</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Preview</source>
        <translation>&amp;Vorschau</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Preview&lt;/b&gt;&lt;br/&gt;Plays the current &lt;i&gt;on&lt;/i&gt; sound followed by the current &lt;i&gt;off&lt;/i&gt; sound.</source>
        <translation>&lt;b&gt;Vorschau&lt;/b&gt;&lt;br /&gt;Spielt den aktuellen &lt;i&gt;An&lt;/i&gt; Sound gefolgt vom aktuellen &lt;i&gt;Aus&lt;/i&gt; Sound.</translation>
    </message>
</context>
<context>
    <name>AudioInputDialog</name>
    <message>
        <location filename="AudioConfigDialog.cpp" line="+71"/>
        <source>Continuous</source>
        <translation>Kontinuierlich</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Voice Activity</source>
        <translation>Sprachaktivierung</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Push To Talk</source>
        <translation>Push-To-Talk</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Audio Input</source>
        <translation>Audioeingabe</translation>
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
        <translation>Aus</translation>
    </message>
    <message>
        <location line="-30"/>
        <source>%1 s</source>
        <translation>%1 s</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>%1 kb/s</source>
        <translation>%1 kb/s</translation>
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
        <translation>%1 kbit/s (Audio %2 %5, Position %4, Overhead %3)</translation>
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
        <source>Choose sound file</source>
        <translation type="obsolete">Wählen Sie eine Sounddatei</translation>
    </message>
    <message>
        <source>Invalid sound file</source>
        <translation type="obsolete">Ungültige Sounddatei</translation>
    </message>
    <message>
        <source>The file &apos;%1&apos; does not exist or is not a valid file.</source>
        <translation type="obsolete">Die Datei &apos;%1&apos; existiert nicht oder ist keine gültige Datei.</translation>
    </message>
    <message>
        <source>%1kbit/s (Audio %2, Position %4, Overhead %3)</source>
        <translation type="obsolete">%1 kbit/s (Audio %2, Position %4, Overhead %3)</translation>
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
        <translation>Formular</translation>
    </message>
    <message>
        <location/>
        <source>Interface</source>
        <translation>Schnittstelle</translation>
    </message>
    <message>
        <location/>
        <source>System</source>
        <translation>System</translation>
    </message>
    <message>
        <location/>
        <source>Output method for audio</source>
        <translation>Audio-Ausgabemethode</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is the output method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation>Dies ist die Audio-Ausgabemethode die verwendet werden soll.</translation>
    </message>
    <message>
        <location/>
        <source>Device</source>
        <translation>Gerät</translation>
    </message>
    <message>
        <location/>
        <source>Output device for audio</source>
        <translation>Audio-Ausgabegerät</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is the output device to use for audio.&lt;/b&gt;</source>
        <translation>&lt;b&gt;Dies ist das Audio-Ausgabegerät.&lt;/b&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Positional Audio</source>
        <translation>Positionsabhängiges Audio</translation>
    </message>
    <message>
        <location/>
        <source>Audio Output</source>
        <translation>Audioausgabe</translation>
    </message>
    <message>
        <location/>
        <source>Default &amp;Jitter Buffer</source>
        <translation>Standard &amp;Jitter-Puffer</translation>
    </message>
    <message>
        <location/>
        <source>Safety margin for jitter buffer</source>
        <translation>Sicherheitsgrenze für den Jitter-Puffer</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the minimum safety margin for the jitter buffer.&lt;/b&gt;&lt;br /&gt;All incoming audio is buffered, and the jitter buffer continually tries to push the buffer to the minimum sustainable by your network, so latency can be as low as possible. This sets the minimum buffer size to use. If the start of sentences you hear is very jittery, increase this value.</source>
        <translation>&lt;b&gt;Dies legt die minimale Sicherheitsgrenze für den Jitter-Puffer fest.&lt;/b&gt;&lt;br /&gt;Eingehende Sprachdaten werden gepuffert und der Jitter-Puffer versucht den Puffer so klein wie möglich zu halten so dass die Latenz so gering wie möglich sein kann. Dies legt die minimale Puffergröße fest. Falls sich der Anfang eines Satzes unterbrochen anhört, erhöhen Sie diesen Wert.</translation>
    </message>
    <message>
        <source>Attenuate other applications while you talk</source>
        <translation type="obsolete">Lautstärke anderer Programme dämpfen während gesprochen wird</translation>
    </message>
    <message>
        <location/>
        <source>Minimum distance to user before sound volume decreases</source>
        <translation>Minimale Distanz zu einem Benutzer bevor die Lautstärke abnimmt</translation>
    </message>
    <message>
        <location/>
        <source>This sets the minimum distance for sound calculations. The volume of other users&apos; speech will not decrease until they are at least this far away from you.</source>
        <translation>Dies setzt die minimale Distanz für Klangberechnungen fest. Die Lautstärke anderer Benutzer wird nicht sinken, so lange Sie nicht mindestens so weit entfernt sind.</translation>
    </message>
    <message>
        <location/>
        <source>This sets the maximum distance for sound calculations. When farther away than this, other users&apos; speech volume will not decrease any further.</source>
        <translation>Dies setzt die maximale Distanz für Klangberechnungen fest. Die Lautstärke anderer Benutzer wird nicht mehr sinken, wenn sie weiter entfernt sind.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This enables one of the loopback test modes.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;None&lt;/i&gt; - Loopback disabled&lt;br /&gt;&lt;i&gt;Local&lt;/i&gt; - Emulate a local server.&lt;br /&gt;&lt;i&gt;Server&lt;/i&gt; - Request loopback from server.&lt;br /&gt;Please note than when loopback is enabled, no other users will hear your voice. This setting is not saved on application exit.</source>
        <translation>&lt;b&gt;Dies aktiviert einen der Rückkopplungs-Testmodi.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;Keine&lt;/i&gt; - Rückkopplung deaktiviert&lt;br /&gt;&lt;i&gt;Lokal&lt;/i&gt; - Emuliere einen lokalen Server.&lt;br/&gt;&lt;i&gt;Server&lt;/i&gt; - Rückkopplung vom Server (Anfrage).&lt;br/&gt;Bitte beachten Sie, dass, so lange die Rückkopplung aktiv ist, Sie kein anderer Benutzer hören kann. Diese Einstellung wird beim Beenden des Programms nicht gespeichert.</translation>
    </message>
    <message>
        <source>TextLabel</source>
        <translation type="obsolete">Textlabel</translation>
    </message>
    <message>
        <location/>
        <source>Volume</source>
        <translation>Lautstärke</translation>
    </message>
    <message>
        <location/>
        <source>Volume of incoming speech</source>
        <translation>Lautstärke der ankommenden Sprache</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This adjusts the volume of incoming speech.&lt;/b&gt;&lt;br /&gt;Note that if you increase this beyond 100%, audio will be distorted.</source>
        <translation>&lt;b&gt;Dies legt die Lautstärke der eingehenden Sprache fest.&lt;/b&gt;&lt;br /&gt;Beachten Sie, dass bei einem Wert über 100% die Geräusche verzerrt werden.</translation>
    </message>
    <message>
        <location/>
        <source>Output Delay</source>
        <translation>Ausgabeverzögerung</translation>
    </message>
    <message>
        <location/>
        <source>Amount of data to buffer</source>
        <translation>Datenmenge die gepuffert werden soll</translation>
    </message>
    <message>
        <source>This sets the amount of data to prebuffer in the output buffer. Experiment with different values and set it to the lowest which doesn&apos;t cause rapid jitter in the sound.</source>
        <translation type="obsolete">Dies legt die Menge an Daten fest, die im Ausgabepuffer vorgepuffert werden sollen. Experimentieren Sie mit verschiedenen Werten und setzen Sie es auf den niedrigsten Wert, der noch kein schnelles Zittern/Flackern im Ton verursacht.</translation>
    </message>
    <message>
        <source>Volume attenuation</source>
        <translation type="obsolete">Lautstärkendämpfung</translation>
    </message>
    <message>
        <source>MinDistance</source>
        <translation type="obsolete">MinDistanz</translation>
    </message>
    <message>
        <source>Minimum distance to player before sound decreases</source>
        <translation type="obsolete">Minimale Distanz zum Spieler bevor Lautstärke sinkt</translation>
    </message>
    <message>
        <source>This sets the minimum distance for sound calculations. The volume of other players&apos; speech will not decrease until they are at least this far away from you.</source>
        <translation type="obsolete">Dies legt die minimale Distanz für Klangberechnungnen fest. Die Lautstärke anderer Spieler wird nicht sinken, solange sie nicht wenigstens soweit wie eingestellt entfernt sind.</translation>
    </message>
    <message>
        <source>MaxDistance</source>
        <translation type="obsolete">MaxDistanz</translation>
    </message>
    <message>
        <source>Maximum distance, beyond which sound won&apos;t decrease</source>
        <translation type="obsolete">Maximale Distanz, über der die Lautstärke nicht mehr sinkt</translation>
    </message>
    <message>
        <source>This sets the maximum distance for sound calculations. When farther away than this, other players&apos; sound volume will not decrease any more.</source>
        <translation type="obsolete">Dies legt die maximale Distanz für Klangberechnungnen fest. Die Lautstärke anderer Spieler wird nicht mehr sinken, wenn sie weiter als eingestellt entfernt sind.</translation>
    </message>
    <message>
        <source>RollOff</source>
        <translation type="obsolete">Rolloff</translation>
    </message>
    <message>
        <location/>
        <source>Factor for sound volume decrease</source>
        <translation>Faktor zur Lautstärkensenkung</translation>
    </message>
    <message>
        <source>How fast should sound volume drop when passing beyond the minimum distance. The normal (1.0) is that sound volume halves each time the distance doubles. Increasing this value means sound volume drops faster, while decreasing it means it drops slower.</source>
        <translation type="obsolete">Wie schnell soll die Lautstärke sinken, wenn die Minimaldistanz überschritten ist. Mit der Standardeinstellung (1.0) wird die Lautstärke bei doppelter Entfernung halbiert. Ein höherer Wert lässt die Lautstärke schneller fallen, während ein geringerer Wert die Lautstärker langsamer fallen lässt.</translation>
    </message>
    <message>
        <location/>
        <source>Bloom</source>
        <translation>Bloom</translation>
    </message>
    <message>
        <location/>
        <source>Loopback Test</source>
        <translation>Rückkopplungs-Test</translation>
    </message>
    <message>
        <location/>
        <source>Delay Variance</source>
        <translation>Verzögerungsvarianz</translation>
    </message>
    <message>
        <location/>
        <source>Variance in packet latency</source>
        <translation>Varianz der Paketlatenz</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the packet latency variance for loopback testing.&lt;/b&gt;&lt;br /&gt;Most audio paths contain some variable latency. This allows you to set that variance for loopback mode testing. For example, if you set this to 15ms, this will emulate a network with 20-35ms ping latency or one with 80-95ms latency. Most domestic net connections have a variance of about 5ms.</source>
        <oldsource>&lt;b&gt;This sets the packet latency variance for loopback testing.&lt;/b&gt;&lt;br /&gt;Most audio paths contain some variable latency. This allows you set that variance for loopback mode testing. For example, if you set this to 15ms, this will emulate a network with 20-35ms ping latency or one with 80-95ms latency. Most domestic net connections have a variance of about 5ms.</oldsource>
        <translation>&lt;b&gt;Dies legt die Varianz der Paketlatenz für den Rückkopplungs-Test fest.&lt;/b&gt;&lt;br/&gt;Die meisten Audiopfade haben eine variable, sich ständig Ändernde Latenz. Hiermit können Sie die Varianz für den Rückkopplungs-Test festlegen. Falls Sie dies zum Beispiel auf 15ms setzen, wird ein Netzwerk mit 20-35ms oder eines mit 80-95ms Latenz immitiert. Die meisten Netzverbindungen haben eine Varianz von etwa 5ms.</translation>
    </message>
    <message>
        <location/>
        <source>Packet Loss</source>
        <translation>Paketverlust</translation>
    </message>
    <message>
        <location/>
        <source>Packet loss for loopback mode</source>
        <translation>Paketverlust für den Rückkopplungs-Modus</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the packet loss for loopback mode.&lt;/b&gt;&lt;br /&gt;This will be the ratio of packets lost. Unless your outgoing bandwidth is peaked or there&apos;s something wrong with your network connection, this will be 0%</source>
        <translation>&lt;b&gt;Dies legt den Paketverlust für den Rückkopplungs-Modus fest.&lt;/b&gt;&lt;br /&gt;Dies ist das Verhältnis an verlorenen Paketen. Solange Ihre Ausgangsbandbreite nicht an ihre Grenze stößt oder etwas mit Ihrer Netzwerkverbindung nicht stimmt, wird dieser 0% betragen</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Loopback</source>
        <translation>&amp;Rückkopplung</translation>
    </message>
    <message>
        <location/>
        <source>Desired loopback mode</source>
        <translation>Gewünschter Rückkopplungs-Modus</translation>
    </message>
    <message>
        <source>&lt;b&gt;This enables one of the loopback testmodes.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;None&lt;/i&gt; - Loopback disabled&lt;br /&gt;&lt;i&gt;Local&lt;/i&gt; - Emulate a local server.&lt;br /&gt;&lt;i&gt;Server&lt;/i&gt; - Request loopback from server.&lt;br /&gt;Please note than when loopback is enabled, no other players will hear your voice. This setting is not saved on application exit.</source>
        <translation type="obsolete">&lt;b&gt;Dies aktiviert einen der Rückkopplungs-Testmodi.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;Keiner&lt;/i&gt; - Rückkopplung deaktiviert&lt;br /&gt;&lt;i&gt;Lokal&lt;/i&gt; - Immitiert einen lokalen Server.&lt;br /&gt;&lt;i&gt;Server&lt;/i&gt; - Fordere vom Server eine Rückkopplung.&lt;br /&gt;Beachten Sie, dass Sie niemand bei aktiviertem Loopback hören kann. Diese Einstellung wird beim Beenden der Anwendung nicht gespeichert.</translation>
    </message>
    <message>
        <location/>
        <source>Factor for sound volume increase</source>
        <translation>Faktor zur Lautstärkenerhöhung</translation>
    </message>
    <message>
        <location/>
        <source>The connected &quot;speakers&quot; are actually headphones</source>
        <oldsource>The connected &quot;speakers&quot; are actually headphones.</oldsource>
        <translation>Die angeschlossenen Audioausgabe-Geräte sind Kopfhörer</translation>
    </message>
    <message>
        <location/>
        <source>Minimum Distance</source>
        <translation>Minimale Distanz</translation>
    </message>
    <message>
        <source>Minimum distance to player before sound volume decreases</source>
        <translation type="obsolete">Minimale Distanz zum Spieler, bevor dessen Lautstärke geringer wird.</translation>
    </message>
    <message>
        <location/>
        <source>Maximum Distance</source>
        <translation>Maximale Distanz</translation>
    </message>
    <message>
        <location/>
        <source>Maximum distance, beyond which speech volume won&apos;t decrease</source>
        <translation>Maximale Distanz zum Benutzer, hinter der seine Lautstärke nicht mehr geringer wird</translation>
    </message>
    <message>
        <source>This sets the maximum distance for sound calculations. When farther away than this, other players&apos; speech volume will not decrease any further.</source>
        <translation type="obsolete">Dies setzt die maximale Distanz für Ton-Berechnungen. Wenn man weiter weg als diese Distanz ist, wird die Lautstärke des anderen Spielers nicht mehr weiter verringert.</translation>
    </message>
    <message>
        <location/>
        <source>Minimum Volume</source>
        <translation>Minimale Lautstärke</translation>
    </message>
    <message>
        <location/>
        <source>What should the volume be at the maximum distance?</source>
        <translation>Wie hoch soll die Lautstärke bei maximaler Distanz sein?</translation>
    </message>
    <message>
        <location/>
        <source>How much should sound volume increase for sources that are really close?</source>
        <translation>Um wie viel soll die Lautstärke für Quellen erhöht werden, die sehr nah sind?</translation>
    </message>
    <message>
        <source>Connected &quot;speakers&quot; are actually headphones.</source>
        <translation type="obsolete">Kopfhöhrer sind angeschlossen und werden verwendet.</translation>
    </message>
    <message>
        <location/>
        <source>Checking this indicates that you don&apos;t have speakers connected, just headphones. This is important, as speakers are usually in front of you, while headphones are directly to your left/right.</source>
        <translation>Ist dies aktiviert bedeutet dies, dass keine Lautsprecher angeschlossen sind, sondern nur Kopfhörer. Dies ist wichtig, da Lautsprecher für gewöhnlich vor Ihnen sind, während Kopfhörer direkt links und rechts anliegen.</translation>
    </message>
    <message>
        <location/>
        <source>Headphones</source>
        <translation>Kopfhörer</translation>
    </message>
    <message>
        <source>Other Applications</source>
        <translation type="obsolete">Andere Anwendungen</translation>
    </message>
    <message>
        <location/>
        <source>This sets the amount of data to pre-buffer in the output buffer. Experiment with different values and set it to the lowest which doesn&apos;t cause rapid jitter in the sound.</source>
        <translation>Dies setzt die Menge an Daten, welche im Ausgabepuffer vorgepuffert wird. Experimentieren Sie mit verschiedenen Werten und nehmen Sie den niedrigsten, welcher kein schnelles Zittern im Ton produziert.</translation>
    </message>
    <message>
        <source>Volume of other applications during speech</source>
        <translation type="obsolete">Lautstärke anderer Anwendungen während jemand spricht</translation>
    </message>
    <message>
        <source>&lt;b&gt;Decrease volume of other applications during speech.&lt;/b&gt;&lt;br /&gt;Mumble supports decreasing the volume of other applications during incoming speech. This sets the relative volume of other applications when others are talking.</source>
        <translation type="obsolete">&lt;b&gt;Reduziere Lautstärke anderer Anwendungen während jemand spricht.&lt;/b&gt;&lt;br /&gt;Mumble unterstützt die Reduzierung der Lautstärke anderer Anwendungen während des Sprechens. Dies legt die relative Lautstärke anderer Anwendungen fest, wenn andere sprechen.</translation>
    </message>
    <message>
        <source>vol</source>
        <translation type="obsolete">vol</translation>
    </message>
    <message>
        <source>od</source>
        <translation type="obsolete">od</translation>
    </message>
    <message>
        <location/>
        <source>Attenuate applications by...</source>
        <translation>Anwendungslautstärken herabsetzen um…</translation>
    </message>
    <message>
        <location/>
        <source>Attenuation of other applications during speech</source>
        <translation>Lautstärkedämpfung anderer Anwendungen während des Sprechens</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Attenuate volume of other applications during speech&lt;/b&gt;&lt;br /&gt;Mumble supports decreasing the volume of other applications during incoming and/or outgoing speech. This sets the attenuation of other applications if the feature is enabled.</source>
        <translation>&lt;b&gt;Lautstärke anderer Anwendungen dämpfen während gesprochen wird&lt;/b&gt;&lt;br/&gt;
Mumble unterstützt das Herabsetzen der Lautstärke anderer Anwendungen während ein- und/oder ausgehendem Sprechen. Dieser Regler legt die Höhe der Herabsenkung der Lautstärke der anderen Anwendungen fest, wenn die Funktion aktiviert ist.</translation>
    </message>
    <message>
        <source>at</source>
        <translation type="obsolete">at</translation>
    </message>
    <message>
        <location/>
        <source>If checked Mumble lowers the volume of other applications while other users talk</source>
        <translation>Vermindert die Lautstärke anderer Anwendungen während andere sprechen</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Attenuate applications while other users talk&lt;/b&gt;&lt;br /&gt;Mumble supports decreasing the volume of other applications during incoming and/or outgoing speech. This makes mumble activate the feature while other users talk to you.</source>
        <translation>&lt;b&gt;Lautstärke anderer Anwendungen herabsetzen während andere Benutzer sprechen&lt;/b&gt;&lt;br/&gt;
Mumble unterstützt das Herabsetzen der Lautstärke anderer Anwendungen während ein- und/oder ausgehendem Sprechen. Dies lässt Mumble diese Funktion für das Sprechen anderer Benutzer zu ihnen aktivieren.</translation>
    </message>
    <message>
        <location/>
        <source>while other users talk</source>
        <translation>Während andere Benutzer sprechen</translation>
    </message>
    <message>
        <location/>
        <source>If checked Mumble lowers the volume of other applications while you talk</source>
        <translation>Vermindert die Lautstärke anderer Anwendungen während Sie sprechen</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Attenuate applications while you talk&lt;/b&gt;&lt;br /&gt;Mumble supports decreasing the volume of other applications during incoming and/or outgoing speech. This makes mumble activate the feature while you talk.</source>
        <translation>&lt;b&gt;Lautstärke anderer Anwendungen herabsetzen während Sie sprechen&lt;/b&gt;&lt;br/&gt;
Mumble unterstützt das Herabsetzen der Lautstärke anderer Anwendungen während ein- und/oder ausgehendem Sprechen. Dies aktiviert diese Funktion für wenn Sie sprechen.</translation>
    </message>
    <message>
        <location/>
        <source>while you talk</source>
        <translation>Während Sie sprechen</translation>
    </message>
    <message>
        <source>md</source>
        <translation type="obsolete">md</translation>
    </message>
    <message>
        <source>mv</source>
        <translation type="obsolete">ml</translation>
    </message>
    <message>
        <source>bl</source>
        <translation type="obsolete">bl</translation>
    </message>
    <message>
        <source>dv</source>
        <translation type="obsolete">dv</translation>
    </message>
    <message>
        <source>pl</source>
        <translation type="obsolete">pl</translation>
    </message>
    <message>
        <location/>
        <source>Exclusive mode, not recommended.</source>
        <translation>Exklusiv-Modus, nicht empfohlen.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This opens the device in exclusive mode.&lt;/b&gt;&lt;br /&gt;No other application will be able to use the device.</source>
        <translation>&lt;b&gt;Verwendet das Gerät im Exklusiv-Modus.&lt;/b&gt;&lt;br /&gt;Keine andere Anwendung wird das Gerät verwenden können.</translation>
    </message>
    <message>
        <location/>
        <source>Exclusive</source>
        <translation>Exklusiv</translation>
    </message>
</context>
<context>
    <name>AudioOutputDialog</name>
    <message>
        <location filename="AudioConfigDialog.cpp" line="+165"/>
        <source>None</source>
        <translation>Keine</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Local</source>
        <translation>Lokal</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Server</source>
        <translation>Server</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Audio Output</source>
        <translation>Audioausgabe</translation>
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
        <oldsource>%1%</oldsource>
        <translation>%1 %</translation>
    </message>
    <message>
        <location line="-16"/>
        <location line="+6"/>
        <source>%1 m</source>
        <oldsource>%1m</oldsource>
        <translation>%1 m</translation>
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
        <translation>Wählen Sie eine Sounddatei</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Invalid sound file</source>
        <translation>Ungültige Sounddatei</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The file &apos;%1&apos; cannot be used by Mumble. Please select a file with a compatible format and encoding.</source>
        <translation>Die Datei &apos;%1&apos; kann nicht in Mumble verwendet werden. Bitte wählen Sie eine Datei mit kompatiblem Format und Kodierung.</translation>
    </message>
</context>
<context>
    <name>AudioStats</name>
    <message>
        <location filename="AudioStats.ui"/>
        <source>Peak microphone level</source>
        <translation>Spitzen Mikrofonpegel</translation>
    </message>
    <message>
        <location/>
        <source>Peak speaker level</source>
        <translation>Spitzen Lautsprecherpegel</translation>
    </message>
    <message>
        <location/>
        <source>Peak clean level</source>
        <translation>Gesäuberter Spitzenpegel</translation>
    </message>
    <message>
        <source>Microphone loudness</source>
        <translation type="obsolete">Mikrofonlautstärke</translation>
    </message>
    <message>
        <source>Mic Signal-To-Noise</source>
        <translation type="obsolete">Mic Signal-Rauschen-Verhältnis</translation>
    </message>
    <message>
        <location/>
        <source>Speech Probability</source>
        <translation>Sprachwahrscheinlichkeit</translation>
    </message>
    <message>
        <source>Audio bitrate</source>
        <translation type="obsolete">Audio Bitrate</translation>
    </message>
    <message>
        <location/>
        <source>Peak power in last frame</source>
        <translation>Spitzenpegel im letzten Paket</translation>
    </message>
    <message>
        <source>This shows the peak power in the last frame (20 ms), and is the same measurement as you would usually find displayed as &quot;input power&quot;. Please disregard this and look at &lt;b&gt;Loudness&lt;/b&gt; instead, which is much more steady and disregards outliers.</source>
        <translation type="obsolete">Dies zeigt die Spitzenleistung im letzten Frame (20ms) an und ist als Maß äquivalent zu &quot;Input Power&quot;. Stattdessen bitte &lt;b&gt;Lautstärke&lt;/b&gt; nutzen.</translation>
    </message>
    <message>
        <location/>
        <source>This shows the peak power of the speakers in the last frame (20 ms). Unless you are using a multi-channel sampling method (such as ASIO) with speaker channels configured, this will be 0. If you have such a setup configured, and this still shows 0 while you&apos;re playing audio from other programs, your setup is not working.</source>
        <translation>Dies zeigt den Spitzenpegel des Lautsprechers im letzten Paket (20ms) an. Solange keine Multikanal-Sampling-Methode (wie ASIO) genutzt wird, wird dieser Wert 0 sein. Verwenden Sie jedoch eine solche Anordnung und der Wert ist beim abspielen von Audio in anderen Programmen trotzdem 0 so arbeitet die Einstellung nicht korrekt.</translation>
    </message>
    <message>
        <source>This shows the peak power in the last frame (20 ms) after all processing. Ideally, this should be -96 dB when you&apos;re not talking. In reality, a sound studio should see -60 dB, and you should hopefully see somewhere around -20 dB. When you are talking, this should rise to somewhere between -5 and -10 dB.&lt;br /&gt;If you are using echo cancellation, and this rises to more than -15 dB when you&apos;re not talking, your setup is not working, and you&apos;ll annoy other players with echoes.</source>
        <translation type="obsolete">Dies zeigt den Spitzenpegel nach dem Processing im letzten Paket (20ms) an. Im Idealfall sollte dies -96 dB betragen, wenn man nicht spricht. Die Realität zeigt, dass man in einem Tonstudio etwa -60 dB und zuhause -20 dB erwarten kann. When man spricht, sollte dies auf -5 bis -10 dB ansteigen.&lt;br /&gt;Nutzt man Echounterdrückung und der Wert steigt auf mehr als -15 dB wenn man nicht spricht, sind die Audioeinstellungen nicht korrekt und man stört andere Spieler mit Echos.</translation>
    </message>
    <message>
        <location/>
        <source>How close the current input level is to ideal</source>
        <translation>Wie nah der aktuelle Eingangspegel am Idealwert ist</translation>
    </message>
    <message>
        <source>This shows how close your current input volume is to the ideal. To adjust your microphone level, open whatever program you use to adjust the recording volume, and look at the value here while talking.&lt;br /&gt;&lt;b&gt;Talk loud, as you would when you&apos;re upset over getting fragged by a noob.&lt;/b&gt;&lt;br /&gt;Adjust the volume until this value is close to 100%, but make sure it doesn&apos;t go above. If it does go above, you are likely to get clipping in parts of your speech, which will degrade sound quality</source>
        <translation type="obsolete">Dies zeigt an wie nah die Eingangssignallautstärke am Ideal ist. Um die Mikrofonlautstärke anzupassen, sollte das Programm gestartet werden mit dem die Aufnahmelautstärke angepasst wird.&lt;br /&gt;&lt;b&gt;Nun sollte man, so laut wie es im Spiel vorkommt, sprechen&lt;/b&gt;&lt;br /&gt;Die Lautstärke muss nun so nah wie möglich an die 100% angepasst werden, aber nicht darüber hinaus. Wenn dem so ist, wird es hörbares Clipping geben, was die Soundqualität mindert</translation>
    </message>
    <message>
        <location/>
        <source>Signal-To-Noise ratio from the microphone</source>
        <translation>Signal-Rausch-Verhältnis des Mikrofons</translation>
    </message>
    <message>
        <location/>
        <source>This is the Signal-To-Noise Ratio (SNR) of the microphone in the last frame (20 ms). It shows how much clearer the voice is compared to the noise.&lt;br /&gt;If this value is below 1.0, there&apos;s more noise than voice in the signal, and so quality is reduced.&lt;br /&gt;There is no upper limit to this value, but don&apos;t expect to see much above 40-50 without a sound studio.</source>
        <translation>Dies ist das Signal-Rausch-Verhältnis (SRV) des Mikrofons im letzten Paket (20ms). Es zeigt an, um wie viel stärker die Stimme im Vergleich zum Hintergrundrauschen ist.&lt;br /&gt;Ist der Wert unter 1.0 gibt es mehr Geräusche als Stimme im Signal, weshalb die Qualität reduziert ist.&lt;br /&gt;Es gibt keine Obergrenze, aber ein Wert über 40-50 kann ohne Tonstudio nicht erwartet werden.</translation>
    </message>
    <message>
        <location/>
        <source>Probability of speech</source>
        <translation>Sprachwahrscheinlichkeit</translation>
    </message>
    <message>
        <location/>
        <source>This is the probability that the last frame (20 ms) was speech and not environment noise.&lt;br /&gt;Voice activity transmission depends on this being right. The trick with this is that the middle of a sentence is always detected as speech; the problem is the pauses between words and the start of speech. It&apos;s hard to distinguish a sigh from a word starting with &apos;h&apos;.&lt;br /&gt;If this is in bold font, it means Mumble is currently transmitting (if you&apos;re connected).</source>
        <translation>Dies ist die Wahrscheinlichkeit, dass das letzte Paket (20ms) Sprache und kein Umgebungsgeräusch war.&lt;br /&gt;Die Sprachaktivitätserkennung hängt von der Korrektheit dieses Wertes ab. Der Trick ist, dass die Mitte eines Satzes immer als Sprache erkannt wird; problematisch sind Pausen zwischen Wörtern und der Anfang des Satzes. Es ist schwierig ein Seufzen von einem mit &apos;h&apos; beginnenden Wort zu unterscheiden.&lt;br /&gt;Ist dieser Text fett geschrieben, bedeutet dies, dass Mumble gerade überträgt (falls Sie verbunden sind).</translation>
    </message>
    <message>
        <location/>
        <source>Bitrate of last frame</source>
        <translation>Bitrate des letzten Paketes</translation>
    </message>
    <message>
        <source>This is the audio bitrate of the last compressed frame (20 ms), and as such will jump up and down as the VBR adjusts the quality. To adjust the peak bitrate, adjust &lt;b&gt;Compression Complexity&lt;/b&gt; in the Settings dialog.</source>
        <translation type="obsolete">Dies ist die Audiobitrate des letzten komprimierten Paketes (20ms). Aus diesem Grund springt sie auf und ab während die VBR die Qualität anpasst. Um die Spitzenbitrate einzustellen muss &lt;b&gt;Kompressionsqualität&lt;/b&gt; im Einstellungsdialog angepasst werden.</translation>
    </message>
    <message>
        <location/>
        <source>Power spectrum of input signal and noise estimate</source>
        <translation>Leistungsspektrum des Eingangssignals und Rauschabschätzung</translation>
    </message>
    <message>
        <location/>
        <source>This shows the power spectrum of the current input signal (red line) and the current noise estimate (filled blue).&lt;br /&gt;All amplitudes are multiplied by 30 to show the interesting parts (how much more signal than noise is present in each waveband).&lt;br /&gt;This is probably only of interest if you&apos;re trying to fine-tune noise conditions on your microphone. Under good conditions, there should be just a tiny flutter of blue at the bottom. If the blue is more than halfway up on the graph, you have a seriously noisy environment.</source>
        <translation>Dieses Diagramm zeigt das Leistungsspektrum des Eingangssignals (rote Linie) und die aktuelle Rauschabschätzung (blau gefüllt) an.&lt;br /&gt;Alle Amplituden werden mit 30 multipliziert um anzuzeigen wie viel mehr Signal als Rauschen im Frequenzband vorhanden ist.&lt;br /&gt;Dies ist vermutlich nur von Interesse, wenn man die Rauschbedingungen des Mikrofones präzise einstellt. Unter guten Voraussetzungen sollte nur eine winzige blaue Stelle unten zu sehen sein. Ist mehr als die Hälfte des Graphen blau, hat man eine ungünstige Geräuschkulisse.</translation>
    </message>
    <message>
        <location/>
        <source>Weights of the echo canceller</source>
        <translation>Gewichte des Echounterdrückers</translation>
    </message>
    <message>
        <location/>
        <source>This shows the weights of the echo canceller, with time increasing downwards and frequency increasing to the right.&lt;br /&gt;Ideally, this should be black, indicating no echo exists at all. More commonly, you&apos;ll have one or more horizontal stripes of bluish color representing time delayed echo. You should be able to see the weights updated in real time.&lt;br /&gt;Please note that as long as you have nothing to echo off, you won&apos;t see much useful data here. Play some music and things should stabilize. &lt;br /&gt;You can choose to view the real or imaginary parts of the frequency-domain weights, or alternately the computed modulus and phase. The most useful of these will likely be modulus, which is the amplitude of the echo, and shows you how much of the outgoing signal is being removed at that time step. The other viewing modes are mostly useful to people who want to tune the echo cancellation algorithms.&lt;br /&gt;Please note: If the entire image fluctuates massively while in modulus mode, the echo canceller fails to find any correlation whatsoever between the two input sources (speakers and microphone). Either you have a very long delay on the echo, or one of the input sources is configured wrong.</source>
        <translation>Dieses Diagramm zeigt die Gewichte des Echounterdrückers mit der Zeit nach unten und der Frequenz nach rechts wachsend.&lt;br /&gt;Idealerweise sollte das Diagramm schwarz sein, also kein Echo existieren. Normalerweise hat man einen oder mehrere horizontale Streifen von blauer Farbe, die das zeitverzögerte Echo darstellen. Sie sollten sehen können, wie die Gewichte in Echtzeit aktualisiert werden.&lt;br /&gt;Solange keine Echoquelle vorhanden ist werden Sie hier keine nützlichen Daten sehen. Spielen Sie deshalb z.B. Musik ab um das Diagramm zu stabilisieren.&lt;br /&gt;Sie können zwischen Real- und Imaginäranteilen der Frequenzdomänen-Gewichte, oder alternativ dem berechneten Modulus und der Phase auswählen. Am nützlichsten ist hierbei Modulus, dieser ist die Amplitude des Echos und zeigt an wieviel vom ausgehenden Signal bei diesem Zeitschritt entfernt wird. Die anderen Ansichten sind nützlich für diejenigen die den Echoauslöschungs-Algorithmus feinjustieren wollen.&lt;br /&gt;Achtung: Wenn das Gesamtbild im Modulus-Modus massiv fluktuiert, kann der Echoauslöscher keine Korrelation zwischen den beiden Eingangsquellen (Lautsprecher und Mikrofon) herstellen. Entweder Sie haben eine sehr lange Verzögerung des Echos oder eine der Eingangsquellen ist falsch konfiguriert.</translation>
    </message>
    <message>
        <source>Mumble</source>
        <translation type="obsolete">Mumble</translation>
    </message>
    <message>
        <source>Real</source>
        <translation type="obsolete">Real</translation>
    </message>
    <message>
        <source>Imaginary</source>
        <translation type="obsolete">Imaginär</translation>
    </message>
    <message>
        <source>Modulus</source>
        <translation type="obsolete">Modulus</translation>
    </message>
    <message>
        <source>Phase</source>
        <translation type="obsolete">Phase</translation>
    </message>
    <message>
        <source>Doublepush interval</source>
        <translation type="obsolete">DoublePush Intervall</translation>
    </message>
    <message>
        <location filename="AudioStats.cpp" line="+405"/>
        <source>&gt;1000 ms</source>
        <oldsource>&gt;1000ms</oldsource>
        <translation>&gt;1000 ms</translation>
    </message>
    <message>
        <location filename="AudioStats.ui"/>
        <source>Audio Statistics</source>
        <translation>Audiostatistiken</translation>
    </message>
    <message>
        <location/>
        <source>Input Levels</source>
        <translation>Eingangspegel</translation>
    </message>
    <message>
        <location/>
        <source>This shows the peak power in the last frame (20 ms), and is the same measurement as you would usually find displayed as &quot;input power&quot;. Please disregard this and look at &lt;b&gt;Microphone power&lt;/b&gt; instead, which is much more steady and disregards outliers.</source>
        <translation>Dies zeigt den Spitzenpegel im letzten Paket (20ms) an und zeigt was üblicherweise auch &quot;Eingangspegel&quot; genannt wird. Bitte ignorieren Sie diesen Wert und sehen Sie sich stattdessen &lt;b&gt;Mikrofonpegel&lt;/b&gt; an, was deutlich stabiler ist und Ausreißer verwirft.</translation>
    </message>
    <message>
        <source>TextLabel</source>
        <translation type="obsolete">Textlabel</translation>
    </message>
    <message>
        <location/>
        <source>This shows the peak power in the last frame (20 ms) after all processing. Ideally, this should be -96 dB when you&apos;re not talking. In reality, a sound studio should see -60 dB, and you should hopefully see somewhere around -20 dB. When you are talking, this should rise to somewhere between -5 and -10 dB.&lt;br /&gt;If you are using echo cancellation, and this rises to more than -15 dB when you&apos;re not talking, your setup is not working, and you&apos;ll annoy other users with echoes.</source>
        <translation>Dies zeigt den Spitzenwert des letzten Frames (20 ms) nach der kompletten Verarbeitung. Idealerweise sollte dies -96 dB sein, wenn Sie nicht sprechen. In der Realität sollte ein Tonstudio -60 dB und Sie hoffentlich in etwa -20 dB sehen. Wenn Sie sprechen sollte dieser Wert auf ungefähr -5 bis -10 dB steigen.&lt;br /&gt;Wenn Sie Echounterdrückung verwenden und der Wert auf über -15 dB steigt wenn Sie sind Einstellungen oder Hardware fehlerhaft und Sie werden andere Benutzer mit Echos stören.</translation>
    </message>
    <message>
        <location/>
        <source>Signal Analysis</source>
        <translation>Signalanalyse</translation>
    </message>
    <message>
        <location/>
        <source>Microphone power</source>
        <translation>Mikrofonpegel</translation>
    </message>
    <message>
        <location/>
        <source>This shows how close your current input volume is to the ideal. To adjust your microphone level, open whatever program you use to adjust the recording volume, and look at the value here while talking.&lt;br /&gt;&lt;b&gt;Talk loud, as you would when you&apos;re upset over getting fragged by a noob.&lt;/b&gt;&lt;br /&gt;Adjust the volume until this value is close to 100%, but make sure it doesn&apos;t go above. If it does go above, you are likely to get clipping in parts of your speech, which will degrade sound quality.</source>
        <translation>Dieser Wert zeigt wie nah die aktuelle Eingangslautstärke am Ideal ist. Um den Mikrofonpegel anzupassen, öffnen Sie ein Programm mit welchem sie ihre Aufnahmelautstärken verändern können und schauen Sie hier auf den Wert während Sie reden.&lt;br /&gt;&lt;b&gt;Sprechen Sie so laut, als wären Sie über etwas verärgert.&lt;/b&gt;&lt;br /&gt;Passen Sie die Lautstärke an, bis der Wert nahe der 100% ist, aber nicht darüber hinaus. Sollte der Wert darüber steigen werden vermutlich Teile ihrer Sprache abgeschnitten, was die Soundqualität stark beeinträchtigt.</translation>
    </message>
    <message>
        <location/>
        <source>Signal-To-Noise ratio</source>
        <translation>Signal-Rausch-Verhältnis</translation>
    </message>
    <message>
        <location/>
        <source>Configuration feedback</source>
        <translation>Konfigurations-Rückmeldung</translation>
    </message>
    <message>
        <location/>
        <source>Current audio bitrate</source>
        <translation>Aktuelle Audio Bitrate</translation>
    </message>
    <message>
        <location/>
        <source>DoublePush interval</source>
        <translation>DoppelDruck Intervall</translation>
    </message>
    <message>
        <location/>
        <source>Time between last two Push-To-Talk presses</source>
        <oldsource>Time between last two PushToTalk presses</oldsource>
        <translation>Zeit zwischen den beiden letzten Push-To-Talk-Betätigungen</translation>
    </message>
    <message>
        <location/>
        <source>Speech Detection</source>
        <translation>Spracherkennung</translation>
    </message>
    <message>
        <location/>
        <source>Current speech detection chance</source>
        <translation>Aktuelle Spracherkennungswahrscheinlichkeit</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This shows the current speech detection settings.&lt;/b&gt;&lt;br /&gt;You can change the settings from the Settings dialog or from the Audio Wizard.</source>
        <translation>&lt;b&gt;Dies zeigt die aktuellen Spracherkennungs-Einstellungen an.&lt;/b&gt;&lt;br /&gt;Diese Einstellungen können im Einstellungsdialog oder dem Audio-Assistenten geändert werden.</translation>
    </message>
    <message>
        <location/>
        <source>Signal and noise power spectrum</source>
        <translation>Signal- und Rauschleistungsspektrum</translation>
    </message>
    <message>
        <location/>
        <source>Echo Analysis</source>
        <translation>Echo Analyse</translation>
    </message>
    <message>
        <location/>
        <source>This is the audio bitrate of the last compressed frame (20 ms), and as such will jump up and down as the VBR adjusts the quality. The peak bitrate can be adjusted in the Settings dialog.</source>
        <translation>Dies ist die Audiobitrate des letzten komprimierten Paketes (20ms). Aus diesem Grund springt sie auf und ab während die VBR die Qualität anpasst. Die Spitzenbandbreite kann in den Einstellungen angepasst werden.</translation>
    </message>
</context>
<context>
    <name>AudioWizard</name>
    <message>
        <location filename="AudioWizard.ui"/>
        <source>Audio Tuning Wizard</source>
        <translation>Audio Einstellungs-Assistent</translation>
    </message>
    <message>
        <location/>
        <source>Introduction</source>
        <translation>Einführung</translation>
    </message>
    <message>
        <location/>
        <source>Welcome to the Mumble Audio Wizard</source>
        <translation>Willkommen zum Mumble Audio-Assistenten</translation>
    </message>
    <message>
        <source>This is the audio tuning wizard for Mumble. This will help you correctly set the input levels of your sound card, and also set the correct parameters for sound processing in Mumble.</source>
        <translation type="obsolete">Dies ist der Audio Einstellungs-Assistent von Mumble. Er wird Ihnen helfen, die richtigen Eingabelautstärken ihrer Soundkarte und die korrekten Parameter für die Tonverarbeitung festzulegen.</translation>
    </message>
    <message>
        <source>Please be aware that as long as this wizard is active, audio will be looped locally to allow you to listen to it, and no audio will be sent to the server.</source>
        <translation type="obsolete">Bitte beachten Sie, dass, während dieser Assistent ausgeführt wird, jegliche Tondaten lokal zurückgekoppelt werden und keine Ton-Daten an den Server gesendet werden.</translation>
    </message>
    <message>
        <location/>
        <source>Finished</source>
        <translation>Fertig</translation>
    </message>
    <message>
        <location/>
        <source>Enjoy using Mumble</source>
        <translation>Viel Spaß mit Mumble</translation>
    </message>
    <message>
        <source>Congratulations. You should now be ready to enjoy a richer sound experience with Mumble.</source>
        <translation type="obsolete">Herzlichen Glückwunsch. Sie sollten nun eine reichere Sounderfahrung mit Mumble machen.</translation>
    </message>
    <message>
        <location/>
        <source>Device selection</source>
        <translation>Geräteauswahl</translation>
    </message>
    <message>
        <location/>
        <source>Selecting the input and output device to use with Mumble.</source>
        <translation>Wählen Sie das Eingabe- und Ausgabegerät aus, welches Mumble verwenden soll.</translation>
    </message>
    <message>
        <source>Input device</source>
        <translation type="obsolete">Eingabegerät</translation>
    </message>
    <message>
        <location/>
        <source>This is the device your microphone is connected to.</source>
        <translation>Dies ist das Gerät, mit dem ihr Mikrofon verbunden ist.</translation>
    </message>
    <message>
        <location/>
        <source>System</source>
        <translation>System</translation>
    </message>
    <message>
        <location/>
        <source>Input method for audio</source>
        <translation>Eingabemethode für Audio</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is the input method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation>&lt;b&gt;Dies ist die Eingabemethode für Audio.&lt;/b&gt;&lt;br /&gt;Am wahrscheinlichsten sollte DirectSound ausgewählt werden.</translation>
    </message>
    <message>
        <location/>
        <source>Device</source>
        <translation>Gerät</translation>
    </message>
    <message>
        <location/>
        <source>Input device to use</source>
        <translation>Eingabegerät</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Selects which sound card to use for audio input.&lt;/b&gt;</source>
        <translation>&lt;b&gt;Wählen Sie, welche Soundkarte für Audioeingaben verwendet werden soll.&lt;/b&gt;</translation>
    </message>
    <message>
        <source>Output device</source>
        <translation type="obsolete">Ausgabegerät</translation>
    </message>
    <message>
        <location/>
        <source>This is the device your speakers or headphones are connected to.</source>
        <translation>Dies ist das Gerät, an welches ihre Lautsprecher oder Kopfhörer angeschlossen sind.</translation>
    </message>
    <message>
        <location/>
        <source>Output method for audio</source>
        <translation>Ausgabemethode für Audio</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is the Output method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation>&lt;b&gt;Dies ist die Ausgabemethode für Audio.&lt;/b&gt;&lt;br /&gt;Am wahrscheinlichsten sollte DirectSound ausgewählt werden.</translation>
    </message>
    <message>
        <location/>
        <source>Output device to use</source>
        <translation>Ausgabegerät</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Selects which sound card to use for audio Output.&lt;/b&gt;</source>
        <translation>&lt;b&gt;Wählen Sie, welche Soundkarte für Audioausgaben verwendet werden soll.&lt;/b&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Volume tuning</source>
        <translation>Lautstärken-Einstellung</translation>
    </message>
    <message>
        <location/>
        <source>Tuning microphone hardware volume to optimal settings.</source>
        <translation>Mikrofonhardware-Lautstärke auf optimalen Wert einstellen.</translation>
    </message>
    <message>
        <source>Open your sound control panel and go to the recording settings. Make sure the microphone is selected as active input with maximum recording volume. If there&apos;s an option to enable a &quot;Microphone boost&quot; make sure it&apos;s checked.</source>
        <translation type="obsolete">Öffnen Sie die Lautstärkeeinstellungen und gehen Sie zu den Aufnahmeeinstellungen. Versichern Sie sich, dass das Mikrofon als aktives Eingabegerät mit maximaler Aufnahmelautstärke ausgewählt ist. Falls es eine Option &quot;Mikrofon Boost&quot; gibt, sollte diese aktiviert sein.</translation>
    </message>
    <message>
        <source>Speak loudly, as when you are annoyed or excited. Decrease the volume in the sound control panel until the bar below stays as high as possible in the blue and green but &lt;b&gt;not&lt;/b&gt; the red zone while you speak.</source>
        <translation type="obsolete">Sprechen Sie so laut, als wären Sie verärgert oder aufgeregt. Verringern Sie die Lautstärke in den Lautstärkeeinstellungen bis der Balken so weit wie möglich oben im blauen und grünen, aber nicht im roten Bereich ist, während Sie sprechen.</translation>
    </message>
    <message>
        <location/>
        <source>Now talk softly, as you would when talking late at night and you don&apos;t want to disturb anyone. Adjust the slider below so that the bar moves into green when you talk, but stays blue while you&apos;re silent.</source>
        <translation>Sprechen Sie jetzt so laut, als würden Sie spät nachts sprechen und niemanden stören wollen. Bewegen Sie den Schieber so, dass der Balken im Grünen ist während Sie sprechen, aber im Blauen bleibt wenn Sie nichts sagen.</translation>
    </message>
    <message>
        <location/>
        <source>Voice Activity Detection</source>
        <translation>Sprachaktivitätserkennung</translation>
    </message>
    <message>
        <location/>
        <source>Letting Mumble figure out when you&apos;re talking and when you&apos;re silent.</source>
        <translation>Lassen Sie Mumble herausfinden wann Sie sprechen und wann nicht.</translation>
    </message>
    <message>
        <location/>
        <source>This will help Mumble figure out when you are talking. The first step is selecting which data value to use.</source>
        <translation>Dies wird Mumble helfen herauszufinden, wann Sie sprechen. Der erste Schritt ist den zu benutzenden Datenwert auszuwählen.</translation>
    </message>
    <message>
        <location/>
        <source>Raw amplitude from input</source>
        <translation>Rohamplitude der Eingabe</translation>
    </message>
    <message>
        <location/>
        <source>Signal-To-Noise ratio</source>
        <translation>Signal-Rausch-Verhältnis</translation>
    </message>
    <message>
        <location/>
        <source>Next you need to adjust the following slider. The first few utterances you say should end up in the green area (definitive speech). While talking, you should stay inside the yellow (might be speech) and when you&apos;re not talking, everything should be in the red (definitively not speech).</source>
        <oldsource>Next you need to adjust the following two sliders. The first few utterances you say should end up in the green area (definitive speech). While talking, you should stay inside the yellow (might be speech) and when you&apos;re not talking, everything should be in the red (definitively not speech).</oldsource>
        <translation>Als nächstes müssen Sie den folgenden Schieber anpassen. Die ersten paar Geräusche die Sie beim Sprechen machen sollten im grünen Bereich (definitv Sprache) landen. Während Sie sprechen sollten Sie im gelben Bereich (könnte Sprache sein) bleiben und wenn Sie nicht sprechen, sollte alles im roten Bereich (definitiv keine Sprache) bleiben.</translation>
    </message>
    <message>
        <source>Last, you need to adjust the grace time. Mumble may detect this much non-speech and still keep transmitting before it breaks off. This allows you to catch your breath while speaking.</source>
        <translation type="obsolete">Als letztes müssen Sie die Aufschubzeit festlegen. Mumble kann so lange weiteren Ton aufnehmen und weiterhin übertragen, bevor es abbricht. Dies erlaubt ihnen während des Sprechens Luft zu holen.</translation>
    </message>
    <message>
        <location/>
        <source>Device tuning</source>
        <translation>Geräte-Einstellungen</translation>
    </message>
    <message>
        <location/>
        <source>Changing hardware output delays to their minimum value.</source>
        <translation>Setze Hardware Ausgabeverzögerungen auf ein Minimum.</translation>
    </message>
    <message>
        <source>To keep latency to an absolute minium, it&apos;s important to buffer as little audio as possible on the soundcard. However, many soundcards report that they require a much smaller buffer than what they can actually work with, so the only way to set this value is to try and fail.</source>
        <translation type="obsolete">Um die Latenz auf ein Minimum zu reduzieren ist es wichtig, so wenig Audio wie möglich auf der Soundkarte zu puffern. Viele Soundkarten geben jedoch einen deutlich geringeren Puffer an als jenen, den Sie eigentlich zum arbeiten benötigen. Deshalb ist der einzige Weg die Größe auszutesten.</translation>
    </message>
    <message>
        <source>You should hear a multitone chord. Change the slider below to the lowest value which gives &lt;b&gt;no&lt;/b&gt; interruptions or jitter in the sound.</source>
        <translation type="obsolete">Sie sollten einen Multiton-Akkord hören. Setzen Sie den Schieber auf den niedrigsten Wert, der &lt;b&gt;keine&lt;/b&gt; Unterbrechungen oder Jitter im Sound verursacht.</translation>
    </message>
    <message>
        <source>Amount of data to buffer.</source>
        <translation type="obsolete">Menge an zu puffernden Daten.</translation>
    </message>
    <message>
        <source>This sets the amount of data to prebuffer in the output buffer. Experiment with different values and set it to the lowest which doesn&apos;t cause rapid jitter in the sound.</source>
        <translation type="obsolete">Dies legt die Menge an Daten fest, die im Ausgabepuffer vorgepuffert werden sollen. Experimentieren Sie mit verschiedenen Werten und setzen Sie es auf den niedrigsten Wert, der noch kein schnelles Zittern oder Vibrieren im Ton verursacht.</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="+306"/>
        <source>%1 ms</source>
        <oldsource>%1ms</oldsource>
        <translation>%1 ms</translation>
    </message>
    <message>
        <source>%1 s</source>
        <translation type="obsolete">%1 s</translation>
    </message>
    <message>
        <source>You should hear a single tone that&apos;s changing in frequency. Change the slider below to the lowest value which gives &lt;b&gt;no&lt;/b&gt; interruptions or jitter in the sound. Please note that local echo is disabled during this test to improve audio path recognition.</source>
        <translation type="obsolete">Sie sollten eine Person reden hören. Schieben Sie den Regler auf den niedrigsten Wert der noch &lt;b&gt;keine&lt;/b&gt; Unterbrechungen oder Knacken im Ton liefert. Bitte beachten Sie, dass die lokale Rückkopplung ausgeschaltet ist, um die Ausgabe besser zu hören.</translation>
    </message>
    <message>
        <source>If you position the microphone so it can pick up the speakers or headset, Mumble will measure the total audio path delay in your system; this means the delay from a sample is placed in an outbound buffer until it&apos;s found in a matching incoming buffer.</source>
        <translation type="obsolete">Wenn Sie das Mikrofon so positionieren, dass es die Lautsprecher oder das Headset aufnimmt, wird Mumble die Audiopfad-Gesamtverzögerung in Ihrem System messen. Dies ist die Zeit zwischen dem Ausgeben eines Tons bis er im Eingangspuffer gefunden wird.</translation>
    </message>
    <message>
        <source>Audio path is %1ms long.</source>
        <translation type="obsolete">Audiopfad ist %1ms lang.</translation>
    </message>
    <message>
        <source>Audio path cannot be determined. Input not recognized.</source>
        <translation type="obsolete">Audiopfad konnte nicht bestimmt werden. Eingabe wurde nicht erkannt.</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui"/>
        <source>Use echo cancellation</source>
        <translation>Benutze Echounterdrückung</translation>
    </message>
    <message>
        <source>Cancel echo from headset or speakers.</source>
        <translation type="obsolete">Unterdrücke Echos vom Headset oder den Lautsprechern.</translation>
    </message>
    <message>
        <location/>
        <source>This enables echo cancellation of outgoing audio, which helps both on speakers and on headsets.</source>
        <translation>Dies aktiviert die Echo-Unterdrückung für ausgehende Audio-Signale, was sowohl bei Lautsprechern wie auch Headsets hilft.</translation>
    </message>
    <message>
        <location/>
        <source>Enable positional audio</source>
        <translation>Positionsabhängiges Audio aktivieren</translation>
    </message>
    <message>
        <location/>
        <source>Allows positioning of sound</source>
        <oldsource>Allows positioning of sound.</oldsource>
        <translation>Erlaubt die Positionierung der Audio-Signale</translation>
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
Dies ist Mumbles Assistent zum konfigurieren Ihrer Audio-Einstellungen. Er wird Ihnen helfen die korrekte Eingangslautstärke Ihrer Soundkarte und die korrekten Parameter für die Tonverarbeitung in Mumble zu wählen.
&lt;/p&gt;
&lt;p&gt;
Bitte beachten Sie, dass, so lange dieser Assistent aktiv ist, der Ton lokal ausgegeben wird damit Sie ihn hören können und kein Ton an den Server gesendet wird.
&lt;/p&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Input Device</source>
        <translation>Eingabegerät</translation>
    </message>
    <message>
        <location/>
        <source>Cancel echo from headset or speakers</source>
        <translation>Unterdrücke das Echo des Headsets oder der Lautsprecher</translation>
    </message>
    <message>
        <location/>
        <source>Output Device</source>
        <translation>Ausgabegerät</translation>
    </message>
    <message>
        <location/>
        <source>This allows Mumble to use positional audio to place voices.</source>
        <translation>Die Option erlaubt Mumble positionsabhängiges Audio zu verwenden um Stimmen zu platzieren.</translation>
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
Um die Latenz (Übertragungsverzögerung) auf einem absoluten Minimum zu halten ist es wichtig so wenig Ton wie möglich auf der Soundkarte zwischenzuspeichern (auch “puffern” genannt).
Leider geben viele Soundkarten an mit geringeren Puffern zu funktionieren, als Sie tatsächlich unterstützen. Daher müssen Sie selbst den optimalen Wert durch ausprobieren finden.
&lt;/p&gt;
&lt;p&gt;
Sie sollten eine Beispielaufnahme hören. Ändern Sie den Schieberegler zum niedrigst möglichen Wert, welcher noch &lt;b&gt;keine&lt;/b&gt; Unterbrechungen oder Tonstörungen verursacht.
Bitte beachten Sie, dass die lokale Wiedergabe Ihrer Aufnahmegeräte für diesen Test deaktiviert ist.
&lt;/p&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Amount of data to buffer</source>
        <translation>Menge an Daten die gepuffert werden sollen</translation>
    </message>
    <message>
        <location/>
        <source>This sets the amount of data to pre-buffer in the output buffer. Experiment with different values and set it to the lowest which doesn&apos;t cause rapid jitter in the sound.</source>
        <translation>Dies setzt die Menge an Daten, welche im Ausgabepuffer vorgepuffert werden. Experimentieren Sie mit verschiedenen Werten und setzen Sie ihn auf den niedrigsten, bei dem keine Tonstörungen auftreten.</translation>
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
Öffnen Sie Ihre Ton-Einstellungen (des Betriebssystems oder Ihrer Soundkarte) und wechseln Sie zu den Aufnahme-Einstellungen.
Stellen Sie sicher, dass das Mikrofon als aktives Aufnahmegerät gewählt ist und mit maximaler Lautstärke aufnimmt.
Sofern es eine Option für “Mikrofon-Boost” gibt stellen Sie sicher, dass diese ebenfalls gewählt ist (Diese Option finden Sie ggf. unter den erweiterten Einstellungen).
&lt;/p&gt;
&lt;p&gt;
Sprechen Sie so laut als wären Sie wütend oder aufgeregt.
Verringern Sie die Mikrofonlautstärke in Ihren Ton-Einstellungen so weit, dass der Balken unten so weit oben wie möglich im blauen und grünen, aber &lt;b&gt;nicht&lt;/b&gt; im roten Bereich ist, so lange Sie sprechen.
&lt;/p&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Quality &amp; Notifications</source>
        <translation>Qualität &amp; Hinweise</translation>
    </message>
    <message>
        <location/>
        <source>Quality settings</source>
        <translation>Qualitätseinstellungen</translation>
    </message>
    <message>
        <location/>
        <source>Low</source>
        <translation>Niedrig</translation>
    </message>
    <message>
        <location/>
        <source>Balanced</source>
        <translation>Ausgeglichen</translation>
    </message>
    <message>
        <location/>
        <source>Notification settings</source>
        <translation>Benachrichtigungen</translation>
    </message>
    <message>
        <location/>
        <source>Use Text-To-Speech to read notifications and messages to you.</source>
        <translation>Verwende Text-Zu-Sprache um Meldungen vorzulesen.</translation>
    </message>
    <message>
        <location/>
        <source>Disable Text-To-Speech and use sounds instead.</source>
        <translation>Text-zu-Sprache deaktivieren und Sounds benutzen.</translation>
    </message>
    <message>
        <location/>
        <source>Positional Audio</source>
        <translation>Positionsabhängiges Audio</translation>
    </message>
    <message>
        <location/>
        <source>Adjusting attenuation of positional audio.</source>
        <translation>Einstellen der Lautstärkenverminderung positionsabhängiger Audio-Signale.</translation>
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
Mumble unterstützt für manche Spiele positionsabhängigen Ton und kann die Stimmen der anderen Benutzer entsprechend Ihrer relativen Position zu Ihnen im Spiel positionieren.
Je nach ihrer Position wird die Lautstärke und die Position der Stimmen angepasst um die Richtung und Distanz von ihnen zu simulieren.
Eine solche Positionierung benötigt eine korrekte Lautsprechereinstellung in Ihrem Betriebssystem.
Deshalb wird dies hier getestet.
&lt;/p&gt;
&lt;p&gt;
Die untenstehende Grafik zeigt die Position von &lt;font color=&quot;red&quot;&gt;Ihnen&lt;/font&gt;, den &lt;font color=&quot;yellow&quot;&gt;Lautsprechern&lt;/font&gt; und einer &lt;font color=&quot;green&quot;&gt;sich bewegenden Tonquelle&lt;/font&gt; von oben betrachtet.
Sie sollten den Ton sich zwischen den Kanälen bewegen hören.
&lt;/p&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Use headphones instead of speakers</source>
        <translation>Benutze Kopfhörer anstatt Lautsprechern</translation>
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
Herzlichen Glückwunsch.
Sie sollten Mumble nun optimal benutzen können.
&lt;/p&gt;
&lt;p&gt;
Mumble wird ständig weiterentwickelt! Das Entwicklungsteam möchte sich auf Funktionen konzentrieren, welche den meisten Benutzern von Vorteil sind, deshalb kann Mumble anonyme Statistiken über Ihre Konfiguration an die Entwickler senden.
Diese Statistiken sind grundlegend für die weitere Entwicklung und stellen sicher, dass jene Funktionen die Sie verwenden nicht wegfallen.
&lt;/p&gt;</translation>
    </message>
    <message>
        <source>Mumble supports positional audio for some games, and will position the voice of other players relative to their position in game. Depending on their position, the volume of the voice will be changed between the speakers to simulate the direction and distance the other player is at. Such positioning depends on your speaker configuration being correct in your operating system, so a test is done here.</source>
        <translation type="obsolete">Mumble unterstützt positionsabhängiges Audio für einige Spiele und positioniert die Stimmen der anderen Spieler relativ zu Ihrer Position im Spiel. In Abhängigkeit von ihrer Position wird die Lautstärke der Stimmen geändert um Richtung und Abstand der Spieler zu simulieren. Solch eine Positionierung hängt von einer korrekten Lautsprecherkonfiguration in Ihrem Betriebssystem ab, weshalb hier ein Test gemacht wird.</translation>
    </message>
    <message>
        <source>The graph below shows the position of &lt;font color=&quot;red&quot;&gt;you&lt;/font&gt;, the &lt;font color=&quot;yellow&quot;&gt;speakers&lt;/font&gt; and a &lt;font color=&quot;green&quot;&gt;moving sound source&lt;/font&gt; as if seen from above. You should hear the audio move between the channels.</source>
        <translation type="obsolete">Die Grafik unten zeigt die Position von &lt;font color=&quot;red&quot;&gt;Ihnen&lt;/font&gt;, den &lt;font color=&quot;yellow&quot;&gt;Lautsprechern&lt;/font&gt; und einer sich &lt;font color=&quot;green&quot;&gt;bewegenden Quelle&lt;/font&gt; von oben gesehen. Sie sollten das Geräusch sich zwischen den Kanälen bewegen hören.</translation>
    </message>
    <message>
        <location/>
        <source>Use headphones</source>
        <translation>Benutze Kopfhörer</translation>
    </message>
    <message>
        <source>Use headphones instead of speakers.</source>
        <translation type="obsolete">Benutze Kopfhörer statt Lautsprechern.</translation>
    </message>
    <message>
        <location/>
        <source>This ignores the OS speaker configuration and configures the positioning for headphones instead.</source>
        <translation>Dies ignoriert die Betriebssystem Lautsprechereinstellungen und konfiguriert die Positionierung stattdessen für Kopfhörer.</translation>
    </message>
    <message>
        <source>Mumble is under continuous development, and the development team wants to focus on the features that benefit the most users. To this end, Mumble supports submitting anonymous statistics about your configuration to the developers. These statistcs are essential for future development, and also make sure the features you use aren&apos;t deprecated.</source>
        <translation type="obsolete">Mumble ist in ständiger Entwicklung und das Entwicklungsteam will sich auf die Features konzentrieren, die den meisten Benutzern nützen. Dazu unterstützt Mumble das Senden anonymer Statistiken über die Konfiguration. Diese Statistiken sind essentiell für die weitere Entwicklung und stellen sicher, dass die benutzten Features die Sie nutzen nicht entfernt werden.</translation>
    </message>
    <message>
        <location/>
        <source>Submit anonymous statistics to the Mumble project</source>
        <translation>Sende anonyme Statistiken an das Mumble Projekt</translation>
    </message>
    <message>
        <location/>
        <source>Push To Talk:</source>
        <translation>Push-To-Talk:</translation>
    </message>
    <message>
        <location/>
        <source>Adjust quality and notification settings.</source>
        <translation>Qualitätseinstellungen und Meldungen anpassen.</translation>
    </message>
    <message>
        <location/>
        <source>Enables attenuation of other applications while users talk to you</source>
        <translation>Aktiviert die Abschwächung anderer Anwendungen während Sie sprechen</translation>
    </message>
    <message>
        <source>Enables attenuation of other applications while users talk to you. This means that as soon someone is starting to speak to you in mumble the sound of all other applications, like audio players, will get attenuated so you can understand them better.</source>
        <translation type="obsolete">Aktiviert die Abschwächung anderer Andwendungen während anderre Benutzer zu ihnen sprechen. Das bedeutet, dass z.B. Musikprogramme automatisch leiser werden, sobald jemand mit zu Ihnen spricht.</translation>
    </message>
    <message>
        <location/>
        <source>Attenuate applications while other users talk</source>
        <translation>Schwächt Anwendungen ab während andere Benutzer sprechen</translation>
    </message>
    <message>
        <location/>
        <source>In this configuration Mumble will use a &lt;b&gt;low amount of bandwidth&lt;/b&gt;. This will inevitably result in high latency and poor quality. Choose this only if your connection cannot handle the other settings. (Speex 16kbit/s, 60ms per packet)</source>
        <translation>Mit dieser Einstellung verwendet Mumble eine &lt;b&gt;geringe Bandbreite&lt;/b&gt;. Dies ergibt eine hohe Latenz und geringe Qualität. Verwenden sie diese Einstellung nur wenn ihre Verbindung mit anderen Einstellungen nicht klarkommt. (Speec 16kbit/s, 60ms pro Paket)</translation>
    </message>
    <message>
        <location/>
        <source>This is the &lt;b&gt;recommended default&lt;/b&gt; configuration. It provides a good balance between quality, latency, and bandwidth usage. (CELT 40kbit/s, 20ms per packet)</source>
        <translation>Dies ist die &lt;b&gt;empfohlene&lt;/b&gt; Einstellung. Sie liefert eine gute Balance zwischen Qualität, Latenz und Bandbreite. (CELT 40kbit/s, 20ms pro Paket)</translation>
    </message>
    <message>
        <location/>
        <source>High</source>
        <translation>Hoch</translation>
    </message>
    <message>
        <location/>
        <source>This configuration is only recommended for use in setups where bandwidth is not an issue, like a LAN. It provides the lowest latency supported by Mumble and &lt;b&gt;high quality&lt;/b&gt;. (CELT 72kbit/s, 10ms per packet)</source>
        <translation>Diese Einstellung wird nur empfohlen wenn genügend Bandbreite vorhanden ist, z.B. in einem LAN. Sie bietet die geringste Latenz die von Mumble unterstützt wird und &lt;b&gt;hohe Qualität&lt;/b&gt;. (CELT 72kbit/s, 10ms pro Paket)</translation>
    </message>
    <message>
        <location/>
        <source>Custom</source>
        <translation>Benutzerdefiniert</translation>
    </message>
    <message>
        <location/>
        <source>You already set a customized quality configuration in Mumble. Select this setting to keep it.</source>
        <translation>Sie haben bereits angepasste Qualitätseinstellungen in Mumble. Mit dieser Einstellungen behalten Sie sie.</translation>
    </message>
    <message>
        <location/>
        <source>Enables attenuation of other applications while users talk to you. This means that as soon someone starts to speak to you in Mumble, the sound of all other applications (like audio players) will get attenuated so you can hear them more clearly.</source>
        <translation>Aktiviert die Herabsetzung der Lautstärke anderer Anwendungen während andere Benutzer zu Ihnen sprechen. Das heißt sobald jemand in Mumble zu Ihnen spricht wird die Lautstärke aller anderen Programme (etwa einem Musik-/Mediaplayer) herabgesetzt, damit Sie die zu Ihnen sprechende Person besser verstehen können.</translation>
    </message>
    <message>
        <location/>
        <source>Apply some high contrast optimizations for visually impaired users</source>
        <translation>Auf hohen Kontrast optimierte Darstellung für sehbehinderte Benutzer verwenden</translation>
    </message>
    <message>
        <location/>
        <source>Use high contrast graphics</source>
        <translation>Anzeigen mit hohem Kontrast verwenden</translation>
    </message>
    <message>
        <location/>
        <source>Keep custom Text-To-Speech settings.</source>
        <translation>Derzeitige Text-Zu-Sprache-Einstellungen beibehalten.</translation>
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
        <translation>&lt;p&gt;
Öffnen Sie Ihre Ton-Einstellungen (des Betriebssystems oder Ihrer Soundkarte) und wechseln Sie zu den Aufnahme-Einstellungen.
Stellen Sie sicher, dass das Mikrofon als aktives Aufnahmegerät gewählt ist und mit maximaler Lautstärke aufnimmt.
Sofern es eine Option für “Mikrofon-Boost” gibt stellen Sie sicher, dass diese ebenfalls gewählt ist (Diese Option finden Sie ggf. unter den erweiterten Einstellungen).
&lt;/p&gt;
&lt;p&gt;
Sprechen Sie so laut als wären Sie wütend oder aufgeregt.
Verringern Sie die Mikrofonlautstärke in Ihren Ton-Einstellungen so weit, dass der Balken unten so weit oben wie möglich im gestreiften und leeren, aber &lt;b&gt;nicht&lt;/b&gt; im karierten Bereich ist während Sie sprechen.
&lt;/p&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Now talk softly, as you would when talking late at night and you don&apos;t want to disturb anyone. Adjust the slider below so that the bar moves into empty zone when you talk, but stays in the striped one while you&apos;re silent.</source>
        <comment>For high contrast mode</comment>
        <translation>Sprechen Sie jetzt so laut, als würden Sie spät nachts sprechen und niemanden stören wollen. Bewegen Sie den Schieber so, dass der Balken im leeren Bereich ist während Sie sprechen, aber im gestreiften Bereich bleibt wenn Sie nichts sagen.</translation>
    </message>
    <message>
        <source>Next you need to adjust the following slider. The first few utterances you say should end up in the empty area (definitive speech). While talking, you should stay inside the striped (might be speech) and when you&apos;re not talking, everything should be in the criss-crossed (definitively not speech).</source>
        <comment>For high contrast mode</comment>
        <translation type="obsolete">Als nächstes müssen Sie den folgenden Schieber anpassen. Die ersten paar Geräusche die Sie beim Sprechen machen sollten im leeren Bereich (definitv Sprache) landen. Während Sie sprechen sollten Sie im gestreiften Bereich (könnte Sprache sein) bleiben und wenn Sie nicht sprechen, sollte alles im karierten Bereich (definitiv keine Sprache) bleiben.</translation>
    </message>
    <message>
        <location/>
        <source>Next you need to adjust the following slider. The first few utterances you say should end up in the empty area (definitive speech). While talking, you should stay inside the striped (might be speech) and when you&apos;re not talking, everything should be in the crisscrossed (definitively not speech).</source>
        <comment>For high contrast mode</comment>
        <translation>Als nächstes müssen Sie den folgenden Schieber anpassen. Die ersten paar Geräusche die Sie beim Sprechen machen sollten im leeren Bereich (definitiv Sprache) landen. Während Sie sprechen sollten Sie im gestreiften Bereich (könnte Sprache sein) bleiben und wenn Sie nicht sprechen, sollte alles im karierten Bereich (definitiv keine Sprache) bleiben.</translation>
    </message>
</context>
<context>
    <name>BanEditor</name>
    <message>
        <location filename="BanEditor.ui"/>
        <source>Reason</source>
        <translation>Begründung</translation>
    </message>
    <message>
        <location/>
        <source>Start</source>
        <translation>Start</translation>
    </message>
    <message>
        <location/>
        <source>End</source>
        <translation>Ende</translation>
    </message>
    <message>
        <location/>
        <source>User</source>
        <translation>Benutzer</translation>
    </message>
    <message>
        <location/>
        <source>Hash</source>
        <translation>Hash</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Add</source>
        <translation>&amp;Hinzufügen</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Update</source>
        <translation>&amp;Aktualisieren</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Remove</source>
        <translation>&amp;Entfernen</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>Accept changes</source>
        <translation type="obsolete">Änderungen übernehmen</translation>
    </message>
    <message>
        <source>This button will accept current groups/ACLs and send them to the server. Note that if you mistakenly remove write permission from yourself, the server will add it.</source>
        <translation type="obsolete">Dieser Button wird aktuelle Gruppen/ACLs akzeptieren und sie an den Server senden. Wenn versehentlich Schreibrechte entfernt wurden, wird der Server sie hinzufügen.</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Abbrechen</translation>
    </message>
    <message>
        <source>Reject changes</source>
        <translation type="obsolete">Änderungen nicht übernehmen</translation>
    </message>
    <message>
        <source>This button will cancels all changes and closes the dialog without updating the ACLs or groups on the server.</source>
        <translation type="obsolete">Dieser Button macht alle Änderungen rückgängig und schließt den Dialog ohne ACLs oder Gruppen auf dem Server zu aktualisieren. </translation>
    </message>
    <message>
        <location/>
        <source>Mumble - Edit Bans</source>
        <translation>Mumble - Bannliste bearbeiten</translation>
    </message>
    <message>
        <source>0.0.0.0</source>
        <translation type="obsolete">0.0.0.0</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Address</source>
        <translation>&amp;Adresse</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Mask</source>
        <translation>&amp;Maske</translation>
    </message>
</context>
<context>
    <name>CertView</name>
    <message>
        <location filename="Cert.cpp" line="+50"/>
        <source>Name</source>
        <translation>Name</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Email</source>
        <translation>Email</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Issuer</source>
        <translation>Aussteller</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Expiry Date</source>
        <translation>Verfallsdatum</translation>
    </message>
    <message>
        <location line="+34"/>
        <source>(none)</source>
        <translation>(keiner)</translation>
    </message>
    <message>
        <location line="+11"/>
        <source>Self-signed</source>
        <translation>Selbst signiert</translation>
    </message>
</context>
<context>
    <name>CertWizard</name>
    <message>
        <location line="+90"/>
        <source>Resolving domain %1.</source>
        <translation>Löse Domain %1 auf.</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Unable to validate email.&lt;br /&gt;Enter a valid (or blank) email to continue.</source>
        <translation>Email konnte nicht validiert werden.&lt;br /&gt;
Geben Sie eine korrekte (oder leere) E-Mail Adresse an um fortzufahren.</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>There was an error generating your certificate.&lt;br /&gt;Please try again.</source>
        <translation>Beim generieren Ihres Zertifikats ist ein Fehler aufgetreten.&lt;br /&gt;Bitte versuchen Sie es erneut.</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Your certificate and key could not be exported to PKCS#12 format. There might be an error in your certificate.</source>
        <translation>Ihr Zertifikat und der Schlüssel konnten nicht in das Format PKCS#12 exportiert werden.
Ihr Zertifikat könnte einen Fehler enthalten.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>The file could not be opened for writing. Please use another file.</source>
        <translation>Die Datei konnte nicht zum Schreiben geöffnet werden.
Bitte verwenden Sie eine andere Datei.</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>The file could not be written successfully. Please use another file.</source>
        <translation>Die Datei konnte nicht beschrieben werden.
Bitte verwenden Sie eine andere Datei.</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>The file could not be opened for reading. Please use another file.</source>
        <translation>Die Datei konnte nicht zum Lesen geöffnet werden.
Bitte verwenden Sie eine andere Datei.</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>The file is empty or could not be read. Please use another file.</source>
        <translation>Die Datei ist leer oder konnte nicht gelesen werden.
Bitte verwenden Sie eine andere Datei.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>The file did not contain a valid certificate and key. Please use another file.</source>
        <translation>Die Datei enthielt kein gültiges Zertifikat und Schlüssel.
Bitte verwenden Sie eine andere Datei.</translation>
    </message>
    <message>
        <location line="+22"/>
        <source>Select file to export certificate to</source>
        <translation>Wählen Sie eine Datei, in welche das Zertifikat exportiert werden soll</translation>
    </message>
    <message>
        <location line="+37"/>
        <source>Select file to import certificate from</source>
        <translation>Wählen Sie eine Datei, aus welcher ein Zertifikat importiert werden soll</translation>
    </message>
    <message>
        <location line="+57"/>
        <source>Unable to resolve domain.</source>
        <translation>Auflösen der Domain nicht möglich.</translation>
    </message>
    <message>
        <location line="+43"/>
        <source>Mumble User</source>
        <translation>Mumble-Benutzer</translation>
    </message>
    <message>
        <location filename="main.cpp" line="+425"/>
        <source>&lt;b&gt;Certificate Expiry:&lt;/b&gt; Your certificate is about to expire. You need to renew it, or you will no longer be able to connect to servers you are registered on.</source>
        <translation>&lt;b&gt;Ablauf des Zertifikats:&lt;/b&gt; Ihr Zertifikat wird bald ablaufen. Sie müssen es erneuern oder Sie werden nicht mehr in der Lage sein sich auf Server zu verbinden auf denen Sie registriert sind.</translation>
    </message>
</context>
<context>
    <name>Certificates</name>
    <message>
        <location filename="Cert.ui"/>
        <source>Certificate Management</source>
        <translation>Zertifikat-Management</translation>
    </message>
    <message>
        <source>This opens a file selection dialog to choose a file to export the certificate to.</source>
        <translation type="obsolete">Dies öffnet ein Dateiauswahl-Fenster um eine Datei zu wählen, in welche das Zertifikat exportiert wird.</translation>
    </message>
    <message>
        <location/>
        <source>Certificate Authentication</source>
        <translation>Zertifikat Authentifikation</translation>
    </message>
    <message>
        <location/>
        <source>Authenticating to servers without using passwords</source>
        <translation>Gegenüber Servern ohne Passwort authentifizieren</translation>
    </message>
    <message>
        <source>&lt;p&gt;Mumble can use certificates to authenticate with servers. Using certificates avoids passwords, meaning you don&apos;t need to disclose any password to the remote site. It also enables very easy user registration.&lt;/p&gt;&lt;p&gt;While Mumble can work without certificates, the majority of servers will expect you to have one.&lt;/p&gt;</source>
        <translation type="obsolete">&lt;p&gt;
Mumble kann Zertifikate verwenden, um sich gegenüber Servern zu authentifizieren.
Das Verwenden von Zertifikaten vermeidet die Nutzung von Passwörtern, was wiederum heißt ,dass Sie der anderen Seite kein Passwort mitteilen müssen.
Sie erlaubt außerdem eine sehr einfache Benutzerregistrierung.
&lt;/p&gt;
&lt;p&gt;
Auch wenn Mumble ohne Zertifikate funktioniert werden die meisten Server erwarten, dass Sie eines haben.
&lt;/p&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Current certificate</source>
        <translation>Aktuelles Zertifikat</translation>
    </message>
    <message>
        <location/>
        <source>This is the certificate Mumble currently uses.</source>
        <translation>Dies ist das Zertifikat, welches Mumble momentan verwendet.</translation>
    </message>
    <message>
        <location/>
        <source>Current Certificate</source>
        <translation>Aktuelles Zertifikat</translation>
    </message>
    <message>
        <location/>
        <source>Create a new certificate</source>
        <translation>Neues Zertifikat erstellen</translation>
    </message>
    <message>
        <location/>
        <source>This will create a new certificate.</source>
        <translation>Hiermit wird ein neues Zertifikat erstellt.</translation>
    </message>
    <message>
        <location/>
        <source>Import certificate from file</source>
        <translation>Zertifikat aus einer Datei importieren</translation>
    </message>
    <message>
        <location/>
        <source>This will import a certificate from file.</source>
        <translation>Hiermit wird ein Zertifikat aus einer Datei importiert.</translation>
    </message>
    <message>
        <location/>
        <source>Import a certificate</source>
        <translation>Zertifikat importieren</translation>
    </message>
    <message>
        <location/>
        <source>Export Certificate</source>
        <translation>Zertifikat exportieren</translation>
    </message>
    <message>
        <location/>
        <source>This will export a certificate to file.</source>
        <translation>Hiermit wird ein Zertifikat in eine Datei exportiert.</translation>
    </message>
    <message>
        <location/>
        <source>Export current certificate</source>
        <translation>Aktuelles Zertifikat exportieren</translation>
    </message>
    <message>
        <location/>
        <source>Import Certificate</source>
        <translation>Zertifikat importieren</translation>
    </message>
    <message>
        <location/>
        <source>PKCS #12 Certificate import</source>
        <translation>PKCS#12-Zertifikat Import</translation>
    </message>
    <message>
        <location/>
        <source>&lt;p&gt;Mumble can import certificates stored in PKCS #12 format. This is the format used when exporting a key from Mumble, and also when exporting keys from Firefox, Internet Explorer, Opera etc.&lt;/p&gt;&lt;p&gt;If the file is password protected, you will need the password to import the certificate.&lt;/p&gt;</source>
        <oldsource>&lt;p&gt;Mumble can import certificates stored in PKCS #12 format. This is the format used when exporting a key from Mumble, and also when exporting keys from FireFox, Internet Explorer, Opera etc.&lt;/p&gt;&lt;p&gt;If the file is password protected, you will need the password to import the certificate.&lt;/p&gt;</oldsource>
        <translation>&lt;p&gt;
Mumble kann Zertifikate, welche im PKCS#12-Format gespeichert sind, importieren.
Dieses Format wird beim exportieren eines Schlüssels aus Mumble verwendet, genauso wie beim exportieren von Schlüsseln aus FireFox, dem Internet Explorer, Opera usw.
&lt;/p&gt;
&lt;p&gt;
Wenn die Datei passwortgeschützt ist benötigen Sie dieses, um das Zertifikat zu importieren.
&lt;/p&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Import from</source>
        <translation>Importieren aus</translation>
    </message>
    <message>
        <location/>
        <source>Filename to import from</source>
        <translation>Datei aus welcher importiert werden soll</translation>
    </message>
    <message>
        <location/>
        <source>This is the filename you wish to import a certificate from.</source>
        <translation>Dies ist die Datei, aus welcher Sie ein Zertifikat importieren möchten.</translation>
    </message>
    <message>
        <location/>
        <source>Select file to import from</source>
        <translation>Wählen Sie die Datei, aus welcher importiert werden soll</translation>
    </message>
    <message>
        <location/>
        <source>This opens a file selection dialog to choose a file to import a certificate from.</source>
        <translation>Dies öffnet ein Dateiauswahlfenster zur Auswahl der Datei, aus welcher das Zertifikat importiert werden soll.</translation>
    </message>
    <message>
        <location/>
        <source>Open...</source>
        <translation>Öffnen...</translation>
    </message>
    <message>
        <location/>
        <source>Password</source>
        <translation>Passwort</translation>
    </message>
    <message>
        <location/>
        <source>Password for PKCS#12 file</source>
        <translation>Passwort für PKCS#12-Datei</translation>
    </message>
    <message>
        <location/>
        <source>This is the password for the PKCS#12 file containing your certificate.</source>
        <translation>Dies ist das Password für die PKCS#12-Datei, welche Ihr Zertifikat enthält.</translation>
    </message>
    <message>
        <location/>
        <source>Certificate to import</source>
        <translation>Zu importierendes Zertifikat</translation>
    </message>
    <message>
        <location/>
        <source>This is the certificate you are importing.</source>
        <translation>Dies ist das Zertifikat, welches Sie importieren.</translation>
    </message>
    <message>
        <location/>
        <source>Certificate Details</source>
        <translation>Zertifikat-Details</translation>
    </message>
    <message>
        <location/>
        <source>Replace Certificate</source>
        <translation>Zertifikat ersetzen</translation>
    </message>
    <message>
        <location/>
        <source>Replace existing certificate with new certificate?</source>
        <translation>Soll das vorhandene Zertifikat durch das Neue ersetzt werden?</translation>
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
        <translation>&lt;p&gt;Sie haben bereits ein Zertifikat in Mumble gespeichert und sind dabei dieses zu ersetzen.&lt;/p&gt;
&lt;p&gt;Wenn Sie ein vorhandenes, von einer vertrauenswürdigen Zertifizierungsstelle erzeugtes Zertifikat mit gleicher E-Mail Adresse nur aktualisieren ist dies sicher. Server werden automatisch Ihr vertrauenswürdiges Zertifikat für Ihre E-Mail Adresse erkennen.&lt;/p&gt;
&lt;p&gt;
Sollte dies nicht der Fall sein werden Sie von keinem Server mehr erkannt, bei dem Sie sich bisher authentifiziert haben.
Wenn Sie sich noch nirgends registriert haben müssen Sie sich darüber keine Gedanken machen.
&lt;/p&gt;
&lt;p&gt;
Sind Sie sicher, dass Sie Ihr Zertifikat ersetzen möchten?
&lt;/p&gt;</translation>
    </message>
    <message>
        <location/>
        <source>This is the certificate Mumble currently uses. It will be replaced.</source>
        <translation>Dies ist das aktuell von Mumble verwendete Zertifikat. Es wird ersetzt.</translation>
    </message>
    <message>
        <location/>
        <source>New certificate</source>
        <translation>Neues Zertifikat</translation>
    </message>
    <message>
        <location/>
        <source>This is the new certificate that will replace the old one.</source>
        <translation>Dies ist das neue Zertifikat, welches das Alte ersetzen wird.</translation>
    </message>
    <message>
        <location/>
        <source>New Certificate</source>
        <translation>Neues Zertifikat</translation>
    </message>
    <message>
        <location/>
        <source>Make a backup of your certificate</source>
        <translation>Erstelle eine Sicherheitskopie des Zertifikats</translation>
    </message>
    <message>
        <source>&lt;p&gt;If you ever lose your current certificate, which will happen if your computer suffers a hardware failure or you reinstall your machine, you will no longer be able to authenticate to any server you are registered on. It is therefore &lt;b&gt;mandatory&lt;/b&gt; that you make a backup of your certificate. We strongly recommend you store this backup on removable storage, such as an USB memory stick.&lt;/p&gt;
&lt;p&gt;Note that this file will not be encrypted, and if anyone gains access to it, they will be able to impersonate you, so take good care of it.&lt;/p&gt;</source>
        <translation type="obsolete">&lt;p&gt;
Wenn Sie Ihr aktuelles Zertifikat verlieren, was etwa passiert wenn in ihrem Computer Hardware kaputt geht und Sie Ihr System neu installieren müssen, können Sie sich nicht mehr gegenüber Servern authentifizieren, auf welchen Sie sich registriert haben.
Deshalb ist es &lt;b&gt;notwendig&lt;/b&gt;, dass Sie eine Sicherung Ihres Zertifikats erstellen.
Diese Sicherung sollte auf jeden Fall auf einem externen Datenträger, etwa einem USB Stick, gespeichert werden.
&lt;/p&gt;
&lt;p&gt;
Beachten Sie, dass die Datei nicht verschlüsselt. Jeder der Zugriff auf diese Datei hat kann ihre Identität annehmen.
Stellen Sie also sicher, dass nur Sie Zugriff auf diese Datei haben.
&lt;/p&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Export to</source>
        <translation>Exportieren nach</translation>
    </message>
    <message>
        <location/>
        <source>Filename to export to</source>
        <translation>Datei, in welche exportiert werden soll</translation>
    </message>
    <message>
        <location/>
        <source>This is the filename you wish to export a certificate to.</source>
        <translation>Dies ist die Datei, in welche das Zertifikat exportiert wird.</translation>
    </message>
    <message>
        <location/>
        <source>Save As...</source>
        <translation>Speichern unter...</translation>
    </message>
    <message>
        <location/>
        <source>This is the certificate Mumble currently uses. It will be exported.</source>
        <translation>Dies ist das Zertifikat, welches Mumble derzeit verwendet. Es wird exportiert.</translation>
    </message>
    <message>
        <location/>
        <source>Generate a new certificate for strong authentication</source>
        <translation>Generiere ein neues Zertifikat zur sicheren Authentifikation</translation>
    </message>
    <message>
        <location/>
        <source>&lt;p&gt;Mumble will now generate a strong certificate for authentication to servers.&lt;/p&gt;&lt;p&gt;If you wish, you may provide some additional information to be stored in the certificate, which will be presented to servers when you connect. If you provide a valid email address, you can upgrade to a CA issued email certificate later on, which provides strong identification.&lt;/p&gt;</source>
        <translation>&lt;p&gt;Mumble wird nun ein sicheres Zertifikat zur Authentifikation gegenüber Servern generieren.&lt;/p&gt;
&lt;p&gt;
Wenn Sie möchten können Sie einige zusätzliche Informationen, welche im Zertifikat gespeichert werden, angeben.
Diese werden beim Verbinden zum Server diesem ebenfalls übermittelt.
Wenn Sie eine korrekte E-Mail Adresse angeben können Sie zu einem von einer Zertifizierungstelle erzeugten Zertifikat upgraden, welches sichere Identifikation erlaubt.
&lt;/p&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Name</source>
        <translation>Name</translation>
    </message>
    <message>
        <location/>
        <source>Email</source>
        <translation>E-Mail</translation>
    </message>
    <message>
        <location/>
        <source>Your email address (e.g. johndoe@mumble.info)</source>
        <translation>Ihre E-Mail-Adresse (z.B. johndoe@mumble.info)</translation>
    </message>
    <message>
        <location/>
        <source>This is your email address. It is strongly recommended to provide a valid email address, as this will allow you to upgrade to a strong certificate without authentication problems.</source>
        <translation>Dies ist Ihre E-Mail Adresse.
Es wird dringend empfohlen hier eine korrekte E-Mail Adresse anzugeben, dieses erlaubt Ihnen später ohne Probleme zu einem vertrauenswürdigen Zertifikat zu upgraden.</translation>
    </message>
    <message>
        <location/>
        <source>Your name (e.g. John Doe)</source>
        <translation>Ihr Name (z.B. John Doe)</translation>
    </message>
    <message>
        <location/>
        <source>This is your name, and will be filled out in the certificate. This field is entirely optional.</source>
        <translation>Dies ist Ihr Name und wird in Ihrem Zertifikat gespeichert.
Dieses Feld ist optional.</translation>
    </message>
    <message>
        <location/>
        <source>Finish</source>
        <translation>Fertigstellen</translation>
    </message>
    <message>
        <location/>
        <source>Certificate-based authentication is ready for use</source>
        <translation>Die Zertifikat-basierte Authentifikation ist zur Verwendung bereit</translation>
    </message>
    <message>
        <location/>
        <source>Enjoy using Mumble with strong authentication.</source>
        <translation>Viel Spaß bei der Verwendung von Mumble mit sicherer Authentifikation.</translation>
    </message>
    <message>
        <location/>
        <source>Automatic certificate creation</source>
        <translation>Zertifikat automatisch erstellen</translation>
    </message>
    <message>
        <location/>
        <source>&lt;p&gt;Mumble can use certificates to authenticate with servers. Using certificates avoids passwords, meaning you don&apos;t need to disclose any password to the remote site. It also enables very easy user registration.&lt;/p&gt;&lt;p&gt;While Mumble can work without certificates, the majority of servers will expect you to have one.&lt;/p&gt;
&lt;p&gt;
It is &lt;b&gt;strongly&lt;/b&gt; recommended that you &lt;a href=&quot;http://mumble.info/certificate.php&quot;&gt;create a trusted certificate&lt;/a&gt;.
&lt;/p&gt;
</source>
        <translation>&lt;p&gt;Mumble kann Zertifikate verwenden um sich beim Server zu authentifizieren. Die Benutzung von Zertifikaten hat den Vorteil, dass man nicht mit Passwörtern hantieren muss. Ausserdem ermöglichen Zertifikate die einfache Registrierung von Benutzern.&lt;/p&gt;&lt;p&gt;Mumble funktioniert auch ohne Zertifikate, jedoch erwartet die Mehrheit der Server, dass Sie über ein Zertifikat verfügen.&lt;/p&gt;
&lt;p&gt;
Es wird &lt;b&gt;sehr&lt;/b&gt; empfohlen, dass Sie &lt;a href=&quot;http://mumble.info/certificate.php&quot;&gt;ihr eigenes Zertifikat erstellen&lt;/a&gt;.
&lt;/p&gt;
</translation>
    </message>
    <message>
        <location/>
        <source>&lt;p&gt;If you ever lose your current certificate, which will happen if your computer suffers a hardware failure or you reinstall your machine, you will no longer be able to authenticate to any server you are registered on. It is therefore &lt;b&gt;mandatory&lt;/b&gt; that you make a backup of your certificate. We strongly recommend you store this backup on removable storage, such as a USB flash drive.&lt;/p&gt;
&lt;p&gt;Note that this file will not be encrypted, and if anyone gains access to it, they will be able to impersonate you, so take good care of it.&lt;/p&gt;</source>
        <translation>&lt;p&gt;Wenn Sie Ihr aktuelles Zertifikat verlieren, was etwa passiert wenn in ihrem Computer Hardware kaputt geht und Sie Ihr System neu installieren müssen, können Sie sich nicht mehr gegenüber Servern authentifizieren, auf welchen Sie sich registriert haben. Deshalb ist es &lt;b&gt;notwendig&lt;/b&gt;, dass Sie eine Sicherung Ihres Zertifikats erstellen. Diese Sicherung sollte auf jeden Fall auf einem externen Datenträger, z.B. einem USB Stick, gespeichert werden.&lt;/p&gt;
&lt;p&gt;Beachten Sie, dass die Datei nicht verschlüsselt wird. Jeder der Zugriff auf diese Datei hat kann ihre Identität annehmen. Stellen Sie also sicher, dass nur Sie Zugriff auf diese Datei haben.&lt;/p&gt;</translation>
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
        <translation>Keine</translation>
    </message>
    <message>
        <source>Write</source>
        <translation type="obsolete">Schreiben</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Traverse</source>
        <translation>Durchqueren</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Enter</source>
        <translation>Betreten</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Speak</source>
        <translation>Sprechen</translation>
    </message>
    <message>
        <source>AltSpeak</source>
        <translation type="obsolete">AltSpeak</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Mute/Deafen</source>
        <translation>Stumm-/Taubstellen</translation>
    </message>
    <message>
        <source>Move/Kick</source>
        <translation type="obsolete">Verschieben/Kicken</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Make channel</source>
        <translation>Kanal erstellen</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Make temporary</source>
        <translation>Temporären Kanal erstellen</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Link channel</source>
        <translation>Kanal verbinden</translation>
    </message>
    <message>
        <location line="-87"/>
        <source>This represents no privileges.</source>
        <translation>Dies repräsentiert keine Rechte.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>This represents total access to the channel, including the ability to change group and ACL information. This privilege implies all other privileges.</source>
        <translation>Dies repräsentiert vollen Zugriff auf den Kanal, einschließlich den Fähigkeiten Gruppen und Berechtigungen zu ändern. Dieses Recht impliziert alle anderen Rechte.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>This represents the permission to traverse the channel. If a user is denied this privilege, he will be unable to access this channel and any sub-channels in any way, regardless of other permissions in the sub-channels.</source>
        <translation>Dies repräsentiert das Recht den Kanal zu durchqueren. Wenn ein Benutzer dieses Privileg nicht hat, kann er nicht auf diesen Kanal oder einen der Unterkanäle zugreifen, egal welche Rechte er in den Unterkanälen hat.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>This represents the permission to join the channel. If you have a hierarchical channel structure, you might want to give everyone Traverse, but restrict Enter in the root of your hierarchy.</source>
        <translation>Dies repräsentiert das Recht den Kanal zu betreten. Hat man eine hierarchische Kanalstruktur, sollte man jedem das Recht Durchqueren geben und dafür Betreten in der Wurzel der Hierarchie einschränken.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>This represents the permission to speak in a channel. Users without this privilege will be suppressed by the server (seen as muted), and will be unable to speak until they are unmuted by someone with the appropriate privileges.</source>
        <translation>Dies repräsentiert das Recht in einem Kanal zu sprechen. Benutzer ohne dieses Recht werden vom Server unterdrückt (gekennzeichnet als stumm) und können in diesem Kanal erst sprechen, wenn sie von jemandem mit den entsprechenden Rechten ent-stummgestellt werden.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>This represents the permission to whisper to this channel from the outside. This works exactly like the &lt;i&gt;speak&lt;/i&gt; privilege, but applies to packets spoken with the Whisper key held down. This may be used to broadcast to a hierarchy of channels without linking.</source>
        <translation>Dies repräsentiert das Recht von außerhalb in diesen Kanal zu flüstern.
Dies funktioniert exakt wie das &lt;i&gt;Sprechen&lt;/i&gt;-Recht gilt aber für Sprachpakete mit gedrückter Flüstern-Taste.
Dies kann verwendet werden, um zu einer Hierarchie von Kanälen zu sprechen ohne diese zu verbinden.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>This represents the permission to mute and deafen other users. Once muted, a user will stay muted until he is unmuted by another privileged user or reconnects to the server.</source>
        <translation>Dies representiert das Recht andere Benutzer stumm und taub zu stellen. Wenn ein Benutzer stummgestellt wird bleibt er dies, bis ein anderer privilegierten Benutzer das stummstellen entfernt, oder der Benutzer neu zum Server verbindet.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>This represents the permission to move a user to another channel or kick him from the server. To actually move the user, either the moving user must have Move privileges in the destination channel, or the user must normally be allowed to enter the channel. Users with this privilege can move users into channels the target user normally wouldn&apos;t have permission to enter.</source>
        <translation>Dies repräsentiert das Recht einen Benutzer in einen anderen Kanal zu verschieben oder ihn vom Server zu kicken.
Um einen Benutzer auch wirklich zu verschieben muss der verschiebende Benutzer entweder Verschieben/Kick-Rechte im Zielkanal haben oder dem verschobenen Benutzer muss es erlaubt sein, den Zielkanal zu betreten.
Benutzer mit diesem Recht können also (ausreichende Rechte im Zielkanal vorausgesetzt) andere Benutzer in Kanäle verschieben, die diese normalerweise nicht betreten dürfen.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>This represents the permission to make sub-channels. The user making the sub-channel will be added to the admin group of the sub-channel.</source>
        <translation>Dies repräsentiert das Recht Unterkanäle zu erstellen.
Der Benutzer, welcher den Unterkanal erstellt, wird automatisch zur admin-Gruppe des Kanals hinzugefügt.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>This represents the permission to make a temporary subchannel. The user making the sub-channel will be added to the admin group of the sub-channel. Temporary channels are not stored and disappear when the last user leaves.</source>
        <translation>Dies repräsentiert das Recht temporäre Unterkanäle zu erstellen.
Der Benutzer, welcher den Unterkanal erstellt, wird automatisch zur admin-Gruppe des Unterkanals hinzugefügt.
Temporäre Kanäle werden nicht gespeichert und verschwinden, sobald der letzte Benutzer den Kanal verlässt.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>This represents the permission to link channels. Users in linked channels hear each other, as long as the speaking user has the &lt;i&gt;speak&lt;/i&gt; privilege in the channel of the listener. You need the link privilege in both channels to create a link, but just in either channel to remove it.</source>
        <translation>Dies repräsentiert das Recht Kanäle zu verbinden.
Benutzer in verbundenen Kanälen können sich gegenseitig hören, so lange der sprechende Benutzer das &lt;i&gt;Sprechen&lt;/i&gt;-Recht im Kanal des Hörers hat.
Man benötigt Verbindungsrechte in beiden Kanälen um eine Verbindung aufzubauen, aber nur in einem der Beiden um sie aufzuheben.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>This represents the permission to write text messages to other users in this channel.</source>
        <translation>Dies repräsentiert das Recht Textnachrichten an andere Benutzer dieses Kanals zu senden.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>This represents the permission to forcibly remove users from the server.</source>
        <translation>Dies repräsentiert das Recht, andere Benutzer zwingend vom Server zu entfernen (zu “kicken”).</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>This represents the permission to permanently remove users from the server.</source>
        <translation>Dies repräsentiert das Recht Benutzer permanent vom Server zu entfernen (zu ”bannen”).</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>This represents the permission to register and unregister users on the server.</source>
        <translation>Dies repräsentiert das Recht andere Benutzer auf dem Server zu registrieren und diese auch wieder zu entfernen.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>This represents the permission to register oneself on the server.</source>
        <translation>Dies repräsentiert das Recht sich selbst auf dem Server zu registrieren.</translation>
    </message>
    <message>
        <location line="+31"/>
        <source>Whisper</source>
        <translation>Flüstern</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Move</source>
        <translation>Bewegen</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Text message</source>
        <translation>Textnachricht</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Kick</source>
        <translation>Kicken</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Ban</source>
        <translation>Bannen</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Register User</source>
        <translation>Benutzer registrieren</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Register Self</source>
        <translation>Selbst registrieren</translation>
    </message>
    <message>
        <source>This represents the permission to speak in a channel with flagged speech. This works exactly like the &lt;i&gt;speak&lt;/i&gt; privilege, but applies to packets spoken with AltPushToTalk held down. This may be used to broadcast to a hierarchy of channels without linking.</source>
        <translation type="obsolete">Dies repräsentiert das Recht in einem Kanal mit alternativer Sprache (AltSpeak) zu sprechen. Dies funktioniert genauso wie das &lt;i&gt;Sprechen&lt;/i&gt;-Privileg, betrifft aber Audio-Daten die aufgenommen wurden, während Alt-Push-To-Talk gedrückt wurde. Es wird genutzt um Sprache in einer Hierarchie von Kanälen zu verbreiten ohne sie zu verbinden.</translation>
    </message>
    <message>
        <source>This represents the permission to mute and deafen other players. Once muted, a player will stay muted until he is unmuted by another privileged player or reconnects to the server.</source>
        <translation type="obsolete">Dies repräsentiert das Recht andere Benutzer stumm oder taub zu stellen. Einmal stummgestellt bleibt ein Benutzer stumm bis ein anderer Benutzer mit entsprechenden Privilegien dies ändert oder der Benutzer sich neu zum Server verbindet.</translation>
    </message>
    <message>
        <source>This represents the permission to move a player to another channel or kick him from the server. To actually move the player, either the moving player must have Move/Kick privileges in the destination channel, or the player must normally be allowed to enter the channel. Players with this privilege can move players into channels the target player normally wouldn&apos;t have permission to enter.</source>
        <translation type="obsolete">Dies repräsentiert das Recht einen Benutzer in einen anderen Kanal zu verschieben oder ihn vom Server zu kicken. Um einen Benutzer auch wirklich zu verschieben muss der verschiebende Benutzer entweder Verschieben/Kick-Rechte im Zielkanal haben, oder dem verschobenen Benutzer muss es erlaubt sein, den Zielkanal zu betreten. Benutzer mit diesem Privileg können also (ausreichende Rechte im Zielkanal vorausgesetzt) andere Benutzer in Kanäle verschieben, die diese normalerweise nicht betreten dürfen.</translation>
    </message>
    <message>
        <source>This represents the permission to make sub-channels. The player making the sub-channel will be added to the admin group of the sub-channel.</source>
        <translation type="obsolete">Dies repräsentiert das Recht Unterkanäle zu erstellen. Der Benutzer, der den Unterkanal erstellt, gehört automatisch zur Admingruppe des Kanals.</translation>
    </message>
    <message>
        <source>This represents the permission to link channels. Players in linked channels hear each other, as long as the speaking player has the &lt;i&gt;speak&lt;/i&gt; privilege in the channel of the listener. You need the link privilege in both channels to create a link, but just in either channel to remove it.</source>
        <translation type="obsolete">Dies repräsentiert das Recht Kanäle zu verbinden. Benutzer in verbundenen Kanälen können sich gegenseitig hören, solange der sprechende Benutzer das &lt;i&gt;Sprechen&lt;/i&gt;-Privileg im Kanal des Hörers hat. Man benötigt Verbindungsprivilegien in beiden Kanälen um eine Verbindung aufzubauen, aber nur in einem der Beiden um sie aufzuheben.</translation>
    </message>
    <message>
        <location line="-28"/>
        <source>Write ACL</source>
        <translation>Berechtigungen bearbeiten</translation>
    </message>
</context>
<context>
    <name>ChatbarLineEdit</name>
    <message>
        <source>Paste and send</source>
        <translation type="obsolete">Einfügen und senden</translation>
    </message>
    <message>
        <source>&lt;center&gt;Type chat message here&lt;/center&gt;</source>
        <translation type="obsolete">&lt;center&gt;Textnachricht hier eingeben&lt;/center&gt;</translation>
    </message>
    <message>
        <source>Type chat message here</source>
        <translation type="obsolete">Geben Sie die Chatnachricht hier ein</translation>
    </message>
</context>
<context>
    <name>ChatbarTextEdit</name>
    <message>
        <location filename="CustomElements.cpp" line="+79"/>
        <source>Paste and send</source>
        <translation>Einfügen und senden</translation>
    </message>
    <message>
        <location line="+25"/>
        <source>&lt;center&gt;Type chat message here&lt;/center&gt;</source>
        <translation>&lt;center&gt;Textnachricht hier eingeben&lt;/center&gt;</translation>
    </message>
</context>
<context>
    <name>ClientPlayer</name>
    <message>
        <source>Authenticated</source>
        <translation type="obsolete">Authentifiziert</translation>
    </message>
    <message>
        <source>Muted (server)</source>
        <translation type="obsolete">Stumm (Server)</translation>
    </message>
    <message>
        <source>Deafened (server)</source>
        <translation type="obsolete">Taub (Server)</translation>
    </message>
    <message>
        <source>Local Mute</source>
        <translation type="obsolete">Lokal Stumm</translation>
    </message>
    <message>
        <source>Muted (self)</source>
        <translation type="obsolete">Stumm (selbst)</translation>
    </message>
    <message>
        <source>Deafened (self)</source>
        <translation type="obsolete">Taub (selbst)</translation>
    </message>
</context>
<context>
    <name>ClientUser</name>
    <message>
        <location filename="ClientUser.cpp" line="+130"/>
        <source>Friend</source>
        <translation>Freund</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Authenticated</source>
        <translation>Authentifiziert</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Priority speaker</source>
        <translation>Bevorzugter Sprecher</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Recording</source>
        <translation>Aufnahme</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Muted (server)</source>
        <translation>Stumm (Server)</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Deafened (server)</source>
        <translation>Taub (Server)</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Local Mute</source>
        <translation>Lokal Stumm</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Muted (self)</source>
        <translation>Stumm (selbst)</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Deafened (self)</source>
        <translation>Taub (selbst)</translation>
    </message>
</context>
<context>
    <name>ConfigDialog</name>
    <message>
        <location filename="ConfigDialog.ui"/>
        <source>Mumble Configuration</source>
        <translation>Mumble-Konfiguration</translation>
    </message>
    <message>
        <location/>
        <location filename="ConfigDialogDelegate.mm" line="+142"/>
        <source>Advanced</source>
        <translation>Erweitert</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="+54"/>
        <source>Accept changes</source>
        <translation>Änderungen übernehmen</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This button will accept current settings and return to the application.&lt;br /&gt;The settings will be stored to disk when you leave the application.</source>
        <translation>Dieser Knopf übernimmt die aktuellen Einstellungen und kehrt zur Anwendung zurück.&lt;br /&gt;Die Einstellungen werden beim Beenden der Anwendung gespeichert.</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Abbrechen</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Reject changes</source>
        <translation>Änderungen verwerfen</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This button will reject all changes and return to the application.&lt;br /&gt;The settings will be reset to the previous positions.</source>
        <translation>Der Knopf verwirft alle Änderungen und kehrt zur Anwendung zurück.&lt;br /&gt;Die Einstellungen werden auf die vorherigen Werte zurückgesetzt.</translation>
    </message>
    <message>
        <source>&amp;Apply</source>
        <translation type="obsolete">Ü&amp;bernehmen</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Apply changes</source>
        <translation>Änderungen übernehmen</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This button will immediately apply all changes.</source>
        <translation>Dieser Button übernimmt alle Änderungen sofort.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Undo changes for current page</source>
        <translation>Änderungen der aktuellen Seite rückgängig machen</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This button will revert any changes done on the current page to the most recent applied settings.</source>
        <translation>Dieser Knopf setzt alle Änderunge auf der aktuellen Seite auf die zuletzt angewandten Einstellung zurück.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Restore defaults for current page</source>
        <translation>Standardeinstellungen für die aktuelle Seite wiederherstellen</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This button will restore the defaults for the settings on the current page. Other pages will not be changed.&lt;br /&gt;To restore all settings to their defaults, you will have to use this button on every page.</source>
        <oldsource>This button will restore the settings for the current page only to their defaults. Other pages will not be changed.&lt;br /&gt;To restore all settings to their defaults, you will have to use this button on every page.</oldsource>
        <translation>Dieser Knopf setzt die Einstellungen der aktuellen Seite auf die Standardwerte zurück. Andere Seiten werden nicht geändert.&lt;br /&gt;Um alle Einstellungen auf die Standardwerte zurückzusetzen müssen Sie diesen Knopf auf allen Seiten verwenden.</translation>
    </message>
    <message>
        <source>This button will restore the settings for the current page only to their defaults. Other pages will be not be changed.&lt;br /&gt;To restore all settings to their defaults, you will have to use this button on every page.</source>
        <translation type="obsolete">Dieser Knopf stellt die Einstellungen der aktuellen Seite auf ihre Standardwerte zurück. Andere Seiten werden nicht verändert.&lt;br /&gt;Um alle Einstellungen auf Standardwerte zu setzen, muss dieser Knopf auf allen Seiten betätigt werden.</translation>
    </message>
    <message>
        <source>Show all configuration items</source>
        <translation type="obsolete">Zeige alle Konfigurationselemente</translation>
    </message>
    <message>
        <source>&lt;b&gt;This will show all configuration items.&lt;/b&gt;&lt;br /&gt;Mumble contains a lot of configuration items that most users won&apos;t need to change. Checking this will show all configurable items.</source>
        <translation type="obsolete">&lt;b&gt;Dies zeigt alle Konfigurationselemente.&lt;/b&gt;&lt;br /&gt;Mumble enthält viele Konfigurationselemente die die meisten Benutzer nicht ändern werden. Dies zu aktivieren zeigt alle konfigurierbaren Elemente an.</translation>
    </message>
    <message>
        <source>Expert Config</source>
        <translation type="obsolete">Expertenkonfiguration</translation>
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
        <translation>Änderungen übernehmen</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This button will accept current settings and return to the application.&lt;br /&gt;The settings will be stored to disk when you leave the application.</source>
        <translatorcomment>Knopf? Wirklich? Oder eher Denglisch Button.</translatorcomment>
        <translation>Dieser Knopf übernimmt die aktuellen Einstellungen und kehrt zur Anwendung zurück.&lt;br /&gt;Die Einstellungen werden beim Beenden der Anwendung gespeichert.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Reject changes</source>
        <translation>Änderungen verwerfen</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This button will reject all changes and return to the application.&lt;br /&gt;The settings will be reset to the previous positions.</source>
        <translation>Der Knopf verwirft alle Änderungen und kehrt zur Anwendung zurück.&lt;br /&gt;Die Einstellungen werden auf die vorherigen Werte zurückgesetzt.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Apply changes</source>
        <translation>Änderungen übernehmen</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This button will immediately apply all changes.</source>
        <translation>Dieser Button übernimmt alle Änderungen sofort.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Undo changes for current page</source>
        <translatorcomment>Vielleicht wäre hier (auch im englischen) Fenster (bzw window) besser?</translatorcomment>
        <translation>Änderungen der aktuellen Seite rückgängig machen</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This button will revert any changes done on the current page to the most recent applied settings.</source>
        <translation>Dieser Knopf setzt alle Änderunge auf der aktuellen Seite auf die zuletzt angewandten Einstellung zurück.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Restore defaults for current page</source>
        <translation>Standardeinstellungen für die aktuelle Seite wiederherstellen</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This button will restore the defaults for the settings on the current page. Other pages will not be changed.&lt;br /&gt;To restore all settings to their defaults, you will have to use this button on every page.</source>
        <oldsource>This button will restore the settings for the current page only to their defaults. Other pages will not be changed.&lt;br /&gt;To restore all settings to their defaults, you will have to use this button on every page.</oldsource>
        <translation>Dieser Knopf setzt die Einstellungen der aktuellen Seite auf die Standardwerte zurück. Andere Seiten werden nicht geändert.&lt;br /&gt;Um alle Einstellungen auf ihre Standardwerte zurückzusetzen müssen Sie diesen Knopf jeweils auf allen Seiten verwenden.</translation>
    </message>
</context>
<context>
    <name>ConnectDialog</name>
    <message>
        <source>&amp;Name</source>
        <translation type="obsolete">&amp;Name</translation>
    </message>
    <message>
        <source>A&amp;ddress</source>
        <translation type="obsolete">A&amp;dresse</translation>
    </message>
    <message>
        <source>&amp;Port</source>
        <translation type="obsolete">&amp;Port</translation>
    </message>
    <message>
        <source>&amp;Username</source>
        <translation type="obsolete">Ben&amp;utzername</translation>
    </message>
    <message>
        <source>&amp;Password</source>
        <translation type="obsolete">&amp;Passwort</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="+757"/>
        <source>&amp;Connect</source>
        <translation>&amp;Verbinden</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Abbrechen</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation type="obsolete">&amp;Hinzufügen</translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation type="obsolete">&amp;Entfernen</translation>
    </message>
    <message>
        <source>&amp;Custom Servers</source>
        <translation type="obsolete">&amp;Benutzerdefinierte Server</translation>
    </message>
    <message>
        <source>Server &amp;Browser</source>
        <translation type="obsolete">Server &amp;Browser</translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="obsolete">Name</translation>
    </message>
    <message>
        <source>Address</source>
        <translation type="obsolete">Adresse</translation>
    </message>
    <message>
        <source>URL</source>
        <translation type="obsolete">URL</translation>
    </message>
    <message>
        <source>C&amp;opy to custom</source>
        <translation type="obsolete">Zu Benutzerdefinierten &amp;hinzufügen</translation>
    </message>
    <message>
        <source>&amp;View Webpage</source>
        <translation type="obsolete">&amp;Webseite besuchen</translation>
    </message>
    <message>
        <source>Favorite</source>
        <translation type="obsolete">Favorit</translation>
    </message>
    <message>
        <source>LAN</source>
        <translation type="obsolete">LAN</translation>
    </message>
    <message>
        <location line="-296"/>
        <source>Hostname</source>
        <translation>Hostname</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Bonjour name</source>
        <translation>Bonjourname</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Port</source>
        <translation>Port</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Addresses</source>
        <translation>Adressen</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Website</source>
        <translation>Webseite</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Packet loss</source>
        <translation>Paketverlust</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Ping (80%)</source>
        <translation>Ping (80%)</translation>
    </message>
    <message>
        <location line="+0"/>
        <location line="+2"/>
        <source>%1 ms</source>
        <translation>%1 ms</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Ping (95%)</source>
        <translation>Ping (95%)</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Bandwidth</source>
        <translation>Bandbreite</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>%1 kbit/s</source>
        <translation>%1 kbit/s</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui"/>
        <location filename="ConnectDialog.cpp" line="+1"/>
        <source>Users</source>
        <translation>Benutzer</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="+1"/>
        <source>Version</source>
        <translation>Version</translation>
    </message>
    <message>
        <source>Connect</source>
        <translation type="obsolete">Verbinden</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui"/>
        <location filename="ConnectDialog.cpp" line="+276"/>
        <source>&amp;Add New...</source>
        <oldsource>Add New...</oldsource>
        <translation>&amp;Server hinzufügen...</translation>
    </message>
    <message>
        <source>Filters</source>
        <translation type="obsolete">Filter</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="+131"/>
        <source>Connecting to %1</source>
        <translation>Verbinde zu %1</translation>
    </message>
    <message>
        <location line="-491"/>
        <location line="+491"/>
        <source>Enter username</source>
        <translation>Benutzername eingeben</translation>
    </message>
    <message>
        <location line="-491"/>
        <source>Adding host %1</source>
        <translation>Host %1 hinzufügen</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui"/>
        <location filename="ConnectDialog.cpp" line="+61"/>
        <source>Servername</source>
        <translation>Servername</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="+336"/>
        <source>&amp;Filters</source>
        <translation>&amp;Filter</translation>
    </message>
    <message>
        <source>Mumble</source>
        <translation type="obsolete">Mumble</translation>
    </message>
    <message>
        <location line="+692"/>
        <source>Failed to fetch server list</source>
        <translation>Serverliste konnte nicht geladen werden</translation>
    </message>
    <message>
        <source>&amp;Label</source>
        <translation type="obsolete">&amp;Bezeichnung</translation>
    </message>
    <message>
        <source>Label</source>
        <translation type="obsolete">Bezeichnung</translation>
    </message>
    <message>
        <source>-Unnamed entry-</source>
        <translation type="obsolete">-Unbenannter Eintrag-</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui"/>
        <source>Mumble Server Connect</source>
        <translation>Mumble - Server Verbinden</translation>
    </message>
    <message>
        <source>Location</source>
        <translation type="obsolete">Standort</translation>
    </message>
    <message>
        <location/>
        <source>Ping</source>
        <translation>Ping</translation>
    </message>
    <message>
        <location/>
        <source>Remove from Favorites</source>
        <translation>Aus den Favoriten entfernen</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Edit...</source>
        <oldsource>Edit...</oldsource>
        <translation>&amp;Bearbeiten...</translation>
    </message>
    <message>
        <location/>
        <source>Add custom server</source>
        <translation>Benutzerdefinierten Server hinzufügen</translation>
    </message>
    <message>
        <location/>
        <source>Add to &amp;Favorites</source>
        <oldsource>Add to Favorites</oldsource>
        <translation>Zu &amp;Favoriten hinzufügen</translation>
    </message>
    <message>
        <location/>
        <source>Open &amp;Webpage</source>
        <oldsource>Open Webpage</oldsource>
        <translation>&amp;Webseite öffnen</translation>
    </message>
    <message>
        <location/>
        <source>Show &amp;Reachable</source>
        <oldsource>Show Reachable</oldsource>
        <translation>Nur &amp;erreichbare Server anzeigen</translation>
    </message>
    <message>
        <location/>
        <source>Show all servers that respond to ping</source>
        <translation>Alle Server anzeigen die auf Pings antworten</translation>
    </message>
    <message>
        <location/>
        <source>Show &amp;Populated</source>
        <oldsource>Show Populated</oldsource>
        <translation>Server mit &amp;Benutzern anzeigen</translation>
    </message>
    <message>
        <location/>
        <source>Show all servers with users</source>
        <translation>Zeige alle Server auf denen sich Benutzern befinden</translation>
    </message>
    <message>
        <location/>
        <source>Show &amp;All</source>
        <oldsource>Show All</oldsource>
        <translation>&amp;Alle anzeigen</translation>
    </message>
    <message>
        <location/>
        <source>Show all servers</source>
        <translation>Alle Server anzeigen</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Copy</source>
        <translation>&amp;Kopieren</translation>
    </message>
    <message>
        <location/>
        <source>Copy favorite link to clipboard</source>
        <translation>Kopiere Favoriten-Link in die Zwischenablage</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Paste</source>
        <translation>&amp;Einfügen</translation>
    </message>
    <message>
        <location/>
        <source>Paste favorite from clipboard</source>
        <translation>Füge Favorit aus der Zwischenablage ein</translation>
    </message>
    <message>
        <source>Show only Favorites</source>
        <translation type="obsolete">Zeige nur Favoriten</translation>
    </message>
    <message>
        <source>Show only Favorite and LAN servers</source>
        <translation type="obsolete">Zeige nur Favoriten und LAN Server</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Abbrechen</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation type="obsolete">Unbekannt</translation>
    </message>
    <message>
        <source>New</source>
        <translation type="obsolete">Neu</translation>
    </message>
    <message>
        <source>Add</source>
        <translation type="obsolete">Hinzufügen</translation>
    </message>
    <message>
        <source>Update</source>
        <translation type="obsolete">Aktualisieren</translation>
    </message>
</context>
<context>
    <name>ConnectDialogEdit</name>
    <message>
        <location filename="ConnectDialogEdit.ui"/>
        <source>Edit Server</source>
        <translation>Server bearbeiten</translation>
    </message>
    <message>
        <location/>
        <source>Name of the server</source>
        <translation>Name des Servers</translation>
    </message>
    <message>
        <source>&lt;b&gt;Name&lt;/b&gt;&lt;br/&gt;
Name of the server. This is what the server will be named like in your serverlist and can be chosen freely.</source>
        <translation type="obsolete">&lt;b&gt;Name&lt;/b&gt;&lt;br/&gt;
Name des Servers. Dies ist ein frei wählbarer Name der in der Serverliste anzeigt wird.</translation>
    </message>
    <message>
        <location/>
        <source>A&amp;ddress</source>
        <translation>A&amp;dresse</translation>
    </message>
    <message>
        <location/>
        <source>Internet address of the server.</source>
        <oldsource>Internet address of the server. </oldsource>
        <translation>Internet-Adresse des Servers.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Address&lt;/b&gt;&lt;br/&gt;
Internet address of the server. This can be a normal hostname, an IPv4/IPv6 address or a Bonjour service identifier. Bonjour service identifiers have to be prefixed with a &apos;@&apos; to be recognized by Mumble.</source>
        <oldsource>&lt;b&gt;Address&lt;/b&gt;&lt;/br&gt;
Internet address of the server. This can be a normal hostname, an ipv4/6 address or a bonjour service identifier. Bonjour service identifiers have to be prefixed with a &apos;@&apos; to be recognized by Mumble.</oldsource>
        <translatorcomment>Bonjour Service Idenfitier</translatorcomment>
        <translation>&lt;b&gt;Adresse&lt;/b&gt;&lt;br /&gt;
Internet-Adresse des Servers. Dies kann ein normaler Hostname, eine IPv4/IPv6 Adresse oder eine Bonjour-Service-Identifikation sein. Bonjour-Service-Identifikationen müssen mit einem &apos;@&apos; beginnen um von Mumble erkannt zu werden.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Port</source>
        <translation>&amp;Port</translation>
    </message>
    <message>
        <location/>
        <source>Port on which the server is listening</source>
        <translation>Port, auf welchem der Server hört</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Port&lt;/b&gt;&lt;br/&gt;
Port on which the server is listening. If the server is identified by a Bonjour service identifier this field will be ignored.</source>
        <translation>&lt;b&gt;Port&lt;/b&gt;&lt;br /&gt;
Port auf welchem der Server hört. Wenn eine Bonjour-Service-Identifikation verwendet wird, wird dieses Feld ignoriert.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Username</source>
        <translation>Ben&amp;utzername</translation>
    </message>
    <message>
        <location/>
        <source>Username to send to the server</source>
        <translation>Benutzername, welcher an den Server gesendet wird</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Username&lt;/b&gt;&lt;br/&gt;
Username to send to the server. Be aware that the server can impose restrictions on how a username might look like. Also your username could already be taken by another user.</source>
        <translation>&lt;b&gt;Benutzername&lt;/b&gt;&lt;br /&gt;
Benutzername, welcher an den Server gesendet wird. Beachten Sie, dass jeder Serveradminstrator eigenen Einschränkungen für Benutzernamen festlegen kann. Ihr Benutzername kann außerdem bereits von einem anderen Benutzer belegt sein.</translation>
    </message>
    <message>
        <location/>
        <source>Label</source>
        <translation>Bezeichnung</translation>
    </message>
    <message>
        <source>&lt;b&gt;Label&lt;/b&gt;&lt;br/&gt;
Label of the server. This is what the server will be named like in your serverlist and can be chosen freely.</source>
        <translation type="obsolete">&lt;b&gt;Bezeichnung&lt;/b&gt;&lt;br/&gt;
Dies ist die Bezeichnung des Servers, welche in der Serverliste erscheint und frei gewählt werden kann.</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="-804"/>
        <source>Add Server</source>
        <translation>Server hinzufügen</translation>
    </message>
    <message>
        <location filename="ConnectDialogEdit.ui"/>
        <source>Password</source>
        <translation>Passwort</translation>
    </message>
    <message>
        <location/>
        <source>Password to send to the server</source>
        <translation>Das Passwort, welches an den Server gesendet wird</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Password&lt;/b&gt;&lt;br/&gt;
Password to be sent to the server on connect. This password is needed when connecting as &lt;i&gt;SuperUser&lt;/i&gt; or to a server using password authentication. If not entered here the password will be queried on connect.</source>
        <translation>&lt;b&gt;Passwort&lt;/b&gt;&lt;br /&gt;
Passwort, das an den Server gesendet wird. Dieses Passwort wird benötigt wenn man sich als &lt;i&gt;SuperUser&lt;/i&gt; oder zu einem Server mit Passwort-Authentifizierung verbindet. Falls nicht angegeben so wird das Passwort beim Verbinden abgefragt.</translation>
    </message>
    <message>
        <location/>
        <source>Show password</source>
        <translation>Passwort anzeigen</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Label&lt;/b&gt;&lt;br/&gt;
Label of the server. This is what the server will be named like in your server list and can be chosen freely.</source>
        <translation>&lt;b&gt;Bezeichnung&lt;/b&gt;&lt;br /&gt;
Dies ist die Bezeichnung des Servers wie sie in den Favoriten erscheint und kann frei gewählt werden.</translation>
    </message>
</context>
<context>
    <name>CoreAudioSystem</name>
    <message>
        <location filename="CoreAudio.cpp" line="+83"/>
        <source>Default Device</source>
        <translation>Standardgerät</translation>
    </message>
</context>
<context>
    <name>CrashReporter</name>
    <message>
        <location filename="CrashReporter.cpp" line="+37"/>
        <source>Mumble Crash Report</source>
        <translation>Mumble - Absturzbericht</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>&lt;p&gt;&lt;b&gt;We&apos;re terribly sorry, but it seems Mumble has crashed. Do you want to send a crash report to the Mumble developers?&lt;/b&gt;&lt;/p&gt;&lt;p&gt;The crash report contains a partial copy of Mumble&apos;s memory at the time it crashed, and will help the developers fix the problem.&lt;/p&gt;</source>
        <translation>&lt;p&gt;&lt;b&gt;Es tut uns sehr leid, aber es scheint so, als sei Mumble abgestürzt. Möchten Sie einen Absturz-Bericht an die Mumble-Entwickler senden?&lt;/b&gt;&lt;/p&gt;
&lt;p&gt;
Der Absturz-Bericht enthält eine Teilkopie von Mumbles Speicher zum Zeitpunkt des Absturzes und wird den Entwicklern helfen, das Problem zu beheben.
&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Email address (optional)</source>
        <translation>E-Mail Adresse (optional)</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Please describe briefly, in English, what you were doing at the time of the crash</source>
        <oldsource>Please briefly describe what you were doing at the time of the crash</oldsource>
        <translation>Bitte beschreiben Sie kurz, in Englischer Sprache, was Sie getan haben als Mumble abgestürzt ist</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Send Report</source>
        <translation>Bericht senden</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Don&apos;t send report</source>
        <translation>Bericht nicht senden</translation>
    </message>
    <message>
        <location line="+24"/>
        <source>Crash upload successful</source>
        <oldsource>Crash upload successfull</oldsource>
        <translation>Hochladen des Absturz-Berichts erfolgreich</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Thank you for helping make Mumble better!</source>
        <translation>Danke dass Sie dabei helfen, Mumble zu verbessern!</translation>
    </message>
    <message>
        <location line="+2"/>
        <location line="+2"/>
        <source>Crash upload failed</source>
        <translation>Hochladen des Berichts fehlgeschlagen</translation>
    </message>
    <message>
        <location line="-2"/>
        <source>We&apos;re really sorry, but it appears the crash upload has failed with error %1 %2. Please inform a developer.</source>
        <translation>Das Hochladen des Berichts scheint mit der Fehlermeldung %1 %2 fehlgeschlagen zu sein. Bitte informieren Sie einen Entwickler.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>This really isn&apos;t funny, but apparently there&apos;s a bug in the crash reporting code, and we&apos;ve failed to upload the report. You may inform a developer about error %1</source>
        <translation>Das ist wirklich nicht witzig, aber es scheint einen Bug im Absturz-Bericht-Melden-Code zu geben und das Hochladen des Berichts schlug fehl. Sie können einen Entwickler über den Fehler %1 informieren</translation>
    </message>
    <message>
        <location line="+114"/>
        <source>Uploading crash report</source>
        <translation>Absturzbericht wird hochladen</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Abort upload</source>
        <translation>Upload abbrechen</translation>
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
        <translation type="obsolete">Öffnen des DirectSound Eingangsgerätes gescheitert. Es wird kein Mikrofonsound aufgenommen.</translation>
    </message>
    <message>
        <source>Opening chosen DirectSound Input failed. Using defaults.</source>
        <translation type="obsolete">Öffnen des DirectSound Eingangsgerätes gescheitert. Standardeinstellungen werden benutzt.</translation>
    </message>
    <message>
        <location filename="DirectSound.cpp" line="+554"/>
        <source>Opening chosen DirectSound Input failed. Default device will be used.</source>
        <translation>Öffnen des DirectSound Eingangsgerätes fehlgeschlagen. Standardgerät wird benutzt.</translation>
    </message>
    <message>
        <location line="-382"/>
        <source>Default DirectSound Voice Input</source>
        <translation>Standard DirectSound Spracheingabe</translation>
    </message>
    <message>
        <location line="+461"/>
        <source>Opening chosen DirectSound Input device failed. No microphone capture will be done.</source>
        <translation>Öffnen des DirectSound Eingangsgerätes fehlgeschlagen. Es wird kein Mikrofonsound aufgenommen.</translation>
    </message>
    <message>
        <location line="-5"/>
        <source>Lost DirectSound input device.</source>
        <translation>DirectSound Eingangsgerät verloren.</translation>
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
        <translation>Öffnen des DirectSound Ausgabegerätes fehlgeschlagen. Es kann kein Ton gehört werden.</translation>
    </message>
    <message>
        <source>Opening chosen DirectSound Output failed. Using defaults.</source>
        <translation type="obsolete">Öffnen des DirectSound Ausgabegerätes gescheitert. Standardeinstellungen werden benutzt.</translation>
    </message>
    <message>
        <location line="-82"/>
        <source>Opening chosen DirectSound Output failed. Default device will be used.</source>
        <translation>Öffnen des DirectSound Ausgabegerätes fehlgeschlagen. Standardgerät wird benutzt.</translation>
    </message>
    <message>
        <location line="-247"/>
        <source>Default DirectSound Voice Output</source>
        <translation>Standard DirectSound Sprachausgabe</translation>
    </message>
    <message>
        <location line="+325"/>
        <source>Lost DirectSound output device.</source>
        <translation>DirectSound Ausgabegerät verloren.</translation>
    </message>
</context>
<context>
    <name>DXConfigDialog</name>
    <message>
        <source>Default DirectSound Voice Input</source>
        <translation type="obsolete">Standard DirectSound Spracheingabe</translation>
    </message>
    <message>
        <source>Default DirectSound Voice Output</source>
        <translation type="obsolete">Standard DirectSound Sprachausgabe</translation>
    </message>
    <message>
        <source>Device selection</source>
        <translation type="obsolete">Geräteauswahl</translation>
    </message>
    <message>
        <source>Device to use for microphone</source>
        <translation type="obsolete">Mikrofongerät</translation>
    </message>
    <message>
        <source>This sets the input device to use, which is where you have connected the microphone.</source>
        <translation type="obsolete">Dies legt das Eingabegerät fest, welches mit dem Mikrofon verbunden ist.</translation>
    </message>
    <message>
        <source>Input</source>
        <translation type="obsolete">Eingang</translation>
    </message>
    <message>
        <source>Device to use for speakers/headphones</source>
        <translation type="obsolete">Gerät für Lautsprecher/Kopfhörer</translation>
    </message>
    <message>
        <source>This sets the output device to use, which is where you have connected your speakers or your headset.</source>
        <translation type="obsolete">Dies legt das Ausgabegerät fest, welches mit Lautsprechern oder Headset verbunden ist.</translation>
    </message>
    <message>
        <source>Output</source>
        <translation type="obsolete">Ausgabe</translation>
    </message>
    <message>
        <source>Output Options</source>
        <translation type="obsolete">Ausgabeoptionen</translation>
    </message>
    <message>
        <source>Output Delay</source>
        <translation type="obsolete">Ausgabeverzögerung</translation>
    </message>
    <message>
        <source>Amount of data to buffer for DirectSound</source>
        <translation type="obsolete">Menge der zu puffernden Daten für DirectSound</translation>
    </message>
    <message>
        <source>This sets the amount of data to prebuffer in the DirectSound buffer. Experiment with different values and set it to the lowest which doesn&apos;t cause rapid jitter in the sound.</source>
        <translation type="obsolete">Dies legt die Menge an Daten fest, die für den DirectSound-Puffer vorgepuffert werden sollen. Es sollte der niedrigste Wert gewählt werden, der noch kein Knacken im Sound produziert.</translation>
    </message>
    <message>
        <source>Positional Audio</source>
        <translation type="obsolete">Positionales Audio</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Keiner</translation>
    </message>
    <message>
        <source>Panning</source>
        <translation type="obsolete">Panning</translation>
    </message>
    <message>
        <source>Light HRTF</source>
        <translation type="obsolete">Leichtes HRTF</translation>
    </message>
    <message>
        <source>Full HRTF</source>
        <translation type="obsolete">Volles HRTF</translation>
    </message>
    <message>
        <source>3D Sound Algorithm</source>
        <translation type="obsolete">3D Soundalgorithmus</translation>
    </message>
    <message>
        <source>This sets what 3D Sound algorithm to use.&lt;br /&gt;&lt;b&gt;None&lt;/b&gt; - Disable 3D Sound (least CPU).&lt;br /&gt;&lt;b&gt;Panning&lt;/b&gt; - Just use stereo panning (some CPU).&lt;br /&gt;&lt;b&gt;Light/Full HRTF&lt;/b&gt; - Head-Related Transfer Functions enabled. This may use a small amount of CPU.&lt;br /&gt;Note that if you have a soundcard with &lt;i&gt;hardware&lt;/i&gt; 3D processing, HRTF processing will be done on the soundcard and will use practically no processing power.</source>
        <translation type="obsolete">Dies legt den 3D-Soundalgorithmus fest.&lt;br /&gt;&lt;b&gt;Keiner&lt;/b&gt; - 3D-Sound deaktivieren.&lt;br /&gt;&lt;b&gt;Panning&lt;/b&gt; - Nur Stereo-Panorama (etwas CPU).&lt;br /&gt;&lt;b&gt;Leichtes/Volles HRTF&lt;/b&gt; - HRTF Funktionen eingeschalten. Dies verwendet etwas CPU-Zeit.&lt;br /&gt;Falls die Soundkarte &lt;i&gt;Hardware&lt;/i&gt;-3D-Processing unterstützt, wird HRTF praktisch keine CPU-Zeit beanspruchen.</translation>
    </message>
    <message>
        <source>Method</source>
        <translation type="obsolete">Methode</translation>
    </message>
    <message>
        <source>MinDistance</source>
        <translation type="obsolete">MinDistanz</translation>
    </message>
    <message>
        <source>Minimum distance to player before sound decreases</source>
        <translation type="obsolete">Minimale Distanz zum Spieler bevor Lautstärke sinkt</translation>
    </message>
    <message>
        <source>This sets the minimum distance for sound calculations. The volume of other players&apos; speech will not decrease until they are at least this far away from you.</source>
        <translation type="obsolete">Dies legt die minimale Distanz für Klangberechnungnen fest. Die Lautstärke anderer Spieler wird nicht sinken, solange sie nicht wenigstens soweit wie eingestellt entfernt sind.</translation>
    </message>
    <message>
        <source>MaxDistance</source>
        <translation type="obsolete">MaxDistanz</translation>
    </message>
    <message>
        <source>Maximum distance, beyond which sound won&apos;t decrease</source>
        <translation type="obsolete">Maximale Distanz worüber hinaus die Lautstärke nicht sinkt</translation>
    </message>
    <message>
        <source>This sets the maximum distance for sound calculations. When farther away than this, other players&apos; sound volume will not decrease any more.</source>
        <translation type="obsolete">Dies legt die maximale Distanz für Klangberechnungnen fest. Die Lautstärke anderer Spieler wird nicht mehr sinken, wenn sie weiter als eingestellt entfernt sind.</translation>
    </message>
    <message>
        <source>RollOff</source>
        <translation type="obsolete">Rolloff</translation>
    </message>
    <message>
        <source>Factor for sound volume decrease</source>
        <translation type="obsolete">Faktor zur Lautstärkensenkung</translation>
    </message>
    <message>
        <source>How fast should sound volume drop when passing beyond the minimum distance. The normal (1.0) is that sound volume halves each time the distance doubles. Increasing this value means sound volume drops faster, while decreasing it means it drops slower.</source>
        <translation type="obsolete">Wie schnell soll die Lautstärke sinken, wenn die Minimaldistanz überschritten ist. Mit der Standardeinstellung (1.0) wird die Lautstärke bei doppelter Entfernung halbiert. Den Wert erhöhen lässt die Lautstärke schneller fallen, während ihn zu senken die Lautstärker langsamer fallen lässt.</translation>
    </message>
    <message>
        <source>DirectSound</source>
        <translation type="obsolete">DirectSound</translation>
    </message>
    <message>
        <source>%1 ms</source>
        <translation type="obsolete">%1 ms</translation>
    </message>
    <message>
        <source>%1m</source>
        <translation type="obsolete">%1m</translation>
    </message>
    <message>
        <source>%1</source>
        <translation type="obsolete">%1</translation>
    </message>
    <message>
        <source>Players more than %1 meters away have %2% intensity</source>
        <translation type="obsolete">Spieler mehr als %1 Meter entfernt haben %2% Intensität</translation>
    </message>
    <message>
        <source>%1ms</source>
        <translation type="obsolete">%1ms</translation>
    </message>
    <message>
        <source>Form</source>
        <translation type="obsolete">Formular</translation>
    </message>
    <message>
        <source>TextLabel</source>
        <translation type="obsolete">Textlabel</translation>
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
        <translation>Mumble konnte keine Datenbank an einem
der möglichen Orte initialisieren.</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>The database &apos;%1&apos; is read-only. Mumble cannot store server settings (i.e. SSL certificates) until you fix this problem.</source>
        <oldsource>The database &apos;%1&apos; is read-only. Mumble can not store server settings (ie. SSL certificates) until you fix this problem.</oldsource>
        <translation>Auf die Datenbank &apos;%1&apos; kann nicht schreibend zugegriffen werden. Mumble kann keine Servereinstellungen (z.B. SSL-Zertifikate) speichern.</translation>
    </message>
</context>
<context>
    <name>DirectInputKeyWidget</name>
    <message>
        <source>Press Shortcut</source>
        <translation type="obsolete">Shortcut drücken</translation>
    </message>
</context>
<context>
    <name>DockTitleBar</name>
    <message>
        <location filename="CustomElements.cpp" line="+135"/>
        <source>Drag here</source>
        <translation>Hier ziehen</translation>
    </message>
</context>
<context>
    <name>GlobalShortcut</name>
    <message>
        <location filename="GlobalShortcut.ui"/>
        <source>Shortcuts</source>
        <translation>Tastenkürzel</translation>
    </message>
    <message>
        <location/>
        <source>List of configured shortcuts</source>
        <translation>Liste der konfigurierten Tastenkürzel</translation>
    </message>
    <message>
        <location/>
        <source>Function</source>
        <translation>Funktion</translation>
    </message>
    <message>
        <location/>
        <source>Data</source>
        <translation>Daten</translation>
    </message>
    <message>
        <location/>
        <source>Shortcut</source>
        <translation>Tastenkürzel</translation>
    </message>
    <message>
        <location/>
        <source>Suppress</source>
        <translation>Unterdrücken</translation>
    </message>
    <message>
        <location/>
        <source>Add new shortcut</source>
        <translation>Neues Tastenkürzel hinzufügen</translation>
    </message>
    <message>
        <location/>
        <source>This will add a new global shortcut</source>
        <translation>Dies fügt ein globales Tastenkürzel hinzu</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Add</source>
        <translation>&amp;Hinzufügen</translation>
    </message>
    <message>
        <location/>
        <source>Remove selected shortcut</source>
        <translation>Gewähltes Tastenkürzel entfernen</translation>
    </message>
    <message>
        <location/>
        <source>This will permanently remove a selected shortcut.</source>
        <translation>Dies wird das gewählte Tastenkürzel permanent löschen.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Remove</source>
        <translation>&amp;Entfernen</translation>
    </message>
</context>
<context>
    <name>GlobalShortcutConfig</name>
    <message>
        <location filename="GlobalShortcut.cpp" line="+654"/>
        <source>Shortcuts</source>
        <translation>Tastenkürzel</translation>
    </message>
    <message>
        <source>Function</source>
        <translation type="obsolete">Funktion</translation>
    </message>
    <message>
        <source>Shortcut</source>
        <translation type="obsolete">Kurzbefehl</translation>
    </message>
    <message>
        <source>Shortcut bound to %1.</source>
        <translation type="obsolete">Kurzbefehl auf %1 gebunden.</translation>
    </message>
    <message>
        <source>&lt;b&gt;This is the global shortcut bound to %1&lt;/b&gt;&lt;br /&gt;Click this field and then the desired key/button combo to rebind. Double-click to clear.</source>
        <translation type="obsolete">&lt;b&gt;Dies ist der globale Kurzbefehl, der an %1 gebunden ist&lt;/b&gt;&lt;br /&gt;Klicken Sie dieses Feld an und drücken Sie dann die gewünschte Tastenkombination um sie neu zu binden. Doppelklicken zum löschen.</translation>
    </message>
    <message>
        <source>Double-click an entry to clear the shortcut.</source>
        <translation type="obsolete">Auf einen Eintrag doppelklicken, um den Kurzbefehl zu entfernen.</translation>
    </message>
    <message>
        <source>Suppress</source>
        <translation type="obsolete">Unterdrücken</translation>
    </message>
    <message>
        <location line="+38"/>
        <source>Shortcut button combination.</source>
        <translation>Tastenkombination des Tastenkürzels.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>&lt;b&gt;This is the global shortcut key combination.&lt;/b&gt;&lt;br /&gt;Click this field and then press the desired key/button combo to rebind. Double-click to clear.</source>
        <oldsource>&lt;b&gt;This is the global shortcut key combination.&lt;/b&gt;&lt;br /&gt;Click this field and then the desired key/button combo to rebind. Double-click to clear.</oldsource>
        <translation>&lt;b&gt;Dies ist die globale Tastenkombination des Tastenkürzels.&lt;/b&gt;&lt;br /&gt;Doppelklicken Sie dieses Feld und dann die gewünschte Tastenkombination um sie neu zu setzen.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Suppress keys from other applications</source>
        <translation>Unterdrücke Tasten vor anderen Anwendungen</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>&lt;b&gt;This hides the button presses from other applications.&lt;/b&gt;&lt;br /&gt;Enabling this will hide the button (or the last button of a multi-button combo) from other applications. Note that not all buttons can be suppressed.</source>
        <translation>&lt;b&gt;Dies unterdrückt Tastendrücke von anderen Anwendungen.&lt;/b&gt;&lt;br /&gt;Durch Aktivierung wird der Tastendruck (oder die letzte Taste einer Mehrfachtasten-Kombination) vor anderen Anwendungen versteckt. Es können aber nicht alle Tasten unterdrückt werden.</translation>
    </message>
</context>
<context>
    <name>GlobalShortcutMacInit</name>
    <message>
        <location filename="GlobalShortcut_macx.mm" line="+65"/>
        <source>Mumble has detected that it is unable to receive Global Shortcut events when it is in the background.&lt;br /&gt;&lt;br /&gt;This is because the Universal Access feature called &apos;Enable access for assistive devices&apos; is currently disabled.&lt;br /&gt;&lt;br /&gt;Please &lt;a href=&quot; &quot;&gt;enable this setting&lt;/a&gt; and continue when done.</source>
        <translation>Mumble hat festgestellt, dass es keine globalen Tastenkürzel Ereignisse erhalten kann, wenn es im Hintergrund läuft.&lt;br /&gt;&lt;br /&gt;Dies wird durch die Deaktivierung des Universal Access Features &quot;Enable access for assitive devices&quot; verursacht.&lt;br /&gt;&lt;br /&gt;Bitte &lt;a href=&quot;&quot;&gt;aktivieren sie diese Einstellung&lt;/a&gt;.</translation>
    </message>
</context>
<context>
    <name>GlobalShortcutTarget</name>
    <message>
        <location filename="GlobalShortcutTarget.ui"/>
        <source>Whisper Target</source>
        <translation>Flüster-Ziel</translation>
    </message>
    <message>
        <location/>
        <source>Whisper to list of Users</source>
        <translation>An Liste von Benutzern flüstern</translation>
    </message>
    <message>
        <location/>
        <source>Channel Target</source>
        <translation>Zielkanal</translation>
    </message>
    <message>
        <location/>
        <source>Restrict to Group</source>
        <translation>Beschränke auf Gruppe</translation>
    </message>
    <message>
        <location/>
        <source>If specified, only members of this group will receive the whisper.</source>
        <translation>Wenn festgelegt empfangen nur Mitglieder dieser Gruppe das Flüstern.</translation>
    </message>
    <message>
        <source>If checked the whisper will also be transmitted to linked channels.</source>
        <translation type="obsolete">Wenn gewählt wird das Flüstern auch an verknüpfte Kanäle gesandt.</translation>
    </message>
    <message>
        <source>Whisper to Linked channels</source>
        <translation type="obsolete">An verbundene Kanäle flüstern</translation>
    </message>
    <message>
        <source>If checked this whisper will also be sent to the subchannels of the channel target.</source>
        <translation type="obsolete">Wenn gewählt wird das Flüstern auch an Unterkanäle des Zielkanals gesandt.</translation>
    </message>
    <message>
        <location/>
        <source>List of users</source>
        <translation>Benutzerliste</translation>
    </message>
    <message>
        <location/>
        <source>Add</source>
        <translation>Hinzufügen</translation>
    </message>
    <message>
        <location/>
        <source>Remove</source>
        <translation>Entfernen</translation>
    </message>
    <message>
        <location/>
        <source>Modifiers</source>
        <translation>Optionen</translation>
    </message>
    <message>
        <location/>
        <source>Do not send positional audio information when using this whisper shortcut.</source>
        <translation>Sende keine Positionsinformationen beim Flüstern.</translation>
    </message>
    <message>
        <location/>
        <source>Ignore positional audio</source>
        <translation>Ignoriere positionsabhängiges Audio</translation>
    </message>
    <message>
        <location/>
        <source>Shout to Linked channels</source>
        <translation>An verknüpfte Kanäle rufen</translation>
    </message>
    <message>
        <location/>
        <source>Shout to subchannels</source>
        <translation>An Unterkanäle rufen</translation>
    </message>
    <message>
        <location/>
        <source>Shout to Channel</source>
        <translation>An den Kanal rufen</translation>
    </message>
    <message>
        <location/>
        <source>The whisper will also be transmitted to linked channels.</source>
        <translation>Das Flüstern wird auch an verknüpfte Kanäle gesendet.</translation>
    </message>
    <message>
        <location/>
        <source>The whisper will also be sent to the subchannels of the channel target.</source>
        <translation>Das Flüstern wird auch an die Unterkanäle des Zielkanals gesendet.</translation>
    </message>
</context>
<context>
    <name>GlobalShortcutWinConfig</name>
    <message>
        <source>Shortcuts</source>
        <translation type="obsolete">Shortcuts</translation>
    </message>
    <message>
        <source>Function</source>
        <translation type="obsolete">Funktion</translation>
    </message>
    <message>
        <source>Shortcut</source>
        <translation type="obsolete">Shortcut</translation>
    </message>
    <message>
        <source>Shortcut bound to %1.</source>
        <translation type="obsolete">Shortcut an %1 gebunden.</translation>
    </message>
    <message>
        <source>&lt;b&gt;This is the global shortcut bound to %1&lt;/b&gt;&lt;br /&gt;Click this field and then the desired key/button combo to rebind. Double-click to clear.</source>
        <translation type="obsolete">&lt;b&gt;Der globale Shortcut ist an %1 gebunden&lt;/b&gt;&lt;br /&gt;Dieses Feld anklicken und dann die gewünschte Taste/Button drücken zum neubinden. Doppelklicken zum löschen.</translation>
    </message>
    <message>
        <source>Double-click an entry to clear the shortcut.</source>
        <translation type="obsolete">Auf einen Eintrag doppelklicken, um den Shortcut zu entfernen.</translation>
    </message>
</context>
<context>
    <name>GlobalShortcutX</name>
    <message>
        <location filename="GlobalShortcut_unix.cpp" line="+325"/>
        <source>Mouse %1</source>
        <translation>Maus %1</translation>
    </message>
</context>
<context>
    <name>GlobalShortcutXConfig</name>
    <message>
        <source>Shortcuts</source>
        <translation type="obsolete">Shortcuts</translation>
    </message>
    <message>
        <source>Function</source>
        <translation type="obsolete">Funktion</translation>
    </message>
    <message>
        <source>Shortcut</source>
        <translation type="obsolete">Shortcut</translation>
    </message>
    <message>
        <source>Shortcut bound to %1.</source>
        <translation type="obsolete">Shortcut an %1 gebunden.</translation>
    </message>
    <message>
        <source>&lt;b&gt;This is the global shortcut bound to %1&lt;/b&gt;&lt;br /&gt;Click this field and then the desired key combo to rebind. Double-click to clear.</source>
        <translation type="obsolete">&lt;b&gt;Der globale Shortcut ist an %1 gebunden&lt;/b&gt;&lt;br /&gt;Dieses Feld anklicken und dann die gewünschte Taste/Button drücken zum neubinden. Doppelklicken zum löschen.</translation>
    </message>
</context>
<context>
    <name>LCD</name>
    <message>
        <location filename="LCD.cpp" line="+279"/>
        <source>Not connected</source>
        <translation>Nicht verbunden</translation>
    </message>
</context>
<context>
    <name>LCDConfig</name>
    <message>
        <source>Graphic</source>
        <translation type="obsolete">Grafik</translation>
    </message>
    <message>
        <source>Character</source>
        <translation type="obsolete">Zeichen</translation>
    </message>
    <message>
        <location line="-160"/>
        <source>Enable this device</source>
        <translation>Dieses Gerät aktivieren</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>LCD</source>
        <translation>LCD</translation>
    </message>
    <message>
        <location filename="LCD.ui"/>
        <source>Form</source>
        <translation>Formular</translation>
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
        <translation type="obsolete">&lt;!DOCTYPE HTML PUBLIC &quot;-//W3C//DTD HTML 4.0//EN&quot; &quot;http://www.w3.org/TR/REC-html40/strict.dtd&quot;&gt;
&lt;html&gt;&lt;head&gt;&lt;meta name=&quot;qrichtext&quot; content=&quot;1&quot; /&gt;&lt;style type=&quot;text/css&quot;&gt;
p, li { white-space: pre-wrap; }
&lt;/style&gt;&lt;/head&gt;&lt;body style=&quot; font-family:&apos;MS Shell Dlg 2&apos;; font-size:8.25pt; font-weight:400; font-style:normal;&quot;&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;Dies ist die Liste der verfügbaren LCD-Geräte auf ihrem System. Es listet die Geräte nach Namen auf und enthält außerdem die Größe des Displays und den LCD-Typ. Mumble unterstützt die zeitgleiche Ausgabe auf mehrere LCD-Geräte.&lt;/p&gt;
&lt;p style=&quot;-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;&lt;span style=&quot; font-weight:600; font-style:italic;&quot;&gt;Typ:&lt;/span&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;Mumble unterstützt zwei verschiedene LCD-Geräte Typen. Ein Grafik-LCD erlaubt Mumble einzelne Pixel zu zeichen, während ein Zeichen-LCD nur das Anzeigen von Zeichen erlaubt. Einige Features sind nur auf einem Grafik-LCD verfügbar.&lt;/p&gt;
&lt;p style=&quot;-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;&lt;span style=&quot; font-weight:600; font-style:italic;&quot;&gt;Größe:&lt;/span&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;Dieses Feld beschreibt die Größe eines LCD-Geräts. Die Größe ist entweder in Pixeln (für Grafik-LCDs) oder in Zeichen (für Zeichen-LCDs) angegeben.&lt;/p&gt;
&lt;p style=&quot;-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;&lt;span style=&quot; font-weight:600; font-style:italic;&quot;&gt;Aktiviert:&lt;/span&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt; font-weight:600; font-style:italic;&quot;&gt;&lt;span style=&quot; font-weight:400; font-style:normal;&quot;&gt;Dies entscheidet ob Mumble auf ein bestimmtes LCD zeichnen soll.&lt;/span&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Devices</source>
        <translation>Geräte</translation>
    </message>
    <message>
        <location/>
        <source>Name</source>
        <translation>Name</translation>
    </message>
    <message>
        <source>Type</source>
        <translation type="obsolete">Typ</translation>
    </message>
    <message>
        <location/>
        <source>&lt;p&gt;This is the list of available LCD devices on your system.  It lists devices by name, but also includes the size of the display. Mumble supports outputting to several LCD devices at a time.&lt;/p&gt;
&lt;h3&gt;Size:&lt;/h3&gt;
&lt;p&gt;
This field describes the size of an LCD device. The size is given either in pixels (for Graphic LCDs) or in characters (for Character LCDs).&lt;/p&gt;
&lt;h3&gt;Enabled:&lt;/h3&gt;
&lt;p&gt;This decides whether Mumble should draw to a particular LCD device.&lt;/p&gt;</source>
        <translation>&lt;p&gt;Dies ist die Liste der verfügbaren LCD-Geräte in Ihrem System. Sie listet die Geräte alphabetisch sortiert, enthält aber auch die Größe des Displays. Mumble kann Ausgaben zeitgleich an mehrere LCD Geräte ausgeben.&lt;/p&gt;
&lt;h3&gt;Größe:&lt;/h3&gt;
&lt;p&gt;Dieses Feld beschreibt die Größe eines LCD Gerätes. Die Größe ist entweder in Pixel (für grafische LCDs) oder in Zeichen (für Zeichen-LCDs) angegeben.&lt;/p&gt;
&lt;h3&gt;Aktiviert:&lt;/h3&gt;
&lt;p&gt;Dies legt fest, ob Mumble ein bestimmtes LCD Gerät verwenden soll.&lt;/p&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Size</source>
        <translation>Größe</translation>
    </message>
    <message>
        <location/>
        <source>Enabled</source>
        <translation>Aktiviert</translation>
    </message>
    <message>
        <location/>
        <source>Views</source>
        <translation>Ansichten</translation>
    </message>
    <message>
        <location/>
        <source>Minimum Column Width</source>
        <translation>Minimale Spaltenbreite</translation>
    </message>
    <message>
        <location/>
        <source>&lt;p&gt;This option decides the minimum width a column in the User View.&lt;/p&gt;
&lt;p&gt;If too many people are speaking at once, the User View will split itself into columns. You can use this option to pick a compromise between number of users shown on the LCD, and width of user names.&lt;/p&gt;
</source>
        <translation>&lt;p&gt;Diese Option legt die minimale Spaltenbreite fest.&lt;/p&gt;
&lt;p&gt;Wenn zu viele Benutzer gleichzeitig reden wird die Benutzeransicht in mehrere Spalten geteilt. Sie können diese Option verwenden, um einen Kompromiss zwischen auf dem LCD angezeigten Benutzern oder der Breite der angezeigten Benutzernamen zu finden.&lt;/p&gt;</translation>
    </message>
    <message>
        <location/>
        <source>This setting decides the width of column splitter.</source>
        <translation>Diese Einstellung legt die Spaltebreite fest.</translation>
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
        <translation type="obsolete">&lt;!DOCTYPE HTML PUBLIC &quot;-//W3C//DTD HTML 4.0//EN&quot; &quot;http://www.w3.org/TR/REC-html40/strict.dtd&quot;&gt;&lt;html&gt;&lt;head&gt;&lt;meta name=&quot;qrichtext&quot; content=&quot;1&quot; /&gt;&lt;style type=&quot;text/css&quot;&gt;p, li { white-space: pre-wrap; }&lt;/style&gt;&lt;/head&gt;&lt;body style=&quot; font-family:&apos;MS Shell Dlg 2&apos;; font-size:8.25pt; font-weight:400; font-style:normal;&quot;&gt;&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;&lt;span style=&quot; font-size:8pt;&quot;&gt;Diese Option legt die minimale Breite einer Spalte in der Spieleransicht fest.&lt;/span&gt;&lt;/p&gt;&lt;p style=&quot;-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;&lt;/p&gt;&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;Wenn zu viele Leute gleichzeitig sprechen, wird die Spieleransicht sich selbst in Spalten aufteilen.&lt;/p&gt;&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;Mit dieser Option kann ein Kompromiss zwischen der angezeigten Anzahl auf dem&lt;/p&gt;&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;LCD und der Spielerbreite geschlossen werden.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <source>TextLabel</source>
        <translation type="obsolete">Textlabel</translation>
    </message>
    <message>
        <location/>
        <source>Splitter Width</source>
        <translation>Splitterbreite</translation>
    </message>
    <message>
        <source>&lt;!DOCTYPE HTML PUBLIC &quot;-//W3C//DTD HTML 4.0//EN&quot; &quot;http://www.w3.org/TR/REC-html40/strict.dtd&quot;&gt;
&lt;html&gt;&lt;head&gt;&lt;meta name=&quot;qrichtext&quot; content=&quot;1&quot; /&gt;&lt;style type=&quot;text/css&quot;&gt;
p, li { white-space: pre-wrap; }
&lt;/style&gt;&lt;/head&gt;&lt;body style=&quot; font-family:&apos;MS Shell Dlg 2&apos;; font-size:8.25pt; font-weight:400; font-style:normal;&quot;&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;&lt;span style=&quot; font-size:8pt;&quot;&gt;This setting decides the width of column splitter.&lt;/span&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="obsolete">&lt;!DOCTYPE HTML PUBLIC &quot;-//W3C//DTD HTML 4.0//EN&quot; &quot;http://www.w3.org/TR/REC-html40/strict.dtd&quot;&gt;&lt;html&gt;&lt;head&gt;&lt;meta name=&quot;qrichtext&quot; content=&quot;1&quot; /&gt;&lt;style type=&quot;text/css&quot;&gt;p, li { white-space: pre-wrap; }&lt;/style&gt;&lt;/head&gt;&lt;body style=&quot; font-family:&apos;MS Shell Dlg 2&apos;; font-size:8.25pt; font-weight:400; font-style:normal;&quot;&gt;&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;&lt;span style=&quot; font-size:8pt;&quot;&gt;Diese Einstellung legt die Breite des Spalten-Splitters fest.&lt;/span&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
</context>
<context>
    <name>Log</name>
    <message>
        <source>[%2] %1</source>
        <translation type="obsolete">[%2] %1</translation>
    </message>
    <message>
        <location filename="Log.cpp" line="+220"/>
        <source>Debug</source>
        <translation>Debug</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Critical</source>
        <translation>Kritisch</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Warning</source>
        <translation>Warnung</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Information</source>
        <translation>Information</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Server Connected</source>
        <translation>Server verbunden</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Server Disconnected</source>
        <translation>Serververbindung abgebrochen</translation>
    </message>
    <message>
        <source>Player Joined Server</source>
        <translation type="obsolete">Benutzer betrat Server</translation>
    </message>
    <message>
        <source>Player Left Server</source>
        <translation type="obsolete">Benutzer verließ Server</translation>
    </message>
    <message>
        <source>Player kicked (you or by you)</source>
        <translation type="obsolete">Benutzer gekickt (du oder von dir)</translation>
    </message>
    <message>
        <source>Player kicked</source>
        <translation type="obsolete">Benutzer gekickt</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>You self-muted/deafened</source>
        <translation>Selbst stumm/taub gestellt</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Other self-muted/deafened</source>
        <translation>Anderer stumm/taub gestellt</translation>
    </message>
    <message>
        <location line="-3"/>
        <source>User kicked (you or by you)</source>
        <oldsource>Player muted (you)</oldsource>
        <translation>Benutzer gekickt (du oder von dir)</translation>
    </message>
    <message>
        <source>Player muted (by you)</source>
        <translation type="obsolete">Benutzer stummgestellt (durch dich)</translation>
    </message>
    <message>
        <source>Player muted (other)</source>
        <translation type="obsolete">Benutzer stummgestellt (jemand anderes)</translation>
    </message>
    <message>
        <source>Player Joined Channel</source>
        <translation type="obsolete">Benutzer betrat Kanal</translation>
    </message>
    <message>
        <source>Player Left Channel</source>
        <translation type="obsolete">Benutzer verließ Kanal</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Permission Denied</source>
        <translation>Erlaubnis verweigert</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Text Message</source>
        <translation>Textnachricht</translation>
    </message>
    <message>
        <source>Script Errors</source>
        <translation type="obsolete">Skript Fehler</translation>
    </message>
    <message>
        <location line="-13"/>
        <source>User Joined Server</source>
        <translation>Benutzer betrat den Server</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>User Left Server</source>
        <translation>Benutzer verließ den Server</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>User recording state changed</source>
        <translation>Benutzer Aufnahmestatus geändert</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>User kicked</source>
        <translation>Benutzer gekickt</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>User muted (you)</source>
        <translation>Benutzer stumm gestellt (du selbst)</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>User muted (by you)</source>
        <translation>Benutzer stumm gestellt (durch dich)</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>User muted (other)</source>
        <translation>Benutzer stummgestellt (jemand anderes)</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>User Joined Channel</source>
        <translation>Benutzer betrat Kanal</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>User Left Channel</source>
        <translation>Benutzer verließ den Kanal</translation>
    </message>
    <message>
        <location line="+54"/>
        <source>the server</source>
        <translation>den Server</translation>
    </message>
    <message>
        <location line="+117"/>
        <source>[[ Text object too large to display ]]</source>
        <translation>[[ Text zu groß um dargestellt zu werden ]]</translation>
    </message>
    <message>
        <location line="+42"/>
        <source>[Date changed to %1]
</source>
        <translation>[Datum geändert nach %1]
</translation>
    </message>
    <message>
        <location line="+142"/>
        <source>link to %1</source>
        <translation>Link zu %1</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>ftp link to %1</source>
        <translation>FTP Link zu %1</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>player link</source>
        <translation>Link auf Benutzer</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>channel link</source>
        <translation>Link auf Kanal</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1 link</source>
        <translation>%1 Link</translation>
    </message>
</context>
<context>
    <name>LogConfig</name>
    <message>
        <location line="-521"/>
        <source>Toggle console for %1 events</source>
        <translation>Wechsle Einstellung für Konsole für %1 Ereignisse</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Toggle pop-up notifications for %1 events</source>
        <translation>De-/Aktiviere Pop-up Benachrichtgungen für %1 Ereignisse</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Toggle Text-To-Speech for %1 events</source>
        <translation>De-/Aktiviere Text-zu-Sprache für %1 Ereignisse</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Click here to toggle sound notification for %1 events</source>
        <translation>Klicken Sie hier, um die Audio-Hinweise für %1 Ereignisse zu wechseln</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Path to sound file used for sound notifications in the case of %1 events&lt;br /&gt;Single click to play&lt;br /&gt;Double-click to change</source>
        <oldsource>Path to soundfile used for sound notifications in the case of %1 events&lt;br /&gt;Single click to play&lt;br /&gt;Doubleclick to change</oldsource>
        <translation>Pfad zur Sounddatei, die für die Audio-Benachrichtigung für %1 Ereignisse verwendet wird.&lt;br /&gt;Einfacher Klick zum abspielen&lt;br /&gt;Doppelklick zum ändern</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Click here to toggle console output for %1 events.&lt;br /&gt;If checked, this option makes Mumble output all %1 events in its message log.</source>
        <translation>Klicken Sie hier, um die Konsolenausgabe für %1 Ereignisse zu wechseln (zu aktivieren oder zu deaktivieren).&lt;br /&gt;Wenn gewählt gibt Mumble alle %1 Ereignisse in seinem Nachrichten-Log aus.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Click here to toggle pop-up notifications for %1 events.&lt;br /&gt;If checked, a notification pop-up will be created by Mumble for every %1 event.</source>
        <oldsource>Click here to toggle pop-up notifications for %1 events.&lt;br /&gt;If checked, a notification pop-up will be created by mumble for every %1 event.</oldsource>
        <translation>Klicken Sie hier, um die Pop-up Benachrichtigungen für %1 Ereignisse zu aktivieren oder zu deaktivieren.&lt;br /&gt;
Wenn gewählt, wird für jedes %1 Ereignis ein Benachrichtigungs-Pop-up erzeugt.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Click here to toggle Text-To-Speech for %1 events.&lt;br /&gt;If checked, Mumble uses Text-To-Speech to read %1 events out loud to you. Text-To-Speech is also able to read the contents of the event which is not true for sound files. Text-To-Speech and sound files cannot be used at the same time.</source>
        <oldsource>Click here to toggle Text-To-Speech for %1 events.&lt;br /&gt;If checked, Mumble uses Text-To-Speech to read %1 events out loud to you. Text-To-Speech is also able to read the contents of the event which is not true for soundfiles. Text-To-Speech and soundfiles cannot be used at the same time.</oldsource>
        <translation>Klicken Sie hier, um Text-zu-Sprache für %1 Ereignisse zu aktivieren oder zu deaktivieren.&lt;br /&gt;
Wenn gewählt veranlasst Mumble, dass %1 Ereignisse vorgelesen werden. Text-zu-Sprache kann auch den Inhalt von Ereignissen vorlesen, was Sounddateien nicht können. Text-zu-Sprache und Sounddateien können nicht gleichzeitig verwendet werden.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Click here to toggle sound notification for %1 events.&lt;br /&gt;If checked, Mumble uses a sound file predefined by you to indicate %1 events. Sound files and Text-To-Speech cannot be used at the same time.</source>
        <oldsource>Click here to toggle sound notification for %1 events.&lt;br /&gt;If checked, Mumble uses a soundfile predefined by you to indicate %1 events. Soundfiles and Text-To-Speech cannot be used at the same time.</oldsource>
        <translation>Klicken Sie hier, um Sound-Benachrichtigungen für %1 Ereignisse zu aktivieren oder zu deaktivieren.&lt;br /&gt;Wenn gewählt, verwendet Mumble von ihnen gewählte Sounddateien um %1 Ereignisse zu kennzeichnen. Sounddateien und Text-zu-Sprache können nicht gleichzeitig verwendet werden.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Path to sound file used for sound notifications in the case of %1 events.&lt;br /&gt;Single click to play&lt;br /&gt;Double-click to change&lt;br /&gt;Ensure that sound notifications for these events are enabled or this field will not have any effect.</source>
        <translation>Pfad zur Sounddatei zur Wiedergabe von Sound-Hinweisen im Falle von %1 Ereignissen.&lt;br /&gt;Einfaches Klicken zum Abspielen&lt;br /&gt;Doppelklicken zum Ändern&lt;br /&gt;Stellen Sie sicher, dass die Sound-Hinweise für diesen Ereignistyp aktiviert sind, ansonsten hat diese Einstellung keinen Effekt.</translation>
    </message>
    <message>
        <source>Choose sound file</source>
        <translation type="obsolete">Wählen Sie eine Sounddatei</translation>
    </message>
    <message>
        <source>Invalid sound file</source>
        <translation type="obsolete">Ungültige Sounddatei</translation>
    </message>
    <message>
        <source>The file &apos;%1&apos; does not exist or is not a valid file.</source>
        <translation type="obsolete">Die Datei &apos;%1&apos; existiert nicht oder ist keine gültige Datei.</translation>
    </message>
    <message>
        <source>The file &apos;%1&apos; does not exist or is not a valid speex file.</source>
        <translation type="obsolete">Die Datei &apos;%1&apos; existiert nicht oder ist keine gültige speex Datei.</translation>
    </message>
    <message>
        <location filename="Log.ui"/>
        <source>Messages</source>
        <translation>Nachrichten</translation>
    </message>
    <message>
        <location/>
        <source>Console</source>
        <translation>Konsole</translation>
    </message>
    <message>
        <source>TTS</source>
        <translation type="obsolete">TTS</translation>
    </message>
    <message>
        <source>Enable console for %1</source>
        <translation type="obsolete">Konsole für %1 einschalten</translation>
    </message>
    <message>
        <source>Enable Text-To-Speech for %1</source>
        <translation type="obsolete">Text-zu-Sprache für %1 einschalten</translation>
    </message>
    <message>
        <location/>
        <source>Text To Speech</source>
        <translation>Text-zu-Sprache</translation>
    </message>
    <message>
        <location/>
        <source>Volume</source>
        <translation>Lautstärke</translation>
    </message>
    <message>
        <location/>
        <source>Volume of Text-To-Speech Engine</source>
        <translation>Lautstärke der Text-zu-Sprache-Engine</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is the volume used for the speech synthesis.&lt;/b&gt;</source>
        <translation>&lt;b&gt;Dies ist die Lautstärke für die Sprachsynthese.&lt;/b&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Length threshold</source>
        <translation>Längen-Schwellwert</translation>
    </message>
    <message>
        <location/>
        <source>Message length threshold for Text-To-Speech Engine</source>
        <translation>Nachrichtenlängen-Schwellwert für die Text-zu-Sprache-Engine</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is the length threshold used for the Text-To-Speech Engine.&lt;/b&gt;&lt;br /&gt;Messages longer than this limit will not be read aloud in their full length.</source>
        <translation>&lt;b&gt;Dieser Längen-Schwellwert wird von der Text-zu-Sprache-Engine genutzt.&lt;/b&gt;&lt;br /&gt;Nachrichten die Länger als der Grenzwert sind, werden nicht in voller Länge laut vorgelesen.</translation>
    </message>
    <message>
        <location/>
        <source>Whisper</source>
        <translation>Flüstern</translation>
    </message>
    <message>
        <location/>
        <source>If checked you will only hear whispers from users you added to your friend list.</source>
        <translation>Wenn diese Option gewählt ist erhalten Sie Flüsternachrichten nur von Benutzern auf Ihrer Freundesliste.</translation>
    </message>
    <message>
        <location/>
        <source>Only accept whispers from friends</source>
        <translation>Flüstern nur von Freunden akzeptieren</translation>
    </message>
    <message>
        <location/>
        <source>Message</source>
        <translation>Nachricht</translation>
    </message>
    <message>
        <location/>
        <source>Notification</source>
        <translation>Benachrichtigung</translation>
    </message>
    <message>
        <location/>
        <source>Text-To-Speech</source>
        <translation>Text-zu-Sprache</translation>
    </message>
    <message>
        <location/>
        <source>Soundfile</source>
        <translation>Sounddatei</translation>
    </message>
    <message>
        <location/>
        <source>Path</source>
        <translation>Pfad</translation>
    </message>
    <message>
        <location/>
        <source> Characters</source>
        <translation> Zeichen</translation>
    </message>
</context>
<context>
    <name>LookConfig</name>
    <message>
        <location filename="LookConfig.ui"/>
        <source>Language</source>
        <translation>Sprache</translation>
    </message>
    <message>
        <location/>
        <source>Look and Feel</source>
        <translation>Aussehen</translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="+52"/>
        <location line="+11"/>
        <source>System default</source>
        <translation>Systemstandard</translation>
    </message>
    <message>
        <location filename="LookConfig.ui"/>
        <source>Language to use (requires restart)</source>
        <translation>Sprachauswahl (benötigt Neustart)</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets which language Mumble should use.&lt;/b&gt;&lt;br /&gt;You have to restart Mumble to use the new language.</source>
        <translation>&lt;b&gt;Dies legt die Sprache fest, die Mumble verwenden soll.&lt;/b&gt;&lt;br /&gt;Ein Neustart ist nötig um die neue Sprache zu nutzen.</translation>
    </message>
    <message>
        <location/>
        <source>Style</source>
        <translation>Stil</translation>
    </message>
    <message>
        <location/>
        <source>Layout</source>
        <translation>Anordnung</translation>
    </message>
    <message>
        <location/>
        <source>Classic</source>
        <translation>Klassisch</translation>
    </message>
    <message>
        <location/>
        <source>Stacked</source>
        <translation>Gestapelt</translation>
    </message>
    <message>
        <location/>
        <source>Hybrid</source>
        <translation>Hybrid</translation>
    </message>
    <message>
        <location/>
        <source>Custom</source>
        <translation>Benutzerdefiniert</translation>
    </message>
    <message>
        <location/>
        <source>This changes the behavior when moving channels.</source>
        <translation>Ändert das Verhalten beim Bewegen von Kanälen.</translation>
    </message>
    <message>
        <location/>
        <source>This sets the behavior of channel drags; it can be used to prevent accidental dragging. &lt;i&gt;Move&lt;/i&gt; moves the channel without prompting. &lt;i&gt;Do Nothing&lt;/i&gt; does nothing and prints an error message. &lt;i&gt;Ask&lt;/i&gt; uses a message box to confirm if you really wanted to move the channel.</source>
        <oldsource>This sets the behavior of channel drags; it can be used to prevent accidental dragging. &lt;i&gt;Move Channel&lt;/i&gt; moves the channel without prompting. &lt;i&gt;Do Nothing&lt;/i&gt; does nothing and prints an error message. &lt;i&gt;Ask&lt;/i&gt; uses a message box to confirm if you really wanted to move the channel.</oldsource>
        <translation>Dies verändert das Verhalten beim Verschieben von Kanälen. Es kann verwendet werden, um versehentliches Verschieben von Kanälen zu verhindern. &lt;i&gt;Nichts tun&lt;/i&gt; tut nichts und gibt eine Fehlermeldung aus. &lt;i&gt;Fragen&lt;/i&gt; fragt in einem Nachrichtenfenster ob der Kanal wirklich verschoben werden soll.</translation>
    </message>
    <message>
        <location/>
        <source>Basic widget style</source>
        <translation>Einfacher Widget-Stil</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the basic look and feel to use.&lt;/b&gt;</source>
        <translation>&lt;b&gt;Dies legt das einfache Aussehen fest.&lt;/b&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Skin</source>
        <translation>Skin</translation>
    </message>
    <message>
        <location/>
        <source>Skin file to use</source>
        <translation>Zu verwendende Skindatei</translation>
    </message>
    <message>
        <location/>
        <source>This setting controls when the application will be always on top.</source>
        <translation>Diese Einstellung legt fest, wann die Anwendung immer über anderen angezeigt wird (“always on top”).</translation>
    </message>
    <message>
        <location/>
        <source>Never</source>
        <translation>Niemals</translation>
    </message>
    <message>
        <location/>
        <source>Always</source>
        <translation>Immer</translation>
    </message>
    <message>
        <location/>
        <source>In minimal view</source>
        <translation>In der Minimalansicht</translation>
    </message>
    <message>
        <location/>
        <source>In normal view</source>
        <translation>In der Normalansicht</translation>
    </message>
    <message>
        <source>Check to show chat bar</source>
        <translation type="obsolete">Auswählen um die Chatbar anzuzeigen</translation>
    </message>
    <message>
        <source>&lt;b&gt;If checked the chat bar is shown.&lt;/b&gt;&lt;br /&gt;Uncheck this to hide it.</source>
        <oldsource>&lt;b&gt;If checked the chat bar is shown&lt;/b&gt;&lt;br /&gt;Uncheck to hide it</oldsource>
        <translation type="obsolete">&lt;b&gt;Wenn diese Option ausgewählt ist wird der Chat-Balken angezeigt.&lt;/b&gt;&lt;br/&gt;
Entfernen Sie die Auswahl um ihn zu verstecken.</translation>
    </message>
    <message>
        <source>Show chatbar</source>
        <translation type="obsolete">Zeige Chatbar</translation>
    </message>
    <message>
        <location/>
        <source>Show number of users in each channel</source>
        <translation>Zeige die Anzahl der Benutzer in jedem Kanal</translation>
    </message>
    <message>
        <location/>
        <source>Show channel user count</source>
        <translation>Anzahl der Benutzer im Kanal anzeigen</translation>
    </message>
    <message>
        <source>Displays talking status in tray</source>
        <translation type="obsolete">Zeigt den Sendestatus im Tray</translation>
    </message>
    <message>
        <location/>
        <source>Show talking status in tray icon</source>
        <translation>Zeige Sprechstatus im Tray Icon</translation>
    </message>
    <message>
        <source>Enabled advanced channel editor</source>
        <translation type="obsolete">Aktiviere erweiterten Kaneleditor</translation>
    </message>
    <message>
        <source>Advanced channel editor</source>
        <translation type="obsolete">Erweiterter Kanaleditor</translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets which skin Mumble should use.&lt;/b&gt;&lt;br /&gt;The skin is a style file applied on top of the basic widget style.</source>
        <translation type="obsolete">&lt;b&gt;Dies legt fest, welchen Skin Mumble nutzen soll.&lt;/b&gt;&lt;br /&gt;Der Skin ist eine Stildatei die auf den einfachen Widgetstil angewandt wird.</translation>
    </message>
    <message>
        <source>Use Horizontal Splitter</source>
        <translation type="obsolete">Benutze horizontale Splitter</translation>
    </message>
    <message>
        <source>List players above subchannels (requires restart).</source>
        <translation type="obsolete">Liste Spieler über Unterkanälen auf (benötigt Neustart).</translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="+14"/>
        <source>User Interface</source>
        <translation>Benutzerinterface</translation>
    </message>
    <message>
        <source>...</source>
        <translation type="obsolete">...</translation>
    </message>
    <message>
        <location line="+130"/>
        <source>Choose skin file</source>
        <translation>Wähle Skindatei</translation>
    </message>
    <message>
        <source>&lt;b&gt;If set, players will be shown above subchannels in the channel view.&lt;/b&gt;&lt;br /&gt;A restart of Mumble is required to see the change.</source>
        <translation type="obsolete">&lt;b&gt;Wenn gesetzt werden Spieler im darüberliegenden Unterkanal in der Kanalansicht angezeigt.&lt;/b&gt;&lt;br /&gt;Ein Neustart von Mumble ist nötig, um die Änderungen zu sehen.</translation>
    </message>
    <message>
        <source>Expand All Channels</source>
        <translation type="obsolete">Expandiere alle Kanäle</translation>
    </message>
    <message>
        <source>Expand all channels when connecting</source>
        <translation type="obsolete">Expandiere alle Kanäle beim Verbindungsaufbau</translation>
    </message>
    <message>
        <source>&lt;b&gt;If set, all channels will be expanded by default when you connect to a server.&lt;/b&gt;</source>
        <translation type="obsolete">&lt;b&gt;Wenn gesetzt werden alle Kanäle standardmäßig beim Verbindungsaufbau zum Server expandiert.&lt;/b&gt;</translation>
    </message>
    <message>
        <source>Players above Channels</source>
        <translation type="obsolete">Spieler über Kanälen</translation>
    </message>
    <message>
        <location filename="LookConfig.ui"/>
        <source>&lt;b&gt;This sets which skin Mumble should use.&lt;/b&gt;&lt;br /&gt;The skin is a style file applied on top of the basic widget style. If there are icons in the same directory as the style sheet, those will replace the default icons.</source>
        <translation>&lt;b&gt;Dies legt fest, welchen Skin Mumble verwenden soll.&lt;/b&gt;&lt;br /&gt;Der Skin ist eine Stildatei, die über den einfachen Widgetstil gelegt wird. Wenn Icons im selben Verzeichnis wie die Stilvorlage liegen, so werden die Standard-Icons ausgetauscht.</translation>
    </message>
    <message>
        <source>Form</source>
        <translation type="obsolete">Formular</translation>
    </message>
    <message>
        <source>New version check</source>
        <translation type="obsolete">Überprüfe neue Version</translation>
    </message>
    <message>
        <source>Check for updates on startup</source>
        <translation type="obsolete">Prüfe nach Updates bei Start</translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="-140"/>
        <source>None</source>
        <translation>Keine</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Only with users</source>
        <oldsource>Only with players</oldsource>
        <translation>Nur mit Benutzern</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>All</source>
        <translation>Alle</translation>
    </message>
    <message>
        <location filename="LookConfig.ui"/>
        <source>Expand</source>
        <translation>Expandieren</translation>
    </message>
    <message>
        <location/>
        <source>When to automatically expand channels</source>
        <translation>Wann sollen Kanäle automatisch erweitert werden</translation>
    </message>
    <message>
        <source>This sets which channels to automatically expand. &lt;i&gt;None&lt;/i&gt; and &lt;i&gt;All&lt;/i&gt; will expand no or all channels, while &lt;i&gt;Only with players&lt;/i&gt; will expand and collapse channels as players join and leave them.</source>
        <translation type="obsolete">Dies legt fest, welche Kanäle automatisch expandiert werden sollen.&lt;i&gt;Keine&lt;/i&gt; und &lt;i&gt;Alle&lt;/i&gt; expandieren keinen, respektive alle Kanäle, während &lt;i&gt;Nur mit Benutzer&lt;/i&gt; die Kanäle expandiert bzw. zusammenschließt in denen Benutzer beitreten bzw. verlassen.</translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="+2"/>
        <source>Ask</source>
        <translation>Fragen</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Do Nothing</source>
        <translation>Nichts tun</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Move</source>
        <translation>Bewegen</translation>
    </message>
    <message>
        <location filename="LookConfig.ui"/>
        <source>Channel Dragging</source>
        <translation>Kanal ziehen</translation>
    </message>
    <message>
        <source>This changes the behaviour when moving channels.</source>
        <translation type="obsolete">Dies ändert das Verhalten, wenn Kanäle bewegt werden.</translation>
    </message>
    <message>
        <source>This sets the behaviour of channel drags, it can be used to prevent accidental dragging. &lt;i&gt;Move Channel&lt;/i&gt; moves the channel without prompting. &lt;i&gt;Do Nothing&lt;/i&gt; does nothing and prints an error message. &lt;i&gt;Ask&lt;/i&gt; uses a message box to confirm if you really wanted to move the channel.</source>
        <translation type="obsolete">Dies legt das Verhalten vom Kanal ziehen fest, um unabsichtliches Ziehen zu verhindern.&lt;i&gt;Bewege Kanal&lt;/i&gt; bewegt den Kanal ohne nachzufragen.&lt;i&gt;Tue Nichts&lt;/i&gt; tut nichts und zeigt eine Fehlermeldung an.&lt;i&gt;Frage nach&lt;/i&gt; zeigt ein Nachrichtenfenster an, mit dem das Ziehen bestätigt werden kann.</translation>
    </message>
    <message>
        <location/>
        <source>Ask whether to close or minimize when quitting Mumble.</source>
        <translation>Fragen, ob das Mumblefenster geschlossen oder minimiert werden soll, wenn Mumble beendet wird.</translation>
    </message>
    <message>
        <source>&lt;b&gt;If set, will verify you want to quit if connected.&lt;b&gt;</source>
        <translation type="obsolete">&lt;b&gt;Wenn gesetzt, wird gefragt ob beendet werden soll, wenn noch verbunden ist.&lt;b&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Ask on quit while connected</source>
        <translation>Frage beim Beenden mit aktiver Verbindung nach</translation>
    </message>
    <message>
        <source>Make the Mumble window appear on top of other windows.</source>
        <translation type="obsolete">Lässt das Mumble-Fenster über allen anderen Fenstern erscheinen.</translation>
    </message>
    <message>
        <source>&lt;b&gt;This makes the Mumble window a topmost window.&lt;/b&gt;</source>
        <translation type="obsolete">&lt;b&gt;Dies lässt das Mumblefenster das oberste Fenster sein.&lt;/b&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Always On Top</source>
        <translation>Immer oben</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;If set, minimizing the Mumble main window will cause it to be hidden and accessible only from the tray. Otherwise, it will be minimized as a window normally would.&lt;/b&gt;</source>
        <translation>&lt;b&gt;Wenn gesetzt, wird das Mumble-Hauptfenster in den Infobereich minimiert. Anderenfalls wird es wie üblich minimiert.&lt;/b&gt;</translation>
    </message>
    <message>
        <location/>
        <source>This sets which channels to automatically expand. &lt;i&gt;None&lt;/i&gt; and &lt;i&gt;All&lt;/i&gt; will expand no or all channels, while &lt;i&gt;Only with users&lt;/i&gt; will expand and collapse channels as users join and leave them.</source>
        <translation>Dies setzt welche Kanäle automatisch expandiert werden. &lt;i&gt;Keine&lt;/i&gt; und &lt;i&gt;Alle&lt;/i&gt; expandieren keine und alle Kanäle, während &lt;i&gt;Nur mit Benutzern&lt;/i&gt; Kanäle expandiert und schließt wenn Benutzer diese betreten oder verlassen.</translation>
    </message>
    <message>
        <location/>
        <source>List users above subchannels (requires restart).</source>
        <translation>Zeige Benutzer über den Kanälen (benötigt einen Neustart).</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;If set, users will be shown above subchannels in the channel view.&lt;/b&gt;&lt;br /&gt;A restart of Mumble is required to see the change.</source>
        <translation>&lt;b&gt;Wenn gesetzt, werden Benutzer über den Unterkanälen angezeigt.&lt;/b&gt;&lt;br /&gt;Ein Neustart von Mumble ist notwendig damit die Änderung wirksam wird.</translation>
    </message>
    <message>
        <location/>
        <source>Users above Channels</source>
        <translation>Benutzer über Kanälen</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;If set, will verify you want to quit if connected.&lt;/b&gt;</source>
        <translation>&lt;b&gt;Wenn aktiviert, kommt beim Beenden eine Abfrage.&lt;/b&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Hide in tray when minimized</source>
        <translation>Verstecke im Tray, wenn minimiert</translation>
    </message>
    <message>
        <source>Show all configuration items</source>
        <translation type="obsolete">Zeige alle Konfigurationselemente</translation>
    </message>
    <message>
        <source>&lt;b&gt;This will show all configuration items.&lt;/b&gt;&lt;br /&gt;Mumble contains a lot of configuration items that most users won&apos;t need to change. Checking this will show all configurable items.</source>
        <translation type="obsolete">&lt;b&gt;Dies zeigt alle Konfigurationselemente.&lt;/b&gt;&lt;br /&gt;Mumble enthält viele Konfigurationselemente, die die meisten Benutzer nicht ändern werden. Wird dies aktiviert werden alle konfigurierbaren Elemente angezeigt.</translation>
    </message>
    <message>
        <source>Expert Config</source>
        <translation type="obsolete">Expertenkonfiguration</translation>
    </message>
    <message>
        <location/>
        <source>Hide the main Mumble window in the tray when it is minimized.</source>
        <translation>Verstecke das Mumble-Fenster im Tray, wenn es minimiert wird.</translation>
    </message>
    <message>
        <location/>
        <source>Displays talking status in system tray</source>
        <translation>Zeigt den Sprechstatus im System Tray</translation>
    </message>
    <message>
        <location/>
        <source>This setting controls in which situations the application will stay always on top. If you select &lt;i&gt;Never&lt;/i&gt; the application will not stay on top. &lt;i&gt;Always&lt;/i&gt; will always keep the application on top. &lt;i&gt;In minimal view&lt;/i&gt; / &lt;i&gt;In normal view&lt;/i&gt; will only keep the application always on top when minimal view is activated / deactivated.</source>
        <translation>Diese Einstellung legt fest, in welchen Situationen das Mumble-Fenster immer über den anderen bleibt. Wenn Sie &lt;i&gt;Niemals&lt;/i&gt; wählen bleibt das Fenster niemals oben auf. &lt;i&gt;Immer&lt;/i&gt; zeigt das Fenster immer über den anderen. &lt;i&gt;In der Minimalansicht&lt;/i&gt;/&lt;i&gt;In der Normalansicht&lt;/i&gt; lässt das Fenster nur im jeweiligen Anzeigemodus über den anderen stehen.</translation>
    </message>
    <message>
        <location/>
        <source>Show context menu in menu bar</source>
        <translation>Zeige Kontextmenü in der Menüleiste</translation>
    </message>
    <message>
        <location/>
        <source>Apply some high contrast optimizations for visually impaired users</source>
        <translation>Auf hohen Kontrast optimierte Darstellung für sehbehinderte Benutzer verwenden</translation>
    </message>
    <message>
        <location/>
        <source>Optimize for high contrast</source>
        <translation>Optimiere für hohen Kontrast</translation>
    </message>
    <message>
        <location/>
        <source>Application</source>
        <translation>Anwendung</translation>
    </message>
    <message>
        <location/>
        <source>Adds user and channel context menus into the menu bar</source>
        <translation>Fügt Benutzer- und Kanalkontextmenüs zur Menüleiste hinzu</translation>
    </message>
    <message>
        <location/>
        <source>Tray Icon</source>
        <translation>Tray Icon</translation>
    </message>
    <message>
        <location/>
        <source>Channel Tree</source>
        <translation>Kanalbaum</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Browse...</source>
        <translation>&amp;Durchsuchen...</translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <location filename="MainWindow.cpp" line="+171"/>
        <location line="+2048"/>
        <source>Root</source>
        <translation>Hauptkanal</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>&amp;Connect</source>
        <translation>&amp;Verbinden</translation>
    </message>
    <message>
        <location/>
        <source>Open the server connection dialog</source>
        <translation>Öffne den Serververbindungs-Dialog</translation>
    </message>
    <message>
        <source>Shows a dialog of registered servers, and also allows quick connect.</source>
        <translation type="obsolete">Zeigt einen Dialog mit registrierten Servern und erlaubt schnelles Verbinden.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Disconnect</source>
        <translation>Tre&amp;nnen</translation>
    </message>
    <message>
        <location/>
        <source>Disconnect from server</source>
        <translation>Verbindung zum Server trennen</translation>
    </message>
    <message>
        <location/>
        <source>Disconnects you from the server.</source>
        <translation>Trennt Ihre Verbindung zum Server.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Ban lists</source>
        <translation>&amp;Bannliste</translation>
    </message>
    <message>
        <location/>
        <source>Edit ban lists on server</source>
        <translation>Bannliste des Servers bearbeiten</translation>
    </message>
    <message>
        <location/>
        <source>This lets you edit the server-side IP ban lists.</source>
        <translation>Dies lässt Sie die serverseitige IP-Bannliste bearbeiten.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Kick</source>
        <translation>&amp;Kicken</translation>
    </message>
    <message>
        <source>Kick player (with reason)</source>
        <translation type="obsolete">Benutzer kicken (mit Grund)</translation>
    </message>
    <message>
        <source>Kick selected player off server. You&apos;ll be asked to specify a reason.</source>
        <translation type="obsolete">Den ausgewählten Benutzer vom Server werfen. Sie werden aufgefordert, einen Grund anzugeben.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Ban</source>
        <translation>&amp;Bannen</translation>
    </message>
    <message>
        <source>Kick and ban player (with reason)</source>
        <translation type="obsolete">Benutzer vom Server kicken und bannen (mit Grund)</translation>
    </message>
    <message>
        <source>Kick and ban selected player from server. You&apos;ll be asked to specify a reason.</source>
        <translation type="obsolete">Den ausgewählten Benutzer vom Server werfen und bannen. Man wird aufgefordert, einen Grund anzugeben.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Mute</source>
        <translation>&amp;Stumm stellen</translation>
    </message>
    <message>
        <source>Mute player</source>
        <translation type="obsolete">Benutzer stumm stellen</translation>
    </message>
    <message>
        <source>Mute or unmute player on server. Unmuting a deafened player will also undeafen them.</source>
        <translation type="obsolete">Den Benutzer stumm stellen oder das reden wieder erlauben. Erlaubt man einem Taub gestellten Benutzer das reden, darf er automatisch auch wieder etwas hören.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Deafen</source>
        <translation>&amp;Taub stellen</translation>
    </message>
    <message>
        <source>Deafen player</source>
        <translation type="obsolete">Benutzer taub stellen</translation>
    </message>
    <message>
        <source>Deafen or undeafen player on server. Deafening a player will also mute them.</source>
        <translation type="obsolete">Benutzer taub stellen oder das zuhören wieder erlauben. Einen Benutzer taub zu stellen wird ihn automatisch auch stumm stellen.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Local Mute</source>
        <translation>&amp;Lokal stumm stellen</translation>
    </message>
    <message>
        <source>Mute or unmute player locally.</source>
        <translation type="obsolete">Benutzer lokal stumm stellen oder entstummen.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Add</source>
        <translation>&amp;Hinzufügen</translation>
    </message>
    <message>
        <location/>
        <source>Add new channel</source>
        <translation>Neuen Kanal hinzufügen</translation>
    </message>
    <message>
        <location/>
        <source>This adds a new sub-channel to the currently selected channel.</source>
        <translation>Dies fügt dem aktuell gewählten Kanal einen Unterkanal hinzu.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Remove</source>
        <translation>&amp;Entfernen</translation>
    </message>
    <message>
        <location/>
        <source>Remove channel</source>
        <translation>Kanal entfernen</translation>
    </message>
    <message>
        <location/>
        <source>This removes a channel and all sub-channels.</source>
        <translation>Dies entfernt einen Kanal und all seine Unterkanäle.</translation>
    </message>
    <message>
        <source>&amp;Edit ACL</source>
        <translation type="obsolete">ACL &amp;editieren</translation>
    </message>
    <message>
        <location/>
        <source>Edit Groups and ACL for channel</source>
        <translation>Gruppen und Berechtigungen des Kanals bearbeiten</translation>
    </message>
    <message>
        <location/>
        <source>This opens the Group and ACL dialog for the channel, to control permissions.</source>
        <translation>Dies öffnet den Gruppen- und Berechtigungen-Dialog des Kanals um die Berechtigungen einzustellen.</translation>
    </message>
    <message>
        <source>&amp;Rename</source>
        <translation type="obsolete">&amp;Umbenennen</translation>
    </message>
    <message>
        <source>&amp;Change Description</source>
        <translation type="obsolete">&amp;Beschreibung ändern</translation>
    </message>
    <message>
        <source>Changes the channel description</source>
        <translation type="obsolete">Ändert die Kanalbeschreibung</translation>
    </message>
    <message>
        <source>This changes the description of a channel.</source>
        <translation type="obsolete">Dies ändert die Beschreibung eines Kanals.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Link</source>
        <translation>&amp;Verknüpfen</translation>
    </message>
    <message>
        <location/>
        <source>Link your channel to another channel</source>
        <translation>Verknüpft ihren aktuellen Kanal mit einem anderen</translation>
    </message>
    <message>
        <source>This links your current channel to the selected channel. If they have permission to speak in the other channel, players can now hear each other. This is a permanent link, and will last until manually unlinked or the server is restarted. Please see the shortcuts for push-to-link.</source>
        <translation type="obsolete">Dies verbindet den aktuellen Kanal mit dem Gewählten. Wenn Spieler Spracherlaubnis im anderen Kanal haben, so können sie sich jetzt gegenseitig hören. Dies ist eine permanente Verbindung die solange bestehen bleibt, bis die Verknüpfung manuell aufgehoben oder der Server neugestart wird.</translation>
    </message>
    <message>
        <source>&amp;Unlink</source>
        <translation type="obsolete">&amp;Unlink</translation>
    </message>
    <message>
        <location/>
        <source>Unlink your channel from another channel</source>
        <translation>Die Verknüpfung des aktuellen Kanals zu einem anderen Kanal trennen</translation>
    </message>
    <message>
        <location/>
        <source>This unlinks your current channel from the selected channel.</source>
        <translation>Dies trennt die Verknüpfung des aktuellen Kanals zum Ausgewählten.</translation>
    </message>
    <message>
        <source>Unlink &amp;All</source>
        <translation type="obsolete">&amp;Alle Verknüpfungen entfernen</translation>
    </message>
    <message>
        <location/>
        <source>Unlinks your channel from all linked channels.</source>
        <translation>Trennt alle verbundenen Kanäle.</translation>
    </message>
    <message>
        <location/>
        <source>This unlinks your current channel (not the selected one) from all linked channels.</source>
        <translation>Dies trennt die Verbindungen des aktuellen Kanals (nicht des ausgewählten) zu allen verknüpften Kanälen.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Reset</source>
        <translation>&amp;Zurücksetzen</translation>
    </message>
    <message>
        <location/>
        <source>Reset audio preprocessor</source>
        <translation>Setzt den Audio-Präprozessor zurück</translation>
    </message>
    <message>
        <location/>
        <source>This will reset the audio preprocessor, including noise cancellation, automatic gain and voice activity detection. If something suddenly worsens the audio environment (like dropping the microphone) and it was temporary, use this to avoid having to wait for the preprocessor to readjust.</source>
        <translation>Setzt den Audio-Präprozessor zurück: unter anderem Geräuschunterdrückung, automatische Gain und Sprachaktivitätserkennung. Wenn etwas plötzlich die Audioumgebung verschlechtert (z.B. das Fallen lassen des Mikrofons), so kann dies verwendet werden um nicht auf das Anpassen des Präprozessor warten zu müssen.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Mute Self</source>
        <translation>Selbst &amp;Stumm stellen</translation>
    </message>
    <message>
        <location/>
        <source>Mute yourself</source>
        <translation>Sich selbst stumm stellen</translation>
    </message>
    <message>
        <location/>
        <source>Mute or unmute yourself. When muted, you will not send any data to the server. Unmuting while deafened will also undeafen.</source>
        <translation>Stellen Sie sich selbst stumm oder deaktivieren es. Wenn Sie stummgestellt sind werden keine Daten von Ihnen an den Server gesendet. Deaktivieren Sie das Stummstellen während Sie taubgestellt sind, wird dieses auch deaktiviert.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Deafen Self</source>
        <translation>Selbst &amp;Taub stellen</translation>
    </message>
    <message>
        <location/>
        <source>Deafen yourself</source>
        <translation>Sich selbst taubstellen</translation>
    </message>
    <message>
        <location/>
        <source>Deafen or undeafen yourself. When deafened, you will not hear anything. Deafening yourself will also mute.</source>
        <translation>Sich selbst Taubstellen an/aus. Wenn taubgestellt hört man nichts. Sich taub zu stellen stellt einen automatisch stumm.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Text-To-Speech</source>
        <translation>&amp;Text-Zu-Sprache</translation>
    </message>
    <message>
        <location/>
        <source>Toggle Text-To-Speech</source>
        <translation>Text-Zu-Sprache ein-/ausschalten</translation>
    </message>
    <message>
        <location/>
        <source>Enable or disable the text-to-speech engine. Only messages enabled for TTS in the Configuration dialog will actually be spoken.</source>
        <translation>Text-Zu-Sprache-Engine ein- oder ausschalten. Nur Nachrichten die in den Einstellungen aktiviert sind werden auch tatsächlich gesprochen.</translation>
    </message>
    <message>
        <source>S&amp;tatistics</source>
        <translation type="obsolete">St&amp;atistiken</translation>
    </message>
    <message>
        <location/>
        <source>Display audio statistics</source>
        <translation>Zeigt Audiostatistiken an</translation>
    </message>
    <message>
        <location/>
        <source>Pops up a small dialog with information about your current audio input.</source>
        <translation>Zeigt einen kleinen Dialog mit Informationen über den aktuellen Audio-Eingang.</translation>
    </message>
    <message>
        <location/>
        <source>Forcibly unlink plugin</source>
        <translation>Trennen des Plugins erzwingen</translation>
    </message>
    <message>
        <location/>
        <source>This forces the current plugin to unlink, which is handy if it is reading completely wrong data.</source>
        <translation>Dies zwingt das aktuelle Plugin zur Trennung, was hilfreich sein kann, wenn es völlig falsche Daten einliest.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Settings</source>
        <translation>&amp;Einstellungen</translation>
    </message>
    <message>
        <location/>
        <source>Configure Mumble</source>
        <translation>Mumble konfigurieren</translation>
    </message>
    <message>
        <location/>
        <source>Allows you to change most settings for Mumble.</source>
        <translation>Erlaubt Ihnen die meisten Einstellungen von Mumble zu ändern.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;What&apos;s This?</source>
        <translation>&amp;Was ist das?</translation>
    </message>
    <message>
        <location/>
        <source>Enter What&apos;s This? mode</source>
        <translation>Was ist das?-Modus betreten</translation>
    </message>
    <message>
        <location/>
        <source>Click this to enter &quot;What&apos;s This?&quot; mode. Your cursor will turn into a question mark. Click on any button, menu choice or area to show a description of what it is.</source>
        <translation>Dies anklicken um den &quot;Was ist das?&quot;-Modus zu aktivieren. Der Cursor wird sich in ein Fragezeichen ändern. Klickt man damit auf irgendeinen Button, Menüeintrag oder ein Gebiet, wird eine kurze Beschreibung angezeigt.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;About</source>
        <translation>Ü&amp;ber</translation>
    </message>
    <message>
        <location/>
        <source>Information about Mumble</source>
        <translation>Informationen über Mumble</translation>
    </message>
    <message>
        <location/>
        <source>Shows a small dialog with information and license for Mumble.</source>
        <translation>Zeigt einen kleinen Dialog mit Informationen über Mumble und zur Lizenzierung.</translation>
    </message>
    <message>
        <location/>
        <source>About &amp;Speex</source>
        <translation>Über &amp;Speex</translation>
    </message>
    <message>
        <location/>
        <source>Information about Speex</source>
        <translation>Informationen über Speex</translation>
    </message>
    <message>
        <location/>
        <source>Shows a small dialog with information about Speex.</source>
        <translation>Zeigt einen kleinen Dialog mit Informationen über Speex.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Certificate Wizard</source>
        <translation>&amp;Zertifikats-Assistent</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Register</source>
        <translation>&amp;Registrieren</translation>
    </message>
    <message>
        <location/>
        <source>This will permanently register the user on the server.</source>
        <translatorcomment>permanent auslassen?</translatorcomment>
        <translation>Dies registriert den Benutzer permanent auf dem Server.</translation>
    </message>
    <message>
        <source>Change &amp;Texture</source>
        <translation type="obsolete">&amp;Textur ändern</translation>
    </message>
    <message>
        <source>&amp;Remove Texture</source>
        <translation type="obsolete">Textur entfe&amp;rnen</translation>
    </message>
    <message>
        <location/>
        <source>Reset &amp;Comment</source>
        <translation>&amp;Kommentar zurücksetzen</translation>
    </message>
    <message>
        <location/>
        <source>Reset the comment of the selected user.</source>
        <translation>Setzt den Kommentar des ausgewählten Benutzers zurück.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Join Channel</source>
        <translation>&amp;Kanal betreten</translation>
    </message>
    <message>
        <location/>
        <source>View Comment</source>
        <translation>Zeige Kommentar</translation>
    </message>
    <message>
        <location/>
        <source>View comment in editor</source>
        <translation>Zeige Kommentar im Editor</translation>
    </message>
    <message>
        <source>Remove Texture</source>
        <translation type="obsolete">Textur entfernen</translation>
    </message>
    <message>
        <source>Remove currently defined user texture.</source>
        <translation type="obsolete">Entfernt die aktuelle Benutzertextur.</translation>
    </message>
    <message>
        <source>This sends a text message to a channel and its subchannels.</source>
        <translation type="obsolete">Sendet eine Textnachricht an einen Kanal und alle Unterkanäle.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Change Comment</source>
        <translation>K&amp;ommentar ändern</translation>
    </message>
    <message>
        <source>Change the comment on the selected user.</source>
        <translation type="obsolete">Ändert den Kommentar des ausgewählten Benutzers.</translation>
    </message>
    <message>
        <source>This allows you to change the comment (shown as a tooltip) of a user. Unless you&apos;ve been given extra privileges on the server, you can only change your own comment.</source>
        <translation type="obsolete">Dies erlaubt es Ihnen den Kommentar (angezeigt als Tooltip) eines Benutzers zu ändern. Nur wenn Sie die notwendigen Rechte auf dem Server haben können Sie den Kommentar anderer Benutzer ändern, sonst nur Ihren eigenen.</translation>
    </message>
    <message>
        <source>Certificate Wizard</source>
        <translation type="obsolete">Zertifikats-Assistent</translation>
    </message>
    <message>
        <location/>
        <source>Configure certificates for strong authentication</source>
        <translation>Konfiguriert Zertifikate für die sichere Authentifikation</translation>
    </message>
    <message>
        <location/>
        <source>This starts the wizard for creating, importing and exporting certificates for authentication against servers.</source>
        <translation>Dies startet einen Assistenten zum Erstellen, Importieren und Exportieren von Zertifikaten welche zum authentifizieren gegenüber Servern verwendet werden.</translation>
    </message>
    <message>
        <source>Register</source>
        <translation type="obsolete">Registrieren</translation>
    </message>
    <message>
        <location/>
        <source>Register user on server</source>
        <translation>Registriert den Benutzer auf dem Server</translation>
    </message>
    <message>
        <source>This will permanently register the user on the server. Note that you can&apos;t change the username later on, nor can you easily unregister a user, so make sure you really want to do this.</source>
        <translation type="obsolete">Dies registriert den Benutzer permanent auf dem Server. Beachten Sie, dass Sie den Benutzernamen später nicht mehr ändern können und den Benutzer nicht einfach löschen können. Gehen Sie also sicher, dass Sie dies möchten.</translation>
    </message>
    <message>
        <location/>
        <source>Add &amp;Friend</source>
        <translation>&amp;Freund hinzufügen</translation>
    </message>
    <message>
        <location/>
        <source>Adds a user as your friend.</source>
        <translation>Fügt einen Benutzer als Freund hinzu.</translation>
    </message>
    <message>
        <location/>
        <source>This will add the user as a friend, so you can recognize him on this and other servers.</source>
        <translation>Dies fügt den Benutzer als Freund hinzu, so dass Sie ihn sowohl auf diesem, wie auch auf anderen Servern wieder erkennen.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Remove Friend</source>
        <translation>Freund entfe&amp;rnen</translation>
    </message>
    <message>
        <location/>
        <source>Removes a user from your friends.</source>
        <translation>Entfernt einen Benutzer aus der Freundesliste.</translation>
    </message>
    <message>
        <location/>
        <source>This will remove a user from your friends list.</source>
        <translation>Entfernt einen Benutzer aus der Freundesliste.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Update Friend</source>
        <translation>Fre&amp;und aktualisieren</translation>
    </message>
    <message>
        <location/>
        <source>Update name of your friend.</source>
        <translation>Aktualisiere den Namen des Freundes.</translation>
    </message>
    <message>
        <location/>
        <source>Your friend uses a different name than what is in your database. This will update the name.</source>
        <translation>Ihr Freund verwendet einen anderen Namen als jenen, der in Ihrer Datenbank gespeichert ist. Dies aktualisiert den Namen.</translation>
    </message>
    <message>
        <location/>
        <source>Registered &amp;Users</source>
        <translation>Registrierte Ben&amp;utzer</translation>
    </message>
    <message>
        <location/>
        <source>Edit registered users list</source>
        <translation>Bearbeitet die Liste der registrierten Benutzer</translation>
    </message>
    <message>
        <location/>
        <source>This opens the editor for registered users, which allow you to change their name or unregister them.</source>
        <translation>Dies öffnet den Editor für registrierte Benutzer, welcher es Ihnen erlaubt deren Namen zu ändern und Sie zu löschen.</translation>
    </message>
    <message>
        <source>Change Texture</source>
        <translation type="obsolete">Textur ändern</translation>
    </message>
    <message>
        <source>Change your overlay texture on this server</source>
        <translation type="obsolete">Ändert die Overlay-Textur auf diesem Server</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Access Tokens</source>
        <translation>&amp;Zugriffscodes</translation>
    </message>
    <message>
        <location/>
        <source>Add or remove text-based access tokens</source>
        <translation>Text-basierte Zugriffscodes (Passwörter) hinzufügen und entfernen</translation>
    </message>
    <message>
        <source>&amp;About Qt</source>
        <translation type="obsolete">Über &amp;Qt</translation>
    </message>
    <message>
        <location/>
        <source>Information about Qt</source>
        <translation>Informationen über Qt</translation>
    </message>
    <message>
        <location/>
        <source>Shows a small dialog with information about Qt.</source>
        <translation>Zeigt einen kleinen Dialog mit Informationen über Qt.</translation>
    </message>
    <message>
        <location/>
        <source>Check for &amp;Updates</source>
        <translation>Auf &amp;Updates prüfen</translation>
    </message>
    <message>
        <location/>
        <source>Check for new version of Mumble</source>
        <translation>Prüfen, ob eine neue Version von Mumble verfügbar ist</translation>
    </message>
    <message>
        <location/>
        <source>Connects to the Mumble webpage to check if a new version is available, and notifies you with an appropriate download URL if this is the case.</source>
        <translation>Verbindet sich mit der Mumble-Webseite um zu überprüfen ob eine neue Version verfügbar ist. Ist eine neue Version verfügbar wird mit einer passenden Download-URL darauf hingewiesen.</translation>
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
        <translation type="obsolete">Log mit Nachrichten</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>This shows all recent activity. Connecting to servers, errors and information messages all show up here.&lt;br /&gt;To configure exactly which messages show up here, use the &lt;b&gt;Settings&lt;/b&gt; command from the menu.</source>
        <translation>Dieser Bereich zeigt die letzten Aktivitäten an. Verbindungen zum Server, Fehler und Informationsnachrichten werden hier angezeigt.&lt;br /&gt;Um genau festzulegen welche Nachrichten hier angezeigt werden verwenden Sie den &lt;b&gt;Einstellungen&lt;/b&gt;-Menüeintrag und öffnen die Registerkarte &lt;b&gt;Nachrichten&lt;/b&gt;.</translation>
    </message>
    <message>
        <source>&amp;Server</source>
        <translation type="obsolete">&amp;Server</translation>
    </message>
    <message>
        <source>&amp;Player</source>
        <translation type="obsolete">B&amp;enutzer</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="-2246"/>
        <source>&amp;Channel</source>
        <translation>&amp;Kanal</translation>
    </message>
    <message>
        <source>&amp;Audio</source>
        <translation type="obsolete">&amp;Audio</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>C&amp;onfigure</source>
        <translation>K&amp;onfiguration</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Help</source>
        <translation>&amp;Hilfe</translation>
    </message>
    <message>
        <source>Kicking player %1</source>
        <translation type="obsolete">Kicke Benutzer %1</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="+1156"/>
        <location line="+19"/>
        <source>Enter reason</source>
        <translation>Grund eingeben</translation>
    </message>
    <message>
        <source>Banning player %1</source>
        <translation type="obsolete">Banne Benutzer %1</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>Mumble</source>
        <translation>Mumble</translation>
    </message>
    <message>
        <source>Channel Name</source>
        <translation type="obsolete">Kanalname</translation>
    </message>
    <message>
        <source>Are you sure you want to delete %1 and all it&apos;s subchannels?</source>
        <translation type="obsolete">Sind Sie sicher Kanal %1 und all seine Unterkanäle zu löschen?</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="+482"/>
        <source>Unmuted and undeafened.</source>
        <translation>Stumm- und Taubstellen deaktiviert.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Unmuted.</source>
        <translation>Stummstellen deaktiviert.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Muted.</source>
        <translation>Stumm.</translation>
    </message>
    <message>
        <location line="+31"/>
        <source>Muted and deafened.</source>
        <translation>Stumm und taub gestellt.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Deafened.</source>
        <translation>Taub gestellt.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Undeafened.</source>
        <translation>Taubstellen deaktiviert.</translation>
    </message>
    <message>
        <location line="+94"/>
        <source>About Qt</source>
        <translation>Über Qt</translation>
    </message>
    <message>
        <source>Joining %1.</source>
        <translation type="obsolete">Betrete %1.</translation>
    </message>
    <message>
        <source>Connected to server.</source>
        <translation type="obsolete">Mit Server verbunden.</translation>
    </message>
    <message>
        <location line="+420"/>
        <source>Server connection failed: %1.</source>
        <translation>Serververbindung fehlgeschlagen: %1.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Disconnected from server.</source>
        <translation>Verbindung zum Server unterbrochen.</translation>
    </message>
    <message>
        <location line="-1492"/>
        <source>Reconnecting.</source>
        <translation>Neuverbindung.</translation>
    </message>
    <message>
        <source>Joined server: %1.</source>
        <translation type="obsolete">Server beigetreten: %1.</translation>
    </message>
    <message>
        <source>the server</source>
        <translation type="obsolete">den Server</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="+323"/>
        <source>You were unmuted and undeafened by %1.</source>
        <translation>Stumm-/Taubstellen wurde deaktiviert durch %1.</translation>
    </message>
    <message>
        <location line="+29"/>
        <source>You muted and deafened %1.</source>
        <translation>Sie haben %1 stumm und taub gestellt.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>You unmuted and undeafened %1.</source>
        <translation>Sie haben Taub-/Stummstellen von %1 deaktiviert.</translation>
    </message>
    <message>
        <location line="+11"/>
        <source>You undeafened %1.</source>
        <translation>Sie haben die Taubstellung von %1 deaktiviert.</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>You suppressed %1.</source>
        <translation>Sie haben die Sprachunterdrückung von %1 aktiviert.</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>%1 muted and deafened by %2.</source>
        <translation>%1 wurde stumm und taub gestellt durch %2.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1 unmuted and undeafened by %2.</source>
        <translation>Taub-/Stummstellen von %1 wurden durch %2 deaktiviert.</translation>
    </message>
    <message>
        <location line="+18"/>
        <source>%1 suppressed by %2.</source>
        <translation>Sprachunterdrückung von %1 aktiviert durch %2.</translation>
    </message>
    <message>
        <location line="+25"/>
        <source>%1 moved to %2.</source>
        <translation>%1 wechselte zum Kanal %2.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1 moved to %2 by %3.</source>
        <translation>%1 verschoben nach %2 durch %3.</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>%1 moved in from %2 by %3.</source>
        <translation>%1 hinein verschoben aus %2 durch %3.</translation>
    </message>
    <message>
        <source>Left server: %1.</source>
        <translation type="obsolete">Server verlassen: %1.</translation>
    </message>
    <message>
        <location line="-157"/>
        <source>%1 is now muted and deafened.</source>
        <translation>%1 hat sich stumm und taub gestellt.</translation>
    </message>
    <message>
        <location line="-114"/>
        <source>You were denied %1 privileges in %2.</source>
        <translation>Ihnen wurden die Rechte %1 in %2 verweigert.</translation>
    </message>
    <message>
        <location line="-69"/>
        <location line="+48"/>
        <source>Welcome message: %1</source>
        <translation>Willkommensnachricht: %1</translation>
    </message>
    <message>
        <location line="-15"/>
        <source>Mumble: %1</source>
        <translation>Mumble: %1</translation>
    </message>
    <message>
        <location line="+38"/>
        <source>%3 was denied %1 privileges in %2.</source>
        <translation>%3 wurden die Rechte %1 in %2 verweigert.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Denied: Cannot modify SuperUser.</source>
        <translation>Abgelehnt: SuperUser kann nicht geändert werden.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Denied: Invalid channel name.</source>
        <translation>Abgelehnt: Ungültiger Kanalname.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Denied: Text message too long.</source>
        <translation>Abgelehnt: Text-Nachricht zu lang.</translation>
    </message>
    <message>
        <location line="+25"/>
        <source>Denied: Operation not permitted in temporary channel.</source>
        <translation>Abgelehnt: Operation in temporärem Kanal nicht erlaubt.</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>You need a certificate to perform this operation.</source>
        <translation>Sie benötigen ein Zertifikat um diese Operation auszuführen.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1 does not have a certificate.</source>
        <translation>%1 hat kein Zertifikat.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Invalid username: %1.</source>
        <translation>Ungültiger Benutzername: %1.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Invalid username.</source>
        <translation>Ungültiger Benutzername.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Channel is full.</source>
        <translation>Der Kanal ist voll.</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Permission denied.</source>
        <translation>Zugriff verweigert.</translation>
    </message>
    <message>
        <location line="+37"/>
        <source>%1 connected.</source>
        <translation>%1 beigetreten.</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>%1 is now muted.</source>
        <translation>%1 hat sich stumm gestellt.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1 is now unmuted.</source>
        <translation>%1 ist jetzt nicht mehr stumm gestellt.</translation>
    </message>
    <message>
        <location line="+11"/>
        <source>Recording started</source>
        <translation>Aufnahme gestartet</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Recording stopped</source>
        <translation>Aufnahme beendet</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>%1 started recording.</source>
        <translation>%1 nimmt jetzt auf.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1 stopped recording.</source>
        <translation>%1 nimmt nicht mehr auf.</translation>
    </message>
    <message>
        <location line="+41"/>
        <source>You were unsuppressed.</source>
        <translation>Ihre Sprachunterdrückung wurde deaktiviert.</translation>
    </message>
    <message>
        <location line="+96"/>
        <source>%1 is recording</source>
        <translation>%1 nimmt auf</translation>
    </message>
    <message>
        <location line="+46"/>
        <source>%1 disconnected.</source>
        <translation>%1 hat den Server verlassen.</translation>
    </message>
    <message>
        <location line="+91"/>
        <source>(Tree) </source>
        <translation>(Baum) </translation>
    </message>
    <message>
        <location line="+2"/>
        <source>(Channel) </source>
        <translation>(Kanal) </translation>
    </message>
    <message>
        <location line="+3"/>
        <source>%2%1: %3</source>
        <translation>%2%1: %3</translation>
    </message>
    <message>
        <source>server</source>
        <translation type="obsolete">Server</translation>
    </message>
    <message>
        <location line="-254"/>
        <source>You were muted by %1.</source>
        <translation>Stummstellung wurde aktiviert durch %1.</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>You were suppressed.</source>
        <oldsource>You were suppressed by %1.</oldsource>
        <translation>Sprachunterdrückung wurde aktiviert.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>You were unsuppressed by %1.</source>
        <translation>Ihre Sprachunterdrückung wurde deaktiviert durch %1.</translation>
    </message>
    <message>
        <location line="-16"/>
        <source>You were unmuted by %1.</source>
        <translation>Stummstellung wurde aufgehoben durch %1.</translation>
    </message>
    <message>
        <source>You deafened %1.</source>
        <translation type="obsolete">Sie haben %1 stumm und taubgestellt.</translation>
    </message>
    <message>
        <location line="+29"/>
        <source>You muted %1.</source>
        <translation>Sie haben %1 stumm gestellt.</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>You unsuppressed %1.</source>
        <translation>Sprachunterdrückung von %1 deaktiviert.</translation>
    </message>
    <message>
        <location line="-14"/>
        <source>You unmuted %1.</source>
        <translation>Stummstellung von %1 aufgehoben.</translation>
    </message>
    <message>
        <location line="+25"/>
        <source>%1 muted by %2.</source>
        <translation>%1 durch %2 stumm gestellt.</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>%1 unsuppressed by %2.</source>
        <translation>Die Sprachunterdrückung von %1 wurde durch %2 deaktiviert.</translation>
    </message>
    <message>
        <location line="-14"/>
        <source>%1 unmuted by %2.</source>
        <translation>Stummstellen von %1 wurde durch %2 deaktiviert.</translation>
    </message>
    <message>
        <source>the server</source>
        <comment>message from</comment>
        <translation type="obsolete">Server</translation>
    </message>
    <message>
        <location line="+338"/>
        <source>Unable to find matching CELT codecs with other clients. You will not be able to talk to all users.</source>
        <translation>Es wurde kein für alle Benutzer passender CELT-Codec gefunden. Sie werden nicht zu allen Benutzern sprechen können.</translation>
    </message>
    <message>
        <location line="-404"/>
        <source>You were muted and deafened by %1.</source>
        <oldsource>You were deafened by %1.</oldsource>
        <translation>Sie wurden taub gestellt durch %1.</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>You were undeafened by %1.</source>
        <translation>Taubstellen wurde deaktiviert durch %1.</translation>
    </message>
    <message>
        <source>%1 deafened by %2.</source>
        <translation type="obsolete">%1 wurde taub gestellt durch %2.</translation>
    </message>
    <message>
        <location line="+58"/>
        <source>%1 undeafened by %2.</source>
        <translation>Taubstellen von %1 wurde durch %2 deaktiviert.</translation>
    </message>
    <message>
        <location line="+86"/>
        <source>You were kicked from the server by %1: %2.</source>
        <translation>Sie wurden vom Server gekickt durch %1: %2.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>%3 was kicked from the server by %1: %2.</source>
        <translation>%3 wurde vom Server gekickt durch %1: %2.</translation>
    </message>
    <message>
        <location line="-7"/>
        <source>You were kicked and banned from the server by %1: %2.</source>
        <translation>Sie wurden vom Server gekickt und gebannt durch %1: %2.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>%3 was kicked and banned from the server by %1: %2.</source>
        <translation>%3 wurde vom Server gekickt und gebannt durch %1: %2.</translation>
    </message>
    <message>
        <location line="-60"/>
        <source>You were moved to %1 by %2.</source>
        <translation>Sie wurden nach %1 verschoben durch %2.</translation>
    </message>
    <message>
        <source>%1 left channel.</source>
        <translation type="obsolete">%1 hat den Kanal verlassen.</translation>
    </message>
    <message>
        <source>%1 moved out by %2 to %3.</source>
        <translation type="obsolete">%1 wurde durch %2 aus ihrem Kanal nach %3 verschoben.</translation>
    </message>
    <message>
        <source>%1 moved in by %2 from %3.</source>
        <translation type="obsolete">%1 wurde durch %2 von %3 in ihren Kanal verschoben.</translation>
    </message>
    <message>
        <source>%1 moved out by %2.</source>
        <translation type="obsolete">%1 wurde von %2 rausgeschoben.</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>%1 entered channel.</source>
        <translation>%1 betrat den Kanal.</translation>
    </message>
    <message>
        <source>%1 moved in by %2.</source>
        <translation type="obsolete">%1 wurde von %2 reingeschoben.</translation>
    </message>
    <message>
        <location line="-346"/>
        <source>Server connection rejected: %1.</source>
        <translation>Serververbindung abgelehnt : %1.</translation>
    </message>
    <message>
        <location line="+141"/>
        <source>Denied: %1.</source>
        <translation>Abgelehnt: %1.</translation>
    </message>
    <message>
        <location filename="main.cpp" line="-52"/>
        <source>Welcome to Mumble.</source>
        <translation>Willkommen in Mumble.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="-690"/>
        <source>Push-to-Talk</source>
        <comment>Global Shortcut</comment>
        <translation>Push-To-Talk</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Push and hold this button to send voice.</source>
        <comment>Global Shortcut</comment>
        <translation>Drücken und halten Sie diese Taste um Sprache zu senden.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This configures the push-to-talk button, and as long as you hold this button down, you will transmit voice.</source>
        <comment>Global Shortcut</comment>
        <translation>Dies konfiguriert die Push-To-Talk-Taste. So lange Sie diese Taste gedrückt halten, übermitteln Sie Sprache.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Reset Audio Processor</source>
        <comment>Global Shortcut</comment>
        <translation>Audio-Prozessor zurücksetzen</translation>
    </message>
    <message>
        <source>Toggle Mute Self</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">Selbst stumm stellen umschalten</translation>
    </message>
    <message>
        <source>Toggle self-mute status.</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">Wechsle &apos;selbst stumm gestellt&apos; Status.</translation>
    </message>
    <message>
        <source>This will toggle your muted status. If you toggle this off, you will also disable self-deafen.</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">Dies wechselt Ihren &apos;selbst stumm gestellt&apos; Status. Wenn Sie diesen deaktivieren, deaktivieren Sie auch &apos;selbst taub gestellt&apos;.</translation>
    </message>
    <message>
        <source>Toggle Deafen Self</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">Selbst taub stellen umschalten</translation>
    </message>
    <message>
        <source>Toggle self-deafen status.</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">Wechsle &apos;selbst taub gestellt&apos; Status.</translation>
    </message>
    <message>
        <source>This will toggle your deafen status. If you toggle this on, you will also enable self-mute.</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">Dies wechselt Ihren &apos;selbst taub gestellt&apos; Status. Wenn Sie diesen aktivieren, aktivieren Sie damit auch &apos;selbst stumm gestellt&apos;.</translation>
    </message>
    <message>
        <source>Force Center Position</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">Erzwinge Mittelposition</translation>
    </message>
    <message>
        <source>Chan Parent</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">Elternkanal</translation>
    </message>
    <message>
        <source>Chan Sub#%1</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">Unterkanal #%1</translation>
    </message>
    <message>
        <source>Chan All Subs</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">Alle Unterkanäle</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Push-to-Mute</source>
        <comment>Global Shortcut</comment>
        <translatorcomment>Sollte nicht uebersetzt werden, da entweder zu lang oder unverstaendlich</translatorcomment>
        <translation>Push-to-Mute</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Join Channel</source>
        <comment>Global Shortcut</comment>
        <translation>Kanal betreten</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Toggle Overlay</source>
        <comment>Global Shortcut</comment>
        <translation>Overlay-Modus umschalten</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Toggle state of in-game overlay.</source>
        <comment>Global Shortcut</comment>
        <translation>Wechsle den Modus des in-game Overlays.</translation>
    </message>
    <message>
        <source>This will switch the states of the ingame overlay between showing everybody, just the players who are talking, and nobody.</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">Dies wechselt den in-game Overlay Modus zwischen &apos;zeige Jeden&apos;, &apos;nur Sprechende&apos; und &apos;Niemanden&apos;.</translation>
    </message>
    <message>
        <source>Alt Push-to-Talk</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">Alt Push-To-Talk</translation>
    </message>
    <message>
        <location line="-11"/>
        <source>Unlink Plugin</source>
        <comment>Global Shortcut</comment>
        <translation>Plugin trennen</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>This will switch the states of the in-game overlay.</source>
        <comment>Global Shortcut</comment>
        <translation>Dies wechselt den Modus des Ingame-Overlays.</translation>
    </message>
    <message>
        <location line="+399"/>
        <source>File is not a configuration file.</source>
        <translation>Die Datei ist keine Konfigurationsdatei.</translation>
    </message>
    <message>
        <location line="+31"/>
        <source>This version of Mumble can&apos;t handle URLs for Mumble version %1.%2.%3</source>
        <translation>Diese Version von Mumble kann keine URLs von Mumble Version %1.%2.%3 verarbeiten</translation>
    </message>
    <message>
        <location line="+43"/>
        <location line="+176"/>
        <source>Connecting to server %1.</source>
        <translation>Zu Server %1 verbinden.</translation>
    </message>
    <message>
        <location line="+142"/>
        <source>&lt;h2&gt;Version&lt;/h2&gt;&lt;p&gt;Protocol %1.%2.%3.&lt;/p&gt;</source>
        <translation>&lt;h2&gt;Version&lt;/h2&gt;&lt;p&gt;Protokoll %1.%2.%3.&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>&lt;p&gt;No build information or OS version available.&lt;/p&gt;</source>
        <translation>&lt;p&gt;Keine Build- oder Betriebssystem-Informationen verfügbar.&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>&lt;p&gt;%1 (%2)&lt;br /&gt;%3&lt;/p&gt;</source>
        <translation>&lt;p&gt;%1 (%2)&lt;br /&gt;%3&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+1579"/>
        <source>Images (*.png *.jpg *.jpeg *.svg)</source>
        <translation>Bilder (*.png *.jpg *.jpeg *.svg)</translation>
    </message>
    <message>
        <source>&lt;h2&gt;Control channel&lt;/h2&gt;&lt;p&gt;Encrypted with %1 bit %2&lt;br /&gt;%3 ms average latency (%4 deviation)&lt;/p&gt;</source>
        <translation type="obsolete">&lt;h2&gt;Kontrollkanal&lt;/h2&gt;&lt;p&gt;Verschlüsselt mit %1 bit %2&lt;br /&gt;%3 ms Durchschnittslatenz (%4 Abweichung)&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="-1559"/>
        <source>&lt;h2&gt;Voice channel&lt;/h2&gt;&lt;p&gt;Encrypted with 128 bit OCB-AES128&lt;br /&gt;%1 ms average latency (%4 deviation)&lt;/p&gt;</source>
        <translation>&lt;h2&gt;Sprachkanal&lt;/h2&gt;&lt;p&gt;Verschlüsselt mit 128 bit OCB-AES128&lt;br /&gt;%1 ms Durchschnittslatenz (%4 Abweichung)&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+11"/>
        <source>&lt;h2&gt;Audio bandwidth&lt;/h2&gt;&lt;p&gt;Maximum %1 kbit/s&lt;br /&gt;Current %2 kbit/s&lt;/p&gt;</source>
        <translation>&lt;h2&gt;Audio-Bandbreite&lt;/h2&gt;&lt;p&gt;Maximum %1 kbit/s&lt;br /&gt;Derzeit %2 kbit/s&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+347"/>
        <source>View comment on user %1</source>
        <translation>Zeige den Kommentar des Benutzers %1</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Are you sure you want to reset the comment of user %1?</source>
        <translation>Wollen Sie wirklich den Kommentar des Benutzers %1 löschen?</translation>
    </message>
    <message>
        <location line="+273"/>
        <source>To %1 (Tree): %2</source>
        <oldsource>(Tree) %1: %2</oldsource>
        <translation>An %1 (Baum): %2</translation>
    </message>
    <message>
        <location line="-318"/>
        <location line="+84"/>
        <location line="+4"/>
        <location line="+232"/>
        <source>To %1: %2</source>
        <oldsource>%1: %2</oldsource>
        <translation>An %1: %2</translation>
    </message>
    <message>
        <location line="+675"/>
        <source>Failed to launch compatibility client</source>
        <translation>Starten des Kompatibilitätsclienten fehlgeschlagen</translation>
    </message>
    <message>
        <source>Images (*.png *.jpg)</source>
        <translation type="obsolete">Bilddateien (*.png *.jpg)</translation>
    </message>
    <message>
        <source>Could not open file.</source>
        <translation type="obsolete">Konnte Datei nicht öffnen.</translation>
    </message>
    <message>
        <source>Failed to open image file.</source>
        <translation type="obsolete">Öffnen der Bilddatei fehlgeschlagen.</translation>
    </message>
    <message>
        <location line="-1417"/>
        <location line="+327"/>
        <source>Register yourself as %1</source>
        <translation>Sich selbst als %1 registrieren</translation>
    </message>
    <message>
        <location line="-327"/>
        <location line="+327"/>
        <source>&lt;p&gt;You are about to register yourself on this server. This action cannot be undone, and your username cannot be changed once this is done. You will forever be known as &apos;%1&apos; on this server.&lt;/p&gt;&lt;p&gt;Are you sure you want to register yourself?&lt;/p&gt;</source>
        <translation>&lt;p&gt;Sie sind dabei sich auf dem Server zu registrieren. Diese Aktion kann nicht rückgängig gemacht werden und Ihr Benutzername &apos;%1&apos; auf diesem Server kann von Ihnen selbst nicht mehr geändert werden.&lt;/p&gt;&lt;p&gt;Sind Sie sicher, dass Sie sich selbst registrieren möchten?&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Register user %1</source>
        <translation>Benutzer %1 registrieren</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>&lt;p&gt;You are about to register %1 on the server. This action cannot be undone, the username cannot be changed, and as a registered user, %1 will have access to the server even if you change the server password.&lt;/p&gt;&lt;p&gt;From this point on, %1 will be authenticated with the certificate currently in use.&lt;/p&gt;&lt;p&gt;Are you sure you want to register %1?&lt;/p&gt;</source>
        <translation>&lt;p&gt;Sie sind dabei %1 auf dem Server zu registrieren. Diese Aktion kann nicht rückgängig gemacht werden und %1 wird sich, auch wenn Sie Ihr Serverpasswort ändern, weiterhin mit dem Server verbinden können.&lt;/p&gt;
&lt;p&gt;Von diesem Punkt an wird %1 mit dem Zertifikat welches er momentan verwendet, authentifiziert.&lt;/p&gt;
&lt;p&gt;Sind Sie sicher, dass Sie %1 registrieren möchten?&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+40"/>
        <source>Kicking user %1</source>
        <translation>Benutzer %1 kicken</translation>
    </message>
    <message>
        <location line="+19"/>
        <source>Banning user %1</source>
        <translation>Benutzer %1 bannen</translation>
    </message>
    <message>
        <source>Change comment on user %1</source>
        <translation type="obsolete">Ändert den Kommentar für Benutzer %1</translation>
    </message>
    <message>
        <source>To channel %1: %2</source>
        <translation type="obsolete">An den Kanal %1: %2</translation>
    </message>
    <message>
        <location line="+118"/>
        <location line="+236"/>
        <source>Message to channel %1</source>
        <translation>Nachricht an den Kanal %1</translation>
    </message>
    <message>
        <location line="-82"/>
        <source>Are you sure you want to delete %1 and all its sub-channels?</source>
        <translation>Sind Sie sicher, dass Sie %1 und alle Unterkanäle löschen möchten?</translation>
    </message>
    <message>
        <location line="+572"/>
        <source>Connected.</source>
        <translation>Verbunden.</translation>
    </message>
    <message>
        <location line="+159"/>
        <source>SSL Version mismatch</source>
        <translatorcomment>please improve… “mismatch”?</translatorcomment>
        <translation>SSL Versions Diskrepanz</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>This server is using an older encryption standard. It might be an older 1.1 based Mumble server.&lt;br /&gt;Would you like to launch the compatibility client to connect to it?</source>
        <translation>Dieser Server verwendet einen älteren Verschlüsselungsstandard. Es könnte ein älterer 1.1 basierter Mumble Server sein.&lt;br/&gt;
Möchten Sie die Kompatibilitätsversion starten und zu ihm verbinden?</translation>
    </message>
    <message>
        <source>Failed to launch compatibilty client</source>
        <translation type="obsolete">Konnte die Kompatibilitätsversion nicht starten</translation>
    </message>
    <message>
        <location line="+26"/>
        <source>The compatibility client could not be found, or failed to start.&lt;br /&gt;Note that the compatibility client is an optional component for most installations, and might not be installed.</source>
        <translation>Die Kompatibilitätsversion konnte nicht gefunden oder nicht gestartet werden.&lt;br/&gt;
Beachten Sie, dass die Kompatibilitätsversion eine optionale Komponente bei den meisten Installationen ist und möglicherweise nicht mit installiert wurde.</translation>
    </message>
    <message>
        <location line="+127"/>
        <source>&lt;center&gt;Type message to channel &apos;%1&apos; here&lt;/center&gt;</source>
        <oldsource>Type message to channel &apos;%1&apos; here</oldsource>
        <translation>&lt;center&gt;Geben Sie hier Ihre Nachricht an den Kanal &apos;%1&apos; ein&lt;/center&gt;</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>&lt;center&gt;Type message to user &apos;%1&apos; here&lt;/center&gt;</source>
        <oldsource>Type message to user &apos;%1&apos; here</oldsource>
        <translation>&lt;center&gt;Nachricht an den Benutzer &apos;%1&apos; hier eingeben&lt;/center&gt;</translation>
    </message>
    <message>
        <location line="+105"/>
        <source>Choose image file</source>
        <translation>Bilddatei wählen</translation>
    </message>
    <message>
        <source>Images (*.png *.jpg *.svg)</source>
        <translation type="obsolete">Bilder (*.png *.jpg *.svg)</translation>
    </message>
    <message>
        <location line="+7"/>
        <location line="+17"/>
        <source>Failed to load image</source>
        <translation>Fehler beim Laden des Bildes</translation>
    </message>
    <message>
        <location line="-17"/>
        <source>Could not open file for reading.</source>
        <translation>Konnte Datei nicht zum Lesen öffnen.</translation>
    </message>
    <message>
        <location line="+17"/>
        <source>Image format not recognized.</source>
        <translation>Bildformat wurde nicht erkannt.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>&amp;Quit Mumble</source>
        <translation>Mumble B&amp;eenden</translation>
    </message>
    <message>
        <location/>
        <source>Closes the program</source>
        <translation>Schließt das Programm</translation>
    </message>
    <message>
        <location/>
        <source>Exits the application.</source>
        <translation>Beendet die Applikation.</translation>
    </message>
    <message>
        <source>Ctrl+Q</source>
        <comment>Quit</comment>
        <translation type="obsolete">Ctrl+Q</translation>
    </message>
    <message>
        <source>Send Messa&amp;ge</source>
        <translation type="obsolete">Sende Nachri&amp;cht</translation>
    </message>
    <message>
        <location/>
        <source>Send a Text Message</source>
        <translation>Sende eine Text-Nachricht</translation>
    </message>
    <message>
        <source>Send a text message to another user.</source>
        <translation type="obsolete">Sende eine Text-Nachricht an einen anderen Benutzer.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="-1266"/>
        <source>Sending message to %1</source>
        <translation>Sende Nachricht an %1</translation>
    </message>
    <message>
        <location line="+12"/>
        <location line="+88"/>
        <source>Message to %1</source>
        <translation>Nachricht an %1</translation>
    </message>
    <message>
        <source>Change description of channel %1</source>
        <translation type="obsolete">Ändere Beschreibung von Kanal %1</translation>
    </message>
    <message>
        <location line="+928"/>
        <source>Invalid username</source>
        <translation>Ungültiger Benutzername</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>You connected with an invalid username, please try another one.</source>
        <translation>Sie verwenden einen ungültigen Benutzernamen. Bitte versuchen Sie einen anderen.</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>That username is already in use, please try another username.</source>
        <translation>Dieser Benutzername ist bereits vergeben. Bitte benutzen Sie einen anderen.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Wrong password</source>
        <translation>Falsches Passwort</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Wrong password for registered users, please try again.</source>
        <translation>Falsches Passwort für den registrierten Benutzer. Bitte versuchen Sie es noch einmal.</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Wrong server password for unregistered user account, please try again.</source>
        <translation>Falsches Serverpasswort für unregistrierte Benutzer. Bitte versuchen Sie es noch einmal.</translation>
    </message>
    <message>
        <source>From %1: %2</source>
        <translation type="obsolete">Von %1: %2</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="+353"/>
        <source>Message from %1</source>
        <translation>Nachricht von %1</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>&amp;Audio Wizard</source>
        <translation>&amp;Audio-Assistent</translation>
    </message>
    <message>
        <location/>
        <source>Sends a text message to another user.</source>
        <translation>Schickt eine Textnachricht an einen anderen Benutzer.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="-2231"/>
        <source>&amp;User</source>
        <translation>&amp;Benutzer</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>This is the chatbar&lt;br /&gt;If you enter text here and then press enter the text is sent to the user or channel that was selected. If nothing is selected the message is sent to your current channel.</source>
        <translation>Dies ist die Chatbar&lt;br /&gt;
Wenn Sie hier Text eingeben und Enter drücken wird der Text an den Benutzer oder Kanal gesendet, welcher ausgewählt ist. Sofern nichts selektiert ist wird die Nachricht an den aktuellen Kanal gesendet.</translation>
    </message>
    <message>
        <location/>
        <source>Chatbar</source>
        <translation>Chatbar</translation>
    </message>
    <message>
        <location/>
        <source>Shows a dialog of registered servers, and also allows quick-connect.</source>
        <translation>Zeigt ein Fenster mit registrierten Servern und erlaubt es Ihnen schnell zu diesen zu verbinden.</translation>
    </message>
    <message>
        <location/>
        <source>Kick user (with reason)</source>
        <translation>Benutzer kicken (mit Begründung)</translation>
    </message>
    <message>
        <location/>
        <source>Kick selected user off server. You&apos;ll be asked to specify a reason.</source>
        <translation>Den ausgewählten Benutzer vom Server kicken. Sie werden nach einer Begründung gefragt.</translation>
    </message>
    <message>
        <location/>
        <source>Mute user</source>
        <translation>Benutzer stumm stellen</translation>
    </message>
    <message>
        <location/>
        <source>Mute or unmute user on server. Unmuting a deafened user will also undeafen them.</source>
        <translation>Benutzer auf dem Server stummstellen oder das stummstellen aufheben. Das aufheben auf einem taubgestellten Benutzer wird das taubstellen ebenfalls entfernen.</translation>
    </message>
    <message>
        <location/>
        <source>Kick and ban user (with reason)</source>
        <translation>Benutzer kicken und bannen (mit Begründung)</translation>
    </message>
    <message>
        <location/>
        <source>Kick and ban selected user from server. You&apos;ll be asked to specify a reason.</source>
        <translation>Den gewählten Benutzer vom Server kicken und bannen. Sie werden nach einem Grund gefragt werden.</translation>
    </message>
    <message>
        <location/>
        <source>Deafen user</source>
        <translation>Benutzer taub stellen</translation>
    </message>
    <message>
        <location/>
        <source>Deafen or undeafen user on server. Deafening a user will also mute them.</source>
        <translation>Den Benutzer auf dem Server taubstellen oder das taubstellen aufheben. Beim Taubstellen eines Benutzers wird dieser automatisch auch stummgestellt.</translation>
    </message>
    <message>
        <location/>
        <source>Mute user locally</source>
        <translation>Benutzer lokal stumm stellen</translation>
    </message>
    <message>
        <location/>
        <source>Mute or unmute user locally. Use this on other users in the same room.</source>
        <translation>Benutzer lokal stummstellen oder lokales Stummstellen aufheben. Verwenden Sie dies auf andere Benutzer im selben Raum.</translation>
    </message>
    <message>
        <location/>
        <source>Send &amp;Message</source>
        <translation>&amp;Nachricht senden</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Edit</source>
        <translation>B&amp;earbeiten</translation>
    </message>
    <message>
        <location/>
        <source>This links your current channel to the selected channel. If users in a channel have permission to speak in the other channel, users can now hear each other. This is a permanent link, and will last until manually unlinked or the server is restarted. Please see the shortcuts for push-to-link.</source>
        <oldsource>This links your current channel to the selected channel. If they have permission to speak in the other channel, users can now hear each other. This is a permanent link, and will last until manually unlinked or the server is restarted. Please see the shortcuts for push-to-link.</oldsource>
        <translation>Dies verbindet den aktuellen Kanal mit dem Gewählten. Wenn Benutzer Spracherlaubnis auch im anderen Kanal haben, so können sie sich jetzt, mit jenen aus dem anderen Kanal, gegenseitig hören. Dies ist eine permanente Verbindung die so lange bestehen bleibt, bis die Verknüpfung manuell aufgehoben oder der Server neu gestartet wird.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Unlink All</source>
        <translation>&amp;Alle Verknüpfungen entfernen</translation>
    </message>
    <message>
        <location/>
        <source>Start the audio configuration wizard</source>
        <translation>Startet den Audiokonfigurations-Assistenten</translation>
    </message>
    <message>
        <location/>
        <source>This will guide you through the process of configuring your audio hardware.</source>
        <translation>Dies leitet Sie durch den Hardware-Konfigurationsprozess.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="+2141"/>
        <source>SSL Verification failed: %1</source>
        <translation>SSL-Überprüfung fehlgeschlagen: %1</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>&lt;b&gt;WARNING:&lt;/b&gt; The server presented a certificate that was different from the stored one.</source>
        <translation>&lt;b&gt;WARNUNG:&lt;/b&gt; Der Server hat ein Zertifikat gesendet, das sich vom für diesen Server gespeicherten Zertifikat unterscheidet.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Sever presented a certificate which failed verification.</source>
        <translation>Der Server hat Zertifikat gesendet, dessen Überprüfung gescheitert ist.</translation>
    </message>
    <message>
        <source>&lt;li&gt;&lt;b&gt;Common Name:&lt;/b&gt; %1&lt;/li&gt;</source>
        <translation type="obsolete">&lt;li&gt;&lt;b&gt;Allgemeiner Name:&lt;/b&gt; %1&lt;/li&gt;</translation>
    </message>
    <message>
        <source>&lt;li&gt;&lt;b&gt;Organization:&lt;/b&gt; %1&lt;/li&gt;</source>
        <translation type="obsolete">&lt;li&gt;&lt;b&gt;Organisation:&lt;/b&gt; %1&lt;/li&gt;</translation>
    </message>
    <message>
        <source>&lt;li&gt;&lt;b&gt;Subunit:&lt;/b&gt; %1&lt;/li&gt;</source>
        <translation type="obsolete">&lt;li&gt;&lt;b&gt;Untereinheit:&lt;/b&gt; %1&lt;/li&gt;</translation>
    </message>
    <message>
        <source>&lt;li&gt;&lt;b&gt;Country:&lt;/b&gt; %1&lt;/li&gt;</source>
        <translation type="obsolete">&lt;li&gt;&lt;b&gt;Land:&lt;/b&gt; %1&lt;/li&gt;</translation>
    </message>
    <message>
        <source>&lt;li&gt;&lt;b&gt;Locality:&lt;/b&gt; %1&lt;/li&gt;</source>
        <translation type="obsolete">&lt;li&gt;&lt;b&gt;Lokalität:&lt;/b&gt; %1&lt;/li&gt;</translation>
    </message>
    <message>
        <source>&lt;li&gt;&lt;b&gt;State:&lt;/b&gt; %1&lt;/li&gt;</source>
        <translation type="obsolete">&lt;li&gt;&lt;b&gt;Bundesstaat:&lt;/b&gt; %1&lt;/li&gt;</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>&lt;p&gt;%1.&lt;br /&gt;The specific errors with this certificate are: &lt;/p&gt;&lt;ol&gt;%2&lt;/ol&gt;&lt;p&gt;Do you wish to accept this certificate anyway?&lt;br /&gt;(It will also be stored so you won&apos;t be asked this again.)&lt;/p&gt;</source>
        <translation>&lt;p&gt;%1.&lt;br /&gt;Die Fehler dieses Zertifikats sind: &lt;/p&gt;&lt;ol&gt;%2&lt;/ol&gt;&lt;p&gt;Wollen Sie dieses Zertifikat trotzdem akzeptieren?&lt;br /&gt;(Es wird auch abgespeichert, weshalb Sie nicht wieder gefragt werden.)&lt;/p&gt;</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>&amp;Information</source>
        <translation>&amp;Informationen</translation>
    </message>
    <message>
        <location/>
        <source>Show information about the server connection</source>
        <translation>Zeigt Informationen über die Serververbindung an</translation>
    </message>
    <message>
        <location/>
        <source>This will show extended information about the connection to the server.</source>
        <translation>Dies zeigt erweitertete Informationen über die Verbindung zum Server an.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="-1261"/>
        <source>Mumble Server Information</source>
        <translation>Mumble - Server Informationen</translation>
    </message>
    <message>
        <source>Control channel: %1 ms latency, Encrypted with %3 bit %4&lt;br /&gt;Voice channel: %2 ms latency, Encrypted with OCB-AES128</source>
        <translation type="obsolete">Kontrollkanal: %1 ms Latenz. Verschlüsselt mit %3 bit %4&lt;br /&gt;Sprachkanal: %2 ms Latenz, verschlüsselt mit OCB-AES128</translation>
    </message>
    <message>
        <location line="+4"/>
        <location line="+1264"/>
        <source>&amp;View Certificate</source>
        <translation>&amp;Zertifikat zeigen</translation>
    </message>
    <message>
        <location line="-1714"/>
        <source>Opening URL %1</source>
        <translation>Öffne URL %1</translation>
    </message>
    <message>
        <location line="-407"/>
        <source>Mute Self</source>
        <comment>Global Shortcut</comment>
        <translation>Selbst stumm stellen</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Set self-mute status.</source>
        <comment>Global Shortcut</comment>
        <translation>Selbst stumm stellen Status.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This will set or toggle your muted status. If you turn this off, you will also disable self-deafen.</source>
        <comment>Global Shortcut</comment>
        <translation>Dies setzt oder wechselt Ihren selbst-stummstellen Status. Wenn Sie dies deaktivieren werden Sie ebenfalls das selbst taubstellen deaktivieren.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Deafen Self</source>
        <comment>Global Shortcut</comment>
        <translation>Selbst taub stellen</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Set self-deafen status.</source>
        <comment>Global Shortcut</comment>
        <translation>Setze selbst-taub stellen Status.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This will set or toggle your deafened status. If you turn this on, you will also enable self-mute.</source>
        <comment>Global Shortcut</comment>
        <translation>Dies setzt oder wechselt Ihren selbst-taub stellen Status. Wenn Sie dies aktivieren werden Sie ebenfalls selbst-stumm gestellt.</translation>
    </message>
    <message>
        <source>This will switch the states of the in-game overlay between showing everybody, just the users who are talking, and nobody.</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">Dies wechselt den in-game Overlay Modus zwischen &apos;Zeige Jeden&apos;, &apos;nur Sprechende&apos; und &apos;Niemanden&apos;.</translation>
    </message>
    <message>
        <location line="+30"/>
        <source>Whisper/Shout</source>
        <oldsource>Whisper</oldsource>
        <translation>Flüstern/Rufen</translation>
    </message>
    <message>
        <location line="+58"/>
        <location line="+2189"/>
        <source>&lt;center&gt;Not connected&lt;/center&gt;</source>
        <oldsource>Not connected</oldsource>
        <translation>&lt;center&gt;Nicht verbunden&lt;/center&gt;</translation>
    </message>
    <message>
        <location line="-1874"/>
        <source>File does not exist</source>
        <translation>Datei existiert nicht</translation>
    </message>
    <message>
        <location line="+310"/>
        <source>Change your comment</source>
        <translation>Ändere deinen Kommentar</translation>
    </message>
    <message>
        <location line="+108"/>
        <source>&lt;h2&gt;Control channel&lt;/h2&gt;&lt;p&gt;Encrypted with %1 bit %2&lt;br /&gt;%3 ms average latency (%4 deviation)&lt;/p&gt;&lt;p&gt;Remote host %5 (port %6)&lt;/p&gt;</source>
        <translation>&lt;h2&gt;Steuerkanal&lt;/h2&gt;&lt;p&gt;Verschlüsselt mit %1 bit %2&lt;br /&gt;%3 ms Durchschnittslatenz (%4 Abweichung)&lt;/p&gt;&lt;p&gt;Gegenstelle %5 (Port %6)&lt;/p&gt;</translation>
    </message>
    <message>
        <source>File is not a config file.</source>
        <translation type="obsolete">Diese Datei ist keine Konfigurationsdatei.</translation>
    </message>
    <message>
        <location line="-402"/>
        <source>Settings merged from file.</source>
        <translation>Einstellungen aus Datei eingelesen.</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>URL scheme is not &apos;mumble&apos;</source>
        <translation>URL-Schema ist nicht &apos;mumble&apos;</translation>
    </message>
    <message>
        <location line="+48"/>
        <source>Connecting to %1</source>
        <translation>Verbinde zu %1</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Enter username</source>
        <translation>Benutzernamen eingeben</translation>
    </message>
    <message>
        <source>&amp;Rename channel</source>
        <translation type="obsolete">Kanal u&amp;mbenennen</translation>
    </message>
    <message>
        <source>Renames the channel</source>
        <translation type="obsolete">Benennt den Kanal um</translation>
    </message>
    <message>
        <source>This renames a channel.</source>
        <translation type="obsolete">Dies benennt einen Kanal um.</translation>
    </message>
    <message>
        <source>This is the first time you&apos;re starting Mumble.&lt;br /&gt;Would you like to go through the Audio Wizard to configure your soundcard?</source>
        <translation type="obsolete">Dies ist das erste Mal, dass Sie Mumble starten.&lt;br /&gt;Wollen Sie den Audio-Assistenten starten, um Ihre Soundkarte zu konfigurieren?</translation>
    </message>
    <message>
        <source>&lt;h2&gt;Control channel&lt;/h2&gt;&lt;p&gt;Encrypted with %1 bit %2&lt;br /&gt;%3 ms average latency (%4 variance)&lt;/p&gt;</source>
        <translation type="obsolete">&lt;h2&gt;Kontrollkanal&lt;/h2&gt;&lt;p&gt;Verschlüsselt mit %1 Bit %2&lt;br /&gt;%3 ms Durchschnittslatenz (%4 Varianz)&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+357"/>
        <source>Voice channel is sent over control channel.</source>
        <translation>Sprachkanal wird über den Kontrollkanal gesendet.</translation>
    </message>
    <message>
        <source>&lt;h2&gt;Voice channel&lt;/h2&gt;&lt;p&gt;Encrypted with 128 bit OCB-AES128&lt;br /&gt;%1 ms average latency (%4 variance)&lt;/p&gt;</source>
        <translation type="obsolete">&lt;h2&gt;Sprachkanal&lt;/h2&gt;&lt;p&gt;Verschlüsselt mit 128 Bit OCB-AES128&lt;br /&gt;%1 ms Durchschnittslatenz (%4 Varianz)&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>UDP Statistics</source>
        <translation>UDP-Statistiken</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>To Server</source>
        <translation>Zum Server</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>From Server</source>
        <translation>Vom Server</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Good</source>
        <translation>Gut</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Late</source>
        <translation>Verspätet</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Lost</source>
        <translation>Verloren</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Resync</source>
        <translation>Neu synchronisieren</translation>
    </message>
    <message>
        <source>You are now in local deafen mode. This mode is not reflected on the server, and you will still be transmitting voice to the server. This mode should only be used if there are several people in the same room and one of them have Mumble on loudspeakers.</source>
        <translation type="obsolete">Sie befinden sich jetzt im lokalen Taubmodus. Dieser Modus stimmt nicht mit dem des Servers überein und ihre Stimme wird weiterhin zum Server übertragen. Dieser Modus sollte nur genutzt werden, wenn sich mehrere Leute im selben Raum befinden und einer von ihnen benutzt Lautsprecher.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>Ctrl+Q</source>
        <translation>Strg+Q</translation>
    </message>
    <message>
        <source>Mute player locally</source>
        <translation type="obsolete">Benutzer lokal stummstellen</translation>
    </message>
    <message>
        <source>Mute or unmute player locally. Use this on other players in the same room.</source>
        <translation type="obsolete">Benutzer lokal stummstellen. Benutzen Sie dies wenn sich andere Spieler im selben Raum befinden.</translation>
    </message>
    <message>
        <source>&amp;Rename Channel</source>
        <translation type="obsolete">Kanal &amp;umbenennen</translation>
    </message>
    <message>
        <source>&amp;Local deafen</source>
        <translation type="obsolete">&amp;Lokal stummstellen</translation>
    </message>
    <message>
        <source>Silence loudspeakers</source>
        <translation type="obsolete">Lautsprecher leise stellen</translation>
    </message>
    <message>
        <source>Sets you in local deafen mode, wherein your speakers are silenced but you are still transmitting audio.</source>
        <translation type="obsolete">Versetzt Sie in den lokale Taubmodus in welchem Ihre Lautsprecher leise gestellt sind, aber immer noch Audio übertragen wird.</translation>
    </message>
    <message>
        <location/>
        <source>About &amp;Qt</source>
        <translation>Über &amp;Qt</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="+631"/>
        <source>Sending message to channel %1</source>
        <translation>Sende Nachricht an den Kanal %1</translation>
    </message>
    <message>
        <source>Sending message to channel tree %1</source>
        <translation type="obsolete">Sende Nachricht an Kanalbaum %1</translation>
    </message>
    <message>
        <source>To tree %1: %2</source>
        <translation type="obsolete">An Baum %1: %2</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Message to tree %1</source>
        <translation>Nachricht an Baum %1</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>Sends a text message to all users in a channel.</source>
        <translation>Schickt eine Textnachricht an alle Benutzer im Kanal.</translation>
    </message>
    <message>
        <source>Send &amp;Tree Message</source>
        <translation type="obsolete">Sende &amp;Baumnachricht</translation>
    </message>
    <message>
        <source>Send message to channel tree.</source>
        <translation type="obsolete">Sende Nachricht an Kanalbaum (Kanal und Unterkanäle).</translation>
    </message>
    <message>
        <source>This sends a text message to a channel and it&apos;s subchannels.</source>
        <translation type="obsolete">Dies schickt eine Textnachricht an einen Kanal und seine Unterkanäle.</translation>
    </message>
    <message>
        <source>Server maximum bandwidth is only %1 kbit/s. Quality auto-adjusted.</source>
        <translation type="obsolete">Maximale Bandbreite des Servers ist nur %1 kbit/s. Die Qualität wurde automatisch angepasst.</translation>
    </message>
    <message>
        <source>&lt;h2&gt;Audio bandwidth&lt;/h2&gt;&lt;p&gt;Maximum %1 kbit/s&lt;br /&gt;Current %2 kbit/s (Quality %3)&lt;/p&gt;</source>
        <translation type="obsolete">&lt;h2&gt;Audio Bandbreite&lt;/h2&gt;&lt;p&gt;Maximum %1 kbit/s&lt;br /&gt;Aktuell %2 kbit/s (Qualität %3)&lt;/p&gt;</translation>
    </message>
    <message>
        <source>Connected to server %1.</source>
        <translation type="obsolete">Mit Server %1 verbunden.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="-1310"/>
        <source>Mumble is currently connected to a server. Do you want to Close or Minimize it?</source>
        <translation>Mumble ist gerade zu einem Server verbunden. Möchten Sie es schließen oder minimieren?</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Close</source>
        <translation>Schließen</translation>
    </message>
    <message>
        <location line="-125"/>
        <location line="+126"/>
        <source>Minimize</source>
        <translation>Minimieren</translation>
    </message>
    <message>
        <location line="-128"/>
        <source>&amp;Window</source>
        <translation>&amp;Fenster</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Ctrl+M</source>
        <translation>Strg+M</translation>
    </message>
    <message>
        <source>Ctrl+W</source>
        <translation type="obsolete">Strg+W</translation>
    </message>
    <message>
        <location line="-28"/>
        <source>Toggle Minimal</source>
        <comment>Global Shortcut</comment>
        <translation>Minimalansichts-Modus umschalten</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>&amp;Minimal View</source>
        <translation>&amp;Minimalansicht</translation>
    </message>
    <message>
        <location/>
        <source>Toggle minimal window modes</source>
        <translation>Zwischen Minimalansichts-Modi umschalten</translation>
    </message>
    <message>
        <location/>
        <source>This will toggle minimal mode, where the log window and menu is hidden.</source>
        <translation>Dies schaltet in den Minimal-Modus um, in dem das Logfenster und das Menu versteckt sind.</translation>
    </message>
    <message>
        <location/>
        <source>Log</source>
        <translation>Log</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="+3"/>
        <source>Volume Up (+10%)</source>
        <comment>Global Shortcut</comment>
        <translation>Lauter (+10%)</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Volume Down (-10%)</source>
        <comment>Global Shortcut</comment>
        <translation>Leiser (-10%)</translation>
    </message>
    <message>
        <location line="+349"/>
        <source>Clear</source>
        <translation>Löschen</translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="+1381"/>
        <source>You have Channel Dragging set to &quot;Do Nothing&quot; so the channel wasn&apos;t moved.</source>
        <translation>Die Kanal-Ziehen-Aktion wurde auf &quot;Nichts tun&quot; gesetzt. Der Kanal wird also nicht bewegt.</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Unknown Channel Drag mode in UserModel::dropMimeData.</source>
        <translation>Unbekannter Kanal-Verschiebe-Modus in UserModel::dropMimeData.</translation>
    </message>
    <message>
        <source>Unknown Channel Drag mode in PlayerModel::dropMimeData.</source>
        <translation type="obsolete">Unbekannter Kanalzieh-Modus in PlayerModel:dropMimeData.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>&amp;Unlink</source>
        <comment>Channel</comment>
        <translation>&amp;Verknüpfung entfernen</translation>
    </message>
    <message>
        <source>&amp;Unlink</source>
        <comment>Plugin</comment>
        <translation type="obsolete">&amp;Deaktivieren</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="-10"/>
        <location line="+1"/>
        <source>Server</source>
        <comment>message from</comment>
        <translation>Server</translation>
    </message>
    <message>
        <source>&amp;Quit</source>
        <translation type="obsolete">&amp;Beenden</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>Hide Frame</source>
        <translation>Fenster verstecken</translation>
    </message>
    <message>
        <location/>
        <source>Toggle showing frame on minimal window</source>
        <translation>Anzeige des Rahmens im Minimal-Modus wechseln</translation>
    </message>
    <message>
        <location/>
        <source>This will toggle whether the minimal window should have a frame for moving and resizing.</source>
        <translation>Dies wechselt ob das Minimal-Fenster einen Rahmen zum verschieben und vergrößern hat oder nicht.</translation>
    </message>
    <message>
        <location/>
        <source>Query server for connection information for user</source>
        <translation>Zeige Informationen über den Benutzer an</translation>
    </message>
    <message>
        <location/>
        <source>S&amp;erver</source>
        <translation>S&amp;erver</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Self</source>
        <translation>&amp;Selbst</translation>
    </message>
    <message>
        <location/>
        <source>Audio S&amp;tatistics</source>
        <translation>Audios&amp;tatistik</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Unlink Plugins</source>
        <translation>Pl&amp;ugins trennen</translation>
    </message>
    <message>
        <source>Change your own comment.</source>
        <translation type="obsolete">Ändere eigenen Kommentar.</translation>
    </message>
    <message>
        <location/>
        <source>R&amp;egister</source>
        <translation>R&amp;egistrieren</translation>
    </message>
    <message>
        <location/>
        <source>Register yourself on the server</source>
        <translation>Sich selbst auf dem Server registrieren</translation>
    </message>
    <message>
        <location/>
        <source>Change &amp;Avatar</source>
        <translation>&amp;Avatar ändern</translation>
    </message>
    <message>
        <location/>
        <source>Change your avatar image on this server</source>
        <translation>Avatarbild auf diesem Server ändern</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Remove Avatar</source>
        <translation>Avatar entfe&amp;rnen</translation>
    </message>
    <message>
        <location/>
        <source>Remove currently defined avatar image.</source>
        <translation>Aktuell verwendetes Avatarbild entfernen.</translation>
    </message>
    <message>
        <location/>
        <source>Icon Toolbar</source>
        <translation>Icon Toolbar</translation>
    </message>
    <message>
        <location/>
        <source>Change your own comment</source>
        <translation>Ändert Ihren eigenen Kommentar</translation>
    </message>
    <message>
        <source>Priority speaker</source>
        <translation type="obsolete">Bevorzugter Sprecher</translation>
    </message>
    <message>
        <location/>
        <source>Recording</source>
        <translation>Aufnahme</translation>
    </message>
    <message>
        <location/>
        <source>Priority Speaker</source>
        <translation>Bevorzugter Sprecher</translation>
    </message>
</context>
<context>
    <name>NetworkConfig</name>
    <message>
        <location filename="NetworkConfig.cpp" line="+53"/>
        <source>Network</source>
        <translation>Netzwerk</translation>
    </message>
    <message>
        <location line="+36"/>
        <source>Updates are mandatory when using snapshot releases.</source>
        <translation>Aktualisierungen sind verpflichtend wenn Sie Snapshots verwenden.</translation>
    </message>
    <message>
        <source>Form</source>
        <translation type="obsolete">Formular</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui"/>
        <source>Connection</source>
        <translation>Verbindung</translation>
    </message>
    <message>
        <location/>
        <source>Use TCP compatibility mode</source>
        <translation>Benutze TCP-Kompatibilitätsmodus</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Enable TCP compatibility mode&lt;/b&gt;.&lt;br /&gt;This will make Mumble use only TCP when communicating with the server. This will increase overhead and cause lost packets to produce noticeable pauses in communication, so this should only be used if you are unable to use the default (which uses UDP for voice and TCP for control).</source>
        <translation>&lt;b&gt;TCP-Kompatibilitätsmodus aktivieren&lt;/b&gt;.&lt;br /&gt;Dies lässt Mumble nur TCP zur Kommunikation mit dem Server nutzen. Dies wird den Overhead erhöhen und bei verlorenen Paketen hörbare Pausen produzieren. Sie sollten diese Einstellung also nur verwenden, wenn die Standardeinstellungen nicht funktionieren.</translation>
    </message>
    <message>
        <location/>
        <source>Force TCP mode</source>
        <oldsource>Use TCP mode</oldsource>
        <translation>Erzwinge TCP-Modus</translation>
    </message>
    <message>
        <location/>
        <source>Enable QoS to prioritize packets</source>
        <translation>Aktiviere QoS um Pakete zu priorisieren</translation>
    </message>
    <message>
        <location/>
        <source>This will enable QoS, which will attempt to prioritize voice packets over other traffic.</source>
        <translation>Dies aktiviert QoS, welches versucht Audio-Pakete höher zu priorisieren als anderen Netzwerkverkehr.</translation>
    </message>
    <message>
        <location/>
        <source>Use Quality of Service</source>
        <translation>Verwende Quality of Service (QoS)</translation>
    </message>
    <message>
        <location/>
        <source>Reconnect when disconnected</source>
        <translation>Nach Verbindungsabbruch erneut verbinden</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Reconnect when disconnected&lt;/b&gt;.&lt;br /&gt;This will make Mumble try to automatically reconnect after 10 seconds if your server connection fails.</source>
        <translation>&lt;b&gt;Neuverbinden nach Verbindungsabbruch&lt;/b&gt;.&lt;br /&gt;Ist dieses Feld aktiviert führt Mumble beim Abbruch der Serververbindung eine automatische Neuverbindung nach 10 Sekunden durch.</translation>
    </message>
    <message>
        <location/>
        <source>Reconnect automatically</source>
        <translation>Automatisch Neuverbinden</translation>
    </message>
    <message>
        <location/>
        <source>Don&apos;t send certificate to server and don&apos;t save passwords. (Not saved).</source>
        <translation>Schicke kein Zertifikat zum Server und speichere keine Passwörter (Wird nicht gespeichert).</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This will suppress identity information from the client.&lt;/b&gt;&lt;p&gt;The client will not identify itself with a certificate, even if defined, and will not cache passwords for connections. This is primarily a test-option and is not saved.&lt;/p&gt;</source>
        <translation>&lt;b&gt;Dies verhindert das senden von Identitäts-Informationen vom Client.&lt;/b&gt;
&lt;p&gt;Der Client wird sich nicht mit einem Zertifikat identifizieren, selbst wenn der Zugriff dann verweigert wird, und wird keine Passwörter für Verbindungen zwischenspeichern. Dies ist hauptsächlich eine Test-Option und wird daher nicht gespeichert.&lt;/p&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Suppress certificate and password storage</source>
        <translation>Zertifikats- und Passwort-Speicherung unterdrücken</translation>
    </message>
    <message>
        <location/>
        <source>Proxy</source>
        <translation>Proxy</translation>
    </message>
    <message>
        <location/>
        <source>Type</source>
        <translation>Typ</translation>
    </message>
    <message>
        <location/>
        <source>Type of proxy to connect through</source>
        <translation>Art des Proxys durch den verbunden werden soll</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Type of proxy to connect through.&lt;/b&gt;&lt;br /&gt;This makes Mumble connect through a proxy for all outgoing connections. Note: Proxy tunneling forces Mumble into TCP compatibility mode, causing all voice data to be sent via the control channel.</source>
        <translation>&lt;b&gt;Proxytyp über den verbunden wird.&lt;/b&gt;&lt;br /&gt;Dies lässt Mumble für alle ausgehenden Verbindungen über einen Proxy verbinden. Beachten Sie: Proxy tunneling zwingt Mumble in den TCP-Kompatibilitätsmodus was zur Folge hat, dass alle Sprach-Daten über den Kontrollkanal gesendet werden.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Hostname of the proxy.&lt;/b&gt;&lt;br /&gt;This field specifies the hostname of the proxy you wish to tunnel network traffic through.</source>
        <translation>&lt;b&gt;Hostname des Proxys.&lt;/b&gt;&lt;br/&gt;
Dieses Feld legt den Hostnamen des Proxys, durch welchen Sie die Netzwerk Daten versenden möchten, fest.</translation>
    </message>
    <message>
        <location/>
        <source>Check for new releases of Mumble automatically.</source>
        <translation>Überprüfe automatisch, ob eine neue Mumble Version verfügbar ist.</translation>
    </message>
    <message>
        <location/>
        <source>This will check for new releases of Mumble every time you start the program, and notify you if one is available.</source>
        <translation>Dies wird bei jedem Start des Programms auf neue Mumble Versionen prüfen und Sie, sofern es welche gibt, darüber informieren.</translation>
    </message>
    <message>
        <location/>
        <source>Check for application updates on startup</source>
        <translation>Überprüfe beim Start ob Updates verfügbar sind</translation>
    </message>
    <message>
        <location/>
        <source>Check for new releases of plugins automatically.</source>
        <translation>Überprüfe automatisch, ob aktualisierte Plugins verfügbar sind.</translation>
    </message>
    <message>
        <location/>
        <source>This will check for new releases of plugins every time you start the program, and download them automatically.</source>
        <translation>Dies veranlasst Mumble bei jedem Programmstart auf neue Plugin-Versionen zu prüfen und diese gegebenenfalls automatisch herunterzuladen.</translation>
    </message>
    <message>
        <source>Download plugin updates on startup</source>
        <translation type="obsolete">Plugin Updates beim Starten herunterladen</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Submit anonymous statistics.&lt;/b&gt;&lt;br /&gt;Mumble has a small development team, and as such needs to focus its development where it is needed most. By submitting a bit of statistics you help the project determine where to focus development.</source>
        <translation>&lt;b&gt;Anonyme Statistiken senden&lt;/b&gt;&lt;br/&gt;
Mumble hat ein kleines Entwickler-Team. Deshalb muss die verfügbare Zeit auf die wichtigsten Dinge konzentriert werden. Indem Sie einige anonymisierte Daten senden, helfen Sie beim festlegen der Entwicklungsprioritäten.</translation>
    </message>
    <message>
        <source>&lt;b&gt;Type of proxy to connect through&lt;/b&gt;&lt;br /&gt;This makes Mumble connect through a proxy for all outgoing connections. Note: Proxy tunneling forces Mumble into TCP compatibility mode, causing all voice data to be sent via the control channel.</source>
        <translation type="obsolete">&lt;b&gt;Art des Proxys durch den verbunden werden soll&lt;/b&gt;&lt;br /&gt;Dies lässt Mumble alle ausgehenden Verbindungen durch einen Proxy gehen. Beachten Sie, dass Proxytunneling Mumble in den TCP Kompatibilitätsmodus zwingt, was alle Sprachdaten über den Kontrollkanal sendet.</translation>
    </message>
    <message>
        <location/>
        <source>Direct connection</source>
        <translation>Direkte Verbindung</translation>
    </message>
    <message>
        <location/>
        <source>HTTP(S) proxy</source>
        <translation>HTTP(S) Proxy</translation>
    </message>
    <message>
        <location/>
        <source>SOCKS5 proxy</source>
        <translation>SOCKS5 Proxy</translation>
    </message>
    <message>
        <location/>
        <source>Hostname</source>
        <translation>Hostname</translation>
    </message>
    <message>
        <location/>
        <source>Hostname of the proxy</source>
        <translation>Hostname des Proxys</translation>
    </message>
    <message>
        <source>&lt;b&gt;Hostname of the proxy.&lt;b&gt;&lt;br /&gt;This field specifies the hostname of the proxy you wish to tunnel network traffic through.</source>
        <translation type="obsolete">&lt;b&gt;Hostname des Proxys.&lt;/b&gt;&lt;br /&gt;Dieses Feld spezifiert den Hostnamen des Proxys durch den Netzwerkverkehr getunnelt werden soll.</translation>
    </message>
    <message>
        <location/>
        <source>Port</source>
        <translation>Port</translation>
    </message>
    <message>
        <location/>
        <source>Port number of the proxy</source>
        <translation>Portnummer des Proxys</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Port number of the proxy.&lt;/b&gt;&lt;br /&gt;This field specifies the port number that the proxy expects connections on.</source>
        <translation>&lt;b&gt;Portnummer des Proxys.&lt;/b&gt;&lt;br /&gt;Dieses Feld spezifiert die Portnummer auf der der Proxy eine Verbindung erwartet.</translation>
    </message>
    <message>
        <location/>
        <source>Username</source>
        <translation>Benutzername</translation>
    </message>
    <message>
        <location/>
        <source>Username for proxy authentication</source>
        <translation>Benutzername für Proxy-Authentisierung</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Username for proxy authentication.&lt;/b&gt;&lt;br /&gt;This specifies the username you use for authenticating yourself with the proxy. In case the proxy does not use authentication, or you want to connect anonymously, simply leave this field blank.</source>
        <translation>&lt;b&gt;Benutzername für Proxy-Authentisierung.&lt;/b&gt;&lt;br /&gt;Dieses Feld spezifiert den Benutzernamen mit dem Sie sich beim Proxy authentisieren. Falls der Proxy keine Authentisierung verwendet, oder Sie sich anonym verbinden wollen, lassen Sie das Feld frei.</translation>
    </message>
    <message>
        <location/>
        <source>Password</source>
        <translation>Passwort</translation>
    </message>
    <message>
        <location/>
        <source>Password for proxy authentication</source>
        <translation>Passwort für Proxy-Authentisierung</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Password for proxy authentication.&lt;/b&gt;&lt;br /&gt;This specifies the password you use for authenticating yourself with the proxy. In case the proxy does not use authentication, or you want to connect anonymously, simply leave this field blank.</source>
        <translation>&lt;b&gt;Passwort für Proxy-Authentisierung.&lt;/b&gt;&lt;br /&gt;Dieses Feld spezifiert das Passwort welches zur Authentisierung mit dem Proxy verwendet wird. Falls der Proxy keine Authentisierung verwendet oder Sie sich anonym verbinden wollen, lassen Sie das Feld frei.</translation>
    </message>
    <message>
        <location/>
        <source>Misc</source>
        <translation>Verschiedenes</translation>
    </message>
    <message>
        <location/>
        <source>Prevent log from downloading images</source>
        <translation>Eingebettete Bilddateien nicht herunterladen</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Disable image download&lt;/b&gt;&lt;br/&gt;
Prevents the client from downloading images embedded into chat messages with the img tag.</source>
        <oldsource>&lt;b&gt;Disable image download&lt;b&gt;&lt;br/&gt;
Prevents the client from downloading images embedded into chat messages with the img tag.</oldsource>
        <translation>&lt;b&gt;Herunterladen von Bilddateien deaktivieren&lt;/b&gt;&lt;br/&gt;
Verhindert das Herunterladen von Nachrichten die mittels img-Tag in Nachrichten eingebunden sind.</translation>
    </message>
    <message>
        <location/>
        <source>Disable image download</source>
        <translation>Eingebettete Bilddateien nicht herunterladen</translation>
    </message>
    <message>
        <location/>
        <source>Mumble services</source>
        <translation>Mumble Dienste</translation>
    </message>
    <message>
        <source>Check for updates on startup</source>
        <translation type="obsolete">Prüfe bei Start ob neue Updates verfügbar sind</translation>
    </message>
    <message>
        <location/>
        <source>Submit anonymous statistics to the Mumble project</source>
        <translation>Sende anonyme Statistiken an das Mumble Projekt</translation>
    </message>
    <message>
        <source>&lt;b&gt;Submit anonymous statistics.&lt;/b&gt;&lt;br /&gt;Mumble has a small development team, and as such needs to focus it&apos;s development where it&apos;s needed most. By submitting a bit of statistics you help the project determine where to focus development.</source>
        <translation type="obsolete">&lt;b&gt;Sende anonyme Statistiken.&lt;/b&gt;&lt;br /&gt;Mumble hat ein kleines Entwicklungsteam und als solches muss es sich auf die Entwicklungen konzentrieren, die am meisten benötigt werden. Durch das Senden von Statistiken helfen Sie dem Projekt festzustellen, wohin die Entwicklung führen soll.</translation>
    </message>
    <message>
        <location/>
        <source>Submit anonymous statistics</source>
        <translation>Sende anonyme Statistiken</translation>
    </message>
    <message>
        <location/>
        <source>Reconnect to last used server when starting Mumble</source>
        <translation>Beim Start zum zuletzt benutzten Server verbinden</translation>
    </message>
    <message>
        <location/>
        <source>Reconnect to last server on startup</source>
        <translation>Beim Start zum zuletzt benutzten Server verbinden</translation>
    </message>
    <message>
        <location/>
        <source>Download plugin and overlay updates on startup</source>
        <translation>Plugin und Overlay Updates beim Starten herunterladen</translation>
    </message>
</context>
<context>
    <name>OSSConfig</name>
    <message>
        <source>OSS</source>
        <translation type="obsolete">OSS</translation>
    </message>
    <message>
        <source>%1ms</source>
        <translation type="obsolete">%1ms</translation>
    </message>
    <message>
        <source>Form</source>
        <translation type="obsolete">Form</translation>
    </message>
    <message>
        <source>Device selection</source>
        <translation type="obsolete">Geräteauswahl</translation>
    </message>
    <message>
        <source>Input</source>
        <translation type="obsolete">Eingang</translation>
    </message>
    <message>
        <source>Device to use for microphone</source>
        <translation type="obsolete">Mikrofongerät</translation>
    </message>
    <message>
        <source>This set which device mumble should use. The &lt;i&gt;default&lt;/i&gt; device is whatever you have configured in alsaconfig, the &lt;i&gt;hwplug&lt;/i&gt; devices are specific hardware devices backed by the ALSA mixer and the &lt;i&gt;hw&lt;/i&gt; devices are raw hardware access. Unless your soundcard supports hardware mixing of audio, using the &lt;i&gt;hw&lt;/i&gt; device will exclude all other programs from using audio.</source>
        <translation type="obsolete">Dies legt fest, welches Gerät Mumble nutzen soll. Das &lt;i&gt;Standard&lt;/i&gt;-Gerät ist durch alsaconfig konfiguriert, die &lt;i&gt;hwplug&lt;/i&gt; Geräte sind spezielle Hardware-Geräte die vom ALSA-Mixer genutzt werden und die &lt;i&gt;hw&lt;/i&gt;-Geräte bieten rohen Hardwarezugriff. Falls ihre Soundkarte Hardware Mixing von Audio nicht unterstützt, wird die Verwendung von &lt;i&gt;hw&lt;/i&gt;-Geräten alle anderen Programme vom Audiozugriff ausschließen.</translation>
    </message>
    <message>
        <source>Output</source>
        <translation type="obsolete">Ausgabe</translation>
    </message>
    <message>
        <source>Device to use for speakers/headphones</source>
        <translation type="obsolete">Gerät für Lautsprecher/Kopfhörer</translation>
    </message>
    <message>
        <source>Output Options</source>
        <translation type="obsolete">Ausgabeoptionen</translation>
    </message>
    <message>
        <source>Output Delay</source>
        <translation type="obsolete">Ausgabeverzögerung</translation>
    </message>
    <message>
        <source>Amount of data to buffer for OSS</source>
        <translation type="obsolete">Menge an Daten die ALSA puffern soll</translation>
    </message>
    <message>
        <source>This sets the amount of data to prebuffer in the output buffer. Experiment with different values and set it to the lowest which doesn&apos;t cause rapid jitter in the sound.</source>
        <translation type="obsolete">Dies liegt die Menge an Daten fest, die im Ausgabepuffer vorgepuffert werden sollen. Experimentiere mit verschiedenen Werten und setze es auf den niedrigsten Wert, der noch kein schnelles Jitter im Sound verursacht.</translation>
    </message>
    <message>
        <source>TextLabel</source>
        <translation type="obsolete">Textlabel</translation>
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
        <translation>Still</translation>
    </message>
    <message>
        <location line="+3"/>
        <location filename="OverlayUser.cpp" line="+3"/>
        <source>Talking</source>
        <translation>Sprechen</translation>
    </message>
    <message>
        <location line="+3"/>
        <location filename="OverlayUser.cpp" line="+3"/>
        <source>Whisper</source>
        <translation>Flüstern</translation>
    </message>
    <message>
        <location line="+3"/>
        <location filename="OverlayUser.cpp" line="+3"/>
        <source>Shout</source>
        <translation>Rufen</translation>
    </message>
    <message>
        <location line="+17"/>
        <location filename="OverlayUser.cpp" line="-110"/>
        <source>Channel</source>
        <translation>Kanal</translation>
    </message>
    <message>
        <location filename="Overlay.cpp" line="+100"/>
        <source>Failed to create communication with overlay at %2: %1. No overlay will be available.</source>
        <translation>Kommunikation mit dem Overlay auf %2: %1 fehlgeschlagen. Das Overlay wird nicht verfügbar sein.</translation>
    </message>
    <message>
        <location line="+255"/>
        <source>Skipping overlay update in debug mode.</source>
        <translation>Überspringe Overlay-Aktualisierung im Debug Modus.</translation>
    </message>
    <message>
        <location line="+66"/>
        <source>Downloaded new or updated overlay support file to %1.</source>
        <translation>Neue oder aktualisierte Datei zur Overlayunterstützung heruntergeladen nach %1.</translation>
    </message>
    <message>
        <source>Failed to load overlay library. This means either that:
- the library (mumble_ol.dll) wasn&apos;t found in the directory you ran Mumble from
- you&apos;re on an OS earlier than WinXP SP2
- you do not have the June 2007 updated version of DX9.0c</source>
        <translation type="obsolete">Konnte Overlay-Bibliothek nicht laden. Dies bedeutet entweder, dass:
- die Bibliothek (mumble_ol.dll) nicht in dem Verzeichnis gefunden werden konnte, von dem aus Mumble gestartet wurde
- Sie ein Betriebssystem älter als WinXP SP2 nutze
- Sie nicht die Juni 2007 Version von DX9.0c installiert haben</translation>
    </message>
    <message>
        <source>Failed to load overlay library. This means either that:
- the library (mumble_ol.dll) wasn&apos;t found in the directory you ran Mumble from
- you&apos;re on an OS earlier than WinXP SP2</source>
        <translation type="obsolete">Das Laden der Overlay-Bibliothek ist fehlgeschlagen. Das heißt entweder dass:
- die Bibliothek (mumble_ol.dll) nicht in dem Verzeichnis gefunden wurde, aus welchem Sie Mumble gestartet haben
- Sie ein Betriebssystem vor WinXP SP2 benutzen</translation>
    </message>
    <message>
        <source>Failed to initialize overlay memory. This usually means that the shared memory is locked by the OS, and you need to reboot to release it.</source>
        <translation type="obsolete">Das Initialisieren des Overlay-Speichers ist fehlgeschlagen. Das heißt meistens, dass der Gemeinsame Speicher (shared memory) vom Betriebssystem gesperrt ist und Sie neu starten müssen um ihn freizugeben.</translation>
    </message>
</context>
<context>
    <name>OverlayClient</name>
    <message>
        <location filename="OverlayUserGroup.cpp" line="+94"/>
        <source>Filter</source>
        <translation>Filter</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Only talking</source>
        <translation>Nur sprechen</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>All in current channel</source>
        <translation>Alle im aktuellen Kanal</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>All in linked channels</source>
        <translation>Alle in verknüpften Kanälen</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Always show yourself</source>
        <translation>Sich selbst immer anzeigen</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Columns</source>
        <translation>Spalten</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Edit...</source>
        <translation>Bearbeiten...</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Reset Zoom</source>
        <translation>Zoom zurücksetzen</translation>
    </message>
    <message>
        <location filename="OverlayClient.cpp" line="+108"/>
        <source>FPS: %1</source>
        <translation>FPS: %1</translation>
    </message>
</context>
<context>
    <name>OverlayConfig</name>
    <message>
        <location filename="Overlay.ui"/>
        <source>Options</source>
        <translation>Optionen</translation>
    </message>
    <message>
        <source>Position</source>
        <translation type="obsolete">Position</translation>
    </message>
    <message>
        <source>Form</source>
        <oldsource>Font</oldsource>
        <translation type="obsolete">Schriftart</translation>
    </message>
    <message>
        <location/>
        <source>Enable Overlay</source>
        <translation>Overlay aktivieren</translation>
    </message>
    <message>
        <location/>
        <source>Enable overlay.</source>
        <translation>Overlay aktivieren.</translation>
    </message>
    <message>
        <source>This sets whether the overlay is enabled or not. This setting is only checked with D3D9 applications are started, so make sure Mumble is running and this option is on before you start the application.&lt;br /&gt;Please note that if you start the application after starting Mumble, or if you disable the overlay while running, there is no safe way to restart the overlay without also restarting the application.</source>
        <translation type="obsolete">Dies legt fest, ob das Overlay benutzt wird oder nicht. Diese Einstellung wird nur angezeigt wenn D3D9 Applikationen gestartet sind. Versichern Sie sich also dass Mumble läuft und diese Option aktiviert ist bevor Sie die Applikation starten.&lt;br /&gt;Bitte beachten Sie dass, falls Sie die Applikation nach Mumble starten, oder Sie das Overlay während des Betriebs deaktivieren, es keinen sicheren Weg gibt das Overlay neuzustarten ohne die Applikation neuzustarten.</translation>
    </message>
    <message>
        <source>No one</source>
        <translation type="obsolete">Niemand</translation>
    </message>
    <message>
        <source>Only talking</source>
        <translation type="obsolete">Nur sprechen</translation>
    </message>
    <message>
        <source>Everyone</source>
        <translation type="obsolete">Jeder</translation>
    </message>
    <message>
        <source>Show</source>
        <translation type="obsolete">Zeige</translation>
    </message>
    <message>
        <source>Who to show on the overlay</source>
        <translation type="obsolete">Wer auf dem Overlay angezeigt werden soll</translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets who to show in the in-game overlay.&lt;/b&gt;&lt;br /&gt;If many people are connected to the same channel, the overlay list might be very long. Use this to shorten it.&lt;br /&gt;&lt;i&gt;No one&lt;/i&gt; - Don&apos;t show anyone (but leave overlay running).&lt;br /&gt;&lt;i&gt;Only talking&lt;/i&gt; - Only show talking people.&lt;br /&gt;&lt;i&gt;Everyone&lt;/i&gt; - Show everyone.</source>
        <translation type="obsolete">&lt;b&gt;Dies legt fest, wer im Overlay angezeigt werden soll.&lt;/b&gt;&lt;br /&gt;Falls viele Spieler im selben Kanal sind, könnte die Overlayliste sehr lang werden. Benutzen Sie dies um sie zu kürzen.&lt;br /&gt;&lt;i&gt;Niemand&lt;/i&gt; - Zeige niemanden  (aber lass das Overlay laufen).&lt;br /&gt;&lt;i&gt;Nur Sprechende&lt;/i&gt; - Nur sprechende Benutzer zeigen.&lt;br /&gt;&lt;i&gt;Jeder&lt;/i&gt; - Zeige alle.</translation>
    </message>
    <message>
        <source>Always Show Self</source>
        <translation type="obsolete">Sich selbst immer anzeigen</translation>
    </message>
    <message>
        <source>Always show yourself on overlay.</source>
        <translation type="obsolete">Sich selbst immer auf dem Overlay anzeigen.</translation>
    </message>
    <message>
        <source>This sets whether to always show yourself or not. This setting is useful if you aren&apos;t showing everyone in the overlay, as then you would only see your own status if you were talking, which wouldn&apos;t let you see that you were deafened or muted.</source>
        <translation type="obsolete">Dies legt fest, ob Sie selbst immer angezeigt werden sollen oder nicht. Diese Einstellung ist nützlich, wenn Sie im Overlay nicht jeden anzeigen. In dem Fall würden sie ihren Status nur sehen, wenn Sie sprechen und sie könnten nicht sehen ob sie taub- oder stummgestellt wurden.</translation>
    </message>
    <message>
        <source>Show User Textures</source>
        <translation type="obsolete">Zeige Benutzertexturen</translation>
    </message>
    <message>
        <source>Show User custom textures instead of text on the overlay.</source>
        <translation type="obsolete">Zeige benutzerdefinierte Texturen statt Text auf dem Overlay.</translation>
    </message>
    <message>
        <source>This sets whether to download and use custom textures for registered users. If disabled, the regular outline text will be used instead.</source>
        <translation type="obsolete">Dies legt fest ob Benutzertexturen von registrierten Benutzern heruntergeladen und benutzt werden sollen. Wenn deaktiviert, wird normaler Text angezeigt.</translation>
    </message>
    <message>
        <source>Grow Left</source>
        <translation type="obsolete">Nach links wachsen</translation>
    </message>
    <message>
        <source>Let overlay grow to the left</source>
        <translation type="obsolete">Das Overlay nach links wachsen lassen</translation>
    </message>
    <message>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow to the left if needed.</source>
        <translation type="obsolete">Das Overlay versucht so klein wie möglich und an der Position die Sie gewählt haben zu bleiben. Dies erlaubt dem Overlay notfalls nach links zu wachsen.</translation>
    </message>
    <message>
        <source>Grow Right</source>
        <translation type="obsolete">Nach rechts wachsen</translation>
    </message>
    <message>
        <source>Grow Up</source>
        <translation type="obsolete">Nach oben wachsen</translation>
    </message>
    <message>
        <source>Let overlay grow upwards</source>
        <translation type="obsolete">Das Overlay nach oben wachsen lassen</translation>
    </message>
    <message>
        <source>Grow Down</source>
        <translation type="obsolete">Nach unten wachsen</translation>
    </message>
    <message>
        <source>Let overlay grow downwards</source>
        <translation type="obsolete">Das Overlay nach unten wachsen lassen</translation>
    </message>
    <message>
        <source>X-Position of Overlay</source>
        <translation type="obsolete">X-Position des Overlays</translation>
    </message>
    <message>
        <source>This sets the relative X position of the overlay.</source>
        <translation type="obsolete">Dies legt die relative X-Position des Overlays fest.</translation>
    </message>
    <message>
        <source>Y-Position of Overlay</source>
        <translation type="obsolete">Y-Position des Overlays</translation>
    </message>
    <message>
        <location/>
        <source>This sets whether the overlay is enabled or not. This setting is only checked when applications are started, so make sure Mumble is running and this option is on before you start the application.&lt;br /&gt;Please note that if you start Mumble after starting the application, or if you disable the overlay while the application is running, there is no safe way to restart the overlay without also restarting the application.</source>
        <oldsource>This sets whether the overlay is enabled or not. This setting is only checked when applications are started, so make sure Mumble is running and this option is on before you start the application.&lt;br /&gt;Please note that if you start the application after starting Mumble, or if you disable the overlay while running, there is no safe way to restart the overlay without also restarting the application.</oldsource>
        <translation>Dies legt fest ob das Overlay aktiv ist oder nicht. Diese Einstellung wird nur überprüft wenn Anwendungen gestartet werden, stellen Sie also sicher, dass Mumble läuft und die Option aktiviert ist, bevor Sie die andere Applikation, in welcher das Overlay erscheinen soll, starten.&lt;br/&gt;
Bitte beachten Sie: Wenn Sie Mumble nach dem Applikationsstart starten oder das Overlay deaktivieren während die Applikation läuft gibt es keinen sicheren Weg das Overlay wieder zu starten ohne die Applikation neu zu starten.</translation>
    </message>
    <message>
        <source>This sets the relative Y position of the overlay.</source>
        <translation type="obsolete">Dies legt die relative Y-Position des Overlay fest.</translation>
    </message>
    <message>
        <source>Current Font</source>
        <translation type="obsolete">Aktuelle Schriftart</translation>
    </message>
    <message>
        <source>Set Font</source>
        <translation type="obsolete">Setze Schriftart</translation>
    </message>
    <message>
        <source>Maximum height of names.</source>
        <translation type="obsolete">Maximale Höhe von Namen.</translation>
    </message>
    <message>
        <source>This sets the maximum height of names shown, relative to the screen height. If your active 3D window is 800 pixels tall and this is set to 5%, each name will be 40 pixels tall. Note that the names will not be taller than 60 pixels no matter what you set here.</source>
        <translation type="obsolete">Dies legt die maximale Höhe der Namen  (oder Benutzertexturen) relativ zur Bildschirmauflösung fest. Wenn Ihr aktives 3D-Fenster 800 Pixel hoch ist und Sie dies auf 5% setzen, wird jeder Name maximal 40 Pixel hoch. Namen werden eine Höhe von 60 Pixeln nicht überschreiten, egal was Sie hier festlegen.</translation>
    </message>
    <message>
        <source>Maximum height</source>
        <translation type="obsolete">Maximale Höhe</translation>
    </message>
    <message>
        <source>Change</source>
        <translation type="obsolete">Ändern</translation>
    </message>
    <message>
        <source>Color for players</source>
        <translation type="obsolete">Farbe für Benutzer</translation>
    </message>
    <message>
        <source>Color for talking players</source>
        <translation type="obsolete">Farbe für sprechende Benutzer</translation>
    </message>
    <message>
        <source>Color for alt-talking players</source>
        <translation type="obsolete">Farbe für alternativ-sprechende Benutzer</translation>
    </message>
    <message>
        <source>Color for Channels</source>
        <translation type="obsolete">Farbe für Kanäle</translation>
    </message>
    <message>
        <source>Color for active Channels</source>
        <translation type="obsolete">Farbe für aktive Kanäle</translation>
    </message>
    <message>
        <location filename="OverlayConfig.cpp" line="+78"/>
        <source>To move the users, drag the little red dot.</source>
        <translation>Ziehen Sie den kleinen roten Punkt um die Benutzer zu verschieben.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>To resize the users, mouse wheel over a user.</source>
        <oldsource>To resize the users, mousewheel over a user.</oldsource>
        <translation>Um die Größe der Benutzer zu ändern benutzen Sie die Scrollfunktion des Mausrads.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>For more options, right click a user.</source>
        <oldsource>For more options, rightclick a user.</oldsource>
        <translation>Für mehr Optionen klicken Sie mit der rechten Maustaste auf einen Benutzer.</translation>
    </message>
    <message>
        <location line="+30"/>
        <source>FPS: %1</source>
        <translation>FPS: %1</translation>
    </message>
    <message>
        <location line="+194"/>
        <source>Overlay</source>
        <translation>Overlay</translation>
    </message>
    <message>
        <location line="+75"/>
        <source>Choose executable</source>
        <translation>Anwendung auswählen</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Choose application</source>
        <translation>Anwendung auswählen</translation>
    </message>
    <message>
        <location line="+38"/>
        <source>Mumble has deemed the installer valid.</source>
        <translation>Mumble hat den Installer als gültig erachtet.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Mumble was unable to verify the authenticity of the installer.</source>
        <oldsource>Mumble was unable to validate authenticity of the installer.</oldsource>
        <translation>Mumble konnte die Echtheit des Installers nicht überprüfen.</translation>
    </message>
    <message>
        <location line="+87"/>
        <source>Load Overlay Presets</source>
        <translation>Lade Overlay Vorlage</translation>
    </message>
    <message>
        <location line="+2"/>
        <location line="+28"/>
        <source>Mumble overlay presets (*.mumblelay)</source>
        <translation>Mumble Overlay Vorlage (*.mumblelay)</translation>
    </message>
    <message>
        <location line="-2"/>
        <source>Save Overlay Presets</source>
        <translation>Overlay Vorlage abspeichern</translation>
    </message>
    <message>
        <source>Show no one</source>
        <translation type="obsolete">Zeige niemanden</translation>
    </message>
    <message>
        <source>Show only talking</source>
        <translation type="obsolete">Zeige nur Sprechende</translation>
    </message>
    <message>
        <source>Show everyone</source>
        <translation type="obsolete">Zeige Jeden</translation>
    </message>
    <message>
        <source>Color for users</source>
        <translation type="obsolete">Farbe für Benutzer</translation>
    </message>
    <message>
        <source>Color for talking users</source>
        <translation type="obsolete">Farbe für sprechende Benutzer</translation>
    </message>
    <message>
        <source>Color for whispering users</source>
        <translation type="obsolete">Farbe für flüsternde Benutzer</translation>
    </message>
    <message>
        <source>Color for channels</source>
        <translation type="obsolete">Farbe für Kanäle</translation>
    </message>
    <message>
        <source>Color for active channels</source>
        <translation type="obsolete">Farbe für aktive Kanäle</translation>
    </message>
    <message>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow upwards if needed.</source>
        <translation type="obsolete">Das Overlay versucht so klein wie möglich und an der Position die Sie gewählt haben zu bleiben. Dies erlaubt dem Overlay notfalls nach oben zu wachsen.</translation>
    </message>
    <message>
        <source>Let overlay grow to the right</source>
        <translation type="obsolete">Das Overlay nach rechts wachsen lassen</translation>
    </message>
    <message>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow to the right if needed.</source>
        <translation type="obsolete">Das Overlay versucht so klein wie möglich und an der Position die Sie gewählt haben zu bleiben. Dies erlaubt dem Overlay notfalls nach rechts zu wachsen.</translation>
    </message>
    <message>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow downwards if needed.</source>
        <translation type="obsolete">Das Overlay versucht so klein wie möglich und an der Position die Sie gewählt haben zu bleiben. Dies erlaubt dem Overlay notfalls nach unten zu wachsen.</translation>
    </message>
    <message>
        <source>TextLabel</source>
        <translation type="obsolete">Textlabel</translation>
    </message>
    <message>
        <location filename="Overlay.ui"/>
        <source>Layout</source>
        <translation>Anordnung</translation>
    </message>
    <message>
        <location/>
        <source>Overlay exceptions</source>
        <translation>Ausnahmeliste</translation>
    </message>
    <message>
        <location/>
        <source>Blacklist</source>
        <translation>Blacklist</translation>
    </message>
    <message>
        <location/>
        <source>Whitelist</source>
        <translation>Whitelist</translation>
    </message>
    <message>
        <source>Add</source>
        <translation type="obsolete">Hinzufügen</translation>
    </message>
    <message>
        <location/>
        <source>Remove</source>
        <translation>Entfernen</translation>
    </message>
    <message>
        <location/>
        <source>Uninstall Overlay</source>
        <translation>Overlay deinstallieren</translation>
    </message>
    <message>
        <location/>
        <source>Overlay Installation</source>
        <translation>Overlay Installation</translation>
    </message>
    <message>
        <location/>
        <source>Mumble has detected that you do not have the Mumble Overlay installed.

Click the button below to install the overlay.</source>
        <translation>Mumble hat erkannt, dass Sie kein Mumble Overlay installiert haben.

Klicken Sie den Button unten um das Overlay zu installieren.</translation>
    </message>
    <message>
        <location/>
        <source>Install Mumble Overlay</source>
        <translation>Mumble Overlay Installieren</translation>
    </message>
    <message>
        <location/>
        <source>Overlay Upgrade</source>
        <translation>Overlay Aktualisierung</translation>
    </message>
    <message>
        <location/>
        <source>Mumble has detected an old version of the overlay support files installed on your computer.

To upgrade these files to their latest versions, click the button below.</source>
        <translation>Mumble hat erkannt, dass Sie eine alte Version der Overlay Unterstützungs-Dateien installiert sind.

Um diese Dateien zu aktualisieren, klicken Sie unten den Button.</translation>
    </message>
    <message>
        <location/>
        <source>Upgrade Mumble Overlay</source>
        <translation>Mumble Overlay Aktualisieren</translation>
    </message>
    <message>
        <location/>
        <source>Display a frame counter in the overlay</source>
        <translation>Zeige die FPS im Overlay an</translation>
    </message>
    <message>
        <location/>
        <source>Show FPS counter</source>
        <translation>Zeige FPS Zähler</translation>
    </message>
    <message>
        <location/>
        <source>View Certificates</source>
        <translation>Zertifikate ansehen</translation>
    </message>
    <message>
        <source>&amp;Add...</source>
        <translation type="obsolete">&amp;Hinzufügen...</translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation type="obsolete">&amp;Entfernen</translation>
    </message>
    <message>
        <location/>
        <source>Load an overlay preset from file</source>
        <translation>Lädt eine Overlay Vorlage aus einer Datei</translation>
    </message>
    <message utf8="true">
        <location/>
        <source>Load…</source>
        <translation>Laden...</translation>
    </message>
    <message>
        <location/>
        <source>Save your overlay settings to file</source>
        <translation>Sichert Ihre Overlay Einstellungen in eine Datei</translation>
    </message>
    <message utf8="true">
        <location/>
        <source>Save…</source>
        <translation>Sichern...</translation>
    </message>
    <message>
        <location/>
        <source>FPS Display</source>
        <translation>FPS Anzeige</translation>
    </message>
    <message>
        <location/>
        <source>Set the overlay font.</source>
        <translation>Setzt die Overlay Schrift.</translation>
    </message>
    <message>
        <location/>
        <source>Font</source>
        <translation>Schrift</translation>
    </message>
    <message>
        <location/>
        <source>Set the overlay text color.</source>
        <translation>Setzt die Overlay Textfarbe.</translation>
    </message>
    <message>
        <location/>
        <source>Color</source>
        <translation>Farbe</translation>
    </message>
    <message>
        <location/>
        <source>Add...</source>
        <translation>Hinzufügen...</translation>
    </message>
</context>
<context>
    <name>OverlayEditor</name>
    <message>
        <source>Dialog</source>
        <translation type="obsolete">Dialog</translation>
    </message>
    <message>
        <location filename="OverlayEditor.ui"/>
        <source>State</source>
        <translation>Status</translation>
    </message>
    <message>
        <location/>
        <source>User is not talking</source>
        <translation>Benutzer spricht nicht</translation>
    </message>
    <message>
        <location/>
        <source>Passive</source>
        <translation>Inaktiv</translation>
    </message>
    <message>
        <location/>
        <source>User is talking in your channel or a linked channel</source>
        <translation>Benutzer spricht in Ihrem oder einem verknüpften Kanal</translation>
    </message>
    <message>
        <location/>
        <source>Talking</source>
        <translation>Sprechen</translation>
    </message>
    <message>
        <location/>
        <source>User is whispering to you privately</source>
        <translation>Benutzer flüstert privat zu Ihnen</translation>
    </message>
    <message>
        <location/>
        <source>Private Whisper</source>
        <translation>Flüstern</translation>
    </message>
    <message>
        <location/>
        <source>User is shouting to your channel</source>
        <translation>Benutzer ruft zu den Benutzern in Ihrem Kanal</translation>
    </message>
    <message>
        <location/>
        <source>Channel Whisper</source>
        <translation>Rufen (Kanal)</translation>
    </message>
    <message>
        <location/>
        <source>Zoom</source>
        <translation>Zoom</translation>
    </message>
    <message>
        <location/>
        <source>Zoom Factor</source>
        <translation>Zoomfaktor</translation>
    </message>
    <message>
        <location/>
        <source>Enabled Elements</source>
        <translation>Aktivierte Elemente</translation>
    </message>
    <message>
        <location/>
        <source>User avatar, chosen by each user</source>
        <translation>Avatar, den jeder Benutzer selbst wählen kann</translation>
    </message>
    <message>
        <location/>
        <source>Avatar</source>
        <translation>Avatar</translation>
    </message>
    <message>
        <location/>
        <source>User&apos;s name</source>
        <translation>Benutzername</translation>
    </message>
    <message>
        <location/>
        <source>Username</source>
        <translation>Benutzername</translation>
    </message>
    <message>
        <location/>
        <source>Name of user&apos;s channel, if outside your current channel</source>
        <translation>Name des Kanals des Benutzers, sofern er sich nicht innerhalb ihres Kanals befindet</translation>
    </message>
    <message>
        <location/>
        <source>Channel</source>
        <translation>Kanal</translation>
    </message>
    <message>
        <location/>
        <source>Muted or deafened</source>
        <translation>Stumm oder taub gestellt</translation>
    </message>
    <message>
        <location/>
        <source>Mute state</source>
        <translation>Stumm Status</translation>
    </message>
    <message>
        <location/>
        <source>Bounding box, automatically shrunk to minimum size to contain all visible elements</source>
        <translation>Ein Rahmen, der automatisch alle sichtbaren Elemente einschliesst</translation>
    </message>
    <message>
        <location/>
        <source>Bounding box</source>
        <translation>Rahmen</translation>
    </message>
    <message>
        <location/>
        <source>Overlay Editor</source>
        <translation>Overlay Editor</translation>
    </message>
</context>
<context>
    <name>OverlayEditorScene</name>
    <message>
        <location filename="OverlayEditorScene.cpp" line="+396"/>
        <source>Layout preset</source>
        <translation>Layout Vorlage</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Large square avatar</source>
        <translation>Großer quadratischer Avatar</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Avatar and Name</source>
        <translation>Avatar und Name</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>User Opacity</source>
        <translation>Benutzer-Deckkraft</translation>
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
        <translation>Objekt-Deckkraft</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Alignment</source>
        <translation>Ausrichtung</translation>
    </message>
    <message>
        <location line="+11"/>
        <source>Left</source>
        <translation>Links</translation>
    </message>
    <message>
        <location line="+5"/>
        <location line="+18"/>
        <source>Center</source>
        <translation>Mitte</translation>
    </message>
    <message>
        <location line="-13"/>
        <source>Right</source>
        <translation>Rechts</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Top</source>
        <translation>Oben</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Bottom</source>
        <translation>Unten</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Color...</source>
        <translation>Farbe...</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Font...</source>
        <translation>Schrift...</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Bounding box</source>
        <translation>Rahmen</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Pen width</source>
        <translation>Rahmenbreite</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Padding</source>
        <translation>Rahmenabstand</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Pen color</source>
        <translation>Rahmenfarbe</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Fill color</source>
        <translation>Füllfarbe</translation>
    </message>
    <message>
        <location line="+87"/>
        <source>Pick pen color</source>
        <translation>Pinsel-Farbe wählen</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Pick fill color</source>
        <translation>Füll-Farbe wählen</translation>
    </message>
    <message>
        <location line="+14"/>
        <source>Pick color</source>
        <translation>Farbe wählen</translation>
    </message>
    <message>
        <location line="+21"/>
        <source>Pick font</source>
        <translation>Schriftart wählen</translation>
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
        <translation>Das Laden der Overlay-Bibliothek ist fehlgeschlagen. Das heißt entweder dass:
- die Bibliothek (mumble_ol.dll) nicht in dem Verzeichnis gefunden wurde, aus welchem Sie Mumble gestartet haben
- Sie ein Betriebssystem vor WinXP SP2 benutzen</translation>
    </message>
</context>
<context>
    <name>OverlayUser</name>
    <message>
        <source>Whisper</source>
        <translation type="obsolete">Flüstern</translation>
    </message>
    <message>
        <source>%1%</source>
        <translation type="obsolete">%1%</translation>
    </message>
</context>
<context>
    <name>PAAudioConfig</name>
    <message>
        <source>PortAudio</source>
        <translation type="obsolete">Portaudio</translation>
    </message>
    <message>
        <source>Input Device</source>
        <translation type="obsolete">Eingabegerät</translation>
    </message>
    <message>
        <source>Output Device</source>
        <translation type="obsolete">Ausgabegerät</translation>
    </message>
</context>
<context>
    <name>PlayerModel</name>
    <message>
        <source>Name of player</source>
        <translation type="obsolete">Name des Spielers</translation>
    </message>
    <message>
        <source>Name of channel</source>
        <translation type="obsolete">Name des Kanals</translation>
    </message>
    <message>
        <source>Player flags</source>
        <translation type="obsolete">Spielerattribute</translation>
    </message>
    <message>
        <source>This is a player connected to the server. The icon to the left of the player indicates whether or not they are talking:&lt;br /&gt;&lt;img src=&quot;skin:talking_on.png&quot; /&gt; Talking&lt;br /&gt;&lt;img src=&quot;skin:talking_off.png&quot; /&gt; Not talking</source>
        <translation type="obsolete">Dies ist ein mit dem Server verbundener Benutzer. Das Icon links vom Spieler zeigt an, ob er spricht oder nicht: &lt;br /&gt;&lt;img src=&quot;skin:talking_on.png&quot; /&gt;Sprechend&lt;br /&gt;&lt;img src=&quot;skin:talking_off.png&quot;/&gt; Nicht sprechend</translation>
    </message>
    <message>
        <source>This is a channel on the server. Only players in the same channel can hear each other.</source>
        <translation type="obsolete">Dies ist ein Kanal auf dem Server. Nur Spieler im selben Kanal können sich gegenseitig hören.</translation>
    </message>
    <message>
        <source>This shows the flags the player has on the server, if any:&lt;br /&gt;&lt;img src=&quot;skin:authenticated.png&quot; /&gt;Authenticated user&lt;br /&gt;&lt;img src=&quot;skin:muted_self.png&quot; /&gt;Muted (by self)&lt;br /&gt;&lt;img src=&quot;skin:muted_server.png&quot; /&gt;Muted (by admin)&lt;br /&gt;&lt;img src=&quot;skin:deafened_self.png&quot; /&gt;Deafened (by self)&lt;br /&gt;&lt;img src=&quot;skin:deafened_server.png&quot; /&gt;Deafened (by admin)&lt;br /&gt;A player muted by himself is probably just away, talking on the phone or something like that.&lt;br /&gt;A player muted by an admin is probably also just away, and the noise the player is making was annoying enough that an admin muted him.</source>
        <translation type="obsolete">Dies zeigt die Attribute an, die ein Benutzer auf dem Server hat:&lt;br /&gt;&lt;img src=&quot;skin:authenticated.png&quot; /&gt;Authentifizierter Benutzer&lt;br /&gt;&lt;img src=&quot;skin:muted_self.png&quot; /&gt;Stummgestellt (durch sich selbst)&lt;br /&gt;&lt;img src=&quot;skin:muted_server.png&quot; /&gt;Stummgestellt (durch Admin)&lt;br /&gt;&lt;img src=&quot;skin:deafened_self.png&quot; /&gt;Taubgestellt (durch sich selbst)&lt;br /&gt;&lt;img src=&quot;skin:deafened_server.png&quot; /&gt;Taubgestellt (durch Admin)&lt;br /&gt;Ein Benutzer der sich selbst stummgestellt hat, ist vermutlich weg oder ist am Telefon.&lt;br /&gt;Ein Benutzer der durch einen Admin stummgestellt wurde, ist vermutlich auch weg, aber die Geräusche die der Spieler macht, waren so störend, dass der Admin ihn stummgestellt hat.</translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="obsolete">Name</translation>
    </message>
    <message>
        <source>Flags</source>
        <translation type="obsolete">Attribute</translation>
    </message>
    <message>
        <source>Mumble</source>
        <translation type="obsolete">Mumble</translation>
    </message>
    <message>
        <source>Are you sure you want to drag this channel?</source>
        <translation type="obsolete">Sind Sie sicher, dass Sie den Kanal verschieben möchten?</translation>
    </message>
</context>
<context>
    <name>PluginConfig</name>
    <message>
        <location filename="Plugins.ui"/>
        <source>Options</source>
        <translation>Optionen</translation>
    </message>
    <message>
        <location/>
        <location filename="Plugins.cpp" line="+75"/>
        <source>Plugins</source>
        <translation>Plugins</translation>
    </message>
    <message>
        <location/>
        <source>Link to Game and Transmit Position</source>
        <translation>Mit Spiel verbinden und Position übertragen</translation>
    </message>
    <message>
        <location/>
        <source>Enable plugins and transmit positional information</source>
        <translation>Plugins aktivieren und Positionsinformationen übertragen</translation>
    </message>
    <message>
        <source>This enables plugins for supported games to fetch your in-game position and transmit that with each voice packet. This enables other players to hear your voice in-game from the direction your character is in relation to their own.</source>
        <translation type="obsolete">Dies aktiviert Plugins für unterstützte Spiele um Ihre In-Game-Position zu ermitteln und diese mit jedem Sprachpaket zu übertragen. Dies erlaubt anderen Spielern ihre Stimme aus der Richtung zu hören, in der Sie sich befinden.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Reload plugins</source>
        <translation>Plugins &amp;neu laden</translation>
    </message>
    <message>
        <location/>
        <source>Reloads all plugins</source>
        <translation>Lädt alle Plugins neu</translation>
    </message>
    <message>
        <location/>
        <source>This allows plugins for supported games to fetch your in-game position and transmit it with each voice packet. This enables other users to hear your voice in-game from the direction your character is in relation to their own.</source>
        <oldsource>This enables plugins for supported games to fetch your in-game position and transmit that with each voice packet. This enables other users to hear your voice in-game from the direction your character is in relation to their own.</oldsource>
        <translation>Dies erlaubt es Plugins Ihre Position im Spiel zu bestimmen und diese mit jedem Sprachpaket zu versenden. Dies erlaubt es anderen Benutzern Sie aus jener Richtung zu hören, in welcher Sie aus der Sicht der anderen Person stehen.</translation>
    </message>
    <message>
        <location/>
        <source>Name</source>
        <translation>Name</translation>
    </message>
    <message>
        <location/>
        <source>Enabled</source>
        <translation>Aktiviert</translation>
    </message>
    <message>
        <location/>
        <source>This rescans and reloads plugins. Use this if you just added or changed a plugin to the plugins directory.</source>
        <translation>Dies liest alle Plugins neu ein. Benutzen Sie dies, falls Sie dem Pluginverzeichnis ein Plugin hinzugefügt, oder ein Plugin geändert haben.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;About</source>
        <translation>Ü&amp;ber</translation>
    </message>
    <message>
        <location/>
        <source>Information about plugin</source>
        <translation>Informationen über das Plugin</translation>
    </message>
    <message>
        <location/>
        <source>This shows a small information message about the plugin.</source>
        <translation>Dies zeigt eine kleine Informationsnachricht über das Plugin an.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Configure</source>
        <translation>&amp;Konfigurieren</translation>
    </message>
    <message>
        <location/>
        <source>Show configuration page of plugin</source>
        <translation>Zeige Konfiguration des Plugins</translation>
    </message>
    <message>
        <location/>
        <source>This shows the configuration page of the plugin, if any.</source>
        <translation>Dies zeigt eine Konfigurationsseite des Plugins an, falls eine existiert.</translation>
    </message>
    <message>
        <source>Mumble</source>
        <translation type="obsolete">Mumble</translation>
    </message>
    <message>
        <location filename="Plugins.cpp" line="+56"/>
        <source>Plugin has no configure function.</source>
        <translation>Plugin hat keine Konfigurationsfunktion.</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Plugin has no about function.</source>
        <translation>Plugin hat keine Über-Funktion.</translation>
    </message>
    <message>
        <location filename="Plugins.ui"/>
        <source>Form</source>
        <translation>Formular</translation>
    </message>
</context>
<context>
    <name>Plugins</name>
    <message>
        <location filename="Plugins.cpp" line="+330"/>
        <source>Skipping plugin update in debug mode.</source>
        <translation>Überspringe Plugin-Aktualisierung im Debug-Modus.</translation>
    </message>
    <message>
        <location line="+155"/>
        <location line="+6"/>
        <source>Downloaded new or updated plugin to %1.</source>
        <translation>Neues oder aktualisiertes Plugin nach %1 heruntergeladen.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Failed to install new plugin to %1.</source>
        <translation>Installation eines neuen Plugins nach %1 fehlgeschlagen.</translation>
    </message>
    <message>
        <source>Plugin %1 lost link.</source>
        <translation type="obsolete">Plugin %1 hat Verbindung verloren.</translation>
    </message>
    <message>
        <location line="-265"/>
        <source>%1 lost link.</source>
        <translation>%1 hat Verbindung verloren.</translation>
    </message>
    <message>
        <source>Plugin %1 linked.</source>
        <translation type="obsolete">Plugin %1 hat sich verbunden.</translation>
    </message>
    <message>
        <location line="+74"/>
        <source>%1 linked.</source>
        <translation>%1 verbunden.</translation>
    </message>
</context>
<context>
    <name>PortAudioSystem</name>
    <message>
        <location filename="PAAudio.cpp" line="+273"/>
        <source>Default Device</source>
        <translation>Standardgerät</translation>
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
        <translation type="obsolete">Geräteauswahl</translation>
    </message>
    <message>
        <source>Output</source>
        <translation type="obsolete">Ausgabe</translation>
    </message>
    <message>
        <source>Device to use for speakers/headphones</source>
        <translation type="obsolete">Gerät für Lautsprecher/Kopfhörer</translation>
    </message>
    <message>
        <source>Output Options</source>
        <translation type="obsolete">Ausgabeoptionen</translation>
    </message>
    <message>
        <source>Output Delay</source>
        <translation type="obsolete">Ausgabeverzögerung</translation>
    </message>
    <message>
        <source>Amount of data to buffer for ALSA</source>
        <translation type="obsolete">Menge an Daten die ALSA puffern soll</translation>
    </message>
    <message>
        <source>TextLabel</source>
        <translation type="obsolete">Textlabel</translation>
    </message>
</context>
<context>
    <name>PulseAudioSystem</name>
    <message>
        <location filename="PulseAudio.cpp" line="+545"/>
        <source>Default Input</source>
        <translation>Standardeingabe</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Default Output</source>
        <translation>Standardausgabe</translation>
    </message>
</context>
<context>
    <name>RichTextEditor</name>
    <message>
        <location filename="RichTextEditor.cpp" line="+241"/>
        <source>Failed to load image</source>
        <translation>Laden des Bildes fehlgeschlagen</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Image file too large to embed in document. Please use images smaller than %1 kB.</source>
        <translation>Bilddatei zu groß um Sie im Dokument einzubetten. Bitte verwenden Sie Bilder kleiner als %1 kB.</translation>
    </message>
    <message>
        <location line="+81"/>
        <source>Message is too long.</source>
        <translation>Nachricht ist zu lang.</translation>
    </message>
    <message>
        <source>TabWidget</source>
        <translation type="obsolete">TabWidget</translation>
    </message>
    <message>
        <location filename="RichTextEditor.ui"/>
        <source>Display</source>
        <translation>Anzeige</translation>
    </message>
    <message>
        <location/>
        <source>Source Text</source>
        <translation>Quelltext</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Bold</source>
        <translation>&amp;Fett</translation>
    </message>
    <message>
        <location/>
        <source>Ctrl+B</source>
        <translation>STRG+B</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Italic</source>
        <translation>&amp;Kursiv</translation>
    </message>
    <message>
        <location/>
        <source>Italic</source>
        <translation>Kursiv</translation>
    </message>
    <message>
        <location/>
        <source>Ctrl+I</source>
        <translation>STRG+I</translation>
    </message>
    <message>
        <location/>
        <source>Underline</source>
        <translation>Unterstrichen</translation>
    </message>
    <message>
        <location/>
        <source>Ctrl+U</source>
        <translation>STRG+U</translation>
    </message>
    <message>
        <location/>
        <source>Color</source>
        <translation>Farbe</translation>
    </message>
    <message>
        <location/>
        <source>Insert Link</source>
        <translation>Link einfügen</translation>
    </message>
    <message>
        <location/>
        <source>Ctrl+L</source>
        <translation>STRG+L</translation>
    </message>
    <message>
        <location/>
        <source>Insert Image</source>
        <translation>Bild einfügen</translation>
    </message>
</context>
<context>
    <name>RichTextEditorLink</name>
    <message>
        <location filename="RichTextEditorLink.ui"/>
        <source>Add Link</source>
        <translation>Link hinzufügen</translation>
    </message>
    <message>
        <location/>
        <source>URL</source>
        <translation>URL</translation>
    </message>
    <message>
        <location/>
        <source>Text</source>
        <translation>Text</translation>
    </message>
</context>
<context>
    <name>ServerHandler</name>
    <message>
        <source>UDP packets can not be received from the server. Switching to TCP mode.</source>
        <translation type="obsolete">UDP Pakete konnten nicht vom Server empfangen werden. Wechsel zum TCP-Modus.</translation>
    </message>
    <message>
        <location filename="ServerHandler.cpp" line="+401"/>
        <source>UDP packets cannot be sent to or received from the server. Switching to TCP mode.</source>
        <translation>UDP Pakete können nicht gesendet oder vom Server nicht empfangen werden. Wechsle zu TCP-Modus.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>UDP packets cannot be sent to the server. Switching to TCP mode.</source>
        <translation>UDP Pakete können nicht zum Server gesendet werden. Wechsle in den TCP-Modus.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>UDP packets cannot be received from the server. Switching to TCP mode.</source>
        <translation>UDP Pakete können vom Server nicht empfangen werden. Wechsle in den TCP-Modus.</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>UDP packets can be sent to and received from the server. Switching back to UDP mode.</source>
        <translation>UDP Pakete können zum Server gesendet und vom Server empfangen werden. Wechsle zurück in den UDP-Modus.</translation>
    </message>
    <message>
        <location line="+36"/>
        <source>Connection timed out</source>
        <translation>Verbindung wegen Zeitüberschreitung unterbrochen</translation>
    </message>
</context>
<context>
    <name>ServerView</name>
    <message>
        <location filename="ConnectDialog.cpp" line="-611"/>
        <source>Favorite</source>
        <translation>Favoriten</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>LAN</source>
        <translation>LAN</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Public Internet</source>
        <translation>Internet – Öffentlich</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Africa</source>
        <translation>Afrika</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Asia</source>
        <translation>Asien</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>North America</source>
        <translation>Nordamerika</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>South America</source>
        <translation>Südamerika</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Europe</source>
        <translation>Europa</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Oceania</source>
        <translation>Ozeanien</translation>
    </message>
</context>
<context>
    <name>ShortcutActionWidget</name>
    <message>
        <location filename="GlobalShortcut.cpp" line="-483"/>
        <source>Unassigned</source>
        <translation>Nicht zugewiesen</translation>
    </message>
</context>
<context>
    <name>ShortcutDelegate</name>
    <message>
        <location line="+361"/>
        <source>On</source>
        <translation>An</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Off</source>
        <translation>Aus</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Toggle</source>
        <translation>Wechseln</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Unassigned</source>
        <translation>Nicht zugewiesen</translation>
    </message>
</context>
<context>
    <name>ShortcutKeyWidget</name>
    <message>
        <location line="-435"/>
        <source>Press Shortcut</source>
        <translation>Tastenkürzel drücken</translation>
    </message>
</context>
<context>
    <name>ShortcutTargetDialog</name>
    <message>
        <location line="+206"/>
        <source>Root</source>
        <translation>Hauptkanal</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Parent</source>
        <translation>Elternkanal</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Current</source>
        <translation>Aktueller Kanal</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Subchannel #%1</source>
        <translation>Unterkanal #%1</translation>
    </message>
</context>
<context>
    <name>ShortcutTargetWidget</name>
    <message>
        <location line="+85"/>
        <source>...</source>
        <translation>…</translation>
    </message>
    <message>
        <location line="+38"/>
        <source>, </source>
        <translation>, </translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Root</source>
        <translation>Hauptkanal</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Parent</source>
        <translation>Elternkanal</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Current</source>
        <translation>Aktueller Kanal</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Subchannel #%1</source>
        <translation>Unterkanal #%1</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Invalid</source>
        <translation>Ungültig</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Empty</source>
        <oldsource>&lt;Empty&gt;</oldsource>
        <translation>Leer</translation>
    </message>
</context>
<context>
    <name>ShortcutToggleWidget</name>
    <message>
        <location line="-259"/>
        <source>Off</source>
        <translation>Aus</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Toggle</source>
        <translation>Wechseln</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>On</source>
        <translation>An</translation>
    </message>
</context>
<context>
    <name>TextMessage</name>
    <message>
        <source>Disables html formatting</source>
        <translation type="obsolete">Deaktiviere HTML-Formatierung</translation>
    </message>
    <message>
        <source>Disable text formatting</source>
        <translation type="obsolete">Deaktiviere Text-Formatierung</translation>
    </message>
    <message>
        <source>Message to send</source>
        <translation type="obsolete">Zu sendende Nachricht</translation>
    </message>
    <message>
        <source>Preview</source>
        <translation type="obsolete">Vorschau</translation>
    </message>
    <message>
        <source>Send raw message</source>
        <translation type="obsolete">Roh-Nachricht senden</translation>
    </message>
    <message>
        <source>Disables html formating</source>
        <translation type="obsolete">Deaktiviert HTML Formatierung</translation>
    </message>
    <message>
        <location filename="TextMessage.h" line="+46"/>
        <source>Enter text</source>
        <translation>Text eingeben</translation>
    </message>
    <message>
        <location filename="TextMessage.ui"/>
        <source>If checked the message is recursively sent to all subchannels</source>
        <translation>Wenn aktiviert wird die Nachricht rekursiv auch an alle Unterkanäle gesendet</translation>
    </message>
    <message>
        <location/>
        <source>Send recursively to subchannels</source>
        <translation>Sende rekursiv zu Unterkanälen</translation>
    </message>
</context>
<context>
    <name>Tokens</name>
    <message>
        <location filename="Tokens.cpp" line="+69"/>
        <source>Empty Token</source>
        <translation>Leerer Zugriffscode</translation>
    </message>
    <message>
        <location filename="Tokens.ui"/>
        <source>Mumble - Access Tokens</source>
        <translation>Mumble - Zugriffscodes</translation>
    </message>
    <message>
        <location/>
        <source>List of access tokens on current server</source>
        <translation>Liste von Zugriffscodes auf dem aktuellen Server</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is an editable list of access tokens on the connected server.&lt;/b&gt;
&lt;br /&gt;
An access token is a text string, which can be used as a password for very simple access management on channels. Mumble will remember the tokens you&apos;ve used and resend them to the server next time you reconnect, so you don&apos;t have to enter these every time.
</source>
        <translation>&lt;b&gt;Dies ist eine editierbare Liste von Zugriffscodes auf dem verbundenen Server.&lt;/b&gt;
&lt;br /&gt;
Ein Zugriffscode ist eine Zeichenfolge, die als Passwort für ein sehr einfaches Zugriffsmanagement für Kanäle verwendet werden kann. Mumble merkt sich die von Ihnen verwendeten Zugriffscodes und sendet diese das nächste Mal automatisch an den Server, damit Sie sie nicht jedes Mal eingeben müssen.</translation>
    </message>
    <message>
        <location/>
        <source>Add a token</source>
        <translation>Einen Zugriffscode hinzufügen</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Add</source>
        <translation>&amp;Hinzufügen</translation>
    </message>
    <message>
        <location/>
        <source>Remove a token</source>
        <translation>Einen Zugriffscode entfernen</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Remove</source>
        <translation>&amp;Entfernen</translation>
    </message>
</context>
<context>
    <name>UserEdit</name>
    <message>
        <location filename="UserEdit.ui"/>
        <source>Registered Users</source>
        <translation>Registrierte Benutzer</translation>
    </message>
    <message>
        <location filename="UserEdit.cpp" line="+91"/>
        <source>Rename</source>
        <translation>Umbenennen</translation>
    </message>
    <message>
        <location filename="UserEdit.ui"/>
        <location filename="UserEdit.cpp" line="+5"/>
        <source>Remove</source>
        <translation>Entfernen</translation>
    </message>
</context>
<context>
    <name>UserInformation</name>
    <message>
        <location filename="UserInformation.ui"/>
        <source>User Information</source>
        <translation>Benutzerinformationen</translation>
    </message>
    <message>
        <location/>
        <source>Connection Information</source>
        <translation>Verbindungsinformationen</translation>
    </message>
    <message>
        <location/>
        <source>Version</source>
        <translation>Version</translation>
    </message>
    <message>
        <location/>
        <source>OS</source>
        <translation>Betriebssystem</translation>
    </message>
    <message>
        <location/>
        <source>Certificate</source>
        <translation>Zertifikat</translation>
    </message>
    <message>
        <location/>
        <source>IP Address</source>
        <translation>IP-Adresse</translation>
    </message>
    <message>
        <location/>
        <source>CELT Versions</source>
        <translation>CELT-Versionen</translation>
    </message>
    <message>
        <location/>
        <source>Details...</source>
        <translation>Details...</translation>
    </message>
    <message>
        <location/>
        <source>Ping Statistics</source>
        <translation>Pingstatistik</translation>
    </message>
    <message>
        <location/>
        <source>Pings received</source>
        <translation>Empfangen</translation>
    </message>
    <message>
        <location/>
        <source>Average ping</source>
        <translation>Durchschnitt</translation>
    </message>
    <message>
        <source>Ping variance</source>
        <translation type="obsolete">Varianz</translation>
    </message>
    <message>
        <location/>
        <source>TCP (Control)</source>
        <translation>TCP (Steuerung)</translation>
    </message>
    <message>
        <location/>
        <source>UDP (Voice)</source>
        <translation>UDP (Stimme)</translation>
    </message>
    <message>
        <location/>
        <source>UDP Network statistics</source>
        <translation>UDP Netzwerkstatistik</translation>
    </message>
    <message>
        <location/>
        <source>Good</source>
        <translation>Gut</translation>
    </message>
    <message>
        <location/>
        <source>Late</source>
        <translation>Spät</translation>
    </message>
    <message>
        <location/>
        <source>Lost</source>
        <translation>Verloren</translation>
    </message>
    <message>
        <location/>
        <source>Resync</source>
        <translation>Resync</translation>
    </message>
    <message>
        <location/>
        <source>From Client</source>
        <translation>Vom Client</translation>
    </message>
    <message>
        <location/>
        <source>To Client</source>
        <translation>Zum Client</translation>
    </message>
    <message>
        <source>Bandwidth</source>
        <translation type="obsolete">Bandbreite</translation>
    </message>
    <message>
        <location/>
        <source>Connection time</source>
        <translation>Verbindungszeit</translation>
    </message>
    <message>
        <location filename="UserInformation.cpp" line="+93"/>
        <source>%1w</source>
        <translation>%1w</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1d</source>
        <translation>%1d</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1h</source>
        <translation>%1h</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1m</source>
        <translation>%1m</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>%1s</source>
        <translation>%1s</translation>
    </message>
    <message>
        <source> </source>
        <translation type="obsolete"> </translation>
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
        <translation>%1 online (%2 untätig)</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1 online</source>
        <translation>%1 online</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>%1 kbit/s</source>
        <translation>%1 kbit/s</translation>
    </message>
    <message>
        <location filename="UserInformation.ui"/>
        <source>Bandwidth</source>
        <comment>GroupBox</comment>
        <translation>Bandbreite</translation>
    </message>
    <message>
        <location/>
        <source>Bandwidth</source>
        <comment>Label</comment>
        <translation>Bandbreite</translation>
    </message>
</context>
<context>
    <name>UserModel</name>
    <message>
        <location filename="UserModel.cpp" line="-804"/>
        <source>This is a user connected to the server. The icon to the left of the user indicates whether or not they are talking:</source>
        <translation>Dies ist ein zum Server verbundener Benutzer. Das Icon links vom Benutzer zeigt, ob der Benutzer spricht oder nicht:</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Talking to your channel.</source>
        <translation>Spricht zu Ihrem Kanal.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Shouting directly to your channel.</source>
        <oldsource>Whispering directly to your channel.</oldsource>
        <translation>Direkt an Ihren Kanal rufen.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Whispering directly to you.</source>
        <translation>Flüstert direkt zu Ihnen.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Not talking.</source>
        <translation>Spricht nicht.</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>This is a channel on the server. The icon indicates the state of the channel:</source>
        <translation>Dies ist ein Kanal auf dem Server. Das Icon zeigt den Zustand des Kanals:</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Your current channel.</source>
        <translation>Ihr aktueller Kanal.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>A channel that is linked with your channel. Linked channels can talk to each other.</source>
        <translation>Ein Kanal welcher mit Ihrem Kanal verknüpft ist. Benutzer in verknüpften Kanälen können miteinander sprechen.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>A channel on the server that you are not linked to.</source>
        <translation>Ein Kanal auf dem Server mit dem Sie nicht verknüpft sind.</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>This shows the flags the user has on the server, if any:</source>
        <translation>Dies zeigt die Attribute, welche der Benutzer auf dem Server hat (sofern er welche hat):</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>On your friend list</source>
        <translation>Auf deiner Freundesliste</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Authenticated user</source>
        <translation>Authentifizierter Benutzer</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Muted (manually muted by self)</source>
        <translation>Stumm gestellt (selbst)</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Muted (manually muted by admin)</source>
        <translation>Stumm gestellt (von Admin)</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Muted (not allowed to speak in current channel)</source>
        <translation>Stumm (darf im aktuellen Kanal nicht sprechen)</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Muted (muted by you, only on your machine)</source>
        <translation>Stumm gestellt (von Ihnen, nur für Sie)</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Deafened (by self)</source>
        <translation>Taub gestellt (selbst)</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Deafened (by admin)</source>
        <translation>Taub gestellt (von Admin)</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>User has a new comment set (click to show)</source>
        <translation>Benutzer hat einen neuen Kommentar gesetzt (klicken um anzusehen)</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Channel has a new comment set (click to show)</source>
        <translation>Kanal hat einen neuen Kommentar (klicken um anzusehen)</translation>
    </message>
    <message>
        <location line="+835"/>
        <location line="+20"/>
        <source>Cannot perform this movement automatically, please reset the numeric sorting indicators or adjust it manually.</source>
        <translation>Kann dieses Verschieben nicht automatisiert ausführen. Bitte setzen Sie die Kanalpositionswert zurück oder setzen Sie sie manuell.</translation>
    </message>
    <message>
        <location line="-863"/>
        <source>User has a comment set, which you&apos;ve already seen. (click to show)</source>
        <translation>Benutzer hat einen Kommentar gesetzt, welchen Sie schon gesehen haben. (klicken um anzusehen)</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>This shows the flags the channel has, if any:</source>
        <translation>Dies zeigt die Attribute, welche der Kanal hat (sofern vorhanden):</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Channel has a comment set, which you&apos;ve already seen. (click to show)</source>
        <translation>Kanal hat einen Kommentar gesetzt, welchen Sie bereits gesehen haben. (klicken um anzusehen)</translation>
    </message>
    <message>
        <location line="+18"/>
        <source>Name</source>
        <translation>Name</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Flags</source>
        <translation>Attribute</translation>
    </message>
    <message>
        <source>Mumble</source>
        <translation type="obsolete">Mumble</translation>
    </message>
    <message>
        <location line="+721"/>
        <source>Are you sure you want to drag this channel?</source>
        <translation>Sind Sie sicher, dass Sie den Kanal verschieben möchten?</translation>
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
        <translation type="obsolete">Mumble konnte keine Versionsinformationen vom SourceForge-Server ermitteln.</translation>
    </message>
    <message>
        <location filename="VersionCheck.cpp" line="+130"/>
        <source>Upgrade Mumble</source>
        <translation>Mumble aktualisieren</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>A new version of Mumble has been detected and automatically downloaded. It is recommended that you either upgrade to this version, or downgrade to the latest stable release. Do you want to launch the installer now?</source>
        <translation>Eine neue Version von Mumble ist verfügbar und wurde automatisch heruntergeladen. Es wird empfohlen, dass Sie entweder diese neue Version oder die letzte stabile Version installieren. Möchten Sie die neue Version jetzt installieren?</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Failed to launch snapshot installer.</source>
        <translation>Start des Snapshot-Installers fehlgeschlagen.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Corrupt download of new version detected. Automatically removed.</source>
        <translation>Der Download der neuen Mumble-Version war fehlerhaft und wurde gelöscht.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Downloading new snapshot from %1 to %2</source>
        <translation>Neuer Snapshot wird heruntergeladen von %1 nach %2</translation>
    </message>
    <message>
        <location line="+18"/>
        <source>Failed to write new version to disk.</source>
        <oldsource>Failed to write new version to disc.</oldsource>
        <translation>Neue Version konnte nicht auf die Festplatte geschrieben werden.</translation>
    </message>
    <message>
        <location line="+17"/>
        <source>Mumble failed to retrieve version information from the central server.</source>
        <translation>Mumble konnte keine Versions-Informationen vom Zentralserver abfragen.</translation>
    </message>
</context>
<context>
    <name>ViewCert</name>
    <message>
        <location filename="ViewCert.cpp" line="+41"/>
        <source>Certificate Chain Details</source>
        <translation>Zertifikatsketten-Details</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Certificate chain</source>
        <translation>Zertifikatskette</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Certificate details</source>
        <translation>Zertifikatdetails</translation>
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
        <translation>Allgemeiner Name: %1</translation>
    </message>
    <message>
        <location line="-27"/>
        <location line="+28"/>
        <source>Organization: %1</source>
        <translation>Organisation: %1</translation>
    </message>
    <message>
        <location line="-27"/>
        <source>Subunit: %1</source>
        <translation>Untereinheit: %1</translation>
    </message>
    <message>
        <location line="+1"/>
        <location line="+28"/>
        <source>Country: %1</source>
        <translation>Staat: %1</translation>
    </message>
    <message>
        <location line="-27"/>
        <location line="+28"/>
        <source>Locality: %1</source>
        <translation>Lokalität: %1</translation>
    </message>
    <message>
        <location line="-27"/>
        <location line="+28"/>
        <source>State: %1</source>
        <translation>Land: %1</translation>
    </message>
    <message>
        <location line="-27"/>
        <source>Valid from: %1</source>
        <translation>Gültig von : %1</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Valid to: %1</source>
        <translation>Gültig bis: %1</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Serial: %1</source>
        <translation>Serial: %1</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Public Key: %1 bits %2</source>
        <translation>Öffentlicher Schlüssel: %1 bits %2</translation>
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
        <translation>Digest (MD5): %1</translation>
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
        <translation>Ausgegeben von:</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Unit Name: %1</source>
        <translation>Einheitname: %1</translation>
    </message>
</context>
<context>
    <name>VoiceRecorder</name>
    <message>
        <location filename="VoiceRecorder.cpp" line="+184"/>
        <source>Invalid sample rate given to recorder</source>
        <translation>Ungültige Abtastrate wurde an den Recorder übergeben</translation>
    </message>
    <message>
        <location line="+98"/>
        <source>Recorder failed to create directory &apos;%1&apos;</source>
        <translation>Recorder konnte Verzeichnis &apos;%1&apos; nicht erstellen</translation>
    </message>
    <message>
        <location line="+14"/>
        <source>Recorder failed to open file &apos;%s&apos;</source>
        <translation>Recorder konnte Datei &apos;%s&apos; nicht öffnen</translation>
    </message>
    <message>
        <location line="+109"/>
        <source>.wav - Uncompressed</source>
        <translation>.wav - Unkomprimiert</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>.ogg (Vorbis) - Compressed</source>
        <translation>.ogg (Vorbis) - Komprimiert</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>.au - Uncompressed</source>
        <translation>.au - Unkomprimiert</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>.flac - Lossless compressed</source>
        <translation>.flac - Verlustfrei komprimiert</translation>
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
        <translation>Aufnahme</translation>
    </message>
    <message>
        <location/>
        <source>Control</source>
        <translation>Steuerung</translation>
    </message>
    <message>
        <location/>
        <source>00:00:00</source>
        <translation>00:00:00</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Start</source>
        <translation>&amp;Start</translation>
    </message>
    <message>
        <location/>
        <source>S&amp;top</source>
        <translation>S&amp;top</translation>
    </message>
    <message>
        <location/>
        <source>Mode</source>
        <translation>Modus</translation>
    </message>
    <message>
        <location/>
        <source>Multichannel</source>
        <translation>Mehrkanal</translation>
    </message>
    <message>
        <location/>
        <source>Output</source>
        <translation>Ausgabe</translation>
    </message>
    <message>
        <location/>
        <source>Output format</source>
        <translation>Ausgabeformat</translation>
    </message>
    <message>
        <location/>
        <source>Target directory</source>
        <translation>Zielverzeichnis</translation>
    </message>
    <message>
        <location/>
        <source>Filename</source>
        <translation>Dateiname</translation>
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
Gültige Variablen sind:
%user	Fügt den Benutzernamen ein
%date	Fügt das aktuelle Datum ein
%time	Fügt die aktuelle Uhrzeit ein
%host	Fügt den Hostnamen ein
&lt;/pre&gt;</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Browse...</source>
        <translation>&amp;Durchsuchen...</translation>
    </message>
    <message>
        <location filename="VoiceRecorderDialog.cpp" line="-195"/>
        <source>Closing the recorder will stop your current recording. Do you really want to close the recorder?</source>
        <translation>Wenn Sie den Aufnahmedialog schließen wird die Aufnahme beendet. Sind sie sicher?</translation>
    </message>
    <message>
        <source>Recoder</source>
        <translation type="obsolete">Aufnahme</translation>
    </message>
    <message>
        <location line="-31"/>
        <source>Valid variables are:</source>
        <translation>Gültige Variablen sind:</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Inserts the user&apos;s name</source>
        <translation>Fügt den Namen des Benutzers ein</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Inserts the current date</source>
        <translation>Fügt das aktuelle Datum ein</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Inserts the current time</source>
        <translation>Fügt die aktuelle Uhrzeit ein</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Inserts the hostname</source>
        <translation>Fügt den Hostnamen ein</translation>
    </message>
    <message>
        <location line="+60"/>
        <source>Unable to start recording. Not connected to a server.</source>
        <translation>Aufnahme kann nicht gestartet werden, da keine Verbindung zu einem Server besteht.</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>The server you are currently connected to is version 1.2.2 or older. For privacy reasons, recording on servers of versions older than 1.2.3 is not possible.
Please contact your server administrator for further information.</source>
        <translation>Der Server mit dem Sie aktuell verbunden sind hat die Version 1.2.2 oder älter. Aus Gründen der Privatsphäre sind Aufnahmen auf Servern älter als 1.2.3 nicht möglich.</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>There is already a recorder active for this server.</source>
        <translation>Es läuft bereits eine Aufnahme für diesen Server.</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Please select a recording format.</source>
        <translation>Bitte wählen Sie ein Aufnahmeformat aus.</translation>
    </message>
    <message>
        <location line="+95"/>
        <source>Select target directory</source>
        <translation>Zielverzeichnis auswählen</translation>
    </message>
    <message>
        <location filename="VoiceRecorderDialog.ui"/>
        <source>Downmix</source>
        <translation>Gemischt</translation>
    </message>
</context>
<context>
    <name>WASAPIConfig</name>
    <message>
        <source>Device selection</source>
        <translation type="obsolete">Geräteauswahl</translation>
    </message>
    <message>
        <source>Output</source>
        <translation type="obsolete">Ausgabe</translation>
    </message>
    <message>
        <source>Device to use for speakers/headphones</source>
        <translation type="obsolete">Gerät für Lautsprecher/Kopfhörer</translation>
    </message>
</context>
<context>
    <name>WASAPISystem</name>
    <message>
        <location filename="WASAPI.cpp" line="+180"/>
        <source>Default Device</source>
        <translation>Standardgerät</translation>
    </message>
</context>
<context>
    <name>XInputKeyWidget</name>
    <message>
        <source>Press Shortcut</source>
        <translation type="obsolete">Shortcut drücken</translation>
    </message>
</context>
</TS>
