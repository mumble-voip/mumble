<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.0" language="fr">
<context>
    <name>ACLEditor</name>
    <message>
        <source>Mumble - Edit ACL for %1</source>
        <translation type="obsolete">Mumble - Editer la LCA pour %1</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui"/>
        <source>Active ACLs</source>
        <translation>LCAs actifs</translation>
    </message>
    <message>
        <location/>
        <source>Context</source>
        <translation>Contexte</translation>
    </message>
    <message>
        <location/>
        <source>User/Group</source>
        <translation>Utilisateur/Groupe</translation>
    </message>
    <message>
        <source>This controls which group of users this entry applies to.&lt;br /&gt;Note that the group is evaluated in the context of the channel the entry is used in. For example, the default ACL on the Root channel gives &lt;i&gt;Write&lt;/i&gt; permission to the &lt;i&gt;admin&lt;/i&gt; group. This entry, if inherited by a channel, will give a user write privileges if he belongs to the &lt;i&gt;admin&lt;/i&gt; group in that channel, even if he doesn&apos;t belong to the &lt;i&gt;admin&lt;/i&gt; group in the channel where the ACL originated.&lt;br /&gt;If a group name starts with a &apos;!&apos;, its membership is negated, and if it starts with a &apos;~&apos;, it is evaluated in the channel the ACL was defined in, rather than the channel the ACL is active in.&lt;br /&gt;If a group name starts with a  &apos;#&apos;, it is interpreted as an access token. Users must have entered whatever follows the &apos;#&apos; in their list of access tokens to match. This can be used for very simple password access to channels for non-authenticated users.&lt;br /&gt;If a group name starts with a  &apos;$&apos;, it will only match users whose certificate hash matches what follows the &apos;$&apos;.&lt;br /&gt;A few special predefined groups are:&lt;br /&gt;&lt;b&gt;all&lt;/b&gt; - Everyone will match.&lt;br /&gt;&lt;b&gt;auth&lt;/b&gt; - All authenticated users will match.&lt;br /&gt;&lt;b&gt;sub,a,b,c&lt;/b&gt; - User currently in a sub-channel minimum &lt;i&gt;a&lt;/i&gt; common parents, and between &lt;i&gt;b&lt;/i&gt; and &lt;i&gt;c&lt;/i&gt; channels down the chain. See the website for more extensive documentation on this one.&lt;br /&gt;&lt;b&gt;in&lt;/b&gt; - Users currently in the channel will match (convenience for &apos;&lt;i&gt;sub,0,0,0&lt;/i&gt;&apos;).&lt;br /&gt;&lt;b&gt;out&lt;/b&gt; - Users outside the channel will match (convenience for &apos;&lt;i&gt;!sub,0,0,0&lt;/i&gt;&apos;).&lt;br /&gt;Note that an entry applies to either a user or a group, not both.</source>
        <translation type="obsolete">Contrôle à quel groupe d&apos;utilisateurs cette entrée s&apos;applique.&lt;br /&gt;Il est à noter que les groupes sont évalués dans le contexte du salon dans lequel l&apos;entrée est utilisée Par exemple, la LCA du salon racine par défaut donne la permission d&apos;&lt;i&gt;écriture&lt;/i&gt; au groupe &lt;i&gt;admin&lt;/i&gt;. Cette entrée, si héritée par un salon, donnera à l&apos;utilisateur le privilège d&apos;écriture s&apos;il appartient au groupe &lt;i&gt;admin&lt;/i&gt; du salon, même s&apos;il n&apos;appartient pas au groupe &lt;i&gt;admin&lt;/i&gt; dans le salon de la LCA originelle.&lt;br /&gt;Si le nom d&apos;un groupe commence par &apos;!&apos;, son appartenance est négative, et s&apos;il commence par un &apos;~&apos;, il est évalué dans le salon dans lequel la LCA a été définie, plutôt que dans le salon dans lequel la LCA est active.&lt;br /&gt;Si le nom d&apos;un groupe commence par &apos;#&apos;, son contenu sera interprété comme un jeton d&apos;accès. Les utilisateurs doivent saisir ce qui suit le &apos;#&apos; dans leur liste de jetons d&apos;accès pour être valide. Cette méthode peut être utilisée pour un accès simple par mots de passe pour les utilisateurs non authentifiés.&lt;br /&gt;Si le nom d&apos;un groupe commence par &apos;$&apos;, seulement les utilisateurs dont le hash du certificat, qui doit suivre le &apos;$&apos;, seront valides.&lt;br /&gt;Quelques groupes prédéfinis sont :&lt;br /&gt;&lt;b&gt;all&lt;/b&gt; - Tout le monde est concerné.&lt;br /&gt;&lt;b&gt;auth&lt;/b&gt; - Tous les utilisateurs authentifiés&lt;br /&gt;&lt;b&gt;sub,a,b,c&lt;/b&gt; - Utilisateurs actuellement dans un sous-salon avec au moins &lt;i&gt;a&lt;/i&gt; parents commun, entre les salons &lt;i&gt;b&lt;/i&gt; et &lt;i&gt;c&lt;/i&gt;. Voir le site web pour une explication plus complète.&lt;br /&gt;&lt;b&gt;in&lt;/b&gt; - Utilisateurs dans le salon (raccourci pour &lt;i&gt;sub,0,0,0&lt;/i&gt;)&lt;br /&gt;&lt;b&gt;out&lt;/b&gt; - Utilisateurs en dehors du salon (raccourci pour &lt;i&gt;!sub,0,0,0&lt;/i&gt;).&lt;br /&gt;Il est à noter que ces entrées s&apos;appliquent soit à un utilisateur, soit à un groupe, mais pas aux deux.</translation>
    </message>
    <message>
        <location/>
        <source>Permissions</source>
        <translation>Permissions</translation>
    </message>
    <message>
        <location/>
        <source>Group</source>
        <translation>Groupe</translation>
    </message>
    <message>
        <location/>
        <source>Members</source>
        <translation>Membres</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Add</source>
        <translation>&amp;Ajouter</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Remove</source>
        <translation>Supp&amp;rimer</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Up</source>
        <translation>Ha&amp;ut</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Down</source>
        <translation>&amp;Bas</translation>
    </message>
    <message>
        <location/>
        <source>Inherit ACLs</source>
        <translation>Hérite des LCAs</translation>
    </message>
    <message>
        <location/>
        <source>Applies to this channel</source>
        <translation>Appliquer à ce salon</translation>
    </message>
    <message>
        <location/>
        <source>Applies to sub-channels</source>
        <translation>Appliquer aux sous-salons</translation>
    </message>
    <message>
        <location/>
        <source>User ID</source>
        <translation>ID Utilisateur</translation>
    </message>
    <message>
        <location filename="ACLEditor.cpp" line="+101"/>
        <source>Deny</source>
        <translation>Refuser</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Allow</source>
        <translation>Accorder</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui"/>
        <source>Remove</source>
        <translation>Supprimer</translation>
    </message>
    <message>
        <location/>
        <source>Inherit</source>
        <translation>Hérite</translation>
    </message>
    <message>
        <location/>
        <source>Inheritable</source>
        <translation>Héritable</translation>
    </message>
    <message>
        <location/>
        <source>Inherited</source>
        <translation>Hérité</translation>
    </message>
    <message>
        <location/>
        <source>Add</source>
        <translation>Ajouter</translation>
    </message>
    <message>
        <source>Add to Remove</source>
        <translation type="obsolete">Ajouter à la liste de suppression</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Groups</source>
        <translation>&amp;Groupes</translation>
    </message>
    <message>
        <location/>
        <source>&amp;ACL</source>
        <translation>&amp;LCA</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Valider</translation>
    </message>
    <message>
        <source>Accept changes</source>
        <translation type="obsolete">Accepter les modifications</translation>
    </message>
    <message>
        <source>This button will accept current groups/ACLs and send them to the server. Note that if you mistakenly remove write permission from yourself, the server will add it.</source>
        <translation type="obsolete">Ce bouton va accepter les groupes/LCAs actuels et les envoyer au serveur. Il est à noter que si vous enlevez par erreur votre permission d&apos;écriture, le serveur va l&apos;ajouter.</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">A&amp;nnuler</translation>
    </message>
    <message>
        <source>Reject changes</source>
        <translation type="obsolete">Annuler les modifications</translation>
    </message>
    <message>
        <source>This button will cancel all changes and closes the dialog without updating the ACLs or groups on the server.</source>
        <translation type="obsolete">Ce bouton va annuler toutes les modifications et fermer la fenêtre sans mettre à jour les LCAs ou groupes sur le serveur.</translation>
    </message>
    <message>
        <location/>
        <source>List of entries</source>
        <translation>Liste des entrées</translation>
    </message>
    <message>
        <source>This shows all the entries active on this channel. Entries inherited from parent channels will be show in italics.</source>
        <translation type="obsolete">Montrer toutes les entrées actives sur ce salon. Les entrées héritées des salons parents seront montrées en italique.</translation>
    </message>
    <message>
        <location/>
        <source>Inherit ACL of parent?</source>
        <translation>Hériter la LCA du parent ?</translation>
    </message>
    <message>
        <location/>
        <source>This sets whether or not the ACL up the chain of parent channels are applied to this object. Only those entries that are marked in the parent as &quot;Apply to sub-channels&quot; will be inherited.</source>
        <translation>Définit si la LCA en haut de la chaîne des salons parents s&apos;applique à cet objet ou non. Seules les entrées qui sont marquées dans les parents comme &quot;Appliquer au sous-salons&quot; seront hérités.</translation>
    </message>
    <message>
        <location/>
        <source>Add new entry</source>
        <translation>Ajouter une nouvelle entrée</translation>
    </message>
    <message>
        <location/>
        <source>This adds a new entry, initially set with no permissions and applying to all.</source>
        <translation>Ajoute une nouvelle entrée, initialement définie avec aucune permission, et s&apos;appliquant à tous.</translation>
    </message>
    <message>
        <location/>
        <source>Remove entry</source>
        <translation>Supprimer une entrée</translation>
    </message>
    <message>
        <location/>
        <source>This removes the currently selected entry.</source>
        <translation>Supprime l&apos;entrée actuellement sélectionnée.</translation>
    </message>
    <message>
        <location/>
        <source>Move entry up</source>
        <translation>Déplacer l&apos;entrée vers le haut</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation type="obsolete">Propriétés</translation>
    </message>
    <message>
        <location/>
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <location/>
        <source>Enter the channel name here.</source>
        <translation>Saisir le nom du salon ici.</translation>
    </message>
    <message>
        <source>&lt;b&gt;Name&lt;/b&gt;&lt;br&gt;Enter the channel name in this field. The name has to comply with the restriction imposed by the server you are connected to.</source>
        <translation type="obsolete">&lt;b&gt;Nom&lt;/b&gt;&lt;br/&gt;Saisir le nom du salon dans ce champ. Le nom doit se conformer aux restrictions imposées par le serveur auquel vous êtes connecté.</translation>
    </message>
    <message>
        <location/>
        <source>Description</source>
        <translation>Description</translation>
    </message>
    <message>
        <location/>
        <source>Password</source>
        <translation>Mot de passe</translation>
    </message>
    <message>
        <location/>
        <source>Enter the channel password here.</source>
        <translation>Saisir le mot de passe du salon ici.</translation>
    </message>
    <message>
        <source>&lt;b&gt;Password&lt;/b&gt;&lt;br&gt;This field allows you to easily set and change the password of a channel. In the background it uses Mumble&apos;s access tokens feature. To allow more fine grained and powerful access control directly use ACLs and groups instead (&lt;i&gt;Advanced configuration&lt;/i&gt; has to be checked to be able to see these settings).</source>
        <translation type="obsolete">&lt;b&gt;Mot de passe&lt;/b&gt;&lt;br/&gt;Ce champ vous permet de définir ou de changer le mot de passe du salon. Il utilise les fonctions de jetons d&apos;accès de Mumble en arrière plan. Pour définir plus finement et plus puissamment les droits d&apos;accès, utilisez directement les LCA et groupes à la place (&lt;i&gt;Configuration avancée&lt;/i&gt; doit être coché pour afficher ces paramètres).</translation>
    </message>
    <message>
        <location/>
        <source>Check to create a temporary channel.</source>
        <translation>Cocher pour créer un salon temporaire.</translation>
    </message>
    <message>
        <source>&lt;b&gt;Temporary&lt;/b&gt;&lt;br&gt;
When checked the channel created will be marked as temporary. This means when the last player leaves it the channel will be automatically deleted by the server.</source>
        <translation type="obsolete">&lt;b&gt;Temporaire&lt;/b&gt;&lt;br&gt;
Quand cette case est cochée, le salon ainsi créé sera marqué temporaire. En conséquence, le salon sera automatiquement supprimé dès que le dernier utilisateur l&apos;aura quitté.</translation>
    </message>
    <message>
        <location/>
        <source>Temporary</source>
        <translation>Temporaire</translation>
    </message>
    <message>
        <location/>
        <source>Channel positioning facility value</source>
        <translation>Valeur de positionnement du salon</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Position&lt;/b&gt;&lt;br/&gt;
This value enables you to change the way Mumble arranges the channels in the tree. A channel with a higher &lt;i&gt;Position&lt;/i&gt; value will always be placed below one with a lower value and the other way around. If the &lt;i&gt;Position&lt;/i&gt; value of two channels is equal they will get sorted alphabetically by their name.</source>
        <oldsource>&lt;b&gt;Position&lt;/b&gt;&lt;br/&gt;
This value enables you to change the way mumble arranges the channels in the tree. A channel with a higher &lt;i&gt;Position&lt;/i&gt; value will always be placed below one with a lower value and the other way around. If the &lt;i&gt;Position&lt;/i&gt; value of two channels is equal they will get sorted alphabetically by their name.</oldsource>
        <translation>&lt;b&gt;Position&lt;/b&gt;&lt;br/&gt;
Cette valeur vous permet de définir comment Mumble arrange les salons dans l&apos;arborescence. Un salon avec une valeur de &lt;b&gt;Position&lt;/b&gt; plus grance sera toujours placé en dessous de ceux qui ont une valeur plus petite, et inversement. Si la valeur de &lt;b&gt;Position&lt;/b&gt; est identique entre deux salons, ceux-ci seront trié par ordre alphabétique à partir de leur noms.</translation>
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
        <translation type="obsolete">&lt;b&gt;Groupe&lt;/b&gt;&lt;br/&gt;
Ce sont tous les groupes actuellement définis pour ce salon. Pour créer un nouveau groupe, il suffit de saisir un nom et d&apos;appuyer sur Entrée.</translation>
    </message>
    <message>
        <location/>
        <source>Add new group</source>
        <translation>Ajouter un nouveau groupe</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Add&lt;/b&gt;&lt;br/&gt;
Add a new group.</source>
        <translation>&lt;b&gt;Ajouter&lt;/b&gt;&lt;br/&gt;
Ajouter un nouveau groupe.</translation>
    </message>
    <message>
        <source>&lt;b&gt;Remove&lt;/b&gt;&lt;br&gt;This removes the currently selected group. If the group was inherited, it will not be removed from the list, but all local information about the group will be cleared.</source>
        <translation type="obsolete">&lt;b&gt;Supprimer&lt;/b&gt;&lt;br/&gt;Efface le groupe actuellement sélectionné. Si le groupe était hérité, il ne sera pas retiré de la liste, mais toutes les modifications locales du groupe seront effacées.</translation>
    </message>
    <message>
        <source>&lt;b&gt;Inherit&lt;/b&gt;&lt;br&gt;This inherits all the members in the group from the parent, if the group is marked as &lt;i&gt;Inheritable&lt;/i&gt; in the parent channel.</source>
        <translation type="obsolete">&lt;b&gt;Hérite&lt;/b&gt;&lt;br/&gt;Permet à tous les membres du groupe dêtre hérités du groupe parent, si le groupe est marqué comme étant &lt;i&gt;Héritable&lt;i&gt; dans le salon parent.</translation>
    </message>
    <message>
        <source>&lt;b&gt;Inheritable&lt;/b&gt;&lt;br&gt;This makes this group inheritable to sub-channels. If the group is non-inheritable, sub-channels are still free to create a new group with the same name.</source>
        <translation type="obsolete">&lt;b&gt;Héritable&lt;/b&gt;&lt;br/&gt;
Indique que ce groupe peut être hérité par les sous salons. Si le groupe est non héritable, les sous salons sont libres de créer un nouveau groupe avec le même nom.</translation>
    </message>
    <message>
        <source>&lt;b&gt;Inherited&lt;/b&gt;&lt;br&gt;This indicates that the group was inherited from the parent channel. You cannot edit this flag, it&apos;s just for information.</source>
        <translation type="obsolete">&lt;b&gt;Hérité&lt;/b&gt;&lt;br/&gt;Indique que le groupe est hérité du salon parent. Vous ne pouvez pas modifier cette option, elle n&apos;est qu&apos;informative.</translation>
    </message>
    <message>
        <location/>
        <source>Inherited members</source>
        <translation>Membres hérités</translation>
    </message>
    <message>
        <location/>
        <source>Contains the list of members added to the group by this channel.</source>
        <translation>Contient la liste des membres ajoutés au groupe par ce salon.</translation>
    </message>
    <message>
        <source>&lt;b&gt;Members&lt;/b&gt;&lt;br&gt;
This list contains all members that were added to the group by the current channel. Be aware that this does not include members inherited by higher levels of the channel tree. These can be found in the &lt;i&gt;Inherited members&lt;/i&gt; list. To prevent this list to be inherited by lower level channels uncheck &lt;i&gt;Inheritable&lt;/i&gt; or manually add the members to the &lt;i&gt;Excluded members&lt;/i&gt; list.</source>
        <translation type="obsolete">&lt;b&gt;Membres&lt;/&gt;&lt;br/&gt;
Cette liste contient tous les membres ajoutés au groupe dans le salon actuel. Notez que celà n&apos;inclut pas les membres hérités par les salons de niveaux suppérieurs. Ceux-ci peuvent être trouvés dans la liste des &lt;i&gt;Membres hérités&lt;/i&gt;. Pour éviter que cette liste ne soit héritée par les sous salons, décochez &lt;i&gt;Héritable&lt;/i&gt; ou ajouter manuellement les membres à la liste des &lt;i&gt;Membres exclus&lt;/i&gt;.</translation>
    </message>
    <message>
        <location/>
        <source>Contains a list of members whose group membership will not be inherited from the parent channel.</source>
        <translation>Contient la liste des membres dont l&apos;appartenance au groupe ne doit pas être hérité depuis le salon parent.</translation>
    </message>
    <message>
        <source>&lt;b&gt;Excluded members&lt;/b&gt;&lt;br&gt;
Contains a list of members whose group membership will not be inherited from the parent channel.</source>
        <translation type="obsolete">&lt;b&gt;Membres exclus&lt;/b&gt;&lt;br/&gt;
Contient une liste de membres dont l&apos;appartenance au groupe ne doit pas être hérité du salon parent.</translation>
    </message>
    <message>
        <location/>
        <source>Contains the list of members inherited by other channels.</source>
        <translation>Contient une liste des membres hérités depuis d&apos;autres salons.</translation>
    </message>
    <message>
        <source>&lt;b&gt;Inherited members&lt;/b&gt;&lt;br&gt;
Contains the list of members inherited by the current channel. Uncheck &lt;i&gt;Inherit&lt;/i&gt; to prevent inheritance from higher level channels.</source>
        <translation type="obsolete">&lt;b&gt;Membres hérités&lt;/b&gt;&lt;br /&gt;
Contient la liste des membres hérités par le salon actuel. Décochez &lt;i&gt;Hérite&lt;/i&gt; pour empêcher l&apos;héritage depuis un salon de niveau supérieur.</translation>
    </message>
    <message>
        <location/>
        <source>Type in the name of a user you wish to add to the group and click Add.</source>
        <translation>Saisir le nom de l&apos;utilisateur que vous souhaitez ajouter au groupe et cliquez sur Ajouter.</translation>
    </message>
    <message>
        <location/>
        <source>Type in the name of a user you wish to remove from the group and click Add.</source>
        <translation>Saisir le nom de l&apos;utilisateur que vous souhaitez retirer du groupe et cliquez sur Ajouter.</translation>
    </message>
    <message>
        <location/>
        <source>Exclude</source>
        <translation>Exclure</translation>
    </message>
    <message>
        <location/>
        <source>Excluded members</source>
        <translation>Membres exclus</translation>
    </message>
    <message>
        <location/>
        <source>This moves the entry up in the list. As entries are evaluated in order, this may change the effective permissions of users. You cannot move an entry above an inherited entry, if you really need that you&apos;ll have to duplicate the inherited entry.</source>
        <translation>Déplace une entrée vers le haut dans la liste. Comme les entrées sont évaluées dans l&apos;ordre, cela peut changer la permission effective des utilisateurs. Vous ne pouvez pas déplacer une entrée héritée, si vous le nécessitez vraiment, vous aurez à dupliquer l&apos;entrée héritée.</translation>
    </message>
    <message>
        <location/>
        <source>Move entry down</source>
        <translation>Déplacer une entrée vers le bas</translation>
    </message>
    <message>
        <location/>
        <source>This moves the entry down in the list. As entries are evaluated in order, this may change the effective permissions of users.</source>
        <translation>Déplace une entrée vers le bas dans la liste. Comme les entrées sont évaluées dans l&apos;ordre, ceci peut changer les permissions effectives des utilisateurs.</translation>
    </message>
    <message>
        <location/>
        <source>Entry should apply to this channel.</source>
        <oldsource>Entry should apply to this channel</oldsource>
        <translation>Les entrées devraient s&apos;appliquer à ce salon.</translation>
    </message>
    <message>
        <location/>
        <source>This makes the entry apply to this channel.</source>
        <translation>Applique l&apos;entrée à ce salon.</translation>
    </message>
    <message>
        <location/>
        <source>Entry should apply to sub-channels.</source>
        <translation>L&apos;entrée devrait s&apos;appliquer aux sous-salons.</translation>
    </message>
    <message>
        <location/>
        <source>This makes the entry apply to sub-channels of this channel.</source>
        <translation>Applique l&apos;entrée aux sous-salons de ce salon.</translation>
    </message>
    <message>
        <location/>
        <source>Group this entry applies to</source>
        <translation>Le groupe sur lequel cette entrée s&apos;applique</translation>
    </message>
    <message>
        <source>This controls which group of users this entry applies to.&lt;br /&gt;Note that the group is evaluated in the context of the channel the entry is used in. For example, the default ACL on the Root channel gives &lt;i&gt;Write&lt;/i&gt; permission to the &lt;i&gt;admin&lt;/i&gt; group. This entry, if inherited by a channel, will give a user write privileges if he belongs to the &lt;i&gt;admin&lt;/i&gt; group in that channel, even if he doesn&apos;t belong to the &lt;i&gt;admin&lt;/i&gt; group in the channel where the ACL originated.&lt;br /&gt;If a group name starts with a !, its membership is negated, and if it starts with a ~, it is evaluated in the channel the ACL was defined in, rather than the channel the ACL is active in. Order is important; &lt;i&gt;!~in&lt;/i&gt; is valid, but &lt;i&gt;~!in&lt;/i&gt; is not.&lt;br /&gt;A few special predefined groups are:&lt;br /&gt;&lt;b&gt;all&lt;/b&gt; - Everyone will match.&lt;br /&gt;&lt;b&gt;auth&lt;/b&gt; - All authenticated users will match.&lt;br /&gt;&lt;b&gt;sub,a,b,c&lt;/b&gt; - User currently in a sub-channel minimum &lt;i&gt;a&lt;/i&gt; common parents, and between &lt;i&gt;b&lt;/i&gt; and &lt;i&gt;c&lt;/i&gt; channels down the chain. See the website for more extensive documentation on this one.&lt;br /&gt;&lt;b&gt;in&lt;/b&gt; - Users currently in the channel will match (convenience for &lt;i&gt;sub,0,0,0&lt;/i&gt;).&lt;br /&gt;&lt;b&gt;out&lt;/b&gt; - Users outside the channel will match (convenience for &lt;i&gt;!sub,0,0,0&lt;/i&gt;).&lt;br /&gt;Note that an entry applies to either a user or a group, not both.</source>
        <translation type="obsolete">Contrôle à quel groupe d&apos;utilisateurs cette entrée s&apos;applique.&lt;br /&gt;Il est à noter que les groupes sont évalués dans le contexte du salon dans lequel l&apos;entrée est utilisée Par exemple, la LCA du salon racine par défaut donne la permission d&apos;&lt;i&gt;écriture&lt;/i&gt; au groupe &lt;i&gt;admin&lt;/i&gt;. Cette entrée, si héritée par un salon, donnera à l&apos;utilisateur le privilège d&apos;écriture s&apos;il appartient au groupe &lt;i&gt;admin&lt;/i&gt; du salon, même s&apos;il n&apos;appartient pas au groupe &lt;i&gt;admin&lt;/i&gt; dans le salon de la LCA originelle.&lt;br /&gt;Si le nom d&apos;un groupe commence avec !, son appartenance est négative, et s&apos;il commence par un ~, il est évalué dans le salon dans lequel la LCA a été définie, plutôt que dans le salon dans lequel la LCA est active. L&apos;ordre est important; &lt;i&gt;!~in&lt;/i&gt; est valide, mais &lt;i&gt;~!in&lt;/i&gt; ne l&apos;est pas.&lt;br /&gt;Quelques groupes prédéfinis sont :&lt;br /&gt;&lt;b&gt;all&lt;/b&gt; - Tout le monde est concerné.&lt;br /&gt;&lt;b&gt;auth&lt;/b&gt; - Tous les utilisateurs autenthifiés&lt;br /&gt;&lt;b&gt;sub,a,b,c&lt;/b&gt; - Utilisateurs actuellement dans un sous-salon avec au moins &lt;i&gt;a&lt;/i&gt; parents commun, entre les salons &lt;i&gt;b&lt;/i&gt; et &lt;i&gt;c&lt;/i&gt;. Voire le site web pour une explication plus complète.&lt;br /&gt;&lt;b&gt;in&lt;/b&gt; - Utilisateurs dans le salon ( &lt;i&gt;sub,0,0,0&lt;/i&gt; )&lt;br /&gt;&lt;b&gt;out&lt;/b&gt; - Utilisateurs en dehors du salon ( &lt;i&gt;!sub,0,0,0&lt;/i&gt; ).&lt;br /&gt;Il est à noter que ces entrées s&apos;appliquent soit à un utilisateur ou à un groupe, mais pas aux deux.</translation>
    </message>
    <message>
        <location/>
        <source>User this entry applies to</source>
        <translation>Utilisateur sur lequel cette entrée s&apos;applique</translation>
    </message>
    <message>
        <location/>
        <source>This controls which user this entry applies to. Just type in the user name and hit enter to query the server for a match.</source>
        <translation>Contrôle sur quel utilisateur cette entrée s&apos;applique. Tapez simplement le nom de l&apos;utilisateur et pressez entrée pour interroger le serveur.</translation>
    </message>
    <message>
        <location filename="ACLEditor.cpp" line="+21"/>
        <source>Allow %1</source>
        <translation>Permettre %1</translation>
    </message>
    <message>
        <location line="-8"/>
        <source>Deny %1</source>
        <translation>Refuser %1</translation>
    </message>
    <message>
        <location line="-62"/>
        <source>Mumble - Add channel</source>
        <translation>Mumble - Ajouter un salon</translation>
    </message>
    <message>
        <location line="+22"/>
        <location line="+165"/>
        <source>Failed: Invalid channel</source>
        <translation>Échec: Salon invalide</translation>
    </message>
    <message>
        <location line="-153"/>
        <source>Mumble - Edit %1</source>
        <translation>Mumble - Édite %1</translation>
    </message>
    <message>
        <location line="+37"/>
        <source>This grants the %1 privilege. If a privilege is both allowed and denied, it is denied.&lt;br /&gt;%2</source>
        <translation>Accorde le privilège %1. Si un privilège est à la fois accordé et refusé, il est refusé.&lt;br /&gt;%2</translation>
    </message>
    <message>
        <location line="-8"/>
        <source>This revokes the %1 privilege. If a privilege is both allowed and denied, it is denied.&lt;br /&gt;%2</source>
        <translation>Retirer le privilège %1. Si le privilège est à la fois accordé et refusé, il est refusé.&lt;br /&gt;%2</translation>
    </message>
    <message>
        <location filename="ACLEditor.ui"/>
        <source>List of groups</source>
        <translation>Liste des groupes</translation>
    </message>
    <message>
        <source>This is all the groups currently defined for the channel. To create a new group, just type in the name and press enter.</source>
        <translation type="obsolete">Tous les groupes actuellements définis pour le salon. Pour créer un groupe, tappez simplement son nom et appuyez sur enter.</translation>
    </message>
    <message>
        <location/>
        <source>Remove selected group</source>
        <translation>Supprime le groupe sélectionné</translation>
    </message>
    <message>
        <source>This removes the currently selected group. If the group was inherited, it will not be removed from the list, but all local information about the group will be cleared.</source>
        <translation type="obsolete">Supprime le groupe actuellement sélectionné. Si le groupe était hérité, il ne va pas être supprimé de la liste, mais toutes les informations le concernant seront effacées.</translation>
    </message>
    <message>
        <location/>
        <source>Inherit group members from parent</source>
        <translation>Hériter les groupes du parent</translation>
    </message>
    <message>
        <source>This inherits all the members in the group from the parent, if the group is marked as &lt;i&gt;Inheritable&lt;/i&gt; in the parent channel.</source>
        <translation type="obsolete">Tous les membres du groupe héritent du parent, si le groupe est marqué comme &lt;i&gt;Héritable&lt;/i&gt; dans le salon parant.</translation>
    </message>
    <message>
        <location/>
        <source>Make group inheritable to sub-channels</source>
        <translation>Rendre le groupe héritable pour les sous-salons</translation>
    </message>
    <message>
        <source>This makes this group inheritable to sub-channels. If the group is non-inheritable, sub-channels are still free to create a new group with the same name.</source>
        <translation type="obsolete">Rend ce groupe héritable pour les sous-salons. Si un groupe n&apos;est pas héritable, ses sous-salons seront toujours libre de créer un nouveau groupe avec le même nom.</translation>
    </message>
    <message>
        <location/>
        <source>Group was inherited from parent channel</source>
        <translation>Le groupe a hérité du salon parent</translation>
    </message>
    <message>
        <source>This indicates that the group was inherited from the parent channel. You cannot edit this flag, it&apos;s just for information.</source>
        <translation type="obsolete">Indique que le groupe a hérité du salon parent. Vous ne pouvez pas éditer ce fanion, ce n&apos;est qu&apos;une information.</translation>
    </message>
    <message>
        <location/>
        <source>Add member to group</source>
        <translation>Ajouter un membre au groupe</translation>
    </message>
    <message>
        <source>Type in the name of a player you wish to add to the group and press enter.</source>
        <translation type="obsolete">Tappez le nom du joueur que vous souhaitez ajouter au groupe et appuyez sur entrée.</translation>
    </message>
    <message>
        <location/>
        <source>Remove member from group</source>
        <translation>Retire le membre du groupe</translation>
    </message>
    <message>
        <source>Type in the name of a player you wish to remove from the group and press enter.</source>
        <translation type="obsolete">Tappez le nom du joueur que vous souhaitez retirer du groupe et appuyez sur entrée.</translation>
    </message>
    <message>
        <location/>
        <source>Dialog</source>
        <translation>Dialogue</translation>
    </message>
    <message>
        <source>Type in the name of a player you wish to add to the group and click Add.</source>
        <translation type="obsolete">Ecrire le nom du joueur que vous souhaitez ajouter dans le groupe et clicker sur ajouter.</translation>
    </message>
    <message>
        <source>Type in the name of a player you wish to remove from the group and click Add.</source>
        <translation type="obsolete">Ecrire le nom du joueur que vous souhaitez supprimer dans le groupe et clicker sur ajouter.</translation>
    </message>
    <message>
        <source>This shows all the entries active on this channel. Entries inherited from parent channels will be show in italics.&lt;br /&gt;ACLs are evaluated top to bottom, meaning priority increases as you move down the list.</source>
        <translation type="obsolete">Cela montre toutes les entrées actives sur ce salon. Le nombre de salons hérités seront montré en italique. &lt;br /&gt;Les ACL sont évalués de haut en bas, ce qui signifie que la priorité augmente à mesure que vous déplacez vers le bas de la liste.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Properties</source>
        <translation>&amp;Propriétés</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Name&lt;/b&gt;&lt;br /&gt;Enter the channel name in this field. The name has to comply with the restriction imposed by the server you are connected to.</source>
        <translation>&lt;b&gt;Nom&lt;/b&gt;&lt;br/&gt;Saisir le nom du salon dans ce champ. Le nom doit être conforme aux restrictions imposées par le serveur auquel vous êtes connecté.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Temporary&lt;/b&gt;&lt;br /&gt;
When checked the channel created will be marked as temporary. This means when the last player leaves it the channel will be automatically deleted by the server.</source>
        <translation>&lt;b&gt;Temporaire&lt;/b&gt;&lt;br&gt;
Quand cette case est cochée, le salon ainsi créé sera marqué temporaire. En conséquence, le salon sera automatiquement supprimé dès que le dernier utilisateur l&apos;aura quitté.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Group&lt;/b&gt;&lt;br /&gt;
These are all the groups currently defined for the channel. To create a new group, just type in the name and press enter.</source>
        <translation>&lt;b&gt;Groupe&lt;/b&gt;&lt;br/&gt;
Ce sont tous les groupes actuellement définis pour ce salon. Pour créer un nouveau groupe, il suffit de saisir un nom et d&apos;appuyer sur Entrée.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Remove&lt;/b&gt;&lt;br /&gt;This removes the currently selected group. If the group was inherited, it will not be removed from the list, but all local information about the group will be cleared.</source>
        <translation>&lt;b&gt;Supprimer&lt;/b&gt;&lt;br/&gt;Efface le groupe actuellement sélectionné. Si le groupe était hérité, il ne sera pas retiré de la liste, mais toutes les modifications locales du groupe seront effacées.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Inherit&lt;/b&gt;&lt;br /&gt;This inherits all the members in the group from the parent, if the group is marked as &lt;i&gt;Inheritable&lt;/i&gt; in the parent channel.</source>
        <translation>&lt;b&gt;Hérite&lt;/b&gt;&lt;br/&gt;Permet à tous les membres du groupe d&apos;être hérités du groupe parent, si le groupe est marqué comme étant &lt;i&gt;Héritable&lt;i&gt; dans le salon parent.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Inheritable&lt;/b&gt;&lt;br /&gt;This makes this group inheritable to sub-channels. If the group is non-inheritable, sub-channels are still free to create a new group with the same name.</source>
        <translation>&lt;b&gt;Héritable&lt;/b&gt;&lt;br/&gt;Indique que ce groupe peut être hérité par les sous salons. Si le groupe est non héritable, les sous salons sont libres de créer un nouveau groupe avec le même nom.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Inherited&lt;/b&gt;&lt;br /&gt;This indicates that the group was inherited from the parent channel. You cannot edit this flag, it&apos;s just for information.</source>
        <translation>&lt;b&gt;Hérité&lt;/b&gt;&lt;br/&gt;Indique que le groupe est hérité du salon parent. Vous ne pouvez pas modifier cette option, elle n&apos;est qu&apos;informative.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Members&lt;/b&gt;&lt;br /&gt;
This list contains all members that were added to the group by the current channel. Be aware that this does not include members inherited by higher levels of the channel tree. These can be found in the &lt;i&gt;Inherited members&lt;/i&gt; list. To prevent this list to be inherited by lower level channels uncheck &lt;i&gt;Inheritable&lt;/i&gt; or manually add the members to the &lt;i&gt;Excluded members&lt;/i&gt; list.</source>
        <translation>&lt;b&gt;Membres&lt;/&gt;&lt;br/&gt;
Cette liste contient tous les membres ajoutés au groupe dans le salon actuel. Notez que cela n&apos;inclut pas les membres hérités par les salons de niveaux supérieurs. Ceux-ci peuvent être trouvés dans la liste des &lt;i&gt;Membres hérités&lt;/i&gt;. Pour éviter que cette liste ne soit héritée par les sous salons, décochez &lt;i&gt;Héritable&lt;/i&gt; ou ajoutez manuellement les membres à la liste des &lt;i&gt;Membres exclus&lt;/i&gt;.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Excluded members&lt;/b&gt;&lt;br /&gt;
Contains a list of members whose group membership will not be inherited from the parent channel.</source>
        <translation>&lt;b&gt;Membres exclus&lt;/b&gt;&lt;br/&gt;
Contient une liste de membres dont l&apos;appartenance au groupe ne doit pas être hérité du salon parent.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Inherited members&lt;/b&gt;&lt;br /&gt;
Contains the list of members inherited by the current channel. Uncheck &lt;i&gt;Inherit&lt;/i&gt; to prevent inheritance from higher level channels.</source>
        <translation>&lt;b&gt;Membres hérités&lt;/b&gt;&lt;br /&gt;
Contient la liste des membres hérités par le salon actuel. Décochez &lt;i&gt;Hérite&lt;/i&gt; pour empêcher l&apos;héritage depuis un salon de niveau supérieur.</translation>
    </message>
    <message>
        <location/>
        <source>This controls which group of users this entry applies to.&lt;br /&gt;Note that the group is evaluated in the context of the channel the entry is used in. For example, the default ACL on the Root channel gives &lt;i&gt;Write&lt;/i&gt; permission to the &lt;i&gt;admin&lt;/i&gt; group. This entry, if inherited by a channel, will give a user write privileges if he belongs to the &lt;i&gt;admin&lt;/i&gt; group in that channel, even if he doesn&apos;t belong to the &lt;i&gt;admin&lt;/i&gt; group in the channel where the ACL originated.&lt;br /&gt;If a group name starts with &apos;!&apos;, its membership is negated, and if it starts with &apos;~&apos;, it is evaluated in the channel the ACL was defined in, rather than the channel the ACL is active in.&lt;br /&gt;If a group name starts with &apos;#&apos;, it is interpreted as an access token. Users must have entered whatever follows the &apos;#&apos; in their list of access tokens to match. This can be used for very simple password access to channels for non-authenticated users.&lt;br /&gt;If a group name starts with &apos;$&apos;, it will only match users whose certificate hash matches what follows the &apos;$&apos;.&lt;br /&gt;A few special predefined groups are:&lt;br /&gt;&lt;b&gt;all&lt;/b&gt; - Everyone will match.&lt;br /&gt;&lt;b&gt;auth&lt;/b&gt; - All authenticated users will match.&lt;br /&gt;&lt;b&gt;sub,a,b,c&lt;/b&gt; - User currently in a sub-channel minimum &lt;i&gt;a&lt;/i&gt; common parents, and between &lt;i&gt;b&lt;/i&gt; and &lt;i&gt;c&lt;/i&gt; channels down the chain. See the website for more extensive documentation on this one.&lt;br /&gt;&lt;b&gt;in&lt;/b&gt; - Users currently in the channel will match (convenience for &apos;&lt;i&gt;sub,0,0,0&lt;/i&gt;&apos;).&lt;br /&gt;&lt;b&gt;out&lt;/b&gt; - Users outside the channel will match (convenience for &apos;&lt;i&gt;!sub,0,0,0&lt;/i&gt;&apos;).&lt;br /&gt;Note that an entry applies to either a user or a group, not both.</source>
        <translation>Contrôle à quel groupe d&apos;utilisateurs cette entrée s&apos;applique.&lt;br /&gt;Notez que les groupes sont évalués dans le contexte du salon dans lequel l&apos;entrée est utilisée. Par exemple, la LCA du salon racine par défaut donne la permission d&apos;&lt;i&gt;écriture&lt;/i&gt; au groupe &lt;i&gt;admin&lt;/i&gt;. Cette entrée, si héritée par un salon, donnera à l&apos;utilisateur le privilège d&apos;écriture s&apos;il appartient au groupe &lt;i&gt;admin&lt;/i&gt; du salon, même s&apos;il n&apos;appartient pas au groupe &lt;i&gt;admin&lt;/i&gt; dans le salon de la LCA originelle.&lt;br /&gt;Si le nom d&apos;un groupe commence par &apos;!&apos;, son appartenance est négative, et s&apos;il commence par un &apos;~&apos;, il est évalué dans le salon dans lequel la LCA a été définie, plutôt que dans le salon dans lequel la LCA est active.&lt;br /&gt;Si le nom d&apos;un groupe commence par &apos;#&apos;, son contenu sera interprété comme un jeton d&apos;accès. Les utilisateurs doivent saisir ce qui suit le &apos;#&apos; dans leur liste de jetons d&apos;accès pour être valide. Cette méthode peut être utilisée pour un accès simple par mots de passe pour les utilisateurs non authentifiés.&lt;br /&gt;Si le nom d&apos;un groupe commence par &apos;$&apos;, seulement les utilisateurs dont le hash du certificat, qui doit suivre le &apos;$&apos;, seront valides.&lt;br /&gt;Quelques groupes prédéfinis sont :&lt;br /&gt;&lt;b&gt;all&lt;/b&gt; - Tout le monde est concerné.&lt;br /&gt;&lt;b&gt;auth&lt;/b&gt; - Tous les utilisateurs authentifiés&lt;br /&gt;&lt;b&gt;sub,a,b,c&lt;/b&gt; - Utilisateurs actuellement dans un sous-salon avec au moins &lt;i&gt;a&lt;/i&gt; parents commun, entre les salons &lt;i&gt;b&lt;/i&gt; et &lt;i&gt;c&lt;/i&gt;. Voir le site web pour une explication plus complète.&lt;br /&gt;&lt;b&gt;in&lt;/b&gt; - Utilisateurs dans le salon (raccourci pour &lt;i&gt;sub,0,0,0&lt;/i&gt;)&lt;br /&gt;&lt;b&gt;out&lt;/b&gt; - Utilisateurs en dehors du salon (raccourci pour &lt;i&gt;!sub,0,0,0&lt;/i&gt;).&lt;br /&gt;Notez que ces entrées s&apos;appliquent soit à un utilisateur, soit à un groupe, mais pas aux deux.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Password&lt;/b&gt;&lt;br /&gt;This field allows you to easily set and change the password of a channel. It uses Mumble&apos;s access tokens feature in the background. Use ACLs and groups if you need more fine grained and powerful access control.</source>
        <translation>&lt;b&gt;Mot de passe&lt;/b&gt;&lt;br /&gt;Ce champ vous permet de définir ou de changer aisément le mot de passe d&apos;un canal. Il utilise les jetons d&apos;accès de Mumble. Utilisez les LCAs et les groupes si vous avez besoin de plus de finesse dans le contrôle des accès.</translation>
    </message>
    <message>
        <location/>
        <source>This shows all the entries active on this channel. Entries inherited from parent channels will be shown in italics.&lt;br /&gt;ACLs are evaluated top to bottom, meaning priority increases as you move down the list.</source>
        <translation>Cela montre toutes les entrées actives sur ce salon. Le nombre de salons hérités est en italique. &lt;br /&gt;Les LCAs sont évalués de haut en bas, ce qui signifie que la priorité augmente à mesure que vous déplacez vers le bas de la liste.</translation>
    </message>
</context>
<context>
    <name>ALSAAudioInput</name>
    <message>
        <location filename="ALSAAudio.cpp" line="+360"/>
        <source>Opening chosen ALSA Input failed: %1</source>
        <translation>L&apos;ouverture du périphérique d&apos;entrée ALSA choisit a échoué : %1</translation>
    </message>
    <message>
        <location line="-191"/>
        <source>Default ALSA Card</source>
        <translation>Carte ALSA par défaut</translation>
    </message>
</context>
<context>
    <name>ALSAAudioOutput</name>
    <message>
        <location line="+326"/>
        <source>Opening chosen ALSA Output failed: %1</source>
        <translation>L&apos;ouverture du périphérique de sortie ALSA choisit a échoué : %1</translation>
    </message>
    <message>
        <location line="-325"/>
        <source>Default ALSA Card</source>
        <translation>Carte ALSA par défaut</translation>
    </message>
</context>
<context>
    <name>ALSAConfig</name>
    <message>
        <source>Device selection</source>
        <translation type="obsolete">Configuration du périphérique</translation>
    </message>
    <message>
        <source>Device to use for microphone</source>
        <translation type="obsolete">Périphérique à utiliser pour le microphone</translation>
    </message>
    <message>
        <source>This set which device mumble should use. The &lt;i&gt;default&lt;/i&gt; device is whatever you have configured in alsaconfig, the &lt;i&gt;hwplug&lt;/i&gt; devices are specific hardware devices backed by the ALSA mixer and the &lt;i&gt;hw&lt;/i&gt; devices are raw hardware access. Unless your soundcard supports hardware mixing of audio, using the &lt;i&gt;hw&lt;/i&gt; device will exclude all other programs from using audio.</source>
        <translation type="obsolete">Spécifie quel périphérique devrait être utilisé par mumble. Le périphérique &lt;i&gt;default&lt;/i&gt; est ce que vous avez configuré dans alsaconfig, les périphériques &lt;i&gt;hwplug&lt;/i&gt; sont les périphériques accessible par le mixer d&apos;ALSA et les périphériques &lt;i&gt;hw&lt;/i&gt; sont les adresses brut du matériel. À moins que votre carte son ne supporte le mixage matériel, en utilisant les périphériques &lt;i&gt;hw&lt;/i&gt;, vous excluerez tout autre programme utilisant l&apos;audio..</translation>
    </message>
    <message>
        <source>Device to use for speakers/headphones</source>
        <translation type="obsolete">Périphérique à utiliser pour les haut-parleurs/casques</translation>
    </message>
    <message>
        <source>Input</source>
        <translation type="obsolete">Entrée</translation>
    </message>
    <message>
        <source>Output</source>
        <translation type="obsolete">Sortie</translation>
    </message>
    <message>
        <source>Output Options</source>
        <translation type="obsolete">Options de sortie</translation>
    </message>
    <message>
        <source>Output Delay</source>
        <translation type="obsolete">Délai de sortie</translation>
    </message>
    <message>
        <source>Amount of data to buffer for ALSA</source>
        <translation type="obsolete">Quantité de données du tampon pour ALSA</translation>
    </message>
    <message>
        <source>This sets the amount of data to prebuffer in the output buffer. Experiment with different values and set it to the lowest which doesn&apos;t cause rapid jitter in the sound.</source>
        <translation type="obsolete">Spécifie la quantité de données à mettre en cache dans le tampon de sortie. Essayez différentes valeurs et mettez la plus bassequi ne cause pas de rapides décalages du son.</translation>
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
        <source>Default ALSA Card</source>
        <translation type="obsolete">Carte alsa par défaut</translation>
    </message>
</context>
<context>
    <name>ASIOConfig</name>
    <message>
        <location filename="ASIOInput.ui"/>
        <source>Device selection</source>
        <translation>Sélection du périphérique</translation>
    </message>
    <message>
        <location/>
        <source>Capabilities</source>
        <translation>Capacités</translation>
    </message>
    <message>
        <location/>
        <source>Buffer size</source>
        <translation>Taille du tampon</translation>
    </message>
    <message>
        <location/>
        <source>This will configure the input channels for ASIO. Make sure you select at least one channel as microphone and speaker. &lt;i&gt;Microphone&lt;/i&gt; should be where your microphone is attached, and &lt;i&gt;Speaker&lt;/i&gt; should be a channel that samples &apos;&lt;i&gt;What you hear&lt;/i&gt;&apos;.&lt;br /&gt;For example, on the Audigy 2 ZS, a good selection for Microphone would be &apos;&lt;i&gt;Mic L&lt;/i&gt;&apos; while Speaker should be &apos;&lt;i&gt;Mix L&lt;/i&gt;&apos; and &apos;&lt;i&gt;Mix R&lt;/i&gt;&apos;.</source>
        <translation>Configurera les canaux d&apos;entrée pour l&apos;ASIO. Veuillez vous assurer que vous avez sélectionné au moins un canal comme micro et haut-parleurs. &lt;i&gt;Microphone&lt;/i&gt; indique l&apos;endroit où votre microphone est connecté, et &lt;i&gt;Haut-parleurs&lt;/i&gt;indique le canal qui joue &apos;&lt;i&gt;Ce que vous entendez&lt;/i&gt;&apos;.&lt;br/&gt;
Par exemple, sur Audigy 2 ZS, une bonne sélection pour le microphone est &apos;&lt;i&gt;Mic L&lt;/l&gt;&apos; tandis que haut-parleur est &apos;&lt;i&gt;Mix L&lt;/i&gt;&apos; et &apos;&lt;i&gt;Lix R&lt;/i&gt;&apos;.</translation>
    </message>
    <message>
        <location/>
        <source>Channels</source>
        <translation>Canaux</translation>
    </message>
    <message>
        <location/>
        <source>Configure input channels</source>
        <translation>Configurer les cannaux d&apos;entrée</translation>
    </message>
    <message>
        <source>This will configure the input channels for ASIO. Make sure you select at least one channel as microphone and speaker. &lt;i&gt;Microphone&lt;/i&gt; should be where your microphone is attached, and &lt;i&gt;Speaker&lt;/i&gt; should be a channel that samples &quot;What you hear&quot;.&lt;br /&gt;For example, on the Audigy 2 ZS, a good selection for Microphone would be &quot;Mic L&quot; while Speaker should be &quot;Mix L&quot; and &quot;Mix R&quot;</source>
        <translation type="obsolete">Configure les canaux d&apos;entrée ASIO. Vérifiez que vous avez choisi au moins un canal pour le microphone et les haut-parleur. &lt;i&gt;Microphone&lt;/i&gt; devrait être l&apos;emplacement de votre microphone et &lt;i&gt;Speaker&lt;/i&gt; devrait être être le canal &quot;What you hear&quot;.&lt;br /&gt; Par exemple, sur la carte Audigy 2 ZS, une bonne sélection pour le micro serait &quot;Mic L&quot; tandis que les Haut-Parleurs seraient &quot;Mix L&quot; et &quot;Mix R&quot;</translation>
    </message>
    <message>
        <location/>
        <source>Device to use for microphone</source>
        <translation>Périphérique à utiliser pour le microphone</translation>
    </message>
    <message>
        <location/>
        <source>This chooses what device to query. You still need to actually query the device and select which channels to use.</source>
        <translation>Choisis le périphérique à interroger. Vous devez toujours interroger le périphérique et sélectionner les canaux à utiliser.</translation>
    </message>
    <message>
        <location/>
        <source>Device</source>
        <translation>Périphérique</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Query</source>
        <translation>&amp;Interroger</translation>
    </message>
    <message>
        <location/>
        <source>Query selected device</source>
        <translation>Interroger le périphérique sélectionné</translation>
    </message>
    <message>
        <location/>
        <source>This queries the selected device for channels. Be aware that many ASIO drivers are buggy to the extreme, and querying them might cause a crash of either the application or the system.</source>
        <translation>Interroge le périphérique sélectionné pour connaître ses cannaux. Soyez conscients que beaucoup de pilotes ASIO sont bogués à l&apos;extrême et les interroger peut crasher l&apos;application ou le système.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Configure</source>
        <translation>&amp;Configurer</translation>
    </message>
    <message>
        <location/>
        <source>Configure selected device</source>
        <translation>Configurer le périphérique sélectionné</translation>
    </message>
    <message>
        <location/>
        <source>This configures the selected device. Be aware that many ASIO drivers are buggy to the extreme, and querying them might cause a crash of either the application or the system.</source>
        <translation>Configure le périphérique sélectionné. Soyez conscients que les pilotes ASIO sont bogués à l&apos;extrême, et les utiliser pourrait causer le crash de l&apos;application ou du système.</translation>
    </message>
    <message>
        <location/>
        <source>Driver name</source>
        <translation>Nom du pilote</translation>
    </message>
    <message>
        <source>Buffersize</source>
        <translation type="obsolete">Taille du tampon</translation>
    </message>
    <message>
        <location/>
        <source>Microphone</source>
        <translation>Microphone</translation>
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
        <translation>Non utilisé</translation>
    </message>
    <message>
        <location/>
        <source>Speakers</source>
        <translation>Haut parleurs</translation>
    </message>
    <message>
        <source>%1 (ver %2)</source>
        <translation type="obsolete">%1 (ver %2)</translation>
    </message>
    <message>
        <source>%1 ms -&gt; %2 ms (%3 ms resolution) %4Hz</source>
        <translation type="obsolete">%1 ms -&gt; %2ms ( résolution de %3 ms ) %4Hz</translation>
    </message>
    <message>
        <source>%1 ms -&gt; %2 ms (%3 ms resolution) %4Hz -- Unusable</source>
        <translation type="obsolete">%1 ms -&gt; %2ms ( résolution de %3 ms ) %4Hz -- Inutilisable</translation>
    </message>
    <message>
        <location filename="ASIOInput.cpp" line="+222"/>
        <source>%1 (version %2)</source>
        <translation>%1 (version %2)</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>%1 -&gt; %2 samples buffer, with %3 sample resolution (%4 preferred) at %5 Hz</source>
        <oldsource>%1 -&gt; %2 samples buffer, with %3 sample resolution (%4 preferred) at %5Hz</oldsource>
        <translation>%1 -&gt; %2 échantillons tampon, avec %3 résolution exemple (%4 préféré) à %5 Hz</translation>
    </message>
    <message>
        <source>Mumble</source>
        <translation type="obsolete">Mumble</translation>
    </message>
    <message>
        <location line="+43"/>
        <location line="+25"/>
        <source>ASIO Initialization failed: %1</source>
        <translation>L&apos;initialisation de ASIO a échoué: %1</translation>
    </message>
    <message>
        <location line="-21"/>
        <location line="+25"/>
        <source>Failed to instantiate ASIO driver</source>
        <translation>Impossible d&apos;instancier le pilote ASIO</translation>
    </message>
    <message>
        <location line="+37"/>
        <source>ASIO</source>
        <translation>ASIO</translation>
    </message>
    <message>
        <location filename="ASIOInput.ui"/>
        <source>Form</source>
        <translation>Form</translation>
    </message>
    <message>
        <source>TextLabel</source>
        <translation type="obsolete">TextLabel</translation>
    </message>
    <message>
        <source>This will configure the input channels for ASIO. Make sure you select at least one channel as microphone and speaker. &lt;i&gt;Microphone&lt;/i&gt; should be where your microphone is attached, and &lt;i&gt;Speaker&lt;/i&gt; should be a channel that samples &lt;i&gt;What you hear&lt;/i&gt;.&lt;br /&gt;For example, on the Audigy 2 ZS, a good selection for Microphone would be &lt;i&gt;Mic L&lt;/i&gt; while Speaker should be &lt;i&gt;Mix L&lt;/i&gt; and &lt;i&gt;Mix R&lt;/i&gt;.</source>
        <translation type="obsolete">Configure les canaux d&apos;entrée ASIO. Vérifiez que vous avez choisi au moins un canal pour le microphone et les haut-parleur. &lt;i&gt;Microphone&lt;/i&gt; devrait être l&apos;emplacement de votre microphone, et &lt;i&gt;Speaker&lt;/i&gt; devrait être être le canal &lt;i&gt;What you hear&lt;/i&gt;.&lt;br /&gt; Par exemple, sur la carte Audigy 2 ZS, une bonne sélection pour le micro serait &lt;i&gt;Mic L&lt;/i&gt; tandis que les Haut-Parleurs seraient &lt;i&gt;Mix L&lt;/i&gt; et &lt;i&gt;Mix R&lt;/i&gt;.</translation>
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
        <translation>Vous devez sélectionner au moins un microphone et un haut-parleur pour utiliser ASIO. Si vous avez seulement besoin du microphone, utilisez DirectSound.</translation>
    </message>
    <message>
        <location line="+103"/>
        <source>Opening selected ASIO device failed. No input will be done.</source>
        <translation>L&apos;ouverture du périphérique ASIO sélectionné a échouée. Il n&apos;y aura pas d&apos;entrée.</translation>
    </message>
</context>
<context>
    <name>AboutDialog</name>
    <message>
        <source>&lt;h3&gt;Mumble v%1&lt;/h3&gt;&lt;p&gt;Copyright 2005-2008 Thorvald Natvig&lt;br /&gt;slicer@users.sourceforge.net&lt;/p&gt;&lt;p&gt;&lt;b&gt;A voice-chat utility for gamers&lt;/b&gt;&lt;/p&gt;&lt;p&gt;&lt;tt&gt;http://mumble.sourceforge.net/&lt;/tt&gt;&lt;/p&gt;</source>
        <translation type="obsolete">&lt;h3&gt;Mumble v%1&lt;/h3&gt;&lt;p&gt;Copyright 2005-2008 Thorvald Natvig&lt;br /&gt;slicer@users.sourceforge.net&lt;/p&gt;&lt;p&gt;&lt;b&gt;Un utilitaire de chat vocal pour les joueurs&lt;/b&gt;&lt;/p&gt;&lt;p&gt;&lt;tt&gt;http://mumble.sourceforge.net/&lt;/tt&gt;&lt;/p&gt;</translation>
    </message>
    <message>
        <location filename="About.cpp" line="+63"/>
        <source>&amp;About Mumble</source>
        <translation>&amp;A propos de Mumble</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>&amp;License</source>
        <translation>&amp;Licence</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>OK</source>
        <translation>Valider</translation>
    </message>
    <message>
        <location line="-29"/>
        <source>About Mumble</source>
        <translation>À propos de Mumble</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>&lt;h3&gt;Mumble (%1)&lt;/h3&gt;&lt;p&gt;Copyright %3 Thorvald Natvig&lt;br /&gt;slicer@users.sourceforge.net&lt;/p&gt;&lt;p&gt;&lt;b&gt;A voice-chat utility for gamers&lt;/b&gt;&lt;/p&gt;&lt;p&gt;&lt;tt&gt;&lt;a href=&quot;%2&quot;&gt;%2&lt;/a&gt;&lt;/tt&gt;&lt;/p&gt;</source>
        <translation>&lt;h3&gt;Mumble (%1)&lt;/h3&gt;&lt;p&gt;Copyright %3 Thorvald Natvig&lt;br /&gt;slicer@users.sourceforge.net&lt;/p&gt;&lt;p&gt;&lt;b&gt;Un utilitaire de communication vocale pour les joueurs&lt;/b&gt;&lt;/p&gt;&lt;p&gt;&lt;tt&gt;&lt;a href=&quot;%2&quot;&gt;%2&lt;/a&gt;&lt;/tt&gt;&lt;/p&gt;</translation>
    </message>
</context>
<context>
    <name>AboutSpeexDialog</name>
    <message>
        <location line="+25"/>
        <source>&lt;h3&gt;About Speex&lt;/h3&gt;&lt;p&gt;&lt;tt&gt;&lt;a href=&quot;%1&quot;&gt;%1&lt;/a&gt;&lt;/tt&gt;&lt;/p&gt;&lt;p&gt;This program uses SpeexDSP.&lt;/p&gt;&lt;p&gt;Speex is used for echo cancellation, noise&lt;br /&gt;filtering and voice activity detection.&lt;/p&gt;</source>
        <translation>&lt;h3&gt;À propos de Speex&lt;/h3&gt;&lt;p&gt;&lt;tt&gt;&lt;a href=&quot;%1&quot;&gt;%1&lt;/a&gt;&lt;/tt&gt;&lt;/p&gt;&lt;p&gt;Ce programme utilise SpeexDSP&lt;/p&gt;&lt;p&gt;Speex est utilisé pour la suppression de l&apos;écho,le filtrage&lt;br /&gt; du bruit et la détection de l&apos;activité vocale.&lt;/p&gt;</translation>
    </message>
    <message>
        <source>&lt;h3&gt;About Speex&lt;/h3&gt;&lt;p&gt;&lt;tt&gt;http://www.speex.org/&lt;/tt&gt;&lt;/p&gt;&lt;p&gt;This program uses Speex version %1&lt;/p&gt;&lt;p&gt;Speex is used for echo cancellation, noise&lt;br /&gt;filtering, voice activity detection and speech&lt;br /&gt;compression.&lt;/p&gt;</source>
        <translation type="obsolete">&lt;h3&gt;À propos de Speex&lt;/h3&gt;&lt;p&gt;&lt;tt&gt;http://www.speex.org/&lt;/tt&gt;&lt;/p&gt;&lt;p&gt;Ce programme utilise la version %1 de Speex&lt;/p&gt;&lt;p&gt;Speex est utilisé pour l&apos;annulation de l&apos;écho,&lt;br /&gt;le filtrage du bruit, la détection de l&apos;activité vocale,&lt;br /&gt;et la compression de la voix.&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>OK</source>
        <translation>Valider</translation>
    </message>
    <message>
        <location line="-12"/>
        <source>About Speex</source>
        <translation>À propos de Speex</translation>
    </message>
    <message>
        <source>&lt;h3&gt;About Speex&lt;/h3&gt;&lt;p&gt;&lt;tt&gt;&lt;a href=&quot;%2&quot;&gt;%2&lt;/a&gt;&lt;/tt&gt;&lt;/p&gt;&lt;p&gt;This program uses Speex version %1&lt;/p&gt;&lt;p&gt;Speex is used for echo cancellation, noise&lt;br /&gt;filtering, voice activity detection and speech&lt;br /&gt;compression.&lt;/p&gt;</source>
        <translation type="obsolete">&lt;h3&gt;A propos de Speex&lt;/h3&gt;&lt;p&gt;&lt;tt&gt;&lt;a href=&quot;%2&quot;&gt;%2&lt;/a&gt;&lt;/tt&gt;&lt;/p&gt;&lt;p&gt;Ce programme utilise la version de Speex %1&lt;/p&gt;&lt;p&gt;Speex est utilisé pour la suppression de bruit et d&apos;écho&lt;br /&gt;le filtrage, la détection de l&apos;activité vocale&lt;br /&gt;compression.&lt;/p&gt;</translation>
    </message>
</context>
<context>
    <name>AudioConfigDialog</name>
    <message>
        <source>Interfaces</source>
        <translation type="obsolete">Interfaces</translation>
    </message>
    <message>
        <source>Transmission</source>
        <translation type="obsolete">Transmission</translation>
    </message>
    <message>
        <source>Compression</source>
        <translation type="obsolete">Compression</translation>
    </message>
    <message>
        <source>Jitter Buffer</source>
        <translation type="obsolete">Tampon de gigue</translation>
    </message>
    <message>
        <source>Input</source>
        <translation type="obsolete">Entrée</translation>
    </message>
    <message>
        <source>Input method for audio</source>
        <translation type="obsolete">Méthode d&apos;entrée pour l&apos;audio</translation>
    </message>
    <message>
        <source>&lt;b&gt;This is the input method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation type="obsolete">&lt;b&gt;Il s&apos;agit de la méthode d&apos;entrée utilisée pour l&apos;audio.&lt;/b&gt;&lt;br /&gt;En général vous utilisez DirectSound.</translation>
    </message>
    <message>
        <source>Output</source>
        <translation type="obsolete">Sortie</translation>
    </message>
    <message>
        <source>Output method for audio</source>
        <translation type="obsolete">Méthode de sortie audio</translation>
    </message>
    <message>
        <source>&lt;b&gt;This is the output method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation type="obsolete">&lt;b&gt;Méthode de sortie audio. &lt;/b&gt;&lt;br /&gt;En général vous utilisez DirectSound.</translation>
    </message>
    <message>
        <source>Continuous</source>
        <translation type="obsolete">Continu</translation>
    </message>
    <message>
        <source>Voice Activity</source>
        <translation type="obsolete">Activité vocale</translation>
    </message>
    <message>
        <source>Push To Talk</source>
        <translation type="obsolete">Appuyez pour parler</translation>
    </message>
    <message>
        <source>Transmit</source>
        <translation type="obsolete">Transmission</translation>
    </message>
    <message>
        <source>When to transmit your speech</source>
        <translation type="obsolete">Quand transmettre votre voix</translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets when speech should be transmitted.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;Continuous&lt;/i&gt; - All the time&lt;br /&gt;&lt;i&gt;Voice Activity&lt;/i&gt; - When you are speaking clearly.&lt;br /&gt;&lt;i&gt;Push To Talk&lt;/i&gt; - When you hold down the hotkey set under &lt;i&gt;Shortcuts&lt;/i&gt;.</source>
        <translation type="obsolete">&lt;b&gt;Détermine quand transmettre votre voix.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;Continu&lt;/i&gt; - en continu&lt;br /&gt;&lt;i&gt;Activité Vocale&lt;/i&gt; - Quand vous parlez clairement.&lt;br /&gt;&lt;i&gt;Poussez pour parler&lt;/i&gt; - Quand vous pressez la touche choisie ci-dessous &lt;i&gt;Raccourcis&lt;/i&gt;.</translation>
    </message>
    <message>
        <source>Voice Hold</source>
        <translation type="obsolete">Maintient de la voix</translation>
    </message>
    <message>
        <source>How long to keep transmitting after silence</source>
        <translation type="obsolete">Combien de temps après un silence continuer à maintenir la voix</translation>
    </message>
    <message>
        <source>&lt;b&gt;This selects how long after a perceived stop in speech transmission should continue.&lt;/b&gt;&lt;br /&gt;Set this higher if your voice breaks up when you speak (seen by a rapidly blinking voice icon next to your name).&lt;br /&gt;Only makes sense when used with Voice Activity transmission.</source>
        <translation type="obsolete">&lt;b&gt;Détermine combien de temps la transmission devrait continuer après une pause dans la conversation.&lt;/b&gt;&lt;br /&gt;Choisissez une valeur supérieure si votre voix est coupée lorsque vous parlez (l&apos;icône située a coté de votre nom clignote alors très rapidement).&lt;br /&gt;Cela n&apos;a de sens que si vous utilisez la transmission selon l&apos;Activité Vocale.</translation>
    </message>
    <message>
        <source>Audio per packet</source>
        <translation type="obsolete">Trames par paquet</translation>
    </message>
    <message>
        <source>How many audio frames to send per packet</source>
        <translation type="obsolete">Nombre de trames audio à envoyer par paquet</translation>
    </message>
    <message>
        <source>&lt;b&gt;This selects how many audio frames should be put in one packet.&lt;/b&gt;&lt;br /&gt;Increasing this will increase the latency of your voice, but will also reduce bandwidth requirements.</source>
        <translation type="obsolete">&lt;b&gt;Sélectionne le nombre de trames audio qui devraient être envoyées dans un paquet.&lt;/b&gt;&lt;br /&gt;Augmenter cette valeur augmentera votre latence, mais réduira aussi les vos besoins en bande passante.</translation>
    </message>
    <message>
        <source>Use TCP mode</source>
        <translation type="obsolete">Utilise le mode TCP</translation>
    </message>
    <message>
        <source>Use TCP Compatibility mode</source>
        <translation type="obsolete">Utiliser le mode de compatibilité TCP</translation>
    </message>
    <message>
        <source>&lt;b&gt;Enable TCP Compatibility mode&lt;/b&gt;.&lt;br /&gt;This will make Mumble use only TCP when communicating with the server. This will increase overhead and cause lost packets to produce noticeable pauses in communication, so this should only be used if you are unable to use the default (which uses UDP for voice and TCP for control).</source>
        <translation type="obsolete">&lt;b&gt;Activer le mode de compatibilité TCP&lt;/b&gt;.&lt;br /&gt;Fait que Mumble utilisera seulement TCP pour communiquer avec le serveur. Ceci va augmenter la taille des headers et causer des pauses notables de la communication en cas de perte de paquets, donc cette fonctionnalité ne devrait être activée seulement si vous ne pouvez utiliser la configuration par défaut ( qui utilise UDP pour la voix et TCP pour le contrôle ).</translation>
    </message>
    <message>
        <source>Automatic Reconnect</source>
        <translation type="obsolete">Reconnexion Automatique</translation>
    </message>
    <message>
        <source>Reconnect when disconnected</source>
        <translation type="obsolete">Reconnexion quand déconnecté</translation>
    </message>
    <message>
        <source>&lt;b&gt;Reconnect when disconnected&lt;/b&gt;.&lt;br /&gt;This will make Mumble try to automatically reconnect after 10 seconds if your server connection fails.</source>
        <translation type="obsolete">&lt;b&gt;Se reconnecter quand déconnecté&lt;/b&gt;.&lt;br /&gt;Mumble essaiera automatiquement de se reconnecter après dix secondes si la connexion échoue.</translation>
    </message>
    <message>
        <source>Maximum bandwidth used for sent audio</source>
        <translation type="obsolete">Bande passante maximum utilisé pour l&apos;envoi audio</translation>
    </message>
    <message>
        <source>&lt;b&gt;This shows peak outgoing bandwidth used.&lt;/b&gt;&lt;br /&gt;This shows the peak amount of bandwidth sent out from your machine. Audio bitrate is the maximum bitrate (as we use VBR) for the audio data alone. Position is the bitrate used for positional information. Overhead is our framing and the IP packet headers (IP and UDP is 90% of this overhead).</source>
        <translation type="obsolete">&lt;b&gt;Montre la bande passante maximale utilisée.&lt;/b&gt;&lt;br /&gt;Montre la bande maximale passante maximale utilisée par cotre machine. Le débit binaire audio est le débit binaire maximum ( puisqu&apos;un débit binaire variable est utilisé ) pour les données seules. La position est le débit binaire utilisé pour les informations de position. L&apos;entête contient le système de frames et les entête des paquets IP.</translation>
    </message>
    <message>
        <source>Outgoing Bitrate</source>
        <translation type="obsolete">Débit binaire en sortie</translation>
    </message>
    <message>
        <source>Quality</source>
        <translation type="obsolete">Qualité</translation>
    </message>
    <message>
        <source>Quality of compression (peak bandwidth)</source>
        <translation type="obsolete">Qualité de la compression ( influe sur la bande passante )</translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets the quality of compression.&lt;/b&gt;&lt;br /&gt;This determines how much bandwidth Mumble is allowed to use for outgoing audio.</source>
        <translation type="obsolete">&lt;b&gt;Définis la qualité de la compression.&lt;/b&gt;&lt;br /&gt;Détermine combien de bande passante Mumble est autorisé à utiliser pour l&apos;audio en sortie.</translation>
    </message>
    <message>
        <source>Complexity</source>
        <translation type="obsolete">Complexité</translation>
    </message>
    <message>
        <source>Complexity of compression (CPU)</source>
        <translation type="obsolete">Complexité de la compression ( CPU )</translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets the complexity of compression.&lt;/b&gt;&lt;br /&gt;This determines how much CPU Mumble is allowed to use to increase transmitted voice quality. Settings above 5 give only marginal gain.</source>
        <translation type="obsolete">&lt;b&gt;Définis la complexité de la compression.&lt;b&gt;&lt;br /&gt;Détermine combien de CPU mumble est autorisé à utiliser pour améliorer la qualité de la voix transmise.</translation>
    </message>
    <message>
        <source>Amp</source>
        <translation type="obsolete">Amplification</translation>
    </message>
    <message>
        <source>Maximum amplification of input sound</source>
        <translation type="obsolete">Amplification maximum de l&apos;entrée sonore</translation>
    </message>
    <message>
        <source>&lt;b&gt;Maximum amplification of input.&lt;/b&gt;&lt;br /&gt;Mumble normalizes the input volume before compressing, and this sets how much it&apos;s allowed to amplify.&lt;br /&gt;The actual level is continually updated based on your current speech pattern, but it will never go above the level specified here.&lt;br /&gt;If the &lt;i&gt;Microphone loudness&lt;/i&gt; level of the audio statistics hover around 100%, you probably want to set this to 2.0 or so, but if, like most people, you are unable to reach 100%, set this to something much higher.&lt;br /&gt;Ideally, set it so &lt;i&gt;Microphone Loudness * Amplification Factor &gt;= 100&lt;/i&gt;, even when you&apos;re speaking really soft.&lt;br /&gt;&lt;br /&gt;Note that there is no harm in setting this to maximum, but Mumble will start picking up other conversations if you leave it to auto-tune to that level.</source>
        <translation type="obsolete">&lt;b&gt;Amplification de l&apos;entrée maximum.&lt;/b&gt;&lt;br /&gt;Mumble normalise le son de l&apos;entrée avant de le compresser, et l&apos;amplifie aussi fort qu&apos;il en est autorisé.&lt;br /&gt;Le niveau actuel est continuellement mis à jour selon votre modèle de parole actuel, mais il ne dépassera jamais le niveau spécifié ici.&lt;br /&gt;Si le &lt;i&gt;Volume du microphone&lt;/li&gt; des statistiques audio tourne autour de 100%, vous voudrez probablement le mettre à 2.0 ou quelque chose comme ça, mais si, comme la plupart des gens, vous n&apos;arrivez pas à atteindre 100%, définissez quelque chose de beaucoup plus haut.&lt;br /&gt;Idéallement, définissez le de manière à ce que &lt;i&gt;Volume du microphone * Facteur d&apos;amplification &gt;= 100&lt;/i&gt;, même si vous parlez vraiment doucement.&lt;br /&gt;&lt;br /&gt;Il est à noter que mettre cette option au maximum ne causera aucun dégât, mais que Mumble risque d&apos;entendre d&apos;autres conversations, si vous laissez à ce niveau.</translation>
    </message>
    <message>
        <source>Default Jitter Buffer</source>
        <translation type="obsolete">Tampon de gigue par défaut</translation>
    </message>
    <message>
        <source>How long to prebuffer on first packet</source>
        <translation type="obsolete">Combien de temps mettre en tampon le premier paquet</translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets the default buffer size for the jitter buffer&lt;/b&gt;.&lt;br /&gt;All incoming audio is buffered, and the jitter buffer continually tries to push the buffer to the minimum sustainable by your network, so latency can be as low as possible. This sets the default buffer size to use on the first packet to arrive from a new speaker, or when a speaker using Voice Activity or Push-To-Talk just started talking again. If the start of sentences you hear is very jittery, increase this value.</source>
        <translation type="obsolete">&lt;b&gt;Spécifie le tampon de gigue par défaut&lt;/b&gt;.&lt;br /&gt;Toutes les trames audio arrivantes sont mise en mémoire tampon, et le tampon de gigue essaie de mettre le tampon à la taille minimum soutenable par votre réseau, donc la latence est la plus basse possible. Indique la taille par défaut du tampon de gigue à utiliser sur le premier paquet qui arrive d&apos;un nouveau joueur, ou quand un joueur utilise l&apos;activation par activité vocale, ou appuyer pour parler. Si le début des phrases que vous entendez est très craquelé, augmentez cette valeur.</translation>
    </message>
    <message>
        <source>Basic Audio</source>
        <translation type="obsolete">Audio basique</translation>
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
        <translation type="obsolete">%1kbits/s ( Audio %2, Position %4, Entêtes %3 )</translation>
    </message>
    <message>
        <source>Loopback Test</source>
        <translation type="obsolete">Test loopback</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Aucun</translation>
    </message>
    <message>
        <source>Local</source>
        <translation type="obsolete">Local</translation>
    </message>
    <message>
        <source>Server</source>
        <translation type="obsolete">Serveur</translation>
    </message>
    <message>
        <source>Desired loopback mode</source>
        <translation type="obsolete">Mode de loopback désiré</translation>
    </message>
    <message>
        <source>&lt;b&gt;This enables one of the loopback testmodes.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;None&lt;/i&gt; - Loopback disabled&lt;br /&gt;&lt;i&gt;Local&lt;/i&gt; - Emulate a local server.&lt;br /&gt;&lt;i&gt;Server&lt;/i&gt; - Request loopback from server.&lt;br /&gt;Please note than when loopback is enabled, no other players will hear your voice. This setting is not saved on application exit.</source>
        <translation type="obsolete">&lt;b&gt;Ceci permet de s&apos;écouter parler à des fins de test.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;Aucun&lt;/i&gt; - Loopback désactivé&lt;br /&gt;&lt;i&gt;Local&lt;/i&gt; - Emulation d&apos;un serveur local.&lt;br /&gt;&lt;i&gt;Serveur&lt;/i&gt; - Loopback depuis le serveur.&lt;br /&gt;A noter que quand le loopback est activé les autres joueurs ne pourront pas vous entendre. Ces paramètres ne sont pas sauvegardés quand l&apos;application est quitté. </translation>
    </message>
    <message>
        <source>Packet Loss</source>
        <translation type="obsolete">Paquet perdu</translation>
    </message>
    <message>
        <source>Packet loss for loopback mode</source>
        <translation type="obsolete">Paquet perdu pour le mode de loopback</translation>
    </message>
    <message>
        <source>%1 s</source>
        <translation type="obsolete">%1 s</translation>
    </message>
    <message>
        <source>%1%</source>
        <translation type="obsolete">%1 %</translation>
    </message>
    <message>
        <source>-%1 dB</source>
        <translation type="obsolete">-%1 dB</translation>
    </message>
    <message>
        <source>&amp;Input</source>
        <translation type="obsolete">&amp;Entrée</translation>
    </message>
    <message>
        <source>&amp;Output</source>
        <translation type="obsolete">&amp;Sortie</translation>
    </message>
    <message>
        <source>&amp;Transmit</source>
        <translation type="obsolete">&amp;Transmission</translation>
    </message>
    <message>
        <source>Voice &amp;Hold</source>
        <translation type="obsolete">Maintient de la &amp;Voix</translation>
    </message>
    <message>
        <source>&lt;b&gt;This shows peak outgoing bandwidth used.&lt;/b&gt;&lt;br /&gt;This shows the peak amount of bandwidth sent out from your machine. Audio bitrate is the maximum bitrate (as we use VBR) for the audio data alone. Position is the bitrate used for positional information. Overhead is our framing and the IP packet headers (IP and UDP is 75% of this overhead).</source>
        <translation type="obsolete">&lt;b&gt;Montre la bande passante maximale utilisée.&lt;/b&gt;&lt;br /&gt;Montre la bande maximale passante maximale utilisée par cotre machine. Le débit binaire audio est le débit binaire maximum ( puisqu&apos;un débit binaire variable est utilisé ) pour les données seules. La position est le débit binaire utilisé pour les informations de position. L&apos;entête contient le système de frames et les entêtes des paquets IP. (75%?).</translation>
    </message>
    <message>
        <source>&amp;Quality</source>
        <translation type="obsolete">&amp;Qualité</translation>
    </message>
    <message>
        <source>&amp;Complexity</source>
        <translation type="obsolete">&amp;Complexité</translation>
    </message>
    <message>
        <source>Audio Processing</source>
        <translation type="obsolete">Processeur audio</translation>
    </message>
    <message>
        <source>Noise Suppression</source>
        <translation type="obsolete">Suppression du bruit</translation>
    </message>
    <message>
        <source>Noise suppression</source>
        <translation type="obsolete">Suppression du bruit</translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets the ammount of noise suppression to apply.&lt;/b&gt;&lt;br /&gt;The higher this value, the more aggressively stationary noise will be supressed.</source>
        <translation type="obsolete">&lt;b&gt;Cela fixe le niveau de suppression de bruit.&lt;/b&gt;&lt;br /&gt;Plus cette valeur est haute, plus il y aura de suppression de bruit.</translation>
    </message>
    <message>
        <source>Amplification</source>
        <translation type="obsolete">Amplification</translation>
    </message>
    <message>
        <source>Audio Output</source>
        <translation type="obsolete">Sortie audio</translation>
    </message>
    <message>
        <source>Volume</source>
        <translation type="obsolete">Volume</translation>
    </message>
    <message>
        <source>Volume of incoming speech</source>
        <translation type="obsolete">Volume entrant de parole</translation>
    </message>
    <message>
        <source>&lt;b&gt;This adjusts the volume of incoming speech.&lt;/b&gt;&lt;br /&gt;Note that this can only be used to decrease the volume. No amplification is possible.</source>
        <translation type="obsolete">&lt;b&gt;Ceci ajuste le volume entrant de parole.&lt;/b&gt;&lt;br /&gt;Notez que cela ne peut être utilisée pour diminuer le volume. Pas d&apos;amplification possible.</translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets the packet loss for loopback mode.&lt;/b&gt;&lt;br /&gt;This will be the ratio of packets lost. Unless your outgoing bandwidth is peaked or there&apos;s something wrong with your network connection, this will be 0%</source>
        <translation type="obsolete">&lt;b&gt;Ceci définit la perte de paquets pour le mode loopback.&lt;/b&gt;&lt;br /&gt;Ce sera le taux de paquets perdus. À moins que votre bande passante sortante soit atteinte ou s&apos;il ya un problème avec votre connexion réseau, ce sera 0%</translation>
    </message>
    <message>
        <source>&amp;Loopback</source>
        <translation type="obsolete">&amp;Loopback</translation>
    </message>
    <message>
        <source>Off</source>
        <translation type="obsolete">Eteint</translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets speech detection to use Signal to Noise ratio.&lt;/b&gt;&lt;br /&gt;In this mode, the input is analyzed for something resembling a clear singal, and the clarity of that signal is used to trigger speech detection.</source>
        <translation type="obsolete">&lt;b&gt;Ceci défiinit la détection de parole à utiliser pour le taux de signal/bruit.&lt;/b&gt;&lt;br /&gt;Dans ce mode, l&apos;entrée est analysé pour quelque chose ressemblant à un signal clair, et que la clarté du signal est utilisé pour déclencher la parole de détection.</translation>
    </message>
    <message>
        <source>Signal to Noise</source>
        <translation type="obsolete">Signal/Bruit</translation>
    </message>
    <message>
        <source>Amplitude</source>
        <translation type="obsolete">Amplitude</translation>
    </message>
</context>
<context>
    <name>AudioInput</name>
    <message>
        <location filename="AudioInput.ui"/>
        <source>Form</source>
        <translation>Form</translation>
    </message>
    <message>
        <location/>
        <source>Interface</source>
        <translation>Interface</translation>
    </message>
    <message>
        <location/>
        <source>System</source>
        <translation>Système</translation>
    </message>
    <message>
        <location/>
        <source>Input method for audio</source>
        <translation>Méthode d&apos;entrée pour l&apos;audio</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is the input method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation>&lt;b&gt;Il s&apos;agit de la méthode d&apos;entrée utilisée pour l&apos;audio.&lt;/b&gt;&lt;br /&gt;En général vous utilisez DirectSound.</translation>
    </message>
    <message>
        <location/>
        <source>Device</source>
        <translation>Périphérique</translation>
    </message>
    <message>
        <location/>
        <source>Input device for audio</source>
        <oldsource>Output method for audio</oldsource>
        <translation>Périphérique d&apos;entrée pour l&apos;audio</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is the input device to use for audio.&lt;/b&gt;</source>
        <oldsource>&lt;b&gt;This is the output method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</oldsource>
        <translation>&lt;b&gt;C&apos;est le périphérique d&apos;entrée à utiliser pour l&apos;audio.&lt;.b&gt;</translation>
    </message>
    <message>
        <source>Cancel Echo</source>
        <translation type="obsolete">Supprimer l&apos;écho</translation>
    </message>
    <message>
        <location/>
        <source>Transmission</source>
        <translation>Transmission</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Transmit</source>
        <translation>&amp;Transmission</translation>
    </message>
    <message>
        <location/>
        <source>When to transmit your speech</source>
        <translation>Quand transmettre votre voix</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets when speech should be transmitted.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;Continuous&lt;/i&gt; - All the time&lt;br /&gt;&lt;i&gt;Voice Activity&lt;/i&gt; - When you are speaking clearly.&lt;br /&gt;&lt;i&gt;Push To Talk&lt;/i&gt; - When you hold down the hotkey set under &lt;i&gt;Shortcuts&lt;/i&gt;.</source>
        <translation>&lt;b&gt;Détermine quand transmettre votre voix.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;Continu&lt;/i&gt; - En continu&lt;br /&gt;&lt;i&gt;Activité Vocale&lt;/i&gt; - Quand vous parlez clairement.&lt;br /&gt;&lt;i&gt;Appuyez pour parler&lt;/i&gt; - Quand vous pressez la touche choisie dans les &lt;i&gt;Raccourcis&lt;/i&gt;.</translation>
    </message>
    <message>
        <location/>
        <source>DoublePush Time</source>
        <translation>Temps entre deux pressions</translation>
    </message>
    <message>
        <source>TextLabel</source>
        <translation type="obsolete">TextLabel</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This enables the audio cues for push to talk.&lt;/b&gt;&lt;br /&gt;Setting this will give you a short audio beep when push to talk is pressed and released.</source>
        <translation>&lt;b&gt;Active le bip sonore pour le &apos;Appuyer-pour-parler&apos;&lt;/b&gt;&lt;br/&gt;Si vous activez cette option, vous entendrez un court bip sonore quand le bouton appuyez-pour-parler sera enfoncé ou relaché.</translation>
    </message>
    <message>
        <location/>
        <source>PTT Audio cue</source>
        <translation>Sons APP</translation>
    </message>
    <message>
        <location/>
        <source>Use SNR based speech detection</source>
        <translation>Utilise le rapport Signal/Bruit pour la détection de la voix</translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets speech detection to use Signal to Noise ratio.&lt;/b&gt;&lt;br /&gt;In this mode, the input is analyzed for something resembling a clear singal, and the clarity of that signal is used to trigger speech detection.</source>
        <translation type="obsolete">&lt;b&gt;Ceci défiinit la détection de parole à utiliser pour le taux de signal/bruit.&lt;/b&gt;&lt;br /&gt;Dans ce mode, l&apos;entrée est analysé pour quelque chose ressemblant à un signal clair, et que la clarté du signal soit utilisé pour déclencher la parole de détection.</translation>
    </message>
    <message>
        <location/>
        <source>Signal to Noise</source>
        <translation>Signal/Bruit</translation>
    </message>
    <message>
        <source>Use Amplitude based speech detection.</source>
        <translation type="obsolete">Utilise l&apos;amplitude pour la détection de la voix.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets speech detection to use Amplitude.&lt;/b&gt;&lt;br /&gt;In this mode, the raw strength of the input signal is used to detect speech.</source>
        <translation>&lt;b&gt;Définit l&apos;amplitude comme méthode de détection de la voix.&lt;/b&gt;&lt;br/&gt;Dans ce mode, la force du signal d&apos;entrée sera utilisée pour détecter la voix.</translation>
    </message>
    <message>
        <location/>
        <source>Amplitude</source>
        <translation>Amplitude</translation>
    </message>
    <message>
        <location/>
        <source>Voice &amp;Hold</source>
        <translation>Maintient de la &amp;voix</translation>
    </message>
    <message>
        <location/>
        <source>How long to keep transmitting after silence</source>
        <translation>Combien de temps après un silence continuer à maintenir la transmission</translation>
    </message>
    <message>
        <source>&lt;b&gt;This selects how long after a perceived stop in speech transmission should continue.&lt;/b&gt;&lt;br /&gt;Set this higher if your voice breaks up when you speak (seen by a rapidly blinking voice icon next to your name).&lt;br /&gt;Only makes sense when used with Voice Activity transmission.</source>
        <translation type="obsolete">&lt;b&gt;Détermine combien de temps la transmission devrait continuer après une pause dans la conversation.&lt;/b&gt;&lt;br /&gt;Choisissez une valeur supérieure si votre voix est coupée lorsque vous parlez (l&apos;icône située a coté de votre nom clignote alors très rapidement).&lt;br /&gt;Cela n&apos;a de sens que si vous utilisez la transmission via l&apos;Activité Vocale.</translation>
    </message>
    <message>
        <location/>
        <source>Silence Below</source>
        <translation>Silence en deça</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the trigger values for voice detection.&lt;/b&gt;&lt;br /&gt;Use this together with the Audio Statistics window to manually tune the trigger values for detecting speech. Input values below &quot;Silence Below&quot; always count as silence. Values above &quot;Speech Above&quot; always count as voice. Values in between will count as voice if you&apos;re already talking, but will not trigger a new detection.</source>
        <translation>&lt;b&gt;Définit la valeur pour la détection de la voix.&lt;/b&gt;&lt;br/&gt;Utilisez ce réglage avec la fenêtre de Statistiques Audio pour régler manuellement la valeur de détection de la voix. Toute valeur d&apos;entrée en dessous de &quot;Silence en deça&quot; sera toujours considérée comme du silence. Les valeurs au delà de &quot;Voix au delà&quot; seront toujours considérées comme de la voix. Les valeurs intermédiaires seront considérées comme étant de la voix si vous êtes en train de parler, mais n&apos;activeront pas une nouvelle détection.</translation>
    </message>
    <message>
        <location/>
        <source>Speech Above</source>
        <translation>Voix au delà</translation>
    </message>
    <message>
        <location/>
        <source>Signal values above this count as voice</source>
        <translation>Les valeurs de signal au delà comptent comme de la voix</translation>
    </message>
    <message>
        <location/>
        <source>Compression</source>
        <translation>Compression</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Quality</source>
        <translation>&amp;Qualité</translation>
    </message>
    <message>
        <location/>
        <source>Quality of compression (peak bandwidth)</source>
        <translation>Qualité de la compression (influe sur la bande passante)</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the quality of compression.&lt;/b&gt;&lt;br /&gt;This determines how much bandwidth Mumble is allowed to use for outgoing audio.</source>
        <translation>&lt;b&gt;Définis la qualité de la compression.&lt;/b&gt;&lt;br /&gt;Détermine combien de bande passante Mumble est autorisé à utiliser pour l&apos;audio en sortie.</translation>
    </message>
    <message>
        <source>&amp;Complexity</source>
        <translation type="obsolete">&amp;Complexité</translation>
    </message>
    <message>
        <source>Complexity of compression (CPU)</source>
        <translation type="obsolete">Complexité de la compression ( CPU )</translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets the complexity of compression.&lt;/b&gt;&lt;br /&gt;This determines how much CPU Mumble is allowed to use to increase transmitted voice quality. Settings above 5 give only marginal gain.</source>
        <translation type="obsolete">&lt;b&gt;Définis la complexité de la compression.&lt;b&gt;&lt;br /&gt;Détermine combien de CPU mumble est autorisé à utiliser pour améliorer la qualité de la voix transmise.</translation>
    </message>
    <message>
        <location/>
        <source>Audio per packet</source>
        <translation>Trames par paquet</translation>
    </message>
    <message>
        <location/>
        <source>How many audio frames to send per packet</source>
        <translation>Nombre de trames audio a  envoyer par paquet</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This selects how many audio frames should be put in one packet.&lt;/b&gt;&lt;br /&gt;Increasing this will increase the latency of your voice, but will also reduce bandwidth requirements.</source>
        <translation>&lt;b&gt;Sélectionne le nombre de trames audio qui devraient être envoyées dans un paquet.&lt;/b&gt;&lt;br /&gt;Augmenter cette valeur augmentera votre latence, mais réduira aussi vos besoins en bande passante.</translation>
    </message>
    <message>
        <source>Maximum bandwidth used for sent audio</source>
        <translation type="obsolete">Bande passante maximum utilisée pour l&apos;envoi audio</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This shows peak outgoing bandwidth used.&lt;/b&gt;&lt;br /&gt;This shows the peak amount of bandwidth sent out from your machine. Audio bitrate is the maximum bitrate (as we use VBR) for the audio data alone. Position is the bitrate used for positional information. Overhead is our framing and the IP packet headers (IP and UDP is 75% of this overhead).</source>
        <translation>&lt;b&gt;Montre la bande passante maximale utilisée.&lt;/b&gt;&lt;br /&gt;Montre la bande maximale passante maximale utilisée par votre machine. Le débit binaire audio est le débit binaire maximum (puisqu&apos;un débit binaire variable est utilisé) pour les données seules. La position est le débit binaire utilisé pour les informations de position. L&apos;entête contient le système de trames et les entêtes des paquets IP. (IP ET UPD est de 75% de l&apos;entête).</translation>
    </message>
    <message>
        <location/>
        <source>Audio Processing</source>
        <translation>Processeur audio</translation>
    </message>
    <message>
        <location/>
        <source>Noise Suppression</source>
        <translation>Suppression du bruit</translation>
    </message>
    <message>
        <location/>
        <source>Noise suppression</source>
        <translation>Suppression du bruit</translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets the ammount of noise suppression to apply.&lt;/b&gt;&lt;br /&gt;The higher this value, the more aggressively stationary noise will be supressed.</source>
        <translation type="obsolete">&lt;b&gt;Cela fixe le niveau de suppression de bruit.&lt;/b&gt;&lt;br /&gt;Plus cette valeur est haute, moins il y aura de parasites.</translation>
    </message>
    <message>
        <location/>
        <source>Amplification</source>
        <translation>Amplification</translation>
    </message>
    <message>
        <location/>
        <source>Maximum amplification of input sound</source>
        <translation>Amplification maximale de l&apos;entrée sonore</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Maximum amplification of input.&lt;/b&gt;&lt;br /&gt;Mumble normalizes the input volume before compressing, and this sets how much it&apos;s allowed to amplify.&lt;br /&gt;The actual level is continually updated based on your current speech pattern, but it will never go above the level specified here.&lt;br /&gt;If the &lt;i&gt;Microphone loudness&lt;/i&gt; level of the audio statistics hover around 100%, you probably want to set this to 2.0 or so, but if, like most people, you are unable to reach 100%, set this to something much higher.&lt;br /&gt;Ideally, set it so &lt;i&gt;Microphone Loudness * Amplification Factor &gt;= 100&lt;/i&gt;, even when you&apos;re speaking really soft.&lt;br /&gt;&lt;br /&gt;Note that there is no harm in setting this to maximum, but Mumble will start picking up other conversations if you leave it to auto-tune to that level.</source>
        <translation>&lt;b&gt;Amplification de l&apos;entrée maximale.&lt;/b&gt;&lt;br /&gt;Mumble normalise le son de l&apos;entrée avant de le compresser, et l&apos;amplifie aussi fort qu&apos;il en est autorisé.&lt;br /&gt;Le niveau actuel est continuellement mis à jour selon votre modèle de parole actuel, mais il ne dépassera jamais le niveau spécifié ici.&lt;br /&gt;Si le &lt;i&gt;Volume du microphone&lt;/li&gt; des statistiques audio tourne autour de 100%, vous voudrez probablement le mettre à 2.0 ou quelque chose comme ça, mais si, comme la plupart des gens, vous n&apos;arrivez pas à atteindre 100%, définissez quelque chose de beaucoup plus haut.&lt;br /&gt;Idéallement, définissez le de manière à ce que &lt;i&gt;Volume du microphone * Facteur d&apos;amplification &gt;= 100&lt;/i&gt;, même si vous parlez vraiment doucement.&lt;br /&gt;&lt;br /&gt;Notez que mettre cette option au maximum ne causera aucun dégât, mais que Mumble risque d&apos;entendre d&apos;autres conversations, si vous le laissez à ce niveau.</translation>
    </message>
    <message>
        <location/>
        <source>Current speech detection chance</source>
        <translation>Détection de parole actuelle</translation>
    </message>
    <message>
        <location/>
        <source>Cancel echo from speakers</source>
        <translation>Suppression de l&apos;écho des haut-parleurs</translation>
    </message>
    <message>
        <location/>
        <source>Enabling this will cancel the echo from your speakers. Mixed has low CPU impact, but only works well if your speakers are equally loud and equidistant from the microphone. Multichannel echo cancellation provides much better echo cancellation, but at a higher CPU cost.</source>
        <translation>Activer cette option annulera l&apos;écho de vos haut-parleurs. Mixé a le moins d&apos;impact CPU, mais ne fonctionne que si vos enceintes sont au même volume et équidistants de votre microphone. Multicanal offre de meilleurs résultats, mais au prix d&apos;une charge CPU plus importante.</translation>
    </message>
    <message>
        <location/>
        <source>Disabled</source>
        <translation>Désactivé</translation>
    </message>
    <message>
        <location/>
        <source>Mixed</source>
        <translation>Mixé</translation>
    </message>
    <message>
        <location/>
        <source>Multichannel</source>
        <translation>Multicanal</translation>
    </message>
    <message>
        <location/>
        <source>Echo</source>
        <translation>Écho</translation>
    </message>
    <message>
        <location/>
        <source>If you press the PTT key twice in this time it will get locked.</source>
        <translation>Si vous pressez la touche Appuyez-pour-parler deux fois dans cet intervale de temps, cela la bloquera.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;DoublePush Time&lt;/b&gt;&lt;br /&gt;If you press the push-to-talk key twice during the configured interval of time it will be locked. Mumble will keep transmitting until you hit the key once more to unlock PTT again.</source>
        <translation>&lt;b&gt;Temps entre deux pressions&lt;/b&gt;&lt;br/&gt;Si vous pressez la touche Appuyez-pour-parler deux fois durant l&apos;intervale de temps configuré, cela la verrouillera. Mumble continuera à transmettre tant que la touche ne sera pas enfoncée une nouvelle fois.</translation>
    </message>
    <message>
        <location/>
        <source>Gets played when the PTT button is pressed</source>
        <translation>Sera joué quand le boutton Appuyez-pour-parler sera pressé</translation>
    </message>
    <message>
        <location/>
        <source>Gets played when the PTT button is released</source>
        <translation>Sera joué quand le boutton Appuyez-pour-parler sera relaché</translation>
    </message>
    <message>
        <location/>
        <source>Reset audio cue to default</source>
        <translation>Remet les sons par défaut</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Reset&lt;/b&gt;&lt;br/&gt;Reset the paths for the files to their default.</source>
        <translation>&lt;b&gt;Remettre à zéro&lt;/b&gt;&lt;br/&gt;Reinitialise les chemins des fichiers sonores aux valeurs par défaut.</translation>
    </message>
    <message>
        <source>Reset</source>
        <translation type="obsolete">R.A.Z</translation>
    </message>
    <message>
        <location/>
        <source>Browse for on audio file</source>
        <translation>Recherche un fichier audio quand activé</translation>
    </message>
    <message>
        <source>Browse</source>
        <translation type="obsolete">Parcourir</translation>
    </message>
    <message>
        <location/>
        <source>Browse for off audio file</source>
        <translation>Rechercher un fichier audio quand éteind</translation>
    </message>
    <message>
        <location/>
        <source>Off</source>
        <translation>Éteint</translation>
    </message>
    <message>
        <location/>
        <source>On</source>
        <translation>Allumé</translation>
    </message>
    <message>
        <location/>
        <source>Preview the audio cues</source>
        <translation>Écouter les bips sonores</translation>
    </message>
    <message>
        <source>&lt;b&gt;Preview&lt;/b&gt;&lt;br/&gt;Plays the current &lt;i&gt;on&lt;/i&gt; soundfile followed by the current &lt;i&gt;off&lt;/i&gt; soundfile.</source>
        <translation type="obsolete">&lt;b&gt;Prévisualisation&lt;/b&gt;&lt;br/&gt;Joue le son pour l&apos;action &lt;i&gt;Allumé&lt;/i&gt; suivi de celui pour &lt;i&gt;Éteint&lt;/i&gt;.</translation>
    </message>
    <message>
        <source>Preview</source>
        <translation type="obsolete">Prévisualisation</translation>
    </message>
    <message>
        <location/>
        <source>Audible audio cue when push-to-talk is activated or deactivated</source>
        <translation>Son entendu lorsque la touche appuyez-pour-parler est enfoncée ou reclachée</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets speech detection to use Signal to Noise ratio.&lt;/b&gt;&lt;br /&gt;In this mode, the input is analyzed for something resembling a clear signal, and the clarity of that signal is used to trigger speech detection.</source>
        <translation>&lt;b&gt;Définit la détection vocale sur l&apos;utilisation du rapport Signal/Bruit.&lt;/b&gt;&lt;br/&gt;Dans ce mode, l&apos;entrée est analysée pour trouver ce qui ressemble à un signal clair, et la qualité de ce signal est utilisé comme base pour déterminer la détection vocale.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This shows the current speech detection settings.&lt;/b&gt;&lt;br /&gt;You can change the settings from the Settings dialog or from the Audio Wizard.</source>
        <translation>&lt;b&gt;Ceci affiche le niveau actuel des paramètres de détection de parole.&lt;/b&gt;&lt;br /&gt;Vous pouvez changer ces paramètres depuis les paramètres ou l&apos;assistant audio.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the amount of noise suppression to apply.&lt;/b&gt;&lt;br /&gt;The higher this value, the more aggressively stationary noise will be suppressed.</source>
        <translation>&lt;b&gt;Définit la quantité de son à supprimer à appliquer.&lt;/b&gt;&lt;br/&gt;Plus la valeur est élevée, plus les bruits seront supprimés.</translation>
    </message>
    <message>
        <source>Idle AutoMute</source>
        <translation type="obsolete">Auto-absence (devenir muet)</translation>
    </message>
    <message>
        <source>How long to remain idle before auto-mute.</source>
        <translation type="obsolete">Combien de temps avant l&apos;absence automatique.</translation>
    </message>
    <message>
        <source>This sets an idle timer. If the timer expires without any audio being sent to the server, you are muted.</source>
        <translation type="obsolete">Ceci règle les paramètres d&apos;absence. Si pendant cette durée il n&apos;y a eut aucun envoi audio vers le serveur. Vous deviendrez muet.</translation>
    </message>
    <message>
        <location/>
        <source>Signal values below this count as silence</source>
        <translation>Les valeurs de signal en deça sont du silence</translation>
    </message>
    <message>
        <location/>
        <source>Maximum bandwidth used for sending audio</source>
        <translation>Bande passante maximale utilisée pour envoyer l&apos;audio</translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets the amount of noise suppression to apply.&lt;/b&gt;&lt;br /&gt;The higher this value, the more aggressively stationary noise will be supressed.</source>
        <translation type="obsolete">&lt;b&gt;Cela fixe le niveau de suppression de bruit à appliquer.&lt;/b&gt;&lt;br /&gt;Plus cette valeur est haute, plus il y aura de suppression de bruit.</translation>
    </message>
    <message>
        <location filename="AudioInput.cpp" line="+520"/>
        <source>Server maximum network bandwidth is only %1 kbit/s. Audio quality auto-adjusted to %2 kbit/s (%3ms)</source>
        <translation>La bande passante maximale est seulement de %1 kbits/s. La qualité audio est automatiquement ajustée à %2 kbits/s (%3ms)</translation>
    </message>
    <message>
        <location filename="AudioInput.ui"/>
        <source>Use Amplitude based speech detection</source>
        <translation>Utiliser l&apos;amplitude pour la détection de la voix</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This selects how long after a perceived stop in speech transmission should continue.&lt;/b&gt;&lt;br /&gt;Set this higher if your voice breaks up when you speak (seen by a rapidly blinking voice icon next to your name).</source>
        <translation>&lt;b&gt;Détermine combien de temps la transmission devrait continuer après une pause.&lt;/b&gt;&lt;br /&gt;Choisissez une valeur supérieure si votre voix est coupée lorsque vous parlez (l&apos;icône située a coté de votre nom clignote alors très rapidement).&lt;br /&gt;Cela n&apos;a de sens que si vous utilisez la transmission selon l&apos;Activité Vocale.</translation>
    </message>
    <message>
        <location/>
        <source>Idle AutoDeafen</source>
        <translation>Auto-absence
(devenir sourd)</translation>
    </message>
    <message>
        <location/>
        <source>How long to remain idle before auto-deafen.</source>
        <translation>Combien de temps avant l&apos;absence automatique.</translation>
    </message>
    <message>
        <location/>
        <source>This sets an idle timer. If the timer expires without any audio being sent to the server, you are muted and deafened.</source>
        <translation>Ceci règle les paramètres d&apos;absence. Si pendant cette durée il n&apos;y a eut aucun envoi audio vers le serveur. Vous deviendrez sourd et muet.</translation>
    </message>
    <message>
        <location/>
        <source>Exclusive mode</source>
        <translation>Mode exclusif</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This opens the device in exclusive mode.&lt;/b&gt;&lt;br /&gt;No other application will be able to use the device.</source>
        <translation>&lt;b&gt;Ceci utilise le périphérique en mode exclusif.&lt;/b&gt;&lt;br /&gt;Aucune autre application ne pourra l&apos;utiliser.</translation>
    </message>
    <message>
        <location/>
        <source>Exclusive</source>
        <translation>Exclusif</translation>
    </message>
    <message>
        <location/>
        <source>R&amp;eset</source>
        <translation>&amp;Réinitialiser</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Browse...</source>
        <translation>&amp;Parcourir...</translation>
    </message>
    <message>
        <location/>
        <source>B&amp;rowse...</source>
        <translation>P&amp;arcourir...</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Preview</source>
        <translation>Ap&amp;erçu</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Preview&lt;/b&gt;&lt;br/&gt;Plays the current &lt;i&gt;on&lt;/i&gt; sound followed by the current &lt;i&gt;off&lt;/i&gt; sound.</source>
        <translation>&lt;b&gt;Prévisualisation&lt;/b&gt;&lt;br/&gt;Joue le son pour l&apos;action &lt;i&gt;Allumé&lt;/i&gt; suivi de celui pour &lt;i&gt;Éteint&lt;/i&gt;.</translation>
    </message>
</context>
<context>
    <name>AudioInputDialog</name>
    <message>
        <location filename="AudioConfigDialog.cpp" line="+71"/>
        <source>Continuous</source>
        <translation>Continu</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Voice Activity</source>
        <translation>Activité vocale</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Push To Talk</source>
        <translation>Appuyez-pour-parler</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Audio Input</source>
        <translation>Entrée audio</translation>
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
        <translation>Éteint</translation>
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
        <translation>%1kbits/s (Audio %2 %5, Position %4, Entêtes %3)</translation>
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
        <translation type="obsolete">%1kbits/s ( Audio %2, Position %4, Entêtes %3 )</translation>
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
        <translation>Form</translation>
    </message>
    <message>
        <location/>
        <source>Interface</source>
        <translation>Interface</translation>
    </message>
    <message>
        <location/>
        <source>System</source>
        <translation>Système</translation>
    </message>
    <message>
        <location/>
        <source>Output method for audio</source>
        <oldsource>Input method for audio</oldsource>
        <translation>Méthode de sortie audio</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is the output method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <oldsource>&lt;b&gt;This is the input method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</oldsource>
        <translation>&lt;b&gt;Méthode de sortie audio.&lt;/b&gt;&lt;br /&gt;En général vous utilisez DirectSound.</translation>
    </message>
    <message>
        <location/>
        <source>Device</source>
        <translation>Périphérique</translation>
    </message>
    <message>
        <location/>
        <source>Output device for audio</source>
        <translation>Périphérique de sortie audio</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is the output device to use for audio.&lt;/b&gt;</source>
        <translation>&lt;b&gt;C&apos;est le périphérique de sortie à utiliser pour l&apos;audio.&lt;/b&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Positional Audio</source>
        <translation>Position Audio</translation>
    </message>
    <message>
        <location/>
        <source>Audio Output</source>
        <translation>Sortie audio</translation>
    </message>
    <message>
        <location/>
        <source>Default &amp;Jitter Buffer</source>
        <translation>Tampon de &amp;gigue par défaut</translation>
    </message>
    <message>
        <location/>
        <source>Safety margin for jitter buffer</source>
        <translation>Marge de sécurité pour le tampon de gigue</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the minimum safety margin for the jitter buffer.&lt;/b&gt;&lt;br /&gt;All incoming audio is buffered, and the jitter buffer continually tries to push the buffer to the minimum sustainable by your network, so latency can be as low as possible. This sets the minimum buffer size to use. If the start of sentences you hear is very jittery, increase this value.</source>
        <translation>&lt;b&gt;C&apos;est la valeur minimale de sécurité du tampon de gigue.&lt;/b&gt;&lt;br/&gt;Tout le son qui arrive est temporisé, et le tampon de gigue essaie de maintenir le tampon à la valeur minimale soutenable par cotre réseau, la latence est donc maintenue au niveau le plus bas possible. Ce paramètre définit la taille minimale du tampon à utiliser. Si le début des phrases que vous entendez est vraiment mauvais, augmentez cette valeur.</translation>
    </message>
    <message>
        <location/>
        <source>Minimum distance to user before sound volume decreases</source>
        <translation>Distance minimale de l&apos;utilisateur avant que le volume sonore diminue</translation>
    </message>
    <message>
        <location/>
        <source>This sets the minimum distance for sound calculations. The volume of other users&apos; speech will not decrease until they are at least this far away from you.</source>
        <translation>Spécifie la distance minimale pour le calcul du son. Le volume des autres utiisateurs ne sera pas diminué tant qu&apos;ils ne seront pas plus éloigné de vous.</translation>
    </message>
    <message>
        <location/>
        <source>This sets the maximum distance for sound calculations. When farther away than this, other users&apos; speech volume will not decrease any further.</source>
        <translation>Spécifie la distance maximum pour le calcul du son. Passé cette distance, le volume des autres joueurs ne diminuera plus.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This enables one of the loopback test modes.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;None&lt;/i&gt; - Loopback disabled&lt;br /&gt;&lt;i&gt;Local&lt;/i&gt; - Emulate a local server.&lt;br /&gt;&lt;i&gt;Server&lt;/i&gt; - Request loopback from server.&lt;br /&gt;Please note than when loopback is enabled, no other users will hear your voice. This setting is not saved on application exit.</source>
        <translation>&lt;b&gt;Active l&apos;un des tests de boucle locale.&lt;/b&gt;&lt;br/&gt;&lt;i&gt;Aucun&lt;/i&gt;- Test boucle locale désactivée.&lt;br/&gt;&lt;i&gt;Local&lt;/i&gt; - Émule un serveur local.&lt;br/&gt;&lt;i&gt;Serveur&lt;/i&gt; - Demande au serveur de retourner le son.&lt;br/&gt;Veuillez noter que si vous activez le test de boucle locale, aucun autre utilisateur ne vous entendra. Ce paramètre n&apos;est pas mémorisé en quittant.</translation>
    </message>
    <message>
        <source>TextLabel</source>
        <translation type="obsolete">TextLabel</translation>
    </message>
    <message>
        <location/>
        <source>Volume</source>
        <translation>Volume</translation>
    </message>
    <message>
        <location/>
        <source>Volume of incoming speech</source>
        <translation>Volume de parole entrant</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This adjusts the volume of incoming speech.&lt;/b&gt;&lt;br /&gt;Note that if you increase this beyond 100%, audio will be distorted.</source>
        <translation>&lt;b&gt;Ceci ajuste le volume entrant de parole.&lt;/b&gt;&lt;br /&gt;Notez que si vous augmentez cette valeur au-delà de 100%, le son sera déformé.</translation>
    </message>
    <message>
        <location/>
        <source>Output Delay</source>
        <translation>Délai de sortie</translation>
    </message>
    <message>
        <location/>
        <source>Amount of data to buffer</source>
        <translation>Quantitée de données à mettre en tampon</translation>
    </message>
    <message>
        <source>Volume attenuation</source>
        <translation type="obsolete">Atténuation du volume</translation>
    </message>
    <message>
        <source>MinDistance</source>
        <translation type="obsolete">MinDistance</translation>
    </message>
    <message>
        <source>Minimum distance to player before sound decreases</source>
        <translation type="obsolete">Distance minimum d&apos;un joueur avant que le son ne diminue</translation>
    </message>
    <message>
        <source>This sets the minimum distance for sound calculations. The volume of other players&apos; speech will not decrease until they are at least this far away from you.</source>
        <translation type="obsolete">Définis la distance minimum pour calculer le son. Le volume de la voix des autres joueurs ne diminuera pas avant d&apos;être au moins aussi loin de vous.</translation>
    </message>
    <message>
        <source>MaxDistance</source>
        <translation type="obsolete">MaxDistance</translation>
    </message>
    <message>
        <source>Maximum distance, beyond which sound won&apos;t decrease</source>
        <translation type="obsolete">Distance maximale, après laquelle le son ne diminuera plus</translation>
    </message>
    <message>
        <source>This sets the maximum distance for sound calculations. When farther away than this, other players&apos; sound volume will not decrease any more.</source>
        <translation type="obsolete">Spécifie la distance maximum pour le calcul du son. Passé cette distance, le volume des autres joueurs ne diminuera plus.</translation>
    </message>
    <message>
        <source>RollOff</source>
        <translation type="obsolete">Atténuation</translation>
    </message>
    <message>
        <location/>
        <source>Factor for sound volume decrease</source>
        <translation>Facteur de diminution du volume sonore</translation>
    </message>
    <message>
        <source>How fast should sound volume drop when passing beyond the minimum distance. The normal (1.0) is that sound volume halves each time the distance doubles. Increasing this value means sound volume drops faster, while decreasing it means it drops slower.</source>
        <translation type="obsolete">À quelle vitesse le volume sonore devrait descendre en dépassant la distance minimale. Normalement ( 1.0 ), le son diminue de moitié à chaque fois que la distance double. Augmenter cette valeur signifie que le son descend plus vite, tandis que la diminuer signifie que le son diminue plus lentement.</translation>
    </message>
    <message>
        <location/>
        <source>Bloom</source>
        <translation>Augmentation</translation>
    </message>
    <message>
        <location/>
        <source>Loopback Test</source>
        <translation>Test de boucle locale</translation>
    </message>
    <message>
        <location/>
        <source>Delay Variance</source>
        <translation>Délai de variance</translation>
    </message>
    <message>
        <location/>
        <source>Variance in packet latency</source>
        <translation>Variance de la latence des paquets</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the packet latency variance for loopback testing.&lt;/b&gt;&lt;br /&gt;Most audio paths contain some variable latency. This allows you to set that variance for loopback mode testing. For example, if you set this to 15ms, this will emulate a network with 20-35ms ping latency or one with 80-95ms latency. Most domestic net connections have a variance of about 5ms.</source>
        <oldsource>&lt;b&gt;This sets the packet latency variance for loopback testing.&lt;/b&gt;&lt;br /&gt;Most audio paths contain some variable latency. This allows you set that variance for loopback mode testing. For example, if you set this to 15ms, this will emulate a network with 20-35ms ping latency or one with 80-95ms latency. Most domestic net connections have a variance of about 5ms</oldsource>
        <translation>&lt;b&gt;Définit la variance de la latence des paquets pour le test de boucle locale.&lt;/b&gt;&lt;br/&gt;Beaucoup de chemins audio contiennent une latence variable. Vous pouvez spécifier ici cette variance pour effectuer des tests de boucle locale. Par exemple, si vous la définissez à 15ms, un réseau avec une latence de 20-35 ms de ping ou de 80-90 ms de lantence sera émulé. La majorité des connexions domestiques ont une variance proche de 5 ms.</translation>
    </message>
    <message>
        <location/>
        <source>Packet Loss</source>
        <translation>Paquet perdu</translation>
    </message>
    <message>
        <location/>
        <source>Packet loss for loopback mode</source>
        <translation>Paquet perdu pour le mode de boucle locale</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the packet loss for loopback mode.&lt;/b&gt;&lt;br /&gt;This will be the ratio of packets lost. Unless your outgoing bandwidth is peaked or there&apos;s something wrong with your network connection, this will be 0%</source>
        <translation>&lt;b&gt;Ceci définit la perte de paquets pour le mode de boucle locale.&lt;/b&gt;&lt;br /&gt;Ce sera le taux de paquets perdus. À moins que votre bande passante sortante soit atteinte ou qu&apos;il y ai un problème avec votre connexion réseau, ce sera 0%</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Loopback</source>
        <translation>Boucle &amp;locale</translation>
    </message>
    <message>
        <location/>
        <source>Desired loopback mode</source>
        <translation>Mode de boucle locale désirée</translation>
    </message>
    <message>
        <source>&lt;b&gt;This enables one of the loopback testmodes.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;None&lt;/i&gt; - Loopback disabled&lt;br /&gt;&lt;i&gt;Local&lt;/i&gt; - Emulate a local server.&lt;br /&gt;&lt;i&gt;Server&lt;/i&gt; - Request loopback from server.&lt;br /&gt;Please note than when loopback is enabled, no other players will hear your voice. This setting is not saved on application exit.</source>
        <translation type="obsolete">&lt;b&gt;Ceci permet de s&apos;écouter parler à des fins de test.&lt;/b&gt;&lt;br /&gt;&lt;i&gt;Aucun&lt;/i&gt; - Loopback désactivé&lt;br /&gt;&lt;i&gt;Local&lt;/i&gt; - Emulation d&apos;un serveur local.&lt;br /&gt;&lt;i&gt;Serveur&lt;/i&gt; - Loopback depuis le serveur.&lt;br /&gt;A noter que quand le loopback est activé les autres joueurs ne pourront pas vous entendre. Ces paramètres ne sont pas sauvegardés quand l&apos;application est quitté. </translation>
    </message>
    <message>
        <location/>
        <source>Factor for sound volume increase</source>
        <translation>Facteur de diminution du volume sonore</translation>
    </message>
    <message>
        <location/>
        <source>The connected &quot;speakers&quot; are actually headphones</source>
        <oldsource>The connected &quot;speakers&quot; are actually headphones.</oldsource>
        <translation>Les &quot;haut-parleurs&quot; actuellement connectés sont un casque</translation>
    </message>
    <message>
        <location/>
        <source>Minimum Distance</source>
        <translation>Distance minimale</translation>
    </message>
    <message>
        <location/>
        <source>Maximum Distance</source>
        <translation>Distance maximale</translation>
    </message>
    <message>
        <location/>
        <source>Maximum distance, beyond which speech volume won&apos;t decrease</source>
        <translation>Distance maximale, au delà de laquelle le volume n&apos;est pas diminué</translation>
    </message>
    <message>
        <location/>
        <source>Minimum Volume</source>
        <translation>Volume minimum</translation>
    </message>
    <message>
        <location/>
        <source>What should the volume be at the maximum distance?</source>
        <translation>Quel doit être le volume à la distance maximale ?</translation>
    </message>
    <message>
        <location/>
        <source>How much should sound volume increase for sources that are really close?</source>
        <translation>Comment doit être augmenté le volume pour les sources qui sont vraiment très proches ?</translation>
    </message>
    <message>
        <source>Connected &quot;speakers&quot; are actually headphones.</source>
        <translation type="obsolete">Les &quot;hauts parleurs&quot; connectés sont en fait des écouteurs.</translation>
    </message>
    <message>
        <location/>
        <source>Checking this indicates that you don&apos;t have speakers connected, just headphones. This is important, as speakers are usually in front of you, while headphones are directly to your left/right.</source>
        <translation>Cocher ceci indique que vous n&apos;avez pas de haut-parleurs connectés, juste un casque. C&apos;est important, car les haut-parleurs sont nromalement en face de vous, alors que le casque est directement sur votre gauche/droite.</translation>
    </message>
    <message>
        <location/>
        <source>Headphones</source>
        <translation>Casque audio</translation>
    </message>
    <message>
        <source>Other Applications</source>
        <translation type="obsolete">Autres applications</translation>
    </message>
    <message>
        <location/>
        <source>This sets the amount of data to pre-buffer in the output buffer. Experiment with different values and set it to the lowest which doesn&apos;t cause rapid jitter in the sound.</source>
        <translation>Spécifie la quantité de données à mettre en cache dans le tampon de sortie. Essayez différentes valeurs et mettez la plus basse qui ne cause pas de rapides décalages du son.</translation>
    </message>
    <message>
        <source>Volume of other applications during speech</source>
        <translation type="obsolete">Le volume pour les autres applications pendant la conversation</translation>
    </message>
    <message>
        <source>&lt;b&gt;Decrease volume of other applications during speech.&lt;/b&gt;&lt;br /&gt;Mumble supports decreasing the volume of other applications during incoming speech. This sets the relative volume of other applications when others are talking.</source>
        <translation type="obsolete">&lt;b&gt;Baisser le volume des autres applications pendant la conversation.&lt;/b&gt;&lt;br /&gt;Ceci permet de baisser le volume des autres applications pendant que vous discutez sur Mumble.</translation>
    </message>
    <message>
        <location/>
        <source>Attenuate applications by...</source>
        <translation>Atténuer les applications de...</translation>
    </message>
    <message>
        <location/>
        <source>Attenuation of other applications during speech</source>
        <translation>Atténuation des autes applications lors de l&apos;écoute ou de la parole</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Attenuate volume of other applications during speech&lt;/b&gt;&lt;br /&gt;Mumble supports decreasing the volume of other applications during incoming and/or outgoing speech. This sets the attenuation of other applications if the feature is enabled.</source>
        <translation>&lt;b&gt;Atténuer le volume des autres applications durant l&apos;écoute ou la parole&lt;/b&gt;&lt;br /&gt;Mumble peut abaisser le volume des autres applications durant l&apos;écoute ou la parole. Ceci règle l&apos;atténuation des autres applications si cette fonction est activée.</translation>
    </message>
    <message>
        <location/>
        <source>If checked Mumble lowers the volume of other applications while other users talk</source>
        <translation>Si cochée, Mumble baisse le volume des autres applications lors de l&apos;écoute</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Attenuate applications while other users talk&lt;/b&gt;&lt;br /&gt;Mumble supports decreasing the volume of other applications during incoming and/or outgoing speech. This makes mumble activate the feature while other users talk to you.</source>
        <translation>&lt;b&gt;Atténuer les applications lors de l&apos;écoute&lt;/b&gt;&lt;br /&gt;Mumble peut abaisser le volume des autres applications durant l&apos;écoute ou la parole. Ceci active l&apos;atténuation des autres applications si un utilisateur vous parle.</translation>
    </message>
    <message>
        <location/>
        <source>while other users talk</source>
        <translation>lors de l&apos;écoute</translation>
    </message>
    <message>
        <location/>
        <source>If checked Mumble lowers the volume of other applications while you talk</source>
        <translation>Si cochée, Mumble baisse le volume des autres applications lors de la parole</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Attenuate applications while you talk&lt;/b&gt;&lt;br /&gt;Mumble supports decreasing the volume of other applications during incoming and/or outgoing speech. This makes mumble activate the feature while you talk.</source>
        <translation>&lt;b&gt;Atténuer les applications lors de la parole&lt;/b&gt;&lt;br /&gt;Mumble peut abaisser le volume des autres applications durant l&apos;écoute ou la parole. Ceci active l&apos;atténuation des autres applications dès que vous parlez.</translation>
    </message>
    <message>
        <location/>
        <source>while you talk</source>
        <translation>lors de la parole</translation>
    </message>
    <message>
        <location/>
        <source>Exclusive mode, not recommended.</source>
        <translation>Mode exclusif, non recommandé.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This opens the device in exclusive mode.&lt;/b&gt;&lt;br /&gt;No other application will be able to use the device.</source>
        <translation>&lt;b&gt;Ceci utilise le périphérique en mode exclusif.&lt;/b&gt;&lt;br /&gt;Aucune autre application ne pourra l&apos;utiliser.</translation>
    </message>
    <message>
        <location/>
        <source>Exclusive</source>
        <translation>Exclusif</translation>
    </message>
</context>
<context>
    <name>AudioOutputDialog</name>
    <message>
        <location filename="AudioConfigDialog.cpp" line="+165"/>
        <source>None</source>
        <translation>Aucun</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Local</source>
        <translation>Local</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Server</source>
        <translation>Serveur</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Audio Output</source>
        <translation>Sortie audio</translation>
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
        <translation type="obsolete">%1 %</translation>
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
        <translation>Choisir le fichier son</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Invalid sound file</source>
        <translation>Fichier son invalide</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The file &apos;%1&apos; cannot be used by Mumble. Please select a file with a compatible format and encoding.</source>
        <translation>Le fichier &apos;%1&apos; ne peut pas être utilisé par Mumble. Veuillez choisir un fichier avec un format et un encodage compatible.</translation>
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
        <translation>Niveau maximal du microphone</translation>
    </message>
    <message>
        <location/>
        <source>Peak speaker level</source>
        <translation>Niveau maximal du haut-parleur</translation>
    </message>
    <message>
        <location/>
        <source>Peak clean level</source>
        <translation>Niveau maximal nettoyé</translation>
    </message>
    <message>
        <source>Microphone loudness</source>
        <translation type="obsolete">Volume du microphone</translation>
    </message>
    <message>
        <source>Mic Signal-To-Noise</source>
        <translation type="obsolete">Signal/Bruit du micro</translation>
    </message>
    <message>
        <location/>
        <source>Speech Probability</source>
        <translation>Probabilité de parole</translation>
    </message>
    <message>
        <source>Audio bitrate</source>
        <translation type="obsolete">Débit binaire audio</translation>
    </message>
    <message>
        <source>Real</source>
        <translation type="obsolete">Réel</translation>
    </message>
    <message>
        <source>Imaginary</source>
        <translation type="obsolete">Imaginaire</translation>
    </message>
    <message>
        <source>Modulus</source>
        <translation type="obsolete">Modulo</translation>
    </message>
    <message>
        <source>Phase</source>
        <translation type="obsolete">Phase</translation>
    </message>
    <message>
        <location/>
        <source>Peak power in last frame</source>
        <translation>Puissance maximale dans la trame précédente</translation>
    </message>
    <message>
        <source>This shows the peak power in the last frame (20 ms), and is the same measurement as you would usually find displayed as &quot;input power&quot;. Please disregard this and look at &lt;b&gt;Loudness&lt;/b&gt; instead, which is much more steady and disregards outliers.</source>
        <translation type="obsolete">Montre la puissance maximale de la dernière trame ( 20 ms ), et</translation>
    </message>
    <message>
        <location/>
        <source>This shows the peak power of the speakers in the last frame (20 ms). Unless you are using a multi-channel sampling method (such as ASIO) with speaker channels configured, this will be 0. If you have such a setup configured, and this still shows 0 while you&apos;re playing audio from other programs, your setup is not working.</source>
        <oldsource>This shows the peak power in the last frame (20 ms) of the speakers. Unless you are using a multi-channel sampling method (such as ASIO) with speaker channels configured, this will be 0. If you have such a setup configured, and this still shows 0 while you&apos;re playing audio from other programs, your setup is not working.</oldsource>
        <translation>Montre la puissance maximale de la dernière trame (20 ms) des haut-parleurs. Sauf si vous utilisez une méthode multicanal d&apos;échantillonnage (telles que l&apos;ASIO) des canaux des haut-parleurs configuré, cette valeur sera 0. Si vous avez une telle configuration, affichant toujours 0 pendant que vous entendez de le son d&apos;autres applications, votre configuration ne fonctionne pas.</translation>
    </message>
    <message>
        <location/>
        <source>This shows the peak power in the last frame (20 ms) after all processing. Ideally, this should be -96 dB when you&apos;re not talking. In reality, a sound studio should see -60 dB, and you should hopefully see somewhere around -20 dB. When you are talking, this should rise to somewhere between -5 and -10 dB.&lt;br /&gt;If you are using echo cancellation, and this rises to more than -15 dB when you&apos;re not talking, your setup is not working, and you&apos;ll annoy other users with echoes.</source>
        <translation>Montre la puissance maximale de la dernière trame (20 ms) après tous les traitements. Idéalement, elle doit être de -96dB quand vous ne parlez pas. En réalité, un studio sonore atteindra -60 dB, et vous aurrez joyeusement -20 dB ailleurs. Quand vous ne parlez pas, il devrait se trouver entre -5 et -10 dB.&lt;br/&gt;Si vous utiliser la suppression de l&apos;écho, et que la valeur est de plus de -15 dB quand vous ne parlez pas, votre configuration ne fonctionne pas, et vous dérangez les autres utilisateurs avec l&apos;écho.</translation>
    </message>
    <message>
        <location/>
        <source>How close the current input level is to ideal</source>
        <translation>À quel point le niveau sonore en entrée est proche de l&apos;idéal</translation>
    </message>
    <message>
        <location/>
        <source>Signal-To-Noise ratio from the microphone</source>
        <translation>Rapport Signal/Bruit du microphone</translation>
    </message>
    <message>
        <location/>
        <source>This is the Signal-To-Noise Ratio (SNR) of the microphone in the last frame (20 ms). It shows how much clearer the voice is compared to the noise.&lt;br /&gt;If this value is below 1.0, there&apos;s more noise than voice in the signal, and so quality is reduced.&lt;br /&gt;There is no upper limit to this value, but don&apos;t expect to see much above 40-50 without a sound studio.</source>
        <translation>C&apos;est le rapport Signal/Bruit (SNR) de votre microphone de la dernière trame (20 ms). Il vous affiche comment votre voix est nettoyée des bruits.&lt;br/&gt;Si cette valeur est suppérieure à 1.0, Il y a davantage de bruits que de signal vocal, et la qualité est donc réduite.&lt;br/&gt;Il n&apos;y a pas de limite supérieure à cette valeur, mais n&apos;espérez pas avoir de valeurs supérieures à 40-50 sans studio audio.</translation>
    </message>
    <message>
        <location/>
        <source>Probability of speech</source>
        <translation>Probabilité de parole</translation>
    </message>
    <message>
        <location/>
        <source>This is the probability that the last frame (20 ms) was speech and not environment noise.&lt;br /&gt;Voice activity transmission depends on this being right. The trick with this is that the middle of a sentence is always detected as speech; the problem is the pauses between words and the start of speech. It&apos;s hard to distinguish a sigh from a word starting with &apos;h&apos;.&lt;br /&gt;If this is in bold font, it means Mumble is currently transmitting (if you&apos;re connected).</source>
        <translation>Il s&apos;agit de la probabilité que la trame précédente (20 ms) soit de la parole et non du bruit.&lt;br /&gt;La transmission de l&apos;activité vocale dépend de cette valeur. L&apos;astuce est que le milieu de la phrase est toujours détecté comme de la voix ; les problèmes sont les pauses entre les mots et le début des phrases. Il est difficile de distinguer un soupir d&apos;un mot commençant par &quot;h&quot;.&lt;br /&gt;Si c&apos;est écrit en gras, cela signifie que Mumble est en train de transmettre des données (si vous êtes connectés).</translation>
    </message>
    <message>
        <location/>
        <source>Bitrate of last frame</source>
        <translation>Débit binaire de la dernière trame</translation>
    </message>
    <message>
        <source>This is the audio bitrate of the last compressed frame (20 ms), and as such will jump up and down as the VBR adjusts the quality. To adjust the peak bitrate, adjust &lt;b&gt;Compression Complexity&lt;/b&gt; in the Settings dialog.</source>
        <translation type="obsolete">Il s&apos;agit du débit binaire de la dernière trame compressée (20 ms). Ceci va varier de haut en bas puisque nous utilisons un bitrate variable pour ajuster la qualité. Pour ajuster le bitrate maximal, ajustez &lt;b&gt;Complexité de la compression&lt;/b&gt; dans les Préférences.</translation>
    </message>
    <message>
        <location/>
        <source>Time between last two Push-To-Talk presses</source>
        <translation>Temps entre la dernière pression double sur Appuyez-pour-Parler</translation>
    </message>
    <message>
        <location/>
        <source>Power spectrum of input signal and noise estimate</source>
        <translation>Spectre de la puissance du signal d&apos;entrée et du bruit estimé</translation>
    </message>
    <message>
        <location/>
        <source>This shows the power spectrum of the current input signal (red line) and the current noise estimate (filled blue).&lt;br /&gt;All amplitudes are multiplied by 30 to show the interesting parts (how much more signal than noise is present in each waveband).&lt;br /&gt;This is probably only of interest if you&apos;re trying to fine-tune noise conditions on your microphone. Under good conditions, there should be just a tiny flutter of blue at the bottom. If the blue is more than halfway up on the graph, you have a seriously noisy environment.</source>
        <translation>Montre le spectre de puissance du signal d&apos;entrée actuel (ligne rouge) et l&apos;estimation du bruit actuel (en bleu).&lt;br /&gt;Toutes les amplitudes sont multipliées par 30 pour montrer les parties intéressantes (quantité de signal par rapport au bruit dans chaque bande de fréquence).&lt;br /&gt;Ce n&apos;est intéressant que si vous voulez configurer finement les conditions de bruit de votre microphone. Dans de bonnes conditions, il devrait y avoir seulement un léger battement de bleu. Si le niveau de bleu dépasse la moitié sur la graphique, vous avez un envireonnement vraiment bruyant.</translation>
    </message>
    <message>
        <location/>
        <source>Weights of the echo canceller</source>
        <translation>Force de l&apos;écho suppression</translation>
    </message>
    <message>
        <location/>
        <source>This shows the weights of the echo canceller, with time increasing downwards and frequency increasing to the right.&lt;br /&gt;Ideally, this should be black, indicating no echo exists at all. More commonly, you&apos;ll have one or more horizontal stripes of bluish color representing time delayed echo. You should be able to see the weights updated in real time.&lt;br /&gt;Please note that as long as you have nothing to echo off, you won&apos;t see much useful data here. Play some music and things should stabilize. &lt;br /&gt;You can choose to view the real or imaginary parts of the frequency-domain weights, or alternately the computed modulus and phase. The most useful of these will likely be modulus, which is the amplitude of the echo, and shows you how much of the outgoing signal is being removed at that time step. The other viewing modes are mostly useful to people who want to tune the echo cancellation algorithms.&lt;br /&gt;Please note: If the entire image fluctuates massively while in modulus mode, the echo canceller fails to find any correlation whatsoever between the two input sources (speakers and microphone). Either you have a very long delay on the echo, or one of the input sources is configured wrong.</source>
        <translation>Montre le poids de la suppression de l&apos;écho, avec le temps en ordonnée et la fréquence en absisse.&lt;br /&gt;Idéalement, celui-ci devrait être noir, indiquant qu&apos;aucun écho n&apos;existe. Plus communément, vous aurez une ou plusieurs bandes bleutées représentant le temps de retard de l&apos;écho. Vous devriez être capable de visualiser les poids mis à jour en temps réel.&lt;br /&gt;Notez que tant que vous n&apos;avez aucun signal où enlever l&apos;écho, vous ne verrez pas beaucoup de données utiles ici. Ecoutez de la musique et les choses devraient se stabiliser.&lt;br /&gt;Vous pouvez choisir de voir la partie réelle ou imaginaire dans le domaine fréquentiel, ou alternativement le module et la phase des poids. Le plus utile de ces outils serait probablement le module, qui est l&apos;amplitude de l&apos;écho, qui montre la quantité de signal sortant supprimé à chaque instant. Les autres modes de visualisation sont principalement utiles pour les gens qui veulent régler les algorithmes d&apos;annulation de l&apos;écho.&lt;br /&gt;Remarque : si l&apos;image entière fluctue massivement en mode module, l&apos;algorithme de suppression de l&apos;écho n&apos;a pas réussi à trouver une corrélation entre les deux sources d&apos;entrée (haut-parleurs et microphone). Soit vous avez un retard très long sur l&apos;écho, soit une des sources d&apos;entrée est mal configurée.</translation>
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
        <translation>Statistiques audio</translation>
    </message>
    <message>
        <location/>
        <source>Input Levels</source>
        <translation>Niveaux d&apos;entrée</translation>
    </message>
    <message>
        <location/>
        <source>This shows the peak power in the last frame (20 ms), and is the same measurement as you would usually find displayed as &quot;input power&quot;. Please disregard this and look at &lt;b&gt;Microphone power&lt;/b&gt; instead, which is much more steady and disregards outliers.</source>
        <translation>Montre la puissance maximale de la dernière trame (20 ms), c&apos;est la même valeur que vous trouverez normalement en &quot;puissance d&apos;entrée&quot;.Veuillez ignorer cette valeur et regarder plutôt la &lt;b&gt;puissance du micro&lt;/b&gt;, qui est beaucoup plus stable et ne tient pas compte des valeurs aberrantes.</translation>
    </message>
    <message>
        <source>TextLabel</source>
        <translation type="obsolete">TextLabel</translation>
    </message>
    <message>
        <location/>
        <source>Signal Analysis</source>
        <translation>Analyse du signal</translation>
    </message>
    <message>
        <location/>
        <source>Microphone power</source>
        <translation>Puissance du micro</translation>
    </message>
    <message>
        <location/>
        <source>This shows how close your current input volume is to the ideal. To adjust your microphone level, open whatever program you use to adjust the recording volume, and look at the value here while talking.&lt;br /&gt;&lt;b&gt;Talk loud, as you would when you&apos;re upset over getting fragged by a noob.&lt;/b&gt;&lt;br /&gt;Adjust the volume until this value is close to 100%, but make sure it doesn&apos;t go above. If it does go above, you are likely to get clipping in parts of your speech, which will degrade sound quality.</source>
        <translation>Montre à quel point le niveau de volume d&apos;entré courant est proche de l&apos;idéal. Pour ajuster le niveau de votre microphone, ouvrez le programme que vous utilisez pour ajuster le volume d&apos;enregistrement, et regardez la valeur ici pendant que vous parlez.&lt;br /&gt;&lt;b&gt;Parlez fort, comme lorsque vous êtes énervé d&apos;avoir été tué par un noob.&lt;/b&gt;&lt;br /&gt;Réglez le volume jusqu&apos;à ce que cette valeur soit proche de 100%, mais assurez-vous qu&apos;elle ne le dépasse pas. Si ça va au-delà, vous aurez probablement des coupure dans votre discussion, ce qui degradera la qualité audio.</translation>
    </message>
    <message>
        <location/>
        <source>Signal-To-Noise ratio</source>
        <translation>Taux de signal/bruit</translation>
    </message>
    <message>
        <location/>
        <source>Configuration feedback</source>
        <translation>Retour de configuration</translation>
    </message>
    <message>
        <location/>
        <source>Current audio bitrate</source>
        <translation>Débit binaire audio actuel</translation>
    </message>
    <message>
        <location/>
        <source>DoublePush interval</source>
        <translation>Intervalle double PTT</translation>
    </message>
    <message>
        <location/>
        <source>Speech Detection</source>
        <translation>Détection de parole</translation>
    </message>
    <message>
        <location/>
        <source>Current speech detection chance</source>
        <translation>Détection de parole actuelle</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This shows the current speech detection settings.&lt;/b&gt;&lt;br /&gt;You can change the settings from the Settings dialog or from the Audio Wizard.</source>
        <translation>&lt;b&gt;Ceci affiche le niveau actuel des paramètres de détection de parole.&lt;/b&gt;&lt;br /&gt;Vous pouvez changer ses paramètres depuis les paramètres ou l&apos;assistant audio.</translation>
    </message>
    <message>
        <location/>
        <source>Signal and noise power spectrum</source>
        <translation>Spectre de la puissance du signal d&apos;entrée et du bruit</translation>
    </message>
    <message>
        <location/>
        <source>Echo Analysis</source>
        <translation>Analyse de l&apos;écho</translation>
    </message>
    <message>
        <location/>
        <source>This is the audio bitrate of the last compressed frame (20 ms), and as such will jump up and down as the VBR adjusts the quality. The peak bitrate can be adjusted in the Settings dialog.</source>
        <translation>Il s&apos;agit du débit binaire de la dernière trame compressée (20 ms). Ceci va varier de haut en bas puisque nous utilisons un bitrate variable pour ajuster la qualité. Pour ajuster le bitrate maximal, ajustez &lt;b&gt;Complexité de la compression&lt;/b&gt; dans les Préférences.</translation>
    </message>
</context>
<context>
    <name>AudioWizard</name>
    <message>
        <location filename="AudioWizard.ui"/>
        <source>Audio Tuning Wizard</source>
        <translation>Assistant audio</translation>
    </message>
    <message>
        <location/>
        <source>Introduction</source>
        <translation>Introduction</translation>
    </message>
    <message>
        <location/>
        <source>Welcome to the Mumble Audio Wizard</source>
        <translation>Bienvenue dans l&apos;assistant audio de Mumble</translation>
    </message>
    <message>
        <source>This is the audio tuning wizard for Mumble. This will help you correctly set the input levels of your sound card, and also set the correct parameters for sound processing in Mumble.</source>
        <translation type="obsolete">Ceci est l&apos;assistant audio pour Mumble. Il vous aide à configurer correctement le le niveau d&apos;entré de votre carte son, Et également définir les paramètres corrects dans le traitement du son pour Mumble.</translation>
    </message>
    <message>
        <source>Please be aware that as long as this wizard is active, audio will be looped locally to allow you to listen to it, and no audio will be sent to the server.</source>
        <translation type="obsolete">Durant la phase de paramètrage de la carte son, l&apos;audio est sera bouclée localement pour vous permettre d&apos;écouter sa réponse, et non pas depuis un serveur.</translation>
    </message>
    <message>
        <location/>
        <source>Finished</source>
        <translation>Terminé</translation>
    </message>
    <message>
        <location/>
        <source>Enjoy using Mumble</source>
        <translation>Profitez de l&apos;utilisation de Mumble</translation>
    </message>
    <message>
        <source>Congratulations. You should now be ready to enjoy a richer sound experience with Mumble.</source>
        <translation type="obsolete">Félicitations. Vous devriez maintenant être prêt à profiter d&apos;une expérience sonore riche avec Mumble.</translation>
    </message>
    <message>
        <location/>
        <source>Device selection</source>
        <translation>Sélection du périphérique</translation>
    </message>
    <message>
        <location/>
        <source>Selecting the input and output device to use with Mumble.</source>
        <translation>Sélectionner le périphérique d&apos;entrée et de sortie à utiliser dans Mumble.</translation>
    </message>
    <message>
        <source>Input device</source>
        <translation type="obsolete">Périphérique d&apos;entrée</translation>
    </message>
    <message>
        <location/>
        <source>This is the device your microphone is connected to.</source>
        <translation>Périphérique auquel votre microphone est connecté.</translation>
    </message>
    <message>
        <location/>
        <source>System</source>
        <translation>Système</translation>
    </message>
    <message>
        <location/>
        <source>Input method for audio</source>
        <translation>Méthode d&apos;entrée pour l&apos;audio</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is the input method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation>&lt;b&gt;Il s&apos;agit de la méthode d&apos;entrée utilisée pour l&apos;audio.&lt;/b&gt;&lt;br /&gt;En général vous utilisez DirectSound.</translation>
    </message>
    <message>
        <location/>
        <source>Device</source>
        <translation>Périphérique</translation>
    </message>
    <message>
        <location/>
        <source>Input device to use</source>
        <translation>Périphérique d&apos;entrée à utiliser</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Selects which sound card to use for audio input.&lt;/b&gt;</source>
        <translation>&lt;b&gt;Sélectionner la carte son a utiliser pour la sortie audio.&lt;/b&gt;</translation>
    </message>
    <message>
        <source>Output device</source>
        <translation type="obsolete">Périphérique de sortie</translation>
    </message>
    <message>
        <location/>
        <source>This is the device your speakers or headphones are connected to.</source>
        <translation>Périphérique auquel vos haut-parleurs ou vos écouteurs sont connectés.</translation>
    </message>
    <message>
        <location/>
        <source>Output method for audio</source>
        <translation>Méthode de sortie audio</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is the Output method to use for audio.&lt;/b&gt;&lt;br /&gt;Most likely you want to use DirectSound.</source>
        <translation>&lt;b&gt;Méthode de sortie audio. &lt;/b&gt;&lt;br /&gt;En général vous utilisez DirectSound.</translation>
    </message>
    <message>
        <location/>
        <source>Output device to use</source>
        <translation>Périphérique de sortie à utiliser</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Selects which sound card to use for audio Output.&lt;/b&gt;</source>
        <translation>&lt;b&gt;Sélectionner quelle carte son utiliser pour la sortie sonore.&lt;/b&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Volume tuning</source>
        <translation>Réglage du volume</translation>
    </message>
    <message>
        <location/>
        <source>Tuning microphone hardware volume to optimal settings.</source>
        <translation>Réglage du volume matériel du micro pour une meilleure configuration.</translation>
    </message>
    <message>
        <source>Open your sound control panel and go to the recording settings. Make sure the microphone is selected as active input with maximum recording volume. If there&apos;s an option to enable a &quot;Microphone boost&quot; make sure it&apos;s checked.</source>
        <translation type="obsolete">Ouvrir le panneau de contrôle de votre carte son et aller dans les options d&apos;enregistrements. Vérifiez que le microphone est séléctionné en tant qu&apos;entrée active avec le volume d&apos;enregistrement au maximum. S&apos;il ya une option pour permettre le &quot;mic boost&quot; vérifiez qu&apos;il est cochée.</translation>
    </message>
    <message>
        <source>Speak loudly, as when you are annoyed or excited. Decrease the volume in the sound control panel until the bar below stays as high as possible in the blue and green but &lt;b&gt;not&lt;/b&gt; the red zone while you speak.</source>
        <translation type="obsolete">Parler fort, comme lorsque vous êtes ennuyé ou excitée. Diminuer le volume sonore dans le panneau de contrôle jusqu&apos;à ce que la barre soit aussi haute que possible dans le bleu et le vert, mais &lt;b&gt;pas&lt;/ b&gt; dans la zone rouge pendant que vous parlez.</translation>
    </message>
    <message>
        <location/>
        <source>Now talk softly, as you would when talking late at night and you don&apos;t want to disturb anyone. Adjust the slider below so that the bar moves into green when you talk, but stays blue while you&apos;re silent.</source>
        <translation>Maintenant, parlez doucement, comme vous le feriez lorsque l&apos;on parle tard dans la nuit et que vous ne voulez pas déranger qui que ce soit. Ajustez le curseur ci-dessous pour que la barre se déplace dans le vert lorsque vous parlez, mais reste bleu quand vous êtes silencieux.</translation>
    </message>
    <message>
        <location/>
        <source>Voice Activity Detection</source>
        <translation>Détection de l&apos;Activité Vocale</translation>
    </message>
    <message>
        <location/>
        <source>Letting Mumble figure out when you&apos;re talking and when you&apos;re silent.</source>
        <translation>Mumble détermine quand vous parlez, et lorsque vous faites le silence.</translation>
    </message>
    <message>
        <location/>
        <source>This will help Mumble figure out when you are talking. The first step is selecting which data value to use.</source>
        <translation>Cela permettra de Mumble savoir quand vous parlez. La première étape consiste à sélectionner les données de valeur à utiliser.</translation>
    </message>
    <message>
        <location/>
        <source>Raw amplitude from input</source>
        <translation>Amplitude d&apos;entrée</translation>
    </message>
    <message>
        <location/>
        <source>Signal-To-Noise ratio</source>
        <translation>Taux de signal/bruit</translation>
    </message>
    <message>
        <location/>
        <source>Next you need to adjust the following slider. The first few utterances you say should end up in the green area (definitive speech). While talking, you should stay inside the yellow (might be speech) and when you&apos;re not talking, everything should be in the red (definitively not speech).</source>
        <oldsource>Next you need to adjust the following two sliders. The first few utterances you say should end up in the green area (definitive speech). While talking, you should stay inside the yellow (might be speech) and when you&apos;re not talking, everything should be in the red (definitively not speech).</oldsource>
        <translation>Ensuite, vous devez ajuster les deux curseurs. Les premières syllabes des phares devraient se trouver dans la zone verte (voix confirmée). Tout en parlant, vous devriez rester à l&apos;intérieur du jaune (voix hypotétique), et lorsque vous ne parlez pas, tout devrait être dans le rouge (aucune voix).</translation>
    </message>
    <message>
        <source>Last, you need to adjust the grace time. Mumble may detect this much non-speech and still keep transmitting before it breaks off. This allows you to catch your breath while speaking.</source>
        <translation type="obsolete">Enfin, vous avez besoin d&apos;ajuster le temps de parole. Mumble peut détecter le temps de non-parole et toujours transmettre avant qu&apos;il se rompt. Cela vous permet de reprendre votre souffle.</translation>
    </message>
    <message>
        <location/>
        <source>Device tuning</source>
        <translation>Réglage périphérique</translation>
    </message>
    <message>
        <location/>
        <source>Changing hardware output delays to their minimum value.</source>
        <translation>Changer le délai de sortie matériel à sa valeur minimale.</translation>
    </message>
    <message>
        <source>Amount of data to buffer.</source>
        <translation type="obsolete">Quantitée de données à mettre en tampon.</translation>
    </message>
    <message>
        <source>This sets the amount of data to prebuffer in the output buffer. Experiment with different values and set it to the lowest which doesn&apos;t cause rapid jitter in the sound.</source>
        <translation type="obsolete">Spécifie la quantité de données à mettre en cache dans le tampon de sortie. Essayez différentes valeurs et mettez la plus basse qui ne cause pas de rapides décalages du son.</translation>
    </message>
    <message>
        <location filename="AudioWizard.cpp" line="+306"/>
        <source>%1 ms</source>
        <oldsource>%1ms</oldsource>
        <translation>%1ms</translation>
    </message>
    <message>
        <source>%1 s</source>
        <translation type="obsolete">%1 s</translation>
    </message>
    <message>
        <source>Audio path is %1ms long.</source>
        <translation type="obsolete">Le chemin audio est de %1ms.</translation>
    </message>
    <message>
        <source>Audio path cannot be determined. Input not recognized.</source>
        <translation type="obsolete">Le chemin audio ne peut pas être déterminé.</translation>
    </message>
    <message>
        <location filename="AudioWizard.ui"/>
        <source>Use echo cancellation</source>
        <translation>Utiliser l&apos;annulation de l&apos;écho</translation>
    </message>
    <message>
        <source>Cancel echo from headset or speakers.</source>
        <translation type="obsolete">Annuler l&apos;écho depuis les hauts parleurs ou écouteurs.</translation>
    </message>
    <message>
        <location/>
        <source>This enables echo cancellation of outgoing audio, which helps both on speakers and on headsets.</source>
        <translation>Ceci active l&apos;annulation d&apos;écho de la sortie audio, ce qui aide à la fois la parole et les écouteurs.</translation>
    </message>
    <message>
        <location/>
        <source>Enable positional audio</source>
        <translation>Activer la position audio</translation>
    </message>
    <message>
        <location/>
        <source>Allows positioning of sound</source>
        <oldsource>Allows positioning of sound.</oldsource>
        <translation>Autoriser la position audio</translation>
    </message>
    <message>
        <location/>
        <source>&lt;p&gt;
This is the audio tuning wizard for Mumble. This will help you correctly set the input levels of your sound card, and also set the correct parameters for sound processing in Mumble.
&lt;/p&gt;
&lt;p&gt;
Please be aware that as long as this wizard is active, audio will be looped locally to allow you to listen to it, and no audio will be sent to the server.
&lt;/p&gt;</source>
        <translation>&lt;p&gt;C&apos;est l&apos;assistant de configuration audio de Mumble. Il vous aidera à régler correctement les niveaux d&apos;entrée de votre carte son, ainsi que les paramètres corrects pour le traitement du son sous Mumble.&lt;/p&gt;
&lt;p&gt;Notez que tant que cet assistant sera actif, le son sera joué localement pour vous permettre de l&apos;écouter, et aucun son ne sera envoyé au serveur.&lt;/p&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Input Device</source>
        <translation>Périphérique d&apos;entrée</translation>
    </message>
    <message>
        <location/>
        <source>Cancel echo from headset or speakers</source>
        <translation>Suppression de l&apos;écho du casque ou des haut-parleurs</translation>
    </message>
    <message>
        <location/>
        <source>Output Device</source>
        <translation>Périphérique de sortie</translation>
    </message>
    <message>
        <location/>
        <source>This allows Mumble to use positional audio to place voices.</source>
        <translation>Ceci autorise Mumble à utiliser la postion audio.</translation>
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
        <translation>&lt;p&gt;Pour maintenir une latence à la valeur minimale, il est important de spéficier un tampon aussi petit que possible sur votre carte son. Cependant, plusieurs cartes sons rapportent qu&apos;elles nécessitent un tampon plus petit que nécessaire pour un fonctionnement correct, la seule façon de régler cette valeur est d&apos;essayer diverses valeurs.&lt;/p&gt;
&lt;p&gt;Vous devez entendre une voix exemple. Déplacez le curseur sur la valeur la plus petite ne donnant &lt;b&gt;aucune&lt;/b&gt; interruption ni décalage de son. Veuillez noter que la suppression de l&apos;écho est désactivée durant ce test.&lt;/p&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Amount of data to buffer</source>
        <translation>Quantité de données à mettre en tampon</translation>
    </message>
    <message>
        <location/>
        <source>This sets the amount of data to pre-buffer in the output buffer. Experiment with different values and set it to the lowest which doesn&apos;t cause rapid jitter in the sound.</source>
        <translation>Spécifie la quantité de données à mettre en cache dans le tampon de sortie. Essayez différentes valeurs et mettez la plus basse qui ne cause pas de rapides décalages du son.</translation>
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
        <translation>&lt;p&gt;Ouvrez votre panneau de configuration de la carte son, et allez aux paramètres d&apos;enregistrement. Veuillez vous assurer que le microphone est sélectionné comme périphérique actif avec le volume réglé au maximum. Si il y a une option permettant de &quot;Booster le microphone&quot;, assurez vous qu&apos;elle soit cochée.&lt;/p&gt;
&lt;p&gt;Parlez fort, comme lorsque vous êtes irrité ou excité. Diminuer le volume dans le panneau de contrôle sonore jusqu&apos;à ce que la barre ci-dessous reste le plus haut possible dans le bleu et le vert mais &lt;b&gt;pas&lt;/b&gt; dans la zone rouge quand vous parlez.&lt;/p&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Quality &amp; Notifications</source>
        <translation>Qualité &amp; Notifications</translation>
    </message>
    <message>
        <location/>
        <source>Adjust quality and notification settings.</source>
        <translation>Changer les paramètres de qualité et de notification.</translation>
    </message>
    <message>
        <location/>
        <source>Quality settings</source>
        <translation>Paramètres de qualité</translation>
    </message>
    <message>
        <location/>
        <source>Low</source>
        <translation>Bas</translation>
    </message>
    <message>
        <location/>
        <source>Balanced</source>
        <translation>Normal</translation>
    </message>
    <message>
        <location/>
        <source>Notification settings</source>
        <translation>Paramètres de notification</translation>
    </message>
    <message>
        <location/>
        <source>Use Text-To-Speech to read notifications and messages to you.</source>
        <translation>Utiliser la synthèse vocal pour lire vos notifications et messages.</translation>
    </message>
    <message>
        <location/>
        <source>Disable Text-To-Speech and use sounds instead.</source>
        <translation>Désactiver la synthèse vocale et utiliser des sons à la place.</translation>
    </message>
    <message>
        <location/>
        <source>Positional Audio</source>
        <translation>Position Audio</translation>
    </message>
    <message>
        <location/>
        <source>Adjusting attenuation of positional audio.</source>
        <translation>Ajuster l&apos;attenuation de la position audio.</translation>
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
Mumble supporte le positionnement audio dans certains jeux, et positionne la voix des autres utilisateurs par rapport à leur position dans le jeu. En fonction de leur position, le volume de la voix sera adapté entre les haut-parleurs pour simuler leur direction et leur éloignement. Un tel positionnement dépend d&apos;une bonne configuration de vos haut-parleurs, elle est donc testée ici.
&lt;/p&gt;
&lt;p&gt;
Le dessin ci-dessous montre &lt;font color=&quot;red&quot;&gt;votre&lt;/font&gt; position, celle de vos &lt;font color=&quot;yellow&quot;&gt;haut-parleurs&lt;/font&gt; et une &lt;fond color=&quot;green&quot;&gt;source sonore en mouvement&lt;/font&gt; comme si vous étiez vu de dessus. Vous deviez entendre le son se déplacer entre les canaux.
&lt;/p&gt;
</translation>
    </message>
    <message>
        <location/>
        <source>Use headphones instead of speakers</source>
        <translation>Utiliser un casque à la place de haut-parleurs</translation>
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
        <translation>&lt;p&gt;Félicitations. Vous êtes prêt à bénéficier d&apos;une meilleure expérience sonore avec Mumble.&lt;/p&gt;
&lt;p&gt;Mumble est en développement continu, et l&apos;équipe de développement veut se concentrer sur les options qui vont bénéficier au maximum d&apos;utilisateurs. Pour y parvenir, Mumble propose d&apos;envoyer des statistiques anonymes à propos de votre configuration aux développeurs. Ces statistiques sont essentiels pour les développements futurs, mais aussi pour s&apos;assurer que les fonctions que vous utilisez ne sont pas obsolètes.&lt;/p&gt;</translation>
    </message>
    <message>
        <source>Mumble supports positional audio for some games, and will position the voice of other players relative to their position in game. Depending on their position, the volume of the voice will be changed between the speakers to simulate the direction and distance the other player is at. Such positioning depends on your speaker configuration being correct in your operating system, so a test is done here.</source>
        <translation type="obsolete">Mumble supporte la position audio pour quelques jeux, et et la position de la voix des autres joueurs par rapport à leur position dans le jeu. Selon leur position, le volume de la voix sera modifiée entre les intervenants afin de simuler la direction et la distance à laquelle l&apos;autre joueur se trouve. Cette position dépend de votre bonne configuration des haut-parleurs au niveau de votre système d&apos;exploitation, donc un test est fait ici.</translation>
    </message>
    <message>
        <location/>
        <source>Use headphones</source>
        <translation>Utiliser les écouteurs</translation>
    </message>
    <message>
        <source>Use headphones instead of speakers.</source>
        <translation type="obsolete">Utiliser les écouteurs plutôt que les hauts parleurs.</translation>
    </message>
    <message>
        <location/>
        <source>This ignores the OS speaker configuration and configures the positioning for headphones instead.</source>
        <translation>Ceci ignore la configuration des haut parleurs du système d&apos;exploitation et configure la position pour les écouteurs à la place.</translation>
    </message>
    <message>
        <location/>
        <source>Submit anonymous statistics to the Mumble project</source>
        <translation>Soumettre les statistiques anonyme au projet Mumble</translation>
    </message>
    <message>
        <location/>
        <source>Push To Talk:</source>
        <translation>Appuyez pour parler :</translation>
    </message>
    <message>
        <location/>
        <source>Enables attenuation of other applications while users talk to you</source>
        <translation>Activer l&apos;atténuation des autres applications lors de l&apos;écoute</translation>
    </message>
    <message>
        <location/>
        <source>Attenuate applications while other users talk</source>
        <translation>Atténue les applications lorsque d&apos;autres utilisateurs parlent</translation>
    </message>
    <message>
        <location/>
        <source>In this configuration Mumble will use a &lt;b&gt;low amount of bandwidth&lt;/b&gt;. This will inevitably result in high latency and poor quality. Choose this only if your connection cannot handle the other settings. (Speex 16kbit/s, 60ms per packet)</source>
        <translation>Dans cette configuration, Mumble utilisera &lt;b&gt;peu de bande passante&lt;/b&gt;. Ceci impliquera une forte latence ainsi qu&apos;une mauvaise qualité. Choisissez ceci si votre connexion ne peut supporter l&apos;autre réglage. (Speex 16kbit/s, 60ms par paquet)</translation>
    </message>
    <message>
        <location/>
        <source>This is the &lt;b&gt;recommended default&lt;/b&gt; configuration. It provides a good balance between quality, latency, and bandwidth usage. (CELT 40kbit/s, 20ms per packet)</source>
        <translation>C&apos;est la configuration &lt;b&gt;par défaut et recommandée&lt;/b&gt;. Elle apporte un bon équilibre entre qualité, latence et utilisation de bande passante. (CELT 40kbit/s, 20ms par paquet)</translation>
    </message>
    <message>
        <location/>
        <source>High</source>
        <translation>Haut</translation>
    </message>
    <message>
        <location/>
        <source>This configuration is only recommended for use in setups where bandwidth is not an issue, like a LAN. It provides the lowest latency supported by Mumble and &lt;b&gt;high quality&lt;/b&gt;. (CELT 72kbit/s, 10ms per packet)</source>
        <translation>Cette configuration n&apos;est recommandée que lorsque la bande passante n&apos;est pas un problème, comme en LAN. Elle apporte la plus faible latence supportée par Mumble et une &lt;b&gt;grande qualité&lt;/b&gt;. (CELT 72kbit/s, 10ms par paquet)</translation>
    </message>
    <message>
        <location/>
        <source>Custom</source>
        <translation>Personnalisé</translation>
    </message>
    <message>
        <location/>
        <source>You already set a customized quality configuration in Mumble. Select this setting to keep it.</source>
        <translation>Vous avez déjà configuré une qualité personnalisée pour Mumble. Sélectionnez ceci pour garder cette configuration.</translation>
    </message>
    <message>
        <location/>
        <source>Enables attenuation of other applications while users talk to you. This means that as soon someone starts to speak to you in Mumble, the sound of all other applications (like audio players) will get attenuated so you can hear them more clearly.</source>
        <translation>Active l&apos;atténuation des applications lors de l&apos;écoute. Cela signifie que dès que quelqu&apos;un commence à vous parler via Mumble, le son de toutes les autres applications (comme un lecteur audio) sera réduit afin que vous puissiez mieux l&apos;entendre.</translation>
    </message>
    <message>
        <location/>
        <source>Apply some high contrast optimizations for visually impaired users</source>
        <translation>Applique quelques optimisations de contraste pour les utilisateurs malvoyants</translation>
    </message>
    <message>
        <location/>
        <source>Use high contrast graphics</source>
        <translation>Augmenter le contraste</translation>
    </message>
    <message>
        <location/>
        <source>Keep custom Text-To-Speech settings.</source>
        <translation>Garder les paramètres personnalisés de synthèse vocale.</translation>
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
Ouvrez votre panneau de configuration de la carte son, et allez aux paramètres d&apos;enregistrement. Veuillez vous assurer que le microphone est sélectionné comme périphérique actif avec le volume réglé au maximum. Si il y a une option permettant de &quot;Booster le microphone&quot;, assurez vous qu&apos;elle soit cochée.
&lt;/p&gt;
&lt;p&gt;
Parlez fort, comme lorsque vous êtes irrité ou excité. Diminuez le volume dans le panneau de contrôle sonore jusqu&apos;à ce que la barre ci-dessous reste le plus haut possible dans le bleu et le vert mais &lt;b&gt;pas&lt;/b&gt; dans la zone rouge quand vous parlez.
&lt;/p&gt;
</translation>
    </message>
    <message>
        <location/>
        <source>Now talk softly, as you would when talking late at night and you don&apos;t want to disturb anyone. Adjust the slider below so that the bar moves into empty zone when you talk, but stays in the striped one while you&apos;re silent.</source>
        <comment>For high contrast mode</comment>
        <translation>Maintenant, parlez doucement, comme vous le feriez lorsque l&apos;on parle tard dans la nuit et que vous ne voulez pas déranger qui que ce soit. Ajustez le curseur ci-dessous pour que la barre se déplace dans le vert lorsque vous parlez, mais reste bleu quand vous êtes silencieux.</translation>
    </message>
    <message>
        <location/>
        <source>Next you need to adjust the following slider. The first few utterances you say should end up in the empty area (definitive speech). While talking, you should stay inside the striped (might be speech) and when you&apos;re not talking, everything should be in the crisscrossed (definitively not speech).</source>
        <comment>For high contrast mode</comment>
        <translation>Ensuite, vous devez ajuster les deux curseurs. Les premières syllabes des phares devraient se trouver dans la zone verte (voix confirmée). Tout en parlant, vous devriez rester à l&apos;intérieur du jaune (voix hypotétique), et lorsque vous ne parlez pas, tout devrait être dans le rouge (aucune voix).</translation>
    </message>
</context>
<context>
    <name>BanEditor</name>
    <message>
        <location filename="BanEditor.ui"/>
        <source>Mumble - Edit Bans</source>
        <translation>Mumble - Éditer la liste des bannis</translation>
    </message>
    <message>
        <source>0.0.0.0</source>
        <translation type="obsolete">0.0.0.0</translation>
    </message>
    <message>
        <location/>
        <source>Reason</source>
        <translation>Raison</translation>
    </message>
    <message>
        <location/>
        <source>Start</source>
        <translation>Début</translation>
    </message>
    <message>
        <location/>
        <source>End</source>
        <translation>Fin</translation>
    </message>
    <message>
        <location/>
        <source>User</source>
        <translation>Utilisateur</translation>
    </message>
    <message>
        <location/>
        <source>Hash</source>
        <translation>Hash</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Add</source>
        <translation>&amp;Ajouter</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Update</source>
        <translation>Mettre à jo&amp;ur</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Remove</source>
        <translation>Supp&amp;rimer</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Valider</translation>
    </message>
    <message>
        <source>Accept changes</source>
        <translation type="obsolete">Accepter les modifications</translation>
    </message>
    <message>
        <source>This button will accept current groups/ACLs and send them to the server. Note that if you mistakenly remove write permission from yourself, the server will add it.</source>
        <translation type="obsolete">Ce bouton acceptera les groupes/LCAs actuels et les enverra au serveur. Veuillez noter que si vous avez retiré par erreur votre permission d&apos;écriture, le serveur va l&apos;ajouter.</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">A&amp;nnuler</translation>
    </message>
    <message>
        <source>Reject changes</source>
        <translation type="obsolete">Annuler les modifications</translation>
    </message>
    <message>
        <source>This button will cancels all changes and closes the dialog without updating the ACLs or groups on the server.</source>
        <translation type="obsolete">Ce bouton va annuler les modifications et fermer la fenêtre sans mettre à jour les LCAs ou les groupes sur le serveur.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Address</source>
        <translation>&amp;Adresse</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Mask</source>
        <translation>&amp;Masque</translation>
    </message>
</context>
<context>
    <name>CertView</name>
    <message>
        <location filename="Cert.cpp" line="+50"/>
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Email</source>
        <translation>Courriel</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Issuer</source>
        <translation>Émetteur</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Expiry Date</source>
        <translation>Date d&apos;expiration</translation>
    </message>
    <message>
        <location line="+34"/>
        <source>(none)</source>
        <translation>(aucun)</translation>
    </message>
    <message>
        <location line="+11"/>
        <source>Self-signed</source>
        <translation>Auto-signé</translation>
    </message>
</context>
<context>
    <name>CertWizard</name>
    <message>
        <location line="+90"/>
        <source>Resolving domain %1.</source>
        <translation>Résolution du domaine %1.</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Unable to validate email.&lt;br /&gt;Enter a valid (or blank) email to continue.</source>
        <translation>Incapable de valider le courriel.&lt;br/&gt;Saisissez une adresse de courriel valide (ou vide) pour continuer.</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>There was an error generating your certificate.&lt;br /&gt;Please try again.</source>
        <translation>Une erreur est survenue durant la génération de votre certificat.&lt;br/&gt;Veuillez réessayer.</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Your certificate and key could not be exported to PKCS#12 format. There might be an error in your certificate.</source>
        <translation>Votre certificat et votre clé ne peuvent pas être exportés au format PKCS#12. Il doit y avoir une erreur dans votre certificat.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>The file could not be opened for writing. Please use another file.</source>
        <translation>Le fichier ne peut pas être ouvert en ecriture. Veuillez utiliser un autre fichier.</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>The file could not be written successfully. Please use another file.</source>
        <translation>Le fichier n&apos;a pas pu être enregistré correctement. Veuillez utiliser un autre fichier.</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>The file could not be opened for reading. Please use another file.</source>
        <translation>Le fichier ne peut pas être lu. Veuillez utiliser un autre fichier.</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>The file is empty or could not be read. Please use another file.</source>
        <translation>Le fichier est vide ou ne peut être lu. Veuillez utiliser un autre fichier.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>The file did not contain a valid certificate and key. Please use another file.</source>
        <translation>Le fichier ne contient pas de clé et de certificat valide. Veuillez utiliser un autre fichier.</translation>
    </message>
    <message>
        <location line="+22"/>
        <source>Select file to export certificate to</source>
        <translation>Sélectionnez le fichier dans lequel exporter le certificat</translation>
    </message>
    <message>
        <location line="+37"/>
        <source>Select file to import certificate from</source>
        <translation>Sélectionnez le fichier à partir duquel importer le certificat</translation>
    </message>
    <message>
        <location line="+57"/>
        <source>Unable to resolve domain.</source>
        <translation>Incapable de résoudre le domaine.</translation>
    </message>
    <message>
        <location line="+43"/>
        <source>Mumble User</source>
        <translation>Utilisateur Mumble</translation>
    </message>
    <message>
        <location filename="main.cpp" line="+425"/>
        <source>&lt;b&gt;Certificate Expiry:&lt;/b&gt; Your certificate is about to expire. You need to renew it, or you will no longer be able to connect to servers you are registered on.</source>
        <translation>&lt;b&gt;Expiration du certificat :&lt;/b&gt; Votre certificat va bientôt expirer. Vous devez le renouveler ou vous ne pourrez plus vous connecter aux serveurs auxquels vous êtes enregistrés.</translation>
    </message>
</context>
<context>
    <name>Certificates</name>
    <message>
        <location filename="Cert.ui"/>
        <source>Certificate Management</source>
        <translation>Gestion du Certificat</translation>
    </message>
    <message>
        <source>This opens a file selection dialog to choose a file to export the certificate to.</source>
        <translation type="obsolete">Ouvre une fenêtre de sélection pour choisir le fichier devant contenir le certificat exporté.</translation>
    </message>
    <message>
        <location/>
        <source>Certificate Authentication</source>
        <translation>Authentification par certificat</translation>
    </message>
    <message>
        <location/>
        <source>Authenticating to servers without using passwords</source>
        <translation>Authentification aux serveurs sans utiliser de mots de passe</translation>
    </message>
    <message>
        <source>&lt;p&gt;Mumble can use certificates to authenticate with servers. Using certificates avoids passwords, meaning you don&apos;t need to disclose any password to the remote site. It also enables very easy user registration.&lt;/p&gt;&lt;p&gt;While Mumble can work without certificates, the majority of servers will expect you to have one.&lt;/p&gt;</source>
        <translation type="obsolete">&lt;p&gt;Mumble peut utiliser des certificats pour s&apos;authentifier avec les serveurs. L&apos;usage des certificats évite d&apos;utiliser des mots de passe, vous ne devez donc pas les divulguer à des sites distants. Il permet aussi de s&apos;enregistrer vraiment facilement.&lt;/p&gt;&lt;p&gt;Même si Mumble peut fonctionner sans certificat, la majorité des serveurs s&apos;attendent à ce que vous en ayez un.&lt;/p&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Current certificate</source>
        <translation>Certificat actuel</translation>
    </message>
    <message>
        <location/>
        <source>This is the certificate Mumble currently uses.</source>
        <translation>C&apos;est le certificat que Mumble utilise actuellement.</translation>
    </message>
    <message>
        <location/>
        <source>Current Certificate</source>
        <translation>Certificat actuel</translation>
    </message>
    <message>
        <location/>
        <source>Create a new certificate</source>
        <translation>Créer un nouveau certificat</translation>
    </message>
    <message>
        <location/>
        <source>This will create a new certificate.</source>
        <translation>Créera un nouveau certificat.</translation>
    </message>
    <message>
        <location/>
        <source>Import certificate from file</source>
        <translation>Importer un certificat depuis un fichier</translation>
    </message>
    <message>
        <location/>
        <source>This will import a certificate from file.</source>
        <translation>Importera un certificat depuis un fichier.</translation>
    </message>
    <message>
        <location/>
        <source>Import a certificate</source>
        <translation>Importer un certificat</translation>
    </message>
    <message>
        <location/>
        <source>Export Certificate</source>
        <translation>Exporter un certificat</translation>
    </message>
    <message>
        <location/>
        <source>This will export a certificate to file.</source>
        <translation>Exportera le certificat vers un fichier.</translation>
    </message>
    <message>
        <location/>
        <source>Export current certificate</source>
        <translation>Exporter le certificat actuel</translation>
    </message>
    <message>
        <location/>
        <source>Import Certificate</source>
        <translation>Importer un certificat</translation>
    </message>
    <message>
        <location/>
        <source>PKCS #12 Certificate import</source>
        <translation>Importation de certificat PKCS #12</translation>
    </message>
    <message>
        <location/>
        <source>&lt;p&gt;Mumble can import certificates stored in PKCS #12 format. This is the format used when exporting a key from Mumble, and also when exporting keys from Firefox, Internet Explorer, Opera etc.&lt;/p&gt;&lt;p&gt;If the file is password protected, you will need the password to import the certificate.&lt;/p&gt;</source>
        <oldsource>&lt;p&gt;Mumble can import certificates stored in PKCS #12 format. This is the format used when exporting a key from Mumble, and also when exporting keys from FireFox, Internet Explorer, Opera etc.&lt;/p&gt;&lt;p&gt;If the file is password protected, you will need the password to import the certificate.&lt;/p&gt;</oldsource>
        <translation>&lt;p&gt;Mumble peut importer des certificats stockés au format PKCS #12. C&apos;est le format utilisé lors de l&apos;exportation de la clé de Mumble, et aussi lors de l&apos;export de clés depuis Firefox, Internet Explorer, Opera, etc.&lt;/p&gt;&lt;p&gt;Si le fichier est protégé par un mot de passe, vous avez besoin de celui-ci pour importer le certificat.&lt;/p&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Import from</source>
        <translation>Importer depuis</translation>
    </message>
    <message>
        <location/>
        <source>Filename to import from</source>
        <translation>Nom de fichier à importer</translation>
    </message>
    <message>
        <location/>
        <source>This is the filename you wish to import a certificate from.</source>
        <translation>C&apos;est le fichier à partir duquel vous souhaitez importer le certificat.</translation>
    </message>
    <message>
        <location/>
        <source>Select file to import from</source>
        <translation>Sélectionnez le fichier à importer</translation>
    </message>
    <message>
        <location/>
        <source>This opens a file selection dialog to choose a file to import a certificate from.</source>
        <translation>Ouvre la boite de dialogue pour sélectionner le fichier de certificat à importer.</translation>
    </message>
    <message>
        <location/>
        <source>Open...</source>
        <translation>Ouvrir...</translation>
    </message>
    <message>
        <location/>
        <source>Password</source>
        <translation>Mot de passe</translation>
    </message>
    <message>
        <location/>
        <source>Password for PKCS#12 file</source>
        <translation>Fichier pour le mot de passe au format PKCS #12</translation>
    </message>
    <message>
        <location/>
        <source>This is the password for the PKCS#12 file containing your certificate.</source>
        <translation>C&apos;est le mot de passe pour le fichier PKCS #12 contenant votre certificat.</translation>
    </message>
    <message>
        <location/>
        <source>Certificate to import</source>
        <translation>Certificat à importer</translation>
    </message>
    <message>
        <location/>
        <source>This is the certificate you are importing.</source>
        <translation>C&apos;est le certificat que vous aller importer.</translation>
    </message>
    <message>
        <location/>
        <source>Certificate Details</source>
        <translation>Détails du certificat</translation>
    </message>
    <message>
        <location/>
        <source>Replace Certificate</source>
        <translation>Remplacer le certificat</translation>
    </message>
    <message>
        <location/>
        <source>Replace existing certificate with new certificate?</source>
        <translation>Remplacer le certificat existant par le nouveau ?</translation>
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
        <translation>&lt;p&gt;Vous avez déjà un certificat enregistré dans Mumble, et vous êtes sur le point de le remplacer.&lt;/p&gt;
&lt;p&gt;Si vous êtes en train de le mettre à jour avec un certificat certifié par une authorité de confiance, et que les adresses de courriel correspondent, cette opération est sûre, et les serveurs auxquels vous vous connectez reconnaîtrons le certificat de votre adresse de courriel.&lt;/p&gt;
&lt;p&gt;Si ce n&apos;est pas le cas, vous ne pourrez plus être reconnu par les serveurs auxquels vous vous êtes précédement authentifiés. Si vous ne vous êtes enregistré sur aucun serveur, vous n&apos;avez pas de soucis à vous faire.&lt;/p&gt;
&lt;p&gt;Êtes-vous sûr de vouloir remplacer votre certificat ?&lt;/p&gt;</translation>
    </message>
    <message>
        <location/>
        <source>This is the certificate Mumble currently uses. It will be replaced.</source>
        <translation>C&apos;est le certificat Mumble que vous utilisez actuellement. Il sera remplacé.</translation>
    </message>
    <message>
        <location/>
        <source>New certificate</source>
        <translation>Nouveau certificat</translation>
    </message>
    <message>
        <location/>
        <source>This is the new certificate that will replace the old one.</source>
        <translation>C&apos;est le nouveau certificat qui va remplacer l&apos;ancien.</translation>
    </message>
    <message>
        <location/>
        <source>New Certificate</source>
        <translation>Nouveau certificat</translation>
    </message>
    <message>
        <location/>
        <source>Make a backup of your certificate</source>
        <translation>Faire une sauvegarde de votre certificat</translation>
    </message>
    <message>
        <source>&lt;p&gt;If you ever lose your current certificate, which will happen if your computer suffers a hardware failure or you reinstall your machine, you will no longer be able to authenticate to any server you are registered on. It is therefore &lt;b&gt;mandatory&lt;/b&gt; that you make a backup of your certificate. We strongly recommend you store this backup on removable storage, such as an USB memory stick.&lt;/p&gt;
&lt;p&gt;Note that this file will not be encrypted, and if anyone gains access to it, they will be able to impersonate you, so take good care of it.&lt;/p&gt;</source>
        <translation type="obsolete">&lt;p&gt;Si vous perdez votre certificat, ce qui peut se produire si votre ordinateur est victime d&apos;une panne matérielle ou si vous réinstallez votre machine, vous ne pourrez plus vous authentifier sur aucun des serveurs sur lesquels vous vous étiez enregistré. Il est donc &lt;b&gt;obligatoire&lt;/b&gt; de faire une sauvegarde de votre certificat. Nous recommandons fortement de le sauvegarder sur un stockage amovible, tel une clé USB.&lt;/p&gt;
&lt;p&gt;Notez que ce fichier ne sera pas chiffré, et que toute personne qui mettra la main dessus pourra se faire passer pour vous, prenez donc ceci en considération.&lt;/p&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Export to</source>
        <translation>Exporter vers</translation>
    </message>
    <message>
        <location/>
        <source>Filename to export to</source>
        <translation>Nom du fichier exporté</translation>
    </message>
    <message>
        <location/>
        <source>This is the filename you wish to export a certificate to.</source>
        <translation>C&apos;est le nom de fichier dans lequel vous souhaitez exporter votre certificat.</translation>
    </message>
    <message>
        <location/>
        <source>Save As...</source>
        <translation>Enregistrer sous...</translation>
    </message>
    <message>
        <location/>
        <source>This is the certificate Mumble currently uses. It will be exported.</source>
        <translation>C&apos;est le certificat que Mumble utilise actuellement. Il sera exporté.</translation>
    </message>
    <message>
        <location/>
        <source>Generate a new certificate for strong authentication</source>
        <translation>Génère un nouveau certificat pour l&apos;authentification forte</translation>
    </message>
    <message>
        <location/>
        <source>&lt;p&gt;Mumble will now generate a strong certificate for authentication to servers.&lt;/p&gt;&lt;p&gt;If you wish, you may provide some additional information to be stored in the certificate, which will be presented to servers when you connect. If you provide a valid email address, you can upgrade to a CA issued email certificate later on, which provides strong identification.&lt;/p&gt;</source>
        <translation>&lt;p&gt;Mumble va maintenant générer un certificat pour l&apos;authentification forte avec les serveurs.&lt;/p&gt;&lt;p&gt;Si vous voulez, vous pouvez spécifier des informations additionnelles à enregistrer dans le certificat, qui seront présentées aux serveurs lors de vos connexions. Si vous fournissez une adresse de courriel valide, vous pouvez demander à une autorité de certification de vous délivrer un certificat courriel plus tard, qui permet une identification forte.&lt;/p&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <location/>
        <source>Email</source>
        <translation>Courriel</translation>
    </message>
    <message>
        <location/>
        <source>Your email address (e.g. johndoe@mumble.info)</source>
        <translation>Votre adresse de courriel (ex. jacquesdupond@mumble.info)</translation>
    </message>
    <message>
        <location/>
        <source>This is your email address. It is strongly recommended to provide a valid email address, as this will allow you to upgrade to a strong certificate without authentication problems.</source>
        <translation>C&apos;est votre adresse de courriel. Il est fortement recommandé de fournir une adresse de courriel valide, car cela vous permettra de passer à un certificat d&apos;authentification forte sans problèmes.</translation>
    </message>
    <message>
        <location/>
        <source>Your name (e.g. John Doe)</source>
        <translation>Votre nom (ex. Jacques Dupond)</translation>
    </message>
    <message>
        <location/>
        <source>This is your name, and will be filled out in the certificate. This field is entirely optional.</source>
        <translation>C&apos;est votre nom, et celui-ci sera recopié dans le certificat. Ce champ est entièrement optionnel.</translation>
    </message>
    <message>
        <location/>
        <source>Finish</source>
        <translation>Terminer</translation>
    </message>
    <message>
        <location/>
        <source>Certificate-based authentication is ready for use</source>
        <translation>L&apos;authentification par certificat est prête à être utilisée</translation>
    </message>
    <message>
        <location/>
        <source>Enjoy using Mumble with strong authentication.</source>
        <translation>Profitez de l&apos;authentification forte avec Mumble.</translation>
    </message>
    <message>
        <location/>
        <source>Automatic certificate creation</source>
        <translation>Création automatique d&apos;un certificat</translation>
    </message>
    <message>
        <location/>
        <source>&lt;p&gt;Mumble can use certificates to authenticate with servers. Using certificates avoids passwords, meaning you don&apos;t need to disclose any password to the remote site. It also enables very easy user registration.&lt;/p&gt;&lt;p&gt;While Mumble can work without certificates, the majority of servers will expect you to have one.&lt;/p&gt;
&lt;p&gt;
It is &lt;b&gt;strongly&lt;/b&gt; recommended that you &lt;a href=&quot;http://mumble.info/certificate.php&quot;&gt;create a trusted certificate&lt;/a&gt;.
&lt;/p&gt;
</source>
        <translation>&lt;p&gt;Mumble utilise des certificat pour l&apos;authentification avec les serveurs. L&apos;utilisation de certificats permet d&apos;éviter les mots de passe, ainsi, vous n&apos;avez pas besoin de saisir de mot de passe pour vous connecter. Cela permet aussi de simplifier l&apos;enregistrement des utilisateurs.&lt;/p&gt;&lt;p&gt;Mumble peut fonctionner sans certificats, cependant, la majorité des serveurs vous en demanderont un.&lt;/p&gt;
&lt;p&gt;
Il est &lt;b&gt;fortement&lt;/b&gt; conseillé de &lt;a href=&quot;http://mumble.info/certificate.php&quot;&gt;créer un certificat de confiance&lt;/a&gt;.
&lt;/p&gt;</translation>
    </message>
    <message>
        <location/>
        <source>&lt;p&gt;If you ever lose your current certificate, which will happen if your computer suffers a hardware failure or you reinstall your machine, you will no longer be able to authenticate to any server you are registered on. It is therefore &lt;b&gt;mandatory&lt;/b&gt; that you make a backup of your certificate. We strongly recommend you store this backup on removable storage, such as a USB flash drive.&lt;/p&gt;
&lt;p&gt;Note that this file will not be encrypted, and if anyone gains access to it, they will be able to impersonate you, so take good care of it.&lt;/p&gt;</source>
        <translation>&lt;p&gt;Si vous perdez votre certificat, ce qui peut se produire si votre ordinateur est victime d&apos;une panne matérielle ou si vous réinstallez votre machine, vous ne pourrez plus vous authentifier sur aucun des serveurs sur lesquels vous vous étiez enregistré. Il est donc &lt;b&gt;obligatoire&lt;/b&gt; de faire une sauvegarde de votre certificat. Nous recommandons fortement de le sauvegarder sur un stockage amovible, tel une clé USB.&lt;/p&gt;
&lt;p&gt;Notez que ce fichier ne sera pas chiffré, et que toute personne qui mettra la main dessus pourra se faire passer pour vous, prenez donc ceci en considération.&lt;/p&gt;</translation>
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
        <translation>Aucun</translation>
    </message>
    <message>
        <source>Write</source>
        <translation type="obsolete">Écrire</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Traverse</source>
        <translation>Traverser</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Enter</source>
        <translation>Entrer</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Speak</source>
        <translation>Parler</translation>
    </message>
    <message>
        <source>AltSpeak</source>
        <translation type="obsolete">Parler Alt</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Mute/Deafen</source>
        <translation>Rend muet(te)/sourd(e)</translation>
    </message>
    <message>
        <source>Move/Kick</source>
        <translation type="obsolete">Déplacer/Éjecter</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Make channel</source>
        <translation>Créer un salon</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Make temporary</source>
        <translation>Créer des temporaires</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Link channel</source>
        <translation>Lier des salons</translation>
    </message>
    <message>
        <location line="-87"/>
        <source>This represents no privileges.</source>
        <translation>Ne représente aucun privilège.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>This represents total access to the channel, including the ability to change group and ACL information. This privilege implies all other privileges.</source>
        <translation>Représente l&apos;accès total au salon, incluant la capacité de changer les informations des groupes et des LCA. Ce privilège inclut tous les autres privilèges.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>This represents the permission to traverse the channel. If a user is denied this privilege, he will be unable to access this channel and any sub-channels in any way, regardless of other permissions in the sub-channels.</source>
        <translation>Représente la permission de traverser un salon. Si ce privilège est interdit à un utilisateur, il va être incapable d&apos;accéder à ce salon ainsi qu&apos;à ses sous-salons, peu importe les autres permissions des sous-salons.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>This represents the permission to join the channel. If you have a hierarchical channel structure, you might want to give everyone Traverse, but restrict Enter in the root of your hierarchy.</source>
        <translation>Représente la permission de joindre un salon. Si vous avez une structure de salons hiérarchiques, vous voudriez peut être donner à tout le monde le droit de Traverser, mais restreindre le droit d&apos;Entrer dans le salon racine de votre hiérarchie.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>This represents the permission to speak in a channel. Users without this privilege will be suppressed by the server (seen as muted), and will be unable to speak until they are unmuted by someone with the appropriate privileges.</source>
        <translation>Représente la permission de parler dans un salon. Un utilisateur sans ce droit va être supprimé par le serveur (vu comme muet) et ne sera pas capable de parler à moins que quelqu&apos;un ayant les droits ne lui rende la parole.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>This represents the permission to whisper to this channel from the outside. This works exactly like the &lt;i&gt;speak&lt;/i&gt; privilege, but applies to packets spoken with the Whisper key held down. This may be used to broadcast to a hierarchy of channels without linking.</source>
        <translation>Représente la permission de chuchoter vers ce salon depuis l&apos;extérieur. Fonctionne exactement comme le droit de &lt;i&gt;parler&lt;/i&gt;, mais s&apos;applique aux paquets envoyés avec la touche Chuchoter enfoncée. Peut être utilisé pour envoyer un message à plusieurs salons sans avoir besoin de les lier.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>This represents the permission to mute and deafen other users. Once muted, a user will stay muted until he is unmuted by another privileged user or reconnects to the server.</source>
        <translation>Représente la permission de rendre muet et sourd d&apos;autres utilisateurs. Une fois muet, un utilisateur le reste jusqu&apos;à ce qu&apos;un autre utilisateur ayant ce privilège lui rende la voix, ou qu&apos;il se reconnecte au serveur.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>This represents the permission to move a user to another channel or kick him from the server. To actually move the user, either the moving user must have Move privileges in the destination channel, or the user must normally be allowed to enter the channel. Users with this privilege can move users into channels the target user normally wouldn&apos;t have permission to enter.</source>
        <translation>Représente la permission de déplacer un utilisateur vers un autre salon ou de l&apos;éjecter du serveur. Pour déplacer un utilisateur, celui ci doit avoir le droit d&apos;entrer dans le salon, ou l&apos;utilisateur le déplaçant doit avoir le droit de déplacement/éjection sur le salon ciblé. Les utilisateurs ayant ce privilège peuvent déplacer des utilisateurs dans des salons dans lesquels l&apos;utilisateur ciblé n&apos;a normalement pas le droit d&apos;entrer.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>This represents the permission to make sub-channels. The user making the sub-channel will be added to the admin group of the sub-channel.</source>
        <translation>Représente la permission de créer des sous-salons. L&apos;utilisateur créant le sous-salon sera ajouté au groupe administrateur de ce sous-salon.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>This represents the permission to make a temporary subchannel. The user making the sub-channel will be added to the admin group of the sub-channel. Temporary channels are not stored and disappear when the last user leaves.</source>
        <translation>Représente la permission de créer un sous salon temporaire. L&apos;utilisateur créant le sous salon sera ajouté au groupe admin du sous salon. Les salons temporaires ne sont pas mémorisés et disparaissent dès que le dernier utilisateur le quitte.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>This represents the permission to link channels. Users in linked channels hear each other, as long as the speaking user has the &lt;i&gt;speak&lt;/i&gt; privilege in the channel of the listener. You need the link privilege in both channels to create a link, but just in either channel to remove it.</source>
        <translation>Représente la permission de lier des salons. Les utilisateurs présents dans des salons liés s&apos;entendent mutuellement, aussi longtemps que l&apos;utilisateur a le privilège de parler dans le salon de celui qui écoute. Vous avez besoin du privilège de lien dans les deux salons pour créer un lien, mais seulement d&apos;un des deux pour le supprimer.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>This represents the permission to write text messages to other users in this channel.</source>
        <translation>Représente la permission d&apos;écrire des messages textuels à l&apos;autres utilisateurs dans ce salon.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>This represents the permission to forcibly remove users from the server.</source>
        <translation>Représente la permission d&apos;expulser un utilisateur du serveur.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>This represents the permission to permanently remove users from the server.</source>
        <translation>Représente la permission de bannir un utilisateur du serveur.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>This represents the permission to register and unregister users on the server.</source>
        <translation>Représente la permission d&apos;enregistrer ou désinscrire les utilisateurs du serveur.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>This represents the permission to register oneself on the server.</source>
        <translation>Représente la permission de s&apos;enregistrer soi même sur le serveur.</translation>
    </message>
    <message>
        <location line="+31"/>
        <source>Whisper</source>
        <translation>Chuchoter</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Move</source>
        <translation>Déplacer</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Text message</source>
        <translation>Message texte</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Kick</source>
        <translation>Expulser</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Ban</source>
        <translation>Bannir</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Register User</source>
        <translation>Enregistrer l&apos;utilisateur</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Register Self</source>
        <translation>S&apos;enregistrer</translation>
    </message>
    <message>
        <source>This represents the permission to mute and deafen other players. Once muted, a player will stay muted until he is unmuted by another privileged player or reconnects to the server.</source>
        <translation type="obsolete">Représente la permission de rendre muet et sourd d&apos;autres joueurs. Une fois muet, un joueur le reste jusqu&apos;à ce qu&apos;un autre joueur ayant ce privilège lui rende la voix, ou qu&apos;il se reconnecte au serveur.</translation>
    </message>
    <message>
        <source>This represents the permission to move a player to another channel or kick him from the server. To actually move the player, either the moving player must have Move/Kick privileges in the destination channel, or the player must normally be allowed to enter the channel. Players with this privilege can move players into channels the target player normally wouldn&apos;t have permission to enter.</source>
        <translation type="obsolete">Représente la permission de déplacer un joueur vers un autre salon ou de l&apos;éjecter du serveur. Pour déplacer un joueur, celui ci doit avoir le droit d&apos;entrer dans le salon, ou le joueur le déplaçant doit avoir le droit de déplacement/éjection sur le salon ciblé. Les joueurs ayant ce privilège peuvent déplacer des joueurs dans des salons dans lesquels le joueur ciblé n&apos;a normalement pas le droit d&apos;entrer.</translation>
    </message>
    <message>
        <source>This represents the permission to make sub-channels. The player making the sub-channel will be added to the admin group of the sub-channel.</source>
        <translation type="obsolete">Représente la permission de créer des sous-salons. Le joueur créant les sous-salons sera ajouté au groupe administrateur de ce sous-salon.</translation>
    </message>
    <message>
        <source>This represents the permission to link channels. Players in linked channels hear each other, as long as the speaking player has the &lt;i&gt;speak&lt;/i&gt; privilege in the channel of the listener. You need the link privilege in both channels to create a link, but just in either channel to remove it.</source>
        <translation type="obsolete">Représente la permission de lier des salons. Les joueurs présents dans des salons liés s&apos;entendent mutuellement, aussi longtemps que le joueur a le privilège de parler dans le salon de celui qui écoute. Vous avez besoin du privilège de lien dans les deux salons pour créer un lien, mais seulement d&apos;un des deux pour le supprimer.</translation>
    </message>
    <message>
        <location line="-28"/>
        <source>Write ACL</source>
        <translation>Écrire LCA</translation>
    </message>
</context>
<context>
    <name>ChatbarLineEdit</name>
    <message>
        <source>Paste and send</source>
        <translation type="obsolete">Coller et envoyer</translation>
    </message>
    <message>
        <source>&lt;center&gt;Type chat message here&lt;/center&gt;</source>
        <translation type="obsolete">&lt;center&gt;Saisir un message ici&lt;/center&gt;</translation>
    </message>
    <message>
        <source>Type chat message here</source>
        <translation type="obsolete">Saisir un message ici</translation>
    </message>
</context>
<context>
    <name>ChatbarTextEdit</name>
    <message>
        <location filename="CustomElements.cpp" line="+79"/>
        <source>Paste and send</source>
        <translation>Coller et envoyer</translation>
    </message>
    <message>
        <location line="+25"/>
        <source>&lt;center&gt;Type chat message here&lt;/center&gt;</source>
        <translation>&lt;center&gt;Saisir un message ici&lt;/center&gt;</translation>
    </message>
</context>
<context>
    <name>ClientPlayer</name>
    <message>
        <source>Authenticated</source>
        <translation type="obsolete">Authentifié</translation>
    </message>
    <message>
        <source>Muted (server)</source>
        <translation type="obsolete">Muet (serveur)</translation>
    </message>
    <message>
        <source>Deafened (server)</source>
        <translation type="obsolete">Sourd (server)</translation>
    </message>
    <message>
        <source>Local Mute</source>
        <translation type="obsolete">Muet (local)</translation>
    </message>
    <message>
        <source>Muted (self)</source>
        <translation type="obsolete">Devenir muet(te)</translation>
    </message>
    <message>
        <source>Deafened (self)</source>
        <translation type="obsolete">Devenir sourd</translation>
    </message>
</context>
<context>
    <name>ClientUser</name>
    <message>
        <location filename="ClientUser.cpp" line="+130"/>
        <source>Friend</source>
        <translation>Ami</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Authenticated</source>
        <translation>Authentifié(e)</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Priority speaker</source>
        <translation>Parole prioritaire</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Recording</source>
        <translation>Enregistrement</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Muted (server)</source>
        <translation>Muet(te) (serveur)</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Deafened (server)</source>
        <translation>Sourd(e) (serveur)</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Local Mute</source>
        <translation>Muet(te) (local)</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Muted (self)</source>
        <translation>Devenir muet(te)</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Deafened (self)</source>
        <translation>Devenir sourd(e)</translation>
    </message>
</context>
<context>
    <name>ConfigDialog</name>
    <message>
        <location filename="ConfigDialog.ui"/>
        <source>Mumble Configuration</source>
        <translation>Configuration de Mumble</translation>
    </message>
    <message>
        <location/>
        <location filename="ConfigDialogDelegate.mm" line="+142"/>
        <source>Advanced</source>
        <translation>Avancé</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Valider</translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="+54"/>
        <source>Accept changes</source>
        <translation>Accepter les modifications</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This button will accept current settings and return to the application.&lt;br /&gt;The settings will be stored to disk when you leave the application.</source>
        <translation>Ce bouton acceptera les préférences actuelles et retournera à l&apos;application.&lt;br /&gt;Les préférences seront sauvegardées sur votre disque dur quand vous quitterez l&apos;application.</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">A&amp;nnuler</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Reject changes</source>
        <translation>Annuler les modifications</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This button will reject all changes and return to the application.&lt;br /&gt;The settings will be reset to the previous positions.</source>
        <translation>Ce bouton annulera toutes les modifications et retournera à l&apos;application.&lt;br /&gt;Les paramètres seront restaurer à leurs états précédents.</translation>
    </message>
    <message>
        <source>&amp;Apply</source>
        <translation type="obsolete">&amp;Appliquer</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Apply changes</source>
        <translation>Appliquer les modifications</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This button will immediately apply all changes.</source>
        <translation>Ce bouton appliquera immédiatement toutes les modifications.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Undo changes for current page</source>
        <translation>Annuler les modifications apportées à la page courante</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This button will revert any changes done on the current page to the most recent applied settings.</source>
        <translation>Ce bouton revient sur tous les changements effectués sur la page en cours depuis la dernière sauvegarde de paramètres.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Restore defaults for current page</source>
        <translation>Restaurer par défaut pour la page courante</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This button will restore the defaults for the settings on the current page. Other pages will not be changed.&lt;br /&gt;To restore all settings to their defaults, you will have to use this button on every page.</source>
        <oldsource>This button will restore the settings for the current page only to their defaults. Other pages will not be changed.&lt;br /&gt;To restore all settings to their defaults, you will have to use this button on every page.</oldsource>
        <translation>Ce boutton restaure les paramètres par défaut de la page courante seulement. Les autres pages ne sont pas modifiées.&lt;br/&gt;Pour restaurer tous les paramètres par défaut, vous devez utiliser ce boutton sur toutes les pages.</translation>
    </message>
    <message>
        <source>This button will restore the settings for the current page only to their defaults. Other pages will be not be changed.&lt;br /&gt;To restore all settings to their defaults, you will have to use this button on every page.</source>
        <translation type="obsolete">Ce bouton restaurera les paramètres pour la page courante par leur valeur par défaut. Les autres pages ne seront pas changés.&lt;br/&gt;Pour restaurer tout les paramètres par défaut, vous devai utiliser ce bouton sur chacune des pages.</translation>
    </message>
    <message>
        <source>Show all configuration items</source>
        <translation type="obsolete">Afficher tout les outils de configuration</translation>
    </message>
    <message>
        <source>Expert Config</source>
        <translation type="obsolete">Configuration avancée</translation>
    </message>
    <message>
        <source>Tab 1</source>
        <translation type="obsolete">Onglet 1</translation>
    </message>
</context>
<context>
    <name>ConfigDialogMac</name>
    <message>
        <location filename="ConfigDialog_macx.mm" line="+70"/>
        <source>Accept changes</source>
        <translation>Accepter les modifications</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This button will accept current settings and return to the application.&lt;br /&gt;The settings will be stored to disk when you leave the application.</source>
        <translation>Ce bouton acceptera les préférences actuelles et retournera à l&apos;application.&lt;br /&gt;Les préférences seront sauvegardées sur votre disque dur quand vous quitterez l&apos;application.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Reject changes</source>
        <translation>Annuler les modifications</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This button will reject all changes and return to the application.&lt;br /&gt;The settings will be reset to the previous positions.</source>
        <translation>Ce bouton annulera toutes les modifications et retournera à l&apos;application.&lt;br /&gt;Les paramètres seront restaurer à leurs états précédents.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Apply changes</source>
        <translation>Appliquer les modifications</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This button will immediately apply all changes.</source>
        <translation>Ce bouton appliquera immédiatement toutes les modifications.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Undo changes for current page</source>
        <translation>Annuler les modifications apportées à la page courante</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This button will revert any changes done on the current page to the most recent applied settings.</source>
        <translation>Ce bouton revient sur tous les changements effectués sur la page en cours depuis la dernière sauvegarde de paramètres.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Restore defaults for current page</source>
        <translation>Restaurer par défaut pour la page courante</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This button will restore the defaults for the settings on the current page. Other pages will not be changed.&lt;br /&gt;To restore all settings to their defaults, you will have to use this button on every page.</source>
        <oldsource>This button will restore the settings for the current page only to their defaults. Other pages will not be changed.&lt;br /&gt;To restore all settings to their defaults, you will have to use this button on every page.</oldsource>
        <translation>Ce boutton restaure les paramètres par défaut de la page courante seulement. Les autres pages ne sont pas modifiées.&lt;br/&gt;Pour restaurer tous les paramètres par défaut, vous devez utiliser ce boutton sur toutes les pages.</translation>
    </message>
</context>
<context>
    <name>ConnectDialog</name>
    <message>
        <source>&amp;Name</source>
        <translation type="obsolete">&amp;Nom</translation>
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
        <translation type="obsolete">&amp;Nom d&apos;utilisateur</translation>
    </message>
    <message>
        <source>&amp;Password</source>
        <translation type="obsolete">Mot de &amp;passe</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="+757"/>
        <source>&amp;Connect</source>
        <translation>&amp;Connexion</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Annuler</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation type="obsolete">&amp;Ajouter</translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation type="obsolete">&amp;Retirer</translation>
    </message>
    <message>
        <source>&amp;Custom Servers</source>
        <translation type="obsolete">&amp;Serveurs personnalisés</translation>
    </message>
    <message>
        <source>Server &amp;Browser</source>
        <translation type="obsolete">Navi&amp;gateur de serveurs</translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="obsolete">Nom</translation>
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
        <translation type="obsolete">C&amp;opier vers un serveur personnalisé</translation>
    </message>
    <message>
        <source>&amp;View Webpage</source>
        <translation type="obsolete">&amp;Voir la page web</translation>
    </message>
    <message>
        <location line="+133"/>
        <source>Connecting to %1</source>
        <translation>Connexion à %1</translation>
    </message>
    <message>
        <location line="-491"/>
        <location line="+491"/>
        <source>Enter username</source>
        <translation>Entrer le nom d&apos;utilisateur</translation>
    </message>
    <message>
        <location line="-491"/>
        <source>Adding host %1</source>
        <translation>Ajout de l&apos;hôte %1</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui"/>
        <location filename="ConnectDialog.cpp" line="+61"/>
        <source>Servername</source>
        <translation>Nom du serveur</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="+1"/>
        <source>Hostname</source>
        <translation>Nom d&apos;hôte</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Bonjour name</source>
        <translation>Nom Bonjour</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Port</source>
        <translation>Port</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Addresses</source>
        <translation>Adresses</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Website</source>
        <translation>Site internet</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Packet loss</source>
        <translation>Paquets perdus</translation>
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
        <translation>Bande passante</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>%1 kbit/s</source>
        <translation>%1 kbits/s</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui"/>
        <location filename="ConnectDialog.cpp" line="+1"/>
        <source>Users</source>
        <translation>Utilisateurs</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="+1"/>
        <source>Version</source>
        <translation>Version</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui"/>
        <location filename="ConnectDialog.cpp" line="+276"/>
        <source>&amp;Add New...</source>
        <oldsource>Add New...</oldsource>
        <translation>&amp;Ajouter un nouveau...</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="+37"/>
        <source>&amp;Filters</source>
        <oldsource>Filters</oldsource>
        <translation>&amp;Filtres</translation>
    </message>
    <message>
        <source>Mumble</source>
        <translation type="obsolete">Mumble</translation>
    </message>
    <message>
        <location line="+692"/>
        <source>Failed to fetch server list</source>
        <translation>Impossible de récupérer la liste des serveurs</translation>
    </message>
    <message>
        <source>&amp;Label</source>
        <translation type="obsolete">&amp;Label</translation>
    </message>
    <message>
        <source>Label</source>
        <translation type="obsolete">Label</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation type="obsolete">Inconnu</translation>
    </message>
    <message>
        <source>-Unnamed entry-</source>
        <translation type="obsolete">-Sans nom-</translation>
    </message>
    <message>
        <location filename="ConnectDialog.ui"/>
        <source>Mumble Server Connect</source>
        <translation>Connexion au serveur Mumble</translation>
    </message>
    <message>
        <location/>
        <source>Ping</source>
        <translation>Ping</translation>
    </message>
    <message>
        <location/>
        <source>Remove from Favorites</source>
        <translation>Supprimer des favoris</translation>
    </message>
    <message>
        <source>Edit...</source>
        <translation type="obsolete">Éditer...</translation>
    </message>
    <message>
        <location/>
        <source>Add custom server</source>
        <translation>Ajouter un serveur personnalisé</translation>
    </message>
    <message>
        <source>Add to Favorites</source>
        <translation type="obsolete">Ajouter aux favoris</translation>
    </message>
    <message>
        <source>Open Webpage</source>
        <translation type="obsolete">Ouvrir la page Web</translation>
    </message>
    <message>
        <source>Show Reachable</source>
        <translation type="obsolete">Afficher les joignables</translation>
    </message>
    <message>
        <location/>
        <source>Show all servers that respond to ping</source>
        <translation>Afficher tous les serveurs répondant au ping</translation>
    </message>
    <message>
        <source>Show Populated</source>
        <translation type="obsolete">Afficher les peuplés</translation>
    </message>
    <message>
        <location/>
        <source>Show all servers with users</source>
        <translation>Afficher tous les serveurs avec des utilisateurs</translation>
    </message>
    <message>
        <source>Show All</source>
        <translation type="obsolete">Afficher tous</translation>
    </message>
    <message>
        <location/>
        <source>Show all servers</source>
        <translation>Afficher tous les serveurs</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Copy</source>
        <translation>&amp;Copier</translation>
    </message>
    <message>
        <location/>
        <source>Copy favorite link to clipboard</source>
        <translation>Copier le lien favoris dans le presse papier</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Paste</source>
        <translation>C&amp;oller</translation>
    </message>
    <message>
        <location/>
        <source>Paste favorite from clipboard</source>
        <translation>Coller le favoris du presse papier</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">A&amp;nnuler</translation>
    </message>
    <message>
        <source>New</source>
        <translation type="obsolete">Nouveau</translation>
    </message>
    <message>
        <source>Add</source>
        <translation type="obsolete">Ajouter</translation>
    </message>
    <message>
        <source>Update</source>
        <translation type="obsolete">Mettre à jour</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Edit...</source>
        <translation>&amp;Editer...</translation>
    </message>
    <message>
        <location/>
        <source>Add to &amp;Favorites</source>
        <translation>Ajouter aux &amp;Favoris</translation>
    </message>
    <message>
        <location/>
        <source>Open &amp;Webpage</source>
        <translation>Ouvrir la page &amp;web</translation>
    </message>
    <message>
        <location/>
        <source>Show &amp;Reachable</source>
        <translation>Affiche&amp;r les joignables</translation>
    </message>
    <message>
        <location/>
        <source>Show &amp;Populated</source>
        <translation>Afficher les &amp;peuplés</translation>
    </message>
    <message>
        <location/>
        <source>Show &amp;All</source>
        <translation>&amp;Afficher tous</translation>
    </message>
</context>
<context>
    <name>ConnectDialogEdit</name>
    <message>
        <location filename="ConnectDialogEdit.ui"/>
        <source>Edit Server</source>
        <translation>Éditer le serveur</translation>
    </message>
    <message>
        <location/>
        <source>Name of the server</source>
        <translation>Nom du serveur</translation>
    </message>
    <message>
        <source>&lt;b&gt;Name&lt;/b&gt;&lt;br/&gt;
Name of the server. This is what the server will be named like in your serverlist and can be chosen freely.</source>
        <translation type="obsolete">&lt;b&gt;Nom&lt;/b&gt;&lt;br/&gt;
Nom du serveur. C&apos;est le nom du serveur tel qu&apos;il apparaîtra dans vos favoris et qui vous pemettra de le choisir plus facilement.</translation>
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
        <translation>Adresse internet du serveur.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Address&lt;/b&gt;&lt;br/&gt;
Internet address of the server. This can be a normal hostname, an IPv4/IPv6 address or a Bonjour service identifier. Bonjour service identifiers have to be prefixed with a &apos;@&apos; to be recognized by Mumble.</source>
        <oldsource>&lt;b&gt;Address&lt;/b&gt;&lt;/br&gt;
Internet address of the server. This can be a normal hostname, an ipv4/6 address or a bonjour service identifier. Bonjour service identifiers have to be prefixed with a &apos;@&apos; to be recognized by Mumble.</oldsource>
        <translation>&lt;b&gt;Adresse&lt;/b&gt;&lt;br/&gt;
Adresse internet du serveur. Ça peut être une nom d&apos;hôte, une adresse IPv4/IPv6 ou un identifiant du service Bonjour. Les identifiants du service Bonjour doivent être préfixés avec un &apos;@&apos; pour être reconnus par Mumble.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Port</source>
        <translation>&amp;Port</translation>
    </message>
    <message>
        <location/>
        <source>Port on which the server is listening</source>
        <translation>Port d&apos;écoute du serveur</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Port&lt;/b&gt;&lt;br/&gt;
Port on which the server is listening. If the server is identified by a Bonjour service identifier this field will be ignored.</source>
        <translation>&lt;b&gt;Port&lt;/b&gt;&lt;br/&gt;
Port d&apos;écoute du seveur. Si le serveur est identifié par le service Bonjour, ce champ est ignoré.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Username</source>
        <translation>&amp;Nom d&apos;utilisateur</translation>
    </message>
    <message>
        <location/>
        <source>Username to send to the server</source>
        <translation>Nom d&apos;utilisateur envoyé au serveur</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Username&lt;/b&gt;&lt;br/&gt;
Username to send to the server. Be aware that the server can impose restrictions on how a username might look like. Also your username could already be taken by another user.</source>
        <translation>&lt;b&gt;Nom d&apos;utilisateur&lt;/b&gt;&lt;br/&gt;
Nom d&apos;utilisateur envoyé au serveur. Notez que le serveur peut imposer des restrictions sur l&apos;utilisation des noms. De plus, votre nom peut déjà avoir été choisi par un autre utilisateur.</translation>
    </message>
    <message>
        <location/>
        <source>Label</source>
        <translation>Nom</translation>
    </message>
    <message>
        <location filename="ConnectDialog.cpp" line="-804"/>
        <source>Add Server</source>
        <translation>Ajouter le serveur</translation>
    </message>
    <message>
        <location filename="ConnectDialogEdit.ui"/>
        <source>Password</source>
        <translation>Mot de passe</translation>
    </message>
    <message>
        <location/>
        <source>Password to send to the server</source>
        <translation>Mot de passe envoyé au serveur</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Password&lt;/b&gt;&lt;br/&gt;
Password to be sent to the server on connect. This password is needed when connecting as &lt;i&gt;SuperUser&lt;/i&gt; or to a server using password authentication. If not entered here the password will be queried on connect.</source>
        <translation>&lt;b&gt;Mot de passe&lt;/b&gt;&lt;br /&gt;
Mot de passe envoyé au serveur lors de la connexion. Ce mot de passe est nécessaire lors de la connexion en tant que &lt;i&gt;SuperUser&lt;/i&gt; ou à un serveur utilisant l&apos;authentification par mot de passe. S&apos;il n&apos;est pas renseigné ici, le mot de passe sera demandé lors de la connexion.</translation>
    </message>
    <message>
        <location/>
        <source>Show password</source>
        <translation>Voir le mot de passe</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Label&lt;/b&gt;&lt;br/&gt;
Label of the server. This is what the server will be named like in your server list and can be chosen freely.</source>
        <translation>&lt;b&gt;Nom&lt;/b&gt;&lt;br/&gt;
Nom du serveur. C&apos;est le nom du serveur tel qu&apos;il apparaîtra dans vos favoris et qui vous pemettra de le choisir plus facilement.</translation>
    </message>
</context>
<context>
    <name>CoreAudioSystem</name>
    <message>
        <location filename="CoreAudio.cpp" line="+83"/>
        <source>Default Device</source>
        <translation>Périphérique par défaut</translation>
    </message>
</context>
<context>
    <name>CrashReporter</name>
    <message>
        <location filename="CrashReporter.cpp" line="+37"/>
        <source>Mumble Crash Report</source>
        <translation>Rapport d&apos;erreur de Mumble</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>&lt;p&gt;&lt;b&gt;We&apos;re terribly sorry, but it seems Mumble has crashed. Do you want to send a crash report to the Mumble developers?&lt;/b&gt;&lt;/p&gt;&lt;p&gt;The crash report contains a partial copy of Mumble&apos;s memory at the time it crashed, and will help the developers fix the problem.&lt;/p&gt;</source>
        <translation>&lt;p&gt;&lt;b&gt;Nous sommes vraiment navrés, mais il semblerai que Mumble a cessé de fonctionner. Voulez-vous envoyer un rapport de l&apos;incident aux développeurs de Mumble ?&lt;/b&gt;&lt;/p&gt;&lt;p&gt;Ce rapport contient une copie partielle de la mémoire de Mumble au moment de son arrêt, et permettra aux développeurs de résoudre le problème.&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Email address (optional)</source>
        <translation>Adresse de courriel (optionnel)</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Please describe briefly, in English, what you were doing at the time of the crash</source>
        <oldsource>Please briefly describe what you were doing at the time of the crash</oldsource>
        <translation>Veuillez décrire brièvement et en anglais ce que vous faisiez au moment du plantage</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Send Report</source>
        <translation>Envoyer le rapport</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Don&apos;t send report</source>
        <translation>Ne pas envoyer de rapport</translation>
    </message>
    <message>
        <location line="+24"/>
        <source>Crash upload successful</source>
        <translation>Envoi du rapport effectué</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Thank you for helping make Mumble better!</source>
        <translation>Merci d&apos;aider Mumble à devenir meilleur !</translation>
    </message>
    <message>
        <location line="+2"/>
        <location line="+2"/>
        <source>Crash upload failed</source>
        <translation>Erreur lors de l&apos;envoi du rapport</translation>
    </message>
    <message>
        <location line="-2"/>
        <source>We&apos;re really sorry, but it appears the crash upload has failed with error %1 %2. Please inform a developer.</source>
        <translation>Nous sommes navrés, mais l&apos;envoi du rapport d&apos;erreur a échoué avec l&apos;erreur %1 %2. Veuillez en informer un développeur.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>This really isn&apos;t funny, but apparently there&apos;s a bug in the crash reporting code, and we&apos;ve failed to upload the report. You may inform a developer about error %1</source>
        <translation>Ce n&apos;est vraiment pas drôle, mais il semblerait qu&apos;il y ait un bogue dans le code du rapport d&apos;erreur, et donc, le rapport n&apos;a pu être envoyé. Vous devriez informer un développeur à propos de l&apos;erreur %1</translation>
    </message>
    <message>
        <location line="+114"/>
        <source>Uploading crash report</source>
        <translation>Envoi du rapport d&apos;erreur</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Abort upload</source>
        <translation>Annuler l&apos;envoi</translation>
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
        <translation type="obsolete">Ouverture de l&apos;entrée son choisie impossible. Aucune capture son depuis le microphone sera faite.</translation>
    </message>
    <message>
        <location filename="DirectSound.cpp" line="+554"/>
        <source>Opening chosen DirectSound Input failed. Default device will be used.</source>
        <translation>Ouverture de l&apos;entrée son choisie impossible. Le périphérique par défaut sera utilisé.</translation>
    </message>
    <message>
        <location line="-382"/>
        <source>Default DirectSound Voice Input</source>
        <translation>Entrée Voix par défaut</translation>
    </message>
    <message>
        <location line="+461"/>
        <source>Opening chosen DirectSound Input device failed. No microphone capture will be done.</source>
        <translation>Ouverture de l&apos;entrée son choisie impossible. Aucune capture son depuis le microphone sera faite.</translation>
    </message>
    <message>
        <location line="-5"/>
        <source>Lost DirectSound input device.</source>
        <translation>Périphérique d&apos;entrée son perdu.</translation>
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
        <translation>Ouverture de la sortie son choisie impossible. Vous n&apos;entendrez aucun son.</translation>
    </message>
    <message>
        <location line="-82"/>
        <source>Opening chosen DirectSound Output failed. Default device will be used.</source>
        <translation>Ouverture de la sortie son choisie impossible. Le périphérique par défaut sera utilisé.</translation>
    </message>
    <message>
        <location line="-247"/>
        <source>Default DirectSound Voice Output</source>
        <translation>Sortie Voix par défaut</translation>
    </message>
    <message>
        <location line="+325"/>
        <source>Lost DirectSound output device.</source>
        <translation>Périphérique de sortie perdu.</translation>
    </message>
</context>
<context>
    <name>DXConfigDialog</name>
    <message>
        <source>Default DirectSound Voice Input</source>
        <translation type="obsolete">Entrée Voix par défaut</translation>
    </message>
    <message>
        <source>Default DirectSound Voice Output</source>
        <translation type="obsolete">Sortie Voix par défaut</translation>
    </message>
    <message>
        <source>Device selection</source>
        <translation type="obsolete">Sélection périphérique</translation>
    </message>
    <message>
        <source>Device to use for microphone</source>
        <translation type="obsolete">Périphérique utilisé comme microphone</translation>
    </message>
    <message>
        <source>This sets the input device to use, which is where you have connected the microphone.</source>
        <translation type="obsolete">Sélectionne le périphérique d&apos;entrée à utiliser, qui est l&apos;endroit où vous avez connecter le microphone.</translation>
    </message>
    <message>
        <source>Input</source>
        <translation type="obsolete">Entrée</translation>
    </message>
    <message>
        <source>Device to use for speakers/headphones</source>
        <translation type="obsolete">Périphérique à utiliser pour les hauts-parleurs</translation>
    </message>
    <message>
        <source>This sets the output device to use, which is where you have connected your speakers or your headset.</source>
        <translation type="obsolete">Sélectionne le périphérique de sortie à utiliser, qui est l&apos;endroit où vous avez connecter les hauts-parleurs.</translation>
    </message>
    <message>
        <source>Output</source>
        <translation type="obsolete">Sortie</translation>
    </message>
    <message>
        <source>Output Options</source>
        <translation type="obsolete">Options de Sortie</translation>
    </message>
    <message>
        <source>Output Delay</source>
        <translation type="obsolete">Délais de sortie</translation>
    </message>
    <message>
        <source>Amount of data to buffer for DirectSound</source>
        <translation type="obsolete">Quantité de données à mettre en tampon pour DirectSound</translation>
    </message>
    <message>
        <source>This sets the amount of data to prebuffer in the DirectSound buffer. Experiment with different values and set it to the lowest which doesn&apos;t cause rapid jitter in the sound.</source>
        <translation type="obsolete">Spécifie la quantité de données à mettre en cache dans le tampon de sortie. Essayez différentes valeurs et mettez la plus bassequi ne cause pas de rapides décalages du son.</translation>
    </message>
    <message>
        <source>Positional Audio</source>
        <translation type="obsolete">Position Audio</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Aucun</translation>
    </message>
    <message>
        <source>Panning</source>
        <translation type="obsolete">Panoramique</translation>
    </message>
    <message>
        <source>Light HRTF</source>
        <translation type="obsolete">HRTF léger</translation>
    </message>
    <message>
        <source>Full HRTF</source>
        <translation type="obsolete">HRTF complet</translation>
    </message>
    <message>
        <source>3D Sound Algorithm</source>
        <translation type="obsolete">Algorithme de son 3D</translation>
    </message>
    <message>
        <source>This sets what 3D Sound algorithm to use.&lt;br /&gt;&lt;b&gt;None&lt;/b&gt; - Disable 3D Sound (least CPU).&lt;br /&gt;&lt;b&gt;Panning&lt;/b&gt; - Just use stereo panning (some CPU).&lt;br /&gt;&lt;b&gt;Light/Full HRTF&lt;/b&gt; - Head-Related Transfer Functions enabled. This may use a small amount of CPU.&lt;br /&gt;Note that if you have a soundcard with &lt;i&gt;hardware&lt;/i&gt; 3D processing, HRTF processing will be done on the soundcard and will use practically no processing power.</source>
        <translation type="obsolete">Définis quel algorithme de son 3D utiliser.&lt;br /&gt;&lt;b&gt;Aucun&lt;/b&gt; - Désactive le son 3D ( le moins de CPU ).&lt;br /&gt;&lt;b&gt;Pannoramique&lt;/b&gt; - N&apos;utilise que le stéréo panoramique ( un peu de CPU ).&lt;br /&gt;&lt;b&gt;HRTF Léger/Plein&lt;/b&gt; - Head-Related Transfer Functions activées. Il est possible que ceci utilise un peu de CPU.&lt;br /&gt;Veuillez noter que si vous avez une carte avec ???? &lt;i&gt;matériel&lt;/i&gt;, HRTF ???.</translation>
    </message>
    <message>
        <source>Method</source>
        <translation type="obsolete">Méthode</translation>
    </message>
    <message>
        <source>MinDistance</source>
        <translation type="obsolete">MinDistance</translation>
    </message>
    <message>
        <source>Minimum distance to player before sound decreases</source>
        <translation type="obsolete">Distance minimum d&apos;un joueur avant que le son ne diminue</translation>
    </message>
    <message>
        <source>This sets the minimum distance for sound calculations. The volume of other players&apos; speech will not decrease until they are at least this far away from you.</source>
        <translation type="obsolete">Définis la distance minimum pour calculer le son. Le volume de la voix des autres joueurs ne diminuera pas avant d&apos;être au moins aussi loin de vous.</translation>
    </message>
    <message>
        <source>MaxDistance</source>
        <translation type="obsolete">MaxDistance</translation>
    </message>
    <message>
        <source>Maximum distance, beyond which sound won&apos;t decrease</source>
        <translation type="obsolete">Distance maximale, après laquelle le son ne diminuera plus</translation>
    </message>
    <message>
        <source>This sets the maximum distance for sound calculations. When farther away than this, other players&apos; sound volume will not decrease any more.</source>
        <translation type="obsolete">Spécifie la distance maximum pour le calcul du son. Passé cette distance, le volume des autres joueurs ne diminuera plus.</translation>
    </message>
    <message>
        <source>Factor for sound volume decrease</source>
        <translation type="obsolete">Facteur de diminution du volume sonore</translation>
    </message>
    <message>
        <source>How fast should sound volume drop when passing beyond the minimum distance. The normal (1.0) is that sound volume halves each time the distance doubles. Increasing this value means sound volume drops faster, while decreasing it means it drops slower.</source>
        <translation type="obsolete">À quelle vitesse le volume sonore devrait descendre en dépassant la distance minimale. Normalement ( 1.0 ), le son diminue de moitié à chaque fois que la distance double. Augmenter cette valeur signifie que le son descend plus vite, tandis que la diminuer signifie que le son diminue plus lentement.</translation>
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
        <translation type="obsolete">%1m</translation>
    </message>
    <message>
        <source>%1</source>
        <translation type="obsolete">%1</translation>
    </message>
    <message>
        <source>Players more than %1 meters away have %2% intensity</source>
        <translation type="obsolete">Les joueurs à plus de %1 mètres ont toujours une intensité de %2</translation>
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
        <translation>Mumble n&apos;a pas réussi à initialiser la base de données
à aucun des emplacements possibles.</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>The database &apos;%1&apos; is read-only. Mumble cannot store server settings (i.e. SSL certificates) until you fix this problem.</source>
        <oldsource>The database &apos;%1&apos; is read-only. Mumble can not store server settings (ie. SSL certificates) until you fix this problem.</oldsource>
        <translation>La base de données &apos;%1&apos; est en lecture seule. Mumble ne peut pas enregistrer les paramètres du serveur (ex. le certificat SSL) tant que vous n&apos;aurez pas résolu le problème.</translation>
    </message>
</context>
<context>
    <name>DirectInputKeyWidget</name>
    <message>
        <source>Press Shortcut</source>
        <translation type="obsolete">Pressez un raccourcis</translation>
    </message>
</context>
<context>
    <name>DockTitleBar</name>
    <message>
        <location filename="CustomElements.cpp" line="+135"/>
        <source>Drag here</source>
        <translation>Glisser ici</translation>
    </message>
</context>
<context>
    <name>GlobalShortcut</name>
    <message>
        <location filename="GlobalShortcut.ui"/>
        <source>Shortcuts</source>
        <translation>Raccourcis</translation>
    </message>
    <message>
        <location/>
        <source>List of configured shortcuts</source>
        <translation>Liste des raccourcis actuellement configurés</translation>
    </message>
    <message>
        <location/>
        <source>Function</source>
        <translation>Fonction</translation>
    </message>
    <message>
        <location/>
        <source>Data</source>
        <translation>Données</translation>
    </message>
    <message>
        <location/>
        <source>Shortcut</source>
        <translation>Raccourci</translation>
    </message>
    <message>
        <location/>
        <source>Suppress</source>
        <translation>Suspend</translation>
    </message>
    <message>
        <location/>
        <source>Add new shortcut</source>
        <translation>Ajouter un nouveau raccourci</translation>
    </message>
    <message>
        <location/>
        <source>This will add a new global shortcut</source>
        <translation>Ajoutera un nouveau raccourci global</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Add</source>
        <translation>&amp;Ajouter</translation>
    </message>
    <message>
        <location/>
        <source>Remove selected shortcut</source>
        <translation>Supprime le raccourci sélectionné</translation>
    </message>
    <message>
        <location/>
        <source>This will permanently remove a selected shortcut.</source>
        <translation>Supprime définitivement le raccourci sélectioné.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Remove</source>
        <translation>Supp&amp;rimer</translation>
    </message>
</context>
<context>
    <name>GlobalShortcutConfig</name>
    <message>
        <location filename="GlobalShortcut.cpp" line="+654"/>
        <source>Shortcuts</source>
        <translation>Raccourcis</translation>
    </message>
    <message>
        <source>Function</source>
        <translation type="obsolete">Fonction</translation>
    </message>
    <message>
        <source>Shortcut</source>
        <translation type="obsolete">Raccourci</translation>
    </message>
    <message>
        <source>Shortcut bound to %1.</source>
        <translation type="obsolete">Raccourcis lié à %1.</translation>
    </message>
    <message>
        <source>&lt;b&gt;This is the global shortcut bound to %1&lt;/b&gt;&lt;br /&gt;Click this field and then the desired key/button combo to rebind. Double-click to clear.</source>
        <translation type="obsolete">&lt;b&gt;Il s&apos;agit du raccourcis lié à %1&lt;/b&gt;&lt;br /&gt;Cliquez dans ce champ et tappez la combinaison de touche/bouton à relier. Double-cliquez pour effacer.</translation>
    </message>
    <message>
        <source>Double-click an entry to clear the shortcut.</source>
        <translation type="obsolete">Double-cliquez sur une entrée pour effacer le raccourci.</translation>
    </message>
    <message>
        <source>Suppress</source>
        <translation type="obsolete">Supprimer</translation>
    </message>
    <message>
        <location line="+38"/>
        <source>Shortcut button combination.</source>
        <translation>Combinaison de touche du raccourci.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>&lt;b&gt;This is the global shortcut key combination.&lt;/b&gt;&lt;br /&gt;Click this field and then press the desired key/button combo to rebind. Double-click to clear.</source>
        <oldsource>&lt;b&gt;This is the global shortcut key combination.&lt;/b&gt;&lt;br /&gt;Double-click this field and then the desired key/button combo to rebind.</oldsource>
        <translation>&lt;b&gt;Combinaison de touches du raccourci global.&lt;/b&gt;&lt;br/&gt;Effectuez un clic dans ce champ puis pressez la combinaison de touches/boutons désiré que vous souhaitez attribuer. Double-cliquez pour effacer.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Suppress keys from other applications</source>
        <translation>Supprimer les touches pour les autres applications</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>&lt;b&gt;This hides the button presses from other applications.&lt;/b&gt;&lt;br /&gt;Enabling this will hide the button (or the last button of a multi-button combo) from other applications. Note that not all buttons can be suppressed.</source>
        <translation>&lt;b&gt;Cache les touches pressées aux autres applications.&lt;/b&gt;&lt;br/&gt;Activer cette option cachera la touche (ou la dernière touche pour une combinaison) aux autres applications. Notez que toutes les touches ne peuvent pas être supprimées.</translation>
    </message>
</context>
<context>
    <name>GlobalShortcutMacInit</name>
    <message>
        <location filename="GlobalShortcut_macx.mm" line="+65"/>
        <source>Mumble has detected that it is unable to receive Global Shortcut events when it is in the background.&lt;br /&gt;&lt;br /&gt;This is because the Universal Access feature called &apos;Enable access for assistive devices&apos; is currently disabled.&lt;br /&gt;&lt;br /&gt;Please &lt;a href=&quot; &quot;&gt;enable this setting&lt;/a&gt; and continue when done.</source>
        <translation>Mumble a détecté qu&apos;il n&apos;est pas capable de recevoir l&apos;évènements de raccourcis global quand il est en arrière plan.&lt;br/&gt;&lt;br/&gt;C&apos;est parce que la fonction d&apos;Accès Universel nommée &apos;Activer l&apos;accès pour les périphériques d&apos;aide&apos; est actuellement désactivée.&lt;br/&gt;&lt;br/&gt;Veuillez &lt;a href=&quot; &quot;&gt;activer ce paramètre&lt;/a&gt; et continuer quand c&apos;est fait.</translation>
    </message>
</context>
<context>
    <name>GlobalShortcutTarget</name>
    <message>
        <location filename="GlobalShortcutTarget.ui"/>
        <source>Whisper Target</source>
        <translation>Cible du chuchotement</translation>
    </message>
    <message>
        <location/>
        <source>Whisper to list of Users</source>
        <translation>Chuchoter à une liste d&apos;utilisateurs</translation>
    </message>
    <message>
        <location/>
        <source>Channel Target</source>
        <translation>Salon cible</translation>
    </message>
    <message>
        <location/>
        <source>Restrict to Group</source>
        <translation>Restreindre au groupe</translation>
    </message>
    <message>
        <location/>
        <source>If specified, only members of this group will receive the whisper.</source>
        <translation>Si spécifié, seulement les membres de ce groupe recevront le chuchotement.</translation>
    </message>
    <message>
        <source>If checked the whisper will also be transmitted to linked channels.</source>
        <translation type="obsolete">Si cochée, le chuchotement sera aussi transmit aux salons liés.</translation>
    </message>
    <message>
        <source>Whisper to Linked channels</source>
        <translation type="obsolete">Chuchoter aux salons liés</translation>
    </message>
    <message>
        <source>If checked this whisper will also be sent to the subchannels of the channel target.</source>
        <translation type="obsolete">Si cochée, ce chuchotement sera aussi transmit aux sous salons du salon cible.</translation>
    </message>
    <message>
        <location/>
        <source>List of users</source>
        <translation>Liste des utilisateurs</translation>
    </message>
    <message>
        <location/>
        <source>Add</source>
        <translation>Ajouter</translation>
    </message>
    <message>
        <location/>
        <source>Remove</source>
        <translation>Supprimer</translation>
    </message>
    <message>
        <location/>
        <source>Modifiers</source>
        <translation>Modificateurs</translation>
    </message>
    <message>
        <location/>
        <source>Do not send positional audio information when using this whisper shortcut.</source>
        <translation>Ne pas envoyer les informations de positionnement audio en utilisant le chuchotement.</translation>
    </message>
    <message>
        <location/>
        <source>Ignore positional audio</source>
        <translation>Ignorer le positionement audio</translation>
    </message>
    <message>
        <location/>
        <source>Shout to Linked channels</source>
        <translation>Crier aux salons liés</translation>
    </message>
    <message>
        <location/>
        <source>Shout to subchannels</source>
        <translation>Crier aux sous-salons</translation>
    </message>
    <message>
        <location/>
        <source>Shout to Channel</source>
        <translation>Crier au salon</translation>
    </message>
    <message>
        <location/>
        <source>The whisper will also be transmitted to linked channels.</source>
        <translation>Le chuchotement sera aussi transmis aux salons liés.</translation>
    </message>
    <message>
        <location/>
        <source>The whisper will also be sent to the subchannels of the channel target.</source>
        <translation>Le chuchotement sera aussi transmis aux sous salons du salon cible.</translation>
    </message>
</context>
<context>
    <name>GlobalShortcutWinConfig</name>
    <message>
        <source>Shortcuts</source>
        <translation type="obsolete">Raccourcis</translation>
    </message>
    <message>
        <source>Function</source>
        <translation type="obsolete">Fonction</translation>
    </message>
    <message>
        <source>Shortcut</source>
        <translation type="obsolete">Raccourcis</translation>
    </message>
    <message>
        <source>Shortcut bound to %1.</source>
        <translation type="obsolete">Raccourcis lié à %1.</translation>
    </message>
    <message>
        <source>&lt;b&gt;This is the global shortcut bound to %1&lt;/b&gt;&lt;br /&gt;Click this field and then the desired key/button combo to rebind. Double-click to clear.</source>
        <translation type="obsolete">&lt;b&gt;Il s&apos;agit du raccourcis lié à %1&lt;/b&gt;&lt;br /&gt;Cliquez dans ce champ et tappez la combinaison de touche/bouton à relier. Double-cliquez pour effacer.</translation>
    </message>
</context>
<context>
    <name>GlobalShortcutX</name>
    <message>
        <location filename="GlobalShortcut_unix.cpp" line="+325"/>
        <source>Mouse %1</source>
        <translation>Souris %1</translation>
    </message>
</context>
<context>
    <name>GlobalShortcutXConfig</name>
    <message>
        <source>Shortcuts</source>
        <translation type="obsolete">Raccourcis</translation>
    </message>
    <message>
        <source>Function</source>
        <translation type="obsolete">Fonction</translation>
    </message>
    <message>
        <source>Shortcut</source>
        <translation type="obsolete">Raccourcis</translation>
    </message>
    <message>
        <source>Shortcut bound to %1.</source>
        <translation type="obsolete">Raccourcis lié à %1.</translation>
    </message>
    <message>
        <source>&lt;b&gt;This is the global shortcut bound to %1&lt;/b&gt;&lt;br /&gt;Click this field and then the desired key combo to rebind. Double-click to clear.</source>
        <translation type="obsolete">&lt;b&gt;Il s&apos;agit du raccourcis lié à %1&lt;/b&gt;&lt;br /&gt;Cliquez dans ce champ et tappez la combinaison de touche/bouton à relier. Double-cliquez pour effacer.</translation>
    </message>
</context>
<context>
    <name>LCD</name>
    <message>
        <location filename="LCD.cpp" line="+279"/>
        <source>Not connected</source>
        <translation>Non connecté</translation>
    </message>
</context>
<context>
    <name>LCDConfig</name>
    <message>
        <source>Graphic</source>
        <translation type="obsolete">Graphique</translation>
    </message>
    <message>
        <location line="-160"/>
        <source>Enable this device</source>
        <translation>Activé ce périphérique</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>LCD</source>
        <translation>LCD</translation>
    </message>
    <message>
        <location filename="LCD.ui"/>
        <source>Form</source>
        <translation>Form</translation>
    </message>
    <message>
        <location/>
        <source>Devices</source>
        <translation>Périphériques</translation>
    </message>
    <message>
        <location/>
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <source>Type</source>
        <translation type="obsolete">Type</translation>
    </message>
    <message>
        <location/>
        <source>&lt;p&gt;This is the list of available LCD devices on your system.  It lists devices by name, but also includes the size of the display. Mumble supports outputting to several LCD devices at a time.&lt;/p&gt;
&lt;h3&gt;Size:&lt;/h3&gt;
&lt;p&gt;
This field describes the size of an LCD device. The size is given either in pixels (for Graphic LCDs) or in characters (for Character LCDs).&lt;/p&gt;
&lt;h3&gt;Enabled:&lt;/h3&gt;
&lt;p&gt;This decides whether Mumble should draw to a particular LCD device.&lt;/p&gt;</source>
        <translation>&lt;p&gt;C&apos;est la liste des périphériques LCD disponibles sur votre système. Il liste leur nom, mais inclus aussi la taille de l&apos;affichage. Mumble supporte l&apos;affichage simultané sur plusieurs LCD.&lt;/p&gt;
&lt;h3&gt;Taille :&lt;/h3&gt;
&lt;p&gt;Ce champ décrit la taille d&apos;un périphérique LCD. La taille est donnée en pixels (pour les LCD graphiqies) ou en caractères (pour les LCD texte).&lt;/p&gt;
&lt;h3&gt;Activé :&lt;/h3&gt;
&lt;p&gt;Indique à Mumble s&apos;il doit utiliser un périphérique LCD particulier.&lt;/p&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Size</source>
        <translation>Taille</translation>
    </message>
    <message>
        <location/>
        <source>Enabled</source>
        <translation>Activé</translation>
    </message>
    <message>
        <location/>
        <source>Views</source>
        <translation>Vues</translation>
    </message>
    <message>
        <location/>
        <source>Minimum Column Width</source>
        <translation>Taille minimale des colonnes</translation>
    </message>
    <message>
        <location/>
        <source>&lt;p&gt;This option decides the minimum width a column in the User View.&lt;/p&gt;
&lt;p&gt;If too many people are speaking at once, the User View will split itself into columns. You can use this option to pick a compromise between number of users shown on the LCD, and width of user names.&lt;/p&gt;
</source>
        <translation>&lt;p&gt;Cette option définie la taille minimale de la colonne dans la vue de utilisateur.&lt;/p&gt;
&lt;p&gt;Si trop de personnes parlent en même temps, la vue utilisateur sera coupée en colonnes. Vous pouvez utliser cette option pour spécifier un compromis entre le nombre d&apos;utilisateurs affichés sur le LCD, et la longueur des noms d&apos;utilisateurs.&lt;/p&gt;</translation>
    </message>
    <message>
        <location/>
        <source>This setting decides the width of column splitter.</source>
        <translation>Ce paramètre détermine la largeur de la séparation entre les colonnes.</translation>
    </message>
    <message>
        <source>TextLabel</source>
        <translation type="obsolete">TextLabel</translation>
    </message>
    <message>
        <location/>
        <source>Splitter Width</source>
        <translation>Largeur de la séparation</translation>
    </message>
</context>
<context>
    <name>Log</name>
    <message>
        <location filename="Log.cpp" line="+220"/>
        <source>Debug</source>
        <translation>Debogage</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Critical</source>
        <translation>Critique</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Warning</source>
        <translation>Avertissement</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Information</source>
        <translation>Information</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Server Connected</source>
        <translation>Serveur Connecté</translation>
    </message>
    <message>
        <source>Server Deconnected</source>
        <translation type="obsolete">Serveur Déconnecté</translation>
    </message>
    <message>
        <source>Player Joined Server</source>
        <translation type="obsolete">Joueur arrive sur Le Serveur</translation>
    </message>
    <message>
        <source>Player Left Server</source>
        <translation type="obsolete">Joueur quitte le serveur</translation>
    </message>
    <message>
        <source>Player kicked (you or by you)</source>
        <translation type="obsolete">Joueur expulsé (vous ou par vous)</translation>
    </message>
    <message>
        <source>Player kicked</source>
        <translation type="obsolete">Joueur expulsé</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>You self-muted/deafened</source>
        <translation>Vous vous êtes rendu(e) sourd(e)/muet(te)</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Other self-muted/deafened</source>
        <translation>Les autres sont rendus sourd/muet</translation>
    </message>
    <message>
        <source>Player muted (you)</source>
        <translation type="obsolete">Joueur muet (vous)</translation>
    </message>
    <message>
        <source>Player muted (by you)</source>
        <translation type="obsolete">Joueur muet (par vous)</translation>
    </message>
    <message>
        <source>Player muted (other)</source>
        <translation type="obsolete">Joueur muet (autre)</translation>
    </message>
    <message>
        <source>Player Joined Channel</source>
        <translation type="obsolete">Joueur arrive sur le canal</translation>
    </message>
    <message>
        <source>Player Left Channel</source>
        <translation type="obsolete">Joueur quitte le canal</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Permission Denied</source>
        <translation>Permission refusée</translation>
    </message>
    <message>
        <source>[%2] %1</source>
        <translation type="obsolete">[%2] %1</translation>
    </message>
    <message>
        <location line="-13"/>
        <source>Server Disconnected</source>
        <translation>Déconnecté du serveur</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>User Joined Server</source>
        <translation>Un utilisateur a rejoint le serveur</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>User Left Server</source>
        <translation>Un utilisateur quitte le serveur</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>User recording state changed</source>
        <translation>Les paramètres d&apos;enregistrement d&apos;un utilisateur ont changés</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>User kicked (you or by you)</source>
        <translation>Un utilisateur expulsé (vous ou par vous)</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>User kicked</source>
        <translation>Un utilisateur expulsé</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>User muted (you)</source>
        <translation>Un utilisateur rendu muet (vous)</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>User muted (by you)</source>
        <translation>Un utilisateur rendu muet (par vous)</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>User muted (other)</source>
        <translation>Un utilisateur rendu muet (autre)</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>User Joined Channel</source>
        <translation>Un utilisateur arrive sur le salon</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>User Left Channel</source>
        <translation>Un utilisateur quitte le salon</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Text Message</source>
        <translation>Message texte</translation>
    </message>
    <message>
        <location line="+52"/>
        <source>the server</source>
        <translation>le serveur</translation>
    </message>
    <message>
        <location line="+117"/>
        <source>[[ Text object too large to display ]]</source>
        <translation>[[ Texte trop long pour être affiché ]]</translation>
    </message>
    <message>
        <location line="+42"/>
        <source>[Date changed to %1]
</source>
        <translation>[Date changée en %1]</translation>
    </message>
    <message>
        <location line="+142"/>
        <source>link to %1</source>
        <translation>lien pour %1</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>ftp link to %1</source>
        <translation>lien FTP pour %1</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>player link</source>
        <translation>lien du joueur</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>channel link</source>
        <translation>lien du salon</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1 link</source>
        <translation>%1 lien</translation>
    </message>
</context>
<context>
    <name>LogConfig</name>
    <message>
        <location line="-521"/>
        <source>Toggle console for %1 events</source>
        <translation>Active/désactive la console pour les événements %1</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Toggle pop-up notifications for %1 events</source>
        <translation>Active/désactive les notifications sous forme de bulle pour les événements %1</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Toggle Text-To-Speech for %1 events</source>
        <translation>Active/désactive la synthèse vocale pour les événements %1</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Click here to toggle sound notification for %1 events</source>
        <translation>Cliquez ici pour activer/désactiver les notifications sonores pour les événements %1</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Path to sound file used for sound notifications in the case of %1 events&lt;br /&gt;Single click to play&lt;br /&gt;Double-click to change</source>
        <translation>Chemin d&apos;accès au fichier son utilisé pour les notifications d&apos;évènements %1&lt;br/&gt;Cliquez une fois pour écouter&lt;br/&gt;Cliquez deux fois pour le modifier</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Click here to toggle console output for %1 events.&lt;br /&gt;If checked, this option makes Mumble output all %1 events in its message log.</source>
        <translation>Active/désactive les messages sur la console pour les événements %1.&lt;br/&gt;Si l&apos;option est activée, Mumble affichera tous les evénements de %1 dans la fenêtre d&apos;historique.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Click here to toggle pop-up notifications for %1 events.&lt;br /&gt;If checked, a notification pop-up will be created by Mumble for every %1 event.</source>
        <oldsource>Click here to toggle pop-up notifications for %1 events.&lt;br /&gt;If checked, a notification pop-up will be created by mumble for every %1 event.</oldsource>
        <translation>Active/désactive les notifications sous forme de bulle  pour les événements %1.&lt;br/&gt;Si l&apos;option est activée, Mumble affichera tous les evénements de %1 dans une bulle de notification.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Path to sound file used for sound notifications in the case of %1 events.&lt;br /&gt;Single click to play&lt;br /&gt;Double-click to change&lt;br /&gt;Ensure that sound notifications for these events are enabled or this field will not have any effect.</source>
        <translation>Chemin d&apos;accès au fichier son utilisé pour les notifications d&apos;évènements %1&lt;br/&gt;Cliquez une fois pour écouter&lt;br/&gt;Cliquez deux fois pour le modifier&lt;br/&gt;Veuillez vérifier que la notification sonore pour ces évènements est activée, ou ce paramètre n&apos;aura aucun effet.</translation>
    </message>
    <message>
        <location line="-2"/>
        <source>Click here to toggle Text-To-Speech for %1 events.&lt;br /&gt;If checked, Mumble uses Text-To-Speech to read %1 events out loud to you. Text-To-Speech is also able to read the contents of the event which is not true for sound files. Text-To-Speech and sound files cannot be used at the same time.</source>
        <oldsource>Click here to toggle sound notification for %1 events.&lt;br /&gt;If checked, Mumble uses a soundfile predefined by you to indicate %1 events. Soundfiles and Text-To-Speech cannot be used at the same time.</oldsource>
        <translation>Active/désactive la synthèse vocale pour les évènements %1.&lt;br/&gt;Si l&apos;option est activée, Mumble utilisera la synthèse vocale pour vous lire tous les evènements de %1. La synthèse vocale est aussi capable de lire le contenu des évènements, ce qui n&apos;est pas le cas des fichiers sonores. La synthèse vocale et les fichiers sonores ne peuvent pas être activés en même temps.
</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Click here to toggle sound notification for %1 events.&lt;br /&gt;If checked, Mumble uses a sound file predefined by you to indicate %1 events. Sound files and Text-To-Speech cannot be used at the same time.</source>
        <oldsource>Path to soundfile used for sound notifications in the case of %1 events.&lt;br /&gt;Single click to play&lt;br /&gt;Doubleclick to change&lt;br /&gt;Be sure that sound notifications for these events are enabled or this field will not have any effect.</oldsource>
        <translation>Active/désactive les notifications sonores pour les évènements %1.&lt;br/&gt;Si l&apos;option est activée, Mumble utilisera le fichier son que vous avez prédéfini pour vous indiquer tous les evènements de %1. Les fichiers sonores et la synthèse vocale ne peuvent pas être activés en même temps.</translation>
    </message>
    <message>
        <location filename="Log.ui"/>
        <source>Messages</source>
        <translation>Messages</translation>
    </message>
    <message>
        <location/>
        <source>Console</source>
        <translation>Console</translation>
    </message>
    <message>
        <source>TTS</source>
        <translation type="obsolete">TTS</translation>
    </message>
    <message>
        <source>Enable console for %1</source>
        <translation type="obsolete">Activer la console pour %1</translation>
    </message>
    <message>
        <source>Enable Text-To-Speech for %1</source>
        <translation type="obsolete">Activer le Text-To-Speech pour %1</translation>
    </message>
    <message>
        <location/>
        <source>Text To Speech</source>
        <translation>Synthèse vocale</translation>
    </message>
    <message>
        <location/>
        <source>Volume</source>
        <translation>Volume</translation>
    </message>
    <message>
        <location/>
        <source>Volume of Text-To-Speech Engine</source>
        <translation>Volume du moteur de synthèse vocale</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is the volume used for the speech synthesis.&lt;/b&gt;</source>
        <translation>&lt;b&gt;C&apos;est le volume utilisé pour le mode synthèse.&lt;/b&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Length threshold</source>
        <translation>Seuil de longueur</translation>
    </message>
    <message>
        <location/>
        <source>Message length threshold for Text-To-Speech Engine</source>
        <translation>Seuil de longueur pour les messages de la synthèse vocale</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is the length threshold used for the Text-To-Speech Engine.&lt;/b&gt;&lt;br /&gt;Messages longer than this limit will not be read aloud in their full length.</source>
        <translation>&lt;b&gt;Il s&apos;agit du seuil de longueur utilisé pour la synthèse vocale.&lt;/b&gt;&lt;br /&gt;Les messages plus longs que cette limite ne seront pas lu en entier.</translation>
    </message>
    <message>
        <location/>
        <source>Whisper</source>
        <translation>Chuchotement</translation>
    </message>
    <message>
        <location/>
        <source>If checked you will only hear whispers from users you added to your friend list.</source>
        <translation>Si cochée, vous n&apos;entendrez les chuchotements que des utilisateurs que vous avez ajouté à votre liste d&apos;amis.</translation>
    </message>
    <message>
        <location/>
        <source>Only accept whispers from friends</source>
        <translation>N&apos;accepter les chuchotements que des amis</translation>
    </message>
    <message>
        <location/>
        <source>Message</source>
        <translation>Message</translation>
    </message>
    <message>
        <location/>
        <source>Notification</source>
        <translation>Notification</translation>
    </message>
    <message>
        <location/>
        <source>Text-To-Speech</source>
        <translation>Synthèse vocale</translation>
    </message>
    <message>
        <location/>
        <source>Soundfile</source>
        <translation>Fichier son</translation>
    </message>
    <message>
        <location/>
        <source>Path</source>
        <translation>Chemin</translation>
    </message>
    <message>
        <location/>
        <source> Characters</source>
        <translation> Caractères</translation>
    </message>
</context>
<context>
    <name>LookConfig</name>
    <message>
        <location filename="LookConfig.ui"/>
        <source>Language</source>
        <translation>Langue</translation>
    </message>
    <message>
        <location/>
        <source>Look and Feel</source>
        <translation>Apparence</translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="+52"/>
        <location line="+11"/>
        <source>System default</source>
        <translation>Par défault</translation>
    </message>
    <message>
        <location filename="LookConfig.ui"/>
        <source>Language to use (requires restart)</source>
        <translation>Langue a utiliser (nécessite un redémarrage)</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets which language Mumble should use.&lt;/b&gt;&lt;br /&gt;You have to restart Mumble to use the new language.</source>
        <translation>&lt;b&gt;Défini quel langue Mumble devrait utiliser&lt;/b&gt;&lt;br /&gt;Vous devez redémarrer Mumble pour utiliser une nouvelle langue.</translation>
    </message>
    <message>
        <location/>
        <source>Style</source>
        <translation>Style</translation>
    </message>
    <message>
        <location/>
        <source>Layout</source>
        <translation>Disposition</translation>
    </message>
    <message>
        <location/>
        <source>Classic</source>
        <translation>Classique</translation>
    </message>
    <message>
        <location/>
        <source>Stacked</source>
        <translation>Empilé</translation>
    </message>
    <message>
        <location/>
        <source>Hybrid</source>
        <translation>Hybride</translation>
    </message>
    <message>
        <location/>
        <source>Custom</source>
        <translation>Personnalisé</translation>
    </message>
    <message>
        <location/>
        <source>This changes the behavior when moving channels.</source>
        <translation>Change le comportement lors du déplacement des salons.</translation>
    </message>
    <message>
        <location/>
        <source>This sets the behavior of channel drags; it can be used to prevent accidental dragging. &lt;i&gt;Move&lt;/i&gt; moves the channel without prompting. &lt;i&gt;Do Nothing&lt;/i&gt; does nothing and prints an error message. &lt;i&gt;Ask&lt;/i&gt; uses a message box to confirm if you really wanted to move the channel.</source>
        <oldsource>This sets the behavior of channel drags; it can be used to prevent accidental dragging. &lt;i&gt;Move Channel&lt;/i&gt; moves the channel without prompting. &lt;i&gt;Do Nothing&lt;/i&gt; does nothing and prints an error message. &lt;i&gt;Ask&lt;/i&gt; uses a message box to confirm if you really wanted to move the channel.</oldsource>
        <translation>Définit le comportement en cas de déplacement d&apos;un salon ; peut être utilisé pour prévenir un déplacement accidentel. &lt;i&gt;Déplacer&lt;/i&gt; déplace le salon sans rien demander. &lt;i&gt;Ne rien faire&lt;/i&gt; n&apos;effectue aucune opération et affiche un message d&apos;erreur. &lt;i&gt;Demander&lt;/i&gt; affiche une fenêtre de confirmation pour confirmer le déplacement du salon.</translation>
    </message>
    <message>
        <location/>
        <source>Basic widget style</source>
        <translation>Modèle de contrôle basique</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This sets the basic look and feel to use.&lt;/b&gt;</source>
        <translation>&lt;b&gt;Ceci spécifie l&apos;apparence à utiliser.&lt;/b&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Skin</source>
        <translation>Thème</translation>
    </message>
    <message>
        <source>...</source>
        <translation type="obsolete">...</translation>
    </message>
    <message>
        <location/>
        <source>This setting controls when the application will be always on top.</source>
        <translation>Contrôle quand l&apos;application sera toujours au dessus.</translation>
    </message>
    <message>
        <source>This setting controls in which situations the application will stay always on top. If you select &lt;i&gt;Never&lt;/i&gt; the application will not stay on top. &lt;i&gt;Always&lt;/i&gt; will always keep the application on top. &lt;i&gt;In minimal view&lt;/i&gt; / &lt;i&gt;In normal mode&lt;/i&gt; will only raise the application on top when minimal view is activated / deactivated.  </source>
        <translation type="obsolete">Définit sous quelles conditions l&apos;application restera toujours au dessus. Si vous sélectionnez &lt;i&gt;Jamais&lt;/i&gt;, l&apos;application ne sera jamais qu dessus. &lt;i&gt;Toujours&lt;/i&gt; permettra à l&apos;application de toujours rester au dessus. &lt;i&gt;En vue minimale&lt;/&gt; / &lt;i&gt;En vue normale&lt;/i&gt; n&apos;affichera l&apos;application toujours au dessus quand l&apos;affichage en vue minimale sera activé / désactivé.</translation>
    </message>
    <message>
        <location/>
        <source>Never</source>
        <translation>Jamais</translation>
    </message>
    <message>
        <location/>
        <source>Always</source>
        <translation>Toujours</translation>
    </message>
    <message>
        <location/>
        <source>In minimal view</source>
        <translation>En vue minimale</translation>
    </message>
    <message>
        <location/>
        <source>In normal view</source>
        <translation>En vue normale</translation>
    </message>
    <message>
        <source>Check to show chat bar</source>
        <translation type="obsolete">Cocher pour afficher la zone de saisir</translation>
    </message>
    <message>
        <source>&lt;b&gt;If checked the chat bar is shown.&lt;/b&gt;&lt;br /&gt;Uncheck this to hide it.</source>
        <oldsource>&lt;b&gt;If checked the chat bar is shown&lt;/b&gt;&lt;br /&gt;Uncheck to hide it</oldsource>
        <translation type="obsolete">&lt;b&gt;Si coché, la zone de saisie est affichée&lt;/b&gt;&lt;br/&gt;Décocher pour la masquer.</translation>
    </message>
    <message>
        <source>Show chatbar</source>
        <translation type="obsolete">Afficher la zone de saisie</translation>
    </message>
    <message>
        <location/>
        <source>Show number of users in each channel</source>
        <translation>Afficher le nombre l&apos;utilisateurs dans chaque salon</translation>
    </message>
    <message>
        <location/>
        <source>Show channel user count</source>
        <translation>Afficher le nombre d&apos;utilisateurs</translation>
    </message>
    <message>
        <location/>
        <source>Skin file to use</source>
        <translation>Fichier de thème à utiliser</translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets which skin Mumble should use.&lt;/b&gt;&lt;br /&gt;The skin is a style file applied on top of the basic widget style.</source>
        <translation type="obsolete">&lt;b&gt;Ceci spécifie quel thème Mumble devrait utiliser.&lt;/b&gt;&lt;br /&gt;Le thème est un fichier de style appliqué à un contrôle basique.</translation>
    </message>
    <message>
        <source>Use Horizontal Splitter</source>
        <translation type="obsolete">Utiliser un séparateur horizontal</translation>
    </message>
    <message>
        <source>Use horizontal or vertical splitter for the main window</source>
        <translation type="obsolete">Utiliser un séparateur horizontal ou vertical pour une même fenêtre</translation>
    </message>
    <message>
        <source>&lt;b&gt;If set, players will be shown above subchannels in the channel view.&lt;/b&gt;&lt;br /&gt;A restart of Mumble is required to see the change.</source>
        <translation type="obsolete">&lt;b&gt;Si renseigné, les joueurs seront indiquées ci-dessus dans le sous-canal.&lt;/b&gt;&lt;br /&gt;Un redémarrage de Mumble est requis pour voir le changement.</translation>
    </message>
    <message>
        <source>Expand All Channels</source>
        <translation type="obsolete">Étendre tout les canaux</translation>
    </message>
    <message>
        <source>Expand all channels when connecting</source>
        <translation type="obsolete">Etendre tout les canaux à la connection</translation>
    </message>
    <message>
        <source>&lt;b&gt;If set, all channels will be expanded by default when you connect to a server.&lt;/b&gt;</source>
        <translation type="obsolete">&lt;b&gt;Si coché, tous les salons seront étendus par défaut quand vous vous connecterez à un serveur&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="+14"/>
        <source>User Interface</source>
        <translation>Interface utilisateur</translation>
    </message>
    <message>
        <location line="+130"/>
        <source>Choose skin file</source>
        <translation>Choisir le fichier thème</translation>
    </message>
    <message>
        <source>Players above Channels</source>
        <translation type="obsolete">Joueurs au dessus des salons</translation>
    </message>
    <message>
        <source>List players above subchannels (requires restart).</source>
        <translation type="obsolete">Liste les joueurs au dessus des sous-salons ( nécessite un redémarrage ).</translation>
    </message>
    <message>
        <source>&lt;b&gt;If set, players will be shown above subchannels in the channel view.&lt;/b&gt;&lt;br /&gt;A restart of mumble is required to see the change.</source>
        <translation type="obsolete">&lt;b&gt;Si définis, les joueurs seront montrés au dessus des sous-cannaux dans la vue des cannaux.&lt;/b&gt;&lt;br /&gt;Un redémarrage de mumble est nécessaire pour que ce changement fasse effet.</translation>
    </message>
    <message>
        <location filename="LookConfig.ui"/>
        <source>&lt;b&gt;This sets which skin Mumble should use.&lt;/b&gt;&lt;br /&gt;The skin is a style file applied on top of the basic widget style. If there are icons in the same directory as the style sheet, those will replace the default icons.</source>
        <translation>&lt;b&gt;Définit le skin Mumble à utiliser.&lt;/b&gt;&lt;br/&gt;Ce skin est un fichier de style appliqué sur le style par défaut. Si il y a des icônes dans le même dossier que le style, celles-ci remplacent les icônes par défaut.</translation>
    </message>
    <message>
        <source>Form</source>
        <translation type="obsolete">Form</translation>
    </message>
    <message>
        <source>New version check</source>
        <translation type="obsolete">Vérifer nouvelle version</translation>
    </message>
    <message>
        <source>Check for updates on startup</source>
        <translation type="obsolete">Vérifier les mises a jour au démarrage</translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="-140"/>
        <source>None</source>
        <translation>Aucun</translation>
    </message>
    <message>
        <source>Only with players</source>
        <translation type="obsolete">Seulement avec les joueurs</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>All</source>
        <translation>Tous</translation>
    </message>
    <message>
        <location filename="LookConfig.ui"/>
        <source>Expand</source>
        <translation>Elargir</translation>
    </message>
    <message>
        <location/>
        <source>When to automatically expand channels</source>
        <translation>Quand étendre automatiquement les salons</translation>
    </message>
    <message>
        <location filename="LookConfig.cpp" line="-1"/>
        <source>Only with users</source>
        <translation>Seulement quand il y a des utilisateurs</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Ask</source>
        <translation>Demander</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Do Nothing</source>
        <translation>Ne rien faire</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Move</source>
        <translation>Déplacer</translation>
    </message>
    <message>
        <location filename="LookConfig.ui"/>
        <source>Channel Dragging</source>
        <translation>Faire glisser le salon</translation>
    </message>
    <message>
        <location/>
        <source>Ask whether to close or minimize when quitting Mumble.</source>
        <translation>Demande si on quitte ou réduit la fenêtre en fermant Mumble.</translation>
    </message>
    <message>
        <location/>
        <source>Ask on quit while connected</source>
        <translation>Confirmer l&apos;arrêt si connecté</translation>
    </message>
    <message>
        <source>Make the Mumble window appear on top of other windows.</source>
        <translation type="obsolete">Permettre à Mumble d&apos;apparaitre au dessus des autres fenêtres.</translation>
    </message>
    <message>
        <source>&lt;b&gt;This makes the Mumble window a topmost window.&lt;/b&gt;</source>
        <translation type="obsolete">&lt;b&gt;Affiche la fenêtre de Mumble au dessus des autres fenêtres.&lt;/b&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Always On Top</source>
        <translation>Toujours au dessus</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;If set, minimizing the Mumble main window will cause it to be hidden and accessible only from the tray. Otherwise, it will be minimized as a window normally would.&lt;/b&gt;</source>
        <translation>&lt;b&gt;Si cochée, réduire la fenêtre principale de Mumble la cachera et elle ne sera accessible que depuis la barre des tâches. Sinon, il sera réduit comme toute fenêtre normale.&lt;/b&gt;</translation>
    </message>
    <message>
        <location/>
        <source>This sets which channels to automatically expand. &lt;i&gt;None&lt;/i&gt; and &lt;i&gt;All&lt;/i&gt; will expand no or all channels, while &lt;i&gt;Only with users&lt;/i&gt; will expand and collapse channels as users join and leave them.</source>
        <translation>Définit quels sont les salons à étendre automatiquement. &lt;i&gt;Aucun&lt;/i&gt; et &lt;i&gt;Tous&lt;/i&gt; étendra aucun ou tous les salons, alors que &lt;i&gt;Seulement avec des utilisateurs&lt;/i&gt; étendra et réduira les salons dès que les utilisateurs les joindront ou les quitteront.</translation>
    </message>
    <message>
        <location/>
        <source>List users above subchannels (requires restart).</source>
        <translation>Liste les utilisateurs au dessus des sous-salons (redémarrage requis).</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;If set, users will be shown above subchannels in the channel view.&lt;/b&gt;&lt;br /&gt;A restart of Mumble is required to see the change.</source>
        <translation>&lt;b&gt;Si cochée, les utilisateurs seront affiché au dessus des sous-salons dans l&apos;arborescence.&lt;/b&gt;&lt;br/&gt;Mumble doit être redémarré pour activer les changements.</translation>
    </message>
    <message>
        <location/>
        <source>Users above Channels</source>
        <translation>Utilisateurs en haut des salons</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;If set, will verify you want to quit if connected.&lt;/b&gt;</source>
        <translation>&lt;b&gt;Si cochée, vérifie si vous voulez quitter si vous êtes connecté.&lt;/b&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Hide in tray when minimized</source>
        <translation>Réduire dans la barre des tâches</translation>
    </message>
    <message>
        <source>Show all configuration items</source>
        <translation type="obsolete">Afficher tout les outils de configuration</translation>
    </message>
    <message>
        <source>Expert Config</source>
        <translation type="obsolete">Configuration avancée</translation>
    </message>
    <message>
        <location/>
        <source>Hide the main Mumble window in the tray when it is minimized.</source>
        <translation>Cache la fenêtre principale de Mumble dans la barre des taches quand il est réduit.</translation>
    </message>
    <message>
        <location/>
        <source>Displays talking status in system tray</source>
        <translation>Affiche mon statut vocal dans la barre des tâches</translation>
    </message>
    <message>
        <location/>
        <source>Show talking status in tray icon</source>
        <translation>Afficher mon statut vocal dans la barre des tâches</translation>
    </message>
    <message>
        <location/>
        <source>This setting controls in which situations the application will stay always on top. If you select &lt;i&gt;Never&lt;/i&gt; the application will not stay on top. &lt;i&gt;Always&lt;/i&gt; will always keep the application on top. &lt;i&gt;In minimal view&lt;/i&gt; / &lt;i&gt;In normal view&lt;/i&gt; will only keep the application always on top when minimal view is activated / deactivated.</source>
        <translation>Ce paramétrage détermine dans quelle situation l&apos;application restera toujours au dessus. Si vous sélectionnez &lt;i&gt;Jamais&lt;/i&gt;, l&apos;application ne restera jamais au dessus. &lt;i&gt;Toujours&lt;/i&gt; gardera toujours l&apos;application au dessus.&lt;i&gt;En vue minimale&lt;/i&gt; ne gardera l&apos;application au dessus que lorsque la vue minimale est activée / désactivée.</translation>
    </message>
    <message>
        <location/>
        <source>Show context menu in menu bar</source>
        <translation>Menu contextuel dans la barre de menu</translation>
    </message>
    <message>
        <location/>
        <source>Apply some high contrast optimizations for visually impaired users</source>
        <translation>Applique quelques optimisations de contraste pour les utilisateurs malvoyants</translation>
    </message>
    <message>
        <location/>
        <source>Optimize for high contrast</source>
        <translation>Augmenter le contraste</translation>
    </message>
    <message>
        <location/>
        <source>Application</source>
        <translation>Application</translation>
    </message>
    <message>
        <location/>
        <source>Adds user and channel context menus into the menu bar</source>
        <translation>Ajoute les menus contextuels d&apos;utilisateur et de salon dans la barre de menu</translation>
    </message>
    <message>
        <location/>
        <source>Tray Icon</source>
        <translation>Icône de la barre des tâches</translation>
    </message>
    <message>
        <location/>
        <source>Channel Tree</source>
        <translation>Arborescence des salons</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Browse...</source>
        <translation>&amp;Parcourir...</translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <location filename="main.cpp" line="-52"/>
        <source>Welcome to Mumble.</source>
        <translation>Bienvenue sur Mumble.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="+171"/>
        <location line="+2048"/>
        <source>Root</source>
        <translation>Racine</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>&amp;Quit Mumble</source>
        <translation>&amp;Quitter Mumble</translation>
    </message>
    <message>
        <location/>
        <source>Closes the program</source>
        <translation>Ferme le programme</translation>
    </message>
    <message>
        <location/>
        <source>Exits the application.</source>
        <translation>Quitte l&apos;application.</translation>
    </message>
    <message>
        <source>Ctrl+Q</source>
        <comment>Quit</comment>
        <translation type="obsolete">Quitter</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Connect</source>
        <translation>&amp;Connexion</translation>
    </message>
    <message>
        <location/>
        <source>Open the server connection dialog</source>
        <translation>Ouvrir la fenêtre de connexion du serveur</translation>
    </message>
    <message>
        <source>Shows a dialog of registered servers, and also allows quick connect.</source>
        <translation type="obsolete">Voir une fenêtre des serveurs enregistrés, et également autoriser la connexion rapide.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Disconnect</source>
        <translation>&amp;Déconnexion</translation>
    </message>
    <message>
        <location/>
        <source>Disconnect from server</source>
        <translation>Déconnexion du serveur</translation>
    </message>
    <message>
        <location/>
        <source>Disconnects you from the server.</source>
        <translation>Vous avez été déconnecté du serveur.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Ban lists</source>
        <translation>Liste des &amp;bannis</translation>
    </message>
    <message>
        <location/>
        <source>Edit ban lists on server</source>
        <translation>Éditer la liste des bannis du serveur</translation>
    </message>
    <message>
        <location/>
        <source>This lets you edit the server-side IP ban lists.</source>
        <translation>Éditer la liste des IP bannies stockée sur le serveur.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Kick</source>
        <translation>&amp;Expulser</translation>
    </message>
    <message>
        <source>Kick player (with reason)</source>
        <translation type="obsolete">Expulser joueur (avec raison)</translation>
    </message>
    <message>
        <source>Kick selected player off server. You&apos;ll be asked to specify a reason.</source>
        <translation type="obsolete">Éjecte le joueur sélectionné. Il vous sera demandé de spécifier une raison.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Ban</source>
        <translation>&amp;Bannir</translation>
    </message>
    <message>
        <source>Kick and ban player (with reason)</source>
        <translation type="obsolete">Expulser et bannir joueur (avec raison)</translation>
    </message>
    <message>
        <source>Kick and ban selected player from server. You&apos;ll be asked to specify a reason.</source>
        <translation type="obsolete">Expulser et bannir un joueur du serveur. Vous devrez spécifier une raison.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Mute</source>
        <translation>&amp;Muet</translation>
    </message>
    <message>
        <source>Mute player</source>
        <translation type="obsolete">Rendre le joueur muet</translation>
    </message>
    <message>
        <source>Mute or unmute player on server. Unmuting a deafened player will also undeafen them.</source>
        <translation type="obsolete">Rend muet ou rend la parole à un joueur du serveur. Rendre la parole à un sourd lui rendra également l&apos;ouie.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Deafen</source>
        <translation>Ren&amp;dre sourd</translation>
    </message>
    <message>
        <source>Deafen player</source>
        <translation type="obsolete">Rend le joueur sourd</translation>
    </message>
    <message>
        <source>Deafen or undeafen player on server. Deafening a player will also mute them.</source>
        <translation type="obsolete">Rend sourd ou rend l&apos;ouïe à un joueur du serveur. Rendre un joueur sourd va aussi le rendre muet.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Local Mute</source>
        <translation>Ne plus &amp;l&apos;écouter</translation>
    </message>
    <message>
        <source>Mute or unmute player locally.</source>
        <translation type="obsolete">Ne plus écouter ou écouter à nouveau un joueur du serveur.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Add</source>
        <translation>&amp;Ajouter</translation>
    </message>
    <message>
        <location/>
        <source>Add new channel</source>
        <translation>Ajouter un nouveau salon</translation>
    </message>
    <message>
        <location/>
        <source>This adds a new sub-channel to the currently selected channel.</source>
        <translation>Ceci ajoute un nouveau sous-salon au salon actuellement choisi.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Remove</source>
        <translation>Supp&amp;rimer</translation>
    </message>
    <message>
        <location/>
        <source>Remove channel</source>
        <translation>Supprimer le salon</translation>
    </message>
    <message>
        <location/>
        <source>This removes a channel and all sub-channels.</source>
        <translation>Ceci supprime un salon et tout ses sous-salons.</translation>
    </message>
    <message>
        <source>&amp;Edit ACL</source>
        <translation type="obsolete">&amp;Editer LCA</translation>
    </message>
    <message>
        <location/>
        <source>Edit Groups and ACL for channel</source>
        <translation>Editer groupe et LCA pour le salon</translation>
    </message>
    <message>
        <location/>
        <source>This opens the Group and ACL dialog for the channel, to control permissions.</source>
        <translation>Ceci ouvre la fenêtre de groupe et LCA pour le salon, pour contrôler les permissions.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Link</source>
        <translation>&amp;Lier</translation>
    </message>
    <message>
        <location/>
        <source>Link your channel to another channel</source>
        <translation>Lier votre salon à un autre salon</translation>
    </message>
    <message>
        <source>This links your current channel to the selected channel. If they have permission to speak in the other channel, players can now hear each other. This is a permanent link, and will last until manually unlinked or the server is restarted. Please see the shortcuts for push-to-link.</source>
        <translation type="obsolete">Lie le canal actuel avec le canal séplectionné. S&apos;ils ont la permission de parler dans l&apos;autre canal, les joueurs pourront s&apos;entendre entre eux. Ce lien est permanent et demeurera jusqu&apos;à ce qu&apos;il soit désactivé manuellement ou que le serveur soit redémarré. Veuillez consulter les raccourcis relatifs à la fonction « appuyez-pour-lier ».</translation>
    </message>
    <message>
        <source>&amp;Unlink</source>
        <translation type="obsolete">&amp;Délier</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="-2026"/>
        <source>&amp;User</source>
        <translation>&amp;Utilisateur</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>This is the chatbar&lt;br /&gt;If you enter text here and then press enter the text is sent to the user or channel that was selected. If nothing is selected the message is sent to your current channel.</source>
        <translation>C&apos;est la barre de dialogue&lt;br/&gt;Si vous saisissez du texte ici et pressez Entrée, le texte est envoyé à l&apos;utilisateur ou au salon sélectionné. Si rien n&apos;est sélectionné, le message est envoyé au salon actuel.</translation>
    </message>
    <message>
        <location/>
        <source>Chatbar</source>
        <translation>Barre de dialogue</translation>
    </message>
    <message>
        <location/>
        <source>Shows a dialog of registered servers, and also allows quick-connect.</source>
        <translation>Affiche une fenêtre des serveurs enregistrés, et permet aussi la connexion rapide.</translation>
    </message>
    <message>
        <location/>
        <source>Kick user (with reason)</source>
        <translation>Expulser l&apos;utilisateur (avec raison)</translation>
    </message>
    <message>
        <location/>
        <source>Kick selected user off server. You&apos;ll be asked to specify a reason.</source>
        <translation>Expulse l&apos;utilisateur sélectionné du serveur. Vous devrez saisir une raison.</translation>
    </message>
    <message>
        <location/>
        <source>Mute user</source>
        <translation>Rendre muet l&apos;utilisateur</translation>
    </message>
    <message>
        <location/>
        <source>Mute or unmute user on server. Unmuting a deafened user will also undeafen them.</source>
        <translation>Rend muet ou rend la parole à l&apos;utilisateur du serveur. Rendre la parole à un utilisateur sourd lui rend aussi l&apos;ouïe.</translation>
    </message>
    <message>
        <location/>
        <source>Kick and ban user (with reason)</source>
        <translation>Expulser et bannir l&apos;utilisateur (avec raison)</translation>
    </message>
    <message>
        <location/>
        <source>Kick and ban selected user from server. You&apos;ll be asked to specify a reason.</source>
        <translation>Expulse et banni l&apos;utilisateur sélectionné du serveur. Vous devrez saisir une raison.</translation>
    </message>
    <message>
        <location/>
        <source>Deafen user</source>
        <translation>Rendre sourd l&apos;utilisateur</translation>
    </message>
    <message>
        <location/>
        <source>Deafen or undeafen user on server. Deafening a user will also mute them.</source>
        <translation>Rend sourd ou rend l&apos;ouïe à un utilisateur du serveur. Rendre sourd un utilisateur le rend aussi muet.</translation>
    </message>
    <message>
        <location/>
        <source>Mute user locally</source>
        <translation>Rend un utilisateur muet localement</translation>
    </message>
    <message>
        <location/>
        <source>Mute or unmute user locally. Use this on other users in the same room.</source>
        <translation>Rend muet ou rend la parole à un utilisateur localement. Utiliser cette option pour les autres utilisateurs dans la même pièce.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Edit</source>
        <translation>&amp;Editer</translation>
    </message>
    <message>
        <location/>
        <source>This links your current channel to the selected channel. If users in a channel have permission to speak in the other channel, users can now hear each other. This is a permanent link, and will last until manually unlinked or the server is restarted. Please see the shortcuts for push-to-link.</source>
        <oldsource>This links your current channel to the selected channel. If they have permission to speak in the other channel, users can now hear each other. This is a permanent link, and will last until manually unlinked or the server is restarted. Please see the shortcuts for push-to-link.</oldsource>
        <translation>Lie votre salon actuel au salon sélectionné. Si des utilisateurs dans un salon ont la permission de parler dans l&apos;autre salon, les utilisateurs peuvent s&apos;entendre. C&apos;est une liaison permanente, et restera jusqu&apos;à ce qu&apos;ils soient manuellement déliés, ou que le serveur soit redémarré. Veuillez regarder le raccourcis pour Appuyez-pour-lier.</translation>
    </message>
    <message>
        <location/>
        <source>Unlink your channel from another channel</source>
        <translation>Délie votre salon d&apos;un autre salon</translation>
    </message>
    <message>
        <location/>
        <source>This unlinks your current channel from the selected channel.</source>
        <translation>Délie votre salon actuel du salon sélectionné.</translation>
    </message>
    <message>
        <source>Unlink &amp;All</source>
        <translation type="obsolete">Délier &amp;Tous</translation>
    </message>
    <message>
        <location/>
        <source>Unlinks your channel from all linked channels.</source>
        <translation>Délie votre salon de tous les autres salons liés.</translation>
    </message>
    <message>
        <location/>
        <source>This unlinks your current channel (not the selected one) from all linked channels.</source>
        <translation>Délie le salon actuel (pas celui qui est sélectionné) de tous les autres salons liés.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Reset</source>
        <translation>&amp;Remettre à zéro</translation>
    </message>
    <message>
        <location/>
        <source>Reset audio preprocessor</source>
        <translation>Remet à zéro le pré-processeur audio</translation>
    </message>
    <message>
        <location/>
        <source>This will reset the audio preprocessor, including noise cancellation, automatic gain and voice activity detection. If something suddenly worsens the audio environment (like dropping the microphone) and it was temporary, use this to avoid having to wait for the preprocessor to readjust.</source>
        <translation>Remet à zéro le pré-processeur audio, en incluant l&apos;annulation du bruit, le volume automatique et la détection de l&apos;activité vocale. Si quelque chose a soudainement perturbé l&apos;environnement audio (si le microphone tombe par exemple) et que ce n&apos;est que temporaire, utilisez cette fonctionalité pour éviter d&apos;avoir à attendre que le pré-processeur audio se ré-étalone.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Mute Self</source>
        <translation>Devenir &amp;muet(te)</translation>
    </message>
    <message>
        <location/>
        <source>Mute yourself</source>
        <translation>Se rendre muet(te)</translation>
    </message>
    <message>
        <location/>
        <source>Mute or unmute yourself. When muted, you will not send any data to the server. Unmuting while deafened will also undeafen.</source>
        <translation>Se rendre muet(te) ou non. Lorsque vous êtes muet(te), vous n&apos;envoyez pas de données au serveur. Enlever le mode muet alors que vous êtes en mode sourd enlèvrera également le mode sourd.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Deafen Self</source>
        <translation>Devenir s&amp;ourd(e)</translation>
    </message>
    <message>
        <location/>
        <source>Deafen yourself</source>
        <translation>Se rendre sourd(e)</translation>
    </message>
    <message>
        <location/>
        <source>Deafen or undeafen yourself. When deafened, you will not hear anything. Deafening yourself will also mute.</source>
        <translation>Se rendre sourd(e) ou non. Vous rendre sourd(e) vous rendra aussi muet(te).</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Text-To-Speech</source>
        <translation>Syn&amp;thèse vocale</translation>
    </message>
    <message>
        <location/>
        <source>Toggle Text-To-Speech</source>
        <translation>Activer/désactiver la synthèse vocale</translation>
    </message>
    <message>
        <location/>
        <source>Enable or disable the text-to-speech engine. Only messages enabled for TTS in the Configuration dialog will actually be spoken.</source>
        <translation>Active ou désactive le moteur de voix de synthèse. Seulement les messages activés pour la synthèse vocale dans la fenêtre de configuration seront entendus.</translation>
    </message>
    <message>
        <source>S&amp;tatistics</source>
        <translation type="obsolete">&amp;Statistiques</translation>
    </message>
    <message>
        <location/>
        <source>Display audio statistics</source>
        <translation>Affiche les statistiques audio</translation>
    </message>
    <message>
        <location/>
        <source>Pops up a small dialog with information about your current audio input.</source>
        <translation>Affiche une petit fenêtre d&apos;information sur votre sortie audio actuelle.</translation>
    </message>
    <message>
        <location/>
        <source>Forcibly unlink plugin</source>
        <translation>Forcer la désactivation du plugin</translation>
    </message>
    <message>
        <location/>
        <source>This forces the current plugin to unlink, which is handy if it is reading completely wrong data.</source>
        <translation>Force le plugin actuel à se désactiver, ce qui est utile s&apos;il lit des données erronées.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Settings</source>
        <translation>Paramètre&amp;s</translation>
    </message>
    <message>
        <location/>
        <source>Configure Mumble</source>
        <translation>Configurer Mumble</translation>
    </message>
    <message>
        <location/>
        <source>Allows you to change most settings for Mumble.</source>
        <translation>Permet de change la plupart des paramètres de Mumble.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;What&apos;s This?</source>
        <translation>&amp;Qu&apos;est-ce que c&apos;est ?</translation>
    </message>
    <message>
        <location/>
        <source>Enter What&apos;s This? mode</source>
        <translation>Entrer dans le mode « Qu&apos;est-ce que c&apos;est ? »</translation>
    </message>
    <message>
        <location/>
        <source>Click this to enter &quot;What&apos;s This?&quot; mode. Your cursor will turn into a question mark. Click on any button, menu choice or area to show a description of what it is.</source>
        <translation>Cliquez sur ce bouton pour entre dans le mode « Qu&apos;est-ce que c&apos;est ? ». Votre curseur se changera en point d&apos;interrogation. Cliquez alors sur n&apos;importe quel bouton, choix de menu ou zone pour voir apparaitre une une description de son utilité.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;About</source>
        <translation>&amp;A propos</translation>
    </message>
    <message>
        <location/>
        <source>Information about Mumble</source>
        <translation>Informations sur Mumble</translation>
    </message>
    <message>
        <location/>
        <source>Shows a small dialog with information and license for Mumble.</source>
        <translation>Affiche une petite fenêtre d&apos;information et la licence de Mumble.</translation>
    </message>
    <message>
        <location/>
        <source>About &amp;Speex</source>
        <translation>À propos de &amp;Speex</translation>
    </message>
    <message>
        <location/>
        <source>Information about Speex</source>
        <translation>Informations sur Speex</translation>
    </message>
    <message>
        <location/>
        <source>Shows a small dialog with information about Speex.</source>
        <translation>Affiche une petite fenêtre d&apos;information sur Speex.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Change Comment</source>
        <translation>Modifier le &amp;commentaire</translation>
    </message>
    <message>
        <source>Change the comment on the selected user.</source>
        <translation type="obsolete">Modifier le commentaire de l&apos;utilisateur sélectionné.</translation>
    </message>
    <message>
        <source>This allows you to change the comment (shown as a tooltip) of a user. Unless you&apos;ve been given extra privileges on the server, you can only change your own comment.</source>
        <translation type="obsolete">Permet de modifier le commentaire (affiche en aide textuelle) de l&apos;utilisateur. Si vous n&apos;avez pas de droits étendus sur le serveur, vous ne pouvez  modifier que votre commentaire.</translation>
    </message>
    <message>
        <source>Certificate Wizard</source>
        <translation type="obsolete">Assistant Certificat</translation>
    </message>
    <message>
        <location/>
        <source>Configure certificates for strong authentication</source>
        <translation>Configure les certificats pour une authentification forte</translation>
    </message>
    <message>
        <location/>
        <source>This starts the wizard for creating, importing and exporting certificates for authentication against servers.</source>
        <translation>Lance l&apos;assistant de création, importation et exportation de certificat pour s&apos;authentifier auprès des serveurs.</translation>
    </message>
    <message>
        <source>Register</source>
        <translation type="obsolete">S&apos;enregistrer</translation>
    </message>
    <message>
        <location/>
        <source>Register user on server</source>
        <translation>Enregistrer l&apos;utilisateur sur le serveur</translation>
    </message>
    <message>
        <location/>
        <source>This will permanently register the user on the server.</source>
        <translation>Enregistrera de manière définitive l&apos;utilisateur sur le serveur.</translation>
    </message>
    <message>
        <location/>
        <source>Add &amp;Friend</source>
        <translation>Ajouter un &amp;ami</translation>
    </message>
    <message>
        <location/>
        <source>Adds a user as your friend.</source>
        <translation>Ajoute un utilisateur à vos amis.</translation>
    </message>
    <message>
        <location/>
        <source>This will add the user as a friend, so you can recognize him on this and other servers.</source>
        <translation>Ajoutera l&apos;utilisateur à vos amis, vous pourrez donc le reconnaitre sur ce serveur et sur les autres serveurs.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Remove Friend</source>
        <translation>Supp&amp;rimer un ami</translation>
    </message>
    <message>
        <location/>
        <source>Removes a user from your friends.</source>
        <translation>Retire un utilisateur de vos amis.</translation>
    </message>
    <message>
        <location/>
        <source>This will remove a user from your friends list.</source>
        <translation>Retirera un utilisateur de votre liste d&apos;amis.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Update Friend</source>
        <translation>Met à &amp;jour votre ami</translation>
    </message>
    <message>
        <location/>
        <source>Update name of your friend.</source>
        <translation>Met à jour le nom de votre ami.</translation>
    </message>
    <message>
        <location/>
        <source>Your friend uses a different name than what is in your database. This will update the name.</source>
        <translation>Votre ami utilise un nom différent de celui que vous avez dans votre base. Ceci mettra à jour son nom.</translation>
    </message>
    <message>
        <location/>
        <source>Registered &amp;Users</source>
        <translation>&amp;Utilisateurs enregistrés</translation>
    </message>
    <message>
        <location/>
        <source>Edit registered users list</source>
        <translation>Édite la liste des utilisateurs enregistrés</translation>
    </message>
    <message>
        <location/>
        <source>This opens the editor for registered users, which allow you to change their name or unregister them.</source>
        <translation>Ouvre l&apos;éditeur des utilisateurs enregistrés, ce qui vous permettra de changer leur nom ou de supprimer leur enregistrement.</translation>
    </message>
    <message>
        <source>Change Texture</source>
        <translation type="obsolete">Modifier la texture</translation>
    </message>
    <message>
        <source>Change your overlay texture on this server</source>
        <translation type="obsolete">Modifier votre texture d&apos;overlay sur ce serveur</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Access Tokens</source>
        <translation>&amp;Jetons d&apos;accès</translation>
    </message>
    <message>
        <location/>
        <source>Add or remove text-based access tokens</source>
        <translation>Ajoute ou retire des jetons d&apos;accès</translation>
    </message>
    <message>
        <source>Remove Texture</source>
        <translation type="obsolete">Retire la texture</translation>
    </message>
    <message>
        <source>Remove currently defined user texture.</source>
        <translation type="obsolete">Supprime votre texture utilisateur.</translation>
    </message>
    <message>
        <source>&amp;About Qt</source>
        <translation type="obsolete">&amp;A propos de Qt</translation>
    </message>
    <message>
        <location/>
        <source>Information about Qt</source>
        <translation>Informations sur Qt</translation>
    </message>
    <message>
        <location/>
        <source>Shows a small dialog with information about Qt.</source>
        <translation>Affiche une petite fenêtre d&apos;information sur Qt.</translation>
    </message>
    <message>
        <location/>
        <source>Check for &amp;Updates</source>
        <translation>Vérifier les mises à jo&amp;ur</translation>
    </message>
    <message>
        <location/>
        <source>Check for new version of Mumble</source>
        <translation>Vérifier les nouvelles versions de Mumble</translation>
    </message>
    <message>
        <location/>
        <source>Connects to the Mumble webpage to check if a new version is available, and notifies you with an appropriate download URL if this is the case.</source>
        <translation>Se connecte au site web de Mumble, vérifie si une nouvelle version est disponible, et vous propose un lien de téléchargement approprié le cas échéant.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="+76"/>
        <location line="+12"/>
        <location line="+2159"/>
        <source>Mumble -- %1</source>
        <translation>Mumble -- %1</translation>
    </message>
    <message>
        <source>Log of messages</source>
        <translation type="obsolete">Journal des messages</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>This shows all recent activity. Connecting to servers, errors and information messages all show up here.&lt;br /&gt;To configure exactly which messages show up here, use the &lt;b&gt;Settings&lt;/b&gt; command from the menu.</source>
        <translation>Affiche toute l&apos;ativité récente. Les connexion aux serveurs, les messages d&apos;erreurs et d&apos;informations sont affichés ici.</translation>
    </message>
    <message>
        <source>&amp;Server</source>
        <translation type="obsolete">&amp;Serveur</translation>
    </message>
    <message>
        <source>&amp;Player</source>
        <translation type="obsolete">&amp;Joueur</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="-2246"/>
        <source>&amp;Channel</source>
        <translation>&amp;Salon</translation>
    </message>
    <message>
        <source>&amp;Audio</source>
        <translation type="obsolete">Au&amp;dio</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>C&amp;onfigure</source>
        <translation>C&amp;onfigurer</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Help</source>
        <translation>&amp;Aide</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="+31"/>
        <source>Push-to-Talk</source>
        <comment>Global Shortcut</comment>
        <translation>Appuyez-pour-parler</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Push and hold this button to send voice.</source>
        <comment>Global Shortcut</comment>
        <translation>Pressez et maintenez ce bouton pour parler.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This configures the push-to-talk button, and as long as you hold this button down, you will transmit voice.</source>
        <comment>Global Shortcut</comment>
        <translation>Configure le bouton Appuyez-pour-parler, et tant que vous presserez ce bouton, votre voix sera transmise.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Reset Audio Processor</source>
        <comment>Global Shortcut</comment>
        <translation>RàZ processeur audio</translation>
    </message>
    <message>
        <source>Toggle Mute Self</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">Activer le mode muet pour soi</translation>
    </message>
    <message>
        <source>Toggle Deafen Self</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">Activer la surdité pour soi</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Unlink Plugin</source>
        <comment>Global Shortcut</comment>
        <translation>Désactiver le plugin</translation>
    </message>
    <message>
        <source>Force Center Position</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">Forcer la position centrale</translation>
    </message>
    <message>
        <source>Chan Parent</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">Canal parent</translation>
    </message>
    <message>
        <source>Chan Sub#%1</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">Sous salon #%1</translation>
    </message>
    <message>
        <source>Chan All Subs</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">Tous les Sous Salon</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Push-to-Mute</source>
        <comment>Global Shortcut</comment>
        <translation>Appuyez-pour-Muet</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Join Channel</source>
        <comment>Global Shortcut</comment>
        <translation>Rejoindre le salon</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Toggle Overlay</source>
        <comment>Global Shortcut</comment>
        <translation>Activer/désactiver l&apos;overlay</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Toggle state of in-game overlay.</source>
        <comment>Global Shortcut</comment>
        <translation>Activer/Désactiver l&apos;affichage de l&apos;overlay.</translation>
    </message>
    <message>
        <source>Alt Push-to-Talk</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">Alternative Push-to-Talk</translation>
    </message>
    <message>
        <source>Kicking player %1</source>
        <translation type="obsolete">Expulsion du joueur %1</translation>
    </message>
    <message>
        <location line="+1095"/>
        <location line="+19"/>
        <source>Enter reason</source>
        <translation>Entrez la raison</translation>
    </message>
    <message>
        <source>Banning player %1</source>
        <translation type="obsolete">Bannissement du joueur %1</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>Mumble</source>
        <translation>Mumble</translation>
    </message>
    <message>
        <source>Channel Name</source>
        <translation type="obsolete">Nom du canal</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="+272"/>
        <source>Are you sure you want to delete %1 and all its sub-channels?</source>
        <translation>Etes-vous sûr(e) de vouloir supprimer %1 ainsi que tous ses sous-salons ?</translation>
    </message>
    <message>
        <location line="+210"/>
        <source>Unmuted and undeafened.</source>
        <translation>N&apos;est pas sourd(e) et muet(te).</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Unmuted.</source>
        <translation>N&apos;est pas muet (te).</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Muted.</source>
        <translation>Muet(te).</translation>
    </message>
    <message>
        <location line="+31"/>
        <source>Muted and deafened.</source>
        <translation>Sourd(e) et muet(te).</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Deafened.</source>
        <translation>Sourd(e).</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Undeafened.</source>
        <translation>N&apos;est pas sourd(e).</translation>
    </message>
    <message>
        <location line="+94"/>
        <source>About Qt</source>
        <translation>À propos de QT</translation>
    </message>
    <message>
        <source>Joining %1.</source>
        <translation type="obsolete">%1 rejoint le serveur.</translation>
    </message>
    <message>
        <source>Connected to server.</source>
        <translation type="obsolete">Connecté au serveur.</translation>
    </message>
    <message>
        <location line="+420"/>
        <source>Server connection failed: %1.</source>
        <translation>La connexion au serveur à échoué : %1.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Disconnected from server.</source>
        <translation>Déconnecté du serveur.</translation>
    </message>
    <message>
        <location line="-1492"/>
        <source>Reconnecting.</source>
        <translation>Reconnexion.</translation>
    </message>
    <message>
        <source>Joined server: %1.</source>
        <translation type="obsolete">%1 a rejoint le serveur.</translation>
    </message>
    <message>
        <source>the server</source>
        <translation type="obsolete">le serveur</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="+323"/>
        <source>You were unmuted and undeafened by %1.</source>
        <translation>%1 vous a rendu la parole et l&apos;ouïe.</translation>
    </message>
    <message>
        <location line="+29"/>
        <source>You muted and deafened %1.</source>
        <translation>Vous avez rendu %1 sourd(e) et muet(te).</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>You unmuted and undeafened %1.</source>
        <translation>Vous avez rendu la parole et l&apos;ouïe à %1.</translation>
    </message>
    <message>
        <location line="+11"/>
        <source>You undeafened %1.</source>
        <translation>Vous avez rendu l&apos;ouïe à %1.</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>You suppressed %1.</source>
        <translation>Vous avez suspendu %1.</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>%1 muted and deafened by %2.</source>
        <translation>%2 a rendu %1 sourd(e) et muet(te).</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1 unmuted and undeafened by %2.</source>
        <translation>%2 a rendu la porole et l&apos;ouïe à %1.</translation>
    </message>
    <message>
        <location line="+18"/>
        <source>%1 suppressed by %2.</source>
        <translation>%2 a suspendu %1.</translation>
    </message>
    <message>
        <location line="+25"/>
        <source>%1 moved to %2.</source>
        <translation>%1 s&apos;est déplacé(e) dans le salon %2.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1 moved to %2 by %3.</source>
        <translation>%1 a été sorti(e) du salon %2 par %3.</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>%1 moved in from %2 by %3.</source>
        <translation>%1 a été déplacé(e) depuis le salon %2 par %3.</translation>
    </message>
    <message>
        <source>Left server: %1.</source>
        <translation type="obsolete">%1 a quitté le serveur.</translation>
    </message>
    <message>
        <location line="-157"/>
        <source>%1 is now muted and deafened.</source>
        <translation>%1 est maintenant sourd(e) et muet(te).</translation>
    </message>
    <message>
        <location line="-114"/>
        <source>You were denied %1 privileges in %2.</source>
        <translation>Le privilège %1 vous est refusé dans %2.</translation>
    </message>
    <message>
        <location line="-69"/>
        <location line="+48"/>
        <source>Welcome message: %1</source>
        <translation>Message de bienvenue : %1</translation>
    </message>
    <message>
        <location line="-15"/>
        <source>Mumble: %1</source>
        <translation>Mumble : %1</translation>
    </message>
    <message>
        <location line="+38"/>
        <source>%3 was denied %1 privileges in %2.</source>
        <translation>%3 vous a refusé le privilège %1 dans %2.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Denied: Cannot modify SuperUser.</source>
        <translation>Refusé: Ne peut modifier SuperUser.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Denied: Invalid channel name.</source>
        <translation>Refusé: Nom de salon invalide.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Denied: Text message too long.</source>
        <translation>Refusé: Message texte trop long.</translation>
    </message>
    <message>
        <location line="+25"/>
        <source>Denied: Operation not permitted in temporary channel.</source>
        <translation>Refusé: Opération non permise dans les salons temporaires.</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>You need a certificate to perform this operation.</source>
        <translation>Vous avez besoin d&apos;un certificat pour effectuer cette action.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1 does not have a certificate.</source>
        <translation>%1 n&apos;a pas de certificat.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Invalid username: %1.</source>
        <translation>Nom d&apos;utilisateur invalide : %1.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Invalid username.</source>
        <translation>Nom d&apos;utilisateur invalide.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Channel is full.</source>
        <translation>Le salon est plein.</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Permission denied.</source>
        <translation>Authorisation refusée.</translation>
    </message>
    <message>
        <location line="+37"/>
        <source>%1 connected.</source>
        <translation>%1 connecté.</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>%1 is now muted.</source>
        <translation>%1 est maintenant muet(te).</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1 is now unmuted.</source>
        <translation>%1 n&apos;est plus muet(te).</translation>
    </message>
    <message>
        <location line="+11"/>
        <source>Recording started</source>
        <translation>Début de l&apos;enregistrement</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Recording stopped</source>
        <translation>Fin de l&apos;enregistrement</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>%1 started recording.</source>
        <translation>%1 a débuté l&apos;enregistrement.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1 stopped recording.</source>
        <translation>%1 a arrêté l&apos;enregistrement.</translation>
    </message>
    <message>
        <location line="+41"/>
        <source>You were unsuppressed.</source>
        <translation>Vous avez été rétabli.</translation>
    </message>
    <message>
        <location line="+96"/>
        <source>%1 is recording</source>
        <translation>%1 enregistre</translation>
    </message>
    <message>
        <location line="+46"/>
        <source>%1 disconnected.</source>
        <translation>%1 déconnecté.</translation>
    </message>
    <message>
        <location line="+91"/>
        <source>(Tree) </source>
        <translation>(Arborescence) </translation>
    </message>
    <message>
        <location line="+2"/>
        <source>(Channel) </source>
        <translation>(Salon) </translation>
    </message>
    <message>
        <location line="+3"/>
        <source>%2%1: %3</source>
        <translation>%2%1 : %3</translation>
    </message>
    <message>
        <source>server</source>
        <translation type="obsolete">serveur</translation>
    </message>
    <message>
        <location line="-254"/>
        <source>You were muted by %1.</source>
        <translation>%1 vous a rendu muet(te).</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>You were suppressed.</source>
        <oldsource>You were suppressed by %1.</oldsource>
        <translation>Vous avez été suspendu.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>You were unsuppressed by %1.</source>
        <translation>Vous avez été rétabli(e) par %1.</translation>
    </message>
    <message>
        <location line="-16"/>
        <source>You were unmuted by %1.</source>
        <translation>%1 vous a rendu le droit à la parole.</translation>
    </message>
    <message>
        <source>You deafened %1.</source>
        <translation type="obsolete">Vous avez rendu sourd %1.</translation>
    </message>
    <message>
        <location line="+29"/>
        <source>You muted %1.</source>
        <translation>Vous avez rendu muet %1.</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>You unsuppressed %1.</source>
        <translation>Vous avez rétabli %1.</translation>
    </message>
    <message>
        <location line="-14"/>
        <source>You unmuted %1.</source>
        <translation>Vous avez rendu la voix à %1.</translation>
    </message>
    <message>
        <location line="+25"/>
        <source>%1 muted by %2.</source>
        <translation>%2 a rendu %1 muet(te).</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>%1 unsuppressed by %2.</source>
        <translation>%1 a été rétabli(e) par %2.</translation>
    </message>
    <message>
        <location line="-14"/>
        <source>%1 unmuted by %2.</source>
        <translation>%1 n&apos;est plus muet (%2).</translation>
    </message>
    <message>
        <source>the server</source>
        <comment>message from</comment>
        <translation type="obsolete">le serveur</translation>
    </message>
    <message>
        <location line="+338"/>
        <source>Unable to find matching CELT codecs with other clients. You will not be able to talk to all users.</source>
        <translation>Incapable de trouver des codecs CELT correspondant aux autres clients. Vous ne serez pas capable de parler aux autres utilisateurs.</translation>
    </message>
    <message>
        <location line="-404"/>
        <source>You were muted and deafened by %1.</source>
        <oldsource>You were deafened by %1.</oldsource>
        <translation>%1 vous a rendu sourd(e) et muet(te).</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>You were undeafened by %1.</source>
        <translation>%1 vous a rendu le droit à la parole et à l&apos;écoute.</translation>
    </message>
    <message>
        <source>%1 deafened by %2.</source>
        <translation type="obsolete">%1 a été rendu sourd(e) et muet(te) par %2.</translation>
    </message>
    <message>
        <location line="+58"/>
        <source>%1 undeafened by %2.</source>
        <translation>%2 a rendu l&apos;ouïe à %1.</translation>
    </message>
    <message>
        <location line="+86"/>
        <source>You were kicked from the server by %1: %2.</source>
        <translation>Vous avez été expulsé(e) du serveur par %1 : %2.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>%3 was kicked from the server by %1: %2.</source>
        <translation>%3 a été expulsé(e) du serveur par %1 : %2.</translation>
    </message>
    <message>
        <location line="-7"/>
        <source>You were kicked and banned from the server by %1: %2.</source>
        <translation>Vous avez été expulsé(e) et banni(e) du serveur par %1 : %2.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>%3 was kicked and banned from the server by %1: %2.</source>
        <translation>%3 a été expulsé(e) et banni(e) du serveur par %1 : %2.</translation>
    </message>
    <message>
        <location line="-60"/>
        <source>You were moved to %1 by %2.</source>
        <translation>Vous avez été déplacé(e) dans le salon %1 par %2.</translation>
    </message>
    <message>
        <source>%1 left channel.</source>
        <translation type="obsolete">%1 a quitté le canal.</translation>
    </message>
    <message>
        <source>%1 moved out by %2.</source>
        <translation type="obsolete">%1 a été sorti(e) du canal par %2.</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>%1 entered channel.</source>
        <translation>%1 est entré(e) dans le salon.</translation>
    </message>
    <message>
        <source>%1 moved in by %2.</source>
        <translation type="obsolete">%1 a été déplacé(e) dans le canal par %2.</translation>
    </message>
    <message>
        <location line="-346"/>
        <source>Server connection rejected: %1.</source>
        <translation>Connexion au serveur rejetée : %1.</translation>
    </message>
    <message>
        <location line="+141"/>
        <source>Denied: %1.</source>
        <translation>Refusé : %1.</translation>
    </message>
    <message>
        <source>Send Messa&amp;ge</source>
        <translation type="obsolete">Envoyer un messa&amp;ge</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>Send a Text Message</source>
        <translation>Envoyer un message textuel</translation>
    </message>
    <message>
        <source>Send a text message to another user.</source>
        <translation type="obsolete">Envoyer un message textuel à un autre utilisateur.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="+476"/>
        <source>Sending message to %1</source>
        <translation>Envoi du message à %1</translation>
    </message>
    <message>
        <source>Enter message</source>
        <translation type="obsolete">Tappez votre message</translation>
    </message>
    <message>
        <location line="+12"/>
        <location line="+84"/>
        <location line="+4"/>
        <location line="+232"/>
        <source>To %1: %2</source>
        <translation>À %1: %2</translation>
    </message>
    <message>
        <location line="-320"/>
        <location line="+88"/>
        <source>Message to %1</source>
        <translation>Message à %1</translation>
    </message>
    <message>
        <location line="-59"/>
        <source>View comment on user %1</source>
        <translation>Voir le commentaire de l&apos;utilisateur %1</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Are you sure you want to reset the comment of user %1?</source>
        <translation>Êtes-vous sûr(e) de vouloir réinitialiser le commentaire de l&apos;utilisateur %1 ?</translation>
    </message>
    <message>
        <location line="+273"/>
        <source>To %1 (Tree): %2</source>
        <oldsource>(Tree) %1: %2</oldsource>
        <translation>A l&apos;arborescence %1 : %2</translation>
    </message>
    <message>
        <location line="+677"/>
        <source>Failed to launch compatibility client</source>
        <translation>Erreur lors du lancement du client de compatibilité</translation>
    </message>
    <message>
        <location line="+21"/>
        <source>Invalid username</source>
        <translation>Nom d&apos;utilisateur invalide</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>You connected with an invalid username, please try another one.</source>
        <translation>Vous vous êtes connectés avec un nom d&apos;utilisateur invalide, veuillez en essayer un autre.</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>That username is already in use, please try another username.</source>
        <translation>Ce nom d&apos;utilisateur est déjà utilisé, veuillez essayer un autre nom d&apos;utilisateur.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Wrong password</source>
        <translation>Mauvais mot de passe</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Wrong password for registered users, please try again.</source>
        <translation>Mauvais mot de passe pour l&apos;utilisateur enregistré, veuillez essayer à nouveau.</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Wrong server password for unregistered user account, please try again.</source>
        <translation>Mauvais mot de passe de serveur pour un utilisateur non enregistré, veuillez essayer à nouveau.</translation>
    </message>
    <message>
        <source>Images (*.png *.jpg *.svg)</source>
        <translation type="obsolete">Images (*.png *.jpg *.svg)</translation>
    </message>
    <message>
        <source>From %1: %2</source>
        <translation type="obsolete">De %1 : %2</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="+353"/>
        <source>Message from %1</source>
        <translation>Message de la part de %1</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>Sends a text message to another user.</source>
        <translation>Envoyer un message textuel à un autre utilisateur.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Audio Wizard</source>
        <translation>Assistant &amp;Audio</translation>
    </message>
    <message>
        <location/>
        <source>Start the audio configuration wizard</source>
        <translation>Démarre l&apos;assistant de configuration audio</translation>
    </message>
    <message>
        <location/>
        <source>This will guide you through the process of configuring your audio hardware.</source>
        <translation>Ceci vous guidera dans la configuration de votre matériel audio.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="-90"/>
        <source>SSL Verification failed: %1</source>
        <translation>Vérification SSL échoué: %1</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>&lt;b&gt;WARNING:&lt;/b&gt; The server presented a certificate that was different from the stored one.</source>
        <translation>&lt;b&gt;ATTENTION :&lt;/b&gt;Le serveur présente un certificat différent de celui mémorisé.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Sever presented a certificate which failed verification.</source>
        <translation>Le serveur présente un certificat qui a une vérification invalide.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>&amp;Information</source>
        <translation>&amp;Information</translation>
    </message>
    <message>
        <location/>
        <source>Show information about the server connection</source>
        <translation>Afficher des informations sur la connexion avec le serveur</translation>
    </message>
    <message>
        <location/>
        <source>This will show extended information about the connection to the server.</source>
        <translation>Ceci affiche des informations supplémentaires sur la connexion au serveur.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="-1700"/>
        <source>Opening URL %1</source>
        <translation>Ouvrir l&apos;URL %1</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>File does not exist</source>
        <translation>Fichier inexistant</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>File is not a configuration file.</source>
        <translation>Le fichier n&apos;est pas un fichier de configuration.</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Settings merged from file.</source>
        <translation>Paramètres fusionnée à partir du fichier.</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>URL scheme is not &apos;mumble&apos;</source>
        <translation>Le schéma d&apos;URL n&apos;est pas &apos;mumble&apos;</translation>
    </message>
    <message>
        <location line="+19"/>
        <source>This version of Mumble can&apos;t handle URLs for Mumble version %1.%2.%3</source>
        <translation>Cette version de Mumble ne peut pas manipuler les URLs pour Mumble version %1.%2.%3</translation>
    </message>
    <message>
        <location line="+29"/>
        <source>Connecting to %1</source>
        <translation>Connexion à %1</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Enter username</source>
        <translation>Entrer le nom d&apos;utilisateur</translation>
    </message>
    <message>
        <location line="+14"/>
        <location line="+176"/>
        <source>Connecting to server %1.</source>
        <translation>Connexion au serveur %1.</translation>
    </message>
    <message>
        <location line="+50"/>
        <source>Change your comment</source>
        <translation>Changer votre commentaire</translation>
    </message>
    <message>
        <location line="+92"/>
        <source>&lt;h2&gt;Version&lt;/h2&gt;&lt;p&gt;Protocol %1.%2.%3.&lt;/p&gt;</source>
        <translation>&lt;h2&gt;Version&lt;/h2&gt;&lt;p&gt;Protocole %1.%2.%3.&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>&lt;p&gt;No build information or OS version available.&lt;/p&gt;</source>
        <translation>&lt;p&gt;Aucune information de version de système disponible.&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>&lt;p&gt;%1 (%2)&lt;br /&gt;%3&lt;/p&gt;</source>
        <translation>&lt;p&gt;%1 (%2)&lt;br /&gt;%3&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+1579"/>
        <source>Images (*.png *.jpg *.jpeg *.svg)</source>
        <translation>Images (*.png *.jpg *.jpeg *.svg)</translation>
    </message>
    <message>
        <source>&lt;h2&gt;Control channel&lt;/h2&gt;&lt;p&gt;Encrypted with %1 bit %2&lt;br /&gt;%3 ms average latency (%4 deviation)&lt;/p&gt;</source>
        <translation type="obsolete">&lt;h2&gt;Canal de contrôle&lt;/h2&gt;&lt;p&gt;Chiffré en %2 %1 bits&lt;br/&gt;%3 ms de latence moyenne (déviation de %4)&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="-1559"/>
        <source>&lt;h2&gt;Voice channel&lt;/h2&gt;&lt;p&gt;Encrypted with 128 bit OCB-AES128&lt;br /&gt;%1 ms average latency (%4 deviation)&lt;/p&gt;</source>
        <translation>&lt;h2&gt;Canal de voix&lt;/h2&gt;&lt;p&gt;Chiffré en OCB-AES128 128 bits&lt;br/&gt;%1 ms de latence moyenne (déviation de %4)&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+11"/>
        <source>&lt;h2&gt;Audio bandwidth&lt;/h2&gt;&lt;p&gt;Maximum %1 kbit/s&lt;br /&gt;Current %2 kbit/s&lt;/p&gt;</source>
        <translation>&lt;h2&gt;Bande passante audio&lt;/h2&gt;&lt;p&gt;Maximale de %1 kbits/s&lt;br/&gt;Actuelle %2 kbits/s&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Mumble Server Information</source>
        <translation>Information sur le serveur Mumble</translation>
    </message>
    <message>
        <location line="+4"/>
        <location line="+1264"/>
        <source>&amp;View Certificate</source>
        <translation>&amp;Voir le certificat</translation>
    </message>
    <message>
        <location line="-1374"/>
        <location line="+327"/>
        <source>Register yourself as %1</source>
        <translation>S&apos;enregistrement en tant que %1</translation>
    </message>
    <message>
        <location line="-327"/>
        <location line="+327"/>
        <source>&lt;p&gt;You are about to register yourself on this server. This action cannot be undone, and your username cannot be changed once this is done. You will forever be known as &apos;%1&apos; on this server.&lt;/p&gt;&lt;p&gt;Are you sure you want to register yourself?&lt;/p&gt;</source>
        <translation>&lt;p&gt;Vous êtes sur le point de vous enregistrer sur ce serveur. Cette action ne peut pas être annulée, et votre identifiant ne pourra plus être modifié une fois cette étape effectuée. Vous serez pour toujours connu comme &apos;%1&apos; sur ce serveur&lt;/p&gt;&lt;p&gt;Êtes-vous sûr de vouloir vous enregistrer ?&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Register user %1</source>
        <translation>Enregistrer l&apos;utilisateur %1</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>&lt;p&gt;You are about to register %1 on the server. This action cannot be undone, the username cannot be changed, and as a registered user, %1 will have access to the server even if you change the server password.&lt;/p&gt;&lt;p&gt;From this point on, %1 will be authenticated with the certificate currently in use.&lt;/p&gt;&lt;p&gt;Are you sure you want to register %1?&lt;/p&gt;</source>
        <translation>&lt;p&gt;Vous êtes sur le point d&apos;enregistrer %1 sur le serveur. Cette action est irréversible, le nom d&apos;utilisateur ne pourra pas être modifié, et %1 aura accès au serveur même si vous changez le mot de passe du serveur.&lt;/p&gt;&lt;p&gt;À partir de ce points, %1 sera authentifié avec le certificat actuellement utilisé.&lt;/p&gt;&lt;p&gt;Êtes-vous sûr de vouloir l&apos;enregistrer %1 ?&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+40"/>
        <source>Kicking user %1</source>
        <translation>Expulsion de l&apos;utilisateur %1</translation>
    </message>
    <message>
        <location line="+19"/>
        <source>Banning user %1</source>
        <translation>Bannissement de l&apos;utilisateur %1</translation>
    </message>
    <message>
        <source>Change comment on user %1</source>
        <translation type="obsolete">Modifier le commentaire de l&apos;utilisateur %1</translation>
    </message>
    <message>
        <source>To channel %1: %2</source>
        <translation type="obsolete">Du salon %1: %2</translation>
    </message>
    <message>
        <location line="+118"/>
        <location line="+236"/>
        <source>Message to channel %1</source>
        <translation>Message pour le salon %1</translation>
    </message>
    <message>
        <location line="+490"/>
        <source>Connected.</source>
        <translation>Connecté.</translation>
    </message>
    <message>
        <location line="+159"/>
        <source>SSL Version mismatch</source>
        <translation>Incompatibilité de version SSL</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>This server is using an older encryption standard. It might be an older 1.1 based Mumble server.&lt;br /&gt;Would you like to launch the compatibility client to connect to it?</source>
        <translation>Ce serveur utilse un vieux standard de chiffrage. Ce doit être un serveur Mumble basé sur la version 1.1&lt;br/&gt;Voulez-vous lancer le client de compatibilité pour vous y connecter ?</translation>
    </message>
    <message>
        <source>Failed to launch compatibilty client</source>
        <translation type="obsolete">Impossible de lancer le client de compatibilité</translation>
    </message>
    <message>
        <location line="+26"/>
        <source>The compatibility client could not be found, or failed to start.&lt;br /&gt;Note that the compatibility client is an optional component for most installations, and might not be installed.</source>
        <translation>Le client de compatibilité n&apos;a pas pu être trouvé, ou n&apos;a pas pu être lancé.&lt;br/&gt;Celui-ci étant un composant optionnel pour la majorité des installations, il peut ne pas être installé.</translation>
    </message>
    <message>
        <location line="-50"/>
        <source>&lt;p&gt;%1.&lt;br /&gt;The specific errors with this certificate are: &lt;/p&gt;&lt;ol&gt;%2&lt;/ol&gt;&lt;p&gt;Do you wish to accept this certificate anyway?&lt;br /&gt;(It will also be stored so you won&apos;t be asked this again.)&lt;/p&gt;</source>
        <translation>&lt;p&gt;%1.&lt;br /&gt;Les erreurs de ce certificat sont les suivantes: &lt;/p&gt;&lt;ol&gt;%2&lt;/ol&gt;&lt;p&gt;Voulez-vous quand même accepter ce certificat ?&lt;br /&gt;(Il sera également stocké et ne vous sera pas redemandé.)&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+177"/>
        <source>&lt;center&gt;Type message to channel &apos;%1&apos; here&lt;/center&gt;</source>
        <oldsource>Type message to channel &apos;%1&apos; here</oldsource>
        <translation>&lt;center&gt;Saisir un message pour le salon &apos;%1&apos; ici&lt;/center&gt;</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>&lt;center&gt;Type message to user &apos;%1&apos; here&lt;/center&gt;</source>
        <oldsource>Type message to user &apos;%1&apos; here</oldsource>
        <translation>&lt;center&gt;Saisir un message pour l&apos;utilisateur &apos;%1&apos; ici&lt;/center&gt;</translation>
    </message>
    <message>
        <location line="+105"/>
        <source>Choose image file</source>
        <translation>Choisir un fichier image</translation>
    </message>
    <message>
        <source>Images (*.png *.jpg)</source>
        <translation type="obsolete">Image (*.png *.jpg)</translation>
    </message>
    <message>
        <location line="+7"/>
        <location line="+17"/>
        <source>Failed to load image</source>
        <translation>Impossible de charger l&apos;image</translation>
    </message>
    <message>
        <location line="-17"/>
        <source>Could not open file for reading.</source>
        <translation>N&apos;a pas pu ouvrir le fichier en lecture.</translation>
    </message>
    <message>
        <location line="+17"/>
        <source>Image format not recognized.</source>
        <translation>Format d&apos;image non reconnu.</translation>
    </message>
    <message>
        <location line="-1585"/>
        <source>Voice channel is sent over control channel.</source>
        <translation>La voix est envoyée au dessus du canal de contrôle.</translation>
    </message>
    <message>
        <location line="-816"/>
        <source>This will switch the states of the in-game overlay.</source>
        <comment>Global Shortcut</comment>
        <translation>Ceci changera l&apos;état de l&apos;overlay.</translation>
    </message>
    <message>
        <location line="+807"/>
        <source>&lt;h2&gt;Control channel&lt;/h2&gt;&lt;p&gt;Encrypted with %1 bit %2&lt;br /&gt;%3 ms average latency (%4 deviation)&lt;/p&gt;&lt;p&gt;Remote host %5 (port %6)&lt;/p&gt;</source>
        <translation>&lt;h2&gt;Canal de contrôle&lt;/h2&gt;&lt;p&gt;Chiffré en %2 %1 bits&lt;br/&gt;%3 ms de latence moyenne (déviation de %4)&lt;/p&gt;&lt;p&gt;Hôte distant %5 (port %6)&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+18"/>
        <source>UDP Statistics</source>
        <translation>Statistiques UDP</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>To Server</source>
        <translation>Au serveur</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>From Server</source>
        <translation>Du serveur</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Good</source>
        <translation>Bon</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Late</source>
        <translation>Tardif</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Lost</source>
        <translation>Perdu</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Resync</source>
        <translation>Resynchronise</translation>
    </message>
    <message>
        <location line="+631"/>
        <source>Sending message to channel %1</source>
        <translation>Envoyé un message au salon %1</translation>
    </message>
    <message>
        <source>Sending message to channel tree %1</source>
        <translation type="obsolete">Envoyé un message à l&apos;arborescence du canal %1</translation>
    </message>
    <message>
        <source>To tree %1: %2</source>
        <translation type="obsolete">A l&apos;arborescence %1: %2</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Message to tree %1</source>
        <translation>Message à l&apos;arborescence %1</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>Ctrl+Q</source>
        <translation>Ctrl+Q</translation>
    </message>
    <message>
        <source>Mute player locally</source>
        <translation type="obsolete">Ne plus écouter le joueur localement</translation>
    </message>
    <message>
        <source>Mute or unmute player locally. Use this on other players in the same room.</source>
        <translation type="obsolete">Rendre muet ou rendre la parole à un joueur localement. A utiliser sur les autres joueurs dans le même salon.</translation>
    </message>
    <message>
        <source>&amp;Rename Channel</source>
        <translation type="obsolete">&amp;Renommez canal</translation>
    </message>
    <message>
        <source>Renames the channel</source>
        <translation type="obsolete">Renomme le canal</translation>
    </message>
    <message>
        <source>This renames a channel.</source>
        <translation type="obsolete">Ceci renomme le canal.</translation>
    </message>
    <message>
        <source>Silence loudspeakers</source>
        <translation type="obsolete">Silence haut-parleurs</translation>
    </message>
    <message>
        <location/>
        <source>About &amp;Qt</source>
        <translation>À propos de &amp;Qt</translation>
    </message>
    <message>
        <location/>
        <source>Sends a text message to all users in a channel.</source>
        <translation>Envoyer un message texte à tous les utilisateurs dans le salon.</translation>
    </message>
    <message>
        <source>Send &amp;Tree Message</source>
        <translation type="obsolete">Envoyer un message à l&apos;&amp;Arborescence</translation>
    </message>
    <message>
        <source>Send message to channel tree.</source>
        <translation type="obsolete">Envoyer un message à l&apos;arborescence du canal.</translation>
    </message>
    <message>
        <source>This sends a text message to a channel and it&apos;s subchannels.</source>
        <translation type="obsolete">Ceci envoi un message au canal et c&apos;est sous-canaux.</translation>
    </message>
    <message>
        <source>Server maximum bandwidth is only %1 kbit/s. Quality auto-adjusted.</source>
        <translation type="obsolete">La bande passante du serveur est de maximum %1 kbit/s. La qualité audio doit être réajusté.</translation>
    </message>
    <message>
        <source>This is the first time you&apos;re starting Mumble.&lt;br /&gt;Would you like to go through the Audio Wizard to configure your soundcard?</source>
        <translation type="obsolete">C&apos;est la première fois que vous lancez Mumble.&lt;br /&gt; Souhaitez-vous passer par l&apos;Assistant audio pour configurer votre carte son ?</translation>
    </message>
    <message>
        <source>&lt;h2&gt;Audio bandwidth&lt;/h2&gt;&lt;p&gt;Maximum %1 kbit/s&lt;br /&gt;Current %2 kbit/s (Quality %3)&lt;/p&gt;</source>
        <translation type="obsolete">&lt;h2&gt;Bande passante audio&lt;/h2&gt;&lt;p&gt;Maximum %1 kbit/s&lt;br /&gt;Actuel %2 kbit/s (Qualité %3)&lt;/p&gt;</translation>
    </message>
    <message>
        <source>Connected to server %1.</source>
        <translation type="obsolete">Connecté au serveur %1.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="-1310"/>
        <source>Mumble is currently connected to a server. Do you want to Close or Minimize it?</source>
        <translation>Mumble est actuellement connecté à un serveur. Voulez-vous le fermer ou le réduire dans la barre des tâches?</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Close</source>
        <translation>Fermer</translation>
    </message>
    <message>
        <location line="-125"/>
        <location line="+126"/>
        <source>Minimize</source>
        <translation>Réduire</translation>
    </message>
    <message>
        <location line="-179"/>
        <source>Mute Self</source>
        <comment>Global Shortcut</comment>
        <translation>Se rendre muet(te)</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Set self-mute status.</source>
        <comment>Global Shortcut</comment>
        <translation>Activer le status muet pour soi même.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This will set or toggle your muted status. If you turn this off, you will also disable self-deafen.</source>
        <comment>Global Shortcut</comment>
        <translation>Activera ou désactivera votre status muet. Si vous vous rendez la voix, vous entendrez aussi.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Deafen Self</source>
        <comment>Global Shortcut</comment>
        <translation>Se rendre sourd(e)</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Set self-deafen status.</source>
        <comment>Global Shortcut</comment>
        <translation>Activer le status sourd pour soi même.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This will set or toggle your deafened status. If you turn this on, you will also enable self-mute.</source>
        <comment>Global Shortcut</comment>
        <translation>Se rendre sourd(e) ou non. Vous rendre sourd(e) vous rendra aussi muet(te).</translation>
    </message>
    <message>
        <source>This will switch the states of the in-game overlay between showing everybody, just the users who are talking, and nobody.</source>
        <comment>Global Shortcut</comment>
        <translation type="obsolete">Bascule l&apos;affichage de l&apos;overlay en jeux entre afficher tout le monde, seulement les utilisateurs parlant ou personne.</translation>
    </message>
    <message>
        <location line="+30"/>
        <source>Whisper/Shout</source>
        <oldsource>Whisper</oldsource>
        <translation>Chuchoter/Crier</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>&amp;Window</source>
        <translation>&amp;Fenêtre</translation>
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
        <translation>Active/désactive l&apos;interface minimale</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>&amp;Minimal View</source>
        <translation>Vue &amp;minimale</translation>
    </message>
    <message>
        <location/>
        <source>Toggle minimal window modes</source>
        <translation>Basculer l&apos;interface en mode minimal ou normal</translation>
    </message>
    <message>
        <location/>
        <source>This will toggle minimal mode, where the log window and menu is hidden.</source>
        <translation>Cela basculer en mode minimal, où la fenêtre et le menu est caché.</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="+3"/>
        <source>Volume Up (+10%)</source>
        <comment>Global Shortcut</comment>
        <translation>Augmenter volume (+10%)</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Volume Down (-10%)</source>
        <comment>Global Shortcut</comment>
        <translation>Diminuer le volume (-10%)</translation>
    </message>
    <message>
        <location line="+65"/>
        <location line="+2189"/>
        <source>&lt;center&gt;Not connected&lt;/center&gt;</source>
        <oldsource>Not connected</oldsource>
        <translation>&lt;center&gt;Non connecté&lt;/center&gt;</translation>
    </message>
    <message>
        <location line="-1905"/>
        <source>Clear</source>
        <translation>Effacer</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>Log</source>
        <translation>Historique</translation>
    </message>
    <message>
        <location filename="UserModel.cpp" line="+1381"/>
        <source>You have Channel Dragging set to &quot;Do Nothing&quot; so the channel wasn&apos;t moved.</source>
        <translation>Votre option &apos;Faire glisser le salon&apos; est définie sur &quot;Ne rien faire&quot;, le salon n&apos;a donc pas été déplacé.</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Unknown Channel Drag mode in UserModel::dropMimeData.</source>
        <translation>Mode de déplacement du salon inconnu dans UserModel::dropMimeData.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>&amp;Unlink</source>
        <comment>Channel</comment>
        <translation>&amp;Délier</translation>
    </message>
    <message>
        <source>&amp;Unlink</source>
        <comment>Plugin</comment>
        <translation type="obsolete">&amp;Délier</translation>
    </message>
    <message>
        <location filename="Messages.cpp" line="-10"/>
        <location line="+1"/>
        <source>Server</source>
        <comment>message from</comment>
        <translation>Serveur</translation>
    </message>
    <message>
        <source>&amp;Quit</source>
        <translation type="obsolete">&amp;Quitter</translation>
    </message>
    <message>
        <location filename="MainWindow.ui"/>
        <source>Hide Frame</source>
        <translation>Cacher la bordure</translation>
    </message>
    <message>
        <location/>
        <source>Toggle showing frame on minimal window</source>
        <translation>Affiche/masque la bordure sur la fenêtre minimale</translation>
    </message>
    <message>
        <location/>
        <source>This will toggle whether the minimal window should have a frame for moving and resizing.</source>
        <translation>Permet de choisir si l&apos;affichage minimale doit avoir une bordure pour déplacer ou redimensionner la fenêtre.</translation>
    </message>
    <message>
        <location/>
        <source>Send &amp;Message</source>
        <translation>Envoyer le &amp;message</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Unlink All</source>
        <translation>Délier to&amp;us</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Certificate Wizard</source>
        <translation>Assistant &amp;certificat</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Register</source>
        <translation>S&apos;en&amp;registrer</translation>
    </message>
    <message>
        <location/>
        <source>Reset &amp;Comment</source>
        <translation>Réinitialiser le &amp;commentaire</translation>
    </message>
    <message>
        <location/>
        <source>Reset the comment of the selected user.</source>
        <translation>Réinitialiser le commentaire de l&apos;utilisateur sélectionné.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Join Channel</source>
        <translation>Re&amp;joindre le salon</translation>
    </message>
    <message>
        <location/>
        <source>View Comment</source>
        <translation>Voir commentaire</translation>
    </message>
    <message>
        <location/>
        <source>View comment in editor</source>
        <translation>Voir le commentaire dans l&apos;éditeur</translation>
    </message>
    <message>
        <location/>
        <source>Query server for connection information for user</source>
        <translation>Demander au serveur les informations de connexion pour l&apos;utilisateur</translation>
    </message>
    <message>
        <location/>
        <source>S&amp;erver</source>
        <translation>S&amp;erveur</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Self</source>
        <translation>&amp;Soi</translation>
    </message>
    <message>
        <location/>
        <source>Audio S&amp;tatistics</source>
        <translation>S&amp;tatistiques audio</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Unlink Plugins</source>
        <translation>Délier les pl&amp;ugins</translation>
    </message>
    <message>
        <location/>
        <source>R&amp;egister</source>
        <translation>&amp;Enregistrer</translation>
    </message>
    <message>
        <location/>
        <source>Register yourself on the server</source>
        <translation>S&apos;enregistrer sur le serveur</translation>
    </message>
    <message>
        <location/>
        <source>Change &amp;Avatar</source>
        <translation>Changer l&apos;&amp;avatar</translation>
    </message>
    <message>
        <location/>
        <source>Change your avatar image on this server</source>
        <translation>Change l&apos;avatar sur ce serveur</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Remove Avatar</source>
        <translation>Supp&amp;rimer l&apos;avatar</translation>
    </message>
    <message>
        <location/>
        <source>Remove currently defined avatar image.</source>
        <translation>Supprime l&apos;avatar défini actuellement.</translation>
    </message>
    <message>
        <location/>
        <source>Icon Toolbar</source>
        <translation>Icône de la barre d&apos;outils</translation>
    </message>
    <message>
        <location/>
        <source>Change your own comment</source>
        <translation>Modifier votre commentaire</translation>
    </message>
    <message>
        <location/>
        <source>Recording</source>
        <translation>Enregistrement</translation>
    </message>
    <message>
        <location/>
        <source>Priority Speaker</source>
        <translation>Parole prioritaire</translation>
    </message>
</context>
<context>
    <name>NetworkConfig</name>
    <message>
        <location filename="NetworkConfig.cpp" line="+53"/>
        <source>Network</source>
        <translation>Réseau</translation>
    </message>
    <message>
        <location line="+36"/>
        <source>Updates are mandatory when using snapshot releases.</source>
        <translation>Les mises à jour sont obligatoires lors de l&apos;utilisation d&apos;une version snapshot.</translation>
    </message>
    <message>
        <source>Form</source>
        <translation type="obsolete">Form</translation>
    </message>
    <message>
        <location filename="NetworkConfig.ui"/>
        <source>Connection</source>
        <translation>Connexion</translation>
    </message>
    <message>
        <location/>
        <source>Use TCP compatibility mode</source>
        <oldsource>Use TCP compatability mode</oldsource>
        <translation>Utiliser le mode de compatibilité TCP</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Enable TCP compatibility mode&lt;/b&gt;.&lt;br /&gt;This will make Mumble use only TCP when communicating with the server. This will increase overhead and cause lost packets to produce noticeable pauses in communication, so this should only be used if you are unable to use the default (which uses UDP for voice and TCP for control).</source>
        <translation>&lt;b&gt;Activer le mode de compatibilité TCP&lt;/b&gt;.&lt;br /&gt;Cela permet d&apos;utiliser Mumble uniquement avec le protocole TCP pour communiquer avec le serveur. Cette initiative augmente l&apos;utilisation de la bande passante, et causer la perte de paquets  produira ainsi des pauses dans la communication. Donc ceci ne devrait être utilisée que si vous ne pouvez pas utiliser les paramètres par défaut (qui utilise pour la voix UDP et TCP pour le contrôle).</translation>
    </message>
    <message>
        <source>Use TCP mode</source>
        <translation type="obsolete">Utilise le mode TCP</translation>
    </message>
    <message>
        <location/>
        <source>Reconnect when disconnected</source>
        <translation>Reconnexion quand vous êtes déconnecté</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Reconnect when disconnected&lt;/b&gt;.&lt;br /&gt;This will make Mumble try to automatically reconnect after 10 seconds if your server connection fails.</source>
        <translation>&lt;b&gt;Se reconnecter quand vous êtes déconnecté&lt;/b&gt;.&lt;br /&gt;Mumble essaiera automatiquement de se reconnecter après dix secondes si la connexion échoue.</translation>
    </message>
    <message>
        <location/>
        <source>Reconnect automatically</source>
        <translation>Reconnexion automatique</translation>
    </message>
    <message>
        <location/>
        <source>Proxy</source>
        <translation>Proxy</translation>
    </message>
    <message>
        <location/>
        <source>Type</source>
        <translation>Type</translation>
    </message>
    <message>
        <location/>
        <source>Type of proxy to connect through</source>
        <translation>Type de proxy a utiliser</translation>
    </message>
    <message>
        <location/>
        <source>Direct connection</source>
        <translation>Connexion directe</translation>
    </message>
    <message>
        <location/>
        <source>HTTP(S) proxy</source>
        <translation>Proxy HTTP(S)</translation>
    </message>
    <message>
        <location/>
        <source>SOCKS5 proxy</source>
        <translation>Proxy SOCKS5</translation>
    </message>
    <message>
        <location/>
        <source>Hostname</source>
        <translation>Nom d&apos;hôte</translation>
    </message>
    <message>
        <location/>
        <source>Hostname of the proxy</source>
        <translation>Nom d&apos;hôte du proxy</translation>
    </message>
    <message>
        <source>&lt;b&gt;Hostname of the proxy.&lt;b&gt;&lt;br /&gt;This field specifies the hostname of the proxy you wish to tunnel network traffic through.</source>
        <translation type="obsolete">&lt;b&gt;Nom d&apos;hôte du proxy.&lt;b&gt;&lt;br /&gt;Ce champ spécifie le nom d&apos;hôte du proxy que vous souhaitez utiliser.</translation>
    </message>
    <message>
        <location/>
        <source>Force TCP mode</source>
        <translation>Forcer le mode TCP</translation>
    </message>
    <message>
        <location/>
        <source>Enable QoS to prioritize packets</source>
        <translation>Activer la Qualité de Service pour la priorité des paquets</translation>
    </message>
    <message>
        <location/>
        <source>This will enable QoS, which will attempt to prioritize voice packets over other traffic.</source>
        <translation>Active la Qualité de Service, qui tend à augmenter la priorité des paquets vocaux par rapport à d&apos;autres traffics.</translation>
    </message>
    <message>
        <location/>
        <source>Use Quality of Service</source>
        <translation>Utiliser la Qualité de Service</translation>
    </message>
    <message>
        <location/>
        <source>Don&apos;t send certificate to server and don&apos;t save passwords. (Not saved).</source>
        <translation>N&apos;envoie pas de certificat au serveur et ne sauve pas les mots de passe. (Option non enregistrée).</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This will suppress identity information from the client.&lt;/b&gt;&lt;p&gt;The client will not identify itself with a certificate, even if defined, and will not cache passwords for connections. This is primarily a test-option and is not saved.&lt;/p&gt;</source>
        <translation>&lt;b&gt;Désactive les informations d&apos;identification du client.&lt;/b&gt;&lt;p&gt;Le client ne va pas s&apos;identifier à l&apos;aide d&apos;un certificat, même si celui-ci existe, et il ne mémorisera pas les mots de passes de la connexion. Cette option est principalement utilisée à des fins de tests et n&apos;est pas mémorisée.&lt;/p&gt;</translation>
    </message>
    <message>
        <location/>
        <source>Suppress certificate and password storage</source>
        <translation>Désactiver l&apos;usage du certificat et mots de passes</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Type of proxy to connect through.&lt;/b&gt;&lt;br /&gt;This makes Mumble connect through a proxy for all outgoing connections. Note: Proxy tunneling forces Mumble into TCP compatibility mode, causing all voice data to be sent via the control channel.</source>
        <translation>&lt;b&gt;Type de connexion via le proxy.&lt;/b&gt;&lt;br/&gt;Permet à Mumble de se connecter au travers d&apos;un proxy pour toutes les connexions sortantes. Notez que l&apos;usage du proxy force Mumble à utiliser le mode de compatibilité TCP, la voix est donc envoyée au dessus du canal de contrôle.</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Hostname of the proxy.&lt;/b&gt;&lt;br /&gt;This field specifies the hostname of the proxy you wish to tunnel network traffic through.</source>
        <translation>&lt;b&gt;Nom d&apos;hôte du proxy.&lt;/b&gt;&lt;br/&gt;Ce champ identifie le nom d&apos;hôte du proxy que vous souhiatez utiliser.</translation>
    </message>
    <message>
        <location/>
        <source>Port</source>
        <translation>Port</translation>
    </message>
    <message>
        <location/>
        <source>Port number of the proxy</source>
        <translation>Numéro de port du proxy</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Port number of the proxy.&lt;/b&gt;&lt;br /&gt;This field specifies the port number that the proxy expects connections on.</source>
        <translation>&lt;b&gt;Numéro du port du proxy.&lt;/b&gt;&lt;br /&gt;Ce champ spécifie le numéro de port du serveur proxy sur lequel il écoute.</translation>
    </message>
    <message>
        <location/>
        <source>Username</source>
        <translation>Nom d&apos;utilisateur</translation>
    </message>
    <message>
        <location/>
        <source>Username for proxy authentication</source>
        <translation>Nom d&apos;utilisateur à utiliser pour l&apos;authentification</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Username for proxy authentication.&lt;/b&gt;&lt;br /&gt;This specifies the username you use for authenticating yourself with the proxy. In case the proxy does not use authentication, or you want to connect anonymously, simply leave this field blank.</source>
        <translation>&lt;b&gt;Nom d&apos;utilisateur à utiliser pour l&apos;authentification sur le proxy.&lt;/b&gt;&lt;br /&gt;Dans le cas ou le proxy n&apos;utilise pas d&apos;authentification, ou si vous voulez vous connecté en tant qu&apos;anonyme, laissez simplement le champ vide.</translation>
    </message>
    <message>
        <location/>
        <source>Password</source>
        <translation>Mot de passe</translation>
    </message>
    <message>
        <location/>
        <source>Password for proxy authentication</source>
        <translation>Mot de passe pour l&apos;authentification sur le proxy</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Password for proxy authentication.&lt;/b&gt;&lt;br /&gt;This specifies the password you use for authenticating yourself with the proxy. In case the proxy does not use authentication, or you want to connect anonymously, simply leave this field blank.</source>
        <translation>&lt;b&gt;Mot de passe à utiliser pour l&apos;authentification sur le proxy .&lt;/b&gt;&lt;br /&gt;Dans le cas ou le proxy n&apos;utilise pas d&apos;authentification, ou si vous voulez vous connecté en tant qu&apos;anonyme, laissez simplement le champ vide.</translation>
    </message>
    <message>
        <location/>
        <source>Misc</source>
        <translation>Divers</translation>
    </message>
    <message>
        <location/>
        <source>Prevent log from downloading images</source>
        <translation>Empêche Mumble de télécharger les images</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Disable image download&lt;/b&gt;&lt;br/&gt;
Prevents the client from downloading images embedded into chat messages with the img tag.</source>
        <translation>&lt;b&gt;Désactiver le téléchargement des images&lt;/b&gt;&lt;br/&gt;
Empêche le client de télécharger les images intégrées dans les messages de chat avec la balise img.</translation>
    </message>
    <message>
        <location/>
        <source>Disable image download</source>
        <translation>Désactiver le téléchargement des images</translation>
    </message>
    <message>
        <location/>
        <source>Mumble services</source>
        <translation>Services de Mumble</translation>
    </message>
    <message>
        <location/>
        <source>Check for new releases of Mumble automatically.</source>
        <translation>Vérifier les nouvelles versions de Mumble automatiquement.</translation>
    </message>
    <message>
        <location/>
        <source>This will check for new releases of Mumble every time you start the program, and notify you if one is available.</source>
        <translation>Vérifiera l&apos;existance de nouvelles version de Mumble à chaque lancement de celui-ci, et vous notifiera si elle est disponible.</translation>
    </message>
    <message>
        <location/>
        <source>Check for application updates on startup</source>
        <translation>Vérifier les nouvelles versions au démarrage</translation>
    </message>
    <message>
        <location/>
        <source>Check for new releases of plugins automatically.</source>
        <translation>Vérifie automatiquement les nouvelles versions des plugins au démarrage.</translation>
    </message>
    <message>
        <location/>
        <source>This will check for new releases of plugins every time you start the program, and download them automatically.</source>
        <translation>Vérifiera l&apos;existance de nouvelle version des plugins à chaque lancement de l&apos;application, et les téléchargera automatiquement.</translation>
    </message>
    <message>
        <source>Download plugin updates on startup</source>
        <translation type="obsolete">Télécharger les mises à jour des plugins au démarrage</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Submit anonymous statistics.&lt;/b&gt;&lt;br /&gt;Mumble has a small development team, and as such needs to focus its development where it is needed most. By submitting a bit of statistics you help the project determine where to focus development.</source>
        <translation>&lt;b&gt;Envoyer les statistiques anonymes.&lt;/b&gt;&lt;br/&gt;Mumble dispose d&apos;une petite équipe de développement, et comme tel, doit concentrer son développement où celui-ci est le plus nécessaire. En envoyant quelques statistiques, vous aidez le projet à déterminer où le développement doit se focaliser.</translation>
    </message>
    <message>
        <source>Check for updates on startup</source>
        <translation type="obsolete">Vérifier les mises a jour au démarrage</translation>
    </message>
    <message>
        <location/>
        <source>Submit anonymous statistics to the Mumble project</source>
        <translation>Soumettre des statistique anonyme au projet Mumble</translation>
    </message>
    <message>
        <location/>
        <source>Submit anonymous statistics</source>
        <translation>Envoyer des statistiques anonymes</translation>
    </message>
    <message>
        <location/>
        <source>Reconnect to last used server when starting Mumble</source>
        <translation>Se reconnecter au dernier serveur utilisé au lancement de Mumble</translation>
    </message>
    <message>
        <location/>
        <source>Reconnect to last server on startup</source>
        <translation>Se reconnecter au dernier serveur utilisé au démarrage</translation>
    </message>
    <message>
        <location/>
        <source>Download plugin and overlay updates on startup</source>
        <translation>Télécharger les mises à jour des plugins et de l&apos;overlay au démarrage</translation>
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
        <source>Device selection</source>
        <translation type="obsolete">Sélectionner un périphérique</translation>
    </message>
    <message>
        <source>Input</source>
        <translation type="obsolete">Entrée</translation>
    </message>
    <message>
        <source>Device to use for microphone</source>
        <translation type="obsolete">Périphérique à utiliser pour le microphone</translation>
    </message>
    <message>
        <source>This set which device mumble should use. The &lt;i&gt;default&lt;/i&gt; device is whatever you have configured in alsaconfig, the &lt;i&gt;hwplug&lt;/i&gt; devices are specific hardware devices backed by the ALSA mixer and the &lt;i&gt;hw&lt;/i&gt; devices are raw hardware access. Unless your soundcard supports hardware mixing of audio, using the &lt;i&gt;hw&lt;/i&gt; device will exclude all other programs from using audio.</source>
        <translation type="obsolete">Spécifie quel périphérique devrait être utilisé par mumble. Le périphérique &lt;i&gt;default&lt;/i&gt; est ce que vous avez configuré dans alsaconfig, les périphériques &lt;i&gt;hwplug&lt;/i&gt; sont les périphériques accessible par le mixer d&apos;ALSA et les périphériques &lt;i&gt;hw&lt;/i&gt; sont les adresses brut du matériel. À moins que votre carte son ne supporte le mixage matériel, en utilisant les périphériques &lt;i&gt;hw&lt;/i&gt;, vous excluerez tout autre programme utilisant l&apos;audio..</translation>
    </message>
    <message>
        <source>Output</source>
        <translation type="obsolete">Sortie</translation>
    </message>
    <message>
        <source>Device to use for speakers/headphones</source>
        <translation type="obsolete">Périphérique a utiliser pour les enceintes/casques</translation>
    </message>
    <message>
        <source>Output Options</source>
        <translation type="obsolete">Options de sortie</translation>
    </message>
    <message>
        <source>Output Delay</source>
        <translation type="obsolete">Retard de sortie</translation>
    </message>
    <message>
        <source>Amount of data to buffer for OSS</source>
        <translation type="obsolete">Quantité de données du tampon pour OSS</translation>
    </message>
    <message>
        <source>This sets the amount of data to prebuffer in the output buffer. Experiment with different values and set it to the lowest which doesn&apos;t cause rapid jitter in the sound.</source>
        <translation type="obsolete">Spécifie la quantité de données à mettre en cache dans le tampon de sortie. Essayez différentes valeurs et mettez la plus basse qui ne cause pas de rapides décalages du son.</translation>
    </message>
    <message>
        <source>This set which device mumble should use. The &lt;i&gt;default&lt;/i&gt; device is your primary sound device.</source>
        <translation type="obsolete">Ceci spécifie quel périphérique est utilisé dans mumble. Le périphérique par &lt;i&gt;defaut&lt;/i&gt; est votre carte son principale.</translation>
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
        <translation>Silenceux</translation>
    </message>
    <message>
        <location line="+3"/>
        <location filename="OverlayUser.cpp" line="+3"/>
        <source>Talking</source>
        <translation>Parle</translation>
    </message>
    <message>
        <location line="+3"/>
        <location filename="OverlayUser.cpp" line="+3"/>
        <source>Whisper</source>
        <translation>Chuchote</translation>
    </message>
    <message>
        <location line="+3"/>
        <location filename="OverlayUser.cpp" line="+3"/>
        <source>Shout</source>
        <translation>Crie</translation>
    </message>
    <message>
        <location line="+17"/>
        <location filename="OverlayUser.cpp" line="-110"/>
        <source>Channel</source>
        <translation>Salon</translation>
    </message>
    <message>
        <location filename="Overlay.cpp" line="+100"/>
        <source>Failed to create communication with overlay at %2: %1. No overlay will be available.</source>
        <translation>Échec lors de la création de la communication avec l&apos;overlay à %2 : %1. L&apos;overlay ne sera pas disponible.</translation>
    </message>
    <message>
        <location line="+255"/>
        <source>Skipping overlay update in debug mode.</source>
        <translation>Passe la mise à jour de l&apos;overlay en mode débogage.</translation>
    </message>
    <message>
        <location line="+66"/>
        <source>Downloaded new or updated overlay support file to %1.</source>
        <translation>Nouveau plugin téléchargé ou mis à jour vers %1.</translation>
    </message>
    <message>
        <source>Failed to load overlay library. This means either that:
- the library (mumble_ol.dll) wasn&apos;t found in the directory you ran Mumble from
- you&apos;re on an OS earlier than WinXP SP2
- you do not have the August 2006 updated version of DX9.0c</source>
        <translation type="obsolete">Impossible de charger la librairie d&apos;overlay. Cela signifie que :
- la librairie ( mumble_ol.dll ) n&apos;a pas été trouvée dans le répertoire depuis lequel vous avez lancé Mumble.
- vous êtes sur une versions de windows antérieure à XP SP2
- vous n&apos;avez pas fait la mise à jour d&apos;Aout 2006 DX9.0c</translation>
    </message>
    <message>
        <source>Failed to load overlay library. This means either that:
- the library (mumble_ol.dll) wasn&apos;t found in the directory you ran Mumble from
- you&apos;re on an OS earlier than WinXP SP2
- you do not have the June 2007 updated version of DX9.0c</source>
        <translation type="obsolete">Impossible de charger la librairie d&apos;overlay. Cela signifie que :- la librairie ( mumble_ol.dll ) n&apos;a pas été trouvée dans le répertoire depuis lequel vous avez lancé Mumble.- vous êtes sur une versions de windows antérieure à XP SP2- vous n&apos;avez pas fait la mise à jour de Juin 2007 DX9.0c</translation>
    </message>
    <message>
        <source>Failed to load overlay library. This means either that:
- the library (mumble_ol.dll) wasn&apos;t found in the directory you ran Mumble from
- you&apos;re on an OS earlier than WinXP SP2</source>
        <translation type="obsolete">Impossible de charger la librairie d&apos;overlay. Cela signifie que :
- la librairie (mumble_ol.dll) n&apos;a pas été trouvée dans le répertoire depuis lequel vous avez lancé Mumble.
- vous êtes sur une versions de Windows antérieure à XP SP2</translation>
    </message>
    <message>
        <source>Failed to initialize overlay memory. This usually means that the shared memory is locked by the OS, and you need to reboot to release it.</source>
        <translation type="obsolete">Impossible d&apos;initialiser la mémoire pour l&apos;overlay. Celà signifie généralement que la mémoire est bloquée par le système d&apos;exploitation, et que vous devez redémarrer pour la libérer.</translation>
    </message>
</context>
<context>
    <name>OverlayClient</name>
    <message>
        <location filename="OverlayUserGroup.cpp" line="+94"/>
        <source>Filter</source>
        <translation>Filtre</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Only talking</source>
        <translation>Parlant uniquement</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>All in current channel</source>
        <translation>Tous dans le salon courant</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>All in linked channels</source>
        <translation>Tous dans les salons liés</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Always show yourself</source>
        <translation>Toujours s&apos;afficher</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Columns</source>
        <translation>Colonnes</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Edit...</source>
        <translation>Éditer...</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Reset Zoom</source>
        <translation>Réinitialiser le zoom</translation>
    </message>
    <message>
        <location filename="OverlayClient.cpp" line="+108"/>
        <source>FPS: %1</source>
        <translation>FPS : %1</translation>
    </message>
</context>
<context>
    <name>OverlayConfig</name>
    <message>
        <location filename="Overlay.ui"/>
        <source>Options</source>
        <translation>Options</translation>
    </message>
    <message>
        <source>Position</source>
        <translation type="obsolete">Position</translation>
    </message>
    <message>
        <source>Form</source>
        <oldsource>Font</oldsource>
        <translation type="obsolete">Overlay</translation>
    </message>
    <message>
        <location/>
        <source>Enable Overlay</source>
        <translation>Activer l&apos;overlay</translation>
    </message>
    <message>
        <location/>
        <source>Enable overlay.</source>
        <translation>Activer l&apos;overlay.</translation>
    </message>
    <message>
        <source>This sets whether the overlay is enabled or not. This setting is only checked with D3D9 applications are started, so make sure Mumble is running and this option is on before you start the application.&lt;br /&gt;Please note that if you start the application after starting Mumble, or if you disable the overlay while running, there is no safe way to restart the overlay without also restarting the application.</source>
        <translation type="obsolete">Spécifie si l&apos;overlay doit être activé ou non. Cette option est seulement vérifiée si des applications D3D9 sont démarrées, donc, vérifiez que Mumble fonctionne et que cette option avant de lancer une application.&lt;br /&gt;Veuillez noter que si vous démarez une application après avoir lancé Mumble ou que vous désactivez l&apos;overlay pendant qu&apos;il fonctionne, il n&apos;y a pas de moyen sûr de relancer l&apos;overlay sans relancer l&apos;application.</translation>
    </message>
    <message>
        <source>No one</source>
        <translation type="obsolete">Personne</translation>
    </message>
    <message>
        <source>Only talking</source>
        <translation type="obsolete">Parlant uniquement</translation>
    </message>
    <message>
        <source>Everyone</source>
        <translation type="obsolete">Tout le monde</translation>
    </message>
    <message>
        <source>Show</source>
        <translation type="obsolete">Afficher</translation>
    </message>
    <message>
        <source>Who to show on the overlay</source>
        <translation type="obsolete">Qui montrer sur l&apos;overlay</translation>
    </message>
    <message>
        <source>&lt;b&gt;This sets who to show in the in-game overlay.&lt;/b&gt;&lt;br /&gt;If many people are connected to the same channel, the overlay list might be very long. Use this to shorten it.&lt;br /&gt;&lt;i&gt;No one&lt;/i&gt; - Don&apos;t show anyone (but leave overlay running).&lt;br /&gt;&lt;i&gt;Only talking&lt;/i&gt; - Only show talking people.&lt;br /&gt;&lt;i&gt;Everyone&lt;/i&gt; - Show everyone.</source>
        <translation type="obsolete">&lt;b&gt;Définis qui montrer en jeu dans l&apos;overlay.&lt;/b&gt;&lt;br /&gt;Si de nombreuses personnes sont connectées sur le même salon, la liste de l&apos;overlay peut être très longue. Utilisez ça pour la raccourcir.&lt;br /&gt;&lt;i&gt;Personne&lt;/i&gt; - Ne montre personne (mais laisse l&apos;overlay fonctionner).&lt;br /&gt;&lt;i&gt;Parlant uniquement&lt;/i&gt; - Ne montre que les gens qui parlent.&lt;br /&gt;&lt;i&gt;Tout le monde&lt;/i&gt; - Montre tout le monde.</translation>
    </message>
    <message>
        <source>Always Show Self</source>
        <translation type="obsolete">Toujours s&apos;afficher soi même</translation>
    </message>
    <message>
        <source>Always show yourself on overlay.</source>
        <translation type="obsolete">Toujours vous montrer sur l&apos;overlay.</translation>
    </message>
    <message>
        <source>This sets whether to always show yourself or not. This setting is useful if you aren&apos;t showing everyone in the overlay, as then you would only see your own status if you were talking, which wouldn&apos;t let you see that you were deafened or muted.</source>
        <translation type="obsolete">Spécifie si vous voulez vous affichez dans l&apos;overlay ou non. C&apos;est utile si vous n&apos;êtes pas en train de montrer tout le monde, car vous pourrez voir votre propre status, donc savoir si vous parlez, si vous êtes sourd ou muet.</translation>
    </message>
    <message>
        <source>Grow Left</source>
        <translation type="obsolete">Aggrandir vers la gauche</translation>
    </message>
    <message>
        <source>Let overlay grow to the left</source>
        <translation type="obsolete">Laisser l&apos;overlay s&apos;aggrandir vers la gauche</translation>
    </message>
    <message>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow to the left if needed.</source>
        <translation type="obsolete">L&apos;overlay tente de rester aussi petit que possible à la position que vous avez sélectionné. Autorise l&apos;overlay à s&apos;aggrandir sur la gauche si besoin.</translation>
    </message>
    <message>
        <source>Grow Right</source>
        <translation type="obsolete">Aggrandir vers la droite</translation>
    </message>
    <message>
        <source>Let overlay grow to the Right</source>
        <translation type="obsolete">Laisser l&apos;overlay s&apos;aggrandir vers la droite</translation>
    </message>
    <message>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow to the Right if needed.</source>
        <translation type="obsolete">L&apos;overlay tente de rester aussi petit que possible à la position que vous avez sélectionné. Autorise l&apos;overlay à s&apos;aggrandir sur la droite si besoin.</translation>
    </message>
    <message>
        <source>Grow Up</source>
        <translation type="obsolete">Aggrandir vers le haut</translation>
    </message>
    <message>
        <source>Let overlay grow upwards</source>
        <translation type="obsolete">Laisser l&apos;overlay s&apos;aggrandir vers le haut</translation>
    </message>
    <message>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow to the top if needed.</source>
        <translation type="obsolete">L&apos;overlay tente de rester aussi petit que possible à la position que vous avez sélectionné. Autorise l&apos;overlay à s&apos;aggrandir vers le haut si besoin.</translation>
    </message>
    <message>
        <source>Grow Down</source>
        <translation type="obsolete">Aggrandir vers le bas</translation>
    </message>
    <message>
        <source>Let overlay grow downwards</source>
        <translation type="obsolete">Laisser l&apos;aggrandir vers le bas</translation>
    </message>
    <message>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow towards the bottom if needed.</source>
        <translation type="obsolete">L&apos;overlay tente de rester aussi petit que possible à la position que vous avez sélectionné. Autorise l&apos;overlay à s&apos;aggrandir vers le bas si besoin.</translation>
    </message>
    <message>
        <source>X-Position of Overlay</source>
        <translation type="obsolete">Position X de l&apos;Overlay</translation>
    </message>
    <message>
        <source>This sets the relative X position of the overlay.</source>
        <translation type="obsolete">Définit la position relative de l&apos;overlay sur l&apos;axe X.</translation>
    </message>
    <message>
        <source>Y-Position of Overlay</source>
        <translation type="obsolete">Position Y de l&apos;Overlay</translation>
    </message>
    <message>
        <source>This sets the relative Y position of the overlay.</source>
        <translation type="obsolete">Définit la position relative de l&apos;overlay sur l&apos;axe Y.</translation>
    </message>
    <message>
        <source>Current Font</source>
        <translation type="obsolete">Police actuelle</translation>
    </message>
    <message>
        <source>Set Font</source>
        <translation type="obsolete">Définir la police</translation>
    </message>
    <message>
        <source>Maximum width of names.</source>
        <translation type="obsolete">Largeur marximum des noms.</translation>
    </message>
    <message>
        <source>This sets the maximum width of names shown, relative to the height. If you set this to 100%, no line of text will be wider than it is high. At 500%, no line will be more than 5 times wider than its height, and so on.</source>
        <translation type="obsolete">Affecte la largeur maximum des noms affichés relativement à leur hauteur. Si vous mettez cette valeur à 100%, aucune ligne de texte ne sera plus large que haute. A 500%, aucune ligne ne sera 5 fois plus grande que sa hauteur, et ainsi de suite.</translation>
    </message>
    <message>
        <source>Maximum width</source>
        <translation type="obsolete">Largeur maximum</translation>
    </message>
    <message>
        <source>Change</source>
        <translation type="obsolete">Changer</translation>
    </message>
    <message>
        <source>Color for players</source>
        <translation type="obsolete">Couleur des joueurs</translation>
    </message>
    <message>
        <source>Color for talking players</source>
        <translation type="obsolete">Couleur des joueurs qui parlent</translation>
    </message>
    <message>
        <source>Color for alt-talking players</source>
        <translation type="obsolete">Couleur des joueurs qui ne parlent pas</translation>
    </message>
    <message>
        <source>Color for Channels</source>
        <translation type="obsolete">Couleur des canaux</translation>
    </message>
    <message>
        <source>Color for active Channels</source>
        <translation type="obsolete">Couleur des canaux actifs</translation>
    </message>
    <message>
        <location filename="OverlayConfig.cpp" line="+78"/>
        <source>To move the users, drag the little red dot.</source>
        <translation>Pour déplacer des utilisateurs, glissez-déplacez le petit point rouge.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>To resize the users, mouse wheel over a user.</source>
        <oldsource>To resize the users, mousewheel over a user.</oldsource>
        <translation>Pour redimensionner les utilisateurs, utilisez la molette au dessus de l&apos;un d&apos;eux.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>For more options, right click a user.</source>
        <oldsource>For more options, rightclick a user.</oldsource>
        <translation>Pour plus d&apos;options, faites un clic-droit sur un utilisateur.</translation>
    </message>
    <message>
        <location line="+30"/>
        <source>FPS: %1</source>
        <translation>FPS : %1</translation>
    </message>
    <message>
        <location line="+194"/>
        <source>Overlay</source>
        <translation>Overlay</translation>
    </message>
    <message>
        <location line="+75"/>
        <source>Choose executable</source>
        <translation>Choisir le fichier exécutable</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Choose application</source>
        <translation>Choisir l&apos;application</translation>
    </message>
    <message>
        <location line="+38"/>
        <source>Mumble has deemed the installer valid.</source>
        <translation>Mumble a jugé le programme d&apos;installation valide.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Mumble was unable to verify the authenticity of the installer.</source>
        <oldsource>Mumble was unable to validate authenticity of the installer.</oldsource>
        <translation>Mumble n&apos;a pas pu vérifier l&apos;authenticité du programme d&apos;installation.</translation>
    </message>
    <message>
        <location line="+87"/>
        <source>Load Overlay Presets</source>
        <translation>Charger les préférences de l&apos;overlay</translation>
    </message>
    <message>
        <location line="+2"/>
        <location line="+28"/>
        <source>Mumble overlay presets (*.mumblelay)</source>
        <translation>Préférences de l&apos;overlay (*.mumblelay)</translation>
    </message>
    <message>
        <location line="-2"/>
        <source>Save Overlay Presets</source>
        <translation>Sauvegarder les préférences de l&apos;overlay</translation>
    </message>
    <message>
        <source>Maximum height of names.</source>
        <translation type="obsolete">Hauteur maximale des noms.</translation>
    </message>
    <message>
        <source>This sets the maximum height of names shown, relative to the screen height. If your active 3D window is 800 pixels tall and this is set to 5%, each name will be 40 pixels tall. Note that the names will not be taller than 60 pixels no matter what you set here.</source>
        <translation type="obsolete">Spécifie la hauteur maximale des noms affichés, relatif à la taille de l&apos;écran. Si vous avez une fenêtre 3D de 800 pixels de haut et que vous définissez 5%, chaque nom sera haut de 40 pixels. Il est à noter que, peu importe ce que vous avez défini, les noms ne feront pas plus de 60 pixels de haut.</translation>
    </message>
    <message>
        <source>Maximum height</source>
        <translation type="obsolete">Hauteur maximum</translation>
    </message>
    <message>
        <source>Show User Textures</source>
        <translation type="obsolete">Montrer les textures des utilisateurs</translation>
    </message>
    <message>
        <source>Show User custom textures instead of text on the overlay.</source>
        <translation type="obsolete">Montrer les textures des utilisateurs au lieu du texte de l&apos;overlay.</translation>
    </message>
    <message>
        <source>This sets whether to download and use custom textures for registered users. If disabled, the regular outline text will be used instead.</source>
        <translation type="obsolete">Ceci définit le téléchargement et l&apos;utilisation des textures personnalisées pour les utilisateurs enregistrés. Si désactivé, l&apos;esquisse de texte ordinaire sera utilisé à sa place.</translation>
    </message>
    <message>
        <source>Show no one</source>
        <translation type="obsolete">Afficher aucun</translation>
    </message>
    <message>
        <source>Show only talking</source>
        <translation type="obsolete">Montrer seulement ceux qui parlent</translation>
    </message>
    <message>
        <source>Show everyone</source>
        <translation type="obsolete">Montrer tout le monde</translation>
    </message>
    <message>
        <source>Color for users</source>
        <translation type="obsolete">Couleur des utilisateurs</translation>
    </message>
    <message>
        <source>Color for talking users</source>
        <translation type="obsolete">Couleur des utilisateurs parlants</translation>
    </message>
    <message>
        <source>Color for whispering users</source>
        <translation type="obsolete">Couleur des utilisateurs chuchotant</translation>
    </message>
    <message>
        <source>Color for channels</source>
        <translation type="obsolete">Couleur des salons</translation>
    </message>
    <message>
        <source>Color for active channels</source>
        <translation type="obsolete">Couleur des salons actifs</translation>
    </message>
    <message>
        <location filename="Overlay.ui"/>
        <source>This sets whether the overlay is enabled or not. This setting is only checked when applications are started, so make sure Mumble is running and this option is on before you start the application.&lt;br /&gt;Please note that if you start Mumble after starting the application, or if you disable the overlay while the application is running, there is no safe way to restart the overlay without also restarting the application.</source>
        <oldsource>This sets whether the overlay is enabled or not. This setting is only checked when applications are started, so make sure Mumble is running and this option is on before you start the application.&lt;br /&gt;Please note that if you start the application after starting Mumble, or if you disable the overlay while running, there is no safe way to restart the overlay without also restarting the application.</oldsource>
        <translation>Définit si l&apos;overlay est actif ou pas. Cette option est vérifiée au lancement des applications, assurez-vous donc que Mumble fonctionne et que cette option soit activée avant de démarrer l&apos;application.&lt;br/&gt;Veuillez noter que si vous démarrez Mumble après le lancement de l&apos;application, ou si vous désactivez l&apos;overlay pendant que l&apos;application fonctionne, il n&apos;y a pas de manières sûre pour réactiver l&apos;overlay sans redémarrer l&apos;application.</translation>
    </message>
    <message>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow upwards if needed.</source>
        <translation type="obsolete">L&apos;overlay tente de rester aussi petit que possible à la position que vous avez sélectionné. Autorise l&apos;overlay à s&apos;aggrandir vers le haut si besoin.</translation>
    </message>
    <message>
        <source>Let overlay grow to the right</source>
        <translation type="obsolete">Laisser l&apos;overlay s&apos;aggrandir vers la droite</translation>
    </message>
    <message>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow to the right if needed.</source>
        <translation type="obsolete">L&apos;overlay tente de rester aussi petit que possible à la position que vous avez sélectionné. Autorise l&apos;overlay à s&apos;aggrandir sur la droite si besoin.</translation>
    </message>
    <message>
        <source>The overlay tries to stay as small as possible and at the position you have selected. This allows the overlay to grow downwards if needed.</source>
        <translation type="obsolete">L&apos;overlay tente de rester aussi petit que possible à la position que vous avez sélectionné. Autorise l&apos;overlay à s&apos;aggrandir vers le bas si besoin.</translation>
    </message>
    <message>
        <source>TextLabel</source>
        <translation type="obsolete">TextLabel</translation>
    </message>
    <message>
        <location/>
        <source>Layout</source>
        <translation>Disposition</translation>
    </message>
    <message>
        <location/>
        <source>Overlay exceptions</source>
        <translation>Exceptions pour l&apos;overlay</translation>
    </message>
    <message>
        <location/>
        <source>Blacklist</source>
        <translation>Liste noire</translation>
    </message>
    <message>
        <location/>
        <source>Whitelist</source>
        <translation>Liste blanche</translation>
    </message>
    <message>
        <source>Add</source>
        <translation type="obsolete">Ajouter</translation>
    </message>
    <message>
        <location/>
        <source>Remove</source>
        <translation>Supprimer</translation>
    </message>
    <message>
        <location/>
        <source>Uninstall Overlay</source>
        <translation>Désinstaller l&apos;overlay</translation>
    </message>
    <message>
        <location/>
        <source>Overlay Installation</source>
        <translation>Installation de l&apos;overlay</translation>
    </message>
    <message>
        <location/>
        <source>Mumble has detected that you do not have the Mumble Overlay installed.

Click the button below to install the overlay.</source>
        <translation>Mumble a détecté que l&apos;overlay n&apos;était pas installé.

Cliquez sur le bouton ci-dessous pour l&apos;installer.</translation>
    </message>
    <message>
        <location/>
        <source>Install Mumble Overlay</source>
        <translation>Installer l&apos;overlay</translation>
    </message>
    <message>
        <location/>
        <source>Overlay Upgrade</source>
        <translation>Mise à jour de l&apos;overlay</translation>
    </message>
    <message>
        <location/>
        <source>Mumble has detected an old version of the overlay support files installed on your computer.

To upgrade these files to their latest versions, click the button below.</source>
        <translation>Mumble a détecté une ancienne version de l&apos;overlay sur votre ordinateur.

Pour mettre à jour l&apos;overlay, cliquez sur le bouton ci-dessous.</translation>
    </message>
    <message>
        <location/>
        <source>Upgrade Mumble Overlay</source>
        <translation>Mettre à jour l&apos;overlay</translation>
    </message>
    <message>
        <location/>
        <source>Display a frame counter in the overlay</source>
        <translation>Afficher les FPS dans l&apos;overlay</translation>
    </message>
    <message>
        <location/>
        <source>Show FPS counter</source>
        <translation>Afficher les FPS</translation>
    </message>
    <message>
        <location/>
        <source>View Certificates</source>
        <translation>Voir les certificats</translation>
    </message>
    <message>
        <location/>
        <source>Load an overlay preset from file</source>
        <translation>Importe les préférences de l&apos;overlay depuis un fichier</translation>
    </message>
    <message utf8="true">
        <location/>
        <source>Load…</source>
        <translation>Importer...</translation>
    </message>
    <message>
        <location/>
        <source>Save your overlay settings to file</source>
        <translation>Exporte vos préférences de l&apos;overlay dans un fichier</translation>
    </message>
    <message utf8="true">
        <location/>
        <source>Save…</source>
        <translation>Exporter...</translation>
    </message>
    <message>
        <location/>
        <source>FPS Display</source>
        <translation>Affichage des FPS</translation>
    </message>
    <message>
        <location/>
        <source>Set the overlay font.</source>
        <translation>Règle la police de l&apos;overlay.</translation>
    </message>
    <message>
        <location/>
        <source>Font</source>
        <translation>Police</translation>
    </message>
    <message>
        <location/>
        <source>Set the overlay text color.</source>
        <translation>Définir la couleur du texte de l&apos;overlay.</translation>
    </message>
    <message>
        <location/>
        <source>Color</source>
        <translation>Couleur</translation>
    </message>
    <message>
        <location/>
        <source>Add...</source>
        <translation>Ajouter...</translation>
    </message>
</context>
<context>
    <name>OverlayEditor</name>
    <message>
        <source>Dialog</source>
        <translation type="obsolete">Dialogue</translation>
    </message>
    <message>
        <location filename="OverlayEditor.ui"/>
        <source>State</source>
        <translation>État</translation>
    </message>
    <message>
        <location/>
        <source>User is not talking</source>
        <translation>L&apos;utilisateur ne parle pas</translation>
    </message>
    <message>
        <location/>
        <source>Passive</source>
        <translation>Passif</translation>
    </message>
    <message>
        <location/>
        <source>User is talking in your channel or a linked channel</source>
        <translation>L&apos;utilisateur parle dans votre salon ou dans un salon lié</translation>
    </message>
    <message>
        <location/>
        <source>Talking</source>
        <translation>Parle</translation>
    </message>
    <message>
        <location/>
        <source>User is whispering to you privately</source>
        <translation>L&apos;utilisateur vous chuchote en privé</translation>
    </message>
    <message>
        <location/>
        <source>Private Whisper</source>
        <translation>Chuchotement privé</translation>
    </message>
    <message>
        <location/>
        <source>User is shouting to your channel</source>
        <translation>L&apos;utilisateur crie dans votre salon</translation>
    </message>
    <message>
        <location/>
        <source>Channel Whisper</source>
        <translation>Crie au salon</translation>
    </message>
    <message>
        <location/>
        <source>Zoom</source>
        <translation>Zoom</translation>
    </message>
    <message>
        <location/>
        <source>Zoom Factor</source>
        <translation>Facteur de zoom</translation>
    </message>
    <message>
        <location/>
        <source>Enabled Elements</source>
        <translation>Éléments activés</translation>
    </message>
    <message>
        <location/>
        <source>User avatar, chosen by each user</source>
        <translation>Avatar d&apos;utilisateur, choisi par chaque utilisateur</translation>
    </message>
    <message>
        <location/>
        <source>Avatar</source>
        <translation>Avatar</translation>
    </message>
    <message>
        <location/>
        <source>User&apos;s name</source>
        <translation>Nom d&apos;utilisateur</translation>
    </message>
    <message>
        <location/>
        <source>Username</source>
        <translation>Nom d&apos;utilisateur</translation>
    </message>
    <message>
        <location/>
        <source>Name of user&apos;s channel, if outside your current channel</source>
        <translation>Nom des salons de l&apos;utilisateur, si en dehors de votre salon actuel</translation>
    </message>
    <message>
        <location/>
        <source>Channel</source>
        <translation>Salon</translation>
    </message>
    <message>
        <location/>
        <source>Muted or deafened</source>
        <translation>Sourd(e) ou muet(te)</translation>
    </message>
    <message>
        <location/>
        <source>Mute state</source>
        <translation>Muet</translation>
    </message>
    <message>
        <location/>
        <source>Bounding box, automatically shrunk to minimum size to contain all visible elements</source>
        <translation>Zone de délimitation, réduit automatiquement à la taille minimale pour contenir tout élément visible</translation>
    </message>
    <message>
        <location/>
        <source>Bounding box</source>
        <translation>Zone de délimitation</translation>
    </message>
    <message>
        <location/>
        <source>Overlay Editor</source>
        <translation>Éditeur de l&apos;overlay</translation>
    </message>
</context>
<context>
    <name>OverlayEditorScene</name>
    <message>
        <location filename="OverlayEditorScene.cpp" line="+396"/>
        <source>Layout preset</source>
        <translation>Disposition prédéfinie</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Large square avatar</source>
        <translatorcomment>Grand avatar carré</translatorcomment>
        <translation>Grand avatar carré</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Avatar and Name</source>
        <translation>Avatar et nom</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>User Opacity</source>
        <translation>Opacité de l&apos;utilisateur</translation>
    </message>
    <message>
        <location line="+6"/>
        <location line="+31"/>
        <source>%1%</source>
        <translation>%1 %</translation>
    </message>
    <message>
        <location line="-5"/>
        <source>Object Opacity</source>
        <translation>Opacité de l&apos;objet</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Alignment</source>
        <translation>Alignement</translation>
    </message>
    <message>
        <location line="+11"/>
        <source>Left</source>
        <translation>Gauche</translation>
    </message>
    <message>
        <location line="+5"/>
        <location line="+18"/>
        <source>Center</source>
        <translation>Centre</translation>
    </message>
    <message>
        <location line="-13"/>
        <source>Right</source>
        <translation>Droite</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Top</source>
        <translation>Haut</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Bottom</source>
        <translation>Bas</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Color...</source>
        <translation>Couleur...</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Font...</source>
        <translation>Police...</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Bounding box</source>
        <translation>Zone de délimitation</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Pen width</source>
        <translation>Largeur du crayon</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Padding</source>
        <translatorcomment>Is there a way to translate this ? oO</translatorcomment>
        <translation>Padding</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Pen color</source>
        <translation>Couleur du crayon</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Fill color</source>
        <translation>Couleur de remplissage</translation>
    </message>
    <message>
        <location line="+87"/>
        <source>Pick pen color</source>
        <translation>Choisir la couleur du crayon</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Pick fill color</source>
        <translation>Choisir la couleur de remplissage</translation>
    </message>
    <message>
        <location line="+14"/>
        <source>Pick color</source>
        <translation>Choisir la couleur</translation>
    </message>
    <message>
        <location line="+21"/>
        <source>Pick font</source>
        <translation>Choisir la police</translation>
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
        <translation>Impossible de charger la librairie d&apos;overlay. Cela signifie que :
- la librairie (mumble_ol.dll) n&apos;a pas été trouvée dans le répertoire depuis lequel vous avez lancé Mumble.
- vous êtes sur une versions de Windows antérieure à XP SP2</translation>
    </message>
</context>
<context>
    <name>OverlayUser</name>
    <message>
        <source>%1%</source>
        <translation type="obsolete">%1 %</translation>
    </message>
</context>
<context>
    <name>PAAudioConfig</name>
    <message>
        <source>Input Device</source>
        <translation type="obsolete">Périphérique d&apos;entré</translation>
    </message>
    <message>
        <source>Output Device</source>
        <translation type="obsolete">Périphérique de sortie</translation>
    </message>
</context>
<context>
    <name>PlayerModel</name>
    <message>
        <source>Name of player</source>
        <translation type="obsolete">Nom du joueur</translation>
    </message>
    <message>
        <source>Name of channel</source>
        <translation type="obsolete">Nom du canal</translation>
    </message>
    <message>
        <source>Player flags</source>
        <translation type="obsolete">Attributs du joueur</translation>
    </message>
    <message>
        <source>This is a player connected to the server. The icon to the left of the player indicates whether or not they are talking:&lt;br /&gt;&lt;img src=&quot;skin:talking_on.png&quot; /&gt; Talking&lt;br /&gt;&lt;img src=&quot;skin:talking_off.png&quot; /&gt; Not talking</source>
        <translation type="obsolete">Il s&apos;agit d&apos;un joueur connecté au serveur. L&apos;icône située à gauche du lecteur indique si elles sont ou non en train de parler: &lt;br /&gt;&lt;img src=&quot;skin:talking_on.png&quot; /&gt; En train de parler&lt;br /&gt;&lt;img src=&quot;skin:talking_off.png&quot; /&gt; Ne parlant pas</translation>
    </message>
    <message>
        <source>This is a channel on the server. Only players in the same channel can hear each other.</source>
        <translation type="obsolete">Canal du serveur. Seuls les joueurs dans le même canal pourront s&apos;entendre.</translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="obsolete">Nom</translation>
    </message>
    <message>
        <source>Flags</source>
        <translation type="obsolete">Attributs</translation>
    </message>
    <message>
        <source>Mumble</source>
        <translation type="obsolete">Mumble</translation>
    </message>
    <message>
        <source>Are you sure you want to drag this channel?</source>
        <translation type="obsolete">Etes vous sur de vouloir déplacer ce salon?</translation>
    </message>
</context>
<context>
    <name>PluginConfig</name>
    <message>
        <location filename="Plugins.ui"/>
        <source>Options</source>
        <translation>Options</translation>
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
        <translation>Lier au jeu et transmettre la position</translation>
    </message>
    <message>
        <location/>
        <source>Enable plugins and transmit positional information</source>
        <translation>Activer les plugins et transmettre les informations de position</translation>
    </message>
    <message>
        <source>This enables plugins for supported games to fetch your in-game position and transmit that with each voice packet. This enables other players to hear your voice in-game from the direction your character is in relation to their own.</source>
        <translation type="obsolete">Active le plugin permettant aux jeux supportés de transmettre la position du joueur dans chaque paquet de voix. Cela permet aux autres joueurs d&apos;entendre votre voix dans le jeu dans la position de votre personnage par rapport aux autres.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Reload plugins</source>
        <translation>&amp;Recharger les plugins</translation>
    </message>
    <message>
        <location/>
        <source>Reloads all plugins</source>
        <translation>Recharge tous les plugins</translation>
    </message>
    <message>
        <location/>
        <source>This allows plugins for supported games to fetch your in-game position and transmit it with each voice packet. This enables other users to hear your voice in-game from the direction your character is in relation to their own.</source>
        <translation>Permet aux plugins des jeux supportés récupérer votre position dans le jeu et de la transmettre. Permet aussi aux autres utilisateurs d&apos;entendre votre voix dans le jeux en rapport avec votre position et sa propre position.</translation>
    </message>
    <message>
        <location/>
        <source>This rescans and reloads plugins. Use this if you just added or changed a plugin to the plugins directory.</source>
        <translation>Rechercher et recharger les plugins. Utilisez cette fonction si vous venez d&apos;ajouter ou de modifier un plugin dans le répertoire des plugins.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;About</source>
        <translation>&amp;A propos</translation>
    </message>
    <message>
        <location/>
        <source>Information about plugin</source>
        <translation>Informations sur le plugin</translation>
    </message>
    <message>
        <location/>
        <source>This shows a small information message about the plugin.</source>
        <translation>Affiche une rapide introduction au fonctionnement du plugin.</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Configure</source>
        <translation>&amp;Configurer</translation>
    </message>
    <message>
        <location/>
        <source>Show configuration page of plugin</source>
        <translation>Afficher la page de configuration du plugin</translation>
    </message>
    <message>
        <location/>
        <source>This shows the configuration page of the plugin, if any.</source>
        <translation>Affiche la page de configuration du plugin si elle existe.</translation>
    </message>
    <message>
        <source>Mumble</source>
        <translation type="obsolete">Mumble</translation>
    </message>
    <message>
        <location filename="Plugins.cpp" line="+56"/>
        <source>Plugin has no configure function.</source>
        <translation>Le plugin n&apos;a pas de fonction « configure » (configurer).</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Plugin has no about function.</source>
        <translation>Le plugin n&apos;a pas de fonction « about » (à propos).</translation>
    </message>
    <message>
        <location filename="Plugins.ui"/>
        <source>Form</source>
        <translation>Form</translation>
    </message>
    <message>
        <location/>
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <location/>
        <source>Enabled</source>
        <translation>Activé</translation>
    </message>
</context>
<context>
    <name>Plugins</name>
    <message>
        <location filename="Plugins.cpp" line="+330"/>
        <source>Skipping plugin update in debug mode.</source>
        <translation>Passe la mise à jour des plugins en mode débogage.</translation>
    </message>
    <message>
        <location line="+155"/>
        <location line="+6"/>
        <source>Downloaded new or updated plugin to %1.</source>
        <translation>Téléchargé le nouveau plugin ou mis à jour vers %1.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Failed to install new plugin to %1.</source>
        <translation>Échec de l&apos;installation du nouveau plugin dans %1.</translation>
    </message>
    <message>
        <source>Plugin %1 lost link.</source>
        <translation type="obsolete">Le plugin %1 est désactivé.</translation>
    </message>
    <message>
        <location line="-265"/>
        <source>%1 lost link.</source>
        <translation>%1 est désactivé.</translation>
    </message>
    <message>
        <source>Plugin %1 linked.</source>
        <translation type="obsolete">Le plugin %1 est activé.</translation>
    </message>
    <message>
        <location line="+74"/>
        <source>%1 linked.</source>
        <translation>%1 est activé.</translation>
    </message>
</context>
<context>
    <name>PortAudioSystem</name>
    <message>
        <location filename="PAAudio.cpp" line="+273"/>
        <source>Default Device</source>
        <translation>Périphérique par défaut</translation>
    </message>
</context>
<context>
    <name>PulseAudioConfig</name>
    <message>
        <source>PulseAudio</source>
        <translation type="obsolete">Pulseaudio</translation>
    </message>
    <message>
        <source>%1ms</source>
        <translation type="obsolete">%1ms</translation>
    </message>
    <message>
        <source>Device selection</source>
        <translation type="obsolete">Selection du périphérique</translation>
    </message>
    <message>
        <source>Input (Microphone)</source>
        <translation type="obsolete">Entrée (Microphone)</translation>
    </message>
    <message>
        <source>Device to use for microphone</source>
        <translation type="obsolete">Périphérique à utiliser pour le microphone</translation>
    </message>
    <message>
        <source>This set which source mumble should use for the microphone. The &lt;i&gt;default&lt;/i&gt; device means to use the PulseAudio defaults.</source>
        <translation type="obsolete">Ceci renseigne quelle source mumble doit utiliser pour le microphone. Le périphérique par &lt;i&gt;defaut&lt;/i&gt; à utiliser lePulseAudio.</translation>
    </message>
    <message>
        <source>Output</source>
        <translation type="obsolete">Sortie</translation>
    </message>
    <message>
        <source>Device to use for speakers/headphones</source>
        <translation type="obsolete">Périphérique à utiliser pour les enceintes/casques</translation>
    </message>
    <message>
        <source>Enable echo cancellation</source>
        <translation type="obsolete">Activer l&apos;annulation de l&apos;echo</translation>
    </message>
    <message>
        <source>This enables echo cancellation. Don&apos;t enable this unless you are listening on speakers; for headphones it will cause audio degradation.</source>
        <translation type="obsolete">Ceci active l&apos;annulation de l&apos;echo.Ne pas activer si vous écouter avec des enceintes; Pour les casques cela peut causer des dégradations de la qualité audio.</translation>
    </message>
    <message>
        <source>Use echo cancellation</source>
        <translation type="obsolete">Utiliser l&apos;annulation de l&apos;echo</translation>
    </message>
    <message>
        <source>Output Options</source>
        <translation type="obsolete">Options de sortie</translation>
    </message>
    <message>
        <source>Output Delay</source>
        <translation type="obsolete">Retard de sortie</translation>
    </message>
    <message>
        <source>Amount of data to buffer for ALSA</source>
        <translation type="obsolete">Quantité de données du tampon pour ALSA</translation>
    </message>
    <message>
        <source>This sets the amount of data to prebuffer in the output buffer. Experiment with different values and set it to the lowest which doesn&apos;t cause rapid jitter in the sound.</source>
        <translation type="obsolete">Spécifie la quantité de données à mettre en cache dans le tampon de sortie. Essayez différentes valeurs et mettez la plus basse qui ne cause pas de rapides décalages du son.</translation>
    </message>
</context>
<context>
    <name>PulseAudioSystem</name>
    <message>
        <location filename="PulseAudio.cpp" line="+545"/>
        <source>Default Input</source>
        <translation>Entrée par défaut</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Default Output</source>
        <translation>Sortie par défaut</translation>
    </message>
</context>
<context>
    <name>RichTextEditor</name>
    <message>
        <location filename="RichTextEditor.cpp" line="+241"/>
        <source>Failed to load image</source>
        <translation>Impossible de charger l&apos;image</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Image file too large to embed in document. Please use images smaller than %1 kB.</source>
        <translation>L&apos;image est trop grosse pour être intégrée au document. Veuillez utliser des images plus petites que %1 ko.</translation>
    </message>
    <message>
        <location line="+81"/>
        <source>Message is too long.</source>
        <translation>Le message est trop long.</translation>
    </message>
    <message>
        <source>TabWidget</source>
        <translation type="obsolete">TabWidget</translation>
    </message>
    <message>
        <location filename="RichTextEditor.ui"/>
        <source>Display</source>
        <translation>Affichage</translation>
    </message>
    <message>
        <location/>
        <source>Source Text</source>
        <translation>Texte source</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Bold</source>
        <translation>&amp;Gras</translation>
    </message>
    <message>
        <location/>
        <source>Ctrl+B</source>
        <translation>Ctrl+B</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Italic</source>
        <translation>&amp;Italique</translation>
    </message>
    <message>
        <location/>
        <source>Italic</source>
        <translation>Italique</translation>
    </message>
    <message>
        <location/>
        <source>Ctrl+I</source>
        <translation>Ctrl+I</translation>
    </message>
    <message>
        <location/>
        <source>Underline</source>
        <translation>Souligne</translation>
    </message>
    <message>
        <location/>
        <source>Ctrl+U</source>
        <translation>Ctrl+U</translation>
    </message>
    <message>
        <location/>
        <source>Color</source>
        <translation>Couleur</translation>
    </message>
    <message>
        <location/>
        <source>Insert Link</source>
        <translation>Insérer un Lien</translation>
    </message>
    <message>
        <location/>
        <source>Ctrl+L</source>
        <translation>Ctrl+L</translation>
    </message>
    <message>
        <location/>
        <source>Insert Image</source>
        <translation>Insérer une Image</translation>
    </message>
</context>
<context>
    <name>RichTextEditorLink</name>
    <message>
        <location filename="RichTextEditorLink.ui"/>
        <source>Add Link</source>
        <translation>Ajouter un lien</translation>
    </message>
    <message>
        <location/>
        <source>URL</source>
        <translation>URL</translation>
    </message>
    <message>
        <location/>
        <source>Text</source>
        <translation>Texte</translation>
    </message>
</context>
<context>
    <name>ServerHandler</name>
    <message>
        <location filename="ServerHandler.cpp" line="+401"/>
        <source>UDP packets cannot be sent to or received from the server. Switching to TCP mode.</source>
        <translation>Les paquets UDP ne peuvent pas être envoyés ni reçus du serveur. Passage en mode TCP.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>UDP packets cannot be sent to the server. Switching to TCP mode.</source>
        <translation>Les paquets UDP ne peuvent pas être envoyés au  serveur. Passage en mode TCP.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>UDP packets cannot be received from the server. Switching to TCP mode.</source>
        <translation>Les paquets UDP ne peuvent pas être reçus du serveur. Passage en mode TCP.</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>UDP packets can be sent to and received from the server. Switching back to UDP mode.</source>
        <translation>Les paquets UDP  peuvent être envoyés et reçus du serveur. Retour en mode UDP.</translation>
    </message>
    <message>
        <location line="+36"/>
        <source>Connection timed out</source>
        <translation>Délai de connexion dépassé</translation>
    </message>
</context>
<context>
    <name>ServerView</name>
    <message>
        <location filename="ConnectDialog.cpp" line="-611"/>
        <source>Favorite</source>
        <translation>Favoris</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>LAN</source>
        <translation>Réseau local</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Public Internet</source>
        <translation>Internet public</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Africa</source>
        <translation>Afrique</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Asia</source>
        <translation>Asie</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>North America</source>
        <translation>Amérique du Nord</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>South America</source>
        <translation>Amérique du Sud</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Europe</source>
        <translation>Europe</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Oceania</source>
        <translation>Océanie</translation>
    </message>
</context>
<context>
    <name>ShortcutActionWidget</name>
    <message>
        <location filename="GlobalShortcut.cpp" line="-483"/>
        <source>Unassigned</source>
        <translation>Non assigné</translation>
    </message>
</context>
<context>
    <name>ShortcutDelegate</name>
    <message>
        <location line="+361"/>
        <source>On</source>
        <translation>Activer</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Off</source>
        <translation>Désactiver</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Toggle</source>
        <translation>Commuter</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Unassigned</source>
        <translation>Non assigné</translation>
    </message>
</context>
<context>
    <name>ShortcutKeyWidget</name>
    <message>
        <location line="-435"/>
        <source>Press Shortcut</source>
        <translation>Appuyer sur un raccourci clavier</translation>
    </message>
</context>
<context>
    <name>ShortcutTargetDialog</name>
    <message>
        <location line="+206"/>
        <source>Root</source>
        <translation>Racine</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Parent</source>
        <translation>Parent</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Current</source>
        <translation>Actuel</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Subchannel #%1</source>
        <translation>Sous salon #%1</translation>
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
        <translation>Racine</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Parent</source>
        <translation>Parent</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Current</source>
        <translation>Actuel</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Subchannel #%1</source>
        <translation>Sous salon #%1</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Invalid</source>
        <translation>Invalide</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Empty</source>
        <oldsource>&lt;Empty&gt;</oldsource>
        <translation>Vide</translation>
    </message>
</context>
<context>
    <name>ShortcutToggleWidget</name>
    <message>
        <location line="-259"/>
        <source>Off</source>
        <translation>Désactiver</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Toggle</source>
        <translation>Commuter</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>On</source>
        <translation>Activer</translation>
    </message>
</context>
<context>
    <name>TextMessage</name>
    <message>
        <source>Message to send</source>
        <translation type="obsolete">Message à envoyer</translation>
    </message>
    <message>
        <source>Preview</source>
        <translation type="obsolete">Prévisualisation</translation>
    </message>
    <message>
        <source>Disables html formating</source>
        <translation type="obsolete">Désactivé le format html</translation>
    </message>
    <message>
        <location filename="TextMessage.h" line="+46"/>
        <source>Enter text</source>
        <translation>Saisir le texte</translation>
    </message>
    <message>
        <location filename="TextMessage.ui"/>
        <source>If checked the message is recursively sent to all subchannels</source>
        <translation>Si cochée, le message sera envoyé récursivement à tous les sous salons</translation>
    </message>
    <message>
        <location/>
        <source>Send recursively to subchannels</source>
        <translation>Envoyer récursivement aux sous salons</translation>
    </message>
</context>
<context>
    <name>Tokens</name>
    <message>
        <location filename="Tokens.cpp" line="+69"/>
        <source>Empty Token</source>
        <translation>Jeton vide</translation>
    </message>
    <message>
        <location filename="Tokens.ui"/>
        <source>Mumble - Access Tokens</source>
        <translation>Mumble - Jetons d&apos;accès</translation>
    </message>
    <message>
        <location/>
        <source>List of access tokens on current server</source>
        <translation>Liste des jetons d&apos;accès du serveur actuel</translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;This is an editable list of access tokens on the connected server.&lt;/b&gt;
&lt;br /&gt;
An access token is a text string, which can be used as a password for very simple access management on channels. Mumble will remember the tokens you&apos;ve used and resend them to the server next time you reconnect, so you don&apos;t have to enter these every time.
</source>
        <translation>&lt;b&gt;C&apos;est une liste éditable des jetons d&apos;accès du serveur actuel.&lt;/b&gt;&lt;br/&gt;
Un jeton d&apos;accès est une chaîne de caractères, qui peut être utilisée comme mot de passe pour une gestion simple des accès aux salons. Mumble mémorise les jetons que vous avez utilisé et les réutilisera la prochaine fois que vous vous reconecterez, vous n&apos;avez donc pas à les ressaisir à chaque fois.</translation>
    </message>
    <message>
        <location/>
        <source>Add a token</source>
        <translation>Ajouter un jeton</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Add</source>
        <translation>&amp;Ajouter</translation>
    </message>
    <message>
        <location/>
        <source>Remove a token</source>
        <translation>Supprimer un jeton</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Remove</source>
        <translation>Supp&amp;rimer</translation>
    </message>
</context>
<context>
    <name>UserEdit</name>
    <message>
        <location filename="UserEdit.ui"/>
        <source>Registered Users</source>
        <translation>Utilisateurs enregistrés</translation>
    </message>
    <message>
        <location/>
        <location filename="UserEdit.cpp" line="+96"/>
        <source>Remove</source>
        <translation>Supprimer</translation>
    </message>
    <message>
        <location filename="UserEdit.cpp" line="-5"/>
        <source>Rename</source>
        <translation>Renommer</translation>
    </message>
</context>
<context>
    <name>UserInformation</name>
    <message>
        <location filename="UserInformation.ui"/>
        <source>User Information</source>
        <translation>Information d&apos;utilisateur</translation>
    </message>
    <message>
        <location/>
        <source>Connection Information</source>
        <translation>Information de connexion</translation>
    </message>
    <message>
        <location/>
        <source>Version</source>
        <translation>Version</translation>
    </message>
    <message>
        <location/>
        <source>OS</source>
        <translation>Système d&apos;exploitation</translation>
    </message>
    <message>
        <location/>
        <source>Certificate</source>
        <translation>Certificat</translation>
    </message>
    <message>
        <location/>
        <source>IP Address</source>
        <translation>Adresse IP</translation>
    </message>
    <message>
        <location/>
        <source>CELT Versions</source>
        <translation>Versions CELT</translation>
    </message>
    <message>
        <location/>
        <source>Details...</source>
        <translation>Détails...</translation>
    </message>
    <message>
        <location/>
        <source>Ping Statistics</source>
        <translation>Statistiques du ping</translation>
    </message>
    <message>
        <location/>
        <source>Pings received</source>
        <translation>Pings reçus</translation>
    </message>
    <message>
        <location/>
        <source>Average ping</source>
        <translation>Moyenne du ping</translation>
    </message>
    <message>
        <location/>
        <source>TCP (Control)</source>
        <translation>TCP (Contrôle)</translation>
    </message>
    <message>
        <location/>
        <source>UDP (Voice)</source>
        <translation>UDP (Voix)</translation>
    </message>
    <message>
        <location/>
        <source>UDP Network statistics</source>
        <translation>Statistiques réseau UDP</translation>
    </message>
    <message>
        <location/>
        <source>Good</source>
        <translation>Bon</translation>
    </message>
    <message>
        <location/>
        <source>Late</source>
        <translation>Tardif</translation>
    </message>
    <message>
        <location/>
        <source>Lost</source>
        <translation>Perdu</translation>
    </message>
    <message>
        <location/>
        <source>Resync</source>
        <translation>Resynchronise</translation>
    </message>
    <message>
        <location/>
        <source>From Client</source>
        <translation>Depuis le Client</translation>
    </message>
    <message>
        <location/>
        <source>To Client</source>
        <translation>Pour le Client</translation>
    </message>
    <message>
        <source>Bandwidth</source>
        <translation type="obsolete">Bande passante</translation>
    </message>
    <message>
        <location/>
        <source>Connection time</source>
        <translation>Temps de connexion</translation>
    </message>
    <message>
        <location filename="UserInformation.cpp" line="+93"/>
        <source>%1w</source>
        <translation>%1sem</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1d</source>
        <translation>%1j</translation>
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
        <translation>%1 en ligne (%2 au repos)</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1 online</source>
        <translation>%1 en ligne</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>%1 kbit/s</source>
        <translation>%1 kbits/s</translation>
    </message>
    <message>
        <location filename="UserInformation.ui"/>
        <source>Bandwidth</source>
        <comment>GroupBox</comment>
        <translation>Bande passante</translation>
    </message>
    <message>
        <location/>
        <source>Bandwidth</source>
        <comment>Label</comment>
        <translation>Bande passante</translation>
    </message>
</context>
<context>
    <name>UserModel</name>
    <message>
        <location filename="UserModel.cpp" line="-804"/>
        <source>This is a user connected to the server. The icon to the left of the user indicates whether or not they are talking:</source>
        <translation>C&apos;est un utilisateur connecté au serveur. L&apos;icône à gauche de l&apos;utilisateur infique si il parle ou pas :</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Talking to your channel.</source>
        <translation>Parle dans votre salon.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Shouting directly to your channel.</source>
        <oldsource>Whispering directly to your channel.</oldsource>
        <translation>Crie directement dans votre salon.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Whispering directly to you.</source>
        <translation>Vous chuchote directement.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Not talking.</source>
        <translation>Ne parle pas.</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>This is a channel on the server. The icon indicates the state of the channel:</source>
        <translation>C&apos;est un salon dans le serveur. L&apos;icône indique l&apos;état du salon :</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Your current channel.</source>
        <translation>Votre salon actuel.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>A channel that is linked with your channel. Linked channels can talk to each other.</source>
        <translation>Un salon qui est lié au votre. Les salons liés peuvent se parler l&apos;un-l&apos;autre.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>A channel on the server that you are not linked to.</source>
        <translation>Un salon du serveur qui n&apos;est pas lié.</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>This shows the flags the user has on the server, if any:</source>
        <translation>Affiche les attributs de l&apos;utilisateur sur le serveur, si disponible :</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>On your friend list</source>
        <translation>Dans votre liste d&apos;amis</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Authenticated user</source>
        <translation>Utilisateur authentifié</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Muted (manually muted by self)</source>
        <translation>Muet(te) (s&apos;est rendu muet(te))</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Muted (manually muted by admin)</source>
        <translation>Muet(te) (manuellement par un admin)</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Muted (not allowed to speak in current channel)</source>
        <translation>Muet(te) (n&apos;a pas la permission de parler dans le salon actuel)</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Muted (muted by you, only on your machine)</source>
        <translation>Muet(te) (vous l&apos;avez rendu muet(te), seulement sur votre machine)</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Deafened (by self)</source>
        <translation>Sourd(e) (s&apos;est rendu sourd(e))</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Deafened (by admin)</source>
        <translation>Sourd(e) (par un admin)</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>User has a new comment set (click to show)</source>
        <translation>L&apos;utilisateur a un nouveau commentaire (cliquer pour afficher)</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>User has a comment set, which you&apos;ve already seen. (click to show)</source>
        <translation>L&apos;utilisateur a un commentaire que vous avez déjà vu. (Cliquer pour afficher)</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>This shows the flags the channel has, if any:</source>
        <translation>Affiche l&apos;attribut du salon, si disponible :</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Channel has a new comment set (click to show)</source>
        <translation>Le salon a un nouveau commentaire (cliquer pour afficher)</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Channel has a comment set, which you&apos;ve already seen. (click to show)</source>
        <translation>Le salon a un commentaire que vous avez déjà lu. (Cliquer pour afficher)</translation>
    </message>
    <message>
        <location line="+18"/>
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Flags</source>
        <translation>Attributs</translation>
    </message>
    <message>
        <source>Mumble</source>
        <translation type="obsolete">Mumble</translation>
    </message>
    <message>
        <location line="+721"/>
        <source>Are you sure you want to drag this channel?</source>
        <translation>Etes vous sur de vouloir déplacer ce salon ?</translation>
    </message>
    <message>
        <location line="+93"/>
        <location line="+20"/>
        <source>Cannot perform this movement automatically, please reset the numeric sorting indicators or adjust it manually.</source>
        <translation>Ne peut pas effectuer ce déplacement automatiquement, veuillez réinitialiser les indicateurs de position, ou l&apos;ajuster manuellement.</translation>
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
        <translation type="obsolete">Mumble n&apos;a pas pu récupérer les informations de version depuis le serveur de SourceForge.</translation>
    </message>
    <message>
        <location filename="VersionCheck.cpp" line="+130"/>
        <source>Upgrade Mumble</source>
        <translation>Mettre Mumble à jour</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>A new version of Mumble has been detected and automatically downloaded. It is recommended that you either upgrade to this version, or downgrade to the latest stable release. Do you want to launch the installer now?</source>
        <translation>Une nouvelle version de Mumble a été détectée et automatiquement téléchargée. Il est recommandé de mettre à niveau à cette version ou de revenir à la dernière version stable. Voulez-vous lancer l&apos;installateur maintenant ?</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Failed to launch snapshot installer.</source>
        <translation>Échec dans le lancement de l&apos;installateur du snapshot.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Corrupt download of new version detected. Automatically removed.</source>
        <translation>Détection d&apos;un téléchargement corrompu d&apos;une nouvelle version. Suppression automatique.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Downloading new snapshot from %1 to %2</source>
        <translation>Téléchargement du nouveau snapshot de %1 à %2</translation>
    </message>
    <message>
        <location line="+18"/>
        <source>Failed to write new version to disk.</source>
        <oldsource>Failed to write new version to disc.</oldsource>
        <translation>Échec lors de l&apos;écriture de la nouvelle version sur le disque.</translation>
    </message>
    <message>
        <location line="+17"/>
        <source>Mumble failed to retrieve version information from the central server.</source>
        <translation>Mumble ne peut pas retrouver les informations de version depuis le serveur central.</translation>
    </message>
</context>
<context>
    <name>ViewCert</name>
    <message>
        <location filename="ViewCert.cpp" line="+41"/>
        <source>Certificate Chain Details</source>
        <translation>Détail de la chaîne de certificats</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Certificate chain</source>
        <translation>Chaîne de certificat</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Certificate details</source>
        <translation>Détails du certificat</translation>
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
        <translation>Nom courant : %1</translation>
    </message>
    <message>
        <location line="-27"/>
        <location line="+28"/>
        <source>Organization: %1</source>
        <translation>Organistaion : %1</translation>
    </message>
    <message>
        <location line="-27"/>
        <source>Subunit: %1</source>
        <translation>Nom d&apos;unité : %1</translation>
    </message>
    <message>
        <location line="+1"/>
        <location line="+28"/>
        <source>Country: %1</source>
        <translation>Pays : %1</translation>
    </message>
    <message>
        <location line="-27"/>
        <location line="+28"/>
        <source>Locality: %1</source>
        <translation>Localité : %1</translation>
    </message>
    <message>
        <location line="-27"/>
        <location line="+28"/>
        <source>State: %1</source>
        <translation>Pays : %1</translation>
    </message>
    <message>
        <location line="-27"/>
        <source>Valid from: %1</source>
        <translation>Valide depuis : %1</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Valid to: %1</source>
        <translation>Valide jusqu&apos;a : %1</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Serial: %1</source>
        <translation>Numéro de série : %1</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Public Key: %1 bits %2</source>
        <translation>Clé publique : %1 bits %2</translation>
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
        <translation>Digest (MD5) : %1</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Email: %1</source>
        <translation>Courriel : %1</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>DNS: %1</source>
        <translation>DNS : %1</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Issued by:</source>
        <translation>Délivré par :</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Unit Name: %1</source>
        <translation>Nom d&apos;unité : %1</translation>
    </message>
</context>
<context>
    <name>VoiceRecorder</name>
    <message>
        <location filename="VoiceRecorder.cpp" line="+184"/>
        <source>Invalid sample rate given to recorder</source>
        <translation>Taux d&apos;échantillonnage donné à l&apos;enregistreur incorrect</translation>
    </message>
    <message>
        <location line="+98"/>
        <source>Recorder failed to create directory &apos;%1&apos;</source>
        <translation>L&apos;enregistreur n&apos;a pas pu créer le répertoire &apos;%1&apos;</translation>
    </message>
    <message>
        <location line="+14"/>
        <source>Recorder failed to open file &apos;%s&apos;</source>
        <translation>L&apos;enregistreur n&apos;a pas pu ouvrir le fichier &apos;%s&apos;</translation>
    </message>
    <message>
        <location line="+109"/>
        <source>.wav - Uncompressed</source>
        <translation>.wave - Non compressé</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>.ogg (Vorbis) - Compressed</source>
        <translation>.ogg (Vorbis) - Compressé</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>.au - Uncompressed</source>
        <translation>.au - Non compressé</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>.flac - Lossless compressed</source>
        <translation>.flac - Compressé sans perte</translation>
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
        <translation>Enregistreur</translation>
    </message>
    <message>
        <location/>
        <source>Control</source>
        <translation>Commandes</translation>
    </message>
    <message>
        <location/>
        <source>00:00:00</source>
        <translation>00:00:00</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Start</source>
        <translation>&amp;Démarrer</translation>
    </message>
    <message>
        <location/>
        <source>S&amp;top</source>
        <translation>&amp;Arrêter</translation>
    </message>
    <message>
        <location/>
        <source>Mode</source>
        <translation>Mode</translation>
    </message>
    <message>
        <location/>
        <source>Multichannel</source>
        <translation>Multicanal</translation>
    </message>
    <message>
        <location/>
        <source>Output</source>
        <translation>Sortie</translation>
    </message>
    <message>
        <location/>
        <source>Output format</source>
        <translation>Format</translation>
    </message>
    <message>
        <location/>
        <source>Target directory</source>
        <translation>Répertoire</translation>
    </message>
    <message>
        <location/>
        <source>Filename</source>
        <translation>Nom du fichier</translation>
    </message>
    <message>
        <location/>
        <source>&amp;Browse...</source>
        <translation>&amp;Parcourir...</translation>
    </message>
    <message>
        <location filename="VoiceRecorderDialog.cpp" line="-195"/>
        <source>Closing the recorder will stop your current recording. Do you really want to close the recorder?</source>
        <translation>Fermer l&apos;enregistreur arrêtera l&apos;enregistrement en cours. Voulez-vous vraiment fermer l&apos;enregistreur ?</translation>
    </message>
    <message>
        <location line="-31"/>
        <source>Valid variables are:</source>
        <translation>Les variables valides sont :</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Inserts the user&apos;s name</source>
        <translation>Insère le nom de l&apos;utilisateur</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Inserts the current date</source>
        <translation>Insère la date actuelle</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Inserts the current time</source>
        <translation>Insère l&apos;heure courante</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Inserts the hostname</source>
        <translation>Insère le nom d&apos;hôte</translation>
    </message>
    <message>
        <location line="+60"/>
        <source>Unable to start recording. Not connected to a server.</source>
        <translation>Impossible de débuter l&apos;enregistrement. Vous n&apos;êtes connecté à aucun serveur.</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>The server you are currently connected to is version 1.2.2 or older. For privacy reasons, recording on servers of versions older than 1.2.3 is not possible.
Please contact your server administrator for further information.</source>
        <translation>Le serveur auquel vous êtes connecté est d&apos;une version antérieure à la 1.2.3. Pour des raisons de protection de la vie privée, l&apos;enregistrement sur ces serveurs n&apos;est pas possible.
Contactez l&apos;administrateur de votre serveur pour de plus amples informations.</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>There is already a recorder active for this server.</source>
        <translation>Il y a déjà en enregistrement en cours pour ce serveur.</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Please select a recording format.</source>
        <translation>Merci de choisir un format d&apos;enregistrement.</translation>
    </message>
    <message>
        <location line="+95"/>
        <source>Select target directory</source>
        <translation>Sélectionnez le répertoire de destination</translation>
    </message>
    <message>
        <location filename="VoiceRecorderDialog.ui"/>
        <source>Downmix</source>
        <translation>Downmix</translation>
    </message>
</context>
<context>
    <name>WASAPIConfig</name>
    <message>
        <source>Input (Microphone)</source>
        <translation type="obsolete">Entrée (Microphone)</translation>
    </message>
    <message>
        <source>Output</source>
        <translation type="obsolete">Sortie</translation>
    </message>
    <message>
        <source>Enable echo cancellation</source>
        <translation type="obsolete">Activer l&apos;annulation de l&apos;echo</translation>
    </message>
    <message>
        <source>This enables echo cancellation. Don&apos;t enable this unless you are listening on speakers; for headphones it will cause audio degradation.</source>
        <translation type="obsolete">Ceci active l&apos;annulation de l&apos;echo.Ne pas activer si vous écouter avec des enceintes; Pour les casques cela peut causer des dégradations de la qualité audio.</translation>
    </message>
    <message>
        <source>Use echo cancellation</source>
        <translation type="obsolete">Utiliser l&apos;annulation de l&apos;echo</translation>
    </message>
</context>
<context>
    <name>WASAPISystem</name>
    <message>
        <location filename="WASAPI.cpp" line="+180"/>
        <source>Default Device</source>
        <translation>Périphérique par défaut</translation>
    </message>
</context>
<context>
    <name>XInputKeyWidget</name>
    <message>
        <source>Press Shortcut</source>
        <translation type="obsolete">Pressez les touches du raccourci</translation>
    </message>
</context>
</TS>
