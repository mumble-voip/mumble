// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Game.h"

#include <sstream>

#include <utf8/cpp11.h>

Game::Game(const procid_t id, const std::string name) : m_proc(id, name) {
}

Mumble_PositionalDataErrorCode Game::init() {
	if (!m_proc.isOk()) {
		return MUMBLE_PDEC_ERROR_TEMP;
	}

	const auto &modules = m_proc.modules();
	const auto iter     = modules.find("GameAssembly.dll");
	if (iter == modules.cend()) {
		return MUMBLE_PDEC_ERROR_TEMP;
	}

	// 75 58             jnz    short loc_????????
	// A1 ?? ?? ?? ??    mov    eax, AmongUsClient_c **
	// 8B 40 5C          mov    eax, [eax+5Ch]
	const std::vector< uint8_t > clientPattern = { 0x75, 0x58, 0xA1, '?', '?', '?', '?', 0x8B, 0x40, 0x5C };
	m_client                                   = m_proc.findPattern(clientPattern, iter->second);

	if (!m_client) {
		return MUMBLE_PDEC_ERROR_PERM;
	}

	// +3 in order to skip to the memory address we actually care about
	m_client = m_proc.peekPtr(m_proc.peekPtr(m_client + 3));
	if (!m_client) {
		return MUMBLE_PDEC_ERROR_TEMP;
	}

	const auto clientC = m_proc.peek< AmongUsClient_c >(m_client);
	if (!clientC.staticFields) {
		return MUMBLE_PDEC_ERROR_TEMP;
	}

	const auto clientStaticFields = m_proc.peek< AmongUsClient_StaticFields >(clientC.staticFields);
	if (!clientStaticFields.instance) {
		return MUMBLE_PDEC_ERROR_TEMP;
	}

	m_client = clientStaticFields.instance;

	const auto fields = clientFields();

	const auto playerControlO = m_proc.peek< PlayerControl_o >(fields.playerPrefab);
	if (!playerControlO.klass) {
		return MUMBLE_PDEC_ERROR_TEMP;
	}

	const auto playerControlC = m_proc.peek< PlayerControl_c >(playerControlO.klass);
	if (!playerControlC.staticFields) {
		return MUMBLE_PDEC_ERROR_TEMP;
	}

	m_playerControlStaticFields = playerControlC.staticFields;

	return MUMBLE_PDEC_OK;
}

PlayerControl_Fields Game::playerControlFields() {
	const auto playerControlStaticFields = m_proc.peek< PlayerControl_StaticFields >(m_playerControlStaticFields);
	if (playerControlStaticFields.localPlayer) {
		return m_proc.peek< PlayerControl_o >(playerControlStaticFields.localPlayer).fields;
	}

	return {};
}

GameData_PlayerOutfit_Fields Game::playerOutfitFields(const GameData_PlayerInfo_Fields &fields) {
	const auto dictFields = m_proc.peek< Dictionary_o >(fields.outfits).fields;
	if (!dictFields.entries || dictFields.count < 1) {
		return {};
	}

	const auto array = m_proc.peek< Dictionary_Array >(dictFields.entries);

	return m_proc.peek< GameData_PlayerOutfit_o >(array.items[0].value).fields;
}

std::string Game::string(const procptr_t address) {
	const auto object = m_proc.peek< String_o >(address);

	std::u16string string;
	string.resize(object.fields.length);
	if (!m_proc.peek(address + sizeof(object), &string[0], sizeof(char16_t) * string.size())) {
		return {};
	}

	try {
		return utf8::utf16to8(string);
	} catch (const utf8::invalid_utf16 &) {
		return {};
	}
}

const std::string &Game::context(const AmongUsClient_Fields &fields) {
	std::ostringstream stream;

	stream << " {\"Game ID\": " << fields.gameId << "}";

	m_context = stream.str();

	return m_context;
}

const std::string &Game::identity(const AmongUsClient_Fields &fields, const PlayerControl_Fields &controlFields) {
	const GameData_PlayerInfo_Fields infoFields     = playerInfoFields(controlFields);
	const GameData_PlayerOutfit_Fields outfitFields = playerOutfitFields(infoFields);

	std::ostringstream stream;

	stream << "ID: " << std::to_string(infoFields.playerId) << '\n';
	stream << "Name: " << string(outfitFields.playerName) << '\n';
	stream << "Level: " << std::to_string(infoFields.playerLevel + 1) << '\n';
	stream << "Color ID: " << std::to_string(outfitFields.colorId) << '\n';
	stream << "Skin: " << string(outfitFields.skinId) << '\n';
	stream << "Hat: " << string(outfitFields.hatId) << '\n';
	stream << "Visor: " << string(outfitFields.visorId) << '\n';
	stream << "Pet: " << string(outfitFields.petId) << '\n';
	stream << "Nameplate: " << string(outfitFields.namePlateId) << '\n';
	stream << "Dead: " << (infoFields.isDead ? "true" : "false") << '\n';
	stream << "Client ID: " << std::to_string(fields.clientId) << '\n';
	stream << "Host ID: " << std::to_string(fields.hostId) << '\n';
	stream << "Public game: " << (fields.isGamePublic ? "true" : "false");

	m_identity = stream.str();

	return m_identity;
}
