// Copyright 2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef GTAV_GAME_H_
#define GTAV_GAME_H_

#include "ProcessWindows.h"
#include "PluginComponents_v_1_0_x.h"

class Game {
public:
	Game(const procid_t id, const std::string &name);

	Mumble_PositionalDataErrorCode init();
	Mumble_PositionalDataErrorCode fetchPositionalData(float *avatarPos, float *avatarDir, float *avatarAxis,
													   float *cameraPos, float *cameraDir, float *cameraAxis,
													   const char **contextPtr, const char **identityPtr);

protected:
	ProcessWindows m_proc;
	procptr_t m_moduleBase = 0;
	std::string m_identity;

	// Memory addresses
	procptr_t m_stateAddr, m_avatarPosAddr, m_cameraPosAddr, m_avatarBaseAddr, m_avatarDirAddr, m_avatarAxisAddr,
		m_cameraDirAddr, m_cameraAxisAddr, m_playerAddr, m_vehicleAddr, m_locationAddr, m_streetAddr;
};

#endif
