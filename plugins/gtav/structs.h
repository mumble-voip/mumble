// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef GTAV_STRUCTS
#define GTAV_STRUCTS

#include <array>
#include <cstdint>

// Reference: https://github.com/Yimura/GTAV-Classes

using ptr_t = uint64_t;

enum class GameState : int32_t {
	Invalid = -1,
	Playing,
	Died,
	Arrested,
	FailedMission,
	LeftGame,
	Respawn,
	InMPCutscene
};

enum class ModelType : uint8_t { Unk0, Object, Unk1, Unk2, Weapon, Vehicle, Ped, Plant = 129 };

// Rockstar Advanced Game Engine (RAGE).
namespace rage {
template< typename T > using vector3 = std::array< T, 3 >;

using fvector3 = vector3< float >;

struct netAddress {
	uint8_t field4;
	uint8_t field3;
	uint8_t field2;
	uint8_t field1;
};

struct netPlayerData {
	uint8_t pad1[8];
	uint64_t rockstarID;
	uint8_t pad2[52];
	netAddress relayIP;
	uint16_t relayPort;
	uint8_t pad3[2];
	netAddress externalIP;
	uint16_t externalPort;
	uint8_t pad4[2];
	netAddress internalIP;
	uint16_t internalPort;
	uint8_t pad5[6];
	uint64_t hostToken;
	uint64_t peerID;
	uint64_t rockstarID2;
	uint8_t pad6[12];
	char name[20];
};

#pragma pack(push, 4)
struct fwEntity {
	uint8_t pad1[32];
	ptr_t modelInfo; // CBaseModelInfo *
	uint8_t pad2[1];
	int8_t entityType;
	uint8_t pad3[2];
	uint8_t invisible;
	uint8_t pad4[3];
	ptr_t navigation; // CNavigation *
	uint8_t pad5[16];
	ptr_t drawData; // rage::fwDrawData *
	uint8_t pad6[16];
	rage::fvector3 right;
	uint8_t pad7[4];
	rage::fvector3 forward;
	uint8_t pad8[4];
	rage::fvector3 up;
	uint8_t pad9[4];
	rage::fvector3 position;
	uint8_t pad10[52];
	ptr_t netObject; // rage::netObject *
	uint8_t pad11[176];
	uint32_t damageBits;
};
#pragma pack(pop)
} // namespace rage

#pragma pack(push, 4)
struct CPed : public rage::fwEntity {
	uint8_t hostility;
	uint8_t pad1[243];
	float health;
	uint8_t pad2[28];
	float maxHealth;
	uint8_t pad3[124];
	rage::fvector3 velocity;
	uint8_t pad4[2564];
	ptr_t vehicle; // CAutomobile *
	uint8_t pad5[912];
	ptr_t playerInfo; // CPlayerInfo *
	uint8_t pad6[8];
	ptr_t weaponManager; // CPedWeaponManager *
	uint8_t pad7[907];
	uint8_t pedTaskFlag;
	uint8_t pad8[196];
	float armor;
};
#pragma pack(pop)

struct CGameCameraAngles {
	ptr_t cameraManagerAngles; // CCameraManagerAngles *
	uint8_t pad[56];
};

struct CCameraManagerAngles {
	ptr_t cameraAngles; // CCameraAngles *
};

struct alignas(8) CCameraAngles {
	uint8_t pad1[960];
	ptr_t playerAngles; // CPlayerAngles *
	uint8_t pad2[60];
};

struct CPlayerAngles {
	uint8_t pad1[16];
	ptr_t camData; // CPlayerCameraData *
	uint8_t pad2[24];
	rage::fvector3 right;
	uint8_t pad3[4];
	rage::fvector3 forward;
	uint8_t pad4[4];
	rage::fvector3 up;
	uint8_t pad5[4];
	rage::fvector3 position;
	uint8_t pad6[36];
};

struct CPlayerInfo {
	uint8_t pad1[32];
	rage::netPlayerData netData;
	uint8_t pad2[184];
	float swimSpeed;
	uint8_t pad3[20];
	uint32_t waterProof;
	uint8_t pad4[76];
	GameState gameState;
	uint8_t pad5[12];
	ptr_t ped; // CPed *
	uint8_t pad6[40];
	uint32_t frameFlags;
	uint8_t pad7[52];
	uint32_t playerControls;
	uint8_t pad8[1256];
	float wantedCanChange;
	uint8_t pad9[304];
	uint32_t npcIgnore;
	uint8_t pad10[12];
	bool isWanted;
	uint8_t pad11[7];
	uint32_t wantedLevel;
	uint32_t wantedLevelDisplay;
	uint8_t pad12[1120];
	float runSpeed;
	float stamina;
	float staminaRegen;
	uint8_t pad13[16];
	float weaponDamageMult;
	float weaponDefenceMult;
	uint8_t pad14[4];
	float meleeWeaponDamageMult;
	float meleeDamageMult;
	float meleeDefenceMult;
	uint8_t pad15[8];
	float meleeWeaponDefenceMult;
};

#pragma pack(push, 1)
struct CNetGamePlayer {
	ptr_t vtable;
	uint8_t pad1[8];
	ptr_t nonPhysicalData; // CNonPhysicalPlayerData *
	uint32_t msgID;
	uint8_t pad2[4];
	uint8_t activeID;
	uint8_t id;
	uint8_t pad3[3];
	uint16_t complaints;
	uint8_t pad4[17];
	ptr_t unknownList[10]; // CNetGamePlayer *
	uint8_t pad5[24];
	ptr_t info; // CPlayerInfo *
};
#pragma pack(pop)

#pragma pack(push, 2)
struct CNetworkPlayerMgr {
	ptr_t vtable;
	uint8_t pad1[224];
	ptr_t player; // CNetGamePlayer *
	uint8_t pad2[144];
	ptr_t players[32]; // CNetGamePlayer *
	uint16_t playersMax;
	uint8_t pad_0282[10];
	uint16_t playersCount;
};
#pragma pack(pop)

static_assert(sizeof(rage::netAddress) == 0x4, "");
static_assert(sizeof(rage::netPlayerData) == 0x98, "");
static_assert(sizeof(rage::fwEntity) == 0x18C, "");

static_assert(sizeof(CPed) == 0x1534, "");
static_assert(sizeof(CGameCameraAngles) == 0x40, "");
static_assert(sizeof(CCameraManagerAngles) == 0x8, "");
static_assert(sizeof(CCameraAngles) == 0x408, "");
static_assert(sizeof(CPlayerAngles) == 0x90, "");
static_assert(sizeof(CPlayerInfo) == 0xD30, "");
static_assert(sizeof(CNetGamePlayer) == 0xA8, "");
static_assert(sizeof(CNetworkPlayerMgr) == 0x28E, "");

#endif
