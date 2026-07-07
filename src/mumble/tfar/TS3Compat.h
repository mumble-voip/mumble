// MUMBLE-TFAR
// Internal interface between the TS3 compatibility layer (TeamspeakMumble.cpp)
// and the Qt bridge (TFARBridge.cpp) / patched Mumble client code.

#ifndef MUMBLE_MUMBLE_TFAR_TS3COMPAT_H_
#define MUMBLE_MUMBLE_TFAR_TS3COMPAT_H_

#include "datatypes.hpp"
#include "teamlog/logtypes.h"

#include <QtCore/QVariant>

#include <chrono>
#include <string>

namespace ts3compat {

/// True while Mumble is connected to a server and fully synchronized.
bool isConnectedAndSynchronized();
/// Set by the bridge on serverSynchronized / disconnect.
void setConnectedState(bool connectedAndSynchronized);
/// True when the connected server runs the mumble-tfar extensions
/// (channel broadcast for TFAR messages, server-side state caching).
bool serverHasTFARSupport();

/// Dispatches a received PluginDataTransmission ("TFAR" commands, "TFARST" state).
void handleIncomingPluginData(unsigned int senderSession, const char *dataID, const std::string &data);
/// Drops cached state (virtual nickname, metadata) of a disconnected session.
void forgetSession(unsigned int session);
/// Sends our TFAR state (in-game nickname + metadata) to a single client.
void sendStateToClient(unsigned int session);

/// TFAR log output routed into the Mumble client log.
void clientLogMessage(const char *message, LogLevel severity);
/// Mumble's PTT hold time — replaces TeamSpeak's delay_ptt setting.
std::chrono::milliseconds getPTTHoldDelay();
/// Directory containing TFAR resources (radio-sounds/...), with trailing separator.
std::string getPluginResourcePath();

/// Converts a Mumble channel ID to the offset TFAR channel ID.
dataType::TSChannelID channelFromMumble(unsigned int mumbleChannelId);

/// Executes a "/tfar <command>" chat command (diag, pos, rstflt, debug, ...).
void processChatCommand(const std::string &command);

/// Default Push-to-Talk button ("`" / "ё" key) — defined in GlobalShortcut_win.cpp,
/// where the platform keyboard input type lives.
QVariant defaultPTTButton();

} // namespace ts3compat

#endif // MUMBLE_MUMBLE_TFAR_TS3COMPAT_H_
