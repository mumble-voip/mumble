// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ACL.h"
#include "Channel.h"
#include "Group.h"
#include "User.h"

#ifdef MURMUR
#	include "ServerUser.h"

#	include <QtCore/QStack>
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

ChanACL::operator QString() const {
	QString aclString;
	bool isFirstEntry = true;
	// Iterate over all flags and assume none of the important ones (all other than Cached and All)
	// have a value > 2^32
	for (int i = 0; i < 32; i++) {
		int currentPermInt = 1 << i;
		// If we have a name for the permission, we know it exists.
		// Note that we won't reach 0 with this tactic but we don't care about the None
		// permission anyways.
		QString name = permName(static_cast< Perm >(currentPermInt));

		if (!name.isEmpty()) {
			if (!((pAllow & currentPermInt) || (pDeny & currentPermInt))) {
				// The current permission is left unchanged by this ACL -> Don't include it in the
				// string representation
				continue;
			}

			if (!isFirstEntry) {
				aclString += QLatin1String(", ");
			}

			isFirstEntry = false;

			aclString += QString::fromLatin1("\"%1\": ").arg(name);

			if ((pAllow & currentPermInt) && (pDeny & currentPermInt)) {
				// The permissions is allowed and denied. In the current implementation this should
				// result in i tbeing denied, but for printing purposes we don't make assumptions about
				// the handling of this in the implementation.
				aclString += QLatin1String("Allowed && Denied");
			} else {
				if (pAllow & currentPermInt) {
					aclString += QLatin1String("Allowed");
				} else {
					aclString += QLatin1String("Denied");
				}
			}
		}
	}

	if (!aclString.isEmpty()) {
		// Also include info about affected user and/or group
		if (!qsGroup.isEmpty() && iUserId >= 0) {
			// both group and user-id are set
			return QString::fromLatin1("ACL for group \"%1\" and user with ID %2: %3")
				.arg(qsGroup)
				.arg(iUserId)
				.arg(aclString);
		} else if (!qsGroup.isEmpty()) {
			return QString::fromLatin1("ACL for group \"%1\": %2").arg(qsGroup).arg(aclString);
		} else if (iUserId >= 0) {
			return QString::fromLatin1("ACL for user with ID %1: %2").arg(iUserId).arg(qsGroup);
		}
	}

	return aclString;
}

// Check permissions.
// This will always return true for the superuser,
// and will return false if a user isn't allowed to
// traverse to the channel. (Need "read" in all preceding channels)

#ifdef MURMUR

bool ChanACL::hasPermission(ServerUser *p, Channel *chan, QFlags< Perm > perm, ACLCache *cache) {
	Permissions granted = effectivePermissions(p, chan, cache);

	return ((granted & perm) != None);
}

// Return effective permissions.
QFlags< ChanACL::Perm > ChanACL::effectivePermissions(ServerUser *p, Channel *chan, ACLCache *cache) {
	// Superuser
	if (p->iId == 0) {
		return static_cast< Permissions >(All & ~(Speak | Whisper));
	}

	Permissions granted;

	if (cache) {
		QHash< Channel *, Permissions > *h = cache->value(p);
		if (h)
			granted = h->value(chan);
	}

	if (granted & Cached) {
		return granted;
	}

	QStack< Channel * > chanstack;
	Channel *ch = chan;

	while (ch) {
		chanstack.push(ch);
		ch = ch->cParent;
	}

	// Default permissions
	Permissions def = Traverse | Enter | Speak | Whisper | TextMessage | Listen;

	granted = def;

	bool traverse = true;
	bool write    = false;

	// Iterate over all parent channels from root to the channel the user is in (inclusive)
	while (!chanstack.isEmpty()) {
		ch = chanstack.pop();
		if (!ch->bInheritACL) {
			granted = def;
		}

		for (const ChanACL *acl : ch->qlACL) {
			bool matchUser  = (acl->iUserId != -1) && (acl->iUserId == p->iId);
			bool matchGroup = Group::appliesToUser(*chan, *ch, acl->qsGroup, *p);

			bool applyFromSelf  = (ch == chan && acl->bApplyHere);
			bool applyInherited = (ch != chan && acl->bApplySubs);

			// Flag indicating whether the current ACL affects the target channel "chan"
			bool apply = applyFromSelf || applyInherited;

			// "apply" will be true for ACLs set in the reference channel directly (applyHere),
			// or from a parent channel which hands the ACLs down (applySubs).
			// However, we have one ACL that needs to be evaluated differently - the Traverse ACL.
			// Consider this channel layout:
			// Root
			// - A (Traverse denied for THIS channel, but not sub channels)
			//  - B
			//   - C
			// If the user tries to enter C, we need to deny Traverse, because the user
			// should already be blocked from traversing A. But "apply" will be false,
			// as the "normal" ACL inheritence rules do not apply here.
			// Therefore, we need applyDenyTraverse which will be true, if any channel
			// from root to the reference channel denies Traverse without necessarily
			// handing it down.
			bool applyDenyTraverse = applyInherited || acl->bApplyHere;

			if (matchUser || matchGroup) {
				// The "traverse" and "write" booleans do not grant or deny anything here.
				// We merely check, if we are missing traverse AND write in this
				// channel and therefore abort without any permissions later on.
				if (apply && (acl->pAllow & Traverse)) {
					traverse = true;
				}
				if (applyDenyTraverse && (acl->pDeny & Traverse)) {
					traverse = false;
				}

				write = apply && (acl->pAllow & Write) && !(acl->pDeny & Write);

				// These permissions are only grantable from the root channel
				// as they affect the users globally. For example: You can not
				// kick a client from a channel without kicking them from the server.
				if (ch->iId == 0 && applyFromSelf) {
					if (acl->pAllow & Kick) {
						granted |= Kick;
					}
					if (acl->pAllow & Ban) {
						granted |= Ban;
					}
					if (acl->pAllow & ResetUserContent) {
						granted |= ResetUserContent;
					}
					if (acl->pAllow & Register) {
						granted |= Register;
					}
					if (acl->pAllow & SelfRegister) {
						granted |= SelfRegister;
					}
				}

				// Every other regular ACL is handled here
				if (apply) {
					granted |= (acl->pAllow & ~(Kick | Ban | ResetUserContent | Register | SelfRegister | Cached));
					granted &= ~acl->pDeny;
				}
			}
		}
		if (!traverse && !write) {
			granted = None;
			break;
		}
	}

	if (granted & Write) {
		granted |=
			Traverse | Enter | MuteDeafen | Move | MakeChannel | LinkChannel | TextMessage | MakeTempChannel | Listen;
		if (chan->iId == 0)
			granted |= Kick | Ban | ResetUserContent | Register | SelfRegister;
	}

	if (cache) {
		if (!cache->contains(p))
			cache->insert(p, new QHash< Channel *, Permissions >);

		cache->value(p)->insert(chan, granted | Cached);
	}

	return granted;
}

#else

QString ChanACL::whatsThis(Perm p) {
	switch (p) {
		case None:
			return tr("This represents no privileges.");
		case Write:
			return tr("This represents total access to the channel, including the ability to change group and ACL "
					  "information. "
					  "This privilege implies all other privileges.");
		case Traverse:
			return tr(
				"This represents the permission to traverse the channel. If a user is denied this privilege, he will "
				"be "
				"unable to access this channel and any sub-channels in any way, regardless of other permissions in the "
				"sub-channels.");
		case Enter:
			return tr(
				"This represents the permission to join the channel. If you have a hierarchical channel structure, you "
				"might want to give everyone Traverse, but restrict Enter in the root of your hierarchy.");
		case Speak:
			return tr(
				"This represents the permission to speak in a channel. Users without this privilege will be suppressed "
				"by "
				"the server (seen as muted), and will be unable to speak until they are unmuted by someone with the "
				"appropriate privileges.");
		case Whisper:
			return tr("This represents the permission to whisper to this channel from the outside. This works exactly "
					  "like the <i>speak</i> "
					  "privilege, but applies to packets spoken with the Whisper key held down. This may be used to "
					  "broadcast to a hierarchy "
					  "of channels without linking.");
		case MuteDeafen:
			return tr(
				"This represents the permission to mute and deafen other users. Once muted, a user will stay muted "
				"until he is unmuted by another privileged user or reconnects to the server.");
		case Move:
			return tr(
				"This represents the permission to move a user to another channel or kick him from the server. To "
				"actually "
				"move the user, either the moving user must have Move privileges in the destination channel, or "
				"the user must normally be allowed to enter the channel. Users with this privilege can move users "
				"into channels the target user normally wouldn't have permission to enter.");
		case MakeChannel:
			return tr("This represents the permission to make sub-channels. The user making the sub-channel will be "
					  "added to the "
					  "admin group of the sub-channel.");
		case MakeTempChannel:
			return tr("This represents the permission to make a temporary subchannel. The user making the sub-channel "
					  "will be added to the "
					  "admin group of the sub-channel. Temporary channels are not stored and disappear when the last "
					  "user leaves.");
		case LinkChannel:
			return tr(
				"This represents the permission to link channels. Users in linked channels hear each other, as long as "
				"the speaking user has the <i>speak</i> privilege in the channel of the listener. You need the link "
				"privilege in both channels to create a link, but just in either channel to remove it.");
		case TextMessage:
			return tr("This represents the permission to write text messages to other users in this channel.");
		case Kick:
			return tr("This represents the permission to forcibly remove users from the server.");
		case Ban:
			return tr("This represents the permission to permanently remove users from the server.");
		case ResetUserContent:
			return tr("This represents the permission to reset the comment or avatar of a user.");
		case Register:
			return tr("This represents the permission to register and unregister users on the server.");
		case SelfRegister:
			return tr("This represents the permission to register oneself on the server.");
		case Listen:
			return tr("This represents the permission to use the listen-feature allowing to listen to a channel "
					  "without being in it.");
		default:
			break;
	}
	return QString();
}

#endif

QString ChanACL::permName(QFlags< Perm > p) {
	QStringList qsl;
	for (int i = 0; i <= 31; ++i) {
		if (p & (1 << i))
			qsl << permName(static_cast< Perm >(1 << i));
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
		case ResetUserContent:
			return tr("Reset User Content");
		case Register:
			return tr("Register User");
		case SelfRegister:
			return tr("Register Self");
		case Listen:
			return tr("Listen");
		default:
			break;
	}
	return QString();
}

bool ChanACL::isPassword() const {
	// A password is an ACL that applies to a group of the form '#<something>'
	// AND grants 'Enter'
	// AND grants 'Speak', 'Whisper', 'TextMessage', 'LinkChannel' and potentially Traverse but NOTHING else
	// AND does not deny anything.
	// Furthermore the ACL must apply directly to the channel and may not be inherited.
	return this->qsGroup.startsWith(QLatin1Char('#')) && this->bApplyHere && !this->bInherited
		   && (this->pAllow & ChanACL::Enter)
		   && (this->pAllow
				   == (ChanACL::Enter | ChanACL::Speak | ChanACL::Whisper | ChanACL::TextMessage | ChanACL::LinkChannel)
			   || // Backwards compat with old behaviour that didn't deny traverse
			   this->pAllow
				   == (ChanACL::Enter | ChanACL::Speak | ChanACL::Whisper | ChanACL::TextMessage | ChanACL::LinkChannel
					   | ChanACL::Traverse))
		   && this->pDeny == ChanACL::None;
}
