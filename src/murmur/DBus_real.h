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

#ifndef _DBUS_H
#define _DBUS_H

#include "murmur_pch.h"

#include "Player.h"
#include "Channel.h"
#include "Group.h"
#include "ACL.h"

struct PlayerInfo
{
  int session;
  int id;
  QString name;
  bool mute, deaf, suppressed;
  bool selfMute, selfDeaf;
  int channel;
  PlayerInfo() { };
  PlayerInfo(Player *);
};
Q_DECLARE_METATYPE(PlayerInfo);


struct ChannelInfo
{
  int id;
  QString name;
  int parent;
  QList<int> links;
  ChannelInfo() { };
  ChannelInfo(Channel *c);
};
Q_DECLARE_METATYPE(ChannelInfo);

struct GroupInfo
{
  QString name;
  bool inherited, inherit, inheritable;
  QList<int> add, remove, members;
};
Q_DECLARE_METATYPE(GroupInfo);

struct ACLInfo
{
  bool applyHere, applySubs, inherited;
  int playerid;
  QString group;
  unsigned int allow, deny;
};
Q_DECLARE_METATYPE(ACLInfo);

class MurmurDBus : public QDBusAbstractAdaptor {
  Q_OBJECT
  Q_CLASSINFO("D-Bus Interface", "net.sourceforge.mumble.Murmur");
  public:
    MurmurDBus(QCoreApplication &application);

    void playerStateChanged(Player *p);
    void playerConnected(Player *p);
    void playerDisconnected(Player *p);

    void channelStateChanged(Channel *c);
    void channelCreated(Channel *c);
    void channelRemoved(Channel *c);
  public slots:
    QList<PlayerInfo> getPlayers();
    QList<ChannelInfo> getChannels();

  signals:
    void playerStateChanged(PlayerInfo);
    void playerConnected(PlayerInfo);
    void playerDisconnected(PlayerInfo);

    void channelStateChanged(ChannelInfo);
    void channelCreated(ChannelInfo);
    void channelRemoved(ChannelInfo);
};

extern MurmurDBus *dbus;

#else
class MurmurDBus;
#endif
