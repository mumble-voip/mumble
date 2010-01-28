/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>

   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.
   - Neither the name of the Mumble Developers nor the names of its
     contributors may be used to endorse or promote products derived from this
     software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "ACL.h"
#include "Channel.h"
#include "Group.h"
#include "User.h"

#ifdef MURMUR
#include "ServerUser.h"
#endif

ChanACL::ChanACL(Channel *chan) : QObject(chan) {
	bApplyHere = true;
	bApplySubs = true;
	bInherited = false;

	iUserId = -1;

	c = chan;
	if (c)
		c->qlACL << this;
}

// Check permissions.
// This will always return true for the superuser,
// and will return false if a user isn't allowed to
// traverse to the channel. (Need "read" in all preceeding channels)

#ifdef MURMUR

bool ChanACL::hasPermission(ServerUser *p, Channel *chan, QFlags<Perm> perm, ACLCache &cache) {
	QStack<Channel *> chanstack;
	Channel *ch;
	ChanACL *acl;

	// Superuser
	if (p->iId == 0) {
		switch (perm) {
			case Speak:
			case Whisper:
				return false;
			default:
				return true;
		}
	}

	Permissions granted = 0;

	QHash<Channel *, Permissions> *h = cache.value(p);
	if (h)
		granted = h->value(chan);

	if (granted & Cached) {
		if ((perm != Speak) && (perm != Whisper))
			return ((granted & (perm | Write)) != None);
		else
			return ((granted & perm) != None);
	}

	ch = chan;
	while (ch) {
		chanstack.push(ch);
		ch = ch->cParent;
	}

	// Default permissions
	Permissions def = Traverse | Enter | Speak | Whisper | TextMessage;

	granted = def;

	bool traverse = true;
	bool write = false;

	while (! chanstack.isEmpty()) {
		ch = chanstack.pop();
		if (! ch->bInheritACL)
			granted = def;

		foreach(acl, ch->qlACL) {
			bool matchUser = (acl->iUserId != -1) && (acl->iUserId == p->iId);
			bool matchGroup = Group::isMember(chan, ch, acl->qsGroup, p);
			if (matchUser || matchGroup) {
				if (acl->pAllow & Traverse)
					traverse = true;
				if (acl->pDeny & Traverse)
					traverse = false;
				if (acl->pAllow & Write)
					write = true;
				if (acl->pDeny & Write)
					write = false;
				if ((ch==chan && acl->bApplyHere) || (ch!=chan && acl->bApplySubs)) {
					granted |= acl->pAllow;
					granted &= ~acl->pDeny;
				}
			}
		}
		if (! traverse && ! write) {
			granted = None;
			break;
		}
	}

	if (! cache.contains(p))
		cache.insert(p, new QHash<Channel *, Permissions>);

	cache.value(p)->insert(chan, granted | Cached);

	if ((perm != Speak) && (perm != Whisper))
		return ((granted & (perm | Write)) != None);
	else
		return ((granted & perm) != None);
}

#else

QString ChanACL::whatsThis(Perm p) {
	switch (p) {
		case None:
			return tr("This represents no privileges.");
		case Write:
			return tr("This represents total access to the channel, including the ability to change group and ACL information. "
			          "This privilege implies all other privileges.");
		case Traverse:
			return tr("This represents the permission to traverse the channel. If a user is denied this privilege, he will be "
			          "unable to access this channel and any sub-channels in any way, regardless of other permissions in the "
			          "sub-channels.");
		case Enter:
			return tr("This represents the permission to join the channel. If you have a hierarchical channel structure, you "
			          "might want to give everyone Traverse, but restrict Enter in the root of your hierarchy.");
		case Speak:
			return tr("This represents the permission to speak in a channel. Users without this privilege will be suppressed by "
			          "the server (seen as muted), and will be unable to speak until they are unmuted by someone with the "
			          "appropriate privileges.");
		case Whisper:
			return tr("This represents the permission to whisper to this channel from the outside. This works exactly like the <i>speak</i> "
			          "privilege, but applies to packets spoken with the Whisper key held down. This may be used to broadcast to a hierarchy "
			          "of channels without linking.");
		case MuteDeafen:
			return tr("This represents the permission to mute and deafen other users. Once muted, a user will stay muted "
			          "until he is unmuted by another privileged user or reconnects to the server.");
		case Move:
			return tr("This represents the permission to move a user to another channel or kick him from the server. To actually "
			          "move the user, either the moving user must have Move privileges in the destination channel, or "
			          "the user must normally be allowed to enter the channel. Users with this privilege can move users "
			          "into channels the target user normally wouldn't have permission to enter.");
		case MakeChannel:
			return tr("This represents the permission to make sub-channels. The user making the sub-channel will be added to the "
			          "admin group of the sub-channel.");
		case MakeTempChannel:
			return tr("This represents the permission to make a temporary subchannel. The user making the sub-channel will be added to the "
			          "admin group of the sub-channel. Temporary channels are not stored and disappear when the last user leaves.");
		case LinkChannel:
			return tr("This represents the permission to link channels. Users in linked channels hear each other, as long as "
			          "the speaking user has the <i>speak</i> privilege in the channel of the listener. You need the link "
			          "privilege in both channels to create a link, but just in either channel to remove it.");
		case TextMessage:
			return tr("This represents the permission to write text messages to other users in this channel.");
		case Kick:
			return tr("This represents the permission to forcibly remove users from the server.");
		case Ban:
			return tr("This represents the permission to permanently remove users from the server.");
		case Register:
			return tr("This represents the permission to register and unregister users on the server.");
		case SelfRegister:
			return tr("This represents the permission to register oneself on the server.");
		default:
			break;
	}
	return QString();
}

#endif

QString ChanACL::permName(QFlags<Perm> p) {
	QStringList qsl;
	for (int i=0;i<=31;++i) {
		if (p & (1<<i))
			qsl << permName(static_cast<Perm>(1<<i));
	}
	return qsl.join(QLatin1String(", "));
}

QString ChanACL::permName(Perm p) {
	switch (p) {
		case None:
			return tr("None");
		case Write:
			return tr("Write ACL");
		case Traverse:
			return tr("Traverse");
		case Enter:
			return tr("Enter");
		case Speak:
			return tr("Speak");
		case Whisper:
			return tr("Whisper");
		case MuteDeafen:
			return tr("Mute/Deafen");
		case Move:
			return tr("Move");
		case MakeChannel:
			return tr("Make channel");
		case MakeTempChannel:
			return tr("Make temporary");
		case LinkChannel:
			return tr("Link channel");
		case TextMessage:
			return tr("Text message");
		case Kick:
			return tr("Kick");
		case Ban:
			return tr("Ban");
		case Register:
			return tr("Register User");
		case SelfRegister:
			return tr("Register Self");
		default:
			break;
	}
	return QString();
}
