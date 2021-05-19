// Copyright 2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

/// This header file contains definitions of types and other components used in Mumble's plugin system

#ifndef MUMBLE_PLUGINCOMPONENT_H_
#define MUMBLE_PLUGINCOMPONENT_H_

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


/// A macro holding the exit status of a successful operation
#define STATUS_OK EC_OK
/// A macro holding the version object that is considered to correspond to an unknown version
#define VERSION_UNKNOWN Version({ 0, 0, 0 })


/// This enum's values correspond to special feature sets a plugin may provide.
/// They are meant to be or'ed together to represent the total feature set of a plugin.
enum Mumble_PluginFeature {
	/// None of the below
	FEATURE_NONE = 0,
	/// The plugin provides positional data from a game
	FEATURE_POSITIONAL = 1 << 0,
	/// The plugin modifies the input/output audio itself
	FEATURE_AUDIO = 1 << 1
};

/// This enum's values represent talking states a user can be in when using Mumble.
enum Mumble_TalkingState { INVALID = -1, PASSIVE = 0, TALKING, WHISPERING, SHOUTING, TALKING_MUTED };

/// This enum's values represent transmission modes a user might have configured. Transmission mode
/// in this context is referring to a method that determines when a user is speaking and thus when
/// to transmit audio packets.
enum Mumble_TransmissionMode { TM_CONTINOUS, TM_VOICE_ACTIVATION, TM_PUSH_TO_TALK };

/// This enum's values represent the error codes that are being used by the MumbleAPI.
/// You can get a string-representation for each error code via the errorMessage function.
enum Mumble_ErrorCode {
	EC_INTERNAL_ERROR = -2,
	EC_GENERIC_ERROR  = -1,
	EC_OK             = 0,
	EC_POINTER_NOT_FOUND,
	EC_NO_ACTIVE_CONNECTION,
	EC_USER_NOT_FOUND,
	EC_CHANNEL_NOT_FOUND,
	EC_CONNECTION_NOT_FOUND,
	EC_UNKNOWN_TRANSMISSION_MODE,
	EC_AUDIO_NOT_AVAILABLE,
	EC_INVALID_SAMPLE,
	EC_INVALID_PLUGIN_ID,
	EC_INVALID_MUTE_TARGET,
	EC_CONNECTION_UNSYNCHRONIZED,
	EC_INVALID_API_VERSION,
	EC_UNSYNCHRONIZED_BLOB,
	EC_UNKNOWN_SETTINGS_KEY,
	EC_WRONG_SETTINGS_TYPE,
	EC_SETTING_WAS_REMOVED,
	EC_DATA_TOO_BIG,
	EC_DATA_ID_TOO_LONG,
};

/// This enum's values represent error codes specific to the framework of handling positional data
/// gathering (needed for Mumble's positional audio feature).
enum Mumble_PositionalDataErrorCode {
	/// Positional data has been initialized properly
	PDEC_OK = 0,
	/// Positional data is temporarily unavailable (e.g. because the corresponding process isn't running) but might be
	/// at another point in time.
	PDEC_ERROR_TEMP,
	/// Positional data is permanently unavailable (e.g. because the respective memory offsets are outdated).
	PDEC_ERROR_PERM
};

/// This enum's values represent keys for specific settings inside Mumble.
enum Mumble_SettingsKey {
	MSK_INVALID                           = -1,
	MSK_AUDIO_INPUT_VOICE_HOLD            = 0,
	MSK_AUDIO_INPUT_VAD_SILENCE_THRESHOLD = 1,
	MSK_AUDIO_INPUT_VAD_SPEECH_THRESHOLD  = 2,
	MSK_AUDIO_OUTPUT_PA_MINIMUM_DISTANCE  = 3,
	MSK_AUDIO_OUTPUT_PA_MAXIMUM_DISTANCE  = 4,
	MSK_AUDIO_OUTPUT_PA_BLOOM             = 5,
	MSK_AUDIO_OUTPUT_PA_MINIMUM_VOLUME    = 6,
};

/// This enum's values represent the key-codes Mumble's API uses to reference keys on the keyboard.
enum Mumble_KeyCode {
	KC_INVALID = -1,

	// Non-printable characters first
	KC_NULL       = 0,
	KC_END        = 1,
	KC_LEFT       = 2,
	KC_RIGHT      = 4,
	KC_UP         = 5,
	KC_DOWN       = 6,
	KC_DELETE     = 7,
	KC_BACKSPACE  = 8,
	KC_TAB        = 9,
	KC_ENTER      = 10, // == '\n'
	KC_ESCAPE     = 27,
	KC_PAGE_UP    = 11,
	KC_PAGE_DOWN  = 12,
	KC_SHIFT      = 13,
	KC_CONTROL    = 14,
	KC_META       = 15,
	KC_ALT        = 16,
	KC_ALT_GR     = 17,
	KC_CAPSLOCK   = 18,
	KC_NUMLOCK    = 19,
	KC_SUPER      = 20, // == windows key
	KC_HOME       = 21, // == Pos1
	KC_PRINT      = 22,
	KC_SCROLLLOCK = 23,

	// Printable characters are assigned to their ASCII code
	KC_SPACE             = ' ',
	KC_EXCLAMATION_MARK  = '!',
	KC_DOUBLE_QUOTE      = '"',
	KC_HASHTAG           = '#',
	KC_DOLLAR            = '$',
	KC_PERCENT           = '%',
	KC_AMPERSAND         = '&',
	KC_SINGLE_QUOTE      = '\'',
	KC_OPEN_PARENTHESIS  = '(',
	KC_CLOSE_PARENTHESIS = ')',
	KC_ASTERISK          = '*',
	KC_PLUS              = '+',
	KC_COMMA             = ',',
	KC_MINUS             = '-',
	KC_PERIOD            = '.',
	KC_SLASH             = '/',
	KC_0                 = '0',
	KC_1                 = '1',
	KC_2                 = '2',
	KC_3                 = '3',
	KC_4                 = '4',
	KC_5                 = '5',
	KC_6                 = '6',
	KC_7                 = '7',
	KC_8                 = '8',
	KC_9                 = '9',
	KC_COLON             = ':',
	KC_SEMICOLON         = ';',
	KC_LESS_THAN         = '<',
	KC_EQUALS            = '=',
	KC_GREATER_THAN      = '>',
	KC_QUESTION_MARK     = '?',
	KC_AT_SYMBOL         = '@',
	KC_A                 = 'A',
	KC_B                 = 'B',
	KC_C                 = 'C',
	KC_D                 = 'D',
	KC_E                 = 'E',
	KC_F                 = 'F',
	KC_G                 = 'G',
	KC_H                 = 'H',
	KC_I                 = 'I',
	KC_J                 = 'J',
	KC_K                 = 'K',
	KC_L                 = 'L',
	KC_M                 = 'M',
	KC_N                 = 'N',
	KC_O                 = 'O',
	KC_P                 = 'P',
	KC_Q                 = 'Q',
	KC_R                 = 'R',
	KC_S                 = 'S',
	KC_T                 = 'T',
	KC_U                 = 'U',
	KC_V                 = 'V',
	KC_W                 = 'W',
	KC_X                 = 'X',
	KC_Y                 = 'Y',
	KC_Z                 = 'Z',
	// leave out lowercase letters (for now)
	KC_OPEN_BRACKET  = '[',
	KC_BACKSLASH     = '\\',
	KC_CLOSE_BRACKET = ']',
	KC_CIRCUMFLEX    = '^',
	KC_UNDERSCORE    = '_',
	KC_GRAVE_AKCENT  = '`',
	KC_OPEN_BRACE    = '{',
	KC_VERTICAL_BAR  = '|',
	KC_CLOSE_BRACE   = '}',
	KC_TILDE         = '~',

	// Some characters from the extended ASCII code
	KC_DEGREE_SIGN = 176,



	// F-keys
	// Start at a value of 256 as extended ASCII codes range up to 255
	KC_F1  = 256,
	KC_F2  = 257,
	KC_F3  = 258,
	KC_F4  = 259,
	KC_F5  = 260,
	KC_F6  = 261,
	KC_F7  = 262,
	KC_F8  = 263,
	KC_F9  = 264,
	KC_F10 = 265,
	KC_F11 = 266,
	KC_F12 = 267,
	KC_F13 = 268,
	KC_F14 = 269,
	KC_F15 = 270,
	KC_F16 = 271,
	KC_F17 = 272,
	KC_F18 = 273,
	KC_F19 = 274,
};

/// A struct for representing a version of the form major.minor.patch
struct Version {
	int32_t major;
	int32_t minor;
	int32_t patch;
#ifdef __cplusplus
	bool operator<(const Version &other) const {
		if (this->major != other.major) {
			return this->major < other.major;
		}
		if (this->minor != other.minor) {
			return this->minor < other.minor;
		}
		// Major and Minor are equal
		return this->patch < other.patch;
	}

	bool operator>(const Version &other) const {
		if (this->major != other.major) {
			return this->major > other.major;
		}
		if (this->minor != other.minor) {
			return this->minor > other.minor;
		}
		// Major and Minor are equal
		return this->patch > other.patch;
	}

	bool operator>=(const Version &other) const {
		if (this->major != other.major) {
			return this->major > other.major;
		}
		if (this->minor != other.minor) {
			return this->minor > other.minor;
		}
		// Major and Minor are equal
		return this->patch >= other.patch;
	}

	bool operator<=(const Version &other) const {
		if (this->major != other.major) {
			return this->major < other.major;
		}
		if (this->minor != other.minor) {
			return this->minor < other.minor;
		}
		// Major and Minor are equal
		return this->patch <= other.patch;
	}

	bool operator==(const Version &other) const {
		return this->major == other.major && this->minor == other.minor && this->patch == other.patch;
	}

	bool operator!=(const Version &other) const {
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
inline const char *errorMessage(int16_t errorCode) {
	switch (errorCode) {
		case EC_GENERIC_ERROR:
			return "Generic error";
		case EC_OK:
			return "Ok - this is not an error";
		case EC_POINTER_NOT_FOUND:
			return "Can't find the passed pointer";
		case EC_NO_ACTIVE_CONNECTION:
			return "There is currently no active connection to a server";
		case EC_USER_NOT_FOUND:
			return "Can't find the requested user";
		case EC_CHANNEL_NOT_FOUND:
			return "Can't find the requested channel";
		case EC_CONNECTION_NOT_FOUND:
			return "Can't identify the requested connection";
		case EC_UNKNOWN_TRANSMISSION_MODE:
			return "Unknown transmission mode encountered";
		case EC_AUDIO_NOT_AVAILABLE:
			return "There is currently no audio output available";
		case EC_INVALID_SAMPLE:
			return "Attempted to use invalid sample (can't play it)";
		case EC_INVALID_PLUGIN_ID:
			return "Used an invalid plugin ID";
		case EC_INVALID_MUTE_TARGET:
			return "Used an invalid mute-target";
		case EC_CONNECTION_UNSYNCHRONIZED:
			return "The requested server connection has not yet finished synchronizing";
		case EC_INVALID_API_VERSION:
			return "The used API version is invalid or not supported";
		case EC_UNSYNCHRONIZED_BLOB:
			return "The requested blob (content) has not yet been synchronized between the client and the server";
		case EC_UNKNOWN_SETTINGS_KEY:
			return "The used settings-key does not match any key known to Mumble";
		case EC_WRONG_SETTINGS_TYPE:
			return "The referenced setting has a different type than requested";
		case EC_SETTING_WAS_REMOVED:
			return "The referenced setting got removed from Mumble and is no longer used";
		case EC_DATA_TOO_BIG:
			return "The given data is too large (exceeds limit)";
		case EC_DATA_ID_TOO_LONG:
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
typedef struct Version mumble_version_t;
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

#endif // MUMBLE_PLUGINCOMPONENT_H_
