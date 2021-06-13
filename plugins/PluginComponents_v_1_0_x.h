// Copyright 2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

/// This header file contains definitions of types and other components used in Mumble's plugin system

#ifndef EXTERNAL_MUMBLE_PLUGINCOMPONENT_H_
#define EXTERNAL_MUMBLE_PLUGINCOMPONENT_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
#	include <string>
#endif

#if defined(QT_CORE_LIB) || defined(QT_VERSION)
#	include <QString>
#endif

// define the calling convention macro based on the compiler being used
#if defined(_MSC_VER)
#	define PLUGIN_CALLING_CONVENTION __cdecl
#elif defined(__MINGW32__)
#	define PLUGIN_CALLING_CONVENTION __attribute__((cdecl))
#else
#	define PLUGIN_CALLING_CONVENTION
#endif


/// This enum's values correspond to special feature sets a plugin may provide.
/// They are meant to be or'ed together to represent the total feature set of a plugin.
enum Mumble_PluginFeature {
	/// None of the below
	MUMBLE_FEATURE_NONE = 0,
	/// The plugin provides positional data from a game
	MUMBLE_FEATURE_POSITIONAL = 1 << 0,
	/// The plugin modifies the input/output audio itself
	MUMBLE_FEATURE_AUDIO = 1 << 1
};

/// This enum's values represent talking states a user can be in when using Mumble.
enum Mumble_TalkingState {
	MUMBLE_TS_INVALID = -1,
	MUMBLE_TS_PASSIVE = 0,
	MUMBLE_TS_TALKING,
	MUMBLE_TS_WHISPERING,
	MUMBLE_TS_SHOUTING,
	MUMBLE_TS_TALKING_MUTED
};

/// This enum's values represent transmission modes a user might have configured. Transmission mode
/// in this context is referring to a method that determines when a user is speaking and thus when
/// to transmit audio packets.
enum Mumble_TransmissionMode { MUMBLE_TM_CONTINOUS, MUMBLE_TM_VOICE_ACTIVATION, MUMBLE_TM_PUSH_TO_TALK };

/// This enum's values represent the error codes that are being used by the MumbleAPI.
/// You can get a string-representation for each error code via the errorMessage function.
enum Mumble_ErrorCode {
	MUMBLE_EC_INTERNAL_ERROR = -2,
	MUMBLE_EC_GENERIC_ERROR  = -1,
	MUMBLE_EC_OK             = 0,
	MUMBLE_EC_POINTER_NOT_FOUND,
	MUMBLE_EC_NO_ACTIVE_CONNECTION,
	MUMBLE_EC_USER_NOT_FOUND,
	MUMBLE_EC_CHANNEL_NOT_FOUND,
	MUMBLE_EC_CONNECTION_NOT_FOUND,
	MUMBLE_EC_UNKNOWN_TRANSMISSION_MODE,
	MUMBLE_EC_AUDIO_NOT_AVAILABLE,
	MUMBLE_EC_INVALID_SAMPLE,
	MUMBLE_EC_INVALID_PLUGIN_ID,
	MUMBLE_EC_INVALID_MUTE_TARGET,
	MUMBLE_EC_CONNECTION_UNSYNCHRONIZED,
	MUMBLE_EC_INVALID_API_VERSION,
	MUMBLE_EC_UNSYNCHRONIZED_BLOB,
	MUMBLE_EC_UNKNOWN_SETTINGS_KEY,
	MUMBLE_EC_WRONG_SETTINGS_TYPE,
	MUMBLE_EC_SETTING_WAS_REMOVED,
	MUMBLE_EC_DATA_TOO_BIG,
	MUMBLE_EC_DATA_ID_TOO_LONG,
};

/// This enum's values represent error codes specific to the framework of handling positional data
/// gathering (needed for Mumble's positional audio feature).
enum Mumble_PositionalDataErrorCode {
	/// Positional data has been initialized properly
	MUMBLE_PDEC_OK = 0,
	/// Positional data is temporarily unavailable (e.g. because the corresponding process isn't running) but might be
	/// at another point in time.
	MUMBLE_PDEC_ERROR_TEMP,
	/// Positional data is permanently unavailable (e.g. because the respective memory offsets are outdated).
	MUMBLE_PDEC_ERROR_PERM
};

/// This enum's values represent keys for specific settings inside Mumble.
enum Mumble_SettingsKey {
	MUMBLE_SK_INVALID                           = -1,
	MUMBLE_SK_AUDIO_INPUT_VOICE_HOLD            = 0,
	MUMBLE_SK_AUDIO_INPUT_VAD_SILENCE_THRESHOLD = 1,
	MUMBLE_SK_AUDIO_INPUT_VAD_SPEECH_THRESHOLD  = 2,
	MUMBLE_SK_AUDIO_OUTPUT_PA_MINIMUM_DISTANCE  = 3,
	MUMBLE_SK_AUDIO_OUTPUT_PA_MAXIMUM_DISTANCE  = 4,
	MUMBLE_SK_AUDIO_OUTPUT_PA_BLOOM             = 5,
	MUMBLE_SK_AUDIO_OUTPUT_PA_MINIMUM_VOLUME    = 6,
};

/// This enum's values represent the key-codes Mumble's API uses to reference keys on the keyboard.
enum Mumble_KeyCode {
	MUMBLE_KC_INVALID = -1,

	// Non-printable characters first
	MUMBLE_KC_NULL       = 0,
	MUMBLE_KC_END        = 1,
	MUMBLE_KC_LEFT       = 2,
	MUMBLE_KC_RIGHT      = 4,
	MUMBLE_KC_UP         = 5,
	MUMBLE_KC_DOWN       = 6,
	MUMBLE_KC_DELETE     = 7,
	MUMBLE_KC_BACKSPACE  = 8,
	MUMBLE_KC_TAB        = 9,
	MUMBLE_KC_ENTER      = 10, // == '\n'
	MUMBLE_KC_ESCAPE     = 27,
	MUMBLE_KC_PAGE_UP    = 11,
	MUMBLE_KC_PAGE_DOWN  = 12,
	MUMBLE_KC_SHIFT      = 13,
	MUMBLE_KC_CONTROL    = 14,
	MUMBLE_KC_META       = 15,
	MUMBLE_KC_ALT        = 16,
	MUMBLE_KC_ALT_GR     = 17,
	MUMBLE_KC_CAPSLOCK   = 18,
	MUMBLE_KC_NUMLOCK    = 19,
	MUMBLE_KC_SUPER      = 20, // == windows key
	MUMBLE_KC_HOME       = 21, // == Pos1
	MUMBLE_KC_PRINT      = 22,
	MUMBLE_KC_SCROLLLOCK = 23,

	// Printable characters are assigned to their ASCII code
	MUMBLE_KC_SPACE             = ' ',
	MUMBLE_KC_EXCLAMATION_MARK  = '!',
	MUMBLE_KC_DOUBLE_QUOTE      = '"',
	MUMBLE_KC_HASHTAG           = '#',
	MUMBLE_KC_DOLLAR            = '$',
	MUMBLE_KC_PERCENT           = '%',
	MUMBLE_KC_AMPERSAND         = '&',
	MUMBLE_KC_SINGLE_QUOTE      = '\'',
	MUMBLE_KC_OPEN_PARENTHESIS  = '(',
	MUMBLE_KC_CLOSE_PARENTHESIS = ')',
	MUMBLE_KC_ASTERISK          = '*',
	MUMBLE_KC_PLUS              = '+',
	MUMBLE_KC_COMMA             = ',',
	MUMBLE_KC_MINUS             = '-',
	MUMBLE_KC_PERIOD            = '.',
	MUMBLE_KC_SLASH             = '/',
	MUMBLE_KC_0                 = '0',
	MUMBLE_KC_1                 = '1',
	MUMBLE_KC_2                 = '2',
	MUMBLE_KC_3                 = '3',
	MUMBLE_KC_4                 = '4',
	MUMBLE_KC_5                 = '5',
	MUMBLE_KC_6                 = '6',
	MUMBLE_KC_7                 = '7',
	MUMBLE_KC_8                 = '8',
	MUMBLE_KC_9                 = '9',
	MUMBLE_KC_COLON             = ':',
	MUMBLE_KC_SEMICOLON         = ';',
	MUMBLE_KC_LESS_THAN         = '<',
	MUMBLE_KC_EQUALS            = '=',
	MUMBLE_KC_GREATER_THAN      = '>',
	MUMBLE_KC_QUESTION_MARK     = '?',
	MUMBLE_KC_AT_SYMBOL         = '@',
	MUMBLE_KC_A                 = 'A',
	MUMBLE_KC_B                 = 'B',
	MUMBLE_KC_C                 = 'C',
	MUMBLE_KC_D                 = 'D',
	MUMBLE_KC_E                 = 'E',
	MUMBLE_KC_F                 = 'F',
	MUMBLE_KC_G                 = 'G',
	MUMBLE_KC_H                 = 'H',
	MUMBLE_KC_I                 = 'I',
	MUMBLE_KC_J                 = 'J',
	MUMBLE_KC_K                 = 'K',
	MUMBLE_KC_L                 = 'L',
	MUMBLE_KC_M                 = 'M',
	MUMBLE_KC_N                 = 'N',
	MUMBLE_KC_O                 = 'O',
	MUMBLE_KC_P                 = 'P',
	MUMBLE_KC_Q                 = 'Q',
	MUMBLE_KC_R                 = 'R',
	MUMBLE_KC_S                 = 'S',
	MUMBLE_KC_T                 = 'T',
	MUMBLE_KC_U                 = 'U',
	MUMBLE_KC_V                 = 'V',
	MUMBLE_KC_W                 = 'W',
	MUMBLE_KC_X                 = 'X',
	MUMBLE_KC_Y                 = 'Y',
	MUMBLE_KC_Z                 = 'Z',
	// leave out lowercase letters (for now)
	MUMBLE_KC_OPEN_BRACKET  = '[',
	MUMBLE_KC_BACKSLASH     = '\\',
	MUMBLE_KC_CLOSE_BRACKET = ']',
	MUMBLE_KC_CIRCUMFLEX    = '^',
	MUMBLE_KC_UNDERSCORE    = '_',
	MUMBLE_KC_GRAVE_AKCENT  = '`',
	MUMBLE_KC_OPEN_BRACE    = '{',
	MUMBLE_KC_VERTICAL_BAR  = '|',
	MUMBLE_KC_CLOSE_BRACE   = '}',
	MUMBLE_KC_TILDE         = '~',

	// Some characters from the extended ASCII code
	MUMBLE_KC_DEGREE_SIGN = 176,



	// F-keys
	// Start at a value of 256 as extended ASCII codes range up to 255
	MUMBLE_KC_F1  = 256,
	MUMBLE_KC_F2  = 257,
	MUMBLE_KC_F3  = 258,
	MUMBLE_KC_F4  = 259,
	MUMBLE_KC_F5  = 260,
	MUMBLE_KC_F6  = 261,
	MUMBLE_KC_F7  = 262,
	MUMBLE_KC_F8  = 263,
	MUMBLE_KC_F9  = 264,
	MUMBLE_KC_F10 = 265,
	MUMBLE_KC_F11 = 266,
	MUMBLE_KC_F12 = 267,
	MUMBLE_KC_F13 = 268,
	MUMBLE_KC_F14 = 269,
	MUMBLE_KC_F15 = 270,
	MUMBLE_KC_F16 = 271,
	MUMBLE_KC_F17 = 272,
	MUMBLE_KC_F18 = 273,
	MUMBLE_KC_F19 = 274,
};

/// A struct for representing a version of the form major.minor.patch
struct MumbleVersion {
	int32_t major;
	int32_t minor;
	int32_t patch;
#ifdef __cplusplus
	bool operator<(const MumbleVersion &other) const {
		if (this->major != other.major) {
			return this->major < other.major;
		}
		if (this->minor != other.minor) {
			return this->minor < other.minor;
		}
		// Major and Minor are equal
		return this->patch < other.patch;
	}

	bool operator>(const MumbleVersion &other) const {
		if (this->major != other.major) {
			return this->major > other.major;
		}
		if (this->minor != other.minor) {
			return this->minor > other.minor;
		}
		// Major and Minor are equal
		return this->patch > other.patch;
	}

	bool operator>=(const MumbleVersion &other) const {
		if (this->major != other.major) {
			return this->major > other.major;
		}
		if (this->minor != other.minor) {
			return this->minor > other.minor;
		}
		// Major and Minor are equal
		return this->patch >= other.patch;
	}

	bool operator<=(const MumbleVersion &other) const {
		if (this->major != other.major) {
			return this->major < other.major;
		}
		if (this->minor != other.minor) {
			return this->minor < other.minor;
		}
		// Major and Minor are equal
		return this->patch <= other.patch;
	}

	bool operator==(const MumbleVersion &other) const {
		return this->major == other.major && this->minor == other.minor && this->patch == other.patch;
	}

	bool operator!=(const MumbleVersion &other) const {
		return this->major != other.major || this->minor != other.minor || this->patch != other.patch;
	}

	operator std::string() const {
		return std::string("v") + std::to_string(this->major) + std::string(".") + std::to_string(this->minor)
			   + std::string(".") + std::to_string(this->patch);
	}

#	if defined(QT_CORE_LIB) || defined(QT_VERSION)
	operator QString() const {
		return QString::fromLatin1("v%0.%1.%2").arg(this->major).arg(this->minor).arg(this->patch);
	}
#	endif
#endif
};

/// Obtains a String representation for the given numeric error code.
/// Note that the exact String representation corresponding to an error code may change and is thus
/// not part of the plugin API as such. This function acts merely as a convenience helper for printing
/// errors in a meaningful way.
///
/// @param errorCode The error code to get the String representation for
/// @returns The error message coresponding to the given error code. The message
/// is encoded as a C-string and is static, meaning that it is safe to use the
/// returned pointer in your code.
inline const char *mumble_errorMessage(int16_t errorCode) {
	switch (errorCode) {
		case MUMBLE_EC_GENERIC_ERROR:
			return "Generic error";
		case MUMBLE_EC_OK:
			return "Ok - this is not an error";
		case MUMBLE_EC_POINTER_NOT_FOUND:
			return "Can't find the passed pointer";
		case MUMBLE_EC_NO_ACTIVE_CONNECTION:
			return "There is currently no active connection to a server";
		case MUMBLE_EC_USER_NOT_FOUND:
			return "Can't find the requested user";
		case MUMBLE_EC_CHANNEL_NOT_FOUND:
			return "Can't find the requested channel";
		case MUMBLE_EC_CONNECTION_NOT_FOUND:
			return "Can't identify the requested connection";
		case MUMBLE_EC_UNKNOWN_TRANSMISSION_MODE:
			return "Unknown transmission mode encountered";
		case MUMBLE_EC_AUDIO_NOT_AVAILABLE:
			return "There is currently no audio output available";
		case MUMBLE_EC_INVALID_SAMPLE:
			return "Attempted to use invalid sample (can't play it)";
		case MUMBLE_EC_INVALID_PLUGIN_ID:
			return "Used an invalid plugin ID";
		case MUMBLE_EC_INVALID_MUTE_TARGET:
			return "Used an invalid mute-target";
		case MUMBLE_EC_CONNECTION_UNSYNCHRONIZED:
			return "The requested server connection has not yet finished synchronizing";
		case MUMBLE_EC_INVALID_API_VERSION:
			return "The used API version is invalid or not supported";
		case MUMBLE_EC_UNSYNCHRONIZED_BLOB:
			return "The requested blob (content) has not yet been synchronized between the client and the server";
		case MUMBLE_EC_UNKNOWN_SETTINGS_KEY:
			return "The used settings-key does not match any key known to Mumble";
		case MUMBLE_EC_WRONG_SETTINGS_TYPE:
			return "The referenced setting has a different type than requested";
		case MUMBLE_EC_SETTING_WAS_REMOVED:
			return "The referenced setting got removed from Mumble and is no longer used";
		case MUMBLE_EC_DATA_TOO_BIG:
			return "The given data is too large (exceeds limit)";
		case MUMBLE_EC_DATA_ID_TOO_LONG:
			return "The given data ID is too long (exceeds limit)";
		default:
			return "Unknown error code";
	}
}


/// This struct is used to return Strings from a plugin to Mumble. It is needed in order to
/// work around the limitation of std::string not being part of C (it holds important information
/// about the String's lifetime management requirements).
struct MumbleStringWrapper {
	/// The pointer to the actual String data
	const char *data;
	/// The size of the pointed String data
	size_t size;
	/// Whether the wrapped String needs to be released
	/// after its usage. Instances for which this would be
	/// false: Static Strings, String literals
	bool needsReleasing;
};

/// Typedef for the type of a talking state
typedef enum Mumble_TalkingState mumble_talking_state_t;
/// Typedef for the type of a transmission mode
typedef enum Mumble_TransmissionMode mumble_transmission_mode_t;
/// Typedef for the type of a version
typedef struct MumbleVersion mumble_version_t;
/// Typedef for the type of a connection
typedef int32_t mumble_connection_t;
/// Typedef for the type of a user
typedef uint32_t mumble_userid_t;
/// Typedef for the type of a channel
typedef int32_t mumble_channelid_t;
/// Typedef for the type of an error (code)
typedef enum Mumble_ErrorCode mumble_error_t;
/// Typedef for the type of a plugin ID
typedef uint32_t mumble_plugin_id_t;
/// Typedef for the type of a key to a setting in Mumble
typedef enum Mumble_SettingsKey mumble_settings_key_t;
/// Typedef for the type of a key-code
typedef enum Mumble_KeyCode mumble_keycode_t;



/// A constant holding the exit status of a successful operation
static const mumble_error_t MUMBLE_STATUS_OK = MUMBLE_EC_OK;
/// A constant holding the version object that is considered to correspond to an unknown version
static const mumble_version_t MUMBLE_VERSION_UNKNOWN = { 0, 0, 0 };

#endif // EXTERNAL_MUMBLE_PLUGINCOMPONENT_H_
