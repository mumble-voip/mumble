// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////// CONTROL OVER WHAT SECTIONS TO INCLUDE ////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

// Whether or not to include default implementations
#ifdef MUMBLE_PLUGIN_NO_DEFAULT_FUNCTION_DEFINITIONS
#	undef EXTERNAL_MUMBLE_PLUGIN_DEFAULT_IMPLEMENTATIONS_
#	define EXTERNAL_MUMBLE_PLUGIN_DEFAULT_IMPLEMENTATIONS_
#endif

// Whether or not to create the MumbleAPI typedef
#undef MUMBLE_PLUGIN_CREATE_MUMBLE_API_TYPEDEF
#ifndef MUMBLE_PLUGIN_NO_API_TYPEDEF
#	define MUMBLE_PLUGIN_CREATE_MUMBLE_API_TYPEDEF
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// MACRO DEFINITIONS //////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef EXTERNAL_MUMBLE_PLUGIN_MACROS_
#	define EXTERNAL_MUMBLE_PLUGIN_MACROS_

/*
 * Plugin version information
 */
#	define MUMBLE_PLUGIN_INTERFACE_MAJOR_MACRO 1
#	define MUMBLE_PLUGIN_INTERFACE_MINOR_MACRO 2
#	define MUMBLE_PLUGIN_INTERFACE_PATCH_MACRO 0

// Allow the selected API version be overwritten by external definitions
#	ifndef MUMBLE_PLUGIN_API_MAJOR_MACRO
#		define MUMBLE_PLUGIN_API_MAJOR_MACRO 1
#	endif
#	ifndef MUMBLE_PLUGIN_API_MINOR_MACRO
#		define MUMBLE_PLUGIN_API_MINOR_MACRO 2
#	endif
#	ifndef MUMBLE_PLUGIN_API_PATCH_MACRO
#		define MUMBLE_PLUGIN_API_PATCH_MACRO 0
#	endif

#	define MUMBLE_PLUGIN_FUNCTIONS_MAJOR_MACRO 1
#	define MUMBLE_PLUGIN_FUNCTIONS_MINOR_MACRO 1
#	define MUMBLE_PLUGIN_FUNCTIONS_PATCH_MACRO 0

/*
 * MUMBLE_PLUGIN_EXPORT
 */
#	if defined(__GNUC__) && !defined(__MINGW32__) // GCC on Unix-like systems
#		define MUMBLE_PLUGIN_EXPORT __attribute__((visibility("default")))
#	elif defined(_MSC_VER)
#		define MUMBLE_PLUGIN_EXPORT __declspec(dllexport)
#	elif defined(__MINGW32__)
#		define MUMBLE_PLUGIN_EXPORT __attribute__((dllexport))
#	else
#		error No MUMBLE_PLUGIN_EXPORT definition available
#	endif

/*
 * MUMBLE_PLUGIN_CALLING_CONVENTION
 */
#	if defined(_MSC_VER)
#		define MUMBLE_PLUGIN_CALLING_CONVENTION __cdecl
#	elif defined(__MINGW32__)
#		define MUMBLE_PLUGIN_CALLING_CONVENTION __attribute__((cdecl))
#	else
#		define MUMBLE_PLUGIN_CALLING_CONVENTION
#	endif

/*
 * MUMBLE_PLUGIN_HAS_QT
 */
#	if defined(QT_CORE_LIB) || defined(QT_VERSION)
#		define MUMBLE_PLUGIN_HAS_QT
#	endif


/*
 * MUMBLE_PLUGIN_VERSION_CHECK
 * Converts a major, minor and patch version number into a single integer to be used in preprocessor
 * if conditions
 */
#	define MUMBLE_PLUGIN_VERSION_CHECK(major, minor, patch) (((major) << 16) | ((minor) << 8) | (patch))

/*
 * MUMBLE_PLUGIN_CONSTEXPR
 */
#	ifdef __cplusplus
#		define MUMBLE_PLUGIN_CONSTEXPR constexpr
#	else
#		define MUMBLE_PLUGIN_CONSTEXPR
#	endif

/*
 * MUMBLE_EXTERN_C_BEGIN / MUMBLE_EXTERN_C_END
 */
#	ifdef __cplusplus
#		define MUMBLE_EXTERN_C_BEGIN extern "C" {
#		define MUMBLE_EXTERN_C_END }
#	else
#		define MUMBLE_EXTERN_C_BEGIN
#		define MUMBLE_EXTERN_C_END
#	endif

// For more info on the necessity of the with/without pattern have a look
// at https://stackoverflow.com/a/3419392

/*
 * MUMBLE_CONCAT
 * Concatenates the given values WITHOUT macro expansion
 */
#	define MUMBLE_CONCAT(a, b) a##_##b
/**
 * MUMBLE_ECONCAT
 * Concatenates the given values WITH macro expansion
 */
#	define MUMBLE_ECONCAT(a, b) MUMBLE_CONCAT(a, b)
/**
 * MUMBLE_QUOTE
 * Quotes the given value WITHOUT macro expansion
 */
#	define MUMBLE_QUOTE(arg) #    arg
/**
 * MUMBLE_EQUOTE
 * Quotes the given value WITH macro expansion
 */
#	define MUMBLE_EQUOTE(arg) MUMBLE_QUOTE(arg)

#endif // EXTERNAL_MUMBLE_PLUGIN_MACROS_



///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////// PLUGIN TYPES /////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef EXTERNAL_MUMBLE_PLUGIN_TYPES_
#	define EXTERNAL_MUMBLE_PLUGIN_TYPES_

#	include <stdint.h>
#	include <stddef.h>
#	include <stdbool.h>

#	ifdef __cplusplus
#		include <string>

#		ifdef MUMBLE_PLUGIN_HAS_QT
#			include <QString>
#		endif
#	endif

MUMBLE_EXTERN_C_BEGIN

/**
 * This enum's values correspond to special feature sets a plugin may provide.
 * They are meant to be or'ed together to represent the total feature set of a plugin.
 */
enum Mumble_PluginFeature {
	/**
	 * None of the below
	 */
	MUMBLE_FEATURE_NONE = 0,
	/**
	 * The plugin provides positional data from a game
	 */
	MUMBLE_FEATURE_POSITIONAL = 1 << 0,
	/**
	 * The plugin modifies the input/output audio itself
	 */
	MUMBLE_FEATURE_AUDIO = 1 << 1,
};

/**
 * This enum's values represent talking states a user can be in when using Mumble.
 */
enum Mumble_TalkingState {
	MUMBLE_TS_INVALID = -1,
	MUMBLE_TS_PASSIVE = 0,
	MUMBLE_TS_TALKING,
	MUMBLE_TS_WHISPERING,
	MUMBLE_TS_SHOUTING,
	MUMBLE_TS_TALKING_MUTED,
};

/**
 * This enum's values represent transmission modes a user might have configured. Transmission mode
 * in this context is referring to a method that determines when a user is speaking and thus when
 * to transmit audio packets.
 */
enum Mumble_TransmissionMode {
	MUMBLE_TM_CONTINOUS,
	MUMBLE_TM_VOICE_ACTIVATION,
	MUMBLE_TM_PUSH_TO_TALK,
};

/**
 * This enum's values represent the error codes that are being used by the MumbleAPI.
 * You can get a string-representation for each error code via the errorMessage function.
 */
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
	MUMBLE_EC_API_REQUEST_TIMEOUT,
	MUMBLE_EC_OPERATION_UNSUPPORTED_BY_SERVER,
};

/**
 * This enum's values represent error codes specific to the framework of handling positional data
 * gathering (needed for Mumble's positional audio feature).
 */
enum Mumble_PositionalDataErrorCode {
	/**
	 * Positional data has been initialized properly
	 */
	MUMBLE_PDEC_OK = 0,
	/**
	 * Positional data is temporarily unavailable (e.g. because the corresponding process isn't running) but might be
	 * at another point in time.
	 */
	MUMBLE_PDEC_ERROR_TEMP,
	/**
	 * Positional data is permanently unavailable (e.g. because the respective memory offsets are outdated).
	 */
	MUMBLE_PDEC_ERROR_PERM,
};

/**
 * This enum's values represent keys for specific settings inside Mumble.
 */
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

/**
 * This enum's values represent the key-codes Mumble's API uses to reference keys on the keyboard.
 */
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

/**
 * A struct for representing a version of the form major.minor.patch
 */
struct MumbleVersion {
	int32_t major;
	int32_t minor;
	int32_t patch;
#	ifdef __cplusplus
	explicit operator std::string() const {
		return std::string("v") + std::to_string(this->major) + std::string(".") + std::to_string(this->minor)
			   + std::string(".") + std::to_string(this->patch);
	}

#		ifdef MUMBLE_PLUGIN_HAS_QT
	explicit operator QString() const {
		return QString::fromLatin1("v%0.%1.%2").arg(this->major).arg(this->minor).arg(this->patch);
	}
#		endif
#	endif // __cplusplus
};

/**
 * This struct is used to return Strings from a plugin to Mumble. It is needed in order to
 * work around the limitation of std::string not being part of C (it holds important information
 * about the String's lifetime management requirements).
 */
struct MumbleStringWrapper {
	/**
	 * The pointer to the actual String data
	 */
	const char *data;
	/**
	 * The size of the pointed String data
	 */
	size_t size;
	/**
	 * Whether the wrapped String needs to be released
	 * after its usage. Instances for which this would be
	 * false: Static Strings, String literals
	 */
	bool needsReleasing;
};

MUMBLE_EXTERN_C_END

#endif // EXTERNAL_MUMBLE_PLUGIN_TYPES_


///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// Type aliases / Typedefs /////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef EXTERNAL_MUMBLE_PLUGIN_TYPEDEFS_
#	define EXTERNAL_MUMBLE_PLUGIN_TYPEDEFS_

/**
 * Typedef for the type of a talking state
 */
typedef enum Mumble_TalkingState mumble_talking_state_t;
/**
 * Typedef for the type of a transmission mode
 */
typedef enum Mumble_TransmissionMode mumble_transmission_mode_t;
/**
 * Typedef for the type of a version
 */
typedef struct MumbleVersion mumble_version_t;
/**
 * Typedef for the type of a connection
 */
typedef int32_t mumble_connection_t;
/**
 * Typedef for the type of a user
 */
typedef uint32_t mumble_userid_t;
/**
 * Typedef for the type of a channel
 */
typedef int32_t mumble_channelid_t;
/**
 * Typedef for the type of an error (code)
 */
typedef enum Mumble_ErrorCode mumble_error_t;
/**
 * Typedef for the type of a plugin ID
 */
typedef uint32_t mumble_plugin_id_t;
/**
 * Typedef for the type of a key to a setting in Mumble
 */
typedef enum Mumble_SettingsKey mumble_settings_key_t;
/**
 * Typedef for the type of a key-code
 */
typedef enum Mumble_KeyCode mumble_keycode_t;

#endif // EXTERNAL_MUMBLE_PLUGIN_TYPEDEFS_



///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// Non-enum Constants /////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef EXTERNAL_MUMBLE_PLUGIN_CONSTANTS_
#	define EXTERNAL_MUMBLE_PLUGIN_CONSTANTS_

/*
 * Version constants
 */

static const MUMBLE_PLUGIN_CONSTEXPR int32_t MUMBLE_PLUGIN_INTERFACE_MAJOR = MUMBLE_PLUGIN_INTERFACE_MAJOR_MACRO;
static const MUMBLE_PLUGIN_CONSTEXPR int32_t MUMBLE_PLUGIN_INTERFACE_MINOR = MUMBLE_PLUGIN_INTERFACE_MINOR_MACRO;
static const MUMBLE_PLUGIN_CONSTEXPR int32_t MUMBLE_PLUGIN_INTERFACE_PATCH = MUMBLE_PLUGIN_INTERFACE_PATCH_MACRO;
static const MUMBLE_PLUGIN_CONSTEXPR mumble_version_t MUMBLE_PLUGIN_INTERFACE_VERSION = {
	MUMBLE_PLUGIN_INTERFACE_MAJOR, MUMBLE_PLUGIN_INTERFACE_MINOR, MUMBLE_PLUGIN_INTERFACE_PATCH
};

static const MUMBLE_PLUGIN_CONSTEXPR int32_t MUMBLE_PLUGIN_API_MAJOR            = MUMBLE_PLUGIN_API_MAJOR_MACRO;
static const MUMBLE_PLUGIN_CONSTEXPR int32_t MUMBLE_PLUGIN_API_MINOR            = MUMBLE_PLUGIN_API_MINOR_MACRO;
static const MUMBLE_PLUGIN_CONSTEXPR int32_t MUMBLE_PLUGIN_API_PATCH            = MUMBLE_PLUGIN_API_PATCH_MACRO;
static const MUMBLE_PLUGIN_CONSTEXPR mumble_version_t MUMBLE_PLUGIN_API_VERSION = { MUMBLE_PLUGIN_API_MAJOR,
																					MUMBLE_PLUGIN_API_MINOR,
																					MUMBLE_PLUGIN_API_PATCH };

static const MUMBLE_PLUGIN_CONSTEXPR int32_t MUMBLE_PLUGIN_FUNCTIONS_MAJOR = MUMBLE_PLUGIN_FUNCTIONS_MAJOR_MACRO;
static const MUMBLE_PLUGIN_CONSTEXPR int32_t MUMBLE_PLUGIN_FUNCTIONS_MINOR = MUMBLE_PLUGIN_FUNCTIONS_MINOR_MACRO;
static const MUMBLE_PLUGIN_CONSTEXPR int32_t MUMBLE_PLUGIN_FUNCTIONS_PATCH = MUMBLE_PLUGIN_FUNCTIONS_PATCH_MACRO;
static const MUMBLE_PLUGIN_CONSTEXPR mumble_version_t MUMBLE_PLUGIN_FUNCTIONS_VERSION = {
	MUMBLE_PLUGIN_FUNCTIONS_MAJOR, MUMBLE_PLUGIN_FUNCTIONS_MINOR, MUMBLE_PLUGIN_FUNCTIONS_PATCH
};

/**
 * The exit status of a successful operation
 */
static const MUMBLE_PLUGIN_CONSTEXPR mumble_error_t MUMBLE_STATUS_OK = MUMBLE_EC_OK;
/**
 * A version object that is considered to correspond to an unknown version
 */
static const MUMBLE_PLUGIN_CONSTEXPR mumble_version_t MUMBLE_VERSION_UNKNOWN = { 0, 0, 0 };

#endif // EXTERNAL_MUMBLE_PLUGIN_CONSTANTS_



///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// C convenience implementations /////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef EXTERNAL_MUMBLE_PLUGIN_C_CONVENIENCE_IMPLS_
#	define EXTERNAL_MUMBLE_PLUGIN_C_CONVENIENCE_IMPLS_

/**
 * Obtains a String representation for the given numeric error code.
 * Note that the exact String representation corresponding to an error code may change and is thus
 * not part of the plugin API as such. This function acts merely as a convenience helper for printing
 * errors in a meaningful way.
 *
 * @param errorCode The error code to get the String representation for
 * @returns The error message coresponding to the given error code. The message
 * is encoded as a C-string and is static, meaning that it is safe to use the
 * returned pointer in your code.
 */
MUMBLE_PLUGIN_CONSTEXPR inline const char *mumble_errorMessage(int16_t errorCode) {
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
		case MUMBLE_EC_API_REQUEST_TIMEOUT:
			return "A blocking API call took too long and was thus aborted (probably preventing a deadlock)";
		case MUMBLE_EC_OPERATION_UNSUPPORTED_BY_SERVER:
			return "The requested API operation depends on server-side functionality, not supported by the server "
				   "you're connected to";
	}

	return "Unknown error code";
}
#endif // EXTERNAL_MUMBLE_PLUGIN_C_CONVENIENCE_IMPLS_



///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// C++ convenience implementations ////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

#if !defined(EXTERNAL_MUMBLE_PLUGIN_CPP_CONVENIENCE_IMPLS_) && defined(__cplusplus)
#	define EXTERNAL_MUMBLE_PLUGIN_CPP_CONVENIENCE_IMPLS_

/*
 * Comparison operator overloads for MumbleVersion structs
 */
constexpr bool operator<(const MumbleVersion &lhs, const MumbleVersion &rhs) {
	if (lhs.major != rhs.major) {
		return lhs.major < rhs.major;
	}
	if (lhs.minor != rhs.minor) {
		return lhs.minor < rhs.minor;
	}
	// Major and Minor are equal
	return lhs.patch < rhs.patch;
}

constexpr bool operator==(const MumbleVersion &lhs, const MumbleVersion &rhs) {
	return lhs.major == rhs.major && lhs.minor == rhs.minor && lhs.patch == rhs.patch;
}


constexpr bool operator!=(const MumbleVersion &lhs, const MumbleVersion &rhs) {
	return !(lhs == rhs);
}

constexpr bool operator>(const MumbleVersion &lhs, const MumbleVersion &rhs) {
	return !(lhs == rhs || lhs < rhs);
}

constexpr bool operator>=(const MumbleVersion &lhs, const MumbleVersion &rhs) {
	return lhs == rhs || lhs > rhs;
}

constexpr bool operator<=(const MumbleVersion &lhs, const MumbleVersion &rhs) {
	return lhs == rhs || lhs < rhs;
}

#endif // EXTERNAL_MUMBLE_PLUGIN_CPP_CONVENIENCE_IMPLS_



///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////// Plugin functions //////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef EXTERNAL_MUMBLE_PLUGIN_FUNCTIONS_
#	define EXTERNAL_MUMBLE_PLUGIN_FUNCTIONS_

#	include <stdint.h>
#	include <stddef.h>
#	include <stdbool.h>

MUMBLE_EXTERN_C_BEGIN

// >>>>>>>>>>>>>>>>>>>> MANDATORY functions <<<<<<<<<<<<<<<<<<<<

/**
 * Gets called right after loading the plugin in order to let the plugin initialize.
 *
 * Registers the ID of this plugin.
 * @param id The ID for this plugin. This is the ID Mumble will reference this plugin with
 * and by which this plugin can identify itself when communicating with Mumble.
 * @returns The status of the initialization. If everything went fine, return STATUS_OK
 */
MUMBLE_PLUGIN_EXPORT mumble_error_t MUMBLE_PLUGIN_CALLING_CONVENTION mumble_init(mumble_plugin_id_t id);

/**
 * Gets called when unloading the plugin in order to allow it to clean up after itself.
 * Note that it is still safe to call API functions from within this callback.
 */
MUMBLE_PLUGIN_EXPORT void MUMBLE_PLUGIN_CALLING_CONVENTION mumble_shutdown();

/**
 * Gets the name of the plugin.
 *
 * NOTE: This function may be called without the plugin being loaded
 *
 * @returns A String-wrapper containing the requested name
 */
MUMBLE_PLUGIN_EXPORT struct MumbleStringWrapper MUMBLE_PLUGIN_CALLING_CONVENTION mumble_getName();

/**
 * Gets the Version of the plugin-API this plugin intends to use.
 * Mumble will decide whether this plugin is loadable or not based on the return value of this function.
 *
 * NOTE: This function may be called without the plugin being loaded
 *
 * @returns The respective API Version
 */
MUMBLE_PLUGIN_EXPORT mumble_version_t MUMBLE_PLUGIN_CALLING_CONVENTION mumble_getAPIVersion();

/**
 * Provides the MumbleAPI struct to the plugin. This struct contains function pointers that can be used
 * to interact with the Mumble client. It is up to the plugin to store this struct somewhere if it wants to make use
 * of it at some point.
 *
 * NOTE: This function may be called without the plugin being loaded
 *
 * @param api A pointer to the MumbleAPI struct. The API struct must be cast to the version corresponding to the
 * user API version. If your plugin is e.g. using the 1.0.x API, then you have to cast this pointer to
 * MumbleAPI_v_1_0_x. Note also that you **must not store this pointer**. It will become invalid. Therefore
 * you have to copy the struct in order to use it later on.
 */
MUMBLE_PLUGIN_EXPORT void MUMBLE_PLUGIN_CALLING_CONVENTION mumble_registerAPIFunctions(void *apiStruct);

/**
 * Releases the resource pointed to by the given pointer. If the respective resource has been allocated before,
 * this would be the time to free/delete it.
 * The resources processed by this functions are only those that have been specifically allocated in order to return
 * them in one of the plugin functions to Mumble (e.g. the String returned by mumble_getName) and has nothing to do
 * with your plugin's internal resource management.
 * In short: Only resources passed from the plugin to Mumble via a return value may be processed by this function.
 *
 * NOTE1: This function may be called without the plugin being loaded
 *
 * NOTE2: that the pointer might be pointing to memory that had to be allocated without the plugin being loaded.
 * Therefore you should be very sure that there'll be another callback in which you want to free this memory,
 * should you decide to not do it here (which is hereby explicitly advised against).
 *
 * NOTE3: The pointer is const as Mumble won't mess with the memory allocated by the plugin (no modifications).
 * Nontheless this function is explicitly responsible for freeing the respective memory parts. If the memory has
 * been allocated using malloc(), it needs to be freed using free() which requires a const-cast. If however the
 * memory has been created using the new operator you have to cast the pointer back to its original type and then
 * use the  delete operator on it (no const-cast necessary in this case).
 * See https://stackoverflow.com/questions/2819535/unable-to-free-const-pointers-in-c
 * and https://stackoverflow.com/questions/941832/is-it-safe-to-delete-a-void-pointer
 *
 * @param pointer The pointer to the memory that needs free-ing
 */
MUMBLE_PLUGIN_EXPORT void MUMBLE_PLUGIN_CALLING_CONVENTION mumble_releaseResource(const void *pointer);



// >>>>>>>>>>>>>>>>>>>> GENERAL functions <<<<<<<<<<<<<<<<<<<<

/**
 * Tells the plugin some basic information about the Mumble client loading it.
 * This function will be the first one that is being called on this plugin - even before it is decided whether to load
 * the plugin at all.
 *
 * @param mumbleVersion The Version of the Mumble client
 * @param mumbleAPIVersion The Version of the plugin-API the Mumble client runs with
 * @param minimumExpectedAPIVersion The minimum Version the Mumble clients expects this plugin to meet in order to load
 * it
 */
MUMBLE_PLUGIN_EXPORT void MUMBLE_PLUGIN_CALLING_CONVENTION mumble_setMumbleInfo(
	mumble_version_t mumbleVersion, mumble_version_t mumbleAPIVersion, mumble_version_t minimumExpectedAPIVersion);

/**
 * Gets the Version of this plugin
 *
 * NOTE: This function may be called without the plugin being loaded
 *
 * @returns The plugin's version
 */
MUMBLE_PLUGIN_EXPORT mumble_version_t MUMBLE_PLUGIN_CALLING_CONVENTION mumble_getVersion();

/**
 * Gets the name of the plugin author(s).
 *
 * NOTE: This function may be called without the plugin being loaded
 *
 * @returns A String-wrapper containing the requested author name(s)
 */
MUMBLE_PLUGIN_EXPORT struct MumbleStringWrapper MUMBLE_PLUGIN_CALLING_CONVENTION mumble_getAuthor();

/**
 * Gets the description of the plugin.
 *
 * NOTE: This function may be called without the plugin being loaded
 *
 * @returns A String-wrapper containing the requested description
 */
MUMBLE_PLUGIN_EXPORT struct MumbleStringWrapper MUMBLE_PLUGIN_CALLING_CONVENTION mumble_getDescription();

/**
 * Gets the feature set of this plugin. The feature set is described by bitwise or'ing the elements of the
 * Mumble_PluginFeature enum together.
 *
 * NOTE: This function may be called without the plugin being loaded
 *
 * @returns The feature set of this plugin
 */
MUMBLE_PLUGIN_EXPORT uint32_t MUMBLE_PLUGIN_CALLING_CONVENTION mumble_getFeatures();

/**
 * Requests this plugin to deactivate the given (sub)set of provided features.
 * If this is not possible, the features that can't be deactivated shall be returned by this function.
 *
 * Example (check if FEATURE_POSITIONAL shall be deactivated):
 * @code
 * if (features & FEATURE_POSITIONAL) {
 *   // positional shall be deactivated
 * }
 * @endcode
 *
 * @param features The feature set that shall be deactivated
 * @returns The feature set that can't be disabled (bitwise or'ed). If all requested features can be disabled, return
 * FEATURE_NONE. If none of the requested features can be disabled return the unmodified features parameter.
 */
MUMBLE_PLUGIN_EXPORT uint32_t MUMBLE_PLUGIN_CALLING_CONVENTION mumble_deactivateFeatures(uint32_t features);


// >>>>>>>>>>>>>>>>>>>> POSITIONAL DATA functions <<<<<<<<<<<<<<<<<<<<

/*
 * If this plugin wants to provide positional data, the mumble_initPositionalData, mumble_fetchPositionalData
 * and mumble_shutdownPositionalData functions have to be implemented together (implementing only a subset
 * will yield the same result as if no support for positional data was implemened).
 */

/**
 * Indicates that Mumble wants to use this plugin to request positional data. Therefore it should check whether it is
 * currently able to do so and allocate memory that is needed for that process. As a parameter this function gets an
 * array of names and an array of PIDs. They are of same length and the PID at index i belongs to a program whose name
 * is listed at index i in the "name-array".
 *
 * @param programNames An array of pointers to the program names
 * @param programPIDs An array of the corresponding program PIDs
 * @param programCount The length of programNames and programPIDs
 * @returns The error code. If everything went fine PDEC_OK shall be returned. In that case Mumble will start
 * frequently calling fetchPositionalData. If this returns anything but PDEC_OK, Mumble will assume that the plugin is
 * (currently) uncapable of providing positional data. In this case this function must not have allocated any memory
 * that needs to be cleaned up later on. Depending on the returned error code, Mumble might try to call this function
 * again at some point.
 */
MUMBLE_PLUGIN_EXPORT uint8_t MUMBLE_PLUGIN_CALLING_CONVENTION mumble_initPositionalData(const char *const *programNames,
																						const uint64_t *programPIDs,
																						size_t programCount);

/**
 * Retrieves the positional data. If no data can be fetched, set all float-vectors to 0 and return false.
 *
 * @param[out] avatarPos A float-array of size 3 representing the cartesian position of the player/avatar in the ingame
 * world. One unit represents one meter of distance.
 * @param[out] avatarDir A float-array of size 3 representing the cartesian direction-vector of the player/avatar
 * ingame (where it is facing).
 * @param[out] avatarAxis A float-array of size 3 representing the vector pointing from the toes of the character to
 * its head. One unit represents one meter of distance.
 * @param[out] cameraPos A float-array of size 3 representing the cartesian position of the camera in the ingame world.
 * One unit represents one meter of distance.
 * @param[out] cameraDir A float-array of size 3 representing the cartesian direction-vector of the camera ingame
 * (where it is facing).
 * @param[out] cameraAxis A float-array of size 3 representing a vector from the bottom of the camera to its top. One
 * unit represents one meter of distance.
 * @param[out] context A pointer to where the pointer to a C-encoded string storing the context of the provided
 * positional data shall be written. This context should include information about the server (and team) the player is
 * on. Only players with identical context will be able to hear each other's audio. The returned pointer has to remain
 * valid until the next invokation of this function or until shutdownPositionalData is called.
 * @param[out] identity A pointer to where the pointer to a C-encoded string storing the identity of the player shall
 * be written. It can be polled by external scripts from the server and should uniquely identify the player in the
 * game. The pointer has to remain valid until the next invokation of this function or until shutdownPositionalData is
 * called.
 * @returns Whether this plugin can continue delivering positional data. If this function returns false,
 * shutdownPositionalData will be called.
 */
MUMBLE_PLUGIN_EXPORT bool MUMBLE_PLUGIN_CALLING_CONVENTION
	mumble_fetchPositionalData(float *avatarPos, float *avatarDir, float *avatarAxis, float *cameraPos,
							   float *cameraDir, float *cameraAxis, const char **context, const char **identity);

/**
 * Indicates that this plugin will not be asked for positional data any longer. Thus any memory allocated for this
 * purpose should be freed at this point.
 */
MUMBLE_PLUGIN_EXPORT void MUMBLE_PLUGIN_CALLING_CONVENTION mumble_shutdownPositionalData();

/**
 * The context in positional data is used to determine whether different positional data sets from different
 * clients belong to the same game (and same server). Only if the contexts matches up across these clients,
 * will Mumble activate the positional audio effects, as it will assume that these clients are playing the
 * same game together.
 * The context is set during fetching of the other positional data and is usually something like e.g. the
 * current server's name. In order to avoid clashes between different plugins (that most likely work for
 * different games), the context is prefixed by Mumble. If this function is not implemented, the name of
 * the plugin is used as a prefix (which tends to be the supported game's name), but sometimes a different
 * prefix is desirable. For these cases, a custom prefix can be provided through this function.
 *
 * NOTE that while it is possible to allocate a string for this purpose every time this function is called
 * and then letting mumble release the resource again (via mumble_releaseResource), it is generally not the
 * advised way of doing things (it may impact overall performance negatively, since this function will be
 * called very frequently). Instead you should either return a static string (if your language supports that
 * and if it actually fits your needs) or you should allocate a string during mumble_initPositionalData and
 * free it in mumble_shutdownPositionalData and when returning the string in this function, tell Mumble that
 * the string does not need releasing.
 *
 * @returns The context prefix to use for positional data fetched by this plugin.
 *
 * @since Plugin interface v1.1.0
 */
MUMBLE_PLUGIN_EXPORT struct MumbleStringWrapper MUMBLE_PLUGIN_CALLING_CONVENTION
	mumble_getPositionalDataContextPrefix();



// >>>>>>>>>>>>>>>>>>>> EVENTHANDLER / CALLBACK functions <<<<<<<<<<<<<<<<<<<<

/**
 * Called when connecting to a server.
 * Note that in most cases you'll want to use mumble_onServerSynchronized instead.
 * Note also that this callback will be called from a DIFFERENT THREAD!
 *
 * @param connection The ID of the newly established server-connection
 */
MUMBLE_PLUGIN_EXPORT void MUMBLE_PLUGIN_CALLING_CONVENTION mumble_onServerConnected(mumble_connection_t connection);

/**
 * Called when disconnecting from a server.
 * Note that this callback is called from a DIFFERENT THREAD!
 *
 * @param connection The ID of the server-connection that has been terminated
 */
MUMBLE_PLUGIN_EXPORT void MUMBLE_PLUGIN_CALLING_CONVENTION mumble_onServerDisconnected(mumble_connection_t connection);

/**
 * Called when the client has finished synchronizing with the server
 *
 * @param connection The ID of the server-connection that has been terminated
 */
MUMBLE_PLUGIN_EXPORT void MUMBLE_PLUGIN_CALLING_CONVENTION mumble_onServerSynchronized(mumble_connection_t connection);

/**
 * Called whenever any user on the server enters a channel
 * This function will also be called when freshly connecting to a server as each user on that
 * server needs to be "added" to the respective channel as far as the local client is concerned.
 *
 * @param connection The ID of the server-connection this event is connected to
 * @param userID The ID of the user this event has been triggered for
 * @param previousChannelID The ID of the chanel the user is coming from. Negative IDs indicate that there is no
 * previous channel (e.g. the user freshly connected to the server) or the channel isn't available because of any other
 * reason.
 * @param newChannelID The ID of the channel the user has entered. If the ID is negative, the new channel could not be
 * retrieved. This means that the ID is invalid.
 */
MUMBLE_PLUGIN_EXPORT void MUMBLE_PLUGIN_CALLING_CONVENTION mumble_onChannelEntered(mumble_connection_t connection,
																				   mumble_userid_t userID,
																				   mumble_channelid_t previousChannelID,
																				   mumble_channelid_t newChannelID);

/**
 * Called whenever a user leaves a channel.
 * This includes a client disconnecting from the server as this will also lead to the user not being in that channel
 * anymore.
 *
 * @param connection The ID of the server-connection this event is connected to
 * @param userID The ID of the user that left the channel
 * @param channelID The ID of the channel the user left. If the ID is negative, the channel could not be retrieved.
 * This means that the ID is invalid.
 */
MUMBLE_PLUGIN_EXPORT void MUMBLE_PLUGIN_CALLING_CONVENTION mumble_onChannelExited(mumble_connection_t connection,
																				  mumble_userid_t userID,
																				  mumble_channelid_t channelID);

/**
 * Called when any user changes his/her talking state.
 *
 * @param connection The ID of the server-connection this event is connected to
 * @param userID The ID of the user whose talking state has been changed
 * @param talkingState The new TalkingState the user has switched to.
 */
MUMBLE_PLUGIN_EXPORT void MUMBLE_PLUGIN_CALLING_CONVENTION mumble_onUserTalkingStateChanged(
	mumble_connection_t connection, mumble_userid_t userID, mumble_talking_state_t talkingState);

/**
 * Called whenever there is audio input.
 * Note that this callback will be called from the AUDIO THREAD.
 * Note also that blocking this callback will cause Mumble's audio processing to get suspended.
 *
 * @param inputPCM A pointer to a short-array holding the pulse-code-modulation (PCM) representing the audio input. Its
 * length is sampleCount * channelCount. The PCM format for stereo input is [LRLRLR...] where L and R are samples of
 * the left and right channel respectively.
 * @param sampleCount The amount of sample points per channel
 * @param channelCount The amount of channels in the audio
 * @param sampleRate The used sample rate in Hz
 * @param isSpeech A boolean flag indicating whether Mumble considers the input as part of speech (instead of
 * background noise)
 * @returns Whether this callback has modified the audio input-array
 */
MUMBLE_PLUGIN_EXPORT bool MUMBLE_PLUGIN_CALLING_CONVENTION mumble_onAudioInput(short *inputPCM, uint32_t sampleCount,
																			   uint16_t channelCount,
																			   uint32_t sampleRate, bool isSpeech);

/**
 * Called whenever Mumble fetches data from an active audio source (could be a voice packet or a playing sample).
 * The provided audio buffer is the raw buffer without any processing applied to it yet.
 * Note that this callback will be called from the AUDIO THREAD.
 * Note also that blocking this callback will cause Mumble's audio processing to get suspended.
 *
 * @param outputPCM A pointer to a float-array holding the pulse-code-modulation (PCM) representing the audio output.
 * Its length is sampleCount * channelCount. The PCM format for stereo output is [LRLRLR...] where L and R are samples
 * of the left and right channel respectively.
 * @param sampleCount The amount of sample points per channel
 * @param channelCount The amount of channels in the audio
 * @param sampleRate The used sample rate in Hz
 * @param isSpeech Whether this audio belongs to a received voice packet (and will thus (most likely) contain speech)
 * @param userID If isSpeech is true, this contains the ID of the user this voice packet belongs to. If isSpeech is
 * false, the content of this parameter is unspecified and should not be accessed
 * @returns Whether this callback has modified the audio output-array
 */
MUMBLE_PLUGIN_EXPORT bool MUMBLE_PLUGIN_CALLING_CONVENTION
	mumble_onAudioSourceFetched(float *outputPCM, uint32_t sampleCount, uint16_t channelCount, uint32_t sampleRate,
								bool isSpeech, mumble_userid_t userID);

/**
 * Called whenever the fully mixed and processed audio is about to be handed to the audio backend (about to be played).
 * Note that this happens immediately before Mumble clips the audio buffer.
 * Note that this callback will be called from the AUDIO THREAD.
 * Note also that blocking this callback will cause Mumble's audio processing to get suspended.
 *
 * @param outputPCM A pointer to a float-array holding the pulse-code-modulation (PCM) representing the audio output.
 * Its length is sampleCount * channelCount. The PCM format for stereo output is [LRLRLR...] where L and R are samples
 * of the left and right channel respectively.
 * @param sampleCount The amount of sample points per channel
 * @param channelCount The amount of channels in the audio
 * @param sampleRate The used sample rate in Hz
 * @returns Whether this callback has modified the audio output-array
 */
MUMBLE_PLUGIN_EXPORT bool MUMBLE_PLUGIN_CALLING_CONVENTION mumble_onAudioOutputAboutToPlay(float *outputPCM,
																						   uint32_t sampleCount,
																						   uint16_t channelCount,
																						   uint32_t sampleRate);

/**
 * Called whenever data has been received that has been sent by a plugin. This data should only be processed by the
 * intended plugin. For this reason a dataID is provided that should be used to determine whether the data is intended
 * for this plugin or not. As soon as the data has been processed, no further plugins will be notified about it.
 *
 * @param connection The ID of the server-connection the data is coming from
 * @param sender The ID of the user whose client's plugin has sent the data
 * @param data The sent data array. This can be an arbitrary sequence of bytes.
 * @param dataLength The length of the data array
 * @param dataID The ID of this data (C-encoded)
 * @return Whether the given data has been processed by this plugin
 */
MUMBLE_PLUGIN_EXPORT bool MUMBLE_PLUGIN_CALLING_CONVENTION mumble_onReceiveData(mumble_connection_t connection,
																				mumble_userid_t sender,
																				const uint8_t *data, size_t dataLength,
																				const char *dataID);

/**
 * Called when a new user gets added to the user model. This is the case when that new user freshly connects to the
 * server the local user is on but also when the local user connects to a server other clients are already connected to
 * (in this case this method will be called for every client already on that server).
 *
 * @param connection An object used to identify the current connection
 * @param userID The ID of the user that has been added
 */

MUMBLE_PLUGIN_EXPORT void MUMBLE_PLUGIN_CALLING_CONVENTION mumble_onUserAdded(mumble_connection_t connection,
																			  mumble_userid_t userID);

/**
 * Called when a user gets removed from the user model. This is the case when that user disconnects from the server the
 * local user is on but also when the local user disconnects from a server other clients are connected to (in this case
 * this method will be called for every client on that server).
 *
 * @param connection An object used to identify the current connection
 * @param userID The ID of the user that has been removed
 */
MUMBLE_PLUGIN_EXPORT void MUMBLE_PLUGIN_CALLING_CONVENTION mumble_onUserRemoved(mumble_connection_t connection,
																				mumble_userid_t userID);

/**
 * Called when a new channel gets added to the user model. This is the case when a new channel is created on the server
 * the local user is on but also when the local user connects to a server that contains channels other than the
 * root-channel (in this case this method will be called for ever non-root channel on that server).
 *
 * @param connection An object used to identify the current connection
 * @param channelID The ID of the channel that has been added
 */
MUMBLE_PLUGIN_EXPORT void MUMBLE_PLUGIN_CALLING_CONVENTION mumble_onChannelAdded(mumble_connection_t connection,
																				 mumble_channelid_t channelID);

/**
 * Called when a channel gets removed from the user model. This is the case when a channel is removed on the server the
 * local user is on but also when the local user disconnects from a server that contains channels other than the
 * root-channel (in this case this method will be called for ever non-root channel on that server).
 *
 * @param connection An object used to identify the current connection
 * @param channelID The ID of the channel that has been removed
 */
MUMBLE_PLUGIN_EXPORT void MUMBLE_PLUGIN_CALLING_CONVENTION mumble_onChannelRemoved(mumble_connection_t connection,
																				   mumble_channelid_t channelID);

/**
 * Called when a channel gets renamed. This also applies when a new channel is created (thus assigning it an initial
 * name is also considered renaming).
 *
 * @param connection An object used to identify the current connection
 * @param channelID The ID of the channel that has been renamed
 */
MUMBLE_PLUGIN_EXPORT void MUMBLE_PLUGIN_CALLING_CONVENTION mumble_onChannelRenamed(mumble_connection_t connection,
																				   mumble_channelid_t channelID);

/**
 * Called when a key has been pressed or released while Mumble has keyboard focus.
 * Note that this callback will only work if the user has explicitly given permission to monitor keyboard
 * events for this plugin. Thus if you want to use this callback, make sure your users know that they have to
 * enable that.
 *
 * @param keyCode The key code of the respective key. The character codes are defined
 * via the Mumble_KeyCode enum. For printable 7-bit ASCII characters these codes conform
 * to the ASCII code-page with the only difference that case is not distinguished. Therefore
 * always the upper-case letter code will be used for letters.
 * @param wasPres Whether the respective key has been pressed (instead of released)
 */
MUMBLE_PLUGIN_EXPORT void MUMBLE_PLUGIN_CALLING_CONVENTION mumble_onKeyEvent(uint32_t keyCode, bool wasPress);



// >>>>>>>>>>>>>>>>>>>> PLUGIN UPDATE functions <<<<<<<<<<<<<<<<<<<<

/**
 * This function is used to determine whether the plugin can find an update for itself that is available for download.
 *
 * NOTE: This function may be called without the plugin being loaded
 *
 * @return Whether the plugin was able to find an update for itself
 */
MUMBLE_PLUGIN_EXPORT bool MUMBLE_PLUGIN_CALLING_CONVENTION mumble_hasUpdate();

/**
 * This function is used to retrieve the URL for downloading the newer/updated version of this plugin.
 *
 * NOTE: This function may be called without the plugin being loaded
 *
 * @returns A String-wrapper containing the requested URL
 */
MUMBLE_PLUGIN_EXPORT struct MumbleStringWrapper MUMBLE_PLUGIN_CALLING_CONVENTION mumble_getUpdateDownloadURL();



// >>>>>>>>>>>>>>>>>>>> DEFAULT functions <<<<<<<<<<<<<<<<<<<<
// These functions don't have to be implemented by you

/**
 * Gets the version of the plugin functions interface that this plugin uses or wants to use.
 * Based on this, Mumble will decide what kind of functions to look for and load from your plugin.
 */
MUMBLE_PLUGIN_EXPORT mumble_version_t mumble_getPluginFunctionsVersion();

MUMBLE_EXTERN_C_END
#endif // EXTERNAL_MUMBLE_PLUGIN_FUNCTIONS_


///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// DEFAULT IMPLEMENTATIONS /////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef EXTERNAL_MUMBLE_PLUGIN_DEFAULT_IMPLEMENTATIONS_
#	define EXTERNAL_MUMBLE_PLUGIN_DEFAULT_IMPLEMENTATIONS_

mumble_version_t mumble_getPluginFunctionsVersion() {
	return MUMBLE_PLUGIN_FUNCTIONS_VERSION;
}

#endif // EXTERNAL_MUMBLE_PLUGIN_DEFAULT_IMPLEMENTATIONS_



///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////// Mumble API ///////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef EXTERNAL_MUMBLE_PLUGIN_MUMBLE_API_
#	define EXTERNAL_MUMBLE_PLUGIN_MUMBLE_API_

// Define a struct name that contains the respective version number
#	undef MUMBLE_API_STRUCT_NAME
#	define MUMBLE_API_STRUCT_NAME                                                                                     \
		MUMBLE_ECONCAT(                                                                                                \
			MUMBLE_ECONCAT(MUMBLE_ECONCAT(MumbleAPI_v, MUMBLE_PLUGIN_API_MAJOR_MACRO), MUMBLE_PLUGIN_API_MINOR_MACRO), \
			x)

#	undef MUMBLE_API_CAST
#	define MUMBLE_API_CAST(ptr) (*((MUMBLE_API_STRUCT_NAME *) ptr))

// Define some helper macros to make version-specific changes to the API functions
#	define SELECTED_API_VERSION                                                                  \
		MUMBLE_PLUGIN_VERSION_CHECK(MUMBLE_PLUGIN_API_MAJOR_MACRO, MUMBLE_PLUGIN_API_MINOR_MACRO, \
									MUMBLE_PLUGIN_API_PATCH_MACRO)
#	if SELECTED_API_VERSION >= MUMBLE_PLUGIN_VERSION_CHECK(1, 2, 0)
#		define PARAM_v1_2(arg) , arg
#	else
#		define PARAM_v1_2(arg)
#	endif

struct MUMBLE_API_STRUCT_NAME {
	/*
	 * GENERAL NOTES
	 *
	 * All functions that take in a connection as a parameter may only be called **after** the connection
	 * has finished synchronizing. The only exception from this is isConnectionSynchronized.
	 *
	 * Strings returned by the API are UTF-8 encoded
	 * Strings passed to the API are expected to be UTF-8 encoded
	 *
	 * All API functions are synchronized and will be executed in Mumble's "main thread" from which most plugin
	 * callbacks are called as well. Note however that an API call is BLOCKING if invoked from a different
	 * thread. This means that they can cause deadlocks if used without caution. An example that will lead
	 * to a deadlock is:
	 * - plugin callback gets called from the main thread
	 * - callback messages a separate thread to do something and waits for the action to have completed
	 * - Separate thread calls an API function
	 * - The function blocks and waits to be executed in the main thread which is currently blocked waiting
	 * - deadlock
	 */


	// -------- Memory management --------

	/**
	 * Frees the given pointer.
	 *
	 * @param callerID The ID of the plugin calling this function
	 * @param pointer The pointer to free
	 * @returns The error code. If everything went well, STATUS_OK will be returned.
	 */
	mumble_error_t(MUMBLE_PLUGIN_CALLING_CONVENTION *freeMemory)(mumble_plugin_id_t callerID, const void *pointer);



	// -------- Getter functions --------

	/**
	 * Gets the connection ID of the server the user is currently active on (the user's audio output is directed at).
	 *
	 * @param callerID The ID of the plugin calling this function
	 * @param[out] connection A pointer to the memory location the ID should be written to
	 * @returns The error code. If everything went well, STATUS_OK will be returned. Only then it is valid to access
	 * the value of the provided pointer
	 */
	mumble_error_t(MUMBLE_PLUGIN_CALLING_CONVENTION *getActiveServerConnection)(mumble_plugin_id_t callerID,
																				mumble_connection_t *connection);

	/**
	 * Checks whether the given connection has finished initializing yet.
	 *
	 * @param callerID The ID of the plugin calling this function
	 * @param connection The ID of the server-connection to use as a context
	 * @param[out] A pointer to the boolean variable that'll hold the info whether the server has finished
	 * synchronization yet after this function has executed successfully.
	 * @returns The error code. If everything went well, STATUS_OK will be returned. Only then the passed pointer
	 * may be accessed
	 */
	mumble_error_t(MUMBLE_PLUGIN_CALLING_CONVENTION *isConnectionSynchronized)(mumble_plugin_id_t callerID,
																			   mumble_connection_t connection,
																			   bool *synchronized);

	/**
	 * Fills in the information about the local user.
	 *
	 * @param callerID The ID of the plugin calling this function
	 * @param connection The ID of the server-connection to use as a context
	 * @param[out] userID A pointer to the memory the user's ID shall be written to
	 * @returns The error code. If everything went well, STATUS_OK will be returned. Only then the passed pointer
	 * may be accessed
	 */
	mumble_error_t(MUMBLE_PLUGIN_CALLING_CONVENTION *getLocalUserID)(mumble_plugin_id_t callerID,
																	 mumble_connection_t connection,
																	 mumble_userid_t *userID);

	/**
	 * Fills in the information about the given user's name.
	 *
	 * @param callerID The ID of the plugin calling this function
	 * @param connection The ID of the server-connection to use as a context
	 * @param userID The user's ID whose name should be obtained
	 * @param[out] userName A pointer to where the pointer to the allocated string (C-encoded) should be written to.
	 * The allocated memory has to be freed by a call to freeMemory by the plugin eventually. The memory will only be
	 * allocated if this function returns STATUS_OK.
	 * @returns The error code. If everything went well, STATUS_OK will be returned. Only then the passed pointer
	 * may be accessed
	 */
	mumble_error_t(MUMBLE_PLUGIN_CALLING_CONVENTION *getUserName)(mumble_plugin_id_t callerID,
																  mumble_connection_t connection,
																  mumble_userid_t userID, const char **userName);

	/**
	 * Fills in the information about the given channel's name.
	 *
	 * @param callerID The ID of the plugin calling this function
	 * @param connection The ID of the server-connection to use as a context
	 * @param channelID The channel's ID whose name should be obtained
	 * @param[out] channelName A pointer to where the pointer to the allocated string (C-ecoded) should be written to.
	 * The allocated memory has to be freed by a call to freeMemory by the plugin eventually. The memory will only be
	 * allocated if this function returns STATUS_OK.
	 * @returns The error code. If everything went well, STATUS_OK will be returned. Only then the passed pointer
	 * may be accessed
	 */
	mumble_error_t(MUMBLE_PLUGIN_CALLING_CONVENTION *getChannelName)(mumble_plugin_id_t callerID,
																	 mumble_connection_t connection,
																	 mumble_channelid_t channelID,
																	 const char **channelName);

	/**
	 * Gets an array of all users that are currently connected to the provided server. Passing a nullptr as any of the
	 * out-parameter will prevent that property to be set/allocated. If you are only interested in the user count you
	 * can thus pass nullptr as the users parameter and save time on allocating + freeing the channels-array while
	 * still getting the size out.
	 *
	 * @param callerID The ID of the plugin calling this function
	 * @param connection The ID of the server-connection to use as a context
	 * @param[out] users A pointer to where the pointer of the allocated array shall be written. The
	 * allocated memory has to be freed by a call to freeMemory by the plugin eventually. The memory will only be
	 * allocated if this function returns STATUS_OK.
	 * @param[out] userCount A pointer to where the size of the allocated user-array shall be written to
	 * @returns The error code. If everything went well, STATUS_OK will be returned. Only then the passed pointer
	 * may be accessed
	 */
	mumble_error_t(MUMBLE_PLUGIN_CALLING_CONVENTION *getAllUsers)(mumble_plugin_id_t callerID,
																  mumble_connection_t connection,
																  mumble_userid_t **users, size_t *userCount);

	/**
	 * Gets an array of all channels on the provided server. Passing a nullptr as any of the out-parameter will prevent
	 * that property to be set/allocated. If you are only interested in the channel count you can thus pass nullptr as
	 * the channels parameter and save time on allocating + freeing the channels-array while still getting the size
	 * out.
	 *
	 * @param callerID The ID of the plugin calling this function
	 * @param connection The ID of the server-connection to use as a context
	 * @param[out] channels A pointer to where the pointer of the allocated array shall be written. The
	 * allocated memory has to be freed by a call to freeMemory by the plugin eventually. The memory will only be
	 * allocated if this function returns STATUS_OK.
	 * @param[out] channelCount A pointer to where the size of the allocated channel-array shall be written to
	 * @returns The error code. If everything went well, STATUS_OK will be returned. Only then the passed pointer
	 * may be accessed
	 */
	mumble_error_t(MUMBLE_PLUGIN_CALLING_CONVENTION *getAllChannels)(mumble_plugin_id_t callerID,
																	 mumble_connection_t connection,
																	 mumble_channelid_t **channels,
																	 size_t *channelCount);

	/**
	 * Gets the ID of the channel the given user is currently connected to.
	 *
	 * @param callerID The ID of the plugin calling this function
	 * @param connection The ID of the server-connection to use as a context
	 * @param userID The ID of the user to search for
	 * @param[out] A pointer to where the ID of the channel shall be written
	 * @returns The error code. If everything went well, STATUS_OK will be returned. Only then the passed pointer
	 * may be accessed
	 */
	mumble_error_t(MUMBLE_PLUGIN_CALLING_CONVENTION *getChannelOfUser)(mumble_plugin_id_t callerID,
																	   mumble_connection_t connection,
																	   mumble_userid_t userID,
																	   mumble_channelid_t *channel);

	/**
	 * Gets an array of all users in the specified channel.
	 *
	 * @param callerID The ID of the plugin calling this function
	 * @param connection The ID of the server-connection to use as a context
	 * @param channelID The ID of the channel whose users shall be retrieved
	 * @param[out] userList A pointer to where the pointer of the allocated array shall be written. The allocated
	 * memory has to be freed by a call to freeMemory by the plugin eventually. The memory will only be allocated if
	 * this function returns STATUS_OK.
	 * @param[out] userCount A pointer to where the size of the allocated user-array shall be written to
	 * @returns The error code. If everything went well, STATUS_OK will be returned. Only then the passed pointer
	 * may be accessed
	 */
	mumble_error_t(MUMBLE_PLUGIN_CALLING_CONVENTION *getUsersInChannel)(mumble_plugin_id_t callerID,
																		mumble_connection_t connection,
																		mumble_channelid_t channelID,
																		mumble_userid_t **userList, size_t *userCount);

	/**
	 * Gets the current transmission mode of the local user.
	 *
	 * @param callerID The ID of the plugin calling this function
	 * @param[out] transmissionMode A pointer to where the transmission mode shall be written.
	 * @returns The error code. If everything went well, STATUS_OK will be returned. Only then the passed pointer
	 * may be accessed
	 */
	mumble_error_t(MUMBLE_PLUGIN_CALLING_CONVENTION *getLocalUserTransmissionMode)(
		mumble_plugin_id_t callerID, mumble_transmission_mode_t *transmissionMode);

	/**
	 * Checks whether the given user is currently locally muted.
	 *
	 * @param callerID The ID of the plugin calling this function
	 * @param connection The ID of the server-connection to use as a context
	 * @param userID The ID of the user to check for
	 * @param[out] muted A pointer to where the local mute state of that user shall be written
	 * @returns The error code. If everything went well, STATUS_OK will be returned. Only then the passed pointer
	 * may be accessed
	 */
	mumble_error_t(MUMBLE_PLUGIN_CALLING_CONVENTION *isUserLocallyMuted)(mumble_plugin_id_t callerID,
																		 mumble_connection_t connection,
																		 mumble_userid_t userID, bool *muted);

	/**
	 * Checks whether the local user is currently muted.
	 *	/// @param callerID The ID of the plugin calling this function
	 * @param[out] muted A pointer to where the mute state of the local user shall be written
	 * @returns The error code. If everything went well, STATUS_OK will be returned. Only then the passed pointer
	 * may be accessed
	 */
	mumble_error_t(MUMBLE_PLUGIN_CALLING_CONVENTION *isLocalUserMuted)(mumble_plugin_id_t callerID, bool *muted);

	/**
	 * Checks whether the local user is currently deafened.
	 *
	 * @param callerID The ID of the plugin calling this function
	 * @param[out] deafened A pointer to where the deaf state of the local user shall be written
	 * @returns The error code. If everything went well, STATUS_OK will be returned. Only then the passed pointer
	 * may be accessed
	 */
	mumble_error_t(MUMBLE_PLUGIN_CALLING_CONVENTION *isLocalUserDeafened)(mumble_plugin_id_t callerID, bool *deafened);

	/**
	 * Gets the hash of the given user (can be used to recognize users between restarts)
	 *
	 * @param callerID The ID of the plugin calling this function
	 * @param connection The ID of the server-connection to use as a context
	 * @param userID The ID of the user to search for
	 * @param[out] hash A pointer to where the pointer to the allocated string (C-encoded) should be written to. The
	 * allocated memory has to be freed by a call to freeMemory by the plugin eventually. The memory will only be
	 * allocated if this function returns STATUS_OK.
	 * @returns The error code. If everything went well, STATUS_OK will be returned. Only then the passed pointer
	 * may be accessed
	 */
	mumble_error_t(MUMBLE_PLUGIN_CALLING_CONVENTION *getUserHash)(mumble_plugin_id_t callerID,
																  mumble_connection_t connection,
																  mumble_userid_t userID, const char **hash);

	/**
	 * Gets the hash of the server for the given connection (can be used to recognize servers between restarts)
	 *
	 * @param callerID The ID of the plugin calling this function
	 * @param connection The ID of the server-connection
	 * @param[out] hash A pointer to where the pointer to the allocated string (C-encoded) should be written to. The
	 * allocated memory has to be freed by a call to freeMemory by the plugin eventually. The memory will only be
	 * allocated if this function returns STATUS_OK.
	 * @returns The error code. If everything went well, STATUS_OK will be returned. Only then the passed pointer
	 * may be accessed
	 */
	mumble_error_t(MUMBLE_PLUGIN_CALLING_CONVENTION *getServerHash)(mumble_plugin_id_t callerID,
																	mumble_connection_t connection, const char **hash);

	/**
	 * Gets the comment of the given user. Note that a user might have a comment configured that hasn't been
	 * synchronized to this client yet. In this case this function will return EC_UNSYNCHRONIZED_BLOB. As of now there
	 * is now way to request the synchronization to happen via the Plugin-API.
	 *
	 * @param callerID The ID of the plugin calling this function
	 * @param connection The ID of the server-connection
	 * @param userID the ID of the user whose comment should be obtained
	 * @param[out] comment A pointer to where the pointer to the allocated string (C-encoded) should be written to. The
	 * allocated memory has to be freed by a call to freeMemory by the plugin eventually. The memory will only be
	 * allocated if this function returns STATUS_OK.
	 * @returns The error code. If everything went well, STATUS_OK will be returned. Only then the passed pointer
	 * may be accessed
	 */
	mumble_error_t(MUMBLE_PLUGIN_CALLING_CONVENTION *getUserComment)(mumble_plugin_id_t callerID,
																	 mumble_connection_t connection,
																	 mumble_userid_t userID, const char **comment);

	/**
	 * Gets the description of the given channel. Note that a channel might have a description configured that hasn't
	 * been synchronized to this client yet. In this case this function will return EC_UNSYNCHRONIZED_BLOB. As of now
	 * there is now way to request the synchronization to happen via the Plugin-API.
	 *
	 * @param callerID The ID of the plugin calling this function
	 * @param connection The ID of the server-connection
	 * @param channelID the ID of the channel whose comment should be obtained
	 * @param[out] description A pointer to where the pointer to the allocated string (C-encoded) should be written to.
	 * The allocated memory has to be freed by a call to freeMemory by the plugin eventually. The memory will only be
	 * allocated if this function returns STATUS_OK.
	 * @returns The error code. If everything went well, STATUS_OK will be returned. Only then the passed pointer
	 * may be accessed
	 */
	mumble_error_t(MUMBLE_PLUGIN_CALLING_CONVENTION *getChannelDescription)(mumble_plugin_id_t callerID,
																			mumble_connection_t connection,
																			mumble_channelid_t channelID,
																			const char **description);


	// -------- Request functions --------

	/**
	 * Requests Mumble to set the local user's transmission mode to the specified one. If you only need to temporarily
	 * set the transmission mode to continous, use requestMicrophoneActivationOverwrite instead as this saves you the
	 * work of restoring the previous state afterwards.
	 *
	 * @param callerID The ID of the plugin calling this function
	 * @param transmissionMode The requested transmission mode
	 * @returns The error code. If everything went well, STATUS_OK will be returned.
	 */
	mumble_error_t(MUMBLE_PLUGIN_CALLING_CONVENTION *requestLocalUserTransmissionMode)(
		mumble_plugin_id_t callerID, mumble_transmission_mode_t transmissionMode);

	/**
	 * Requests Mumble to move the given user into the given channel
	 *
	 * @param callerID The ID of the plugin calling this function
	 * @param connection The ID of the server-connection to use as a context
	 * @param userID The ID of the user that shall be moved
	 * @param channelID The ID of the channel to move the user to
	 * @param password The password of the target channel (UTF-8 encoded as a C-string). Pass NULL if the target
	 * channel does not require a password for entering
	 * @returns The error code. If everything went well, STATUS_OK will be returned.
	 */
	mumble_error_t(MUMBLE_PLUGIN_CALLING_CONVENTION *requestUserMove)(mumble_plugin_id_t callerID,
																	  mumble_connection_t connection,
																	  mumble_userid_t userID,
																	  mumble_channelid_t channelID,
																	  const char *password);

	/**
	 * Requests Mumble to overwrite the microphone activation so that the microphone is always on (same as if the user
	 * had chosen the continous transmission mode). If a plugin requests this overwrite, it is responsible for
	 * deactivating the overwrite again once it is no longer required
	 *
	 * @param callerID The ID of the plugin calling this function
	 * @param activate Whether to activate the overwrite (false deactivates an existing overwrite)
	 * @returns The error code. If everything went well, STATUS_OK will be returned.
	 */
	mumble_error_t(MUMBLE_PLUGIN_CALLING_CONVENTION *requestMicrophoneActivationOvewrite)(mumble_plugin_id_t callerID,
																						  bool activate);

	/**
	 * Requests Mumble to set the local mute state of the given client. Note that this only affects the **local** mute
	 * state opposed to a server-mute (client is globally muted by the server) or the client's own mute-state (client
	 * has muted its microphone and thus isn't transmitting any audio). Furthermore it must be noted that muting the
	 * local user with this function does not work (it doesn't make sense). If you try to do so, this function will
	 * fail. In order to make this work, this function will also fail if the server has not finished synchronizing with
	 * the client yet. For muting the local user, use requestLocalUserMute instead.
	 *
	 * @param callerID The ID of the plugin calling this function.
	 * @param connection The ID of the server-connection to use as a context
	 * @param userID The ID of the user that shall be muted
	 * @param muted Whether to locally mute the given client (opposed to unmuting it)
	 * @returns The error code. If everything went well, STATUS_OK will be returned.
	 */
	mumble_error_t(MUMBLE_PLUGIN_CALLING_CONVENTION *requestLocalMute)(mumble_plugin_id_t callerID,
																	   mumble_connection_t connection,
																	   mumble_userid_t userID, bool muted);

	/**
	 * Requests Mumble to set the mute state of the local user. In the UI this is referred to as "self-mute".
	 *
	 * @param callerID The ID of the plugin calling this function.
	 * @param muted Whether to locally mute the local user (opposed to unmuting it)
	 * @returns The error code. If everything went well, STATUS_OK will be returned.
	 */
	mumble_error_t(MUMBLE_PLUGIN_CALLING_CONVENTION *requestLocalUserMute)(mumble_plugin_id_t callerID, bool muted);

	/**
	 * Requests Mumble to set the deaf state of the local user. In the UI this is referred to as "self-deaf".
	 *
	 * @param callerID The ID of the plugin calling this function.
	 * @param deafened Whether to locally deafen the local user (opposed to undeafening it)
	 * @returns The error code. If everything went well, STATUS_OK will be returned.
	 */
	mumble_error_t(MUMBLE_PLUGIN_CALLING_CONVENTION *requestLocalUserDeaf)(mumble_plugin_id_t callerID, bool deafened);

	/**
	 * Sets the comment of the local user
	 *
	 * @param callerID The ID of the plugin calling this function
	 * @param connection The ID of the server-connection
	 * @param comment The new comment to use (C-encoded). A subset of HTML formatting is supported.
	 * @returns The error code. If everything went well, STATUS_OK will be returned. Only then the passed pointer
	 * may be accessed
	 */
	mumble_error_t(MUMBLE_PLUGIN_CALLING_CONVENTION *requestSetLocalUserComment)(mumble_plugin_id_t callerID,
																				 mumble_connection_t connection,
																				 const char *comment);



	// -------- Find functions --------

	/**
	 * Fills in the information about a user with the specified name, if such a user exists. The search is
	 * case-sensitive.
	 *
	 * @param callerID The ID of the plugin calling this function
	 * @param connection The ID of the server-connection to use as a context
	 * @param userName The respective user's name
	 * @param[out] userID A pointer to the memory the user's ID shall be written to
	 * @returns The error code. If everything went well, STATUS_OK will be returned. Only then the passed pointer may
	 * be accessed.
	 */
	mumble_error_t(MUMBLE_PLUGIN_CALLING_CONVENTION *findUserByName)(mumble_plugin_id_t callerID,
																	 mumble_connection_t connection,
																	 const char *userName, mumble_userid_t *userID);

	/**
	 * Fills in the information about a channel with the specified name, if such a channel exists. The search is
	 * case-sensitive.
	 *
	 * @param callerID The ID of the plugin calling this function
	 * @param connection The ID of the server-connection to use as a context
	 * @param channelName The respective channel's name
	 * @param[out] channelID A pointer to the memory the channel's ID shall be written to
	 * @returns The error code. If everything went well, STATUS_OK will be returned. Only then the passed pointer may
	 * be accessed.
	 */
	mumble_error_t(MUMBLE_PLUGIN_CALLING_CONVENTION *findChannelByName)(mumble_plugin_id_t callerID,
																		mumble_connection_t connection,
																		const char *channelName,
																		mumble_channelid_t *channelID);



	// -------- Settings --------

	/**
	 * Fills in the current value of the setting with the given key. Note that this function can only be used for
	 * settings whose value is a bool!
	 *
	 * @param callerID The ID of the plugin calling this function
	 * @param key The key to the desired setting
	 * @param[out] outValue A pointer to the memory the setting's value shall be written to.
	 * @returns The error code. If everything went well, STATUS_OK will be returned. Only then the passed pointer may
	 * be accessed.
	 */
	mumble_error_t(MUMBLE_PLUGIN_CALLING_CONVENTION *getMumbleSetting_bool)(mumble_plugin_id_t callerID,
																			mumble_settings_key_t key, bool *outValue);

	/**
	 * Fills in the current value of the setting with the given key. Note that this function can only be used for
	 * settings whose value is an int!
	 *
	 * @param callerID The ID of the plugin calling this function
	 * @param key The key to the desired setting
	 * @param[out] outValue A pointer to the memory the setting's value shall be written to.
	 * @returns The error code. If everything went well, STATUS_OK will be returned. Only then the passed pointer may
	 * be accessed.
	 */
	mumble_error_t(MUMBLE_PLUGIN_CALLING_CONVENTION *getMumbleSetting_int)(mumble_plugin_id_t callerID,
																		   mumble_settings_key_t key,
																		   int64_t *outValue);

	/**
	 * Fills in the current value of the setting with the given key. Note that this function can only be used for
	 * settings whose value is a double!
	 *
	 * @param callerID The ID of the plugin calling this function
	 * @param key The key to the desired setting
	 * @param[out] outValue A pointer to the memory the setting's value shall be written to.
	 * @returns The error code. If everything went well, STATUS_OK will be returned. Only then the passed pointer may
	 * be accessed.
	 */
	mumble_error_t(MUMBLE_PLUGIN_CALLING_CONVENTION *getMumbleSetting_double)(mumble_plugin_id_t callerID,
																			  mumble_settings_key_t key,
																			  double *outValue);

	/**
	 * Fills in the current value of the setting with the given key. Note that this function can only be used for
	 * settings whose value is a String!
	 *
	 * @param callerID The ID of the plugin calling this function
	 * @param key The key to the desired setting
	 * @param[out] outValue The memory address to which the pointer to the setting's value (the String) will be
	 * written. The allocated memory has to be freed by a call to freeMemory by the plugin eventually. The memory will
	 * only be allocated if this function returns STATUS_OK.
	 * @returns The error code. If everything went well, STATUS_OK will be returned. Only then the passed pointer may
	 * be accessed.
	 */
	mumble_error_t(MUMBLE_PLUGIN_CALLING_CONVENTION *getMumbleSetting_string)(mumble_plugin_id_t callerID,
																			  mumble_settings_key_t key,
																			  const char **outValue);


	/**
	 * Sets the value of the setting with the given key. Note that this function can only be used for settings whose
	 * value is a bool!
	 *
	 * @param callerID The ID of the plugin calling this function
	 * @param key The key to the desired setting
	 * @param value The value that should be set for the given setting
	 * @returns The error code. If everything went well, STATUS_OK will be returned.
	 */
	mumble_error_t(MUMBLE_PLUGIN_CALLING_CONVENTION *setMumbleSetting_bool)(mumble_plugin_id_t callerID,
																			mumble_settings_key_t key, bool value);

	/**
	 * Sets the value of the setting with the given key. Note that this function can only be used for settings whose
	 * value is an int!
	 *
	 * @param callerID The ID of the plugin calling this function
	 * @param key The key to the desired setting
	 * @param value The value that should be set for the given setting
	 * @returns The error code. If everything went well, STATUS_OK will be returned.
	 */
	mumble_error_t(MUMBLE_PLUGIN_CALLING_CONVENTION *setMumbleSetting_int)(mumble_plugin_id_t callerID,
																		   mumble_settings_key_t key, int64_t value);

	/**
	 * Sets the value of the setting with the given key. Note that this function can only be used for settings whose
	 * value is a double!
	 *
	 * @param callerID The ID of the plugin calling this function
	 * @param key The key to the desired setting
	 * @param value The value that should be set for the given setting
	 * @returns The error code. If everything went well, STATUS_OK will be returned.
	 */
	mumble_error_t(MUMBLE_PLUGIN_CALLING_CONVENTION *setMumbleSetting_double)(mumble_plugin_id_t callerID,
																			  mumble_settings_key_t key, double value);

	/**
	 * Sets the value of the setting with the given key. Note that this function can only be used for settings whose
	 * value is a string!
	 *
	 * @param callerID The ID of the plugin calling this function
	 * @param key The key to the desired setting
	 * @param value The value that should be set for the given setting
	 * @returns The error code. If everything went well, STATUS_OK will be returned.
	 */
	mumble_error_t(MUMBLE_PLUGIN_CALLING_CONVENTION *setMumbleSetting_string)(mumble_plugin_id_t callerID,
																			  mumble_settings_key_t key,
																			  const char *value);



	// -------- Miscellaneous --------

	/**
	 * Sends the provided data to the provided client(s). This kind of data can only be received by another plugin
	 * active on that client. The sent data can be seen by any active plugin on the receiving client. Therefore the
	 * sent data must not contain sensitive information or anything else that shouldn't be known by others.
	 *
	 * NOTE: Messages sent via this API function are rate-limited by the server. If the rate-limit is hit, the message
	 * will be dropped without an error message. The rate-limiting is global (e.g. it doesn't matter which plugin sent
	 * the respective messages - they all count to the same limit).
	 * Therefore if you have multiple messages to send, you should consider sending them asynchronously one at a time
	 * with a little delay in between (~1 second).
	 *
	 * @param callerID The ID of the plugin calling this function
	 * @param connection The ID of the server-connection to send the data through (the server the given users are on)
	 * @param users An array of user IDs to send the data to
	 * @param userCount The size of the provided user-array
	 * @param data The data array that shall be sent. This can be an arbitrary sequence of bytes. Note that the size of
	 * is restricted to <= 1KB.
	 * @param dataLength The length of the data array
	 * @param dataID The ID of the sent data. This has to be used by the receiving plugin(s) to figure out what to do
	 * with the data. This has to be a C-encoded String. It is recommended that the ID starts with a plugin-specific
	 * prefix in order to avoid name clashes. Note that the size of this string is restricted to <= 100 bytes.
	 * @returns The error code. If everything went well, STATUS_OK will be returned.
	 */
	mumble_error_t(MUMBLE_PLUGIN_CALLING_CONVENTION *sendData)(mumble_plugin_id_t callerID,
															   mumble_connection_t connection,
															   const mumble_userid_t *users, size_t userCount,
															   const uint8_t *data, size_t dataLength,
															   const char *dataID);

	/**
	 * Logs the given message (typically to Mumble's console). All passed strings have to be UTF-8 encoded.
	 *
	 * @param callerID The ID of the plugin calling this function
	 * @param message The message to log
	 * @returns The error code. If everything went well, STATUS_OK will be returned.
	 */
	mumble_error_t(MUMBLE_PLUGIN_CALLING_CONVENTION *log)(mumble_plugin_id_t callerID, const char *message);

	/**
	 * Plays the provided sample. It uses libsndfile as a backend so the respective file format needs to be supported
	 * by it in order for this to work out (see http://www.mega-nerd.com/libsndfile/).
	 *
	 * @param callerID The ID of the plugin calling this function
	 * @param samplePath The path to the sample that shall be played (UTF-8 encoded)
	 * @param volume The volume multiplier used when playing the sample (for no change use 1.0f)
	 * @returns The error code. If everything went well, STATUS_OK will be returned.
	 */
	mumble_error_t(MUMBLE_PLUGIN_CALLING_CONVENTION *playSample)(mumble_plugin_id_t callerID,
																 const char *samplePath PARAM_v1_2(float volume));
};

#	ifdef MUMBLE_PLUGIN_CREATE_MUMBLE_API_TYPEDEF
/**
 * Typedef for the Mumble API struct for convenient (unversioned) access
 */
typedef struct MUMBLE_API_STRUCT_NAME MumbleAPI;
typedef struct MUMBLE_API_STRUCT_NAME mumble_api_t;
#	endif

#	undef SELECTED_API_VERSION
#	undef PARAM_v1_2

#endif // EXTERNAL_MUMBLE_PLUGIN_MUMBLE_API_
