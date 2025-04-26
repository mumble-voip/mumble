// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef AMONGUS_GAME
#define AMONGUS_GAME

#include "structs.h"

#include "ProcessWindows.h"
#define MUMBLE_PLUGIN_NO_DEFAULT_FUNCTION_DEFINITIONS
#include "MumblePlugin.h"
#undef MUMBLE_PLUGIN_NO_DEFAULT_FUNCTION_DEFINITIONS

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
	GameData_PlayerOutfit_Fields playerOutfitFields(const GameData_PlayerInfo_Fields &fields);

	std::string string(const procptr_t address);

	const std::string &context(const AmongUsClient_Fields &fields);
	const std::string &identity(const AmongUsClient_Fields &fields, const PlayerControl_Fields &controlFields);

	Game(const procid_t id, const std::string name);

protected:
	ptr_t m_client;
	ptr_t m_playerControlStaticFields;
	std::string m_context;
	std::string m_identity;
	ProcessWindows m_proc;
};

#endif
