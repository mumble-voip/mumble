/* Copyright (C) 2005, Thorvald Natvig <thorvald@natvig.com>

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
#include <QStack>

ChanACL::ChanACL(Channel *chan) {
	bApplyHere = true;
	bApplySubs = true;

	iPlayerId = -1;

	c = chan;
	c->qlACL << this;
}

// Check permissions.
// This will always return true for the superuser,
// and will return false if a user isn't allowed to
// traverse to the channel. (Need "read" in all preceeding channels)

bool ChanACL::hasPermission(Player *p, Channel *chan, Perm perm) {
	QStack<Channel *> chanstack;
	Channel *ch;
	ChanACL *acl;

	// Superuser
	if (p->iId == 0)
		return true;

	ch = chan;
	while (ch) {
		chanstack.push(ch);
		ch = ch->cParent;
	}

	// Default permissions
	Permissions def = Read | Enter | Speak;
	Permissions granted = def;

	bool traverse = true;

	while (! chanstack.isEmpty()) {
		ch = chanstack.pop();
		if (! ch->bInheritACL)
			granted = def;

		foreach(acl, ch->qlACL) {
			if ((acl->iPlayerId == p->iId) || Group::isMember(chan, acl->qsGroup, p)) {
				if (acl->pAllow & Read)
					traverse = true;
				if (acl->pDeny & Read)
					traverse = false;
				if ((ch==chan && acl->bApplyHere) || (ch!=chan && acl->bApplySubs)) {
					granted |= acl->pAllow;
					granted &= ~acl->pDeny;
				}
			}
		}
		if (! traverse)
			return false;
	}

	return ((granted & (perm | Write)) != None);
}

QString ChanACL::permName(Perm p) {
	switch (p) {
		case None:
			return "None";
		case Write:
			return "Write";
		case Read:
			return "Read";
		case Enter:
			return "Enter";
		case Speak:
			return "Speak";
		case MuteDeafen:
			return "Mute/Deafen";
		case MoveKick:
			return "Mute/Kick";
		case MakeChannel:
			return "Make channel";
		default:
			qFatal("Asked to translate unknown permission %d", p);
	}
	return "";
}