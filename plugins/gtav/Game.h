// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef GTAV_GAME_H_
#define GTAV_GAME_H_

#include "structs.h"

#include "ProcessWindows.h"
#define MUMBLE_PLUGIN_NO_DEFAULT_FUNCTION_DEFINITIONS
#include "MumblePlugin.h"
#undef MUMBLE_PLUGIN_NO_DEFAULT_FUNCTION_DEFINITIONS

class Game {
public:
	Mumble_PositionalDataErrorCode init();

	static constexpr bool isMultiplayer(const CNetworkPlayerMgr &mgr) { return mgr.player; }

	CNetworkPlayerMgr playerMgr() const { return m_proc.peek< CNetworkPlayerMgr >(m_playerMgr); }

	CNetGamePlayer player(const CNetworkPlayerMgr &manager) const {
		return m_proc.peek< CNetGamePlayer >(manager.player);
	}

	CPlayerInfo playerInfo(const CNetGamePlayer &player) const { return m_proc.peek< CPlayerInfo >(player.info); }

	CPed playerEntity(const CPlayerInfo &info) const { return m_proc.peek< CPed >(info.ped); }

	CPlayerAngles playerAngles() const;

	const std::string &identity(const CNetGamePlayer &player, const CPlayerInfo &info, const CPed &entity);

	Game(const procid_t id, const std::string &name);

protected:
	bool setupPointers(const Module &module);

	ptr_t m_playerMgr;
	ptr_t m_cameraMgr;
	std::string m_identity;
	ProcessWindows m_proc;
};

#endif
