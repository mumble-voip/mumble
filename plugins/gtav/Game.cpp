// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Game.h"

#include <sstream>

Game::Game(const procid_t id, const std::string &name) : m_proc(id, name) {
}

bool Game::setupPointers(const Module &module) {
	// 48 8B 0D ?? ?? ?? ??    mov    rcx, cs:off_????????
	const std::vector< uint8_t > playerMgrPattern = { 0x48, 0x8B, 0x0D, '?', '?',  '?',  '?',  0x8A, 0xD3, 0x48, 0x8B,
													  0x01, 0xFF, 0x50, '?', 0x4C, 0x8B, 0x07, 0x48, 0x8B, 0xCF };

	m_playerMgr = m_proc.findPattern(playerMgrPattern, module);
	if (!m_playerMgr) {
		return false;
	}

	// 48 8B 05 ?? ?? ?? ??    mov    rax, cs:qword_????????
	const std::vector< uint8_t > cameraMgrPattern = { 0x48, 0x8B, 0x05, '?', '?', '?', '?', 0x4A, 0x8B, 0x1C, 0xF0 };
	m_cameraMgr                                   = m_proc.findPattern(cameraMgrPattern, module);
	if (!m_cameraMgr) {
		return false;
	}

	m_playerMgr = m_proc.peekPtr(m_proc.peekRIP(m_playerMgr + 3));
	m_cameraMgr = m_proc.peekRIP(m_cameraMgr + 3);

	return true;
}

Mumble_PositionalDataErrorCode Game::init() {
	if (!m_proc.isOk()) {
		return MUMBLE_PDEC_ERROR_TEMP;
	}

	const Modules &modules = m_proc.modules();
	const auto iter        = modules.find("GTA5.exe");
	if (iter == modules.cend()) {
		return MUMBLE_PDEC_ERROR_TEMP;
	}

	if (!setupPointers(iter->second)) {
		// TODO: Return MUMBLE_PDEC_ERROR_PERM.
		// The game process is spawned by its launcher, but it takes at least a few seconds to unpack.
		// We have to figure out a way to detect that state.
		return MUMBLE_PDEC_ERROR_TEMP;
	}

	if (!m_playerMgr || !m_cameraMgr) {
		return MUMBLE_PDEC_ERROR_TEMP;
	}

	return MUMBLE_PDEC_OK;
}

CPlayerAngles Game::playerAngles() const {
	const auto gameCameraAngles    = m_proc.peek< CGameCameraAngles >(m_cameraMgr);
	const auto cameraManagerAngles = m_proc.peek< CCameraManagerAngles >(gameCameraAngles.cameraManagerAngles);
	const auto cameraAngles        = m_proc.peek< CCameraAngles >(cameraManagerAngles.cameraAngles);

	return m_proc.peek< CPlayerAngles >(cameraAngles.playerAngles);
}

const std::string &Game::identity(const CNetGamePlayer &player, const CPlayerInfo &info, const CPed &entity) {
	std::ostringstream stream;

	stream << "ID: " << std::to_string(player.id) << '\n';
	stream << "Name: " << info.netData.name << '\n';
	stream << "Health: " << entity.health << '\n';
	stream << "Max health: " << entity.maxHealth << '\n';
	stream << "Armor: " << entity.armor << '\n';
	stream << "Wanted level: " << info.wantedLevel << '\n';
	stream << "State: "
		   << std::to_string(static_cast< std::underlying_type_t< decltype(info.gameState) > >(info.gameState)) << '\n';
	stream << "Peer ID: " << std::to_string(info.netData.peerID) << '\n';
	stream << "Host token: " << std::to_string(info.netData.hostToken);

	m_identity = stream.str();

	return m_identity;
}
