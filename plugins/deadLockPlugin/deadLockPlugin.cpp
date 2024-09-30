// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

// Include the definitions of the plugin functions
// Not that this will also include ../PluginComponents.h
#include "../MumblePlugin.h"

#include <condition_variable>
#include <cstring>
#include <iostream>
#include <mutex>
#include <thread>

MumbleAPI mumAPI;
mumble_connection_t activeConnection;
mumble_plugin_id_t ownID;
std::mutex dummyMutex;
std::condition_variable waiter;
bool dummyRan                = false;
bool deadlockPreventionWorks = false;

//////////////////////////////////////////////////////////////
//////////////////// OBLIGATORY FUNCTIONS ////////////////////
//////////////////////////////////////////////////////////////

mumble_error_t mumble_init(uint32_t id) {
	ownID = id;
	mumAPI.log(ownID, "Intitialized");

	// MUMBLE_STATUS_OK is a macro set to the appropriate status flag (ErrorCode)
	// If you need to return any other status have a look at the ErrorCode enum
	// inside PluginComponents.h and use one of its values
	return MUMBLE_STATUS_OK;
}

void mumble_shutdown() {
	mumAPI.log(ownID, "Shutdown");
}

MumbleStringWrapper mumble_getName() {
	static const char *name = "DeadLockPlugin";

	MumbleStringWrapper wrapper;
	wrapper.data = name;
	wrapper.size = strlen(name);
	// It's a static String and therefore doesn't need releasing
	wrapper.needsReleasing = false;

	return wrapper;
}

mumble_version_t mumble_getAPIVersion() {
	// MUMBLE_PLUGIN_API_VERSION will always contain the API version of the used header file (the one used to build
	// this plugin against). Thus you should always return that here in order to no have to worry about it.
	return MUMBLE_PLUGIN_API_VERSION;
}

void mumble_registerAPIFunctions(void *api) {
	// In this function the plugin is presented with a struct of function pointers that can be used
	// to interact with Mumble. Thus you should store it somewhere safe for later usage.

	// The pointer has to be cast to the respective API struct. You always have to cast to the same API version
	// as this plugin itself is using. Thus if this plugin is compiled using the API version 1.0.x (where x is an
	// arbitrary version) the pointer has to be cast to MumbleAPI_v_1_0_x (where x is a literal "x"). Furthermore the
	// struct HAS TO BE COPIED!!! Storing the pointer is not an option as it will become invalid quickly!

	// **If** you are using the same API version that is specified in the included header file (as you should), you
	// can simply use the MUMBLE_API_CAST to cast the pointer to the correct type and automatically dereferencing it.
	mumAPI = MUMBLE_API_CAST(api);
}

void mumble_releaseResource(const void *pointer) {
	std::cerr << "[ERROR]: Unexpected call to mumble_releaseResources" << std::endl;
	std::terminate();

	// Mark as unused
	(void) pointer;
}


//////////////////////////////////////////////////////////////
///////////////////// OPTIONAL FUNCTIONS /////////////////////
//////////////////////////////////////////////////////////////
// The implementation of below functions is optional. If you don't need them, don't include them in your
// plugin

mumble_version_t mumble_getVersion() {
	// Mumble uses semantic versioning (see https://semver.org/)
	// { major, minor, patch }
	return { 1, 0, 0 };
}

MumbleStringWrapper mumble_getAuthor() {
	static const char *author = "MumbleDevelopers";

	MumbleStringWrapper wrapper;
	wrapper.data = author;
	wrapper.size = strlen(author);
	// It's a static String and therefore doesn't need releasing
	wrapper.needsReleasing = false;

	return wrapper;
}

MumbleStringWrapper mumble_getDescription() {
	static const char *description =
		"This plugin intentionally produces a deadlock (if Mumble doesn't prevent it properly). "
		"It shouldn't be included in the release build of Mumble.";

	MumbleStringWrapper wrapper;
	wrapper.data = description;
	wrapper.size = strlen(description);
	// It's a static String and therefore doesn't need releasing
	wrapper.needsReleasing = false;

	return wrapper;
}

uint32_t mumble_getFeatures() {
	// Tells Mumble whether this plugin delivers some known common functionality. See the PluginFeature enum in
	// PluginComponents.h for what is available.
	// If you want your plugin to deliver positional data, you'll want to return FEATURE_POSITIONAL
	return MUMBLE_FEATURE_NONE;
}

uint32_t mumble_deactivateFeatures(uint32_t features) {
	return features;
}

void dummyFunction() {
	std::lock_guard< std::mutex > guard(dummyMutex);

	// The client has finished synchronizing with the server. Thus we can now obtain a list of all users on this server
	mumble_connection_t connection;
	mumble_error_t error = mumAPI.getActiveServerConnection(ownID, &connection);
	if (error == MUMBLE_EC_API_REQUEST_TIMEOUT) {
		deadlockPreventionWorks = true;
	} else {
		std::cerr << "[ERROR]: Deadlocked API request returned " << error << " (\"" << mumble_errorMessage(error)
				  << "\")" << std::endl;

		deadlockPreventionWorks = false;
	}

	dummyRan = true;
	waiter.notify_all();
}

void mumble_onServerSynchronized(mumble_connection_t) {
	mumAPI.log(ownID, "Starting deadlock attempt...");

	// We enter this function, which is called from Mumble's main thread
	// We spawn a separate thread and make sure that we block this function until the separate thread has finished. In
	// that separate thread, we are calling an API function, which Mumble needs to execute in the main thread (for
	// synchronization purposes). However, since we are blocking the main thread here, we produce a deadlock. Only if
	// Mumble's deadlock prevention works as expected, is a freeze prevented.
	std::unique_lock< std::mutex > lock(dummyMutex);

	std::thread myThread(dummyFunction);

	waiter.wait(lock, []() { return dummyRan; });

	myThread.join();

	if (deadlockPreventionWorks) {
		mumAPI.log(ownID, "Deadlock prevented successfully");
	} else {
		mumAPI.log(ownID, "Deadlock prevented but not in the expected way");
	}
}
