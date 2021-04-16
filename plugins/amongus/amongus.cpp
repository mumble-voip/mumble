// Copyright 2020-2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Game.h"

#define MUMBLE_ALLOW_DEPRECATED_LEGACY_PLUGIN_API
#include "../mumble_legacy_plugin.h"

#include "../mumble_positional_audio_utils.h"

#include <sstream>

std::unique_ptr< Game > game;

static inline bool inGame(const AmongUsClient_Fields &fields) {
	if (fields.gameMode != GameMode::FreePlay && fields.gameState != GameState::NotJoined) {
		return true;
	}

	return false;
}

static int fetch(float *avatarPos, float *avatarFront, float *avatarTop, float *cameraPos, float *cameraFront,
				 float *cameraTop, std::string &context, std::wstring &identity) {
	for (uint8_t i = 0; i < 3; ++i) {
		avatarPos[i] = avatarFront[i] = avatarTop[i] = cameraPos[i] = cameraFront[i] = cameraTop[i] = 0.f;
	}

	const auto fields = game->clientFields();

	if (!inGame(fields)) {
		return false;
	}

	if (fields.gameState == GameState::Ended) {
		return true;
	}

	const auto playerControlFields = game->playerControlFields();

	// Mumble | Game
	// X      | X
	// Y      | -
	// Z      | Y
	const auto position = game->playerPosition(playerControlFields.netTransform);
	avatarPos[0] = cameraPos[0] = position.x;
	avatarPos[2] = cameraPos[2] = position.y;

	// Context
	std::ostringstream stream;
	stream << " {\"Game ID\": " << fields.gameId << "}";
	context = stream.str();

	stream.str(std::string());
	stream.clear();

	// Identity
	const auto playerFields = game->playerInfoFields(playerControlFields.cachedData);

	stream << "Name: " << game->string(playerFields.playerName) << '\n';
	stream << "Client ID: " << std::to_string(fields.clientId) << '\n';
	stream << "Player ID: " << std::to_string(playerFields.playerId) << '\n';
	stream << "Color ID: " << std::to_string(playerFields.colorId) << '\n';
	stream << "Skin ID: " << std::to_string(playerFields.skinId) << '\n';
	stream << "Hat ID: " << std::to_string(playerFields.hatId) << '\n';
	stream << "Pet ID: " << std::to_string(playerFields.petId) << '\n';
	stream << "Dead: " << (playerFields.isDead ? "true" : "false") << '\n';
	stream << "Host ID: " << std::to_string(fields.hostId) << '\n';
	stream << "Public game: " << (fields.isGamePublic ? "true" : "false");

	identity = utf8ToUtf16(stream.str());

	return true;
}

static int tryLock(const std::multimap< std::wstring, unsigned long long int > &pids) {
	const std::string name = "Among Us.exe";
	const auto id          = Process::find(name, pids);
	if (!id) {
		return false;
	}

	game = std::make_unique< Game >(id, name);
	if (!game->isOk()) {
		game.reset();
		return false;
	}

	const auto fields = game->clientFields();
	if (!inGame(fields)) {
		game.reset();
		return false;
	}

	return true;
}

static const std::wstring longDesc() {
	return std::wstring(L"Supports Among Us with context and identity support.");
}

static std::wstring description(L"Among Us");
static std::wstring shortName(L"Among Us");

static int tryLock1() {
	return tryLock(std::multimap< std::wstring, unsigned long long int >());
}

static void nullUnlock() {
}

static MumblePlugin amongusPlug = { MUMBLE_PLUGIN_MAGIC, description, shortName, nullptr, nullptr, tryLock1,
									nullUnlock,          longDesc,    fetch };

static MumblePlugin2 amongusPlug2 = { MUMBLE_PLUGIN_MAGIC_2, MUMBLE_PLUGIN_VERSION, tryLock };

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin *getMumblePlugin() {
	return &amongusPlug;
}

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin2 *getMumblePlugin2() {
	return &amongusPlug2;
}
