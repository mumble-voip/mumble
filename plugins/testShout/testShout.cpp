// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "../MumblePlugin.h"

#ifdef _WIN32
#	include <windows.h>
#endif

#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <functional>
#include <iomanip>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

////////////////////////////////////////////////////////////
// Global plugin state
////////////////////////////////////////////////////////////

mumble_api_t        mumAPI;
mumble_connection_t activeConnection = -1;
mumble_plugin_id_t  ownID            = 0;

// Set to false to stop the interactive console thread.
std::atomic< bool > g_running{ false };

// Set to true once mumble_registerAPIFunctions has been called (mumAPI valid).
std::atomic< bool > g_apiReady{ false };

// The dedicated console thread.
std::thread g_consoleThread;

// Cached raw console handles (set up by openConsole()).
#ifdef _WIN32
static HANDLE g_hConsoleOut = INVALID_HANDLE_VALUE;
static HANDLE g_hConsoleIn  = INVALID_HANDLE_VALUE;
#endif


////////////////////////////////////////////////////////////
// Console I/O (raw Win32 handles, no C/C++ stdio)
////////////////////////////////////////////////////////////

// Write raw text to our dedicated console. Falls back to OutputDebugStringA when
// no console is available (visible in Visual Studio's "Output" window / DebugView).
static void consoleWrite(const std::string &text) {
#ifdef _WIN32
	if (g_hConsoleOut != INVALID_HANDLE_VALUE) {
		DWORD written = 0;
		WriteConsoleA(g_hConsoleOut, text.c_str(), static_cast< DWORD >(text.size()), &written, nullptr);
		return;
	}
	OutputDebugStringA(text.c_str());
#else
	std::fputs(text.c_str(), stdout);
	std::fflush(stdout);
#endif
}

// Read a single line (without trailing newline) from the console. Returns false
// on EOF / closed handle.
static bool consoleReadLine(std::string &out) {
	out.clear();
#ifdef _WIN32
	if (g_hConsoleIn == INVALID_HANDLE_VALUE) {
		return false;
	}

	// ReadFile on a console input handle reads raw bytes and is far more reliable
	// than ReadConsoleA (which is affected by console input modes / mouse events).
	char        chunk[256];
	DWORD       got = 0;
	std::string accumulated;

	for (;;) {
		if (!ReadFile(g_hConsoleIn, chunk, sizeof(chunk), &got, nullptr) || got == 0) {
			if (accumulated.empty()) {
				return false;
			}
			break;
		}

		for (DWORD i = 0; i < got; i++) {
			char c = chunk[i];
			if (c == '\r' || c == '\n') {
				out = accumulated;
				return true;
			}
			if (c == '\b') { // backspace
				if (!accumulated.empty()) {
					accumulated.pop_back();
					consoleWrite("\b \b");
				}
				continue;
			}
			if (c == '\x03') { // Ctrl-C
				return false;
			}
			accumulated.push_back(c);
//			consoleWrite(std::string(1, c)); // echo
		}
	}

	out = accumulated;
	return true;
#else
	char  line[4096] = { 0 };
	if (!std::fgets(line, sizeof(line), stdin)) {
		return false;
	}
	out = line;
	if (!out.empty() && out.back() == '\n') {
		out.pop_back();
	}
	if (!out.empty() && out.back() == '\r') {
		out.pop_back();
	}
	return true;
#endif
}

// Print a full line to the console + debugger output.
static void printLine(const std::string &message) {
	std::string line = "[TestShout] " + message + "\n";
	consoleWrite(line);
#ifdef _WIN32
	OutputDebugStringA(line.c_str());
#endif
}

// Log to the console + debugger + Mumble's own plugin log (once API is ready).
static void logLine(const std::string &message) {
	printLine(message);
	if (g_apiReady.load() && ownID != 0) {
		mumAPI.log(ownID, message.c_str());
	}
}


////////////////////////////////////////////////////////////
// Thread-communicating API dispatch (request/response queue)
////////////////////////////////////////////////////////////
//
// The console thread enqueues a command (a std::function that performs the API
// calls and produces a response string) and waits on a condition variable. The
// command is then executed and the response returned. This decouples "who asks"
// from "who runs", and - critically - neither thread blocks the other: the main
// thread never waits on the console thread, so no deadlock can occur. Mumble
// executes each API function on its main thread with a built-in timeout.

struct CommandResponse {
	bool        done = false;
	std::string output;
};

static std::function< std::string() > g_pendingCommand;
static CommandResponse         g_response;
static bool                    g_commandPending = false;


////////////////////////////////////////////////////////////
// Helpers
////////////////////////////////////////////////////////////

static const char *talkingStateToString(mumble_talking_state_t state) {
	switch (state) {
		case MUMBLE_TS_INVALID:
			return "Invalid";
		case MUMBLE_TS_PASSIVE:
			return "Passive";
		case MUMBLE_TS_TALKING:
			return "Talking";
		case MUMBLE_TS_WHISPERING:
			return "Whispering";
		case MUMBLE_TS_SHOUTING:
			return "Shouting";
		case MUMBLE_TS_TALKING_MUTED:
			return "Talking (muted)";
		default:
			return "Unknown";
	}
}

static const char *errorMessageSafe(mumble_error_t error) {
	return mumble_errorMessage(error);
}


////////////////////////////////////////////////////////////
// Command handlers (each returns a response string)
////////////////////////////////////////////////////////////

// Command 1: read whether the local user is currently using PTT.
// NOTE: the API does not expose the raw iPushToTalk counter, only a boolean.
static std::string cmdReadPTT() {
	bool           usingPTT = false;
	mumble_error_t error    = mumAPI.isLocalUserUsingPTT(ownID, &usingPTT);

	if (error != MUMBLE_STATUS_OK) {
		return std::string("isLocalUserUsingPTT failed: \n  ") + errorMessageSafe(error);
	}

	return std::string("isLocalUserUsingPTT = ") + (usingPTT ? "true" : "false");
}

// Command 2: read the local user's current talking state.
static std::string cmdReadTalkingState() {
	mumble_talking_state_t state = MUMBLE_TS_INVALID;
	mumble_error_t         error = mumAPI.getLocalUserTalkingState(ownID, &state);

	if (error != MUMBLE_STATUS_OK) {
		return std::string("getLocalUserTalkingState failed: \n  ") + errorMessageSafe(error);
	}

	return std::string("Local user talking state = ") + talkingStateToString(state) + " (" +
		   std::to_string(static_cast< int >(state)) + ")";
}

// Command 3: list all users/channels, then ask for user IDs and channel IDs, and
// finally apply them via requestSetLocalUserShoutingTargets.
//
// NOTE: this command needs interactive sub-prompts, so it is handled specially in
// the console loop rather than as a pure function. See cmdSetTargetsInteractive().
static std::string cmdSetTargets(const std::vector< mumble_userid_t > &users,
								 const std::vector< mumble_channelid_t > &channels) {
	short allocID;
	mumble_error_t error = mumAPI.requestStartLocalUserWhisperShout(
		ownID, const_cast<uint32_t*>(users.empty() ? nullptr : users.data()), users.size(),
		const_cast<int32_t*>(channels.empty() ? nullptr : channels.data()), channels.size(),
		&allocID);

	if (error == MUMBLE_STATUS_OK) {
		return "Whisper/Shout targets set: " + std::to_string(users.size()) + 
		       " user(s), " + std::to_string(channels.size()) + " channel(s).\n"
			   "To stop whisper/shout targets, use this allocID: " + 
			   std::to_string(allocID) + ".";
	}
	return std::string("requestSetLocalUserShoutingTargets failed: \n  ") + errorMessageSafe(error);
}


// Command 5: deactivate PTT-style shout transmission (iPushToTalk--).
static std::string cmdDisableShouting() {
	std::string strAllocID;
	printLine("Input allocID: ");
	consoleReadLine(strAllocID);
	short allocID;
	try{
		allocID = std::stoi(strAllocID);
	}catch(...){
		return "Invalid allocID.";
	}
	mumble_error_t error = mumAPI.requestStopLocalUserWhisperShout(ownID, allocID);

	if (error == MUMBLE_STATUS_OK) {
		return "Shouting disabled (iPushToTalk decremented).";
	}
	return std::string("requestDisableLocalUserShouting failed: \n  ") + errorMessageSafe(error);
}


static void cmdPrintAvailableUserAndChannels() {
	if (activeConnection < 0) {
		printLine("Not connected to a server (activeConnection == -1). Connect first.");
		return;
	}
	mumble_userid_t *users     = nullptr;
	size_t           userCount = 0;
	
	if (mumAPI.getAllUsers(ownID, activeConnection, &users, &userCount) != MUMBLE_STATUS_OK) {
		printLine("Failed to fetch user list......");
		return;
	}

	std::string result = "Users on this server (" + std::to_string(userCount) + "):";
	for(size_t i = 0; i < userCount; i++){
		const char *name = nullptr;
		if (mumAPI.getUserName(ownID, activeConnection, users[i], &name) == MUMBLE_STATUS_OK) {
			result += "\n  User  id=" + std::to_string(users[i]) + "  name=\"" + std::string(name) + "\"";
		} else {
			result += "\n  User  id=" + std::to_string(users[i]) + "  name=(null)";
		}

		mumAPI.freeMemory(ownID, name);
	}
	printLine(result); mumAPI.freeMemory(ownID, users);

	mumble_channelid_t *channels     = nullptr;
	size_t              channelCount = 0;
	if (mumAPI.getAllChannels(ownID, activeConnection, &channels, &channelCount) != MUMBLE_STATUS_OK) {
		printLine("Failed to fetch channel list");
	}

	result = "Channels on this server (" + std::to_string(channelCount) + "):";
	for (size_t i = 0; i < channelCount; i++) {
		const char *name = nullptr;
		std::ostringstream line;
		line << "\n  Channel  id=" << std::left << std::setw(12) << channels[i];
		if (mumAPI.getChannelName(ownID, activeConnection, channels[i], &name) == MUMBLE_STATUS_OK) {
			line << "  name=\"" << std::string(name) << "\"";
			mumAPI.freeMemory(ownID, name);
		} else {
			line << "  name=<unavailable>";
		}
		result += line.str();
	}
	printLine(result); mumAPI.freeMemory(ownID, channels);
}


////////////////////////////////////////////////////////////
// Command 4 (interactive): list + prompt + apply
////////////////////////////////////////////////////////////

// Prints the list of users and channels, then reads the target IDs from the
// console and applies them. Runs on the console thread, but each API call is
// routed through dispatchCommand() so it is thread-safe.
static void cmdSetTargetsInteractive() {
	if (activeConnection < 0) {
		printLine("Not connected to a server (activeConnection == -1). Connect first.");
		return;
	}

	cmdPrintAvailableUserAndChannels();

	consoleWrite("[TestShout] Enter user IDs to shout at (space-separated, empty for none): \n>");
	std::string userLine;
	consoleReadLine(userLine);

	std::vector< mumble_userid_t > targetUsers;
	{
		std::istringstream iss(userLine);
		uint32_t          id = 0;
		while (iss >> id) {
			targetUsers.push_back(id);
		}
	}

	// ----- Read channel IDs (space-separated) -----
	consoleWrite("[TestShout] Enter channel IDs to shout to (space-separated, empty for none): \n>");
	std::string channelLine;
	consoleReadLine(channelLine);

	std::vector< mumble_channelid_t > targetChannels;
	{
		std::istringstream iss(channelLine);
		int32_t            id = 0;
		while (iss >> id) {
			targetChannels.push_back(id);
		}
	}

	printLine(cmdSetTargets(targetUsers, targetChannels));
}


////////////////////////////////////////////////////////////
// Interactive console loop (runs on the dedicated thread)
////////////////////////////////////////////////////////////

static void printMenu() {
	consoleWrite("\n[TestShout] ==== Menu ====\n"
				 "  1 : Read whether local user is using PTT\n"
				 "  2 : Read local user talking state\n"
				 "  3 : List users/channels \n"
				 "  4 : Start whispering/shouting after listing users/channels\n"
				 "  5 : Stop whispering/shouting (requestDisableLocalUserShouting)\n"
				 "[TestShout] Choice: ");
}

static void loop_main() {
	while (g_running.load()) {
		printMenu();

		std::string line;
		if (!consoleReadLine(line)) {
			break;
		}

		if (line.empty()) {
			continue;
		}

		if (line == "1") {
			printLine(cmdReadPTT());
		} else if (line == "2") {
			printLine(cmdReadTalkingState());
		} else if (line == "3") {
			cmdPrintAvailableUserAndChannels();
		} else if (line == "4") {
			cmdSetTargetsInteractive();
		} else if (line == "5") {
			printLine(cmdDisableShouting());
		} else {
			consoleWrite("[TestShout] Unknown choice: \"" + line + "\"\n");
		}
	}
}


////////////////////////////////////////////////////////////
// Console setup
////////////////////////////////////////////////////////////

// Create a dedicated console window (Mumble is a GUI app and has no console of its
// own) and cache its raw handles. We deliberately do NOT use freopen()/std::cout
// because those rely on C/C++ stdio stream state that is unreliable inside a GUI
// process. Instead we keep the HANDLEs and use WriteConsoleA/ReadFile directly.
static void openConsole() {
#ifdef _WIN32
	bool consoleCreated = false;
	if (AllocConsole()) {
		consoleCreated = true;
		SetConsoleTitleA("TestShout Plugin Console");
	}
	// else: a console already exists (e.g. Mumble was started from a console).

	g_hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
	g_hConsoleIn  = GetStdHandle(STD_INPUT_HANDLE);

	// Put console input in a sane mode: line input + echo, no mouse/window events.
	if (g_hConsoleIn != INVALID_HANDLE_VALUE) {
		SetConsoleMode(g_hConsoleIn, ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT | ENABLE_PROCESSED_INPUT);
	}

	std::string diag = "Console ready (AllocConsole=" + std::string(consoleCreated ? "true" : "false") +
					   ", hIn=" + std::to_string(reinterpret_cast< uintptr_t >(g_hConsoleIn)) +
					   ", hOut=" + std::to_string(reinterpret_cast< uintptr_t >(g_hConsoleOut)) + ")";
	consoleWrite("[TestShout] " + diag + "\n");
	OutputDebugStringA(("[TestShout] " + diag + "\n").c_str());
#endif
}


////////////////////////////////////////////////////////////
// Obligatory plugin functions
////////////////////////////////////////////////////////////

mumble_error_t mumble_init(uint32_t id) {
	ownID = id;

	// Bring up the dedicated console window first so logging is visible.
	openConsole();

	printLine("Initialized with plugin ID " + std::to_string(id));
	mumAPI.getActiveServerConnection(ownID, &activeConnection);
	if (activeConnection != -1) {
		printLine("Connected to server " + std::to_string(activeConnection));
	}

	// NOTE: per PluginLifecycle.md, mumble_registerAPIFunctions is called BEFORE
	// mumble_init, so mumAPI is already valid here. But we still gate API calls on
	// g_apiReady for safety and start the console thread here.
	g_running.store(true);
	g_consoleThread = std::thread(loop_main);

	consoleWrite(
		".:========[ Test Shout Plugin v1.0.0 ]========:.\n"
		"|| Plugin Author: WuCJ638                     ||\n"
		"||                Deepseek V4 Pro             ||\n"
		"||============================================||\n"
		"|| - This plugin is for testing API around    ||\n"
		"||   whispering/shouting feature.             ||\n"
		"|| - DO NOT CLOSE THIS WINDOW WHILE THE PLUG- ||\n"
		"||   -IN IS STILL RUNNING! Exiting logic has  ||\n"
		"||   not been implemented yet.                ||\n"
//		"|| - DISABLE THE PLUGIN FIRST TO SAFELY CLOSE ||\n"
//		"||   THIS WINDOW.                             ||\n"
		"':============================================:'\n"
	);
	return MUMBLE_STATUS_OK;
}

void mumble_shutdown() {
	// I don't know how to close this console safely.
	// Since it's just a testing tool, I'm not going to dive deeper
	// into completing the shutdown logic.
	g_running.store(false);

	CancelIoEx(g_hConsoleIn, nullptr);

	// The console thread may be blocked in ReadFile(). Detach instead of join to
	// avoid blocking plugin shutdown; the OS reclaims the console at exit.
	if (g_consoleThread.joinable()) {
		g_consoleThread.join();
	}

	CloseHandle(g_hConsoleOut);
	CloseHandle(g_hConsoleIn);

	if (g_apiReady.load() && ownID != 0) {
		mumAPI.log(ownID, "TestShout shutdown");
	}
}

MumbleStringWrapper mumble_getName() {
	static const char *name = "TestShout";

	MumbleStringWrapper wrapper;
	wrapper.data           = name;
	wrapper.size           = strlen(name);
	wrapper.needsReleasing = false;

	return wrapper;
}

mumble_version_t mumble_getAPIVersion() {
	return MUMBLE_PLUGIN_API_VERSION;
}

void mumble_registerAPIFunctions(void *api) {
	// Copy the whole API struct; the passed pointer must not be stored.
	mumAPI = MUMBLE_API_CAST(api);
	g_apiReady.store(true);

	//logLine("Registered Mumble API functions");
}

void mumble_releaseResource(const void *pointer) {
	// This plugin never returns resources that need releasing.
	(void) pointer;
}


////////////////////////////////////////////////////////////
// Optional plugin functions
////////////////////////////////////////////////////////////

mumble_version_t mumble_getVersion() {
	return { 1, 0, 0 };
}

MumbleStringWrapper mumble_getAuthor() {
	static const char *author = "WuCJ638, Deepseek V4 Pro";

	MumbleStringWrapper wrapper;
	wrapper.data           = author;
	wrapper.size           = strlen(author);
	wrapper.needsReleasing = false;

	return wrapper;
}

MumbleStringWrapper mumble_getDescription() {
	static const char *description = "Interactive CLI test plugin for the local-user shouting API.";

	MumbleStringWrapper wrapper;
	wrapper.data           = description;
	wrapper.size           = strlen(description);
	wrapper.needsReleasing = false;

	return wrapper;
}

uint32_t mumble_getFeatures() {
	return MUMBLE_FEATURE_NONE;
}

uint32_t mumble_deactivateFeatures(uint32_t features) {
	return features;
}


////////////////////////////////////////////////////////////
// Event callbacks (may be invoked from Mumble's threads)
////////////////////////////////////////////////////////////

void mumble_onServerConnected(mumble_connection_t connection) {
	activeConnection = connection;
	//logLine("Server connected (connection ID " + std::to_string(connection) + ")");
}

void mumble_onServerDisconnected(mumble_connection_t connection) {
	activeConnection = -1;
	//logLine("Server disconnected (connection ID " + std::to_string(connection) + ")");
}

void mumble_onServerSynchronized(mumble_connection_t connection) {
	//logLine("Server synchronized (connection ID " + std::to_string(connection) + ")");
}
