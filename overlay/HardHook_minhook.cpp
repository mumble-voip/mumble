// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "HardHook.h"
#include "ods.h"

#include <MinHook.h>

static LONG minhook_init_once = 0;

// EnsureMinHookInitialized ensures that the MinHook
// library is initialized. If MinHook is already
// initialized, calling this function is a no-op.
static void EnsureMinHookInitialized() {
	// Ensure MH_Initialize is only called once.
	if (InterlockedCompareExchange(&minhook_init_once, 1, 0) == 0) {
		MH_STATUS status = MH_Initialize();
		if (status != MH_OK && status != MH_ERROR_ALREADY_INITIALIZED) {
			fods("HardHook: Failed to initialize minhook; MH_Initialize returned %s", MH_StatusToString(status));
		}
	}
}

/**
 * @brief Constructs a new hook without actually injecting.
 */
HardHook::HardHook() : m_func(nullptr), m_replacement(nullptr), call(nullptr) {
	EnsureMinHookInitialized();
}

/**
 * @brief Constructs a new hook by injecting given replacement function into func.
 * @see HardHook::setup
 * @param func Funktion to inject replacement into.
 * @param replacement Function to inject into func.
 */
HardHook::HardHook(voidFunc func, voidFunc replacement) : m_func(nullptr), m_replacement(nullptr), call(nullptr) {
	EnsureMinHookInitialized();

	setup(func, replacement);
}

/**
 * @brief Makes sure the given replacement function is run once func is called.
 *
 * Uses MinHook to put the hook in place.
 *
 * @param func Pointer to function to redirect.
 * @param replacement Pointer to code to redirect to.
 */
void HardHook::setup(voidFunc func, voidFunc replacement) {
	m_func        = func;
	m_replacement = replacement;

	MH_STATUS status = MH_CreateHook((LPVOID) func, (LPVOID) replacement, (LPVOID *) &call);
	if (status != MH_OK) {
		fods("HardHook: setup failed, MH_CreateHook returned %s", MH_StatusToString(status));
	}

	inject(true);
}

void HardHook::setupInterface(IUnknown *unkn, LONG funcoffset, voidFunc replacement) {
	fods("HardHook: setupInterface: Replacing %p function #%ld", unkn, funcoffset);
	void **ptr = reinterpret_cast< void ** >(unkn);
	ptr        = reinterpret_cast< void ** >(ptr[0]);
	setup(reinterpret_cast< voidFunc >(ptr[funcoffset]), replacement);
}

void HardHook::reset() {
	m_func        = nullptr;
	m_replacement = nullptr;
	call          = nullptr;
}

/**
 * @brief Injects redirection code into the target function.
 *
 * @param force No-op in the MinHook-based HardHook implementation.
 */
void HardHook::inject(bool force) {
	if (!force) {
		// MinHook guarantees the presence of a trampoline, so
		// inject() and restore() are no-ops unless force is
		// set to true.
		return;
	}

	MH_STATUS status = MH_EnableHook((LPVOID) m_func);
	if (status != MH_OK) {
		fods("HardHook: inject() failed: MH_EnableHook returned %s", MH_StatusToString(status));
	}
}

/**
 * @brief Restores the original code in a target function.
 *
 * @param force No-op in the MinHook-based HardHook implementation.
 */
void HardHook::restore(bool force) {
	if (!force) {
		// MinHook guarantees the presence of a trampoline, so
		// inject() and restore() are no-ops unless force is
		// set to true.
		return;
	}

	MH_STATUS status = MH_DisableHook((LPVOID) m_func);
	if (status != MH_OK) {
		fods("HardHook: restore() failed: MH_DisableHook returned %s", MH_StatusToString(status));
	}
}

void HardHook::print() {
	fods("HardHook: unused 'print' method called for MinHook-based HardHook");
}

/**
 * @brief No-op in MinHook-based HardHook implementation.
 */
void HardHook::check() {
	fods("HardHook: unused 'check' method called for MinHook-based HardHook");
}
