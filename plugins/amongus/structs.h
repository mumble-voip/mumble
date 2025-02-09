// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef AMONGUS_STRUCTS
#define AMONGUS_STRUCTS

#include <cstdint>

typedef uint32_t ptr_t;
typedef uint32_t il2cpp_array_size_t; // uintptr_t

enum class DisconnectReason {
	ExitGame            = 0x0,
	GameFull            = 0x1,
	GameStarted         = 0x2,
	GameNotFound        = 0x3,
	IncorrectVersion    = 0x5,
	Banned              = 0x6,
	Kicked              = 0x7,
	Custom              = 0x8,
	InvalidName         = 0x9,
	Hacking             = 0xA,
	Destroy             = 0x10,
	Error               = 0x11,
	IncorrectGame       = 0x12,
	ServerRequest       = 0x13,
	ServerFull          = 0x14,
	IntentionalLeaving  = 0xD0,
	FocusLostBackground = 0xCF,
	FocusLost           = 0xD1,
	NewConnection       = 0xD2
};

enum class DiscoverState { Off, Broadcast };

enum class GameMode { LocalGame, OnlineGame, FreePlay };

enum class GameState { NotJoined, Joined, Started, Ended };

enum class Mode { None, Client, HostAndClient };

struct Il2CppType {
	ptr_t data;
	uint32_t bits;
};

struct Il2CppObject {
	ptr_t klass;
	ptr_t monitor;
};

struct Il2CppClass_1 {
	ptr_t image;
	ptr_t gcDesc;
	ptr_t name;
	ptr_t namespaze;
	Il2CppType byvalArg;
	Il2CppType thisArg;
	ptr_t elementClass;
	ptr_t castClass;
	ptr_t declaringType;
	ptr_t parent;
	ptr_t genericClass;
	ptr_t typeMetadataHandle;
	ptr_t interopData;
	ptr_t klass;
	ptr_t fields;
	ptr_t events;
	ptr_t properties;
	ptr_t methods;
	ptr_t nestedTypes;
	ptr_t implementedInterfaces;
	ptr_t interfaceOffsets;
};

struct Dictionary_Item {
	int32_t hashCode;
	int32_t next;
	ptr_t key;
	ptr_t value;
};

struct Dictionary_Array {
	Il2CppObject obj;
	ptr_t bounds;
	il2cpp_array_size_t maxLength;
	Dictionary_Item items[1];
};

struct Dictionary_Fields {
	ptr_t buckets;
	ptr_t entries;
	int32_t count;
	int32_t version;
	int32_t freeList;
	int32_t freeCount;
	ptr_t comparer;
	ptr_t keys;
	ptr_t values;
	ptr_t syncRoot;
};

struct Dictionary_o {
	ptr_t klass;
	ptr_t monitor;
	Dictionary_Fields fields;
};

struct UnityEngine_Vector2_Fields {
	float x;
	float y;
};

struct UnityEngine_Vector2_o {
	UnityEngine_Vector2_Fields fields;
};

struct UnityEngine_Vector3_Fields {
	float x;
	float y;
	float z;
};

struct UnityEngine_Vector3_o {
	UnityEngine_Vector3_Fields fields;
};

struct String_Fields {
	int32_t length;
	// char16_t string[];
};

struct String_o {
	ptr_t klass;
	ptr_t monitor;
	String_Fields fields;
};

struct UnityEngine_Object_Fields {
	ptr_t cachedPtr;
};

struct InnerNet_InnerNetClient_Fields : UnityEngine_Object_Fields {
	float minSendInterval;
	uint32_t netIdCnt;
	float timer;
	ptr_t spawnableObjects;
	ptr_t nonAddressableSpawnableObjects;
	bool inOnlineScene;
	ptr_t destroyedObjects;
	ptr_t allObjects;
	ptr_t allObjectsFast;
	ptr_t streams;
	int32_t msgNum;
	ptr_t networkAddress;
	int32_t networkPort;
	bool useDTLS;
	ptr_t connection;
	Mode mode;
	GameMode gameMode;
	int32_t gameId;
	int32_t hostId;
	int32_t clientId;
	ptr_t allClients;
	ptr_t recentClients;
	DisconnectReason lastDisconnectReason;
	ptr_t lastCustomDisconnect;
	uint8_t lastServerChatMode;
	ptr_t preSpawnDispatcher;
	ptr_t dispatcher;
	bool isGamePublic;
	GameState gameState;
	bool isConnecting;
	bool platformSpecificsChecked;
	ptr_t tempQueue;
	bool appPaused;
};

struct InnerNet_InnerNetObject_Fields : UnityEngine_Object_Fields {
	uint32_t spawnId;
	uint32_t netId;
	uint32_t dirtyBits;
	uint8_t spawnFlags;
	uint8_t sendMode;
	int32_t ownerId;
	bool despawnOnDestroy;
};

struct CustomNetworkTransform_Fields : InnerNet_InnerNetObject_Fields {
	ptr_t xRange;
	ptr_t yRange;
	float sendInterval;
	float snapThreshold;
	float interpolateMovement;
	ptr_t body;
	UnityEngine_Vector2_o targetSyncPosition;
	UnityEngine_Vector2_o targetSyncVelocity;
	uint16_t lastSequenceId;
	UnityEngine_Vector2_o prevPosSent;
	UnityEngine_Vector2_o prevVelSent;
};

struct CustomNetworkTransform_o {
	ptr_t klass;
	ptr_t monitor;
	CustomNetworkTransform_Fields fields;
};

struct AmongUsClient_Fields : InnerNet_InnerNetClient_Fields {
	ptr_t onlineScene;
	ptr_t mainMenuScene;
	ptr_t gameDataPrefab;
	ptr_t playerPrefab;
	ptr_t shipPrefabs;
	int32_t tutorialMapId;
	float spawnRadius;
	DiscoverState discoverState;
	ptr_t disconnectHandlers;
	ptr_t gameListHandlers;
	int32_t crossplayPrivilegeError;
};

struct AmongUsClient_StaticFields {
	ptr_t instance;
};

struct AmongUsClient_c {
	Il2CppClass_1 _1;
	ptr_t staticFields;
};

struct AmongUsClient_o {
	ptr_t klass;
	ptr_t monitor;
	AmongUsClient_Fields fields;
};

struct PlayerControl_Fields : InnerNet_InnerNetObject_Fields {
	int32_t lastStartCounter;
	uint8_t playerId;
	ptr_t friendCode;
	ptr_t puid;
	float maxReportDistance;
	bool moveable;
	ptr_t bodySprites;
	ptr_t currentBodySprite;
	ptr_t normalBodySprite;
	int32_t currentOutfitType;
	bool inVent;
	bool protectedByGuardianThisRound;
	bool shapeshifting;
	ptr_t cachedData;
	bool protectedByGuardian;
	float flashlightAngle;
	ptr_t footSteps;
	ptr_t killSfx;
	ptr_t killAnimations;
	float killTimer;
	int32_t remainingEmergencies;
	ptr_t nameText;
	ptr_t lightPrefab;
	ptr_t myLight;
	ptr_t collider;
	ptr_t myPhysics;
	ptr_t netTransform;
	ptr_t currentPet;
	ptr_t hatRenderer;
	ptr_t visorSlot;
	ptr_t myAnim;
	ptr_t horseAnim;
	ptr_t hitBuffer;
	ptr_t myTasks;
	UnityEngine_Vector3_o defaultPlayerScale;
	ptr_t scannerAnims;
	ptr_t scannersImages;
	ptr_t currentRoleAnimations;
	ptr_t closest;
	bool isNew;
	bool isDummy;
	bool notRealPlayer;
	ptr_t cache;
	ptr_t itemsInRange;
	ptr_t newItemsInRange;
	uint8_t scannerCount;
	bool roleAssigned;
};

struct PlayerControl_StaticFields {
	ptr_t localPlayer;
	ptr_t gameOptions;
	ptr_t allPlayers;
};

struct PlayerControl_c {
	Il2CppClass_1 _1;
	ptr_t staticFields;
};

struct PlayerControl_o {
	ptr_t klass;
	ptr_t monitor;
	PlayerControl_Fields fields;
};

struct GameData_PlayerOutfit_Fields {
	bool dontCensorName;
	int32_t colorId;
	ptr_t hatId;
	ptr_t petId;
	ptr_t skinId;
	ptr_t visorId;
	ptr_t namePlateId;
	ptr_t playerName;
	ptr_t preCensorName;
	ptr_t postCensorName;
};

struct GameData_PlayerOutfit_o {
	ptr_t klass;
	ptr_t monitor;
	GameData_PlayerOutfit_Fields fields;
};

struct GameData_PlayerInfo_Fields {
	uint8_t playerId;
	ptr_t friendCode;
	ptr_t puid;
	ptr_t outfits;
	uint32_t playerLevel;
	bool disconnected;
	ptr_t role;
	ptr_t tasks;
	bool isDead;
	ptr_t object;
};

struct GameData_PlayerInfo_o {
	ptr_t klass;
	ptr_t monitor;
	GameData_PlayerInfo_Fields fields;
};

#endif
