<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.0" language="it_IT" sourcelanguage="en">
<context>
    <name>ACLEditor</name>
    <message>
        <location filename="ACLEditor.cpp" line="+47"/>
        <source>Mumble - Edit ACL for %1</source>
        <translation>Mumble - Modifica ACL per %1</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Deny</source>
        <translation>Nega</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Allow</source>
        <translation>Permetti</translation>
    </message>
    <message>
        <location line="+102"/>
        <source>Allow %1</source>
        <translation>Permetti %1</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Deny %1</source>
        <translation>Nega %1</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This grants the %1 privilege. If a privilege is both allowed and denied, it is denied.&lt;br /&gt;%2</source>
        <translation>Permette il privilegio %1. Se un privilegio è contemporaneamente permesso e negato, sarà negato.&lt;br /&gt;%2</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This revokes the %1 privilege. If a privilege is both allowed and denied, it is denied.&lt;br /&gt;%2</source>
        <translation>Revoca il privilegio %1. Se un privilegio è contemporaneamente permesso e negato, sarà negato.&lt;br /&gt;%2</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui"/>
        <source>Dialog</source>
        <translation>Dialogo</translation>
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
        <location/>
        <source>This is all the groups currently defined for the channel. To create a new group, just type in the name and press enter.</source>
        <translation>Questi sono tutti i gruppi attualmente definiti per un canale. Per creare un nuovo gruppo, scrivi il nome e premi invio.</translation>
    </message>
    <message>
        <location/>
        <source>Remove selected group</source>
        <translation>Cancella il gruppo selezionato</translation>
    </message>
    <message>
        <location/>
        <source>This removes the currently selected group. If the group was inherited, it will not be removed from the list, but all local information about the group will be cleared.</source>
        <translation>Cancella il gruppo selezionato. Se un gruppo era eraditario, non verrà rimosso dalla lista, ma tutte le informazioni locali riguardanti il gruppo saranno cancellate.</translation>
    </message>
    <message>
        <location/>
        <source>Remove</source>
        <translation>Rimuovi</translation>
    </message>
    <message>
        <location/>
        <source>Inherit group members from parent</source>
        <translation>Eredita i membri del gruppo dal genitore</translation>
    </message>
    <message>
        <location/>
        <source>This inherits all the members in the group from the parent, if the group is marked as &lt;i&gt;Inheritable&lt;/i&gt; in the parent channel.</source>
        <translation>Eredita tutti i membri di un gruppo dal padre, se un gruppo è marcato come &lt;i&gt;Ereditabile&lt;/i&gt;nel canale padre.</translation>
    </message>
    <message>
        <location/>
        <source>Inherit</source>
        <translation>Eredita</translation>
    </message>
    <message>
        <location/>
        <source>Make group inheritable to sub-channels</source>
        <translation>Rendi un gruppo ereditabile ai sotto-canali</translation>
    </message>
    <message>
        <location/>
        <source>This makes this group inheritable to sub-channels. If the group is non-inheritable, sub-channels are still free to create a new group with the same name.</source>
        <translation>Rende ereditabile un gruppo ai sotto-canali. Se un gruppo non è ereditabile, i sotto-canali saranno comunque liberi di creare un nuovo gruppo con lo stesso nome.</translation>
    </message>
    <message>
        <location/>
        <source>Inheritable</source>
        <translation>Ereditabile</translation>
    </message>
    <message>
        <location/>
        <source>Group was inherited from parent channel</source>
        <translation>Gruppo ereditato dal canale padre</translation>
    </message>
    <message>
        <location/>
        <source>This indicates that the group was inherited from the parent channel. You cannot edit this flag, it&apos;s just for information.</source>
        <translation>Indica che un gruppo è stato ereditato dal canale padre. Non puoi modificare questa impostazione, è solo informativa.</translation>
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
        <location/>
        <source>Add to Remove</source>
        <translation>Aggiungi a Rimuovi</translation>
    </message>
    <message>
        <location/>
        <source>Add member to group</source>
        <translation>Aggiungi membro al gruppo</translation>
    </message>
    <message>
        <location/>
        <source>Type in the name of a player you wish to add to the group and click Add.</source>
        <translation>Inserisci il nome del giocatore che vuoi aggiungere al gruppo e premi Aggiungi.</translation>
    </message>
    <message>
        <location/>
        <source>Remove member from group</source>
        <translation>Cancella un membro dal gruppo</translation>
    </message>
    <message>
        <location/>
        <source>Type in the name of a player you wish to remove from the group and click Add.</source>
        <translation>Inserisci il nome del giocatore che vuoi cancellare dal gruppo e premi Add.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;ACL</source>
        <translation>&amp;ACL</translation>
    </message>
    <message>
        <location/>
        <source>Active ACLs</source>
        <translation>Attiva gli ACL</translation>
    </message>
    <message>
        <location/>
        <source>List of entries</source>
        <translation>Elenco di voci</translation>
    </message>
    <message>
        <location/>
        <source>This shows all the entries active on this channel. Entries inherited from parent channels will be show in italics.&lt;br /&gt;ACLs are evaluated top to bottom, meaning priority increases as you move down the list.</source>
        <translation>Mostra tutte le voci attive su questo canale. Le voci ereditate dal canale padre verranno visualizzate in corsivo.&lt;br /&gt;Le ACL sono valutate dall&apos;alto al basso, cioè la priorità aumenta mentre ci si sposta verso il basso nell&apos;elenco.</translation>
    </message>
    <message>
        <location/>
        <source>Inherit ACL of parent?</source>
        <translation>Eredita ACL dal padre?</translation>
    </message>
    <message>
        <location/>
        <source>This sets whether or not the ACL up the chain of parent channels are applied to this object. Only those entries that are marked in the parent as &quot;Apply to sub-channels&quot; will be inherited.</source>
        <translation>Imposta se le ACL provenienti dalla catena parentale dei canali sono applicate a questo oggetto. Solo quelle voci che sono contrassegnate nel padre come &quot;Applica ai sotto-canali&quot; saranno ereditate.</translation>
    </message>
    <message>
        <location/>
        <source>Inherit ACLs</source>
        <translation>Eredita le ACL</translation>
    </message>
    <message>
        <location/>
        <source>Move entry up</source>
        <translation>Sposta voce in su</translation>
    </message>
    <message>
        <location/>
        <source>This moves the entry up in the list. As entries are evaluated in order, this may change the effective permissions of users. You cannot move an entry above an inherited entry, if you really need that you&apos;ll have to duplicate the inherited entry.</source>
        <translation>Sposta in su la voce nella lista. Dato che le voci vengono valutate in ordine, questo potrebbe cambiare le autorizzazioni attive per gli utenti. Non è possibile spostare una voce al di sopra di un ingresso ereditato, se proprio avete bisogno di farlo potete duplicare la voce ereditata.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Up</source>
        <translation>&amp;Su</translation>
    </message>
    <message>
        <location/>
        <source>Move entry down</source>
        <translation>Sposta la voce in giù</translation>
    </message>
    <message>
        <location/>
        <source>This moves the entry down in the list. As entries are evaluated in order, this may change the effective permissions of users.</source>
        <translation>Sposta in giù la voce nella lista. Dato che le voci vengono valutate in ordine, questo potrebbe cambiare le autorizzazioni attive per gli utenti.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Down</source>
        <translation>&amp;Giù</translation>
    </message>
    <message>
        <location/>
        <source>Add new entry</source>
        <translation>Aggiungi una voce</translation>
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
        <translation>Cancella voce</translation>
    </message>
    <message>
        <location/>
        <source>This removes the currently selected entry.</source>
        <translation>Cancella la voce selezionata.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Remove</source>
        <translation>&amp;Cancella</translation>
    </message>
    <message>
        <location/>
        <source>Context</source>
        <translation>Contestuale</translation>
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
        <translation>La voce dovrebbe applicarsi ai sotto-canali.</translation>
    </message>
    <message>
        <location/>
        <source>This makes the entry apply to sub-channels of this channel.</source>
        <translation>Applica la voce ai sotto-canali di questo canale.</translation>
    </message>
    <message>
        <location/>
        <source>Applies to sub-channels</source>
        <translation>Applica ai sotto-canali</translation>
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
        <translation>Gruppo di questa voce si applica a</translation>
    </message>
    <message>
        <location/>
        <source>This controls which group of users this entry applies to.&lt;br /&gt;Note that the group is evaluated in the context of the channel the entry is used in. For example, the default ACL on the Root channel gives &lt;i&gt;Write&lt;/i&gt; permission to the &lt;i&gt;admin&lt;/i&gt; group. This entry, if inherited by a channel, will give a user write privileges if he belongs to the &lt;i&gt;admin&lt;/i&gt; group in that channel, even if he doesn&apos;t belong to the &lt;i&gt;admin&lt;/i&gt; group in the channel where the ACL originated.&lt;br /&gt;If a group name starts with a !, its membership is negated, and if it starts with a ~, it is evaluated in the channel the ACL was defined in, rather than the channel the ACL is active in. Order is important; &lt;i&gt;!~in&lt;/i&gt; is valid, but &lt;i&gt;~!in&lt;/i&gt; is not.&lt;br /&gt;A few special predefined groups are:&lt;br /&gt;&lt;b&gt;all&lt;/b&gt; - Everyone will match.&lt;br /&gt;&lt;b&gt;auth&lt;/b&gt; - All authenticated users will match.&lt;br /&gt;&lt;b&gt;sub,a,b,c&lt;/b&gt; - User currently in a sub-channel minimum &lt;i&gt;a&lt;/i&gt; common parents, and between &lt;i&gt;b&lt;/i&gt; and &lt;i&gt;c&lt;/i&gt; channels down the chain. See the website for more extensive documentation on this one.&lt;br /&gt;&lt;b&gt;in&lt;/b&gt; - Users currently in the channel will match (convenience for &lt;i&gt;sub,0,0,0&lt;/i&gt;).&lt;br /&gt;&lt;b&gt;out&lt;/b&gt; - Users outside the channel will match (convenience for &lt;i&gt;!sub,0,0,0&lt;/i&gt;).&lt;br /&gt;Note that an entry applies to either a user or a group, not both.</source>
        <translation>Controlla a quale gruppo di utenti si applica questa voce.&lt;br /&gt;Si noti che il gruppo è valutato nel contesto del canale in cui la voce è inserita. Ad esempio, il valore predefinito ACL sul canale Root dà il permesso &lt;i&gt;Scrivi&lt;/i&gt; al gruppo &lt;i&gt;admin&lt;/i&gt;. Questa voce, se ereditata da un canale, darà ad un utente i privilegi di scrittura se egli appartiene al gruppo &lt;i&gt;admin&lt;/i&gt; in quel canale, anche se non apparteneva al gruppo &lt;i&gt;admin&lt;/i&gt; nel canale in cui è nato l&apos;ACL.&lt;br /&gt;Se il nome di un gruppo inizia con un !, è negata ai suoi membri, e se inizia con una ~, è valutata nel canale in cui l&apos;ACL è stata definita, al posto che nel canale in cui è stata attivata l&apos;ACL. L&apos;ordine è importante; &lt;i&gt;!~in&lt;/i&gt; è valido, ma &lt;i&gt;~!in&lt;/i&gt; non lo è.&lt;br /&gt;Pochi particolari gruppi predefiniti sono i seguenti:&lt;br /&gt;&lt;b&gt;all&lt;/b&gt; - Corrisponderà a tutti.&lt;b&gt;auth&lt;/b&gt; - Corrisponderanno tutti gli utenti autenticati.&lt;br /&gt;&lt;b&gt;sub,a,b,c&lt;/b&gt; - Utenti in a sotto-canali minimo &lt;i&gt;a&lt;/i&gt; genitori comuni, e tra &lt;i&gt;b&lt;/i&gt; e &lt;i&gt;c&lt;/i&gt; canali verso il basso nella catena. Vedere il sito web per una documentazione più estesa su questo punto.&lt;br /&gt;&lt;b&gt;in&lt;/b&gt; - Utenti attualmente nel canale (scorciatoia di &lt;i&gt;sub,0,0,0&lt;/i&gt;).&lt;br /&gt;&lt;b&gt;out&lt;/b&gt; - Gli utenti al di fuori del canale (scorciatoia di &lt;i&gt;!sub,0,0,0&lt;/i&gt;).&lt;br /&gt;Si noti che una voce si applica ad un utente o ad un gruppo, non a entrambi.</translation>
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
        <source>Entry should apply to this channel.</source>
        <translation type="unfinished"></translation>
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
        <translation>Fallita l&apos;apertura dell&apos;Input ALSA selezionato: %1</translation>
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
        <translation>Fallita l&apos;apertura dell&apos;Output ALSA selezionato: %1</translation>
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
        <translation>Inizializzazione di ASIO fallita: %1</translation>
    </message>
    <message>
        <location line="-21"/>
        <location line="+25"/>
        <source>Failed to instantiate ASIO driver</source>
        <translation>Instanziazione del driver ASIO fallita</translation>
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
        <translation>Interroga il dispositivo selezionato per i canali. Molti driver ASIO sono estremamente bacati, e interrogandoli potrebbero causare un crash dell&apos;applicazione o del sistema.</translation>
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
        <translation>Interroga il dispositivo selezionato. Molti driver ASIO sono estremamente bacati, e interrogandoli potrebbero causare un crash dell&apos;applicazione o del sistema.</translation>
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
        <translation>Altoparlanti</translation>
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
        <translation>Devi selezionare almeno un microfono e un altoparlante per usare ASIO. Se hai bisogno solo il campionamento microfonico, usa DirectSound.</translation>
    </message>
    <message>
        <location line="+103"/>
        <source>Opening selected ASIO device failed. No input will be done.</source>
        <translation>Apertura del dispositivo ASIO fallito. Non ci sarà nessun ingresso.</translation>
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
        <translation>&lt;h3&gt;Mumble (%1)&lt;/h3&gt;&lt;p&gt;Copyright %3 Thorvald Natvig&lt;br /&gt;slicer@users.sourceforge.net&lt;/p&gt;&lt;p&gt;&lt;b&gt;Una utility per la chat vocale per giocatori&lt;/b&gt;&lt;/p&gt;&lt;p&gt;&lt;tt&gt;&lt;a href=&quot;%2&quot;&gt;%2&lt;/a&gt;&lt;/tt&gt;&lt;/p&gt;</translation>
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
        <location line="+11"/>
        <source>About Speex</source>
        <translation>Informazioni su Speex</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>&lt;h3&gt;About Speex&lt;/h3&gt;&lt;p&gt;&lt;tt&gt;&lt;a href=&quot;%2&quot;&gt;%2&lt;/a&gt;&lt;/tt&gt;&lt;/p&gt;&lt;p&gt;This program uses Speex version %1&lt;/p&gt;&lt;p&gt;Speex is used for echo cancellation, noise&lt;br /&gt;filtering, voice activity detection and speech&lt;br /&gt;compression.&lt;/p&gt;</source>
        <translation>&lt;h3&gt;Informazioni su Speex&lt;/h3&gt;&lt;p&gt;&lt;tt&gt;&lt;a href=&quot;%2&quot;&gt;%2&lt;/a&gt;&lt;/tt&gt;&lt;/p&gt;&lt;p&gt;Questo programma usa Speex versione %1&lt;/p&gt;&lt;p&gt;Speex è usato per la cancellazione dell&apos;eco, il filtraggio&lt;br /&gt;del rumore, il rilevamento del parlato e per la compressione&lt;br /&gt;della voce.&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+9"/>
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
        <translation>Metodo di input per l&apos;audio</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is the input method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation>&lt;b&gt;Metodo di ingrsso da utilizzare per l&apos;audio.&lt;/b&gt;&lt;br /&gt;Molto probabilmente vorrai usare DirectSound.</translation>
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
        <location/>
        <source>Cancel Echo</source>
        <translation>Rimuovi l&apos;Eco</translation>
    </message>
    <message>
        <location/>
        <source>Transmission</source>
        <translation>Trasmissione</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Transmit</source>
        <translation>&amp;Trasmetti</translation>
    </message>
    <message>
        <location/>
        <source>When to transmit your speech</source>
        <translation>Quando trasmettere la tua voce</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets when speech should be transmitted.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;Continuous&lt;/i&gt; - All the time&lt;br /&gt;&lt;i&gt;Voice Activity&lt;/i&gt; - When you are speaking clearly.&lt;br /&gt;&lt;i&gt;Push To Talk&lt;/i&gt; - When you hold down the hotkey set under &lt;i&gt;Shortcuts&lt;/i&gt;.</source>
        <translation>Imposta quando la voce dovrebbe essere trasmessa.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;Continuamente&lt;/i&gt;- Tutto il tempo&lt;br /&gt;&lt;i&gt;Attivazione vocale&lt;/i&gt;- Quando parli chiaramente.&lt;br /&gt;&lt;i&gt;Premi-Per-Parlare&lt;/i&gt;- Quando premi il pulsante sotto &lt;i&gt;Tasti di scelta rapida&lt;/i&gt;.</translation>
    </message>
    <message>
        <location/>
        <source>DoublePush Time</source>
        <translation>Tempo di DoppiaPremuta</translation>
    </message>
    <message>
        <location/>
        <source>TextLabel</source>
        <translation>Etichetta testo</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This enables the audio cues for push to talk.&lt;/b&gt;&lt;br /&gt;Setting this will give you a short audio beep when push to talk is pressed and released.</source>
        <translation>&lt;b&gt;Abilita l&apos;imbeccata audio per il push to talk.&lt;/b&gt;&lt;br /&gt;Attivandolo sentirete un breve segnale acustico quando il push-to-talk viene premuto e rilasciato.</translation>
    </message>
    <message>
        <location/>
        <source>PTT Audio cue</source>
        <translation>PTT Imbeccata Audio</translation>
    </message>
    <message>
        <location/>
        <source>Use SNR based speech detection</source>
        <translation>Usa il riconoscimento della voce basato su SNR</translation>
    </message>
    <message>
        <location/>
        <source>Signal to Noise</source>
        <translation>Segnale Rumore</translation>
    </message>
    <message>
        <location/>
        <source>Use Amplitude based speech detection.</source>
        <translation>Usa riconoscimento della voce basato sull&apos;Ampiezza.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets speech detection to use Amplitude.&lt;/b&gt;&lt;br /&gt;In this mode, the raw strength of the input signal is used to detect speech.</source>
        <translation>&lt;b&gt; Imposta di usare l&apos;ampiezza per il rilevamento della voce.&lt;/b&gt;&lt;br /&gt;In questa modalità, la forza grezza del segnale d&apos;ingresso viene utilizzata per rilevara la voce.</translation>
    </message>
    <message>
        <location/>
        <source>Amplitude</source>
        <translation>Ampiezza</translation>
    </message>
    <message>
        <location/>
        <source>Voice &amp;Hold</source>
        <translation>&amp;Silenzio dopo</translation>
    </message>
    <message>
        <location/>
        <source>How long to keep transmitting after silence</source>
        <translation>Per quanto tempo continua a trasmettere dopo l&apos;inizio del silenzio</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This selects how long after a perceived stop in speech transmission should continue.&lt;/b&gt;&lt;br /&gt;Set this higher if your voice breaks up when you speak (seen by a rapidly blinking voice icon next to your name).&lt;br /&gt;Only makes sense when used with Voice Activity transmission.</source>
        <translation>&lt;b&gt;Imposta per quanto tempo dopo un arresto nel discorso la trasmissione deve continuare.&lt;/b&gt;&lt;br /&gt;Alzare se la tua voce viene tagliata quando si parla (si vede un lampeggio rapido dell&apos;icona voce accanto al tuo nome).&lt;br /&gt;Ha senso solo quando viene utilizzato con la trasmissione ad Attività Vocale.</translation>
    </message>
    <message>
        <location/>
        <source>Silence Below</source>
        <translation>Silenzio Sotto</translation>
    </message>
    <message>
        <location/>
        <source>Signal values below this counts as silence</source>
        <translation>Valore del Segnale sotto il quale conta come silenzio</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the trigger values for voice detection.&lt;/b&gt;&lt;br /&gt;Use this together with the Audio Statistics window to manually tune the trigger values for detecting speech. Input values below &quot;Silence Below&quot; always count as silence. Values above &quot;Speech Above&quot; always count as voice. Values in between will count as voice if you&apos;re already talking, but will not trigger a new detection.</source>
        <translation>&lt;b&gt;Questo cursore imposta il valore di soglia per il rilevamento del discorso.&lt;/b&gt;&lt;br /&gt;Usa questo insieme alla finestra delle statistiche audio per sintonizzare manualmente il cursore per i valori di soglia per l&apos;individuazione del discorso. un ingresso al di sotto dei valori &quot;Silenzio sotto&quot; vieni sempre considerato come silenzio. Un ingresso al di sopra di &quot;Voce sopra&quot; viene considerato come voce. Un segnale compreso tra i due cursori conterà come voce se si stava parlando, ma non farà scattare di nuovo la voce.</translation>
    </message>
    <message>
        <location/>
        <source>Speech Above</source>
        <translation>Voce Sopra</translation>
    </message>
    <message>
        <location/>
        <source>Signal values above this count as voice</source>
        <translation>Valore del Segnale sopra il quale conta come voce</translation>
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
        <translation>&lt;b&gt;Questo seleziona quante parti dell&apos;audio dovrebbero essere messe in un pacchetto&lt;/b&gt;&lt;br /&gt;Incrementando questo sarà incrementata la latenza della tua voce, ma ridurrà anche la banda richiesta.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This shows peak outgoing bandwidth used.&lt;/b&gt;&lt;br /&gt;This shows the peak amount of bandwidth sent out from your machine. Audio bitrate is the maximum bitrate (as we use VBR) for the audio data alone. Position is the bitrate used for positional information. Overhead is our framing and the IP packet headers (IP and UDP is 75% of this overhead).</source>
        <translation>&lt;b&gt; Questo mostra il picco della larghezza di banda utilizzata in uscita.&lt;/b&gt;&lt;br /&gt; Questo mostra la quantità del picco della larghezza di banda inviata dal tuo computer. Bitrate audio è il massimo bitrate (come si usa VBR) per i dati audio da soli. Posizione è il bitrate utilizzato per le informazioni sulla posizione. Carico è l&apos;inquadratura e l&apos;header del pacchetto IP (IP e UDP è il 75% del carico).</translation>
    </message>
    <message>
        <location/>
        <source>Audio Processing</source>
        <translation>Elaborazione Audio</translation>
    </message>
    <message>
        <location/>
        <source>Noise Suppression</source>
        <translation>Soppressione Rumore</translation>
    </message>
    <message>
        <location/>
        <source>Noise suppression</source>
        <translation>Soppressione rumore</translation>
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
        <translation>&lt;b&gt;Massima amplificazione di ingresso &lt;/b&gt;. &lt;br /&gt; Mumble normalizza il volume in ingresso prima della compressione, e questo imposta quanto è consentito amplificare. &lt;br /&gt; Il livello attuale è continuamente aggiornato in base al suo attuale modello di intervento , ma non potrà mai andare al di sopra del livello indicato qui. &lt;br /&gt; Se le statistiche dell livello del &lt;i&gt;Volume del microfono&lt;/i&gt; gravitano attorno al 100%, probabilmente si desidera impostare questo al 2,0 o più, ma se , come la maggior parte delle persone, non si riesce a raggiungere il 100%, impostalo molto al di sopra. &lt;br /&gt; Idealmente, impostare in modo che &lt;i&gt; Volume del microfono * Fattore di amplificazione&gt; = 100 &lt;/i&gt;, anche quando stai parlando veramente piano. &lt;br /&gt;&lt;br /&gt; Si noti che non vi è nulla di male in questa impostazione di massima, ma Mumble inizierà raccogliendo le altre conversazioni se lasciarlo per l&apos;auto-tune a quel livello.</translation>
    </message>
    <message>
        <location/>
        <source>Current speech detection chance</source>
        <translation>Probabilità corrente di riconoscimento della voce</translation>
    </message>
    <message>
        <location/>
        <source>Audible audio cue when push-to-talk is activated or deactivated</source>
        <translation>Barra dell&apos;audio udibile quando premi per parlare è attivato o disattivato</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets speech detection to use Signal to Noise ratio.&lt;/b&gt;&lt;br /&gt;In this mode, the input is analyzed for something resembling a clear signal, and the clarity of that signal is used to trigger speech detection.</source>
        <translation>&lt;b&gt;Questo imposta l&apos;individuazione del discorso con il rapporto segnale/rumore. &lt;/b&gt; &lt;br /&gt; In questa modalità, l&apos;ingresso è analizzato per qualcosa di simile a un segnale chiaro, e la chiarezza del segnale viene utilizzata per attivare il rilevamento del discorso.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This shows the current speech detection settings.&lt;/b&gt;&lt;br /&gt;You can change the settings from the Settings dialog or from the Audio Wizard.</source>
        <translation>&lt;b&gt;Questo visualizza le impostazioni correnti per il riconoscimento del parlato.&lt;/b&gt;&lt;br /&gt;Puoi cambiare le impostazioni dalla finestra delle impostazioni o dall&apos;Audio Wizard.</translation>
    </message>
    <message>
        <location/>
        <source>Idle AutoMute</source>
        <translation>AutoMuto quando Inattivo</translation>
    </message>
    <message>
        <location/>
        <source>How long to remain idle before auto-mute.</source>
        <translation>Per quanto tempo devi rimanere inattivo prima dell&apos;auto-muto.</translation>
    </message>
    <message>
        <location/>
        <source>This sets an idle timer. If the timer expires without any audio being sent to the server, you are muted.</source>
        <translation>Stabilisce un timer di inattività. Se il timer scade senza che nessun audio venga inviato al server, verrai disattivato.</translation>
    </message>
    <message>
        <location/>
        <source>Maximum bandwidth used for sending audio</source>
        <translation>Banda massima usata per inviare l&apos;audio</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the amount of noise suppression to apply.&lt;/b&gt;&lt;br /&gt;The higher this value, the more aggressively stationary noise will be supressed.</source>
        <translation>&lt;b&gt;Questo imposta la quantità di rumore da eliminare.&lt;b&gt;&lt;br /&gt;Più alto sarà questo valore e più rumore sarà eliminato.</translation>
    </message>
    <message>
        <location/>
        <source>Input device for audio</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is the input device to use for audio.&lt;/b&gt;</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>AudioInputDialog</name>
    <message>
        <location filename="AudioConfigDialog.cpp" line="+74"/>
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
        <translation>Premi per parlare</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>Audio Input</source>
        <translation>Ingresso audio</translation>
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
        <translation>Spento</translation>
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
        <translation>%1kbit/s (Audio %2, Posizione %4, Sovraccarico %3)</translation>
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
        <translation>&lt;b&gt;Questo è il sistema d&apos;uscita da utilizzare per l&apos;audio.&lt;/b&gt;&lt;br /&gt;Molto probabilmente vorrete utilizzare DirectSound.</translation>
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
        <translation>Default &amp;Jitter Buffer</translation>
    </message>
    <message>
        <location/>
        <source>Safety margin for jitter buffer</source>
        <translation>Margine di sicurezza per il jitter buffer</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the minimum safety margin for the jitter buffer.&lt;/b&gt;&lt;br /&gt;All incoming audio is buffered, and the jitter buffer continually tries to push the buffer to the minimum sustainable by your network, so latency can be as low as possible. This sets the minimum buffer size to use. If the start of sentences you hear is very jittery, increase this value.</source>
        <translation>&lt;b&gt;Imposta il minimo margine di sicurezza per il jitter buffer.&lt;/b&gt;&lt;br /&gt;Tutto l&apos;audio in ingresso è bufferizzato, e il jitter buffer cerca continuamente di tenere il buffer al minimo sostenibile dalla tua rete, in modo da avere una latenza quanto più bassa possibile. Imposta la dimensione minima del buffer da utilizzare. Se l&apos;inizio delle frasi è molto jitterato, aumentare questo valore.</translation>
    </message>
    <message>
        <location/>
        <source>TextLabel</source>
        <translation>Etichetta testo</translation>
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
        <translation>Quantità di dati da bufferizzare</translation>
    </message>
    <message>
        <location/>
        <source>This sets the amount of data to prebuffer in the output buffer. Experiment with different values and set it to the lowest which doesn&apos;t cause rapid jitter in the sound.</source>
        <translation>Imposta la quantità di dati da prebufferizzare nel buffer d&apos;uscita. Sperimentare diversi valori e tenere al minimo che non causa un jitter veloce nel suono.</translation>
    </message>
    <message>
        <location/>
        <source>The connected &quot;speakers&quot; are actually headphones.</source>
        <translation>Gli altoparlanti attualmente connessi sono delle cuffie.</translation>
    </message>
    <message>
        <location/>
        <source>This sets the minimum distance for sound calculations. The volume of other players&apos; speech will not decrease until they are at least this far away from you.</source>
        <translation>Imposta la distanza minima per i calcoli sonori. Il volume della voce degli altri giocatori non diminuirà fino a che non saranno almeno così lontani da voi.</translation>
    </message>
    <message>
        <location/>
        <source>Factor for sound volume decrease</source>
        <translation>Fattore di decremento per il volume dei suoni</translation>
    </message>
    <message>
        <location/>
        <source>Bloom</source>
        <translation>Fattore incremento</translation>
    </message>
    <message>
        <location/>
        <source>Factor for sound volume increase</source>
        <translation>Fattore di incremento del volume sonoro</translation>
    </message>
    <message>
        <location/>
        <source>How much should sound volume increase for sources that are really close?</source>
        <translation>Quanto dovrebbe aumentare il volume del suono per le sorgenti che sono veramente vicine?</translation>
    </message>
    <message>
        <location/>
        <source>Checking this indicates that you don&apos;t have speakers connected, just headphones. This is important, as speakers are usually in front of you, while headphones are directly to your left/right.</source>
        <translation>Se impostato indica che non sono collegati degli altoparlanti, ma solo le cuffie. Questo è importante, in quanto gli altoparlanti solitamente sono di fronte a voi, mentre le cuffie sono direttamente a sinistra/destra.</translation>
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
        <location/>
        <source>Minimum distance to player before sound volume decreases</source>
        <translation>Distanza minima dal giocatore prima che il volume del suono decresca</translation>
    </message>
    <message>
        <location/>
        <source>Maximum Distance</source>
        <translation>Distanza massima</translation>
    </message>
    <message>
        <location/>
        <source>Maximum distance, beyond which speech volume won&apos;t decrease</source>
        <translation>Massima distanza, dopo la quale il volume del discorso non decrescerà</translation>
    </message>
    <message>
        <location/>
        <source>This sets the maximum distance for sound calculations. When farther away than this, other players&apos; speech volume will not decrease any further.</source>
        <translation>Questo imposta la massima distanza per il calcolo del suono. Quando saranno più lontano di così il volume dei discorsi degli altri giocatori non decrescerà più.</translation>
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
        <source>&lt;b&gt;This sets the packet latency variance for loopback testing.&lt;/b&gt;&lt;br /&gt;Most audio paths contain some variable latency. This allows you set that variance for loopback mode testing. For example, if you set this to 15ms, this will emulate a network with 20-35ms ping latency or one with 80-95ms latency. Most domestic net connections have a variance of about 5ms</source>
        <oldsource>&lt;b&gt;This sets the packet latency variance for loopback testing&lt;/b&gt;&lt;br /&gt;Most audio paths contain some variable latency. This allows you set that variance for loopback mode testing. For example, if you set this to 15ms, this will emulate a network with 20-35ms ping latency or one with 80-95ms latency. Most domestic net connections have a variance of about 5ms</oldsource>
        <translation>&lt;b&gt;Questo imposta la latenza del pacchetto per il test di riciclico locale. &lt;/b&gt;&lt;br /&gt; La maggior parte dell&apos;audio contiene alcuni percorsi di latenza variabile. Questo permette di impostare le variabili per la modalità di test di riciclo. Ad esempio, se si imposta questo a 15ms, questo emula una rete con 20-35ms di latenza oppure con 80-95ms di latenza. La maggior parte delle connessioni di rete nazionale ha ancora una variazione di circa 5ms</translation>
    </message>
    <message>
        <location/>
        <source>Packet Loss</source>
        <translation>Pacchetti Persi</translation>
    </message>
    <message>
        <location/>
        <source>Packet loss for loopback mode</source>
        <translation>Pacchetti persi in modo ricircolo</translation>
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
        <translation>Richiesto modo ricircolo</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This enables one of the loopback testmodes.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;None&lt;/i&gt; - Loopback disabled&lt;br /&gt;&lt;i&gt;Local&lt;/i&gt; - Emulate a local server.&lt;br /&gt;&lt;i&gt;Server&lt;/i&gt; - Request loopback from server.&lt;br /&gt;Please note than when loopback is enabled, no other players will hear your voice. This setting is not saved on application exit.</source>
        <translation>&lt;b&gt;Consente uno dei modi di test di ricircolo.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;Nessuno&lt;/i&gt; - Ricircolo disattivato&lt;br /&gt;&lt;i&gt;Locale&lt;/i&gt; - Emulare un server locale.&lt;br /&gt;&lt;i&gt;Server&lt;/i&gt; - Richiesta di ricircolo al server.&lt;br /&gt;Si prega di notare che quando il ricircolo è abilitato, gli altri giocatori si sentiranno la tua voce. Questa impostazione non viene salvata all&apos;uscita dall&apos;applicazione.</translation>
    </message>
    <message>
        <location/>
        <source>Other Applications</source>
        <translation>Altre applicazioni</translation>
    </message>
    <message>
        <location/>
        <source>Volume of other applications during speech</source>
        <translation>Volume delle altre applicazioni durante il parlato</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Decrease volume of other applications during speech.&lt;/b&gt;&lt;br /&gt;Mumble supports decreasing the volume of other applications during incoming speech. This sets the relative volume of other applications when others are talking.</source>
        <translation>&lt;b&gt;Diminuire il volume delle altre applicazioni durante il paralto.&lt;/b&gt;&lt;br /&gt;Mumble supporta la diminuzione dell volume delle altre applicazioni durante la ricezione della voce. Imposta il volume relativo delle altre applicazioni quando stanno parlando gli altri.</translation>
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
</context>
<context>
    <name>AudioOutputDialog</name>
    <message>
        <location filename="AudioConfigDialog.cpp" line="+119"/>
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
</context>
<context>
    <name>AudioStats</name>
    <message>
        <source>&gt;1000ms</source>
        <translation type="obsolete">&gt;1000ms</translation>
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
        <translation>Picco di potenza nell&apos;ultima trama</translation>
    </message>
    <message>
        <location/>
        <source>This shows the peak power in the last frame (20 ms), and is the same measurement as you would usually find displayed as &quot;input power&quot;. Please disregard this and look at &lt;b&gt;Microphone power&lt;/b&gt; instead, which is much more steady and disregards outliers.</source>
        <translation>Mostra la potenza di picco nell&apos;ultima trama (20 ms), ed è la stessa misura che si trova solitamente visualizzata come &quot;potenza in ingresso&quot;. Si prega di ignorare questo aspetto e invece badare a &lt;b&gt;Potenza del microfono&lt;/b&gt;, che è molto più costante e non tiene conto delle anomalie.</translation>
    </message>
    <message>
        <source>TextLabel</source>
        <translation type="obsolete">Etichetta testo</translation>
    </message>
    <message>
        <location/>
        <source>Peak speaker level</source>
        <translation>Livello di picco degli altoparlanti</translation>
    </message>
    <message>
        <location/>
        <source>This shows the peak power of the speakers in the last frame (20 ms). Unless you are using a multi-channel sampling method (such as ASIO) with speaker channels configured, this will be 0. If you have such a setup configured, and this still shows 0 while you&apos;re playing audio from other programs, your setup is not working.</source>
        <oldsource>This shows the peak power in the last frame (20 ms) of the speakers. Unless you are using a multi-channel sampling method (such as ASIO) with speaker channels configured, this will be 0. If you have such a setup configured, and this still shows 0 while you&apos;re playing audio from other programs, your setup is not working.</oldsource>
        <translation>Questo mostra la potenza di picco degli altoparlanti nel corso degli ultimi frame (20 ms). A meno che non si sta utilizzando un metodo di campionamento multi-canale (come ASIO), questo sarà 0. Se avete configurato una tale impostazione, e questo mostra ancora 0 mentre si sta riproducendo l&apos;audio da altri programmi, la configurazione non funziona.</translation>
    </message>
    <message>
        <location/>
        <source>Peak clean level</source>
        <translation>Livello di picco pulito</translation>
    </message>
    <message>
        <source>This shows the peak power in the last frame (20 ms) after all processing. Ideally, this should be -96 dB when you&apos;re not talking. In reality, a sound studio should see -60 dB, and you should hopefully see somewhere around -20 dB. When you are talking, this should rise to somewhere between -5 and -10 dB.&lt;br /&gt;If you are using echo cancellation, and this rises to more than -15 dB when you&apos;re not talking, your setup is not working, and you&apos;ll annoy other players with echoes.</source>
        <translation type="obsolete">Mostra la potenza di picco nell&apos;ultima trama (20 ms) dopo tutti i processamenti. Idealmente, questa dovrebbe essere -96 dB quando non stai parlando. In realtà, un studio audio dovrebbe vedere -60 dB, e si spera che tu veda qualcosa intorno a -20 dB. Mentre stai parlando, questo dovrebbe salire tra -5 e -10 dB.&lt;br /&gt;Se stai usando la cancellazione dell&apos;eco, e aumenta a più di -15 dB quando non stai parlando, l&apos;impostazione non funziona, e infastidirai gli altri giocatori con echi.</translation>
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
        <translation>Come scegliere il livello di ingresso attuale al valore ideale</translation>
    </message>
    <message>
        <location/>
        <source>This shows how close your current input volume is to the ideal. To adjust your microphone level, open whatever program you use to adjust the recording volume, and look at the value here while talking.&lt;br /&gt;&lt;b&gt;Talk loud, as you would when you&apos;re upset over getting fragged by a noob.&lt;/b&gt;&lt;br /&gt;Adjust the volume until this value is close to 100%, but make sure it doesn&apos;t go above. If it does go above, you are likely to get clipping in parts of your speech, which will degrade sound quality.</source>
        <translation>Mostra come tenere l&apos;ingresso corrente al volume ideale. Per regolare il livello del microfono, aprire il programma che usi per regolare il volume di registrazione, e guarda al valore mentre stai parlando.&lt;br /&gt;&lt;b&gt;Parla forte, come faresti quando sei spaventato.&lt;/b&gt;&lt;br /&gt;Regolare il volume fino a quando questo valore è vicino al 100%, ma assicurarsi che non va sopra. Se andasse sopra, probabilmente taglierai alcune parti del tuo parlato, che degraderà la qualità del suono.</translation>
    </message>
    <message>
        <location/>
        <source>Signal-To-Noise ratio</source>
        <translation>Rapporto Segnale Rumore</translation>
    </message>
    <message>
        <location/>
        <source>Signal-To-Noise ratio from the microphone</source>
        <translation>Rapporto Segnale Rumore dal microfono</translation>
    </message>
    <message>
        <location/>
        <source>This is the Signal-To-Noise Ratio (SNR) of the microphone in the last frame (20 ms). It shows how much clearer the voice is compared to the noise.&lt;br /&gt;If this value is below 1.0, there&apos;s more noise than voice in the signal, and so quality is reduced.&lt;br /&gt;There is no upper limit to this value, but don&apos;t expect to see much above 40-50 without a sound studio.</source>
        <translation>E&apos; il rapporto segnale-rumore (SNR) del microfono nell&apos;ultima trama (20 ms). Mostra quanto è più forte la voce in confrontato con il rumore.&lt;br /&gt;Se questo valore è inferiore a 1.0, c&apos;è più rumore della voce nel segnale, e quindi la qualità è bassa.&lt;br /&gt;Non vi è alcun limite al presente valore, ma non aspettatevi di vedere molto al di sopra 40-50 senza essere in uno studio audio.</translation>
    </message>
    <message>
        <location/>
        <source>Speech Probability</source>
        <translation>Probabilità di parlato</translation>
    </message>
    <message>
        <location/>
        <source>Probability of speech</source>
        <translation>Probabilità di parlato</translation>
    </message>
    <message>
        <location/>
        <source>This is the probability that the last frame (20 ms) was speech and not environment noise.&lt;br /&gt;Voice activity transmission depends on this being right. The trick with this is that the middle of a sentence is always detected as speech; the problem is the pauses between words and the start of speech. It&apos;s hard to distinguish a sigh from a word starting with &apos;h&apos;.&lt;br /&gt;If this is in bold font, it means Mumble is currently transmitting (if you&apos;re connected).</source>
        <translation>E&apos; la probabilità che l&apos;ultima trama (20 ms) sia del parlato e non rumore ambiente.&lt;br /&gt;L&apos;attivazione vocale dipende da quando è corretta questo valore. Il trucco è che la parte centrale di una frase è sempre individuato come discorso, il problema è nelle pause tra le parole e l&apos;inizio del discorso. E &apos;difficile distinguere un sospiro da una parola che comincia con la &apos;h&apos;.&lt;br /&gt;Se è in grassetto, significa che Mumble è attualmente in trasmissione (se connesso).</translation>
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
        <translation>Banda dell&apos;ultima trama</translation>
    </message>
    <message>
        <source>This is the audio bitrate of the last compressed frame (20 ms), and as such will jump up and down as the VBR adjusts the quality. To adjust the peak bitrate, adjust &lt;b&gt;Compression Complexity&lt;/b&gt; in the Settings dialog.</source>
        <translation type="obsolete">E&apos; il bitrate audio dell&apos;ultima trama compressa (20 ms), e come tale, salta su e giù mentre il VBR regola la qualità. Per regolare il bitrate di picco, regolare &lt;b&gt;Complessità della Compressione&lt;/b&gt; nella finestra di dialogo Impostazioni.</translation>
    </message>
    <message>
        <location/>
        <source>DoublePush interval</source>
        <translation>Intervallo di DoppiaPremuta</translation>
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
        <translation>Probabilità di riconoscimento del parlato attuale</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This shows the current speech detection settings.&lt;/b&gt;&lt;br /&gt;You can change the settings from the Settings dialog or from the Audio Wizard.</source>
        <translation>&lt;b&gt;Mostra l&apos;attuale intervento di rilevamento della voce.&lt;/b&gt;&lt;br /&gt;Puoi cambiare le impostazioni della finestra di dialogo Impostazioni o dalla Procedura guidata Audio.</translation>
    </message>
    <message>
        <location/>
        <source>Signal and noise power spectrum</source>
        <translation>Potenza spettrale del segnale più rumore</translation>
    </message>
    <message>
        <location/>
        <source>Power spectrum of input signal and noise estimate</source>
        <translation>Potenza spettrale del segnale più rumore di ingresso stimata</translation>
    </message>
    <message>
        <location/>
        <source>This shows the power spectrum of the current input signal (red line) and the current noise estimate (filled blue).&lt;br /&gt;All amplitudes are multiplied by 30 to show the interesting parts (how much more signal than noise is present in each waveband).&lt;br /&gt;This is probably only of interest if you&apos;re trying to fine-tune noise conditions on your microphone. Under good conditions, there should be just a tiny flutter of blue at the bottom. If the blue is more than halfway up on the graph, you have a seriously noisy environment.</source>
        <translation>Mostra la potenza dello spettro del segnale di ingresso corrente (linea rossa) e l&apos;attuale stima del rumore (area di colore blu).&lt;br /&gt;Tutte le ampiezze sono moltiplicate per 30 per mostrare le parti interessanti (quanto più il segnale rispetto al rumore è presente in ogni banda di frequenza).&lt;br /&gt;Probabilmente è di interesse solo se si sta cercando di mettere a punto il rumore del microfono. In buone condizioni, ci dovrebbe essere solo una piccola vibrazione di colore blu nella parte inferiore. Se il blu è più che a mezza costa sul grafico, ci si trova in un ambiente seriamente rumoroso.</translation>
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
        <translation>Mostra il peso del cancellatore di echo, con il tempo crescente verso il basso e la frequenza crescente a destra.&lt;br /&gt;In teoria, dovrebbe essere nero, cioè che non esiste eco. Più comunemente, otterrai una o più strisce orizzontali di colore bluastro che rappresentano l&apos;eco al tempo ritardato. Si dovrebbe poter vedere i pesi aggiornati in tempo reale.&lt;br /&gt;Notare che finché l&apos;echo è nullo, qui non sarà possibile visualizzare dati molto utili. Riproduci un po&apos; di musica e le cose dovrebbero stabilizzarsi.&lt;br /&gt;Puoi scegliere di visualizzare la parte reale o immaginaria dei pesi nel dominio della frequenza, o alternativamente il modulo e la fase calcolati. Il più utile di questi sarà probabilmente il modulo, che è l&apos;ampiezza dell&apos;eco, e mostra quanta parte del segnale in uscita è stato rimossa in quel momento temporale. Le altre modalità di visualizzazione sono per lo più utili per le persone che vogliono affinare gli algoritmi per la cancellazione dell&apos;eco.&lt;br /&gt;Notare: Se l&apos;intera immagine oscilla massicciamente mentre si è in modalità modulo, il cancellatore di echo non sta fallendo nel trovare una correlazione tra le due sorgenti di ingresso (altoparlanti e microfono). O hai un ritardo di eco veramente lungo, o una delle sorgenti di ingresso è configurata male.</translation>
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
        <translation>Wizard di sintonia dell&apos;audio</translation>
    </message>
    <message>
        <location line="+50"/>
        <source>Introduction</source>
        <translation>Introduzione</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Welcome to the Mumble Audio Wizard</source>
        <translation>Benvenuto all&apos;Audio Wizard di Mumble</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>This is the audio tuning wizard for Mumble. This will help you correctly set the input levels of your sound card, and also set the correct parameters for sound processing in Mumble.</source>
        <translation>Questo è il wizard di sintonia dell&apos;audio per Mumble, ti aiuterà ad impostare correttamente i livelli di input della tua scheda audio e imposterà anche i parametri corretti per il processo dei suoni su Mumble.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Please be aware that as long as this wizard is active, audio will be looped locally to allow you to listen to it, and no audio will be sent to the server.</source>
        <translation>Finche la procedura sarà attiva, l&apos;audio sarà riciclato localmente per darti la possibilità di risentirti e non sarà inviato al server.</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Finished</source>
        <translation>Finito</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Enjoy using Mumble</source>
        <translation>Divertiti con Mumble</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Congratulations. You should now be ready to enjoy a richer sound experience with Mumble.</source>
        <translation>Congratulazioni. Ora dovresti essere pronto a provare una ricca esperienza audio con Mumble.</translation>
    </message>
    <message>
        <location line="+23"/>
        <source>Device selection</source>
        <translation>Selezione dispositivo</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Selecting the input and output device to use with Mumble.</source>
        <translation>Seleziona i dispositivi di input e output da usare con Mumble.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Input device</source>
        <translation>Dispositivo di input</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>This is the device your microphone is connected to.</source>
        <translation>Questo è il dispositivo a cui è connesso il microfono.</translation>
    </message>
    <message>
        <location line="+14"/>
        <location line="+52"/>
        <source>System</source>
        <translation>Sistema</translation>
    </message>
    <message>
        <location line="-49"/>
        <source>Input method for audio</source>
        <translation>Metodo di input per l&apos;audio</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>&lt;b&gt;This is the input method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation>&lt;b&gt;Questo è il metodo dell&apos;input da usare per l&apos;audio.&lt;/b&gt;&lt;br /&gt;è probabile che si desideri utilizzare DirectSound.</translation>
    </message>
    <message>
        <location line="+12"/>
        <location line="+52"/>
        <source>Device</source>
        <translation>Dispositivo</translation>
    </message>
    <message>
        <location line="-49"/>
        <source>Input device to use</source>
        <translation>Dispositivo di input da usare</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>&lt;b&gt;Selects which sound card to use for audio input.&lt;/b&gt;</source>
        <translation>&lt;b&gt;Seleziona la scheda audio da usare come input audio.&lt;/b&gt;</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Use echo cancellation</source>
        <translation>Usa cancellazione eco</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Cancel echo from headset or speakers.</source>
        <translation>Cancella l&apos;eco delle cuffie o degli altoparlanti.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This enables echo cancellation of outgoing audio, which helps both on speakers and on headsets.</source>
        <translation>Questo abilita l&apos;eliminazione dell&apos;eco per l&apos;audio in uscita, che aiuta sia con gli altoparlanti che con le cuffie.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Output device</source>
        <translation>Dispositivo di output</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>This is the device your speakers or headphones are connected to.</source>
        <translation>Questo è il dispositivo a cui sono connessi gli altoparlanti o le cuffie.</translation>
    </message>
    <message>
        <location line="+19"/>
        <source>Output method for audio</source>
        <translation>Metodo di output per l&apos;audio</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>&lt;b&gt;This is the Output method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation>&lt;b&gt;Questo è il metodo dell&apos;output da usare per l&apos;audio.&lt;/b&gt;&lt;br /&gt;è probabile che si desideri utilizzare DirectSound.</translation>
    </message>
    <message>
        <location line="+15"/>
        <source>Output device to use</source>
        <translation>Dispositivo di output da usare</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>&lt;b&gt;Selects which sound card to use for audio Output.&lt;/b&gt;</source>
        <translation>&lt;b&gt;Seleziona la scheda audio da usare come output audio.&lt;/b&gt;</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Enable positional audio</source>
        <translation>Abilita audio posizionale</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Allows positioning of sound.</source>
        <translation>Consente il posizionamento del suono.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This allows Mumble to use positional audio to place voices.</source>
        <translation>Questo consente a Mumble di usare l&apos;audio posizionale per posizionare le voci.</translation>
    </message>
    <message>
        <location line="+17"/>
        <source>Positional Audio</source>
        <translation>Audio posizionale</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Adjusting attenuation of positional audio.</source>
        <translation>Aggiustamento dell&apos;attenuazione dell&apos;audio posizionale.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Mumble supports positional audio for some games, and will position the voice of other players relative to their position in game. Depending on their position, the volume of the voice will be changed between the speakers to simulate the direction and distance the other player is at. Such positioning depends on your speaker configuration being correct in your operating system, so a test is done here.</source>
        <translation>Mumble supporta l&apos;audio posizionale per alcuni giochi, e posizionerà la voce dell&apos;altro giocatore relativamente alla sua posizione nel gioco. Il volume sarà dipendente dalla sua posizione e sarà aumentato o diminuito su un altoparlante per simulare la direzione e la distanza dell&apos;altro giocatore. L&apos;audio posizionale dipenderà dai tuoi altoparlanti e dalla loro configurazione nel sistema operativo, il test è finito qui.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>The graph below shows the position of &lt;font color=&quot;red&quot;&gt;you&lt;/font&gt;, the &lt;font color=&quot;yellow&quot;&gt;speakers&lt;/font&gt; and a &lt;font color=&quot;green&quot;&gt;moving sound source&lt;/font&gt; as if seen from above. You should hear the audio move between the channels.</source>
        <translation>Il grafico sottostante visualizza la posizione di &lt;font color=&quot;red&quot;&gt;Tu&lt;/font&gt;, gli &lt;font color=&quot;yellow&quot;&gt;Altoparlanti&lt;/font&gt; e &lt;font color=&quot;green&quot;&gt;la fonte mobile dell&apos;audio&lt;/font&gt;come se fosse visto da sopra.Dovresti sentire l&apos;audio muoversi tra gli altoparlanti.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Use headphones</source>
        <translation>Usa cuffie</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Use headphones instead of speakers.</source>
        <translation>Usa le cuffie al posto degli altoparlanti.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This ignores the OS speaker configuration and configures the positioning for headphones instead.</source>
        <translation>Questo ignora la configurazione degli altoparlanti dal sistema operativo e invece configura il posizionamente delle cuffie.</translation>
    </message>
    <message>
        <location line="+22"/>
        <source>Volume tuning</source>
        <translation>Sintonia volume</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Tuning microphone hardware volume to optimal settings.</source>
        <translation>Sintonizza il volume dell&apos;hardware del microfono sulle impostazioni ottimali.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Open your sound control panel and go to the recording settings. Make sure the microphone is selected as active input with maximum recording volume. If there&apos;s an option to enable a &quot;Microphone boost&quot; make sure it&apos;s checked.</source>
        <translation>Apre il pannello di controllo del suono e và alle impostazioni di registrazione. Assicurati che il microfono sia selezionato come input attivo con il massimo volume di registrazione. Se c&apos;è un&apos;opzione da abilitare chiamata &quot;Preamplificazione microfono&quot; assicurati che sia selezionata.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Speak loudly, as when you are annoyed or excited. Decrease the volume in the sound control panel until the bar below stays as high as possible in the blue and green but &lt;b&gt;not&lt;/b&gt; the red zone while you speak.</source>
        <translation>Parla ad alta voce, come quando sei infastidito o eccitato. Poi diminuisci il volume del suono dal pannello di controllo fino a che la barra sotto rimane il più alto possibile nella zona blu e verde quando si parla, ma &lt;b&gt;non&lt;/b&gt; in quella rossa.</translation>
    </message>
    <message>
        <location line="+11"/>
        <source>Now talk softly, as you would when talking late at night and you don&apos;t want to disturb anyone. Adjust the slider below so that the bar moves into green when you talk, but stays blue while you&apos;re silent.</source>
        <translation>Ora parla dolcemente, come quando si parla a tarda notte e non vuoi disturbare nessuno. Regola il cursore sotto in modo che la barra si sposti nella zona verde quando si parla, ma rimane nel blu mentre si sta in silenzio.</translation>
    </message>
    <message>
        <location line="+19"/>
        <source>Voice Activity Detection</source>
        <translation>Rilevamento dell&apos;attività della voce</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Letting Mumble figure out when you&apos;re talking and when you&apos;re silent.</source>
        <translation>Lascia capire a Mumble quando stai parlando e quando stai in silenzio.</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>This will help Mumble figure out when you are talking. The first step is selecting which data value to use.</source>
        <translation>Ciò aiuterà Mumble a capire quando si sta parlando. Il primo passo è la scelta dei dati di valore da usare.</translation>
    </message>
    <message>
        <location line="+26"/>
        <source>Raw amplitude from input</source>
        <translation>Ampiezza grezza dall&apos;input</translation>
    </message>
    <message>
        <location line="-4"/>
        <source>Signal-To-Noise ratio</source>
        <translation>Rapporto Segnale-Rumore</translation>
    </message>
    <message>
        <location line="+27"/>
        <source>Next you need to adjust the following two sliders. The first few utterances you say should end up in the green area (definitive speech). While talking, you should stay inside the yellow (might be speech) and when you&apos;re not talking, everything should be in the red (definitively not speech).</source>
        <translation>Dopo devi aggiustare i seguenti 2 cursori. Le prime parole pronunciate dovrebbero finire nella zona verde (inizio del parlato). Mentre stai parlando dovresti stare dentro il giallo (discorso) e quando non stai parlando, tutto dovrebbe stare nella zona rossa (sottofondo).</translation>
    </message>
    <message>
        <location line="+24"/>
        <source>Last, you need to adjust the grace time. Mumble may detect this much non-speech and still keep transmitting before it breaks off. This allows you to catch your breath while speaking.</source>
        <translation>Infine, è necessario regolare il tempo di grazia. Mumble può rilevare che non stai più parlando e continuare la trasmissione per il tempo di grazia. Questo ti permetterà di riprendere fiato mentre parli senza interrompere la trasmissione.</translation>
    </message>
    <message>
        <location line="+28"/>
        <source>Device tuning</source>
        <translation>Sintonia dispositivo</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Changing hardware output delays to their minimum value.</source>
        <translation>Modifica output dell&apos;hardware al suo valore minimo.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>To keep latency to an absolute minium, it&apos;s important to buffer as little audio as possible on the soundcard. However, many soundcards report that they require a much smaller buffer than what they can actually work with, so the only way to set this value is to try and fail.</source>
        <translation>Per mantenere la latenza a un minimo assoluto, è importante avere come buffer il minimo audio possibile sulla scheda audio. Tuttavia, molte schede audio riportano che esse richiedono molto meno buffer di quello con cui possono effettivamente lavorare , quindi l&apos;unico modo di impostare questo valore è quello di provare, sbagliare e riprovare.</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>You should hear a single tone that&apos;s changing in frequency. Change the slider below to the lowest value which gives &lt;b&gt;no&lt;/b&gt; interruptions or jitter in the sound. Please note that local echo is disabled during this test to improve audio path recognition.</source>
        <translation>Dovresti sentire un tono singolo che cambia di frequenza.Imposta il cursore sottostante al minor valore possibile dove &lt;b&gt;NON&lt;/b&gt; ci sono interruzioni o distorsioni nel suono. Ricorda che l&apos;eco locale è disabilitato durante questo test per trovare meglio l&apos;origine audio.</translation>
    </message>
    <message>
        <location line="+17"/>
        <source>Amount of data to buffer.</source>
        <translation>Quantità di dati del buffer.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This sets the amount of data to prebuffer in the output buffer. Experiment with different values and set it to the lowest which doesn&apos;t cause rapid jitter in the sound.</source>
        <translation>Questo imposta la quantità di prebuffer da mettere nel buffer in uscita. Prova con diversi valori e imposta quello  più basso che non causi rapide distorsioni nel suono.</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>If you position the microphone so it can pick up the speakers or headset, Mumble will measure the total audio path delay in your system; this means the delay from a sample is placed in an outbound buffer until it&apos;s found in a matching incoming buffer.</source>
        <translation>Se si posiziona il microfono in modo che possa raccogliere l&apos;audio dagli altoparlanti o dalle cuffie, Mumble misurerà il ritardo audio nel vostro sistema, questo significa che il ritardo di un campione sarà collocato in un buffer in uscita fino a quando non sarà trovato un buffer corrispondente in arrivo.</translation>
    </message>
    <message>
        <location line="+103"/>
        <source>%1ms</source>
        <translation>%1ms</translation>
    </message>
    <message>
        <location line="+191"/>
        <source>Audio path is %1ms long.</source>
        <translation>L&apos;origine dell&apos;audio è lunga %1ms.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Audio path cannot be determined. Input not recognized.</source>
        <translation>Il percorso dell&apos;audio non può essere determinato. input non riconosciuto.</translation>
    </message>
    <message>
        <location line="+51"/>
        <source>%1 s</source>
        <translation>%1ms</translation>
    </message>
    <message>
        <location line="-708"/>
        <source>Mumble is under continuous development, and the development team wants to focus on the features that benefit the most users. To this end, Mumble supports submitting anonymous statistics about your configuration to the developers. These statistcs are essential for future development, and also make sure the features you use aren&apos;t deprecated.</source>
        <translation>Mumble è in continuo sviluppo, e gli sviluppatori cercano di concentrarsi sulle caratteristiche di cui beneficiano la maggior parte degli utenti. A tal fine, sostenete Mumble inviando statistiche anonime sulla vostra configurazione agli sviluppatori. Queste statistiche sono essenziali per lo sviluppo futuro, e anche per essere sicuri che le caratteristiche che usi non siano disapprovate.</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Submit anonymous statistics to the Mumble project</source>
        <translation>Invia statistiche anonime al progetto Mumble per migliorare il programma</translation>
    </message>
    <message>
        <location line="+224"/>
        <source>Push To Talk:</source>
        <translation>Premi Per Parlare:</translation>
    </message>
</context>
<context>
    <name>BanEditor</name>
    <message>
        <location filename="BanEditor.ui"/>
        <source>Mumble - Edit Bans</source>
        <translation>Mumble - Edita lista utenti banditi</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Address</source>
        <translation>&amp;Indirizzo</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Mask</source>
        <translation>&amp;Maschera</translation>
    </message>
    <message>
        <location/>
        <source>0.0.0.0</source>
        <translation>0.0.0.0</translation>
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
        <translation>Niente</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Traverse</source>
        <translation>Discendi</translation>
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
        <location line="+2"/>
        <source>AltSpeak</source>
        <translation>Stop Parla</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Mute/Deafen</source>
        <translation>Silenzia/Isola</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Move/Kick</source>
        <translation>Sposta/Espelli</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Make channel</source>
        <translation>Crea canale</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Link channel</source>
        <translation>Collega canale</translation>
    </message>
    <message>
        <location line="+10"/>
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
        <translation>Rappresenta il permesso di attraversare il canale. Se a un utente è negato questo privilegio, non potrà accedere al canale e a tutti i sotto-canali, indipendentemente dai permessi dei sotto-canali.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>This represents the permission to join the channel. If you have a hierarchical channel structure, you might want to give everyone Traverse, but restrict Enter in the root of your hierarchy.</source>
        <translation>Rappresenta il permesso di unirsi al canale. In una struttura gerarchica dei canali, potresti volere dare a tutti l&apos;Attraversamento, ma impedire di Entrare nella radice della gerarchia.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>This represents the permission to speak in a channel. Users without this privilege will be suppressed by the server (seen as muted), and will be unable to speak until they are unmuted by someone with the appropriate privileges.</source>
        <translation>Rappresenta il permesso di parlare in un cenale. Un utente senza questo privilegio verrà silenziato dal server (visto come silenziato), e non sarà in grado di parlare fino a che non sarà attivato da qualcuno che abbia i diritti sufficienti.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>This represents the permission to speak in a channel with flagged speech. This works exactly like the &lt;i&gt;speak&lt;/i&gt; privilege, but applies to packets spoken with AltPushToTalk held down. This may be used to broadcast to a hierarchy of channels without linking.</source>
        <translation>Rappresenta il permesso di parlare in un canale. Funziona esattamente come il privilegio per &lt;i&gt;parlare&lt;/i&gt;, ma si applica al parlato mentre è tenuto premuto AltPremiPerParlare. Può essere utilizzato per trasmettere a una gerarchia di canali senza collegarli.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>This represents the permission to mute and deafen other players. Once muted, a player will stay muted until he is unmuted by another privileged player or reconnects to the server.</source>
        <translation>Rappresenta il permesso di silenziare o escludere altri giocatori. Una volta silenziato, un giocatore non potrà parlare fino a che sarà sbloccato da un altro giocatore con privilegi o se si riconnette al server.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>This represents the permission to move a player to another channel or kick him from the server. To actually move the player, either the moving player must have Move/Kick privileges in the destination channel, or the player must normally be allowed to enter the channel. Players with this privilege can move players into channels the target player normally wouldn&apos;t have permission to enter.</source>
        <translation>Rappresenta l&apos;autorizzazione a spostare un giocatore su un altro canale o a espellerlo dal server. Per spostare un giocatore, il giocatore che sposta deve avere il privilegio Sposta/Espelli nel canale di destinazione, o il giocatore che sposta deve essere autorizzato a entrare nel canale. I giocatori con questo privilegio possono muovere altri giocatori in canali dove normalmente non avrebbero permesso di entrare.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>This represents the permission to make sub-channels. The player making the sub-channel will be added to the admin group of the sub-channel.</source>
        <translation>Rappresenta il permesso di creare sotto-canali. Il giocatore che crea un sotto-canale verrà aggiunto al gruppo admin del sotto-canale.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>This represents the permission to link channels. Players in linked channels hear each other, as long as the speaking player has the &lt;i&gt;speak&lt;/i&gt; privilege in the channel of the listener. You need the link privilege in both channels to create a link, but just in either channel to remove it.</source>
        <translation>Rappresenta il permesso di collegare i canali. I giocatori in canali collegati si ascoltano a vicenda, se il giocatore che parla ha il privilegio &lt;i&gt;Parlare&lt;/i&gt; nel canale dell&apos;ascoltatore. È necessario il privilegio di Collegamento in entrambi i canali per poter creare un collegamento, ma in uno qualsiasi dei canali per rimuoverlo.</translation>
    </message>
    <message>
        <location line="-57"/>
        <source>Write ACL</source>
        <translation>Scrive le ACL</translation>
    </message>
</context>
<context>
    <name>ClientPlayer</name>
    <message>
        <location filename="Player.cpp" line="+53"/>
        <source>Authenticated</source>
        <translation>Autenticato</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Muted (server)</source>
        <translation>Microfono disattivato (server)</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Deafened (server)</source>
        <translation>Microfono e audio disattivati (server)</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Local Mute</source>
        <translation>Microfono disattivato localmente</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Muted (self)</source>
        <translation>Microfono disattivato (auto)</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Deafened (self)</source>
        <translation>Microfono e audio disattivati (auto)</translation>
    </message>
</context>
<context>
    <name>ConfigDialog</name>
    <message>
        <location filename="ConfigDialog.cpp" line="+54"/>
        <source>Accept changes</source>
        <translation>Applicare i cambiamenti</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This button will accept current settings and return to the application.&lt;br /&gt;The settings will be stored to disk when you leave the application.</source>
        <translation>Il bottone attiverà le impostazioni correnti e tornerà all&apos;applicazione.&lt;br /&gt;Le impostazioni saranno salvate sul disco quando esci dall&apos;applicazione.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Reject changes</source>
        <translation>Annulla i cambiamenti</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This button will reject all changes and return to the application.&lt;br /&gt;The settings will be reset to the previous positions.</source>
        <translation>Il bottone annullerà tutti i cambiamenti e tornerà all&apos;applicazione.&lt;br /&gt;Le impostazioni saranno riimpostate al valore precedente.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Apply changes</source>
        <translation>Appllica i cambiamenti</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This button will immediately apply all changes.</source>
        <translation>Il bottone applicherà tutti i cambiamenti.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Undo changes for current page</source>
        <translation>Annulla i cambiamenti per la pagina corrente</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This button will revert any changes done on the current page to the most recent applied settings.</source>
        <translation>Il bottone annullerà tutti i cambiamenti fatti sulla pagina corrente al valore precedente.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Restore defaults for current page</source>
        <translation>Ricarica il valore predefinito per la pagina corrente</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This button will restore the defaults for the settings on the current page. Other pages will not be changed.&lt;br /&gt;To restore all settings to their defaults, you will have to use this button on every page.</source>
        <oldsource>This button will restore the settings for the current page only to their defaults. Other pages will not be changed.&lt;br /&gt;To restore all settings to their defaults, you will have to use this button on every page.</oldsource>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This button will restore the settings for the current page only to their defaults. Other pages will be not be changed.&lt;br /&gt;To restore all settings to their defaults, you will have to use this button on every page.</source>
        <translation type="obsolete">Il bottone caricherà le impostazioni per la pagina corrente al loro default. Le altre pagine non saranno modificate.&lt;br /&gt;Per riimpostare tutti i settaggi ai loro valori predefiniti, devi usare questo bottone su tutte le pagine.</translation>
    </message>
    <message>
        <location filename="ConfigDialog.ui"/>
        <source>Mumble Configuration</source>
        <translation>Configurazione di Mumble</translation>
    </message>
    <message>
        <source>Tab 1</source>
        <translation type="obsolete">Tab 1</translation>
    </message>
    <message>
        <location/>
        <source>Advanced</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ConnectDialog</name>
    <message>
        <location filename="ConnectDialog.cpp" line="+124"/>
        <source>Connecting to %1</source>
        <translation>Connessione a %1</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Enter username</source>
        <translation>Inserire il nome utente</translation>
    </message>
    <message>
        <location line="+85"/>
        <source>Unknown</source>
        <translation>Sconosciuto</translation>
    </message>
    <message>
        <source>Mumble</source>
        <translation type="obsolete">Mumble</translation>
    </message>
    <message>
        <location line="+14"/>
        <source>Failed to fetch server list</source>
        <translation>Scaricamento della lista dei server fallita</translation>
    </message>
    <message>
        <location line="+42"/>
        <location line="+22"/>
        <source>New</source>
        <translation>Nuovo</translation>
    </message>
    <message>
        <location line="+38"/>
        <source>-Unnamed entry-</source>
        <translation>-Posizione senza nome-</translation>
    </message>
    <message>
        <location line="+24"/>
        <location line="+27"/>
        <source>Add</source>
        <translation>Aggiungi</translation>
    </message>
    <message>
        <location line="-2"/>
        <source>Update</source>
        <translation>Aggiorna</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui"/>
        <source>Mumble Server Connect</source>
        <translation>Connessione al Server Mumble</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Custom Servers</source>
        <translation>&amp;Server personalizzati</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Label</source>
        <translation>&amp;Etichetta</translation>
    </message>
    <message>
        <location/>
        <source>A&amp;ddress</source>
        <translation>&amp;Indirizzo</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Port</source>
        <translation>&amp;Porta</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Username</source>
        <translation>&amp;Nome utente</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Password</source>
        <translation>&amp;Password</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Connect</source>
        <translation>&amp;Connessione</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Cancel</source>
        <translation>&amp;Annulla</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Add</source>
        <translation>&amp;Aggiungi</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Remove</source>
        <translation>&amp;Cancella</translation>
    </message>
    <message>
        <location/>
        <source>Server &amp;Browser</source>
        <translation>Elenco &amp;Server</translation>
    </message>
    <message>
        <location/>
        <source>Label</source>
        <translation>Etichetta</translation>
    </message>
    <message>
        <location/>
        <source>Address</source>
        <translation>Indirizzo</translation>
    </message>
    <message>
        <location/>
        <source>URL</source>
        <translation>URL</translation>
    </message>
    <message>
        <location/>
        <source>Cancel</source>
        <translation>Annulla</translation>
    </message>
    <message>
        <location/>
        <source>C&amp;opy to custom</source>
        <translation>&amp;Copia al personalizzato</translation>
    </message>
    <message>
        <location/>
        <source>&amp;View Webpage</source>
        <translation>&amp;Visualizza pagina Web</translation>
    </message>
</context>
<context>
    <name>CoreAudioSystem</name>
    <message>
        <location filename="CoreAudio.cpp" line="+83"/>
        <source>Default Device</source>
        <translation type="unfinished">Periferica predefinita</translation>
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
        <location filename="DirectSound.cpp" line="+172"/>
        <source>Default DirectSound Voice Input</source>
        <translation>Ingresso Voce DirectSound predefinito</translation>
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
        <translation>Dispositivo di ingresso DirectSound sconnesso.</translation>
    </message>
</context>
<context>
    <name>DXAudioOutput</name>
    <message>
        <location line="-496"/>
        <source>Default DirectSound Voice Output</source>
        <translation>Uscita Voce DirectSound predefinita</translation>
    </message>
    <message>
        <location line="+247"/>
        <source>Opening chosen DirectSound Output failed. Default device will be used.</source>
        <translation>Apetuta dell&apos;uscita DirectSound scelta fallita. Verrà usato il dispositivo di default.</translation>
    </message>
    <message>
        <location line="+78"/>
        <source>Lost DirectSound output device.</source>
        <translation>Dispositivo di uscita DirectSound sconnesso.</translation>
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
        <location filename="Database.cpp" line="+127"/>
        <source>Mumble failed to initialize a database in any
of the possible locations.</source>
        <translation>Mumble non è riuscito a inizializzare il database in nessuna
delle possibili posizioni.</translation>
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
        <source>Shortcut</source>
        <translation>Scorciatoia</translation>
    </message>
    <message>
        <location/>
        <source>Suppress</source>
        <translation>Sopprimi</translation>
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
        <location filename="GlobalShortcut.cpp" line="+228"/>
        <source>Shortcuts</source>
        <translation>Scorciatoie</translation>
    </message>
    <message>
        <location line="+33"/>
        <source>Shortcut button combination.</source>
        <translation>Combinazione di bottoni della scorciatoia.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>&lt;b&gt;This is the global shortcut key combination.&lt;/b&gt;&lt;br /&gt;Double-click this field and then the desired key/button combo to rebind.</source>
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
        <translation>&lt;b&gt;Nasconde le pressioni dei tasti nelle altre applicazioni.&lt;/b&gt;&lt;br /&gt;Abilitando verranno nascosti i bottoni (o l&apos;ultimo bottone di una sequenza multi bottone) dalle altre applicazioni. Nota che non tutti i bottoni possono essere premuti.</translation>
    </message>
</context>
<context>
    <name>GlobalShortcutMacInit</name>
    <message>
        <location filename="GlobalShortcut_macx.cpp" line="+63"/>
        <source>Mumble has detected that it is unable to receive Global Shortcut events when it is in the background.&lt;br /&gt;&lt;br /&gt;This is because the Universal Access feature called &apos;Enable access for assistive devices&apos; is currently disabled.&lt;br /&gt;&lt;br /&gt;Please &lt;a href=&quot; &quot;&gt;enable this setting&lt;/a&gt; and continue when done.</source>
        <translation>Mumble ha rilevato che non è in grado di ricevere i segnali delle Scorciatoie Globali quando è in background.&lt;br /&gt;&lt;br /&gt;Questo è dovuto alla funzione di Accesso Universale chiamata &apos;Abilita accesso per i dispositivi di assistenza&apos; che è attualmente disattiva.&lt;br /&gt;&lt;br /&gt;Si prega &lt;a href=&quot;&quot;&gt;abilitare questa impostazione&lt;/a&gt; e proseguire quando terminato.</translation>
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
        <location filename="LCD.cpp" line="+274"/>
        <source>Not connected</source>
        <translation>Non collegato</translation>
    </message>
</context>
<context>
    <name>LCDConfig</name>
    <message>
        <location line="-161"/>
        <source>Graphic</source>
        <translation>Grafica</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Character</source>
        <translation>Carattere</translation>
    </message>
    <message>
        <location line="+11"/>
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
        <translation>&lt;!DOCTYPE HTML PUBLIC &quot;-//W3C//DTD HTML 4.0//EN&quot; &quot;http://www.w3.org/TR/REC-html40/strict.dtd&quot;&gt;
&lt;html&gt;&lt;head&gt;&lt;meta name=&quot;qrichtext&quot; content=&quot;1&quot; /&gt;&lt;style type=&quot;text/css&quot;&gt;
p, li { white-space: pre-wrap; }
&lt;/style&gt;&lt;/head&gt;&lt;body style=&quot; font-family:&apos;MS Shell Dlg 2&apos;; font-size:8.25pt; font-weight:400; font-style:normal;&quot;&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;Questa è la lista dei dispositivi LCD nel tuo sistema. Nella lista ci sono i nomi, ma include anche la dimensione del display e il tipo.Mumble supporta l&apos;invio a molti dispositivi LCD allo stesso tempo.&lt;/p&gt;
&lt;p style=&quot;-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;&lt;span style=&quot; font-weight:600; font-style:italic;&quot;&gt;Type:&lt;/span&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;Mumble supporta 2 differenti tipi di dispositivi LCD. Gli LCD Graphic, dove è possibile disegnare in ogni pizel individualmente, ma anche gli LCD Character dove è possibile solo scrivere dei caratteri. Alcune funzioni di Mumble sono disponibili solo sugli LDC Graphic.&lt;/p&gt;
&lt;p style=&quot;-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;&lt;span style=&quot; font-weight:600; font-style:italic;&quot;&gt;Size:&lt;/span&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;Questo campo descrive la dimenzione di un dispositivo LCD. Ladimensione è espressa in pixel o in caratteri.&lt;/p&gt;
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
        <location/>
        <source>Type</source>
        <translation>Tipo</translation>
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
        <translation>Viste</translation>
    </message>
    <message>
        <location/>
        <source>Minimum Column Width</source>
        <translation>Larghezza minima colonna</translation>
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
        <translation>&lt;!DOCTYPE HTML PUBLIC &quot;-//W3C//DTD HTML 4.0//EN&quot; &quot;http://www.w3.org/TR/REC-html40/strict.dtd&quot;&gt;&lt;html&gt;&lt;head&gt;&lt;meta name=&quot;qrichtext&quot; content=&quot;1&quot; /&gt;&lt;style type=&quot;text/css&quot;&gt;p, li { white-space: pre-wrap; }&lt;/style&gt;&lt;/head&gt;&lt;body style=&quot; font-family:&apos;MS Shell Dlg 2&apos;; font-size:8.25pt; font-weight:400; font-style:normal;&quot;&gt;&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;&lt;span style=&quot; font-size:8pt;&quot;&gt;Questa opzione imposta la minima larghezza della colonna nella vista del Giocatore.&lt;/span&gt;&lt;/p&gt;&lt;p style=&quot;-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;&lt;/p&gt;&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;Se troppe persone stanno parlando insieme, la Vista Giocatore verrà divisa in colonne.&lt;/p&gt;&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;Puoi usare questa opzione per scegliere un compromesso tra il numero di giocatori mostrati sull&apos;LCD,&lt;/p&gt;&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;e la larghezza dei nomi dei giocatori.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location/>
        <source>TextLabel</source>
        <translation>Etichetta testo</translation>
    </message>
    <message>
        <location/>
        <source>Splitter Width</source>
        <translation>Larghezza Splitter</translation>
    </message>
    <message>
        <location/>
        <source>&lt;!DOCTYPE HTML PUBLIC &quot;-//W3C//DTD HTML 4.0//EN&quot; &quot;http://www.w3.org/TR/REC-html40/strict.dtd&quot;&gt;
&lt;html&gt;&lt;head&gt;&lt;meta name=&quot;qrichtext&quot; content=&quot;1&quot; /&gt;&lt;style type=&quot;text/css&quot;&gt;
p, li { white-space: pre-wrap; }
&lt;/style&gt;&lt;/head&gt;&lt;body style=&quot; font-family:&apos;MS Shell Dlg 2&apos;; font-size:8.25pt; font-weight:400; font-style:normal;&quot;&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;&lt;span style=&quot; font-size:8pt;&quot;&gt;This setting decides the width of column splitter.&lt;/span&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;!DOCTYPE HTML PUBLIC &quot;-//W3C//DTD HTML 4.0//EN&quot; &quot;http://www.w3.org/TR/REC-html40/strict.dtd&quot;&gt;&lt;html&gt;&lt;head&gt;&lt;meta name=&quot;qrichtext&quot; content=&quot;1&quot; /&gt;&lt;style type=&quot;text/css&quot;&gt;p, li { white-space: pre-wrap; }&lt;/style&gt;&lt;/head&gt;&lt;body style=&quot; font-family:&apos;MS Shell Dlg 2&apos;; font-size:8.25pt; font-weight:400; font-style:normal;&quot;&gt;&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;&lt;span style=&quot; font-size:8pt;&quot;&gt;Questa impostazione regola la larghezza della colonna dello splitter.&lt;/span&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
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
        <translation type="unfinished">Critici</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Warning</source>
        <translation type="unfinished">Avvisi</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Information</source>
        <translation type="unfinished">Informativi</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Server Connected</source>
        <translation type="unfinished">Server Connesso</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Server Disconnected</source>
        <translation type="unfinished">Serve Disconnesso</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Player Joined Server</source>
        <translation type="unfinished">Giocatore connesso al Server</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Player Left Server</source>
        <translation type="unfinished">Giocatore disconnesso dal Server</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Player kicked (you or by you)</source>
        <translation type="unfinished">Giocatore espulso (tu o da te)</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Player kicked</source>
        <translation type="unfinished">Giocatore espulso</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>You self-muted/deafened</source>
        <translation type="unfinished">Hai disattivato il tuo mirofono/audio</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Other self-muted/deafened</source>
        <translation type="unfinished">Utente con mirofono/audio disattivato</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Player muted (you)</source>
        <translation type="unfinished">utente sileziato (tu)</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Player muted (by you)</source>
        <translation type="unfinished">Utente silenziato (da te)</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Player muted (other)</source>
        <translation type="unfinished">Utente silenziato (altro)</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Player Joined Channel</source>
        <translation type="unfinished">Utente entrato nel Canale</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Player Left Channel</source>
        <translation type="unfinished">Utente uscito dal Canale</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Permission Denied</source>
        <translation type="unfinished">Permesso Negato</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Text Message</source>
        <translation type="unfinished">Messaggio di Testo</translation>
    </message>
    <message>
        <source>Script Errors</source>
        <translation type="obsolete">Errori di script</translation>
    </message>
</context>
<context>
    <name>LogConfig</name>
    <message>
        <location line="-164"/>
        <source>Toggle console for %1 events</source>
        <translation>Cambia console per gli eventi %1</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Toggle pop-up notifications for %1 events</source>
        <translation>Cambia notifica pop-up per gli eventi %1</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Toggle Text-To-Speech for %1 events</source>
        <translation>Cambia Sintesi vocale per gli eventi %1</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Click here to toggle sound notification for %1 events</source>
        <translation>Premi qui per cambiare la notifica audio per gli eventi %1</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Path to soundfile used for sound notifications in the case of %1 events&lt;br /&gt;Single click to play&lt;br /&gt;Doubleclick to change</source>
        <translation>Percorso dei file audio utilizzati per le notifiche in caso di eventi %1&lt;br /&gt;Unico click per sentire&lt;br /&gt;Doppio click per cambiare</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Click here to toggle console output for %1 events.&lt;br /&gt;If checked, this option makes Mumble output all %1 events in its message log.</source>
        <translation>Clicca qui per attivare la console di uscita per gli eventi %1.&lt;br /&gt; Se selezionata, questa opzione scrive  tutti gli eventi %1 nel messaggio di log.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Click here to toggle pop-up notifications for %1 events.&lt;br /&gt;If checked, a notification pop-up will be created by Mumble for every %1 event.</source>
        <oldsource>Click here to toggle pop-up notifications for %1 events.&lt;br /&gt;If checked, a notification pop-up will be created by mumble for every %1 event.</oldsource>
        <translation type="unfinished">Clicca qui per cambiare le notifiche pop-up per gli eventi%1. &lt;br /&gt; Se selezionato, una notifica pop-up verrà creata per ogni evento%1.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Click here to toggle Text-To-Speech for %1 events.&lt;br /&gt;If checked, Mumble uses Text-To-Speech to read %1 events out loud to you. Text-To-Speech is also able to read the contents of the event which is not true for soundfiles. Text-To-Speech and soundfiles cannot be used at the same time.</source>
        <oldsource>Click here to toggle Text-To-Speech for %1 events.&lt;br /&gt;If checked, mumble uses Text-To-Speech to read %1 events out loud to you. Text-To-Speech is also able to read the contents of the event which is not true for soundfiles. Text-To-Speech and soundfiles cannot be used at the same time.</oldsource>
        <translation type="unfinished">Clicca qui per notificare gli eventi%1 con un messaggio di Sintesi Vocale. &lt;br /&gt; Se selezionato, mumble utilizza La sintesi vocale per leggere gli eventi %1 ad alta voce a voi. La sintesi vocale è anche in grado di leggere il contenuto dell&apos;evento che non è leggibile con il file audio. il file audio per la sintesi vocale e non può essere utilizzato allo stesso tempo.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Click here to toggle sound notification for %1 events.&lt;br /&gt;If checked, Mumble uses a soundfile predefined by you to indicate %1 events. Soundfiles and Text-To-Speech cannot be used at the same time.</source>
        <oldsource>Click here to toggle sound notification for %1 events.&lt;br /&gt;If checked, mumble uses a soundfile predefined by you to indicate %1 events. Soundfiles and Text-To-Speech cannot be used at the same time.</oldsource>
        <translation type="unfinished">Clicca qui per cambiare la notifica sonora per gli eventi %1.&lt;br /&gt;Se selezionato mumble userò un file audio predefinito per indicare gli eventi %1. File audio e SIntesi Vocale non possono essere usati allo stesso tempo.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Path to soundfile used for sound notifications in the case of %1 events.&lt;br /&gt;Single click to play&lt;br /&gt;Doubleclick to change&lt;br /&gt;Be sure that sound notifications for these events are enabled or this field will not have any effect.</source>
        <translation>Percorso al fiel audio usato per la notifica audio nel caso degli eventi %1.&lt;br /&gt;Click singolo per ascoltare&lt;br /&gt;Doppio click per cambiare&lt;br /&gt;Assicurati che la notifia audio sia abilitata per questi eventi oppure questi campi non produrranno effetti.</translation>
    </message>
    <message>
        <location line="+93"/>
        <source>Choose sound file</source>
        <translation>Scegli il file del suono</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Invalid sound file</source>
        <translation>Suono del file invalido</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The file &apos;%1&apos; does not exist or is not a valid speex file.</source>
        <translation>Il file &quot;%1&quot; non esiste o è un file speex invalido.</translation>
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
        <translation>File del suono</translation>
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
        <translation>&lt;b&gt;Soglia di lunghezza usata per il Motore di Sintesi Vocale.&lt;/b&gt;&lt;br&gt;Messaggi più lunghi di questo limite non verranno letti interamente.</translation>
    </message>
    <message>
        <location/>
        <source> Characters</source>
        <translation>Caratteri</translation>
    </message>
</context>
<context>
    <name>LookConfig</name>
    <message>
        <location filename="LookConfig.cpp" line="+46"/>
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
        <location line="+1"/>
        <source>Only with players</source>
        <translation>Solo con i giocatori</translation>
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
        <location line="+85"/>
        <source>Choose skin file</source>
        <translation>Scegli file delle skin</translation>
    </message>
    <message>
        <location filename="LookConfig.ui"/>
        <source>Form</source>
        <translation>Modulo</translation>
    </message>
    <message>
        <location/>
        <source>Language</source>
        <translation>Lingua</translation>
    </message>
    <message>
        <location/>
        <source>Language to use (requires restart)</source>
        <translation>Lingua da usare (richiede riavvio)</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets which language Mumble should use.&lt;/b&gt;&lt;br /&gt;You have to restart Mumble to use the new language.</source>
        <translation>&lt;b&gt;Stabilisce quale lingua dovrebbe usare Mumble.&lt;/b&gt;&lt;br /&gt;Devi riavviare Mumble per utilizzare la nuova lingua.</translation>
    </message>
    <message>
        <location/>
        <source>Look and Feel</source>
        <translation>Aspetto e Sensazione</translation>
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
        <location/>
        <source>...</source>
        <translation>...</translation>
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
        <location/>
        <source>This sets which channels to automatically expand. &lt;i&gt;None&lt;/i&gt; and &lt;i&gt;All&lt;/i&gt; will expand no or all channels, while &lt;i&gt;Only with players&lt;/i&gt; will expand and collapse channels as players join and leave them.</source>
        <translation>Stabilisce quali canali espandere automaticamente. &lt;i&gt;Nessuno&lt;/i&gt; e &lt;i&gt;Tutti&lt;/i&gt; non espanderà nessuno o tutti i canali, mentre &lt;i&gt;Solo con i giocatori&lt;/i&gt; espanderà e comprimerà i canali appena i giocatori entrano o escono.</translation>
    </message>
    <message>
        <location/>
        <source>List players above subchannels (requires restart).</source>
        <translation>Elenca i giocatori sui sottocanali (richiede riavvio).</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;If set, players will be shown above subchannels in the channel view.&lt;/b&gt;&lt;br /&gt;A restart of Mumble is required to see the change.</source>
        <translation>&lt;b&gt;Se impostato, i giocatori verranno visualizzati sopra i sottocanali mostrati.&lt;/b&gt;&lt;br /&gt;E&apos; necessario un riavvio di Mumble per vedere il cambiamento.</translation>
    </message>
    <message>
        <location/>
        <source>Players above Channels</source>
        <translation>Giocatori sui Canali</translation>
    </message>
    <message>
        <location/>
        <source>Channel Dragging</source>
        <translation>Trascinamento Canale</translation>
    </message>
    <message>
        <location/>
        <source>This changes the behaviour when moving channels.</source>
        <translation>Cambia il comportamento quando sposti i canali.</translation>
    </message>
    <message>
        <location/>
        <source>This sets the behaviour of channel drags, it can be used to prevent accidental dragging. &lt;i&gt;Move Channel&lt;/i&gt; moves the channel without prompting. &lt;i&gt;Do Nothing&lt;/i&gt; does nothing and prints an error message. &lt;i&gt;Ask&lt;/i&gt; uses a message box to confirm if you really wanted to move the channel.</source>
        <translation>Imposta il comportamento dei trascinamenti di canale, può essere utilizzato per evitare il trascinamento accidentale. &lt;i&gt;Sposta Canale&lt;/i&gt; il canale si sposta senza chiedere conferma. &lt;i&gt;Non fare nulla&lt;/i&gt; non li sposta e stampa un messaggio di errore. &lt;i&gt;Chiedi&lt;/i&gt; mostra una finestra di dialogo per chiedere conferma se si voleva davvero spostare il canale.</translation>
    </message>
    <message>
        <location/>
        <source>Ask whether to close or minimize when quitting Mumble.</source>
        <translation>Chiede se chiudere o minimizzare quando esci da Mumble.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;If set, will verify you want to quit if connected.&lt;b&gt;</source>
        <translation>&lt;b&gt;Se impostato, chiederà conferma che tu voglia veramente uscire quando sei collegato.&lt;b&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Ask on quit while connected</source>
        <translation>Chiedi conferma in uscita se connesso</translation>
    </message>
    <message>
        <location/>
        <source>Make the Mumble window appear on top of other windows.</source>
        <translation>Tieni la finestra di Mumble sopra alle altre finestre.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This makes the Mumble window a topmost window.&lt;/b&gt;</source>
        <translation>&lt;b&gt;Tiene la finestra di Mumble sopra le altre.&lt;/b&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Always On Top</source>
        <translation>Sempre in primo piano</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;If set, minimizing the Mumble main window will cause it to be hidden and accessible only from the tray. Otherwise, it will be minimized as a window normally would.&lt;/b&gt;</source>
        <translation>&lt;b&gt;Se impostato, minimizare la finestra principale di Mumble causerà questa ad essere nascosta e accessibile solo dalla tray. Altrimenti sarà minimizzata come una normale finestra.&lt;/b&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Hide in tray when minimized</source>
        <translation>Nascondi nella tray quando minimizzato</translation>
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
        <translation>Nascondi la finestra principale di Mumble nella tray quando è minimizzata.</translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <location filename="MainWindow.cpp" line="+127"/>
        <location line="+1196"/>
        <source>Root</source>
        <translation>Radice</translation>
    </message>
    <message>
        <location line="-1160"/>
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
        <translation>Questo configura il bottone premi-pre-parlare, e fino a che terrai premuto questo bottone, trasmetterai la voce.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Reset Audio Processor</source>
        <comment>Global Shortcut</comment>
        <translation>Reset processore audio</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Toggle Mute Self</source>
        <comment>Global Shortcut</comment>
        <translation>Cambia Silenziarti</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Toggle self-mute status.</source>
        <comment>Global Shortcut</comment>
        <translation>Cambia stato Silenziati.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This will toggle your muted status. If you toggle this off, you will also disable self-deafen.</source>
        <comment>Global Shortcut</comment>
        <translation>Questo cambia il tuo stato di silenziato. Se la selezione è off,verrai riattivato.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Toggle Deafen Self</source>
        <comment>Global Shortcut</comment>
        <translation>Cambia stato isolamento</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Toggle self-deafen status.</source>
        <comment>Global Shortcut</comment>
        <translation>Cambia stato asoalti.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This will toggle your deafen status. If you toggle this on, you will also enable self-mute.</source>
        <comment>Global Shortcut</comment>
        <translation>Questo cambia il tuo stato isolati. Se la selezione è on, sarà abilitato ache il Silenziati.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Unlink Plugin</source>
        <comment>Global Shortcut</comment>
        <translation>Scollega Plugin</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Force Center Position</source>
        <comment>Global Shortcut</comment>
        <translation>Forza posizione centrale</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Chan Parent</source>
        <comment>Global Shortcut</comment>
        <translation>Parente canale</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Chan Sub#%1</source>
        <comment>Global Shortcut</comment>
        <translation>Sotto-canale#%1</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Chan All Subs</source>
        <comment>Global Shortcut</comment>
        <translation>Canale tutti sotto-canali</translation>
    </message>
    <message>
        <location line="+4"/>
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
        <translation>Cambia la copertura</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Toggle state of in-game overlay.</source>
        <comment>Global Shortcut</comment>
        <translation>Cambia stato di sovrapposizione in-game.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This will switch the states of the ingame overlay between showing everybody, just the players who are talking, and nobody.</source>
        <comment>Global Shortcut</comment>
        <translation>Questo cambia lo stato della sovrapposizione in-game tra visualizza tutti, solo il giocatore a cui stai parlando e nessuno.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Alt Push-to-Talk</source>
        <comment>Global Shortcut</comment>
        <translation>Alt Premi-per.parlare</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Toggle Minimal</source>
        <comment>Global Shortcut</comment>
        <translation>Cambia  Visualizzazione minima</translation>
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
        <location filename="MainWindow.cpp" line="+13"/>
        <source>Mumble -- %1</source>
        <translation>Mumble -- %1</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>&amp;Window</source>
        <translation>&amp;Finestra</translation>
    </message>
    <message>
        <location line="+2"/>
        <location line="+76"/>
        <source>Minimize</source>
        <translation>Minimizza</translation>
    </message>
    <message>
        <location line="-76"/>
        <source>Ctrl+M</source>
        <translation>Ctrl+M</translation>
    </message>
    <message>
        <location line="+75"/>
        <source>Close</source>
        <translation>Chiudi</translation>
    </message>
    <message>
        <location line="-1"/>
        <source>Mumble is currently connected to a server. Do you want to Close or Minimize it?</source>
        <translation>Mumble è connesso ad un server. Vuoi Chiuderlo o Minimizzarlo?</translation>
    </message>
    <message>
        <location line="+83"/>
        <source>Clear</source>
        <translation>Cancella</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Opening URL %1</source>
        <translation>Apertura URL %1</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>URL scheme is not &apos;mumble&apos;</source>
        <translation>Lo schema dell&apos;URL non è &apos;mumble&apos;</translation>
    </message>
    <message>
        <location line="+25"/>
        <source>Connecting to %1</source>
        <translation>Connessione a %1</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Enter username</source>
        <translation>Inserisci nome utente</translation>
    </message>
    <message>
        <location line="+136"/>
        <source>Reconnecting.</source>
        <translation>Riconnessione.</translation>
    </message>
    <message>
        <location line="+50"/>
        <source>&lt;h2&gt;Control channel&lt;/h2&gt;&lt;p&gt;Encrypted with %1 bit %2&lt;br /&gt;%3 ms average latency (%4 variance)&lt;/p&gt;</source>
        <translation>&lt;h2&gt;Canale di controllo&lt;/h2&gt;&lt;p&gt;Crittografato con %1 bit %2&lt;br/&gt;%3 ms latenza media (%4 variazione)&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Voice channel is sent over control channel.</source>
        <translation>Il canale di voce è trasmesso sul canale di controllo.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>&lt;h2&gt;Voice channel&lt;/h2&gt;&lt;p&gt;Encrypted with 128 bit OCB-AES128&lt;br /&gt;%1 ms average latency (%4 variance)&lt;/p&gt;</source>
        <translation>&lt;h2&gt;Canale di voce&lt;/h2&gt;&lt;p&gt;Crittografato con 128 bit OCB-AES128&lt;br/&gt;%1 ms latenza media (%4 variazione)&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+7"/>
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
        <location line="+4"/>
        <source>&lt;h2&gt;Audio bandwidth&lt;/h2&gt;&lt;p&gt;Maximum %1 kbit/s&lt;br /&gt;Current %2 kbit/s (Quality %3)&lt;/p&gt;</source>
        <translation>&lt;h2&gt;banda audio&lt;/h2&gt;&lt;p&gt;Massimo %1 kbit/s&lt;br/&gt;Corrente %2 kbit/s (Qualità %3)&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Mumble Server Information</source>
        <translation>Informazioni del server di Mumble</translation>
    </message>
    <message>
        <location line="+4"/>
        <location line="+753"/>
        <source>&amp;View Certificate</source>
        <translation>&amp;Visualizza certificato</translation>
    </message>
    <message>
        <location line="-653"/>
        <source>Kicking player %1</source>
        <translation>Espulsione giocatore %1</translation>
    </message>
    <message>
        <location line="+0"/>
        <location line="+22"/>
        <source>Enter reason</source>
        <translation>Inserisci motivazione</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Banning player %1</source>
        <translation>Bandimento giocatore %1</translation>
    </message>
    <message>
        <location line="+22"/>
        <source>Sending message to %1</source>
        <translation>Invio messaggio a %1</translation>
    </message>
    <message>
        <location line="+11"/>
        <location line="+253"/>
        <source>To %1: %2</source>
        <translation>Per %1: %2</translation>
    </message>
    <message>
        <location line="-253"/>
        <location line="+253"/>
        <source>Message to %1</source>
        <translation>Messaggio a %1</translation>
    </message>
    <message>
        <location line="-157"/>
        <location line="+43"/>
        <source>Channel Name</source>
        <translation>Nome canale</translation>
    </message>
    <message>
        <location line="-21"/>
        <source>Are you sure you want to delete %1 and all its sub-channels?</source>
        <translation>Sei sicuro di voler cancellare %1 e tutti i suoi sotto-canali?</translation>
    </message>
    <message>
        <location line="+43"/>
        <source>Change description of channel %1</source>
        <translation>Cambia la descrizione del canale %1</translation>
    </message>
    <message>
        <location line="+79"/>
        <source>Sending message to channel %1</source>
        <translation>Invio messaggio al canale %1</translation>
    </message>
    <message>
        <location line="+27"/>
        <source>Sending message to channel tree %1</source>
        <translation>Invio messaggio al canale e ai sottocanali di %1</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>To tree %1: %2</source>
        <translation>Per il canale e i sottocanali di %1: %2</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Message to tree %1</source>
        <translation>Messaggio al canale e ai sottocanali %1</translation>
    </message>
    <message>
        <location line="+21"/>
        <source>Unmuted and undeafened.</source>
        <translation>Microfono e audio riattivato.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Unmuted.</source>
        <translation>Microfono riattivato.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Muted.</source>
        <translation>Microfono disattivato.</translation>
    </message>
    <message>
        <location line="+20"/>
        <source>Muted and deafened.</source>
        <translation>Microfono e audio disattivati.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Deafened.</source>
        <translation>Audio disattivato.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Undeafened.</source>
        <translation>Audio riattivato.</translation>
    </message>
    <message>
        <location line="+62"/>
        <source>About Qt</source>
        <translation>Informazioni su Qt</translation>
    </message>
    <message>
        <location line="+97"/>
        <source>Joining %1.</source>
        <translation>Entrata %1.</translation>
    </message>
    <message>
        <location line="+33"/>
        <source>Connected to server %1.</source>
        <translation>Connessione al server %1.</translation>
    </message>
    <message>
        <location line="+56"/>
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
        <location line="+28"/>
        <source>Server connection failed: %1.</source>
        <translation>Connessione al server fallita: %1.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Disconnected from server.</source>
        <translation>Disconnesso dal server.</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Invalid username</source>
        <translation>Nome utente invalido</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>You connected with an invalid username, please try another one.</source>
        <translation>Ti sei connesso con un nome utente invalido, provane un&apos;altro.</translation>
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
        <location filename="MainWindow.ui"/>
        <source>&amp;Server</source>
        <translation>&amp;Server</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Player</source>
        <translation>&amp;Giocatore</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Channel</source>
        <translation>&amp;Canale</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Audio</source>
        <translation>&amp;Audio</translation>
    </message>
    <message>
        <location/>
        <source>C&amp;onfigure</source>
        <translation>C&amp;onfigurazione</translation>
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
        <location/>
        <source>Shows a dialog of registered servers, and also allows quick connect.</source>
        <translation>Visualizza una finestra dei server registrati, e permette anche la connessione rapida.</translation>
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
        <translation>Modifica la lista dei ban sul server</translation>
    </message>
    <message>
        <location/>
        <source>This lets you edit the server-side IP ban lists.</source>
        <translation>Questo ti fà modificare la lista degli ip bannati dal lato server.</translation>
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
        <location/>
        <source>Kick player (with reason)</source>
        <translation>Espelli giocatore (con motivazione)</translation>
    </message>
    <message>
        <location/>
        <source>Kick selected player off server. You&apos;ll be asked to specify a reason.</source>
        <translation>Espelle il giocatore dal server. Ti sarà chiesto di specificare una motivazione.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Mute</source>
        <translation>Disattiva &amp;microfono</translation>
    </message>
    <message>
        <location/>
        <source>Mute player</source>
        <translation>Disattiva microfono giocatore</translation>
    </message>
    <message>
        <location/>
        <source>Mute or unmute player on server. Unmuting a deafened player will also undeafen them.</source>
        <translation>Disattiva o riattiva il microfono dell&apos;utente nel server. Riattivare un giocatore escluso lo farà includere di nuovo.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Ban</source>
        <translation>&amp;Bandisci</translation>
    </message>
    <message>
        <location/>
        <source>Kick and ban player (with reason)</source>
        <translation>Espelli e bandisci il giocatore (con motivazione)</translation>
    </message>
    <message>
        <location/>
        <source>Kick and ban selected player from server. You&apos;ll be asked to specify a reason.</source>
        <translation>Espelli e bandisci il giocatore  dal server. Tri sarà chiesto di specificare una motivazione.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Deafen</source>
        <translation>Disattiva &amp;audio</translation>
    </message>
    <message>
        <location/>
        <source>Deafen player</source>
        <translation>Disattiva l&apos;audio del giocatore</translation>
    </message>
    <message>
        <location/>
        <source>Deafen or undeafen player on server. Deafening a player will also mute them.</source>
        <translation>Disattiva o riattiva l&apos;audio dell&apos;utente nel server. Disattivando l&apos;audio dell&apos;utente disattiverai anche il suo microfono.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Local Mute</source>
        <translation>Disattiva &amp;microfono localmente</translation>
    </message>
    <message>
        <location/>
        <source>Mute player locally</source>
        <translation>Disattiva il microfono dell&apos;utente localmente</translation>
    </message>
    <message>
        <location/>
        <source>Mute or unmute player locally. Use this on other players in the same room.</source>
        <translation>Disattiva o riattiva il mirofono dell&apos;utente localmente. Usa questo su altri giocatore nella stessa room.</translation>
    </message>
    <message>
        <location/>
        <source>Send Messa&amp;ge</source>
        <translation>Invia &amp;Messaggio</translation>
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
        <translation>Questo aggiungerà un nuovo sotto-canale al canale selezionato.</translation>
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
        <translation>Questo rimuoverà il canale e tutti i suoi sotto-canali.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Edit ACL</source>
        <translation>&amp;Modifica ACL</translation>
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
        <location/>
        <source>&amp;Rename</source>
        <translation>&amp;Rinomina</translation>
    </message>
    <message>
        <location/>
        <source>Renames the channel</source>
        <translation>Rinomina il canale</translation>
    </message>
    <message>
        <location/>
        <source>This renames a channel.</source>
        <translation>Questo rinomina il canale.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Change Description</source>
        <translation>&amp;Cambia Descrizione</translation>
    </message>
    <message>
        <location/>
        <source>Changes the channel description</source>
        <translation>Cambia la descrizione del canale</translation>
    </message>
    <message>
        <location/>
        <source>This changes the description of a channel.</source>
        <translation>Questo cambia la descrizione del canale.</translation>
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
        <location/>
        <source>This links your current channel to the selected channel. If they have permission to speak in the other channel, players can now hear each other. This is a permanent link, and will last until manually unlinked or the server is restarted. Please see the shortcuts for push-to-link.</source>
        <translation>Questo collega il canale corrente con il canale selezionato. Se i giocatori hanno il permesso per parlare nell&apos;altro canale, si potranno sentire con gli altri. Questo è un collegamento permanente, e non terminerà fino a che i canali non saranno scollegati manualmente o il server verrà riavviato. Per favore guarda la scorciatoia per il Premi-per-collegare.</translation>
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
        <location/>
        <source>Unlink &amp;All</source>
        <translation>Scollega da &amp;tutti</translation>
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
        <translation>Disattiva &amp;microfono</translation>
    </message>
    <message>
        <location/>
        <source>Mute yourself</source>
        <translation>Disattiva il tuo microfono</translation>
    </message>
    <message>
        <location/>
        <source>Mute or unmute yourself. When muted, you will not send any data to the server. Unmuting while deafened will also undeafen.</source>
        <translation>Silenzia o riattiva te stesso. Quando sei silenziato tu non invii nessun dato al server. Riattivarsi quando si è esclusi implica anche riammettersi.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Deafen Self</source>
        <translation>Disattiva &amp;audio</translation>
    </message>
    <message>
        <location/>
        <source>Deafen yourself</source>
        <translation>Disattiva il tuo audio</translation>
    </message>
    <message>
        <location/>
        <source>Deafen or undeafen yourself. When deafened, you will not hear anything. Deafening yourself will also mute.</source>
        <translation>Disattiva o riattiva il tuo audio. Disattivare l&apos;audio implica anche disattivare il microfono.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Text-To-Speech</source>
        <translation>&amp;Sintesi vocale</translation>
    </message>
    <message>
        <location/>
        <source>Toggle Text-To-Speech</source>
        <translation>Cambia lo stato di Sintesi vocale</translation>
    </message>
    <message>
        <location/>
        <source>Enable or disable the text-to-speech engine. Only messages enabled for TTS in the Configuration dialog will actually be spoken.</source>
        <translation>Abilita o disabilita il motore di sintesi vocale. Solamente i messaggi abilitati per la sintesi vocale nella finestra di configurazione saranno trasmessi.</translation>
    </message>
    <message>
        <location/>
        <source>S&amp;tatistics</source>
        <translation>S&amp;tatistiche</translation>
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
        <location/>
        <source>&amp;Unlink</source>
        <comment>Plugin</comment>
        <translation>S&amp;collega</translation>
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
        <translation>&amp;Impostazioni</translation>
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
        <source>Send &amp;Tree Message</source>
        <translation>Invia un messaggio al &amp;canale e ai sottocanali</translation>
    </message>
    <message>
        <location/>
        <source>Send message to channel tree.</source>
        <translation>Invia un messaggio ad un canale e ai suoi sottocanali.</translation>
    </message>
    <message>
        <location/>
        <source>This sends a text message to a channel and it&apos;s subchannels.</source>
        <translation>Questo invia un messaggio al canale e a tutti i suoi sotto-canali.</translation>
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
        <location filename="Messages.cpp" line="+56"/>
        <source>Joined server: %1.</source>
        <translation>%1 è entrato nel server.</translation>
    </message>
    <message>
        <location line="+20"/>
        <source>Left server: %1.</source>
        <translation>%1 ha lasciato il server.</translation>
    </message>
    <message>
        <location line="+33"/>
        <source>%1 is now muted and deafened.</source>
        <translation>%1 ha disattivato audio e microfono.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1 is now muted.</source>
        <translation>%1 ha disattivato il microfono.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1 is now unmuted.</source>
        <translation>%1 ha riattivato il microfono.</translation>
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
        <translation>Il tuo microfono è stato disattivato da %1.</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>You were unmuted by %1.</source>
        <translation>Il tuo microfono è stato riattivato da %1.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>%1 muted by %2.</source>
        <translation>Il microfono di %1 è stato disattivato da %2.</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>%1 unmuted by %2.</source>
        <translation>Il microfono di %1 è stato riattivato da %2.</translation>
    </message>
    <message>
        <location line="+17"/>
        <source>You were deafened by %1.</source>
        <translation>Il tuo audio è stato disattivatob da %1.</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>You were undeafened by %1.</source>
        <translation>Il tuo audio è stato riattivato da %1.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>%1 deafened by %2.</source>
        <translation>L&apos;audio di %1 è stato disattivato da %2.</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>%1 undeafened by %2.</source>
        <translation>L&apos;audio di %1 è stato riattivato da %2.</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>You were kicked from the server by %1: %2.</source>
        <translation>Sei stato espulso dal server da %1:%2.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>%3 was kicked from the server by %1: %2.</source>
        <translation>%3 è stato espulso dal server da %1:%2.</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>You were kicked and banned from the server by %1: %2.</source>
        <translation>Sei stato espulso e bandito dal server da %1: %2.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>%3 was kicked and banned from the server by %1: %2.</source>
        <translation>%3 è stato espulto e bandito dal server da %1:%2.</translation>
    </message>
    <message>
        <location line="+27"/>
        <source>You were moved to %1 by %2.</source>
        <translation>Sei stato spostato sul canale %1 da %2.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>%1 left channel.</source>
        <translation>%1 ha lasciato il canale.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1 moved out by %2 to %3.</source>
        <translation>%1 spostato fuori dal canale da %2 su %3.</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>%1 moved in by %2 from %3.</source>
        <translation>%1 spostato dentro al canale da %2 dal canale %3.</translation>
    </message>
    <message>
        <location line="-2"/>
        <source>%1 entered channel.</source>
        <translation>%1 è entrato nel canale.</translation>
    </message>
    <message>
        <location line="+100"/>
        <source>Server connection rejected: %1.</source>
        <translation>Connessione al server respinta: %1.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Denied: %1.</source>
        <translation>Negato: %1.</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Server maximum bandwidth is only %1 kbit/s. Quality auto-adjusted.</source>
        <translation>La banda massima del server è solamente %1 kbit/s. Qualità auto-aggiustata.</translation>
    </message>
    <message>
        <location line="+11"/>
        <source>From %1: %2</source>
        <translation>Da %1: %2</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Message from %1</source>
        <translation>Messaggio da %1</translation>
    </message>
    <message>
        <location filename="PlayerModel.cpp" line="+995"/>
        <source>You have Channel Dragging set to &quot;Do Nothing&quot; so the channel wasn&apos;t moved.</source>
        <translation>Hai selezionato &quot;non fare niente&quot; sulle impostazioni del trascinamento del canale, quindi il canale non sarà spostato.</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Unknown Channel Drag mode in PlayerModel::dropMimeData.</source>
        <translation>Modalità trascianamento canale sconosciuta in PlayerModel::dropMimeData.</translation>
    </message>
    <message>
        <location filename="main.cpp" line="+239"/>
        <source>Welcome to Mumble.</source>
        <translation>Benvenuto su Mumble.</translation>
    </message>
    <message>
        <location line="+17"/>
        <source>This is the first time you&apos;re starting Mumble.&lt;br /&gt;Would you like to go through the Audio Wizard to configure your soundcard?</source>
        <translation>Questa è la prima volta che avvii Mumble.&lt;br /&gt;Vorresti essere aiutato dall&apos;Audio Wizard per configurare la tua scheda audio e le impostazioni audio?</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="-2"/>
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
</context>
<context>
    <name>NetworkConfig</name>
    <message>
        <location filename="NetworkConfig.cpp" line="+51"/>
        <source>Network</source>
        <translation>Rete</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui"/>
        <source>Form</source>
        <translation>Modulo</translation>
    </message>
    <message>
        <location/>
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
        <location/>
        <source>Use TCP mode</source>
        <translation>Usa modalità TCP</translation>
    </message>
    <message>
        <location/>
        <source>Reconnect when disconnected</source>
        <translation>Ricollegati quando ti disconnetti</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Reconnect when disconnected&lt;/b&gt;.&lt;br /&gt;This will make Mumble try to automatically reconnect after 10 seconds if your server connection fails.</source>
        <translation>&lt;b&gt;Riconnetti quando ti disconnetti&lt;/b&gt;.&lt;br /&gt;Mumble proverà a ricollegarsi automaticamente dopo 10 secondi se cade la connessione al server.</translation>
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
        <location/>
        <source>&lt;b&gt;Type of proxy to connect through&lt;/b&gt;&lt;br /&gt;This makes Mumble connect through a proxy for all outgoing connections. Note: Proxy tunneling forces Mumble into TCP compatibility mode, causing all voice data to be sent via the control channel.</source>
        <oldsource>&lt;b&gt;Type of proxy to connect through&lt;/b&gt;&lt;br /&gt;This makes Mumble connect through a proxy for all outgoing connections. Note: Proxy tunneling forces Mumble into TCP compatability mode, causing all voice data to be sent via the control channel.</oldsource>
        <translation>&lt;b&gt;Tipo di proxy tramite cui connettersi&lt;/b&gt;&lt;br /&gt;Ciò fà connettere Mumble attraverso un proxy per tutte le connessioni in uscita. Nota: il proxy tunneling forza Mumble nella modalita di compatibilità TCP, causando l&apos;invio di tutti i dati vocali tramite il canale di controllo.</translation>
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
        <translation>Nome dell&apos;host</translation>
    </message>
    <message>
        <location/>
        <source>Hostname of the proxy</source>
        <translation>Nome dell&apos;host proxy</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Hostname of the proxy.&lt;b&gt;&lt;br /&gt;This field specifies the hostname of the proxy you wish to tunnel network traffic through.</source>
        <translation>&lt;b&gt;Nome dell&apos;host del proxy.&lt;b&gt;&lt;br /&gt;Specifica il nome del proxy attraverso cui vuoi che passi il traffico di rete.</translation>
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
        <source>Mumble services</source>
        <translation>Servizi di Mumble</translation>
    </message>
    <message>
        <location/>
        <source>Check for updates on startup</source>
        <translation>Controlla aggiornamenti all&apos;avvio</translation>
    </message>
    <message>
        <location/>
        <source>Submit anonymous statistics to the Mumble project</source>
        <translation>Invia statistiche anonime al progetto Mumble</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Submit anonymous statistics.&lt;/b&gt;&lt;br /&gt;Mumble has a small development team, and as such needs to focus it&apos;s development where it&apos;s needed most. By submitting a bit of statistics you help the project determine where to focus development.</source>
        <translation>&lt;b&gt;Invia statistiche anonime.&lt;/b&gt;&lt;br /&gt;Mumble ha un piccolo team di sviluppo, e come tale ha bisogno di concentrare lo sviluppo dove è maggiormente necessario. Inviando un po&apos; di statistiche permetti al progetto di stabilire dove concentrare lo sviluppo.</translation>
    </message>
    <message>
        <location/>
        <source>Submit anonymous statistics</source>
        <translation>Invia statistiche anonime</translation>
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
        <translation type="obsolete">Caricamento della libreria di sovrapposizione fallito. Significa che o:
- la libreria (mumble_ol.dll) non è stata trovata nella directory dove stai eseguendo Mumble
- hai un OS precedente a WinXP SP2
- non hai la versione di DX9.0c aggiornata al June 2007</translation>
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
        <source>Overlay</source>
        <translation type="obsolete">Sovrapposizione</translation>
    </message>
    <message>
        <source>Form</source>
        <translation type="obsolete">Modulo</translation>
    </message>
    <message>
        <source>Options</source>
        <translation type="obsolete">Opzioni</translation>
    </message>
    <message>
        <source>Enable overlay.</source>
        <translation type="obsolete">Abilita sovrapposizione.</translation>
    </message>
    <message>
        <source>This sets whether the overlay is enabled or not. This setting is only checked with D3D9 applications are started, so make sure Mumble is running and this option is on before you start the application.&lt;br /&gt;Please note that if you start the application after starting Mumble, or if you disable the overlay while running, there is no safe way to restart the overlay without also restarting the application.</source>
        <translation type="obsolete">Stabilisce se la sovrapposizione è attivata o meno. Questa impostazione è settata solo con applicazioni D3D9 in esecuzione, quindi assicuratevi che Mumble sia in esecuzione e che questa opzione sia attiva prima di avviare l&apos;applicazione.&lt;br /&gt;Si prega di notare che se si avvia l&apos;applicazione dopo l&apos;avvio di Mumble, o se si disattiva la sovrapposizione durante l&apos;esecuzione, non vi è alcun modo sicuro per riavviare la sovrapposizione senza dover riavviare l&apos;applicazione.</translation>
    </message>
    <message>
        <source>Enable Overlay</source>
        <translation type="obsolete">Abilita la sovrapposizione</translation>
    </message>
    <message>
        <source>Who to show on the overlay</source>
        <translation type="obsolete">Chi mostrare nella sovrapposizione</translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets who to show in the in-game overlay.&lt;/b&gt;&lt;br /&gt;If many people are connected to the same channel, the overlay list might be very long. Use this to shorten it.&lt;br /&gt;&lt;i&gt;No one&lt;/i&gt; - Don&apos;t show anyone (but leave overlay running).&lt;br /&gt;&lt;i&gt;Only talking&lt;/i&gt; - Only show talking people.&lt;br /&gt;&lt;i&gt;Everyone&lt;/i&gt; - Show everyone.</source>
        <translation type="obsolete">Stabilisce chi mostrare nella sovrapposizione al gioco.&lt;/b&gt;&lt;br /&gt;Se molte persone sono collegate allo stesso canale, la lista sovrapposta potrebbe essere molto lunga. Usa questo per accorciarla.&lt;br /&gt;&lt;i&gt;Nessuno&lt;/i&gt; - Non mostrare nessuno (ma lascia in esecuzione di sovrapposizione).&lt;br /&gt;&lt;i&gt;Solo chi sta parlando&lt;/i&gt; - Mostra solo chi sta parlando.&lt;br /&gt;&lt;i&gt;Tutti&lt;/i&gt; - Mostra tutti.</translation>
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
        <translation type="obsolete">Fai crescere verso l&apos;alto la sovrapposizione</translation>
    </message>
    <message>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow upwards if needed.</source>
        <translation type="obsolete">Tiene la sovrapposizione la più piccola possibile e alla posizione da te selezionata. Permette alla sovrapposizione di crescere verso l&apos;alto se necessario.</translation>
    </message>
    <message>
        <source>Grow Up</source>
        <translation type="obsolete">Cresce in alto</translation>
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
        <translation type="obsolete">Fai crescere la sovrapposizione verso sinistra</translation>
    </message>
    <message>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow to the left if needed.</source>
        <translation type="obsolete">Tiene la sovrapposizione la più piccola possibile e alla posizione da te selezionata. Permette alla sovrapposizione di crescere verso sinistra se necessario.</translation>
    </message>
    <message>
        <source>Grow Left</source>
        <translation type="obsolete">Cresce a sinistra</translation>
    </message>
    <message>
        <source>Let overlay grow to the right</source>
        <translation type="obsolete">Lascia crescere verso destra</translation>
    </message>
    <message>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow to the right if needed.</source>
        <translation type="obsolete">Tiene la sovrapposizione la più piccola possibile e alla posizione da te selezionata. Permette alla sovrapposizione di crescere verso destra se necessario.</translation>
    </message>
    <message>
        <source>Grow Right</source>
        <translation type="obsolete">Cresce a destra</translation>
    </message>
    <message>
        <source>Let overlay grow downwards</source>
        <translation type="obsolete">Fai crescere la sovrapposizione verso il basso</translation>
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
        <source>Font</source>
        <translation type="obsolete">Carattere</translation>
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
</context>
<context>
    <name>PlayerModel</name>
    <message>
        <location filename="PlayerModel.cpp" line="-581"/>
        <source>This is a player connected to the server. The icon to the left of the player indicates whether or not they are talking:&lt;br /&gt;&lt;img src=&quot;skin:talking_on.png&quot; /&gt; Talking&lt;br /&gt;&lt;img src=&quot;skin:talking_off.png&quot; /&gt; Not talking</source>
        <translation>Giocatore connesso al server. L&apos;icona alla sinistra del giocatore indica se sta parlando:&lt;br /&gt;&lt;img src=&quot;skin:talking_on.png&quot; /&gt;Parla&lt;br /&gt;&lt;img src=&quot;skin:talking_off.png&quot; /&gt;In silenzio</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>This is a channel on the server. Only players in the same channel can hear each other.</source>
        <translation>Canale sul server. Solamente i giocatori nello stesso canale potranno sentirsi tra loro.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>This shows the flags the player has on the server, if any:&lt;br /&gt;&lt;img src=&quot;skin:authenticated.png&quot; /&gt;Authenticated user&lt;br /&gt;&lt;img src=&quot;skin:muted_self.png&quot; /&gt;Muted (by self)&lt;br /&gt;&lt;img src=&quot;skin:muted_server.png&quot; /&gt;Muted (by admin)&lt;br /&gt;&lt;img src=&quot;skin:deafened_self.png&quot; /&gt;Deafened (by self)&lt;br /&gt;&lt;img src=&quot;skin:deafened_server.png&quot; /&gt;Deafened (by admin)&lt;br /&gt;A player muted by himself is probably just away, talking on the phone or something like that.&lt;br /&gt;A player muted by an admin is probably also just away, and the noise the player is making was annoying enough that an admin muted him.</source>
        <translation>Mostra le impostazioni che il giocatore ha sul server, se ne ha:&lt;br /&gt;&lt;img src=&quot;skin:authenticated.png&quot; /&gt;Utente Autenticato&lt;br /&gt;&lt;img src=&quot;skin:muted_self.png&quot; /&gt;Silenziato (Auto)&lt;br /&gt; &lt;img src=&quot;skin:muted_server.png&quot; /&gt;Silenziato (da admin)&lt;br /&gt;&lt;img src=&quot;skin:deafened_self.png&quot; /&gt; Escluso (Auto)&lt;br /&gt;&lt;img src=&quot;skin:deafened_server.png&quot; /&gt;Isolato (da admin)&lt;br /&gt;Un giocatore autosilenziato probabilmente è solo assente, sta parlando al telefono o qualcosa di simile.&lt;br /&gt;Un giocatore silenziato da un amministratore probabilmente anche lui assente, e il rumore che il giocatore stava facendo era abbastanza fastidioso che un amministratore l&apos;ha silenziato.</translation>
    </message>
    <message>
        <location line="+24"/>
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
        <location line="+535"/>
        <source>Are you sure you want to drag this channel?</source>
        <translation>Sei sicuro di voler spostare questo canale?</translation>
    </message>
</context>
<context>
    <name>PluginConfig</name>
    <message>
        <location filename="Plugins.ui"/>
        <location filename="Plugins.cpp" line="+64"/>
        <source>Plugins</source>
        <translation>Plugins</translation>
    </message>
    <message>
        <source>Mumble</source>
        <translation type="obsolete">Mumble</translation>
    </message>
    <message>
        <location filename="Plugins.cpp" line="+28"/>
        <source>Plugin has no configure function.</source>
        <translation>Il Plugins non ha funzioni di configurazione.</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>Plugin has no about function.</source>
        <translation>Il Plugins non ha funzioni di informazione.</translation>
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
        <location/>
        <source>This enables plugins for supported games to fetch your in-game position and transmit that with each voice packet. This enables other players to hear your voice in-game from the direction your character is in relation to their own.</source>
        <translation>Abilita i plugins per i giochi supportati a ottenere la tua posizione nel gioco e la trasmette con ogni pacchetto voce. Questo consente agli altri giocatori di sentire la tua voce dalla direzione con cui il tuo personaggio è in relazione alla loro.</translation>
    </message>
    <message>
        <location/>
        <source>Link to Game and Transmit Position</source>
        <translation>Collegato al Gioco e Trasmetti la Posizione</translation>
    </message>
    <message>
        <location/>
        <source>Reloads all plugins</source>
        <translation>Ricarica tutti i plugins</translation>
    </message>
    <message>
        <location/>
        <source>This rescans and reloads plugins. Use this if you just added or changed a plugin to the plugins directory.</source>
        <translation>Ricerca e ricarica tutti i plugins. Usare se hai appena aggiunto o cambiato un plugin nella directory dei plugins.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Reload plugins</source>
        <translation>&amp;Ricarica i plugins</translation>
    </message>
    <message>
        <location/>
        <source>Information about plugin</source>
        <translation>Informazioni circa il plugin</translation>
    </message>
    <message>
        <location/>
        <source>This shows a small information message about the plugin.</source>
        <translation>Mostra qualche informazione riguardo al plugin.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;About</source>
        <translation>&amp;Circa</translation>
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
</context>
<context>
    <name>Plugins</name>
    <message>
        <location filename="Plugins.cpp" line="+127"/>
        <source>%1 lost link.</source>
        <translation>%1 collegamento perso.</translation>
    </message>
    <message>
        <location line="+39"/>
        <source>%1 linked.</source>
        <translation>%1 collegato.</translation>
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
        <translation>Input predefinito</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Default Output</source>
        <translation>Output predefinito</translation>
    </message>
</context>
<context>
    <name>ShortcutActionWidget</name>
    <message>
        <location filename="GlobalShortcut.cpp" line="-143"/>
        <source>Unassigned</source>
        <translation>Non asseganto</translation>
    </message>
</context>
<context>
    <name>ShortcutDelegate</name>
    <message>
        <location line="+58"/>
        <source>Unassigned</source>
        <translation>Non asseganto</translation>
    </message>
</context>
<context>
    <name>ShortcutKeyWidget</name>
    <message>
        <location line="-122"/>
        <source>Press Shortcut</source>
        <translation>Premi scorciatoia</translation>
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
        <location filename="TextMessage.ui"/>
        <source>Message to send</source>
        <translation>Messaggio da mandare</translation>
    </message>
    <message>
        <location/>
        <source>Preview</source>
        <translation>Anteprima</translation>
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
        <source>Mumble failed to retrieve version information from the SourceForge server.</source>
        <translation type="obsolete">Mumble non è riuscito a recuperare le informazioni sull&apos;ultima versione dal server SourceForge.</translation>
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
        <translation>Inviato da:</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Unit Name: %1</source>
        <translation>Nome Unità: %1</translation>
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
