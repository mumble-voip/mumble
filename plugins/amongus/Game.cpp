// Copyright 2020-2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Game.h"

#include "mumble_plugin_utils.h"

Game::Game(const procid_t id, const std::string name) : m_ok(false), m_proc(id, name) {
	if (!m_proc.isOk()) {
		return;
	}

	const auto &modules = m_proc.modules();
	const auto iter     = modules.find("GameAssembly.dll");
	if (iter == modules.cend()) {
		return;
	}

	// 74 39             jz     short loc_????????
	// A1 ?? ?? ?? ??    mov    eax, AmongUsClient_c **
	// 8B 40 5C          mov    eax, [eax+5Ch]
	// 8B 00             mov    eax, [eax]
	const std::vector< uint8_t > clientPattern = { 0x74, 0x39, 0xA1, '?', '?', '?', '?', 0x8B, 0x40, 0x5C, 0x8B, 0x00 };
	m_client                                   = m_proc.findPattern(clientPattern, iter->second);

	if (!m_client) {
		return;
	}

	// +3 in order to skip to the memory address we actually care about
	m_client = m_proc.peekPtr(m_proc.peekPtr(m_client + 3));
	if (!m_client) {
		return;
	}

	const auto clientC = m_proc.peek< AmongUsClient_c >(m_client);
	if (!clientC.staticFields) {
		return;
	}

	const auto clientStaticFields = m_proc.peek< AmongUsClient_StaticFields >(clientC.staticFields);
	if (!clientStaticFields.instance) {
		return;
	}

	m_client = clientStaticFields.instance;

	const auto fields = clientFields();

	const auto playerControlO = m_proc.peek< PlayerControl_o >(fields.playerPrefab);
	if (!playerControlO.klass) {
		return;
	}

	const auto playerControlC = m_proc.peek< PlayerControl_c >(playerControlO.klass);
	if (!playerControlC.staticFields) {
		return;
	}

	m_playerControlStaticFields = playerControlC.staticFields;

	m_ok = true;
}

PlayerControl_Fields Game::playerControlFields() {
	const auto playerControlStaticFields = m_proc.peek< PlayerControl_StaticFields >(m_playerControlStaticFields);
	if (playerControlStaticFields.localPlayer) {
		return m_proc.peek< PlayerControl_o >(playerControlStaticFields.localPlayer).fields;
	}

	return {};
}

std::string Game::string(const procptr_t address) {
	const auto object = m_proc.peek< String_o >(address);

	std::u16string string;
	string.resize(object.fields.length);
	if (m_proc.peek(address + sizeof(object), &string[0], sizeof(char16_t) * string.size())) {
		return utf16ToUtf8(string.data());
	}

	return {};
}
