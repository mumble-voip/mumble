/* Copyright (C) 2005-2007, Thorvald Natvig <thorvald@natvig.com>

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

#include "Server.h"
#include "DBus.h"

QDBusArgument &operator<<(QDBusArgument &a, const PlayerInfo &s) {
  a.beginStructure();
  a << s.session << s.id << s.name << s.mute << s.deaf << s.suppressed << s.selfMute << s.selfDeaf << s.channel;
  a.endStructure();
  return a;
}

const QDBusArgument & operator >>(const QDBusArgument &a, PlayerInfo &s) {
  a.beginStructure();
  a >> s.session >> s.id >> s.name >> s.mute >> s.deaf >> s.suppressed >> s.selfMute >> s.selfDeaf >> s.channel;
  a.endStructure();
  return a;
}

QDBusArgument &operator<<(QDBusArgument &a, const ChannelInfo &s) {
  a.beginStructure();
  a << s.id << s.name << s.parent << s.links;
  a.endStructure();
  return a;
}

const QDBusArgument & operator >>(const QDBusArgument &a, ChannelInfo &s) {
  a.beginStructure();
  a >> s.id >> s.name >> s.parent >> s.links;
  a.endStructure();
  return a;
}

QDBusArgument &operator<<(QDBusArgument &a, const GroupInfo &s) {
  a.beginStructure();
  a << s.name << s.inherited << s.inherit << s.inheritable;
  a << s.add << s.remove << s.members;
  a.endStructure();
  return a;
}

const QDBusArgument & operator >>(const QDBusArgument &a, GroupInfo &s) {
  a.beginStructure();
  a >> s.name >> s.inherited >> s.inherit >> s.inheritable;
  a >> s.add >> s.remove >> s.members;
  a.endStructure();
  return a;
}

QDBusArgument &operator<<(QDBusArgument &a, const ACLInfo &s) {
  a.beginStructure();
  a << s.applyHere << s.applySubs << s.inherited;
  a << s.playerid << s.group;
  a << s.allow << s.deny;
  a.endStructure();
  return a;
}

const QDBusArgument & operator >>(const QDBusArgument &a, ACLInfo &s) {
  a.beginStructure();
  a >> s.applyHere >> s.applySubs >> s.inherited;
  a >> s.playerid >> s.group;
  a >> s.allow >> s.deny;
  a.endStructure();
  return a;
}

MurmurDBus::MurmurDBus(QCoreApplication &app) : QDBusAbstractAdaptor(&app) {
  qDBusRegisterMetaType<PlayerInfo>();
  qDBusRegisterMetaType<ChannelInfo>();
  qDBusRegisterMetaType<GroupInfo>();
  qDBusRegisterMetaType<ACLInfo>();
}

QList<PlayerInfo> MurmurDBus::getPlayers() {
  QList<PlayerInfo> a;
  foreach(Player *p, g_sServer->qmPlayers) {
    a << PlayerInfo(p);
  }
  return a;
}

QList<ChannelInfo> MurmurDBus::getChannels() {
  QList<ChannelInfo> a;
  return a;
}

PlayerInfo::PlayerInfo(Player *p) {
  session = p->sId;
  id = p->iId;
  name = p->qsName;
  mute = p->bMute;
  deaf = p->bDeaf;
  suppressed = p->bSuppressed;
  selfMute = p->bSelfMute;
  selfDeaf = p->bSelfDeaf;
  channel = p->cChannel->iId;
}

ChannelInfo::ChannelInfo(Channel *c) {
  id = c->iId;
  name = c->qsName;
  parent = c->cParent ? c->cParent->iId : -1;
  foreach(Channel *chn, c->qsPermLinks)
    links << chn->iId;
}

void MurmurDBus::playerStateChanged(Player *p) {
  emit playerStateChanged(PlayerInfo(p));
}

void MurmurDBus::playerConnected(Player *p) {
  emit playerConnected(PlayerInfo(p));
}

void MurmurDBus::playerDisconnected(Player *p) {
  emit playerDisconnected(PlayerInfo(p));
}

void MurmurDBus::channelStateChanged(Channel *c) {
  emit channelStateChanged(ChannelInfo(c));
}

void MurmurDBus::channelCreated(Channel *c) {
  emit channelCreated(ChannelInfo(c));
}

void MurmurDBus::channelRemoved(Channel *c) {
  emit channelRemoved(ChannelInfo(c));
}

