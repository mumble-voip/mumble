// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "LinkedMem.h"
#include "SharedMemory.h"

#ifndef _WIN32
#	include <unistd.h>
#endif

#include <chrono>
#include <csignal>
#include <cstring>
#include <cwchar>
#include <iostream>
#include <random>
#include <thread>

SharedMemory sharedMem;
LinkedMem lm;
std::random_device dev;
std::mt19937 rng(dev());
std::uniform_real_distribution< float > generator(0, 100);

void initMumble() {
	sharedMem.mapMemory(getLinkedMemoryName());

	lm = LinkedMem();
}

void updateMumble() {
	if (!sharedMem.isMemoryMapped())
		return;

	if (lm.uiVersion != 2) {
		wcsncpy(lm.name, L"TestLink", 256);
		wcsncpy(lm.description, L"TestLink is a test of the Link plugin.", 2048);
		lm.uiVersion = 2;
	}
	lm.uiTick++;

	// Left handed coordinate system.
	// X positive towards "right".
	// Y positive towards "up".
	// Z positive towards "front".
	//
	// 1 unit = 1 meter

	// Unit vector pointing out of the avatar's eyes aka "At"-vector.
	lm.fAvatarFront[0] = 0.0f;
	lm.fAvatarFront[1] = 0.0f;
	lm.fAvatarFront[2] = 1.0f;

	// Unit vector pointing out of the top of the avatar's head aka "Up"-vector (here Top points straight up).
	lm.fAvatarTop[0] = 0.0f;
	lm.fAvatarTop[1] = 1.0f;
	lm.fAvatarTop[2] = 0.0f;

	// Position of the avatar (here standing slightly off the origin)
	lm.fAvatarPosition[0] = generator(rng);
	lm.fAvatarPosition[1] = generator(rng);
	lm.fAvatarPosition[2] = generator(rng);

	// Same as avatar but for the camera.
	lm.fCameraPosition[0] = 0.0f;
	lm.fCameraPosition[1] = 0.0f;
	lm.fCameraPosition[2] = 0.0f;

	lm.fCameraFront[0] = 0.0f;
	lm.fCameraFront[1] = 0.0f;
	lm.fCameraFront[2] = 1.0f;

	lm.fCameraTop[0] = 0.0f;
	lm.fCameraTop[1] = 1.0f;
	lm.fCameraTop[2] = 0.0f;

	// Identifier which uniquely identifies a certain player in a context (e.g. the ingame name).
	wcsncpy(lm.identity, L"Unique ID", 256);
	// Context should be equal for players which should be able to hear each other positional and
	// differ for those who shouldn't (e.g. it could contain the server+port and team)
	memcpy(lm.context, "ContextBlob\x00\x01\x02\x03\x04", 16);
	lm.context_len = 16;

	sharedMem.write(lm);
}

void signalHandler(int signum) {
	std::cout << "Interrupt signal (" << signum << ") received - shutting down..." << std::endl;

	sharedMem.close();

	std::exit(signum);
}

int main() {
	signal(SIGINT, signalHandler);

	initMumble();

	if (!sharedMem.isMemoryMapped()) {
		std::cerr << "Failed to create shared memory region (" << sharedMem.lastError() << ")" << std::endl;
		return 1;
	}

	std::cout << "Shared memory created successfully - Now starting update loop" << std::endl;

	while (true) {
		std::cout << "Tick" << std::endl;
		updateMumble();

		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}
