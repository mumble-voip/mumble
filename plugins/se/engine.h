// Copyright 2020-2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_PLUGIN_SE_ENGINE_
#define MUMBLE_MUMBLE_PLUGIN_SE_ENGINE_

struct NetInfo {
	uint32_t type;
	uint8_t ip[4];
	uint16_t port;
};

static procptr_t getLocalClient(const procptr_t engineClient) {
	// We use GetBaseLocalClient() instead of GetLocalClient() because we just need the main client.
	// GetLocalClient() gets the client from an array at the index passed to the function.
	// There are multiple clients because of the split screen feature.

	const auto GetNetChannelInfo = proc->virtualFunction(engineClient, 74);

	// Windows:
	// E8 ?? ?? ?? ??    call    GetBaseLocalClient
	// 8B 40 ??          mov     eax, [eax+?]
	// C3                retn
	//
	// Linux:
	// 55                push    ebp
	// 89 E5             mov     ebp, esp
	// 83 EC 08          sub     esp, 8
	// E8 ?? ?? ?? ??    call    GetBaseLocalClient
	// 8B 40 ??          mov     eax, [eax+?]
	// C9                leave
	// C3                retn
	const auto callTarget         = proc->peek< int32_t >(GetNetChannelInfo + (isWin32 ? 1 : 7));
	const auto callInstructionEnd = GetNetChannelInfo + (isWin32 ? 5 : 11);
	const auto GetBaseLocalClient = callInstructionEnd + callTarget;

	// Windows:
	// A1 ?? ?? ?? ??             mov     eax, dword_????????
	// 83 C0 ??                   add     eax, ?
	// C3                         retn
	if (isWin32) {
		return proc->peekPtr(proc->peek< uint32_t >(GetBaseLocalClient + 1))
			   + proc->peek< int8_t >(GetBaseLocalClient + 7);
	}

	// Linux:
	// 55                         push    ebp
	// 89 E5                      mov     ebp, esp
	// 83 EC 18                   sub     esp, 18h
	// C7 44 24 04 00 00 00 00    mov     dword ptr [esp+4], 0
	// C7 04 24 ?? ?? ?? ??       mov     dword ptr [esp], offset dword_????????
	// E8 ?? ?? ?? ??             call    sub_????????
	// C9                         leave
	// C3                         retn
	//
	// The function is quite different on Linux. It returns the result of an unknown function:
	//
	// 55                         push    ebp
	// 89 E5                      mov     ebp, esp
	// 53                         push    ebx
	// 83 EC 14                   sub     esp, 14h
	// 8B 45 0C                   mov     eax, [ebp+arg_4]
	// 8B 5D 08                   mov     ebx, [ebp+arg_0]
	// 83 F8 FF                   cmp     eax, 0FFFFFFFFh
	// 74 0E                      jz      short loc_1
	// 8B 44 83 04                mov     eax, [ebx+eax*4+4]
	// 83 C0 04                   add     eax, 4
	//
	// loc_0:
	// 83 C4 14                   add     esp, 14h
	// 5B                         pop     ebx
	// 5D                         pop     ebp
	// C3                         retn
	//
	// 90                         align 10h
	//
	// loc_1:
	// 8B 03                      mov     eax, [ebx]
	// 89 1C 24                   mov     [esp], ebx
	// FF 50 14                   call    dword ptr [eax+14h]
	// 8B 44 83 04                mov     eax, [ebx+eax*4+4]
	// 83 C0 04                   add     eax, 4
	// EB E8                      jmp     short loc_0
	//
	// Its purpose seem to be to iterate over the clients array, which is done directly by GetBaseLocalClient() and
	// GetLocalClient() on Windows.
	return proc->peekPtr(proc->peek< uint32_t >(GetBaseLocalClient + 17) + 4) + 4;
}

static int8_t getSignOnStateOffset(const procptr_t engineClient) {
	const auto IsInGame = proc->virtualFunction(engineClient, 26);

	// Windows:
	// E8 ?? ?? ?? ??    call    GetBaseLocalClient
	// 33 C9             xor     ecx, ecx
	// 83 78 ?? 06       cmp     dword ptr [eax+?], 6
	// 0F 94 C0          setz    al
	// C3                retn
	//
	// Linux:
	// 55                push    ebp
	// 89 E5             mov     ebp, esp
	// 83 EC 08          sub     esp, 8
	// E8 ?? ?? ?? ??    call    GetBaseLocalClient
	// 83 78 ?? 06       cmp     dword ptr [eax+?], 6
	// C9                leave
	// 0F 94 C0          setz    al
	// C3                retn
	return proc->peek< int8_t >(IsInGame + (isWin32 ? 9 : 13));
}

static int32_t getLevelNameOffset(const procptr_t engineClient) {
	const auto GetLevelNameShort = proc->virtualFunction(engineClient, 53);

	// Windows:
	// ...
	//
	// E8 ?? ?? ?? ??    call    GetBaseLocalClient
	// 05 ?? ?? ?? ??    add     eax, ?
	// C3                retn
	//
	// Linux:
	// ...
	//
	// E8 ?? ?? ?? ??    call    GetBaseLocalClient
	// C9                leave
	// 05 ?? ?? ?? ??    add     eax, ?
	// C3                retn
	if (isWin32) {
		if (proc->peek< uint8_t >(GetLevelNameShort + 37) == 0x05) {
			// Left 4 Dead
			return proc->peek< int32_t >(GetLevelNameShort + 38);
		} else {
			return proc->peek< int32_t >(GetLevelNameShort + 40);
		}
	}

	return proc->peek< int32_t >(GetLevelNameShort + 57);
}

static int32_t getNetInfoOffset(const procptr_t localClient, const procptr_t engineClient) {
	const auto GetNetChannelInfo = proc->virtualFunction(engineClient, 74);

	// Windows:
	// E8 ?? ?? ?? ??    call    GetBaseLocalClient
	// 8B 40 ??          mov     eax, [eax+?]
	// C3                retn
	//
	// Linux:
	// 55                push    ebp
	// 89 E5             mov     ebp, esp
	// 83 EC 08          sub     esp, 8
	// E8 ?? ?? ?? ??    call    GetBaseLocalClient
	// 8B 40 ??          mov     eax, [eax+?]
	// C9                leave
	// C3                retn
	const auto NetChannelInfo =
		proc->peekPtr(localClient + proc->peek< int8_t >(GetNetChannelInfo + (isWin32 ? 7 : 13)));
	const auto GetAddress = proc->virtualFunction(NetChannelInfo, 1);

	// Windows:
	// 6A 00                      push    0
	// 81 C1 ?? ?? ?? ??          add     ecx, ?
	// E8 C3 9D 1D 00             call    ToString
	// C3                         retn
	//
	// Linux:
	// 55                         push    ebp
	// 89 E5                      mov     ebp, esp
	// 83 EC ??                   sub     esp, ?
	// 8B 45 08                   mov     eax, [ebp+arg_0]
	// C7 44 24 04 00 00 00 00    mov     dword ptr [esp+4], 0
	// 05 ?? ?? ?? ??             add     eax, ?
	// 89 04 24                   mov     [esp], eax
	// E8 ?? ?? ?? ??             call    ToString
	// C9                         leave
	// C3                         retn
	const auto netInfo = NetChannelInfo + proc->peek< int32_t >(GetAddress + (isWin32 ? 4 : 18));

	return static_cast< int32_t >(netInfo - localClient);
}

#endif
