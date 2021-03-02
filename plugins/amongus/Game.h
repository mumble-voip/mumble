// Copyright 2020-2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef AMONGUS_GAME
#define AMONGUS_GAME

#include "structs.h"

#include "ProcessWindows.h"

class Game {
protected:
	bool m_ok;
	ptr_t m_client;
	ptr_t m_playerControlStaticFields;
	ProcessWindows m_proc;

public:
	inline bool isOk() const { return m_ok; }

	inline AmongUsClient_Fields clientFields() const { return m_proc.peek< AmongUsClient_o >(m_client).fields; }

	inline GameData_PlayerInfo_Fields playerInfoFields(const ptr_t cachedData) const {
		return m_proc.peek< GameData_PlayerInfo_o >(cachedData).fields;
	}

	inline UnityEngine_Vector2_Fields playerPosition(const ptr_t netTransform) const {
		const auto networkTransform = m_proc.peek< CustomNetworkTransform_o >(netTransform);
		return networkTransform.fields.prevPosSent.fields;
	}

	PlayerControl_Fields playerControlFields();

	std::string string(const procptr_t address);

	Game(const procid_t id, const std::string name);
};

#endif
