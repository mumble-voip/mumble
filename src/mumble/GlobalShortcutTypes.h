// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_GLOBALSHORTCUTTYPES_H_
#define MUMBLE_MUMBLE_GLOBALSHORTCUTTYPES_H_

/**
 * The entries in this namespace/enum represent the shortcut types (often referred to as a shortcut's "index")
 * of the existing global shortcuts.
 *
 * IMPORTANT: ONLY EVER APPEND TO THE END OF THE ENUM!
 */
namespace GlobalShortcutType {
enum Type {
	PushToTalk = 1,
	ResetAudio,
	MuteSelf,
	DeafenSelf,
	UnlinkPlugin,
	PushToMute,
	JoinChannel,
	ToggleOverlay,
	ToggleMinimalView,
	VolumeUp,
	VolumeDown,
	Whisper_Shout,
	LinkChannel,
	CycleTransmitMode,
	ToggleMainWindowVisibility,
	UsePushToTalk,
	UseContinous,
	UseVAD,
	SendTextMessage,
	SendTextMessageClipboard,
	ToggleTalkingUI,
	ToggleSearch,
	ListenToChannel,
	ServerConnect,
	ServerDisconnect,
	ServerInformation,
	ServerTokens,
	ServerUserList,
	ServerBanList,
	SelfPrioritySpeaker,
	Recording,
	SelfComment,
	ServerTexture,
	ServerTextureRemove,
	SelfRegister,
	AudioStats,
	ConfigDialog,
	AudioWizard,
	ConfigCert,
	AudioTTS,
	HelpAbout,
	HelpAboutQt,
	HelpVersionCheck,
	TogglePositionalAudio,
};

// A few assertions meant to catch, if anyone inserts a new value in-between instead of appending
// to the end of the enum
static_assert(PushToTalk == 1, "You may only append to the end of the enum!");
static_assert(ToggleMinimalView == 9, "You may only append to the end of the enum!");
static_assert(ToggleSearch == 22, "You may only append to the end of the enum!");
static_assert(HelpVersionCheck == 43, "You may only append to the end of the enum!");
static_assert(TogglePositionalAudio == 44, "You may only append to the end of the enum!");
} // namespace GlobalShortcutType

#endif // MUMBLE_MUMBLE_GLOBALSHORTCUTTYPES_H_
