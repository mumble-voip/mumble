-- Copyright (C) 2005-2007, Thorvald Natvig <thorvald@natvig.com>
--
--   All rights reserved.
--
--   Redistribution and use in source and binary forms, with or without
--   modification, are permitted provided that the following conditions
--   are met:
--
--   - Redistributions of source code must retain the above copyright notice,
--     this list of conditions and the following disclaimer.
--   - Redistributions in binary form must reproduce the above copyright notice,
--     this list of conditions and the following disclaimer in the documentation
--     and/or other materials provided with the distribution.
--   - Neither the name of the Mumble Developers nor the names of its
--     contributors may be used to endorse or promote products derived from this
--     software without specific prior written permission.
--
--   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
--   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
--   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
--   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
--   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
--   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
--   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
--   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
--   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
--   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
--   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

DROP TABLE IF EXISTS bans;
DROP TABLE IF EXISTS channel_links;
DROP TABLE IF EXISTS acl;
DROP TABLE IF EXISTS groups;
DROP TABLE IF EXISTS channels;
DROP TABLE IF EXISTS player_auth;
DROP TABLE IF EXISTS players;

CREATE TABLE players (player_id INTEGER PRIMARY KEY AUTO_INCREMENT, name varchar(255), email varchar(255), pw varchar(255), lastchannel INTEGER) Type=InnoDB;
CREATE UNIQUE INDEX players_name ON players (name);

CREATE TABLE player_auth (player_auth_id INTEGER PRIMARY KEY AUTO_INCREMENT, name varchar(255), pw varchar(255), email varchar(255), authcode varchar(255)) Type=InnoDB;
CREATE UNIQUE INDEX player_auth_name ON player_auth(name);
CREATE UNIQUE INDEX player_auth_code ON player_auth(authcode);

CREATE TABLE channels (channel_id INTEGER PRIMARY KEY AUTO_INCREMENT, parent_id INTEGER, name varchar(255), inheritACL INTEGER) Type=InnoDB;
CREATE UNIQUE INDEX channels_bugged_mysql ON channels(parent_id, name);
ALTER TABLE channels ADD CONSTRAINT channel_del_channel FOREIGN KEY (parent_id) REFERENCES channels(channel_id) ON DELETE CASCADE;

CREATE TABLE groups (group_id INTEGER PRIMARY KEY AUTO_INCREMENT, name varchar(255), channel_id INTEGER, inherit INTEGER, inheritable INTEGER) Type=InnoDB;
CREATE UNIQUE INDEX groups_name_channels ON groups(name, channel_id);
ALTER TABLE groups ADD CONSTRAINT groups_del_channel FOREIGN KEY (channel_id) REFERENCES channels(channel_id) ON DELETE CASCADE;

DROP TABLE IF EXISTS group_members;
CREATE TABLE group_members (group_members_id INTEGER PRIMARY KEY AUTO_INCREMENT, group_id INTEGER, player_id INTEGER, addit INTEGER) Type=InnoDB;
ALTER TABLE group_members ADD CONSTRAINT groups_members_del_group FOREIGN KEY (group_id) REFERENCES groups(group_id) ON DELETE CASCADE;
ALTER TABLE group_members ADD CONSTRAINT groups_members_del_player FOREIGN KEY (player_id) REFERENCES players(player_id) ON DELETE CASCADE;

CREATE TABLE acl (acl_id INTEGER PRIMARY KEY AUTO_INCREMENT, channel_id INTEGER, priority INTEGER, player_id INTEGER, group_name varchar(255), apply_here INTEGER, apply_sub INTEGER, grantpriv INTEGER, revokepriv INTEGER) Type=InnoDB;
CREATE UNIQUE INDEX acl_channel_pri ON acl(channel_id, priority);
ALTER TABLE acl ADD CONSTRAINT acl_del_channel FOREIGN KEY (channel_id) REFERENCES channels(channel_id) ON DELETE CASCADE;
ALTER TABLE acl ADD CONSTRAINT acl_del_player FOREIGN KEY (player_id) REFERENCES players(player_id) ON DELETE CASCADE;

CREATE TABLE channel_links (channel_links_id INTEGER PRIMARY KEY AUTO_INCREMENT, channel_id INTEGER, link_id INTEGER) Type=InnoDB;
ALTER TABLE channel_links ADD CONSTRAINT channel_links_del_channel FOREIGN KEY (channel_id) REFERENCES channels(channel_id) ON DELETE CASCADE;

CREATE TABLE bans (ban_id INTEGER PRIMARY KEY AUTO_INCREMENT, base INTEGER, mask INTEGER) Type=InnoDB;
