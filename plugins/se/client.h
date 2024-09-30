// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_PLUGIN_SE_CLIENT_
#define MUMBLE_MUMBLE_PLUGIN_SE_CLIENT_

struct TypeDescription {
	uint32_t fieldType;
	uint32_t fieldName;
	int32_t fieldOffset;
	uint16_t fieldSize;
	int16_t flags;
	uint32_t externalName;
	uint32_t saveRestoreOps;
	uint32_t inputFunc;
	uint32_t td;
	int32_t fieldSizeInBytes;
	uint32_t overrideField;
	int32_t overrideCount;
	float fieldTolerance;
	int32_t flatOffset[2];
	uint16_t flatGroup;
};

struct DataMap {
	uint32_t dataDesc;
	int32_t dataNumFields;
	uint32_t dataClassName;
	uint32_t baseMap;
	int32_t nPackedSize;
	uint32_t optimizedDataMap;
};

struct EntityCacheInfo {
	uint32_t networkable;
	uint16_t baseEntitiesIndex;
	uint16_t isDormant;
};

static int32_t getDataVar(const std::string &name, procptr_t predMap) {
	while (predMap) {
		DataMap dataMap;
		if (!proc->peek(predMap, dataMap)) {
			return 0;
		}

		// The structure is 4 bytes bigger on Linux.
		const size_t realStructSize = isWin32 ? sizeof(TypeDescription) : sizeof(TypeDescription) + 4;

		const auto descs = proc->peekVector< TypeDescription >(dataMap.dataDesc, dataMap.dataNumFields, realStructSize);
		for (const auto &desc : descs) {
			if (!desc.fieldName) {
				continue;
			}

			const auto fieldName = proc->peekString(desc.fieldName);

			if (fieldName == name) {
				return desc.fieldOffset;
			}

			if (desc.td) {
				const auto offset = getDataVar(name, desc.td);
				if (offset) {
					return offset;
				}
			}
		}

		predMap = dataMap.baseMap;
	}

	return 0;
}

static int32_t getDataVarFromEntity(const std::string &name, const procptr_t entity) {
	procptr_t GetPredDescMap;

	// Brute-force virtual function index.
	for (uint8_t i = 20; i > 16; --i) {
		GetPredDescMap = proc->virtualFunction(entity, i);

		if (proc->peek< uint8_t >(GetPredDescMap + (isWin32 ? 0 : 1)) == 0xB8) {
			break;
		}
	}

	// Windows:
	// B8 ?? ?? ?? ??    mov     eax, offset dword_????????
	// C3                retn
	//
	// Linux:
	// 55                push    ebp
	// B8 ?? ?? ?? ??    mov     eax, offset dword_????????
	// 89 E5             mov     ebp, esp
	// 5D                pop     ebp
	// C3                retn
	return getDataVar(name, proc->peek< uint32_t >(GetPredDescMap + (isWin32 ? 1 : 2)));
}

static procptr_t getLocalPlayer(const procptr_t localClient, procptr_t clientEntityList, const procptr_t engineClient) {
	const auto GetLocalPlayer = proc->virtualFunction(engineClient, 12);

	// Windows:
	// 6A FF                   push    0FFFFFFFFh
	// E8 ?? ?? ?? ??          call    GetLocalClient
	// 8B 80 ?? ?? ?? ??       mov     eax, [eax+?]
	// 83 C4 04                add     esp, 4
	// 40                      inc     eax
	// C3                      retn
	//
	// Linux:
	// 55                      push    ebp
	// 89 E5                   mov     ebp, esp
	// 83 EC 18                sub     esp, 18h
	// C7 04 24 FF FF FF FF    mov     dword ptr [esp], 0FFFFFFFFh
	// E8 ?? ?? ?? ??          call    GetLocalClient
	// 8B 80 ?? ?? ?? ??       mov     eax, [eax+?]
	// C9                      leave
	// 83 C0 01                add     eax, 1
	// C3                      retn
	const auto localPlayerIndexOffset = proc->peek< int32_t >(GetLocalPlayer + (isWin32 ? 9 : 20));
	const auto localPlayerIndex       = proc->peek< uint32_t >(localClient + localPlayerIndexOffset) + 1;

	auto GetClientNetworkable = proc->virtualFunction(clientEntityList, 0);

	// Left 4 Dead:
	// 8B 44 24 04                mov     eax, [esp+arg_0]
	// 8B 44 C1 ??                mov     eax, [ecx+eax*8+?]
	// C2 04 00                   retn    4

	// Windows:
	// 55                         push    ebp
	// 8B EC                      mov     ebp, esp
	// 8B 45 08                   mov     eax, [ebp+arg_0]
	// 3D ?? ?? ?? ??             cmp     eax, ?
	// 77 08                      ja      short 08
	// 8B 44 C1 ??                mov     eax, [ecx+eax*8+?]
	// 5D                         pop     ebp
	// C2 04 00                   retn    4
	//
	// Linux:
	// 81 6C 24 04 ?? ?? ?? ??    sub     [esp+arg_0], ????????
	// EB ??                      jmp     short GetClientNetworkable
	if (!isWin32) {
		clientEntityList -= proc->peek< int32_t >(GetClientNetworkable + 4);
		GetClientNetworkable = GetClientNetworkable + 10 + proc->peek< int8_t >(GetClientNetworkable + 9);

		// 55                      push    ebp
		// 89 E5                   mov     ebp, esp
		// 8B 45 0C                mov     eax, [ebp+arg_4]
		// 3D ?? ?? ?? ??          cmp     eax, ?
		// 77 13                   ja      short 13
		// 8B 55 08                mov     edx, [ebp+arg_0]
		// 5D                      pop     ebp
		// 8B 84 C2 ?? ?? ?? ??    mov     eax, [edx+eax*8+?]
		// C3                      retn
	}

	procptr_t entityCacheInfo;

	if (isWin32) {
		if (proc->peek< uint8_t >(GetClientNetworkable + 6) == 0xC1) {
			// Left 4 Dead
			entityCacheInfo = clientEntityList + proc->peek< int8_t >(GetClientNetworkable + 7);
		} else {
			entityCacheInfo = clientEntityList + proc->peek< int8_t >(GetClientNetworkable + 16);
		}
	} else {
		entityCacheInfo = clientEntityList + proc->peek< int32_t >(GetClientNetworkable + 20);
	}

	const auto entity = proc->peek< EntityCacheInfo >(entityCacheInfo + sizeof(EntityCacheInfo) * localPlayerIndex);

	// We subtract 8 bytes in order to cast from IClientNetworkable to IClientEntity.
	return entity.networkable ? (entity.networkable - 8) : 0;
}

#endif
