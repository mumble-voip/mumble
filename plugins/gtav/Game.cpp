// Copyright 2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Game.h"

#include "mumble_positional_audio_utils.h"

#include <cstring>

Game::Game(const procid_t id, const std::string &name) : m_proc((id), name) {
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
	// Find out the offset of the memory region
	const MemoryRegions &regions = iter->second.regions();
	const auto &r                = regions.cbegin();
	if (r != regions.cend()) {
		m_moduleBase = r->address;
	}

	// Check if we can get meaningful data from it
	const std::string expected_name = "Grand Theft Auto V";
	if (m_proc.peekString(m_moduleBase + 0x1901130, expected_name.length()) == expected_name) {
		// Steam version
		m_stateAddr      = m_moduleBase + 0x291B2F0;
		m_avatarPosAddr  = m_moduleBase + 0x1D1D850;
		m_cameraPosAddr  = m_moduleBase + 0x1DACEB0;
		m_avatarDirAddr  = m_moduleBase + 0x20476D0;
		m_avatarAxisAddr = m_moduleBase + 0x20476A0;
		m_cameraDirAddr  = m_moduleBase + 0x1FA7A80;
		m_cameraAxisAddr = m_moduleBase + 0x1FA7A90;
		m_playerAddr     = m_moduleBase + 0x2928674;
		m_vehicleAddr    = m_moduleBase + 0x2503F10;
		m_locationAddr   = m_moduleBase + 0x25038CB;
		m_streetAddr     = m_moduleBase + 0x2500600;
	} else if (m_proc.peekString(0x180D4D8, expected_name.length()) == expected_name) {
		// Retail version
		m_stateAddr      = m_moduleBase + 0x2733490;
		m_avatarPosAddr  = m_moduleBase + 0x1F7EAA0;
		m_cameraPosAddr  = m_moduleBase + 0x1C58630;
		m_avatarBaseAddr = m_moduleBase + 0x1B956C0;
		// Avatar pointer
		procptr_t avatarBase = m_proc.peekPtr(m_avatarBaseAddr);
		if (!avatarBase)
			return MUMBLE_PDEC_ERROR_TEMP;
		m_avatarDirAddr  = avatarBase + 0x70;
		m_avatarAxisAddr = avatarBase + 0x80;
		m_cameraDirAddr  = m_moduleBase + 0x1C5A0F0;
		m_cameraAxisAddr = m_moduleBase + 0x1F7D9F0;
		m_playerAddr     = m_moduleBase + 0x273DBAC;
		m_vehicleAddr    = m_moduleBase + 0x2331890;
		m_locationAddr   = m_moduleBase + 0x233125B;
		m_streetAddr     = m_moduleBase + 0x232DFA0;
	} else {
		// Return permanent error on unknown version
		return MUMBLE_PDEC_ERROR_PERM;
	}

	// Check if we can get meaningful data from it
	float apos[3], afront[3], atop[3], cpos[3], cfront[3], ctop[3];
	const char *identity = "";
	const char *context  = "";

	return fetchPositionalData(apos, afront, atop, cpos, cfront, ctop, &context, &identity);
}

Mumble_PositionalDataErrorCode Game::fetchPositionalData(float *avatarPos, float *avatarDir, float *avatarAxis,
														 float *cameraPos, float *cameraDir, float *cameraAxis,
														 const char **contextPtr, const char **identityPtr) {
	for (uint8_t i = 0; i < 3; ++i) {
		avatarPos[i] = avatarDir[i] = avatarAxis[i] = cameraPos[i] = cameraDir[i] = cameraAxis[i] = 0.0f;
	}

	// Boolean value to check if game addresses retrieval is successful
	bool ok;
	// Char values for extra features
	char state, player[50], vehicle[50], location[50], street[50];

	// Peekproc and assign game addresses to our containers, so we can retrieve positional data
	ok = m_proc.peek(m_stateAddr, &state, 1)
		 && // Magical state value: 0 when in single player, 2 when online and 3 when in a lobby.
		 m_proc.peek(m_avatarPosAddr, avatarPos, 12) &&   // Avatar Position values (X, Z and Y).
		 m_proc.peek(m_cameraPosAddr, cameraPos, 12) &&   // Camera Position values (X, Z and Y).
		 m_proc.peek(m_avatarDirAddr, avatarDir, 12) &&   // Avatar Front Vector values (X, Z and Y).
		 m_proc.peek(m_avatarAxisAddr, avatarAxis, 12) && // Avatar Top Vector values (X, Z and Y).
		 m_proc.peek(m_cameraDirAddr, cameraDir, 12) &&   // Camera Front Vector values (X, Z and Y).
		 m_proc.peek(m_cameraAxisAddr, cameraAxis, 12) && // Camera Top Vector values (X, Z and Y).
		 m_proc.peek(m_playerAddr, player) &&             // Player nickname.
		 m_proc.peek(m_vehicleAddr, vehicle) &&           // Vehicle name if in a vehicle, empty if not.
		 m_proc.peek(m_locationAddr, location) &&         // Location name.
		 m_proc.peek(m_streetAddr, street);               // Street name if on a street, empty if not.

	// This prevents the plugin from linking to the game in case something goes wrong during values retrieval from
	// memory addresses.
	if (!ok)
		return MUMBLE_PDEC_ERROR_TEMP;

	// State
	if (state != 2) {      // If not in-game
		*contextPtr  = ""; // Clear contextPtr
		*identityPtr = ""; // Clear identityPtr
		return MUMBLE_PDEC_ERROR_TEMP;
	}

	// Begin identity
	std::stringstream oidentity;
	oidentity << "{";

	// Player
	escape(player, sizeof(player));
	if (strcmp(player, "") != 0) {
		oidentity << std::endl;
		oidentity << "\"Player\": \"" << player << "\","; // Set player nickname in identity.
	} else {
		oidentity << std::endl << "\"Player\": null,";
	}

	// Vehicle
	escape(vehicle, sizeof(vehicle));
	if (strcmp(vehicle, "") != 0) {
		oidentity << std::endl;
		oidentity << "\"Vehicle\": \"" << vehicle << "\","; // Set vehicle name in identity.
	} else {
		oidentity << std::endl << "\"Vehicle\": null,";
	}

	// Location
	escape(location, sizeof(location));
	if (strcmp(location, "") != 0) {
		oidentity << std::endl;
		oidentity << "\"Location\": \"" << location << "\","; // Set location name in identity.
	} else {
		oidentity << std::endl << "\"Location\": null,";
	}

	// Street
	escape(street, sizeof(street));
	if (strcmp(street, "") != 0) {
		oidentity << std::endl;
		oidentity << "\"Street\": \"" << street << "\""; // Set street name in identity.
	} else {
		oidentity << std::endl << "\"Street\": null";
	}

	oidentity << std::endl << "}";
	m_identity   = oidentity.str();
	*identityPtr = m_identity.c_str();
	// End identity

	return MUMBLE_PDEC_OK;
}
