// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_ENUMSTRINGCONVERSIONS_H_
#define MUMBLE_MUMBLE_ENUMSTRINGCONVERSIONS_H_


#include "EchoCancelOption.h"
#include "Log.h"
#include "SearchDialog.h"
#include "Settings.h"

#include <type_traits>

const char *enumToString(Settings::AudioTransmit e);
const char *enumToString(Settings::VADSource e);
const char *enumToString(Settings::LoopMode e);
const char *enumToString(Settings::ChannelExpand e);
const char *enumToString(Settings::ChannelDrag e);
const char *enumToString(Settings::ServerShow e);
const char *enumToString(Settings::TalkState e);
const char *enumToString(Settings::IdleAction e);
const char *enumToString(Settings::NoiseCancel e);
const char *enumToString(EchoCancelOptionID e);
const char *enumToString(Settings::ProxyType e);
const char *enumToString(Settings::AlwaysOnTopBehaviour e);
const char *enumToString(QuitBehavior e);
const char *enumToString(Settings::WindowLayout e);
const char *enumToString(Settings::RecordingMode e);
const char *enumToString(Search::SearchDialog::UserAction e);
const char *enumToString(Search::SearchDialog::ChannelAction e);
const char *enumToString(Log::MsgType e);
const char *enumToString(OverlaySettings::OverlayPresets e);
const char *enumToString(OverlaySettings::OverlayShow e);
const char *enumToString(OverlaySettings::OverlaySort e);
const char *enumToString(OverlaySettings::OverlayExclusionMode e);

namespace details {

void stringToEnum(const std::string &str, Settings::AudioTransmit &e);
void stringToEnum(const std::string &str, Settings::VADSource &e);
void stringToEnum(const std::string &str, Settings::LoopMode &e);
void stringToEnum(const std::string &str, Settings::ChannelExpand &e);
void stringToEnum(const std::string &str, Settings::ChannelDrag &e);
void stringToEnum(const std::string &str, Settings::ServerShow &e);
void stringToEnum(const std::string &str, Settings::TalkState &e);
void stringToEnum(const std::string &str, Settings::IdleAction &e);
void stringToEnum(const std::string &str, Settings::NoiseCancel &e);
void stringToEnum(const std::string &str, EchoCancelOptionID &e);
void stringToEnum(const std::string &str, Settings::ProxyType &e);
void stringToEnum(const std::string &str, Settings::AlwaysOnTopBehaviour &e);
void stringToEnum(const std::string &str, QuitBehavior &e);
void stringToEnum(const std::string &str, Settings::WindowLayout &e);
void stringToEnum(const std::string &str, Settings::RecordingMode &e);
void stringToEnum(const std::string &str, Search::SearchDialog::UserAction &e);
void stringToEnum(const std::string &str, Search::SearchDialog::ChannelAction &e);
void stringToEnum(const std::string &str, Log::MsgType &e);
void stringToEnum(const std::string &str, OverlaySettings::OverlayPresets &e);
void stringToEnum(const std::string &str, OverlaySettings::OverlayShow &e);
void stringToEnum(const std::string &str, OverlaySettings::OverlaySort &e);
void stringToEnum(const std::string &str, OverlaySettings::OverlayExclusionMode &e);

} // namespace details

template< typename T > T stringToEnum(const std::string &str) {
	static_assert(std::is_enum< T >::value, "Only enums can be converted to strings with this function");

	T enumVal;
	details::stringToEnum(str, enumVal);

	return enumVal;
}

#endif // MUMBLE_MUMBLE_ENUMSTRINGCONVERSIONS_H_
