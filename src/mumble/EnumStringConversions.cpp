// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "EnumStringConversions.h"

#define AUDIO_TRANSMIT_VALUES                                  \
	PROCESS(Settings::AudioTransmit, Continuous, "Continuous") \
	PROCESS(Settings::AudioTransmit, VAD, "VAD")               \
	PROCESS(Settings::AudioTransmit, PushToTalk, "PTT")

#define VAD_SOURCE_VALUES                                \
	PROCESS(Settings::VADSource, Amplitude, "Amplitude") \
	PROCESS(Settings::VADSource, SignalToNoise, "SignalToNoise")

#define LOOP_MODE_VALUES                        \
	PROCESS(Settings::LoopMode, None, "None")   \
	PROCESS(Settings::LoopMode, Local, "Local") \
	PROCESS(Settings::LoopMode, Server, "Server")

#define CHANNEL_EXPAND_VALUES                                                \
	PROCESS(Settings::ChannelExpand, NoChannels, "NoChannels")               \
	PROCESS(Settings::ChannelExpand, ChannelsWithUsers, "ChannelsWithUsers") \
	PROCESS(Settings::ChannelExpand, AllChannels, "AllChannels")

#define CHANNEL_DRAG_VALUES                                \
	PROCESS(Settings::ChannelDrag, Ask, "Ask")             \
	PROCESS(Settings::ChannelDrag, DoNothing, "DoNothing") \
	PROCESS(Settings::ChannelDrag, Move, "Move")

#define SERVER_SHOW_VALUES                                        \
	PROCESS(Settings::ServerShow, ShowPopulated, "ShowPopulated") \
	PROCESS(Settings::ServerShow, ShowReachable, "ShowReachable") \
	PROCESS(Settings::ServerShow, ShowAll, "ShowAll")

#define TALK_STATE_VALUES                                  \
	PROCESS(Settings::TalkState, Passive, "Passive")       \
	PROCESS(Settings::TalkState, Talking, "Talking")       \
	PROCESS(Settings::TalkState, Whispering, "Whispering") \
	PROCESS(Settings::TalkState, Shouting, "Shouting")     \
	PROCESS(Settings::TalkState, MutedTalking, "MutedTalking")

#define IDLE_ACTION_VALUES                            \
	PROCESS(Settings::IdleAction, Nothing, "Nothing") \
	PROCESS(Settings::IdleAction, Deafen, "Deafen")   \
	PROCESS(Settings::IdleAction, Mute, "Mute")

#define NOISE_CANCEL_VALUES                                   \
	PROCESS(Settings::NoiseCancel, NoiseCancelOff, "Off")     \
	PROCESS(Settings::NoiseCancel, NoiseCancelSpeex, "Speex") \
	PROCESS(Settings::NoiseCancel, NoiseCancelRNN, "RNN")     \
	PROCESS(Settings::NoiseCancel, NoiseCancelBoth, "Speex&RNN")

#define ECHO_CANCEL_VALUES                                                \
	PROCESS(EchoCancelOptionID, DISABLED, "Disabled")                     \
	PROCESS(EchoCancelOptionID, SPEEX_MIXED, "Speex_MixedChannel")        \
	PROCESS(EchoCancelOptionID, SPEEX_MULTICHANNEL, "Speex_Multichannel") \
	PROCESS(EchoCancelOptionID, APPLE_AEC, "Apple_AEC")

#define PROXY_TYPE_VALUES                           \
	PROCESS(Settings::ProxyType, NoProxy, "None")   \
	PROCESS(Settings::ProxyType, HttpProxy, "Http") \
	PROCESS(Settings::ProxyType, Socks5Proxy, "Socks5")

#define ALWAYS_ON_TOP_VALUES                                             \
	PROCESS(Settings::AlwaysOnTopBehaviour, OnTopNever, "Never")         \
	PROCESS(Settings::AlwaysOnTopBehaviour, OnTopAlways, "Always")       \
	PROCESS(Settings::AlwaysOnTopBehaviour, OnTopInMinimal, "InMinimal") \
	PROCESS(Settings::AlwaysOnTopBehaviour, OnTopInNormal, "InNormal")

#define QUIT_VALUES                                                         \
	PROCESS(QuitBehavior, ALWAYS_ASK, "AlwaysAsk")                          \
	PROCESS(QuitBehavior, ASK_WHEN_CONNECTED, "AskWhenConnected")           \
	PROCESS(QuitBehavior, ALWAYS_MINIMIZE, "AlwaysMinimize")                \
	PROCESS(QuitBehavior, MINIMIZE_WHEN_CONNECTED, "MinimizeWhenConnected") \
	PROCESS(QuitBehavior, ALWAYS_QUIT, "AlwaysQuit")

#define WINDOW_LAYOUT_VALUES                                  \
	PROCESS(Settings::WindowLayout, LayoutClassic, "Classic") \
	PROCESS(Settings::WindowLayout, LayoutStacked, "Stacked") \
	PROCESS(Settings::WindowLayout, LayoutHybrid, "Hybrid")   \
	PROCESS(Settings::WindowLayout, LayoutCustom, "Custom")

#define RECORDING_MODE_VALUES                                     \
	PROCESS(Settings::RecordingMode, RecordingMixdown, "Mixdown") \
	PROCESS(Settings::RecordingMode, RecordingMultichannel, "Multichannel")

#define SEARCH_USER_ACTION_VALUES                           \
	PROCESS(Search::SearchDialog::UserAction, NONE, "None") \
	PROCESS(Search::SearchDialog::UserAction, JOIN, "Join")

#define SEARCH_CHANNEL_ACTION_VALUES                           \
	PROCESS(Search::SearchDialog::ChannelAction, NONE, "None") \
	PROCESS(Search::SearchDialog::ChannelAction, JOIN, "Join")

#define LOG_MSG_TYPE_VALUES                                                 \
	PROCESS(Log::MsgType, DebugInfo, "DebugInfo")                           \
	PROCESS(Log::MsgType, CriticalError, "CriticalError")                   \
	PROCESS(Log::MsgType, Warning, "Warning")                               \
	PROCESS(Log::MsgType, Information, "Information")                       \
	PROCESS(Log::MsgType, ServerConnected, "ServerConnected")               \
	PROCESS(Log::MsgType, ServerDisconnected, "ServerDisconnected")         \
	PROCESS(Log::MsgType, UserJoin, "UserJoin")                             \
	PROCESS(Log::MsgType, UserLeave, "UserLeave")                           \
	PROCESS(Log::MsgType, Recording, "Recording")                           \
	PROCESS(Log::MsgType, YouKicked, "YouKicked")                           \
	PROCESS(Log::MsgType, UserKicked, "UserKicked")                         \
	PROCESS(Log::MsgType, SelfMute, "SelfMute")                             \
	PROCESS(Log::MsgType, OtherSelfMute, "OtherSelfMute")                   \
	PROCESS(Log::MsgType, YouMuted, "YouMuted")                             \
	PROCESS(Log::MsgType, YouMutedOther, "YouMutedOther")                   \
	PROCESS(Log::MsgType, OtherMutedOther, "OtherMutedOther")               \
	PROCESS(Log::MsgType, ChannelJoin, "ChannelJoin")                       \
	PROCESS(Log::MsgType, ChannelLeave, "ChannelLeave")                     \
	PROCESS(Log::MsgType, PermissionDenied, "PermissionDenied")             \
	PROCESS(Log::MsgType, TextMessage, "TextMessage")                       \
	PROCESS(Log::MsgType, SelfUnmute, "SelfUnmute")                         \
	PROCESS(Log::MsgType, SelfDeaf, "SelfDeaf")                             \
	PROCESS(Log::MsgType, SelfUndeaf, "SelfUndeaf")                         \
	PROCESS(Log::MsgType, UserRenamed, "UserRenamed")                       \
	PROCESS(Log::MsgType, SelfChannelJoin, "SelfChannelJoin")               \
	PROCESS(Log::MsgType, SelfChannelJoinOther, "SelfChannelJoinOther")     \
	PROCESS(Log::MsgType, ChannelJoinConnect, "ChannelJoinConnect")         \
	PROCESS(Log::MsgType, ChannelLeaveDisconnect, "ChannelLeaveDisconnect") \
	PROCESS(Log::MsgType, PrivateTextMessage, "PrivateTextMessage")         \
	PROCESS(Log::MsgType, ChannelListeningAdd, "ChannelListeningAdd")       \
	PROCESS(Log::MsgType, ChannelListeningRemove, "ChannelListeningRemove") \
	PROCESS(Log::MsgType, PluginMessage, "PluginMessage")

#define OVERLAY_PRESETS_VALUES                                               \
	PROCESS(OverlaySettings::OverlayPresets, AvatarAndName, "AvatarAndName") \
	PROCESS(OverlaySettings::OverlayPresets, LargeSquareAvatar, "LargeSquareAvatar")

#define OVERLAY_SHOW_VALUES                                           \
	PROCESS(OverlaySettings::OverlayShow, Talking, "Talking")         \
	PROCESS(OverlaySettings::OverlayShow, Active, "Active")           \
	PROCESS(OverlaySettings::OverlayShow, HomeChannel, "HomeChannel") \
	PROCESS(OverlaySettings::OverlayShow, LinkedChannels, "LinkedChannels")

#define OVERLAY_SORT_VALUES                                             \
	PROCESS(OverlaySettings::OverlaySort, Alphabetical, "Alphabetical") \
	PROCESS(OverlaySettings::OverlaySort, LastStateChange, "LastStateChange")

#define OVERLAY_EXCLUSION_MODE_VALUES                                                             \
	PROCESS(OverlaySettings::OverlayExclusionMode, LauncherFilterExclusionMode, "LauncherFilter") \
	PROCESS(OverlaySettings::OverlayExclusionMode, WhitelistExclusionMode, "Whitelist")           \
	PROCESS(OverlaySettings::OverlayExclusionMode, BlacklistExclusionMode, "Blacklist")


#define PROCESS_ALL_ENUMS                              \
	BEFORE_CODE(Settings::AudioTransmit)               \
	AUDIO_TRANSMIT_VALUES                              \
	AFTER_CODE                                         \
	BEFORE_CODE(Settings::VADSource)                   \
	VAD_SOURCE_VALUES                                  \
	AFTER_CODE                                         \
	BEFORE_CODE(Settings::LoopMode)                    \
	LOOP_MODE_VALUES                                   \
	AFTER_CODE                                         \
	BEFORE_CODE(Settings::ChannelExpand)               \
	CHANNEL_EXPAND_VALUES                              \
	AFTER_CODE                                         \
	BEFORE_CODE(Settings::ChannelDrag)                 \
	CHANNEL_DRAG_VALUES                                \
	AFTER_CODE                                         \
	BEFORE_CODE(Settings::ServerShow)                  \
	SERVER_SHOW_VALUES                                 \
	AFTER_CODE                                         \
	BEFORE_CODE(Settings::TalkState)                   \
	TALK_STATE_VALUES                                  \
	AFTER_CODE                                         \
	BEFORE_CODE(Settings::IdleAction)                  \
	IDLE_ACTION_VALUES                                 \
	AFTER_CODE                                         \
	BEFORE_CODE(Settings::NoiseCancel)                 \
	NOISE_CANCEL_VALUES                                \
	AFTER_CODE                                         \
	BEFORE_CODE(EchoCancelOptionID)                    \
	ECHO_CANCEL_VALUES                                 \
	AFTER_CODE                                         \
	BEFORE_CODE(Settings::ProxyType)                   \
	PROXY_TYPE_VALUES                                  \
	AFTER_CODE                                         \
	BEFORE_CODE(Settings::AlwaysOnTopBehaviour)        \
	ALWAYS_ON_TOP_VALUES                               \
	AFTER_CODE                                         \
	BEFORE_CODE(QuitBehavior)                          \
	QUIT_VALUES                                        \
	AFTER_CODE                                         \
	BEFORE_CODE(Settings::WindowLayout)                \
	WINDOW_LAYOUT_VALUES                               \
	AFTER_CODE                                         \
	BEFORE_CODE(Settings::RecordingMode)               \
	RECORDING_MODE_VALUES                              \
	AFTER_CODE                                         \
	BEFORE_CODE(Search::SearchDialog::UserAction)      \
	SEARCH_USER_ACTION_VALUES                          \
	AFTER_CODE                                         \
	BEFORE_CODE(Search::SearchDialog::ChannelAction)   \
	SEARCH_CHANNEL_ACTION_VALUES                       \
	AFTER_CODE                                         \
	BEFORE_CODE(Log::MsgType)                          \
	LOG_MSG_TYPE_VALUES                                \
	AFTER_CODE                                         \
	BEFORE_CODE(OverlaySettings::OverlayPresets)       \
	OVERLAY_PRESETS_VALUES                             \
	AFTER_CODE                                         \
	BEFORE_CODE(OverlaySettings::OverlayShow)          \
	OVERLAY_SHOW_VALUES                                \
	AFTER_CODE                                         \
	BEFORE_CODE(OverlaySettings::OverlaySort)          \
	OVERLAY_SORT_VALUES                                \
	AFTER_CODE                                         \
	BEFORE_CODE(OverlaySettings::OverlayExclusionMode) \
	OVERLAY_EXCLUSION_MODE_VALUES                      \
	AFTER_CODE


#define BEFORE_CODE(enumType)              \
	const char *enumToString(enumType e) { \
		switch (e) {
#define AFTER_CODE                                \
	}                                             \
	throw "This code part should be unreachable"; \
	}
#define PROCESS(enumType, enumValue, stringValue) \
	case enumType::enumValue:                     \
		return stringValue;

PROCESS_ALL_ENUMS

#undef BEFORE_CODE
#undef AFTER_CODE
#undef PROCESS

#define BEFORE_CODE(enumType) void stringToEnum(const std::string &str, enumType &e) {
#define AFTER_CODE                                           \
	{ throw "Unable to convert given string to enum type"; } \
	}
#define PROCESS(enumType, enumValue, stringValue) \
	if (str == stringValue) {                     \
		e = enumType::enumValue;                  \
	} else

namespace details {

PROCESS_ALL_ENUMS

}

#undef PROCESS
#undef AFTER_CODE
#undef BEFORE_CODE
#undef PROCESS_ALL_ENUMS
#undef OVERLAY_EXCLUSION_MODE_VALUES
#undef OVERLAY_SORT_VALUES
#undef OVERLAY_SHOW_VALUES
#undef OVERLAY_PRESETS_VALUES
#undef LOG_MSG_TYPE_VALUES
#undef SEARCH_CHANNEL_ACTION_VALUES
#undef SEARCH_USER_ACTION_VALUES
#undef RECORDING_MODE_VALUES
#undef WINDOW_LAYOUT_VALUES
#undef ALWAYS_ON_TOP_VALUES
#undef QUIT_VALUES
#undef PROXY_TYPE_VALUES
#undef ECHO_CANCEL_VALUES
#undef NOISE_CANCEL_VALUES
#undef IDLE_ACTION_VALUES
#undef TALK_STATE_VALUES
#undef SERVER_SHOW_VALUES
#undef CHANNEL_DRAG_VALUES
#undef CHANNEL_EXPAND_VALUES
#undef LOOP_MODE_VALUES
#undef VAD_SOURCE_VALUES
#undef AUDIO_TRANSMIT_VALUES
