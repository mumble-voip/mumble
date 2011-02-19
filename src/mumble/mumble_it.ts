<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.0" language="it_IT" sourcelanguage="en">
<context>
    <name>ACLEditor</name>
    <message>
        <source>Mumble - Edit ACL for %1</source>
        <translation type="obsolete">Mumble - Modifica ACL per %1</translation>
    </message>
    <message>
        <location filename="ACLEditor.cpp" line="+101"/>
        <source>Deny</source>
        <translation>Nega</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Allow</source>
        <translation>Permetti</translation>
    </message>
    <message>
        <location line="+21"/>
        <source>Allow %1</source>
        <translation>Permetti %1</translation>
    </message>
    <message>
        <location line="-8"/>
        <source>Deny %1</source>
        <translation>Nega %1</translation>
    </message>
    <message>
        <location line="-62"/>
        <source>Mumble - Add channel</source>
        <translation>Mumble - Aggiunta di un canale</translation>
    </message>
    <message>
        <location line="+22"/>
        <location line="+165"/>
        <source>Failed: Invalid channel</source>
        <translation>Errore: Canale non valido</translation>
    </message>
    <message>
        <location line="-153"/>
        <source>Mumble - Edit %1</source>
        <translation>Mumble - Modifica %1</translation>
    </message>
    <message>
        <location line="+37"/>
        <source>This grants the %1 privilege. If a privilege is both allowed and denied, it is denied.&lt;br /&gt;%2</source>
        <translation>Garantisce il privilegio %1. Se un privilegio è contemporaneamente garantito e negato, sarà negato.&lt;br /&gt;%2</translation>
    </message>
    <message>
        <location line="-8"/>
        <source>This revokes the %1 privilege. If a privilege is both allowed and denied, it is denied.&lt;br /&gt;%2</source>
        <translation>Revoca il privilegio %1. Se un privilegio è contemporaneamente garantito e negato, sarà negato.&lt;br /&gt;%2</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui"/>
        <source>Dialog</source>
        <translation>Dialogo</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation type="obsolete">Proprietà</translation>
    </message>
    <message>
        <location/>
        <source>Name</source>
        <translation>Nome</translation>
    </message>
    <message>
        <location/>
        <source>Enter the channel name here.</source>
        <translation>Inserisci qui il nome del canale.</translation>
    </message>
    <message>
        <source>&lt;b&gt;Name&lt;/b&gt;&lt;br&gt;Enter the channel name in this field. The name has to comply with the restriction imposed by the server you are connected to.</source>
        <translation type="obsolete">&lt;b&gt;Nome&lt;/b&gt;&lt;br&gt;Inserisci il nome del canale in questo campo. Il nome deve attenersi alle restrizioni imposte dal server a cui sei connesso.</translation>
    </message>
    <message>
        <location/>
        <source>Description</source>
        <translation>Descrizione</translation>
    </message>
    <message>
        <location/>
        <source>Password</source>
        <translation>Password</translation>
    </message>
    <message>
        <location/>
        <source>Enter the channel password here.</source>
        <translation>Inserisci qui la password del canale.</translation>
    </message>
    <message>
        <source>&lt;b&gt;Password&lt;/b&gt;&lt;br&gt;This field allows you to easily set and change the password of a channel. In the background it uses Mumble&apos;s access tokens feature. To allow more fine grained and powerful access control directly use ACLs and groups instead (&lt;i&gt;Advanced configuration&lt;/i&gt; has to be checked to be able to see these settings).</source>
        <translation type="obsolete">&lt;b&gt;Password&lt;/b&gt;&lt;br&gt;Questo campo ti permette di impostare e cambiare facilmente la password del canale. Come base usa il sistema dei Token d&apos;accesso di Mumble. Per consentire un controllo piú forte e raffinato usa le ACL ed i gruppi (&lt;i&gt;Configurazione avanzata&lt;/i&gt; deve essere selezionata per poter vedere queste impostazioni).</translation>
    </message>
    <message>
        <location/>
        <source>Check to create a temporary channel.</source>
        <translation>Seleziona per creare un canale temporaneo.</translation>
    </message>
    <message>
        <source>&lt;b&gt;Temporary&lt;/b&gt;&lt;br&gt;
When checked the channel created will be marked as temporary. This means when the last player leaves it the channel will be automatically deleted by the server.</source>
        <translation type="obsolete">&lt;b&gt;Temporaneo&lt;/b&gt;&lt;br&gt;
Quando selezionato il canale creato sarà segnato come temporaneo. Questo vuol dire che quando l&apos;ultimo utente uscirà dal canale, questo verrà automaticamente cancellato dal server.</translation>
    </message>
    <message>
        <location/>
        <source>Temporary</source>
        <translation>Canale temporaneo</translation>
    </message>
    <message>
        <location/>
        <source>Channel positioning facility value</source>
        <translation>Valore posizionamento canale</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Position&lt;/b&gt;&lt;br/&gt;
This value enables you to change the way Mumble arranges the channels in the tree. A channel with a higher &lt;i&gt;Position&lt;/i&gt; value will always be placed below one with a lower value and the other way around. If the &lt;i&gt;Position&lt;/i&gt; value of two channels is equal they will get sorted alphabetically by their name.</source>
        <oldsource>&lt;b&gt;Position&lt;/b&gt;&lt;br/&gt;
This value enables you to change the way mumble arranges the channels in the tree. A channel with a higher &lt;i&gt;Position&lt;/i&gt; value will always be placed below one with a lower value and the other way around. If the &lt;i&gt;Position&lt;/i&gt; value of two channels is equal they will get sorted alphabetically by their name.</oldsource>
        <translation>&lt;b&gt;Posizione&lt;/b&gt;&lt;br/&gt;
Questo valore ti dà la possibilità di cambiare il modo in cui Mumble posiziona i canali nell&apos;albero dei canali. Un canale con un valore di &lt;i&gt;Posizione&lt;/i&gt; più alto sarà sempre posto sotto uno con un valore più basso e così via. Se il valore della &lt;i&gt;Posizione&lt;/i&gt; di due canali è uguale allora questi saranno ordinati in ordine alfabetico secondo il loro nome.</translation>
    </message>
    <message>
        <location/>
        <source>Position</source>
        <translation>Posizione</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Groups</source>
        <translation>&amp;Gruppi</translation>
    </message>
    <message>
        <location/>
        <source>Group</source>
        <translation>Gruppo</translation>
    </message>
    <message>
        <location/>
        <source>List of groups</source>
        <translation>Lista dei gruppi</translation>
    </message>
    <message>
        <source>This is all the groups currently defined for the channel. To create a new group, just type in the name and press enter.</source>
        <translation type="obsolete">Questi sono tutti i gruppi attualmente definiti per un canale. Per creare un nuovo gruppo, scrivi il nome e premi invio.</translation>
    </message>
    <message>
        <location/>
        <source>Remove selected group</source>
        <translation>Rimuove il gruppo selezionato</translation>
    </message>
    <message>
        <source>This removes the currently selected group. If the group was inherited, it will not be removed from the list, but all local information about the group will be cleared.</source>
        <translation type="obsolete">Cancella il gruppo selezionato. Se un gruppo era ereditario, non verrà rimosso dalla lista, ma tutte le informazioni locali riguardanti il gruppo saranno cancellate.</translation>
    </message>
    <message>
        <location/>
        <source>Remove</source>
        <translation>Rimuovi</translation>
    </message>
    <message>
        <location/>
        <source>Inherit group members from parent</source>
        <translation>Eredita i membri del gruppo dal canale superiore</translation>
    </message>
    <message>
        <source>This inherits all the members in the group from the parent, if the group is marked as &lt;i&gt;Inheritable&lt;/i&gt; in the parent channel.</source>
        <translation type="obsolete">Eredita tutti i membri di un gruppo dal padre, se un gruppo è segnato come &lt;i&gt;Ereditabile&lt;/i&gt;nel canale padre.</translation>
    </message>
    <message>
        <location/>
        <source>Inherit</source>
        <translation>Eredita</translation>
    </message>
    <message>
        <location/>
        <source>Make group inheritable to sub-channels</source>
        <translation>Rende un gruppo ereditabile ai sottocanali</translation>
    </message>
    <message>
        <source>This makes this group inheritable to sub-channels. If the group is non-inheritable, sub-channels are still free to create a new group with the same name.</source>
        <translation type="obsolete">Rende ereditabile un gruppo ai sotto-canali. Se un gruppo non è ereditabile, i sotto-canali saranno comunque liberi di creare un nuovo gruppo con lo stesso nome.</translation>
    </message>
    <message>
        <location/>
        <source>Inheritable</source>
        <translation>Ereditabile</translation>
    </message>
    <message>
        <location/>
        <source>Group was inherited from parent channel</source>
        <translation>Gruppo ereditato dal canale superiore</translation>
    </message>
    <message>
        <source>This indicates that the group was inherited from the parent channel. You cannot edit this flag, it&apos;s just for information.</source>
        <translation type="obsolete">Indica che un gruppo è stato ereditato dal canale padre. Non puoi modificare questa impostazione, è solo informativa.</translation>
    </message>
    <message>
        <location/>
        <source>Inherited</source>
        <translation>Ereditato</translation>
    </message>
    <message>
        <location/>
        <source>Members</source>
        <translation>Membri</translation>
    </message>
    <message>
        <location/>
        <source>Add</source>
        <translation>Aggiungi</translation>
    </message>
    <message>
        <source>Add to Remove</source>
        <translation type="obsolete">Aggiungi a Rimuovi</translation>
    </message>
    <message>
        <location/>
        <source>Add member to group</source>
        <translation>Aggiungi membro al gruppo</translation>
    </message>
    <message>
        <source>Type in the name of a player you wish to add to the group and click Add.</source>
        <translation type="obsolete">Inserisci il nome del giocatore che vuoi aggiungere al gruppo e premi Aggiungi.</translation>
    </message>
    <message>
        <location/>
        <source>Remove member from group</source>
        <translation>Rimuove un membro dal gruppo</translation>
    </message>
    <message>
        <source>Type in the name of a player you wish to remove from the group and click Add.</source>
        <translation type="obsolete">Inserisci il nome del giocatore che vuoi cancellare dal gruppo e premi Add.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;ACL</source>
        <translation>&amp;ACL</translation>
    </message>
    <message>
        <location/>
        <source>Active ACLs</source>
        <translation>ACL attive</translation>
    </message>
    <message>
        <location/>
        <source>List of entries</source>
        <translation>Elenco di voci</translation>
    </message>
    <message>
        <source>This shows all the entries active on this channel. Entries inherited from parent channels will be show in italics.&lt;br /&gt;ACLs are evaluated top to bottom, meaning priority increases as you move down the list.</source>
        <translation type="obsolete">Mostra tutte le voci attive su questo canale. Le voci ereditate dal canale padre verranno visualizzate in corsivo.&lt;br /&gt;Le ACL sono considerate dall&apos;alto al basso, cioè la priorità aumenta spostandosi verso il basso nell&apos;elenco.</translation>
    </message>
    <message>
        <location/>
        <source>Inherit ACL of parent?</source>
        <translation>Eredita ACL del canale superiore?</translation>
    </message>
    <message>
        <location/>
        <source>This sets whether or not the ACL up the chain of parent channels are applied to this object. Only those entries that are marked in the parent as &quot;Apply to sub-channels&quot; will be inherited.</source>
        <translation>Imposta se le ACL provenienti dalla catena parentale dei canali sono applicate a questo oggetto. Solo quelle voci che sono contrassegnate nel canale superiore come &quot;Applica ai sottocanali&quot; saranno ereditate.</translation>
    </message>
    <message>
        <location/>
        <source>Inherit ACLs</source>
        <translation>Eredita le ACL</translation>
    </message>
    <message>
        <location/>
        <source>Move entry up</source>
        <translation>Sposta voce in alto</translation>
    </message>
    <message>
        <location/>
        <source>This moves the entry up in the list. As entries are evaluated in order, this may change the effective permissions of users. You cannot move an entry above an inherited entry, if you really need that you&apos;ll have to duplicate the inherited entry.</source>
        <translation>Sposta in alto la voce nella lista. Dato che le voci vengono valutate in ordine, questo potrebbe cambiare le autorizzazioni attive per gli utenti. Non è possibile spostare una voce al di sopra di un ingresso ereditato, se proprio avete bisogno di farlo potete duplicare la voce ereditata.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Up</source>
        <translation>&amp;Su</translation>
    </message>
    <message>
        <location/>
        <source>Move entry down</source>
        <translation>Sposta la voce in basso</translation>
    </message>
    <message>
        <location/>
        <source>This moves the entry down in the list. As entries are evaluated in order, this may change the effective permissions of users.</source>
        <translation>Sposta in giú la voce nella lista. Dato che le voci vengono valutate in ordine, questo potrebbe cambiare le autorizzazioni attive per gli utenti.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Down</source>
        <translation>&amp;Giú</translation>
    </message>
    <message>
        <location/>
        <source>Add new entry</source>
        <translation>Aggiungi nuova voce</translation>
    </message>
    <message>
        <location/>
        <source>This adds a new entry, initially set with no permissions and applying to all.</source>
        <translation>Aggiunge una nuova voce, inizialmente impostata senza permessi e applicata a tutti.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Add</source>
        <translation>&amp;Aggiungi</translation>
    </message>
    <message>
        <location/>
        <source>Remove entry</source>
        <translation>Rimuovi voce</translation>
    </message>
    <message>
        <location/>
        <source>This removes the currently selected entry.</source>
        <translation>Rimuove la voce selezionata.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Remove</source>
        <translation>&amp;Rimuovi</translation>
    </message>
    <message>
        <location/>
        <source>Context</source>
        <translation>Contesto</translation>
    </message>
    <message>
        <source>Entry should apply to this channel</source>
        <translation type="obsolete">La voce dovrebbe valere per questo canale</translation>
    </message>
    <message>
        <location/>
        <source>This makes the entry apply to this channel.</source>
        <translation>Applica la voce a questo canale.</translation>
    </message>
    <message>
        <location/>
        <source>Applies to this channel</source>
        <translation>Applica a questo canale</translation>
    </message>
    <message>
        <location/>
        <source>Entry should apply to sub-channels.</source>
        <translation>La voce dovrebbe applicarsi ai sottocanali.</translation>
    </message>
    <message>
        <source>&lt;b&gt;Group&lt;/b&gt;&lt;br&gt;
These are all the groups currently defined for the channel. To create a new group, just type in the name and press enter.</source>
        <oldsource>&lt;b&gt;Group&lt;/b&gt;&lt;br&gt;
This is all the groups currently defined for the channel. To create a new group, just type in the name and press enter.</oldsource>
        <translation type="obsolete">&lt;b&gt;Gruppo&lt;/b&gt;&lt;br&gt;
Questi sono tutti i gruppi attualmente definiti per il canale. Per creare un nuovo gruppo basta inserire il nome e premere invio.</translation>
    </message>
    <message>
        <location/>
        <source>Add new group</source>
        <translation>Aggiungi un nuovo gruppo</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Add&lt;/b&gt;&lt;br/&gt;
Add a new group.</source>
        <translation>&lt;b&gt;Aggiungi&lt;/b&gt;&lt;br/&gt;
Aggiunge un nuovo gruppo.</translation>
    </message>
    <message>
        <source>&lt;b&gt;Remove&lt;/b&gt;&lt;br&gt;This removes the currently selected group. If the group was inherited, it will not be removed from the list, but all local information about the group will be cleared.</source>
        <translation type="obsolete">&lt;b&gt;Rimuovi&lt;/b&gt;&lt;br&gt;Rimuove il gruppo selezionato. Se il gruppo è stato ereditato, questo non sarà rimosso dalla lista, ma tutte le informazioni locali saranno rimosse.</translation>
    </message>
    <message>
        <source>&lt;b&gt;Inherit&lt;/b&gt;&lt;br&gt;This inherits all the members in the group from the parent, if the group is marked as &lt;i&gt;Inheritable&lt;/i&gt; in the parent channel.</source>
        <translation type="obsolete">&lt;b&gt;Eredita&lt;/b&gt;&lt;br&gt;Eredita tutti i membri nel gruppo dal canale genitore, se il gruppo è segnato come &lt;i&gt;Ereditabile&lt;/i&gt; nel canale genitore.</translation>
    </message>
    <message>
        <source>&lt;b&gt;Inheritable&lt;/b&gt;&lt;br&gt;This makes this group inheritable to sub-channels. If the group is non-inheritable, sub-channels are still free to create a new group with the same name.</source>
        <translation type="obsolete">&lt;b&gt;Ereditabile&lt;/b&gt;&lt;br&gt;Permette al gruppo di essere ereditato ai sottocanali. Se il gruppo non è ereditabile i sottocanali sono ancora liberi di creare un gruppo con lo stesso nome.</translation>
    </message>
    <message>
        <source>&lt;b&gt;Inherited&lt;/b&gt;&lt;br&gt;This indicates that the group was inherited from the parent channel. You cannot edit this flag, it&apos;s just for information.</source>
        <translation type="obsolete">&lt;b&gt;Ereditato&lt;/b&gt;&lt;br&gt;Indica che il gruppo è stato ereditato dal canale genitore. Non puoi cambiare questa selezione, è solamente un&apos;informazione.</translation>
    </message>
    <message>
        <location/>
        <source>Inherited members</source>
        <translation>Membri ereditati</translation>
    </message>
    <message>
        <location/>
        <source>Contains the list of members added to the group by this channel.</source>
        <translation>Contiene la lista dei membri aggiunti al gruppo da questo canale.</translation>
    </message>
    <message>
        <source>&lt;b&gt;Members&lt;/b&gt;&lt;br&gt;
This list contains all members that were added to the group by the current channel. Be aware that this does not include members inherited by higher levels of the channel tree. These can be found in the &lt;i&gt;Inherited members&lt;/i&gt; list. To prevent this list to be inherited by lower level channels uncheck &lt;i&gt;Inheritable&lt;/i&gt; or manually add the members to the &lt;i&gt;Excluded members&lt;/i&gt; list.</source>
        <translation type="obsolete">&lt;b&gt;Membri&lt;/b&gt;&lt;br&gt;
Questa lista contiene tutti i membri che sono stati aggiunti al gruppo dal canale attuale. Attenzione, non include i membri ereditati da livelli superiori dell&apos;albero dei canali. Questi possono essere trovati nella lista dei &lt;i&gt;Membri ereditati&lt;/i&gt;. Per prevenire che questa lista sia ereditata dai canali inferiori deseleziona &lt;i&gt;Ereditabile&lt;/i&gt; o aggiungi manualmente i membri alla lista dei &lt;i&gt;Membri esclusi dall&apos;eredità&lt;/i&gt;.</translation>
    </message>
    <message>
        <location/>
        <source>Contains a list of members whose group membership will not be inherited from the parent channel.</source>
        <translation>Contiene una lista dei membri che non saranno ereditati ai canali inferiori.</translation>
    </message>
    <message>
        <source>&lt;b&gt;Excluded members&lt;/b&gt;&lt;br&gt;
Contains a list of members whose group membership will not be inherited from the parent channel.</source>
        <translation type="obsolete">&lt;b&gt;Membri esclusi dall&apos;eredità&lt;/b&gt;&lt;br&gt;
Contiene una lista dei membri che non saranno ereditati ai canali inferiori.</translation>
    </message>
    <message>
        <location/>
        <source>Contains the list of members inherited by other channels.</source>
        <translation>Contiene la lista degli utenti ereditati dagli altri canali.</translation>
    </message>
    <message>
        <source>&lt;b&gt;Inherited members&lt;/b&gt;&lt;br&gt;
Contains the list of members inherited by the current channel. Uncheck &lt;i&gt;Inherit&lt;/i&gt; to prevent inheritance from higher level channels.</source>
        <translation type="obsolete">&lt;b&gt;Membri ereditati&lt;/b&gt;&lt;br&gt;
Contiene la lista dei membri ereditati dal canale attuale. Deseleziona &lt;i&gt;Eredita&lt;/i&gt; per prevenire l&apos;ereditazione da canali di livello piú alto.</translation>
    </message>
    <message>
        <location/>
        <source>Type in the name of a user you wish to add to the group and click Add.</source>
        <translation>Inserisci il nome dell&apos;utente che vuoi aggiungere al gruppo e premi Aggiungi.</translation>
    </message>
    <message>
        <location/>
        <source>Type in the name of a user you wish to remove from the group and click Add.</source>
        <translation>Inserisci il nome dell&apos;utente che vuoi rimuovere dal gruppo e premi Aggiungi.</translation>
    </message>
    <message>
        <location/>
        <source>Exclude</source>
        <translation>Escludi</translation>
    </message>
    <message>
        <location/>
        <source>Excluded members</source>
        <translation>Membri esclusi dall&apos;eredità</translation>
    </message>
    <message>
        <location/>
        <source>This makes the entry apply to sub-channels of this channel.</source>
        <translation>Applica le regole ai sottocanali di questo canale.</translation>
    </message>
    <message>
        <location/>
        <source>Applies to sub-channels</source>
        <translation>Applica ai sottocanali</translation>
    </message>
    <message>
        <location/>
        <source>Entry should apply to this channel.</source>
        <translation>Applica le regole a questo canale.</translation>
    </message>
    <message>
        <source>This controls which group of users this entry applies to.&lt;br /&gt;Note that the group is evaluated in the context of the channel the entry is used in. For example, the default ACL on the Root channel gives &lt;i&gt;Write&lt;/i&gt; permission to the &lt;i&gt;admin&lt;/i&gt; group. This entry, if inherited by a channel, will give a user write privileges if he belongs to the &lt;i&gt;admin&lt;/i&gt; group in that channel, even if he doesn&apos;t belong to the &lt;i&gt;admin&lt;/i&gt; group in the channel where the ACL originated.&lt;br /&gt;If a group name starts with a &apos;!&apos;, its membership is negated, and if it starts with a &apos;~&apos;, it is evaluated in the channel the ACL was defined in, rather than the channel the ACL is active in.&lt;br /&gt;If a group name starts with a  &apos;#&apos;, it is interpreted as an access token. Users must have entered whatever follows the &apos;#&apos; in their list of access tokens to match. This can be used for very simple password access to channels for non-authenticated users.&lt;br /&gt;If a group name starts with a  &apos;$&apos;, it will only match users whose certificate hash matches what follows the &apos;$&apos;.&lt;br /&gt;A few special predefined groups are:&lt;br /&gt;&lt;b&gt;all&lt;/b&gt; - Everyone will match.&lt;br /&gt;&lt;b&gt;auth&lt;/b&gt; - All authenticated users will match.&lt;br /&gt;&lt;b&gt;sub,a,b,c&lt;/b&gt; - User currently in a sub-channel minimum &lt;i&gt;a&lt;/i&gt; common parents, and between &lt;i&gt;b&lt;/i&gt; and &lt;i&gt;c&lt;/i&gt; channels down the chain. See the website for more extensive documentation on this one.&lt;br /&gt;&lt;b&gt;in&lt;/b&gt; - Users currently in the channel will match (convenience for &apos;&lt;i&gt;sub,0,0,0&lt;/i&gt;&apos;).&lt;br /&gt;&lt;b&gt;out&lt;/b&gt; - Users outside the channel will match (convenience for &apos;&lt;i&gt;!sub,0,0,0&lt;/i&gt;&apos;).&lt;br /&gt;Note that an entry applies to either a user or a group, not both.</source>
        <translation type="obsolete">Controlla a quale gruppo di utenti si applica questa voce.&lt;br /&gt;Si noti che il gruppo è valutato nel contesto del canale in cui la voce è inserita. Ad esempio, il valore predefinito ACL sul canale Root dà il permesso &lt;i&gt;Scrivi&lt;/i&gt; al gruppo &lt;i&gt;admin&lt;/i&gt;. Questa voce, se ereditata da un canale, darà ad un utente i privilegi di scrittura se egli appartiene al gruppo &lt;i&gt;admin&lt;/i&gt; in quel canale, anche se non apparteneva al gruppo &lt;i&gt;admin&lt;/i&gt; nel canale in cui è nato l&apos;ACL.&lt;br /&gt;Se il nome di un gruppo inizia con un !, è negata ai suoi membri, e se inizia con una ~, è valutata nel canale in cui l&apos;ACL è stata definita, al posto che nel canale in cui è stata attivata l&apos;ACL.&lt;br /&gt;Se il nome di un gruppo inizia con &apos;#&apos; questo viene interpretato come un Token di accesso. Gli utenti devono aver inserito qualcosa dopo &apos;#&apos; nella loro lista dei Token di accesso per essere riconosciuto. Questo può essere usato come semplice accesso con password ai canali, per utenti non autenticati.&lt;br /&gt;Se il nome di un gruppo inizia con &apos;$&apos;, questo accetta solo utenti con nome del certificato che inizia con &apos;$&apos;.&lt;br /&gt; Pochi particolari gruppi predefiniti sono i seguenti:&lt;br /&gt;&lt;b&gt;all&lt;/b&gt; - Corrisponderà a tutti.&lt;b&gt;auth&lt;/b&gt; - Corrisponderanno tutti gli utenti autenticati.&lt;br /&gt;&lt;b&gt;sub,a,b,c&lt;/b&gt; - Utenti in a sotto-canali minimo &lt;i&gt;a&lt;/i&gt; genitori comuni, e tra &lt;i&gt;b&lt;/i&gt; e &lt;i&gt;c&lt;/i&gt; canali verso il basso nella catena. Vedere il sito web per una documentazione piú estesa su questo punto.&lt;br /&gt;&lt;b&gt;in&lt;/b&gt; - Utenti attualmente nel canale (scorciatoia di &lt;i&gt;sub,0,0,0&lt;/i&gt;).&lt;br /&gt;&lt;b&gt;out&lt;/b&gt; - Gli utenti al di fuori del canale (scorciatoia di &lt;i&gt;!sub,0,0,0&lt;/i&gt;).&lt;br /&gt;Si noti che una voce si applica ad un utente o ad un gruppo, non a entrambi.</translation>
    </message>
    <message>
        <location/>
        <source>Permissions</source>
        <translation>Permessi</translation>
    </message>
    <message>
        <location/>
        <source>User/Group</source>
        <translation>Utente/Gruppo</translation>
    </message>
    <message>
        <location/>
        <source>Group this entry applies to</source>
        <translation>Gruppo a cui si applica questa voce</translation>
    </message>
    <message>
        <source>This controls which group of users this entry applies to.&lt;br /&gt;Note that the group is evaluated in the context of the channel the entry is used in. For example, the default ACL on the Root channel gives &lt;i&gt;Write&lt;/i&gt; permission to the &lt;i&gt;admin&lt;/i&gt; group. This entry, if inherited by a channel, will give a user write privileges if he belongs to the &lt;i&gt;admin&lt;/i&gt; group in that channel, even if he doesn&apos;t belong to the &lt;i&gt;admin&lt;/i&gt; group in the channel where the ACL originated.&lt;br /&gt;If a group name starts with a !, its membership is negated, and if it starts with a ~, it is evaluated in the channel the ACL was defined in, rather than the channel the ACL is active in. Order is important; &lt;i&gt;!~in&lt;/i&gt; is valid, but &lt;i&gt;~!in&lt;/i&gt; is not.&lt;br /&gt;A few special predefined groups are:&lt;br /&gt;&lt;b&gt;all&lt;/b&gt; - Everyone will match.&lt;br /&gt;&lt;b&gt;auth&lt;/b&gt; - All authenticated users will match.&lt;br /&gt;&lt;b&gt;sub,a,b,c&lt;/b&gt; - User currently in a sub-channel minimum &lt;i&gt;a&lt;/i&gt; common parents, and between &lt;i&gt;b&lt;/i&gt; and &lt;i&gt;c&lt;/i&gt; channels down the chain. See the website for more extensive documentation on this one.&lt;br /&gt;&lt;b&gt;in&lt;/b&gt; - Users currently in the channel will match (convenience for &lt;i&gt;sub,0,0,0&lt;/i&gt;).&lt;br /&gt;&lt;b&gt;out&lt;/b&gt; - Users outside the channel will match (convenience for &lt;i&gt;!sub,0,0,0&lt;/i&gt;).&lt;br /&gt;Note that an entry applies to either a user or a group, not both.</source>
        <translation type="obsolete">Controlla a quale gruppo di utenti si applica questa voce.&lt;br /&gt;Si noti che il gruppo è valutato nel contesto del canale in cui la voce è inserita. Ad esempio, il valore predefinito ACL sul canale Root dà il permesso &lt;i&gt;Scrivi&lt;/i&gt; al gruppo &lt;i&gt;admin&lt;/i&gt;. Questa voce, se ereditata da un canale, darà ad un utente i privilegi di scrittura se egli appartiene al gruppo &lt;i&gt;admin&lt;/i&gt; in quel canale, anche se non apparteneva al gruppo &lt;i&gt;admin&lt;/i&gt; nel canale in cui è nato l&apos;ACL.&lt;br /&gt;Se il nome di un gruppo inizia con un !, è negata ai suoi membri, e se inizia con una ~, è valutata nel canale in cui l&apos;ACL è stata definita, al posto che nel canale in cui è stata attivata l&apos;ACL. L&apos;ordine è importante; &lt;i&gt;!~in&lt;/i&gt; è valido, ma &lt;i&gt;~!in&lt;/i&gt; non lo è.&lt;br /&gt;Pochi particolari gruppi predefiniti sono i seguenti:&lt;br /&gt;&lt;b&gt;all&lt;/b&gt; - Corrisponderà a tutti.&lt;b&gt;auth&lt;/b&gt; - Corrisponderanno tutti gli utenti autenticati.&lt;br /&gt;&lt;b&gt;sub,a,b,c&lt;/b&gt; - Utenti in a sotto-canali minimo &lt;i&gt;a&lt;/i&gt; genitori comuni, e tra &lt;i&gt;b&lt;/i&gt; e &lt;i&gt;c&lt;/i&gt; canali verso il basso nella catena. Vedere il sito web per una documentazione piú estesa su questo punto.&lt;br /&gt;&lt;b&gt;in&lt;/b&gt; - Utenti attualmente nel canale (scorciatoia di &lt;i&gt;sub,0,0,0&lt;/i&gt;).&lt;br /&gt;&lt;b&gt;out&lt;/b&gt; - Gli utenti al di fuori del canale (scorciatoia di &lt;i&gt;!sub,0,0,0&lt;/i&gt;).&lt;br /&gt;Si noti che una voce si applica ad un utente o ad un gruppo, non a entrambi.</translation>
    </message>
    <message>
        <location/>
        <source>User ID</source>
        <translation>ID Utente</translation>
    </message>
    <message>
        <location/>
        <source>User this entry applies to</source>
        <translation>Utente a cui applicare questa voce</translation>
    </message>
    <message>
        <location/>
        <source>This controls which user this entry applies to. Just type in the user name and hit enter to query the server for a match.</source>
        <translation>Controlla a quale utente si applica questa voce. Basta digitare il nome utente e premere invio per chiedere al server una corrispondenza.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Properties</source>
        <translation>&amp;Proprietà</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Name&lt;/b&gt;&lt;br /&gt;Enter the channel name in this field. The name has to comply with the restriction imposed by the server you are connected to.</source>
        <translation>&lt;b&gt;Nome&lt;/b&gt;&lt;br /&gt;Inserisci il nome del canale in questo campo. Il nome deve attenersi alle restrizioni imposte dal server a cui sei connesso.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Temporary&lt;/b&gt;&lt;br /&gt;
When checked the channel created will be marked as temporary. This means when the last player leaves it the channel will be automatically deleted by the server.</source>
        <translation>&lt;b&gt;Temporaneo&lt;/b&gt;&lt;br /&gt;
Quando selezionato il canale creato sarà segnato come temporaneo. Questo vuol dire che quando l&apos;ultimo utente uscirà dal canale, questo verrà automaticamente cancellato dal server.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Group&lt;/b&gt;&lt;br /&gt;
These are all the groups currently defined for the channel. To create a new group, just type in the name and press enter.</source>
        <translation>&lt;b&gt;Gruppo&lt;/b&gt;&lt;br /&gt;
Questi sono tutti i gruppi attualmente definiti per il canale. Per creare un nuovo gruppo basta inserire il nome e premere invio.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Remove&lt;/b&gt;&lt;br /&gt;This removes the currently selected group. If the group was inherited, it will not be removed from the list, but all local information about the group will be cleared.</source>
        <translation>&lt;b&gt;Rimuovi&lt;/b&gt;&lt;br /&gt;Rimuove il gruppo selezionato. Se il gruppo è stato ereditato, questo non sarà rimosso dalla lista, ma tutte le informazioni locali saranno rimosse.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Inherit&lt;/b&gt;&lt;br /&gt;This inherits all the members in the group from the parent, if the group is marked as &lt;i&gt;Inheritable&lt;/i&gt; in the parent channel.</source>
        <translation>&lt;b&gt;Eredita&lt;/b&gt;&lt;br /&gt;Eredita tutti i membri nel gruppo dal canale superiore, se il gruppo è segnato come &lt;i&gt;Ereditabile&lt;/i&gt; nel canale superiore.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Inheritable&lt;/b&gt;&lt;br /&gt;This makes this group inheritable to sub-channels. If the group is non-inheritable, sub-channels are still free to create a new group with the same name.</source>
        <translation>&lt;b&gt;Ereditabile&lt;/b&gt;&lt;br /&gt;Permette al gruppo di essere ereditato ai sottocanali. Se il gruppo non è ereditabile i sottocanali sono ancora liberi di creare un gruppo con lo stesso nome.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Inherited&lt;/b&gt;&lt;br /&gt;This indicates that the group was inherited from the parent channel. You cannot edit this flag, it&apos;s just for information.</source>
        <translation>&lt;b&gt;Ereditato&lt;/b&gt;&lt;br /&gt;Indica che il gruppo è stato ereditato dal canale superiore. Non puoi cambiare questa selezione, è solamente un&apos;informazione.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Members&lt;/b&gt;&lt;br /&gt;
This list contains all members that were added to the group by the current channel. Be aware that this does not include members inherited by higher levels of the channel tree. These can be found in the &lt;i&gt;Inherited members&lt;/i&gt; list. To prevent this list to be inherited by lower level channels uncheck &lt;i&gt;Inheritable&lt;/i&gt; or manually add the members to the &lt;i&gt;Excluded members&lt;/i&gt; list.</source>
        <translation>&lt;b&gt;Membri&lt;/b&gt;&lt;br /&gt;
Questa lista contiene tutti i membri che sono stati aggiunti al gruppo dal canale attuale. Attenzione, non include i membri ereditati da livelli superiori dell&apos;albero dei canali. Questi possono essere trovati nella lista dei &lt;i&gt;Membri ereditati&lt;/i&gt;. Per prevenire che questa lista sia ereditata dai canali inferiori deseleziona &lt;i&gt;Ereditabile&lt;/i&gt; o aggiungi manualmente i membri alla lista dei &lt;i&gt;Membri esclusi dall&apos;eredità&lt;/i&gt;.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Excluded members&lt;/b&gt;&lt;br /&gt;
Contains a list of members whose group membership will not be inherited from the parent channel.</source>
        <translation>&lt;b&gt;Membri esclusi dall&apos;eredità&lt;/b&gt;&lt;br /&gt;
Contiene una lista dei membri la cui appartenenza ad un gruppo non sarà ereditata dal canale superiore.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Inherited members&lt;/b&gt;&lt;br /&gt;
Contains the list of members inherited by the current channel. Uncheck &lt;i&gt;Inherit&lt;/i&gt; to prevent inheritance from higher level channels.</source>
        <translation>&lt;b&gt;Membri ereditati&lt;/b&gt;&lt;br /&gt;
Contiene la lista dei membri ereditati dal canale attuale. Deseleziona &lt;i&gt;Eredita&lt;/i&gt; per prevenire l&apos;ereditazione dai canali superiori.</translation>
    </message>
    <message>
        <location/>
        <source>This controls which group of users this entry applies to.&lt;br /&gt;Note that the group is evaluated in the context of the channel the entry is used in. For example, the default ACL on the Root channel gives &lt;i&gt;Write&lt;/i&gt; permission to the &lt;i&gt;admin&lt;/i&gt; group. This entry, if inherited by a channel, will give a user write privileges if he belongs to the &lt;i&gt;admin&lt;/i&gt; group in that channel, even if he doesn&apos;t belong to the &lt;i&gt;admin&lt;/i&gt; group in the channel where the ACL originated.&lt;br /&gt;If a group name starts with &apos;!&apos;, its membership is negated, and if it starts with &apos;~&apos;, it is evaluated in the channel the ACL was defined in, rather than the channel the ACL is active in.&lt;br /&gt;If a group name starts with &apos;#&apos;, it is interpreted as an access token. Users must have entered whatever follows the &apos;#&apos; in their list of access tokens to match. This can be used for very simple password access to channels for non-authenticated users.&lt;br /&gt;If a group name starts with &apos;$&apos;, it will only match users whose certificate hash matches what follows the &apos;$&apos;.&lt;br /&gt;A few special predefined groups are:&lt;br /&gt;&lt;b&gt;all&lt;/b&gt; - Everyone will match.&lt;br /&gt;&lt;b&gt;auth&lt;/b&gt; - All authenticated users will match.&lt;br /&gt;&lt;b&gt;sub,a,b,c&lt;/b&gt; - User currently in a sub-channel minimum &lt;i&gt;a&lt;/i&gt; common parents, and between &lt;i&gt;b&lt;/i&gt; and &lt;i&gt;c&lt;/i&gt; channels down the chain. See the website for more extensive documentation on this one.&lt;br /&gt;&lt;b&gt;in&lt;/b&gt; - Users currently in the channel will match (convenience for &apos;&lt;i&gt;sub,0,0,0&lt;/i&gt;&apos;).&lt;br /&gt;&lt;b&gt;out&lt;/b&gt; - Users outside the channel will match (convenience for &apos;&lt;i&gt;!sub,0,0,0&lt;/i&gt;&apos;).&lt;br /&gt;Note that an entry applies to either a user or a group, not both.</source>
        <translation>Controlla a quale gruppo di utenti si applica questa voce.&lt;br /&gt;Si noti che il gruppo è valutato nel contesto del canale in cui la voce è inserita. Ad esempio, il valore predefinito ACL sul canale Radice (Root, quello superiore a tutti) dà il permesso &lt;i&gt;Scrivi&lt;/i&gt; al gruppo &lt;i&gt;admin&lt;/i&gt;. Questa voce, se ereditata da un canale, darà ad un utente i privilegi di scrittura se egli appartiene al gruppo &lt;i&gt;admin&lt;/i&gt; in quel canale, anche se non apparteneva al gruppo &lt;i&gt;admin&lt;/i&gt; nel canale in cui è nato l&apos;ACL.&lt;br /&gt;Se il nome di un gruppo inizia con un &apos;!&apos;, è negata ai suoi membri, e se inizia con un &apos;~&apos;, è valutata nel canale in cui l&apos;ACL è stata definita, al posto che nel canale in cui è stata attivata l&apos;ACL.&lt;br /&gt;Se il nome di un gruppo inizia con &apos;#&apos; viene interpretato come un token di accesso. Gli utenti devono avere inserito nella loro lista dei token tutto quello che segue &apos;#&apos; per appartenere a quel gruppo. Questo può essere utilizzato per un semplice accesso ai canali con password da parte di utenti non registrati. Se il nome di un gruppo inizia con &apos;$&apos;, gli utenti che ne faranno parte saranno solo quelli il cui hash del certificato sarà quello che è scritto dopo &apos;$&apos;. Alcuni particolari gruppi predefiniti sono i seguenti:&lt;br /&gt;&lt;b&gt;all&lt;/b&gt; - Tutti ne fanno parte.&lt;b&gt;auth&lt;/b&gt; - Tutti gli utenti autenticati ne fanno parte.&lt;br /&gt;&lt;b&gt;sub,a,b,c&lt;/b&gt; - Utenti in almeno un sottocanale &lt;i&gt;a&lt;/i&gt; canali superiori comuni, e tra &lt;i&gt;b&lt;/i&gt; e &lt;i&gt;c&lt;/i&gt; canali verso il basso nella catena. Visitare il sito web per una documentazione piú estesa su questo punto.&lt;br /&gt;&lt;b&gt;in&lt;/b&gt; - Utenti attualmente nel canale (scorciatoia per &apos;&lt;i&gt;sub,0,0,0&lt;/i&gt;&apos;).&lt;br /&gt;&lt;b&gt;out&lt;/b&gt; - Gli utenti al di fuori del canale (scorciatoia per &apos;&lt;i&gt;!sub,0,0,0&lt;/i&gt;&apos;).&lt;br /&gt;Nota: una voce si applica ad un utente o ad un gruppo, non a entrambi.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Password&lt;/b&gt;&lt;br /&gt;This field allows you to easily set and change the password of a channel. It uses Mumble&apos;s access tokens feature in the background. Use ACLs and groups if you need more fine grained and powerful access control.</source>
        <translation>&lt;b&gt;Password&lt;/b&gt;&lt;br /&gt;Questo campo ti permette di impostare e cambiare facilmente la password di un canale. Utilizza la funzione di Mumble accesso con token. Se hai bisogno di un controllo maggiore sull&apos;accesso a questo canale utilizza le ACL e i gruppi.</translation>
    </message>
    <message>
        <location/>
        <source>This shows all the entries active on this channel. Entries inherited from parent channels will be shown in italics.&lt;br /&gt;ACLs are evaluated top to bottom, meaning priority increases as you move down the list.</source>
        <translation>Mostra tutte le voci attive su questo canale. Le voci ereditate dal canale superiore verranno visualizzate in corsivo.&lt;br /&gt;Le ACL sono considerate dall&apos;alto verso il basso, quindi le voci piú in basso saranno piú importanti e quindi applicate in caso di conflitto.</translation>
    </message>
</context>
<context>
    <name>ALSAAudioInput</name>
    <message>
        <location filename="ALSAAudio.cpp" line="+169"/>
        <source>Default ALSA Card</source>
        <translation>Scheda ALSA predefinita</translation>
    </message>
    <message>
        <location line="+191"/>
        <source>Opening chosen ALSA Input failed: %1</source>
        <translation>Errore nell&apos;apertura dell&apos;ingresso ALSA selezionato: %1</translation>
    </message>
</context>
<context>
    <name>ALSAAudioOutput</name>
    <message>
        <location line="-190"/>
        <source>Default ALSA Card</source>
        <translation>Scheda ALSA predefinita</translation>
    </message>
    <message>
        <location line="+325"/>
        <source>Opening chosen ALSA Output failed: %1</source>
        <translation>Errore nell&apos;apertura dell&apos;uscita ALSA selezionata: %1</translation>
    </message>
</context>
<context>
    <name>ASIOConfig</name>
    <message>
        <source>%1 (ver %2)</source>
        <translation type="obsolete">%1 (ver %2)</translation>
    </message>
    <message>
        <source>%1 ms -&gt; %2 ms (%3 ms resolution) %4Hz</source>
        <translation type="obsolete">%1 ms -&gt; %2 ms (%3 ms risoluzione) %4Hz</translation>
    </message>
    <message>
        <source>%1 ms -&gt; %2 ms (%3 ms resolution) %4Hz -- Unusable</source>
        <translation type="obsolete">%1 ms -&gt; %2 ms (%3 ms risoluzione) %4Hz -- Non usabile</translation>
    </message>
    <message>
        <location filename="ASIOInput.cpp" line="+222"/>
        <source>%1 (version %2)</source>
        <translation>%1 (versione %2)</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>%1 -&gt; %2 samples buffer, with %3 sample resolution (%4 preferred) at %5 Hz</source>
        <oldsource>%1 -&gt; %2 samples buffer, with %3 sample resolution (%4 preferred) at %5Hz</oldsource>
        <translation>%1 -&gt; %2 campioni del buffer, con risoluzione campioni %3 (%4 preferito) a %5Hz</translation>
    </message>
    <message>
        <source>Mumble</source>
        <translation type="obsolete">Mumble</translation>
    </message>
    <message>
        <location line="+43"/>
        <location line="+25"/>
        <source>ASIO Initialization failed: %1</source>
        <translation>Errore nell&apos;inizializzazione di ASIO: %1</translation>
    </message>
    <message>
        <location line="-21"/>
        <location line="+25"/>
        <source>Failed to instantiate ASIO driver</source>
        <translation>Errore nell&apos;instanziazione del driver ASIO</translation>
    </message>
    <message>
        <location line="+37"/>
        <source>ASIO</source>
        <translation>ASIO</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui"/>
        <source>Form</source>
        <translation>Modulo</translation>
    </message>
    <message>
        <location/>
        <source>Device selection</source>
        <translation>Selezione del dispositivo</translation>
    </message>
    <message>
        <location/>
        <source>Device</source>
        <translation>Dispositivo</translation>
    </message>
    <message>
        <location/>
        <source>Device to use for microphone</source>
        <translation>Dispositivo da usare per il microfono</translation>
    </message>
    <message>
        <location/>
        <source>This chooses what device to query. You still need to actually query the device and select which channels to use.</source>
        <translation>Sceglie quale dispositivo interrogare. Devi comunque interrogare il dispositivo e selezionare quale canale usare.</translation>
    </message>
    <message>
        <location/>
        <source>Query selected device</source>
        <translation>Interroga il dispositivo selezionato</translation>
    </message>
    <message>
        <location/>
        <source>This queries the selected device for channels. Be aware that many ASIO drivers are buggy to the extreme, and querying them might cause a crash of either the application or the system.</source>
        <translation>Interroga il dispositivo selezionato per i canali. Molti driver ASIO sono estremamente difettati, e utilizzandoli potrebbero causare un crash dell&apos;applicazione o del sistema.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Query</source>
        <translation>&amp;Interroga</translation>
    </message>
    <message>
        <location/>
        <source>Configure selected device</source>
        <translation>Configura il dispositivo selezionato</translation>
    </message>
    <message>
        <location/>
        <source>This configures the selected device. Be aware that many ASIO drivers are buggy to the extreme, and querying them might cause a crash of either the application or the system.</source>
        <translation>Configura il dispositivo selezionato. Molti driver ASIO sono estremamente difettati, e utilizzandoli potrebbero causare un crash dell&apos;applicazione o del sistema.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Configure</source>
        <translation>&amp;Configura</translation>
    </message>
    <message>
        <location/>
        <source>Capabilities</source>
        <translation>Funzionalità</translation>
    </message>
    <message>
        <location/>
        <source>Driver name</source>
        <translation>Nome del driver</translation>
    </message>
    <message>
        <location/>
        <source>Buffer size</source>
        <translation>Grandezza del buffer</translation>
    </message>
    <message>
        <location/>
        <source>This will configure the input channels for ASIO. Make sure you select at least one channel as microphone and speaker. &lt;i&gt;Microphone&lt;/i&gt; should be where your microphone is attached, and &lt;i&gt;Speaker&lt;/i&gt; should be a channel that samples &apos;&lt;i&gt;What you hear&lt;/i&gt;&apos;.&lt;br /&gt;For example, on the Audigy 2 ZS, a good selection for Microphone would be &apos;&lt;i&gt;Mic L&lt;/i&gt;&apos; while Speaker should be &apos;&lt;i&gt;Mix L&lt;/i&gt;&apos; and &apos;&lt;i&gt;Mix R&lt;/i&gt;&apos;.</source>
        <translation>Questo configurerà i canali d&apos;ingresso per ASIO. Assicurati di aver selezionato almeno un canale per il microfono e le cuffie/casse. &lt;i&gt;Microfono&lt;/i&gt; dovrebbe essere dove il tuo microfono è attaccato, e &lt;i&gt;Casse&lt;/i&gt; dovrebbe essere dove sono i canali di &apos;&lt;i&gt;Quello che senti&lt;/i&gt;&apos;.&lt;br /&gt; Per esempio, sull&apos;Audigy 2 ZS, una buona selezione per Microfono potrebbe essere &apos;&lt;i&gt;Mic L&lt;/i&gt;&apos; mentre Casse &apos;&lt;i&gt;Mix L&lt;/i&gt;&apos; e &apos;&lt;i&gt;Mix R&lt;/i&gt;&apos;.</translation>
    </message>
    <message>
        <source>TextLabel</source>
        <translation type="obsolete">Etichetta testo</translation>
    </message>
    <message>
        <source>Buffersize</source>
        <translation type="obsolete">Dimensione del Buffer</translation>
    </message>
    <message>
        <location/>
        <source>Configure input channels</source>
        <translation>Configura i canali di ingresso</translation>
    </message>
    <message>
        <source>This will configure the input channels for ASIO. Make sure you select at least one channel as microphone and speaker. &lt;i&gt;Microphone&lt;/i&gt; should be where your microphone is attached, and &lt;i&gt;Speaker&lt;/i&gt; should be a channel that samples &lt;i&gt;What you hear&lt;/i&gt;.&lt;br /&gt;For example, on the Audigy 2 ZS, a good selection for Microphone would be &lt;i&gt;Mic L&lt;/i&gt; while Speaker should be &lt;i&gt;Mix L&lt;/i&gt; and &lt;i&gt;Mix R&lt;/i&gt;.</source>
        <translation type="obsolete">Configurarerà i canali di ingresso per ASIO. Assicurarsi di selezionare almeno un canale come microfono e altoparlante. &lt;i&gt;Microfono&lt;/i&gt; deve essere dove è collegato il  microfono, e &lt;i&gt;Altoparlante&lt;/i&gt; deve essere il canale che catturi &lt;i&gt;Cosa si sente&lt;/i&gt;.&lt;br /&gt;Ad esempio, con Audigy 2 ZS, una buona scelta per Microfono sarebbe &lt;i&gt;Mic L&lt;/i&gt;mentre per gli altoparlanti dovrebbe essere &lt;i&gt;Mix L&lt;/i&gt; e &lt;i&gt;Mix R&lt;/i&gt;.</translation>
    </message>
    <message>
        <location/>
        <source>Channels</source>
        <translation>Canali</translation>
    </message>
    <message>
        <location/>
        <source>Microphone</source>
        <translation>Microfono</translation>
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
        <translation>Non usato</translation>
    </message>
    <message>
        <location/>
        <source>Speakers</source>
        <translation>Casse/cuffie</translation>
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
        <translation>Devi selezionare almeno un microfono e un altoparlante per usare ASIO. Se hai bisogno solo il campionamento microfonico, usa DirectSound.</translation>
    </message>
    <message>
        <location line="+103"/>
        <source>Opening selected ASIO device failed. No input will be done.</source>
        <translation>Apertura del dispositivo ASIO fallita. Non ci sarà nessun ingresso.</translation>
    </message>
</context>
<context>
    <name>AboutDialog</name>
    <message>
        <location filename="About.cpp" line="+37"/>
        <source>About Mumble</source>
        <translation>Informazioni su Mumble</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>&lt;h3&gt;Mumble (%1)&lt;/h3&gt;&lt;p&gt;Copyright %3 Thorvald Natvig&lt;br /&gt;slicer@users.sourceforge.net&lt;/p&gt;&lt;p&gt;&lt;b&gt;A voice-chat utility for gamers&lt;/b&gt;&lt;/p&gt;&lt;p&gt;&lt;tt&gt;&lt;a href=&quot;%2&quot;&gt;%2&lt;/a&gt;&lt;/tt&gt;&lt;/p&gt;</source>
        <translation>&lt;h3&gt;Mumble (%1)&lt;/h3&gt;&lt;p&gt;Copyright %3 Thorvald Natvig&lt;br /&gt;slicer@users.sourceforge.net&lt;br /&gt;&lt;br /&gt;Traduzione italiana a cura di Spaccaossi&lt;br /&gt;spaccaossi@users.sourceforge.net&lt;/p&gt;&lt;p&gt;&lt;b&gt;Utility di chat vocale per videogiocatori&lt;/b&gt;&lt;/p&gt;&lt;p&gt;&lt;tt&gt;&lt;a href=&quot;%2&quot;&gt;%2&lt;/a&gt;&lt;/tt&gt;&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>&amp;About Mumble</source>
        <translation>&amp;Info su Mumble</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>&amp;License</source>
        <translation>&amp;Licenza</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>OK</source>
        <translation>OK</translation>
    </message>
</context>
<context>
    <name>AboutSpeexDialog</name>
    <message>
        <location line="+8"/>
        <source>About Speex</source>
        <translation>Informazioni su Speex</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>&lt;h3&gt;About Speex&lt;/h3&gt;&lt;p&gt;&lt;tt&gt;&lt;a href=&quot;%1&quot;&gt;%1&lt;/a&gt;&lt;/tt&gt;&lt;/p&gt;&lt;p&gt;This program uses SpeexDSP.&lt;/p&gt;&lt;p&gt;Speex is used for echo cancellation, noise&lt;br /&gt;filtering and voice activity detection.&lt;/p&gt;</source>
        <translation>&lt;h3&gt;Informazioni su Speex&lt;/h3&gt;&lt;p&gt;&lt;tt&gt;&lt;a href=&quot;%1&quot;&gt;%1&lt;/a&gt;&lt;/tt&gt;&lt;/p&gt;&lt;p&gt;Questo programma usa SpeexDSP.&lt;/p&gt;&lt;p&gt;Speex è usato per la cancellazione dell&apos;eco, &lt;br /&gt;il filtraggio dei rumori e l&apos;attivazione vocale.&lt;/p&gt;</translation>
    </message>
    <message>
        <source>&lt;h3&gt;About Speex&lt;/h3&gt;&lt;p&gt;&lt;tt&gt;&lt;a href=&quot;%2&quot;&gt;%2&lt;/a&gt;&lt;/tt&gt;&lt;/p&gt;&lt;p&gt;This program uses Speex version %1&lt;/p&gt;&lt;p&gt;Speex is used for echo cancellation, noise&lt;br /&gt;filtering, voice activity detection and speech&lt;br /&gt;compression.&lt;/p&gt;</source>
        <translation type="obsolete">&lt;h3&gt;Informazioni su Speex&lt;/h3&gt;&lt;p&gt;&lt;tt&gt;&lt;a href=&quot;%2&quot;&gt;%2&lt;/a&gt;&lt;/tt&gt;&lt;/p&gt;&lt;p&gt;Questo programma usa Speex versione %1&lt;/p&gt;&lt;p&gt;Speex è usato per la cancellazione dell&apos;eco, il filtraggio&lt;br /&gt;del rumore, il rilevamento del parlato e per la compressione&lt;br /&gt;della voce.&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>OK</source>
        <translation>OK</translation>
    </message>
</context>
<context>
    <name>AudioInput</name>
    <message>
        <location filename="AudioInput.ui"/>
        <source>Form</source>
        <translation>Modulo</translation>
    </message>
    <message>
        <location/>
        <source>Interface</source>
        <translation>Interfaccia</translation>
    </message>
    <message>
        <location/>
        <source>System</source>
        <translation>Sistema</translation>
    </message>
    <message>
        <location/>
        <source>Input method for audio</source>
        <translation>Metodo di ingresso sonoro</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is the input method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation>&lt;b&gt;Metodo di ingresso da utilizzare per il suono.&lt;/b&gt;&lt;br /&gt;Molto probabilmente vorrai usare DirectSound.</translation>
    </message>
    <message>
        <location/>
        <source>Device</source>
        <translation>Dispositivo</translation>
    </message>
    <message>
        <source>Output method for audio</source>
        <translation type="obsolete">Metodo d&apos;output per l&apos;audio</translation>
    </message>
    <message>
        <source>&lt;b&gt;This is the output method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation type="obsolete">&lt;b&gt;Metodo da utilizzare per l&apos;output dell&apos;audio.&lt;/b&gt;&lt;br /&gt;Molto probabilmente vorrai usare DirectSound.</translation>
    </message>
    <message>
        <source>Cancel Echo</source>
        <translation type="obsolete">Rimuovi l&apos;Eco</translation>
    </message>
    <message>
        <location/>
        <source>Transmission</source>
        <translation>Trasmissione</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Transmit</source>
        <translation>&amp;Metodo di trasmissione</translation>
    </message>
    <message>
        <location/>
        <source>When to transmit your speech</source>
        <translation>Quando trasmettere la tua voce</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets when speech should be transmitted.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;Continuous&lt;/i&gt; - All the time&lt;br /&gt;&lt;i&gt;Voice Activity&lt;/i&gt; - When you are speaking clearly.&lt;br /&gt;&lt;i&gt;Push To Talk&lt;/i&gt; - When you hold down the hotkey set under &lt;i&gt;Shortcuts&lt;/i&gt;.</source>
        <translation>&lt;b&gt;Imposta quando la voce dovrebbe essere trasmessa.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;Continuamente&lt;/i&gt; - Tutto il tempo&lt;br /&gt;&lt;i&gt;Attivazione vocale&lt;/i&gt; - Quando stai parlando.&lt;br /&gt;&lt;i&gt;Premi-Per-Parlare&lt;/i&gt; - Quando premi il pulsante PPP (Premi Per Parlare) impostato sotto &lt;i&gt;Scorciatoie&lt;/i&gt; nelle impostazioni di Mumble.</translation>
    </message>
    <message>
        <location/>
        <source>DoublePush Time</source>
        <translation>Tempo di DoppiaPressione</translation>
    </message>
    <message>
        <location/>
        <source>If you press the PTT key twice in this time it will get locked.</source>
        <translation>Se premi il pulsante PPP due volte la trasmissione si dovrebbe bloccare.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;DoublePush Time&lt;/b&gt;&lt;br /&gt;If you press the push-to-talk key twice during the configured interval of time it will be locked. Mumble will keep transmitting until you hit the key once more to unlock PTT again.</source>
        <translation>&lt;b&gt;Tempo di DoppiaPressione&lt;/b&gt;&lt;br /&gt;Se premi il pulsante Premi-Per-Parlare due volte durante l&apos;intervallo di tempo configurato la trasmissione non finirà fino a che non premerai di nuovo il pulsante PPP.</translation>
    </message>
    <message>
        <source>TextLabel</source>
        <translation type="obsolete">Etichetta testo</translation>
    </message>
    <message>
        <location/>
        <source>Gets played when the PTT button is pressed</source>
        <translation>Viene riprodotto quando il pulsante PPP viene premuto</translation>
    </message>
    <message>
        <location/>
        <source>Gets played when the PTT button is released</source>
        <translation>Viene riprodotto quando il pulsante PPP viene rilasciato</translation>
    </message>
    <message>
        <location/>
        <source>Reset audio cue to default</source>
        <translation>Reimposta segnale audio su predefinito</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Reset&lt;/b&gt;&lt;br/&gt;Reset the paths for the files to their default.</source>
        <translation>&lt;b&gt;Reimposta&lt;/b&gt;&lt;br/&gt;Reimposta il percorso dei file al loro valore predefinito.</translation>
    </message>
    <message>
        <source>Reset</source>
        <translation type="obsolete">Reimposta</translation>
    </message>
    <message>
        <location/>
        <source>Browse for on audio file</source>
        <translation>Sfoglia le cartelle per impostare un file sonoro per On</translation>
    </message>
    <message>
        <source>Browse</source>
        <translation type="obsolete">Sfoglia</translation>
    </message>
    <message>
        <location/>
        <source>Browse for off audio file</source>
        <translation>Sfoglia le cartelle per impostare un file sonoro per Off</translation>
    </message>
    <message>
        <location/>
        <source>Off</source>
        <translation>Off</translation>
    </message>
    <message>
        <location/>
        <source>On</source>
        <translation>On</translation>
    </message>
    <message>
        <location/>
        <source>Preview the audio cues</source>
        <translation>Riproduce un&apos;anteprima del segnale sonoro</translation>
    </message>
    <message>
        <source>&lt;b&gt;Preview&lt;/b&gt;&lt;br/&gt;Plays the current &lt;i&gt;on&lt;/i&gt; soundfile followed by the current &lt;i&gt;off&lt;/i&gt; soundfile.</source>
        <translation type="obsolete">&lt;b&gt;Anteprima&lt;/b&gt;&lt;br/&gt;Riproduce un&apos;anteprima dei file attuali per &lt;i&gt;on&lt;/i&gt; e di seguito&lt;i&gt;off&lt;/i&gt;.</translation>
    </message>
    <message>
        <source>Preview</source>
        <translation type="obsolete">Anteprima</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This enables the audio cues for push to talk.&lt;/b&gt;&lt;br /&gt;Setting this will give you a short audio beep when push to talk is pressed and released.</source>
        <translation>&lt;b&gt;Abilita la segnalazione audio per il premi per parlare.&lt;/b&gt;&lt;br /&gt;Attivandolo sentirete un breve segnale acustico quando il PPP viene premuto e rilasciato.</translation>
    </message>
    <message>
        <location/>
        <source>PTT Audio cue</source>
        <translation>Segnale audio per PPP</translation>
    </message>
    <message>
        <location/>
        <source>Use SNR based speech detection</source>
        <translation>Usa il riconoscimento della voce basato su SNR</translation>
    </message>
    <message>
        <location/>
        <source>Signal to Noise</source>
        <translation>Rapporto segnale/rumore</translation>
    </message>
    <message>
        <source>Use Amplitude based speech detection.</source>
        <translation type="obsolete">Usa riconoscimento della voce basato sull&apos;Ampiezza.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets speech detection to use Amplitude.&lt;/b&gt;&lt;br /&gt;In this mode, the raw strength of the input signal is used to detect speech.</source>
        <translation>&lt;b&gt;Imposta l&apos;uso dell&apos;ampiezza per il rilevamento della voce.&lt;/b&gt;&lt;br /&gt;In questa modalità, il livello del segnale d&apos;ingresso viene utilizzato per rilevare la voce.</translation>
    </message>
    <message>
        <location/>
        <source>Amplitude</source>
        <translation>Ampiezza</translation>
    </message>
    <message>
        <location/>
        <source>Voice &amp;Hold</source>
        <translation>Trasmetti &amp;per</translation>
    </message>
    <message>
        <location/>
        <source>How long to keep transmitting after silence</source>
        <translation>Per quanto tempo continua a trasmettere dopo l&apos;inizio del silenzio</translation>
    </message>
    <message>
        <source>&lt;b&gt;This selects how long after a perceived stop in speech transmission should continue.&lt;/b&gt;&lt;br /&gt;Set this higher if your voice breaks up when you speak (seen by a rapidly blinking voice icon next to your name).&lt;br /&gt;Only makes sense when used with Voice Activity transmission.</source>
        <translation type="obsolete">&lt;b&gt;Imposta per quanto tempo dopo un arresto del discorso la trasmissione deve continuare.&lt;/b&gt;&lt;br /&gt;Aumentalo se la tua voce viene tagliata quando parli (si vedi un lampeggio rapido dell&apos;icona voce accanto al tuo nome).&lt;br /&gt;Ha senso impostarlo solo quando viene utilizzata la trasmissione ad Attività Vocale.</translation>
    </message>
    <message>
        <location/>
        <source>Silence Below</source>
        <translation>Sottofondo</translation>
    </message>
    <message>
        <source>Signal values below this counts as silence</source>
        <translation type="obsolete">Valore del Segnale sotto il quale conta come silenzio</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the trigger values for voice detection.&lt;/b&gt;&lt;br /&gt;Use this together with the Audio Statistics window to manually tune the trigger values for detecting speech. Input values below &quot;Silence Below&quot; always count as silence. Values above &quot;Speech Above&quot; always count as voice. Values in between will count as voice if you&apos;re already talking, but will not trigger a new detection.</source>
        <translation>&lt;b&gt;Questo cursore imposta il valore di soglia per il rilevamento del discorso.&lt;/b&gt;&lt;br /&gt;Usa questo insieme alla finestra delle statistiche audio per sintonizzare manualmente il cursore per i valori di soglia per l&apos;individuazione del discorso. un segnale al di sotto del cursore &quot;Sottofondo&quot; (zona rossa) viene sempre considerato come silenzio. Un segnale al di sopra del cursore &quot;Voce&quot; (zona gialla) viene considerato come discorso. Un segnale compreso tra i due cursori conterà come discorso solo se prima il valore  del segnale in ingresso è andato nella zona verde, ma non farà iniziare una nuova trasmissione.</translation>
    </message>
    <message>
        <location/>
        <source>Speech Above</source>
        <translation>Voce</translation>
    </message>
    <message>
        <location/>
        <source>Signal values above this count as voice</source>
        <translation>Valore del segnale sopra il quale l&apos;ingresso conta come discorso</translation>
    </message>
    <message>
        <location/>
        <source>Compression</source>
        <translation>Compressione</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Quality</source>
        <translation>&amp;Qualità</translation>
    </message>
    <message>
        <location/>
        <source>Quality of compression (peak bandwidth)</source>
        <translation>Qualità di compressione (picco di banda)</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the quality of compression.&lt;/b&gt;&lt;br /&gt;This determines how much bandwidth Mumble is allowed to use for outgoing audio.</source>
        <translation>&lt;b&gt;Questo imposterà la qualità della compressione.&lt;/b&gt;&lt;br /&gt;Questo determinerà quanta banda Mumble sarà autorizzato ad usare per l&apos;audio in uscita.</translation>
    </message>
    <message>
        <location/>
        <source>Audio per packet</source>
        <translation>Audio per pacchetto</translation>
    </message>
    <message>
        <location/>
        <source>How many audio frames to send per packet</source>
        <translation>Quante parti dell&apos;audio mandare per pacchetto</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This selects how many audio frames should be put in one packet.&lt;/b&gt;&lt;br /&gt;Increasing this will increase the latency of your voice, but will also reduce bandwidth requirements.</source>
        <translation>&lt;b&gt;Questo seleziona quante parti dell&apos;audio saranno messe in un pacchetto&lt;/b&gt;&lt;br /&gt;Incrementando questo sarà incrementata la latenza della tua voce, ma verrà ridotta la banda richiesta.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This shows peak outgoing bandwidth used.&lt;/b&gt;&lt;br /&gt;This shows the peak amount of bandwidth sent out from your machine. Audio bitrate is the maximum bitrate (as we use VBR) for the audio data alone. Position is the bitrate used for positional information. Overhead is our framing and the IP packet headers (IP and UDP is 75% of this overhead).</source>
        <translation>&lt;b&gt; Questo mostra il picco della larghezza di banda utilizzata in uscita.&lt;/b&gt;&lt;br /&gt; Questo mostra la quantità del picco della larghezza di banda inviata dal tuo computer. Bitrate audio è il massimo bitrate (come si usa VBR) per i dati audio da soli. Posizione è il bitrate utilizzato per le informazioni sulla posizione. Carico è l&apos;inquadratura e l&apos;header del pacchetto IP (IP e UDP sono il 75% del carico).</translation>
    </message>
    <message>
        <location/>
        <source>Audio Processing</source>
        <translation>Elaborazione Audio</translation>
    </message>
    <message>
        <location/>
        <source>Noise Suppression</source>
        <translation>Riduzione Rumore</translation>
    </message>
    <message>
        <location/>
        <source>Noise suppression</source>
        <translation>Riduzione rumore</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the amount of noise suppression to apply.&lt;/b&gt;&lt;br /&gt;The higher this value, the more aggressively stationary noise will be suppressed.</source>
        <translation>&lt;b&gt;Questo imposta la quantità di rumore da eliminare.&lt;b&gt;&lt;br /&gt;Più alto sarà questo valore e più rumore fisso sarà eliminato.</translation>
    </message>
    <message>
        <location/>
        <source>Amplification</source>
        <translation>Amplificazione</translation>
    </message>
    <message>
        <location/>
        <source>Maximum amplification of input sound</source>
        <translation>Massima amplificazione del segnale di ingresso</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Maximum amplification of input.&lt;/b&gt;&lt;br /&gt;Mumble normalizes the input volume before compressing, and this sets how much it&apos;s allowed to amplify.&lt;br /&gt;The actual level is continually updated based on your current speech pattern, but it will never go above the level specified here.&lt;br /&gt;If the &lt;i&gt;Microphone loudness&lt;/i&gt; level of the audio statistics hover around 100%, you probably want to set this to 2.0 or so, but if, like most people, you are unable to reach 100%, set this to something much higher.&lt;br /&gt;Ideally, set it so &lt;i&gt;Microphone Loudness * Amplification Factor &gt;= 100&lt;/i&gt;, even when you&apos;re speaking really soft.&lt;br /&gt;&lt;br /&gt;Note that there is no harm in setting this to maximum, but Mumble will start picking up other conversations if you leave it to auto-tune to that level.</source>
        <translation>&lt;b&gt;Massima amplificazione dell&apos;ingresso.&lt;/b&gt;&lt;br /&gt; Mumble normalizza il volume in ingresso prima della compressione, e questo imposta quanto gli è consentito amplificare.&lt;br /&gt;Il livello attuale è continuamente aggiornato in base al suo attuale modello di intervento, ma non potrà mai andare al di sopra del livello indicato qui.&lt;br /&gt;Se le statistiche dell livello del &lt;i&gt;Volume del microfono&lt;/i&gt; gravitano attorno al 100%, probabilmente si desidera impostare questo al 2,0 o piú, ma se , come la maggior parte delle persone, non si riesce a raggiungere il 100%, impostalo molto al di sopra.&lt;br /&gt;Idealmente, impostarlo in modo che &lt;i&gt;Volume del microfono x Fattore di amplificazione&gt;= 100&lt;/i&gt;, anche quando stai parlando piano.&lt;br /&gt;&lt;br /&gt;Si noti che non vi è nulla di male nel mettere questa impostazione al massimo, ma Mumble inizierà raccogliendo le altre conversazioni se lasciato per l&apos;auto-tune a quel livello.</translation>
    </message>
    <message>
        <location/>
        <source>Current speech detection chance</source>
        <translation>Probabilità attuale di riconoscimento della voce</translation>
    </message>
    <message>
        <location/>
        <source>Input device for audio</source>
        <translation>Dispositivo di ingresso per il suono</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is the input device to use for audio.&lt;/b&gt;</source>
        <translation>&lt;b&gt;Questo è il dispositivo d&apos;ingresso da usare per il suono.&lt;/b&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Cancel echo from speakers</source>
        <translation>Cancella l&apos;eco dalle casse</translation>
    </message>
    <message>
        <location/>
        <source>Enabling this will cancel the echo from your speakers. Mixed has low CPU impact, but only works well if your speakers are equally loud and equidistant from the microphone. Multichannel echo cancellation provides much better echo cancellation, but at a higher CPU cost.</source>
        <translation>Abilitando questo si cancellerà l&apos;eco dalle casse/cuffie. La modalità &apos;Misto&apos; ha un impatto minore sulla CPU, ma funziona meglio se le tue casse/cuffie hanno un livello sonoro uguale e se sono equidistanti dal microfono. L&apos;eliminazione eco multicanale fornisce invece una cancellazione eco migliore, ma con un maggior utilizzo della CPU.</translation>
    </message>
    <message>
        <location/>
        <source>Disabled</source>
        <translation>Disabilitato</translation>
    </message>
    <message>
        <location/>
        <source>Mixed</source>
        <translation>Misto</translation>
    </message>
    <message>
        <location/>
        <source>Multichannel</source>
        <translation>Multicanale</translation>
    </message>
    <message>
        <location/>
        <source>Echo</source>
        <translation>Canc. Eco</translation>
    </message>
    <message>
        <location/>
        <source>Audible audio cue when push-to-talk is activated or deactivated</source>
        <translation>Barra dell&apos;audio udibile quando premi per parlare è attivato o disattivato</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets speech detection to use Signal to Noise ratio.&lt;/b&gt;&lt;br /&gt;In this mode, the input is analyzed for something resembling a clear signal, and the clarity of that signal is used to trigger speech detection.</source>
        <translation>&lt;b&gt;Questo imposta l&apos;individuazione del discorso con il rapporto segnale/rumore.&lt;/b&gt;&lt;br /&gt;In questa modalità, l&apos;ingresso è analizzato per trovare qualcosa di simile a un chiaro segnale, e la chiarezza del segnale viene utilizzata per attivare il rilevamento del discorso.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This shows the current speech detection settings.&lt;/b&gt;&lt;br /&gt;You can change the settings from the Settings dialog or from the Audio Wizard.</source>
        <translation>&lt;b&gt;Questo visualizza le impostazioni correnti per il riconoscimento del parlato.&lt;/b&gt;&lt;br /&gt;Puoi cambiare le impostazioni dalla finestra delle impostazioni o dall&apos;Audio Wizard.</translation>
    </message>
    <message>
        <source>Idle AutoMute</source>
        <translation type="obsolete">AutoMuto quando Inattivo</translation>
    </message>
    <message>
        <source>How long to remain idle before auto-mute.</source>
        <translation type="obsolete">Per quanto tempo devi rimanere inattivo prima dell&apos;auto-muto.</translation>
    </message>
    <message>
        <source>This sets an idle timer. If the timer expires without any audio being sent to the server, you are muted.</source>
        <translation type="obsolete">Stabilisce un timer di inattività. Se il timer scade senza che nessun audio venga inviato al server, il tuo microfono verrà disattivato.</translation>
    </message>
    <message>
        <location/>
        <source>Signal values below this count as silence</source>
        <translation>Valore del segnale sotto il quale l&apos;ngresso conta come silenzio</translation>
    </message>
    <message>
        <location/>
        <source>Maximum bandwidth used for sending audio</source>
        <translation>Banda massima usata per inviare l&apos;audio</translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets the amount of noise suppression to apply.&lt;/b&gt;&lt;br /&gt;The higher this value, the more aggressively stationary noise will be supressed.</source>
        <translation type="obsolete">&lt;b&gt;Questo imposta la quantità di rumore da eliminare.&lt;b&gt;&lt;br /&gt;Più alto sarà questo valore e piú rumore sarà eliminato.</translation>
    </message>
    <message>
        <location filename="AudioInput.cpp" line="+520"/>
        <source>Server maximum network bandwidth is only %1 kbit/s. Audio quality auto-adjusted to %2 kbit/s (%3ms)</source>
        <translation>La banda massima del server è solamente %1 kbit/s. Qualità sonora impostata su %2 kbit/s (%3ms)</translation>
    </message>
    <message>
        <location filename="AudioInput.ui"/>
        <source>Use Amplitude based speech detection</source>
        <translation>Usa riconoscimento della voce basato sull&apos;Ampiezza</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This selects how long after a perceived stop in speech transmission should continue.&lt;/b&gt;&lt;br /&gt;Set this higher if your voice breaks up when you speak (seen by a rapidly blinking voice icon next to your name).</source>
        <translation>&lt;b&gt;Imposta per quanto tempo dopo la fine del discorso la trasmissione deve continuare.&lt;/b&gt;&lt;br /&gt;Aumentalo se la tua voce viene tagliata mentre stai parlando (si vedi un lampeggio rapido dell&apos;icona voce accanto al tuo nome).</translation>
    </message>
    <message>
        <location/>
        <source>Idle AutoDeafen</source>
        <translation>Disattivazione automatica</translation>
    </message>
    <message>
        <location/>
        <source>How long to remain idle before auto-deafen.</source>
        <translation>Quanto dovrà rimanere inattivo prima della disattivazione automatica.</translation>
    </message>
    <message>
        <location/>
        <source>This sets an idle timer. If the timer expires without any audio being sent to the server, you are muted and deafened.</source>
        <translation>Imposta un timer di inattività. Se durante questo periodo di tempo non viene inviato nessun suono al server, ti verrà disattivato l&apos;audio.</translation>
    </message>
    <message>
        <location/>
        <source>Exclusive mode</source>
        <translation>Modalità esclusiva</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This opens the device in exclusive mode.&lt;/b&gt;&lt;br /&gt;No other application will be able to use the device.</source>
        <translation>&lt;b&gt; Utilizza il dispositivo in modalità esclusiva.&lt;/b&gt;&lt;br /&gt;Le altre applicazioni non potranno utilizzare questo dispositivo.</translation>
    </message>
    <message>
        <location/>
        <source>Exclusive</source>
        <translation>Esclusivo</translation>
    </message>
    <message>
        <location/>
        <source>R&amp;eset</source>
        <translation>&amp;Reset</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Browse...</source>
        <translation>&amp;Sfoglia...</translation>
    </message>
    <message>
        <location/>
        <source>B&amp;rowse...</source>
        <translation>S&amp;foglia...</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Preview</source>
        <translation>&amp;Anteprima</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Preview&lt;/b&gt;&lt;br/&gt;Plays the current &lt;i&gt;on&lt;/i&gt; sound followed by the current &lt;i&gt;off&lt;/i&gt; sound.</source>
        <translation>&lt;b&gt;Anteprima&lt;/b&gt;&lt;br/&gt;Riproduce un&apos;anteprima dei file attuali per &lt;i&gt;on&lt;/i&gt; e di seguito&lt;i&gt;off&lt;/i&gt;.</translation>
    </message>
</context>
<context>
    <name>AudioInputDialog</name>
    <message>
        <location filename="AudioConfigDialog.cpp" line="+71"/>
        <source>Continuous</source>
        <translation>Continuo</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Voice Activity</source>
        <translation>Attività vocale</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Push To Talk</source>
        <translation>Premi Per Parlare</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Audio Input</source>
        <translation>Ingresso audio</translation>
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
        <translation>Off</translation>
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
        <translation>%1kbit/s (Audio %2 %5, Posizione %4, Sovrapposizione %3)</translation>
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
        <translation type="obsolete">%1kbit/s (Audio %2, Posizione %4, Sovraccarico %3)</translation>
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
        <translation>Modulo</translation>
    </message>
    <message>
        <location/>
        <source>Interface</source>
        <translation>Interfaccia</translation>
    </message>
    <message>
        <location/>
        <source>System</source>
        <translation>Sistema</translation>
    </message>
    <message>
        <source>Input method for audio</source>
        <translation type="obsolete">Metodo di ingresso per l&apos;audio</translation>
    </message>
    <message>
        <source>&lt;b&gt;This is the input method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation type="obsolete">&lt;b&gt;Questo è il metodo di input da utilizzare per l&apos;audio.&lt;/b&gt;&lt;br /&gt;Molto probabilmente vorrai utilizzare DirectSound.</translation>
    </message>
    <message>
        <location/>
        <source>Device</source>
        <translation>Dispositivo</translation>
    </message>
    <message>
        <location/>
        <source>Output method for audio</source>
        <translation>Metodo d&apos;uscita per l&apos;audio</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is the output method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation>&lt;b&gt;Questo è il sistema d&apos;uscita da utilizzare per l&apos;audio.&lt;/b&gt;&lt;br /&gt;Molto probabilmente vorrai utilizzare DirectSound.</translation>
    </message>
    <message>
        <location/>
        <source>Output device for audio</source>
        <translation>Dispositivo sonoro di uscita</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is the output device to use for audio.&lt;/b&gt;</source>
        <translation>&lt;b&gt;Questo è il dispositivo di uscita da usare per il suono.&lt;/b&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Positional Audio</source>
        <translation>Audio posizionale</translation>
    </message>
    <message>
        <location/>
        <source>Audio Output</source>
        <translation>Uscita audio</translation>
    </message>
    <message>
        <location/>
        <source>Default &amp;Jitter Buffer</source>
        <translation>Spazio &amp;buffer predefinito</translation>
    </message>
    <message>
        <location/>
        <source>Safety margin for jitter buffer</source>
        <translation>Margine di sicurezza per lo spazio del buffer</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the minimum safety margin for the jitter buffer.&lt;/b&gt;&lt;br /&gt;All incoming audio is buffered, and the jitter buffer continually tries to push the buffer to the minimum sustainable by your network, so latency can be as low as possible. This sets the minimum buffer size to use. If the start of sentences you hear is very jittery, increase this value.</source>
        <translation>&lt;b&gt;Imposta il minimo margine di sicurezza per lo spazio del buffer.&lt;/b&gt;&lt;br /&gt;Tutto l&apos;audio in ingresso è messo nel buffer, e il limite dello spazio del buffer cerca continuamente di tenere il buffer al minimo sostenibile dalla tua rete, in modo da avere una latenza quanto piú bassa possibile. Imposta la dimensione minima del buffer da utilizzare. Se all&apos;inizio delle frasi c&apos;è un rumore distorto, aumenta questo valore.</translation>
    </message>
    <message>
        <source>TextLabel</source>
        <translation type="obsolete">Etichetta testo</translation>
    </message>
    <message>
        <location/>
        <source>Volume</source>
        <translation>Volume</translation>
    </message>
    <message>
        <location/>
        <source>Volume of incoming speech</source>
        <translation>Volume per la voce in ingresso</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This adjusts the volume of incoming speech.&lt;/b&gt;&lt;br /&gt;Note that if you increase this beyond 100%, audio will be distorted.</source>
        <translation>&lt;b&gt;Regola il volume di ingresso della voce.&lt;/b&gt;&lt;br /&gt;Si noti che se si aumenta oltre il 100%, l&apos;audio verrà distorto.</translation>
    </message>
    <message>
        <location/>
        <source>Output Delay</source>
        <translation>Ritardo d&apos;uscita</translation>
    </message>
    <message>
        <location/>
        <source>Amount of data to buffer</source>
        <translation>Quantità di dati da usare come buffer</translation>
    </message>
    <message>
        <source>This sets the amount of data to prebuffer in the output buffer. Experiment with different values and set it to the lowest which doesn&apos;t cause rapid jitter in the sound.</source>
        <translation type="obsolete">Imposta la quantità di dati da prebufferizzare nel buffer d&apos;uscita. Sperimentare diversi valori e tenere al minimo che non causa un jitter veloce nel suono.</translation>
    </message>
    <message>
        <location/>
        <source>The connected &quot;speakers&quot; are actually headphones</source>
        <oldsource>The connected &quot;speakers&quot; are actually headphones.</oldsource>
        <translation>Gli altoparlanti attualmente connessi sono delle cuffie</translation>
    </message>
    <message>
        <source>This sets the minimum distance for sound calculations. The volume of other players&apos; speech will not decrease until they are at least this far away from you.</source>
        <translation type="obsolete">Imposta la distanza minima per i calcoli sonori. Il volume della voce degli altri giocatori non diminuirà fino a che non saranno almeno cosí lontani da voi.</translation>
    </message>
    <message>
        <location/>
        <source>Factor for sound volume decrease</source>
        <translation>Fattore di decremento per il volume dei suoni</translation>
    </message>
    <message>
        <location/>
        <source>Bloom</source>
        <translation>Bloom</translation>
    </message>
    <message>
        <location/>
        <source>Factor for sound volume increase</source>
        <translation>Fattore di incremento del volume sonoro</translation>
    </message>
    <message>
        <location/>
        <source>How much should sound volume increase for sources that are really close?</source>
        <translation>Quanto dovrebbe aumentare il volume del suono per le sorgenti che sono molto vicine?</translation>
    </message>
    <message>
        <location/>
        <source>Checking this indicates that you don&apos;t have speakers connected, just headphones. This is important, as speakers are usually in front of you, while headphones are directly to your left/right.</source>
        <translation>Se impostato indica che non sono collegati degli altoparlanti, ma solo le cuffie. Questo è importante, in quanto gli altoparlanti solitamente sono di fronte a voi, mentre le cuffie sono direttamente a sinistra/destra.</translation>
    </message>
    <message>
        <location/>
        <source>This sets the amount of data to pre-buffer in the output buffer. Experiment with different values and set it to the lowest which doesn&apos;t cause rapid jitter in the sound.</source>
        <translation>Imposta la quantità di dati da tenere nel pre-buffer per il buffer d&apos;uscita. Sperimentare diversi valori e tenere al valore minimo che non causa una distorsione veloce del suono.</translation>
    </message>
    <message>
        <location/>
        <source>Headphones</source>
        <translation>Cuffie</translation>
    </message>
    <message>
        <location/>
        <source>Minimum Distance</source>
        <translation>Distanza minima</translation>
    </message>
    <message>
        <source>Minimum distance to player before sound volume decreases</source>
        <translation type="obsolete">Distanza minima dal giocatore prima che il volume del suono decresca</translation>
    </message>
    <message>
        <location/>
        <source>Maximum Distance</source>
        <translation>Distanza massima</translation>
    </message>
    <message>
        <location/>
        <source>Maximum distance, beyond which speech volume won&apos;t decrease</source>
        <translation>Massima distanza, dopo la quale il volume del discorso non decrescerà piú</translation>
    </message>
    <message>
        <source>This sets the maximum distance for sound calculations. When farther away than this, other players&apos; speech volume will not decrease any further.</source>
        <translation type="obsolete">Questo imposta la massima distanza per il calcolo del suono. Quando saranno piú lontano di cosí il volume dei discorsi degli altri giocatori non decrescerà piú.</translation>
    </message>
    <message>
        <location/>
        <source>Minimum distance to user before sound volume decreases</source>
        <translation>Distanza minima dall&apos;utente prima che il volume del suono inizi a decrescere</translation>
    </message>
    <message>
        <location/>
        <source>This sets the minimum distance for sound calculations. The volume of other users&apos; speech will not decrease until they are at least this far away from you.</source>
        <translation>Imposta la distanza minima per i calcoli sonori. Il volume della voce degli altri utenti non diminuirà fino a che non saranno almeno cosí lontani da voi.</translation>
    </message>
    <message>
        <location/>
        <source>This sets the maximum distance for sound calculations. When farther away than this, other users&apos; speech volume will not decrease any further.</source>
        <translation>Questo imposta la massima distanza per il calcolo del suono. Quando saranno piú lontano di cosí il volume dei discorsi degli altri utenti non decrescerà piú.</translation>
    </message>
    <message>
        <location/>
        <source>Minimum Volume</source>
        <translation>Volume minimo</translation>
    </message>
    <message>
        <location/>
        <source>What should the volume be at the maximum distance?</source>
        <translation>Quale dovrebbe essere il volume alla massima distanza?</translation>
    </message>
    <message>
        <location/>
        <source>Loopback Test</source>
        <translation>Test di Ricircolo</translation>
    </message>
    <message>
        <location/>
        <source>Delay Variance</source>
        <translation>Varianza del Ritardo</translation>
    </message>
    <message>
        <location/>
        <source>Variance in packet latency</source>
        <translation>Varianza della latenza dei pacchetti</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the packet latency variance for loopback testing.&lt;/b&gt;&lt;br /&gt;Most audio paths contain some variable latency. This allows you to set that variance for loopback mode testing. For example, if you set this to 15ms, this will emulate a network with 20-35ms ping latency or one with 80-95ms latency. Most domestic net connections have a variance of about 5ms.</source>
        <oldsource>&lt;b&gt;This sets the packet latency variance for loopback testing.&lt;/b&gt;&lt;br /&gt;Most audio paths contain some variable latency. This allows you set that variance for loopback mode testing. For example, if you set this to 15ms, this will emulate a network with 20-35ms ping latency or one with 80-95ms latency. Most domestic net connections have a variance of about 5ms.</oldsource>
        <translation>&lt;b&gt;Questo imposta la latenza del pacchetto per il test di ricircolo locale. &lt;/b&gt;&lt;br /&gt; La maggior parte dell&apos;audio contiene alcuni percorsi di latenza variabile. Questo permette di impostare le variabili per la modalità di test di ricircolo. Ad esempio, se si imposta questo a 15ms, questo emula una rete con 20-35ms di latenza oppure con 80-95ms di latenza. La maggior parte delle connessioni di rete domestiche ha una variazione di circa 5ms.</translation>
    </message>
    <message>
        <location/>
        <source>Packet Loss</source>
        <translation>Pacchetti Persi</translation>
    </message>
    <message>
        <location/>
        <source>Packet loss for loopback mode</source>
        <translation>Pacchetti persi in modalità ricircolo</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the packet loss for loopback mode.&lt;/b&gt;&lt;br /&gt;This will be the ratio of packets lost. Unless your outgoing bandwidth is peaked or there&apos;s something wrong with your network connection, this will be 0%</source>
        <translation>&lt;b&gt;Imposta la perdita di pacchetti per la modalità di ricircolo.&lt;/b&gt;&lt;br /&gt;Questo sarà il rapporto dei pacchetti persi. A meno che non ci sono dei picchi di banda in uscita o se c&apos;è qualcosa che non va con la connessione di rete, questo sarà 0%</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Loopback</source>
        <translation>&amp;Ricircolo</translation>
    </message>
    <message>
        <location/>
        <source>Desired loopback mode</source>
        <translation>Modalità ricircolo desiderata</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This enables one of the loopback test modes.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;None&lt;/i&gt; - Loopback disabled&lt;br /&gt;&lt;i&gt;Local&lt;/i&gt; - Emulate a local server.&lt;br /&gt;&lt;i&gt;Server&lt;/i&gt; - Request loopback from server.&lt;br /&gt;Please note than when loopback is enabled, no other users will hear your voice. This setting is not saved on application exit.</source>
        <translation>&lt;b&gt;Consente uno dei modi di test di ricircolo.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;Nessuno&lt;/i&gt; - Ricircolo disattivato&lt;br /&gt;&lt;i&gt;Locale&lt;/i&gt; - Emula un server locale.&lt;br /&gt;&lt;i&gt;Server&lt;/i&gt; - Richiesta di ricircolo al server.&lt;br /&gt;Si prega di notare che quando il ricircolo è abilitato, gli altri utenti non sentiranno la tua voce. Questa impostazione non viene salvata all&apos;uscita dall&apos;applicazione.</translation>
    </message>
    <message>
        <source>&lt;b&gt;This enables one of the loopback testmodes.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;None&lt;/i&gt; - Loopback disabled&lt;br /&gt;&lt;i&gt;Local&lt;/i&gt; - Emulate a local server.&lt;br /&gt;&lt;i&gt;Server&lt;/i&gt; - Request loopback from server.&lt;br /&gt;Please note than when loopback is enabled, no other players will hear your voice. This setting is not saved on application exit.</source>
        <translation type="obsolete">&lt;b&gt;Consente uno dei modi di test di ricircolo.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;Nessuno&lt;/i&gt; - Ricircolo disattivato&lt;br /&gt;&lt;i&gt;Locale&lt;/i&gt; - Emulare un server locale.&lt;br /&gt;&lt;i&gt;Server&lt;/i&gt; - Richiesta di ricircolo al server.&lt;br /&gt;Si prega di notare che quando il ricircolo è abilitato, gli altri giocatori si sentiranno la tua voce. Questa impostazione non viene salvata all&apos;uscita dall&apos;applicazione.</translation>
    </message>
    <message>
        <source>Other Applications</source>
        <translation type="obsolete">Altre applicazioni</translation>
    </message>
    <message>
        <source>Volume of other applications during speech</source>
        <translation type="obsolete">Volume delle altre applicazioni durante il parlato</translation>
    </message>
    <message>
        <source>&lt;b&gt;Decrease volume of other applications during speech.&lt;/b&gt;&lt;br /&gt;Mumble supports decreasing the volume of other applications during incoming speech. This sets the relative volume of other applications when others are talking.</source>
        <translation type="obsolete">&lt;b&gt;Diminuire il volume delle altre applicazioni durante il parlato.&lt;/b&gt;&lt;br /&gt;Mumble supporta la diminuzione del volume delle altre applicazioni durante la ricezione della voce. Imposta il volume relativo delle altre applicazioni quando stanno parlando gli altri.</translation>
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
        <translation>Attenua applicazioni di...</translation>
    </message>
    <message>
        <location/>
        <source>Attenuation of other applications during speech</source>
        <translation>Attenuazione delle altre applicazioni durante il discorso</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Attenuate volume of other applications during speech&lt;/b&gt;&lt;br /&gt;Mumble supports decreasing the volume of other applications during incoming and/or outgoing speech. This sets the attenuation of other applications if the feature is enabled.</source>
        <translation>&lt;b&gt;Attenua volume delle altre applicazioni durante il discorso&lt;/b&gt;&lt;br /&gt;Mumble supporta il decremento del volume delle altre applicazioni durante il discorso in ingresso e/o in uscita. Questo imposta l&apos;attenuazione delle altre applicazioni se la caratteristica è abilitata.</translation>
    </message>
    <message>
        <source>at</source>
        <translation type="obsolete">at</translation>
    </message>
    <message>
        <location/>
        <source>If checked Mumble lowers the volume of other applications while other users talk</source>
        <translation>Se selezionato Mumble diminuisce il volume delle altre applicazioni mentre gli altri utenti parlano</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Attenuate applications while other users talk&lt;/b&gt;&lt;br /&gt;Mumble supports decreasing the volume of other applications during incoming and/or outgoing speech. This makes mumble activate the feature while other users talk to you.</source>
        <translation>&lt;b&gt;Attenua applicazioni mentre gli altri utenti parlano&lt;/b&gt;&lt;br /&gt;Mumble supporta il decremento del volume delle altre applicazioni durante il discorso in ingresso e/o in uscita. Questo imposta l&apos;attenuazione delle altre applicazioni mentre gli altri utenti stanno parlando.</translation>
    </message>
    <message>
        <location/>
        <source>while other users talk</source>
        <translation>mentre gli altri parlano</translation>
    </message>
    <message>
        <location/>
        <source>If checked Mumble lowers the volume of other applications while you talk</source>
        <translation>Se selezionato Mumble diminuisce il volume delle altre applicazioni mentre parli</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Attenuate applications while you talk&lt;/b&gt;&lt;br /&gt;Mumble supports decreasing the volume of other applications during incoming and/or outgoing speech. This makes mumble activate the feature while you talk.</source>
        <translation>&lt;b&gt;Attenua applicazioni mentre stai parlando&lt;/b&gt;&lt;br /&gt;Mumble supporta il decremento del volume delle altre applicazioni durante il discorso in ingresso e/o in uscita. Questo imposta l&apos;attenuazione delle altre applicazioni mentre tu stai parlando.</translation>
    </message>
    <message>
        <location/>
        <source>while you talk</source>
        <translation>mentre tu parli</translation>
    </message>
    <message>
        <source>md</source>
        <translation type="obsolete">md</translation>
    </message>
    <message>
        <source>mv</source>
        <translation type="obsolete">mv</translation>
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
        <translation>Modalità esclusiva, non raccomandata.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This opens the device in exclusive mode.&lt;/b&gt;&lt;br /&gt;No other application will be able to use the device.</source>
        <translation>&lt;b&gt; Utilizza il dispositivo in modalità esclusiva.&lt;/b&gt;&lt;br /&gt;Le altre applicazioni non potranno utilizzare questo dispositivo.</translation>
    </message>
    <message>
        <location/>
        <source>Exclusive</source>
        <translation>Esclusivo</translation>
    </message>
</context>
<context>
    <name>AudioOutputDialog</name>
    <message>
        <location filename="AudioConfigDialog.cpp" line="+165"/>
        <source>None</source>
        <translation>Niente</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Local</source>
        <translation>Locale</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Server</source>
        <translation>Server</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Audio Output</source>
        <translation>Uscita Audio</translation>
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
        <translation>%1 %</translation>
    </message>
    <message>
        <location line="-16"/>
        <location line="+6"/>
        <source>%1 m</source>
        <translation>%1 m</translation>
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
</context>
<context>
    <name>AudioOutputSample</name>
    <message>
        <location filename="AudioOutput.cpp" line="+297"/>
        <source>Choose sound file</source>
        <translation>Scegli il file sonoro</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Invalid sound file</source>
        <translation>File sonoro non valido</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The file &apos;%1&apos; cannot be used by Mumble. Please select a file with a compatible format and encoding.</source>
        <translation>Il file &apos;%1&apos; non può essere usato da Mumble. Per favore seleziona un file con un formato e una codifica compatibile.</translation>
    </message>
</context>
<context>
    <name>AudioStats</name>
    <message>
        <location filename="AudioStats.cpp" line="+405"/>
        <source>&gt;1000 ms</source>
        <oldsource>&gt;1000ms</oldsource>
        <translation>&gt;1000ms</translation>
    </message>
    <message>
        <location filename="AudioStats.ui"/>
        <source>Audio Statistics</source>
        <translation>Statistiche Audio</translation>
    </message>
    <message>
        <location/>
        <source>Input Levels</source>
        <translation>Livelli di Ingresso</translation>
    </message>
    <message>
        <location/>
        <source>Peak microphone level</source>
        <translation>Livello di picco del microfono</translation>
    </message>
    <message>
        <location/>
        <source>Peak power in last frame</source>
        <translation>Picco di potenza nell&apos;ultima parte</translation>
    </message>
    <message>
        <location/>
        <source>This shows the peak power in the last frame (20 ms), and is the same measurement as you would usually find displayed as &quot;input power&quot;. Please disregard this and look at &lt;b&gt;Microphone power&lt;/b&gt; instead, which is much more steady and disregards outliers.</source>
        <translation>Mostra la potenza di picco nell&apos;ultima parte (20 ms), ed è la stessa misurazione che si trova solitamente visualizzata come &quot;potenza in ingresso&quot;. Si prega di ignorare questo aspetto e invece guardare &lt;b&gt;Potenza del microfono&lt;/b&gt;, che è molto più costante e non tiene conto delle anomalie.</translation>
    </message>
    <message>
        <source>TextLabel</source>
        <translation type="obsolete">Etichetta testo</translation>
    </message>
    <message>
        <location/>
        <source>Peak speaker level</source>
        <translation>Livello di picco dell&apos;uscita</translation>
    </message>
    <message>
        <location/>
        <source>This shows the peak power of the speakers in the last frame (20 ms). Unless you are using a multi-channel sampling method (such as ASIO) with speaker channels configured, this will be 0. If you have such a setup configured, and this still shows 0 while you&apos;re playing audio from other programs, your setup is not working.</source>
        <oldsource>This shows the peak power in the last frame (20 ms) of the speakers. Unless you are using a multi-channel sampling method (such as ASIO) with speaker channels configured, this will be 0. If you have such a setup configured, and this still shows 0 while you&apos;re playing audio from other programs, your setup is not working.</oldsource>
        <translation>Questo mostra la potenza di picco dell&apos;uscita nel corso delle ultime parti (20 ms). A meno che non si sta utilizzando un metodo di campionamento multi-canale (come ASIO), questo sarà 0. Se avete configurato una tale impostazione, e questo mostra ancora 0 mentre si sta riproducendo l&apos;audio da altri programmi, la configurazione non funziona.</translation>
    </message>
    <message>
        <location/>
        <source>Peak clean level</source>
        <translation>Livello pulito di picco</translation>
    </message>
    <message>
        <source>This shows the peak power in the last frame (20 ms) after all processing. Ideally, this should be -96 dB when you&apos;re not talking. In reality, a sound studio should see -60 dB, and you should hopefully see somewhere around -20 dB. When you are talking, this should rise to somewhere between -5 and -10 dB.&lt;br /&gt;If you are using echo cancellation, and this rises to more than -15 dB when you&apos;re not talking, your setup is not working, and you&apos;ll annoy other players with echoes.</source>
        <translation type="obsolete">Mostra la potenza di picco nell&apos;ultima trama (20 ms) dopo tutti i processamenti. Idealmente, questa dovrebbe essere -96 dB quando non stai parlando. In realtà, un studio audio dovrebbe vedere -60 dB, e si spera che tu veda qualcosa intorno a -20 dB. Mentre stai parlando, questo dovrebbe salire tra -5 e -10 dB.&lt;br /&gt;Se stai usando la cancellazione dell&apos;eco, e aumenta a piú di -15 dB quando non stai parlando, l&apos;impostazione non funziona, e infastidirai gli altri giocatori con echi.</translation>
    </message>
    <message>
        <location/>
        <source>This shows the peak power in the last frame (20 ms) after all processing. Ideally, this should be -96 dB when you&apos;re not talking. In reality, a sound studio should see -60 dB, and you should hopefully see somewhere around -20 dB. When you are talking, this should rise to somewhere between -5 and -10 dB.&lt;br /&gt;If you are using echo cancellation, and this rises to more than -15 dB when you&apos;re not talking, your setup is not working, and you&apos;ll annoy other users with echoes.</source>
        <translation>Mostra la potenza di picco nell&apos;ultima parte (20 ms) dopo tutti i processi. Idealmente, questa dovrebbe essere -96 dB quando non stai parlando. In realtà, uno studio audio dovrebbe vedere -60 dB, e si spera che tu veda qualcosa intorno a -20 dB. Mentre stai parlando, questo dovrebbe salire tra -5 e -10 dB.&lt;br /&gt;Se stai usando la cancellazione dell&apos;eco, ed è piú alta di -15 dB quando non stai parlando, l&apos;impostazione non funziona, e infastidirai gli altri giocatori con l&apos;eco.</translation>
    </message>
    <message>
        <location/>
        <source>Signal Analysis</source>
        <translation>Analisi del segnale</translation>
    </message>
    <message>
        <location/>
        <source>Microphone power</source>
        <translation>Potenza del microfono</translation>
    </message>
    <message>
        <location/>
        <source>How close the current input level is to ideal</source>
        <translation>Quanto il livello dell&apos;ingresso attuale è vicino a quello ideale</translation>
    </message>
    <message>
        <location/>
        <source>This shows how close your current input volume is to the ideal. To adjust your microphone level, open whatever program you use to adjust the recording volume, and look at the value here while talking.&lt;br /&gt;&lt;b&gt;Talk loud, as you would when you&apos;re upset over getting fragged by a noob.&lt;/b&gt;&lt;br /&gt;Adjust the volume until this value is close to 100%, but make sure it doesn&apos;t go above. If it does go above, you are likely to get clipping in parts of your speech, which will degrade sound quality.</source>
        <translation>Mostra quanto il livello dell&apos;ingresso attuale è vicino a quello ideale. Per regolare il livello del microfono, apri il programma che usi per regolare il volume di registrazione, e guarda il valore mentre stai parlando.&lt;br /&gt;&lt;b&gt;Parla forte, come faresti quando sei spaventato.&lt;/b&gt;&lt;br /&gt;Regola il volume fino a quando questo valore è vicino al 100%, ma assicurarsi che non vada al di sopra. Se andesse sopra, probabilmente taglieresti alcune parti del tuo parlato, che degraderà la qualità del suono.</translation>
    </message>
    <message>
        <location/>
        <source>Signal-To-Noise ratio</source>
        <translation>Rapporto Segnale/Rumore</translation>
    </message>
    <message>
        <location/>
        <source>Signal-To-Noise ratio from the microphone</source>
        <translation>Rapporto Segnale/Rumore dal microfono</translation>
    </message>
    <message>
        <location/>
        <source>This is the Signal-To-Noise Ratio (SNR) of the microphone in the last frame (20 ms). It shows how much clearer the voice is compared to the noise.&lt;br /&gt;If this value is below 1.0, there&apos;s more noise than voice in the signal, and so quality is reduced.&lt;br /&gt;There is no upper limit to this value, but don&apos;t expect to see much above 40-50 without a sound studio.</source>
        <translation>E&apos; il rapporto segnale/rumore (SNR) del microfono nell&apos;ultima parte (20 ms). Mostra quanto è piú forte la voce confrontata con il rumore.&lt;br /&gt;Se questo valore è inferiore a 1.0, c&apos;è piú rumore della voce nel segnale, e quindi la qualità sarà bassa.&lt;br /&gt;Non vi è alcun limite a questo valore, ma non aspettatevi di vederlo molto al di sopra di 40-50 senza essere in uno studio audio.</translation>
    </message>
    <message>
        <location/>
        <source>Speech Probability</source>
        <translation>Probabilità del parlato</translation>
    </message>
    <message>
        <location/>
        <source>Probability of speech</source>
        <translation>Probabilità del parlato</translation>
    </message>
    <message>
        <location/>
        <source>This is the probability that the last frame (20 ms) was speech and not environment noise.&lt;br /&gt;Voice activity transmission depends on this being right. The trick with this is that the middle of a sentence is always detected as speech; the problem is the pauses between words and the start of speech. It&apos;s hard to distinguish a sigh from a word starting with &apos;h&apos;.&lt;br /&gt;If this is in bold font, it means Mumble is currently transmitting (if you&apos;re connected).</source>
        <translation>E&apos; la probabilità che l&apos;ultima parte (20 ms) sia del parlato e non rumore ambientale.&lt;br /&gt;L&apos;attivazione vocale dipende da quanto è corretto questo valore. Il trucco è che la parte centrale di una frase sia sempre individuata come discorso, il problema è nelle pause tra le parole e l&apos;inizio del discorso. E&apos; difficile distinguere un sospiro da una parola che comincia con la &apos;h&apos;.&lt;br /&gt;Se è in grassetto, significa che Mumble è attualmente in trasmissione (se connesso).</translation>
    </message>
    <message>
        <location/>
        <source>Configuration feedback</source>
        <translation>Ritorno di configurazione</translation>
    </message>
    <message>
        <location/>
        <source>Current audio bitrate</source>
        <translation>Banda audio attuale</translation>
    </message>
    <message>
        <location/>
        <source>Bitrate of last frame</source>
        <translation>Banda dell&apos;ultima parte</translation>
    </message>
    <message>
        <source>This is the audio bitrate of the last compressed frame (20 ms), and as such will jump up and down as the VBR adjusts the quality. To adjust the peak bitrate, adjust &lt;b&gt;Compression Complexity&lt;/b&gt; in the Settings dialog.</source>
        <translation type="obsolete">E&apos; il bitrate audio dell&apos;ultima parte compressa (20 ms), e come tale, si muove su e giú mentre il VBR regola la qualità. Per regolare il bitrate di picco, regolare &lt;b&gt;Complessità della Compressione&lt;/b&gt; nella finestra di dialogo Impostazioni.</translation>
    </message>
    <message>
        <location/>
        <source>DoublePush interval</source>
        <translation>Intervallo di DoppiaPressione</translation>
    </message>
    <message>
        <location/>
        <source>Time between last two Push-To-Talk presses</source>
        <translation>Tempo tra le ultime due pressioni del tasto Premi-Per-Parlare</translation>
    </message>
    <message>
        <source>Time between last two PushToTalk presses</source>
        <translation type="obsolete">Intervallo tra le ultime due premute di PremiPerParlare</translation>
    </message>
    <message>
        <location/>
        <source>Speech Detection</source>
        <translation>Riconoscimento del parlato</translation>
    </message>
    <message>
        <location/>
        <source>Current speech detection chance</source>
        <translation>Probabilità attuale di riconoscimento del parlato</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This shows the current speech detection settings.&lt;/b&gt;&lt;br /&gt;You can change the settings from the Settings dialog or from the Audio Wizard.</source>
        <translation>&lt;b&gt;Mostra l&apos;attuale intervento di rilevamento del parlato.&lt;/b&gt;&lt;br /&gt;Puoi cambiare le impostazioni della finestra di dialogo Impostazioni o dall&apos;Audio Wizard.</translation>
    </message>
    <message>
        <location/>
        <source>Signal and noise power spectrum</source>
        <translation>Potenza spettrale del segnale e del rumore</translation>
    </message>
    <message>
        <location/>
        <source>Power spectrum of input signal and noise estimate</source>
        <translation>Potenza spettrale del segnale e del rumore di ingresso stimata</translation>
    </message>
    <message>
        <location/>
        <source>This shows the power spectrum of the current input signal (red line) and the current noise estimate (filled blue).&lt;br /&gt;All amplitudes are multiplied by 30 to show the interesting parts (how much more signal than noise is present in each waveband).&lt;br /&gt;This is probably only of interest if you&apos;re trying to fine-tune noise conditions on your microphone. Under good conditions, there should be just a tiny flutter of blue at the bottom. If the blue is more than halfway up on the graph, you have a seriously noisy environment.</source>
        <translation>Mostra la potenza dello spettro del segnale di ingresso attuale (linea rossa) e l&apos;attuale stima del rumore (area di colore blu).&lt;br /&gt;Tutte le ampiezze sono moltiplicate per 30 per mostrare le parti interessanti (quanto piú il segnale rispetto al rumore è presente in ogni banda di frequenza).&lt;br /&gt;Probabilmente è di interesse solo se si sta cercando di mettere a punto il rumore del microfono. In buone condizioni, ci dovrebbe essere solo una piccola vibrazione di colore blu nella parte inferiore. Se il blu è piú di metà sul grafico, ci si trova in un ambiente molto rumoroso.</translation>
    </message>
    <message>
        <location/>
        <source>Echo Analysis</source>
        <translation>Analisi dell&apos;eco</translation>
    </message>
    <message>
        <location/>
        <source>Weights of the echo canceller</source>
        <translation>Peso del cancellatore di eco</translation>
    </message>
    <message>
        <location/>
        <source>This shows the weights of the echo canceller, with time increasing downwards and frequency increasing to the right.&lt;br /&gt;Ideally, this should be black, indicating no echo exists at all. More commonly, you&apos;ll have one or more horizontal stripes of bluish color representing time delayed echo. You should be able to see the weights updated in real time.&lt;br /&gt;Please note that as long as you have nothing to echo off, you won&apos;t see much useful data here. Play some music and things should stabilize. &lt;br /&gt;You can choose to view the real or imaginary parts of the frequency-domain weights, or alternately the computed modulus and phase. The most useful of these will likely be modulus, which is the amplitude of the echo, and shows you how much of the outgoing signal is being removed at that time step. The other viewing modes are mostly useful to people who want to tune the echo cancellation algorithms.&lt;br /&gt;Please note: If the entire image fluctuates massively while in modulus mode, the echo canceller fails to find any correlation whatsoever between the two input sources (speakers and microphone). Either you have a very long delay on the echo, or one of the input sources is configured wrong.</source>
        <translation>Mostra il peso del cancellatore di eco, con il tempo crescente verso il basso e la frequenza crescente a destra.&lt;br /&gt;In teoria, dovrebbe essere nero, cioè che non esiste eco. Più comunemente, otterrai una o piú strisce orizzontali di colore bluastro che rappresentano l&apos;eco  in ritardo. Si dovrebbe poter vedere i pesi aggiornati in tempo reale.&lt;br /&gt;Notare che finché l&apos;eco è nullo, qui non sarà possibile visualizzare dati molto utili. Riproduci un po&apos; di musica e le cose dovrebbero stabilizzarsi.&lt;br /&gt;Puoi scegliere di visualizzare la parte reale o immaginaria dei pesi nel dominio della frequenza, o alternativamente il modulo e la fase calcolati. Il piú utile di questi sarà probabilmente il modulo, che è l&apos;ampiezza dell&apos;eco, e mostra quanta parte del segnale in uscita è stata rimossa in quel momento. Le altre modalità di visualizzazione sono per lo piú utili per le persone che vogliono affinare gli algoritmi per la cancellazione dell&apos;eco.&lt;br /&gt;Notare: Se l&apos;intera immagine oscilla molto mentre si è in modalità modulo, il cancellatore di echo non sta fallendo nel trovare una correlazione tra le due sorgenti di ingresso (altoparlanti e microfono). O hai un ritardo di eco veramente lungo, o una delle sorgenti di ingresso è configurata male.</translation>
    </message>
    <message>
        <location/>
        <source>This is the audio bitrate of the last compressed frame (20 ms), and as such will jump up and down as the VBR adjusts the quality. The peak bitrate can be adjusted in the Settings dialog.</source>
        <translation>E&apos; il bitrate audio dell&apos;ultima parte compressa (20 ms), e come tale, si muove su e giú mentre il VBR regola la qualità. Puoi regolare il bitrate di picco attraverso la finestra delle Impostazioni.</translation>
    </message>
</context>
<context>
    <name>AudioWizard</name>
    <message>
        <location filename="AudioWizard.ui"/>
        <source>Audio Tuning Wizard</source>
        <translation>Procedura guidata di sintonia dell&apos;audio</translation>
    </message>
    <message>
        <location/>
        <source>Introduction</source>
        <translation>Introduzione</translation>
    </message>
    <message>
        <location/>
        <source>Welcome to the Mumble Audio Wizard</source>
        <translation>Benvenuto nella procedura guidata per l&apos;impostazioni audio di Mumble</translation>
    </message>
    <message>
        <source>This is the audio tuning wizard for Mumble. This will help you correctly set the input levels of your sound card, and also set the correct parameters for sound processing in Mumble.</source>
        <translation type="obsolete">Questo è il wizard di sintonia dell&apos;audio per Mumble, ti aiuterà ad impostare correttamente i livelli di input della tua scheda audio e imposterà anche i parametri corretti per il processo dei suoni su Mumble.</translation>
    </message>
    <message>
        <source>Please be aware that as long as this wizard is active, audio will be looped locally to allow you to listen to it, and no audio will be sent to the server.</source>
        <translation type="obsolete">Finche la procedura sarà attiva, l&apos;audio sarà ricircolato localmente per darti la possibilità di risentirti e non sarà inviato al server.</translation>
    </message>
    <message>
        <location/>
        <source>Finished</source>
        <translation>Finito</translation>
    </message>
    <message>
        <location/>
        <source>Enjoy using Mumble</source>
        <translation>Divertiti usando Mumble</translation>
    </message>
    <message>
        <source>Congratulations. You should now be ready to enjoy a richer sound experience with Mumble.</source>
        <translation type="obsolete">Congratulazioni. Ora dovresti essere pronto a provare una ricca esperienza audio con Mumble.</translation>
    </message>
    <message>
        <location/>
        <source>Device selection</source>
        <translation>Selezione dispositivo</translation>
    </message>
    <message>
        <location/>
        <source>Selecting the input and output device to use with Mumble.</source>
        <translation>Seleziona i dispositivi di input e output da usare con Mumble.</translation>
    </message>
    <message>
        <source>Input device</source>
        <translation type="obsolete">Dispositivo di input</translation>
    </message>
    <message>
        <location/>
        <source>This is the device your microphone is connected to.</source>
        <translation>Questo è il dispositivo a cui è connesso il microfono.</translation>
    </message>
    <message>
        <location/>
        <source>System</source>
        <translation>Sistema</translation>
    </message>
    <message>
        <location/>
        <source>Input method for audio</source>
        <translation>Metodo di ingresso per il suono</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is the input method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation>&lt;b&gt;Questo è il metodo dell&apos;ingresso da usare per il suono.&lt;/b&gt;&lt;br /&gt;è probabile che si desideri utilizzare DirectSound.</translation>
    </message>
    <message>
        <location/>
        <source>Device</source>
        <translation>Dispositivo</translation>
    </message>
    <message>
        <location/>
        <source>Input device to use</source>
        <translation>Dispositivo di ingresso da utilizzare</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Selects which sound card to use for audio input.&lt;/b&gt;</source>
        <translation>&lt;b&gt;Seleziona la scheda audio da usare come ingresso sonoro.&lt;/b&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Cancel echo from headset or speakers</source>
        <translation>Cancella l&apos;eco dalle cuffie o dalle casse</translation>
    </message>
    <message>
        <location/>
        <source>Use echo cancellation</source>
        <translation>Usa cancellazione eco</translation>
    </message>
    <message>
        <source>Cancel echo from headset or speakers.</source>
        <translation type="obsolete">Cancella l&apos;eco delle cuffie o degli altoparlanti.</translation>
    </message>
    <message>
        <location/>
        <source>This enables echo cancellation of outgoing audio, which helps both on speakers and on headsets.</source>
        <translation>Questo abilita l&apos;eliminazione dell&apos;eco per il suono in uscita, che aiuta sia con gli altoparlanti che con le cuffie.</translation>
    </message>
    <message>
        <source>Output device</source>
        <translation type="obsolete">Dispositivo di output</translation>
    </message>
    <message>
        <location/>
        <source>This is the device your speakers or headphones are connected to.</source>
        <translation>Questo è il dispositivo a cui sono connessi gli altoparlanti o le cuffie.</translation>
    </message>
    <message>
        <location/>
        <source>Output method for audio</source>
        <translation>Metodo di uscita per l&apos;audio</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is the Output method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation>&lt;b&gt;Questo è il metodo dell&apos;uscita da usare per l&apos;audio.&lt;/b&gt;&lt;br /&gt;è probabile che si desideri utilizzare DirectSound.</translation>
    </message>
    <message>
        <location/>
        <source>Output device to use</source>
        <translation>Dispositivo di uscita da utilizzare</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Selects which sound card to use for audio Output.&lt;/b&gt;</source>
        <translation>&lt;b&gt;Seleziona la scheda audio da usare come uscita sonora.&lt;/b&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Enable positional audio</source>
        <translation>Abilita audio posizionale</translation>
    </message>
    <message>
        <location/>
        <source>Allows positioning of sound</source>
        <oldsource>Allows positioning of sound.</oldsource>
        <translation>Consente il posizionamento del suono</translation>
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
Questa è la procedura guidata per impostare l&apos;audio di Mumble. Questo ti aiuterà ad impostare correttamente i livelli di ingresso della tua scheda audio, e anche ad impostare i parametri corretti per l&apos;elaborazione del suono su Mumble.
&lt;/p&gt;
&lt;p&gt;
Ricordati che fino a quando la procedura guidata sarà attiva, il suono sarà ricircolato localmente per darti la possibilità di sentirlo, e nessun suono sarà inviato al server.
&lt;/p&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Input Device</source>
        <translation>Dispositivo di ingresso</translation>
    </message>
    <message>
        <location/>
        <source>Output Device</source>
        <translation>Dispositivo di uscita</translation>
    </message>
    <message>
        <location/>
        <source>This allows Mumble to use positional audio to place voices.</source>
        <translation>Questo consente a Mumble di usare l&apos;audio posizionale per posizionare le voci degli altri utenti.</translation>
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
Per mantenere la latenza ad un minimo assoluto, è importante avere come buffer il minimo audio possibile sulla scheda audio. Tuttavia, molte schede audio riportano che esse richiedono molto meno buffer di quello con cui possono effettivamente lavorare , quindi l&apos;unico modo di impostare questo valore è quello di provare, sbagliare e riprovare.
&lt;/p&gt;
&lt;p&gt;
Dovresti sentire un esempio di voce.Imposta il cursore sottostante al minor valore possibile dove &lt;b&gt;NON&lt;/b&gt; ci sono interruzioni o distorsioni nel suono. Ricorda che l&apos;eco locale è disabilitato durante questo test per trovare meglio l&apos;origine audio.
&lt;/p&gt;
</translation>
    </message>
    <message>
        <location/>
        <source>Amount of data to buffer</source>
        <translation>Quantità di dati da inserire nel buffer</translation>
    </message>
    <message>
        <location/>
        <source>This sets the amount of data to pre-buffer in the output buffer. Experiment with different values and set it to the lowest which doesn&apos;t cause rapid jitter in the sound.</source>
        <translation>Questo imposta la quantità di pre-buffer da mettere nel buffer in uscita. Prova con diversi valori e imposta quello  piú basso che non causi rapide distorsioni nel suono.</translation>
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
Apre il pannello di controllo del suono e va alle impostazioni di registrazione. Assicurati che il microfono sia selezionato come input attivo con il massimo volume di registrazione. Se c&apos;è un&apos;opzione da abilitare chiamata &quot;Preamplificazione microfono&quot; assicurati che sia selezionata.
&lt;/p&gt;
&lt;p&gt;
Parla ad alta voce, come quando sei infastidito o eccitato. Poi diminuisci il volume del suono dal pannello di controllo fino a che la barra sotto rimane il piú alto possibile nella zona blu e verde quando stai parlando, ma &lt;b&gt;non&lt;/b&gt; in quella rossa.
&lt;/p&gt;
</translation>
    </message>
    <message>
        <location/>
        <source>Positional Audio</source>
        <translation>Audio posizionale</translation>
    </message>
    <message>
        <location/>
        <source>Adjusting attenuation of positional audio.</source>
        <translation>Aggiustamento dell&apos;attenuazione dell&apos;audio posizionale.</translation>
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
Mumble supporta l&apos;audio posizionale per alcuni giochi, e posizionerà la voce degli altri giocatori relativamente alla sua posizione nel gioco. Il volume sarà dipendente dalla sua posizione e distanza e sarà aumentato o diminuito su un altoparlante o l&apos;altro per simulare la direzione e la distanza degli altri giocatori. L&apos;audio posizionale dipenderà dai tuoi altoparlanti e dalla loro configurazione nel sistema operativo, il test è finito qui.
&lt;/p&gt;
&lt;p&gt;
Il grafico sottostante visualizza la posizione di &lt;font color=&quot;red&quot;&gt;Tu&lt;/font&gt;, gli &lt;font color=&quot;yellow&quot;&gt;Altoparlanti&lt;/font&gt; e &lt;font color=&quot;green&quot;&gt;la fonte mobile dell&apos;audio&lt;/font&gt;come se fosse visto da sopra.Dovresti sentire l&apos;audio muoversi tra gli altoparlanti.
&lt;/p&gt;
</translation>
    </message>
    <message>
        <location/>
        <source>Use headphones instead of speakers</source>
        <translation>Utilizza cuffie invece delle casse</translation>
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
Congratulazioni. Ora dovresti essere pronto a provare una ricca esperienza audio con Mumble.
&lt;/p&gt;
&lt;p&gt;
Mumble è in continuo sviluppo, e gli sviluppatori cercano di concentrarsi sulle caratteristiche di cui beneficiano la maggior parte degli utenti. A tal fine, sostenete Mumble inviando statistiche anonime sulla vostra configurazione agli sviluppatori. Queste statistiche sono essenziali per lo sviluppo futuro, e anche per essere sicuri che lo sviluppo delle caratteristiche che usi non sia tralasciato.
&lt;/p&gt;
</translation>
    </message>
    <message>
        <source>Mumble supports positional audio for some games, and will position the voice of other players relative to their position in game. Depending on their position, the volume of the voice will be changed between the speakers to simulate the direction and distance the other player is at. Such positioning depends on your speaker configuration being correct in your operating system, so a test is done here.</source>
        <translation type="obsolete">Mumble supporta l&apos;audio posizionale per alcuni giochi, e posizionerà la voce dell&apos;altro giocatore relativamente alla sua posizione nel gioco. Il volume sarà dipendente dalla sua posizione e sarà aumentato o diminuito su un altoparlante per simulare la direzione e la distanza dell&apos;altro giocatore. L&apos;audio posizionale dipenderà dai tuoi altoparlanti e dalla loro configurazione nel sistema operativo, il test è finito qui.</translation>
    </message>
    <message>
        <source>The graph below shows the position of &lt;font color=&quot;red&quot;&gt;you&lt;/font&gt;, the &lt;font color=&quot;yellow&quot;&gt;speakers&lt;/font&gt; and a &lt;font color=&quot;green&quot;&gt;moving sound source&lt;/font&gt; as if seen from above. You should hear the audio move between the channels.</source>
        <translation type="obsolete">Il grafico sottostante visualizza la posizione di &lt;font color=&quot;red&quot;&gt;Tu&lt;/font&gt;, gli &lt;font color=&quot;yellow&quot;&gt;Altoparlanti&lt;/font&gt; e &lt;font color=&quot;green&quot;&gt;la fonte mobile dell&apos;audio&lt;/font&gt;come se fosse visto da sopra.Dovresti sentire l&apos;audio muoversi tra gli altoparlanti.</translation>
    </message>
    <message>
        <location/>
        <source>Use headphones</source>
        <translation>Usa cuffie</translation>
    </message>
    <message>
        <source>Use headphones instead of speakers.</source>
        <translation type="obsolete">Usa le cuffie al posto degli altoparlanti.</translation>
    </message>
    <message>
        <location/>
        <source>This ignores the OS speaker configuration and configures the positioning for headphones instead.</source>
        <translation>Questo ignora la configurazione degli altoparlanti dal sistema operativo e invece configura il posizionamento delle cuffie.</translation>
    </message>
    <message>
        <location/>
        <source>Volume tuning</source>
        <translation>Sintonia volume</translation>
    </message>
    <message>
        <location/>
        <source>Tuning microphone hardware volume to optimal settings.</source>
        <translation>Sintonizza il volume dell&apos;hardware del microfono sulle impostazioni ottimali.</translation>
    </message>
    <message>
        <source>Open your sound control panel and go to the recording settings. Make sure the microphone is selected as active input with maximum recording volume. If there&apos;s an option to enable a &quot;Microphone boost&quot; make sure it&apos;s checked.</source>
        <translation type="obsolete">Apre il pannello di controllo del suono e va alle impostazioni di registrazione. Assicurati che il microfono sia selezionato come input attivo con il massimo volume di registrazione. Se c&apos;è un&apos;opzione da abilitare chiamata &quot;Preamplificazione microfono&quot; assicurati che sia selezionata.</translation>
    </message>
    <message>
        <source>Speak loudly, as when you are annoyed or excited. Decrease the volume in the sound control panel until the bar below stays as high as possible in the blue and green but &lt;b&gt;not&lt;/b&gt; the red zone while you speak.</source>
        <translation type="obsolete">Parla ad alta voce, come quando sei infastidito o eccitato. Poi diminuisci il volume del suono dal pannello di controllo fino a che la barra sotto rimane il piú alto possibile nella zona blu e verde quando si parla, ma &lt;b&gt;non&lt;/b&gt; in quella rossa.</translation>
    </message>
    <message>
        <location/>
        <source>Now talk softly, as you would when talking late at night and you don&apos;t want to disturb anyone. Adjust the slider below so that the bar moves into green when you talk, but stays blue while you&apos;re silent.</source>
        <translation>Ora parla dolcemente, come quando stai parlando a tarda notte e non vuoi disturbare nessuno. Regola il cursore sotto in modo che la barra si sposti nella zona verde quando stai parlando, ma rimane nel blu mentre stai in silenzio.</translation>
    </message>
    <message>
        <location/>
        <source>Voice Activity Detection</source>
        <translation>Rilevamento dell&apos;attività della voce</translation>
    </message>
    <message>
        <location/>
        <source>Letting Mumble figure out when you&apos;re talking and when you&apos;re silent.</source>
        <translation>Lascia capire a Mumble quando stai parlando e quando stai in silenzio.</translation>
    </message>
    <message>
        <location/>
        <source>This will help Mumble figure out when you are talking. The first step is selecting which data value to use.</source>
        <translation>Ciò aiuterà Mumble a capire quando si sta parlando. Il primo passo è la scelta dei dati di valore da usare.</translation>
    </message>
    <message>
        <location/>
        <source>Raw amplitude from input</source>
        <translation>Ampiezza grezza dall&apos;input</translation>
    </message>
    <message>
        <location/>
        <source>Signal-To-Noise ratio</source>
        <translation>Rapporto Segnale/Rumore</translation>
    </message>
    <message>
        <location/>
        <source>Next you need to adjust the following slider. The first few utterances you say should end up in the green area (definitive speech). While talking, you should stay inside the yellow (might be speech) and when you&apos;re not talking, everything should be in the red (definitively not speech).</source>
        <oldsource>Next you need to adjust the following two sliders. The first few utterances you say should end up in the green area (definitive speech). While talking, you should stay inside the yellow (might be speech) and when you&apos;re not talking, everything should be in the red (definitively not speech).</oldsource>
        <translation>Ora devi aggiustare i seguenti 2 cursori. Le prime parole pronunciate dovrebbero finire nella zona verde (inizio del discorso). Mentre stai parlando dovresti stare dentro il giallo (discorso) e quando non stai parlando, tutto dovrebbe stare nella zona rossa (sottofondo).</translation>
    </message>
    <message>
        <source>Last, you need to adjust the grace time. Mumble may detect this much non-speech and still keep transmitting before it breaks off. This allows you to catch your breath while speaking.</source>
        <translation type="obsolete">Infine, è necessario regolare il tempo di grazia. Mumble può rilevare che non stai piú parlando e continuare la trasmissione per il tempo di grazia. Questo ti permetterà di riprendere fiato mentre parli senza interrompere la trasmissione.</translation>
    </message>
    <message>
        <location/>
        <source>Device tuning</source>
        <translation>Sintonia dispositivo</translation>
    </message>
    <message>
        <location/>
        <source>Changing hardware output delays to their minimum value.</source>
        <translation>Modifica l&apos;uscita dell&apos;hardware al suo valore minimo.</translation>
    </message>
    <message>
        <source>To keep latency to an absolute minium, it&apos;s important to buffer as little audio as possible on the soundcard. However, many soundcards report that they require a much smaller buffer than what they can actually work with, so the only way to set this value is to try and fail.</source>
        <translation type="obsolete">Per mantenere la latenza a un minimo assoluto, è importante avere come buffer il minimo audio possibile sulla scheda audio. Tuttavia, molte schede audio riportano che esse richiedono molto meno buffer di quello con cui possono effettivamente lavorare , quindi l&apos;unico modo di impostare questo valore è quello di provare, sbagliare e riprovare.</translation>
    </message>
    <message>
        <source>You should hear a single tone that&apos;s changing in frequency. Change the slider below to the lowest value which gives &lt;b&gt;no&lt;/b&gt; interruptions or jitter in the sound. Please note that local echo is disabled during this test to improve audio path recognition.</source>
        <translation type="obsolete">Dovresti sentire un tono singolo che cambia di frequenza.Imposta il cursore sottostante al minor valore possibile dove &lt;b&gt;NON&lt;/b&gt; ci sono interruzioni o distorsioni nel suono. Ricorda che l&apos;eco locale è disabilitato durante questo test per trovare meglio l&apos;origine audio.</translation>
    </message>
    <message>
        <source>Amount of data to buffer.</source>
        <translation type="obsolete">Quantità di dati del buffer.</translation>
    </message>
    <message>
        <source>This sets the amount of data to prebuffer in the output buffer. Experiment with different values and set it to the lowest which doesn&apos;t cause rapid jitter in the sound.</source>
        <translation type="obsolete">Questo imposta la quantità di prebuffer da mettere nel buffer in uscita. Prova con diversi valori e imposta quello  più basso che non causi rapide distorsioni nel suono.</translation>
    </message>
    <message>
        <source>If you position the microphone so it can pick up the speakers or headset, Mumble will measure the total audio path delay in your system; this means the delay from a sample is placed in an outbound buffer until it&apos;s found in a matching incoming buffer.</source>
        <translation type="obsolete">Se si posiziona il microfono in modo che possa raccogliere l&apos;audio dagli altoparlanti o dalle cuffie, Mumble misurerà il ritardo audio nel vostro sistema, questo significa che il ritardo di un campione sarà collocato in un buffer in uscita fino a quando non sarà trovato un buffer corrispondente in arrivo.</translation>
    </message>
    <message>
        <source>%1ms</source>
        <translation type="obsolete">%1ms</translation>
    </message>
    <message>
        <source>Audio path is %1ms long.</source>
        <translation type="obsolete">L&apos;origine dell&apos;audio è lunga %1ms.</translation>
    </message>
    <message>
        <source>Audio path cannot be determined. Input not recognized.</source>
        <translation type="obsolete">Il percorso dell&apos;audio non può essere determinato. input non riconosciuto.</translation>
    </message>
    <message>
        <source>%1 s</source>
        <translation type="obsolete">%1ms</translation>
    </message>
    <message>
        <source>Mumble is under continuous development, and the development team wants to focus on the features that benefit the most users. To this end, Mumble supports submitting anonymous statistics about your configuration to the developers. These statistcs are essential for future development, and also make sure the features you use aren&apos;t deprecated.</source>
        <translation type="obsolete">Mumble è in continuo sviluppo, e gli sviluppatori cercano di concentrarsi sulle caratteristiche di cui beneficiano la maggior parte degli utenti. A tal fine, sostenete Mumble inviando statistiche anonime sulla vostra configurazione agli sviluppatori. Queste statistiche sono essenziali per lo sviluppo futuro, e anche per essere sicuri che le caratteristiche che usi non siano disapprovate.</translation>
    </message>
    <message>
        <location/>
        <source>Submit anonymous statistics to the Mumble project</source>
        <translation>Invia statistiche anonime al progetto Mumble per migliorare il programma</translation>
    </message>
    <message>
        <location/>
        <source>Push To Talk:</source>
        <translation>Premi Per Parlare:</translation>
    </message>
    <message>
        <location/>
        <source>Quality &amp; Notifications</source>
        <translation>Qualità &amp; notifiche</translation>
    </message>
    <message>
        <location/>
        <source>Adjust quality and notification settings.</source>
        <translation>Regola impostazion dii qualità e notifiche.</translation>
    </message>
    <message>
        <location/>
        <source>Quality settings</source>
        <translation>Impostazioni qualità</translation>
    </message>
    <message>
        <location/>
        <source>Low</source>
        <translation>Basso</translation>
    </message>
    <message>
        <location/>
        <source>Balanced</source>
        <translation>Bilanciato</translation>
    </message>
    <message>
        <location/>
        <source>Notification settings</source>
        <translation>Impostazioni notifiche</translation>
    </message>
    <message>
        <location/>
        <source>Use Text-To-Speech to read notifications and messages to you.</source>
        <translation>Utilizza sintesi vocale per leggere le notifiche ed i messaggi diretti a te.</translation>
    </message>
    <message>
        <location/>
        <source>Disable Text-To-Speech and use sounds instead.</source>
        <translation>Disabilita sintesi vocale e utilizza invece i suoni.</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="+306"/>
        <source>%1 ms</source>
        <translation>%1 ms</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui"/>
        <source>Enables attenuation of other applications while users talk to you</source>
        <translation>Abilita l&apos;attenuazione delle altre applicazioni mentre gli utenti ti stanno parlando</translation>
    </message>
    <message>
        <source>Enables attenuation of other applications while users talk to you. This means that as soon someone is starting to speak to you in mumble the sound of all other applications, like audio players, will get attenuated so you can understand them better.</source>
        <translation type="obsolete">Abilita attenuazione delle altre applicazioni mentre gli utenti ti stanno parlando. Questo vuol dire che appena qualcuno inizia a parlarti su Mumble il suono di tutte le altre applicazioni, come lettori audio, sarà attenuato in modo da sentirlo meglio.</translation>
    </message>
    <message>
        <location/>
        <source>Attenuate applications while other users talk</source>
        <translation>Attenua applicazioni mentre gli altri utenti stanno parlando</translation>
    </message>
    <message>
        <location/>
        <source>In this configuration Mumble will use a &lt;b&gt;low amount of bandwidth&lt;/b&gt;. This will inevitably result in high latency and poor quality. Choose this only if your connection cannot handle the other settings. (Speex 16kbit/s, 60ms per packet)</source>
        <translation>In questa configurazione Mumble userà un &lt;b&gt;basso ammontare di banda&lt;/b&gt;. Questo inevitabilmente risulterà in una alta latenza ed una bassa qualità. Selezionalo solo se la tua connessione non può sostenere le altre impostazioni. (Speex 16kbit/s, 60ms per pacchetto)</translation>
    </message>
    <message>
        <location/>
        <source>This is the &lt;b&gt;recommended default&lt;/b&gt; configuration. It provides a good balance between quality, latency, and bandwidth usage. (CELT 40kbit/s, 20ms per packet)</source>
        <translation>Questa è la configurazione &lt;b&gt;predefinita e raccomandata&lt;/b&gt;. Fornisce un buon bilanciamento tra qualità,latenza e utilizzo di banda. (CELT 40kbit/s, 20ms per pacchetto)</translation>
    </message>
    <message>
        <location/>
        <source>High</source>
        <translation>Alto</translation>
    </message>
    <message>
        <location/>
        <source>This configuration is only recommended for use in setups where bandwidth is not an issue, like a LAN. It provides the lowest latency supported by Mumble and &lt;b&gt;high quality&lt;/b&gt;. (CELT 72kbit/s, 10ms per packet)</source>
        <translation>Questa configurazione è raccomandata solamente per un utilizzo in cui la banda non è un problema, come una LAN. Fornisce la più bassa latenza supportata da Mumble e la &lt;b&gt;massima qualità&lt;/b&gt;. (CELT 72kbit/s, 10ms per pacchetto)</translation>
    </message>
    <message>
        <location/>
        <source>Custom</source>
        <translation>Personalizzato</translation>
    </message>
    <message>
        <location/>
        <source>You already set a customized quality configuration in Mumble. Select this setting to keep it.</source>
        <translation>Hai già impostato una configurazione di qualità personalizzata in Mumble. Selezionando questa impostazione la manterrai.</translation>
    </message>
    <message>
        <location/>
        <source>Enables attenuation of other applications while users talk to you. This means that as soon someone starts to speak to you in Mumble, the sound of all other applications (like audio players) will get attenuated so you can hear them more clearly.</source>
        <translation>Abilita attenuazione delle altre applicazioni mentre gli utenti ti stanno parlando. Questo vuol dire che appena qualcuno inizia a parlarti su Mumble il suono di tutte le altre applicazioni, come lettori audio, sarà attenuato in modo da sentire meglio chi ti stà parlando.</translation>
    </message>
    <message>
        <location/>
        <source>Apply some high contrast optimizations for visually impaired users</source>
        <translation>Applica alcune ottimizzazioni per ottenere un contrasto elevato (utile per gli utenti con problemi visivi)</translation>
    </message>
    <message>
        <location/>
        <source>Use high contrast graphics</source>
        <translation>Utilizza grafica con alto contrasto</translation>
    </message>
    <message>
        <location/>
        <source>Keep custom Text-To-Speech settings.</source>
        <translation>Lascia impostazioni predefinite sulla sintesi vocale.</translation>
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
Apre il pannello di controllo del suono e va alle impostazioni di registrazione. Assicurati che il microfono sia selezionato come input attivo con il massimo volume di registrazione. Se c&apos;è un&apos;opzione da abilitare chiamata &quot;Preamplificazione microfono&quot; assicurati che sia selezionata.
&lt;/p&gt;
&lt;p&gt;
Parla ad alta voce, come quando sei infastidito o eccitato. Poi diminuisci il volume del suono dal pannello di controllo fino a che la barra sotto rimane il più alto possibile nella zona a strisce e vuota mentre stai parlando, ma &lt;b&gt;non&lt;/b&gt; in quella crocettata.
&lt;/p&gt;
</translation>
    </message>
    <message>
        <location/>
        <source>Now talk softly, as you would when talking late at night and you don&apos;t want to disturb anyone. Adjust the slider below so that the bar moves into empty zone when you talk, but stays in the striped one while you&apos;re silent.</source>
        <comment>For high contrast mode</comment>
        <translation>Ora parla dolcemente, come quando stai parlando a tarda notte e non vuoi disturbare nessuno. Regola il cursore sotto in modo che la barra si sposti nella zona vuota mentre stai parlando, ma rimane nella zona a strisce mentre stai in silenzio.</translation>
    </message>
    <message>
        <location/>
        <source>Next you need to adjust the following slider. The first few utterances you say should end up in the empty area (definitive speech). While talking, you should stay inside the striped (might be speech) and when you&apos;re not talking, everything should be in the crisscrossed (definitively not speech).</source>
        <comment>For high contrast mode</comment>
        <translation>Ora devi aggiustare i seguenti 2 cursori. Le prime parole pronunciate dovrebbero finire nella zona vuota (inizio del discorso). Mentre stai parlando dovresti stare dentro la zona a strisce (discorso) e quando non stai parlando, tutto dovrebbe stare nella zona crocettata (sottofondo).</translation>
    </message>
</context>
<context>
    <name>BanEditor</name>
    <message>
        <location filename="BanEditor.ui"/>
        <source>Mumble - Edit Bans</source>
        <translation>Mumble - Modifica della lista degli utenti banditi</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Address</source>
        <translation>&amp;Indirizzo IP</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Mask</source>
        <translation>&amp;Maschera</translation>
    </message>
    <message>
        <location/>
        <source>Reason</source>
        <translation>Motivo</translation>
    </message>
    <message>
        <location/>
        <source>Start</source>
        <translation>Inizio</translation>
    </message>
    <message>
        <location/>
        <source>End</source>
        <translation>Fine</translation>
    </message>
    <message>
        <location/>
        <source>User</source>
        <translation>Utente</translation>
    </message>
    <message>
        <location/>
        <source>Hash</source>
        <translation>Hash</translation>
    </message>
    <message>
        <source>0.0.0.0</source>
        <translation type="obsolete">0.0.0.0</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Add</source>
        <translation>&amp;Aggiungi</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Update</source>
        <translation>&amp;Aggiorna</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Remove</source>
        <translation>&amp;Rimuovi</translation>
    </message>
</context>
<context>
    <name>CertView</name>
    <message>
        <location filename="Cert.cpp" line="+50"/>
        <source>Name</source>
        <translation>Nome</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Email</source>
        <translation>Email</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Issuer</source>
        <translation>Emittente</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Expiry Date</source>
        <translation>Data di scadenza</translation>
    </message>
    <message>
        <location line="+34"/>
        <source>(none)</source>
        <translation>(nessuno)</translation>
    </message>
    <message>
        <location line="+11"/>
        <source>Self-signed</source>
        <translation>Autofirmato</translation>
    </message>
</context>
<context>
    <name>CertWizard</name>
    <message>
        <location line="+90"/>
        <source>Resolving domain %1.</source>
        <translation>Risoluzione dominio %1.</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Unable to validate email.&lt;br /&gt;Enter a valid (or blank) email to continue.</source>
        <translation>Impossibile validare email.&lt;br /&gt;Inserisci un&apos;email valida (o lascia in bianco) per continuare.</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>There was an error generating your certificate.&lt;br /&gt;Please try again.</source>
        <translation>C&apos;è stato un errore durante la generazione del tuo certificato.&lt;br /&gt;Riprova.</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Your certificate and key could not be exported to PKCS#12 format. There might be an error in your certificate.</source>
        <translation>La tua chiave e il tuo certificato non possono essere esportati nel formato PKCS#12. Ci potrebbe essere un errore nel tuo certificato.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>The file could not be opened for writing. Please use another file.</source>
        <translation>Il file non può essere aperto per la scrittura. Per favore usa un&apos;altro file.</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>The file could not be written successfully. Please use another file.</source>
        <translation>Il file non può essere scritto. Per favore usa un&apos;altro file.</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>The file could not be opened for reading. Please use another file.</source>
        <translation>Il file non può essere aperto per la lettura. Per favore usa un&apos;altro file.</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>The file is empty or could not be read. Please use another file.</source>
        <translation>Il file è vuoto o non può essere letto. Per favore usa un&apos;altro file.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>The file did not contain a valid certificate and key. Please use another file.</source>
        <translation>Il file non contiene un valido certificato e/o chiave. Per favore usa un&apos;altro file.</translation>
    </message>
    <message>
        <location line="+22"/>
        <source>Select file to export certificate to</source>
        <translation>Seleziona il file dove esportare il certificato</translation>
    </message>
    <message>
        <location line="+37"/>
        <source>Select file to import certificate from</source>
        <translation>Seleziona il file da cui importare il certificato</translation>
    </message>
    <message>
        <location line="+57"/>
        <source>Unable to resolve domain.</source>
        <translation>Impossibile risolvere dominio.</translation>
    </message>
    <message>
        <location line="+43"/>
        <source>Mumble User</source>
        <translation>Utente Mumble</translation>
    </message>
    <message>
        <location filename="main.cpp" line="+425"/>
        <source>&lt;b&gt;Certificate Expiry:&lt;/b&gt; Your certificate is about to expire. You need to renew it, or you will no longer be able to connect to servers you are registered on.</source>
        <translation>&lt;b&gt;Scadenza certificato:&lt;/b&gt; Il tuo certificato sta per scadere. Devi rinnovarlo, o non sarai più in grado di connetterti ai server a cui ti sei registrato.</translation>
    </message>
</context>
<context>
    <name>Certificates</name>
    <message>
        <location filename="Cert.ui"/>
        <source>Certificate Management</source>
        <translation>Gestione del certificato</translation>
    </message>
    <message>
        <source>This opens a file selection dialog to choose a file to export the certificate to.</source>
        <translation type="obsolete">Apre una finestra di dialogo per scegliere il file sul quale esportare il certificato.</translation>
    </message>
    <message>
        <location/>
        <source>Certificate Authentication</source>
        <translation>Autenticazione del certificato</translation>
    </message>
    <message>
        <location/>
        <source>Authenticating to servers without using passwords</source>
        <translation>Autenticazione con il server senza utilizzare password</translation>
    </message>
    <message>
        <source>&lt;p&gt;Mumble can use certificates to authenticate with servers. Using certificates avoids passwords, meaning you don&apos;t need to disclose any password to the remote site. It also enables very easy user registration.&lt;/p&gt;&lt;p&gt;While Mumble can work without certificates, the majority of servers will expect you to have one.&lt;/p&gt;</source>
        <translation type="obsolete">&lt;p&gt;Mumble può usare i certificati per autenticarsi con i server. Usare i certificati evita di dover usare password, vuol dire che non c&apos;è bisogno di inviare password al sito remoto. In questo modo è possibile registrarsi ai server molto più facilmente.&lt;/p&gt;&lt;p&gt;Anche se Mumble può funzionare senza certificati, molti server ne richiedono uno.&lt;/p&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Current certificate</source>
        <translation>Certificato attuale</translation>
    </message>
    <message>
        <location/>
        <source>This is the certificate Mumble currently uses.</source>
        <translation>Questo è il certificato attualmente in uso da Mumble.</translation>
    </message>
    <message>
        <location/>
        <source>Current Certificate</source>
        <translation>Certificato attuale</translation>
    </message>
    <message>
        <location/>
        <source>Create a new certificate</source>
        <translation>Crea un nuovo certificato</translation>
    </message>
    <message>
        <location/>
        <source>This will create a new certificate.</source>
        <translation>Questo creerà un nuovo certificato.</translation>
    </message>
    <message>
        <location/>
        <source>Import certificate from file</source>
        <translation>Importa certificato da un file</translation>
    </message>
    <message>
        <location/>
        <source>This will import a certificate from file.</source>
        <translation>Questo importerà un certificato da un file.</translation>
    </message>
    <message>
        <location/>
        <source>Import a certificate</source>
        <translation>Importa un certificato</translation>
    </message>
    <message>
        <location/>
        <source>Export Certificate</source>
        <translation>Esporta certificato</translation>
    </message>
    <message>
        <location/>
        <source>This will export a certificate to file.</source>
        <translation>Questo esporterà un certificato su un file.</translation>
    </message>
    <message>
        <location/>
        <source>Export current certificate</source>
        <translation>Esporta certificato attuale</translation>
    </message>
    <message>
        <location/>
        <source>Import Certificate</source>
        <translation>Importa certificato</translation>
    </message>
    <message>
        <location/>
        <source>PKCS #12 Certificate import</source>
        <translation>Importa certificato PKCS #12</translation>
    </message>
    <message>
        <location/>
        <source>&lt;p&gt;Mumble can import certificates stored in PKCS #12 format. This is the format used when exporting a key from Mumble, and also when exporting keys from Firefox, Internet Explorer, Opera etc.&lt;/p&gt;&lt;p&gt;If the file is password protected, you will need the password to import the certificate.&lt;/p&gt;</source>
        <oldsource>&lt;p&gt;Mumble can import certificates stored in PKCS #12 format. This is the format used when exporting a key from Mumble, and also when exporting keys from FireFox, Internet Explorer, Opera etc.&lt;/p&gt;&lt;p&gt;If the file is password protected, you will need the password to import the certificate.&lt;/p&gt;</oldsource>
        <translation>&lt;p&gt;Mumble può importare certificati salvati in formato PKCS #12. Questo è il formato usato quando esporti una chiave per Mumble, e anche quando esporti chiavi da FireFox, Internet Explorer, Opera etc.&lt;/p&gt;&lt;p&gt;Se il file è protetto da password, avrai bisogno della password per importare il certificato.&lt;p&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Import from</source>
        <translation>Importa da</translation>
    </message>
    <message>
        <location/>
        <source>Filename to import from</source>
        <translation>Nome del file da cui importare</translation>
    </message>
    <message>
        <location/>
        <source>This is the filename you wish to import a certificate from.</source>
        <translation>Questo è il nome del file da cui vuoi importare il certificato.</translation>
    </message>
    <message>
        <location/>
        <source>Select file to import from</source>
        <translation>Seleziona il file da cui importare</translation>
    </message>
    <message>
        <location/>
        <source>This opens a file selection dialog to choose a file to import a certificate from.</source>
        <translation>Apre una finestra di dialogo per scegliere il file da dove importare il certificato.</translation>
    </message>
    <message>
        <location/>
        <source>Open...</source>
        <translation>Apri...</translation>
    </message>
    <message>
        <location/>
        <source>Password</source>
        <translation>Password</translation>
    </message>
    <message>
        <location/>
        <source>Password for PKCS#12 file</source>
        <translation>Password del file PKCS#12</translation>
    </message>
    <message>
        <location/>
        <source>This is the password for the PKCS#12 file containing your certificate.</source>
        <translation>Questa è la password del file PKCS #12 contenente il tuo certificato.</translation>
    </message>
    <message>
        <location/>
        <source>Certificate to import</source>
        <translation>Certificato da importare</translation>
    </message>
    <message>
        <location/>
        <source>This is the certificate you are importing.</source>
        <translation>Questo è il certificato che stai importando.</translation>
    </message>
    <message>
        <location/>
        <source>Certificate Details</source>
        <translation>Dettagli certificato</translation>
    </message>
    <message>
        <location/>
        <source>Replace Certificate</source>
        <translation>Sostituisci certificato</translation>
    </message>
    <message>
        <location/>
        <source>Replace existing certificate with new certificate?</source>
        <translation>Sostituisci certificato esistente con un nuovo certificato?</translation>
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
        <translation>&lt;p&gt;Hai già un certificato conservato in Mumble, e stai per sostituirlo.&lt;/p&gt;
&lt;p&gt;Se stai aggiornando ad un certificato rilasciato a te da un CA attendibile e l&apos;indirizzo email corrisponde a quello del tuo attuale certificato, questo è completamente sicuro, e i server a cui ti connetterai riconosceranno automaticamente la sicura autorizzazione per il tuo indirizzo email.
&lt;/p&gt;
&lt;p&gt;Se questo non è il caso, il tuo certificato non sarà più riconosciuto da qualsiasi server in cui eri entrato in precedenza. Se non ti sei registrato ancora in nessun server, non hai niente di cui preoccuparti.
&lt;/p&gt;
&lt;p&gt;
Sei sicuro di voler sostituire il tuo certificato?
&lt;/p&gt;
</translation>
    </message>
    <message>
        <location/>
        <source>This is the certificate Mumble currently uses. It will be replaced.</source>
        <translation>Questo è il certificato che Mumble usa attualmente. Sarà sostituito.</translation>
    </message>
    <message>
        <location/>
        <source>New certificate</source>
        <translation>Nuovo certificato</translation>
    </message>
    <message>
        <location/>
        <source>This is the new certificate that will replace the old one.</source>
        <translation>Questo è il nuovo certificato che sostituirà quello vecchio.</translation>
    </message>
    <message>
        <location/>
        <source>New Certificate</source>
        <translation>Nuovo certificato</translation>
    </message>
    <message>
        <location/>
        <source>Make a backup of your certificate</source>
        <translation>Crea un backup del tuo certificato</translation>
    </message>
    <message>
        <source>&lt;p&gt;If you ever lose your current certificate, which will happen if your computer suffers a hardware failure or you reinstall your machine, you will no longer be able to authenticate to any server you are registered on. It is therefore &lt;b&gt;mandatory&lt;/b&gt; that you make a backup of your certificate. We strongly recommend you store this backup on removable storage, such as an USB memory stick.&lt;/p&gt;
&lt;p&gt;Note that this file will not be encrypted, and if anyone gains access to it, they will be able to impersonate you, so take good care of it.&lt;/p&gt;</source>
        <translation type="obsolete">&lt;p&gt;Se mai perderai il tuo certificato attuale, che potrebbe accadere nel caso in cui l&apos;hardware del tuo PC si rompa o se reinstallerai il tuo SO, in questo caso non sarai più in grado di autenticarti a nessuno dei server in cui ti eri registrato. E&apos; quindi &lt;b&gt;necessario&lt;/b&gt; fare una copia di backup del tuo certificato. Noi raccomandiamo fortemente di tenere questo backup in un dispositivo rimovibile, come una memoria USB.&lt;/p&gt;
&lt;p&gt;Nota che questo file non sarà criptato, e se qualcuno ne avrà l&apos;accesso, questo sarà in grado di impersonare te, quindi abbine cura.&lt;/p&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Export to</source>
        <translation>Esporta in</translation>
    </message>
    <message>
        <location/>
        <source>Filename to export to</source>
        <translation>Nome del file su cui esportare</translation>
    </message>
    <message>
        <location/>
        <source>This is the filename you wish to export a certificate to.</source>
        <translation>Questo è il nome del file su cui vuoi esportare il certificato.</translation>
    </message>
    <message>
        <location/>
        <source>Save As...</source>
        <translation>Salva come...</translation>
    </message>
    <message>
        <location/>
        <source>This is the certificate Mumble currently uses. It will be exported.</source>
        <translation>Questo è il certificato che Mumble usa attualmente. Sarà esportato.</translation>
    </message>
    <message>
        <location/>
        <source>Generate a new certificate for strong authentication</source>
        <translation>Genera un nuovo certificato per una sicura autenticazione</translation>
    </message>
    <message>
        <location/>
        <source>&lt;p&gt;Mumble will now generate a strong certificate for authentication to servers.&lt;/p&gt;&lt;p&gt;If you wish, you may provide some additional information to be stored in the certificate, which will be presented to servers when you connect. If you provide a valid email address, you can upgrade to a CA issued email certificate later on, which provides strong identification.&lt;/p&gt;</source>
        <translation>&lt;p&gt;Mumble ora genererà un certificato sicuro per l&apos;autenticazione ai server.&lt;/p&gt;&lt;p&gt;Se desideri, puoi fornire alcune informazioni aggiuntive da salvare nel certificato, che saranno presentate ai server quando ti connetterai. Se fornisci un indirizzo email valido, puoi aggiornare il tuo certificato ad uno migliore che fornisce una migliore identificazione.&lt;/p&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Name</source>
        <translation>Nome</translation>
    </message>
    <message>
        <location/>
        <source>Email</source>
        <translation>Email</translation>
    </message>
    <message>
        <location/>
        <source>Your email address (e.g. johndoe@mumble.info)</source>
        <translation>Il tuo indirizzo email (es. johndoe@mumble.info)</translation>
    </message>
    <message>
        <location/>
        <source>This is your email address. It is strongly recommended to provide a valid email address, as this will allow you to upgrade to a strong certificate without authentication problems.</source>
        <translation>Questo è il tuo indirizzo email. Si raccomanda fortemente di fornire un indirizzo email valido, perchè questo ti permette di avere un certificato sicuro e riconoscibile, quindi senza problemi di autenticazione.</translation>
    </message>
    <message>
        <location/>
        <source>Your name (e.g. John Doe)</source>
        <translation>Tuo nome (es. John Doe)</translation>
    </message>
    <message>
        <location/>
        <source>This is your name, and will be filled out in the certificate. This field is entirely optional.</source>
        <translation>Questo è il tuo nome, e sarà inserito nel tuo certificato. Questo campo non è obbligatorio.</translation>
    </message>
    <message>
        <location/>
        <source>Finish</source>
        <translation>Fine</translation>
    </message>
    <message>
        <location/>
        <source>Certificate-based authentication is ready for use</source>
        <translation>Autenticazione basata sul certificato pronta per l&apos;uso</translation>
    </message>
    <message>
        <location/>
        <source>Enjoy using Mumble with strong authentication.</source>
        <translation>Divertiti utilizzando Mumble con un&apos;autenticazione sicura.</translation>
    </message>
    <message>
        <location/>
        <source>Automatic certificate creation</source>
        <translation>Creazione automatica certificato</translation>
    </message>
    <message>
        <location/>
        <source>&lt;p&gt;Mumble can use certificates to authenticate with servers. Using certificates avoids passwords, meaning you don&apos;t need to disclose any password to the remote site. It also enables very easy user registration.&lt;/p&gt;&lt;p&gt;While Mumble can work without certificates, the majority of servers will expect you to have one.&lt;/p&gt;
&lt;p&gt;
It is &lt;b&gt;strongly&lt;/b&gt; recommended that you &lt;a href=&quot;http://mumble.info/certificate.php&quot;&gt;create a trusted certificate&lt;/a&gt;.
&lt;/p&gt;
</source>
        <translation>&lt;p&gt;Mumble può usare i certificati per autenticarsi con i server. Usare i certificati evita di dover usare password, vuol dire che non c&apos;è bisogno di inviare password al sito remoto. In questo modo è possibile registrarsi ai server molto più facilmente.&lt;/p&gt;&lt;p&gt;Anche se Mumble può funzionare senza certificati, molti server ne richiedono uno.&lt;/p&gt;
&lt;p&gt;
&lt;a href=&quot;http://mumble.info/certificate.php&quot;&gt;Creare un certificato&lt;/a&gt; è &lt;b&gt;vivamente&lt;/b&gt; raccomandato.
&lt;/p&gt;
</translation>
    </message>
    <message>
        <location/>
        <source>&lt;p&gt;If you ever lose your current certificate, which will happen if your computer suffers a hardware failure or you reinstall your machine, you will no longer be able to authenticate to any server you are registered on. It is therefore &lt;b&gt;mandatory&lt;/b&gt; that you make a backup of your certificate. We strongly recommend you store this backup on removable storage, such as a USB flash drive.&lt;/p&gt;
&lt;p&gt;Note that this file will not be encrypted, and if anyone gains access to it, they will be able to impersonate you, so take good care of it.&lt;/p&gt;</source>
        <translation>&lt;p&gt;Se mai perderai il tuo certificato attuale, che potrebbe accadere nel caso in cui l&apos;hardware del tuo PC si rompa o se reinstallerai il tuo SO, in questo caso non sarai più in grado di autenticarti a nessuno dei server in cui ti eri registrato. E&apos; quindi &lt;b&gt;necessario&lt;/b&gt; fare una copia di backup del tuo certificato. Noi raccomandiamo fortemente di tenere questo backup in un dispositivo rimovibile, come una memoria esterna USB.&lt;/p&gt;
&lt;p&gt;Nota che questo file non sarà criptato, e se qualcuno ne avrà l&apos;accesso, questo sarà in grado di impersonare te, quindi abbine cura.&lt;/p&gt;</translation>
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
        <translation>Niente</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Traverse</source>
        <translation>Attraversa</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Enter</source>
        <translation>Entra</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Speak</source>
        <translation>Parla</translation>
    </message>
    <message>
        <source>AltSpeak</source>
        <translation type="obsolete">Stop Parla</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Mute/Deafen</source>
        <translation>Disattiva audio o microfono</translation>
    </message>
    <message>
        <source>Move/Kick</source>
        <translation type="obsolete">Sposta/Espelli</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Make channel</source>
        <translation>Crea canale</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Make temporary</source>
        <translation>Crea canale temporaneo</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Link channel</source>
        <translation>Collega canali</translation>
    </message>
    <message>
        <location line="-87"/>
        <source>This represents no privileges.</source>
        <translation>Non rappresenta privilegi.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>This represents total access to the channel, including the ability to change group and ACL information. This privilege implies all other privileges.</source>
        <translation>Rappresenta il totale controllo sul canale, includendo la possibilità di cambiare gruppo e le informazioni ACL. Questo privilegio implica tutti gli altri privilegi.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>This represents the permission to traverse the channel. If a user is denied this privilege, he will be unable to access this channel and any sub-channels in any way, regardless of other permissions in the sub-channels.</source>
        <translation>Rappresenta il permesso di attraversare il canale. Se a un utente è negato questo privilegio, non potrà accedere al canale e a tutti i sottocanali, indipendentemente dai permessi dei sottocanali.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>This represents the permission to join the channel. If you have a hierarchical channel structure, you might want to give everyone Traverse, but restrict Enter in the root of your hierarchy.</source>
        <translation>Rappresenta il permesso di entrare nel canale. In una struttura gerarchica dei canali, potresti voler dare a tutti l&apos;Attraversamento, ma impedire di Entrare nella radice della gerarchia (il canale più in alto).</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>This represents the permission to speak in a channel. Users without this privilege will be suppressed by the server (seen as muted), and will be unable to speak until they are unmuted by someone with the appropriate privileges.</source>
        <translation>Rappresenta il permesso di parlare in un canale. Un utente senza questo privilegio verrà silenziato dal server (visto come se avesse il microfono disattivato), e non sarà in grado di parlare fino a che non sarà attivato da qualcuno che abbia i diritti sufficienti.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>This represents the permission to whisper to this channel from the outside. This works exactly like the &lt;i&gt;speak&lt;/i&gt; privilege, but applies to packets spoken with the Whisper key held down. This may be used to broadcast to a hierarchy of channels without linking.</source>
        <translation>Rappresenta il permesso di sussurrare in un canale da fuori. Funziona esattamente come il privilegio &lt;i&gt;parla&lt;/i&gt; , ma si applica ai pacchetti trasmessi mentre si stà tenendo premuto il pulsante Sussurra. Può essere usato per trasmettere ad una gerarchia di canali senza collegarli.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>This represents the permission to mute and deafen other users. Once muted, a user will stay muted until he is unmuted by another privileged user or reconnects to the server.</source>
        <translation>Rappresenta il permesso di disattivare il microfono e l&apos;audio degli altri utenti. una volta disattivato il suo microfono , questo rimarrà tale fino a che un&apos;altro utente con gli stessi privilegi non lo riattiverà, oppure dopo una riconnessione al server.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>This represents the permission to move a user to another channel or kick him from the server. To actually move the user, either the moving user must have Move privileges in the destination channel, or the user must normally be allowed to enter the channel. Users with this privilege can move users into channels the target user normally wouldn&apos;t have permission to enter.</source>
        <translation>Rappresenta l&apos;autorizzazione a spostare un utente su un altro canale o ad espellerlo dal server. Per spostare un utente, l&apos;utente che sposta deve avere il privilegio Sposta nel canale di destinazione, o l&apos;utente spostato deve essere autorizzato a entrare nel canale. Gli utenti con questo privilegio possono muovere altri utenti in canali dove normalmente non avrebbero permesso di entrare.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>This represents the permission to make sub-channels. The user making the sub-channel will be added to the admin group of the sub-channel.</source>
        <translation>Rappresenta il permesso di creare sottocanali. L&apos;utente che creerà il canale sarà automaticamente aggiunto al gruppo degli admin di quel sottocanale.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>This represents the permission to make a temporary subchannel. The user making the sub-channel will be added to the admin group of the sub-channel. Temporary channels are not stored and disappear when the last user leaves.</source>
        <translation>Rappresenta il permesso di creare sottocanali temporanei. L&apos;utente che creerà il canale sarà automaticamente aggiunto al gruppo degli admin di quel sottocanale. I canali temporanei non saranno salvati e spariranno quando l&apos;ultimo utente lascerà il canale.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>This represents the permission to link channels. Users in linked channels hear each other, as long as the speaking user has the &lt;i&gt;speak&lt;/i&gt; privilege in the channel of the listener. You need the link privilege in both channels to create a link, but just in either channel to remove it.</source>
        <translation>Rappresenta il permesso di collegare i canali. Gli utenti in canali collegati si ascoltano a vicenda, se l&apos;utente che parla ha il privilegio &lt;i&gt;Parlare&lt;/i&gt; nel canale dell&apos;ascoltatore. È necessario il privilegio di Collegamento in entrambi i canali per poter creare un collegamento, ma in uno qualsiasi dei canali per rimuoverlo.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>This represents the permission to write text messages to other users in this channel.</source>
        <translation>Rappresenta il permesso di scrivere messaggi di testo agli altri utenti di questo canale.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>This represents the permission to forcibly remove users from the server.</source>
        <translation>Rappresenta il permesso di rimuovere gli utenti dal server.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>This represents the permission to permanently remove users from the server.</source>
        <translation>Rappresenta il permesso di rimuovere permanentemente gli utenti dal server.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>This represents the permission to register and unregister users on the server.</source>
        <translation>Rappresenta il permesso di registrare e cancellare gli utenti dal server.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>This represents the permission to register oneself on the server.</source>
        <translation>Rappresenta il permesso di registrarsi da soli sul server.</translation>
    </message>
    <message>
        <location line="+31"/>
        <source>Whisper</source>
        <translation>Sussurra</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Move</source>
        <translation>Sposta</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Text message</source>
        <translation>Messaggio di testo</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Kick</source>
        <translation>Espelli</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Ban</source>
        <translation>Bandisci</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Register User</source>
        <translation>Registra utente</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Register Self</source>
        <translation>Registrati</translation>
    </message>
    <message>
        <source>This represents the permission to speak in a channel with flagged speech. This works exactly like the &lt;i&gt;speak&lt;/i&gt; privilege, but applies to packets spoken with AltPushToTalk held down. This may be used to broadcast to a hierarchy of channels without linking.</source>
        <translation type="obsolete">Rappresenta il permesso di parlare in un canale. Funziona esattamente come il privilegio per &lt;i&gt;parlare&lt;/i&gt;, ma si applica al parlato mentre è tenuto premuto AltPremiPerParlare. Può essere utilizzato per trasmettere a una gerarchia di canali senza collegarli.</translation>
    </message>
    <message>
        <source>This represents the permission to mute and deafen other players. Once muted, a player will stay muted until he is unmuted by another privileged player or reconnects to the server.</source>
        <translation type="obsolete">Rappresenta il permesso di silenziare o escludere altri giocatori. Una volta silenziato, un giocatore non potrà parlare fino a che sarà sbloccato da un altro giocatore con privilegi o se si riconnette al server.</translation>
    </message>
    <message>
        <source>This represents the permission to move a player to another channel or kick him from the server. To actually move the player, either the moving player must have Move/Kick privileges in the destination channel, or the player must normally be allowed to enter the channel. Players with this privilege can move players into channels the target player normally wouldn&apos;t have permission to enter.</source>
        <translation type="obsolete">Rappresenta l&apos;autorizzazione a spostare un giocatore su un altro canale o a espellerlo dal server. Per spostare un giocatore, il giocatore che sposta deve avere il privilegio Sposta/Espelli nel canale di destinazione, o il giocatore che sposta deve essere autorizzato a entrare nel canale. I giocatori con questo privilegio possono muovere altri giocatori in canali dove normalmente non avrebbero permesso di entrare.</translation>
    </message>
    <message>
        <source>This represents the permission to make sub-channels. The player making the sub-channel will be added to the admin group of the sub-channel.</source>
        <translation type="obsolete">Rappresenta il permesso di creare sotto-canali. Il giocatore che crea un sotto-canale verrà aggiunto al gruppo admin del sotto-canale.</translation>
    </message>
    <message>
        <source>This represents the permission to link channels. Players in linked channels hear each other, as long as the speaking player has the &lt;i&gt;speak&lt;/i&gt; privilege in the channel of the listener. You need the link privilege in both channels to create a link, but just in either channel to remove it.</source>
        <translation type="obsolete">Rappresenta il permesso di collegare i canali. I giocatori in canali collegati si ascoltano a vicenda, se il giocatore che parla ha il privilegio &lt;i&gt;Parlare&lt;/i&gt; nel canale dell&apos;ascoltatore. È necessario il privilegio di Collegamento in entrambi i canali per poter creare un collegamento, ma in uno qualsiasi dei canali per rimuoverlo.</translation>
    </message>
    <message>
        <location line="-28"/>
        <source>Write ACL</source>
        <translation>Scrivere le ACL</translation>
    </message>
</context>
<context>
    <name>ChatbarLineEdit</name>
    <message>
        <source>Paste and send</source>
        <translation type="obsolete">Incolla e invia</translation>
    </message>
    <message>
        <source>Type chat message here</source>
        <translation type="obsolete">Inserisci il messaggio di chat qui</translation>
    </message>
</context>
<context>
    <name>ChatbarTextEdit</name>
    <message>
        <location filename="CustomElements.cpp" line="+79"/>
        <source>Paste and send</source>
        <translation>Incolla e invia</translation>
    </message>
    <message>
        <location line="+25"/>
        <source>&lt;center&gt;Type chat message here&lt;/center&gt;</source>
        <translation>&lt;center&gt;Inserisci qui il messaggio&lt;/center&gt;</translation>
    </message>
</context>
<context>
    <name>ClientPlayer</name>
    <message>
        <source>Authenticated</source>
        <translation type="obsolete">Autenticato</translation>
    </message>
    <message>
        <source>Muted (server)</source>
        <translation type="obsolete">Silenziato (server)</translation>
    </message>
    <message>
        <source>Deafened (server)</source>
        <translation type="obsolete">Escluso (server)</translation>
    </message>
    <message>
        <source>Local Mute</source>
        <translation type="obsolete">Silenziamento locale</translation>
    </message>
    <message>
        <source>Muted (self)</source>
        <translation type="obsolete">Silenziato (auto)</translation>
    </message>
    <message>
        <source>Deafened (self)</source>
        <translation type="obsolete">Escluso (auto)</translation>
    </message>
</context>
<context>
    <name>ClientUser</name>
    <message>
        <location filename="ClientUser.cpp" line="+130"/>
        <source>Friend</source>
        <translation>Amico</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Authenticated</source>
        <translation>Autenticato</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Priority speaker</source>
        <translation>Priorità sugli altri</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Recording</source>
        <translation>Registrazione in corso</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Muted (server)</source>
        <translation>Mutato (server)</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Deafened (server)</source>
        <translation>Disattivato (server)</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Local Mute</source>
        <translation>Mutato localmente</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Muted (self)</source>
        <translation>Mutato (auto)</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Deafened (self)</source>
        <translation>Disattivato (auto)</translation>
    </message>
</context>
<context>
    <name>ConfigDialog</name>
    <message>
        <location filename="ConfigDialog.cpp" line="+54"/>
        <source>Accept changes</source>
        <translation>Accetta i cambiamenti</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This button will accept current settings and return to the application.&lt;br /&gt;The settings will be stored to disk when you leave the application.</source>
        <translation>Il pulsante accetterà le impostazioni attuali e tornerà all&apos;applicazione.&lt;br /&gt;Le impostazioni saranno salvate su disco quando uscirai dall&apos;applicazione.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Reject changes</source>
        <translation>Annulla i cambiamenti</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This button will reject all changes and return to the application.&lt;br /&gt;The settings will be reset to the previous positions.</source>
        <translation>Il pulsante annullerà tutti i cambiamenti e tornerà all&apos;applicazione.&lt;br /&gt;Quello che hai modificato sarà reimpostato al valore precedente.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Apply changes</source>
        <translation>Applica i cambiamenti</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This button will immediately apply all changes.</source>
        <translation>Il pulsante applicherà immediatamente tutti i cambiamenti.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Undo changes for current page</source>
        <translation>Annulla i cambiamenti per la pagina corrente</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This button will revert any changes done on the current page to the most recent applied settings.</source>
        <translation>Il pulsante annullerà tutti i cambiamenti fatti sulla pagina corrente e li reimposterà al valore precedente.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Restore defaults for current page</source>
        <translation>Reimposta i valori predefiniti per la pagina corrente</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This button will restore the defaults for the settings on the current page. Other pages will not be changed.&lt;br /&gt;To restore all settings to their defaults, you will have to use this button on every page.</source>
        <oldsource>This button will restore the settings for the current page only to their defaults. Other pages will not be changed.&lt;br /&gt;To restore all settings to their defaults, you will have to use this button on every page.</oldsource>
        <translation>Questo pulsante reimposta i valori della pagina corrente ai loro predefiniti. Le altre pagine non saranno cambiate.&lt;br /&gt;Per reimpostare tutti i valori (compresi quelli delle altre pagine) devi usare questo pulsante su tutte le pagine.</translation>
    </message>
    <message>
        <source>This button will restore the settings for the current page only to their defaults. Other pages will be not be changed.&lt;br /&gt;To restore all settings to their defaults, you will have to use this button on every page.</source>
        <translation type="obsolete">Il bottone caricherà le impostazioni per la pagina corrente al loro default. Le altre pagine non saranno modificate.&lt;br /&gt;Per reimpostare tutti i settaggi ai loro valori predefiniti, devi usare questo bottone su tutte le pagine.</translation>
    </message>
    <message>
        <location filename="ConfigDialog.ui"/>
        <source>Mumble Configuration</source>
        <translation>Configurazione di Mumble</translation>
    </message>
    <message>
        <location/>
        <location filename="ConfigDialogDelegate.mm" line="+142"/>
        <source>Advanced</source>
        <translation>Impostazioni avanzate</translation>
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
        <translation>Accetta i cambiamenti</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This button will accept current settings and return to the application.&lt;br /&gt;The settings will be stored to disk when you leave the application.</source>
        <translation>Il pulsante accetterà le impostazioni attuali e tornerà all&apos;applicazione.&lt;br /&gt;Le impostazioni saranno salvate su disco quando uscirai dall&apos;applicazione.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Reject changes</source>
        <translation>Annulla i cambiamenti</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This button will reject all changes and return to the application.&lt;br /&gt;The settings will be reset to the previous positions.</source>
        <translation>Il pulsante annullerà tutti i cambiamenti e tornerà all&apos;applicazione.&lt;br /&gt;Quello che hai modificato sarà reimpostato al valore precedente.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Apply changes</source>
        <translation>Applica i cambiamenti</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This button will immediately apply all changes.</source>
        <translation>Il pulsante applicherà immediatamente tutti i cambiamenti.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Undo changes for current page</source>
        <translation>Annulla i cambiamenti per la pagina corrente</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This button will revert any changes done on the current page to the most recent applied settings.</source>
        <translation>Il pulsante reimposterà tutti i cambiamenti fatti sulla pagina corrente al valore precedente.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Restore defaults for current page</source>
        <translation>Reimposta i valori predefiniti per la pagina corrente</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This button will restore the defaults for the settings on the current page. Other pages will not be changed.&lt;br /&gt;To restore all settings to their defaults, you will have to use this button on every page.</source>
        <oldsource>This button will restore the settings for the current page only to their defaults. Other pages will not be changed.&lt;br /&gt;To restore all settings to their defaults, you will have to use this button on every page.</oldsource>
        <translation>Questo pulsante reimposta le impostazioni della pagina corrente ai loro valori predefiniti. Le altre pagine non saranno cambiate.&lt;br /&gt;Per reimpostare tutte le impostazioni ai loro valori predefiniti devi usare questo pulsante su tutte le pagine.</translation>
    </message>
</context>
<context>
    <name>ConnectDialog</name>
    <message>
        <location filename="ConnectDialog.cpp" line="+890"/>
        <source>Connecting to %1</source>
        <translation>Connessione a %1</translation>
    </message>
    <message>
        <location line="-491"/>
        <location line="+491"/>
        <source>Enter username</source>
        <translation>Inserisci il nome utente</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation type="obsolete">Sconosciuto</translation>
    </message>
    <message>
        <location line="-491"/>
        <source>Adding host %1</source>
        <translation>Aggiunta host %1</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui"/>
        <location filename="ConnectDialog.cpp" line="+61"/>
        <source>Servername</source>
        <translation>Nome server</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="+1"/>
        <source>Hostname</source>
        <translation>Nome host</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Bonjour name</source>
        <translation>Nome Bonjour</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Port</source>
        <translation>Porta</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Addresses</source>
        <translation>Indirizzi</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Website</source>
        <translation>Sito web</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Packet loss</source>
        <translation>Perdita pacchetti</translation>
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
        <translation>Banda</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>%1 kbit/s</source>
        <translation>%1 kbit/s</translation>
    </message>
    <message>
        <location line="+315"/>
        <source>&amp;Filters</source>
        <translation>&amp;Filtri</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui"/>
        <location filename="ConnectDialog.cpp" line="-314"/>
        <source>Users</source>
        <translation>Utenti</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="+1"/>
        <source>Version</source>
        <translation>Versione</translation>
    </message>
    <message>
        <source>Connect</source>
        <translation type="obsolete">Connetti</translation>
    </message>
    <message>
        <source>Add New...</source>
        <translation type="obsolete">Aggiungi nuovo...</translation>
    </message>
    <message>
        <source>Filters</source>
        <translation type="obsolete">Filtri</translation>
    </message>
    <message>
        <source>Mumble</source>
        <translation type="obsolete">Mumble</translation>
    </message>
    <message>
        <location line="+1005"/>
        <source>Failed to fetch server list</source>
        <translation>Errore nello scaricamento della lista dei server</translation>
    </message>
    <message>
        <source>New</source>
        <translation type="obsolete">Nuovo</translation>
    </message>
    <message>
        <source>-Unnamed entry-</source>
        <translation type="obsolete">-Posizione senza nome-</translation>
    </message>
    <message>
        <source>Add</source>
        <translation type="obsolete">Aggiungi</translation>
    </message>
    <message>
        <source>Update</source>
        <translation type="obsolete">Aggiorna</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui"/>
        <source>Mumble Server Connect</source>
        <translation>Connessione al Server Mumble</translation>
    </message>
    <message>
        <location/>
        <source>Ping</source>
        <translation>Ping</translation>
    </message>
    <message>
        <location/>
        <source>Remove from Favorites</source>
        <translation>Rimuovi dai preferiti</translation>
    </message>
    <message>
        <source>Edit...</source>
        <translation type="obsolete">Modifica...</translation>
    </message>
    <message>
        <location/>
        <source>Add custom server</source>
        <translation>Aggiungi server personalizzato</translation>
    </message>
    <message>
        <source>Add to Favorites</source>
        <translation type="obsolete">Aggiungi ai preferiti</translation>
    </message>
    <message>
        <source>Open Webpage</source>
        <translation type="obsolete">Apri pagina web</translation>
    </message>
    <message>
        <source>Show Reachable</source>
        <translation type="obsolete">Visualizza raggiungibili</translation>
    </message>
    <message>
        <location/>
        <source>Show all servers that respond to ping</source>
        <translation>Visualizza tutti i server che rispondono al ping</translation>
    </message>
    <message>
        <source>Show Populated</source>
        <translation type="obsolete">Visualizza popolati</translation>
    </message>
    <message>
        <location/>
        <source>Show all servers with users</source>
        <translation>Visualizza tutti i server con utenti</translation>
    </message>
    <message>
        <source>Show All</source>
        <translation type="obsolete">Visualizza tutti</translation>
    </message>
    <message>
        <location/>
        <source>Show all servers</source>
        <translation>Visualizza tutti i server</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Copy</source>
        <translation>&amp;Copia</translation>
    </message>
    <message>
        <location/>
        <source>Copy favorite link to clipboard</source>
        <translation>Copia collegamento preferito negli appunti</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Paste</source>
        <translation>&amp;Incolla</translation>
    </message>
    <message>
        <location/>
        <source>Paste favorite from clipboard</source>
        <translation>Incolla preferito dagli appunti</translation>
    </message>
    <message>
        <source>&amp;Custom Servers</source>
        <translation type="obsolete">&amp;Server personalizzati</translation>
    </message>
    <message>
        <source>&amp;Label</source>
        <translation type="obsolete">&amp;Etichetta</translation>
    </message>
    <message>
        <source>A&amp;ddress</source>
        <translation type="obsolete">&amp;Indirizzo</translation>
    </message>
    <message>
        <source>&amp;Port</source>
        <translation type="obsolete">&amp;Porta</translation>
    </message>
    <message>
        <source>&amp;Username</source>
        <translation type="obsolete">&amp;Nome utente</translation>
    </message>
    <message>
        <source>&amp;Password</source>
        <translation type="obsolete">&amp;Password</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="-731"/>
        <source>&amp;Connect</source>
        <translation>&amp;Connetti</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Annulla</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation type="obsolete">&amp;Aggiungi</translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation type="obsolete">&amp;Cancella</translation>
    </message>
    <message>
        <source>Server &amp;Browser</source>
        <translation type="obsolete">Elenco &amp;Server</translation>
    </message>
    <message>
        <source>Label</source>
        <translation type="obsolete">Etichetta</translation>
    </message>
    <message>
        <source>Address</source>
        <translation type="obsolete">Indirizzo</translation>
    </message>
    <message>
        <source>URL</source>
        <translation type="obsolete">URL</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Annulla</translation>
    </message>
    <message>
        <source>C&amp;opy to custom</source>
        <translation type="obsolete">&amp;Copia al personalizzato</translation>
    </message>
    <message>
        <source>&amp;View Webpage</source>
        <translation type="obsolete">&amp;Visualizza pagina Web</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui"/>
        <source>&amp;Edit...</source>
        <translation>&amp;Modifica...</translation>
    </message>
    <message>
        <location/>
        <location filename="ConnectDialog.cpp" line="+2"/>
        <source>&amp;Add New...</source>
        <translation>&amp;Aggiungi...</translation>
    </message>
    <message>
        <location/>
        <source>Add to &amp;Favorites</source>
        <translation>Aggiungi ai &amp;Preferiti</translation>
    </message>
    <message>
        <location/>
        <source>Open &amp;Webpage</source>
        <translation>Apri Pagina &amp;Web</translation>
    </message>
    <message>
        <location/>
        <source>Show &amp;Reachable</source>
        <translation>Visualizza &amp;Raggiungibili</translation>
    </message>
    <message>
        <location/>
        <source>Show &amp;Populated</source>
        <translation>Visualizza &amp;Popolati</translation>
    </message>
    <message>
        <location/>
        <source>Show &amp;All</source>
        <translation>Visualizza &amp;Tutti</translation>
    </message>
</context>
<context>
    <name>ConnectDialogEdit</name>
    <message>
        <location filename="ConnectDialogEdit.ui"/>
        <source>Edit Server</source>
        <translation>Modifica server</translation>
    </message>
    <message>
        <location/>
        <source>Name of the server</source>
        <translation>Nome che apparirà sulla tua lista dei server preferiti</translation>
    </message>
    <message>
        <source>&lt;b&gt;Name&lt;/b&gt;&lt;br/&gt;
Name of the server. This is what the server will be named like in your serverlist and can be chosen freely.</source>
        <translation type="obsolete">&lt;b&gt;Nome&lt;/b&gt;&lt;br/&gt;
Nome del server. Questo è come il server si chiamerà, come nella tua lista server, e può essere scelto liberamente.</translation>
    </message>
    <message>
        <location/>
        <source>A&amp;ddress</source>
        <translation>&amp;Indirizzo</translation>
    </message>
    <message>
        <location/>
        <source>Internet address of the server.</source>
        <oldsource>Internet address of the server. </oldsource>
        <translation>Indirizzo internet del server.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Address&lt;/b&gt;&lt;br/&gt;
Internet address of the server. This can be a normal hostname, an IPv4/IPv6 address or a Bonjour service identifier. Bonjour service identifiers have to be prefixed with a &apos;@&apos; to be recognized by Mumble.</source>
        <oldsource>&lt;b&gt;Address&lt;/b&gt;&lt;/br&gt;
Internet address of the server. This can be a normal hostname, an ipv4/6 address or a bonjour service identifier. Bonjour service identifiers have to be prefixed with a &apos;@&apos; to be recognized by Mumble.</oldsource>
        <translation>&lt;b&gt;Indirizzo&lt;/b&gt;&lt;/br&gt;
Indirizzo internet del server. Può essere un normale nome di host, un indirizzo IPv4/IPv6, o un identificatore di servizio Bonjour. Gli identificatori di servizi Bonjour devono avere il prefisso &apos;@&apos; per essere riconosciuti da Mumble.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Port</source>
        <translation>&amp;Porta</translation>
    </message>
    <message>
        <location/>
        <source>Port on which the server is listening</source>
        <translation>Porta di ascolto del server</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Port&lt;/b&gt;&lt;br/&gt;
Port on which the server is listening. If the server is identified by a Bonjour service identifier this field will be ignored.</source>
        <translation>&lt;b&gt;Porta&lt;/b&gt;&lt;br/&gt;
Porta sulla quale il server è in ascolto. Se il server è identificato con un identificatore di servizio Bonjour questo campo sarà ignorato.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Username</source>
        <translation>&amp;Nome utente</translation>
    </message>
    <message>
        <location/>
        <source>Username to send to the server</source>
        <translation>Nome utente da inviare al server</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Username&lt;/b&gt;&lt;br/&gt;
Username to send to the server. Be aware that the server can impose restrictions on how a username might look like. Also your username could already be taken by another user.</source>
        <translation>&lt;b&gt;Nome utente&lt;/b&gt;&lt;br/&gt;
Nome utente da inviare al server. ll server potrebbe avere delle restrizioni su come il nome utente deve essere. Un nome utente potrebbe essere già stato preso da un&apos;altro utente.</translation>
    </message>
    <message>
        <location/>
        <source>Label</source>
        <translation>Nome</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="-75"/>
        <source>Add Server</source>
        <translation>Aggiungi server</translation>
    </message>
    <message>
        <location filename="ConnectDialogEdit.ui"/>
        <source>Password</source>
        <translation>Password</translation>
    </message>
    <message>
        <location/>
        <source>Password to send to the server</source>
        <translation>Password del server</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Password&lt;/b&gt;&lt;br/&gt;
Password to be sent to the server on connect. This password is needed when connecting as &lt;i&gt;SuperUser&lt;/i&gt; or to a server using password authentication. If not entered here the password will be queried on connect.</source>
        <translation>&lt;b&gt;Password&lt;/b&gt;&lt;br/&gt;
Password da inviare al server a cui devi connetterti. La password è richiesta solo se ti stai connettendo come &lt;i&gt;SuperUser&lt;/i&gt; o se il server ne richiede una per connettersi. Se non verrà inserita e sarà richiesta dal server, ti apparirà una finestra dove dovrai inserirla.</translation>
    </message>
    <message>
        <location/>
        <source>Show password</source>
        <translation>Visualizza password</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Label&lt;/b&gt;&lt;br/&gt;
Label of the server. This is what the server will be named like in your server list and can be chosen freely.</source>
        <translation>&lt;b&gt;Nome&lt;/b&gt;&lt;br/&gt;
Nome del server. Questo è il nome che apparirà sulla tua lista dei server preferiti, può essere scelto liberamente.</translation>
    </message>
</context>
<context>
    <name>CoreAudioSystem</name>
    <message>
        <location filename="CoreAudio.cpp" line="+83"/>
        <source>Default Device</source>
        <translation>Periferica predefinita</translation>
    </message>
</context>
<context>
    <name>CrashReporter</name>
    <message>
        <location filename="CrashReporter.cpp" line="+37"/>
        <source>Mumble Crash Report</source>
        <translation>Rapporto errori di Mumble</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>&lt;p&gt;&lt;b&gt;We&apos;re terribly sorry, but it seems Mumble has crashed. Do you want to send a crash report to the Mumble developers?&lt;/b&gt;&lt;/p&gt;&lt;p&gt;The crash report contains a partial copy of Mumble&apos;s memory at the time it crashed, and will help the developers fix the problem.&lt;/p&gt;</source>
        <translation>&lt;p&gt;&lt;b&gt;Siamo terribilmente spiacenti, ma sembra che Mumble si sia chiuso per colpa di un errore. Vuoi inviare un rapporto sugli errori agli sviluppatori di Mumble?&lt;/b&gt;&lt;/p&gt;&lt;p&gt;Il rapporto contiene una copia parziale della memoria di Mumble al momento dell&apos;errore, e questo aiuterà gli sviluppatori a correggere il problema.&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Email address (optional)</source>
        <translation>Indirizzo email (facoltativo)</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Please describe briefly, in English, what you were doing at the time of the crash</source>
        <oldsource>Please briefly describe what you were doing at the time of the crash</oldsource>
        <translation>Per favore descrivi brevemente cosa stavi facendo al momento della chiusura (in inglese)</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Send Report</source>
        <translation>Invia rapporto</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Don&apos;t send report</source>
        <translation>Non inviare rapporto</translation>
    </message>
    <message>
        <location line="+24"/>
        <source>Crash upload successful</source>
        <translation>Invio rapporto completato con successo</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Thank you for helping make Mumble better!</source>
        <translation>Grazie per averci aiutato a rendere Mumble migliore!</translation>
    </message>
    <message>
        <location line="+2"/>
        <location line="+2"/>
        <source>Crash upload failed</source>
        <translation>Invio rapporto fallito</translation>
    </message>
    <message>
        <location line="-2"/>
        <source>We&apos;re really sorry, but it appears the crash upload has failed with error %1 %2. Please inform a developer.</source>
        <translation>Siamo veramente spiacenti, ma sembra che l&apos;invio del rapporto è fallito con l&apos;errore %1 %2. Per favore informa uno sviluppatore.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>This really isn&apos;t funny, but apparently there&apos;s a bug in the crash reporting code, and we&apos;ve failed to upload the report. You may inform a developer about error %1</source>
        <translation>Questo veramente non è divertente, ma apparentemente c&apos;è un bug nel codice del rapporto, e abbiamo fallito ad inviare il rapporto. Puoi informare uno sviluppatore sull&apos;errore %1</translation>
    </message>
    <message>
        <location line="+114"/>
        <source>Uploading crash report</source>
        <translation>Invio rapporto errore in corso</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Abort upload</source>
        <translation>Interrompi invio</translation>
    </message>
</context>
<context>
    <name>DXAudioInput</name>
    <message>
        <location filename="DirectSound.cpp" line="+172"/>
        <source>Default DirectSound Voice Input</source>
        <translation>Ingresso DirectSound predefinito</translation>
    </message>
    <message>
        <location line="+461"/>
        <source>Opening chosen DirectSound Input device failed. No microphone capture will be done.</source>
        <translation>Apertura del dispositivo di ingresso DirectSound fallita. Non ci sarà nessun ingresso microfonico.</translation>
    </message>
    <message>
        <location line="-79"/>
        <source>Opening chosen DirectSound Input failed. Default device will be used.</source>
        <translation>Apertura dell&apos;ingresso DirectSound fallita. Verrà usato il dispositivo predefinito.</translation>
    </message>
    <message>
        <location line="+74"/>
        <source>Lost DirectSound input device.</source>
        <translation>Dispositivo di ingresso DirectSound disconnesso.</translation>
    </message>
</context>
<context>
    <name>DXAudioOutput</name>
    <message>
        <location line="-496"/>
        <source>Default DirectSound Voice Output</source>
        <translation>Uscita DirectSound predefinita</translation>
    </message>
    <message>
        <location line="+247"/>
        <source>Opening chosen DirectSound Output failed. Default device will be used.</source>
        <translation>Apertura dell&apos;uscita DirectSound scelta fallita. Verrà usato il dispositivo predefinito.</translation>
    </message>
    <message>
        <location line="+78"/>
        <source>Lost DirectSound output device.</source>
        <translation>Dispositivo di uscita DirectSound disconnesso.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Opening chosen DirectSound Output failed. No audio will be heard.</source>
        <translation>Apertura dell&apos;uscita DirectSound fallita. Non si udirà nessun audio.</translation>
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
        <translation>Mumble non è riuscito a inizializzare il database in nessuna
delle possibili posizioni.</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>The database &apos;%1&apos; is read-only. Mumble cannot store server settings (i.e. SSL certificates) until you fix this problem.</source>
        <oldsource>The database &apos;%1&apos; is read-only. Mumble can not store server settings (ie. SSL certificates) until you fix this problem.</oldsource>
        <translation>Il database &apos;%1&apos; è sola-lettura. Mumble non può salvare le impostazioni del server (es. certificati SSl) fino a che non risolverai il problema.</translation>
    </message>
</context>
<context>
    <name>DockTitleBar</name>
    <message>
        <location filename="CustomElements.cpp" line="+135"/>
        <source>Drag here</source>
        <translation>Trascina qui</translation>
    </message>
</context>
<context>
    <name>GlobalShortcut</name>
    <message>
        <location filename="GlobalShortcut.ui"/>
        <source>Shortcuts</source>
        <translation>Scorciatoie</translation>
    </message>
    <message>
        <location/>
        <source>List of configured shortcuts</source>
        <translation>lista delle scorciatoie configurate</translation>
    </message>
    <message>
        <location/>
        <source>Function</source>
        <translation>Funzione</translation>
    </message>
    <message>
        <location/>
        <source>Data</source>
        <translation>Dati</translation>
    </message>
    <message>
        <location/>
        <source>Shortcut</source>
        <translation>Scorciatoia</translation>
    </message>
    <message>
        <location/>
        <source>Suppress</source>
        <translation>Disabilita</translation>
    </message>
    <message>
        <location/>
        <source>Add new shortcut</source>
        <translation>Aggiungi una nuova scorciatoia</translation>
    </message>
    <message>
        <location/>
        <source>This will add a new global shortcut</source>
        <translation>Questo aggiugerà una scorciatoia globale</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Add</source>
        <translation>&amp;Aggiungi</translation>
    </message>
    <message>
        <location/>
        <source>Remove selected shortcut</source>
        <translation>Rimuove la scorciatoia selezionata</translation>
    </message>
    <message>
        <location/>
        <source>This will permanently remove a selected shortcut.</source>
        <translation>Questo rimuoverà permanentemente la scorciatoia selezionata.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Remove</source>
        <translation>&amp;Rimuovi</translation>
    </message>
</context>
<context>
    <name>GlobalShortcutConfig</name>
    <message>
        <location filename="GlobalShortcut.cpp" line="+654"/>
        <source>Shortcuts</source>
        <translation>Scorciatoie</translation>
    </message>
    <message>
        <location line="+38"/>
        <source>Shortcut button combination.</source>
        <translation>Combinazione di pulsanti della scorciatoia.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>&lt;b&gt;This is the global shortcut key combination.&lt;/b&gt;&lt;br /&gt;Click this field and then press the desired key/button combo to rebind. Double-click to clear.</source>
        <oldsource>&lt;b&gt;This is the global shortcut key combination.&lt;/b&gt;&lt;br /&gt;Double-click this field and then the desired key/button combo to rebind.</oldsource>
        <translation>&lt;b&gt;Questa è la combinazione della scorciatoia globale.&lt;/b&gt;&lt;br /&gt;Fai doppio click in questo campo e poi premi la combinazione di tasti per riassegnarla.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Suppress keys from other applications</source>
        <translation>Elimina tasti da altre applicazioni</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>&lt;b&gt;This hides the button presses from other applications.&lt;/b&gt;&lt;br /&gt;Enabling this will hide the button (or the last button of a multi-button combo) from other applications. Note that not all buttons can be suppressed.</source>
        <translation>&lt;b&gt;Nasconde le pressioni dei tasti nelle altre applicazioni.&lt;/b&gt;&lt;br /&gt;Abilitando verranno nascosti i pulsanti (o l&apos;ultimo pulsante di una sequenza multi pulsante) dalle altre applicazioni. Nota che non tutti i bottoni possono essere premuti.</translation>
    </message>
</context>
<context>
    <name>GlobalShortcutMacInit</name>
    <message>
        <location filename="GlobalShortcut_macx.mm" line="+65"/>
        <source>Mumble has detected that it is unable to receive Global Shortcut events when it is in the background.&lt;br /&gt;&lt;br /&gt;This is because the Universal Access feature called &apos;Enable access for assistive devices&apos; is currently disabled.&lt;br /&gt;&lt;br /&gt;Please &lt;a href=&quot; &quot;&gt;enable this setting&lt;/a&gt; and continue when done.</source>
        <translation>Mumble ha rilevato che non è in grado di ricevere i segnali delle Scorciatoie Globali quando è in background.&lt;br /&gt;&lt;br /&gt;Questo è dovuto alla funzione di Accesso Universale chiamata &apos;Abilita accesso per i dispositivi di assistenza&apos; che è attualmente disattivata.&lt;br /&gt;&lt;br /&gt;Si prega di &lt;a href=&quot;&quot;&gt;abilitare questa impostazione&lt;/a&gt; e proseguire quando terminato.</translation>
    </message>
</context>
<context>
    <name>GlobalShortcutTarget</name>
    <message>
        <location filename="GlobalShortcutTarget.ui"/>
        <source>Whisper Target</source>
        <translation>Destinatario del sussurro</translation>
    </message>
    <message>
        <location/>
        <source>Whisper to list of Users</source>
        <translation>&quot;Sussurra&quot; a lista di utenti</translation>
    </message>
    <message>
        <location/>
        <source>Channel Target</source>
        <translation>Canale destinatario</translation>
    </message>
    <message>
        <location/>
        <source>Restrict to Group</source>
        <translation>Restringi al gruppo</translation>
    </message>
    <message>
        <location/>
        <source>If specified, only members of this group will receive the whisper.</source>
        <translation>Se specificato, solo i membri di questo gruppo riceveranno il sussurro.</translation>
    </message>
    <message>
        <source>If checked the whisper will also be transmitted to linked channels.</source>
        <translation type="obsolete">Se selezionato il sussurro sarà trasmesso anche ai canali collegati.</translation>
    </message>
    <message>
        <source>Whisper to Linked channels</source>
        <translation type="obsolete">Sussurra ai canali collegati</translation>
    </message>
    <message>
        <source>If checked this whisper will also be sent to the subchannels of the channel target.</source>
        <translation type="obsolete">Se selezionato il sussurro sarà trasmesso anche ai sottocanali del canale destinatario.</translation>
    </message>
    <message>
        <location/>
        <source>List of users</source>
        <translation>Lista degli utenti</translation>
    </message>
    <message>
        <location/>
        <source>Add</source>
        <translation>Aggiungi</translation>
    </message>
    <message>
        <location/>
        <source>Remove</source>
        <translation>Rimuovi</translation>
    </message>
    <message>
        <location/>
        <source>Modifiers</source>
        <translation>Modificatori</translation>
    </message>
    <message>
        <location/>
        <source>Do not send positional audio information when using this whisper shortcut.</source>
        <translation>Non spedisce audio posizionale quando si stà usando questo collegamento per il sussurro.</translation>
    </message>
    <message>
        <location/>
        <source>Ignore positional audio</source>
        <translation>Ignora audio posizionale</translation>
    </message>
    <message>
        <location/>
        <source>Shout to Linked channels</source>
        <translation>&quot;Urla&quot; ai canali Collegati</translation>
    </message>
    <message>
        <location/>
        <source>Shout to subchannels</source>
        <translation>&quot;Urla&quot; ai sottocanali</translation>
    </message>
    <message>
        <location/>
        <source>Shout to Channel</source>
        <translation>&quot;Urla&quot; al canale</translation>
    </message>
    <message>
        <location/>
        <source>The whisper will also be transmitted to linked channels.</source>
        <translation>Il sussurro sarà inviato anche ai canali collegati a quello selezionato.</translation>
    </message>
    <message>
        <location/>
        <source>The whisper will also be sent to the subchannels of the channel target.</source>
        <translation>Il sussurro sarà inviato anche ai sottocanali del canale selezionato.</translation>
    </message>
</context>
<context>
    <name>GlobalShortcutX</name>
    <message>
        <location filename="GlobalShortcut_unix.cpp" line="+325"/>
        <source>Mouse %1</source>
        <translation>Mouse %1</translation>
    </message>
</context>
<context>
    <name>LCD</name>
    <message>
        <location filename="LCD.cpp" line="+279"/>
        <source>Not connected</source>
        <translation>Non connesso</translation>
    </message>
</context>
<context>
    <name>LCDConfig</name>
    <message>
        <source>Graphic</source>
        <translation type="obsolete">Grafica</translation>
    </message>
    <message>
        <source>Character</source>
        <translation type="obsolete">Carattere</translation>
    </message>
    <message>
        <location line="-160"/>
        <source>Enable this device</source>
        <translation>Abilita questo dispositivo</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>LCD</source>
        <translation>LCD</translation>
    </message>
    <message>
        <location filename="LCD.ui"/>
        <source>Form</source>
        <translation>Modulo</translation>
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
        <translation type="obsolete">&lt;!DOCTYPE HTML PUBLIC &quot;-//W3C//DTD HTML 4.0//EN&quot; &quot;http://www.w3.org/TR/REC-html40/strict.dtd&quot;&gt;
&lt;html&gt;&lt;head&gt;&lt;meta name=&quot;qrichtext&quot; content=&quot;1&quot; /&gt;&lt;style type=&quot;text/css&quot;&gt;
p, li { white-space: pre-wrap; }
&lt;/style&gt;&lt;/head&gt;&lt;body style=&quot; font-family:&apos;MS Shell Dlg 2&apos;; font-size:8.25pt; font-weight:400; font-style:normal;&quot;&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;Questa è la lista dei dispositivi LCD nel tuo sistema. Nella lista ci sono i nomi, ma include anche la dimensione del display e il tipo.Mumble supporta l&apos;invio a molti dispositivi LCD allo stesso tempo.&lt;/p&gt;
&lt;p style=&quot;-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;&lt;span style=&quot; font-weight:600; font-style:italic;&quot;&gt;Type:&lt;/span&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;Mumble supporta 2 differenti tipi di dispositivi LCD. Gli LCD Graphic, dove è possibile disegnare in ogni pixel individualmente, ma anche gli LCD Character dove è possibile solo scrivere dei caratteri. Alcune funzioni di Mumble sono disponibili solo sugli LDC Graphic.&lt;/p&gt;
&lt;p style=&quot;-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;&lt;span style=&quot; font-weight:600; font-style:italic;&quot;&gt;Size:&lt;/span&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;Questo campo descrive la dimensione di un dispositivo LCD. La dimensione è espressa in pixel o in caratteri.&lt;/p&gt;
&lt;p style=&quot;-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;&lt;span style=&quot; font-weight:600; font-style:italic;&quot;&gt;Abilitati:&lt;/span&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt; font-weight:600; font-style:italic;&quot;&gt;&lt;span style=&quot; font-weight:400; font-style:normal;&quot;&gt;Questo decide se Mumble deve attingere ad un particolare dispositivo LCD.&lt;/span&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Devices</source>
        <translation>Dispositivo</translation>
    </message>
    <message>
        <location/>
        <source>Name</source>
        <translation>Nome</translation>
    </message>
    <message>
        <source>Type</source>
        <translation type="obsolete">Tipo</translation>
    </message>
    <message>
        <location/>
        <source>&lt;p&gt;This is the list of available LCD devices on your system.  It lists devices by name, but also includes the size of the display. Mumble supports outputting to several LCD devices at a time.&lt;/p&gt;
&lt;h3&gt;Size:&lt;/h3&gt;
&lt;p&gt;
This field describes the size of an LCD device. The size is given either in pixels (for Graphic LCDs) or in characters (for Character LCDs).&lt;/p&gt;
&lt;h3&gt;Enabled:&lt;/h3&gt;
&lt;p&gt;This decides whether Mumble should draw to a particular LCD device.&lt;/p&gt;</source>
        <translation>&lt;p&gt;Questa è la lista dei dispositivi LCD nel tuo sistema. Nella lista ci sono i nomi, ma include anche la dimensione del display e il tipo.Mumble supporta l&apos;invio a molti dispositivi LCD allo stesso tempo.&lt;/p&gt;
&lt;h3&gt;Dimensione:&lt;/h3&gt;
&lt;p&gt;
Questo campo descrive la dimensione di un dispositivo LCD. La dimensione è espressa in pixel o in caratteri.&lt;/p&gt;
&lt;h3&gt;Abilitato:&lt;/h3&gt;
&lt;p&gt;Questo decide se Mumble deve attingere ad un particolare dispositivo LCD.&lt;/p&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Size</source>
        <translation>Dimensione</translation>
    </message>
    <message>
        <location/>
        <source>Enabled</source>
        <translation>Abilitato</translation>
    </message>
    <message>
        <location/>
        <source>Views</source>
        <translation>Visualizzazioni</translation>
    </message>
    <message>
        <location/>
        <source>Minimum Column Width</source>
        <translation>Larghezza minima colonna</translation>
    </message>
    <message>
        <location/>
        <source>&lt;p&gt;This option decides the minimum width a column in the User View.&lt;/p&gt;
&lt;p&gt;If too many people are speaking at once, the User View will split itself into columns. You can use this option to pick a compromise between number of users shown on the LCD, and width of user names.&lt;/p&gt;
</source>
        <translation>&lt;p&gt;Questa opzione imposta la minima larghezza della colonna nella visuale dell&apos;utente&lt;/p&gt;
&lt;p&gt;Se troppe persone stanno parlando insieme, la Vista Giocatore verrà divisa in colonne. Puoi usare questa opzione per scegliere un compromesso tra il numero di giocatori mostrati sull&apos;LCD, e la larghezza dei nomi dei giocatori.&lt;/p&gt;
</translation>
    </message>
    <message>
        <location/>
        <source>This setting decides the width of column splitter.</source>
        <translation>Questa impostazione decide la larghezza della divisione tra le colonne.</translation>
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
        <translation type="obsolete">&lt;!DOCTYPE HTML PUBLIC &quot;-//W3C//DTD HTML 4.0//EN&quot; &quot;http://www.w3.org/TR/REC-html40/strict.dtd&quot;&gt;&lt;html&gt;&lt;head&gt;&lt;meta name=&quot;qrichtext&quot; content=&quot;1&quot; /&gt;&lt;style type=&quot;text/css&quot;&gt;p, li { white-space: pre-wrap; }&lt;/style&gt;&lt;/head&gt;&lt;body style=&quot; font-family:&apos;MS Shell Dlg 2&apos;; font-size:8.25pt; font-weight:400; font-style:normal;&quot;&gt;&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;&lt;span style=&quot; font-size:8pt;&quot;&gt;Questa opzione imposta la minima larghezza della colonna nella vista del Giocatore.&lt;/span&gt;&lt;/p&gt;&lt;p style=&quot;-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;&lt;/p&gt;&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;Se troppe persone stanno parlando insieme, la Vista Giocatore verrà divisa in colonne.&lt;/p&gt;&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;Puoi usare questa opzione per scegliere un compromesso tra il numero di giocatori mostrati sull&apos;LCD,&lt;/p&gt;&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;e la larghezza dei nomi dei giocatori.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <source>TextLabel</source>
        <translation type="obsolete">Etichetta testo</translation>
    </message>
    <message>
        <location/>
        <source>Splitter Width</source>
        <translation>Larghezza divisione</translation>
    </message>
    <message>
        <source>&lt;!DOCTYPE HTML PUBLIC &quot;-//W3C//DTD HTML 4.0//EN&quot; &quot;http://www.w3.org/TR/REC-html40/strict.dtd&quot;&gt;
&lt;html&gt;&lt;head&gt;&lt;meta name=&quot;qrichtext&quot; content=&quot;1&quot; /&gt;&lt;style type=&quot;text/css&quot;&gt;
p, li { white-space: pre-wrap; }
&lt;/style&gt;&lt;/head&gt;&lt;body style=&quot; font-family:&apos;MS Shell Dlg 2&apos;; font-size:8.25pt; font-weight:400; font-style:normal;&quot;&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;&lt;span style=&quot; font-size:8pt;&quot;&gt;This setting decides the width of column splitter.&lt;/span&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="obsolete">&lt;!DOCTYPE HTML PUBLIC &quot;-//W3C//DTD HTML 4.0//EN&quot; &quot;http://www.w3.org/TR/REC-html40/strict.dtd&quot;&gt;&lt;html&gt;&lt;head&gt;&lt;meta name=&quot;qrichtext&quot; content=&quot;1&quot; /&gt;&lt;style type=&quot;text/css&quot;&gt;p, li { white-space: pre-wrap; }&lt;/style&gt;&lt;/head&gt;&lt;body style=&quot; font-family:&apos;MS Shell Dlg 2&apos;; font-size:8.25pt; font-weight:400; font-style:normal;&quot;&gt;&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;&lt;span style=&quot; font-size:8pt;&quot;&gt;Questa impostazione regola la larghezza della colonna dello splitter.&lt;/span&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
</context>
<context>
    <name>Log</name>
    <message>
        <location filename="Log.cpp" line="+220"/>
        <source>Debug</source>
        <translation>Debug</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Critical</source>
        <translation>Critico</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Warning</source>
        <translation>Avviso</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Information</source>
        <translation>Informativo</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Server Connected</source>
        <translation>Connessione al server</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Server Disconnected</source>
        <translation>Disconnessione dal server</translation>
    </message>
    <message>
        <source>Player Joined Server</source>
        <translation type="obsolete">Giocatore connesso al Server</translation>
    </message>
    <message>
        <source>Player Left Server</source>
        <translation type="obsolete">Giocatore disconnesso dal Server</translation>
    </message>
    <message>
        <source>Player kicked (you or by you)</source>
        <translation type="obsolete">Giocatore espulso (tu o da te)</translation>
    </message>
    <message>
        <source>Player kicked</source>
        <translation type="obsolete">Giocatore espulso</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>You self-muted/deafened</source>
        <translation>Mutato/disattivato (tu)</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Other self-muted/deafened</source>
        <translation>Mutato/disattivato (altro)</translation>
    </message>
    <message>
        <source>Player muted (you)</source>
        <translation type="obsolete">Giocatore silenziato (tu)</translation>
    </message>
    <message>
        <source>Player muted (by you)</source>
        <translation type="obsolete">Giocatore silenziato (da te)</translation>
    </message>
    <message>
        <source>Player muted (other)</source>
        <translation type="obsolete">Giocatore silenziato (altro)</translation>
    </message>
    <message>
        <source>Player Joined Channel</source>
        <translation type="obsolete">Giocatore entrato nel Canale</translation>
    </message>
    <message>
        <source>Player Left Channel</source>
        <translation type="obsolete">Giocatore uscito dal Canale</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Permission Denied</source>
        <translation>Permesso Negato</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Text Message</source>
        <translation>Messaggio di Testo</translation>
    </message>
    <message>
        <source>Script Errors</source>
        <translation type="obsolete">Errori di script</translation>
    </message>
    <message>
        <location line="-13"/>
        <source>User Joined Server</source>
        <translation>Utente entrato nel server</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>User Left Server</source>
        <translation>Utente ha lasciato il server</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>User recording state changed</source>
        <translation>Stato di registrazione utente cambiato</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>User kicked (you or by you)</source>
        <translation>Utente espulso (tu o da tu)</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>User kicked</source>
        <translation>Utente espulso</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>User muted (you)</source>
        <translation>Utente mutato (tu)</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>User muted (by you)</source>
        <translation>Utente mutato (da te)</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>User muted (other)</source>
        <translation>Utente mutato (altro)</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>User Joined Channel</source>
        <translation>Utente entrato nel canale</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>User Left Channel</source>
        <translation>Utente uscito dal canale</translation>
    </message>
    <message>
        <location line="+54"/>
        <source>the server</source>
        <translation>il server</translation>
    </message>
    <message>
        <location line="+117"/>
        <source>[[ Text object too large to display ]]</source>
        <translation>[[ Oggetto di testo troppo grande per essere visualizzato ]]</translation>
    </message>
    <message>
        <location line="+42"/>
        <source>[Date changed to %1]
</source>
        <translation>[Data cambiata in %1]
</translation>
    </message>
    <message>
        <location line="+142"/>
        <source>link to %1</source>
        <translation>collegamento con %1</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>ftp link to %1</source>
        <translation>collegamento ftp con %1</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>player link</source>
        <translation>giocatore collegato</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>channel link</source>
        <translation>canale collegato</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1 link</source>
        <translation>%1 collegato</translation>
    </message>
</context>
<context>
    <name>LogConfig</name>
    <message>
        <location line="-521"/>
        <source>Toggle console for %1 events</source>
        <translation>Imposta console per gli eventi %1</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Toggle pop-up notifications for %1 events</source>
        <translation>Imposta notifica pop-up per gli eventi %1</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Toggle Text-To-Speech for %1 events</source>
        <translation>Imposta Sintesi vocale per gli eventi %1</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Click here to toggle sound notification for %1 events</source>
        <translation>Premi qui per cambiare la notifica audio per gli eventi %1</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Path to sound file used for sound notifications in the case of %1 events&lt;br /&gt;Single click to play&lt;br /&gt;Double-click to change</source>
        <translation>Percorso del file sonoro da usare come notifica nel caso degli eventi %1&lt;br /&gt;Click singolo per ascoltare&lt;br /&gt;Doppio-click per cambiare</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Click here to toggle console output for %1 events.&lt;br /&gt;If checked, this option makes Mumble output all %1 events in its message log.</source>
        <translation>Cicca qui per attivare la console di uscita per gli eventi %1.&lt;br /&gt; Se selezionata, questa opzione fa scrivere a Mumble tutti gli eventi %1 nel messaggio di log.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Click here to toggle pop-up notifications for %1 events.&lt;br /&gt;If checked, a notification pop-up will be created by Mumble for every %1 event.</source>
        <translation>Premi qui per impostare le notifiche pop-up per gli eventi %1.&lt;br /&gt;Se selezionato, un pop-up di notifica sarà creato da Mumble per gli eventi %1.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Path to sound file used for sound notifications in the case of %1 events.&lt;br /&gt;Single click to play&lt;br /&gt;Double-click to change&lt;br /&gt;Ensure that sound notifications for these events are enabled or this field will not have any effect.</source>
        <translation>Percorso del file sonoro da usare come notifica nel caso degli eventi %1&lt;br /&gt;Click singolo per ascoltare&lt;br /&gt;Doppio-click per cambiare&lt;br /&gt;Assicurati che le notifiche sonore siano abilitate per questi eventi o questi campi non avranno effetto.</translation>
    </message>
    <message>
        <source>Click here to toggle Text-To-Speech for %1 events.&lt;br /&gt;If checked, mumble uses Text-To-Speech to read %1 events out loud to you. Text-To-Speech is also able to read the contents of the event which is not true for soundfiles. Text-To-Speech and soundfiles cannot be used at the same time.</source>
        <translation type="obsolete">Clicca qui per notificare gli eventi%1 con un messaggio di Sintesi Vocale. &lt;br /&gt; Se selezionato, mumble utilizza La sintesi vocale per leggere gli eventi %1 ad alta voce a voi. La sintesi vocale è anche in grado di leggere il contenuto dell&apos;evento che non è leggibile con il file audio. il file audio per la sintesi vocale e non può essere utilizzato allo stesso tempo.</translation>
    </message>
    <message>
        <location line="-2"/>
        <source>Click here to toggle Text-To-Speech for %1 events.&lt;br /&gt;If checked, Mumble uses Text-To-Speech to read %1 events out loud to you. Text-To-Speech is also able to read the contents of the event which is not true for sound files. Text-To-Speech and sound files cannot be used at the same time.</source>
        <oldsource>Click here to toggle sound notification for %1 events.&lt;br /&gt;If checked, mumble uses a soundfile predefined by you to indicate %1 events. Soundfiles and Text-To-Speech cannot be used at the same time.</oldsource>
        <translation>Clicca qui per cambiare la notifica sonora per gli eventi %1.&lt;br /&gt;Se selezionato mumble userà un file audio predefinito per indicare gli eventi %1. File audio e Sintesi Vocale non possono essere usati allo stesso tempo.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Click here to toggle sound notification for %1 events.&lt;br /&gt;If checked, Mumble uses a sound file predefined by you to indicate %1 events. Sound files and Text-To-Speech cannot be used at the same time.</source>
        <oldsource>Path to soundfile used for sound notifications in the case of %1 events.&lt;br /&gt;Single click to play&lt;br /&gt;Doubleclick to change&lt;br /&gt;Be sure that sound notifications for these events are enabled or this field will not have any effect.</oldsource>
        <translation>Premi qui per impostare le notifiche sonore per gli eventi %1.&lt;br /&gt;Se selezionato, Mumble userà un file definito dall&apos;utente per indicare gli eventi %1. File sonori e sintesi vocale non possono essere usati allo stesso tempo.</translation>
    </message>
    <message>
        <source>Choose sound file</source>
        <translation type="obsolete">Scegli il file del suono</translation>
    </message>
    <message>
        <source>Invalid sound file</source>
        <translation type="obsolete">Suono del file invalido</translation>
    </message>
    <message>
        <source>The file &apos;%1&apos; does not exist or is not a valid speex file.</source>
        <translation type="obsolete">Il file &quot;%1&quot; non esiste o è un file speex invalido.</translation>
    </message>
    <message>
        <location filename="Log.ui"/>
        <source>Messages</source>
        <translation>Messaggi</translation>
    </message>
    <message>
        <location/>
        <source>Message</source>
        <translation>Messaggio</translation>
    </message>
    <message>
        <location/>
        <source>Console</source>
        <translation>Console</translation>
    </message>
    <message>
        <location/>
        <source>Notification</source>
        <translation>Notifiche</translation>
    </message>
    <message>
        <location/>
        <source>Text-To-Speech</source>
        <translation>Sintesi vocale</translation>
    </message>
    <message>
        <location/>
        <source>Soundfile</source>
        <translation>File sonoro</translation>
    </message>
    <message>
        <location/>
        <source>Path</source>
        <translation>Percorso</translation>
    </message>
    <message>
        <location/>
        <source>Text To Speech</source>
        <translation>Sintesi vocale</translation>
    </message>
    <message>
        <location/>
        <source>Volume</source>
        <translation>Volume</translation>
    </message>
    <message>
        <location/>
        <source>Volume of Text-To-Speech Engine</source>
        <translation>Volume del motore di Sintesi Vocale</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is the volume used for the speech synthesis.&lt;/b&gt;</source>
        <translation>&lt;b&gt;Volume usato per la sintesi vocale.&lt;/b&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Length threshold</source>
        <translation>Soglia di lunghezza</translation>
    </message>
    <message>
        <location/>
        <source>Message length threshold for Text-To-Speech Engine</source>
        <translation>Soglia di lunghezza messaggio per il Motore di Sintesi Vocale</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is the length threshold used for the Text-To-Speech Engine.&lt;/b&gt;&lt;br /&gt;Messages longer than this limit will not be read aloud in their full length.</source>
        <translation>&lt;b&gt;Soglia di lunghezza usata per il Motore di Sintesi Vocale.&lt;/b&gt;&lt;br&gt;I messaggi più lunghi di questo limite non verranno letti.</translation>
    </message>
    <message>
        <location/>
        <source> Characters</source>
        <translation> Caratteri</translation>
    </message>
    <message>
        <location/>
        <source>Whisper</source>
        <translation>Sussurra</translation>
    </message>
    <message>
        <location/>
        <source>If checked you will only hear whispers from users you added to your friend list.</source>
        <translation>Se selezionato sentirai sussurri solo dagli utenti aggiunti alla tua lista amici.</translation>
    </message>
    <message>
        <location/>
        <source>Only accept whispers from friends</source>
        <translation>Accetta sussurri solo dagli amici</translation>
    </message>
</context>
<context>
    <name>LookConfig</name>
    <message>
        <location filename="LookConfig.cpp" line="+52"/>
        <location line="+11"/>
        <source>System default</source>
        <translation>Predefinito del sistema</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>None</source>
        <translation>Niente</translation>
    </message>
    <message>
        <source>Only with players</source>
        <translation type="obsolete">Solo con i giocatori</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Only with users</source>
        <translation>Solo con utenti</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>All</source>
        <translation>Tutti</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Ask</source>
        <translation>Chiedi</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Do Nothing</source>
        <translation>Non fare niente</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Move</source>
        <translation>Sposta</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>User Interface</source>
        <translation>Interfaccia Utente</translation>
    </message>
    <message>
        <location line="+130"/>
        <source>Choose skin file</source>
        <translation>Scegli file della skin</translation>
    </message>
    <message>
        <source>Form</source>
        <translation type="obsolete">Modulo</translation>
    </message>
    <message>
        <location filename="LookConfig.ui"/>
        <source>This sets which channels to automatically expand. &lt;i&gt;None&lt;/i&gt; and &lt;i&gt;All&lt;/i&gt; will expand no or all channels, while &lt;i&gt;Only with users&lt;/i&gt; will expand and collapse channels as users join and leave them.</source>
        <translation>Imposta quali canali saranno automaticamente espansi. &lt;i&gt;Nessuno&lt;/i&gt; e &lt;i&gt;Tutti&lt;/i&gt; espanderanno o no tutti i canali, mentre &lt;i&gt;Solo con utenti&lt;/i&gt; espanderà e comprimerà i canali solo quando gli utenti entreranno o usciranno dai canali.</translation>
    </message>
    <message>
        <location/>
        <source>List users above subchannels (requires restart).</source>
        <translation>Elenca gli utenti sopra i sottocanali (richiede riavvio di Mumble).</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;If set, users will be shown above subchannels in the channel view.&lt;/b&gt;&lt;br /&gt;A restart of Mumble is required to see the change.</source>
        <translation>&lt;b&gt;Se selezionato, gli utenti saranno mostrati sopra i sottocanali.&lt;/b&gt;&lt;br /&gt;Un riavvio di Mumble è richiesto per vedere i cambiamenti.</translation>
    </message>
    <message>
        <location/>
        <source>Users above Channels</source>
        <translation>Utenti sopra i canali</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;If set, will verify you want to quit if connected.&lt;/b&gt;</source>
        <translation>&lt;b&gt;Se selezionato, all&apos;uscita, se connesso ad un server, verificherà se vuoi davvero uscire&lt;/b&gt;</translation>
    </message>
    <message>
        <source>Check to show chat bar</source>
        <translation type="obsolete">Seleziona per visualizzare la barra della chat</translation>
    </message>
    <message>
        <source>&lt;b&gt;If checked the chat bar is shown.&lt;/b&gt;&lt;br /&gt;Uncheck this to hide it.</source>
        <oldsource>&lt;b&gt;If checked the chat bar is shown&lt;/b&gt;&lt;br /&gt;Uncheck to hide it</oldsource>
        <translation type="obsolete">&lt;b&gt;Se selezionato la barra della chat sarà visibile&lt;/b&gt;&lt;br /&gt;Deselezionalo per nasconderla.</translation>
    </message>
    <message>
        <source>Show chatbar</source>
        <translation type="obsolete">Visualizza chatbar</translation>
    </message>
    <message>
        <location/>
        <source>Show number of users in each channel</source>
        <translation>Visualizza il numero degli utenti in ogni canale</translation>
    </message>
    <message>
        <location/>
        <source>Show channel user count</source>
        <translation>Visualizza numero utenti</translation>
    </message>
    <message>
        <location/>
        <source>Language</source>
        <translation>Linguaggio</translation>
    </message>
    <message>
        <location/>
        <source>Language to use (requires restart)</source>
        <translation>Linguaggio da utilizzare (richiede riavvio)</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets which language Mumble should use.&lt;/b&gt;&lt;br /&gt;You have to restart Mumble to use the new language.</source>
        <translation>&lt;b&gt;Imposta quale linguaggio deve utilizzare Mumble.&lt;/b&gt;&lt;br /&gt;Devi riavviare Mumble per utilizzare il nuovo linguaggio.</translation>
    </message>
    <message>
        <location/>
        <source>Look and Feel</source>
        <translation>Aspetto</translation>
    </message>
    <message>
        <location/>
        <source>Layout</source>
        <translation>Disposizione</translation>
    </message>
    <message>
        <location/>
        <source>Classic</source>
        <translation>Classica</translation>
    </message>
    <message>
        <location/>
        <source>Stacked</source>
        <translation>Impilata</translation>
    </message>
    <message>
        <location/>
        <source>Hybrid</source>
        <translation>Ibrida</translation>
    </message>
    <message>
        <location/>
        <source>Custom</source>
        <translation>Personalizzata</translation>
    </message>
    <message>
        <location/>
        <source>This changes the behavior when moving channels.</source>
        <translation>Questo cambia il comportamento quando sposterai dei canali.</translation>
    </message>
    <message>
        <location/>
        <source>This sets the behavior of channel drags; it can be used to prevent accidental dragging. &lt;i&gt;Move&lt;/i&gt; moves the channel without prompting. &lt;i&gt;Do Nothing&lt;/i&gt; does nothing and prints an error message. &lt;i&gt;Ask&lt;/i&gt; uses a message box to confirm if you really wanted to move the channel.</source>
        <oldsource>This sets the behavior of channel drags; it can be used to prevent accidental dragging. &lt;i&gt;Move Channel&lt;/i&gt; moves the channel without prompting. &lt;i&gt;Do Nothing&lt;/i&gt; does nothing and prints an error message. &lt;i&gt;Ask&lt;/i&gt; uses a message box to confirm if you really wanted to move the channel.</oldsource>
        <translation>Imposta il comportamento dei trascinamenti di canale, può essere utilizzato per evitare il trascinamento accidentale. &lt;i&gt;Sposta&lt;/i&gt; il canale si sposta senza chiedere conferma. &lt;i&gt;Non fare nulla&lt;/i&gt; non sposta e visualizza un messaggio di errore. &lt;i&gt;Chiedi&lt;/i&gt; mostra una finestra di dialogo per chiedere conferma se si voleva davvero spostare il canale.</translation>
    </message>
    <message>
        <location/>
        <source>Style</source>
        <translation>Stile</translation>
    </message>
    <message>
        <location/>
        <source>Basic widget style</source>
        <translation>Stile di base dei widget</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the basic look and feel to use.&lt;/b&gt;</source>
        <translation>&lt;b&gt;Imposta l&apos;aspetto e la sensazione di base da utilizzare.&lt;/b&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Skin</source>
        <translation>Skin</translation>
    </message>
    <message>
        <location/>
        <source>Skin file to use</source>
        <translation>File di skin da usare</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets which skin Mumble should use.&lt;/b&gt;&lt;br /&gt;The skin is a style file applied on top of the basic widget style. If there are icons in the same directory as the style sheet, those will replace the default icons.</source>
        <translation>&lt;b&gt;Stabilisce quale skin dovrebbe usare Mumble.&lt;/b&gt;&lt;br /&gt;La skin è un file di stile applicato sopra allo stile base dei widget. Se ci sono icone nella stessa directory del foglio di stile, sostituiranno le icone predefinite.</translation>
    </message>
    <message>
        <source>...</source>
        <translation type="obsolete">...</translation>
    </message>
    <message>
        <location/>
        <source>Expand</source>
        <translation>Espandi</translation>
    </message>
    <message>
        <location/>
        <source>When to automatically expand channels</source>
        <translation>Quando espandere automaticamente i canali</translation>
    </message>
    <message>
        <source>This sets which channels to automatically expand. &lt;i&gt;None&lt;/i&gt; and &lt;i&gt;All&lt;/i&gt; will expand no or all channels, while &lt;i&gt;Only with players&lt;/i&gt; will expand and collapse channels as players join and leave them.</source>
        <translation type="obsolete">Stabilisce quali canali espandere automaticamente. &lt;i&gt;Nessuno&lt;/i&gt; e &lt;i&gt;Tutti&lt;/i&gt; non espanderà nessuno o tutti i canali, mentre &lt;i&gt;Solo con i giocatori&lt;/i&gt; espanderà e comprimerà i canali appena i giocatori entrano o escono.</translation>
    </message>
    <message>
        <source>List players above subchannels (requires restart).</source>
        <translation type="obsolete">Elenca i giocatori sui sottocanali (richiede riavvio).</translation>
    </message>
    <message>
        <source>&lt;b&gt;If set, players will be shown above subchannels in the channel view.&lt;/b&gt;&lt;br /&gt;A restart of Mumble is required to see the change.</source>
        <translation type="obsolete">&lt;b&gt;Se impostato, i giocatori verranno visualizzati sopra i sottocanali mostrati.&lt;/b&gt;&lt;br /&gt;E&apos; necessario un riavvio di Mumble per vedere il cambiamento.</translation>
    </message>
    <message>
        <source>Players above Channels</source>
        <translation type="obsolete">Giocatori sui Canali</translation>
    </message>
    <message>
        <location/>
        <source>Channel Dragging</source>
        <translation>Trascinamento Canale</translation>
    </message>
    <message>
        <source>This changes the behaviour when moving channels.</source>
        <translation type="obsolete">Cambia il comportamento quando sposti i canali.</translation>
    </message>
    <message>
        <source>This sets the behaviour of channel drags, it can be used to prevent accidental dragging. &lt;i&gt;Move Channel&lt;/i&gt; moves the channel without prompting. &lt;i&gt;Do Nothing&lt;/i&gt; does nothing and prints an error message. &lt;i&gt;Ask&lt;/i&gt; uses a message box to confirm if you really wanted to move the channel.</source>
        <translation type="obsolete">Imposta il comportamento dei trascinamenti di canale, può essere utilizzato per evitare il trascinamento accidentale. &lt;i&gt;Sposta Canale&lt;/i&gt; il canale si sposta senza chiedere conferma. &lt;i&gt;Non fare nulla&lt;/i&gt; non li sposta e stampa un messaggio di errore. &lt;i&gt;Chiedi&lt;/i&gt; mostra una finestra di dialogo per chiedere conferma se si voleva davvero spostare il canale.</translation>
    </message>
    <message>
        <location/>
        <source>Ask whether to close or minimize when quitting Mumble.</source>
        <translation>Chiede se chiudere o minimizzare la finestra all&apos;uscita da Mumble.</translation>
    </message>
    <message>
        <source>&lt;b&gt;If set, will verify you want to quit if connected.&lt;b&gt;</source>
        <translation type="obsolete">&lt;b&gt;Se impostato, chiederà conferma che tu voglia veramente uscire quando sei collegato.&lt;b&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Ask on quit while connected</source>
        <translation>Chiedi conferma all&apos;uscita se connesso</translation>
    </message>
    <message>
        <source>Make the Mumble window appear on top of other windows.</source>
        <translation type="obsolete">Tieni la finestra di Mumble sopra alle altre finestre.</translation>
    </message>
    <message>
        <source>&lt;b&gt;This makes the Mumble window a topmost window.&lt;/b&gt;</source>
        <translation type="obsolete">&lt;b&gt;Tiene la finestra di Mumble sopra le altre.&lt;/b&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Always On Top</source>
        <translation>Sempre in primo piano</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;If set, minimizing the Mumble main window will cause it to be hidden and accessible only from the tray. Otherwise, it will be minimized as a window normally would.&lt;/b&gt;</source>
        <translation>&lt;b&gt;Se impostato, minimizzare la finestra principale di Mumble e la nascoderà in tray, unico posto in cui sarà accessibile. Altrimenti sarà minimizzata come una normale finestra.&lt;/b&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Hide in tray when minimized</source>
        <translation>Nascondi in tray quando minimizzato</translation>
    </message>
    <message>
        <source>Show all configuration items</source>
        <translation type="obsolete">Mostra tutte le voci di configurazione</translation>
    </message>
    <message>
        <source>&lt;b&gt;This will show all configuration items.&lt;/b&gt;&lt;br /&gt;Mumble contains a lot of configuration items that most users won&apos;t need to change. Checking this will show all configurable items.</source>
        <translation type="obsolete">&lt;b&gt;Mostrerà tutti gli elementi di configurazione.&lt;/b&gt;&lt;br /&gt;Mumble ha tante voci di configurazione che la maggior parte degli utenti non avranno bisogno di cambiare. Attivando questa voce mostrerà tutti gli elementi configurabili.</translation>
    </message>
    <message>
        <source>Expert Config</source>
        <translation type="obsolete">Configurazione Avanzata</translation>
    </message>
    <message>
        <location/>
        <source>Hide the main Mumble window in the tray when it is minimized.</source>
        <translation>Nascondi la finestra principale di Mumble in tray quando è minimizzata.</translation>
    </message>
    <message>
        <location/>
        <source>This setting controls when the application will be always on top.</source>
        <translation>Questa impostazione controlla quando l&apos;applicazione deve rimanere sempre in primo piano.</translation>
    </message>
    <message>
        <source>This setting controls in which situations the application will stay always on top. If you select &lt;i&gt;Never&lt;/i&gt; the application will not stay on top. &lt;i&gt;Always&lt;/i&gt; will always keep the application on top. &lt;i&gt;In minimal view&lt;/i&gt; / &lt;i&gt;In normal mode&lt;/i&gt; will only keep the application always on top when minimal view is activated / deactivated.</source>
        <translation type="obsolete">Questa impostazione controlla in quali situazioni l&apos;applicazione deve rimanere sempre in primo piano. Se selezioni &lt;i&gt;Mai&lt;/i&gt; l&apos;applicazione non starà sopra le altre. &lt;i&gt;Sempre&lt;/i&gt; manterrà l&apos;applicazione sempre sopra le altre. &lt;i&gt;In visuale minima&lt;/i&gt; / &lt;i&gt;In visuale normale&lt;/i&gt; manterrà l&apos;applicazione sopra le altre quando la visuale minima è attivata / disattivata.</translation>
    </message>
    <message>
        <location/>
        <source>Never</source>
        <translation>Mai</translation>
    </message>
    <message>
        <location/>
        <source>Always</source>
        <translation>Sempre</translation>
    </message>
    <message>
        <location/>
        <source>In minimal view</source>
        <translation>In visuale minima</translation>
    </message>
    <message>
        <location/>
        <source>In normal view</source>
        <translation>In visuale normale</translation>
    </message>
    <message>
        <location/>
        <source>Displays talking status in system tray</source>
        <translation>Visualizza stato del parlato nella tray di sistema</translation>
    </message>
    <message>
        <location/>
        <source>Show talking status in tray icon</source>
        <translation>Visualizza stato nell&apos;icona in tray</translation>
    </message>
    <message>
        <location/>
        <source>This setting controls in which situations the application will stay always on top. If you select &lt;i&gt;Never&lt;/i&gt; the application will not stay on top. &lt;i&gt;Always&lt;/i&gt; will always keep the application on top. &lt;i&gt;In minimal view&lt;/i&gt; / &lt;i&gt;In normal view&lt;/i&gt; will only keep the application always on top when minimal view is activated / deactivated.</source>
        <translation>Questa impostazione controlla in quali situazioni l&apos;applicazione deve rimanere sempre in primo piano. Se selezioni &lt;i&gt;Mai&lt;/i&gt; l&apos;applicazione non starà sopra le altre. &lt;i&gt;Sempre&lt;/i&gt; manterrà l&apos;applicazione sempre sopra le altre. &lt;i&gt;In visuale minima&lt;/i&gt; / &lt;i&gt;In visuale normale&lt;/i&gt; manterrà l&apos;applicazione sopra le altre quando la visuale minima è attivata / disattivata.</translation>
    </message>
    <message>
        <location/>
        <source>Show context menu in menu bar</source>
        <translation>Visualizza menu contestuale nella barra del menu</translation>
    </message>
    <message>
        <location/>
        <source>Apply some high contrast optimizations for visually impaired users</source>
        <translation>Applica alcune ottimizzazioni di contrasto elevato per gli utenti con problemi visivi</translation>
    </message>
    <message>
        <location/>
        <source>Optimize for high contrast</source>
        <translation>Ottimizza per contrasto elevato</translation>
    </message>
    <message>
        <location/>
        <source>Application</source>
        <translation>Applicazione</translation>
    </message>
    <message>
        <location/>
        <source>Adds user and channel context menus into the menu bar</source>
        <translation>Aggiunge menu contestuale per l&apos;utente e il canale nella barra del menu</translation>
    </message>
    <message>
        <location/>
        <source>Tray Icon</source>
        <translation>Icona in tray</translation>
    </message>
    <message>
        <location/>
        <source>Channel Tree</source>
        <translation>Albero dei canali</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Browse...</source>
        <translation>&amp;Sfoglia...</translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <location filename="MainWindow.cpp" line="+171"/>
        <location line="+2048"/>
        <source>Root</source>
        <translation>Radice</translation>
    </message>
    <message>
        <location line="-1994"/>
        <source>Push-to-Talk</source>
        <comment>Global Shortcut</comment>
        <translation>Premi-per-parlare</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Push and hold this button to send voice.</source>
        <comment>Global Shortcut</comment>
        <translation>Tieni premuto questo bottone per inviare la voce.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This configures the push-to-talk button, and as long as you hold this button down, you will transmit voice.</source>
        <comment>Global Shortcut</comment>
        <translation>Questo configura il bottone Premi-Per-Parlare, e fino a quando manterrai premuto questo bottone, trasmetterai la voce.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Reset Audio Processor</source>
        <comment>Global Shortcut</comment>
        <translation>Reset processore audio</translation>
    </message>
    <message>
        <source>Toggle Mute Self</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">Cambia Silenziarti</translation>
    </message>
    <message>
        <source>Toggle self-mute status.</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">Cambia stato Silenziati.</translation>
    </message>
    <message>
        <source>This will toggle your muted status. If you toggle this off, you will also disable self-deafen.</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">Questo cambia il tuo stato silenziati. Se la selezione è off, disabiliterai anche l&apos;Escluditi.</translation>
    </message>
    <message>
        <source>Toggle Deafen Self</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">Cambia escluditi</translation>
    </message>
    <message>
        <source>Toggle self-deafen status.</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">Cambia stato Escluditi.</translation>
    </message>
    <message>
        <source>This will toggle your deafen status. If you toggle this on, you will also enable self-mute.</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">Questo cambia il tuo stato Escluditi. Se la selezione è on, sarà abilitato ache il Silenziati.</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Unlink Plugin</source>
        <comment>Global Shortcut</comment>
        <translation>Scollega Plugin</translation>
    </message>
    <message>
        <source>Force Center Position</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">Forza posizione centrale</translation>
    </message>
    <message>
        <source>Chan Parent</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">Parente canale</translation>
    </message>
    <message>
        <source>Chan Sub#%1</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">Sotto-canale#%1</translation>
    </message>
    <message>
        <source>Chan All Subs</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">Canale tutti sotto-canali</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Push-to-Mute</source>
        <comment>Global Shortcut</comment>
        <translation>Premi-per-mutare</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Join Channel</source>
        <comment>Global Shortcut</comment>
        <translation>Entra nel canale</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Toggle Overlay</source>
        <comment>Global Shortcut</comment>
        <translation>Imposta la sovrapposizione</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Toggle state of in-game overlay.</source>
        <comment>Global Shortcut</comment>
        <translation>Imposta stato di sovrapposizione in-game.</translation>
    </message>
    <message>
        <source>This will switch the states of the ingame overlay between showing everybody, just the players who are talking, and nobody.</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">Questo cambia lo stato della sovrapposizione in-game tra visualizza tutti, solo il giocatore a cui stai parlando e nessuno.</translation>
    </message>
    <message>
        <source>Alt Push-to-Talk</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">Alt Premi-per.parlare</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Toggle Minimal</source>
        <comment>Global Shortcut</comment>
        <translation>Imposta Visuale minima</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Volume Up (+10%)</source>
        <comment>Global Shortcut</comment>
        <translation>Aumenta volume (+10%)</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Volume Down (-10%)</source>
        <comment>Global Shortcut</comment>
        <translation>Diminuisci volume (-10%)</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>Mumble</source>
        <translation>Mumble</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="+4"/>
        <location line="+12"/>
        <location line="+2159"/>
        <source>Mumble -- %1</source>
        <translation>Mumble -- %1</translation>
    </message>
    <message>
        <location line="-2155"/>
        <source>&amp;Window</source>
        <translation>&amp;Finestra</translation>
    </message>
    <message>
        <location line="+2"/>
        <location line="+126"/>
        <source>Minimize</source>
        <translation>Minimizza</translation>
    </message>
    <message>
        <location line="-126"/>
        <source>Ctrl+M</source>
        <translation>Ctrl+M</translation>
    </message>
    <message>
        <location line="+125"/>
        <source>Close</source>
        <translation>Chiudi</translation>
    </message>
    <message>
        <location line="-1"/>
        <source>Mumble is currently connected to a server. Do you want to Close or Minimize it?</source>
        <translation>Mumble è attualmente connesso ad un server. Vuoi Chiuderlo o Minimizzarlo?</translation>
    </message>
    <message>
        <location line="-177"/>
        <source>Mute Self</source>
        <comment>Global Shortcut</comment>
        <translation>Mutati</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Set self-mute status.</source>
        <comment>Global Shortcut</comment>
        <translation>Imposta il tuo stato mutati.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This will set or toggle your muted status. If you turn this off, you will also disable self-deafen.</source>
        <comment>Global Shortcut</comment>
        <translation>Questo imposta o cambia lo stato di mutati. Se lo imposti su OFF, disabiliterai anche la disattivazione automatica.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Deafen Self</source>
        <comment>Global Shortcut</comment>
        <translation>Disattivati</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Set self-deafen status.</source>
        <comment>Global Shortcut</comment>
        <translation>Imposta stato di microfono e audio disattivati (tuo).</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This will set or toggle your deafened status. If you turn this on, you will also enable self-mute.</source>
        <comment>Global Shortcut</comment>
        <translation>Questo imposta o cambia lo stato di audio e microfono disattivato. Se lo imposti su ON, abiliterai anche la disattivazione automatica del microfono.</translation>
    </message>
    <message>
        <source>This will switch the states of the in-game overlay between showing everybody, just the users who are talking, and nobody.</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">Questo cambia lo stato della sovrapposizione in-game tra visualizza tutti, solo gli utenti che stanno parlando e nessuno.</translation>
    </message>
    <message>
        <location line="+30"/>
        <source>Whisper/Shout</source>
        <oldsource>Whisper</oldsource>
        <translation>Sussurra/Urla</translation>
    </message>
    <message>
        <location line="+58"/>
        <location line="+2189"/>
        <source>&lt;center&gt;Not connected&lt;/center&gt;</source>
        <oldsource>Not connected</oldsource>
        <translation>&lt;center&gt;Disconnesso&lt;/center&gt;</translation>
    </message>
    <message>
        <location line="-1905"/>
        <source>Clear</source>
        <translation>Cancella</translation>
    </message>
    <message>
        <location line="+27"/>
        <source>Opening URL %1</source>
        <translation>Apertura URL %1</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>File does not exist</source>
        <translation>Il file non esiste</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>File is not a configuration file.</source>
        <translation>Il file non è un file di configurazione.</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Settings merged from file.</source>
        <translation>Impostazioni unite con quelle del file.</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>URL scheme is not &apos;mumble&apos;</source>
        <translation>Lo schema dell&apos;URL non è &apos;mumble&apos;</translation>
    </message>
    <message>
        <location line="+19"/>
        <source>This version of Mumble can&apos;t handle URLs for Mumble version %1.%2.%3</source>
        <translatorcomment>Questa versione di Mumble non può utilizzare gli URL di Mumble versione %1.%2.%3</translatorcomment>
        <translation></translation>
    </message>
    <message>
        <location line="+29"/>
        <source>Connecting to %1</source>
        <translation>Connessione a %1</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Enter username</source>
        <translation>Inserisci nome utente</translation>
    </message>
    <message>
        <location line="+14"/>
        <location line="+176"/>
        <source>Connecting to server %1.</source>
        <translation>Connessione al server %1.</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Reconnecting.</source>
        <translation>Riconnessione.</translation>
    </message>
    <message>
        <location line="+40"/>
        <source>Change your comment</source>
        <translation>Cambia il tuo commento</translation>
    </message>
    <message>
        <location line="+92"/>
        <source>&lt;h2&gt;Version&lt;/h2&gt;&lt;p&gt;Protocol %1.%2.%3.&lt;/p&gt;</source>
        <translation>&lt;h2&gt;Versione&lt;/h2&gt;&lt;p&gt;Protocollo %1.%2.%3.&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>&lt;p&gt;No build information or OS version available.&lt;/p&gt;</source>
        <translation>&lt;p&gt;Nessuna informazione disponibile sul SO.&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>&lt;p&gt;%1 (%2)&lt;br /&gt;%3&lt;/p&gt;</source>
        <translation>&lt;p&gt;%1 (%2)&lt;br /&gt;%3&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>&lt;h2&gt;Control channel&lt;/h2&gt;&lt;p&gt;Encrypted with %1 bit %2&lt;br /&gt;%3 ms average latency (%4 deviation)&lt;/p&gt;&lt;p&gt;Remote host %5 (port %6)&lt;/p&gt;</source>
        <translation>&lt;h2&gt;Canale di controllo&lt;/h2&gt;&lt;p&gt;Crittografato con %1 bit %2&lt;br/&gt;%3 ms latenza media (%4 deviazione)&lt;/p&gt;&lt;p&gt;Host remoto %5 (porta%6)&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+1570"/>
        <source>Images (*.png *.jpg *.jpeg *.svg)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Images (*.png *.jpg *.svg)</source>
        <translation type="obsolete">Immagini (*.png *.jpg *.svg)</translation>
    </message>
    <message>
        <source>&lt;h2&gt;Control channel&lt;/h2&gt;&lt;p&gt;Encrypted with %1 bit %2&lt;br /&gt;%3 ms average latency (%4 deviation)&lt;/p&gt;</source>
        <translation type="obsolete">&lt;h2&gt;Canale di controllo&lt;/h2&gt;&lt;p&gt;Crittografato con %1 bit %2&lt;br/&gt;%3 ms latenza media (%4 deviazione)&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="-1559"/>
        <source>&lt;h2&gt;Voice channel&lt;/h2&gt;&lt;p&gt;Encrypted with 128 bit OCB-AES128&lt;br /&gt;%1 ms average latency (%4 deviation)&lt;/p&gt;</source>
        <translation>&lt;h2&gt;Canale di voce&lt;/h2&gt;&lt;p&gt;Crittografato con 128 bit OCB-AES128&lt;br/&gt;%1 ms latenza media (%4 deviazione)&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+11"/>
        <source>&lt;h2&gt;Audio bandwidth&lt;/h2&gt;&lt;p&gt;Maximum %1 kbit/s&lt;br /&gt;Current %2 kbit/s&lt;/p&gt;</source>
        <translation>&lt;h2&gt;banda audio&lt;/h2&gt;&lt;p&gt;Massimo %1 kbit/s&lt;br/&gt;Attuale %2 kbit/s&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="-104"/>
        <location line="+327"/>
        <source>Register yourself as %1</source>
        <translation>Registrati come %1</translation>
    </message>
    <message>
        <location line="-1052"/>
        <source>This will switch the states of the in-game overlay.</source>
        <comment>Global Shortcut</comment>
        <translation>Cambia lo stato della sovrapposizione in gioco.</translation>
    </message>
    <message>
        <location line="+725"/>
        <location line="+327"/>
        <source>&lt;p&gt;You are about to register yourself on this server. This action cannot be undone, and your username cannot be changed once this is done. You will forever be known as &apos;%1&apos; on this server.&lt;/p&gt;&lt;p&gt;Are you sure you want to register yourself?&lt;/p&gt;</source>
        <translation>&lt;p&gt;Stai per registrarti sul server. Quest&apos;azione non può essere annullata, il tuo nome utente non potrà essere cambiato. Sarai per sempre conosciuto come &apos;%1&apos; su questo server.&lt;p&gt;Se sicuro di volerti registrare?&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Register user %1</source>
        <translation>Registra utente %1</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>&lt;p&gt;You are about to register %1 on the server. This action cannot be undone, the username cannot be changed, and as a registered user, %1 will have access to the server even if you change the server password.&lt;/p&gt;&lt;p&gt;From this point on, %1 will be authenticated with the certificate currently in use.&lt;/p&gt;&lt;p&gt;Are you sure you want to register %1?&lt;/p&gt;</source>
        <translation>&lt;p&gt;Stai per registrare %1 sul server. Quest&apos;azione non può essere annullata, il nome utente non può essere cambiato, ed essendo un utente registrato, %1 avrà accesso al server anche se il server cambierà password.&lt;/p&gt;&lt;p&gt;Da questo punto in poi, %1 sarà autenticato con il certificato attualmente in uso.&lt;/p&gt;&lt;p&gt;Se sicuro di voler registrare %1?&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+40"/>
        <source>Kicking user %1</source>
        <translation>Espulsione utente %1</translation>
    </message>
    <message>
        <location line="+19"/>
        <source>Banning user %1</source>
        <translation>Bandimento utente %1</translation>
    </message>
    <message>
        <source>Change comment on user %1</source>
        <translation type="obsolete">Cambia commento sull&apos;utente %1</translation>
    </message>
    <message>
        <location line="+63"/>
        <source>View comment on user %1</source>
        <translation>Visualizza commento dell&apos;utente %1</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Are you sure you want to reset the comment of user %1?</source>
        <translation>Sei sicuro di voler cancellare il commento dell&apos;utente%1?</translation>
    </message>
    <message>
        <source>To channel %1: %2</source>
        <translation type="obsolete">Al canale %1: %2</translation>
    </message>
    <message>
        <location line="+39"/>
        <location line="+236"/>
        <source>Message to channel %1</source>
        <translation>Messaggio al canale %1</translation>
    </message>
    <message>
        <location line="-2"/>
        <source>To %1 (Tree): %2</source>
        <oldsource>(Tree) %1: %2</oldsource>
        <translation>Al canale %1 e ai suoi sottocanali:%2</translation>
    </message>
    <message>
        <location line="-318"/>
        <location line="+84"/>
        <location line="+4"/>
        <location line="+232"/>
        <source>To %1: %2</source>
        <oldsource>%1: %2</oldsource>
        <translation>Per %1: %2</translation>
    </message>
    <message>
        <location line="+490"/>
        <source>Connected.</source>
        <translation>Connesso.</translation>
    </message>
    <message>
        <location line="+159"/>
        <source>SSL Version mismatch</source>
        <translation>Versione SSL diversa</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>This server is using an older encryption standard. It might be an older 1.1 based Mumble server.&lt;br /&gt;Would you like to launch the compatibility client to connect to it?</source>
        <translation>Questo server utilizza un vecchio standard di cifratura. Potrebbe essere un vecchio server basato su Mumble 1.1 .&lt;br /&gt;Vorresti aprire il client retrocompatibile per connetterti a questo server?</translation>
    </message>
    <message>
        <location line="+26"/>
        <source>The compatibility client could not be found, or failed to start.&lt;br /&gt;Note that the compatibility client is an optional component for most installations, and might not be installed.</source>
        <translation>Il client retrocompatibile non è stato trovato, o la sua apertura è fallita.&lt;br /&gt;Nota che il client retrocompatibile è un componente facoltativo in molte installazioni, e potrebbe non essere installato.</translation>
    </message>
    <message>
        <location line="+127"/>
        <source>&lt;center&gt;Type message to channel &apos;%1&apos; here&lt;/center&gt;</source>
        <oldsource>Type message to channel &apos;%1&apos; here</oldsource>
        <translation>&lt;center&gt;Inserisci qui il messaggio per il canale &apos;%1&apos;&lt;/center&gt;</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>&lt;center&gt;Type message to user &apos;%1&apos; here&lt;/center&gt;</source>
        <oldsource>Type message to user &apos;%1&apos; here</oldsource>
        <translation>&lt;center&gt;inserisci qui il messaggio per l&apos;utente &apos;%1&apos;&lt;/center&gt;</translation>
    </message>
    <message>
        <location line="+105"/>
        <source>Choose image file</source>
        <translation>Scegli file immagine</translation>
    </message>
    <message>
        <source>Images (*.png *.jpg)</source>
        <translation type="obsolete">Immagini (*.png *.jpg)</translation>
    </message>
    <message>
        <location line="+7"/>
        <location line="+17"/>
        <source>Failed to load image</source>
        <translation>Caricamento immagine fallito</translation>
    </message>
    <message>
        <location line="-17"/>
        <source>Could not open file for reading.</source>
        <translation>Impossibile aprire il file per la lettura.</translation>
    </message>
    <message>
        <location line="+17"/>
        <source>Image format not recognized.</source>
        <translation>Formato dell&apos;immagine non riconosciuto.</translation>
    </message>
    <message>
        <source>&lt;h2&gt;Control channel&lt;/h2&gt;&lt;p&gt;Encrypted with %1 bit %2&lt;br /&gt;%3 ms average latency (%4 variance)&lt;/p&gt;</source>
        <translation type="obsolete">&lt;h2&gt;Canale di controllo&lt;/h2&gt;&lt;p&gt;Crittografato con %1 bit %2&lt;br/&gt;%3 ms latenza media (%4 variazione)&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="-1585"/>
        <source>Voice channel is sent over control channel.</source>
        <translation>Il canale di voce è trasmesso sul canale di controllo.</translation>
    </message>
    <message>
        <source>&lt;h2&gt;Voice channel&lt;/h2&gt;&lt;p&gt;Encrypted with 128 bit OCB-AES128&lt;br /&gt;%1 ms average latency (%4 variance)&lt;/p&gt;</source>
        <translation type="obsolete">&lt;h2&gt;Canale di voce&lt;/h2&gt;&lt;p&gt;Crittografato con 128 bit OCB-AES128&lt;br/&gt;%1 ms latenza media (%4 variazione)&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>UDP Statistics</source>
        <translation>Statistiche UDP</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>To Server</source>
        <translation>Al server</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>From Server</source>
        <translation>Dal server</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Good</source>
        <translation>Buono</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Late</source>
        <translation>Ultimo</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Lost</source>
        <translation>Perso</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Resync</source>
        <translation>Risincronizzazione</translation>
    </message>
    <message>
        <source>&lt;h2&gt;Audio bandwidth&lt;/h2&gt;&lt;p&gt;Maximum %1 kbit/s&lt;br /&gt;Current %2 kbit/s (Quality %3)&lt;/p&gt;</source>
        <translation type="obsolete">&lt;h2&gt;banda audio&lt;/h2&gt;&lt;p&gt;Massimo %1 kbit/s&lt;br/&gt;Corrente %2 kbit/s (Qualità %3)&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Mumble Server Information</source>
        <translation>Informazioni del server di Mumble</translation>
    </message>
    <message>
        <location line="+4"/>
        <location line="+1264"/>
        <source>&amp;View Certificate</source>
        <translation>&amp;Visualizza certificato</translation>
    </message>
    <message>
        <source>Kicking player %1</source>
        <translation type="obsolete">Espulsione giocatore %1</translation>
    </message>
    <message>
        <location line="-1005"/>
        <location line="+19"/>
        <source>Enter reason</source>
        <translation>Inserisci motivazione</translation>
    </message>
    <message>
        <source>Banning player %1</source>
        <translation type="obsolete">Bandimento giocatore %1</translation>
    </message>
    <message>
        <location line="+22"/>
        <source>Sending message to %1</source>
        <translation>Invio messaggio a %1</translation>
    </message>
    <message>
        <location line="+12"/>
        <location line="+88"/>
        <source>Message to %1</source>
        <translation>Messaggio a %1</translation>
    </message>
    <message>
        <source>Channel Name</source>
        <translation type="obsolete">Nome canale</translation>
    </message>
    <message>
        <location line="+150"/>
        <source>Are you sure you want to delete %1 and all its sub-channels?</source>
        <translation>Sei sicuro di voler cancellare %1 e tutti i suoi sottocanali?</translation>
    </message>
    <message>
        <source>Change description of channel %1</source>
        <translation type="obsolete">Cambia la descrizione del canale %1</translation>
    </message>
    <message>
        <location line="+71"/>
        <source>Sending message to channel %1</source>
        <translation>Invio messaggio al canale %1</translation>
    </message>
    <message>
        <source>Sending message to channel tree %1</source>
        <translation type="obsolete">Invio messaggio all&apos;albero del canale %1</translation>
    </message>
    <message>
        <source>To tree %1: %2</source>
        <translation type="obsolete">Per l&apos;albero di canali %1: %2</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Message to tree %1</source>
        <translation>Messaggio al canale %1 ed ai suoi sottocanali</translation>
    </message>
    <message>
        <location line="+130"/>
        <source>Unmuted and undeafened.</source>
        <translation>Ti sei riattivato.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Unmuted.</source>
        <translation>Hai riattivato il tuo microfono.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Muted.</source>
        <translation>Ti sei mutato.</translation>
    </message>
    <message>
        <location line="+31"/>
        <source>Muted and deafened.</source>
        <translation>Ti sei escluso.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Deafened.</source>
        <translation>Ti sei escluso.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Undeafened.</source>
        <translation>Ti sei riattivato.</translation>
    </message>
    <message>
        <location line="+94"/>
        <source>About Qt</source>
        <translation>Informazioni su Qt</translation>
    </message>
    <message>
        <source>Joining %1.</source>
        <translation type="obsolete">Entrata %1.</translation>
    </message>
    <message>
        <source>Connected to server %1.</source>
        <translation type="obsolete">Connessione al server %1.</translation>
    </message>
    <message>
        <location line="+349"/>
        <source>SSL Verification failed: %1</source>
        <translation>Verifica SSL fallita: %1</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>&lt;b&gt;WARNING:&lt;/b&gt; The server presented a certificate that was different from the stored one.</source>
        <translation>&lt;b&gt;ATTENZIONE:&lt;/b&gt; Il server presenta un certificato che è differente da quello posseduto.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Sever presented a certificate which failed verification.</source>
        <translation>Il server ha presentato un certificato che ha fallito la verifica.</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>&lt;p&gt;%1.&lt;br /&gt;The specific errors with this certificate are: &lt;/p&gt;&lt;ol&gt;%2&lt;/ol&gt;&lt;p&gt;Do you wish to accept this certificate anyway?&lt;br /&gt;(It will also be stored so you won&apos;t be asked this again.)&lt;/p&gt;</source>
        <translation>&lt;p&gt;%1.&lt;br /&gt;L&apos;errore specifico con il certificato è: &lt;/p&gt;&lt;ol&gt;%2&lt;/ol&gt;&lt;p&gt;Vuoi accettare questo certificato ugualmente?&lt;br /&gt;(Questo sarà anche salvato quindi non ti sarà chiesto di nuovo.)&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+50"/>
        <source>Failed to launch compatibility client</source>
        <translation>Apertura client retrocompatibile fallita</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Server connection failed: %1.</source>
        <translation>Connessione al server fallita: %1.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Disconnected from server.</source>
        <translation>Disconnessione dal server.</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>Invalid username</source>
        <translation>Nome utente non valido</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>You connected with an invalid username, please try another one.</source>
        <translation>Ti sei connesso con un nome utente non valido, provane un&apos;altro.</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>That username is already in use, please try another username.</source>
        <translation>Questo nome utente è già in uso, prova con un&apos;altro nome.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Wrong password</source>
        <translation>Password errata</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Wrong password for registered users, please try again.</source>
        <translation>Password errata per gli utenti registrati, prova di nuovo.</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Wrong server password for unregistered user account, please try again.</source>
        <translation>Password errata per un account non registrato, prova di nuovo.</translation>
    </message>
    <message>
        <source>&amp;Server</source>
        <translation type="obsolete">&amp;Server</translation>
    </message>
    <message>
        <source>&amp;Player</source>
        <translation type="obsolete">&amp;Giocatore</translation>
    </message>
    <message>
        <location line="-2230"/>
        <source>&amp;Channel</source>
        <translation>&amp;Canale</translation>
    </message>
    <message>
        <source>&amp;Audio</source>
        <translation type="obsolete">&amp;Audio</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>C&amp;onfigure</source>
        <translation>&amp;Impostazioni</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Help</source>
        <translation>A&amp;iuto</translation>
    </message>
    <message>
        <location/>
        <source>Log</source>
        <translation>Log</translation>
    </message>
    <message>
        <location/>
        <source>This shows all recent activity. Connecting to servers, errors and information messages all show up here.&lt;br /&gt;To configure exactly which messages show up here, use the &lt;b&gt;Settings&lt;/b&gt; command from the menu.</source>
        <translation>Questo visualizza tutte le attività recenti.Connessioni ai server, errori e messaggi informativi sono visualizzati qui.&lt;br /&gt;Per configurare quali messaggi saranno visualizzati qui, usa il comando &lt;b&gt;Impostazioni&lt;/b&gt; dal menu.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Quit Mumble</source>
        <translation>&amp;Esci da Mumble</translation>
    </message>
    <message>
        <location/>
        <source>Closes the program</source>
        <translation>Chiude il programma</translation>
    </message>
    <message>
        <location/>
        <source>Exits the application.</source>
        <translation>Esce dall&apos;applicazione.</translation>
    </message>
    <message>
        <location/>
        <source>Ctrl+Q</source>
        <translation>Ctrl+Q</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Connect</source>
        <translation>&amp;Connetti</translation>
    </message>
    <message>
        <location/>
        <source>Open the server connection dialog</source>
        <translation>Apre la finestra della connessione al server</translation>
    </message>
    <message>
        <source>Shows a dialog of registered servers, and also allows quick connect.</source>
        <translation type="obsolete">Visualizza una finestra dei server registrati, e permette anche la connessione rapida.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Disconnect</source>
        <translation>&amp;Disconnetti</translation>
    </message>
    <message>
        <location/>
        <source>Disconnect from server</source>
        <translation>Disconnetti dal server</translation>
    </message>
    <message>
        <location/>
        <source>Disconnects you from the server.</source>
        <translation>Disconnette dal server.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Ban lists</source>
        <translation>&amp;Lista utenti banditi</translation>
    </message>
    <message>
        <location/>
        <source>Edit ban lists on server</source>
        <translation>Modifica la lista degli utenti banditi dal server</translation>
    </message>
    <message>
        <location/>
        <source>This lets you edit the server-side IP ban lists.</source>
        <translation>Questo ti permette di modificare la lista degli IP banditi dal lato server.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Information</source>
        <translation>&amp;Informazioni</translation>
    </message>
    <message>
        <location/>
        <source>Show information about the server connection</source>
        <translation>Visualizza le informazione della connessione al server</translation>
    </message>
    <message>
        <location/>
        <source>This will show extended information about the connection to the server.</source>
        <translation>Questo visualizza informazioni estese sulla connessione al server.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Kick</source>
        <translation>&amp;Espelli</translation>
    </message>
    <message>
        <source>Kick player (with reason)</source>
        <translation type="obsolete">Espelli giocatore (con motivazione)</translation>
    </message>
    <message>
        <source>Kick selected player off server. You&apos;ll be asked to specify a reason.</source>
        <translation type="obsolete">Espelle il giocatore dal server. Ti sarà chiesto di specificare una motivazione.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Mute</source>
        <translation>&amp;Muta</translation>
    </message>
    <message>
        <source>Mute player</source>
        <translation type="obsolete">Silenzia giocatore</translation>
    </message>
    <message>
        <source>Mute or unmute player on server. Unmuting a deafened player will also undeafen them.</source>
        <translation type="obsolete">Silenzia o riattiva il giocatore nel server. Riattivare un giocatore escluso lo farà includere di nuovo.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Ban</source>
        <translation>&amp;Bandisci</translation>
    </message>
    <message>
        <source>Kick and ban player (with reason)</source>
        <translation type="obsolete">Espelli e bandisci il giocatore (con motivazione)</translation>
    </message>
    <message>
        <source>Kick and ban selected player from server. You&apos;ll be asked to specify a reason.</source>
        <translation type="obsolete">Espelli e bandisci il giocatore  dal server. Ti sarà chiesto di specificare una motivazione.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Deafen</source>
        <translation>&amp;Escludi</translation>
    </message>
    <message>
        <source>Deafen player</source>
        <translation type="obsolete">Esclude il giocatore</translation>
    </message>
    <message>
        <source>Deafen or undeafen player on server. Deafening a player will also mute them.</source>
        <translation type="obsolete">Esclude o include il giocatore nel server. Escludendo un giocatore questo sarà anche silenziato.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Local Mute</source>
        <translation>Muta &amp;localmente</translation>
    </message>
    <message>
        <source>Mute player locally</source>
        <translation type="obsolete">Muta il giocatore localmente</translation>
    </message>
    <message>
        <source>Mute or unmute player locally. Use this on other players in the same room.</source>
        <translation type="obsolete">Silenzia o riattiva un giocatore localmente. Usa questo su altri giocatore nella stessa room.</translation>
    </message>
    <message>
        <source>Send Messa&amp;ge</source>
        <translation type="obsolete">Invia &amp;Messaggio</translation>
    </message>
    <message>
        <location/>
        <source>Send a Text Message</source>
        <translation>Invia un messaggio di testo</translation>
    </message>
    <message>
        <location/>
        <source>Sends a text message to another user.</source>
        <translation>Invia un messaggio di testo ad un&apos;altro utente.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Add</source>
        <translation>&amp;Aggiungi</translation>
    </message>
    <message>
        <location/>
        <source>Add new channel</source>
        <translation>Aggiungi un nuovo canale</translation>
    </message>
    <message>
        <location/>
        <source>This adds a new sub-channel to the currently selected channel.</source>
        <translation>Questo aggiungerà un nuovo sottocanale al canale selezionato.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Remove</source>
        <translation>&amp;Rimuovi</translation>
    </message>
    <message>
        <location/>
        <source>Remove channel</source>
        <translation>Rimuove il canale</translation>
    </message>
    <message>
        <location/>
        <source>This removes a channel and all sub-channels.</source>
        <translation>Questo rimuoverà il canale e tutti i suoi sottocanali.</translation>
    </message>
    <message>
        <source>&amp;Edit ACL</source>
        <translation type="obsolete">&amp;Modifica ACL</translation>
    </message>
    <message>
        <location/>
        <source>Edit Groups and ACL for channel</source>
        <translation>Modifica Gruppi e ACL per il canale</translation>
    </message>
    <message>
        <location/>
        <source>This opens the Group and ACL dialog for the channel, to control permissions.</source>
        <translation>Questo apre la finestra Gruppi e ACL per il canale, il controllo dei permessi.</translation>
    </message>
    <message>
        <source>&amp;Rename</source>
        <translation type="obsolete">&amp;Rinomina</translation>
    </message>
    <message>
        <source>Renames the channel</source>
        <translation type="obsolete">Rinomina il canale</translation>
    </message>
    <message>
        <source>This renames a channel.</source>
        <translation type="obsolete">Questo rinomina il canale.</translation>
    </message>
    <message>
        <source>&amp;Change Description</source>
        <translation type="obsolete">&amp;Cambia Descrizione</translation>
    </message>
    <message>
        <source>Changes the channel description</source>
        <translation type="obsolete">Cambia la descrizione del canale</translation>
    </message>
    <message>
        <source>This changes the description of a channel.</source>
        <translation type="obsolete">Questo cambia la descrizione del canale.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Link</source>
        <translation>&amp;Collega</translation>
    </message>
    <message>
        <location/>
        <source>Link your channel to another channel</source>
        <translation>Collega il canale ad un&apos;altro canale</translation>
    </message>
    <message>
        <source>This links your current channel to the selected channel. If they have permission to speak in the other channel, players can now hear each other. This is a permanent link, and will last until manually unlinked or the server is restarted. Please see the shortcuts for push-to-link.</source>
        <translation type="obsolete">Questo collega il canale corrente con il canale selezionato. Se i giocatori hanno il permesso per parlare nell&apos;altro canale, si potranno sentire con gli altri. Questo è un collegamento permanente, e non terminerà fino a che i canali non saranno scollegati manualmente o il server verrà riavviato. Per favore guarda la scorciatoia per il Premi-per-collegare.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="-1"/>
        <source>&amp;User</source>
        <translation>&amp;Utente</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>This is the chatbar&lt;br /&gt;If you enter text here and then press enter the text is sent to the user or channel that was selected. If nothing is selected the message is sent to your current channel.</source>
        <translation>Questa è la barra di chat&lt;br /&gt;Se inserisci qui del testo e dopo premi invio, il testo sarà inviato all&apos;utente o al canale selezionato. Se non c&apos;è niente selezionato il messaggio sarà inviato al canale attuale.</translation>
    </message>
    <message>
        <location/>
        <source>Chatbar</source>
        <translation>Barra della chat</translation>
    </message>
    <message>
        <location/>
        <source>Shows a dialog of registered servers, and also allows quick-connect.</source>
        <translation>Visualizza una finestra dei server registrati, e permette anche la connessione rapida.</translation>
    </message>
    <message>
        <location/>
        <source>Kick user (with reason)</source>
        <translation>Espelli utente (con motivazione)</translation>
    </message>
    <message>
        <location/>
        <source>Kick selected user off server. You&apos;ll be asked to specify a reason.</source>
        <translation>Espelle l&apos;utente dal server. Ti sarà chiesto di specificare una motivazione.</translation>
    </message>
    <message>
        <location/>
        <source>Mute user</source>
        <translation>Muta l&apos;utente</translation>
    </message>
    <message>
        <location/>
        <source>Mute or unmute user on server. Unmuting a deafened user will also undeafen them.</source>
        <translation>Muta o riattiva il microfono di un utente sul server. Riattivando il microfono di un utente riattiverai anche il suo audio.</translation>
    </message>
    <message>
        <location/>
        <source>Kick and ban user (with reason)</source>
        <translation>Espelli e bandisci l&apos;utente (con motivazione)</translation>
    </message>
    <message>
        <location/>
        <source>Kick and ban selected user from server. You&apos;ll be asked to specify a reason.</source>
        <translation>Espelli e bandisci l&apos;utente selezionato dal server. Ti sarà chiesto di specificare una ragione.</translation>
    </message>
    <message>
        <location/>
        <source>Deafen user</source>
        <translation>Esclude l&apos;utente dalle conversazioni</translation>
    </message>
    <message>
        <location/>
        <source>Deafen or undeafen user on server. Deafening a user will also mute them.</source>
        <translation>Esclude o riattiva l&apos;utente sul server. Escudendo un utente lo muterai automaticamente.</translation>
    </message>
    <message>
        <location/>
        <source>Mute user locally</source>
        <translation>Muta localmente l&apos; utente</translation>
    </message>
    <message>
        <location/>
        <source>Mute or unmute user locally. Use this on other users in the same room.</source>
        <translation>Muta o riattiva il microfono di un utente localmente. Usa questo su altri giocatore nello stesso canale.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Edit</source>
        <translation>&amp;Modifica</translation>
    </message>
    <message>
        <location/>
        <source>This links your current channel to the selected channel. If users in a channel have permission to speak in the other channel, users can now hear each other. This is a permanent link, and will last until manually unlinked or the server is restarted. Please see the shortcuts for push-to-link.</source>
        <oldsource>This links your current channel to the selected channel. If they have permission to speak in the other channel, users can now hear each other. This is a permanent link, and will last until manually unlinked or the server is restarted. Please see the shortcuts for push-to-link.</oldsource>
        <translation>Questo collega il canale corrente con il canale selezionato. Se gli utenti hanno il permesso per parlare nell&apos;altro canale, si potranno sentire con gli altri. Questo è un collegamento permanente, e non terminerà fino a che i canali non saranno scollegati manualmente o il server verrà riavviato. Per favore guarda la scorciatoia per il Premi-per-collegare.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Unlink</source>
        <comment>Channel</comment>
        <translation>&amp;Scollega</translation>
    </message>
    <message>
        <location/>
        <source>Unlink your channel from another channel</source>
        <translation>Scollega il tuo canale da un&apos;altro canale</translation>
    </message>
    <message>
        <location/>
        <source>This unlinks your current channel from the selected channel.</source>
        <translation>Questo scollegherà il tuo canale dal canale selezionato.</translation>
    </message>
    <message>
        <source>Unlink &amp;All</source>
        <translation type="obsolete">Scollega da &amp;tutti</translation>
    </message>
    <message>
        <location/>
        <source>Unlinks your channel from all linked channels.</source>
        <translation>Scollega il tuo canale da tutti gli altri canali collegati precedentemente.</translation>
    </message>
    <message>
        <location/>
        <source>This unlinks your current channel (not the selected one) from all linked channels.</source>
        <translation>Questo scollegerà il canale corrente(non quello selezionato) da tutti i canali collegati.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Reset</source>
        <translation>&amp;Reset</translation>
    </message>
    <message>
        <location/>
        <source>Reset audio preprocessor</source>
        <translation>Reset preprocesore audio</translation>
    </message>
    <message>
        <location/>
        <source>This will reset the audio preprocessor, including noise cancellation, automatic gain and voice activity detection. If something suddenly worsens the audio environment (like dropping the microphone) and it was temporary, use this to avoid having to wait for the preprocessor to readjust.</source>
        <translation>Questo resetta il preprocessore audio, inclusa la cancellazione del rumore, il guadagno automatico e la rilevazione dell&apos;attività della voce. Se qualcosa peggiorasse improvvisamente l&apos;audio dell&apos;ambiente (come far cadere il microfono) e questo è temporaneo, usa questo per evitare di aspettare che il preprocessore audio si riaggiusti.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Mute Self</source>
        <translation>&amp;Mutati</translation>
    </message>
    <message>
        <location/>
        <source>Mute yourself</source>
        <translation>Disattiva il tuo microfono</translation>
    </message>
    <message>
        <location/>
        <source>Mute or unmute yourself. When muted, you will not send any data to the server. Unmuting while deafened will also undeafen.</source>
        <translation>Muta o riattiva il tuo microfono. Quando il tuo microfono è disattivato tu non invii nessun dato al server. Riattivarsi il microfono quando si è escusi implica anche raittivarsi.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Deafen Self</source>
        <translation>&amp;Escluditi</translation>
    </message>
    <message>
        <location/>
        <source>Deafen yourself</source>
        <translation>Escude te stesso dalle comunicazioni</translation>
    </message>
    <message>
        <location/>
        <source>Deafen or undeafen yourself. When deafened, you will not hear anything. Deafening yourself will also mute.</source>
        <translation>Escude o riattiva te stesso. Quando sei escuso non sentirai niente e non potrai comunicare vocalmente con gli altri perchè escudersi implica mutarsi.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Text-To-Speech</source>
        <translation>&amp;Sintesi vocale</translation>
    </message>
    <message>
        <location/>
        <source>Toggle Text-To-Speech</source>
        <translation>Attiva/disattiva Sintesi vocale</translation>
    </message>
    <message>
        <location/>
        <source>Enable or disable the text-to-speech engine. Only messages enabled for TTS in the Configuration dialog will actually be spoken.</source>
        <translation>Abilita o disabilita il motore di sintesi vocale. Solamente i messaggi abilitati per la sintesi vocale nella finestra di configurazione saranno trasmessi.</translation>
    </message>
    <message>
        <source>S&amp;tatistics</source>
        <translation type="obsolete">S&amp;tatistiche</translation>
    </message>
    <message>
        <location/>
        <source>Display audio statistics</source>
        <translation>Visualizza le statistiche audio</translation>
    </message>
    <message>
        <location/>
        <source>Pops up a small dialog with information about your current audio input.</source>
        <translation>Visualizza una piccola finestra di dialogo con le informazioni sull&apos;input audio corrente.</translation>
    </message>
    <message>
        <source>&amp;Unlink</source>
        <comment>Plugin</comment>
        <translation type="obsolete">S&amp;collega</translation>
    </message>
    <message>
        <location/>
        <source>Forcibly unlink plugin</source>
        <translation>Forza scollegamento plugin</translation>
    </message>
    <message>
        <location/>
        <source>This forces the current plugin to unlink, which is handy if it is reading completely wrong data.</source>
        <translation>Questo forza il plugin corrente a scollegarsi, che è utile se questo stà leggendo delle informazioni completamente sbagliate.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Settings</source>
        <translation>&amp;Configurazione</translation>
    </message>
    <message>
        <location/>
        <source>Configure Mumble</source>
        <translation>Configura Mumble</translation>
    </message>
    <message>
        <location/>
        <source>Allows you to change most settings for Mumble.</source>
        <translation>Permette di cambiare molte delle impostazioni di Mumble.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Audio Wizard</source>
        <translation>&amp;Audio Wizard</translation>
    </message>
    <message>
        <location/>
        <source>Start the audio configuration wizard</source>
        <translation>Inizia la procedura guidata per la configurazione dell&apos;audio</translation>
    </message>
    <message>
        <location/>
        <source>This will guide you through the process of configuring your audio hardware.</source>
        <translation>Questo ti guiderà attraverso il processo di configurazione dell&apos;hardware audio.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;What&apos;s This?</source>
        <translation>&amp;Cos&apos;è questo?</translation>
    </message>
    <message>
        <location/>
        <source>Enter What&apos;s This? mode</source>
        <translation>Entra nella modalità Cos&apos;è questo</translation>
    </message>
    <message>
        <location/>
        <source>Click this to enter &quot;What&apos;s This?&quot; mode. Your cursor will turn into a question mark. Click on any button, menu choice or area to show a description of what it is.</source>
        <translation>Premi qui per entrare nella modalità &quot;Cos&apos;è questo&quot;. Il cursore diventerà un punto interrogativo. Premendo su qualsiasi bottone, scelta del menu o area, verrà visualizzata una descrizione di cosa è quello.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;About</source>
        <translation>&amp;Info</translation>
    </message>
    <message>
        <location/>
        <source>Information about Mumble</source>
        <translation>Informazioni su Mumble</translation>
    </message>
    <message>
        <location/>
        <source>Shows a small dialog with information and license for Mumble.</source>
        <translation>Visualizza una piccola finestra con le informazioni e la licenza di Mumble.</translation>
    </message>
    <message>
        <location/>
        <source>About &amp;Speex</source>
        <translation>Info su &amp;Speex</translation>
    </message>
    <message>
        <location/>
        <source>Information about Speex</source>
        <translation>Informazioni su Speex</translation>
    </message>
    <message>
        <location/>
        <source>Shows a small dialog with information about Speex.</source>
        <translation>Visualizza una piccola finestra con le informazioni su Speex.</translation>
    </message>
    <message>
        <location/>
        <source>About &amp;Qt</source>
        <translation>Info su &amp;Qt</translation>
    </message>
    <message>
        <location/>
        <source>Information about Qt</source>
        <translation>Informazioni su Qt</translation>
    </message>
    <message>
        <location/>
        <source>Shows a small dialog with information about Qt.</source>
        <translation>Visualizza un dialogo con le informazioni su Qt.</translation>
    </message>
    <message>
        <location/>
        <source>Check for &amp;Updates</source>
        <translation>Cerca &amp;Aggiornamenti</translation>
    </message>
    <message>
        <location/>
        <source>Check for new version of Mumble</source>
        <translation>Cerca una nuova versione di Mumble</translation>
    </message>
    <message>
        <location/>
        <source>Connects to the Mumble webpage to check if a new version is available, and notifies you with an appropriate download URL if this is the case.</source>
        <translation>Connette alla pagina internet di Mumble per cercare se una nuova versione del programma è disponibili e avverte con un URL se è il caso di scaricare una versione aggiornata.</translation>
    </message>
    <message>
        <location/>
        <source>Sends a text message to all users in a channel.</source>
        <translation>Invia un messaggio di testo a tutti gli utenti in questo canale.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Change Comment</source>
        <translation>&amp;Cambia commento</translation>
    </message>
    <message>
        <source>Change the comment on the selected user.</source>
        <translation type="obsolete">Cambia il commento sull&apos;utente selezionato.</translation>
    </message>
    <message>
        <source>This allows you to change the comment (shown as a tooltip) of a user. Unless you&apos;ve been given extra privileges on the server, you can only change your own comment.</source>
        <translation type="obsolete">Questo ti permette di cambiare il commento (visualizzato come consiglio) di un utente. Fino a quando non ti saranno assegnati privilegi superiori nel server, potrai cambiare solo il tuo commento.</translation>
    </message>
    <message>
        <source>Certificate Wizard</source>
        <translation type="obsolete">Wizard del certificato</translation>
    </message>
    <message>
        <location/>
        <source>Configure certificates for strong authentication</source>
        <translation>Configura certificati per un&apos;autenticazione sicura</translation>
    </message>
    <message>
        <location/>
        <source>This starts the wizard for creating, importing and exporting certificates for authentication against servers.</source>
        <translation>Questo avvia la procedura guidata per creare, importare o esportare i certificati per autenticarsi con i server.</translation>
    </message>
    <message>
        <source>Register</source>
        <translation type="obsolete">Registra</translation>
    </message>
    <message>
        <location/>
        <source>Register user on server</source>
        <translation>Registra utente sul server</translation>
    </message>
    <message>
        <location/>
        <source>This will permanently register the user on the server.</source>
        <translation>Questo registrerà permanentemente l&apos;utente nel server.</translation>
    </message>
    <message>
        <source>This will permanently register the user on the server. Note that you can&apos;t change the username later on, nor can you easily unregister a user, so make sure you really want to do this.</source>
        <translation type="obsolete">Questo registrerà permanentemente l&apos;utente nel server. Nota che non puoi cambiare il tuo nome utente successivamente, nemmeno rimuovere facilmente la tua registrazione, quindi accertati di volerlo fare veramente.</translation>
    </message>
    <message>
        <location/>
        <source>Add &amp;Friend</source>
        <translation>Aggiungi &amp;amico</translation>
    </message>
    <message>
        <location/>
        <source>Adds a user as your friend.</source>
        <translation>Aggiunge utente come tuo amico.</translation>
    </message>
    <message>
        <location/>
        <source>This will add the user as a friend, so you can recognize him on this and other servers.</source>
        <translation>Questo aggiunge l&apos;utente alla lista degli amici, così potrai trovarlo in questo ed altri server.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Remove Friend</source>
        <translation>&amp;Rimuovi amico</translation>
    </message>
    <message>
        <location/>
        <source>Removes a user from your friends.</source>
        <translation>Rimuove un utente dai tuoi amici.</translation>
    </message>
    <message>
        <location/>
        <source>This will remove a user from your friends list.</source>
        <translation>Questo rimuove l&apos;utente dalla lista degli amici.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Update Friend</source>
        <translation>&amp;Aggiorna amico</translation>
    </message>
    <message>
        <location/>
        <source>Update name of your friend.</source>
        <translation>Aggiorna il nome del tuo amico.</translation>
    </message>
    <message>
        <location/>
        <source>Your friend uses a different name than what is in your database. This will update the name.</source>
        <translation>Il tuo amico usa un nome differente da quello che è nel tuo database. Questo aggiornerà il nome.</translation>
    </message>
    <message>
        <location/>
        <source>Registered &amp;Users</source>
        <translation>&amp;Utenti registrati</translation>
    </message>
    <message>
        <location/>
        <source>Edit registered users list</source>
        <translation>Modifica la lista degli utenti registrati</translation>
    </message>
    <message>
        <location/>
        <source>This opens the editor for registered users, which allow you to change their name or unregister them.</source>
        <translation>Questo apre l&apos;editor degli utenti registrati, che ti permette di cambiare il loro nome o di eliminare la loro registrazione.</translation>
    </message>
    <message>
        <source>Change Texture</source>
        <translation type="obsolete">Cambia Immagine</translation>
    </message>
    <message>
        <source>Change your overlay texture on this server</source>
        <translation type="obsolete">Cambia l&apos;immagine della sovrapposizione su questo server</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Access Tokens</source>
        <translation>&amp;Token di accesso</translation>
    </message>
    <message>
        <location/>
        <source>Add or remove text-based access tokens</source>
        <translation>Aggiunge o rimuove Token di accesso basati sul testo</translation>
    </message>
    <message>
        <source>Remove Texture</source>
        <translation type="obsolete">Rimuovi immagine</translation>
    </message>
    <message>
        <source>Remove currently defined user texture.</source>
        <translation type="obsolete">Rimuove l&apos;immagine attuale dell&apos;utente.</translation>
    </message>
    <message>
        <source>Send &amp;Tree Message</source>
        <translation type="obsolete">Invia un messaggi ad &amp;Albero</translation>
    </message>
    <message>
        <source>Send message to channel tree.</source>
        <translation type="obsolete">Invia un messaggio all&apos;albero del canale.</translation>
    </message>
    <message>
        <source>This sends a text message to a channel and it&apos;s subchannels.</source>
        <translation type="obsolete">Questo invia un messaggio al canale e a tutti i suoi sotto-canali.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Minimal View</source>
        <translation>Visuale &amp;Minima</translation>
    </message>
    <message>
        <location/>
        <source>Toggle minimal window modes</source>
        <translation>Cambia la modalità visualizzazione minima</translation>
    </message>
    <message>
        <location/>
        <source>This will toggle minimal mode, where the log window and menu is hidden.</source>
        <translation>Questo seleziona la modalità minima, dove la finestra del log e il menu sono noscosti.</translation>
    </message>
    <message>
        <source>Joined server: %1.</source>
        <translation type="obsolete">%1 è entrato nel server.</translation>
    </message>
    <message>
        <source>the server</source>
        <translation type="obsolete">il server</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="+323"/>
        <source>You were unmuted and undeafened by %1.</source>
        <translation>Microfono e audio riattivati da %1.</translation>
    </message>
    <message>
        <location line="+29"/>
        <source>You muted and deafened %1.</source>
        <translation>Hai escluso %1.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>You unmuted and undeafened %1.</source>
        <translation>Hai riattivato %1.</translation>
    </message>
    <message>
        <location line="+11"/>
        <source>You undeafened %1.</source>
        <translation>Hai riattivato %1.</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>You suppressed %1.</source>
        <translation>Hai mutato %1.</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>%1 muted and deafened by %2.</source>
        <translation>%2 ha escluso %1.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1 unmuted and undeafened by %2.</source>
        <translation>%2 ha riattivato %1.</translation>
    </message>
    <message>
        <location line="+18"/>
        <source>%1 suppressed by %2.</source>
        <translation>%1 mutato da %2.</translation>
    </message>
    <message>
        <location line="+25"/>
        <source>%1 moved to %2.</source>
        <translation>%1 spostato su %2.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1 moved to %2 by %3.</source>
        <translation>%3 ha spostato %1 su %2.</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>%1 moved in from %2 by %3.</source>
        <translation>%3 ha spostato %1 qui dal canale %2.</translation>
    </message>
    <message>
        <source>Left server: %1.</source>
        <translation type="obsolete">%1 ha lasciato il server.</translation>
    </message>
    <message>
        <location line="-157"/>
        <source>%1 is now muted and deafened.</source>
        <translation>%1 si è escluso.</translation>
    </message>
    <message>
        <location line="-114"/>
        <source>You were denied %1 privileges in %2.</source>
        <translation>Ti è stato negato il privilegio %1 su %2.</translation>
    </message>
    <message>
        <location line="-69"/>
        <location line="+48"/>
        <source>Welcome message: %1</source>
        <translation>Messaggio di benvenuto: %1</translation>
    </message>
    <message>
        <location line="-15"/>
        <source>Mumble: %1</source>
        <translation>Mumble: %1</translation>
    </message>
    <message>
        <location line="+38"/>
        <source>%3 was denied %1 privileges in %2.</source>
        <translation>A %3 è stato negato il privilegio %1 su %2.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Denied: Cannot modify SuperUser.</source>
        <translation>Negato: Impossibile modificare SuperUser.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Denied: Invalid channel name.</source>
        <translation>Negato: Nome del canale non valido.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Denied: Text message too long.</source>
        <translation>Negato: Messaggio di testo troppo lungo.</translation>
    </message>
    <message>
        <location line="+25"/>
        <source>Denied: Operation not permitted in temporary channel.</source>
        <translation>Negato: Operazione non permessa in un canale temporaneo.</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>You need a certificate to perform this operation.</source>
        <translation>Hai bisogno di un certificato per eseguire questa operazione.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1 does not have a certificate.</source>
        <translation>%1 non ha un certificato.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Invalid username: %1.</source>
        <translation>Nome utente non valido: %1.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Invalid username.</source>
        <translation>Nome utente non valido.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Channel is full.</source>
        <translation>Il canale è pieno.</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Permission denied.</source>
        <translation>Permesso negato.</translation>
    </message>
    <message>
        <location line="+37"/>
        <source>%1 connected.</source>
        <translation>Si è connesso %1.</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>%1 is now muted.</source>
        <translation>%1 si è mutato.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1 is now unmuted.</source>
        <translation>%1 ha riattivato il suo microfono.</translation>
    </message>
    <message>
        <location line="+11"/>
        <source>Recording started</source>
        <translation>Registrazione iniziata</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Recording stopped</source>
        <translation>Registrazione interrotta</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>%1 started recording.</source>
        <translation>%1 ha iniziato a registrare.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1 stopped recording.</source>
        <translation>%1 ha smesso di registrare.</translation>
    </message>
    <message>
        <location line="+41"/>
        <source>You were unsuppressed.</source>
        <translation>Sei stato riattivato.</translation>
    </message>
    <message>
        <location line="+96"/>
        <source>%1 is recording</source>
        <translation>%1 stà registrando</translation>
    </message>
    <message>
        <location line="+46"/>
        <source>%1 disconnected.</source>
        <translation>%1 si è disconnesso.</translation>
    </message>
    <message>
        <location line="+91"/>
        <source>(Tree) </source>
        <translation>(Canale e sottocanali) </translation>
    </message>
    <message>
        <location line="+2"/>
        <source>(Channel) </source>
        <translation>(Canale) </translation>
    </message>
    <message>
        <location line="+3"/>
        <source>%2%1: %3</source>
        <translation>%2%1: %3</translation>
    </message>
    <message>
        <source>server</source>
        <translation type="obsolete">server</translation>
    </message>
    <message>
        <location line="-254"/>
        <source>You were muted by %1.</source>
        <translation>Sei stato mutato da %1.</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>You were suppressed.</source>
        <oldsource>You were suppressed by %1.</oldsource>
        <translation>Sei stato mutato dal server.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>You were unsuppressed by %1.</source>
        <translation>Sei stato riattivato da %1.</translation>
    </message>
    <message>
        <location line="-16"/>
        <source>You were unmuted by %1.</source>
        <translation>%1 ti ha riattivato il microfono.</translation>
    </message>
    <message>
        <source>You deafened %1.</source>
        <translation type="obsolete">Ha disattivato l&apos;audio di %1.</translation>
    </message>
    <message>
        <location line="+29"/>
        <source>You muted %1.</source>
        <translation>Hai mutato %1.</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>You unsuppressed %1.</source>
        <translation>Hai riattivato %1.</translation>
    </message>
    <message>
        <location line="-14"/>
        <source>You unmuted %1.</source>
        <translation>Hai riattivato il microfono di %1.</translation>
    </message>
    <message>
        <location line="+25"/>
        <source>%1 muted by %2.</source>
        <translation>%2 ha mutato %1.</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>%1 unsuppressed by %2.</source>
        <translation>%2 ha riattivato %1.</translation>
    </message>
    <message>
        <location line="-14"/>
        <source>%1 unmuted by %2.</source>
        <translation>%2 ha riattivato il microfono di %1.</translation>
    </message>
    <message>
        <source>the server</source>
        <comment>message from</comment>
        <translation type="obsolete">messaggio dal server</translation>
    </message>
    <message>
        <location line="+338"/>
        <source>Unable to find matching CELT codecs with other clients. You will not be able to talk to all users.</source>
        <translation>Impossibile trovare codec CELT uguali a quelli degli altri utenti. Non sarai in grado di parlare con gli altri utenti.</translation>
    </message>
    <message>
        <location line="-404"/>
        <source>You were muted and deafened by %1.</source>
        <oldsource>You were deafened by %1.</oldsource>
        <translation>%1 ti ha mutato.</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>You were undeafened by %1.</source>
        <translation>%1 ti ha riattivato.</translation>
    </message>
    <message>
        <source>%1 deafened by %2.</source>
        <translation type="obsolete">L&apos;audio e il microfono di %1 sono stati disattivati da %2.</translation>
    </message>
    <message>
        <location line="+58"/>
        <source>%1 undeafened by %2.</source>
        <translation>%2 ha riattivato%1.</translation>
    </message>
    <message>
        <location line="+86"/>
        <source>You were kicked from the server by %1: %2.</source>
        <translation>Sei stato espulso dal server da %1 per la seguente motivazione: %2.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>%3 was kicked from the server by %1: %2.</source>
        <translation>%3 è stato espulso dal server da %1 per la seguente motivazione: %2.</translation>
    </message>
    <message>
        <location line="-7"/>
        <source>You were kicked and banned from the server by %1: %2.</source>
        <translation>Sei stato espulso e bandito dal server da %1 per la seguente motivazione: %2.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>%3 was kicked and banned from the server by %1: %2.</source>
        <translation>%3 è stato espulso e bandito dal server da %1 per la seguente motivazione: %2.</translation>
    </message>
    <message>
        <location line="-60"/>
        <source>You were moved to %1 by %2.</source>
        <translation>%2 ti ha spostato sul canale %1.</translation>
    </message>
    <message>
        <source>%1 left channel.</source>
        <translation type="obsolete">%1 ha lasciato il canale.</translation>
    </message>
    <message>
        <source>%1 moved out by %2 to %3.</source>
        <translation type="obsolete">%1 spostato fuori da %2 a %3.</translation>
    </message>
    <message>
        <source>%1 moved in by %2 from %3.</source>
        <translation type="obsolete">%1 spostato dentro da %2 dal canale %3.</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>%1 entered channel.</source>
        <translation>%1 è entrato nel canale.</translation>
    </message>
    <message>
        <location line="-346"/>
        <source>Server connection rejected: %1.</source>
        <translation>Connessione al server respinta: %1.</translation>
    </message>
    <message>
        <location line="+141"/>
        <source>Denied: %1.</source>
        <translation>Negato: %1.</translation>
    </message>
    <message>
        <source>Server maximum bandwidth is only %1 kbit/s. Quality auto-adjusted.</source>
        <translation type="obsolete">La banda massima del server è solamente %1 kbit/s. Qualità auto-aggiustata.</translation>
    </message>
    <message>
        <source>From %1: %2</source>
        <translation type="obsolete">Da %1: %2</translation>
    </message>
    <message>
        <location line="+353"/>
        <source>Message from %1</source>
        <translation>Messaggio da %1</translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="+1381"/>
        <source>You have Channel Dragging set to &quot;Do Nothing&quot; so the channel wasn&apos;t moved.</source>
        <translation>Hai selezionato &quot;non fare niente&quot; sulle impostazioni del trascinamento del canale, quindi il canale non sarà spostato.</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Unknown Channel Drag mode in UserModel::dropMimeData.</source>
        <translation>Modalità trascinamento canale sconosciuta in UserModel::dropMimeData.</translation>
    </message>
    <message>
        <source>Unknown Channel Drag mode in PlayerModel::dropMimeData.</source>
        <translation type="obsolete">Modalità trascianamento canale sconosciuta in PlayerModel::dropMimeData.</translation>
    </message>
    <message>
        <location filename="main.cpp" line="-52"/>
        <source>Welcome to Mumble.</source>
        <translation>Benvenuto su Mumble.</translation>
    </message>
    <message>
        <source>This is the first time you&apos;re starting Mumble.&lt;br /&gt;Would you like to go through the Audio Wizard to configure your soundcard?</source>
        <translation type="obsolete">Questa è la prima volta che avvii Mumble.&lt;br /&gt;Vorresti essere aiutato dall&apos;Audio Wizard per configurare la tua scheda audio e le impostazioni audio?</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="-10"/>
        <location line="+1"/>
        <source>Server</source>
        <comment>message from</comment>
        <translation>Server</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>Hide Frame</source>
        <translation>Nascondi cornice</translation>
    </message>
    <message>
        <location/>
        <source>Toggle showing frame on minimal window</source>
        <translation>Cambia visualizzazione cornice nella finestra minina</translation>
    </message>
    <message>
        <location/>
        <source>This will toggle whether the minimal window should have a frame for moving and resizing.</source>
        <translation>Questo imposterà se la finestra minima deve avere la cornice per muoversi e ridimensionarsi.</translation>
    </message>
    <message>
        <location/>
        <source>Send &amp;Message</source>
        <translation>Invia &amp;Messaggio</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Unlink All</source>
        <translation>&amp;Scollega tutti</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Certificate Wizard</source>
        <translation>&amp;Wizard del Certificato</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Register</source>
        <translation>&amp;Registra</translation>
    </message>
    <message>
        <source>Change &amp;Texture</source>
        <translation type="obsolete">Cambia &amp;Immagine</translation>
    </message>
    <message>
        <source>&amp;Remove Texture</source>
        <translation type="obsolete">&amp;Rimuovi Immagine</translation>
    </message>
    <message>
        <location/>
        <source>Reset &amp;Comment</source>
        <translation>Cancella &amp;Commento</translation>
    </message>
    <message>
        <location/>
        <source>Reset the comment of the selected user.</source>
        <translation>Cancella il commento dell&apos;utente selezionato.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Join Channel</source>
        <translation>&amp;Entra nel Canale</translation>
    </message>
    <message>
        <location/>
        <source>View Comment</source>
        <translation>Visualizza Commento</translation>
    </message>
    <message>
        <location/>
        <source>View comment in editor</source>
        <translation>Visualizza commento nell&apos;editor</translation>
    </message>
    <message>
        <location/>
        <source>Query server for connection information for user</source>
        <translation>Interrogazione del server per le informazioni di connessione dell&apos;utente</translation>
    </message>
    <message>
        <location/>
        <source>S&amp;erver</source>
        <translation>&amp;Server</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Self</source>
        <translatorcomment>non saprei cos&apos;altro metterci</translatorcomment>
        <translation>&amp;Utente</translation>
    </message>
    <message>
        <location/>
        <source>Audio S&amp;tatistics</source>
        <translation>&amp;Statistiche audio</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Unlink Plugins</source>
        <translation>&amp;Scollega plugin</translation>
    </message>
    <message>
        <source>Change your own comment.</source>
        <translation type="obsolete">Cambia il tuo stesso commento.</translation>
    </message>
    <message>
        <location/>
        <source>R&amp;egister</source>
        <translation>R&amp;egistrati</translation>
    </message>
    <message>
        <location/>
        <source>Register yourself on the server</source>
        <translation>Registrati sul server</translation>
    </message>
    <message>
        <location/>
        <source>Change &amp;Avatar</source>
        <translation>Cambia &amp;avatar</translation>
    </message>
    <message>
        <location/>
        <source>Change your avatar image on this server</source>
        <translation>Cambia l&apos;immagine dell&apos;avatar in questo server</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Remove Avatar</source>
        <translation>&amp;Rimuovi avatar</translation>
    </message>
    <message>
        <location/>
        <source>Remove currently defined avatar image.</source>
        <translation>Rimuove l&apos;immagine attualmente definita come avatar.</translation>
    </message>
    <message>
        <location/>
        <source>Icon Toolbar</source>
        <translation>Toolbar delle icone</translation>
    </message>
    <message>
        <location/>
        <source>Change your own comment</source>
        <translation>Cambia il tuo commento</translation>
    </message>
    <message>
        <location/>
        <source>Recording</source>
        <translation>Registra</translation>
    </message>
    <message>
        <location/>
        <source>Priority Speaker</source>
        <translation>Priorità sugli altri</translation>
    </message>
</context>
<context>
    <name>NetworkConfig</name>
    <message>
        <location filename="NetworkConfig.cpp" line="+53"/>
        <source>Network</source>
        <translation>Rete</translation>
    </message>
    <message>
        <location line="+36"/>
        <source>Updates are mandatory when using snapshot releases.</source>
        <translation>Gli aggiornamenti sono obbligatori quando stai usando una versione &quot;Snapshot&quot;.</translation>
    </message>
    <message>
        <source>Form</source>
        <translation type="obsolete">Modulo</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui"/>
        <source>Connection</source>
        <translation>Connessione</translation>
    </message>
    <message>
        <location/>
        <source>Use TCP compatibility mode</source>
        <oldsource>Use TCP compatability mode</oldsource>
        <translation>Usa la modalità per la compatibilità TCP</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Enable TCP compatibility mode&lt;/b&gt;.&lt;br /&gt;This will make Mumble use only TCP when communicating with the server. This will increase overhead and cause lost packets to produce noticeable pauses in communication, so this should only be used if you are unable to use the default (which uses UDP for voice and TCP for control).</source>
        <translation>&lt;b&gt;Abilita la modalità compatibile TCP&lt;/b&gt;.&lt;br /&gt;Farà in modo che Mumble usi solo il protocollo TCP quando comunica con il server. Ciò aumenterà il sovraccarico di rete e causerà la perdita di pacchetti che creeranno vuoti nelle comunicazioni, quindi va usato solo se con il sistema standard (che usa UDP per la voce e il TCP per il controllo) non funziona.</translation>
    </message>
    <message>
        <source>Use TCP mode</source>
        <translation type="obsolete">Usa modo TCP</translation>
    </message>
    <message>
        <location/>
        <source>Reconnect when disconnected</source>
        <translation>Ricollegati quando disconnesso</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Reconnect when disconnected&lt;/b&gt;.&lt;br /&gt;This will make Mumble try to automatically reconnect after 10 seconds if your server connection fails.</source>
        <translation>&lt;b&gt;Riconnetti quando disconnesso&lt;/b&gt;.&lt;br /&gt;Mumble proverà a ricollegarsi automaticamente dopo 10 secondi se cade la connessione al server.</translation>
    </message>
    <message>
        <location/>
        <source>Reconnect automatically</source>
        <translation>Ricollegati automaticamente</translation>
    </message>
    <message>
        <location/>
        <source>Proxy</source>
        <translation>Proxy</translation>
    </message>
    <message>
        <location/>
        <source>Type</source>
        <translation>Tipo</translation>
    </message>
    <message>
        <location/>
        <source>Type of proxy to connect through</source>
        <translation>Tipo di proxy da cui passa la connessione</translation>
    </message>
    <message>
        <source>&lt;b&gt;Type of proxy to connect through&lt;/b&gt;&lt;br /&gt;This makes Mumble connect through a proxy for all outgoing connections. Note: Proxy tunneling forces Mumble into TCP compatibility mode, causing all voice data to be sent via the control channel.</source>
        <oldsource>&lt;b&gt;Type of proxy to connect through&lt;/b&gt;&lt;br /&gt;This makes Mumble connect through a proxy for all outgoing connections. Note: Proxy tunneling forces Mumble into TCP compatability mode, causing all voice data to be sent via the control channel.</oldsource>
        <translation type="obsolete">&lt;b&gt;Tipo di proxy tramite cui connettersi&lt;/b&gt;&lt;br /&gt;Ciò fà connettere Mumble attraverso un proxy per tutte le connessioni in uscita. Nota: il proxy tunneling forza Mumble nella modalita di compatibilità TCP, causando l&apos;invio di tutti i dati vocali tramite il canale di controllo.</translation>
    </message>
    <message>
        <location/>
        <source>Direct connection</source>
        <translation>Connessione diretta</translation>
    </message>
    <message>
        <location/>
        <source>HTTP(S) proxy</source>
        <translation>HTTP(S) proxy</translation>
    </message>
    <message>
        <location/>
        <source>SOCKS5 proxy</source>
        <translation>SOCKS5 proxy</translation>
    </message>
    <message>
        <location/>
        <source>Hostname</source>
        <translation>Hostname</translation>
    </message>
    <message>
        <location/>
        <source>Hostname of the proxy</source>
        <translation>Nome del proxy</translation>
    </message>
    <message>
        <source>&lt;b&gt;Hostname of the proxy.&lt;b&gt;&lt;br /&gt;This field specifies the hostname of the proxy you wish to tunnel network traffic through.</source>
        <translation type="obsolete">&lt;b&gt;Hostname del proxy.&lt;b&gt;&lt;br /&gt;Specifica il nome del proxy attraverso cui vuoi che passi il traffico di rete.</translation>
    </message>
    <message>
        <location/>
        <source>Force TCP mode</source>
        <translation>Forza modalità TCP</translation>
    </message>
    <message>
        <location/>
        <source>Enable QoS to prioritize packets</source>
        <translation>Abilita QoS a priorizzare i pacchetti</translation>
    </message>
    <message>
        <location/>
        <source>This will enable QoS, which will attempt to prioritize voice packets over other traffic.</source>
        <translation>Questo abilita QoS, che tenterà di priorizzare i pacchetti vocali sull&apos;altro traffico.</translation>
    </message>
    <message>
        <location/>
        <source>Use Quality of Service</source>
        <translation>Utilizza &quot;Quality of Service&quot;</translation>
    </message>
    <message>
        <location/>
        <source>Don&apos;t send certificate to server and don&apos;t save passwords. (Not saved).</source>
        <translation>Non invia certificati al server e non salva le password (Non salvato).</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This will suppress identity information from the client.&lt;/b&gt;&lt;p&gt;The client will not identify itself with a certificate, even if defined, and will not cache passwords for connections. This is primarily a test-option and is not saved.&lt;/p&gt;</source>
        <translation>&lt;b&gt;Questo evita l&apos;invio di informazioni di identità da parte del client. &lt;/b&gt;&lt;p&gt; Il client non dovrà identificarsi con un certificato, anche se definito, e non saranno salvate le password per le connessioni. Ciò è principalmente un&apos;opzione di prova e non viene salvata.&lt;/P&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Suppress certificate and password storage</source>
        <translation>Impedisci conservazione certificati e password</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Type of proxy to connect through.&lt;/b&gt;&lt;br /&gt;This makes Mumble connect through a proxy for all outgoing connections. Note: Proxy tunneling forces Mumble into TCP compatibility mode, causing all voice data to be sent via the control channel.</source>
        <translation>&lt;b&gt;Tipo di proxy tramite cui connettersi.&lt;/b&gt;&lt;br /&gt;Ciò fà connettere Mumble attraverso un proxy per tutte le connessioni in uscita. Nota: il proxy tunneling forza Mumble nella modalita di compatibilità TCP, causando l&apos;invio di tutti i dati vocali tramite il canale di controllo.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Hostname of the proxy.&lt;/b&gt;&lt;br /&gt;This field specifies the hostname of the proxy you wish to tunnel network traffic through.</source>
        <translation>&lt;b&gt;Nome dell&apos;host del proxy.&lt;/b&gt;&lt;br /&gt;Specifica il nome dell&apos;host del proxy attraverso cui vuoi che passi il traffico di rete.</translation>
    </message>
    <message>
        <location/>
        <source>Port</source>
        <translation>Porta</translation>
    </message>
    <message>
        <location/>
        <source>Port number of the proxy</source>
        <translation>Numero di porta del proxy</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Port number of the proxy.&lt;/b&gt;&lt;br /&gt;This field specifies the port number that the proxy expects connections on.</source>
        <translation>&lt;b&gt;Numero di porta del proxy.&lt;/b&gt;&lt;br /&gt;Specifica il numero della porta su cui il proxy sta in ascolto per le connessioni.</translation>
    </message>
    <message>
        <location/>
        <source>Username</source>
        <translation>Nome utente</translation>
    </message>
    <message>
        <location/>
        <source>Username for proxy authentication</source>
        <translation>Nome utente per l&apos;autenticazione del proxy</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Username for proxy authentication.&lt;/b&gt;&lt;br /&gt;This specifies the username you use for authenticating yourself with the proxy. In case the proxy does not use authentication, or you want to connect anonymously, simply leave this field blank.</source>
        <translation>&lt;b&gt;Nome utente per l&apos;autenticazione proxy.&lt;/b&gt;&lt;br /&gt;Specifica il nome utente utilizzato per l&apos;autenticazione con il proxy. Nel caso in cui il proxy non utilizza l&apos;autenticazione, o se si desidera effettuare la connessione anonima, è sufficiente lasciare vuoto questo campo.</translation>
    </message>
    <message>
        <location/>
        <source>Password</source>
        <translation>Password</translation>
    </message>
    <message>
        <location/>
        <source>Password for proxy authentication</source>
        <translation>Password per l&apos;autenticazione del proxy</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Password for proxy authentication.&lt;/b&gt;&lt;br /&gt;This specifies the password you use for authenticating yourself with the proxy. In case the proxy does not use authentication, or you want to connect anonymously, simply leave this field blank.</source>
        <translation>&lt;b&gt;Password per l&apos;autenticazione proxy.&lt;/b&gt;&lt;br /&gt;Specifica la password utilizzata per l&apos;autenticazione con il proxy. Nel caso in cui il proxy non utilizza l&apos;autenticazione, o se si desidera effettuare la connessione anonima, è sufficiente lasciare vuoto questo campo.</translation>
    </message>
    <message>
        <location/>
        <source>Misc</source>
        <translation>Varie</translation>
    </message>
    <message>
        <location/>
        <source>Prevent log from downloading images</source>
        <translation>Impedisce il download delle immagini sul log</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Disable image download&lt;/b&gt;&lt;br/&gt;
Prevents the client from downloading images embedded into chat messages with the img tag.</source>
        <translation>&lt;b&gt;Disabilita download delle immagini&lt;/b&gt;&lt;br/&gt;
Impedisce il download delle immagini incorporate nei messaggi di chat con il tag img.</translation>
    </message>
    <message>
        <location/>
        <source>Disable image download</source>
        <translation>Disabilita download immagini</translation>
    </message>
    <message>
        <location/>
        <source>Mumble services</source>
        <translation>Servizi di Mumble</translation>
    </message>
    <message>
        <location/>
        <source>Check for new releases of Mumble automatically.</source>
        <translation>Cerca nuove versioni di Mumble automaticamente.</translation>
    </message>
    <message>
        <location/>
        <source>This will check for new releases of Mumble every time you start the program, and notify you if one is available.</source>
        <translation>Cercherà nuove versioni di Mumble ogni volta che aprirai il programma, e ti avviserà se una di queste è disponibile.</translation>
    </message>
    <message>
        <location/>
        <source>Check for application updates on startup</source>
        <translation>Cerca aggiornamenti all&apos;avvio</translation>
    </message>
    <message>
        <location/>
        <source>Check for new releases of plugins automatically.</source>
        <translation>Cerca nuove versioni dei plugin automaticamente.</translation>
    </message>
    <message>
        <location/>
        <source>This will check for new releases of plugins every time you start the program, and download them automatically.</source>
        <translation>Cercherà nuove versioni dei plugin ogni volta che aprirai il programma, e li scaricherà automaticamente.</translation>
    </message>
    <message>
        <source>Download plugin updates on startup</source>
        <translation type="obsolete">Cerca aggiornamenti plugin all&apos;avvio</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Submit anonymous statistics.&lt;/b&gt;&lt;br /&gt;Mumble has a small development team, and as such needs to focus its development where it is needed most. By submitting a bit of statistics you help the project determine where to focus development.</source>
        <translation>&lt;b&gt;Invia statistiche anonime.&lt;/b&gt;&lt;br /&gt;Mumble ha un piccolo team di sviluppo, e come tale ha bisogno di concentrare lo sviluppo dove è maggiormente necessario. Inviando un po&apos; di statistiche permetti al progetto di stabilire dove concentrare lo sviluppo.</translation>
    </message>
    <message>
        <source>Check for updates on startup</source>
        <translation type="obsolete">Controlla aggiornamenti all&apos;avvio</translation>
    </message>
    <message>
        <location/>
        <source>Submit anonymous statistics to the Mumble project</source>
        <translation>Invia statistiche anonime al progetto Mumble</translation>
    </message>
    <message>
        <source>&lt;b&gt;Submit anonymous statistics.&lt;/b&gt;&lt;br /&gt;Mumble has a small development team, and as such needs to focus it&apos;s development where it&apos;s needed most. By submitting a bit of statistics you help the project determine where to focus development.</source>
        <translation type="obsolete">&lt;b&gt;Invia statistiche anonime.&lt;/b&gt;&lt;br /&gt;Mumble ha un piccolo team di sviluppo, e come tale ha bisogno di concentrare lo sviluppo dove è maggiormente necessario. Inviando un po&apos; di statistiche permetti al progetto di stabilire dove concentrare lo sviluppo.</translation>
    </message>
    <message>
        <location/>
        <source>Submit anonymous statistics</source>
        <translation>Invia statistiche anonime</translation>
    </message>
    <message>
        <location/>
        <source>Reconnect to last used server when starting Mumble</source>
        <translation>All&apos;avvio di Mumble riconnetti all&apos;ultimo server visitato</translation>
    </message>
    <message>
        <location/>
        <source>Reconnect to last server on startup</source>
        <translation>All&apos;avvio riconnetti all&apos;ultimo server visitato</translation>
    </message>
    <message>
        <location/>
        <source>Download plugin and overlay updates on startup</source>
        <translation>Aggiorna plugin e sovrapposizione all&apos;avvio</translation>
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
        <translation>Silenzio</translation>
    </message>
    <message>
        <location line="+3"/>
        <location filename="OverlayUser.cpp" line="+3"/>
        <source>Talking</source>
        <translation>Parlato</translation>
    </message>
    <message>
        <location line="+3"/>
        <location filename="OverlayUser.cpp" line="+3"/>
        <source>Whisper</source>
        <translation>&quot;Sussurro&quot;</translation>
    </message>
    <message>
        <location line="+3"/>
        <location filename="OverlayUser.cpp" line="+3"/>
        <source>Shout</source>
        <translation>&quot;Urlo&quot;</translation>
    </message>
    <message>
        <location line="+17"/>
        <location filename="OverlayUser.cpp" line="-110"/>
        <source>Channel</source>
        <translation>Canale</translation>
    </message>
    <message>
        <location filename="Overlay.cpp" line="+100"/>
        <source>Failed to create communication with overlay at %2: %1. No overlay will be available.</source>
        <translation>Impossibile creare una comunicazione con la sovrapposizione su %2 :%1. La sovrapposizione non sarà disponibile.</translation>
    </message>
    <message>
        <location line="+255"/>
        <source>Skipping overlay update in debug mode.</source>
        <translation>Salta gli aggiornamenti della sovrapposizione in modalità Debug.</translation>
    </message>
    <message>
        <location line="+66"/>
        <source>Downloaded new or updated overlay support file to %1.</source>
        <translation>Scaricato o aggiornato il file di supporto per la sovrapposzione alla versione %1.</translation>
    </message>
    <message>
        <source>Failed to load overlay library. This means either that:
- the library (mumble_ol.dll) wasn&apos;t found in the directory you ran Mumble from
- you&apos;re on an OS earlier than WinXP SP2
- you do not have the June 2007 updated version of DX9.0c</source>
        <translation type="obsolete">Caricamento della libreria di sovrapposizione fallito. Significa che o:
- la libreria (mumble_ol.dll) non è stata trovata nella directory dove stai eseguendo Mumble
- hai un OS precedente a WinXP SP2
- non hai la versione di DX9.0c aggiornata al June 2007</translation>
    </message>
    <message>
        <source>Failed to load overlay library. This means either that:
- the library (mumble_ol.dll) wasn&apos;t found in the directory you ran Mumble from
- you&apos;re on an OS earlier than WinXP SP2</source>
        <translation type="obsolete">Caricamento della libreria di sovrapposizione fallito. Significa che o:
- la libreria (mumble_ol.dll) non è stata trovata nella directory dove stai eseguendo Mumble
- hai un OS precedente a WinXP SP2</translation>
    </message>
    <message>
        <source>Failed to initialize overlay memory. This usually means that the shared memory is locked by the OS, and you need to reboot to release it.</source>
        <translation type="obsolete">Inizializzazione memoria di sovrapposizione fallita. Questo di solito vuol dire che la memoria condivisa è stata bloccata dal SO, devi riavviare per sbloccarla.</translation>
    </message>
</context>
<context>
    <name>OverlayClient</name>
    <message>
        <location filename="OverlayUserGroup.cpp" line="+94"/>
        <source>Filter</source>
        <translation>Filtro</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Only talking</source>
        <translation>Solo chi stà parlando</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>All in current channel</source>
        <translation>Tutti quelli nel canale attuale</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>All in linked channels</source>
        <translation>Tutti quelli nei canali collegati</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Always show yourself</source>
        <translation>Visualizza sempre se stessi</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Columns</source>
        <translation>Colonne</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Edit...</source>
        <translation>Modifica...</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Reset Zoom</source>
        <translation>Reimposta zoom</translation>
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
        <source>Show no one</source>
        <translation type="obsolete">Non mostrare nessuno</translation>
    </message>
    <message>
        <source>Show only talking</source>
        <translation type="obsolete">Mostra solo chi parla</translation>
    </message>
    <message>
        <source>Show everyone</source>
        <translation type="obsolete">Mostra tutti</translation>
    </message>
    <message>
        <source>Color for users</source>
        <translation type="obsolete">Colore per gli utenti</translation>
    </message>
    <message>
        <source>Color for talking users</source>
        <translation type="obsolete">Colore per gli utenti che parlano</translation>
    </message>
    <message>
        <source>Color for whispering users</source>
        <translation type="obsolete">Colore per gli utenti che sussurrano</translation>
    </message>
    <message>
        <source>Color for channels</source>
        <translation type="obsolete">Colore per i canali</translation>
    </message>
    <message>
        <source>Color for active channels</source>
        <translation type="obsolete">Colore per i canali attivi</translation>
    </message>
    <message>
        <location filename="OverlayConfig.cpp" line="+78"/>
        <source>To move the users, drag the little red dot.</source>
        <translation>Per muovere gli utenti, spostare il piccolo punto rosso.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>To resize the users, mouse wheel over a user.</source>
        <oldsource>To resize the users, mousewheel over a user.</oldsource>
        <translation>Per ridimensionare gli utenti, muovere la rotella del mouse.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>For more options, right click a user.</source>
        <oldsource>For more options, rightclick a user.</oldsource>
        <translation>Per le impostazioni avanzate, fare click con il pulsante destro su di un utente.</translation>
    </message>
    <message>
        <location line="+30"/>
        <source>FPS: %1</source>
        <translation>FPS: %1</translation>
    </message>
    <message>
        <location line="+194"/>
        <source>Overlay</source>
        <translation>Sovrapposizione</translation>
    </message>
    <message>
        <location line="+75"/>
        <source>Choose executable</source>
        <translation>Seleziona eseguibile</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Choose application</source>
        <translation>Seleziona applicazione</translation>
    </message>
    <message>
        <location line="+38"/>
        <source>Mumble has deemed the installer valid.</source>
        <translation>Mumble ha considerato valido il file di configurazione.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Mumble was unable to verify the authenticity of the installer.</source>
        <oldsource>Mumble was unable to validate authenticity of the installer.</oldsource>
        <translation>Mumble non è riuscito a verificare l&apos;autenticità del file di configurazione.</translation>
    </message>
    <message>
        <location line="+87"/>
        <source>Load Overlay Presets</source>
        <translation>Carica configurazione per la sovrapposizione</translation>
    </message>
    <message>
        <location line="+2"/>
        <location line="+28"/>
        <source>Mumble overlay presets (*.mumblelay)</source>
        <translation>File di configurazione della sovrapposizione (*.mumblelay)</translation>
    </message>
    <message>
        <location line="-2"/>
        <source>Save Overlay Presets</source>
        <translation>Salva configurazione della sovrapposizione</translation>
    </message>
    <message>
        <source>Form</source>
        <translation type="obsolete">Modulo</translation>
    </message>
    <message>
        <location filename="Overlay.ui"/>
        <source>Options</source>
        <translation>Opzioni</translation>
    </message>
    <message>
        <location/>
        <source>Enable overlay.</source>
        <translation>Abilita la sovrapposizione.</translation>
    </message>
    <message>
        <source>This sets whether the overlay is enabled or not. This setting is only checked with D3D9 applications are started, so make sure Mumble is running and this option is on before you start the application.&lt;br /&gt;Please note that if you start the application after starting Mumble, or if you disable the overlay while running, there is no safe way to restart the overlay without also restarting the application.</source>
        <translation type="obsolete">Stabilisce se la sovrapposizione è attivata o meno. Questa impostazione è settata solo con applicazioni D3D9 in esecuzione, quindi assicuratevi che Mumble sia in esecuzione e che questa opzione sia attiva prima di avviare l&apos;applicazione.&lt;br /&gt;Si prega di notare che se si avvia l&apos;applicazione dopo l&apos;avvio di Mumble, o se si disattiva la sovrapposizione durante l&apos;esecuzione, non vi è alcun modo sicuro per riavviare la sovrapposizione senza dover riavviare l&apos;applicazione.</translation>
    </message>
    <message>
        <location/>
        <source>Enable Overlay</source>
        <translation>Abilita la Sovrapposizione</translation>
    </message>
    <message>
        <source>Who to show on the overlay</source>
        <translation type="obsolete">Chi mostrare nella sovrapposizione</translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets who to show in the in-game overlay.&lt;/b&gt;&lt;br /&gt;If many people are connected to the same channel, the overlay list might be very long. Use this to shorten it.&lt;br /&gt;&lt;i&gt;No one&lt;/i&gt; - Don&apos;t show anyone (but leave overlay running).&lt;br /&gt;&lt;i&gt;Only talking&lt;/i&gt; - Only show talking people.&lt;br /&gt;&lt;i&gt;Everyone&lt;/i&gt; - Show everyone.</source>
        <translation type="obsolete">Stabilisce chi mostrare nella sovrapposizione in gioco.&lt;/b&gt;&lt;br /&gt;Se molte persone sono collegate allo stesso canale, la lista sovrapposta potrebbe essere molto lunga. Usa questo per accorciarla.&lt;br /&gt;&lt;i&gt;Nessuno&lt;/i&gt; - Non mostrare nessuno (ma lascia in esecuzione la sovrapposizione).&lt;br /&gt;&lt;i&gt;Solo chi stà parlando&lt;/i&gt; - Mostra solo chi stà parlando.&lt;br /&gt;&lt;i&gt;Tutti&lt;/i&gt; - Mostra tutti.</translation>
    </message>
    <message>
        <source>Always show yourself on overlay.</source>
        <translation type="obsolete">Mostra sempre se stessi nella sovrapposizione.</translation>
    </message>
    <message>
        <source>This sets whether to always show yourself or not. This setting is useful if you aren&apos;t showing everyone in the overlay, as then you would only see your own status if you were talking, which wouldn&apos;t let you see that you were deafened or muted.</source>
        <translation type="obsolete">Stabilisce se mostrare sempre te stesso o meno. Questa impostazione è utile se non stai visualizzando tutti nella sovrapposizione, dato che vedresti solo il tuo stato se stessi parlando, che non ti permette di vedere che sei statoi escluso o silenziato.</translation>
    </message>
    <message>
        <source>Always Show Self</source>
        <translation type="obsolete">Mostra sempre se stessi</translation>
    </message>
    <message>
        <source>Show User custom textures instead of text on the overlay.</source>
        <translation type="obsolete">Mostra una immagine personalizzata dell&apos;utente al posto della sovrapposizione testuale.</translation>
    </message>
    <message>
        <source>This sets whether to download and use custom textures for registered users. If disabled, the regular outline text will be used instead.</source>
        <translation type="obsolete">Imposta se scaricare e usare una immagine personalizzata per gli utenti registrati. Se disabilitato, verrà usato il testo standard.</translation>
    </message>
    <message>
        <source>Show User Textures</source>
        <translation type="obsolete">Mostra le immagini Utente</translation>
    </message>
    <message>
        <source>Position</source>
        <translation type="obsolete">Posizione</translation>
    </message>
    <message>
        <source>Let overlay grow upwards</source>
        <translation type="obsolete">Fa crescere la sovrapposizione verso l&apos;alto</translation>
    </message>
    <message>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow upwards if needed.</source>
        <translation type="obsolete">Tiene la sovrapposizione la più piccola possibile e alla posizione da te selezionata. Permette alla sovrapposizione di crescere verso l&apos;alto se necessario.</translation>
    </message>
    <message>
        <source>Grow Up</source>
        <translation type="obsolete">Cresci in alto</translation>
    </message>
    <message>
        <source>Y-Position of Overlay</source>
        <translation type="obsolete">Coordinata Y della sovrapposizione</translation>
    </message>
    <message>
        <source>This sets the relative Y position of the overlay.</source>
        <translation type="obsolete">Imposta la coordinata Y relativa della sovrapposizione.</translation>
    </message>
    <message>
        <source>Let overlay grow to the left</source>
        <translation type="obsolete">Fa crescere la sovrapposizione verso sinistra</translation>
    </message>
    <message>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow to the left if needed.</source>
        <translation type="obsolete">Tiene la sovrapposizione la più piccola possibile e alla posizione da te selezionata. Permette alla sovrapposizione di crescere verso sinistra se necessario.</translation>
    </message>
    <message>
        <source>Grow Left</source>
        <translation type="obsolete">Cresci a sinistra</translation>
    </message>
    <message>
        <source>Let overlay grow to the right</source>
        <translation type="obsolete">Fa crescere la sovrapposizione verso destra</translation>
    </message>
    <message>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow to the right if needed.</source>
        <translation type="obsolete">Tiene la sovrapposizione la più piccola possibile e alla posizione da te selezionata. Permette alla sovrapposizione di crescere verso destra se necessario.</translation>
    </message>
    <message>
        <source>Grow Right</source>
        <translation type="obsolete">Cresci a destra</translation>
    </message>
    <message>
        <source>Let overlay grow downwards</source>
        <translation type="obsolete">Fa crescere la sovrapposizione verso il basso</translation>
    </message>
    <message>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow downwards if needed.</source>
        <translation type="obsolete">Tiene la sovrapposizione la più piccola possibile e alla posizione da te selezionata. Permette alla sovrapposizione di crescere verso il basso se necessario.</translation>
    </message>
    <message>
        <source>Grow Down</source>
        <translation type="obsolete">Cresci in basso</translation>
    </message>
    <message>
        <source>X-Position of Overlay</source>
        <translation type="obsolete">Coordinata X della sovrapposizione</translation>
    </message>
    <message>
        <source>This sets the relative X position of the overlay.</source>
        <translation type="obsolete">Imposta la coordinata X relativa della sovrapposizione.</translation>
    </message>
    <message>
        <source>Current Font</source>
        <translation type="obsolete">Carattere corrente</translation>
    </message>
    <message>
        <source>TextLabel</source>
        <translation type="obsolete">EtichettaTesto</translation>
    </message>
    <message>
        <source>Set Font</source>
        <translation type="obsolete">Imposta Carattere</translation>
    </message>
    <message>
        <location/>
        <source>This sets whether the overlay is enabled or not. This setting is only checked when applications are started, so make sure Mumble is running and this option is on before you start the application.&lt;br /&gt;Please note that if you start Mumble after starting the application, or if you disable the overlay while the application is running, there is no safe way to restart the overlay without also restarting the application.</source>
        <oldsource>This sets whether the overlay is enabled or not. This setting is only checked when applications are started, so make sure Mumble is running and this option is on before you start the application.&lt;br /&gt;Please note that if you start the application after starting Mumble, or if you disable the overlay while running, there is no safe way to restart the overlay without also restarting the application.</oldsource>
        <translation>Stabilisce se la sovrapposizione è attivata oppure no. Questa impostazione è settata solo con applicazioni in esecuzione, quindi assicuratevi che Mumble sia in esecuzione e che questa opzione sia attiva prima di avviare l&apos;applicazione.&lt;br /&gt;Si noti che se si avvia Mumble prima dell&apos;applicazione, o se si disattiva la sovrapposizione durante l&apos;esecuzione, non vi è alcun modo sicuro per riavviare la sovrapposizione senza dover riavviare l&apos;applicazione.</translation>
    </message>
    <message>
        <source>Maximum height</source>
        <translation type="obsolete">Altezza massima</translation>
    </message>
    <message>
        <source>Maximum height of names.</source>
        <translation type="obsolete">Massima altezza dei nomi.</translation>
    </message>
    <message>
        <source>This sets the maximum height of names shown, relative to the screen height. If your active 3D window is 800 pixels tall and this is set to 5%, each name will be 40 pixels tall. Note that the names will not be taller than 60 pixels no matter what you set here.</source>
        <translation type="obsolete">Stabilisce l&apos;altezza massima dei nomi mostrati, rispetto all&apos;altezza dello schermo. Se la finestra 3D attiva è di 800 pixel di altezza e questa è impostata al 5%, ogni nome sarà di 40 pixel di altezza. Si noti che i nomi non saranno più alti di 60 pixel, con qualsiasi impostazione.</translation>
    </message>
    <message>
        <source>Color for players</source>
        <translation type="obsolete">Colore per i giocatori</translation>
    </message>
    <message>
        <source>Change</source>
        <translation type="obsolete">Cambia</translation>
    </message>
    <message>
        <source>Color for talking players</source>
        <translation type="obsolete">Colore per il giocatore parlante</translation>
    </message>
    <message>
        <source>Color for alt-talking players</source>
        <translation type="obsolete">Colore per il giocatore Alt parlante</translation>
    </message>
    <message>
        <source>Color for Channels</source>
        <translation type="obsolete">Colore per i Canali</translation>
    </message>
    <message>
        <source>Color for active Channels</source>
        <translation type="obsolete">Colore per i Canali attivi</translation>
    </message>
    <message>
        <location/>
        <source>Layout</source>
        <translation>Aspetto</translation>
    </message>
    <message>
        <location/>
        <source>Overlay exceptions</source>
        <translation>Applicazioni escluse</translation>
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
        <translation type="obsolete">Aggiungi</translation>
    </message>
    <message>
        <location/>
        <source>Remove</source>
        <translation>Rimuovi</translation>
    </message>
    <message>
        <location/>
        <source>Uninstall Overlay</source>
        <translation>Disinstalla la sovrapposizione</translation>
    </message>
    <message>
        <location/>
        <source>Overlay Installation</source>
        <translation>Installazione Sovrapposizione</translation>
    </message>
    <message>
        <location/>
        <source>Mumble has detected that you do not have the Mumble Overlay installed.

Click the button below to install the overlay.</source>
        <translation>Mumble ha rilevato che non hai la Sovrapposizione installata.

Premere il pulsante sottostante per installarla.</translation>
    </message>
    <message>
        <location/>
        <source>Install Mumble Overlay</source>
        <translation>Installa la sovrapposizione di Mumble</translation>
    </message>
    <message>
        <location/>
        <source>Overlay Upgrade</source>
        <translation>Aggiornamento Sovrapposizione</translation>
    </message>
    <message>
        <location/>
        <source>Mumble has detected an old version of the overlay support files installed on your computer.

To upgrade these files to their latest versions, click the button below.</source>
        <translation>Mumble ha rilevato una vecchia versione dei file di configurazione della sovrapposizione.

Per aggiornare questi file all&apos;ultima versione, premi il pulsante sottostante.</translation>
    </message>
    <message>
        <location/>
        <source>Upgrade Mumble Overlay</source>
        <translation>Aggiorna Sovrapposizione di Mumble</translation>
    </message>
    <message>
        <location/>
        <source>Display a frame counter in the overlay</source>
        <translation>Visualizza un contatore di frame nella sovrapposizione</translation>
    </message>
    <message>
        <location/>
        <source>Show FPS counter</source>
        <translation>Visualizza contatore FPS</translation>
    </message>
    <message>
        <location/>
        <source>View Certificates</source>
        <translation>Visualizza certificati</translation>
    </message>
    <message>
        <location/>
        <source>Load an overlay preset from file</source>
        <translation>Carica da file una configurazione per la sovrapposizione</translation>
    </message>
    <message utf8="true">
        <location/>
        <source>Load…</source>
        <translation>Carica...</translation>
    </message>
    <message>
        <location/>
        <source>Save your overlay settings to file</source>
        <translation>Salva la configurazione della sovrapposizione su file</translation>
    </message>
    <message utf8="true">
        <location/>
        <source>Save…</source>
        <translation>Salva...</translation>
    </message>
    <message>
        <location/>
        <source>FPS Display</source>
        <translation>Visualizza FPS</translation>
    </message>
    <message>
        <location/>
        <source>Set the overlay font.</source>
        <translation>Imposta il carattere del testo della sovrapposizione.</translation>
    </message>
    <message>
        <location/>
        <source>Set the overlay text color.</source>
        <translation>Imposta il colore del testo della sovrapposizione.</translation>
    </message>
    <message>
        <location/>
        <source>Color</source>
        <translation>Colore</translation>
    </message>
    <message>
        <location/>
        <source>Add...</source>
        <translation>Aggiungi...</translation>
    </message>
    <message>
        <location/>
        <source>Font</source>
        <translation>Carattere</translation>
    </message>
</context>
<context>
    <name>OverlayEditor</name>
    <message>
        <source>Dialog</source>
        <translation type="obsolete">Dialogo</translation>
    </message>
    <message>
        <location filename="OverlayEditor.ui"/>
        <source>State</source>
        <translation>Stato</translation>
    </message>
    <message>
        <location/>
        <source>User is not talking</source>
        <translation>L&apos;utente non stà parlando</translation>
    </message>
    <message>
        <location/>
        <source>Passive</source>
        <translation>Silenzio</translation>
    </message>
    <message>
        <location/>
        <source>User is talking in your channel or a linked channel</source>
        <translation>L&apos;utente stà parlando nel tuo canale o in uno collegato</translation>
    </message>
    <message>
        <location/>
        <source>Talking</source>
        <translation>Parlato</translation>
    </message>
    <message>
        <location/>
        <source>User is whispering to you privately</source>
        <translation>L&apos;utente ti stà &quot;sussurrando&quot; privatamente</translation>
    </message>
    <message>
        <location/>
        <source>Private Whisper</source>
        <translation>&quot;Sussurro&quot; privato</translation>
    </message>
    <message>
        <location/>
        <source>User is shouting to your channel</source>
        <translation>L&apos;utente stà &quot;urlando&quot; al tuo canale</translation>
    </message>
    <message>
        <location/>
        <source>Channel Whisper</source>
        <translation>&quot;Sussurro&quot; al canale</translation>
    </message>
    <message>
        <location/>
        <source>Zoom</source>
        <translation>Zoom</translation>
    </message>
    <message>
        <location/>
        <source>Zoom Factor</source>
        <translation>Fattore di Zoom</translation>
    </message>
    <message>
        <location/>
        <source>Enabled Elements</source>
        <translation>Elementi abilitati</translation>
    </message>
    <message>
        <location/>
        <source>User avatar, chosen by each user</source>
        <translation>Avatar dell&apos;utente, scelto da ogni utente</translation>
    </message>
    <message>
        <location/>
        <source>Avatar</source>
        <translation>Avatar</translation>
    </message>
    <message>
        <location/>
        <source>User&apos;s name</source>
        <translation>Nome dell&apos;utente</translation>
    </message>
    <message>
        <location/>
        <source>Username</source>
        <translation>Nome utente</translation>
    </message>
    <message>
        <location/>
        <source>Name of user&apos;s channel, if outside your current channel</source>
        <translation>Nome del canale dell&apos;utente, se lui si trova fuori dal tuo stesso canale</translation>
    </message>
    <message>
        <location/>
        <source>Channel</source>
        <translation>Canale</translation>
    </message>
    <message>
        <location/>
        <source>Muted or deafened</source>
        <translation>Microfono o audio disattivati</translation>
    </message>
    <message>
        <location/>
        <source>Mute state</source>
        <translation>Stato</translation>
    </message>
    <message>
        <location/>
        <source>Bounding box, automatically shrunk to minimum size to contain all visible elements</source>
        <translation>Riquadro, si dimensiona automaticamente per contenere tutti gli elementi visibili</translation>
    </message>
    <message>
        <location/>
        <source>Bounding box</source>
        <translation>Riquadro</translation>
    </message>
    <message>
        <location/>
        <source>Overlay Editor</source>
        <translation>Editor della sovrapposizione</translation>
    </message>
</context>
<context>
    <name>OverlayEditorScene</name>
    <message>
        <location filename="OverlayEditorScene.cpp" line="+396"/>
        <source>Layout preset</source>
        <translation>Preimpostazioni</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Large square avatar</source>
        <translation>Avatar larghi e quadrati</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Avatar and Name</source>
        <translation>Avatar e Nome</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>User Opacity</source>
        <translation>Opacità utenti</translation>
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
        <translation>Opacità oggetti</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Alignment</source>
        <translation>Allineamento</translation>
    </message>
    <message>
        <location line="+11"/>
        <source>Left</source>
        <translation>Sinistra</translation>
    </message>
    <message>
        <location line="+5"/>
        <location line="+18"/>
        <source>Center</source>
        <translation>Centrato</translation>
    </message>
    <message>
        <location line="-13"/>
        <source>Right</source>
        <translation>Destra</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Top</source>
        <translation>Alto</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Bottom</source>
        <translation>Basso</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Color...</source>
        <translation>Colore...</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Font...</source>
        <translation>Carattere...</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Bounding box</source>
        <translation>Riquadro</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Pen width</source>
        <translation>Larghezza penna</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Padding</source>
        <translation>Riempimento</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Pen color</source>
        <translation>Colore penna</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Fill color</source>
        <translation>Colore del riempimento</translation>
    </message>
    <message>
        <location line="+87"/>
        <source>Pick pen color</source>
        <translation>Seleziona il colore della penna</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Pick fill color</source>
        <translation>Seleziona il colore di riempimento</translation>
    </message>
    <message>
        <location line="+14"/>
        <source>Pick color</source>
        <translation>Colore penna</translation>
    </message>
    <message>
        <location line="+21"/>
        <source>Pick font</source>
        <translation>Seleziona il carattere</translation>
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
        <translation>Caricamento della libreria di sovrapposizione fallito. Significa che o:
- la libreria (mumble_ol.dll) non è stata trovata nella directory dove stai eseguendo Mumble
- hai un SO precedente a WinXP SP2</translation>
    </message>
</context>
<context>
    <name>OverlayUser</name>
    <message>
        <source>Whisper</source>
        <translation type="obsolete">Sussurra</translation>
    </message>
    <message>
        <source>%1%</source>
        <translation type="obsolete">%1%</translation>
    </message>
</context>
<context>
    <name>PlayerModel</name>
    <message>
        <source>This is a player connected to the server. The icon to the left of the player indicates whether or not they are talking:&lt;br /&gt;&lt;img src=&quot;skin:talking_on.png&quot; /&gt; Talking&lt;br /&gt;&lt;img src=&quot;skin:talking_off.png&quot; /&gt; Not talking</source>
        <translation type="obsolete">Giocatore connesso al server. L&apos;icona alla sinistra del giocatore indica se sta parlando:&lt;br /&gt;&lt;img src=&quot;skin:talking_on.png&quot; /&gt;Parla&lt;br /&gt;&lt;img src=&quot;skin:talking_off.png&quot; /&gt;In silenzio</translation>
    </message>
    <message>
        <source>This is a channel on the server. Only players in the same channel can hear each other.</source>
        <translation type="obsolete">Canale sul server. Solamente i giocatori nello stesso canale potranno sentirsi tra loro.</translation>
    </message>
    <message>
        <source>This shows the flags the player has on the server, if any:&lt;br /&gt;&lt;img src=&quot;skin:authenticated.png&quot; /&gt;Authenticated user&lt;br /&gt;&lt;img src=&quot;skin:muted_self.png&quot; /&gt;Muted (by self)&lt;br /&gt;&lt;img src=&quot;skin:muted_server.png&quot; /&gt;Muted (by admin)&lt;br /&gt;&lt;img src=&quot;skin:deafened_self.png&quot; /&gt;Deafened (by self)&lt;br /&gt;&lt;img src=&quot;skin:deafened_server.png&quot; /&gt;Deafened (by admin)&lt;br /&gt;A player muted by himself is probably just away, talking on the phone or something like that.&lt;br /&gt;A player muted by an admin is probably also just away, and the noise the player is making was annoying enough that an admin muted him.</source>
        <translation type="obsolete">Mostra le impostazioni che il giocatore ha sul server, se ne ha:&lt;br /&gt;&lt;img src=&quot;skin:authenticated.png&quot; /&gt;Utente Autenticato&lt;br /&gt;&lt;img src=&quot;skin:muted_self.png&quot; /&gt;Silenziato (Auto)&lt;br /&gt; &lt;img src=&quot;skin:muted_server.png&quot; /&gt;Silenziato (da admin)&lt;br /&gt;&lt;img src=&quot;skin:deafened_self.png&quot; /&gt; Escluso (Auto)&lt;br /&gt;&lt;img src=&quot;skin:deafened_server.png&quot; /&gt;Escluso (da admin)&lt;br /&gt;Un giocatore autosilenziato probabilmente è solo assente, sta parlando al telefono o qualcosa di simile.&lt;br /&gt;Un giocatore silenziato da un amministratore probabilmente anche lui assente, e il rumore che il giocatore stava facendo era abbastanza fastidioso che un amministratore l&apos;ha silenziato.</translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="obsolete">Nome</translation>
    </message>
    <message>
        <source>Flags</source>
        <translation type="obsolete">Nome</translation>
    </message>
    <message>
        <source>Mumble</source>
        <translation type="obsolete">Mumble</translation>
    </message>
    <message>
        <source>Are you sure you want to drag this channel?</source>
        <translation type="obsolete">Sei sicuro di voler spostare questo canale?</translation>
    </message>
</context>
<context>
    <name>PluginConfig</name>
    <message>
        <location filename="Plugins.ui"/>
        <location filename="Plugins.cpp" line="+75"/>
        <source>Plugins</source>
        <translation>Plugin</translation>
    </message>
    <message>
        <source>Mumble</source>
        <translation type="obsolete">Mumble</translation>
    </message>
    <message>
        <location filename="Plugins.cpp" line="+56"/>
        <source>Plugin has no configure function.</source>
        <translation>Il Plugins non ha funzioni di configurazione.</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Plugin has no about function.</source>
        <translation>Il Plugin non ha informazioni.</translation>
    </message>
    <message>
        <location filename="Plugins.ui"/>
        <source>Form</source>
        <translation>Modulo</translation>
    </message>
    <message>
        <location/>
        <source>Options</source>
        <translation>Opzioni</translation>
    </message>
    <message>
        <location/>
        <source>Enable plugins and transmit positional information</source>
        <translation>Abilita i plugins e trasmette le informazioni posizionali</translation>
    </message>
    <message>
        <source>This enables plugins for supported games to fetch your in-game position and transmit that with each voice packet. This enables other players to hear your voice in-game from the direction your character is in relation to their own.</source>
        <translation type="obsolete">Abilita i plugins per i giochi supportati a ottenere la tua posizione nel gioco e la trasmette con ogni pacchetto voce. Questo consente agli altri giocatori di sentire la tua voce dalla direzione con cui il tuo personaggio è in relazione alla loro.</translation>
    </message>
    <message>
        <location/>
        <source>This allows plugins for supported games to fetch your in-game position and transmit it with each voice packet. This enables other users to hear your voice in-game from the direction your character is in relation to their own.</source>
        <translation>Questo permette ai plugin per i giochi supportati di capire la tua posizione nel gioco e trasmettere l&apos;audio di conseguenza. Questo permette inoltre agli altri utenti di sentire la tua voce dalla direzione del tuo personaggio e in relazione alla posizione del loro.</translation>
    </message>
    <message>
        <location/>
        <source>Link to Game and Transmit Position</source>
        <translation>Collega al Gioco e Trasmetti la Posizione</translation>
    </message>
    <message>
        <location/>
        <source>Reloads all plugins</source>
        <translation>Ricarica tutti i plugin</translation>
    </message>
    <message>
        <location/>
        <source>This rescans and reloads plugins. Use this if you just added or changed a plugin to the plugins directory.</source>
        <translation>Ricerca e ricarica tutti i plugin. Usare se hai appena aggiunto o cambiato un plugin nella cartella dei plugin.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Reload plugins</source>
        <translation>&amp;Ricarica i plugin</translation>
    </message>
    <message>
        <location/>
        <source>Information about plugin</source>
        <translation>Informazioni sul plugin</translation>
    </message>
    <message>
        <location/>
        <source>This shows a small information message about the plugin.</source>
        <translation>Mostra qualche informazione riguardo al plugin.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;About</source>
        <translation>&amp;Informazioni su</translation>
    </message>
    <message>
        <location/>
        <source>Show configuration page of plugin</source>
        <translation>Mostra la pagina di configurazione del plugin</translation>
    </message>
    <message>
        <location/>
        <source>This shows the configuration page of the plugin, if any.</source>
        <translation>Mostra la pagina di configurazione del plugin, se c&apos;è.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Configure</source>
        <translation>&amp;Configura</translation>
    </message>
    <message>
        <location/>
        <source>Name</source>
        <translation>Nome</translation>
    </message>
    <message>
        <location/>
        <source>Enabled</source>
        <translation>Abilitato</translation>
    </message>
</context>
<context>
    <name>Plugins</name>
    <message>
        <location filename="Plugins.cpp" line="+330"/>
        <source>Skipping plugin update in debug mode.</source>
        <translation>Salta gli aggiornamenti dei plugin in modalità Debug.</translation>
    </message>
    <message>
        <location line="+155"/>
        <location line="+6"/>
        <source>Downloaded new or updated plugin to %1.</source>
        <translation>Scaricato nuovo o aggiornato plugin a %1.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Failed to install new plugin to %1.</source>
        <translation>Installazione fallita del nouvo plugin a %1.</translation>
    </message>
    <message>
        <location line="-265"/>
        <source>%1 lost link.</source>
        <translation>Collegamento perso con %1.</translation>
    </message>
    <message>
        <location line="+74"/>
        <source>%1 linked.</source>
        <translation>collegato con %1.</translation>
    </message>
</context>
<context>
    <name>PortAudioSystem</name>
    <message>
        <location filename="PAAudio.cpp" line="+273"/>
        <source>Default Device</source>
        <translation>Periferica predefinita</translation>
    </message>
</context>
<context>
    <name>PulseAudioSystem</name>
    <message>
        <location filename="PulseAudio.cpp" line="+545"/>
        <source>Default Input</source>
        <translation>Ingresso predefinito</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Default Output</source>
        <translation>Uscita predefinita</translation>
    </message>
</context>
<context>
    <name>RichTextEditor</name>
    <message>
        <location filename="RichTextEditor.cpp" line="+241"/>
        <source>Failed to load image</source>
        <translation>Caricamento immagine fallito</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Image file too large to embed in document. Please use images smaller than %1 kB.</source>
        <translation>File immagine troppo grande da inserire nel documento. Per favore inserisci un&apos;immagine più piccola di %1 kB.</translation>
    </message>
    <message>
        <location line="+81"/>
        <source>Message is too long.</source>
        <translation>Il messaggio è troppo lungo.</translation>
    </message>
    <message>
        <source>TabWidget</source>
        <translation type="obsolete">TabWidget</translation>
    </message>
    <message>
        <location filename="RichTextEditor.ui"/>
        <source>Display</source>
        <translation>Visualizza</translation>
    </message>
    <message>
        <location/>
        <source>Source Text</source>
        <translation>Testo sorgente</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Bold</source>
        <translation>&amp;Grassetto</translation>
    </message>
    <message>
        <location/>
        <source>Ctrl+B</source>
        <translation>Ctrl+B</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Italic</source>
        <translation>&amp;Corsivo</translation>
    </message>
    <message>
        <location/>
        <source>Italic</source>
        <translation>Corsivo</translation>
    </message>
    <message>
        <location/>
        <source>Ctrl+I</source>
        <translation>Ctrl+I</translation>
    </message>
    <message>
        <location/>
        <source>Underline</source>
        <translation>Sottolineato</translation>
    </message>
    <message>
        <location/>
        <source>Ctrl+U</source>
        <translation>Ctrl+U</translation>
    </message>
    <message>
        <location/>
        <source>Color</source>
        <translation>Colore</translation>
    </message>
    <message>
        <location/>
        <source>Insert Link</source>
        <translation>Inserisci link</translation>
    </message>
    <message>
        <location/>
        <source>Ctrl+L</source>
        <translation>Ctrl+L</translation>
    </message>
    <message>
        <location/>
        <source>Insert Image</source>
        <translation>Inserisci immagine</translation>
    </message>
</context>
<context>
    <name>RichTextEditorLink</name>
    <message>
        <location filename="RichTextEditorLink.ui"/>
        <source>Add Link</source>
        <translation>Aggiungi collegamento</translation>
    </message>
    <message>
        <location/>
        <source>URL</source>
        <translation>Indirizzo</translation>
    </message>
    <message>
        <location/>
        <source>Text</source>
        <translation>Testo</translation>
    </message>
</context>
<context>
    <name>ServerHandler</name>
    <message>
        <location filename="ServerHandler.cpp" line="+401"/>
        <source>UDP packets cannot be sent to or received from the server. Switching to TCP mode.</source>
        <translation>Impossibile inviare o ricevere pacchetti UDP dal server. Cambio modalità su TCP.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>UDP packets cannot be sent to the server. Switching to TCP mode.</source>
        <translation>Impossibile inviare pacchetti UDP al server. Cambio modalità su TCP.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>UDP packets cannot be received from the server. Switching to TCP mode.</source>
        <translation>Impossibile ricevere pacchetti UDP dal server. Cambio modalità su TCP.</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>UDP packets can be sent to and received from the server. Switching back to UDP mode.</source>
        <translation>I pacchetti UDP possono essere inviati e ricevuti dal server. Ritorno a modalità UDP.</translation>
    </message>
    <message>
        <location line="+36"/>
        <source>Connection timed out</source>
        <translation>Tempo per la connessione esaurito</translation>
    </message>
</context>
<context>
    <name>ServerView</name>
    <message>
        <location filename="ConnectDialog.cpp" line="-611"/>
        <source>Favorite</source>
        <translation>Preferiti</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>LAN</source>
        <translation>LAN</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Public Internet</source>
        <translation>Internet</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Africa</source>
        <translation>Africa</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Asia</source>
        <translation>Asia</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>North America</source>
        <translation>Nord America</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>South America</source>
        <translation>Sud America</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Europe</source>
        <translation>Europa</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Oceania</source>
        <translation>Oceania</translation>
    </message>
</context>
<context>
    <name>ShortcutActionWidget</name>
    <message>
        <location filename="GlobalShortcut.cpp" line="-483"/>
        <source>Unassigned</source>
        <translation>Non assegnato</translation>
    </message>
</context>
<context>
    <name>ShortcutDelegate</name>
    <message>
        <location line="+361"/>
        <source>On</source>
        <translation>On</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Off</source>
        <translation>Off</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Toggle</source>
        <translation>Imposta</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Unassigned</source>
        <translation>Non asseganto</translation>
    </message>
</context>
<context>
    <name>ShortcutKeyWidget</name>
    <message>
        <location line="-435"/>
        <source>Press Shortcut</source>
        <translation>Premi scorciatoia</translation>
    </message>
</context>
<context>
    <name>ShortcutTargetDialog</name>
    <message>
        <location line="+206"/>
        <source>Root</source>
        <translation>Radice</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Parent</source>
        <translation>Principale</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Current</source>
        <translation>Attuale</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Subchannel #%1</source>
        <translation>Sotto-canale #%1</translation>
    </message>
</context>
<context>
    <name>ShortcutTargetWidget</name>
    <message>
        <location line="+85"/>
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
        <translation>Radice</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Parent</source>
        <translation>Superiore</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Current</source>
        <translation>Attuale</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Subchannel #%1</source>
        <translation>Sottocanale #%1</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Invalid</source>
        <translation>Non valido</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Empty</source>
        <oldsource>&lt;Empty&gt;</oldsource>
        <translation>Vuoto</translation>
    </message>
</context>
<context>
    <name>ShortcutToggleWidget</name>
    <message>
        <location line="-259"/>
        <source>Off</source>
        <translation>Off</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Toggle</source>
        <translation>Imposta</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>On</source>
        <translation>On</translation>
    </message>
</context>
<context>
    <name>TextMessage</name>
    <message>
        <source>Send raw message</source>
        <translation type="obsolete">Invia messaggio grezzo</translation>
    </message>
    <message>
        <source>Disables html formating</source>
        <translation type="obsolete">Disabilita formattazione html</translation>
    </message>
    <message>
        <source>Message to send</source>
        <translation type="obsolete">Messaggio da mandare</translation>
    </message>
    <message>
        <source>Preview</source>
        <translation type="obsolete">Anteprima</translation>
    </message>
    <message>
        <location filename="TextMessage.h" line="+46"/>
        <source>Enter text</source>
        <translation>Inserisci testo</translation>
    </message>
    <message>
        <location filename="TextMessage.ui"/>
        <source>If checked the message is recursively sent to all subchannels</source>
        <translation>Se selezionato il messaggio verrà inviato ricorsivamente a tutti i sottocanali</translation>
    </message>
    <message>
        <location/>
        <source>Send recursively to subchannels</source>
        <translation>Invia ricorsivamente ai sottocanali</translation>
    </message>
</context>
<context>
    <name>Tokens</name>
    <message>
        <location filename="Tokens.cpp" line="+69"/>
        <source>Empty Token</source>
        <translation>Token vuoto</translation>
    </message>
    <message>
        <location filename="Tokens.ui"/>
        <source>Mumble - Access Tokens</source>
        <translation>Mumble - Token di accesso</translation>
    </message>
    <message>
        <location/>
        <source>List of access tokens on current server</source>
        <translation>Lista di token di accesso sul server attuale</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is an editable list of access tokens on the connected server.&lt;/b&gt;
&lt;br /&gt;
An access token is a text string, which can be used as a password for very simple access management on channels. Mumble will remember the tokens you&apos;ve used and resend them to the server next time you reconnect, so you don&apos;t have to enter these every time.
</source>
        <translation>&lt;b&gt;Questa è una lista modificabile dei token di accesso sul server connesso.&lt;/b&gt;
&lt;br /&gt;Un token di Accesso è una stringa di testo, che può essere usata come password per una semplice amministrazione delle password dei canali. Mumble ricorderà i token usati e li invierà di nuovo al server la prossima volta che ti connetterai, così non dovrai reinserirle ogni volta.
</translation>
    </message>
    <message>
        <location/>
        <source>Add a token</source>
        <translation>Aggiunge un token</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Add</source>
        <translation>&amp;Aggiungi</translation>
    </message>
    <message>
        <location/>
        <source>Remove a token</source>
        <translation>Rimuove un token</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Remove</source>
        <translation>&amp;Rimuovi</translation>
    </message>
</context>
<context>
    <name>UserEdit</name>
    <message>
        <location filename="UserEdit.ui"/>
        <source>Registered Users</source>
        <translation>Utenti registrati</translation>
    </message>
    <message>
        <location/>
        <location filename="UserEdit.cpp" line="+96"/>
        <source>Remove</source>
        <translation>Rimuovi</translation>
    </message>
    <message>
        <location filename="UserEdit.cpp" line="-5"/>
        <source>Rename</source>
        <translation>Rinomina</translation>
    </message>
</context>
<context>
    <name>UserInformation</name>
    <message>
        <location filename="UserInformation.ui"/>
        <source>User Information</source>
        <translation>Informazione utente</translation>
    </message>
    <message>
        <location/>
        <source>Connection Information</source>
        <translation>Informazione connessione</translation>
    </message>
    <message>
        <location/>
        <source>Version</source>
        <translation>Versione</translation>
    </message>
    <message>
        <location/>
        <source>OS</source>
        <translation>SO</translation>
    </message>
    <message>
        <location/>
        <source>Certificate</source>
        <translation>Certificato</translation>
    </message>
    <message>
        <location/>
        <source>IP Address</source>
        <translation>Indirizzo IP</translation>
    </message>
    <message>
        <location/>
        <source>CELT Versions</source>
        <translation>Versioni CELT</translation>
    </message>
    <message>
        <location/>
        <source>Details...</source>
        <translation>Dettagli...</translation>
    </message>
    <message>
        <location/>
        <source>Ping Statistics</source>
        <translation>Statistiche ping</translation>
    </message>
    <message>
        <location/>
        <source>Pings received</source>
        <translation>Ping ricevuti</translation>
    </message>
    <message>
        <location/>
        <source>Average ping</source>
        <translation>Ping medio</translation>
    </message>
    <message>
        <location/>
        <source>TCP (Control)</source>
        <translation>TCP (Controllo)</translation>
    </message>
    <message>
        <location/>
        <source>UDP (Voice)</source>
        <translation>UDP (Voce)</translation>
    </message>
    <message>
        <location/>
        <source>UDP Network statistics</source>
        <translation>Statistiche UDP rete</translation>
    </message>
    <message>
        <location/>
        <source>Good</source>
        <translation>Buono</translation>
    </message>
    <message>
        <location/>
        <source>Late</source>
        <translation>Ultimo</translation>
    </message>
    <message>
        <location/>
        <source>Lost</source>
        <translation>Perso</translation>
    </message>
    <message>
        <location/>
        <source>Resync</source>
        <translation>Risincronizzazione</translation>
    </message>
    <message>
        <location/>
        <source>From Client</source>
        <translation>Dal Client</translation>
    </message>
    <message>
        <location/>
        <source>To Client</source>
        <translation>Al Client</translation>
    </message>
    <message>
        <source>Bandwidth</source>
        <translation type="obsolete">Banda</translation>
    </message>
    <message>
        <location/>
        <source>Connection time</source>
        <translation>Tempo di connessione</translation>
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
        <translation>%1 online (%2 inattivo)</translation>
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
        <translation>Banda</translation>
    </message>
    <message>
        <location/>
        <source>Bandwidth</source>
        <comment>Label</comment>
        <translation>Banda</translation>
    </message>
</context>
<context>
    <name>UserModel</name>
    <message>
        <location filename="UserModel.cpp" line="-804"/>
        <source>This is a user connected to the server. The icon to the left of the user indicates whether or not they are talking:</source>
        <translation>Questo è un utente connesso al server. L&apos;icona sulla sinistra dell&apos;utente indica se stà parlando o no:</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Talking to your channel.</source>
        <translation>Parla al tuo canale.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Shouting directly to your channel.</source>
        <oldsource>Whispering directly to your channel.</oldsource>
        <translation>&quot;Sussurra&quot; direttamente al tuo canale.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Whispering directly to you.</source>
        <translation>Sussurra direttamente a te.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Not talking.</source>
        <translation>Non stà parlando.</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>This is a channel on the server. The icon indicates the state of the channel:</source>
        <translation>Questo è un canale nel server. L&apos;icona indica lo stato del canale:</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Your current channel.</source>
        <translation>Il tuo canale attuale.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>A channel that is linked with your channel. Linked channels can talk to each other.</source>
        <translation>Un canale che è collegato al tuo canale. Canali collegati possono parlare tra di loro.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>A channel on the server that you are not linked to.</source>
        <translation>Un canale nel server al quale non sei collegato.</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>This shows the flags the user has on the server, if any:</source>
        <translation>Questo visualizza i segnali che l&apos;utente possiede nel server, se ce ne sono:</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>On your friend list</source>
        <translation>Sulla tua lista amici</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Authenticated user</source>
        <translation>Utente autenticato</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Muted (manually muted by self)</source>
        <translation>Microfono disattivato (manualmente e da solo)</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Muted (manually muted by admin)</source>
        <translation>Microfono disattivato (manualmente da un&apos;amministratore)</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Muted (not allowed to speak in current channel)</source>
        <translation>Microfono disattivato (non abilitato a parlare nel canale attuale)</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Muted (muted by you, only on your machine)</source>
        <translation>Microfono disattivato (da te, solo sul tuo PC)</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Deafened (by self)</source>
        <translation>Audio disattivato (da solo)</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Deafened (by admin)</source>
        <translation>Audio disattivato (da un amministratore)</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>User has a new comment set (click to show)</source>
        <translation>Utente che ha un nuovo commento impostato (click per vedere)</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>User has a comment set, which you&apos;ve already seen. (click to show)</source>
        <translation>Utente che ha un commento impostato, che hai già visto. (click per vedere)</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>This shows the flags the channel has, if any:</source>
        <translation>Questo visualizza i segnali che il canale possiede, se ce ne sono:</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Channel has a new comment set (click to show)</source>
        <translation>Canale che ha un nuovo commento impostato (click per vedere)</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Channel has a comment set, which you&apos;ve already seen. (click to show)</source>
        <translation>Canale che ha un commento impostato, che hai già visto. (click per vedere)</translation>
    </message>
    <message>
        <location line="+18"/>
        <source>Name</source>
        <translation>Nome</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Flags</source>
        <translation>Segnali</translation>
    </message>
    <message>
        <source>Mumble</source>
        <translation type="obsolete">Mumble</translation>
    </message>
    <message>
        <location line="+721"/>
        <source>Are you sure you want to drag this channel?</source>
        <translation>Sei sicuro di voler spostare questo canale?</translation>
    </message>
    <message>
        <location line="+93"/>
        <location line="+20"/>
        <source>Cannot perform this movement automatically, please reset the numeric sorting indicators or adjust it manually.</source>
        <translation>Impossibile eseguire questo movimento automaticamente, per favore reimposta gli indicatori numerici o aggiustalo manualmente.</translation>
    </message>
</context>
<context>
    <name>VersionCheck</name>
    <message>
        <source>Mumble failed to retrieve version information from the SourceForge server.</source>
        <translation type="obsolete">Mumble non è riuscito a recuperare le informazioni sull&apos;ultima versione dal server SourceForge.</translation>
    </message>
    <message>
        <location filename="VersionCheck.cpp" line="+130"/>
        <source>Upgrade Mumble</source>
        <translation>Aggiorna Mumble</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>A new version of Mumble has been detected and automatically downloaded. It is recommended that you either upgrade to this version, or downgrade to the latest stable release. Do you want to launch the installer now?</source>
        <translation>Una nuova versione di Mumble è stata individuata e scaricata automaticamente. Si raccomanda di aggiornare a questa versione, o di reinstallare l&apos;ultima versione stabile. Vuoi lanciare l&apos;installazione ora?</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Failed to launch snapshot installer.</source>
        <translation>Impossibile lanciare l&apos;installazione della Snapshot.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Corrupt download of new version detected. Automatically removed.</source>
        <translation>La nuova versione scaricata si è rivelata corrotta ed è quindi stata automaticamente rimossa.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Downloading new snapshot from %1 to %2</source>
        <translation>Download in corso della Snapshot %2 (attuale %1)</translation>
    </message>
    <message>
        <location line="+18"/>
        <source>Failed to write new version to disk.</source>
        <oldsource>Failed to write new version to disc.</oldsource>
        <translation>Impossibile scrivere la nuova versione su disco.</translation>
    </message>
    <message>
        <location line="+17"/>
        <source>Mumble failed to retrieve version information from the central server.</source>
        <translation>Mumble non è riuscito a recuperare le informazioni sull&apos;ultima versione dal server centrale.</translation>
    </message>
</context>
<context>
    <name>ViewCert</name>
    <message>
        <location filename="ViewCert.cpp" line="+41"/>
        <source>Certificate Chain Details</source>
        <translation>Dettagli sulla Catena dei Certificati</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Certificate chain</source>
        <translation>Catena dei Certificati</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>%1 %2</source>
        <translation>%1 %2</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Certificate details</source>
        <translation>Dettagli dei Certificati</translation>
    </message>
    <message>
        <location line="+28"/>
        <location line="+28"/>
        <source>Common Name: %1</source>
        <translation>Nome Comune: %1</translation>
    </message>
    <message>
        <location line="-27"/>
        <location line="+28"/>
        <source>Organization: %1</source>
        <translation>Organizzazione: %1</translation>
    </message>
    <message>
        <location line="-27"/>
        <source>Subunit: %1</source>
        <translation>Sottounità: %1</translation>
    </message>
    <message>
        <location line="+1"/>
        <location line="+28"/>
        <source>Country: %1</source>
        <translation>Paese: %1</translation>
    </message>
    <message>
        <location line="-27"/>
        <location line="+28"/>
        <source>Locality: %1</source>
        <translation>Località: %1</translation>
    </message>
    <message>
        <location line="-27"/>
        <location line="+28"/>
        <source>State: %1</source>
        <translation>Stato: %1</translation>
    </message>
    <message>
        <location line="-27"/>
        <source>Valid from: %1</source>
        <translation>Valido da: %1</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Valid to: %1</source>
        <translation>Valido fino a: %1</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Serial: %1</source>
        <translation>Seriale: %1</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Public Key: %1 bits %2</source>
        <translation>Chiave Pubblica: %1 bits %2</translation>
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
        <translation>Firma (MD5): %1</translation>
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
        <translation>Inviato da:</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Unit Name: %1</source>
        <translation>Nome Unità: %1</translation>
    </message>
</context>
<context>
    <name>VoiceRecorder</name>
    <message>
        <location filename="VoiceRecorder.cpp" line="+184"/>
        <source>Invalid sample rate given to recorder</source>
        <translation>Al registratore è stata data una frequenza di campionamento non valida</translation>
    </message>
    <message>
        <location line="+98"/>
        <source>Recorder failed to create directory &apos;%1&apos;</source>
        <translation>Il registratore non è riuscito a creare la cartella &apos;%1&apos;</translation>
    </message>
    <message>
        <location line="+14"/>
        <source>Recorder failed to open file &apos;%s&apos;</source>
        <translation>Il registratore non è riuscito ad aprire il file &apos;%s&apos;</translation>
    </message>
    <message>
        <location line="+109"/>
        <source>.wav - Uncompressed</source>
        <translation>.wav - non compresso</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>.ogg (Vorbis) - Compressed</source>
        <translation>.ogg (Vorbis) - Compresso</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>.au - Uncompressed</source>
        <translation>.au - Non compresso</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>.flac - Lossless compressed</source>
        <translation>.flac - Compresso senza perdite</translation>
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
        <translation>Registratore</translation>
    </message>
    <message>
        <location/>
        <source>Control</source>
        <translation>Controllo</translation>
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
        <translation>Modalità</translation>
    </message>
    <message>
        <location/>
        <source>Multichannel</source>
        <translation>Multicanale</translation>
    </message>
    <message>
        <location/>
        <source>Output</source>
        <translation>Output</translation>
    </message>
    <message>
        <location/>
        <source>Output format</source>
        <translation>Formato dell&apos;output</translation>
    </message>
    <message>
        <location/>
        <source>Target directory</source>
        <translation>Cartella di destinazione</translation>
    </message>
    <message>
        <location/>
        <source>Filename</source>
        <translation>Nome del file</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Browse...</source>
        <translation>&amp;Sfoglia...</translation>
    </message>
    <message>
        <location filename="VoiceRecorderDialog.cpp" line="-195"/>
        <source>Closing the recorder will stop your current recording. Do you really want to close the recorder?</source>
        <translation>Chiudere il registratore interromperà la registrazione attuale. Vuoi veramente chiuderlo?</translation>
    </message>
    <message>
        <location line="-31"/>
        <source>Valid variables are:</source>
        <translation>Variabili valide sono:</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Inserts the user&apos;s name</source>
        <translation>Inserisce il nome dell&apos;utente</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Inserts the current date</source>
        <translation>Inserisce la data attuale</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Inserts the current time</source>
        <translation>Inserisce l&apos;ora attuale</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Inserts the hostname</source>
        <translation>Inserisce il nome del server</translation>
    </message>
    <message>
        <location line="+60"/>
        <source>Unable to start recording. Not connected to a server.</source>
        <translation>Impossibile iniziare la registrazione. Non sei connesso ad un server.</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>The server you are currently connected to is version 1.2.2 or older. For privacy reasons, recording on servers of versions older than 1.2.3 is not possible.
Please contact your server administrator for further information.</source>
        <translation>Il server a cui sei attualmente connesso è versione 1.2.2 o minore. Per ragioni di privacy non è possibile registrare nei server con versione minore a 1.2.3.
Per favore contatta l&apos;amministratore del server per maggiori informazioni.</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>There is already a recorder active for this server.</source>
        <translation>C&apos;è già in corso una registrazione in questo server.</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Please select a recording format.</source>
        <translation>Seleziona un formato di registrazione.</translation>
    </message>
    <message>
        <location line="+95"/>
        <source>Select target directory</source>
        <translation>Seleziona la cartella di destinazione</translation>
    </message>
    <message>
        <location filename="VoiceRecorderDialog.ui"/>
        <source>Downmix</source>
        <translation>Downmix</translation>
    </message>
</context>
<context>
    <name>WASAPISystem</name>
    <message>
        <location filename="WASAPI.cpp" line="+180"/>
        <source>Default Device</source>
        <translation>Periferica predefinita</translation>
    </message>
</context>
</TS>
