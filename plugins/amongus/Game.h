// Copyright 2020-2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef AMONGUS_GAME
#define AMONGUS_GAME

#include "structs.h"

#include "ProcessWindows.h"
#include "PluginComponents_v_1_0_x.h"

class Game {
public:
	Mumble_PositionalDataErrorCode init();

	static inline bool isMultiplayer(const AmongUsClient_Fields &fields) {
		return fields.gameMode != GameMode::FreePlay && fields.gameState != GameState::NotJoined;
	}

	inline AmongUsClient_Fields clientFields() const { return m_proc.peek< AmongUsClient_o >(m_client).fields; }

	inline GameData_PlayerInfo_Fields playerInfoFields(const PlayerControl_Fields &fields) const {
		return m_proc.peek< GameData_PlayerInfo_o >(fields.cachedData).fields;
	}

	inline UnityEngine_Vector2_Fields playerPosition(const PlayerControl_Fields &fields) const {
		const auto networkTransform = m_proc.peek< CustomNetworkTransform_o >(fields.netTransform);
		return networkTransform.fields.prevPosSent.fields;
	}

	PlayerControl_Fields playerControlFields();

	std::string string(const procptr_t address);

	const std::string &context(const AmongUsClient_Fields &fields);
	const std::string &identity(const AmongUsClient_Fields &fields, const GameData_PlayerInfo_Fields &playerFields);

	Game(const uint64_t id, const std::string name);

protected:
	ptr_t m_client;
	ptr_t m_playerControlStaticFields;
	std::string m_context;
	std::string m_identity;
	ProcessWindows m_proc;
};

#endif
